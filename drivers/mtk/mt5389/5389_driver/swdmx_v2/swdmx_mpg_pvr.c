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
 * $RCSfile: swdmx_mpg_pvr.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_mpg_pvr.c
 *  Software DMX for PVR
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_assert.h"
LINT_EXT_HEADER_END

#include "x_timer.h"

#include "dmx_if.h"
#include "dmx_mm_if.h"
#include "fvr_if.h"
#include "vdec_drvif.h"

#include "swdmx_mpg_pvr.h"
#include "swdmx_drvif.h"
#include "swdmx_mpg.h"
#include "swdmx_mpg_dmx.h"
#include "swdmx_debug.h"
#include "x_util.h"


//-----------------------------------------------------------------------------
// Extern functions
//-----------------------------------------------------------------------------
//extern VOID _SWDMX_MpgCreateFindSema(VOID);


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SWDMX_PVR_FEEDER_TRY_COUNT            10

#define SWDMX_PVR_MIN_BUFDATA                 (1024 * 240)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
// move the struct to swdmx_drvif.h
#if 0
typedef enum
{
    SWDMX_PVR_STATE_IDLE,
    SWDMX_PVR_STATE_PLAYING,
    SWDMX_PVR_STATE_PAUSE,
    SWDMX_PVR_STATE_NODATA,
    SWDMX_PVR_STATE_ERROR
} SWDMX_PVR_STATE_T;
#endif

typedef enum
{
    SWDMX_PVR_CMD_STOP,
    SWDMX_PVR_CMD_START,
    SWDMX_PVR_CMD_PAUSE,
    SWDMX_PVR_CMD_FILLDATA,
    SWDMX_PVR_CMD_FLUSH,
    SWDMX_PVR_CMD_RESET
} SWDMX_PVR_CMD_T;

typedef struct
{
    SWDMX_PVR_CMD_T eCmd;
} SWDMX_PVR_MSG_T;

typedef struct
{
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    UINT32 u4Offset;
    UINT32 u4Size;
    UINT32 u4NewOffset;
} SWDMX_PVR_BUF_T;

// move the struct to swdmx_drvif.h
#if 0
typedef struct
{
    BOOL fgEnable;
    UINT16 u2PidNum;
    DMX_PID_TYPE_T eType;
} SWDMX_PVR_PID_T;
#endif
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables needed move to SWMDX_MPG_INFO_T
//-----------------------------------------------------------------------------
#if 0
static UINT32 _u4Threshold = 0;

static SWDMX_PVR_STATE_T _eSWDMXPVRState;

/// State word for locking interrupt
//static CRIT_STATE_T _rSfvrLock;

/// Is interrupt locking? For making sure Lock()/Unlock() are not nested
//static BOOL _fgSfvrLocking = FALSE;

static UINT32 _u4PVRPlayTime = 0;
static SWDMX_PVR_PID_T _arPVRPlayPid[DMX_NUM_PID_INDEX];

static INT32 _i4PVRSpeed = 1000;

// When normal to trick, SWDMX will call resume before flush buffer
// So just keep resuem flag, and do flush in send data function
// Call in the same thread, need not critical section
static BOOL _fgPVRMpgResume = FALSE;

// Pause buffer status
static DMX_PVRPLAY_PTR_T _rPVRPauseBufPtr;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#if 0
//-----------------------------------------------------------------------------
/** _SFVR_Lock
 *  Enter demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _SFVR_Lock(void)
{
    _rSfvrLock = x_crit_start();

    ASSERT(!_fgSfvrLocking);
    _fgSfvrLocking = TRUE;

    UNUSED(_fgSfvrLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** _SFVR_Unlock
 *  Leave demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _SFVR_Unlock(void)
{
    ASSERT(_fgSfvrLocking);
    _fgSfvrLocking = FALSE;

    x_crit_end(_rSfvrLock);

    UNUSED(_fgSfvrLocking);          // Make Lint happy
}
#endif


//-----------------------------------------------------------------------------
/** _SPVR_AdvanceAddr
 */
