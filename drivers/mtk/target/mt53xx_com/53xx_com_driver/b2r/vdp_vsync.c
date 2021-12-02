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
 * $RCSfile: vdp_vsync.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_vsync.c
 *  Brief of file vdp_vsync.c.
 *  Details of file vdp_vsync.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdp_vsync.h"
#include "vdp_hal.h"
#include "vdp_frc.h"
#include "fbm_drvif.h"
#include "vdp_debug.h"
#include "b2r_drvif.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "drv_common.h"
#include "x_os.h"
#include "x_assert.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define VDP_MSG_Q_NUM           8

#define VDP_NOTIFY_Q_NUM        32

#define VDP_VSYNC_Q_FULL_ERR    8

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef UINT32 (*VDP_CMD_FUNC)(UCHAR ucVdpId);

/** Brief of VSYNC_RECORD_T
 */
typedef struct
{
    UINT32 u4VdpId;
    UINT32 u4Bottom;
    UINT32 u4IsrType;
    UINT32 u4RightView;
} VSYNC_RECORD_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define VDP_MEMSET(addr, value, len)    (VERIFY(x_memset((addr), value, len) == (addr)))


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static UINT32 _VdpCmdRst(UCHAR ucVdpId);
static UINT32 _VdpCmdEnable(UCHAR ucVdpId);
static UINT32 _VdpCmdMode(UCHAR ucVdpId);
static UINT32 _VdpCmdInput(UCHAR ucVdpId);
static UINT32 _VdpCmdOutput(UCHAR ucVdpId);
static UINT32 _VdpCmdBg(UCHAR ucVdpId);
static UINT32 _VdpCmdSrcOutRegion(UCHAR ucVdpId);
static void _B2rRoutine(void* pvArg);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static const VDP_CMD_FUNC _apfCmdHandle[] =
{
    _VdpCmdRst,
    _VdpCmdEnable,
    _VdpCmdMode,
    _VdpCmdInput,
    _VdpCmdOutput,
    _VdpCmdSrcOutRegion,
    _VdpCmdSrcOutRegion,
    _VdpCmdBg,
    _VdpCmdSrcOutRegion
};

static UCHAR _ucVdpVsyncInitiated = 0;

static HANDLE_T _hVdpVsyncMsgQ;

static HANDLE_T _hVdpNotifyMsgQ;

static VDP_CONF_T* _prVdpConf[VDP_NS];

static HANDLE_T _hDtvMutex;

static UINT32 _u4VdpVsyncOnOff[VDP_NS];     // For Video Image Module

