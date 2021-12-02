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
 * $RCSfile: b2r_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_if.c
 *  Video Plane driver - public interface
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "b2r_drvif.h"
#include "b2r_if.h"
#include "fbm_drvif.h"
#include "vdp_if.h"
#include "vdp_drvif.h"
#include "drv_scaler.h"
#include "vdp_vsync.h"
#include "vdp_frc.h"
#include "vdp_hal.h"
#include "vdp_image.h"
#include "vdp_debug.h"
#include "drv_video.h"
#include "nptv_if.h"
#include "vdp_display.h"
#include "mute_if.h"
#include "video_def.h"
#include "sv_const.h"
#include "vdec_drvif.h"

#ifdef CC_SRM_ON
#include "srm_drvif.h"
#endif

#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif

#ifdef CC_FAST_INIT
#include "x_bim.h"
#endif

#include "x_lint.h"
#include "x_assert.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#ifndef CC_CLI
#define CC_CLI
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#ifdef CC_SCPOS_EN
EXTERN void vMpegInit(void);
EXTERN void vMpegInitB2rConf(void);
EXTERN void vMpegModeChg(UCHAR ucVdpId);
EXTERN void vMpegModeDetDone(UCHAR ucVdpId);
EXTERN void vMpegModeChg(UCHAR ucVdpId);
#endif

EXTERN void _VDPSetChkPTS(UINT32 u4Pts);
EXTERN void _VDP_SetEncTrigger(BOOL fgEnable);
EXTERN BOOL _VDP_SendB2RChgFrameMsg(VDP_B2R_CHG_FRAME_MSG_T* prMsg);
EXTERN BOOL _VDP_FlushB2RChgFrameMsg(VOID);


typedef void (*VDP_GET_PIC_TYPE_CB_FUNC)(VDEC_PICTYPE_CB_T* prPicType);

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

extern VDP_CB_FUNC_T _rVdpCbFunc; /* To include VDP callback function */

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifdef CC_53XX_SWDMX_V2
UCHAR _arEs2Fbg[MAX_ES_NS];
#else
static UCHAR _arEs2Fbg[MAX_ES_NS];
#endif
static VDP_CONF_T* _prVdpConf[VDP_NS];
static VDP_PRM_T*    _prVdpPrm[VDP_NS];
static UCHAR _aucImageConnected[VDP_NS];

/* Digital decoder variable-dram-size notification */
static HANDLE_T _hB2RVdecConfMutex = (HANDLE_T)(NULL);

static UCHAR _ucThumbnailMode; // Thumbnail mode in MM mode

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define VERIFY_VDP_ID(id)                                   \
          do {                                              \
            if (id >= VDP_NS) { return VDP_SET_ERROR; }     \
          } while (0)

#define VDP_MUTEX_LOCK                  (VERIFY(x_sema_lock(VDP_GetMutex(), X_SEMA_OPTION_WAIT) == OSR_OK))

#define VDP_MUTEX_UNLOCK                (VERIFY(x_sema_unlock(VDP_GetMutex()) == OSR_OK))

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _B2RSrmVDecMemConfCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    UNUSED(u4Arg1);
    UNUSED(u4Arg2);
    UNUSED(u4Arg3);
    UNUSED(u4Arg4);
    LOG(9, "[SRM] Callback _B2RSrmVDecMemConfCb\n");
    VERIFY (x_sema_unlock(_hB2RVdecConfMutex) == OSR_OK);
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

/**
 * VDP DTV frame buffer group ready check
 *
 * @param void
 * @return void
 */
#ifdef CC_53XX_SWDMX_V2
void _VdpCheckFbgReady(UCHAR ucFbgId, UCHAR ucEsId)
{
    UINT32 u4EsId = (UINT32)ucEsId;
    UINT32 u4FbgNs;
    //UINT32 u4PortIdx;
    UINT32 u4FbgIdx;
    UINT32 u4VdpId;
    UINT32 u4DisplayReady =0;

    UNUSED(u4VdpId);

    u4FbgNs = FBM_GetFbgNs();
    for (u4FbgIdx = 0; u4FbgIdx < u4FbgNs; u4FbgIdx++)
    {
        u4EsId = (UINT32)ucEsId;
        u4DisplayReady = 0;
        if(u4EsId >= 0xFF)
        {
            for(u4EsId =0 ; u4EsId< MAX_ES_NS ; u4EsId++)
            {
                if (_arEs2Fbg[u4EsId] == (UCHAR)u4FbgIdx)
                {
                    u4DisplayReady = 1;
                    break;
                }
            }
        }
        else
        {
            if(u4EsId < MAX_ES_NS)
            {
                if (_arEs2Fbg[u4EsId] == (UCHAR)u4FbgIdx)
                {
                    u4DisplayReady = 1;
                    break;
                }
            }
            else
            {
                ASSERT((u4EsId < MAX_ES_NS));
            }
        }
        if(u4DisplayReady == 1)
        {
            break;
        }
        /*
        for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
        {
        //            if ((_prVdpConf[u4VdpId]->ucEnable) &&
        //                (_prVdpConf[u4VdpId]->ucMode != VDP_MODE_BG))
            if (_prVdpConf[u4VdpId]->ucEnable)
            {
                for (u4PortIdx = 0; u4PortIdx < VDP_MAX_INPORT_NS; u4PortIdx++)
                {
                    u4EsId = _prVdpConf[u4VdpId]->ucInputPort[u4PortIdx];

                    if (u4EsId < MAX_ES_NS)
                    {
        //                        if ((_arEs2Fbg[u4EsId] == u4FbgIdx) &&
        //                            (_prVdpConf[u4VdpId]->ucMode != VDP_MODE_BG))
                        if (_arEs2Fbg[u4EsId] == u4FbgIdx)
                        {
                            u4DisplayReady = 1;
                            u4PortIdx = VDP_MAX_INPORT_NS;
                            u4VdpId = VDP_NS;
                        }
                    }
                }
            }
        }
        */
    }

    if(u4FbgIdx < u4FbgNs)
    {
        if (u4DisplayReady)
        {
            FBM_SetFrameBufferFlag((UCHAR)u4FbgIdx, FBM_FLAG_DISP_READY);
        }
        else
        {
            FBM_ClrFrameBufferFlag((UCHAR)u4FbgIdx, FBM_FLAG_DISP_READY);
            FBM_SetFrameBufferFlag((UCHAR)u4FbgIdx, FBM_FLAG_RESET);
        }
    }

#ifndef CC_B2R_SKYPE_SUPPORT
    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        u4EsId = _prVdpConf[u4VdpId]->ucInputPort[0];

        if (u4EsId >= MAX_ES_NS)
        {
            return;
        }

        if ((_arEs2Fbg[u4EsId] == FBM_FBG_ID_UNKNOWN) &&
                (_aucImageConnected[u4VdpId] == 0))
        {
            _VDP_StatusNofify(u4VdpId, VDP_B2R_NO_SIGNAL);
        }
    }
#endif
    UNUSED(ucFbgId);
}
#else
void _VdpCheckFbgReady(void)
{
    UINT32 u4EsId;
    UINT32 u4FbgNs;
    UINT32 u4PortIdx;
    UINT32 u4FbgIdx;
    UINT32 u4VdpId;
    UINT32 u4DisplayReady;

    u4FbgNs = FBM_GetFbgNs();
    for (u4FbgIdx = 0; u4FbgIdx < u4FbgNs; u4FbgIdx++)
    {
        u4DisplayReady = 0;
        for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
        {
//            if ((_prVdpConf[u4VdpId]->ucEnable) &&
//                (_prVdpConf[u4VdpId]->ucMode != VDP_MODE_BG))
            if (_prVdpConf[u4VdpId]->ucEnable)
            {
                for (u4PortIdx = 0; u4PortIdx < VDP_MAX_INPORT_NS; u4PortIdx++)
                {
                    u4EsId = _prVdpConf[u4VdpId]->ucInputPort[u4PortIdx];

                    if (u4EsId < MAX_ES_NS)
                    {
//                        if ((_arEs2Fbg[u4EsId] == u4FbgIdx) &&
//                            (_prVdpConf[u4VdpId]->ucMode != VDP_MODE_BG))
                        if (_arEs2Fbg[u4EsId] == u4FbgIdx)
                        {
                            u4DisplayReady = 1;
                            u4PortIdx = VDP_MAX_INPORT_NS;
                            u4VdpId = VDP_NS;
                        }
                    }
                }
            }
        }

        if (u4DisplayReady)
        {
            FBM_SetFrameBufferFlag(u4FbgIdx, FBM_FLAG_DISP_READY);
        }
        else
        {
            FBM_ClrFrameBufferFlag(u4FbgIdx, FBM_FLAG_DISP_READY);
            FBM_SetFrameBufferFlag(u4FbgIdx, FBM_FLAG_RESET);
        }
    }

    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        u4EsId = _prVdpConf[u4VdpId]->ucInputPort[0];

        if (u4EsId >= MAX_ES_NS)
        {
            return;
        }

        if ((_arEs2Fbg[u4EsId] == FBM_FBG_ID_UNKNOWN) &&
                (_aucImageConnected[u4VdpId] == 0))
        {
            _VDP_StatusNofify(u4VdpId, VDP_B2R_NO_SIGNAL);
        }
    }
}
#endif
/**
 * VDP DTV frame buffer group change notify
 *
 * @param ucFbgId specify the frame buffer group id.
 * @param ucEsId specify the video decoder id.
 * @return void
 */
