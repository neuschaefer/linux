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
##############################################################################
# $RCSfile: Makefile,v $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
##############################################################################
#$(THIS_COMPONENT) only work at sys build
THIS_COMPONENT = $(BRDCST_IF)
#$(THIS_COMPONENT_DIR) need for driver build
THIS_COMPONENT_DIR = TunerPool

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../../..
endif


SRC = tuner_interface.c
#if customized tuner,build Cust_tuner_registration.c
ifeq "$(findstring VDRTuner,$(BRDCST_IF))" "VDRTuner"
SRC+=Cust_tuner_registration.c 
else
SRC+=tuner_registration.c 
endif

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS =

CC_INC += -I../CtrlBus
DEFINES += -DPFXMSG=ITuner
#to avoid BRDCST_IF  is undefined
ifneq "$(BRDCST_IF)" ""
BRDCST_IF_MAJOR := $(word 1, $(subst _P_, P_, $(BRDCST_IF)))
#BRDCST_IF_TUNER := $(word $(words $(subst _, , $(BRDCST_IF_MAJOR))) , $(subst _, , $(BRDCST_IF_MAJOR)))
BRDCST_IF_TUNER := $(word $(words $(subst _, , $(BRDCST_IF_MAJOR))) , $(subst _, , $(BRDCST_IF_MAJOR)))
else
BRDCST_IF_MAJOR :=
BRDCST_IF_TUNER :=
endif
############################5387DTD###############
ifeq "$(BRDCST_IF_TUNER)" "TDAU4XB02A"
OPTIONAL_SUB_COMPONENTS += tdau4xb02a
endif
ifeq "$(BRDCST_IF_TUNER)" "TDQU8X001A"
OPTIONAL_SUB_COMPONENTS += tdqu8x001a
endif
ifeq "$(BRDCST_IF_TUNER)" "TDVWH810F"
OPTIONAL_SUB_COMPONENTS += tdvwh810f
endif
ifeq "$(BRDCST_IF_TUNER)" "VA4A1FB5010"
OPTIONAL_SUB_COMPONENTS += va4a1fb5010
endif
ifeq "$(BRDCST_IF_TUNER)" "TD1636"
OPTIONAL_SUB_COMPONENTS += td1636
endif
ifeq "$(BRDCST_IF_TUNER)" "TEQE3"
OPTIONAL_SUB_COMPONENTS += teqe3
endif
ifeq "$(BRDCST_IF_TUNER)" "TEQE3SUN"
OPTIONAL_SUB_COMPONENTS += teqe3
endif
ifeq "$(BRDCST_IF_TUNER)" "TDAU4D01A"
OPTIONAL_SUB_COMPONENTS += tdau4d01a
endif
ifeq "$(BRDCST_IF_TUNER)" "TEQH3L01A"
OPTIONAL_SUB_COMPONENTS += teqh3l01a
endif
ifeq "$(BRDCST_IF_TUNER)" "TEQH3L01ASUN"
OPTIONAL_SUB_COMPONENTS += teqh3l01a
endif
ifeq "$(BRDCST_IF_TUNER)" "EMCSLST"
OPTIONAL_SUB_COMPONENTS += teqe3 tdau4d01a teqh3l01a
endif
ifeq "$(BRDCST_IF_TUNER)" "TDAU4D02A"
OPTIONAL_SUB_COMPONENTS += tdau4d01a
endif
ifeq "$(BRDCST_IF_TUNER)" "ENV56S"
OPTIONAL_SUB_COMPONENTS += env56s
endif
ifeq "$(BRDCST_IF_TUNER)" "DA58CT"
OPTIONAL_SUB_COMPONENTS += da58ct
endif
ifeq "$(BRDCST_IF_TUNER)" "DTVA50CVH"
OPTIONAL_SUB_COMPONENTS += dtva50cvh
endif

ifeq "$(BRDCST_IF_TUNER)" "VADTLST"
OPTIONAL_SUB_COMPONENTS += dtva50cvh va1e1bf2401
endif

ifeq "$(BRDCST_IF_TUNER)" "FA2307A"
OPTIONAL_SUB_COMPONENTS += fa2307a
endif
#ifeq "$(BRDCST_IF_TUNER)" "MT2063"
#OPTIONAL_SUB_COMPONENTS += mt2063
#endif

