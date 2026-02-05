#include "interrupt_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Counter to manage the nested callings of global disable/enable interrupt.
 */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_InstallHandler
 * Description   : Install an interrupt handler routine for a given IRQ number
 * This function will let application register/replace the interrupt
 * handler for specified IRQ number. IRQ number is different than Vector
 * number. IRQ 0 will start from Vector 16 address. Refer to reference
 * manual for details. Also refer to startup_<CPU>.s file for each chip
 * family to find out the default interrupt handler for each device. This
 * function will convert the IRQ number to vector number by adding 16 to
 * it.
 *
 * Note          : This method is applicable only if interrupt vector is
 *                 copied in RAM, __flash_vector_table__ symbol is used to
 *                 control this from linker options.
 * Implements INT_SYS_InstallHandler_Activity
 *
 *END**************************************************************************/
void INT_SYS_InstallHandler(IRQn_Type irqNumber,
                            const isr_t newHandler,
                            isr_t* const oldHandler)
{

}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_EnableIRQ
 * Description   : Enables an interrupt for a given IRQ number.
 * It calls the system NVIC API to access the interrupt control
 * register and MSCM (if available) API for interrupt routing.
 * The input IRQ number does not include the core interrupt, only
 * the peripheral interrupt, from 0 to a maximum supported IRQ.
 * Implements INT_SYS_EnableIRQ_Activity
 *END**************************************************************************/
void INT_SYS_EnableIRQ(IRQn_Type irqNumber)
{
    NVIC_EnableIRQ(irqNumber);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_DisableIRQ
 * Description   : Disable individual interrupt for a specified IRQ
 * It  calls the system NVIC API to access the interrupt control register
 * and MSCM (if available) API for interrupt routing.
 * Implements INT_SYS_DisableIRQ_Activity
 *
 *END**************************************************************************/
void INT_SYS_DisableIRQ(IRQn_Type irqNumber)
{
      NVIC_DisableIRQ(irqNumber);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_EnableIRQGlobal
 * Description   : Enable system interrupt
 * This function will enable the global interrupt by calling the core API
 * Implements INT_SYS_EnableIRQGlobal_Activity
 *
 *END**************************************************************************/
void INT_SYS_EnableIRQGlobal(void)
{

}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_DisableIRQGlobal
 * Description   : Disable system interrupt
 * This function will disable the global interrupt by calling the core API
 * Implements INT_SYS_DisableIRQGlobal_Activity
 *
 *END**************************************************************************/
void INT_SYS_DisableIRQGlobal(void)
{

}


