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
# $SWAuthor: 
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
# The following functions are used to translate libraries and library path
# 
set_lib      = $(addprefix -l, $(1))
set_lib_path = $(addprefix -L,$(1))
#
# Add all libraries in "lib_lib" directory
#
LIB_LIBS := $(notdir $(wildcard $(LIB_LIB_DIR)/*.a))

TARGET_LIBS_PATH = -L $(TARGET_LIB_DIR)

TARGET_INC = -I. 



ifndef TARGET_DRV_INC
TARGET_DRV_INC = -I. \
		         -I$(TARGET_ROOT)/5398_driver/inc \
                 -I$(TARGET_ROOT)/5398_driver/drv_inc \
                 -I$(TARGET_ROOT)/5398_driver/private_inc \
                 -I$(TARGET_ROOT)/d_inc
endif

DEFINES += -D__GCC__      -D__CC_GCC__    

CC := $(CROSS_COMPILE)gcc
LD :=  $(CROSS_COMPILE)ld
LD_OUTPUT = -o

ASM := $(CROSS_COMPILE)gcc
ASM_FLAG := -c

ifeq "$(ODB)" "true"
CC = odbc comp $(CROSS_COMPILE)gcc
ASM := odbc comp $(CROSS_COMPILE)gcc
endif

AR      = $(CROSS_COMPILE)ar
AR_FLAG = -ru

CC_FLAG = $(OPTIMIZE_LVL) -Wall -Wno-long-long -std=gnu99 -fsigned-char -fshort-wchar -Wno-strict-aliasing 
#CC_FLAG = + -mtune=arm1176jz-s 

#ifneq "$(shell echo $(WARNING_TO_ERROR) | tr '[:lower:]' '[:upper:]')" "FALSE"
#CC_FLAG += -Werror
#endif

ERR_NO_LIST = 1,47,111,117,167,174,177,186,188,193,223,513,C3017,68,550,940,546,120,767,C3488
ifeq "$(shell echo $(WARNING_TO_ERROR) | tr '[:lower:]' '[:upper:]')" "FALSE"
#CC_FLAG += --diag_warning $(ERR_NO_LIST)
else
#CC_FLAG += --diag_error $(ERR_NO_LIST)
endif


#keep these option to make final executable small
#ifneq ($(strip $(NO_SYM_DEBUG)),TRUE)
CC_FLAG += -g -mapcs-frame
#endif

ifeq "$(TOOL_CHAIN)" "4.4.1"
CC_FLAG += -fno-dwarf2-cfi-asm
endif

#ifeq "$(ENABLE_VFP)" "true"
#CC_FLAG += -mfloat-abi=softfp -mfpu=vfpv3-d16
#endif

#ifeq "$(ENABLE_CA9)" "true"
#CC_FLAG += -march=armv7-a -mtune=cortex-a9
#endif

CC_FLAG += -march=armv7-a -mtune=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv3-d16
ASM_FLAG += -march=armv7-a -mtune=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv3-d16

#Do not predefine __linux__
CC_FLAG += -U__linux__

ifeq "$(THUMB)" "y"
ifdef ARM_FLAG
CC_FLAG += $(ARM_FLAG)
ASM_FLAG += $(ARM_FLAG) 
else
CC_FLAG += -mthumb -DCC_MINI_DRIVER -mapcs  -mthumb-interwork
#CC_FLAG += -mlong-calls
#CC_FLAG += -mcallee-super-interworking  -mcaller-super-interworking
#ASM_FLAG += -mthumb -mapcs  
endif
endif


#PLATFORM_LIBS := -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc  -L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lc /mtktools/mtk-toolchain/gnuarm-4.4.2_002/cross-tools/arm11/armv6z-mediatek-linux-gnueabi/lib/libgcc_s.so

PROLOADER_LIBS :=  -L$(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`)/libgcc.a  $(CROSS_TOOLCHAIN_SYSROOT)/lib/libc.a
LOADER_LIBS := -L$(CROSS_TOOLCHAIN_SYSROOT_LIB) $(PROLOADER_LIBS)  $(CROSS_TOOLCHAIN_SYSROOT_LIB)/libgcc_s.so $(CROSS_TOOLCHAIN_SYSROOT_LIB)/libc.so.6 $(CROSS_TOOLCHAIN_SYSROOT_LIB)/ld-linux.so.3 --no-wchar-size-warning --no-enum-size-warning 

