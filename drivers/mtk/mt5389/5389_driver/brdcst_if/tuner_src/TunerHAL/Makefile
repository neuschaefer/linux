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
##############################################################################

#$(THIS_COMPONENT) only work at sys build
THIS_COMPONENT = $(BRDCST_IF)
#$(THIS_COMPONENT_DIR) need for driver build
THIS_COMPONENT_DIR = TunerHAL

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../../..
endif

SRC = UtilAPI.c TDMgr.c drvapi_tuner.c TDRegistration.c I2c_sw.c \
	pd_remap_glue.c nim_drvif.c

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS =

CC_INC += -I../CtrlBus

DEFINES += -DPFXMSG=HAL

BRDCST_IF_MAJOR := $(word 1, $(subst _P_, P_, $(BRDCST_IF)))
BRDCST_IF_PARAM := $(word 2, $(subst _P_, P_, $(BRDCST_IF)))


###############################################################
#5365DUMMY_MT5135_RE201_P_atdun
ifeq "$(findstring DUMMY,$(BRDCST_IF_MAJOR))" "DUMMY"
ifeq "$(findstring MT5135,$(BRDCST_IF_MAJOR))" "MT5135"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdun,$(BRDCST_IF_PARAM)),-D5135DUMMY_P_ATDUN))
endif
endif
########## Highest priority on TOP  ###########################
#exact match BRDCST_IF=dummy
TD:=$(if $(strip $(TD)),$(TD),$(if $(subst dummy,,$(BRDCST_IF)),,-DDUMMY)) 

#####################DTD&DUMMY#############################
ifeq "$(findstring DTDDUMMY,$(BRDCST_IF_MAJOR))" "DTDDUMMY"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring uatd,$(BRDCST_IF_PARAM)),-DDTDDUMMY_P_UATD))
endif
#####################VDRDVBC#############################
ifeq "$(findstring VDRDVBC,$(BRDCST_IF_MAJOR))" "VDRDVBC"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DVDRDVBC_P_ATDEU -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_CAB_DIG=Vendor))
endif
endif
###########################DVBT2#############################
ifeq "$(findstring CXD2820,$(BRDCST_IF_MAJOR))" "CXD2820"
ifeq "$(findstring S3501,$(BRDCST_IF_MAJOR))" "S3501"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDVB2TS_P_ATDEU -DPD_REMAP_TER_DIG=DVBT2 -DPD_REMAP_SAT_DIG=DVBS -DPD_REMAP_CAB_DIG=DVBT2))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anaeu,$(BRDCST_IF_PARAM)),-DDVB2T_P_ANAEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDVB2T_P_ATDEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDVB2T_P_DUMMY))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anana,$(BRDCST_IF_PARAM)),-DDVB2T_P_ANANA))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DDVB2T_P_ATDNA))
endif
endif
###################################################
ifeq "$(findstring VDRDOMEDT2,$(BRDCST_IF_MAJOR))" "VDRDOMEDT2"
ifeq "$(findstring VDRDVBS,$(BRDCST_IF_MAJOR))" "VDRDVBS"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDVB2TS_P_ATDEU -DPD_REMAP_TER_DIG=DVBT2 -DPD_REMAP_SAT_DIG=DVBS -DPD_REMAP_CAB_DIG=DVBT2))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anaeu,$(BRDCST_IF_PARAM)),-DDVB2T_P_ANAEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDVB2T_P_ATDEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDVB2T_P_DUMMY))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anana,$(BRDCST_IF_PARAM)),-DDVB2T_P_ANANA))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DDVB2T_P_ATDNA))
endif
endif

