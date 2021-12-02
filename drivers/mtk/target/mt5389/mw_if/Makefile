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
# $Revision: #9 $
# $Date: 2009/08/04 $
# $Author: eddie.huang $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

THIS_COMPONENT = mw_if

ifneq "$(LINUX_SOLUTION)" "true"
ifndef DRV_ROOT
export DRV_ROOT = ..
endif
endif

SRC = mw_drv_init.c      \
      custom_cmd.c       \
      indicator_mw.c     \
      fpd_mw.c           \
      rtc_mw.c           \
      char_mw.c          \
      block_mw.c         \
      irrx_mw.c          \
      dbg_mw.c           \
      gfx_mw.c           \
      vdec_mw.c          \
      cec_mw.c           \
      vdp_mw.c           \
      mpv_mw.c           \
      dmx_mw.c           \
      dmx_psi_mw.c       \
      dmx_pes_mw.c       \
      mw_comp_excl_reg.c \
      tvsrc_mw.c         \
      drv_cust_api.c     \
      aud_mw.c           \
      mw_drv_post_init.c \
      d_drv_cust_util.c  \
      avc_combi_mw.c     \
      pcd_mw.c           \
      tuner_mw.c         \
      osd_mw.c           \
      bagt_mw.c          \
      pmx_mw.c           \
      usb_socket_mw.c    \
      usb_sicd_mw.c      \
      jpg_mw.c           \
      stc_mw.c           \
      cust_drv_mw.c      \
      tve_mw.c           \
      vout_mw.c          \
      drm_mw.c

      # drv_cust_api.c jpg_mw.c ci_mw.c pod_mw.c
ifneq "$(LINUX_SOLUTION)" "true"
SRC += uart_mw.c	\
      pcmcia_mw.c
endif

ifeq "$(LINUX_SOLUTION)" "true"
ifneq ($(strip $(findstring -DDVBT_CI_ENABLE, $(DEFINES))),)
SRC += pcmcia_mw.c
endif
endif

SRC += chip_spec_mw.c

ifdef ENABLE_MULTIMEDIA
SRC += dmx_mm_mw_v2.c dmx_mm_pes_mw.c
endif

ifdef TIME_SHIFT_SUPPORT
SRC += dmx_rec_mw.c pvr_mw.c pvrkey_mw.c
endif

OBJ =

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS =

DEFINES +=

ifeq "$(LINUX_SOLUTION)" "true"
DEFINES += -D_CPU_LITTLE_ENDIAN_ -UCC_DRIVER_PROGRAM
DEFINES += -DDXX_HV_POS=1
endif

CC_INC += -I$(DRV_ROOT)/$(subst mt,,$(TARGET))_driver/brdcst_if/tuner_src/inc
CC_INC += -I$(DRV_ROOT)/$(subst mt,,$(TARGET))_driver/brdcst_if/tuner_src/TunerHAL

export ADD_TO_SYMLINK_FILES = \
	avc_combi_mw.h bagt_mw.h block_mw.h cec_mw.h cust_drv_mw.h d_drv_cust_util.h \
	dmx_mw.h drv_comp_id.h drv_dsp_cfg.h drv_jpg.h drv_name.h mw_comp_excl_reg.h \
	mw_drv_post_init.h tvsrc_mw.h usb_sicd_mw.h vdec_mw.h

#############################################################################
#
# Include the makefile common to all components
#

#ifneq "$(LINUX_SOLUTION)" "true"
include $(DRV_ROOT)/brdcst_if.mak
include $(DRV_ROOT)/nptv.mak
ifeq "$(LINUX_SOLUTION)" "true"
-include $(DRV_ROOT)/symlink.mak
LINUX_ROOT    ?= $(word 1, $(subst /vm_linux/,/vm_linux /, $(shell pwd)))
DRV_ADPT_ROOT ?= $(LINUX_ROOT)/dtv_linux/dtv_driver_adpt
include $(DRV_ADPT_ROOT)/driver.mak
else
include $(DRV_ROOT)/driver.mak
endif
#else
#include $(DRV_ADPT_ROOT)/driver.mak
#endif
###############
#linefeed test
###############

