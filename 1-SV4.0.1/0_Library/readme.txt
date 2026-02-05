2024-09-27
V9
更新 SKY32B750.h, CAN结构体定义
修改 flexcan_driver.c FLEXCAN_IRQHandler 函数

2024-09-13
V9
修改sadc.h
修改 SADC_TRIGGER 宏定义配置寄存器错误

2024-09-13
V8
修改sadc.h
SADC_GET_EOC_FLG SADC_CLR_EOC_FLG 宏定义
修改sadc.c
更新 uint32_t SADC_Get_Voltage(uint32_t u32Vref, uint8_t u8Channel) 函数，通道选择的写法

2024-07-26
V8
SKY32B750.h
修改CAN_Type结构体定义

2024-07-02
V8
dma.c, dma.h
修改 DMA_Channel_Disable 函数Bug

2024-06-04
V8
efc.c, efc.h
增加3个API函数

2024-05-31
V8
usci.c
修正TE中断等不到的问题

2024-05-20
V8
gpio.c
修正中断模式配置不成功的BUG

2023-12-28
增加CAN驱动，修改gpio.h和sadc.h包含头文件问题


2023-10-25
修改efc占用8K SRAM问题，改为指针指向SRAM2_BASE

2023-6-20
V8
SKY32B750.h, system_SKY32B750.h, system_SKY32B750.c, delay.c, usci.c
Change hardware timeout to software timeout
fix Set_HRC() func, added configuration parameter range protection

2023-6-14
V8
usci.h
usci.c
add USCI_SPI_Transmit_Ext() and USCI_SPI_TRx_Ext() func, add timeout function and clear TE Intr at last byte

2023-4-14
V7
fix DMA_Mask_Irq() and DMA_Unmask_Irq() func bug

2023-2-27
V7
update dma.h
fix DMA_Channel_Enable() func conflicting types bug

2023-2-24
V7
update dma.c
delete DMA_IRQHandler();

2023-2-10
V7
update qspi.c
fix qspi->CS register configuration bug

2022-12-27
V7
update SKY32B750.h, i2c.c, i2c.h

2022-12-5
V7
update SKY32B750.h, gpio.h&.c, ospi.h&.c, qspi.h&.c, retarget.c, usci.c, i2c.h
add GPIO_AT_Ctrl() func;
modify USCI_UART_Init(), fix baud rate bug;

2022-10-31
V6
update system_SKY32B750.c
add delete Cache_Invalidate() and Cache_Disable() in SystemInit() func

2022-10-21
V5
update system_SKY32B750.h
add CLR_WDT_RST_STATE/CLR_PIN_RST_STATE/CLR_POR_RST_STATE macro definition

2022-9-23
V5
update fadc.c and fadc.h
add FADC_WriteWxT funcs in fadc.c, used to write the fadc bit weight value
fix clerical error in FADC_SPI_ReadSingle()

2022-6-20
V4
update rtc.c and rtc.h

2022-6-16
V4
change QSPI_Receive_Enhanced_IO_First() to QSPI_Receive_QUAD_IO_Fast() in qspi.c
change QSPI_Receive_Enhanced_IO() to QSPI_Receive_QUAD_IO_Fast_Continuous() in qspi.c
change QSPI_Receive_Enhanced_IO_DUAL() to QSPI_Receive_DUAL_IO_Fast() in qspi.c
update qspi.h

2022-6-16
V3
update qspi.c and qspi.h

2022-6-15
V3
update wwdg.c and wwdg.h

2022-6-10
V3
fix TMRA_Compare_Cfg() and TMRA_PWM_AutoDead_Cfg() func in timerA.c
add macro definition in timerA.h
add dma config in timerA.c and timerA.h

2022-6-9
V3
update timerA.c and timerA.h

2022-5-27
V3
fix flash write & erase bug for remap in efc.c and efc.h
update dma.c and dma.h

2022-5-18
V3
modify TMRB_CLK in timerB.h
modify funcs in timerB.c
add CRC_Calculate func in crc.c and crc.h

2022-5-17
V2
update retarget.c for any USCI port
add USCI_Enable & USCI_Disable func in usci.c

2022-5-16
V1
initial version