###########################DVBT2#############################
###########################DVBT2#############################
ifeq "$(findstring MN88471,$(BRDCST_IF_MAJOR))" "MN88471"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anaeu,$(BRDCST_IF_PARAM)),-DDVB2T_P_ANAEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDVB2T_P_ATDEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDVB2T_P_DUMMY))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anana,$(BRDCST_IF_PARAM)),-DDVB2T_P_ANANA))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DDVB2T_P_ATDNA))
endif
#####################DTDDVBTC##########################################
ifeq "$(findstring DTDDVBTC,$(BRDCST_IF_MAJOR))" "DTDDVBTC"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTDDVBTC_P_ANALOG -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_CAB_DIG=DVBC))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DDTDDVBTC_P_ANALOG -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_CAB_DIG=DVBC))
endif
endif

#####################DVBCT#############################
ifeq "$(findstring DVBCT,$(BRDCST_IF_MAJOR))" "DVBCT"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDVBCT_P_ATDEU -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_CAB_DIG=DVBC))
endif
endif
#####################DVBTC#############################Lei_code_dB##############
ifeq "$(findstring DVBTC,$(BRDCST_IF_MAJOR))" "DVBTC"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDVBTC_P_ATDEU -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_CAB_DIG=DVBC))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DDVBTC_P_ATDNA -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_CAB_DIG=DVBC))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring DUMMY,$(BRDCST_IF_PARAM)),-DDVBTC_P_DUMMY -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_CAB_DIG=DVBC))
endif
endif
###########################MT5135#############################
ifeq "$(findstring MT5135,$(BRDCST_IF_MAJOR))" "MT5135"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
ifeq "$(findstring S3501,$(BRDCST_IF_MAJOR))" "S3501"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DMT5135DVBS_P_ATDEU -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_SAT_DIG=DVBS -DPD_REMAP_CAB_DIG=DVBC))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DMT5135_P_ATDEU -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_CAB_DIG=DVBC))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DMT5135_P_ATDNA -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_CAB_DIG=DVBC))
endif
endif
endif

###########################MT5135/MT5112EE#############################
ifeq "$(findstring MT5135MT5112EE,$(BRDCST_IF_MAJOR))" "MT5135MT5112EE"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring uatd,$(BRDCST_IF_PARAM)),-DMT5135MT5112EE_P_UATD -DPD_REMAP_TER_DIG=DVBT -DPD_REMAP_CAB_DIG=DVBC))
endif
endif
###########################DVBT#############################
ifeq "$(findstring DUMMY,$(BRDCST_IF_MAJOR))" "DUMMY"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anana,$(BRDCST_IF_PARAM)),-DDUMMY_P_ANANA))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DDUMMY_P_ATDNA))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anaeu,$(BRDCST_IF_PARAM)),-DDUMMY_P_ANAEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDUMMY_P_ATDEU))
endif
###########################DTD#############################
ifeq "$(findstring DTD,$(BRDCST_IF_MAJOR))" "DTD"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anana,$(BRDCST_IF_PARAM)),-DDTD_P_ANANA))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DDTD_P_ATDNA))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTD_P_ATDEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDTD_P_DUMMY))
endif
endif
###########################MT5112#############################
ifeq "$(findstring MT5112EE,$(BRDCST_IF_MAJOR))" "MT5112EE"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DMT5112EE_P_ATDNA))
endif
endif
###########################DVBS#############################
ifeq "$(findstring S3501,$(BRDCST_IF_MAJOR))" "S3501"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anaeu,$(BRDCST_IF_PARAM)),-DDVBS_P_ANAEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDVBS_P_ATDEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDVBS_P_DUMMY))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anana,$(BRDCST_IF_PARAM)),-DDVBS_P_ANANA))
#TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDVBS_P_ATDEU -DPD_REMAP_SAT_DIG=DVBS -DPD_REMAP_CAB_DIG=DVBC))
endif
endif
###########################DVBT#############################
ifeq "$(findstring DVBT,$(BRDCST_IF_MAJOR))" "DVBT"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anaeu,$(BRDCST_IF_PARAM)),-DDVBT_P_ANAEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDVBT_P_ATDEU))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDVBT_P_DUMMY))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anana,$(BRDCST_IF_PARAM)),-DDVBT_P_ANANA))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DDVBT_P_ATDNA))
endif
endif
#####################VDRISDBT#############################
ifeq "$(findstring VDRISDBT,$(BRDCST_IF_MAJOR))" "VDRISDBT"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DVDRISDBT_P_ATDNA))
endif
else
############################ISDBT#############################
ifeq "$(findstring ISDBT,$(BRDCST_IF_MAJOR))" "ISDBT"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anana,$(BRDCST_IF_PARAM)),-DISDBT_P_ANANA))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdna,$(BRDCST_IF_PARAM)),-DISDBT_P_ATDNA))
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DISDBT_P_DUMMY))
endif
endif
endif
############################3rd_party porting DTMB#############################
ifeq "$(findstring VDRDTMBDVBC,$(BRDCST_IF_MAJOR))" "VDRDTMBDVBC"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTMBREMAP_P_ATDEU -DPD_REMAP_TER_DIG=DTMB -DPD_REMAP_CAB_DIG=DVBC))
endif
ifeq "$(findstring VDRDTMB,$(BRDCST_IF_MAJOR))" "VDRDTMB"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTMB_P_ATDEU))
endif
############################DTMB#############################
ifeq "$(findstring DTMB,$(BRDCST_IF_MAJOR))" "DTMB"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTMB_P_ATDEU))
endif
endif
############################8G80#############################
ifeq "$(findstring 8G80,$(BRDCST_IF_MAJOR))" "8G80"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
ifeq "$(findstring anaeu,$(BRDCST_IF_PARAM))" "anaeu"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring anaeu,$(BRDCST_IF_PARAM)),-DDTMB_P_ANAEU))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTMB_P_ATDEU))
#TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTMB_P_ATDEU -DPD_REMAP_TER_DIG=DTMB -DPD_REMAP_CAB_DIG=DVBC))
endif
endif
endif