void _VdpFbgChgNotify(UCHAR ucFbgId, UCHAR ucEsId)
{
    UINT32 u4VdpIdx;
    UINT32 u4PortIdx;
#ifdef CC_DYNAMIC_MPEG_SIZE /* Digital decoder variable-dram-size notification */
    UINT32 u4FbWidth;
    UINT32 u4FbHeight;
#endif

    ASSERT(ucEsId < MAX_ES_NS);

    VDP_MUTEX_LOCK;

    if(ucEsId < MAX_ES_NS)
    {
        _arEs2Fbg[ucEsId] = ucFbgId;
    }
    else
    {
        ASSERT(ucEsId < MAX_ES_NS);
    }

    for (u4VdpIdx = 0; u4VdpIdx < VDP_NS; u4VdpIdx++)
    {
        for (u4PortIdx = 0; u4PortIdx < VDP_MAX_INPORT_NS; u4PortIdx++)
        {
            if (_prVdpConf[u4VdpIdx]->ucInputPort[u4PortIdx] == ucEsId)
            {
                UINT8 bType;
                bType = bGetVideoDecType(u4VdpIdx);
                if((SV_VD_MPEGHD == bType) || (SV_VD_MPEGSD == bType))
                {
                    //_VDP_SetScposDelayUnMute(u4VdpIdx, 10, 1);
                    _vDrvVideoSetMute(MUTE_MODULE_B2R, u4VdpIdx, 10, TRUE);
                    _VDP_VsyncSendCmd(u4VdpIdx, VDP_CMD_SET_INPUT);
                }
                u4PortIdx = VDP_MAX_INPORT_NS;

            }
        }
    }

#ifdef CC_53XX_SWDMX_V2
    _VdpCheckFbgReady(ucFbgId, ucEsId);
#else
    _VdpCheckFbgReady();
#endif

    VDP_MUTEX_UNLOCK;

#ifdef CC_DYNAMIC_MPEG_SIZE /* Digital decoder variable-dram-size notification */
    if (ucFbgId < FBM_FBG_ID_UNKNOWN)
    {
        for (u4VdpIdx = 0; u4VdpIdx < VDP_NS; u4VdpIdx++)
        {
            for (u4PortIdx = 0; u4PortIdx < VDP_MAX_INPORT_NS; u4PortIdx++)
            {
                if (_prVdpConf[u4VdpIdx]->ucInputPort[u4PortIdx] == ucEsId)
                {
                    if(u4VdpIdx ==0)
                    {
                        FBM_GetFrameBufferSize(ucFbgId, &u4FbWidth, &u4FbHeight);
                        SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_MPEG_SIZE + (UINT32)u4VdpIdx),
                                      (u4FbWidth*u4FbHeight),
                                      FBM_GetRunningVdecMemSize(ucFbgId));
                        vMpegModeChg(u4VdpIdx);
                        vMpegModeDetDone(u4VdpIdx);
                        VERIFY (x_sema_lock(_hB2RVdecConfMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
                    }
                }
            }
        }
    }
#endif
}

#ifdef CC_SUPPORT_TVE
extern UCHAR _ucSameB2rVdp;
#endif

/**
 * VDP DTV video frontend enable/disable
 *
 * @param ucVdpId specify the video plane id.
 * @return void
 */
void _B2rSetEnable(UCHAR ucVdpId)
{
    UCHAR ucEnable;

    if (ucVdpId >= VDP_NS)
    {
        return;
    }

#if defined(CC_B2R_EMULATION) || defined(FBM_VIRTUAL_MPEG)
    _prVdpConf[ucVdpId]->ucB2rId = B2R_1;
#endif

    ucEnable = _prVdpConf[ucVdpId]->ucEnable;
#ifdef CC_SCPOS_EN
    if ((_prVdpConf[ucVdpId]->ucB2rId < B2R_NS) &&
            (_prVdpConf[ucVdpId]->ucVdpEnable))
    {
        // B2R enable = switch to B2R & VDP enable
        _prVdpConf[ucVdpId]->ucEnable = 1;
    }
    else
    {
        _prVdpConf[ucVdpId]->ucEnable = 0;
    }
#else
    _prVdpConf[ucVdpId]->ucEnable = _prVdpConf[ucVdpId]->ucVdpEnable;
#endif

    if (ucEnable != _prVdpConf[ucVdpId]->ucEnable)
    {
#ifdef CC_SUPPORT_TVE
        if (_ucSameB2rVdp != VDP_NS && _prVdpConf[ucVdpId]->ucEnable == 0)
        {
            /* don't reset the b2r if dual Vdp are running */
#if 0 // Set _ucSameB2rVdp in VDP_B2rSwitch fucntion
            _ucSameB2rVdp = VDP_NS;
#endif
            return;
        }
#endif

        _prVdpConf[ucVdpId]->ucStatus = VDP_STATUS_NOSIGNAL;

#ifdef CC_53XX_SWDMX_V2
        _VdpCheckFbgReady(0xFF, 0xFF);
#else
        _VdpCheckFbgReady();
#endif
        _VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_ENABLE);

#ifdef CC_SCPOS_EN
        if (_prVdpConf[ucVdpId]->ucEnable)
        {
            if (_prVdpConf[ucVdpId]->ucB2rId < B2R_NS)
            {
                vMpegModeChg(ucVdpId);
#ifdef CC_SUPPORT_TVE
                if (_prVdpConf[ucVdpId]->ucB2rId==_prVdpConf[1-ucVdpId]->ucB2rId)
                {
                    _prVdpConf[ucVdpId]->ucStatus = _prVdpConf[1-ucVdpId]->ucStatus;
                    vMpegModeDetDone(ucVdpId);
                }
#endif
            }
        }
#endif
    }
}

void B2R_SetEs2Fbg(UCHAR ucEsId, UCHAR ucFbgId)
{
    VDP_MUTEX_LOCK;
    if (ucEsId >= MAX_ES_NS)
    {
        VDP_MUTEX_UNLOCK;
        return;
    }

    _arEs2Fbg[ucEsId] = ucFbgId;
    VDP_MUTEX_UNLOCK;
}

UCHAR B2R_GetEs2Fbg(UCHAR ucEsId)
{
    UCHAR ucFbgId;
    if (ucEsId >= MAX_ES_NS)
    {
        VDP_MUTEX_UNLOCK;
        return VDP_SET_ERROR;
    }

    VDP_MUTEX_LOCK;
    ucFbgId = _arEs2Fbg[ucEsId];
    VDP_MUTEX_UNLOCK;
    return ucFbgId;
}

/**
 * DTV status notify
 *
 * @param ucVdpId specify the video plane id.
 * @param u4Status specify DTV status.
 * @return void
 */
void _VDP_StatusNofify(UCHAR ucVdpId, UINT32 u4Status)
{
    LOG(3, "DTV(%d) Event(%d)\n", ucVdpId, u4Status);

    VDP_MUTEX_LOCK;

#ifdef __MODEL_slt__
    if (_prVdpConf[ucVdpId]->ucB2rId < B2R_NS)
    {
        if (u4Status == VDP_B2R_START_PLAY)
        {
            _prVdpConf[ucVdpId]->ucStatus = VDP_STATUS_STABLE;
        }
        vMpegModeChg(ucVdpId);
        vMpegModeDetDone(ucVdpId);
    }

#else

#ifdef CC_SCPOS_EN
    if (_prVdpConf[ucVdpId]->ucB2rId < B2R_NS)
    {
        UINT32 u4IssueModeChange = 0;

        // When DTV signal not ready, mute SCPOS
        if ((u4Status == VDP_B2R_NO_SIGNAL))
        {
            _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, FOREVER_MUTE, FALSE);
            LOG(3, "DTV Mute B2R No Signal VDP(%d)\n", ucVdpId);
        }
        else if (u4Status == VDP_B2R_START_PLAY)
        {
            LOG(3, "DTV Unmute Start Play -S VDP(%d)(%d)\n", ucVdpId, _prVdpConf[ucVdpId]->ucEarlyDisp);
            if (_prVdpConf[ucVdpId]->ucEarlyDisp == 0)
            {
                _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, 0, FALSE);
                LOG(3, "DTV Unmute Start Play VDP(%d)(%d)\n", ucVdpId, _prVdpConf[ucVdpId]->ucEarlyDisp);
            }
        }

        if (u4Status == VDP_B2R_NO_SIGNAL)
        {
            if (_prVdpConf[ucVdpId]->ucStatus != VDP_STATUS_NOSIGNAL)
            {
                u4IssueModeChange = 1;
            }

            _prVdpConf[ucVdpId]->ucStatus = VDP_STATUS_NOSIGNAL;

#ifdef CC_SRM_ON
            _prVdpConf[ucVdpId]->u4MpegHeight = 0;
            _prVdpConf[ucVdpId]->u4MpegWidth = 0;

#if 0 /* notification change */
            SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_MPEG_SIZE + (UINT32)ucVdpId),
                          0, 0);
