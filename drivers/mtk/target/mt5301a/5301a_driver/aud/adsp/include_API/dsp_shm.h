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
 * $RCSfile: dsp_shm.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_shm.h
 *  Brief of file dsp_shm.h.
 *  Details of file dsp_shm.h (optional).
 */

/* PLEASE NOTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*                                                                            */
/* Strict rules - NO replacement of share memory for backward compatible      */
/*                Until new version for architecture is established           */
/*                                                                            */
/* PLEASE NOTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

#ifndef DSP_SHM_H_
#define DSP_SHM_H_

//#include "d2rc_shm.h"

#define   DSP_GRP_MAX                  64

/************** System related ********************/
#define   SYSTEM_GRP                   0

#define   W_INTCTRL                    ((SYSTEM_GRP * 32) + 0x00)   // system
#define   W_SYSTEM_CTRL                ((SYSTEM_GRP * 32) + 0x02)   // system
#define   W_SYSTEM_SETUP               ((SYSTEM_GRP * 32) + 0x04)   // system
#define   W_SYSTEM_SETUP_DEC2          ((SYSTEM_GRP * 32) + 0x06)   // system
#define   W_INTERNAL_BUF_IN_FLAG       ((SYSTEM_GRP * 32) + 0x08)   // system
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
#define   W_SYSTEM_SETUP_DEC3          ((SYSTEM_GRP * 32) + 0x0A)   // system
#define   W_INTERNAL_BUF2_IN_FLAG       ((SYSTEM_GRP * 32) + 0x0C)   // system
#endif
/************** Mixsound related ********************/
#define   MXSND_GRP                    1

#define   W_MIXSOUND_FLAG              ((MXSND_GRP * 32) + 0x00)    // mixed sound
#define   W_MIXSOUND_CFG               ((MXSND_GRP * 32) + 0x02)    // mixed sound
#define   D_MIXSOUND_PADDR             ((MXSND_GRP * 32) + 0x04)    // mixed sound
#define   D_MIXSOUND_PLEN              ((MXSND_GRP * 32) + 0x08)    // mixed sound
#define   D_MIXSOUND_GAIN              ((MXSND_GRP * 32) + 0x0C)    // mixed sound
#define   W_LIMITER_MODE               ((MXSND_GRP * 32) + 0x10)    // limiter
#define   W_LIMITER_FLAG               ((MXSND_GRP * 32) + 0x12)    // limiter
#define   D_LIMITER_FIX_THRESHOLD      ((MXSND_GRP * 32) + 0x14)    // limiter
#define   D_LIMITER_GAIN_RATIO         ((MXSND_GRP * 32) + 0x18)    // limiter

/************** Volume related part-1 for first decoder ********************/
#define   VOL_GRP1                     2

#define   D_TRIM_C                     ((VOL_GRP1 * 32) + 0x00) // first decoder
#define   D_TRIM_L                     ((VOL_GRP1 * 32) + 0x04) // first decoder
#define   D_TRIM_R                     ((VOL_GRP1 * 32) + 0x08) // first decoder
#define   D_TRIM_LS                    ((VOL_GRP1 * 32) + 0x0C) // first decoder
#define   D_TRIM_RS                    ((VOL_GRP1 * 32) + 0x10) // first decoder
#define   D_TRIM_CH7                   ((VOL_GRP1 * 32) + 0x14) // first decoder
#define   D_TRIM_CH8                   ((VOL_GRP1 * 32) + 0x18) // first decoder
#define   D_TRIM_SUB                   ((VOL_GRP1 * 32) + 0x1C) // first decoder

/************** Volume related part-2 for first decoder ********************/
#define   VOL_GRP2                     3

#define   D_TRIM_LFE                   ((VOL_GRP2 * 32) + 0x00) // first decoder
#define   D_DIALOGUE                   ((VOL_GRP2 * 32) + 0x04) // first decoder
#define   D_VOL                        ((VOL_GRP2 * 32) + 0x08) // first decoder
#define   B_VOLUPORDER                 ((VOL_GRP2 * 32) + 0x0C) // first decoder
#define   B_VOLDOWNORDER               ((VOL_GRP2 * 32) + 0x0D) // first decoder
#define   B_SOFTMUTEORDER              ((VOL_GRP2 * 32) + 0x0E) // first decoder
#define   W_ERRORMUTEBANK              ((VOL_GRP2 * 32) + 0x10) // first decoder
#define   B_DRC_AVEORDER               ((VOL_GRP2 * 32) + 0x12) // first decoder
#define   B_DRC_ATTACKORDER            ((VOL_GRP2 * 32) + 0x13) // first decoder
#define   W_DRC_THRESHOLD              ((VOL_GRP2 * 32) + 0x14) // first decoder
#define   W_DRC_MARGIN                 ((VOL_GRP2 * 32) + 0x16) // first decoder
#define   D_DRC_RE_GAIN                ((VOL_GRP2 * 32) + 0x18) // first decoder
#define   D_INSRC_GAIN                 ((VOL_GRP2 * 32) + 0x1C) // first decoder

/************** Volume related part-1 for second decoder ********************/
#define   VOL_GRP3                     4

#define   D_TRIM_C_DEC2                ((VOL_GRP3 * 32) + 0x00) // second decoder -- dummy
#define   D_TRIM_L_DEC2                ((VOL_GRP3 * 32) + 0x04) // second decoder
#define   D_TRIM_R_DEC2                ((VOL_GRP3 * 32) + 0x08) // second decoder
// Light: Add trim of ch9 / ch10 in group 4, since group 2/3 are full.
#define   D_TRIM_CH9                   ((VOL_GRP3 * 32) + 0x0C) // first decoder
#define   D_TRIM_CH10                  ((VOL_GRP3 * 32) + 0x10) // first decoder
#define   D_INSRC_GAIN_DEC2            ((VOL_GRP3 * 32) + 0x14) // second decoder

/************** Volume related part-2 for second decoder ********************/
#define   VOL_GRP4                     5

#define   D_TRIM_LFE_DEC2              ((VOL_GRP4 * 32) + 0x00) // second decoder
#define   D_DIALOGUE_DEC2              ((VOL_GRP4 * 32) + 0x04) // second decoder
#define   D_VOL_DEC2                   ((VOL_GRP4 * 32) + 0x08) // second decoder
#define   B_VOLUPORDER_DEC2            ((VOL_GRP4 * 32) + 0x0C) // second decoder
#define   B_VOLDOWNORDER_DEC2          ((VOL_GRP4 * 32) + 0x0D) // second decoder
#define   B_SOFTMUTEORDER_DEC2         ((VOL_GRP4 * 32) + 0x0E) // second decoder
#define   W_ERRORMUTEBANK_DEC2         ((VOL_GRP4 * 32) + 0x10) // second decoder
#define   B_DRC_AVEORDER_DEC2          ((VOL_GRP4 * 32) + 0x12) // second decoder
#define   B_DRC_ATTACKORDER_DEC2       ((VOL_GRP4 * 32) + 0x13) // second decoder
#define   W_DRC_THRESHOLD_DEC2         ((VOL_GRP4 * 32) + 0x14) // second decoder
#define   W_DRC_MARGIN_DEC2            ((VOL_GRP4 * 32) + 0x16) // second decoder
#define   D_DRC_RE_GAIN_DEC2           ((VOL_GRP4 * 32) + 0x18) // second decoder

/************** Ctrl for first decoder ********************/
#define   CTRL_GRP1                    6

#define   W_SPEED                      ((CTRL_GRP1 * 32) + 0x00)    // first decoder
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
#define   W_PROCMOD                    ((CTRL_GRP1 * 32) + 0x02)    // first decoder
#define   W_PINKNOISE                  ((CTRL_GRP1 * 32) + 0x04)    // first decoder
#define   B_BIT                        ((CTRL_GRP1 * 32) + 0x06)    // first decoder
#define   B_KARAFLAG                   ((CTRL_GRP1 * 32) + 0x07)    // first decoder
#define   D_LRMIXRATIO                 ((CTRL_GRP1 * 32) + 0x08)    // first decoder
#define   D_DECODING_STR_PNT           ((CTRL_GRP1 * 32) + 0x1C)    // first decoder

/************** Ctrl for second decoder ********************/
#define   CTRL_GRP2                    7

#define   W_SPEED_DEC2                 ((CTRL_GRP2 * 32) + 0x00)    // second decoder
#define   W_PROCMOD_DEC2               ((CTRL_GRP2 * 32) + 0x02)    // second decoder
#define   W_PINKNOISE_DEC2             ((CTRL_GRP2 * 32) + 0x04)    // second decoder
#define   B_BIT_DEC2                   ((CTRL_GRP2 * 32) + 0x06)    // second decoder
#define   B_KARAFLAG_DEC2              ((CTRL_GRP2 * 32) + 0x07)    // second decoder
#define   D_LRMIXRATIO_DEC2            ((CTRL_GRP2 * 32) + 0x08)    // second decoder
#define   D_DECODING_STR_PNT_DEC2      ((CTRL_GRP2 * 32) + 0x1C)    // second decoder

/************** Config for first decoder ********************/
#define   CFG_GRP1                     8

#define   B_BANK384NUM                 ((CFG_GRP1 * 32) + 0x00) // first decoder
#define   B_BANK576NUM                 ((CFG_GRP1 * 32) + 0x01) // first decoder
#define   B_BANK320NUM                 ((CFG_GRP1 * 32) + 0x02) // first decoder
#define   B_BANK256NUM                 ((CFG_GRP1 * 32) + 0x03) // first decoder
#define   D_MEMBACKUPADDR              ((CFG_GRP1 * 32) + 0x04) // first decoder
#define   B_DACBIT                     ((CFG_GRP1 * 32) + 0x08) // first decoder
#define   B_EQFLAG                     ((CFG_GRP1 * 32) + 0x09) // first decoder
#define   B_EQBANDNUM                  ((CFG_GRP1 * 32) + 0x0A) // first decoder
#define   B_SBASSDELAYNUM              ((CFG_GRP1 * 32) + 0x0B) // first decoder
#define   D_SBASSBOOSTGAIN             ((CFG_GRP1 * 32) + 0x0C) // first decoder
#define   D_SBASSCLEARGAIN             ((CFG_GRP1 * 32) + 0x10) // first decoder
#define   B_SPECTRUMUP                 ((CFG_GRP1 * 32) + 0x14) // first decoder
#define   B_SPECTRUMDOWN               ((CFG_GRP1 * 32) + 0x15) // first decoder

