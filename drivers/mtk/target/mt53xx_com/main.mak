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
#############################################################################
# DO NOT MODIFY BELOW..........
#############################################################################
#
# Syntax of this Makefile
#
#	> make -> to build common_debug version
#
#	> make [clean|purge|lint|lint_dir|lint_file] -> to do op on common_debug
#			clean: remove obj/dep files in whole project.
#			purge: remove the directory containing all generated files in whole
#					project.
#			lint: Recursive do lint checking to stdout in main program directory.
#			lint_dir: do lint checking to stdout in main program directory.
#			lint_file: do lint checking to files in main program directory.
#
#	> make [model] -> to build [model]_debug version
#
#	> make [model]_[mode] -> to build [model]_[mode] version
#
#	> make [op]_[model] -> to do some operation at some model of debug mode.
#
#	> make [op]_[model]_[mode] -> to do some operation at some model of some mode.
#			op are build, clean, purge, and lint.
#
#	> make build_all
#	> make clean_all
#	> make purge_all
#
##############################################################################

EXE = $(THIS_COMPONENT)

ifneq ($(strip $(findstring /drv_build/, $(PWD))),)

#3rd_party library
ifeq "$(findstring DVBCT,$(BRDCST_IF))" "DVBCT"
export 3RD_PARTIES:=demod
endif
ifneq "$(findstring VDRISDBT,$(BRDCST_IF))" "VDRISDBT"
ifeq "$(findstring ISDBT,$(BRDCST_IF))" "ISDBT"
export 3RD_PARTIES:=demod
endif
endif

export DRV_BUILD_ROOT := $(word 1, $(subst /drv_build/,/drv_build /, $(PWD)))
DRV_BUILD_BASE := $(firstword $(subst /, ,$(word 2, $(subst /drv_build/,/drv_build /, $(PWD)))))
export DRV_ROOT := $(DRV_BUILD_ROOT)/$(DRV_BUILD_BASE)/driver

else

export DRV_ROOT := meow

endif

export EXE
export SRC
export DEFINES
export LOCAL_DEFINES
export CC_INC
export LOCAL_INC

ifndef MODEL
MODEL = common
endif

ifndef MODE
MODE = debug
endif

##############################################################################
# debug/release mode list
##############################################################################
MODE_LIST=debug release
-include $(DRV_ROOT)/symlink.mak

##############################################################################
# Build/Clean/Purge/Lint model list
##############################################################################

# Include ../model_list.mak to get $(MODEL_LIST)
-include $(DRV_ROOT)/model_list.mak

CLEAN_MODEL_LIST=$(addprefix clean_,$(MODEL_LIST))

PURGE_MODEL_LIST=$(addprefix purge_,$(MODEL_LIST))

LINT_MODEL_LIST=$(addprefix lint_,$(MODEL_LIST))

##############################################################################
# Build/Clean/Purge/Lint model_mode list
##############################################################################
DEBUG_MODEL_LIST=$(addsuffix _debug,$(MODEL_LIST))
RELEASE_MODEL_LIST=$(addsuffix _release,$(MODEL_LIST))
MODEL_MODE_LIST=$(DEBUG_MODEL_LIST) $(RELEASE_MODEL_LIST)

CLEAN_MODEL_MODE_LIST=$(addprefix clean_,$(MODEL_MODE_LIST))

PURGE_MODEL_MODE_LIST=$(addprefix purge_,$(MODEL_MODE_LIST))

LINT_MODEL_MODE_LIST=$(addprefix lint_,$(MODEL_MODE_LIST))


.PHONY : all clean purge lint lint_dir lint_file		\
	build_all clean_all purge_all mw_lib $(PREMAIN)		\
	$(MODEL_MODE_LIST) $(CLEAN_MODEL_MODEL_LIST)		\
	$(PURGE_MODEL_MODE_LIST) $(LINT_MODEL_MODE_LIST)


-include $(DRV_ROOT)/target.mak


##############################################################################
# default usage of our previous habit
##############################################################################
all: common
	@echo "Build common_debug finished"

clean: purge_all
	@echo "Clean common_debug finished"

purge: purge_common
	@echo "Purge common_debug finshed"

lint: lint_common
	@echo "Lint common_debug finished"

lint_dir:
	$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$(MODEL)) MODEL=$(MODEL) MODE=$(MODE) lint_dir

lint_file:
	$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$(MODEL)) MODEL=$(MODEL) MODE=$(MODE) lint_file

