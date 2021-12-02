
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
# $RCSfile: customer.mak,v $
# $Revision: #8 $
# $Date: 2012/06/26 $
# $Author: xiaokuan.shi $
# $SWAuthor: Yan Wang $
#
# Description:
#         Makefile to build a "MT-5351" demo board build. The following
#         targets are supported:
#
#############################################################################

ifndef TARGET
TARGET = linux_mak
endif

export TARGET
export BUILD_NAME
export CUSTOMER
export CUSTOM
export MODEL_NAME
export VERSION
export THIS_ROOT

ifndef RLS_CUSTOM_BUILD
	export RLS_CUSTOM_BUILD=$(shell if [ -d $(LINUX_ROOT)/project_x/mtk_obj ]; then echo 'true'; else echo 'false'; fi )
endif

#
# Set the configuration
#
ifeq "$(BUILD_CFG)" "debug"
CFG_DEF := $(MODEL_NAME)_dbg.cfg
else

ifeq "$(BUILD_CFG)" "cli"
CFG_DEF := $(MODEL_NAME)_cli.cfg
else

CFG_DEF := $(MODEL_NAME).cfg
endif
endif

#
# Get all the path values set
#
ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
endif
LIB_MTAL_DIR =$(VM_LINUX_ROOT)/chiling/app_if/mtal/lib
export CFG_FILE_DIR := $(PHASE2_CFG_FILE_ROOT)/dtv_common

include $(PROJECT_ROOT)/path.mak
include $(TARGET_ROOT)/target.mak
include $(PROJECT_ROOT)/object_type.mak
-include $(THIS_ROOT)/link.mak

#
# Set the search path's
#
ifndef OUTPUT_ROOT
OUTPUT_ROOT     = $(THIS_ROOT)/output/$(OBJECT_TYPE)
endif

ifndef PHASE2_OBJ_ROOT
PHASE2_OBJ_ROOT := $(OBJ_ROOT)/phase2_all
endif

DBG_OUTPUT_ROOT := $(PHASE2_OBJ_ROOT)/lib

#
# Check Fast Build
#
export BRANCH_NAME  = $(word 2,$(BUILD_NAME))
export WARNING_LOG  = $(THIS_ROOT)/build_$(TARGET).log
export WARNING_RST_LOG  = $(THIS_ROOT)/build_warning.log
export ERROR_LOG    = $(THIS_ROOT)/build_fail.log
export CHK_ERR_WARN_SCRIPT = $(TOOL_ROOT)/pbuild/chk_warn_err.sh 
export CHK_ERR_WARN_PARM   = $(TOOL_ROOT)/pbuild $(BRANCH_NAME) $$WARNING_LOG $(ERROR_LOG)

export AUTHOR_XLS_SCRIPT = $(TOOL_ROOT)/pbuild/author_xls.sh 
export AUTHOR_XLS_PARM   = $(TOOL_ROOT)/pbuild $(MODEL_NAME) $(BRANCH_NAME) $(THIS_ROOT)/phase2_warning.result.log

export CHECK_WARNING_BUILD ?= false
ifeq "$(CHECK_WARNING_BUILD)" "true"
export FIND_AUTHOR ?= true
endif

export FIND_AUTHOR ?= false

RLS_CUSTOM_BUILD ?= false
ifeq "$(RLS_CUSTOM_BUILD)" "true"
CHK_ERR_WARN_PARM += CUST_RLS
AUTHOR_XLS_PARM   += CUST_RLS
else
ifneq "$(FIND_AUTHOR)" "true"
CHK_ERR_WARN_PARM += CUST_RLS
AUTHOR_XLS_PARM   += CUST_RLS
endif
endif



#FAST_SUPPORT=false

ifndef FAST_SUPPORT
export FAST_SUPPORT = true
endif

#ifneq "$(FAST_SUPPORT)" "false"
#MAKE = $(FMAKE)
#endif

#
# Set the dynamic lib as default
#
ifndef STATIC_LIB
	STATIC_LIB = false
endif

MEM_ROM_MAP      ?= false
BUILD_PHASE      ?= PHASE2.0
TOOL_CHAIN       ?= 4.4.2

#
# OpenSrc Link Version
ZLIB_VER     ?= 1.2.3
FT_VER       ?= 2.3.7
JPEG_VER     ?= 6b
PNG_VER      ?= 1.2.43
MNG_VER      ?= 1.0.10

#
# Bluetooth
#
BLUETOOTH    ?= false

THIRDPARTY_ROOT     ?= $(PROJECT_ROOT)/../third_party
export VFP_SUFFIX  = 

ifeq "$(TOOL_CHAIN)" "4.5.1"
ifeq "$(ENABLE_VFP)" "true"
export VFP_SUFFIX = _vfp
else ifeq "$(ENABLE_CA9)" "true"
export VFP_SUFFIX = _vfp_ca9
endif
else
ifeq "$(ENABLE_VFP)" "true"
export VFP_SUFFIX = _vfp
endif
endif


#
#for dependency 
#

ifneq "$(MMP_SUPPORT)" "none"
DEPENDENCY_cmpb       = rpc_ipc dtv_common app_if
endif
ifeq "$(BLUETOOTH)" "true"
DEPENDENCY_bluetooth  = dtv_common
endif
DEPENDENCY_app_if     = dtv_common
DEPENDENCY_dtv_common = pthread #stdc++
DEPENDENCY_dtv_svc    = dtv_common dtv_svc_if rpc_ipc pthread  dl #rt config_parser network_detect
DEPENDENCY_dtv_svc_if = rpc_ipc dtv_common
DEPENDENCY_handle_app = pthread dtv_common
DEPENDENCY_rpc_ipc    = pthread 
DEPENDENCY_ut_sample  = app_if rpc_ipc dtv_common pthread handle_app
DEPENDENCY_mtkapp     = dtv_common app_if pthread dl
DEPENDENCY_tqtvd_atv_porting_layer	= dtv_common pthread dl cmpb mng

DEPENDENCY_validate_common = app_if app_if_rpc dtv_common handle_app rpc_ipc pthread

ifneq "$(MMP_SUPPORT)" "none"
DEPENDENCY_mtkapp += cmpb
DEPENDENCY_validate_common += cmpb
endif

ifeq "$(GINGA_SUPPORT)" "true"
DEPENDENCY_dtv_svc    += tqtvd_atv_porting_layer cmpb handle_app 
endif

#DEPENDENCY_validate_network_detect += network_detect pthread

ifeq "$(BLUETOOTH)" "true"
DEPENDENCY_mtkapp += bluetooth
DEPENDENCY_validate_common += bluetooth
DEPENDENCY_dtv_svc += bluetooth
endif

#
#for p1.75 new library dependency
#
DEPENDENCY_app_if_rpc     = rpc_ipc dtv_common
DEPENDENCY_app_if_nonrpc  = dtv_common

ifeq "$(BUILD_PHASE)" "PHASE2.0"
DEPENDENCY_mtkapp  += handle_app rpc_ipc app_if_rpc
endif
ifeq "$(BUILD_PHASE)" "PHASE1.75"
DEPENDENCY_mtkapp  += app_if_nonrpc
DEPENDENCY_dtv_svc += app_if app_if_nonrpc
ifneq "$(MMP_SUPPORT)" "none"
DEPENDENCY_dtv_svc += cmpb
endif
endif




