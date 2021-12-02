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
 * $Date: 2012/06/28 $
 * $RCSfile: d2rc_shm.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file d2rc_shm.h
 *  Brief of file d2rc_shm.h.
 *  Details of file d2rc_shm.h (optional).
 */

#ifdef AUD_EXTERN_FILE
#error "Please don't include this header file in release C file"
#endif

#ifndef D2RC_SHM_H
#define D2RC_SHM_H

#include "ctrl_para.h"

//===================================================================
// ADDR_D2RC_SHARED_INFO Section (08000h~08100h, 100h)
//===================================================================
#define ADDR_D2RC_BASIC_INFO_BASE               (ADDR_D2RC_SHARED_INFO + 0x0000)
#define ADDR_D2RC_VOLUME_BASE                   (ADDR_D2RC_SHARED_INFO + 0x0010)
#define ADDR_D2RC_AUDIO_INFO_BASE               (ADDR_D2RC_SHARED_INFO + 0x0020)
#define ADDR_D2RC_POST_INFO_BASE                (ADDR_D2RC_SHARED_INFO + 0x0030)
#define ADDR_D2RC_MISC_INFO_BASE                (ADDR_D2RC_SHARED_INFO + 0x0040)
#define ADDR_D2RC_DECODING_INFO_BASE            (ADDR_D2RC_SHARED_INFO + 0x0050)
#define ADDR_D2RC_DECODING_INFO_BASE_DEC2       (ADDR_D2RC_SHARED_INFO + 0x0060)
#define ADDR_D2RC_TRNCODING_INFO_BASE           (ADDR_D2RC_SHARED_INFO + 0x0070)
#define ADDR_D2RC_SPECTRUM_BASE                 (ADDR_D2RC_SHARED_INFO + 0x0080)
#define ADDR_D2RC_AUDIO_INFO_BASE_DEC2          (ADDR_D2RC_SHARED_INFO + 0x00A0)
#define ADDR_D2RC_POST_INFO_BASE_DEC2           (ADDR_D2RC_SHARED_INFO + 0x00B0)
#define ADDR_D2RC_COMMON_LOG                    (ADDR_D2RC_SHARED_INFO + 0x00C0)
#define ADDR_D2RC_DECODER_LOG                   (ADDR_D2RC_SHARED_INFO + 0x00D0)
#define ADDR_D2RC_DECODER_LOG_DEC2              (ADDR_D2RC_SHARED_INFO + 0x00E8)

//===================================================================
// ADDR_D2RC_SHARED_INFO_DEC3 Section (08200h~08280h, 80h)
//===================================================================
#define ADDR_D2RC_DECODING_INFO_BASE_DEC3       (ADDR_D2RC_SHARED_INFO_DEC3 + 0x0000)
#define ADDR_D2RC_AUDIO_INFO_BASE_DEC3          (ADDR_D2RC_SHARED_INFO_DEC3 + 0x0010)
#define ADDR_D2RC_POST_INFO_BASE_DEC3           (ADDR_D2RC_SHARED_INFO_DEC3 + 0x0020)
#define ADDR_D2RC_DECODER_LOG_DEC3              (ADDR_D2RC_SHARED_INFO_DEC3 + 0x0028)

#define ADDR_D2RC_DECODING_INFO_BASE_DEC4       (ADDR_D2RC_SHARED_INFO_DEC3 + 0x0040)
#define ADDR_D2RC_AUDIO_INFO_BASE_DEC4          (ADDR_D2RC_SHARED_INFO_DEC3 + 0x0050)
#define ADDR_D2RC_POST_INFO_BASE_DEC4           (ADDR_D2RC_SHARED_INFO_DEC3 + 0x0060)
#define ADDR_D2RC_DECODER_LOG_DEC4              (ADDR_D2RC_SHARED_INFO_DEC3 + 0x0068)

//===================================================================
// ADDR_D2RC_BASIC_INFO_BASE Section (08000h~08010h, 10h)
// Basic Info
//===================================================================
#define ADDR_D2RC_ROM_CODE_VERSION              (ADDR_D2RC_BASIC_INFO_BASE + 0x0000)
//RomCode Version Code
#define ADDR_D2RC_COMMON_CODE_VERSOIN           (ADDR_D2RC_BASIC_INFO_BASE + 0x0001)
//Common RamCode Version Code
#define ADDR_D2RC_RAM_CODE_VERSION              (ADDR_D2RC_BASIC_INFO_BASE + 0x0002)
//Main Audio Decoder Version Code
#define ADDR_D2RC_DEC2RAM_CODE_VERSION          (ADDR_D2RC_BASIC_INFO_BASE + 0x0003)
//Aux Audio Decoder Version Code
#define ADDR_D2RC_DEC3RAM_CODE_VERSION          (ADDR_D2RC_BASIC_INFO_BASE + 0x0004)
// 3rd Audio Decoder Version Code
#define ADDR_RISC_CODE_VERSION                  (ADDR_D2RC_BASIC_INFO_BASE + 0x0005)
#define ADDR_RISC_FLASH_VERSION                 (ADDR_D2RC_BASIC_INFO_BASE + 0x0006)
#define ADDR_D2RC_VSURR_CODE_VERSION            (ADDR_D2RC_BASIC_INFO_BASE + 0x0007)
#define ADDR_D2RC_UPMIX_CODE_VERSION            (ADDR_D2RC_BASIC_INFO_BASE + 0x0008)
//#define ADDR_D2RC_BASIC_INFO_09H              (ADDR_D2RC_BASIC_INFO_BASE + 0x0009)
//#define ADDR_D2RC_BASIC_INFO_0AH              (ADDR_D2RC_BASIC_INFO_BASE + 0x000A)
//#define ADDR_D2RC_BASIC_INFO_0BH              (ADDR_D2RC_BASIC_INFO_BASE + 0x000B)
//#define ADDR_D2RC_BASIC_INFO_09H              (ADDR_D2RC_BASIC_INFO_BASE + 0x000C)
//#define ADDR_D2RC_BASIC_INFO_09H              (ADDR_D2RC_BASIC_INFO_BASE + 0x000D)
//#define ADDR_D2RC_BASIC_INFO_09H              (ADDR_D2RC_BASIC_INFO_BASE + 0x000E)
//#define ADDR_D2RC_BASIC_INFO_09H              (ADDR_D2RC_BASIC_INFO_BASE + 0x000F)

