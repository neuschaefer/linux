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
ifeq "$(BUILD_CFG)" "debug"
CFG_DEF := $(MODEL_NAME)_dbg.cfg
else

ifeq "$(BUILD_CFG)" "cli"
CFG_DEF := $(MODEL_NAME)_cli.cfg
else

CFG_DEF := $(MODEL_NAME).cfg
endif
endif


#
# Set the optimization level
#
ifdef OPTIMIZE_LVL
export OPTIMIZE_LVL
endif


ifndef LOADER_MAX_SIZE
export LOADER_MAX_SIZE = 131072
endif

DEFINES += -DLOADER_MAX_SIZE=$(LOADER_MAX_SIZE)

ifdef PQDATA_ADDRESS
DEFINES += -DUSER_PQDATA_ADDRESS=$(PQDATA_ADDRESS)
endif

#
# Get all the path values set
#
include $(PROJECT_ROOT)/linux_porting.mak

ifndef PROJECT_ROOT
PROJECT_ROOT := $(LINUX_PORTING_PROJECT_X_PATH)
endif

include $(PROJECT_ROOT)/path.mak
include $(TARGET_ROOT)/target.mak


#
# List the sources which must be compiled	#####
#
# SRC := sys_start.c
ifndef TARGET_DRIVER
export TARGET_DRIVER := $(subst mt,,$(TARGET))_driver
endif


#
# Target sub component list. Note that front end selection is handled specifically.
# If the target drivers would not require compilation flags, there would not have
# to be an inclusion of an additional definition file, which depends on the front
# end driver type.
#
ifndef TARGET_SUB_COMP_LIST
TARGET_SUB_COMP_LIST := 5351_driver
endif

ifeq "$(TARGET)" "mt5381"
#TARGET_SUB_COMP_LIST += mw_if diag
TARGET_SUB_COMP_LIST += diag
endif

ifneq "$(BRDCST_IF)" "none"
TARGET_SUB_COMP_LIST += brdcst_if
endif


ifeq "$(CA_IF)" "pod_dk"
TARGET_SUB_COMP_LIST += digitalkeystone
endif

ifeq "$(1394_IF)" "vividlogic"
TARGET_SUB_COMP_LIST += vividlogic
endif

ifeq "$(TARGET)" "simulator"
TARGET_SUB_COMP_LIST += brdcst_if/$(BRDCST_IF) dummy start bsp dbg dump os
endif

#
# Add additional selective fonts
#
CUST_SEL_FONTS := $(addprefix $(CUSTOM_BASE)/,$(SEL_FONTS))

#
# Have to do that because the 5351 tuner driver requires knowledge of the specific
# front end.
#
NIM := $(BRDCST_IF)
export BRDCST_IF
export NIM


#
# Executable name
#
EXE_NAME := $(MODEL_NAME)

ifeq "$(BUILD_CFG)" "debug"
EXE_NAME := $(EXE_NAME)_dbg

else
ifeq "$(BUILD_CFG)" "cli"
EXE_NAME := $(EXE_NAME)_cli
endif
endif

MAP_NAME := $(EXE_NAME)$(MAP_SUFFIX)

export EXE_NAME
export MAP_NAME

include $(PROJECT_ROOT)/object_type.mak

