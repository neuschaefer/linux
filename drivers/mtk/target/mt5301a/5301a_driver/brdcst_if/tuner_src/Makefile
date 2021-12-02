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
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#
#         BRDCST_IF naming rule:
#
#             [chip number][digital demod]_P_[analog demod](_[parameter])
#
#         e.g. 5387DTD_TDAU4XB02A_P_anana
#              5386DTD_SS203EH_P_atdna
#         
##############################################################################

THIS_COMPONENT = $(BRDCST_IF)

export APPEND_OBJ_TYPE := brdcst_if/$(BRDCST_IF)
export BRDCST_IF_PARAM := $(word 2, $(subst _P_, P_, $(BRDCST_IF)))

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../..
endif

#export TUNER_SRC_ROOT = .

SRC =

OPTIONAL_SUB_COMPONENTS=

#add symbol TUNER_SUPPORT_SCART_OUT here to avoid build code error when build all library under NA sys build directory
#need refine this under mt539x

#-------------------------------------------------

####################################################################################
####### brdcst_if_global_flag.def define tuner driver global compile flag.
####################################################################################
#ifdef LINUX_DRV_ROOT
#include $(DRV_ROOT)/5391_driver/brdcst_if/tuner_src/brdcst_if_global_flag.def
#else
#include ./brdcst_if_global_flag.def
#endif
# For MT539x
DEFINES += -DfcADD_DIAG_INFO


####################################################################################
####### depend on BRDCST_IF to select tuner module
####################################################################################
ifeq "$(findstring DTD, $(BRDCST_IF))" "DTD"
OPTIONAL_SUB_COMPONENTS  += TunerDTD
DEFINES += -DDTD_INCLUDE
endif
ifeq "$(findstring DVBT, $(BRDCST_IF))" "DVBT"
OPTIONAL_SUB_COMPONENTS  += TunerDVBT
endif
ifeq "$(findstring DVBCT, $(BRDCST_IF))" "DVBCT"
OPTIONAL_SUB_COMPONENTS  += TunerDVBT
OPTIONAL_SUB_COMPONENTS  += Tuner3rdParty
DEFINES += -DCC_CAB_DEMOD_TUNER_BYPASS
endif
ifeq "$(findstring DVBS, $(BRDCST_IF))" "DVBS"
OPTIONAL_SUB_COMPONENTS  += TunerDVBS
OPTIONAL_SUB_COMPONENTS  += Tuner3rdParty
DEFINES += -DCC_CAB_DEMOD_TUNER_BYPASS
endif
ifeq "$(findstring DUMMY, $(BRDCST_IF))" "DUMMY"
OPTIONAL_SUB_COMPONENTS  += TunerDummy
endif
###if not 3-party,then include TunerISDBT
ifneq "$(findstring VDRISDBT, $(BRDCST_IF))" "VDRISDBT"
ifeq "$(findstring ISDBT,$(BRDCST_IF))" "ISDBT"
OPTIONAL_SUB_COMPONENTS  += Tuner3rdParty
endif
endif
ifeq "$(findstring anana, $(BRDCST_IF_PARAM))" "anana"
OPTIONAL_SUB_COMPONENTS += TunerAnalogNA
endif
ifeq "$(findstring anaeu, $(BRDCST_IF_PARAM))" "anaeu"
OPTIONAL_SUB_COMPONENTS += TunerAnalogEU
endif
ifeq "$(findstring atd, $(BRDCST_IF_PARAM))" "atd"
OPTIONAL_SUB_COMPONENTS += TunerATD
endif
######################################################################################
#######  All BRDCST_IF will need TunerPool & CtrlBus module(ITuner) & TunerHAL module
######################################################################################
ifeq "$(BRDCST_IF)" "dummy"
OPTIONAL_SUB_COMPONENTS += TunerDummy
else
OPTIONAL_SUB_COMPONENTS += TunerPool
endif
OPTIONAL_SUB_COMPONENTS += CtrlBus
OPTIONAL_SUB_COMPONENTS += TunerHAL

