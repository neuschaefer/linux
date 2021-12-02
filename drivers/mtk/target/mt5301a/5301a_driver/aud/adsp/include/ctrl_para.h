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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: ctrl_para.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file ctrl_para.h
 *  Brief of file ctrl_para.h.
 *  RISC-Controlled DSP Parameter Definition Header File
 */

#ifndef CTRL_PARA_H
#define CTRL_PARA_H

/*===================== Common Page ========================*/
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
//DSP-RISC Shared Section for Transferring
#define ADDR_D2RC_SHARED_INFO                           (0x0000)
//#ifdef CC_MT5391_AUD_SUPPORT
#if 1
#define ADDR_D2RC_SHARED_INFO_DEC3                 (0x0f00)
#endif

//RISC-DSP Shared Section for Transferring
#define ADDR_RC2D_SHARED_MEMORY                         (0x0100)
//#ifdef CC_MT5391_AUD_SUPPORT
#if 1
#define DRAM_RC2D_SHARED_MEMORY_DEC3              (0x0f80)
#endif

// Common Normal
#define DRAM_NORMAL_BASE                                (0x0000)
// Common Compact
#define DRAM_CMPT_BASE                                  (0x1000)

/****************************< Normal DRAM Allocation >************************/

#define ADDR_RC2D_PLAY_SPEED                 (ADDR_RC2D_SHARED_MEMORY + 0x0016) //sunman for play speed 

/* common part */
// general section //
#define ADDR_RC2D_CTRL1_BASE                 (ADDR_RC2D_SHARED_MEMORY + 0x0000)
#define ADDR_RC2D_TRAP_BASE                  (ADDR_RC2D_SHARED_MEMORY + 0x001F)
// foreground decoder section
#define ADDR_RC2D_CTRL2_BASE                 (ADDR_RC2D_SHARED_MEMORY + 0x0030)
#define ADDR_RC2D_CTRL3_BASE                 (ADDR_RC2D_SHARED_MEMORY + 0x0040)
#define ADDR_RC2D_DECODER_INFO_BASE          (ADDR_RC2D_SHARED_MEMORY + 0x0050)
#define ADDR_RC2D_MISC_BASE                  (ADDR_RC2D_SHARED_MEMORY + 0x0060)
// background decoder section
#define ADDR_RC2D_CTRL4_BASE                 (ADDR_RC2D_SHARED_MEMORY + 0x0080)
#define ADDR_RC2D_CTRL5_BASE                 (ADDR_RC2D_SHARED_MEMORY + 0x0090)
#define ADDR_RC2D_DECODER_INFO_BASE_DEC2     (ADDR_RC2D_SHARED_MEMORY + 0x00A0)
#define ADDR_RC2D_MISC_BASE_DEC2             (ADDR_RC2D_SHARED_MEMORY + 0x00B0)

#ifdef DSP_SUPPORT_NPTV
// Assume "ADDR_RC2D_MISC_BASE_DEC2": 0x81B0h~0x81CFh
//===============================================================================
//DRAM_RISC_SHARED_MEMORY_CTRLC 81D0H~81DFH
//===============================================================================
#define ADDR_DRAM_SHARED_MEMORY_CTRLC      (ADDR_RC2D_SHARED_MEMORY + 0x00D0)
#define ADDR_HDEV_MODE               (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x00)
#define ADDR_CARRIER_SHIFT_MODE          (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x01)
#define ADDR_FM_SATU_MUTE_MODE          (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x02)
 //for FM carrier sense mute
#define ADDR_FM_CARRIER_MUTE_MODE          (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x03)
#define ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT          (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x04)
#define ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW          (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x05)
// MTS Detection
#define ADDR_MTS_NUM_CHECK                  (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x06)
#define ADDR_MTS_STEREO_MID                 (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x07)
#define ADDR_MTS_STEREO_CON_MID             (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x08)
#define ADDR_MTS_PILOT3_MID                 (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x09)
#define ADDR_MTS_PILOT3_CON_MID             (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x0A)
#define ADDR_MTS_SAP_MID                    (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x0B)
#define ADDR_MTS_SAP_CON_MID                (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x0C)
  //PAL DETECTION
#define ADDR_PAL_PARITY_ERR_THRESHOLD (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x0D)
#define ADDR_PAL_EVERY_NUM_FRAMES  (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x0E)
#define ADDR_PAL_CORRECT_THRESHOLD     (ADDR_DRAM_SHARED_MEMORY_CTRLC+0x0F)
//===============================================================================
//DRAM_RISC_SHARED_MEMORY_CTRLD 81E0H~81EFH
//===============================================================================
#define ADDR_DRAM_SHARED_MEMORY_CTRLD      (ADDR_RC2D_SHARED_MEMORY + 0x00E0)
//PAL DETECTION
#define ADDR_PAL_TOTAL_SYNC1_LOOP      (ADDR_DRAM_SHARED_MEMORY_CTRLD+0)
#define ADDR_PAL_ERR_THRESHOLD         (ADDR_DRAM_SHARED_MEMORY_CTRLD+1)
  //A2 DETECTION
#define ADDR_A2_NUM_CHECK              (ADDR_DRAM_SHARED_MEMORY_CTRLD+2)
#define ADDR_A2_NUM_DOUBLE_CHECK       (ADDR_DRAM_SHARED_MEMORY_CTRLD+3)
#define ADDR_A2_MONO_WEIGHT            (ADDR_DRAM_SHARED_MEMORY_CTRLD+4)
#define ADDR_A2_STEREO_WEIGHT          (ADDR_DRAM_SHARED_MEMORY_CTRLD+5)
#define ADDR_A2_DUAL_WEIGHT            (ADDR_DRAM_SHARED_MEMORY_CTRLD+6)
#define ADDR_A2_MONO_STEREO_THRES            (ADDR_DRAM_SHARED_MEMORY_CTRLD+7)
#define ADDR_A2_STEREO_MONO_THRES            (ADDR_DRAM_SHARED_MEMORY_CTRLD+8)

#define  ADDR_MANUAL_OUTPUT_MATRIX          (ADDR_DRAM_SHARED_MEMORY_CTRLD+9)
#define  ADDR_MANUAL_MTS_SW         (ADDR_DRAM_SHARED_MEMORY_CTRLD+10)
#define  ADDR_FM_NON_EU_MODE          (ADDR_DRAM_SHARED_MEMORY_CTRLD+0x0B)
#define  ADDR_DETECTED_TYPE                   (ADDR_DRAM_SHARED_MEMORY_CTRLD+0x0C) 
#define  ADDR_DETECTED_TYPE_HDEV             (ADDR_DRAM_SHARED_MEMORY_CTRLD+0x0D) 
#define ADDR_PAL_NOT_FIRST_AOUT_INIT     (ADDR_DRAM_SHARED_MEMORY_CTRLD+0x0E)
#define ADDR_PAL_HDEV_BAD_SIGNAL_CHECK     (ADDR_DRAM_SHARED_MEMORY_CTRLD+0x0F)
//===============================================================================
//DRAM_RISC_SHARED_MEMORY_CTRLD 81F0H~81FFH
//===============================================================================
#define ADDR_DRAM_SHARED_MEMORY_CTRLE      (ADDR_RC2D_SHARED_MEMORY + 0x00F0)
//MTS DETECTION
#define ADDR_MTS_NUM_PILOT               (ADDR_DRAM_SHARED_MEMORY_CTRLE+0) 
#define ADDR_MTS_NUM_STEREO               (ADDR_DRAM_SHARED_MEMORY_CTRLE+1)  
#define ADDR_MTS_NUM_SAP              (ADDR_DRAM_SHARED_MEMORY_CTRLE+2) 
#define ADDR_MTS_PILOT_OFFSET_DETECTION              (ADDR_DRAM_SHARED_MEMORY_CTRLE+3) 
#define ADDR_MTS_MUTE_SAP_LOW                (ADDR_DRAM_SHARED_MEMORY_CTRLE+4) 
#define ADDR_MTS_MUTE_SAP_HIGH                (ADDR_DRAM_SHARED_MEMORY_CTRLE+5) 
#define ADDR_MTS_SATU_MUTE_HIGH                (ADDR_DRAM_SHARED_MEMORY_CTRLE+6) 
#define ADDR_MTS_SATU_MUTE_LOW                (ADDR_DRAM_SHARED_MEMORY_CTRLE+7)
#define ADDR_FM_ONLY                (ADDR_DRAM_SHARED_MEMORY_CTRLE+8)
#define ADDR_DEFAULT_MTS_MODE                (ADDR_DRAM_SHARED_MEMORY_CTRLE+9)   // default mts mode for US model.
                                                                                                                                           // 0:no default, 1:stereo, 2 SAP      
#define  ADDR_MANUAL_OUTPUT_MATRIX_DEC2          (ADDR_DRAM_SHARED_MEMORY_CTRLE+0x0A)                                                                                                                                           // 0:no default, 1:stereo, 2 SAP      
#define ADDR_MTS_SAP_HP_NOISE_LOW                     (ADDR_DRAM_SHARED_MEMORY_CTRLE+0x0B)  
#define ADDR_MTS_SAP_HP_NOISE_HIGH                    (ADDR_DRAM_SHARED_MEMORY_CTRLE+0x0C)  
#define ADDR_MTS_SAP_FILTER_SEL                            (ADDR_DRAM_SHARED_MEMORY_CTRLE+0x0D)  
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
#define ADDR_MTS_OUTPUT_LEVEL_GAIN                     (ADDR_DRAM_SHARED_MEMORY_CTRLE+0x0E) 
#endif
#define ADDR_MTS_SAP_RATIO                                     (ADDR_DRAM_SHARED_MEMORY_CTRLE+0x0F)  
#endif

/* first decoder part */
#define ADDR_RC2D_DRAM_BASS_MANAGEMENT_BASE         (DRAM_NORMAL_BASE + 0x0220)
#define ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE        (DRAM_NORMAL_BASE + 0x0280)
#define ADDR_RC2D_DRAM_DOWNMIX_BASE                 (DRAM_NORMAL_BASE + 0x0290)
#define ADDR_RC2D_DRAM_PTS_STC_BASE                 (DRAM_NORMAL_BASE + 0x02B0)
#define ADDR_RC2D_DRAM_KARAOKE_BASE                 (DRAM_NORMAL_BASE + 0x02C0)
#define ADDR_RC2D_DRAM_EQUALIZER_BASE               (DRAM_NORMAL_BASE + 0x02D0)
#define ADDR_RC2D_AUDIO_BASIC_BASE                  (DRAM_NORMAL_BASE + 0x03F0)
#define ADDR_RC2D_DRAM_VS_BUF_BASE                  (DRAM_NORMAL_BASE + 0x0410)
#define ADDR_RC2D_SUPER_BASS_BASE                   (DRAM_NORMAL_BASE + 0x0510)
#define ADDR_RC2D_DRAM_REVERB_BASE                  (DRAM_NORMAL_BASE + 0x05F0)

#define ADDR_RC2D_DRAM_UPMIX_BASE                   (DRAM_NORMAL_BASE + 0x0610)

#define ADDR_RC2D_DRAM_MIXSOUND_BASE                (DRAM_NORMAL_BASE + 0x0710)
#define ADDR_RC2D_DRAM_MEM_SW                       (DRAM_NORMAL_BASE + 0x0800)

/* second decoder part */
#define ADDR_RC2D_DRAM_BASS_MANAGEMENT_BASE_DEC2    (DRAM_NORMAL_BASE + 0x0830)
#define ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE_DEC2   (DRAM_NORMAL_BASE + 0x0880)
#define ADDR_RC2D_DRAM_DOWNMIX_BASE_DEC2            (DRAM_NORMAL_BASE + 0x0890)
#define ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2            (DRAM_NORMAL_BASE + 0x08B0)
#define ADDR_RC2D_DRAM_KARAOKE_BASE_DEC2            (DRAM_NORMAL_BASE + 0x08C0)
#define ADDR_RC2D_DRAM_EQUALIZER_BASE_DEC2          (DRAM_NORMAL_BASE + 0x08D0)
#define ADDR_RC2D_AUDIO_BASIC_BASE_DEC2             (DRAM_NORMAL_BASE + 0x09F0)
#define ADDR_RC2D_DRAM_VS_BUF_BASE_DEC2             (DRAM_NORMAL_BASE + 0x0a10)

#define DRAM_VIRTUAL_SURROUND_DEC2            (ADDR_RC2D_DRAM_VS_BUF_BASE_DEC2)
#define DRAM_QSOUND_DEC2                      (ADDR_RC2D_DRAM_VS_BUF_BASE_DEC2)


#define ADDR_RC2D_VBASS_BASE                        (DRAM_NORMAL_BASE + 0x0af0)
#define ADDR_RC2D_SUPER_BASS_BASE_DEC2              (DRAM_NORMAL_BASE + 0x0b10)
#define ADDR_RC2D_DRAM_REVERB_BASE_DEC2             (DRAM_NORMAL_BASE + 0x0bF0)


#define ADDR_RC2D_DRAM_UPMIX_BASE_DEC2              (DRAM_NORMAL_BASE + 0x0c10)

//#define ADDR_RC2D_SPKCAL_BASE                          (DRAM_NORMAL_BASE + 0x0c80)
#define ADDR_RC2D_SPKCAL_BASE                          (DRAM_NORMAL_BASE + 0x0c10)//(DRAM_NORMAL_BASE + 0x0c30)

#ifdef CC_AUD_SPEAKER_HEIGHT
#define ADDR_RC2D_SPKHEIGHT_BASE                          (DRAM_NORMAL_BASE + 0x0d70)//cb0
#endif

/* Misc part */
#define ADDR_RC2D_DRAM_AVC_BASE                     (DRAM_NORMAL_BASE + 0x0d10)
#define ADDR_RC2D_DRAM_AVC_BASE_DEC2                (DRAM_NORMAL_BASE + 0x0d70)

#define ADDR_RC2D_DCF_BASE                          (DRAM_NORMAL_BASE + 0x0dD0)
#define ADDR_RC2D_DCF_BASE_DEC2                     (DRAM_NORMAL_BASE + 0x0E00)

// Limiter (8e30h~8e50h)
#define ADDR_RC2D_LIMITER_GAIN_MODE                 (DRAM_NORMAL_BASE + 0x0e30) // 1
// 0: Limiter off, 1: Adaptive mode, 2: Fix mode
#define ADDR_RC2D_LIMITER_FLAG                      (ADDR_RC2D_LIMITER_GAIN_MODE+1) // 1
#define ADDR_RC2D_LIMITER_FIX_THRESHOLD             (ADDR_RC2D_LIMITER_FLAG+1) // 1
#ifdef CC_RATIO_LIMITER
#define ADDR_RC2D_LIMITER_GAIN_RATIO                (ADDR_RC2D_LIMITER_FIX_THRESHOLD+1) // 1
#define ADDR_RC2D_LIMITER_GAIN_BASE                 (ADDR_RC2D_LIMITER_GAIN_RATIO+1)
#else
#define ADDR_RC2D_LIMITER_GAIN_BASE                 (ADDR_RC2D_LIMITER_FIX_THRESHOLD+1)
#endif
#define ADDR_RC2D_LIMITER_GAIN_VOL                  (ADDR_RC2D_LIMITER_GAIN_BASE) // 2
#define ADDR_RC2D_LIMITER_GAIN_VSURR                (ADDR_RC2D_LIMITER_GAIN_VOL+2) // 2
#define ADDR_RC2D_LIMITER_GAIN_SBASS                (ADDR_RC2D_LIMITER_GAIN_VSURR+2) // 2
#define ADDR_RC2D_LIMITER_GAIN_EQ                   (ADDR_RC2D_LIMITER_GAIN_SBASS+2) // 2
#define ADDR_RC2D_LIMITER_GAIN_REVERB               (ADDR_RC2D_LIMITER_GAIN_EQ+2)  // 2
#define ADDR_RC2D_LIMITER_GAIN_SPEAKER              (ADDR_RC2D_LIMITER_GAIN_REVERB+2) // 2
#define ADDR_RC2D_LIMITER_GAIN_VOICE                (ADDR_RC2D_LIMITER_GAIN_SPEAKER+2) // 2

