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

#ifndef __LED_RGB_H__
#define __LED_RGB_H__

#define LED_RGB_TIM TIM1
#define LED_RGB_GPIO GPIOA
#define LED_RGB_R 8
#define LED_RGB_G 9
#define LED_RGB_B 10
#define LED_RGB_MSK ((1 << LED_RGB_R) | \
					(1 << LED_RGB_G) | (1 << LED_RGB_B))

void led_rgb_init();

static inline void __led_rgb_on()
{
	LED_RGB_TIM->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E |
						 TIM_CCER_CC3E;
}

static inline void __led_rgb_off()
{
	LED_RGB_TIM->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E |
						   TIM_CCER_CC3E);
}

static inline void __led_rgb_timer_on()
{
	LED_RGB_TIM->CR1 |= TIM_CR1_CEN;
}

static inline void __led_rgb_timer_off()
{
	LED_RGB_TIM->CR1 &= ~(TIM_CR1_CEN);
}

static inline void led_rgb_set_r(uint8_t r)
{
	LED_RGB_TIM->CCR1 = r;
}

static inline void led_rgb_set_g(uint8_t g)
{
	LED_RGB_TIM->CCR2 = g;
}

static inline void led_rgb_set_b(uint8_t b)
{
	LED_RGB_TIM->CCR3 = b;
}

static inline void led_rgb_set_color(uint8_t r, uint8_t g, uint8_t b)
{
	led_rgb_set_r(r);
	led_rgb_set_g(g);
	led_rgb_set_b(b);
}

#endif
