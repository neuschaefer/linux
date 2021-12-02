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
 * $RCSfile: fvr_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fvr_psi.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fvr_debug.h"
#include "fvr.h"

#include "x_assert.h"

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
/** _FVRUpdateReadPointer
 */
//-----------------------------------------------------------------------------
static BOOL _FVRUpdateReadPointer(UINT8 u1Pidx, UINT32 u4Rp, UINT32 u4FrameAddr)
{
    UINT32 u4BufStart, u4BufEnd, u4DataSize, u4UpdateSize;
    FVR_PID_STRUCT_T* prPidStruct;
    FVR_PID_STRUCT_T rPidStruct;

    // Check if pid index is valid
    if (u1Pidx >= FVR_NUM_PID_INDEX)
    {
        LOG(3, "Update RP: invalid PID index %u\n", u1Pidx);
        return FALSE;
    }

    _FVR_Lock();

    // Check if RP out of range
    prPidStruct = _FVR_GetPidStruct(u1Pidx);
    rPidStruct = *prPidStruct;

    _FVR_Unlock();

    u4BufStart = rPidStruct.u4BufStart;
    u4BufEnd = rPidStruct.u4BufEnd + 1;
    u4Rp = VIRTUAL(u4Rp);
    if ((u4Rp < u4BufStart) || (u4Rp >= u4BufEnd))
    {
        // RP out of range
        LOG(3, "Pidx: %u, Invalid RP: 0x%08x\n", u1Pidx, u4Rp);
        return FALSE;
    }

    // Check update size
    u4DataSize = DATASIZE(rPidStruct.u4Rp, rPidStruct.u4Wp,
        rPidStruct.u4BufLen);
    u4UpdateSize = DATASIZE(rPidStruct.u4Rp, u4Rp, rPidStruct.u4BufLen);
    if (u4UpdateSize > u4DataSize)
    {
        // Update size is larger than data size, something wrong
        LOG(3, "Pidx: %u, Invalid RP 2: 0x%08x, data size: %u, update size: %u\n",
            u1Pidx, u4Rp, u4DataSize, u4UpdateSize);
        return FALSE;
    }

    _FVR_Lock();
    FVR_PER_PID_S_W(u1Pidx, 9) = PHYSICAL(u4Rp);
    prPidStruct->u4Rp = u4Rp;
    _FVR_Unlock();

    // Debug
    LOG(7, "Update RP - pidx: %u, RP: 0x%08x, frame: 0x%08x\n", u1Pidx, u4Rp, u4FrameAddr);

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _FVR_PSI_UnlockBuffer
 *  Unlock a section buffer
 */
//-----------------------------------------------------------------------------
BOOL _FVR_PSI_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4Size,
    UINT32 u4FrameAddr)
{
    FVR_PID_STRUCT_T rPidStruct;
    FVR_PID_STRUCT_T* prPidStruct;
    UINT32 u4Rp;

    LOG(6, "Unlock PSI buffer: pidx %u, frame: 0x%08x, size: %u\n", u1Pidx,
        u4FrameAddr, u4Size);

    // Check if it's a PSI PID
    ASSERT(u1Pidx < FVR_NUM_PID_INDEX);

    _FVR_Lock();

    prPidStruct = _FVR_GetPidStruct(u1Pidx);
    rPidStruct = *prPidStruct;

    _FVR_Unlock();

    if ((u1SerialNumber <= DMX_MAX_SERIAL_NUM) &&
        (rPidStruct.u1SerialNumber != u1SerialNumber))
    {
        // Unmatched serial numbers
        LOG(3, "Pidx %u: unlock, unmatched serial number %u [%u]\n", u1Pidx,
            u1SerialNumber, rPidStruct.u1SerialNumber);
        return FALSE;
    }

    if (rPidStruct.ePidType != DMX_PID_TYPE_PSI)
    {
        return FALSE;
    }

    u4Rp = _DMX_AdvanceAddr(u4FrameAddr, (INT32)u4Size, rPidStruct.u4Wp,
        rPidStruct.u4BufStart, rPidStruct.u4BufEnd + 1);

    return _FVRUpdateReadPointer(u1Pidx, u4Rp, u4FrameAddr);
}


