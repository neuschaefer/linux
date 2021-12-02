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
 * $RCSfile: playmgr_if.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mtmplayer.c
 *  This header file declares exported APIs and data structures of MPlayer Module.
 */

/********************************************************************
  INCLUDE FILES
********************************************************************/
//#include "x_os.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "playmgr_if.h"
#include "stc_drvif.h"
#include "playmgr.h"
#include "playmgr_util.h"
#include "playmgr_debug.h"
#include "x_util.h"
LINT_EXT_HEADER_END

#if !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)

/********************************************************************
  MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
  MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
  TYPE DEFINITION
********************************************************************/

/********************************************************************
  VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
static PLAYMGR_STATUS_T _arPlaymgrCurStatus[PLAYMGR_MAX_INPUT_SRC];
#ifdef CC_SKYPE_FINE_INSTANCE
static BOOL _afgPlaymgrGetInfo[PLAYMGR_MAX_INPUT_SRC] = {FALSE, FALSE, FALSE, FALSE, FALSE};
#elif defined(CC_FBM_TWO_FBP)
static BOOL _afgPlaymgrGetInfo[PLAYMGR_MAX_INPUT_SRC] = {FALSE, FALSE};
#else
static BOOL _afgPlaymgrGetInfo[PLAYMGR_MAX_INPUT_SRC] = {FALSE};
#endif

/**Initialize MPlayer 
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Init(VOID)
{
    UINT8 i;
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Init();
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_INIT;
        for (i = 0; i < PLAYMGR_MAX_INPUT_SRC; i++)
        {
            fgRet = _PLAYMGR_SetCMD(i, rStatus);
            if (!fgRet)
            {
                return fgRet;
            }
        }
    }
    
    return fgRet; 
}

/**Initialize MPlayer 
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Open(UINT8 u1SrcId, UCHAR* pucFileName)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet, fgParsingOK = FALSE;

    _afgPlaymgrGetInfo[u1SrcId] = FALSE;
    x_memset(&_arPlaymgrCurStatus[u1SrcId], 0, sizeof(PLAYMGR_STATUS_T));

    fgRet = _PLAYMGR_Open(u1SrcId, (CHAR*) pucFileName);
    if (fgRet)
    {        
        rStatus.eCmd = PLAYMGR_SETCMD_OPEN;
        fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
        if (fgRet)
        {
            fgParsingOK = _PLAYMGR_GetInfo(u1SrcId, &_arPlaymgrCurStatus[u1SrcId]);   
            if (fgParsingOK)        
            {            
                rStatus.eCmd = PLAYMGR_SETCMD_STOP;
            }
            else
            {
                rStatus.eCmd = PLAYMGR_SETCMD_CLOSE;
            }
            fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
        }
    }    

    _afgPlaymgrGetInfo[u1SrcId] = fgParsingOK;
    return fgRet;     
}

/**Terminate Mplayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Close(UINT8 u1SrcId)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    _afgPlaymgrGetInfo[u1SrcId] = FALSE;
    fgRet = _PLAYMGR_Close(u1SrcId);
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_CLOSE;
        fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    }
    
    return fgRet; 
}

/**Reset Mplayer 
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Reset(UINT8 u1SrcId)
{
    return TRUE;
}

/**Set the stream settings to MPlayer
*@param MPlayer Handle
*@param pSettings
*@param fgSetPos              set position?
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Set(UINT8 u1SrcId, PLAYMGR_SETTING_T* prSettings)
{
    BOOL fgRet;
    PLAYMGR_STATUS_T rPlaymgrCurStatus;
    if(prSettings == NULL)
    {
        //LOG(1, "PLAYMGR_Set fail, prSettings NULL\n");
        return FALSE;
    }
    fgRet = _PLAYMGR_Set(u1SrcId, prSettings);
    if(fgRet)
    {
        if(prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PROGRAM)
        {
            if (_afgPlaymgrGetInfo[u1SrcId])
            {
                fgRet = PLAYMGR_GetInfoByPgm(u1SrcId, &rPlaymgrCurStatus,
                    prSettings->u2PlayPgm);
                UNUSED(rPlaymgrCurStatus);
            }
        }
        // sync the info with playmgr.c
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_VIDEO)
        {
            _arPlaymgrCurStatus[u1SrcId].u4PlayVidIdx = prSettings->u2VideoPidIdx;
            _arPlaymgrCurStatus[u1SrcId].u4PlayVidStrmId = prSettings->u2VideoPid;
            _arPlaymgrCurStatus[u1SrcId].au4VideoId[0] = prSettings->u2VideoPid;
            _arPlaymgrCurStatus[u1SrcId].u2EnVTrackNum = 0;
            _arPlaymgrCurStatus[u1SrcId].u2VideoTracks = 1;
            _arPlaymgrCurStatus[u1SrcId].aeVidFormat[0] = prSettings->eVdecFmt;
        }
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_AUDIO)
        {
            _arPlaymgrCurStatus[u1SrcId].u4PlayAudIdx = prSettings->u2AudioPidIdx;
            _arPlaymgrCurStatus[u1SrcId].u4PlayAudStrmId = prSettings->u2AudioPid;
            _arPlaymgrCurStatus[u1SrcId].au4AudioId[0] = prSettings->u2AudioPid;
            _arPlaymgrCurStatus[u1SrcId].u2EnATrackNum = 0;
            _arPlaymgrCurStatus[u1SrcId].u2AudioTracks = 1;
            _arPlaymgrCurStatus[u1SrcId].aeAudFormat[0] = prSettings->eAdecFmt;
        }
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_PCR)
        {
            _arPlaymgrCurStatus[u1SrcId].u4PlayPcrIdx = prSettings->u2PcrPidIdx;
            _arPlaymgrCurStatus[u1SrcId].u4PlayPcrStrmId = prSettings->u2PcrPid;
            _arPlaymgrCurStatus[u1SrcId].au4PcrId[0] = prSettings->u2PcrPid;
            _arPlaymgrCurStatus[u1SrcId].u2EnPcrTrackNum = 0;
            _arPlaymgrCurStatus[u1SrcId].u2PcrTracks = 1;
        }
    }
    return fgRet;
}

/**Set the stream settings to MPlayer
*@param MPlayer Handle
*@param pSettings
*@param fgSetPos              set position?
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Get(UINT8 u1SrcId, PLAYMGR_SETTING_T* prSettings)
{
    return _PLAYMGR_Get(u1SrcId, prSettings);
}


/**Get playback capability
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_GetCap(UINT8 u1SrcId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2)
{
	return _PLAYMGR_GetCap(u1SrcId, u4Type, u4Param1, u4Param2);
}


/**Start Play.
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Start(UINT8 u1SrcId)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Start(u1SrcId);
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_START;
        fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    }
    
    return fgRet; 
}

/**Stop MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Stop(UINT8 u1SrcId)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Stop(u1SrcId);
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_STOP;
        fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    }
    
    return fgRet; 
}


/**Abort.
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Abort(UINT8 u1SrcId)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;
    
    rStatus.eCmd = PLAYMGR_SETCMD_ABORT;
    fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    
    return fgRet; 
}

/**Pause MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Pause(UINT8 u1SrcId)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Pause(u1SrcId);
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_PAUSE;
        fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    }
    
    return fgRet; 
}

BOOL PLAYMGR_ABRepeat(UINT8 u1SrcId, PLAYMGR_ABREPEAT_TYPE_T eMode)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_ABRepeat(u1SrcId, eMode);

    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_ABREPEAT;
        fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    }
    
    return fgRet; 
}

/**Resume MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Resume(UINT8 u1SrcId)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Resume(u1SrcId);
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_RESUME;
        fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    }
    
    return fgRet; 
}

/**Get the stream settings and status from MPlayer
*@param handle
*@param prSettings
*@param prStatus
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_GetInfo(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus)
{    
    BOOL fgRet;
    
    if (prStatus == NULL)
    {
        return FALSE;
    }
    
    if (!_afgPlaymgrGetInfo[u1SrcId])
    {
        _afgPlaymgrGetInfo[u1SrcId] = TRUE;
        fgRet = _PLAYMGR_GetInfo(u1SrcId, &_arPlaymgrCurStatus[u1SrcId]);         
    }
    else
    {
        fgRet = TRUE;
    }
    
    x_memcpy(prStatus, &_arPlaymgrCurStatus[u1SrcId], sizeof(PLAYMGR_STATUS_T));
    prStatus->u4TotalDuration /= 90;//1000 / 90000
    _arPlaymgrCurStatus[u1SrcId].u4CurPos = PLAYMGR_GetPos(u1SrcId);
    prStatus->u4CurPos = _arPlaymgrCurStatus[u1SrcId].u4CurPos;
    _arPlaymgrCurStatus[u1SrcId].u4CurTick = PLAYMGR_GetTick(u1SrcId);
    prStatus->u4CurTick = _arPlaymgrCurStatus[u1SrcId].u4CurTick;
    _arPlaymgrCurStatus[u1SrcId].u8CurBytePos = PLAYMGR_GetBytePos(u1SrcId);
    prStatus->u8CurBytePos = _arPlaymgrCurStatus[u1SrcId].u8CurBytePos;
    _arPlaymgrCurStatus[u1SrcId].i4Speed = _PLAYMGR_GetSpeed(u1SrcId);
    prStatus->i4Speed = _arPlaymgrCurStatus[u1SrcId].i4Speed;
    _arPlaymgrCurStatus[u1SrcId].fgDataExhausted = _PLAYMGR_GetDataExhausted(u1SrcId);
    prStatus->fgDataExhausted = _arPlaymgrCurStatus[u1SrcId].fgDataExhausted;
    return fgRet;
}

/**Get the audio stream settings and status from MPlayer
*@param handle
*@param prSettings
*@param prStatus
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_GetAudInfo(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus)
{        
    if (prStatus == NULL)
    {
        return FALSE;
    }
    
    x_memcpy(prStatus, &_arPlaymgrCurStatus[u1SrcId], sizeof(PLAYMGR_STATUS_T));
    prStatus->u4TotalDuration /= 90;
    _arPlaymgrCurStatus[u1SrcId].u4CurPos = PLAYMGR_GetPos(u1SrcId);
    prStatus->u4CurPos = _arPlaymgrCurStatus[u1SrcId].u4CurPos;
    _arPlaymgrCurStatus[u1SrcId].u8CurBytePos = PLAYMGR_GetBytePos(u1SrcId);
    prStatus->u8CurBytePos = _arPlaymgrCurStatus[u1SrcId].u8CurBytePos;
    _arPlaymgrCurStatus[u1SrcId].i4Speed = _PLAYMGR_GetSpeed(u1SrcId);
    prStatus->i4Speed = _arPlaymgrCurStatus[u1SrcId].i4Speed;
    
    return TRUE;
}

/**Get the stream settings and status from MPlayer
*@param handle
*@param prStatus
*@param u4Pgm
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_GetInfoByPgm(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus, UINT32 u4Pgm)
{    
    BOOL fgRet;
    
    if (prStatus == NULL)
    {
        return FALSE;
    }
    
    if ((!_afgPlaymgrGetInfo[u1SrcId]) || 
        (u4Pgm >= _arPlaymgrCurStatus[u1SrcId].u2TotalPrmNum))
    {
        return FALSE;     
    }
    
    fgRet = _PLAYMGR_GetInfoByPgm(u1SrcId, &_arPlaymgrCurStatus[u1SrcId], u4Pgm);
    
    x_memcpy((VOID*)prStatus, (VOID*)&_arPlaymgrCurStatus[u1SrcId], sizeof(PLAYMGR_STATUS_T));
    prStatus->u4TotalDuration /= 90;//1000 / 90000
    _arPlaymgrCurStatus[u1SrcId].u4CurPos = PLAYMGR_GetPos(u1SrcId);
    prStatus->u4CurPos = _arPlaymgrCurStatus[u1SrcId].u4CurPos;
    _arPlaymgrCurStatus[u1SrcId].u8CurBytePos = PLAYMGR_GetBytePos(u1SrcId);
    prStatus->u8CurBytePos = _arPlaymgrCurStatus[u1SrcId].u8CurBytePos;
    _arPlaymgrCurStatus[u1SrcId].i4Speed = _PLAYMGR_GetSpeed(u1SrcId);
    prStatus->i4Speed = _arPlaymgrCurStatus[u1SrcId].i4Speed;
    
    return fgRet;
}

/**Register the callback function of MPlayer
 MPlayer can uses the registered callback function to notify the upper layer. 
*@param handle
*@param eCallbackType
*@param callback funtion pointer
*@param reserved
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_RegCb(UINT8 u1SrcId, PLAYMGR_CALLBACK_TYPE_T eCallbackType, 
                                     PLAYMGR_CALLBACK fnCallback, 
                                     UINT32 u4Param)
{
    return _PLAYMGR_RegCb(u1SrcId, eCallbackType, fnCallback, u4Param);
}

/**Set the seek position by time.
*@param handle
*@param speed (1000 base -> 1x)
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Speed(UINT8 u1SrcId, INT32 i4Speed)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Speed(u1SrcId, i4Speed);

    if (fgRet && (i4Speed != 1000))
    {
        rStatus.eCmd = PLAYMGR_SETCMD_TRICK;
        fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    }
    
    return fgRet; 
}

/**Set the seek position by time.
*@param handle
*@param seek time(millisecond)
*@retval file pos
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_SeekTime(UINT8 u1SrcId, UINT32 u4SeekTime)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    rStatus.eCmd = PLAYMGR_SETCMD_SEEKTIME;
    rStatus.u4Param1 = u4SeekTime;
    fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    
    return fgRet; 
}


/**Set the seek position by bytes.
*@param handle
*@param seek bytes
*@retval file pos
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_SeekPos(UINT8 u1SrcId, UINT64 u8SeekBytes)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    rStatus.eCmd = PLAYMGR_SETCMD_SEEKPOSITION;
    rStatus.u4Param1 = (UINT32)(u8SeekBytes>>32);
    rStatus.u4Param2 = (UINT32)(u8SeekBytes&0xFFFFFFFF);
    fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    
    return fgRet; 
}

/**Set the seek position by time.
*@param handle
*@param seek percent
*@retval file pos
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_SeekPercent(UINT8 u1SrcId, UINT32 u4SeekPercent)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    rStatus.eCmd = PLAYMGR_SETCMD_SEEKPERCENT;
    rStatus.u4Param1 = u4SeekPercent;
    fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    
    return fgRet; 
}

BOOL PLAYMGR_GetMetaData(UINT8 u1SrcId, PLAYMGR_AVMETA_T *prMeta)
{    
    return _PLAYMGR_GetMetaData(u1SrcId, prMeta);
}

/**get current position(millisecond)
*@retval position
*/
UINT32 PLAYMGR_GetPos(UINT8 u1SrcId)
{
    return _PLAYMGR_GetPos(u1SrcId);
}

