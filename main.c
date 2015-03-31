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
#include "pwm.h"
#include "serial.h"
#include "string_utils.h"
#include "waterflow.h"
#include "utils.h"

uint16_t menu_read_uint(const char *label, const char *unit, int16_t step) {
	uint16_t tmp = 0;
	char buffer[6];
	uint8_t c = 0;
	do {
		if (c == 'j') {
			tmp -= step;
		} else if (c == 'k') {
			tmp += step;
		}
		serial_puts("\r                                                 \r");
		itoa(tmp, buffer);
		serial_puts(label);
		serial_puts(buffer);
		serial_puts(unit);

	} while ((c = serial_getc()) != 13);
	serial_puts("\r\n");
	return tmp;
}

int main() {
	delay_init();
	pwm_init();
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER5_0;
	serial_init();
	serial_puts("\r\n[I] System reboot...\n\r");
	waterflow_init();
	serial_puts("[I] Waterflow sensor initialized...\n\r");


	char animation[] = "|/-\\";
	uint8_t animation_i = 0;
	uint16_t ml_max;
	uint16_t speed;
	char buffer[128];
	uint16_t current_ml;
	uint16_t current_freq;
	uint8_t percentage;
	while (1) {
		current_ml = 0;
		current_freq = 0;
		percentage = 0;
		ml_max = menu_read_uint("[U] Set target: ", "ml", 100);
		speed = menu_read_uint("[U] Set pump speed: ", "%", 1);
		itoa(speed, buffer);
		serial_puts("[I] Setting pump speed to ");
		serial_puts(buffer);
		serial_puts("%...\n\r");
		pwm_write(speed);
		serial_puts("[I] Enabling waterflow sensor...\n\r");
		waterflow_start();
		serial_puts("[I] Waterflow sensor started...\n\r");
		serial_puts("[I] Starting pump...\n\r");	
		__pwm_on();
		serial_puts("[I] Entering sensor loop...\n\r\n\r");
		// TODO: check it in IRQ!
		while (current_ml <= ml_max) {
			current_ml = waterflow_read_ml();
			current_freq = waterflow_read_frequency();
			percentage = map(current_ml, 0, ml_max, 0, 100);
			serial_puts("\r                                                                                      \r");
			serial_putc('[');
			serial_putc(animation[animation_i]);
			animation_i = (animation_i + 1) % 4;
			serial_puts("] Sensor: ");
			itoa(current_freq, buffer);
			serial_puts(buffer);
			serial_puts("Hz | Water level: ");
			itoa(current_ml, buffer);
			serial_puts(buffer);
			serial_puts("ml (");
			itoa(percentage, buffer);
			serial_puts(buffer);
			serial_puts("%)\n\r");
			_delay_ms(25);
		}
		__pwm_off();
		serial_puts("\n\r\n[I] Stopping pump!\n\r");
		serial_puts("[I] Waiting for stop...\n\r");
		// TODO: check it in IRQ!
		while (waterflow_read_frequency() != 0);
		waterflow_stop();
		serial_puts("[I] Disabling waterflow sensor...\n\r\n\r");
		itoa(ml_max, buffer);
		serial_puts("[S] Requested: ");
		serial_puts(buffer);
		serial_puts("ml | Calculated: ");
		itoa(current_ml, buffer);
		serial_puts(buffer);
		serial_puts("ml | Error: ");
		itoa((int32_t)((int32_t)current_ml - (int32_t)ml_max), buffer);
		serial_puts(buffer);
		serial_puts("ml\n\r\n\r");
	}

}