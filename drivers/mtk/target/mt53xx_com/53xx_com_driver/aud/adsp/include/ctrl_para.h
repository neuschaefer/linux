/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *
 * $Author: kai.wang $
 * $Date: 2012/10/23 $
 * $RCSfile: ctrl_para.h,v $
 * $Revision: #7 $
 *
 *---------------------------------------------------------------------------*/

/** @file ctrl_para.h
 *  Brief of file ctrl_para.h.
 *  RISC-Controlled DSP Parameter Definition Header File
 */

#ifndef CTRL_PARA_H
#define CTRL_PARA_H

#ifdef AUD_EXTERN_FILE
#error "Please don't include this header file in release C file"
#endif

/*===================== Common Page ========================*/
#include "x_lint.h"
#include "rc2d_int.h"
#include "d2rc_shm.h"
#include "dsp_reg.h"
LINT_EXT_HEADER_BEGIN

//===================================================================
// DRAM Section Layout
//===================================================================
// Common Normal
#define DRAM_NORMAL_BASE                        (0x0000)
// Common Compact
#define DRAM_CMPT_BASE                          (0x0000)

/************************< Normal DRAM Allocation >*****************/
// DSP-RISC Shared Section for Transferring
#define ADDR_D2RC_SHARED_INFO                   (DRAM_NORMAL_BASE + 0x0000)
// RISC-DSP Shared Section for Transferring
#define ADDR_RC2D_SHARED_MEMORY                 (DRAM_NORMAL_BASE + 0x0100)
#define ADDR_D2RC_SHARED_INFO_DEC3              (DRAM_NORMAL_BASE + 0x0200)
#define ADDR_RC2D_SHARED_MEMORY_DEC3            (DRAM_NORMAL_BASE + 0x0280)

/* first decoder part */
#define ADDR_RC2D_DRAM_MIXSOUND_BASE            (DRAM_NORMAL_BASE + 0x0320)
#define ADDR_RC2D_DRAM_BASS_MANAGEMENT_BASE     (DRAM_NORMAL_BASE + 0x0430)
#define ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE    (DRAM_NORMAL_BASE + 0x04B0)
#define ADDR_RC2D_DRAM_PTS_STC_BASE             (DRAM_NORMAL_BASE + 0x04E0)
#define ADDR_RC2D_DRAM_EQUALIZER_BASE           (DRAM_NORMAL_BASE + 0x0500)
#define ADDR_RC2D_AUDIO_BASIC_BASE              (DRAM_NORMAL_BASE + 0x06E0)
#define ADDR_RC2D_DRAM_VS_BUF_BASE              (DRAM_NORMAL_BASE + 0x06F0)
#define ADDR_RC2D_SUPER_BASS_BASE               (DRAM_NORMAL_BASE + 0x07F0)
#define ADDR_RC2D_DRAM_UPMIX_BASE               (DRAM_NORMAL_BASE + 0x0850)

/* second decoder part */
#define ADDR_RC2D_DRAM_BASS_MANAGEMENT_BASE_DEC2    (DRAM_NORMAL_BASE + 0x0950)
#define ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2        (DRAM_NORMAL_BASE + 0x09C0)
#define ADDR_RC2D_AUDIO_BASIC_BASE_DEC2         (DRAM_NORMAL_BASE + 0x09E0)
#define ADDR_RC2D_AVC_BASE                      (DRAM_NORMAL_BASE + 0x09F0)
#define ADDR_RC2D_LIMITER_BASE                  (DRAM_NORMAL_BASE + 0x0A50)
#define ADDR_RC2D_SILENCE_BASE                  (DRAM_NORMAL_BASE + 0x0A70)
#define ADDR_DSP_DELAY_OFFSET                   (DRAM_NORMAL_BASE + 0x0A90)  //QUERY_DELAY
#define ADDR_RC2D_OPL_FUNC_VCT_BASE             (DRAM_NORMAL_BASE + 0x0AA0)
#define ADDR_RC2D_DRAM_PTS_STC_BASE_DEC3        (DRAM_NORMAL_BASE + 0x0AE0)

#define ADDR_RC2D_DRAM_MIXSOUND_BASE2           (DRAM_NORMAL_BASE + 0x0C00) //0x8C00~0x8C80

#define ADDR_RC2D_DRAM_KEYSHIFT_BASE            (DRAM_NORMAL_BASE + 0x1300) //0x9300~0x9400
// DDT
#define ADDR_RC2D_DRAM_DDT_DOWNMIX              (DRAM_NORMAL_BASE + 0x13B0) //0x13b0~0x13c0

#define ADDR_RC2D_OPL_FUNC_NORMAL_BASE          (DRAM_NORMAL_BASE + 0x6000) //0x6000~0x8000 reserve 0x2000 space for normal dram

/************************< Compact DRAM Allocation >****************/
/* first decoder part */
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define CMPT_POST_TEMP_BUFFER                   (DRAM_CMPT_BASE + 0x07400)
#define CMPT_UPMIX_BUFFER                       (DRAM_CMPT_BASE + 0x07800)
#define CMPT_DONWMIX_BUFFER                     (DRAM_CMPT_BASE + 0x07900)
#define CMPT_VSURR_DELAY_BUFFER                 (DRAM_CMPT_BASE + 0x07B00)
#define DRAM_OPL_FUNC_CMPT_BASE                 (DRAM_CMPT_BASE + 0x026860)

#define DRAM_INTERNAL_BUF2                      (DRAM_CMPT_BASE + 0x600)
#define DRAM_INTERNAL_BUF2_SIZE                 (0x2880)
#define DRAM_INTERNAL_BUF3                      (DRAM_CMPT_BASE + 0x6600)
#define DRAM_INTERNAL_BUF3_SIZE                 (0xD80)
#define DRAM_INTERNAL_BUF                       (DRAM_CMPT_BASE + 0x37180)
#define DRAM_INTERNAL_BUF_SIZE                  (0x5100)
#define DRAM_INTERNAL_BUF4                      (DRAM_CMPT_BASE + 0x5E520)
#define DRAM_INTERNAL_BUF4_SIZE                 (0x2880)
#define DRAM_INTERNAL_BUF5                      (DRAM_CMPT_BASE + 0x60DA0)
#define DRAM_INTERNAL_BUF5_SIZE                 (0xD80)
#define DRAM_INTERNAL_BUF6                      (DRAM_CMPT_BASE + 0x61B20)
#define DRAM_INTERNAL_BUF6_SIZE                 (0xD80)
    #else
#define CMPT_POST_TEMP_BUFFER                   (DRAM_CMPT_BASE + 0x04200)
#define CMPT_UPMIX_BUFFER                       (DRAM_CMPT_BASE + 0x04600)
#define CMPT_DONWMIX_BUFFER                     (DRAM_CMPT_BASE + 0x04700)
#define CMPT_VSURR_DELAY_BUFFER                 (DRAM_CMPT_BASE + 0x04900)
#define DRAM_OPL_FUNC_CMPT_BASE                 (DRAM_CMPT_BASE + 0x020A60)
#define DRAM_HPF_BUFFER                          (DRAM_CMPT_BASE + 0x020A60)

#define DRAM_INTERNAL_BUF2                      (DRAM_CMPT_BASE + 0x300)
#define DRAM_INTERNAL_BUF2_SIZE                 (0x2880)
#define DRAM_INTERNAL_BUF3                      (DRAM_CMPT_BASE + 0x3480)
#define DRAM_INTERNAL_BUF3_SIZE                 (0xD80)
#define DRAM_INTERNAL_BUF                       (DRAM_CMPT_BASE + 0xD000)
#define DRAM_INTERNAL_BUF_SIZE                  (0x2880)
#define DRAM_INTERNAL_BUF4                      (DRAM_CMPT_BASE + 0x37400)
#define DRAM_INTERNAL_BUF4_SIZE                 (0x2880)
    #endif

#define DRAM_SRS_LMT_BUF                        (DRAM_CMPT_BASE + 0x52E00) //len 0x480
//===================================================================
// ADDR_RC2D_SHARED_MEMORY Section (08100h~08200h, 100h)
//===================================================================
// general section //
#define ADDR_RC2D_CTRL1_BASE                    (ADDR_RC2D_SHARED_MEMORY + 0x0000)
#define ADDR_RC2D_TRAP_BASE                     (ADDR_RC2D_SHARED_MEMORY + 0x001F)
// foreground decoder section
#define ADDR_RC2D_CTRL2_BASE                    (ADDR_RC2D_SHARED_MEMORY + 0x0030)
#define ADDR_RC2D_CTRL3_BASE                    (ADDR_RC2D_SHARED_MEMORY + 0x0040)
#define ADDR_RC2D_DECODER_INFO_BASE             (ADDR_RC2D_SHARED_MEMORY + 0x0050)
#define ADDR_RC2D_MISC_BASE                     (ADDR_RC2D_SHARED_MEMORY + 0x0060)
// background decoder section
#define ADDR_RC2D_CTRL4_BASE                    (ADDR_RC2D_SHARED_MEMORY + 0x0080)
#define ADDR_RC2D_CTRL5_BASE                    (ADDR_RC2D_SHARED_MEMORY + 0x0090)
#define ADDR_RC2D_DECODER_INFO_BASE_DEC2        (ADDR_RC2D_SHARED_MEMORY + 0x00A0)
#define ADDR_RC2D_MISC_BASE_DEC2                (ADDR_RC2D_SHARED_MEMORY + 0x00B0)
#define ADDR_RC2D_ATV_CTRL_BASE                 (ADDR_RC2D_SHARED_MEMORY + 0x00D0)

/************************< Normal DRAM Definition >****************************/

//===================================================================
// ADDR_RC2D_SHARED_MEMORY Section (08100h~08200h)
//===================================================================

//===================================================================
// Ctrl_1 variable DRAM (08100h~0811Fh, 1Fh)
//===================================================================
#define ADDR_RC2D_CTRL_1_NUM_1                  (ADDR_RC2D_CTRL1_BASE + 0x000)
// Interrupt Control
#define ADDR_RC2D_INTERRUPT_CTRL                (ADDR_RC2D_CTRL1_BASE + 0x001)
#define ADDR_RC2D_SYSTEM_CTRL                   (ADDR_RC2D_CTRL1_BASE + 0x002)
    #define SHM_SYSTEM_CTRL_DUAL_DEC_MODE       (UINT16)(0x01 << 0)
    #define SHM_INBUF_SRC_SEL                   (UINT16)(0x01 << 1) // 0: MAIN 1: AUX
    #define SHM_SYSTEM_CTRL_TRI_DEC_MODE        (UINT16)(0x01 << 3)
    #define SHM_INBUF4_SRC_SEL                  (UINT16)(0x01 << 5) /// 0: MAIN 1: AUX, MP3ENC_SUPPORT
#define ADDR_RC2D_SYSTEM_CTRL2                  (ADDR_RC2D_CTRL1_BASE + 0x003)
    #define SHM_SYSTEM_CTRL_4TH_DEC_MODE        (UINT16)(0x01 << 0)
    #define SHM_ASRC_HALF_PLL_MODE              (UINT16)(0x01 << 2)
    #define SHM_SYSTEM_CTRL2_AUX_MIX_MAIN_MODE  (UINT16)(0x01 << 3)
    #define SHM_ASRC_TRACKING_MODE              (UINT16)(0x01 << 4)
#define ADDR_RC2D_SYSTEM_SETUP                  (ADDR_RC2D_CTRL1_BASE + 0x004)
#define ADDR_RC2D_SYSTEM_SETUP_DEC2             (ADDR_RC2D_CTRL1_BASE + 0x005)
#define ADDR_RC2D_CH_TRIM                       (ADDR_RC2D_CTRL1_BASE + 0x006)  // 11
#define ADDR_RC2D_CH_TRIM_CH9                   (ADDR_RC2D_CTRL1_BASE + 0x00E)
#define ADDR_RC2D_CH_TRIM_CH10                  (ADDR_RC2D_CTRL1_BASE + 0x00F)
#define ADDR_RC2D_CH_TRIM_LFE                   (ADDR_RC2D_CTRL1_BASE + 0x010)
#define ADDR_RC2D_DIALOGUE_GAIN                 (ADDR_RC2D_CTRL1_BASE + 0x011)
#define ADDR_RC2D_DIALOGUE_GAIN_DEC2            (ADDR_RC2D_CTRL1_BASE + 0x012)
#define ADDR_RC2D_CH_TRIM_DEC2_L                (ADDR_RC2D_CTRL1_BASE + 0x013)
#define ADDR_RC2D_CH_TRIM_DEC2_R                (ADDR_RC2D_CTRL1_BASE + 0x014)
#define ADDR_RC2D_CH_TRIM_DEC2_LFE              (ADDR_RC2D_CTRL1_BASE + 0x015)
#define ADDR_RC2D_SYSTEM_SETUP_DEC3             (ADDR_RC2D_CTRL1_BASE + 0x016)
#define ADDR_RC2D_DIALOGUE_GAIN_DEC3            (ADDR_RC2D_CTRL1_BASE + 0x017)
#define ADDR_RC2D_CH_TRIM_DEC3_L                (ADDR_RC2D_CTRL1_BASE + 0x018)
#define ADDR_RC2D_CH_TRIM_DEC3_R                (ADDR_RC2D_CTRL1_BASE + 0x019)
#define ADDR_RC2D_PLAY_SPEED                    (ADDR_RC2D_CTRL1_BASE + 0x01A)
#define ADDR_RC2D_SYSTEM_SETUP_DEC4             (ADDR_RC2D_CTRL1_BASE + 0x01B)
// Block copy end (0x1c variables)
#define ADDR_RD2D_INTERNAL_BUF_IN_FLAG          (ADDR_RC2D_CTRL1_BASE + 0x01C)
#define ADDR_RD2D_INTERNAL_BUF2_IN_FLAG         (ADDR_RC2D_CTRL1_BASE + 0x01D)
#define ADDR_RD2D_INTERNAL_BUF3_IN_FLAG         (ADDR_RC2D_CTRL1_BASE + 0x01E)

//===================================================================
// Trap Related (ROM Code) Section (0811Fh~08130h, 11h)
//===================================================================
#define ADDR_RC2D_TRAP_BPEN                     (ADDR_RC2D_TRAP_BASE + 0x000)
    #define TRAP_BPEN                           0xFF00
#define ADDR_RC2D_TRAP_BP0                      (ADDR_RC2D_TRAP_BASE + 0x001)
#define ADDR_RC2D_TRAP_BPT0                     (ADDR_RC2D_TRAP_BASE + 0x002)
    #define TRAP_BP0                            0x0000
    #define TRAP_BPT0                           0x0000
#define ADDR_RC2D_TRAP_BP1                      (ADDR_RC2D_TRAP_BASE + 0x003)
#define ADDR_RC2D_TRAP_BPT1                     (ADDR_RC2D_TRAP_BASE + 0x004)
    #define TRAP_BP1                            0x0000
    #define TRAP_BPT1                           0x0000
#define ADDR_RC2D_TRAP_BP2                      (ADDR_RC2D_TRAP_BASE + 0x005)
#define ADDR_RC2D_TRAP_BPT2                     (ADDR_RC2D_TRAP_BASE + 0x006)
    #define TRAP_BP2                            0x0000
    #define TRAP_BPT2                           0x0000
#define ADDR_RC2D_TRAP_BP3                      (ADDR_RC2D_TRAP_BASE + 0x007)
#define ADDR_RC2D_TRAP_BPT3                     (ADDR_RC2D_TRAP_BASE + 0x008)
    #define TRAP_BP3                            0x0000
    #define TRAP_BPT3                           0x0000
#define ADDR_RC2D_TRAP_BP4                      (ADDR_RC2D_TRAP_BASE + 0x009)
#define ADDR_RC2D_TRAP_BPT4                     (ADDR_RC2D_TRAP_BASE + 0x00A)
    #define TRAP_BP4                            0x0000
    #define TRAP_BPT4                           0x0000
#define ADDR_RC2D_TRAP_BP5                      (ADDR_RC2D_TRAP_BASE + 0x00B)
#define ADDR_RC2D_TRAP_BPT5                     (ADDR_RC2D_TRAP_BASE + 0x00C)
    #define TRAP_BP5                            0x0000
    #define TRAP_BPT5                           0x0000
