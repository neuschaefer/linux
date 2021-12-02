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
 * $RCSfile: rc2d_int.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file rc2d_int.h
 *  Brief of file rc2d_int.h.
 *  Details of file rc2d_int.h (optional).
 */
#ifndef RC2D_INT_H
#define RC2D_INT_H

/*********************( General Interrupt 0x00~0x7F )*********************/

//-------------------------------------------------------------------
//  CTRL_1
//-------------------------------------------------------------------
#define INT_CTRL1_BASE                          (0x00)
#define CTRL_1_NUM_1                             (19-1)

#define INTERRUPT_CTRL                          (INT_CTRL1_BASE + 0x00)
#define INT_RC2D_INTERRUPT_CTRL                 (INT_CTRL1_BASE + 0x00)
    #define RISC_WAKEUP_DSP                          0x000100
    #define RISC_WAKEUP_CODE                         (0x1 << 0)
    #define RAM_TIMER1                               (0x1 << 1)
    #define RAM_TIMER2                               (0x1 << 2)
    #define RAM_AOUT_INT                             (0x1 << 3)
    #define RAM_EXT_RISC_INT                         (0x1 << 4)
    #define RAM_RISC_INT                             (0x1 << 5)
    #define RAM_IEC_INT                              (0x1 << 6)
    #define RAM_8032_INT                             (0x1 << 7)
    #define RAM_RS232_INT                            (0x1 << 8)
    #define MAINCORE_ERR_HANG                        (0x1 << 9)
    #define TRAP_PRESET_ON                           (0x1 << 10)
    #define TRAP_SET_IN_DRAM                         (0x1 << 11)
    #define RAM_AOUT_INT2                            (0x1 << 12) 
    #define RAM_IEC_INT2                             (0x1 << 13)
    #define RAM_MIXSOUND_INT                         (0x1 << 14)
    #define RAM_MAIN_CORE                            (0x1 << 15)
#define INT_RC2D_SYSTEM_CTRL                    (INT_CTRL1_BASE + 0x01)
#define INT_RC2D_SYSTEM_SETUP                   (INT_CTRL1_BASE + 0x02)
    #define AIN_BUF_RD_NOT_HOLD_WR                   (1 << 8)    
    #define USE_STC_A1                               (1 << 14)
//Channel Trim Volume
// 0dB: 0x020000, 6dB: 0x03fd93h (approx. 0x040000)
// Normal channel: +10dB, Min Value: -10dB
// LFE channel: 0dB, Min Value: -10dB
#define INT_RC2D_CH_TRIM_C                      (INT_CTRL1_BASE + 0x04)
#define INT_RC2D_CH_TRIM_L                      (INT_CTRL1_BASE + 0x05)
#define INT_RC2D_CH_TRIM_R                      (INT_CTRL1_BASE + 0x06)
#define INT_RC2D_CH_TRIM_LS                      (INT_CTRL1_BASE + 0x07)
#define INT_RC2D_CH_TRIM_RS                     (INT_CTRL1_BASE + 0x08)
#define INT_RC2D_CH_TRIM_CH7                    (INT_CTRL1_BASE + 0x09)
#define INT_RC2D_CH_TRIM_CH8                    (INT_CTRL1_BASE + 0x0a)
#define INT_RC2D_CH_TRIM_SUBWOOFER              (INT_CTRL1_BASE + 0x0b)
#define INT_RC2D_CH_TRIM_CH9                    (INT_CTRL1_BASE + 0x0c)
#define INT_RC2D_CH_TRIM_CH10                   (INT_CTRL1_BASE + 0x0d)
#define INT_RC2D_CH_TRIM_LFE                    (INT_CTRL1_BASE + 0x0e)
#define INT_RC2D_DIALOGUE_GAIN                  (INT_CTRL1_BASE + 0x0f)
#define INT_RC2D_DIALOGUE_GAIN_DEC2             (INT_CTRL1_BASE + 0x10)
#define INT_RC2D_CH_TRIM_L_DEC2                 (INT_CTRL1_BASE + 0x11)
#define INT_RC2D_CH_TRIM_R_DEC2                 (INT_CTRL1_BASE + 0x12)
#define INT_RC2D_CH_TRIM_LFE_DEC2               (INT_CTRL1_BASE + 0x13)
    #define CH_VOL_SHM_DEFAULT          0x20000
    #define MASTER_VOL_SHM_DEFAULT      0x20000  // about 6 dB for AGC is on
