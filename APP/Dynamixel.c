/**
  ******************************************************************************
  * @file    	Dynamixel.C
  * @author  Luzian
  * @version V1.0.0          modified by Keil for MDK-ARM V5.0
  * @date    31-Dezenmber-2015
  * @brief   all the Basical communication for controlling the Dynamixel
	******************************************************************************
**/
#include "Dynamixel.h"
#include "Finger.h"
#include "stdarg.h"

EEPROM_Struct Dynl_Init;      //设置舵机基本参数的变量EEPROM
Status_Struct Present_Status; //舵机当前状态信息的变量
Control_Struct Goal;     //舵机转动速度和位置的变量
const NameType Addr[31][2]=
{
    {(NameType)0,(NameType)2},	//这么多 (NameType) 完全是消除warnning!!!
    {(NameType)2,(NameType)1},
    {(NameType)3,(NameType)1},
    {(NameType)4,(NameType)1},
    {(NameType)5,(NameType)1},
    {(NameType)6,(NameType)2},
    {(NameType)8,(NameType)2},
    {(NameType)11,(NameType)1},
    {(NameType)12,(NameType)1},
    {(NameType)13,(NameType)1},
    {(NameType)14,(NameType)1},
    {(NameType)15,(NameType)2},
    {(NameType)17,(NameType)1},
    {(NameType)18,(NameType)1},
    {(NameType)24,(NameType)1},
    {(NameType)25,(NameType)1},
    {(NameType)27,(NameType)1},
    {(NameType)28,(NameType)1},
    {(NameType)29,(NameType)1},
    {(NameType)30,(NameType)2},
    {(NameType)32,(NameType)2},
    {(NameType)35,(NameType)2},
    {(NameType)37,(NameType)2},
    {(NameType)39,(NameType)2},
    {(NameType)41,(NameType)2},
    {(NameType)45,(NameType)1},
    {(NameType)46,(NameType)1},
    {(NameType)47,(NameType)1},
    {(NameType)49,(NameType)1},
    {(NameType)50,(NameType)1},
    {(NameType)51,(NameType)2},
};

/**************************************************************/
/** @name   Set_Goal_Value()
  * @brief  目标值设置 一般为 速度 角度 力矩三个参数的设置更新
  * @param  Control_Struct goal  此结构体由速度 角度 力矩变量组成
  * @retval None
  */
/**************************************************************/
void Set_Goal_Value(u8 id,u16 position,u16 speed)
{
    WriteData(id,goal_position,2,position,speed);
}
//重设ID号
void Reset_ID(u8 old_id,u8 new_id)
{
    DataOPE(old_id,WRITE_DATA,Id,new_id);

}
//重设某个ID舵机的波特率
void Set_Baud_Rate(u8 id,u8 new_baud_rate)
{
    DataOPE(id,WRITE_DATA,baud_Rate,new_baud_rate);
}
//设置LED
void Set_XL_LED(u8 id,LED_Type led)
{
    DataOPE(id,WRITE_DATA,LED,led);
}

// 设置返回模式 Level:0-none 1-read 2-all
void Set_Return_Level(u8 id,u8 level)
{
    DataOPE(id,WRITE_DATA,return_level,level);
}
/*________________________特殊指令（写）_______________________*/
/**************************************************************/
/** @name   SYNC_CMD()
  * @brief  可以同时设置多个舵机在同个地址的参数
  * @param  len 数组数组的长度
            data[] 数据数组（包括地址）
            规则同查看舵机的文档中的关于SYNC指令的组成规则部分
* @retval None
  */
/**************************************************************/
void SYNC_CMD(u8 len,u8 data[])
{
    u8 i;
    Dynamixel_Structure cmd;
    cmd.ID = 0xfe;  // 广播的ID号
    cmd.Insruction = SYNC_WRITE;
    cmd.Len = len;
    for(i=0; i<len; i++)
    {
        cmd.Data[i] = data[i];
    }
    Send_Data_TTL(cmd);
}
//专门为手指控制的函数 三个手指同一输出
/**************************************************************/
/** @name   SYNC_CMD()
  * @brief  专门为手指控制的函数 三个手指同一输出
  * @param  position1    MIDLE_FINGER Pos
						position2		 RIGHT_FINGER Pos
						position3		 LEFT_FINGER  Pos
            规则同查看舵机的文档中的关于SYNC指令的组成规则部分
* @retval None
  */
