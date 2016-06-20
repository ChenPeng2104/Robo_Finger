/**
  ******************************************************************************
  * @file    USART.h
  * @author  Luzian
  * @version V1.0.0          modified by Keil for MDK-ARM V5.0
  * @date    7-Dezenmber-2015
  * @brief   the head file for 485
	******************************************************************************
**/

#ifndef    _USART_FOR_DYNAMIXEL_H_
#define    _USART_FOR_DYNAMIXEL_H_

#include "stm32f10x.h"
#include "stddef.h"
/* Exported types ------------------------------------------------------------*/
typedef struct
{
    u8 ID;
    u8 Insruction;
    u8 Len;      //为了编程的方便，此处的Len并非是协议中的LEN，而是Data[]的长度
    u8 Data[20];
    u8 Check_sum;

} Dynamixel_Structure;
typedef struct
{
    u8 front;
    u8 rear;
    u8 Data[100];
} QUE_Struct;

/*___Style__*/
#define MX_485 1
#define XL_TTL 2
#define Upper_CMD 3

//typedef enum
//{
//	ID,
//	P,
//	I,
//	D,
//	MAXSPEED,
//	OVERCURRENT,
//	OVERCURRENTTIME,
//
//	DATA_NUM,
//	WRITE_FLASH,
//	TESTSPEED,
//
//	MOTORDIRECTION_TEST,
//	MOTORDIRECTION,
//	FLASH_ACK,
//	REC_ACK,
//}DataType;

//#define PID1_P 0x20
//#define PID1_I 0x21
//#define PID1_D 0x22

//#define PID2_P 0x23
//#define PID2_I 0x24
//#define PID2_D 0x25

//#define PID3_P 0x26
//#define PID3_I 0x27
//#define PID3_D 0x28

//#define Send_Speed 10
//#define Send_Position_X 11
//#define Send_Position_Y 12
//#define Send_Yaw 13
//#define Send_Pitch 14
//#define Send_Roll 15
//#define Send_Number1 16
//#define Send_Number2 17
//#define Send_Number3 18
//#define Send_Number4 19
//#define Send_Number5 20
//#define Send_Number6 21
//#define Send_CPUProcesse 22
//#define Send_FreeHeap 23
//#define Send_FailNum 24
//#define Send_CatchTheTopSuccess 25
//#define Send_CatchTheTopFail 26

void USART_TTL_Init(int baud_rate);
void USART2_Init(int baud_rate);
Dynamixel_Structure* Receive_Data(u8 style);

void Send_Data_485(Dynamixel_Structure data);
void Send_Data_TTL(Dynamixel_Structure data);

#endif