/************** Config for second decoder ********************/
#define   CFG_GRP2                     9

#define   B_BANK384NUM_DEC2            ((CFG_GRP2 * 32) + 0x00) // second decoder
#define   B_BANK576NUM_DEC2            ((CFG_GRP2 * 32) + 0x01) // second decoder
#define   B_BANK320NUM_DEC2            ((CFG_GRP2 * 32) + 0x02) // second decoder
#define   B_BANK256NUM_DEC2            ((CFG_GRP2 * 32) + 0x03) // second decoder
#define   D_MEMBACKUPADDR_DEC2         ((CFG_GRP2 * 32) + 0x04) // second decoder
#define   B_DACBIT_DEC2                ((CFG_GRP2 * 32) + 0x08) // second decoder
/* currently not support */
#define   B_EQFLAG_DEC2                ((CFG_GRP2 * 32) + 0x09)      // second decoder
#define   B_EQBANDNUM_DEC2             ((CFG_GRP2 * 32) + 0x0A)      // second decoder
#define   B_SBASSDELAYNUM_DEC2         ((CFG_GRP2 * 32) + 0x0B)      // second decoder
#define   D_SBASSBOOSTGAIN_DEC2        ((CFG_GRP2 * 32) + 0x0C)      // second decoder
#define   D_SBASSCLEARGAIN_DEC2        ((CFG_GRP2 * 32) + 0x10)      // second decoder

/************** Decoder option for first decoder ********************/
#define   DEC_GRP1                     10

#define   W_AC3KARAMOD                 ((DEC_GRP1 * 32) + 0x00) // first decoder
#define   W_AC3DUALMODE                ((DEC_GRP1 * 32) + 0x02) // first decoder
#define   W_AC3COMPMOD                 ((DEC_GRP1 * 32) + 0x04) // first decoder
#define   D_AC3DYN_LOW                 ((DEC_GRP1 * 32) + 0x08) // first decoder
#define   D_AC3DYN_HIGH                ((DEC_GRP1 * 32) + 0x0C) // first decoder
#define   B_AC3AUTODNMIX               ((DEC_GRP1 * 32) + 0x10) // first decoder
#define   B_MPEGDRC                    ((DEC_GRP1 * 32) + 0x11) // first decoder
#define   W_MPEGERRDET                 ((DEC_GRP1 * 32) + 0x12) // first decoder
#define   W_WMAPACKET                  ((DEC_GRP1 * 32) + 0x14) // first decoder
#define   W_VORBISPACKET               ((DEC_GRP1 * 32) + 0x16) // first decoder
#define   W_PCM_INPUT_CH_ASSIGNMENT    ((DEC_GRP1 * 32) + 0x18) // first decoder
#define   W_PCM_INPUT_CHANNEL_NUM      ((DEC_GRP1 * 32) + 0x1a) // first decoder
#define   W_PCM_INPUT_TYPE                    ((DEC_GRP1 * 32) + 0x1C)  // first decoder
#define   W_PCM_INPUT_SAMPLING_RATE                    ((DEC_GRP1 * 32) + 0x1e)  // first decoder
/************** Decoder option for first decoder ********************/
#define   DEC_GRP2                     11

#define   W_AC3KARAMOD_DEC2            ((DEC_GRP2 * 32) + 0x00) // second decoder
#define   W_AC3DUALMODE_DEC2           ((DEC_GRP2 * 32) + 0x02) // second decoder
#define   W_AC3COMPMOD_DEC2            ((DEC_GRP2 * 32) + 0x04) // second decoder
#define   D_AC3DYN_LOW_DEC2            ((DEC_GRP2 * 32) + 0x08) // second decoder
#define   D_AC3DYN_HIGH_DEC2           ((DEC_GRP2 * 32) + 0x0C) // second decoder
#define   B_AC3AUTODNMIX_DEC2          ((DEC_GRP2 * 32) + 0x10) // second decoder
#define   B_MPEGDRC_DEC2               ((DEC_GRP2 * 32) + 0x11) // second decoder
#define   W_MPEGERRDET_DEC2            ((DEC_GRP2 * 32) + 0x12) // second decoder
#define   W_PCM_INPUT_CH_ASSIGNMENT_DEC2 ((DEC_GRP2 * 32) + 0x18) // second decoder
#define   W_PCM_INPUT_CHANNEL_NUM_DEC2 ((DEC_GRP2 * 32) + 0x1a) // second decoder
#define   W_PCM_INPUT_TYPE_DEC2                    ((DEC_GRP2 * 32) + 0x1C)  // second decoder
#define   W_PCM_INPUT_SAMPLING_RATE_DEC2                    ((DEC_GRP2 * 32) + 0x1e)  // second decoder
/************** Bass related for first decoder ********************/
#define   BASS_GRP1                    12

#define   D_SPKCFG                     ((BASS_GRP1 * 32) + 0x00)    // first decoder
#define   D_CUTOFF_FREQ                ((BASS_GRP1 * 32) + 0x04)    // first decoder
#define   W_CHDELAY_C                  ((BASS_GRP1 * 32) + 0x08)    // first decoder
#define   W_CHDELAY_L                  ((BASS_GRP1 * 32) + 0x0A)    // first decoder
#define   W_CHDELAY_R                  ((BASS_GRP1 * 32) + 0x0C)    // first decoder
#define   W_CHDELAY_LS                 ((BASS_GRP1 * 32) + 0x0E)    // first decoder
#define   W_CHDELAY_RS                 ((BASS_GRP1 * 32) + 0x10)    // first decoder
#define   W_CHDELAY_CH7                ((BASS_GRP1 * 32) + 0x12)    // first decoder
#define   W_CHDELAY_CH8                ((BASS_GRP1 * 32) + 0x14)    // first decoder
#define   W_CHDELAY_SUB                ((BASS_GRP1 * 32) + 0x16)    // first decoder
#define   W_CHDELAY_CH9                ((BASS_GRP1 * 32) + 0x18)    // first decoder
#define   W_CHDELAY_CH10               ((BASS_GRP1 * 32) + 0x1A)    // first decoder
#define   W_AC3_SPKCFG                 ((BASS_GRP1 * 32) + 0x1C)    // first decoder
#define   W_MP2_SPKCFG                 ((BASS_GRP1 * 32) + 0x1E)    // first decoder

/************** Bass related for second decoder ********************/
#define   BASS_GRP2                    13
#define   D_SPKCFG_DEC2                ((BASS_GRP2 * 32) + 0x00)    // second decoder
#define   D_CUTOFF_FREQ_DEC2           ((BASS_GRP2 * 32) + 0x04)    // second decoder
#define   W_CHDELAY_C_DEC2             ((BASS_GRP2 * 32) + 0x08)    // second decoder -- dummy
#define   W_CHDELAY_L_DEC2             ((BASS_GRP2 * 32) + 0x0A)    // second decoder
#define   W_CHDELAY_R_DEC2             ((BASS_GRP2 * 32) + 0x0C)    // second decoder
#define   W_AC3_SPKCFG_DEC2            ((BASS_GRP2 * 32) + 0x1C)    // second decoder
#define   W_MP2_SPKCFG_DEC2            ((BASS_GRP2 * 32) + 0x1E)    // second decoder

/************** IEC related for both decoders ********************/
#define   IEC_GRP1                     14

#define   B_IECFLAG                    ((IEC_GRP1 * 32) + 0x00)
#define   B_IEC_MUTE                   ((IEC_GRP1 * 32) + 0x01)
#define   B_IEC_PCMCH                  ((IEC_GRP1 * 32) + 0x02)
#define   B_IEC_OPTION                 ((IEC_GRP1 * 32) + 0x03)
#define   B_IEC_MAX_FREQ               ((IEC_GRP1 * 32) + 0x04)
#define   B_IECLATENCY                 ((IEC_GRP1 * 32) + 0x05)
#define   B_IEC_WORD_LENGTH            ((IEC_GRP1 * 32) + 0x06)
#define   B_IEC_COPYRIGHT              ((IEC_GRP1 * 32) + 0x07)
#define   B_IEC_CATEGORY_CODE          ((IEC_GRP1 * 32) + 0x08)

#ifdef CC_MT5391_AUD_3_DECODER
//#if (0)
/************** DEC3 related  ********************/
#define   DEC3_GRP1                     15

#define   D_VOL_DEC3                   ((DEC3_GRP1 * 32) + 0x00) // 3rd decoder
#define   D_INSRC_GAIN_DEC3     ((DEC3_GRP1 * 32) + 0x04) // 3rd decoder
#define   W_PROCMOD_DEC3         ((DEC3_GRP1 * 32) + 0x08)   
#define   B_BANK384NUM_DEC3    ((DEC3_GRP1 * 32) + 0x0A) // 3rd decoder
#define   B_BANK576NUM_DEC3    ((DEC3_GRP1 * 32) + 0x0B) // 3rd decoder
#define   B_BANK320NUM_DEC3    ((DEC3_GRP1 * 32) + 0x0C) // 3rd decoder
#define   B_BANK256NUM_DEC3    ((DEC3_GRP1 * 32) + 0x0D) // 3rd decoder
#define   W_PCM_INPUT_CH_ASSIGNMENT_DEC3 ((DEC3_GRP1 * 32) + 0x0E) // 3rd decoder
#define   W_PCM_INPUT_CHANNEL_NUM_DEC3 ((DEC3_GRP1 * 32) + 0x10) // 3rd decoder
#define   W_PCM_INPUT_TYPE_DEC3                    ((DEC3_GRP1 * 32) + 0x12)  // 3rd decoder
#define   W_PCM_INPUT_SAMPLING_RATE_DEC3   ((DEC3_GRP1 * 32) + 0x14)
#define   W_MP2_SPKCFG_DEC3            ((BASS_GRP2 * 32) + 0x16)
#define   W_MPEGERRDET_DEC3   ((DEC3_GRP1 * 32) + 0x18) 
#define   B_MPEGDRC_DEC3         ((DEC3_GRP1 * 32) + 0x1A) // 3rd decode
#define   B_BIT_DEC3                   ((DEC3_GRP1 * 32) + 0x1B)    // 3rd decoder
#define   D_MEMBACKUPADDR_DEC3         ((DEC3_GRP1 * 32) + 0x1C) // 3rd decoder
#endif
/************** 3D(Reverb) for first decoder ********************/
#define   DSP3D_GRP1                   16

