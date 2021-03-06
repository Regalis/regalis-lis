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
#include "delay.h"

static inline void systick_stop() {
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

static inline void systick_start() {
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

static inline void systick_wait() {
	while (( SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
}


void delay_init() {
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->LOAD = 47; // tick every 1,008us
}

void _delay_us(uint32_t us) {
	systick_start();
	while (us != 0) {
		systick_wait();
		--us;
	}
	systick_stop();
}

inline void _delay_ms(uint32_t ms) {
	_delay_us(ms * 1000);
}
