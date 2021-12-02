###############################################################################
# Copyright Statement:                                                        #
#                                                                             #
#   This software/firmware and related documentation ("MediaTek Software")    #
# are protected under international and related jurisdictions'copyright laws  #
# as unpublished works. The information contained herein is confidential and  #
# proprietary to MediaTek Inc. Without the prior written permission of        #
# MediaTek Inc., any reproduction, modification, use or disclosure of         #
# MediaTek Software, and information contained herein, in whole or in part,   #
# shall be strictly prohibited.                                               #
# MediaTek Inc. Copyright (C) 2010. All rights reserved.                      #
#                                                                             #
#   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND      #
# AGREES TO THE FOLLOWING:                                                    #
#                                                                             #
#   1)Any and all intellectual property rights (including without             #
# limitation, patent, copyright, and trade secrets) in and to this            #
# Software/firmware and related documentation ("MediaTek Software") shall     #
# remain the exclusive property of MediaTek Inc. Any and all intellectual     #
# property rights (including without limitation, patent, copyright, and       #
# trade secrets) in and to any modifications and derivatives to MediaTek      #
# Software, whoever made, shall also remain the exclusive property of         #
# MediaTek Inc.  Nothing herein shall be construed as any transfer of any     #
# title to any intellectual property right in MediaTek Software to Receiver.  #
#                                                                             #
#   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its  #
# representatives is provided to Receiver on an "AS IS" basis only.           #
# MediaTek Inc. expressly disclaims all warranties, expressed or implied,     #
# including but not limited to any implied warranties of merchantability,     #
# non-infringement and fitness for a particular purpose and any warranties    #
# arising out of course of performance, course of dealing or usage of trade.  #
# MediaTek Inc. does not provide any warranty whatsoever with respect to the  #
# software of any third party which may be used by, incorporated in, or       #
# supplied with the MediaTek Software, and Receiver agrees to look only to    #
# such third parties for any warranty claim relating thereto.  Receiver       #
# expressly acknowledges that it is Receiver's sole responsibility to obtain  #
# from any third party all proper licenses contained in or delivered with     #
# MediaTek Software.  MediaTek is not responsible for any MediaTek Software   #
# releases made to Receiver's specifications or to conform to a particular    #
# standard or open forum.                                                     #
#                                                                             #
#   3)Receiver further acknowledge that Receiver may, either presently        #
# and/or in the future, instruct MediaTek Inc. to assist it in the            #
# development and the implementation, in accordance with Receiver's designs,  #
# of certain softwares relating to Receiver's product(s) (the "Services").    #
# Except as may be otherwise agreed to in writing, no warranties of any       #
# kind, whether express or implied, are given by MediaTek Inc. with respect   #
# to the Services provided, and the Services are provided on an "AS IS"       #
# basis. Receiver further acknowledges that the Services may contain errors   #
# that testing is important and it is solely responsible for fully testing    #
# the Services and/or derivatives thereof before they are used, sublicensed   #
# or distributed. Should there be any third party action brought against      #
# MediaTek Inc. arising out of or relating to the Services, Receiver agree    #
# to fully indemnify and hold MediaTek Inc. harmless.  If the parties         #
# mutually agree to enter into or continue a business relationship or other   #
# arrangement, the terms and conditions set forth herein shall remain         #
# effective and, unless explicitly stated otherwise, shall prevail in the     #
# event of a conflict in the terms in any agreements entered into between     #
# the parties.                                                                #
#                                                                             #
#   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and     #
# cumulative liability with respect to MediaTek Software released hereunder   #
# will be, at MediaTek Inc.'s sole discretion, to replace or revise the       #
# MediaTek Software at issue.                                                 #
#                                                                             #
#   5)The transaction contemplated hereunder shall be construed in            #
# accordance with the laws of Singapore, excluding its conflict of laws       #
# principles.  Any disputes, controversies or claims arising thereof and      #
# related thereto shall be settled via arbitration in Singapore, under the    #
# then current rules of the International Chamber of Commerce (ICC).  The     #
# arbitration shall be conducted in English. The awards of the arbitration    #
# shall be final and binding upon both parties and shall be entered and       #
# enforceable in any court of competent jurisdiction.                         #
###############################################################################
###########################################################################
# $RCSfile: target.mak,v $
# $Revision: #1 $
# $Date: 2012/04/27 $
# $Author: dtvbm11 $
# $CCRevision: /main/DTV_X/DTV_X_HQ_int/29 $
# $SWAuthor: Felisa Hsiao $
# $MD5HEX: 75d957b926cbd32e9aa3e2356dc40cc2 $
#
# Description:
#         Compiler tool parameters to be included in the make process.
#
#         Note: this file provide the default compile, loader, utility
#         commands and their options for the default developer environment
#         (Linux/GNUWIN).
#
#         For a specific TRAGET build, the TARGET directory needs to
#         provide a target specific target.mak which specifies the
#         compiling tools for that target.
#
#############################################################################