//===================================================================
// ADDR_D2RC_VOLUME_BASE Section (08010h~08020h, 10h)
// Volume Info
//===================================================================
#define ADDR_D2RC_VOLUME_TOTAL                  (ADDR_D2RC_VOLUME_BASE + 0)
#define ADDR_D2RC_CH_VOLUME_L                   (ADDR_D2RC_VOLUME_BASE + 1)
#define ADDR_D2RC_CH_VOLUME_R                   (ADDR_D2RC_CH_VOLUME_L + 1)
#define ADDR_D2RC_CH_VOLUME_LS                  (ADDR_D2RC_CH_VOLUME_R + 1)
#define ADDR_D2RC_CH_VOLUME_RS                  (ADDR_D2RC_CH_VOLUME_LS + 1)
#define ADDR_D2RC_CH_VOLUME_C                   (ADDR_D2RC_CH_VOLUME_RS + 1)
#define ADDR_D2RC_CH_VOLUME_SUBWOOFER           (ADDR_D2RC_CH_VOLUME_C + 1)
#define ADDR_D2RC_CH_VOLUME_CH7                 (ADDR_D2RC_CH_VOLUME_SUBWOOFER + 1)
#define ADDR_D2RC_CH_VOLUME_CH8                 (ADDR_D2RC_CH_VOLUME_CH7 + 1)
#define ADDR_D2RC_CH_VOLUME_CH9                 (ADDR_D2RC_CH_VOLUME_CH8 + 1)
#define ADDR_D2RC_CH_VOLUME_CH10                (ADDR_D2RC_CH_VOLUME_CH9 + 1)
#define ADDR_D2RC_VOLUME_TOTAL_DEC2             (ADDR_D2RC_VOLUME_BASE + 0xB)
#define ADDR_D2RC_CH_VOLUME_L_DEC2              (ADDR_D2RC_VOLUME_BASE + 0xC)
#define ADDR_D2RC_CH_VOLUME_R_DEC2              (ADDR_D2RC_VOLUME_BASE + 0xD)
#define ADDR_D2RC_CH_VOLUME_L_DEC3              (ADDR_D2RC_VOLUME_BASE + 0xE)
#define ADDR_D2RC_CH_VOLUME_R_DEC3              (ADDR_D2RC_VOLUME_BASE + 0xF)

//===================================================================
// ADDR_D2RC_AUDIO_INFO_BASE Section (08020h~08030h, 10h)
// Audio Information for first decoder
//===================================================================
#define ADDR_D2RC_RISC_INFO_SAMPLING_RATE	    (ADDR_D2RC_AUDIO_INFO_BASE + 0x00)
#define ADDR_D2RC_RISC_INFO_CHANNEL_NUM		    (ADDR_D2RC_AUDIO_INFO_BASE + 0x01)
#define ADDR_D2RC_RISC_INFO_BASS_FLAG1		    (ADDR_D2RC_AUDIO_INFO_BASE + 0x02)
#define ADDR_D2RC_RISC_INFO_BASS_FLAG2		    (ADDR_D2RC_AUDIO_INFO_BASE + 0x03)
#define ADDR_D2RC_RISC_INFO_BASS_FLAG3		    (ADDR_D2RC_AUDIO_INFO_BASE + 0x04)
#define ADDR_D2RC_RISC_INFO_INPUT_CH_CFG	    (ADDR_D2RC_AUDIO_INFO_BASE + 0x05)
#define ADDR_D2RC_INPUT_CH_CFG                  (ADDR_D2RC_RISC_INFO_INPUT_CH_CFG)
//----------------------------------------------------------------------
//[16.8] format
//b000: 2/0 (Dual Mono)
//b001: 1/0
//b010: 2/0
//b011: 3/0
//b100: 2/1
//b101: 3/1
//b110: 2/2
//b111: 3/2
//bit 3: Ch-7 present(1)/absent(0)
//bit 4: Ch-8 present(1)/absent(0)
//bit 5: Subwoofer Channel present(1)/absent(0)
//----------------------------------------------------------------------
//lint -e621 Identifier clash 
#define ADDR_D2RC_RISC_INFO_DECODING_CH_CFG0    (ADDR_D2RC_AUDIO_INFO_BASE + 0x06)
#define ADDR_D2RC_RISC_INFO_DECODING_CH_CFG1    (ADDR_D2RC_AUDIO_INFO_BASE + 0x07)
#define ADDR_D2RC_RISC_INFO_DECODING_CH_NUM     (ADDR_D2RC_AUDIO_INFO_BASE + 0x08)
#define ADDR_D2RC_RISC_INFO_BIT_RATE   	        (ADDR_D2RC_AUDIO_INFO_BASE + 0x09)
//[16.8] format in bps
#define ADDR_D2RC_RISC_INFO_BIT_RESSOLUTION     (ADDR_D2RC_AUDIO_INFO_BASE + 0x0A)
#define ADDR_D2RC_RISC_INFO_INPUT_SAMPLING_RATE (ADDR_D2RC_AUDIO_INFO_BASE + 0x0B)
#define ADDR_D2RC_RISC_INFO_APUT_BANK_DEC1      (ADDR_D2RC_AUDIO_INFO_BASE + 0x0C)
#define ADDR_D2RC_RISC_INFO_APUT_BANK_DEC2      (ADDR_D2RC_AUDIO_INFO_BASE + 0x0D)
#define ADDR_D2RC_RISC_INFO_APUT_BANK_DEC3      (ADDR_D2RC_AUDIO_INFO_BASE + 0x0E)
#define ADDR_D2RC_RISC_INFO_EFFECT_CH_NUM       (ADDR_D2RC_AUDIO_INFO_BASE + 0x0F)

