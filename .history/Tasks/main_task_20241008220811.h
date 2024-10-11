/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-10-06 19:07:55
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-10-08 17:34:50
 * @FilePath: \HW1\Tasks\main_task.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

/**
*******************************************************************************
* @file      ：<file>.h
* @brief     :
* @history   :
*  Version     Date            Author          Note
*  V0.9.0      yyyy-mm-dd      <author>        1. <note>
*******************************************************************************
* @attention :
*******************************************************************************
*  Copyright (c) 2024 Hello World Team，Zhejiang University.
*  All Rights Reserved.
*******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_TASK_HPP_
#define __MAIN_TASK_HPP_

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */
/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/

struct CANCommData
{
    uint32_t tick;
    float value1;
    uint8_t value2;
    bool flag1;
    bool flag2;
    bool flag3;
    bool flag4;
};

void RobotInit();

void MainInit(void);

void MainTask(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MAIN_TASK_HPP_ */