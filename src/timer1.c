/**
  * @file    timer1.c
  * @brief   Timer1 module with 100탎 base
  * @author  Rubin Khadka
  */

#include "stm32f10x.h"
#include "timer1.h"

// Global busy flag
volatile uint8_t timer1_busy = 0;

void TIMER1_Init(void)
{
    // Enable timer 1 clock
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    
    // Configure for 100 microsecond base (8 MHz timer clock)
    // 8MHz / 800 = 10kHz = 100탎 per tick
    TIM1->PSC = 799;  // 800-1
    TIM1->CNT = 0;
    
    // Enable update interrupt
    TIM1->DIER |= TIM_DIER_UIE;
    
    // Clear any pending interrupt flag
    TIM1->SR &= ~TIM_SR_UIF;
    
    // Configure NVIC
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    NVIC_SetPriority(TIM1_UP_IRQn, 2);  // Lower priority than ADC
}

void TIMER1_Delay_ms(uint32_t ms_delay)
{
    // Stop timer if running
    TIM1->CR1 &= ~TIM_CR1_CEN; 
    
    // Clear any pending interrupt
    TIM1->SR &= ~TIM_SR_UIF;
    
    // Set ARR for desired delay (100탎 base)
    // 1ms = 10 ticks at 100탎
    // Maximum ms_delay = 6553 (fits in 16 bits)
    if(ms_delay > 6553)
    {
        ms_delay = 6553;  // Cap at max hardware can handle
    }
    
    TIM1->ARR = (uint16_t)((ms_delay * 10) - 1);  // 500ms = 5000 ticks
    TIM1->CNT = 0;
    
    // Set busy flag and start timer
    timer1_busy = 1;
    TIM1->CR1 |= TIM_CR1_CEN;
}

// For when you need microsecond precision (74HC595 timing)
void TIMER1_Delay_us(uint32_t us_delay)
{
    // For short delays, use polling (no interrupt overhead)
    // But with 100탎 base, we need to convert
    // Round up to nearest 100탎
    uint32_t ticks = (us_delay + 99) / 100;  // Convert 탎 to 100탎 ticks
    
    if(ticks == 0) ticks = 1;
    
    TIM1->CR1 &= ~TIM_CR1_CEN;
    TIM1->CNT = 0;
    TIM1->CR1 |= TIM_CR1_CEN;
    
    while(TIM1->CNT < ticks);
    
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

uint8_t TIMER1_IsBusy(void)
{
    return timer1_busy;
}

void TIM1_UP_IRQHandler(void)
{
    if(TIM1->SR & TIM_SR_UIF)
    {
        TIM1->SR &= ~TIM_SR_UIF;   // Clear interrupt flag
        TIM1->CR1 &= ~TIM_CR1_CEN; // Stop timer
        timer1_busy = 0;            // Clear busy flag
    }
}
