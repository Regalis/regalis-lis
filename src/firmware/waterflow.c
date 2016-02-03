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
#include "waterflow.h"
#include "pwm.h"

static volatile uint16_t waterflow_frequency;
static volatile uint16_t waterflow_frequency_tmp;
static volatile uint32_t waterflow_ml;
static volatile uint32_t waterflow_target_ml;

void waterflow_init() {
	FREQ_GPIO->MODER |= (0x2 << (FREQ_PIN << 1));
	FREQ_GPIO->OSPEEDR |= (0x3 << (FREQ_PIN << 1));
	FREQ_GPIO->AFR[0] |= (FREQ_AF << ((FREQ_PIN) << 2));
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN | RCC_APB1ENR_TIM3EN;

	// Only counter overflow/underflow generates an update interrupt
	// or DMA request if enabled
	FREQ_TIM->CR1 |= TIM_CR1_URS;

	// External Clock Mode 1,
	// TRGI = Filtered Timer Input 2 (TI2FP2)
	FREQ_TIM->SMCR |= TIM_SMCR_SMS | TIM_SMCR_TS_2 | TIM_SMCR_TS_1;

	// CC2 channel is configured as input, IC2 is mapped on TI2
	FREQ_TIM->CCMR1 |= TIM_CCMR1_CC2S_0;

	// Auto reload regster = MAX
	FREQ_TIM->ARR = 0xFFFF;

	// CC1 is configured as output by default,
	// enable CC1 interrupt
	FREQ_TIM->DIER |= TIM_DIER_CC1IE;

	TIMEBASE_TIM->CR1 |= TIM_CR1_URS;
	TIMEBASE_TIM->PSC = 48000 - 1;
	TIMEBASE_TIM->ARR = 1000 - 1;
	TIMEBASE_TIM->DIER |= TIM_DIER_UIE | TIM_DIER_CC1IE;

	NVIC_SetPriority(FREQ_TIM_IRQ, 0);
	NVIC_EnableIRQ(FREQ_TIM_IRQ);
	NVIC_SetPriority(TIMEBASE_TIM_IRQ, 0);
	NVIC_EnableIRQ(TIMEBASE_TIM_IRQ);
}

void waterflow_stop() {
	FREQ_TIM->CR1 &= ~(TIM_CR1_CEN);
	TIMEBASE_TIM->CR1 &= ~(TIM_CR1_CEN);
}

static inline void __waterflow_start_timers() {
	FREQ_TIM->EGR |= TIM_EGR_UG;
	FREQ_TIM->CR1 |= TIM_CR1_CEN;
	TIMEBASE_TIM->CCR1 = 250 - 1;
	TIMEBASE_TIM->EGR |= TIM_EGR_UG;
	TIMEBASE_TIM->CR1 |= TIM_CR1_CEN;
}

uint16_t waterflow_read_frequency() {
	return waterflow_frequency;
}

uint32_t waterflow_read_ml() {
	return waterflow_ml;
}

void waterflow_set_target_ml(uint32_t ml) {
	waterflow_target_ml = ml;
	FREQ_TIM->CCR1 = (uint32_t)((float)ml / 2.222);
}

void waterflow_start() {
	waterflow_ml = 0;
	waterflow_frequency = 0;
	waterflow_frequency_tmp = 0;
	__waterflow_start_timers();
}

void TIM3_CC_IRQHandler() {
	if (FREQ_TIM->SR & TIM_SR_CC1IF) {	
		__pwm_off();
		FREQ_TIM->SR &= ~(TIM_SR_CC1IF);
	}
}

void TIM14_IRQHandler() {
	static uint16_t raw_frequency;
	if (TIMEBASE_TIM->SR & TIM_SR_CC1IF) { // 250ms
		raw_frequency = __waterflow_read_raw();
		TIMEBASE_TIM->SR &= ~(TIM_SR_CC1IF);

		if (TIMEBASE_TIM->CCR1 == 999)
			TIMEBASE_TIM->CCR1 = 250 - 1;
		else
			TIMEBASE_TIM->CCR1 += 250;

		waterflow_ml = (uint32_t)((float)raw_frequency * 2.222);

		GPIOA->ODR ^= GPIO_ODR_5;
	}
	if (TIMEBASE_TIM->SR & TIM_SR_UIF) { // 1s
		//TIMEBASE_TIM->CCR1 = 250 - 1;
		TIMEBASE_TIM->SR &= ~(TIM_SR_UIF);
		waterflow_frequency = raw_frequency - waterflow_frequency_tmp;
		waterflow_frequency_tmp = raw_frequency;
	}
}