//-----------------------------------------------------------------------------
static UINT32 _SPVR_AdvanceAddr(UINT32 u4Addr, INT32 i4Increment,
                                    UINT32 u4FifoStart, UINT32 u4FifoEnd)
{
	UINT32 u4FifoSize;

    // Debug
    if ((u4Addr < u4FifoStart) || (u4Addr >= u4FifoEnd))
    {
        return 0;
    }

    //ASSERT((u4Addr >= u4FifoStart) && (u4Addr < u4FifoEnd));

	u4FifoSize = u4FifoEnd - u4FifoStart;

    u4Addr = (UINT32)((INT32)u4Addr + i4Increment);
    if (u4Addr >= u4FifoEnd)
    {
        u4Addr -= u4FifoSize;
    }
    if (u4Addr < u4FifoStart)
    {
    	u4Addr += u4FifoSize;
    }

    return u4Addr;
}


//-----------------------------------------------------------------------------
/** _SPVR_CopyBuffer
 */
//-----------------------------------------------------------------------------
static void _SPVR_CopyBuffer(UINT32 u4DestBuf, UINT32 u4Addr, UINT32 u4Size,
                             UINT32 u4SrcStart, UINT32 u4SrcEnd)
{
    UINT32 u4Len;

    u4Len = MIN((u4SrcEnd - u4Addr), u4Size);
    x_memcpy((void*)u4DestBuf, (void*)u4Addr, u4Len);

    if(u4Len < u4Size)
    {
        u4DestBuf += u4Len;
        x_memcpy((void*)u4DestBuf, (void*)u4SrcStart, u4Size - u4Len);
    }
}

//-----------------------------------------------------------------------------
/** _SwdmxPVR_ErrorCheck
 */
//-----------------------------------------------------------------------------
static void _SwdmxPVR_ErrorCheck(const DMX_PVRPLAY_BUFPTR_T *prBufPtr, const DMX_MM_DATA_T *prDmxMMData)
{
    UINT32 u4Addr, u4TempAddr, u4TempAddr2, u4Count, u4PktAddr;
    UINT32 u4BufStart, u4BufEnd;
    INT32 i, j;
    UINT8 au1Buf[12];

    LOG(0, "%s\n", __FUNCTION__);

    if((prBufPtr == NULL) || (prDmxMMData == NULL))
    {
        LOG(5, "%s,%d error\n", __FUNCTION__, __LINE__);
        return;
    }

    for(i=0;i<12;i++)
    {
        au1Buf[i] = 0;
    }

    u4Addr = prBufPtr->u4HwRp;
    if(u4Addr == 0)
    {
        LOG(5, "Should be panic case\n");
        return;
    }
    u4BufStart = prDmxMMData->u4BufStart;
    u4BufEnd = prDmxMMData->u4BufEnd;
    u4PktAddr = 0;
    for(i=0; i<256; i++)
    {
        u4TempAddr = _SPVR_AdvanceAddr(u4Addr, i * (-1), u4BufStart, u4BufEnd);
        //ASSERT(u4TempAddr != 0);
        if(u4TempAddr == 0)
        {
            LOG(0, "%s,%d, addr error\n", __FUNCTION__, __LINE__);
            return;
        }

        if((*(UINT8*)u4TempAddr) == 0x47)
        {
            u4Count = 0;
            for(j=1; j<3; j++)
            {
                u4TempAddr2 = _SPVR_AdvanceAddr(u4TempAddr, (-192) * j, u4BufStart, u4BufEnd);
                //ASSERT(u4TempAddr2 != 0);
                if(u4TempAddr2 == 0)
                {
                    LOG(0, "%s,%d, addr error\n", __FUNCTION__, __LINE__);
                    return;
                }
                if((*(UINT8*)u4TempAddr2) == 0x47)
                {
                    u4Count++;
                }
                else
                {
                    break;
                }
            }

            if(u4Count == 2)
            {
                u4TempAddr -= 4;            // minus timestamp
                if(u4TempAddr < u4BufStart)
                {
                    u4TempAddr += (u4BufEnd - u4BufStart);
                }
                u4PktAddr = u4TempAddr;
                break;
            }
        }
    }

    if(u4PktAddr == 0)
    {
        LOG(0, "%s,%d can not find sync byte\n", __FUNCTION__, __LINE__);
        return;
    }

    u4TempAddr = _SPVR_AdvanceAddr(u4PktAddr, (-192) * 3, u4BufStart, u4BufEnd);
    for(i=0; i<6; i++)
    {
        u4TempAddr2 = _SPVR_AdvanceAddr(u4TempAddr, 192 * i, u4BufStart, u4BufEnd);
        _SPVR_CopyBuffer((UINT32)au1Buf, u4TempAddr2, 8, u4BufStart, u4BufEnd);
        LOG(0, "%d: %02x %02x %02x %02x %02x %02x %02x %02x\n", i,
            au1Buf[0], au1Buf[1], au1Buf[2], au1Buf[3], au1Buf[4], au1Buf[5],
            au1Buf[6], au1Buf[7]);
    }

}


