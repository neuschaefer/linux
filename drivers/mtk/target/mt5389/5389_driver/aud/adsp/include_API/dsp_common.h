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
 * $Author: bofeng.fu $
 * $Date: 2012/07/17 $
 * $RCSfile: dsp_common.h,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_common.h
 *  Brief of file dsp_common.h.
 *  Details of file dsp_common.h (optional).
 */

#ifndef DSP_COMMON_H
#define DSP_COMMON_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_common.h"
#include "../include/dsp_intf.h"

//-----------------------------------------------------------------------------
// Audio DSP FPGA
//-----------------------------------------------------------------------------
#ifdef CC_FPGA
//#define DSP_ROMBOOT_TEST
#define DSP_TEST_PUTBITS
//#define DSP_TEST_CORDIC
#define DSP_TEST_PSRDMA
#define DSP_TEST_DRAM_BLOCK
    #define DSP_WORK_BUF_SIZE (8*1024*1024)
//#define DSP_TEST_CODE_FETCH
//#define DSP_TEST_LINE2
#endif

//-----------------------------------------------------------------------------
// Feature define
//-----------------------------------------------------------------------------
#define DSP_SUPPORT_NPTV
#define CC_SET_VOLUME
#define DATA_DISC_WMA_SUPPORT
#define UPMIX_SUPPORT    // PrologicII, Neo6
#define DOWNMIX_SUPPORT  // Virtual surround, Sony VSS
#ifdef UPMIX_SUPPORT
  #define PROLOGICII_SUPPORT
#endif // UPMIX_SUPPORT

//#define DSP_SUPPORT_SRSWOWHD
#define DSP_SUPPORT_SRSTSHD
//#define DSP_SUPPORT_SRSTSHD_LEVELINDEP
//#define DSP_SUPPORT_SRSVIQ
//#define DSP_SUPPORT_SRS_SSHD
//#ifndef CC_AUD_HPF_SUPPORT
//#define CC_AUD_HPF_SUPPORT
//#endif

//#define DSP_SUPPORT_SRS_PURESOUND
//#define DSP_SUPPORT_SRS_AEQ


#define DSP_G711_A_LAW   //G711 is A_LAW or U_LAW

///Use CC_VORBIS_SUPPORT instead of DSP_VORBIS_SUPPORT
///#define DSP_VORBIS_SUPPORT

#ifdef CC_AUD_DATA_UPLOAD_SUPPORT
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define AUD_UPLOAD_BUFFER_BASE (0x2ED00 * 4)
#else
#define AUD_UPLOAD_BUFFER_BASE (0x39D00 * 4)
#endif
#define AUD_UPLOAD_BUFFER_SIZE (0x8000) //(0x8000 * 4) //(0x16800) //(576,256,384,320)=23020*4=0x16800 (0x8000 * 4)
#endif

#ifndef CC_MT5389
#define MP3ENC_SUPPORT
#endif

///#define MP3ENC_DBG
#ifdef MP3ENC_SUPPORT
  ///#define UPLOAD_BUFFER_BASE        0x39D00*4
  ///32 output buffer bank, 0x100 for 256 align.

  ///Default use 128Kbps
  ///#define MP3ENC_320K_BR
  ///#define MP3ENC_192K_BR
  #define MP3ENC_128K_BR

  /// Current Mp3Enc buffer time = 1152*64/48000 = 1.536 sec. if 32-> 1152*32/48000 = 0.765 sec.
  #define MP3ENC_DSP_HQ_BUF         128  /// must be 2^N, if change need modify DSP code too.
  #if defined (MP3ENC_320K_BR)
  #define MP3ENC_BR         0x0E
  #define MP3ENC_FRAME_SIZE 0x3C0                   /// 1152/48000*320000/8 = 960 = 0x3C0
  #define MP3_OUT_BUF_SZ    (MP3ENC_FRAME_SIZE*MP3ENC_DSP_HQ_BUF/0x100)
  #elif defined (MP3ENC_192K_BR)
  #define MP3ENC_BR         0x0B
  #define MP3ENC_FRAME_SIZE 0x240                   /// 1152/48000*192000/8 = 576 = 0x240
  #define MP3_OUT_BUF_SZ    (MP3ENC_FRAME_SIZE*MP3ENC_DSP_HQ_BUF/0x100)
  #else ///defined (MP3ENC_128K_BR)
  #define MP3ENC_BR         0x09
  #define MP3ENC_FRAME_SIZE 0x180                   /// 1152/48000*128000/8 = 384 = 0x180
  #define MP3_OUT_BUF_SZ    (MP3ENC_FRAME_SIZE*MP3ENC_DSP_HQ_BUF/0x100)
  #endif
#endif

// DSP check definition and variables
//#define ADSP_CHECK_RAM_CODE

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define NEW_MIXSOUND
#endif
//#define MIXSOUND_MIPS_MEASURE
#define ALSA_MIXSND_PATH
//#define MIXSOUND_GAIN_CHG_WITH_VOL

#define MAX_AUD_MIXSND_STREAM_NUM 9 //8+1(alsa)
#ifdef ALSA_MIXSND_PATH
#define ALSA_MIXSND_STREAM_ID (MAX_AUD_MIXSND_STREAM_NUM-1)
#endif

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define DSP_SUPPORT_17BIT
#endif

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define FIX_AUD_UOP_TIMEOUT
#define SPDIF_MUTE_BY_PINMUX
#endif

//===================================================================
// Common variables
//===================================================================
#define SRAM_COMMVAR                            (0x0900U)
#define SRAM_COMMVAR2                           (0x3680U)
#define SRAM_COMMVAR3                           (0x0980U)
#define SRAM_COMMVAR4                           (0x0A00U)

