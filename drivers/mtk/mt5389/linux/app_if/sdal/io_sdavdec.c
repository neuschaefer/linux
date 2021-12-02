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
 * $RCSfile: io_sdavdec.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sdavdec.c
 *  SDAL SdAVDec Module ioctl interface of MT538X.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_if.h"
#include "mpv_if.h"
#include "vdec_if.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "vdec_drvif.h"
#include "fbm_drvif.h"
#include "vdp_if.h"
#include "feeder_if.h"
#include "sd_vdopath.h"
#include "sd_cb.h"
#include "cb_low.h"
#include "sd_audio.h"
#include "sd_dmx.h"
#include "x_assert.h"
#include "x_linux.h"
#include <linux/sdal_ioctl.h>


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define CC_SDAL_DEBLOCK_OFF

#define SUPPORT_AUXOUT
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// Internal MPV Info structure
typedef struct
{
    UCHAR ucEsId;
    UCHAR ucStcId;
} _MPV_INFO_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define SOUND_ID_TO_DEC_ID(SdSoundID_k)                         \
    (SdSoundID_k == SD_SOUND1) ? AUD_DEC_AUX : AUD_DEC_MAIN     \


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// Tracking if nested critical section occurred
static BOOL _fgLocking = FALSE;

/// Critical section state
static CRIT_STATE_T _rLock;

/// Video pidx
static UINT8 _u1VideoPidx = DMX_NUM_PID;

/// Audio pidx
static UINT8 _u1AudioPidx = DMX_NUM_PID;

/// PCR pidx
static UINT8 _u1PcrPidx = DMX_NUM_PID;

/// Backup of decoder settings
SdAVDec_Settings_t rAvDecSettings;

/// MPV info
static _MPV_INFO_T _arMpvInfo[SD_AVDEC_MAX];

/// Is demux allocated or not
static BOOL _fgDemuxAllocated = FALSE;

/// Call Back info
static SdAVDecID_k _eCbAVDecId = SD_AVDEC0;
static uint32_t _u4CbParam = 0;
static SdMask_t _rCbMask = 0;
static SdBool_t _bCbOneShot = 0;
static SdMask_t _rCbShotMask = 0;
static SdAVDec_CallBack _pCbCallback = 0;

