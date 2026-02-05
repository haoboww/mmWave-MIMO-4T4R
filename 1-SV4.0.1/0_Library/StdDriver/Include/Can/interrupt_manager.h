#ifndef INTERRUPT_MANAGER_H
#define INTERRUPT_MANAGER_H

#include "SKY32B750.h"

 /*! @file interrupt_manager.h */

/*! @addtogroup interrupt_manager*/
/*! @{*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*! @brief Interrupt handler type */
typedef void (* isr_t)(void);


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name Interrupt manager APIs*/
/*@{*/

/*!
 * @brief Installs an interrupt handler routine for a given IRQ number.
 *
 * This function lets the application register/replace the interrupt
 * handler for a specified IRQ number. See a chip-specific reference
 * manual for details and the  startup_<SoC>.s file for each chip
 * family to find out the default interrupt handler for each device.
 *
 * @note This method is applicable only if interrupt vector is copied in RAM.
 *
 * @param irqNumber   IRQ number
 * @param newHandler  New interrupt handler routine address pointer
 * @param oldHandler  Pointer to a location to store current interrupt handler
 */
void INT_SYS_InstallHandler(IRQn_Type irqNumber,
                            const isr_t newHandler,
                            isr_t* const oldHandler);

/*!
 * @brief Enables an interrupt for a given IRQ number.
 *
 * This function  enables the individual interrupt for a specified IRQ number.
 *
 * @param irqNumber IRQ number
 */
void INT_SYS_EnableIRQ(IRQn_Type irqNumber);

/*!
 * @brief Disables an interrupt for a given IRQ number.
 *
 * This function disables the individual interrupt for a specified IRQ number.
 *
 * @param irqNumber IRQ number
 */
void INT_SYS_DisableIRQ(IRQn_Type irqNumber);

/*!
 * @brief Enables system interrupt.
 *
 * This function enables the global interrupt by calling the core API.
 *
 */
void INT_SYS_EnableIRQGlobal(void);

/*!
 * @brief Disable system interrupt.
 *
 * This function disables the global interrupt by calling the core API.
 *
 */
void INT_SYS_DisableIRQGlobal(void);

/*! @brief  Set Interrupt Priority
 *
 *   The function sets the priority of an interrupt.
 *
 *   @param  irqNumber  Interrupt number.
 *   @param  priority  Priority to set.
 */
void INT_SYS_SetPriority(IRQn_Type irqNumber, uint8_t priority);

/*! @brief  Get Interrupt Priority
 *
 *   The function gets the priority of an interrupt.
 *
 *   @param  irqNumber  Interrupt number.
 *   @return priority   Priority of the interrupt.
 */
uint8_t INT_SYS_GetPriority(IRQn_Type irqNumber);

#if FEATURE_INTERRUPT_HAS_PENDING_STATE
/*!
 * @brief Clear Pending Interrupt
 *
 * The function clears the pending bit of a peripheral interrupt
 * or a directed interrupt to this CPU (if available).
 *
 * @param irqNumber IRQ number
 */
void INT_SYS_ClearPending(IRQn_Type irqNumber);

/*!
 * @brief Set Pending Interrupt
 *
 * The function configures the pending bit of a peripheral interrupt.
 *
 * @param irqNumber IRQ number
 */
void INT_SYS_SetPending(IRQn_Type irqNumber);

/*!
 * @brief Get Pending Interrupt
 *
 * The function gets the pending bit of a peripheral interrupt
 * or a directed interrupt to this CPU (if available).
 *
 * @param irqNumber IRQ number
 * @return pending  Pending status 0/1
 */
uint32_t INT_SYS_GetPending(IRQn_Type irqNumber);

#endif /* FEATURE_INTERRUPT_HAS_PENDING_STATE */



/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* INTERRUPT_MANAGER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
