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
ifndef PRJ_ROOT
	ifdef KERNEL_DRV_ROOT
		ifdef O
			DRV_SRC_ROOT =../../
		endif
	else
		DRV_SRC_ROOT =
	endif
	export PRJ_ROOT =$(DRV_SRC_ROOT)drivers/mtk/mt5396
endif

ifndef LINUX_DRV_ROOT
export LINUX_DRV_ROOT = $(PRJ_ROOT)
endif

ifndef DRV_ROOT
export DRV_ROOT = $(PRJ_ROOT)
endif

TARGET_ROOT = $(DRV_ROOT)/$(DRIVER_DIR)
TARGET_INC = -I$(TARGET_ROOT)/private_inc	\
		     -I$(TARGET_ROOT)/drv_inc		\
		     -I$(TARGET_ROOT)/inc           \
		     -I$(TARGET_ROOT)/psr           \
		     -I$(TARGET_ROOT)/drv_cust      \
             -I$(TARGET_ROOT)/brdcst_if/tuner_src/inc                   \
             -I$(TARGET_ROOT)/b2r                                       \
             -I$(TARGET_ROOT)/playmgr                                   \
             -I$(TARGET_ROOT)/jpg                                       \
             -I$(TARGET_ROOT)/bsp                                       \
             -I$(TARGET_ROOT)/pcmcia                                    \
             -I$(TARGET_ROOT)/pcmcia/CableCard                          \
             -I$(TARGET_ROOT)/brdcst_if/tuner_src/TunerATD/eu           \
             -I$(TARGET_ROOT)/brdcst_if/tuner_src/TunerATD/na           \
             -I$(TARGET_ROOT)/brdcst_if/brdcst_if/tuner_src/TunerATD/na \
             -I$(TARGET_ROOT)/usb/include                               \
             -I$(TARGET_ROOT)/usb/include/class                         \
             -I$(TARGET_ROOT)/usb/src/cd                                \
             -I$(TARGET_ROOT)/usb/src/drivers                           \
             -I$(TARGET_ROOT)/usb/src/drivers/aud                       \
             -I$(TARGET_ROOT)/usb/src/drivers/bluetooth                 \
             -I$(TARGET_ROOT)/usb/src/drivers/hid                       \
             -I$(TARGET_ROOT)/usb/src/drivers/hub                       \
             -I$(TARGET_ROOT)/usb/src/drivers/msd                       \
             -I$(TARGET_ROOT)/usb/src/drivers/sicd                      \
             -I$(TARGET_ROOT)/usb/src/drivers/trans                     \
             -I$(TARGET_ROOT)/usb/src/functions/trans_fn                \
             -I$(TARGET_ROOT)/usb/src                                   \
             -I$(TARGET_ROOT)/usb/src/systems/none/afs                  \
             -I$(TARGET_ROOT)/usb/src/systems/none                      \
             -I$(TARGET_ROOT)/usb/src/systems/ucos						\
             -I$(TARGET_ROOT)/venc										\
             -I$(TARGET_ROOT)/venc/h264									\
             -I$(TARGET_ROOT)/osd6896   \
             -I$(TARGET_ROOT)/dmx6896
ifdef DRV_INC_DIR
TARGET_INC += -I$(DRV_INC_DIR)
endif
AUD_ROOT = $(DRV_ROOT)/$(DRIVER_DIR)/aud
AUD_INC = -I$(AUD_ROOT)/include             \
          -I$(AUD_ROOT)/adsp                \
          -I$(AUD_ROOT)/adsp/include        \
          -I$(AUD_ROOT)/adsp/include_API    \
          -I$(AUD_ROOT)/audio \
          -I$(AUD_ROOT)/periph

D_CUST_INC = -I$(DRV_ROOT)/../d_inc         \
             -I$(DRV_ROOT)/d_inc

ifeq "$(OSAI_VER)" "2"
OSAI_ROOT = osai_v2
else
OSAI_ROOT = osai
endif

LINUX_ROOT = $(DRV_ROOT)/linux
LINUX_INC = -Iinclude/asm/arch/inc          \
            -I$(LINUX_ROOT)/include         \
            -I$(LINUX_ROOT)/$(OSAI_ROOT)/inc        \
		    -I$(LINUX_ROOT)/lib/mw/x_inc	\
		    -I$(LINUX_ROOT)/app_if/mtal     \
		    -I$(LINUX_ROOT)/include/linux   \
		    -I$(LINUX_ROOT)/cb

PLAYMGR_ROOT = $(DRV_ROOT)/$(DRIVER_DIR)/playmgr_v2
PLAYMGR_INC = -I$(PLAYMGR_ROOT)

SWDMX_ROOT = $(DRV_ROOT)/$(DRIVER_DIR)/swdmx_v2
SWDMX_INC = -I$(SWDMX_ROOT)

DMX_ROOT =  $(DRV_ROOT)/$(DRIVER_DIR)/dmx6896
DMX_INC = -I$(DMX_ROOT) -I$(DMX_ROOT)/gcpu -I$(DMX_ROOT)/aes -I$(DMX_ROOT)/drm/mtk_skb_driver/ -I$(DMX_ROOT)/drm/libtomcrypt-1.16/src/headers -I$(DMX_ROOT)/drm/libtommath-0.41