#ifdef CC_MT5391_AUD_SUPPORT
#define CTRL_2_NUM_1                             (0xB-1)

#define INT_RC2D_STC_DIFF                        0x0020
        //
#define INT_RC2D_SPEED                           0x0021
        //
#define INT_RC2D_PROCESSING_MODE                 0x0022
        //
#define INT_RC2D_BIT_RESOLUTION                  0x0023
        //Value: 16~24 <<8
#define INT_RC2D_KARAOKE_FLAG                    0x0024
        //bit 0: L/R Mixing
        //bit 1: Voice Mute
        //bit 2: Vocal Assist
#define INT_RC2D_LRCH_MIX_RATIO                  0x0025
        //Value: 0x800000~0
#define INT_RC2D_STC_DIFF_LOBOUND                0x0026
#define INT_RC2D_STC_DIFF_HIBOUND                0x0027
#define INT_RC2D_STC_DIFF_WSBOUND                0x0028
//Volume
#define INT_RC2D_MASTER_VOLUME                   0x002c
#define AOUT_BANK_576_NUM                          0x400    //0x600?
#define AOUT_BANK_384_NUM                          0x700
#define AOUT_BANK_320_NUM                          0x800
#define AOUT_BANK_256_NUM                          0x900
#define ERROR_MUTE_BANK_NUM                        0x2EE00
#define VOLUME_LEVEL_UP_ORDER                      0x300
#define VOLUME_LEVEL_DOWN_ORDER                    0x600
#define INT_RC2D_SPDIF_FLAG                      0x002d
        //
//Bonding Options
#define INT_RC2D_STC_LEVEL_THRESHOLD             0x002e
#define INT_RC2D_DECODING_STR_PNT                0x002f
#else
//======= Ctrl_2 =========
#define CTRL_2_NUM_1                             (0xB-1)

#define INT_RC2D_STC_DIFF                        0x0020
        //
#define INT_RC2D_SPEED                           0x0021
        //
#define INT_RC2D_PROCESSING_MODE                 0x0022
        //
#define INT_RC2D_BIT_RESOLUTION                  0x0023
        //Value: 16~24 <<8
#define INT_RC2D_KARAOKE_FLAG                    0x0024
        //bit 0: L/R Mixing
        //bit 1: Voice Mute
        //bit 2: Vocal Assist
#define INT_RC2D_LRCH_MIX_RATIO                  0x0025
        //Value: 0x800000~0
#define INT_AVC_FLAG                             0x0026 
//Volume
#define INT_SMOOTH_VOLUME_DIFF                   0x0027
#define INT_SMOOTH_VOLUME_NO                     0x0028
#define INT_SMOOTH_VOLUME                        0x0029
#define INT_RC2D_MASTER_VOLUME                   0x002a
        //
#define INT_RC2D_SPDIF_FLAG                      0x002b
#define INT_RC2D_STC_LEVEL_THRESHOLD             0x002c
#define INT_RC2D_DECODING_STR_PNT                0x002d

#define AOUT_BANK_576_NUM                          0x400    //0x600?
#define AOUT_BANK_384_NUM                          0x700
#define AOUT_BANK_320_NUM                          0x800
#define AOUT_BANK_256_NUM                          0x900
#define ERROR_MUTE_BANK_NUM                        0x2EE00
#define VOLUME_LEVEL_UP_ORDER                      0x300
#define VOLUME_LEVEL_DOWN_ORDER                    0x600
#endif

//======= Ctrl_3 =========
#define CTRL_3_NUM_1                             (11-1)

#define INT_RC2D_BONDING_OPTIONS                 0x0030
//Channel Use -> Pink Noise
#define INT_RC2D_INT31                           0x0031
#define INT_RC2D_CHANNEL_USE                     0x0031
        //
//Dolby Karaoke Mode/LPCM channel assignment/CDDA deemphasis/WMA packet_no/Vorbis packet_no
#define INT_RC2D_INT32                           0x0032
        //OFF: 0x1 <<8, V1,V2: 0x2 <<8, None: 0x4 <<8
        //V1: 0x8 <<8, V2: 0x10 <<8, V1+V2: 0x20 <<8
