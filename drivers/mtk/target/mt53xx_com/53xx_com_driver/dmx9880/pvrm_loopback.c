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
 * $RCSfile: pvrm_loopback.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fvr_app.c
 *  FVR demo application
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_hal_arm.h"
#include "x_timer.h"
#include "dmx.h"
#include "fvr.h"
#include "dmx_mm_if.h"
#include "fvr_if.h"

#include "fvr_debug.h"

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

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
// Loopback
static UINT32 _u4LoopbackDistance;

static HANDLE_T _hPVRMTestSema = NULL_HANDLE;

static HAL_TIME_T _rPVRMTestTime;
static UINT32 _u4PVRMPktCount = 0;

static HANDLE_T _hPVRMLPThread = NULL_HANDLE;
static BOOL _fgPVRMLPEnable = FALSE;
static volatile UINT32 _u4PVRMLPBufMax = 0x0;
static volatile UINT32 _u4PVRMLPBufMin = 0xFFFFFFFF;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _PVRMLPThread
 */
//-----------------------------------------------------------------------------
static VOID _PVRMLPThread(VOID* pvArg)
{
    UINT32 u4RecWp1, u4RecWp2, u4PlayRp;
    UINT32 u4DataSize;
    FVR_GBL_T rGBL;

    UNUSED(pvArg);

    while(1)
    {
        if(_fgPVRMLPEnable)
        {
            VERIFY(FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL));
            u4DataSize = DATASIZE(rGBL.u4Rp, rGBL.u4Wp, rGBL.u4BufSize);
            if((_u4LoopbackDistance != 0) && (_u4LoopbackDistance > u4DataSize))
            {
                LOG(9, "Skip update play WP\n");
                x_thread_delay(10);
                continue;
            }
            _u4LoopbackDistance = 0x0;  // Only first time is effective
            if(u4DataSize > _u4PVRMLPBufMax)
            {
                _u4PVRMLPBufMax = u4DataSize;
            }

            if(u4DataSize < _u4PVRMLPBufMin)
            {
                _u4PVRMLPBufMin = u4DataSize;
            }

            //---------------------------------------------
            do
            {
                u4RecWp1 = FVR_GBL_ARY_W(0, 3);
                u4RecWp2 = FVR_GBL_ARY_W(0, 3);
            } while(u4RecWp1 != u4RecWp2);

            _DMX_Lock();
            u4PlayRp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP);
            
            // Update new pointer
            FVR_GBL_ARY_W(0, 4) = u4PlayRp;
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, u4RecWp1);
            _DMX_Unlock();
        }

        x_thread_delay(10);
    }
}


//-----------------------------------------------------------------------------
/** _PVRMLPRecNotify
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _PVRMLPRecNotify(FVR_GBL_NOTIFY_CODE_T eCode, UINT32 u4Data, const void* pvTag)
{   
#if 0
    FVR_GBL_PKTNOTIFY_INFO_T* prPktInfo;
    UINT32 u4DataSize;

    prPktInfo = (FVR_GBL_PKTNOTIFY_INFO_T*)u4Data;
    
    u4DataSize = FVR_GetDataSize();

    if((_u4LoopbackDistance != 0) && (_u4LoopbackDistance > u4DataSize))
    {
        LOG(9, "Skip update play WP\n");
        return;
    }
    _u4LoopbackDistance = 0x0;  // Only first time is effective    

    if(!DMX_PVRPlay_UpdateWp(prPktInfo->u4Addr, prPktInfo->u4Size))
    {
        LOG(1, "Update PVR play wp fail\n");
        return;
    }
#else
    UNUSED(eCode);
    UNUSED(u4Data);
    UNUSED(pvTag);
#endif
}

//-----------------------------------------------------------------------------
/** _PVRMLPPlayNotify
 */
