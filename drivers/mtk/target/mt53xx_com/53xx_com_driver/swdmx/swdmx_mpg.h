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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: swdmx_mpg.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_flw.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef SWDMX_MPV_H
#define SWDMX_MPV_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "swdmx_drvif.h"
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define TS_PES_VIDEO_FIFO_SIZE      (3*1082880)
#define TS_PES_AUDIO_FIFO_SIZE      (86016)
#define PS_PES_VIDEO_FIFO_SIZE      (3*1082880)
#define PS_PES_AUDIO_FIFO_SIZE      (86016)

#define TS_OPEN_FILE_SIZE           (235*1024*16)
#define PS_OPEN_FILE_SIZE           (235*1024*16)
#define ES_OPEN_FILE_SIZE           (235*1024*16)

#define FIRST_PTS_DETECT_SIZE       (235*1024*8*5)
#define LAST_PTS_DETECT_SIZE        (235*1024*8)
#define SEQ_DETECT_SIZE             (235*1024*8*5)

#define MPG_LBA_BLOCK_SIZE (192)

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


/**
*/
EXTERN BOOL _SWDMX_MpgEsInit(ENUM_SWDMX_FMT_T eType);

/**
*/
EXTERN BOOL _SWDMX_MpgPsInit(ENUM_SWDMX_FMT_T eType);

/**
*/
EXTERN BOOL _SWDMX_MpgTsInit(ENUM_SWDMX_FMT_T eType);

/**
*/
EXTERN BOOL _SWDMX_MpgPlay(VOID);

/**
*/
EXTERN BOOL _SWDMX_MpgPause(VOID);

/**
*/
EXTERN BOOL _SWDMX_MpgAbort(VOID);

EXTERN BOOL _SWDMX_MpgSetStrmID(UINT8 u1HdrType,
    UINT32 u4StrmID, UINT32 u4StrmSubID);

EXTERN BOOL _SWDMX_MpgEnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc);

EXTERN BOOL _SWDMX_MpgDisableStrm(UINT8 u1StrmType);
    
/**
*/
EXTERN BOOL _SWDMX_MpgGetStreamInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo);

/**
*/
/*
EXTERN BOOL _SWDMX_MpgPsGetStreamInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo);
*/

/**
*/
EXTERN BOOL _SWDMX_MpgTsGetStreamInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo);

/**
*/
EXTERN BOOL _SWDMX_MpgEsGetStreamInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo);

/**
*/
EXTERN VOID _SWDMX_MpgSetRange(SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID, 
								      UINT32 u4AudStrmID, UINT32 u4SubTitleID);

/**
*/
EXTERN SWDMX_HDR_PRS_STATE_T _SWDMX_MpgHeaderParse(
	SWDMX_RANGE_LIST_T* prRangeList,
	INT32 i4PlaySpeed,
	UINT64 u8SeekPos,
	BOOL fgSpeedChange);

/**
*/
EXTERN BOOL _SWDMX_MpgStop(VOID);

/**
*/
EXTERN BOOL _SWDMX_MpgClose(VOID);

/**
*/
EXTERN BOOL _SWDMX_MpgCloseGetInfo(VOID);

/**
*/
EXTERN BOOL _SWDMX_MpgFlush(VOID);

EXTERN BOOL _SWDMX_MpgSetInfo(UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3);

EXTERN BOOL _SWDMX_MpgGetInfo(UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3);

EXTERN VOID _SWDMX_MpgSetSwdmxInfo(SWDMX_INFO_T* prSwdmxInfo);
EXTERN BOOL _SWDMX_MpgAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);

EXTERN BOOL _SWDMX_MpgGetPosByTime(BOOL fgUpdate, UINT32 u4Time, UINT64 *pu8Pos);
EXTERN BOOL _SWDMX_MpgGetPosByTick(BOOL fgUpdate, UINT32 u4Tick, UINT64 *pu8Pos);
EXTERN BOOL _SWDMX_MpgGetPosByTickNum(BOOL fgUpdate, UINT32 u4TickNum, UINT64 *pu8Pos);
EXTERN BOOL _SWDMX_MpgGetIPosByTickNum(UINT64 u8Start, UINT32 u4TickNum, UINT64 *pu8Pos);
EXTERN BOOL _SWDMX_MpgGetTickByPos(BOOL fgUpdate, UINT64 u8Pos, INT32 i4PlaySpeed, UINT32 *pu4TickNum);
EXTERN BOOL _SWDMX_MpgPcrCallBack(UINT32 u4PcrBase);

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif //SWDMX_MPV_H