//Dolby Dual Mode/LPCM frequency/HDCD config
#define INT_RC2D_INT33                           0x0033
        //Stereo: 0x0, Ch0 Only: 0x1 <<8
        //Ch1 Only: 0x2 <<8, Ch0+Ch1 Mixing: 0x3 <<8
//Dolby Compression/LPCM bit shift/HDCD upsampling
#define INT_RC2D_INT34                           0x0034
        //Custom Mode 0: 0x0, Custom Mode 1: 0x1 <<8
        //Line Mode: 0x2 <<8, RF Mode: 0x3 <<8
//Dolby dynamic low/DRC value/HDCD dither mode
#define INT_RC2D_INT35                           0x0035
        //Value: User Define
//Dolby dynamic high/LPCM q/DTS table_flag/CDDA check cbit/Vorbis table_flag
#define INT_RC2D_INT36                           0x0036
        //
//DTS/LPCM/MPEG Dynamic Range Control Flag/Dolby AC3 downmix/CDDA user data report
#define INT_RC2D_INT37                           0x0037
        //OFF: 0x0, AUTO: 0x1 <<8
//MPEG CRC Value/LPCM decoding type/SPDIF PCM downrate
#define INT_RC2D_INT38                           0x0038
        //Value: User Define
//LPCM ADPCM block align
#define INT_RC2D_INT39                           0x0039
        //
//LPCM ADPCM channel number
#define INT_RC2D_INT3A                           0x003A
        //
//LPCM ADPCM decoding type
#define INT_RC2D_INT3B                           0x003B

#ifdef CC_MT5391_AUD_SUPPORT
#define CTRL_4_NUM_1                             (9-1)

#define INT_RC2D_STC_DIFF_DEC2                   0x0040
        //
#define INT_RC2D_SPEED_DEC2                      0x0041
        //
#define INT_RC2D_PROCESSING_MODE_DEC2            0x0042
        //
#define INT_RC2D_BIT_RESOLUTION_DEC2             0x0043
        //Value: 16~24 <<8
#define INT_RC2D_KARAOKE_FLAG_DEC2               0x0044
        //bit 0: L/R Mixing
        //bit 1: Voice Mute
        //bit 2: Vocal Assist
#define INT_RC2D_LRCH_MIX_RATIO_DEC2             0x0045
        //Value: 0x800000~0
#define INT_RC2D_STC_DIFF_LOBOUND_DEC2           0x0046
#define INT_RC2D_STC_DIFF_HIBOUND_DEC2           0x0047
#define INT_RC2D_STC_DIFF_WSBOUND_DEC2           0x0048
//Volume
#define INT_RC2D_MASTER_VOLUME_DEC2              0x004c
  #define AOUT_BANK_576_NUM                        0x400    //0x600?
  #define AOUT_BANK_384_NUM                        0x700
  #define AOUT_BANK_320_NUM                        0x800
  #define AOUT_BANK_256_NUM                        0x900
  #define ERROR_MUTE_BANK_NUM                      0x2EE00
  #define VOLUME_LEVEL_UP_ORDER                    0x300
  #define VOLUME_LEVEL_DOWN_ORDER                  0x600
#define INT_RC2D_SPDIF_FLAG_DEC2                 0x004d
        //
//Bonding Options
#define INT_RC2D_STC_LEVEL_THRESHOLD_DEC2        0x004e
#define INT_RC2D_DECODING_STR_PNT_DEC2           0x004f
#else
//======= Ctrl_4 =========
#define CTRL_4_NUM_1                             (14-1)

#define INT_RC2D_STC_DIFF_DEC2                   0x0040
        //
#define INT_RC2D_SPEED_DEC2                      0x0041
        //
#define INT_RC2D_PROCESSING_MODE_DEC2            0x0042
        //
#define INT_RC2D_BIT_RESOLUTION_DEC2             0x0043
        //Value: 16~24 <<8
#define INT_RC2D_KARAOKE_FLAG_DEC2               0x0044
        //bit 0: L/R Mixing
        //bit 1: Voice Mute
        //bit 2: Vocal Assist
#define INT_RC2D_LRCH_MIX_RATIO_DEC2             0x0045
        //Value: 0x800000~0
#define INT_AVC_FLAG_DEC2                        0x0046
//Volume
#define INT_SMOOTH_VOLUME_DIFF_DEC2              0x0047
#define INT_SMOOTH_VOLUME_NO_DEC2                0x0048
#define INT_SMOOTH_VOLUME_DEC2                   0x0049
#define INT_RC2D_MASTER_VOLUME_DEC2              0x004a
        //
