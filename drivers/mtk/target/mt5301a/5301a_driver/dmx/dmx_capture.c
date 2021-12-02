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
 * $RCSfile: dmx_capture.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_capture.c
 *  Demux driver - Bitstream capture
 */

//  This module exists only in non-mini driver
//#ifndef CC_MINI_DRIVER
#if 1


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_debug.h"
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
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_SetBypassMode
 *  Set demux (DBM) to bypass mode, mainly used for frontend capture function
 *
 *  @param  u1TsIndex       TS index (0 - 1)
 *  @param  u1PacketSize    TS packet size in byte
 *  @param  fgSteerToFTuP   Steer to FTuP or not
 *  @param  fgReset         Reset FTI or not
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetBypassMode(UINT8 u1TsIndex, UINT8 u1PacketSize,
    BOOL fgSteerToFTuP, BOOL fgReset)
{
    UINT32 u4Cfg, u4RegBypass, i;

    if (u1TsIndex > 3)
    {
        return FALSE;
    }

    if (fgReset)
    {
        // Reset Demux
        if (!DMX_Reset())
        {
            return FALSE;
        }
    }

    // Set packet size
    u4Cfg = DMXCMD_READ32(DMX_REG_CONFIG2);
    u4Cfg &= 0xffff00ff;
    u4Cfg |= (u1PacketSize << 8);
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Cfg);

    // reset DBM
    u4Cfg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Cfg |= 0x80000000;
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Cfg);
    u4Cfg &= 0x7fffffff;
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Cfg);

    // Framer control
    u4Cfg = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Cfg = (u4Cfg & 0xffffff00) | 0xff;    // Positive edge
//    u4Cfg = (u4Cfg & 0xffffff00) | 0x77;    // Negative edge
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Cfg);

    // Disable framer packet error handling
    DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE, 0);

    // Set DBM to normal mode and keep all TS packets
    u4Cfg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
