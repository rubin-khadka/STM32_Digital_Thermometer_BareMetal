/**
  * @file    spi.c
  * @brief   SPI module - Register level implementation
  * @author  Rubin Khadka
  */
	
#include "spi.h"

void SPI1_init(void)
{
	// Enable peripheral clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_SPI1EN;
	
	// Configure pins for connection with shift registers
	// PA7 MOSI
	GPIOA->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
	GPIOA->CRL |= GPIO_CRL_MODE7;
	GPIOA->CRL |= GPIO_CRL_CNF7_1;
	
	// PA6 MISO
	GPIOA->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
	GPIOA->CRL |= GPIO_CRL_MODE6;
	GPIOA->CRL |= GPIO_CRL_CNF6_1;
	
	// PA5 SCK
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
	GPIOA->CRL |= GPIO_CRL_MODE5;
	GPIOA->CRL |= GPIO_CRL_CNF5_1;
	
	// PA4 GPIO output
	GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);
	GPIOA->CRL |= GPIO_CRL_MODE4;
	GPIOA->BSRR = GPIO_BSRR_BS4;
	
	// SPI1 Configurations
	SPI1->CR1 = 0;
	SPI1->CR1 |= SPI_CR1_SSM;
	SPI1->CR1 |= SPI_CR1_SSI;
	SPI1->CR1 |= SPI_CR1_MSTR;
	
	// 8MHz/2 = 4MHz, already set bu line 37
	
	// CPOL and CPHA for 74HC595
	SPI1->CR1 &= ~SPI_CR1_CPOL;	// Clock idle low (0)
	SPI1->CR1 &= ~SPI_CR1_CPHA;	// Data capture on first edge
	
	// MSB first
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
	
	// Enable SPI
	SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_Transmit16(uint16_t data) 
{
	// Wait until TX buffer is empty
	while (!(SPI1->SR & SPI_SR_TXE));
	
	// Write high byte first
	SPI1->DR = (data >> 8) & 0xFF;
	
	// Wait for first byte to be sent
	while (!(SPI1->SR & SPI_SR_TXE));
	
	// Write low byte
	SPI1->DR = data & 0xFF;
	
	// Wait for transmission to complete
	while (SPI1->SR & SPI_SR_BSY);
}
