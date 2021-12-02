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
#  Copyright (c) 2004, MediaTek, Inc.
#  All rights reserved
#
#  Unauthorized use, practice, perform, copy, distribution, reproduction,
#  or disclosure of this information in whole or in part is prohibited.
###########################################################################
# $RCSfile: Makefile,v $
# $Revision: #1 $
# $Date: 2009/02/23 $
# $Author: eddiehuang $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

THIS_COMPONENT = mtk_skb

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../..
endif

SRC = SkbGenericCipher.c SkbGenericEngine.c SkbGenericSecureData.c SkbGenericTransform.c tomcrypt_asn1_missing.c

SUB_COMPONENTS =

#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak
