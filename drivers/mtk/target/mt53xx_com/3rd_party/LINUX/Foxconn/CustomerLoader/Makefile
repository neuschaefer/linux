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
#  
#  Copyright (c) 2004, CrystalMedia Technology, Inc.
#  All rights reserved 
#
#  Unauthorized use, practice, perform, copy, distribution, reproduction,
#  or disclosure of this information in whole or in part is prohibited.
###########################################################################
# $RCSfile: Makefile,v $
# $Revision: #2 $
# $Date: 2008/05/01 $
# $Author: dwu $
# $CCRevision: /main/DTV_X_ATSC/3 $
# $SWAuthor: Iolo Tsai $
# $MD5HEX: 2a4bf594c46e7fdf5fedba9d01fb6f2d $
#
# Description:
#         Leave-level makefile to build the subcomponent of Middleware library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
endif

-include $(PROJECT_ROOT)/path.mak

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT =
endif

DRV_ROOT := $(shell if [ -e $(DRV_ROOT)/driver.mak ]; then echo "$(DRV_ROOT)"; else echo "$(PROJECT_ROOT)/target/$(TARGET_IC)"; fi)

THIS_COMPONENT = CustomerLoader

ifeq "$(3RD_PARTIES_MODEL)" "Figaro_EU_PAD"
SUB_COMPONENTS = $(3RD_PARTIES_MODEL)
else
SUB_COMPONENTS = Common
endif
#$(warning ==================================================$(DEFINES))
SRC = GpioCtrl.c UsbUpgrade.c LedCtrl.c

OBJ =

OPTIONAL_SUB_COMPONENTS = 

DEFINES +=

CC_INC += -I$(DRV_ROOT)/3rd_party/LINUX/$(COMPANY)/d_inc \
          -I$(DRV_ROOT)/$(TARGET_DRIVER)/drv_cust/$(COMPANY) \
          -I$(DRV_ROOT)/$(TARGET_DRIVER)/drv_cust \
          -I$(DRV_ROOT)/$(TARGET_DRIVER)/aud/periph

#############################################################################
#
# Include the makefile common to all components
# 
include $(DRV_ROOT)/driver.mak