ifneq "$(LIBS_EXPAT)" ""
LINK_LIBS_expat       = -L$(LIBS_EXPAT_DIR) -l$(LIBS_EXPAT)
else
LINK_LIBS_expat       =
endif

ifeq "$(INET_SUPPORT)" "true"
LINK_LIBS_inet_dtv_svc= -L$(OPENSSL_LIB_PATH)  -lcrypto -lssl \
			-L$(NETUTIL_LIB_PATH)  -lnet_info

LINK_LIBS_inet        = -L$(OPENSSL_LIB_PATH)  -lcrypto -lssl \
                        -L$(LIBCURL_LIB_PATH)  -lcurl \
                        -L$(LIBCARES_LIB_PATH) -lcares \
                        -L$(NETUTIL_LIB_PATH)  -lnet_info -lring_buffer
                        
LINK_LIBS_dlna        = -L$(LIBS_DLNA_DIR)     $(addprefix -l,$(LIBS_DLNA))

LINK_LIBS_securestorage = -L$(LIBS_SECURESTORAGE_DIR)     $(addprefix -l,$(LIBS_SECURESTORAGE))
else
LINK_LIBS_inet_dtv_svc=
LINK_LIBS_inet        =
LINK_LIBS_dlna        =
LINK_LIBS_securestorage =
endif


LINK_LIBS_app_if     =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_app_if))))

ifneq "$(MMP_SUPPORT)" "none"
LINK_LIBS_cmpb       =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           $(LINK_LIBS_securestorage) \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_cmpb))))
endif
                                           
ifeq "$(BLUETOOTH)" "true"
LINK_LIBS_bluetooth  =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_bluetooth))))
endif
                                           
LINK_LIBS_dtv_common =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           -L$(DFB_LIB_DIR) -ldirectfb, \
                        $(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_dtv_common))))
LINK_LIBS_dtv_common += -L$(LIB_MTAL_DIR) -lmtal
LINK_LIBS_dtv_common += -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype
LINK_LIBS_tqtvd_atv_porting_layer =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/mng/$(MNG_VER)/pre-install/lib -lmng \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           -L$(DFB_LIB_DIR) -ldirectfb, \
                        $(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_tqtvd_atv_porting_layer))))

LINK_LIBS_app_if_rpc     = $(addprefix -l, $(DEPENDENCY_app_if_rpc))
LINK_LIBS_app_if_nonrpc  = $(addprefix -l, $(DEPENDENCY_app_if_nonrpc))

ifeq "$(SYS_MHEG5_SUPPORT)" "true"
LINK_LIBS_mheg5		= -L$(LIB_MHEG5_DIR)     $(addprefix -l,$(LIB_MHEG5))
endif

ifeq "$(INET_SUPPORT)" "true"
LINK_LIBS_dtv_common += $(LINK_LIBS_inet)
else
ifeq "$(CI_PLUS_SUPPORT)" "true"
LINK_LIBS_dtv_common += -L$(OPENSSL_LIB_PATH)  #-lcrypto -lssl #
#                        -L$(NETUTIL_LIB_PATH)  -lnet_info -lring_buffer
endif
endif

ifeq "$(RELEASE_3RD_LIB_DLNA)" "true"
LINK_LIBS_dtv_common += $(LINK_LIBS_dlna)
endif

ifeq "$(RELEASE_3RD_LIB_SECURESTORAGE)" "true"
#LINK_LIBS_dtv_common += $(LINK_LIBS_securestorage)
endif

ifeq "$(FW_UPG_SUPPORT)" "uli"
LIBS_UPDATETV_DIR ?= $(UPDATETV_LIB_PATH)
LINK_LIBS_updatetv += -L$(LIBS_UPDATETV_DIR) $(addprefix -l,$(LIBS_UPDATETV))
endif

ifeq "$(BLUETOOTH)" "true"
LINK_LIBS_alsa += -L$(LIBS_ALSA_DIR) $(addprefix -l,$(LIBS_ALSA))
LINK_LIBS_dbus += -L$(LIBS_DBUS_DIR) $(addprefix -l,$(LIBS_DBUS))
endif

ifeq "$(ENABLE_MUDFLAP_DTV_SVC)" "true"
LINK_LIBS_MUDFLAP = -fmudflapth -lmudflapth
LINK_LIBS_dtv_svc += $(LINK_LIBS_MUDFLAP)
endif
LINK_LIBS_dtv_svc += -L$(LIB_MTAL_DIR) -lmtal
LINK_LIBS_dtv_svc += -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype
ifeq "$(ENABLE_NAUTILUS_SUPPORT)" "true"
LINK_LIBS_dtv_svc += -L$(NAUTILUS_LIB_DIR) -lnp_nautilus -lhttp_nautilus -lnet_nautilus -ltl_cmn_nautilus -ltl_tcpip_nautilus
endif

ifeq "$(ENABLE_EFENCE_DTV_SVC)" "true"
LINK_LIBS_dtv_svc += -L$(EFENCE_LIB_DIR) -lefence
endif

ifeq "$(GINGA_SUPPORT)" "true"
LINK_LIBS_dtv_svc    +=  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/mng/$(MNG_VER)/pre-install/lib -lmng \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           $(LINK_LIBS_securestorage) \
                                           $(LINK_LIBS_inet_dtv_svc) \
                                           -L$(THIRDPARTY_LIB_ROOT)/ginga/cvm/lib -lcvm \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_dtv_svc))))
ifneq "$(GINGA_FAST_BOOT)" "true"
LINK_LIBS_dtv_svc    += $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/ginga/lib -ltqtvd_astronav -ltqtvd_astrotv -lxml2 ,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_dtv_svc))))
endif
else
LINK_LIBS_dtv_svc    +=  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           $(LINK_LIBS_securestorage) \
                                           $(LINK_LIBS_inet_dtv_svc) \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_dtv_svc))))
endif
 
LINK_LIBS_dtv_svc_if =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_dtv_svc_if))))
LINK_LIBS_handle_app =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_handle_app))))
LINK_LIBS_rpc_ipc    =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_rpc_ipc))))
LINK_LIBS_ut_sample  =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_ut_sample))))
LINK_LIBS_mtkapp     =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           $(LINK_LIBS_securestorage) \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_mtkapp))))
                                           
ifeq "$(NETTV_SUPPORT)" "true"
LINK_LIBS_mtkapp += -L$(THIRDPARTY_LIB_ROOT)/nettv/lib -lphilipsnettv -lECD -lsqlite3
endif
                                           
LINK_LIBS_validate_common     =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz $(LINK_LIBS_MUDFLAP) \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_validate_common))))
                                           
