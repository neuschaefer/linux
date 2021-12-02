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
 * $RCSfile: dmx_pes.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_pes.c
 *  Demux driver - PES-to-memory helpers
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_debug.h"
#include "x_assert.h"
#include "x_os.h"


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


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_PES_AllocateBuffer
 *  Allocate PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Size          Buffer size
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PES_AllocateBuffer(UINT8 u1Pidx, UINT32 u4Size)
{
    DMX_PID_T rPid;

    LOG(6, "Allocate PES buffer: pidx %u, size: %u\n", u1Pidx, u4Size);

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    rPid.u4BufAddr = 0;
    rPid.u4BufSize = u4Size;
    rPid.ePidType = DMX_PID_TYPE_PES;
    rPid.fgAllocateBuffer = TRUE;

    if (!_DMX_SetPid(u1Pidx, DMX_PID_FLAG_BUFFER, &rPid, FALSE))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PES_FreeBuffer
 *  Free a PES buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PES_FreeBuffer(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;
    BOOL fgRet;

    LOG(6, "Free PES buffer: pidx %u\n", u1Pidx);

    // Check if it's a PES PID
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if (prPidStruct->ePidType != DMX_PID_TYPE_PES)
    {
        return FALSE;
    }

    fgRet = _DMX_FreePidBuffer(u1Pidx);

    _DMX_Lock();
    prPidStruct->ePidType = DMX_PID_TYPE_NONE;
    _DMX_Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_PES_UnlockBuffer
 *  Unlock a PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PES_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4Size,
    UINT32 u4FrameAddr)
{
    PID_STRUCT_T rPidStruct;
    PID_STRUCT_T* prPidStruct;
    UINT32 u4Rp;

    LOG(6, "Unlock PES buffer: pidx %u, frame: 0x%08x, size: %u\n", u1Pidx,
        u4FrameAddr, u4Size);

    // Check if it's a PES PID
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    _DMX_Lock();

    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    rPidStruct = *prPidStruct;

    _DMX_Unlock();

    if ((u1SerialNumber <= DMX_MAX_SERIAL_NUM) &&
        (rPidStruct.u1SerialNumber != u1SerialNumber))
    {
        // Unmatched serial numbers
        LOG(3, "Pidx %u: unmatched serial number %u [%u]\n", u1Pidx,
            u1SerialNumber, rPidStruct.u1SerialNumber);
        return FALSE;
    }

    if (rPidStruct.ePidType != DMX_PID_TYPE_PES)
    {
        return FALSE;
    }

    u4Rp = _DMX_AdvanceAddr(u4FrameAddr, (INT32)u4Size, rPidStruct.u4Wp,
        rPidStruct.u4BufStart, rPidStruct.u4BufEnd + 1);

    return _DMX_UpdateReadPointer(u1Pidx, u4Rp, u4FrameAddr);
}


//-----------------------------------------------------------------------------
/** _DMX_PES_FlushBuffer
 *  Flush a PES buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PES_FlushBuffer(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;

    LOG(6, "Flush PES buffer: pidx %u\n", u1Pidx);

    // Check if it's a PES PID
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if (prPidStruct->ePidType != DMX_PID_TYPE_PES)
    {
        return FALSE;
    }

    return _DMX_FlushPidBuffer(u1Pidx);
}


//-----------------------------------------------------------------------------
/** _DMX_PES_GetBuffer
 *  Copy data from a PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param  pu1Dest         Destination buffer
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PES_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
    UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest)
{
    PID_STRUCT_T rPidStruct;
    PID_STRUCT_T* prPidStruct;
    UINT32 u4Rp, u4Wp, u4BufSize, u4DataSize = 0, u4TotalSize, u4Size;
    BOOL fgRet = FALSE;

    if ((u1Pidx >= DMX_NUM_PID_INDEX) || (pu1Dest == NULL))
    {
        return FALSE;
    }

    LOG(7, "Get PES buffer: pidx %u, frame: 0x%08x, skip: %u, copy: %u\n",
        u1Pidx, u4FrameAddr, u4SkipSize, u4CopySize);

    _DMX_Lock();

    // Get PID structure
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    rPidStruct = *prPidStruct;

    _DMX_Unlock();

    if ((u1SerialNumber <= DMX_MAX_SERIAL_NUM) &&
        (rPidStruct.u1SerialNumber != u1SerialNumber))
    {
        // Unmatched serial numbers
        LOG(3, "Pidx %u: unmatched serial number %u [%u]\n", u1Pidx,
            u1SerialNumber, rPidStruct.u1SerialNumber);
        return FALSE;
    }

    // Check if it's a PES PID
    if (rPidStruct.ePidType != DMX_PID_TYPE_PES)
    {
        LOG(3, "Not a PES PID!\n");
        goto _Exit;
    }

    // Compute avalible data size
    u4Rp = rPidStruct.u4Rp;
    u4Wp = rPidStruct.u4Wp;
    u4BufSize = rPidStruct.u4BufLen;
    u4DataSize = DATASIZE(u4FrameAddr, u4Wp, u4BufSize);
    u4TotalSize = DATASIZE(u4Rp, u4Wp, u4BufSize);

    // Check framer address
    if ((u4FrameAddr < rPidStruct.u4BufStart) ||
        (u4FrameAddr > rPidStruct.u4BufEnd) ||
        (u4DataSize > u4TotalSize))
    {
        LOG(3, "Invalid frame address! RP: 0x%08x, WP: 0x%08x\n", u4Rp, u4Wp);
        LOG(3, "    Buf: %u, org_size: %u, new_size: %u\n", u4BufSize,
            u4TotalSize, u4DataSize);
        goto _Exit;
    }

    // Check if available data size is enough
    if ((u4SkipSize + u4CopySize) > u4DataSize)
    {
        // No such many data
        LOG(3, "Not enough data!\n");
        goto _Exit;
    }

    // Skip specified bytes
    u4Rp = u4FrameAddr + u4SkipSize;
    if (u4Rp > rPidStruct.u4BufEnd)
    {
        u4Rp -= u4BufSize;
    }

    // Copy data
    if ((u4Rp > u4Wp) && (u4CopySize > 0))
    {
        u4Size = MIN((rPidStruct.u4BufEnd - u4Rp) + 1, u4CopySize);
        x_memcpy(pu1Dest, (void*)u4Rp, u4Size);
        u4Rp += u4Size;
        if (u4Rp > rPidStruct.u4BufEnd)
        {
            u4Rp = rPidStruct.u4BufStart;
        }
        pu1Dest += u4Size;
        u4CopySize -= u4Size;
    }

    if ((u4Rp < u4Wp) && (u4CopySize > 0))
    {
        u4Size = MIN(u4Wp - u4Rp, u4CopySize);
        x_memcpy(pu1Dest, (void*)u4Rp, u4Size);
    }

    fgRet = TRUE;

_Exit:

    if (!fgRet)
    {
        LOG(3, "Fail to get PES data!\n");
        LOG(3, " Pidx: %u, frame: 0x%08x, data: %u, skip: %u, copy: %u\n",
            u1Pidx, u4FrameAddr, u4DataSize, u4SkipSize, u4CopySize);
    }

    return fgRet;
}

