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
 * $RCSfile: fvr_swap.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fvr_swap.c
 *  FVR swap driver - FVR Driver Interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fvr_debug.h"
#include "fvr.h"

#include "x_os.h"
#include "x_assert.h"
#include "x_hal_arm.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define TS_PACKET_SIZE                       188
#define TS_PACKET_PAYLOAD_SIZE               184

#define FVR_SWAP_QUEUE_NAME                  "FVRSWAPQ"
#define FVR_SWAP_QUEUE_SIZE                  32

#define FVR_SWAP_THREAD_NAME                 "FVRSWAPT"
#define FVR_SWAP_THREAD_STACK_SIZE           1024
#define FVR_SWAP_THREAD_PRIORITY             60

#define FVR_SWAP_PRIORITY_HIGH			     0
#define FVR_SWAP_PRIORITY_NORMAL             128
#define FVR_SWAP_PRIORITY_LOW                255

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
    FVR_SWAPCMD_SWAP,
    FVR_SWAPCMD_SETPATTERN,
    FVR_SWAPCMD_SETPATTERN_NULL,
    FVR_SWAPCMD_FREEPATTERN,
    FVR_SWAPCMD_EXCHANGE_KEY,
    FVR_SWAPCMD_NONE
} FVR_SWAPCMD_T;

typedef struct
{
    FVR_SWAPCMD_T eSwapCmd;
    UINT8 u1Pidx;
    UINT32 u4Addr;
    UINT16 u2Len;
} FVR_SWAP_MSG_T;

typedef struct
{
    BOOL fgValid;
    UINT32 u4Addr;

    UINT16 u2TSIndex;
    UINT16 u2TSCount;
    UINT8 u1Continuity;
} FVR_SWAP_PATTERN_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T _hFVRSwapQueue = NULL_HANDLE;

static HANDLE_T _hFVRSwapThread = NULL_HANDLE;