//-----------------------------------------------------------------------------
/** _DMX_PSI_FlushBuffer
 *  Flush a section buffer
 */
//-----------------------------------------------------------------------------
BOOL _FVR_PSI_FlushBuffer(UINT8 u1Pidx)
{
    FVR_PID_STRUCT_T* prPidStruct;
    BOOL fgRet;

    LOG(6, "Flush PSI buffer: pidx %u\n", u1Pidx);

    // Check if it's a PSI PID
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);
    prPidStruct = _FVR_GetPidStruct(u1Pidx);
    if (prPidStruct->ePidType != DMX_PID_TYPE_PSI)
    {
        return FALSE;
    }

    fgRet = _FVR_FlushPidBuffer(u1Pidx);

    _FVR_Lock();
    prPidStruct->u4Rp = prPidStruct->u4BufStart;
    prPidStruct->u4Wp = prPidStruct->u4BufStart;
    _FVR_Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _FVR_PSI_GetBuffer
 */
//-----------------------------------------------------------------------------
BOOL _FVR_PSI_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
    UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest)
{
    FVR_PID_STRUCT_T rPidStruct;
    FVR_PID_STRUCT_T* prPidStruct;
    UINT32 u4Rp, u4Wp, u4BufStart, u4BufEnd;
    UINT32 u4BufSize, u4DataSize = 0, u4TotalSize, u4Size;
    BOOL fgRet = FALSE;

    if ((u1Pidx >= FVR_NUM_PID_INDEX) || (pu1Dest == NULL))
    {
        return FALSE;
    }

    _FVR_Lock();

    // Get PID structure
    prPidStruct = _FVR_GetPidStruct(u1Pidx);
    rPidStruct = *prPidStruct;

    _FVR_Unlock();

    if ((u1SerialNumber <= DMX_MAX_SERIAL_NUM) &&
        (rPidStruct.u1SerialNumber != u1SerialNumber))
    {
        // Unmatched serial numbers
        LOG(3, "Pidx %u: getbuffer, unmatched serial number %u [%u]\n", u1Pidx,
            u1SerialNumber, rPidStruct.u1SerialNumber);
        return FALSE;
    }

    // Check if it's a PSI PID
    if (rPidStruct.ePidType != DMX_PID_TYPE_PSI)
    {
        LOG(3, "Not a PSI PID!\n");
        goto _Exit;
    }

    // Compute avalible data size
    u4Rp = rPidStruct.u4Rp;
    u4Wp = rPidStruct.u4Wp;
    u4BufStart = rPidStruct.u4BufStart;
    u4BufEnd = rPidStruct.u4BufEnd;
    u4BufSize = rPidStruct.u4BufLen;
    u4DataSize = DATASIZE(u4FrameAddr, u4Wp, u4BufSize);
    u4TotalSize = DATASIZE(u4Rp, u4Wp, u4BufSize);

    // Check framer address
    if ((u4FrameAddr < u4BufStart) || (u4FrameAddr > u4BufEnd) ||
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
    if (u4Rp > u4BufEnd)
    {
        u4Rp -= u4BufSize;
    }

    // Copy data
    if ((u4Rp > u4Wp) && (u4CopySize > 0))
    {
        u4Size = MIN((u4BufEnd - u4Rp) + 1, u4CopySize);
        x_memcpy(pu1Dest, (void*)u4Rp, u4Size);
        u4Rp += u4Size;
        if (u4Rp > u4BufEnd)
        {
            u4Rp = u4BufStart;
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
        LOG(3, "Fail to get section data!\n");
        LOG(3, " Pidx: %u, frame: 0x%08x, data: %u, skip: %u, copy: %u\n",
            u1Pidx, u4FrameAddr, u4DataSize, u4SkipSize, u4CopySize);
    }

    return fgRet;
}


