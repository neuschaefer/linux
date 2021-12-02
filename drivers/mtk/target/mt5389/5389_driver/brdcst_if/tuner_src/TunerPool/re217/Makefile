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
#$(THIS_COMPONENT) only work at sys build
THIS_COMPONENT = $(BRDCST_IF)
#$(THIS_COMPONENT_DIR) need for driver build
THIS_COMPONENT_DIR = env57s

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../../../..
endif


SRC = pd_tuner_re217.c pi_tuner_re217.c 

OBJ =

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS =

CC_INC += -I..
CC_INC += -I../../CtrlBus
CC_INC += -I../../TunerDVBT
CC_INC += -I../../TunerATD/common
#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak
include $(DRV_ROOT)/brdcst_if.mak
# $(THIS_OBJ_DIR) only for driver build, sys build doesn't use this parameter
export THIS_OBJ_DIR = $(THIS_OBJ_DIR_TUNER_SRC)/$(BRDCST_IF)/$(DIR_TUNER_DVBT)/$(THIS_COMPONENT_DIR)
