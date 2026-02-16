################################################################################
#
# Makefile
#  Description:
#  Makefile for the Packet processor driver.
# 
# Copyright(c) 2007-2012 Texas Instruments, Inc. All rights reserved.
# 
# This program is free software; you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation; either version 2 of the License, or (at your option) 
# any later version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
# more details.
# 
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 59 
# Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# 
# The full GNU General Public License is included in this distribution in the
# file called LICENSE.
# 
################################################################################

EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/generic
EXTRA_CFLAGS += -I$(srctree)/include/linux
EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/puma6
EXTRA_CFLAGS += -I$(srctree)/include/asm-arm/arch-avalanche/puma5

obj-$(CONFIG_ARM_AVALANCHE_PPD) += avalanche_ppd.o

avalanche_ppd-objs :=  ti_ppd.o ti_ppd_pmon.o

obj-$(CONFIG_ARM_AVALANCHE_PPD_TEST) += ppd_test.o