#define ADDR_RC2D_TRAP_BP6                      (ADDR_RC2D_TRAP_BASE + 0x00D)
#define ADDR_RC2D_TRAP_BPT6                     (ADDR_RC2D_TRAP_BASE + 0x00E)
    #define TRAP_BP6                            0x0000
    #define TRAP_BPT6                           0x0000
#define ADDR_RC2D_TRAP_BP7                      (ADDR_RC2D_TRAP_BASE + 0x00F)
#define ADDR_RC2D_TRAP_BPT7                     (ADDR_RC2D_TRAP_BASE + 0x010)
    #define TRAP_BP7                            0x0000
    #define TRAP_BPT7                           0x0000

//===================================================================
// Ctrl_2 variable DRAM (08130h~08140h, 10h)
//===================================================================
#define ADDR_RC2D_CTRL_2_NUM_1                  (ADDR_RC2D_CTRL2_BASE + 0x000)
#define ADDR_RC2D_STC_DIFF                      (ADDR_RC2D_CTRL2_BASE + 0x001)
#define ADDR_RC2D_SPEED                         (ADDR_RC2D_CTRL2_BASE + 0x002)
/*-------------------------------------------------------
; Processing mode
;   bit 0:  EQ Spectrum ON/OFF
;   bit 1:  DRC ON/OFF
;   bit 2:  Karaoke Scoring Path in RAM
;   bit 3:  Auto DRC ON/OFF

;   bit 4:  Bass Management C,LS,RS Mix-to LR(0)/LFE(1)
;   bit 5:  _bass_ch_mixto_lfe in PRAM
;   bit 6:  Lfe input gain -5 db (0) or 0 db (1)
;   bit 7:  LFE redirect to large speaker

;   bit 8:  Bassmanagement always do IIR on LFE
;   bit 9:  L/R is not the same as Down-Mix Output
;   bit 10: Down-Mix to LT/RT
;   bit 11: Down-Mix 2ch for LG

;   bit 12: Bass Management Bypass Mode
;   bit 13: Post Processing Bypass Mode
;   bit 14: Trim Out Bypass Mode
;   bit 15: Delay Bypass Mode
-------------------------------------------------------*/
#define ADDR_RC2D_PROCESSING_MODE               (ADDR_RC2D_CTRL2_BASE + 0x003)
    #define BM_CLRS_MIX_TO_LFE                  (0x1 << 4)
    #define BASS_CH_MIX_TO_LFE                  (0x1 << 5)
    #define LFE_INPUT_GAIN_0_DB                 (0x1 << 6)
    #define LFE_REDIRECT_TO_LARGE_SPKR          (0x1 << 7)
    #define LFE_ALWAYS_DO_IIR                   (0x1 << 8)
#define ADDR_RC2D_BIT_RESOLUTION                (ADDR_RC2D_CTRL2_BASE + 0x004)
// L/R Channel Mixing and Microphone Level Threshold
// bit 0: L/R Mixing
// bit 1: Voice Mute
// bit 2: Vocal Assist
#define ADDR_RC2D_KARAOKE_FLAG                  (ADDR_RC2D_CTRL2_BASE + 0x005)
// Value: 0x800000~0
#define ADDR_RC2D_LRCH_MIX_RATIO                (ADDR_RC2D_CTRL2_BASE + 0x006)
// Block copy end (0x6 variables)
#define ADDR_RC2D_MASTER_VOLUME                 (ADDR_RC2D_CTRL2_BASE + 0x007)
#define ADDR_RC2D_SPDIF_FLAG                    (ADDR_RC2D_CTRL2_BASE + 0x008)
#define ADDR_RC2D_MASTER_VOLUME_BACKUP          (ADDR_RC2D_CTRL2_BASE + 0x009)
#define ADDR_RC2D_DECODING_STR_PNT              (ADDR_RC2D_CTRL2_BASE + 0x00A)
#define ADDR_RD2D_PCM_LINE_IN_LPF_FLAG          (ADDR_RC2D_CTRL2_BASE + 0x00C)  /// 0813Ch, CC_AUD_PCM_LINE_IN_LPF
#define ADDR_RC2D_AD_FADE_VALUE                 (ADDR_RC2D_CTRL2_BASE + 0x0E)
//===================================================================
// Ctrl_3 variable DRAM (08140h~08150h, 10h)
//===================================================================
#define DSP_RC2D_CTRL3_AREA_SZ                  (0x10)
#define ADDR_RC2D_CTRL_3_NUM_1                  (ADDR_RC2D_CTRL3_BASE + 0x000)
#define ADDR_RC2D_BONDING_OPTIONS               (ADDR_RC2D_CTRL3_BASE + 0x001)   // 30
// Channel Use -> Pink Noise
#define ADDR_RC2D_CHANNEL_USE                   (ADDR_RC2D_CTRL3_BASE + 0x002)   // 31
// Decoder shared DRAM
#define RC2D_INT32                              (ADDR_RC2D_CTRL3_BASE + 0x003)
#define RC2D_INT33                              (ADDR_RC2D_CTRL3_BASE + 0x004)
#define RC2D_INT34                              (ADDR_RC2D_CTRL3_BASE + 0x005)
#define RC2D_INT35                              (ADDR_RC2D_CTRL3_BASE + 0x006)
#define RC2D_INT36                              (ADDR_RC2D_CTRL3_BASE + 0x007)
#define RC2D_INT37                              (ADDR_RC2D_CTRL3_BASE + 0x008)
#define RC2D_INT38                              (ADDR_RC2D_CTRL3_BASE + 0x009)
#define RC2D_INT39                              (ADDR_RC2D_CTRL3_BASE + 0x00A)
#define RC2D_INT3A                              (ADDR_RC2D_CTRL3_BASE + 0x00B)
#define RC2D_INT3B                              (ADDR_RC2D_CTRL3_BASE + 0x00C)
#define RC2D_INT3C                              (ADDR_RC2D_CTRL3_BASE + 0x00D)
#define RC2D_INT3D                              (ADDR_RC2D_CTRL3_BASE + 0x00E)
#define RC2D_INT3E                              (ADDR_RC2D_CTRL3_BASE + 0x00F)

// PCM
#define ADDR_RC2D_PCM_INPUT_SAMPLING_RATE       (RC2D_INT32)
#define ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT       (RC2D_INT33)
#define ADDR_RC2D_PCM_INPUT_CHANNEL_NUM         (RC2D_INT34)
#define ADDR_RC2D_PCM_INPUT_TYPE                (RC2D_INT35)
#define ADDR_RC2D_PCM_PREBUF_DELAY_BANK         (RC2D_INT36)
#define ADDR_RC2D_PCM_LRCK_MODE                 (RC2D_INT3A)


// COOK
#define ADDR_RC2D_COOK_SAMP_PER_FRAME           (RC2D_INT32)
#define ADDR_RC2D_COOK_CHANNEL_NUM              (RC2D_INT33)
#define ADDR_RC2D_COOK_INPUT_SAMPLING_RATE      (RC2D_INT34)
#define ADDR_RC2D_COOK_FRM_SZ_IN_BYTES          (RC2D_INT35)
#define ADDR_RC2D_COOK_REGION_NUM               (RC2D_INT36)
#define ADDR_RC2D_COOK_COUPLING_START_REGN      (RC2D_INT37)
#define ADDR_RC2D_COOK_COUPLING_QBITS           (RC2D_INT38)




// LPCM
#define ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT       (RC2D_INT32)
#define ADDR_RC2D_LPCM_FREQUENCY                (RC2D_INT33)
#define ADDR_RC2D_LPCM_BIT_SHIFT                (RC2D_INT34)
#define ADDR_RC2D_LPCM_DRC_VALUE                (RC2D_INT35)
#define ADDR_RC2D_LPCM_Q                        (RC2D_INT36)
#define ADDR_RC2D_LPCM_DRC_FLAG                 (RC2D_INT37)
#define ADDR_RC2D_LPCM_DECODING_TYPE            (RC2D_INT38)

// ADPCM
#define ADDR_RC2D_ADPCM_BLOCK_ALIGN             (RC2D_INT36)
#define ADDR_RC2D_ADPCM_CHANNEL_NUM             (RC2D_INT37)
#define ADDR_RC2D_LPCM_ADPCM_DEC_TYPE           (RC2D_INT38)
#define ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE    (RC2D_INT39)

// AC3
#define ADDR_RC2D_AC3_KARAOKE_MODE              (RC2D_INT32)
// Stereo: 0x0, Ch0 Only: 0x1 <<8
// Ch1 Only: 0x2 <<8, Ch0+Ch1 Mixing: 0x3 <<8
#define ADDR_RC2D_AC3_DUAL_MODE                 (RC2D_INT33)
// Custom Mode 0: 0x0, Custom Mode 1: 0x1 <<8
// Line Mode: 0x2 <<8, RF Mode: 0x3 <<8
#define ADDR_RC2D_AC3_COMPRESS_MODE             (RC2D_INT34)
#define ADDR_RC2D_AC3_DYNAMIC_LOW	            (RC2D_INT35)
#define ADDR_RC2D_AC3_DYNAMIC_HIGH              (RC2D_INT36)
#define ADDR_RC2D_AC3_DOWNMIX                   (RC2D_INT37)
#ifndef CC_AUD_SUPPORT_MS10
#define ADDR_RC2D_AC3_BYPASS_FLAG               (RC2D_INT38)
#endif
#define ADDR_RC2D_AC3_CONTROL                   (RC2D_INT39)
#ifdef CC_AUD_SUPPORT_MS10
#define ADDR_RC2D_DDC_STRMID                    (RC2D_INT3A)
#define ADDR_RC2D_DDC_ASSOC_MIX                 (RC2D_INT38)
#endif

//DDT
#define ADDR_RC2D_DDT_DUAL_MODE                 (RC2D_INT33)
#define ADDR_RC2D_DDT_DMX_MODE                  (RC2D_INT34)
#define ADDR_RC2D_DDT_DRC                       (RC2D_INT35)
#define ADDR_RC2D_DDT_CONTROL                   (RC2D_INT36)

// MPEG
#define ADDR_RC2D_MPEG_DOWNMIX_MODE             (RC2D_INT32)
#define ADDR_RC2D_MPEG_SYNC_MODE                (RC2D_INT33)
#define ADDR_RC2D_MPEG_DEEMPHASIS_MODE          (RC2D_INT34)
#define ADDR_RC2D_MPEG_CRC_MODE                 (RC2D_INT35)
#define ADDR_RC2D_MPEG_DRC_FLAG                 (RC2D_INT37)
// OFF: 0x0, AUTO: 0x1 <<8
#define ADDR_RC2D_CRC_FLAG                      (RC2D_INT38)

// MP3
#define ADDR_RC2D_MP3_NORMAL_PLAY               (RC2D_INT32)
#define ADDR_RC2D_MP3_SYNC_MODE                 (RC2D_INT33)
#define ADDR_RC2D_MP3_DEEMPHASIS_MODE           (RC2D_INT34)
#define ADDR_RC2D_MP3_CRC_MODE                  (RC2D_INT35)
#define ADDR_RC2D_MP3_PREPARSING_MODE           (RC2D_INT36)
#define ADDR_RC2D_MP3_SMOOTH_FLAG               (RC2D_INT37)
#define ADDR_RC2D_MP3_QUALITY_FLAG              (RC2D_INT38)

// CDDA
#define ADDR_RC2D_CDDA_CFGDEEMPHASIS            (RC2D_INT32)

// WMA
#define ADDR_RC2D_WMA_PACKET_NO                 (RC2D_INT32)
#define ADDR_RC2D_WMA_HEADER_SOURCE             (RC2D_INT33)
#define ADDR_RC2D_WMA_PACKET_COUNT              (RC2D_INT34)
#define ADDR_RC2D_WMA_PACKET_SIZE               (RC2D_INT35)
#define ADDR_RC2D_WMA_VERSION                   (RC2D_INT36)
#define ADDR_RC2D_WMA_NUM_CHANNELS              (RC2D_INT37)
#define ADDR_RC2D_WMA_SAMPLE_PER_SECOND         (RC2D_INT38)
#define ADDR_RC2D_WMA_BYTES_PER_SECOND          (RC2D_INT39)
#define ADDR_RC2D_WMA_BLOCK_SIZE                (RC2D_INT3A)
#define ADDR_RC2D_WMA_ENCODER_OPTIONS           (RC2D_INT3B)

// WMAPRO
#define ADDR_RC2D_WMAPRO_PACKET_NO              (RC2D_INT32)
#define ADDR_RC2D_WMAPRO_HEADER_SOURCE          (RC2D_INT33)
#define ADDR_RC2D_WMAPRO_NUM_CHANNELS           (RC2D_INT34)
#define ADDR_RC2D_WMAPRO_SAMPLE_PER_SECOND      (RC2D_INT35)
#define ADDR_RC2D_WMAPRO_BYTES_PER_SECOND       (RC2D_INT36)
#define ADDR_RC2D_WMAPRO_BLOCK_SIZE             (RC2D_INT37)
#define ADDR_RC2D_WMAPRO_ENCODER_OPTIONS        (RC2D_INT38)
#define ADDR_RC2D_WMAPRO_CH_MASK                (RC2D_INT39)
#define ADDR_RC2D_WMAPRO_VALID_BITS             (RC2D_INT3A)
#define ADDR_RC2D_WMAPRO_ADV_OPTION             (RC2D_INT3B)
#define ADDR_RC2D_WMAPRO_ADV_OPTION2            (RC2D_INT3C)



// HDCD
#define ADDR_RC2D_HDCD_CONFIG                   (RC2D_INT33)
#define ADDR_RC2D_HDCD_CFGUPSAMPLING            (RC2D_INT34)
#define ADDR_RC2D_HDCD_CFGDITHMODE              (RC2D_INT35)

// DRC
#define ADDR_RC2D_DRC_VALUE                     (RC2D_INT35)

// DTS Dynamic Range Control Flag
#define ADDR_RC2D_DTS_DRC_FLAG                  (RC2D_INT32)
#define ADDR_RC2D_DTS_LFE_mixto_FRONT           (RC2D_INT33)
#define ADDR_RC2D_DTS_DRC_ONOFF                 (RC2D_INT34)
#define ADDR_RC2D_DTS_DIALNORM_ONOFF            (RC2D_INT35)
#define ADDR_RC2D_DTS_DRC_LOW                   (RC2D_INT36)
#define ADDR_RC2D_DTS_DRC_MEDIUM                (RC2D_INT37)
#define ADDR_RC2D_DTS_DRC_HIGH                  (RC2D_INT38)
#define ADDR_RC2D_DTS_DIALNORM_LOW              (RC2D_INT39)
#define ADDR_RC2D_DTS_DIALNORM_MEDIUM           (RC2D_INT3A)
#define ADDR_RC2D_DTS_DIALNORM_HIGH             (RC2D_INT3B)

//AAC
#define ADDR_RC2D_AAC_DOWNMIX_LEVEL             (RC2D_INT32)
#define ADDR_RC2D_AAC_TARGET_LEVEL              (RC2D_INT33)

//VORBIS, ///CC_VORBIS_SUPPORT
#define ADDR_RC2D_VORBIS_PACKET_NO              (RC2D_INT32)    /// 08143h

//===================================================================
// Decoder info for dec1 (08150h~08160h, 10h)
//===================================================================
#define DSP_RC2D_SETTING_AREA_SZ                (0x10)
#ifdef DSP_SUPPORT_NPTV
// Demodulation Parameters
#define ADDR_DEM_SOURCE_MODE                    (ADDR_RC2D_DECODER_INFO_BASE+0x000)
#define ADDR_DEM_USER_MODE                      (ADDR_RC2D_DECODER_INFO_BASE+0x001)
#define ADDR_DEM_ADC_GAIN                       (ADDR_RC2D_DECODER_INFO_BASE+0x002)
#define ADDR_DEM_ENFORCED_MODE                  (ADDR_RC2D_DECODER_INFO_BASE+0x003)
// Detection parameters for Top Level Detection
// 0 -> PAL, 1->SECAM L; 8150h
#define ADDR_DETECTOR_DEFAULT_MODE              (ADDR_RC2D_DECODER_INFO_BASE+0x000)
// 0->OFF, 1->ON; 8151h
#define ADDR_DETECTOR_START_FLAG                (ADDR_RC2D_DECODER_INFO_BASE+0x001)
#define ADDR_FM_RDO_DET_INBAND_TH               (ADDR_RC2D_DECODER_INFO_BASE+0x002)
#define ADDR_FM_RDO_DET_NOISE_TH                (ADDR_RC2D_DECODER_INFO_BASE+0x003)
#define ADDR_DETECTOR_FM_NOISE_TH               (ADDR_RC2D_DECODER_INFO_BASE+0x003)

