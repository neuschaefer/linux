###########################################################################
#
#  Copyright (c) 2004, MediaTek, Inc.
#  All rights reserved
#
#  Unauthorized use, practice, perform, copy, distribution, reproduction,
#  or disclosure of this information in whole or in part is prohibited.
###########################################################################
# $RCSfile: Makefile,v $
# $Revision: #1 $
# $Date: 2009/01/14 $
# $Author: ytchen $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

THIS_COMPONENT = lzma

export DRV_ROOT = ../..

SRC = Alloc.c LzFind.c LzmaDec.c LzmaLib.c

ASM_SRC =

OBJ =

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS =

DEFINES += -DCC_NO_LZMA_ENCODE

CC_INC +=

ifeq "$(BUILD_LINUX_LOADER)" "true"
ARM_FLAG = -marm
else
ARM_FLAG = --arm
endif

#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak
