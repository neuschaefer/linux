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
 * $RCSfile: dmx_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_api.c
 *  Demux driver - main implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_ide.h"
#include "dmx_debug.h"
#include "dmx_if.h"
#include "fvr.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "mpv_drvif.h"
#include "aud_drvif.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_hal_arm.h"
#include "x_mid.h"
#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#include "nim_if.h"
#endif

LINT_EXT_HEADER_END


#ifdef TIME_MEASUREMENT
LINT_EXT_HEADER_BEGIN
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
LINT_EXT_HEADER_END
BOOL _fgWaitForFirstI = FALSE;  // Measure the time from "Enabling PID"
                                // to the time "the first I frame is received".
#endif  // TIME_MEASUREMENT

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

/// Define DEBUG_MEMORY_INTRUSION to protect video FIFO from writing by other
/// agents
//
//#define DEBUG_MEMORY_INTRUSION

#undef CC_DMX_USE_CT
#ifndef DMX_NT_UCODE_DISABLE
#define CC_DMX_USE_CT
#endif  // DMX_NT_UCODE_DISABLE


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// 4 KB
#define PES_HEADER_BUF_SIZE                 0x1000

/// Default interrupt threshold of video PID (in unit of 188-byte)
#define DEF_INT_THRESHOLD_VIDEO             0

/// Default interrupt threshold of audio PID (in unit of 188-byte)
#define DEF_INT_THRESHOLD_AUDIO             3

#ifdef CC_DMX_USE_CT
// Prevent this PID value from being set into the hardware PID filter.
#define BPS_PID                             0x1FFB

// The flag "ENABLE_0X1FFB_FILTERING" is no longer needed on MT5387/5388.
// The uP is told to perform 0x1FFB through the Byte 3 of CT_SETTING.
#endif  // CC_DMX_USE_CT

/// Test lock times
#define TS_LOCK_TEST_TIMES                  3

/// Maximum waiting loop of TS locking
#define TS_LOCK_MAX_LOOP                    3000

/// Minimum packet count of TS locking
#define TS_LOCK_MIN_PACKET                  5

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// The PID structures
static PID_STRUCT_T _arPidStruct[DMX_NUM_PID_INDEX];

/// The DMX memory tracker
static DMX_MEM_TRACKER_T _rMemTracker;

/// The filter structures
static FILTER_STRUCT_T _arFilterStruct[DMX_NUM_FILTER_INDEX];

/// DMX certain states
static DMX_STATES_T _rDmxStates;

/// State word for locking interrupt
static CRIT_STATE_T _rDmxLock;

/// Is interrupt locking? For making sure Lock()/Unlock() are not nested
static BOOL _fgDmxLocking = FALSE;

/// Interrupt threshold of video PID (in unit of 188-byte)
static UINT16 _u2IntThresholdVideo = DEF_INT_THRESHOLD_VIDEO;

/// Interrupt threshold of audio PID (in unit of 188-byte)
static UINT16 _u2IntThresholdAudio = DEF_INT_THRESHOLD_AUDIO;

#ifdef DEBUG_MEMORY_INTRUSION

    /// Region ID of protected buffer
    static UINT32 _u4ProtectedRegion = MID_MAX_REGIONS;

#endif  // DEBUG_MEMORY_INTRUSION

#ifdef DMX_CHECK_SECTION_CRC
static BOOL _afgFilterCRC[32][50];
static UINT32 _au4FilterCRCIdx[32];
#endif  // DMX_CHECK_SECTION_CRC

static FILTER_TYPE_T _eFilterType = FILTER_TYPE_LEGACY;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifdef DEBUG_MEMORY_INTRUSION

//-----------------------------------------------------------------------------
/** _DmxIntrudeHandler
 *  Memory intrusion handler
 *
 *  @param  u4Region        The intruded memory region
 *  @param  eAgentId        The agent who intrudes the protected memory
 *  @param  u4Addr          The intruded memory address
 *
 *  @retval -
 */
//-----------------------------------------------------------------------------
static void _DmxIntrudeHandler(UINT32 u4Region, MID_AGENT_ID_T eAgentId,
    UINT32 u4Addr)
{
    // Flush log data
    UTIL_LogFlush();

    // Show intrusion info
    Printf("MEMORY VIOLATION - agent: %s, address: 0x%08x\n",
        MID_AgentIdToString(eAgentId), u4Addr);
    Printf("System halted\n");

    // Reset MID
    VERIFY(MID_Reset());

    // Halt system
    BSP_HaltSystem();

    UNUSED(u4Region);
}


//-----------------------------------------------------------------------------
/** _DmxInitMemoryProtection
 *  Init memory protection
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitMemoryProtection(void)
{
    PFN_MID_CALLBACK pfnOrgHandler;

    // Init MID
    if (!MID_Init())
    {
        return FALSE;
    }

    // Register handler
    pfnOrgHandler = MID_RegisterHandler(_DmxIntrudeHandler);
    UNUSED(pfnOrgHandler);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxIsProtectedMemory
 *  If the buffer of a given PID should be protected or not
 *
 *  @param  prPidStruct     PID structure
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
static BOOL _DmxIsProtectedMemory(const PID_STRUCT_T* prPidStruct)
{
    BOOL fgRet;

    ASSERT(prPidStruct != NULL);

    // Currently, only video ES buffer is protected
    fgRet = (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DmxSetProtectedMemory
 *  Setup a memory region to be protected
 *
 *  @param  u4BufStart      Start address of the protected memory
 *  @param  u4BufEnd        End address of the protected memory
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxSetProtectedMemory(UINT32 u4BufStart, UINT32 u4BufEnd)
{
    ASSERT(_DMX_IsAligned(u4BufStart, MID_ADDR_ALIGNMENT));
    ASSERT(_DMX_IsAligned(u4BufEnd, MID_ADDR_ALIGNMENT));

    // Allocate a read-only region
    if (!MID_AllocateFreeRegion(FALSE, &_u4ProtectedRegion))
    {
        return FALSE;
    }

    // Set RO section to read-only
    if (!MID_SetRegion(_u4ProtectedRegion, MID_AGENT_DEMUX, u4BufStart,
        u4BufEnd))
    {
        return FALSE;
    }

    // Enable the region detection
    if (!MID_EnableRegionProtect(_u4ProtectedRegion))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxResetProtectedMemory
 *  Reset memory protection
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxResetProtectedMemory(void)
{
    if (_u4ProtectedRegion < MID_MAX_REGIONS)
    {
        VERIFY(MID_ResetRegion(_u4ProtectedRegion));
        _u4ProtectedRegion = MID_MAX_REGIONS;
    }

    return TRUE;
}


#endif  // DEBUG_MEMORY_INTRUSION


#ifdef CC_DMX_USE_CT
//-----------------------------------------------------------------------------
/** _Dmx_GetBypassPidRegWordOffset
 *  Get the word address of the Bypass PID Register.
 *
 *  @param  u1IsIndex       TS index
 *
 *  @retval                 the word offset from the DMUX0 (0x20017000).
 */
//-----------------------------------------------------------------------------
static UINT32 _Dmx_GetBypassPidRegWordOffset(UINT8 u1TsIndex)
{
    UINT32 u4Register = DMX_REG_DBM_BYPASS_PID;

    switch (u1TsIndex)
    {
    case 0:
        u4Register = DMX_REG_DBM_BYPASS_PID;
        break;
    case 1:
        u4Register = DMX_REG_DBM_BYPASS_PID_2;
        break;
    case 2:
        u4Register = DMX_REG_DBM_BYPASS_PID_3;
        break;
    case 3:
        u4Register = DMX_REG_DBM_BYPASS_PID_4;
        break;
    default:
        LOG(1, "Unknown TS_index (%u)!\n", u1TsIndex);
        ASSERT(0);
        break;
    }

    return u4Register;
}
#endif  // CC_DMX_USE_CT