#define SRAM_JOBVAR                             (0x3740U)
#define SRAM_FIXDAT                             (0x0B00U)
#define SRAM_FIXDAT_DEC2                        (0x3600U)
#define SRAM_FIXDAT_DEC3                        (0x3580U)
#define SRAM_FIXDAT_DEC4                        (0x0B80U)
#define SRAM_CTRL_1                             (0x0800U)
#define SRAM_CTRL_2                             (0x0820U)
#define SRAM_CTRL_4                             (0x0840U)
#define SRAM_CTRL_6                             (0x0860U)
#define SRAM_EXTVAR                             (0x0A80U)

#define SRAM_DECODER_INPUT_UNDERRUN             (SRAM_COMMVAR  + 0x14U)
#define SRAM_DECODER_INPUT_UNDERRUN_DEC2        (SRAM_COMMVAR2 + 0x14U)
#define SRAM_DECODER_INPUT_UNDERRUN_DEC3        (SRAM_COMMVAR3 + 0x14U)
#define SRAM_DECODER_INPUT_UNDERRUN_DEC4        (SRAM_COMMVAR4 + 0x14U)
#define SRAM_DECODER_OUTPUT_UNDERRUN            (SRAM_COMMVAR  + 0x15U)
#define SRAM_DECODER_OUTPUT_UNDERRUN_DEC2       (SRAM_COMMVAR2 + 0x15U)
#define SRAM_DECODER_OUTPUT_UNDERRUN_DEC3       (SRAM_COMMVAR3 + 0x15U)
#define SRAM_DECODER_OUTPUT_UNDERRUN_DEC4       (SRAM_COMMVAR4 + 0x15U)
#define SRAM_DECODE_ERROR_COUNT                 (SRAM_COMMVAR  + 0x16U)
#define SRAM_DECODE_ERROR_COUNT_DEC2            (SRAM_COMMVAR2 + 0x16U)
#define SRAM_DECODE_ERROR_COUNT_DEC3            (SRAM_COMMVAR3 + 0x16U)
#define SRAM_DECODE_ERROR_COUNT_DEC4            (SRAM_COMMVAR4 + 0x16U)
#define SRAM_DECODE_FRAME_COUNT                 (SRAM_COMMVAR  + 0x17U)
#define SRAM_DECODE_FRAME_COUNT_DEC2            (SRAM_COMMVAR2 + 0x17U)
#define SRAM_DECODE_FRAME_COUNT_DEC3            (SRAM_COMMVAR3 + 0x17U)
#define SRAM_DECODE_FRAME_COUNT_DEC4            (SRAM_COMMVAR4 + 0x17U)

#define SRAM_DECODER_CONTROL                    (SRAM_COMMVAR  + 0x00U)
#define SRAM_DECODER_CONTROL_DEC2               (SRAM_COMMVAR2 + 0x00U)
#define SRAM_DECODER_CONTROL_DEC3               (SRAM_COMMVAR3 + 0x00U)
#define SRAM_DECODER_CONTROL_DEC4               (SRAM_COMMVAR4 + 0x00U)

#define SRAM_DECODER_STATE                      (SRAM_JOBVAR + 0x06U)
#define SRAM_DECODER_STATE_DEC2                 (SRAM_JOBVAR + 0x07U)
#define SRAM_DECODER_STATE_DEC3                 (SRAM_JOBVAR + 0x08U)
#define SRAM_DECODER_STATE_DEC4                 (SRAM_JOBVAR + 0x09U)

#define SRAM_PTS_PRS_PNT                        (SRAM_COMMVAR  + 0x68U)
#define SRAM_PTS_STCH                           (SRAM_COMMVAR  + 0x6AU)
#define SRAM_PTS_STCL                           (SRAM_COMMVAR  + 0x6BU)
#define SRAM_PTS_PRS_PNT_DEC2                   (SRAM_COMMVAR2 + 0x68U)
#define SRAM_PTS_STCH_DEC2                      (SRAM_COMMVAR2 + 0x6AU)
#define SRAM_PTS_STCL_DEC2                      (SRAM_COMMVAR2 + 0x6BU)

#define SRAM_IEC_FLAG                           (SRAM_COMMVAR  + 0x50U)
#define SRAM_IEC_FLAG_DEC2                      (SRAM_COMMVAR2 + 0x50U)

#define SRAM_EFFECT_CONTROL                     (SRAM_COMMVAR  + 0x0FU)
#define SRAM_EFFECT_CONTROL_DEC2	            (SRAM_COMMVAR2 + 0x0FU)
#define SRAM_EFFECT_CONTROL_DEC3	            (SRAM_COMMVAR3 + 0x0FU)

#define SRAM_SYSTEM_CTRL                        (SRAM_CTRL_1 + 0x01U)
    #define LOG_DRAM_REFRESH                    (0x1U<<4)

#define SRAM_RISC_POST_CTRL                    (SRAM_CTRL_1 + 0x1DU)


#define SRAM_SMOOTH_VOLUME                      (SRAM_CTRL_2 + 0x09U)
#define SRAM_SMOOTH_VOLUME_DEC2                 (SRAM_CTRL_4 + 0x09U)
#define SRAM_SMOOTH_VOLUME_DEC3                 (SRAM_CTRL_6 + 0x04U)

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
#define SRAM_ADV_chCalcLevel_L                  (SRAM_COMMVAR3  + 0x1AU)
#define SRAM_ADV_chCalcLevel_R                  (SRAM_COMMVAR3  + 0x1BU)
#endif

#define SRAM_SRC_FLAG                           (SRAM_EXTVAR + 0x00U)
#define SRAM_SRC_PUT_BANK_CNT_DEC1              (SRAM_EXTVAR + 0x11U)
#define SRAM_SRC_GET_BANK_CNT_DEC1              (SRAM_EXTVAR + 0x12U)
#define SRAM_SRC_PUT_BANK_CNT_DEC2              (SRAM_EXTVAR + 0x13U)
#define SRAM_SRC_GET_BANK_CNT_DEC2              (SRAM_EXTVAR + 0x14U)
#define SRAM_SRC_PUT_BANK_CNT_DEC3              (SRAM_EXTVAR + 0x15U)
#define SRAM_SRC_GET_BANK_CNT_DEC3              (SRAM_EXTVAR + 0x16U)