ifeq "$(BRDCST_IF_TUNER)" "VA1G5BF"
OPTIONAL_SUB_COMPONENTS += va1g5bf
endif
ifeq "$(BRDCST_IF_TUNER)" "UV1336"
OPTIONAL_SUB_COMPONENTS += uv1336
endif
ifeq "$(BRDCST_IF_TUNER)" "DTVS203EH"
OPTIONAL_SUB_COMPONENTS += dtvs203eh
endif
ifeq "$(BRDCST_IF_TUNER)" "DTOS40A"
OPTIONAL_SUB_COMPONENTS += dtos40a
endif
ifeq "$(BRDCST_IF_TUNER)" "UV1356E"
OPTIONAL_SUB_COMPONENTS += uv1356e
endif
ifeq "$(BRDCST_IF_TUNER)" "UV1316E"
OPTIONAL_SUB_COMPONENTS += uv1316e
endif
ifeq "$(BRDCST_IF_TUNER)" "TDVRH751F"
OPTIONAL_SUB_COMPONENTS += tdvrh751f
endif
ifeq "$(BRDCST_IF_TUNER)" "TDTGG902D"
OPTIONAL_SUB_COMPONENTS += tdtgg902d
endif
ifeq "$(BRDCST_IF_TUNER)" "TDAG4B03A"
OPTIONAL_SUB_COMPONENTS += tdag4b03a
endif
ifeq "$(BRDCST_IF_TUNER)" "TDTWS710D"
OPTIONAL_SUB_COMPONENTS += tdtws710d
endif

ifeq "$(BRDCST_IF_TUNER)" "TAFTZ716D"
OPTIONAL_SUB_COMPONENTS += taftz716d
endif

ifeq "$(BRDCST_IF_TUNER)" "FT2101"
OPTIONAL_SUB_COMPONENTS += ft2101
endif
ifeq "$(BRDCST_IF_TUNER)" "OM3869"
OPTIONAL_SUB_COMPONENTS += om3869
endif
ifeq "$(BRDCST_IF_TUNER)" "ENV57S"
OPTIONAL_SUB_COMPONENTS += env57s
endif
ifeq "$(BRDCST_IF_TUNER)" "FT2607"
OPTIONAL_SUB_COMPONENTS += ft2607
endif
ifeq "$(BRDCST_IF_TUNER)" "TDTWS810D"
OPTIONAL_SUB_COMPONENTS += tdtws810d
endif
ifeq "$(BRDCST_IF_TUNER)" "TH2603"
OPTIONAL_SUB_COMPONENTS += th2603
endif

ifeq "$(BRDCST_IF_TUNER)" "ENV57U03D5F"
OPTIONAL_SUB_COMPONENTS += env57u03d5f
endif

ifeq "$(BRDCST_IF_TUNER)" "ENV57U09D5F"
OPTIONAL_SUB_COMPONENTS += env57u09d5f
endif

ifeq "$(BRDCST_IF_TUNER)" "RE201"
OPTIONAL_SUB_COMPONENTS += re201
endif

ifeq "$(BRDCST_IF_TUNER)" "RE205"
OPTIONAL_SUB_COMPONENTS += re205
endif

ifeq "$(BRDCST_IF_TUNER)" "RE217"
OPTIONAL_SUB_COMPONENTS += re217
ifeq "$(findstring DUMMY, $(BRDCST_IF))" "DUMMY"
DEFINES += -DCC_DUMMY_RE217
endif
endif

ifeq "$(BRDCST_IF_TUNER)" "ENV57U04D8F"
OPTIONAL_SUB_COMPONENTS += env57u04d8f
endif
ifeq "$(BRDCST_IF_TUNER)" "TDFRC151D"
OPTIONAL_SUB_COMPONENTS += tdfrc151d
endif


ifeq "$(BRDCST_IF_TUNER)" "TDAG8"
OPTIONAL_SUB_COMPONENTS += tdag8
endif
ifeq "$(BRDCST_IF_TUNER)" "DT70WI"
OPTIONAL_SUB_COMPONENTS += dt70wi
endif
ifeq "$(BRDCST_IF_TUNER)" "DTT8D1C"
OPTIONAL_SUB_COMPONENTS += dtt8d1c
endif
ifeq "$(BRDCST_IF_TUNER)" "F05WT"
OPTIONAL_SUB_COMPONENTS += f05wt
endif
ifeq "$(BRDCST_IF_TUNER)" "ENV5SDF"
OPTIONAL_SUB_COMPONENTS += env5sdf
endif
ifeq "$(BRDCST_IF_TUNER)" "F20WT"
OPTIONAL_SUB_COMPONENTS += f20wt
endif
ifeq "$(BRDCST_IF_TUNER)" "HFT2"
OPTIONAL_SUB_COMPONENTS += HFT2
endif
ifeq "$(BRDCST_IF_TUNER)" "TAF6"
OPTIONAL_SUB_COMPONENTS += TAF6
endif
ifeq "$(BRDCST_IF_TUNER)" "FTF6F"
OPTIONAL_SUB_COMPONENTS += FTF6F
endif
ifeq "$(BRDCST_IF_TUNER)" "SYSP002"
OPTIONAL_SUB_COMPONENTS += sysp002
endif