#
# Decide RTOS
#
ifndef RTOS
RTOS = nucleus
endif
export PROJECT_X=y

#
# Assembler & C compiler tool chains command
#
ifeq "$(RVCT31)" "n"
RVCT21 = y
else
RVCT31 = y
endif

#export CA9 = y
ifeq "$(CA9_RVCT40)" "y"

ifndef RVCT40_ROOT
RVCT40_ROOT = /mtktools/ARM/RVDS40/RVCT
endif


ifdef ARMASM_4_0_BUILD_400
export ASM = $(ARMASM_4_0_BUILD_400)
else
export ASM = armasm.4.0.400
endif

ifdef ARMCC_4_0_BUILD_400
export CC = $(ARMCC_4_0_BUILD_400)
else
export CC = armcc.4.0.400
endif

ifdef ARMLINK_4_0_BUILD_400
export LD = $(ARMLINK_4_0_BUILD_400)
else
export LD = armlink.4.0.400
endif

ifdef ARMAR_4_0_BUILD_400
export AR = $(ARMAR_4_0_BUILD_400)
else
export AR = armar.4.0.400
endif

ifdef FROMELF_4_0_BUILD_400
export FROMELF = $(FROMELF_4_0_BUILD_400)
else
export FROMELF = fromelf.4.0.400
endif

ifdef ARM_4_0_BUILD_400_BIN_PATH
export ARM_BIN=$(ARM_4_0_BUILD_400_BIN_PATH)
else
export ARM_BIN=$(RVCT40_ROOT)/Programs/4.0/400/linux-pentium/
endif

ifdef ARM_4_0_BUILD_400_INCLUDE_PATH
export ARM_INC=$(ARM_4_0_BUILD_400_INCLUDE_PATH)
export ARM_INCLUDE_PATH=$(ARM_4_0_BUILD_400_INCLUDE_PATH)
else
export ARM_INC=$(RVCT40_ROOT)/Data/4.0/400/include/unix/
export ARM_INCLUDE_PATH=$(RVCT40_ROOT)/Data/4.0/400/include/unix/
endif

ifdef ARM_4_0_BUILD_400_LIB_PATH
export ARM_LIB_PATH=$(ARM_4_0_BUILD_400_LIB_PATH)
else
export ARM_LIB_PATH=$(RVCT40_ROOT)/Data/4.0/400/lib
endif




ifeq "$(ODB)" "true"
CC := odbc comp $(CC)

endif
else

ifeq "$(RVCT21)" "y"

ifndef RVCT21_ROOT
RVCT21_ROOT = /mtktools/ARM/RVCT
endif
ASM = armasm
CC = armcc
LD = armlink
AR = armar
FROMELF = fromelf
export ARM_BIN=$(RVCT21_ROOT)/Programs/2.1/328/linux-pentium/
export ARM_INC=$(RVCT21_ROOT)/Data/2.1/328/include/unix/
export ARM_INCLUDE_PATH=$(RVCT21_ROOT)/Data/2.1/328/include/unix/
export ARM_LIB_PATH=$(RVCT21_ROOT)/Data/2.1/328/lib

else	#============== RVCT 3.1 ================

ifndef RVCT31_ROOT
RVCT31_ROOT = /mtktools/ARM/RVDS3.1/RVCT
endif
ifdef ARMASM_3_1_BUILD_761
ASM = $(ARMASM_3_1_BUILD_761)
else
ASM = armasm.761
endif

ifdef ARMCC_3_1_BUILD_761
CC = $(ARMCC_3_1_BUILD_761)
else
CC = armcc.761
endif
ifeq "$(ODB)" "true"
CC := odbc comp $(CC)
endif

