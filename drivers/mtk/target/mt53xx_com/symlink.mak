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
# $RCSfile: symlink.mak,v $
# $Revision: #1 $
# $Date: 2012/04/27 $
# $Author: dtvbm11 $
#
# Description:
#         makefile to build symbolic links
#############################################################################

ifndef PROJECT_ROOT
PROJECT_ROOT := $(strip $(word 1, $(subst /project_x/,/project_x /, $(shell pwd))))
endif

#
# which chip to reference source code
#
ifndef REF_TARGET
REF_TARGET = 53xx_com
endif

#
# this target model number
#
ifndef MY_TARGET
MY_TARGET = 5387
endif

ifndef REF_TARGET_ROOT
REF_TARGET_ROOT = $(PROJECT_ROOT)/target/mt$(REF_TARGET)
endif

#
# source file or directory location
#
MY_PATH = $(word 2, $(subst /mt$(MY_TARGET)/,/mt$(MY_TARGET) , $(shell bash -c pwd -L)))
MY_REF_PATH = $(word 2, $(subst /mt$(MY_TARGET)/,/mt$(MY_TARGET) , $(shell bash -c pwd -L)))
MY_REF_PATH := $(subst $(MY_TARGET),$(REF_TARGET),$(MY_REF_PATH))

#
# source files to do symbolic link
#
SYMLINK_FILES = $(SRC)

#
# check if additional source file to do symbolic link
#
ifneq ($(ADD_TO_SYMLINK_FILES),)
SYMLINK_FILES += $(ADD_TO_SYMLINK_FILES)
endif


#
# check if any source file will be removed from doing symbolic link
#
ifneq ($(REMOVE_FROM_SYMLINK_FILES),)
SYMLINK_FILES := $(filter-out $(REMOVE_FROM_SYMLINK_FILES), $(SYMLINK_FILES))
endif

ifeq ($(SYMLINK_FILES),)
SYMLINK_FILES_EMPTY=1
else
SYMLINK_FILES_EMPTY=0
endif


#
# directories to do symbolic link
#
export SYMLINK_DIRS = $(SUB_COMPONENTS) $(OPT_COMPONENTS)

#
# check if additional dirs to do symbolic link
#
ifneq ($(ADD_TO_SYMLINK_DIRS),)
SYMLINK_DIRS += $(ADD_TO_SYMLINK_DIRS)
endif

#
# check if any directory will be removed from doing symbolic link
#
ifneq ($(REMOVE_FROM_SYMLINK_DIRS),)
SYMLINK_DIRS := $(filter-out $(REMOVE_FROM_SYMLINK_DIRS), $(SYMLINK_DIRS))
endif

ifeq ($(SYMLINK_DIRS),)
SYMLINK_DIRS_EMPTY=1
else
SYMLINK_DIRS_EMPTY=0
endif


.PHONY: symlink clean_symlink gen_link $(LOCAL_LINK)

symlink: gen_link
#	@echo "target symlink dir->$(SYMLINK_DIRS)"
#	@echo "target symlink file->$(SYMLINK_FILES)"
	@if [ !$(SYMLINK_DIRS_EMPTY) ]; \
	then \
		for i in $(SYMLINK_DIRS); \
		do \
			if [ ! -h $$i -a -d $$i ]; \
			then \
				$(MAKE) $(MAKE_FLAG) -C $$i $@ || exit; \
			fi; \
		done; \
	fi;

gen_link: $(LOCAL_LINK)
	@if [ !$(SYMLINK_FILES_EMPTY) ]; \
	then \
		for i in $(SYMLINK_FILES); \
		do \
			if [ ! -e $$i -a ! -h $$i ]; \
			then \
				if [ -e $(REF_TARGET_ROOT)/$(MY_REF_PATH)/$$i -o -h $(REF_TARGET_ROOT)/$(MY_REF_PATH)/$$i ]; \
				then \
					echo "symlink_file: mt$(REF_TARGET)/$(MY_REF_PATH)/$$i"; \
					($(call if_file_notexist_w_sym_fct, $(REF_TARGET_ROOT)/$(MY_REF_PATH)/$$i, $(TARGET_ROOT)/$(MY_PATH)/$$i ); exit 0); \
				fi \
			fi; \
		done; \
	fi;
	@if [ !$(SYMLINK_DIRS_EMPTY) ]; \
	then \
		for i in $(SYMLINK_DIRS); \
		do \
			if [ ! -e $$i -a ! -h $$i ]; \
			then \
				if [ -e $(REF_TARGET_ROOT)/$(MY_REF_PATH)/$$i -o -h $(REF_TARGET_ROOT)/$(MY_REF_PATH)/$$i ]; \
				then \
					echo "symlink_dir:  mt$(REF_TARGET)/$(MY_REF_PATH)/$$i"; \
					($(call if_file_notexist_w_sym_fct, $(REF_TARGET_ROOT)/$(MY_REF_PATH)/$$i, $(TARGET_ROOT)/$(MY_PATH)/$$i ); exit 0); \
				fi \
			fi; \
		done; \
	fi;

clean_symlink:
	@if [ !$(SYMLINK_FILES_EMPTY) ]; \
	then \
		for i in $(SYMLINK_FILES); \
		do \
			if [ -h $$i -a -e $$i ]; \
			then \
				rm -f ./$$i; \
			fi; \
		done; \
	fi;
	@if [ !$(SYMLINK_DIRS_EMPTY) ]; \
	then \
		for i in $(SYMLINK_DIRS); \
		do \
			if [ -h $$i -a -e $$i ]; \
			then \
				rm -f ./$$i; \
			fi; \
		done; \
	fi;
	@if [ !$(SYMLINK_DIRS_EMPTY) ]; \
	then \
		for i in $(SYMLINK_DIRS); \
		do \
			if [ -d $$i ]; \
			then \
				$(MAKE) $(MAKE_FLAG) -C $$i $@ || exit; \
			fi; \
		done; \
	fi;