/**get current tick
*@retval tick number
*/
UINT32 PLAYMGR_GetTick(UINT8 u1SrcId)
{
    return _PLAYMGR_GetTick(u1SrcId);
}

/**get current position in bytes
*@retval position
*/
UINT64 PLAYMGR_GetBytePos(UINT8 u1SrcId)
{
    return _PLAYMGR_GetBytePos(u1SrcId);
}

BOOL PLAYMGR_SetFifo(UINT8 u1SrcId, PLAYMGR_FIFO_T *prFifo)
{
    return _PLAYMGR_SetFifo(u1SrcId, prFifo);
}

BOOL PLAYMGR_Step(UINT8 u1SrcId, INT32 i4StepAmount)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    rStatus.eCmd = PLAYMGR_SETCMD_STEP;
    rStatus.u4Param1 = (UINT32)i4StepAmount;
    fgRet = _PLAYMGR_SetCMD(u1SrcId, rStatus);
    
    return fgRet; 
}

BOOL PLAYMGR_SetAudTime(UINT8 u1SrcId, PLAYMGR_AUDTIME_T eCmd, INT32 i4Time)
{
    UINT32 u4PTS;

    STC_SetSyncMode(STC_SRC_A1, AUD_DEC_MAIN, ES0, AV_SYNC_MODE_SYSTEM_MASTER);
    
    switch (eCmd)
    {
    case PLAYMGR_AUDTIME_SEEK:
        u4PTS = ((UINT32)i4Time * 90);
        STC_StopStc(STC_SRC_A1);
        STC_SetStcValue(STC_SRC_A1, u4PTS);
        STC_StartStc(STC_SRC_A1);
        break;
    case PLAYMGR_AUDTIME_PLAY:
        STC_StartStc(STC_SRC_A1);
        break;
    case PLAYMGR_AUDTIME_STOP:
        STC_StopStc(STC_SRC_A1);
        break;
    default:
        return FALSE;
    }
    
    return TRUE;
}   