/**************************************************************/
void SYNC_Finger(float position_midle,u16 speed_midle,float position_right,u16 speed_right,float position_left,u16 speed_left)
{
    u8 i=0;
    u16 PosM,PosR,PosL;
    Dynamixel_Structure SYNCcmd;

    if(position_midle!=0)
        PosM =(u16)(MIDLE_MIN+position_midle*(MIDLE_MAX-MIDLE_MIN)/100);
    else
        PosM = MIDLE_MIN;
    if(PosM<MIDLE_MIN)
        PosM = MIDLE_MIN;
    else if(PosM> MIDLE_MAX)
        PosM = MIDLE_MAX;

    if(position_right!=0)
        PosR =(u16)(RIGHT_MIN+position_right*(RIGHT_MAX-RIGHT_MIN)/100);
    else
        PosR = RIGHT_MIN;
    if(PosR < RIGHT_MIN)
        PosR = RIGHT_MIN;
    else if(PosR>RIGHT_MAX)
        PosR= RIGHT_MAX;

    if(position_left!=0)
        PosL = (u16)(LEFT_MAX-position_left*(LEFT_MAX-LEFT_MIN)/100);
    else
        PosL=LEFT_MAX;

    if(PosL>LEFT_MAX)
        PosL= LEFT_MAX;

    SYNCcmd.ID = 0xfe;  // 广播的ID号
    SYNCcmd.Insruction = SYNC_WRITE;
    SYNCcmd.Data[i++] = 0x1E;     //地址低8位
    SYNCcmd.Data[i++] = 0x00;
    SYNCcmd.Data[i++] = 0x04;			//读取数据长度低8位
    SYNCcmd.Data[i++] = 0x00;
    SYNCcmd.Data[i++] = MIDLE_FINGER;
    SYNCcmd.Data[i++] = (PosM&0xff);
    SYNCcmd.Data[i++] = ((PosM>>8)&0xff);
    SYNCcmd.Data[i++] = (speed_midle&0xff);				//默认速度1023
    SYNCcmd.Data[i++] = ((speed_midle>>8)&0xff);

    SYNCcmd.Data[i++] = RIGHT_FINGER;
    SYNCcmd.Data[i++] = (PosR&0xff);
    SYNCcmd.Data[i++] = ((PosR>>8)&0xff);
    SYNCcmd.Data[i++] = (speed_right&0xff);
    SYNCcmd.Data[i++] = ((speed_right>>8)&0xff);

    SYNCcmd.Data[i++] = LEFT_FINGER;
    SYNCcmd.Data[i++] = (PosL&0xff);
    SYNCcmd.Data[i++] = ((PosL>>8)&0xff);
    SYNCcmd.Data[i++] = (speed_left&0xff);
    SYNCcmd.Data[i++] = ((speed_left>>8)&0xff);
    SYNCcmd.Len = i;
    Send_Data_TTL(SYNCcmd);
}

/**************************************************************/
/** @name   DataOPA()
  * @brief  发出读/写指令
  * @param  id 所要设置参数的舵机的id号
            ins 指令 读/写
			parm  写：要写入的参数   读：读取数据个数
  * @retval None
  */