VDEC_ROOT = $(DRV_ROOT)/$(DRIVER_DIR)/vdec_v2
VDEC_INC = -I$(VDEC_ROOT) \
           -I$(VDEC_ROOT)/h264 \
           -I$(VDEC_ROOT)/mpeg4 \
           -I$(VDEC_ROOT)/mpv \
           -I$(VDEC_ROOT)/wmv \
           -I$(VDEC_ROOT)/img \
           -I$(VDEC_ROOT)/rv \
           -I$(VDEC_ROOT)/avs \
           -I$(VDEC_ROOT)/vp6 \
           -I$(VDEC_ROOT)/vp8

TUNER_ROOT = $(DRV_ROOT)/$(DRIVER_DIR)/brdcst_if/tuner_src
TUNER_INC = -I$(TUNER_ROOT)/Tuner \
            -I$(TUNER_ROOT)/TunerHAL \
            -I$(TUNER_ROOT)/CtrlBus \
            -I$(TUNER_ROOT)/TunerPool \
            -I$(TUNER_ROOT)/TunerNptv/system

ifeq "$(findstring 5396_MT5112EE,$(BRDCST_IF))" "5396_MT5112EE"
TUNER_INC += -I$(TUNER_ROOT)/TunerDTD
TUNER_INC += -I$(TUNER_ROOT)/TunerAnalogNA
DEFINES += -DCC_FEDTD
endif
ifeq "$(findstring 5396_MT5135,$(BRDCST_IF))" "5396_MT5135"
TUNER_INC += -I$(TUNER_ROOT)/TunerDVBT
TUNER_INC += -I$(TUNER_ROOT)/TunerAnalogEU
DEFINES += -DCC_FEDVBT
endif
ifeq "$(findstring 5396DUAL,$(BRDCST_IF))" "5396DUAL"
TUNER_INC += -I$(TUNER_ROOT)/TunerDTD
TUNER_INC += -I$(TUNER_ROOT)/TunerAnalogNA
TUNER_INC += -I$(TUNER_ROOT)/TunerDVBT
TUNER_INC += -I$(TUNER_ROOT)/TunerAnalogEU
endif
ifeq "$(findstring 5396DVBTC,$(BRDCST_IF))" "5396DVBTC"
TUNER_INC += -I$(TUNER_ROOT)/TunerDVBT
TUNER_INC += -I$(TUNER_ROOT)/TunerDVBC
DEFINES += -DCC_DVBT_SUPPORT
endif
ifeq "$(findstring TC90517,$(BRDCST_IF))" "TC90517"
TUNER_INC += -I$(TUNER_ROOT)/Tuner3rdParty/TC90517
endif
ifeq "$(findstring MN88471,$(BRDCST_IF))" "MN88471"
TUNER_INC += -I$(TUNER_ROOT)/Tuner3rdParty/MN88471/
TUNER_INC += -I$(TUNER_ROOT)/TunerDVBT
endif

ifeq "$(findstring CXD2820,$(BRDCST_IF))" "CXD2820"
TUNER_INC += -I$(TUNER_ROOT)/Tuner3rdParty/CXD2820
TUNER_INC += -I$(TUNER_ROOT)/TunerDVBT
endif
TUNER_INC += -I$(TUNER_ROOT)/TunerATD/common

NPTV_ROOT = $(DRV_ROOT)/$(DRIVER_DIR)/nptv6896

NPTV_INC += -I$(NPTV_ROOT)/inc \
		  -I$(NPTV_ROOT)/inc/hw \
		  -I$(NPTV_ROOT)/inc/drv \
		  -I$(NPTV_ROOT)/inc/sys \
		  -I$(NPTV_ROOT)/drv/tuner/system \
		  -I$(NPTV_ROOT)/inc/empty \
		  -I$(NPTV_ROOT)/inc/tmp

VDP_ROOT = $(DRV_ROOT)/$(DRIVER_DIR)/vdp6896

VDP_INC += -I$(VDP_ROOT)

export DRIVER_INC = $(TARGET_INC) $(AUD_INC) $(D_CUST_INC) $(LINUX_INC) $(TUNER_INC) $(VDEC_INC) $(PLAYMGR_INC) $(SWDMX_INC) $(NPTV_INC) $(VDP_INC) $(DMX_INC)

DEFINES += -D_CPU_LITTLE_ENDIAN_ -DCC_MT5396 -DCC_MT5396 -DCC_DRIVER_PROGRAM
DEFINES += -DCC_SCPOS_EN -DCC_SRM_ON -DDEFAULT_PANEL_SELECT=0
DEFINES += -DCC_53XX_SWDMX_V2
# -DDEFAULT_PANEL_SELECT=$(PANEL_SELECT)
DEFINES += -DCC_DYNAMIC_POWER_ONOFF

ifeq "$(BUILD_CFG)" "debug"
DEFINES += -g -mapcs-frame
else
DEFINES += -DNDEBUG
endif

OPTIMIZE_LVL ?= -O2
DEFINES += $(OPTIMIZE_LVL)

export DEFINES := $(sort $(DEFINES))
