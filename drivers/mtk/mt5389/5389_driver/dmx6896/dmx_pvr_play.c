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
 * $RCSfile: dmx_pvr_play.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_pvr_play.c
 *  dmx pvr driver - DMX PVR play function
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_hal_arm.h"

LINT_EXT_HEADER_BEGIN
#include "x_ckgen.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
//#define DMX_PVRPLAY_MEASURE_TIME

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define PVR_BUF_FULL_GAP                    (192 * 4)//(188 * 4)
#define DMX_PVR_BUF_ALIGNMENT               16
#define DMX_PVR_POINTER_ALIGNMENT           4

#define DMX_PVRP_STOP_PKT_THRESHOLD         (20)

#define DMX_PVRP_DBM_PKT_SIZE               (192)
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    BOOL fgValid;
    UINT16 u2PidNum;
} DMX_PVR_INFO_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DMX_PVR_PLAY_T _rPVRPlay;
static HANDLE_T _hDMXPVRPlaySema = NULL_HANDLE;

static BOOL _fgTSOut = FALSE;

static DMX_PVRPLAY_COUNTER_T _rPVRPlayCounter;

static DMX_PVRPLAY_PORT_T _ePVRPort;

static DMX_TSFMT_T _eDmxPVRTSFmt;

static BOOL _fgDmxTimeshiftOpen = FALSE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DmxPVRPlay_UpdateAddr
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxPVRPlay_UpdateAddr(UINT32 u4Addr, UINT32 u4Size, UINT32 u4BufStart,
                                     UINT32 u4BufEnd)
{
    UINT32 u4NewAddr;

    u4NewAddr = u4Addr + u4Size;
    if(u4NewAddr >= u4BufEnd)
    {
        u4NewAddr -= (u4BufEnd - u4BufStart);
    }

    ASSERT((u4BufStart<=u4NewAddr)&&(u4NewAddr<u4BufEnd));

    return u4NewAddr;
}