//-----------------------------------------------------------------------------
static BOOL _PVRMLPPlayNotify(UINT32 u4Addr, UINT32 u4Size)
{
#if 0
    //Printf("Play, addr:0x%x, size:0x%x\n", u4Addr, u4Size);
    
    if(!FVR_UnlockBuffer(u4Addr, u4Size))   // Update capture read pointer
    {
        return FALSE;
    }
#else
    UNUSED(u4Addr);
    UNUSED(u4Size);
#endif
    return TRUE;
}

//-----------------------------------------------------------------------------
static void _PVRMLPTimeCallback(FVR_GBL_NOTIFY_CODE_T eCode, 
                             UINT32 u4Data, const void* pvTag)
{
    FVR_GBL_TIMENOTIFY_INFO_T *prTimeInfo;
    HAL_TIME_T rTime, rTimeDiff;
    UINT32 u4PktDiff;

    if(eCode == FVR_GBL_NFY_OVERFLOW)
    {
        VERIFY(x_sema_unlock(_hPVRMTestSema) == OSR_OK);
        return;
    }

    if(eCode != FVR_GBL_NFY_TIME)
    {
        return;
    }

    prTimeInfo = (FVR_GBL_TIMENOTIFY_INFO_T*)u4Data;    
    
    HAL_GetTime(&rTime);
    HAL_GetDeltaTime(&rTimeDiff, &_rPVRMTestTime, &rTime);
    _rPVRMTestTime = rTime;

    u4PktDiff = prTimeInfo->u4PktCount - _u4PVRMPktCount;
    _u4PVRMPktCount = prTimeInfo->u4PktCount;
    
    LOG(1, "%s, %d:%06d, pktcount: %d, size: %d\n", 
                        __FUNCTION__, rTimeDiff.u4Seconds, rTimeDiff.u4Micros, 
                        u4PktDiff, prTimeInfo->u4Size);

    ASSERT((u4PktDiff * 192) == prTimeInfo->u4Size);

    UNUSED(pvTag);
}