#define   D_REVERBMEMADDR              ((DSP3D_GRP1 * 32) + 0x00)   // first decoder
#define   B_REVERBFLAG                 ((DSP3D_GRP1 * 32) + 0x04)   // first decoder
#define   D_REVERBGAIN                 ((DSP3D_GRP1 * 32) + 0x08)   // first decoder
#define   B_REVERBBANK0                ((DSP3D_GRP1 * 32) + 0x0C)   // first decoder
#define   B_REVERBBANK1                ((DSP3D_GRP1 * 32) + 0x0D)   // first decoder
#define   B_REVERBBANK2                ((DSP3D_GRP1 * 32) + 0x0E)   // first decoder
#define   B_REVERBBANK3                ((DSP3D_GRP1 * 32) + 0x0F)   // first decoder

/************** 3D(VSS/PL2) for first decoder ********************/
#define   DSP3D_GRP2                   17

#define   D_DNMX_BUF_ADR               ((DSP3D_GRP2 * 32) + 0x00)   // first decoder
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
#define   B_VSURRFLAG                  ((DSP3D_GRP2 * 32) + 0x04)   // first decoder
#define   B_VSURRDELAYNUM              ((DSP3D_GRP2 * 32) + 0x05)   // first decoder
#else
#define   W_VSURRFLAG                  ((DSP3D_GRP2 * 32) + 0x04)   // first decoder
#define   B_VSURRDELAYNUM              ((DSP3D_GRP2 * 32) + 0x06)   // first decoder
#endif
#define   D_VSURRGAIN                  ((DSP3D_GRP2 * 32) + 0x08)   // first decoder
#define   D_VSURRWIDEGAIN              ((DSP3D_GRP2 * 32) + 0x0C)   // first decoder
#define   D_3D_BUF_ADDR                ((DSP3D_GRP2 * 32) + 0x10)   // first decoder
#define   W_PLSURRDELAY                ((DSP3D_GRP2 * 32) + 0x14)   // first decoder
#define   W_PLIICONFIG                 ((DSP3D_GRP2 * 32) + 0x16)   // first decoder
#define   W_PLIIMODE                   ((DSP3D_GRP2 * 32) + 0x18)   // first decoder
#define   B_ADFLAG                     ((DSP3D_GRP2 * 32) + 0x1A)   // first decoder  //For Audio Description
#define   B_DDCO_FLAG                     ((DSP3D_GRP2 * 32) + 0x1B)   // first decoder  ,ddco flag
#define   B_DDCO_AGC_FLAG                     ((DSP3D_GRP2 * 32) + 0x1C)   // first decoder  ,ddco  AGC flag
#define   B_DDCO_LFE_LPF_FLAG                     ((DSP3D_GRP2 * 32) + 0x1D)   // first decoder  ,ddco  LFE LPF flag



/************** 3D(Reverb) for second decoder ********************/
#define   DSP3D_GRP3                   18

#define   D_REVERBMEMADDR_DEC2         ((DSP3D_GRP3 * 32) + 0x00)    // second decoder
#define   B_REVERBFLAG_DEC2            ((DSP3D_GRP3 * 32) + 0x04)    // second decoder
#define   D_REVERBGAIN_DEC2            ((DSP3D_GRP3 * 32) + 0x08)    // second decoder
#define   B_REVERBBANK0_DEC2           ((DSP3D_GRP3 * 32) + 0x0C)    // second decoder
#define   B_REVERBBANK1_DEC2           ((DSP3D_GRP3 * 32) + 0x0D)    // second decoder
#define   B_REVERBBANK2_DEC2           ((DSP3D_GRP3 * 32) + 0x0E)    // second decoder
#define   B_REVERBBANK3_DEC2           ((DSP3D_GRP3 * 32) + 0x0F)    // second decoder

/************** 3D(VSS/PL2) for second decoder ********************/
#define   DSP3D_GRP4                   19

/* currently not support */
//#define   D_DNMX_BUF_ADR_DEC2          ((DSP3D_GRP4 * 32) + 0x00)    // second decoder
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
#define   B_VSURRFLAG_DEC2             ((DSP3D_GRP4 * 32) + 0x04)    // second decoder
#define   B_VSURRDELAYNUM_DEC2         ((DSP3D_GRP4 * 32) + 0x05)    // second decoder
#else
#define   W_VSURRFLAG_DEC2             ((DSP3D_GRP4 * 32) + 0x04)    // second decoder
#define   B_VSURRDELAYNUM_DEC2         ((DSP3D_GRP4 * 32) + 0x06)    // second decoder
#endif
#define   D_VSURRGAIN_DEC2             ((DSP3D_GRP4 * 32) + 0x08)    // second decoder
#define   D_VSURRWIDEGAIN_DEC2         ((DSP3D_GRP4 * 32) + 0x0C)    // second decoder
//#define   D_3D_BUF_ADDR_DEC2           ((DSP3D_GRP4 * 32) + 0x10)    // second decoder
//#define   W_PLSURRDELAY_DEC2           ((DSP3D_GRP4 * 32) + 0x14)    // second decoder
//#define   W_PLIICONFIG_DEC2            ((DSP3D_GRP4 * 32) + 0x16)    // second decoder
//#define   W_PLIIMODE_DEC2              ((DSP3D_GRP4 * 32) + 0x18)    // second decoder

/************** Equalizer for first decoder ********************/
#define   DSP_EQ_GRP1                  20
#define   D_EQ_DRY                      ((DSP_EQ_GRP1 * 32) + 0x00)
#define   B_EQ_BAND1                    ((DSP_EQ_GRP1 * 32) + 0x04)
#define   B_EQ_BAND2                    ((DSP_EQ_GRP1 * 32) + 0x05)
#define   B_EQ_BAND3                    ((DSP_EQ_GRP1 * 32) + 0x06)
#define   B_EQ_BAND4                    ((DSP_EQ_GRP1 * 32) + 0x07)
#define   B_EQ_BAND5                    ((DSP_EQ_GRP1 * 32) + 0x08)
#define   B_EQ_BAND6                    ((DSP_EQ_GRP1 * 32) + 0x09)
#define   B_EQ_BAND7                    ((DSP_EQ_GRP1 * 32) + 0x0A)
#define   B_EQ_BAND8                    ((DSP_EQ_GRP1 * 32) + 0x0B)
#define   B_EQ_BAND9                    ((DSP_EQ_GRP1 * 32) + 0x0C)
#define   B_EQ_BAND10                   ((DSP_EQ_GRP1 * 32) + 0x0D)

/************** Equalizer for second decoder ********************/
#define   DSP_EQ_GRP2                  21
#define   D_EQ_DRY_DEC2                ((DSP_EQ_GRP2 * 32) + 0x00)
#define   B_EQ_BAND1_DEC2               ((DSP_EQ_GRP2 * 32) + 0x04)
#define   B_EQ_BAND2_DEC2               ((DSP_EQ_GRP2 * 32) + 0x05)
#define   B_EQ_BAND3_DEC2               ((DSP_EQ_GRP2 * 32) + 0x06)
#define   B_EQ_BAND4_DEC2               ((DSP_EQ_GRP2 * 32) + 0x07)
#define   B_EQ_BAND5_DEC2               ((DSP_EQ_GRP2 * 32) + 0x08)
#define   B_EQ_BAND6_DEC2               ((DSP_EQ_GRP2 * 32) + 0x09)
#define   B_EQ_BAND7_DEC2               ((DSP_EQ_GRP2 * 32) + 0x0A)
#define   B_EQ_BAND8_DEC2               ((DSP_EQ_GRP2 * 32) + 0x0B)
#define   B_EQ_BAND9_DEC2               ((DSP_EQ_GRP2 * 32) + 0x0C)
#define   B_EQ_BAND10_DEC2              ((DSP_EQ_GRP2 * 32) + 0x0D)

#ifdef CC_MT5391_AUD_3_DECODER
//#if (0)
/************** DEC3 related  ********************/
#define   DEC3_GRP2                     22

#define   B_SOFTMUTEORDER_DEC3                       ((DEC3_GRP2 * 32) + 0x00) // 3rd decoder

/************** PTS/STC for first decoder ********************/
#define   DSP_STC_GRP3                 23

#define   D_1ST_PTS_PRS_PNT_DEC3       ((DSP_STC_GRP3 * 32) + 0x00) // second decoder
#define   W_1ST_PTS_STCH_DEC3          ((DSP_STC_GRP3 * 32) + 0x04) // second decoder
#define   W_1ST_PTS_STCL_DEC3          ((DSP_STC_GRP3 * 32) + 0x06) // second decoder
#define   D_STC_DIFF_LO_DEC3           ((DSP_STC_GRP3 * 32) + 0x08) // second decoder
#define   D_STC_DIFF_HI_DEC3           ((DSP_STC_GRP3 * 32) + 0x0c) // second decoder
#define   D_STC_DIFF_WS_DEC3           ((DSP_STC_GRP1 * 32) + 0x10) // second decoder 
#define   D_STC_THRESHOLD_DEC3         ((DSP_STC_GRP3 * 32) + 0x14) // second decoder

#endif
/************** noise reduction for atv decoder ********************/
#define   DSP_NR_GRP1                 24
#define   W_NR_PAL_TH1_HIGH   		   ((DSP_NR_GRP1 * 32) + 0x00)
#define   W_NR_PAL_TH1_LOW   		   ((DSP_NR_GRP1 * 32) + 0x02)
#define   W_NR_PAL_TH2_HIGH   		   ((DSP_NR_GRP1 * 32) + 0x04)
#define   W_NR_PAL_TH2_LOW   		   ((DSP_NR_GRP1 * 32) + 0x06)
#define   W_NR_PAL_HDEV_TH1_HIGH   	   ((DSP_NR_GRP1 * 32) + 0x08)
#define   W_NR_PAL_HDEV_TH1_LOW   	   ((DSP_NR_GRP1 * 32) + 0x0A)
#define   W_NR_PAL_HDEV_TH2_HIGH   	   ((DSP_NR_GRP1 * 32) + 0x0C)
#define   W_NR_PAL_HDEV_TH2_LOW   	   ((DSP_NR_GRP1 * 32) + 0x0E)
#define   W_NR_AM_TH1_HIGH   	       ((DSP_NR_GRP1 * 32) + 0x10)
#define   W_NR_AM_TH1_LOW   	       ((DSP_NR_GRP1 * 32) + 0x12)
#define   W_NR_AM_TH2_HIGH   	       ((DSP_NR_GRP1 * 32) + 0x14)
#define   W_NR_AM_TH2_LOW   	       ((DSP_NR_GRP1 * 32) + 0x16)
#define   W_NR_A2_TH1_HIGH   	       ((DSP_NR_GRP1 * 32) + 0x18)
#define   W_NR_A2_TH1_LOW   	       ((DSP_NR_GRP1 * 32) + 0x1A)
#define   W_NR_A2_TH2_HIGH   	       ((DSP_NR_GRP1 * 32) + 0x1C)
#define   W_NR_A2_TH2_LOW   	       ((DSP_NR_GRP1 * 32) + 0x1E)

