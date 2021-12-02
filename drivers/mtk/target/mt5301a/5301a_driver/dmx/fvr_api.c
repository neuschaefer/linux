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
 * $RCSfile: fvr_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_fvr_if.c
 *  Demux driver - FVR Driver Interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fvr.h"
#include "fvr_debug.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_ckgen.h"
LINT_EXT_HEADER_END

#include "dmx_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define FVR_SIZE_ALIGNMENT      1536            // 16 and 512 bytes alignment
#define FVR_TIME_DIVIDER        8000            // 16 bit

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT32 u4Pattern;
    UINT32 u4Mask;
} FVR_STARTCODE_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgFVRLocking = FALSE;
static CRIT_STATE_T _rFVRLock;

/// The PID structures
static FVR_GBL_STRUCT_T _rGblStruct;
static FVR_PID_STRUCT_T _arFVRPidStruct[FVR_PID_COUNT];

static BOOL _fgFVRToggleKey = FALSE;

static FVR_TIMESTAMP_T _rTimeStamp;

FVR_VIDEO_TYPE_T _eFVRVideoType = FVR_VIDEO_NONE;

static FVR_STARTCODE_T _arFVRNONEStartCode[] =
{
    { 0x00000000, 0x00000000 },
};

static FVR_STARTCODE_T _arFVRMPEG2StartCode[] =
{
    { 0x01000008, 0xffff0038 },
    { 0x01000010, 0xffff0038 },
    { 0x01000018, 0xffff0038 }
};

