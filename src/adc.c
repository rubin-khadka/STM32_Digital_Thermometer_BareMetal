/**
  * @file    adc.c
  * @brief   ADC module initialization 
  * @author  Rubin Khadka
  */
	
#include "stm32f10x.h"  // Device header
#include "adc.h"

/* Global Variables */
volatile uint32_t adc_raw_value = 0;
volatile uint8_t adc_conversion_complete = 0;

void ADC_Init(void)
{
	// Enable ADC1 peripheral clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_ADC1EN;
	
	// Small delay for clocks to stabilize
	for(volatile int i = 0; i < 100; i++);
	
	// GPIO Configuration for ADC1
	GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
	
	// Power on ADC
	ADC1->CR2 |= ADC_CR2_ADON;
	
	// Configure Sampling time for Channel 0
	ADC1->SMPR2 &= ~(0x07UL << 0);	// Clear existing sampling cycle
	ADC1->SMPR2 |= (TEMP_ADC_SAMPLING_TIME << 0);	// Set sampling cycle 28.5 
	
	// Configure conversion sequence 
	ADC1->SQR1 &= ~(0x0FUL << 20);	// 1 conversion in sequence
	ADC1->SQR3 = TEMP_ADC_CHANNEL;	// SQ1[4:0] = Channel 0
	
	// ADC calibration 
	ADC1->CR2 |= ADC_CR2_RSTCAL;
	while (ADC1->CR2 & ADC_CR2_RSTCAL);
	
	ADC1->CR2 |= ADC_CR2_CAL;
	while (ADC1->CR2 & ADC_CR2_CAL);
	
	ADC1->CR2 |= ADC_CR2_EXTSEL;     // Software trigger
	ADC1->CR2 |= ADC_CR2_EXTTRIG;    // Enable external trigger
	
	// Final configuration
	ADC1->CR2 &= ~ADC_CR2_CONT;  // Single conversion mode
	ADC1->CR2 &= ~ADC_CR2_ALIGN; // Right alignment
	
	// Enable ADC interrupt in NVIC
	ADC1->CR1 |= ADC_CR1_EOCIE;
	NVIC_EnableIRQ(ADC1_2_IRQn);   
	
	// Clear any previous values 
	adc_raw_value = 0;
	adc_conversion_complete = 0;
	
	// Brief stabilization delay
	for (volatile uint32_t i = 0; i < 1000; i++);  // Small delay for stabilisation
}

void ADC_StartConversion(void)
{
	adc_conversion_complete = 0;
	// ADC1->CR2 |= (0x01 << 22);
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint8_t ADC_IsReady(void)
{
	return adc_conversion_complete;
}

float ADC_ReadCelsius(void)
{
	float voltage = ((float)adc_raw_value * VREF) / ADC_RESOLUTION;
	float temperature = (voltage - 1.65f) * LM35_SCALE_FACTOR;
	temperature = (temperature - 0.1f) * 0.9756f; // Reduce gain by 2.5% to compensate for the offset
  return temperature;
}

void ADC1_2_IRQHandler(void)
{
	if (ADC1->SR & ADC_SR_EOC)
	{
		adc_raw_value = ADC1->DR;
		adc_conversion_complete = 1;
		ADC1->SR &= (uint32_t)~ADC_SR_EOC;
	}
}
