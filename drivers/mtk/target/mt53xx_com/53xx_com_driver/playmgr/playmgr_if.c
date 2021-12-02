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
static PLAYMGR_STATUS_T _PlaymgrCurStatus;
static BOOL _fgPlaymgrGetInfo = FALSE;

/**Initialize MPlayer 
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Init(VOID)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Init();
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_INIT;
        fgRet = _PLAYMGR_SetCMD(rStatus);
    }
    
    return fgRet; 
}

/**Initialize MPlayer 
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Open(UCHAR* pucFileName)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet, fgParsingOK = FALSE;

    _fgPlaymgrGetInfo = FALSE;
    x_memset(&_PlaymgrCurStatus, 0, sizeof(PLAYMGR_STATUS_T));

    fgRet = _PLAYMGR_Open((CHAR*) pucFileName);
    if (fgRet)
    {        
        rStatus.eCmd = PLAYMGR_SETCMD_OPEN;
        fgRet = _PLAYMGR_SetCMD(rStatus);
        if (fgRet)
        {
            fgParsingOK = _PLAYMGR_GetInfo(&_PlaymgrCurStatus);   
            if (fgParsingOK)        
            {            
                rStatus.eCmd = PLAYMGR_SETCMD_STOP;
            }
            else
            {
                rStatus.eCmd = PLAYMGR_SETCMD_CLOSE;
            }
            fgRet = _PLAYMGR_SetCMD(rStatus);
        }
    }    

    _fgPlaymgrGetInfo = fgParsingOK;
    return fgRet;     
}

/**Terminate Mplayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Close(VOID)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    _fgPlaymgrGetInfo = FALSE;
    fgRet = _PLAYMGR_Close();
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_CLOSE;
        fgRet = _PLAYMGR_SetCMD(rStatus);
    }
    
    return fgRet; 
}

/**Reset Mplayer 
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Reset(VOID)
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
BOOL PLAYMGR_Set(PLAYMGR_SETTING_T* prSettings)
{
    BOOL fgRet;
    PLAYMGR_STATUS_T rPlaymgrCurStatus;
    if(prSettings == NULL)
    {
        //LOG(1, "PLAYMGR_Set fail, prSettings NULL\n");
        return FALSE;
    }
    fgRet = _PLAYMGR_Set(prSettings);
    if(fgRet)
    {
        if(prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PROGRAM)
        {
            if (_fgPlaymgrGetInfo)
            {
                fgRet = PLAYMGR_GetInfoByPgm(&rPlaymgrCurStatus,
                    prSettings->u2PlayPgm);
                UNUSED(rPlaymgrCurStatus);
            }
        }
        // sync the info with playmgr.c
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_VIDEO)
        {
            _PlaymgrCurStatus.u4PlayVidIdx = prSettings->u2VideoPidIdx;
            _PlaymgrCurStatus.u4PlayVidStrmId = prSettings->u2VideoPid;
            _PlaymgrCurStatus.au4VideoId[0] = prSettings->u2VideoPid;
            _PlaymgrCurStatus.u2EnVTrackNum = 0;
            _PlaymgrCurStatus.u2VideoTracks = 1;
            _PlaymgrCurStatus.aeVidFormat[0] = prSettings->eVdecFmt;
        }
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_AUDIO)
        {
            _PlaymgrCurStatus.u4PlayAudIdx = prSettings->u2AudioPidIdx;
            _PlaymgrCurStatus.u4PlayAudStrmId = prSettings->u2AudioPid;
            _PlaymgrCurStatus.au4AudioId[0] = prSettings->u2AudioPid;
            _PlaymgrCurStatus.u2EnATrackNum = 0;
            _PlaymgrCurStatus.u2AudioTracks = 1;
            _PlaymgrCurStatus.aeAudFormat[0] = prSettings->eAdecFmt;
        }
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_PCR)
        {
            _PlaymgrCurStatus.u4PlayPcrIdx = prSettings->u2PcrPidIdx;
            _PlaymgrCurStatus.u4PlayPcrStrmId = prSettings->u2PcrPid;
            _PlaymgrCurStatus.au4PcrId[0] = prSettings->u2PcrPid;
            _PlaymgrCurStatus.u2EnPcrTrackNum = 0;
            _PlaymgrCurStatus.u2PcrTracks = 1;
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
BOOL PLAYMGR_Get(PLAYMGR_SETTING_T* prSettings)
{
    return _PLAYMGR_Get(prSettings);
}

/**Start Play.
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Start(VOID)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Start();
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_START;
        fgRet = _PLAYMGR_SetCMD(rStatus);
    }
    
    return fgRet; 
}

/**Stop MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Stop(VOID)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Stop();
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_STOP;
        fgRet = _PLAYMGR_SetCMD(rStatus);
    }
    
    return fgRet; 
}


/**Abort.
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Abort(VOID)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;
    
    rStatus.eCmd = PLAYMGR_SETCMD_ABORT;
    fgRet = _PLAYMGR_SetCMD(rStatus);
    
    return fgRet; 
}

/**Pause MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Pause(VOID)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Pause();
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_PAUSE;
        fgRet = _PLAYMGR_SetCMD(rStatus);
    }
    
    return fgRet; 
}

/**Resume MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Resume(VOID)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Resume();
    if (fgRet)
    {
        rStatus.eCmd = PLAYMGR_SETCMD_RESUME;
        fgRet = _PLAYMGR_SetCMD(rStatus);
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
BOOL PLAYMGR_GetInfo(PLAYMGR_STATUS_T *prStatus)
{    
    BOOL fgRet;
    
    if (prStatus == NULL)
    {
        return FALSE;
    }
    
    if (!_fgPlaymgrGetInfo)
    {
        _fgPlaymgrGetInfo = TRUE;
        fgRet = _PLAYMGR_GetInfo(&_PlaymgrCurStatus);         
    }
    else
    {
        fgRet = TRUE;
    }
    
    x_memcpy(prStatus, &_PlaymgrCurStatus, sizeof(PLAYMGR_STATUS_T));
    prStatus->u4TotalDuration /= 90;//1000 / 90000
    _PlaymgrCurStatus.u4CurPos = PLAYMGR_GetPos();
    prStatus->u4CurPos = _PlaymgrCurStatus.u4CurPos;
    _PlaymgrCurStatus.u4CurTick = PLAYMGR_GetTick();
    prStatus->u4CurTick = _PlaymgrCurStatus.u4CurTick;
    _PlaymgrCurStatus.u8CurBytePos = PLAYMGR_GetBytePos();
    prStatus->u8CurBytePos = _PlaymgrCurStatus.u8CurBytePos;
    _PlaymgrCurStatus.i4Speed = _PLAYMGR_GetSpeed();
    prStatus->i4Speed = _PlaymgrCurStatus.i4Speed;
    _PlaymgrCurStatus.fgDataExhausted = _PLAYMGR_GetDataExhausted();
    prStatus->fgDataExhausted = _PlaymgrCurStatus.fgDataExhausted;
    return fgRet;
}

/**Get the audio stream settings and status from MPlayer
*@param handle
*@param prSettings
*@param prStatus
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_GetAudInfo(PLAYMGR_STATUS_T *prStatus)
{        
    if (prStatus == NULL)
    {
        return FALSE;
    }
    
    x_memcpy(prStatus, &_PlaymgrCurStatus, sizeof(PLAYMGR_STATUS_T));
    prStatus->u4TotalDuration /= 90;
    _PlaymgrCurStatus.u4CurPos = PLAYMGR_GetPos();
    prStatus->u4CurPos = _PlaymgrCurStatus.u4CurPos;
    _PlaymgrCurStatus.u8CurBytePos = PLAYMGR_GetBytePos();
    prStatus->u8CurBytePos = _PlaymgrCurStatus.u8CurBytePos;
    _PlaymgrCurStatus.i4Speed = _PLAYMGR_GetSpeed();
    prStatus->i4Speed = _PlaymgrCurStatus.i4Speed;
    
    return TRUE;
}

/**Get the stream settings and status from MPlayer
*@param handle
*@param prStatus
*@param u4Pgm
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_GetInfoByPgm(PLAYMGR_STATUS_T *prStatus, UINT32 u4Pgm)
{    
    BOOL fgRet;
    
    if (prStatus == NULL)
    {
        return FALSE;
    }
    
    if ((!_fgPlaymgrGetInfo) || 
        (u4Pgm >= _PlaymgrCurStatus.u2TotalPrmNum))
    {
        return FALSE;     
    }
    
    fgRet = _PLAYMGR_GetInfoByPgm(&_PlaymgrCurStatus, u4Pgm);
    
    x_memcpy((VOID*)prStatus, (VOID*)&_PlaymgrCurStatus, sizeof(PLAYMGR_STATUS_T));
    prStatus->u4TotalDuration /= 90;//1000 / 90000
    _PlaymgrCurStatus.u4CurPos = PLAYMGR_GetPos();
    prStatus->u4CurPos = _PlaymgrCurStatus.u4CurPos;
    _PlaymgrCurStatus.u8CurBytePos = PLAYMGR_GetBytePos();
    prStatus->u8CurBytePos = _PlaymgrCurStatus.u8CurBytePos;
    _PlaymgrCurStatus.i4Speed = _PLAYMGR_GetSpeed();
    prStatus->i4Speed = _PlaymgrCurStatus.i4Speed;
    
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
BOOL PLAYMGR_RegCb(PLAYMGR_CALLBACK_TYPE_T eCallbackType, 
                                     PLAYMGR_CALLBACK fnCallback, 
                                     UINT32 u4Param)
{
    return _PLAYMGR_RegCb(eCallbackType, fnCallback, u4Param);
}

/**Set the seek position by time.
*@param handle
*@param speed (1000 base -> 1x)
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_Speed(INT32 i4Speed)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    fgRet = _PLAYMGR_Speed(i4Speed);

    if (fgRet && (i4Speed != 1000))
    {
        rStatus.eCmd = PLAYMGR_SETCMD_TRICK;
        fgRet = _PLAYMGR_SetCMD(rStatus);
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
BOOL PLAYMGR_SeekTime(UINT32 u4SeekTime)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    rStatus.eCmd = PLAYMGR_SETCMD_SEEKTIME;
    rStatus.u4Param1 = u4SeekTime;
    fgRet = _PLAYMGR_SetCMD(rStatus);
    
    return fgRet; 
}


/**Set the seek position by bytes.
*@param handle
*@param seek bytes
*@retval file pos
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_SeekPos(UINT64 u8SeekBytes)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    rStatus.eCmd = PLAYMGR_SETCMD_SEEKPOSITION;
    rStatus.u4Param1 = (UINT32)(u8SeekBytes>>32);
    rStatus.u4Param2 = (UINT32)(u8SeekBytes&0xFFFFFFFF);
    fgRet = _PLAYMGR_SetCMD(rStatus);
    
    return fgRet; 
}

/**Set the seek position by time.
*@param handle
*@param seek percent
*@retval file pos
*@retval TRUE                    Success.
*@retval FALSE                Fail.
*/
BOOL PLAYMGR_SeekPercent(UINT32 u4SeekPercent)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    rStatus.eCmd = PLAYMGR_SETCMD_SEEKPERCENT;
    rStatus.u4Param1 = u4SeekPercent;
    fgRet = _PLAYMGR_SetCMD(rStatus);
    
    return fgRet; 
}

