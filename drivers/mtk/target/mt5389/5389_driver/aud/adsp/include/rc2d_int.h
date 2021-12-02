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
 * $RCSfile: rc2d_int.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

#ifdef AUD_EXTERN_FILE
#error "Please don't include this header file in release C file"
#endif

/** @file rc2d_int.h
 *  Brief of file rc2d_int.h.
 *  Details of file rc2d_int.h (optional).
 */
#ifndef RC2D_INT_H
#define RC2D_INT_H

/*********************( General Interrupt 0x00~0x7F )*********************/
//-------------------------------------------------------------------
//  CTRL_1 Interrupt (0x00~0x1F)
//-------------------------------------------------------------------
#define INT_CTRL1_BASE                          (0x00)
#define CTRL_1_NUM_1                            (0x1B-1)

#define INT_RC2D_INTERRUPT_CTRL                 (INT_CTRL1_BASE + 0x00)
    #define RISC_WAKEUP_DSP                     0x000100
    #define RISC_WAKEUP_CODE                    (0x1 << 0)
    #define RAM_TIMER1                          (0x1 << 1)
    #define RAM_TIMER2                          (0x1 << 2)
    #define RAM_AOUT_INT                        (0x1 << 3)
    #define RAM_EXT_RISC_INT                    (0x1 << 4)
    #define RAM_RISC_INT                        (0x1 << 5)
    #define RAM_IEC_INT                         (0x1 << 6)
    #define RAM_8032_INT                        (0x1 << 7)
    #define RAM_RS232_INT                       (0x1 << 8)
    #define MAINCORE_ERR_HANG                   (0x1 << 9)
    #define TRAP_PRESET_ON                      (0x1 << 10)
    #define TRAP_SET_IN_DRAM                    (0x1 << 11)
    #define RAM_AOUT_INT2                       (0x1 << 12) 
    #define RAM_IEC_INT2                        (0x1 << 13)
    #define RAM_MIXSOUND_INT                    (0x1 << 14)
    #define RAM_MAIN_CORE                       (0x1 << 15)
#define INT_RC2D_SYSTEM_CTRL                    (INT_CTRL1_BASE + 0x01)
#define INT_RC2D_SYSTEM_CTRL2                   (INT_CTRL1_BASE + 0x02)
#define INT_RC2D_SYSTEM_SETUP                   (INT_CTRL1_BASE + 0x03)
    #define AIN_BUF_RD_NOT_HOLD_WR              (0x1U << 8)    
    #define AD_FADE_POSITION                    (0x1U << 9)
    #define COPY_CH78_TO_LS_RS                  (0x1U << 12)
    #define USE_STC_A1                          (0x1U << 14)
    #define SYSTEM_MASTER                       (0x1U << 15)
#define INT_RC2D_SYSTEM_SETUP_DEC2              (INT_CTRL1_BASE + 0x04) // 2_STC_Task
#define INT_RC2D_SYSTEM_SETUP_DEC3              (INT_CTRL1_BASE + 0x15)

//Channel Trim Volume
    // 0dB: 0x020000, 6dB: 0x03fd93h (approx. 0x040000)
    // Normal channel: +10dB, Min Value: -10dB
    // LFE channel: 0dB, Min Value: -10dB
#define INT_RC2D_CH_TRIM_C                      (INT_CTRL1_BASE + 0x05)
#define INT_RC2D_CH_TRIM_L                      (INT_CTRL1_BASE + 0x06)
#define INT_RC2D_CH_TRIM_R                      (INT_CTRL1_BASE + 0x07)
#define INT_RC2D_CH_TRIM_LS                     (INT_CTRL1_BASE + 0x08)
#define INT_RC2D_CH_TRIM_RS                     (INT_CTRL1_BASE + 0x09)
#define INT_RC2D_CH_TRIM_CH7                    (INT_CTRL1_BASE + 0x0a)
#define INT_RC2D_CH_TRIM_CH8                    (INT_CTRL1_BASE + 0x0b)
#define INT_RC2D_CH_TRIM_SUBWOOFER              (INT_CTRL1_BASE + 0x0c)
#define INT_RC2D_CH_TRIM_CH9                    (INT_CTRL1_BASE + 0x0d)
#define INT_RC2D_CH_TRIM_CH10                   (INT_CTRL1_BASE + 0x0e)
#define INT_RC2D_CH_TRIM_LFE                    (INT_CTRL1_BASE + 0x0f)
#define INT_RC2D_DIALOGUE_GAIN                  (INT_CTRL1_BASE + 0x10)
#define INT_RC2D_DIALOGUE_GAIN_DEC2             (INT_CTRL1_BASE + 0x11)
#define INT_RC2D_CH_TRIM_L_DEC2                 (INT_CTRL1_BASE + 0x12)
#define INT_RC2D_CH_TRIM_R_DEC2                 (INT_CTRL1_BASE + 0x13)
#define INT_RC2D_CH_TRIM_LFE_DEC2               (INT_CTRL1_BASE + 0x14)
    #define CH_VOL_SHM_DEFAULT                  0x20000 
    #define MASTER_VOL_SHM_DEFAULT              0x20000  // about 6 dB for AGC is on

