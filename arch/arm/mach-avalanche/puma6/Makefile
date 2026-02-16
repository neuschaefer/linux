#
#  GPL LICENSE SUMMARY
#
#  Copyright(c) 2014 Intel Corporation.
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of version 2 of the GNU General Public License as
#  published by the Free Software Foundation.
#
#  This program is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
#  The full GNU General Public License is included in this distribution
#  in the file called LICENSE.GPL.
#
#  Contact Information:
#    Intel Corporation
#    2200 Mission College Blvd.
#    Santa Clara, CA  97052
#/*
#*
#* Makefile
#* Description:
#*
#*/

EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/generic
EXTRA_CFLAGS += -I$(srctree)/include/linux
EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/puma6

obj-y := puma6_core.o \
         puma6_board_setup.o \
         puma6_cppi.o \
         puma6_pp.o \
         puma6_clk_cntl.o \
         puma6_bootcfg_ctrl.o

obj-$(CONFIG_AVALANCHE_GENERIC_GPIO)    += puma6_gpio_ctrl.o
obj-$(CONFIG_ARM_AVALANCHE_COLORED_LED) += puma6_board_led.o
obj-$(CONFIG_ARM_AVALANCHE_SPI) 	+= puma6_spi.o
obj-$(CONFIG_ARM_AVALANCHE_MMC) 	+= puma6_mmc.o
obj-$(CONFIG_IOSFSB_DRV)                += iosfsb_drv/
obj-$(CONFIG_P_UNIT_DRV) 	        += p_unit_drv/
obj-y                           	+= aep_drv/
