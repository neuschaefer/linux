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
COPY = cp
COPYFLAG = -f
CPUPFLAG = -u

ifndef COMPANY
COMPANY := mtk
endif

ifndef DRV_INC_DIR
DRV_INC_DIR := .
endif
ifdef MODEL
ifndef DRV_ROOT
DRV_ROOT := ../../
endif
ifndef UBOOT_LIBRARY
include $(DRV_ROOT)/drv_opt/$(COMPANY)/$(MODEL).def
endif
endif

ifndef DRV_CUSTOM
DRV_CUSTOM := common
endif

FILE_LIST =  drv_dbase.h
FILE_LIST += panel.h
FILE_LIST += panel_cust.h
#FILE_LIST += aud_api.h
FILE_LIST += c_model.h
FILE_LIST += loader_imghdr.h
FILE_LIST += loader_if.h

ifndef DRIVER_DIR_LIST
DIR_LIST = $(sort $(shell if [ -e ../Makefile ]; then make -C ../ get_DRIVER_DIR_LIST --no-print-directory -s 2>/dev/null; else echo ''; fi))

ifdef LINUX_DRV_ROOT
DIR_LIST += loaderlib
else
DIR_LIST += adspbin bsp net smc
endif

else
DIR_LIST := $(DRIVER_DIR_LIST)
endif


FILE_DRV_H_END_LIST =	$(wildcard ../$1/*_drvif.h)	\
			$(wildcard ../$1/*/*_drvif.h)	\
			$(wildcard ../$1/*/*/*_drvif.h)	\
			$(wildcard ../$1/*/*/*/*_drvif.h) \
			$(wildcard ../$1/*/*/*/*/*_drvif.h)

FILE_IF_H_END_LIST =	$(wildcard ../$1/*_if.h)	\
			$(wildcard ../$1/*/*_if.h)	\
			$(wildcard ../$1/*/*/*_if.h)	\
			$(wildcard ../$1/*/*/*/*_if.h)  \
			$(wildcard ../$1/*/*/*/*/*_if.h)

FILE_H_LIST = $(FILE_DRV_H_END_LIST) $(FILE_IF_H_END_LIST)

DRV_INC_FILE_LIST = $(addprefix $(DRV_INC_DIR)/, $(FILE_LIST))
# customized files must put into .PHONY

.PHONY: c_model.h $(DIR_LIST) lib all

all: lib

$(DRV_INC_DIR)/loader_imghdr.h: ../loaderlib/loader_imghdr.h
	$(COPY) $(COPYFLAG) $< $@

$(DRV_INC_DIR)/drv_dbase.h: ../dbs/drv_dbase.h
	$(COPY) $(COPYFLAG) $< $@

$(DRV_INC_DIR)/panel.h: ../drv_cust/panel.h
	$(COPY) $(COPYFLAG) $< $@

$(DRV_INC_DIR)/panel_cust.h: ../drv_cust/$(COMPANY)/panel_cust.h
	$(COPY) $(COPYFLAG) $< $@

$(DRV_INC_DIR)/loader_if.h: ../loaderlib/loader_if.h 
	$(COPY) $(COPYFLAG) $< $@

$(DRV_INC_DIR)/ir_api.h: ../ir/ir_api.h
	$(COPY) $(COPYFLAG) $< $@

$(DRV_INC_DIR)/c_model.h:
	@echo "#ifndef C_MODEL_H" > $@.tmp
	@echo "#define C_MODEL_H" >> $@.tmp
	@echo "/* This is generated by drv_cust/$(subst private_,,$(COMPANY))/config/$(DRV_CUSTOM).h */" >> $@.tmp
	@cat ../drv_cust/$(subst private_,,$(COMPANY))/config/$(DRV_CUSTOM).h | grep -v "#include" >> $@.tmp
	@echo "#endif /* C_MODEL_H */" >> $@.tmp
	@sh ./check_file_update.sh $@.tmp $@
	@rm $@.tmp

$(DIR_LIST):
	@for i in $(call FILE_H_LIST,$@); do \
		sh ./check_file_update.sh $$i $(DRV_INC_DIR)/`basename $$i`; \
	done

lib target: $(DRV_INC_FILE_LIST) $(DIR_LIST)
	@echo "$(DRIVER_DIR_LIST)"

clean:
ifeq "$(BUILD_PRIV)" "true"
	@echo "Don't remove drv_inc/xxx.h"
else
	rm -rf $(DRV_INC_FILE_LIST) $(DRV_INC_DIR)/*_if.h $(DRV_INC_DIR)/*_drvif.h
endif

