/******************************************************************************
 * @Filename     : usci.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/06/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : USCI driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"

/**
 * @brief     USCI_Enable: Enable USCIx Clock
 * @DateTime  2022-05-17T18:22:36+0800
 * @param     usci                      USCI port. It could be USCI0 ~ USCI7
 * @return :  uint16_t                  set result
 *                                      - GPIO_INVALID_PARA: Parameter Error
 *                                      - OK: Set Successfully
 */
uint16_t USCI_Enable(USCI_T *usci)
{
    if (usci == USCI0)      USCI0_CLK_ENABLE;
    else if (usci == USCI1) USCI1_CLK_ENABLE;
    else if (usci == USCI2) USCI2_CLK_ENABLE;
    else if (usci == USCI3) USCI3_CLK_ENABLE;
    else if (usci == USCI4) USCI4_CLK_ENABLE;
    else if (usci == USCI5) USCI5_CLK_ENABLE;
    else if (usci == USCI6) USCI6_CLK_ENABLE;
    else if (usci == USCI7) USCI7_CLK_ENABLE;
    else return USCI_INVALID_PARA;
    return OK;
}

/**
 * @brief     USCI_Disable: Disable USCIx Clock
 * @DateTime  2022-05-17T18:24:46+0800
 * @param     usci                      USCI port. It could be USCI0 ~ USCI7
 * @return :  uint16_t                  set result
 *                                      - GPIO_INVALID_PARA: Parameter Error
 *                                      - OK: Set Successfully
 */
uint16_t USCI_Disable(USCI_T *usci)
{
    if (usci == USCI0)      USCI0_CLK_DISABLE;
    else if (usci == USCI1) USCI1_CLK_DISABLE;
    else if (usci == USCI2) USCI2_CLK_DISABLE;
    else if (usci == USCI3) USCI3_CLK_DISABLE;
    else if (usci == USCI4) USCI4_CLK_DISABLE;
    else if (usci == USCI5) USCI5_CLK_DISABLE;
    else if (usci == USCI6) USCI6_CLK_DISABLE;
    else if (usci == USCI7) USCI7_CLK_DISABLE;
    else return USCI_INVALID_PARA;
    return OK;
}

/**
 * @brief     USCI_Clear_ISR: Clear all interrupt
 * @DateTime  2022-05-24T14:32:33+0800
 * @param     usci                      pointer to Usci instance
 */
void USCI_Clear_ISR(usci_handler_t *usci)
{
    usci->instance->ISR = 0xFF;
}

/**
 * @brief     USCI_Clear_ESR: Clear all error interrupt
 * @DateTime  2022-05-24T14:32:49+0800
 * @param     usci                      pointer to Usci instance
 */
void USCI_Clear_ESR(usci_handler_t *usci)
{
    usci->instance->ESR = 0xFF;
}

/**
 * @Breif  : USCI_UART_Init : Uart Init function
 *
 * @Param  : *usci          : pointer to Usci instance
 *
 * @Returns: uint8_t        : USCI_INVALID_PARA
 *                          : OK
 */
uint16_t USCI_UART_Init(usci_handler_t *usci)
{
    uint32_t reg_cfg1 = 0;
    uint32_t baud_div = 0;
    uint32_t baud_mod = 0;
    uint32_t raw_baud = 0;

    if (usci == NULL)
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    reg_cfg1 = ((usci->u_init.uart_data_len << USCI_CFG1_DATA_LEN_Pos)  |  \
                (usci->u_init.uart_lsb << USCI_CFG1_LSB_Pos)            |  \
                (usci->u_init.uart_stop_bit << USCI_CFG1_STOP_LEN_Pos));
    if (usci->u_init.uart_parity_en)
        reg_cfg1 |= ((usci->u_init.uart_parity_en << USCI_CFG1_PAR_EN_Pos) | (usci->u_init.uart_parity_typ << USCI_CFG1_PAR_TYP_Pos));

    reg_cfg1 |= (USCI_CFG1_TX_EN_Msk | USCI_CFG1_RX_EN_Msk);

    usci->instance->CFG0 = (USCI_CFG0_WK_MODE_Msk & USCI_WORK_MODE_UART);
    usci->instance->CFG1 = reg_cfg1;

    raw_baud = (pclk << 4) / usci->u_init.uart_baud_rate;
    baud_div = raw_baud >> 4;
    baud_mod = ((raw_baud >> 1) & 0x7) + (raw_baud & 1);
    baud_div = baud_div + ((baud_mod >> 3) & 1);
    baud_mod = baud_mod & 0x7;
    usci->instance->MOD  = (USCI_BAUD_SDIV_Msk & (baud_mod << USCI_BAUD_SDIV_Pos));
    usci->instance->MOD &= ~USCI_BAUD_S16_Msk;
    usci->instance->BAUD = USCI_BAUD_DIV_Msk & ((baud_div > 0) ? (baud_div - 1) : (baud_div));

    return OK;
}

