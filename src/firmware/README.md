# Libre Irrigation System

Firmware for Libre Irrigation System

* CPU: STM32F030R8T6
	* Core: ARM Cortex M0
	* Flash: 64K
	* SRAM: 8K
	* Clock: 48MHz
* Waterflow sensor YF-S401
	* 1 - 30 l/min
* Pump:
	* 1x Car Warsher Pump (12V @ 3A ~3.5 l/min) - 1 output
* Motor driver:
	* Custom N-Mosfet based

## CPU pinout

* PA0 - NC
* PA1 - NC
* PA2 - USART1 serial tx
* PA3 - USART1 serial rx
* PA4 - NC
* PA5 - status LED
* PA6 - TIM16 PWM output
* PA7 - TIM3 CH2 external clock input (waterflow sensor)
* PA9 - I2C SCL
* PA10 - I2C SDA
* PA13 - SWDIO
* PA14 - SWCLK
* PB1 - NC

License
=======

Copyright (C) Patryk Jaworski \<regalis@regalis.com.pl\>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses.