/************** noise reduction for atv decoder ********************/
#define   DSP_NR_GRP2                 25
#define   W_NR_ENABLE   		       ((DSP_NR_GRP2 * 32) + 0x00)
#define   W_NR_MTS_TH1_HIGH   	       ((DSP_NR_GRP2 * 32) + 0x02)
#define   W_NR_MTS_TH1_LOW   	       ((DSP_NR_GRP2 * 32) + 0x04)
#define   W_NR_MTS_TH2_HIGH   	       ((DSP_NR_GRP2 * 32) + 0x06)
#define   W_NR_MTS_TH2_LOW   	       ((DSP_NR_GRP2 * 32) + 0x08)
#define   W_NR_SAP_TH1_HIGH   	       ((DSP_NR_GRP2 * 32) + 0x0A)
#define   W_NR_SAP_TH1_LOW   	       ((DSP_NR_GRP2 * 32) + 0x0C)
#define   W_NR_SAP_TH2_HIGH   	       ((DSP_NR_GRP2 * 32) + 0x0E)
#define   W_NR_SAP_TH2_LOW   	       ((DSP_NR_GRP2 * 32) + 0x10)

#if (0)
#define   DSP_EQ_BAND_START            (DSP_EQ_GRP1 * 32)   // first decoder

#define   D_C_DRY                      ((DSP_EQ_GRP1 * 32) + 0x00)  // first decoder
#define   D_C_BAND1                    ((DSP_EQ_GRP1 * 32) + 0x04)  // first decoder
#define   D_C_BAND2                    ((DSP_EQ_GRP1 * 32) + 0x08)  // first decoder
#define   D_C_BAND3                    ((DSP_EQ_GRP1 * 32) + 0x0C)  // first decoder
#define   D_C_BAND4                    ((DSP_EQ_GRP1 * 32) + 0x10)  // first decoder
#define   D_C_BAND5                    ((DSP_EQ_GRP1 * 32) + 0x14)  // first decoder
#define   D_C_BAND6                    ((DSP_EQ_GRP1 * 32) + 0x18)  // first decoder
#define   D_C_BAND7                    ((DSP_EQ_GRP1 * 32) + 0x1C)  // first decoder

#define   DSP_EQ_GRP2                  21

#define   D_C_BAND8                    ((DSP_EQ_GRP2 * 32) + 0x00)  // first decoder
#define   D_C_BAND9                    ((DSP_EQ_GRP2 * 32) + 0x04)  // first decoder
#define   D_C_BAND10                   ((DSP_EQ_GRP2 * 32) + 0x08)  // first decoder
#define   D_L_DRY                      ((DSP_EQ_GRP2 * 32) + 0x0C)  // first decoder
#define   D_L_BAND1                    ((DSP_EQ_GRP2 * 32) + 0x10)  // first decoder
#define   D_L_BAND2                    ((DSP_EQ_GRP2 * 32) + 0x14)  // first decoder
#define   D_L_BAND3                    ((DSP_EQ_GRP2 * 32) + 0x18)  // first decoder
#define   D_L_BAND4                    ((DSP_EQ_GRP2 * 32) + 0x1C)  // first decoder

#define   DSP_EQ_GRP3                  22

#define   D_L_BAND5                    ((DSP_EQ_GRP3 * 32) + 0x00)  // first decoder
#define   D_L_BAND6                    ((DSP_EQ_GRP3 * 32) + 0x04)  // first decoder
#define   D_L_BAND7                    ((DSP_EQ_GRP3 * 32) + 0x08)  // first decoder
#define   D_L_BAND8                    ((DSP_EQ_GRP3 * 32) + 0x0C)  // first decoder
#define   D_L_BAND9                    ((DSP_EQ_GRP3 * 32) + 0x10)  // first decoder
#define   D_L_BAND10                   ((DSP_EQ_GRP3 * 32) + 0x14)  // first decoder
#define   D_R_DRY                      ((DSP_EQ_GRP3 * 32) + 0x18)  // first decoder
#define   D_R_BAND1                    ((DSP_EQ_GRP3 * 32) + 0x1C)  // first decoder

#define   DSP_EQ_GRP4                  23

#define   D_R_BAND2                    ((DSP_EQ_GRP4 * 32) + 0x00)  // first decoder
#define   D_R_BAND3                    ((DSP_EQ_GRP4 * 32) + 0x04)  // first decoder
#define   D_R_BAND4                    ((DSP_EQ_GRP4 * 32) + 0x08)  // first decoder
#define   D_R_BAND5                    ((DSP_EQ_GRP4 * 32) + 0x0C)  // first decoder
#define   D_R_BAND6                    ((DSP_EQ_GRP4 * 32) + 0x10)  // first decoder
#define   D_R_BAND7                    ((DSP_EQ_GRP4 * 32) + 0x14)  // first decoder
#define   D_R_BAND8                    ((DSP_EQ_GRP4 * 32) + 0x18)  // first decoder
#define   D_R_BAND9                    ((DSP_EQ_GRP4 * 32) + 0x1C)  // first decoder

#define   DSP_EQ_GRP5                  24

#define   D_R_BAND10                   ((DSP_EQ_GRP5 * 32) + 0x00)  // first decoder
#define   D_LS_DRY                     ((DSP_EQ_GRP5 * 32) + 0x04)  // first decoder
#define   D_LS_BAND1                   ((DSP_EQ_GRP5 * 32) + 0x08)  // first decoder
#define   D_LS_BAND2                   ((DSP_EQ_GRP5 * 32) + 0x0C)  // first decoder
#define   D_LS_BAND3                   ((DSP_EQ_GRP5 * 32) + 0x10)  // first decoder
#define   D_LS_BAND4                   ((DSP_EQ_GRP5 * 32) + 0x14)  // first decoder
#define   D_LS_BAND5                   ((DSP_EQ_GRP4 * 32) + 0x18)  // first decoder
#define   D_LS_BAND6                   ((DSP_EQ_GRP4 * 32) + 0x1C)  // first decoder

#define   DSP_EQ_GRP6                  25

#define   D_LS_BAND7                   ((DSP_EQ_GRP6 * 32) + 0x00)  // first decoder
#define   D_LS_BAND8                   ((DSP_EQ_GRP6 * 32) + 0x04)  // first decoder
#define   D_LS_BAND9                   ((DSP_EQ_GRP6 * 32) + 0x08)  // first decoder
#define   D_LS_BAND10                  ((DSP_EQ_GRP6 * 32) + 0x0C)  // first decoder
#define   D_RS_DRY                     ((DSP_EQ_GRP6 * 32) + 0x10)  // first decoder
#define   D_RS_BAND1                   ((DSP_EQ_GRP6 * 32) + 0x14)  // first decoder
#define   D_RS_BAND2                   ((DSP_EQ_GRP6 * 32) + 0x18)  // first decoder
#define   D_RS_BAND3                   ((DSP_EQ_GRP6 * 32) + 0x1C)  // first decoder

#define   DSP_EQ_GRP7                  26

#define   D_RS_BAND4                   ((DSP_EQ_GRP7 * 32) + 0x00)  // first decoder
#define   D_RS_BAND5                   ((DSP_EQ_GRP7 * 32) + 0x04)  // first decoder
#define   D_RS_BAND6                   ((DSP_EQ_GRP7 * 32) + 0x08)  // first decoder
#define   D_RS_BAND7                   ((DSP_EQ_GRP7 * 32) + 0x0C)  // first decoder
#define   D_RS_BAND8                   ((DSP_EQ_GRP7 * 32) + 0x10)  // first decoder
#define   D_RS_BAND9                   ((DSP_EQ_GRP7 * 32) + 0x14)  // first decoder
#define   D_RS_BAND10                  ((DSP_EQ_GRP7 * 32) + 0x18)  // first decoder
#define   D_CH7_DRY                    ((DSP_EQ_GRP7 * 32) + 0x1C)  // first decoder

#define   DSP_EQ_GRP8                  27

#define   D_CH7_BAND1                  ((DSP_EQ_GRP8 * 32) + 0x00)  // first decoder
#define   D_CH7_BAND2                  ((DSP_EQ_GRP8 * 32) + 0x04)  // first decoder
#define   D_CH7_BAND3                  ((DSP_EQ_GRP8 * 32) + 0x08)  // first decoder
#define   D_CH7_BAND4                  ((DSP_EQ_GRP8 * 32) + 0x0C)  // first decoder
#define   D_CH7_BAND5                  ((DSP_EQ_GRP8 * 32) + 0x10)  // first decoder
#define   D_CH7_BAND6                  ((DSP_EQ_GRP8 * 32) + 0x14)  // first decoder
#define   D_CH7_BAND7                  ((DSP_EQ_GRP8 * 32) + 0x18)  // first decoder
#define   D_CH7_BAND8                  ((DSP_EQ_GRP8 * 32) + 0x1C)  // first decoder

#define   DSP_EQ_GRP9                  28

#define   D_CH7_BAND9                  ((DSP_EQ_GRP9 * 32) + 0x00)  // first decoder
#define   D_CH7_BAND10                 ((DSP_EQ_GRP9 * 32) + 0x04)  // first decoder
#define   D_CH8_DRY                    ((DSP_EQ_GRP9 * 32) + 0x08)  // first decoder
#define   D_CH8_BAND1                  ((DSP_EQ_GRP9 * 32) + 0x0C)  // first decoder
#define   D_CH8_BAND2                  ((DSP_EQ_GRP9 * 32) + 0x10)  // first decoder
#define   D_CH8_BAND3                  ((DSP_EQ_GRP9 * 32) + 0x14)  // first decoder
#define   D_CH8_BAND4                  ((DSP_EQ_GRP9 * 32) + 0x18)  // first decoder
#define   D_CH8_BAND5                  ((DSP_EQ_GRP9 * 32) + 0x1C)  // first decoder

#define   DSP_EQ_GRP10                 29

