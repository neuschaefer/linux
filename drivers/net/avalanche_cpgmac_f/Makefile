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


# File: drivers/net/avalanche_cpgmac_f/Makefile
#
# Makefile for the Linux network (CPGMAC_F) device drivers.
#
# DDA
#EXTRA_CFLAGS        += -DCPMAC_DDA_DEBUG
#EXTRA_CFLAGS        += -DPAL_INCLUDE_OSMEM -DPAL_INCLUDE_OSPROTECT 
EXTRA_CFLAGS        += -DCPMAC_CACHE_WRITEBACK_MODE 
#EXTRA_CFLAGS        += -DCPMAC_DDA_CACHE_INVALIDATE_FIX

EXTRA_CFLAGS        += -DCPGMAC_PORTSEG_TASKLET_MODE
EXTRA_CFLAGS        += -DTX_TASKLET_MODE
EXTRA_CFLAGS        += -DRX_TASKLET_MODE
# DDC
#EXTRA_CFLAGS        += -DCPMAC_DDC_DEBUG
#EXTRA_CFLAGS        += -DCPMAC_POLL_MODE
EXTRA_CFLAGS        += -DCPGMAC_USE_ACC_LIST 

#EXTRA_CFLAGS       += -DCPMAC_DDC_USE_ASSEMBLY
#EXTRA_CFLAGS       += -DCPMAC_DDC_ARM_OPTIMIZED
EXTRA_CFLAGS	    += -DCPPI4_CACHE_WRITEBACK_MODE
#EXTRA_CFLAGS	    += -DCPPI4_PAL_ARM_OPTIMIZED
EXTRA_CFLAGS		+= -DCPMAC_INCLUDE_ASSERT


#EXTRA_CFLAGS += -DCPMAC_CAPTURE_PERF_STATS


# Configuration
# Note: For testing we are use ENV variable 
#EXTRA_CFLAGS        += -DCPMAC_USE_CONFIG_SERVICE
#EXTRA_CFLAGS        += -DCPMAC_USE_ENV

#should point to the location of the psp_trace.h
EXTRA_CFLAGS += -I$(srctree)/include/linux 


#INCLUDE PATH FOR MIB SPECIFIC HEADER FILES

ifeq ("$(PATHCHLEVEL)", "4")
EXTRA_CFLAGS        += -I$(srctree)/include/asm-arm/arch-avalanche/generic
O_TARGET := avalanche_cpgmac_f.o
list-multi := avalanche_cpgmac_f.o
else
EXTRA_CFLAGS        += -I$(srctree)/include/asm-arm/arch-avalanche/generic
endif

obj-$(CONFIG_ARM_AVALANCHE_CPGMAC_F) += avalanche_cpgmac_f.o

avalanche_cpgmac_f-objs :=  ddc_cpgmac_f.o ddc_cpgmac_f_TxRx.o cpswhalcommon_miimdio.o

ifeq ($(CONFIG_ARM_EXTERNAL_SWITCH_SEGREGATION),y)
avalanche_cpgmac_f-objs +=  cpgmac_f_NetLx_switch.o cpgmac_f_NetLxTxRx_switch.o
else
avalanche_cpgmac_f-objs +=  cpgmac_f_NetLx.o cpgmac_f_NetLxTxRx.o 
endif

ifeq ($(CONFIG_ARM_EXTERNAL_SWITCH),y)
avalanche_cpgmac_f-objs += dda_switch.o csl_switch.o ddc_switch.o
endif
ifeq ($(CONFIG_ARM_AVALANCHE_MARVELL_6063),y)
avalanche_cpgmac_f-objs += csl_marvell6063.o
endif
ifeq ($(CONFIG_ARM_AVALANCHE_MARVELL_6060),y)
avalanche_cpgmac_f-objs += csl_marvell6063.o
endif
ifeq ($(CONFIG_ARM_AVALANCHE_ATHEROS_8328N),y)
avalanche_cpgmac_f-objs += csl_atheros8328N.o
endif

ifeq ("$(PATHCHLEVEL)", "4")
include $(srctree)/Rules.make

avalanche_cpgmac_f.o:        $(avalanche_cpgmac_f-objs)
	$(LD) -r -o $@ $(avalanche_cpgmac_f-objs)

clean:
	rm -f core *.o *.a *.s
endif