//-----------------------------------------------------------------------------
// Externl function forward declarations
//-----------------------------------------------------------------------------
extern BOOL AUD_DtvLock(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static void _AvDec_VdpUnMuteCb(UINT32 u4VdpId, BOOL fgUnMute);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Enter critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _Lock(void)
{
    _rLock = x_crit_start();

    ASSERT(!_fgLocking);
    _fgLocking = TRUE;

    UNUSED(_fgLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** Leave critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _Unlock(void)
{
    ASSERT(_fgLocking);
    _fgLocking = FALSE;

    x_crit_end(_rLock);

    UNUSED(_fgLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** _GetStcId
 *  Get STC ID
 *
 *  @param  u1DecoderId     Decoder id
 *
 *  @retval The STC ID
 */
//-----------------------------------------------------------------------------
static UINT8 _GetStcId(UINT8 u1DecoderId)
{
    UINT8 u1StcId;

    switch (u1DecoderId)
    {
    case SD_AVDEC0:
        u1StcId = STC_SRC_A1;
        break;

    case SD_AVDEC1:
        u1StcId = STC_SRC_A2;
        break;

    default:
        // Invalid ID
        u1StcId = STC_SRC_NS;
        break;
    }

    return u1StcId;
}


//-----------------------------------------------------------------------------
/** _SendVideoPesCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MPEGVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rMpvPes;

    ASSERT(prPes != NULL);

    x_memset(&rMpvPes, 0, sizeof(rMpvPes));
    switch (prPes->u4FrameType)
    {
    case PIC_TYPE_I:
        rMpvPes.ucPicType = MPV_I_TYPE;
        break;

    case PIC_TYPE_P:
        rMpvPes.ucPicType = MPV_P_TYPE;
        break;

    case PIC_TYPE_B:
        rMpvPes.ucPicType = MPV_B_TYPE;
        break;

    default:
        ASSERT(0);
        break;
    }

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;
    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;
    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    rMpvPes.fgGop = prPes->fgGop;

    return (VDEC_SendEs(&rMpvPes) == 1);
}

//-----------------------------------------------------------------------------
/** _H264VideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _H264VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rMpvPes;
    UINT32 u4Addr;

    ASSERT(prPes != NULL);

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }    
    
    x_memset(&rMpvPes, 0, sizeof(rMpvPes));
    rMpvPes.ucPicType = *(UINT8*)VIRTUAL(u4Addr);
    
    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;
    
    u4Addr = prPes->u4FrameAddr + 4;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }    
    rMpvPes.u4VldReadPtr = u4Addr;
    
    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    //rMpvPes.fgGop = prPes->fgGop;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}

//-----------------------------------------------------------------------------
/** _VideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _VideoCallback(const DMX_PES_MSG_T* prPes)
{
    switch(DMX_GetVideoType())
    {
    case DMX_VIDEO_MPEG:
        return _MPEGVideoCallback(prPes);
        break;

    case DMX_VIDEO_H264:
        return _H264VideoCallback(prPes);
        break;

    default:
        printk("Unknown video type\n");
        break;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DemuxResetVideoPid
 *  Reset demux
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DemuxResetVideoPid(void)
{
    UINT8 u1Pidx;

    u1Pidx = _u1VideoPidx;

    // Release PID
    if (u1Pidx < DMX_NUM_PID)
    {
        DMX_PID_T rPid;

        // Reset mapping of PID index and STC id
        UNUSED(DMX_SetStcSrc(u1Pidx, STC_SRC_NS));

        // Disable PID
        rPid.fgEnable = FALSE;
        if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            return FALSE;
        }

        // Free PID
        VERIFY(DMX_FreePid(u1Pidx));
        VERIFY(_DMX_ReleasePidx(u1Pidx));

        _Lock();
        _u1VideoPidx = DMX_NUM_PID;
        _Unlock();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DemuxResetAudioPid
 *  Reset demux
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DemuxResetAudioPid(void)
{
    UINT8 u1Pidx;

    u1Pidx = _u1AudioPidx;

    if (u1Pidx < DMX_NUM_PID)
    {
        DMX_PID_T rPid;

        // Reset mapping of PID index and STC id
        UNUSED(DMX_SetStcSrc(u1Pidx, STC_SRC_NS));

        // Disable PID
        rPid.fgEnable = FALSE;
        if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            return FALSE;
        }

        // Free PID
        VERIFY(DMX_FreePid(u1Pidx));
        VERIFY(_DMX_ReleasePidx(u1Pidx));

        _Lock();
        _u1AudioPidx = DMX_NUM_PID;
        _Unlock();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DemuxResetPcrPid
 *  Reset demux
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DemuxResetPcrPid(void)
{
    UINT8 u1Pidx;

    u1Pidx = _u1PcrPidx;

    if (u1Pidx < DMX_NUM_PID)
    {
        DMX_PID_T rPid;

        // Disable PID
        rPid.fgEnable = FALSE;
        if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            return FALSE;
        }

        // Free PID
        VERIFY(DMX_FreePid(u1Pidx));
        VERIFY(_DMX_ReleasePidx(u1Pidx));

        _Lock();
        _u1PcrPidx = DMX_NUM_PID;
        _Unlock();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DemuxReset
 *  Reset demux
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DemuxReset(void)
{
    BOOL fgRet = TRUE;

    if (!_DemuxResetVideoPid())
    {
        fgRet = FALSE;
    }

    if (!_DemuxResetAudioPid())
    {
        fgRet = FALSE;
    }

    if (!_DemuxResetPcrPid())
    {
        fgRet = FALSE;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DemuxSet
 *  Set demux attributes
 *
 *  @param  u1DecoderId     Decoder id
 *  @param  pSettings       AD decoder settings
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DemuxSet(UINT8 u1DecoderId, SdAVDec_Settings_t* pSettings)
{
    DMX_PID_T rPid, rPid0;
    FBM_POOL_T* prFbmPool;
    UINT32 au4BufStart[2], au4BufEnd[2];
    UINT8 u1StcId, u1Channel;
    UINT8 u1VideoPidx, u1AudioPidx, u1PcrPidx;
    BOOL fgRet;

    ASSERT(pSettings != NULL);

    fgRet = FALSE;
    u1StcId = STC_SRC_A1;

    // Setup default attributes
    rPid.fgEnable = TRUE;
    rPid.fgAllocateBuffer = FALSE;
    rPid.fgPrimary = TRUE;
    rPid.u1TsIndex = 0;
    rPid.u1DeviceId = u1DecoderId;
    rPid.u1KeyIndex = 0;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.u2Pid = (UINT16)0x1fff;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = 0;
    rPid.ePidType = DMX_PID_TYPE_NONE;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.pfnNotify = NULL;
    rPid.pfnScramble = NULL;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;

    _Lock();
    u1VideoPidx = _u1VideoPidx;
    u1AudioPidx = _u1AudioPidx;
    u1PcrPidx = _u1PcrPidx;
    _Unlock();

    if (pSettings->video.pid >= 0)
    {
        // Get current pid value
        rPid0.u2Pid = 0xffff;
        if (u1VideoPidx < DMX_NUM_PID)
        {
            if (!DMX_GetPid(u1VideoPidx, DMX_PID_FLAG_PID, &rPid0))
            {
                goto _exit;
            }
        }

        // Check if pid value changed
        if (rPid0.u2Pid != (UINT16)pSettings->video.pid)
        {
            // Reset video pidx
            if (!_DemuxResetVideoPid())
            {
                goto _exit;
            }

            // Allocate video pidx
            u1VideoPidx = DMX_NUM_PID;
            if (!_DMX_AllocatePidx(&u1VideoPidx, pSettings->video.pid))
            {
                goto _exit;
            }

            // Allocate video buffer from FBM
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
            ASSERT(prFbmPool != NULL);
            ASSERT(prFbmPool->u4Addr != 0);

            // Setup video pid
            rPid.u2Pid = (UINT16)pSettings->video.pid;
            rPid.u4BufAddr = prFbmPool->u4Addr;
            rPid.u4BufSize = prFbmPool->u4Size;
            rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;

            if (pSettings->pcr.pid == pSettings->video.pid)
            {
                rPid.ePcrMode = DMX_PCR_MODE_NEW;

                // Set mapping of PID index and STC id
                u1StcId = _GetStcId(0);
                UNUSED(DMX_SetStcSrc(u1VideoPidx, u1StcId));

                // Check if PCR is already set
                if (u1PcrPidx < DMX_NUM_PID)
                {
                    // Reset the standalone PCR pid
                    if (!_DemuxResetPcrPid())
                    {
                        goto _exit;
                    }
                    u1PcrPidx = DMX_NUM_PID;
                }
            }
            else
            {
                rPid.ePcrMode = DMX_PCR_MODE_NONE;
            }

            if (!DMX_SetPid(u1VideoPidx, DMX_PID_FLAG_ALL, &rPid))
            {
                goto _exit;
            }
        }
    }
    else    // if (pSettings->video.pid >= 0)
    {
        // Reset video pidx
        if (!_DemuxResetVideoPid())
        {
            goto _exit;
        }
        u1VideoPidx = DMX_NUM_PID;
    }

    if (pSettings->audio.pid >= 0)
    {
        // Get current pid value
        rPid0.u2Pid = 0xffff;
        if (u1AudioPidx < DMX_NUM_PID)
        {
            if (!DMX_GetPid(u1AudioPidx, DMX_PID_FLAG_PID, &rPid0))
            {
                goto _exit;
            }
        }

        // Check if pid value changed
        if (rPid0.u2Pid != (UINT16)pSettings->audio.pid)
        {
            // Reset video pidx
            if (!_DemuxResetAudioPid())
            {
                goto _exit;
            }

            // Allocate audio pidx
            u1AudioPidx = DMX_NUM_PID;
            if (!_DMX_AllocatePidx(&u1AudioPidx, pSettings->audio.pid))
            {
                goto _exit;
            }

            // Allocate audio buffer
            if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
                &au4BufEnd[1]) != AUD_OK)
            {
                // Can't allocate audio buffer
                goto _exit;
            }
            if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0))
            {
                // Can't allocate audio buffer
                goto _exit;
            }

            // Set audio pid
            rPid.u2Pid = (UINT16)pSettings->audio.pid;
            rPid.u4BufAddr = au4BufStart[0];
            rPid.u4BufSize = au4BufEnd[0] - au4BufStart[0];
            rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;

            if (pSettings->pcr.pid == pSettings->audio.pid)
            {
                rPid.ePcrMode = DMX_PCR_MODE_NEW;

                // Set mapping of PID index and STC id
                u1StcId = _GetStcId(0);
                UNUSED(DMX_SetStcSrc(u1AudioPidx, u1StcId));

                // Check if PCR is already set
                if (u1PcrPidx < DMX_NUM_PID)
                {
                    // Reset the standalone PCR pid
                    if (!_DemuxResetPcrPid())
                    {
                        goto _exit;
                    }
                    u1PcrPidx = DMX_NUM_PID;
                }
            }
            else
            {
                rPid.ePcrMode = DMX_PCR_MODE_NONE;
            }

            if (!DMX_SetPid(u1AudioPidx, DMX_PID_FLAG_ALL, &rPid))
            {
                goto _exit;
            }
        }
    }
    else    // if (pSettings->audio.pid >= 0)
    {
        // Reset audio pidx
        if (!_DemuxResetAudioPid())
        {
            goto _exit;
        }
        u1AudioPidx = DMX_NUM_PID;
    }


    if (pSettings->pcr.pid >= 0)
    {
        // Set PCR pid
        if ((pSettings->pcr.pid != pSettings->video.pid) &&
            (pSettings->pcr.pid != pSettings->audio.pid))
        {
            // Get current pid value
            rPid0.u2Pid = 0xffff;
            if (u1PcrPidx < DMX_NUM_PID)
            {
                if (!DMX_GetPid(u1PcrPidx, DMX_PID_FLAG_PID, &rPid0))
                {
                    goto _exit;
                }
            }

            // Check if pid value changed
            if (rPid0.u2Pid != (UINT16)pSettings->pcr.pid)
            {
                // Reset video pidx
                if (!_DemuxResetPcrPid())
                {
                    goto _exit;
                }

                u1PcrPidx = DMX_NUM_PID;
                if (!_DMX_AllocatePidx(&u1PcrPidx, pSettings->pcr.pid))
                {
                    goto _exit;
                }

                rPid.u2Pid = pSettings->pcr.pid;
                rPid.u4BufAddr = 0;
                rPid.u4BufSize = 0;
                rPid.ePcrMode = DMX_PCR_MODE_NEW;
                rPid.ePidType = DMX_PID_TYPE_NONE;
                rPid.u1SteerMode = 0;
                rPid.fgPrimary = FALSE;

                if (!DMX_SetPid(u1PcrPidx, DMX_PID_FLAG_ALL, &rPid))
                {
                    goto _exit;
                }

                // Set mapping of PID index and STC id
                u1StcId = _GetStcId(0);
                UNUSED(DMX_SetStcSrc(u1PcrPidx, u1StcId));
            }
        }
    }
    else    // if (pSettings->pcr.pid >= 0)
    {
        // Reset pcr pidx
        if (!_DemuxResetPcrPid())
        {
            goto _exit;
        }
        u1PcrPidx = DMX_NUM_PID;
    }

    if (u1VideoPidx < DMX_NUM_PID)
    {
        // Get channel id
        if (!DMX_GetVideoChannel(u1VideoPidx, &u1Channel))
        {
            goto _exit;
        }

        // Set channel and STC id to MPV
        _arMpvInfo[u1DecoderId].ucEsId = u1Channel;
        _arMpvInfo[u1DecoderId].ucStcId = u1StcId;
    }

    fgRet = TRUE;

_exit:

    // Update global pid indexes even on errors, then next _DemuxReset() can
    // restore resources and states correctly
    _Lock();
    _u1VideoPidx = u1VideoPidx;
    _u1AudioPidx = u1AudioPidx;
    _u1PcrPidx = u1PcrPidx;
    _Unlock();

    return fgRet;
}

#if 0

static BOOL _CheckAudioLock(UINT8 u1DecId)
{
    INT32 i4AudPid=-1;
    DMX_PID_COUNTERS_T rCounters;
    DMX_PID_COUNTERS_T rCounters1;
    // Find audio pid
    // Show all counters of active PIDs
    DMX_PID_T rPid;
    UINT32 i;

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if (DMX_GetPid(i, DMX_PID_FLAG_VALID, &rPid))
        {
            // Get PID info
            if(DMX_GetPid(i, DMX_PID_FLAG_ALL, &rPid))
            {
                if (rPid.fgEnable && rPid.ePidType == DMX_PID_TYPE_ES_AUDIO)
                {
                    i4AudPid = i;
                    break;
                }
            }
        }
    }
    if(i4AudPid == -1 ) // no audio PID
    {
        return FALSE;
    }
    UNUSED(DMX_GetPidCounters((UINT8)(i4AudPid), &rCounters));

    x_thread_delay(30);

    UNUSED(DMX_GetPidCounters((UINT8)(i4AudPid),  &rCounters1));

    if(rCounters1.u4PesCount == rCounters.u4PesCount)
    {
        return FALSE;
    }
    return TRUE;

}

#endif


//-----------------------------------------------------------------------------
/** _AudSet
 *  Set Audio attributes
 *
 *  @param  pSettings       AD decoder settings
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _AudSet(SdAVDecID_k eAVDecId, SdAVDec_Settings_t* pSettings)
{
    UNUSED(eAVDecId);
    _SD_AUD_SetAudFmt(SD_SOUND0, pSettings->eAudFormat, pSettings);

    if (pSettings->bAVSyncOn)
    {
	AUD_SetAvSynMode(0, AV_SYNC_SLAVE);
    }
    else
    {
	AUD_SetAvSynMode(0, AV_SYNC_FREE_RUN);
    }

    if ((pSettings->eAudFormat == SD_AUDIO_FORMAT_MPEG1)||
    	(pSettings->eAudFormat == SD_AUDIO_FORMAT_MPEG2))	
    {
        switch(pSettings->eAudStereoMode)
        {
        case SD_AUD_STEREO_MODE_MONO:
              AUD_DspMonoDownMix(AUD_DEC_MAIN, TRUE);
              AUD_DspSetKaraokeMixRatio(AUD_DEC_MAIN, 0x400000);     
              break;
        case SD_AUD_STEREO_MODE_STEREO:
              AUD_DspMonoDownMix(AUD_DEC_MAIN, FALSE);
              break;
        case SD_AUD_STEREO_MODE_LEFT_TO_BOTH:
              AUD_DspMonoDownMix(AUD_DEC_MAIN, TRUE);
              AUD_DspSetKaraokeMixRatio(AUD_DEC_MAIN, 0);     
              break;
        case SD_AUD_STEREO_MODE_RIGHT_TO_BOTH:
              AUD_DspMonoDownMix(AUD_DEC_MAIN, TRUE);
              AUD_DspSetKaraokeMixRatio(AUD_DEC_MAIN, 0x800000);     
              break;
        default:
              AUD_DspMonoDownMix(AUD_DEC_MAIN, FALSE);
              break;
        }
    }
    else
    {
        AUD_DspMonoDownMix(AUD_DEC_MAIN, FALSE);
    }
    
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _MpvSet
 *  Set Mpv attributes
 *
 *  @param  pSettings       AD decoder settings
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _VdecSet(SdAVDecID_k eAVDecId, SdAVDec_Settings_t* pSettings)
{
    UINT32 u4VdpId;

    if ((pSettings->eVidFormat != SD_VIDEO_FORMAT_MPEG1) &&
        (pSettings->eVidFormat != SD_VIDEO_FORMAT_MPEG2) &&
        (pSettings->eVidFormat != SD_VIDEO_FORMAT_MPEG4) &&
        (pSettings->eVidFormat != SD_VIDEO_FORMAT_H264) &&
        (pSettings->eVidFormat != SD_VIDEO_FORMAT_DIVX) &&
        (pSettings->eVidFormat != SD_VIDEO_FORMAT_WMV3) &&
        (pSettings->eVidFormat != SD_VIDEO_FORMAT_XVID) &&
        (pSettings->eVidFormat != SD_VIDEO_FORMAT_MJPEG))
    {
        // Only MPEG1 and MPEG2 are supported.
        return FALSE;
    }

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return -1;
    }

#if 1
    u4VdpId = VDP_1;
    if ((sdal_avdec[eAVDecId].video_mod != NULL) &&
        (sdal_avdec[eAVDecId].video_mod->vpath_assigned) &&
        (sdal_avdec[eAVDecId].video_mod->out_id < VDP_NS))
    {
        u4VdpId = sdal_avdec[eAVDecId].video_mod->out_id;
    }
#else
    // Current Case
    u4VdpId = VDP_1;
#endif

    if (VDP_SetInput(u4VdpId, _arMpvInfo[eAVDecId].ucEsId, 0) != VDP_SET_OK)
    {
        return FALSE;
    }

    if (pSettings->bAVSyncOn)
    {
        if (_arMpvInfo[eAVDecId].ucStcId != 0)
        {
            return FALSE;
        }
        VDEC_SyncStc(_arMpvInfo[eAVDecId].ucEsId, VID_SYNC_MODE_SLAVE,
            _arMpvInfo[eAVDecId].ucStcId);
    }

    return TRUE;
 }

//-----------------------------------------------------------------------------
/** _AvDec_Init
 *  Handler of AVDec init command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Init(unsigned long arg)
{
	DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
    SdPowerState_k ePowerState;
    SdSound_Step_k eStep = SD_SOUND_STEP0;
    int i;

    ePowerState = (SdPowerState_k) arg;

    SdVPath_MutexLock();
     for(i=0;i<2;i++)
    {
        sdal_avdec[i].module_id=i;
        sdal_avdec[i].be_id=SD_BE_IN_AVDEC0+i;
        Sd_VdoPathDisconnect(0,0,SD_BE_IN_AVDEC0+i);
        sdal_avdec[i].fe_connected=0;
        sdal_avdec[i].fe_src=0;
        sdal_avdec[i].vpath_assigned=0;
        sdal_avdec[i].video_mod=0;
    }
    SdVPath_MutexUnLock();

    // Initialize demux
    if (!DMX_Init())
    {
        return -1;
    }

    // Initialize MPV
    VDEC_Init();

    // Initialize VDP
    VDP_Init();

    // Initialize AUD
    UNUSED(_SD_AUD_Init(eStep, ePowerState));

    // Set decoder callback functions
	rDecoderCallbacks.pfnSendVideoPes = _VideoCallback;
	rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
	rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;
	DMX_SetDecoderCallbacks(&rDecoderCallbacks);

    // Connect to decoder
	DMX_SetToDecoder(TRUE);

    // Register Call back function
    VDP_RegCbFunc(VDP_CB_FUNC_UNMUTE_IND, (UINT32)_AvDec_VdpUnMuteCb, 0, 0);

#ifdef CC_SDAL_DEBLOCK_OFF
    MPV_SetDbk(0, 0);
#else
    MPV_SetDbk(1, 0);
#endif

    // Reset local states
    _fgDemuxAllocated = FALSE;

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Dmx_Allocate
 *  Handler of AVDec demux allocate command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Dmx_Allocate(unsigned long arg)
{
    SDAL_IOCTL_8ARG_T rArg8;
    SdAVDec_DmxHandle *pDmxHndl;
    SdSource_k eSrc;
    SdAVDec_DemuxType_k eDemuxType;
    SdAVDec_DemuxOut_k eDemuxOut;
    SdBool_t bEnableRealloclc;
    SdAVDec_DmxHandle hDmx;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_8ARG_T)))
    {
        printk("_AvDec_Dmx_Allocate argument error\n");
        return -1;
    }

    // Copy user space argument
    if (copy_from_user(&rArg8, (void __user *)arg,
                       sizeof(SDAL_IOCTL_8ARG_T)))
    {
        printk("_AvDec_Dmx_Allocate argument error\n");
        return -1;
    }

    pDmxHndl = (SdAVDec_DmxHandle *) rArg8.au4Arg[0];
    eSrc = (SdSource_k) rArg8.au4Arg[1];
    eDemuxType = (SdAVDec_DemuxType_k) rArg8.au4Arg[2];
    eDemuxOut = (SdAVDec_DemuxType_k) rArg8.au4Arg[3];
    bEnableRealloclc = (SdBool_t) rArg8.au4Arg[4];

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pDmxHndl,
                   sizeof(SdAVDec_DmxHandle)))
    {
        printk("_AvDec_Dmx_Allocate argument error\n");
        return -1;
    }

    // Chech source
    if ((eSrc != SD_SOURCE_DTV1) && (eSrc != SD_SOURCE_DTV2))
    {
        // Source must be DTV
        return -1;
    }

    // Check type
    if (eDemuxType != SD_DEMUX_TYPE_TS)
    {
        // Support type TS only
        return -1;
    }

    // Check output destination
    if (eDemuxOut != SD_DEMUX_OUT_DECODER)
    {
        // Can output to decoder only
        return -1;
    }
/*
    // If already allocated
    if (_fgDemuxAllocated)
    {
        return -1;
    }
*/
    // Allocate demux handle
    hDmx = (SdAVDec_DmxHandle)DMX_HANDLE;

    if (copy_to_user((void __user *)pDmxHndl, &hDmx,
                     sizeof(SdAVDec_DmxHandle)))
    {
        printk("_AvDec_Dmx_Allocate argument error\n");
        return -1;
    }

    _fgDemuxAllocated = TRUE;

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Dmx_Deallocate
 *  Handler of AVDec demux deallocate command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Dmx_Deallocate(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    SdAVDec_DmxHandle dmxHndl;
    SdAVDec_DemuxOut_k eDemuxOut;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Dmx_Deallocate argument error\n");
        return -1;
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg,
                       sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Dmx_Deallocate argument error\n");
        return -1;
    }

    dmxHndl = (SdAVDec_DmxHandle) rArg2.au4Arg[0];
    eDemuxOut = (SdAVDec_DemuxType_k) rArg2.au4Arg[1];

    // Check output destination
    if (eDemuxOut != SD_DEMUX_OUT_DECODER)
    {
        // Can output to decoder only
        return -1;
    }

    // If not allocated
    if (!_fgDemuxAllocated)
    {
        return -1;
    }

    _fgDemuxAllocated = FALSE;

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Dmx_Start
 *  Handler of AVDec demux start command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Dmx_Start(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    SdAVDec_DmxHandle hDmx;
    SdAVDec_DemuxOut_k eDemuxOut;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Dmx_Start argument error\n");
        return -1;
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg,
                       sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Dmx_Start argument error\n");
        return -1;
    }

    hDmx = (SdAVDec_DmxHandle) rArg2.au4Arg[0];
    eDemuxOut = (SdAVDec_DemuxType_k) rArg2.au4Arg[1];

    // Check demux handle
    if (!_fgDemuxAllocated || (hDmx != (SdAVDec_DmxHandle)DMX_HANDLE))
    {
        // Incorrect demux handle
        return -1;
    }

    // Check output destination
    if (eDemuxOut != SD_DEMUX_OUT_DECODER)
    {
        // Can output to decoder only
        return -1;
    }

    // Start demux
    if (!DMX_Start())
    {
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Dmx_Stop
 *  Handler of AVDec demux stop command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Dmx_Stop(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    SdAVDec_DmxHandle hDmx;
    SdAVDec_DemuxOut_k eDemuxOut;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Dmx_Stop argument error\n");
        return -1;
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg,
                       sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Dmx_Stop argument error\n");
        return -1;
    }

    hDmx = (SdAVDec_DmxHandle) rArg2.au4Arg[0];
    eDemuxOut = (SdAVDec_DemuxType_k) rArg2.au4Arg[1];

    // Check demux handle
    if (!_fgDemuxAllocated || (hDmx != (SdAVDec_DmxHandle)DMX_HANDLE))
    {
        // Incorrect demux handle
        return -1;
    }

    // Check output destination
    if (eDemuxOut != SD_DEMUX_OUT_DECODER)
    {
        // Can output to decoder only
        return -1;
    }

    // Stop demux
    if (!DMX_Stop())
    {
        return -1;
    }

    // Reset PIDs
    if (!_DemuxReset())
    {
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Dmx_SetPid
 *  Handler of AVDec demux set pid command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Dmx_SetPid(unsigned long arg)
{
    // Currently, demux doesn't support outputting to 1394/PVR
    return -1;

#if 0

    SDAL_IOCTL_8ARG_T rArg8;
    SdAVDec_DmxHandle dmxHndl;
    int32_t *puwPids;
    uint32_t uwNumPids;
    SdAVDec_DemuxOut_k eDemuxOut;
    SdBool_t bCaOn;

    int32_t auwPids[8];

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_8ARG_T)))
    {
        printk("_AvDec_Dmx_SetPid argument error\n");
        return -1;
    }

    // Copy user space argument
    if (copy_from_user(&rArg8, (void __user *)arg,
                       sizeof(SDAL_IOCTL_8ARG_T)))
    {
        printk("_AvDec_Dmx_SetPid argument error\n");
        return -1;
    }

    dmxHndl = (SdAVDec_DmxHandle) rArg8.au4Arg[0];
    puwPids = (int32_t *) rArg8.au4Arg[1];
    uwNumPids = (uint32_t) rArg8.au4Arg[2];
    eDemuxOut = (SdAVDec_DemuxOut_k) rArg8.au4Arg[3];
    bCaOn = (SdBool_t) rArg8.au4Arg[4];

    //// TODO DMX, check
    if (uwNumPids >= 8)
    {
        return -1;
    }

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)puwPids,
                   (sizeof(int32_t) * uwNumPids)))
    {
        printk("_AvDec_Dmx_SetPid argument error\n");
        return -1;
    }

    // Copy user space argument
    if (copy_from_user(auwPids, (void __user *)puwPids,
                       (sizeof(int32_t) * uwNumPids)))
    {
        printk("_AvDec_Dmx_SetPid argument error\n");
        return -1;
    }

    //// TODO DMX (handle pid array inside auwPids)

    return 0;
#endif  // 0
}

//-----------------------------------------------------------------------------
/** _AvDec_Dec_Connect
 *  Handler of AVDec decoder connect command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Dec_Connect(unsigned long arg)
{
    SDAL_IOCTL_4ARG_T rArg4;
    SdAVDecID_k eAVDecId;
    SdAVDec_DmxHandle hDmx;
    SdVideoID_k eOutVideoId;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_4ARG_T)))
    {
        printk("_AvDec_Dec_Connect argument error\n");
        return SD_ERR_INV;
    }

    // Copy user space argument
    if (copy_from_user(&rArg4, (void __user *)arg,
                       sizeof(SDAL_IOCTL_4ARG_T)))
    {
        printk("_AvDec_Dec_Connect argument error\n");
        return SD_ERR_INV;
    }

    eAVDecId = (SdAVDecID_k) rArg4.au4Arg[0];
    hDmx = (SdAVDec_DmxHandle) rArg4.au4Arg[1];
    eOutVideoId = (SdVideoID_k) rArg4.au4Arg[2];

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }

#if 0
    // Check demux handle
    if (((!_fgDemuxAllocated) || (hDmx != (SdAVDec_DmxHandle)DMX_HANDLE)))
    {
        // Incorrect demux handle
        return -1;
    }
#endif

#ifndef SUPPORT_AUXOUT
    // Nothing to do here for demux
    if ( sdal_avdec[eAVDecId].fe_connected)
    	return SD_NOT_OK;
#endif    

    SdVPath_MutexLock();
    Sd_VdoPathConnect(0, eOutVideoId, eAVDecId);
        // fail to Connect B2R to VDP
        SdVPath_MutexUnLock();

    //// TODO AUD

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Dec_Connect2
 *  Handler of AVDec decoder connect command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Dec_Connect2(unsigned long arg)
{
    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Dec_Disconnect
 *  Handler of AVDec decoder disconnect command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Dec_Disconnect(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    SdAVDecID_k eAVDecId;
    SdAVDec_DisconnectOpt_k eOption;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Dec_Disconnect argument error\n");
        return SD_ERR_INV;
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg,
                       sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Dec_Disconnect argument error\n");
        return SD_ERR_INV;
    }

    eAVDecId = (SdAVDecID_k) rArg2.au4Arg[0];
    eOption = (SdAVDec_DisconnectOpt_k) rArg2.au4Arg[1];

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }

    if (!_DemuxReset())
    {
        // Reset fail
        return -1;
    }

    if(!sdal_avdec[eAVDecId].fe_connected)
    	return SD_NOT_OK;
    SdVPath_MutexLock();
    Sd_VdoPathDisconnect(0, 0, eAVDecId);
    SdVPath_MutexUnLock();

    //// TODO MPV
    //// TODO AUD

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Vdo_Start
 *  Handler of AVDec video start command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Vdo_Start(unsigned long arg)
{
    SdAVDecID_k eAVDecId;
    SdBool_t bRet;

    eAVDecId = (SdAVDecID_k) arg;

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }

    bRet = TRUE;
    switch(rAvDecSettings.eVidFormat)
    {
        case SD_VIDEO_FORMAT_DIVX:
        case SD_VIDEO_FORMAT_XVID:
            bRet = (SdBool_t)VDEC_Play(0, VDEC_FMT_MP4);
            DMX_SetVideoType(DMX_VIDEO_MPEG4);
            break;
        case SD_VIDEO_FORMAT_MPEG4:
        	bRet = (SdBool_t)VDEC_Play(0, VDEC_FMT_MP4);
        	DMX_SetVideoType(DMX_VIDEO_MPEG4);
        	break;
        case SD_VIDEO_FORMAT_MPEG1:
        case SD_VIDEO_FORMAT_MPEG2:
        	bRet = (SdBool_t)VDEC_Play(0, VDEC_FMT_MPV);
        	DMX_SetVideoType(DMX_VIDEO_MPEG);
        	break;
        case SD_VIDEO_FORMAT_H264:
        	bRet = (SdBool_t)VDEC_Play(0, VDEC_FMT_H264);
        	DMX_SetVideoType(DMX_VIDEO_H264);
        	break;
        case SD_VIDEO_FORMAT_WMV3:
            bRet = (SdBool_t)VDEC_Play(0,VDEC_FMT_WMV);
            DMX_SetVideoType(DMX_VIDEO_VC1);
      break;
        default:
        	return SD_NOT_SUPPORTED;
    }

    if(!bRet)
    {
        return SD_NOT_SUPPORTED;
    }
    
    return SD_OK;
}

//-----------------------------------------------------------------------------
/** _AvDec_Vdo_Stop
 *  Handler of AVDec video stop command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Vdo_Stop(unsigned long arg)
{
    SdAVDecID_k eAVDecId;

    eAVDecId = (SdAVDecID_k) arg;

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }
    
    VDEC_Stop(_arMpvInfo[eAVDecId].ucEsId);

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Aud_Start
 *  Handler of AVDec audio start command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Aud_Start(unsigned long arg)
{
    SdAVDecID_k eAVDecId;

    eAVDecId = (SdAVDecID_k) arg;

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return -1;
    }
        
    _SD_AUD_DtvPlay(SOUND_ID_TO_DEC_ID(eAVDecId));

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Aud_Stop
 *  Handler of AVDec audio stop command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Aud_Stop(unsigned long arg)
{
    SdAVDecID_k eAVDecId;

    eAVDecId = (SdAVDecID_k) arg;

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return -1;
    }
        
    _SD_AUD_DtvStop(SOUND_ID_TO_DEC_ID(eAVDecId));

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Vdo_Pause
 *  Handler of AVDec video pause command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Vdo_Pause(unsigned long arg)
{
    SdAVDecID_k eAVDecId;

    eAVDecId = (SdAVDecID_k) arg;

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }
    
    VDEC_Pause(_arMpvInfo[eAVDecId].ucEsId);

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Vdo_Resume
 *  Handler of AVDec video resume command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Vdo_Resume(unsigned long arg)
{
    SdAVDecID_k eAVDecId;
    SdBool_t bRet;

    eAVDecId = (SdAVDecID_k) arg;

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }

    bRet = TRUE;
    switch(rAvDecSettings.eVidFormat)
    {
        case SD_VIDEO_FORMAT_DIVX:
        case SD_VIDEO_FORMAT_XVID:
            bRet = (SdBool_t)VDEC_Play(0, VDEC_FMT_MP4);
            break;
        case SD_VIDEO_FORMAT_MPEG4:
        	bRet = (SdBool_t)VDEC_Play(0, VDEC_FMT_MP4);
        	break;
        case SD_VIDEO_FORMAT_MPEG1:
        case SD_VIDEO_FORMAT_MPEG2:
        	bRet = (SdBool_t)VDEC_Play(0, VDEC_FMT_MPV);
        	break;
        case SD_VIDEO_FORMAT_H264:
        	bRet = (SdBool_t)VDEC_Play(0, VDEC_FMT_H264);
        	break;
        case SD_VIDEO_FORMAT_WMV3:
            bRet = (SdBool_t)VDEC_Play(0,VDEC_FMT_WMV);
      break;
        default:
        	return SD_NOT_SUPPORTED;
    }

    if(!bRet)
    {
        return SD_NOT_SUPPORTED;
    }
    
    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Vdo_SinglePlay
 *  Handler of AVDec video single play command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Vdo_SinglePlay(unsigned long arg)
{
    SdAVDecID_k eAVDecId;

    eAVDecId = (SdAVDecID_k) arg;

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }

    //// TODO MPV

    return -1;
}

//-----------------------------------------------------------------------------
/** _AvDec_Aud_Pause
 *  Handler of AVDec audio pause command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Aud_Pause(unsigned long arg)
{
    SdAVDecID_k eAVDecId;

    eAVDecId = (SdAVDecID_k) arg;

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return -1;
    }
    
    AUD_DSPCmdPause(SOUND_ID_TO_DEC_ID(eAVDecId));

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Aud_Resume
 *  Handler of AVDec audio resume command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Aud_Resume(unsigned long arg)
{
    SdAVDecID_k eAVDecId;

    eAVDecId = (SdAVDecID_k) arg;

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return -1;
    }
    
    AUD_DSPCmdResume(SOUND_ID_TO_DEC_ID(eAVDecId));

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Get
 *  Handler of AVDec get command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Get(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    SdAVDecID_k eAVDecId;
    SdAVDec_Settings_t *pSettings;

    SdAVDec_Settings_t Settings;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Get argument error\n");
        return SD_ERR_INV;
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg,
                       sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Get argument error\n");
        return SD_ERR_INV;
    }

    eAVDecId = (SdAVDecID_k) rArg2.au4Arg[0];
    pSettings = (SdAVDec_Settings_t *) rArg2.au4Arg[1];

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pSettings,
                   sizeof(SdAVDec_Settings_t)))
    {
        printk("_AvDec_Get argument error\n");
        return SD_ERR_INV;
    }

    // Simply restore settings from backup
    Settings = rAvDecSettings;

#if 0
    Settings.eVidFormat =
        (SdVideoFormat_k)MPV_GetVideoFormat(_arMpvInfo[eAVDecId].ucEsId);
#endif

    // Support normal playback only
    Settings.sPlaySettings.Speed = 0;
    Settings.sPlaySettings.ePlaymode = SD_AVDEC_PLAYMODE_NORMAL;
    Settings.sPlaySettings.bReversePlay = 0;

    // Fill audio relative info
    if (Settings.bAdOn)
    {
        Settings.sPCMSettings.eFsRate = SD_AUDIO_FS_48KHZ;
        Settings.sPCMSettings.bitWidth = 24;
        Settings.sPCMSettings.channels = 2;
    }
    else  // ?
    {
        Settings.sPCMSettings.eFsRate = SD_AUDIO_FS_48KHZ;
        Settings.sPCMSettings.bitWidth = 24;
        Settings.sPCMSettings.channels = 2;
    }

    if (copy_to_user((void __user *)pSettings, &Settings,
                     sizeof(SdAVDec_Settings_t)))
    {
        printk("_AvDec_Get argument error\n");
        return SD_ERR_INV;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_Set
 *  Handler of AVDec set command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Set(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    SdAVDecID_k eAVDecId;
    SdAVDec_Settings_t *pSettings;
    SdAVDec_Settings_t Settings;
    INT32 i4Ret = 0;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Set argument error\n");
        return SD_ERR_INV;
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg,
                       sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_AvDec_Set argument error\n");
        return SD_ERR_INV;
    }

    eAVDecId = (SdAVDecID_k) rArg2.au4Arg[0];
    pSettings = (SdAVDec_Settings_t *) rArg2.au4Arg[1];

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)pSettings,
                   sizeof(SdAVDec_Settings_t)))
    {
        printk("_AvDec_Set argument error\n");
        return SD_ERR_INV;
    }

    // Copy user space argument
    if (copy_from_user(&Settings, (void __user *)pSettings,
                       sizeof(SdAVDec_Settings_t)))
    {
        printk("_AvDec_Set argument error\n");
        return SD_ERR_INV;
    }

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        printk("_AvDec_Set error #1\n");
        return SD_NOT_SUPPORTED;
    }

    // ToDo: add it when feeder is ready
    //if (FeederSetDecodeFmt((UINT32)Settings.eVidFormat, (UINT32)Settings.eAudFormat) != FEEDER_E_OK)
    {
    }


    if (Settings.eVidFormat != SD_VIDEO_FORMAT_NONE)
    {
        if (!_DemuxSet((UINT8)eAVDecId, &Settings) )
        {
            // Fail to set demux
            printk("_AvDec_Set error #2\n");
            i4Ret = -1;
        }

        if (!_VdecSet(eAVDecId, &Settings))
        {
            // Fail to set vdec
            printk("_AvDec_Set error #3\n");
            i4Ret = SD_PARAMETER_ERROR;
        }
    }
    
    if (!_AudSet(eAVDecId, &Settings))
    {
        // Fail to set audio
        printk("_AvDec_Set error #4\n");
        i4Ret = -1;
    }

    // Backup decoder settings
    rAvDecSettings = Settings;

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _AvDec_GetStatus
 *  Handler of AVDec get status command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_GetStatus(unsigned long arg)
{
    SDAL_IOCTL_4ARG_T rArg4;
    SdAVDecID_k eAVDecId;
    SdMask_t mask;
    SdAVDec_Status_t *pState;
    uint32_t timeout;
    VDEC_HDR_INFO_T rVdecHdrInfo;
    static uint32_t u4FailCntLast = 0;

    SdAVDec_Status_t State;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_4ARG_T)))
    {
        printk("_AvDec_GetStatus argument error\n");
        return SD_ERR_INV;
    }

    // Copy user space argument
    if (copy_from_user(&rArg4, (void __user *)arg,
                       sizeof(SDAL_IOCTL_4ARG_T)))
    {
        printk("_AvDec_GetStatus argument error\n");
        return SD_ERR_INV;
    }

    eAVDecId = (SdAVDecID_k) rArg4.au4Arg[0];
    mask = (SdMask_t) rArg4.au4Arg[1];
    pState = (SdAVDec_Status_t *) rArg4.au4Arg[2];
    timeout = (uint32_t) rArg4.au4Arg[3];

    // Check decoder ID
    if (eAVDecId != SD_AVDEC0)
    {
        // Only SD_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pState,
                   sizeof(SdAVDec_Status_t)))
    {
        printk("_AvDec_GetStatus argument error\n");
        return SD_ERR_INV;
    }

    // Zero all states
    x_memset(&State, 0, sizeof (State));

    // Check decoder id
    if (eAVDecId != SD_AVDEC0)
    {
        return SD_NOT_SUPPORTED;
    }

    if ((mask & SD_AVDEC_STATUS_TSDLOCK) != 0)
    {
        State.bTSDLock = DMX_GetLockState();
    }

    if ((mask & SD_AVDEC_STATUS_VIDEOLOCK) != 0)
    {
        State.bVideoLock = (SdBool_t)VDEC_IsLock(_arMpvInfo[eAVDecId].ucEsId);
    }

    if ((mask & SD_AVDEC_STATUS_VIDEOINFO) != 0)
    {
        State.sVideoInfo.hResolution = 0;
        State.sVideoInfo.vResolution = 0;
        State.sVideoInfo.hStart = 0;
        State.sVideoInfo.vStart = 0;
        State.sVideoInfo.hTotal = 0;
        State.sVideoInfo.vTotal = 0;
        State.sVideoInfo.bProgressScan = 0;
        State.sVideoInfo.frameRate = 0;
        State.sVideoInfo.eRatio = SD_ASPECT_RATIO_NONE;
        State.sVideoInfo.eMatrix_Coefficients = SD_MATRIX_COEFF_UNKNOWN;
        

        /* old protocol
        MPV_GetVideoInfo(_arMpvInfo[eAVDecId].ucEsId,
            &(State.sVideoInfo.hResolution),
            &(State.sVideoInfo.vResolution),
            &(State.sVideoInfo.bProgressScan),
            (MPV_ALL_ASPECT_RATIO_T*)&(State.sVideoInfo.eRatio),
            &(State.sVideoInfo.frameRate));
        */

        x_memset(&rVdecHdrInfo, 0, sizeof (rVdecHdrInfo));
        if(VDEC_QueryInfo(_arMpvInfo[eAVDecId].ucEsId,
            &rVdecHdrInfo))
        {
            State.sVideoInfo.hResolution = rVdecHdrInfo.u2Width;
            State.sVideoInfo.vResolution = rVdecHdrInfo.u2Height;
            State.sVideoInfo.bProgressScan = rVdecHdrInfo.fgProgressiveSeq;
            State.sVideoInfo.eRatio = (SdAVDec_AspectRatio_k)rVdecHdrInfo.eAspectRatio;
            State.sVideoInfo.frameRate = rVdecHdrInfo.u2FrmRate;
            if(rVdecHdrInfo.ucMatrixCoefficients == 1) // 13818-2, table 6-9
            {
                State.sVideoInfo.eMatrix_Coefficients = SD_MATRIX_COEFF_BT709;
            }
            else if(rVdecHdrInfo.ucMatrixCoefficients == 4)
            {
                State.sVideoInfo.eMatrix_Coefficients = SD_MATRIX_COEFF_FCC;
            }
            else if(rVdecHdrInfo.ucMatrixCoefficients == 5)
            {
                State.sVideoInfo.eMatrix_Coefficients = SD_MATRIX_COEFF_BT470_2;
            }
            else if(rVdecHdrInfo.ucMatrixCoefficients == 6)
            {
                State.sVideoInfo.eMatrix_Coefficients = SD_MATRIX_COEFF_SMPTE170M;
            }
            else if(rVdecHdrInfo.ucMatrixCoefficients == 7)
            {
                State.sVideoInfo.eMatrix_Coefficients = SD_MATRIX_COEFF_SMPTE240M;
            }
            else
            {
                State.sVideoInfo.eMatrix_Coefficients = SD_MATRIX_COEFF_UNKNOWN;
            }
        }
    }
    
    if ((mask & SD_AVDEC_STATUS_AUDIOINFO) != 0)
    {

        AUD_DEC_INFO_T  rAudInfo;
        // SDAL FixMe
        AUD_GetDtvAudInfo(eAVDecId, &rAudInfo); 
        
        switch (rAudInfo.e_aud_type)
        {
        case AUD_TYPE_STEREO:
        	State.sAudioInfo.eAudioChMode = SD_AUD_CH_MODE_STEREO;
        	break;
        case AUD_TYPE_JOINT_STEREO:
        	State.sAudioInfo.eAudioChMode = SD_AUD_CH_MODE_JOINT_STEREO;
        	break;
        case AUD_TYPE_DUAL_MONO:
        	State.sAudioInfo.eAudioChMode = SD_AUD_CH_MODE_DUAL;
        	break;
        case AUD_TYPE_MONO:
        	State.sAudioInfo.eAudioChMode = SD_AUD_CH_MODE_SINGLE;
        	break;
        default:
            State.sAudioInfo.eAudioChMode = SD_AUD_CH_MODE_STEREO;
        	break;
        }	

        // mpeg decoder only returns these three values
        switch (rAudInfo.ui4_sample_rate)
        {
        case 44100:
            State.sAudioInfo.eAudioFsRate = SD_AUDIO_FS_44KHZ;
        	break;
        case 48000:
        	State.sAudioInfo.eAudioFsRate = SD_AUDIO_FS_48KHZ;
        	break;
        case 32000:
        	State.sAudioInfo.eAudioFsRate = SD_AUDIO_FS_32KHZ;
        	break;
        default:
            State.sAudioInfo.eAudioFsRate= SD_AUDIO_FS_44KHZ;        	
        	break;
        }	    
    }
    
    if ((mask & SD_AVDEC_STATUS_VIDEOUNDERFLOW) != 0)
    {
        State.bDecoderUnderflow = 1;
        {
            DMX_PID_T rPid;
            UINT32 u4DataSize;
            DMX_GetPid(_u1VideoPidx, DMX_PID_FLAG_ALL, &rPid);
            if (rPid.fgEnable)
            {
                u4DataSize = DATASIZE(rPid.u4Rp, rPid.u4Wp, rPid.u4BufSize);
                if(u4DataSize < rAvDecSettings.videoUnderflowLevel)
                {
                    State.bDecoderUnderflow = 0;
                }
            }
        }
    }
    
    if ((mask & SD_AVDEC_STATUS_SUPPORTTRICK) != 0)
    {
        // SDAL FixMe
        State.bSupportTrickmode = 0;
    }
    
    if ((mask & SD_AVDEC_STATUS_ERROR) != 0)
    {
        State.eAVDecError = SD_AVDEC_ERROR_NONE;
        x_memset(&rVdecHdrInfo, 0, sizeof (rVdecHdrInfo));
        rVdecHdrInfo.u4FailCnt = u4FailCntLast;
        VDEC_QueryInfo(_arMpvInfo[eAVDecId].ucEsId, &rVdecHdrInfo);
        {
            if(rVdecHdrInfo.eNotifyStatus == VDEC_DEC_DECODE_UNKNOWN ||
                rVdecHdrInfo.eNotifyStatus == VDEC_DEC_DECODE_NO_DATA ||
                rVdecHdrInfo.eNotifyStatus == VDEC_DEC_DECODE_ERROR)
            {
                State.eAVDecError = SD_AVDEC_ERROR_NOT_SUPPORTED_VIDEO;
            }
            else if(rVdecHdrInfo.eNotifyStatus == VDEC_DEC_DECODE_HD_NOT_SUPPORT)
            {
                State.eAVDecError = SD_AVDEC_ERROR_NOT_SUPPORTED_VIDEO_RESOLUTION;
            }
            else // ToDo: check FRAMERATE and PROFILE in case we meet such kind of clip
            {
                State.eAVDecError = SD_AVDEC_ERROR_NONE;
            }
            // when we have error, change eAVDecError
            if(rVdecHdrInfo.u4FailCnt != u4FailCntLast)
            {
                State.eAVDecError = SD_AVDEC_ERROR_NOT_SUPPORTED_VIDEO;
                u4FailCntLast = rVdecHdrInfo.u4FailCnt;
            }
        }
        if(rAvDecSettings.eAudFormat >= SD_AUDIO_FORMAT_MAX ||
            rAvDecSettings.eAudFormat <= SD_AUDIO_FORMAT_NONE)
        {
            State.eAVDecError = SD_AVDEC_ERROR_NOT_SUPPORTED_AUDIO;
        }
    }

    if ((mask & SD_AVDEC_STATUS_UNMUTE) != 0)
    {
        UINT32 u4VdpId;

#if 1
        u4VdpId = VDP_1;
        if ((sdal_avdec[eAVDecId].video_mod != NULL) &&
            (sdal_avdec[eAVDecId].video_mod->vpath_assigned) &&
            (sdal_avdec[eAVDecId].video_mod->out_id < VDP_NS))
        {
            u4VdpId = sdal_avdec[eAVDecId].video_mod->out_id;
        }
#else
        // Current Case
        u4VdpId = VDP_1;
#endif

        State.bUnmuteEnable = (SdBool_t)VDP_GetUnMuteStatus(u4VdpId);
    }

    if ((mask & SD_AVDEC_STATUS_AUDIOLOCK) != 0)
    {
        State.bAudioLock = AUD_DtvLock(eAVDecId);
    }

    if ((mask & SD_AVDEC_STATUS_STC) != 0)
    {
        UINT8 u1StcId;

        u1StcId = _GetStcId(0);

        if (u1StcId < STC_SRC_NS)
        {
            STC_CLOCK rStc;
            uint64_t u8Temp;

            if (STC_GetSrc(u1StcId, &rStc) == STC_VALID)
            {
            }

            // 0~32 = Base, 33 ~ 41 = Extension.

            u8Temp = (uint64_t)rStc.u2Ext;              // Externsion
            u8Temp <<= 1;
            u8Temp += (uint64_t)(rStc.ucBaseHi & 1);    // Bit32 of Base
            u8Temp <<= 32;
            u8Temp += (uint64_t)rStc.u4Base;            // Bit0~31 of Base

            State.stc = u8Temp;
        }
        else
        {
            State.stc = 0;
        }
    }

    if (copy_to_user((void __user *)pState, &State,
                     sizeof(SdAVDec_Status_t)))
    {
        printk("_AvDec_GetStatus argument error\n");
        return SD_ERR_INV;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** _AvDec_InstallCb
 *  Handler of AVDec install call back command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_InstallCb(unsigned long arg)
{
    SDAL_IOCTL_8ARG_T rArg8;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_8ARG_T)))
    {
        printk("_AvDec_InstallCb argument error\n");
        return SD_ERR_INV;
    }

    // Copy user space argument
    if (copy_from_user(&rArg8, (void __user *)arg,
                       sizeof(SDAL_IOCTL_8ARG_T)))
    {
        printk("_AvDec_InstallCb argument error\n");
        return SD_ERR_INV;
    }

    _eCbAVDecId = (SdAVDecID_k) rArg8.au4Arg[0];
    _pCbCallback = (SdAVDec_CallBack) rArg8.au4Arg[1];
    _u4CbParam = (uint32_t) rArg8.au4Arg[2];
    _rCbMask = (SdMask_t) rArg8.au4Arg[3];
    _bCbOneShot = (SdBool_t) rArg8.au4Arg[4];
    _rCbShotMask = (SdMask_t) 0;

    MPV_RegAVDecCb(_arMpvInfo[_eCbAVDecId].ucEsId,
        (PFN_MPV_AVDEC_CB)Sd_AvDec_CallBack);

    return SD_OK;
}

