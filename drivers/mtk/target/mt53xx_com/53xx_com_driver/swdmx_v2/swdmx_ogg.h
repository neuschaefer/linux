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
 * $RCSfile: swdmx_mp4.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_flw.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef SWDMX_OGG_H
#define SWDMX_OGG_H


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

#define OGG_PAGE_HEADER 0x5367674F //OGGS

#define OGG_FOUR_CC 4
#define OGG_HEADER_SIZE 27
#define OGG_MAX_AUDIO_TRACK 8
#define KEY_LEN 30
#define MAX_REPEAT_TIME 35
#define OGG_FF_SEEK_TIME  4
#define OGG_FB_SEEK_TIME  5
#define OGG_NORMAL_PAGE_SIZE 0x1F00
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct{

    UINT64 u8KeyOffset;
    UINT64 u8GranulPos;
    UINT32 u4KeyPts;
}OGG_KEY_DATA;

typedef struct {

    OGG_KEY_DATA stKeyData[KEY_LEN];
    UINT8    u1CurIdx;
    BOOL    fgRing;
}KEY_INFO;

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

BOOL _SWDMX_OggInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType);

/**
*/
void _SWDMX_OggSetRange(UINT8 u1SrcId, 
    SWDMX_RANGE_LIST_T *prRangeList, 
    UINT32 u4VidStrmID, 
    UINT32 u4AudStrmID, 
    UINT32 u4SubTitleID);

BOOL _SWDMX_OggAbort(UINT8 u1SrcId);


BOOL _SWDMX_OggSetDecoderInfo(UINT8 u1SrcId,
    UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3);

BOOL _SWDMX_OggGetInfo(UINT8 u1SrcId,
    UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3);

BOOL _SWDMX_OggSetStrmID(UINT8 u1SrcId,
    UINT8 u1HdrType,  
    UINT32 u4StrmID, 
    UINT32 u4StrmSubID);

/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_OggHdrPrs(
    UINT8 u1SrcId,
	SWDMX_RANGE_LIST_T* prRangeList,
	 SWDMX_CUR_CMD_T* prCmd);

BOOL _SWDMX_OggEnableStrm(UINT8 u1SrcId,
    UINT8 u1StrmType, 
    VOID* pfnCbFunc);

BOOL _SWDMX_OggDisableStrm(UINT8 u1SrcId,
    UINT8 u1StrmType);

BOOL _SWDMX_OggAudioCb(UINT8 u1SrcId,
    ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);


BOOL _SWDMX_OggVideoCallback(const DMX_PES_MSG_T* prPes);

BOOL _SWDMX_OggAudioCallback(const DMX_AUDIO_PES_T* prPes);

/**
*/
BOOL _SWDMX_OggStop(UINT8 u1SrcId);

typedef enum
{
    SEEK_FORWARD = 1,
    SEEK_BACKWARD,
    SEEK_KEY
}SEEK_DERECTION;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif //VDEC_FLW_H