//-----------------------------------------------------------------------------
/** _DmxAllocateVideoChannel
 *  Allocate a video channel for a given pid index
 *
 *  @param  u1Pidx          PID index
 *  @param  pu1Channel [output]
 *                          The allocated video channel
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxAllocateVideoChannel(UINT8 u1Pidx, UINT8* pu1Channel)
{
    UINT8 i;
    BOOL fgRet = FALSE;

    ASSERT(pu1Channel != NULL);

    _DMX_Lock();

    for (i = 0; i < MPV_MAX_ES; i++)
    {
        if (_rDmxStates.au1VideoPidx[i] == DMX_NULL_PIDX)
        {
            // Found an empty channel
            _rDmxStates.au1VideoPidx[i] = u1Pidx;
            *pu1Channel = i;
            fgRet = TRUE;
            break;
        }
    }

    _DMX_Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DmxSetPidBuffer
 *  Setup PID buffer(s)
 *
 *  @param  u1Pidx          PID index
 *  @param  prPid           PID structure
 *  @param  prPidStruct     Local PID structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxSetPidBuffer(UINT8 u1Pidx, const DMX_PID_T* prPid,
    PID_STRUCT_T* prPidStruct)
{
    UINT32 u4BufStart, u4PhyBufStart, u4W0, u4BufSize;

    ASSERT(prPid != NULL);
    ASSERT(prPidStruct != NULL);

    if (prPid->ePidType == DMX_PID_TYPE_NONE)
    {
        // No PID type, can't setup buffer
        return TRUE;
    }

    // Check if buffer already allocated, to avoid memory leakage
    if ((prPidStruct->u4BufStart != 0) || (prPidStruct->u4HeaderBufAddr != 0))
    {
        return FALSE;
    }

    if (prPid->ePidType == DMX_PID_TYPE_PSI)
    {
        u4W0 = (UINT32)
                ((1 << 0) |         // PSI
                (1 << 1) |          // Section filter on
                (0xfe << 8) |       // Filter match table, accept all matched
                (46 << 16));        // Max bytes sending to section filter
    }
    else if (prPid->ePidType == DMX_PID_TYPE_PES)
    {
        // PES to memory
        // Turn on filter, no need to setup header buffer
        u4W0 = (UINT32)(1 << 1);
    }
    else
    {
        if (prPid->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
            UINT8 u1Channel;

            // Map pidx to MPV channel
            if (!_DmxAllocateVideoChannel(u1Pidx, &u1Channel))
            {
                return FALSE;
            }
        }

        // Section filter flags
        // bit 0: 1: PSI, 0: PES
        // bit 1: Section filter on
        // bit 2: Output mode
        // bit 4: PID on/off toggle (combined with bit 8 of PID index entry)
        // bit 5: PID disable
        // bit 6: To CD-FIFO
        // bit 7: Enable picture finder
        u4W0 = (UINT32)((1 << 1) | (1 << 6) |
            (((prPid->ePidType == DMX_PID_TYPE_ES_VIDEO) ? 1 : 0) << 7));

        if (prPid->ePidType == DMX_PID_TYPE_ES_VIDEOCLIP)
        {
            u4W0 |= (1 << 2);       // Output entire data
            u4W0 |= (1 << 7);       // Enable picture finder
        }

        // Setup header buffer start address
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(PES_HEADER_BUF_SIZE,
            DMX_HEADER_FIFO_ALIGNMENT);
        if (u4BufStart == 0)
        {
            LOG(3, "%s:%d: Can't allocate memory!\n", __FILE__, __LINE__);
            return FALSE;
        }

        // Header buffer is not calculated in memory usage.
        //_DMX_SetMemoryUsage(u1Pidx, PES_HEADER_BUF_SIZE);

        _DMX_Lock();

        u4BufStart = VIRTUAL(u4BufStart);
        prPidStruct->u4HeaderBufAddr = u4BufStart;
        u4PhyBufStart = PHYSICAL(u4BufStart);
        PID_S_W(u1Pidx, 10) = u4PhyBufStart;    // FIFO start
        PID_S_W(u1Pidx, 12) = u4PhyBufStart;    // Start
        PID_S_W(u1Pidx, 13) = u4PhyBufStart;    // Write pointer
        PID_S_W(u1Pidx, 14) = u4PhyBufStart;    // Read pointer
        prPidStruct->u4NextPic = 0;

        // Header buffer end address
        PID_S_W(u1Pidx, 11) = (u4PhyBufStart + PES_HEADER_BUF_SIZE) - 1;

        _DMX_Unlock();
    }

    _DMX_Lock();
    prPidStruct->u4BufLen = prPid->u4BufSize;
    if (prPid->fgAllocateBuffer && (prPid->ePidType == DMX_PID_TYPE_PSI))
    {
        prPidStruct->u4BufLen = prPid->u4BufSize * DMX_PSI_BUF_TIMES;
    }
    u4BufSize = prPidStruct->u4BufLen;
    _DMX_Unlock();

    // FIFO start address
    if (prPid->fgAllocateBuffer)
    {
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(u4BufSize,
            DMX_ES_FIFO_ALIGNMENT);
        if (u4BufStart == 0)
        {
            VERIFY(BSP_FreeAlignedDmaMemory(
                (void*)prPidStruct->u4HeaderBufAddr));
            // Header buffer is not calculated in memory usage.
            //_DMX_ClearMemoryUsage(u1Pidx, PES_HEADER_BUF_SIZE);

            _DMX_Lock();
            prPidStruct->u4HeaderBufAddr = 0;
            _DMX_Unlock();

            LOG(3, "%s:%d: Can't allocate memory!\n", __FILE__, __LINE__);
            return FALSE;
        }
        _DMX_SetMemoryUsage(u1Pidx, u4BufSize);
    }
    else
    {
        ASSERT(prPid->u4BufAddr != 0);
        u4BufStart = prPid->u4BufAddr;
    }

    _DMX_Lock();

    // Preserve bit 4: PID on/off toggle (combined with bit 8 of PID index)
    PID_S_W(u1Pidx, 0) = (PID_S_W(u1Pidx, 0) & (1 << 4)) | u4W0;

    u4BufStart = VIRTUAL(u4BufStart);
    prPidStruct->u4BufStart = u4BufStart;
    prPidStruct->u4Wp = u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4SectionRp = u4BufStart;
    prPidStruct->u4PesRp = u4BufStart;
    u4PhyBufStart = PHYSICAL(u4BufStart);
    PID_S_W(u1Pidx, 5) = u4PhyBufStart;     // FIFO start
    PID_S_W(u1Pidx, 7) = u4PhyBufStart;     // PES_start
    PID_S_W(u1Pidx, 8) = u4PhyBufStart;     // Write pointer
    PID_S_W(u1Pidx, 9) = u4PhyBufStart;     // Read pointer

    // FIFO end address
    //prPidStruct->u4BufEnd = (u4BufStart + prPid->u4BufSize) - 1;
    //PID_S_W(u1Pidx, 6) = (u4PhyBufStart + prPid->u4BufSize) - 1;
    prPidStruct->u4BufEnd = (u4BufStart + u4BufSize) - 1;
    PID_S_W(u1Pidx, 6) = (u4PhyBufStart + u4BufSize) - 1;

    // FIFO size
    //prPidStruct->u4BufLen = prPid->u4BufSize;

    prPidStruct->fgAllocateBuffer = prPid->fgAllocateBuffer;
    prPidStruct->ePidType = prPid->ePidType;

    _DMX_Unlock();

#ifdef DEBUG_MEMORY_INTRUSION

    if (_DmxIsProtectedMemory(prPidStruct))
    {
        VERIFY(_DmxSetProtectedMemory(u4PhyBufStart, PID_S_W(u1Pidx, 6) + 1));
    }

#endif

    HalFlushInvalidateDCache();

    LOG(6, "Set pixd %u buffer: 0x%08x - 0x%08x\n", u1Pidx, u4BufStart,
        prPidStruct->u4BufEnd);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxFreePidBuffer
 *  Free PID buffer(s)
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxFreePidBuffer(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);
    prPidStruct = &_arPidStruct[u1Pidx];

    LOG(6, "Free pidx %u buffer\n", u1Pidx);

#ifdef DEBUG_MEMORY_INTRUSION

    if (_DmxIsProtectedMemory(prPidStruct))
    {
        VERIFY(_DmxResetProtectedMemory());
    }

#endif  // DEBUG_MEMORY_INTRUSION
    // Make sure the final DMA transaction terminates before freeing PID buffer.
    x_thread_delay(1);

    // Free buffer
    if (prPidStruct->fgAllocateBuffer)
    {
        if (!BSP_FreeAlignedDmaMemory((void*)prPidStruct->u4BufStart))
        {
            LOG(3, "Fail to free PID buffer! pidx: %u, addr: 0x%08x\n",
                u1Pidx, prPidStruct->u4BufStart);
            return FALSE;
        }
        _DMX_ClearMemoryUsage(u1Pidx, prPidStruct->u4BufLen);
    }

    // Free header buffer
    if (!BSP_FreeAlignedDmaMemory((void*)prPidStruct->u4HeaderBufAddr))
    {
        LOG(3, "Fail to free PID header buffer! pidx: %u, addr: 0x%08x\n",
            u1Pidx, prPidStruct->u4HeaderBufAddr);
        return FALSE;
    }
    // Header buffer is not calculated in memory usage.
    //_DMX_ClearMemoryUsage(u1Pidx, PES_HEADER_BUF_SIZE);

    _DMX_Lock();

    // Update PID state
    prPidStruct->fgAllocateBuffer = FALSE;
    prPidStruct->u4BufStart = 0;
    prPidStruct->u4BufEnd = 0;
    prPidStruct->u4BufLen = 0;
    prPidStruct->u4Rp = 0;
    prPidStruct->u4Wp = 0;
    prPidStruct->u4HeaderBufAddr = 0;

    PID_S_W(u1Pidx, 10) = 0x0;
    PID_S_W(u1Pidx, 11) = 0x0;

    _DMX_Unlock();

    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        // Reset pidx-to-channel mapping
        _DMX_FreeVideoChannel(u1Pidx);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxEnablePid
 *  Enable or disable a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  fgEnable        Enable or disable
 *  @param  pu4PidEntry     Pointer to current value of PID index entry
 *  @param  pu4PidWord0     Pointer to the word 0 of PID memory
 *  @param  prPidStruct     Internal PID data structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxEnablePid(UINT8 u1Pidx, BOOL fgEnable, UINT32* pu4PidEntry,
    UINT32* pu4PidWord0, PID_STRUCT_T* prPidStruct, BOOL fgKeepScramble)
{
    UINT32 u4ActiveFlag;
    BOOL fgRet = TRUE;

    ASSERT(prPidStruct != NULL);
    ASSERT(pu4PidEntry != NULL);
    ASSERT(pu4PidWord0 != NULL);

    LOG(6, "%s pidx %u\n", fgEnable ? "Enable" : "Disable", u1Pidx);

    if (fgEnable)
    {
        #ifdef TIME_MEASUREMENT
        if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
            TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, "DMX_FIRST_I", "DMX_EnablePid");
            _fgWaitForFirstI = TRUE;
        }
        #endif  // TIME_MEASUREMENT

        #ifdef CHANNEL_CHANGE_LOG
        HAL_TIME_T dt;
        if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
            SIGNAL rSignal;
            NIM_GetCableSignal(&rSignal);
            HAL_GetTime(&dt);
            LOG(0, " %u.%06u s [AV SYNC] 2 1 DMX Enable V-PID ( 0x%x ), Freq %u\n", dt.u4Seconds, dt.u4Micros, prPidStruct->u2Pid, (UINT32)rSignal.Frequency);
        }
        else
        if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
        {
            HAL_GetTime(&dt);
            LOG(0, " %u.%06u s [AV SYNC] 2 2 DMX Enable A-PID ( 0x%x )\n", dt.u4Seconds, dt.u4Micros, prPidStruct->u2Pid);
        }
        #endif  // CHANNEL_CHANGE_LOG

        if (!fgKeepScramble && (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO))
        {
            VERIFY(DMX_SetInterruptThreshold(DMX_PID_TYPE_ES_VIDEO, 0));
        }

        if (!_DMX_ResetPidBuffer(u1Pidx, prPidStruct, fgKeepScramble))
        {
            return FALSE;
        }

        _DMX_Lock();

        // Read bit 4 of the first word in PID memory
        u4ActiveFlag = (UINT32)((((*pu4PidWord0) & (1 << 4)) != 0) ? 1 : 0);

        // Copy to bit 8 of PID index table
        *pu4PidEntry = ((*pu4PidEntry) & ~(1 << 8)) | (u4ActiveFlag << 8);

        *pu4PidEntry |= 0x80000000;

        prPidStruct->fgEnable = fgEnable;

        _DMX_Unlock();
    }
    else
    {
        _DMX_Lock();

        // Read bit 8 of PID index table
        u4ActiveFlag = (UINT32)(((*pu4PidEntry & (1 << 8)) == 0) ? 1 : 0);

        // Copy inverse to bit 4 of the first word in PID memory
        *pu4PidWord0 = (UINT32)(((*pu4PidWord0) & ~(1 << 4)) | (u4ActiveFlag << 4));

        *pu4PidEntry &= 0x7FFFFFFF;
/*
        // Disable PID index table first
        PID_INDEX_TABLE(u1Pidx) &= 0x7fffffff;
*/
        prPidStruct->fgEnable = fgEnable;

        // Update serial number
        prPidStruct->u1SerialNumber++;
        if (prPidStruct->u1SerialNumber > DMX_MAX_SERIAL_NUM)
        {
            prPidStruct->u1SerialNumber = 0;
        }

        _DMX_Unlock();
    }

    // Notify handler
    _DMX_NotifyEnablePid(u1Pidx, fgEnable);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DmxIsPidEnabled
 *  Is a PID enabled or disabled
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Enabled
 *  @retval FALSE           Disabled
 */
//-----------------------------------------------------------------------------
static BOOL _DmxIsPidEnabled(UINT8 u1Pidx)
{
    BOOL fgActive1, fgActive2;

#ifdef DMX_SUPPORT_DTCP
    PID_STRUCT_T* prPidStruct;
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid PID index: %u\n", u1Pidx);
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_DTCP)
    {
        return TRUE;  // always return TRUE for DLNA/DTCP.
    }
    else