static FVR_STARTCODE_T _arFVRH264StartCode[] =
{
    { 0x01018000, 0xff1f8000 },     // IDR
    { 0x01058000, 0xff1f8000 }      // Non-IDR
};


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _SetFVRBuffer
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _SetFVRBuffer(const FVR_GBL_T* prGBL)
{
    UINT32 u4BufStart, u4BufSize, u4Align, u4PhyAddr, u4VirAddr;

    if(prGBL == NULL)
    {
        return FALSE;
    }
    
    // Check if buffer already allocated, to avoid memory leakage
    if ((_rGblStruct.u4BufStart != 0) || (_rGblStruct.u4BufEnd != 0))
    {
        return FALSE;
    }

    // For error recovery case
    if((prGBL->fgSetAlign == 0) && (prGBL->u4BufAlign == 0) && 
       (prGBL->fgAllocateBuffer == 0) && (prGBL->u4BufStart == 0))
    {
        return TRUE;
    }

    if(prGBL->fgSetAlign)
    {
        u4Align = prGBL->u4BufAlign;
    }
    else
    {
        u4Align = FVR_SIZE_ALIGNMENT;
    }

    // PVR buffer must 16-byte alignment
    ASSERT((u4Align % 16) == 0);

    u4BufSize = prGBL->u4BufSize - (prGBL->u4BufSize % u4Align);
    _rGblStruct.u4BufSize = u4BufSize;

    if(_rGblStruct.fgAllocateBuffer)
    {
    	// Setup header buffer start address
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(u4BufSize, FVR_FIFO_ALIGNMENT);
    	if (u4BufStart == 0)
    	{
    		LOG(3, "Can't allocate memory!\n");
    		return FALSE;
    	}    
    }
    else
    {
        ASSERT(prGBL->u4BufStart != 0);
        u4BufStart = prGBL->u4BufStart;    
    }

    u4PhyAddr = PHYSICAL(u4BufStart);
    u4VirAddr = VIRTUAL(u4BufStart);

    _FVR_Lock();
    FVR_GBL_PID_W(0) = u4PhyAddr;                                  // Ring buf start
    FVR_GBL_PID_W(1) = u4PhyAddr + (u4BufSize - 1);                // Ring buf end
    FVR_GBL_PID_W(3) = u4PhyAddr;                                  // write pointer
    FVR_GBL_PID_W(4) = u4PhyAddr;                                  // read pointer

    _rGblStruct.u4BufStart = u4VirAddr;
    _rGblStruct.u4BufEnd = u4VirAddr + (u4BufSize - 1);
    _rGblStruct.u4Rp = u4VirAddr;
    _rGblStruct.u4Wp = u4VirAddr;
    _rGblStruct.u4PktRp = u4VirAddr;
    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SetFVRVideoStartCode
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _SetFVRVideoStartCode(UINT8 u1Count, const FVR_STARTCODE_T *prStartCode)
{
    UINT32 u4Control;
    UINT8 i;

    if(prStartCode == NULL)
    {
        return FALSE;
    }

    u4Control = 0x0;
    
    _FVR_Lock();

    FVR_PATTERN_MATCH_W(0) = 0x0;
    for(i=0; i<FVR_PATTERN_MATCH_PAIR; i++)
    {
        FVR_PATTERN_MATCH_W(1 + (i*2)) = 0x0;
        FVR_PATTERN_MATCH_W(1 + (i*2) + 1) = 0x0;
    }

    for(i=0; i<u1Count; i++)
    {
        FVR_PATTERN_MATCH_W(1 + (i*2)) = prStartCode[i].u4Pattern;
        FVR_PATTERN_MATCH_W(1 + (i*2) + 1) = prStartCode[i].u4Mask;

        u4Control |= (1 << i);
    }
    
    // Write 0x1 to the "pattern changed" byte.
    FVR_PATTERN_MATCH_W(0) = (u4Control << 16) | 0x1;

    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _FVR_Lock
 *  Enter FVR critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_Lock(void)
{
    _rFVRLock = x_crit_start();

    if(_fgFVRLocking)
    {
        LOG(3, "Already lock\n");
        ASSERT(0);
    }
    
    _fgFVRLocking = TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_Unlock
 *  Leave FVR critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_Unlock(void)
{
    if(!_fgFVRLocking)
    {
        LOG(3, "Already Unlock\n");
        ASSERT(0);
    }
    
    _fgFVRLocking = FALSE;

    x_crit_end(_rFVRLock);
}


//-----------------------------------------------------------------------------
/** _FVR_InitAPI
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_InitAPI()
{  
    x_memset((void*)&_rTimeStamp, 0x0, sizeof(_rTimeStamp));

    _fgFVRToggleKey = FALSE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetGBL
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetGBL(UINT32 u4Flags, const FVR_GBL_T* prGBL)
{
    UINT32 u4Ctrl;

    LOG(5, "%s\n", __FUNCTION__);

    if(prGBL == NULL)
    {
        return FALSE;
    }

    if (u4Flags == FVR_GBL_FLAG_NONE)
    {
        return TRUE;
    }  
        
    if(u4Flags & FVR_GBL_FLAG_BUFFER)
    {
        _rGblStruct.fgAllocateBuffer = prGBL->fgAllocateBuffer;
        _rGblStruct.fgSetAlign = prGBL->fgSetAlign;
        _rGblStruct.u4BufAlign = prGBL->u4BufAlign;
        if(!_SetFVRBuffer(prGBL))
        {
            return FALSE;
        }

        _rGblStruct.u2CurPktCount = 0;
        _rGblStruct.u4LongPktCount = 0;
        FVR_GBL_PID_W(5) &= 0xFFFF0000;
        FVR_GBL_PID_W(6) = 0x0;
    }

    if(u4Flags & FVR_GBL_FLAG_TARGET_PKT)
    {
        _rGblStruct.u2TargetPktCount = prGBL->u2TargetPktCount;
        u4Ctrl = FVR_GBL_PID_W(5);
        u4Ctrl &= 0xFFFF;
        u4Ctrl |= (UINT32)((prGBL->u2TargetPktCount)<<16);
        FVR_GBL_PID_W(5) = u4Ctrl;        
    }
    
    if(u4Flags & FVR_GBL_FLAG_NOTIFY)
    {
        _rGblStruct.pfnNotifyFunc = prGBL->pfnNotifyFunc;
        _rGblStruct.pvNotifyTag = prGBL->pvNotifyTag;
    }

    if(u4Flags & FVR_GBL_FLAG_TIME_INFO)
    {
        _rGblStruct.u4TimeToNotify = prGBL->u4TimeToNotify;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetGBL
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetGBL(UINT32 u4Flags, FVR_GBL_T *prGBL)
{
    UINT32 u4Rp, u4Wp;
    
    if(prGBL == NULL)
    {
        return FALSE;
    }
    
    if (u4Flags == FVR_GBL_FLAG_NONE)
    {
        return TRUE;
    }  
        
    if(u4Flags & FVR_GBL_FLAG_BUFFER)
    {
        prGBL->fgAllocateBuffer = _rGblStruct.fgAllocateBuffer;
        prGBL->u4BufSize = _rGblStruct.u4BufSize;
        prGBL->u4BufStart = _rGblStruct.u4BufStart;
        prGBL->u4BufAlign = _rGblStruct.u4BufAlign;

        _FVR_Lock();
        u4Wp = VIRTUAL(FVR_GBL_PID_W(3));
        u4Rp = VIRTUAL(FVR_GBL_PID_W(4));
        _FVR_Unlock();

        prGBL->u4Wp = u4Wp;
        prGBL->u4Rp = u4Rp;
    }

    if(u4Flags & FVR_GBL_FLAG_TARGET_PKT)
    {
        prGBL->u2TargetPktCount = _rGblStruct.u2TargetPktCount;
    }    

    if(u4Flags & FVR_GBL_FLAG_NOTIFY)
    {
        prGBL->pfnNotifyFunc = _rGblStruct.pfnNotifyFunc;
        prGBL->pvNotifyTag = _rGblStruct.pvNotifyTag;
    }

    if(u4Flags & FVR_GBL_FLAG_TIME_INFO)
    {
        prGBL->u4TimeToNotify = _rGblStruct.u4TimeToNotify;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_FreeGBL
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL _FVR_FreeGBL(void)
{
    UINT32 i, u4Reg;
    BOOL fgPidEnable = FALSE;

    if(_FVR_IsRecording())
    {
        LOG(3, "Still recording...\n");
        return FALSE;
    }

    _FVR_Lock();
    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(_arFVRPidStruct[i].fgEnable)
        {
            fgPidEnable = TRUE;
            break;
        }
    }
    _FVR_Unlock();

    if(fgPidEnable)
    {
        LOG(3, "Record pid %d still enabled\n", i);
        return FALSE;
    }
    
    x_thread_delay(5);
    
    _FVR_Lock();
    FVR_GBL_PID_W(5) = 0;
    FVR_GBL_PID_W(6) = 0x0;        
    FVR_GBL_PID_W(9) = 0x0;     // Disable timer   
    u4Reg = FVR_GBL_PID_W(10) & 0xFF000000;
    u4Reg |= 0x1;
    FVR_GBL_PID_W(10) = u4Reg;    // Disable timer 
    _FVR_Unlock();

    if(!_FVR_FreeBuffer())
    {
        return FALSE;
    }
   
    _FVR_Lock();
    x_memset((void*)&_rGblStruct, 0, sizeof(_rGblStruct));
    _FVR_Unlock();
   
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetGBLStruct
 *  @retval 
 */
//-----------------------------------------------------------------------------
FVR_GBL_STRUCT_T* _FVR_GetGBLStruct(void)
{
    return &_rGblStruct;
}

//-----------------------------------------------------------------------------
/** _FVR_TogglePidKey
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_TogglePidKey(void)
{
    UINT32 u4PidEntry;
    UINT8 i;

    _FVR_Lock();    
    for(i=0; i<FVR_PID_COUNT; i++)
    {
        if(_arFVRPidStruct[i].fgEnable && _arFVRPidStruct[i].fgScramble &&
           (_arFVRPidStruct[i].eKeyType == FVR_KEY_BOTH))
        {
            u4PidEntry = FVR_PID_INDEX_TABLE(i);
            u4PidEntry &= ~0x80;
            if(_arFVRPidStruct[i].fgOddKey)
            {
                _arFVRPidStruct[i].fgOddKey = FALSE;
            }
            else
            {
                _arFVRPidStruct[i].fgOddKey = TRUE;
                u4PidEntry |= 0x80;
            }
            FVR_PID_INDEX_TABLE(i) = u4PidEntry;
        }
    }
    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetIsToggleKey
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_SetIsToggleKey(void)
{
    UINT8 i;

    _FVR_Lock();
    for(i=0; i<FVR_PID_COUNT; i++)
    {
        if(_arFVRPidStruct[i].fgEnable && _arFVRPidStruct[i].fgScramble &&
           (_arFVRPidStruct[i].eKeyType == FVR_KEY_BOTH))
        {
            _fgFVRToggleKey = TRUE;
            break;
        }
    }
    _FVR_Unlock();
}


//-----------------------------------------------------------------------------
/** _FVR_GetIsToggleKey
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetIsToggleKey(void)
{
    BOOL fgToggle;

    _FVR_Lock();
    fgToggle = _fgFVRToggleKey;
    _FVR_Unlock();
    
    return fgToggle;
}


//-----------------------------------------------------------------------------
/** _FVR_SetPid
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const FVR_PID_T* prPid)
{
    UINT32 u4PidEntry;
    
    ASSERT(u1Pidx < FVR_NUM_PID_INDEX);

    if(prPid == NULL)
    {
        return FALSE;
    }

    if (u4Flags == FVR_PID_FLAG_NONE)
    {
        return TRUE;
    }

    _FVR_Lock();
    
    u4PidEntry = FVR_PID_INDEX_TABLE(u1Pidx);

    if(u4Flags & FVR_PID_FLAG_TSINDEX)
    {
        _arFVRPidStruct[u1Pidx].u1TsIndex= prPid->u1TsIndex;
        u4PidEntry &= 0x9fffffff;
        u4PidEntry |= (prPid->u1TsIndex & 0x3) << 29;
    }

    if(u4Flags & FVR_PID_FLAG_PID)
    {
        _arFVRPidStruct[u1Pidx].u2Pid = prPid->u2Pid;
        u4PidEntry &= ~0x1FFF0000;
        u4PidEntry |= ((prPid->u2Pid & 0x1FFF) << 16);
    }

    if(u4Flags & FVR_PID_FLAG_PICSEARCH)
    {
        _arFVRPidStruct[u1Pidx].fgPicSearch = prPid->fgPicSearch;
        _arFVRPidStruct[u1Pidx].u1PicSearchMask = prPid->u1PicSearchMask;
        _arFVRPidStruct[u1Pidx].pfnPicSearchNotify = prPid->pfnPicSearchNotify;
        _arFVRPidStruct[u1Pidx].pvPicSearchNotifyTag = prPid->pvPicSearchNotifyTag;
        _arFVRPidStruct[u1Pidx].u1PerPidIdx = prPid->u1PerPidIdx;
        if(prPid->fgPicSearch && (prPid->u1PerPidIdx >= FVR_PER_PID_NUM))
        {
            _FVR_Unlock();
            LOG(1, "Video search index too large\n");            
            return FALSE;
        }
        
        u4PidEntry &= ~0xF800;

        if(prPid->fgPicSearch)
        {
            u4PidEntry |= 0x800;
            u4PidEntry |= ((prPid->u1PerPidIdx & 0xF) << 12);
        }
    }

    if(u4Flags & FVR_PID_FLAG_PKTSWAP)
    {
        _arFVRPidStruct[u1Pidx].fgPktSwap = prPid->fgPktSwap;
        _arFVRPidStruct[u1Pidx].pfnPicSearchNotify = prPid->pfnPicSearchNotify;
        _arFVRPidStruct[u1Pidx].pvSwapNotifyTag = prPid->pvSwapNotifyTag;
        u4PidEntry &= ~0x400;
        if(prPid->fgPktSwap)
        {
            u4PidEntry |= 0x400;
        }
    }

    if(u4Flags & FVR_PID_FLAG_KEY_TYPE)
    {
        _arFVRPidStruct[u1Pidx].eKeyType = prPid->eKeyType;
        u4PidEntry &= ~0x80;

        if((prPid->eKeyType == FVR_KEY_EVEN) || (prPid->eKeyType == FVR_KEY_BOTH))
        {
            _arFVRPidStruct[u1Pidx].fgOddKey = FALSE;
        }
        else
        {
            _arFVRPidStruct[u1Pidx].fgOddKey = TRUE;
            u4PidEntry |= 0x80;
        }
    }

    if(u4Flags & FVR_PID_FLAG_KEY_LEN)
    {
        if((prPid->u4KeyLen == 128) || (prPid->u4KeyLen == 192) ||
           (prPid->u4KeyLen == 256))
        {
            _arFVRPidStruct[u1Pidx].u4KeyLen = prPid->u4KeyLen;
            u4PidEntry &= ~0xC;

            if(prPid->u4KeyLen == 128)
            {
                u4PidEntry |= 0x0;
            }
            else if(prPid->u4KeyLen == 192)
            {
                u4PidEntry |= 0x4;
            }
            else if(prPid->u4KeyLen == 256)
            {
                u4PidEntry |= 0x8;
            }
        }
        else
        {
            if(!_DMX_GetIsPanicReset())
            {
                _FVR_Unlock();
                LOG(1, "Key length wrong %d\n", prPid->u4KeyLen);
                return FALSE;
            }
        }
    }

    if(u4Flags & FVR_PID_FLAG_KEY_INDEX)
    {
        _arFVRPidStruct[u1Pidx].u1KeyIdx = prPid->u1KeyIdx;
        u4PidEntry |= ((prPid->u1KeyIdx & 0x7) << 4);
    }

    if(u4Flags & FVR_PID_FLAG_CBC_MODE)
    {
        _arFVRPidStruct[u1Pidx].fgCBCMode = prPid->fgCBCMode;
        u4PidEntry &= ~0x1;
        if(prPid->fgCBCMode)
        {
            u4PidEntry |= 0x1;
        }
    }

    if(u4Flags & FVR_PID_FLAG_SCRAMBLE_ENABLE)
    {
        _arFVRPidStruct[u1Pidx].fgScramble = prPid->fgScramble;
        u4PidEntry &= ~0x2;
        if(prPid->fgScramble)
        {
            u4PidEntry |= 0x2;
        }
    }

    if(u4Flags & FVR_PID_FLAG_DETECT_SCRAMBLE)
    {
        _arFVRPidStruct[u1Pidx].fgDetectScramble = prPid->fgDetectScramble;
        _arFVRPidStruct[u1Pidx].pfnDetectScrambleNotify = prPid->pfnDetectScrambleNotify;
        _arFVRPidStruct[u1Pidx].u1PerPidIdx = prPid->u1PerPidIdx;

        if(prPid->fgDetectScramble && (prPid->u1PerPidIdx >= FVR_PER_PID_NUM))
        {
            _FVR_Unlock();
            LOG(1, "Video search index too large\n");
            return FALSE;
        }
        
        // bit 9: detect scramble
        // bit 15: A/V PES
        u4PidEntry &= ~0x8200;
        if(prPid->fgDetectScramble)
        {
            u4PidEntry |= 0x8200;
            u4PidEntry |= ((prPid->u1PerPidIdx & 0xF) << 12);            
        }
    }

    if(u4Flags & FVR_PID_FLAG_FREEZE)
    {
        _arFVRPidStruct[u1Pidx].fgFreeze = prPid->fgFreeze;
        u4PidEntry &= ~0x100;
        if(prPid->fgFreeze)
        {
            u4PidEntry |= 0x100;
        }
    }

    if(u4Flags & FVR_PID_FLAG_ENABLE)
    {
        // Check toggle PID
        _FVR_Unlock();
        _FVR_SetIsToggleKey();
        _FVR_Lock();

        _arFVRPidStruct[u1Pidx].fgEnable = prPid->fgEnable;
        if(prPid->fgEnable)
        {
            // Set per-pid struct to default value
            u4PidEntry |= 0x80000000;

            if(_arFVRPidStruct[u1Pidx].fgDetectScramble || _arFVRPidStruct[u1Pidx].fgPicSearch)
            {
                FVR_PER_PID_VID_S_W(_arFVRPidStruct[u1Pidx].u1PerPidIdx, 0) = 0xFFFF00FF;
                FVR_PER_PID_VID_S_W(_arFVRPidStruct[u1Pidx].u1PerPidIdx, 1) = 0xFFFFFF;
            }            
        }
        else
        {
            u4PidEntry &= ~0x80000000;
        }
    }

    FVR_PID_INDEX_TABLE(u1Pidx) = u4PidEntry;

    _FVR_Unlock();

    if(u4Flags & FVR_PID_FLAG_ENABLE)
    {        
        LOG(5, "%s, enable:%d\n", __FUNCTION__, (INT32)prPid->fgEnable);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetPid
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetPid(UINT8 u1Pidx, UINT32 u4Flags, FVR_PID_T *prPid)
{    
    ASSERT(u1Pidx < FVR_NUM_PID_INDEX);

    if(prPid == NULL)
    {
        return FALSE;
    }

    if (u4Flags == FVR_PID_FLAG_NONE)
    {
        return TRUE;
    }

    if(u4Flags & FVR_PID_FLAG_TSINDEX)
    {
        prPid->u1TsIndex = _arFVRPidStruct[u1Pidx].u1TsIndex;
    }
    
    if(u4Flags & FVR_PID_FLAG_PID)
    {
        prPid->u2Pid = _arFVRPidStruct[u1Pidx].u2Pid;
    }

    if(u4Flags & FVR_PID_FLAG_PICSEARCH)
    {
        prPid->fgPicSearch = _arFVRPidStruct[u1Pidx].fgPicSearch;
        prPid->u1PicSearchMask = _arFVRPidStruct[u1Pidx].u1PicSearchMask;
        prPid->pfnPicSearchNotify= _arFVRPidStruct[u1Pidx].pfnPicSearchNotify;
        prPid->pvPicSearchNotifyTag = _arFVRPidStruct[u1Pidx].pvPicSearchNotifyTag;
        prPid->u1PerPidIdx = _arFVRPidStruct[u1Pidx].u1PerPidIdx;
    }
    
    if(u4Flags & FVR_PID_FLAG_PKTSWAP)
    {
        prPid->fgPktSwap = _arFVRPidStruct[u1Pidx].fgPktSwap;
        prPid->pfnSwapNotify= _arFVRPidStruct[u1Pidx].pfnSwapNotify;
        prPid->pvSwapNotifyTag = _arFVRPidStruct[u1Pidx].pvSwapNotifyTag;
    }

    if(u4Flags & FVR_PID_FLAG_KEY_TYPE)
    {
        prPid->eKeyType = _arFVRPidStruct[u1Pidx].eKeyType;
    }

    if(u4Flags & FVR_PID_FLAG_KEY_LEN)
    {
        prPid->u4KeyLen = _arFVRPidStruct[u1Pidx].u4KeyLen;
    }

    if(u4Flags & FVR_PID_FLAG_KEY_INDEX)
    {
        prPid->u1KeyIdx = _arFVRPidStruct[u1Pidx].u1KeyIdx;
    }   

    if(u4Flags & FVR_PID_FLAG_CBC_MODE)
    {
        prPid->fgCBCMode = _arFVRPidStruct[u1Pidx].fgCBCMode;
    }

    if(u4Flags & FVR_PID_FLAG_SCRAMBLE_ENABLE)
    {
        prPid->fgScramble = _arFVRPidStruct[u1Pidx].fgScramble;
    }

    if(u4Flags & FVR_PID_FLAG_DETECT_SCRAMBLE)
    {
        prPid->fgDetectScramble = _arFVRPidStruct[u1Pidx].fgDetectScramble;
        prPid->pfnDetectScrambleNotify = _arFVRPidStruct[u1Pidx].pfnDetectScrambleNotify;
        prPid->u1PerPidIdx = _arFVRPidStruct[u1Pidx].u1PerPidIdx;
    }

    if(u4Flags & FVR_PID_FLAG_FREEZE)
    {
        prPid->fgFreeze = _arFVRPidStruct[u1Pidx].fgFreeze;
    }

    if(u4Flags & FVR_PID_FLAG_ENABLE)
    {
        prPid->fgEnable = _arFVRPidStruct[u1Pidx].fgEnable;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetPidStruct
 *  @retval 
 */
//-----------------------------------------------------------------------------
FVR_PID_STRUCT_T* _FVR_GetPidStruct(UINT8 u1Pidx)
{
    ASSERT(u1Pidx < FVR_PID_COUNT);

    return &_arFVRPidStruct[u1Pidx];
}


//-----------------------------------------------------------------------------
/** _FVR_FreePid
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL _FVR_FreePid(UINT8 u1Pidx)
{
    FVR_PID_STRUCT_T *prPid;

    LOG(5, "%s\n", __FUNCTION__);
    
    if(u1Pidx >= FVR_PID_COUNT)
    {
        return FALSE;
    }

    prPid = _FVR_GetPidStruct(u1Pidx);

    if(prPid == NULL)
    {
        return FALSE;
    }

    if(prPid->fgEnable)
    {
        LOG(7, "Try to free enable PVR pid\n");
    }

    if(!_DMX_GetIsPanicReset())
    {
        if(prPid->fgPktSwap)
        {
            if(!_FVR_SwapFreePattern(u1Pidx))
            {
                LOG(3, "No set swap pattern\n");
            }
        }
    }

    _FVR_Lock();
    FVR_PID_INDEX_TABLE(u1Pidx) = 0x0;    
    x_memset((void*)prPid, 0, sizeof(FVR_PID_STRUCT_T));
    _FVR_Unlock();
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetTimestamp
 *  @retval 
 */
//-----------------------------------------------------------------------------
void _FVR_SetTimestamp(UINT32 u4Flag, const FVR_TIMESTAMP_T* prTimeStamp)
{
    UINT32 u4Reg;

    if(prTimeStamp == NULL)
    {
        LOG(1, "Null timestamp\n");
        return;
    }

    if(u4Flag & FVR_TIMESTAMP_FLAG_MASK)
    {
        _rTimeStamp.u4Mask = prTimeStamp->u4Mask;
    }

    if(u4Flag & FVR_TIMESTAMP_FLAG_CLKDIV)
    {
        _rTimeStamp.u2ClkDiv = prTimeStamp->u2ClkDiv;
        DMXCMD_WRITE32(DMX_REG_REC_CONFIG1, prTimeStamp->u2ClkDiv);
    }
/*  Unused now
    if(u4Flag & FVR_TIMESTAMP_FLAG_COUNTER)
    {
        _rTimeStamp.u4Counter = prTimeStamp->u4Counter;
        DMXCMD_WRITE32(DMX_REG_REC_CONFIG2, prTimeStamp->u4Counter);
        x_thread_delay(1);
    }
*/
    if(u4Flag & FVR_TIMESTAMP_FLAG_ENABLE)
    {
        _rTimeStamp.fgEnable = prTimeStamp->fgEnable;

        u4Reg = DMXCMD_READ32(DMX_REG_CONFIG2) & 0xDFFFFFFF;
        
        if(prTimeStamp->fgEnable)
        {
            u4Reg |= 0x20000000;
        }
        
        DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Reg);
    }

}


//-----------------------------------------------------------------------------
/** _FVR_GetTimestamp
 *  @retval 
 */
//-----------------------------------------------------------------------------
void _FVR_GetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp)
{
    
    if(prTimeStamp == NULL)
    {
        LOG(1, "Null timestamp\n");
        return;
    }
    
    if(u4Flags & FVR_TIMESTAMP_FLAG_MASK)
    {
        prTimeStamp->u4Mask = _rTimeStamp.u4Mask;
    }

    if(u4Flags & FVR_TIMESTAMP_FLAG_CLKDIV)
    {
        prTimeStamp->u2ClkDiv = _rTimeStamp.u2ClkDiv;
    }
    
    if(u4Flags & FVR_TIMESTAMP_FLAG_COUNTER)
    {
        prTimeStamp->u4Counter = _rTimeStamp.u4Counter;
    }

    if(u4Flags & FVR_TIMESTAMP_FLAG_ENABLE)
    {
        prTimeStamp->fgEnable = _rTimeStamp.fgEnable;
    }
}


//-----------------------------------------------------------------------------
/** _FVR_FlushBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL _FVR_FlushBuffer(void)
{
    UINT32 u4BufStart;

    LOG(5, "%s\n", __FUNCTION__);

    if(_FVR_IsRecording())
    {
        LOG(3, "Still recording...\n");
        return FALSE;
    }

    // Avoid race condition
    x_thread_delay(5);

    _FVR_Lock();

    u4BufStart = _rGblStruct.u4BufStart;
    FVR_GBL_PID_W(3) = PHYSICAL(u4BufStart);              // write pointer
    FVR_GBL_PID_W(4) = PHYSICAL(u4BufStart);              // read pointer
    _rGblStruct.u4Wp = u4BufStart;
    _rGblStruct.u4Rp = u4BufStart;
    _rGblStruct.u4PktRp = u4BufStart;

    // Update pkt count
    _rGblStruct.u2CurPktCount = 0;
    _rGblStruct.u4LongPktCount = 0;
    FVR_GBL_PID_W(5) &= 0xFFFF0000;
    FVR_GBL_PID_W(6) = 0x0;        

    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetBuffer(UINT32 u4Addr, UINT32 u4CopySize, UINT8 *pu1Buf, UINT32 *pu4RetAddr)
{
    UINT32 u4BufStart, u4BufEnd, u4BufSize, u4Wp, u4Rp;
    UINT32 u4DataSize, u4TotalSize, u4Size;

    if(u4CopySize == 0)
    {
        return TRUE;
    }

    if((pu1Buf == NULL) || (pu4RetAddr == NULL))
    {
        return FALSE;
    }

    _FVR_Lock();

    u4BufStart = _rGblStruct.u4BufStart;
    u4BufEnd = _rGblStruct.u4BufEnd + 1;
    u4BufSize = _rGblStruct.u4BufSize;
    u4Wp = _rGblStruct.u4Wp;
    u4Rp = _rGblStruct.u4Rp;

    _FVR_Unlock();

    u4DataSize = DATASIZE(u4Addr, u4Wp, u4BufSize);
    u4TotalSize = DATASIZE(u4Rp, u4Wp, u4BufSize);

    if((u4Addr < u4BufStart) || (u4BufEnd <= u4Addr) || (u4DataSize > u4TotalSize))
    {
        LOG(3, "_FVR_GetBuffer, invalid addr! Rp:0x%x, Wp:0x%x\n", u4Rp, u4Wp);
        LOG(3, "   DataAddr:0x%x, DataSize:0x%x, TotalSize:0x%x\n", u4Addr, u4DataSize, u4TotalSize);
        return FALSE;
    }

    if(u4CopySize > u4DataSize)
    {
        LOG(3, "_FVR_GetBuffer: not enough size!\n");
        return FALSE;
    }

    // Copy data
    u4Rp = u4Addr;
    if ((u4Rp > u4Wp) && (u4CopySize > 0))
    {
        u4Size = MIN(u4BufEnd - u4Rp, u4CopySize);
        x_memcpy(pu1Buf, (VOID*)u4Rp, u4Size);
        u4Rp += u4Size;
        if (u4Rp >= u4BufEnd)
        {
            u4Rp = u4BufStart;
        }
        pu1Buf += u4Size;
        u4CopySize -= u4Size;
    }

    if ((u4Rp < u4Wp) && (u4CopySize > 0))
    {
        u4Size = MIN(u4Wp - u4Rp, u4CopySize);
        x_memcpy(pu1Buf, (void*)u4Rp, u4Size);
        u4Rp += u4Size;
    }

    *pu4RetAddr = u4Rp;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_UnlockBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL _FVR_UnlockBuffer(UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 u4BufStart, u4BufEnd, u4Rp, u4Wp, u4BufSize;
    UINT32 u4DataSize, u4NewRp;

    _FVR_Lock();
    u4BufStart = _rGblStruct.u4BufStart;
    u4BufEnd = _rGblStruct.u4BufEnd + 1;
    u4Rp = _rGblStruct.u4Rp;
    u4Wp = _rGblStruct.u4Wp;
    _FVR_Unlock();

    if((u4Addr < u4BufStart) || (u4Addr >= u4BufEnd))
    {
        LOG(5, "%s, addr:0x%x not in buf range\n", __FUNCTION__, u4Addr);
        return FALSE;
    }

    u4BufSize = u4BufEnd - u4BufStart;
    u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);
    if(u4DataSize < u4Size)
    {
        LOG(5, "%s, data size not enough, 0x%x, 0x%x\n", __FUNCTION__, u4DataSize, u4Size);
        return FALSE;
    }

    u4NewRp = u4Addr + u4Size;
    if(u4NewRp >= u4BufEnd)    
    {
        u4NewRp -= (u4BufEnd - u4BufStart);
    }

    _FVR_Lock();
    _rGblStruct.u4Rp = u4NewRp;
    FVR_GBL_PID_W(4) = PHYSICAL(u4NewRp);              // read pointer
    _FVR_Unlock();
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_FreeBuffer
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_FreeBuffer(void)
{
    BOOL fgAllocBuf;
    UINT32 u4BufStart;

    if(_FVR_IsRecording())
    {
        LOG(3, "Still recording...\n");
        return FALSE;
    }
    
    // Avoid race condition
    x_thread_delay(5);

    _FVR_Lock();
    fgAllocBuf = _rGblStruct.fgAllocateBuffer;
    u4BufStart = _rGblStruct.u4BufStart;
    _FVR_Unlock();

    // Free buffer
    if (fgAllocBuf)
    {
        if (!BSP_FreeAlignedDmaMemory((void*)u4BufStart))
        {
            LOG(3, "Fail to free PID buffer!, addr: 0x%08x\n", u4BufStart);
            return FALSE;
        }
    }

    _FVR_Lock();    
    _rGblStruct.u4BufStart = 0x0;
    _rGblStruct.u4BufEnd = 0x0;
    _rGblStruct.u4Wp = 0x0;
    _rGblStruct.u4Rp = 0x0;
    _rGblStruct.u4PktRp = 0x0;

    FVR_GBL_PID_W(0) = 0x0;
    FVR_GBL_PID_W(1) = 0x0;
    FVR_GBL_PID_W(3) = 0x0;              // write pointer
    FVR_GBL_PID_W(4) = 0x0;              // read pointer    
    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetByPass
 *  @retval 
 */
//-----------------------------------------------------------------------------
void _FVR_SetByPass(UINT8 u1TsIndex, UINT32 u4Val, BOOL fgEnable)
{    
    UINT32 u4Cfg;

    if(fgEnable)
    {
        // Set DBM to normal mode and keep all TS packets
        u4Cfg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Cfg &= 0xc3ffffff;
        u4Cfg |= (1 << (26 + u1TsIndex));
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Cfg);

        // Set DBM Bypass PID register, steer to PVR
        DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID + u1TsIndex, u4Val);
    }
    else
    {
        // Set DBM to normal mode and keep all TS packets
        u4Cfg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Cfg &= 0xc3ffffff;
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Cfg);

        DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID + u1TsIndex, 0x0);
    }

}


