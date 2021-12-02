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
 * $RCSfile: dmx_ddi.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_ddi.c
 *  Demux DDI driver - DDI API
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "swdmx_if.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_pinmux.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_os.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DDI_MAX_PACKET_SIZE         255
#define DDI_PROLOG_PACKETS          2
#define DDI_EPILOG_PACKETS          13
#define DDI_PROLOG_BUF_SIZE         (DDI_MAX_PACKET_SIZE * DDI_PROLOG_PACKETS + (2 * DDI_BUF_ALIGNMENT))
#define DDI_EPILOG_BUF_SIZE         (DDI_MAX_PACKET_SIZE * DDI_EPILOG_PACKETS + (2 * DDI_BUF_ALIGNMENT))
#define DDI_PADDING_BUF_SIZE        (DDI_MAX_PACKET_SIZE + (2 * DDI_BUF_ALIGNMENT))


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT8 *pu1Prolog;
    UINT8 *pu1Epilog;
    UINT8 u1PacketSize;
    UINT8 u1SyncOffset;
    UINT32 u4BufFullGap;    // (_u1PacketSize * 4) + 32
    UINT32 u4PrologSize;    // _u1PacketSize * DDI_PROLOG_PACKETS
    UINT32 u4EpilogSize;    // _u1PacketSize * DDI_EPILOG_PACKETS
} DMX_TS_PTKSIZE_INFO_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DMX_DDI_STRUCT_T _rDmxDDI;
static DMX_DDI_PORT_T _eDDIPortType = DMX_DDI_PORT_NO_FRAMER;
static BOOL _fgDDISingleSent;               // Has single chunk been sent?
static HANDLE_T _hDmxDDISema = NULL_HANDLE;
static BOOL _fgDDIWaitTransfer = FALSE;
static UINT32 _u4Rate1Mb;       // 1Mbit rate: _u1PacketSize * 8 * 27
static UINT8 _u1DmxDDITsIndex = 0;

