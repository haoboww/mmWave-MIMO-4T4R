/**
 * @page misra_violations MISRA-C:2012 violations
 *
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, A project should not contain
 * unused type declarations.
 * The header defines callback types for all PAL modules.
 */
#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*! @brief Define the enum of the events which can trigger CAN callback
 *  This enum should include the events for all platforms
 *  Implements : can_event_t_Class
 */
typedef enum {
    CAN_EVENT_RX_COMPLETE,     /*!< A frame was received in the configured Rx buffer. */
    CAN_EVENT_TX_COMPLETE,     /*!< A frame was sent from the configured Tx buffer. */
} can_event_t;

/*! @brief Callback for all peripherals which support CAN features
 * Implements : can_callback_t_Class
 */
typedef void (*can_callback_t)(uint32_t instance,
                               can_event_t eventType,
                               uint32_t objIdx,
                               void *driverState);

#endif /* CALLBACKS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