//===================================================================
// ADDR_D2RC_POST_INFO_BASE Section (08030h~08040h, 10h)
// Post Info for first decoder
//===================================================================
#define ADDR_D2RC_RISC_INFO_POST_PROCESSING     (ADDR_D2RC_POST_INFO_BASE + 0x00)
//----------------------------------------------------------------------
//[16.8] format
//Post Processing Information
//RISC_INFO_POST_PROCESSING
//bit 0: Channel Equalizer On/Off
//bit 1: Bass Boost On/Off (Bass)
//bit 2: Super Bass On/Off (Pipe)
//bit 3: Clear Boost On/Off (Treble)
//bit 4: Reverb On/Off
//bit 5: MTK Virtual Surround On/Off
//bit 6: Prologic II On/Off
//bit 7: (Dolby Digital EX On/Off)
//bit 8: DRC On/Off
//bit 9: Karaoke L/R Mixing On/Off
//bit 10: 
//bit 11: 
//bit 12: (Third party Upmix On/Off)
//bit 13: (Third party Equalizer On/Off)
//bit 14: (Third party Headphone On/Off)
//bit 15: (Third party Virtual Surround On/Off)
//----------------------------------------------------------------------
#define ADDR_D2RC_RISC_INFO_POST_PROCESSINGII   (ADDR_D2RC_POST_INFO_BASE + 0x01)
#define ADDR_D2RC_RISC_INFO_VOLUME_NOW          (ADDR_D2RC_POST_INFO_BASE + 0x02)
#define ADDR_D2RC_RISC_INFO_DRC_VALUE_MAX       (ADDR_D2RC_POST_INFO_BASE + 0x03)
#define ADDR_D2RC_DRAM_LOCK                     (ADDR_D2RC_POST_INFO_BASE + 0x04)
    #define REINIT_LOCK                         (0x01)
#define ADDR_D2RC_DRAM_STC_VALID                (ADDR_D2RC_POST_INFO_BASE + 0x05)
#define ADDR_D2RC_DRAM_STC_HIGH                 (ADDR_D2RC_POST_INFO_BASE + 0x06)
#define ADDR_D2RC_DRAM_STC_LOW                  (ADDR_D2RC_POST_INFO_BASE + 0x07)
#define ADDR_D2RC_RISC_INFO_REG_BUF_PNT         (ADDR_D2RC_POST_INFO_BASE + 0x08)

//===================================================================
// ADDR_D2RC_MISC_INFO_BASE Section (08040h~08050h, 10h)
// Misc Info
//===================================================================
#define ADDR_D2RC_RISC_LOG_DUMP_TYPE            (ADDR_D2RC_MISC_INFO_BASE + 0x00)
//----------------------------------------------------------------------
//Log dump Information
//RISC_INFO_LOG_DUMP_TYPE
// 1: stc_diff
// 2: stc_diff_dec2
// 3: stc_adjust_frame
// 4: stc_adjust_frame_dec2
// 5: aout_ok enable 
// 6: aout_ok enable
//----------------------------------------------------------------------
    #define LOG_STC_DIFF                        0x1
    #define LOG_STC_DIFF_DEC2                   0x2
    #define LOG_STC_ADJUST_FRAME                0x3
    #define LOG_STC_ADJUST_FRAME_DEC2           0x4
    #define LOG_AOUT_OK_ENABLE                  0x5
    #define LOG_AOUT_OK_ENABLE_DEC2             0x6
    #define LOG_STC_DIFF_DEC3                   0x7
    #define LOG_STC_ADJUST_FRAME_DEC3           0x8
    
#define ADDR_D2RC_RISC_LOG_DUMP_DATA            (ADDR_D2RC_MISC_INFO_BASE + 0x01)
#define ADDR_D2RC_RISC_LOG_DUMP_DATA_2          (ADDR_D2RC_MISC_INFO_BASE + 0x02)
#define ADDR_D2RC_RISC_LOG_DUMP_DATA_3          (ADDR_D2RC_MISC_INFO_BASE + 0x03)
#define ADDR_D2RC_RISC_LOG_DUMP_DATA_4          (ADDR_D2RC_MISC_INFO_BASE + 0x04)
//#define ADDR_D2RC_RISC_INFO_POST_MIPS         (ADDR_D2RC_MISC_INFO_BASE + 0x00)
//#define ADDR_D2RC_RISC_INFO_TOTAL_MIPS        (ADDR_D2RC_MISC_INFO_BASE + 0x01)

