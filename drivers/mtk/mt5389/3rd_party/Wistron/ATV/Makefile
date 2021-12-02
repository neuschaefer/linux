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
# $Date: 2009/06/25 $
# $Author: kl-ylwong $
# $CCRevision: /main/DTV_X/DTV_X_HQ_int/6 $
# $SWAuthor: Iolo Tsai $
# $MD5HEX: 41efd1a44dbe52abcafab1c10e444d07 $
#
# Description:
#         Leave-level makefile to build the subcomponent of Middleware library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
endif

include $(PROJECT_ROOT)/path.mak


THIS_COMPONENT = ATV


SRC = 

OBJ =

ifdef SONY_PRIV_FILES
SUB_COMPONENTS = priv_drv
else
SUB_COMPONENTS = app drv priv_drv
endif

OPTIONAL_SUB_COMPONENTS = 


DEFINES +=

CC_INC += -I$(TARGET_ROOT)/3rd_party


#############################################################################

#
# To support executing 'make' in the local subdirectory, we need to determine
# some additional information.  The 'Makefile' is more complicate, but it
# is nicer from the developer's perspective because you can run make while in
# the directory you are working in.
#

#
# Include the target specific compiler tool and their options
#
include $(TARGET_ROOT)/common.mak