/**
 * @Breif  : USCI_UART_Transmit : Uart transmit function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : tx_buf    : pointer to tx data buffer
 * @Param  : size      : tx data byte length
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_UART_Transmit(usci_handler_t *usci, uint8_t *tx_buf, int32_t size)
{
    if ((usci == NULL) || (tx_buf == NULL) || (size <= 0))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    while (size > 0)
    {
        usci->instance->TRX = *tx_buf;
        /* Wait for Tx fifo not full */
        while ((usci->instance->ISR & USCI_IS_TXE_Msk) == 0);
        /* TX data */
        tx_buf += 1;
        size -= 1;
    }
    while ((usci->instance->ISR & USCI_IS_TE_Msk) == 0);
    usci->instance->ISR = USCI_IS_TE_Msk;
    return OK;
}

/**
 * @Breif  : USCI_UART_Receive : Uart Receive function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : rx_buf    : pointer to rx data buffer
 * @Param  : size      : rx data byte length
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_UART_Receive(usci_handler_t *usci, uint8_t *rx_buf, int32_t size)
{
    if ((usci == NULL) || (rx_buf == NULL) || (size <= 0))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    while (size > 0)
    {
        while ((usci->instance->ISR & USCI_IS_RXNE_Msk) == 0);
        *rx_buf = usci->instance->TRX;
        rx_buf += 1;
        size -= 1;
    }

    return OK;
}

////////////////////////////////////////////////////////////////////////////
//SPI
////////////////////////////////////////////////////////////////////////////
/**
 * @Breif  : USCI_SPI_Init  : Spi Init function
 *
 * @Param  : *usci          : pointer to Usci instance
 *
 * @Returns: uint8_t        : USCI_INVALID_PARA
 *                          : OK
 */
uint16_t USCI_SPI_Init(usci_handler_t *usci)
{
    if (usci == NULL)
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    usci->instance->CFG0 = (USCI_CFG0_WK_MODE_Msk & USCI_WORK_MODE_SPI);

    usci->instance->CFG1 = 0;
    usci->instance->CFG1 = ((usci->s_init.spi_clkp << USCI_CFG1_CLKP_Pos)           |       \
                            (usci->s_init.spi_data_len << USCI_CFG1_DATA_LEN_Pos)   |       \
                            (usci->s_init.spi_msb << USCI_CFG1_LSB_Pos)             |       \
                            (usci->s_init.spi_posedge << USCI_CFG1_TRX_NEG_Pos)     |       \
                            (usci->s_init.spi_mode << USCI_CFG1_SLV_SEL_Pos));
    usci->instance->CFG1 |= (USCI_CFG1_TX_EN_Msk | USCI_CFG1_RX_EN_Msk);

    //SPI slave mode, set CS and GPIO Num
    if (usci->s_init.spi_mode == USCI_SPI_WORK_MODE_SLAVE)
    {
        if (usci->s_init.spi_cs_level == USCI_SPI_SLAVE_CS_LEVEL_HIGH)
        {
            usci->instance->CFG1 |= USCI_CFG1_CS_LVL_Msk;
        }
        usci->instance->CFG1 |= ((usci->s_init.spi_cs_gpio << USCI_CFG1_CS_SEL_Pos) & USCI_CFG1_CS_SEL_Msk);
    }

    usci->instance->BAUD = usci->usci_clock_div;
    usci->instance->CLK_DIV = 0;

    return OK;
}

