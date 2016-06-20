/**
  ******************************************************************************
  * @file    	ADC.C
  * @author  Luzian
  * @version V1.0.0          modified by Keil for MDK-ARM V5.0
  * @date    19-1-2015
  * @brief   all the Basical communication for ADC
	******************************************************************************
**/
#include "ADC.h"
/**************************ADC**************************/

volatile unsigned short ADC_ConvertedValue[ADC_SampleSize];
/*ADC*/
void ADC_Configuration(void)
{
    GPIO_InitTypeDef         GPIO_InitStructure;
    ADC_InitTypeDef          ADC_InitStructure;//定义ADC初始化结构体变量
    DMA_InitTypeDef          DMA_InitStructure;//定义AMA初始化结构体
    NVIC_InitTypeDef         NVIC_InitStructure;
    //时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);  //  48M/4 =12
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA , ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    //GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //DMA配置
    DMA_DeInit(DMA1_Channel1);//通道配置
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(ADC1->DR)); //外设地址
    DMA_InitStructure.DMA_MemoryBaseAddr  = (u32)&ADC_ConvertedValue; //内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//DMA传输方向
    DMA_InitStructure.DMA_BufferSize = ADC_SampleSize;//定义DMA缓冲区大小6
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//当前外设寄存器地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//当前存储器地址改变
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//定义外设数据宽度16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //定义存储器宽度16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA通道操作模式为环形缓冲模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA通道优先级高
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//初始化DMA通道1

    DMA_Cmd(DMA1_Channel1, ENABLE); //使能DMA通道1
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
//    //DMA中断配置
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQHandler;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);


    //ADC通用配置

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //扫描关闭
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//ADC转换工作在连续模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//由软件控制转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//转换数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 6;//转换通道个数
    ADC_Init(ADC1, &ADC_InitStructure); //初始化ADC1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_13Cycles5);//ADC1选择信道4.等级1，采样时间

    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_13Cycles5);//ADC1选择信道4.等级1，采样时间

    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_13Cycles5);//ADC1选择信道4.等级1，采样时间

    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_13Cycles5);//ADC1选择信道4.等级1，采样时间

    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_13Cycles5);//ADC1选择信道4.等级1，采样时间

    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 6, ADC_SampleTime_13Cycles5);//ADC1选择信道4.等级1，采样时间

    ADC_DMACmd(ADC1, ENABLE);//使能ADC1模块DMA
    ADC_Cmd(ADC1, ENABLE);//使能ADC1

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);      // Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

#define referenceVoltage 3.3f

int ADC_ConvertedTime = 0;
float realtimeVoltage=0;
void DMA1_Stream1_IRQHandler(void)
{
//    int i ;
//    int ADC_total = 0;
//    if (DMA_GetITStatus(DMA1_Stream1,DMA_IT_TC) == SET)
//    {
//        DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_TC);

//        for (i = 0; i < ADC_SampleSize; i++)
//        {
//            ADC_total += ADC_ConvertedValue[i];
//        }

//        realtimeVoltage = (float)ADC_total / ADC_SampleSize;

//        ADC_ConvertedTime++;
//    }
}