############################8G80#############################
ifeq "$(findstring ATBM8848,$(BRDCST_IF_MAJOR))" "ATBM8848"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTMBREMAP_P_ATDEU -DPD_REMAP_TER_DIG=DTMB -DPD_REMAP_CAB_DIG=DVBC))
endif
endif
############################8846E#############################
ifeq "$(findstring ATBM8846E,$(BRDCST_IF_MAJOR))" "ATBM8846E"
ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
else
TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTMBREMAP_P_ATDEU -DPD_REMAP_TER_DIG=DTMB -DPD_REMAP_CAB_DIG=DVBC))
endif
endif
############################MT5175#############################
#MT5175, MT5175DTMB, MT5135DVBCMT5175DTMB  maybe include in brdcst_if name
ifeq "$(findstring MT5175,$(BRDCST_IF_MAJOR))" "MT5175"
	ifeq "$(findstring MT5175DTMB,$(BRDCST_IF_MAJOR))" "MT5175DTMB"
		ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
			TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
		else
			TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTMB_P_ATDEU))
		endif            
	else
		ifeq "$(findstring dummy,$(BRDCST_IF_PARAM))" "dummy"
			TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring dummy,$(BRDCST_IF_PARAM)),-DDUMMY))
		else
			TD:=$(if $(strip $(TD)),$(TD),$(if $(findstring atdeu,$(BRDCST_IF_PARAM)),-DDTMBREMAP_P_ATDEU -DPD_REMAP_TER_DIG=DTMB -DPD_REMAP_CAB_DIG=DVBC))
		endif    	
	endif
	
endif
#nothing defined, error
TD:=$(if $(strip $(TD)),$(TD),$(error BRDCST_IF not match for TDRegistration!!!)) 
DEFINES += $(strip $(TD))

##############################################################################
#
# Include the makefile common to all components
#
##############################################################################

include $(DRV_ROOT)/driver.mak
include $(DRV_ROOT)/brdcst_if.mak