/**
 * @Breif  : USCI_SPI_Transmit : Spi Master Transmit function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : tx_buf    : pointer to tx data buffer
 * @Param  : size      : tx data byte length
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_SPI_Transmit(usci_handler_t *usci, uint8_t *tx_buf, int32_t size)
{
    if ((usci == NULL) || (tx_buf == NULL) || (size <= 0))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    while (size > 0)
    {
        usci->instance->TRX = *tx_buf;
        /* Wait for Tx fifo not full */
        while ((usci->instance->ISR & USCI_IS_TXE_Msk) == 0);
        /* TX data */
        tx_buf += 1;
        size -= 1;
    }
    while ((usci->instance->ISR & USCI_IS_TE_Msk) == 0)
        ;
    usci->instance->ISR = USCI_IS_TE_Msk;

    return OK;
}

/**
 * @Breif  : USCI_SPI_Transmit_Ext : Spi Master Transmit function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : tx_buf    : pointer to tx data buffer
 * @Param  : size      : tx data byte length
 * @param  : tout_ms : Timeout judgment
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_SPI_Transmit_Ext(usci_handler_t *usci, uint8_t *tx_buf, int32_t size, uint32_t tout_ms)
{
    uint32_t cnt = 0;
    uint32_t tout_cnt = tout_ms * ms_step;

    if ((usci == NULL) || (tx_buf == NULL) || (size <= 0))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    while (size > 0)
    {
        usci->instance->TRX = *tx_buf;
        /*Clear TE after send last Byte*/
        if (size == 1)
            usci->instance->ISR = USCI_IS_TE_Msk;
        /* Wait for Tx fifo not full */
        cnt = 0;
        while ((usci->instance->ISR & USCI_IS_TXE_Msk) == 0)
        {
            cnt++;
            if (cnt > tout_cnt)
                return USCI_TIMEOUT_ERR;
        }
        /* TX data */
        tx_buf += 1;
        size -= 1;
    }
    cnt = 0;
    while ((usci->instance->ISR & USCI_IS_TE_Msk) == 0)
    {
        cnt++;
        if (cnt > tout_cnt)
            return USCI_TIMEOUT_ERR;
    }

    usci->instance->ISR = USCI_IS_TE_Msk;

    return OK;
}
/**
 * @Breif  : USCI_SPI_Receive : Spi Master Receive function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : rx_buf    : pointer to rx data buffer
 * @Param  : size      : rx data byte length
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_SPI_Receive(usci_handler_t *usci, uint8_t *rx_buf, int32_t size)
{
    if ((usci == NULL) || (rx_buf == NULL) || (size <= 0))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    //rx first,  tx dummy data
    usci->instance->TRX = USCI_SPI_DUMMY_DATA;
    while ((usci->instance->ISR & USCI_IS_TXE_Msk) == 0);
    while ((usci->instance->ISR & USCI_IS_TE_Msk) == 0)
        ;
    usci->instance->ISR = USCI_IS_TE_Msk;
    //rx data
    while ((usci->instance->ISR & USCI_IS_RXNE_Msk) == 0);
    *rx_buf = usci->instance->TRX;
    rx_buf += 1;
    size -= 1;

    while (size > 0)
    {
        usci->instance->TRX = USCI_SPI_DUMMY_DATA;
        while ((usci->instance->ISR & USCI_IS_TXE_Msk) == 0);
        while ((usci->instance->ISR & USCI_IS_RXNE_Msk) == 0);
        *rx_buf = usci->instance->TRX;
        rx_buf += 1;
        size -= 1;
    }

    return OK;
}

/**
 * @Breif  : USCI_SPI_TRx : Spi Transmit and Receive Master function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : tx_buf    : pointer to tx data buffer
 * @Param  : rx_buf    : pointer to rx data buffer
 * @Param  : size      : tx and rx data byte length
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_SPI_TRx(usci_handler_t *usci, uint8_t *tx_buf, int32_t size, uint8_t *rx_buf)
{
    if ((usci == NULL) || (tx_buf == NULL) || (size <= 0) || (rx_buf == NULL))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    while (size > 0)
    {
        usci->instance->TRX = *tx_buf;
        while ((usci->instance->ISR & USCI_IS_TXE_Msk) == 0);
        while ((usci->instance->ISR & USCI_IS_RXNE_Msk) == 0);
        *rx_buf = usci->instance->TRX;
        rx_buf += 1;
        tx_buf += 1;
        size -= 1;
    }

    return OK;
}
/**
 * @Breif  : USCI_SPI_TRx_Ext : Spi Transmit and Receive Master function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : tx_buf    : pointer to tx data buffer
 * @Param  : rx_buf    : pointer to rx data buffer
 * @Param  : size      : tx and rx data byte length
 * @param  : tout_ms : Timeout judgment
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_SPI_TRx_Ext(usci_handler_t *usci, uint8_t *tx_buf, int32_t size, uint8_t *rx_buf, uint32_t tout_ms)
{
    uint32_t cnt = 0;
    uint32_t tout_cnt = tout_ms * ms_step;

    if ((usci == NULL) || (tx_buf == NULL) || (size <= 0) || (rx_buf == NULL))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    while (size > 0)
    {
        usci->instance->TRX = *tx_buf;
        cnt = 0;
        while ((usci->instance->ISR & USCI_IS_TXE_Msk) == 0)
        {
            cnt++;
            if (cnt > tout_cnt)
                return USCI_TIMEOUT_ERR;
        }
        cnt = 0;
        while ((usci->instance->ISR & USCI_IS_RXNE_Msk) == 0)
        {
            cnt++;
            if (cnt > tout_cnt)
                return USCI_TIMEOUT_ERR;
        }
        *rx_buf = usci->instance->TRX;
        rx_buf += 1;
        tx_buf += 1;
        size -= 1;
    }

    return OK;
}
/**
 * @Breif  : USCI_SPI_Slave_Transmit : Spi Slave Transmit function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : tx_buf    : pointer to tx data buffer
 * @Param  : size      : tx data byte length
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_SPI_Slave_Transmit(usci_handler_t *usci, uint8_t *tx_buf, int32_t size)
{
    if ((usci == NULL) || (tx_buf == NULL) || (size <= 0))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    while (size > 0)
    {
        usci->instance->TRX = *tx_buf;
        /* Wait for Tx fifo not full */
        while ((usci->instance->ISR & USCI_IS_TXE_Msk) == 0);
        /* TX data */
        tx_buf += 1;
        size -= 1;
    }
    while ((usci->instance->ISR & USCI_IS_TE_Msk) == 0)
        ;
    usci->instance->ISR = USCI_IS_TE_Msk;

    return OK;
}