/**************************************************************/
u8 DataOPE(u8 id,u8 instruction,NameType Name,u16 parm)
{
    u8 k=0;
    Dynamixel_Structure cmd;
    cmd.ID = id;

    switch(instruction)
    {
    case WRITE_DATA:
    {
        cmd.Insruction = WRITE_DATA;
        cmd.Data[k++]= Addr[Name][0];   //低地址
        cmd.Data[k++]=0x00;
        if(Addr[Name][1]==1)	//查询数据占用几个Byte
            cmd.Data[k++] = (u8)parm; //写入参数
        else
        {
            cmd.Data[k++] = (u8)(parm&0xff);     			//写入参数
            cmd.Data[k++] = (u8)((parm>>8)&0xff);     			//写入参数
        }
        cmd.Len = k;
        Send_Data_TTL(cmd);
        break;
    }
    case READ_DATA:
    {
        cmd.Insruction = READ_DATA;
        cmd.Data[k++]= Addr[Name][0];      //低地址
        cmd.Data[k++]=0x00;
        cmd.Data[k++]=(u8)parm;    				//此时 parm 代表读取长度   低长度
        cmd.Data[k++]=0x00;
        cmd.Len = k;
        Send_Data_TTL(cmd);
        break;
    }
    case PING:
    {
        Dynamixel_Structure cmd,*data=NULL;
        cmd.ID = id;
        cmd.Insruction = PING;
        cmd.Len = 0x00;
        Send_Data_TTL(cmd);
        while( (data = Receive_Data(XL_TTL))==NULL||(*data).ID !=id); //获取返回的信息
        if((*data).Data[0])		//判断Error Byte
            return 0;
        else
            return 1;						//在线 且无错误信息
    }
    case REG_WRITE:
    {
        cmd.Insruction = REG_WRITE;
        cmd.Data[k++]= Addr[Name][0];   //低地址
        cmd.Data[k++]=0x00;
        if(Addr[Name][1]==1)	//查询数据占用几个Byte
            cmd.Data[k++] = (u8)parm; //写入参数
        else
        {
            cmd.Data[k++] = (u8)(parm&0xff);     			//写入参数
            cmd.Data[k++] = (u8)((parm>>8)&0xff);     			//写入参数
        }
        cmd.Len = k;
        Send_Data_TTL(cmd);
        break;
    }
    case ACTION:
    {
        cmd.ID = id;
        cmd.Insruction = ACTION;
        cmd.Len = 0x00;
        Send_Data_TTL(cmd);
        break;
    }
    case Reboot:
    {
        cmd.ID = id;
        cmd.Insruction = Reboot;
        cmd.Len = 0x00;
        Send_Data_TTL(cmd);
        break;
    }
    case FACTORY_RESET:
    {
        cmd.ID = id;
        cmd.Insruction = FACTORY_RESET;
        cmd.Data[0] = 0x02;
        cmd.Len = 0x01;
        Send_Data_TTL(cmd);
    }
    break;
    default :
        return 1;
    }
    return 1;
}

/**************************************************************/
/** @name   WriteData()
  * @brief  写多个数据
  * @param  id 所要设置参数的舵机的id号
            NameType 寄存器名字  查表头文件表格
						parm  写：要写入的参数   读：读取数据个数
						len   写寄存器的个数  注意它是u16和变参的变量类型是一致的
						... ： 变参  数据类型必须为u16
  * @retval None
  */