#define SRAM_EFFECT_TRANSITION_FLAG             (SRAM_COMMVAR  + 0x36U)
#define SRAM_EFFECT_TRANSITION_FLAG_DEC2        (SRAM_COMMVAR2  + 0x36U)
    #define IS_WITH_VIDEO                       0x000400

#define SRAM_AOUT_CH_CFG0                       (SRAM_COMMVAR  + 0x26U) //QUERY_DELAY
#define SRAM_AOUT_CH_CFG1                       (SRAM_COMMVAR  + 0x27U)
#define SRAM_AOUT_CH_CFG2                       (SRAM_COMMVAR  + 0x28U)

#define SRAM_CHK_FRAME_SIZE_BACKUP              (SRAM_COMMVAR  + 0x2fU)
#define SRAM_CHK_FRAME_SIZE_BACKUP_DEC2         (SRAM_COMMVAR2 + 0x2fU)
#define SRAM_CHK_FRAME_SIZE_BACKUP_DEC3         (SRAM_COMMVAR3 + 0x2fU)
#define SRAM_CHK_FRAME_SIZE_BACKUP_DEC4         (SRAM_COMMVAR4 + 0x2fU)

#define SRAM_INTBUF4_INPUT_FLAG					(SRAM_COMMVAR4 + 0x2bU)

//===================================================================
// HW Registers
//===================================================================

#define REG_DSP_BUF_PNT                         (0x20bU)
#define REG_DSP_PRSPNT                          (0x250U)
#define REG_DSP_BUF_PNT_DEC2                    (0x20eU)
#define REG_DSP_PRSPNT_DEC2                     (0x251U)
#define REG_DSP_BUF_PNT_DEC3                    (0x21aU)
#define REG_DSP_PRSPNT_DEC3                     (0x25cU)
#define REG_DSP_BUF_PNT_DEC4                    (0x21dU)
#define REG_DSP_PRSPNT_DEC4                     (0x25dU)

#define REG_DSP_CHL_1_ADDR                      (0x280U)
#define REG_DSP_CHL_2_ADDR                      (0x281U)
#define REG_DSP_CHL_3_ADDR                      (0x282U)
#define REG_DSP_CHL_4_ADDR                      (0x283U)
#define REG_DSP_CHL_5_ADDR                      (0x284U)
#define REG_DSP_CHL_6_ADDR                      (0x285U)
#define REG_DSP_CHL_7_ADDR                      (0x286U)
#define REG_DSP_CHL_8_ADDR                      (0x287U)
#define REG_DSP_CHL_9_ADDR                      (0x288U)
#define REG_DSP_CHL_10_ADDR                     (0x289U)
#define REG_DSP_CHL_11_ADDR                     (0x28aU)

#define REG_DSP_AOUT_CTRL                       (0x297U)
#define REG_DSP_CH_CFG_0                        (0x298U)
#define REG_DSP_CH_CFG_1                        (0x299U)
#define REG_DSP_CH_CFG_2                        (0x29aU)
#define REG_DSP_CH_NUM                          (0x29bU)

#define REG_DSP_PBS_SADR                        (0x2D4U)    /// MP3ENC_SUPPORT
#define REG_DSP_PBS_EADR                        (0x2D5U)

#define REG_DSP_CHL_1_ADDR_2                    (0x2e0U)
#define REG_DSP_CHL_2_ADDR_2                    (0x2e1U)
#define REG_DSP_CHL_3_ADDR_2                    (0x2e2U)

#define REG_DSP_AOUT_CTRL_2                     (0x2f7U)
#define REG_DSP_CH_CFG0_2                       (0x2f8U)
#define REG_DSP_CH_NUM_2                        (0x2fbU)

#define REG_DSP_SRC_GEN_CONF                    (0x580U)
#define REG_DSP_SRC_GEN_CONF2                   (0x583U)
#define REG_DSP_SRC_FREQUENCY_0                 (0x588U)
#define REG_DSP_SRC_FREQUENCY_1                 (0x589U)
#define REG_DSP_SRC_FREQUENCY_2                 (0x58aU)
#define REG_DSP_SRC_FREQUENCY_3                 (0x58bU)
#define REG_DSP_SRC_IBUF_SADR                   (0x58cU)
#define REG_DSP_SRC_IBUF_SIZE                   (0x58dU)
#define REG_DSP_SRC_OBUF_SADR                   (0x58eU)
#define REG_DSP_SRC_OBUF_SIZE                   (0x58fU)
#define REG_DSP_SRC_CH01_IBUF_RDPNT             (0x590U)
#define REG_DSP_SRC_CH23_IBUF_RDPNT             (0x591U)
#define REG_DSP_SRC_CH45_IBUF_RDPNT             (0x592U)
#define REG_DSP_SRC_CH67_IBUF_RDPNT             (0x593U)
#define REG_DSP_SRC_CH01_IBUF_WRPNT             (0x594U)
#define REG_DSP_SRC_CH23_IBUF_WRPNT             (0x595U)
#define REG_DSP_SRC_CH45_IBUF_WRPNT             (0x596U)
#define REG_DSP_SRC_CH67_IBUF_WRPNT             (0x597U)
#define REG_DSP_SRC_CH01_OBUF_WRPNT             (0x598U)
#define REG_DSP_SRC_CH23_OBUF_WRPNT             (0x599U)
#define REG_DSP_SRC_CH45_OBUF_WRPNT             (0x59aU)
#define REG_DSP_SRC_CH67_OBUF_WRPNT             (0x59bU)
#define REG_DSP_SRC_CH01_OBUF_RDPNT             (0x59cU)
#define REG_DSP_SRC_CH23_OBUF_RDPNT             (0x59dU)
#define REG_DSP_SRC_CH45_OBUF_RDPNT             (0x59eU)
#define REG_DSP_SRC_CH67_OBUF_RDPNT             (0x59fU)
#define REG_DSP_SRC_CH89_IBUF_RDPNT             (0x5acU)
#define REG_DSP_SRC_CH89_IBUF_WRPNT             (0x5adU)
#define REG_DSP_SRC_CH89_OBUF_WRPNT             (0x5aeU)
#define REG_DSP_SRC_CH89_OBUF_RDPNT             (0x5afU)

