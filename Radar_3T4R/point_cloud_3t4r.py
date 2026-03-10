"""
IWR6843ISK 3T4R Point Cloud Generator

Reads raw ADC data captured via DCA1000EVM, performs the full radar signal
processing pipeline (Range FFT -> Doppler FFT -> CFAR -> Doppler Compensation
-> 3D Angle Estimation), and saves per-frame point clouds to an NPZ file
compatible with vis_2t4r.py.

Output NPZ schema:
    points : structured array with fields
        x, y, z, range, velocity, azimuth, elevation, power   (all float32)
    frames : structured array with fields
        frame_id (int32), timestamp (float64), num_points (int32), offset (int32)

Usage:
    python point_cloud_3t4r.py --bin <raw_0.bin> [--cfg <.cfg>] [--out <output.npz>]
                               [--save-rd] [--min-range 0.5] [--max-range 6.0]
"""

import sys
from pathlib import Path

_project_root = Path(__file__).resolve().parent.parent.parent
if str(_project_root) not in sys.path:
    sys.path.insert(0, str(_project_root))

import argparse
import time
import numpy as np
from matplotlib import pyplot as plt

from driver.mmwave.radar_config import config_set
from driver.mmwave.mmwave.dataloader import DCA1000
import driver.mmwave.mmwave.dsp as dsp
from driver.mmwave.mmwave.dsp.utils import Window
from driver.mmwave.mmwave.dsp.angle_estimation import doppler_compensation, naive_xyz


# ── Output dtype (must match vis_2t4r.py expectations) ─────────────────────
POINT_DTYPE = np.dtype([
    ('x',         '<f4'),
    ('y',         '<f4'),
    ('z',         '<f4'),
    ('range',     '<f4'),
    ('velocity',  '<f4'),
    ('azimuth',   '<f4'),
    ('elevation', '<f4'),
    ('power',     '<f4'),
])

FRAME_DTYPE = np.dtype([
    ('frame_id',    '<i4'),
    ('timestamp',   '<f8'),
    ('num_points',  '<i4'),
    ('offset',      '<i4'),
])