#define INT_RC2D_SPDIF_FLAG_DEC2                 0x004b
#define INT_RC2D_STC_LEVEL_THRESHOLD_DEC2        0x004c
#define INT_RC2D_DECODING_STR_PNT_DEC2           0x004d

  #define AOUT_BANK_576_NUM                        0x400    //0x600?
  #define AOUT_BANK_384_NUM                        0x700
  #define AOUT_BANK_320_NUM                        0x800
  #define AOUT_BANK_256_NUM                        0x900
  #define ERROR_MUTE_BANK_NUM                      0x2EE00
  #define VOLUME_LEVEL_UP_ORDER                    0x300
  #define VOLUME_LEVEL_DOWN_ORDER                  0x600
#endif
//======= Ctrl_5 =========
#define CTRL_5_NUM_1                             (12-1)

#define INT_RC2D_BONDING_OPTIONS_DEC2            0x0050
//Channel Use -> Pink Noise
#define INT_RC2D_INT51                           0x0051
#define INT_RC2D_CHANNEL_USE_DEC2                0x0051
        //
//Dolby Karaoke Mode/LPCM channel assignment/CDDA deemphasis/WMA packet_no/Vorbis packet_no
#define INT_RC2D_INT52                           0x0052
        //OFF: 0x1 <<8, V1,V2: 0x2 <<8, None: 0x4 <<8
        //V1: 0x8 <<8, V2: 0x10 <<8, V1+V2: 0x20 <<8
//Dolby Dual Mode/LPCM frequency/HDCD config
#define INT_RC2D_INT53                           0x0053
        //Stereo: 0x0, Ch0 Only: 0x1 <<8
        //Ch1 Only: 0x2 <<8, Ch0+Ch1 Mixing: 0x3 <<8
//Dolby Compression/LPCM bit shift/HDCD upsampling
#define INT_RC2D_INT54                           0x0054
        //Custom Mode 0: 0x0, Custom Mode 1: 0x1 <<8
        //Line Mode: 0x2 <<8, RF Mode: 0x3 <<8
//Dolby dynamic low/DRC value/HDCD dither mode
#define INT_RC2D_INT55                           0x0055
        //Value: User Define
//Dolby dynamic high/LPCM q/DTS table_flag/CDDA check cbit/Vorbis table_flag
#define INT_RC2D_INT56                           0x0056
        //
//DTS/LPCM/MPEG Dynamic Range Control Flag/Dolby AC3 downmix/CDDA user data report
#define INT_RC2D_INT57                           0x0057
        //OFF: 0x0, AUTO: 0x1 <<8
//MPEG CRC Value/LPCM decoding type/SPDIF PCM downrate
#define INT_RC2D_INT58                           0x0058
        //Value: User Define
//LPCM ADPCM block align
#define INT_RC2D_INT59                           0x0059
        //
//LPCM ADPCM channel number
#define INT_RC2D_INT5A                           0x005A
        //
//LPCM ADPCM decoding type
#define INT_RC2D_INT5B                           0x005B
        //

#ifdef DSP_SUPPORT_NPTV
  //===============================================================================
  //
  // CTRL_6
  //
  // NPTV input control variable for demodulation
  //
  //===============================================================================
#define INT_CTRL6_BASE             (0x60)
#define CTRL_6_NUM_1               (13)

#define INT_PARAM_INT50            (INT_CTRL6_BASE)
#define INT_PARAM_INT51            (INT_CTRL6_BASE+0x01)
#define INT_PARAM_INT52            (INT_CTRL6_BASE+0x02)
#define INT_PARAM_INT53            (INT_CTRL6_BASE+0x03)
#define INT_PARAM_INT54            (INT_CTRL6_BASE+0x04)
#define INT_PARAM_INT55            (INT_CTRL6_BASE+0x05)
#define INT_PARAM_INT56            (INT_CTRL6_BASE+0x06)
#define INT_PARAM_INT57            (INT_CTRL6_BASE+0x07)
#define INT_PARAM_INT58            (INT_CTRL6_BASE+0x08)
#define INT_PARAM_INT59            (INT_CTRL6_BASE+0x09)
#define INT_PARAM_INT5A            (INT_CTRL6_BASE+0x0A)
#define INT_PARAM_INT5B            (INT_CTRL6_BASE+0x0B)
#define INT_PARAM_INT5C            (INT_CTRL6_BASE+0x0C)
#define INT_PARAM_INT5D            (INT_CTRL6_BASE+0x0D)
#define INT_PARAM_INT5E            (INT_CTRL6_BASE+0x0E)
#endif
/*********************( Specific Interrupt 0x80~0xFF )*********************/

