##############################################################################
# No Warranty                                                                #
# Except as may be otherwise agreed to in writing, no warranties of any      #
# kind, whether express or implied, are given by MTK with respect to any MTK #
# Deliverables or any use thereof, and MTK Deliverables are provided on an   #
# "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        #
# including any implied warranties of merchantability, non-infringement and  #
# fitness for a particular purpose and any warranties arising out of course  #
# of performance, course of dealing or usage of trade.  Parties further      #
# acknowledge that Company may, either presently and/or in the future,       #
# instruct MTK to assist it in the development and the implementation, in    #
# accordance with Company's designs, of certain softwares relating to        #
# Company's product(s) (the "Services").  Except as may be otherwise agreed  #
# to in writing, no warranties of any kind, whether express or implied, are  #
# given by MTK with respect to the Services provided, and the Services are   #
# provided on an "AS IS" basis.  Company further acknowledges that the       #
# Services may contain errors, that testing is important and Company is      #
# solely responsible for fully testing the Services and/or derivatives       #
# thereof before they are used, sublicensed or distributed.  Should there be #
# any third party action brought against MTK, arising out of or relating to  #
# the Services, Company agree to fully indemnify and hold MTK harmless.      #
# If the parties mutually agree to enter into or continue a business         #
# relationship or other arrangement, the terms and conditions set forth      #
# hereunder shall remain effective and, unless explicitly stated otherwise,  #
# shall prevail in the event of a conflict in the terms in any agreements    #
# entered into between the parties.                                          #
##############################################################################
###########################################################################
#  Copyright (c) 2005, MediaTek, Inc.
#  All rights reserved
#
#  Unauthorized use, practice, perform, copy, distribution, reproduction,
#  or disclosure of this information in whole or in part is prohibited.
###########################################################################
# $RCSfile: Makefile,v $
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################
####CONFIG_CHECK_LIST=0,1,2##########

BUILD_OBJ:=TRUE#FALSE###0 /**must no empty, "TRUE  " or "TRUE"**/
##########tuner_src build config#########
BUILD_INTERNAL_TUNER_SRC:=FALSE    			###1
############SONY RE215 CONFIG#############

DEFINES += -DSONY_DEMOD									###2


#####################no change###########################
ifeq "$(BUILD_INTERNAL_TUNER_SRC)" "TRUE"  ###
DEFINES += -DINTERNAL_3RD_DEMOD
endif

ifeq "$(BUILD_INTERNAL_TUNER_SRC)" "TRUE"  ###

#$(THIS_COMPONENT) only work at sys build
THIS_COMPONENT = $(BRDCST_IF)
#$(THIS_COMPONENT_DIR) need for driver build


ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../../../..
endif

else ###

ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
endif

-include $(PROJECT_ROOT)/path.mak
endif ###

THIS_COMPONENT_DIR = sonyre215
#cust_tuner_interface.c
$(warning "tuner SRC----------$(SRC)")

SRC_ONLY= pd_tuner_re215.c pi_tuner_re215.c #for some file never use obj format

SRC_FILES = re215_i2c_MTK.c re215_ascot2d.c re215_i2c.c


ifneq "$(BUILD_OBJ)" "TRUE"
SRC =$(SRC_FILES)
SRC +=$(SRC_ONLY)
OBJ=
else
SRC=
SRC +=$(SRC_ONLY)
OBJ = $(patsubst %.c,%.o,$(SRC_FILES))
endif

$(warning "tuner SRC---$(SRC) OBJ---$(OBJ)")
SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS =


ifeq "$(BUILD_INTERNAL_TUNER_SRC)" "TRUE"  ###
CC_INC += -I..
CC_INC += -I../../CtrlBus
CC_INC += -I../../TunerDVBT
CC_INC += -I../../TunerATD/common
else ###
ifdef LINUX_DRV_ROOT  ### linux
$(warning "LINUX_SYSTEM include")
CC_INC += -I$(DRV_ROOT)/3rd_party/LINUX/VDRBRDCST/inc
else
$(warning "NCLS_SYSTEM include")
CC_INC += -I$(DRV_ROOT)/3rd_party/VDRBRDCST/inc
endif ###

endif ###

export DEFINES
#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak
ifeq "$(BUILD_INTERNAL_TUNER_SRC)" "TRUE"  ###
include $(DRV_ROOT)/brdcst_if.mak
endif ###
