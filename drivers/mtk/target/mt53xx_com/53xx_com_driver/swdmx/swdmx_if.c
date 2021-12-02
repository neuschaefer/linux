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
 * $RCSfile: swdmx_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_if.c
 *  Software program stream demux driver - public interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "swdmx_if.h"
#include "swdmx_drvif.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** SWDMX_Init
 *  Initialize software program stream demux driver
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
VOID SWDMX_Init(VOID)
{
    _SWDMX_Init();
}


//-----------------------------------------------------------------------------
/** SWDMX_GetStreamInfo
 *  
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_GetStreamInfo(ENUM_SWDMX_FMT_T eType, 
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    return _SWDMX_GetStreamInfo(eType, u4InfoMask, u8FileSize, prPInfo);
}

//-----------------------------------------------------------------------------
/** SWDMX_SetInfo
 *  
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_SetInfo(UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3)
{
    return _SWDMX_SetInfo(u1Type, u4Para1, u4Para2, u4Para3);
}

//-----------------------------------------------------------------------------
/** SWDMX_GetInfo
 *  
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_GetInfo(UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3)
{
    return _SWDMX_GetInfo(u1Type, pu4Para1, pu4Para2, pu4Para3);
}

//-----------------------------------------------------------------------------
/** SWDMX_Play
 *  Start to run software program stream demux
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_Play(VOID)
{
    return _SWDMX_Play();
}


//-----------------------------------------------------------------------------
/** SWDMX_Pause
 *  Pause software demux
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_Pause(VOID)
{
    return _SWDMX_Pause();
}


//-----------------------------------------------------------------------------
/** SWDMX_Stop
 *  Stop software program stream demux
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_Stop(VOID)
{
    return _SWDMX_Stop();
}


//-----------------------------------------------------------------------------
/** SWDMX_Flush
 *  Flush A/V data
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_Flush(VOID)
{
    return _SWDMX_Flush();
}

//-----------------------------------------------------------------------------
/** SWDMX_SetSpeed
 *  Set the playback speed 
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_SetSpeed(INT32 i4Speed)
{
    return _SWDMX_SetSpeed(i4Speed);
}

//-----------------------------------------------------------------------------
/** SWDMX_SeekTime
 *  Set the file position 
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_SeekTime(UINT32 u4SeekTime, UINT64* pu8FilePos)
{
    return _SWDMX_SeekTime(u4SeekTime, pu8FilePos);
}

//-----------------------------------------------------------------------------
/** SWDMX_SeekPos
 *  Set the file position 
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_SeekPos(UINT64 u8SeekPos, UINT64 u8SeekPosI)
{
    return _SWDMX_SeekPos(u8SeekPos, u8SeekPosI);
}

//-----------------------------------------------------------------------------
/** SWDMX_SeekPercent
 *  Set the file position 
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_SeekPercent(UINT32 u4SeekPercent, UINT64* pu8FilePos)
{
    return _SWDMX_SeekPercent(u4SeekPercent, pu8FilePos);
}


//-----------------------------------------------------------------------------
/** SWDMX_RenderFromPos
 *  Set the file position 
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_RenderFromPos(VOID *prPosition)
{
    return _SWDMX_RenderFromPos(prPosition);
}

//-----------------------------------------------------------------------------
/** SWDMX_Add_Range
 *  Set the demux range 
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_AddRange(VOID* pvRangeInfo, UINT32 u4TagID)
{
    MM_RANGE_INFO_T* prRangeInfo = (MM_RANGE_INFO_T*)pvRangeInfo;
    
    return _SWDMX_AddRange(prRangeInfo, u4TagID);
}


//-----------------------------------------------------------------------------
/** SWDMX_Del_Range
 *  Delete the demux range 
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_DelRange(VOID* pvRangeInfo, UINT32 u4TagID, BOOL fgDelAllRange)
{
    MM_RANGE_INFO_T* prRangeInfo = (MM_RANGE_INFO_T*)pvRangeInfo;
    
    return _SWDMX_DelRange(prRangeInfo, u4TagID, fgDelAllRange);
}


//-----------------------------------------------------------------------------
/** SWDMX_SetContainerType
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_SetContainerType(ENUM_SWDMX_FMT_T eContainerType)
{
	return _SWDMX_SetContainerType(eContainerType);
}

//-----------------------------------------------------------------------------
/** SWDMX_SetStrmID
 *  Set the demux range 
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_SetStrmID(UINT32 u4StrmID, UINT32 u4StrmSubID, ENUM_SWDMX_STRM_TYPE_T eStrmType)
{
    switch(eStrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
        	return _SWDMX_SetVideoStreamID(u4StrmID);
        case eSWDMX_STRM_TYPE_AUD:
        	return _SWDMX_SetAudioStreamID(u4StrmID, u4StrmSubID);
        case eSWDMX_STRM_TYPE_AUD2:
        	return _SWDMX_SetAudio2StreamID(u4StrmID);
        case eSWDMX_STRM_TYPE_SUBTITLE:
        	return _SWDMX_SetSubTitleStreamID(u4StrmID);
        case eSWDMX_STRM_TYPE_PCR:
        	return _SWDMX_SetPcrStreamID(u4StrmID);
        default:
        	return FALSE;
    }
}


//-----------------------------------------------------------------------------
/** SWDMX_QueryStatus
 *  Query the swdmx status 
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
ENUM_SWDMX_PLAY_STATE_T SWDMX_QueryStatus(VOID)
{
    return _SWDMX_QueryStatus();
}

//-----------------------------------------------------------------------------
/** SWDMX_SetRangeCb
 *  Set range callback function
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_SetRangeCb(VOID* pfnCbFunc)
{
    return _SWDMX_SetRangeCb(pfnCbFunc);
}

//-----------------------------------------------------------------------------
/** SWDMX_QueryRangeInfo
 *  Query swdmx range information, including id and start PTS
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------

VOID SWDMX_QueryRangeInfo(SWDMX_RANGE_INFO_T* prRangeInfo)
{
    _SWDMX_QueryRangeInfo(prRangeInfo);
    return;
}

//-----------------------------------------------------------------------------
/** SWDMX_SetDmxNfy
 *  Set notify functio for ctrl
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
INT32 SWDMX_SetDmxNfy(const SWDMX_NFY_INFO_T* prSwdmxNfyInfo)
{
    return _SWDMX_SetDmxNfy(prSwdmxNfyInfo);
}


//-----------------------------------------------------------------------------
/** SWDMX_SetDmxNfy
 *  Set notify functio for ctrl
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_ReadBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo)
{
    return _SWDMX_ReadBuffer(eStrmType, prReadInfo);
}


//-----------------------------------------------------------------------------
/** SWDMX_ReleaseBuffer
 *  Release (unlock) buffer
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_ReleaseBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo)
{
    return _SWDMX_ReleaseBuffer(eStrmType, prReadInfo);
}


//-----------------------------------------------------------------------------
/** SWDMX_AudioNotify
 *  Set notify functio for audio down
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_AudioNotify(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    return _SWDMX_AudioNotify(eAudioNotifyType);
}


//-----------------------------------------------------------------------------
/** SWDMX_PcrCallBack
 *  Set notify function pcr data
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL SWDMX_PcrCallBack(UINT32 u4PcrBase)
{
    return _SWDMX_PcrCallBack(u4PcrBase);
}


#ifdef CC_SWDMX_RENDER_PTS
//-----------------------------------------------------------------------------
/** SWDMX_SetRenderPts
 *  Set render pts
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
VOID SWDMX_SetRenderPts(UINT32 u4RenderPts)
{
    _SWDMX_SetRenderPts(u4RenderPts);
}
#endif


#if 0//def CC_SWDMX_RENDER_POSITION
EXTERN VOID SWDMX_GetRenderPosition(INT32 *pi4TempRef, UINT16 *pu2DecodingOrder, UINT64 *pu8Offset, UINT64 *pu8OffsetI)
{
    _SWDMX_GetRenderPosition(pi4TempRef, pu2DecodingOrder, pu8Offset, pu8OffsetI);
}
EXTERN VOID SWDMX_SetRenderPosition(INT32 i4TempRef, UINT16 u2DecodingOrder, UINT64 u8Offset, UINT64 u8OffsetI)
{
    _SWDMX_SetRenderPosition(i4TempRef, u2DecodingOrder, u8Offset, u8OffsetI);
}
#endif