mw_lib:
	@make -C $(DRV_ROOT)
	@make -C $(DRV_ROOT)/mw_lib

help:
	@echo "make or make all"
	@echo "	To build common model debug version binary(common_debug)"
	@echo "make clean"
	@echo "	To clean obj/dep files of current directory and sub-directories of common_debug"
	@echo "make purge"
	@echo "	To clean all binaries/libraries/obj/dep/lint files of common_debug"
	@echo "make lint OR make lint_dir OR make lint_file"
	@echo "	To do lint based on common_debug"
	@echo "make build_all OR make clean_all OR make purge_all"
	@echo "	To build/clean/purge all [model]_debug"
	@echo "make [model] [MODE=[debug|release]]"
	@echo "	Models are $(MODEL_LIST)"
	@echo "make [model]_[mode]"
	@echo "	All targets are:"
	@echo "		$(MODEL_MODE_LIST)"
	@echo "make [op]_[model]"
	@echo "	All targets are:"
	@echo "		$(CLEAN_MODEL_LIST)"
	@echo "		$(PURGE_MODEL_LIST)"
	@echo "		$(LINT_MODEL_LIST)"
	@echo "make [op]_[model]_[mode]"
	@echo "	All targets are:"
	@echo "		$(CLEAN_MODEL_MODE_LIST)"
	@echo "		$(PURGE_MODEL_MODE_LIST)"
	@echo "		$(LINT_MODEL_MODE_LIST)"
	@echo ""
	@echo "make options"
	@echo "	[BRDCST_IF={nim library}] to link with correct nim library"
	@echo -n "	All library names are"
#	@for i in `ls $(DRV_ROOT)/brdcst_if`; do \
		if [ -f "$(DRV_ROOT)/brdcst_if/$$i/libtuner.a" ] ; then \
			echo -n " $$i"; \
		fi \
	done
	@echo " "

##############################################################################
# build/clean/purge/lint all
##############################################################################
build_all: mw_lib $(MODEL_MODE_LIST)
	@echo "Build all model $(MODEL_LIST) debug/release mode finshed"

clean_all: $(CLEAN_MODEL_MODE_LIST)
	@$(RM) -f *.axf *.bin

purge_all: $(PURGE_MODEL_MODE_LIST)
	@$(RM) -f *.axf *.bin


##############################################################################
# both modes of single model build/clean/purge/lint operations
##############################################################################
$(MODEL_LIST): $(PREMAIN)
	echo "make model_list ($@) [$(call NAME_COMPANY,$@)]"
	$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$@) MODEL=$@ MODE=$(MODE)

$(CLEAN_MODEL_LIST):
	echo "$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$(subst clean_,,$@)) MODEL=$(subst clean_,,$@) MODE=$(MODE) clean"
	$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$(subst clean_,,$@)) MODEL=$(subst clean_,,$@) MODE=$(MODE) clean

$(PURGE_MODEL_LIST):
	@$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$(subst purge_,,$@)) MODEL=$(subst purge_,,$@) MODE=$(MODE) purge

$(LINT_MODEL_LIST):
	@$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$(subst lint_,,$@)) MODEL=$(subst lint_,,$@) MODE=$(MODE) lint

##############################################################################
# model_mode build/clean/purge/lint operations
##############################################################################
# build model_mode binary
$(MODEL_MODE_LIST):
	echo "make model_mode_list $@"
	@$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$(word 1,$(subst _, ,$@))) MODEL=$(word 1,$(subst _, ,$@)) MODE=$(word 2,$(subst _, ,$@))

# clean model_mode object/dep of current directory source only
$(CLEAN_MODEL_MODE_LIST):
	@$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$(word 2,$(subst _, ,$@))) MODEL=$(word 2,$(subst _, ,$@)) MODE=$(word 3,$(subst _, ,$@)) clean

# clean model_mode obj/dep/lib/lint all generated files
$(PURGE_MODEL_MODE_LIST):
	@$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$(word 2,$(subst _, ,$@))) MODEL=$(word 2,$(subst _, ,$@)) MODE=$(word 3,$(subst _, ,$@)) purge

# do lint for model_mode definition
$(LINT_MODEL_MODE_LIST):
	@$(MAKE) -f $(DRV_ROOT)/exe.mak COMPANY=$(call NAME_COMPANY,$(word 2,$(subst _, ,$@))) MODEL=$(word 2,$(subst _, ,$@)) MODE=$(word 3,$(subst _, ,$@)) lint_file


