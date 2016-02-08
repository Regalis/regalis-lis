/*
* 
* Copyright (C) Patryk Jaworski <regalis@regalis.com.pl>
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* 
*/

#include <stm32f0xx.h>

#include "serial.h"

#define SERIAL_TX 2
#define SERIAL_RX 3
#define SERIAL USART2
#define SERIAL_AF 1

inline void serial_init() {
	/* GPIO configuration */
	/* PA2 in Alternate function mode */
	GPIOA->MODER |= (2 << (SERIAL_TX << 1));
	/* high speed */
	GPIOA->OSPEEDR |= (3 << (SERIAL_TX << 1));
	/* set SERIAL AF (AF7) */
	GPIOA->AFR[0] |= (SERIAL_AF << ((SERIAL_TX) << 2));
	/* PA3 in Alternate function mode */
	GPIOA->MODER |= (2 << (SERIAL_RX << 1));
	/* high speed */
	GPIOA->OSPEEDR |= (3 << (SERIAL_RX << 1));
	/* set SERIAL AF (AF7) */
	GPIOA->AFR[0] |= (SERIAL_AF << ((SERIAL_RX) << 2));


	/* SERIAL configuration */
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	/* enable transmitter */
	SERIAL->BRR |= 417;
	SERIAL->CR3 |= USART_CR3_OVRDIS;
	SERIAL->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

inline void serial_putc(char c) {
	while ((SERIAL->ISR & USART_ISR_TC) == 0);
	SERIAL->TDR = (uint8_t)c;
}

uint8_t serial_getc() {
	while (!(SERIAL->ISR & USART_ISR_RXNE));
	return (uint8_t)(SERIAL->RDR & (uint8_t)0x00ff);
}

void serial_puts(const char *buffer) {
	while (*buffer != '\0') {
		serial_putc(*(buffer++));
	}
}