#if 0
//-----------------------------------------------------------------------------
/** _DmxPVRPlay_DoStop
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPVRPlay_DoStop(DMX_PVRPLAY_PTR_T *prPtr)
{
    UINT32 u4Wp, u4Rp, u4BufStart, u4BufEnd;
    UINT32 u4NewRp, u4Align;
    UINT32 u4Reg;
    UINT32 i;
    UINT32 u4Val;

    if(_rPVRPlay.eState == DMX_PVRPLAY_STATE_STOP)
    {
        LOG(3, "PVR play already stop!\n");
        return TRUE;
    }

    _DMX_Lock();
    u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP);
    u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP);
    u4BufStart = _rPVRPlay.u4BufStart;
    u4BufEnd = _rPVRPlay.u4BufEnd;
    _DMX_Unlock();

    u4Wp = VIRTUAL(u4Wp);
    u4Rp = VIRTUAL(u4Rp);

    if(_rPVRPlay.fgIgnoreTimeStamp)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0); // stop at packet boundary
        if(prPtr != NULL)
        {
            _DMX_Lock();
            prPtr->u4Rp = u4Rp;
            prPtr->u4Wp = u4Wp;
            _DMX_Unlock();
        }
        _rPVRPlay.eState = DMX_PVRPLAY_STATE_STOP;
        return TRUE;
    }

    ASSERT(_rPVRPlay.fgContainTimeStamp);

    // Disable PVR play
    //DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0); // stop at packet boundary
    u4Reg = DMXCMD_READ32(DMX_REG_PVR_CONTROL) | 0x400;
    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, u4Reg);
#if 1
    for(i=0; i<50; i++)
    {
        x_thread_delay(1);
        if((DMXCMD_READ32(224) & 0x1) == 0)
        {
            break;
        }
    }

    if(i == 50)
    {
        LOG(3, "pvr stop timeout\n");
    }

    // Adjust RP to alignment
    u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP);

    // Disable PVR
    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0);

    u4Align = (u4Rp - u4BufStart) % 192;
    //LOG(3, "align: %d\n", u4Align);
    u4NewRp = u4Rp - u4Align;

    if(u4NewRp < u4BufStart)
    {
        u4NewRp += (u4BufEnd - u4BufStart);
    }
    //DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, u4NewRp);
    if(prPtr != NULL)
    {
        _DMX_Lock();
        prPtr->u4Rp = u4NewRp;
        prPtr->u4Wp = u4Wp;
        _DMX_Unlock();
    }

    _rPVRPlay.eState = DMX_PVRPLAY_STATE_STOP;
#else
    _DMX_Lock();
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    // Set DBM output spacing
    u4Reg = (u4Reg & 0xff00ffff) | (1 << 16);
    // Set DBM max playback TS packets
    u4Reg = (u4Reg & 0xffff00ff) | (2 << 8);
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
    _DMX_Unlock();

    HAL_Delay_us(1);

    u4Val = _fgTSOut ? (1 << 16) : (1 << 17);
    u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & (~u4Val);
    DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);

    i = 0;
    u4Val = _fgTSOut ? 0x80000000 : 0x40000000;
    while(1)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
        if((u4Reg & u4Val) == 0)
        {
            break;
        }
        if(i++ > 50)
        {
            LOG(0, "%s, pkt boundary timeout\n", __FUNCTION__);
            break;
        }
        //LOG(3, "delay....\n");
        x_thread_delay(1);
    }

    _DMX_Lock();
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    // Set DBM output spacing
    u4Reg = (u4Reg & 0xff00ffff) | (8 << 16);
    ////u4Reg = (u4Reg & 0xff00ffff) | (1 << 16);
    // Set DBM max playback TS packets
    u4Reg = (u4Reg & 0xffff00ff) | (1 << 8);
    ////u4Reg = (u4Reg & 0xffff00ff) | (2 << 8);
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
    _DMX_Unlock();

    // Disable PVR
    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0);

    // Adjust RP to alignment
    u4Align = (u4Rp - u4BufStart) % 192;
    u4NewRp = u4Rp - (192 + u4Align);

    if(u4NewRp < u4BufStart)
    {
        u4NewRp += (u4BufEnd - u4BufStart);
    }
    //DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, u4NewRp);
    if(prPtr != NULL)
    {
        _DMX_Lock();
        prPtr->u4Rp = u4NewRp;
        prPtr->u4Wp = u4Wp;
        _DMX_Unlock();
    }

    _rPVRPlay.eState = DMX_PVRPLAY_STATE_STOP;

#endif
    return TRUE;
}
#else
//-----------------------------------------------------------------------------
/** _DmxPVRPlay_DoStop
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPVRPlay_DoStop(DMX_PVRPLAY_PTR_T *prPtr)
{
    UINT32 u4Wp, u4Rp, u4BufStart, u4BufSize, u4BufEnd, u4DataSize;
    UINT32 u4NewWp, u4NewRp, u4Align;
    UINT32 i, u4Reg;

    u4Wp = 0;
    u4NewRp = 0;

    if(_rPVRPlay.eState != DMX_PVRPLAY_STATE_PLAYING)
    {
        LOG(3, "PVR play already stop!\n");
        return TRUE;
    }

    if(_rPVRPlay.fgIgnoreTimeStamp)
    {
        // Disable PVR play
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0);
    }
    else
    {
        _DMX_Lock();
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        // Set DBM output spacing
        u4Reg = (u4Reg & 0xff00ffff) | (1 << 16);
        // Set DBM max playback TS packets
        u4Reg = (u4Reg & 0xffff00ff) | (3 << 8);
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
        _DMX_Unlock();

        //--------------------------------------------------
        // Special case that this operation must be atomic
        //--------------------------------------------------
        _DMX_Lock();
        u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP);
        u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP);
        u4BufStart = _rPVRPlay.u4BufStart;
        u4BufEnd = _rPVRPlay.u4BufEnd;
        u4BufSize = _rPVRPlay.u4BufSize;

        u4Wp = VIRTUAL(u4Wp);
        u4Rp = VIRTUAL(u4Rp);

        if(u4Wp != u4Rp)
        {
            u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);
            u4NewWp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP);
            u4NewWp = VIRTUAL(u4NewWp);
            if(u4DataSize > (DMX_PVRP_STOP_PKT_THRESHOLD * 192))
            {
                u4Align = (u4Rp - u4BufStart) % 192;
                // Make sure u4NewRp is 192 bytes alignment
                u4NewRp = _DmxPVRPlay_UpdateAddr(u4Rp, (192 - u4Align),
                                                 u4BufStart, u4BufEnd);
                u4NewWp = _DmxPVRPlay_UpdateAddr(u4NewRp,
                                (DMX_PVRP_STOP_PKT_THRESHOLD * 192),
                                u4BufStart, u4BufEnd);
                DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, PHYSICAL(u4NewWp));
            }
            _DMX_Unlock();

            i = 0;
            while(1)
            {
                u4Rp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP));
                if(u4Rp == u4NewWp)
                {
                    break;
                }
                x_thread_delay(1);
                if(i++ > 50)
                {
                    LOG(1,"%s timeout\n", __FUNCTION__);
                    break;
                }
            }
        }
        else
        {
            _DMX_Unlock();
        }

        // Disable PVR play
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0);

        _DMX_Lock();
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        // Set DBM output spacing
        u4Reg = (u4Reg & 0xff00ffff) | (8 << 16);
        // Set DBM max playback TS packets
        u4Reg = (u4Reg & 0xffff00ff) | (1 << 8);
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
        _DMX_Unlock();

        LOG(6, "dostop, u4Rp:0x%x, u4NewRp:0x%x, u4Wp:0x%x\n", u4Rp, u4NewRp, u4Wp);

    }

    //u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP);
    u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP);
    if(prPtr != NULL)
    {
        _DMX_Lock();
        prPtr->u4Rp = VIRTUAL(u4Rp);
        prPtr->u4Wp = VIRTUAL(u4Wp);
        _DMX_Unlock();
    }

    _rPVRPlay.eState = DMX_PVRPLAY_STATE_STOP;

    return TRUE;
}
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Free
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Free(void)
{
    if(!_DMX_PVRPlay_Stop(TRUE))
    {
        return FALSE;
    }

    if(!_DMX_PVRPlay_FreeBuf())
    {
        return FALSE;
    }

    _DMX_Lock();

    x_memset((void*)&_rPVRPlay, 0x0, sizeof(DMX_PVR_PLAY_T));
    _rPVRPlay.fgContainTimeStamp = FALSE;
    _rPVRPlay.fgIgnoreTimeStamp = TRUE;
    _rPVRPlay.eState = DMX_PVRPLAY_STATE_STOP;
    _rPVRPlay.eMode = DMX_PVR_PLAY_STREAM;

    _fgTSOut = FALSE;

    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0x0);

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Init
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Init()
{
    static BOOL _fgInit = FALSE;

    // When error reset, do not init following variables
    if(!_DMX_GetIsPanicReset())
    {
        _DMX_Lock();
        _fgDmxTimeshiftOpen = FALSE;
        x_memset((void*)&_rPVRPlay, 0x0, sizeof(DMX_PVR_PLAY_T));
        _rPVRPlay.fgAllocBuf = FALSE;
        _rPVRPlay.fgContainTimeStamp = FALSE;
        _rPVRPlay.fgIgnoreTimeStamp = TRUE;
        _rPVRPlay.eState = DMX_PVRPLAY_STATE_STOP;
        _rPVRPlay.eMode = DMX_PVR_PLAY_STREAM;
        x_memset((void*)&_rPVRPlayCounter, 0, sizeof(DMX_PVRPLAY_COUNTER_T));
        _DMX_Unlock();
    }

    _DMX_Lock();
    _fgTSOut = FALSE;
    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0x0);
    _DMX_Unlock();

    _DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_DBM);
    if(!_DMX_PVRPlay_SetMoveMode(DMX_TSFMT_188, TRUE))
    {
        return FALSE;
    }

    if(!_fgInit)
    {
        VERIFY(x_sema_create(&_hDMXPVRPlaySema, X_SEMA_TYPE_BINARY,
                              X_SEMA_STATE_LOCK) == OSR_OK);
        _fgInit = TRUE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Start
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Start(void)
{
    UINT32 u4RegControl;

    if(_rPVRPlay.eState != DMX_PVRPLAY_STATE_STOP)
    {
        LOG(3, "PVR play already enable!\n");
        return FALSE;
    }

    if(_rPVRPlay.u4BufSize == 0)
    {
        LOG(1, "No buffer allocated!\n");
        return FALSE;
    }

    ///////// Set framer ///////////

#ifdef CC_DMX_EMULATION
    DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD, 0x100000);
#else
    DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD, 0xFFFFFFFF);
#endif

    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD, _rPVRPlay.u4ThresholdSize);

    u4RegControl = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & 0xFFFCFFFF;
    if(_fgTSOut)
    {
        u4RegControl |= (1 << 16);
    }
    else
    {
        u4RegControl |= (1 << 17);
    }
    DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4RegControl);

    u4RegControl = 0x0;
    u4RegControl |= (((UINT32)_rPVRPlay.u2TimeStampFreqDiv) << 16) |
                    (1 << 14) |                                     // Enable interrupt
                    (1 << 6) |                                      // Enable bit
                    ((_rPVRPlay.fgIgnoreTimeStamp ? 1: 0) << 3) |   // Ignore timestamp
                    ((_rPVRPlay.fgContainTimeStamp ? 1 : 0) << 0);  // Contain timestamp

    u4RegControl |= 0x200;          // Reset timestamp
    u4RegControl |= 0x20;
    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, u4RegControl);

    _rPVRPlay.eState = DMX_PVRPLAY_STATE_PLAYING;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Stop
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Stop(BOOL fgForce)
{
    //LOG(3, "%s, force:%d\n", __FUNCTION__, (INT32)fgForce);

    if(!_DmxPVRPlay_DoStop(NULL))
    {
        return FALSE;
    }

    if(fgForce)
    {
        _DMX_PVRPlay_Reset();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Pause
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Pause(DMX_PVRPLAY_PTR_T *prPtr)
{
    //LOG(3, "%s\n", __FUNCTION__);
#if 1
    if(!_DmxPVRPlay_DoStop(prPtr))
    {
        return FALSE;
    }

    _DMX_PVRPlay_Reset();
#else
    UINT32 u4Ctrl;

    u4Ctrl = DMXCMD_READ32(DMX_REG_PVR_CONTROL) & (~0x400);
    u4Ctrl |= 0x400;
    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, u4Ctrl);
#endif

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Resume
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Resume(DMX_PVRPLAY_PTR_T *prPtr)
{
#if 1
    //LOG(3, "%s\n", __FUNCTION__);

    if(prPtr == NULL)
    {
        return FALSE;
    }

    if(!_DMX_PVRPlay_SetBuffer(_rPVRPlay.u4BufStart, _rPVRPlay.u4BufEnd - 1,
                               prPtr->u4Wp, prPtr->u4Rp))
    {
        return FALSE;
    }

    if(!_DMX_PVRPlay_Start())
    {
        return FALSE;
    }
#else
    UINT32 u4Ctrl;

    u4Ctrl = DMXCMD_READ32(DMX_REG_PVR_CONTROL);
    u4Ctrl &= ~0x400;
    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, u4Ctrl);
#endif

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Reset
 */
