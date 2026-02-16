#/*
#*
#* Makefile
#* Description:
#* see below
#*
#*
#* Copyright (C) 2008, Texas Instruments, Incorporated
#*
#*  This program is free software; you can distribute it and/or modify it
#*  under the terms of the GNU General Public License (Version 2) as
#*  published by the Free Software Foundation.
#*
#*  This program is distributed in the hope it will be useful, but WITHOUT
#*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#*  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#*  for more details.
#*
#*  You should have received a copy of the GNU General Public License along
#*  with this program; if not, write to the Free Software Foundation, Inc.,
#*  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
#*
#*/


# File: drivers/char/avalanche_led/Makefile
#
# Makefile for the Linux LED device driver.
#
ifeq ("$(PATCHLEVEL)", "4")
EXTRA_CFLAGS := -I$(srctree)/include/asm-arm/arch-avalanche/generic
O_TARGET         := avalanche_led.o
list-multi   := avalanche_led.o
export-objs := led_manager.o 
else
EXTRA_CFLAGS := -I$(srctree)/include/asm-arm/arch-avalanche/generic 
endif


obj-$(CONFIG_ARM_AVALANCHE_COLORED_LED) += avalanche_led.o

avalanche_led-objs :=	led_manager.o led_hal.o

ifeq ("$(PATCHLEVEL)", "4")
include $(srctree)/Rules.make

avalanche_led.o:	$(avalanche_led-objs)
			$(LD) -r -o $@ $(avalanche_led-objs)

clean:
	rm -f core *.o *.a *.s

endif


