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
# $Revision: #2 $
# $Date: 2009/11/25 $
# $Author: jessica.wu $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

THIS_COMPONENT = amrwb

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../..
endif

SRC = awbif.c \
armdsp_gcc.c \
		audip_exp.c \
	 awb_dspfns_m_extend.c \
	 awb_dspfns_m.c \
 		wbamr_agc2.c \
   wbamr_api.c \
   wbamr_autocorr.c \
   wbamr_az_isp.c \
   wbamr_basicop2.c \
   wbamr_bits.c \
   wbamr_c2t64fx.c \
   wbamr_c4t64fx.c \
   wbamr_cod_main.c \
   wbamr_convolve.c \
   wbamr_cor_h_x.c \
   wbamr_d2t64fx.c \
   wbamr_d4t64fx.c \
   wbamr_d_gain2.c \
   wbamr_dec_homing.c \
   wbamr_dec_main.c \
   wbamr_decim54.c \
   wbamr_deemph.c \
   wbamr_dtx.c \
   wbamr_enc_homing.c \
   wbamr_g_pitch.c \
   wbamr_gpclip.c \
   wbamr_hp6k.c \
   wbamr_hp7k.c \
   wbamr_hp50.c \
   wbamr_hp400.c \
   wbamr_hp_wsp.c \
   wbamr_int_lpc.c \
   wbamr_isfextrp.c \
   wbamr_isp_az.c \
   wbamr_isp_isf.c \
   wbamr_lag_wind.c \
   wbamr_lagconc.c \
   wbamr_levinson.c \
   wbamr_log2.c \
   wbamr_lp_dec2.c \
   wbamr_math_op.c \
   wbamr_norm_corr.c \
   wbamr_oper_32b.c \
   wbamr_p_med_ol.c \
   wbamr_ph_disp.c \
   wbamr_pit_shrp.c \
   wbamr_pitch_f4.c \
   wbamr_pred_lt4.c \
   wbamr_preemph.c \
   wbamr_q_gain2.c \
   wbamr_q_pulse.c \
   wbamr_qisf_ns.c \
   wbamr_qpisf_2s.c \
   wbamr_random.c \
   wbamr_residu.c \
   wbamr_rom.c \
   wbamr_scale.c \
   wbamr_sid_sync.c \
   wbamr_syn_filt.c \
   wbamr_updt_tar.c \
   wbamr_util.c \
   wbamr_voicefac.c \
   wbamr_wb_vad.c \
   wbamr_weight_a.c


OBJ =

SUB_COMPONENTS =

DEFINES += -DCC_MTK_PLATFORM -DNO_IPCOMMON -D__arm__

CC_INC += -I../include -I./include

ccflags-y +=  -fno-builtin-round


#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak