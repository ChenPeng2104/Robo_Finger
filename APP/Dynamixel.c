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

EEPROM_Struct Dynl_Init;      //���ö�����������ı���EEPROM
Status_Struct Present_Status; //�����ǰ״̬��Ϣ�ı���
Control_Struct Goal;     //���ת���ٶȺ�λ�õı���
const NameType Addr[31][2]=
{
    {(NameType)0,(NameType)2},	//��ô�� (NameType) ��ȫ������warnning!!!
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
  * @brief  Ŀ��ֵ���� һ��Ϊ �ٶ� �Ƕ� �����������������ø���
  * @param  Control_Struct goal  �˽ṹ�����ٶ� �Ƕ� ���ر������
  * @retval None
  */
/**************************************************************/
void Set_Goal_Value(u8 id,u16 position,u16 speed)
{
    WriteData(id,goal_position,2,position,speed);
}
//����ID��
void Reset_ID(u8 old_id,u8 new_id)
{
    DataOPE(old_id,WRITE_DATA,Id,new_id);

}
//����ĳ��ID����Ĳ�����
void Set_Baud_Rate(u8 id,u8 new_baud_rate)
{
    DataOPE(id,WRITE_DATA,baud_Rate,new_baud_rate);
}
//����LED
void Set_XL_LED(u8 id,LED_Type led)
{
    DataOPE(id,WRITE_DATA,LED,led);
}

// ���÷���ģʽ Level:0-none 1-read 2-all
void Set_Return_Level(u8 id,u8 level)
{
    DataOPE(id,WRITE_DATA,return_level,level);
}
/*________________________����ָ�д��_______________________*/
/**************************************************************/
/** @name   SYNC_CMD()
  * @brief  ����ͬʱ���ö�������ͬ����ַ�Ĳ���
  * @param  len ��������ĳ���
            data[] �������飨������ַ��
            ����ͬ�鿴������ĵ��еĹ���SYNCָ�����ɹ��򲿷�
* @retval None
  */
/**************************************************************/
void SYNC_CMD(u8 len,u8 data[])
{
    u8 i;
    Dynamixel_Structure cmd;
    cmd.ID = 0xfe;  // �㲥��ID��
    cmd.Insruction = SYNC_WRITE;
    cmd.Len = len;
    for(i=0; i<len; i++)
    {
        cmd.Data[i] = data[i];
    }
    Send_Data_TTL(cmd);
}
//ר��Ϊ��ָ���Ƶĺ��� ������ָͬһ���
/**************************************************************/
/** @name   SYNC_CMD()
  * @brief  ר��Ϊ��ָ���Ƶĺ��� ������ָͬһ���
  * @param  position1    MIDLE_FINGER Pos
						position2		 RIGHT_FINGER Pos
						position3		 LEFT_FINGER  Pos
            ����ͬ�鿴������ĵ��еĹ���SYNCָ�����ɹ��򲿷�
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

    SYNCcmd.ID = 0xfe;  // �㲥��ID��
    SYNCcmd.Insruction = SYNC_WRITE;
    SYNCcmd.Data[i++] = 0x1E;     //��ַ��8λ
    SYNCcmd.Data[i++] = 0x00;
    SYNCcmd.Data[i++] = 0x04;			//��ȡ���ݳ��ȵ�8λ
    SYNCcmd.Data[i++] = 0x00;
    SYNCcmd.Data[i++] = MIDLE_FINGER;
    SYNCcmd.Data[i++] = (PosM&0xff);
    SYNCcmd.Data[i++] = ((PosM>>8)&0xff);
    SYNCcmd.Data[i++] = (speed_midle&0xff);				//Ĭ���ٶ�1023
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
  * @brief  ������/дָ��
  * @param  id ��Ҫ���ò����Ķ����id��
            ins ָ�� ��/д
			parm  д��Ҫд��Ĳ���   ������ȡ���ݸ���
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
        cmd.Data[k++]= Addr[Name][0];   //�͵�ַ
        cmd.Data[k++]=0x00;
        if(Addr[Name][1]==1)	//��ѯ����ռ�ü���Byte
            cmd.Data[k++] = (u8)parm; //д�����
        else
        {
            cmd.Data[k++] = (u8)(parm&0xff);     			//д�����
            cmd.Data[k++] = (u8)((parm>>8)&0xff);     			//д�����
        }
        cmd.Len = k;
        Send_Data_TTL(cmd);
        break;
    }
    case READ_DATA:
    {
        cmd.Insruction = READ_DATA;
        cmd.Data[k++]= Addr[Name][0];      //�͵�ַ
        cmd.Data[k++]=0x00;
        cmd.Data[k++]=(u8)parm;    				//��ʱ parm �����ȡ����   �ͳ���
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
        while( (data = Receive_Data(XL_TTL))==NULL||(*data).ID !=id); //��ȡ���ص���Ϣ
        if((*data).Data[0])		//�ж�Error Byte
            return 0;
        else
            return 1;						//���� ���޴�����Ϣ
    }
    case REG_WRITE:
    {
        cmd.Insruction = REG_WRITE;
        cmd.Data[k++]= Addr[Name][0];   //�͵�ַ
        cmd.Data[k++]=0x00;
        if(Addr[Name][1]==1)	//��ѯ����ռ�ü���Byte
            cmd.Data[k++] = (u8)parm; //д�����
        else
        {
            cmd.Data[k++] = (u8)(parm&0xff);     			//д�����
            cmd.Data[k++] = (u8)((parm>>8)&0xff);     			//д�����
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
  * @brief  д�������
  * @param  id ��Ҫ���ò����Ķ����id��
            NameType �Ĵ�������  ���ͷ�ļ����
						parm  д��Ҫд��Ĳ���   ������ȡ���ݸ���
						len   д�Ĵ����ĸ���  ע������u16�ͱ�εı���������һ�µ�
						... �� ���  �������ͱ���Ϊu16
  * @retval None
  */
/**************************************************************/
void WriteData(u8 id,NameType Name,int LEN,...)
{
    u8 k=0,l=0;
    u16 temp=0,i=0;
    Dynamixel_Structure cmd,*data=NULL;
    va_list ap;				//���� �������
    va_start(ap,LEN);	//ָ�� ��ε�ַ
    cmd.ID = id;
    cmd.Insruction = WRITE_DATA;
    cmd.Data[k++]=Addr[Name][0];		//��ַ
    cmd.Data[k++]=0x00;
    for(l=0; l<LEN; l++)
    {
        if(Addr[Name+l][1]==1)
            cmd.Data [k++] = (u8)((va_arg(ap, int))&0xff);   //ȡ�����
        else
        {
            temp =va_arg(ap, int);
            cmd.Data [k++] = (u8)(temp&0xff);   //ȡ�����
            cmd.Data [k++] = (u8)((temp>>8)&0xff);   //ȡ�����
        }
    }
    va_end(ap);
    cmd.Len = k;     //���ݸ���+������ַbyte
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
//    cmd.Data[k++]=Addr[Name][0];		//��ַ
//    cmd.Data[k++]=0x00;
//		cmd.Data [k++] = (u8)(Postion&0xff);   //ȡ�����
//		cmd.Data [k++] = (u8)((Postion>>8)&0xff);   //ȡ�����
//		cmd.Data [k++] = (u8)(speed&0xff);   //ȡ�����
//		cmd.Data [k++] = (u8)((speed>>8)&0xff);   //ȡ�����
//    cmd.Len = k;     //���ݸ���+������ַbyte
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
  * @brief  ��ȡ�����Ĵ�������
  * @param  id ��Ҫ���ò����Ķ����id��
            NameType �Ĵ�������  ���ͷ�ļ����

  * @retval ���ص����Ĵ���ֵ
  */
/**************************************************************/
u16 ReadData(u8 id,NameType Name)
{
    Dynamixel_Structure *data=NULL;
    u16 status,i;
    DataOPE(id,READ_DATA,Name,Addr[Name][1]);
    while( (((data = Receive_Data(XL_TTL))==NULL)||((*data).ID !=id)||(((*data).Len-4)!=Addr[Name][1]))&&(i<7000))
        i++; //��ȡ���ص���Ϣ
    if(i!=7000)
    {
        if(Addr[Name][1] == 1)		//�жϼĴ�������
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
  * @brief  ��ȡ����Ĵ�������
  * @param  id ��Ҫ���ò����Ķ����id��
            NameType �Ĵ�������  ���ͷ�ļ����
						len   ��ȡ�Ĵ������� ���������ݸ���  ����Ҫע�� �����Ĵ�����ַ�����������ģ�
						*extractdata   �����ݵ�ַ
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
        temp=temp+Addr[Name+l][1];		//��k���� �ɼĴ���������������� ���ݸ���
    }
    DataOPE(id,READ_DATA,Name,temp);
    while(( ((data = Receive_Data(XL_TTL))==NULL||(*data).ID !=id))&&i<7000) //��ȡ���ص���Ϣ //��ȡ���ص���Ϣ
        i++;
    temp=0;  //����k ����
    if(((*data).ID==id)&&(data !=NULL))
    {
        for(l=0; l<len; l++)
        {
            if(Addr[Name+l][1] == 1)		//�жϼĴ�������
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
    if(i < 200)			//��ʱ�ж�
        return (*data).Data[0];
    else
        return -1;
}
