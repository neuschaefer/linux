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
 * $RCSfile: d2rc_int.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file d2rc_int.h
 *  Brief of file d2rc_int.h.
 *  Details of file d2rc_int.h (optional).
 */
 
#ifndef D2RC_INT_H
#define  D2RC_INT_H

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
/*
 * Interrupt Case  
*/
// Light: For debug, add DSP print out interrupt (0x80)
/************* Debug2Risc Interrupt ***************/
#define INT_D2RC_PRINT_DATA                               0x0080

/************* Decoder2Risc Interrupt ***************/
//DTS Table --> reserved
#define INT_D2RC_DTS_ADPCM                                 0x0000
#define INT_D2RC_DTS_VQ                                    0x0001
#define INT_D2RC_DTS_ADPCM96                               0x0002
#define INT_D2RC_DTS_VQ96                                  0x0003
//HDCD --> to be removed
#define INT_D2RC_HDCD_TRK_STM_CHG                          0x0003
#define D2RC_STM_HDCD_OFF                                  0x01
#define D2RC_STM_HDCD_ON                                   0x02
//Deemphasis Notify MP2/MP3
#define INT_D2RC_DEEMPHASIS_NOTIFY                         0x0004
//Deemphasis Notify MP2/MP3 for second decoder
#define INT_D2RC_DEEMPHASIS_NOTIFY_DEC2                    0x0005
#define DEEMPH_ENABLE                                      0x01
#define DEEMPH_DISABLE                                     0x00
// MP3
#define INT_D2RC_MP3_TYPE                                  0x0006
//LPCM --> reserved
#define INT_D2RC_LPCM_PARAMETER                            0x0007
//VORBIS Table --> reserved
#define INT_D2RC_VORBIS_CODEBOOK                           0x0008
// DVD Audio --> to be removed
#define INT_D2RC_DVD_A_WATERMARK                           0x0009

//JPEG notify, #ifdef ADSP_JPEG_DEC2
#define INT_D2RC_JPEG_NOTIFY                                0x0D0           /// interrupt address
 #define JPEG_D2RC_NOTIFY_PLAY                              0x00001         /// notify start decode
 #define JPEG_D2RC_NOTIFY_JPEG_INFO                         0x00002         /// notify got JFIF info
 #define JPEG_D2RC_NOTIFY_ONE_MCU_DONE                      0x00003         /// notify one MCU line decode done
 #define JPEG_D2RC_NOTIFY_DEC_STOP                          0x00004         /// notify decoder stop already.
 #define JPEG_D2RC_NOTIFY_PROG_SOS                          0x00005         /// notify progressive sos

 #define JPEG_D2RC_LOG                                      0x0000F

 #define JPEG_D2RC_NOTIFY_ADR_REPARSE                       0x000E0         /// progressive request re-parse

 #define JPEG_D2RC_NOTIFY_DONE                              0x000F0         /// notify JPEG decoding finish
 #define JPEG_D2RC_NOTIFY_ERROR                             0x000FF         /// notify decoding error
 #define JPEG_D2RC_NON_SUPPORT_WIDTH                        0x000FE         /// notify JPEG width too large and current DRAM size non-support
//JPEG notify end