//-------------------------------------------------------------------
//  CTRL_2 Interrupt (0x20~0x2F)
//-------------------------------------------------------------------
#define INT_CTRL2_BASE                          (0x20)
#define CTRL_2_NUM_1                            (0x06-1)

#define INT_RC2D_STC_DIFF                       (INT_CTRL2_BASE + 0x00)
#define INT_RC2D_SPEED                          (INT_CTRL2_BASE + 0x01)
#define INT_RC2D_PROCESSING_MODE                (INT_CTRL2_BASE + 0x02)
// Value: 16~24 <<8
#define INT_RC2D_BIT_RESOLUTION                 (INT_CTRL2_BASE + 0x03)
// bit 0: L/R Mixing
// bit 1: Voice Mute
// bit 2: Vocal Assist
#define INT_RC2D_KARAOKE_FLAG                   (INT_CTRL2_BASE + 0x04)
// Value: 0x800000~0
#define INT_RC2D_LRCH_MIX_RATIO                 (INT_CTRL2_BASE + 0x05)
#define INT_SMOOTH_VOLUME_DIFF                  (INT_CTRL2_BASE + 0x07)
#define INT_SMOOTH_VOLUME_NO                    (INT_CTRL2_BASE + 0x08)
#define INT_SMOOTH_VOLUME                       (INT_CTRL2_BASE + 0x09)
#define INT_RC2D_MASTER_VOLUME                  (INT_CTRL2_BASE + 0x0A)
#define INT_RC2D_SPDIF_FLAG                     (INT_CTRL2_BASE + 0x0B)
#define INT_RC2D_STC_LEVEL_THRESHOLD            (INT_CTRL2_BASE + 0x0C)
#define INT_RC2D_DECODING_STR_PNT               (INT_CTRL2_BASE + 0x0D)

//-------------------------------------------------------------------
//  CTRL_3 Interrupt (0x30~0x3F)
//-------------------------------------------------------------------
#define INT_CTRL3_BASE                          (0x30)
#define CTRL_3_NUM_1                            (0x0C-1)

#define INT_RC2D_BONDING_OPTIONS                (INT_CTRL3_BASE + 0x00)
// Channel Use -> Pink Noise
#define INT_RC2D_CHANNEL_USE                    (INT_CTRL3_BASE + 0x01)
#define INT_RC2D_INT32                          (INT_CTRL3_BASE + 0x02)
#define INT_RC2D_INT33                          (INT_CTRL3_BASE + 0x03)
#define INT_RC2D_INT34                          (INT_CTRL3_BASE + 0x04)
#define INT_RC2D_INT35                          (INT_CTRL3_BASE + 0x05)
#define INT_RC2D_INT36                          (INT_CTRL3_BASE + 0x06)
#define INT_RC2D_INT37                          (INT_CTRL3_BASE + 0x07)
#define INT_RC2D_INT38                          (INT_CTRL3_BASE + 0x08)
#define INT_RC2D_INT39                          (INT_CTRL3_BASE + 0x09)
#define INT_RC2D_INT3A                          (INT_CTRL3_BASE + 0x0A)

//-------------------------------------------------------------------
//  CTRL_4 Interrupt (0x40~0x4F)
//-------------------------------------------------------------------
#define INT_CTRL4_BASE                          (0x40)
#define CTRL_4_NUM_1                            (0x06-1)