####################################################################################
#######  When adding a new BRDCST_IF, add fcTUNER_TYPE here.
#######  Also need add a fcTUNER_TYPE number in Tuner/pd_tuner_type.h
####################################################################################
#  For 5387.
####################################################################################
ifeq "$(BRDCST_IF)" "5387DTD_TDAU4XB02A_P_anana"
DEFINES += -DfcTUNER_TYPE=cALPS_TDAU4XB02A
endif
ifeq "$(BRDCST_IF)" "5387DTD_TDQU8X001A_P_anana"
DEFINES += -DfcTUNER_TYPE=cALPS_TDQU8X001A
endif
ifeq "$(findstring TDVWH810F,$(BRDCST_IF))" "TDVWH810F"
DEFINES += -DfcTUNER_TYPE=cLG_TDVWH810F
endif
ifeq "$(BRDCST_IF)" "5387DTD_TD1636_P_atdna"
DEFINES += -DfcTUNER_TYPE=cNXP_TD1636
endif
ifeq "$(BRDCST_IF)" "5387DUMMY_TEQE3_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cALPS_TEQE3
endif
ifeq "$(BRDCST_IF)" "5387DUMMY_TEQE3SUN_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cALPS_TEQE3_SUNBOARD
endif
ifeq "$(BRDCST_IF)" "5387DTD_TDAU4D01A_P_atdna"
DEFINES += -DfcTUNER_TYPE=cALPS_TDAU4D01A 
endif
ifeq "$(BRDCST_IF)" "5387DUMMY_TEQH3L01A_P_atdna"
DEFINES += -DfcTUNER_TYPE=cALPS_TEQH3L01A
endif
ifeq "$(BRDCST_IF)" "5387DUMMY_TEQH3L01ASUN_P_atdna"
DEFINES += -DfcTUNER_TYPE=cALPS_TEQH3L01A_SUNBOARD
endif
ifeq "$(BRDCST_IF)" "5387DTD_TDAU4D02A_P_atdna"
DEFINES += -DfcTUNER_TYPE=cALPS_TDAU4D02A
endif
ifeq "$(BRDCST_IF)" "5387DTD_ENV56S_P_atdna"
DEFINES += -DfcTUNER_TYPE=cPANASONIC_ENV56S
endif
ifeq "$(BRDCST_IF)" "5387DTD_DA58CT_P_atdna"
DEFINES += -DfcTUNER_TYPE=cTCL_DA58CT
endif
ifeq "$(BRDCST_IF)" "5387DTD_TUNERALL_P_anana"
DEFINES += -DfcTUNER_TYPE=cDTD_TUNER_ALL
endif
ifeq "$(BRDCST_IF)" "5387DTDDUMMY_EMCSLST_P_uatd"
DEFINES += -DfcTUNER_TYPE=cEMCS_TUNERLST
endif
#TTELST for TCL 5387 Project
ifeq "$(BRDCST_IF)" "5387DTD_TTELST_P_atdna"
DEFINES += -DfcTUNER_TYPE=cTTE_TUNERLST
endif

#TPV2K10LST for TPV_SHARP_ATSC 5387 Project
ifeq "$(BRDCST_IF)" "5387DTD_TPV2K10LST_P_atdna"
DEFINES += -DfcTUNER_TYPE=cTPV2K10_TUNERLST
endif

ifeq "$(BRDCST_IF)" "5387DTD_DTVA50CVH_P_atdna"
DEFINES += -DfcTUNER_TYPE=cSAMSUNG_DTVA50CVH
endif

ifeq "$(BRDCST_IF)" "5387DTD_FA2307A_P_atdna"
DEFINES += -DfcTUNER_TYPE=cNUTUNE_FA2307A
endif

ifeq "$(BRDCST_IF)" "8223DUMMY_F20WT_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cTCL_F20WT
endif

#MT5388 For EU Analog-only
ifeq "$(BRDCST_IF)" "5387DUMMY_ENV57S_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cPANASONIC_ENV57S
endif
####################################################################################
#  For 5386.
####################################################################################
ifeq "$(BRDCST_IF)" "5386DTD_SS203EH_P_atdna"
DEFINES += -DfcTUNER_TYPE=cSAMSUNG_DTVS203EH
endif
ifeq "$(BRDCST_IF)" "5386DTD_TEQH3_P_atdna"
DEFINES += -DfcTUNER_TYPE=cALPS_TEQH3
endif
####################################################################################
#  For 5363.
####################################################################################
ifeq "$(BRDCST_IF)" "5363DVBT_TDVRH751F_P_anana"
DEFINES += -DfcTUNER_TYPE=cLG_TDVRH751F
DEFINES += -DCC_MT5363
endif