#endif
#endif
        }
        else if (u4Status == VDP_B2R_START_PLAY)
        {
            if (_prVdpConf[ucVdpId]->ucStatus != VDP_STATUS_STABLE)
            {
                u4IssueModeChange = 1;
            }

            _prVdpConf[ucVdpId]->ucStatus = VDP_STATUS_STABLE;
            //CR DTV00140773
#ifdef CC_SUPPORT_TVE
#if 0
            if ((1-ucVdpId) < VDP_NS && (1-ucVdpId >=0))
            {
                _prVdpConf[1-ucVdpId]->ucStatus = VDP_STATUS_STABLE;
            }
#endif
#endif
            //CR DTV00140773
#ifdef CC_SRM_ON
#if 0 /* notification change */
            SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_MPEG_SIZE + (UINT32)ucVdpId),
                          _prVdpConf[ucVdpId]->u4MpegHeight * _prVdpConf[ucVdpId]->u4MpegWidth,
                          0);
#endif
#endif
        }
        else
//        if ((u4Status == VDP_B2R_OUTPUT_CHG) || (u4Status == VDP_B2R_RESOLUTION_NFY))
        {
            // RM interface change
            static UCHAR _aucLastPmxMode[VDP_NS];

            if (_prVdpConf[ucVdpId]->rOutInfo.ucPmxMode != _aucLastPmxMode[ucVdpId])
            {
                _aucLastPmxMode[ucVdpId] = _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode;

                if (_prVdpConf[ucVdpId]->ucStatus == VDP_STATUS_STABLE)
                {
                    u4IssueModeChange = 1;
                }
            }
            else if (_prVdpConf[ucVdpId]->ucStatus == VDP_STATUS_STABLE)
            {
#if 0
                vApiVideoInputChangeNotify(ucVdpId);
#endif
                /* Code Review to use mode change instead of calling vApiVideoInputChangeNotify */
                u4IssueModeChange = 1;
            }
        }

#if 0
        if ((DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5371) ||
                (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5331))
        {
            _VDP_ScposNotReady(ucVdpId, 6);
        }
#endif
        if (_ucThumbnailMode != 0)
        {
            if (u4IssueModeChange != 0)
            {
                u4IssueModeChange = 0;
                LOG(5, "Thumbnail mode, no mode change\n");
            }
        }

        if (u4IssueModeChange != 0)
        {
            LOG(3, "DTV Mode Change\n");
#ifdef TIME_MEASUREMENT
            TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "B2R Mode Change");
#endif

            _vDrvVideoSetMute(MUTE_MODULE_MODECHG, ucVdpId, 10, FALSE);

            vMpegModeChg(ucVdpId);
            vMpegModeDetDone(ucVdpId);
        }
    }
    else
    {
        _prVdpConf[ucVdpId]->u4MpegHeight = 0;
        _prVdpConf[ucVdpId]->u4MpegWidth = 0;

#ifdef CC_SRM_ON
#if 0 /* notification change */
        SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_MPEG_SIZE + (UINT32)ucVdpId),
                      0, 0);
#endif
#endif
    }
#endif

#endif // __MODEL_slt__

    VDP_MUTEX_UNLOCK;
}

/**
 * DTV event notify
 *
 * @param ucVdpId specify the video plane id.
 * @param u4Arg1 specify arguement #1.
 * @param u4Arg1 specify arguement #2.
 * @param u4Arg1 specify arguement #3.
 * @return void
 */
void _VDP_EventNofify(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3)
{
    if (u4Arg1 >= (UINT32)VDP_CB_FUNC_NS)
    {
        ASSERT(u4Arg1 < (UINT32)VDP_CB_FUNC_NS);
        return;
    }

    if(ucVdpId >= VDP_NS)
    {
        ASSERT(0);
        return;
    }

    if(!_prVdpConf[ucVdpId])
    {
        return;
    }

    LOG(10, "EVENT NFY (%d %d %d %d)\n", ucVdpId, u4Arg1, u4Arg2, u4Arg3);

    if (VDP_CHECK_CB_FUNC_VERIFY(_rVdpCbFunc.au4CbFunc[u4Arg1], _rVdpCbFunc.au4CbFuncCRC[u4Arg1]))
    {
        if (u4Arg1 == (UINT32)VDP_CB_FUNC_PLAY_DONE_IND)
        {
            ((PLAYDONE_CFG_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_PLAY_DONE_IND])(ucVdpId, u4Arg1, u4Arg2);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_AFD_IND)
        {
            ((AFD_CFG_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_AFD_IND])(ucVdpId, u4Arg2, (FBM_ASPECT_RATIO_T*)u4Arg3);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_AFD_IND_2)
        {
            ((AFD_CFG_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_AFD_IND_2])(ucVdpId, u4Arg2, (FBM_ASPECT_RATIO_T*)u4Arg3);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_PTS_IND)
        {
            ((PTS_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_PTS_IND])(ucVdpId, u4Arg2, u4Arg3);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_UNMUTE_IND)
        {
            ((UNMUTE_CFG_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_UNMUTE_IND])(ucVdpId, (BOOL)u4Arg2);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_MM_COND_IND)
        {
            //BOOL fgFastForward = VDP_GetFastForward(u4Arg3);
#ifdef CC_53XX_SWDMX_V2
            ((VDP_MM_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_MM_COND_IND])
            (ucVdpId, (VDP_COND_T)u4Arg2,(BOOL)(_prVdpPrm[ucVdpId]->rPortPrm.fgFastForward),
             (UINT32)(u4Arg3));
#else
            ((VDP_MM_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_MM_COND_IND])
            (ucVdpId, (VDP_COND_T)u4Arg2,(BOOL)(_prVdpPrm[ucVdpId]->rPortPrm.fgFastForward));
#endif
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_LIPSYNC_IND)
        {
            ((VDP_LIPSYNC_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_LIPSYNC_IND])();
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_Get_PIC_TYPE)
        {
            VDEC_PICTYPE_CB_T rPicType;
            x_memset(&rPicType, 0, sizeof(VDEC_PICTYPE_CB_T));
            rPicType.u1EsId = (UINT8)u4Arg2;
            rPicType.u1PicType = (UINT8)u4Arg3;
            ((VDP_GET_PIC_TYPE_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_Get_PIC_TYPE])(&rPicType);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_MM_STEP_FIN_IND)
        {
#ifdef CC_53XX_SWDMX_V2
            ((VDP_STEP_FIN_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_MM_STEP_FIN_IND])
            (ucVdpId, u4Arg2, u4Arg3,
             _prVdpPrm[ucVdpId]->rPortPrm.u4AttachedSrcId);
#else
            ((VDP_STEP_FIN_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_MM_STEP_FIN_IND])
            (ucVdpId, u4Arg2, u4Arg3);
#endif
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_MM_SEEK_FIN_IND)
        {
#ifdef CC_53XX_SWDMX_V2
            ((VDP_SEEK_FIN_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_MM_SEEK_FIN_IND])(_prVdpPrm[ucVdpId]->rPortPrm.u4AttachedSrcId, ucVdpId, u4Arg2, u4Arg3);
#else
            ((VDP_SEEK_FIN_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_MM_SEEK_FIN_IND])(ucVdpId, u4Arg2, u4Arg3);
#endif
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_REPEAT_DROP_IND)
        {
            ((VDP_REPEAT_DROP_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_REPEAT_DROP_IND])(ucVdpId, u4Arg2);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_LIPSYNC_OK_TO_NG_IND)
        {
            ((VDP_LIPSYNC_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_LIPSYNC_OK_TO_NG_IND])();
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_LIPSYNC_NG_TO_OK_IND)
        {
            ((VDP_LIPSYNC_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_LIPSYNC_NG_TO_OK_IND])();
        }
        else
        {
            ASSERT(0);
        }
    }
    else
    {
        ASSERT(_rVdpCbFunc.au4CbFunc[u4Arg1] == 0);
    }
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/**
 * VDP query function, show DTV status in CLI with debug level 1.
 *
 * @param void
 * @return void
 */
void VDP_PrintDtvInfo(void)
{
    UINT32 u4VdpId;

    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        if (_prVdpConf[u4VdpId]->ucB2rId < B2R_NS)
        {
            LOG(3, "DTV INFO RESOLUTION(%d, %d) Aspect Ratio(%d) AFD(%d)\n",
                _prVdpConf[u4VdpId]->u4SrcWidth,
                _prVdpConf[u4VdpId]->u4SrcHeight,
                _prVdpConf[u4VdpId]->u4AspectRatio,
                _prVdpConf[u4VdpId]->u4Afd);
        }
    }
}

