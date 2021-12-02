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
# $MD5HEX: 61118bc277d8ac078b83c14a0d509c1a $
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
#             loader:
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
#             clean_all:     Forcefully removes all version(rel/cli/debug) of libray and object
#                            directories as well as the run image. A fast way
#                            to perform cleanup.
#             clean_loader:
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

ifeq "$(LOADER_VERSION)" ""
LOADER_VERSION := $(VERSION)
endif

export TARGET
export BUILD_NAME
export CUSTOMER
export CUSTOM
export MODEL_NAME
export VERSION
export LOADER_VERSION
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

ifeq "$(BUILD_CFG)" "debug"
CFG_MEM_CHK = debug
else
ifeq "$(BUILD_CFG)" "cli"
CFG_MEM_CHK = cli
else
CFG_MEM_CHK = rel
endif
endif
#
# Set the optimization level
#
ifdef OPTIMIZE_LVL
export OPTIMIZE_LVL
endif


ifndef UBOOT_LOADER
export UBOOT_LOADER = FALSE
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
ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
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

export DRIVER_DIR := $(TARGET_DRIVER)

#
# Add local compile defines
#

TARGET_SPEC_DRV_CUST_HDR_FILE = d_mtk_cust.h
DEFINES += -DTARGET_SPEC_DRV_CUST_HDR_FILE=\"d_mtk_cust.h\"

#
# Add local include directives		#####
#
# CC_INC += -I$(CUSTOM_ROOT)/amb -I$(CUSTOM_ROOT)/app_util -I$(MW_ROOT) -I$(CUSTOM_ROOT)/am


#
# Add local tuner driver setting.	#####
#
# ifndef BRDCST_IF
# BRDCST_IF := 5111philips
# endif

#
# Target sub component list. Note that front end selection is handled specifically.
# If the target drivers would not require compilation flags, there would not have
# to be an inclusion of an additional definition file, which depends on the front
# end driver type.
#
ifndef TARGET_SUB_COMP_LIST
TARGET_SUB_COMP_LIST := 5351_driver
endif

TARGET_SUB_COMP_LIST += mw_if diag

ifneq "$(BRDCST_IF)" "none"
TARGET_SUB_COMP_LIST += # brdcst_if
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

ALL_EXE_NAME := $(MODEL_NAME) $(MODEL_NAME)_cli $(MODEL_NAME)_dbg 
export ALL_EXE_NAME
export EXE_NAME
export MAP_NAME

include $(PROJECT_ROOT)/object_type.mak