/**
 * @Breif  : USCI_SPI_Slave_Receive : Spi Slave Receive function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : rx_buf    : pointer to rx data buffer
 * @Param  : size      : rx data byte length
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_SPI_Slave_Receive(usci_handler_t *usci, uint8_t *rx_buf, int32_t size)
{
    if ((usci == NULL) || (rx_buf == NULL) || (size <= 0))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    while (size > 0)
    {
        while ((usci->instance->ISR & USCI_IS_RXNE_Msk) == 0);
        *rx_buf = usci->instance->TRX;
        rx_buf += 1;
        size -= 1;
    }

    return OK;
}

////////////////////////////////////////////////////////////////////////////
//U7816
////////////////////////////////////////////////////////////////////////////
/**
 * @Breif  : USCI_U7816_Init  : U7816 Init function
 *
 * @Param  : *usci            : pointer to Usci instance
 *
 * @Returns: uint8_t          : USCI_INVALID_PARA
 *                            : OK
 */
uint16_t USCI_U7816_Init(usci_handler_t *usci)
{
    uint32_t reg_cfg1 = 0;

    if (usci == NULL)
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    usci->instance->CFG0 = (USCI_CFG0_WK_MODE_Msk & USCI_WORK_MODE_U7816);

    reg_cfg1 = ((usci->c_init.u7816_data_len << USCI_CFG1_DATA_LEN_Pos)    |  \
                (usci->c_init.u7816_lsb << USCI_CFG1_LSB_Pos)              |  \
                (usci->c_init.u7816_invert << USCI_CFG1_BIT_INV_Pos)       |  \
                (usci->c_init.u7816_stop_bit << USCI_CFG1_STOP_LEN_Pos));

    if (usci->c_init.u7816_parity_en)
        reg_cfg1 |= ((usci->c_init.u7816_parity_en << USCI_CFG1_PAR_EN_Pos) | (usci->c_init.u7816_parity_typ << USCI_CFG1_PAR_TYP_Pos));

    usci->instance->CFG1 = reg_cfg1;
    //usci->instance->CFG1 = 0x00250;
    //not used
    //usci->instance->CFG1 = 0x01210;
    //usci->instance->CLK_DIV = 0;
    usci->instance->CLK_DIV = usci->usci_clock_div;

    usci->instance->BAUD = usci->c_init.u7816_speed;

    return OK;
}

