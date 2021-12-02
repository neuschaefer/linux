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

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define FVR_FIFO_ALIGNMENT      64

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


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

static FVR_TIMESTAMP_T _rTimeStamp;


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
    UINT32 u4BufStart;
    
    // Check if buffer already allocated, to avoid memory leakage
    if ((_rGblStruct.u4BufStart != 0) || (_rGblStruct.u4BufEnd != 0))
    {
        return FALSE;
    }

    if(_rGblStruct.fgAllocateBuffer)
    {
    	// Setup header buffer start address
    	u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(_rGblStruct.u4BufSize,
                            	    FVR_FIFO_ALIGNMENT);
    	if (u4BufStart == 0)
    	{
    		LOG(3, "Can't allocate memory!\n");
    		return FALSE;
    	}    
    }
    else
    {
        ASSERT(_rGblStruct.u4BufStart != 0);
        u4BufStart = prGBL->u4BufStart;    
    }

    FVR_GBL_PID_W(0) = u4BufStart;                                  // Ring buf start
    FVR_GBL_PID_W(1) = u4BufStart + _rGblStruct.u4BufSize - 1;      // Ring buf end
    FVR_GBL_PID_W(3) = u4BufStart;                                  // write pointer
    FVR_GBL_PID_W(4) = u4BufStart;                                  // read pointer

    _rGblStruct.u4BufStart = u4BufStart;
    _rGblStruct.u4BufEnd = u4BufStart + _rGblStruct.u4BufSize - 1;
    _rGblStruct.u4Rp = u4BufStart;
    _rGblStruct.u4Wp = u4BufStart;
    _rGblStruct.u4PktRp = u4BufStart;

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

    ASSERT(!_fgFVRLocking);
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
    ASSERT(_fgFVRLocking);
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
}


//-----------------------------------------------------------------------------
/** _FVR_SetGBL
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetGBL(UINT32 u4Flags, const FVR_GBL_T* prGBL)
{
    UINT32 u4Ctrl;

    if (u4Flags == FVR_GBL_FLAG_NONE)
    {
        return TRUE;
    }  
        
    if(u4Flags & FVR_GBL_FLAG_BUFFER)
    {
        _rGblStruct.fgAllocateBuffer = prGBL->fgAllocateBuffer;
        _rGblStruct.u4BufSize = prGBL->u4BufSize;
        if(!_SetFVRBuffer(prGBL))
        {
            return FALSE;
        }
    }

    if(u4Flags & FVR_GBL_FLAG_TARGET_PKT)
    {
        _rGblStruct.u2TargetPktCount = prGBL->u2TargetPktCount;
        u4Ctrl = FVR_GBL_PID_W(5);
        u4Ctrl &= 0xFFFF;
        u4Ctrl |= (UINT32)((prGBL->u2TargetPktCount)<<16);
        FVR_GBL_PID_W(5) = u4Ctrl;        
    }
    
    if(u4Flags & FVR_GBL_FLAG_CBC_IV)
    {
        _rGblStruct.u4CBCIVLow = prGBL->u4CBCIVLow;
        _rGblStruct.u4CBCIVHigh = prGBL->u4CBCIVHigh;
        FVR_GBL_PID_W(7) = prGBL->u4CBCIVLow;
        FVR_GBL_PID_W(8) = prGBL->u4CBCIVHigh;
    }

    if(u4Flags & FVR_GBL_FLAG_NOTIFY)
    {
        _rGblStruct.pfnPktCountNotify = prGBL->pfnPktCountNotify;
    }

    if(u4Flags & FVR_GBL_FLAG_TIME_NOTIFY)
    {
        _rGblStruct.fgEnableTimeNotify = prGBL->fgEnableTimeNotify;
        _rGblStruct.pfnTimeNotify = prGBL->pfnTimeNotify;
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
    if (u4Flags == FVR_GBL_FLAG_NONE)
    {
        return TRUE;
    }  
        
    if(u4Flags & FVR_GBL_FLAG_BUFFER)
    {
        prGBL->fgAllocateBuffer = _rGblStruct.fgAllocateBuffer;
        prGBL->u4BufSize = _rGblStruct.u4BufSize;
        prGBL->u4BufStart = _rGblStruct.u4BufStart;
    }

    if(u4Flags & FVR_GBL_FLAG_CBC_IV)
    {
        prGBL->u4CBCIVLow = _rGblStruct.u4CBCIVLow;
        prGBL->u4CBCIVHigh = _rGblStruct.u4CBCIVHigh;
    }

    if(u4Flags & FVR_GBL_FLAG_NOTIFY)
    {
        prGBL->pfnPktCountNotify = _rGblStruct.pfnPktCountNotify;
    }

    if(u4Flags & FVR_GBL_FLAG_TIME_NOTIFY)
    {
        prGBL->fgEnableTimeNotify = _rGblStruct.fgEnableTimeNotify;
        prGBL->pfnTimeNotify =  _rGblStruct.pfnTimeNotify;
        prGBL->u4TimeToNotify = _rGblStruct.u4TimeToNotify;
    }    

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetGBLStruct
 *  @retval 
 */