#define   D_CH8_BAND6                  ((DSP_EQ_GRP10 * 32) + 0x00) // first decoder
#define   D_CH8_BAND7                  ((DSP_EQ_GRP10 * 32) + 0x04) // first decoder
#define   D_CH8_BAND8                  ((DSP_EQ_GRP10 * 32) + 0x08) // first decoder
#define   D_CH8_BAND9                  ((DSP_EQ_GRP10 * 32) + 0x0C) // first decoder
#define   D_CH8_BAND10                 ((DSP_EQ_GRP10 * 32) + 0x10) // first decoder

/************** Equalizer for second decoder ********************/
#define   DSP_EQ_GRP11                 30
#define   DSP_EQ_BAND_START_DEC2       (DSP_EQ_GRP11 * 32)  // second decoder

/* currently not support */
#define   D_L_DRY_DEC2                 ((DSP_EQ_GRP11 * 32) + 0x00)  // second decoder
#define   D_L_BAND1_DEC2               ((DSP_EQ_GRP11 * 32) + 0x04)  // second decoder
#define   D_L_BAND2_DEC2               ((DSP_EQ_GRP11 * 32) + 0x08)  // second decoder
#define   D_L_BAND3_DEC2               ((DSP_EQ_GRP11 * 32) + 0x0C)  // second decoder
#define   D_L_BAND4_DEC2               ((DSP_EQ_GRP11 * 32) + 0x10)  // second decoder
#define   D_L_BAND5_DEC2               ((DSP_EQ_GRP11 * 32) + 0x14)  // second decoder
#define   D_L_BAND6_DEC2               ((DSP_EQ_GRP11 * 32) + 0x18)  // second decoder
#define   D_L_BAND7_DEC2               ((DSP_EQ_GRP11 * 32) + 0x1C)  // second decoder

#define   DSP_EQ_GRP12                 31

#define   D_L_BAND8_DEC2               ((DSP_EQ_GRP12 * 32) + 0x00)  // second decoder
#define   D_L_BAND9_DEC2               ((DSP_EQ_GRP12 * 32) + 0x04)  // second decoder
#define   D_L_BAND10_DEC2              ((DSP_EQ_GRP12 * 32) + 0x08)  // second decoder
#define   D_R_DRY_DEC2                 ((DSP_EQ_GRP12 * 32) + 0x0C)  // second decoder
#define   D_R_BAND1_DEC2               ((DSP_EQ_GRP12 * 32) + 0x10)  // second decoder
#define   D_R_BAND2_DEC2               ((DSP_EQ_GRP12 * 32) + 0x14)  // second decoder
#define   D_R_BAND3_DEC2               ((DSP_EQ_GRP12 * 32) + 0x18)  // second decoder
#define   D_R_BAND4_DEC2               ((DSP_EQ_GRP12 * 32) + 0x1C)  // second decoder

#define   DSP_EQ_GRP13                 32

#define   D_R_BAND5_DEC2               ((DSP_EQ_GRP13 * 32) + 0x00)  // second decoder
#define   D_R_BAND6_DEC2               ((DSP_EQ_GRP13 * 32) + 0x04)  // second decoder
#define   D_R_BAND7_DEC2               ((DSP_EQ_GRP13 * 32) + 0x08)  // second decoder
#define   D_R_BAND8_DEC2               ((DSP_EQ_GRP13 * 32) + 0x0C)  // second decoder
#define   D_R_BAND9_DEC2               ((DSP_EQ_GRP13 * 32) + 0x10)  // second decoder
#define   D_R_BAND10_DEC2              ((DSP_EQ_GRP13 * 32) + 0x14)  // second decoder
#endif
/************** AVC for first decoder ********************/
// Light added for AVC
#define   DSP_AVC_GRP1                 33

#define   W_AVC_TARGET_LEV             ((DSP_AVC_GRP1 * 32) + 0x00)
#define   W_AVC_SILENCE_LEV            ((DSP_AVC_GRP1 * 32) + 0x02)
#define   W_AVC_MAX_GAIN_UP            ((DSP_AVC_GRP1 * 32) + 0x04)
#define   W_AVC_MAX_GAIN_DOWN          ((DSP_AVC_GRP1 * 32) + 0x06)
#define   W_AVC_FLAG                   ((DSP_AVC_GRP1 * 32) + 0x08)
#define   W_AVC_ATTACK_THRES           ((DSP_AVC_GRP1 * 32) + 0x0a)
#define   W_AVC_ADJUST_RATE            ((DSP_AVC_GRP1 * 32) + 0x0c)
#define   W_DSP_AVC_UI_ADJUST_RATE     ((DSP_AVC_GRP1 * 32) + 0x0e)

/************** AVC for second decoder ********************/
// Light added for AVC
#define   DSP_AVC_GRP2                 34

#define   W_AVC_TARGET_LEV_DEC2        ((DSP_AVC_GRP2 * 32) + 0x00)
#define   W_AVC_SILENCE_LEV_DEC2       ((DSP_AVC_GRP2 * 32) + 0x02)
#define   W_AVC_MAX_GAIN_UP_DEC2       ((DSP_AVC_GRP2 * 32) + 0x04)
#define   W_AVC_MAX_GAIN_DOWN_DEC2     ((DSP_AVC_GRP2 * 32) + 0x06)
#define   W_AVC_FLAG_DEC2              ((DSP_AVC_GRP2 * 32) + 0x08)
#define   W_AVC_ATTACK_THRES_DEC2      ((DSP_AVC_GRP2 * 32) + 0x0a)
#define   W_AVC_ADJUST_RATE_DEC2       ((DSP_AVC_GRP2 * 32) + 0x0c)
#define   W_DSP_AVC_UI_ADJUST_RATE_DEC2 ((DSP_AVC_GRP2 * 32) + 0x0e)

/************** PTS/STC for first decoder ********************/
#define   DSP_STC_GRP1                 35

#define   D_1ST_PTS_PRS_PNT            ((DSP_STC_GRP1 * 32) + 0x00) // first decoder
#define   W_1ST_PTS_STCH               ((DSP_STC_GRP1 * 32) + 0x04) // first decoder
#define   W_1ST_PTS_STCL               ((DSP_STC_GRP1 * 32) + 0x06) // first decoder
#define   D_STC_DIFF_LO                ((DSP_STC_GRP1 * 32) + 0x08) // first decoder
#define   D_STC_DIFF_HI                ((DSP_STC_GRP1 * 32) + 0x0c) // first decoder
#define   D_STC_DIFF_WS                ((DSP_STC_GRP1 * 32) + 0x10) // first decoder 
#define   D_STC_THRESHOLD              ((DSP_STC_GRP1 * 32) + 0x14) // first decoder

/************** PTS/STC for first decoder ********************/
#define   DSP_STC_GRP2                 36

#define   D_1ST_PTS_PRS_PNT_DEC2       ((DSP_STC_GRP2 * 32) + 0x00) // second decoder
#define   W_1ST_PTS_STCH_DEC2          ((DSP_STC_GRP2 * 32) + 0x04) // second decoder
#define   W_1ST_PTS_STCL_DEC2          ((DSP_STC_GRP2 * 32) + 0x06) // second decoder
#define   D_STC_DIFF_LO_DEC2           ((DSP_STC_GRP2 * 32) + 0x08) // second decoder
#define   D_STC_DIFF_HI_DEC2           ((DSP_STC_GRP2 * 32) + 0x0c) // second decoder
#define   D_STC_DIFF_WS_DEC2           ((DSP_STC_GRP2 * 32) + 0x10) // second decoder 
#define   D_STC_THRESHOLD_DEC2         ((DSP_STC_GRP2 * 32) + 0x14) // second decoder

/************** NPTV Demodulation ********************/
#define   DEMOD_GRP                     37
#define   W_SOURCE_MODE                 ((DEMOD_GRP * 32) + 0x0)
#define   W_USER_MODE                   ((DEMOD_GRP * 32) + 0x2)
#define   W_ADC_GAIN                    ((DEMOD_GRP * 32) + 0x4)
#define   B_DETECT_DEFAULT              ((DEMOD_GRP * 32) + 0x06)
#define   W_VOL_GAIN                    ((DEMOD_GRP * 32) + 0x08)

/************** NPTV Demodulation Detetion 1 ********************/
#define  DETECTION_GPR1                 38
#define  W_MTS_NUM_CHECK                ((DETECTION_GPR1 * 32) + 0x0)
#define  W_MTS_STEREO_MID               ((DETECTION_GPR1 * 32) + 0x2)
#define  W_MTS_STEREO_CON_MID           ((DETECTION_GPR1 * 32) + 0x4)
#define  W_MTS_PILOT3_MID               ((DETECTION_GPR1 * 32) + 0x6)
#define  W_MTS_PILOT3_CON_MID           ((DETECTION_GPR1 * 32) + 0x8)
#define  W_MTS_SAP_MID                  ((DETECTION_GPR1 * 32) + 0xA)
#define  W_MTS_SAP_CON_MID              ((DETECTION_GPR1 * 32) + 0xC)
#define  W_FMFM_NUM_CHECK               ((DETECTION_GPR1 * 32) + 0xE)
#define  W_FMFM_NUM_DOUBLE_CHECK        ((DETECTION_GPR1 * 32) + 0x10)
#define  W_FMFM_MONO_WEIGHT             ((DETECTION_GPR1 * 32) + 0x12)
#define  W_FMFM_STEREO_WEIGHT           ((DETECTION_GPR1 * 32) + 0x14)
#define  W_FMFM_DUAL_WEIGHT             ((DETECTION_GPR1 * 32) + 0x16)
#define  W_A2_NUM_CHECK                 ((DETECTION_GPR1 * 32) + 0x18)
#define  W_A2_NUM_DOUBLE_CHECK          ((DETECTION_GPR1 * 32) + 0x1A)
#define  W_A2_MONO_WEIGHT               ((DETECTION_GPR1 * 32) + 0x1C)
#define  W_A2_STEREO_WEIGHT             ((DETECTION_GPR1 * 32) + 0x1E)
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
#define  B_FM_BAD_SIGNAL                       ((DETECTION_GPR1 * 32) + 0x1F)  
#endif

