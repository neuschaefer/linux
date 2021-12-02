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
 * $RCSfile: dmx_mm_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_mm_api.c
 *  Demux program stream driver
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "dmx_drm_drvif.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "mpv_drvif.h"
#include "aud_drvif.h"
LINT_EXT_HEADER_END

#include "x_assert.h"
#include "x_os.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_timer.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
//#define DMX_MM_MOVEDATA_MEASURETIME

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MM_BUF_FULL_GAP                 ((188*4) + 32)

#define NULL_MM_VIDEO_IDX               0xFF
#define MM_MAX_VIDEO_NUM                2

#define MM_PES_HEADER_BUF_SIZE          0x4000

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT8 _u1CurStreamID;

static DMX_MM_DATA_T _rDmxMMData;

static BOOL _afgMMSearchStartCode[DMX_MM_STRUCT_NUM];

static HANDLE_T _hDmxMMSema = NULL_HANDLE;  // Make sure _rDmxMMData is synchronization

static DMX_DRM_MODE_T   _arDmxMMDRMMode[DMX_MM_STRUCT_NUM];
static DMX_MM_DRM_AES_T _arDmxMMDRMAes[DMX_MM_STRUCT_NUM];

#ifdef DMX_MM_MOVEDATA_MEASURETIME
static HAL_TIME_T _rTimePeriod;
#endif