//======= RAM Interrupt (0x80~0xBF) =========

#define INT_RC2D_READ_DRAM_PAGE_3                0x00BB         // 187
#define INT_RC2D_READ_DRAM_PAGE_2                0x00BC         // 188
#define INT_RC2D_READ_DRAM_PAGE_1                0x00BD         // 189
#define INT_RC2D_READ_DRAM_PAGE_0                0x00BE         // 190
#define INT_RC2D_ROM_WAIT_RAM                    0x00BF         // 191

#ifdef CC_MT5391_AUD_SUPPORT
//======= Ctrl_7 =========
#define CTRL_7_NUM_1                             (9-1)

#define INT_RC2D_STC_DIFF_DEC3                   0x0080
        //
#define INT_RC2D_SPEED_DEC3                      0x0081
        //
#define INT_RC2D_PROCESSING_MODE_DEC3            0x0082
        //
#define INT_RC2D_BIT_RESOLUTION_DEC3             0x0083
        //Value: 16~24 <<8
#define INT_RC2D_KARAOKE_FLAG_DEC3               0x0084
        //bit 0: L/R Mixing
        //bit 1: Voice Mute
        //bit 2: Vocal Assist
#define INT_RC2D_LRCH_MIX_RATIO_DEC3             0x0085
        //Value: 0x800000~0
#define INT_RC2D_STC_DIFF_LOBOUND_DEC3           0x0086
#define INT_RC2D_STC_DIFF_HIBOUND_DEC3           0x0087
#define INT_RC2D_STC_DIFF_WSBOUND_DEC3           0x0088
//Volume
#define INT_RC2D_MASTER_VOLUME_DEC3              0x008c
  #define AOUT_BANK_576_NUM                        0x400    //0x600?
  #define AOUT_BANK_384_NUM                        0x700
  #define AOUT_BANK_320_NUM                        0x800
  #define AOUT_BANK_256_NUM                        0x900
  #define ERROR_MUTE_BANK_NUM                      0x2EE00
  #define VOLUME_LEVEL_UP_ORDER                    0x300
  #define VOLUME_LEVEL_DOWN_ORDER                  0x600
#define INT_RC2D_SPDIF_FLAG_DEC3                 0x008d
        //
//Bonding Options
#define INT_RC2D_STC_LEVEL_THRESHOLD_DEC3        0x008e
#define INT_RC2D_DECODING_STR_PNT_DEC3           0x008f

//======= Ctrl_8 =========
#define CTRL_8_NUM_1                             (12-1)

#define INT_RC2D_BONDING_OPTIONS_DEC3            0x0090
//Channel Use -> Pink Noise
#define INT_RC2D_INT91                           0x0091
#define INT_RC2D_CHANNEL_USE_DEC3                0x0091
        //
//Dolby Karaoke Mode/LPCM channel assignment/CDDA deemphasis/WMA packet_no/Vorbis packet_no
#define INT_RC2D_INT92                           0x0092
        //OFF: 0x1 <<8, V1,V2: 0x2 <<8, None: 0x4 <<8
        //V1: 0x8 <<8, V2: 0x10 <<8, V1+V2: 0x20 <<8
//Dolby Dual Mode/LPCM frequency/HDCD config
#define INT_RC2D_INT93                           0x0093
        //Stereo: 0x0, Ch0 Only: 0x1 <<8
        //Ch1 Only: 0x2 <<8, Ch0+Ch1 Mixing: 0x3 <<8
//Dolby Compression/LPCM bit shift/HDCD upsampling
#define INT_RC2D_INT94                           0x0094
        //Custom Mode 0: 0x0, Custom Mode 1: 0x1 <<8
        //Line Mode: 0x2 <<8, RF Mode: 0x3 <<8
//Dolby dynamic low/DRC value/HDCD dither mode
#define INT_RC2D_INT95                           0x0095
        //Value: User Define
//Dolby dynamic high/LPCM q/DTS table_flag/CDDA check cbit/Vorbis table_flag
#define INT_RC2D_INT96                           0x0096
        //
