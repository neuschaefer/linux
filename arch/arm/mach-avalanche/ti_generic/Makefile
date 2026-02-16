#
#
#  GPL LICENSE SUMMARY
#
#  Copyright(c) 2008-2015 Intel Corporation. All rights reserved.
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

EXTRA_CFLAGS += -I$(srctree)/include/asm/arch

ifeq ($(CONFIG_MACH_PUMA7), y)
EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/puma7/hw_mbox
endif

obj-y := avalanche_generic_setup.o avalanche_misc.o avalanche_intc.o ti_gpl_wrapper.o \
		 pal_cppi41.o avalanche_intd.o ramtest.o puma_intd.o

obj-$(CONFIG_MACH_PUMA5) += avalanche_io_power_cntl.o
obj-$(CONFIG_ARM_AVALANCHE_VLYNQ) += avalanche_vlynq_intc.o pal_vlynq.o \
                                      pal_vlynqDev.o pal_vlynqIoctl.o \
                                      avalanche_vlynq_config.o

ifneq (,$(filter y,$(CONFIG_MACH_PUMA6) $(CONFIG_MACH_PUMA7)))
obj-y                                   += cru_ctrl.o 
obj-y                                   += arm_atom_mbx/
obj-y                                   += sram_drv/
endif

ifeq ($(CONFIG_MACH_PUMA7), y)
obj-$(CONFIG_CPPI4_HW_MBOX_SUPPORT)     += cppi41_hw_mbox_if.o
endif

obj-y += pal_sys/
obj-$(CONFIG_HW_MUTEXES) += hw_mutex/
obj-y += time_capture/