#ifdef __KERNEL__
/**Nofify playmgr to get data
*@param u4Size                Received data size
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_ReceiveData(UINT8 u1SrcId, UINT32 u4Size, UINT64 u8Pos)
{    
    return _PLAYMGR_ReceiveData(u1SrcId, u4Size, u8Pos);
}

#endif


BOOL PLAYMGR_ChkSubtitle(UINT8 u1SrcId, BOOL fgDrawAck)
{
    return _PLAYMGR_ChkSubtitle(u1SrcId, fgDrawAck);
}


BOOL PLAYMGR_GetDivxDRMStatus(UINT8 u1SrcId, PLAYMGR_DRMTYPE *peDRMType, UINT32 *pu4DRMRentalLimit, UINT32 *pu4DRMRentalCount)
{
    return _PLAYMGR_GetDivxDRMStatus(u1SrcId, peDRMType, pu4DRMRentalLimit, pu4DRMRentalCount);
}


BOOL PLAYMGR_GetDivxDRMRegCode(UINT8 u1SrcId, char *pu1Code)
{
    return _PLAYMGR_GetDivxDRMRegCode(u1SrcId, pu1Code);
}


BOOL PLAYMGR_GetDivxDRMUIHelpInfo(UINT8 u1SrcId, UINT32 *pu4Info)
{
    return _PLAYMGR_GetDivxDRMUIHelpInfo(u1SrcId, pu4Info);
}


BOOL PLAYMGR_DivxDRMDeactivation(UINT8 u1SrcId, char *pu1Code)
{
    return _PLAYMGR_DivxDRMDeactivation(u1SrcId, pu1Code);
}


VOID PLAYMGR_SetSrcType(UINT8 u1SrcId, PLAYMGR_SourceType_T eSrcType)
{
    _PLAYMGR_SetSrcType(u1SrcId, eSrcType);
}

PLAYMGR_SourceType_T PLAYMGR_GetSrcType(UINT8 u1SrcId)
{
    return _PLAYMGR_GetSrcType(u1SrcId);
}

VOID PLAYMGR_SetNumberOfBitstream(UINT8 u1SrcId, UINT32 u4Size)
{
    _PLAYMGR_SetNumberOfBitstream( u1SrcId, u4Size);
}

UINT32 PLAYMGR_GetNumberOfBitstream(UINT8 u1SrcId)
{
    return _PLAYMGR_GetNumberOfBitstream(u1SrcId);
}

BOOL PLAYMGR_SetLPCMInfo(UINT8 u1SrcId, PLAYMGR_LPCM_INFO_T rPlayLPCMInfo)
{
/*
    if (_PLAYMGR_GetSrcType() == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        _PlaymgrCurStatus.u8FileSize = rPlayLPCMInfo.u8FileSize;
        _PlaymgrCurStatus.u4TotalDuration = (UINT32)(rPlayLPCMInfo.u8Duration * 1000);
    }        
*/
    
    return _PLAYMGR_SetLPCMInfo(u1SrcId, rPlayLPCMInfo);
}

