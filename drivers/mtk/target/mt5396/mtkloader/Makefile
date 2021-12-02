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
DRV_ROOT = ../../$(TARGET)

#FROMELF = fromelf.761
#inlcude target.mak only for armcc fromelf path
ifneq "$(BUILD_LINUX_LOADER)" "true"
include ../target.mak
endif

OBJCOPY = $(CROSS_COMPILE)objcopy

ifndef TOOL_ROOT

ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
include $(PROJECT_ROOT)/path.mak
endif
endif


PACKPROG = packprog
ifeq ($(LZMA_DECODE), true)
LZHSENC = $(TOOL_ROOT)/mt5391_pack/lzma
else
LZHSENC = $(TOOL_ROOT)/mt5391_pack/lzhsenc
endif

T_MTK_15K = $(TOOL_ROOT)/mt5391_pack/t_mtk_15k
T_MTK = $(TOOL_ROOT)/mt5391_pack/t_mtk
PACKIMG = $(TOOL_ROOT)/mt5391_pack/packimg
SCATTER = $(TOOL_ROOT)/mt5391_pack/40KB.sc

-include $(CFG_FILE_DIR)/cfg.mak

CC_PACK =

ifeq "$(BOOT_TYPE)" "ROM2NOR"
DEFINES += -DCC_ROM_BOOT
endif
ifeq "$(BOOT_TYPE)" "ROM2NAND"
DEFINES += -DCC_ROM_BOOT
endif
ifeq "$(BOOT_TYPE)" "ROM2EMMC"
DEFINES += -DCC_ROM_BOOT
endif
ifeq "$(SECURE)" "ROM"
DEFINES += -DCC_SECURE_BOOT
endif
ifeq "$(SECURE)" "ALL"
DEFINES += -DCC_SECURE_BOOT
DEFINES += -DCC_SECURE_BOOT_ALL
endif

ifeq "$(SECURE_CUSTKEY)" "y"
DEFINES += -DCC_SECURE_BOOT_CHECK_CUSTKEY
endif

SIGGEN_KEY := $(shell if [ -e $(THIS_ROOT)/data/public_key -a -e $(THIS_ROOT)/data/private_key ]; then echo "$(THIS_ROOT)/data/public_key $(THIS_ROOT)/data/private_key"; else echo "";fi)

ifeq "$(CUST_CLR_IMG)" "true"

CUST_CLR_ROOT      = $(THIS_ROOT)/secure_rls
CUST_CLR_TOOL_PATH = $(CUST_CLR_ROOT)/tools


ifdef BUILD_CFG
CUST_CLR_PATH = $(CUST_CLR_ROOT)/cli/bin
else
CUST_CLR_PATH = $(CUST_CLR_ROOT)/rel/bin
endif
endif

.PHONY:	all preloader loader packprog packing

#all: preloader loader packprog packing
all: packing

preloader:
	$(MAKE) -C $(DRV_ROOT)/preloader EXE_NAME=$(EXE_NAME)
ifeq "$(BUILD_LINUX_LOADER)" "true"
	$(OBJCOPY) ${OBJCFLAGS} -O binary $(PRELOADER_BIN_DIR)/$(EXE_NAME) $(MTKLOADER_BIN_DIR)/$(EXE_NAME)_preloader.bin
else
	$(FROMELF) --bin $(DRV_ROOT)/preloader/$(EXE_NAME) --output $(EXE_NAME)_preloader.bin
endif
ifdef SECURE
	chmod +x ${T_MTK}
	chmod +x ${T_MTK_15K}
	chmod +x ${PACKIMG}
ifeq "$(CUST_CLR_IMG)" "true"
	cp -f $(T_MTK_15K) $(CUST_CLR_TOOL_PATH)
	cp -f $(MTKLOADER_BIN_DIR)/$(EXE_NAME)_preloader.bin $(CUST_CLR_PATH)/preloader.bin
	cp -f $(PACKIMG) $(CUST_CLR_TOOL_PATH)
	cp -f ${SCATTER} $(CUST_CLR_TOOL_PATH)
	cp -f Register_Script $(CUST_CLR_ROOT)
