/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OSPI_DRIVER_H
#define _OSPI_DRIVER_H
#include "ospi.h"

#define OSPI_DUMMY_DATA                (0xFF)
enum ospi_status {
  OSPI_OK = 0,
  OSPI_BUSY,
  OSPI_ERROR,
  OSPI_PARA_ERROR
};
extern uint8_t ospi_sendover_flag;
extern uint8_t ospi_receiveover_flag;




void OSPI_MasterInit(void);
void OSPI_SlaveInit(void);
void OSPI_Trans64(uint64_t *srcAddr,uint32_t len);
void OSPI_Receive64(uint32_t *dstAddr,uint32_t len);
void OSPI_TransDMA64(uint32_t *srcAddr,uint32_t len);

enum ospi_status OSPI_Receive(OSPI_T *ospi, XOSPI_T *xospi, uint8_t *cmd_buf, uint8_t cmd_len, uint8_t *rx_buf, uint32_t rx_len);
enum ospi_status OSPI_Transmit(OSPI_T *ospi, XOSPI_T *xospi,  uint8_t *cmd_buf, uint8_t cmd_len, uint8_t *tx_buf, uint32_t tx_len);
enum ospi_status OSPI_Transmit_Enhanced(OSPI_T *ospi, XOSPI_T *xospi, uint8_t *cmd_buf, uint8_t cmd_len, uint8_t *tx_buf, uint32_t tx_len, enum qspi_frame_mode qspi_mode);
enum ospi_status OSPI_Receive_Enhanced(OSPI_T *ospi, XOSPI_T *xospi, uint8_t *cmd_buf, uint8_t cmd_len, uint8_t *rx_buf, uint32_t rx_len, enum qspi_frame_mode qspi_mode);














#endif /*_OSPI_DRIVER_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























