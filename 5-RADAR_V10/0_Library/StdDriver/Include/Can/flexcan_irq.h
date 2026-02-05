#ifndef FLEXCAN_IRQ_H
#define FLEXCAN_IRQ_H

#include "SKY32B750.h"

/*!
 * @brief Interrupt handler for a FlexCAN instance.
 *
 * @param   instance    The FlexCAN instance number.
 */
void FLEXCAN_IRQHandler(uint8_t instance);

/*!
 * @brief Error interrupt handler for a FlexCAN instance.
 *
 * @param   instance    The FlexCAN instance number.
 */
void FLEXCAN_Error_IRQHandler(uint8_t instance);

/*!
 * @brief Error interrupt handler for a FlexCAN instance.
 *
 * @param   instance    The FlexCAN instance number.
 */
void FLEXCAN_BusOff_IRQHandler(uint8_t instance);

#if FEATURE_CAN_HAS_WAKE_UP_IRQ

/*!
 * @brief Wake up handler for a FlexCAN instance.
 *
 * @param   instance    The FlexCAN instance number.
 */
void FLEXCAN_WakeUpHandler(uint8_t instance);

#endif /* FEATURE_CAN_HAS_WAKE_UP_IRQ */

/*******************************************************************************
 * Default interrupt handlers signatures
 ******************************************************************************/
void CAN0_IRQHandler(void);
void CAN1_IRQHandler(void);

#endif /* FLEXCAN_IRQ_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
