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
 * $RCSfile: fvr_isr.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_fvr_isr.c
 *  Demux driver - FVR Interrupt Handler
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fvr.h"
#include "fvr_debug.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_hal_arm.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
//#define FVR_SWAP_TEST

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
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT32 _au4FvrOverflowCount[FVR_GBL_COUNT];
static UINT32 _au4FvrBufPeakSize[FVR_GBL_COUNT];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifdef FVR_SWAP_TEST
//-----------------------------------------------------------------------------
static void _FVRTestTimeSwap(UINT32 u4StartAddr)
{
    FVR_GBL_T rGBL;
    UINT32 i, j, u4BufEnd, u4Len1, u4Len2, u4Addr1, u4Addr2;
    UINT8 *pu1Tmp;

    VERIFY(_FVR_GetGBL(0, FVR_GBL_FLAG_BUFFER, &rGBL));

    u4BufEnd = rGBL.u4BufStart + rGBL.u4BufSize;

    u4Addr1 = u4StartAddr;
    u4Len1 = 188;
    u4Len2 = 0;
    if((u4StartAddr + 188) >= u4BufEnd)
    {
        u4Addr2 = rGBL.u4BufStart;
        u4Len1 = u4BufEnd - u4Addr1;
        u4Len2 = 188 - u4Len1;        
    }

    if(u4Len1 > 0)
    {
        pu1Tmp = (UINT8*)u4Addr1;
        for(i=0; i<u4Len1; i++)
        {
            pu1Tmp[i] = i;
        }    
    }

    if(u4Len2 > 0)
    {
        pu1Tmp = (UINT8*)u4Addr2;
        for(i=0,j=u4Len1; i<u4Len2; i++,j++)
        {
            pu1Tmp[i] = j;
        }
    }
    
}
#endif