//-----------------------------------------------------------------------------
VOID _DMX_PVRPlay_Reset(void)
{
    if(_rPVRPlay.eState == DMX_PVRPLAY_STATE_PLAYING)
    {
        ASSERT(0);
    }

    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0x100);
    HAL_Delay_us(1);
    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0x0);
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_GetState
 */
//-----------------------------------------------------------------------------
DMX_PVR_PLAYSTATE_T _DMX_PVRPlay_GetState(void)
{
    return _rPVRPlay.eState;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Set
    Push or pull mode
    Timestamp
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Set(UINT32 u4Flags, const DMX_PVR_PLAY_T* prPVRPlay)
{
    UINT32 u4BufStart, u4BufEnd;
    UINT32 u4VirBufStart, u4PhyBufStart, u4PhyBufEnd;

    if(prPVRPlay == NULL)
    {
        return FALSE;
    }

    if(_rPVRPlay.eState == DMX_PVRPLAY_STATE_PLAYING)
    {
        LOG(3, "Now playing, stop first!\n");
        return FALSE;
    }

    if(u4Flags == PVRPLAY_FLAGS_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();

    if(u4Flags & PVRPLAY_FLAGS_TIMESTAMP)
    {
        _rPVRPlay.fgContainTimeStamp = prPVRPlay->fgContainTimeStamp;
        _rPVRPlay.fgIgnoreTimeStamp = prPVRPlay->fgIgnoreTimeStamp;
        _rPVRPlay.u2TimeStampFreqDiv = prPVRPlay->u2TimeStampFreqDiv;

        _rPVRPlayCounter.fgUseTimestamp = !prPVRPlay->fgIgnoreTimeStamp;
    }

    if(u4Flags & PVRPLAY_FLAGS_CALLBACK)
    {
        _rPVRPlay.pfnPVRNotify = prPVRPlay->pfnPVRNotify;
    }

    if(u4Flags & PVRPLAY_FLAGS_THRESHOLD)
    {
        _rPVRPlay.u4ThresholdSize = prPVRPlay->u4ThresholdSize;
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD, _rPVRPlay.u4ThresholdSize);
    }

    if(u4Flags & PVRPLAY_FLAGS_MODE)
    {
        _rPVRPlay.eMode = prPVRPlay->eMode;
    }

    if(u4Flags & PVRPLAY_FLAGS_BUFFER)
    {
        if((_rPVRPlay.u4BufStart != 0) || (_rPVRPlay.u4BufEnd != 0))
        {
            _DMX_Unlock();
            LOG(3, "Buffer already allocated\n");
            return FALSE;
        }

        if(prPVRPlay->fgAllocBuf)
        {
            u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(prPVRPlay->u4BufSize, 32);
            if (u4BufStart == 0)
            {
                _DMX_Unlock();
                LOG(1, "Out of memory!\n");
                return FALSE;
            }
            u4BufEnd = u4BufStart + prPVRPlay->u4BufSize;
            //LOG(5, "PVR play buffer: 0x%08x ~ 0x%08x\n", u4BufStart, u4BufEnd);
            // Can not print here
            //Printf("PVR play buffer: 0x%08x ~ 0x%08x\n", u4BufStart, u4BufEnd);
   	        HalInvalidateDCacheMultipleLine(VIRTUAL(u4BufStart), prPVRPlay->u4BufSize);
        }
        else
        {
            ASSERT(prPVRPlay->u4BufStart != 0);
            u4BufStart = prPVRPlay->u4BufStart;
            u4BufEnd = prPVRPlay->u4BufStart + prPVRPlay->u4BufSize;
        }

        u4VirBufStart = VIRTUAL(u4BufStart);

        _rPVRPlay.fgAllocBuf = prPVRPlay->fgAllocBuf;
        _rPVRPlay.u4BufStart = u4BufStart;
        _rPVRPlay.u4BufEnd = u4BufStart + prPVRPlay->u4BufSize;
        _rPVRPlay.u4BufSize = prPVRPlay->u4BufSize;
        _rPVRPlay.u4Wp = u4VirBufStart;
        _rPVRPlay.u4Rp = u4VirBufStart;

        u4PhyBufStart = PHYSICAL(u4BufStart);
        u4PhyBufEnd = PHYSICAL(u4BufEnd);

        // Set playback buffer
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START, u4PhyBufStart);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END, u4PhyBufEnd - 1);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, u4PhyBufStart);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, u4PhyBufStart);
    }

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Get
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Get(UINT32 u4Flags, DMX_PVR_PLAY_T* prPVRPlay)
{
    UINT32 u4Wp, u4Rp;

    if(prPVRPlay == NULL)
    {
        return FALSE;
    }

    if(u4Flags == PVRPLAY_FLAGS_NONE)
    {
        return TRUE;
    }

    if(u4Flags & PVRPLAY_FLAGS_TIMESTAMP)
    {
        prPVRPlay->fgContainTimeStamp = _rPVRPlay.fgContainTimeStamp;
        prPVRPlay->fgIgnoreTimeStamp = _rPVRPlay.fgIgnoreTimeStamp;
        prPVRPlay->u2TimeStampFreqDiv = _rPVRPlay.u2TimeStampFreqDiv;
    }

    if(u4Flags & PVRPLAY_FLAGS_THRESHOLD)
    {
        prPVRPlay->u4ThresholdSize = _rPVRPlay.u4ThresholdSize;
    }

    if(u4Flags & PVRPLAY_FLAGS_CALLBACK)
    {
        prPVRPlay->pfnPVRNotify = _rPVRPlay.pfnPVRNotify;
    }

    if(u4Flags & PVRPLAY_FLAGS_MODE)
    {
        prPVRPlay->eMode = _rPVRPlay.eMode;
    }

    if(u4Flags & PVRPLAY_FLAGS_BUFFER)
    {
        prPVRPlay->fgAllocBuf = _rPVRPlay.fgAllocBuf;
        prPVRPlay->u4BufStart = _rPVRPlay.u4BufStart;
        prPVRPlay->u4BufEnd = _rPVRPlay.u4BufEnd;
        prPVRPlay->u4BufSize = _rPVRPlay.u4BufSize;
        prPVRPlay->u4ThresholdSize = _rPVRPlay.u4ThresholdSize;

        _DMX_Lock();
        u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP);
        u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP);
        _DMX_Unlock();

        prPVRPlay->u4Rp = VIRTUAL(u4Rp);
        prPVRPlay->u4Wp = VIRTUAL(u4Wp);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Notify
    Call in HISR