static FVR_SWAP_PATTERN_T _arSwapPattern[FVR_NUM_PID_INDEX];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _FVRSwapPacket
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _FVRFormatPacket(UINT8 u1Pidx, UINT16 u2PidNum, UINT16 u2Size,
                             UINT32 u4SrcAddr)
{
    UINT32 u4NewSize, u4TSAddr, u4UpdateSize;
    UINT16 i, u2TSCount;
    UINT8 *pu1Src, *pu1Dst;

    //
    // Caculate TS count and size
    //
    u4NewSize = (UINT32)u2Size;
    u2TSCount = 0;
    if(u4NewSize < (TS_PACKET_PAYLOAD_SIZE - 1))
    {
        u4NewSize = 0;
    }
    else
    {
        u4NewSize -= (TS_PACKET_PAYLOAD_SIZE - 1);      // pointer_field
    }

    u2TSCount = 1;
    if(u4NewSize > 0)
    {
        u2TSCount += (UINT16)
                ((u4NewSize + (TS_PACKET_PAYLOAD_SIZE - 1)) / TS_PACKET_PAYLOAD_SIZE);
    }

    u4TSAddr = (UINT32)x_mem_alloc((SIZE_T)(u2TSCount * TS_PACKET_SIZE));
    if(u4TSAddr == 0)
    {
        LOG(3, "Allocate buffer fail\n");
        return FALSE;
    }

    //
    // Fill pattern
    //
    u4NewSize = (UINT32)u2Size;
    pu1Src = (UINT8*)u4SrcAddr;
    pu1Dst = (UINT8*)u4TSAddr;

    pu1Dst[0] = 0x47;
    pu1Dst[1] = (UINT8)(0x40 | (UINT8)((u2PidNum >> 8) & 0x1F));
    pu1Dst[2] = (UINT8)(u2PidNum & 0xFF);
    pu1Dst[3] = 0x10;          // Continuity counter update later
    pu1Dst[4] = 0;          // pointer_field

    u4UpdateSize = (UINT32)MIN((TS_PACKET_PAYLOAD_SIZE - 1), u4NewSize);
    x_memset((VOID*)(pu1Dst + 5), 0xFF, (TS_PACKET_PAYLOAD_SIZE - 1));
    x_memcpy((VOID*)(pu1Dst + 5), (VOID*)pu1Src, u4UpdateSize);

    pu1Src += u4UpdateSize;
    pu1Dst += (u4UpdateSize + 5);
    u4NewSize -= u4UpdateSize;

    for(i=1; i<u2TSCount; i++)
    {
        pu1Dst[0] = 0x47;
        pu1Dst[1] = (UINT8)((u2PidNum >> 8) & 0x1F);
        pu1Dst[2] = (UINT8)(u2PidNum & 0xFF);
        pu1Dst[3] = 0x10;       // No adaptation_field, payload only

        u4UpdateSize = (UINT32)MIN(TS_PACKET_PAYLOAD_SIZE, u4NewSize);
        x_memset((VOID*)(pu1Dst + 4), 0xFF, TS_PACKET_PAYLOAD_SIZE);
        x_memcpy((VOID*)(pu1Dst + 4), (VOID*)pu1Src, u4UpdateSize);

        pu1Src += u4UpdateSize;
        pu1Dst += (u4UpdateSize + 4);
        u4NewSize -= u4UpdateSize;
    }

    _arSwapPattern[u1Pidx].fgValid = TRUE;
    _arSwapPattern[u1Pidx].u4Addr = u4TSAddr;
    _arSwapPattern[u1Pidx].u2TSCount = u2TSCount;
    _arSwapPattern[u1Pidx].u2TSIndex = 0;
    _arSwapPattern[u1Pidx].u1Continuity = 0;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVRSwapPacket
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _FVRSwapPacket(UINT8 u1Pidx, UINT32 u4Addr)
{
    FVR_GBL_T rGBL;
    FVR_PID_STRUCT_T *prStruct;
    UINT32 u4BufStart, u4BufEnd, u4PatAddr, u4ConAddr;
    UINT32 u4CpLen1, u4CpLen2;
    UINT16 u2TSCount, u2TSIndex;
    UINT8 u1Continuity, u1Tmp;

    if(u1Pidx >= FVR_NUM_PID_INDEX)
    {
        LOG(3, "Pix %d error\n", u1Pidx);
        return FALSE;
    }

    if(!_arSwapPattern[u1Pidx].fgValid)
    {
        LOG(5, "%s,%d, No valid swap pattern\n", __FUNCTION__, __LINE__);
        return TRUE;
    }

    prStruct = _FVR_GetPidStruct(u1Pidx);
    if(!prStruct->fgEnable)
    {
        LOG(5, "%s,%d, pid %d, disable\n", __FUNCTION__, __LINE__, u1Pidx);
        return TRUE;
    }

    if(!_FVR_GetGBL(0, FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return FALSE;
    }

    u4BufStart = rGBL.u4BufStart;
    u4BufEnd = u4BufStart + rGBL.u4BufSize;

    if((u4Addr < u4BufStart) || (u4Addr >= u4BufEnd))
    {
        LOG(3, "Addr: 0x%x out of range\n", u4Addr);
        return FALSE;
    }

    if((u4Addr + TS_PACKET_SIZE) >= u4BufEnd)
    {
        u4CpLen1 = u4BufEnd - u4Addr;
        u4CpLen2 = TS_PACKET_SIZE - u4CpLen1;
    }
    else
    {
        u4CpLen1 = TS_PACKET_SIZE;
        u4CpLen2 = 0;
    }

    u1Continuity = _arSwapPattern[u1Pidx].u1Continuity;
    u2TSCount = _arSwapPattern[u1Pidx].u2TSCount;
    u2TSIndex = _arSwapPattern[u1Pidx].u2TSIndex;
    u4PatAddr = _arSwapPattern[u1Pidx].u4Addr + (TS_PACKET_SIZE * u2TSIndex);

    ASSERT(u2TSIndex < u2TSCount);

    x_memcpy((VOID*)u4Addr, (VOID*)u4PatAddr, u4CpLen1);
    if(u4CpLen2 > 0)
    {
        x_memcpy((VOID*)u4BufStart, (VOID*)(u4PatAddr + u4CpLen1), u4CpLen2);
    }

    u4ConAddr = u4Addr + 3;
    if(u4ConAddr >= u4BufEnd)
    {
        u4ConAddr -= (u4BufEnd - u4BufStart);
    }

    u1Tmp = *(UINT8*)u4ConAddr;
    u1Tmp &= 0xF0;
    u1Tmp |= u1Continuity;
    *(UINT8*)u4ConAddr = u1Tmp;

    u1Continuity++;
    u1Continuity %= 16;
    _arSwapPattern[u1Pidx].u1Continuity = u1Continuity;

    u2TSIndex++;
    u2TSIndex %= u2TSCount;
    _arSwapPattern[u1Pidx].u2TSIndex = u2TSIndex;

    HalFlushInvalidateDCache();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVRFreeSwapPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _FVRFreeSwapPattern(UINT8 u1Pidx)
{
    if(u1Pidx >= FVR_NUM_PID_INDEX)
    {
        return FALSE;
    }

    if(!_arSwapPattern[u1Pidx].fgValid)
    {
        LOG(3, "%d pix Pattern invalid\n", u1Pidx);
        return TRUE;
    }

    x_mem_free((VOID*)_arSwapPattern[u1Pidx].u4Addr);
    x_memset((VOID*)&_arSwapPattern[u1Pidx], 0, sizeof(FVR_SWAP_PATTERN_T));

    return TRUE;
}



//-----------------------------------------------------------------------------
/** _FVRSetSwapPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _FVRSetSwapPattern(UINT8 u1Pidx, UINT8* pu1Pattern, UINT16 u2Len)
{
    FVR_PID_STRUCT_T* prPid;

    if(u1Pidx >= FVR_NUM_PID_INDEX)
    {
        return FALSE;
    }

    if(pu1Pattern == NULL)
    {
        return FALSE;
    }

    prPid = _FVR_GetPidStruct(u1Pidx);

    if(_arSwapPattern[u1Pidx].fgValid)
    {
        if(!_FVRFreeSwapPattern(u1Pidx))
        {
            return FALSE;
        }
    }

    if(!_FVRFormatPacket(u1Pidx, prPid->u2Pid, u2Len, (UINT32)pu1Pattern))
    {
        LOG(1, "Format TS packets error\n");
        return FALSE;
    }

	return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVRSetSwapNullPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _FVRSetSwapNullPattern(UINT8 u1Pidx)
{
    UINT32 u4TSAddr;
    UINT8 *pu1Tmp;

    if(u1Pidx >= FVR_NUM_PID_INDEX)
    {
        return FALSE;
    }

    if(_arSwapPattern[u1Pidx].fgValid)
    {
        if(!_FVRFreeSwapPattern(u1Pidx))
        {
            return FALSE;
        }
    }

    u4TSAddr = (UINT32)x_mem_alloc((SIZE_T)(TS_PACKET_SIZE));
    pu1Tmp = (UINT8*)u4TSAddr;
    if(pu1Tmp == NULL)
    {
        return FALSE;
    }
    x_memset((void*)u4TSAddr, 0xFF, TS_PACKET_SIZE);    
    pu1Tmp[0] = 0x47;
    pu1Tmp[1] = 0x1F;
    pu1Tmp[2] = 0xFF;
    pu1Tmp[3] = 0x10;

    _arSwapPattern[u1Pidx].fgValid = TRUE;
    _arSwapPattern[u1Pidx].u4Addr = u4TSAddr;
    _arSwapPattern[u1Pidx].u2TSCount = 1;
    _arSwapPattern[u1Pidx].u2TSIndex = 0;
    _arSwapPattern[u1Pidx].u1Continuity = 0;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVRSwapThread
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _FVRSwapThread(VOID* pvArg)
{
    static FVR_SWAP_MSG_T rSwapMsg;
    INT32 i4Ret;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex;
    BOOL fgRet;

    UNUSED(pvArg);

    while (1)
    {
        zMsgSize = sizeof (rSwapMsg);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rSwapMsg, &zMsgSize,
                        &_hFVRSwapQueue, 1, X_MSGQ_OPTION_WAIT);

        if (i4Ret != OSR_OK)
        {
            LOG(1, "Error on receiving swap message!\n");
            continue;
        }

        _FVR_ApiLock();

        ASSERT(zMsgSize == sizeof (rSwapMsg));
        ASSERT(u2QueueIndex == 0);

        fgRet = TRUE;
        switch(rSwapMsg.eSwapCmd)
        {
        case FVR_SWAPCMD_SWAP:
            fgRet = _FVRSwapPacket(rSwapMsg.u1Pidx, rSwapMsg.u4Addr);
            break;

        case FVR_SWAPCMD_SETPATTERN:
            fgRet = _FVRSetSwapPattern(rSwapMsg.u1Pidx, (UINT8*)rSwapMsg.u4Addr, rSwapMsg.u2Len);
            break;

        case FVR_SWAPCMD_SETPATTERN_NULL:
            fgRet = _FVRSetSwapNullPattern(rSwapMsg.u1Pidx);
            break;

        case FVR_SWAPCMD_FREEPATTERN:
            fgRet = _FVRFreeSwapPattern(rSwapMsg.u1Pidx);
            break;

        case FVR_SWAPCMD_EXCHANGE_KEY:
            fgRet = _FVR_TogglePidKey();
            break;

        default:
            break;
        }

        if(!fgRet)
        {
            LOG(5, "%s,%d, do swap fail\n", __FUNCTION__, __LINE__);
        }

        _FVR_ApiUnlock();
    }
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _FVR_DoSwap
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_DoSwap(UINT8 u1Pidx, UINT32 u4Addr)
{
    FVR_SWAP_MSG_T rSwapMsg;

    rSwapMsg.eSwapCmd = FVR_SWAPCMD_SWAP;
    rSwapMsg.u1Pidx = u1Pidx;
    rSwapMsg.u4Addr = u4Addr;
    if (x_msg_q_send(_hFVRSwapQueue, (VOID*)&rSwapMsg, sizeof (FVR_SWAP_MSG_T),
                        FVR_SWAP_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(3, "FVR swap queue full!\n");
        return FALSE;
    }

	return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SwapSetPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SwapSetPattern(UINT8 u1Pidx, UINT8* pu1Pattern, UINT16 u2Len)
{
    FVR_SWAP_MSG_T rSwapMsg;

    rSwapMsg.eSwapCmd = FVR_SWAPCMD_SETPATTERN;
    rSwapMsg.u1Pidx = u1Pidx;
    rSwapMsg.u4Addr = (UINT32)pu1Pattern;
    rSwapMsg.u2Len = u2Len;

    if (x_msg_q_send(_hFVRSwapQueue, (VOID*)&rSwapMsg, sizeof (FVR_SWAP_MSG_T),
                        FVR_SWAP_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(3, "FVR swap queue full!\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SwapSetNullPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SwapSetNullPattern(UINT8 u1Pidx)
{
    FVR_SWAP_MSG_T rSwapMsg;

    rSwapMsg.eSwapCmd = FVR_SWAPCMD_SETPATTERN_NULL;
    rSwapMsg.u1Pidx = u1Pidx;

    if (x_msg_q_send(_hFVRSwapQueue, (VOID*)&rSwapMsg, sizeof (FVR_SWAP_MSG_T),
                        FVR_SWAP_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(3, "FVR swap queue full!\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SwapFreePattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SwapFreePattern(UINT8 u1Pidx)
{
    FVR_SWAP_MSG_T rSwapMsg;

    rSwapMsg.eSwapCmd = FVR_SWAPCMD_FREEPATTERN;
    rSwapMsg.u1Pidx = u1Pidx;

    if (x_msg_q_send(_hFVRSwapQueue, (VOID*)&rSwapMsg, sizeof (FVR_SWAP_MSG_T),
                        FVR_SWAP_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(3, "FVR swap queue full!\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_DoExchangeKey
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_DoExchangeKey(UINT8 u1Idx)
{
    FVR_SWAP_MSG_T rSwapMsg;

    if(!_FVR_GetIsToggleKey(u1Idx))
    {
        return TRUE;
    }

    rSwapMsg.eSwapCmd = FVR_SWAPCMD_EXCHANGE_KEY;
    rSwapMsg.u1Pidx = 0;
    rSwapMsg.u4Addr = 0;
    if (x_msg_q_send(_hFVRSwapQueue, (VOID*)&rSwapMsg, sizeof (FVR_SWAP_MSG_T),
                        FVR_SWAP_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(3, "FVR swap queue full!\n");
        return FALSE;
    }

	return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SwapInit
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SwapInit(void)
{
    static BOOL _fgInit = FALSE;
    UINT32 i;

    if(!_fgInit)
    {
        // Create swap queues
        if (x_msg_q_create(&_hFVRSwapQueue, FVR_SWAP_QUEUE_NAME,
            sizeof (FVR_SWAP_MSG_T), FVR_SWAP_QUEUE_SIZE) != OSR_OK)
        {
            LOG(1, "Fail to create swap queue!\n");
            return FALSE;
        }

        // Create PSI thread
        if (x_thread_create(&_hFVRSwapThread, FVR_SWAP_THREAD_NAME,
            FVR_SWAP_THREAD_STACK_SIZE, FVR_SWAP_THREAD_PRIORITY, _FVRSwapThread, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create swap thread!\n");
            return FALSE;
        }

        _fgInit = TRUE;
    }

    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        x_memset((VOID*)&_arSwapPattern[i], 0, sizeof(FVR_SWAP_PATTERN_T));
    }

    return TRUE;
}