#endif  // DMX_SUPPORT_DTCP
    {
        _DMX_Lock();

        // Bit 4 (PID on/off toggle) of the first word in PID memory
        fgActive1 = ((PID_S_W(u1Pidx, 0) & (1 << 4)) != 0);

        // Bit 8 of PID index table
        fgActive2 = ((PID_INDEX_TABLE(u1Pidx) & (1 << 8)) != 0);

        _DMX_Unlock();
    }

    return ((fgActive1 && fgActive2) || (!fgActive1 && !fgActive2));
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_Lock
 *  Enter demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_Lock(void)
{
    _rDmxLock = x_crit_start();

    ASSERT(!_fgDmxLocking);
    _fgDmxLocking = TRUE;

    UNUSED(_fgDmxLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** _DMX_Unlock
 *  Leave demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_Unlock(void)
{
    ASSERT(_fgDmxLocking);
    _fgDmxLocking = FALSE;

    x_crit_end(_rDmxLock);

    UNUSED(_fgDmxLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** _DMX_AllocateVideoChannel
 *  Allocate a video channel for a given pid index
 *
 *  @param  u1Pidx          PID index
 *  @param  pu1Channel[output]
 *                          The allocated channel
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_AllocateVideoChannel(UINT8 u1Pidx, UINT8* pu1Channel)
{
    return _DmxAllocateVideoChannel(u1Pidx, pu1Channel);
}


//-----------------------------------------------------------------------------
/** _DMX_FreeVideoChannel
 *  Release mapping of a given pidx and its corresponding video channel
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_FreeVideoChannel(UINT8 u1Pidx)
{
    UINT32 i;

    _DMX_Lock();

    for (i = 0; i < MPV_MAX_ES; i++)
    {
        if (_rDmxStates.au1VideoPidx[i] == u1Pidx)
        {
            // Found the pidx
            // Note only video channels can be found
            _rDmxStates.au1VideoPidx[i] = DMX_NULL_PIDX;
            break;
        }
    }

    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_GetVideoChannel
 *  Get corresponding video channel of a given pid index
 *
 *  @param  u1Pid           PID index
 *  @param  pu1Channel [output]
 *                          The channel
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetVideoChannel(UINT8 u1Pidx, UINT8* pu1Channel)
{
    UINT8 i;

    ASSERT(pu1Channel != NULL);

    for (i = 0; i < MPV_MAX_ES; i++)
    {
        if (_rDmxStates.au1VideoPidx[i] == u1Pidx)
        {
            *pu1Channel = i;
            return TRUE;
        }
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetVideoPidx
 *  Get corresponding pid index of a given video channel
 *
 *  @param  u1Channel       Channel index
 *  @param  pu1Pidx [output]
 *                          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetVideoPidx(UINT8 u1Channel, UINT8* pu1Pidx)
{
    ASSERT(pu1Pidx != NULL);

    if (u1Channel >= MPV_MAX_ES)
    {
        return FALSE;
    }

    if (_rDmxStates.au1VideoPidx[u1Channel] == DMX_NULL_PIDX)
    {
        return FALSE;
    }

    ASSERT(_rDmxStates.au1VideoPidx[u1Channel] < DMX_NUM_PID_INDEX);
    *pu1Pidx = _rDmxStates.au1VideoPidx[u1Channel];

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetPidStruct
 *  Get the PID structure of a given PID index
 *
 *  @param  u4PidIndex      PID index
 *
 *  @retval The pointer of the PID structure
 */
//-----------------------------------------------------------------------------
PID_STRUCT_T* _DMX_GetPidStruct(UINT32 u4PidIndex)
{
    ASSERT(u4PidIndex < DMX_NUM_PID_INDEX);

    return &_arPidStruct[u4PidIndex];
}


//-----------------------------------------------------------------------------
/** _DMX_SetFilterType
 */
//-----------------------------------------------------------------------------
void _DMX_SetFilterType(FILTER_TYPE_T eType)
{
    _eFilterType = eType;
}


//-----------------------------------------------------------------------------
/** _DMX_GetFilterType
 */
//-----------------------------------------------------------------------------
FILTER_TYPE_T _DMX_GetFilterType(void)
{
    return _eFilterType;
}


//-----------------------------------------------------------------------------
/** _DMX_SetFilter
 *  Setup a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u1Bank          Filter bank (0 - 1)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetFilter(UINT8 u1FilterIndex, UINT8 u1Bank, UINT32 u4Flags,
    const DMX_FILTER_T* prFilter)
{
    PID_STRUCT_T *prPidStruct;

    ASSERT(prFilter != NULL);

    _DMX_SetFilterType(FILTER_TYPE_LEGACY);

    // Check parameters
    if ((u1FilterIndex >= DMX_NUM_FILTER_INDEX) ||
        (u1Bank >= DMX_NUM_FILTER_BANK))
    {
        return FALSE;
    }

    if (u4Flags == DMX_FILTER_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();

    // PID index
    if ((u4Flags & DMX_FILTER_FLAG_PIDX) != 0)
    {
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].u1Pidx = prFilter->u1Pidx;
    }

    // Offset
    if ((u4Flags & DMX_FILTER_FLAG_OFFSET) != 0)
    {
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].u1Offset = prFilter->u1Offset;
    }

    // Check CRC
    if ((u4Flags & DMX_FILTER_FLAG_CRC) != 0)
    {
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].fgCheckCrc = prFilter->fgCheckCrc;
    }

    // Filter mode
    if ((u4Flags & DMX_FILTER_FLAG_MODE) != 0)
    {
        if(u1Bank == 0)
        {
            _arFilterStruct[u1FilterIndex].eMode = prFilter->eMode;
        }
        else
        {
            if(_arFilterStruct[u1FilterIndex].eMode != prFilter->eMode)
            {
                LOG(1, "%d, Sect filter mode not match\n", u1FilterIndex);
                ASSERT(0);
            }
        }
    }

    // Pattern
    if ((u4Flags & DMX_FILTER_FLAG_PATTERN) != 0)
    {
        // Set pattern and mask
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[0] = GET_BYTE(prFilter->au4Mask[0], 3);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[1] = GET_BYTE(prFilter->au4Mask[0], 2);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[2] = GET_BYTE(prFilter->au4Mask[0], 1);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[3] = GET_BYTE(prFilter->au4Mask[0], 0);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[4] = GET_BYTE(prFilter->au4Mask[1], 3);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[5] = GET_BYTE(prFilter->au4Mask[1], 2);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[6] = GET_BYTE(prFilter->au4Mask[1], 1);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[7] = GET_BYTE(prFilter->au4Mask[1], 0);

        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[0] = GET_BYTE(prFilter->au4Data[0], 3);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[1] = GET_BYTE(prFilter->au4Data[0], 2);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[2] = GET_BYTE(prFilter->au4Data[0], 1);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[3] = GET_BYTE(prFilter->au4Data[0], 0);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[4] = GET_BYTE(prFilter->au4Data[1], 3);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[5] = GET_BYTE(prFilter->au4Data[1], 2);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[6] = GET_BYTE(prFilter->au4Data[1], 1);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[7] = GET_BYTE(prFilter->au4Data[1], 0);

        if (u1Bank == 0)
        {
            if (u1FilterIndex % 2)
            {
                PID_S_W(34, u1FilterIndex >> 1) &= 0x0000ffff;
                PID_S_W(34, u1FilterIndex >> 1) |= ((prFilter->au4Mask[0] >> 24) << 24);
                PID_S_W(34, u1FilterIndex >> 1) |= ((prFilter->au4Data[0] >> 24) << 16);
            }
            else
            {
                PID_S_W(34, u1FilterIndex >> 1) &= 0xffff0000;
                PID_S_W(34, u1FilterIndex >> 1) |= ((prFilter->au4Mask[0] >> 24) << 8);
                PID_S_W(34, u1FilterIndex >> 1) |= ((prFilter->au4Data[0] >> 24) << 0);
            }
        }
    }

    // Valid
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        prPidStruct = &(_arPidStruct[_arFilterStruct[u1FilterIndex].arBank[u1Bank].u1Pidx]);
        _arFilterStruct[u1FilterIndex].arBank[u1Bank].fgEnable = prFilter->fgEnable;

        if (prFilter->fgEnable)
        {
            prPidStruct->u1SecCount++;
            
            if(prPidStruct->u1SecCount == 1)
            {
                prPidStruct->u1SecIdx = u1FilterIndex;
            }
            PID_S_W(_arFilterStruct[u1FilterIndex].arBank[u1Bank].u1Pidx, 11) |=
                                (_arFilterStruct[u1FilterIndex].arBank[u1Bank].fgCheckCrc ? 1 : 0) << u1FilterIndex;
            PID_S_W(_arFilterStruct[u1FilterIndex].arBank[u1Bank].u1Pidx, 10) |=
                                (1 << u1FilterIndex);
        }
        else
        {
            if (prPidStruct->u1SecCount > 0)
            {
                prPidStruct->u1SecCount--;
            }
            PID_S_W(_arFilterStruct[u1FilterIndex].arBank[u1Bank].u1Pidx, 10) &=
                                ~(UINT32)(1 << u1FilterIndex);
            PID_S_W(_arFilterStruct[u1FilterIndex].arBank[u1Bank].u1Pidx, 11) &=
                                ~(UINT32)(1 << u1FilterIndex);
        }
    }

    _DMX_Unlock();

    // Debug
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        LOG(6, "%s filter %u bank %u\n", prFilter->fgEnable ? "Enable" : "Disable",
            u1FilterIndex, u1Bank);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetFilter
 *  Get a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u1Bank          Filter bank (0 - 1)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetFilter(UINT8 u1FilterIndex, UINT8 u1Bank, UINT32 u4Flags,
    DMX_FILTER_T* prFilter)
{
    ASSERT(prFilter != NULL);

    // Check parameters
    if ((u1FilterIndex >= DMX_NUM_FILTER_INDEX) ||
        (u1Bank >= DMX_NUM_FILTER_BANK))
    {
        return FALSE;
    }

    if (u4Flags == DMX_FILTER_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();

    // PID index
    if ((u4Flags & DMX_FILTER_FLAG_PIDX) != 0)
    {
        prFilter->u1Pidx = _arFilterStruct[u1FilterIndex].arBank[u1Bank].u1Pidx;
    }

    // Offset
    if ((u4Flags & DMX_FILTER_FLAG_OFFSET) != 0)
    {
        prFilter->u1Offset= _arFilterStruct[u1FilterIndex].arBank[u1Bank].u1Offset;
    }

    // Check CRC
    if ((u4Flags & DMX_FILTER_FLAG_CRC) != 0)
    {
        prFilter->fgCheckCrc = _arFilterStruct[u1FilterIndex].arBank[u1Bank].fgCheckCrc;
    }

    // Valid
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        prFilter->fgEnable= _arFilterStruct[u1FilterIndex].arBank[u1Bank].fgEnable;
    }

    // Filter mode
    if ((u4Flags & DMX_FILTER_FLAG_MODE) != 0)
    {
        prFilter->eMode = _arFilterStruct[u1FilterIndex].eMode;
    }

    // Pattern
    if ((u4Flags & DMX_FILTER_FLAG_PATTERN) != 0)
    {
        prFilter->au4Data[0] = ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[0] << 24) |
                               ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[1] << 16) |
                               ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[2] << 8) |
                               (UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[3];
        prFilter->au4Data[1] = ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[4] << 24) |
                               ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[5] << 16) |
                               ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[6] << 8) |
                               (UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Data[7];

        prFilter->au4Mask[0] = ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[0] << 24) |
                               ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[1] << 16) |
                               ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[2] << 8) |
                               (UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[3];
        prFilter->au4Mask[1] = ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[4] << 24) |
                               ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[5] << 16) |
                               ((UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[6] << 8) |
                               (UINT32)_arFilterStruct[u1FilterIndex].arBank[u1Bank].au1Mask[7];
    }

    _DMX_Unlock();

    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        LOG(6, "%s filter %u bank %u\n", prFilter->fgEnable ? "Enable" : "Disable",
            u1FilterIndex, u1Bank);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetGenFilter
 *  Setup a generic section filter
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                              const DMX_FILTER_GENERIC_T* prFilter)
{
    PID_STRUCT_T *prPidStruct;
    UINT32 i;

    ASSERT(prFilter != NULL);

    _DMX_SetFilterType(FILTER_TYPE_GENERIC);

    // Check parameters
    if (u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
        return FALSE;
    }

    if (u4Flags == DMX_FILTER_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();

    // Always set to generic mode
    _arFilterStruct[u1FilterIndex].eMode = DMX_FILTER_MODE_GENERIC;

    // PID index
    if ((u4Flags & DMX_FILTER_FLAG_PIDX) != 0)
    {
        _arFilterStruct[u1FilterIndex].rGeneric.u1Pidx = prFilter->u1Pidx;
    }

    // Offset
    if ((u4Flags & DMX_FILTER_FLAG_OFFSET) != 0)
    {
        _arFilterStruct[u1FilterIndex].rGeneric.u1Offset = prFilter->u1Offset;
    }

    // Check CRC
    if ((u4Flags & DMX_FILTER_FLAG_CRC) != 0)
    {
        _arFilterStruct[u1FilterIndex].rGeneric.fgCheckCrc = prFilter->fgCheckCrc;
    }

    // Pattern
    if ((u4Flags & DMX_FILTER_FLAG_PATTERN) != 0)
    {
        for(i=0; i<16; i++)
        {
            _arFilterStruct[u1FilterIndex].rGeneric.au1Mask[i] = prFilter->au1Mask[i];
            _arFilterStruct[u1FilterIndex].rGeneric.au1Data[i] = prFilter->au1Data[i];
            _arFilterStruct[u1FilterIndex].rGeneric.au1PosNeg[i] = prFilter->au1PosNeg[i];
        }

        if (u1FilterIndex % 2)
        {
            PID_S_W(34, u1FilterIndex >> 1) &= 0x0000ffff;
            PID_S_W(34, u1FilterIndex >> 1) |= ((UINT32)(prFilter->au1Mask[0]) << 24);
            PID_S_W(34, u1FilterIndex >> 1) |= ((UINT32)(prFilter->au1Data[0]) << 16);
        }
        else
        {
            PID_S_W(34, u1FilterIndex >> 1) &= 0xffff0000;
            PID_S_W(34, u1FilterIndex >> 1) |= ((UINT32)(prFilter->au1Mask[0]) << 8);
            PID_S_W(34, u1FilterIndex >> 1) |= ((UINT32)(prFilter->au1Data[0]) << 0);
        }
    }

    // Valid
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        prPidStruct = &_arPidStruct[_arFilterStruct[u1FilterIndex].rGeneric.u1Pidx];

        _arFilterStruct[u1FilterIndex].rGeneric.fgEnable = prFilter->fgEnable;

        if (prFilter->fgEnable)
        {
            prPidStruct->u1SecCount++;

            if(prPidStruct->u1SecCount == 1)
            {
                prPidStruct->u1SecIdx = u1FilterIndex;
            }

            PID_S_W(_arFilterStruct[u1FilterIndex].rGeneric.u1Pidx, 11) |=
                                ((_arFilterStruct[u1FilterIndex].rGeneric.fgCheckCrc ? 1 : 0) << u1FilterIndex);
            PID_S_W(_arFilterStruct[u1FilterIndex].rGeneric.u1Pidx, 10) |=
                                (1 << u1FilterIndex);
        }
        else
        {
            if (prPidStruct->u1SecCount > 0)
            {
                prPidStruct->u1SecCount--;
            }
            PID_S_W(_arFilterStruct[u1FilterIndex].rGeneric.u1Pidx, 10) &=
                                ~(UINT32)(1 << u1FilterIndex);
            PID_S_W(_arFilterStruct[u1FilterIndex].rGeneric.u1Pidx, 11) &=
                                ~(UINT32)(1 << u1FilterIndex);
        }
    }

    _DMX_Unlock();

    // Debug
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        LOG(6, "%s filter %u\n", prFilter->fgEnable ? "Enable" : "Disable",
            u1FilterIndex);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetGenFilter
 *  Get a section filter
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                DMX_FILTER_GENERIC_T* prFilter)
{
    UINT32 i;

    ASSERT(prFilter != NULL);

    // Check parameters
    if(u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
        return FALSE;
    }

    if (u4Flags == DMX_FILTER_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();

    // PID index
    if ((u4Flags & DMX_FILTER_FLAG_PIDX) != 0)
    {
        prFilter->u1Pidx = _arFilterStruct[u1FilterIndex].rGeneric.u1Pidx;
    }

    // Offset
    if ((u4Flags & DMX_FILTER_FLAG_OFFSET) != 0)
    {
        prFilter->u1Offset= _arFilterStruct[u1FilterIndex].rGeneric.u1Offset;
    }

    // Check CRC
    if ((u4Flags & DMX_FILTER_FLAG_CRC) != 0)
    {
        prFilter->fgCheckCrc = _arFilterStruct[u1FilterIndex].rGeneric.fgCheckCrc;
    }

    // Valid
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        prFilter->fgEnable = _arFilterStruct[u1FilterIndex].rGeneric.fgEnable;
    }

    // Filter mode
    if ((u4Flags & DMX_FILTER_FLAG_MODE) != 0)
    {
        //prFilter->eMode = DMX_FILTER_MODE_GENERIC;
    }

    // Pattern
    if ((u4Flags & DMX_FILTER_FLAG_PATTERN) != 0)
    {
        for(i=0; i<16; i++)
        {
            prFilter->au1Mask[i] = _arFilterStruct[u1FilterIndex].rGeneric.au1Mask[i];
            prFilter->au1Data[i] = _arFilterStruct[u1FilterIndex].rGeneric.au1Data[i];
            prFilter->au1PosNeg[i] = _arFilterStruct[u1FilterIndex].rGeneric.au1PosNeg[i];
        }
    }

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetFilterStruct
 *  Get the Filter structure of a given filter index
 *
 *  @param  u4FilterIndex      filter index
 *  @retval The pointer of the filter structure
 */
//-----------------------------------------------------------------------------
FILTER_STRUCT_T* _DMX_GetFilterStruct(UINT32 u4FilterIndex)
{
    ASSERT(u4FilterIndex < DMX_NUM_FILTER_INDEX);

    return &_arFilterStruct[u4FilterIndex];
}


//-----------------------------------------------------------------------------
/** _DMX_SetPid
 *  Set a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid, BOOL fgKeepScrmble)
{
    PID_STRUCT_T rPidStruct;
    UINT32 u4PidEntry, u4PidWord0 = 0;
    BOOL fgPidWord0Modified = FALSE;
#ifndef __MODEL_slt__
    BOOL fgEnable;
    UINT32 i;
#endif  // __MODEL_slt__

    ASSERT(prPid != NULL);

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid PID index: %u\n", u1Pidx);
        return FALSE;
    }

    if (u4Flags == DMX_PID_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();

    rPidStruct = _arPidStruct[u1Pidx];

#if 0
    // Check if it's already active
    if (rPidStruct.fgEnable)
    {
        // What can be done to an active pid is disabling it only
        if ((u4Flags & DMX_PID_FLAG_VALID) == 0)
        {
            _DMX_Unlock();
            LOG(3, "Error: try to change attributes of the active pidx %u\n",
                u1Pidx);
            return FALSE;
        }
    }
#endif

    // Get current PID settings
    u4PidEntry = PID_INDEX_TABLE(u1Pidx);

    // Ask uP to stop first
    // TBD...

    // Disable PID first
    PID_INDEX_TABLE(u1Pidx) &= 0x7fffffff;

    _DMX_Unlock();

    // TS index
    if ((u4Flags & DMX_PID_FLAG_TS_INDEX) != 0)
    {
        // Check parameter
        if (prPid->u1TsIndex > DMX_MAX_TUNER_INDEX)
        {
            LOG(3, "Invalid TS index: %u\n", prPid->u1TsIndex);
            return FALSE;
        }

        u4PidEntry &= 0x9fffffff;
        u4PidEntry |= (prPid->u1TsIndex & 0x3) << 29;
        rPidStruct.u1TsIndex = prPid->u1TsIndex & 0x3;
    }

    // PID
    if ((u4Flags & DMX_PID_FLAG_PID) != 0)
    {
        // Check parameter
        if (prPid->u2Pid > 0x1fff)
        {
            LOG(3, "Invalid PID value: 0x%x\n", prPid->u2Pid);
            return FALSE;
        }

#ifdef CC_DMX_USE_CT
        // When users want to set BPS_PID as the PID value, the PID value is
        // not set into the hardware "PID Filter".  Instead, the PID value in
        // the PID Filter is set to the PID value of a null packet (0x1FFF).
        // The software records such an event.
        if (prPid->u2Pid == BPS_PID)
        {
            u4PidEntry = (u4PidEntry & 0xe000ffff) | (0x1FFF << 16);
            rPidStruct.u2Pid = prPid->u2Pid;
        }
        else
#endif  // CC_DMX_USE_CT
        {
            u4PidEntry &= 0xe000ffff;
            u4PidEntry |= (prPid->u2Pid & 0x1fff) << 16;
            rPidStruct.u2Pid = prPid->u2Pid & 0x1fff;
        }
    }

    // PCR
    if ((u4Flags & DMX_PID_FLAG_PCR) != 0)
    {
        BOOL fgHasPcr = FALSE, fgPcrNewMode = FALSE;

        switch (prPid->ePcrMode)
        {
        case DMX_PCR_MODE_OLD:
            fgHasPcr = TRUE;
            break;

        case DMX_PCR_MODE_NEW:
            fgHasPcr = TRUE;
            fgPcrNewMode = TRUE;
            break;

        case DMX_PCR_MODE_NONE:
        default:
            break;
        }

        u4PidEntry &= 0xffff7ff7;
        u4PidEntry |= ((fgHasPcr ? 1 : 0) << 15) |
            ((fgPcrNewMode ? 0 : 1) << 3);
        rPidStruct.ePcrMode = prPid->ePcrMode;
    }

    // Buffer
    if ((u4Flags & DMX_PID_FLAG_BUFFER) != 0)
    {
        if (!_DmxSetPidBuffer(u1Pidx, prPid, &rPidStruct))
        {
            return FALSE;
        }
    }

    // Steering
    if ((u4Flags & DMX_PID_FLAG_STEER) != 0)
    {
        UINT32 u4Steering;
        BOOL fgTo1394, fgToPvr, fgToFTuP;

        fgTo1394 = ((prPid->u1SteerMode & DMX_STEER_TO_1394) != 0);
        fgToPvr = ((prPid->u1SteerMode & DMX_STEER_TO_PVR) != 0);
        fgToFTuP = ((prPid->u1SteerMode & DMX_STEER_TO_FTUP) != 0);

        u4Steering = (UINT32)(((fgTo1394 ? 1 : 0) << 7) | ((fgToPvr ? 1 : 0) << 6) |
            ((fgToFTuP ? 1 : 0) << 5));

        u4PidEntry = (u4PidEntry & 0xffffff0f) | u4Steering;
        rPidStruct.u1SteerMode = prPid->u1SteerMode;
    }

    // Descramble mode
    if ((u4Flags & DMX_PID_FLAG_DESC_MODE) != 0)
    {
        u4PidEntry &= 0xffffc3ff;
        u4PidEntry |= (((UINT8)prPid->eDescMode) & 0xf) << 10;
        rPidStruct.eDescMode = prPid->eDescMode;
    }

    // Key index
    if ((u4Flags & DMX_PID_FLAG_KEY_INDEX) != 0)
    {
        u4PidEntry &= 0xfffffff8;
        u4PidEntry |= prPid->u1KeyIndex & 0x7;
        rPidStruct.u1KeyIndex = prPid->u1KeyIndex & 0x7;
    }

    // Device ID
    if ((u4Flags & DMX_PID_FLAG_DEVICE_ID) != 0)
    {
        rPidStruct.u1DeviceId = prPid->u1DeviceId;
    }

    // Notification callback function
    if ((u4Flags & DMX_PID_FLAG_CALLBACK) != 0)
    {
        rPidStruct.pfnNotify = prPid->pfnNotify;
        rPidStruct.pvNotifyTag = prPid->pvNotifyTag;
    }

    // Scramble state callback function
    if ((u4Flags & DMX_PID_FLAG_SCRAMBLE_STATE) != 0)
    {
        rPidStruct.pfnScramble = prPid->pfnScramble;
        rPidStruct.pvScrambleTag = prPid->pvScrambleTag;
    }

    // Primary PID
    if ((u4Flags & DMX_PID_FLAG_PRIMARY) != 0)
    {
        if (prPid->fgPrimary)
        {
            u4PidEntry |= (1 << 14);
        }
        else
        {
            u4PidEntry &= ~(1 << 14);
        }

        rPidStruct.fgPrimary = prPid->fgPrimary;
    }

    // Valid bit
    if ((u4Flags & DMX_PID_FLAG_VALID) != 0)
    {
        if ((rPidStruct.fgEnable && !prPid->fgEnable) ||
            (!rPidStruct.fgEnable && prPid->fgEnable))
        {
#ifndef __MODEL_slt__
            // Framer reset may cause rare error in SLT data comparison.
            //-----------------------------------------
            // Reset framer
            //-----------------------------------------
            if(prPid->fgEnable)
            {
                for(i=0; i<DMX_NUM_PID_INDEX; i++)
                {
                    _DMX_Lock();
                    fgEnable = _arPidStruct[i].fgEnable;
                    _DMX_Unlock();

                    if(fgEnable)
                    {
                        break;
                    }
                }

                if(i == DMX_NUM_PID_INDEX)
                {
                    _DMX_ResetFramer();
                }
            }
#endif  //__MODEL_slt__

            u4PidEntry &= 0x7fffffff;
#ifdef CC_DMX_USE_CT
            if (rPidStruct.u2Pid != BPS_PID)
#endif  // CC_DMX_USE_CT
            {
                u4PidEntry |= ((prPid->fgEnable ? 1 : 0) << 31);
            }
            u4PidWord0 = PID_S_W(u1Pidx, 0);
            fgPidWord0Modified = TRUE;

            if (!_DmxEnablePid(u1Pidx, prPid->fgEnable, &u4PidEntry,
                &u4PidWord0, &rPidStruct, fgKeepScrmble))
            {
                return FALSE;
            }
#ifdef CC_DMX_USE_CT
#define DMX_DRIVER_WITH_AW
            // Be aware of the scenario of capturing bit stream on demand.
            if (rPidStruct.u2Pid == BPS_PID)
            {
                UINT8 u1TsIndex;
                UINT32 u4Register, u4Value;

                // Always turn off the Valid bit if PID value is BPS_PID.
                u4PidEntry &= 0x7FFFFFFF;

                u1TsIndex = rPidStruct.u1TsIndex;
                u4Register = _Dmx_GetBypassPidRegWordOffset(u1TsIndex);
                _DMX_Lock();
                if (prPid->fgEnable)
                {
                    // The "To Microprocessor" flag should have been set in u4PidEntry.
                    u4Value = u4PidEntry & 0xFFFF;
                    u4Value |= u1Pidx << 16;
                    DMXCMD_WRITE32(u4Register, u4Value);

                    CT_SETTING = (CT_SETTING & 0x00FFFFFF) | ((UINT32)u1Pidx << 24);

                    // Turn on the Bypass mode after setting the appropriate DBM_BYPASS_PID register.
                    u4Value = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
                    u4Value |= 1 << (26 + u1TsIndex);
                    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Value);
                }
                else
                {
                    CT_SETTING = (CT_SETTING & 0x00FFFFFF) | 0xFF000000;

                    u4Value = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
                    u4Value &= ~(1 << (26 + u1TsIndex));
                    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Value);

                    DMXCMD_WRITE32(u4Register, 0);
                }
                _DMX_Unlock();
            }
#endif  // CC_DMX_USE_CT
        }
    }

    // Update PID index table
    _DMX_Lock();
    PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
    if (fgPidWord0Modified)
    {
        PID_S_W(u1Pidx, 0) = u4PidWord0;
    }
    _arPidStruct[u1Pidx] = rPidStruct;
    _DMX_Unlock();

    LOG(7, "PidTable[%u] = 0x%08x\n", u1Pidx, u4PidEntry);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetPid
 *  Get a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_PID_T* prPid)
{
    PID_STRUCT_T* prPidStruct;
    ASSERT(prPid != NULL);

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid PID index: %u\n", u1Pidx);
        return FALSE;
    }

    if (u4Flags == DMX_PID_FLAG_NONE)
    {
        return TRUE;
    }

    prPidStruct = &_arPidStruct[u1Pidx];

    // TS index
    if ((u4Flags & DMX_PID_FLAG_TS_INDEX) != 0)
    {
        prPid->u1TsIndex = prPidStruct->u1TsIndex;
    }

    // PID
    if ((u4Flags & DMX_PID_FLAG_PID) != 0)
    {
        prPid->u2Pid = prPidStruct->u2Pid;
    }

    // PCR
    if ((u4Flags & DMX_PID_FLAG_PCR) != 0)
    {
        prPid->ePcrMode = prPidStruct->ePcrMode;
    }

    // Buffer
    if ((u4Flags & DMX_PID_FLAG_BUFFER) != 0)
    {
        prPid->fgAllocateBuffer = prPidStruct->fgAllocateBuffer;
        prPid->u4BufAddr = prPidStruct->u4BufStart;
        prPid->u4BufSize = prPidStruct->u4BufLen;
        prPid->ePidType = prPidStruct->ePidType;
    }

    // Steering
    if ((u4Flags & DMX_PID_FLAG_STEER) != 0)
    {
        prPid->u1SteerMode = prPidStruct->u1SteerMode;
    }

    // Descramble mode
    if ((u4Flags & DMX_PID_FLAG_DESC_MODE) != 0)
    {
        prPid->eDescMode = prPidStruct->eDescMode;
    }

    // Key index
    if ((u4Flags & DMX_PID_FLAG_KEY_INDEX) != 0)
    {
        prPid->u1KeyIndex = prPidStruct->u1KeyIndex;
    }

    // Device ID
    if ((u4Flags & DMX_PID_FLAG_DEVICE_ID) != 0)
    {
        prPid->u1DeviceId = prPidStruct->u1DeviceId;
    }

    // Notification callback function
    if ((u4Flags & DMX_PID_FLAG_CALLBACK) != 0)
    {
        prPid->pfnNotify = prPidStruct->pfnNotify;
        prPid->pvNotifyTag = prPidStruct->pvNotifyTag;
    }

    // Data pointers
    if ((u4Flags & DMX_PID_FLAG_DATA_POINTERS) != 0)
    {
        prPid->u4Rp = VIRTUAL(prPidStruct->u4Rp);
        prPid->u4Wp = VIRTUAL(prPidStruct->u4Wp);
        prPid->u4PeakBufFull = prPidStruct->u4PeakBufFull;
    }

    // Primary PID
    if ((u4Flags & DMX_PID_FLAG_PRIMARY) != 0)
    {
        prPid->fgPrimary = prPidStruct->fgPrimary;
    }

    // Valid
    if ((u4Flags & DMX_PID_FLAG_VALID) != 0)
    {
        prPid->fgEnable = prPidStruct->fgEnable;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_FreePid
 *  Free a PID
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_FreePid(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;
    UINT8 u1SerialNumber;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    prPidStruct = &_arPidStruct[u1Pidx];
    if (prPidStruct->fgEnable)
    {
        UINT32 u4PidEntry, u4PidWord0;

        LOG(3, "Try to free an active PID! (pidx: %u)\n", u1Pidx);

        // Stop PID
        u4PidEntry = PID_INDEX_TABLE(u1Pidx);
        u4PidWord0 = PID_S_W(u1Pidx, 0);
        if (!_DmxEnablePid(u1Pidx, FALSE, &u4PidEntry, &u4PidWord0, prPidStruct, FALSE))
        {
            return FALSE;
        }

        // Actual stop PID
        _DMX_Lock();
        PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
        PID_S_W(u1Pidx, 0) = u4PidWord0;
#ifdef CC_DMX_USE_CT
        if (prPidStruct->u2Pid == BPS_PID)
        {
            UINT8 u1TsIndex;
            UINT32 u4Register, u4Value;

            CT_SETTING = (CT_SETTING & 0x00FFFFFF) | 0xFF000000;

            u1TsIndex = prPidStruct->u1TsIndex;
            u4Register = _Dmx_GetBypassPidRegWordOffset(u1TsIndex);

            u4Value = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
            u4Value &= ~(1 << (26 + u1TsIndex));
            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Value);

            DMXCMD_WRITE32(u4Register, 0);
        }
#endif  // CC_DMX_USE_CT
        _DMX_Unlock();
    }

    // Stop and clear PID index table except for the Toggle bit.
    _DMX_Lock();
    PID_INDEX_TABLE(u1Pidx) = PID_INDEX_TABLE(u1Pidx) & (1 << 8);
    _DMX_Unlock();

    // Flush MPV
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        UINT8 u1Channel;

        // Sync with MPV
        VERIFY(_DMX_GetVideoChannel(u1Pidx, &u1Channel));
#ifndef CC_EMULATION
        MPV_FlushEsmQ(u1Channel, TRUE);
#endif

        // No need to send purge message if serail number is applied
        // Discard intermediate pictures, clean FIFO
//        VERIFY(_DMX_SendPurgeMessage(u1Pidx, FALSE));
    }
    else if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        // FIXME
        if(!_DMX_GetIsPanicReset())
        {
            AUD_WaitDspFlush(prPidStruct->u1DeviceId);
        }

        // No need to send purge message if serial number is applied
        // Discard intermediate frames, clean FIFO
//        VERIFY(_DMX_SendPurgeMessage(u1Pidx, FALSE));
    }

    // Free buffer
    if (!_DmxFreePidBuffer(u1Pidx))
    {
        return FALSE;
    }

    _DMX_Lock();

    // Clear PID structures and PID memory
    // Note that serial number must be preserved across freeing PID
    // Also, peak buffer fullness is stored while channel changed
    u1SerialNumber = prPidStruct->u1SerialNumber;
    x_memset((void*)prPidStruct, 0, sizeof (PID_STRUCT_T));
    prPidStruct->u1SerialNumber = u1SerialNumber;

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_FreePidBuffer(UINT8 u1Pidx)
 *  Free PID buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_FreePidBuffer(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    prPidStruct = &_arPidStruct[u1Pidx];
    if (prPidStruct->fgEnable)
    {
        LOG(3, "Try to free buffer of active PID! (pidx: %u)\n", u1Pidx);
        return FALSE;
    }

    return _DmxFreePidBuffer(u1Pidx);
}


//-----------------------------------------------------------------------------
/** _DMX_ResetPidBuffer
 *  Reset PID buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  prPidStruct     Internal PID data structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ResetPidBuffer(UINT8 u1Pidx, PID_STRUCT_T* prPidStruct, BOOL fgKeepScramble)
{
    UINT32 u4BufStart, u4OrgScrambleState;
    BOOL fgRet = TRUE;

    FUNC_ENTRY;

    ASSERT(prPidStruct != NULL);

    _DMX_Lock();

    // Check if PID is inactive
    if ((prPidStruct->fgEnable) ||
        ((PID_INDEX_TABLE(u1Pidx) & 0x80000000) != 0))
    {
        _DMX_Unlock();
        LOG(3, "Reset buffer of an active pid\n");
        return FALSE;
    }

    // Setup PID structures
    PID_S_W(u1Pidx, 1) = 0;

    if(fgKeepScramble)
    {
        u4OrgScrambleState = PID_S_W(u1Pidx, 2);
        PID_S_W(u1Pidx, 2) = (u4OrgScrambleState & 0x0000FF00);
    }
    else
    {
        PID_S_W(u1Pidx, 2) = 0x0000ff00;        // Scramble state = unknown
    }

    PID_S_W(u1Pidx, 3) = 0xffff0001;        // Default for PES
    PID_S_W(u1Pidx, 4) = 0xffffffff;

    // Reset payload FIFO
    u4BufStart = prPidStruct->u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4Wp = u4BufStart;
    u4BufStart = PHYSICAL(u4BufStart);
    PID_S_W(u1Pidx, 7) = u4BufStart;        // PES_start
    PID_S_W(u1Pidx, 8) = u4BufStart;        // Write pointer
    PID_S_W(u1Pidx, 9) = u4BufStart;        // Read pointer

    // Set a small threshold for audio type, added from v2.16
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        // uCode doesn't handle the case that partial PES threshold is
        // reached but PES header still incomplete. So the minimum
        // threshold is 2 (PES_header_length field is 8-bit in PES header)
//        ASSERT(DMX_INT_THRESHOLD_AUDIO > 1);

        PID_S_W(u1Pidx, 15) = _u2IntThresholdAudio;
    }
    else if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        PID_S_W(u1Pidx, 15) = _u2IntThresholdVideo;
    }
    else
    {
        PID_S_W(u1Pidx, 15) = 0;            // 0: No partial PES interrupt
    }

    // Extra checking/setting for different PID types
    //
    switch (prPidStruct->ePidType)
    {
    case DMX_PID_TYPE_PSI:
        PID_S_W(u1Pidx, 3) = 0xffff0002;
        prPidStruct->u4SectionRp = prPidStruct->u4BufStart;
        break;

    case DMX_PID_TYPE_ES_VIDEO:
    case DMX_PID_TYPE_ES_AUDIO:
    case DMX_PID_TYPE_ES_VIDEOCLIP:
    case DMX_PID_TYPE_ES_OTHER:
        // Reset header FIFO
        u4BufStart = PHYSICAL(prPidStruct->u4HeaderBufAddr);
        PID_S_W(u1Pidx, 12) = u4BufStart;   // Start
        PID_S_W(u1Pidx, 13) = u4BufStart;   // Write pointer
        PID_S_W(u1Pidx, 14) = u4BufStart;   // Read pointer
        prPidStruct->u4NextPic = 0;
        prPidStruct->u4PesRp = prPidStruct->u4BufStart;
        break;

    case DMX_PID_TYPE_PES:
        prPidStruct->u4PesRp = prPidStruct->u4BufStart;
        break;

    case DMX_PID_TYPE_ES_DTCP:
    case DMX_PID_TYPE_NONE:
        break;

    default:
        fgRet = FALSE;
        break;
    }

    _DMX_Unlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_FlushPidBuffer(UINT8 u1Pidx)
 *  Flush PID buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_FlushPidBuffer(UINT8 u1Pidx)
{
    PID_STRUCT_T rPidStruct;
    UINT32 u4PidEntry, u4PidWord0, u4BufStart;
    BOOL fgEnable;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);
    rPidStruct = _arPidStruct[u1Pidx];
    fgEnable = rPidStruct.fgEnable;

    LOG(6, "Flush pidx %u buffer\n", u1Pidx);

    // Stop PID
    u4PidEntry = PID_INDEX_TABLE(u1Pidx);
    u4PidWord0 = PID_S_W(u1Pidx, 0);
    if (!_DmxEnablePid(u1Pidx, FALSE, &u4PidEntry, &u4PidWord0, &rPidStruct, FALSE))
    {
        return FALSE;
    }

    _DMX_Lock();

    // Actual stop PID
    PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
    PID_S_W(u1Pidx, 0) = u4PidWord0;
#ifdef CC_DMX_USE_CT
    if (rPidStruct.u2Pid == BPS_PID)
    {
        UINT8 u1TsIndex;
        UINT32 u4Register, u4Value;

        CT_SETTING = (CT_SETTING & 0x00FFFFFF) | 0xFF000000;

        u1TsIndex = rPidStruct.u1TsIndex;
        u4Register = _Dmx_GetBypassPidRegWordOffset(u1TsIndex);

        u4Value = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Value &= ~(1 << (26 + u1TsIndex));
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Value);

        DMXCMD_WRITE32(u4Register, 0);
    }
#endif  // CC_DMX_USE_CT

    _DMX_Unlock();

    HAL_Delay_us(1);

    // Move read/write pointers to buffer start
    _DMX_Lock();
    u4BufStart = rPidStruct.u4BufStart;
    rPidStruct.u4Rp = u4BufStart;
    rPidStruct.u4Wp = u4BufStart;
    u4BufStart = PHYSICAL(u4BufStart);
    PID_S_W(u1Pidx, 7) = u4BufStart;        // PES_start
    PID_S_W(u1Pidx, 8) = u4BufStart;        // Write pointer
    PID_S_W(u1Pidx, 9) = u4BufStart;        // Read pointer
    _arPidStruct[u1Pidx] = rPidStruct;

    _DMX_Unlock();

    // If the PID was not enabled, just return TRUE.
    if (!fgEnable)
    {
        return TRUE;
    }

    // Re-enable PID only if it was enabled.
    if (!_DmxEnablePid(u1Pidx, TRUE, &u4PidEntry, &u4PidWord0, &rPidStruct, FALSE))
    {
        return FALSE;
    }

    _DMX_Lock();
#ifdef CC_DMX_USE_CT
    // Be aware of the scenario of capturing bit stream on demand.
    if (rPidStruct.u2Pid == BPS_PID)
    {
        UINT8 u1TsIndex;
        UINT32 u4Register, u4Value;

        // Always turn off the Valid bit if PID value is BPS_PID.
        u4PidEntry &= 0x7FFFFFFF;

        u1TsIndex = rPidStruct.u1TsIndex;
        u4Register = _Dmx_GetBypassPidRegWordOffset(u1TsIndex);
        // The "To Microprocessor" flag should have been set in u4PidEntry.
        u4Value = u4PidEntry & 0xFFFF;
        u4Value |= u1Pidx << 16;
        DMXCMD_WRITE32(u4Register, u4Value);

        CT_SETTING = (CT_SETTING & 0x00FFFFFF) | ((UINT32)u1Pidx << 24);

        // Turn on the Bypass mode after setting the appropriate DBM_BYPASS_PID register.
        u4Value = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Value |= 1 << (26 + u1TsIndex);
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Value);
    }
#endif  // CC_DMX_USE_CT
    PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
    PID_S_W(u1Pidx, 0) = u4PidWord0;
    _arPidStruct[u1Pidx] = rPidStruct;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetPidIndex
 *  Find PID index for a given PID value
 *
 *  @param  u2Pid           PID
 *  @param  pu1PidIndex [out]
 *                          The PID index if found
 *
 *  @retval TRUE            Found
 *  @retval FALSE           Not found
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetPidIndex(UINT16 u2Pid, UINT8* pu1PidIndex)
{
    UINT32 i;
    BOOL fgRet = FALSE;

    ASSERT(pu1PidIndex != NULL);

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if (_arPidStruct[i].u2Pid == u2Pid)
        {
            *pu1PidIndex = (UINT8)i;
            fgRet = TRUE;
            break;
        }
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_SetPcr
 *  Set a PCR
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  fgEnable        Enable PCR of the PID or not
 *  @param  fgNewMode       Use the new PCR mode or not
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetPcr(UINT8 u1PidIndex, BOOL fgEnable, BOOL fgNewMode)
{
    UINT32 u4PidInfo;
    PID_STRUCT_T* prPidStruct;
    DMX_PCR_MODE_T ePcrMode = DMX_PCR_MODE_NONE;

    if (u1PidIndex >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    if (fgEnable)
    {
        if (fgNewMode)
        {
            ePcrMode = DMX_PCR_MODE_NEW;
        }
        else
        {
            ePcrMode = DMX_PCR_MODE_OLD;
        }
    }

    _DMX_Lock();

    prPidStruct = &_arPidStruct[u1PidIndex];
    prPidStruct->ePcrMode = ePcrMode;

    u4PidInfo = PID_INDEX_TABLE(u1PidIndex);
    u4PidInfo &= ~((1 << 15) | (1 << 3));
    u4PidInfo |= ((fgEnable ? 1 : 0) << 15) | ((fgNewMode ? 1 : 0) << 3);
    PID_INDEX_TABLE(u1PidIndex) = u4PidInfo;

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_IsVideoPid
 *  Check if a given PID index is for video data
 *
 *  @param  u1PidIndex      PID index
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IsVideoPid(UINT8 u1PidIndex)
{
    BOOL fgIsVideoPid;

    ASSERT(u1PidIndex < DMX_NUM_PID_INDEX);

    fgIsVideoPid = (PID_S_W(u1PidIndex, 0) & (1 << 7)) != 0;

    return fgIsVideoPid;
}


//-----------------------------------------------------------------------------
/** _DMX_SetToDecoder
 *  Set data sink to real decoders
 *
 *  @param  fgToDecoder     TRUE: to decoders, FALSE: not to decoders
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetToDecoder(BOOL fgToDecoder)
{
    _rDmxStates.fgToDecoder = fgToDecoder;
}


//-----------------------------------------------------------------------------
/** _DMX_IsToDecoder
 *  Query if data sink is set to real decoders
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IsToDecoder(void)
{
    return _rDmxStates.fgToDecoder;
}


//-----------------------------------------------------------------------------
/** _DMX_SetDebugPictureFinder
 *  Enable or disable debugging picture finder
 *
 *  @param  fgEnable        TRUE: enable, FALSE: disable
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetDebugPictureFinder(BOOL fgEnable)
{
    _rDmxStates.fgDebugPictureFinder = fgEnable;
}


//-----------------------------------------------------------------------------
/** _DMX_IsDebugPictureFinder
 *  Query if picture finder debugging is enabled or not
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IsDebugPictureFinder(void)
{
    return _rDmxStates.fgDebugPictureFinder;
}


//-----------------------------------------------------------------------------
/** _DMX_GetPidCounters
 *  Get the counter structure of a given pid index
 *
 *  @param  u1Pidx          PID index
 *  @param  prCounters [output]
 *                          Pointer to the PID counter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetPidCounters(UINT8 u1Pidx, DMX_PID_COUNTERS_T* const prCounters)
{
    if ((u1Pidx >= DMX_NUM_PID_INDEX) || (prCounters == NULL))
    {
        return FALSE;
    }

    // Copy structure here to avoid race condition
    *prCounters = _arPidStruct[u1Pidx].rCounters;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_ResetPidCounters
 *  Reset counters of a given pid index
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ResetPidCounters(UINT8 u1Pidx)
{
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    // Zero entire counter structure
    x_memset(&(_arPidStruct[u1Pidx].rCounters), 0,
        sizeof (DMX_PID_COUNTERS_T));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetStates
 *  Get demux states
 *
 *  @retval The pointer of the structure of demux states
 */
//-----------------------------------------------------------------------------
DMX_STATES_T* _DMX_GetStates(void)
{
    return &_rDmxStates;
}


//-----------------------------------------------------------------------------
/** _DMX_SetStartCodePattern_Ex
 *  Set search start code pattern
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetStartCodePattern_Ex(UINT8 u1NoIntrCount, const DMX_STARTCODE_T *prNoIntrStartCode,
                                 UINT8 u1IntrCount, const DMX_STARTCODE_T *prIntrStartCode)
{
    UINT16 u2Control;
    UINT8 i;

    if((u1NoIntrCount > DMX_STARTCODE_NONINTR_NUM) ||
       (u1IntrCount > DMX_STARTCODE_INTR_NUM))
    {
        return FALSE;
    }

    u2Control = 0x0;
    _DMX_Lock();

    if((u1NoIntrCount > 0) && (prNoIntrStartCode != NULL))
    {
        for(i=0; i<u1NoIntrCount; i++)
        {
            *(UINT32*)(DMX_PATTERN_MATCH_BASE + (1 + (i * 2)) * 4) =
                                            prNoIntrStartCode[i].u4Pattern;
            *(UINT32*)(DMX_PATTERN_MATCH_BASE + (1 + (i * 2) + 1) * 4) =
                                            prNoIntrStartCode[i].u4Mask;
            u2Control |= (1 << i);
        }
    }

    if((u1IntrCount > 0) && (prIntrStartCode != NULL))
    {
        for(i=0; i<u1IntrCount; i++)
        {
            *(UINT32*)(DMX_PATTERN_MATCH_BASE + (1 + ((i + DMX_STARTCODE_NONINTR_NUM) * 2)) * 4) =
                                            prIntrStartCode[i].u4Pattern;
            *(UINT32*)(DMX_PATTERN_MATCH_BASE + (1 + ((i + DMX_STARTCODE_NONINTR_NUM) * 2) + 1) * 4) =
                                            prIntrStartCode[i].u4Mask;
            u2Control |= (1 << (i + DMX_STARTCODE_NONINTR_NUM));
        }
    }

    // Write 0x1 to the "pattern changed" byte.
    *(UINT32*)DMX_PATTERN_MATCH_BASE = ((UINT32)u2Control << 16) | 0x1;

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetFramerErrorHandlingTable
 *  Set error handling table of framers
 *
 *  @param  u4Value         New value of error handling table
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetFramerErrorHandlingTable(UINT32 u4Value)
{
    DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE, u4Value);
}


//-----------------------------------------------------------------------------
/** _DMX_GetFramerErrorHandlingTable
 *  Get error handling table of framers
 *
 *  @retval The current value of error handling table
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetFramerErrorHandlingTable(void)
{
    return DMXCMD_READ32(DMX_REG_FRAMER_ERROR_HANDLE);
}


//-----------------------------------------------------------------------------
/** _DMX_SetInterruptThreshold
 *  Set interrupt threshold for partial PES
 *
 *  @param  ePidType        PID type, valid for video and audio only
 *  @param  u4Bytes         Threshold in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32 u4Bytes)
{
    UINT32 u4Packets;
    BOOL fgRet;

#ifdef ENABLE_MULTIMEDIA
    if (_DMX_GetInputType() == DMX_IN_PLAYBACK_MM)
    {
        _u2IntThresholdVideo = 0;
        _u2IntThresholdAudio = 0;
        LOG(9, "Prevent interrrupt threshold from being set for the DRM uCode!\n");
        return TRUE;
    }
#endif  // ENABLE_MULTIMEDIA

    u4Packets = (u4Bytes + 187) / 188;
    if (u4Packets > 0x10000)
    {
        // Too big to set to demux
        return FALSE;
    }

    fgRet = FALSE;

    _DMX_Lock();

    switch (ePidType)
    {
    case DMX_PID_TYPE_ES_VIDEO:
        _u2IntThresholdVideo = (UINT16)u4Packets;
        fgRet = TRUE;
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        _u2IntThresholdAudio = (UINT16)u4Packets;
        fgRet = TRUE;
        break;

    default:
        break;
    }

    if (fgRet)
    {
        UINT32 i;

        // Also set the new threshold to all running PIDs of the same PID type
        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (_arPidStruct[i].fgEnable &&
                (_arPidStruct[i].ePidType == ePidType))
            {
                PID_S_W(i, 15) = (UINT16)u4Packets;
            }
        }
    }

    _DMX_Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_GetInterruptThreshold
 *  Get interrupt threshold for partial PES
 *
 *  @param  ePidType        PID type, valid for video and audio only
 *  @param  pu4Bytes[output]
 *                          Threshold in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32* pu4Bytes)
{
    BOOL fgRet;

    if (pu4Bytes == NULL)
    {
        return FALSE;
    }

    fgRet = FALSE;

    switch (ePidType)
    {
    case DMX_PID_TYPE_ES_VIDEO:
        *pu4Bytes = (UINT32)(_u2IntThresholdVideo * 188);
        fgRet = TRUE;
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        *pu4Bytes = (UINT32)(_u2IntThresholdAudio * 188);
        fgRet = TRUE;
        break;

    default:
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_IsPidEnabled
 *  Is a PID enabled or disabled
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Enabled
 *  @retval FALSE           Disabled
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IsPidEnabled(UINT8 u1Pidx)
{
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    return _DmxIsPidEnabled(u1Pidx);
}


//-----------------------------------------------------------------------------
/** _DMX_GetScrambleState
 *  Get scramble state
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval The scramble state
 */
//-----------------------------------------------------------------------------
DMX_SCRAMBLE_STATE_T _DMX_GetScrambleState(UINT8 u1Pidx)
{
    UINT32 u4ScrambleFlag;
    DMX_SCRAMBLE_STATE_T eState;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return DMX_SCRAMBLE_STATE_UNKNOWN;
    }

    u4ScrambleFlag = (PID_S_W(u1Pidx, 2) >> 8) & 0xff;
    switch (u4ScrambleFlag)
    {
    case 0:
        eState = DMX_SCRAMBLE_STATE_CLEAR;
        break;

    case 1:
        eState = DMX_SCRAMBLE_STATE_SCRAMBLED;
        break;

    default:
        eState = DMX_SCRAMBLE_STATE_UNKNOWN;
        break;
    }

    return eState;
}


//-----------------------------------------------------------------------------
/** _DMX_InitAPI
 *  Initialize stuffs in this file
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_InitAPI(void)
{
    UINT32 i;
#ifdef DMX_CHECK_SECTION_CRC
    UINT32 j;
#endif  // DMX_CHECK_SECTION_CRC

    // Reset demux states
    //
    _rDmxStates.fgToDecoder = FALSE;
    _rDmxStates.fgDebugPictureFinder = FALSE;
    _rDmxStates.rCaptureSettings.pfnHandler = NULL;
    _rDmxStates.rCaptureSettings.pvTag = NULL;
    _rDmxStates.rCaptureSettings.u4BufStart = 0;
    _rDmxStates.rCaptureSettings.u4BufEnd = 0;

    _eFilterType = FILTER_TYPE_LEGACY;

    for (i = 0; i < MPV_MAX_ES; i++)
    {
        _rDmxStates.au1VideoPidx[i] = DMX_NULL_PIDX;
    }

#ifdef DEBUG_MEMORY_INTRUSION

    VERIFY(_DmxInitMemoryProtection());

#endif  // DEBUG_MEMORY_INTRUSION

#ifdef DMX_CHECK_SECTION_CRC
    for(i=0; i<DMX_NUM_FILTER_INDEX; i++)
    {
        _au4FilterCRCIdx[i] = 0;
        for(j=0; j<50; j++)
        {
            _afgFilterCRC[i][j] = 0xFF;
        }
    }

    for(i=0; i<DMX_NUM_FILTER_INDEX; i++)
    {
        _arFilterStruct[i].arBank[0].u1Pidx = 0xFF;
    }
#endif  // DMX_CHECK_SECTION_CRC
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetLockState
 *  Get lock state
 *
 *  @retval TRUE            TS locked
 *  @retval FALSE           TS not lock
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetLockState(void)
{
    UINT32 u4Packets0, u4Packets1, u4Diff, i;
    UINT32 u4LockTimes = 0;

    for(i=0; i<TS_LOCK_TEST_TIMES; i++)
    {
        u4Packets0 = DMXCMD_READ32(DMX_REG_FRAMER0_STATUS) & 0xffff;

        x_thread_delay(1);

        u4Packets1 = DMXCMD_READ32(DMX_REG_FRAMER0_STATUS) & 0xffff;
        if (u4Packets1 >= u4Packets0)
        {
            u4Diff = u4Packets1 - u4Packets0;
        }
        else
        {
            u4Diff = (u4Packets0 + 0x10000) - u4Packets1;
        }

        if (u4Diff >= TS_LOCK_MIN_PACKET)
        {
            u4LockTimes++;
        }
    }

    if(u4LockTimes == TS_LOCK_TEST_TIMES)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//-----------------------------------------------------------------------------
/** _DMX_GetLockStateEx
 *  Get lock state of framer
 *
 *  @retval TRUE            TS locked
 *  @retval FALSE           TS not lock
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetLockStateEx(UINT8 u1FramerIdx)
{
    UINT32 u4Packets0, u4Packets1, u4Diff, i, u4Reg;
    UINT32 u4LockTimes = 0;

    if(u1FramerIdx == 0)
    {
        u4Reg = DMX_REG_FRAMER0_STATUS;
    }
    else if(u1FramerIdx == 1)
    {
        u4Reg = DMX_REG_FRAMER1_STATUS;
    }
    else
    {
        LOG(1, "Unknow framer\n");
        return FALSE;
    }

    for(i=0; i<TS_LOCK_TEST_TIMES; i++)
    {
        u4Packets0 = DMXCMD_READ32(u4Reg) & 0xffff;

        x_thread_delay(1);

        u4Packets1 = DMXCMD_READ32(u4Reg) & 0xffff;
        if (u4Packets1 >= u4Packets0)
        {
            u4Diff = u4Packets1 - u4Packets0;
        }
        else
        {
            u4Diff = (u4Packets0 + 0x10000) - u4Packets1;
        }

        if (u4Diff >= TS_LOCK_MIN_PACKET)
        {
            u4LockTimes++;
        }
    }

    if(u4LockTimes == TS_LOCK_TEST_TIMES)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//-----------------------------------------------------------------------------
/** DMX_GetReceivedPacketNumber
 *  Get received TS packet number
 *
 *  @retval The received packet number
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetReceivedPacketNumber(void)
{
    UINT32 u4ReceivedPackets;

    u4ReceivedPackets = DMXCMD_READ32(DMX_REG_FRAMER0_STATUS) & 0xffff;

    return u4ReceivedPackets;
}


//-----------------------------------------------------------------------------
/** DMX_GetDroppedPacketNumber
 *  Get dropped TS packet number
 *
 *  @retval The dropped packet number
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetDroppedPacketNumber(void)
{
    UINT32 u4DroppedPackets;

    u4DroppedPackets = (DMXCMD_READ32(DMX_REG_FRAMER0_STATUS) & 0xffff0000)
        >> 16;

    return u4DroppedPackets;
}


//-----------------------------------------------------------------------------
/** _DMX_SetPacketSize
 *
 *  DMX uP processes a certain amount of bytes at a time.  The amount is called
 *  a packet here.  When this function is called, DMX must have been stoped.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetPacketSize(UINT8 u1PacketSize)
{
    UINT32 u4Reg;

    switch (u1PacketSize)
    {
    case 188:
    case 192:
        break;

    default:
        LOG(1, "Packet size (%u) is not supported!", u1PacketSize);
        return FALSE;
    }

    // _DMX_EnableFTI(FALSE);      // Disable FTI

    u4Reg = DMXCMD_READ32(DMX_REG_CONFIG2) & 0xFFFF00FF;
    u4Reg |= (u1PacketSize << 8);
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Reg);

    if (!_DMX_ResetDbmSafely())
    {
        return FALSE;
    }

    // _DMX_EnableFTI(TRUE);       // Enable FTI

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetMinFreeBufferSize
 *
 *  Suppose the minimal size of the Audio/Video buffer is X, and this function
 *  is provided to calculate the value X.  This function must also take the
 *  Demux internal buffer into account by substracting the internal buffer size
 *  from X.  It is important that users must query the size of data (suppose Y)
 *  in DDI before calling this function.  Then, the amount of data to be sent
 *  to DDI must be less than (X - Y).  Otherwise, excess data will be
 *  lost/dropped in the Half-Push mode.
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetMinFreeBufferSize(DMX_PID_TYPE_T ePidType)
{
    UINT32 u4BufSize, u4FreeLen, u4MinFreeSize;
    UINT32 u4BufStart, u4BufEnd, u4Rp, u4Wp;
    UINT8 i;
    BOOL fgEnabled;
    PID_STRUCT_T *prPidStruct;

    if ((ePidType != DMX_PID_TYPE_ES_AUDIO) &&
        (ePidType != DMX_PID_TYPE_ES_VIDEO) &&
        (ePidType != DMX_PID_TYPE_PSI))
    {
        LOG(9, "Cannot query buffer size other than A/V.\n");
        return 0;
    }

    u4MinFreeSize = 0xFFFFFFFF;

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
#ifdef TIME_SHIFT_SUPPORT
        BOOL fgPVREnabled;
        fgPVREnabled = FVR_IsRecording();
#endif  // TIME_SHIFT_SUPPORT       

        prPidStruct = _DMX_GetPidStruct(i);

#ifdef TIME_SHIFT_SUPPORT
        if(fgPVREnabled && (prPidStruct->u1TsIndex != DMX_PVRPLAY_TS_INDEX))
        {
            continue;
        }
#endif  // TIME_SHIFT_SUPPORT       

        fgEnabled = _DmxIsPidEnabled(i);

        _DMX_Lock();

        if (fgEnabled && (prPidStruct->ePidType == ePidType))
        {
            UINT16 u2Loop;

            if (ePidType == DMX_PID_TYPE_PSI)
            {
                // Read the hardware buffer.
                // There's no need to convert hardware addresses to virtual ones.
                u4BufStart = PID_S_W(i, 5);
                u4BufEnd = PID_S_W(i, 6);
                u4Rp = PID_S_W(i, 9);
                // Repeatedly read the WP until it stabilizes.
                // The caller of this function must stop data transfer before
                // calling this function in the blocking DDI mode.  In such a
                // case, the following lines are redundant.
                u2Loop = 0;
                do {
                    u4Wp = PID_S_W(i, 8);
                    if ((++u2Loop) >= 1000)
                    {
                        LOG(3, "(1) Write pointer is not stabilized!\n");
                        break;
                    }
                } while (u4Wp != PID_S_W(i, 8));
            }
            else
            {
                // Read the hardware buffer.
                // There's no need to convert hardware addresses to virtual ones.
                u4BufStart = PID_S_W(i, 5);
                u4BufEnd = PID_S_W(i, 6);
                u4Rp = PID_S_W(i, 9);
                // Repeatedly read the WP until it stabilizes.
                // The caller of this function must stop data transfer before
                // calling this function in the blocking DDI mode.  In such a
                // case, the following lines are redundant.
                u2Loop = 0;
                do {
                    u4Wp = PID_S_W(i, 8);
                    if ((++u2Loop) >= 1000)
                    {
                        LOG(3, "(2) Write pointer is not stabilized!\n");
                        break;
                    }
                } while (u4Wp != PID_S_W(i, 8));
            }

            u4BufSize = (u4BufEnd - u4BufStart) + 1;
            u4FreeLen = EMPTYSIZE(u4Rp, u4Wp, u4BufSize);
            if (u4FreeLen < u4MinFreeSize)
            {
                u4MinFreeSize = u4FreeLen;
            }
        }
        _DMX_Unlock();
    }

    if (u4MinFreeSize == (UINT32)0xFFFFFFFF)
    {
        return 0;
    }

    // The internal buffer of the 5391 Demux can hold up to 10 TS packets
    // (188 * 10).  For each packet, at most 184 bytes will be sent to the
    // output buffer.  Thus, we shall be able to use 184 instead of 188 (not
    // tested yet!).
    u4MinFreeSize = (UINT32)((u4MinFreeSize < 1880) ? 0 : (u4MinFreeSize - 1880));

    return u4MinFreeSize;
}


//-----------------------------------------------------------------------------
/** _DMX_SetTsIndex
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetTsIndex(UINT8 u1TsIdx)
{
    DMX_PID_T rPid;
    UINT8 i;
#ifdef TIME_SHIFT_SUPPORT   
    FVR_PID_T rFVRPid;
#endif // TIME_SHIFT_SUPPORT       

    //-------------------------------------------
    // Play PID
    //-------------------------------------------
    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        if(!_DMX_GetPid(i, DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_VALID, &rPid))
        {
            return FALSE;
        }

        if(rPid.fgEnable && (rPid.u1TsIndex == 2))
        {
            // PVR case, don't change TS index
            continue;
        }

        rPid.u1TsIndex = u1TsIdx;
        if(!_DMX_SetPid(i, DMX_PID_FLAG_TS_INDEX, &rPid, FALSE))
        {
            return FALSE;
        }
    }

#ifdef TIME_SHIFT_SUPPORT
    //-------------------------------------------
    // Record PID
    //-------------------------------------------
    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(!_FVR_GetPid(i, FVR_PID_FLAG_ENABLE | FVR_PID_FLAG_TSINDEX, &rFVRPid))
        {
            return FALSE;
        }

        if(!rFVRPid.fgEnable)
        {
            continue;
        }

        rFVRPid.u1TsIndex = u1TsIdx;
        if(!_FVR_SetPid(i, FVR_PID_FLAG_TSINDEX, &rFVRPid))
        {
            return FALSE;
        }
    }
#endif  //TIME_SHIFT_SUPPORT

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_InitMemoryTracker
 */
//-----------------------------------------------------------------------------
void _DMX_InitMemoryTracker(void)
{
#if 1
    static BOOL _fgInited = FALSE;
    UINT8 u1Pidx;

    if (!_fgInited)
    {
        _fgInited = TRUE;
    }
    else
    {
        for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
        {
            if (_rMemTracker.arPidMemUsage[u1Pidx].fgValid)
            {
                // FIXME: For PSI PID, free the associated filter!
                if (!_DMX_FreePid(u1Pidx))
                {
                    LOG(3, "Memory tracker failed to free pidx %d!\n", u1Pidx);
                    continue;
                }
                LOG(3, "Memory tracker frees the memory for pidx %d!\n", u1Pidx);
            }
        }
    }
#endif

    x_memset((void*)&_rMemTracker, 0, sizeof(_rMemTracker));
}


//-----------------------------------------------------------------------------
/** _DMX_SetMemoryUsage
 *  The header buffer is not calculated in _rMemTracker.
 */
//-----------------------------------------------------------------------------
void _DMX_SetMemoryUsage(UINT8 u1Pidx, UINT32 u4Size)
{
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "%s(): pidx (%d) is out of range\n", __func__, u1Pidx);
        return;
    }

    if (_rMemTracker.arPidMemUsage[u1Pidx].fgValid)
    {
        LOG(3, "Memory of pidx %d is not freed before set!\n", u1Pidx);
        return;
    }

    _rMemTracker.arPidMemUsage[u1Pidx].fgValid = TRUE;
    _rMemTracker.arPidMemUsage[u1Pidx].u4CurrentUsage = u4Size;
    if (u4Size > _rMemTracker.arPidMemUsage[u1Pidx].u4PeakUsage)
    {
        _rMemTracker.arPidMemUsage[u1Pidx].u4PeakUsage = u4Size;
    }

    _rMemTracker.u4GlobalCurrent += u4Size;
    if (_rMemTracker.u4GlobalCurrent > _rMemTracker.u4GlobalPeak)
    {
        _rMemTracker.u4GlobalPeak = _rMemTracker.u4GlobalCurrent;
    }
}

//-----------------------------------------------------------------------------
/** _DMX_ClearMemoryUsage
 *  The header buffer is not calculated in _rMemTracker.
 */
//-----------------------------------------------------------------------------
void _DMX_ClearMemoryUsage(UINT8 u1Pidx, UINT32 u4Size)
{
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "%s(): pidx (%d) is out of range\n", __func__, u1Pidx);
        return;
    }

    if (!(_rMemTracker.arPidMemUsage[u1Pidx].fgValid))
    {
        LOG(3, "Memory of pidx %d has already been freed!\n", u1Pidx);
        return;
    }

    if (u4Size != _rMemTracker.arPidMemUsage[u1Pidx].u4CurrentUsage)
    {
        LOG(3, "Current memory usage does not match the freed size!\n");
        return;
    }
    _rMemTracker.arPidMemUsage[u1Pidx].u4CurrentUsage = 0;

    if (u4Size > _rMemTracker.u4GlobalCurrent)
    {
        LOG(3, "Current global memory usage is less than the freed size!\n");
        return;
    }
    _rMemTracker.u4GlobalCurrent -= u4Size;

    _rMemTracker.arPidMemUsage[u1Pidx].fgValid = FALSE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetMemoryUsage
 */