#define INT_RC2D_STC_DIFF_DEC2                  (INT_CTRL4_BASE + 0x00)
#define INT_RC2D_SPEED_DEC2                     (INT_CTRL4_BASE + 0x01)
#define INT_RC2D_PROCESSING_MODE_DEC2           (INT_CTRL4_BASE + 0x02)
// Value: 16~24 <<8
#define INT_RC2D_BIT_RESOLUTION_DEC2            (INT_CTRL4_BASE + 0x03)
// bit 0: L/R Mixing
// bit 1: Voice Mute
// bit 2: Vocal Assist
#define INT_RC2D_KARAOKE_FLAG_DEC2              (INT_CTRL4_BASE + 0x04)
// Value: 0x800000~0
#define INT_RC2D_LRCH_MIX_RATIO_DEC2            (INT_CTRL4_BASE + 0x05)
#define INT_SMOOTH_VOLUME_DIFF_DEC2             (INT_CTRL4_BASE + 0x07)
#define INT_SMOOTH_VOLUME_NO_DEC2               (INT_CTRL4_BASE + 0x08)
#define INT_SMOOTH_VOLUME_DEC2                  (INT_CTRL4_BASE + 0x09)
#define INT_RC2D_MASTER_VOLUME_DEC2             (INT_CTRL4_BASE + 0x0A)
#define INT_RC2D_SPDIF_FLAG_DEC2                (INT_CTRL4_BASE + 0x0B)
#define INT_RC2D_STC_LEVEL_THRESHOLD_DEC2       (INT_CTRL4_BASE + 0x0C)
#define INT_RC2D_DECODING_STR_PNT_DEC2          (INT_CTRL4_BASE + 0x0D)

//-------------------------------------------------------------------
//  CTRL_5 Interrupt (0x50~0x5F)
//-------------------------------------------------------------------
#define INT_CTRL5_BASE                          (0x50)
#define CTRL_5_NUM_1                            (0x0A-1)

#define INT_RC2D_BONDING_OPTIONS_DEC2           (INT_CTRL5_BASE + 0x00)
//Channel Use -> Pink Noise
#define INT_RC2D_CHANNEL_USE_DEC2               (INT_CTRL5_BASE + 0x01)
#define INT_RC2D_INT52                          (INT_CTRL5_BASE + 0x02)
#define INT_RC2D_INT53                          (INT_CTRL5_BASE + 0x03)
#define INT_RC2D_INT54                          (INT_CTRL5_BASE + 0x04)
#define INT_RC2D_INT55                          (INT_CTRL5_BASE + 0x05)
#define INT_RC2D_INT56                          (INT_CTRL5_BASE + 0x06)
#define INT_RC2D_INT57                          (INT_CTRL5_BASE + 0x07)
#define INT_RC2D_INT58                          (INT_CTRL5_BASE + 0x08)
#define INT_RC2D_INT59                          (INT_CTRL5_BASE + 0x09)
#define INT_RC2D_INT5A                          (INT_CTRL5_BASE + 0x0A)

//-------------------------------------------------------------------
//  CTRL_6 Interrupt (0x60~0x6F)
//-------------------------------------------------------------------
#define INT_CTRL6_BASE                          (0x60)
#define CTRL_6_NUM_1                            (0x02-1)

#define INT_RC2D_STC_DIFF_DEC3                  (INT_CTRL6_BASE + 0x00)
#define INT_RC2D_PROCESSING_MODE_DEC3           (INT_CTRL6_BASE + 0x01)
#define INT_SMOOTH_VOLUME_DIFF_DEC3             (INT_CTRL6_BASE + 0x02)
#define INT_SMOOTH_VOLUME_NO_DEC3               (INT_CTRL6_BASE + 0x03)
#define INT_SMOOTH_VOLUME_DEC3                  (INT_CTRL6_BASE + 0x04)
#define INT_RC2D_MASTER_VOLUME_DEC3             (INT_CTRL6_BASE + 0x05)
#define INT_RC2D_STC_LEVEL_THRESHOLD_DEC3       (INT_CTRL6_BASE + 0x06)
#define INT_RC2D_DECODING_STR_PNT_DEC3          (INT_CTRL6_BASE + 0x07)
#define INT_RC2D_INT68                          (INT_CTRL6_BASE + 0x08)
#define INT_RC2D_INT69                          (INT_CTRL6_BASE + 0x09)
#define INT_RC2D_INT6A                          (INT_CTRL6_BASE + 0x0A)
#define INT_RC2D_INT6B                          (INT_CTRL6_BASE + 0x0B)
#define INT_RC2D_INT6C                          (INT_CTRL6_BASE + 0x0C)
#define INT_RC2D_INT6D                          (INT_CTRL6_BASE + 0x0D)
#define INT_RC2D_INT6E                          (INT_CTRL6_BASE + 0x0E)
#define INT_RC2D_INT6F                          (INT_CTRL6_BASE + 0x0F)