#ifdef DSP_SUPPORT_NPTV
//PAL notify
#define INT_DSP_PAL_DETECTED_MODE                           0x00DF
#define NICAM_DETECTED_STEREO                               0x0001
#define NICAM_DETECTED_DUAL                                 0x0002
#define NICAM_DETECTED_MONO                                 0x0004
#define PAL_FM_MONO                                            0x0000
//TTX notify
#define INT_DSP_TTXACC_NOTIFY                               0x00E1
#define TTXACC_WAKEUP                                       0x0000
#define TTXACC_CALCULATION_DONE                             0x0001
#define TTXACC_NONEMACRO			                        0x0002
//Miner notify
#define INT_DSP_MINER_NOTIFY                                0x00E0
#define MINER_CAPTURE_START                                 0x0000
#define MINER_CAPTURE_DONE                                  0x0001
//MTS notify
#define INT_DSP_MTS_DETECTED_MODE                           0x00E2
#define MTS_DETECTED_STEREO_SAP                             0x0000
#define MTS_DETECTED_SAP                                    0x0001
#define MTS_DETECTED_STEREO                                 0x0002
#define MTS_DETECTED_MONO                                   0x0003
//JPN notify
#define INT_DSP_JPN_DETECTED_MODE                           0x00E3
#define JPN_DETECTED_MONO                                   0x0001
#define JPN_DETECTED_STEREO                                 0x0002
#define JPN_DETECTED_DUAL                                   0x0003
//DETECTOR notify
#define INT_DSP_DETECTOR_NOTIFY                             0x00E4
// CHANGE SOUND STANDARD
#define INT_DSP_CHANGE_ATV_STANDARD                              0x00E5
// high deviation mode auto switch(on/off) log
#define INT_DSP_HDEV_AUTO_SWITCH                          0x00E6
#define HDEV_MODE_OFF                                                0x0000
#define HDEV_MODE_ON                                                  0x0001
// FM radio channel detection notify
#define INT_D2RC_FM_RDO_DET_FINISH                       0x00E7
#define NONE_FM_CHANNEL_DETECTED                         0x0000
#define FM_CHANNEL_DETECTED                                    0x0001
#endif

/************* Common2Risc Interrupt ***************/
#define INT_D2RC_DSP_INFO                                  0x00F4
#define D2RC_DSP_INFO_AIN_UPDATE_ADC_TUNE_CFG              0x01
#define INT_D2RC_LOG_DUMP                                  0x00F5
#define INT_D2RC_PTS_1ST_REFRESH_DEC2                      0x00F6
#define INT_D2RC_PTS_1ST_REFRESH                           0x00F7
// Dsp r/w risc
#define INT_D2RC_WRITE_REG_DATA                            0x00F8
#define INT_D2RC_WRITE_REG_ADR                             0x00F9
#define INT_D2RC_READ_REG                                  0x00FA
// Dram Read
// Set length first and then send address
#define INT_D2RC_PRINT_DRAM_ADR                            0x00FB
#define INT_D2RC_PRINT_DRAM_LEN                            0x00FC
#define INT_D2RC_COMPARE_RESULT                            0x00FD
#define INT_D2RC_WAKEUP_OK                                 0x00FE
#define INT_D2RC_FLOW_CONTROL                              0x00FF
  // for first decoder
#define D2RC_FLOW_CONTROL_FLUSH_DONE                       0x00
#define D2RC_FLOW_CONTROL_SAMPLING_RATE                    0x01
#define D2RC_FLOW_CONTROL_AOUT_ENABLE                      0x02
#define D2RC_FLOW_CONTROL_PAUSE_OK                         0x04
#define D2RC_FLOW_CONTROL_MODE_CHANGE                      0x08  // for decoder mode changed, Light
  // for second decoder
#define D2RC_FLOW_CONTROL_FLUSH_DONE_DEC2                  0x10
#define D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC2               0x11
#define D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC2                 0x12
#define D2RC_FLOW_CONTROL_PAUSE_OK_DEC2                    0x14
#define D2RC_FLOW_CONTROL_MODE_CHANGE_DEC2                 0x18  // for acmod changed, Light
#if 0
#define D2RC_FLOW_CONTROL_UPDATE_EFFECT                    0x20
#else
  // for third decoder
#define D2RC_FLOW_CONTROL_FLUSH_DONE_DEC3                  0x20
#define D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC3               0x21
#define D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC3                 0x22
#define D2RC_FLOW_CONTROL_PAUSE_OK_DEC3                    0x24
#define D2RC_FLOW_CONTROL_MODE_CHANGE_DEC3                 0x28  // for acmod changed, Light
#define D2RC_FLOW_CONTROL_UPDATE_EFFECT                    0x30
#endif
#define D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE                    0x31
#define D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE_DEC2                    0x32




LINT_EXT_HEADER_END

#endif  // D2RC_INT_H

