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
# $Revision: #8 $
# $Date: 2009/06/24 $
# $Author: luis.chen $
# $CCRevision: /main/DTV_X_ATSC/6 $
# $SWAuthor: Joshuah Huang $
# $MD5HEX: e2c57029ea8dd3e986bb5c3b4fa60554 $
#
# Description:
#         For each main program to define the executable file name.
#
#         Specify the source files to be compile in SRC.
#############################################################################

ifndef TARGET
TARGET		:= mt5301a
endif

ifndef CUSTOMER
CUSTOMER	:= cmtk / mtk
endif
VERSION		:= 2.00-alpha

ifndef THIS_ROOT
THIS_ROOT	:= $(shell pwd)
endif

#
# Override Object Type
#
export APPEND_OBJ_TYPE := loader

ifndef PROJECT_ROOT
PROJECT_ROOT	:= $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
endif

export THUMB = y

export LOADER_OPTIMIZE_LVL = -O2

export 5301A_LOADER = y
export MTK_LOADER = y

include $(PROJECT_ROOT)/path.mak
include $(TARGET_ROOT)/target.mak

# SRC := loader_main.c xmodem.c x_crc16.c zip.c dummy_cli.c
SRC := loader_main.c
SRC += loader_cmd.c
SRC += memtest_cmd.c eeprom_cmd.c nor_cmd.c nand_cmd.c # pmx_cmd.c

LOADER_LIB         := $(subst mt,,$(TARGET))_loader

-include $(TARGET_ROOT)/cfg.mak

ifeq ($(BOOT), nand)
DEFINES += -DCC_NAND_BOOT
endif

# For memory configuration
DEFINES += -DFBM_MEM_CFG=FBM_MEM_CFG_$(FBM_MEM_CFG)
DEFINES += -DCC_MTK_LOADER -DCC_CLI

# USB module include path.
USBDIR =  $(TARGET_ROOT)/$(subst mt,,$(TARGET))_driver/usb
CC_INC += -I$(USBDIR)/include \
          -I$(USBDIR)/include/class \
          -I$(USBDIR)/src/systems/ucos \
          -I$(USBDIR)/src/systems/ucos/afs \
          -I$(USBDIR)/src/cd \
          -I$(USBDIR)/src/drivers/msd \
          -I$(USBDIR)/src/drivers/hub \
	  -I$(TARGET_ROOT)/$(subst mt,,$(TARGET))_driver/nptv/inc \
	  -I$(TARGET_ROOT)/$(subst mt,,$(TARGET))_driver/nptv/inc/drv \
	  -I$(TARGET_ROOT)/$(subst mt,,$(TARGET))_driver/nptv/inc/hw \
	  -I$(TARGET_ROOT)/$(subst mt,,$(TARGET))_driver/nptv/inc/sys \
	  -I$(TARGET_ROOT)/$(subst mt,,$(TARGET))_driver/loaderlib \
	  -I$(TARGET_ROOT)/$(subst mt,,$(TARGET))_driver/fm_loader/fat \
	  -I$(TARGET_ROOT)/$(subst mt,,$(TARGET))_driver/fm_loader \
	  -I$(TARGET_ROOT)/$(subst mt,,$(TARGET))_loader/dlm

EXE_NAME		:= $(EXE_NAME)_loader