UINT32 VDP_SetPicInfo(UCHAR ucVdpId, VDP_SET_PIC_INFO_T rPicInfo)
{
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UCHAR ucEsId;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;

    VDP_MUTEX_LOCK;
    ucEsId = _prVdpConf[ucVdpId]->ucInputPort[0];

    if (ucEsId >= MAX_ES_NS)
    {
        VDP_MUTEX_UNLOCK;
        return VDP_SET_ERROR;
    }
    ucFbgId = _arEs2Fbg[ucEsId] ;
    ucFbId = VDP_FrcGetLockFrameBuffer(ucVdpId);

    if (ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);

        if ((prFbmSeqHdr == NULL) || (prFbmPicHdr == NULL))
        {
            VDP_MUTEX_UNLOCK;
            return VDP_SET_ERROR;
        }
        else
        {
            prFbmPicHdr->u4TickNum = rPicInfo.u4TickNum;
            _VDP_SetCurrentTick(ucVdpId, rPicInfo.u4TickNum);
        }
    }
    else
    {
        VDP_MUTEX_UNLOCK;
        return VDP_SET_ERROR;

    }
    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

UINT32 VDP_GetPicInfo(UINT8 u1B2rId, VDP_PIC_INFO_T* prPicInfo)
{
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UCHAR ucEsId;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;
    UINT32 u4Pts;

    VERIFY_NULL(prPicInfo);

    VDP_MUTEX_LOCK;
#if 0
    ucEsId = _prVdpConf[ucVdpId]->ucInputPort[0];
#else
    ucEsId = u1B2rId;
#endif

    if (ucEsId >= MAX_ES_NS)
    {
        VDP_MUTEX_UNLOCK;
        return VDP_SET_ERROR;
    }
    ucFbgId = _arEs2Fbg[ucEsId];
    ucFbId = VDP_FrcGetLockFrameBuffer(u1B2rId);

    if (ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);

        if ((prFbmSeqHdr == NULL) || (prFbmPicHdr == NULL))
        {
            VDP_MUTEX_UNLOCK;
            return VDP_SET_ERROR;
        }
        else
        {
            prPicInfo->ucHours = prFbmSeqHdr->ucHours;
            prPicInfo->ucMinutes = prFbmSeqHdr->ucMinutes;
            prPicInfo->ucSeconds = prFbmSeqHdr->ucSeconds;
            prPicInfo->ucPictures = prFbmSeqHdr->ucPictures;
            prPicInfo->u4TotlaTimeOffset = prFbmPicHdr->u4TotlaTimeOffset;
            prPicInfo->u4TickNum = prFbmPicHdr->u4TickNum;

            prPicInfo->u8Offset = prFbmPicHdr->u8Offset;
            prPicInfo->u8OffsetDisp = prFbmPicHdr->u8OffsetDisp;
            //prPicInfo->u4Pts = prFbmPicHdr->u4PTS;
            u4Pts = _VDP_GetCurrentPts(u1B2rId);

            prPicInfo->u4Pts = u4Pts;

            if (prFbmPicHdr->ucTrueZeroPTS == FBM_USE_LAST_PTS_IF_ZERO)
            {
                if (u4Pts == 0)
                {
                    prPicInfo->u4Pts = _VDP_GetLastPts(u1B2rId);
                }
            }
        }
    }
    else
    {
        VDP_MUTEX_UNLOCK;
        return VDP_SET_ERROR;

    }
    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

/**
 * Get mute status of DTV playback
 *
 * @param ucVdpId specify the video plane id.
 * @return TRUE for mute, FALSE for not mute
 */
BOOL VDP_GetUnMuteStatus(UCHAR ucVdpId)
{
    return VDP_FrcGetUnMuteStatus(ucVdpId);
}

/**
 * Video Image Module : Get information
 *
 * @param void
 * @return pointer of Video Image Module Capability Infomation
 */
VDP_IMAGE_INFO_T* VDP_Image_GetInfo(void)
{
    return _VDP_Image_GetInfo();
}

/**
 * Video Image Module : Connect
 *
 * @param u4VdpId: Video plane connected to
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 VDP_Image_Connect(UINT32 u4VdpId)
{
    UINT32 u4ReturnValue;
    UINT32 u4Idx;

    VERIFY_VDP_ID(u4VdpId);

    VDP_MUTEX_LOCK;

    for (u4Idx = 0; u4Idx < VDP_NS; u4Idx++)
    {
        _aucImageConnected[u4Idx] = 0;
    }

    u4ReturnValue = _VDP_Image_Connect(u4VdpId);

    if (u4ReturnValue == VDP_SET_OK)
    {
        _aucImageConnected[u4VdpId] = 1;
    }

    VDP_MUTEX_UNLOCK;

    return u4ReturnValue;
}

/**
 * Video Image Module : Disconnect
 *
 * @param void
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 VDP_Image_Disconnect(void)
{
    UINT32 u4VdpId;

    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        _aucImageConnected[u4VdpId] = 0;
    }

    return _VDP_Image_Disconnect();
}

/**
 * Video Image Module : Mute on/off
 *
 * @param u4OnOff: Mute on/off
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 VDP_Image_Mute(UINT32 u4OnOff)
{
    return _VDP_Image_Mute(u4OnOff);
}

/**
 * Video Image Module : Show Frame Buffer
 *
 * @param prInfo: Frame Buffer Info
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 VDP_Image_Show(VDP_IMAGE_ARG_T* prInfo)
{
    return _VDP_Image_Show(prInfo);
}

/**
 * Video Image Module : Get Frame Buffer Address
 *
 * @param u4FrameBufferId: Frame Buffer id
 * @param u4YAddr: pointer to store Y address
 * @param u4CAddr: pointer to store C address
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 VDP_Image_GetFrameBuffer(UINT32 u4FrameBufferId, UINT32* u4YAddr, UINT32* u4CAddr)
{
    return _VDP_Image_GetFrameBuffer(u4FrameBufferId, u4YAddr, u4CAddr);
}

/**
 * VDP connect to video decoder.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucEsId specify video decoder id.
 * @param ucPort always 0.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetInput(UCHAR ucVdpId, UCHAR ucEsId, UCHAR ucPort)
{
#if 0
    UINT32 u4FbgId;
    UINT32 u4VdpIdx;
    UINT32 u4PortIdx;
    UINT32 u4PortFbgId;
#endif

    VERIFY_VDP_ID(ucVdpId);

    if (ucPort >= VDP_MAX_INPORT_NS)
    {
        return VDP_SET_ERROR;
    }

    if (_prVdpConf[ucVdpId]->ucInputPort[ucPort] == ucEsId)
    {
        return VDP_SET_OK;
    }

    VDP_MUTEX_LOCK;

    //workaround for 3D
#if 0
    _prVdpConf[ucVdpId]->ucInputPort[ucPort] = 0;//ucEsId;
#else
    _prVdpConf[ucVdpId]->ucInputPort[ucPort] = ucEsId;
#endif

#if 0 /* Main and Sub are with DTV for SCART out/TVE application */
    if (ucEsId >= MAX_ES_NS)
    {
        u4FbgId = FBM_FBG_ID_UNKNOWN;
    }
    else
    {
        u4FbgId = B2R_GetEs2Fbg(ucEsId);
    }

    // protection for dual TV
    if (u4FbgId != FBM_FBG_ID_UNKNOWN)
    {
        for (u4VdpIdx = 0; u4VdpIdx < VDP_NS; u4VdpIdx++)
        {
            if (u4VdpIdx != ucVdpId)
            {
                for (u4PortIdx = 0; u4PortIdx <VDP_MAX_INPORT_NS; u4PortIdx++)
                {
                    if (_arVdpConf[u4VdpIdx].ucInputPort[u4PortIdx] >= MAX_ES_NS)
                    {
                        u4PortFbgId = FBM_FBG_ID_UNKNOWN;
                    }
                    else
                    {
                        u4PortFbgId = B2R_GetEs2Fbg(_arVdpConf[u4VdpIdx].ucInputPort[u4PortIdx]);
                    }

                    if (u4PortFbgId == u4FbgId)
                    {
                        LOG(1, "Dual TV is NOT ready! (%d, %d, %d) \n", u4VdpIdx, u4PortIdx, u4FbgId);
                        LOG(1, "Port (%d) (%d)\n"
                            , _arVdpConf[0].ucInputPort[0]
                            , _arVdpConf[1].ucInputPort[0]);
                        LOG(1, "ES (%d, %d, %d %d, %d, %d)\n"
                            , B2R_GetEs2Fbg(0)
                            , B2R_GetEs2Fbg(1)
                            , B2R_GetEs2Fbg(2)
                            , B2R_GetEs2Fbg(3)
                            , B2R_GetEs2Fbg(4)
                            , B2R_GetEs2Fbg(5));
                        //ASSERT(0);
                        _arVdpConf[ucVdpId].ucInputPort[ucPort] = MAX_ES_NS;

                    }
                }
            }
        }
    }
#endif
    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_INPUT);
#ifdef CC_53XX_SWDMX_V2
    _VdpCheckFbgReady(0xFF, 0xFF);
#else
    _VdpCheckFbgReady();
#endif

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

