#
# 
# Copyright (C) Patryk Jaworski <regalis@regalis.com.pl>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
#
LIBRARY_PATH=../STM32Cube_FW_F0_V1.0.0/

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OC=arm-none-eabi-objcopy
AS=arm-none-eabi-as
AR=arm-none-eabi-ar

MCU_CORE=cortex-m0
MCU=STM32F030x6

CCFLAGS=-Wall -mcpu=$(MCU_CORE) -mlittle-endian -mthumb \
		-ffunction-sections -fdata-sections

CCFLAGS+=$(INCLUDE) $(DEFS) -g -Os -Werror
LDFLAGS=-T$(LD_SCRIPT) -Wl,--gc-sections,--print-gc-sections


INCLUDE_PATH=Drivers/CMSIS/Device/ST/STM32F0xx/Include \
		Drivers/CMSIS/Include

LD_SCRIPT=flash.ld
DEFS=-D$(MCU)
STARTUP_CODE=$(LIBRARY_PATH)Projects/STM32F030R8-Nucleo/Examples/GPIO/GPIO_IOToggle/TrueSTUDIO/startup_stm32f030x8.s
INCLUDE=$(patsubst %, -I$(LIBRARY_PATH)%, $(INCLUDE_PATH))
INCLUDE+=-I../lib
SOURCES_PATH := .
SOURCES := $(foreach DIR,$(SOURCES_PATH),$(wildcard $(DIR)/*.c $(DIR)/*.cpp))
OBJS := $(patsubst %.c,%.o,$(filter %.c, $(SOURCES)))
OBJS+=$(patsubst %.cpp, %.o, $(filter %.cpp, $(SOURCES)))
OBJS+=startup_stm32f030x8.o

#
# Dependency based section
#

all: main.hex

main.elf: $(OBJS) 
	$(CC) $(CCFLAGS) $(LDFLAGS) $^ -o $@

main.hex: main.elf
	$(OC) -Oihex $< $@

startup_stm32f030x8.o: $(STARTUP_CODE)
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

openocd:
	openocd -f $(OPENOCD_SCRIPT)

cc_cmd:
	@echo $(CC) $(CCFLAGS) -c INPUT -o OUTPUT
	
clean:
	@rm -v $(OBJS) || /bin/true
	@rm -v main.elf main.hex || /bin/true

.PHONY: clean openocd cc_cmd
