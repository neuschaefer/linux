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
 * Description:
 *
 *      The inter-module database
 *
 *---------------------------------------------------------------------------*/

#ifndef DRV_DBASE_H
#define DRV_DBASE_H


#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_os.h"
#include "x_timer.h"

#include "vdp_drvif.h"
#include "stc_drvif.h"
#include "mpv_drvif.h"
#include "aud_if.h"
LINT_EXT_HEADER_END


//=====================================================================
// Constant definitions

#define DRV_DBASE_CURRENT_VERSION       1

//=====================================================================
// Type definitions

//---------------------------------------------------------
// Denifitons of module parameters

typedef struct
{
	UINT32	u4DispOk;
	UINT32	u4DispFail;
	UINT32	u4DispSkip;
	UINT32	u4DispRepeat;		// Repeat due to AV Sync
	UINT32	u4DispDrop;			// Drop due to AV Sync
	UINT32	u4SyncMode;			// Mode of AV Sync

    HAL_TIME_T 		rStartTime;
} PARAM_VDP_FB_COUNTER_T;

typedef struct
{
	PARAM_VDP_FB_COUNTER_T	arFbCounter[VDP_NS][VDP_MAX_INPORT_NS];
	UINT32	au4UnderFlow[VDP_NS];
	UINT32	au4Tearing[VDP_NS];
	UINT32	au4StartLineCnt[VDP_NS];
	UINT32	au4StopLineCnt[VDP_NS];
} PARAM_VDP_T;

typedef struct
{
	UINT32 u4PsrDropNs;
	UINT32 u4RetrieveNs;
	UINT32 u4EsmSkipNs;
	UINT32 u4VpsrDropNs;
	UINT32 u4DecOkNs;
	UINT32 u4TimeOutNs;
	UINT32 u4ErrDropNs;
	UINT32 u4EsmQCnt;
	HAL_TIME_T rDecFirstTime;
	UINT32 u4NoIAfterSeqH;
	UINT32 u4HaveI;
	UINT16 u2HSize;
	UINT16 u2VSize;
	UCHAR ucPicStruct;
	UCHAR ucFrameRate;
	HAL_TIME_T rIDeltaTime;

} PARAM_MPV_T;

typedef struct
{
    /* common code field*/
    UINT32 dwInputUnderRun; /* error count for input underrun*/
    UINT32 dwInputUnderRunDec2; /* error count for input underrun of decoder 2*/
    UINT32 dwInputUnderRunMixSound; /* error count for input underrun of mixsound*/
    UINT32 dwOutputUnderRun; /* error count for input output underrun*/
    UINT32 dwOutputUnderRunDec2; /* error count for input output underrun of decoder2*/
    UINT32 dwOutputUnderRunMixSound; /* error count for input output underrun of mixsound*/
    UINT32 dwReserved0[9]; /* reserved field */
    /* first decoder's field*/
    UINT32 dwFrameRepeatCount; /* repeat count for frames*/
    UINT32 dwFrameDropCount; /* drop count for frames*/
    UINT32 dwTotalErrorCount; /* total error count*/
    UINT32 dwTotalFrameCount; /* total frame count*/
    UINT32 dwReserved1[3]; /* reserved field*/
    UINT32 dwDetailLogs[16] ; /* detail log for decoder1*/
    /* second decoder's field*/
    UINT32 dwFrameRepeatCountDec2; /* repeat count for frames*/
    UINT32 dwFrameDropCountDec2; /* drop count for frames*/
    UINT32 dwTotalErrorCountDec2; /* total error count*/
    UINT32 dwTotalFrameCountDec2; /* total frame count*/
    UINT32 dwReserved2[3]; /* reserved field*/
    UINT32 dwDetailLogsDec2[16] ; /* detail log for decoder2*/
} PARAM_AUD_T;

typedef struct
{
	UINT32 au4PcrRxNs[STC_SRC_NS];
	UINT32 u4VCxO;
} PARAM_PCR_T;

typedef struct
{
    UINT32 u4MinBw;
    UINT32 u4MaxBw;
    UINT32 u4AvgBw;
    UINT32 u4Count;
    UINT32 u4AvgBwFraction;
} PARAM_DRAM_T;

typedef struct
{
    UINT32 u4AudioPts;
    UINT32 u4AudioPtsStcDelta;
    UINT32 u4VideoPts;
    UINT32 u4VideoPtsStcDelta;
} PARAM_PCR_PTS_INFO;

//---------------------------------------------------------
// The database definition

typedef struct
{
    UINT32                  u4Version;      // Version number
    PARAM_MPV_T             rMpv[MPV_MAX_ES];           // MPEG decoder
    PARAM_VDP_T             rVdp;           // Video Plane
    PARAM_AUD_T             rAud;           // Audio
    PARAM_PCR_T				rPcr;			// PCR

    PARAM_DRAM_T            rDram;

#ifdef CC_GET_PCR_PTS_INFO
    PARAM_PCR_PTS_INFO      rPts;
#endif

    // Other modules go here
    // ...

} DRV_DBASE_T;



//=====================================================================
// Interface

extern BOOL DBS_Init(void);

extern DRV_DBASE_T* DBS_Lock(CRIT_STATE_T* prCritState);

extern BOOL DBS_Unlock(const DRV_DBASE_T* prDbase, CRIT_STATE_T rCritState);

#endif  // DRV_DBASE_H
