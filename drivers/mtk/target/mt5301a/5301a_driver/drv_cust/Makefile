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
# $Revision: #12 $
# $Date: 2009/09/22 $
# $Author: jacky.ko $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

THIS_COMPONENT = drv_cust

ifeq "$(UBOOT_LIBRARY)" "y"

ifeq "$(UBOOT_NEW_LIBRARY)" "y"
include $(ROOTDIR)/config.mak
CPPFLAGS += -Wa,-gstabs -g -Os -D__KERNEL__ -Wstrict-prototypes -DCC_MTK_LOADER
CFLAGS += -Wa,-gstabs -g -Os -D__KERNEL__ -Wstrict-prototypes -DCC_MTK_LOADER
else
include $(TOPDIR)/config.mk
endif

CPPFLAGS += -I$(TOPDIR)/board/$(BOARDDIR)/drv_lib/drv_inc -I$(TOPDIR)/board/$(BOARDDIR)/drv_lib/inc
CFLAGS += -I$(TOPDIR)/board/$(BOARDDIR)/drv_lib/drv_inc -I$(TOPDIR)/board/$(BOARDDIR)/drv_lib/inc
CPPFLAGS += -I$(TOPDIR)/board/$(BOARDDIR)/include -I$(OSAI_INC) -I$(COMMON_INC)
CFLAGS += -I$(TOPDIR)/board/$(BOARDDIR)/include -I$(OSAI_INC) -I$(COMMON_INC)

ifeq ($(PRELOADER_DEF),CC_MTK_PRELOADER)
CPPFLAGS += -DCC_MTK_PRELOADER
CFLAGS += -DCC_MTK_PRELOADER
endif

LIB	= lib$(THIS_COMPONENT).a


OBJS	:= drvcust_if.o drvcust_init.o panel_table.o drvcust_lib.o

SOBJS	:= 

$(LIB):	$(OBJS) $(SOBJS)
	$(AR) crv $@ $^

clean:
	rm -f $(SOBJS) $(OBJS)

distclean:	clean
	rm -f $(LIB) core *.bak .depend

#########################################################################

.depend:	Makefile $(SOBJS:.o=.S) $(OBJS:.o=.c)
		$(CC) -M $(CPPFLAGS) $(SOBJS:.o=.S) $(OBJS:.o=.c) > $@

-include .depend

else # UBOOT_LIBRARY

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../..
endif

SRC = drvcust_if.c drvcust_init.c panel_table.c drvcust_lib.c

OBJ =

ifeq "$(DRV_CUSTOM)" ""
DRV_CUSTOM = common
endif

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS =

# For customization function
# DEFINES += -DCUSTOMIZE_QRYFUNC=$(DRV_CUSTOM)_qry -DCUSTOMIZE_INITQRYFUNC=$(DRV_CUSTOM)_initqry
# SUB_COMPONENTS += $(DRV_CUSTOM)

# For config file
DEFINES += -DCUSTOM_CFG_FILE=\"$(subst private_,,$(COMPANY))/config/$(DRV_CUSTOM).h\"
DEFINES += -DPANEL_TABLE_CUST_FILE=\"$(subst private_,,$(COMPANY))/panel_table_cust.c\"

# For b.version command
DEFINES += -DDRV_CUSTOM=\"$(DRV_CUSTOM)\"


# For memory configuration
# DEFINES += -DFBM_MEM_CFG=FBM_MEM_CFG_MT$(FBM_MEM_CFG)

ifeq "$(BRDCST_IF)" ""
DEFINES += -DBRDCST_IF=\"????\"
else
DEFINES += -DBRDCST_IF=\"$(BRDCST_IF)\"
endif

# export TARGET_DRV_INC := -I.

export NO_LINT = y
ADD_TO_SYMLINK_DIRS = mtk

.PHONY : touch

#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak
include $(DRV_ROOT)/nptv.mak

CC_INC += -I$(DRV_ROOT)/3rd_party/$(COMPANY)/d_inc
CC_INC += -I$(DRV_ROOT)/3rd_party/$(COMPANY)
CC_INC += -I$(DRV_ROOT)/d_inc/$(COMPANY)

touch:
	echo "DRV_CUSTOM is $(DRV_CUSTOM)"
#	touch *.c
	
all: touch
lib: touch
target: touch



endif # UBOOT_LIBRARY

