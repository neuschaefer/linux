#/*
#*
#* Makefile
#* Description:
#*
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

EXTRA_CFLAGS += -I$(srctree)/include/asm/arch/generic -I$(srctree)/include/linux
EXTRA_CFLAGS += -I$(srctree)/include/asm/arch/puma5  -I$(srctree)/include/asm-arm/arch-avalanche/generic

obj-y := puma5_core.o puma5_board_setup.o pal_sysGpioCtrl.o pal_sysHw.o puma5_cppi.o pal_sysPscResetCtrl.o pal_sysPscPowerCtrl.o pal_sysPsc.o 

ifeq ($(CONFIG_MACH_PUMA5_VOLCANO), y)
obj-y += puma5_emu_clk_cntl.o
else
obj-y += puma5_clk_cntl.o
endif
obj-$(CONFIG_ARM_AVALANCHE_COLORED_LED) += puma5_board_led.o
obj-$(CONFIG_ARM_AVALANCHE_SPI) += puma5_spi.o
obj-$(CONFIG_ARM_AVALANCHE_PPD) += puma5_pp.o