#
# Specify some root path's
#
OBJ_DIR    := $(THIS_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
DEPEND_DIR := $(THIS_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
BIN_DIR    := $(THIS_ROOT)/bin/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
OBJ_ROOT_DIR    := $(THIS_ROOT)/obj
DEPEND_ROOT_DIR := $(THIS_ROOT)/obj
BIN_ROOT_DIR    := $(THIS_ROOT)/bin
MTKLOADER_BIN_DIR := $(TARGET_ROOT)/mtkloader

export OBJ_DIR
export DEPEND_DIR
export BIN_DIR
export OBJ_ROOT_DIR
export DEPEND_ROOT_DIR
export BIN_ROOT_DIR
export MTKLOADER_BIN_DIR

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
LOADER_EXE := $(BIN_DIR)/loader/$(EXE_NAME)$(TARGET_EXE_SUFFIX)

export ABS_EXE
export ABS_MAP


#
# MT5351 specific commands
#
GZIP := gzip

ifndef MTKTOOL_BIN
MTKTOOL_BIN := $(EXE_NAME)_image.bin
endif

ALL_MTKTOOL_BIN := $(foreach i,$(ALL_EXE_NAME),$(i)_image.bin)
#
# DOS platform don't do packing.
#
ifeq "$(PROJECT_X_HOST)" "dos"
DOLOADER := loader
else
DOLOADER := loader packing
endif

#
# Packing tool
#
PACK_TOOL_DIR = $(TOOL_ROOT)/$(PACK_TOOL_NAME)
PACK_TOOL_NAME = mt5391_pack
MT53XX_PACK = $(PACK_TOOL_DIR)/$(PACK_TOOL_NAME)_$(PROJECT_X_HOST)
MTK_PACKIMG = $(PACK_TOOL_DIR)/packimg
ifdef SERIAL_NUMBER
PACK_CMDLINE = $(MT53XX_PACK) $(EXE_NAME)_loader.bin $(EXE_NAME).bin.lzhs $(MTKTOOL_BIN) 2 "$(CUSTOMER)" "$(MODEL_NAME)" "$(SERIAL_NUMBER)" $(LOADER_MAX_SIZE) 2
else
PACK_CMDLINE = $(MT53XX_PACK) $(EXE_NAME)_loader.bin $(EXE_NAME).bin.lzhs $(MTKTOOL_BIN) 2 "$(CUSTOMER)" "$(MODEL_NAME)" "$(LOADER_VERSION)" $(LOADER_MAX_SIZE) 2
endif
MT53XX_CMDLINE  = $(PACK_CMDLINE)

ifeq ($(LZMA_DECODE), true)
MT53XX_COMPRESS = $(TOOL_ROOT)/$(PACK_TOOL_NAME)/lzma $(EXE_NAME).bin $(EXE_NAME).bin.lzhs
else
MT53XX_COMPRESS = $(TOOL_ROOT)/$(PACK_TOOL_NAME)/lzhsenc $(EXE_NAME).bin $(EXE_NAME).bin.lzhs
endif

#
# Check Fast Build
#
export BRANCH_NAME  = $(word 2,$(BUILD_NAME))
export WARNING_LOG  = $(THIS_ROOT)/build_$(TARGET).log
export ERROR_LOG    = $(THIS_ROOT)/build_fail.log
export CHK_ERR_WARN_SCRIPT = $(TOOL_ROOT)/pbuild/chk_warn_err.sh 
export CHK_ERR_WARN_PARM   = $(TOOL_ROOT)/pbuild $(BRANCH_NAME) $(WARNING_LOG) $(ERROR_LOG)

export CHECK_WARNING_BUILD ?= false
ifeq "$(CHECK_WARNING_BUILD)" "true"
export FIND_AUTHOR ?= true
endif

export FIND_AUTHOR ?= false

RLS_CUSTOM_BUILD_PROJX ?= false
ifeq "$(RLS_CUSTOM_BUILD_PROJX)" "true"
CHK_ERR_WARN_PARM += CUST_RLS
else
ifneq "$(FIND_AUTHOR)" "true"
CHK_ERR_WARN_PARM += CUST_RLS
endif
endif


ifndef FAST_SUPPORT
export FAST_SUPPORT=true
endif

ifeq "$(FAST_SUPPORT)" "false"
PARALLEL = 1
endif

#
#for archieve library
#
LIB_LIST =	$(wildcard $(AR_LIB_PATH)/*.content)


#
# Build the bits
#
.PHONY : all cfg mw x_inc c_inc custom target image loader packing clean clean_packing clean_mw clean_x clean_c clean_custom clean_target clean_image clean_force clean_loader clean_brdcst_if clean_sym_link clean_oad init fast fast_mw fast_custom fast_target fast_image fast_packing fast_loader target_symlink drv_inc $(LIB_LIST) ar_lib mem_analysis get_BRDCST_IF collect_output clean_collect_output pre_check clean_pre_check


all : pre_check init fast_ice image $(DOLOADER) do_oad mem_analysis doc collect_output

fast_ice:
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) mw target custom ; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	$(call if_file_exist_fct, $(ERROR_LOG), exit 1;)

mem_analysis:
ifeq "$(STATIC_CONSUMPTION)" "true"
	cd $(TOOL_ROOT)/mem_analysis/; \
	perl $(TOOL_ROOT)/mem_analysis/mem_chk_static.pl $(CUSTOMER) $(MODEL_NAME) $(CFG_MEM_CHK) 2 STATIC_CONSUMPTION
endif
ifeq "$(SYS_MEM_CHK)" "true"
	cd $(TOOL_ROOT)/mem_analysis/; \
	perl $(TOOL_ROOT)/mem_analysis/mem_chk_static.pl $(CUSTOMER) $(MODEL_NAME) $(CFG_MEM_CHK)
endif

do_oad :
ifneq "$(CUST_OAD_OUI)" ""
	@if [ -e ./oad_table.mak ]; then \
		for i in $(TS_LIST); do \
			$(MAKE) -f ./oad_table.mak $$i=true; \
		done \
	else \
		if [ -e $(TOOL_ROOT)/gen_oad/oad.sh ]; then \
			sh $(TOOL_ROOT)/gen_oad/oad.sh; \
		fi; \
	fi
endif

collect_output:
	@$(call if_dir_not_exist_fct, $(THIS_ROOT)/output, $(MKDIR) $(MKDIR_FLAG) $(THIS_ROOT)/output; );
	@$(CP) $(CP_FLAG) $(THIS_ROOT)/*.bin $(THIS_ROOT)/output/
	@$(CP) $(CP_FLAG) $(THIS_ROOT)/*.pkg $(THIS_ROOT)/output/
	@if [ -e $(THIS_ROOT)/make.log ]; then \
		$(CP) $(CP_FLAG) $(THIS_ROOT)/make.log $(THIS_ROOT)/output/; \
	fi
	
clean_collect_output:
	@$(RM) $(RM_REC_FLAG) $(THIS_ROOT)/output

fast_inc:
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) x_inc c_inc drv_inc 

init: target_symlink fast_inc remove_lock
	@echo "REMOVE LOG"
	@$(RM) $(RM_REC_FLAG) $(WARNING_LOG) $(ERROR_LOG)
	@echo "FINISH INITIALIZATION"

remove_lock : cfg
	@echo "REMOVE LIBRARY LOCKFILE"
	$(call if_dir_exist_fct,$(call path_cnvt,$(ALL_TARGET_LIB_DIR)), find $(call path_cnvt,$(ALL_TARGET_LIB_DIR))/ -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)
	$(call if_dir_exist_fct,$(call path_cnvt,$(ALL_MW_LIB_DIR)),     find $(call path_cnvt,$(ALL_MW_LIB_DIR))/ -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)
	$(call if_dir_exist_fct,$(call path_cnvt,$(ALL_CUSTOM_LIB_DIR)), find $(call path_cnvt,$(ALL_CUSTOM_LIB_DIR))/ -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)
	$(call if_dir_exist_fct,$(call path_cnvt,$(TARGET_ROOT)/$(TARGET_DRIVER)/drv_inc), find $(call path_cnvt,$(TARGET_ROOT)/$(TARGET_DRIVER)/drv_inc)/ -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)
	@$(RM) $(RM_REC_FLAG) $(WARNING_LOG) $(ERROR_LOG)

cfg :
	@chmod +x $(GEN_CFG)
	@$(LOCKFILE) cfg.lock
	$(GEN_CFG) -i $(CFG_DEF) -r $(PROJECT_ROOT) || ($(RM) $(RM_REC_FLAG) cfg.lock ; exit 1)
	@$(RM) $(RM_REC_FLAG) cfg.lock
	
	
target_symlink : cfg
ifneq "$(RLS_CUSTOM_BUILD_PROJX)" "true"
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(TARGET_ROOT) symlink INC_CFG_MAK=TRUE --no-print-directory -s
endif
	
drv_inc :
	@echo "BUILD DRV_INC"
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(TARGET_ROOT) do_drv_inc --no-print-directory -s INC_CFG_MAK=TRUE

x_inc c_inc : cfg
	@echo "BUILD $@"
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(MW_ROOT) $@ --no-print-directory -s INC_CFG_MAK=TRUE

fonts : cfg
	@for i in $(CUST_SEL_FONTS); do \
		$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(CUST_SEL_FONTS) all INC_CFG_MAK=TRUE; \
	done

mw :
	@echo "BUILD_MW_START_TIME = `$(DATE) $(DATE_FLAG)`"
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) --no-print-directory -C $(MW_ROOT) all INC_CFG_MAK=TRUE; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	$(call if_file_exist_fct, $(ERROR_LOG), exit 1;)
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -f $(TARGET_ROOT)/customer.mak ar_lib AR_LIB_PATH=$(call path_cnvt,$(MW_LIB_DIR))
	@echo "BUILD_MW_END_TIME   = `$(DATE) $(DATE_FLAG)`"

custom : fonts fast_inc
	@echo "BUILD_CUSTOM_START_TIME = `$(DATE) $(DATE_FLAG)`"
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) --no-print-directory -C $(CUSTOM_ROOT) all INC_CFG_MAK=TRUE; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	$(call if_file_exist_fct, $(ERROR_LOG), exit 1;)
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -f $(TARGET_ROOT)/customer.mak ar_lib AR_LIB_PATH=$(call path_cnvt,$(CUSTOM_LIB_DIR))
	@echo "BUILD_CUSTOM_END_TIME   = `$(DATE) $(DATE_FLAG)`"


target : x_inc clean_diag_linux_obj target_symlink
	@echo "BUILD_TARGET_START_TIME = `$(DATE) $(DATE_FLAG)`"	
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) --no-print-directory -C $(TARGET_ROOT) sub_comp SUB_COMP="$(TARGET_SUB_COMP_LIST)" INC_CFG_MAK=TRUE; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	$(call if_file_exist_fct, $(ERROR_LOG), exit 1;)
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -f $(TARGET_ROOT)/customer.mak ar_lib AR_LIB_PATH=$(call path_cnvt,$(TARGET_LIB_DIR))
	@echo "BUILD_TARGET_END_TIME   = `$(DATE) $(DATE_FLAG)`"
	
image :
ifeq "$(RLS_CUSTOM_BUILD_PROJX)" "true"
	@$(MKDIR) $(MKDIR_FLAG) $(TARGET_ROOT)/lib_mw/$(CUSTOMER)/$(MODEL_NAME)/$(OBJECT_TYPE)
	@$(MKDIR) $(MKDIR_FLAG) $(TARGET_ROOT)/lib_target/$(CUSTOMER)/$(MODEL_NAME)/$(OBJECT_TYPE)
	@$(CP) $(CP_FLAG) $(PROJECT_ROOT)/mtk_obj/$(CUSTOMER)/$(MODEL_NAME)/$(OBJECT_TYPE)/lib_mw/*.a      $(TARGET_ROOT)/lib_mw/$(CUSTOMER)/$(MODEL_NAME)/$(OBJECT_TYPE)
	@$(CP) $(CP_FLAG) $(PROJECT_ROOT)/mtk_obj/$(CUSTOMER)/$(MODEL_NAME)/$(OBJECT_TYPE)/lib_target/*.a  $(TARGET_ROOT)/lib_target/$(CUSTOMER)/$(MODEL_NAME)/$(OBJECT_TYPE)
endif
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) --no-print-directory -f all.mak all INC_CFG_MAK=TRUE; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))

clean : clean_mw clean_custom clean_target clean_image clean_loader clean_pack clean_cfg clean_oad clean_sym_link clean_packing clean_collect_output clean_pre_check
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
	$(call if_dir_exist_fct,$(THIS_ROOT)/obj,          $(call if_dir_empty_fct,$(THIS_ROOT)/obj,          $(RM) $(RM_REC_FLAG) $(THIS_ROOT)/obj);)
	$(call if_dir_exist_fct,$(THIS_ROOT)/bin,          $(call if_dir_empty_fct,$(THIS_ROOT)/bin,          $(RM) $(RM_REC_FLAG) $(THIS_ROOT)/bin);)

clean_cfg :
	@echo "remove cfg.mak"
	@chmod +x $(GEN_CFG)
	@$(GEN_CFG) -c $(CFG_DEF) -r $(PROJECT_ROOT) 1>/dev/null 2>/dev/null


clean_all_cfg : clean_cfg
	@$(GEN_CFG) -c $(MODEL_NAME).cfg -r $(PROJECT_ROOT)     1>/dev/null 2>/dev/null
	@$(GEN_CFG) -c $(MODEL_NAME)_cli.cfg -r $(PROJECT_ROOT) 1>/dev/null 2>/dev/null
	@$(GEN_CFG) -c $(MODEL_NAME)_dbg.cfg -r $(PROJECT_ROOT) 1>/dev/null 2>/dev/null


clean_mw :
	@echo "remove MW obj"
	$(call if_dir_exist_fct,$(TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE), $(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -C $(MW_ROOT) clean INC_CFG_MAK=TRUE;)

clean_x clean_c :
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -C $(MW_ROOT) $@ INC_CFG_MAK=TRUE


clean_custom :
	@echo "remove Custom obj"
	$(call if_dir_exist_fct,$(TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE), $(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -C $(CUSTOM_ROOT) clean INC_CFG_MAK=TRUE;)


clean_target :
	@echo "remove Target obj"
	$(call if_dir_exist_fct,$(TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE), $(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -C $(TARGET_ROOT) clean INC_CFG_MAK=TRUE;)


clean_image :
	@echo "remove Image"
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -i -f clean.mak clean INC_CFG_MAK=TRUE
	@if [ -e upg_pkg.mak ]; then 	                        \
		$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -f upg_pkg.mak clean INC_CFG_MAK=TRUE; \
	fi


clean_all_image :
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -f clean.mak clean_all INC_CFG_MAK=TRUE
	@if [ -e upg_pkg.mak ]; then 	                        \
		$(MAKE) -s -f upg_pkg.mak clean_all INC_CFG_MAK=TRUE; \
	fi

clean_force :
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -C $(MW_ROOT) $@ INC_CFG_MAK=TRUE
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -C $(CUSTOM_ROOT) $@ INC_CFG_MAK=TRUE
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -C $(TARGET_ROOT) $@ INC_CFG_MAK=TRUE
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -s -f clean.mak $@ INC_CFG_MAK=TRUE

clean_all : clean_x clean_c clean_all_image clean_all_loader clean_all_pack clean_all_cfg clean_oad
	@$(RM) $(RM_REC_FLAG) $(call path_cnvt,$(ALL_OBJ_DIR)) $(NULL_DEV)
	@$(RM) $(RM_REC_FLAG) $(call path_cnvt,$(ALL_LIB_LIB_DIR)) $(NULL_DEV)
	@$(RM) $(RM_REC_FLAG) $(call path_cnvt,$(ALL_MW_LIB_DIR)) $(NULL_DEV)
	@$(RM) $(RM_REC_FLAG) $(call path_cnvt,$(ALL_CUSTOM_LIB_DIR)) $(NULL_DEV)
	@$(RM) $(RM_REC_FLAG) $(call path_cnvt,$(ALL_TARGET_LIB_DIR)) $(NULL_DEV)

loader : fast_inc
ifeq "$(UBOOT_LOADER)" "TRUE"
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) --no-print-directory -C $(TARGET_ROOT)/uboot
else
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) --no-print-directory -C $(TARGET_ROOT)/mtkloader TARGET=$(TARGET) CFG_FILE_DIR=$(TARGET_ROOT)
endif

clean_oad :
	@echo "remove oad"
	@$(RM) -r $(RM_FLAG) $(THIS_ROOT)/oad

clean_loader :
ifeq "$(UBOOT_LOADER)" "TRUE"
	$(call if_dir_exist_fct,$(TARGET_ROOT)/uboot,     $(MAKE) -s -i -C $(TARGET_ROOT)/uboot --no-print-directory -s clean;)
else
	$(call if_dir_exist_fct,$(TARGET_ROOT)/mtkloader, $(MAKE) -s -i -C $(TARGET_ROOT)/mtkloader --no-print-directory -s clean;)
endif
	@echo "remove Loader"
	@$(RM) $(RM_FLAG) $(EXE_NAME) $(EXE_NAME).bin $(EXE_NAME).bin.gz $(EXE_NAME).bin.lzhs
	@$(RM) $(RM_FLAG) $(EXE_NAME)_loader.bin $(MTKTOOL_BIN)
	@$(RM) $(RM_FLAG) $(EXE_NAME)_dbg $(EXE_NAME)_dbg.bin $(EXE_NAME)_dbg.bin.gz $(EXE_NAME)_dbg.bin.lzhs
	@$(RM) $(RM_FLAG) $(EXE_NAME)_dbg_loader.bin $(MTKTOOL_BIN)

clean_all_loader : clean_loader
	$(RM) $(RM_FLAG) $(ALL_EXE_NAME) $(foreach i,$(ALL_EXE_NAME),$(i).bin) $(foreach i, $(ALL_EXE_NAME), $(i).bin.gz) $(foreach i,$(ALL_EXE_NAME), $(i).bin.lzhs)
	$(RM) $(RM_FLAG) $(foreach i,$(ALL_EXE_NAME), $(i)_loader.bin) $(ALL_MTKTOOL_BIN)


clean_pack :
	@echo "remove Packing"
	@$(RM) $(RM_FLAG) $(THIS_ROOT)/$(EXE_NAME)_loader.bin.hdr.bin $(THIS_ROOT)/$(EXE_NAME)_loader.bin
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) --no-print-directory -s -C $(TOOL_ROOT)/mt5391_pack INC_PATH=target/$(TARGET)/$(TARGET_DRIVER)/loaderlib TARGET=linux_pc TARGET_ROOT=$(PROJECT_ROOT)/target/linux_pc clean
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) --no-print-directory -s -C $(TOOL_ROOT)/mt5381_pack INC_PATH=target/$(TARGET)/$(TARGET_DRIVER)/loaderlib TARGET=linux_pc TARGET_ROOT=$(PROJECT_ROOT)/target/linux_pc clean

clean_all_pack :
ifeq "$(UBOOT_LOADER)" "TRUE"
	$(RM) $(RM_FLAG) $(foreach i,$(EXE_NAME),$(THIS_ROOT)/$(i)_loader.bin)
endif
	$(RM) $(RM_FLAG) $(foreach i, $(ALL_EXE_NAME),$(THIS_ROOT)/$(i)_loader.bin.hdr.bin) $(THIS_ROOT)/$(EXE_NAME)_loader.bin.hdr.bin
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) --no-print-directory -s -C $(TOOL_ROOT)/mt5391_pack INC_PATH=target/$(TARGET)/$(TARGET_DRIVER)/loaderlib TARGET=linux_pc TARGET_ROOT=$(PROJECT_ROOT)/target/linux_pc clean
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) --no-print-directory -s -C $(TOOL_ROOT)/mt5381_pack INC_PATH=target/$(TARGET)/$(TARGET_DRIVER)/loaderlib TARGET=linux_pc TARGET_ROOT=$(PROJECT_ROOT)/target/linux_pc clean

packing :
ifeq "$(CUSTOMER)" "mtk"
	cp -f $(TARGET_ROOT)/$(TARGET_DRIVER)/drv_cust/mtk/QTY_DEFAULT/pq.bin $(THIS_ROOT)/data/pq.bin
else
	cp -f $(TARGET_ROOT)/$(TARGET_DRIVER)/drv_cust/$(CUSTOMER)/QTY_$(MODEL_NAME)/pq.bin $(THIS_ROOT)/data/pq.bin
endif
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(TOOL_ROOT)/mt5391_pack INC_PATH=target/$(TARGET)/$(TARGET_DRIVER)/loaderlib TARGET=linux_pc TARGET_ROOT=$(PROJECT_ROOT)/target/linux_pc
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(TOOL_ROOT)/mt5381_pack INC_PATH=target/$(TARGET)/$(TARGET_DRIVER)/loaderlib TARGET=linux_pc TARGET_ROOT=$(PROJECT_ROOT)/target/linux_pc
	$(FROMELF) --bin $(EXE_NAME) --output $(EXE_NAME).bin
	$(RM) $(RM_FLAG) $(EXE_NAME).bin.gz $(EXE_NAME).bin.lzhs $(MTKTOOL_BIN)
ifeq ($(LZMA_DECODE), true)
	chmod +x $(TOOL_ROOT)/$(PACK_TOOL_NAME)/lzma
endif
	$(MT53XX_COMPRESS)
ifdef SECURE
	chmod +x $(TOOL_ROOT)/$(PACK_TOOL_NAME)/t_mtk
	$(TOOL_ROOT)/$(PACK_TOOL_NAME)/t_mtk $(EXE_NAME).bin.lzhs 0 1
	rm -rf 14KB_preloader.bin Code_Signature Customer_Key_Encrypt_Data Customer_Key_Signature SHA1_Signature
	rm -rf MTK_PRIVATE_KEY_0 MTK_PUBLIC_KEY_0 VENDOR_PRIVATE_KEY_0 VENDOR_PUBLIC_KEY_0
endif
ifeq "$(UBOOT_LOADER)" "TRUE"
	$(CP) $(CP_FLAG) $(TARGET_ROOT)/uboot/$(EXE_NAME)_loader.bin $(THIS_ROOT)/$(EXE_NAME)_loader.bin
else
	$(CP) $(CP_FLAG) $(TARGET_ROOT)/mtkloader/$(EXE_NAME)_mtkloader.bin $(EXE_NAME)_loader.bin
endif
	$(MT53XX_CMDLINE)
	@if [ -f packing.mak ]; then 								\
		$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -f packing.mak; 							\
	else											\
		echo "obase=16;$(LOADER_MAX_SIZE)" > .cal.txt;					\
		echo "quit" >> .cal.txt;							\
		echo "$(MTKTOOL_BIN)	.	." > packing_$(EXE_NAME).scr;			\
		echo -n "$(EXE_NAME)_loader.bin.hdr.bin 0x0 0x" >> packing_$(EXE_NAME).scr;	\
		bc -q .cal.txt >> packing_$(EXE_NAME).scr;					\
		echo -n "$(EXE_NAME).bin.lzhs.hdr 0x" >> packing_$(EXE_NAME).scr;		\
		echo -n `bc -q .cal.txt` >> packing_$(EXE_NAME).scr;				\
		echo " ." >> packing_$(EXE_NAME).scr;						\
		$(MTK_PACKIMG) -f packing_$(EXE_NAME).scr;					\
		$(RM) $(RM_FLAG) packing_$(EXE_NAME).scr;					\
	fi
	@if [ -f upg_pkg.mak ]; then \
		$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -f upg_pkg.mak; \
	fi
ifdef SCRIPT_LIST
	for i in $(SCRIPT_LIST); do \
		echo $(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -f upg_pkg.mak SCRIPT_PROFILE=$(THIS_ROOT)/upgrade_scripts/$$i; \
		$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -f upg_pkg.mak SCRIPT_PROFILE=$(THIS_ROOT)/upgrade_scripts/$$i; \
	done
endif
	@$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	@echo "BUILD_START_TIME = $(BUILD_START_TIME)"
	@echo "BUILD_END_TIME   = `$(DATE) $(DATE_FLAG)`"
	@echo "Finished ..."

clean_packing :
	@if [ -f packing.mak ]; then                          \
		$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -f packing.mak clean;                     \
	fi

ar_lib :$(LIB_LIST) 
	@echo "Archieve Library Done"
	
$(LIB_LIST) :
	@sh $(TOOL_ROOT)/archieve_lib/ar_lib.sh $(AR) $(AR_FLAG) `echo $@|sed -r 's/\.content//'` $@
	@$(RM) $(RM_REC_FLAG) $@


clean_brdcst_if :
	find $(TARGET_ROOT)/obj/ $(TARGET_ROOT)/lib_target/ -name brdcst_if -exec $(RM) $(RM_REC_FLAG) {} \;
	find $(TARGET_ROOT)/lib_target/ -name *P_analog* -exec $(RM) $(RM_REC_FLAG) {} \;
	find $(TARGET_ROOT)/lib_target/ -name libtuner* -exec $(RM) $(RM_REC_FLAG) {} \;

clean_sym_link :
	@echo "remove Symbolic Link"
	@find $(TARGET_ROOT) -type l -exec stat {} \; |grep "File:"|sed -r "s/\`//g"|sed -r "s/'//g"|grep -v secure|awk '{print $$2}' |xargs $(RM) $(RM_REC_FLAG)

nand_writer_scr_gen :
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(TOOL_ROOT)/nand_writer_scr_gen
	$(TOOL_ROOT)/nand_writer_scr_gen/nand_writer_scr_gen

clean_nand_writer_scr_gen :
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(TOOL_ROOT)/nand_writer_scr_gen clean

clean_diag_linux_obj :
ifneq "$(RLS_CUSTOM_BUILD_PROJX)" "true"
	$(RM) $(RM_FLAG) $(TARGET_ROOT)/diag/*.o
endif

	
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

all_lint : mw_lint custom_lint target_lint

mw_lint: cfg
	$(MAKE) -C $(MW_ROOT) lint INC_CFG_MAK=TRUE

custom_lint: fonts x_inc c_inc target_symlink
	$(MAKE) -C $(CUSTOM_ROOT) lint INC_CFG_MAK=TRUE

target_lint: x_inc
	$(MAKE) -C $(TARGET_ROOT) symlink do_drv_inc lint INC_CFG_MAK=TRUE DRV_LINT=TRUE

clean_all_lint: clean_mw_lint clean_custom_lint clean_target_lint

clean_mw_lint:
	$(MAKE) -C $(MW_ROOT) clean_lint INC_CFG_MAK=TRUE

clean_custom_lint:
	$(MAKE) -C $(CUSTOM_ROOT) clean_lint INC_CFG_MAK=TRUE

clean_target_lint:
	$(MAKE) -C $(TARGET_ROOT) clean_lint INC_CFG_MAK=TRUE

.PHONY : doc doc_init doc_do
AUTO_API_PREDEFINED="$(subst -D, ,$(DEFINES))"
AUTO_API_STRIP_FROM_PATH = "$(subst project_x, ,$(PROJECT_ROOT))"
ifdef API_LEVEL
  ifeq "$(API_LEVEL)" "all"
  	DOXYGEN_FILE_PATTERNS=*.h
  else
  ifeq "$(API_LEVEL)" "papi"
    DOXYGEN_FILE_PATTERNS=*.h
  else
  ifeq "$(API_LEVEL)" "xapi"
    DOXYGEN_FILE_PATTERNS=u_*.h x_*.h
    #DOXYGEN_INPUT_FILTER=sed '/\@page *pageMW/d'
    ENABLED_SECTIONS = DETAIL_DESCRIPTION_TYPE_X
  else
  ifeq "$(API_LEVEL)" "capi"
    DOXYGEN_FILE_PATTERNS=u_*.h c_*.h
    ENABLED_SECTIONS = DETAIL_DESCRIPTION_TYPE_C
  else
  ifeq "$(API_LEVEL)" "none"
    DOXYGEN_FILE_PATTERNS=*.h
  endif
  endif
  endif
  endif
  endif
else
  DOXYGEN_FILE_PATTERNS=*.h
endif

ifeq "$(API_MODULE)" "mw"
  DOXYGEN_INPUT_PATH=$(MW_ROOT)
  DOXYGEN_EXCLUDE_PATH=$(MW_ROOT)/atv_mngr  $(MW_ROOT)/html_psr $(MW_ROOT)/conn_mngr/outofband $(MW_ROOT)/evctx/evctx_oob_eng $(MW_ROOT)/svl_bldr/sb_oob_eng $(MW_ROOT)/svl_bldr/sb_cqam_eng $(MW_ROOT)/tbl_mngr/mhp_eng $(MW_ROOT)/tbl_mngr/sect_eng $(MW_ROOT)/tbl_mngr/scte_si_eng
else
ifeq "$(API_MODULE)" "custom"
	DOXYGEN_INPUT_PATH=$(PROJECT_ROOT)/custom
else
ifeq "$(API_MODULE)" "target"
	DOXYGEN_INPUT_PATH=$(TARGET_ROOT)
else
ifeq "$(API_MODULE)" "all"
	DOXYGEN_INPUT_PATH=$(MW_ROOT) $(PROJECT_ROOT)/custom/ $(TARGET_ROOT)
else
ifeq "$(API_MODULE)" "user"
	DOXYGEN_INPUT_PATH=$(INPUT_DIR)
else
ifeq "$(API_MODULE)" "none"
	DOXYGEN_INPUT_PATH=
endif
endif
endif
endif
endif
endif

ifeq "$(API_TODO_BUG_LIST)" "true"
	DOXYGEN_GENERATE_TODOLIST=YES
	DOXYGEN_GENERATE_BUGLIST=YES
else
	DOXYGEN_GENERATE_TODOLIST=NO
	DOXYGEN_GENERATE_BUGLIST=NO
endif

ifdef API_CUSTOM
  ifeq "$(API_CUSTOM)" "TPV"
  	DOXYGEN_HTML_STYLESHEET=$(PROJECT_ROOT)/tools/auto_api/config/style/html/TPV.css
  	DOXYGEN_HTML_HEADER = $(PROJECT_ROOT)/tools/auto_api/config/style/html/MTKheader_TPV.html
  else
  ifeq "$(API_CUSTOM)" "SAC"
    DOXYGEN_HTML_STYLESHEET=$(PROJECT_ROOT)/tools/auto_api/config/style/html/SAC.css
    DOXYGEN_HTML_HEADER = $(PROJECT_ROOT)/tools/auto_api/config/style/html/MTKheader_SAC.html
  endif
  endif
else
  DOXYGEN_HTML_STYLESHEET=$(PROJECT_ROOT)/tools/auto_api/config/style/html/doxygen.css
  DOXYGEN_HTML_HEADER = $(PROJECT_ROOT)/tools/auto_api/config/style/html/MTKheader.html
endif


export DOXYGEN_ENABLED_SECTIONS = $(ENABLED_SECTIONS)
export DOXYGEN_INPUT_PATH
export AUTO_API_PREDEFINED
export DOXYGEN_FILE_PATTERNS
export DOXYGEN_GENERATE_TODOLIST
export DOXYGEN_GENERATE_BUGLIST
export AUTO_API_STRIP_FROM_PATH
export DOXYGEN_EXCLUDE_PATH
export DOXYGEN_HTML_STYLESHEET
export DOXYGEN_HTML_HEADER
export DOXYGEN_INPUT_FILTER
doc:	doc_init doc_do

doc_init:
ifdef API_MODULE
ifneq "$(API_MODULE)" "none"
	rm -rf ./auto_api_doc
	mkdir -p ./auto_api_doc/html
	mkdir -p ./auto_api_doc/chm
	mkdir -p ./auto_api_doc/rtf
	cp $(PROJECT_ROOT)/tools/auto_api/config/style/html/MTK*.* ./auto_api_doc/html
	cp $(PROJECT_ROOT)/tools/auto_api/config/style/html/*.jpg ./auto_api_doc/html
	chmod 777 $(PROJECT_ROOT)/tools/auto_api/bin/doxygen
	chmod 777 $(PROJECT_ROOT)/tools/auto_api/bin/hhc
	chmod 777 $(PROJECT_ROOT)/tools/auto_api/bin/dot
	chmod 777 $(PROJECT_ROOT)/tools/auto_api/bin/rchm
export PATH := $(PATH):$(PROJECT_ROOT)/tools/auto_api/bin
endif
endif


doc_do:
ifdef API_MODULE
ifneq "$(API_MODULE)" "none"
	$(PROJECT_ROOT)/tools/auto_api/bin/doxygen $(PROJECT_ROOT)/tools/auto_api/config/Doxyfile
endif
endif

get_BRDCST_IF:
	@echo $(BRDCST_IF)

get_RE_3RD_PARITY_COMPONENTS:
	@echo $(3RD_PARTIES)
    
include $(PROJECT_ROOT)/target/mt53xx_com/precheck.mak