//-----------------------------------------------------------------------------
/** _SwdmxPVR_WaitFinish
 */
//-----------------------------------------------------------------------------
static BOOL _SwdmxPVR_WaitFinish(void)
{
    DMX_PVRPLAY_BUFPTR_T rPtr1;
    UINT32 i;

    x_memset((void*)&rPtr1, 0, sizeof(DMX_PVRPLAY_BUFPTR_T));
    for(i=0; i<100; i++)
    {
        if(!DMX_PVRPlay_GetBufPointer(&rPtr1))
        {
            return FALSE;
        }

        // If read pointer1 == read pointer 2, error handling
        if(rPtr1.u4HwWp == rPtr1.u4HwRp)
        {
            break;
        }

        x_thread_delay(1);
    }

    if(i == 100)
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SwdmxPVR_Flush
 */
//-----------------------------------------------------------------------------
static BOOL _SwdmxPVR_Flush(UINT8 u1SrcId, const DMX_PVRPLAY_PTR_T *prPtr)
{
    DMX_PVRPLAY_PTR_T rPVRPtr;
    DMX_PVR_PLAY_T rPlay;

    if(!DMX_PVRPlay_Pause(&rPVRPtr))
    {
        LOG(3, "%s, pause fail!\n", __FUNCTION__);
        return FALSE;
    }

    if(prPtr != NULL)                                   // Flush from pause state
    {
        rPVRPtr = *prPtr;
    }

    // Set ignore timestamp
    rPlay.eMode = DMX_PVR_PLAY_SINGLEMOVE;
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = TRUE;
    rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
    if(!DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP), &rPlay))
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_Resume(&rPVRPtr))
    {
        LOG(3, "%s resume fail!\n", __FUNCTION__);
        return FALSE;
    }

    if(!_SwdmxPVR_WaitFinish())
    {
        LOG(3, "Wait finish fail\n");
        return FALSE;
    }

    // Stop
    if(!DMX_PVRPlay_Stop(TRUE))
    {
        LOG(3, "Stop play fail!\n");
        return FALSE;
    }

	UNUSED(u1SrcId);
    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _SWDMX_PVR_Init
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_Init(UINT8 u1SrcId)
{
    UINT32 i;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    //-----------------------------------------------------------
    // Meta file
    //-----------------------------------------------------------
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.u4PVRPlayTime = 0;
    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[i].fgEnable = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[i].u2PidNum = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[i].eType = DMX_PID_TYPE_NONE;
    }
    //-----------------------------------------------------------

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState = SWDMX_PVR_STATE_IDLE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.i4PVRSpeed = 1000;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.u4Threshold = 0x800000;            // Not use now

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_GetInfo
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_GetInfo(UINT8 u1SrcId,SWDMX_PROGRAM_INFO_T* prOpenInfo)
{
    UINT32 i;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    if(prOpenInfo == NULL)
    {
        return FALSE;
    }

    x_memset(prOpenInfo, 0, sizeof(prOpenInfo));
    prOpenInfo->u4PgmNum = 1;
    //prOpenInfo->u4InfoMask;

    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        // Apply first video and audio
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[i].fgEnable)
        {
            if((prOpenInfo->rPgm[0].u2VideoNum == 0)
               && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[i].eType == DMX_PID_TYPE_ES_VIDEO))
            {
                prOpenInfo->rPgm[0].u2VideoNum = 1;
                prOpenInfo->rPgm[0].au2VideoId[0] = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[i].u2PidNum;
                prOpenInfo->rPgm[0].aeVideo[0] = ENUM_SWDMX_VDEC_MPEG1_MPEG2;   // temp
            }

            if((prOpenInfo->rPgm[0].u2AudioNum == 0) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[i].eType == DMX_PID_TYPE_ES_AUDIO))
            {
                prOpenInfo->rPgm[0].u2AudioNum = 1;
                prOpenInfo->rPgm[0].au2AudioId[0] = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[i].u2PidNum;
                prOpenInfo->rPgm[0].aeAudio[0] = ENUM_SWDMX_ADEC_MPEG;  // temp
            }

            if((prOpenInfo->rPgm[0].u2PcrNum == 0) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[i].eType == DMX_PID_TYPE_NONE))
            {
                prOpenInfo->rPgm[0].u2PcrNum = 1;
                prOpenInfo->rPgm[0].au2PcrId[0] = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[i].u2PidNum;
            }
        }
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.u4PVRPlayTime == 0)     // temp
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.u4PVRPlayTime = 20 * 60;
    }
    prOpenInfo->rPgm[0].u4TotalTime = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.u4PVRPlayTime * 90000;//(20*60*90000);
    prOpenInfo->rPgm[0].u4FirstPts = 0;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_OPEN
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_OPEN(UINT8 u1SrcId)
{
    DMX_PVR_PLAY_T rPlay;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "%s\n", __FUNCTION__);

    if(!DMX_PVRPlay_Stop(TRUE))
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_FreeBuf())
    {
        LOG(5, "%s DMX_PVRPlay_FreeBuf Fail.\n", __FUNCTION__);
        return FALSE;
    }

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) !=
        FEEDER_E_OK)
    {
        LOG(7, "%s Feeder input buffer error.\n", __FUNCTION__);
        return FALSE;
    }

    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = prSwdmxInst->rFeederInfo.u4StartAddr;
    rPlay.u4BufSize = prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr;
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = FALSE;
    rPlay.u2TimeStampFreqDiv = (UINT16)(DMX_PVRPLAY_TIMESTAMP_DIV_BASE & 0xFFFF);
    rPlay.eMode = DMX_PVR_PLAY_STREAM;
    rPlay.pfnPVRNotify = NULL;
    rPlay.u4ThresholdSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.u4Threshold;

    if(!DMX_PVRPlay_Set((UINT32)PVRPLAY_FLAGS_ALL, &rPlay))
    {
        return FALSE;
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState = SWDMX_PVR_STATE_IDLE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.i4PVRSpeed = 1000;

    if(!DMX_OpenTimeshift(TRUE))
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_SetDBMTestPid())
    {
        return FALSE;
    }

    LOG(3, "%s - OK\n", __FUNCTION__);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_OPEN2
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_OPEN2(UINT8 u1SrcId, UINT32 u4BufStart, UINT32 u4BufSize)
{
    DMX_PVR_PLAY_T rPlay;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "%s\n", __FUNCTION__);

    if(!DMX_PVRPlay_Stop(TRUE))
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_FreeBuf())
    {
        LOG(5, "%s DMX_PVRPlay_FreeBuf Fail.\n", __FUNCTION__);
        return FALSE;
    }

    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = u4BufStart;
    rPlay.u4BufSize = u4BufSize;
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = FALSE;
    rPlay.u2TimeStampFreqDiv = (UINT16)(DMX_PVRPLAY_TIMESTAMP_DIV_BASE & 0xFFFF);
    rPlay.eMode = DMX_PVR_PLAY_STREAM;
    rPlay.pfnPVRNotify = NULL;
    rPlay.u4ThresholdSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.u4Threshold;

    if(!DMX_PVRPlay_Set((UINT32)PVRPLAY_FLAGS_ALL, &rPlay))
    {
        return FALSE;
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState = SWDMX_PVR_STATE_IDLE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.i4PVRSpeed = 1000;

    if(!DMX_OpenTimeshift(TRUE))
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_SetDBMTestPid())
    {
        return FALSE;
    }

    LOG(3, "%s - OK\n", __FUNCTION__);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_Close
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_Close(UINT8 u1SrcId)
{
    if(!DMX_PVRPlay_FreeDBMTestPid())
    {
        return FALSE;
    }

    if(!DMX_OpenTimeshift(FALSE))
    {
        return FALSE;
    }

    UNUSED(u1SrcId);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_SendData
 */
//-----------------------------------------------------------------------------
SWDMX_PVR_SENDDATA_RET_T _SWDMX_PVR_SendData(UINT8 u1SrcId,
                const DMX_MM_DATA_T *prDmxMMData,
                UINT32 u4MinBuf, BOOL fgFastMode)
{
    //static DMX_PVRPLAY_BUFPTR_T _rCurPtr = { 0, 0, 0, 0 };      // lint
    //static HAL_TIME_T _rSwDmxPVRTime;
    //static BOOL _fgCheckHang = FALSE;
    DMX_PVRPLAY_BUFPTR_T rPtr;
    DMX_PVR_PLAY_T rPlay;
    UINT32 u4DataSize;
    UINT32 u4Wp;
    HAL_TIME_T rTime, rDiffTime;
    DMX_PVRPLAY_PTR_T rResumePtr;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if (prSwdmxInst == NULL)
    {
        LOG(0, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__);
        return SWDMX_PVR_SENDDATA_FAIL;
    }
    UNUSED(prSwdmxInst);
/*  // debug
    static UINT32 _u4Size = 0;
    static BOOL _fgRun = TRUE;

    if(_fgRun)
    {
        _u4Size += prDmxMMData->u4FrameSize;
        if(_u4Size >= 0x800000)
        {
            UINT8 *pu1Tmp;

            Printf("test....\n");
            pu1Tmp = (UINT8*)(prDmxMMData->u4StartAddr);
            pu1Tmp[0] = 0xFF;
            pu1Tmp[1] = 0xFF;
            pu1Tmp[2] = 0xFF;
            pu1Tmp[3] = 0xFF;
            _fgRun = FALSE;
        }
    }
*/
    if(prDmxMMData == NULL)
    {
        return SWDMX_PVR_SENDDATA_FAIL;
    }

    ASSERT(((prDmxMMData->u4BufEnd - prDmxMMData->u4BufStart) % 192) == 0);

    x_memset((void*)&rPlay, 0, sizeof(DMX_PVR_PLAY_T));
    if(!DMX_PVRPlay_Get((UINT32)PVRPLAY_FLAGS_MODE, &rPlay))
    {
        return SWDMX_PVR_SENDDATA_FAIL;
    }

    //------------------------------------------------------------
    // Fast mode
    //------------------------------------------------------------
    if(fgFastMode)
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState != SWDMX_PVR_STATE_IDLE))
        {
            if(!_SWDMX_PVR_Stop(u1SrcId))
            {
                return SWDMX_PVR_SENDDATA_FAIL;
            }
        }

        if(rPlay.eMode != DMX_PVR_PLAY_SINGLEMOVE)
        {
            rPlay.eMode = DMX_PVR_PLAY_SINGLEMOVE;
            rPlay.fgContainTimeStamp = TRUE;
            rPlay.fgIgnoreTimeStamp = TRUE;
            rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
            if(!DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP), &rPlay))
            {
                return SWDMX_PVR_SENDDATA_FAIL;
            }
        }

        if(!DMX_PVRPlay_SingleMove(prDmxMMData))
        {
            return SWDMX_PVR_SENDDATA_FAIL;
        }

        return SWDMX_PVR_SENDDATA_OK;
    }

    //------------------------------------------------------------
    // Stream mode
    //------------------------------------------------------------
    if(rPlay.eMode != DMX_PVR_PLAY_STREAM)
    {
        rPlay.eMode = DMX_PVR_PLAY_STREAM;
        rPlay.fgContainTimeStamp = TRUE;
        rPlay.fgIgnoreTimeStamp = FALSE;
        rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
        rPlay.u4ThresholdSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.u4Threshold;
        if(!DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_MODE| PVRPLAY_FLAGS_TIMESTAMP | PVRPLAY_FLAGS_THRESHOLD),
                            &rPlay))
        {
            return SWDMX_PVR_SENDDATA_FAIL;
        }
    }

    //----------------------------------------------------------------------
    // Handle resume
    //----------------------------------------------------------------------
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgPVRMpgResume &&
    	  (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState == SWDMX_PVR_STATE_PAUSE))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgCheckHang = FALSE;

        u4Wp = prDmxMMData->u4StartAddr + prDmxMMData->u4FrameSize;
        if(u4Wp >= prDmxMMData->u4BufEnd)
        {
            u4Wp -= (prDmxMMData->u4BufEnd - prDmxMMData->u4BufStart);
        }
        rResumePtr.u4Wp = u4Wp;
        rResumePtr.u4Rp =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rPVRPauseBufPtr.u4Rp;
        if(!DMX_PVRPlay_Resume(&rResumePtr))
        {
            LOG(3, "%s:%d resume fail!\n", __FUNCTION__, __LINE__);
            return SWDMX_PVR_SENDDATA_FAIL;
        }

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState = SWDMX_PVR_STATE_PLAYING;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgPVRMpgResume = FALSE;

        return SWDMX_PVR_SENDDATA_OK;
    }

    //----------------------------------------------------------------------
    // Handle first time play
    //----------------------------------------------------------------------
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState == SWDMX_PVR_STATE_IDLE)
    {
        //ASSERT(((UINT32)u8StartAddr % 192) == 0);
        LOG(6, "First, addr: 0x%x, size: %d\n", prDmxMMData->u4StartAddr, prDmxMMData->u4FrameSize);

        u4Wp = prDmxMMData->u4StartAddr + prDmxMMData->u4FrameSize;
        if(u4Wp >= prDmxMMData->u4BufEnd)
        {
            u4Wp -= (prDmxMMData->u4BufEnd - prDmxMMData->u4BufStart);
        }

        if(!DMX_PVRPlay_SetBuffer(prDmxMMData->u4BufStart, prDmxMMData->u4BufEnd - 1,
                                  u4Wp, prDmxMMData->u4StartAddr))
        {
            return SWDMX_PVR_SENDDATA_FAIL;
        }

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgCheckHang = FALSE;
        if(!DMX_PVRPlay_Start())
        {
            return SWDMX_PVR_SENDDATA_FAIL;
        }

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState = SWDMX_PVR_STATE_PLAYING;

        return SWDMX_PVR_SENDDATA_OK;
    }

    //--------------------------------------------------------
    // Send Data
    //--------------------------------------------------------
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState != SWDMX_PVR_STATE_PLAYING)
    {
        LOG(3, "%s, %d Not in playing state\n", __FUNCTION__, __LINE__);
        return SWDMX_PVR_SENDDATA_OK;
    }

    //--------------------------------------------------------
    // Run here means streamming mode and in playing state
    //--------------------------------------------------------
    if(DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_STOP)
    {
        // PVR play should not stop here, something error ???
        if(!_SWDMX_PVR_Stop(u1SrcId))
        {
            return SWDMX_PVR_SENDDATA_FAIL;
        }
    }

    if(!DMX_PVRPlay_Get(PVRPLAY_FLAGS_BUFFER, &rPlay))
    {
        return SWDMX_PVR_SENDDATA_FAIL;
    }

    u4DataSize = DATASIZE(rPlay.u4Rp, rPlay.u4Wp, rPlay.u4BufSize);