//-----------------------------------------------------------------------------
/** _FVR_SetVideoType
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetVideoType(FVR_VIDEO_TYPE_T eVideoType)
{
    switch(eVideoType)
    {
    case FVR_VIDEO_NONE:        
        if(!_SetFVRVideoStartCode((UINT8)(sizeof(_arFVRNONEStartCode) / sizeof(FVR_STARTCODE_T)),
                              _arFVRNONEStartCode))
        {
            return FALSE;
        }
        break;

    case FVR_VIDEO_MPEG:
        if(!_SetFVRVideoStartCode((UINT8)(sizeof(_arFVRMPEG2StartCode) / sizeof(FVR_STARTCODE_T)),
                              _arFVRMPEG2StartCode))
        {
            return FALSE;
        }
        break;

    case FVR_VIDEO_H264:        
        if(!_SetFVRVideoStartCode((UINT8)(sizeof(_arFVRH264StartCode) / sizeof(FVR_STARTCODE_T)),
                              _arFVRH264StartCode))
        {
            return FALSE;
        }
        break;

    default:
        LOG(1, "Unknow record video type\n");
        return FALSE;
    }

    _eFVRVideoType = eVideoType;
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetVideoType
 *  @retval 
 */
//-----------------------------------------------------------------------------
FVR_VIDEO_TYPE_T _FVR_GetVideoType(void)
{
    return _eFVRVideoType;
}