*/
//-----------------------------------------------------------------------------
VOID _DMX_PVRPlay_Notify(void)
{
    //UINT32 u4OrgRp, u4Rp;
    UINT32 u4OrgRp, u4Rp, u4Threshold, u4BufEnd, u4BufSize;

    if(_rPVRPlay.eMode == DMX_PVR_PLAY_SINGLEMOVE)
    {
        VERIFY(x_sema_unlock(_hDMXPVRPlaySema) == OSR_OK);
        return;
    }

#ifdef CC_DMX_EMULATION
    _PVR_EMU_Notify();
#else
    if(_rPVRPlay.eState != DMX_PVRPLAY_STATE_PLAYING)
    {
        LOG(3, "Interrupt occur when play disable\n");
        return;
    }

    _DMX_Lock();
    u4OrgRp = _rPVRPlay.u4Rp;
    u4BufEnd = _rPVRPlay.u4BufEnd;
    u4BufSize = _rPVRPlay.u4BufSize;
    u4Threshold = _rPVRPlay.u4ThresholdSize;
    u4Rp = u4OrgRp + u4Threshold;
    if(u4Rp >= u4BufEnd)
    {
        u4Rp -= u4BufSize;
    }
    _rPVRPlay.u4Rp = u4Rp;
    _DMX_Unlock();

    if(_rPVRPlay.pfnPVRNotify != NULL)
    {
        if(!_rPVRPlay.pfnPVRNotify(u4OrgRp, u4Threshold))
        {
            LOG(3, "PVR play notfiy fail\n");
        }
    }
#endif
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_UpdateWp
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_UpdateWp(UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 u4NewWp, u4BufStart, u4BufEnd;
    UINT8 *pu1Tmp;

    _DMX_Lock();
    u4BufStart = _rPVRPlay.u4BufStart;
    u4BufEnd = _rPVRPlay.u4BufEnd;
    _rPVRPlayCounter.u4MoveCount++;
    _rPVRPlayCounter.u4MoveSize += u4Size;
    _DMX_Unlock();

    u4NewWp = _DmxPVRPlay_UpdateAddr(u4Addr, u4Size, u4BufStart, u4BufEnd);

    _DMX_Lock();
    _rPVRPlay.u4Wp = u4NewWp;
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, PHYSICAL(u4NewWp));
    _DMX_Unlock();

    // Check
    u4NewWp = u4Addr + 4;
    if(u4NewWp > u4BufEnd)
    {
        u4NewWp -= (u4BufEnd - u4BufStart);
    }
    pu1Tmp = (UINT8*)u4NewWp;
    if(pu1Tmp[0] == 0x47)
    {
        _DMX_Lock();
        _rPVRPlayCounter.u4MoveCheck++;
        _DMX_Unlock();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_FreeBuf
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_FreeBuf(void)
{
    BOOL fgAllocBuf;
    UINT32 u4BufStart;

    _DMX_Lock();
    fgAllocBuf = _rPVRPlay.fgAllocBuf;
    u4BufStart = _rPVRPlay.u4BufStart;
    _DMX_Unlock();

    if(fgAllocBuf && (u4BufStart != 0))
    {
        if (!BSP_FreeAlignedDmaMemory(u4BufStart))
        {
            LOG(3, "Fail to free PVR play buffer!, addr: 0x%08x\n", u4BufStart);
            return FALSE;
        }
    }

    _DMX_Lock();
    _rPVRPlay.u4BufStart = 0x0;
    _rPVRPlay.u4BufEnd = 0x0;
    _rPVRPlay.u4Wp = 0x0;
    _rPVRPlay.u4Rp = 0x0;
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START, 0x0);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END, 0x0);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, 0x0);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, 0x0);
    _DMX_Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_FlushBuf
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_FlushBuf(void)
{
    UINT32 u4BufStart;

    _DMX_Lock();
    u4BufStart = _rPVRPlay.u4BufStart;

    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, u4BufStart);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, u4BufStart);

    _rPVRPlay.u4Rp = u4BufStart;
    _rPVRPlay.u4Wp = u4BufStart;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SingleMove
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_SingleMove(const DMX_MM_DATA_T* prData)
{
    UINT32 u4PhyBufStart, u4PhyBufEnd, u4PhyRp, u4PhyWp;
    UINT32 u4Wp, u4BufSize;
    BOOL fgRet;
    UINT32 u4CheckAddr;
    UINT8 *pu1Tmp;
#ifdef DMX_PVRPLAY_MEASURE_TIME
    HAL_TIME_T rTime1, rTime2, rTimeDiff;

    HAL_GetTime(&rTime1);
#endif

    if(prData == NULL)
    {
        return FALSE;
    }

    ASSERT(DMX_IsAligned(prData->u4BufStart, DMX_PVR_BUF_ALIGNMENT));
    ASSERT(DMX_IsAligned(prData->u4BufEnd,   DMX_PVR_BUF_ALIGNMENT));
    // PVR WP, RP is byte-alignment
    //ASSERT(DMX_IsAligned(prData->u4StartAddr,DMX_PVR_POINTER_ALIGNMENT));
    //ASSERT(DMX_IsAligned(prData->u4FrameSize,DMX_PVR_POINTER_ALIGNMENT));

    ASSERT(_rPVRPlay.eMode == DMX_PVR_PLAY_SINGLEMOVE);
    ASSERT(_rPVRPlay.fgIgnoreTimeStamp);

    if(_rPVRPlay.eState != DMX_PVRPLAY_STATE_STOP)
    {
        return FALSE;
    }

    if(prData->u4FrameSize == 0)
    {
        return TRUE;
    }

    if((prData->u4StartAddr < prData->u4BufStart) ||
       (prData->u4StartAddr >= prData->u4BufEnd))
    {
        LOG(3, "Addr invalid\n");
        return FALSE;
    }

    u4BufSize = prData->u4BufEnd - prData->u4BufStart;
    if(prData->u4FrameSize >= u4BufSize)
    {
        LOG(3, "Size is too large\n");
        return FALSE;
    }

    u4Wp = prData->u4StartAddr + prData->u4FrameSize;
    if(u4Wp >= prData->u4BufEnd)
    {
        u4Wp -= u4BufSize;
    }

    u4PhyBufStart = PHYSICAL(prData->u4BufStart);
    u4PhyBufEnd = PHYSICAL(prData->u4BufEnd);
    u4PhyRp = PHYSICAL(prData->u4StartAddr);
    u4PhyWp = PHYSICAL(u4Wp);

    HalFlushInvalidateDCache();     // Fixme

    // PVR Start
    _DMX_Lock();
    _rPVRPlay.u4BufStart = VIRTUAL(prData->u4BufStart);
    _rPVRPlay.u4BufEnd = VIRTUAL(prData->u4BufEnd);
    _rPVRPlay.u4BufSize = u4BufSize;
    _rPVRPlay.u4Wp = VIRTUAL(u4Wp);
    _rPVRPlay.u4Rp = VIRTUAL(prData->u4StartAddr);
    _rPVRPlay.u4ThresholdSize = prData->u4FrameSize;
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START, u4PhyBufStart);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END, u4PhyBufEnd - 1);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, u4PhyRp);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, u4PhyWp);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD, prData->u4FrameSize);
    _DMX_Unlock();

    //// Counters
    _DMX_Lock();
    _rPVRPlayCounter.u4SingleCount++;
    _rPVRPlayCounter.u4SingleSize += prData->u4FrameSize;
    u4CheckAddr = prData->u4StartAddr + 4;
    if(u4CheckAddr > _rPVRPlay.u4BufEnd)
    {
        u4CheckAddr -= _rPVRPlay.u4BufSize;
    }
    pu1Tmp = (UINT8*)u4CheckAddr;
    if(pu1Tmp[0] == 0x47)
    {
        _rPVRPlayCounter.u4SingleCheck++;
    }
    _DMX_Unlock();

    if(!_DMX_PVRPlay_Start())
    {
        return FALSE;
    }

    fgRet = TRUE;

    VERIFY(OSR_OK == x_sema_lock(_hDMXPVRPlaySema, X_SEMA_OPTION_WAIT));

