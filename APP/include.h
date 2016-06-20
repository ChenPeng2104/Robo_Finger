/**
  ******************************************************************************
  * @file    include.h
  * @author  Luzian
  * @version V1.0.0          modified by Keil for MDK-ARM V5.0
  * @date    7-Dezenmber-2015
  * @brief   the head file for main
	******************************************************************************
**/

#ifndef    _INCLUDES_H_
#define    _INCLUDES_H_



/*___________APP________________*/
#include "Dynamixel.h"
#include "Finger.h"

/*___________BSP________________*/
#include "stm32f10x.h"
#include "USART_for_Dynamixel.h"
#include "ADC.h"

/*___________FreeRTOS___________*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "portable.h"
#include "FreeRTOSConfig.h"


//typedef struct
//{
//
//}Control_Various;

#endif
