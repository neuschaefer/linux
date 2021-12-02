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
 * $RCSfile: dmx_diag_jchip.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_diag_jchip.c
 *  Demux diagnostic routines for J-chip
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_debug.h"
#include "x_lint.h"
#include "x_rand.h"

//////////////////////////////////////////////////////////////////////////// LINT_EXT_HEADER_BEGIN

#include "x_printf.h"
#include "x_os.h"

//////////////////////////////////////////////////////////////////////////// LINT_EXT_HEADER_END


#ifndef __MODEL_slt__


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define JCHIP_ERR_NO_IMPLEMENTATION         -33


//-----------------------------------------------------------------------------
/** _DMX_JChipDiag
 *  Empty implementation!
 *
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. The return value is error code
 */
//-----------------------------------------------------------------------------
INT32 _DMX_JChipDiag(UINT32 u1TsIndex)
{
    printf("Turn on __MODEL_slt__ please!!!\n");
    return JCHIP_ERR_NO_IMPLEMENTATION;
}


#else


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/* See the comment in _au1Version below. */
#define VCT_CMP_PACKETS                     2

#define JCHIP_ERR_TEST_OK                   0
#define JCHIP_ERR_INIT                      -1
#define JCHIP_ERR_DISABLE_FILTER            -2
#define JCHIP_ERR_FREE_PID                  -3
#define JCHIP_ERR_SELECT_INPUT_TYPE         -4
#define JCHIP_ERR_SET_PID                   -5
#define JCHIP_ERR_SET_FILTER                -6
#define JCHIP_ERR_FREE_0X1FFB_PID           -7
#define JCHIP_ERR_DISABLE_0X1FFB_FILTER     -8
#define JCHIP_ERR_MISSING_VERSION           -66
#define JCHIP_ERR_NOT_ZERO_OUT              -67
#define JCHIP_ERR_TIMEOUT                   -99


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgTestFinished = TRUE;
static BOOL _fgTryToLockVersion = FALSE;
static INT32 _i4TestResult = JCHIP_ERR_INIT;

/*****************************************************************************
* Each TS packet with PID value 0x1FFB contains three VCT.  The one with
* version number 1 has CRC error.
*****************************************************************************/
static UINT8 _au1Version[VCT_CMP_PACKETS] = {0, 2};

/* Make use of static array simply because of the easier implementation. */
static UINT8 _au1Sec[4096];


//-----------------------------------------------------------------------------
/** _DMX_JChipDiag
 *  Verify the J-chip functionality.
 *
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. The return value is error code
 */