/**
 * VDP query video plane input connection.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucPort always 0.
 * @param pucFbgId video decoder id.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetInput(UCHAR ucVdpId, UCHAR ucPort, UCHAR* pucFbgId)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pucFbgId);

    if (ucPort >= VDP_MAX_INPORT_NS)
    {
        return VDP_SET_ERROR;
    }

    if (_prVdpConf[ucVdpId]->ucInputPort[ucPort] < MAX_ES_NS)
    {
        *pucFbgId = B2R_GetEs2Fbg(_prVdpConf[ucVdpId]->ucInputPort[ucPort]);
    }
    else
    {
        *pucFbgId = FBM_FBG_ID_UNKNOWN;
    }

    return VDP_SET_OK;
}

/**
 * VDP switch B2R to corresponding video plane
 *
 * @param ucVdpId specify the video plane id.
 * @param ucB2rId specify B2R id (0 only)
 * @return void
 */
void VDP_B2rSwitch(UCHAR ucVdpId, UCHAR ucB2rId)
{
    // if ucB2rId = B2R_NS, it means that video plane switch to non-DTV video input

    if (ucVdpId < VDP_NS)
    {
#ifndef CC_SUPPORT_TVE
        UINT32 u4VdpIdx;
#endif

#ifdef CC_SCPOS_EN
        vDrvVideoFixedPreScaler(ucVdpId, CC_VDP_FIXED_PRESCALER, 1);
#endif

        _prVdpConf[ucVdpId]->ucB2rId = ucB2rId;

#ifndef CC_SUPPORT_TVE
        // remove this b2r module from all other vdp
        for (u4VdpIdx = 0; u4VdpIdx < VDP_NS; u4VdpIdx++)
        {
            if ((_prVdpConf[u4VdpIdx]->ucB2rId == ucB2rId) &&
                    (u4VdpIdx != ucVdpId))
            {
                _prVdpConf[u4VdpIdx]->ucB2rId = B2R_NS;
            }
        }
#else
        if ( (_prVdpConf[ucVdpId]->ucB2rId == B2R_1) &&
                (_prVdpConf[VDP_1]->ucB2rId == _prVdpConf[VDP_2]->ucB2rId))
        {
            _ucSameB2rVdp = 1-ucVdpId;
        }
        else
        {
            _ucSameB2rVdp = VDP_NS;
        }
#endif

        if (ucB2rId < B2R_NS)
        {
#ifdef CC_SCPOS_EN
            vMpegInitB2rConf();
#endif
#ifndef CC_SUPPORT_TVE
            VDP_HalB2rSwitch(ucVdpId, ucB2rId);
#endif
        }
        else
        {

#ifdef CC_SCPOS_EN
            // When video plane connect to non-DTV, unmute DTV mask
            _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, 0, FALSE);
            LOG(3, "DTV Unmute B2R Switch VDP(%d)\n", ucVdpId);

#endif

#if 1
#ifdef CC_SCPOS_EN
            vMpegInitB2rConf();
#endif
#endif
        }
#ifdef CC_SUPPORT_TVE
        VDP_HalB2rSwitch(ucVdpId, ucB2rId);
#endif

        if (ucB2rId == B2R_NS)
        {
            _prVdpConf[ucVdpId]->u4Afd = ACT_FMT_DEFAULT;
        }

        _B2rSetEnable(ucVdpId);
    }
}

/**
 * VDP transfer VDP id to B2R id
 *
 * @param ucVdpId specify the video plane id.
 * @return B2R id
 */
UCHAR VDP_Vdp2B2r(UCHAR ucVdpId)
{
    if (ucVdpId < VDP_NS)
    {
        return _prVdpConf[ucVdpId]->ucB2rId;
    }

    return B2R_NS;
}

/**
 * VDP transfer B2R id to VDP id
 *
 * @param ucB2rId specify the B2R id.
 * @return video plane id
 */
UCHAR VDP_B2r2Vdp(UCHAR ucB2rId)
{
#ifdef CC_SUPPORT_TVE
    if (_prVdpConf[VDP_1]->ucB2rId == _prVdpConf[VDP_2]->ucB2rId
            && ucB2rId == _prVdpConf[VDP_1]->ucB2rId)
    {
        return VDP_1;
    }
#endif
    if (ucB2rId < B2R_NS)
    {
        UCHAR ucVdpId;

        for (ucVdpId = 0; ucVdpId < VDP_NS; ucVdpId++)
        {
            if (_prVdpConf[ucVdpId]->ucB2rId == ucB2rId)
            {
                return ucVdpId;
            }
        }
    }

    return VDP_NS;
}

/**
 * VDP transfer ES id to VDP id
 *
 * @param ucEsId specify the video decoder id.
 * @return VDP id
 */
UINT32 VDP_Es2Vdp(UCHAR ucEsId)
{
    UINT32 u4VdpIdx;
    UINT32 u4PortIdx;
    UINT32 u4Return = VDP_1;

    ASSERT(ucEsId < MAX_ES_NS);

    VDP_MUTEX_LOCK;

    for (u4VdpIdx = 0; u4VdpIdx < VDP_NS; u4VdpIdx++)
    {
        for (u4PortIdx = 0; u4PortIdx < VDP_MAX_INPORT_NS; u4PortIdx++)
        {
            if (_prVdpConf[u4VdpIdx]->ucInputPort[u4PortIdx] == ucEsId)
            {
                u4Return = u4VdpIdx;
                break;
            }
        }
    }

    VDP_MUTEX_UNLOCK;
    return u4Return;
}

/**
 * VDP send test pattern (for module test)
 *
 * @param void
 * @return void
 */
void VDP_Pattern(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3)
{
    UNUSED(ucVdpId);
    UNUSED(u4Arg1);
    UNUSED(u4Arg2);
    UNUSED(u4Arg3);

    if (ucVdpId >= VDP_NS)
    {
        return;
    }

#if 0
    VDP_IMAGE_ARG_T rInfo;

    VDP_SetEnable(VDP_1, 1);

    VDP_SetMode(VDP_1, VDP_MODE_NORMAL);

    x_thread_delay(500);

    VDP_Image_Connect(VDP_1);

    VDP_Image_Mute(0);

    rInfo.u4ColorMode = VDP_IMAGE_420_MODE;
    rInfo.u4FrameBufferId = 0;
    rInfo.u4Width = 1920;
    rInfo.u4Height = 1080;
    rInfo.u4Pitch = 1920;

    x_thread_delay(500);

    VDP_Image_Show(&rInfo);

#else
    /* Change to B2R pattern directly */
    if (u4Arg1 == 99)
    {
#ifdef CC_B2R_SKYPE_SUPPORT
        _VDP_HalStatusTest(ucVdpId);
#else
    _VDP_HalStatusTest();
#endif
        return;
    }
    else if (u4Arg1 == 98)
    {
#ifdef CC_B2R_SKYPE_SUPPORT
        _VDP_HalDisablePattern(ucVdpId);
#else
    _VDP_HalDisablePattern();
#endif
        return;
    }

    _VDP_VsyncReset(ucVdpId);

    _prVdpConf[ucVdpId]->rSrcRegion.u4X = 0;
    _prVdpConf[ucVdpId]->rSrcRegion.u4Y = 0;
    _prVdpConf[ucVdpId]->rSrcRegion.u4Width = VDP_MAX_REGION_WIDTH;
    _prVdpConf[ucVdpId]->rSrcRegion.u4Height = VDP_MAX_REGION_HEIGHT;

    switch (u4Arg1)
    {
    case VDP_B2R_MODE_480P_60:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_480P_60;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 768;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_027;
        LOG(1, "Pattern : VDP_B2R_MODE_480P_60\n");

        break;

    case VDP_B2R_MODE_576I_50:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_576I_50;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 768;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 576;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 864;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 625;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
        LOG(1, "Pattern : VDP_B2R_MODE_576I_50\n");
        break;

    case VDP_B2R_MODE_576P_50:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_576P_50;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 768;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 576;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 864;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 625;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
        LOG(1, "Pattern : VDP_B2R_MODE_576P_50\n");
        break;

    case VDP_B2R_MODE_720P_60:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_720P_60;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1280;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1650;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
        LOG(1, "Pattern : VDP_B2R_MODE_576I_50\n");
        break;

    case VDP_B2R_MODE_1080I_60:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_1080I_60;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2200;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
        LOG(1, "Pattern : VDP_B2R_MODE_1080I_60\n");
        break;

    case VDP_B2R_MODE_1080P_60:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_1080P_60;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2200;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
        LOG(1, "Pattern : VDP_B2R_MODE_1080P_60\n");
        break;

    case VDP_B2R_MODE_480P_24:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_480P_24;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 24;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 768;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 2;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_027;
        LOG(1, "Pattern : VDP_B2R_MODE_480P_24\n");
        break;

    case VDP_B2R_MODE_480P_30:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_480P_30;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 30;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 768;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_027;
        LOG(1, "Pattern : VDP_B2R_MODE_480P_30\n");
        break;

    case VDP_B2R_MODE_576P_25:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_576P_25;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 25;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 768;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 576;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 864;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 625;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
        LOG(1, "Pattern : VDP_B2R_MODE_576P_25\n");
        break;

    case VDP_B2R_MODE_720P_24:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_720P_24;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 24;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1280;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1650;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 2;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
        LOG(1, "Pattern : VDP_B2R_MODE_720P_24\n");
        break;

    case VDP_B2R_MODE_720P_25:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_720P_25;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 25;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1280;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1980;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
        LOG(1, "Pattern : VDP_B2R_MODE_720P_25\n");
        break;

    case VDP_B2R_MODE_720P_30:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_720P_30;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 30;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1280;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1650;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
        LOG(1, "Pattern : VDP_B2R_MODE_720P_30\n");
        break;

    case VDP_B2R_MODE_720P_50:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_720P_50;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1280;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1980;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
        LOG(1, "Pattern : VDP_B2R_MODE_720P_50\n");
        break;

    case VDP_B2R_MODE_1080P_24:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_1080P_24;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 24;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2750;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
        LOG(1, "Pattern : VDP_B2R_MODE_1080P_24\n");
        break;

    case VDP_B2R_MODE_1080P_25:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_1080P_25;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 25;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2640;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
        LOG(1, "Pattern : VDP_B2R_MODE_1080P_25\n");
        break;

    case VDP_B2R_MODE_1080P_30:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_1080P_30;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 30;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2200;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
        LOG(1, "Pattern : VDP_B2R_MODE_1080P_30\n");
        break;

    case VDP_B2R_MODE_1080I_50:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_1080I_50;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2640;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
        LOG(1, "Pattern : VDP_B2R_MODE_1080I_50\n");
        break;

    case VDP_B2R_MODE_1080P_50:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_1080P_50;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2640;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
        LOG(1, "Pattern : VDP_B2R_MODE_1080P_50\n");
        break;

    default:
    case VDP_B2R_MODE_480I_60:
        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_480I_60;

        _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
        _prVdpConf[ucVdpId]->u4SrcLineSize = 768;
        _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
        _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;

        _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
        _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
        _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
        _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
        _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_027;
        LOG(1, "Pattern : VDP_B2R_MODE_480I_60\n");
        break;
    }


    _prVdpConf[ucVdpId]->u4SrcWidth = _prVdpConf[ucVdpId]->rOutInfo.u4Width;
    _prVdpConf[ucVdpId]->u4SrcHeight = _prVdpConf[ucVdpId]->rOutInfo.u4Height;

    _prVdpConf[ucVdpId]->u4MpegWidth = _prVdpConf[ucVdpId]->rOutInfo.u4Width;
    _prVdpConf[ucVdpId]->u4MpegHeight = _prVdpConf[ucVdpId]->rOutInfo.u4Height;

    _VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_OUTR);

    _prVdpConf[ucVdpId]->ucMode = VDP_MODE_BG;
    _VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_MODE);

    _prVdpConf[ucVdpId]->ucVdpEnable = 1;
    _prVdpConf[ucVdpId]->ucEnable = 1;
    _VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_ENABLE);

