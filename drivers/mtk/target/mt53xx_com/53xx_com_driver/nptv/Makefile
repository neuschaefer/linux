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
# $Revision: #3 $
# $Date: 2009/03/21 $
# $Author: adam.yang $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

THIS_COMPONENT = nptv

ifeq "$(UBOOT_LIBRARY)" "y"

include $(TOPDIR)/config.mk

CPPFLAGS += -I$(TOPDIR)/board/$(BOARDDIR)/drv_lib/drv_inc -I$(TOPDIR)/board/$(BOARDDIR)/drv_lib/inc
CFLAGS += -I$(TOPDIR)/board/$(BOARDDIR)/drv_lib/drv_inc -I$(TOPDIR)/board/$(BOARDDIR)/drv_lib/inc
CPPFLAGS += -I$(TOPDIR)/board/$(BOARDDIR)/include -I$(OSAI_INC)
CFLAGS += -I$(TOPDIR)/board/$(BOARDDIR)/include -I$(OSAI_INC)

LIB	= lib$(THIS_COMPONENT).a


OBJS	:= api/api_backlt.o ostg/drv_display.o ostg/drv_lvds.o io/drv_dram.o io/drv_pwm.o sys/hw_vdoin.o

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
ifeq ("$(DRV_ROOT)", "..")
export DRV_ROOT = ../..
else
export DRV_ROOT ?= ../..
endif
endif


SRC =

OBJ =

SUB_COMPONENTS = api drv sys nlibs


ADD_TO_SYMLINK_DIRS = linux26

# SUB_COMPONENTS = \
	bsp util osdrv dbs share_lib \
	gfx osd nim \
	dmx fbm psr mpv vdp pmx tve \
	pod fci ide nor ir sif nand aud \
	uart crypto cli debug \
	vin mw_if i1394

# OPT_COMPONENTS = \
	dhrystone t32 fnt jpg pvr tsfs slt

# i1394

OPTIONAL_SUB_COMPONENTS =


DEFINES += -DCC_SCPOS_MUTE_MASK 
#-DCC_CLI -DCC_SCPOS_SW_ISR

CC_INC +=

#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak



xfer_linux:
	# filename change for makefile
	if [ -f ./src/makefile ] ; then mv ./src/makefile ./src/Makefile ;fi
	if [ -f  ./makefile ] ; then mv ./makefile ./makefile.nuclues ; fi
	dos2unix src/driver/PWM/Makefile
	dos2unix src/service/BACKLT/Makefile
	dos2unix src/service/TUNER/Makefile
	dos2unix src/service/TUNER/UniTV/Makefile
	for i in `find . -name Makefile`; do \
		 sed 's/export/#L@export/g' $$i > $$i.bak; \
		 mv $$i.bak $$i ;\
		 done

	ln -s linux26/nptv.mak .
	ln -s linux26/driver.mak .
	cp -f linux26/makefile.linux Makefile
	#

check:
	@echo $(DRV_ROOT)

endif # UBOOT_LIBRARY