//-------------------------------------------------------------------
//  CTRL_7 Interrupt (0x70~0x7F)
//-------------------------------------------------------------------
#define INT_CTRL7_BASE                          (0x70)
#define CTRL_7_NUM_1                            (0x01-1)

#define INT_RC2D_STC_DIFF_DEC4                  (INT_CTRL7_BASE + 0x00)
#define INT_RC2D_PROCESSING_MODE_DEC4           (INT_CTRL7_BASE + 0x01)

/*********************( Specific Interrupt 0x80~0xFF )*********************/
//-------------------------------------------------------------------
//  Dec4 Interrupt (0x80~0x8F)
//-------------------------------------------------------------------
//#ifdef CC_AUD_4_DECODER_SUPPORT
#define INT_RC2D_FLOW_CONTROL_DEC4              0x008F         // 143
#define INT_RC2D_DECODER_CONTROL_DEC4           0x008E         // 142 (no use now)
#define INT_RC2D_SMOOTH_VOLUME_DEC4             0x008D         // 141
//#endif

//-------------------------------------------------------------------
//  Reserved (0x90~0x9F)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//  RAM Interrupt (0xA0~0xBF)
//-------------------------------------------------------------------
#define INT_RC2D_READ_DRAM_PAGE_3               0x00BB         // 187
#define INT_RC2D_READ_DRAM_PAGE_2               0x00BC         // 188
#define INT_RC2D_READ_DRAM_PAGE_1               0x00BD         // 189
#define INT_RC2D_READ_DRAM_PAGE_0               0x00BE         // 190
#define INT_RC2D_ROM_WAIT_RAM                   0x00BF         // 191

//-------------------------------------------------------------------
//  Background Interrupt (0xC0~0xCF)
//-------------------------------------------------------------------
#define INT_RC2D_BUFPNT_RESET_DEC2              0x00CA         // 202
// DTS Table Interrupt
#define INT_RC2D_DTS_TABLE_DEC2                 0x00CB         // 203
#define INT_RC2D_PTS_RESET_DEC2                 0x00CC         // 204
// Value: 3, 6, 9, ... 93 <<8 max.
// INDEX_NUM_M3/3 should be greater than PTS Max. number in bitstream buffer
#define INT_RC2D_DECODER_CONTROL_DEC2           0x00CD         // 205
#define INT_RC2D_PTS_DEC2                       0x00CE         // 206
#define INT_RC2D_SMOOTH_VOLUME_DEC2             0x00CF         // 207

//-------------------------------------------------------------------
//  Foreground Interrupt (0xD0~0xDF)
//-------------------------------------------------------------------
// Value: +16~-16 <<8 (up 8 degree to down 8 detree, 0.5degree/level)
#define INT_RC2D_BUFPNT_RESET                   0x00DA         // 218
//DTS Table Interrupt
#define INT_RC2D_DTS_TABLE                      0x00DB         // 219
    #define DTS_TABLE_ADPCM_READY               0x000100
    #define DTS_TABLE_VQ_READY                  0x000200
    #define DTS_TABLE_ADPCM96_READY             0x000400
    #define DTS_TABLE_VQ96_READY                0x000800
    //LPCM Table
    #define LPCM_PARAMETER_READY                0x000200
    //Vorbis Table Interrupt
#define INT_RC2D_VORBIS_TABLE                   INT_RC2D_DTS_TABLE  ///CC_VORBIS_SUPPORT
    #define VORBIS_CODEBOOK_READY               0x000100    ///Dec1: OK
    #define VORBIS_CODEBOOK_READY_DEC2          0x000200    ///Dec2: OK