#define ADDR_D2RC_RISC_VORBIS_INT_DEC           (ADDR_D2RC_MISC_INFO_BASE + 0x05)   ///08045h
// 1: Dec1 codebook interrupt
// 2: Dec2 codebook interrupt

//===================================================================
// ADDR_D2RC_DECODING_INFO_BASE Section (08050h~08060h, 10h)
// ADDR_D2RC_DECODING_INFO_BASE_DEC2 Section (08060h~08070h, 10h)
// ADDR_D2RC_DECODING_INFO_BASE_DEC3 Section (08200h~08210h, 10h)
// Decoding Info
//===================================================================
#define DSP_D2RC_INFO_AREA_SZ                   (0x10)
#define ADDR_D2RC_ADC_TUNE_CFG                  (ADDR_D2RC_DECODING_INFO_BASE)
#define ADDR_D2RC_ADC_TUNE_CFG_DEC2             (ADDR_D2RC_DECODING_INFO_BASE_DEC2)
    #define DRAM_ADC_GAIN_POS                   (0x4) // ADC_TUNE_CFG[8:4] GAIN
    #define DRAM_ADC_GAIN_MASK                  (0x1F << DRAM_ADC_GAIN_POS)
    #define DRAM_ADC_OFFSET_POS                 (0xE) // ADC_TUNE_CFG[19:14] OFFSET
    #define DRAM_ADC_OFFSET_MASK                (0x3F << DRAM_ADC_OFFSET_POS)

// AC3
#define ADDR_D2RC_AC3_ACMOD                     (ADDR_D2RC_DECODING_INFO_BASE + 0x0f) // for acmod changed, Light
#define ADDR_D2RC_AC3_DECTYPE                     (ADDR_D2RC_DECODING_INFO_BASE + 0x0e)
#define ADDR_D2RC_AC3_ACMOD_DEC2                (ADDR_D2RC_DECODING_INFO_BASE_DEC2 + 0x0f) // for acmod changed, Light
#define ADDR_D2RC_AC3_DECTYPE_DEC2                (ADDR_D2RC_DECODING_INFO_BASE_DEC2 + 0x0e)

// WMA
#ifdef DATA_DISC_WMA_SUPPORT
#define ADDR_D2RC_WMA_INFO_SEND_TIME            (ADDR_D2RC_DECODING_INFO_BASE)    //WMA_INFO_SEND_TIME, Ian
#define ADDR_D2RC_WMA_ERROR_MODE                (ADDR_D2RC_DECODING_INFO_BASE+1)  //WMA_ERROR_MODE, Ian
#endif    

// MPEG
#define ADDR_D2RC_MPEG_MODE                     (ADDR_D2RC_DECODING_INFO_BASE)
#define ADDR_D2RC_MPEG_BITRATE                  (ADDR_D2RC_DECODING_INFO_BASE+1)
#define ADDR_D2RC_MPEG_SAMPLINGFREQ             (ADDR_D2RC_DECODING_INFO_BASE+2)
#define ADDR_D2RC_MPEG_FRAME_SIZE               (ADDR_D2RC_DECODING_INFO_BASE+3)
#define ADDR_D2RC_MPEG_ID_LAYER                 (ADDR_D2RC_DECODING_INFO_BASE+4)
#define ADDR_D2RC_MPEG_MODE_DEC2                (ADDR_D2RC_DECODING_INFO_BASE_DEC2)
#define ADDR_D2RC_MPEG_BITRATE_DEC2             (ADDR_D2RC_DECODING_INFO_BASE_DEC2+1)
#define ADDR_D2RC_MPEG_SAMPLINGFREQ_DEC2        (ADDR_D2RC_DECODING_INFO_BASE_DEC2+2)
#define ADDR_D2RC_MPEG_FRAME_SIZE_DEC2          (ADDR_D2RC_DECODING_INFO_BASE_DEC2+3)
#define ADDR_D2RC_MPEG_ID_LAYER_DEC2            (ADDR_D2RC_DECODING_INFO_BASE_DEC2+4)
#define ADDR_D2RC_MPEG_MODE_DEC3                (ADDR_D2RC_DECODING_INFO_BASE_DEC3)
#define ADDR_D2RC_MPEG_BITRATE_DEC3             (ADDR_D2RC_DECODING_INFO_BASE_DEC3+1)
#define ADDR_D2RC_MPEG_SAMPLINGFREQ_DEC3        (ADDR_D2RC_DECODING_INFO_BASE_DEC3+2)
#define ADDR_D2RC_MPEG_FRAME_SIZE_DEC3          (ADDR_D2RC_DECODING_INFO_BASE_DEC3+3)
#define ADDR_D2RC_MPEG_ID_LAYER_DEC3            (ADDR_D2RC_DECODING_INFO_BASE_DEC3+4)

// MP3
#define ADDR_D2RC_MP3_VBR                       (ADDR_D2RC_DECODING_INFO_BASE+4)

