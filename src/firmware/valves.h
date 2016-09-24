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

//
// Default LIS Solenoid valves module provides PCB with opto-isolators
// suitable to drive 12V/2A valves. Opto-isolators are installed with
// common anode configuration.
//

#ifndef __VALVES_H__
#define __VALVES_H__

#define VALVES_GPIO GPIOB
#define VALVES_CHANNELS_NO 4

// General purpose output mode
#define VALVES_MODER_PATTERN ((uint32_t)0x55555555)

// Push pull
#define VALVES_OTYPER_PATTERN ((uint32_t)0x0000FFFF) 

// Full speed
#define VALVES_OSPEEDR_PATTERN ((uint32_t)0xFFFFFFFF)

// BSRR mask
#define VALVES_DISABLE_ALL ((uint16_t)(0xFFFF >> (16 - VALVES_CHANNELS_NO)))

/**
 * Enable (turn on) specified channel
 *
 * @channel_no channel index (from 0 to VALVES_CHANNELS_NO - 1)
 */
static inline void valves_enable_output(uint8_t channel_no) {
	VALVES_GPIO->BSRRL = (1 << channel_no);
}

/** 
 * Disable (turn off) all channels
 */
static inline void valves_disable_all_outputs() {
	VALVES_GPIO->BSRRH = VALVES_DISABLE_ALL;
}

static inline void valves_init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	valves_disable_all_outputs();
	VALVES_GPIO->MODER |= (VALVES_MODER_PATTERN >> (32 - (VALVES_CHANNELS_NO << 1)));
	VALVES_GPIO->OTYPER &= ~(VALVES_OTYPER_PATTERN >> (16 - VALVES_CHANNELS_NO));
	VALVES_GPIO->OSPEEDR |= (VALVES_OSPEEDR_PATTERN >> (32 - (VALVES_CHANNELS_NO << 1)));
}

#endif