#ifdef CC_SCPOS_EN
    _prVdpConf[ucVdpId]->rSrcRegion.u4X = 0;
    _prVdpConf[ucVdpId]->rSrcRegion.u4Y = 0;
    _prVdpConf[ucVdpId]->rSrcRegion.u4Width = VDP_MAX_REGION_WIDTH;
    _prVdpConf[ucVdpId]->rSrcRegion.u4Height = VDP_MAX_REGION_HEIGHT;

    _prVdpConf[ucVdpId]->rOutRegion.u4X = 0;
    _prVdpConf[ucVdpId]->rOutRegion.u4Y = 0;
    _prVdpConf[ucVdpId]->rOutRegion.u4Width = VDP_MAX_REGION_WIDTH;
    _prVdpConf[ucVdpId]->rOutRegion.u4Height = VDP_MAX_REGION_HEIGHT;

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    if (bApiVideoSetSrcRegion(ucVdpId, 0, 0, VDP_MAX_REGION_WIDTH, VDP_MAX_REGION_HEIGHT) != 1)
    {
    }
#else
    if (bApiVideoSetSrcPosSize(ucVdpId, 0, 0, VDP_MAX_REGION_WIDTH, VDP_MAX_REGION_HEIGHT) != 1)
    {
    }
#endif

#endif

    _VDP_StatusNofify(ucVdpId, VDP_B2R_START_PLAY);

    x_thread_delay(1000);
#ifdef CC_B2R_SKYPE_SUPPORT
    _VDP_HalStatusTest(ucVdpId);
#else
    _VDP_HalStatusTest();
#endif

#endif
}

/**
 * VDP set video plane with Pause MM feature.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucPause value 0:disable, 1:Pause MM feature enable.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetPauseMm(UINT8 u1B2rId, BOOL fgPause)
{

    //VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

#if 0
    if (_prVdpPrm[ucVdpId]->fgPauseMM != ucPause)
    {
        _prVdpPrm[ucVdpId]->fgPauseMM = ucPause;
    }
#else
    _VDP_SetPauseMM(u1B2rId,fgPause);
#endif
    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

BOOL VDP_GetFastForward(UINT8 u1B2rId)
{
    STC_SPEED_TYPE_T eSpeed = _VDP_GetFrcStcSpeed(u1B2rId);

    if (eSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

UINT32 VDP_SetSpeed(UINT8 u1B2rId, STC_SPEED_TYPE_T eSpeed)
{
    UINT32 u4Speed;
    BOOL fgDiv10 = 0;

    VDP_MUTEX_LOCK;

    switch (eSpeed)
    {
    case STC_SPEED_TYPE_FORWARD_2X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_2X;
        break;
    case STC_SPEED_TYPE_FORWARD_3X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_3X;
        break;
    case STC_SPEED_TYPE_FORWARD_4X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_4X;
        break;
    case STC_SPEED_TYPE_FORWARD_8X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_8X;
        break;
    case STC_SPEED_TYPE_FORWARD_16X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_16X;
        break;
    case STC_SPEED_TYPE_FORWARD_32X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_32X;
        break;
    case STC_SPEED_TYPE_FORWARD_64X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_64X;
        break;
    case STC_SPEED_TYPE_FORWARD_128X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_128X;
        break;
    case STC_SPEED_TYPE_FORWARD_256X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_256X;
        break;
    case STC_SPEED_TYPE_FORWARD_512X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_512X;
        break;
    case STC_SPEED_TYPE_FORWARD_1024X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1024X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_2X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_2X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_3X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_3X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_4X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_4X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_8X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_8X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_16X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_16X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_32X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_32X;
        break;
    case STC_SPEED_TYPE_FORWARD_0_DOT_6X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_0_DOT_6X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_0_DOT_7X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_0_DOT_7X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_0_DOT_8X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_0_DOT_8X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_0_DOT_9X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_0_DOT_9X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DOT_1X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DOT_1X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DOT_2X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DOT_2X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DOT_3X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DOT_3X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DOT_4X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DOT_4X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DOT_5X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DOT_5X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1X:
    default:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1X;
        break;
    }

    _VDP_HalStcSpeed(u1B2rId,u4Speed, fgDiv10);
    _VDP_FrcStcSpeed(u1B2rId,eSpeed);

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;

}


void VDP_SetB2rUpdateStc(UINT8 u1B2rId,BOOL fgEnable)
{
    _VDP_HalUpdateStc(u1B2rId,fgEnable);
}

BOOL VDP_GetB2rUpdateStc(UINT8 u1B2rId)
{
    return _VDP_HalGetUpdateStc(u1B2rId);
}


BOOL VDP_Is4To1024XSpeed(UINT32 u4Speed)
{
#if 0
    if ( ( _VDP_GetFrcStcSpeed(u1B2rId) >= STC_SPEED_TYPE_REWIND_4X ) &&
            ( _VDP_GetFrcStcSpeed(u1B2rId) <= STC_SPEED_TYPE_REWIND_1024X ) )
    {
        return TRUE;
    }
#endif

    if ( (u4Speed >= B2R_SPEED_TYPE_FORWARD_4X )&&
            (u4Speed <= B2R_SPEED_TYPE_FORWARD_1024X ) )
    {
        return TRUE;
    }

    return FALSE;
}


void VDP_SetB2rStc(UINT8 u1B2rId,UINT32 u4B2rStc)
{
    _VDP_HalSetB2rStc(u1B2rId,u4B2rStc);
}

INT64 VDP_GetAvsyncDelta(void)
{
    INT64   i8Delta = 0x0;

#ifdef VDP_AVS_ADJUST_STC
    i8Delta = _VDP_GetAvsyncDelta();
#endif

    return i8Delta;
}

UINT32 VDP_GetB2rStc(UINT8 u1B2rId)
{
    return _VDP_HalGetB2rStc(u1B2rId);
}

UINT32 VDP_SetFrmCrop(UCHAR ucVdpId, UINT32 u4FrmCropTop, UINT32 u4FrmCropBtm)
{

    VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    _prVdpConf[ucVdpId]->u4FrmCropTop = u4FrmCropTop;
    _prVdpConf[ucVdpId]->u4FrmCropBtm = u4FrmCropBtm;

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

UINT32 VDP_GetPts(UCHAR ucVdpId)
{
    UINT32 u4Pts;

    VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    u4Pts = _VDP_GetCurrentPts(ucVdpId);

    VDP_MUTEX_UNLOCK;

    return u4Pts;
}


UINT32 VDP_GetLockFrameBufferAddr(UCHAR ucVdpId, VDP_CAPTURE_INTO_T* prCapInfo)
{

    UCHAR ucEsId;
    UCHAR ucFbgId;
    UCHAR ucFbId;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;

    VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    ucEsId = _prVdpConf[ucVdpId]->ucInputPort[0];

    if (ucEsId >= MAX_ES_NS)
    {
        VDP_MUTEX_UNLOCK;
        return VDP_SET_ERROR;
    }
    ucFbgId = _arEs2Fbg[ucEsId];

    ucFbId = VDP_FrcGetLockFrameBuffer(ucVdpId);

    VDP_MUTEX_UNLOCK;

    FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &(prCapInfo->u4AddrY), &(prCapInfo->u4AddrC));

    FBM_GetFrameBufferSize(ucFbgId, &(prCapInfo->u4MemHSize), &(prCapInfo->u4MemVSize));

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);

    if (prFbmSeqHdr == NULL)
    {
        return VDP_SET_ERROR;
    }
    else
    {
        /* Thumbnail usage: Image resizer will use this as pitch. */
        if (prFbmPicHdr && prFbmSeqHdr->fgResizeSmallPic)
        {
            prCapInfo->u4MemHSize = (UINT32)prFbmPicHdr->u4PicWidthPitch;

            prCapInfo->u4HSize = (UINT32)prFbmPicHdr->u4PicWidth;
            prCapInfo->u4VSize = (UINT32)prFbmPicHdr->u4PicHeight;
        }
        else
        {
            prCapInfo->u4MemHSize = (UINT32)prFbmSeqHdr->u2LineSize;

            prCapInfo->u4HSize = (UINT32)prFbmSeqHdr->u2OrgHSize;
            prCapInfo->u4VSize = (UINT32)prFbmSeqHdr->u2OrgVSize;
        }

        prCapInfo->u4RRMode = (UINT32)prFbmSeqHdr->fgRRYEn;
    }

    return VDP_SET_OK;

}