VOID PLAYMGR_FeederReset(UINT8 u1SrcId)
{
   _PLAYMGR_FeederReset(u1SrcId);
}
#ifdef __MODEL_slt__
VOID PLAYMGR_SetContainerType(UINT32 u4ContainerType)
{
    _PLAYMGR_SetContainerType(u4ContainerType);
}
VOID PLAYMGR_SetSubType(UINT32 u4SubType)
{
    _PLAYMGR_SetSubType(u4SubType);
}

void PLAYMGR_VdecFlushCtrl(void)
{
    _PLAYMGR_SLTVdecFlushCtrl();
}

void PLAYMGR_VdecWaitCtrlDone(void)
{
    _PLAYMGR_SLTVdecWaitCtrlDone();
}

#endif

// !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)
#else

BOOL PLAYMGR_Open(UINT8 u1SrcId, UCHAR* pucFileName)
{
    UNUSED(u1SrcId);
    UNUSED(pucFileName);
    return FALSE;
}
BOOL PLAYMGR_Close(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return FALSE;
}
BOOL PLAYMGR_Init(VOID)
{
    return FALSE;
}
BOOL PLAYMGR_Reset(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return FALSE;
}
BOOL PLAYMGR_Get(UINT8 u1SrcId, PLAYMGR_SETTING_T* prSettings)
{
    UNUSED(u1SrcId);
    UNUSED(prSettings);
    return FALSE;
}
BOOL PLAYMGR_Set(UINT8 u1SrcId, PLAYMGR_SETTING_T* prSettings)
{
    UNUSED(u1SrcId);
    UNUSED(prSettings);
    return FALSE;
}
BOOL PLAYMGR_GetCap(UINT8 u1SrcId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2)
{
    UNUSED(u1SrcId);
    UNUSED(u4Type);
    UNUSED(u4Param1);
    UNUSED(u4Param2);
    return FALSE;
}
BOOL PLAYMGR_Start(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return FALSE;
}
BOOL PLAYMGR_Stop(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return FALSE;
}
BOOL PLAYMGR_Abort(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return FALSE;
}
BOOL PLAYMGR_Pause(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return FALSE;
}
BOOL PLAYMGR_Resume(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return FALSE;
}
BOOL PLAYMGR_ABRepeat(UINT8 u1SrcId, PLAYMGR_ABREPEAT_TYPE_T eMode)
{
    UNUSED(u1SrcId);
    UNUSED(eMode);
    return FALSE;
}
BOOL PLAYMGR_GetInfo(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus)
{
    UNUSED(u1SrcId);
    UNUSED(prStatus);
    return FALSE;
}
BOOL PLAYMGR_GetAudInfo(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus)
{
    UNUSED(u1SrcId);
    UNUSED(prStatus);
    return FALSE;
}
BOOL PLAYMGR_GetInfoByPgm(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus, UINT32 u4Pgm)
{
    UNUSED(u1SrcId);
    UNUSED(prStatus);
    UNUSED(u4Pgm);
    return FALSE;
}
BOOL PLAYMGR_RegCb(UINT8 u1SrcId, PLAYMGR_CALLBACK_TYPE_T eCallbackType,
                                     PLAYMGR_CALLBACK fnCallback,
                                     UINT32 u4Param)
{
    UNUSED(u1SrcId);
    UNUSED(eCallbackType);
    UNUSED(fnCallback);
    UNUSED(u4Param);
    return FALSE;
}
BOOL PLAYMGR_Speed(UINT8 u1SrcId, INT32 i4Speed)
{
    UNUSED(u1SrcId);
    UNUSED(i4Speed);
    return FALSE;
}
BOOL PLAYMGR_SeekTime(UINT8 u1SrcId, UINT32 u4SeekTime)
{
    UNUSED(u1SrcId);
    UNUSED(u4SeekTime);
    return FALSE;
}
BOOL PLAYMGR_SeekPercent(UINT8 u1SrcId, UINT32 u4SeekPercent)
{
    UNUSED(u1SrcId);
    UNUSED(u4SeekPercent);
    return FALSE;
}
BOOL PLAYMGR_SeekPos(UINT8 u1SrcId, UINT64 u8SeekBytes)
{
    UNUSED(u1SrcId);
    UNUSED(u8SeekBytes);
    return FALSE;
}
BOOL PLAYMGR_GetMetaData(UINT8 u1SrcId, PLAYMGR_AVMETA_T *prMeta)
{
    UNUSED(u1SrcId);
    UNUSED(prMeta);
    return FALSE;
}
UINT32 PLAYMGR_GetPos(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return 0;
}
UINT32 PLAYMGR_GetTick(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return 0;
}
UINT64 PLAYMGR_GetBytePos(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return 0;
}
BOOL PLAYMGR_SetFifo(UINT8 u1SrcId, PLAYMGR_FIFO_T *prFifo)
{
    UNUSED(u1SrcId);
    UNUSED(prFifo);
    return FALSE;
}
BOOL PLAYMGR_Step(UINT8 u1SrcId, INT32 i4StepAmount)
{
    UNUSED(u1SrcId);
    UNUSED(i4StepAmount);
    return FALSE;
}
BOOL PLAYMGR_SetAudTime(UINT8 u1SrcId, PLAYMGR_AUDTIME_T eCmd, INT32 i4Time)
{
    UNUSED(u1SrcId);
    UNUSED(eCmd);
    UNUSED(i4Time);
    return FALSE;
}
#ifdef __KERNEL__
BOOL PLAYMGR_ReceiveData(UINT8 u1SrcId, UINT32 u4Size, UINT64 u8Pos)
{
    UNUSED(u1SrcId);
    UNUSED(u4Size);
    UNUSED(u8Pos);
    return FALSE;
}
#endif