//DTS/LPCM/MPEG Dynamic Range Control Flag/Dolby AC3 downmix/CDDA user data report
#define INT_RC2D_INT97                           0x0097
        //OFF: 0x0, AUTO: 0x1 <<8
//MPEG CRC Value/LPCM decoding type/SPDIF PCM downrate
#define INT_RC2D_INT98                           0x0098
        //Value: User Define
//LPCM ADPCM block align
#define INT_RC2D_INT99                           0x0099
        //
//LPCM ADPCM channel number
#define INT_RC2D_INT9A                           0x009A
        //
//LPCM ADPCM decoding type
#define INT_RC2D_INT9B                           0x009B
        //
#else
//======= Ctrl_7 =========
#define INT_CTRL7_BASE                          (0x80)
#define CTRL_7_NUM_1                             (13-1)

#define INT_RC2D_STC_DIFF_DEC3                  (INT_CTRL7_BASE + 0x00)
        //
#define INT_RC2D_PROCESSING_MODE_DEC3           (INT_CTRL7_BASE + 0x01)
//Volume
#define INT_RC2D_SMOOTH_VOLUME_DIFF_DEC3        (INT_CTRL7_BASE + 0x02)
#define INT_RC2D_SMOOTH_VOLUME_NO_DEC3          (INT_CTRL7_BASE + 0x03)
//#define INT_RC2D_SMOOTH_VOLUME_DEC3             (INT_CTRL7_BASE + 0x04)
        //Max Value: 0x7FFFFF
        //Min Value: 0x000001
        //Value 0dB: 0x020000
        //Value 6dB: 0x03fd93h (approx. 0x040000)
#define INT_RC2D_MASTER_VOLUME_DEC3              (INT_CTRL7_BASE + 0x05)

#define INT_RC2D_STC_LEVEL_THRESHOLD_DEC3        (INT_CTRL7_BASE + 0x06)
#define INT_RC2D_DECODING_STR_PNT_DEC3           (INT_CTRL7_BASE + 0x07)
#endif

//======= Background Interrupt (0xC0~0xCF) =========
#define INT_RC2D_BUFPNT_RESET_DEC2               0x00CA         // 202
//DTS Table Interrupt
#define INT_RC2D_DTS_TABLE_DEC2                  0x00CB         // 203
#define INT_RC2D_IEC_FLAG_DEC2                   0x00CC         // 204
        //OFF: 0x0, ON: 0x1 <<8
        //Value: 48 <<8 ~0
        //OFF: 0x0, ON: 0x1 <<8
#define INT_RC2D_PTS_RESET_DEC2                  0x00CD         // 205
//PTS
#define INT_RC2D_PTS_DEC2                        0x00CE         // 206
        //#define DRAM_PTS_INDEX_NUM_M3       0x5D00
        //Value: 3, 6, 9, ... 93 <<8 max.
        //INDEX_NUM_M3/3 should be greater than PTS Max. number in bitstream buffer
#define INT_RC2D_SMOOTH_VOLUME_DEC2              0x00CF         // 207
        //Max Value: 0x7FFFFF
        //Min Value: 0x000001
        //Value 0dB: 0x020000
        //Value 6dB: 0x03fd93h (approx. 0x040000)

//======= Foreground Interrupt (0xD0~0xDF) =========
//Key Shift
#define INT_RC2D_KEYSHIFT_LEVEL                  0x00D9         // 217
        //Value: +16~-16 <<8 (up 8 degree to down 8 detree, 0.5degree/level)
#define INT_RC2D_BUFPNT_RESET                    0x00DA         // 218
//DTS Table Interrupt
#define INT_RC2D_DTS_TABLE                       0x00DB         // 219
  #define DTS_TABLE_ADPCM_READY                    0x000100
  #define DTS_TABLE_VQ_READY                       0x000200
  #define DTS_TABLE_ADPCM96_READY                  0x000400
  #define DTS_TABLE_VQ96_READY                     0x000800
  //LPCM Table
  #define LPCM_PARAMETER_READY                     0x000200
//Vorbis Table Interrupt
#define INT_RC2D_VORBIS_TABLE                    INT_RC2D_DTS_TABLE    // 219
  #define VORBIS_CODEBOOK_READY                    0x000100