LINK_LIBS_validate_common += -L$(LIB_MTAL_DIR) -lmtal
LINK_LIBS_validate_common += -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype                                          
#LINK_LIBS_validate_network_detect     =  $(subst -ldirectfb,-L$(THIRDPARTY_LIB_ROOT)/png/$(PNG_VER)/pre-install/lib -lpng \
                                           -L$(THIRDPARTY_LIB_ROOT)/jpeg/$(JPEG_VER)/pre-install/lib -ljpeg \
                                           -L$(THIRDPARTY_LIB_ROOT)/freetype/$(FT_VER)/pre-install/lib -lfreetype \
                                           -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz \
                                           $(LINK_LIBS_securestorage) \
                                           -L$(DFB_LIB_DIR) -ldirectfb,$(LINK_LIBS_MUDFLAP) \
                                           $(subst -lpthread,-L$(CROSS_TOOLCHAIN_SYSROOT)/lib -lpthread,$(addprefix -l, $(DEPENDENCY_validate_network_detect))))
                                           #$(subst -lcurl,-L$(LIBCURL_LIB_PATH)/ -lcurl,$(addprefix -l, $(DEPENDENCY_validate_network_detect))))
                                           
#LINK_LIBS_validate_network_detect += -L$(LIBCURL_LIB_PATH) -lcurl
#LINK_LIBS_validate_network_detect +=  -L$(OPENSSL_LIB_PATH)  -lcrypto  
#LINK_LIBS_validate_network_detect +=  -L$(LIBCARES_LIB_PATH) -lcares 
#LINK_LIBS_validate_network_detect +=  -L$(THIRDPARTY_LIB_ROOT)/zlib/$(ZLIB_VER)/pre-install/lib -lz
LINK_LIBS_validate_common += $(LINK_LIBS_inet) $(LINK_LIBS_expat)

ifeq "$(SYS_MHP_SUPPORT)" "true"                          
LINK_LIBS_dtv_svc    += -L$(THIRDPARTY_LIB_ROOT)/mhp -lalticaptor
endif

#ifeq "$(STATIC_LIB)" "true"
ifeq "$(INET_SUPPORT)" "true"
LINK_LIBS_dtv_svc += $(LINK_LIBS_inet)
LINK_LIBS_mtkapp += $(LINK_LIBS_inet)
else
ifeq "$(CI_PLUS_SUPPORT)" "true"
LINK_LIBS_dtv_svc += -L$(OPENSSL_LIB_PATH)  #-lcrypto -lssl   #\
#                        -L$(NETUTIL_LIB_PATH)  -lnet_info -lring_buffer
LINK_LIBS_mtkapp  += -L$(OPENSSL_LIB_PATH)  #-lcrypto #-lssl   #\
#                        -L$(NETUTIL_LIB_PATH)  -lnet_info -lring_buffer
endif
endif
#endif






ifeq "$(SYS_MHEG5_SUPPORT)" "true"
LINK_LIBS_dtv_svc += $(LINK_LIBS_mheg5)
LINK_LIBS_dtv_svc += $(LINK_LIBS_mheg5)
LINK_LIBS_dtv_svc += $(LINK_LIBS_mheg5)
endif

ifeq "$(FW_UPG_SUPPORT)" "uli"
LINK_LIBS_dtv_svc += $(LINK_LIBS_updatetv)
LINK_LIBS_mtkapp += $(LINK_LIBS_updatetv)
endif

ifeq "$(BLUETOOTH)" "true"
LINK_LIBS_bluetooth += $(LINK_LIBS_alsa)
LINK_LIBS_bluetooth += $(LINK_LIBS_dbus)
LINK_LIBS_mtkapp += $(LINK_LIBS_bluetooth)
LINK_LIBS_dtv_svc += $(LINK_LIBS_bluetooth)
LINK_LIBS_validate_common += $(LINK_LIBS_bluetooth)
endif

ifneq "$(LINK_LIBS_expat)" ""
LINK_LIBS_dtv_svc += $(LINK_LIBS_expat)
ifeq "$(BUILD_PHASE)" "PHASE2.0"
LINK_LIBS_mtkapp += $(LINK_LIBS_expat)
endif
endif

ifeq "$(HBBTV_SUPPORT)" "true"
LINK_LIBS_dtv_svc += -L$(HBBTV_LIB_DIR)  -lgaliod
endif



#1. dtv_common, rpc_ipc, network_detect
#2. app_if, app_if_rpc, app_if_nonrpc, bluetooth,  dtv_svc_if, handle_app
#3. mtkapp, cmpb
#4. dtv_svc validate_common, validate_network_detect

PHASE2_LIB_STEP1 = dtv_common   rpc_ipc        usb_detect   
#network_detect

PHASE2_LIB_STEP2 = app_if app_if_rpc  dtv_svc_if handle_app
ifeq "$(BUILD_PHASE)" "PHASE1.75"
PHASE2_LIB_STEP2 += app_if_nonrpc
endif

ifeq "$(BLUETOOTH)" "true"
PHASE2_LIB_STEP2 += bluetooth
endif

#ifeq "$(BUILD_PHASE)" "PHASE1.75"
#PHASE2_LIB_STEP3 += mtkapp
#endif

ifneq "$(MMP_SUPPORT)" "none"
PHASE2_LIB_STEP3 += cmpb
endif

ifeq "$(GINGA_SUPPORT)" "true"
PHASE2_LIB_STEP4 += tqtvd_atv_porting_layer
endif

#ifeq "$(BUILD_PHASE)" "PHASE1.75"
#PHASE2_LIB_STEP4 += mtkapp
#endif 


ifneq "$(BUILD_PHASE)" "PHASE1.75"
PHASE2_EXEC_BIN    = dtv_svc       \
                     mtkapp
endif

### Added for BIVL
ifeq "$(BIVL_SUPPORT)" "true"
    LINK_LIBS_dtv_svc += -L$(THIRDPARTY_LIB_ROOT)/sony/bivl/lib -lbiv
    LINK_LIBS_dtv_svc += -L$(THIRDPARTY_LIB_ROOT)/sqlite3/lib -lsqlite_3_7_2
endif
 
ifeq "$(BUILD_PHASE)" "PHASE1.75"
PHASE2_EXEC_BIN    = dtv_svc
endif

ifeq "$(NO_MTK_APP)" "true"
PHASE2_EXEC_BIN    = dtv_svc
endif

PHASE2_EXEC_BIN += validate_common
#PHASE2_EXEC_BIN += validate_network_detect



PHASE2_LIB := $(PHASE2_LIB_STEP1) $(PHASE2_LIB_STEP2) $(PHASE2_LIB_STEP3) 
                     
PHASE2 = $(PHASE2_LIB) $(PHASE2_EXEC_BIN)

PHASE2_INIT               = $(addsuffix _init,      $(PHASE2))
PHASE2_CLEAN              = $(addprefix clean_,     $(PHASE2))
PHASE2_CLEAN_INIT         = $(addprefix clean_init_,$(PHASE2))


DYNAMIC_SUFFIX = .so
STATIC_SUFFIX  = .a
EXEC_SUFFIX    = .bin

PHASE2_DYNAMIC_LIB_NAME1   = $(addprefix lib,$(addsuffix $(DYNAMIC_SUFFIX),  $(PHASE2_LIB_STEP1)))
PHASE2_STATIC_LIB_NAME1    = $(addprefix lib,$(addsuffix $(STATIC_SUFFIX),   $(PHASE2_LIB_STEP1)))