endif
ifeq "$(SECURE_CUSTKEY)" "y"
	cd $(MTKLOADER_BIN_DIR);  \
	${T_MTK_15K} $(EXE_NAME)_preloader.bin 0 0 15360 $(SIGGEN_KEY)
else
	cd $(MTKLOADER_BIN_DIR); \
	${T_MTK_15K} $(EXE_NAME)_preloader.bin 0
endif
	@if [ ! -e $(MTKLOADER_BIN_DIR)/Register_Script ]; then \
		ln -sf $(PROJECT_ROOT)/target/$(TARGET_IC)/mtkloader/Register_Script $(MTKLOADER_BIN_DIR)/Register_Script ; \
	fi
	cd $(MTKLOADER_BIN_DIR); \
	${PACKIMG} -f ${SCATTER} ; \
	mv $(EXE_NAME)_preloader.bin $(EXE_NAME)_preloader.orig.bin ; \
	mv 16KB.bin $(EXE_NAME)_preloader.bin 
endif

loader: preloader
	$(MAKE) -C $(DRV_ROOT)/loader EXE_NAME=$(EXE_NAME)
ifeq "$(BUILD_LINUX_LOADER)" "true"
	$(OBJCOPY) ${OBJCFLAGS} -O binary $(LOADER_BIN_DIR)/$(EXE_NAME)  $(MTKLOADER_BIN_DIR)/$(EXE_NAME)_loader.bin
else
	$(FROMELF) --bin $(DRV_ROOT)/loader/$(EXE_NAME) --output $(EXE_NAME)_loader.bin
endif
	#rm -f $(DRV_ROOT)/loader/$(EXE_NAME)
	chmod +x $(LZHSENC)
	cd $(MTKLOADER_BIN_DIR); \
	$(LZHSENC) $(EXE_NAME)_loader.bin $(EXE_NAME)_loader.bin.lzhs
ifdef SECURE
	chmod +x ${T_MTK}
	chmod +x ${T_MTK_15K}
	chmod +x ${PACKIMG}
ifeq "$(CUST_CLR_IMG)" "true"
	cp -f $(MTKLOADER_BIN_DIR)/$(EXE_NAME)_loader.bin.lzhs $(CUST_CLR_PATH)/loader.bin.lzhs
	cp -f ${T_MTK} $(CUST_CLR_TOOL_PATH)
endif
ifeq "$(SECURE_CUSTKEY)" "y"
	cd $(MTKLOADER_BIN_DIR) ; \
	${T_MTK} $(EXE_NAME)_loader.bin.lzhs 0 2 0 $(SIGGEN_KEY)
else
	cd $(MTKLOADER_BIN_DIR); \
	${T_MTK} $(EXE_NAME)_loader.bin.lzhs 0 1
endif
	cd $(MTKLOADER_BIN_DIR) ; \
	rm -rf 14KB_preloader.bin Code_Signature Customer_Key_Encrypt_Data Customer_Key_Signature SHA1_Signature ; \
	rm -rf MTK_PRIVATE_KEY_0 MTK_PUBLIC_KEY_0 VENDOR_PRIVATE_KEY_0 VENDOR_PUBLIC_KEY_0
endif

$(PACKPROG): loader
	@echo "BOOT_TYPE: $(BOOT_TYPE)"
	@echo "SECURE: $(SECURE)"
	gcc -m32 -g -I$(DRV_ROOT)/$(subst mt,,$(TARGET))_driver/loaderlib -o $(MTKLOADER_BIN_DIR)/$(PACKPROG) $(DEFINES) -DCC_MT$(IC_NAME) packprog.c
	chmod +x $(MTKLOADER_BIN_DIR)/$(PACKPROG)

packing: packprog
ifeq "$(CUST_CLR_IMG)" "true"
	cp -f $(MTKLOADER_BIN_DIR)/$(PACKPROG)  $(CUST_CLR_TOOL_PATH)
endif
	cd $(MTKLOADER_BIN_DIR); \
	./$(PACKPROG) $(EXE_NAME)_preloader.bin $(EXE_NAME)_loader.bin.lzhs ./$(EXE_NAME)_mtkloader.bin

clean:
	$(MAKE) -C $(DRV_ROOT)/preloader $@
	$(MAKE) -C $(DRV_ROOT)/loader $@
	rm -f *.bin *.lzhs packprog