ifdef ARMCPP_3_1_BUILD_761
CPP = $(ARMCPP_3_1_BUILD_761)
else
CPP = armcpp.761
endif
ifeq "$(ODB)" "true"
CPP := odbc comp $(CPP)
endif

ifdef ARMLINK_3_1_BUILD_761
LD = $(ARMLINK_3_1_BUILD_761)
else
LD = armlink.761
endif

ifdef ARMAR_3_1_BUILD_761
AR = $(ARMAR_3_1_BUILD_761)
else
AR = armar.761
endif

ifdef FROMELF_3_1_BUILD_761
FROMELF = $(FROMELF_3_1_BUILD_761)
else
FROMELF = fromelf.761
endif

export ARM_BIN=$(RVCT31_ROOT)/Programs/3.1/761/linux-pentium/

ifdef ARM_3_1_BUILD_761_INCLUDE_PATH
export ARM_INC=$(ARM_3_1_BUILD_761_INCLUDE_PATH)
export ARM_INCLUDE_PATH=$(ARM_3_1_BUILD_761_INCLUDE_PATH)
else
export ARM_INC=$(RVCT31_ROOT)/Data/3.1/761/include/unix/
export ARM_INCLUDE_PATH=$(RVCT31_ROOT)/Data/3.1/761/include/unix/
endif

ifdef ARM_3_1_BUILD_761_LIB_PATH
export ARM_LIB_PATH=$(ARM_3_1_BUILD_761_LIB_PATH)
else
export ARM_LIB_PATH=$(RVCT31_ROOT)/Data/3.1/761/lib
endif

ifndef _AEABI_PORTABILITY_LEVEL
export _AEABI_PORTABILITY_LEVEL=0
endif
DEFINES += -D_AEABI_PORTABILITY_LEVEL=$(_AEABI_PORTABILITY_LEVEL)

endif

endif

#
# C compiler and options
#

CC_FLAG := $(OPT_FLAG) $(CC_PIC_FLAG) -g -DINLINE=__inline --gnu --signed_chars --no_unaligned_access

ifeq "$(RVCT21)" "y"
CC_FLAG += --cpu ARM1176JZF-S
else
ifeq "$(CA9_RVCT40)" "y"
ifeq "$(CA9_VFP)" "y"
CC_FLAG += --cpu Cortex-A9.no_neon
else
CC_FLAG += --cpu Cortex-A9.no_neon.no_vfp
endif
else
ifeq "$(CA9)" "y"
CC_FLAG += --cpu ARM1176JZF-S
else
CC_FLAG += --cpu ARM1176JZ-S
endif
endif
endif

#CC_FLAG += --diag_warning 193,2795

ERR_NO_LIST = 1,47,111,117,167,174,177,186,188,193,223,513,C3017,68,550,940,546,120,767,C3488,175,1565,92,1295

ifndef WARNING_TO_ERROR
export WARNING_TO_ERROR=FALSE
endif

ifeq "$(shell echo $(WARNING_TO_ERROR) | tr '[:lower:]' '[:upper:]')" "FALSE"
CC_FLAG += --diag_warning $(ERR_NO_LIST)
else
CC_FLAG += --diag_error $(ERR_NO_LIST)
endif

ifndef OPTIMIZE_LVL
OPTIMIZE_LVL = -O2
endif

ifneq ($(strip $(NO_SYM_DEBUG)),TRUE)
CC_FLAG += -g $(OPTIMIZE_LVL)
else
CC_FLAG += $(OPTIMIZE_LVL)
endif

#
# Assembler and options
#
ASM_FLAG := -g --diag_suppress 1745,1786,1788

ifeq "$(CA9_RVCT40)" "y"
ifeq "$(CA9_VFP)" "y"
ASM_FLAG += --cpu=Cortex-A9.no_neon
else
ASM_FLAG += --cpu=Cortex-A9.no_neon.no_vfp
endif
else
ifeq "$(CA9)" "y"
ASM_FLAG += --cpu=ARM1176JZF-S
endif
endif

ifeq "$(THUMB)" "y"
CC_FLAG += --thumb -DCC_MINI_DRIVER --apcs /inter
ASM_FLAG += --thumb --apcs /inter
endif

