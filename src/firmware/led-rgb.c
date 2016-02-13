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

#include "led-rgb.h"

void led_rgb_init()
{
	// GPIOs in alternate function mode
	LED_RGB_GPIO->MODER |=  (0x2 << (LED_RGB_R << 1)) |
							(0x2 << (LED_RGB_G << 1)) |
							(0x2 << (LED_RGB_B << 1));
	
	// GPIOs in high speed mode
	LED_RGB_GPIO->OSPEEDR |= (0x3 << (LED_RGB_R << 1)) |
							 (0x3 << (LED_RGB_G << 1)) |
							 (0x3 << (LED_RGB_B << 1));

	// GPIOs alternate function #2 - TIM1_CHx	
	LED_RGB_GPIO->AFR[1] |= (0x2 << ((LED_RGB_R - 8) << 2)) |
							(0x2 << ((LED_RGB_G - 8) << 2)) |
							(0x2 << ((LED_RGB_B - 8) << 2));
	
	// Timer clock enable
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	LED_RGB_TIM->BDTR |= TIM_BDTR_MOE;

	// PWM Mode 1, output compare {1, 2} preload enable
	LED_RGB_TIM->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_1 |
						  TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2PE |
						  TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

	// PWM Mode 1, output compare 3 preload enable
	LED_RGB_TIM->CCMR2 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC3M_1 |
						  TIM_CCMR2_OC3M_2;

	// ~500Hz PWM output
	LED_RGB_TIM->PSC = ((188 * 2) - 1);

	// 0 - off; 255 - fully on
	LED_RGB_TIM->ARR = (255 -1);

	// Default duty cycle = 0%
	led_rgb_set_color(0, 0, 0);

	// Start timer (PWM outputs remains disabled)
	__led_rgb_timer_on();

}