static UINT32 _u4VdpVsyncQFullCount = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of _VdpCmdRst.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpCmdRst(UCHAR ucVdpId)
{
    UNUSED(ucVdpId);

    _VDP_HalReset();

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCmdEnable.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpCmdEnable(UCHAR ucVdpId)
{
    _VDP_HalSetEnable(ucVdpId, _prVdpConf[ucVdpId]->ucEnable);

    if (!_prVdpConf[ucVdpId]->ucEnable)
    {
        _VDP_HalForceBg(ucVdpId, 1);
    }

    return (VDP_EVENT_RECONFIG | VDP_EVENT_ENABLE_CHG);
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCmdMode.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpCmdMode(UCHAR ucVdpId)
{
    if (_prVdpConf[ucVdpId]->ucMode == VDP_MODE_BG)
    {
        if (_VDP_FrcGetRacingMode(ucVdpId) == 0)
        {
            // Can not mute at racing mode
            // B2R will stop DRAM access, and also stop sending line counter to MPEG
#if 0
            /* B2R can not ouput background at set mode,
               or PSCAN will use background and data to de-interlace. Refer to [DTV00136295] */
            _VDP_HalForceBg(ucVdpId, 1);
#endif
        }

        return 0;
    }
    else
    {
        _VDP_HalForceBg(ucVdpId, 0);
        return (VDP_EVENT_RECONFIG | VDP_EVENT_MODE_CHG);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCmdInput.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpCmdInput(UCHAR ucVdpId)
{
    UNUSED(ucVdpId);
    return VDP_EVENT_INPUT_CHG;
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCmdOutput.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpCmdOutput(UCHAR ucVdpId)
{
    UNUSED(ucVdpId);

#ifdef CC_SCPOS_EN
    // do nothing
    return 0;
#else
    return (VDP_EVENT_RECONFIG| VDP_EVENT_OUTPUT_CHG);
#endif
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCmdBg.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpCmdBg(UCHAR ucVdpId)
{
    UINT32 u4Y, u4Cb, u4Cr;
    UINT32 u4A, u4B, u4C;
    UINT32 u4YCbCr;

    u4A = (_prVdpConf[ucVdpId]->u4BgColor >> 16) & 0xFF;
    u4B= (_prVdpConf[ucVdpId]->u4BgColor >> 8) & 0xFF;
    u4C = (_prVdpConf[ucVdpId]->u4BgColor) & 0xFF;

    u4Y = (16 + (((u4A * 66) + (u4B * 129) + (u4C * 25)) / 255)) & 0xFF;
    u4Cb = ((((128 * 255) + (u4C * 112)) - ((u4A * 38) + (u4B * 74)) ) / 255) & 0xFF;
    u4Cr = ((((128 * 255) + (u4A * 112)) - ((u4B * 94) + (u4C * 18))) / 255) & 0xFF;

    u4YCbCr = ((u4Y << 16) + (u4Cb << 8) + (u4Cr));

    _VDP_HalSetBg(ucVdpId, u4YCbCr);

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCmdSrcOutRegion.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpCmdSrcOutRegion(UCHAR ucVdpId)
{
    // B2R width issue
    ASSERT(_prVdpConf[ucVdpId]->rOutInfo.u4Width >= _prVdpConf[ucVdpId]->u4SrcWidth);

    // Work-Around # 1, B2R/PSW Width 64 byte alignment
    /* 20081127 Pibben: Why we need to use rOutInfo.u4Width/u4Height to set H/V active? */
    _VDP_HalSetSrcSize(ucVdpId,
//        _prVdpConf[ucVdpId]->rOutInfo.u4Width,
                       _prVdpConf[ucVdpId]->u4SrcWidth,
//        _prVdpConf[ucVdpId]->rOutInfo.u4Height,
                       _prVdpConf[ucVdpId]->u4SrcHeight,
                       _prVdpConf[ucVdpId]->u4SrcLineSize);

    _VDP_HalSetOutSize(ucVdpId,
                       _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth,
                       _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight,
                       !(_prVdpConf[ucVdpId]->rOutInfo.ucPrg),
                       _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk,
                       _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode);

    _VDP_VsyncNotify(VDP_MSG_NOTIFY, (UINT32)ucVdpId, VDP_B2R_SOURCE_CHG, 0);

    return VDP_EVENT_RECONFIG;
}

//-----------------------------------------------------------------------------
/** Brief of _B2rRoutine.
 */
//-----------------------------------------------------------------------------
static void _B2rRoutine(void* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        VSYNC_RECORD_T rMsg;
        UINT16 u2MsgQIdx;
        SIZE_T zMsgSize;

        UINT32 u4VdpIdx;

        // wait message
        zMsgSize = sizeof(VSYNC_RECORD_T);
        VERIFY(x_msg_q_receive(&u2MsgQIdx, &rMsg, &zMsgSize, &_hVdpVsyncMsgQ, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);

//        _VDP_HalConfig(0);

        u4VdpIdx = rMsg.u4VdpId;
        if ((u4VdpIdx < VDP_NS) && (_u4VdpVsyncOnOff[u4VdpIdx] != 0))
        {
            _u4VdpVsyncQFullCount = 0;

//            LOG(3, "VSYNC T %d 0x%x 0x%x\n", rMsg.u4VdpId, rMsg.u4Bottom, rMsg.u4IsrType);

            if (rMsg.u4IsrType & (VDP_ISR_ADI))
            {
                // Normal ISR

                VDP_CB_MSG_T rNotifyMsg;
                UINT32 u4Event;

                VERIFY(x_sema_lock(_hDtvMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

                // Frame Rate Control Process
                u4Event = _VDP_FrcProc((UCHAR)u4VdpIdx, (UCHAR)(rMsg.u4Bottom),(UCHAR)(rMsg.u4RightView));

                // Seqeunce Change
                if (u4Event & VDP_EVENT_SEQ_CHG)
                {
                    LOG(7, "VDP_EVENT_SEQ_CHG \n");
                    u4Event |= _VdpCmdSrcOutRegion((UCHAR)u4VdpIdx);
                }

                // Optional Seqeunce Change
                if (u4Event & VDP_EVENT_OPT_SEQ_CHG)
                {
                    //// TODO, notify middleware (aspect ratio, afd, ...)
                }

                // Re-config Hardware
                if (u4Event & VDP_EVENT_RECONFIG)
                {
                    LOG(7, "VDP_EVENT_RECONFIG \n");
                    _VDP_HalConfig((UCHAR)u4VdpIdx);
                }

                _VDP_HalIsrFin((UCHAR)u4VdpIdx);

                VERIFY(x_sema_unlock(_hDtvMutex) == OSR_OK);

                // Notify (Upper Layer, and Main Loop) at non-critical session
                u4Event = 0;
                zMsgSize = sizeof(VDP_CB_MSG_T);
                while (x_msg_q_receive(&u2MsgQIdx, &rNotifyMsg, &zMsgSize, &_hVdpNotifyMsgQ, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
                {
                    LOG(11, "Notify(%d) (%d, %d, %d, %d)\n", u4Event, rNotifyMsg.u4Type, rNotifyMsg.u4Arg1, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                    u4Event++;

                    VERIFY(x_sema_lock(_prVdpConf[u4VdpIdx]->hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

                    if (rNotifyMsg.u4Type == VDP_MSG_NOTIFY)
                    {
                        if (_prVdpConf[VDP_1]->ucB2rId == _prVdpConf[VDP_2]->ucB2rId)
                        {
#if 0 //When TV scart out and Main is DTV, we use Main to play DTV. Below copy is not needed
                            _prVdpConf[VDP_1]->u4AspectRatio = _prVdpConf[VDP_2]->u4AspectRatio;
                            _prVdpConf[VDP_1]->u4Afd = _prVdpConf[VDP_2]->u4Afd;
#endif

                            _VDP_StatusNofify((UCHAR)VDP_1, rNotifyMsg.u4Arg2);
                            _VDP_StatusNofify((UCHAR)VDP_2, rNotifyMsg.u4Arg2);
                        }
                        else
                        {
                            _VDP_StatusNofify((UCHAR)rNotifyMsg.u4Arg1, rNotifyMsg.u4Arg2);
                        }
                    }
                    else if (rNotifyMsg.u4Type == VDP_MSG_CC_CB)
                    {
                        if (_prVdpConf[VDP_1]->ucB2rId == _prVdpConf[VDP_2]->ucB2rId)
                        {
                            _VDP_EventNofify((UCHAR)VDP_1, (UINT32)VDP_CB_FUNC_PTS_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                            _VDP_EventNofify((UCHAR)VDP_2, (UINT32)VDP_CB_FUNC_PTS_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                        }
                        else
                        {
                            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_PTS_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                        }
                    }
                    else if (rNotifyMsg.u4Type == VDP_MSG_AFD_CB)
                    {
                        if (_prVdpConf[VDP_1]->ucB2rId == _prVdpConf[VDP_2]->ucB2rId)
                        {
#if 1 //When TV scart out and Main is DTV, we use Main to play DTV.
                            _prVdpConf[1-((UCHAR)rNotifyMsg.u4Arg1)]->u4AspectRatio = _prVdpConf[(UCHAR)rNotifyMsg.u4Arg1]->u4AspectRatio;
                            _prVdpConf[1-((UCHAR)rNotifyMsg.u4Arg1)]->u4Afd = _prVdpConf[(UCHAR)rNotifyMsg.u4Arg1]->u4Afd;
#endif

                            _VDP_EventNofify((UCHAR)VDP_1, (UINT32)VDP_CB_FUNC_AFD_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                            _VDP_EventNofify((UCHAR)VDP_2, (UINT32)VDP_CB_FUNC_AFD_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                            _VDP_EventNofify((UCHAR)VDP_1, (UINT32)VDP_CB_FUNC_AFD_IND_2, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                            _VDP_EventNofify((UCHAR)VDP_2, (UINT32)VDP_CB_FUNC_AFD_IND_2, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                        }
                        else
                        {
                            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_AFD_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_AFD_IND_2, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                        }
                    }
                    else if (rNotifyMsg.u4Type == VDP_MSG_UNMUTE_CB)
                    {
                        _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_UNMUTE_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                    }
                    else if (rNotifyMsg.u4Type == VDP_MSG_MM_CB)
                    {
                        _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_MM_COND_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                    }
                    else if (rNotifyMsg.u4Type == VDP_MSG_LIPSYNC_CB)
                    {
                        _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_LIPSYNC_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                    }
                    else if (rNotifyMsg.u4Type == VDP_MSG_MM_STEP_FIN_CB)
                    {
                        _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_MM_STEP_FIN_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                    }
                    else if (rNotifyMsg.u4Type == VDP_MSG_MM_SEEK_FIN_CB)
                    {
                        _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_MM_SEEK_FIN_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                    }
                    else if (rNotifyMsg.u4Type == VDP_MSG_REPEAT_DROP_CB)
                    {
                        _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_REPEAT_DROP_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                    }
                    VERIFY(x_sema_unlock(_prVdpConf[u4VdpIdx]->hMutex) == OSR_OK);
                }
            }
            else if (rMsg.u4IsrType & (VDP_ISR_VEI))
            {
                // Racing
                VERIFY(x_sema_lock(_hDtvMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
                _VDP_FrcProcEarly((UCHAR)u4VdpIdx, (UCHAR)(rMsg.u4Bottom));
                VERIFY(x_sema_unlock(_hDtvMutex) == OSR_OK);
            }
            else if (rMsg.u4IsrType & (VDP_ISR_FI))
            {
                ASSERT(0);
            }
        }
    }
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of _VDP_VsyncInit.
 */
//-----------------------------------------------------------------------------
void _VDP_VsyncInit(void)
{
    if (_ucVdpVsyncInitiated == 0)
    {
        HANDLE_T hThread1;
        UINT32 u4VdpIdx;

        _ucVdpVsyncInitiated = 1;

        for (u4VdpIdx = 0; u4VdpIdx < VDP_NS; u4VdpIdx++)
        {
            _prVdpConf[u4VdpIdx] = VDP_GetConf((UCHAR)u4VdpIdx);

            _u4VdpVsyncOnOff[u4VdpIdx] = 1;
        }

        // create semaphore
        VERIFY(x_sema_create(&_hDtvMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

        // create Message Queue (VSYNC)
        VERIFY(x_msg_q_create(&_hVdpVsyncMsgQ, "VSYNCQ", sizeof(VSYNC_RECORD_T), VDP_MSG_Q_NUM) == OSR_OK);

        // create Message Queue (NOTIFY)
        VERIFY(x_msg_q_create(&_hVdpNotifyMsgQ, "VDP-NOTIFY", sizeof(VDP_CB_MSG_T), VDP_NOTIFY_Q_NUM) == OSR_OK);

        _VDP_FrcInit();
        _VDP_HalInit();

        // create thread
        if (x_thread_create(&hThread1, VSYNC_THREAD_NAME, VSYNC_STACK_SIZE, VSYNC_THREAD_PRIORITY,
                            _B2rRoutine, 0, NULL) != OSR_OK)
        {
            ASSERT(0);
        }
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_VsyncReset.
 */
//-----------------------------------------------------------------------------
void _VDP_VsyncReset(UCHAR ucVdpId)
{
    UNUSED(ucVdpId);
    _VDP_HalReset();
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_VsyncOnOff.
 */
//-----------------------------------------------------------------------------
void _VDP_VsyncOnOff(UCHAR ucVdpId, UINT32 u4OnOff)
{
    if (ucVdpId < VDP_NS)
    {
        _u4VdpVsyncOnOff[ucVdpId] = u4OnOff;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_VsyncSendCmd.
 */
//-----------------------------------------------------------------------------
void _VDP_VsyncSendCmd(UCHAR ucVdpId, UCHAR ucCmdId)
{
    // handle API command immediately
    UINT32 u4Event;

    if ((ucCmdId < VDP_CMD_MAX) && (ucVdpId < VDP_NS) && (_u4VdpVsyncOnOff[ucVdpId] != 0))
    {
        VERIFY(x_sema_lock(_hDtvMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        u4Event = _apfCmdHandle[ucCmdId](ucVdpId);

        if (u4Event)
        {
            u4Event |= _VDP_FrcHandlerEvent(ucVdpId, u4Event);
        }

        VERIFY(x_sema_unlock(_hDtvMutex) == OSR_OK);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_VsyncNotify.
 */
//-----------------------------------------------------------------------------
void _VDP_VsyncNotify(UINT32 u4Type, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3)
{
    VDP_CB_MSG_T rMsg;

    rMsg.u4Type = u4Type;
    rMsg.u4Arg1 = u4Arg1;
    rMsg.u4Arg2 = u4Arg2;
    rMsg.u4Arg3 = u4Arg3;

    if (u4Arg1 >= VDP_NS)
    {
        return;
    }

    if (_prVdpConf[u4Arg1]->ucEnable == 0)
    {
        if ((u4Type == VDP_MSG_UNMUTE_CB) ||
                (u4Type == VDP_MSG_CC_CB) ||
                (u4Type == VDP_MSG_AFD_CB) ||
                (u4Type == VDP_MSG_LIPSYNC_CB) ||
                (u4Type == VDP_MSG_REPEAT_DROP_CB))
        {
            // skip redundant notify if video plane not enable
            return;
        }
    }

    if (x_msg_q_send(_hVdpNotifyMsgQ, (void *)(&rMsg), sizeof(VDP_CB_MSG_T), 0) != OSR_OK)
    {
        LOG(5, "DTV NOTIFY-Q FULL\n");
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_VsyncTick.
 */
//-----------------------------------------------------------------------------
void _VDP_VsyncTick(UCHAR ucVdpId, UINT32 u4Bottom, UINT32 u4RightView,UINT32 u4IsrType)
{
    VSYNC_RECORD_T rMsg;

    rMsg.u4VdpId = (UINT32) ucVdpId;
    rMsg.u4Bottom = u4Bottom;
    rMsg.u4IsrType = u4IsrType;
    rMsg.u4RightView = u4RightView;

    // send message
    if (x_msg_q_send(_hVdpVsyncMsgQ, (void *)(&rMsg), sizeof(VSYNC_RECORD_T), 0) != OSR_OK)
    {
        if (++_u4VdpVsyncQFullCount < VDP_VSYNC_Q_FULL_ERR)
        {
            LOG(5, "DTV VSYNCQ FULL\n");
        }
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_VsyncMutexLock.
 */
//-----------------------------------------------------------------------------
void _VDP_VsyncMutexLock(void)
{
    VERIFY(x_sema_lock(_hDtvMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_VsyncMutexLock.
 */
//-----------------------------------------------------------------------------
void _VDP_VsyncMutexUnlock(void)
{
    VERIFY(x_sema_unlock(_hDtvMutex) == OSR_OK);
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


