/**
  * @file    timer1.h
  * @brief   Timer1 module header with 100µs base
  * @author  Rubin Khadka
  */

#ifndef TIMER1_H
#define TIMER1_H

#include <stdint.h>

// Function prototypes
void TIMER1_Init(void);
void TIMER1_Delay_ms(uint32_t ms_delay);
void TIMER1_Delay_us(uint32_t us_delay); 
uint8_t TIMER1_IsBusy(void);
void TIM1_UP_IRQHandler(void);

// Global variable declaration
extern volatile uint8_t timer1_busy;

#endif
