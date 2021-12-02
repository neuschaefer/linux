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
# $RCSfile: Makefile,v $
# $Revision: #19 $
# $Date: 2009/07/01 $
# $Author: jipson.liu $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

THIS_COMPONENT = mt5368

########  !!!!!Put new driver module here !!!!!
MODULE =
MODULE_DIR =  fbm vdp6896 dbs srm b2r6896
MODULE_DIR += gfx6896 imgrz6896 pdwnc6896 psr
# MODULE_DIR += gfx3d
MODULE_DIR += osd6896
MODULE_DIR += pmx
MODULE_DIR += ir6896 rtc
MODULE_DIR += uart sif6896
MODULE_DIR += usb
MODULE_DIR += cec
MODULE_DIR += blkmgr
ifdef ENABLE_SMARTCARD
MODULE_DIR += smc
DEFINES += -DCC_SUPPORT_SMC
endif
MODULE_DIR += nand

ifeq "$(BOOT_TYPE)" "ROM2EMMC"
MODULE_DIR += msdc
endif

MODULE_DIR += tve
MODULE_DIR += nptv6896 aud adspbin6896
MODULE_DIR += dmx6896
ifdef CC_SUPPORT_VENC
MODULE_DIR += venc
endif
MODULE_DIR += vdec_v2
MODULE_DIR += muxer
MODULE_DIR += debug
MODULE_DIR += ckgen
MODULE_DIR += image
MODULE_DIR += feeder # fvr
#MODULE_DIR += pod
MODULE_DIR += pcmcia
ifdef CC_INET_SUPPORT
MODULE_DIR += net
endif
MODULE_DIR += fci
MODULE_DIR += gdma
MODULE_DIR += slt
MODULE_DIR += lzma

ifdef ENABLE_MULTIMEDIA
MODULE_DIR += swdmx_v2
MODULE_DIR += playmgr_v2
endif

ifdef DRV_SUPPORT_MT8283
MODULE_DIR += extmjc_8283
else
MODULE_DIR += extmjc
endif

ifeq "$(CPU_TEST)" "y"
MODULE_DIR += dhrystone
MODULE_DIR += cpu
endif

#MODULE_DIR += share_lib
#############################################################################
#
#  Basic boot up module
#

FIRST_INC_DIR = drv_inc
BASIC_DRV_DIR = bsp cli chip nor rtos util drv_cust


#############################################################################
#
#  Formal variable declaration
#

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ..
endif


SRC =

OBJ =

SUB_COMPONENTS =  # $(FIRST_INC_DIR)
SUB_COMPONENTS += $(BASIC_DRV_DIR)
OPT_COMPONENTS = fm
OPTIONAL_SUB_COMPONENTS = brdcst_if
ifdef TunerVendor
OPTIONAL_SUB_COMPONENTS += brdcstif_vendor
endif
DRIVER_DIR_LIST = $(BASIC_DRV_DIR) $(OPTIONAL_SUB_COMPONENTS) $(OPT_COMPONENTS)

ifeq "$(BOOT_CLI)" "y"
else
SUB_COMPONENTS += $(MODULE_DIR)
DRIVER_DIR_LIST += $(MODULE_DIR)
endif

DEFINES += -DCC_MT5368
DEFINES +=-DCC_SCPOS_EN -DCC_SRM_ON
DEFINES += -DCC_53XX_SWDMX_V2
DEFINES += -DCC_53XX_JPG_ARGB4444

# for SLT
ifeq "$(MODEL)" "slt5368"
SUB_COMPONENTS += slt
DRIVER_DIR_LIST += slt
endif

#REF_TARGET = 5391
ADD_TO_SYMLINK_DIRS = dhrystone brdcst_if loaderlib fm_loader share_lib inc
ifdef TunerVendor
ADD_TO_SYMLINK_DIRS += brdcstif_vendor
endif
ADD_TO_SYMLINK_FILES =

#############################################################################
#
#  Special export variables
#

export DRIVER_DIR_LIST
# export NO_LINT = this
LIBRARY_PATH := $(subst 5368_driver,5368,$(word 2,$(subst target/mt5368/, , $(subst drv_build/mt5368/driver/, , $(shell pwd)))))


#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak

.PHONY: get_DRIVER_DIR_LIST
get_DRIVER_DIR_LIST:
	@echo "$(DRIVER_DIR_LIST)"
