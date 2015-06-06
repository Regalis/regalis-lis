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

#ifndef __LIS_PROTOCOL_H__
#define __LIS_PROTOCOL_H__

/******************************************************************
 * LIS Protocol format:                            
 *                                                  
 *    8bit     8bit    8bit    8bit   L * 8bit    32bit
 * |   PM   |   F   |   C   |   L   |    D     |   X    |
 *                                                 
 *  P - preamble
 *  M - message type (lis_msg_t)
 *  F - flags (lis_flag_t)
 *  C - command (lis_command_t)
 *  L - data length
 *  D - data
 *  X - checksum
 *****************************************************************/

#define LIS_PREAMBLE 0xF0
#define LIS_PREAMBLE_MASK 0xF0
#define LIS_MSG_MASK 0x0F

#define LIS_CHECKSUM_INITIAL_VALUE 0x000000FF

enum lis_msg {
	LIS_MSG_REQUEST = 1,
	LIS_MSG_ANSWER,
	LIS_MSG_PING,
	LIS_MSG_PONG,
	LIS_MSG_BROADCAST,

	LIS_MSG_LAST = LIS_MSG_BROADCAST
};

enum lis_flag {
	LIS_FLAG_NULL = 0,
	LIS_FLAG_REQUEST_ACK = (1 << 0),
	LIS_FLAG_ACK = (1 << 1),
	LIS_FLAG_ACCEPTED = (1 << 2),
	LIS_FLAG_REJECTED = (1 << 3),

	LIS_FLAG_LAST=LIS_FLAG_REJECTED
};

enum lis_command {
	LIS_COMMAND_NULL = 0,
	LIS_COMMAND_READ_REGISTER,
	LIS_COMMAND_WRITE_REGISTER,
	LIS_COMMAND_FIRMWARE_VER,
	LIS_COMMAND_START,
	LIS_COMMAND_STOP,

	LIS_COMMAND_LAST = LIS_COMMAND_STOP
};

enum lis_register {
	LIS_REG_TARGET_ML = 0,
	LIS_REG_TARGET_SPEED,
	LIS_REG_SPEED_HZ,
	LIS_REG_SPEED_ML_MIN,

	LIS_REG_LAST = LIS_REG_SPEED_ML_MIN
};

typedef enum lis_msg lis_msg_t;
typedef enum lis_command lis_command_t;
typedef enum lis_flag lis_flag_t;
typedef enum lis_register lis_register_t;

/**
 * Validate LIS packet
 *
 * @param buffer an non-empty buffer containing LIS packet
 * @param buffer_len lenght of buffer (must be greater than 3)
 * @return 0 if packet is valid, value greater than zero if packet is invalid
 */
static inline uint8_t lis_validate_packet(uint8_t *buffer,
										  uint16_t buffer_len)
{
	uint32_t checksum = LIS_CHECKSUM_INITIAL_VALUE;
	if ((buffer[0] & LIS_PREAMBLE_MASK) != LIS_PREAMBLE)
		return 1;
	if ((buffer[0] & LIS_MSG_MASK) == 0 ||
		(buffer[0] & LIS_MSG_MASK) > LIS_MSG_LAST)
		return 2;
	if (buffer[2] > LIS_COMMAND_LAST)
		return 3;
	
	uint16_t i;
	for (i = 0; i < 5 + buffer[3]; ++i)
		checksum ^= buffer[i];
	
	if (checksum != *((uint32_t*)(buffer + 4 + buffer[3])))
		return 0xF0;
	
	return 0;
}

static inline uint8_t lis_is_preamble(uint8_t byte)
{
	return ((byte & LIS_PREAMBLE_MASK) == LIS_PREAMBLE);
}

static inline uint8_t lis_get_message(uint8_t *buffer)
{
	return buffer[0] & LIS_MSG_MASK;
}

static inline uint8_t lis_get_flags(uint8_t *buffer)
{
	return buffer[1];
}

static inline uint8_t lis_get_command(uint8_t *buffer)
{
	return buffer[2];
}

static inline uint8_t lis_get_data_length(uint8_t *buffer)
{
	return buffer[3];
}

static inline uint8_t *lis_get_data(uint8_t *buffer)
{
	return buffer + lis_get_data_length(buffer) + 4;
}


#endif