//-----------------------------------------------------------------------------
FVR_GBL_STRUCT_T* _FVR_GetGBLStruct()
{
    return &_rGblStruct;
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

    if (u4Flags == FVR_PID_FLAG_NONE)
    {
        return TRUE;
    }  

    _FVR_Lock();
    u4PidEntry = FVR_PID_INDEX_TABLE(u1Pidx);
    _FVR_Unlock();

#if 0   
    if(u4Flags & FVR_PID_FLAG_TS_INDEX)
    {
        _arFVRPidStruct[u1Pidx].u1TsIndex = prPid->u1TsIndex;
        u4PidEntry &= 0xFFFFFFF;
        u4PidEntry |= (prPid->u1TsIndex << 28);
    }
#endif
    if(u4Flags & FVR_PID_FLAG_PID)
    {
        _arFVRPidStruct[u1Pidx].u2Pid = prPid->u2Pid;
        u4PidEntry &= ~0xFFE000;
        u4PidEntry |= (prPid->u2Pid << 15);
    }

    if(u4Flags & FVR_PID_FLAG_PICSEARCH)
    {
        _arFVRPidStruct[u1Pidx].fgPicSearch = prPid->fgPicSearch;
        _arFVRPidStruct[u1Pidx].u1PicSearchMask = prPid->u1PicSearchMask;
        _arFVRPidStruct[u1Pidx].pfnPicSearchNotify = prPid->pfnPicSearchNotify;
        u4PidEntry &= ~0x20;
        if(prPid->fgPicSearch)
        {
            u4PidEntry |= 0x20;
        }
    }

    if(u4Flags & FVR_PID_FLAG_PKTSWAP)
    {
        _arFVRPidStruct[u1Pidx].fgPktSwap = prPid->fgPktSwap;
        _arFVRPidStruct[u1Pidx].pfnPicSearchNotify = prPid->pfnPicSearchNotify;
        u4PidEntry &= ~0x10;
        if(prPid->fgPktSwap)
        {
            u4PidEntry |= 0x10;
        }
    }

    if(u4Flags & FVR_PID_FLAG_KEY_TYPE)
    {
        _arFVRPidStruct[u1Pidx].rKeyType = prPid->rKeyType;
        u4PidEntry &= ~0x8;

        if((prPid->rKeyType == FVR_KEY_EVEN) || (prPid->rKeyType == FVR_KEY_BOTH))
        {
            _arFVRPidStruct[u1Pidx].fgOddKey = FALSE;
        }
        else
        {
            _arFVRPidStruct[u1Pidx].fgOddKey = TRUE;
            u4PidEntry |= 0x8;
        }        
    }
    
    if(u4Flags & FVR_PID_FLAG_ENCRYPT_METHOD)
    {
        _arFVRPidStruct[u1Pidx].fg3DES = prPid->fg3DES;
        u4PidEntry &= ~0x4;
        if(prPid->fg3DES)
        {
            u4PidEntry |= 0x4;
        }
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

    if(u4Flags & FVR_PID_FLAG_EVEN_KEY_1)
    {
        _arFVRPidStruct[u1Pidx].u4EvenKey1Low = prPid->u4EvenKey1Low;
        _arFVRPidStruct[u1Pidx].u4EvenKey1High = prPid->u4EvenKey1High;
        FVR_PER_PID_S_W(u1Pidx, 7) = prPid->u4EvenKey1Low;
        FVR_PER_PID_S_W(u1Pidx, 8) = prPid->u4EvenKey1High;
    }

    if(u4Flags & FVR_PID_FLAG_EVEN_KEY_2)
    {
        _arFVRPidStruct[u1Pidx].u4EvenKey2Low = prPid->u4EvenKey2Low;
        _arFVRPidStruct[u1Pidx].u4EvenKey2High = prPid->u4EvenKey2High;    
        FVR_PER_PID_S_W(u1Pidx, 9) = prPid->u4EvenKey2Low;
        FVR_PER_PID_S_W(u1Pidx, 10) = prPid->u4EvenKey2High;        
    }

    if(u4Flags & FVR_PID_FLAG_ODD_KEY_1)
    {
        _arFVRPidStruct[u1Pidx].u4OddKey1Low = prPid->u4OddKey1Low;
        _arFVRPidStruct[u1Pidx].u4OddKey1High = prPid->u4OddKey1High;
        FVR_PER_PID_S_W(u1Pidx, 3) = prPid->u4OddKey1Low;
        FVR_PER_PID_S_W(u1Pidx, 4) = prPid->u4OddKey1High;
    }

    if(u4Flags & FVR_PID_FLAG_ODD_KEY_2)
    {
        _arFVRPidStruct[u1Pidx].u4OddKey2Low = prPid->u4OddKey2Low;
        _arFVRPidStruct[u1Pidx].u4OddKey2High = prPid->u4OddKey2High;
        FVR_PER_PID_S_W(u1Pidx, 5) = prPid->u4OddKey2Low;
        FVR_PER_PID_S_W(u1Pidx, 6) = prPid->u4OddKey2High;        
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

    if(u4Flags & FVR_PID_FLAG_ENABLE)
    {
        _arFVRPidStruct[u1Pidx].fgEnable = prPid->fgEnable;
        if(prPid->fgEnable)
        {
            // Set per-pid struct to default value
            FVR_PER_PID_S_W(u1Pidx, 1) = 0xFFFF00FF;
            FVR_PER_PID_S_W(u1Pidx, 2) = 0xFFFF;
            u4PidEntry &= 0xFFFFFFF;
        }
        else
        {
            u4PidEntry |= 0xF0000000;
        }
    }

    _FVR_Lock();
    FVR_PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
    _FVR_Unlock();

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

    if (u4Flags == FVR_PID_FLAG_NONE)
    {
        return TRUE;
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
    }
    
    if(u4Flags & FVR_PID_FLAG_PKTSWAP)
    {
        prPid->fgPktSwap = _arFVRPidStruct[u1Pidx].fgPktSwap;
        prPid->pfnSwapNotify= _arFVRPidStruct[u1Pidx].pfnSwapNotify;
    }

    if(u4Flags & FVR_PID_FLAG_KEY_TYPE)
    {
        prPid->rKeyType = _arFVRPidStruct[u1Pidx].rKeyType;
    }
    
    if(u4Flags & FVR_PID_FLAG_ENCRYPT_METHOD)
    {
        prPid->fg3DES = _arFVRPidStruct[u1Pidx].fg3DES;
    }

    if(u4Flags & FVR_PID_FLAG_CBC_MODE)
    {
        prPid->fgCBCMode = _arFVRPidStruct[u1Pidx].fgCBCMode;
    }

    if(u4Flags & FVR_PID_FLAG_EVEN_KEY_1)
    {
        prPid->u4EvenKey1Low = _arFVRPidStruct[u1Pidx].u4EvenKey1Low;
        prPid->u4EvenKey1High = _arFVRPidStruct[u1Pidx].u4EvenKey1High;
    }

    if(u4Flags & FVR_PID_FLAG_EVEN_KEY_2)
    {
        prPid->u4EvenKey2Low = _arFVRPidStruct[u1Pidx].u4EvenKey2Low;
        prPid->u4EvenKey2High = _arFVRPidStruct[u1Pidx].u4EvenKey2High;
    }

    if(u4Flags & FVR_PID_FLAG_ODD_KEY_1)
    {
        prPid->u4OddKey1Low = _arFVRPidStruct[u1Pidx].u4OddKey1Low;
        prPid->u4OddKey1High = _arFVRPidStruct[u1Pidx].u4OddKey1High;
    }

    if(u4Flags & FVR_PID_FLAG_ODD_KEY_2)
    {
        prPid->u4OddKey2Low = _arFVRPidStruct[u1Pidx].u4OddKey2Low;
        prPid->u4OddKey2High = _arFVRPidStruct[u1Pidx].u4OddKey2High;
    }

    if(u4Flags & FVR_PID_FLAG_SCRAMBLE_ENABLE)
    {
        prPid->fgScramble = _arFVRPidStruct[u1Pidx].fgScramble;
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
    
    ASSERT(u1Pidx < FVR_PID_COUNT);

    prPid = _FVR_GetPidStruct(u1Pidx);
    
    if(prPid->fgEnable)
    {
        // Disable PID
        FVR_PID_INDEX_TABLE(u1Pidx) |= 0xF0000000;
        prPid->fgEnable = FALSE;
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetTimestamp
 *  @retval 
 */
//-----------------------------------------------------------------------------
void _FVR_SetTimestamp(UINT32 u4Flag, FVR_TIMESTAMP_T* prTimeStamp)
{
    UINT32 u4Reg;
    
    if(u4Flag & FVR_TIMESTAMP_FLAG_MASK)
    {
        _rTimeStamp.u4Mask = prTimeStamp->u4Mask;
        FVR_WRITE32(FVR_REG_REC_TIMESTAMP_MASK, _rTimeStamp.u4Mask);
    }

    if(u4Flag & FVR_TIMESTAMP_FLAG_CLKDIV)
    {
        _rTimeStamp.u2ClkDiv = prTimeStamp->u2ClkDiv;
        FVR_WRITE32(FVR_REG_REC_TIMESTAMP_DIV, _rTimeStamp.u2ClkDiv);
    }
    
    if(u4Flag & FVR_TIMESTAMP_FLAG_COUNTER)
    {
        _rTimeStamp.u4Counter = prTimeStamp->u4Counter;
        FVR_WRITE32(FVR_REG_REC_TIMESTAMP_COUNTER, _rTimeStamp.u4Counter);
    }

    if(u4Flag & FVR_TIMESTAMP_FLAG_ENABLE)
    {
        _rTimeStamp.fgEnable = prTimeStamp->fgEnable;

        u4Reg = FVR_READ32(FVR_REG_CONFIG_3);
        u4Reg &= ~0x800;
        if(_rTimeStamp.fgEnable)
        {
            FVR_WRITE32(FVR_REG_CONFIG_3, (u4Reg | 0x800));
        }
        else
        {
            FVR_WRITE32(FVR_REG_CONFIG_3, u4Reg);
        }
    }

}


//-----------------------------------------------------------------------------
/** _FVR_GetTimestamp
 *  @retval 
 */
//-----------------------------------------------------------------------------
void _FVR_GetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp)
{
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
BOOL _FVR_FlushBuffer()
{
    UINT32 u4BufStart;
//    UINT32 u4BufEnd, u4Rp, u4Wp;

    u4BufStart = _rGblStruct.u4BufStart;
#if 0
    // For debug purpose
    if (_DMX_IDE_IsEnabled())
    {    
        HalFlushInvalidateDCache();

        u4BufEnd = _rGblStruct.u4BufEnd + 1;
        u4Rp = _rGblStruct.u4Rp;
        u4Wp = FVR_GBL_PID_W(3);
                
        _DMX_IDE_SendPacket(0, u4BufStart, u4BufEnd, u4Rp, u4Wp);
    }
#endif
    _FVR_Lock();

    // Update read/write pointer
    FVR_GBL_PID_W(3) = u4BufStart;              // write pointer
    FVR_GBL_PID_W(4) = u4BufStart;              // read pointer

    _rGblStruct.u4Wp = u4BufStart;
    _rGblStruct.u4Rp = u4BufStart;
    _rGblStruct.u4PktRp = u4BufStart;

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
/** _FVR_GetWritePointer
 *  u4Addr: Get buffer address
 *  u4Size: Get buffer size
 *  pu4RetAddr: u4Addr + *pu4RetSize
 *  pu4RetSize: size not wrap around
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetWritePointer(UINT32 u4Addr, UINT32 u4Size, UINT32 *pu4RetAddr, UINT32 *pu4RetSize)
{
    UINT32 u4BufStart, u4BufEnd, u4BufSize, u4Wp, u4Rp;
    UINT32 u4DataSize, u4TotalSize;

    *pu4RetAddr = u4Addr;
    *pu4RetSize = 0;

    if(u4Size == 0)
    {
        return TRUE;
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
        LOG(3, "_FVR_GetWritePointer, invalid addr! Rp:0x%x, Wp:0x%x\n", u4Rp, u4Wp);
        LOG(3, "   DataAddr:0x%x, DataSize:0x%x, TotalSize:0x%x\n", u4Addr, u4DataSize, u4TotalSize);
        return FALSE;
    }

    if(u4Size > u4DataSize)
    {
        LOG(3, "_FVR_GetWritePointer: not enough size!\n");
        return FALSE;
    }

    // Copy data
    if (u4Rp > u4Wp)
    {
        u4Size = MIN(u4BufEnd - u4Rp, u4Size);
        u4Rp += u4Size;
        if (u4Rp >= u4BufEnd)
        {
            u4Rp = u4BufStart;
        }
    }
    else if (u4Rp < u4Wp)
    {
        u4Size = MIN(u4Wp - u4Rp, u4Size);
        u4Rp += u4Size;
    }
    else
    {
        ASSERT(0);
    }

    *pu4RetSize = u4Size;
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
    UINT32 u4BufStart, u4BufEnd, u4NewRp;

    u4BufStart = _rGblStruct.u4BufStart;
    u4BufEnd = _rGblStruct.u4BufEnd + 1;

    u4NewRp = u4Addr + u4Size;
    if(u4NewRp >= u4BufEnd)    
    {
        u4NewRp -= u4BufStart;
    }

    _rGblStruct.u4Rp = u4NewRp;

    _FVR_Lock();
    FVR_GBL_PID_W(4) = u4NewRp;              // read pointer
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
    // Free buffer
    if (_rGblStruct.fgAllocateBuffer)
    {
        if (!BSP_FreeAlignedDmaMemory((void*)_rGblStruct.u4BufStart))
        {
            LOG(3, "Fail to free PID buffer!, addr: 0x%08x\n", _rGblStruct.u4BufStart);
            return FALSE;
        }
    }

    _rGblStruct.u4BufStart = 0x0;
    _rGblStruct.u4BufEnd = 0x0;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetDataSize
 *  @retval 
 */
//-----------------------------------------------------------------------------
UINT32 _FVR_GetDataSize()
{
    UINT32 u4Rp, u4Wp, u4BufSize, u4DataSize;

    u4Rp = _rGblStruct.u4Rp;
    u4Wp = _rGblStruct.u4Wp;
    u4BufSize = _rGblStruct.u4BufSize;

    u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);

    return u4DataSize;
}


//-----------------------------------------------------------------------------
/** _FVR_QueryStatus
 *  @retval 
 */
//-----------------------------------------------------------------------------
void _FVR_QueryStatus(FVR_QUERY_INFO_T *prInfo)
{
    // Ring bufer range
    prInfo->u4BufStart = FVR_GBL_PID_W(0);
    prInfo->u4BufEnd = FVR_GBL_PID_W(1);
    prInfo->u4Wp = FVR_GBL_PID_W(3);
    prInfo->u4Rp = FVR_GBL_PID_W(4);
    
    prInfo->u4PktCount = FVR_GBL_PID_W(6);
}


//-----------------------------------------------------------------------------
/** _FVR_SetByPass
 *  @retval 
 */
//-----------------------------------------------------------------------------
void _FVR_SetByPass(BOOL fgEnable, UINT8 u1Pidx, UINT16 u2CtrlBits)
{
    UINT32 u4Reg;

    u4Reg = (fgEnable ? 0x80000000 : 0x0);
    u4Reg |= ((UINT32)u1Pidx << 16);
    u4Reg |= (UINT32)u2CtrlBits & 0x7FFF;

    FVR_WRITE32(FVR_REG_RECORD_BYPASS, u4Reg);
}

