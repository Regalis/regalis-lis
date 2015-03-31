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

#ifndef __WATERFLOW_H__
#define __WATERFLOW_H__

#define FREQ_TIM TIM1
#define FREQ_GPIO GPIOA
#define FREQ_PIN 9
#define FREQ_AF 2

#define TIMEBASE_TIM TIM3
#define TIMEBASE_TIM_IRQ TIM3_IRQn

void waterflow_init();

static inline uint16_t __waterflow_read_raw() {
	return FREQ_TIM->CNT;
}

static inline uint32_t __waterflow_frequency_to_ml_min(uint32_t freq) {
	return (uint32_t)((freq * 10000UL) / 75UL);
}

uint32_t waterflow_read_ml();
void waterflow_stop();

void waterflow_start();

uint16_t waterflow_read_frequency();

uint32_t waterflow_read_ml_per_sec();
void TIM3_IRQHandler();

#endif
