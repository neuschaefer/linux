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
 * $Author: jiangbo.wang $
 * $Date: 2012/07/16 $
 * $RCSfile: dmx_loop.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_loopback.c
 *  dmx loopback driver
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "fvr.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_hal_arm.h"

LINT_EXT_HEADER_BEGIN
#include "x_ckgen.h"
LINT_EXT_HEADER_END


#ifdef CC_DMX_TS_LOOPBACK

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DMX_LP_THREAD_NAME              "DMXLP"
#define DMX_LP_THREAD_STACK_SIZE        1024
#define DMX_LP_THREAD_PRIORITY          60

#define DMX_LP_BUFFER_SIZE              0x80000     // 512KB

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T _hDmxLPThread = NULL_HANDLE;

static BOOL _fgDmxLPEnable = FALSE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static VOID _DmxLoopbackThread(VOID* pvArg)
{
    UINT32 u4RecWp1, u4RecWp2, u4PlayRp;

    UNUSED(pvArg);

    while(1)
    {
        if(_fgDmxLPEnable)
        {
            do
            {
                u4RecWp1 = FVR_GBL_ARY_W(0, 3);
                u4RecWp2 = FVR_GBL_ARY_W(0, 3);
            } while(u4RecWp1 != u4RecWp2);

            _DMX_Lock();
            u4PlayRp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP);
            
            // Update new pointer
            FVR_GBL_ARY_W(0, 4) = u4PlayRp;
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, u4RecWp1);
            _DMX_Unlock();
        }

        x_thread_delay(10);
    }
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_Loopback_Init
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Loopback_Init(void)
{
    static BOOL _fgInit = FALSE;

    if(!_fgInit)
    {
        if (x_thread_create(&_hDmxLPThread, DMX_LP_THREAD_NAME, DMX_LP_THREAD_STACK_SIZE,
                            DMX_LP_THREAD_PRIORITY, _DmxLoopbackThread, 0, NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX monitor thread!\n");
            return FALSE;
        }

        _fgInit = TRUE;
    }

    _fgDmxLPEnable = FALSE;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_Loopback_Set
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Loopback_Set(void)
{
    DMX_PVR_PLAY_T rPlay;
    FVR_GBL_T rGBL;
    FVR_TIMESTAMP_T rTimeStamp;
    UINT32 u4Cfg;

    if((_FVR_IsRecording(0)) || (_DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING))
    {
        LOG(5, "Please stop first\n");
        return FALSE;
    }

    //------------------------------------------------------
    // Set PVR play and record
    //------------------------------------------------------
    if(!_FVR_FreeBuffer(0))
    {
        return FALSE;
    }

    if(!_DMX_PVRPlay_FreeBuf())
    {
        return FALSE;
    }

    // Capture init
    x_memset((void*)&rGBL, 0, sizeof(rGBL));
    rGBL.fgAllocateBuffer = TRUE;
    rGBL.u2TargetPktCount = 0;
    rGBL.u4BufSize = DMX_LP_BUFFER_SIZE;
    rGBL.pfnNotifyFunc = NULL;
    rGBL.pvNotifyTag = NULL;
    rGBL.u4TimeToNotify = 1000;
    
    if(!_FVR_SetGBL(0, (UINT32)(FVR_GBL_FLAG_TARGET_PKT | FVR_GBL_FLAG_BUFFER | 
                    FVR_GBL_FLAG_TIME_INFO | FVR_GBL_FLAG_NOTIFY), &rGBL))
    {
        return FALSE;
    }

    if(!_FVR_GetGBL(0, FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return FALSE;
    }
    
    // Play init
    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = rGBL.u4BufStart;
    rPlay.u4BufSize = rGBL.u4BufSize;
    rPlay.u4ThresholdSize = 0x10000000;     // Not usable
    rPlay.pfnPVRNotify = NULL;

    if(!_DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_THRESHOLD |PVRPLAY_FLAGS_BUFFER | 
                                 PVRPLAY_FLAGS_CALLBACK), &rPlay))
    {
        return FALSE;
    }

    // Enabe Record timestamp
    rTimeStamp.fgEnable = TRUE;
    rTimeStamp.u2ClkDiv = 8;
    rTimeStamp.u4Mask = 0xFFFFFFFF;
    rTimeStamp.u4Counter= 0x0;
    _FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimeStamp);

    // Enable PLAY timestamp
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = FALSE;
    rPlay.u2TimeStampFreqDiv = 8;    
    if(!_DMX_PVRPlay_Set((UINT32)PVRPLAY_FLAGS_TIMESTAMP, &rPlay))
    {
        return FALSE;
    }

    //------------------------------------------------------
    // Set record bypass
    //------------------------------------------------------
    // Set DBM to normal mode and keep all TS packets
    u4Cfg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Cfg &= 0xc3ffffff;
    u4Cfg |= (1 << 26);
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Cfg);

    u4Cfg = (0x20 << 16);       // By pass Pid index = 32
    DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID, u4Cfg);

    //------------------------------------------------------
    // TS Out setting
    //------------------------------------------------------
    VERIFY(_DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_FRAMER1));
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_Loopback_Free
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Loopback_Free(void)
{

    if(!_FVR_FreeGBL(0))
    {
        return FALSE;
    }

    if(!_DMX_PVRPlay_Free())
    {
        return FALSE;
    }
        
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_Loopback_Enable
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Loopback_Enable(BOOL fgEnable)
{
    if(fgEnable)
    {        
        if(!_FVR_Start(0))
        {
            return FALSE;
        }

        x_thread_delay(20);

        if(!_DMX_PVRPlay_Start())
        {
            return FALSE;
        }
        
    }
    else
    {
        if(!_FVR_Stop(0))
        {
            return FALSE;
        }

        if(!_DMX_PVRPlay_Stop(TRUE))
        {
            return FALSE;
        }
    }

    _fgDmxLPEnable = fgEnable;
    
    return TRUE;
}

#endif   // CC_DMX_TS_LOOPBACK