// AAC
#define ADDR_D2RC_AAC_IS_DUAL_MONO		        (ADDR_D2RC_DECODING_INFO_BASE)
#define ADDR_D2RC_AAC_BITRATE			        (ADDR_D2RC_DECODING_INFO_BASE+1)
#define ADDR_D2RC_AAC_FRM_CNT			        (ADDR_D2RC_DECODING_INFO_BASE+2)
#define ADDR_D2RC_AAC_CH_MODE  				    (ADDR_D2RC_DECODING_INFO_BASE+3)
#define ADDR_D2RC_AAC_SAMPLINGRATE			    (ADDR_D2RC_DECODING_INFO_BASE+4)
#define ADDR_D2RC_AAC_VERSION  	        	    (ADDR_D2RC_DECODING_INFO_BASE+5)
#define ADDR_D2RC_AAC_TRANS_FORMAT		        (ADDR_D2RC_DECODING_INFO_BASE+6)
#define ADDR_D2RC_AAC_IS_DUAL_MONO_DEC2		    (ADDR_D2RC_DECODING_INFO_BASE_DEC2)
#define ADDR_D2RC_AAC_BITRATE_DEC2		        (ADDR_D2RC_DECODING_INFO_BASE_DEC2+1)
#define ADDR_D2RC_AAC_FRM_CNT_DEC2		        (ADDR_D2RC_DECODING_INFO_BASE_DEC2+2)
#define ADDR_D2RC_AAC_CH_MODE_DEC2  		    (ADDR_D2RC_DECODING_INFO_BASE_DEC2+3)
#define ADDR_D2RC_AAC_SAMPLINGRATE_DEC2		    (ADDR_D2RC_DECODING_INFO_BASE_DEC2+4)
#define ADDR_D2RC_AAC_VERSION_DEC2  		    (ADDR_D2RC_DECODING_INFO_BASE_DEC2+5)
#define ADDR_D2RC_AAC_TRANS_FORMAT_DEC2		    (ADDR_D2RC_DECODING_INFO_BASE_DEC2+6)
#define ADDR_D2RC_AAC_IS_DUAL_MONO_DEC3         (ADDR_D2RC_DECODING_INFO_BASE_DEC3)
#define ADDR_D2RC_AAC_BITRATE_DEC3              (ADDR_D2RC_DECODING_INFO_BASE_DEC3+1)
#define ADDR_D2RC_AAC_FRM_CNT_DEC3              (ADDR_D2RC_DECODING_INFO_BASE_DEC3+2)
#define ADDR_D2RC_AAC_CH_MODE_DEC3              (ADDR_D2RC_DECODING_INFO_BASE_DEC3+3)
#define ADDR_D2RC_AAC_SAMPLINGRATE_DEC3         (ADDR_D2RC_DECODING_INFO_BASE_DEC3+4)

// SBC dec1 info
#define ADDR_D2RC_SBC_MODE                      (ADDR_D2RC_DECODING_INFO_BASE)
#define ADDR_D2RC_SBC_BITRATE                   (ADDR_D2RC_DECODING_INFO_BASE+1)
#define ADDR_D2RC_SBC_SAMPLINGFREQ              (ADDR_D2RC_DECODING_INFO_BASE+2)
#define ADDR_D2RC_SBC_MODE_DEC2                 (ADDR_D2RC_DECODING_INFO_BASE_DEC2)
#define ADDR_D2RC_SBC_BITRATE_DEC2              (ADDR_D2RC_DECODING_INFO_BASE_DEC2+1)
#define ADDR_D2RC_SBC_SAMPLINGFREQ_DEC2         (ADDR_D2RC_DECODING_INFO_BASE_DEC2+2)

#define ADDR_D2RC_CDDA_CURPNT                   (ADDR_D2RC_DECODING_INFO_BASE)
#define ADDR_RC2D_SACD_DECODED_FRAME            (ADDR_D2RC_DECODING_INFO_BASE)

// DRA
#define ADDR_D2RC_DRA_CH_NUM                      (ADDR_D2RC_DECODING_INFO_BASE)
#define ADDR_D2RC_DAR_CH_NUM_DEC2                 (ADDR_D2RC_DECODING_INFO_BASE_DEC2)

//VORBIS, ///CC_VORBIS_SUPPORT
#define ADDR_D2RC_VORBIS_SAMPLING_RATE          (ADDR_D2RC_RISC_INFO_SAMPLING_RATE) // 8020h
#define ADDR_D2RC_VORBIS_INPUT_CH_CFG           (ADDR_D2RC_INPUT_CH_CFG)            // 8025h
#define ADDR_D2RC_VORBIS_BIT_RATE               (ADDR_D2RC_RISC_INFO_BIT_RATE)      // 8029h

#define ADDR_D2RC_VORBIS_SAMPLING_RATE_DEC2     (ADDR_D2RC_RISC_INFO_SAMPLING_RATE_DEC2) // 80A0h
#define ADDR_D2RC_VORBIS_INPUT_CH_CFG_DEC2      (ADDR_D2RC_INPUT_CH_CFG_DEC2)            // 80A5h
#define ADDR_D2RC_VORBIS_BIT_RATE_DEC2          (ADDR_D2RC_RISC_INFO_BIT_RATE_DEC2)      // 80A9h

