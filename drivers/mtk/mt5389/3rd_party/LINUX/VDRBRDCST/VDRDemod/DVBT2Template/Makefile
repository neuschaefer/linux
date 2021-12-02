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
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

####CONFIG_CHECK_LIST=0,1,2,3,4##########

BUILD_OBJ:=TRUE#FALSE###0 /**must no empty, "TRUE  " or "TRUE"**/
##########tuner_src build config#########
BUILD_INTERNAL_TUNER_SRC:=FALSE   			###1
############SONY 2834 CONFIG#############

DEFINES += -DSONY_DEMOD                 ###2
DEFINES += -DSONY_DEMOD_CRYSTAL_41M			###3
#DEFINES += -DSONY_DEMOD_TSOUTPUT_SERIAL  	###4


#####################no change###########################

ifeq "$(BUILD_INTERNAL_TUNER_SRC)" "TRUE"  ###
DEFINES += -DINTERNAL_3RD_DEMOD
endif

#######################################
ifdef LINUX_DRV_ROOT
$(warning "LINUX system")

else
$(warning "NCLS system")
endif

ifeq "$(BUILD_INTERNAL_TUNER_SRC)" "TRUE"

#$(THIS_COMPONENT) only work at sys build
THIS_COMPONENT = $(BRDCST_IF)
#$(THIS_COMPONENT_DIR) need for driver build
THIS_COMPONENT_DIR = CXD2834

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../../../..
endif

else

ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
endif
-include $(PROJECT_ROOT)/path.mak

endif

THIS_COMPONENT_DIR = CXD2834



SRC_ONLY= pd_glue_dvbt2.c#for some file never use obj format

SRC_FILES = sony_dvb_demod_monitor_PER.c sony_dvb_demod_monitor_SSI.c sony_i2c_MTK.c \
sony_common.c sony_dvb_demod.c sony_dvb_demodT.c sony_dvb_demodT2.c sony_dvb_demodC.c \
sony_i2c.c sony_dvb_demod_monitor.c  sony_dvb_demod_monitorT.c sony_dvb_demod_monitorC.c sony_dvb_demod_monitorT2.c  sony_math.c \
#i2c_api.c   

$(warning "BUILD_obj---$(BUILD_OBJ)")
ifneq "$(BUILD_OBJ)" "TRUE"
SRC =$(SRC_FILES)
SRC +=$(SRC_ONLY)
OBJ=
else
SRC=
SRC +=$(SRC_ONLY)
OBJ = $(patsubst %.c,%.o,$(SRC_FILES))
endif

$(warning "demod SRC----------$(SRC) OBJ---$(OBJ)")
     

SUB_COMPONENTS =
OPTIONAL_SUB_COMPONENTS =

ifeq "$(BUILD_INTERNAL_TUNER_SRC)" "TRUE"
CC_INC += -I../../CtrlBus
CC_INC += -I../../inc
else
ifdef LINUX_DRV_ROOT
CC_INC += -I$(DRV_ROOT)/3rd_party/LINUX/VDRBRDCST/inc
else
CC_INC += -I$(DRV_ROOT)/3rd_party/VDRBRDCST/inc
endif
endif


export DEFINES
$(warning "DM---$(DEFINES)")
#############################################################################
#
# Include the makefile common to all components
#
##############################################################################


include $(DRV_ROOT)/driver.mak
ifeq "$(BUILD_INTERNAL_TUNER_SRC)" "TRUE"
include $(DRV_ROOT)/brdcst_if.mak
endif