//-----------------------------------------------------------------------------
/** _FVRPacketCountInt
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRPacketCountInt(UINT8 u1Pidx, UINT32 u4PacketAddr)
{
    FVR_GBL_STRUCT_T *prGBL;
    UINT32 u4BufStart, u4BufEnd, u4PktRp, u4Wp, u4DataLen, u4PktCount;
    UINT32 u4VirPacketAddr;
    FVR_PID_STRUCT_T* prPid;
    UINT8 u1Idx;
    
    prPid = _FVR_GetPidStruct(u1Pidx);
    u1Idx = prPid->u1TsIndex;
    if(u1Idx >= 2)
    {
        return;
    }
    
    ASSERT(((FVR_GBL_ARY_W(u1Idx, 5) >> 16) & 0xFFFF) != 0);

    prGBL = _FVR_GetGBLStruct(u1Idx);

    u4VirPacketAddr = VIRTUAL(u4PacketAddr);

    u4BufStart = prGBL->u4BufStart;
    u4BufEnd = prGBL->u4BufEnd + 1;
    u4PktRp = prGBL->u4PktRp;
    u4Wp = u4VirPacketAddr;

    UNUSED(u4BufStart);
    UNUSED(u4BufEnd);
    ASSERT((u4VirPacketAddr >= u4BufStart) && (u4VirPacketAddr < u4BufEnd));
    prGBL->u4Wp = u4VirPacketAddr;

    // Calculate length
    u4DataLen = DATASIZE(u4PktRp, u4Wp, (prGBL->u4BufSize));

    u4PktCount = FVR_GBL_ARY_W(u1Idx, 6);

    if(prGBL->pfnNotifyFunc != NULL)
    {
        FVR_GBL_PKTNOTIFY_INFO_T rPktInfo;
        rPktInfo.u4Addr = u4PktRp;
        rPktInfo.u4Size = u4DataLen;
        rPktInfo.u4PktCount = u4PktCount;
        prGBL->pfnNotifyFunc(FVR_GBL_NFY_PKTCOUNT, (UINT32)&rPktInfo, prGBL->pvNotifyTag);
        prGBL->u4PktRp = u4VirPacketAddr;
    }

    // Swap key
    if(!_FVR_DoExchangeKey(u1Idx))
    {
        LOG(5, "_FVR_DoExchangeKey fail\n");
    }
}


//-----------------------------------------------------------------------------
/** _FVRPacketSwapInt
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRPacketSwapInt(UINT8 u1Pidx, UINT32 u4StartAddr)
{  
    FVR_GBL_T rGBL;    
    FVR_PID_STRUCT_T* prPid;
    UINT32 u4VirAddr;
    UINT8 u1Idx;

    prPid = _FVR_GetPidStruct(u1Pidx);
    u1Idx = prPid->u1TsIndex;
    if(u1Idx >= 2)
    {
        return;
    }

    if(!_FVR_GetGBL(u1Idx, FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return;
    }

    u4VirAddr = VIRTUAL(u4StartAddr);
    if((u4VirAddr < rGBL.u4BufStart) || 
        (u4VirAddr >= (rGBL.u4BufStart + rGBL.u4BufSize)))
    {
        LOG(1, "Swap address out of range, 0x%x, 0x%x, 0x%x\n", u4VirAddr,
                rGBL.u4BufStart, rGBL.u4BufStart + rGBL.u4BufSize);
        ASSERT(0);
    }

    if(!prPid->fgPktSwap)
    {
        return;
    }
    
    u4VirAddr -= 188;
    if(u4VirAddr < rGBL.u4BufStart)
    {
        u4VirAddr += rGBL.u4BufSize;
    }
#ifdef FVR_SWAP_TEST
    _FVRTestTimeSwap(u4VirAddr);
#else
    if(!_FVR_DoSwap(u1Pidx, u4VirAddr))
    {
        LOG(3, "Do swap fail\n");
    }
#endif
}


//-----------------------------------------------------------------------------
/** _FVRPicSearchInt
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRPicSearchInt(UINT8 u1Pidx, UINT8 u1Pattern, UINT32 u4PktCount)
{

    FVR_PID_STRUCT_T* prPid;
//    UINT8 u1RetType;
//    DMX_VIDEO_TYPE_T eVideoType = DMX_VIDEO_NONE;

    prPid = _FVR_GetPidStruct(u1Pidx);
    ASSERT(prPid->fgPicSearch);
/*
    // Use default
    eVideoType = _DMX_GeteVideoType(0);
    u1RetType = FVR_PIC_UNKNOWN;

    if(eVideoType == DMX_VIDEO_MPEG)
    {
        switch(u1Pattern)
        {
        case 0:
            u1RetType = FVR_PIC_I_TYPE;
            break;
        case 1:
            u1RetType = FVR_PIC_P_TYPE;
            break;
        case 2:
            u1RetType = FVR_PIC_B_TYPE;
            break;
        default:            
            break;
        }
    }
    else if(eVideoType == DMX_VIDEO_H264)
    {
        switch(u1Pattern)
        {
        case 0:
            u1RetType = FVR_PIC_H264_IDR_TYPE;
            break;
        case 1:
            u1RetType = FVR_PIC_H264_NIDR_TYPE;
            break;
        default:            
            break;
        }
    }

    if((u1RetType & prPid->u1PicSearchMask) != 0)
    {    
        if(prPid->pfnPicSearchNotify != NULL)
        {
            prPid->pfnPicSearchNotify(u1Pidx, u1RetType, u4PktCount,
                                    prPid->pvSwapNotifyTag);
        }
    }
*/

        if(prPid->pfnPicSearchNotify != NULL)
        {
            prPid->pfnPicSearchNotify(u1Pidx, u1Pattern, u4PktCount,
                                    prPid->pvSwapNotifyTag);
        }

}


//-----------------------------------------------------------------------------
/** _FVRTimerPktInt
 */
