/**
  ******************************************************************************
  * @file    Dynamixel.h
  * @author  Luzian
  * @version V1.0.0          modified by Keil for MDK-ARM V5.0
  * @date    7-Dezenmber-2015
  * @brief   the head file for Dynamixel
	******************************************************************************
**/

#ifndef    _DYNAMIXEL_H_
#define    _DYNAMIXEL_H_

#include "stm32f10x.h"
#include "USART_for_Dynamixel.h"

/*__Instruction__*/
//MX
#define PING       0x01
#define READ_DATA  0x02
#define WRITE_DATA 0x03
#define REG_WRITE  0x04
#define ACTION     0x05
#define FACTORY_RESET 0x06
#define SYNC_WRITE 0x83
//XL320
#define Reboot     0x08
#define Status     0x55
#define Sync_READ  0x82
#define Bulk_Read  0x92
#define Bulk_Write 0x93

/*__ERROR__*/
//MX
#define Input_Voltage_Error 0x01
#define Angle_Limit_Error   0x02
#define Overheating_Error   0x04
#define Range_Error         0x08
#define Checksum_Error      0x10
#define Overload_Error      0x20
#define Instruction_Error   0x40
#define None_Error          0x00

/*__Baud_Rate__*/
//MX
typedef enum
{
    Baud_9600   = 207,
    Baud_19200  = 103,
    Baud_57600  = 34,
    Baud_115200 = 16,
    Baud_200000 = 9,

    Baud_9600_XL   = 0,
    Baud_57600_XL  = 1,
    Baud_115200_XL = 2,
    Baud_1MBPS_XL  = 3,
} Baud_Type;
//#define Baud_9600   207
//#define Baud_19200  103
//#define Baud_57600  34
//#define Baud_115200 16
//#define Baud_200000 9
////XL_320
//#define Baud_9600_XL   0
//#define Baud_57600_XL  1
//#define Baud_115200_XL 2
//#define Baud_1MBPS_XL  3

//XL
//#define Red_LED   0x01
//#define Green_LED 0x02
//#define Yellow_LED 0x03
//#define Blue_LED 0x04
//#define Pink_LED  0x05
//#define Blue_Green_LED  0x06
//#define White_LED 0x07
//#define Reset_LED 0x00

#define Turn_On  1
#define Turn_Off 0

typedef enum
{
    LED_Reset=0,
    LED_Red,
    LED_Green,
    LED_Yellow,
    LED_Blue,
    LED_Pink,
    LED_Blue_Green,
    LED_White,
} LED_Type;



typedef enum
{
    Model_Number_L,
    Model_Number_H,
    Version_Firmware,
    ID,
    Baud_Rate,
    Return_Delay_Time,
    CW_Angle_Limit_L,

} Address_enum;


typedef struct EEPROM_Value
{   //Default_value  Brief
    u16 Model_Num;
    u8 Version_Fir;

    u8 ID;                //     1         0~254(0XFE).    Broadcast ID is 254(0xFE)
    u8 Baud_Rate;         //    34         "16"-115200  "34"-57600 "103"-19200 "207"-9600
    u8 Return_Delay_Time; //    250        0 to 254 (0xFE) uint is 2us
    u16 CW_Angle_Limit;   //    0x0000     "Wheel Mode"  both are 0   "Multi-turn Mode"  both are 4095
    u16 CCW_Angle_Limit;  //    0x0fff     "Joint Mode"  neither at 0
    u8 Highest_Temperature;//   80         no need to change
    u8 L_Limit_Voltage;    //   60         min 50  unit is 0.1V
    u8 H_Limit_Voltage;    //   160        max 160 unit is 0.1V
    u16 Max_Torque;        //   0x03ff     0x0000~0x03ff
    u8 Statue_Return_Level;//   2          "0"-no return "1"-return only read cmd "2"return all cmd
    u8 Alarm_LED;          //   36                 bit7  bit6  bit5    bit4     bit3  bit2     bit1   bit0
    u8 Alarm_Shutdown;     //   36         error    0   cmd  Overload CheckSum  cmdRange  Heating  Angle  Voltage
    u16 Mulit_Turn_Offset; //   0x0000     Adjusts position(zeroing) -24576 to 24576
    u8 Resolution_Divider; //   1 (1~4)    Present position = (Real Position / Resolution Divider) + Multi-turn Offset
    u8 Control_Mode;       // Only have in XL-320, 1-wheel 2-Join
} EEPROM_Struct;
typedef enum
{
    /*  		  	 EEPROM 		 	  */
    /*read onlu*/
    model_number = 0,	//0
    Version_Fir,			//1
    /* RW  */
    Id ,							//2
    baud_Rate ,				//3
    return_delaytime,	//4
    CWangle_limit,		//5
    CCWangle_limit,		//6
    control_mode,			//7
    limit_temperature,//8
    lower_limitvoltage,//9
    upper_limitvoltage,//10
    max_torque,				//11
    return_level,			//12
    alarmshutdown,		//13
    /*  		  	 RAM 		 	  */
    /*RW*/
    torque_enable,		//14
    LED ,							//15
    D_Gain,						//16
    I_Gain,						//17
    P_Gain,						//18
    goal_position,		//19
    goal_speed,				//20
    goal_torque,			//21
    /*read only*/
    current_position,	//22
    current_speed,		//23
    current_load,			//24
    current_voltage,	//25
    present_tempurature,//26
    registered_instruction,//27
    moving,						//28
    hadware_error,		//29
    punch,						//30
} NameType;