//-----------------------------------------------------------------------------
INT32 _DMX_JChipDiag(UINT32 u1TsIndex)
{
    UINT8 u1Pidx, u1FilterIndex, u1Bank;
    HAL_RAW_TIME_T rRawTime1, rRawTime2, rRawTimeDiff;
    DMX_PID_T rPid;
    DMX_FILTER_T rFilter;
    BOOL fgTestFinished = FALSE;
    INT32 i4TestResult;

    // Clear local variable/structure.
    x_memset(&rFilter, 0, sizeof(rFilter));

    // Disable all section filters.
    rFilter.fgEnable = FALSE;
    for (u1FilterIndex = 0; u1FilterIndex < DMX_NUM_FILTER_INDEX; u1FilterIndex++)
    {
        if (!_DMX_SetFilter(u1FilterIndex, 0, DMX_FILTER_FLAG_VALID, &rFilter))
        {
            LOG(5, "Failed to disable filter (index %u)!\n", u1FilterIndex);
            return JCHIP_ERR_DISABLE_FILTER;
        }
    }

    // Free all PIDs.
    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
    {
        if (!_DMX_FreePid(u1Pidx))
        {
            LOG(5, "Failed to free PID (index %u)!\n", u1Pidx);
            return JCHIP_ERR_FREE_PID;
        }
    }

    // uCode 4.27+ supports all three J-chip usage scenarios.
    // It is not necessary to use a special version of uCode any more.
#if 0
    // Load the appropriate uCode.
    if (!_DMX_SelectInputType(DMX_IN_JCHIP))
    {
        LOG(5, "Failed to select input type!\n");
        return JCHIP_ERR_SELECT_INPUT_TYPE;
    }
#endif

    // Set the seed of the random variable.
    HAL_GetRawTime(&rRawTime1);
    srand(rRawTime1.u4Cycles);

    // Clear local variables/structures.
    x_memset(&rPid, 0, sizeof(rPid));
    x_memset(&rFilter, 0, sizeof(rFilter));

    // Set the global variable to FALSE.
    // It could be set to TRUE in _DMX_CheckVCT().
    _DMX_Lock();
    _fgTryToLockVersion = TRUE;
    _fgTestFinished = FALSE;
    _DMX_Unlock();

    // Generate random number for PID index and Filter index
    u1Pidx = random(DMX_NUM_PID_INDEX);
    u1FilterIndex = random(DMX_NUM_FILTER_INDEX);

    rPid.u1TsIndex = u1TsIndex;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.fgEnable = TRUE;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.ePidType = DMX_PID_TYPE_PSI;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = 0x8000;  // 32KB (more than enough)
    rPid.u2Pid = (UINT16)0x1FFB;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;
    if (!_DMX_SetPid(u1Pidx, DMX_PID_FLAG_ALL, &rPid, FALSE))
    {
        LOG(5, "Failed to set PID (index %u)!\n", u1Pidx);
        return JCHIP_ERR_SET_PID;
    }

    // Set Filter
    rFilter.eMode = DMX_FILTER_MODE_SHORT;
    rFilter.fgEnable = TRUE;
    rFilter.u1Pidx = u1Pidx;
    rFilter.u1Offset = (UINT8)3;
    rFilter.eMode = DMX_FILTER_MODE_SHORT;
    rFilter.fgCheckCrc = TRUE;
    rFilter.au4Data[0] = (UINT32)0xC8 << 24;
    rFilter.au4Data[1] = 0;
    rFilter.au4Mask[0] = 0xFFFFFFFF;
    rFilter.au4Mask[1] = 0xFFFFFFFF;
    u1Bank = 0;
    if (!DMX_SetFilter(u1FilterIndex, u1Bank, DMX_FILTER_FLAG_ALL, &rFilter))
    {
        LOG(5, "Failed to set filter (index %u)!\n", u1FilterIndex);
        return JCHIP_ERR_SET_FILTER;
    }

    do {
        HAL_TIME_T rTime;
        HAL_GetRawTime(&rRawTime2);
        HAL_GetDeltaRawTime(&rRawTimeDiff, &rRawTime1, &rRawTime2);
        HAL_RawToTime(&rRawTimeDiff, &rTime);
        if (rTime.u4Seconds < 3)  // Wait for 3 seconds at most.
        {
            _DMX_Lock();
            fgTestFinished = _fgTestFinished;
            _DMX_Unlock();

            if (fgTestFinished)
            {
                break;
            }
            else
            {
                x_thread_delay(100);
            }
        }
        else
        {
            break;
        }
    } while (1);

    // Free PID
    if (!_DMX_FreePid(u1Pidx))
    {
        LOG(5, "Failed to free PID (index %u)!\n", u1Pidx);
        return JCHIP_ERR_FREE_0X1FFB_PID;
    }

    // Disable all section filters.
    rFilter.fgEnable = FALSE;
    if (!_DMX_SetFilter(u1FilterIndex, 0, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        LOG(5, "Failed to disable filter (index %u)!\n", u1FilterIndex);
        return JCHIP_ERR_DISABLE_0X1FFB_FILTER;
    }

    if (fgTestFinished)
    {
        _DMX_Lock();
        i4TestResult = _i4TestResult;
        _DMX_Unlock();
    }
    else
    {
        i4TestResult = JCHIP_ERR_TIMEOUT;
    }

    return i4TestResult;
}


//-----------------------------------------------------------------------------
/** _DMX_CheckVCT
 *  NOTE:  This function is called by _DmxPSIParser().
 *
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. The return value is error code
 */
//-----------------------------------------------------------------------------
void _DMX_CheckVCT(UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4SecRp,
                    UINT32 u4SecLen)
{
    static UINT8 u1VersionIndex = 0;
    BOOL fgTestFinished, fgTryToLockVersion;

    if ((u4SecRp < u4BufStart) || (u4BufEnd <= u4SecRp))
    {
        LOG(5, "Incorrect pointer range!\n");
        return;
    }

    if ((u4SecLen == 0) || (4096 < u4SecLen))
    {
        LOG(5, "Incorrect section length!\n");
        return;
    }

    if (_DMX_CopyRingBuffer((UINT32)_au1Sec, (UINT32)_au1Sec, (UINT32)_au1Sec + u4SecLen,
        u4SecRp, u4BufStart, u4BufEnd, u4SecLen) != (UINT32)_au1Sec)
    {
        LOG(5, "Fail to copy ring buffer\n");
        return;
    }

    //_DmxDumpBuffer(_au1Sec, u4SecLen, 16);

    _DMX_Lock();
    fgTestFinished = _fgTestFinished;
    fgTryToLockVersion = _fgTryToLockVersion;
    _DMX_Unlock();

    if (fgTestFinished)
    {
        return;
    }

    if (u4SecLen >= DMX_AW_VCT_HEADER)
    {
        UINT8 u1Version, u1NoChannels, u1Channel;
        UINT16 u2Counter = 0;

        u1Version = (_au1Sec[5] >> 1) & 0x1F;

        if (fgTryToLockVersion)
        {
            u1VersionIndex = 0;
            if (u1Version == _au1Version[u1VersionIndex])
            {
                _DMX_Lock();
                _fgTryToLockVersion = FALSE;
                _DMX_Unlock();
            }
            else
            {
                return;
            }
        }
        else
        {
            if (u1Version != _au1Version[u1VersionIndex])
            {
                _DMX_Lock();
                _fgTestFinished = TRUE;
                _i4TestResult = JCHIP_ERR_MISSING_VERSION;
                _DMX_Unlock();
                return;
            }
        }

        u1NoChannels = _au1Sec[9];
        LOG(5, "VCT version: %u\n", u1Version);
        LOG(5, "# of channels: %u\n", u1NoChannels);

        u2Counter = DMX_AW_VCT_HEADER;
        for (u1Channel = 0; u1Channel < u1NoChannels; u1Channel++)
        {
            UINT16 u2Offset, u2DescLen;

            u2Counter += DMX_AW_VCT_SUBHEADER;
            if (u4SecLen < u2Counter)
            {
                LOG(5, "Insufficient VCT length\n", u4SecLen);
            }

            u2DescLen = ((_au1Sec[u2Counter - 2] & 0x3) << 8) | _au1Sec[u2Counter - 1];
            LOG(5, "VCT descriptors_len: %u\n", u2DescLen);

            u2Counter += u2DescLen;
            if (u4SecLen < u2Counter)
            {
                LOG(5, "Insufficient VCT length %u\n", u4SecLen);
            }

            for (u2Offset = 0; u2Offset < u2DescLen; u2Offset++)
            {
                if (_au1Sec[(u2Counter - u2DescLen) + u2Offset] != 0)
                {
                    _DMX_Lock();
                    _fgTestFinished = TRUE;
                    _i4TestResult = JCHIP_ERR_NOT_ZERO_OUT;
                    _DMX_Unlock();
                    LOG(1, "Descriptors are not zeroed out! (offset %u)\n", u2Offset);
                    return;
                }
            }
        }

        u1VersionIndex++;
        if (u1VersionIndex >= VCT_CMP_PACKETS)
        {
            u1VersionIndex = 0;
            _DMX_Lock();
            _fgTestFinished = TRUE;
            _i4TestResult = JCHIP_ERR_TEST_OK;
            _DMX_Unlock();
        }
    }

    return;
}

#endif  // __MODEL_slt__