/************** NPTV Demodulation Group 2 ********************/
#define  DETECTION_GPR2                 39
#define  W_A2_DUAL_WEIGHT               ((DETECTION_GPR2 * 32) + 0x0)
#define  W_PAL_CORRECT_THRESHOLD        ((DETECTION_GPR2 * 32) + 0x2)
#define  W_PAL_TOTAL_SYNC1_LOOP         ((DETECTION_GPR2 * 32) + 0x4)
#define  W_PAL_ERR_THRESHOLD            ((DETECTION_GPR2 * 32) + 0x6)
#define  B_CANON_PLUS                   ((DETECTION_GPR2 * 32) + 0x7)
#define  B_HDEV_MODE                    ((DETECTION_GPR2 * 32) + 0x8)
#define  B_AM_MUTE_MODE                 ((DETECTION_GPR2 * 32) + 0x9)
#define  B_AM_MUTE_THRESHOLD_HIGHT      ((DETECTION_GPR2 * 32) + 0xA)
#define  B_AM_MUTE_THRESHOLD_LOW        ((DETECTION_GPR2 * 32) + 0xB)
#define  B_CARRIER_SHIFT_MODE           ((DETECTION_GPR2 * 32) + 0xC)
#define  B_FM_SATU_MUTE_MODE            ((DETECTION_GPR2 * 32) + 0xD)
#define  W_A2_MONO_STEREO_THRES_EU         ((DETECTION_GPR2 * 32) + 0xE)
#define  W_A2_STEREO_MONO_THRES_EU         ((DETECTION_GPR2 * 32) + 0x10)
#define  W_PAL_PARITY_ERR_THRESHOLD     ((DETECTION_GPR2 * 32) + 0x12)
#define  W_PAL_EVERY_NUM_FRAMES         ((DETECTION_GPR2 * 32) + 0x14)
#define  W_FMFM_DETECT_CONFID                 ((DETECTION_GPR2 * 32) + 0x16)
#define  W_ENFORCED_MODE                 ((DETECTION_GPR2 * 32) + 0x18)
#define  B_MANUAL_OUTPUT_MATRIX                    ((DETECTION_GPR2 * 32) + 0x1A) 
#define  B_MANUAL_MTS_SW                           ((DETECTION_GPR2 * 32) + 0x1B)  
#define  B_NON_EU_FM_MODE                           ((DETECTION_GPR2 * 32) + 0x1C)  
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
#define  B_MANUAL_OUTPUT_MATRIX_DEC2                    ((DETECTION_GPR2 * 32) + 0x1D) 
#define  W_MTS_OUTPUT_LEVEL_GAIN               ((DETECTION_GPR2 * 32) + 0x1E) 
#else
#define  B_FM_BAD_SIGNAL                       ((DETECTION_GPR2 * 32) + 0x1D)  
#define  B_MANUAL_OUTPUT_MATRIX_DEC2                    ((DETECTION_GPR2 * 32) + 0x1E) 
#endif

/********************** SRS WOW ***********************/
#define  DSP_SRS_WOW_GRP                40
#define  D_SRS_WOW_SPKSIZE              ((DSP_SRS_WOW_GRP * 32) + 0x00)
#define  D_SRS_WOW_ENABLE               ((DSP_SRS_WOW_GRP * 32) + 0x04)
#define  D_SRS_WOW_LMENB                ((DSP_SRS_WOW_GRP * 32) + 0x08)
#define  D_SRS_WOW_BRIGHT               ((DSP_SRS_WOW_GRP * 32) + 0x0C)
#define  D_SRS_WOW_WOWCTRL              ((DSP_SRS_WOW_GRP * 32) + 0x10)
#define  D_SRS_WOW_TBCTRL               ((DSP_SRS_WOW_GRP * 32) + 0x14)
#define  D_SRS_WOW_LMCTRL               ((DSP_SRS_WOW_GRP * 32) + 0x18)

/************** SRS TruSurround-XT ********************/
#define  DSP_SRS_TSXT_GRP               41
//bit  0~ 3: 	Bypass (0: No, 1: Yes)
//bit  4~ 7: 	TS Disable (0: No, 1: Yes)
//bit  8~11: 	TS Headphone Mode (0: No, 1: Yes)
//bit 12~15: 	Input Mode  (0~12)
//bit 16~19:	FOCUS Enable (0: No, 1: Yes)
//bit 20~23:	TruBass Enable (0: No, 1: Yes)
//bit 24~27:	TruBass Speaker Size (0~7)
//bit 28~31:	reserved
#define  D_SRS_TSXT_FLAGS               ((DSP_SRS_TSXT_GRP * 32) + 0x00) // 4 bytes
#define  D_SRS_TSXT_FC_Elevation        ((DSP_SRS_TSXT_GRP * 32) + 0x04) // 4 bytes
#define  D_SRS_TSXT_TB_TBCtrl           ((DSP_SRS_TSXT_GRP * 32) + 0x08) // 4 bytes
#define  D_SRS_TSXT_Input_gain          ((DSP_SRS_TSXT_GRP * 32) + 0x0C) // 4 bytes

#define  D_PLAY_SPEED				((DSP_SRS_TSXT_GRP * 32) + 0x10) // 4 bytes //sunman for play speed


/************** NPTV Demodulation Group 3 ********************/
#define  DSP_DEM_FINE_TUNE_VOLUME_GRP1   42
#define  D_PAL_FINE_TUNE_VOLUME                     ((DSP_DEM_FINE_TUNE_VOLUME_GRP1 * 32) + 0x00)
#define  D_NICAM_FINE_TUNE_VOLUME                     ((DSP_DEM_FINE_TUNE_VOLUME_GRP1 * 32) + 0x04)
#define  D_AM_FINE_TUNE_VOLUME                     ((DSP_DEM_FINE_TUNE_VOLUME_GRP1 * 32) + 0x08)
#define  D_A2_FINE_TUNE_VOLUME                     ((DSP_DEM_FINE_TUNE_VOLUME_GRP1 * 32) + 0x0C)  //ADSPREVxxx
#define  D_MTS_FINE_TUNE_VOLUME                     ((DSP_DEM_FINE_TUNE_VOLUME_GRP1 * 32) + 0x10)
#define  D_SAP_FINE_TUNE_VOLUME                     ((DSP_DEM_FINE_TUNE_VOLUME_GRP1 * 32) + 0x14)  //ADSPREVxxx
#define  D_FMFM_MONO_FINE_TUNE_VOLUME                     ((DSP_DEM_FINE_TUNE_VOLUME_GRP1 * 32) + 0x18)  //ADSPREVxxx
#define  D_FMFM_DUAL_FINE_TUNE_VOLUME                     ((DSP_DEM_FINE_TUNE_VOLUME_GRP1 * 32) + 0x1C)  //ADSPREVxxx

/************** NPTV Demodulation Group 5********************/
#define  DETECTION_GPR3                 43
#define  B_FM_MUTE_MODE                              ((DETECTION_GPR3 * 32) + 0x00)
#define  B_FM_MUTE_THRESHOLD_HIGHT                 ((DETECTION_GPR3 * 32) + 0x01)
#define  B_FM_MUTE_THRESHOLD_LOW                 ((DETECTION_GPR3 * 32) + 0x02)
#define  B_FM_MUTE_THRESHOLD_HIGHT_PAL                 ((DETECTION_GPR3 * 32) + 0x03)
#define  B_FM_MUTE_THRESHOLD_LOW_PAL                 ((DETECTION_GPR3 * 32) + 0x04)
#define  B_FM_MUTE_THRESHOLD_HIGHT_A2                 ((DETECTION_GPR3 * 32) + 0x05)
#define  B_FM_MUTE_THRESHOLD_LOW_A2                 ((DETECTION_GPR3 * 32) + 0x06)
#define  B_FM_MUTE_THRESHOLD_HIGHT_MTS                 ((DETECTION_GPR3 * 32) + 0x07)
#define  B_FM_MUTE_THRESHOLD_LOW_MTS                 ((DETECTION_GPR3 * 32) + 0x08)
#define  B_FM_MUTE_THRESHOLD_HIGHT_EIAJ                 ((DETECTION_GPR3 * 32) + 0x09)
#define  B_FM_MUTE_THRESHOLD_LOW_EIAJ                 ((DETECTION_GPR3 * 32) + 0x0A)
#define  W_MTS_NUM_PILOT                          ((DETECTION_GPR3 * 32) + 0x0C) 
#define  W_MTS_NUM_STEREO                         ((DETECTION_GPR3 * 32) + 0x0E)
#define  W_MTS_NUM_SAP                         ((DETECTION_GPR3 * 32) + 0x10)
#define  B_MTS_PILOT_OFFSET_DETECTION                         ((DETECTION_GPR3 * 32) + 0x12)
#define  B_MTS_MUTE_SAP_LOW                         ((DETECTION_GPR3 * 32) + 0x13)
#define  B_MTS_MUTE_SAP_HIGH                        ((DETECTION_GPR3 * 32) + 0x14)
#define  B_SATU_MUTE_THRESHOLD_HIGH      ((DETECTION_GPR3 * 32) + 0x15)
#define  B_SATU_MUTE_THRESHOLD_LOW        ((DETECTION_GPR3 * 32) + 0x16)
#define  B_DETECTION_GPR3_RESERVED1          ((DETECTION_GPR3 * 32) + 0x17)
#define  W_A2_MONO_STEREO_THRES_M         ((DETECTION_GPR3 * 32) + 0x18)
#define  W_A2_STEREO_MONO_THRES_M         ((DETECTION_GPR3 * 32) + 0x1a)
#define  B_SAP_HP_MUTE_LOW                       ((DETECTION_GPR3 * 32) + 0x1c)
#define  B_SAP_HP_MUTE_HIGH                       ((DETECTION_GPR3 * 32) + 0x1d)
#define  B_SAP_FILTER_SEL                            ((DETECTION_GPR3 * 32) + 0x1e)
#define  B_SAP_RATIO                                     ((DETECTION_GPR3 * 32) + 0x1f)  
/************** BBE for first decoder ********************/
#define   DSPBBE_GRP1                   44

// OFF(0) / BBE(1) / BBE ViVA4(2)
#define   B_BBE_FLAG                              ((DSPBBE_GRP1 * 32) + 0x00)
// Level: 0 ~ 6 (-9 ~ -3)
#define   B_BBE_LEVEL                             ((DSPBBE_GRP1 * 32) + 0x01) 
// BBE boost: 3 ~ 9
#define   B_BBE_PROCESS                           ((DSPBBE_GRP1 * 32) + 0x02)
#define   B_BBE_LO_CONTOUR                        ((DSPBBE_GRP1 * 32) + 0x03)
// 3D gain: 0 ~ 101
#define   B_BBE_3D_GAIN                           ((DSPBBE_GRP1 * 32) + 0x04)