typedef enum
{
    Wheel_Mode =1,
    Joint_Mode,
    Torque_Mode,

} ModeType;

typedef struct normal_use  //Ãæ¶Ô×ªÅÌ Ë³Ê±Õë-¼õÐ¡ ÄæÊ±Õë-Ôö´ó
{   //Default_value
    u8 New_ID;
    u8 ID;
    u8 Instruction;
    u8 Address;           //0x1E
    u8 Len;

    u16 Position;       //          0 to 4095 (0xFFF) unit is 0.088 degree
    u16 Speed;          //          0~1023 (0X3FF)    unit is about 0.114rpm.
    u16 Torque;         //0x3ff     0 to 1023 (0x3FF) unit is about 0.1%.
    u8 Direction;       //Ðý×ª·½Ïò -1 ÎªÄæÊ±Õë  1ÎªË³Ê±Õë
    ModeType Work_Mode; // Joint_Mode-1,Wheel_Mode-2,Torque_Mode-3
    u8  Mode_Status;    //Turn_On  /  Turn_Off but Only useful to the Torque_Mode
    u8  Return_Level;   // Level:0-none 1-read 2-all
    Baud_Type Baud;     //9600~2M
    LED_Type LED;
    u8  Channel;         // TTL-2  485-1
} Control_Struct;

typedef struct Present_status
{
    u8 ID;
    u8 error;
//		u8 Address;       //0X24
//		u8 Len;
    u16 Present_Position;
    u16 Present_Speed;
    u16 Present_Load;
    float Present_Voltage;
    u8 Present_Temperature;
    u8 Registered;    //  Means if Instruction is registered
    u8 Moving;        //  Means if there is any movement
    u8 Hardware_error;//Only have in XL_320

} Status_Struct;

typedef struct unnormal_use
{
    //0x18
    u8 Torque_Enable; //0     Torque On/Off
    u8 LED;           //0     LED On/Off
    u8 D_Gain;        //0
    u8 I_Gain;        //0
    u8 P_Gain;        //32
    //0x2f
    u8 Lock;          //0     Locking EEPROM
    u16 Punch;        //0     0x00~0x3FF  Current to drive motor is at minimum.
    u16 Current;      //0     I = ( 4.5mA ) * (CURRENT – 2048 ) in amps unit (A).
    u8 Torque_Control_Mode_Enable;  //0     Goal Torque cannot be bigger than Torque Limit
    u16 Goal_Torque;                //0     0 ~ 2047 (0x7FF)  unit is 4.5mA. 0~1023ÄæÊ±Õë
    u8 Goal_Acceleration;           //0     0~254(0XFE) 8.583 Degree / sec^2.
} Other_Struct;



//¶ÔÍâµÄÈý¸öÈ«¾Ö±äÁ¿µÄ½Ó¿Ú
extern EEPROM_Struct Dynl_Init;      //ÉèÖÃEEPROMÊ±ÐèÒª
extern Status_Struct Present_Status; //±íÊ¾µ±Ç°¶æ»úµÄ×´Ì¬
extern Control_Struct Goal;     //ÉèÖÃ¶æ»úµÄËÙ¶ÈºÍÎ»ÖÃ

extern void SYNC_Finger(float position_midle,u16 speed_midle,float position_right,u16 speed_right,float position_left,u16 speed_left);//Only use to control Fingers
extern void SYNC_CMD(u8 len,u8 data[]);
extern void Reset_ID(u8 old_id,u8 new_id);//ÉèÖÃÄ³¸ö¶æ»úµÄID
extern void PING_CMD(u8 id,u8 channel);              //PINGÖ¸Áî È·ÈÏÍ¨ÐÅÁ´½Ó³É¹¦
extern void Set_Goal_Value(u8 id,u16 position,u16 speed);     //Í¬Ê± ¸ü¸Ä angle ºÍ Speed
extern void Set_Baud_Rate(u8 id,u8 new_baud_rate); //ÉèÖÃÄ³¸ö¶æ»úµÄ²¨ÌØÂÊ
extern void Set_Return_Level(u8 id,u8 level);      // Level:0-none 1-read 2-all
extern void Set_XL_LED(u8 id,LED_Type led);     //ÁÁÃðµÆ ¿ØÖÆ
u8 DataOPE(u8 id,u8 instruction,NameType Name,u16 parm);
void WriteData(u8 id,NameType Name,int LEN,...);
//void WriteData(u8 id,NameType Name,u16 Postion,u16 speed);
u16 ReadData(u8 id,NameType Name);
void ReadDataS(u8 id,NameType Name,u8 len,u16 *extractdata);
int Detect_CMD(void);
#endif