ifeq "$(BRDCST_IF_TUNER)" "TDYU4D01A"
OPTIONAL_SUB_COMPONENTS += tdyu4d01a
endif
ifeq "$(BRDCST_IF_TUNER)" "LC9LST"
OPTIONAL_SUB_COMPONENTS += sysp002 tdyu4d01a
endif

ifeq "$(BRDCST_IF_TUNER)" "TDAU8D01A"
OPTIONAL_SUB_COMPONENTS += tdau8d01a
endif

ifeq "$(BRDCST_IF_TUNER)" "TDAU8D03A"
OPTIONAL_SUB_COMPONENTS += tdau8d03a
endif

ifeq "$(BRDCST_IF_TUNER)" "DVT10A"
OPTIONAL_SUB_COMPONENTS += dvt10a
endif

ifeq "$(BRDCST_IF_TUNER)" "DVT8C"
OPTIONAL_SUB_COMPONENTS += dvt8c
endif

ifeq "$(BRDCST_IF_TUNER)" "ENV59S"
OPTIONAL_SUB_COMPONENTS += env59s
endif

ifeq "$(BRDCST_IF_TUNER)" "TDTKH701F"
OPTIONAL_SUB_COMPONENTS += tdtkh701f
endif

ifeq "$(BRDCST_IF_TUNER)" "TDTKH801F"
OPTIONAL_SUB_COMPONENTS += tdtkh801f
endif

ifeq "$(BRDCST_IF_TUNER)" "DTVA20CVH"
OPTIONAL_SUB_COMPONENTS += dtva20cvh
endif

ifeq "$(BRDCST_IF_TUNER)" "ENV56U05D8F"
OPTIONAL_SUB_COMPONENTS += env56u05d8f
endif

ifeq "$(BRDCST_IF_TUNER)" "TDTKG702D"
OPTIONAL_SUB_COMPONENTS += tdtkg702d
endif

ifeq "$(BRDCST_IF_TUNER)" "TDTRT032D"
OPTIONAL_SUB_COMPONENTS += tdtrt032d
endif

ifeq "$(BRDCST_IF_TUNER)" "DMCT6CH"
OPTIONAL_SUB_COMPONENTS += dmct6ch
endif
ifeq "$(BRDCST_IF_TUNER)" "TDTKG931D"
OPTIONAL_SUB_COMPONENTS += tdtkg931d
endif

ifeq "$(BRDCST_IF_TUNER)" "TAF7"
OPTIONAL_SUB_COMPONENTS += taf7
endif

ifeq "$(BRDCST_IF_TUNER)" "DTC86CT"
OPTIONAL_SUB_COMPONENTS += dtc86ct
endif

ifeq "$(BRDCST_IF_TUNER)" "CTVTUNERLST"
OPTIONAL_SUB_COMPONENTS += dtva20cvh tdtkh801f
endif

ifeq "$(BRDCST_IF_TUNER)" "TPV2K11NAFTALST"
OPTIONAL_SUB_COMPONENTS += tdtkh801f env56u05d8f
endif

ifeq "$(BRDCST_IF_TUNER)" "TPVTUNERALL"
OPTIONAL_SUB_COMPONENTS += tdtkh801f da89wt6e tdvwh810f tdau8d03a
endif

#####################################5363 Third party ISDBT demod#####################
ifeq "$(BRDCST_IF_TUNER)" "VA1E1BF2403"
OPTIONAL_SUB_COMPONENTS += va1e1bf2403
endif

ifeq "$(BRDCST_IF_TUNER)" "VA1E1BF2401"
OPTIONAL_SUB_COMPONENTS += va1e1bf2401
endif

ifeq "$(BRDCST_IF_TUNER)" "VA1P1BF8401"
OPTIONAL_SUB_COMPONENTS += va1p1bf8401
endif