// CFO related parameters (8e50h~8e60h)

// BBE VIVA 4 (8e60h ~ 8e90h)
#define ADDR_BBEV4_FLAG                         (DRAM_NORMAL_BASE + 0x0e60)  // 1
#define ADDR_BBEV4_LEVEL_COEF                   (ADDR_BBEV4_FLAG+1)  // 1
#define ADDR_BBEV4_3D_GAIN                      (ADDR_BBEV4_LEVEL_COEF+1)      // 1
#define ADDR_BBEV4_BBE_F                        (ADDR_BBEV4_3D_GAIN+1)      // 1
#define ADDR_BBEV4_LOW_COEF                     (ADDR_BBEV4_BBE_F+1)        // 1
#define ADDR_BBEV4_HIGH_COEF                    (ADDR_BBEV4_LOW_COEF+1)     // 1

#define ADDR_BBEV4_BBE_ON                       (ADDR_BBEV4_HIGH_COEF+1)  //1
#define ADDR_BBEV4_LOW_ON                       (ADDR_BBEV4_BBE_ON+1)      // 1
#define ADDR_BBEV4_MID_ON                       (ADDR_BBEV4_LOW_ON+1)       // 1
#define ADDR_BBEV4_HIGH_ON                      (ADDR_BBEV4_MID_ON+1)       // 1

#define ADDR_BBEV4_IIR_COEF                     (ADDR_BBEV4_HIGH_ON+1)    // 5
#define ADDR_BBEV4_VAR_BACKUP                   (ADDR_BBEV4_IIR_COEF+5)   // 16

// channel delay related (8e90h ~ 8ea0h)

// NR related  (8ec0h ~ 8ecfh)
#define ADDR_RC2D_NR_PARAM_BASE                 (DRAM_NORMAL_BASE + 0x0ec0)
#define ADDR_RC2D_NR_ENABLE                     (ADDR_RC2D_NR_PARAM_BASE + 0x0)
#define ADDR_RC2D_NR_PAL_TH1_HIGH               (ADDR_RC2D_NR_PARAM_BASE + 0x1)
#define ADDR_RC2D_NR_PAL_TH1_LOW                (ADDR_RC2D_NR_PARAM_BASE + 0x2)
#define ADDR_RC2D_NR_PAL_TH2_HIGH               (ADDR_RC2D_NR_PARAM_BASE + 0x3)
#define ADDR_RC2D_NR_PAL_TH2_LOW                (ADDR_RC2D_NR_PARAM_BASE + 0x4)
#define ADDR_RC2D_NR_PAL_HDEV_TH1_HIGH          (ADDR_RC2D_NR_PARAM_BASE + 0x5)
#define ADDR_RC2D_NR_PAL_HDEV_TH1_LOW           (ADDR_RC2D_NR_PARAM_BASE + 0x6)
#define ADDR_RC2D_NR_PAL_HDEV_TH2_HIGH          (ADDR_RC2D_NR_PARAM_BASE + 0x7)
#define ADDR_RC2D_NR_PAL_HDEV_TH2_LOW           (ADDR_RC2D_NR_PARAM_BASE + 0x8)
#define ADDR_RC2D_NR_AM_TH1_HIGH                (ADDR_RC2D_NR_PARAM_BASE + 0x9)
#define ADDR_RC2D_NR_AM_TH1_LOW                 (ADDR_RC2D_NR_PARAM_BASE + 0xa)
#define ADDR_RC2D_NR_AM_TH2_HIGH                (ADDR_RC2D_NR_PARAM_BASE + 0xb)
#define ADDR_RC2D_NR_AM_TH2_LOW                 (ADDR_RC2D_NR_PARAM_BASE + 0xc)

#define ADDR_RC2D_NR_A2_TH1_HIGH                (ADDR_RC2D_NR_PARAM_BASE + 0x1)
#define ADDR_RC2D_NR_A2_TH1_LOW                 (ADDR_RC2D_NR_PARAM_BASE + 0x2)
#define ADDR_RC2D_NR_A2_TH2_HIGH                (ADDR_RC2D_NR_PARAM_BASE + 0x3)
#define ADDR_RC2D_NR_A2_TH2_LOW                 (ADDR_RC2D_NR_PARAM_BASE + 0x4)

#define ADDR_RC2D_NR_MTS_TH1_HIGH               (ADDR_RC2D_NR_PARAM_BASE + 0x1)
#define ADDR_RC2D_NR_MTS_TH1_LOW                (ADDR_RC2D_NR_PARAM_BASE + 0x2)
#define ADDR_RC2D_NR_MTS_TH2_HIGH               (ADDR_RC2D_NR_PARAM_BASE + 0x3)
#define ADDR_RC2D_NR_MTS_TH2_LOW                (ADDR_RC2D_NR_PARAM_BASE + 0x4)
#define ADDR_RC2D_NR_SAP_TH1_HIGH               (ADDR_RC2D_NR_PARAM_BASE + 0x5)
#define ADDR_RC2D_NR_SAP_TH1_LOW                (ADDR_RC2D_NR_PARAM_BASE + 0x6)
#define ADDR_RC2D_NR_SAP_TH2_HIGH               (ADDR_RC2D_NR_PARAM_BASE + 0x7)
#define ADDR_RC2D_NR_SAP_TH2_LOW                (ADDR_RC2D_NR_PARAM_BASE + 0x8)

//#define ADDR_RC2D_DTS_DOWNMIX_BASE        (DRAM_NORMAL_BASE + 0x0ea0)

#if 0 
#define ADDR_RC2D_DRAM_RESERVED_2                   (DRAM_NORMAL_BASE + 0x0e90)
#else

// Post processing parameters (8eech ~ 8effh)
#define ADDR_RC2D_POST_PROC_PARAM_BASE  (DRAM_NORMAL_BASE + 0x0eec)
#define ADDR_RC2D_POST_PROC_ENABLE      (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x0000)
    #define ADDR_RC2D_POST_PROC_HPF_BIT        (0x100U << 0)
    #define ADDR_RC2D_POST_PROC_CLIPPER_BIT    (0x100U << 1)
    #define ADDR_RC2D_POST_PROC_LIMITER_BIT    (0x100U << 2)
#define ADDR_RC2D_POST_PROC_CLIPPER_C_VAL    (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x0001)
#define ADDR_RC2D_POST_PROC_CLIPPER_L_VAL    (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x0002)
#define ADDR_RC2D_POST_PROC_CLIPPER_R_VAL    (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x0003)
#define ADDR_RC2D_POST_PROC_CLIPPER_LS_VAL   (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x0004)
#define ADDR_RC2D_POST_PROC_CLIPPER_RS_VAL   (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x0005)
#define ADDR_RC2D_POST_PROC_CLIPPER_SW_VAL   (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x0006)
#define ADDR_RC2D_POST_PROC_CLIPPER_CH7_VAL  (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x0007)
#define ADDR_RC2D_POST_PROC_CLIPPER_CH8_VAL  (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x0008)
#define ADDR_RC2D_POST_PROC_CLIPPER_CH9_VAL  (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x0009)
#define ADDR_RC2D_POST_PROC_CLIPPER_CH10_VAL (ADDR_RC2D_POST_PROC_PARAM_BASE + 0x000a)


// Decoder 3 section (8f00h ~ 8fffh)
// 8f00h~8f7fh, dec3 share info
// 8f80h~8fffh, dec3 share memory

#define ADDR_RC2D_CTRL7_BASE                 (DRAM_RC2D_SHARED_MEMORY_DEC3 + 0x0000)
#define ADDR_RC2D_CTRL8_BASE                 (DRAM_RC2D_SHARED_MEMORY_DEC3 + 0x0010)
#define ADDR_RC2D_DECODER_INFO_BASE_DEC3     (DRAM_RC2D_SHARED_MEMORY_DEC3 + 0x0020)
#define ADDR_RC2D_MISC_BASE_DEC3             (DRAM_RC2D_SHARED_MEMORY_DEC3 + 0x0030)

#endif

/************************< Compact DRAM Allocation >***************************/

/* first decoder part */
#define CMPT_REVERB_BUFFER                            (DRAM_CMPT_BASE + 0x0000)
#define CMPT_POST_TEMP_BUFFER                         (DRAM_CMPT_BASE + 0x0F00)
#define CMPT_SMOOTH_BUFFER                            (DRAM_CMPT_BASE + 0x1300)
// without keyshift in DTV
//#define CMPT_KEY_PCM_BUFFER                         (DRAM_CMPT_BASE + 0x1000)
#define CMPT_BUFFER_ADDRESS_UPMIX                     (DRAM_CMPT_BASE + 0x1600)
#define CMPT_BUFFER_ADDRESS_DONWMIX                   (DRAM_CMPT_BASE + 0x1700)
#define CMPT_VSURR_DELAY_BUFFER                       (DRAM_CMPT_BASE + 0x1900)
//#define CMPT_NEO6_BUFFER                            (DRAM_CMPT_BASE + 0x34c0)

/* second decoder part */
#define CMPT_REVERB_BUFFER_DEC2                       (DRAM_CMPT_BASE + 0x5000)
#define CMPT_SMOOTH_BUFFER_DEC2                       (DRAM_CMPT_BASE + 0x5C00)
#define CMPT_POST_TEMP_BUFFER_DEC2                    (DRAM_CMPT_BASE + 0x5F00)
// without keyshift in DTV
//#define CMPT_KEY_PCM_BUFFER_DEC2                    (DRAM_CMPT_BASE + 0xC000)
#define CMPT_BUFFER_ADDRESS_UPMIX_DEC2                (DRAM_CMPT_BASE + 0x6300)
#define CMPT_BUFFER_ADDRESS_DONWMIX_DEC2              (DRAM_CMPT_BASE + 0x6700)

/* MixSound offset address */
//#define DSP_MIXSOUND_OFFSET_ADDR                    (DRAM_CMPT_BASE + 0xF000)


/************************< Normal DRAM Definition >****************************/

//=================================================
// DRAM_RISC_SHARED_MEMORY Section (08100h~08200h)
//=================================================

//### CTRL1
#define ADDR_RC2D_CTRL_1_NUM_1                  (ADDR_RC2D_CTRL1_BASE + 0x0000)
//Interrupt Control
#define ADDR_RC2D_INTERRUPT_CTRL                (ADDR_RC2D_CTRL1_BASE + 0x0001)
#define ADDR_RC2D_SYSTEM_CTRL                   (ADDR_RC2D_CTRL1_BASE + 0x0002)
#define ADDR_RC2D_SYSTEM_SETUP                  (ADDR_RC2D_CTRL1_BASE + 0x0003)
#define ADDR_RC2D_SYSTEM_SETUP_DEC2             (ADDR_RC2D_CTRL1_BASE + 0x0004)
#define ADDR_RC2D_CH_TRIM                       (ADDR_RC2D_CTRL1_BASE + 0x0005)  // 11
#define ADDR_RC2D_CH_TRIM_CH9                   (ADDR_RC2D_CTRL1_BASE + 0x000D)
#define ADDR_RC2D_CH_TRIM_CH10                  (ADDR_RC2D_CTRL1_BASE + 0x000E)
#define ADDR_RC2D_CH_TRIM_LFE                   (ADDR_RC2D_CTRL1_BASE + 0x000F)
#define ADDR_RC2D_DIALOGUE_GAIN                 (ADDR_RC2D_CTRL1_BASE + 0x0010)
#define ADDR_RC2D_DIALOGUE_GAIN_DEC2            (ADDR_RC2D_CTRL1_BASE + 0x0011)
#define ADDR_RC2D_CH_TRIM_DEC2                  (ADDR_RC2D_CTRL1_BASE + 0x0012)  // 3
#define ADDR_RC2D_CH_TRIM_DEC2_L                (ADDR_RC2D_CH_TRIM_DEC2)         //
#define ADDR_RC2D_CH_TRIM_DEC2_R                (ADDR_RC2D_CH_TRIM_DEC2 + 0x01)  //
#define ADDR_RC2D_CH_TRIM_DEC2_LFE              (ADDR_RC2D_CH_TRIM_DEC2 + 0x02)  //

#define ADDR_RD2D_POWERON_RETRY_COUNT           (ADDR_RC2D_CTRL1_BASE + 0x001e)
#define ADDR_RD2D_INTERNAL_BUF_IN_FLAG          (ADDR_RC2D_CTRL1_BASE + 0x0015)

#if 1
//#ifdef CC_MT5391_AUD_3_DECODER
#define ADDR_RC2D_SYSTEM_SETUP_DEC3             (ADDR_RC2D_CTRL1_BASE + 0x0017)
#define ADDR_RC2D_CH_TRIM_DEC3_L                    (ADDR_RC2D_CTRL1_BASE + 0x0018)   
#define ADDR_RC2D_CH_TRIM_DEC3_R                   (ADDR_RC2D_CTRL1_BASE + 0x0019) 
#define ADDR_RD2D_INTERNAL_BUF2_IN_FLAG          (ADDR_RC2D_CTRL1_BASE + 0x001A)
#endif


//### Trap Setting
#define ADDR_RC2D_TRAP_BPEN                     (ADDR_RC2D_TRAP_BASE + 0x0000)
#define TRAP_BPEN                                                      0xFF00
#define ADDR_RC2D_TRAP_BP0                      (ADDR_RC2D_TRAP_BASE + 0x0001)
#define ADDR_RC2D_TRAP_BPT0                     (ADDR_RC2D_TRAP_BASE + 0x0002)
#define TRAP_BP0                                                       0x0000
#define TRAP_BPT0                                                      0x0000
#define ADDR_RC2D_TRAP_BP1                      (ADDR_RC2D_TRAP_BASE + 0x0003)
#define ADDR_RC2D_TRAP_BPT1                     (ADDR_RC2D_TRAP_BASE + 0x0004)
#define TRAP_BP1                                                       0x0000
#define TRAP_BPT1                                                      0x0000
#define ADDR_RC2D_TRAP_BP2                      (ADDR_RC2D_TRAP_BASE + 0x0005)
#define ADDR_RC2D_TRAP_BPT2                     (ADDR_RC2D_TRAP_BASE + 0x0006)
#define TRAP_BP2                                                       0x0000
#define TRAP_BPT2                                                      0x0000
#define ADDR_RC2D_TRAP_BP3                      (ADDR_RC2D_TRAP_BASE + 0x0007)
#define ADDR_RC2D_TRAP_BPT3                     (ADDR_RC2D_TRAP_BASE + 0x0008)
#define TRAP_BP3                                                       0x0000
#define TRAP_BPT3                                                      0x0000
#define ADDR_RC2D_TRAP_BP4                      (ADDR_RC2D_TRAP_BASE + 0x0009)
#define ADDR_RC2D_TRAP_BPT4                     (ADDR_RC2D_TRAP_BASE + 0x000A)
#define TRAP_BP4                                                       0x0000
#define TRAP_BPT4                                                      0x0000
#define ADDR_RC2D_TRAP_BP5                      (ADDR_RC2D_TRAP_BASE + 0x000B)
#define ADDR_RC2D_TRAP_BPT5                     (ADDR_RC2D_TRAP_BASE + 0x000C)
#define TRAP_BP5                                                       0x0000
#define TRAP_BPT5                                                      0x0000
#define ADDR_RC2D_TRAP_BP6                      (ADDR_RC2D_TRAP_BASE + 0x000D)
#define ADDR_RC2D_TRAP_BPT6                     (ADDR_RC2D_TRAP_BASE + 0x000E)
#define TRAP_BP6                                                       0x0000
#define TRAP_BPT6                                                      0x0000
#define ADDR_RC2D_TRAP_BP7                      (ADDR_RC2D_TRAP_BASE + 0x000F)
#define ADDR_RC2D_TRAP_BPT7                     (ADDR_RC2D_TRAP_BASE + 0x0010)
#define TRAP_BP7                                                       0x0000
#define TRAP_BPT7                                                      0x0000

