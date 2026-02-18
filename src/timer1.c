/**
  * @file    timer1.c
  * @brief   Timer1 module with 100µs base
  * @author  Rubin Khadka
  */

#include "stm32f10x.h"
#include "timer1.h"

#define TIMER_PSC_VALUE 799  
#define US_PER_TICK 100  

void TIMER1_Init(void)
{
	// Enable timer 1 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	// Configure for 100 microsecond base (100µs per tick)
	TIM1->PSC = TIMER_PSC_VALUE;
	
	// Disable interrupt (we're using polling)
	TIM1->DIER &= ~TIM_DIER_UIE;
	
	// Stop timer initially
	TIM1->CR1 &= ~TIM_CR1_CEN;
}

void TIMER1_Delay_ms(uint32_t ms_delay)
{
	// Convert ms to ticks: 1ms = 10 ticks
	uint32_t ticks = ms_delay * 10;
	
	// Stop timer if running
	TIM1->CR1 &= ~TIM_CR1_CEN;
	
	// Set auto-reload value
	TIM1->ARR = (uint16_t)(ticks - 1);
	
	// Reset counter
	TIM1->CNT = 0;
	
	// Start timer
	TIM1->CR1 |= TIM_CR1_CEN;
	
	// Wait until counter reaches ARR
	while(!(TIM1->SR & TIM_SR_UIF));
	
	// Clear update flag
	TIM1->SR &= ~TIM_SR_UIF;
	
	// Stop timer
	TIM1->CR1 &= ~TIM_CR1_CEN;
}