#define ADDR_DEM_PAL_FINE_TUNE_VOLUME           (ADDR_RC2D_DECODER_INFO_BASE+0x004)
#define ADDR_DEM_SAP_FINE_TUNE_VOLUME           (ADDR_RC2D_DECODER_INFO_BASE+0x004)
#define ADDR_DEM_NICAM_FINE_TUNE_VOLUME         (ADDR_RC2D_DECODER_INFO_BASE+0x005)
#define ADDR_DEM_FMFM_DUAL_FINE_TUNE_VOLUME     (ADDR_RC2D_DECODER_INFO_BASE+0x005)
#define ADDR_DEM_AM_FINE_TUNE_VOLUME            (ADDR_RC2D_DECODER_INFO_BASE+0x006)
#define ADDR_DEM_MTS_FINE_TUNE_VOLUME           (ADDR_RC2D_DECODER_INFO_BASE+0x006)
#define ADDR_DEM_FMRADIO_FINE_TUNE_VOLUME       (ADDR_RC2D_DECODER_INFO_BASE+0x0006)
#define ADDR_SECAM_L_MUTE_MODE                  (ADDR_RC2D_DECODER_INFO_BASE+0x007)
#define ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT       (ADDR_RC2D_DECODER_INFO_BASE+0x008)
#define ADDR_SECAM_L_MUTE_THRESHOLD_LOW         (ADDR_RC2D_DECODER_INFO_BASE+0x009)
// FMFM DETECTION
#define ADDR_FMFM_NUM_CHECK                     (ADDR_RC2D_DECODER_INFO_BASE+0x00A)
#define ADDR_FMFM_NUM_DOUBLE_CHECK              (ADDR_RC2D_DECODER_INFO_BASE+0x00B)
#define ADDR_FMFM_MONO_WEIGHT                   (ADDR_RC2D_DECODER_INFO_BASE+0x00C)
#define ADDR_FMFM_STEREO_WEIGHT                 (ADDR_RC2D_DECODER_INFO_BASE+0x00D)
#define ADDR_FMFM_DUAL_WEIGHT                   (ADDR_RC2D_DECODER_INFO_BASE+0x00E)
#define ADDR_FMFM_DETECTION_CONFID              (ADDR_RC2D_DECODER_INFO_BASE+0x00F)
#endif // DSP_SUPPORT_NPTV

//===================================================================
// Misc. for Dec1 (08160h~08180h, 20h)
//===================================================================
#define ADDR_RC2D_IEC_FLAG                      (ADDR_RC2D_MISC_BASE + 0x000)
    #define IEC_DSP_MUTE                        (0x1 << 15)
#define ADDR_RC2D_IEC_DELAY                     (ADDR_RC2D_MISC_BASE + 0x001)
#define ADDR_RC2D_CUSTOMER_OPTION0              (ADDR_RC2D_MISC_BASE + 0x002)
    #define IEC_SRC_CH_NUM_DONT_CARE            (0x1 <<  0)
    #define CENTER_SPLIT2LR_WHEN_3_0            (0x1 <<  1)
    #define DVD_A_96K_DO_BASSMANAGE             (0x1 <<  2)
    #define MLP_PARTIAL_DOWNMIX                 (0x1 <<  3)
    #define AUDIO_UPDATE_EFFECT_ONOFF           (0x1 <<  4)
#define ADDR_RC2D_CUSTOMER_OPTION1              (ADDR_RC2D_MISC_BASE + 0x003)
    #define LG_DEF                              (0x1 << 00)
    #define SONY_DEF                            (0x1 << 01)
    #define PHILIPS_DEF                         (0x1 << 02)
    #define PIONEER_DEF                         (0x1 << 03)

#define ADDR_RC2D_DOWNLOAD_TABLE_FLAG           (ADDR_RC2D_MISC_BASE + 0x004)
#define ADDR_RC2D_VOLUME_SMOOTH_ORDER           (ADDR_RC2D_MISC_BASE + 0x005)
#define ADDR_RC2D_VOLUME_LEVEL_UP_ORDER         (ADDR_RC2D_MISC_BASE + 0x006)
#define ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER       (ADDR_RC2D_MISC_BASE + 0x007)
#define ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER        (ADDR_RC2D_MISC_BASE + 0x008)
#define ADDR_RC2D_INPUT_SRC_GAIN                (ADDR_RC2D_MISC_BASE + 0x009)
#define ADDR_RC2D_SPDIF_WORD_LENGTH             (ADDR_RC2D_MISC_BASE + 0x00A)
#define ADDR_RC2D_AUDIO_MUTE_THRESHOLD          (ADDR_RC2D_MISC_BASE + 0x00B)
#define ADDR_RC2D_DISC_TYPE                     (ADDR_RC2D_MISC_BASE + 0x00C)
#define ADDR_RC2D_CGMS_INFO                     (ADDR_RC2D_MISC_BASE + 0x00D)
#define ADDR_RC2D_DAC_BIT_RESOLUTION            (ADDR_RC2D_MISC_BASE + 0x00E)
#define ADDR_RC2D_AOUT_BANK_576_NUM             (ADDR_RC2D_MISC_BASE + 0x00F)
#define ADDR_RC2D_AOUT_BANK_384_NUM             (ADDR_RC2D_MISC_BASE + 0x010)
#define ADDR_RC2D_AOUT_BANK_320_NUM             (ADDR_RC2D_MISC_BASE + 0x011)
#define ADDR_RC2D_AOUT_BANK_256_NUM             (ADDR_RC2D_MISC_BASE + 0x012)
#define ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM      (ADDR_RC2D_MISC_BASE + 0x013)
#define ADDR_RC2D_EQUALIZER_FLAG                (ADDR_RC2D_MISC_BASE + 0x014)
#define ADDR_RC2D_REVERB_FLAG                   (ADDR_RC2D_MISC_BASE + 0x015)
#define ADDR_RC2D_SPECTRUM_HIGH_ORDER           (ADDR_RC2D_MISC_BASE + 0x016)
    #define SPECTRUM_HIGH_ORDER                 0x7
#define ADDR_RC2D_SPECTRUM_LOW_ORDER            (ADDR_RC2D_MISC_BASE + 0x017)
    #define SPECTRUM_LOW_ORDER                  0x5
#define ADDR_RC2D_DDCO_AGC_FLAG                 (ADDR_RC2D_MISC_BASE + 0x018)
#define ADDR_RC2D_DDCO_FLAG                     (ADDR_RC2D_MISC_BASE + 0x019)
#define ADDR_RC2D_DDCO_LFE_LPF_FLAG             (ADDR_RC2D_MISC_BASE + 0x01A)
//#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
#define ADDR_RC2D_AUDYSSEY_FLAG                 (ADDR_RC2D_MISC_BASE + 0x01B)
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
#define ADDR_RC2D_UPLOAD_DATA_FLAG              (ADDR_RC2D_MISC_BASE + 0x01C)
#define ADDR_RC2D_UPLOAD_BLOCK_NUM              (ADDR_RC2D_MISC_BASE + 0x01D)
#define ADDR_RC2D_UPLOAD_BLOCK_SIZE             (ADDR_RC2D_MISC_BASE + 0x01E)
#define ADDR_RC2D_UPLOAD_BLK_INT_CNT            (ADDR_RC2D_MISC_BASE + 0x01F)
//#endif
    //;=========================
    //;Audyssey_flag:
    //;   bit 0: ADV ON/OFF
    //;   bit 1: ADV Light
    //;   bit 2: ADV_Medium
    //;   bit 3: ADV_Heavy
    //;   bit 4: ABX On/Off
    //;   bit 5: AEQ On/Off
    //;=========================
    #define ADDR_RC2D_ABX_ENABLE_BIT            (0x100U<<4)
    #define ADDR_RC2D_AEQ_ENABLE_BIT            (0x100U<<5)
//#endif

//===================================================================
// Ctrl_4 variable DRAM (08180h~08190h, 10h)
//===================================================================
#define DSP_RC2D_CTRL4_AREA_SZ                  (0x10)
#define ADDR_RC2D_CTRL_4_NUM_1                  (ADDR_RC2D_CTRL4_BASE + 0x000)
#define ADDR_RC2D_STC_DIFF_DEC2                 (ADDR_RC2D_CTRL4_BASE + 0x001)
#define ADDR_RC2D_SPEED_DEC2                    (ADDR_RC2D_CTRL4_BASE + 0x002)
/*-------------------------------------------------------
; Processing mode
;	bit 0: 	EQ Spectrum ON/OFF
;	bit 1: 	DRC ON/OFF
;	bit 2: 	Karaoke Scoring Path in RAM
;	bit 3: 	Auto DRC ON/OFF

;	bit 4: 	Bass Management C,LS,RS Mix-to LR(0)/LFE(1)
;	bit 5:	_bass_ch_mixto_lfe in PRAM
;	bit 6:  Lfe input gain -5 db (0) or 0 db (1)
;	bit 7:  LFE redirect to large speaker

;	bit 8:  Bassmanagement always do IIR on LFE
;	bit 9:  L/R is not the same as Down-Mix Output
;	bit 10: Down-Mix to LT/RT
;	bit 11: Down-Mix 2ch for LG

;	bit 12: Bass Management Bypass Mode
;	bit 13: Post Processing Bypass Mode
;	bit 14: Trim Out Bypass Mode
;	bit 15: Delay Bypass Mode
-------------------------------------------------------*/
#define ADDR_RC2D_PROCESSING_MODE_DEC2          (ADDR_RC2D_CTRL4_BASE + 0x003)
#define ADDR_RC2D_BIT_RESOLUTION_DEC2           (ADDR_RC2D_CTRL4_BASE + 0x004)
// L/R Channel Mixing and Microphone Level Threshold
// bit 0: L/R Mixing
// bit 1: Voice Mute
// bit 2: Vocal Assist
#define ADDR_RC2D_KARAOKE_FLAG_DEC2             (ADDR_RC2D_CTRL4_BASE + 0x005)
// Value: 0x800000~0
#define ADDR_RC2D_LRCH_MIX_RATIO_DEC2           (ADDR_RC2D_CTRL4_BASE + 0x006)
#define ADDR_RC2D_MASTER_VOLUME_DEC2            (ADDR_RC2D_CTRL4_BASE + 0x007)
#define ADDR_RC2D_SPDIF_FLAG_DEC2               (ADDR_RC2D_CTRL4_BASE + 0x008)
#define ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC2     (ADDR_RC2D_CTRL4_BASE + 0x009)
#define ADDR_RC2D_DECODING_STR_PNT_DEC2         (ADDR_RC2D_CTRL4_BASE + 0x00A)
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
#define ADDR_RC2D_UPLOAD_WRITE_PTR              (ADDR_RC2D_CTRL4_BASE + 0x00F)
//#endif

//===================================================================
// Ctrl_5 variable DRAM (08190h~081A0h, 10h)
//===================================================================
#define DSP_RC2D_CTRL5_AREA_SZ                  (0x10)
#define ADDR_RC2D_CTRL_5_NUM_1                  (ADDR_RC2D_CTRL5_BASE + 0x000)
#define ADDR_RC2D_DOWNSAMPLE_DEC2               (ADDR_RC2D_CTRL5_BASE + 0x001)   // 30
//Channel Use -> Pink Noise
#define ADDR_RC2D_CHANNEL_USE_DEC2              (ADDR_RC2D_CTRL5_BASE + 0x002)   // 31
#define RC2D_INT52                              (ADDR_RC2D_CTRL5_BASE + 0x003)
#define RC2D_INT53                              (ADDR_RC2D_CTRL5_BASE + 0x004)
#define RC2D_INT54                              (ADDR_RC2D_CTRL5_BASE + 0x005)
#define RC2D_INT55                              (ADDR_RC2D_CTRL5_BASE + 0x006)
#define RC2D_INT56                              (ADDR_RC2D_CTRL5_BASE + 0x007)
#define RC2D_INT57                              (ADDR_RC2D_CTRL5_BASE + 0x008)
#define RC2D_INT58                              (ADDR_RC2D_CTRL5_BASE + 0x009)
#define RC2D_INT59                              (ADDR_RC2D_CTRL5_BASE + 0x00A)
#define RC2D_INT5A                              (ADDR_RC2D_CTRL5_BASE + 0x00B)
#define RC2D_INT5B                              (ADDR_RC2D_CTRL5_BASE + 0x00C)
#define RC2D_INT5C								(ADDR_RC2D_CTRL5_BASE + 0x00D)
#define RC2D_INT5D								(ADDR_RC2D_CTRL5_BASE + 0x00E)
// PCM
#define ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC2  (RC2D_INT52)
#define ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC2  (RC2D_INT53)
#define ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC2    (RC2D_INT54)
#define ADDR_RC2D_PCM_INPUT_TYPE_DEC2           (RC2D_INT55)
#define ADDR_RC2D_PCM_LRCK_MODE_DEC2            (RC2D_INT56)

// AC3
#define ADDR_RC2D_AC3_KARAOKE_MODE_DEC2         (RC2D_INT52)
#define ADDR_RC2D_AC3_DUAL_MODE_DEC2            (RC2D_INT53)
#define ADDR_RC2D_AC3_COMPRESS_MODE_DEC2        (RC2D_INT54)
#define ADDR_RC2D_AC3_DYNAMIC_LOW_DEC2          (RC2D_INT55)
#define ADDR_RC2D_AC3_DYNAMIC_HIGH_DEC2         (RC2D_INT56)
#define ADDR_RC2D_AC3_DOWNMIX_DEC2              (RC2D_INT57)
#define ADDR_RC2D_AC3_CONTROL_DEC2              (RC2D_INT58)
#ifdef CC_AUD_SUPPORT_MS10
#define ADDR_RC2D_DDC_STRMID_DEC2               (RC2D_INT59)
#define ADDR_RC2D_DDC_ASSOC_MIX_DEC2            (RC2D_INT5A)
#endif

//DDT
#define ADDR_RC2D_DDT_DUAL_MODE_DEC2            (RC2D_INT53)
#define ADDR_RC2D_DDT_DMX_MODE_DEC2             (RC2D_INT54)
#define ADDR_RC2D_DDT_DRC_DEC2                  (RC2D_INT55)
#define ADDR_RC2D_DDT_CONTROL_DEC2              (RC2D_INT56)
#define ADDR_RC2D_DDT_DUAL_MODE_DEC3            (RC2D_INT69)
#define ADDR_RC2D_DDT_DMX_MODE_DEC3             (RC2D_INT6A)
#define ADDR_RC2D_DDT_DRC_DEC3                  (RC2D_INT6B)

// MPEG
#define ADDR_RC2D_MPEG_DOWNMIX_MODE_DEC2        (RC2D_INT52)
#define ADDR_RC2D_MPEG_SYNC_MODE_DEC2           (RC2D_INT53)
#define ADDR_RC2D_MPEG_DRC_FLAG_DEC2            (RC2D_INT57)

// MP3
#define ADDR_RC2D_MP3_NORMAL_PLAY_DEC2          (RC2D_INT52)
#define ADDR_RC2D_MP3_SYNC_MODE_DEC2            (RC2D_INT53)
#define ADDR_RC2D_MP3_DEEMPHASIS_MODE_DEC2      (RC2D_INT54)
#define ADDR_RC2D_MP3_PREPARSING_MODE_DEC2      (RC2D_INT56)

//LPCM
#define ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT_DEC2  (RC2D_INT52)
#define ADDR_RC2D_LPCM_FREQUENCY_DEC2           (RC2D_INT53)
#define ADDR_RC2D_LPCM_BIT_SHIFT_DEC2           (RC2D_INT54)
#define ADDR_RC2D_LPCM_DRC_VALUE_DEC2           (RC2D_INT55)
#define ADDR_RC2D_LPCM_Q_DEC2                   (RC2D_INT56)
#define ADDR_RC2D_ADPCM_BLOCK_ALIGN_DEC2        (RC2D_INT56)
#define ADDR_RC2D_LPCM_DRC_FLAG_DEC2            (RC2D_INT57)
#define ADDR_RC2D_ADPCM_CHANNEL_NUM_DEC2        (RC2D_INT57)
#define ADDR_RC2D_LPCM_DECODING_TYPE_DEC2       (RC2D_INT58)
#define ADDR_RC2D_LPCM_ADPCM_DEC_TYPE_DEC2      (RC2D_INT58)
#define ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE_DEC2  (RC2D_INT59)