# ── Utility ────────────────────────────────────────────────────────────────
def _load_raw(bin_path: str, config: dict):
    """Load raw ADC binary and reshape into per-frame ADC cubes."""
    raw = np.fromfile(bin_path, dtype=np.int16)

    samples_per_frame = (config['chirp_num_one_frame_one_tx']
                         * config['tx_num']
                         * config['rx_num']
                         * config['adc_sample_num']
                         * 2)  # IQ
    n_frames = int(raw.shape[0] // samples_per_frame)
    if n_frames == 0:
        raise RuntimeError("Raw data too small for even one frame")

    leftover = raw.shape[0] - n_frames * samples_per_frame
    if leftover:
        print(f"[WARN] Discarding {leftover} trailing samples "
              f"({n_frames} complete frames)")
        raw = raw[:n_frames * samples_per_frame]

    raw = raw.reshape(n_frames, -1)
    adc = np.apply_along_axis(
        DCA1000.organize, 1, raw,
        num_chirps=config['chirp_num_one_frame'],
        num_rx=config['rx_num'],
        num_samples=config['adc_sample_num'],
    )
    return adc  # [frames, chirps, rx, adc_samples]


def _cfar_2d(det_matrix, config):
    """
    2-D CA-CFAR on the Range-Doppler map.

    Returns boolean mask [range_bins, doppler_bins] and noise floor.
    """
    fft2d = det_matrix.astype(np.int64)

    guard_len = 4
    noise_len = 16

    thresh_doppler, noise_doppler = np.apply_along_axis(
        dsp.ca_, axis=0, arr=fft2d.T,
        l_bound=25, guard_len=guard_len, noise_len=noise_len,
    )
    thresh_range, noise_range = np.apply_along_axis(
        dsp.ca_, axis=0, arr=fft2d,
        l_bound=20, guard_len=guard_len, noise_len=noise_len,
    )
    thresh_doppler = thresh_doppler.T
    noise_range = noise_range      # keep for SNR

    mask = (det_matrix > thresh_doppler) & (det_matrix > thresh_range)
    return mask, noise_range


def _process_frame(
    frame, config, *,
    range_resolution, doppler_resolution, center_freq,
    min_range_bin, max_range_bin,
    angle_fft_size,
):
    """
    Full single-frame pipeline.

    Returns:
        points : ndarray of POINT_DTYPE  (may be length-0)
        rd_map : ndarray [range, doppler] – the Range-Doppler map
    """
    tx  = config['tx_num']
    rx  = config['rx_num']
    n_adc = config['adc_sample_num']
    n_chirps = config['chirp_num_one_frame']
    n_chirps_tx = config['chirp_num_one_frame_one_tx']
    ramp_end = config['ramp_end_time']
    idle     = config['idle_time']

    # 1. Range FFT
    radar_cube = dsp.range_processing(frame, window_type_1d=Window.HANNING)

    # 2. Doppler FFT (with clutter removal for human motion)
    _, det_matrix, aoa_input = dsp.doppler_processing(
        radar_cube,
        num_tx_antennas=tx,
        clutter_removal_enabled=True,
        window_type_2d=Window.HANNING,
        interleaved=True,
        accumulate=True,
    )
    # det_matrix : [range_bins, doppler_bins]
    # aoa_input  : [range_bins, virtual_ants, doppler_bins]

    rd_map = det_matrix.copy()

    # 3. 2-D CFAR
    mask, noise_floor = _cfar_2d(det_matrix, config)
    det_idx = np.argwhere(mask)  # [N, 2]  (range_idx, doppler_idx)

    # Reject zero-velocity bins (static clutter)
    moving = det_idx[:, 1] != 0
    det_idx = det_idx[moving]

    # Reject out-of-range detections
    in_range = (det_idx[:, 0] >= min_range_bin) & (det_idx[:, 0] < max_range_bin)
    det_idx = det_idx[in_range]

    if det_idx.shape[0] == 0:
        return np.zeros(0, dtype=POINT_DTYPE), rd_map

    range_idx   = det_idx[:, 0]
    doppler_idx = det_idx[:, 1]
    n_det = len(range_idx)

    peak_vals = det_matrix[range_idx, doppler_idx].astype(np.float32)
    snr = peak_vals - noise_floor[range_idx, doppler_idx].astype(np.float32)

    # 4. Doppler compensation for TDM-MIMO (3TX)
    chirp_time = (ramp_end + idle) * 1e-6
    aoa_compensated = doppler_compensation(aoa_input, times=chirp_time, num_tx=tx, num_rx=rx)

    # 5. Angle estimation (azimuth + elevation via naive_xyz for 3TX)
    azimuth_input = aoa_compensated[range_idx, :, doppler_idx]  # [n_det, virtual_ants]
    x, y, z = naive_xyz(azimuth_input.T, num_tx=tx, num_rx=rx, fft_size=angle_fft_size)

    # Convert to physical coordinates
    ranges = range_idx * range_resolution
    x_m = x * ranges
    y_m = y * ranges
    z_m = z * ranges

    # Velocity
    velocities = np.where(
        doppler_idx >= n_chirps_tx // 2,
        (doppler_idx.astype(np.float64) - n_chirps_tx) * doppler_resolution,
        doppler_idx.astype(np.float64) * doppler_resolution,
    )

    # Azimuth / elevation angles (radians)
    azimuth   = np.arctan2(x_m, y_m)
    elevation = np.arctan2(z_m, np.sqrt(x_m**2 + y_m**2))

    # Build output
    pts = np.zeros(n_det, dtype=POINT_DTYPE)
    pts['x']         = x_m.astype(np.float32)
    pts['y']         = y_m.astype(np.float32)
    pts['z']         = z_m.astype(np.float32)
    pts['range']     = ranges.astype(np.float32)
    pts['velocity']  = velocities.astype(np.float32)
    pts['azimuth']   = azimuth.astype(np.float32)
    pts['elevation'] = elevation.astype(np.float32)
    pts['power']     = snr.astype(np.float32)

    # Filter NaN/Inf (from sqrt of negative in naive_xyz)
    valid = np.isfinite(pts['y']) & np.isfinite(pts['z'])
    pts = pts[valid]

    return pts, rd_map


# ── Main ───────────────────────────────────────────────────────────────────
def main():
    parser = argparse.ArgumentParser(
        description='IWR6843ISK 3T4R Point Cloud Generator',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    parser.add_argument('--bin', type=str, required=True,
                        help='Path to raw ADC binary (*_Raw_0.bin)')
    parser.add_argument('--cfg', type=str,
                        default=str(Path(__file__).resolve().parent / 'configs' / '6843_3tx_highBW.cfg'),
                        help='Path to radar .cfg file')
    parser.add_argument('--out', type=str, default=None,
                        help='Output NPZ path (default: <bin_dir>/pointcloud_3t4r.npz)')
    parser.add_argument('--save-rd', action='store_true',
                        help='Save Range-Doppler maps as PNGs')
    parser.add_argument('--rd-interval', type=int, default=10,
                        help='Save RD map every N frames (default: 10)')
    parser.add_argument('--min-range', type=float, default=0.3,
                        help='Minimum detection range in meters (default: 0.3)')
    parser.add_argument('--max-range', type=float, default=6.0,
                        help='Maximum detection range in meters (default: 6.0)')
    parser.add_argument('--angle-fft', type=int, default=64,
                        help='Angle FFT size (default: 64)')
    args = parser.parse_args()

    # ── Config ──
    config = config_set(args.cfg)
    tx  = config['tx_num']
    rx  = config['rx_num']
    n_adc = config['adc_sample_num']
    n_chirps_tx = config['chirp_num_one_frame_one_tx']
    start_freq  = config['start_freq']
    freq_slope  = config['freq_slope']
    sample_rate = config['sample_rate']
    ramp_end    = config['ramp_end_time']
    idle        = config['idle_time']
    adc_start   = config['adc_start_time']

    assert tx == 3 and rx == 4, f"This script is designed for 3T4R, got {tx}T{rx}R"

    range_res, bandwidth = dsp.range_resolution(
        n_adc, dig_out_sample_rate=sample_rate, freq_slope_const=freq_slope)
    center_freq = start_freq * 1e9 + adc_start * freq_slope * 1e6 + bandwidth / 2
    doppler_res = dsp.doppler_resolution(
        center_freq=center_freq,
        ramp_end_time=ramp_end, idle_time_const=idle,
        num_loops_per_frame=n_chirps_tx, num_tx_antennas=tx,
    )

    min_range_bin = max(1, int(args.min_range / range_res))
    max_range_bin = min(n_adc, int(args.max_range / range_res))

    print("=" * 60)
    print("IWR6843ISK 3T4R Point Cloud Generator")
    print("=" * 60)
    print(f"  Config        : {args.cfg}")
    print(f"  TX={tx}  RX={rx}  Virtual={tx*rx}")
    print(f"  ADC samples   : {n_adc}")
    print(f"  Chirps/TX     : {n_chirps_tx}")
    print(f"  Range res     : {range_res:.4f} m")
    print(f"  Doppler res   : {doppler_res:.4f} m/s")
    print(f"  Detection zone: {args.min_range:.1f} – {args.max_range:.1f} m "
          f"(bins {min_range_bin}–{max_range_bin})")
    print(f"  Angle FFT     : {args.angle_fft}")
    print()

    # ── Load data ──
    print("Loading raw ADC data …")
    adc_data = _load_raw(args.bin, config)
    n_frames = adc_data.shape[0]
    print(f"  {n_frames} frames loaded, shape = {adc_data.shape}")
    print()

    # ── Output dir for RD maps ──
    bin_dir = Path(args.bin).resolve().parent
    if args.save_rd:
        rd_dir = bin_dir / 'rd_maps'
        rd_dir.mkdir(exist_ok=True)
        print(f"  RD maps → {rd_dir}")

    # ── Process ──
    all_points = []
    frame_meta = []
    offset = 0
    t0 = time.time()

    for i in range(n_frames):
        pts, rd_map = _process_frame(
            adc_data[i], config,
            range_resolution=range_res,
            doppler_resolution=doppler_res,
            center_freq=center_freq,
            min_range_bin=min_range_bin,
            max_range_bin=max_range_bin,
            angle_fft_size=args.angle_fft,
        )

        n_pts = len(pts)
        all_points.append(pts)

        meta = np.zeros(1, dtype=FRAME_DTYPE)
        meta['frame_id']   = i
        meta['timestamp']  = i / 15.0  # approximate; real timestamp unavailable
        meta['num_points'] = n_pts
        meta['offset']     = offset
        frame_meta.append(meta)
        offset += n_pts

        # RD map visualisation
        if args.save_rd and (i % args.rd_interval == 0):
            rd_shift = np.fft.fftshift(rd_map, axes=1)
            fig, ax = plt.subplots(figsize=(8, 5))
            extent = [
                -n_chirps_tx // 2 * doppler_res,
                (n_chirps_tx // 2 - 1) * doppler_res,
                0,
                n_adc * range_res,
            ]
            ax.imshow(
                rd_shift, aspect='auto', origin='lower',
                extent=extent, cmap='jet',
            )
            ax.set_xlabel('Velocity (m/s)')
            ax.set_ylabel('Range (m)')
            ax.set_title(f'Range-Doppler Map – Frame {i}')
            fig.tight_layout()
            fig.savefig(rd_dir / f'rd_frame_{i:04d}.png', dpi=100)
            plt.close(fig)

        if (i + 1) % 50 == 0 or i == n_frames - 1:
            elapsed = time.time() - t0
            print(f"  Frame {i+1:>5}/{n_frames}  |  "
                  f"{n_pts:>4} pts  |  "
                  f"{elapsed:.1f}s elapsed")

    # ── Save NPZ ──
    all_points = np.concatenate(all_points) if all_points else np.zeros(0, dtype=POINT_DTYPE)
    frame_meta = np.concatenate(frame_meta) if frame_meta else np.zeros(0, dtype=FRAME_DTYPE)

    out_path = args.out or str(bin_dir / 'pointcloud_3t4r.npz')
    np.savez(out_path, points=all_points, frames=frame_meta)

    total_pts = len(all_points)
    print()
    print("=" * 60)
    print(f"Done!  {n_frames} frames → {total_pts} points")
    print(f"  Output: {out_path}")
    if args.save_rd:
        print(f"  RD maps: {rd_dir}")
    print("=" * 60)


if __name__ == '__main__':
    main()
