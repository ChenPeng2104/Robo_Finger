

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

#define HALF_POSITION 460    // �е�λ������ֵ

/* ���� ����ָ������ΧΪ 0-100%  0��ʾ�����������100��ʾ���ɵ�� */
//��ָ 0%��ʵ����С�Ƕȣ� 100%Ϊʵ�����Ƕ�
#define MIDLE_FINGER 30
#define MIDLE_MIN    123
#define MIDLE_MAX    1023

//��ָ 0%��ʵ����С�Ƕȣ� 100%Ϊʵ�����Ƕ�
#define RIGHT_FINGER 32
#define RIGHT_MIN    123
#define RIGHT_MAX    1023

//��ָ 0%��ʵ����С�Ƕȣ� 100%Ϊʵ�����Ƕ�
#define LEFT_FINGER  33
#define LEFT_MIN     0
#define LEFT_MAX		 850

#define SPIN_FINGER  31
#define SPIN_MIN 0//��ָ������С  �ﵽƽ��
#define SPIN_MAX 350  //��ָ�������  ��ָ���

void GrabCup(void*pvParameters);
void Single_Finger(void*pvParameters);//1 ��ָ  ����(direction=1) or ���� (direction =others)
void Double_Finger(void*pvParameters);//2 ˫ָ  ����(direction=1) or ���� (direction =others)
void Three_Finger(void*pvParameters); //3 ��ָ  ����(direction=1) or ���� (direction =others)

extern void Hand_Reset(void*pvParameters);   //4 ��λ��  ָ���ĸ�λ״̬
extern void Hand_Open(void*pvParameters);    //5 �ſ���  ���ô�����ǰ��Ҫ�ȸ�λ��е��

void FastReset(void*pvParameters);   				 //6 ���ٻظ�
void Vertical_Hold(void*pvParameters);//7 ��������  ��ʼ״̬Ϊ��λ keep power
void Clamp_Card(void*pvParameters);   //8 �п�-��ʼ״̬Ϊ��λ״̬
void Stone_Gesture(void*pvParameters);			 //9	ʯͷ
void Scissors_Gesture(void*pvParameters);		 //10 ����
void Cloth_Gesture(void*pvParameters);			 //11 ��


extern void Play_Game_1(void*pvParameters);  //11 Game1 ʯͷ������
#endif