ifeq "$(BRDCST_IF)" "5363DVBT_TDTWS710D_P_anaeu"
DEFINES += -DfcTUNER_TYPE=cLG_TDTWS710D
DEFINES += -DCC_MT5363
endif

ifeq "$(BRDCST_IF)" "5363DUMMY_TAFTZ716D_P_anaeu"
DEFINES += -DfcTUNER_TYPE=cLG_TAFTZ716D
DEFINES += -DCC_MT5363
endif

ifeq "$(BRDCST_IF)" "5363DVBT_TDAG4B03A_P_anaeu"
DEFINES += -DfcTUNER_TYPE=cALPS_TDAG4B03A
DEFINES += -DCC_MT5363
endif
ifeq "$(BRDCST_IF)" "5363DVBT_ENV57S_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cPANASONIC_ENV57S
DEFINES += -DCC_MT5363
endif
ifeq "$(BRDCST_IF)" "5363DVBT_FT2101_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cNXP_FT2101
DEFINES += -DCC_MT5363
endif
ifeq "$(BRDCST_IF)" "5363DVBT_FT2607_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cNuTune_FT2607
DEFINES += -DCC_MT5363
endif
ifeq "$(BRDCST_IF)" "5363DVBT_TDTWS810D_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cLG_TDTWS810D
#DEFINES += -DLNA_MONITOR 
DEFINES += -DCC_MT5363
endif
ifeq "$(BRDCST_IF)" "5363DVBT_DT70WI_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cTCL_DT70WI
DEFINES += -DCC_MT5363
endif
ifeq "$(BRDCST_IF)" "5363DVBT_F05WT_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cTCL_F05WT
DEFINES += -DCC_MT5363
endif
ifeq "$(BRDCST_IF)" "5363DVBT_DTT8D1C_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cXUGUANG_DTT8D1C
DEFINES += -DCC_MT5363
endif
ifeq "$(BRDCST_IF)" "5363DVBT_ENV5SDF_P_atdna"
DEFINES += -DfcTUNER_TYPE=cPANASONIC_ENV5SDF
DEFINES += -DCC_MT5363
endif
ifeq "$(BRDCST_IF)" "5363DVBT_TUNERALL_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cDVBT_TUNER_ALL
DEFINES += -DCC_MT5363
endif
ifeq "$(BRDCST_IF)" "5363DVBT_2K10LST_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cDVBT_2K10_TUNERLST
DEFINES += -DCC_MT5363
endif
ifeq "$(findstring ENV57S,$(BRDCST_IF))" "ENV57S"
DEFINES += -DfcTUNER_TYPE=cPANASONIC_ENV57S
endif
ifeq "$(findstring FT2101,$(BRDCST_IF))" "FT2101"
DEFINES += -DfcTUNER_TYPE=cNXP_FT2101
endif
ifeq "$(findstring FT2607,$(BRDCST_IF))" "FT2607"
DEFINES += -DfcTUNER_TYPE=cNuTune_FT2607
endif
ifeq "$(findstring 2K10LST,$(BRDCST_IF))" "2K10LST"
DEFINES += -DfcTUNER_TYPE=cDVBT_2K10_TUNERLST
endif
ifeq "$(BRDCST_IF)" "5363DVBT_TTEI2CALL_P_atdeu"
DEFINES += -DfcTUNER_TYPE=cTTE_I2CALLLST
DEFINES += -DCC_MT5363
DEFINES += -DI2C_TUNER_ALL
endif

#########################ISDB-T########################
ifeq "$(findstring 5363,$(BRDCST_IF))" "5363"
DEFINES += -DCC_MT5363
endif

ifeq "$(findstring VA1G5BF,$(BRDCST_IF))" "VA1G5BF"
DEFINES += -DfcTUNER_TYPE=cSHARP_VA1G5BF
endif
ifeq "$(findstring VA1E1BF,$(BRDCST_IF))" "VA1E1BF"
DEFINES += -DfcTUNER_TYPE=cSHARP_VA1E1BF
endif

####################################################################################
#######  when adding a new BRDCST_IF, if don't need TunerAnalog module
#######  define your new OPTIONAL_SUB_COMPONENTS behind this line
####################################################################################

SUB_COMPONENTS =

DEFINES +=


##############################################################################
#
# Include the makefile common to all components
#
##############################################################################

include $(DRV_ROOT)/driver.mak
include $(DRV_ROOT)/brdcst_if.mak