/************** NEW MTK SURROUND for first decoder ********************/
#define   DSP3D_GRP5                   45
#define  D_VSURR_CROS_TALK                ((DSP3D_GRP5 * 32) + 0x00)
#define  D_VSURR_WIDTH                ((DSP3D_GRP5 * 32) + 0x04)
#define  D_VSURR_LR_GAIN                ((DSP3D_GRP5 * 32) + 0x08)
#define  D_VSURR_CLARITY                ((DSP3D_GRP5 * 32) + 0x0C)
#define  D_VSURR_FO                ((DSP3D_GRP5 * 32) + 0x10)
#define  D_VSURR_BASS_GAIN                ((DSP3D_GRP5 * 32) + 0x14)
#define  D_VSURR_OUTPUT_GAIN                ((DSP3D_GRP5 * 32) + 0x18)
//-----------------

/************** Decoder option for first decoder ********************/
#define   DEC_GRP3                      46
// Light: for PCM pre-buffer
#define   W_PCM_PREBUF_DELAY_BANK      ((DEC_GRP3 * 32) + 0x00)
#define   W_PCM_PREBUF_DELAY_BANK_DEC2 ((DEC_GRP3 * 32) + 0x02)
#ifdef DATA_DISC_WMA_SUPPORT
    /***************  WMA  ******************/
    #define  DSP_WMA_GRP                    47
//    #define  W_WMA_PACKET_NO                ((DSP_WMA_GRP * 32) + 0x0) //wma_packet_no //W_WMAPACKET 0:if not mps
    #define  W_WMA_HEADER_SOURCE            ((DSP_WMA_GRP * 32) + 0x2) //wma_header_source 0: if not mps
    #define  D_WMA_PACKET_COUNT             ((DSP_WMA_GRP * 32) + 0x4) //wma_packet_count
    #define  D_WMA_PACKET_SIZE              ((DSP_WMA_GRP * 32) + 0x8) //packet_size
    #define  W_WMA_VERSION                  ((DSP_WMA_GRP * 32) + 0xC) 
    #define  W_WMA_NUM_CH                   ((DSP_WMA_GRP * 32) + 0xE)//num_channels
    #define  D_WMA_SAMPLE_PER_SEC           ((DSP_WMA_GRP * 32) + 0x10)//samples_per_second
    #define  D_WMA_BYTE_PER_SEC             ((DSP_WMA_GRP * 32) + 0x14)//bytes_per_second
    #define  D_WMA_BLOCK_SIZE               ((DSP_WMA_GRP * 32) + 0x18)//block_size dword
    #define  W_WMA_ENCODER_OPTION           ((DSP_WMA_GRP * 32) + 0x1C)//encoder_options word
#endif


/************** Decoder option LPCM decoder ********************/
#define   LPCM_GRP1                      47
// LPCM decoder
#define   B_LPCM_CH_ASSIGNMENT  ((LPCM_GRP1 * 32) + 0x00)
#define   B_LPCM_BIT_SHIFT              ((LPCM_GRP1 * 32) + 0x01)
#define   B_LPCM_DRC_VALUE              ((LPCM_GRP1 * 32) + 0x02)
#define   B_LPCM_DRC_FLAG               ((LPCM_GRP1 * 32) + 0x03)
#define   W_LPCM_Q_VALUE                ((LPCM_GRP1 * 32) + 0x04)
#define   W_LPCM_FREQUENCY              ((LPCM_GRP1 * 32) + 0x06)
#define   W_LPCM_DECODING_TYPE  ((LPCM_GRP1 * 32) + 0x08)
#define   B_ADPCM_CHANNEL_NUM           ((LPCM_GRP1 * 32) + 0x0A)
#define   B_ADPCM_BIT_PERSAMPLE         ((LPCM_GRP1 * 32) + 0x0B)
#define   W_ADPCM_DECODING_TYPE         ((LPCM_GRP1 * 32) + 0x0C)
#define   W_ADPCM_BLOCK_ALIGN           ((LPCM_GRP1 * 32) + 0x0E)

#define   B_LPCM_CH_ASSIGNMENT_DEC2     ((LPCM_GRP1 * 32) + 0x10)
#define   B_LPCM_BIT_SHIFT_DEC2         ((LPCM_GRP1 * 32) + 0x11)
#define   B_LPCM_DRC_VALUE_DEC2         ((LPCM_GRP1 * 32) + 0x12)
#define   B_LPCM_DRC_FLAG_DEC2          ((LPCM_GRP1 * 32) + 0x13)
#define   W_LPCM_Q_VALUE_DEC2           ((LPCM_GRP1 * 32) + 0x14)
#define   W_LPCM_FREQUENCY_DEC2         ((LPCM_GRP1 * 32) + 0x16)
#define   W_LPCM_DECODING_TYPE_DEC2     ((LPCM_GRP1 * 32) + 0x18)
#define   B_ADPCM_CHANNEL_NUM_DEC2      ((LPCM_GRP1 * 32) + 0x1A)
#define   B_ADPCM_BIT_PERSAMPLE_DEC2    ((LPCM_GRP1 * 32) + 0x1B)
#define   W_ADPCM_DECODING_TYPE_DEC2    ((LPCM_GRP1 * 32) + 0x1C)
#define   W_ADPCM_BLOCK_ALIGN_DEC2      ((LPCM_GRP1 * 32) + 0x1E)


/************** Post processing option  ****************************/
#define   POST_PROC_GRP              48

#define   B_HPF_ENABLE               ((POST_PROC_GRP * 32) + 0x00)
#define   W_HPF_FC                   ((POST_PROC_GRP * 32) + 0x02)
#define   B_LIMITER_ENABLE           ((POST_PROC_GRP * 32) + 0x04)
#define   D_LIMITER_FLAG             ((POST_PROC_GRP * 32) + 0x08)
#define   B_PEQ_ENABLE               ((POST_PROC_GRP * 32) + 0x14)

#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
#define   B_SPH_ENABLE               ((POST_PROC_GRP * 32) + 0x15)
#else
#define   B_SPH_MODE              ((POST_PROC_GRP * 32) + 0x15)
#endif
#endif
#ifdef CC_AUD_LINEOUT_VOL_MODE
#define   B_LINEOUT_VOL_MODE              ((POST_PROC_GRP * 32) + 0x16)
#endif
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
#define   D_MTS_OUTPUT_LEVEL_GAIN     ((POST_PROC_GRP * 32) + 0x18)
#endif
/************** PEQ option******************************************/
#define   PEQ_GRP                    49

#define   D_PEQ_SET0_GAIN            ((PEQ_GRP * 32) + 0x00)
#define   W_PEQ_SET0_FC              ((PEQ_GRP * 32) + 0x04)
#define   W_PEQ_SET0_BW              ((PEQ_GRP * 32) + 0x06)
#define   D_PEQ_SET1_GAIN            ((PEQ_GRP * 32) + 0x08)
#define   W_PEQ_SET1_FC              ((PEQ_GRP * 32) + 0x0C)
#define   W_PEQ_SET1_BW              ((PEQ_GRP * 32) + 0x0E)
#define   D_PEQ_SET2_GAIN            ((PEQ_GRP * 32) + 0x10)
#define   W_PEQ_SET2_FC              ((PEQ_GRP * 32) + 0x14)
#define   W_PEQ_SET2_BW              ((PEQ_GRP * 32) + 0x16)
#define   D_PEQ_SET3_GAIN            ((PEQ_GRP * 32) + 0x18)
#define   W_PEQ_SET3_FC              ((PEQ_GRP * 32) + 0x1C)
#define   W_PEQ_SET3_BW              ((PEQ_GRP * 32) + 0x1E)

/************** PEQ option******************************************/
#define   PEQ_GRP2                   50

#define   D_PEQ_SET4_GAIN            ((PEQ_GRP2 * 32) + 0x00)
#define   W_PEQ_SET4_FC              ((PEQ_GRP2 * 32) + 0x04)
#define   W_PEQ_SET4_BW              ((PEQ_GRP2 * 32) + 0x06)
#define   D_PEQ_SET5_GAIN            ((PEQ_GRP2 * 32) + 0x08)
#define   W_PEQ_SET5_FC              ((PEQ_GRP2 * 32) + 0x0C)
#define   W_PEQ_SET5_BW              ((PEQ_GRP2 * 32) + 0x0E)
#define   D_PEQ_SET6_GAIN            ((PEQ_GRP2 * 32) + 0x10)
#define   W_PEQ_SET6_FC              ((PEQ_GRP2 * 32) + 0x14)
#define   W_PEQ_SET6_BW              ((PEQ_GRP2 * 32) + 0x16)
#define   D_PEQ_SET7_GAIN            ((PEQ_GRP2 * 32) + 0x18)
#define   W_PEQ_SET7_FC              ((PEQ_GRP2 * 32) + 0x1C)
#define   W_PEQ_SET7_BW              ((PEQ_GRP2 * 32) + 0x1E)

/************** Virtual Bass option*********************************/
#define   VIR_BASS_GRP               51

#define   D_PEQ_SET8_GAIN            ((VIR_BASS_GRP * 32) + 0x00)
#define   W_PEQ_SET8_FC              ((VIR_BASS_GRP * 32) + 0x04)
#define   W_PEQ_SET8_BW              ((VIR_BASS_GRP * 32) + 0x06)
#define   B_VIR_BASS_ENABLE          ((VIR_BASS_GRP * 32) + 0x08)
#define   D_VIR_BASS_GAIN            ((VIR_BASS_GRP * 32) + 0x0C)
#define   D_VIR_BASS_GAIN_2          ((VIR_BASS_GRP * 32) + 0x10)

/************** Clipper value **************************************/
#define   CLIPPER_GRP                52

#define   B_CLIPPER_ENABLE           ((CLIPPER_GRP * 32) + 0x00)
#define   D_CLIPPER_C_VALUE          ((CLIPPER_GRP * 32) + 0x04)
#define   D_CLIPPER_L_VALUE          ((CLIPPER_GRP * 32) + 0x08)
#define   D_CLIPPER_R_VALUE          ((CLIPPER_GRP * 32) + 0x0C)
#define   D_CLIPPER_LS_VALUE         ((CLIPPER_GRP * 32) + 0x10)
#define   D_CLIPPER_RS_VALUE         ((CLIPPER_GRP * 32) + 0x14)
#define   D_CLIPPER_SW_VALUE         ((CLIPPER_GRP * 32) + 0x18)
#define   D_CLIPPER_CH7_VALUE        ((CLIPPER_GRP * 32) + 0x1C)

/************** Clipper value **************************************/
#define   CLIPPER_GRP_2              53