ifeq "$(BRDCST_IF_TUNER)" "VA1P1EL8402"
OPTIONAL_SUB_COMPONENTS += va1p1el8402
endif
ifeq "$(BRDCST_IF_TUNER)" "FI4801"
OPTIONAL_SUB_COMPONENTS += fi4801
endif

ifeq "$(BRDCST_IF_TUNER)" "SBLST"
OPTIONAL_SUB_COMPONENTS += va1e1bf2403 va1e1bf2401 va1p1bf8401
endif

ifeq "$(BRDCST_IF_TUNER)" "TUNERALL"
OPTIONAL_SUB_COMPONENTS += env57s ft2101 ft2607 tdtws810d
endif

ifeq "$(BRDCST_IF_TUNER)" "TTELST"
OPTIONAL_SUB_COMPONENTS += env56s da58ct
endif

ifeq "$(BRDCST_IF_TUNER)" "TALST"
OPTIONAL_SUB_COMPONENTS += tdvwh810f env56s
endif

ifeq "$(BRDCST_IF_TUNER)" "2K10LST"
OPTIONAL_SUB_COMPONENTS += env57s ft2101 ft2607 tdtws810d
endif

ifeq "$(BRDCST_IF_TUNER)" "TPV2K11EULST"
OPTIONAL_SUB_COMPONENTS += env57u03d5f tdtkg931d
endif

#####################################5363 Third party DTMB demod#####################
ifeq "$(BRDCST_IF_TUNER)" "VA1G2CD"
OPTIONAL_SUB_COMPONENTS += va1g2cd
endif
ifeq "$(BRDCST_IF_TUNER)" "VA1G2CD8001"
OPTIONAL_SUB_COMPONENTS += va1g2cd
endif
ifeq "$(BRDCST_IF_TUNER)" "DT85WT"
OPTIONAL_SUB_COMPONENTS += DT85WT
endif

ifeq "$(BRDCST_IF_TUNER)" "TDAC"
OPTIONAL_SUB_COMPONENTS += tdac
endif

ifeq "$(BRDCST_IF_TUNER)" "DMCT6AH"
OPTIONAL_SUB_COMPONENTS += dmct6ah
endif

ifeq "$(BRDCST_IF_TUNER)" "DMT10B"
OPTIONAL_SUB_COMPONENTS += dmt10b
endif

ifeq "$(BRDCST_IF_TUNER)" "F35CT"
OPTIONAL_SUB_COMPONENTS += f35ct
endif

ifeq "$(BRDCST_IF_TUNER)" "CTMRTUNER"
OPTIONAL_SUB_COMPONENTS += ctmrtuner
endif

ifeq "$(BRDCST_IF_TUNER)" "DT91WT"
OPTIONAL_SUB_COMPONENTS += dt91wt
endif

ifeq "$(BRDCST_IF_TUNER)" "RE215"
OPTIONAL_SUB_COMPONENTS += re215
endif

ifeq "$(BRDCST_IF_TUNER)" "HFT28"
OPTIONAL_SUB_COMPONENTS += hft28
endif

ifeq "$(BRDCST_IF_TUNER)" "RA217"
OPTIONAL_SUB_COMPONENTS += ra217
ifeq "$(findstring DUMMY, $(BRDCST_IF))" "DUMMY"
DEFINES += -DCC_DUMMY_RA217
endif
endif
############################################################################
ifeq "$(BRDCST_IF)" "5387DTD_TUNERALL_P_anana"
OPTIONAL_SUB_COMPONENTS += tdau4xb02a tdqu8x001a
endif
############################################################################
ifeq "$(BRDCST_IF)" "5391ROW_P_analog"
OPTIONAL_SUB_COMPONENTS += uv1336 uv1356e uv1316e
endif
############################################################################
ifeq "$(BRDCST_IF)" "5363DVBT_TTEI2CALL_P_atdeu"
OPTIONAL_SUB_COMPONENTS += env57s dt70wi
endif
############################################################################
ifeq "$(BRDCST_IF_TUNER)" "TAF16"
OPTIONAL_SUB_COMPONENTS += TAF16
endif
ifeq "$(BRDCST_IF_TUNER)" "DA91WT"
OPTIONAL_SUB_COMPONENTS += da91wt
endif
##############################################################################
#
# Include the makefile common to all components
#
##############################################################################

include $(DRV_ROOT)/driver.mak
include $(DRV_ROOT)/brdcst_if.mak