// DTS Dynamic Range Control Flag
#define ADDR_RC2D_DTS_DRC_FLAG_DEC2             (RC2D_INT52)
#define ADDR_RC2D_DTS_LFE_mixto_FRONT_DEC2      (RC2D_INT53)
#define ADDR_RC2D_DTS_DRC_ONOFF_DEC2            (RC2D_INT54)
#define ADDR_RC2D_DTS_DIALNORM_ONOFF_DEC2       (RC2D_INT55)
#define ADDR_RC2D_DTS_DRC_LOW_DEC2              (RC2D_INT56)
#define ADDR_RC2D_DTS_DRC_MEDIUM_DEC2           (RC2D_INT57)
#define ADDR_RC2D_DTS_DRC_HIGH_DEC2             (RC2D_INT58)
#define ADDR_RC2D_DTS_DIALNORM_LOW_DEC2         (RC2D_INT59)
#define ADDR_RC2D_DTS_DIALNORM_MEDIUM_DEC2      (RC2D_INT5A)
#define ADDR_RC2D_DTS_DIALNORM_HIGH_DEC2        (RC2D_INT5B)

//DTS Encoder Ctrl Parameter
//zy_dts
#define ADDR_RC2D_DTS_ENCODER_FLAG				(RC2D_INT5C)
#define ADDR_RC2D_DTS_ENCODER_AMODE				(RC2D_INT5D)

//AAC
#define ADDR_RC2D_AAC_DOWNMIX_LEVEL_DEC2        (RC2D_INT52)
#define ADDR_RC2D_AAC_TARGET_LEVEL_DEC2         (RC2D_INT53)

//===================================================================
// Decoder info for dec2 (081A0h~081B0h, 10h)
//===================================================================
#define ADDR_RC2D_DECODER2_INFO	                (ADDR_RC2D_DECODER_INFO_BASE_DEC2 + 0x000)

#ifdef DSP_SUPPORT_NPTV
// Demodulation Parameters
#define ADDR_DEM_SOURCE_MODE_DEC2               (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x000)
#define ADDR_DEM_USER_MODE_DEC2                 (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x001)
#define ADDR_DEM_ADC_GAIN_DEC2                  (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x002)
#define ADDR_DEM_ENFORCED_MODE_DEC2             (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x003)
// Detection parameters for Top Level Detection
// 0 -> PAL, 1->SECAM L
#define ADDR_DETECTOR_DEFAULT_MODE_DEC2         (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x000)
// 0->OFF, 1->ON
#define ADDR_DETECTOR_START_FLAG_DEC2           (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x001)
#define ADDR_DETECTOR_FM_NOISE_TH_DEC2          (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x003)

#define ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC2      (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x004)
#define ADDR_DEM_SAP_FINE_TUNE_VOLUME_DEC2      (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x004)
#define ADDR_DEM_NICAM_FINE_TUNE_VOLUME_DEC2    (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x005)
#define ADDR_DEM_FMFM_DUAL_FINE_TUNE_VOLUME_DEC2 (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x005)
#define ADDR_DEM_AM_FINE_TUNE_VOLUME_DEC2       (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x006)
#define ADDR_DEM_MTS_FINE_TUNE_VOLUME_DEC2      (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x006)

#define ADDR_SECAM_L_MUTE_MODE_DEC2             (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x007)
#define ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT_DEC2  (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x008)
#define ADDR_SECAM_L_MUTE_THRESHOLD_LOW_DEC2    (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x009)
//FMFM DETECTION
#define ADDR_FMFM_NUM_CHECK_DEC2                (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x00A)
#define ADDR_FMFM_NUM_DOUBLE_CHECK_DEC2         (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x00B)
#define ADDR_FMFM_MONO_WEIGHT_DEC2              (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x00C)
#define ADDR_FMFM_STEREO_WEIGHT_DEC2            (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x00D)
#define ADDR_FMFM_DUAL_WEIGHT_DEC2              (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x00E)
#define ADDR_FMFM_DETECTION_CONFID_DEC2         (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x00F)
#endif // DSP_SUPPORT_NPTV

//===================================================================
// Misc. for Dec2 (081B0h~081D0h, 20h)
//===================================================================
#define ADDR_RC2D_IEC_FLAG_DEC2                 (ADDR_RC2D_MISC_BASE_DEC2 + 0x000)
#define ADDR_RC2D_IEC_DELAY_DEC2                (ADDR_RC2D_MISC_BASE_DEC2 + 0x001)
#define ADDR_RC2D_CUSTOMER_OPTION0_DEC2         (ADDR_RC2D_MISC_BASE_DEC2 + 0x002)
#define ADDR_RC2D_CUSTOMER_OPTION1_DEC2         (ADDR_RC2D_MISC_BASE_DEC2 + 0x003)
#define ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC2      (ADDR_RC2D_MISC_BASE_DEC2 + 0x004)
#define ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC2      (ADDR_RC2D_MISC_BASE_DEC2 + 0x005)
#define ADDR_RC2D_VOLUME_LEVEL_UP_ORDER_DEC2    (ADDR_RC2D_MISC_BASE_DEC2 + 0x006)
#define ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER_DEC2  (ADDR_RC2D_MISC_BASE_DEC2 + 0x007)
#define ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC2   (ADDR_RC2D_MISC_BASE_DEC2 + 0x008)
#define ADDR_RC2D_INPUT_SRC_GAIN_DEC2           (ADDR_RC2D_MISC_BASE_DEC2 + 0x009)
#define ADDR_RC2D_SPDIF_WORD_LENGTH_DEC2        (ADDR_RC2D_MISC_BASE_DEC2 + 0x00A)
#define ADDR_RC2D_AUDIO_MUTE_THRESHOLD_DEC2     (ADDR_RC2D_MISC_BASE_DEC2 + 0x00B)
#define ADDR_RC2D_DISC_TYPE_DEC2                (ADDR_RC2D_MISC_BASE_DEC2 + 0x00C)
#define ADDR_RC2D_CGMS_INFO_DEC2                (ADDR_RC2D_MISC_BASE_DEC2 + 0x00D)
#define ADDR_RC2D_DAC_BIT_RESOLUTION_DEC2       (ADDR_RC2D_MISC_BASE_DEC2 + 0x00E)
#define ADDR_RC2D_AOUT_BANK_576_NUM_DEC2        (ADDR_RC2D_MISC_BASE_DEC2 + 0x00F)
#define ADDR_RC2D_AOUT_BANK_384_NUM_DEC2        (ADDR_RC2D_MISC_BASE_DEC2 + 0x010)
#define ADDR_RC2D_AOUT_BANK_320_NUM_DEC2        (ADDR_RC2D_MISC_BASE_DEC2 + 0x011)
#define ADDR_RC2D_AOUT_BANK_256_NUM_DEC2        (ADDR_RC2D_MISC_BASE_DEC2 + 0x012)
#define ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM_DEC2 (ADDR_RC2D_MISC_BASE_DEC2 + 0x013)


#ifdef DSP_SUPPORT_NPTV
//===================================================================
// ADDR_RC2D_ATV_CTRL_BASE (081D0h~08200h, 30h)
//===================================================================
#define ADDR_HDEV_MODE                          (ADDR_RC2D_ATV_CTRL_BASE + 0x000)
#define ADDR_CARRIER_SHIFT_MODE                 (ADDR_RC2D_ATV_CTRL_BASE + 0x001)
#define ADDR_FM_SATU_MUTE_MODE                  (ADDR_RC2D_ATV_CTRL_BASE + 0x002)
//for FM carrier sense mute
#define ADDR_FM_CARRIER_MUTE_MODE               (ADDR_RC2D_ATV_CTRL_BASE + 0x003)
#define ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT    (ADDR_RC2D_ATV_CTRL_BASE + 0x004)
#define ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW      (ADDR_RC2D_ATV_CTRL_BASE + 0x005)
// MTS Detection
#define ADDR_MTS_NUM_CHECK                      (ADDR_RC2D_ATV_CTRL_BASE + 0x006)
#define ADDR_MTS_STEREO_MID                     (ADDR_RC2D_ATV_CTRL_BASE + 0x007)
#define ADDR_MTS_STEREO_CON_MID                 (ADDR_RC2D_ATV_CTRL_BASE + 0x008)
#define ADDR_MTS_PILOT3_MID                     (ADDR_RC2D_ATV_CTRL_BASE + 0x009)
#define ADDR_MTS_PILOT3_CON_MID                 (ADDR_RC2D_ATV_CTRL_BASE + 0x00A)
#define ADDR_MTS_SAP_MID                        (ADDR_RC2D_ATV_CTRL_BASE + 0x00B)
#define ADDR_MTS_SAP_CON_MID                    (ADDR_RC2D_ATV_CTRL_BASE + 0x00C)
//PAL DETECTION
#define ADDR_PAL_PARITY_ERR_THRESHOLD           (ADDR_RC2D_ATV_CTRL_BASE + 0x00D)
#define ADDR_PAL_EVERY_NUM_FRAMES               (ADDR_RC2D_ATV_CTRL_BASE + 0x00E)
#define ADDR_PAL_CORRECT_THRESHOLD              (ADDR_RC2D_ATV_CTRL_BASE + 0x00F)
#define ADDR_PAL_TOTAL_SYNC1_LOOP               (ADDR_RC2D_ATV_CTRL_BASE + 0x010)
#define ADDR_PAL_ERR_THRESHOLD                  (ADDR_RC2D_ATV_CTRL_BASE + 0x011)
//A2 DETECTION
#define ADDR_A2_NUM_CHECK                       (ADDR_RC2D_ATV_CTRL_BASE + 0x012)
#define ADDR_A2_NUM_DOUBLE_CHECK                (ADDR_RC2D_ATV_CTRL_BASE + 0x013)
#define ADDR_A2_MONO_WEIGHT                     (ADDR_RC2D_ATV_CTRL_BASE + 0x014)
#define ADDR_A2_STEREO_WEIGHT                   (ADDR_RC2D_ATV_CTRL_BASE + 0x015)
#define ADDR_A2_DUAL_WEIGHT                     (ADDR_RC2D_ATV_CTRL_BASE + 0x016)
#define ADDR_A2_MONO_STEREO_THRES               (ADDR_RC2D_ATV_CTRL_BASE + 0x017)
#define ADDR_A2_STEREO_MONO_THRES               (ADDR_RC2D_ATV_CTRL_BASE + 0x018)
#define ADDR_MANUAL_OUTPUT_MATRIX               (ADDR_RC2D_ATV_CTRL_BASE + 0x019)
#define ADDR_MANUAL_MTS_SW                      (ADDR_RC2D_ATV_CTRL_BASE + 0x01A)
#define ADDR_FM_NON_EU_MODE                     (ADDR_RC2D_ATV_CTRL_BASE + 0x01B)
#define ADDR_DETECTED_TYPE                      (ADDR_RC2D_ATV_CTRL_BASE + 0x01C)
#define ADDR_DETECTED_TYPE_HDEV                 (ADDR_RC2D_ATV_CTRL_BASE + 0x01D)
//#define ADDR_PAL_NOT_FIRST_AOUT_INIT            (ADDR_RC2D_ATV_CTRL_BASE + 0x01E)
//#define ADDR_PAL_HDEV_BAD_SIGNAL_CHECK          (ADDR_RC2D_ATV_CTRL_BASE + 0x01F)
#define ADDR_A2_WEAK_RF_LVL_M2S_THRES           (ADDR_RC2D_ATV_CTRL_BASE + 0x01E)
#define ADDR_A2_WEAK_RF_LVL_S2M_THRES           (ADDR_RC2D_ATV_CTRL_BASE + 0x01F)
//MTS DETECTION
#define ADDR_MTS_NUM_PILOT                      (ADDR_RC2D_ATV_CTRL_BASE + 0x020)
#define ADDR_MTS_NUM_STEREO                     (ADDR_RC2D_ATV_CTRL_BASE + 0x021)
#define ADDR_MTS_NUM_SAP                        (ADDR_RC2D_ATV_CTRL_BASE + 0x022)
#define ADDR_MTS_PILOT_OFFSET_DETECTION         (ADDR_RC2D_ATV_CTRL_BASE + 0x023)
#define ADDR_MTS_MUTE_SAP_LOW                   (ADDR_RC2D_ATV_CTRL_BASE + 0x024)
#define ADDR_MTS_MUTE_SAP_HIGH                  (ADDR_RC2D_ATV_CTRL_BASE + 0x025)
#define ADDR_MTS_SATU_MUTE_HIGH                 (ADDR_RC2D_ATV_CTRL_BASE + 0x026)
#define ADDR_MTS_SATU_MUTE_LOW                  (ADDR_RC2D_ATV_CTRL_BASE + 0x027)
#define ADDR_FM_ONLY                            (ADDR_RC2D_ATV_CTRL_BASE + 0x028)
#define ADDR_DEFAULT_MTS_MODE                   (ADDR_RC2D_ATV_CTRL_BASE + 0x029)   // default mts mode for US model.
                                                                                                                                           // 0:no default, 1:stereo, 2 SAP
#define ADDR_MANUAL_OUTPUT_MATRIX_DEC2          (ADDR_RC2D_ATV_CTRL_BASE + 0x02A)                                                                                                                                           // 0:no default, 1:stereo, 2 SAP
#define ADDR_MTS_SAP_HP_NOISE_LOW               (ADDR_RC2D_ATV_CTRL_BASE + 0x02B)
#define ADDR_MTS_SAP_HP_NOISE_HIGH              (ADDR_RC2D_ATV_CTRL_BASE + 0x02C)
#define ADDR_MTS_SAP_FILTER_SEL                 (ADDR_RC2D_ATV_CTRL_BASE + 0x02D)
#define ADDR_DYNAMIC_DETECTIO_SWITCH            (ADDR_RC2D_ATV_CTRL_BASE + 0x02E)   // PAL/A2 decoder
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
#define ADDR_MTS_OUTPUT_LEVEL_GAIN              (ADDR_RC2D_ATV_CTRL_BASE + 0x02E)
#endif
#define ADDR_MTS_SAP_RATIO                      (ADDR_RC2D_ATV_CTRL_BASE + 0x02F)   // MTS decoder
#define ADDR_NICAM_I_BOOST_FLAG                 (ADDR_RC2D_ATV_CTRL_BASE + 0x02F)   // PAL decoder
#endif

//===================================================================
// ADDR_RC2D_SHARED_INFO_DEC3 Section (08200h~08280h, 80h)
//===================================================================


//===================================================================
// ADDR_RC2D_SHARED_MEMORY_DEC3 Section (08280h~08300h, 80h)
//===================================================================

#define ADDR_RC2D_CTRL6_BASE                    (ADDR_RC2D_SHARED_MEMORY_DEC3 + 0x0000)
#define ADDR_RC2D_DECODER_INFO_BASE_DEC3        (ADDR_RC2D_SHARED_MEMORY_DEC3 + 0x0010)
#define ADDR_RC2D_MISC_BASE_DEC3                (ADDR_RC2D_SHARED_MEMORY_DEC3 + 0x0020)

#define ADDR_RC2D_CTRL7_BASE                    (ADDR_RC2D_SHARED_MEMORY_DEC3 + 0x0040)
#define ADDR_RC2D_DECODER_INFO_BASE_DEC4        (ADDR_RC2D_SHARED_MEMORY_DEC3 + 0x0050)
#define ADDR_RC2D_MISC_BASE_DEC4                (ADDR_RC2D_SHARED_MEMORY_DEC3 + 0x0060)

//===================================================================
// Ctrl_6 variable DRAM (08280h~08290h, 10h)
//===================================================================
#define DSP_RC2D_CTRL6_AREA_SZ                  (0x8)
#define ADDR_RC2D_CTRL_6_NUM_1                  (ADDR_RC2D_CTRL6_BASE + 0x000)