#define   D_CLIPPER_CH8_VALUE        ((CLIPPER_GRP_2 * 32) + 0x00)
#define   D_CLIPPER_CH9_VALUE        ((CLIPPER_GRP_2 * 32) + 0x04)
#define   D_CLIPPER_CH10_VALUE       ((CLIPPER_GRP_2 * 32) + 0x08)
/************** FM radio related ***********************************/
#define   FM_RADIO_GRP              54
#define   D_FM_RDO_INBAND_TH        ((FM_RADIO_GRP * 32) + 0x00) 
#define   D_FM_RDO_NOISE_TH           ((FM_RADIO_GRP * 32) + 0x04) 

/************** SRS TruSurround-HD ********************/
//sunman_tshd
#define  DSP_SRS_TSHD_GRP               55

#define	D_SRS_TSHD_FLAGS               	((DSP_SRS_TSHD_GRP * 32) + 0x00) // 4 bytes
#define	D_SRS_TSHD_FC_Elevation	((DSP_SRS_TSHD_GRP * 32) + 0x04) // 4 bytes
#define	D_SRS_TSHD_TB_TBCtrl           ((DSP_SRS_TSHD_GRP * 32) + 0x08) // 4 bytes
#define	D_SRS_TSHD_DefCtrl			((DSP_SRS_TSHD_GRP * 32) + 0x0c) // 4 bytes
#define	D_SRS_TSHD_LimiterCtrl		((DSP_SRS_TSHD_GRP * 32) + 0x10) // 4 bytes
#define	B_SRS_TSHD_SPKSIZE		((DSP_SRS_TSHD_GRP * 32) + 0x14) // 1 bytes
#define	B_SRS_TSHD_SUBSPKSIZE	((DSP_SRS_TSHD_GRP * 32) + 0x15) // 1 bytes
#define	B_SRS_TSHD_COFREQ		((DSP_SRS_TSHD_GRP * 32) + 0x16) // 1 bytes

#define	B_SRS_TSHD_INP_TYPE		((DSP_SRS_TSHD_GRP * 32) + 0x17) // 1 bytes
#define	B_SRS_TSHD_CH_DEST		((DSP_SRS_TSHD_GRP * 32) + 0x18) // 1 bytes

/************** SRS TruSurround-HD ********************/
#define  DSP_SRS_TSHD_GRP1               56


#define	D_SRS_TSHD_Center_Ctrl		((DSP_SRS_TSHD_GRP1 * 32) + 0x00) // 4 bytes
#define	D_SRS_TSHD_Space_Ctrl		((DSP_SRS_TSHD_GRP1 * 32) + 0x04) // 4 bytes
#define	D_SRS_TSHD_Surr_Level		((DSP_SRS_TSHD_GRP1 * 32) + 0x08) // 4 bytes
#define	D_SRS_TSHD_WOW_FC_ctrl	((DSP_SRS_TSHD_GRP1 * 32) + 0x0c) // 4 bytes
#define	D_SRS_TSHD_INPUT_GAIN	((DSP_SRS_TSHD_GRP1 * 32) + 0x10) // 4 bytes
#define	D_SRS_TSHD_OUTPUT_GAIN	((DSP_SRS_TSHD_GRP1 * 32) + 0x14) // 4 bytes


/************** JPEG decoder ********************/
/// #ifdef ADSP_JPEG_DEC2
///_u4ShareMem = 3CEC100						
/// + 32*57 + 0x0 = 0x3CEC820

#define   DSP_JPEG_GRP                      57
#define   D_JPG_MAX_COMPACT_ADDR        ((DSP_JPEG_GRP * 32) + 0x0)
#define   D_JPG_LENGTH                  ((DSP_JPEG_GRP * 32) + 0x4)
#define   W_JPG_CMD_FLAG                ((DSP_JPEG_GRP * 32) + 0x8)
#define   W_JPG_MAX_WIDTH           ((DSP_JPEG_GRP * 32) + 0xA)
#define   B_JPG_MAX_SCAN_NUM            ((DSP_JPEG_GRP * 32) + 0xC)


/************** Decoder option for first decoder ********************/

#define   DSP_DEBUG_GRP                (DSP_GRP_MAX - 1)

#define   B_DSP_DEBUG_1                ((DSP_DEBUG_GRP * 32) + 0x00)

/**************** emulation only *************************************/
#define   DSPSPECTRUM_GRP0             (DSP_GRP_MAX+0)

#define   W_SPEC0                      ((DSPSPECTRUM_GRP0 * 32) + 0x00) // RISC
#define   W_SPEC1                      ((DSPSPECTRUM_GRP0 * 32) + 0x02) // RISC
#define   W_SPEC2                      ((DSPSPECTRUM_GRP0 * 32) + 0x04) // RISC
#define   W_SPEC3                      ((DSPSPECTRUM_GRP0 * 32) + 0x06) // RISC
#define   W_SPEC4                      ((DSPSPECTRUM_GRP0 * 32) + 0x08) // RISC
#define   W_SPEC5                      ((DSPSPECTRUM_GRP0 * 32) + 0x0A) // RISC
#define   W_SPEC6                      ((DSPSPECTRUM_GRP0 * 32) + 0x0C) // RISC
#define   W_SPEC7                      ((DSPSPECTRUM_GRP0 * 32) + 0x0E) // RISC
#define   W_SPEC8                      ((DSPSPECTRUM_GRP1 * 32) + 0x10) // RISC
#define   W_SPEC9                      ((DSPSPECTRUM_GRP1 * 32) + 0x12) // RISC
#define   W_SPEC10                     ((DSPSPECTRUM_GRP1 * 32) + 0x14) // RISC
#define   W_SPEC11                     ((DSPSPECTRUM_GRP1 * 32) + 0x16) // RISC
#define   W_SPEC12                     ((DSPSPECTRUM_GRP1 * 32) + 0x18) // RISC
#define   W_SPEC13                     ((DSPSPECTRUM_GRP1 * 32) + 0x1A) // RISC
#define   W_SPEC14                     ((DSPSPECTRUM_GRP1 * 32) + 0x1C) // RISC
#define   W_SPEC15                     ((DSPSPECTRUM_GRP1 * 32) + 0x1E) // RISC

#define   DSPSPECTRUM_GRP1             (DSP_GRP_MAX+1)

#define   W_SPEC16                     ((DSPSPECTRUM_GRP1 * 32) + 0x00) // RISC
#define   W_SPEC17                     ((DSPSPECTRUM_GRP1 * 32) + 0x02) // RISC
#define   W_SPEC18                     ((DSPSPECTRUM_GRP1 * 32) + 0x04) // RISC
#define   W_SPEC19                     ((DSPSPECTRUM_GRP1 * 32) + 0x06) // RISC
#define   W_SPEC20                     ((DSPSPECTRUM_GRP1 * 32) + 0x08) // RISC
#define   W_SPEC21                     ((DSPSPECTRUM_GRP1 * 32) + 0x0A) // RISC
#define   W_SPEC22                     ((DSPSPECTRUM_GRP1 * 32) + 0x0C) // RISC
#define   W_SPEC23                     ((DSPSPECTRUM_GRP1 * 32) + 0x0E) // RISC
#define   W_SPEC24                     ((DSPSPECTRUM_GRP3 * 32) + 0x10) // RISC
#define   W_SPEC25                     ((DSPSPECTRUM_GRP3 * 32) + 0x12) // RISC
#define   W_SPEC26                     ((DSPSPECTRUM_GRP3 * 32) + 0x14) // RISC
#define   W_SPEC27                     ((DSPSPECTRUM_GRP3 * 32) + 0x16) // RISC
#define   W_SPEC28                     ((DSPSPECTRUM_GRP3 * 32) + 0x18) // RISC
#define   W_SPEC29                     ((DSPSPECTRUM_GRP3 * 32) + 0x1A) // RISC
#define   W_SPEC30                     ((DSPSPECTRUM_GRP3 * 32) + 0x1C) // RISC
#define   W_SPEC31                     ((DSPSPECTRUM_GRP3 * 32) + 0x1E) // RISC

#define   DSP_EMU_GRP1                 (DSP_GRP_MAX+2)

#define   D_STC                        ((DSP_EMU_GRP1 * 32) + 0x00) // RISC
#define   D_PTS_SIZE                   ((DSP_EMU_GRP1 * 32) + 0x04) // RISC
#define   W_STC_SETTING                ((DSP_EMU_GRP1 * 32) + 0x08) // RISC
#define   D_STC_DEC2                   ((DSP_EMU_GRP1 * 32) + 0x0A) // RISC
#define   D_PTS_SIZE_DEC2              ((DSP_EMU_GRP1 * 32) + 0x0E) // RISC
#define   W_STC_SETTING_DEC2           ((DSP_EMU_GRP1 * 32) + 0x12) // RISC

#define   DSP_CH_VOL_GRP               (DSP_GRP_MAX+3)

#define   D_CH_L_VOL                   ((DSP_CH_VOL_GRP * 32) + 0x00)   // RISC
#define   D_CH_R_VOL                   ((DSP_CH_VOL_GRP * 32) + 0x04)   // RISC
#define   D_CH_LS_VOL                  ((DSP_CH_VOL_GRP * 32) + 0x08)   // RISC
#define   D_CH_RS_VOL                  ((DSP_CH_VOL_GRP * 32) + 0x0C)   // RISC
#define   D_CH_C_VOL                   ((DSP_CH_VOL_GRP * 32) + 0x10)   // RISC
#define   D_CH_SUB_VOL                 ((DSP_CH_VOL_GRP * 32) + 0x14)   // RISC
#define   D_CH7_VOL                    ((DSP_CH_VOL_GRP * 32) + 0x18)   // RISC
#define   D_CH8_VOL                    ((DSP_CH_VOL_GRP * 32) + 0x1c)   // RISC

#define   DSP_CH_VOL_GRP2              (DSP_GRP_MAX+4)
#define   D_CH9_VOL                    ((DSP_CH_VOL_GRP2 * 32) + 0x00)  // RISC
#define   D_CH10_VOL                   ((DSP_CH_VOL_GRP2 * 32) + 0x04)  // RISC
#define   D_CH_L_VOL_DEC2              ((DSP_CH_VOL_GRP2 * 32) + 0x08)  // RISC
#define   D_CH_R_VOL_DEC2              ((DSP_CH_VOL_GRP2 * 32) + 0x0C)  // RISC
#define   D_CH_VOLUME_TOTAL            ((DSP_CH_VOL_GRP2 * 32) + 0x10)  // RISC
#define   D_CH_VOLUME_TOTAL_DEC2       ((DSP_CH_VOL_GRP2 * 32) + 0x14)  // RISC
/*****************************************************************/
#endif // DSP_SHM_H