#
# Specify some root path's
#
OBJ_DIR    := $(THIS_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
DEPEND_DIR := $(THIS_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
BIN_DIR    := $(THIS_ROOT)/bin/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)

export OBJ_DIR
export DEPEND_DIR
export BIN_DIR


#
# Specify the object files with full path
#
OBJ      = $(SRC:.c=.o)
ABS_OBJ := $(addprefix $(OBJ_DIR)/, $(OBJ))

export OBJ
export ABS_OBJ


#
# Specify the dependability files
#
DEPEND     := $(SRC:.c=.d)
ABS_DEPEND := $(addprefix $(DEPEND_DIR)/, $(DEPEND))

export DEPEND
export ABS_DEPEND


#
# Specify the absolute executable and map file
#
ABS_EXE := $(BIN_DIR)/$(EXE_NAME)$(TARGET_EXE_SUFFIX)
ABS_MAP := $(BIN_DIR)/$(MAP_NAME)
ifeq "$(TARGET)" "mt5381"
LOADER_EXE := $(BIN_DIR)/loader/$(EXE_NAME)$(TARGET_EXE_SUFFIX)
else
LOADER_EXE := $(ABS_EXE)
endif

export ABS_EXE
export ABS_MAP


#
# MT5351 specific commands
#
FROMELF := fromelf
GZIP := gzip

ifndef MTKTOOL_BIN
MTKTOOL_BIN := $(EXE_NAME)_image.bin
endif

#
# DOS platform don't do packing.
#
ifeq "$(PROJECT_X_HOST)" "dos"
DOLOADER := loader
else
#DOLOADER := loader packing
ifeq "$(LINUX_SOLUTION)" "true"
DOLOADER :=
else
DOLOADER := loader 
endif
endif

#
# Packing tool
#
MT53XX_PACK = $(TOOL_ROOT)/$(TARGET)_pack/$(TARGET)_pack_$(PROJECT_X_HOST)
MT5351_CMDLINE = $(MT53XX_PACK) $(EXE_NAME)_loader.bin $(EXE_NAME).bin.gz $(MTKTOOL_BIN)
MT5371_CMDLINE = $(MT53XX_PACK) -l $(EXE_NAME)_loader.bin -a $(EXE_NAME).bin.gz -t $(MTKTOOL_BIN) $(EFUSE_CMD) -c $(FBM_MEM_CFG)
MT5381_CMDLINE = $(MT53XX_PACK) $(EXE_NAME)_loader.bin $(EXE_NAME).bin.lzhs $(MTKTOOL_BIN) 2 "$(CUSTOMER)" "$(MODEL_NAME)" "$(VERSION)" $(LOADER_MAX_SIZE)

ifeq "$(TARGET)" "mt5371"
MT53XX_CMDLINE  = $(MT5371_CMDLINE)
MT53XX_COMPRESS = $(GZIP) -9 $(EXE_NAME).bin
else

ifeq "$(TARGET)" "mt5351"
MT53XX_CMDLINE  = $(MT5351_CMDLINE)
MT53XX_COMPRESS = $(GZIP) -9 $(EXE_NAME).bin
else

ifeq "$(TARGET)" "mt5381"
MT53XX_CMDLINE  = $(MT5381_CMDLINE)
MT53XX_COMPRESS = $(TOOL_ROOT)/$(TARGET)_pack/lzhsenc $(EXE_NAME).bin $(EXE_NAME).bin.lzhs

else

MT53XX_PACK =
MT53XX_CMDLINE =
endif

endif

endif

ifndef DTV_DRIVER_ADPT_ROOT
DTV_DRIVER_ADPT_ROOT := $(LINUX_PORTING_DTV_LINUX_PRJ_PATH)/dtv_driver_adpt
endif

ifndef DTV_PROCESS_ROOT
DTV_PROCESS_ROOT := $(PROJECT_ROOT)/dtv_process
endif

export DTV_DRIVER_ADPT_ROOT
export DTV_PROCESS_ROOT

LINK := ln

#
# Check Fast Build
#
export BRANCH_NAME  = $(word 2,$(BUILD_NAME))
export WARNING_LOG  = $(THIS_ROOT)/build_$(TARGET).log
export ERROR_LOG    = $(THIS_ROOT)/build_fail.log
export CHK_ERR_WARN_SCRIPT = $(TOOL_ROOT)/pbuild/chk_warn_err.sh 
export CHK_ERR_WARN_PARM   = $(TOOL_ROOT)/pbuild $(BRANCH_NAME) $(WARNING_LOG) $(ERROR_LOG)


ifndef FAST_SUPPORT
export FAST_SUPPORT = true
endif

ifneq "$(FAST_SUPPORT)" "false"
MAKE = $(FMAKE)
endif

#
#for archieve library
#
LIB_LIST =	$(wildcard $(AR_LIB_PATH)/*.content)

#
# Build the bits
#
.PHONY : all unprepare prepare cfg mw x_inc c_inc custom target image packing clean clean_mw clean_x clean_c clean_custom clean_target clean_image clean_force $(LIB_LIST) ar_lib

ifeq "$(RLS_CUSTOM_BUILD)" "true"
all: init cfg x_inc c_inc dtv_process mw custom target image $(DOLOADER)
else
all: init prepare cfg x_inc c_inc dtv_process mw custom target image $(DOLOADER)
endif

init: remove_lock
	@echo "FINISH INITIALIZATION"

cfg :
	@chmod +x $(GEN_CFG)
	$(GEN_CFG) -i $(CFG_DEF) -r $(PROJECT_ROOT)

remove_lock : cfg
	@echo "REMOVE LIBRARY LOCKFILE"
	$(call if_dir_exist_fct,$(call path_cnvt,$(MW_LIB_DIR)),        find $(call path_cnvt,$(MW_LIB_DIR))/        -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)
	$(call if_dir_exist_fct,$(call path_cnvt,$(CUSTOM_LIB_DIR)),    find $(call path_cnvt,$(CUSTOM_LIB_DIR))/    -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)
	$(call if_dir_exist_fct,$(call path_cnvt,$(DTV_LINUX_LIB_DIR)), find $(call path_cnvt,$(DTV_LINUX_LIB_DIR))/ -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)
	@$(RM) $(RM_REC_FLAG) $(WARNING_LOG) $(ERROR_LOG)


ar_lib: $(LIB_LIST) 
	@echo "Archieve Library Done"
	
$(LIB_LIST):
	@sh $(TOOL_ROOT)/archieve_lib/ar_lib.sh $(AR) $(AR_FLAG) `echo $@|sed -r 's/\.content//'` $@
	$(RM) $(RM_REC_FLAG) $@

unprepare:
	@$(RM) -f $(DTV_PROCESS_ROOT)/include/cb_inc
	@$(RM) -f $(DTV_PROCESS_ROOT)/include/ioctl_inc
	@$(RM) -f $(DTV_PROCESS_ROOT)/direct_api/ioctl_direct_call.h

prepare: unprepare
	@$(LINK) -s $(DTV_DRIVER_ADPT_ROOT)/common/include/cb_inc/                              $(DTV_PROCESS_ROOT)/include/cb_inc
	@$(LINK) -s $(DTV_DRIVER_ADPT_ROOT)/common/include/ioctl_inc/                           $(DTV_PROCESS_ROOT)/include/ioctl_inc
	@$(LINK) -s $(DTV_DRIVER_ADPT_ROOT)/common/adaption/adpt_direct_api/ioctl_direct_call.h $(DTV_PROCESS_ROOT)/direct_api/ioctl_direct_call.h

cfg :
	$(GEN_CFG) -i $(CFG_DEF) -r $(PROJECT_ROOT)

dtv_process: cfg
	$(MAKE) -C $(DTV_PROCESS_ROOT) all INC_CFG_MAK=TRUE; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	$(call if_file_exist_fct, $(ERROR_LOG), exit 1;)
	$(MAKE) -f $(TARGET_ROOT)/customer.mak ar_lib AR_LIB_PATH=$(call path_cnvt,$(DTV_LINUX_LIB_DIR))

x_inc c_inc : cfg
	$(MAKE) -C $(MW_ROOT) $@ INC_CFG_MAK=TRUE LINUX_SOLUTION=false

fonts :
	@for i in $(CUST_SEL_FONTS); do \
        	$(MAKE) -C $(CUST_SEL_FONTS) all INC_CFG_MAK=TRUE; \
	done

mw : cfg
	$(MAKE) -C $(MW_ROOT) all INC_CFG_MAK=TRUE; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	$(call if_file_exist_fct, $(ERROR_LOG), exit 1;)
	$(MAKE) -f $(TARGET_ROOT)/customer.mak ar_lib AR_LIB_PATH=$(call path_cnvt,$(MW_LIB_DIR))
ifeq "$(RLS_CUSTOM_BUILD)" "true"
	$(CP) $(PROJECT_ROOT)/mtk_obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/lib_mw/*.a $(MW_LIB_DIR)
endif

custom : cfg fonts
	$(MAKE) -C $(CUSTOM_ROOT) all INC_CFG_MAK=TRUE; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	$(call if_file_exist_fct, $(ERROR_LOG), exit 1;)
	$(MAKE) -f $(TARGET_ROOT)/customer.mak ar_lib AR_LIB_PATH=$(call path_cnvt,$(CUSTOM_LIB_DIR))

target : cfg
	$(MAKE) -C $(TARGET_ROOT) INC_CFG_MAK=TRUE; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	$(call if_file_exist_fct, $(ERROR_LOG), exit 1;)
	$(MAKE) -f $(TARGET_ROOT)/customer.mak ar_lib AR_LIB_PATH=$(call path_cnvt,$(TARGET_LIB_DIR))

image: cfg
	$(MAKE) -f all.mak all INC_CFG_MAK=TRUE


clean : dtv_process_clean clean_x clean_c clean_mw clean_custom clean_target clean_image unprepare
	@echo "remove Log"
	@$(RM) $(RM_REC_FLAG) $(WARNING_LOG) $(ERROR_LOG)
	@echo "remove OBJ Library Floder"
	@$(RM) $(RM_REC_FLAG) $(TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
	@$(RM) $(RM_REC_FLAG) $(MW_LIB_DIR)
	@$(RM) $(RM_REC_FLAG) $(CUSTOM_LIB_DIR)
	@$(RM) $(RM_REC_FLAG) $(TARGET_LIB_DIR)
	$(call if_dir_exist_fct,$(TARGET_ROOT)/obj,        $(call if_dir_empty_fct,$(TARGET_ROOT)/obj,        $(RM) $(RM_REC_FLAG) $(TARGET_ROOT)/obj);)
	$(call if_dir_exist_fct,$(TARGET_ROOT)/lib_target, $(call if_dir_empty_fct,$(TARGET_ROOT)/lib_target, $(RM) $(RM_REC_FLAG) $(TARGET_ROOT)/lib_target);)
	$(call if_dir_exist_fct,$(TARGET_ROOT)/lib_custom, $(call if_dir_empty_fct,$(TARGET_ROOT)/lib_custom, $(RM) $(RM_REC_FLAG) $(TARGET_ROOT)/lib_custom);)
	$(call if_dir_exist_fct,$(TARGET_ROOT)/lib_mw,     $(call if_dir_empty_fct,$(TARGET_ROOT)/lib_mw,     $(RM) $(RM_REC_FLAG) $(TARGET_ROOT)/lib_mw);)
	$(call if_dir_exist_fct,$(TARGET_ROOT)/lib_dtv_process,     $(call if_dir_empty_fct,$(TARGET_ROOT)/lib_dtv_process,     $(RM) $(RM_REC_FLAG) $(TARGET_ROOT)/lib_dtv_process);)
	$(call if_dir_exist_fct,$(THIS_ROOT)/obj,          $(call if_dir_empty_fct,$(THIS_ROOT)/obj,          $(RM) $(RM_REC_FLAG) $(THIS_ROOT)/obj);)
	$(call if_dir_exist_fct,$(THIS_ROOT)/bin,          $(call if_dir_empty_fct,$(THIS_ROOT)/bin,          $(RM) $(RM_REC_FLAG) $(THIS_ROOT)/bin);)
	@echo "------------- make clean completed successfully ----------------------"

dtv_process_clean:
	@$(MAKE) -s -C $(DTV_PROCESS_ROOT) clean

clean_mw :
	@$(MAKE) -s -C $(MW_ROOT) clean INC_CFG_MAK=TRUE

clean_x clean_c :
	@$(MAKE) -s -C $(MW_ROOT) $@ INC_CFG_MAK=TRUE

clean_custom :
	@$(MAKE) -s -C $(CUSTOM_ROOT) clean INC_CFG_MAK=TRUE

clean_target :
	@$(MAKE) -s -C $(TARGET_ROOT) clean INC_CFG_MAK=TRUE

clean_image :
	@$(MAKE) -s -f clean.mak clean INC_CFG_MAK=TRUE

clean_force :
	@$(MAKE) -s -C $(MW_ROOT) $@ INC_CFG_MAK=TRUE
	@$(MAKE) -s -C $(CUSTOM_ROOT) $@ INC_CFG_MAK=TRUE
	@$(MAKE) -s -C $(TARGET_ROOT) $@ INC_CFG_MAK=TRUE
	@$(MAKE) -s -f clean.mak $@ INC_CFG_MAK=TRUE

nand_writer_scr_gen :
	$(MAKE) -C $(TOOL_ROOT)/nand_writer_scr_gen
	$(TOOL_ROOT)/nand_writer_scr_gen/nand_writer_scr_gen

clean_nand_writer_scr_gen :
	$(MAKE) -C $(TOOL_ROOT)/nand_writer_scr_gen clean

help:
	@echo "make"
	@echo "	To build release version without symbol & debug information"
	@echo "make BUILD_CFG=debug"
	@echo "	To build debug version"
	@echo "make help"
	@echo "	This menu"
	@echo "make BRDCST_IF={nim library}"
	@echo "	Default library is $(BRDCST_IF). To link with another nim library."
	@echo -n "	All nim libraries are:"
	@for i in `ls $(TARGET_ROOT)/brdcst_if`; do \
		if [ -f "$(TARGET_ROOT)/brdcst_if/$$i/libtuner.a" ]; then \
			echo -n " $$i"; \
		fi \
	done
	@echo " "

get_custom_name:
	@echo $(CUSTOM)
	
get_target_name:
	@echo $(TARGET)