BOOL PLAYMGR_ChkSubtitle(UINT8 u1SrcId, BOOL fgDrawAck)
{
    UNUSED(u1SrcId);
    UNUSED(fgDrawAck);
    return FALSE;
}
BOOL PLAYMGR_GetDivxDRMStatus(UINT8 u1SrcId, PLAYMGR_DRMTYPE *peDRMType, UINT32 *pu4DRMRentalLimit, UINT32 *pu4DRMRentalCount)
{
    UNUSED(u1SrcId);
    UNUSED(peDRMType);
    UNUSED(pu4DRMRentalLimit);
    UNUSED(pu4DRMRentalCount);
    return FALSE;
}
BOOL PLAYMGR_GetDivxDRMRegCode(UINT8 u1SrcId, char *pu1Code)
{
    UNUSED(u1SrcId);
    UNUSED(pu1Code);
    return FALSE;
}
BOOL PLAYMGR_GetDivxDRMUIHelpInfo(UINT8 u1SrcId, UINT32 *pu4Info)
{
    UNUSED(u1SrcId);
    UNUSED(pu4Info);
    return FALSE;
}
BOOL PLAYMGR_DivxDRMDeactivation(UINT8 u1SrcId, char *pu1Code)
{
    UNUSED(u1SrcId);
    UNUSED(pu1Code);
    return FALSE;
}
VOID PLAYMGR_SetSrcType(UINT8 u1SrcId, PLAYMGR_SourceType_T eSrcType)
{
    UNUSED(u1SrcId);
    UNUSED(eSrcType);
    return;
}
PLAYMGR_SourceType_T PLAYMGR_GetSrcType(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return PLAYMGR_SRC_TYPE_UNKNOWN;
}
VOID PLAYMGR_SetNumberOfBitstream(UINT8 u1SrcId, UINT32 u4Size)
{
    UNUSED(u1SrcId);
    UNUSED(u4Size);
    return;
}
UINT32 PLAYMGR_GetNumberOfBitstream(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return 0;
}
BOOL PLAYMGR_SetLPCMInfo(UINT8 u1SrcId, PLAYMGR_LPCM_INFO_T rPlayLPCMInfo)
{
    UNUSED(u1SrcId);
    UNUSED(rPlayLPCMInfo);
    return FALSE;
}
VOID PLAYMGR_FeederReset(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return;
}

#ifdef __MODEL_slt__
VOID PLAYMGR_SetContainerType(UINT32 u4ContainerType)
{
    UNUSED(u4ContainerType);
    return;
}
VOID PLAYMGR_SetSubType(UINT32 u4SubType)
{
    UNUSED(u4SubType);
    return;
}
void PLAYMGR_VdecFlushCtrl(void)
{
    return;
}
void PLAYMGR_VdecWaitCtrlDone(void)
{
    return;
}
#endif

// !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)
#endif