#define ADDR_RC2D_STC_DIFF_DEC3                 (ADDR_RC2D_CTRL6_BASE + 0x001)
#define ADDR_RC2D_PROCESSING_MODE_DEC3          (ADDR_RC2D_CTRL6_BASE + 0x002)
#define ADDR_RC2D_MASTER_VOLUME_DEC3            (ADDR_RC2D_CTRL6_BASE + 0x003)
#define ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC3     (ADDR_RC2D_CTRL6_BASE + 0x004)
#define ADDR_RC2D_DECODING_STR_PNT_DEC3         (ADDR_RC2D_CTRL6_BASE + 0x005)
#define ADDR_RC2D_CTRL6_DEC_BASE                (ADDR_RC2D_CTRL6_BASE + 0x006)
#define RC2D_INT68                              (ADDR_RC2D_CTRL6_BASE + 0x006)
#define RC2D_INT69                              (ADDR_RC2D_CTRL6_BASE + 0x007)
#define RC2D_INT6A                              (ADDR_RC2D_CTRL6_BASE + 0x008)
#define RC2D_INT6B                              (ADDR_RC2D_CTRL6_BASE + 0x009)
#define RC2D_INT6C                              (ADDR_RC2D_CTRL6_BASE + 0x00A)
#define RC2D_INT6D                              (ADDR_RC2D_CTRL6_BASE + 0x00B)
#define RC2D_INT6E                              (ADDR_RC2D_CTRL6_BASE + 0x00C)

// MPEG
#define ADDR_RC2D_MPEG_SYNC_MODE_DEC3           (RC2D_INT69)
#define ADDR_RC2D_MPEG_CRC_FLAG_DEC3	        (RC2D_INT6B)

//PCM
#define ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC3  (RC2D_INT68)
#define ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC3  (RC2D_INT69)
#define ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC3    (RC2D_INT6A)
#define ADDR_RC2D_PCM_INPUT_TYPE_DEC3           (RC2D_INT6B)
#define ADDR_RC2D_PCM_LRCK_MODE_DEC3            (RC2D_INT6C)

//AC3
#define ADDR_RC2D_AC3_KARAOKE_MODE_DEC3         (RC2D_INT68)
#define ADDR_RC2D_AC3_DUAL_MODE_DEC3            (RC2D_INT69)
#define ADDR_RC2D_AC3_COMPRESS_MODE_DEC3        (RC2D_INT6A)
#define ADDR_RC2D_AC3_DYNAMIC_LOW_DEC3          (RC2D_INT6B)
#define ADDR_RC2D_AC3_DYNAMIC_HIGH_DEC3         (RC2D_INT6C)
#define ADDR_RC2D_AC3_DOWNMIX_DEC3              (RC2D_INT6D)
#define ADDR_RC2D_AC3_CONTROL_DEC3              (RC2D_INT6E)

//AAC
#define ADDR_RC2D_AAC_DOWNMIX_LEVEL_DEC3        (RC2D_INT68)
#define ADDR_RC2D_AAC_TARGET_LEVEL_DEC3         (RC2D_INT69)

//DDT
#define ADDR_RC2D_DDT_CONTROL_DEC3              (RC2D_INT6C)

///MP3ENC, MP3ENC_SUPPORT
//=============================================================
// DRAM Encoder Section (082a0h~082b0h) -- Dec3
//08060~08062h, store to SRAM
//08066~08067h
//08069~0806Ch, maintain by DSP & RISC
//=============================================================
//Note: Please don't change the "order" to compatible with DSPM3E library
#define ADDR_RC2D_ENCODER_SFREQ_DEC3        (ADDR_RC2D_CTRL6_DEC_BASE+0x00)	///08286h
/*
AUD_SFREQ_44K                   0x00
AUD_SFREQ_48K                   0x01
AUD_SFREQ_32K                   0x02
*/
#define ADDR_RC2D_ENCODER_BITRATE_DEC3      (ADDR_RC2D_CTRL6_DEC_BASE+0x01)	///08287h
/*
AUD_BRATE_48K                   0x03
AUD_BRATE_56K                   0x04
AUD_BRATE_64K                   0x05
AUD_BRATE_80K                   0x06
AUD_BRATE_96K                   0x07
AUD_BRATE_112K                  0x08
AUD_BRATE_128K                  0x09
AUD_BRATE_160K                  0x0A
AUD_BRATE_192K                  0x0B
AUD_BRATE_224K                  0x0C
AUD_BRATE_256K                  0x0D
AUD_BRATE_320K                  0x0E
*/
#define ADDR_RC2D_ENCODER_CHANNEL_MODE_DEC3  (ADDR_RC2D_CTRL6_DEC_BASE+0x02)	///08288h
/*
AUD_CH_STEREO                   0x0
AUD_CH_JOINT_STEREO             0x1
AUD_CH_DUAL_MONO                0x2
AUD_CH_MONO                     0x3
*/
#define ADDR_RC2D_ENCODER_MPEG_FLAG_DEC3     (ADDR_RC2D_CTRL6_DEC_BASE+0x03)	///08289h
/*
bit 0 : crc on/off
bit 1 : private bit set/unset
bit 2 : copyright bit set/unset
bit 3 : original bit set/unset
bit 4, 5 : padding bit (00-never, 01-always, 10-auto)
*/
#define ADDR_RC2D_ENCODER_INBUF_OVERFLOW_DEC3       (ADDR_RC2D_CTRL6_DEC_BASE+0x04) ///0828Ah
#define ADDR_RC2D_ENCODER_OUTBUF_READ_PNT_DEC3      (ADDR_RC2D_CTRL6_DEC_BASE+0x05) ///0828Bh
#define ADDR_RC2D_ENCODER_OUTBUF_WRITE_PNT_DEC3     (ADDR_RC2D_CTRL6_DEC_BASE+0x06) ///0828Ch
#define ADDR_RC2D_ENCODER_OUTBUF_SIZE_DEC3          (ADDR_RC2D_CTRL6_DEC_BASE+0x07) ///0828Dh
#define ADDR_RC2D_ENCODER_FRAME_SIZE_DEC3           (ADDR_RC2D_CTRL6_DEC_BASE+0x08) ///0828Eh
///[Joel]20090327 No Use
///#define ADDR_RC2D_ENCODER_BIT_LENGTH              (ADDR_RC2D_CTRL6_DEC_BASE+0x02)
///#define ADDR_RC2D_ENCODER_FLUSH_REMAIN            (ADDR_RC2D_CTRL6_DEC_BASE+0x02)
///#define ADDR_RC2D_ENCODER_SOFTMUTE_REMAIN         (ADDR_RC2D_CTRL6_DEC_BASE+0x02)
///#define ADDR_RC2D_ENCODER_SOFTMUTE_ORDER          (ADDR_RC2D_CTRL6_DEC_BASE+0x02)
///#endif

//===================================================================
// ADDR_RC2D_SHARED_MEMORY_DEC3 (08290h~082A0h, 10h)
//===================================================================
// Dec3 Info

//===================================================================
// Misc. for Dec3 (082A0h~082C0h, 20h)
//===================================================================
#define ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC3  (ADDR_RC2D_MISC_BASE_DEC3 + 0x000)
#define ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC3  (ADDR_RC2D_MISC_BASE_DEC3 + 0x001)
#define ADDR_RC2D_VOLUME_LEVEL_UP_ORDER_DEC3    (ADDR_RC2D_MISC_BASE_DEC3 + 0x002)
#define ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER_DEC3  (ADDR_RC2D_MISC_BASE_DEC3 + 0x003)
#define ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC3   (ADDR_RC2D_MISC_BASE_DEC3 + 0x004)
#define ADDR_RC2D_INPUT_SRC_GAIN_DEC3       (ADDR_RC2D_MISC_BASE_DEC3 + 0x005)
#define ADDR_RC2D_AUDIO_MUTE_THRESHOLD_DEC3 (ADDR_RC2D_MISC_BASE_DEC3 + 0x006)
#define ADDR_RC2D_DISC_TYPE_DEC3            (ADDR_RC2D_MISC_BASE_DEC3 + 0x007)
#define ADDR_RC2D_CGMS_INFO_DEC3            (ADDR_RC2D_MISC_BASE_DEC3 + 0x008)
#define ADDR_RC2D_AOUT_BANK_576_NUM_DEC3    (ADDR_RC2D_MISC_BASE_DEC3 + 0x009)
#define ADDR_RC2D_AOUT_BANK_384_NUM_DEC3    (ADDR_RC2D_MISC_BASE_DEC3 + 0x00A)
#define ADDR_RC2D_AOUT_BANK_320_NUM_DEC3    (ADDR_RC2D_MISC_BASE_DEC3 + 0x00B)
#define ADDR_RC2D_AOUT_BANK_256_NUM_DEC3    (ADDR_RC2D_MISC_BASE_DEC3 + 0x00C)

//===================================================================
// Ctrl_7 variable DRAM (082C0h~082D0h, 10h)
//===================================================================
#define DSP_RC2D_CTRL7_AREA_SZ                  (0x08)
#define ADDR_RC2D_CTRL_7_NUM_1                  (ADDR_RC2D_CTRL7_BASE + 0x000)
#define ADDR_RC2D_STC_DIFF_DEC4                 (ADDR_RC2D_CTRL7_BASE + 0x001)
#define ADDR_RC2D_PROCESSING_MODE_DEC4          (ADDR_RC2D_CTRL7_BASE + 0x002)
#define ADDR_RC2D_MASTER_VOLUME_DEC4            (ADDR_RC2D_CTRL7_BASE + 0x003)
#define ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC4     (ADDR_RC2D_CTRL7_BASE + 0x004)
#define ADDR_RC2D_DECODING_STR_PNT_DEC4         (ADDR_RC2D_CTRL7_BASE + 0x005)
#define ADDR_RC2D_CTRL7_DEC_BASE                (ADDR_RC2D_CTRL7_BASE + 0x006)
#define RC2D_INT78                              (ADDR_RC2D_CTRL7_BASE + 0x006)
#define RC2D_INT79                              (ADDR_RC2D_CTRL7_BASE + 0x007)
#define RC2D_INT7A                              (ADDR_RC2D_CTRL7_BASE + 0x008)
#define RC2D_INT7B                              (ADDR_RC2D_CTRL7_BASE + 0x009)
#define RC2D_INT7C                              (ADDR_RC2D_CTRL7_BASE + 0x00A)
#define RC2D_INT7D                              (ADDR_RC2D_CTRL7_BASE + 0x00B)
#define RC2D_INT7E                              (ADDR_RC2D_CTRL7_BASE + 0x00C)
#define RC2D_INT7F                              (ADDR_RC2D_CTRL7_BASE + 0x00D)

//SBCE
#define ADDR_RC2D_SBCE_SAMPLE_FREQ_DEC4         (RC2D_INT78)
#define ADDR_RC2D_SBCE_BLK_NUM_1_DEC4           (RC2D_INT79)
#define ADDR_RC2D_SBCE_CH_NUM_1_DEC4            (RC2D_INT7A)
#define ADDR_RC2D_SBCE_BITPOOL_DEC4             (RC2D_INT7B)
#define ADDR_RC2D_SBCE_SAMPLE_NUM_DEC4          (RC2D_INT7C)
#define ADDR_RC2D_SBCE_FRAME_SIZE_DEC4          (RC2D_INT7D)
#define ADDR_RC2D_SBCE_HDR_0_DEC4               (RC2D_INT7E)
#define ADDR_RC2D_SBCE_WRITE_PTR_DEC4           (RC2D_INT7F)

//PCM
#define ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC4  (RC2D_INT78)
#define ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC4  (RC2D_INT79)
#define ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC4    (RC2D_INT7A)
#define ADDR_RC2D_PCM_INPUT_TYPE_DEC4           (RC2D_INT7B)
#define ADDR_RC2D_PCM_LRCK_MODE_DEC4            (RC2D_INT7C)

//===================================================================
// ADDR_RC2D_SHARED_MEMORY_DEC4 (082D0h~082E0h, 10h)
//===================================================================
// Dec4 Info

//===================================================================
// Misc. for Dec4 (082E0h~08300h, 20h)
//===================================================================
#define ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC4      (ADDR_RC2D_MISC_BASE_DEC4 + 0x000)
#define ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC4      (ADDR_RC2D_MISC_BASE_DEC4 + 0x001)
//#define ADDR_RC2D_VOLUME_LEVEL_UP_ORDER_DEC4    (ADDR_RC2D_MISC_BASE_DEC4 + 0x002)
//#define ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER_DEC4  (ADDR_RC2D_MISC_BASE_DEC4 + 0x003)
//#define ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC4   (ADDR_RC2D_MISC_BASE_DEC4 + 0x004)
#define ADDR_RC2D_INPUT_SRC_GAIN_DEC4           (ADDR_RC2D_MISC_BASE_DEC4 + 0x005)
//#define ADDR_RC2D_AUDIO_MUTE_THRESHOLD_DEC4     (ADDR_RC2D_MISC_BASE_DEC4 + 0x006)
//#define ADDR_RC2D_DISC_TYPE_DEC4                (ADDR_RC2D_MISC_BASE_DEC4 + 0x007)
//#define ADDR_RC2D_CGMS_INFO_DEC4                (ADDR_RC2D_MISC_BASE_DEC4 + 0x008)
//#define ADDR_RC2D_AOUT_BANK_576_NUM_DEC4        (ADDR_RC2D_MISC_BASE_DEC4 + 0x009)
//#define ADDR_RC2D_AOUT_BANK_384_NUM_DEC4        (ADDR_RC2D_MISC_BASE_DEC4 + 0x00A)
//#define ADDR_RC2D_AOUT_BANK_320_NUM_DEC4        (ADDR_RC2D_MISC_BASE_DEC4 + 0x00B)
//#define ADDR_RC2D_AOUT_BANK_256_NUM_DEC4        (ADDR_RC2D_MISC_BASE_DEC4 + 0x00C)

//===================================================================
// ADDR_RC2D_DRAM_BASS_MANAGEMENT_BASE Section (08320h~083A0h)
//===================================================================
// Speaker Configuration
#define ADDR_RC2D_SPEAKER_CONFIG                (ADDR_RC2D_DRAM_BASS_MANAGEMENT_BASE)
#define ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF  (ADDR_RC2D_SPEAKER_CONFIG + 1)
// Channel Delay
#define ADDR_RC2D_CH_DELAY_C                    (ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF + 10)
#define ADDR_RC2D_CH_DELAY_L                    (ADDR_RC2D_CH_DELAY_C   + 1)
#define ADDR_RC2D_CH_DELAY_R                    (ADDR_RC2D_CH_DELAY_L   + 1)
#define ADDR_RC2D_CH_DELAY_LS                   (ADDR_RC2D_CH_DELAY_R   + 1)
#define ADDR_RC2D_CH_DELAY_RS                   (ADDR_RC2D_CH_DELAY_LS  + 1)
#define ADDR_RC2D_CH_DELAY_CH7                  (ADDR_RC2D_CH_DELAY_RS  + 1)
#define ADDR_RC2D_CH_DELAY_CH8                  (ADDR_RC2D_CH_DELAY_CH7 + 1)
#define ADDR_RC2D_CH_DELAY_SUBWOOFER            (ADDR_RC2D_CH_DELAY_CH8 + 1)
#define ADDR_RC2D_CH_DELAY_CH9                  (ADDR_RC2D_CH_DELAY_SUBWOOFER + 1)
#define ADDR_RC2D_CH_DELAY_CH10                 (ADDR_RC2D_CH_DELAY_CH9 + 1)
// Value: 0~200 <<8 / 5cm
#define ADDR_RC2D_CH_DELAY_FACTOR               (ADDR_RC2D_CH_DELAY_CH10 +1)

//===================================================================
// DRAM_3D_FUNCTION_FLAG Section (083A0h~083B0h) -- Foreground Decoder
//===================================================================
// 3D processing
#define ADDR_RC2D_VIRTUAL_SURROUND              (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE + 0)
#define VSS_NONE                                (0x00)
#define VSS_MTK                                 (0x01 << 0)
#define VSS_N22                                 (0x01 << 1)
#define VSS_SRS_WOW                             (0x01 << 2)
#define VSS_SONY                                (0x01 << 3)
#define VSS_TVS                                 (0x01 << 4)
#define VSS_SRS_TSXT                            (0x01 << 5)
#define VSS_QSOUND                              (0x01 << 6)
#define VSS_DOLBY_HP                            (0x01 << 7)
#define VSS_M2S                                 (0x01 << 8)