//===================================================================
// Decoder variables
//===================================================================
// ATV debug
#define SRAM_DECODER_VERSION                    (0x8002U)
#define SRAM_AUDIO_CARRIER_AIN0                 (0x2b2U)
#define SRAM_AIN_BUFFER0_OVERFLOW               (SRAM_COMMVAR3 + 0x38U)

#define SRAM_PGA_GAIN                           (0x2b5U)
#define SRAM_PGA_GAIN_DEC2                      (0x2b5U)

// MTS debug
#define SRAM_MTS_USER_MODE                      (SRAM_FIXDAT + 0x00U)
#define SRAM_MTS_USER_MODE_DEC2                 (SRAM_FIXDAT_DEC2 + 0x00U)
#define SRAM_MTS_DETECTED_MODE                  (SRAM_FIXDAT + 0x04U)
#define SRAM_MTS_DETECTED_MODE_DEC2             (SRAM_FIXDAT_DEC2 + 0x04U)
#define SRAM_MTS_PILOT_ENERGY                   (SRAM_FIXDAT + 0x38U)
#define SRAM_MTS_PILOT_ENERGY_DEC2              (SRAM_FIXDAT_DEC2 + 0x38U)
#define SRAM_MTS_NOISE_ENERGY                   (SRAM_FIXDAT + 0x3AU)
#define SRAM_MTS_NOISE_ENERGY_DEC2              (SRAM_FIXDAT_DEC2 + 0x3AU)
#define SRAM_MTS_SAP_ENERGY                     (SRAM_FIXDAT + 0x3BU)
#define SRAM_MTS_SAP_ENERGY_DEC2                (SRAM_FIXDAT_DEC2 + 0x3BU)

//lint -e717 do ... while(0);

//-----------------------------------------------------------------------------
// common define
//-----------------------------------------------------------------------------

#define ON                          1
#define OFF                         0

#define SV_OFF                      0
#define SV_ON                       1

/*
  CAUTION : The IDX order is also related by the _u4DspDramBufLen and _u4DspICacheLen in dsp_intf.c
            If you change the IDX order, please also check them.
*/
// For Icache start block settings
#define TYPE_ROM_ICACHE             0
#define TYPE_POST_ICACHE            1
#define TYPE_CRI_ICACHE             2
#define TYPE_COMM_ICACHE            3
#define TYPE_DEC4_ICACHE            4
#define TYPE_DEC1_ICACHE            5
#define TYPE_DEC3_ICACHE            6
#define TYPE_DEC2_ICACHE            7
#ifdef DSP_SUPPORT_17BIT
#define TYPE_DEC4_ICACHE2           8
#define TYPE_DEC1_ICACHE2           9
#define TYPE_DEC3_ICACHE2           10
#define TYPE_DEC2_ICACHE2           11
#define ICACHE_MAX_IDX              TYPE_DEC2_ICACHE2
#else
#define ICACHE_MAX_IDX              TYPE_DEC2_ICACHE
#endif

// For data DRAM settings
#define TYPE_DEC1_DRAM_IDX          0
#define TYPE_DEC2_DRAM_IDX          1
#define TYPE_DEC3_DRAM_IDX          2
#define TYPE_COMM_DRAM_IDX          3
#define TYPE_DEC4_DRAM_IDX          4
#define TYPE_MIX_DRAM_IDX           5
#define TYPE_RESERVED_6_IDX         6
#define TYPE_RESERVED_7_IDX         7

// Audio Stream Type Definition
// Decoder1 Type (0x00 ~ 0x1f)
#define TYPE_AUD_DEC1               0x00
#define AUD_AC3_DEC1                (TYPE_AUD_DEC1)
#define AUD_MPEG_DEC1               (TYPE_AUD_DEC1+0x01)
#define AUD_AAC_DEC1                (TYPE_AUD_DEC1+0x02)
#define AUD_PCM_DEC1                (TYPE_AUD_DEC1+0x03)
#define AUD_MP3_DEC1                (TYPE_AUD_DEC1+0x04)
#define AUD_WMA_DEC1                (TYPE_AUD_DEC1+0x05)
#define AUD_PINK_DEC1               (TYPE_AUD_DEC1+0x06)
#define AUD_MTS_DEC1                (TYPE_AUD_DEC1+0x07)
#define AUD_PAL_DEC1                (TYPE_AUD_DEC1+0x08)
#define AUD_MINER_DEC1              (TYPE_AUD_DEC1+0x09)
#define AUD_A2DEC_DEC1              (TYPE_AUD_DEC1+0x0A)
#define AUD_DETECTOR_DEC1           (TYPE_AUD_DEC1+0x0B)
#define AUD_FMFM_DEC1               (TYPE_AUD_DEC1+0x0C)
#define AUD_LPCM_DEC1               (TYPE_AUD_DEC1+0x0D)
#define AUD_FMRDO_DEC1              (TYPE_AUD_DEC1+0x0E)
#define AUD_FMRDODET_DEC1           (TYPE_AUD_DEC1+0x0F)
#define AUD_SBC_DEC1                (TYPE_AUD_DEC1+0x10)
#define AUD_G711_DEC1               (TYPE_AUD_DEC1+0x11)
#define AUD_G711_ENC1               (TYPE_AUD_DEC1+0x12)
#define AUD_DRA_DEC1                (TYPE_AUD_DEC1+0x13)
#define AUD_COOK_DEC1               (TYPE_AUD_DEC1+0x14)
#define AUD_G729_DEC1               (TYPE_AUD_DEC1+0x15)
#define AUD_DTS_DEC1                (TYPE_AUD_DEC1+0x16)
#define AUD_VORBIS_DEC1             (TYPE_AUD_DEC1+0x17)    ///CC_VORBIS_SUPPORT
#define AUD_WMAPRO_DEC1             (TYPE_AUD_DEC1+0x18)
#define AUD_AMR_DEC1                (TYPE_AUD_DEC1+0x19)
#define AUD_AWB_DEC1                (TYPE_AUD_DEC1+0x1A)
#define DECODER1_MAX_INDEX          (AUD_AWB_DEC1) //fixme when increasing decoders

