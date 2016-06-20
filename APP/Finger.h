

#ifndef    _FINGER_H_
#define    _FINGER_H_

#include "stm32f10x.h"
#include "stdlib.h"
#include "time.h"

#include "Dynamixel.h"

/*___________FreeRTOS___________*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "portable.h"
#include "FreeRTOSConfig.h"

#define LED_RED  GPIO_Pin_5
#define LED_BLUE GPIO_Pin_3
#define LED_GREEN GPIO_Pin_4
#define Set_LED(x)   GPIO_ResetBits(GPIOB,x)
#define Reset_LED(x) GPIO_SetBits(GPIOB,x)

#define HALF_POSITION 460    // 中点位置增量值

/* 定义 当手指收缩范围为 0-100%  0表示收缩到最紧，100表示放松到最开 */
//中指 0%是实际最小角度， 100%为实际最大角度
#define MIDLE_FINGER 30
#define MIDLE_MIN    123
#define MIDLE_MAX    1023

//右指 0%是实际最小角度， 100%为实际最大角度
#define RIGHT_FINGER 32
#define RIGHT_MIN    123
#define RIGHT_MAX    1023

//左指 0%是实际最小角度， 100%为实际最大角度
#define LEFT_FINGER  33
#define LEFT_MIN     0
#define LEFT_MAX		 850

#define SPIN_FINGER  31
#define SPIN_MIN 0//手指自旋最小  达到平行
#define SPIN_MAX 350  //手指自旋最大  两指相对

void GrabCup(void*pvParameters);
void Single_Finger(void*pvParameters);//1 单指  收缩(direction=1) or 放松 (direction =others)
void Double_Finger(void*pvParameters);//2 双指  收缩(direction=1) or 放松 (direction =others)
void Three_Finger(void*pvParameters); //3 三指  收缩(direction=1) or 放松 (direction =others)

extern void Hand_Reset(void*pvParameters);   //4 复位手  指定的复位状态
extern void Hand_Open(void*pvParameters);    //5 张开手  调用此任务前需要先复位机械手

void FastReset(void*pvParameters);   				 //6 快速回复
void Vertical_Hold(void*pvParameters);//7 竖立提起  初始状态为复位 keep power
void Clamp_Card(void*pvParameters);   //8 夹卡-起始状态为复位状态
void Stone_Gesture(void*pvParameters);			 //9	石头
void Scissors_Gesture(void*pvParameters);		 //10 剪刀
void Cloth_Gesture(void*pvParameters);			 //11 布


extern void Play_Game_1(void*pvParameters);  //11 Game1 石头剪刀布
#endif