#define ADDR_RC2D_PRO_LOGIC_II_CONFIG           (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE + 1)
#define ADDR_RC2D_PRO_LOGIC_II_MODE             (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE + 2)
#define ADDR_RC2D_NEWPOST_FLAG                  (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE + 3)
    #define NEWPOST_FLAG_HPF_BIT                (0x1 << 0)     // high pass filter
    #define NEWPOST_FLAG_CLIPPER_BIT            (0x1 << 1)     // clipper
    #define NEWPOST_FLAG_PEQ_BIT			    (0x1 << 2)     // speaker calibration/PEQ
    #define NEWPOST_FLAG_VIR_BASS_BIT           (0x1 << 3)     // virtual bass
    #define NEWPOST_FLAG_SPH_BIT                (0x1 << 4)     // speaker height
    #define NEWPOST_FLAG_M2S_BIT			    (0x1 << 5)     // mono to stereo
    #define NEWPOST_FLAG_CV_BIT			        (0x1 << 6)     // clear voice
    #define NEWPOST_FLAG_NR_BIT			        (0x1 << 7)     // noise reduction
    #define NEWPOST_FLAG_LXP_BIT			    (0x1 << 8)     // LXP post function
    #define NEWPOST_FLAG_CDNOTCH_BIT			(0x1 << 9)     // CDNOTCH post function
    #define NEWPOST_FLAG_KEYSHIFT_BIT			(0x1 << 10)    // keyshift post function
    #define NEWPOST_FLAG_FIR_BIT				(0x1 << 15) 	// FIR post function	//  CC_AUD_FIR_SUPPORT

#define ADDR_RC2D_AD_FLAG                       (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE + 4)

//===================================================================
// ADDR_RC2D_DRAM_PTS_STC Section (083D0h~083E0h) -- Foreground Decoder
//===================================================================
// PTS STC related
#define ADDR_RC2D_MEMORY_BUFFER_ADDRESS         (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x00)
#define ADDR_RC2D_1ST_PTS_PRS_PNT               (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x01)
// 1st pts info to do init AV-sync
#define ADDR_RC2D_1ST_PTS_STCH                  (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x02)
#define ADDR_RC2D_1ST_PTS_STCL                  (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x03)
#define ADDR_RC2D_STC_DIFF_LOBOUND              (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x04)
// trigger sync measure process if over this threshold/
#define STC_DIFF_BOUND_032MS                0x9800  // 32ms, STC=2880
//#define STC_DIFF_BOUND_032MS                    0x11ad   // 50ms, STC=4525
#define ADDR_RC2D_STC_DIFF_HIBOUND              (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x05)
// do sync process if over this threshold
#define STC_DIFF_BOUND_064MS                    0xb000  // 64ms, STC=5760
#define ADDR_RC2D_STC_DIFF_WSBOUND              (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x06)
// ignore the sync process if over this threshold --> PTS/STC are incorrect
#define STC_DIFF_BOUND_500MS                    (0xAFD0*4)   //extend to 2000ms for Allgero AVsync Greek// 500ms, STC=45000
#define ADDR_RC2D_STC_LEVEL_THRESHOLD           (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x07)
/*
; 	bit  0: Dec 0 Aout Int only update dram to STC and skip avsync.
; 	bit  1: Dec 1 Aout Int only update dram to STC and skip avsync.
; 	bit  2: Dec 2 Aout Int only update dram to STC and skip avsync.
; 	bit  3: Dec 3 Aout Int only update dram to STC and skip avsync.
; 	bit  4: Enable PTS_Refresh Log
; 	bit  5: Use Original PTS_refresh method. (Use aput_bank directly)
*/
#define ADDR_RC2D_PTS_STC_CTRL_FLAG             (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x08)
// level threshold of which frame to do repeat/drop
#define STC_THR_36DB                            0x20000  // -36dB
#define STC_THR_42DB                            0x10000  // -42dB

//===================================================================
// ADDR_RC2D_DRAM_EQUALIZER Section (083F0h~084F0h) -- Foreground Decoder
//===================================================================
#define ADDR_RC2D_EQUALIZER_BAND_NUM_1          (ADDR_RC2D_DRAM_EQUALIZER_BASE)    	  // 1
#define ADDR_RC2D_EQUALIZER_TABLE_CHANGE        (ADDR_RC2D_EQUALIZER_BAND_NUM_1 + 1)  // 1
#define ADDR_RC2D_EQUALIZER_TABLE               (ADDR_RC2D_EQUALIZER_TABLE_CHANGE + 1)// 5*10(section)=50
#define ADDR_RC2D_EQUALIZER_HISTORY             (ADDR_RC2D_EQUALIZER_TABLE + 50)	  // 6*10(section)*6(ch)=360
#define ADDR_RC2D_EQUALIZER_DRY_LEVEL           (ADDR_RC2D_EQUALIZER_HISTORY + 360)	  // 1
#define ADDR_RC2D_EQUALIZER_TABLE_NEW           (ADDR_RC2D_EQUALIZER_DRY_LEVEL + 1)	  // 50
#define ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT     (ADDR_RC2D_EQUALIZER_TABLE_NEW+50)      // 1
#define ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT     (ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT+1) // 1

//===================================================================
// ADDR_RC2D_AUDIO_BASIC_SETTING Section (08500h~08510h) -- Foreground Decoder
//===================================================================

// dsp polling dram for mpeg4 to delay G.S Lin's hw
// 0 -> not enable
// else -> how many dsp cycles will dsp request 32 dwrd for dram
// p.s. there will two request from DSP.
//      1. Instruction cache miss ( T.B.D )
//      2. Data fetch ( 32 dwrds)
#define ADDR_RC2D_POLL_DRAM_TIMER_CNT           (ADDR_RC2D_AUDIO_BASIC_BASE+0x00)
// How many dwrds will dsp read in timer interrupt
#define ADDR_RC2D_POLL_DRAM_READ_UINT32_CNT_1   (ADDR_RC2D_AUDIO_BASIC_BASE+0x01)
#define ADDR_RC2D_IEC_CHANNEL_CFG0              (ADDR_RC2D_AUDIO_BASIC_BASE+0x02)
#define ADDR_RC2D_IEC_CHANNEL_CFG1              (ADDR_RC2D_AUDIO_BASIC_BASE+0x03)
#define ADDR_RC2D_IEC_CHANNEL_CFG2              (ADDR_RC2D_AUDIO_BASIC_BASE+0x04)
#define ADDR_RC2D_IEC_BURST_INFO                (ADDR_RC2D_AUDIO_BASIC_BASE+0x05)
#define ADDR_RC2D_IEC_NSNUM                     (ADDR_RC2D_AUDIO_BASIC_BASE+0x06)



//===================================================================
// DRAM_VS_BUF Section (08510h~08610h) -- Foreground Decoder
//===================================================================
#define DRAM_VIRTUAL_SURROUND                   (ADDR_RC2D_DRAM_VS_BUF_BASE)

// New MTK Vsurr
#define ADDR_VSURR_CROS_PARA1                   (DRAM_VIRTUAL_SURROUND)
#define ADDR_VSURR_CROS_PARA2                   (ADDR_VSURR_CROS_PARA1 + 1)
#define ADDR_VSURR_CROS_PARA3                   (ADDR_VSURR_CROS_PARA2+1)
#define ADDR_VSURR_LIMITER_ON                   (ADDR_VSURR_CROS_PARA3+1)
#define ADDR_VSURR_LIMITER_THRESHOLD            (ADDR_VSURR_LIMITER_ON+1)
#define ADDR_VSURR_LIMITER_RSTEP                (ADDR_VSURR_LIMITER_THRESHOLD+1)
#define ADDR_VSURR_GAIN_MPY                     (ADDR_VSURR_LIMITER_RSTEP+1)
#define ADDR_VSURR_GAIN_SL                      (ADDR_VSURR_GAIN_MPY+1)
#define ADDR_VSURR_OUT_OF_PHASE                 (ADDR_VSURR_GAIN_SL+1)
#define ADDR_VSURR_CROS_COEF                    (ADDR_VSURR_OUT_OF_PHASE+1)
#define ADDR_VSURR_DELAY_ON                     (ADDR_VSURR_CROS_COEF+20)
#define ADDR_VSURR_DELAY_SAMPLE_NUM_L           (ADDR_VSURR_DELAY_ON+1)
#define ADDR_VSURR_DELAY_SAMPLE_NUM_R           (ADDR_VSURR_DELAY_SAMPLE_NUM_L+1)
#define ADDR_VSURR_IIR_COEF                     (DRAM_VIRTUAL_SURROUND+39)
#define ADDR_VSURR_BASS_COEF                    (ADDR_VSURR_IIR_COEF+20)
#define ADDR_VSURR_CROS_PARA4                   (ADDR_VSURR_BASS_COEF+5)
#define ADDR_VSURR_CROS_PARA5                   (ADDR_VSURR_CROS_PARA4+1)

#define ADDR_VSURR_1STOP_COEF                   (DRAM_VIRTUAL_SURROUND+0x0DA)
#define ADDR_VSURR_CROS_PARA6                   (ADDR_VSURR_1STOP_COEF+5)
#define ADDR_VSURR_2STOP_COEF                   (ADDR_VSURR_CROS_PARA6+1)
#define ADDR_VSURR_FILTER_HISTORY_2Rstop        (ADDR_VSURR_2STOP_COEF+5)
#define ADDR_VSURR_FILTER_HISTORY_2Lstop        (ADDR_VSURR_FILTER_HISTORY_2Rstop+4)
#define ADDR_VSURR_CROS_PARA7                   (ADDR_VSURR_FILTER_HISTORY_2Lstop+4)

// SRS TSXT
#ifdef DSP_SUPPORT_SRSTSXT
#define ADDR_SRS_TSXT_BAKUP                     (DRAM_VIRTUAL_SURROUND)          // 0 word, a mark only
#define ADDR_SRS_TSXT_DATA                      (ADDR_SRS_TSXT_BAKUP     +  0)
#define ADDR_SRS_TSXT_RODATA                    (ADDR_SRS_TSXT_DATA      + 23)
#define ADDR_SRS_TSXT_BSS                       (ADDR_SRS_TSXT_RODATA    + 58)
#define ADDR_SRS_TSXT_BANK2_BSS                 (ADDR_SRS_TSXT_BSS       + 10)
#define ADDR_SRS_TSXT_DRAM_USE_END              (ADDR_SRS_TSXT_BANK2_BSS + 34)   // 0 word, a mark only
#endif

// DSP_SUPPORT_SRSTSHD DSP_SUPPORT_SRSWOWHD
#define ADDR_SRS_TSHD_BAKUP                     (DRAM_VIRTUAL_SURROUND)             // A mark only
#define ADDR_SRS_TSHD_DATA                      (ADDR_SRS_TSHD_BAKUP + 0)	        // 68
#define ADDR_SRS_TSHD_BANK2_CONST               (ADDR_SRS_TSHD_DATA + 69)	        // 13+2, constants
#define ADDR_SRS_TSHD_M2S_COEF                  (ADDR_SRS_TSHD_BANK2_CONST + 15)    // 30
#define ADDR_SRS_TSHD_PERSP_FR_COEF             (ADDR_SRS_TSHD_M2S_COEF + 30)       // 15
#define ADDR_SRS_TSHD_FOCUS_COEF                (ADDR_SRS_TSHD_PERSP_FR_COEF + 15)	// 5
#define ADDR_SRS_TSHD_PERSP_RR_COEF             (ADDR_SRS_TSHD_FOCUS_COEF + 5)      // 15
#define ADDR_SRS_TSHD_TB_COEF                   (ADDR_SRS_TSHD_PERSP_RR_COEF + 15)  // 25
#define ADDR_SRS_TSHD_DEF_COEF                  (ADDR_SRS_TSHD_TB_COEF + 25)        // 5
#define ADDR_SRS_TSHD_SUB_TB_COEF               (ADDR_SRS_TSHD_DEF_COEF + 5)        // 15
#define ADDR_SRS_TSHD_CROSS_COEF                (ADDR_SRS_TSHD_SUB_TB_COEF + 15)    // 10, LP, HP
#define ADDR_SRS_TSHD_SRS3D_COEF                (ADDR_SRS_TSHD_CROSS_COEF + 10)     // 10, init, final

//===================================================================
// SUPER_BASS Section (08610h~08670h) -- Foreground Decoder
//===================================================================
#define ADDR_RC2D_SUPER_BASS_TABLE_CHANGE       (ADDR_RC2D_SUPER_BASS_BASE)                  // 8500h, 1 word
#define ADDR_RC2D_SUPER_BASS_TABLE              (ADDR_RC2D_SUPER_BASS_TABLE_CHANGE+1)                // 8501h, 10 words
#define ADDR_RC2D_SUPER_BASS_HISTORY            (ADDR_RC2D_SUPER_BASS_TABLE+10)                      // 850Bh, 64 words for 8.0 channel
#define ADDR_RC2D_SUPER_BASS_HISTORY_CH0        (ADDR_RC2D_SUPER_BASS_HISTORY)                       // 850Bh
#define ADDR_RC2D_SUPER_BASS_HISTORY_CH1        (ADDR_RC2D_SUPER_BASS_HISTORY+12)                // 8513h
#define ADDR_RC2D_SUPER_BASS_TABLE_NEW          (ADDR_RC2D_SUPER_BASS_HISTORY+72)                   // 854Bh, 10 words

//===================================================================
// DRAM_3D_FUNCTION_FLAG Section (08670h~08770h) -- Foreground Decoder
//===================================================================
// PL2
#define DRAM_PROLOGIC_II                        (ADDR_RC2D_DRAM_UPMIX_BASE)
#define ADDR_RC2D_3D_BUFFER_ADDR                (DRAM_PROLOGIC_II + 0x00)
#define ADDR_PL2_TABLE                          (DRAM_PROLOGIC_II + 0x20)

//===================================================================
// DRAM_MIXSOUND Section (08770h~08860h) -- Foreground Decoder
//===================================================================
// MixSound
#define ADDR_RC2D_DRAM_MIXSOUND_STATUS			(ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x00)
// 1-->working
// 0-->idle
#define ADDR_RC2D_DRAM_MIXSOUND_CONFIG          (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x01)
//----------------------------------------------------------------------
//Configuration
//  bit	 0: 0-> no need upsampling, 1-> 2x upsampling
//  bit  1: 2-> 4x upsampling, 3-> 8x upsampling
//  bit  2: ADPCM encoding
//  bit  3: ADPCM filter mode : 0->no filter, 1->IIR applied
//  bit  4: source mode : 0->mono, 1->stereo
//  bit  5: memplay mode
//
//  bit  8: mixing data to left & right output
//  bit  9: mixing data to surround output
//  bit 10: mixing data to center output
//  bit 11: mixing data to channel 7 & 8
//  bit 12: mixing data to channel 9 & 10
//----------------------------------------------------------------------
#define ADDR_RC2D_DRAM_MIXSOUND_PCM_ADDR        (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x02)
#define ADDR_RC2D_DRAM_MIXSOUND_PCM_LEN         (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x03)
#define ADDR_RC2D_DRAM_MIXSOUND_GAIN            (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x04)
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
#define ADDR_RC2D_DRAM_MIXSOUND_CLIP_NO_1         (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x05)
#define ADDR_RC2D_DRAM_MIXSOUND_PCM_BUF_PNT_OFST  (ADDR_RC2D_DRAM_MIXSOUND_BASE + 6)

#define ADDR_RC2D_DRAM_MIXSOUND_CLIP1_WP          (ADDR_RC2D_DRAM_MIXSOUND_BASE2 + 0)
#define ADDR_RC2D_DRAM_MIXSOUND_CLIP1_SA          (ADDR_RC2D_DRAM_MIXSOUND_BASE2 + 1)
#define ADDR_RC2D_DRAM_MIXSOUND_CLIP1_EA          (ADDR_RC2D_DRAM_MIXSOUND_BASE2 + 2)
#define ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN        (ADDR_RC2D_DRAM_MIXSOUND_BASE2 + 3)
#define ADDR_RC2D_DRAM_MIXSOUND_CLIP1_RP          (ADDR_RC2D_DRAM_MIXSOUND_BASE2 + 8) //TEST