#
# Specify some root path's
#
OBJ_DIR        := $(THIS_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
DEPEND_DIR     := $(THIS_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
BIN_DIR        := $(THIS_ROOT)/bin/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
TARGET_LIB_DIR := $(TARGET_ROOT)/lib_target/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)

PRIV_OBJ_DIR   := $(THIS_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)


export OBJ_DIR
export DEPEND_DIR
export BIN_DIR
export TARGET_LIB_DIR

export PRIV_OBJ_DIR

# Specify the object files with full path
#
ifneq "$(BUILD_PRIV)" "true"
OBJ      = $(SRC:.c=.o) # pagetab.o
else
OBJ = $(shell sh ${PRIV_BUILD_SHELL} -obj -c -public $(SRC))
PRIV_OBJ = $(shell sh ${PRIV_BUILD_SHELL} -obj -c -priv $(SRC))
ABS_PRIV_OBJ := $(addprefix $(PRIV_OBJ_DIR)/, $(PRIV_OBJ))
endif

ABS_OBJ := $(addprefix $(OBJ_DIR)/, $(OBJ))

ifndef CUST_ABS_OBJ
CUST_ABS_OBJ := 
endif

export OBJ
export ABS_OBJ
export CUST_ABS_OBJ

export PRIV_OBJ
export ABS_PRIV_OBJ


#
# Specify the absolute executable
#
ABS_EXE := $(BIN_DIR)/$(EXE_NAME)$(TARGET_EXE_SUFFIX)

export ABS_EXE


#
# Specify the dependability files
#
ifneq "$(BUILD_PRIV)" "true"
DEPEND     := $(SRC:.c=.d)
else
DEPEND = $(shell sh ${PRIV_BUILD_SHELL} -dep -c -public $(SRC))
endif
ABS_DEPEND := $(addprefix $(DEPEND_DIR)/, $(DEPEND))

export DEPEND
export ABS_DEPEND

ABS_DEFINES = $(sort $(DEFINES))

#
# Set the actual compiler include path and define
#
ABS_INCLUDE = $(TARGET_INC) $(TARGET_DRV_INC) -I$(X_INC_DIR) -I$(C_INC_DIR) -I$(CUSTOM_ROOT) -I$(THIS_ROOT) $(sort $(CC_INC))

ABS_DEFINES = $(sort $(DEFINES))


#
# Add local library definitions
#
LIBS    += $(LOADER_LIB)
LIB_DIR := $(TARGET_LIB_DIR)

#
# Specify the libraries and executable
#
ABS_LIBS      := $(call set_lib,$(LIBS))
ABS_LIBS_PATH := $(call set_lib_path,$(LIB_DIR)) $(TARGET_LIBS_PATH)

ifeq "$(LARGE_IMAGE)" "y"
LOADER_SCATTER_FILE = far_loader_scatter.sc
DEFINES += -DCC_LOADER_MEM_OFFSET=0x2D00000
endif

ifndef LOADER_SCATTER_FILE
LOADER_SCATTER_FILE := loader_scatter.sc
endif

# Redefine LD flags
#
LD_FLAG = --entry INT_Reset --scatter $(LOADER_SCATTER_FILE) --locals --symdefs $(call path_cnvt,$(ABS_EXE)).sym --xref --map --info sizes --list $(call path_cnvt,$(ABS_EXE)).lst

export LD_FLAG


.PHONY : all lib_target drv_inc clean clean_lib_target

# Add zlib building to build our own zip lib for gzip support.
all: lib_target $(OBJ)
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(BIN_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(call path_cnvt,$(BIN_DIR));)
	@echo "lib_dir:$(LIB_DIR) target_libs_path:$(TARGET_LIBS_PATH)"
	$(LD) $(LD_FLAG) $(LD_OUTPUT) $(call path_cnvt,$(ABS_EXE)) $(call path_cnvt,$(ABS_OBJ)) $(call path_cnvt,$(ABS_PRIV_OBJ)) $(CUST_ABS_OBJ) $(call path_cnvt,$(ABS_LIBS_PATH)) $(ABS_LIBS)
	$(CP) $(CP_FLAG) $(call path_cnvt,$(ABS_EXE)) .

lib_target:
	@$(MAKE) -C $(TARGET_ROOT)/$(subst mt,,$(TARGET))_loader OPTIMIZE_LVL=$(LOADER_OPTIMIZE_LVL); \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	$(call gen_archieve_fct,$(call path_cnvt,$(TARGET_LIB_DIR)),$(AR),$(AR_FLAG))

drv_inc:
	@$(MAKE) -C $(TARGET_ROOT)/$(subst mt,,$(TARGET))_driver/drv_inc OPTIMIZE_LVL=$(LOADER_OPTIMIZE_LVL)

%.o : %.c
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(OBJ_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(call path_cnvt,$(OBJ_DIR));)
	@echo "$(C_INC_DIR)"
	@echo "---------------- Building `basename $@` ----------------"
	$(CC) -c $(CC_FLAG) $(call inc_path_cnvt,$(ABS_INCLUDE)) $(ABS_DEFINES) $(call path_cnvt,$<) -o $(call path_cnvt,$(OBJ_DIR)/$@)

%.o : %.s
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(OBJ_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(call path_cnvt,$(OBJ_DIR));)
	@echo ""
	@echo "---------------- Building `basename $@` ----------------"
	$(ASM) $(ASM_DEFINES) -o $(call path_cnvt,$(OBJ_DIR)/$@) $(call path_cnvt,$<)

clean_lib_target:
	@$(MAKE) -C $(TARGET_ROOT)/$(subst mt,,$(TARGET))_loader OPTIMIZE_LVL=$(LOADER_OPTIMIZE_LVL) clean

clean: clean_lib_target
	@$(RM) $(RM_REC_FLAG) $(call path_cnvt,$(OBJ_DIR))
	@$(RM) $(RM_FLAG) $(call path_cnvt,$(ABS_OBJ)) $(call path_cnvt,$(ABS_EXE)) $(NULL_DEV)
	@$(RM) $(RM_REC_FLAG) $(call path_cnvt,$(TARGET_LIB_DIR)) $(NULL_DEV)