#define DSP_DTV				        (TYPE_AUD_DEC1+0x1E) // Light: Add DTV type to reduce analog output latency
#define DSP_TV		    	        (TYPE_AUD_DEC1+0x1F)

// decoder2 Type (0x20~0x3f)
#define TYPE_AUD_DEC2               0x20
#define AUD_AC3_DEC2                (TYPE_AUD_DEC2)
#define AUD_MPEG_DEC2               (TYPE_AUD_DEC2+0x01)
#define AUD_AAC_DEC2                (TYPE_AUD_DEC2+0x02)
#define AUD_PCM_DEC2                (TYPE_AUD_DEC2+0x03)
#define AUD_MTS_DEC2                (TYPE_AUD_DEC2+0x04)
#define AUD_PAL_DEC2                (TYPE_AUD_DEC2+0x05)
#define AUD_MINER_DEC2              (TYPE_AUD_DEC2+0x06)
#define AUD_A2DEC_DEC2              (TYPE_AUD_DEC2+0x07)
#define AUD_DETECTOR_DEC2           (TYPE_AUD_DEC2+0x08)
#define AUD_FMFM_DEC2               (TYPE_AUD_DEC2+0x09)
#define AUD_MP3_DEC2                (TYPE_AUD_DEC2+0x0a)
#define AUD_LPCM_DEC2               (TYPE_AUD_DEC2+0x0b)
#define AUD_DRA_DEC2                (TYPE_AUD_DEC2+0x0c)
#define AUD_G729_DEC2               (TYPE_AUD_DEC2+0x0d)
#define AUD_DTS_DEC2                (TYPE_AUD_DEC2+0x0e)
#define AUD_COOK_DEC2               (TYPE_AUD_DEC2+0x0f)
#define AUD_VORBIS_DEC2             (TYPE_AUD_DEC2+0x10)    ///CC_VORBIS_SUPPORT
#define DECODER2_MAX_INDEX          (AUD_VORBIS_DEC2)

#define AUD_COMM_DEC2               0x40

// mix sound Type (0x40~0x5f)
#define TYPE_AUD_MIX                0x40
#define TYPE_AUD_DEC3               TYPE_AUD_MIX
#define AUD_MPEG_DEC3               (TYPE_AUD_MIX+0x00)
#define AUD_PCM_DEC3                (TYPE_AUD_MIX+0x01)
#define AUD_AAC_DEC3                (TYPE_AUD_MIX+0x02)
#define AUD_AC3_DEC3                (TYPE_AUD_MIX+0x03)
#define AUD_MP3ENC_DEC3             (TYPE_AUD_MIX+0x04)     /// MP3ENC_SUPPORT

#define DECODER3_MAX_INDEX          (AUD_AC3_DEC3)

#define AUD_COMM_MIX                0x80

#ifdef DSP_SUPPORT_NPTV
#define TYPE_DSP_INT                0x90
#define DSP_INT_COMM                (TYPE_DSP_INT+0x00)
#define DSP_INT_NTSC                (TYPE_DSP_INT+0x01)
#define DSP_INT_PAL                 (TYPE_DSP_INT+0x02)
#define DSP_INT_NICAM               (TYPE_DSP_INT+0x03)
#define DSP_INT_A2                  (TYPE_DSP_INT+0x04)
#define DSP_INT_DETECTOR            (TYPE_DSP_INT+0x05)
#define DSP_INT_FMFM                (TYPE_DSP_INT+0x06)
#endif

#define TYPE_AUD_DEC4               0x80
#define AUD_SBCENC_DEC4             (TYPE_AUD_DEC4+0x00)
#define AUD_PCM_DEC4                (TYPE_AUD_DEC4+0x01)
#define DECODER4_MAX_INDEX          (AUD_PCM_DEC4)

// Common RAM
#define TYPE_AUD_CMM                0xE0
#if 0 //def CC_AUD_DSP_SUPPORT_AUDYSSEY
//Daniel, 2009/12/7, ADV code will be automatically loaded with AUD_POSTRAM
#define AUD_ADV       0xF8
#endif

//zy_dts
#define AUD_DTSENC					0xF8
#define DSPF_DROM_DTSE				400  //big value to distinguish with dsp_data.h


#define AUD_DDCO                    0xF9
#define AUD_AININTRAM_MINER         0xFA
#define AUD_AININTRAM               0xFB  // Light added for 2nd cache (AintInt)
#define AUD_ROMCOD                  0xFC
#define AUD_POSTRAM                 0xFD
#define AUD_COMMRAM                 0xFE
#define AUD_TRAP                    0xFF  // Trap