#define ADDR_RC2D_DRAM_MIXSOUND_CLIP2_WP          (ADDR_RC2D_DRAM_MIXSOUND_BASE2 + 16) //TEST
#define ADDR_RC2D_DRAM_MIXSOUND_CLIP2_SA          (ADDR_RC2D_DRAM_MIXSOUND_BASE2 + 17) //TEST
#define ADDR_RC2D_DRAM_MIXSOUND_CLIP2_EA          (ADDR_RC2D_DRAM_MIXSOUND_BASE2 + 18) //TEST
#define ADDR_RC2D_DRAM_MIXSOUND_CLIP2_GAIN        (ADDR_RC2D_DRAM_MIXSOUND_BASE2 + 19) //TEST
#define ADDR_RC2D_DRAM_MIXSOUND_CLIP2_RP          (ADDR_RC2D_DRAM_MIXSOUND_BASE2 + 24) //TEST

#endif
#define ADDR_RC2D_DRAM_MIXSOUND_UPX8_COEFF      (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x10)
#define ADDR_RC2D_DRAM_MIXSOUND_UPX4_COEFF      (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x15)
#define ADDR_RC2D_DRAM_MIXSOUND_UPX2_COEFF      (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x1A)

//===================================================================
// DRAM_BASS_MANAGMENT_DEC2 Section (08890h~088E0h) -- Background Decoder
//===================================================================
//Speaker Configuration
#define ADDR_RC2D_SPEAKER_CONFIG_DEC2           (ADDR_RC2D_DRAM_BASS_MANAGEMENT_BASE_DEC2)
#define ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF_DEC2 (ADDR_RC2D_SPEAKER_CONFIG_DEC2 + 1)
//Channel Delay
#define ADDR_RC2D_CH_DELAY_C_DEC2               (ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF_DEC2 + 10)
#define ADDR_RC2D_CH_DELAY_L_DEC2               (ADDR_RC2D_CH_DELAY_C_DEC2 + 1)
#define ADDR_RC2D_CH_DELAY_R_DEC2               (ADDR_RC2D_CH_DELAY_L_DEC2 + 1)
  //Value: 0~200 <<8 / 5cm
#define ADDR_RC2D_CH_DELAY_FACTOR_DEC2          (ADDR_RC2D_CH_DELAY_R_DEC2 + 1)

//====================================================================
// DRAM_PTS_STC_DEC2 Section (08900h~08910h) -- Background Decoder
//====================================================================
//PTS STC related
#define ADDR_RC2D_1ST_PTS_PRS_PNT_DEC2          (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x01)
// 1st pts info to do init AV-sync
#define ADDR_RC2D_1ST_PTS_STCH_DEC2             (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x02)
#define ADDR_RC2D_1ST_PTS_STCL_DEC2             (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x03)
#define ADDR_RC2D_STC_DIFF_LOBOUND_DEC2         (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x04)
// lo bound of stc_diff --> do stc measure (with stc_level_threhold) when over this bound
#define ADDR_RC2D_STC_DIFF_HIBOUND_DEC2         (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x05)
// hi bound of stc_diff --> force to do repeat/drop when over this bound
#define ADDR_RC2D_STC_DIFF_WSBOUND_DEC2         (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x06)
// worse bound of stc_diff --> ignore AV-sync when over this bound
#define ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC2      (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x07)
// level threshold of which frame to do repeat/drop

//===================================================================
// DRAM_AVC Section (08940h~089A0h) -- Foreground Decoder
//===================================================================
#define ADDR_RC2D_AVC_TARGET_LEVEL              (ADDR_RC2D_AVC_BASE + 0x00)
#define ADDR_RC2D_AVC_SILENCE_LEVEL             (ADDR_RC2D_AVC_BASE + 0x01)
#define ADDR_RC2D_AVC_MAX_GAIN_UP               (ADDR_RC2D_AVC_BASE + 0x02)
#define ADDR_RC2D_AVC_MAX_GAIN_DOWN             (ADDR_RC2D_AVC_BASE + 0x03)
#define ADDR_RC2D_AVC_FLAG                      (ADDR_RC2D_AVC_BASE + 0x04)
#define ADDR_RC2D_AVC_ATTACK_THRES              (ADDR_RC2D_AVC_BASE + 0x05)
#define ADDR_RC2D_AVC_ADJUST_RATE               (ADDR_RC2D_AVC_BASE + 0x06)
#define ADDR_RC2D_AVC_DSP_FLAG                  (ADDR_RC2D_AVC_BASE + 0x07)
#define ADDR_RC2D_AVC_UI_ADJUST_RATE            (ADDR_RC2D_AVC_BASE + 0x08)
#define ADDR_RC2D_AVC_AVERAGE_FACTOR_TABLE      (ADDR_RC2D_AVC_BASE + 0x09)

//===================================================================
// DRAM_LIMITER_BASE Section (089A0h~089C0h)
//===================================================================
#define ADDR_RC2D_LIMITER_GAIN_MODE             (ADDR_RC2D_LIMITER_BASE)            // 1
// 0: Limiter off, 1: Adaptive mode, 2: Fix mode
#define ADDR_RC2D_LIMITER_FLAG                  (ADDR_RC2D_LIMITER_GAIN_MODE+1)     // 1
#define ADDR_RC2D_LIMITER_FIX_THRESHOLD         (ADDR_RC2D_LIMITER_FLAG+1)          // 1
#ifdef CC_RATIO_LIMITER
#define ADDR_RC2D_LIMITER_GAIN_RATIO            (ADDR_RC2D_LIMITER_FIX_THRESHOLD+1) // 1
#define ADDR_RC2D_LIMITER_GAIN_BASE             (ADDR_RC2D_LIMITER_GAIN_RATIO+1)
#else
#define ADDR_RC2D_LIMITER_GAIN_BASE             (ADDR_RC2D_LIMITER_FIX_THRESHOLD+1)
#endif
#define ADDR_RC2D_LIMITER_GAIN_VOL              (ADDR_RC2D_LIMITER_GAIN_BASE)       // 2
#define ADDR_RC2D_LIMITER_GAIN_VSURR            (ADDR_RC2D_LIMITER_GAIN_VOL+2)      // 2
#define ADDR_RC2D_LIMITER_GAIN_SBASS            (ADDR_RC2D_LIMITER_GAIN_VSURR+2)    // 2
#define ADDR_RC2D_LIMITER_GAIN_EQ               (ADDR_RC2D_LIMITER_GAIN_SBASS+2)    // 2
#define ADDR_RC2D_LIMITER_GAIN_REVERB           (ADDR_RC2D_LIMITER_GAIN_EQ+2)       // 2
#define ADDR_RC2D_LIMITER_GAIN_SPEAKER          (ADDR_RC2D_LIMITER_GAIN_REVERB+2)   // 2
#define ADDR_RC2D_LIMITER_GAIN_VOICE            (ADDR_RC2D_LIMITER_GAIN_SPEAKER+2)  // 2
#define ADDR_RC2D_LIMITER_RELEASE_TIME          (ADDR_RC2D_LIMITER_GAIN_MODE+0x1E)  
#define ADDR_RC2D_LIMITER_RELEASE_TIME_O        (ADDR_RC2D_LIMITER_GAIN_MODE+0x1D)  

#ifdef DSP_SUPPORT_SRS_SSHD
#define ADDR_SRS_STUDIO_FLAG                    (ADDR_RC2D_LIMITER_GAIN_MODE+0x1F) //use last DW for SRS sutdio sound
#endif

#ifdef DSP_SUPPORT_SRS_PURESOUND
#define ADDR_SRS_PURESOUND_FLAG                 (ADDR_RC2D_LIMITER_GAIN_MODE+0x1F) //use last DW for SRS pure sound
#endif


//#ifdef CC_AUD_SILENCE_SUPPORT
//===================================================================
// DRAM_SILENCE_BASE Section (08A70h~08A82h)
//===================================================================
#define ADDR_RC2D_SILENCE_GAIN_MODE             (ADDR_RC2D_SILENCE_BASE) // 1
#define ADDR_RC2D_SILENCE_FIX_THRESHOLD         (ADDR_RC2D_SILENCE_GAIN_MODE+1)
#define ADDR_RC2D_SILENCE_GAIN_BASE             (ADDR_RC2D_SILENCE_FIX_THRESHOLD+1)
#define ADDR_RC2D_SILENCE_WAIT_NUM              (ADDR_RC2D_SILENCE_GAIN_BASE+2) // 2
#define ADDR_RC2D_SILENCE_PEAK_VALUE            (ADDR_RC2D_SILENCE_WAIT_NUM+1)
#define ADDR_RC2D_SILENCE_RELEASE_STEP          (ADDR_RC2D_SILENCE_PEAK_VALUE+1)
#define ADDR_RC2D_SILENCE_ATTACK_STEP           (ADDR_RC2D_SILENCE_RELEASE_STEP+1)
#define ADDR_RC2D_SILENCE_ATTACK_GAIN           (ADDR_RC2D_SILENCE_ATTACK_STEP+1)
//#endif

//===================================================================
// DRAM_DELAY_OFFSET Section (08A90h)
//===================================================================
#define ADDR_DELAY_OFFSET_C                     (ADDR_DSP_DELAY_OFFSET + 1)  //QUERY_DELAY

//===================================================================
// DRAM_OPTIONAL_FUNC_VCT_BASE Section (089C0h~089E0h)
//===================================================================
#ifdef CC_AUD_BBE_SUPPORT
#define ADDR_RC2D_BBE_BASE                      (ADDR_RC2D_OPL_FUNC_VCT_BASE+0) // 1
    #define ADDR_BBE_FLAG                       0x00
    #define ADDR_BBEV4_LEVEL_COEF               0x01
    #define ADDR_BBEV4_3D_GAIN                  0x02
    #define ADDR_BBEV4_BBE_F                    0x03
    #define ADDR_BBEV4_LOW_COEF                 0x04
    #define ADDR_BBEV4_HIGH_COEF                0x05
    #define ADDR_BBEV4_BBE_ON                   0x06
    #define ADDR_BBEV4_LOW_ON                   0x07
    #define ADDR_BBEV4_MID_ON                   0x08
    #define ADDR_BBEV4_HIGH_ON                  0x09
    #define ADDR_BBEV4_IIR_COEF                 0x0A
    #define ADDR_BBEV4_VAR_BACKUP               0x0F
#endif

#ifdef CC_AUD_VBASS_SUPPORT
#define ADDR_RC2D_VBASS_BASE                     (ADDR_RC2D_OPL_FUNC_VCT_BASE+1) // 1
    #define ADDR_VBASS_R_COEF                    0x00
	#define ADDR_VBASS_L_COEF                    0x46
	#define ADDR_VBASS_HIGHPASS_COEF             0x8C
	#define ADDR_VBASS_COMB_FILTER_HISTORY_R     0xA0
	#define ADDR_VBASS_COMB_FILTER_HISTORY_L     0xD8
	#define ADDR_VBASS_ALLPASS_FILTER_HISTORY    0x110
	#define ADDR_VBASS_HIGHPASS_FILTER_HISTORY_L 0x11C
	#define ADDR_VBASS_CROS_PARA4                0x128
	#define ADDR_VBASS_CROS_PARA5                0x129
	#define ADDR_VBASS_CROS_PARA6                0x12A
	#define ADDR_VBASS_CROS_PARA7                0x12B
	#define ADDR_EXPEND_GAIN                     0x12C
	#define ADDR_EXPEND_COUNT                    0x12D
	#define ADDR_dram_attck_count                0x12E
	#define ADDR_limiter_step                    0x12F
	#define ADDR_VBASS_CROS_PARA8                0x130
	#define ADDR_VBASS_CROS_PARA9                0x131
#endif

#ifdef CC_AUD_PEQ_SUPPORT
#define ADDR_RC2D_PEQ_BASE                      (ADDR_RC2D_OPL_FUNC_VCT_BASE+2) // 1
    #define ADDR_PEQ_COEF                       0x00
	#define ADDR_PEQ_FILTER_HISTORY_R           0x2D
	#define ADDR_PEQ_FILTER_HISTORY_L           0x63
	#define ADDR_PEQ1_BW                        0x99
	#define ADDR_PEQ1_FC                        0x9A
	#define ADDR_PEQ1_GAIN                      0x9B
	#define ADDR_PEQ2_BW                        0x9C
	#define ADDR_PEQ2_FC                        0x9D
	#define ADDR_PEQ2_GAIN                      0x9E
	//#define ADDR_PEQ_COS_TAB                    0xBB
#endif

//mono to stereo
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
#define ADDR_RC2D_M2S_BASE                      (ADDR_RC2D_OPL_FUNC_VCT_BASE+3)
    #define ADDR_M2S_COMB_R_COEF                0x00
    #define ADDR_M2S_COMB_L_COEF                0x46
    #define ADDR_M2S_ALLPASS_COEF               0x8C
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
#define ADDR_RC2D_SPKHEIGHT_BASE                (ADDR_RC2D_OPL_FUNC_VCT_BASE+5) // 1
#define ADDR_SHELF_FILTER_HISTORY_R             0x00
#define ADDR_SHELF_FILTER_HISTORY_L             0x0C
#define ADDR_ALLPASS1_FILTER_HISTORY            0x18
#define ADDR_SHELF_R_COEF                       0x20
#define ADDR_SHELF_L_COEF                       0x2F
#define ADDR_ALLPASS1_COEF                      0x3E
#define ADDR_SPKHIGH_PARA4                      0x48
#define ADDR_SPKHIGH_PARA5                      0x49
#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
#define ADDR_RC2D_CLIPPER_BASE                  (ADDR_RC2D_OPL_FUNC_VCT_BASE+6) // 1
#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
#define ADDR_RC2D_ADV_BASE                      (ADDR_RC2D_OPL_FUNC_VCT_BASE+12) // 1
#define ADDR_RC2D_ABX_BASE                      (ADDR_RC2D_OPL_FUNC_VCT_BASE+13) // 1
#define ADDR_RC2D_AEQ_BASE                      (ADDR_RC2D_OPL_FUNC_VCT_BASE+14) // 1
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
#define ADDR_RC2D_CDNOTCH_BASE					(ADDR_RC2D_OPL_FUNC_VCT_BASE+15)
#define ADDR_CDNOTCH_USER_Q						0x00
#define ADDR_CDNOTCH_USER_FC					0x01
#define ADDR_CDNOTCH_Q							0x02
#define ADDR_CDNOTCH_FC							0x03
#define ADDR_CDNOTCH_COEFF						0x04	// size 5*4
#define ADDR_CDNOTCH_HISTORY					0x09	// size 6*6*4
#define ADDR_CDNOTCH_ALPHA_IDX                  0x2d
#define ADDR_CDNOTCH_HALF_H_IDX                 0x2e
#endif

#ifdef  KARAOKE_SUPPORT
#define ADDR_RC2D_KEYSHIFT_BASE                 (ADDR_RC2D_DRAM_KEYSHIFT_BASE)
#define ADDR_KEYSHIFT_KEY                       0x85
#define ADDR_KEYSHIFT_KEY_DEC2                  0x95
#endif

#ifdef DSP_SUPPORT_SRS_SSHD    //sunman_peq
#define ADDR_RC2D_SRS_PEQ_BASE                  (ADDR_RC2D_OPL_FUNC_VCT_BASE+16) // 1
#define ADDR_SRS_PEQ_INPUT_GAIN                 0x00
#define ADDR_SRS_PEQ_OUTPUT_GAIN                0x01
#define ADDR_SRS_PEQ_BANDS_1                    0x02
#define ADDR_SRS_PEQ_COEF                       0x03 //40+32+32
#define ADDR_SRS_HPF_COEF                       (0x03 + 104) //len 10 + 8 + 8
#define ADDR_SRS_HPF2_COEF                      (0x03 + 104+26) //len 10 + 8 + 8
#define ADDR_SRS_PEQ_HPF_COEF                   (0x03 + 104 + 26 + 26) //len 10 + 8 + 8