BOOL VDP_GetFilePosition(UCHAR ucVdpId,
                         INT32 *pi4TempRef, UINT16 *pu2DecodingOrder,
                         UINT64 *pu8Offset, UINT64 *pu8OffsetI)
{
    UINT32 u4Ret;
    VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    u4Ret = _VDP_GetFilePosition(ucVdpId, pi4TempRef, pu2DecodingOrder,
                                 pu8Offset, pu8OffsetI);

    VDP_MUTEX_UNLOCK;

    return (u4Ret == 0) ? FALSE : TRUE;
}

BOOL VDP_GetPosInfo(UINT8 u1B2rId, VDP_POS_INTO_T *prPosInfo)
{
    UINT32 u4Ret;
    //VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    u4Ret = _VDP_GetPosInfo(u1B2rId, prPosInfo);

    VDP_MUTEX_UNLOCK;

    return (u4Ret == 0) ? FALSE : TRUE;
}

BOOL VDP_SetABInfo(UCHAR ucVdpId, VDP_AB_INTO_T *prAB)
{
    UINT32 u4Ret;
    VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    u4Ret = _VDP_SetABInfo(ucVdpId, prAB);

    VDP_MUTEX_UNLOCK;

    return (u4Ret == 0) ? FALSE : TRUE;
}

#ifdef CC_B2R_SKYPE_SUPPORT
UCHAR VDP_GetPendingFB(UCHAR ucVdpId)
{
    return _VDP_GetPendingFB(ucVdpId);
}

void VDP_CleanPendingFB(UCHAR ucVdpId)
{
    _VDP_CleanPendingFB(ucVdpId);
}

void VDP_SetReleaseDispQ(UCHAR ucVdpId)
{
    _VDP_SetReleaseDispQ(ucVdpId);
}
#else
UCHAR VDP_GetPendingFB(void)
{
    UCHAR ucVdpId;

    ucVdpId = _B2R_TO_VDP(B2R_1);
    return _VDP_GetPendingFB(ucVdpId);
}

void VDP_CleanPendingFB(void)
{
    UCHAR ucVdpId;

    ucVdpId = _B2R_TO_VDP(B2R_1);
    _VDP_CleanPendingFB(ucVdpId);
}

void VDP_SetReleaseDispQ(void)
{
    UCHAR ucVdpId;
    ucVdpId = _B2R_TO_VDP(B2R_1);
    _VDP_SetReleaseDispQ(ucVdpId);
}
#endif

void VDP_VsyncMutexLock(void)
{
    _VDP_VsyncMutexLock();
}

void VDP_VsyncMutexUnlock(void)
{
    _VDP_VsyncMutexUnlock();
}

