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
# $RCSfile: customer.mak,v $
# $Revision: #1 $
# $Date: 2012/04/27 $
# $Author: dtvbm11 $
# $SWAuthor: Yan Wang $
#
# Description:
#         Makefile to build a "MT-5351" demo board build. The following
#         targets are supported:
#
#             all:           Compiles middleware, custom, target and this
#                            directory and creates a final image.
#             mw:            Compiles the middleware.
#             x_inc:         Creates and copies the middleware header files.
#             c_inc:         Creates and copies the application header files.
#             custom:        Compiles the custom.
#             target:        Compiles the MT-5351 target.
#             image:         Compiles this directory and creates a final
#                            image.
#             packing:
#             clean:         Cleans the middleware, custom, target and this
#                            directory and removes the run image.
#             clean_mw:      Cleans the middleware.
#             clean_custom:  Cleans the custom.
#             clean_target:  Cleans the MT-5351 target.
#             clean_image:   Cleans this directory and removes the run image.
#             clean_force:   Forcefully removes all libray and object
#                            directories as well as the run image. A fast way
#                            to perform cleanup.
#
#         The following commands are supported:
#
#             BUILD_CFG=debug
#                 Builds the specified target with symbolic debug info and the
#                 define file "mt5351_demo.def" is read. The run image is named
#                 is named "mt5351_demo_dbg".If BUILD_CFG is not specified then
#                 a non-debug build is created, which does not contain any
#                 symbolic debug information.
#
#             BRDCST_IF=<name>
#                 Allows the specifying of the front-end. The name must must
#                 be the directory name of the front-end, which follows the
#                 mt5351 targets brdcst_if sub-directory. For example,
#                 name may be set to "gen2004" or "5111" etc. In case
#                 BRDCST_IF is not set, "5111philips" is assumed.
#############################################################################

export TARGET
export BUILD_NAME
export CUSTOMER
export CUSTOM
export MODEL_NAME
export VERSION
export THIS_ROOT
#
# Set the configuration
#
OBJECT_TYPE := rel
ifeq "$(BUILD_CFG)" "debug"
CFG_DEF := $(MODEL_NAME)_dbg.cfg
OBJECT_TYPE := $(BUILD_CFG)
else

ifeq "$(BUILD_CFG)" "cli"
CFG_DEF := $(MODEL_NAME)_cli.cfg
OBJECT_TYPE := $(BUILD_CFG)
else

CFG_DEF := $(MODEL_NAME).cfg
OBJECT_TYPE := rel
endif
endif

export OUTPUT_ROOT     = $(THIS_ROOT)/output/$(OBJECT_TYPE)
export OUTPUT_SYMBOL_ROOT     = $(THIS_ROOT)/output_symbol/$(OBJECT_TYPE)
export LINUX_SOLUTION = true

ifndef VM_LINUX_ROOT
	export VM_LINUX_ROOT := $(word 1, $(subst /vm_linux/,/vm_linux /, $(shell pwd)))
endif

export BUILD_PHASE ?= PHASE2.0

include $(VM_LINUX_ROOT)/dtv_linux/mak/customer.mak

BUILD_PHASE ?= PHASE2.0
			
PHASE2_TARGET=$(shell make --no-print-directory -f $(VM_LINUX_ROOT)/project_x/target/$(TARGET)/customer_p2.mak get_phase2_target BUILD_PHASE=$(BUILD_PHASE) GINGA_SUPPORT=$(GINGA_SUPPORT) DEFINES="$(DEFINES)" CC="$(CC)")


#
# Build the bits
#
.PHONY : $(PHASE2_TARGET)

$(PHASE2_TARGET):
	@$(MAKE) -f $(VM_LINUX_ROOT)/project_x/target/$(TARGET)/customer_p2.mak $@ --no-print-directory
	
test:
	echo "PHASE2_TARGET=$(PHASE2_TARGET)"

