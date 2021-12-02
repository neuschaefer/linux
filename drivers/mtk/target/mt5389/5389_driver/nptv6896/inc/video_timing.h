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
 * $RCSfile: video_timing.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _VIDEO_TIMING_H_
#define _VIDEO_TIMING_H_

#include "vga_table.h"

#define HDTV_SEARCH_START	1
#define HDTV_SEARCH_END		(bHdtvTimings-1)

#define VGA_SEARCH_START		(bHdtvTimings)  //1
#define VGA_SEARCH_END		(bAllTimings-1)

#define DVI_SEARCH_START		(bHdtvTimings)
#define DVI_SEARCH_END		(bHdtvTimings+bVgaTimings-1)

#define MAX_TIMING_FORMAT	(bAllTimings)

enum
{
    MODE_NOSIGNAL = 0,        // No signal
    MODE_525I_OVERSAMPLE = 1,      //SDTV 
    MODE_625I_OVERSAMPLE,       //
    MODE_480P_OVERSAMPLE,       //SDTV
    MODE_576P_OVERSAMPLE,
    MODE_720p_50,               //HDTV 
    MODE_720p_60,               //HDTV   
    MODE_1080i_48,              //HDTV  
    MODE_1080i_50,              //HDTV  
    MODE_1080i,                 //HDTV
    MODE_1080p_24,              //HDTV 
    MODE_1080p_25,
    MODE_1080p_30,
    MODE_1080p_50,              //HDTV 
    MODE_1080p_60,
    MODE_525I,
    MODE_625I,
    MODE_480P,
    MODE_576P,    
    MODE_720p_24,
    MODE_720p_25,    
    MODE_720p_30,        
    MODE_240P,
    MODE_540P,
    MODE_288P,    
    MODE_480P_24,    
    MODE_480P_30,        
    MODE_576P_25,    
    MODE_HDMI_640_480P,
    MODE_HDMI_720p_24,
    MODE_3D_720p_50_FP,
    MODE_3D_720p_60_FP,
    MODE_3D_1080p_24_FP,
    MODE_3D_1080I_60_FP,
    MODE_3D_480p_60_FP,
    MODE_3D_576p_50_FP,
    MODE_3D_720p_24_FP,
    MODE_3D_720p_30_FP,
    MODE_3D_1080p_30_FP,
    MODE_3D_480I_60_FP,
    MODE_3D_576I_60_FP,
    MODE_3D_1080I_50_FP,
    MODE_3D_1080p_50_FP,    
    MODE_3D_1080p_60_FP,
    MODE_3D_1650_750_60_FP,
    MODE_3D_1650_1500_30_FP,
    MODE_3D_640_480p_60_FP,
    MODE_3D_1440_240p_60_FP,
    MODE_3D_1440_288p_50_FP,
    MODE_3D_1440_576p_50_FP,
    MODE_3D_720p_25_FP,
    MODE_3D_1080p_25_FP,
    MODE_3D_1080I_1250TOTAL_50_FP,
    MODE_3D_1080p_24_SBS_FULL,
    MODE_3D_1080p_30_SBS_FULL,
    MODE_3D_1080I_50_SBS_FULL,
    MODE_3D_1080I_60_SBS_FULL,
    MODE_3D_720p_24_SBS_FULL,
    MODE_3D_720p_30_SBS_FULL,
    MODE_3D_720p_50_SBS_FULL,
    MODE_3D_720p_60_SBS_FULL,
    MODE_3D_480p_60_SBS_FULL,
    MODE_3D_576p_50_SBS_FULL,
    MODE_3D_480I_60_SBS_FULL,
    MODE_3D_576I_50_SBS_FULL,
    MODE_3D_640_480p_60_SBS_FULL,
    MODE_3D_640_480p_60_LA,
    MODE_3D_480p_60_LA,
    MODE_3D_576p_50_LA,
    MODE_3D_720p_60_LA,
    MODE_3D_720p_50_LA,
    MODE_3D_1080p_24_LA,
    MODE_3D_480I_60_FA,
    MODE_3D_1080I_60_FA,
    MODE_3D_1080I_50_FA,
    MODE_3D_2880_240p_60_FP,
    MODE_3D_2880_480I_60_FP,    
    MODE_3D_2880_240p_60_SBSH_TB,
    MODE_3D_2880_480p_60_SBSH_TB,
    MODE_3D_2880_288p_50_SBSH_TB,
    MODE_3D_2880_576p_50_SBSH_TB,
    MODE_MAX,
    MODE_DE_MODE = 252,
    MODE_NODISPLAY = 253,
    MODE_NOSUPPORT = 254,      // Signal out of range
    MODE_WAIT = 255
};


enum
{
    B2R_CLK_MODE_27,
    B2R_CLK_MODE_74,
    B2R_CLK_MODE_148,
    B2R_CLK_MODE_MAX
};

#define fgIsUserModeTiming(bMode) (((bMode) >= bUserVgaTimingBegin) && ((bMode) < bAllTimings))
#define fgIsVgaTiming(bMode)   ((((bMode) >= DVI_SEARCH_START) && ((bMode) <= DVI_SEARCH_END)) || ((bMode) == MODE_DE_MODE) || fgIsUserModeTiming(bMode))
#define fgIsVideoTiming(bMode) (((bMode) >= HDTV_SEARCH_START) && ((bMode) <= HDTV_SEARCH_END))
#define fgIsValidTiming(bMode) (fgIsVgaTiming(bMode) || fgIsVideoTiming(bMode) ||fgIsUserModeTiming(bMode))

extern UINT8 _bHdtvTiming;

#define fgIsOversampleTiming() ((((_bHdtvTiming) >= MODE_525I_OVERSAMPLE) && ((_bHdtvTiming) <= MODE_625I_OVERSAMPLE)) \
                                ||(((_bHdtvTiming) >= MODE_480P_OVERSAMPLE) && ((_bHdtvTiming) <= MODE_576P_OVERSAMPLE)))


#define fgIsPScanTiming() ((_bHdtvTiming==MODE_576P) || (_bHdtvTiming==MODE_576P_OVERSAMPLE) || \
                           (_bHdtvTiming==MODE_480P) || (_bHdtvTiming==MODE_480P_OVERSAMPLE) || \
                           (_bHdtvTiming==MODE_240P) || (_bHdtvTiming==MODE_540P))
#define fgIsVBISupportTiming() ((((_bHdtvTiming) >= MODE_525I_OVERSAMPLE) && ((_bHdtvTiming) <= MODE_576P_OVERSAMPLE)) \
                               ||(((_bHdtvTiming) >= MODE_525I) && ((_bHdtvTiming) <= MODE_576P)) \
                               ||(((_bHdtvTiming) >= MODE_480P_24) && ((_bHdtvTiming) <= MODE_576P_25)) \
                               )

#define fgIsVBISupportCCTiming() ((_bHdtvTiming == MODE_525I_OVERSAMPLE)||(_bHdtvTiming == MODE_525I))
/////////////////////////////////////////////////////////////////////
// #define MODE_NOSIGNAL 0         // No signal
// #define MODE_DE_MODE 252
// #define MODE_NODISPLAY 253
// #define MODE_NOSUPPORT 254      // Signal out of range
// #define MODE_WAIT 255

#define PROGRESSIVE 0           //Progressive Mode
#define INTERLACE 1             //Interlace Mode

#endif