//-----------------------------------------------------------------------------
/** _FVR_SetAesIV
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetAesIV(UINT32 *pu4IV)
{
    UINT32 i;

    if(pu4IV == NULL)
    {
        return FALSE;
    }
    
    for(i=0; i<4; i++)
    {
        FVR_CBC_IV_W(3 - i) = pu4IV[i];
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetAesKey
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetAesKey(UINT8 u1KeyIdx, BOOL fgEven, UINT8 *pu1Key)
{
    UINT32 i;

    if(pu1Key == NULL)
    {
        return FALSE;
    }
    
    if(fgEven)
    {
        for(i=0; i<8; i++)
        {
            FVR_PER_PID_KEY_S_W(u1KeyIdx, 15 - i) = 
                                     pu1Key[(i*4) + 3] |
                                    (pu1Key[(i*4) + 2] << 8) |
                                    (pu1Key[(i*4) + 1] << 16) |
                                    (pu1Key[i*4] << 24);
        }
    }
    else
    {
        for(i=0; i<8; i++)
        {
            FVR_PER_PID_KEY_S_W(u1KeyIdx, 7 - i) = 
                                     pu1Key[(i*4) + 3] |
                                    (pu1Key[(i*4) + 2] << 8) |
                                    (pu1Key[(i*4) + 1] << 16) |
                                    (pu1Key[i*4] << 24);
        }
    }
    
    return TRUE;
}