/*
    if(u4DataSize == 0)
    {
        _eSWDMXPVRState = SWDMX_PVR_STATE_IDLE;
        LOG(1, "%s empty buffer\n", __FUNCTION__);
        if(!DMX_PVRPlay_Stop(TRUE))
        {
            return SWDMX_PVR_SENDDATA_FAIL;
        }
    }
*/
    if((u4DataSize < SWDMX_PVR_MIN_BUFDATA) &&
       (u4DataSize < u4MinBuf))
    {
        LOG(6, "Playing, addr: 0x%x, size: %d\n", prDmxMMData->u4StartAddr, prDmxMMData->u4FrameSize);
        if(!DMX_PVRPlay_UpdateWp(prDmxMMData->u4StartAddr, prDmxMMData->u4FrameSize))
        {
            return SWDMX_PVR_SENDDATA_FAIL;
        }

        return SWDMX_PVR_SENDDATA_OK;
    }

    //--------------------------------------------------------
    // Error handling
    //--------------------------------------------------------
    x_memset((void*)&rPtr, 0, sizeof(DMX_PVRPLAY_BUFPTR_T));

    // Read pointer1
    if(!DMX_PVRPlay_GetBufPointer(&rPtr))
    {
        return SWDMX_PVR_SENDDATA_FAIL;
    }

    if((rPtr.u4HwRp == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rCurPtr.u4HwRp) &&
       (rPtr.u4HwWp == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rCurPtr.u4HwWp))
    {
        //LOG(0, "xxx %d\n", (INT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgCheckHang);
        if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgCheckHang)
        {
            HAL_GetTime(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rSwDmxPVRTime);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgCheckHang = TRUE;
        }

        HAL_GetTime(&rTime);
        HAL_GetDeltaTime(&rDiffTime, &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rSwDmxPVRTime, &rTime);
        if((rDiffTime.u4Seconds> 0) || (rDiffTime.u4Micros > 500000))    // 0.5 sec
        //if(rDiffTime.u4Seconds > 0) // 1 sec
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgCheckHang = FALSE;

            LOG(0, "%s, timeout. %d:%06d, %d:%06d, %d:%06d\n", __FUNCTION__,
                        rDiffTime.u4Seconds, rDiffTime.u4Micros,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rSwDmxPVRTime.u4Seconds, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rSwDmxPVRTime.u4Micros,
                        rTime.u4Seconds, rTime.u4Micros);

            LOG(3, "Long time packet\n");

            // Do more check, if really wrong, stop PVR
            // ...
            _SwdmxPVR_ErrorCheck(&rPtr, prDmxMMData);

            if(!_SWDMX_PVR_Stop(u1SrcId))
            {
                return SWDMX_PVR_SENDDATA_FAIL;
            }
        }
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgCheckHang = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rCurPtr.u4HwRp = rPtr.u4HwRp;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rCurPtr.u4HwWp = rPtr.u4HwWp;
        HAL_GetTime(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rSwDmxPVRTime);
    }

    return SWDMX_PVR_SENDDATA_FULL;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_Pause
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_Pause(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    LOG(3, "%s\n", __FUNCTION__);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState == SWDMX_PVR_STATE_PLAYING)
    {
        if(!DMX_PVRPlay_Pause(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rPVRPauseBufPtr))
        {
            LOG(3, "%s pause fail!\n", __FUNCTION__);
            return FALSE;
        }

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState = SWDMX_PVR_STATE_PAUSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_Resume
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_Resume(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgPVRMpgResume = TRUE;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_FlushBuf
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_FlushBuf(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(3, "%s, state:%d\n", __FUNCTION__, (INT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState == SWDMX_PVR_STATE_PLAYING)
    {
        // Get buffer self
        if(!_SwdmxPVR_Flush(u1SrcId, NULL))
        {
            LOG(1, "%s:%d, Flush fail\n", __FUNCTION__, __LINE__);
        }
    }
    else if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState == SWDMX_PVR_STATE_PAUSE)
    {
        // Use pause buffer parameter
        if(!_SwdmxPVR_Flush(u1SrcId, &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rPVRPauseBufPtr))
        {
            LOG(1, "%s:%d, Flush fail\n", __FUNCTION__, __LINE__);
        }
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState = SWDMX_PVR_STATE_IDLE;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_Stop
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_Stop(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(3, "%s\n", __FUNCTION__);

    if(!DMX_PVRPlay_Stop(TRUE))
    {
        LOG(3, "Stop play fail!\n");
        return FALSE;
    }

    x_memset((void*)&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.rPVRPauseBufPtr, 0 , sizeof(DMX_PVRPLAY_PTR_T));
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.eSWDMXPVRState = SWDMX_PVR_STATE_IDLE;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_SetPid
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_SetPid(UINT8 u1SrcId, UINT8 u1Pidx, UINT16 u2PidNum, DMX_PID_TYPE_T ePidType,
                      BOOL fgEnable)
{
     SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    if(fgEnable && prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[u1Pidx].fgEnable)
    {
        LOG(3, "Pidx %d already enable\n", u1Pidx);
        return FALSE;
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[u1Pidx].fgEnable = fgEnable;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[u1Pidx].u2PidNum = u2PidNum;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.arPVRPlayPid[u1Pidx].eType = ePidType;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_SetTime
 */
//-----------------------------------------------------------------------------
void _SWDMX_PVR_SetTime(UINT8 u1SrcId, UINT32 u4TimeSec)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if(prSwdmxInst == NULL)
    {
        return;
    }
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.u4PVRPlayTime = u4TimeSec;
}


//-----------------------------------------------------------------------------
/** _SWDMX_PVR_SetDataMode
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_PVR_SetDataMode(UINT8 u1SrcId, INT32 i4PlaySpeed)
{

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if(prSwdmxInst == NULL)
    {
        return FALSE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.i4PVRSpeed == i4PlaySpeed)
    {
        return TRUE;
    }

    // Speed x1 to others need flush PVR buffer data
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.i4PVRSpeed == 1000) && (i4PlaySpeed != 1000))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgPVRMpgResume = FALSE;

        // Flush data
        if(!_SWDMX_PVR_FlushBuf(u1SrcId))
        {
            LOG(3, "%s, PVR flush fail\n", __FUNCTION__);
            return FALSE;
        }
    }
    else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.i4PVRSpeed != 1000) && (i4PlaySpeed == 1000))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.fgPVRMpgResume = FALSE;
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPvr.i4PVRSpeed = i4PlaySpeed;

    return TRUE;
}