export ARM_FLAG

CC_FLAG_NOWARN_ERROR = $(subst diag_error,diag_warning, $(CC_FLAG))
export CC_FLAG_NOWARN_ERROR

#
# If ARM_INCLUDE_PATH is not defined, use current directory "."
# In case the ARM include path is composed of multiple words print error
# message and stop the build.
#
ifdef ARM_INCLUDE_PATH
ifeq ($(filter-out $(word 1,$(ARM_INCLUDE_PATH)),$(ARM_INCLUDE_PATH)),)
TARGET_INC = -I$(ARM_INCLUDE_PATH)
else
$(error ERROR! Space is not allowed in ARM_INCLUDE_PATH)
endif

else
TARGET_INC = -I.
endif


ifndef TARGET_DRV_INC
TARGET_DRV_INC = -I. \
		 -I$(TARGET_ROOT)/5860_driver/inc \
                 -I$(TARGET_ROOT)/5860_driver/drv_inc \
                 -I$(TARGET_ROOT)/5860_driver/private_inc \
                 -I$(TARGET_ROOT)/d_inc
endif


#
# Linker and options.
#
LD_FLAG   = --entry INT_Reset --scatter $(call path_cnvt,$(TARGET_ROOT)/5860_driver/scatter.sc)
LD_OUTPUT = --Output
LD_MAP    = --xref --map --info unused,sizes,totals --list

ifeq ($(strip $(NO_SYM_DEBUG)),TRUE)
LD_FLAG += --no_debug
endif


#
# The following functions are used to translate libraries and library path
#
set_lib      = $(addprefix lib,$(addsuffix .a,$(1)))
set_lib_path = $(addprefix --userlibpath ,$(1))

#
# Add all libraries in "lib_lib" directory
#
LIB_LIBS := $(notdir $(wildcard $(LIB_LIB_DIR)/*.a))


#
# tool to create object file library.  AR_FLAG contains option to
# add/replace members (.o file) of library archive.
#
AR_FLAG = -r


#
# cc flag for the compiler to automatically generate dependency files
# For GNU cc, use -MM.
#
ifeq "$(BUILD_LINUX_LOADER)" "true"
CC_DEP_GEN_FLAG := -MMD -MF
else
CC_DEP_GEN_FLAG := --md --depend=
endif


#
# CPU's endianess
#
DEFINES += -D_CPU_LITTLE_ENDIAN_ -DD_TARGET_CUST_H=\"d_mtk_cust.h\" $(CHIP_DEFINES)
DEFINES += -DTARGET_SPEC_DRV_CUST_HDR_FILE=\"d_mtk_cust.h\" -DCC_DYNAMIC_POWER_ONOFF


#
# Platform dependency
#
DEFINES += -DCC_MT5396
DEFINES += -DCC_MT5860
#DEFINES += -DCC_ENABLE_L2C
DEFINES += -DCC_53XX_SWDMX_V2

#
# Libraries (and include path)
#
ifdef ARM_LIB_PATH
TARGET_LIBS_PATH = --libpath $(ARM_LIB_PATH)
endif

LOADER_DEFINED=n

ifeq "$(5860_LOADER)" "y"
LOADER_DEFINED=y
endif

export TARGET_IC = mt5860

ifeq "$(LOADER_DEFINED)" "y"
TARGET_LIBS_PATH += --userlibpath $(TARGET_LIB_DIR)
else
TARGET_LIBS_PATH += --userlibpath $(TARGET_LIB_DIR) --userlibpath $(MW_LIB_DIR) --userlibpath $(CUSTOM_LIB_DIR) --userlibpath $(LIB_LIB_DIR)
endif

ifeq "$(5860_LOADER)" "y"
TARGET_LIBS       :=
else
TARGET_LIBS       = $(call set_lib,$(RTOS))
endif

#
# Platform specific executable filename suffic.
#
TARGET_EXE_SUFFIX =


#
# Specify some target specific functions
#
dep_out_fct = -o $(subst .d,.o,$(1))

ifeq "$(BUILD_LINUX_LOADER)" "true"
include $(TARGET_ROOT)/linux_mtkloader.mak 
endif #end BUILD_LINUX_LOADER

#
# Get Host specfic definitions
#
include $(PROJECT_ROOT)/host.mak

