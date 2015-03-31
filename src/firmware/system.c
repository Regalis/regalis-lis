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

void SystemInit() {
	/* Enable Power Control clock */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	/* Wait until HSI ready */
	while ((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	/* Disable main PLL */
	RCC->CR &= ~(RCC_CR_PLLON);

	/* Wait until PLL ready (disabled) */
	while ((RCC->CR & RCC_CR_PLLRDY) != 0);

	/*
	 * Configure Main PLL
	 * HSI/2 as clock input
	 * PLLMUL = 12
	 * SYSCLK = 48MHz
	 * PCLK = 48MHz
	 */
	RCC->CFGR |= RCC_CFGR_PLLMUL12;

	/* PLL On */
	RCC->CR |= RCC_CR_PLLON;

	/* Wait until PLL is locked */
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);

	/* 
	 * FLASH configuration block
	 * enable prefetch
	 * set latency
	 */
	FLASH->ACR |= FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
	
	/* Set clock source to PLL */
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	/* Check clock source */
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);
}
