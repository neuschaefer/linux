#
#
#  GPL LICENSE SUMMARY
#
#  Copyright(c) 2014-2016 Intel Corporation. All rights reserved.
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
#
#

EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/generic
EXTRA_CFLAGS += -I$(srctree)/include/linux
EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/puma7
EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/puma7/io_ctrl
EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/puma7/iosfsb_drv
EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/puma7/hw_mbox

obj-y := \
	puma7_core.o \
	puma7_board_setup.o \
	puma7_reboot_hook.o \
	puma7_cppi.o \
	puma7_bootcfg_ctrl.o \
	puma7_clk_cntl.o \
	puma7_pp.o \
	puma7_gpio_ctrl.o \
	puma7_board_led.o \
	puma7_io_ctrl.o \
	puma7_pmu.o \
	modphy_drv/ \
	hw_mbox_drv/

obj-$(CONFIG_ARM_AVALANCHE_MMC)			+= puma7_mmc.o
obj-$(CONFIG_ARM_AVALANCHE_SPI) 		+= puma7_spi.o
obj-$(CONFIG_AVALANCHE_PUMA7_SPI_MASTER)+=spi_master/
obj-$(CONFIG_IOSFSB_DRV)				+=iosfsb_drv/