#define ADDR_D2RC_VORBIS_SAMPLING_RATE_DEC3     (ADDR_D2RC_RISC_INFO_SAMPLING_RATE_DEC3) // 8210h
#define ADDR_D2RC_VORBIS_INPUT_CH_CFG_DEC3      (ADDR_D2RC_RISC_INFO_INPUT_CH_CFG_DEC3)  // 8212h
#define ADDR_D2RC_VORBIS_BIT_RATE_DEC3          (ADDR_D2RC_RISC_INFO_BIT_RATE_DEC3)      // 8213h

// vorbis decoder information
#define ADDR_D2RC_VORBIS_TABLE_OK                 (ADDR_D2RC_DECODING_INFO_BASE)
#define ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LL16     (ADDR_D2RC_DECODING_INFO_BASE+1)
#define ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LH16     (ADDR_D2RC_DECODING_INFO_BASE+2)
#define ADDR_D2RC_VORBIS_ABS_GRANULE_POS_OK       (ADDR_D2RC_DECODING_INFO_BASE+3)
#define ADDR_D2RC_VORBIS_TABLE_OK_DEC2                  (ADDR_D2RC_DECODING_INFO_BASE_DEC2)
#define ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LL16_DEC2      (ADDR_D2RC_DECODING_INFO_BASE_DEC2+1)
#define ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LH16_DEC2      (ADDR_D2RC_DECODING_INFO_BASE_DEC2+2)
#define ADDR_D2RC_VORBIS_ABS_GRANULE_POS_OK_DEC2        (ADDR_D2RC_DECODING_INFO_BASE_DEC2+3)

// Decode DRAM: Header Parsing Error (Vorbis_dram.inc), CC_VORBIS_SUPPORT   ///CC_VORBIS_SUPPORT_ERR
#define D2RC_VORBIS_SYSTEM_ERROR_CODE               (DRAM_NORMAL_BASE + 0x05)
#define D2RC_VORBIS_PAGE_ERROR_CODE                 (DRAM_NORMAL_BASE + 0x06)
#define D2RC_VORBIS_ID_HEADER_ERROR_CODE            (DRAM_NORMAL_BASE + 0x07)
#define D2RC_VORBIS_CODEBOOK_ERROR_CODE             (DRAM_NORMAL_BASE + 0x08)
#define D2RC_VORBIS_FLOOR_ERROR_CODE                (DRAM_NORMAL_BASE + 0x09)
#define D2RC_VORBIS_RESIDUE_ERROR_CODE              (DRAM_NORMAL_BASE + 0x0A)
#define D2RC_VORBIS_MAPPING_ERROR_CODE              (DRAM_NORMAL_BASE + 0x0B)
#define D2RC_VORBIS_MODE_ERROR_CODE                 (DRAM_NORMAL_BASE + 0x0C)
#define D2RC_VORBIS_PACKET_ERROR_CODE               (DRAM_NORMAL_BASE + 0x0D)

//===================================================================
// ADDR_D2RC_SPECTRUM_BASE Section (08080h~080A0h, 10h)
// Spectrum Info
//===================================================================
#define ADDR_D2RC_SPECTRUM_BAR_0                (ADDR_D2RC_SPECTRUM_BASE + 1)
#define ADDR_D2RC_SPECTRUM_BAR_1                (ADDR_D2RC_SPECTRUM_BAR_0 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_2                (ADDR_D2RC_SPECTRUM_BAR_1 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_3                (ADDR_D2RC_SPECTRUM_BAR_2 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_4                (ADDR_D2RC_SPECTRUM_BAR_3 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_5                (ADDR_D2RC_SPECTRUM_BAR_4 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_6                (ADDR_D2RC_SPECTRUM_BAR_5 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_7                (ADDR_D2RC_SPECTRUM_BAR_6 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_8                (ADDR_D2RC_SPECTRUM_BAR_7 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_9                (ADDR_D2RC_SPECTRUM_BAR_8 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_10               (ADDR_D2RC_SPECTRUM_BAR_9 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_11               (ADDR_D2RC_SPECTRUM_BAR_10 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_12               (ADDR_D2RC_SPECTRUM_BAR_11 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_13               (ADDR_D2RC_SPECTRUM_BAR_12 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_14               (ADDR_D2RC_SPECTRUM_BAR_13 + 2)
#define ADDR_D2RC_SPECTRUM_BAR_15               (ADDR_D2RC_SPECTRUM_BAR_14 + 2)
#define ADDR_D2RC_SPECTRUM_0                    (ADDR_D2RC_SPECTRUM_BASE + 2)
#define ADDR_D2RC_SPECTRUM_1                    (ADDR_D2RC_SPECTRUM_0 + 2)
#define ADDR_D2RC_SPECTRUM_2                    (ADDR_D2RC_SPECTRUM_1 + 2)
#define ADDR_D2RC_SPECTRUM_3                    (ADDR_D2RC_SPECTRUM_2 + 2)
#define ADDR_D2RC_SPECTRUM_4                    (ADDR_D2RC_SPECTRUM_3 + 2)
#define ADDR_D2RC_SPECTRUM_5                    (ADDR_D2RC_SPECTRUM_4 + 2)
#define ADDR_D2RC_SPECTRUM_6                    (ADDR_D2RC_SPECTRUM_5 + 2)
#define ADDR_D2RC_SPECTRUM_7                    (ADDR_D2RC_SPECTRUM_6 + 2)
#define ADDR_D2RC_SPECTRUM_8                    (ADDR_D2RC_SPECTRUM_7 + 2)
#define ADDR_D2RC_SPECTRUM_9                    (ADDR_D2RC_SPECTRUM_8 + 2)
#define ADDR_D2RC_SPECTRUM_10                   (ADDR_D2RC_SPECTRUM_9 + 2)
#define ADDR_D2RC_SPECTRUM_11                   (ADDR_D2RC_SPECTRUM_10 + 2)
#define ADDR_D2RC_SPECTRUM_12                   (ADDR_D2RC_SPECTRUM_11 + 2)
#define ADDR_D2RC_SPECTRUM_13                   (ADDR_D2RC_SPECTRUM_12 + 2)
#define ADDR_D2RC_SPECTRUM_14                   (ADDR_D2RC_SPECTRUM_13 + 2)
#define ADDR_D2RC_SPECTRUM_15                   (ADDR_D2RC_SPECTRUM_14 + 2)