//### CTRL2
#define ADDR_RC2D_CTRL_2_NUM_1                  (ADDR_RC2D_CTRL2_BASE + 0x0000)
#define ADDR_RC2D_STC_DIFF                      (ADDR_RC2D_CTRL2_BASE + 0x0001)
#define ADDR_RC2D_SPEED                         (ADDR_RC2D_CTRL2_BASE + 0x0002)
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
#define ADDR_RC2D_PROCESSING_MODE               (ADDR_RC2D_CTRL2_BASE + 0x0003)
#define BM_CLRS_MIX_TO_LFE                                           (0x1 << 4)
#define BASS_CH_MIX_TO_LFE                                           (0x1 << 5)
#define LFE_INPUT_GAIN_0_DB                                          (0x1 << 6)
#define LFE_REDIRECT_TO_LARGE_SPKR                                   (0x1 << 7)
#define LFE_ALWAYS_DO_IIR                                            (0x1 << 8)
#define ADDR_RC2D_BIT_RESOLUTION                (ADDR_RC2D_CTRL2_BASE + 0x0004)
//L/R Channel Mixing and Microphone Level Threshold
#define ADDR_RC2D_KARAOKE_FLAG                  (ADDR_RC2D_CTRL2_BASE + 0x0005)
  //bit 0: L/R Mixing
  //bit 1: Voice Mute
  //bit 2: Vocal Assist
#define ADDR_RC2D_LRCH_MIX_RATIO                (ADDR_RC2D_CTRL2_BASE + 0x0006)
  //Value: 0x800000~0
#define ADDR_RC2D_MASTER_VOLUME                 (ADDR_RC2D_CTRL2_BASE + 0x000A)
#define ADDR_RC2D_SPDIF_FLAG                    (ADDR_RC2D_CTRL2_BASE + 0x000B)
#define ADDR_RC2D_MASTER_VOLUME_BACKUP          (ADDR_RC2D_CTRL2_BASE + 0x000C)
#define ADDR_RC2D_DECODING_STR_PNT              (ADDR_RC2D_CTRL2_BASE + 0x000D)

//### CTRL3
#define DSP_RC2D_CTRL3_AREA_SZ                  12
#define ADDR_RC2D_CTRL_3_NUM_1                  (ADDR_RC2D_CTRL3_BASE + 0x000)
#define ADDR_RC2D_DOWNSAMPLE                    (ADDR_RC2D_CTRL3_BASE + 0x001)   // 30
//Channel Use -> Pink Noise
#define ADDR_RC2D_CHANNEL_USE                   (ADDR_RC2D_CTRL3_BASE + 0x002)   // 31
#define RC2D_INT32                              (ADDR_RC2D_CTRL3_BASE + 0x003)
//Dolby Karaoke Mode
#define ADDR_RC2D_AC3_KARAOKE_MODE                                (RC2D_INT32)
#define ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT                         (RC2D_INT32)
#define ADDR_RC2D_CDDA_CFGDEEMPHASIS                              (RC2D_INT32)
#define ADDR_RC2D_WMA_PACKET_NO                               (RC2D_INT32)
#define ADDR_RC2D_SACD_FILTER_LENGTH                              (RC2D_INT32)
#define ADDR_RC2D_VORBIS_PACKET_NO                                (RC2D_INT32)
#define ADDR_RC2D_MP3_NORMAL_PLAY                                 (RC2D_INT32)
#define ADDR_RC2D_PCM_INPUT_SAMPLING_RATE                         (RC2D_INT32)
#define RC2D_INT33                              (ADDR_RC2D_CTRL3_BASE + 0x004)
  //OFF: 0x1 <<8, V1,V2: 0x2 <<8, None: 0x4 <<8
  //V1: 0x8 <<8, V2: 0x10 <<8, V1+V2: 0x20 <<8
//Dolby Dual Mode
#define ADDR_RC2D_AC3_DUAL_MODE                                   (RC2D_INT33)
#define ADDR_RC2D_LPCM_FREQUENCY                                  (RC2D_INT33)
#define ADDR_RC2D_HDCD_CONFIG                                     (RC2D_INT33)
#define ADDR_RC2D_SACD_DOWN_SAMPLE                                (RC2D_INT33)
#define ADDR_RC2D_WMA_HEADER_SOURCE                                (RC2D_INT33)
#define ADDR_RC2D_MPEG_SYNC_MODE                                  (RC2D_INT33)
#define ADDR_RC2D_MP3_SYNC_MODE                                   (RC2D_INT33)
#define ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT                         (RC2D_INT33) // Light added
#define RC2D_INT34                              (ADDR_RC2D_CTRL3_BASE + 0x005)
  //Stereo: 0x0, Ch0 Only: 0x1 <<8
  //Ch1 Only: 0x2 <<8, Ch0+Ch1 Mixing: 0x3 <<8
//Dolby Compression
#define ADDR_RC2D_AC3_COMPRESS_MODE                               (RC2D_INT34)
#define ADDR_RC2D_LPCM_BIT_SHIFT                                  (RC2D_INT34)
#define ADDR_RC2D_HDCD_CFGUPSAMPLING                              (RC2D_INT34)
#define ADDR_RC2D_SACD_CH_NUM                                     (RC2D_INT34)
#define ADDR_RC2D_WMA_PACKET_COUNT                                 (RC2D_INT34)
#define ADDR_RC2D_PCM_INPUT_CHANNEL_NUM                           (RC2D_INT34) // Light added
#define ADDR_RC2D_MPEG_DEEMPHASIS_MODE                            (RC2D_INT34)
#define ADDR_RC2D_MP3_DEEMPHASIS_MODE                             (RC2D_INT34)
#define RC2D_INT35                              (ADDR_RC2D_CTRL3_BASE + 0x006)
  //Custom Mode 0: 0x0, Custom Mode 1: 0x1 <<8
  //Line Mode: 0x2 <<8, RF Mode: 0x3 <<8
#define ADDR_RC2D_AC3_DYNAMIC_LOW	                              (RC2D_INT35)
#define ADDR_RC2D_DRC_VALUE                                       (RC2D_INT35)
#define ADDR_RC2D_LPCM_DRC_VALUE                                  (RC2D_INT35)
#define ADDR_RC2D_HDCD_CFGDITHMODE                                (RC2D_INT35)
#define ADDR_RC2D_SACD_DOWN_SAMPLE_ORDER                          (RC2D_INT35)
#define ADDR_RC2D_WMA_PACKET_SIZE                                     (RC2D_INT35)
#define ADDR_RC2D_MPEG_CRC_MODE                                   (RC2D_INT35)
#define ADDR_RC2D_MP3_CRC_MODE                                    (RC2D_INT35)
#define ADDR_RC2D_PCM_INPUT_TYPE                                    (RC2D_INT35)
#define RC2D_INT36                              (ADDR_RC2D_CTRL3_BASE + 0x007)
#define ADDR_RC2D_AC3_DYNAMIC_HIGH                                (RC2D_INT36)
#define ADDR_RC2D_LPCM_Q                                          (RC2D_INT36)
#define ADDR_RC2D_SACD_OUTPUT_MODE                                (RC2D_INT36)
#define ADDR_RC2D_WMA_VERSION                                     (RC2D_INT36)
#define ADDR_RC2D_MP3_PREPARSING_MODE                             (RC2D_INT36)
#define ADDR_RC2D_ADPCM_BLOCK_ALIGN                               (RC2D_INT36)
#define ADDR_RC2D_PCM_PREBUF_DELAY_BANK                           (RC2D_INT36)
#define RC2D_INT37                              (ADDR_RC2D_CTRL3_BASE + 0x008)
//DTS Dynamic Range Control Flag
#define ADDR_RC2D_DTS_DRC_FLAG                                    (RC2D_INT37)
#define ADDR_RC2D_MPEG_DRC_FLAG                                   (RC2D_INT37)
#define ADDR_RC2D_LPCM_DRC_FLAG                                   (RC2D_INT37)
#define ADDR_RC2D_AC3_DOWNMIX                                     (RC2D_INT37)
#define ADDR_RC2D_SACD_INPUT_FRAME                                (RC2D_INT37)
#define ADDR_RC2D_HDCD_USRDATA_RPT                                (RC2D_INT37)
#define ADDR_RC2D_WMA_NUM_CHANNELS                           (RC2D_INT37)
#define ADDR_RC2D_MP3_SMOOTH_FLAG                                 (RC2D_INT37)
#define ADDR_RC2D_ADPCM_CHANNEL_NUM                               (RC2D_INT37)
#define RC2D_INT38                              (ADDR_RC2D_CTRL3_BASE + 0x009)
  //OFF: 0x0, AUTO: 0x1 <<8
//MPEG CRC Value
#define ADDR_RC2D_CRC_FLAG                                        (RC2D_INT38)
#define ADDR_RC2D_LPCM_DECODING_TYPE                              (RC2D_INT38)
#define ADDR_RC2D_WMA_SAMPLE_PER_SECOND                            (RC2D_INT38)
#define ADDR_RC2D_SACD_INPUT_DSD_MODE                             (RC2D_INT38)
#define SACD_INPUT_DSD                                                   (0x1)
#define SACD_INPUT_DST                                                   (0x0)
#define ADDR_RC2D_MP3_QUALITY_FLAG                                (RC2D_INT38)
#define ADDR_RC2D_AC3_BYPASS_FLAG                                 (RC2D_INT38)
#define ADDR_RC2D_LPCM_ADPCM_DEC_TYPE                             (RC2D_INT38)
#define RC2D_INT39                              (ADDR_RC2D_CTRL3_BASE + 0x00A)
#define ADDR_RC2D_WMA_BYTES_PER_SECOND                                  (RC2D_INT39)
#define ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE                      (RC2D_INT39)
#define RC2D_INT3A                              (ADDR_RC2D_CTRL3_BASE + 0x00B)
#define ADDR_RC2D_WMA_BLOCK_SIZE                             (RC2D_INT3A)
#define ADDR_RC2D_PCM_LRCK_MODE                                   (RC2D_INT3A)

//koro: 20080919 cannot use INT3B in 539x and 536x
#define RC2D_INT3B                              (ADDR_RC2D_CTRL3_BASE + 0x00C)
#define ADDR_RC2D_WMA_ENCODER_OPTIONS                             (RC2D_INT3B)
//#define ADDR_RC2D_LPCM_ADPCM_DEC_TYPE                             (RC2D_INT3B)

//### Decoder Info
#define DSP_RC2D_SETTING_AREA_SZ            (0x10)
#ifdef DSP_SUPPORT_NPTV
// Demodulation Parameters
#define ADDR_DEM_SOURCE_MODE                (ADDR_RC2D_DECODER_INFO_BASE+0x0000)
#define ADDR_DEM_USER_MODE                  (ADDR_RC2D_DECODER_INFO_BASE+0x0001)
#define ADDR_DEM_ADC_GAIN                   (ADDR_RC2D_DECODER_INFO_BASE+0x0002)
#define ADDR_DEM_ENFORCED_MODE               (ADDR_RC2D_DECODER_INFO_BASE+0x0003)
// Detection parameters for Top Level Detection
#define ADDR_DETECTOR_DEFAULT_MODE           (ADDR_RC2D_DECODER_INFO_BASE) //0 -> PAL, 1->SECAM L; 8150h
#define ADDR_DETECTOR_START_FLAG               (ADDR_RC2D_DECODER_INFO_BASE+0x0001) //0->OFF, 1->ON; 8151h
#define ADDR_FM_RDO_DET_INBAND_TH            (ADDR_RC2D_DECODER_INFO_BASE+0x0002)
#define ADDR_FM_RDO_DET_NOISE_TH               (ADDR_RC2D_DECODER_INFO_BASE+0x0003)

#define ADDR_DEM_PAL_FINE_TUNE_VOLUME (ADDR_RC2D_DECODER_INFO_BASE+0x0004)   
#define  ADDR_DEM_SAP_FINE_TUNE_VOLUME       (ADDR_RC2D_DECODER_INFO_BASE+0x0004)  
#define ADDR_DEM_NICAM_FINE_TUNE_VOLUME (ADDR_RC2D_DECODER_INFO_BASE+0x0005)
#define ADDR_DEM_FMFM_DUAL_FINE_TUNE_VOLUME       (ADDR_RC2D_DECODER_INFO_BASE+0x0005) 
#define ADDR_DEM_AM_FINE_TUNE_VOLUME (ADDR_RC2D_DECODER_INFO_BASE+0x0006)
#define  ADDR_DEM_MTS_FINE_TUNE_VOLUME       (ADDR_RC2D_DECODER_INFO_BASE+0x0006)  
#define ADDR_SECAM_L_MUTE_MODE               (ADDR_RC2D_DECODER_INFO_BASE+0x0007)  
#define ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT    (ADDR_RC2D_DECODER_INFO_BASE+0x0008)  
#define ADDR_SECAM_L_MUTE_THRESHOLD_LOW       (ADDR_RC2D_DECODER_INFO_BASE+0x0009)  
//FMFM DETECTION
#define ADDR_FMFM_NUM_CHECK                  (ADDR_RC2D_DECODER_INFO_BASE+0x000A)
#define ADDR_FMFM_NUM_DOUBLE_CHECK           (ADDR_RC2D_DECODER_INFO_BASE+0x000B)
#define ADDR_FMFM_MONO_WEIGHT                (ADDR_RC2D_DECODER_INFO_BASE+0x000C)
#define ADDR_FMFM_STEREO_WEIGHT              (ADDR_RC2D_DECODER_INFO_BASE+0x000D)
#define ADDR_FMFM_DUAL_WEIGHT                (ADDR_RC2D_DECODER_INFO_BASE+0x000E)
#define ADDR_FMFM_DETECTION_CONFID          (ADDR_RC2D_DECODER_INFO_BASE+0x000F) 

#endif // DSP_SUPPORT_NPTV

// DVD AUDIO related parameters
// Could be shared Risc to Dsp Decoder Share info II (size : 0x10)
#define ADDR_RC2D_DVD_AUDIO_IEC_CH			(ADDR_RC2D_DECODER_INFO_BASE+0x0000)
#define ADDR_RC2D_DVD_AUDIO_PTS_SHAKE 		(ADDR_RC2D_DECODER_INFO_BASE+0x0001)
#define ADDR_RC2D_DVD_AUDIO_DRAM_STCH_ADDR  (ADDR_RC2D_DECODER_INFO_BASE+0x0002)
#define ADDR_RC2D_DVD_AUDIO_DRAM_STCL_ADDR	(ADDR_RC2D_DECODER_INFO_BASE+0x0003)
#define ADDR_RC2D_DVD_AUDIO_FS_96K_192K 	(ADDR_RC2D_DECODER_INFO_BASE+0x0004)
#define ADDR_RC2D_DVD_AUDIO_DOWN_2_IEC_OUT  (ADDR_RC2D_DECODER_INFO_BASE+0x0005)
#define ADDR_RC2D_DVD_AUDIO_IEC_DOWN_RATE   (ADDR_RC2D_DECODER_INFO_BASE+0x0006)
#define ADDR_RC2D_DVD_AUDIO_DEEMPHASIS      (ADDR_RC2D_DECODER_INFO_BASE+0x0007)
#define ADDR_RC2D_DVD_AUDIO_CCI_INFO        (ADDR_RC2D_DECODER_INFO_BASE+0x0008)
#define ADDR_RC2D_DVD_AUDIO_STEREO_PROHIBIT (ADDR_RC2D_DECODER_INFO_BASE+0x0009)
#define ADDR_RC2D_DVD_AUDIO_ATS_DM_COEFT    (ADDR_RC2D_DECODER_INFO_BASE+0x000A)

//#define ADDR_RC2D_MP3_NORMAL_PLAY           (ADDR_RC2D_DECODER_INFO_BASE+0x0000)
//#define ADDR_RC2D_MP3_CRC_FLAG              (ADDR_RC2D_DECODER_INFO_BASE+0x0001)
//#define ADDR_RC2D_MP3_SMOOTH_FLAG           (ADDR_RC2D_DECODER_INFO_BASE+0x0002)
//#define ADDR_RC2D_MP3_QUALITY_FLAG          (ADDR_RC2D_DECODER_INFO_BASE+0x0003)
//#define ADDR_RC2D_MP3_CUSTOMER_VERSION      (ADDR_RC2D_DECODER_INFO_BASE+0x0004)
//#define ADDR_RC2D_MP3_DEEMPHASIS_FLAG       (ADDR_RC2D_DECODER_INFO_BASE+0x0005)
// WMA
#define ADDR_RC2D_WMA_DRAM_SIZE_FLAG        (ADDR_RC2D_DECODER_INFO_BASE+0x0000)