PHASE2_DYNAMIC_LIB_NAME2   = $(addprefix lib,$(addsuffix $(DYNAMIC_SUFFIX),  $(PHASE2_LIB_STEP2)))
PHASE2_STATIC_LIB_NAME2    = $(addprefix lib,$(addsuffix $(STATIC_SUFFIX),   $(PHASE2_LIB_STEP2)))

PHASE2_DYNAMIC_LIB_NAME3   = $(addprefix lib,$(addsuffix $(DYNAMIC_SUFFIX),  $(PHASE2_LIB_STEP3)))
PHASE2_STATIC_LIB_NAME3    = $(addprefix lib,$(addsuffix $(STATIC_SUFFIX),   $(PHASE2_LIB_STEP3)))

PHASE2_DYNAMIC_LIB_NAME4   = $(addprefix lib,$(addsuffix $(DYNAMIC_SUFFIX),  $(PHASE2_LIB_STEP4)))
PHASE2_STATIC_LIB_NAME4    = $(addprefix lib,$(addsuffix $(STATIC_SUFFIX),  $(PHASE2_LIB_STEP4)))


PHASE2_DYNAMIC_LIB_NAME    = $(PHASE2_DYNAMIC_LIB_NAME1) $(PHASE2_DYNAMIC_LIB_NAME2) $(PHASE2_DYNAMIC_LIB_NAME3) $(PHASE2_DYNAMIC_LIB_NAME4)
PHASE2_STATIC_LIB_NAME     = $(PHASE2_STATIC_LIB_NAME1) $(PHASE2_STATIC_LIB_NAME2) $(PHASE2_STATIC_LIB_NAME3) $(PHASE2_STATIC_LIB_NAME4)

PHASE2_EXEC_NAME           = $(addsuffix $(EXEC_SUFFIX), $(PHASE2_EXEC_BIN))



#
# Set rpc desc header file
#
RPCD                    := $(PROJECT_ROOT)/tools/rpc_desc/rpcd
RPC_DESC_FILE_DIR       := $(PHASE2_OBJ_ROOT)/rpc_desc
RPC_DESC_FILE_BODY_TMP  := ri_desc_tmp
RPC_DESC_FILE_BODY      := ri_desc

