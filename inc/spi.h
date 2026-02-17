/**
  * @file    spi.h
  * @brief   SPI module for STM32F103
  * @author  Rubin Khadka
  */

#ifndef SPI_H
#define SPI_H

/* Include Definitions */
#include "stm32f10x.h" // Device header

/* Macros */
#define LATCH(x) (x==1 ? (HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, GPIO_PIN_SET)):\
												 (HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, GPIO_PIN_RESET)))

/* Function Prototypes */
void SPI1_init(void);
void SPI1_Transmit16(uint16_t data);

#endif /* SPI_H */
