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
    ADC_InitTypeDef          ADC_InitStructure;//����ADC��ʼ���ṹ�����
    DMA_InitTypeDef          DMA_InitStructure;//����AMA��ʼ���ṹ��
    NVIC_InitTypeDef         NVIC_InitStructure;
    //ʱ��
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);  //  48M/4 =12
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA , ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    //GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //DMA����
    DMA_DeInit(DMA1_Channel1);//ͨ������
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(ADC1->DR)); //�����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr  = (u32)&ADC_ConvertedValue; //�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//DMA���䷽��
    DMA_InitStructure.DMA_BufferSize = ADC_SampleSize;//����DMA��������С6
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//��ǰ����Ĵ�����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//��ǰ�洢����ַ�ı�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�����������ݿ��16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //����洢�����16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMAͨ������ģʽΪ���λ���ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMAͨ�����ȼ���
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//��ʼ��DMAͨ��1

    DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��DMAͨ��1
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
//    //DMA�ж�����
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQHandler;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);


    //ADCͨ������

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ɨ��ر�
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//ADCת������������ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//���������ת��
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ת�������Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 6;//ת��ͨ������
    ADC_Init(ADC1, &ADC_InitStructure); //��ʼ��ADC1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_13Cycles5);//ADC1ѡ���ŵ�4.�ȼ�1������ʱ��

    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_13Cycles5);//ADC1ѡ���ŵ�4.�ȼ�1������ʱ��

    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_13Cycles5);//ADC1ѡ���ŵ�4.�ȼ�1������ʱ��

    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_13Cycles5);//ADC1ѡ���ŵ�4.�ȼ�1������ʱ��

    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_13Cycles5);//ADC1ѡ���ŵ�4.�ȼ�1������ʱ��

    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 6, ADC_SampleTime_13Cycles5);//ADC1ѡ���ŵ�4.�ȼ�1������ʱ��

    ADC_DMACmd(ADC1, ENABLE);//ʹ��ADC1ģ��DMA
    ADC_Cmd(ADC1, ENABLE);//ʹ��ADC1

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
