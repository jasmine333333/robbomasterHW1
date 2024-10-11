#ifndef _HW_CAN_H_
#define _HW_CAN_H_
#ifdef __cplusplus
extern "C" {
#endif
/* ------------------------------ Include ------------------------------ */
#include "stm32f103xb.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "can.h"
/* ------------------------------ Macro Definition ------------------------------ */



/* ------------------------------ Type Definition ------------------------------ */



/* ------------------------------ Extern Global Variable ------------------------------ */



/* ------------------------------ Function Declaration (used in other .c files) ------------------------------ */

void CanFilter_Init();

void CAN_Send_Msg(CAN_HandleTypeDef* hcan, uint8_t* msg, uint32_t id, uint8_t len);


#ifdef __cplusplus
}
#endif
#endif
