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
# $Revision: #24 $
# $Date: 2009/06/08 $
# $Author: hotkey.yang $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

THIS_COMPONENT = mt5387

########  !!!!!Put new driver module here !!!!!
MODULE =
MODULE_DIR =  vdp b2r
MODULE_DIR += fbm dbs srm
MODULE_DIR += gfx imgrz
MODULE_DIR += psr
MODULE_DIR += pdwnc
MODULE_DIR += osd
MODULE_DIR += pmx
MODULE_DIR += ir rtc
MODULE_DIR += uart sif
MODULE_DIR += usb
MODULE_DIR += cec
#MODULE_DIR += blkmgr smc
MODULE_DIR += nand
MODULE_DIR += tve
MODULE_DIR += aud 
MODULE_DIR += nptv
MODULE_DIR += dmx
MODULE_DIR += vdec
MODULE_DIR += debug
MODULE_DIR += ckgen
MODULE_DIR += jpg jpg_dec_8520
MODULE_DIR += feeder
#MODULE_DIR  += fvr
MODULE_DIR += pcmcia
#MODULE_DIR += net
MODULE_DIR += fci
MODULE_DIR += gdma

ifdef ENABLE_MULTIMEDIA
MODULE_DIR += swdmx
MODULE_DIR += playmgr
endif

#ifdef DRV_SUPPORT_EXTMJC
MODULE_DIR += extmjc
#endif

#symlink
MODULE_DIR += dhrystone



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

DEFINES += -DCC_MT5387
DEFINES +=-DCC_SCPOS_EN -DCC_SRM_ON

# for SLT
ifeq "$(MODEL)" "slt5388"
SUB_COMPONENTS += slt
DRIVER_DIR_LIST += slt
endif
ifeq "$(MODEL)" "slt5387"
SUB_COMPONENTS += slt
DRIVER_DIR_LIST += slt
endif

#REF_TARGET = 5391
#ADD_TO_SYMLINK_DIRS = dhrystone
#ADD_TO_SYMLINK_FILES = 

#############################################################################
#
#  Special export variables
#

export DRIVER_DIR_LIST
# export NO_LINT = this
LIBRARY_PATH := $(subst 5387_driver,5387,$(word 2,$(subst target/mt5387/, , $(subst drv_build/mt5387/driver/, , $(shell pwd)))))


#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak

.PHONY: get_DRIVER_DIR_LIST
get_DRIVER_DIR_LIST:
	@echo "$(DRIVER_DIR_LIST)"
