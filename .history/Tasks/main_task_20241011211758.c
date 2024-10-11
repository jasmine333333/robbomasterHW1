/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-10-06 19:08:06
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-10-11 21:17:58
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
#include <string.h>
#include <stdbool.h>
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t tick = 0;
uint8_t tx_byte = 0;
uint8_t rx_byte = 0;
uint8_t Tx_Buffer[9];
uint8_t Rx_Buffer[9];
uint8_t Rx_length = 0;
struct uart
{
  uint32_t tick_;
  float value;
};

struct uart uart_tx_data,uart_rx_data;

struct CANCommData Can_tx_data;

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void RobotInit();
void MainInit();
void MainTask(void);
void Can_data_encode(uint8_t *Can_Tx_Buffer, struct CANCommData *u3);
void encode(uint8_t *Tx_Buffer, struct uart u1);
void decode(uint8_t *Rx_Buffer, struct uart *u2);


void RobotInit()
{
    uart_tx_data.tick_ = 0;
    uart_tx_data.value = 0;
    uart_rx_data.tick_ = 0;
    uart_rx_data.value = 0;
    tick = 0;
}

void MainInit()
{
    tick = 0;
    
    HAL_TIM_Base_Start_IT(&htim3);
    //HAL_UART_Receive_IT(&huart2,&rx_byte,1);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2,Rx_Buffer,9);

    CanFilter_Init();
    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
}

void encode(uint8_t *Tx_Buffer,struct uart u1);
void decode(uint8_t *Rx_Buffer, struct uart *u2);
void Can_data_encode(uint8_t *Can_Tx_Buffer, struct CANCommData *u3);

void FloatToBytes(float f, uint8_t *bytes) {  
    memcpy(bytes, &f, sizeof(float));  
}  

void MainTask(void)
{
    tick++;
    uart_tx_data.tick_=tick;
    uart_tx_data.value = sin(tick * 0.001f);
    Can_tx_data.tick = tick;
    Can_tx_data.value1 = sin(tick * 0.001f);
    Can_tx_data.value2 = cos(tick * 0.001f);
    if(tick % 1000 == 0)
    {
        HAL_GPIO_TogglePin(GPIOA,flash_light_Pin);
    }    
    if (tick % 10 == 0)
    {
        encode(Tx_Buffer,uart_tx_data);
        HAL_UART_Transmit_DMA(&huart1,Tx_Buffer,9);
    }
    if (tick % 10 ==1)
    {
    uint8_t Can_Tx_Buffer[8];
    Can_data_encode(Can_Tx_Buffer,&Can_tx_data);
    CAN_Send_Msg(&hcan,Can_Tx_Buffer,0x100,8);
    }
}

void Can_data_encode(uint8_t *Can_Tx_Buffer, struct CANCommData *u3)
{
    Can_Tx_Buffer[0] = u3->tick >> 24;
    Can_Tx_Buffer[1] = u3->tick >> 16;
    Can_Tx_Buffer[2] = u3->tick >> 8;
    Can_Tx_Buffer[3] = u3->tick ;
    int16_t temp1 = u3->value1 *30000;
    Can_Tx_Buffer[4] = temp1 >> 8 ;
    Can_Tx_Buffer[5] = temp1 ;
    int16_t temp2 = u3->value2 *30000;
    Can_Tx_Buffer[6] = temp2 >> 8 ;
    Can_Tx_Buffer[7] = temp2 ;
    
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim3))
    {
      MainTask();
    }
}
void encode(uint8_t *Tx_Buffer, struct uart u1)
{
  Tx_Buffer[0] = 0xAA;
  Tx_Buffer[1] = 0xBB;
  Tx_Buffer[2] = 0xCC;
  Tx_Buffer[3] = u1.tick_ >> 24;
  Tx_Buffer[4] = u1.tick_ >> 16;
  Tx_Buffer[5] = u1.tick_ >> 8;
  Tx_Buffer[6] = u1.tick_ ;
  int16_t temp = u1.value *30000;
  Tx_Buffer[7] = temp >> 8;
  Tx_Buffer[8] = temp ;
}
void decode(uint8_t *Rx_Buffer, struct uart *u2)
{
    uint8_t rx_state;
    uint16_t temp1;
    if (rx_state == 0)
    {
        if (Rx_Buffer[0] == 0xAA && Rx_Buffer[1] == 0xBB && Rx_Buffer[2] == 0xCC)
        {
            rx_state = 1;
        }
    }
    else if (rx_state == 1)
    {
        u2->tick_ = (Rx_Buffer[3]<<24) | (Rx_Buffer[4]<<16) | (Rx_Buffer[5]<<8) | (Rx_Buffer[6]<<0) ;
        temp1 = (Rx_Buffer[7]<<8) | (Rx_Buffer[8]<<0) ;
        u2->value = (int16_t)temp1 / 30000.0f;
        rx_state = 2;
    }
    else if (rx_state == 2)
    {
        rx_state = 0;
    }       
}

/* void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2)
    {
        Rx_Buffer[Rx_length] = rx_byte;
        Rx_length ++;
        if (Rx_length == 9)
        {
            decode(Rx_Buffer,&uart_rx_data);
            Rx_length = 0;
        }
        HAL_UART_Receive_IT(&huart2,&rx_byte,1);
    }
}
 */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t size)
{
    if (huart == &huart2)
    {
        if (size == 9)
        {
            decode(Rx_Buffer,&uart_rx_data);
        }
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2,Rx_Buffer,9);

    }
}

