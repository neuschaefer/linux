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


//-----------------------------------------------------------------------------
// --- Beginning of "check for AW Eraser" ---


/** @file dmx_aw_eraser.c
 *  Demux diagnostic routines for J-chip
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_debug.h"
#include "x_lint.h"
#include "x_rand.h"

LINT_EXT_HEADER_BEGIN

#include "x_os.h"

LINT_EXT_HEADER_END

// This global variable is only referenced in "dmx_psi_handler.c".
BOOL _fgAwEraserCheckBegin = FALSE;


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
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgAwEraserCheckDone = FALSE;
static INT32 _i4AwEraserCheckResult = 0;
static UINT8 *_pu1Sec = NULL;



//-----------------------------------------------------------------------------
/** _DMX_AwEraserCheck
 *  This function is called in _DmxPSIParser().
 *
 *  @retval 0               Succeed
 *  @retval otherwise       Fail.  The return value is the error code.
 */
//-----------------------------------------------------------------------------
void _DMX_AwEraserCheck(UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4SecRp,
                    UINT32 u4SecLen)
{
    UINT8 *pu1Sec;

    if ((u4SecRp < u4BufStart) || (u4BufEnd <= u4SecRp))
    {
        _DMX_Lock();
        _i4AwEraserCheckResult = DMX_AW_ERR_ADDRESS_RANGE;
        _fgAwEraserCheckDone = TRUE;
        _DMX_Unlock();
        return;
    }

    if ((u4SecLen == 0) || (DMX_AW_MAX_PSI_LEN < u4SecLen))
    {
        _DMX_Lock();
        _i4AwEraserCheckResult = DMX_AW_ERR_SECTION_LEN;
        _fgAwEraserCheckDone = TRUE;
        _DMX_Unlock();
        return;
    }

    _DMX_Lock();
    pu1Sec = _pu1Sec;
    _DMX_Unlock();

    if (pu1Sec == NULL)
    {
        _DMX_Lock();
        _i4AwEraserCheckResult = DMX_AW_ERR_NO_MEM;
        _fgAwEraserCheckDone = TRUE;
        _DMX_Unlock();
        return;
    }

    if (_DMX_CopyRingBuffer((UINT32)pu1Sec, (UINT32)pu1Sec, (UINT32)pu1Sec + u4SecLen,
        u4SecRp, u4BufStart, u4BufEnd, u4SecLen) != (UINT32)pu1Sec)
    {
        _DMX_Lock();
        _i4AwEraserCheckResult = DMX_AW_ERR_COPY_BUFFER;
        _fgAwEraserCheckDone = TRUE;
        _DMX_Unlock();
        return;
    }

    if (u4SecLen >= DMX_AW_VCT_HEADER)
    {
        UINT8 u1NoChannels, u1Channel;
        UINT16 u2OffsetCounter = 0;

        u1NoChannels = pu1Sec[9];
        LOG(5, "# of channels: %u\n", u1NoChannels);

        u2OffsetCounter = DMX_AW_VCT_HEADER;
        for (u1Channel = 0; u1Channel < u1NoChannels; u1Channel++)
        {
            UINT16 u2Offset, u2DescLen;

            u2OffsetCounter += DMX_AW_VCT_SUBHEADER;
            if (u4SecLen < u2OffsetCounter)
            {
                _DMX_Lock();
                _i4AwEraserCheckResult = DMX_AW_ERR_INSUFFICIENT_LEN;
                _fgAwEraserCheckDone = TRUE;
                _DMX_Unlock();
                LOG(5, "Insufficient VCT sub-header!\n");
                return;
            }

            u2DescLen = ((pu1Sec[u2OffsetCounter - 2] & 0x3) << 8) |
                          pu1Sec[u2OffsetCounter - 1];
            LOG(7, "VCT descriptors_len: %u\n", u2DescLen);

            u2OffsetCounter += u2DescLen;
            if (u4SecLen < u2OffsetCounter)
            {
                _DMX_Lock();
                _i4AwEraserCheckResult = DMX_AW_ERR_INSUFFICIENT_LEN;
                _fgAwEraserCheckDone = TRUE;
                _DMX_Unlock();
                LOG(5, "Insufficient VCT length!\n");
                return;
            }

            for (u2Offset = 0; u2Offset < u2DescLen; u2Offset++)
            {
                if (pu1Sec[(u2OffsetCounter - u2DescLen) + u2Offset] != 0)
                {
                    _DMX_Lock();
                    _i4AwEraserCheckResult = DMX_AW_ERR_NOT_ZEROED_OUT;
                    _fgAwEraserCheckDone = TRUE;
                    _DMX_Unlock();
                    return;
                }
            }
        }
    }

    _DMX_Lock();
    _i4AwEraserCheckResult = DMX_AW_ERR_OK;
    _fgAwEraserCheckDone = TRUE;
    _DMX_Unlock();
    return;
}