#
#for archieve library
#
LIB_LIST =	$(wildcard $(AR_LIB_PATH)/*.content)

#
# EXTRA_LIBRARY
#
ifeq "$(MTK_WATCHDOG)" "true"
EXTRA_LIBRARY = -L$(OUTPUT_ROOT)/basic -lmtk_watchdog
else
EXTRA_LIBRARY = 
endif

vpath
vpath %.so  $(OUTPUT_ROOT)
vpath %.bin $(OUTPUT_ROOT)

ALL_PHASE2 = $(PHASE2) $(PHASE2_INIT) $(PHASE2_CLEAN) $(PHASE2_CLEAN_INIT) $(PHASE2_STATIC_LIB_NAME) p2_init clean_phase2 clean_phase2_all phase2_all cfg test1 clean_cfg rpc_desc clean_rpc_desc get_custom_name get_target_name get_KERNEL_VER get_RELEASE_3RD_LIB_LIST get_RELEASE_3RD_LIB_WIFI get_RELEASE_3RD_LIB_DLNA get_RELEASE_3RD_LIB_SECURESTORAGE get_3RD_FILE_128 get_SYS_MHEG5_SUPPORT get_RELEASE_3RD_SRC_LIST get_WIFI_DONGLE get_NO_MTK_APP get_ZIP_NATIVE_DRIVER get_ENABLE_VFP get_NO_SYM_DEBUG test_xls get_RELEASE_3RD_LIB_ULI get_RELEASE_3RD_LIB_BLUETOOTH get_CUSTOMER get_MODEL_NAME get_TARGET_IC get_COMPANY get_ENABLE_CA9 get_BRDCST_IF  phase2_libs clean_phase2_libs

ALL_PHASE2 += get_TOOL_CHAIN get_ENABLE_SMP get_KERNEL_VER_FOR_3RD get_OBJ_ROOT
ALL_PHASE2 += get_ANDROID_CUSTOMER

#
# Build the bits
#
.PHONY : p2_init link_c_inc link_x_inc cfg $(ALL_PHASE2) $(LIB_LIST) ar_lib

test_xls:
	$(AUTHOR_XLS_SCRIPT) $(AUTHOR_XLS_PARM)
	
phase2_all : p2_init
	$(call set_start_time_fct,'P2');
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) $(PHASE2_LIB_STEP1); \
	$(call if_file_exist_fct, $(ERROR_LOG), cat $(ERROR_LOG)|sed -r 's/;;;/\t/g'; exit 1;) 
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) $(PHASE2_LIB_STEP2); \
	$(call if_file_exist_fct, $(ERROR_LOG), cat $(ERROR_LOG)|sed -r 's/;;;/\t/g'; exit 1;) 
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) $(PHASE2_LIB_STEP3); \
	$(call if_file_exist_fct, $(ERROR_LOG), cat $(ERROR_LOG)|sed -r 's/;;;/\t/g'; exit 1;)
#	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) $(PHASE2_LIB_STEP4); \
#	$(call if_file_exist_fct, $(ERROR_LOG), cat $(ERROR_LOG)|sed -r 's/;;;/\t/g'; exit 1;)
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) $(PHASE2_EXEC_BIN); \
	$(call if_file_exist_fct, $(ERROR_LOG), cat $(ERROR_LOG)|sed -r 's/;;;/\t/g'; exit 1;) 
	
	@$(RM) $(RM_REC_FLAG) $(THIS_ROOT)/phase2_warning.result.log
	@for i in $(PHASE2); do \
		if [ -e $(THIS_ROOT)/$$i.warning.log ]; then \
			cat $(THIS_ROOT)/$$i.warning.log >> $(THIS_ROOT)/phase2_warning.result.log; \
		fi; \
		$(RM) $(RM_REC_FLAG) $(THIS_ROOT)/$$i.warning.log $(THIS_ROOT)/$$i.build.log; \
	done
	@$(call if_file_exist_fct, $(THIS_ROOT)/phase2_warning.result.log, echo -e "\n=========== Build Phase2 Warning Rst ===========\n";wc $(THIS_ROOT)/phase2_warning.result.log |awk '{print "Warning Count: " $$1}';echo -e "\n\n";)
ifeq "$(CHECK_WARNING_BUILD)" "true"
	$(AUTHOR_XLS_SCRIPT) $(AUTHOR_XLS_PARM)
endif
	$(call cal_exec_time_fct,'P2');

# only 2 build steps for phase2_libs target
phase2_libs : p2_init
	$(call set_start_time_fct,'P2');
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) $(PHASE2_LIB_STEP1); \
	$(call if_file_exist_fct, $(ERROR_LOG), cat $(ERROR_LOG)|sed -r 's/;;;/\t/g'; exit 1;) 
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) $(PHASE2_LIB_STEP2); \
	$(call if_file_exist_fct, $(ERROR_LOG), cat $(ERROR_LOG)|sed -r 's/;;;/\t/g'; exit 1;) 
	
	@$(RM) $(RM_REC_FLAG) $(THIS_ROOT)/phase2_warning.result.log
	@for i in $(PHASE2); do \
		if [ -e $(THIS_ROOT)/$$i.warning.log ]; then \
			cat $(THIS_ROOT)/$$i.warning.log >> $(THIS_ROOT)/phase2_warning.result.log; \
		fi; \
		$(RM) $(RM_REC_FLAG) $(THIS_ROOT)/$$i.warning.log $(THIS_ROOT)/$$i.build.log; \
	done
	@$(call if_file_exist_fct, $(THIS_ROOT)/phase2_warning.result.log, echo -e "\n=========== Build Phase2 Warning Rst ===========\n";wc $(THIS_ROOT)/phase2_warning.result.log |awk '{print "Warning Count: " $$1}';echo -e "\n\n";)
ifeq "$(CHECK_WARNING_BUILD)" "true"
	$(AUTHOR_XLS_SCRIPT) $(AUTHOR_XLS_PARM)
endif
	$(call cal_exec_time_fct,'P2');

p2_init: cfg  mk_OUTPUT_ROOT clean_log
ifneq "$(RLS_CUSTOM_BUILD)" "true"
	@$(MAKE) -C $(TARGET_ROOT)/app_if -f $(TARGET_ROOT)/customer_p2.mak rpc_desc --no-print-directory INC_CFG_MAK=TRUE
endif

cfg :
	@chmod +x $(GEN_CFG)
	@for i in `cat $(CFG_DEF) | grep "^[\t ]*\[*\["  | sed -r 's/\[//; s/\]//g; s/\&//g' | tr -d "\r"`  ; \
	do \
		if [ ! -d $(CFG_ROOT)/$$i ]; then \
			$(MKDIR) $(MKDIR_FLAG) $(CFG_ROOT)/$$i ; \
		fi; \
	done 
	$(GEN_CFG) -i $(THIS_ROOT)/$(CFG_DEF) -r $(CFG_ROOT)

link_c_inc:
	@if [ -d $(CUSTOM_ROOT)/c_inc ]; then $(RM) $(RM_REC_FLAG) $(CUSTOM_ROOT)/c_inc; fi;
	@$(call if_file_notexist_w_sym_fct, $(C_INC_DIR) ,$(CUSTOM_ROOT)/c_inc);

link_x_inc:
	@$(call if_file_notexist_w_sym_fct, $(X_INC_DIR) ,$(TARGET_ROOT)/x_inc);

clean_cfg :
	@chmod +x $(GEN_CFG)
	@$(GEN_CFG) -c $(CFG_DEF) -r $(CFG_ROOT) 1>/dev/null 2>/dev/null

clean_phase2 clean_phase2_all: clean_obj $(PHASE2_CLEAN) clean_init clean_cfg clean_rpc_desc clean_log
ifneq "$(RLS_CUSTOM_BUILD)" "true"
	@$(RM) $(RM_FLAG) $(RPC_DESC_FILE_DIR)
endif


	
clean_obj:
	@echo -e "  RM\tPHASE2 OBJ"
	@$(FCLEAN) $(PHASE2_OBJ_ROOT)
	
clean_init :$(PHASE2_CLEAN_INIT)

$(PHASE2_LIB): 
	@$(call set_start_time_fct,$@);
	@$(call if_file_exist_fct,$(ERROR_LOG), $(RM) $(RM_REC_FLAG) $(WARNING_LOG) $(ERROR_LOG);)
	@$(RM) $(RM_FLAG) $(THIS_ROOT)/$@.*.log $(THIS_ROOT)/$@.build.log
	@$(call if_dir_not_exist_fct,$(DBG_OUTPUT_ROOT), $(MKDIR) $(MKDIR_FLAG) $(DBG_OUTPUT_ROOT);)	
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(DBG_OUTPUT_ROOT)/ -f $(TARGET_ROOT)/customer_p2.mak $(addsuffix _init,$@) INC_CFG_MAK=TRUE CFG_FILE_DIR=$(PHASE2_CFG_FILE_ROOT)/$@
	@WARNING_LOG=$(THIS_ROOT)/$@.build.log; \
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(TARGET_ROOT)/$@ all INC_CFG_MAK=TRUE CFG_FILE_DIR=$(PHASE2_CFG_FILE_ROOT)/$@ WARNING_LOG=$(THIS_ROOT)/$@.build.log; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	@$(RM) $(RM_FLAG) $(THIS_ROOT)/$@.build.log
	@$(call if_file_exist_fct, $(ERROR_LOG), exit 1;)
	@$(MAKE) -f $(TARGET_ROOT)/customer_p2.mak ar_lib AR_LIB_PATH=$(call path_cnvt,$(TARGET_LIB_DIR)/$@)
	
ifeq "$(STATIC_LIB)" "true"
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(DBG_OUTPUT_ROOT)/ -f $(TARGET_ROOT)/customer_p2.mak ar_lib AR_LIB_PATH=$(TARGET_LIB_DIR)/$@
endif

ifneq "$(STATIC_LIB)" "true"
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(DBG_OUTPUT_ROOT)/ -f $(TARGET_ROOT)/customer_p2.mak $(addprefix lib,$(addsuffix $(DYNAMIC_SUFFIX),  $@)) INC_CFG_MAK=TRUE
	@$(LN) $(LN_FLAG) $(DBG_OUTPUT_ROOT)/$(addprefix lib,$(addsuffix $(DYNAMIC_SUFFIX),  $@)) $(THIS_ROOT)/
	@$(CP) $(CP_FLAG) $(DBG_OUTPUT_ROOT)/$(addprefix lib,$(addsuffix $(DYNAMIC_SUFFIX),  $@)) $(OUTPUT_SYMBOL_ROOT)/
else
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(DBG_OUTPUT_ROOT)/ -f $(TARGET_ROOT)/customer_p2.mak $(addprefix lib,$(addsuffix $(STATIC_SUFFIX),   $@)) INC_CFG_MAK=TRUE
	@$(LN) $(LN_FLAG) $(DBG_OUTPUT_ROOT)/$(addprefix lib,$(addsuffix $(STATIC_SUFFIX),  $@)) $(THIS_ROOT)/
	@$(CP) $(CP_FLAG) $(DBG_OUTPUT_ROOT)/$(addprefix lib,$(addsuffix $(STATIC_SUFFIX),  $@)) $(OUTPUT_SYMBOL_ROOT)/
endif
	@$(call cal_exec_time_fct,$@);


$(PHASE2_EXEC_BIN):
	@$(call set_start_time_fct,$@);
	@$(call if_file_exist_fct,$(ERROR_LOG), $(RM) $(RM_REC_FLAG) $(WARNING_LOG) $(ERROR_LOG);)
	@$(RM) $(RM_REC_FLAG) $(DBG_OUTPUT_ROOT)/$@.*.log $(THIS_ROOT)/$@.build.log
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(DBG_OUTPUT_ROOT)/ -f $(TARGET_ROOT)/customer_p2.mak $(addsuffix _init,$@) INC_CFG_MAK=TRUE CFG_FILE_DIR=$(PHASE2_CFG_FILE_ROOT)/$@
	@WARNING_LOG=$(THIS_ROOT)/$@.build.log; \
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(TARGET_ROOT)/$@ all INC_CFG_MAK=TRUE CFG_FILE_DIR=$(PHASE2_CFG_FILE_ROOT)/$@ WARNING_LOG=$(THIS_ROOT)/$@.build.log; \
	$(call set_build_chk_fct,$(CHK_ERR_WARN_SCRIPT),$(CHK_ERR_WARN_PARM))
	@$(RM) $(RM_FLAG) $(THIS_ROOT)/$@.build.log
	@$(call if_file_exist_fct, $(ERROR_LOG), exit 1;)
	@$(MAKE) -f $(TARGET_ROOT)/customer_p2.mak ar_lib AR_LIB_PATH=$(call path_cnvt,$(TARGET_LIB_DIR)/$@)

ifeq "$(STATIC_LIB)" "true"
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(DBG_OUTPUT_ROOT)/ -f $(TARGET_ROOT)/customer_p2.mak ar_lib AR_LIB_PATH=$(TARGET_LIB_DIR)/$@
	$(call if_dir_exist_fct, $(CUSTOM_LIB_DIR)/$@, $(MAKE) -C $(DBG_OUTPUT_ROOT)/ -f $(TARGET_ROOT)/customer_p2.mak ar_lib AR_LIB_PATH=$(CUSTOM_LIB_DIR)/$@;)
endif
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(DBG_OUTPUT_ROOT)/ -f $(TARGET_ROOT)/customer_p2.mak $(addsuffix $(EXEC_SUFFIX), $@) INC_CFG_MAK=TRUE
	@$(LN) $(LN_FLAG) $(DBG_OUTPUT_ROOT)/$@ $(THIS_ROOT)/
	@$(CP) $(CP_FLAG) $(DBG_OUTPUT_ROOT)/$@ $(OUTPUT_SYMBOL_ROOT)/

	@$(call if_file_exist_fct, $(WARNING_LOG),     mv $(WARNING_LOG) $(DBG_OUTPUT_ROOT)/$@.build.log;)
	@$(call if_file_exist_fct, $(WARNING_RST_LOG), mv $(WARNING_RST_LOG) $(DBG_OUTPUT_ROOT)/$@.warning.log;)
	@$(call if_file_exist_fct, $(DBG_OUTPUT_ROOT)/$@.warning.log, cat $(DBG_OUTPUT_ROOT)/$@.warning.log|sed -r 's/;;;/\t/g';)
	@$(call cal_exec_time_fct,$@);

$(PHASE2_INIT):
	@$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(TARGET_ROOT)/$(subst _init,,$@) phase2_init TARGET_COMPONENT=$(subst _init,,$@) INC_CFG_MAK=TRUE --no-print-directory -s

rpc_desc: clean_rpc_desc
	$(call if_dir_not_exist_fct,$(RPC_DESC_FILE_DIR), $(MKDIR) $(MKDIR_FLAG) $(RPC_DESC_FILE_DIR);)
	@$(LOCKFILE) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).c.lock
	@chmod +x $(RPCD)

	$(RPCD) \
    	-o$(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP) \
    	-i$(VM_LINUX_ROOT)/dtv_linux/project_x_linux/rpc/rpc_desc_input.h \
    	-s$(VM_LINUX_ROOT)/dtv_linux/project_x_linux/rpc/rpc_desc_input.h \
    	-I$(PROJECT_ROOT)/middleware \
    	-I$(PROJECT_ROOT)/middleware/inc \
    	-I$(PROJECT_ROOT)/../dtv_linux/rpc_ipc \
    	$(filter-out -DHEADER_COMMON_H%,$(DEFINES))	\
    	-D_CPU_LITTLE_ENDIAN_ \
    	-DHEADER_COMMON_H='\"inc\/u_common.h\"'

	@if [ ! -e $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY).c ]; then \
		$(CP) $(CP_FLAG) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).c $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY).c; \
		$(CP) $(CP_FLAG) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).h $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY).h; \
	else \
		if [ "`diff -q $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).c $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY).c -I "\(MTK RPC Description tool\)"`" != "" ]; then \
			echo "Update $(RPC_DESC_FILE_BODY).c $(RPC_DESC_FILE_BODY).h"; \
			$(CP) $(CP_FLAG) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).c $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY).c; \
			$(CP) $(CP_FLAG) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).h $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY).h; \
		fi \
	fi
	@$(RM) $(RM_FLAG) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).c
	@$(RM) $(RM_FLAG) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).h
	@$(RM) $(RM_FLAG) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).c.lock
	ln -sf $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY).c $(VM_LINUX_ROOT)/dtv_linux/project_x_linux/rpc/$(RPC_DESC_FILE_BODY).c
	ln -sf $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY).h $(VM_LINUX_ROOT)/dtv_linux/project_x_linux/rpc/$(RPC_DESC_FILE_BODY).h


clean_rpc_desc:
	@$(RM) $(RM_FLAG) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).c
	@$(RM) $(RM_FLAG) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).h
	@$(RM) $(RM_FLAG) $(VM_LINUX_ROOT)/dtv_linux/project_x_linux/rpc/$(RPC_DESC_FILE_BODY).c
ifneq "$(RLS_CUSTOM_BUILD)" "true"
	@$(RM) $(RM_FLAG) $(VM_LINUX_ROOT)/dtv_linux/project_x_linux/rpc/$(RPC_DESC_FILE_BODY).h
endif

$(PHASE2_CLEAN): 
	@echo -e "  RM\t$(subst clean_,,$@)"
	@$(FCLEAN) $(PHASE2_OBJ_ROOT)/$(subst clean_,,$@)
	@if [ ! -d $(THIS_ROOT)/$(subst clean_,,$@) ]; then \
		$(RM) $(RM_REC_FLAG) $(THIS_ROOT)/$(subst clean_,,$@); \
	fi
	@$(RM) $(RM_REC_FLAG) $(THIS_ROOT)/lib$(subst clean_,,$@).a
	@$(RM) $(RM_REC_FLAG) $(THIS_ROOT)/lib$(subst clean_,,$@).so	
	@$(RM) $(RM_REC_FLAG) $(DBG_OUTPUT_ROOT)/$(subst clean_,,$@).a
	@$(RM) $(RM_REC_FLAG) $(DBG_OUTPUT_ROOT)/$(subst clean_,,$@).bin
	@$(RM) $(RM_REC_FLAG) $(DBG_OUTPUT_ROOT)/$(subst clean_,,$@).log
	@$(RM) $(RM_REC_FLAG) $(DBG_OUTPUT_ROOT)/$(subst clean_,,$@).a.content
	@$(RM) $(RM_REC_FLAG) $(DBG_OUTPUT_ROOT)/*$(subst clean_,,$@)*  $(OUTPUT_ROOT)/*$(subst clean_,,$@)* $(THIS_ROOT)/lib$(subst clean_,,$@).so
	@$(RM) $(RM_FLAG) $(RPC_DESC_FILE_DIR)/$(RPC_DESC_FILE_BODY_TMP).c.lock
	
$(PHASE2_CLEAN_INIT):
	@$(MAKE) -C $(TARGET_ROOT)/$(subst clean_init_,,$@) clean_init INC_CFG_MAK=TRUE --no-print-directory -s

V ?= 0

ifeq "$(V)" "0"
QUIET = @
else
QUIET = 
endif

define rule_ld
	@echo -e "  LD\t$@"
	$(QUIET) $(LD) -shared $(GCC_LD_FLAG) -o $(DBG_OUTPUT_ROOT)/$@ $(DBG_OUTPUT_ROOT)/$< -L$(DBG_OUTPUT_ROOT)/  $($(subst lib,,$(addprefix LINK_LIBS_, $(subst $(DYNAMIC_SUFFIX),,$@))))
endef


define rule_mem_rom_map_ld
	@echo -e "  LD\t$@"
	$(QUIET) $(LD) -shared -Wl,-Map,$(DBG_OUTPUT_ROOT)/$@.map $(GCC_LD_FLAG) -o $(DBG_OUTPUT_ROOT)/$@ $(DBG_OUTPUT_ROOT)/$< -L$(DBG_OUTPUT_ROOT)/  $($(subst lib,,$(addprefix LINK_LIBS_, $(subst $(DYNAMIC_SUFFIX),,$@))))
endef

$(PHASE2_DYNAMIC_LIB_NAME): %.so : %.a.content

ifeq "$(MEM_ROM_MAP)" "true"
	$(rule_mem_rom_map_ld)
else
	$(rule_ld)
endif

	@touch $(DBG_OUTPUT_ROOT)/$@
	@$(CP) $(CP_FLAG) $(DBG_OUTPUT_ROOT)/$@ $(OUTPUT_ROOT)/
	@$(STRIP) $(STRIP_SO_FLAG) $(OUTPUT_ROOT)/$@
	@$(CP) $(CP_FLAG) $(OUTPUT_ROOT)/$@ $(OUTPUT_ROOT)/basic/
	@$(CP) $(CP_FLAG) $(OUTPUT_ROOT)/$@ $(THIRDPARTY_LIB_ROOT)/mtk
	
$(PHASE2_STATIC_LIB_NAME): %.a : %.a.content
	$(AR) $(AR_FLAG) $(DBG_OUTPUT_ROOT)/$@ `cat $(DBG_OUTPUT_ROOT)/$<`
	$(CP) $(CP_FLAG) $(DBG_OUTPUT_ROOT)/$@ $(OUTPUT_ROOT)/
	$(STRIP) $(STRIP_SO_FLAG) $(OUTPUT_ROOT)/$@
	$(CP) $(CP_FLAG) $(OUTPUT_ROOT)/$@ $(OUTPUT_ROOT)/basic/

	
-include $(DBG_OUTPUT_ROOT)/*.d $(AR_LIB_PATH)/*.d

define rule_exec_ld
	@echo -e "  LD\t$@"
	$(QUIET) $(LD) $(GCC_LD_FLAG_B) -o $(DBG_OUTPUT_ROOT)/$(subst $(EXEC_SUFFIX),,$@) $(DBG_OUTPUT_ROOT)/$< -L$(DBG_OUTPUT_ROOT)/ $($(addprefix LINK_LIBS_, $(subst $(EXEC_SUFFIX),,$@))) $(EXTRA_LIBRARY)
endef


define rule_exec_mem_rom_map_ld
	@echo -e "  LD\t$@"
	$(QUIET) $(LD) -Wl,-Map,$(DBG_OUTPUT_ROOT)/$@.map $(GCC_LD_FLAG_B) -o $(DBG_OUTPUT_ROOT)/$(subst $(EXEC_SUFFIX),,$@) $(DBG_OUTPUT_ROOT)/$< -L$(DBG_OUTPUT_ROOT)/ $($(addprefix LINK_LIBS_, $(subst $(EXEC_SUFFIX),,$@))) $(EXTRA_LIBRARY)
endef

$(PHASE2_EXEC_NAME): %.bin : %.a

ifeq "$(MEM_ROM_MAP)" "true"
	$(rule_mem_rom_map_ld)
else
	$(rule_exec_ld)
endif

	$(shell sleep 3)
	@touch $(DBG_OUTPUT_ROOT)/$(subst $(EXEC_SUFFIX),,$@)
	@$(CP) $(CP_FLAG) $(DBG_OUTPUT_ROOT)/$(subst $(EXEC_SUFFIX),,$@) $(OUTPUT_ROOT)/$(subst $(EXEC_SUFFIX),,$@)
	@$(STRIP) $(STRIP_EXE_FLAG) $(OUTPUT_ROOT)/$(subst $(EXEC_SUFFIX),,$@)
	@if [ "$@" == "dtv_svc.bin" ]; then \
		$(CP) $(CP_FLAG) $(OUTPUT_ROOT)/$(subst $(EXEC_SUFFIX),,$@) $(OUTPUT_ROOT)/basic/; \
	fi



mk_OUTPUT_ROOT :
	$(call if_dir_not_exist_fct,$(OUTPUT_ROOT)/basic, $(MKDIR) $(MKDIR_FLAG) $(OUTPUT_ROOT)/basic;)
	$(call if_dir_not_exist_fct,$(DBG_OUTPUT_ROOT),   $(MKDIR) $(MKDIR_FLAG) $(DBG_OUTPUT_ROOT);)
	
%.a:
	@sed -r 's/ /\n/g' $(TARGET_LIB_DIR)/$(basename $@)/lib$@.content |sort -u >  $(DBG_OUTPUT_ROOT)/$@.tmp	
	@for i in $($(shell echo $(subst lib,lib_,$(basename lib$@)) | tr '[:lower:]' '[:upper:]')); \
	do \
		echo -e "  LNK\t$(basename $@) + $$i"; \
		if [ -e "$(TARGET_LIB_DIR)/$(basename $@)/lib$$i.a.content" ]; then \
			sed -r 's/ /\n/g' $(TARGET_LIB_DIR)/$(basename $@)/lib$$i.a.content |sort -u >>  $(DBG_OUTPUT_ROOT)/$@.tmp; \
		else \
			if [ -e $(CUSTOM_LIB_DIR)/$(basename $@)/lib$$i.a.content ]; then \
				sed -r 's/ /\n/g' $(CUSTOM_LIB_DIR)/$(basename $@)/lib$$i.a.content |sort -u >>  $(DBG_OUTPUT_ROOT)/$@.tmp; \
			fi \
		fi \
	done
	@sed -r 's/ /\n/g' $(DBG_OUTPUT_ROOT)/$@.tmp |sort -u > $@.tmp2
	@echo "$(basename $@).bin: \\" > $(DBG_OUTPUT_ROOT)/$(basename $@).bin.d
	@cat $@.tmp2 |tr "\n" " " >> $(DBG_OUTPUT_ROOT)/$(basename $@).bin.d
	
	@sed '1i INPUT(' -i $@.tmp2
	@sed '$$a )' -i $@.tmp2
	@if [ ! -e $@ ]; then \
		$(CP) $(CP_FLAG) $@.tmp2 $@; \
	fi
	@if [ "`diff -q $@ $@.tmp2`" != "" ]; then \
		$(CP) $(CP_FLAG) $@.tmp2 $@; \
	fi
	@$(RM) $(RM_FLAG) $(DBG_OUTPUT_ROOT)/$@.tmp  $@.tmp2
	
%.a.content :
	@sed -r 's/ /\n/g' $(TARGET_LIB_DIR)/$(subst lib,,$(basename $(basename $@)))/$@ |sort -u >  $(DBG_OUTPUT_ROOT)/$@.tmp
	@for i in $($(shell echo $(subst lib,lib_,$(basename $(basename $@))) | tr '[:lower:]' '[:upper:]')); \
	do \
		echo -e "  LNK\t$(subst lib,,$(basename $(basename $@))) + $$i"; \
		if [ -e "$(TARGET_LIB_DIR)/$(subst lib,,$(basename $(basename $@)))/lib$$i.a.content" ]; then \
			sed -r 's/ /\n/g' $(TARGET_LIB_DIR)/$(subst lib,,$(basename $(basename $@)))/lib$$i.a.content |sort -u >>  $(DBG_OUTPUT_ROOT)/$@.tmp; \
		else \
			if [ -e $(CUSTOM_LIB_DIR)/$(subst lib,,$(basename $(basename $@)))/lib$$i.a.content ]; then \
				sed -r 's/ /\n/g' $(CUSTOM_LIB_DIR)/$(subst lib,,$(basename $(basename $@)))/lib$$i.a.content |sort -u >>  $(DBG_OUTPUT_ROOT)/$@.tmp; \
			fi \
		fi \
	done
	@sed -r 's/ /\n/g' $(DBG_OUTPUT_ROOT)/$@.tmp |sort -u > $(DBG_OUTPUT_ROOT)/$@

	@sed -r 's/ /\n/g' $(DBG_OUTPUT_ROOT)/$@.tmp |sort -u > $@.tmp2
	@echo "$(basename $(basename $@)).so: \\" > $(DBG_OUTPUT_ROOT)/$(basename $(basename $@)).so.d
	@cat $@.tmp2 |tr "\n" " " >> $(DBG_OUTPUT_ROOT)/$(basename $(basename $@)).so.d
	
ifneq "$(STATIC_LIB)" "true"
	@sed '1i INPUT(' -i $@.tmp2
	@sed '$$a )' -i $@.tmp2
endif	
	@if [ ! -e $@ ]; then \
		$(CP) $(CP_FLAG) $@.tmp2 $@; \
	fi
	@if [ "`diff -q $@ $@.tmp2`" != "" ]; then \
		$(CP) $(CP_FLAG) $@.tmp2 $@; \
	fi
	@$(RM) $(RM_FLAG) $(DBG_OUTPUT_ROOT)/$@.tmp  $@.tmp2

ar_lib :$(LIB_LIST) 
	@echo "Archieve Library Done"

$(LIB_LIST) :
	@echo "$(basename $@): \\" > $(basename $@).d
	@cat $@ |tr "\n" " " >> $(basename $@).d
	@sh $(TOOL_ROOT)/archieve_lib/ar_lib.sh $(AR) $(AR_FLAG) $(basename $@) $@

test1 :
	#$(shell echo $@ | tr '[:lower:]' '[:upper:]')
	echo "TARGET=$(TARGET)"
	echo "TARGET_ROOT=$(TARGET_ROOT)"

get_ANDROID_CUSTOMER:
	@echo "$(ANDROID_CUSTOMER)"

get_WIFI_DONGLE:
	@echo "$(WIFI_DONGLE)"
	
get_NO_MTK_APP:
	@echo "$(NO_MTK_APP)"

get_phase2_target:
	@echo "$(ALL_PHASE2)"

get_custom_name:
	@echo $(CUSTOM)
	
get_target_name:
	@echo $(TARGET)

get_ENABLE_SMP:
	@echo $(ENABLE_SMP)

get_KERNEL_VER_FOR_3RD:
	@echo $(KERNEL_VER_FOR_3RD)

get_KERNEL_VER:
	@echo $(KERNEL_VER)

get_RELEASE_3RD_SRC_LIST:
	@echo $(RELEASE_3RD_SRC_LIST)

get_RELEASE_3RD_LIB_LIST:
	@echo $(RELEASE_3RD_LIB_LIST)

get_RELEASE_3RD_LIB_WIFI:
	@echo $(RELEASE_3RD_LIB_WIFI)

get_RELEASE_3RD_LIB_DLNA:
	@echo $(RELEASE_3RD_LIB_DLNA)

get_RELEASE_3RD_LIB_SECURESTORAGE:
	@echo $(RELEASE_3RD_LIB_SECURESTORAGE)

get_3RD_FILE_128:
	@echo $(3RD_FILE_128)

get_SYS_MHEG5_SUPPORT:
	@echo $(SYS_MHEG5_SUPPORT)

get_CUSTOMER:
	@echo $(CUSTOMER)

get_MODEL_NAME:
	@echo $(MODEL_NAME)

get_TARGET_IC:
	@echo $(TARGET_IC)
	
get_ZIP_NATIVE_DRIVER:
	@echo $(ZIP_NATIVE_DRIVER)

get_ENABLE_VFP:
	@echo $(ENABLE_VFP)

get_ENABLE_CA9:
	@echo $(ENABLE_CA9)
	
get_NO_SYM_DEBUG:
	@echo $(NO_SYM_DEBUG)

get_RELEASE_3RD_LIB_ULI:
	@echo $(RELEASE_3RD_LIB_ULI)

get_COMPANY:
	@echo $(COMPANY)

get_BRDCST_IF:
	@echo $(BRDCST_IF)


get_RELEASE_3RD_LIB_BLUETOOTH:
	@echo $(RELEASE_3RD_LIB_BLUETOOTH)

get_TOOL_CHAIN:
	@echo $(TOOL_CHAIN)
	
get_OBJ_ROOT:
	@echo $(OBJ_ROOT)

remove_lock : clean_log
	@echo "REMOVE LIBRARY LOCKFILE"
	$(call if_dir_exist_fct,$(ALL_TARGET_LIB_DIR), find $(ALL_TARGET_LIB_DIR)/ -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)
	$(call if_dir_exist_fct,$(ALL_MW_LIB_DIR),     find $(ALL_MW_LIB_DIR)/ -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)
	$(call if_dir_exist_fct,$(ALL_CUSTOM_LIB_DIR), find $(ALL_CUSTOM_LIB_DIR)/ -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)
	$(call if_dir_exist_fct,$(TARGET_ROOT)/$(TARGET_DRIVER)/drv_inc, find $(TARGET_ROOT)/$(TARGET_DRIVER)/drv_inc/ -name *.lock -exec $(RM) $(RM_REC_FLAG) {} \;;)

clean_log :
	@$(RM) $(RM_REC_FLAG) $(WARNING_LOG) $(ERROR_LOG) *.log.group *.result.log *.result.xls phase2_warning.result.* build_*.log