/**
 * @Breif  : USCI_U7816_Transmit : U7816 Transmit function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : tx_buf    : pointer to tx data buffer
 * @Param  : size      : tx data byte length
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_U7816_Transmit(usci_handler_t *usci, uint8_t *tx_buf, int32_t size)
{
    if ((usci == NULL) || (tx_buf == NULL) || (size <= 0))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    usci->instance->CFG1 &= ~USCI_U7816_TRX_EN_MSK;
    usci->instance->CFG1 |= USCI_CFG1_TX_EN_Msk;

    usci->instance->ISR = USCI_IS_TXE_Msk;
    while (size > 0)
    {
        usci->instance->TRX = *tx_buf;
        /* Wait for Tx fifo not full */
        while ((usci->instance->ISR & USCI_IS_TXE_Msk) == 0);
        /* TX data */
        tx_buf += 1;
        size -= 1;
    }
    while ((usci->instance->ISR & USCI_IS_TE_Msk) == 0)
        ;
    usci->instance->ISR = USCI_IS_TE_Msk;

    return OK;
}

/**
 * @Breif  : USCI_U7816_Receive : U7816 Receive function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : rx_buf    : pointer to rx data buffer
 * @Param  : size      : rx data byte length
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_U7816_Receive(usci_handler_t *usci, uint8_t *rx_buf, int32_t size)
{
    if ((usci == NULL) || (rx_buf == NULL) || (size <= 0))
        return USCI_INVALID_PARA;

    USCI_Clear_ESR(usci);
    USCI_Clear_ISR(usci);

    usci->instance->CFG1 &= ~USCI_U7816_TRX_EN_MSK;
    usci->instance->CFG1 |= USCI_CFG1_RX_EN_Msk;

    usci->instance->ISR = USCI_IS_RXNE_Msk;
    while (size > 0)
    {
        while ((usci->instance->ISR & USCI_IS_RXNE_Msk) == 0);
        *rx_buf = usci->instance->TRX;
        rx_buf += 1;
        size -= 1;
    }

    return OK;
}

/**
 * @Breif  : USCI_U7816_Receive : U7816 Receive function
 *
 * @Param  : *usci     : pointer to Usci instance
 * @Param  : rx_buf    : pointer to rx data buffer
 * @Param  : size      : rx data byte length
 *
 * @Returns: uint8_t   : USCI_INVALID_PARA
 *                     : OK
 */
uint16_t USCI_U7816_Cmd(usci_handler_t *usci, uint32_t cmd)
{
    if ((usci == NULL) || (cmd == NULL))
        return USCI_INVALID_PARA;

    usci->instance->CMD = cmd;

    return OK;
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