//===================================================================
// ADDR_D2RC_AUDIO_INFO_BASE_DEC2 Section (080A0h~080B0h, 10h)
// Audio Information for second decoder
//===================================================================
#define ADDR_D2RC_RISC_INFO_SAMPLING_RATE_DEC2  (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x00)
#define ADDR_D2RC_RISC_INFO_CHANNEL_NUM_DEC2    (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x01)
#define ADDR_D2RC_RISC_INFO_BASS_FLAG1_DEC2	    (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x02)
#define ADDR_D2RC_RISC_INFO_BASS_FLAG2_DEC2	    (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x03)
#define ADDR_D2RC_RISC_INFO_BASS_FLAG3_DEC2	    (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x04)
#define ADDR_D2RC_RISC_INFO_INPUT_CH_CFG_DEC2   (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x05)
#define ADDR_D2RC_INPUT_CH_CFG_DEC2             (ADDR_D2RC_RISC_INFO_INPUT_CH_CFG_DEC2)
#define ADDR_D2RC_RISC_INFO_DECODING_CH_CFG0_DEC2   (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x06)
#define ADDR_D2RC_RISC_INFO_DECODING_CH_CFG1_DEC2   (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x07)
#define ADDR_D2RC_RISC_INFO_DECODING_CH_NUM_DEC2    (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x08)
#define ADDR_D2RC_RISC_INFO_BIT_RATE_DEC2           (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x09)
#define ADDR_D2RC_RISC_INFO_BIT_RESSOLUTION_DEC2    (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x0A)
#define ADDR_D2RC_RISC_INFO_INPUT_SAMPLING_RATE_DEC2    (ADDR_D2RC_AUDIO_INFO_BASE_DEC2 + 0x0B)

//===================================================================
// ADDR_D2RC_POST_INFO_BASE_DEC2 Section (080B0h~080C0h, 10h)
// Post Info for second decoder
//===================================================================
#define ADDR_D2RC_RISC_INFO_POST_PROCESSING_DEC2    (ADDR_D2RC_POST_INFO_BASE_DEC2 + 0x00)
//#define ADDR_D2RC_RISC_INFO_POST_PROCESSINGII_DEC2    (ADDR_D2RC_POST_INFO_BASE_DEC2 + 0x01)
#define ADDR_D2RC_RISC_INFO_VOLUME_NOW_DEC2     (ADDR_D2RC_POST_INFO_BASE_DEC2 + 0x02)
#define ADDR_D2RC_RISC_INFO_DRC_VALUE_MAX_DEC2  (ADDR_D2RC_POST_INFO_BASE_DEC2 + 0x03)
#define ADDR_D2RC_DRAM_LOCK_DEC2                (ADDR_D2RC_POST_INFO_BASE_DEC2 + 0x04)
#define ADDR_D2RC_DRAM_STC_VALID_DEC2           (ADDR_D2RC_POST_INFO_BASE_DEC2 + 0x05)
#define ADDR_D2RC_DRAM_STC_HIGH_DEC2            (ADDR_D2RC_POST_INFO_BASE_DEC2 + 0x06)
#define ADDR_D2RC_DRAM_STC_LOW_DEC2             (ADDR_D2RC_POST_INFO_BASE_DEC2 + 0x07)
#define ADDR_D2RC_RISC_INFO_REG_BUF_PNT_DEC2    (ADDR_D2RC_POST_INFO_BASE_DEC2 + 0x08)

//===================================================================
// ADDR_D2RC_COMMON_LOG Section (080C0h~080D0h, 10h)
// Common logs
//===================================================================
#define ADDR_D2RC_COMMON_LOG_FLAG               (ADDR_D2RC_COMMON_LOG)
#define ADDR_D2RC_INPUT_UNDERRUN                (ADDR_D2RC_COMMON_LOG+1)
#define ADDR_D2RC_INPUT_UNDERRUN_DEC2           (ADDR_D2RC_COMMON_LOG+2)
#define ADDR_D2RC_INPUT_UNDERRUN_DEC3           (ADDR_D2RC_COMMON_LOG+3)
#define ADDR_D2RC_INPUT_UNDERRUN_DEC4           (ADDR_D2RC_COMMON_LOG+4)
#define ADDR_D2RC_OUTPUT_UNDERRUN               (ADDR_D2RC_COMMON_LOG+5)
#define ADDR_D2RC_OUTPUT_UNDERRUN_DEC2          (ADDR_D2RC_COMMON_LOG+6)
#define ADDR_D2RC_OUTPUT_UNDERRUN_DEC3          (ADDR_D2RC_COMMON_LOG+7)
#define ADDR_D2RC_OUTPUT_UNDERRUN_DEC4          (ADDR_D2RC_COMMON_LOG+8)