//    u4Cfg = (u4Cfg & ~(1 << 28)) | (1 << 29);
    u4Cfg &= 0xc3ffffff;
    u4Cfg |= (1 << (u1TsIndex + 26));
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Cfg);

    u4RegBypass = 0;
    switch (u1TsIndex)
    {
    case 0:
        u4RegBypass = DMX_REG_DBM_BYPASS_PID;
        break;
    case 1:
        u4RegBypass = DMX_REG_DBM_BYPASS_PID_2;
        break;
    case 2:
        u4RegBypass = DMX_REG_DBM_BYPASS_PID_3;
        break;
    case 3:
        u4RegBypass = DMX_REG_DBM_BYPASS_PID_4;
        break;

    default:
        ASSERT(0);
        break;
    }

    if (fgSteerToFTuP)
    {
        // Set DBM Bypass PID register, steer to FTuP
        DMXCMD_WRITE32(u4RegBypass, 0x20);
    }
    else
    {
        // Set DBM Bypass PID register, steer to null
        DMXCMD_WRITE32(u4RegBypass, 0x0);
    }

    // Disable all PIDs
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        PID_INDEX_TABLE(i) &= 0x7fffffff;
    }

    // Enable CA input and output buffers
    // ...

    if (fgReset)
    {
        // Enable Demux
        _DMX_EnableFTI(TRUE);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetCapture
 *  Setup capture using CA output buffer
 *
 *  @param  fgEnable        Enable or disable capture function
 *  @param  prCapture       Capture settings
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetCapture(BOOL fgEnable, const DMX_CAPTURE_T* prCapture)
{
    DMX_STATES_T* prStates;
    UINT32 u4Reg;
    UINT8 i;

    prStates = _DMX_GetStates();

    if (fgEnable)
    {
        UINT32 u4BufStart;

        // Check parameters
        if ((prCapture == NULL) ||
            !_DMX_IsAligned(prCapture->u4BufSize, DMX_CA_BUFFER_ALIGNMENT))
        {
            return FALSE;
        }

        // Allocate CA output buffer
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(prCapture->u4BufSize,
            DMX_CA_BUFFER_ALIGNMENT);
        if (u4BufStart == 0)
        {
            // Out of memory
            return FALSE;
        }

        // Enable CA output buffer
        if (!_DMX_SetCaOutputBuffer(TRUE, TRUE, u4BufStart,
            u4BufStart + prCapture->u4BufSize, prCapture->u4Threshold))
        {
            VERIFY(BSP_FreeAlignedDmaMemory((void*)u4BufStart));
            return FALSE;
        }

        // Set capture handler
        prStates->rCaptureSettings.pfnHandler = prCapture->pfnCaptureHandler;
        prStates->rCaptureSettings.pvTag = prCapture->pvCaptureTag;
        prStates->rCaptureSettings.u4BufStart = u4BufStart;
        prStates->rCaptureSettings.u4BufEnd = u4BufStart +
            prCapture->u4BufSize;

        // Set DBM bypass mode
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Reg &= (~0x3C000000);
        u4Reg |= prCapture->u4BytePassDev;
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);

        // Set DBM Bypass PID register, steer to null
        DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID, 0);

        LOG(5, "Capture output buffer: 0x%08x - 0x%08x\n", u4BufStart,
            u4BufStart + prCapture->u4BufSize);
    }
    else
    {
        // Disable output buffer and interrupt
        if (!_DMX_SetCaOutputBuffer(FALSE, FALSE, 0, 0, 0))
        {
            return FALSE;
        }

        // Reset capture handler
        if (!BSP_FreeAlignedDmaMemory(
            (void*)prStates->rCaptureSettings.u4BufStart))
        {
            return FALSE;
        }
        prStates->rCaptureSettings.pfnHandler = NULL;
        prStates->rCaptureSettings.pvTag = NULL;
        prStates->rCaptureSettings.u4BufStart = 0;
        prStates->rCaptureSettings.u4BufEnd = 0;

        u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Reg &= (~0x3C000000);
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);

        // Reset steering to PVR flag for active PIDs
        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            DMX_PID_T rPid;

            VERIFY(_DMX_GetPid(i, DMX_PID_FLAG_STEER, &rPid));
            rPid.u1SteerMode &= ~DMX_STEER_TO_PVR;
            VERIFY(_DMX_SetPid(i, DMX_PID_FLAG_STEER, &rPid, FALSE));
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetCapture2
 *  Setup capture using CA output buffer, not allocate buffer
 *
 *  @param  fgEnable        Enable or disable capture function
 *  @param  prCapture       Capture settings
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetCapture2(BOOL fgEnable, const DMX_CAPTURE2_T* prCapture)
{
    DMX_STATES_T* prStates;
    UINT32 u4Reg, u4BufStart, u4BufEnd;

    prStates = _DMX_GetStates();

    if (fgEnable)
    {
        // Check parameters
        if ((prCapture == NULL) ||
            !_DMX_IsAligned(prCapture->u4BufSize, DMX_CA_BUFFER_ALIGNMENT))
        {
            return FALSE;
        }

        u4BufStart = prCapture->u4BufStart;
        u4BufEnd = prCapture->u4BufStart + prCapture->u4BufSize;

        // Set capture handler
        prStates->rCaptureSettings.pfnHandler = prCapture->pfnCaptureHandler;
        prStates->rCaptureSettings.pvTag = prCapture->pvCaptureTag;
        prStates->rCaptureSettings.u4BufStart = u4BufStart;
        prStates->rCaptureSettings.u4BufEnd = u4BufEnd;

        // Set DBM bypass mode
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Reg &= (~0x3C000000);
        u4Reg |= prCapture->u4BytePassDev;
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);

        // Set DBM Bypass PID register, steer to null
        DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID, 0);
        DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID_2, 0);

        // Enable CA output buffer
        if (!_DMX_SetCaOutputBuffer(TRUE, TRUE, u4BufStart, u4BufEnd,
                                    prCapture->u4Threshold))
        {
            return FALSE;
        }

        LOG(5, "Capture output buffer: 0x%08x - 0x%08x\n", u4BufStart, u4BufEnd);
    }
    else
    {
        // Disable output buffer and interrupt
        if (!_DMX_SetCaOutputBuffer(FALSE, FALSE, 0, 0, 0))
        {
            return FALSE;
        }

        //prStates->rCaptureSettings.pfnHandler = NULL;
        prStates->rCaptureSettings.pvTag = NULL;
        prStates->rCaptureSettings.u4BufStart = 0;
        prStates->rCaptureSettings.u4BufEnd = 0;

        u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Reg &= (~0x3C000000);
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_UpdateCaptureReadPointer
 *  Update read pointer of capture buffer
 *
 *  @param  u4NewRp         The new read pointer
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_UpdateCaptureReadPointer(UINT32 u4NewRp)
{
    UINT32 u4BufStart, u4BufEnd, u4Wp, u4Rp, u4BufSize;
    UINT32 u4DataSize, u4NewDataSize;

    // Check if RP is in buffer range
	u4BufStart = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_START);
	u4BufEnd = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_END) + 1;

	u4NewRp = PHYSICAL(u4NewRp);
	if ((u4NewRp < u4BufStart) || (u4NewRp >= u4BufEnd))
	{
	    return FALSE;
	}

	// Check if the new RP is moved to correct direction
	u4Wp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_WP);
    u4Rp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_RP);

    u4BufSize = u4BufEnd - u4BufStart;
    u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);
    u4NewDataSize = DATASIZE(u4NewRp, u4Wp, u4BufSize);
    if (u4NewDataSize > u4DataSize)
    {
        return FALSE;
    }

    // Update RP
	DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_RP, u4NewRp);

	return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetCaptureInfo
 *  Get capture info
 *
 *  @param  prInfo [output] The capture info
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetCaptureInfo(DMX_CAPTURE_INFO_T* prInfo)
{
    UINT32 u4BufStart, u4BufEnd, u4Wp, u4Rp, u4BufSize, u4DataSize;
    DMX_STATES_T* prStates;

    if (prInfo == NULL)
    {
        return FALSE;
    }

    prStates = _DMX_GetStates();
    if (prStates->rCaptureSettings.pfnHandler == NULL)
    {
        // Not in capture mode
        return FALSE;
    }

	u4BufStart = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_START);
	u4BufEnd = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_END) + 1;
	u4Wp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_WP);
    u4Rp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_RP);

    u4BufSize = u4BufEnd - u4BufStart;
    u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);

    prInfo->u4BufStart = u4BufStart;
    prInfo->u4BufEnd = u4BufEnd;
    prInfo->u4Rp = u4Rp;
    prInfo->u4Wp = u4Wp;
    prInfo->u4DataSize = u4DataSize;

    return TRUE;
}


#endif  // #ifndef CC_MINI_DRIVER