//sunman_lmt
#define ADDR_RC2D_SRS_HL_BASE                   (ADDR_RC2D_OPL_FUNC_VCT_BASE+17) // 2
#define ADDR_SRS_LMT_INPUT_GAIN                 0x00
#define ADDR_SRS_LMT_OUTPUT_GAIN                0x01
#define ADDR_SRS_LMT_BOOST                      0x02
#define ADDR_SRS_LMT_LVL                        0x03
#define ADDR_SRS_LMT_DELAY                      0x04
#define ADDR_SRS_LMT_TABLE                      0x05   // length 3+240
#define ADDR_SRS_LMT_COEFF                      0xF8
#define ADDR_SRS_LMT_THRESH                     0xF9 
#define ADDR_SRS_LMT_HISTORY1                   0xFA
#define ADDR_SRS_LMT_HISTORY2                   0xFB
#define ADDR_SRS_LMT_MYFLAG1                    0xFC
#define ADDR_SRS_LMT_MYFLAG2                    0xFD
#define ADDR_SRS_LMT_NDLEVEL1                   0xFE
#define ADDR_SRS_LMT_NDLEVEL2                   0xFF
#define ADDR_SRS_LMT_MAXHISTORY0                0x100
#define ADDR_SRS_LMT_MAXHISTORY1                0x101
#define ADDR_SRS_LMT_MAXHISTORY2                0x102
#define ADDR_SRS_LMT_LASTGAIN1                  0x103
#define ADDR_SRS_LMT_LASTGAIN2                  0x104
#define ADDR_SRS_LMT_LZTBL                      0x105//256 WORDS
#define ADDR_SRS_LMT_LOOKAHEAD                  0x205
#define ADDR_SRS_LMT_LAINDEX                    0x206


#endif


#ifdef DSP_SUPPORT_SRS_PURESOUND 

//bofeng_lmt
#define ADDR_RC2D_SRS_HL_BASE                   (ADDR_RC2D_OPL_FUNC_VCT_BASE+17) // 2
#define ADDR_SRS_LMT_INPUT_GAIN                 0x00
#define ADDR_SRS_LMT_OUTPUT_GAIN                0x01
#define ADDR_SRS_LMT_BOOST                      0x02
#define ADDR_SRS_LMT_LVL                        0x03
#define ADDR_SRS_LMT_DELAY                      0x04
#define ADDR_SRS_LMT_TABLE                      0x05   // length 3+240
#define ADDR_SRS_LMT_COEFF                      0xF8
#define ADDR_SRS_LMT_THRESH                     0xF9   
#define ADDR_SRS_LMT_HISTORY1                   0xFA
#define ADDR_SRS_LMT_HISTORY2                   0xFB
#define ADDR_SRS_LMT_MYFLAG1                    0xFC
#define ADDR_SRS_LMT_MYFLAG2                    0xFD
#define ADDR_SRS_LMT_NDLEVEL1                   0xFE
#define ADDR_SRS_LMT_NDLEVEL2                   0xFF
#define ADDR_SRS_LMT_MAXHISTORY0                0x100
#define ADDR_SRS_LMT_MAXHISTORY1                0x101
#define ADDR_SRS_LMT_MAXHISTORY2                0x102
#define ADDR_SRS_LMT_LASTGAIN1                  0x103
#define ADDR_SRS_LMT_LASTGAIN2                  0x104
#define ADDR_SRS_LMT_LZTBL                      0x105//256 WORDS
#define ADDR_SRS_LMT_LOOKAHEAD                  0x205
#define ADDR_SRS_LMT_LAINDEX                    0x206


//bofeng_aeq
#define ADDR_RC2D_SRS_AEQ_BASE                    (ADDR_RC2D_OPL_FUNC_VCT_BASE+23) // 2
#define ADDR_SRS_AEQ_INPUT_GAIN                0x00
#define ADDR_SRS_AEQ_OUTPUT_GAIN               0x01
#define ADDR_SRS_AEQ_FIR_ORDER                 0x02
#define ADDR_SRS_AEQ_FIR_LASTGAIN              0x03
#define ADDR_SRS_AEQ_IIR_FILTER                0x04
#define ADDR_SRS_AEQ_IIR_HISTORY_L             44
#define ADDR_SRS_AEQ_IIR_HISTORY_R             44+48
#define ADDR_SRS_AEQ_FIR_FILTER                44+48+48
#define ADDR_SRS_AEQ_FIR_HISTORY_L             44+48+48+256
#define ADDR_SRS_AEQ_FIR_HISTORY_R             44+48+48+256+256
#define ADDR_SRS_AEQ_FIR_BAK                   44+48+48+256+256+256//BAK SIZE=256*3
#define ADDR_SRS_AEQ_IIR_SECTION               44+48+48+256+256+256+(256*3)//size=1
#define ADDR_SRS_AEQ_IIR_LASTGAIN              44+48+48+256+256+256+(256*3)+1//size=1


#endif

#ifdef CC_AUD_FIR_SUPPORT
#define ADDR_RC2D_FIR_BASE                      (ADDR_RC2D_OPL_FUNC_VCT_BASE+21)
#define ADDR_RC2D_FIR_FILTER_COEFF              0x01
#endif


//=============================================================
// DRAM_PTS_STC_DEC3 Section (08b00h~08b10h) -- 3rd Decoder
//=============================================================
//PTS STC related
#define ADDR_RC2D_MEMORY_BUFFER_ADDRESS_DEC3               (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC3 + 0x00)
#define ADDR_RC2D_1ST_PTS_PRS_PNT_DEC3                     (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC3 + 0x01)
// 1st pts info to do init AV-sync
#define ADDR_RC2D_1ST_PTS_STCH_DEC3                        (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC3 + 0x02)
#define ADDR_RC2D_1ST_PTS_STCL_DEC3                        (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC3 + 0x03)
#define ADDR_RC2D_STC_DIFF_LOBOUND_DEC3                    (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC3 + 0x04)
// lo bound of stc_diff --> do stc measure (with stc_level_threhold) when over this bound
#define ADDR_RC2D_STC_DIFF_HIBOUND_DEC3                    (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC3 + 0x05)
// hi bound of stc_diff --> force to do repeat/drop when over this bound
#define ADDR_RC2D_STC_DIFF_WSBOUND_DEC3                    (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC3 + 0x06)
// worse bound of stc_diff --> ignore AV-sync when over this bound
#define ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC3                 (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC3 + 0x07)
// level threshold of which frame to do repeat/drop

//===================================================================
// Audyssey Section
//===================================================================
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
#define ADDR_RC2D_ADV_CHCALBGAIN_L              (dReadDspCommDram(ADDR_RC2D_ABX_BASE)+0)
#define ADDR_RC2D_ADV_CHCALBGAIN_R              (dReadDspCommDram(ADDR_RC2D_ABX_BASE)+1)

#define ADDR_RC2D_ABX_IIR_COEF_ADDR_START       (dReadDspCommDram(ADDR_RC2D_ABX_BASE)+2)

#define ADDR_RC2D_AEQ_TABLE_CHANGE              (dReadDspCommDram(ADDR_RC2D_AEQ_BASE)+0)
#define ADDR_RC2D_AEQ_TABLE                     (dReadDspCommDram(ADDR_RC2D_AEQ_BASE)+1) //(ADDR_RC2D_AEQ_TABLE_CHANGE+1)
#define ADDR_RC2D_AEQ_HISTORY                   (dReadDspCommDram(ADDR_RC2D_AEQ_BASE)+26) //(ADDR_RC2D_AEQ_TABLE+25)
#define ADDR_RC2D_AEQ_HISTORY_CH0               (dReadDspCommDram(ADDR_RC2D_AEQ_BASE)+26) //(ADDR_RC2D_AEQ_HISTORY)
#define ADDR_RC2D_AEQ_HISOTRY_CH1               (dReadDspCommDram(ADDR_RC2D_AEQ_BASE)+46) //(ADDR_RC2D_AEQ_HISTORY+20)
#define ADDR_RC2D_AEQ_TABLE_NEW                 (dReadDspCommDram(ADDR_RC2D_AEQ_BASE)+126) //(ADDR_RC2D_AEQ_HISTORY+100)
#endif

//===================================================================
// Audio clipper Section (08C80h~08C90h)
//===================================================================
#ifdef CC_AUD_CLIPPER_SUPPORT
#define ADDR_RC2D_CLIPPER_C_VAL                 (ADDR_RC2D_CLIPPER_BASE + 0x000)
#define ADDR_RC2D_CLIPPER_L_VAL                 (ADDR_RC2D_CLIPPER_BASE + 0x001)
#define ADDR_RC2D_CLIPPER_R_VAL                 (ADDR_RC2D_CLIPPER_BASE + 0x002)
#define ADDR_RC2D_CLIPPER_LS_VAL                (ADDR_RC2D_CLIPPER_BASE + 0x003)
#define ADDR_RC2D_CLIPPER_RS_VAL                (ADDR_RC2D_CLIPPER_BASE + 0x004)
#define ADDR_RC2D_CLIPPER_SW_VAL                (ADDR_RC2D_CLIPPER_BASE + 0x005)
#define ADDR_RC2D_CLIPPER_CH7_VAL               (ADDR_RC2D_CLIPPER_BASE + 0x006)
#define ADDR_RC2D_CLIPPER_CH8_VAL               (ADDR_RC2D_CLIPPER_BASE + 0x007)
#define ADDR_RC2D_CLIPPER_CH9_VAL               (ADDR_RC2D_CLIPPER_BASE + 0x008)
#define ADDR_RC2D_CLIPPER_CH10_VAL              (ADDR_RC2D_CLIPPER_BASE + 0x009)
    #define CLIPPER_DEFAULT_VALUE               0x7c5e4d //0x1f1234
#endif


//===================================================================
// Karaoke system coefficient (09200h~09300h) - ADDR_RC2D_KARAOKE_COEF
//===================================================================
#ifdef KARAOKE_SUPPORT
#define ADDR_RC2D_DRAM_KEYSHIFT_BASE                (DRAM_NORMAL_BASE + 0x01300)
#define ADDR_R2CD_KEYSHIFT_TABLE                    ADDR_RC2D_DRAM_KEYSHIFT_BASE	// DSP Address 0x9260~0x92e4, size 0x84 DWORDs
#endif

#ifdef CC_AUD_PCM_LINE_IN_LPF
#define ADDR_R2CD_DRAM_PCM_LPF_COEF                 (DRAM_NORMAL_BASE + 0x01400)    ///0x9400
#endif

//===================================================================
// PTS coefficient table (09660h~09673h)
//===================================================================
#define ADDR_R2CD_PTSINFO_TABLE                    (DRAM_NORMAL_BASE + 0x01660)

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define DRAM_HPF_BUFFER                                      (DRAM_NORMAL_BASE + 0x017A0)    ///0x97a0
#endif

#ifdef CC_AUD_SUPPORT_MS10
//===================================================================
// XMX mixer coefficient table (09860h~099d0h)
//===================================================================
#define ADDR_RC2D_XMX_TABLE                     (DRAM_NORMAL_BASE + 0x17D0)//0x01860)
#define ADDR_RC2D_XMX_FLAG                      (DRAM_NORMAL_BASE + 0x0193F)//0x019CF)
#endif


#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define DRAM_HPF2_BUFFER                                     (DRAM_NORMAL_BASE + 0x01C00)    ///0x9c00~9c30 bofeng
#endif


//===================================================================
// Post Section (01000h~02800h)
//===================================================================
#ifdef DSP_SUPPORT_SRSVIQ
//#if defined(DSP_SUPPORT_SRSVIQ) || defined(DSP_SUPPORT_SRSTSHD)
#define MULTI_SURROUND_PRAM_ADR                  (0x0010/2)
#define VIRTUAL_SURROUND_PRAM_ADR                (0x0510/2)
#define SPEAKER_ENHANCEMENT_PRAM_ADR             (0x0B7E/2)
#define VOICE_ENHANCEMENT_PRAM_ADR               (0x0B80/2)
#define POST_SECTION_END_PRAM_ADR		         (0x01800/2)
#else
#define MULTI_SURROUND_PRAM_ADR                  (0x0010/2)
#define VIRTUAL_SURROUND_PRAM_ADR                (0x0510/2)
#define SPEAKER_ENHANCEMENT_PRAM_ADR             (0x0B10/2)
#define VOICE_ENHANCEMENT_PRAM_ADR               (0x0E80/2)
#define POST_SECTION_END_PRAM_ADR		         (0x01800/2)
#endif

//===================================================================
// Codec load table address
//===================================================================
// Decoder Page
#define DRAM_DECODING_TABLE_ADDR                (0x0400)

/*const used for DDCO */
#ifdef CC_AUD_SUPPORT_MS10
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    #define DDCO_PRAM_ADR                           (0x0800/2)
    #define DDCO_TABLE_ADR                          (0x49AC0+0x3C0)//After v6.8.2.21 (DRAM_RESTORE_FOR_DDCO_BUF)
    #define DDCO_TABLE3_ADR                         (DDCO_TABLE_ADR+0x3C0)
    #define DDCO_TABLE4_ADR                         (DDCO_TABLE_ADR+0x360)
    #else   //mt5365/95, 24bit mode
#define DDCO_PRAM_ADR                           (0x0800/2)
#define DDCO_TABLE_ADR                          (0x498e0)
#define DDCO_TABLE3_ADR                         (DDCO_TABLE_ADR+0x900)
#define DDCO_TABLE4_ADR                         (DDCO_TABLE_ADR+0x8A0)
    #endif
#else    // not support ms10
#define DDCO_PRAM_ADR                           (0x0900/2)
#define DDCO_TABLE_ADR                          (0x2dd00+0x400)
#define DDCO_TABLE3_ADR                         (0x2dd00+0x340)
#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
/*const used for ADV */
#define ADV_PRAM_ADR                            (0x0E80/2)
#define ADV_TABLE_ADR                           (dReadDspCommDram(ADDR_RC2D_ADV_BASE)+(0x500))
#endif

/*const used for AAC */
#define AAC_CMPT_TABLE_ADR			            (0x21c0)

//bit15: normalization On/Off
//bit7: center_mix_level_on
//bit4~6: center_mix_level_value (0, -1.5, -3, -4.5, -6, -7.5, -9, -infinite)
//bit3: surround_mix_level_on
//bit0~2: surround_mix_level_value
#define AAC_DEFAULT_DOWNMIX_LEVEL					    (0x80aa)    //-3dB for center and surround, normalization ON
#define AAC_TARGET_LEVEL								(23*4)      //-23dB for D-book 6.2.1
#define AAC_PRAM_DEC2_EXT_ADR                           (0x1900/2)

/*const used for DTS-Dec*/
#define    DTS_ADPCM_VQ_TBL_ADR                       (0xA000-0x8000)
#define    DTS_DTS_ROM_TBL_ADR                        (0xE400-0x8000)
#define    DTS_DTS_BC_TBL_ADR                         (0xE800-0x8000)

/*const used for WMA PRO*/
#define WMAPRO_TABLE_NORMAL                         0x6CBC
#define WMAPRO_TABLE_CMPT                           0x8800	//from v681809

//===================================================================
// SRS VIQ (08c80h~09300h)
//===================================================================

#ifdef DSP_SUPPORT_SRSVIQ

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
#define CMPT_VIQ2_BUFFER                              (DRAM_NORMAL_BASE + 0x0D80) //0x8D80~0x9000
#else
#define CMPT_VIQ2_BUFFER                              (DRAM_NORMAL_BASE + 0x0C80) //0x8C80~8F00
#endif

//sunman_viq //DSP_SUPPORT_SRSVIQ
#define ADDR_RC2D_AVC_REFERENCE_LEVEL           (ADDR_RC2D_AVC_BASE + 0x09)
#define ADDR_RC2D_AVC_MODE			            (ADDR_RC2D_AVC_BASE + 0x0a)
#define ADDR_RC2D_AVC_MAX_GAIN                  (ADDR_RC2D_AVC_BASE + 0x0b)
#define ADDR_RC2D_AVC_INPUT_GAIN                (ADDR_RC2D_AVC_BASE + 0x0c)
#define ADDR_RC2D_AVC_OUTPUT_GAIN               (ADDR_RC2D_AVC_BASE + 0x0d)
#define ADDR_RC2D_AVC_NOISE_TH                  (ADDR_RC2D_AVC_BASE + 0x0e)
#define ADDR_SRS_VIQ_G_FACTOR                   (ADDR_RC2D_AVC_BASE + 0x0f)
#define ADDR_SRS_VIQ_COEF1                      (ADDR_RC2D_AVC_BASE + 0x10)
#define ADDR_SRS_VIQ_COEF2                      (ADDR_RC2D_AVC_BASE + 0x11)
#define ADDR_SRS_VIQ_COEF3                      (ADDR_RC2D_AVC_BASE + 0x12)
// Back up drams
#define ADDR_SRS_BACKUP_DRAM                    (ADDR_RC2D_AVC_BASE + 0x13) //len = 12
#endif

LINT_EXT_HEADER_END

#endif   //CTRL_PARA_H