//### MISC
#define ADDR_RC2D_IEC_FLAG                  (ADDR_RC2D_MISC_BASE + 0x0000)
#define IEC_DSP_MUTE                                           (0x1 << 15)
#define ADDR_RC2D_IEC_LATENCY               (ADDR_RC2D_MISC_BASE + 0x0001)
#define ADDR_RC2D_CUSTOMER_OPTION0          (ADDR_RC2D_MISC_BASE + 0x0002)       // UINT16 option
#define IEC_SRC_CH_NUM_DONT_CARE                               (0x1 <<  0)
#define CENTER_SPLIT2LR_WHEN_3_0                               (0x1 <<  1)
#define DVD_A_96K_DO_BASSMANAGE                                (0x1 <<  2)
#define MLP_PARTIAL_DOWNMIX                                    (0x1 <<  3)
#define AUDIO_UPDATE_EFFECT_ONOFF                              (0x1 <<  4)
#define ADDR_RC2D_CUSTOMER_OPTION1          (ADDR_RC2D_MISC_BASE + 0x0003)       // UINT16 option
#define LG_DEF                                                 (0x1 << 00)
#define SONY_DEF                                               (0x1 << 01)
#define PHILIPS_DEF                                            (0x1 << 02)
#define PIONEER_DEF                                            (0x1 << 03)
#define ADDR_RC2D_EQUALIZER_FLAG            (ADDR_RC2D_MISC_BASE + 0x0004)
#define ADDR_RC2D_REVERB_FLAG               (ADDR_RC2D_MISC_BASE + 0x0005)
//#define ADDR_RC2D_NEWPOST_FLAG              (ADDR_RC2D_MISC_BASE + 0x0005)
    #define NEWPOST_FLAG_VIR_BASS_BIT                          (0x1 << 00)
	#define NEWPOST_FLAG_M2S_BIT							   (0x1 << 01)
	#define NEWPOST_FLAG_SPH_BIT                               (0x1 << 02)//speaker height
    #define NEWPOST_FLAG_PEQ_BIT							   (0x1 << 03)
#define ADDR_RC2D_DOWNLOAD_TABLE_FLAG       (ADDR_RC2D_MISC_BASE + 0x0006)
#define ADDR_RC2D_VOLUME_SMOOTH_ORDER       (ADDR_RC2D_MISC_BASE + 0x0007)
#define ADDR_RC2D_SPECTRUM_HIGH_ORDER       (ADDR_RC2D_MISC_BASE + 0x0008)
#define ADDR_RC2D_SPECTRUM_LOW_ORDER        (ADDR_RC2D_MISC_BASE + 0x0009)
#define ADDR_RC2D_INPUT_SRC_GAIN            (ADDR_RC2D_MISC_BASE + 0x000A)
#define ADDR_RC2D_DDCO_AGC_FLAG                      (ADDR_RC2D_MISC_BASE + 0x000B)
#define ADDR_RC2D_AOUT_BANK_384_NUM         (ADDR_RC2D_MISC_BASE + 0x000C)
#define ADDR_RC2D_SPDIF_WORD_LENGTH         (ADDR_RC2D_MISC_BASE + 0x000D)
#define ADDR_RC2D_AUDIO_MUTE_THRESHOLD      (ADDR_RC2D_MISC_BASE + 0x000E)
#define ADDR_RC2D_DISC_TYPE                 (ADDR_RC2D_MISC_BASE + 0x000F)
#define ADDR_RC2D_CGMS_INFO                 (ADDR_RC2D_MISC_BASE + 0x0010)
#define ADDR_RC2D_DAC_BIT_RESOLUTION        (ADDR_RC2D_MISC_BASE + 0x0011)
#define ADDR_RC2D_DRC_MAX_AVE_ORDER         (ADDR_RC2D_MISC_BASE + 0x0012)
#define ADDR_RC2D_DRC_ATTACK_ORDER          (ADDR_RC2D_MISC_BASE + 0x0013)
#define ADDR_RC2D_DRC_VALUE_THRESHOLD       (ADDR_RC2D_MISC_BASE + 0x0014)
#define ADDR_RC2D_DRC_VALUE_MARGIN          (ADDR_RC2D_MISC_BASE + 0x0015)
#define ADDR_RC2D_DRC_RELEASE_GAIN          (ADDR_RC2D_MISC_BASE + 0x0016)
#define ADDR_RC2D_DDCO_FLAG                      (ADDR_RC2D_MISC_BASE + 0x0017)
#define ADDR_RC2D_DDCO_LFE_LPF_FLAG          (ADDR_RC2D_MISC_BASE + 0x0018)
#define ADDR_RC2D_AOUT_BANK_576_NUM         (ADDR_RC2D_MISC_BASE + 0x0019)
#define ADDR_RC2D_AOUT_BANK_320_NUM         (ADDR_RC2D_MISC_BASE + 0x001A)
#define ADDR_RC2D_AOUT_BANK_256_NUM         (ADDR_RC2D_MISC_BASE + 0x001B)
#define ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM  (ADDR_RC2D_MISC_BASE + 0x001C)
#define ADDR_RC2D_VOLUME_LEVEL_UP_ORDER     (ADDR_RC2D_MISC_BASE + 0x001D)
#define ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER   (ADDR_RC2D_MISC_BASE + 0x001E)
#define ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER    (ADDR_RC2D_MISC_BASE + 0x001F)

//### CTRL4
#define DSP_RC2D_CTRL4_AREA_SZ              10
#define ADDR_RC2D_CTRL_4_NUM_1              (ADDR_RC2D_CTRL4_BASE + 0x0000)
#define ADDR_RC2D_STC_DIFF_DEC2             (ADDR_RC2D_CTRL4_BASE + 0x0001)
#define ADDR_RC2D_SPEED_DEC2                (ADDR_RC2D_CTRL4_BASE + 0x0002)
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
#define ADDR_RC2D_PROCESSING_MODE_DEC2          (ADDR_RC2D_CTRL4_BASE + 0x0003)
// see the definition of first decoder
#define ADDR_RC2D_BIT_RESOLUTION_DEC2           (ADDR_RC2D_CTRL4_BASE + 0x0004)
//L/R Channel Mixing and Microphone Level Threshold
#define ADDR_RC2D_KARAOKE_FLAG_DEC2             (ADDR_RC2D_CTRL4_BASE + 0x0005)
  //bit 0: L/R Mixing
  //bit 1: Voice Mute
  //bit 2: Vocal Assist
#define ADDR_RC2D_LRCH_MIX_RATIO_DEC2           (ADDR_RC2D_CTRL4_BASE + 0x0006)
  //Value: 0x800000~0

#define ADDR_RC2D_MASTER_VOLUME_DEC2            (ADDR_RC2D_CTRL4_BASE + 0x000A)
#define ADDR_RC2D_SPDIF_FLAG_DEC2               (ADDR_RC2D_CTRL4_BASE + 0x000B)
#define ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC2     (ADDR_RC2D_CTRL4_BASE + 0x000C)
#define ADDR_RC2D_DECODING_STR_PNT_DEC2         (ADDR_RC2D_CTRL4_BASE + 0x000D)
#define ADDR_RC2D_BYPASS_ASRC_DEC2		      (ADDR_RC2D_CTRL4_BASE + 0x000E)

//### CTRL5
#define DSP_RC2D_CTRL5_AREA_SZ                  12
#define ADDR_RC2D_CTRL_5_NUM_1                  (ADDR_RC2D_CTRL5_BASE + 0x000)
#define ADDR_RC2D_DOWNSAMPLE_DEC2               (ADDR_RC2D_CTRL5_BASE + 0x001)   // 30
//Channel Use -> Pink Noise
#define ADDR_RC2D_CHANNEL_USE_DEC2              (ADDR_RC2D_CTRL5_BASE + 0x002)   // 31
#define RC2D_INT52                              (ADDR_RC2D_CTRL5_BASE + 0x003)
//Dolby Karaoke Mode
#define ADDR_RC2D_AC3_KARAOKE_MODE_DEC2                           (RC2D_INT52)
#define ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT_DEC2                    (RC2D_INT52)
#define ADDR_RC2D_CDDA_CFGDEEMPHASIS_DEC2                         (RC2D_INT52)
#define ADDR_RC2D_WMA_HEADER_SOURCE_DEC2                          (RC2D_INT52)
#define ADDR_RC2D_SACD_FILTER_LENGTH_DEC2                         (RC2D_INT52)
#define ADDR_RC2D_VORBIS_PACKET_NO_DEC2                           (RC2D_INT52)
#define ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC2               (RC2D_INT52)

#define RC2D_INT53                              (ADDR_RC2D_CTRL5_BASE + 0x004)
  //OFF: 0x1 <<8, V1,V2: 0x2 <<8, None: 0x4 <<8
  //V1: 0x8 <<8, V2: 0x10 <<8, V1+V2: 0x20 <<8
//Dolby Dual Mode
#define ADDR_RC2D_AC3_DUAL_MODE_DEC2                              (RC2D_INT53)
#define ADDR_RC2D_LPCM_FREQUENCY_DEC2                             (RC2D_INT53)
#define ADDR_RC2D_HDCD_CONFIG_DEC2                                (RC2D_INT53)
#define ADDR_RC2D_SACD_DOWN_SAMPLE_DEC2                           (RC2D_INT53)
#define ADDR_RC2D_WMA_PACKET_COUNT_DEC2                           (RC2D_INT53)
#define ADDR_RC2D_MPEG_SYNC_MODE_DEC2                             (RC2D_INT53)
#define ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC2                    (RC2D_INT53) // Light added
#define RC2D_INT54                              (ADDR_RC2D_CTRL5_BASE + 0x005)
  //Stereo: 0x0, Ch0 Only: 0x1 <<8
  //Ch1 Only: 0x2 <<8, Ch0+Ch1 Mixing: 0x3 <<8
//Dolby Compression
#define ADDR_RC2D_AC3_COMPRESS_MODE_DEC2                          (RC2D_INT54)
#define ADDR_RC2D_LPCM_BIT_SHIFT_DEC2                             (RC2D_INT54)
#define ADDR_RC2D_HDCD_CFGUPSAMPLING_DEC2                         (RC2D_INT54)
#define ADDR_RC2D_SACD_CH_NUM_DEC2                                (RC2D_INT54)
#define ADDR_RC2D_WMA_PACKET_SIZE_DEC2                            (RC2D_INT54)
#define ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC2                      (RC2D_INT54) // Light added
#define RC2D_INT55                              (ADDR_RC2D_CTRL5_BASE + 0x006)
  //Custom Mode 0: 0x0, Custom Mode 1: 0x1 <<8
  //Line Mode: 0x2 <<8, RF Mode: 0x3 <<8
#define ADDR_RC2D_AC3_DYNAMIC_LOW_DEC2                            (RC2D_INT55)
#define ADDR_RC2D_DRC_VALUE_DEC2                                  (RC2D_INT55)
#define ADDR_RC2D_LPCM_DRC_VALUE_DEC2                             (RC2D_INT55)
#define ADDR_RC2D_HDCD_CFGDITHMODE_DEC2                           (RC2D_INT55)
#define ADDR_RC2D_SACD_DOWN_SAMPLE_ORDER_DEC2	                  (RC2D_INT55)
#define ADDR_RC2D_WMA_VERSION_DEC2                                (RC2D_INT55)
#define ADDR_RC2D_PCM_INPUT_TYPE_DEC2                             (RC2D_INT55) 
#define RC2D_INT56                              (ADDR_RC2D_CTRL5_BASE + 0x007)
#define ADDR_RC2D_AC3_DYNAMIC_HIGH_DEC2                           (RC2D_INT56)
#define ADDR_RC2D_LPCM_Q_DEC2                                     (RC2D_INT56)
#define ADDR_RC2D_SACD_OUTPUT_MODE_DEC2                           (RC2D_INT56)
#define ADDR_RC2D_WMA_NUM_CHANNELS_DEC2                           (RC2D_INT56)
#define ADDR_RC2D_ADPCM_BLOCK_ALIGN_DEC2                          (RC2D_INT56)
#define ADDR_RC2D_PCM_PREBUF_DELAY_BANK_DEC2                      (RC2D_INT56)
#define RC2D_INT57                              (ADDR_RC2D_CTRL5_BASE + 0x008)
//DTS Dynamic Range Control Flag
#define ADDR_RC2D_DTS_DRC_FLAG_DEC2                               (RC2D_INT57)
#define ADDR_RC2D_MPEG_DRC_FLAG_DEC2                              (RC2D_INT57)
#define ADDR_RC2D_LPCM_DRC_FLAG_DEC2                              (RC2D_INT57)
#define ADDR_RC2D_AC3_DOWNMIX_DEC2                                (RC2D_INT57)
#define ADDR_RC2D_SACD_INPUT_FRAME_DEC2                           (RC2D_INT57)
#define ADDR_RC2D_HDCD_USRDATA_RPT_DEC2                           (RC2D_INT57)
#define ADDR_RC2D_WMA_SAMPLE_PER_SECOND_DEC2                      (RC2D_INT57)
#define ADDR_RC2D_ADPCM_CHANNEL_NUM_DEC2                          (RC2D_INT57)
#define RC2D_INT58                              (ADDR_RC2D_CTRL5_BASE + 0x009)
  //OFF: 0x0, AUTO: 0x1 <<8
//MPEG CRC Value
#define ADDR_RC2D_MPEG_CRC_FLAG_DEC2	                          (RC2D_INT58)
#define ADDR_RC2D_LPCM_DECODING_TYPE_DEC2                         (RC2D_INT58)
#define ADDR_RC2D_WMA_BYTES_PER_SECOND_DEC2                       (RC2D_INT58)
#define ADDR_RC2D_SACD_INPUT_DSD_MODE_DEC2                        (RC2D_INT58)
#define SACD_INPUT_DSD_DEC2                                              (0x1)
#define SACD_INPUT_DST_DEC2                                              (0x0)
#define RC2D_INT59                              (ADDR_RC2D_CTRL5_BASE + 0x00A)
#define ADDR_RC2D_WMA_BLOCK_SIZE_DEC2                             (RC2D_INT59)
#define ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE_DEC2                 (RC2D_INT59)
#define RC2D_INT5A                              (ADDR_RC2D_CTRL5_BASE + 0x00B)
#define ADDR_RC2D_WMA_ENCODER_OPTIONS_DEC2                        (RC2D_INT5A)
#define ADDR_RC2D_PCM_LRCK_MODE_DEC2                              (RC2D_INT5A)

//koro: 20080919 cannot use INT5B in 539x and 536x
//#define RC2D_INT5B                              (ADDR_RC2D_CTRL5_BASE + 0x00C)
//#define ADDR_RC2D_WMA_ENCODER_OPTIONS_DEC2                        (RC2D_INT5B)
//#define ADDR_RC2D_LPCM_ADPCM_DEC_TYPE_DEC2                        (RC2D_INT5B)

#ifdef ADSP_JPEG_DEC2
/// mapping with ADSP Jpeg_var.inc
#define ADDR_RC2D_JPG_CMD_FLAG_DEC2                     (ADDR_RC2D_CTRL5_BASE + 0x001)
#define ADDR_RC2D_JPG_MAX_SCAN_NUM_DEC2                 (ADDR_RC2D_CTRL5_BASE + 0x002)
#define ADDR_RC2D_JPG_MAX_COMPACT_ADDR_DEC2             (ADDR_RC2D_CTRL5_BASE + 0x003)
#define ADDR_RC2D_JPG_LENGTH                            (ADDR_RC2D_CTRL5_BASE + 0x004)  ///8194h, Parsing file length
#define ADDR_RC2D_JPG_SUPPORT_MAX_WIDTH                 (ADDR_RC2D_CTRL5_BASE + 0x005)  ///8195h, Current DRAM can support PIC width.
///[20100201]
#define ADDR_RC2D_JPG_FIFO_FULL                         (ADDR_RC2D_CTRL5_BASE + 0x006)  ///8196h
#endif