BOOL PLAYMGR_GetMetaData(PLAYMGR_AVMETA_T *prMeta)
{    
    return _PLAYMGR_GetMetaData(prMeta);
}

/**get current position(millisecond)
*@retval position
*/
UINT32 PLAYMGR_GetPos(VOID)
{
    return _PLAYMGR_GetPos();
}

/**get current tick
*@retval tick number
*/
UINT32 PLAYMGR_GetTick(VOID)
{
    return _PLAYMGR_GetTick();
}

/**get current position in bytes
*@retval position
*/
UINT64 PLAYMGR_GetBytePos(VOID)
{
    return _PLAYMGR_GetBytePos();
}

BOOL PLAYMGR_SetFifo(PLAYMGR_FIFO_T *prFifo)
{
    return _PLAYMGR_SetFifo(prFifo);
}

BOOL PLAYMGR_Step(INT32 i4StepAmount)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    BOOL fgRet;

    rStatus.eCmd = PLAYMGR_SETCMD_STEP;
    rStatus.u4Param1 = (UINT32)i4StepAmount;
    fgRet = _PLAYMGR_SetCMD(rStatus);
    
    return fgRet; 
}

BOOL PLAYMGR_SetAudTime(PLAYMGR_AUDTIME_T eCmd, INT32 i4Time)
{
    UINT32 u4PTS;

    STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
    
    switch (eCmd)
    {
    case PLAYMGR_AUDTIME_SEEK:
        u4PTS = ((UINT32)i4Time * 90);
        STC_StopStc();
        STC_SetStcValue(u4PTS);
        STC_StartStc();
        break;
    case PLAYMGR_AUDTIME_PLAY:
        STC_StartStc();
        break;
    case PLAYMGR_AUDTIME_STOP:
        STC_StopStc();
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
BOOL PLAYMGR_ReceiveData(UINT32 u4Size, UINT64 u8Pos)
{    
    return _PLAYMGR_ReceiveData(u4Size, u8Pos);
}

#endif


BOOL PLAYMGR_ChkSubtitle(BOOL fgDrawAck)
{
    return _PLAYMGR_ChkSubtitle(fgDrawAck);
}


BOOL PLAYMGR_GetDivxDRMStatus(PLAYMGR_DRMTYPE *peDRMType, UINT32 *pu4DRMRentalLimit, UINT32 *pu4DRMRentalCount)
{
    return _PLAYMGR_GetDivxDRMStatus(peDRMType, pu4DRMRentalLimit, pu4DRMRentalCount);
}


BOOL PLAYMGR_GetDivxDRMRegCode(char *pu1Code)
{
    return _PLAYMGR_GetDivxDRMRegCode(pu1Code);
}


BOOL PLAYMGR_GetDivxDRMUIHelpInfo(UINT32 *pu4Info)
{
    return _PLAYMGR_GetDivxDRMUIHelpInfo(pu4Info);
}


BOOL PLAYMGR_DivxDRMDeactivation(char *pu1Code)
{
    return _PLAYMGR_DivxDRMDeactivation(pu1Code);
}


VOID PLAYMGR_SetSrcType(PLAYMGR_SourceType_T eSrcType)
{
    _PLAYMGR_SetSrcType(eSrcType);
}

PLAYMGR_SourceType_T PLAYMGR_GetSrcType(VOID)
{
    return _PLAYMGR_GetSrcType();
}

VOID PLAYMGR_SetNumberOfBitstream(UINT32 u4Size)
{
    _PLAYMGR_SetNumberOfBitstream(u4Size);
}

UINT32 PLAYMGR_GetNumberOfBitstream(VOID)
{
    return _PLAYMGR_GetNumberOfBitstream();
}

BOOL PLAYMGR_SetLPCMInfo(PLAYMGR_LPCM_INFO_T rPlayLPCMInfo)
{
/*
    if (_PLAYMGR_GetSrcType() == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        _PlaymgrCurStatus.u8FileSize = rPlayLPCMInfo.u8FileSize;
        _PlaymgrCurStatus.u4TotalDuration = (UINT32)(rPlayLPCMInfo.u8Duration * 1000);
    }        
*/
    
    return _PLAYMGR_SetLPCMInfo(rPlayLPCMInfo);
}


#ifdef __MODEL_slt__
VOID PLAYMGR_SetContainerType(UINT32 u4ContainerType)
{
    _PLAYMGR_SetContainerType(u4ContainerType);
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