/* Trick Play at 20081001 */
#ifdef CC_53XX_SWDMX_V2
UINT32 VDP_SetTrickMode(UINT8 u1B2rId,UCHAR ucTrick)
{
#else
UINT32 VDP_SetTrickMode(UCHAR ucTrick)
{
    UINT8 u1B2rId = B2R_1;
#endif
    VDP_MUTEX_LOCK;

    VDP_HalSetTrickMode(u1B2rId,ucTrick);

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

#ifdef CC_53XX_SWDMX_V2
UCHAR VDP_GetTrickMode(UINT8 u1B2rId)
{
#else
UCHAR VDP_GetTrickMode(VOID)
{
    UINT8 u1B2rId = B2R_1;
#endif
    UCHAR ucResult;

    VDP_MUTEX_LOCK;

    ucResult = VDP_HalGetTrickMode(u1B2rId);

    VDP_MUTEX_UNLOCK;

    return ucResult;
}

/* Step Forward at 20081030 */
UINT32 VDP_SetStepForward(UINT8 u1B2rId, UCHAR ucSet)
{
    //VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    VDP_HalSetStepForward(u1B2rId,ucSet);

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

/* Step Forward at 20081030 */
UCHAR VDP_GetStepForward(UINT8 u1B2rId)
{
    UCHAR ucResult;

    //VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    ucResult = VDP_HalGetStepForward(u1B2rId);

    VDP_MUTEX_UNLOCK;

    return ucResult;
}

/* Slow Forward at 20090506 */
UINT32 VDP_SetSlowForward(UINT8 u1B2rId, UCHAR ucSet)
{
    //VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    VDP_HalSetSlowForward(u1B2rId,ucSet);

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

/* Slow Forward at 20090506 */
UCHAR VDP_GetSlowForward(UINT8 u1B2rId)
{
    UCHAR ucResult;

    //VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    ucResult = VDP_HalGetSlowForward(u1B2rId);

    VDP_MUTEX_UNLOCK;

    return ucResult;
}
UINT32 VDP_SetFBNoTimeout(UINT8 u1B2rId, UCHAR ucSet)
{
    UCHAR ucEsId;
    UCHAR ucFbgId;

#if 0

    VERIFY_VDP_ID(ucVdpId);

    ucEsId = _prVdpConf[ucVdpId]->ucInputPort[0];
    if (ucEsId >= MAX_ES_NS)
    {
        return VDP_SET_ERROR;
    }
#else
    ucEsId = u1B2rId;
#endif

    ucFbgId = _arEs2Fbg[ucEsId] ;

    VDP_MUTEX_LOCK;

    if (ucSet != 0)
    {
        FBM_SetFrameBufferGlobalFlag(ucFbgId, FBM_FLAG_FB_NO_TIMEOUT);
    }
    else
    {
        FBM_ClrFrameBufferGlobalFlag(ucFbgId, FBM_FLAG_FB_NO_TIMEOUT);
    }

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;

}


/* VDP trigger mode + audio master to do trigger mode */
void VDP_TriggerAudReceive(UINT32 u4Pts)
{
    _VDPTriggerAudReceive(u4Pts);
}

/* VDP Seek done notification */
void VDP_SetSeek(UCHAR ucVdpId, UCHAR ucSet, UINT32 u4Pts)
{
    VDP_MUTEX_LOCK;

    _VDPSetSeek(ucVdpId, ucSet, u4Pts);

    VDP_MUTEX_UNLOCK;
}

/* VDP AB repeat notification */
void VDP_SetABRepeat(UCHAR ucVdpId, UCHAR ucSet)
{
    VDP_MUTEX_LOCK;

    _VDPSetABRepeat(ucVdpId, ucSet);

    VDP_MUTEX_UNLOCK;
}

/* VDP Set Wait STC while starts */
void VDP_SetWaitSTC(UCHAR ucVdpId, UCHAR ucSet)
{
    VDP_MUTEX_LOCK;

    _VDPSetWaitSTC(ucVdpId, ucSet);

    VDP_MUTEX_UNLOCK;
}

/* VDP Get Wait STC while starts */
UCHAR VDP_GetWaitSTC(UCHAR ucVdpId)
{
    UCHAR ucResult;

    VDP_MUTEX_LOCK;

    ucResult = _VDPGetWaitSTC(ucVdpId);

    VDP_MUTEX_UNLOCK;

    return ucResult;
}

/* B2R CRC check on PTS value */
void VDP_SetChkPTS(UINT32 u4Pts)
{
    UNUSED(u4Pts);
#ifdef VDP_B2R_PSCAN_CRC_FLOW
    _VDPSetChkPTS(u4Pts);
#endif
}

void VDP_SetEncTrigger(BOOL fgEnable)
{
    UNUSED(fgEnable);
#ifdef VDP_B2R_PSCAN_CRC_FLOW
    _VDP_SetEncTrigger(fgEnable);
#endif
}


void VDP_PowerOffB2R(void)
{
    VDP_HalPowerOffB2R();
}

void VDP_TestB2RSdPath(void)
{
    VDP_HalTestB2RSdPath();
}

#ifdef CC_B2R_SKYPE_SUPPORT
void VDP_GetYCStart(UCHAR ucVdpId,UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive)
{
    VDP_HalGetYCStart(ucVdpId,pu4YStart, pu4CStart, pu4LineSize, pu4VActive);
}
#else
void VDP_GetYCStart(UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive)
{
    VDP_HalGetYCStart(pu4YStart, pu4CStart, pu4LineSize, pu4VActive);
}
#endif

void VDP_B2RDumpRegister(BOOL fgOn)
{
    VDP_EnableDumpRegister(fgOn);
}

UCHAR VDP_GetPlayMode(UCHAR ucVdpId)
{
    UCHAR ucPlayMode;
    UCHAR ucEsId;
    UCHAR ucFbgId;

    VERIFY_VDP_ID(ucVdpId);

    ucEsId = _prVdpConf[ucVdpId]->ucInputPort[0];
    if (ucEsId >= MAX_ES_NS)
    {
        return VDP_SET_ERROR;
    }
    ucFbgId = _arEs2Fbg[ucEsId];

//    VDP_MUTEX_LOCK;
// This will cause Assertion fails at x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK?
    FBM_GetPlayMode(ucFbgId, &ucPlayMode);

//    VDP_MUTEX_UNLOCK;
    return ucPlayMode;
}

UCHAR VDP_SetThumbnailMode(UCHAR ucVdpId, UCHAR ucSet)
{
    VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

#if 1
    if (_ucThumbnailMode != ucSet)
    {
        _ucThumbnailMode = ucSet;
        LOG(5, "Thumbnail Mode (%d)\n", _ucThumbnailMode);
    }
#else
    if (ucSet)
    {
        SRM_SetMmMode(SRM_MM_MODE_TYPE_THUMBNAIL);
    }
    else
    {
        SRM_SetMmMode(SRM_MM_MODE_TYPE_VIDEO);
    }
    _ucThumbnailMode = ucSet;
#endif

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

#ifdef CC_B2R_SKYPE_SUPPORT
UINT32 VDP_GetB2RFifoStatus(UCHAR ucVdpId)
{
    UINT32 u4Status;

    u4Status = VDP_HalGetB2RFifoStatus(ucVdpId);
    return u4Status;
}

void VDP_ResetB2RFifoStatus(UCHAR ucVdpId)
{
    VDP_HalResetB2RFifoStatus(ucVdpId);
}
#else
UINT32 VDP_GetB2RFifoStatus(void)
{
    UINT32 u4Status;

    u4Status = VDP_HalGetB2RFifoStatus();
    return u4Status;
}

void VDP_ResetB2RFifoStatus(void)
{
    VDP_HalResetB2RFifoStatus();
}
#endif

void B2R_InitVdpConf(void)
{
    UCHAR i;

    _ucThumbnailMode = 0;

    for (i=0; i<VDP_NS; i++)
    {
        _prVdpConf[i] = VDP_GetConf(i);
        _prVdpPrm[i] = _VDP_GetVdpPrm(i);
    }

    /* Digital decoder variable-dram-size notification */
    if (_hB2RVdecConfMutex == (HANDLE_T)(NULL))
    {
        VERIFY (x_sema_create(&_hB2RVdecConfMutex, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }
    SRM_RegisterCB(SRM_MMBUFCHG_CTRL, _B2RSrmVDecMemConfCb);
}

void B2R_SetImgConnect(UCHAR ucVdpId, UCHAR ucConnect)
{
    _aucImageConnected[ucVdpId] = ucConnect;
}

UCHAR B2R_GetImgConnect(UCHAR ucVdpId)
{
    return _aucImageConnected[ucVdpId];
}

void VDP_VsyncInit(void)
{
    _VDP_VsyncInit();
}

void VDP_Image_Init(void)
{
    _VDP_Image_Init();
}

void VDP_VsyncSendCmd(UCHAR ucVdpId, UCHAR ucCmdId)
{
    _VDP_VsyncSendCmd(ucVdpId, ucCmdId);
}

void VDP_VsyncReset(UCHAR ucVdpId)
{
    _VDP_VsyncReset(ucVdpId);
}

void VDP_HalB2rSwitch(UCHAR ucVdpId, UCHAR ucB2rId)
{
    _VDP_HalB2rSwitch(ucVdpId, ucB2rId);
}

void VDP_FrcSetPtsCb(UCHAR ucVdpId, UCHAR ucPort, UINT32 u4CbPts, UINT32 u4CbArg)
{
    _VDP_FrcSetPtsCb(ucVdpId, ucPort, u4CbPts, u4CbArg);
}

void VDP_FrcSetLipSyncCb(UCHAR ucVdpId, UCHAR ucPort, UINT32 u4Thrsd, UINT32 u4FrmCnt)
{
    _VDP_FrcSetLipSyncCb(ucVdpId, ucPort, u4Thrsd, u4FrmCnt);
}

void VDP_FrcSetLipSyncNGOKCb(UCHAR ucNgOk, UCHAR ucRemove, UINT32 u4Thrsd, UINT32 u4FrmCnt)
{
    _VDP_FrcSetLipSyncNGOKCb(ucNgOk, ucRemove, u4Thrsd, u4FrmCnt);
}

UINT32 VDP_TrickModeSkipFrameNo(UCHAR u1B2rId)
{
    UINT32 u4B2RSkipFrameNo = 0;
    u4B2RSkipFrameNo = _VDP_TrickModeSkipFrameNo(u1B2rId);
    return u4B2RSkipFrameNo;
}

UINT32 VDP_GetFrameRate(UCHAR u1B2rId)
{
    if(u1B2rId >= B2R_NS)
    {
        ASSERT(0);
        return 30;
    }

    if(_prVdpPrm[u1B2rId])
    {
        return _prVdpPrm[u1B2rId]->rPortPrm.ucInFrameRate;
    }
    return 30;
}

//void VDP_SetCb
void VDP_SetCbAttachedSrcId(UCHAR u1B2rId, UINT32 u4AttachId)
{
    if(u1B2rId >= B2R_NS)
    {
        ASSERT(0);
        return;
    }
    if(_prVdpPrm[u1B2rId])
    {
        _prVdpPrm[u1B2rId]->rPortPrm.u4AttachedSrcId = u4AttachId;
    }
}

UINT32 VDP_Set3DInfo(UCHAR u1B2rId,UINT8 u13DType,BOOL fg3DEnable)
{
    _VDP_Set3DInfo(u1B2rId,u13DType,fg3DEnable);

    return VDP_SET_OK;
}

BOOL VDP_IsStartToPlay(UCHAR u1B2rId)
{
    return _VDP_IsStartToPlay(u1B2rId);
}

#if 1
BOOL VDP_SendB2RChgFrameMsg(VDP_B2R_CHG_FRAME_MSG_T* prMsg)
{
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
    VDP_B2R_CHG_FRAME_MSG_T rMsg;
    rMsg.u4VdpId = prMsg->u4VdpId;
    return _VDP_SendB2RChgFrameMsg(&rMsg);
#else
    return TRUE;
#endif
}

BOOL VDP_FlushB2RChgFrameMsg(void)
{
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
    return _VDP_FlushB2RChgFrameMsg();
#else
    return TRUE;
#endif
}
#endif

#ifdef CC_FAST_INIT
void VDP_pm_suspend(void)
{
}


void VDP_pm_resume(void)
{
    BIM_EnableIrq(VECTOR_DISPLAY);
}
#endif


BOOL B2R_CkgenOnOff(BOOL fgEnable)
{
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#define BLOCK_RST_CFG0 0x270
#define B2R_RST ((UINT32)1 << 1)
#define BLOCK_CKEN_CFG0 0x278
#define B2R_CKEN ((UINT32)1 << 1)
    if (fgEnable)
    {
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~B2R_RST);
        IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) | B2R_CKEN);
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) | B2R_RST);
    }
    else
    {
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~B2R_RST);
        IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) & ~B2R_CKEN);
    }
#endif

    UNUSED(fgEnable);
    return TRUE;
}


