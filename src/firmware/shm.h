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

#ifndef __SHM_H__
#define __SHM_H__

#include <stm32f0xx.h>
#include "lis_protocol.h"

extern volatile uint32_t shm[];

static inline uint8_t __shm_write_register(lis_register_t offset, uint32_t value) {
	if ((uint8_t)offset > LIS_REG_LAST)
		return 0;
	__disable_irq();
	shm[offset] = value;
	__enable_irq();
	return 0xff;
}

static inline uint32_t __shm_read_register(lis_register_t offset) {
	if ((uint8_t)offset > LIS_REG_LAST)
		return 0;
	__disable_irq();
	uint32_t tmp = shm[offset];
	__enable_irq();
	return tmp;
}

#endif