//### Decoder2 Info
#define ADDR_RC2D_DECODER2_INFO	     (ADDR_RC2D_DECODER_INFO_BASE_DEC2 + 0x0000)
#ifdef DSP_SUPPORT_NPTV
    // Demodulation Parameters
#define ADDR_DEM_SOURCE_MODE_DEC2                (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0000)
#define ADDR_DEM_USER_MODE_DEC2                  (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0001)
#define ADDR_DEM_ADC_GAIN_DEC2                   (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0002)
#define ADDR_DEM_ENFORCED_MODE_DEC2               (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0003)
    // Detection parameters for Top Level Detection
#define ADDR_DETECTOR_DEFAULT_MODE_DEC2           (ADDR_RC2D_DECODER_INFO_BASE_DEC2) //0 -> PAL, 1->SECAM L; 8150h
#define ADDR_DETECTOR_START_FLAG_DEC2             (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0001) //0->OFF, 1->ON; 8151h

#define ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC2       (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0004)   
#define ADDR_DEM_SAP_FINE_TUNE_VOLUME_DEC2       (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0004)  
#define ADDR_DEM_NICAM_FINE_TUNE_VOLUME_DEC2      (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0005)
#define ADDR_DEM_FMFM_DUAL_FINE_TUNE_VOLUME_DEC2  (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0005) 
#define ADDR_DEM_AM_FINE_TUNE_VOLUME_DEC2        (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0006)
#define ADDR_DEM_MTS_FINE_TUNE_VOLUME_DEC2       (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0006)  

#define ADDR_SECAM_L_MUTE_MODE_DEC2               (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0007)  
#define ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT_DEC2    (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0008)  
#define ADDR_SECAM_L_MUTE_THRESHOLD_LOW_DEC2       (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x0009)  
    //FMFM DETECTION
#define ADDR_FMFM_NUM_CHECK_DEC2                  (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x000A)
#define ADDR_FMFM_NUM_DOUBLE_CHECK_DEC2           (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x000B)
#define ADDR_FMFM_MONO_WEIGHT_DEC2                (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x000C)
#define ADDR_FMFM_STEREO_WEIGHT_DEC2              (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x000D)
#define ADDR_FMFM_DUAL_WEIGHT_DEC2                (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x000E)
#define ADDR_FMFM_DETECTION_CONFID_DEC2          (ADDR_RC2D_DECODER_INFO_BASE_DEC2+0x000F) 
#endif // DSP_SUPPORT_NPTV

//### MISC dec2
#define ADDR_RC2D_IEC_FLAG_DEC2             (ADDR_RC2D_MISC_BASE_DEC2 + 0x0000)
  //#define IEC_DSP_MUTE                                            (0x1 << 15)
#define ADDR_RC2D_IEC_LATENCY_DEC2          (ADDR_RC2D_MISC_BASE_DEC2 + 0x0001)
#define ADDR_RC2D_CUSTOMER_OPTION0_DEC2     (ADDR_RC2D_MISC_BASE_DEC2 + 0x0002)   // UINT16 option
  //#define IEC_SRC_CH_NUM_DONT_CARE                                (0x1 << 00)
  //#define CENTER_SPLIT2LR_WHEN_3_0                                (0x1 << 01)
  //#define DVD_A_96K_DO_BASSMANAGE                                 (0x1 << 02)
  //#define MLP_PARTIAL_DOWNMIX                                     (0x1 << 03)
#define ADDR_RC2D_CUSTOMER_OPTION1_DEC2     (ADDR_RC2D_MISC_BASE_DEC2 + 0x0003)   // UINT16 option
  //#define LG_DEF                                                  (0x1 << 00)
  //#define SONY_DEF                                                (0x1 << 01)
  //#define PHILIPS_DEF                                             (0x1 << 02)
  //#define PIONEER_DEF                                             (0x1 << 03)
#define ADDR_RC2D_EQUALIZER_FLAG_DEC2       (ADDR_RC2D_MISC_BASE_DEC2 + 0x0004)
#define ADDR_RC2D_REVERB_FLAG_DEC2          (ADDR_RC2D_MISC_BASE_DEC2 + 0x0005)
#define ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC2  (ADDR_RC2D_MISC_BASE_DEC2 + 0x0006)
#define ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC2  (ADDR_RC2D_MISC_BASE_DEC2 + 0x0007)
#define ADDR_RC2D_SPECTRUM_HIGH_ORDER_DEC2  (ADDR_RC2D_MISC_BASE_DEC2 + 0x0008)
#define ADDR_RC2D_SPECTRUM_LOW_ORDER_DEC2   (ADDR_RC2D_MISC_BASE_DEC2 + 0x0009)
#define ADDR_RC2D_INPUT_SRC_GAIN_DEC2       (ADDR_RC2D_MISC_BASE_DEC2 + 0x000A)
#define ADDR_RC2D_AOUT_BANK_384_NUM_DEC2    (ADDR_RC2D_MISC_BASE_DEC2 + 0x000C)
#define ADDR_RC2D_SPDIF_WORD_LENGTH_DEC2    (ADDR_RC2D_MISC_BASE_DEC2 + 0x000D)
#define ADDR_RC2D_AUDIO_MUTE_THRESHOLD_DEC2 (ADDR_RC2D_MISC_BASE_DEC2 + 0x000E)
#define ADDR_RC2D_DISC_TYPE_DEC2            (ADDR_RC2D_MISC_BASE_DEC2 + 0x000F)
#define ADDR_RC2D_CGMS_INFO_DEC2            (ADDR_RC2D_MISC_BASE_DEC2 + 0x0010)
#define ADDR_RC2D_DAC_BIT_RESOLUTION_DEC2   (ADDR_RC2D_MISC_BASE_DEC2 + 0x0011)
#define ADDR_RC2D_DRC_MAX_AVE_ORDER_DEC2    (ADDR_RC2D_MISC_BASE_DEC2 + 0x0012)
#define ADDR_RC2D_DRC_ATTACK_ORDER_DEC2     (ADDR_RC2D_MISC_BASE_DEC2 + 0x0013)
#define ADDR_RC2D_DRC_VALUE_THRESHOLD_DEC2  (ADDR_RC2D_MISC_BASE_DEC2 + 0x0014)
#define ADDR_RC2D_DRC_VALUE_MARGIN_DEC2     (ADDR_RC2D_MISC_BASE_DEC2 + 0x0015)
#define ADDR_RC2D_DRC_RELEASE_GAIN_DEC2     (ADDR_RC2D_MISC_BASE_DEC2 + 0x0016)
#define ADDR_RC2D_AOUT_BANK_576_NUM_DEC2    (ADDR_RC2D_MISC_BASE_DEC2 + 0x0019)
#define ADDR_RC2D_AOUT_BANK_320_NUM_DEC2    (ADDR_RC2D_MISC_BASE_DEC2 + 0x001A)
#define ADDR_RC2D_AOUT_BANK_256_NUM_DEC2    (ADDR_RC2D_MISC_BASE_DEC2 + 0x001B)
#define ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM_DEC2 (ADDR_RC2D_MISC_BASE_DEC2 + 0x001C)
#define ADDR_RC2D_VOLUME_LEVEL_UP_ORDER_DEC2    (ADDR_RC2D_MISC_BASE_DEC2 + 0x001D)
#define ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER_DEC2  (ADDR_RC2D_MISC_BASE_DEC2 + 0x001E)
#define ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC2   (ADDR_RC2D_MISC_BASE_DEC2 + 0x001F)

//=============================================================
// DRAM_PTS_STC Section (082a0h~082b0h) -- Foreground Decoder
//=============================================================
//PTS STC related
#define ADDR_RC2D_MEMORY_BUFFER_ADDRESS     (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x00)
#define ADDR_RC2D_1ST_PTS_PRS_PNT           (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x01)
// 1st pts info to do init AV-sync
#define ADDR_RC2D_1ST_PTS_STCH              (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x02)
#define ADDR_RC2D_1ST_PTS_STCL              (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x03)
#define ADDR_RC2D_STC_DIFF_LOBOUND          (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x04)
// trigger sync measure process if over this threshold
//#define STC_DIFF_BOUND_032MS                0x0B50  // 32ms, STC=2880
#define STC_DIFF_BOUND_032MS                0x11ad   // 50ms, STC=4525
#define ADDR_RC2D_STC_DIFF_HIBOUND          (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x05)
// do sync process if over this threshold
#define STC_DIFF_BOUND_064MS                0x1690  // 64ms, STC=5760
#define ADDR_RC2D_STC_DIFF_WSBOUND          (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x06)
// ignore the sync process if over this threshold --> PTS/STC are incorrect
#define STC_DIFF_BOUND_500MS                (0xAFD0*3)   //extend to 1000ms // 500ms, STC=45000
#define ADDR_RC2D_STC_LEVEL_THRESHOLD       (ADDR_RC2D_DRAM_PTS_STC_BASE + 0x07)
// level threshold of which frame to do repeat/drop
#define STC_THR_36DB                        0x20000  // -36dB
#define STC_THR_42DB                        0x10000  // -42dB

//======================================================================
// DRAM_3D_FUNCTION_FLAG Section (08270h~08280h) -- Foreground Decoder
//======================================================================
// PL2
#define DRAM_PROLOGIC_II                        (ADDR_RC2D_DRAM_UPMIX_BASE)
#define ADDR_RC2D_3D_BUFFER_ADDR                  (DRAM_PROLOGIC_II + 0x00)
#define ADDR_PL2_TABLE                            (DRAM_PROLOGIC_II + 0x20)
// Neo6
#define DRAM_NEO6                               (ADDR_RC2D_DRAM_UPMIX_BASE)
#define ADDR_RC2D_NEO6_TABLE_ADDR                        (DRAM_NEO6 + 0x00)
#define ADDR_RC2D_NEO6_DRAM_CMPT_BUFFER_ADDR             (DRAM_NEO6 + 0x01)
#define ADDR_RC2D_NEO6_CGAIN                             (DRAM_NEO6 + 0x02)
#define ADDR_RC2D_NEO6_MODE                              (DRAM_NEO6 + 0x03)
#define ADDR_RC2D_NEO6_LIN                               (DRAM_NEO6 + 0x04)
#define ADDR_RC2D_NEO6_RIN                               (DRAM_NEO6 + 0x05)
#define ADDR_RC2D_NEO6_BASE_ADDR                         (DRAM_NEO6 + 0x06)
// QSound
#define DRAM_QMSS                               (ADDR_RC2D_DRAM_UPMIX_BASE)

//======================================================================
// DRAM_AUDIO_BASIC_SETTING Section (083e0h~08400h) -- Foreground Decoder
//======================================================================

// dsp polling dram for mpeg4 to delay G.S Lin's hw
// 0 -> not enable
// else -> how many dsp cycles will dsp request 32 dwrd for dram
// p.s. there will two request from DSP. 
//      1. Instruction cache miss ( T.B.D )
//      2. Data fetch ( 32 dwrds)
#define ADDR_RC2D_POLL_DRAM_TIMER_CNT          (ADDR_RC2D_AUDIO_BASIC_BASE+0x10)
// How many dwrds will dsp read in timer interrupt
#define ADDR_RC2D_POLL_DRAM_READ_UINT32_CNT_1  (ADDR_RC2D_AUDIO_BASIC_BASE+0x11)
//Virtual Surround
  //OFF: 0x0, ON: 0x1 <<8

//============================================================
// DRAM_VS_BUF Section (08400h~08500h) -- Foreground Decoder
//============================================================
#define DRAM_VIRTUAL_SURROUND                   (ADDR_RC2D_DRAM_VS_BUF_BASE)
  // MTK VSurr
#define ADDR_RC2D_VIRTUAL_SURROUND_GAIN         (DRAM_VIRTUAL_SURROUND + 0)
#define ADDR_RC2D_VIRTUAL_SURROUND_WIDEN_GAIN   (ADDR_RC2D_VIRTUAL_SURROUND_GAIN + 1)
#define ADDR_RC2D_VIRTUAL_SURROUND_DELAY_NUMBER (ADDR_RC2D_VIRTUAL_SURROUND_WIDEN_GAIN + 1)
#define ADDR_RC2D_VIRTUAL_SURROUND_FILTER_COEFF (ADDR_RC2D_VIRTUAL_SURROUND_DELAY_NUMBER + 1)
  // Sony VSS
#define ADDR_RC2D_VSS_BACKUP_ADDRESS            (DRAM_VIRTUAL_SURROUND + 0)
#define ADDR_RC2D_VSS_MODE                      (DRAM_VIRTUAL_SURROUND + 1)
  // SONY TVS
#define ADDR_RC2D_TVS_BACKUP_ADDRESS            (DRAM_VIRTUAL_SURROUND + 0)
#define ADDR_RC2D_TVS_MODE                      (DRAM_VIRTUAL_SURROUND + 1)
  // QSound
#define DRAM_QSOUND                             (ADDR_RC2D_DRAM_VS_BUF_BASE)
#define ADDR_RC2D_QSOUND_MODE                              (DRAM_QSOUND + 0)
#define ADDR_RC2D_QSOUND_DRAM_CMPT_BUFFER_ADDR             (DRAM_QSOUND + 1)
#define ADDR_RC2D_QSOUND_TABLE                             (DRAM_QSOUND + 2)
  // Related configuration of QSound Table
#define ADDR_RC2D_QSOUND_QFSPREADF              (ADDR_RC2D_QSOUND_TABLE + 37)
#define ADDR_RC2D_QSOUND_QFSPREADS              (ADDR_RC2D_QSOUND_TABLE + 38)
#define ADDR_RC2D_QSOUND_QFCDROPF               (ADDR_RC2D_QSOUND_TABLE + 39)
#define ADDR_RC2D_QSOUND_QFCDROPS               (ADDR_RC2D_QSOUND_TABLE + 40)
#define ADDR_RC2D_QSOUND_QFFVOLUME              (ADDR_RC2D_QSOUND_TABLE + 41)
#define ADDR_RC2D_QSOUND_QFSVOLUME              (ADDR_RC2D_QSOUND_TABLE + 42)
#define ADDR_RC2D_QSOUND_QFCVOLUME              (ADDR_RC2D_QSOUND_TABLE + 43)
#define ADDR_RC2D_QSOUND_QSIZZLET1              (ADDR_RC2D_QSOUND_TABLE + 44)
#define ADDR_RC2D_QSOUND_QSIZZLEDELTA           (ADDR_RC2D_QSOUND_TABLE + 45)
#define ADDR_RC2D_QSOUND_QSIZZLET2              (ADDR_RC2D_QSOUND_TABLE + 46)
#define ADDR_RC2D_QSOUND_QSLEVELF               (ADDR_RC2D_QSOUND_TABLE + 47)
#define ADDR_RC2D_QSOUND_QSLEVELS               (ADDR_RC2D_QSOUND_TABLE + 48)
#define ADDR_RC2D_QSOUND_QRUMBLET1              (ADDR_RC2D_QSOUND_TABLE + 49)
#define ADDR_RC2D_QSOUND_QRUMBLEDELTA           (ADDR_RC2D_QSOUND_TABLE + 50)
#define ADDR_RC2D_QSOUND_QRUMBLET2              (ADDR_RC2D_QSOUND_TABLE + 51)
#define ADDR_RC2D_QSOUND_QRLEVELF               (ADDR_RC2D_QSOUND_TABLE + 52)
#define ADDR_RC2D_QSOUND_QRLEVELS               (ADDR_RC2D_QSOUND_TABLE + 53)
#define ADDR_RC2D_QSOUND_QCONSTONE              (ADDR_RC2D_QSOUND_TABLE + 54)
  // Dolby Headphone
#define ADDR_RC2D_DOLBY_HP_WORKING_ADDRESS      (DRAM_VIRTUAL_SURROUND + 0)
#define ADDR_RC2D_DOLBY_HP_MODE                 (DRAM_VIRTUAL_SURROUND + 1)
  // SRS WOW
