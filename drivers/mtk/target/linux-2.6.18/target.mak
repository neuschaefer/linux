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
# $SWAuthor: Yan Wang $
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
# C compiler and options
#
#KERNEL_PWD=$(PROJECT_ROOT)/../chiling/kernel/linux-2.6.18
-include $(PROJECT_ROOT)/linux_porting.mak

ifndef OPTIMIZE_LVL
OPTIMIZE_LVL = -O2
endif

# MHF Linux, liang.zhang - Apr/01/2009
#MONTA_PWD=/opt/montavista
DFBLIB_PWD=$(LINUX_PORTING_CHILING_PATH)/ramdisk/rootfs/lib


LIB2 := 

INC += -I$(PROJECT_ROOT)/middleware/mheg5/OceanBlue/glue/gui_render/png/inc
#endif

INC += -I$(LINUX_PORTING_CHILING_PATH)/directfb/Source/DirectFB/include
INC += -I$(LINUX_PORTING_DTV_LINUX_PRJ_PATH)/dtv_process/include/ioctl_inc

ifndef CROSS_COMPILE
CROSS_COMPILE    = arm11_mtk_le-
endif

CC ?= $(CROSS_COMPILE)gcc
TARGET_INC = -I$(TARGET_ROOT) $(INC)

ifndef TARGET
TARGET = mt5392b
endif

CC_FLAG += $(OPTIMIZE_LVL) -Wall -Wno-long-long -pedantic -fsigned-char -fshort-wchar -I$(DTV_PROCESS_ROOT)
#CC_FLAG += -ansi
CC_FLAG += -Wno-strict-aliasing
DEFINES += -D_GNU_SOURCE

ifneq ($(strip $(NO_SYM_DEBUG)),TRUE)
CC_FLAG += -g
endif

ifeq "$(TOOL_CHAIN)" "4.4.1"
CC_FLAG += -fno-dwarf2-cfi-asm
endif


#
# Loader and options.
#
LD        = $(CROSS_COMPILE)gcc
LD_FLAG   =
LD_OUTPUT = -o

#
# The following functions are used to translate libraries and library path
#
set_lib      = $(addprefix lib,$(addsuffix .a,$(1)))
set_lib_path = $(addprefix -L,$(1))

#
# Add all libraries in "lib_lib" directory
#
#LIB_LIBS := $(strip $(basename $(notdir $(wildcard $(LIB_LIB_DIR)/*.a))))
#ifneq ($(LIB_LIBS),)
#LIB_LIBS := $(subst  lib, -l,$(LIB_LIBS))
#endif


#
# tool to create object file library.  AR_FLAG contains option to
# add/replace members (.o file) of library archive.
#
AR      ?= $(CROSS_COMPILE)ar
AR_FLAG = -ru
AR_EXTRACT_FLAG = x
RANLIB ?= $(CROSS_COMPILE)ranlib


#
# cc flag for the compiler to automatically generate dependency files
# For GNU cc, use -MM.
#
CC_DEP_GEN_FLAG = -MM -I$(DTV_PROCESS_ROOT)


#
# CPU's endianess
#
DEFINES += -D_CPU_LITTLE_ENDIAN_ -DD_TARGET_CUST_H=\"d_5392b_cust.h\" $(CHIP_DEFINES)


#
# Platform specific libraries
#
TARGET_LIBS_PATH = -L$(LIB2) -L$(TARGET_LIB_DIR) -L$(CUSTOM_LIB_DIR) -L$(MW_LIB_DIR) -L$(DTV_LINUX_LIB_DIR) -L$(DFBLIB_PWD)
TARGET_LIBS      = -lc -lpthread


#
# Platform specific executable filename suffic.
#
TARGET_EXE_SUFFIX =


#
# Specify some target specific functions
#
dep_out_fct =


#
# Get Host specfic definitions
#
include $(PROJECT_ROOT)/host.mak