//-----------------------------------------------------------------------------
/** _AvDec_Tsout_Start
 *  Handler of AVDec ts start command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Tsout_Start(unsigned long arg)
{
    // No 1394 out support currently
    return -1;
}

//-----------------------------------------------------------------------------
/** _AvDec_Tsout_Stop
 *  Handler of AVDec ts stop command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static INT32 _AvDec_Tsout_Stop(unsigned long arg)
{
    // No 1394 out support currently
    return -1;
}

//-----------------------------------------------------------------------------
/** _AvDec_VdpUnMuteCb
 *  Call back function of unmute status
 */
//-----------------------------------------------------------------------------
static void _AvDec_VdpUnMuteCb(UINT32 u4VdpId, BOOL fgUnMute)
{
    SdAVDec_Status_t rStatus;

    if ((sdal_avdec[SD_AVDEC0].video_mod != NULL) &&
        (sdal_avdec[SD_AVDEC0].video_mod->vpath_assigned) &&
        (sdal_avdec[SD_AVDEC0].video_mod->out_id == (SdWindowID_k) u4VdpId))
    {
        rStatus.bUnmuteEnable = fgUnMute;

        Sd_AvDec_CallBack(&rStatus, SD_AVDEC_STATUS_UNMUTE);

//        printk("_AvDec_VdpUnMuteCb %d %d\n",u4VdpId, fgUnMute);
    }
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _AvDec_CallBack
 *  Handler of call back command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------

void Sd_AvDec_CallBack(SdAVDec_Status_t *pStatus, SdAVDec_StatusMask_k eMask)
{
    SDAL_AVDEC_CB_T rArg;

    if (_pCbCallback == 0)
    {
        // call back not register
        return;
    }

    if ((((SdMask_t)eMask) & _rCbMask) == 0)
    {
        // no use event
        return;
    }

    if (_bCbOneShot)
    {
        if ((((SdMask_t)eMask) & _rCbShotMask) != 0)
        {
            // specify event happen already
            return;
        }

        _rCbShotMask |= (SdMask_t) eMask;
    }

    rArg.u4AVDecId = (unsigned int) SD_AVDEC0;
    rArg.u4Mask = (unsigned int) eMask;
    rArg.u4Param = (unsigned int) _u4CbParam;

    rArg.u4bTSDLock = (unsigned int) pStatus->bTSDLock;
    rArg.u4bAudioLock = (unsigned int) pStatus->bAudioLock;
    rArg.u4bVideoLock = (unsigned int) pStatus->bVideoLock;
    rArg.u4bUnmuteEnable = (unsigned int) pStatus->bUnmuteEnable;

//    rArg.u4eHDMIAudioFormat = (unsigned int) pStatus->eHDMIAudioFormat;

    rArg.u4hResolution = (unsigned int) pStatus->sVideoInfo.hResolution;
    rArg.u4vResolution = (unsigned int) pStatus->sVideoInfo.vResolution;
    rArg.u4hStart = (unsigned int) pStatus->sVideoInfo.hStart;
    rArg.u4vStart = (unsigned int) pStatus->sVideoInfo.vStart;
    rArg.u4hTotal = (unsigned int) pStatus->sVideoInfo.hTotal;
    rArg.u4vTotal = (unsigned int) pStatus->sVideoInfo.vTotal;
    rArg.u4bProgressScan = (unsigned int) pStatus->sVideoInfo.bProgressScan;
    rArg.u4eRatio = (unsigned int) pStatus->sVideoInfo.eRatio;
    rArg.u4frameRate = (unsigned int) pStatus->sVideoInfo.frameRate;

    UNUSED(_CB_PutEvent(CB_SDAL_AVDEC_TRIGGER, sizeof (SDAL_AVDEC_CB_T), &rArg));
}


//-----------------------------------------------------------------------------
/** sdal_ioctl_sdavdec
 *  Command dispatcher
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------

int sdal_ioctl_sdavdec(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = 0;

	switch (cmd) {
        case SDAL_IO_AVDEC_INIT:
            i4Ret = _AvDec_Init(arg);
			break;

        case SDAL_IO_AVDEC_DMX_ALLOCATE:
            i4Ret = _AvDec_Dmx_Allocate(arg);
			break;

        case SDAL_IO_AVDEC_DMX_DEALLOCATE:
            i4Ret = _AvDec_Dmx_Deallocate(arg);
			break;

        case SDAL_IO_AVDEC_DMX_START:
            i4Ret = _AvDec_Dmx_Start(arg);
			break;

        case SDAL_IO_AVDEC_DMX_STOP:
            i4Ret = _AvDec_Dmx_Stop(arg);
			break;

        case SDAL_IO_AVDEC_DMX_SET_PID:
            i4Ret = _AvDec_Dmx_SetPid(arg);
			break;

        case SDAL_IO_AVDEC_DEC_CONNECT:
            i4Ret = _AvDec_Dec_Connect(arg);
			break;

        case SDAL_IO_AVDEC_DEC_CONNECT2:
            i4Ret = _AvDec_Dec_Connect2(arg);
			break;

        case SDAL_IO_AVDEC_DEC_DISCONNECT:
            i4Ret = _AvDec_Dec_Disconnect(arg);
			break;

        case SDAL_IO_AVDEC_VDO_START:
            i4Ret = _AvDec_Vdo_Start(arg);
			break;

        case SDAL_IO_AVDEC_VDO_STOP:
            i4Ret = _AvDec_Vdo_Stop(arg);
			break;

        case SDAL_IO_AVDEC_AUD_START:
            i4Ret = _AvDec_Aud_Start(arg);
			break;

        case SDAL_IO_AVDEC_AUD_STOP:
            i4Ret = _AvDec_Aud_Stop(arg);
			break;

        case SDAL_IO_AVDEC_VDO_PAUSE:
            i4Ret = _AvDec_Vdo_Pause(arg);
			break;

        case SDAL_IO_AVDEC_VDO_RESUME:
            i4Ret = _AvDec_Vdo_Resume(arg);
			break;

        case SDAL_IO_AVDEC_VDO_SINGLE_PLAY:
            i4Ret = _AvDec_Vdo_SinglePlay(arg);
			break;

        case SDAL_IO_AVDEC_AUD_PAUSE:
            i4Ret = _AvDec_Aud_Pause(arg);
			break;

        case SDAL_IO_AVDEC_AUD_RESUME:
            i4Ret = _AvDec_Aud_Resume(arg);
			break;

        case SDAL_IO_AVDEC_GET:
            i4Ret = _AvDec_Get(arg);
			break;

        case SDAL_IO_AVDEC_SET:
            i4Ret = _AvDec_Set(arg);
			break;

        case SDAL_IO_AVDEC_GET_STATUS:
            i4Ret = _AvDec_GetStatus(arg);
			break;

        case SDAL_IO_AVDEC_INSTALL_CB:
            i4Ret = _AvDec_InstallCb(arg);
			break;

        case SDAL_IO_AVDEC_TSOUT_START:
            i4Ret = _AvDec_Tsout_Start(arg);
			break;

        case SDAL_IO_AVDEC_TSOUT_STOP:
            i4Ret = _AvDec_Tsout_Stop(arg);
			break;

		default:
            i4Ret = SD_ERR_INV;
    		break;
	}
	return i4Ret;
}
