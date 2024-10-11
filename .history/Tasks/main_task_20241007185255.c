/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-10-06 19:08:06
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-10-07 18:48:14
 * @FilePath: \HW1\Tasks\main_task.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/********************************************************************************
* @file      :<file>.c
* @brief     :
* @history   :
*  Version     Date            Author          Note
*  V0.9.0      yyyy-mm-dd      <author>        1. <note>
*******************************************************************************
* @attention :
*******************************************************************************
*  Copyright (c) 2024 Hello World Team,Zhejiang University.
*  All Rights Reserved.
*******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f103xb.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "main_task.h"
#include "math.h"
#include "tim.h"
#include "stdint.h"
#include "gpio.h"
#include "usart.h"
#include "can.h"
#include "HW_can.hpp"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

uint32_t tick = 0;

void RobotInit()
{
    tick = 0;
}

void main_init()
{
    tick = 0;
    HAL_TIM_Base_Start_IT(&htim3);
    CanFilter_Init();
    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
}

uint8_t Tx_Buffer[9];
struct uart
{
  uint32_t tick;
  int8_t value;
}uart_tx_data;

void MainTask(void)
{
    tick++;
    uart_tx_data.tick=tick;
    uart_tx_data.value = sin(tick * 0.001f);
    if (tick == 1000)
    {
        tick = 0;
        HAL_GPIO_TogglePin(GPIOA,flash_light_Pin);
    }      
    if (tick % 100 == 0)
    {
        encode(Tx_Buffer,uart_tx_data);
    }
    
    uint8_t Can_Tx_Buffer[8];
    Can_Tx_Buffer[0] = tick >> 24;
    Can_Tx_Buffer[1] = tick >> 16;
    Can_Tx_Buffer[2] = tick >> 28;
    Can_Tx_Buffer[3] = tick ;
    CAN_Send_Msg(&hcan,Can_Tx_Buffer,0x100,8);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim3))
    {
      MainTask();
    }
}
void encode(uint8_t *Tx_Buffer,struct uart u1)
{
  Tx_Buffer[0] = 0xAA;
  Tx_Buffer[1] = 0xBB;
  Tx_Buffer[2] = 0xCC;
  Tx_Buffer[3] = u1.tick >> 24;
  Tx_Buffer[4] = u1.tick >> 16;
  Tx_Buffer[5] = u1.tick >> 8;
  Tx_Buffer[6] = u1.tick ;
  Tx_Buffer[7] = u1.value >> 8;
  Tx_Buffer[8] = u1.value ;
}


