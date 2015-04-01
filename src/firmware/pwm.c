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

#include "pwm.h"
#include "utils.h"

#define PWM_RAW_MAX 1000

void pwm_init() {
	// TIM16_CH1 <-> PA6
	
	// GPIO configuration
	
	// GPIOA clock enable
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// PWM_PIN in Alternate function mode
	PWM_GPIO->MODER |= (0x2 << (PWM_PIN << 1));
	
	// PWM_PIN in High speed
	PWM_GPIO->OSPEEDR |= (0x3 << (PWM_PIN << 1));

	// PWM_PIN Alternate function #1 (TIM16_CH1)
	PWM_GPIO->AFR[0] |= (0x5 << (PWM_PIN << 2));

	// Timer configuration
	
	// Timer clock enable
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
	
	PWM_TIM->BDTR |= TIM_BDTR_MOE;

	// PWM Mode 1, Output compare 1 preload enable
	PWM_TIM->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 |
						TIM_CCMR1_OC1PE;
	
	// Prescaler 48Mhz/96 = 500KHz
	PWM_TIM->PSC = (96 - 1);

	// Auto-reload register = 500Mhz/1000 = 500Hz
	PWM_TIM->ARR = (1000 - 1);

	// Set duty cycle = 0%
	__pwm_write_raw(0);

	// Generate update event to flush preloaded registers
	PWM_TIM->EGR |= TIM_EGR_UG;	

	// Start timer
	__pwm_timer_start();
}

void pwm_write(uint8_t percentage) {
	__pwm_write_raw((uint16_t)map(percentage, 0, 100, 0, PWM_RAW_MAX));
}
