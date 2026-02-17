/**
  * @file    spi.h
  * @brief   SPI module for STM32F103
  * @author  Rubin Khadka
  */

#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h" // Device header
#include "stdint.h"

#define LATCH(x) ((x) ? (GPIOA->BSRR = GPIO_BSRR_BS4) : (GPIOA->BSRR = GPIO_BSRR_BR4))

/* Function Prototypes */
void SPI1_init(void);
void SPI1_Transmit16(uint16_t data);

#endif /* SPI_H */