#if 0
    HAL_GetTime(&rTime2);
    HAL_GetDeltaTime(&rTimeDiff, &rTime2, &rTime1);
    LOG(3, "Single Move: size:%d .%06d\n", prData->u4FrameSize, rTimeDiff.u4Micros);
#endif

    if(!_DMX_PVRPlay_Stop(TRUE))
    {
        LOG(3, "Stop error\n");
        fgRet = FALSE;
    }

#ifdef DMX_PVRPLAY_MEASURE_TIME
    HAL_GetTime(&rTime2);
    HAL_GetDeltaTime(&rTimeDiff, &rTime2, &rTime1);
    LOG(5, "Single Move: size:%d .%06d\n", prData->u4FrameSize, rTimeDiff.u4Micros);
#endif

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_GetBufPointer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_GetBufPointer(DMX_PVRPLAY_BUFPTR_T *prPtr)
{
    if(prPtr == NULL)
    {
        return FALSE;
    }

    _DMX_Lock();
    prPtr->u4HwWp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP));
    prPtr->u4HwRp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP));
    prPtr->u4SwWp = _rPVRPlay.u4Wp;
    prPtr->u4SwRp = _rPVRPlay.u4Rp;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SetBuffer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_SetBuffer(UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4Wp, UINT32 u4Rp)
{
    ASSERT(u4BufStart < u4BufEnd);
    ASSERT((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
    ASSERT((u4Rp >= u4BufStart) && (u4Rp < u4BufEnd));
    ASSERT((u4BufEnd % 4) == 3);        // End - 1

    if(_rPVRPlay.eState != DMX_PVRPLAY_STATE_STOP)
    {
        return FALSE;
    }

    _DMX_Lock();
    _rPVRPlay.u4BufStart = u4BufStart;
    _rPVRPlay.u4BufEnd = u4BufEnd + 1;
    _rPVRPlay.u4Wp = u4Wp;
    _rPVRPlay.u4Rp = u4Rp;
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START, PHYSICAL(u4BufStart));
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END, PHYSICAL(u4BufEnd));
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, PHYSICAL(u4Wp));
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, PHYSICAL(u4Rp));
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_RequestReset
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_RequestReset(UINT8 u1TsIdx)
{
    LOG(3, "_DMX_PVRPlay_RequestReset\n");

    if(_rPVRPlay.eState == DMX_PVRPLAY_STATE_PLAYING)
    {
        ASSERT(0);
    }

    _DMX_PVRPlay_Reset();

    _DMX_Lock();
    _rPVRPlay.u4BufStart = 0;
    _rPVRPlay.u4BufEnd = 0;
    _rPVRPlay.u4Wp = 0;
    _rPVRPlay.u4Rp = 0;
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START, 0);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END, 0);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, 0);
    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, 0);
    _DMX_Unlock();

    // Reset DMX
    if(!_DDI_TsResetDmxBuffers(u1TsIdx))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_GetCounter
 */
