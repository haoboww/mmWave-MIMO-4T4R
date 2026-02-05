/*******************************************************************************
 * Included files 
 ******************************************************************************/
#include "peripherals_can_pal1.h"

/*******************************************************************************
 * can_pal1 initialization code
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'can_pal1'
- type: 'can_pal_config'
- mode: 'general'
- custom_name_enabled: 'true'
- type_id: 'can_pal'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'CAN'
- config_sets:
  - can_pal:
    - can_instance_t2:
      - name: 'can_pal1_instance'
      - readonly: 'false'
      - instType: 'CAN_INST_TYPE_FLEXCAN'
    - can_user_config_t2:
      - name: 'can_pal1_Config0'
      - readonly: 'true'
      - maxBuffNum: '16'
      - mode: 'CAN_NORMAL_MODE'
      - peClkSrc: 'CAN_CLK_SOURCE_PERIPH'
      - enableFD: 'true'
      - payloadSize: 'CAN_PAYLOAD_SIZE_16'
      - can_bitrate2ts: 'true'
      - flexcan_cfg_time_segments2:
        - nominalBitrate:
          - rJumpwidth: '1'
          - bitrate: '500'
          - samplingPoint: '75'
        - dataBitrate_:
          - rJumpwidth: '1'
          - bitrate: '1000'
          - samplingPoint: '87.5'
    - can_fifo_ext2:
      - isRxFIFO_Enable: 'false'
      - name: 'can_pal1_rx_fifo_ext0'
      - struct_extension:
        - readonly: 'false'
        - numIdFilters: 'FLEXCAN_RX_FIFO_ID_FILTERS_8'
        - idFormat: 'FLEXCAN_RX_FIFO_ID_FORMAT_A'
        - idFilterTable: 'NULL'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files in application code.
 *
 */
    
can_instance_t can_pal1_instance = {
    .instType = CAN_INST_TYPE_FLEXCAN,
    .instIdx = 0UL
};

const can_user_config_t can_pal1_Config0 = {
    .maxBuffNum = 4UL,
//  .mode = CAN_LOOPBACK_MODE,
    .mode = CAN_NORMAL_MODE,
    .peClkSrc = CAN_CLK_SOURCE_OSC,
    .enableFD = false,
    .payloadSize = CAN_PAYLOAD_SIZE_8,

//500Kbps
    .nominalBitrate = {
    .propSeg = 7UL,
    .phaseSeg1 = 5UL,
    .phaseSeg2 = 4UL,
    .preDivider = 3UL,
    .rJumpwidth = 3UL
  },
//500Kbps        
    .dataBitrate = {
    .propSeg = 8UL,
    .phaseSeg1 = 5UL,
    .phaseSeg2 = 4UL,
    .preDivider = 3UL,
    .rJumpwidth = 3UL
  },
        
    .extension = NULL
};