static UINT32 u4MoveDataTimeoutCount = 0;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// _DMXMMAllocBuf
//-----------------------------------------------------------------------------
static BOOL _DMXMMAllocBuf(UINT8 u1Idx, const DMX_MM_T *prMM)
{
    PID_STRUCT_T* prPidStruct;
    UINT32 u4BufStart, u4PhyBufStart, u4W0 = 0, u4Reg;

    if(prMM == NULL)
    {
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if((prPidStruct->u4BufStart != 0) || (prPidStruct->u4BufEnd != 0))
    {
        LOG(3, "Buffer already allocated\n");
        return FALSE;
    }

    // Section filter flags
    // bit 0: 1: PSI, 0: PES
    // bit 1: Section filter on
    // bit 2: Output mode
    // bit 4: PID on/off toggle (combined with bit 8 of PID index entry)
    // bit 5: PID disable
    // bit 6: To CD-FIFO
    // bit 7: Enable picture finder
    if((prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO) && (prMM->fgSearchStartCode))
    {
        u4W0 = (UINT32)((1 << 1) | (1 << 6) | (1 << 7));
    }
    else if((prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO) ||
            (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO))

    {
        u4W0 = (UINT32)((1 << 1) | (1 << 6));
    }
    else if(prPidStruct->ePidType == DMX_PID_TYPE_PES)
    {
        u4W0 = (UINT32)(1 << 1);
    }

    //-----------------------------------------------------
    // Allocate header buffer
    //-----------------------------------------------------
    // Setup header buffer start address
    if((prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO) ||
       (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO))
    {
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(MM_PES_HEADER_BUF_SIZE,
                                DMX_HEADER_FIFO_ALIGNMENT);
        if (u4BufStart == 0)
        {
            LOG(3, "Can't allocate memory!\n");
            return FALSE;
        }

        _DMX_Lock();

        u4BufStart = VIRTUAL(u4BufStart);
        prPidStruct->u4HeaderBufAddr = u4BufStart;
        u4PhyBufStart = PHYSICAL(u4BufStart);
        PID_S_W(u1Idx, 10) = u4PhyBufStart;    // FIFO start
        PID_S_W(u1Idx, 12) = u4PhyBufStart;    // Start
        PID_S_W(u1Idx, 13) = u4PhyBufStart;    // Write pointer
        PID_S_W(u1Idx, 14) = u4PhyBufStart;    // Read pointer
        prPidStruct->u4NextPic = 0;

        // Header buffer end address
        PID_S_W(u1Idx, 11) = (u4PhyBufStart + MM_PES_HEADER_BUF_SIZE) - 1;

        _DMX_Unlock();
    }

    //-----------------------------------------------------
    // Allocate frame buffer
    //-----------------------------------------------------
    prPidStruct->fgAllocateBuffer = prMM->fgAllocBuf;
    if(prMM->fgAllocBuf)
    {
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(prMM->u4BufSize,
                            DMX_ES_FIFO_ALIGNMENT);
        if (u4BufStart == 0)
        {
            VERIFY(BSP_FreeAlignedDmaMemory(
                prPidStruct->u4HeaderBufAddr));
            LOG(3, "Can't allocate memory!\n");
            return FALSE;
        }
    }
    else
    {
        u4BufStart = prMM->u4BufAddr;
    }

    _DMX_Lock();

    // Preserve bit 4: PID on/off toggle (combined with bit 8 of PID index)
    //PID_S_W(u1Idx, 0) = (PID_S_W(u1Idx, 0) & (1 << 4)) | u4W0; // Not necessary for MM
    u4Reg = PID_S_W(u1Idx, 0);
    u4Reg &= 0xFF00FF00;
    u4Reg |= (u4W0 & 0xFF);
    PID_S_W(u1Idx, 0) = u4Reg;

    PID_S_W(u1Idx, 4) = 0xFFFFFFFF;     // Reset previous payload counter to FFFFFFFF
    PID_S_W(u1Idx, 18) = 0xFFFFFFFF;    // Reset previous payload counter to FFFFFFFF

    u4BufStart = VIRTUAL(u4BufStart);
    prPidStruct->u4BufStart = u4BufStart;
    prPidStruct->u4Wp = u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4SectionRp = u4BufStart;
    prPidStruct->u4PesRp = u4BufStart;
    u4PhyBufStart = PHYSICAL(u4BufStart);
    PID_S_W(u1Idx, 5) = u4PhyBufStart;     // FIFO start
    PID_S_W(u1Idx, 7) = u4PhyBufStart;     // PES_start
    PID_S_W(u1Idx, 8) = u4PhyBufStart;     // Write pointer
    PID_S_W(u1Idx, 9) = u4PhyBufStart;     // Read pointer

    // FIFO end address
    prPidStruct->u4BufEnd = (u4BufStart + prMM->u4BufSize) - 1;
    PID_S_W(u1Idx, 6) = (u4PhyBufStart + prMM->u4BufSize) - 1;

    // FIFO size
    prPidStruct->u4BufLen = prMM->u4BufSize;

    prPidStruct->fgAllocateBuffer = prMM->fgAllocBuf;

    prPidStruct->u1ChannelId = prMM->u1ChannelId;

    _DMX_Unlock();

    if(prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        if (!_DMX_AllocateVideoChannel(u1Idx, prPidStruct->u1ChannelId))
        {
            return FALSE;
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
// _DMXMMFreeBuf
//-----------------------------------------------------------------------------
static BOOL _DMXMMFreeBuf(UINT8 u1Idx)
{
    PID_STRUCT_T* prPidStruct;

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    // Free buffer
    if (prPidStruct->fgAllocateBuffer)
    {
        if (!BSP_FreeAlignedDmaMemory(prPidStruct->u4BufStart))
        {
            LOG(3, "Fail to free PID buffer! pidx: %u, addr: 0x%08x\n",
                u1Idx, prPidStruct->u4BufStart);
            return FALSE;
        }
    }

    // Free header buffer
    if (!BSP_FreeAlignedDmaMemory(prPidStruct->u4HeaderBufAddr))
    {
        LOG(3, "Fail to free PID header buffer! pidx: %u, addr: 0x%08x\n",
            u1Idx, prPidStruct->u4HeaderBufAddr);
        return FALSE;
    }

    _DMX_Lock();

    // Update PID state
    prPidStruct->fgAllocateBuffer = FALSE;
    prPidStruct->u4BufStart = 0;
    prPidStruct->u4BufEnd = 0;
    prPidStruct->u4BufLen = 0;
    prPidStruct->u4Rp = 0;
    prPidStruct->u4Wp = 0;
    prPidStruct->u4HeaderBufAddr = 0;

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
UINT32 _DMXMMGetData(void)
{
    return (UINT32)&_rDmxMMData;
}

//-----------------------------------------------------------------------------
/** _DMX_MM_Init
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MM_Init(void)
{
    static BOOL _fgInit = FALSE;
    UINT32 i;

    if(!_fgInit)
    {
        VERIFY(OSR_OK ==x_sema_create(&_hDmxMMSema,
                        X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));
        _fgInit = TRUE;
    }

    x_memset((VOID*)&_rDmxMMData, 0x0, sizeof(DMX_MM_DATA_T));
    x_memset((VOID*)_arDmxMMDRMMode, 0x0, DMX_MM_STRUCT_NUM * sizeof(DMX_DRM_MODE_T));
    x_memset((VOID*)_arDmxMMDRMAes, 0x0, DMX_MM_STRUCT_NUM * sizeof(DMX_MM_DRM_AES_T));

    _u1CurStreamID = 0xFF;
    for(i=0; i<DMX_MM_STRUCT_NUM; i++)
    {
        _afgMMSearchStartCode[i] = FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_MM_Set
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MM_Set(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_T *prMM)
{
    PID_STRUCT_T* prPidStruct;
    UINT32 u4Reg;

    if(prMM == NULL)
    {
        return FALSE;
    }

    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        LOG(3, "Invalid index\n");
        return FALSE;
    }

    if(u4Flags == DMX_MM_FLAG_NONE)
    {
        return TRUE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    // Input type
    prPidStruct->eInputType = DMX_IN_PLAYBACK_MM;
    prPidStruct->eSource = DMX_SOURCE_DRAM0;

    if(u4Flags & DMX_MM_FLAG_TYPE)
    {
        prPidStruct->ePidType = prMM->ePidType;
    }

    if(u4Flags & DMX_MM_FLAG_BUF)
    {
        if(!_DMXMMAllocBuf(u1Idx, prMM))
        {
            return FALSE;
        }
    }

    if(u4Flags & DMX_MM_FLAG_SEARCH_START_CODE)
    {
        _afgMMSearchStartCode[u1Idx] = prMM->fgSearchStartCode;

        // bit 7: Enable picture finder
        u4Reg = PID_S_W(u1Idx, 0);
        u4Reg &= ~0x80;
        if((prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO) && (prMM->fgSearchStartCode))
        {
            u4Reg |= 0x80;
            //set byte2 to 0xff for default picture search by TS_IDX
            u4Reg |= 0xFF0000;
        }
        PID_S_W(u1Idx, 0) = u4Reg;
    }

    if(u4Flags & DMX_MM_FLAG_NOTIFY)
    {
        prPidStruct->pfnNotify = prMM->pfnCallback;
    }

    if(u4Flags & DMX_MM_FLAG_DEVICE_ID)
    {
        prPidStruct->u1DeviceId = prMM->u1DeviceId;
    }

    if (u4Flags & DMX_MM_FLAG_INSTANCE_TAG)
    {
        prPidStruct->pvInstanceTag = prMM->pvInstanceTag;
    }

    if(u4Flags & DMX_MM_FLAG_VALID)
    {
        // Fixme
        prPidStruct->fgEnable = prMM->fgEnable;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_Get
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MM_Get(UINT8 u1Idx, UINT32 u4Flags, DMX_MM_T *prMM)
{
    PID_STRUCT_T* prPidStruct;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);

    if(prMM == NULL)
    {
        return FALSE;
    }

    if(u4Flags == DMX_MM_FLAG_NONE)
    {
        return TRUE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if(u4Flags & DMX_MM_FLAG_TYPE)
    {
        prMM->ePidType = prPidStruct->ePidType;
    }

    if(u4Flags & DMX_MM_FLAG_BUF)
    {
        prMM->u4BufAddr = prPidStruct->u4BufStart;
        prMM->u4BufSize = prPidStruct->u4BufLen;
        prMM->u4Rp = prPidStruct->u4Rp;
        prMM->u4Wp = prPidStruct->u4Wp;
        prMM->u1ChannelId = prPidStruct->u1ChannelId;
    }

    if(u4Flags & DMX_MM_FLAG_SEARCH_START_CODE)
    {
        prMM->fgSearchStartCode = _afgMMSearchStartCode[u1Idx];
    }

    if(u4Flags & DMX_MM_FLAG_NOTIFY)
    {
        prMM->pfnCallback = prPidStruct->pfnNotify;
    }

    if (u4Flags & DMX_MM_FLAG_INSTANCE_TAG)
    {
        prMM->pvInstanceTag = prPidStruct->pvInstanceTag;
    }

    if(u4Flags & DMX_MM_FLAG_DEVICE_ID)
    {
        prMM->u1DeviceId = prPidStruct->u1DeviceId;
    }

    if(u4Flags & DMX_MM_FLAG_VALID)
    {
        prMM->fgEnable = prPidStruct->fgEnable;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_Free
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MM_Free(UINT8 u1Idx)
{
    PID_STRUCT_T* prPidStruct;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);

    _DMX_Lock();
    PID_INDEX_TABLE(u1Idx) = 0;
    PID_S_W(u1Idx, 0) = 0;
    _DMX_Unlock();

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        UINT8 u1Channel;

        // Sync with MPV
        VERIFY(_DMX_GetVideoChannel(u1Idx, &u1Channel));
        MPV_FlushEsmQ(u1Channel, TRUE);

        // No need to send purge message if serail number is applied
        // Discard intermediate pictures, clean FIFO
//        VERIFY(_DMX_SendPurgeMessage(u1Pidx, FALSE));
    }
    else if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        // FIXME
        AUD_WaitDspFlush(prPidStruct->u1DeviceId);

        // No need to send purge message if serial number is applied
        // Discard intermediate frames, clean FIFO
//        VERIFY(_DMX_SendPurgeMessage(u1Pidx, FALSE));
    }

    if(!_DMXMMFreeBuf(u1Idx))
    {
        return FALSE;
    }

    if(prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        _DMX_FreeVideoChannel(u1Idx);
    }

    x_memset((VOID*)prPidStruct, 0x0, sizeof(PID_STRUCT_T));
    _afgMMSearchStartCode[u1Idx] = FALSE;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_GetSearchStartCode
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MM_GetSearchStartCode(UINT8 u1Idx)
{
    return _afgMMSearchStartCode[u1Idx];
}


//-----------------------------------------------------------------------------
/** _DMX_MM_GetBufEmptySize
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_MM_GetBufEmptySize(UINT8 u1Idx)
{
    PID_STRUCT_T* prPidStruct;
    UINT32 u4BufSize, u4Wp, u4Rp, u4EmptySize, u4Ptr1, u4Ptr2;
    //UINT32 u4BufSize, u4Wp, u4Rp, u4EmptySize;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);
    prPidStruct = _DMX_GetPidStruct(u1Idx);

    u4BufSize = prPidStruct->u4BufLen;
    u4Wp = prPidStruct->u4Wp;
    u4Rp = prPidStruct->u4Rp;

    do
    {
        u4Ptr1 = PID_S_W(u1Idx, 8); // Wp
        u4Ptr2 = PID_S_W(u1Idx, 8);
    } while(u4Ptr1 != u4Ptr2);
    u4Wp = u4Ptr1;

    do
    {
        u4Ptr1 = PID_S_W(u1Idx, 9); // Rp
        u4Ptr2 = PID_S_W(u1Idx, 9);
    } while(u4Ptr1 != u4Ptr2);
    u4Rp = u4Ptr1;

    u4EmptySize = EMPTYSIZE(u4Rp, u4Wp, u4BufSize);

    if(u4EmptySize > MM_BUF_FULL_GAP)
    {
        u4EmptySize -= MM_BUF_FULL_GAP;
    }
    else
    {
        u4EmptySize = 0;
    }

    return u4EmptySize;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_MoveData
 *     Blocking function
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MM_MoveData(UINT8 u1Idx, const DMX_MM_DATA_T *prData, UINT32 u4TimeOut)
{
    UINT32 u4BufStart, u4BufEnd, u4SendSize, u4SendAddr;
    UINT32 u4SkipLen, u4DDIAddr, u4DDILen, u4AesLen, u4AesOffset;
    PID_STRUCT_T *prPidStruct;
#ifdef DMX_MM_MOVEDATA_MEASURETIME
    HAL_TIME_T rTime1, rTime2, rTimeDiff;
#endif
#if 1
    INT32 i4Ret;
#endif
    UINT32 u4Reg;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);

    if(prData == NULL)
    {
        return FALSE;
    }

#ifdef DMX_MM_MOVEDATA_MEASURETIME
    HAL_GetTime(&rTime1);
    HAL_GetDeltaTime(&rTimeDiff, &_rTimePeriod, &rTime1);
    LOG(1, "period time: %d.%06d\n", rTimeDiff.u4Seconds, rTimeDiff.u4Micros);
    _rTimePeriod = rTime1;
#endif

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if(prPidStruct == NULL)
    {
        return FALSE;
    }

    if(!prPidStruct->fgEnable)
    {
        return FALSE;
    }

#ifdef CC_DMX_AUDIO_PREBUF
    if(!_DMX_AudHandler_UpdateRp(u1Idx, prData->u4FrameSize))
    {
        return FALSE;
    }
#endif  // CC_DMX_AUDIO_PREBUF

    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        if(_DMX_MM_GetBufEmptySize(u1Idx) < prData->u4FrameSize )
        {
            return FALSE;
        }
    }

	// For protection purpose, if AES length is over the size of moving data,
    // dmx will not trigger this move
    if((PID_S_W(u1Idx, 0) & 0xff000000) >> 24 != 0)
    {
        _DMX_Lock();
        u4AesLen = PID_S_W(u1Idx, 2);
        u4AesOffset = PID_S_W(u1Idx, 15);
        _DMX_Unlock();

        if(u4AesOffset + u4AesLen > prData->u4FrameSize)
        {
            return FALSE;
        }
    }

    // Set DBM Bypass PID register, steer to uP
    // Clear bits [25:24] of the DBM Control Register to set DDI to TS index 3.
    // The next line is commented out because the default value of bits [25:24]
    // of the DBM Control Register is 0 (which is not changed since reset).
    //DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) & 0xFCFFFFFF);
    DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID_4, ((UINT32)u1Idx << 16) | 0x20);

    u4BufEnd = prData->u4BufEnd;
    u4BufStart = prData->u4BufStart;
    u4SendAddr = prData->u4StartAddr;
    u4SendSize = prData->u4FrameSize;

    // Meet the alignment and packet size (188 bytes) requirements.
    u4SkipLen = u4SendAddr % DDI_POINTER_ALIGNMENT;
    u4DDIAddr = u4SendAddr - u4SkipLen;
    u4DDILen  = u4SkipLen + u4SendSize + (188 - 1);  // Overflow? IMHO, no.
    u4DDILen -= u4DDILen % 188;

    LOG(7, "%d, Sendaddr: 0x%x, sendsize:0x%x, ddiaddr:0x%x, ddisize: 0x%x\n",
              u4SkipLen, u4SendAddr, u4SendSize, u4DDIAddr, u4DDILen);

    // Trigger DMX
#ifdef DMX_SUPPORT_NDDRM
    // _arDmxMMDRMAes[u1Idx].u4ChunkLen is useful especially for ND-DRM.
    _arDmxMMDRMAes[u1Idx].u4ChunkLen = u4SendSize;
#endif  // DMX_SUPPORT_NDDRM
    u4Reg = PID_S_W(u1Idx, 0);
    u4Reg &= ~0x18;     // clear toggle bit and commnad queue bit
    PID_S_W(u1Idx, 0) = u4Reg | 0x4;       // Enable output mode
    PID_S_W(u1Idx, 1) = u4SendSize;
    PID_S_W(u1Idx, 3) = (u4SkipLen << 24) | 1;

    _DMX_Lock();
    _u1CurStreamID = u1Idx;
    _rDmxMMData = *prData;
    _DMX_Unlock();

#ifdef DMX_MM_MOVEDATA_MEASURETIME
    HAL_GetTime(&rTime1);
#endif

    // Set DDI
    if(!_DMX_DDI_WaitTransfer(3, u4BufStart, u4BufEnd, u4DDIAddr, u4DDILen))
    {
        LOG(3, "DDI transaction failed!\n");
        return FALSE;
    }
#if 0
    VERIFY(OSR_OK == x_sema_lock(_hDmxMMSema, X_SEMA_OPTION_WAIT));
    UNUSED(u4TimeOut);
#else
    i4Ret = x_sema_lock_timeout(_hDmxMMSema, u4TimeOut);
    if(i4Ret != OSR_OK)
    {
        if(i4Ret == OSR_TIMEOUT)
        {
            u4MoveDataTimeoutCount++;
            LOG(3, "Timeout\n");
            return FALSE;
        }
        else
        {
            ASSERT(0);
        }
    }
#endif

#ifdef DMX_MM_MOVEDATA_MEASURETIME
    HAL_GetTime(&rTime2);

    HAL_GetDeltaTime(&rTimeDiff, &rTime1, &rTime2);
    LOG(1, "send time: %d.%06d\n", rTimeDiff.u4Seconds, rTimeDiff.u4Micros);
#endif

    _DMX_DDI_HWReset();

    return TRUE;
}


BOOL _DMX_PS_MoveData(UINT8 u1Idx, const DMX_PS_MOVEDATA_CMD_T *prData, UINT32 u4TimeOut)
{
    UINT32 u4BufStart, u4BufEnd, u4SendSize, u4SendAddr;
    UINT32 u4SkipLen, u4DDIAddr, u4DDILen;
    PID_STRUCT_T *prPidStruct;
#ifdef DMX_MM_MOVEDATA_MEASURETIME
    HAL_TIME_T rTime1, rTime2, rTimeDiff;
#endif
#if 1
    INT32 i4Ret;
#endif
    UINT32 u4Reg;
    UINT8 i;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);

    if(prData == NULL)
    {
        return FALSE;
    }

#ifdef DMX_MM_MOVEDATA_MEASURETIME
    HAL_GetTime(&rTime1);
    HAL_GetDeltaTime(&rTimeDiff, &_rTimePeriod, &rTime1);
    LOG(1, "period time: %d.%06d\n", rTimeDiff.u4Seconds, rTimeDiff.u4Micros);
    _rTimePeriod = rTime1;
#endif

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if(prPidStruct == NULL)
    {
        return FALSE;
    }

    if(!prPidStruct->fgEnable)
    {
        return FALSE;
    }

#ifdef CC_DMX_AUDIO_PREBUF
    if(!_DMX_AudHandler_UpdateRp(u1Idx, prData->u4FrameSize))
    {
        return FALSE;
    }
#endif  // CC_DMX_AUDIO_PREBUF

    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        if(_DMX_MM_GetBufEmptySize(u1Idx) < prData->u4FrameSize )
        {
            return FALSE;
        }
    }

    // Set DBM Bypass PID register, steer to uP
    // Clear bits [25:24] of the DBM Control Register to set DDI to TS index 3.
    // The next line is commented out because the default value of bits [25:24]
    // of the DBM Control Register is 0 (which is not changed since reset).
    //DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) & 0xFCFFFFFF);
    DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID_4, ((UINT32)u1Idx << 16) | 0x20);

    u4BufEnd = prData->u4BufEnd;
    u4BufStart = prData->u4BufStart;
    u4SendAddr = prData->u4StartAddr;
    u4SendSize = prData->u4FrameSize;

    // Meet the alignment and packet size (188 bytes) requirements.
    u4SkipLen = u4SendAddr % DDI_POINTER_ALIGNMENT;
    u4DDIAddr = u4SendAddr - u4SkipLen;
    u4DDILen  = u4SkipLen + u4SendSize + (188 - 1);  // Overflow? IMHO, no.
    u4DDILen -= u4DDILen % 188;

    LOG(7, "%d, Sendaddr: 0x%x, sendsize:0x%x, ddiaddr:0x%x, ddisize: 0x%x\n",
              u4SkipLen, u4SendAddr, u4SendSize, u4DDIAddr, u4DDILen);

    // Trigger DMX
#ifdef DMX_SUPPORT_NDDRM
    // _arDmxMMDRMAes[u1Idx].u4ChunkLen is useful especially for ND-DRM.
    _arDmxMMDRMAes[u1Idx].u4ChunkLen = u4SendSize;
#endif  // DMX_SUPPORT_NDDRM
    u4Reg = PID_S_W(u1Idx, 0);
    u4Reg &= ~0x10;     // clear toggle bit, make toggle bit the same as bypass register
    PID_S_W(u1Idx, 0) = u4Reg | 0xC;       // Enable output mode and commnand queue
    PID_S_W(u1Idx, 1) = u4SendSize;
    PID_S_W(u1Idx, 3) = (u4SkipLen << 24) | 1;

    // setup command queue
    PID_S_W(u1Idx, 17) =
        (DMX_PS_MOVE_DATA_Q_PIDX * 80) << 16 | ((DMX_PS_MOVE_DATA_Q_PIDX * 80) + prData->u1CmdIndex * 8);

    for(i = 0; i < prData->u1CmdIndex && i < DMX_PS_MOVE_DATA_Q_SIZE; i++)
    {
         PID_S_W(DMX_PS_MOVE_DATA_Q_PIDX, i * 2) = (prData->u2SkipLen[i]);
         PID_S_W(DMX_PS_MOVE_DATA_Q_PIDX, i * 2 + 1) = (prData->u2PayloadLen[i]);
    }

    _DMX_Lock();
    _u1CurStreamID = u1Idx;
    //_rDmxMMData = *prData;

    _rDmxMMData.u1Idx = prData->u1Idx;
    _rDmxMMData.u4BufStart = prData->u4BufStart;
    _rDmxMMData.u4BufEnd = prData->u4BufEnd;
    _rDmxMMData.u4StartAddr = prData->u4StartAddr;
    _rDmxMMData.u4FrameSize = prData->u4FrameSize;
    _rDmxMMData.u4Pts = prData->u4Pts;
    _rDmxMMData.u4Dts = prData->u4Dts;

    _rDmxMMData.fgEOS = FALSE;
    _rDmxMMData.fgFrameHead = FALSE;

    _DMX_Unlock();

#ifdef DMX_MM_MOVEDATA_MEASURETIME
    HAL_GetTime(&rTime1);
#endif

    // Set DDI
    if(!_DMX_DDI_WaitTransfer(3, u4BufStart, u4BufEnd, u4DDIAddr, u4DDILen))
    {
        LOG(3, "DDI transaction failed!\n");
        return FALSE;
    }
#if 0
    VERIFY(OSR_OK == x_sema_lock(_hDmxMMSema, X_SEMA_OPTION_WAIT));
    UNUSED(u4TimeOut);
#else
    i4Ret = x_sema_lock_timeout(_hDmxMMSema, u4TimeOut);
    if(i4Ret != OSR_OK)
    {
        if(i4Ret == OSR_TIMEOUT)
        {
            u4MoveDataTimeoutCount++;
            LOG(3, "Timeout\n");
            return FALSE;
        }
        else
        {
            ASSERT(0);
        }
    }
#endif

#ifdef DMX_MM_MOVEDATA_MEASURETIME
    HAL_GetTime(&rTime2);

    HAL_GetDeltaTime(&rTimeDiff, &rTime1, &rTime2);
    LOG(1, "send time: %d.%06d\n", rTimeDiff.u4Seconds, rTimeDiff.u4Micros);
#endif

    _DMX_DDI_HWReset();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_SetHeader
 *  Call in HISR
 */
//-----------------------------------------------------------------------------
void _DMX_MM_SetHeader(UINT8 u1Idx, DMX_PES_MSG_T *prPes)
{
    if(prPes == NULL)
    {
        LOG(3, "NULL pointer\n");
        return;
    }

    if(u1Idx != _u1CurStreamID)
    {
        prPes->u4Pts= 0;
        prPes->u4Dts= 0;
        prPes->fgPtsDts = FALSE;
        prPes->fgEOS = FALSE;

        LOG(3, "Index not match\n");
        ASSERT(0);
    }

    prPes->u4Pts= _rDmxMMData.u4Pts;
    prPes->u4Dts= _rDmxMMData.u4Dts;
    prPes->fgPtsDts = TRUE;

    prPes->fgEOS = _rDmxMMData.fgEOS;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_SetAudioHeader
 *  Call in HISR
 */
//-----------------------------------------------------------------------------
void _DMX_MM_SetAudioHeader(UINT8 u1Idx, DMX_AUDIO_PES_T *prPes)
{
    if(prPes == NULL)
    {
        LOG(3, "NULL pointer\n");
        return;
    }

    if(u1Idx != _u1CurStreamID)
    {
        prPes->u4Pts= 0;
        prPes->u4Dts= 0;
        prPes->fgEOS = FALSE;
        prPes->fgIgnoreData = FALSE;

        LOG(3, "Index not match\n");
        ASSERT(0);
    }

    prPes->u4Pts= _rDmxMMData.u4Pts;
    prPes->u4Dts= _rDmxMMData.u4Dts;
    prPes->fgEOS = _rDmxMMData.fgEOS;
    prPes->fgIgnoreData = _rDmxMMData.fgIgnoreData;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_ISRHandler
 *  Call in HISR
 */
//-----------------------------------------------------------------------------
void _DMX_MM_ISRHandler(UINT8 u1Idx)
{
    if(u1Idx != _u1CurStreamID)
    {
        LOG(3, "Index not match\n");
        ASSERT(0);
    }

    VERIFY(OSR_OK == x_sema_unlock(_hDmxMMSema));
}


BOOL _DMX_MM_IsFrameHead(void)
{
    if (_rDmxMMData.fgFrameHead)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//-----------------------------------------------------------------------------
/** _DMX_MM_SetDRMMode
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_SetDRMMode(UINT8 u1Idx, DMX_DRM_MODE_T eMode)
{
    UINT32 u4Reg;

    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        LOG(3, "Invalid index\n");
        return FALSE;
    }

    _arDmxMMDRMMode[u1Idx] = eMode;

    if(eMode == DMX_DRM_MODE_BYPASS)
    {
        u4Reg = PID_S_W(u1Idx, 0);
        u4Reg &= 0x00FFFFFF;
        PID_S_W(u1Idx, 0) = u4Reg;
    }
    else if(eMode == DMX_DRM_MODE_AES)
    {
        u4Reg = PID_S_W(u1Idx, 0);
        u4Reg &= 0x00FFFFFF;
        u4Reg |= 0x01000000;        // AES mode
        PID_S_W(u1Idx, 0) = u4Reg;

        // Set AES engine to be controlled by uP
        u4Reg = DMXCMD_READ32(DMX_REG_CONFIG1);
        u4Reg |= (1 << 11);
        DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Reg);
    }
#ifdef DMX_SUPPORT_NDDRM
    else if (eMode == DMX_DRM_MODE_ND)
    {
        // Note: Users need to make sure this function is called AFTER the
        //       function _DMX_MM_Set() has been called.
        //       Otherwise, DMEM Word 0 will be overwritten.

        x_memset((void*)(&(_arDmxMMDRMAes[u1Idx])), 0, sizeof(_arDmxMMDRMAes[u1Idx]));
        _arDmxMMDRMAes[u1Idx].u4KeyBitLen = 128;
        // _arDmxMMDRMAes[u1Idx].fgCBC = FALSE;  // redundant

        // 0x01XX82XX: AES / ECB / AES encryption / 128 bits / Counter mode
        u4Reg = PID_S_W(u1Idx, 0) & 0x00FF00FF;
        u4Reg |= 0x01008200;
        PID_S_W(u1Idx, 0) = u4Reg;

        // Always set the word "AES Offset Length" to 0.
        PID_S_W(u1Idx, 15) = 0;

        // Set AES engine to be controlled by uP
        u4Reg = DMXCMD_READ32(DMX_REG_CONFIG1);
        u4Reg |= (1 << 11);
        DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Reg);
    }
#endif  // DMX_SUPPORT_NDDRM
    else
    {
        LOG(1, "Not support yet\n");
        ASSERT(0);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_GetDRMMode
*/
//-----------------------------------------------------------------------------
DMX_DRM_MODE_T _DMX_MM_GetDRMMode(UINT8 u1Idx)
{
    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        LOG(3, "Invalid index\n");
        return DMX_DRM_MODE_UNKNOWN;
    }

    return _arDmxMMDRMMode[u1Idx];
}


//-----------------------------------------------------------------------------
/** _DMX_MM_SetAES
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_SetAES(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    UINT32 i, j, u4Reg;

    if(prDRM == NULL)
    {
        return FALSE;
    }

    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        LOG(3, "Invalid index\n");
        return FALSE;
    }

    if (u4Flags == DMX_MM_DRM_FLAG_NONE)
    {
        return TRUE;
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_OFFSET) != 0)
    {
        _arDmxMMDRMAes[u1Idx].u4Offset = prDRM->u4Offset;
        PID_S_W(u1Idx, 15) = prDRM->u4Offset;            // Set decrypt offset
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_ENCRYLEN) != 0)
    {
        _arDmxMMDRMAes[u1Idx].u4EncryLen = prDRM->u4EncryLen;
        PID_S_W(u1Idx, 2) = prDRM->u4EncryLen;
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_CBC) != 0)
    {
        UINT32 au4IV[4];

        u4Reg = PID_S_W(u1Idx, 0);
        u4Reg &= 0xFFFFFEFF;
        _arDmxMMDRMAes[u1Idx].fgCBC = prDRM->fgCBC;
        if(prDRM->fgCBC)
        {
            u4Reg |= 0x100;

            for(i=0; i<16; i++)
            {
                _arDmxMMDRMAes[u1Idx].au1IV[i] = prDRM->au1IV[i];
            }

            for(i=0; i<4; i++)
            {
                au4IV[i] = 0;
                for(j=0; j<4; j++)
                {
                    au4IV[i] |= (((UINT32)prDRM->au1IV[(i*4)+j]) << ((3 - j) * 8));
                }
            }

            for (i = 0; i < 4; i++)
            {
                *(volatile UINT32*)(DMX_DMEM_MM_IV_BASE + (4 * i)) = au4IV[3 - i];            
            }
        }

        PID_S_W(u1Idx, 0) = u4Reg;
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_KEY) != 0)
    {
        _arDmxMMDRMAes[u1Idx].u4KeyBitLen = prDRM->u4KeyBitLen;

        u4Reg = PID_S_W(u1Idx, 0);
        u4Reg &= 0xFFFFF3FF;

        if(prDRM->u4KeyBitLen == 128)
        {
            u4Reg |= 0x0;
        }
        else if(prDRM->u4KeyBitLen == 192)
        {
            u4Reg |= 0x400;
        }
        else if(prDRM->u4KeyBitLen == 256)
        {
            u4Reg |= 0x800;
        }
        else
        {
            LOG(1, "Unknow key length\n");
            ASSERT(0);
        }

        PID_S_W(u1Idx, 0) = u4Reg;

        for(i=0; i<32; i++)
        {
            _arDmxMMDRMAes[u1Idx].au1Key[i] = prDRM->au1Key[i];
        }

        for(i=0; i<8; i++)
        {
            *(volatile UINT32*)(DMX_DMEM_MM_KEY_BASE + ((7 - i) * 4)) =
                                             prDRM->au1Key[(i*4) + 3] |
                                            (prDRM->au1Key[(i*4) + 2] << 8) |
                                            (prDRM->au1Key[(i*4) + 1] << 16) |
                                            (prDRM->au1Key[i*4] << 24);
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_GetAES
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_GetAES(UINT8 u1Idx, UINT32 u4Flags, DMX_MM_DRM_AES_T *prDRM)
{
    UINT32 i;

    if(prDRM == NULL)
    {
        return FALSE;
    }

    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        LOG(3, "Invalid index\n");
        return FALSE;
    }

    if (u4Flags == DMX_MM_DRM_FLAG_NONE)
    {
        return TRUE;
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_OFFSET) != 0)
    {
        prDRM->u4Offset = _arDmxMMDRMAes[u1Idx].u4Offset;
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_ENCRYLEN) != 0)
    {
        prDRM->u4EncryLen = _arDmxMMDRMAes[u1Idx].u4EncryLen;
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_CBC) != 0)
    {
        prDRM->fgCBC = _arDmxMMDRMAes[u1Idx].fgCBC;
        if(prDRM->fgCBC)
        {
            for(i=0; i<16; i++)
            {
                prDRM->au1IV[i] = _arDmxMMDRMAes[u1Idx].au1IV[i];
            }
        }
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_KEY) != 0)
    {
        _arDmxMMDRMAes[u1Idx].u4KeyBitLen = prDRM->u4KeyBitLen;

        for(i=0; i<32; i++)
        {
            prDRM->au1Key[i] = _arDmxMMDRMAes[u1Idx].au1Key[i];
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_UnlockBuffer
*   Unlock subtitle buffer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_UnlockBuffer(UINT8 u1Idx, UINT8 u1SerialNumber, UINT32 u4Size,
    UINT32 u4FrameAddr)
{
    return _DMX_PES_UnlockBuffer(u1Idx, u1SerialNumber, u4Size, u4FrameAddr);
}


//-----------------------------------------------------------------------------
/** _DMX_MM_GetBuffer
*    Copy subtitle data to middleware
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_GetBuffer(UINT8 u1Idx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
    UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest)
{
    return _DMX_PES_GetBuffer(u1Idx, u1SerialNumber, u4FrameAddr, u4SkipSize,
                              u4CopySize, pu1Dest);
}


//-----------------------------------------------------------------------------
/** _DMX_MM_FlushBuffer
*
*   The flush of the buffers for TS/PS is written in this function for
*   compatibility reason so that the ME team does not need to modify their
*   code.  The caller of this function must make sure that no data transmission
*   is requested (or taking place) when this function is called.  (2009/04/24)
*
* ATTENTION:
*   This implementation only works with DRM uCode and the packetized
*   implementation of the uCode.  This means it shall work with all versions
*   of TS uCode, but it can only work with the 3.15 (not the 3.14) blood line
*   of PS uCode.
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_FlushBuffer(UINT8 u1Idx)
{
    PID_STRUCT_T* prPidStruct;
    UINT32 u4BufStart, u4PhyBufStart;

    if (u1Idx >= DMX_MM_STRUCT_NUM)
    {
        LOG(3, "Invalid index (%d)!\n", u1Idx);
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    _DMX_Lock();

    // For DRM uCode, do not set byte 0 of word 3 to non-zero value.
    PID_S_W(u1Idx, 4) = 0xFFFFFFFF;     // Reset previous payload counter to FFFFFFFF
    PID_S_W(u1Idx, 18) = 0xFFFFFFFF;    // Reset previous payload counter to FFFFFFFF

    u4BufStart = prPidStruct->u4BufStart;

    prPidStruct->u4Wp = u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4PesRp = u4BufStart;
    u4PhyBufStart = PHYSICAL(u4BufStart);
    PID_S_W(u1Idx, 7) = u4PhyBufStart;     // PES start
    PID_S_W(u1Idx, 8) = u4PhyBufStart;     // Write pointer
    PID_S_W(u1Idx, 9) = u4PhyBufStart;     // Read pointer

    switch (prPidStruct->ePidType)
    {
    case DMX_PID_TYPE_PSI:
        PID_S_W(u1Idx, 3) = 0xffff0002;
        prPidStruct->u4SectionRp = prPidStruct->u4BufStart;
        break;

    case DMX_PID_TYPE_ES_VIDEO:
    case DMX_PID_TYPE_ES_AUDIO:
    case DMX_PID_TYPE_ES_VIDEOCLIP:
    case DMX_PID_TYPE_ES_OTHER:
        u4BufStart = PHYSICAL(prPidStruct->u4HeaderBufAddr);
        PID_S_W(u1Idx, 12) = u4BufStart;    // Header Start
        PID_S_W(u1Idx, 13) = u4BufStart;    // Header Write pointer
        PID_S_W(u1Idx, 14) = u4BufStart;    // Header Read pointer
        prPidStruct->u4NextPic = 0;
        break;
    case DMX_PID_TYPE_PES:
        prPidStruct->u4PesRp = prPidStruct->u4BufStart;
        break;
    default:
        break;
    }

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_TSFlushBuffer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_TSFlushBuffer(UINT8 u1Idx)
{
    PID_STRUCT_T* prPidStruct;
    UINT32 u4BufStart, u4PhyBufStart;
    UINT32 u4ActiveFlag = 0, u4PidTblValue = 0, u4W0 = 0;
    BOOL fgPidEnabled = FALSE;

    if (u1Idx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid index (%d)!\n", u1Idx);
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if(_DMX_IsPidEnabled(u1Idx))
    {
        fgPidEnabled = TRUE;

        _DMX_Lock();
        u4PidTblValue = PID_INDEX_TABLE(u1Idx);
        u4W0 = PID_S_W(u1Idx, 0);
        prPidStruct->u4NextPic = 0;
        prPidStruct->u1SerialNumber++;
        if (prPidStruct->u1SerialNumber > DMX_MAX_SERIAL_NUM)
        {
            prPidStruct->u1SerialNumber = 0;
        }

        // Disable PID index table again.
        PID_INDEX_TABLE(u1Idx) &= 0x7FFFFFDF;  // Clear Valid and "to-uP" flags.
        // Read the inverse of bit 8 of the PID Index Table.
        u4ActiveFlag = (UINT32)(((u4PidTblValue & (1 << 8)) == 0) ? 1 : 0);
        // Copy the inverse to bit 4 of the first word in PID memory.
        PID_S_W(u1Idx, 0) = (u4W0 & ~(1 << 4)) | (u4ActiveFlag << 4);
        prPidStruct->fgEnable = FALSE;
        _DMX_Unlock();
    }

    // TS file using toggle bit mechanism to improve performance
    HAL_Delay_us(1);

    _DMX_Lock();

    PID_S_W(u1Idx, 1) = 0;
    //PID_S_W(u1Idx, 2) = (PID_S_W(u1Idx, 2) & 0xFF) | 0x0000ff00;  // Scramble state = unknown
    PID_S_W(u1Idx, 3) = 0xffff0001;         // For PES (TS/PS uCode)
    PID_S_W(u1Idx, 4) = 0xFFFFFFFF;     // Reset previous payload counter to FFFFFFFF
    PID_S_W(u1Idx, 18) = 0xFFFFFFFF;    // Reset previous payload counter to FFFFFFFF

    u4BufStart = prPidStruct->u4BufStart;

    prPidStruct->u4Wp = u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4PesRp = u4BufStart;
    u4PhyBufStart = PHYSICAL(u4BufStart);
    PID_S_W(u1Idx, 7) = u4PhyBufStart;     // PES start
    PID_S_W(u1Idx, 8) = u4PhyBufStart;     // Write pointer
    PID_S_W(u1Idx, 9) = u4PhyBufStart;     // Read pointer

    switch (prPidStruct->ePidType)
    {
    case DMX_PID_TYPE_PSI:
        PID_S_W(u1Idx, 3) = 0xffff0002;
        prPidStruct->u4SectionRp = prPidStruct->u4BufStart;
        break;

    case DMX_PID_TYPE_ES_VIDEO:
    case DMX_PID_TYPE_ES_AUDIO:
    case DMX_PID_TYPE_ES_VIDEOCLIP:
    case DMX_PID_TYPE_ES_OTHER:
        u4BufStart = PHYSICAL(prPidStruct->u4HeaderBufAddr);
        PID_S_W(u1Idx, 12) = u4BufStart;    // Header Start
        PID_S_W(u1Idx, 13) = u4BufStart;    // Header Write pointer
        PID_S_W(u1Idx, 14) = u4BufStart;    // Header Read pointer
        prPidStruct->u4NextPic = 0;
        break;
    case DMX_PID_TYPE_PES:
        prPidStruct->u4PesRp = prPidStruct->u4BufStart;
        break;
    default:
        break;
    }

    _DMX_Unlock();

    if(fgPidEnabled)
    {
        _DMX_Lock();
        // Copy bit 4 of the stored DMEM word 0 to bit 8 of the PID Index Table.
        PID_INDEX_TABLE(u1Idx) = (u4PidTblValue & ~(1 << 8)) | ((u4W0 & (1 << 4)) << 4);
        PID_S_W(u1Idx, 0) = u4W0;
        prPidStruct->fgEnable = TRUE;
        _DMX_Unlock();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_ESFlushBuffer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_ESFlushBuffer(UINT8 u1Idx)
{
    PID_STRUCT_T* prPidStruct;
    UINT32 u4BufStart, u4PhyBufStart;

    if (u1Idx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid index (%d)!\n", u1Idx);
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    _DMX_Lock();

    // For DRM uCode, do not set byte 0 of word 3 to non-zero value.
    PID_S_W(u1Idx, 4) = 0xFFFFFFFF;     // Reset previous payload counter to FFFFFFFF
    PID_S_W(u1Idx, 18) = 0xFFFFFFFF;    // Reset previous payload counter to FFFFFFFF

    u4BufStart = prPidStruct->u4BufStart;

    prPidStruct->u4Wp = u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4PesRp = u4BufStart;
    u4PhyBufStart = PHYSICAL(u4BufStart);
    PID_S_W(u1Idx, 7) = u4PhyBufStart;     // PES start
    PID_S_W(u1Idx, 8) = u4PhyBufStart;     // Write pointer
    PID_S_W(u1Idx, 9) = u4PhyBufStart;     // Read pointer

    switch (prPidStruct->ePidType)
    {
    case DMX_PID_TYPE_PSI:
        PID_S_W(u1Idx, 3) = 0xffff0002;
        prPidStruct->u4SectionRp = prPidStruct->u4BufStart;
        break;

    case DMX_PID_TYPE_ES_VIDEO:
    case DMX_PID_TYPE_ES_AUDIO:
    case DMX_PID_TYPE_ES_VIDEOCLIP:
    case DMX_PID_TYPE_ES_OTHER:
        u4BufStart = PHYSICAL(prPidStruct->u4HeaderBufAddr);
        PID_S_W(u1Idx, 12) = u4BufStart;    // Header Start
        PID_S_W(u1Idx, 13) = u4BufStart;    // Header Write pointer
        PID_S_W(u1Idx, 14) = u4BufStart;    // Header Read pointer
        prPidStruct->u4NextPic = 0;
        break;
    case DMX_PID_TYPE_PES:
        prPidStruct->u4PesRp = prPidStruct->u4BufStart;
        break;
    default:
        break;
    }

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_AllocateBuffer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_AllocateBuffer(UINT8 u1Idx, UINT32 u4Size)
{
    DMX_MM_T rMM;

    rMM.fgAllocBuf = TRUE;
    rMM.u4BufSize = u4Size;

    if(!_DMXMMAllocBuf(u1Idx, &rMM))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_FreeBuffer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_FreeBuffer(UINT8 u1Idx)
{
    return _DMXMMFreeBuf(u1Idx);
}


#ifdef DMX_SUPPORT_NDDRM
//-----------------------------------------------------------------------------
/** _DMX_MM_SetNDDRM
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MM_SetNDDRM(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    INT32 i;

    if(prDRM == NULL)
    {
        return FALSE;
    }

    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        LOG(3, "%s(): invalid index %d!\n", __func__, u1Idx);
        return FALSE;
    }

    if (u4Flags == DMX_MM_DRM_FLAG_NONE)
    {
        return TRUE;
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_CHUNK_LEN) != 0)
    {
        _arDmxMMDRMAes[u1Idx].u4ChunkLen = prDRM->u4ChunkLen;
        PID_S_W(u1Idx, 1) = prDRM->u4ChunkLen;
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_ENCRYLEN) != 0)
    {
        _arDmxMMDRMAes[u1Idx].u4EncryLen = prDRM->u4EncryLen;
        PID_S_W(u1Idx, 2) = prDRM->u4EncryLen;
    }

    // ND-DRM always makes use of 128-bits (16-bytes) AES encryption.
    if ((u4Flags & DMX_MM_DRM_FLAG_KEY) != 0)
    {
        x_memset((void*)(_arDmxMMDRMAes[u1Idx].au1Key), 0, sizeof(_arDmxMMDRMAes[u1Idx].au1Key));

        for(i = 0; i < 16; i++)
        {
            _arDmxMMDRMAes[u1Idx].au1Key[i] = prDRM->au1Key[i];
        }
/*      xxx, fixme
        for(i = 0; i < 4; i++)
        {
            *(volatile UINT32*)(DMX_DMEM_CA_KEY_BASE + ((7 - i) * 4)) =
                                     prDRM->au1Key[(i*4) + 3]        |
                                    (prDRM->au1Key[(i*4) + 2] << 8)  |
                                    (prDRM->au1Key[(i*4) + 1] << 16) |
                                    (prDRM->au1Key[(i*4) + 0] << 24);
        }
*/
    }

    // The Sample ID is 8 bytes in length.
    if ((u4Flags & DMX_MM_DRM_FLAG_SAMPLE_ID) != 0)
    {
        for (i = 0; i < 8; i++)
        {
            _arDmxMMDRMAes[u1Idx].au1SampleID[i] = prDRM->au1SampleID[i];
        }
/*  xxxx, fixme
        for (i = 0; i < 2; i++)
        {
            *(volatile UINT32*)(DMX_DMEM_CA_KEY_BASE + ((13 - i) * 4)) =
                                     prDRM->au1SampleID[(i*4) + 3]        |
                                    (prDRM->au1SampleID[(i*4) + 2] << 8)  |
                                    (prDRM->au1SampleID[(i*4) + 1] << 16) |
                                    (prDRM->au1SampleID[(i*4) + 0] << 24);
        }
*/
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_NDDRM_MoveData
*/
//-----------------------------------------------------------------------------
BOOL _DMX_NDDRM_MoveData(UINT8 u1Idx, const DMX_MM_DATA_T *prData, UINT32 u4TimeOut)
{
    UINT32 u4BufStart, u4BufEnd, u4SendSize, u4SendAddr;
    UINT32 u4SkipLen, u4DDIAddr, u4DDILen;
    INT32 i;
    UINT8 u1PacketSize;
    BOOL fgTrigger;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);

    if(prData == NULL)
    {
        return FALSE;
    }

/*
    // Set DBM Bypass PID register, steer to uP
#if defined(CC_MT5395) || defined(CC_MT5365)
    // Clear bits [25:24] of the DBM Control Register to set DDI to TS index 3.
    // The next line is commented out because the default value of bits [25:24]
    // of the DBM Control Register is 0 (which is not changed since reset).
    //DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) & 0xFCFFFFFF);
    DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID_4, ((UINT32)u1Idx << 16) | 0x20);
#else
    #error "Customization is needed above."
#endif
*/
    u4BufEnd = prData->u4BufEnd;
    u4BufStart = prData->u4BufStart;
    u4SendAddr = prData->u4StartAddr;
    u4SendSize = prData->u4FrameSize;

    u1PacketSize = _DMX_DDI_GetPacketSize();

    // Meet the alignment and packet size (188 bytes) requirements.
    u4SkipLen = u4SendAddr % DDI_POINTER_ALIGNMENT;
    u4DDIAddr = u4SendAddr - u4SkipLen;
    u4DDILen  = u4SkipLen + u4SendSize + (u1PacketSize - 1);  // Overflow? IMHO, no.
    u4DDILen -= u4DDILen % u1PacketSize;

    LOG(7, "%d, Sendaddr: 0x%x, sendsize:0x%x, ddiaddr:0x%x, ddisize: 0x%x\n",
              u4SkipLen, u4SendAddr, u4SendSize, u4DDIAddr, u4DDILen);

    for(i = 0; i < MM_WAITDMX_POLLING_TIMES; i++)
    {
        if(((PID_S_W(u1Idx, 3)) & 0x1) == 0)
        {
            break;
        }
        x_thread_delay(2);
    }

    if(i == MM_WAITDMX_POLLING_TIMES)
    {
        LOG(3, "DMX not stopped\n");
        return FALSE;
    }


    // Trigger DMX if fgTrigger is TRUE.
    // When the Trigger is set, the Block ID (counter for ND-DRM) is reset to 0.
    // The "Pre-byte length" can be cleared to 0 as long as each chunck length
    // is a multiple of 16 bytes.  When the "PES Complete" interrupt is set, the
    // field "Number of Start Codes" can be cleared to 0.
    fgTrigger = prData->fgTrigger;
    if (fgTrigger)
    {
        PID_S_W(u1Idx, 3) = (u4SkipLen << 24) | 1;
    }
    else
    {
        PID_S_W(u1Idx, 3) = (u4SkipLen << 24) | 2;
    }

    _DMX_Lock();
    _u1CurStreamID = u1Idx;
    _rDmxMMData = *prData;
    _DMX_Unlock();

    // Set DDI
    if(!_DMX_DDI_SingleMove(u4BufStart, u4BufEnd, u4DDIAddr, u4DDILen))
    {
        LOG(3, "DDI transaction failed!\n");
        return FALSE;
    }

    VERIFY(OSR_OK == x_sema_lock(_hDmxMMSema, X_SEMA_OPTION_WAIT));
    UNUSED(u4TimeOut);

    return TRUE;
}
#endif  // DMX_SUPPORT_NDDRM


//-----------------------------------------------------------------------------
/** _DMX_MM_SendAudioChunk
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MM_SendAudioChunk(UINT8 u1Pidx, UINT32 u4Wp, UINT32 u4Pts, BOOL fgEOS)
{
    DMX_AUDIO_PES_T rPes;
    PID_STRUCT_T* prPidStruct;
    UINT32 u4NewWp;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    u4NewWp = VIRTUAL(u4Wp - 1);

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    if((u4NewWp < prPidStruct->u4BufStart) || (u4NewWp > prPidStruct->u4BufEnd))
    {
        return FALSE;
    }

    // Update WP
    prPidStruct->u4Wp = VIRTUAL(u4NewWp);

    if(!_DMX_IsToDecoder())
    {
        return TRUE;
    }

    rPes.u1PidIndex = u1Pidx;
    rPes.u4Wp = PHYSICAL(u4NewWp);
    rPes.u1DeviceId = prPidStruct->u1DeviceId;
    rPes.u4Pts = u4Pts;
    rPes.fgEOS = FALSE;
    rPes.fgCopyright = FALSE;
    rPes.fgOriginal = FALSE;

    VERIFY(_DMX_SendAudioPes(&rPes));

    if(fgEOS)
    {
        x_memset((void*)&rPes, 0, sizeof(rPes));
        rPes.fgEOS = TRUE;
        rPes.u1PidIndex = u1Pidx;
        rPes.u1DeviceId = prPidStruct->u1DeviceId;

        VERIFY(_DMX_SendAudioPes(&rPes));
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MM_GetEnabledPidIndex
 */
//-----------------------------------------------------------------------------
UINT8 _DMX_MM_GetEnabledPidIndex(DMX_INPUT_TYPE_T eInputType, DMX_PID_TYPE_T ePidType)
{
    PID_STRUCT_T* prPidStruct;
    UINT8 i;

    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        prPidStruct = _DMX_GetPidStruct(i);
        if((prPidStruct->fgEnable) && (prPidStruct->eInputType == eInputType) &&
           (prPidStruct->ePidType == ePidType))
        {
            return i;
        }
    }

    return 0xFF;
}

