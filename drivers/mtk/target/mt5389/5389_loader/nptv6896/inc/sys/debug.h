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
 * $Author: jifeng.fan $
 * $Date  $
 * $RCSfile: debug.h,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "typedef.h"

// debug message control

#define SUPPORT_VGA_AMBIGUOUS_H_DETECT 1
//#define VGA_VSYNC_WIDTH_AMBIGUOUS_CHECK 1 //move ot drv_opt
#define HDTV_240P_SUPPORT	0  // TG39  Interlace / Noninterlace switch
#define HDTV_540P_SUPPORT	0
#define HDTV_288P_SUPPORT	0  // TG39  Interlace / Noninterlace switch
#ifndef NEW_SYNC_SLICER_CIRCUIT
#define ADAPTIVE_SLICER_ENABLE 1
#endif
#if ADAPTIVE_SLICER_ENABLE
#define SUPPORT_SET_SLICER 0           //wu add
#define ADAPTIVE_SLICER_PLLERR_CHK 1
#define ADAPTIVE_SLICER_FILED_CHK 0
#define ADAPTIVE_MONITOR_SLICER_MEASURE 1
#if ADAPTIVE_MONITOR_SLICER_MEASURE
#define ADAPTIVE_MONITOR_SLICER_MEASURE_DEBUG 1
#define ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE 1
#define ADAPTIVE_SLICER_DEFAULT_SETTING 0
#else
#define ADAPTIVE_MONITOR_SLICER_MEASURE_DEBUG 0
#define ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE 0
#define ADAPTIVE_SLICER_DEFAULT_SETTING 1
#endif
#else
#define SUPPORT_SET_SLICER 1    //wu add
#define ADAPTIVE_SLICER_PLLERR_CHK 0
#define ADAPTIVE_SLICER_FILED_CHK 0
#define ADAPTIVE_SLICER_DEFAULT_SETTING 0
#define ADAPTIVE_MONITOR_SLICER_MEASURE 0
#if ADAPTIVE_MONITOR_SLICER_MEASURE
#define ADAPTIVE_MONITOR_SLICER_MEASURE_DEBUG 0
#define ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE 0
#define ADAPTIVE_SLICER_DEFAULT_SETTING 0
#else
#define ADAPTIVE_MONITOR_SLICER_MEASURE_DEBUG 0
#define ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE 0
#define ADAPTIVE_SLICER_DEFAULT_SETTING 0
#endif
#endif
#define SUPPORT_HDTV_HARDWARE_MUTE 1 

#define MANUAL_MAIN_RST  1 //fix YPbpr to AV jitter problem in 5360 , use manual reset 
#define MANUAL_MAINSUB_FIFO_RST  1 // 0 //  fix do autophase several times issue

#define FIX_VGA_HDMI_INTERFERENCE (0)
#define YPBPR_480IP_27MHZ  1
#define SUPPORT_PQ_BYPASS   1
#ifdef __MODEL_slt__
#define DECODER_ADD_WIDTH   0
#else
// (1)  ECO fixed :displaymode line buffer wrong at right most 1~2 pixels, need extra 8 pixel data from decoder
// (2)  ECo fixed: 444 to 422 wrong color 
// (3)  Scaler display mode, Pack*N+1, the last pixel lost, need extra 2 pixels
#define DECODER_ADD_WIDTH   0
#endif

#define PSCAN_DISPMODE_MIRROR_WA 0

#define SCART_CLK_SOURCE_CVBS
//sys related verification
#define CC_CHECK_DEC_STABLE 0
#define CC_MEASURE_SYS_TIME 0



#define assert(x)
#define assertISR(x)
#endif


