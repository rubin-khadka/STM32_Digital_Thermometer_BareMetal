/**
  * @file    adc.h
  * @brief   ADC module initialization header file
  * @author  Rubin Khadka
  */
	
#ifndef ADC_H
#define ADC_H

#include "stdint.h"

/* Defines */
#define TEMP_ADC_CHANNEL  0x00  // ADC Channel 0 (PA0)
#define TEMP_ADC_SAMPLING_TIME 	0x03  // 28.5 sampling cycle

// Temperature calculation
#define VREF  3.3f  // 3.3V reference in mV
#define ADC_RESOLUTION  4096.0f  // 12-bit ADC
#define LM35_SCALE_FACTOR  100.0f  // LM35: 10mV/°C = 0.01V/°C × 100

/* Global Variables */
extern volatile uint32_t adc_raw_value;
extern volatile uint8_t adc_conversion_complete;

/* Function Prototypes */
void ADC_Init(void);
void ADC_StartConversion(void);
uint8_t ADC_IsReady(void);
float ADC_ReadCelsius(void);
void ADC1_2_IRQHandler(void);

#endif /* ADC_H */
