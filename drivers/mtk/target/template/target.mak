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
ifeq "$(RVCT31)" "y"
ASM = armasm.569
CC = armcc.569
LD = armlink.569
AR = armar.569
export ARM_BIN=/mtktools/ARM/RVDS3.1/RVCT/Programs/3.1/569/linux-pentium/
export ARM_INC=/mtktools/ARM/RVDS3.1/RVCT/Data/3.1/569/include/unix/
export ARM_INCLUDE_PATH=/mtktools/ARM/RVDS3.1/RVCT/Data/3.1/569/include/unix/
export ARM_LIB_PATH=/mtktools/ARM/RVDS3.1/RVCT/Data/3.1/569/lib
else
ASM = armasm
CC = armcc
LD = armlink
AR = armar
export ARM_BIN=/mtktools/ARM/RVCT/Programs/2.1/328/linux-pentium/
export ARM_INC=/mtktools/ARM/RVCT/Data/2.1/328/include/unix/
export ARM_INCLUDE_PATH=/mtktools/ARM/RVCT/Data/2.1/328/include/unix/
export ARM_LIB_PATH=/mtktools/ARM/RVCT/Data/2.1/328/lib
endif


#
# C compiler and options
#

# CC_FLAG += -W --cpu ARM1176JZ-S --gnu --signed_chars
CC_FLAG += -W --cpu ARM926EJ-S --gnu --signed_chars

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
ASM_FLAG := -G


ifeq "$(THUMB)" ""
else
CC_FLAG += --thumb -DCC_MINI_DRIVER --apcs /inter
ASM_FLAG += --thumb --apcs /inter
endif

export ARM_FLAG

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
		 -I$(TARGET_ROOT)/ic_driver/inc \
                 -I$(TARGET_ROOT)/ic_driver/drv_inc \
                 -I$(TARGET_ROOT)/ic_driver/private_inc \
                 -I$(TARGET_ROOT)/d_inc
endif


#
# Linker and options.
#
LD_FLAG   = --entry INT_Reset --scatter $(call path_cnvt,$(TARGET_ROOT)/ic_driver/scatter.sc)
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
DEFINES += -D_CPU_LITTLE_ENDIAN_ -DD_TARGET_CUST_H=\"d_ic_cust.h\" $(CHIP_DEFINES)


#
# Platform dependency, IC dependent XXX
#
DEFINES += -DCC_MT53xx


#
# Libraries (and include path)
#
ifdef ARM_LIB_PATH
TARGET_LIBS_PATH = --libpath $(ARM_LIB_PATH)
endif

LOADER_DEFINED=n

ifeq "$(MTK_LOADER)" "y"
LOADER_DEFINED=y
endif

# IC dependent XXX
export TARGET_IC = mt53xx

ifeq "$(LOADER_DEFINED)" "y"
TARGET_LIBS_PATH += --userlibpath $(TARGET_LIB_DIR)
else
TARGET_LIBS_PATH += --userlibpath $(TARGET_LIB_DIR) --userlibpath $(MW_LIB_DIR) --userlibpath $(CUSTOM_LIB_DIR) --userlibpath $(LIB_LIB_DIR)
endif

ifeq "$(MTK_LOADER)" "y"
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

#
# Get Host specfic definitions
#
include $(PROJECT_ROOT)/host.mak