#define DSP_NC_RAM_ROM              0x00000001
#define DSP_NC_RAM_POST             0x00000002
#define DSP_NC_RAM_COMM             0x00000004
#define DSP_NC_RAM_DEC2             0x00000008
#define DSP_NC_RAM_AIN              0x00000010
#define DSP_NC_RAM_DEC1             0x00000020

#define DSP_NC_TBL_AC3_DEC1         0x00000001
#define DSP_NC_TBL_MPEG_DEC1        0x00000002
#define DSP_NC_TBL_AAC_DEC1         0x00000004
#define DSP_NC_TBL_MP3_DEC1         0x00000008
#define DSP_NC_TBL_WMA_N_DEC1       0x00000010
#define DSP_NC_TBL_WMA_C_DEC1       0x00000020
#define DSP_NC_TBL_VORBIS_N_DEC1    0x00000040
#define DSP_NC_TBL_VORBIS_C_DEC1    0x00000080
#define DSP_NC_TBL_MTS_DEC1         0x00000100

#define DSP_NC_TBL_AC3_DEC2         0x00010000
#define DSP_NC_TBL_MPEG_DEC2        0x00020000
#define DSP_NC_TBL_AAC_DEC2         0x00040000
#define DSP_NC_TBL_MTS_DEC2         0x00080000


#define ADAC_CMD_FMT                0x01
#define ADAC_CMD_MUTE               0x02
#define ADAC_CMD_VOL                0x03
#define ADAC_CMD_EMPH               0x04
#define ADAC_CMD_DATA_FMT           0x05
#define ADAC_CMD_DAC_MUTE           0x06
#define ADAC_CMD_DSD_MODE           0x07
#define ADAC_SET_DSP_SPEED_DMPLL    0x08
#define ADAC_SET_DSP_SEEED_APLL     0x09
#define ADAC_CMD_AIN_SEL            0x10

//Code 3D
#define CODE3D_NONE                 0
#define CODE3D_VIRTUAL_SURROUND     1
#define CODE3D_PROLOGIC_II          2
#define CODE3D_SRS_WOW              3
#define CODE3D_SRS_TSXT             4

//bonding related
#define BOND_2CH                    (0x1)
#define BOND_INVALID                (0x1 << 15)

//for IEC max frequency
#define SV_48K                      1
#define SV_96K                      2
#define SV_192K                     3

/* MTS auto update use */
#define MTS_MONO                    0x01
#define MTS_STEREO                  0x02
#define MTS_SAP                     0x04

/* Japan & Korea MPX */
#define MPX_MONO                    0x08
#define MPX_STEREO                  0x10
#define MPX_SUB                     0x20
#define MPX_MAIN_SUB                0x40
#define MPX_MAIN                    0x80

#define FM_MONO                     0x01
#define A2_STEREO                   0x02
#define A2_DUAL1                    0x04
#define A2_DUAL2                    0x08
#define NICAM_MONO                  0x10
#define NICAM_STEREO                0x20
#define NICAM_DUAL1                 0x40
#define NICAM_DUAL2                 0x80

#define DSP_EN_CLK                  0x1
#define DSP_SEND_INT                0x6
#define DSP_INT_SVC                 0x7
#define DSP_UOP_SVC                 0x8

#define _VDO_FLG_PORTING            1

#define FLASHC_SA                   0
#define FLASHNC_SA                  0
#define DRAMAC_SA                   0
#define DRAMANC_SA                  0
#define DRAMBC_SA                   0
#define DRAMBNC_SA                  0

#define EQUALIZER_BAND_NUM_1        9

//Cache flush size
#define CACHE_FLUSH_TYPE_ICACHE     0
#define CACHE_FLUSH_TYPE_DATA       1
#define CACHE_FLUSH_TYPE_FREE       2

// Dec3 vector size
#define DEC3_VCT_SIZE               (0x80/2)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// dsp state machine variable
typedef enum
{
    DSP_INIT,
    DSP_CK_INT,
    DSP_CK_SINT,
    DSP_RC_INIT,
    DSP_FLUSH_CACHE,
    DSP_WAIT_POWER_OFF,
    DSP_IDLE
} DSPSTATE;

#ifdef DSP_SUPPORT_SRSTSXT
/* SRS TSXT*/
enum SRSTSXT_MODE_LIST
{
	SV_A_SRSTSXT_OFF,
	SV_A_SRSTSXT_3DMONO,
	SV_A_SRSTSXT_STEREO
};
#endif

#ifdef  CC_AUD_SUPPORT_MS10
#define     DMX_SPKCFG_LTRT    0
#define     DMX_SPKCFG_LORO    2
enum DSP_MS10_DMXMODE
{
    DMX_MS10_CUSTOM0,
    DMX_MS10_CUSTOM1,
    DMX_MS10_LINE,
    DMX_MS10_RF,
};

enum DSP_MDC_DMXMODE
{
    DMX_LINE_LTRT,
    DMX_LINE_LORO,
    DMX_RF_LTRT,
    DMX_RF_LORO
};
#endif

#ifdef DSP_SUPPORT_NPTV
// Copy From 8202 UI
/* Demodulaton Source Type */
enum
{
	SV_SRC_AF,
	SV_SRC_SIF,
	SV_DEMOD_SRC_MAX
};

enum
{
	SV_PAL_ALL_MODE,
	SV_PAL_FM_ONLY_MODE,
	SV_PAL_NICAM_ONLY_MODE
};

enum
{
	SV_NICAM_DUAL_ALL,
	SV_NICAM_DUAL_A,
	SV_NICAM_DUAL_B
};

/* A2, NICAM */
enum
{
	SV_FM_MONO,
	SV_A2_STEREO,
	SV_A2_DUAL1,
	SV_A2_DUAL2,
	SV_NICAM_MONO,
	SV_NICAM_STEREO,
	SV_NICAM_DUAL1,
	SV_NICAM_DUAL2,
	SV_A2_NICAM_MAX
};
//End of Copy From 8202 UI