#define INT_RC2D_IEC_FLAG                        0x00DC         // 220
        //OFF: 0x0, ON: 0x1 <<8
        //Value: 48 <<8 ~0
        //OFF: 0x0, ON: 0x1 <<8
#define INT_RC2D_PTS_RESET                       0x00DD         // 221
//PTS
#define INT_RC2D_PTS                             0x00DE         // 222
        //#define DRAM_PTS_INDEX_NUM_M3       0x5D00
        //Value: 3, 6, 9, ... 93 <<8 max.
        //INDEX_NUM_M3/3 should be greater than PTS Max. number in bitstream buffer
#define INT_RC2D_SMOOTH_VOLUME                   0x00DF         // 223
        //Max Value: 0x7FFFFF
        //Min Value: 0x000001
        //Value 0dB: 0x020000
        //Value 6dB: 0x03fd93h (approx. 0x040000)
//======= Background Interrupt (0xE0~0xEF) =========
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
#define INT_RC2D_IEC_FLAG_DEC3                   0x00EC         // 236
        //OFF: 0x0, ON: 0x1 <<8
        //Value: 48 <<8 ~0
        //OFF: 0x0, ON: 0x1 <<8
#define INT_RC2D_PTS_RESET_DEC3                 0x00ED         // 205
//PTS
#define INT_RC2D_PTS_DEC3                        0x00EE         // 206        
#define INT_RC2D_SMOOTH_VOLUME_DEC3              0x00EF         // 239
        //Max Value: 0x7FFFFF
        //Min Value: 0x000001
        //Value 0dB: 0x020000
        //Value 6dB: 0x03fd93h (approx. 0x040000)
#define INT_RC2D_FLOW_CONTROL_DEC3               0x00EB         // 235
#define INT_RC2D_BUFPNT_RESET_DEC3                0x00E9         // 233
        // temporary define by gallen
#endif        

//======= Common Interrupt (0xF0~0xFF) =========
#define INT_RC2D_ROM_WAIT                        0x00F3         // 243
  #define WAIT_CMD                                 0x1
  #define RETURN_CMD                               0x0
#define INT_RC2D_MIXSOUND_FLAG                   0x00F6         // 246
  #define MIXSOUND_PLAY                            0x0001
  #define MIXSOUND_STOP                            0x0002
  #define MIXSOUND_FLUSH_2_PLAY                    0x0003
  #define MIXSOUND_BREAK_2_PLAY                    0x0004
        //RISC Read DSP Memory
#define INT_RC2D_READ_DSP_MEMORY                 0x00F7         // 247
//RISC Write DSP Memory
#define INT_RC2D_WRITE_DSP_MEMORY                0x00F8         // 248
#define INT_RC2D_XOR                             0x00F9         // 249
#define INT_RC2D_OR                              0x00FA         // 250
#define INT_RC2D_AND                             0x00FB         // 251
#define INT_RC2D_DSP_JOB_RESET                   0x00FC         // 252
  #define JOB_RESET_DECODER                        0x1000       // for job0
  #define JOB_RESET_DECODER2                       0x2000       // for job2
  #define JOB_RESET_JOB3                           0x4000       // for job3
  #define JOB_RESET_DSP                            0x8000       // for all jobs
//Flow Control Flag
#define INT_RC2D_FLOW_CONTROL_GENERAL            0x00FD         // 253
//Flow Control Flag
#define INT_RC2D_FLOW_CONTROL_DEC2               0x00FE         // 254
//Flow Control Flag
#define INT_RC2D_FLOW_CONTROL                    0x00FF         // 255
        //Play: 0x2 <<8, Stop: 0x1 <<8, Flush: 0x4 <<8
  #define FLOW_CONTROL_STOP                        0x0100
  #define FLOW_CONTROL_PLAY                        0x4200
  #define FLOW_CONTROL_FLUSH                       0x0400
  #define FLOW_CONTROL_PAUSE                       0x10800
  #define FLOW_CONTROL_RESUME                      0x20000
//  #define FLOW_CONTROL_RESET                       0x800000
  #define FLOW_CONTROL_AOUT_REINIT                 0x0800
  #define FLOW_CONTROL_SAMPLING_RATE               0x1000
  #define FLOW_CONTROL_AOUT_WAIT_ENABLE            0x40000
  #define FLOW_CONTROL_AOUT_ENABLE                 0x80000
  #define FLOW_CONTROL_TRANSPARENT_REINIT          0x100000

#endif // RC2D_INT_H