#define INT_RC2D_PTS_RESET                      0x00DC         // 220
// Value: 3, 6, 9, ... 93 <<8 max.
// INDEX_NUM_M3/3 should be greater than PTS Max. number in bitstream buffer
#define INT_RC2D_DECODER_CONTROL                0x00DD         // 221
#define INT_RC2D_PTS                            0x00DE         // 222
#define INT_RC2D_SMOOTH_VOLUME                  0x00DF         // 223

//-------------------------------------------------------------------
//  Dec3 Interrupt (0xE0~0xEF)
//-------------------------------------------------------------------
//#ifdef CC_MT5391_AUD_3_DECODER
#define INT_RC2D_BUFPNT_RESET_DEC3              0x00EA         // 234
#define INT_RC2D_PTS_RESET_DEC3                 0x00EB         // 235

#define INT_RC2D_DECODER_CONTROL_DEC3           0x00EC         // 236
#define INT_RC2D_FLOW_CONTROL_DEC3              0x00ED         // 237
#define INT_RC2D_PTS_DEC3                       0x00EE         // 238 
#define INT_RC2D_SMOOTH_VOLUME_DEC3             0x00EF         // 239
//#endif        

//-------------------------------------------------------------------
//  Common Interrupt (0xF0~0xFF)
//-------------------------------------------------------------------
#define INT_RC2D_ROM_WAIT                       0x00F3         // 243
    #define WAIT_CMD                            0x1
    #define RETURN_CMD                          0x0
#define INT_RC2D_MIXSOUND_FLAG                  0x00F6         // 246
    #define MIXSOUND_PLAY                       0x0001
    #define MIXSOUND_STOP                       0x0002
    #define MIXSOUND_FLUSH_2_PLAY               0x0003
    #define MIXSOUND_BREAK_2_PLAY               0x0004
//RISC Read DSP Memory
#define INT_RC2D_READ_DSP_MEMORY                0x00F7         // 247
//RISC Write DSP Memory
#define INT_RC2D_WRITE_DSP_MEMORY               0x00F8         // 248
#define INT_RC2D_XOR                            0x00F9         // 249
#define INT_RC2D_OR                             0x00FA         // 250
#define INT_RC2D_AND                            0x00FB         // 251
#define INT_RC2D_DSP_JOB_RESET                  0x00FC         // 252
    #define JOB_RESET_DECODER                   0x1000       // for job0
    #define JOB_RESET_DECODER2                  0x2000       // for job2
    #define JOB_RESET_JOB3                      0x4000       // for job3
    #define JOB_RESET_JOB4                      0x8000       // for job4
    #define JOB_RESET_DSP                       0x8000       // for all jobs
//Flow Control Flag
#define INT_RC2D_FLOW_CONTROL_DEC2              0x00FE         // 254
//Flow Control Flag
#define INT_RC2D_FLOW_CONTROL                   0x00FF         // 255
    //Play: 0x2 <<8, Stop: 0x1 <<8, Flush: 0x4 <<8
    #define FLOW_CONTROL_STOP                   0x0100
    #define FLOW_CONTROL_PLAY                   0x4200
    #define FLOW_CONTROL_FLUSH                  0x0400
    #define FLOW_CONTROL_PAUSE                  0x10800
    #define FLOW_CONTROL_RESUME                 0x20000
    //#define FLOW_CONTROL_RESET                       0x800000
    #define FLOW_CONTROL_AOUT_REINIT            0x0800
    #define FLOW_CONTROL_SAMPLING_RATE          0x1000
    #define FLOW_CONTROL_AOUT_WAIT_ENABLE       0x40000
    #define FLOW_CONTROL_AOUT_ENABLE            0x80000
    #define FLOW_CONTROL_TRANSPARENT_REINIT     0x100000

///#ifdef MP3ENC_SUPPORT
    #define ENC_CONTROL_STOP       0x000100
    #define ENC_CONTROL_RECORD     0x000200
    #define ENC_CONTROL_FLUSH      0x000400
    #define ENC_CONTROL_HIGHSPEED  0x004000
    #define ENC_CONTROL_PAUSE      0x010000
    #define ENC_CONTROL_RESUME     0x020000
    #define ENC_CONTROL_OUTFULL    0x200000
    #define ENC_CONTROL_RESET      0x800000
///#endif
#endif // RC2D_INT_H