//-----------------------------------------------------------------------------
static void _FVRHandleTimer(UINT8 u1Idx, UINT8 u1SerialNum, UINT32 u4EndAddr)
{
    FVR_GBL_STRUCT_T *prGBL;
    UINT32 u4BufStart, u4BufEnd, u4PktRp, u4Wp, u4DataLen;
    UINT32 u4PeakSize, u4VirEndAddr, u4PktCount, u4DmemEndAddr;
    UINT8 u1DmemSerial;

    u4PktCount = 0;
    u1DmemSerial = 0;
    u4DmemEndAddr = 0;

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return;
    }

    _FVR_Lock();
    u1DmemSerial = (UINT8)(*((volatile UINT32*)FVR_GBL_SERIALNUM_ADDR));
    _FVR_Unlock();
    if(u1SerialNum != u1DmemSerial)
    {
        LOG(3, "%s serial not match: %d, %d\n", __FUNCTION__, u1SerialNum, u1DmemSerial);
        return;
    }

    if(u1Idx == 0)
    {
        _FVR_Lock();
        u4PktCount = FVR_GBL_TIMEINFO_S(8);
        u4DmemEndAddr = FVR_GBL_TIMEINFO_S(7);
        _FVR_Unlock();

        if(u4EndAddr != u4DmemEndAddr)
        {
            // Test
            ASSERT(0);
        }
    }
    else if(u1Idx == 1)
    {
        _FVR_Lock();
        u4PktCount = FVR_GBL_TIMEINFO_S(10);
        u4DmemEndAddr = FVR_GBL_TIMEINFO_S(9);
        _FVR_Unlock();
    }
    else if(u1Idx == 3)
    {
        _FVR_Lock();
        u4PktCount = FVR_GBL_TIMEINFO_S(12);
        u4DmemEndAddr = FVR_GBL_TIMEINFO_S(11);
        _FVR_Unlock();
    }
    else
    {
        return;
    }

    prGBL = _FVR_GetGBLStruct(u1Idx);

    u4VirEndAddr = VIRTUAL(u4DmemEndAddr);

    u4BufStart = prGBL->u4BufStart;
    u4BufEnd = prGBL->u4BufEnd + 1;
    u4PktRp = prGBL->u4PktRp;
    u4Wp = u4VirEndAddr;

    if((u4VirEndAddr < u4BufStart) || (u4VirEndAddr >= u4BufEnd))
    {
        LOG(1, "Wrong buf addr\n");
        ASSERT(0);
    }

    // Calculate peak size
    u4PeakSize = DATASIZE(prGBL->u4Rp, prGBL->u4Wp, prGBL->u4BufSize);
    _FVR_Lock();
    if(u4PeakSize > _au4FvrBufPeakSize[u1Idx])
    {
        _au4FvrBufPeakSize[u1Idx] = u4PeakSize;
    }
    _FVR_Unlock();

    prGBL->u4Wp = u4VirEndAddr;

    // Calculate length
    u4DataLen = DATASIZE(u4PktRp, u4Wp, (prGBL->u4BufSize));

    if(prGBL->pfnNotifyFunc != NULL)
    {
        FVR_GBL_TIMENOTIFY_INFO_T rTimeInfo;
        rTimeInfo.u4Addr = u4PktRp;
        rTimeInfo.u4Size = u4DataLen;
        rTimeInfo.u4PktCount = u4PktCount;
        prGBL->pfnNotifyFunc(FVR_GBL_NFY_TIME, (UINT32)&rTimeInfo, prGBL->pvNotifyTag);        
    }

    prGBL->u4PktRp = u4VirEndAddr;

    // Swap key
    if(!_FVR_DoExchangeKey(u1Idx))
    {
        LOG(5, "_FVR_DoExchangeKey fail\n");
    }
}

