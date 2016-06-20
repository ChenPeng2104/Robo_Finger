
#ifndef    _ADC_H_
#define    _ADC_H_

#include "stm32f10x.h"
#define ADC_SampleSize 6
extern volatile unsigned short ADC_ConvertedValue[ADC_SampleSize];

void ADC_Configuration(void);
#endif