#define ADDR_SRS_WOW_BAKUP                      (DRAM_VIRTUAL_SURROUND)          // 0 word, a mark only
#define ADDR_SRS_WOW_DATA                       (ADDR_SRS_WOW_BAKUP     +  0)    //28 Dwords
#define ADDR_SRS_WOW_RODATA                     (ADDR_SRS_WOW_DATA      + 28)    //37 Dwords
#define ADDR_SRS_WOW_BSS                        (ADDR_SRS_WOW_RODATA    + 37)    // 7 Dwords
#define ADDR_SRS_WOW_BANK2_BSS                  (ADDR_SRS_WOW_BSS       +  7)    //23 Dwords, filter states
#define ADDR_SRS_WOW_DRAM_USE_END               (ADDR_SRS_WOW_BANK2_BSS + 23)    // 0 word, a mark only
  // SRS TSXT
#define ADDR_SRS_TSXT_BAKUP                     (DRAM_VIRTUAL_SURROUND)          // 0 word, a mark only
#define ADDR_SRS_TSXT_DATA                      (ADDR_SRS_TSXT_BAKUP     +  0)
#define ADDR_SRS_TSXT_RODATA                    (ADDR_SRS_TSXT_DATA      + 23)
#define ADDR_SRS_TSXT_BSS                       (ADDR_SRS_TSXT_RODATA    + 58)
#define ADDR_SRS_TSXT_BANK2_BSS                 (ADDR_SRS_TSXT_BSS       + 10)
#define ADDR_SRS_TSXT_DRAM_USE_END              (ADDR_SRS_TSXT_BANK2_BSS + 34)   // 0 word, a mark only
//sunman_tshd
#define ADDR_SRS_TSHD_BAKUP                   (DRAM_VIRTUAL_SURROUND)          			// 0 word, a mark only
#define ADDR_SRS_TSHD_DATA                    (ADDR_SRS_TSHD_BAKUP			+   0)	//50 word
#define ADDR_SRS_TSHD_BANK2_CONST	(ADDR_SRS_TSHD_DATA  			+  50)	//constants : (13+2) words
#define ADDR_SRS_TSHD_M2S_COEF           	(ADDR_SRS_TSHD_BANK2_CONST	+  15)	//30 words
#define ADDR_SRS_TSHD_PERSP_FR_COEF	(ADDR_SRS_TSHD_M2S_COEF		+  30)	//15 words
#define ADDR_SRS_TSHD_FOCUS_COEF        (ADDR_SRS_TSHD_PERSP_FR_COEF	+  15)	//5 words
#define ADDR_SRS_TSHD_PERSP_RR_COEF	(ADDR_SRS_TSHD_FOCUS_COEF 	+   5)	//15 words
#define ADDR_SRS_TSHD_TB_COEF			(ADDR_SRS_TSHD_PERSP_RR_COEF	+  15)	//15 words
#define ADDR_SRS_TSHD_DEF_COEF		(ADDR_SRS_TSHD_TB_COEF		+  15)	//5 words
#define ADDR_SRS_TSHD_SUB_TB_COEF      (ADDR_SRS_TSHD_DEF_COEF		+   5)	//15 words
#define ADDR_SRS_TSHD_CROSS_COEF		(ADDR_SRS_TSHD_SUB_TB_COEF	+  15)	//10 words, LP, HP
#define ADDR_SRS_TSHD_SRS3D_COEF		(ADDR_SRS_TSHD_CROSS_COEF	+  10)	//10 words, init, final
// New MTK Vsurr
#ifdef CC_AUD_OLD_MTK_VSURR
#define ADDR_VSURR_CROS_PARA1                   (DRAM_VIRTUAL_SURROUND)  
#define ADDR_VSURR_CROS_PARA2                   (ADDR_VSURR_CROS_PARA1+1)
#define ADDR_VSURR_CROS_PARA3                   (ADDR_VSURR_CROS_PARA2+1)
#define ADDR_VSURR_GAIN_MPY                     (ADDR_VSURR_CROS_PARA3+1)
#define ADDR_VSURR_GAIN_SL                      (ADDR_VSURR_GAIN_MPY+1)
#define ADDR_VSURR_OUT_OF_PHASE                 (ADDR_VSURR_GAIN_SL+1)
#define ADDR_VSURR_DELAY_ON                     (ADDR_VSURR_OUT_OF_PHASE+1)
#define ADDR_VSURR_DELAY_SAMPLE_NUM_L           (ADDR_VSURR_DELAY_ON+1)
#define ADDR_VSURR_DELAY_SAMPLE_NUM_R           (ADDR_VSURR_DELAY_SAMPLE_NUM_L+1)

#define ADDR_VSURR_CROS_COEF                    (ADDR_VSURR_DELAY_SAMPLE_NUM_R+1) // 10
#define ADDR_VSURR_IIR_COEF                     (DRAM_VIRTUAL_SURROUND+37) // 20
#else
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
#define ADDR_VSURR_IIR_COEF                     (DRAM_VIRTUAL_SURROUND+37)
#define ADDR_VSURR_BASS_COEF                     (ADDR_VSURR_IIR_COEF+20)
#define ADDR_VSURR_CROS_PARA4                   (ADDR_VSURR_BASS_COEF+5)
#define ADDR_VSURR_CROS_PARA5                   (ADDR_VSURR_CROS_PARA4+1)

#define ADDR_VSURR_1STOP_COEF                    (DRAM_VIRTUAL_SURROUND+0x96) 
#define ADDR_VSURR_CROS_PARA6                   (ADDR_VSURR_1STOP_COEF+5)
#define ADDR_VSURR_2STOP_COEF                    (ADDR_VSURR_CROS_PARA6+1)
#define ADDR_VSURR_FILTER_HISTORY_2Rstop (ADDR_VSURR_2STOP_COEF+5)
#define ADDR_VSURR_FILTER_HISTORY_2Lstop (ADDR_VSURR_FILTER_HISTORY_2Rstop+4)
#define ADDR_VSURR_CROS_PARA7                   (ADDR_VSURR_FILTER_HISTORY_2Lstop+4) 
#endif
//===============================================================
// DRAM_EQUALIZER Section (082c0h~083e0h) -- Foreground Decoder
//===============================================================
  //OFF: 0x0, ON: 0x1 <<8
#define ADDR_RC2D_EQUALIZER_BAND_NUM_1          (ADDR_RC2D_DRAM_EQUALIZER_BASE)    	  //82C0h, 1 word (DRAM_EQUALIZER)
#define ADDR_RC2D_EQUALIZER_TABLE_CHANGE        (ADDR_RC2D_EQUALIZER_BAND_NUM_1 + 1)  //       1 word
#define ADDR_RC2D_EQUALIZER_TABLE               (ADDR_RC2D_EQUALIZER_TABLE_CHANGE + 1)//      40 words
#define ADDR_RC2D_EQUALIZER_HISTORY             (ADDR_RC2D_EQUALIZER_TABLE + 40)	  //     160 words for 8.0 channel
#define ADDR_RC2D_EQUALIZER_DRY_LEVEL           (ADDR_RC2D_EQUALIZER_HISTORY + 160)	  //       1 word
#define ADDR_RC2D_EQUALIZER_TABLE_NEW           (ADDR_RC2D_EQUALIZER_DRY_LEVEL + 1)	  //      40 words
#define ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT     (ADDR_RC2D_EQUALIZER_TABLE_NEW+40)   // 1 word
#define ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT     (ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT+1)   // 1 word

//===========================================================
// SUPER_BASS Section (08500h~085e0h) -- Foreground Decoder
//===========================================================
//Super Bass
#ifdef DSP_EQ2_SBASS_SUPPORT
#define ADDR_RC2D_SUPER_BASS_TABLE_CHANGE       (ADDR_RC2D_SUPER_BASS_BASE)                  // 8500h, 1 word
#define ADDR_RC2D_SUPER_BASS_TABLE              (ADDR_RC2D_SUPER_BASS_TABLE_CHANGE+1)                // 8501h, 10 words
#define ADDR_RC2D_SUPER_BASS_HISTORY            (ADDR_RC2D_SUPER_BASS_TABLE+10)                      // 850Bh, 64 words for 8.0 channel
#define ADDR_RC2D_SUPER_BASS_HISTORY_CH0     (ADDR_RC2D_SUPER_BASS_HISTORY)                       // 850Bh
#define ADDR_RC2D_SUPER_BASS_HISTORY_CH1        (ADDR_RC2D_SUPER_BASS_HISTORY+8)                // 8513h
#define ADDR_RC2D_SUPER_BASS_TABLE_NEW          (ADDR_RC2D_SUPER_BASS_HISTORY+64)                   // 854Bh, 10 words
#else
#define ADDR_RC2D_SUPER_BASS_DELAY_NUMBER       (ADDR_RC2D_SUPER_BASS_BASE)
#define ADDR_RC2D_SUPER_BASS_BOOST_GAIN         (ADDR_RC2D_SUPER_BASS_BASE + 1)
#define ADDR_RC2D_SUPER_BASS_CLEAR_GAIN         (ADDR_RC2D_SUPER_BASS_BASE + 2)
#define ADDR_RC2D_SUPER_BASS_BOOST_GAIN_ORG     (ADDR_RC2D_SUPER_BASS_BASE + 3)
#define ADDR_RC2D_SUPER_BASS_CLEAR_GAIN_ORG     (ADDR_RC2D_SUPER_BASS_BASE + 4)
#define ADDR_RC2D_SUPER_BASS_PIPE_FILTER_COEFF  (ADDR_RC2D_SUPER_BASS_BASE + 5)
#define ADDR_RC2D_SUPER_BASS_BOOST_FILTER_COEFF (ADDR_RC2D_SUPER_BASS_BASE + 0x0A)
#define ADDR_RC2D_SUPER_BASS_CLEAR_FILTER_COEFF (ADDR_RC2D_SUPER_BASS_BASE + 0x0F)
#endif
//============================================================
// DRAM_REVERB Section (085e0h~08600h) -- Foreground Decoder
//============================================================
#define ADDR_RC2D_REVERB_BANK_ADDR              (ADDR_RC2D_DRAM_REVERB_BASE)
#define ADDR_RC2D_REVERB_BANK_MAX               (ADDR_RC2D_DRAM_REVERB_BASE + 0x01)
#define ADDR_RC2D_REVERB_BANK_NL_PR             (ADDR_RC2D_DRAM_REVERB_BASE + 0x02)
#define ADDR_RC2D_REVERB_BANK_PL_NR             (ADDR_RC2D_DRAM_REVERB_BASE + 0x03)
#define ADDR_RC2D_REVERB_BANK_PL_PR             (ADDR_RC2D_DRAM_REVERB_BASE + 0x04)
#define ADDR_RC2D_REVERB_IIR_A                  (ADDR_RC2D_DRAM_REVERB_BASE + 0x05)
#define ADDR_RC2D_REVERB_IIR_B                  (ADDR_RC2D_DRAM_REVERB_BASE + 0x06)
#define ADDR_RC2D_REVERB_GAIN                   (ADDR_RC2D_DRAM_REVERB_BASE + 0x07)

//======================================================================
// DRAM_BASS_MANAGMENT Section (08220h~08270h) -- Foreground Decoder
//======================================================================
//Speaker Configuration
#define ADDR_RC2D_SPEAKER_CONFIG                (ADDR_RC2D_DRAM_BASS_MANAGEMENT_BASE)
#define ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF  (ADDR_RC2D_SPEAKER_CONFIG + 1)
//Channel Delay
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
  //Value: 0~200 <<8 / 5cm
#define ADDR_RC2D_CH_DELAY_FACTOR               (ADDR_RC2D_CH_DELAY_CH10 +1)

//======================================================================
// DRAM_3D_FUNCTION_FLAG Section (08270h~08280h) -- Foreground Decoder
//======================================================================
// 3D processing
#define ADDR_RC2D_VIRTUAL_SURROUND              (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE + 0)
#define VSS_NONE                                               (0x00     )
#define VSS_MTK                                                (0x01 << 0)
#define VSS_N22                                                (0x01 << 1)
#define VSS_SRS_WOW                                            (0x01 << 2)
#define VSS_SONY                                               (0x01 << 3)
#define VSS_TVS                                                (0x01 << 4)
#define VSS_SRS_TSXT                                           (0x01 << 5)
#define VSS_QSOUND                                             (0x01 << 6)
#define VSS_DOLBY_HP                                           (0x01 << 7)
#define VSS_M2S                                           	   (0x01 << 8)

#define ADDR_RC2D_PRO_LOGIC_II_CONFIG           (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE + 1)
#define ADDR_RC2D_PRO_LOGIC_II_MODE             (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE + 2)
#define ADDR_RC2D_NEWPOST_FLAG                  (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE + 3)  
#define ADDR_RC2D_AD_FLAG                       (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE + 4)    //For Audio Description

//==============================================================
// DRAM_MIXSOUND Section (08700h~08720h) -- Foreground Decoder
//==============================================================
// MixSound
#define ADDR_RC2D_DRAM_MIXSOUND_STATUS			(ADDR_RC2D_DRAM_MIXSOUND_BASE + 0)
// 1-->working
// 0-->idle
#define ADDR_RC2D_DRAM_MIXSOUND_CONFIG          (ADDR_RC2D_DRAM_MIXSOUND_BASE + 1)
//----------------------------------------------------------------------
//Configuration
//  bit	 0: 0-> no need upsampling, 1-> 2x upsampling
//  bit  1: 2-> 4x upsampling, 3-> 8x upsampling
//  bit  2: ADPCM encoding
//  bit  3: ADPCM filter mode : 0->no filter, 1->IIR applied
//  bit  4: source mode : 0->mono, 1->stereo
//  bit  5: 
//
//  bit  8: mixing data to left & right output
//  bit  9: mixing data to surround output
//  bit 10: mixing data to center output
//  bit 11: mixing data to channel 7 & 8
//  bit 12: mixing data to channel 9 & 10
//----------------------------------------------------------------------
#define ADDR_RC2D_DRAM_MIXSOUND_PCM_ADDR                   (ADDR_RC2D_DRAM_MIXSOUND_BASE + 2)
#define ADDR_RC2D_DRAM_MIXSOUND_PCM_LEN                    (ADDR_RC2D_DRAM_MIXSOUND_BASE + 3)
#define ADDR_RC2D_DRAM_MIXSOUND_GAIN                       (ADDR_RC2D_DRAM_MIXSOUND_BASE + 4)
#define ADDR_RC2D_DRAM_MIXSOUND_UPX8_COEFF                 (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x10)
#define ADDR_RC2D_DRAM_MIXSOUND_UPX4_COEFF                 (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x15)
#define ADDR_RC2D_DRAM_MIXSOUND_UPX2_COEFF                 (ADDR_RC2D_DRAM_MIXSOUND_BASE + 0x1A)