//-----------------------------------------------------------------------------
/** _FVRTimerPktInt
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRTimerPktInt(UINT8 u1SerialNum, UINT32 u4EndAddr)
{
    if(_FVR_IsRecording(0))
    {
        _FVRHandleTimer(0, u1SerialNum, u4EndAddr);
    }

    if(_FVR_IsRecording(1))
    {
        _FVRHandleTimer(1, u1SerialNum, u4EndAddr);
    }

    if(_FVR_IsRecording(3))
    {
        _FVRHandleTimer(3, u1SerialNum, u4EndAddr);
    }    
}


//-----------------------------------------------------------------------------
/** _FVRScrambleChange
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRScrambleChange(UINT8 u1Pidx, BOOL fgScrambled)
{
    FVR_PID_STRUCT_T* prPid;
    FVR_SCRAMBLE_STATE_T eState;

    prPid = _FVR_GetPidStruct(u1Pidx);

    if (fgScrambled)
    {
        eState = FVR_SCRAMBLE_STATE_SCRAMBLED;
    }
    else
    {
        eState = FVR_SCRAMBLE_STATE_CLEAR;
    }

    LOG(3, "%s, Pidx %u scrambling state: %s\n", __FUNCTION__, u1Pidx, fgScrambled ? "SCRAMBLED" : "CLEAR");

    if(prPid->fgDetectScramble && (prPid->pfnDetectScrambleNotify != NULL))
    {
        prPid->pfnDetectScrambleNotify(u1Pidx, eState);
    }

}



//-----------------------------------------------------------------------------
/** _FVR_uPErrorInt
 *  Error handler of uP, current only FIFO full
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_uPErrorInt(UINT8 u1Idx)
{
    FVR_GBL_STRUCT_T* prGBLStruct;    

    if(u1Idx >= 2)
    {
        return;
    }

    _FVR_Lock();
    _au4FvrOverflowCount[u1Idx]++;
    _FVR_Unlock();

    // Currently, error intrrupt only contain overflow
    prGBLStruct = _FVR_GetGBLStruct(u1Idx);

    if(prGBLStruct->pfnNotifyFunc != NULL)
    {
        prGBLStruct->pfnNotifyFunc(FVR_GBL_NFY_OVERFLOW, 0, 
                                   prGBLStruct->pvNotifyTag);
    }

    LOG(3, "FVR uP error happen, %d\n", u1Idx);
}


//-----------------------------------------------------------------------------
/** _FVR_DispatchuPInterrupt
 *  Dispatch uP interrupts
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _FVR_DispatchuPInterrupt(UINT32 u4Status, UINT32 u4Status2)
{
    UINT8 u1Pidx, u1Type, u1Pattern;
    BOOL fgOk = TRUE;
    BOOL fgScrambled;

#if 1
    if(!_FVR_IsRecording(0) && !_FVR_IsRecording(1) && !_FVR_IsRecording(3))
    {
        // Clear interrupt
        return TRUE;
    }
#endif

    u1Pidx = GET_BYTE(u4Status, 2);         // PID index
    u1Type = GET_BYTE(u4Status, 0);
    u1Pattern = GET_BYTE(u4Status, 1);

    if(u1Pidx >= DMX_FVR_START_PID)
    {
        u1Pidx -= DMX_FVR_START_PID;
    }

    switch(u1Type)
    {
    case 64:    // packet count 
        if(u1Pidx >= FVR_NUM_PID_INDEX)
        {
            return FALSE;
        }
        if(u4Status & 0x200)
        {
            _FVRPacketSwapInt(u1Pidx, u4Status2);
        }
    
        if(u4Status & 0x100)
        {
            _FVRPacketCountInt(u1Pidx, u4Status2);
        }
        break;

    case 65:    // I,P,B notification
        if(u1Pidx >= FVR_NUM_PID_INDEX)
        {
            return FALSE;
        }
        _FVRPicSearchInt(u1Pidx, u1Pattern, u4Status2);
        break;

    case 66:    // Timer notification (End Addr + 1)
        _FVRTimerPktInt(u1Pattern, u4Status2);
        break;

    case 68:    // scramble status detect
        if(u1Pidx >= FVR_NUM_PID_INDEX)
        {
            return FALSE;
        }    
        fgScrambled = (BOOL)(u1Pattern == 1);        
        _FVRScrambleChange(u1Pidx, fgScrambled);
        break;

    default:
        fgOk = FALSE;
        break;
    }

    return fgOk;
}


//-----------------------------------------------------------------------------
/** _FVR_SetIsOverflow
 *  @retval 
 */
//-----------------------------------------------------------------------------
void _FVR_SetIsOverflow(UINT8 u1Idx, BOOL fgIsOverflow)
{
    if(u1Idx >= FVR_GBL_COUNT)
    {
        return;
    }
    
    if(fgIsOverflow)
    {
        _FVR_Lock();
        _au4FvrOverflowCount[u1Idx]++;
        _FVR_Unlock();
    }
}


//-----------------------------------------------------------------------------
/** _FVR_QueryStatus
 *  @retval 
 */
//-----------------------------------------------------------------------------
void _FVR_QueryStatus(UINT8 u1Idx, FVR_QUERY_INFO_T *prInfo)
{
    if(prInfo == NULL)
    {
        return;
    }

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return;
    }
    
    // Ring bufer range
    _FVR_Lock();
    prInfo->u4BufStart = VIRTUAL(FVR_GBL_ARY_W(u1Idx, 0));
    prInfo->u4BufEnd = VIRTUAL(FVR_GBL_ARY_W(u1Idx, 1));
    prInfo->u4Wp = VIRTUAL(FVR_GBL_ARY_W(u1Idx, 3));
    prInfo->u4Rp = VIRTUAL(FVR_GBL_ARY_W(u1Idx, 4));
    
    prInfo->u4PktCount = FVR_GBL_ARY_W(u1Idx, 6);
    prInfo->u4OverflowCount = _au4FvrOverflowCount[u1Idx];
    prInfo->u4BufPeakSize = _au4FvrBufPeakSize[u1Idx];
    _FVR_Unlock();
}


//-----------------------------------------------------------------------------
/** _FVR_InitIsr
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL _FVR_InitIsr(void)
{
    UINT8 i;

    for(i=0; i<FVR_GBL_COUNT; i++)
    {
        _au4FvrOverflowCount[i] = 0;
        _au4FvrBufPeakSize[i] = 0;
    }

    return TRUE;
}