#ifndef _TV_AUD_SYS_T
#define _TV_AUD_SYS_T
typedef enum
{
	SV_NONE_DETECTED = 0x0,	    //0x0
	SV_MTS           = 0x1,     //0x1
	SV_FM_FM         = 0x2,     //0x2
	SV_NTSC_M        = 0x3,     //0x3
	SV_A2_BG         = 0x4,     //0x4
	SV_A2_DK1        = 0x5,     //0x5
	SV_A2_DK2        = 0x6,     //0x6
	SV_A2_DK3        = 0x7,     //0x7
	SV_PAL_I         = 0x8,     //0x8
	SV_PAL_BG        = 0x9,     //0x09
	SV_PAL_DK        = 0xa,     //0x0a
	SV_SECAM_L       = 0xb,     //0x0b
	SV_SECAM_L_PLUM  = 0xc,     //0x0c
	SV_SECAM_BG      = 0xd,	//0x0d, 0x0c is reserved for SECAM L' setting for tuner
	SV_SECAM_DK		 = 0xe      //0x0e
} TV_AUD_SYS_T;
#endif
#endif

// decoder1 & decoder2's bitstream type definition
#define COMMDEC_STREAM              0x00
#define AC3_STREAM                  (COMMDEC_STREAM)
#define MPEG12_STREAM               (COMMDEC_STREAM+0x01)
#define AAC_STREAM                  (COMMDEC_STREAM+0x02)
#define PCM_STREAM                  (COMMDEC_STREAM+0x03)
#define MPEG3_STREAM                (COMMDEC_STREAM+0x04)
#define WMA_STREAM                  (COMMDEC_STREAM+0x05)
#define PINK_NOISE_STREAM           (COMMDEC_STREAM+0x06)
#ifdef DSP_SUPPORT_NPTV
#define NTSC_STREAM                 (COMMDEC_STREAM+0x07)
#define PAL_STREAM                  (COMMDEC_STREAM+0x08)
#define MINER_STREAM                (COMMDEC_STREAM+0x09)
#define A2_STREAM                   (COMMDEC_STREAM+0x0A)
#define DETECTOR_STREAM             (COMMDEC_STREAM+0x0B)
#define FMFM_STREAM                 (COMMDEC_STREAM+0x0C)
#endif
#define LPCM_STREAM                 (COMMDEC_STREAM+0x0D)
#define FMRDO_STREAM                (COMMDEC_STREAM+0x0E)
#define FMRDODET_STREAM             (COMMDEC_STREAM+0x0F)
#define SBC_STREAM                  (COMMDEC_STREAM+0x10)
#define G711DEC_STREAM              (COMMDEC_STREAM+0x11)
#define G711ENC_STREAM              (COMMDEC_STREAM+0x12)
#define SBCE_STREAM                 (COMMDEC_STREAM+0x13)
#define DRA_STREAM                  (COMMDEC_STREAM+0x14)
#define COOK_STREAM                 (COMMDEC_STREAM+0x15)
#define MP3ENC_STREAM               (COMMDEC_STREAM+0x16)   /// MP3ENC_SUPPORT
#define G729DEC_STREAM              (COMMDEC_STREAM+0x17)
#define DTSDEC_STREAM               (COMMDEC_STREAM+0x18)
#define VORBIS_STREAM               (COMMDEC_STREAM+0x19)   /// CC_VORBIS_SUPPORT
#define WMAPRO_STREAM               (COMMDEC_STREAM+0x1A)
#define AMR_STREAM                  (COMMDEC_STREAM+0x1B)
#define AWB_STREAM                  (COMMDEC_STREAM+0x1C)


#define COMMMIX_STREAM              0x40

#define TRAP_STREAM                 0xff

//detector related constant
#ifdef DSP_SUPPORT_NPTV
#define SOURCE_PAL_I                0x01
#define SOURCE_PAL_BG	            0x02
#define SOURCE_PAL_DK	            0x04

#define SOURCE_SECAM_L	            0x0100
#define SOURCE_SECAM_BG	            0x0200
#define SOURCE_SECAM_DK	            0x0400

#define SOURCE_NTSC_M               0x01
#define SOURCE_A2_BG                0x02
#define SOURCE_A2_DK1               0x04
#define SOURCE_A2_DK2               0x08
#define SOURCE_A2_DK3               0x10
#endif

/*--------------------- Data  Interface ----------------------*/
typedef struct
{
    UINT16 wDspSpkCfg;
    UINT8 bFrntSpkrNm;
    UINT8 bSrndSpkrNm;
    BOOL fgIsSWExst;
    UINT8 bChNm;
} SPEAKER_SETTING_T;

typedef struct
{
    /* common code field*/
    UINT32 dwInputUnderRun;         /* error count for input underrun*/
    UINT32 dwInputUnderRunDec2;     /* error count for input underrun of decoder 2*/
    UINT32 dwInputUnderRunDec3; /* error count for input underrun of decoder 3*/
    UINT32 dwOutputUnderRun;        /* error count for input output underrun*/
    UINT32 dwOutputUnderRunDec2;    /* error count for input output underrun of decoder2*/
    UINT32 dwOutputUnderRunDec3;/* error count for input output underrun of decoder 3*/
    UINT32 dwReserved0[9];          /* reserved field */
    /* first decoder's field*/
    UINT32 dwFrameRepeatCount;      /* repeat count for frames*/
    UINT32 dwFrameDropCount;        /* drop count for frames*/
    UINT32 dwTotalErrorCount;       /* total error count*/
    UINT32 dwTotalFrameCount;       /* total frame count*/
    UINT32 dwReserved1[3];          /* reserved field*/
    UINT32 dwDetailLogs[16] ;       /* detail log for decoder1*/
    /* second decoder's field*/
    UINT32 dwFrameRepeatCountDec2;  /* repeat count for frames*/
    UINT32 dwFrameDropCountDec2;    /* drop count for frames*/
    UINT32 dwTotalErrorCountDec2;   /* total error count*/
    UINT32 dwTotalFrameCountDec2;   /* total frame count*/
    UINT32 dwReserved2[3];          /* reserved field*/
    UINT32 dwDetailLogsDec2[16] ;   /* detail log for decoder2*/
    /* third decoder's field*/
    UINT32 dwFrameRepeatCountDec3;  /* repeat count for frames*/
    UINT32 dwFrameDropCountDec3;    /* drop count for frames*/
    UINT32 dwTotalErrorCountDec3;   /* total error count*/
    UINT32 dwTotalFrameCountDec3;   /* total frame count*/
    UINT32 dwReserved3[3];          /* reserved field*/
    UINT32 dwDetailLogsDec3[16] ;   /* detail log for decoder2*/
}ADSP_ERROR_LOG_T;