//-----------------------------------------------------------------------------
void _DMX_PVRPlay_GetCounter(DMX_PVRPLAY_COUNTER_T *prCounter)
{
    UINT32 u4Rp, u4Wp;
    if(prCounter != NULL)
    {
        _DMX_Lock();
        u4Wp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP));
        u4Rp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP));
        _rPVRPlayCounter.u4DataSize = DATASIZE(u4Rp, u4Wp, _rPVRPlay.u4BufSize);
        *prCounter = _rPVRPlayCounter;
        _DMX_Unlock();
    }
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_ResetCounter
 */
//-----------------------------------------------------------------------------
void _DMX_PVRPlay_ResetCounter(void)
{
    _DMX_Lock();
    x_memset((void*)&_rPVRPlayCounter, 0, sizeof(DMX_PVRPLAY_COUNTER_T));
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SetPort
 */
//-----------------------------------------------------------------------------
void _DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_T ePort)
{
    UINT8 u1TsIdx = 0;
    BOOL fgTSOut = FALSE;
    UINT32 u4Reg;

    switch(ePort)
    {
    case DMX_PVRPLAY_PORT_FRAMER0:
        u1TsIdx = 0;
        fgTSOut = TRUE;
        break;

    case DMX_PVRPLAY_PORT_FRAMER1:
        u1TsIdx = 1;
        fgTSOut = TRUE;
        break;

    case DMX_PVRPLAY_PORT_DBM:
        u1TsIdx = 2;
        fgTSOut = FALSE;
        break;

    default:
        ASSERT(0);
    }

    if(fgTSOut)
    {
        // PVR TS-out
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
        u4Reg &= ~0xFC0000;
        u4Reg |= (0 << 18) |        // 0: MSB, 1: LSB
                 (0 << 19) |        // 0: 8 sync cycle, 1: 1 sync cycle
                 (0 << 20) |        // 0 ~ 3: valid space cycle
                 (1 << 22) |        // 0:pos edge, 1:neg edge
                 (1 << 23);         // 0:serial, 1:paralle
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);

        // TS-out clock
        u4Reg = CKGEN_READ32(0x214);
        u4Reg &= ~0x8F;
        u4Reg |= 1;  // 108MHz
        //u4Reg |= 2;  // 72MHz
        CKGEN_WRITE32(0x214, u4Reg);

        // TS-out framer selection
        if((u1TsIdx == 0) || (u1TsIdx == 1))
        {
            _DMX_SetFrontEndEx(u1TsIdx, DMX_FE_TSOUT);
        }
    }

    _DMX_Lock();
    _fgTSOut = fgTSOut;
    _ePVRPort = ePort;
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_GetPort
 */