/*------- Speaker Calibration ------------*/
#define ADDR_PEQ_COEF                                       (ADDR_RC2D_SPKCAL_BASE)
#define ADDR_PEQ_FILTER_HISTORY_R                           (ADDR_PEQ_COEF+45)//(ADDR_PEQ_COEF+40) 
#define ADDR_PEQ_FILTER_HISTORY_L                           (ADDR_PEQ_FILTER_HISTORY_R+36)//(ADDR_PEQ_FILTER_HISTORY_R+32)
#define ADDR_PEQ1_BW                                        (ADDR_PEQ_FILTER_HISTORY_L+36)//(ADDR_PEQ_FILTER_HISTORY_L+32)
#define ADDR_PEQ1_FC                                        (ADDR_PEQ1_BW+1)
#define ADDR_PEQ1_GAIN                                      (ADDR_PEQ1_FC+1)
#define ADDR_PEQ2_BW                                        (ADDR_PEQ1_GAIN+1)
#define ADDR_PEQ2_FC                                        (ADDR_PEQ2_BW+1)
#define ADDR_PEQ2_GAIN                                      (ADDR_PEQ2_FC+1)
#ifdef CC_NEW_POST_MULTI_VS_SUPPORT
/*-------Mono to Stereo--------*/
#define ADDR_M2S_COMB_R_COEF                     (CMPT_VSURR_DELAY_BUFFER)
#define ADDR_M2S_COMB_L_COEF                     (ADDR_M2S_COMB_R_COEF+70)
#define ADDR_M2S_ALLPASS_COEF                   (ADDR_M2S_COMB_L_COEF+70)
#define ADDR_M2S_FILTER_HISTORY_R                (ADDR_M2S_ALLPASS_COEF+20) 
#define ADDR_M2S_FILTER_HISTORY_L                   (ADDR_M2S_FILTER_HISTORY_R+56)
#define ADDR_M2S_FILTER_HISTORY_ALLPASS   (ADDR_M2S_FILTER_HISTORY_L+56)
#define ADDR_M2S_FILTER_HISTORY_HP_R         (ADDR_M2S_FILTER_HISTORY_ALLPASS)
#define ADDR_M2S_FILTER_HISTORY_HP_L         (ADDR_M2S_FILTER_HISTORY_HP_R+12)
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
/*-------Speaker Height--------*/
#define ADDR_SHELF_FILTER_HISTORY_R                   (ADDR_RC2D_SPKHEIGHT_BASE) 
#define ADDR_SHELF_FILTER_HISTORY_L                   (ADDR_SHELF_FILTER_HISTORY_R+12)
#define ADDR_ALLPASS1_FILTER_HISTORY                  (ADDR_SHELF_FILTER_HISTORY_L+12)
#define ADDR_SHELF_R_COEF                             (ADDR_ALLPASS1_FILTER_HISTORY+8)
#define ADDR_SHELF_L_COEF                             (ADDR_SHELF_R_COEF+15)
#define ADDR_ALLPASS1_COEF                            (ADDR_SHELF_L_COEF+15)
#define ADDR_SPKHIGH_PARA4                         (ADDR_ALLPASS1_COEF+10)
#define ADDR_SPKHIGH_PARA5                         (ADDR_SPKHIGH_PARA4+1)
#endif

//======================================================================
// DRAM_BASS_MANAGMENT Section (08820h~08870h) -- Background Decoder
//======================================================================
//Speaker Configuration
#define ADDR_RC2D_SPEAKER_CONFIG_DEC2                      (ADDR_RC2D_DRAM_BASS_MANAGEMENT_BASE_DEC2)
#define ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF_DEC2        (ADDR_RC2D_SPEAKER_CONFIG_DEC2 + 1)
//Channel Delay
#define ADDR_RC2D_CH_DELAY_C_DEC2                          (ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF_DEC2 + 10)
#define ADDR_RC2D_CH_DELAY_L_DEC2                          (ADDR_RC2D_CH_DELAY_C_DEC2 + 1)
#define ADDR_RC2D_CH_DELAY_R_DEC2                          (ADDR_RC2D_CH_DELAY_L_DEC2 + 1)
  //Value: 0~200 <<8 / 5cm
#define ADDR_RC2D_CH_DELAY_FACTOR_DEC2                     (ADDR_RC2D_CH_DELAY_R_DEC2 + 1)
//======================================================================
// DRAM_3D_FUNCTION_FLAG Section (08870h~08880h) -- Foreground Decoder
//======================================================================
// 3D processing
#define ADDR_RC2D_VIRTUAL_SURROUND_DEC2                    (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE_DEC2 + 0)
#define VSS_NONE                                                    (0x00     )
#define VSS_MTK                                                     (0x01 << 0)
#define VSS_N22                                                     (0x01 << 1)
#define VSS_SRS_WOW                                                 (0x01 << 2)
#define VSS_SONY                                                    (0x01 << 3)
#define VSS_TVS                                                     (0x01 << 4)
#define VSS_SRS_TSXT                                                (0x01 << 5)
#define VSS_QSOUND                                                  (0x01 << 6)
#define VSS_DOLBY_HP                                                (0x01 << 7)
#define VSS_M2S                                           	        (0x01 << 8)
#define ADDR_RC2D_PRO_LOGIC_II_CONFIG_DEC2                 (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE_DEC2 + 1)
#define ADDR_RC2D_PRO_LOGIC_II_MODE_DEC2                   (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE_DEC2 + 2)
#define ADDR_RC2D_NEO6_FLAG_DEC2                           (ADDR_RC2D_DRAM_3D_FUNCTION_FLAG_BASE_DEC2 + 3)

//=============================================================
// DRAM_PTS_STC_DEC2 Section (088a0h~088b0h) -- Background Decoder
//=============================================================
//PTS STC related
#define ADDR_RC2D_MEMORY_BUFFER_ADDRESS_DEC2               (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x00)
#define ADDR_RC2D_1ST_PTS_PRS_PNT_DEC2                     (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x01)
// 1st pts info to do init AV-sync
#define ADDR_RC2D_1ST_PTS_STCH_DEC2                        (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x02)
#define ADDR_RC2D_1ST_PTS_STCL_DEC2                        (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x03)
#define ADDR_RC2D_STC_DIFF_LOBOUND_DEC2                    (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x04)
// lo bound of stc_diff --> do stc measure (with stc_level_threhold) when over this bound
#define ADDR_RC2D_STC_DIFF_HIBOUND_DEC2                    (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x05)
// hi bound of stc_diff --> force to do repeat/drop when over this bound
#define ADDR_RC2D_STC_DIFF_WSBOUND_DEC2                    (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x06)
// worse bound of stc_diff --> ignore AV-sync when over this bound
#define ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC2                 (ADDR_RC2D_DRAM_PTS_STC_BASE_DEC2 + 0x07)
// level threshold of which frame to do repeat/drop


//===============================================================
// DRAM_EQUALIZER Section (088c0h~089e0h) -- Foreground Decoder
//===============================================================
  //OFF: 0x0, ON: 0x1 <<8
#define ADDR_RC2D_EQUALIZER_BAND_NUM_1_DEC2                (ADDR_RC2D_DRAM_EQUALIZER_BASE_DEC2)    	  //88C0h, 1 word (DRAM_EQUALIZER)
#define ADDR_RC2D_EQUALIZER_TABLE_CHANGE_DEC2              (ADDR_RC2D_EQUALIZER_BAND_NUM_1_DEC2 + 1)  //       1 word
#define ADDR_RC2D_EQUALIZER_TABLE_DEC2                     (ADDR_RC2D_EQUALIZER_TABLE_CHANGE_DEC2 + 1)//      40 words
#define ADDR_RC2D_EQUALIZER_HISTORY_DEC2                   (ADDR_RC2D_EQUALIZER_TABLE_DEC2 + 40)	  //     160 words for 8.0 channel
#define ADDR_RC2D_EQUALIZER_DRY_LEVEL_DEC2                 (ADDR_RC2D_EQUALIZER_HISTORY_DEC2 + 160)	  //       1 word
#define ADDR_RC2D_EQUALIZER_TABLE_NEW_DEC2                 (ADDR_RC2D_EQUALIZER_DRY_LEVEL_DEC2 + 1)	  //      40 words
#define ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT_DEC2           (ADDR_RC2D_EQUALIZER_TABLE_NEW_DEC2+40)   // 1 word
#define ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT_DEC2           (ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT_DEC2+1)   // 1 word

//======================================================================
// DRAM_AUDIO_BASIC_SETTING Section (089e0h~08a00h) -- Background Decoder
//======================================================================

//========================================================================================
// DRAM_VS_BUF_DEC2 Section (08a00h~08b00h) -- background Decoder
//========================================================================================
#define DRAM_VIRTUAL_SURROUND_DEC2                         (ADDR_RC2D_DRAM_VS_BUF_BASE_DEC2)
  // MTK VSurr
#define ADDR_RC2D_VIRTUAL_SURROUND_GAIN_DEC2               (DRAM_VIRTUAL_SURROUND_DEC2 + 0)
#define ADDR_RC2D_VIRTUAL_SURROUND_WIDEN_GAIN_DEC2         (ADDR_RC2D_VIRTUAL_SURROUND_GAIN_DEC2 + 1)
#define ADDR_RC2D_VIRTUAL_SURROUND_DELAY_NUMBER_DEC2       (ADDR_RC2D_VIRTUAL_SURROUND_WIDEN_GAIN_DEC2 + 1)
#define ADDR_RC2D_VIRTUAL_SURROUND_FILTER_COEFF_DEC2       (ADDR_RC2D_VIRTUAL_SURROUND_DELAY_NUMBER_DEC2 + 1)
  // SRS WOW
#define ADDR_SRS_WOW_BAKUP_D2                      (DRAM_VIRTUAL_SURROUND_DEC2    )    // 0 word, a mark only
#define ADDR_SRS_WOW_DATA_D2                       (ADDR_SRS_WOW_BAKUP_D2     +  0)    //28 Dwords
#define ADDR_SRS_WOW_RODATA_D2                     (ADDR_SRS_WOW_DATA_D2      + 28)    //37 Dwords
#define ADDR_SRS_WOW_BSS_D2                        (ADDR_SRS_WOW_RODATA_D2    + 37)    // 7 Dwords
#define ADDR_SRS_WOW_BANK2_BSS_D2                  (ADDR_SRS_WOW_BSS_D2       +  7)    //23 Dwords, filter states
#define ADDR_SRS_WOW_DRAM_USE_END_D2               (ADDR_SRS_WOW_BANK2_BSS_D2 + 23)    // 0 word, a mark only
  // SRS TSXT
#define ADDR_SRS_TSXT_BAKUP_D2                     (DRAM_VIRTUAL_SURROUND_DEC2     )   // 0 word, a mark only
#define ADDR_SRS_TSXT_DATA_D2                      (ADDR_SRS_TSXT_BAKUP_D2     +  0)
#define ADDR_SRS_TSXT_RODATA_D2                    (ADDR_SRS_TSXT_DATA_D2      + 29)
#define ADDR_SRS_TSXT_BSS_D2                       (ADDR_SRS_TSXT_RODATA_D2    + 69)
#define ADDR_SRS_TSXT_BANK2_BSS_D2                 (ADDR_SRS_TSXT_BSS_D2       + 10)
#define ADDR_SRS_TSXT_DRAM_USE_END_D2              (ADDR_SRS_TSXT_BANK2_BSS_D2 + 26)   // 0 word, a mark only

//===========================================================
// SUPER_BASS Section (08b00h~08be0h) -- Background Decoder
//===========================================================
//Super Bass
#ifdef DSP_EQ2_SBASS_SUPPORT
#define ADDR_RC2D_SUPER_BASS_TABLE_CHANGE_DEC2       (ADDR_RC2D_SUPER_BASS_BASE_DEC2)                  // 8B00h, 1 word
#define ADDR_RC2D_SUPER_BASS_TABLE_DEC2              (ADDR_RC2D_SUPER_BASS_TABLE_CHANGE_DEC2+1)                // 8B01h, 10 words
#define ADDR_RC2D_SUPER_BASS_HISTORY_DEC2            (ADDR_RC2D_SUPER_BASS_TABLE_DEC2+10)                      // 8B0Bh, 64 words for 8.0 channel
#define ADDR_RC2D_SUPER_BASS_HISTORY_CH0_DEC2     (ADDR_RC2D_SUPER_BASS_HISTORY_DEC2)                       // 8B0Bh
#define ADDR_RC2D_SUPER_BASS_HISTORY_CH1_DEC2        (ADDR_RC2D_SUPER_BASS_HISTORY_DEC2+8)                // 8B13h
#define ADDR_RC2D_SUPER_BASS_TABLE_NEW_DEC2          (ADDR_RC2D_SUPER_BASS_HISTORY_DEC2+64)                   // 8B4Bh, 10 words
#else
#define ADDR_RC2D_SUPER_BASS_DELAY_NUMBER_DEC2             (ADDR_RC2D_SUPER_BASS_BASE_DEC2)
#define ADDR_RC2D_SUPER_BASS_BOOST_GAIN_DEC2               (ADDR_RC2D_SUPER_BASS_BASE_DEC2 + 1)
#define ADDR_RC2D_SUPER_BASS_CLEAR_GAIN_DEC2               (ADDR_RC2D_SUPER_BASS_BASE_DEC2 + 2)
#define ADDR_RC2D_SUPER_BASS_PIPE_FILTER_COEFF_DEC2        (ADDR_RC2D_SUPER_BASS_BASE_DEC2 + 3)
#define ADDR_RC2D_SUPER_BASS_BOOST_FILTER_COEFF_DEC2       (ADDR_RC2D_SUPER_BASS_BASE_DEC2 + 8)
#define ADDR_RC2D_SUPER_BASS_CLEAR_FILTER_COEFF_DEC2       (ADDR_RC2D_SUPER_BASS_BASE_DEC2 + 0x0D)
#endif
//============================================================
// DRAM_REVERB_DEC2 Section (08be0h~08c00h) -- background Decoder
//============================================================
#define ADDR_RC2D_REVERB_BANK_ADDR_DEC2                    (ADDR_RC2D_DRAM_REVERB_BASE_DEC2)
#define ADDR_RC2D_REVERB_BANK_MAX_DEC2                     (ADDR_RC2D_DRAM_REVERB_BASE_DEC2 + 0x01)
#define ADDR_RC2D_REVERB_BANK_NL_PR_DEC2                   (ADDR_RC2D_DRAM_REVERB_BASE_DEC2 + 0x02)
#define ADDR_RC2D_REVERB_BANK_PL_NR_DEC2                   (ADDR_RC2D_DRAM_REVERB_BASE_DEC2 + 0x03)
#define ADDR_RC2D_REVERB_BANK_PL_PR_DEC2                   (ADDR_RC2D_DRAM_REVERB_BASE_DEC2 + 0x04)
#define ADDR_RC2D_REVERB_IIR_A_DEC2                        (ADDR_RC2D_DRAM_REVERB_BASE_DEC2 + 0x05)
#define ADDR_RC2D_REVERB_IIR_B_DEC2                        (ADDR_RC2D_DRAM_REVERB_BASE_DEC2 + 0x06)
#define ADDR_RC2D_REVERB_GAIN_DEC2                         (ADDR_RC2D_DRAM_REVERB_BASE_DEC2 + 0x07)

// Light added for AVC
//============================================================
// DRAM_AVC Section (08d00h~08d60h) -- Foreground Decoder
//============================================================
#define ADDR_RC2D_AVC_TARGET_LEVEL                         (ADDR_RC2D_DRAM_AVC_BASE + 0x00)
#define ADDR_RC2D_AVC_SILENCE_LEVEL                        (ADDR_RC2D_DRAM_AVC_BASE + 0x01)
#define ADDR_RC2D_AVC_MAX_GAIN_UP                          (ADDR_RC2D_DRAM_AVC_BASE + 0x02)
#define ADDR_RC2D_AVC_MAX_GAIN_DOWN                        (ADDR_RC2D_DRAM_AVC_BASE + 0x03)
#define ADDR_RC2D_AVC_FLAG                                 (ADDR_RC2D_DRAM_AVC_BASE + 0x04)
#define ADDR_RC2D_AVC_ATTACK_THRES                         (ADDR_RC2D_DRAM_AVC_BASE + 0x05)
#define ADDR_RC2D_AVC_ADJUST_RATE                          (ADDR_RC2D_DRAM_AVC_BASE + 0x06)
#define ADDR_RC2D_AVC_DSP_FLAG                             (ADDR_RC2D_DRAM_AVC_BASE + 0x07)
#define ADDR_RC2D_AVC_UI_ADJUST_RATE                       (ADDR_RC2D_DRAM_AVC_BASE + 0x08)


//NEW_AVC
#define ADDR_RC2D_AVC_TARGET_LEVEL_DEC2               (ADDR_RC2D_DRAM_AVC_BASE_DEC2+ 0x00)
#define ADDR_RC2D_AVC_SILENCE_LEVEL_DEC2              (ADDR_RC2D_DRAM_AVC_BASE_DEC2 + 0x01)
#define ADDR_RC2D_AVC_MAX_GAIN_UP_DEC2                (ADDR_RC2D_DRAM_AVC_BASE_DEC2 + 0x02)
#define ADDR_RC2D_AVC_MAX_GAIN_DOWN_DEC2          (ADDR_RC2D_DRAM_AVC_BASE_DEC2 + 0x03)
#define ADDR_RC2D_AVC_FLAG_DEC2                                (ADDR_RC2D_DRAM_AVC_BASE_DEC2 + 0x04)
#define ADDR_RC2D_AVC_ATTACK_THRES_DEC2               (ADDR_RC2D_DRAM_AVC_BASE_DEC2 + 0x05)
#define ADDR_RC2D_AVC_ADJUST_RATE_DEC2                 (ADDR_RC2D_DRAM_AVC_BASE_DEC2 + 0x06)
#define ADDR_RC2D_AVC_DSP_FLAG_DEC2                       (ADDR_RC2D_DRAM_AVC_BASE_DEC2 + 0x07)
#define ADDR_RC2D_AVC_UI_ADJUST_RATE_DEC2           (ADDR_RC2D_DRAM_AVC_BASE_DEC2 + 0x08)