//-----------------------------------------------------------------------------
void _DMX_GetMemoryUsage(DMX_MEM_TRACKER_T *pMemTracker)
{
    ASSERT(pMemTracker != NULL);

    x_memcpy((void*)pMemTracker, (void*)&_rMemTracker, sizeof(_rMemTracker));
}


//-----------------------------------------------------------------------------
/** _DMX_SetBufPointer
*   This function used to set DMX buffer pointer for single move
*/
//-----------------------------------------------------------------------------
BOOL _DMX_SetBufPointer(UINT8 u1Idx, UINT32 u4BufStart, UINT32 u4BufEnd,
                           UINT32 u4Addr)
{
    PID_STRUCT_T* prPidStruct;

    //ASSERT(u4BufStart < u4BufEnd);
    //ASSERT((u4Addr >= u4BufStart) && (u4Addr < u4BufEnd));
    if(!((u4Addr >= u4BufStart) && (u4Addr < u4BufEnd)))
    {
        return FALSE;
    }

    if(u1Idx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    _DMX_Lock();
    prPidStruct->u4BufStart = VIRTUAL(u4BufStart);
    prPidStruct->u4BufEnd = VIRTUAL(u4BufEnd);
    prPidStruct->u4Wp = VIRTUAL(u4Addr);
    prPidStruct->u4Rp = VIRTUAL(u4Addr);
    prPidStruct->u4SectionRp = VIRTUAL(u4Addr);
    prPidStruct->u4PesRp = VIRTUAL(u4Addr);
    prPidStruct->u4BufLen = u4BufEnd - u4BufStart + 1;

    PID_S_W(u1Idx, 5) = PHYSICAL(u4BufStart);     // FIFO start
    PID_S_W(u1Idx, 6) = PHYSICAL(u4BufEnd);       // FIFO end
    PID_S_W(u1Idx, 7) = PHYSICAL(u4Addr);         // PES_start
    PID_S_W(u1Idx, 8) = PHYSICAL(u4Addr);         // Write pointer
    PID_S_W(u1Idx, 9) = PHYSICAL(u4Addr);         // Read pointer
    _DMX_Unlock();

    return TRUE;
}


#ifdef DMX_CHECK_SECTION_CRC
//-----------------------------------------------------------------------------
/** _DMX_GetSFCRCStatus
*/
//-----------------------------------------------------------------------------
BOOL _DMX_GetSFCRCStatus(UINT8 u1Pidx)
{
    UINT8 i;

    for(i=0; i<DMX_NUM_FILTER_INDEX; i++)
    {
        if(_arFilterStruct[i].arBank[0].u1Pidx == u1Pidx)
        {
            return _arFilterStruct[i].arBank[0].fgCheckCrc;
        }
    }

    return FALSE;
}
#endif  // DMX_CHECK_SECTION_CRC


#ifdef CC_EMULATION
//-----------------------------------------------------------------------------
/** _DMX_EnableLocalArbitor
 */
//-----------------------------------------------------------------------------
BOOL _DMX_EnableLocalArbitor(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, BOOL fgEnable)
{
    UINT32 u4Reg;

    u4Reg = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_CTRL2);

    switch(eDevice)
    {
    case DMX_LOCAL_ARBITOR_DMX:
        if(fgEnable)
        {
            u4Reg |= 0x1;
        }
        else
        {
            u4Reg &= ~0x1;
        }
        break;

    case DMX_LOCAL_ARBITOR_DDI:
        if(fgEnable)
        {
            u4Reg |= 0x2;
        }
        else
        {
            u4Reg &= ~0x2;
        }
        break;

    case DMX_LOCAL_ARBITOR_PARSER:
        if(fgEnable)
        {
            u4Reg |= 0x4;
        }
        else
        {
            u4Reg &= ~0x4;
        }
        break;

    default:
        return FALSE;
    }

    DMXCMD_WRITE32(DMX_REG_LOCAL_ARBITOR_CTRL2, u4Reg);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DRAMMeasure
 */
//-----------------------------------------------------------------------------
BOOL _DMX_DRAMMeasure(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, UINT32 u4TimeMs)
{
    UINT32 u4Reg, u4BandWidth, u4Cycles;

    if(eDevice >= DMX_LOCAL_ARBITOR_NUM)
    {
        return FALSE;
    }

    // Reset
    u4Reg = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_CTRL2) & 0xFF0FFFFF;
    DMXCMD_WRITE32(DMX_REG_LOCAL_ARBITOR_CTRL2, u4Reg);

    u4Reg = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_CTRL2) & 0x0F0FFFFF;

    // Set device
    u4Reg |= ((eDevice & 0xF) << 28);

    // Enable
    u4Reg |= 0x100000;
    DMXCMD_WRITE32(DMX_REG_LOCAL_ARBITOR_CTRL2, u4Reg);

    x_thread_delay(u4TimeMs);

    // Stop
    u4Reg |= 0x200000;
    DMXCMD_WRITE32(DMX_REG_LOCAL_ARBITOR_CTRL2, u4Reg);

    // Read value
    DMXCMD_WRITE32(DMX_REG_REG_FILE_ADDR_REG, 0x3000);
    u4Cycles = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_MONITOR);
    DMXCMD_WRITE32(DMX_REG_REG_FILE_ADDR_REG, 0x2000);
    // bandwdith is in the unit of 8 bytes
    u4BandWidth = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_MONITOR);

    // Reset
    u4Reg = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_CTRL2) & 0x0F0FFFFF;
    DMXCMD_WRITE32(DMX_REG_LOCAL_ARBITOR_CTRL2, u4Reg);

    LOG(1, "Bandwidth: 0x%x, cycles: 0x%x\n", u4BandWidth, u4Cycles);

    return TRUE;
}
#endif // CC_EMULATION

//-----------------------------------------------------------------------------
/** _DMX_DriverHasAW
 *  Return TRUE if the driver supports AW.  Otherwise, return FALSE.
*/
//-----------------------------------------------------------------------------
BOOL _DMX_DriverHasAW(void)
{
#ifdef DMX_DRIVER_WITH_AW
    return TRUE;
#else
    return FALSE;
#endif  // DMX_DRIVER_WITH_AW
}


