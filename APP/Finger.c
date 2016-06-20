/**
  ******************************************************************************
  * @file    Finger.C
  * @author  Luzian
  * @version V1.0.0          modified by Keil for MDK-ARM V5.0
  * @date    31-Dezenmber-2015
  * @brief   all the funtions and motivation for the Finger
  Actention   :  1: there are a lot of Delay function  **Base on FreeRTOS**
                 2: use MX-320 Steering engine
	******************************************************************************
**/

#include "Finger.h"
extern u16  Delay_Time_Set;
extern u8 Finish_Flag[];


//8 夹卡-起始状态为复位状态
void Clamp_Card(void*pvParameters)
{
    //Step 1 //放松
//    SYNC_Finger(80,300,300+1024);
//    vTaskDelay(1000/portTICK_RATE_MS);//Delay
//    //Step 2 //转动
//    Set_Goal_Value(31,760,500);
//    vTaskDelay(500/portTICK_RATE_MS);//Delay
//   //Step 3  //暂停一下
//    SYNC_Finger(0,0,0);
//    vTaskDelay(1000/portTICK_RATE_MS);//Delay

//   //Step 3  //收紧
//    SYNC_Finger(0,115+1024,100);
//    vTaskDelay(4000/portTICK_RATE_MS);//Delay
//    //Step 3 //hold
//    SYNC_Finger(0,80+1024,80);

    Finish_Flag[8] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}
//7 竖立提起  初始状态为复位
void Vertical_Hold(void*pvParameters)
{
    //Step 1 //放松
    SYNC_Finger(MIDLE_MAX,800,RIGHT_MAX,800,LEFT_MAX,800);
    vTaskDelay(5000);

//
//    //Step 2 //转动
    Set_Goal_Value(SPIN_FINGER,600+175,800);  //600
    vTaskDelay(5000);//Delay 1000
//
//    //Step 3 //暂停一下
    SYNC_Finger(500,800,500,800,500,800);
    vTaskDelay(5000);//Delay 1000
//
//
//	//Step 4 //收紧
    SYNC_Finger(300,50,300,50,300,50);
    vTaskDelay(5000);//Delay 1000
    Finish_Flag[7] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}
//6 收紧手 ，最后keep收紧
void Hand_Close(void*pvParameters)
{

    SYNC_Finger(MIDLE_MIN,800,RIGHT_MIN,800,LEFT_MIN,800);
    vTaskDelay(10000);

    Finish_Flag[6] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}
//5 张开手  调用此任务前需要先复位机械手
void Hand_Open(void*pvParameters)
{
    SYNC_Finger(60,200,60,200,60,200);	//手指直立
		vTaskDelay(10000);//Delay 500ms
    //Step 1  //两次改变是为了矫正齿轮间隙误差
    Set_Goal_Value(SPIN_FINGER,SPIN_MIN+100,700);	//最大自转角
    vTaskDelay(2000);//Delay 500ms
    Set_Goal_Value(SPIN_FINGER,SPIN_MIN,700);	//最小自传角
    vTaskDelay(2000);//Delay 500ms
		
		SYNC_Finger(80,200,80,200,80,200);
		vTaskDelay(3000);
    Finish_Flag[5] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}
//4 复位手指到指定的复位状态
void Hand_Reset(void*pvParameters)
{
		SYNC_Finger(60,500,60,500,60,500);	//手指直立
		vTaskDelay(3000);//Delay 500ms
    //Step 1  //两次改变是为了矫正齿轮间隙误差
    Set_Goal_Value(SPIN_FINGER,SPIN_MIN+200,700);	//最大自转角
    vTaskDelay(3000);//Delay 500ms
    Set_Goal_Value(SPIN_FINGER,SPIN_MIN,700);	//最小自传角
    vTaskDelay(3000);//Delay 500ms
	
		SYNC_Finger(100,500,100,500,100,500);	//手指直立
		vTaskDelay(3000);
		SYNC_Finger(0,500,0,500,0,500);	
		vTaskDelay(3000);
    Finish_Flag[4] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}

void FastReset(void*pvParameters)
{
		Set_Goal_Value(SPIN_FINGER,SPIN_MIN,700);	//最小自传角
		vTaskDelay(1000);
		SYNC_Finger(0,700,0,700,0,700);	
		Finish_Flag[6] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}
//3 //收紧(direction=1)or 放松(direction=others)
void Three_Finger(void*pvParameters)
{
    //Step 1 //收紧
//	    SYNC_Finger(Goal.Speed+1024,Goal.Speed+1024,Goal.Speed);
//    else	//or 放松
//		SYNC_Finger(Goal.Speed,Goal.Speed,Goal.Speed+1024);
//    vTaskDelay(Delay_Time_Set/portTICK_RATE_MS);//Delay
//    //Step 2 //停止
//    SYNC_Finger(0,0,0);

    Finish_Flag[3] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}
//2 双指  收缩(direction=1) or 放松 (direction =others)
void Double_Finger(void*pvParameters)
{
    //Step 1 //收缩
//	if(Goal.Direction==1)
//        SYNC_Finger(0,Goal.Speed+1024,Goal.Speed);
//	//Step 2 //放松
//	else
//		SYNC_Finger(0,Goal.Speed,Goal.Speed+1024);
//    vTaskDelay(Delay_Time_Set/portTICK_RATE_MS);//Delay

//    //Step 2 //停止
//    SYNC_Finger(0,0,0);

    Finish_Flag[2] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}

//1 单指
void Single_Finger(void*pvParameters)
{
    Set_Goal_Value(SPIN_FINGER,SPIN_MIN,700);	//最大自转角
		vTaskDelay(1000);
		SYNC_Finger(0,700,100,900,0,700);	//快速到达大概位置;
    Finish_Flag[2] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}

float EndPos1=35;
float EndPos2=35;
float EndPos3=35;
void GrabCup(void*pvParameters)
{
		SYNC_Finger(90,400,90,400,90,400);
		vTaskDelay(4000);
		Set_Goal_Value(SPIN_FINGER,SPIN_MIN,700);	//最大自转角
		vTaskDelay(3000);
		SYNC_Finger(50,300,50,300,50,300);	//快速到达大概位置;
		vTaskDelay(3000);
		SYNC_Finger(EndPos1,100,EndPos2,100,EndPos3,100);
		vTaskDelay(3000);
    Finish_Flag[1] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}

void Stone_Gesture(void*pvParameters)
{
		SYNC_Finger(5,400,5,400,5,400);
		vTaskDelay(3000);
		Finish_Flag[9] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}
void Scissors_Gesture(void*pvParameters)
{
		SYNC_Finger(5,1000,90,1000,90,1000);
		Set_Goal_Value(SPIN_FINGER,100,700);	//最大自转角
		vTaskDelay(3000);
		Finish_Flag[10] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}
void Cloth_Gesture(void*pvParameters)
{
		SYNC_Finger(90,1000,90,1000,90,1000);
		Set_Goal_Value(SPIN_FINGER,100,700);	//最大自转角
		vTaskDelay(3000);
		Finish_Flag[11] ++;  //标记完成
    vTaskDelete( NULL );//退出任务
}


