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

#ifndef __PWM_H__
#define __PWM_H__

#define PWM_PIN 6
#define PWM_GPIO GPIOA
#define PWM_TIM TIM16

/** 
 * Initialize timer for PWM output (edge-aligned)
 *  -> enable GPIOA clock
 *  -> configure GPIO
 *  -> enable TIM16 clock
 *  -> configure TIM16 (500Hz, PWM output mode)
 *  -> start timer
 *
 *  Warning! Function will not enable PWM output,
 *  use __pwm_on() to connect timer output to GPIO pin
 *
 *  @return void
 */ 
void pwm_init();

/**
 * Set PWM dudty cycle (0% to 100%)
 *
 * @param percentage - 0 to 100
 */
void pwm_write(uint8_t percentage);

/**
 * Write RAW value into Timer's compare register
 *
 * @param raw new Timer's compare register value
 */
static inline void __pwm_write_raw(uint16_t raw) {
	PWM_TIM->CCR1 = raw;
}

/** 
 * Connect Timer output to corresponding GPIO pin
 */
static inline void __pwm_on() {
	PWM_TIM->CCER |= TIM_CCER_CC1E;
}

/**
 * Disconnect Timer output from corresponding GPIO pin
 */
static inline void __pwm_off() {
	PWM_TIM->CCER &= ~(TIM_CCER_CC1E);
}

/**
 * Start Timer
 */
static inline void __pwm_timer_start() {
	PWM_TIM->CR1 |= TIM_CR1_CEN;
}

/**
 * Stop Timer
 */
static inline void __pwm_timer_stop() {
	PWM_TIM->CR1 &= ~(TIM_CR1_CEN);
}

#endif