//-----------------------------------------------------------------------------
/** _DMX_AwEraserCheck_Init
 *  
 */
//-----------------------------------------------------------------------------
void _DMX_AwEraserCheck_Init(void)
{
    UINT8 *pu1Sec;

    _DMX_Lock();
    pu1Sec = _pu1Sec;
    _DMX_Unlock();

    if (pu1Sec == NULL)
    {
        pu1Sec = (UINT8*)x_mem_alloc(DMX_AW_MAX_PSI_LEN);
        if (pu1Sec == NULL)
        {
            _DMX_Lock();
            _fgAwEraserCheckBegin = FALSE;
            _fgAwEraserCheckDone = TRUE;
            _i4AwEraserCheckResult = DMX_AW_ERR_NO_MEM;
            _DMX_Unlock();
            return;
        }
    }

    _DMX_Lock();
    _pu1Sec = pu1Sec;
    _fgAwEraserCheckBegin = FALSE;
    _fgAwEraserCheckDone = FALSE;  // Make incorrect Get operation return TIMEOUT.
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_AwEraserCheck_Free
 *  
 */
//-----------------------------------------------------------------------------
void _DMX_AwEraserCheck_Free(void)
{
    UINT8 *pu1Sec;

    _DMX_Lock();
    pu1Sec = _pu1Sec;
    _DMX_Unlock();

    if (pu1Sec != NULL)
    {
        x_mem_free((void*)pu1Sec);
        pu1Sec = NULL;
    }

    _DMX_Lock();
    _pu1Sec = pu1Sec;
    _fgAwEraserCheckBegin = FALSE;
    _fgAwEraserCheckDone = FALSE;  // Make incorrect Get operation return TIMEOUT.
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_AwEraserCheck_Set
 *  
 */
//-----------------------------------------------------------------------------
void _DMX_AwEraserCheck_Set(BOOL fgEnable)
{
    BOOL fgAwEraserCheckBegin;

    if (fgEnable)
    {
        fgAwEraserCheckBegin = TRUE;
    }
    else
    {
        fgAwEraserCheckBegin = FALSE;
    }

    _DMX_Lock();
    _fgAwEraserCheckBegin = fgAwEraserCheckBegin;
    _fgAwEraserCheckDone = FALSE;  // Make incorrect Get operation return TIMEOUT.
    _DMX_Unlock();

    return;
}


//-----------------------------------------------------------------------------
/** _DMX_AwEraserCheck_Get
 *  
 */
//-----------------------------------------------------------------------------
INT32 _DMX_AwEraserCheck_Get(UINT32 u4Timeout)
{
    BOOL fgAwEraserCheckDone;
    INT32 i4Result;

    do
    {
        _DMX_Lock();
        fgAwEraserCheckDone = _fgAwEraserCheckDone;
        i4Result = _i4AwEraserCheckResult;
        _DMX_Unlock();

        if (fgAwEraserCheckDone)
        {
            break;
        }
        else
        {
            x_thread_delay(1);
            u4Timeout--;
        }
    } while (u4Timeout > 0);

    if ((u4Timeout == 0) && (!fgAwEraserCheckDone))
    {
        i4Result = DMX_AW_ERR_TIMEOUT;
    }

    return i4Result;
}

// --- End of "check for AW Eraser" ---
//-----------------------------------------------------------------------------

