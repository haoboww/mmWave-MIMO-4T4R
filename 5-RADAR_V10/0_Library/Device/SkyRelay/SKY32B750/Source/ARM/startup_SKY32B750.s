;/******************************************************************************
; * @Filename     : startup_SKY32B750.s
; * @Version      : V1.0
; * @Subversion   : $
; * @Last modified: 05/10/2022 9:30:29
; * @Modified by  : CB
; * @Brief        : CMSIS ARM Cortex-M7 Core Device Startup File
; *
; * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
;*****************************************************************************/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
    IF :LNOT: :DEF: Stack_Size
Stack_Size      EQU     0x00000800
    ENDIF
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
    IF :LNOT: :DEF: Heap_Size
Heap_Size       EQU     0x00000200
    ENDIF
                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB



; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     RES_IRQHandler            ; res
                DCD     WDT_IRQHandler            ; Watch Dog Timer interrupt
                DCD     RTC_IRQHandler            ; RTC interrupt
                DCD     USCI0_IRQHandler          ; USCI0 interrupt
                DCD     USCI1_IRQHandler          ; USCI1 interrupt
                DCD     USCI2_IRQHandler          ; USCI2 interrupt
                DCD     USCI3_IRQHandler          ; USCI3 interrupt
                DCD     USCI4_IRQHandler          ; USCI4 interrupt
                DCD     USCI5_IRQHandler          ; USCI5 interrupt
                DCD     USCI6_IRQHandler          ; USCI6 interrupt
                DCD     USCI7_IRQHandler          ; USCI7 interrupt
                DCD     QSPI0_IRQHandler          ;    QSPI0 interrupt
                DCD     QSPI1_IRQHandler          ;    QSPI1 interrupt
                DCD     TMRA_IRQHandler           ; Timer A interrupt
                DCD     TMRB0_IRQHandler          ; Timer B0 interrupt
                DCD     TMRB1_IRQHandler          ; Timer B1 interrupt
                DCD     TMRB2_IRQHandler          ; Timer B2 interrupt
                DCD     TMRB3_IRQHandler          ; Timer B3 interrupt
                DCD     WWDG_IRQHandler           ;
                DCD     DMA_IRQHandler            ;
                DCD     BB_IRQHandler             ; BB interrupt
                DCD     I2C_IRQHandler            ; I2C interrupt
                DCD     GPIO0_IRQHandler          ;    GPIO0  interrupt
                DCD     GPIO0_E0_IRQHandler       ;
                DCD     GPIO0_E1_IRQHandler       ;
                DCD     GPIO1_IRQHandler          ;    GPIO1  interrupt
                DCD     GPIO1_E0_IRQHandler       ;
                DCD     GPIO1_E1_IRQHandler       ;
                DCD     GPIO2_IRQHandler          ;    GPIO2  interrupt
                DCD     GPIO2_E0_IRQHandler       ;
                DCD     GPIO2_E1_IRQHandler       ;
                DCD     GPIO3_IRQHandler          ;    GPIO3  interrupt
                DCD     GPIO3_E0_IRQHandler       ;
                DCD     GPIO3_E1_IRQHandler       ;
                DCD     CAN0_IRQHandler           ;
                DCD     CAN1_IRQHandler           ;
                DCD     BOD_IRQHandler            ; Brownout low voltage detected interrupt
                DCD     SAR_IRQHandler            ;
                DCD     OSPI0_IRQHandler          ;
                DCD     OSPI1_IRQHandler          ;

                AREA    |.text|, CODE, READONLY


; Reset Handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

;Reset_Handler   PROC
;                EXPORT  Reset_Handler             [WEAK]
;                IMPORT  __main
;                LDR     R0, =__main
;                BX      R0
;                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  RES_IRQHandler           [WEAK]
                EXPORT  WDT_IRQHandler           [WEAK]
                EXPORT  RTC_IRQHandler           [WEAK]
                EXPORT  USCI0_IRQHandler         [WEAK]
                EXPORT  USCI1_IRQHandler         [WEAK]
                EXPORT  USCI2_IRQHandler         [WEAK]
                EXPORT  USCI3_IRQHandler         [WEAK]
                EXPORT  USCI4_IRQHandler         [WEAK]
                EXPORT  USCI5_IRQHandler         [WEAK]
                EXPORT  USCI6_IRQHandler         [WEAK]
                EXPORT  USCI7_IRQHandler         [WEAK]
                EXPORT  QSPI0_IRQHandler         [WEAK]
                EXPORT  QSPI1_IRQHandler         [WEAK]
                EXPORT  TMRA_IRQHandler          [WEAK]
                EXPORT  TMRB0_IRQHandler         [WEAK]
                EXPORT  TMRB1_IRQHandler         [WEAK]
                EXPORT  TMRB2_IRQHandler         [WEAK]
                EXPORT  TMRB3_IRQHandler         [WEAK]
                EXPORT  WWDG_IRQHandler          [WEAK]
                EXPORT  DMA_IRQHandler           [WEAK]
                EXPORT  BB_IRQHandler            [WEAK]
                EXPORT  I2C_IRQHandler           [WEAK]
                EXPORT  GPIO0_IRQHandler         [WEAK]
                EXPORT  GPIO0_E0_IRQHandler      [WEAK]
                EXPORT  GPIO0_E1_IRQHandler      [WEAK]
                EXPORT  GPIO1_IRQHandler         [WEAK]
                EXPORT  GPIO1_E0_IRQHandler      [WEAK]
                EXPORT  GPIO1_E1_IRQHandler      [WEAK]
                EXPORT  GPIO2_IRQHandler         [WEAK]
                EXPORT  GPIO2_E0_IRQHandler      [WEAK]
                EXPORT  GPIO2_E1_IRQHandler      [WEAK]
                EXPORT  GPIO3_IRQHandler         [WEAK]
                EXPORT  GPIO3_E0_IRQHandler      [WEAK]
                EXPORT  GPIO3_E1_IRQHandler      [WEAK]
                EXPORT  CAN0_IRQHandler          [WEAK]
                EXPORT  CAN1_IRQHandler          [WEAK]
                EXPORT  BOD_IRQHandler           [WEAK]
                EXPORT  SAR_IRQHandler           [WEAK]
                EXPORT  OSPI0_IRQHandler         [WEAK]
                EXPORT  OSPI1_IRQHandler         [WEAK]


RES_IRQHandler
WDT_IRQHandler
RTC_IRQHandler
USCI0_IRQHandler
USCI1_IRQHandler
USCI2_IRQHandler
USCI3_IRQHandler
USCI4_IRQHandler
USCI5_IRQHandler
USCI6_IRQHandler
USCI7_IRQHandler
QSPI0_IRQHandler
QSPI1_IRQHandler
TMRA_IRQHandler
TMRB0_IRQHandler
TMRB1_IRQHandler
TMRB2_IRQHandler
TMRB3_IRQHandler
WWDG_IRQHandler
DMA_IRQHandler
BB_IRQHandler
I2C_IRQHandler
GPIO0_IRQHandler
GPIO0_E0_IRQHandler
GPIO0_E1_IRQHandler
GPIO1_IRQHandler
GPIO1_E0_IRQHandler
GPIO1_E1_IRQHandler
GPIO2_IRQHandler
GPIO2_E0_IRQHandler
GPIO2_E1_IRQHandler
GPIO3_IRQHandler
GPIO3_E0_IRQHandler
GPIO3_E1_IRQHandler
CAN0_IRQHandler
CAN1_IRQHandler
BOD_IRQHandler
SAR_IRQHandler
OSPI0_IRQHandler
OSPI1_IRQHandler

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