//===================================================================
// ADDR_D2RC_DECODER_LOG Section (080D0h~080D80h, 08h)
// Dec1 log
//===================================================================
#define ADDR_D2RC_DECODER_LOG_FLAG              (ADDR_D2RC_DECODER_LOG)
#define ADDR_D2RC_FRAME_REPEAT_COUNT            (ADDR_D2RC_DECODER_LOG+1)
#define ADDR_D2RC_FRAME_DROP_COUNT              (ADDR_D2RC_DECODER_LOG+2)
#define ADDR_D2RC_TOTAL_ERROR_COUNT             (ADDR_D2RC_DECODER_LOG+3)
#define ADDR_D2RC_TOTAL_FRAME_COUNT             (ADDR_D2RC_DECODER_LOG+4)

#define ADDR_D2RC_DECODER_DETAIL_LOG            (ADDR_D2RC_DECODER_LOG+8)

//===================================================================
// ADDR_D2RC_DECODER_LOG_DEC2 Section (080E8h~080F00h, 08h)
// Dec1 log
//===================================================================
#define ADDR_D2RC_DECODER_LOG_FLAG_DEC2         (ADDR_D2RC_DECODER_LOG_DEC2)
#define ADDR_D2RC_FRAME_REPEAT_COUNT_DEC2       (ADDR_D2RC_DECODER_LOG_DEC2+1)
#define ADDR_D2RC_FRAME_DROP_COUNT_DEC2         (ADDR_D2RC_DECODER_LOG_DEC2+2)
#define ADDR_D2RC_TOTAL_ERROR_COUNT_DEC2        (ADDR_D2RC_DECODER_LOG_DEC2+3)
#define ADDR_D2RC_TOTAL_FRAME_COUNT_DEC2        (ADDR_D2RC_DECODER_LOG_DEC2+4)

#define ADDR_D2RC_DECODER_DETAIL_LOG_DEC2       (ADDR_D2RC_DECODER_LOG_DEC2+8)

//===================================================================
// ADDR_D2RC_AUDIO_INFO_BASE_DEC3 Section (08210h~08220h, 10h)
//===================================================================
#define ADDR_D2RC_RISC_INFO_SAMPLING_RATE_DEC3  (ADDR_D2RC_AUDIO_INFO_BASE_DEC3 + 0x00)
#define ADDR_D2RC_RISC_INFO_CHANNEL_NUM_DEC3    (ADDR_D2RC_AUDIO_INFO_BASE_DEC3 + 0x01)
#define ADDR_D2RC_RISC_INFO_INPUT_CH_CFG_DEC3   (ADDR_D2RC_AUDIO_INFO_BASE_DEC3 + 0x02)
#define ADDR_D2RC_RISC_INFO_BIT_RATE_DEC3       (ADDR_D2RC_AUDIO_INFO_BASE_DEC3 + 0x03)
#define ADDR_D2RC_RISC_INFO_INPUT_SAMPLING_RATE_DEC3    (ADDR_D2RC_AUDIO_INFO_BASE_DEC3 + 0x04)

//===================================================================
// ADDR_D2RC_POST_INFO_BASE_DEC3 Section (08220h~08228h, 08h)
//===================================================================
#define ADDR_D2RC_RISC_INFO_VOLUME_NOW_DEC3     (ADDR_D2RC_POST_INFO_BASE_DEC3 + 0x00)
#define ADDR_D2RC_RISC_INFO_REG_BUF_PNT_DEC3    (ADDR_D2RC_POST_INFO_BASE_DEC3 + 0x04)

//===================================================================
// ADD2_D2RC_DECODER_LOG_DEC3 Section (08228h~08240h, 18h)
//===================================================================
#define ADDR_D2RC_DECODER_LOG_FLAG_DEC3         (ADDR_D2RC_DECODER_LOG_DEC3 + 0x00)
#define ADDR_D2RC_FRAME_REPEAT_COUNT_DEC3       (ADDR_D2RC_DECODER_LOG_DEC3 + 0x01)
#define ADDR_D2RC_FRAME_DROP_COUNT_DEC3         (ADDR_D2RC_DECODER_LOG_DEC3 + 0x02)
#define ADDR_D2RC_TOTAL_ERROR_COUNT_DEC3        (ADDR_D2RC_DECODER_LOG_DEC3 + 0x03)
#define ADDR_D2RC_TOTAL_FRAME_COUNT_DEC3        (ADDR_D2RC_DECODER_LOG_DEC3 + 0x04)

#define ADDR_D2RC_DECODER_DETAIL_LOG_DEC3       (ADDR_D2RC_DECODER_LOG_DEC3 + 0x08)

//===================================================================
// ADDR_D2RC_AUDIO_INFO_BASE_DEC4 Section (08250h~08260h, 10h)
//===================================================================

//===================================================================
// ADDR_D2RC_POST_INFO_BASE_DEC4 Section (08260h~08268h, 08h)
//===================================================================

//===================================================================
// ADD2_D2RC_DECODER_LOG_DEC4 Section (08268h~08280h, 18h)
//===================================================================

#define ADDR_D2RC_DECODER_DETAIL_LOG_DEC4       (ADDR_D2RC_DECODER_LOG_DEC4 + 0x08)

#endif  //D2RC_SHM_H

