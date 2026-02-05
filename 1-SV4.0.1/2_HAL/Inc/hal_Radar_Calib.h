#ifndef _RADAR_CALIB_H
#define _RADAR_CALIB_H

#include "ParamConfig.h"
#include "bsp_UART.h"
#include "freeRTOS.h"
#include "task.h"

/**
 * 添加校准功能的配置说明：
 *  
 * 本函数提供了校准数据接收与保存到FLASH指定地址的接口，需配合【SKYRELAY雷达校准上位机】使用
 * 雷达校准的基本流程如下：
 * 
 *  SKYRELAY雷达校准上位机         |                  下位机
 * ----------------------------------------------------------------
 *        [开始校准]
 *            v
 *    发送中断工作状态指令     --AT\n-->          中断当前工作状态
 *                           <--AT+OK--
 *      发送配置模式指令    --AT+PROG=01\n-->     设置发送数据模式
 *            v              <--AT+OK=01--
 *     配置下位机开始工作     --AT+START\n-->        发送数据
 *            v
 *       接收ADC数据        <--ADC DATA[]--          
 *            v
 *    接收完成&发送中断指令    --AT\n-->          中断当前工作状态
 *            v             
 *       计算校准参数         <--AT+OK--
 *            v
 *       配置校准模式        --AT+CALIB\n-->        进入校准模式
 *                        <--AT+CALIBREADY--
 *   发送频率与幅相校准数据  --CALIB DATA[]-->        接收数据
 *                                                      v
 *     接受到校准完成指令  <--AT+CALIBCOMPLETE--数据类型转换&保存到FLASH 
 *            v
 *        [校准完成]
 * ----------------------------------------------------------------------
 * 
 * 函数接口：
 * 	
 *  void HAL_CalibDataLoop(STRUCT_UART strUart, uint32_t num_calib_ch, uint32_t len);
 * 
 * 函数配置说明：
 *  1. FLASH_CALIB_FREQ_ADDR为频率校准参数FLASH保存地址，按照uint32_t格式保存，数据已左移23位
 *  2. FLASH_CALIB_AP_ADDR为幅度相位校准参数FLASH保存地址，按照pseudo-floating complex格式保存
 *  3. CALIBDATA_ADDR为uart接收数据的缓存地址，在bsp_UART.h中定义
 * 
 *  
 */

#define CALIB_NUMRX 4
#define CALIB_NUMTX 4

#define FLASH_CALIB_FREQ_ADDR (INFOFLASH_ADDR_BEGIN + 49 * NUM_BIT32)
#define FLASH_CALIB_AP_ADDR (FLASH_CALIB_FREQ_ADDR + CALIB_NUMRX * CALIB_NUMTX * NUM_BIT32)

/// @brief 保存校准数据并写入FLASH（校准数据输入地址与FLASH地址通过宏来定义）
/// @param strUart uart状态结构体
/// @param num_calib_ch 校准通道数据（numTx*numRx）
/// @param len 校准数据总长度(Bytes)
extern void HAL_CalibDataLoop(STRUCT_UART strUart, uint32_t num_calib_ch, uint32_t len);

#endif