//-----------------------------------------------------------------------------
static BOOL _PVRMSetKey(void)
{
    UINT32 i, j;
    UINT32 au4Key[8], u4KeyLen;
    UINT32 au4IV[4], u4Key;
    BOOL fgCBC = FALSE;
    FVR_PID_T rFvrPid;
    DMX_PID_T rPid;
    UINT8 u1KeyIdx;
    BOOL afgKeyIdxUsed[8];

    srand(BIM_READ32(REG_RW_TIMER0_LOW));

    u4KeyLen = 128 + (random(3) * 64);//256;
    fgCBC = (BOOL)random(2);

    VERIFY(_DMX_SetAesKeyLen(0, u4KeyLen));

    // Generate key
    for(i=0; i<4; i++)
    {
        afgKeyIdxUsed[i] = FALSE;

        for(j=0; j<8; j++)
        {
            au4Key[j] = (UINT32)rand();
        }
        if(!_DMX_SetAesKey(i + 8, TRUE, au4Key))
        {
            return FALSE;
        }

        for(j=0; j<8; j++)
        {
            u4Key = au4Key[j];
            au4Key[j] = (GET_BYTE(u4Key, 0) << 24) | (GET_BYTE(u4Key, 1) << 16) |
                        (GET_BYTE(u4Key, 2) << 8) | (GET_BYTE(u4Key, 3));
        }
        if(!_FVR_SetAesKey(i, TRUE, (UINT8*)au4Key))
        {
            return FALSE;
        }

        for(j=0; j<8; j++)
        {
            au4Key[j] = (UINT32)rand();
        }
        if(!_DMX_SetAesKey(i + 8, FALSE, au4Key))
        {
            return FALSE;
        }

        for(j=0; j<8; j++)
        {
            u4Key = au4Key[j];
            au4Key[j] = (GET_BYTE(u4Key, 0) << 24) | (GET_BYTE(u4Key, 1) << 16) |
                        (GET_BYTE(u4Key, 2) << 8) | (GET_BYTE(u4Key, 3));
        }
        if(!_FVR_SetAesKey(i, FALSE, (UINT8*)au4Key))
        {
            return FALSE;
        }
    }

    if(fgCBC)
    {
        for(i=0; i<4; i++)
        {
            au4IV[i] = (UINT32)rand();
        }
        // 5368
        ////_DMX_SetAesIV(au4IV);
        for(i=0; i<4; i++)
        {
            VERIFY(_FVR_SetAesEvenOddIV(i, TRUE, au4IV));
            VERIFY(_FVR_SetAesEvenOddIV(i, FALSE, au4IV));
        }        
    }

    // Set record key
    u1KeyIdx = random(4);
    for(i=0; i<6; i++)
    {        
        afgKeyIdxUsed[u1KeyIdx] = TRUE;

        rFvrPid.u4KeyLen = u4KeyLen;
        rFvrPid.eKeyType = (random(2) == 1) ? FVR_KEY_EVEN : FVR_KEY_ODD;
        rFvrPid.fgCBCMode = fgCBC;
        rFvrPid.fgScramble = TRUE;
        rFvrPid.u1KeyIdx = u1KeyIdx;
        if(!_FVR_SetPid(i, FVR_PID_FLAG_CBC_MODE | FVR_PID_FLAG_KEY_TYPE | FVR_PID_FLAG_KEY_LEN | 
                           FVR_PID_FLAG_KEY_INDEX | FVR_PID_FLAG_SCRAMBLE_ENABLE, &rFvrPid))
        {
            return FALSE;
        }

        rPid.u1KeyIndex = u1KeyIdx;
        rPid.eDescMode = fgCBC ? DMX_DESC_MODE_AES_CBC : DMX_DESC_MODE_AES_ECB;
        //rPid.eDescMode = DMX_DESC_MODE_NONE;
        if(!_DMX_SetPid(i, DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_KEY_INDEX, &rPid, FALSE))
        {
            return FALSE;
        }
    }

    Printf("Key len:%d, CBC:%s, Idx:", u4KeyLen, fgCBC ? "Yes" : "No");
    for(i=0; i<8; i++)
    {
        if(afgKeyIdxUsed[i])
        {
            Printf("%d,",i);
        }
    }
    Printf("\n");
 
    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** PVRM_Loopback_Init
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL PVRM_Loopback_Init(UINT32 u4BufSize, UINT32 u4RecPktCount, UINT32 u4PlayThreshold)
{
    static BOOL _fgInit = FALSE;
    DMX_PVR_PLAY_T rPlay;
    FVR_GBL_T rGBL;
    FVR_TIMESTAMP_T rTimeStamp;

    if((FVR_IsRecording()) || 
       (DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING))
    {
        LOG(3, "Please stop first\n");
        return FALSE;
    }

    //------------------------------------------------
    //------------------------------------------------   
    if(!_fgInit)
    {
        if (x_thread_create(&_hPVRMLPThread, "PVRMLP", 1024, 60, _PVRMLPThread, 0, NULL) != OSR_OK)
        {
            LOG(1, "Fail to create PVRM loopback thread!\n");
            return FALSE;
        }

        _fgInit = TRUE;
    }

    _fgPVRMLPEnable = FALSE;

    //------------------------------------------------
    //------------------------------------------------
    if(!FVR_FreeBuffer())
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_FreeBuf())
    {
        return FALSE;
    }

    // Capture init
    x_memset((void*)&rGBL, 0, sizeof(rGBL));
    rGBL.fgAllocateBuffer = TRUE;
    rGBL.u2TargetPktCount = (UINT16)(u4RecPktCount & 0xFFFF);
    rGBL.u4BufSize = u4BufSize;
    rGBL.pfnNotifyFunc = _PVRMLPRecNotify;
    rGBL.pvNotifyTag = NULL;
    rGBL.u4TimeToNotify = 0;
    
    if(!FVR_SetGBL((UINT32)(FVR_GBL_FLAG_TARGET_PKT | FVR_GBL_FLAG_BUFFER | 
                    FVR_GBL_FLAG_TIME_INFO | FVR_GBL_FLAG_NOTIFY), &rGBL))
    {
        return FALSE;
    }

    if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return FALSE;
    }
    
    // Play init
    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = rGBL.u4BufStart;
    rPlay.u4BufSize = rGBL.u4BufSize;
    rPlay.u4ThresholdSize = u4PlayThreshold;
    rPlay.pfnPVRNotify = _PVRMLPPlayNotify;

    if(!DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_THRESHOLD |PVRPLAY_FLAGS_BUFFER | 
                        PVRPLAY_FLAGS_CALLBACK), &rPlay))
    {
        return FALSE;
    }

    // Enabe Record timestamp
    rTimeStamp.fgEnable = TRUE;
    rTimeStamp.u2ClkDiv = 8;
    rTimeStamp.u4Mask = 0xFFFFFFFF;
    rTimeStamp.u4Counter= 0x0;
    FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimeStamp);

    // Enable PLAY timestamp
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = FALSE;
    rPlay.u2TimeStampFreqDiv = 8;    
    if(!DMX_PVRPlay_Set((UINT32)PVRPLAY_FLAGS_TIMESTAMP, &rPlay))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** PVRM_Loopback_Start
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL PVRM_Loopback_Start(UINT32 u4Distance)
{

    if((FVR_IsRecording()) || 
       (DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING))
    {
        LOG(3, "Please stop first\n");
        return FALSE;
    }

    ////VERIFY(_PVRMSetKey());

    _u4LoopbackDistance = u4Distance;
    _fgPVRMLPEnable = TRUE;
    _u4PVRMLPBufMax = 0;
    _u4PVRMLPBufMin = 0xFFFFFFFF;

    if(!DMX_PVRPlay_Start())
    {
        return FALSE;
    }
    
    if(!FVR_Start())
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** PVRM_Loopback_Stop
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL PVRM_Loopback_Stop()
{
    if(!FVR_Stop())
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_Stop(TRUE))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** PVRM_Loopback_GetDataSize
 *  @retval UINT32
 */
//-----------------------------------------------------------------------------
void PVRM_Loopback_GetDataSize(UINT32 *pu4Size, UINT32 *pu4Max, UINT32 *pu4Min)
{
    DMX_PVR_PLAY_T rPlay;

    if(!DMX_PVRPlay_Get(PVRPLAY_FLAGS_BUFFER, &rPlay))
    {
        return 0xFFFFFFFF;
    }

    if(pu4Size != NULL)
    {
        if((rPlay.u4Rp < rPlay.u4BufStart) || (rPlay.u4Rp > rPlay.u4BufEnd))
        {
            *pu4Size = 0;
        }
        else
        {
            *pu4Size = DATASIZE(rPlay.u4Rp, rPlay.u4Wp, rPlay.u4BufSize);
        }
    }

    if(pu4Max != NULL)
    {
        *pu4Max = _u4PVRMLPBufMax;
    }

    if(pu4Min != NULL)
    {
        *pu4Min = _u4PVRMLPBufMin;
    }
}


//-----------------------------------------------------------------------------
/** _PVRMPktNotify
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _PVRMPktNotify(FVR_GBL_NOTIFY_CODE_T eCode, 
                                   UINT32 u4Data, const void* pvTag)
{
    if(eCode == FVR_GBL_NFY_OVERFLOW)
    {    
        LOG(3, "Record buffer full\n");
        VERIFY(OSR_OK == x_sema_unlock(_hPVRMTestSema));    
    }

    UNUSED(u4Data);
    UNUSED(pvTag);
}


//-----------------------------------------------------------------------------
/** PVRM_Loopback_Record
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL PVRM_Loopback_Record(UINT32 u4BufSize)
{
    FVR_TIMESTAMP_T rTimeStamp;
    FVR_GBL_T rGBL;

    if(_hPVRMTestSema == NULL_HANDLE)
    {
        VERIFY(OSR_OK == x_sema_create(&_hPVRMTestSema,
                         X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));
    }
    
    if(FVR_IsRecording())
    {
        LOG(3, "Please stop record first\n");
        return FALSE;
    }

    if(!FVR_FreeBuffer())
    {
        return FALSE;
    }

    // Enabe Record timestamp
    rTimeStamp.fgEnable = TRUE;
    rTimeStamp.u2ClkDiv = 8;
    rTimeStamp.u4Mask = 0xFFFFFFFF;
    rTimeStamp.u4Counter= 0x0;
    FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimeStamp);

    x_memset((void*)&rGBL, 0, sizeof(FVR_GBL_T));
    rGBL.fgAllocateBuffer = TRUE;
    rGBL.u2TargetPktCount = 0x0;
    rGBL.u4BufSize = u4BufSize;
    rGBL.pfnNotifyFunc = _PVRMPktNotify;
    rGBL.pvNotifyTag = NULL;
    rGBL.u4TimeToNotify = 0;
    if(!FVR_SetGBL((UINT32)(FVR_GBL_FLAG_TARGET_PKT | FVR_GBL_FLAG_BUFFER | FVR_GBL_FLAG_TIME_INFO |
                   FVR_GBL_FLAG_NOTIFY), &rGBL))
    {
        return FALSE;
    }

    if(!FVR_Start())
    {
        LOG(3, "Start fail\n");
        return FALSE;
    }
    
    VERIFY(x_sema_lock(_hPVRMTestSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    if(!FVR_Stop())
    {
        LOG(3, "Stop fail\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** PVRM_Loopback_SinglePlay
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL PVRM_Loopback_SinglePlay(UINT32 u4BufAddr, UINT32 u4BufSize)
{
    DMX_PVR_PLAY_T rPlay;
    UINT32 u4Size, u4Align, u4Addr, u4Count, u4Total;
    DMX_MM_DATA_T rData;
    
    if(DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING)
    {
        LOG(3, "Please stop first\n");
        return FALSE;
    }

    if(!DMX_PVRPlay_FreeBuf())
    {
        return FALSE;
    }

    // Enable PLAY timestamp
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = FALSE;
    rPlay.u2TimeStampFreqDiv = 8;    
    if(!DMX_PVRPlay_Set((UINT32)PVRPLAY_FLAGS_TIMESTAMP, &rPlay))
    {
        return FALSE;
    }

    u4Size = u4BufSize;
    u4Align = u4Size % 192;
    u4Size -= u4Align;
    u4Total = u4Size; // * 3;
    u4Addr = u4BufAddr;

    // Play init
    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = u4BufAddr;
    rPlay.u4BufSize = u4Size;
    rPlay.u4ThresholdSize = (0x200 * 192);
    rPlay.pfnPVRNotify = NULL;
    rPlay.eMode = DMX_PVR_PLAY_SINGLEMOVE;

    if(!DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_THRESHOLD | PVRPLAY_FLAGS_BUFFER | 
                        PVRPLAY_FLAGS_CALLBACK | PVRPLAY_FLAGS_MODE), &rPlay))
    {
        return FALSE;
    }
    
    while(u4Total > 0)
    {
        if(u4Total > (0x200 * 192))
        {
            u4Count = (0x200 * 192);
        }
        else
        {
            //u4Count = u4Size;
            break;
        }
        
        x_memset((void*)&rData, 0, sizeof(DMX_MM_DATA_T));
        rData.u4BufStart = u4BufAddr;
        rData.u4BufEnd = u4BufAddr + u4Size;
        rData.u4StartAddr = u4Addr;
        rData.u4FrameSize = u4Count;
        if(!DMX_PVRPlay_SingleMove(&rData))
        {
            LOG(3, "Single move fail\n");
            return FALSE;
        }

        u4Total -= u4Count;
        u4Addr += u4Count;
        if(u4Addr >= (u4BufAddr + u4Size))
        {
            u4Addr -= u4Size;
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** PVRM_Loopback_StreamPlay
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL PVRM_Loopback_StreamPlay(UINT32 u4BufAddr, UINT32 u4BufSize)
{
    DMX_PVR_PLAY_T rPlay;
    UINT32 u4Size, u4Align;
    
    if(DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING)
    {
        LOG(3, "Please stop first\n");
        return FALSE;
    }

    if(!DMX_PVRPlay_FreeBuf())
    {
        return FALSE;
    }

    // Enable PLAY timestamp
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = FALSE;
    rPlay.u2TimeStampFreqDiv = 8;    
    if(!DMX_PVRPlay_Set((UINT32)PVRPLAY_FLAGS_TIMESTAMP, &rPlay))
    {
        return FALSE;
    }

    u4Size = u4BufSize;
    u4Align = u4Size % 192;
    u4Size -= u4Align;

    // Play init
    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = u4BufAddr;
    rPlay.u4BufSize = u4Size + 192;
    rPlay.u4ThresholdSize = 0x200 * 192;
    rPlay.pfnPVRNotify = NULL;
    rPlay.eMode = DMX_PVR_PLAY_STREAM;

    if(!DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_THRESHOLD |PVRPLAY_FLAGS_BUFFER | 
                        PVRPLAY_FLAGS_CALLBACK | PVRPLAY_FLAGS_MODE), &rPlay))
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_UpdateWp(u4BufAddr, u4Size))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** PVRM_Loopback_ErrorTest
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL PVRM_Loopback_ErrorTest(void)
{
    FVR_GBL_T rGBL;
    UINT32 u4Addr, u4Size, u4BufSize;
    UINT32 u4ErrTime;

    u4BufSize = 0x80000;
    
    if(!PVRM_Loopback_Record(u4BufSize))
    {
        Printf("Fail\n");
        return FALSE;
    }
    
    if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return FALSE;
    }

    u4Addr = rGBL.u4Rp;
    u4Size = rGBL.u4Wp - rGBL.u4Rp;

    u4ErrTime = *(UINT32*)(u4Addr + (192 * 100));
    *(UINT32*)(u4Addr + (192 * 102)) = u4ErrTime;
    
    if(!PVRM_Loopback_SinglePlay(u4Addr, u4Size))
    {
    }

    // If single play fail, it will not reach here

    return TRUE;
}


//-----------------------------------------------------------------------------
/** PVRM_Loopback_RecTimerTest
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL PVRM_Loopback_RecTimerTest(UINT32 u4BufSize)
{
    FVR_TIMESTAMP_T rTimeStamp;
    FVR_GBL_T rGBL;

    _u4PVRMPktCount = 0;

    if(_hPVRMTestSema == NULL_HANDLE)
    {
        VERIFY(OSR_OK == x_sema_create(&_hPVRMTestSema,
                         X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));
    }
    
    if(FVR_IsRecording())
    {
        LOG(3, "Please stop record first\n");
        return FALSE;
    }

    if(!FVR_FreeBuffer())
    {
        return FALSE;
    }

    // Enabe Record timestamp
    rTimeStamp.fgEnable = TRUE;
    rTimeStamp.u2ClkDiv = 8;
    rTimeStamp.u4Mask = 0xFFFFFFFF;
    rTimeStamp.u4Counter= 0x0;
    FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimeStamp);

    x_memset((void*)&rGBL, 0, sizeof(FVR_GBL_T));
    rGBL.fgAllocateBuffer = TRUE;
    rGBL.u2TargetPktCount = 0x0;
    rGBL.u4BufSize = u4BufSize;
    rGBL.pfnNotifyFunc = _PVRMLPTimeCallback;
    rGBL.pvNotifyTag = NULL;
    rGBL.u4TimeToNotify = 500;
    if(!FVR_SetGBL((UINT32)(FVR_GBL_FLAG_TARGET_PKT | FVR_GBL_FLAG_BUFFER | FVR_GBL_FLAG_TIME_INFO |
                   FVR_GBL_FLAG_NOTIFY), &rGBL))
    {
        return FALSE;
    }

    if(!FVR_Start())
    {
        LOG(3, "Start fail\n");
        return FALSE;
    }

    HAL_GetTime(&_rPVRMTestTime);
    
    VERIFY(x_sema_lock(_hPVRMTestSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    if(!FVR_Stop())
    {
        LOG(3, "Stop fail\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** PVRM_Loopback_StreamPlay
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL PVRM_Loopback_PlayResetTest(UINT32 u4Loop, UINT32 u4BufAddr, UINT32 u4BufSize)
{
    UINT32 i;

    for(i=0; i<u4Loop; i++)
    {
        Printf(" %d loop test.........................\n", i);
        if(!PVRM_Loopback_StreamPlay(u4BufAddr, u4BufSize))
        {
            return FALSE;
        }

        if(!DMX_PVRPlay_Start())
        {
            Printf("Start play fail!\n");
            return FALSE;
        }

        x_thread_delay(4000);

        if(!DMX_PVRPlay_Stop(TRUE))
        {
            Printf("Stop play fail!\n");
            return FALSE;
        }

        if(!DMX_PVRPlay_RequestReset())
        {
            LOG(3, "Request reset fail\n");
            return FALSE;
        }        
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** PVRM_Loopback_IgnoreTimeStampTest
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL PVRM_Loopback_IgnoreTimeStampTest(UINT32 u4BufAddr, UINT32 u4BufSize)
{
    DMX_PVR_PLAY_T rPlay;
    UINT32 u4Size, u4Align, u4Addr, u4Count, u4Total;
    DMX_MM_DATA_T rData;
    
    if(DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING)
    {
        LOG(3, "Please stop first\n");
        return FALSE;
    }

    if(!DMX_PVRPlay_FreeBuf())
    {
        return FALSE;
    }

    // Enable PLAY timestamp
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = TRUE;
    rPlay.u2TimeStampFreqDiv = 8;    
    if(!DMX_PVRPlay_Set((UINT32)PVRPLAY_FLAGS_TIMESTAMP, &rPlay))
    {
        return FALSE;
    }

    u4Size = u4BufSize;
    u4Align = u4Size % 192;
    u4Size -= u4Align;
    u4Total = u4Size; // * 3;
    u4Addr = u4BufAddr;

    // Play init
    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = u4BufAddr;
    rPlay.u4BufSize = u4Size;
    rPlay.u4ThresholdSize = (0x200 * 192);
    rPlay.pfnPVRNotify = NULL;
    rPlay.eMode = DMX_PVR_PLAY_SINGLEMOVE;

    if(!DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_THRESHOLD | PVRPLAY_FLAGS_BUFFER | 
                        PVRPLAY_FLAGS_CALLBACK | PVRPLAY_FLAGS_MODE), &rPlay))
    {
        return FALSE;
    }
    
    while(u4Total > 0)
    {
        while(1)
        {
            UINT16 u2QueueSize, u2MaxQueueSize;
            VDEC_GetQueueInfo(ES0, &u2QueueSize, &u2MaxQueueSize);
            if(u2QueueSize >= 10)
            {
                x_thread_delay(10);
            }
            else
            {
                break;
            }
        }
        
        if(u4Total > (0x200 * 192))
        {
            u4Count = (0x200 * 192);
        }
        else
        {
            //u4Count = u4Size;
            break;
        }
        
        x_memset((void*)&rData, 0, sizeof(DMX_MM_DATA_T));
        rData.u4BufStart = u4BufAddr;
        rData.u4BufEnd = u4BufAddr + u4Size;
        rData.u4StartAddr = u4Addr;
        rData.u4FrameSize = u4Count;
        if(!DMX_PVRPlay_SingleMove(&rData))
        {
            LOG(3, "Single move fail\n");
            return FALSE;
        }

        u4Total -= u4Count;
        u4Addr += u4Count;
        if(u4Addr >= (u4BufAddr + u4Size))
        {
            u4Addr -= u4Size;
        }
    }

    return TRUE;
}