/*------- virtual bass ------------*/
#define ADDR_VBASS_R_COEF                     (ADDR_RC2D_VBASS_BASE)
#define ADDR_VBASS_L_COEF                     (ADDR_VBASS_R_COEF+70)
#define ADDR_VBASS_HIGHPASS_COEF                   (ADDR_VBASS_L_COEF+70)
#define ADDR_VBASS_COMB_FILTER_HISTORY_R   (ADDR_VBASS_HIGHPASS_COEF+20)
#define ADDR_VBASS_COMB_FILTER_HISTORY_L   (ADDR_VBASS_COMB_FILTER_HISTORY_R+56)
#define ADDR_VBASS_ALLPASS_FILTER_HISTORY (ADDR_VBASS_COMB_FILTER_HISTORY_L+56)
#define ADDR_VBASS_HIGHPASS_FILTER_HISTORY_L  (ADDR_VBASS_ALLPASS_FILTER_HISTORY+12)
#define ADDR_VBASS_CROS_PARA4                   (ADDR_VBASS_HIGHPASS_FILTER_HISTORY_L+12)
#define ADDR_VBASS_CROS_PARA5                   (ADDR_VBASS_CROS_PARA4+1)
#define ADDR_VBASS_CROS_PARA6                   (ADDR_VBASS_CROS_PARA5+1)
#define ADDR_VBASS_CROS_PARA7                   (ADDR_VBASS_CROS_PARA6+1) 
#define ADDR_EXPEND_GAIN                             (ADDR_VBASS_CROS_PARA7+1)
#define ADDR_EXPEND_COUNT                        (ADDR_EXPEND_GAIN+1) 
#define ADDR_dram_attck_count                           (ADDR_EXPEND_COUNT+1)  
#define ADDR_limiter_step                                 (ADDR_dram_attck_count+1)  
#define ADDR_VBASS_CROS_PARA8                         (ADDR_limiter_step+1)
#define ADDR_VBASS_CROS_PARA9                         (ADDR_VBASS_CROS_PARA8+1)




// ( Normal ) -- Foreground Decoder
// DSP Address 8dc0h~8defh (48 Dwords)
#define ADDR_RC2D_DCF_COEF                                 (ADDR_RC2D_DCF_BASE + 0)
#define ADDR_RC2D_DCF_HIS                                  (ADDR_RC2D_DCF_BASE + 1)

#define ADDR_RC2D_DCF_COEF_DEC2                            (ADDR_RC2D_DCF_BASE_DEC2 + 0)
#define ADDR_RC2D_DCF_HIS_DEC2                             (ADDR_RC2D_DCF_BASE_DEC2 + 1)

//(Normal)
// DTS Downmix Table Section (08ea0h~08eech)
//#define DRAM_DOWNMIX_TABLE_ADR                       (ADDR_RC2D_DTS_DOWNMIX_BASE + 0x00)


//=================================================
// DRAM_RISC_SHARED_MEMORY Section (08f80h~08fffh)
//=================================================
#ifdef CC_MT5391_AUD_SUPPORT
//### CTRL7
#define DSP_RC2D_CTRL7_AREA_SZ              10
#define ADDR_RC2D_CTRL_7_NUM_1              (ADDR_RC2D_CTRL7_BASE + 0x0000)
#define ADDR_RC2D_STC_DIFF_DEC3             (ADDR_RC2D_CTRL7_BASE + 0x0001)
#define ADDR_RC2D_SPEED_DEC3                (ADDR_RC2D_CTRL7_BASE + 0x0002)
#define ADDR_RC2D_PROCESSING_MODE_DEC3          (ADDR_RC2D_CTRL7_BASE + 0x0003)
#define ADDR_RC2D_BIT_RESOLUTION_DEC3           (ADDR_RC2D_CTRL7_BASE + 0x0004)
#define ADDR_RC2D_KARAOKE_FLAG_DEC3            (ADDR_RC2D_CTRL7_BASE + 0x0005)
#define ADDR_RC2D_LRCH_MIX_RATIO_DEC3           (ADDR_RC2D_CTRL7_BASE + 0x0006)
#define ADDR_RC2D_MASTER_VOLUME_DEC3            (ADDR_RC2D_CTRL7_BASE + 0x000A)
#define ADDR_RC2D_SPDIF_FLAG_DEC3               (ADDR_RC2D_CTRL7_BASE + 0x000B)
#define ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC3     (ADDR_RC2D_CTRL7_BASE + 0x000C)
#define ADDR_RC2D_DECODING_STR_PNT_DEC3         (ADDR_RC2D_CTRL7_BASE + 0x000D)
#else  // fir MT536x
#ifdef CC_MT5360B
#define DSP_RC2D_CTRL7_AREA_SZ              10
#define ADDR_RC2D_CTRL_7_NUM_1              (ADDR_RC2D_CTRL7_BASE + 0x0000)
#define ADDR_RC2D_STC_DIFF_DEC3             (ADDR_RC2D_CTRL7_BASE + 0x0001)
#define ADDR_RC2D_SPEED_DEC3                (ADDR_RC2D_CTRL7_BASE + 0x0002)
#define ADDR_RC2D_PROCESSING_MODE_DEC3          (ADDR_RC2D_CTRL7_BASE + 0x0003)
#define ADDR_RC2D_BIT_RESOLUTION_DEC3           (ADDR_RC2D_CTRL7_BASE + 0x0004)
#define ADDR_RC2D_KARAOKE_FLAG_DEC3            (ADDR_RC2D_CTRL7_BASE + 0x0005)
#define ADDR_RC2D_LRCH_MIX_RATIO_DEC3           (ADDR_RC2D_CTRL7_BASE + 0x0006)
#define ADDR_RC2D_MASTER_VOLUME_DEC3            (ADDR_RC2D_CTRL7_BASE + 0x000A)
#define ADDR_RC2D_SPDIF_FLAG_DEC3               (ADDR_RC2D_CTRL7_BASE + 0x000B)
#define ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC3     (ADDR_RC2D_CTRL7_BASE + 0x000C)
#define ADDR_RC2D_DECODING_STR_PNT_DEC3         (ADDR_RC2D_CTRL7_BASE + 0x000D)
#else //5360A
//### CTRL7
#define DSP_RC2D_CTRL7_AREA_SZ              10
#define ADDR_RC2D_CTRL_7_NUM_1              (ADDR_RC2D_CTRL7_BASE + 0x0000)
#define ADDR_RC2D_STC_DIFF_DEC3             (ADDR_RC2D_CTRL7_BASE + 0x0001)
#define ADDR_RC2D_PROCESSING_MODE_DEC3          (ADDR_RC2D_CTRL7_BASE + 0x0002)
#define ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC3     (ADDR_RC2D_CTRL7_BASE + 0x0003)
#define ADDR_RC2D_BIT_RESOLUTION_DEC3           (ADDR_RC2D_CTRL7_BASE + 0x0004)
#define ADDR_RC2D_KARAOKE_FLAG_DEC3            (ADDR_RC2D_CTRL7_BASE + 0x0005)
#define ADDR_RC2D_MASTER_VOLUME_DEC3            (ADDR_RC2D_CTRL7_BASE + 0x0006)
#define ADDR_RC2D_DECODING_STR_PNT_DEC3         (ADDR_RC2D_CTRL7_BASE + 0x0008)
#define ADDR_RC2D_LRCH_MIX_RATIO_DEC3           (ADDR_RC2D_CTRL7_BASE + 0x0006)
//#define ADDR_RC2D_MPEG_SYNC_MODE_DEC3        (ADDR_RC2D_CTRL7_BASE + 0x000A)
#define ADDR_RC2D_SPDIF_FLAG_DEC3               (ADDR_RC2D_CTRL7_BASE + 0x000B)
#define ADDR_RC2D_MPEG_CRC_FLAG_DEC3        (ADDR_RC2D_CTRL7_BASE + 0x000C)
#endif
#endif

// Note that there's no CTRL8 related SRAM in MT536X, but we keep the DRAM variables
// here for code consistency with MT539x. Since there's no PCM_DEC3 applications, 
// it's temp OK.
//### CTRL8
#define DSP_RC2D_CTRL8_AREA_SZ                  12
#define ADDR_RC2D_CTRL_8_NUM_1                  (ADDR_RC2D_CTRL8_BASE + 0x000)
#define ADDR_RC2D_DOWNSAMPLE_DEC3               (ADDR_RC2D_CTRL8_BASE + 0x001)   // 30
//Channel Use -> Pink Noise
#define ADDR_RC2D_CHANNEL_USE_DEC3              (ADDR_RC2D_CTRL8_BASE + 0x002)   // 31
#define RC2D_INT92                              (ADDR_RC2D_CTRL8_BASE + 0x003)
#define ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT_DEC3                    (RC2D_INT92)
#define ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC3               (RC2D_INT92)

#define RC2D_INT93                              (ADDR_RC2D_CTRL8_BASE + 0x004)
  //OFF: 0x1 <<8, V1,V2: 0x2 <<8, None: 0x4 <<8
  //V1: 0x8 <<8, V2: 0x10 <<8, V1+V2: 0x20 <<8
//Dolby Dual Mode
#define ADDR_RC2D_LPCM_FREQUENCY_DEC3                             (RC2D_INT93)
#define ADDR_RC2D_HDCD_CONFIG_DEC3                                (RC2D_INT93)
#define ADDR_RC2D_MPEG_SYNC_MODE_DEC3                             (RC2D_INT93)
#define ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC3                    (RC2D_INT93) // Light added

#define RC2D_INT94                              (ADDR_RC2D_CTRL8_BASE + 0x005)
#define ADDR_RC2D_LPCM_BIT_SHIFT_DEC3                             (RC2D_INT94)
#define ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC3                      (RC2D_INT94) // Light added

#define RC2D_INT95                              (ADDR_RC2D_CTRL8_BASE + 0x006)
  //Custom Mode 0: 0x0, Custom Mode 1: 0x1 <<8
  //Line Mode: 0x2 <<8, RF Mode: 0x3 <<8
#define ADDR_RC2D_LPCM_DRC_VALUE_DEC3                             (RC2D_INT95)
#define ADDR_RC2D_PCM_INPUT_TYPE_DEC3                             (RC2D_INT95) 

#define RC2D_INT96                              (ADDR_RC2D_CTRL8_BASE + 0x007)
#define ADDR_RC2D_LPCM_Q_DEC3                                     (RC2D_INT96)

#define RC2D_INT97                              (ADDR_RC2D_CTRL8_BASE + 0x008)
#define ADDR_RC2D_MPEG_DRC_FLAG_DEC3                              (RC2D_INT97)
#define ADDR_RC2D_LPCM_DRC_FLAG_DEC3                              (RC2D_INT97)
#define ADDR_RC2D_ADPCM_CHANNEL_NUM_DEC3                          (RC2D_INT97)

#define RC2D_INT98                              (ADDR_RC2D_CTRL8_BASE + 0x009)
//MPEG CRC Value
#ifdef CC_MT5391_AUD_SUPPORT
#define ADDR_RC2D_MPEG_CRC_FLAG_DEC3	                          (RC2D_INT98)
#endif
#ifdef CC_MT5360B
#define ADDR_RC2D_MPEG_CRC_FLAG_DEC3	                          (RC2D_INT98)
#endif
#define ADDR_RC2D_LPCM_DECODING_TYPE_DEC3                         (RC2D_INT98)

#define RC2D_INT99                              (ADDR_RC2D_CTRL8_BASE + 0x00A)
#define ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE_DEC3                 (RC2D_INT99)

#define RC2D_INT9A                              (ADDR_RC2D_CTRL8_BASE + 0x00B)
#define ADDR_RC2D_PCM_LRCK_MODE_DEC3                     (RC2D_INT9A)

#define RC2D_INT9B                              (ADDR_RC2D_CTRL8_BASE + 0x00C)
#define ADDR_RC2D_LPCM_ADPCM_DEC_TYPE_DEC3                        (RC2D_INT9B)


//### Decoder3 Info
#define ADDR_RC2D_DECODER3_INFO	     (ADDR_RC2D_DECODER_INFO_BASE_DEC3 + 0x0000)

#define ADDR_RC2D_IEC_FLAG_DEC3             (ADDR_RC2D_DECODER3_INFO + 0x0000)
  //#define IEC_DSP_MUTE                                            (0x1 << 15)
#define ADDR_RC2D_IEC_LATENCY_DEC3          (ADDR_RC2D_DECODER3_INFO + 0x0001)
#define ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC3  (ADDR_RC2D_DECODER3_INFO + 0x0006)
#define ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC3  (ADDR_RC2D_DECODER3_INFO + 0x0007)

#define ADDR_RC2D_INPUT_SRC_GAIN_DEC3       (ADDR_RC2D_DECODER3_INFO + 0x000A)
#define ADDR_RC2D_AOUT_BANK_384_NUM_DEC3    (ADDR_RC2D_DECODER3_INFO + 0x000C)
#define ADDR_RC2D_SPDIF_WORD_LENGTH_DEC3    (ADDR_RC2D_DECODER3_INFO + 0x000D)
#define ADDR_RC2D_AUDIO_MUTE_THRESHOLD_DEC3 (ADDR_RC2D_DECODER3_INFO + 0x000E)
#define ADDR_RC2D_DISC_TYPE_DEC3            (ADDR_RC2D_DECODER3_INFO + 0x000F)
//### MISC dec3
#define ADDR_RC2D_CGMS_INFO_DEC3            (ADDR_RC2D_DECODER3_INFO + 0x0010)
#define ADDR_RC2D_AOUT_BANK_576_NUM_DEC3    (ADDR_RC2D_DECODER3_INFO + 0x0019)
#define ADDR_RC2D_AOUT_BANK_320_NUM_DEC3    (ADDR_RC2D_DECODER3_INFO + 0x001A)
#define ADDR_RC2D_AOUT_BANK_256_NUM_DEC3    (ADDR_RC2D_DECODER3_INFO + 0x001B)
#define ADDR_RC2D_VOLUME_LEVEL_UP_ORDER_DEC3    (ADDR_RC2D_DECODER3_INFO + 0x001D)
#define ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER_DEC3  (ADDR_RC2D_DECODER3_INFO + 0x001E)
#define ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC3   (ADDR_RC2D_DECODER3_INFO + 0x001F)


/*------- 3D Program RAM address ------------*/
#define MULTI_SURROUND_PRAM_ADR                             (0x0010/2)
#define VIRTUAL_SURROUND_PRAM_ADR                           (0x0510/2)
#define SPEAKER_ENHANCEMENT_PRAM_ADR                        (0x0B10/2)
#define VOICE_ENHANCEMENT_PRAM_ADR                          (0x0F00/2)

/*const used for DDCO */
#define DDCO_PRAM_ADR                                                (0x0900/2)
#define DDCO_PRAM_ADR_2                                                (0x0F00/2)
#define DDCO_TABLE_ADR                                              (0x13F00+0x400)

/*const used for AAC */
#define AAC_CMPT_TABLE_ADR								    (0x21c0)

#include "rc2d_int.h"
#define SPECTRUM_HIGH_ORDER                                 0x7
#define SPECTRUM_LOW_ORDER                                  0x5


#include "d2rc_shm.h"
#include "dsp_reg.h"


/*===================== First Decoder Page ========================*/

// Decoder Page
#define DRAM_DECODING_TABLE_ADDR                            (0x0400)
//DISC Downmix Table
#define ADDR_RC2D_LPCM_ATSDWNMIXCOEF_SADR                   (DRAM_DECODING_TABLE_ADDR+0xaf)
LINT_EXT_HEADER_END

#endif   //CTRL_PARA_H