static DMX_TS_PTKSIZE_INFO_T _arDmxDDITsInfo[DMX_FRAMER_COUNT];
//static UINT8 *_pu1Prolog = NULL;
//static UINT8 *_pu1Epilog = NULL;
//static UINT8 _u1PacketSize = 0;
//static UINT8 _u1SyncOffset = 0;
//static UINT32 _u4BufFullGap;    // (_u1PacketSize * 4) + 32
//static UINT32 _u4PrologSize;    // _u1PacketSize * DDI_PROLOG_PACKETS
//static UINT32 _u4EpilogSize;    // _u1PacketSize * DDI_EPILOG_PACKETS

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// The caller of this function must ensure that prDDI is valid.
//-----------------------------------------------------------------------------
static BOOL _DDI_UsePVR(UINT8 u1TsIdx)
{
    if(u1TsIdx == 2)
    {
        return TRUE;
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// The caller of this function must ensure that prDDI is valid.
//-----------------------------------------------------------------------------
static BOOL _DDI_AllocBuf(const DMX_DDI_T *prDDI)
{
    UINT32 u4BufStart, u4BufEnd, u4PhyBufStart, u4PhyBufEnd;

    ASSERT(prDDI != NULL);

    if((_rDmxDDI.u4BufAddr != 0) || (_rDmxDDI.u4BufSize != 0))
    {
        LOG(3, "Buffer had been allocated.\n");
        return FALSE;
    }

    if(!_DMX_IsAligned(prDDI->u4BufSize, DDI_BUF_ALIGNMENT))
    {
        LOG(3, "The requested DDI buffer size is not aligned.\n");
        return FALSE;
    }

    if (!_DMX_IsAligned(prDDI->u4Threshold, DDI_BUF_ALIGNMENT))
    {
        LOG(3, "The requested threshold size is not aligned.\n");
        return FALSE;
    }

    if(prDDI->fgAllocBuf)
    {
        u4BufStart = (UINT32) BSP_AllocAlignedDmaMemory(prDDI->u4BufSize,
                                                        DDI_BUF_ALIGNMENT);
        if (u4BufStart == 0)
        {
            LOG(3, "%s:%u: Memory allocation failed!\n", __FILE__, __LINE__);
            return FALSE;
        }
  	    HalInvalidateDCacheMultipleLine(VIRTUAL(u4BufStart), prDDI->u4BufSize);
    }
    else
    {
        if(!_DMX_IsAligned(prDDI->u4BufAddr, DDI_BUF_ALIGNMENT))
        {
            LOG(3, "The DDI buffer address is not aligned.\n");
            return FALSE;
        }
        u4BufStart = prDDI->u4BufAddr;
    }

    u4BufEnd      = u4BufStart + prDDI->u4BufSize;
    u4BufStart    = VIRTUAL(u4BufStart);
    u4BufEnd      = VIRTUAL(u4BufEnd);
    u4PhyBufStart = PHYSICAL(u4BufStart);
    u4PhyBufEnd   = PHYSICAL(u4BufEnd);

    _DMX_Lock();
    _rDmxDDI.fgAllocBuf  = prDDI->fgAllocBuf;   // Buffer has been allocated
    _rDmxDDI.u4BufAddr   = u4BufStart;
    _rDmxDDI.u4BufSize   = prDDI->u4BufSize;
    _rDmxDDI.u4Rp        = u4BufStart;
    _rDmxDDI.u4Wp        = u4BufStart;
    _rDmxDDI.u4Threshold = prDDI->u4Threshold;
    DDI_WRITE32(DDI_REG_DMA_BUF_START, u4PhyBufStart);     // Buffer start
    DDI_WRITE32(DDI_REG_DMA_BUF_END, u4PhyBufEnd);         // Buffer end
    DDI_WRITE32(DDI_REG_DMA_RP_INIT, u4PhyBufStart);       // Initial RP
    DDI_WRITE32(DDI_REG_DMA_WP, u4PhyBufStart);            // Write pointer
    DDI_WRITE32(DDI_REG_DMA_AP, u4PhyBufStart);            // Alert pointer
    // The update of RP will be done by hardware when DMA is activiated.
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
// Free the resources managed by software.
static BOOL _DDI_FreeBuf(void)
{
    if(_rDmxDDI.fgAllocBuf)
    {
        VERIFY(BSP_FreeAlignedDmaMemory(_rDmxDDI.u4BufAddr));
    }

    _DMX_Lock();
    _rDmxDDI.fgAllocBuf = FALSE;
    _rDmxDDI.u4BufAddr = 0;
    _rDmxDDI.u4BufSize = 0;
    _rDmxDDI.u4Rp = 0;
    _rDmxDDI.u4Wp = 0;
    _rDmxDDI.u4TransferredSize = 0;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DDI_Reset
*/
// Reset the hardware including the hardware buffer pointers.
//-----------------------------------------------------------------------------
void _DDI_Reset(BOOL fgResetHardware)
{
    UINT32 u4Reg;
    DMX_DDI_PORT_T ePort;

    ePort = _DMX_DDI_GetPort();

    _DMX_Lock();

    if (fgResetHardware)
    {
        // Reset DDI
        // 0x3000F: DDI soft reset, turn on DDI DRAM clock.
        //   0x20F: Release soft reset, turn on DDI DRAM clock.
        DDI_WRITE32(DDI_REG_GLOBAL_CTRL, 0x3000F);
        DDI_WRITE32(DDI_REG_GLOBAL_CTRL, 0x20F);
    }

    DDI_WRITE32(DDI_REG_DCR_INT_MASK, 0x7);     // Disable all DMA interrupts
    DDI_WRITE32(DDI_REG_DCR_INT_CLR, 0x7);      // Clear all DMA interrupts

    u4Reg = DDI_READ32(DDI_REG_DMX_RX_CTRL) & 0x10;
#ifdef CC_DMX_PS_CPU
    u4Reg |= 0x3;                               // set Output Delay Byte to 3
#else
    u4Reg |= 0xF;                               // set Output Delay Byte to 15
#endif  // CC_DMX_PS_CPU
    DDI_WRITE32(DDI_REG_DMX_RX_CTRL, u4Reg);

    DDI_WRITE32(DDI_REG_DMA_CTRL, 4);           // Stop the DMA operation

    DDI_WRITE32(DDI_REG_DMA_BUF_START, 0);      // Start pointer
    DDI_WRITE32(DDI_REG_DMA_BUF_END, 0);        // End pointer
    DDI_WRITE32(DDI_REG_DMA_RP_INIT, 0);        // Initial Read pointer
    DDI_WRITE32(DDI_REG_DMA_WP, 0);             // Write pointer
    DDI_WRITE32(DDI_REG_DMA_AP, 0);             // Alert pointer
    // The update of RP will be done by hardware when DMA is activiated.

    // Reset RP to 0.
    DDI_WRITE32(DDI_REG_DMA_CTRL, 5);           // Start the DMA operation
    DDI_WRITE32(DDI_REG_DMA_CTRL, 4);           // Stop the DMA operation

    // Clear the interrupt generated when RP is reset to 0 above.
    DDI_WRITE32(DDI_REG_DCR_INT_CLR, 0x7);      // Clear all DMA interrupts

    _DMX_Unlock();

    _DMX_DDI_SetPort(ePort, TRUE);
}


//-----------------------------------------------------------------------------
/** _DDI_GetTransferredSize
*/
//-----------------------------------------------------------------------------
static UINT32 _DDI_GetTransferredSize(UINT32 u4NewRp, UINT32 u4OldRp)
{
    UINT32 u4TransferredSize = 0;

    u4NewRp = VIRTUAL(u4NewRp);
    u4OldRp = VIRTUAL(u4OldRp);

    if (u4NewRp >= u4OldRp)
    {
        u4TransferredSize = u4NewRp - u4OldRp;
    }
    else
    {
        u4TransferredSize = (u4NewRp + _rDmxDDI.u4BufSize) - u4OldRp;
    }

    return u4TransferredSize;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_WaitTransfer
*/
//-----------------------------------------------------------------------------
static BOOL _PVR_WaitTransfer(UINT32 u4BufStart, UINT32 u4BufEnd,
                              UINT32 u4SendAddr, UINT32 u4SendSize)
{
    DMX_MM_DATA_T rData;

    x_memset((void*)&rData, 0, sizeof(rData));
    rData.u4BufStart = u4BufStart;
    rData.u4BufEnd = u4BufEnd;
    rData.u4StartAddr = u4SendAddr;
    rData.u4FrameSize = u4SendSize;

    if(!_DMX_PVRPlay_SingleMove(&rData))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DDI_WaitTransfer
 */
//-----------------------------------------------------------------------------
static BOOL _DDI_WaitTransfer(UINT8 u1TsIdx, UINT32 u4BufStart, UINT32 u4BufEnd,
                              UINT32 u4SendAddr, UINT32 u4SendSize)
{
    INT32 i4Ret;

    if(_DDI_UsePVR(u1TsIdx)) // PVR
    {
        return _PVR_WaitTransfer(u4BufStart, u4BufEnd, u4SendAddr, u4SendSize);
    }

    // Make sure the semaphore is locked so that this function waits for DMA completion.
    i4Ret = x_sema_lock(_hDmxDDISema, X_SEMA_OPTION_NOWAIT);
    if ((i4Ret != OSR_OK) && (i4Ret != OSR_WOULD_BLOCK))
    {
        LOG(3, "Cannot get DMX DDI semaphore!\n");
        return FALSE;
    }

    _DMX_Lock();
    _fgDDIWaitTransfer = TRUE;
    _DMX_Unlock();

    // Set DDI
    if(!_DMX_DDI_SingleMove(u4BufStart, u4BufEnd, u4SendAddr, u4SendSize))
    {
        LOG(3, "DDI transaction failed!\n");
        _DMX_Lock();
        _fgDDIWaitTransfer = FALSE;
        _DMX_Unlock();
        return FALSE;
    }

    VERIFY(x_sema_lock(_hDmxDDISema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _DMX_Lock();
    _fgDDIWaitTransfer = FALSE;
    _DMX_Unlock();

    HAL_Delay_us(2);
    _DMX_DDI_HWReset();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DDI_SetupProlog
 *
 *  Set up two TS null packets for Framer's Internal Sync Lock.  For now, only
 *  TS file playback make use of Framer (see _DMX_SelectInputType()).  This
 *  function can be regarded as for DMX_IN_PLAYBACK_TS only.
 */
//-----------------------------------------------------------------------------
static BOOL _DDI_SetupProlog(UINT8 u1TsIdx, UINT32 *pu4BufStart, UINT32 *pu4BufEnd)
{
    UINT32 u4SyncWord, i;

    ASSERT(pu4BufStart != NULL);
    ASSERT(pu4BufEnd   != NULL);

    if(u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }

    // Setup two TS null packets for Internal Sync Lock at the stream head.
    // Null Packet (payload only): 0x471FFF10 + 0xFF...0xFF
    u4SyncWord = 0x10FF1F47;
    *pu4BufStart = ((UINT32)_arDmxDDITsInfo[u1TsIdx].pu1Prolog + DDI_BUF_ALIGNMENT)   & 0xFFFFFFF0;
    *pu4BufEnd   = ((UINT32)_arDmxDDITsInfo[u1TsIdx].pu1Prolog + DDI_PROLOG_BUF_SIZE) & 0xFFFFFFF0;

    x_memset((void*)_arDmxDDITsInfo[u1TsIdx].pu1Prolog, 0xFF, DDI_PROLOG_BUF_SIZE);

    for (i = 0; i < DDI_PROLOG_PACKETS; i++)
    {
        x_memcpy((void*)((*pu4BufStart) +
            (i * _arDmxDDITsInfo[u1TsIdx].u1PacketSize) + _arDmxDDITsInfo[u1TsIdx].u1SyncOffset),
            &u4SyncWord, sizeof(u4SyncWord));
    }

    HalFlushInvalidateDCache();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DDI_SetupEpilog
 *
 *  Set up ten packets for taking up the Demux internal buffer.  After these
 *  ten packets is sent to Demux, each byte of previous user data has been
 *  pushed out of Demux.  Then, the EOS message can be sent to the decoder.
 */
//-----------------------------------------------------------------------------
static BOOL _DDI_SetupEpilog(UINT8 u1TsIdx, UINT8 u1Pidx, UINT32 *pu4BufStart, UINT32 *pu4BufEnd)
{
    PID_STRUCT_T *prPidStruct;
    // adaption field only, and adaption field length: 183 (0xB7)
    UINT32 u4Lead[2], i, u4ThreeBytesPayload;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);
    ASSERT(pu4BufStart != NULL);
    ASSERT(pu4BufEnd   != NULL);

    if(u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    *pu4BufStart = ((UINT32)_arDmxDDITsInfo[u1TsIdx].pu1Epilog + DDI_BUF_ALIGNMENT)   & 0xFFFFFFF0;
    *pu4BufEnd   = ((UINT32)_arDmxDDITsInfo[u1TsIdx].pu1Epilog + DDI_EPILOG_BUF_SIZE) & 0xFFFFFFF0;

    x_memset((void*)_arDmxDDITsInfo[u1TsIdx].pu1Epilog, 0xFF, DDI_EPILOG_BUF_SIZE);

    u4Lead[0] = 0x20000047;
    u4Lead[0] |= ((UINT32)(prPidStruct->u2Pid & 0xFF) << 16);
    u4Lead[0] |= ((UINT32)(prPidStruct->u2Pid & 0x1F00));
    u4Lead[1] = 0xFFFFFFB7;
    for (i = 1; i < DDI_EPILOG_PACKETS; i++)
    {
        x_memcpy((void*)((*pu4BufStart) + (i * _arDmxDDITsInfo[u1TsIdx].u1PacketSize) + _arDmxDDITsInfo[u1TsIdx].u1SyncOffset),
            u4Lead, sizeof(u4Lead));
    }
    // For the first packet: (i == 0)
    u4Lead[0] |= 0x10004000;    // PUSI == 1, for "Complete PES interrupt"
    u4Lead[1] = 0xFFFFFFB4;     // Payload contains 3 bytes: 0x000001
    u4ThreeBytesPayload = 0x010000FF;
    x_memcpy((void*)((*pu4BufStart) + _arDmxDDITsInfo[u1TsIdx].u1SyncOffset), u4Lead, sizeof(u4Lead));
    x_memcpy((void*)((*pu4BufStart) + _arDmxDDITsInfo[u1TsIdx].u1PacketSize - 4),
        (void*)&u4ThreeBytesPayload, sizeof(u4ThreeBytesPayload));

    HalFlushInvalidateDCache();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DDI_SendDummyPackets
 */
//-----------------------------------------------------------------------------
static BOOL _DDI_SendDummyPackets(UINT8 u1TsIdx, UINT8 u1Pidx)
{
    UINT32 u4BufStart = 0, u4BufEnd = 0;
#ifdef CC_DMX_TS130
    PID_STRUCT_T *prPidStruct;
#endif

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    if(u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }

    if((_DMX_GetPidInputType(u1Pidx) != DMX_IN_BROADCAST_TS) &&
        (_DMX_GetPidInputType(u1Pidx) != DMX_IN_PLAYBACK_TS))
    {
        return FALSE;
    }

#ifdef CC_DMX_TS130
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if(prPidStruct != NULL)
    {
        if(prPidStruct->fgTS130)
        {
            return _DMX_TS130_SendDummyPacket(u1TsIdx, u1Pidx);
        }
    }
#endif

    VERIFY(_DDI_SetupEpilog(u1TsIdx, u1Pidx, &u4BufStart, &u4BufEnd));

    if (!_DDI_WaitTransfer(u1TsIdx, u4BufStart, u4BufEnd, u4BufStart,
                                    _arDmxDDITsInfo[u1TsIdx].u4EpilogSize))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DDI_Sync_VideoWp
 */
//-----------------------------------------------------------------------------
BOOL _DDI_Sync_VideoWp(UINT8 u1TsIdx)
{
    PID_STRUCT_T *prPidStruct;
    UINT8 u1Pidx;
    UINT32 u4Wp1, u4Wp2, i;
    UINT8 u1Channel;

    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
    {
        if (_DMX_IsPidEnabled(u1Pidx))
        {
            prPidStruct = _DMX_GetPidStruct(u1Pidx);
            if((prPidStruct->eInputType != DMX_IN_BROADCAST_TS) &&
               (prPidStruct->eInputType != DMX_IN_PLAYBACK_TS))
            {
                continue;
            }

            if(_DMX_GetPidTsIndex(u1Pidx) != u1TsIdx)
            {
                continue;
            }

            if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
            {
                for(i=0; i<100; i++)
                {
                    u4Wp1 = PID_S_W(u1Pidx, 8);
                    x_thread_delay(1);
                    u4Wp2 = PID_S_W(u1Pidx, 8);
                    if(u4Wp1 == u4Wp2)
                    {
                        break;
                    }
                }

                if(i == 100)
                {
                    return TRUE;
                }

                // Update write pointer to decoder
                if (_DMX_IsToDecoder())
                {
                    VERIFY(_DMX_GetVideoChannel(u1Pidx, &u1Channel));
                    VERIFY(_DMX_UpdateVideoWritePointer(u1Pidx, u1Channel, PHYSICAL(u4Wp1)));
                }
            }
        }
    }

    return TRUE;

}



//-----------------------------------------------------------------------------
/** _DDI_SendAllDummyPackets
 */
//-----------------------------------------------------------------------------
BOOL _DDI_SendAllDummyPackets(UINT8 u1TsIdx)
{
    PID_STRUCT_T *prPidStruct;
    UINT8 u1Pidx;

    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
    {
        if (_DMX_IsPidEnabled(u1Pidx))
        {
            prPidStruct = _DMX_GetPidStruct(u1Pidx);
            if((prPidStruct->eInputType != DMX_IN_BROADCAST_TS) &&
               (prPidStruct->eInputType != DMX_IN_PLAYBACK_TS))
            {
                continue;
            }

            if(_DMX_GetPidTsIndex(u1Pidx) != u1TsIdx)
            {
                continue;
            }

            if ((prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO) ||
                (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO) ||
                (prPidStruct->ePidType == DMX_PID_TYPE_PSI))
            {
                if (!_DDI_SendDummyPackets(u1TsIdx, u1Pidx))
                {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DDI_SendTSEOS
 */
//-----------------------------------------------------------------------------
BOOL _DDI_SendTSEOS(UINT8 u1TsIdx)
{
    PID_STRUCT_T *prPidStruct;
    UINT8 u1Pidx;

    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
    {
        if (_DMX_IsPidEnabled(u1Pidx))
        {
            prPidStruct = _DMX_GetPidStruct(u1Pidx);
            if((prPidStruct->eInputType != DMX_IN_BROADCAST_TS) &&
               (prPidStruct->eInputType != DMX_IN_PLAYBACK_TS))
            {
                continue;
            }

            if(_DMX_GetPidTsIndex(u1Pidx) != u1TsIdx)
            {
                continue;
            }

            if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
            {
                DMX_AUDIO_PES_T rPes;
                x_memset((void*)&rPes, 0, sizeof(rPes));
                rPes.fgEOS = TRUE;
                rPes.u1PidIndex = u1Pidx;
                rPes.u1DeviceId = _DMX_GetPidStruct(u1Pidx)->u1DeviceId;
                if (!_DMX_SendAudioPes(&rPes))
                {
                    return FALSE;
                }
            }
            else if(prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
            {
                if (!_DMX_SendEOSMessage(u1Pidx))
                {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DDI_TsResetDmxBuffers
 *
 *  0. Stop DDI (It must be performed in the caller of this function.)
 *  1. Store Valid bit and PID index table
 *  2. Reset DBM
 *  3. Change Valid and Toggle bits
 *  4. Flush PID buffers and change software PID structures; Increment SN.
 *  5. Delay 1 ~ 2 ms
 *  6. Reset uP
 *  7. Flush PID buffers
 *  8. Restore valid bit and PID index table
 */
//-----------------------------------------------------------------------------
BOOL _DDI_TsResetDmxBuffers(UINT8 u1TsIdx)
{
    UINT8 u1Pidx;
    BOOL afgPidEnabled[DMX_NUM_PID_INDEX];
    UINT32 au4PidIndexTable[DMX_NUM_PID_INDEX], au4W0[DMX_NUM_PID_INDEX];
    PID_STRUCT_T *prPidStruct;
    DMX_PID_TYPE_T ePidType;

    x_memset((void*)afgPidEnabled, 0, sizeof(afgPidEnabled));
    x_memset((void*)au4PidIndexTable, 0, sizeof(au4PidIndexTable));
    x_memset((void*)au4W0, 0, sizeof(au4W0));

    if(!_DDI_UsePVR(u1TsIdx)) // if not PVR
    {
        // send dummy packet to flush out the ts packet in uP buffer.
        // This can avoid from wrong PTS value when seek to 0.
        if(!_DDI_SendAllDummyPackets(u1TsIdx))
        {
            return FALSE;
        }
    }

    // Store PID Index Table and DMEM word 0.
    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
    {
        if((u1Pidx != DMX_ALL_INSTINDEX) && (_DMX_GetPidTsIndex(u1Pidx) != u1TsIdx))
        {
            continue;
        }

        if (_DMX_IsPidEnabled(u1Pidx))
        {
            BOOL fgEnable;
            UINT32 u4PidIndexTable, u4W0;

            _DMX_Lock();
            u4PidIndexTable = PID_INDEX_TABLE(u1Pidx);
            u4W0 = PID_S_W(u1Pidx, 0);
            prPidStruct = _DMX_GetPidStruct(u1Pidx);
            fgEnable = prPidStruct->fgEnable;
            _DMX_Unlock();

            if (fgEnable)
            {
                au4PidIndexTable[u1Pidx] = u4PidIndexTable;
                au4W0[u1Pidx] = u4W0;
                afgPidEnabled[u1Pidx] = TRUE;
                continue;
            }
        }
        afgPidEnabled[u1Pidx] = FALSE;
    }

    // Change Valid and Toggle bits.
    // Clear the "to-uP" flag, and keep the "to-PVR" flag.
    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
    {
        UINT32 u4ActiveFlag;

        if((u1Pidx != DMX_ALL_INSTINDEX) && (_DMX_GetPidTsIndex(u1Pidx) != u1TsIdx))
        {
            continue;
        }

        if (!(afgPidEnabled[u1Pidx]))
        {
            continue;
        }

        _DMX_Lock();
        prPidStruct = _DMX_GetPidStruct(u1Pidx);
        prPidStruct->u4NextPic = 0;
        prPidStruct->u1SerialNumber++;
        if (prPidStruct->u1SerialNumber > DMX_MAX_SERIAL_NUM)
        {
            prPidStruct->u1SerialNumber = 0;
        }

        // Disable PID index table again.
        PID_INDEX_TABLE(u1Pidx) &= 0x7FFFFFDF;  // Clear Valid and "to-uP" flags.
        // Read the inverse of bit 8 of the PID Index Table.
        u4ActiveFlag = (UINT32)(((au4PidIndexTable[u1Pidx] & (1 << 8)) == 0) ? 1 : 0);
        // Copy the inverse to bit 4 of the first word in PID memory.
        PID_S_W(u1Pidx, 0) = (au4W0[u1Pidx] & ~(1 << 4)) | (u4ActiveFlag << 4);
        prPidStruct->fgEnable = FALSE;

        _DMX_Unlock();
    }

    x_thread_delay(1);

    // Flush buffers, and Restore Valid bit and PID Index Table.
    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
    {
        if((u1Pidx != DMX_ALL_INSTINDEX) && (_DMX_GetPidTsIndex(u1Pidx) != u1TsIdx))
        {
            continue;
        }

        if (!(afgPidEnabled[u1Pidx]))
        {
            continue;
        }

        _DMX_Lock();
        prPidStruct = _DMX_GetPidStruct(u1Pidx);
        ePidType = prPidStruct->ePidType;
        prPidStruct->fgEnable = afgPidEnabled[u1Pidx];

        if ((ePidType == DMX_PID_TYPE_ES_AUDIO) ||
            (ePidType == DMX_PID_TYPE_ES_VIDEO) ||
            (ePidType == DMX_PID_TYPE_PSI))
        {
            UINT32 u4PhyAddr;

            u4PhyAddr = (UINT32)PID_S_W(u1Pidx, 5);     // get buf_start
            PID_S_W(u1Pidx, 7) = u4PhyAddr;     // PES_start = buf_start
            PID_S_W(u1Pidx, 8) = u4PhyAddr;     // WP = buf_start
            PID_S_W(u1Pidx, 9) = u4PhyAddr;     // RP = buf_start
            prPidStruct->u4Rp = VIRTUAL(u4PhyAddr);
            prPidStruct->u4SectionRp = VIRTUAL(u4PhyAddr);
            prPidStruct->u4PesRp = VIRTUAL(u4PhyAddr);
            prPidStruct->u4Wp = VIRTUAL(u4PhyAddr);

            u4PhyAddr = (UINT32)PID_S_W(u1Pidx, 10);    // get buf_start
            PID_S_W(u1Pidx, 12) = u4PhyAddr;    // (Header) Start = buf_start
            PID_S_W(u1Pidx, 13) = u4PhyAddr;    // (Header) WP = buf_start
            PID_S_W(u1Pidx, 14) = u4PhyAddr;    // (Header) RP = buf_start
        }

        // Copy bit 4 of the stored DMEM word 0 to bit 8 of the PID Index Table.
        PID_INDEX_TABLE(u1Pidx) = (au4PidIndexTable[u1Pidx] & ~(1 << 8)) |
                                  ((au4W0[u1Pidx] & (1 << 4)) << 4);
        PID_S_W(u1Pidx, 0) = au4W0[u1Pidx];
      //  PID_S_W(u1Pidx, 0) &= 0xFF00FFFF;
      //due to write Number of Section Bytes to 0, uP will bypass data and section filter will notify wrong data.so mark it.
        PID_S_W(u1Pidx, 1) = 0;
        PID_S_W(u1Pidx, 2) &= 0x0000FF00;
        PID_S_W(u1Pidx, 3) = 0xFFFF0002;
        PID_S_W(u1Pidx, 4) = 0xFFFFFFFF;
        _DMX_Unlock();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_DDI_Init
*/
// Disable empty/alert interrupts here, and then enable them when data is
// written to the buffer.  The interrupts also need to be disabled when the
// Empty or Alert interrupts are triggered.
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_Init(void)
{
    static BOOL _fgInit = FALSE;
    INT32 i4Ret;
    UINT8 i, *pu1Prolog, *pu1Epilog;
    UINT32 u4Reg;

    // Turn on CKGEN clock for MT5363.
    // These two lines need to precede _DDI_Reset(TRUE).
//    CKGEN_WRITE32(0x274, CKGEN_READ32(0x274) | 0xFE00);
    CKGEN_WRITE32(0x278, CKGEN_READ32(0x278) | 0x800000);

    x_memset((void*)&_rDmxDDI, 0, sizeof(_rDmxDDI));

    _DMX_Lock();
    _rDmxDDI.eMode = DMX_DDI_MODE_STREAM; // default transfer mode
    _rDmxDDI.eState = DMX_DDI_STOP;       // in case enum is changed carelessly
    _u1DmxDDITsIndex = 0;
    _DMX_Unlock();

    _DDI_Reset(TRUE);
    // DMA is stopped in _DDI_Reset().  No need to use _DMX_Lock() hereafter.

    if (!_DMX_DDI_InitISR())
    {
        return FALSE;
    }

    _fgDDIWaitTransfer = FALSE;
    _fgDDISingleSent = TRUE;                // Single chunk has been sent.
                                            // In fact, not yet happened.
    // Set default DDI clock source --- begin
    _DMX_SetFrontEndEx(1, DMX_FE_DDI);
    // Set default DDI clock source --- end

    if(!_fgInit)
    {
        VERIFY(OSR_OK == x_sema_create(&_hDmxDDISema,
                         X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));

        x_memset((void*)&_arDmxDDITsInfo, 0, sizeof(_arDmxDDITsInfo));
        for(i=0; i<DMX_MAX_INSTANCE; i++)
        {
            pu1Prolog = (UINT8*)BSP_AllocAlignedDmaMemory(DDI_PROLOG_BUF_SIZE,
                                        DDI_BUF_ALIGNMENT);
            pu1Prolog = (UINT8*)VIRTUAL((UINT32)pu1Prolog);
            pu1Epilog = (UINT8*)BSP_AllocAlignedDmaMemory(DDI_EPILOG_BUF_SIZE,
                                        DDI_BUF_ALIGNMENT);
            pu1Epilog = (UINT8*)VIRTUAL((UINT32)pu1Epilog);

            if((pu1Prolog == NULL) || (pu1Epilog == NULL))
            {
                return FALSE;
            }
            _arDmxDDITsInfo[i].pu1Prolog = pu1Prolog;
            _arDmxDDITsInfo[i].pu1Epilog = pu1Epilog;
        }

        _fgInit = TRUE;
    }
    else
    {
        // Make sure the semaphore is reset to the locked state.
        i4Ret = x_sema_lock(_hDmxDDISema, X_SEMA_OPTION_NOWAIT);
        if (i4Ret == OSR_OK)
        {
            LOG(9, "Reset DDI semaphore to the Locked state!\n");
        }
        else if (i4Ret == OSR_WOULD_BLOCK)
        {
            LOG(9, "DDI semaphore is in the Locked state!\n");
        }
        else
        {
            LOG(3, "%d: Semaphore API failed!\n", __LINE__);
        }
    }

    // Only support 188
    u4Reg = DDI_READ32(DDI_REG_PKT_QUADBYTE_LIMIT);
    u4Reg = (u4Reg & 0xFFFFFF00) | (188 >> 2);
    DDI_WRITE32(DDI_REG_PKT_QUADBYTE_LIMIT, u4Reg);

    if (!_DMX_DDI_SetPacketSize(0, 188))
    {
        LOG(3, "Cannot set DDI packet size!\n");
        return FALSE;
    }

    if (!_DMX_DDI_SetPacketSize(1, 188))
    {
        LOG(3, "Cannot set DDI packet size!\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_Set
*/
// Todo: consider to let users turn on/off "PCR Rate Compensation/Control".
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_Set(UINT32 u4Flags, const DMX_DDI_T *prDDI)
{
    ASSERT(prDDI != NULL);

    if(u4Flags == DMX_DDI_FLAG_NONE)
    {
        return FALSE;
    }

    _DMX_Lock();

    if(u4Flags & DMX_DDI_FLAG_MODE)
    {
        _rDmxDDI.eMode = prDDI->eMode;
    }

    if(u4Flags & DMX_DDI_FLAG_ALLOCBUF)
    {
        if(_rDmxDDI.eState != DMX_DDI_STOP)
        {
            _DMX_Unlock();
            LOG(3, "Please stop DDI before memory allocation.\n");
            return FALSE;
        }

        _DMX_Unlock();
        if(!_DDI_AllocBuf(prDDI))
        {
            return FALSE;
        }
        _DMX_Lock();
    }

    if(u4Flags & DMX_DDI_FLAG_CALLBACK)
    {
        ASSERT(prDDI->pfnDDINotify != NULL);
        _rDmxDDI.pfnDDINotify = prDDI->pfnDDINotify;
    }

    if(u4Flags & DMX_DDI_FLAG_RATE)
    {
        // Rate = (N / M) Mbits/sec
        _rDmxDDI.u4RateN = prDDI->u4RateN;
        _rDmxDDI.u4RateM = prDDI->u4RateM;
        DDI_WRITE32(DDI_REG_PERIOD_M, _rDmxDDI.u4RateM * _u4Rate1Mb);
        DDI_WRITE32(DDI_REG_PERIOD_N, _rDmxDDI.u4RateN);
        DDI_WRITE32(DDI_REG_RATE_CMD, 0x3);     // Update M, N; Clear period counter
    }

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_Get
*/
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_Get(UINT8 u1TsIdx, UINT32 u4Flags, DMX_DDI_T *prDDI)
{
    ASSERT(prDDI != NULL);

    if(u4Flags == DMX_DDI_FLAG_NONE)
    {
        return FALSE;
    }

    if(_DDI_UsePVR(u1TsIdx))
    {
        if(u4Flags & DMX_DDI_FLAG_DATA_SIZE)
        {
            DMX_PVR_PLAY_T rPVRPlay;

            if(!_DMX_PVRPlay_Get(PVRPLAY_FLAGS_BUFFER, &rPVRPlay))
            {
                return FALSE;
            }

            prDDI->u4DataSize = DATASIZE(rPVRPlay.u4Rp, rPVRPlay.u4Wp, rPVRPlay.u4BufSize);
        }
    }

    if(u4Flags & DMX_DDI_FLAG_MODE)
    {
        prDDI->eMode = _rDmxDDI.eMode;
    }

    if(u4Flags & DMX_DDI_FLAG_ALLOCBUF)
    {
        prDDI->fgAllocBuf = _rDmxDDI.fgAllocBuf;
        prDDI->u4BufAddr = _rDmxDDI.u4BufAddr;
        prDDI->u4BufSize = _rDmxDDI.u4BufSize;
        prDDI->u4Threshold = _rDmxDDI.u4Threshold;
    }

    if(u4Flags & DMX_DDI_FLAG_CALLBACK)
    {
        prDDI->pfnDDINotify = _rDmxDDI.pfnDDINotify;
    }

    if(u4Flags & DMX_DDI_FLAG_RATE)
    {
        prDDI->u4RateN = _rDmxDDI.u4RateN;
        prDDI->u4RateM = _rDmxDDI.u4RateM;
    }

    if (u4Flags & DMX_DDI_FLAG_DATA_SIZE)
    {
        UINT32 u4BufStart, u4BufEnd, u4BufSize, u4Rp, u4Wp;
        if (!_rDmxDDI.fgAllocBuf)
        {
            prDDI->u4DataSize = 0;
        }
        else
        {
            u4BufStart = VIRTUAL(DDI_READ32(DDI_REG_DMA_BUF_START));
            u4BufEnd   = VIRTUAL(DDI_READ32(DDI_REG_DMA_BUF_END));
            u4BufSize = u4BufEnd - u4BufStart;
            u4Wp = VIRTUAL(DDI_READ32(DDI_REG_DMA_WP));
            ASSERT((u4BufStart <= u4Wp) && (u4Wp < u4BufEnd));
            u4Rp = VIRTUAL(DDI_READ32(DDI_REG_DMA_RP));
            if (u4Rp == 0)
            {
                prDDI->u4DataSize = u4Wp - u4BufStart;
            }
            else
            {
                ASSERT((u4BufStart <= u4Rp) && (u4Rp < u4BufEnd));
                prDDI->u4DataSize = (u4Wp >= u4Rp) ?
                    (u4Wp - u4Rp) :
                    ((u4Wp + u4BufSize) - u4Rp);
            }
        }
    }

    if (u4Flags & DMX_DDI_FLAG_DEBUG_INFO)
    {
        _DMX_Lock();
        prDDI->rDebugInfo.eState = _rDmxDDI.eState;
        prDDI->rDebugInfo.u4BufStart = _rDmxDDI.u4BufAddr;
        prDDI->rDebugInfo.u4BufEnd   = _rDmxDDI.u4BufAddr + _rDmxDDI.u4BufSize;
        prDDI->rDebugInfo.u4Wp  = VIRTUAL(DDI_READ32(DDI_REG_DMA_WP));
        prDDI->rDebugInfo.u4Ap  = VIRTUAL(DDI_READ32(DDI_REG_DMA_AP));
        prDDI->rDebugInfo.u4Rip = VIRTUAL(DDI_READ32(DDI_REG_DMA_RP_INIT));
        prDDI->rDebugInfo.u4Rp  = VIRTUAL(DDI_READ32(DDI_REG_DMA_RP));
        if (prDDI->rDebugInfo.u4Rp != 0)
        {
            _rDmxDDI.u4TransferredSize +=
                _DDI_GetTransferredSize(prDDI->rDebugInfo.u4Rp, _rDmxDDI.u4Rp);
            _rDmxDDI.u4Rp = prDDI->rDebugInfo.u4Rp;
        }
        prDDI->rDebugInfo.u4TransferredSize = _rDmxDDI.u4TransferredSize;
        _DMX_Unlock();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_SetPort
*/
//-----------------------------------------------------------------------------
void _DMX_DDI_SetPort(DMX_DDI_PORT_T ePort, BOOL fgForce)
{
    UINT32 u4Reg;
    UINT8 u1TsIdx;
    DMX_DDI_PORT_T ePrePort;

    if(!fgForce)
    {
        _DMX_Lock();
        ePrePort = _eDDIPortType;
        _DMX_Unlock();
        if(ePrePort == ePort)
        {
            return;
        }
    }

    u4Reg = DDI_READ32(DDI_REG_DMX_RX_CTRL);
#ifndef CC_DMX_EMULATION
    DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID_4, 0x0);
#endif
    if(ePort == DMX_DDI_PORT_FRAMER0)
    {
        u1TsIdx = 0;
        u4Reg &= ~0x10;

        _DMX_SetFrontEndEx(0, DMX_FE_DDI);
    }
    else if(ePort == DMX_DDI_PORT_FRAMER1)
    {
        u1TsIdx = 1;
        u4Reg &= ~0x10;

        _DMX_SetFrontEndEx(1, DMX_FE_DDI);
    }
    else if(ePort == DMX_DDI_PORT_FRAMER3)
    {
        u1TsIdx = 3;
        u4Reg &= ~0x10;

        _DMX_SetFrontEndEx(3, DMX_FE_DDI);
    }
    else if(ePort == DMX_DDI_PORT_NO_FRAMER)
    {
        u1TsIdx = 3;
        u4Reg |= 0x10;

        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) | (1<<29));
#ifndef CC_DMX_EMULATION
        DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID_4, 0x20);     // Steer to FTuP
#endif  // CC_DMX_EMULATION
        // Optional: turn off Framer here (at 0x17020).
    }
    else        // kind of redundant: C type-checking mechanism shall warn us
    {
        LOG(1, "Unknown format!\n");
        return;
    }

    _DMX_Lock();
    DDI_WRITE32(DDI_REG_DMX_RX_CTRL, u4Reg);
    _eDDIPortType = ePort;
    _u1DmxDDITsIndex = u1TsIdx;
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_SetPortEx
*/
//-----------------------------------------------------------------------------
void _DMX_DDI_SetPortEx(DMX_DDI_PORT_T ePort, UINT8 u1Pidx, BOOL fgForce,  BOOL fgTs)
{
    UINT32 u4Reg;
    UINT8 u1TsIdx;
    DMX_DDI_PORT_T ePrePort;

    if(!fgForce)
    {
        _DMX_Lock();
        ePrePort = _eDDIPortType;
        _DMX_Unlock();
        if(ePrePort == ePort)
        {
            return;
        }
    }

    u4Reg = DDI_READ32(DDI_REG_DMX_RX_CTRL);

    if(ePort == DMX_DDI_PORT_FRAMER0)
    {
        u1TsIdx = 0;
        u4Reg &= ~0x10;

        _DMX_SetFrontEndEx(0, DMX_FE_DDI);

        if(fgTs)
        {
            DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID, 0);

            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) & ~(1<<26));

            _DMX_SetFramerMode(DMX_FMR0, DMX_FRAMER_PARALLEL, FALSE, TRUE);

            DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE,
                DMXCMD_READ32(DMX_REG_FRAMER_ERROR_HANDLE) | (0xf0c0U));
        }
        else
        {
            DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID, u1Pidx << 16 | 0x20);

            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) | (1<<26));

            _DMX_SetFramerMode(DMX_FMR0, DMX_FRAMER_PARALLEL, TRUE, TRUE);

            DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE,
                DMXCMD_READ32(DMX_REG_FRAMER_ERROR_HANDLE) & ~(0xffffU));
        }
    }
    else if(ePort == DMX_DDI_PORT_FRAMER1)
    {
        u1TsIdx = 1;
        u4Reg &= ~0x10;

        _DMX_SetFrontEndEx(1, DMX_FE_DDI);

        if(fgTs)
        {
            DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID_2, 0);

            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) & ~(1<<27));

            // use internal sync
            _DMX_SetFramerMode(DMX_FMR1, DMX_FRAMER_PARALLEL, FALSE, TRUE);

            DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE,
                DMXCMD_READ32(DMX_REG_FRAMER_ERROR_HANDLE) | (0xf0c0U << 16));
        }
        else
        {
            UINT32 u4Ctrl;

            DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID_2, u1Pidx << 16 | 0x20);

            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) | (1<<27));

            // use external sync
            _DMX_SetFramerMode(DMX_FMR1, DMX_FRAMER_PARALLEL, TRUE, TRUE);

            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x2000);
            u4Ctrl |= 0x2000;
            DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
            //x_thread_delay(1);
            HAL_Delay_us(1);
            u4Ctrl &= (~0x2000);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

            DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE,
                DMXCMD_READ32(DMX_REG_FRAMER_ERROR_HANDLE) & ~(0xffffU << 16));
        }
    }
    else if(ePort == DMX_DDI_PORT_NO_FRAMER)
    {
        u1TsIdx = 3;
        u4Reg |= 0x10;

        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) | (1<<29));
#ifndef CC_EMULATION
        DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID_4, 0x20);     // Steer to FTuP
#endif  // CC_EMULATION
        // Optional: turn off Framer here (at 0x17020).
    }
    else        // kind of redundant: C type-checking mechanism shall warn us
    {
        LOG(1, "Unknown format!\n");
        return;
    }

    _DMX_Lock();
    DDI_WRITE32(DDI_REG_DMX_RX_CTRL, u4Reg);
    _eDDIPortType = ePort;
    _u1DmxDDITsIndex = u1TsIdx;
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_GetPort
*/
//-----------------------------------------------------------------------------
DMX_DDI_PORT_T _DMX_DDI_GetPort(void)
{
    DMX_DDI_PORT_T ePort;

    _DMX_Lock();
    ePort = _eDDIPortType;
    _DMX_Unlock();

    return ePort;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_GetTunerIndex
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_DDI_GetTunerIndex(void)
{
    UINT8 u1TsIdx;

    _DMX_Lock();
    u1TsIdx = _u1DmxDDITsIndex;
    _DMX_Unlock();

    return u1TsIdx;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_Free
*/
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_Free(void)
{
    if(_rDmxDDI.eState != DMX_DDI_STOP)
    {
        LOG(3, "Please stop DDI before its buffer is freed.\n");
        return FALSE;
    }

    _DDI_Reset(FALSE);  // Only reset some HW registers (RIP, WP, AP, and etc).

    if(!_DDI_FreeBuf())
    {
        return FALSE;   // Current implementation will never fall into here.
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_Notify
*   Call in HISR - DDI or DMX interrupt
*/
// This function is called by DDI's ISR when the buffer is empty or when the
// buffer needs to be refilled with more data (ie., AP == RP).
// Sync the software RP with the hardware RP.
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_Notify(void)
{
    UINT32 u4Rp;
    DMX_DDI_STRUCT_T rDmxDDI;
    BOOL fgWait;

    _DMX_Lock();
    rDmxDDI = _rDmxDDI;
    _DMX_Unlock();

#ifdef CC_DMX_TS_CIPLUS
    if(!_DMX_CIPlus_GetIsUsed())
#endif  // CC_DMX_TS_CIPLUS
    {
        if ((rDmxDDI.eMode == DMX_DDI_MODE_STREAM) &&
            (rDmxDDI.eState != DMX_DDI_PLAY))
        {
            LOG(3, "The stopped DDI is not notified!\n");
            return FALSE;
        }
    }

    u4Rp = VIRTUAL(DDI_READ32(DDI_REG_DMA_RP));
    ASSERT(u4Rp != 0);

    // Sync before notification.
    // RP may drift forward if the buffer is not empty.
    // The result shall only affect free buffer calculation and nothing else.
    _DMX_Lock();
    _rDmxDDI.u4TransferredSize += _DDI_GetTransferredSize(u4Rp, rDmxDDI.u4Rp);
    _rDmxDDI.u4Rp = u4Rp;
    _DMX_Unlock();

    // Set variables to the appropriate values before notifying users.
    if ((rDmxDDI.eMode == DMX_DDI_MODE_SINGLE) ||
        (rDmxDDI.eMode == DMX_DDI_MODE_NONBLOCKING))
    {
        _DMX_Lock();
        _fgDDISingleSent = TRUE;                // Single chunk has been sent.
        _rDmxDDI.eState = DMX_DDI_STOP;
        _DMX_Unlock();
        _DMX_DDI_SetDMAInt(FALSE, FALSE);       // disable Empty/Alert INT
        DDI_WRITE32(DDI_REG_DMA_CTRL, 4);       // Stop the DMA operation

        _DMX_Lock();
        fgWait = _fgDDIWaitTransfer;
        _DMX_Unlock();
        if(fgWait)
        {
            VERIFY(OSR_OK == x_sema_unlock(_hDmxDDISema));
        }
    }

    if ((rDmxDDI.eMode == DMX_DDI_MODE_SINGLE) ||
        (rDmxDDI.eMode == DMX_DDI_MODE_STREAM))
    {
        if (rDmxDDI.pfnDDINotify != NULL)
        {
            if (!rDmxDDI.pfnDDINotify(DDI_EVENT_TRANSFER_OK))
            {
                LOG(3, "DDI callback failed! (line: %d)\n", __LINE__);
            }
        }
    }
    else if (rDmxDDI.eMode == DMX_DDI_MODE_NONBLOCKING)
    {
        // Callback function will be called in _DDI_Thread().
    }
    else
    {
        ASSERT(0);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_FlushBuf
*/
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_FlushBuf(VOID)
{
    UINT32 u4PhyBufStart;

    if(_rDmxDDI.eState != DMX_DDI_STOP)
    {
        LOG(3, "Please stop DDI before its buffer is flushed.\n");
        return FALSE;
    }

    // Users need to stop DDI before they flush DDI buffer.  Therefore, the
    // _rDmxDDI.u4TransferredSize is not updated here, but in the Stop function.
    _DMX_Lock();
    _rDmxDDI.u4Wp = _rDmxDDI.u4BufAddr;
    _rDmxDDI.u4Rp = _rDmxDDI.u4BufAddr;
    u4PhyBufStart = PHYSICAL(_rDmxDDI.u4BufAddr);
    DDI_WRITE32(DDI_REG_DMA_RP_INIT, u4PhyBufStart);   // Initial RP
    DDI_WRITE32(DDI_REG_DMA_WP, u4PhyBufStart);        // Write pointer
    DDI_WRITE32(DDI_REG_DMA_AP, u4PhyBufStart);        // Alert pointer
    // The update of RP will be done by hardware when DMA is activiated.

    // Reset RP to the starting address of the DDI buffer.
    DDI_WRITE32(DDI_REG_DMA_CTRL, 5);       // Start the DMA operation
    DDI_WRITE32(DDI_REG_DMA_CTRL, 4);       // Stop the DMA operation

    // Empty/Alert interrupts are disabled when DDI is stopped.
    // Clear the interrupt generated when RP is reset above.
    DDI_WRITE32(DDI_REG_DCR_INT_CLR, 0x7);      // Clear all DMA interrupts
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_GetFreeBufSize
*/
// Side effect: update the RP in the global variable.
//
// u4FreeSize shall be more appropriately named u4AvailableSize because not all
// free space is availabel to user (ie., some of the free space is reserved).
//-----------------------------------------------------------------------------
UINT32 _DMX_DDI_GetFreeBufSize(UINT8 u1TsIdx)
{
    UINT32 u4Rp, u4Wp, u4BufStart, u4BufEnd, u4BufSize, u4FreeSize;

    if(u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return 0;
    }

    _DMX_Lock();
    u4BufStart = _rDmxDDI.u4BufAddr;
    u4BufSize  = _rDmxDDI.u4BufSize;
    u4Wp       = _rDmxDDI.u4Wp;
    u4Rp = VIRTUAL(DDI_READ32(DDI_REG_DMA_RP));
    if (u4Rp != 0)
    {
        _rDmxDDI.u4TransferredSize += _DDI_GetTransferredSize(u4Rp, _rDmxDDI.u4Rp);
        _rDmxDDI.u4Rp = u4Rp;   // Sync with hardware RP
    }
    else
    {
        // Do not need to update _rDmxDDI.u4TransferredSize in this case.
        u4Rp = _rDmxDDI.u4Rp;   // hardware RP has not yet been updated
    }
    _DMX_Unlock();

    u4BufEnd = u4BufStart + u4BufSize;

    ASSERT((u4BufStart <= u4Rp) && (u4Rp < u4BufEnd));
    ASSERT((u4BufStart <= u4Wp) && (u4Wp < u4BufEnd));

    u4FreeSize = 0;
    if(u4Rp == u4Wp)
    {
        u4FreeSize = u4BufSize;
    }
    else if(u4Rp > u4Wp)
    {
        u4FreeSize = u4Rp - u4Wp;
    }
    else
    {
        u4FreeSize = (u4BufEnd - u4BufStart) - (u4Wp - u4Rp);
    }

    // Reserve some free space
    if(u4FreeSize > _arDmxDDITsInfo[u1TsIdx].u4BufFullGap)
    {
        u4FreeSize -= _arDmxDDITsInfo[u1TsIdx].u4BufFullGap;
    }
    else
    {
        u4FreeSize = 0;
    }

    return u4FreeSize;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_SingleMove
*   For the Single move mode.
*
*   The parameter u4BufEnd must be the address right next to the real end of
*   the buffer.  In other words, the data byte addressed by u4BufEnd does not
*   belong to the buffer.  Both start/end addresses must be aligned to 16-byte
*   boundary.  Both Read/Write pointers must be aligned to 4-byte boundary.
*/
// The addresses passed into this function shall be virtual addresses.
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_SingleMove(UINT32 u4BufStart, UINT32 u4BufEnd,
                         UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 u4BufSize, u4Wp, u4Rp;
    UINT32 u4PhyBufStart, u4PhyBufEnd, u4PhyBufRp, u4PhyBufWp;
	UINT32 u4Tmp;
    BOOL fgFlush = TRUE;

    if(!_fgDDISingleSent)
    {
        LOG(3, "Still moving data...\n");
        return FALSE;
    }

    if ((_rDmxDDI.eMode != DMX_DDI_MODE_SINGLE) &&
        (_rDmxDDI.eMode != DMX_DDI_MODE_NONBLOCKING))
    {
        LOG(3, "Please switch to SINGLE or NONBLOCKING mode first.\n");
        return FALSE;
    }

    ASSERT(u4BufStart != 0);
    ASSERT(u4BufEnd   != 0);
    ASSERT(u4Addr     != 0);
    ASSERT(_DMX_IsAligned(u4BufStart, DDI_BUF_ALIGNMENT));
    ASSERT(_DMX_IsAligned(u4BufEnd,   DDI_BUF_ALIGNMENT));

    u4BufSize = u4BufEnd - u4BufStart;
    ASSERT(u4BufSize > u4Size);
    ASSERT((u4Addr >= u4BufStart) && (u4Addr < u4BufEnd));

    u4Rp = u4Addr;
    u4Wp = u4Addr + u4Size;
    if(u4Wp >= u4BufEnd)  // DMA never stops if (WP == Buffer End).
    {
        u4Wp -= u4BufSize;
    }

    DDI_WRITE32(DDI_REG_DMA_CTRL, 4);       // Stop the DMA operation
    
#ifdef ENABLE_MULTIMEDIA
#ifdef CC_53XX_SWDMX_V2
    if (SWDMX_GetInfo(SWDMX_PRIMARY_SOURCE, eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0))
#else
	if (SWDMX_GetInfo(eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0))
#endif
	{
		if((u4Tmp >= (UINT32)SWDMX_FMT_MPEG1_DAT)&&(u4Tmp <= (UINT32)SWDMX_FMT_RAW_AUDIO_ES))
		{
		  fgFlush = FALSE;
		}
	}
#endif
	
	if (fgFlush)
	{

    // Flushing cache can avoid the cache consistency problem at the cost of
    // performance.  If a user needs to move his local data via DDI DMA, he
    // needs to flush cache by himself.  This convention can avoid unnecessary
    // cache-flushing operation in the driver.
  #ifndef  CC_DMX_FLUSH_ALL
    if((u4Addr+u4Size)> u4BufEnd)
    {
      HalFlushInvalidateDCacheMultipleLine(u4Addr,(u4BufEnd-u4Addr));
	  HalFlushInvalidateDCacheMultipleLine(u4BufStart,(u4Addr+u4Size-u4BufEnd));
	}
	else
	{
	  HalFlushInvalidateDCacheMultipleLine(u4Addr,u4Size);
	}
  #else
      HalFlushInvalidateDCache();
  #endif
    }

    u4PhyBufStart = PHYSICAL(u4BufStart);
    u4PhyBufEnd   = PHYSICAL(u4BufEnd);
    u4PhyBufRp    = PHYSICAL(u4Rp);
    u4PhyBufWp    = PHYSICAL(u4Wp);

    // For supporting the calculation of _rDmxDDI.u4TransferredSize.
    _DMX_Lock();
    _rDmxDDI.fgAllocBuf  = FALSE;
    _rDmxDDI.u4Threshold = 0;
    _rDmxDDI.u4BufAddr = VIRTUAL(u4BufStart);
    _rDmxDDI.u4BufSize = u4BufSize;
    _rDmxDDI.u4Rp = VIRTUAL(u4Rp);
    _rDmxDDI.u4Wp = VIRTUAL(u4Wp);
    _rDmxDDI.eState = DMX_DDI_PLAY;
    _rDmxDDI.pfnDDINotify = NULL;

    // Set DDI ring buffer
    DDI_WRITE32(DDI_REG_DMA_BUF_START, u4PhyBufStart);      // Start pointer
    DDI_WRITE32(DDI_REG_DMA_BUF_END, u4PhyBufEnd);          // End pointer
    DDI_WRITE32(DDI_REG_DMA_RP_INIT, u4PhyBufRp);           // Initial RP
    DDI_WRITE32(DDI_REG_DMA_WP, u4PhyBufWp);                // Write pointer
    // DDI_WRITE32(DDI_REG_DMA_AP, u4PhyBufWp);             // Alert pointer

    _fgDDISingleSent = FALSE;           // Single chunk has not yet been sent.
                                        // This flag will be changed by ISR.
    _DMX_Unlock();

    _DMX_DDI_SetDMAInt(TRUE, FALSE);    // enable Emtpy Int; disable Alert Int
    DDI_WRITE32(DDI_REG_DMA_CTRL, 5);       // Start the DMA operation

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_SetPacketSize
*
*/
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_SetPacketSize(UINT8 u1TsIdx, UINT8 u1PacketSize)
{
    UINT8 u1SyncOffset;

    if(_rDmxDDI.eState != DMX_DDI_STOP)
    {
        LOG(3, "Please stop DDI first.\n");
        return FALSE;
    }

    if(u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }

    switch (u1PacketSize)
    {
    case 188:
        u1SyncOffset = 0;
        break;

    case 192:
        u1SyncOffset = 4;
        break;

    default:
        LOG(1, "Packet size (%u) is not supported!", u1PacketSize);
        return FALSE;
    }

    _DMX_Lock();
    _arDmxDDITsInfo[u1TsIdx].u1SyncOffset = u1SyncOffset;

    _u4Rate1Mb = (UINT32)(u1PacketSize * 8 * 27);    // 1Mbit rate

    _arDmxDDITsInfo[u1TsIdx].u1PacketSize = u1PacketSize;
    _arDmxDDITsInfo[u1TsIdx].u4BufFullGap = (UINT32)((u1PacketSize * 4) + 32);
    _arDmxDDITsInfo[u1TsIdx].u4PrologSize = (UINT32)(u1PacketSize * DDI_PROLOG_PACKETS);
    _arDmxDDITsInfo[u1TsIdx].u4EpilogSize = (UINT32)(u1PacketSize * DDI_EPILOG_PACKETS);
    _DMX_Unlock();

    // Set framer prebuffer

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_GetPacketSize
*
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_DDI_GetPacketSize(void)
{
    UINT8 u1PacketSize;

    _DMX_Lock();
    u1PacketSize = (UINT8)(DDI_READ32(DDI_REG_PKT_QUADBYTE_LIMIT) & 0xFFFFFF00);
    _DMX_Unlock();

    return u1PacketSize;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_SetSyncOffset
*
* The function _DMX_DDI_SetPacketSize() calculate a default offset value for
* the Sync byte.  Call this function to change the offset value if the default
* value does not meet user's requirement.
*/
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_SetSyncOffset(UINT8 u1TsIdx, UINT8 u1Offset)
{
    UINT8 u1MinOffset = 0, u1MaxOffset;

    if(u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }

    u1MaxOffset = _arDmxDDITsInfo[u1TsIdx].u1PacketSize - (UINT8)188;

    if ((u1MinOffset <= u1Offset) && (u1Offset <= u1MaxOffset))
    {
        _arDmxDDITsInfo[u1TsIdx].u1SyncOffset = u1Offset;
        return TRUE;
    }

    LOG(1, "Sync byte offset (%d) is not supported!\n", u1Offset);
    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_TSMoveData
 *  Because each of the TS/PS uses FULL-PULL mode, the hardware will pause
 *  data transmission operation if there is no vacancy in any one of the
 *  buffers.  From users' perspective, users do not know/care where the PAUSE
 *  operation takes place (in DDI or Demux).  Therefore, waiting for the
 *  semaphore here has the same effect as pausing in Demux for users.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_TSMoveData(DMX_MM_DATA_T *prData)
{
    UINT32 u4BufStart = 0, u4BufEnd = 0, u4SendAddr, u4SendSize;
    UINT8 u1Remainder, au1Padding[DDI_PADDING_BUF_SIZE];

    if(prData == NULL)
    {
        return FALSE;
    }

    if(prData->u1Idx >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    // Make sure the addresses are aligned and packet size is 188N bytes.
    if ((!_DMX_IsAligned(prData->u4BufStart, DDI_BUF_ALIGNMENT)) ||
        (!_DMX_IsAligned(prData->u4BufEnd,   DDI_BUF_ALIGNMENT)))
    {
        LOG(3, "The buffer boundary address is not aligned!\n");
        return FALSE;
    }

    // Transmit two TS null packets for Internal Sync Lock at the stream head.
    // Null Packet: 0x471FFF10 + 0xFF...0xFF
    if (prData->fgFrameHead)
    {
        VERIFY(_DDI_SetupProlog(prData->u1Idx, &u4BufStart, &u4BufEnd));
        if (!_DDI_WaitTransfer(prData->u1Idx, u4BufStart, u4BufEnd, u4BufStart,
                                _arDmxDDITsInfo[prData->u1Idx].u4PrologSize))
        {
            return FALSE;
        }
    }

    u4BufStart = prData->u4BufStart;
    u4BufEnd   = prData->u4BufEnd;
    u4SendAddr = prData->u4StartAddr;
    u4SendSize = prData->u4FrameSize;
    u1Remainder = (UINT8)(u4SendSize % _arDmxDDITsInfo[prData->u1Idx].u1PacketSize);

    if (!(prData->fgEOS))
    {
        u1Remainder = 0; // don't care about u1Remainder != 0
        if (u1Remainder == 0)
        {
            if (!_DDI_WaitTransfer(prData->u1Idx, u4BufStart, u4BufEnd, u4SendAddr, u4SendSize))
            {
                return FALSE;
            }
        }
        else
        {
            LOG(3, "The size is not a multiple of %d bytes.\n",
                            _arDmxDDITsInfo[prData->u1Idx].u1PacketSize);
            return FALSE;
        }
    }
    else
    {
        if (u1Remainder == 0)
        {
            if (!_DDI_WaitTransfer(prData->u1Idx, u4BufStart, u4BufEnd, u4SendAddr, u4SendSize))
            {
                return FALSE;
            }
        }
        else
        {
            // Transmit the first _u1PacketSize*N bytes.
            u4SendSize -= u1Remainder;
            if (!_DDI_WaitTransfer(prData->u1Idx, u4BufStart, u4BufEnd, u4SendAddr, u4SendSize))
            {
                return FALSE;
            }
            u4SendAddr += u4SendSize;
            // Transmit the remaining data along with the padding bytes.
            u4BufStart = (((UINT32)au1Padding + DDI_BUF_ALIGNMENT) & 0xFFFFFFF0);
            u4BufEnd   = (((UINT32)au1Padding + DDI_PADDING_BUF_SIZE) & 0xFFFFFFF0);
            x_memset((void*)au1Padding, 0xFF, sizeof(au1Padding));
            x_memcpy((void*)u4BufStart, (const void*)u4SendAddr, u1Remainder);
            HalFlushInvalidateDCache();
            if (!_DDI_WaitTransfer(prData->u1Idx, u4BufStart, u4BufEnd, u4BufStart,
                            _arDmxDDITsInfo[prData->u1Idx].u1PacketSize))
            {
                return FALSE;
            }
        }

        if(!_DDI_SendAllDummyPackets(prData->u1Idx))
        {
            return FALSE;
        }

        // Wait uCode really finish handle final data chunk
        // Because EOS, delay 10ms is not easy detectable
        x_thread_delay(10);

        if(!_DDI_Sync_VideoWp(prData->u1Idx))
        {
            return FALSE;
        }

        if(!_DDI_SendTSEOS(prData->u1Idx))
        {
            return FALSE;
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_PowerDown
 *
 *  After the DDI is powered down, users need to call DDI_Init() to bring it
 *  back to life.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_PowerDown(VOID)
{
    UINT32 u4Reg;

    _DMX_Lock();

    u4Reg = DDI_READ32(DDI_REG_GLOBAL_CTRL);
    u4Reg &= ~(1 << 3);
    DDI_WRITE32(DDI_REG_GLOBAL_CTRL, u4Reg);

    // Turn off CKGEN clock for MT5363.
    CKGEN_WRITE32(0x278, CKGEN_READ32(0x278) & ~(0x1 << 23));

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_TSRequestReset
 *
 *  Reset DDI/DBM/Framer and ignore all requests in the message queue.
 *  This function is an immediate request, which means that the request will
 *  be processed immediately and will not be postponed.
 *
 *  Note:
 *  Before calling this function, the users must make sure that the previous
 *  DDI data transmission operation has finished.
 *
 *  FIXME: Rename this function to _DMX_DDI_Reset() after the call to
 *         _DMX_DDI_TSRequestReset() is changed to _DMX_DDI_Reset() in swdmx_mpg.c.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_TSRequestReset(UINT8 u1TsIdx)
{
    if(_DDI_UsePVR(u1TsIdx))
    {
        return _DMX_PVRPlay_RequestReset(u1TsIdx);
    }

    // Only for DMX_IN_PLAYBACK_TS and DMX_IN_PLAYBACK_PS

    if (_rDmxDDI.eMode != DMX_DDI_MODE_SINGLE)
    {
        LOG(3, "Please switch to SINGLE mode first.\n");
        return FALSE;
    }

    _DDI_Reset(TRUE);

    _fgDDISingleSent = TRUE;            // No protection (DDId has higher priority).
    _DMX_DDI_SetDMAInt(TRUE, FALSE);    // enable Emtpy Int; disable Alert Int

    if (!_DDI_TsResetDmxBuffers(u1TsIdx))
    {
        LOG(5, "Failed to reset DMX buffers!\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_Unlock
 *
 *  This function is meant to be called by the Monitor thread in auto-reset
 *  to avoid a deadlock condition in which the DMX loops indefinitely and
 *  DDI waits for the completion of data transmission.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_Unlock(void)
{
    return (OSR_OK == x_sema_unlock(_hDmxDDISema));
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_HWReset
 */
//-----------------------------------------------------------------------------
void _DMX_DDI_HWReset(void)
{
    UINT32 u4Reg, u4Reg1;

    u4Reg = DDI_READ32(DDI_REG_DMX_RX_CTRL);
    u4Reg1 = DDI_READ32(DDI_REG_PKT_QUADBYTE_LIMIT);

    // DDI soft reset
    DDI_WRITE32(DDI_REG_GLOBAL_CTRL, 0);
    DDI_WRITE32(DDI_REG_GLOBAL_CTRL, 0x208);

    DDI_WRITE32(DDI_REG_DMX_RX_CTRL, u4Reg);
    DDI_WRITE32(DDI_REG_PKT_QUADBYTE_LIMIT, u4Reg1);
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_WaitTransfer
 */
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_WaitTransfer(UINT8 u1TsIdx, UINT32 u4BufStart, UINT32 u4BufEnd,
                              UINT32 u4SendAddr, UINT32 u4SendSize)
{
    return _DDI_WaitTransfer(u1TsIdx, u4BufStart, u4BufEnd, u4SendAddr, u4SendSize);
}

//-----------------------------------------------------------------------------
/** _DMX_SetTSFilePlayPath
 *  Everytime move TS data, check physical
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetTSFilePlayPath(UINT8 u1TsIndex, DMX_TSFMT_T eFmt)
{
    BOOL fgRet = TRUE;
    UINT8 u1PktSize;

    if(u1TsIndex >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }

    if((eFmt == DMX_TSFMT_192) || (eFmt == DMX_TSFMT_192_ENCRYPT) ||
       (eFmt == DMX_TSFMT_TIMESHIFT))
    {
        u1PktSize = 192;
    }
    else
    {
        u1PktSize = 188;
    }

    if(!_DMX_SetFramerPktSize(u1TsIndex, u1PktSize))
    {
        return FALSE;
    }

    if(u1TsIndex == 2)
    {
        fgRet = _DMX_PVRPlay_SetMoveMode(eFmt, FALSE);
    }
    else
    {
        if(u1TsIndex == 0)
        {
            _DMX_DDI_SetPort(DMX_DDI_PORT_FRAMER0, FALSE);
        }
        else if(u1TsIndex == 1)
        {
            _DMX_DDI_SetPort(DMX_DDI_PORT_FRAMER1, FALSE);
        }
    }

    if(!_DMX_DDI_SetPacketSize(u1TsIndex, u1PktSize))
    {
        return FALSE;
    }

    return fgRet;
}