typedef struct
{
    BOOL fgDspRealPlay;
    BOOL fgDspPlay;
    BOOL fgDspStop;
    BOOL fgDspPause;
    BOOL fgSendSample;
    BOOL fgDspFlush;
    BOOL fgDspStopIssue;
    BOOL fgDspPtsSet;
    BOOL fgSendPts2Dsp;
    BOOL fgDspForceReset;
    BOOL fgDspWakeUp;
    BOOL fgDACOn;
    BOOL fgDspReParsing;
    BOOL fgDspRInt;
    BOOL fgDspDelayParsing;
    BOOL fgSPLINEnable;
    BOOL fgDspFirstPlay;
    BOOL fgDspUop;
    BOOL fgDspSInt;
    BOOL fgVDCodeDwnld;
    BOOL fgMicSfreqOver;
    BOOL fgIECRAWOff;
    BOOL fgDspRamCodeLoad;
    BOOL fgDspForceResetDec;
    BOOL fgDspWakeUpOk;
    BOOL fgDspGetBitrate;
    BOOL fgDspBitrateGot;
    BOOL fgDspGetMpgTyp;
    BOOL fgDspMpgTypGot;
    BOOL fgIecMuteState;
    BOOL fgMicOnState;
    BOOL fgDspDetected;
    BOOL fgDspAoutMuted;
}DSPSTATUS_T;

typedef struct
{
    UINT32 dwDspFreq;
    UINT32 dwDspRamCodeType;
    UINT8 bDspStrTyp;
    UINT32 dwADacFreq;
    UINT32 dwStcDiff;
    UINT32 dwDspReInitPts;
    UINT32 dwTableFreq;
    UINT32 dwDacFreq;
    UINT8 bCode3D;
    UINT8 bCode3DUpmix;
    UINT32 dwDspSIntFail;
    UINT32 dwVolPrevUsr;
    UINT32 dwVolLastSetting;
    UINT32 dwVolSettingStep;
    UINT32 dwDspForceResetCnt;
    UINT32 dwDspBitrate;
    #ifdef DSP_SUPPORT_NPTV
    UINT8  bMtsSrcTyp;
    #endif
    UINT8  b3DSurrMode;
    UINT32 dwDspMpgTyp;
    // For speed control case
    UINT32 dwDspAckPTS;
    UINT32 dwDspPlaySpeed;
    UINT32 dwDspUop;
    UINT8 bDetectTyp;
    SPEAKER_SETTING_T tSrcSpkCfg;
}DSPVARS_T;

// Top Level
typedef struct
{
    UINT8 u1IecOutBitNum;
    UINT8 u1IecChSel;
    BOOL fgIecMute;
    UINT8 u1IecDownSamp;
    UINT8 u1IecDecSel;
} AUDIO_CFG_T;

/* secondary decoder */
#define MAX_DSP_CMD_HIS_NS 16
typedef struct
{
    UINT32 u4Index;
    UINT32 u4Cmd[MAX_DSP_CMD_HIS_NS];
} DSP_CMD_HIS_T;

#define MAX_DSP_CMD_NS 5
typedef struct
{
    UINT8 ucWrIdx;
    UINT8 ucRdIdx;
    UINT8 ucCmdNs;
    UINT8 ucStatus;
    UINT32 u4Cmd[MAX_DSP_CMD_NS];
} DSP_CMD_T;

#ifdef MP3ENC_SUPPORT
typedef struct _PBS_BLK
{
  	UINT16 wSblk;
  	UINT16 wEblk;
  	UINT32 dwRdPtrAdr;
  	UINT32 dwWrPtrAdr;
} PBS_BLK_SET;

extern PBS_BLK_SET _tPbsBlk[2];
#endif

extern UINT32 _u4ShareMem;
extern UINT32 _u4DspBuf;
extern UINT32 _u4DspDramBuf[8];
extern UINT32 _u4DspCmptBuf[8];
extern UINT32 _u4AFIFO[4][2];
extern UINT32 _u4MixSndBuf;
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
extern UINT32 _u4AFIFOHdmiParser[2];
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
extern UINT32 _u4SBCEncBuf[2];
#endif

#ifdef DSP_SUPPORT_17BIT
extern UINT32 _u4DspICacheAddr[12]; //5368/96
#else
extern UINT32 _u4DspICacheAddr[8]; //5365
#endif

extern UINT8 _uDspState;
extern BOOL _fgRamBoot;
extern UINT32 _u4DspSIntLD;
extern UINT8 _uDspSIntAddr;
extern UINT32 _u4DspSIntSD;

extern volatile DSPSTATUS_T _rDspStatus[AUD_DEC_MAX];
extern DSPVARS_T _rDspVars[AUD_DEC_MAX];
extern DSP_CMD_HIS_T _rDspCmdHistory;

#endif // DSP_COMMON_H