/**************************************************************/
void WriteData(u8 id,NameType Name,int LEN,...)
{
    u8 k=0,l=0;
    u16 temp=0,i=0;
    Dynamixel_Structure cmd,*data=NULL;
    va_list ap;				//声明 变参链表
    va_start(ap,LEN);	//指向 变参地址
    cmd.ID = id;
    cmd.Insruction = WRITE_DATA;
    cmd.Data[k++]=Addr[Name][0];		//地址
    cmd.Data[k++]=0x00;
    for(l=0; l<LEN; l++)
    {
        if(Addr[Name+l][1]==1)
            cmd.Data [k++] = (u8)((va_arg(ap, int))&0xff);   //取出变参
        else
        {
            temp =va_arg(ap, int);
            cmd.Data [k++] = (u8)(temp&0xff);   //取出变参
            cmd.Data [k++] = (u8)((temp>>8)&0xff);   //取出变参
        }
    }
    va_end(ap);
    cmd.Len = k;     //数据个数+两个地址byte
    Send_Data_TTL(cmd);
    if(id==SPIN_FINGER)
    {
        if(i)
        {
            while((((data = Receive_Data(XL_TTL))==NULL)||((*data).ID !=id)||(*data).Data[0] !=0)&&(i<20000))
            {
                i++;
                if(i==19999)
                {
                    Send_Data_TTL(cmd);
                    i=0;
                }
            }
        }
    }
}
//void WriteData(u8 id,NameType Name,u16 Postion,u16 speed)
//{
//    u8 k=0,l=0;
//    u16 i,temp=0;
//    Dynamixel_Structure cmd, *data=NULL;
//    cmd.ID = id;
//    cmd.Insruction = WRITE_DATA;
//    cmd.Data[k++]=Addr[Name][0];		//地址
//    cmd.Data[k++]=0x00;
//		cmd.Data [k++] = (u8)(Postion&0xff);   //取出变参
//		cmd.Data [k++] = (u8)((Postion>>8)&0xff);   //取出变参
//		cmd.Data [k++] = (u8)(speed&0xff);   //取出变参
//		cmd.Data [k++] = (u8)((speed>>8)&0xff);   //取出变参
//    cmd.Len = k;     //数据个数+两个地址byte
//    Send_Data_TTL(cmd);
//		if(id==SPIN_FINGER)
//		{
//			if(i)
//			{
//				while((((data = Receive_Data(XL_TTL))==NULL)||((*data).ID !=id)||(*data).Data[0] !=0)&&(i<20000))
//				{
//					i++;
//					if(i==19999)
//					{
//						Send_Data_TTL(cmd);
//						i=0;
//					}
//				}
//			}
//		}
//}
/**************************************************************/
/** @name   ReadEEPData()
  * @brief  读取单个寄存器数据
  * @param  id 所要设置参数的舵机的id号
            NameType 寄存器名字  查表头文件表格

  * @retval 返回单个寄存器值
  */
/**************************************************************/
u16 ReadData(u8 id,NameType Name)
{
    Dynamixel_Structure *data=NULL;
    u16 status,i;
    DataOPE(id,READ_DATA,Name,Addr[Name][1]);
    while( (((data = Receive_Data(XL_TTL))==NULL)||((*data).ID !=id)||(((*data).Len-4)!=Addr[Name][1]))&&(i<7000))
        i++; //获取返回的信息
    if(i!=7000)
    {
        if(Addr[Name][1] == 1)		//判断寄存器长度
        {
            status =(*data).Data[0];
        }
        else
        {
            status = ((*data).Data[1]<<8)+(*data).Data[0];
        }
    }
    return status;
}
/**************************************************************/
/** @name   ReadEEPData()
  * @brief  读取多个寄存器数据
  * @param  id 所要设置参数的舵机的id号
            NameType 寄存器名字  查表头文件表格
						len   读取寄存器个数 （不是数据个数  而且要注意 几个寄存器地址必须是连续的）
						*extractdata   存数据地址
  * @retval none
  */
/**************************************************************/
void ReadDataS(u8 id,NameType Name,u8 len,u16 *extractdata)
{
    Dynamixel_Structure *data=NULL;
    u8 temp=0,l;
    u16 i=0;
    for(l=0; l<len; l++)
    {
        temp=temp+Addr[Name+l][1];		//用k缓存 由寄存器个数计算出来的 数据个数
    }
    DataOPE(id,READ_DATA,Name,temp);
    while(( ((data = Receive_Data(XL_TTL))==NULL||(*data).ID !=id))&&i<7000) //获取返回的信息 //获取返回的信息
        i++;
    temp=0;  //缓存k 清零
    if(((*data).ID==id)&&(data !=NULL))
    {
        for(l=0; l<len; l++)
        {
            if(Addr[Name+l][1] == 1)		//判断寄存器长度
                extractdata[l] =(*data).Data[temp++];
            else
            {
                extractdata[l] = (u16)(((*data).Data[temp+1])<<8)+(u16)(((*data).Data[temp]));
                temp=temp+2;
            }
        }
    }
}

int Detect_CMD(void)
{
    u16 i;
    Dynamixel_Structure *data=NULL;
    while((((data = Receive_Data(Upper_CMD))==NULL)||((*data).ID !=250)||(*data).Len!=1)&&(i<200))
        i++;
    if(i < 200)			//超时判断
        return (*data).Data[0];
    else
        return -1;
}
