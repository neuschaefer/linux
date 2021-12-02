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
ifndef DRV_ROOT
DRV_ROOT = ..
endif
OBJDIR = $(DRV_ROOT)/trustzone/obj

IC_NAME=5860


# toolchain
#RVCT_ROOT = /mtktools/ARM/RVDS40/RVCT
#ARM_BIN=$(RVCT_ROOT)/Programs/4.0/400/linux-pentium/
#ARM_INC=$(RVCT_ROOT)/Data/4.0/400/include/unix/
#ARM_LIB_PATH=$(RVCT_ROOT)/Data/4.0/400/lib
#export ARMLMD_LICENSE_FILE=$ARMLMD_LICENSE_FILE:8224@10.10.5.206

RVCT_ROOT = /mtktools/ARM/RVDS3.1/RVCT
ARM_BIN=$(RVCT_ROOT)/Programs/3.1/761/linux-pentium/
ARM_INC=$(RVCT_ROOT)/Data/3.1/761/include/unix/
ARM_LIB_PATH=$(RVCT_ROOT)/Data/3.1/761/lib

ASM = ${ARM_BIN}armasm
CC = ${ARM_BIN}armcc
LD = ${ARM_BIN}armlink
AR = ${ARM_BIN}armar
FROMELF = ${ARM_BIN}fromelf

CC_FLAG = --littleend -g -O2 -W --dwarf2 --no_unaligned_access -DCC_MT$(IC_NAME) -DNDEBUG -D_CPU_LITTLE_ENDIAN_ -DCC_SECURE_WORLD
CC_FLAG += -DCC_ENABLE_L2C
CC_FLAG += -DCC_CA9
ASM_FLAG = -g --dwarf2 --pd "CC_MT$(IC_NAME) SETL {TRUE}" --diag_suppress A1786W
# cpu option
#CC_FLAG += --cpu Cortex-A9.no_neon
#ASM_FLAG += --cpu=Cortex-A9.no_neon
CC_FLAG += --cpu ARM1176JZF-S
ASM_FLAG += --cpu=ARM1176JZF-S

CC_INC = -I. -I./inc -I$(ARM_INC) \
-I$(DRV_ROOT)/$(IC_NAME)_driver/inc \
-I$(DRV_ROOT)/$(IC_NAME)_driver/drv_cust \
-I$(DRV_ROOT)/$(IC_NAME)_driver/drv_inc \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv8098/inc/hw \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv8098/inc/sys \
-I$(DRV_ROOT)/$(IC_NAME)_driver/bsp \
-I$(DRV_ROOT)/$(IC_NAME)_driver/bsp/dram \
-I$(DRV_ROOT)/$(IC_NAME)_driver/tz \
-I$(DRV_ROOT)/../../middleware \
-I$(DRV_ROOT)/../../middleware/os/inc/


TARGET = mt$(IC_NAME)_tz.axf

LD_FLAG = --entry INT_Reset --scatter ./scatter.sc --libpath $(ARM_LIB_PATH) \
		--xref --map --info sizes --verbose --list=$(subst axf,lst,$(TARGET)) \
		--locals --symdefs $(subst axf,sym,$(TARGET)) --diag_suppress=L6312W

C_SRC = \
trustzone_main.c \
board/tgt_platform_init.c \
board/cpu_cache.c \
board/tgt_boot_pic.c \
manager/tzst_service_manager.c \
common/common.c \
common/printf.c \
services/aes.c \
services/svc_registry.c \
services/dmx_descrambler.c \
services/svc_dmx.c \
services/svc_example.c \
services/svc_drm.c \
services/svc_tzctl.c \
bsp/arm/hal_ca9.c \
bsp/arm/bootp.c

A_SRC = \
board/tgt_vectors.s \
board/tgt_boot_init.s \
manager/tzst_service_api.s \
monitor/monitor.s \
monitor/mon_switch.s \
monitor/mon_vectors.s \
monitor/mon_macros.s \
monitor/mon_init.s


OBJ = $(A_SRC:.s=.o) $(C_SRC:.c=.o)

.PHONY : all bin symlink clean

all: symlink bin
	@echo "build OK"

symlink:
	@if [ ! -e bsp ]; then \
		(ln -s ../../mt$(IC_NAME)/$(IC_NAME)_driver/bsp .; exit 0); \
	fi

bin: $(TARGET)
	@$(FROMELF) --bin $(TARGET) --output $(subst axf,bin,$(TARGET))

$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	$(CC) -o trustzone.scatx $(CC_FLAG) $(CC_INC) $(DEFINES) -E trustzone.scat
	@awk '/ROM_LOAD/,p' trustzone.scatx > scatter.sc
	$(LD) $(LD_FLAG) $(addprefix $(OBJDIR)/,$(OBJ)) -o $(TARGET)

%.o :: %.c
	@echo "--- Building `basename $<` ---"
	@if [ ! -d $(OBJDIR)/`dirname $@` ] ; then \
		mkdir -p $(OBJDIR)/`dirname $@`; \
	fi
	$(CC) -c $(CC_FLAG) $(CC_INC) $(DEFINES) -o $(OBJDIR)/$@ $<

%.o :: %.s
	@echo "--- Building `basename $<` ---"
	@if [ ! -d $(OBJDIR)/`dirname $@` ] ; then \
		mkdir -p $(OBJDIR)/`dirname $@`; \
	fi
	$(ASM) $(ASM_FLAG) $(ASMDEFINES) -o $(OBJDIR)/$@ $<
	@if [ -e $@ ] ; then \
		@mv $@ .; \
	fi


clean:
	rm -rf $(TARGET) *.axf *.bin $(OBJDIR) *.bak *.lst *.sym trustzone.scatx scatter.sc
	rm -f `find -type l`