//-----------------------------------------------------------------------------
DMX_PVRPLAY_PORT_T _DMX_PVRPlay_GetPort(void)
{
    DMX_PVRPLAY_PORT_T ePort;

    _DMX_Lock();
    ePort = _ePVRPort;
    _DMX_Unlock();

    return ePort;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SetMoveMode
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_SetMoveMode(DMX_TSFMT_T eTSFmt, BOOL fgForce)
{
    BOOL fgContainTimestamp, fgUseTimestamp;
    DMX_PVR_PLAY_T rPlay;
    DMX_PVR_PLAYSTATE_T eState;

    _DMX_Lock();
    eState = _rPVRPlay.eState;
    _DMX_Unlock();

    if(eState == DMX_PVRPLAY_STATE_PLAYING)
    {
        return FALSE;
    }

    if(fgForce)
    {
        if(_eDmxPVRTSFmt == eTSFmt)
        {
            return TRUE;
        }
    }

    fgContainTimestamp = FALSE;
    fgUseTimestamp = FALSE;

    if((eTSFmt == DMX_TSFMT_192) || (eTSFmt == DMX_TSFMT_192_ENCRYPT))
    {
        fgContainTimestamp = TRUE;
    }
    else if(eTSFmt == DMX_TSFMT_TIMESHIFT)
    {
        fgContainTimestamp = TRUE;
        fgUseTimestamp = TRUE;
    }

    if(fgContainTimestamp && fgUseTimestamp)
    {
        rPlay.eMode = DMX_PVR_PLAY_STREAM;
    }
    else
    {
        rPlay.eMode = DMX_PVR_PLAY_SINGLEMOVE;
    }

    rPlay.fgContainTimeStamp = fgContainTimestamp;
    rPlay.fgIgnoreTimeStamp = !fgUseTimestamp;
    rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
    if(!_DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP), &rPlay))
    {
        return FALSE;
    }

    _eDmxPVRTSFmt = eTSFmt;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_OpenTimeshift
 */
//-----------------------------------------------------------------------------
BOOL _DMX_OpenTimeshift(BOOL fgOpen)
{
    if(fgOpen)
    {
        //// Fixme, check resource conflict
        if(!_DMX_PVRPlay_SetMoveMode(DMX_TSFMT_TIMESHIFT, TRUE))
        {
            return FALSE;
        }
    }

    _DMX_Lock();
    _fgDmxTimeshiftOpen = fgOpen;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_IsTimeshiftOpen
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IsTimeshiftOpen(void)
{
    BOOL fgOpen;

    _DMX_Lock();
    fgOpen = _fgDmxTimeshiftOpen;
    _DMX_Unlock();

    return fgOpen;
}


#if defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)
//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SetTSOut
 *  For emulation
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_SetTSOut(BOOL fgEnable)
{
    //UINT32 u4Cfg;

    if(fgEnable)
    {
    }
    else
    {
    }

    _fgTSOut = fgEnable;

    return TRUE;
}
#endif  // defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)

