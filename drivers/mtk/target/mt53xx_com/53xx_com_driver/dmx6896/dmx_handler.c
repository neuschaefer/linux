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
 * $Author: weibo.li $
 * $Date: 2012/09/09 $
 * $RCSfile: dmx_handler.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_handler.c
 *  Demux driver - decoder interfaces
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "dmx_if.h"

LINT_EXT_HEADER_BEGIN

#include "vdec_if.h"
#include "mpv_drvif.h"
#include "aud_drvif.h"
#include "drv_common.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_hal_arm.h"

LINT_EXT_HEADER_END


#ifdef TIME_MEASUREMENT
LINT_EXT_HEADER_BEGIN
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
LINT_EXT_HEADER_END
extern BOOL _fgWaitForFirstI;   // Measure the time from "Enabling PID" to the
                                // moment "the first I frame is received".
#endif  // TIME_MEASUREMENT


#if defined(CC_DMX_FRAMER_MONITOR) && !defined(CC_DMX_ENABLE_MONTHREAD)
#error "Defein CC_DMX_FRAMER_MONITOR but not define CC_DMX_ENABLE_MONTHREAD.\n"
#endif // CC_DMX_FRAMER_MONITOR

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// Demux queue and thread paramenters
//
#define DMX_QUEUE_NAME                  "DMX"
#define DMX_PURGE_QUEUE_NAME            "PURGE"
#define DMX_PURGE_THREAD_NAME           "PURGETHREAD"
#define DMX_QUEUE_SIZE                  1024//32

#ifdef CC_DMX_ENABLE_MONTHREAD
#define DMX_MON_THREAD_NAME             "DMXMONTH"
#define DMX_MON_THREAD_STACK_SIZE       1024
#define DMX_MON_THREAD_PRIORITY         100
#define DMX_MON_TIME_PERIOD             100         //  Monitor period
#define DMX_MON_RCV_THRESHOLD           50
#define DMX_MON_DROP_THRESHOLD          100
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// Picture queue for MPEG decoder
static HANDLE_T _hDmxQueue = NULL_HANDLE;
static HANDLE_T _hPurgeQueue = NULL_HANDLE;

/// Picture thread
static HANDLE_T _hDmxThread = NULL_HANDLE;
static HANDLE_T _hPurgeThread = NULL_HANDLE;

#ifdef CC_DMX_ENABLE_MONTHREAD
/// Monitor thread
static HANDLE_T _hDmxMonThread = NULL_HANDLE;
#endif  // CC_DMX_ENABLE_MONTHREAD

/// Decoder calback functions
static DMX_DECODER_CALLBACKS_T _rDecoderCallbacks;

/// Sequence header and GOP flags
static BOOL _afgSeqHeader[DMX_NUM_PID_INDEX];
static UINT32 _au4SeqHeaderAddr[DMX_NUM_PID_INDEX];
static BOOL _afgSeqHeaderPtsDts[DMX_NUM_PID_INDEX];
static UINT32 _au4SeqHeaderPts[DMX_NUM_PID_INDEX];
static UINT32 _au4SeqHeaderDts[DMX_NUM_PID_INDEX];
static BOOL _afgGop[DMX_NUM_PID_INDEX];
static UINT32 _au4GopAddr[DMX_NUM_PID_INDEX];
static BOOL _afgGopPtsDts[DMX_NUM_PID_INDEX];
static UINT32 _au4GopPts[DMX_NUM_PID_INDEX];
static UINT32 _au4GopDts[DMX_NUM_PID_INDEX];
static BOOL _afgSeqEnd[DMX_NUM_PID_INDEX];
static BOOL _afgSeqEndPtsDts[DMX_NUM_PID_INDEX];
static UINT32 _au4SeqEndPts[DMX_NUM_PID_INDEX];
static UINT32 _au4SeqEndDts[DMX_NUM_PID_INDEX];

#ifdef TRACK_LAST_FRAME
/// Last sent frame (video picture or audio frame)
static UINT32 _au4LastFrame[DMX_NUM_PID_INDEX];
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifdef TRACK_LAST_FRAME

//-----------------------------------------------------------------------------
/** _DmxGetLastFrame
 *  Get last sent frame
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval The last frame
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxGetLastFrame(UINT8 u1Pidx)
{
    CRIT_STATE_T rState;
    UINT32 u4LastFrame;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    rState = x_crit_start();
    u4LastFrame = _au4LastFrame[u1Pidx];
    x_crit_end(rState);

    return u4LastFrame;
}


//-----------------------------------------------------------------------------
/** _DmxSetLastFrame
 *  Set last frame of a given PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4LastFrame     The new last frame
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxSetLastFrame(UINT8 u1Pidx, UINT32 u4LastFrame)
{
    CRIT_STATE_T rState;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    rState = x_crit_start();
    _au4LastFrame[u1Pidx] = u4LastFrame;
    x_crit_end(rState);
}

#endif


//-----------------------------------------------------------------------------
/** _DmxSendVideoPes
 *  Virtual video PES handler, debug only
 *
 *  @param  prPesInfo       The PES info structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxSendVideoPes(const DMX_PES_MSG_T* prPes)
{

    UINT32 u4Wp, u4Pic;
    UINT8 u1Pidx;
    PID_STRUCT_T* prPidStruct;

    ASSERT(prPes != NULL);

    u1Pidx = prPes->u1Pidx;
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    u4Wp = prPidStruct->u4Wp;
    u4Pic = prPes->u4FrameAddr;

    // Debug
    LOG(7, "Video PES: pidx = %u, pic = 0x%08x, type = %u\n",
        u1Pidx, u4Pic, prPes->u4FrameType);

    // Just ack the picture
    VERIFY(_DMX_UpdateReadPointer(u1Pidx, u4Wp, u4Pic));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxUpdateVideoWp
 *  Update video write pointer, debug only
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Wp            Write pointer
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxUpdateVideoWp(UINT8 u1Pidx, UINT32 u4Wp)
{
    UNUSED(u1Pidx);
    UNUSED(u4Wp);
}


//-----------------------------------------------------------------------------
/** _DmxSendAudioPes
 *  Virtual audio handler, debug only
 *
 *  @param  prPes           PES structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxSendAudioPes(const DMX_AUDIO_PES_T* prPes)
{
    ASSERT(prPes != NULL);

    LOG(7, "Audio PES: pidx: %u, WP: 0x%08x, RP: 0x%08x\n", prPes->u1PidIndex,
        prPes->u4Wp, prPes->u4Wp);      // FIXME: Get real RP

    // Ack the frame directly
    VERIFY(_DMX_UpdateReadPointer(prPes->u1PidIndex, prPes->u4Wp, prPes->u4Wp));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxDispatchPesMessage
 *  Dispatch PES message to corresponding decoders
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxDispatchPesMessage(const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet = FALSE;
    PID_STRUCT_T* prPidStruct;

    ASSERT(prPes != NULL);

    prPidStruct = _DMX_GetPidStruct(prPes->u1Pidx);
    if(prPidStruct == NULL)
    {
        return FALSE;
    }

    #ifndef CC_DMX_FLUSH_ALL
    if(prPes->eVideoType != DMX_VIDEO_H264)
    {
        HalFlushInvalidateDCacheMultipleLine(prPes->u4BufStart, prPes->u4BufEnd - prPes->u4BufStart);
    }
    #endif

#ifdef TIME_MEASUREMENT
    if ((_fgWaitForFirstI == TRUE) && (prPes->u4FrameType == PIC_TYPE_I))
    {
        TMS_END_EX(TMS_FLAG_CHG_CHL, "DMX_FIRST_I", "DMX_First_I");
        _fgWaitForFirstI = FALSE;
    }

    if ((_fgWaitForFirstI == TRUE) && (prPes->u4FrameType == H264_PIC_TYPE_SEQ))
    {
        // Normally, the I frame appears right after SEQ.
        TMS_END_EX(TMS_FLAG_CHG_CHL, "DMX_FIRST_I", "DMX_First_SEQ");
        _fgWaitForFirstI = FALSE;
    }
#endif  // TIME_MEASUREMENT

    switch (prPes->ePidType)
    {
    case DMX_PID_TYPE_ES_VIDEO:
        // Fixme
        if(prPidStruct->eInputType == DMX_IN_BROADCAST_TS)
        {
            if (_rDecoderCallbacks.pfnSendVideoPes != NULL)
            {
                if (_rDecoderCallbacks.pfnSendVideoPes(prPes))
                {
                    // Send successful
                    fgRet = TRUE;
                }
                else
                {
                    // Queue full
                    LOG(3, "Failed to send video PES!\n");
                }
            }
        }
#ifdef ENABLE_MULTIMEDIA
        else
        {
            if(_DMX_MUL_GetMultipleInstEnabled())
            {
                DMX_DECODER_CALLBACKS_T *prCallback;

                prCallback = _DMX_MUL_GetPidxDecoderCallback(prPes->u1Pidx);
                if((prCallback != NULL) && (prCallback->pfnSendVideoPes != NULL))
                {
                    if (prCallback->pfnSendVideoPes(prPes))
                    {
                        // Send successful
                        fgRet = TRUE;
                    }
                    else
                    {
                        // Queue full
                        LOG(3, "Failed to send video PES!\n");
                    }
                }
            }
            else
            {
                if (_rDecoderCallbacks.pfnSendVideoPes != NULL)
                {
                    if (_rDecoderCallbacks.pfnSendVideoPes(prPes))
                    {
                        // Send successful
                        fgRet = TRUE;
                    }
                    else
                    {
                        // Queue full
                        LOG(3, "Failed to send video PES!\n");
                    }
                }
            }
        }
#endif // ENABLE_MULTIMEDIA
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        // TBD
        break;

    default:
        // Unknown type
        ASSERT(FALSE);
        break;
    }

#ifdef TRACK_LAST_FRAME

    // Record last sent frame
    if (fgRet)
    {
        _DmxSetLastFrame(prPes->u1Pidx, prPes->uFrameAddr);
    }

#endif

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DmxDiscardFifoData
 *  Discard all data in FIFO
 *
 *  @param  u1Pidx          PID index
 *  @param  fgReenable      Re-enable PID
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxDiscardFifoData(UINT8 u1Pidx, BOOL fgReenable)
{
    PID_STRUCT_T* prPidStruct;
    UINT8 u1Channel;

    LOG(6, "Pidx %u: discard fifo data!\n", u1Pidx);

    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if (prPidStruct->fgEnable)
    {
        LOG(5, "Ignore discarding FIFO data of an active pidx %u\n", u1Pidx);
        return FALSE;
    }

    // Flush decoder
    switch (prPidStruct->ePidType)
    {
    case DMX_PID_TYPE_ES_VIDEO:
        if (_DMX_GetVideoChannel(u1Pidx, &u1Channel))
        {
            MPV_FlushEsmQ(u1Channel, FALSE);
        }
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        AUD_WaitDspFlush(prPidStruct->u1DeviceId);
        break;

    default:
        LOG(5, "Pidx %u discard fifo data: unknown pid type %u\n",
            u1Pidx, (UINT32)prPidStruct->ePidType);
        break;
    }

#ifdef TRACK_LAST_FRAME

    // Reset last-sent frame record
    _DmxSetLastFrame(u1Pidx, 0);

#endif

    if (fgReenable)
    {
        DMX_PID_T rPid;

        // Reset FIFO by simply re-enable PID
        rPid.fgEnable = TRUE;
        VERIFY(_DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid, TRUE));
    }
    else
    {
        if (!_DMX_ResetPidBuffer(u1Pidx, prPidStruct, FALSE))
        {
            return FALSE;
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxThread
 *  The picture thread
 *
 *  @param  prArg           The thread argument
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _DmxThread(VOID* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        static DMX_PES_MSG_T rPes;
        PID_STRUCT_T* prPidStruct;
        INT32 i4Ret;
        SIZE_T zMsgSize;
        UINT16 u2QueueIndex;
#if 0
        // Debug
        while (1)
        {
            UINT16 u2MsgNum;

            VERIFY(x_msg_q_num_msgs(_hDmxQueue, &u2MsgNum) == OSR_OK);
            if (u2MsgNum > 4)
            {
                break;
            }

            x_thread_delay(10);
        }
#endif

        zMsgSize = sizeof (rPes);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rPes, &zMsgSize,
            &_hDmxQueue, 1, X_MSGQ_OPTION_WAIT);

        // Inhibit API function calls during processing message
        _DMX_LockApi();

        if (i4Ret != OSR_OK)
        {
            _DMX_UnlockApi();
            LOG(1, "Error on receiving picture message!\n");
            continue;
        }

        prPidStruct = _DMX_GetPidStruct(rPes.u1Pidx);
        if (rPes.u1SerialNumber != prPidStruct->u1SerialNumber)
        {
            // Overdue message, just discard it
            _DMX_UnlockApi();
            LOG(3, "Overdue message of pidx %u!\n", rPes.u1Pidx);
            continue;
        }

        ASSERT(zMsgSize == sizeof (rPes));
        ASSERT(u2QueueIndex == 0);

        switch (rPes.eMsgType)
        {
        case DMX_PES_MSG_TYPE_PES:
            UNUSED(_DmxDispatchPesMessage(&rPes));
            break;

        default:
            // Unknown message type
            ASSERT(FALSE);
            break;
        }   // switch (rPes.eMsgType)

        // Resume API calls
        _DMX_UnlockApi();

    }   // while (1)
}

//-----------------------------------------------------------------------------
/** _PurgeThread
 *  The purge thread
 *
 *  @param  prArg           The thread argument
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _PurgeThread(VOID* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        static DMX_PES_MSG_T rPes;
        PID_STRUCT_T* prPidStruct;
        INT32 i4Ret;
        SIZE_T zMsgSize;
        UINT16 u2QueueIndex;

        zMsgSize = sizeof (rPes);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rPes, &zMsgSize,
            &_hPurgeQueue, 1, X_MSGQ_OPTION_WAIT);

        // Inhibit API function calls during processing message
        _DMX_LockApi();

        if (i4Ret != OSR_OK)
        {
            _DMX_UnlockApi();
            LOG(1, "Error on receiving picture message!\n");
            continue;
        }

        // Use Dmxthread to handle error case
        if(rPes.eMsgType == DMX_PES_MSG_TYPE_ERR_HANDLE)
        {
            _DMX_PanicReset();
            _DMX_UnlockApi();
            continue;
        }

        prPidStruct = _DMX_GetPidStruct(rPes.u1Pidx);
        if (rPes.u1SerialNumber != prPidStruct->u1SerialNumber)
        {
            // Overdue message, just discard it
            _DMX_UnlockApi();
            LOG(3, "Overdue message of pidx %u!\n", rPes.u1Pidx);
            continue;
        }

        ASSERT(zMsgSize == sizeof (rPes));
        ASSERT(u2QueueIndex == 0);

        switch (rPes.eMsgType)
        {
        case DMX_PES_MSG_TYPE_PURGE:
            UNUSED(_DmxDiscardFifoData(rPes.u1Pidx, FALSE));
            break;

        case DMX_PES_MSG_TYPE_PURGE_REENABLE:
            UNUSED(_DmxDiscardFifoData(rPes.u1Pidx, TRUE));
            break;

        default:
            // Unknown message type
            ASSERT(FALSE);
            break;
        }   // switch (rPes.eMsgType)

        // Resume API calls
        _DMX_UnlockApi();

    }   // while (1)
}

#ifdef CC_DMX_ENABLE_MONTHREAD

#ifdef CC_DMX_FRAMER_MONITOR
//-----------------------------------------------------------------------------
/** _DmxMonPktDiff
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxMonPktDiff(UINT32 u4New, UINT32 u4Old)
{
    UINT32 u4Diff;

    if(u4New < u4Old)
    {
        u4Diff = (u4New + 0x10000) - u4Old;
    }
    else
    {
        u4Diff = u4New - u4Old;
    }

    return u4Diff;
}


//-----------------------------------------------------------------------------
/** _DmxMonFramer
 */
//-----------------------------------------------------------------------------
static VOID _DmxMonFramer(void)
{
    static UINT32 _u4PreRcvPtkCount = 0;
    static UINT32 _u4PreDropPktCount = 0;
    static UINT32 _u4FramerCount = 0;
    UINT32 u4RcvPtkCount, u4DropPktCount;
    UINT32 u4PcvPktDiff, u4DropPktDiff;
    PID_STRUCT_T* prPidStruct;
    UINT32 i, u4Control;
    BOOL fgPidEnable;
    UINT8 u1FramerIdx;

    if(!_DMX_GetIsTSEnabled())
    {
        _u4FramerCount = 0;
        return;
    }

    if (_DMX_IsMicroProcessorStopped())
    {
        _u4FramerCount = 0;
        return;
    }

    fgPidEnable = FALSE;
    _DMX_Lock();
    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        prPidStruct = _DMX_GetPidStruct(i);
        if(prPidStruct->fgEnable)
        {
            fgPidEnable = TRUE;
            break;
        }
    }
    _DMX_Unlock();

    if(!fgPidEnable)
    {
        _u4FramerCount = 0;
        return;
    }

    u1FramerIdx = _DMX_GetFramerIndex();
    u4Control = DMXCMD_READ32(DMX_REG_FRAMER0_STATUS + u1FramerIdx);
    u4RcvPtkCount = u4Control & 0xFFFF;
    u4DropPktCount = (u4Control & 0xFFFF0000) >> 16;

    u4PcvPktDiff = _DmxMonPktDiff(u4RcvPtkCount, _u4PreRcvPtkCount);
    u4DropPktDiff = _DmxMonPktDiff(u4DropPktCount, _u4PreDropPktCount);

    //LOG(5, "%s, rcv_diff:%d, drop_diff:%d\n", u4PcvPktDiff, u4DropPktDiff);

    if((u4PcvPktDiff < DMX_MON_RCV_THRESHOLD) && (u4DropPktDiff > DMX_MON_DROP_THRESHOLD))
    {
        _u4FramerCount++;
    }
    else
    {
        _u4FramerCount = 0;
    }

    _u4PreRcvPtkCount = u4RcvPtkCount;
    _u4PreDropPktCount = u4DropPktCount;

    if(_u4FramerCount > 3)
    {
        LOG(1, "Framer is wrong, reset it\n");
        _DMX_ResetFramer(_DMX_GetFramerIndex());
        _u4FramerCount = 0;
    }

}
#endif // CC_DMX_FRAMER_MONITOR


//-----------------------------------------------------------------------------
/** _DmxMonMicroCode
 */
//-----------------------------------------------------------------------------
static VOID _DmxMonMicroCode(void)
{
    static UINT32 _u4StopCount = 0;
    static UINT32 _u4PreVal = 0xFFFFFFFF;
    UINT32 u4Val;

    // Pull mode
    if((DMXCMD_READ32(DMX_REG_CONFIG2) & 0xC0000000) == 0xC0000000)
    {
        _u4StopCount = 0;
        return;
    }

    if (_DMX_IsMicroProcessorStopped())
    {
        _u4StopCount = 0;
        return;
    }

    // Need to as PS
    if(!_DMX_GetIsTSEnabled())
    {
    	_u4StopCount = 0;
        return;
    }

    u4Val = MICROCODE_WATCHDOG;
    if(u4Val == _u4PreVal)
    {
        _u4StopCount++;
    }
    else
    {
        _u4StopCount = 0;
    }

    _u4PreVal = u4Val;

    if(_u4StopCount >= 5)
    {
        _u4StopCount = 0;
        _DMX_PanicMicroCode();
    }

}


//-----------------------------------------------------------------------------
/** _DmxMonThread
 *  The monitor thread
 *
 *  @param  prArg           The thread argument
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _DmxMonThread(VOID* pvArg)
{
    UNUSED(pvArg);

    while(1)
    {
        x_thread_delay(DMX_MON_TIME_PERIOD);

#ifdef CC_DMX_FRAMER_MONITOR
        _DmxMonFramer();
#endif // CC_DMX_FRAMER_MONITOR
        _DmxMonMicroCode();
    }
}
#endif  // CC_DMX_ENABLE_MONTHREAD

//-----------------------------------------------------------------------------
/** _DmxAddPictureHeader
 *  Add a picture header to picture queue
 *
 *  @param  prPes           Video PES structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxAddPictureHeader(DMX_PES_MSG_T* prPes)
{
    PID_STRUCT_T* prPidStruct;

    ASSERT(prPes != NULL);

    prPidStruct = _DMX_GetPidStruct(prPes->u1Pidx);
    prPes->u1SerialNumber = prPidStruct->u1SerialNumber;
    prPes->pvInstanceTag = prPidStruct->pvInstanceTag;

    if (x_msg_q_send(_hDmxQueue, (VOID*)prPes, sizeof (DMX_PES_MSG_T),
        DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(3, "Demux queue full!\n");
        return FALSE;
    }

    // Update picture counter
    prPidStruct->rCounters.u4PicCount++;

    // Debug
    LOG(7, "Send picture: 0x%08x, type: %u\n", prPes->u4FrameAddr,
        prPes->u4FrameType);

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


#ifdef CC_DMX_PES_EXT_BUFFER
//-----------------------------------------------------------------------------
/** _DMX_UpdateExtBufferReadPointer
 *  Update read pointer of PES extension buffer.
 *
 *  @param  u1Pidx          PID index
 *  @param  u4NewRp         The new read pointer
 *  @param  u4DataAddr      The address of the PES_extension_field data
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_UpdateExtBufferReadPointer(UINT8 u1Pidx, UINT32 u4NewRp, UINT32 u4DataAddr)
{
    UINT32 u4ExtBufStart, u4ExtBufEnd, u4ExtBufLen, u4DataSize, u4UpdateSize;
    PID_STRUCT_T* prPidStruct;
    PID_STRUCT_T rPidStruct;

    // Check if pid index is valid
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Update Ext_buf RP: invalid PID index %u\n", u1Pidx);
        return FALSE;
    }

    _DMX_Lock();
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    rPidStruct = *prPidStruct;
    _DMX_Unlock();

    // Check if RP is out of range.
    u4ExtBufStart = rPidStruct.u4ExtBufStart;
    u4ExtBufEnd = rPidStruct.u4ExtBufEnd + 1;  // !!!
    u4ExtBufLen = u4ExtBufEnd - u4ExtBufStart;
    u4NewRp = VIRTUAL(u4NewRp);
    if ((u4NewRp < u4ExtBufStart) || (u4NewRp >= u4ExtBufEnd))
    {
        // RP is out of range.
        LOG(3, "Pidx: %u, Invalid new RP: 0x%08x\n", u1Pidx, u4NewRp);
        return FALSE;
    }

    // Check update size
    u4DataSize = DATASIZE(rPidStruct.u4ExtBufRp, rPidStruct.u4ExtBufWp, u4ExtBufLen);
    u4UpdateSize = DATASIZE(rPidStruct.u4ExtBufRp, u4NewRp, u4ExtBufLen);
    if (u4UpdateSize > u4DataSize)
    {
        // Update size is larger than the data size. Something is wrong...
        LOG(3, "Pidx: %u, Invalid new RP: 0x%08x, dataSize: %u, updateSize: %u\n",
            u1Pidx, u4NewRp, u4DataSize, u4UpdateSize);
        return FALSE;
    }

    _DMX_Lock();
    prPidStruct->u4ExtBufRp = u4NewRp;
    _DMX_Unlock();

    // Debug
    LOG(7, "Update Ext RP - pidx: %u, new RP: 0x%08x, dataAddr: 0x%08x\n",
        u1Pidx, u4NewRp, u4DataAddr);

    return TRUE;
}
#endif  // CC_DMX_PES_EXT_BUFFER


//-----------------------------------------------------------------------------
/** _DMX_UpdateReadPointer
 *  Update read pointer of PID buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Rp            The new read pointer
 *  @param  u4FrameAddr     The frame (picture) address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_UpdateReadPointer(UINT8 u1Pidx, UINT32 u4Rp, UINT32 u4FrameAddr)
{
    UINT32 u4BufStart, u4BufEnd, u4DataSize, u4UpdateSize;
    PID_STRUCT_T* prPidStruct;
    PID_STRUCT_T rPidStruct;

    // Check if pid index is valid
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Update RP: invalid PID index %u\n", u1Pidx);
        return FALSE;
    }

    _DMX_Lock();

    // Check if RP out of range
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    rPidStruct = *prPidStruct;

    _DMX_Unlock();

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

    _DMX_Lock();

    PID_S_W(u1Pidx, 9) = PHYSICAL(u4Rp);
    prPidStruct->u4Rp = u4Rp;

    _DMX_Unlock();

#ifdef TRACK_LAST_FRAME

    // Clear last-sent frame record if it is
    if (_DmxGetLastFrame(u1Pidx) == u4FrameAddr)
    {
        _DmxSetLastFrame(u1Pidx, 0);
    }

#endif

    // Debug
    LOG(7, "Update RP - pidx: %u, RP: 0x%08x, frame: 0x%08x\n", u1Pidx, u4Rp,
        u4FrameAddr);

    return TRUE;
}


BOOL _DMX_UpdateReadPointer2(UINT8 u1Pidx, UINT32 u4Rp, UINT32 u4FrameAddr)
{
    PID_STRUCT_T* prPidStruct;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    _DMX_Lock();

    prPidStruct->u4Rp = u4Rp;
    PID_S_W(u1Pidx, 9) = PHYSICAL(u4Rp);

    _DMX_Unlock();

    UNUSED(u4FrameAddr);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_UpdateVideoReadPointer
 *  Update read pointer of video PID buffer, called by video decoder
 *
 *  @param  u1Channel       The channel
 *  @param  u4Rp            The new read pointer
 *  @param  u4FrameAddr     The frame (picture) address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_UpdateVideoReadPointer(UINT8 u1Channel, UINT32 u4Rp,
    UINT32 u4FrameAddr)
{
    UINT8 u1Pidx;

    // Get pidx from channel
    if (!_DMX_GetVideoPidx(u1Channel, &u1Pidx))
    {
        return FALSE;
    }

    return _DMX_UpdateReadPointer(u1Pidx, u4Rp, u4FrameAddr);
}

//-----------------------------------------------------------------------------
/** DMX_RegSourceStateCbForVdec
 *  Set Source Scrambled callback for VDEC
 *  @param   u1Channel       The channel
 *  @param   pfnStateCb      The function pointer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_RegSourceStateCbForVdec(UINT8 u1Channel, PFN_DMX_SORUCE_STATE_CB_FOR_VDEC pfnStateCb)
{
    UINT8 u1Pidx;
    PID_STRUCT_T* prPidStruct;

    // Get pidx from channel
    if (!_DMX_GetVideoPidx(u1Channel, &u1Pidx))
    {
        return FALSE;
    }

    _DMX_Lock();
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    prPidStruct->pfnStateCbForVdec = pfnStateCb;
    _DMX_Unlock();
    
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_UpdateVideoWritePointer
 *  Update write pointer of video buffer to decoder
 *
 *  @param  u1Channel       The channel
 *  @param  u4Wp            The new write pointer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_UpdateVideoWritePointer(UINT8 u1Pidx, UINT8 u1Channel, UINT32 u4Wp)
{
    PID_STRUCT_T *prPidStruct;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if(prPidStruct == NULL)
    {
        return FALSE;
    }

    if(prPidStruct->eInputType == DMX_IN_BROADCAST_TS)
    {
        if (_rDecoderCallbacks.pfnUpdateVideoWp != NULL)
        {
            _rDecoderCallbacks.pfnUpdateVideoWp(u1Channel, u4Wp);
        }
    }
#ifdef ENABLE_MULTIMEDIA
    else
    {
        if(_DMX_MUL_GetMultipleInstEnabled())
        {
            DMX_DECODER_CALLBACKS_T *prCallback;
            prCallback = _DMX_MUL_GetPidxDecoderCallback(u1Pidx);
            if((prCallback != NULL) && (prCallback->pfnUpdateVideoWp != NULL))
            {
                prCallback->pfnUpdateVideoWp(u1Channel, u4Wp);
            }
        }
        else
        {
            if (_rDecoderCallbacks.pfnUpdateVideoWp != NULL)
            {
                _rDecoderCallbacks.pfnUpdateVideoWp(u1Channel, u4Wp);
            }
        }
    }
#endif  // ENABLE_MULTIMEDIA

    return TRUE;
}


//-----------------------------------------------------------------------------
static BOOL _DMX_SendMPEGPictureHeader(const DMX_PES_MSG_T* prPes)
{
    DMX_PES_MSG_T rPes;
    UINT8 u1Pidx;

    ASSERT(prPes != NULL);

    u1Pidx = prPes->u1Pidx;
    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    rPes = *prPes;
/*
#ifdef TIME_MEASUREMENT
    if ((_fgWaitForFirstI == TRUE) && (rPes.u4FrameType == PIC_TYPE_I))
    {
        TMS_END_EX(TMS_FLAG_CHG_CHL, "DMX_FIRST_I", "DMX_First_I");
        _fgWaitForFirstI = FALSE;
    }
#endif  // TIME_MEASUREMENT
*/
    // Queue the picture header
    switch (rPes.u4FrameType)
    {
#ifdef CC_VDEC_FMT_DETECT
    case H264_PIC_TYPE_NONIDR:
    case H264_PIC_TYPE_SEQ:
        if (!_DmxAddPictureHeader(&rPes))
        {
            return FALSE;
        }
        break;
#endif  // CC_VDEC_FMT_DETECT

    case PIC_TYPE_SEQ_START:
        _afgSeqHeader[u1Pidx] = TRUE;
        _au4SeqHeaderAddr[u1Pidx] = rPes.u4FrameAddr;
        if (_afgSeqEnd[u1Pidx])
        {
            _afgSeqHeaderPtsDts[u1Pidx] = _afgSeqEndPtsDts[u1Pidx];
            _au4SeqHeaderPts[u1Pidx] = _au4SeqEndPts[u1Pidx];
            _au4SeqHeaderDts[u1Pidx] = _au4SeqEndDts[u1Pidx];

            _afgSeqEnd[u1Pidx] = FALSE;
            _afgSeqEndPtsDts[u1Pidx] = FALSE;
        }
        else
        {
            _afgSeqHeaderPtsDts[u1Pidx] = rPes.fgPtsDts;
            _au4SeqHeaderPts[u1Pidx] = rPes.u4Pts;
            _au4SeqHeaderDts[u1Pidx] = rPes.u4Dts;
        }
        break;

    case PIC_TYPE_GOP:
        _afgGop[u1Pidx] = TRUE;
        _au4GopAddr[u1Pidx] = rPes.u4FrameAddr;
        if (_afgSeqEnd[u1Pidx])
        {
            _afgGopPtsDts[u1Pidx] = _afgSeqEndPtsDts[u1Pidx];
            _au4GopPts[u1Pidx] = _au4SeqEndPts[u1Pidx];
            _au4GopDts[u1Pidx] = _au4SeqEndDts[u1Pidx];

            _afgSeqEnd[u1Pidx] = FALSE;
            _afgSeqEndPtsDts[u1Pidx] = FALSE;
        }
        else
        {
            _afgGopPtsDts[u1Pidx] = rPes.fgPtsDts;
            _au4GopPts[u1Pidx] = rPes.u4Pts;
            _au4GopDts[u1Pidx] = rPes.u4Dts;
        }
        break;

    case PIC_TYPE_I:
    case PIC_TYPE_P:
    case PIC_TYPE_B:
        rPes.fgSeqHeader = _afgSeqHeader[u1Pidx];
        rPes.fgGop = _afgGop[u1Pidx];
        if (rPes.fgSeqHeader)
        {
            rPes.u4FrameAddr = _au4SeqHeaderAddr[u1Pidx];
            if (!rPes.fgPtsDts)
            {
                rPes.fgPtsDts = _afgSeqHeaderPtsDts[u1Pidx];
                rPes.u4Pts = _au4SeqHeaderPts[u1Pidx];
                rPes.u4Dts = _au4SeqHeaderDts[u1Pidx];
            }
        }
        else if (rPes.fgGop)
        {
            rPes.u4FrameAddr = _au4GopAddr[u1Pidx];
            if (!rPes.fgPtsDts)
            {
                rPes.fgPtsDts = _afgGopPtsDts[u1Pidx];
                rPes.u4Pts = _au4GopPts[u1Pidx];
                rPes.u4Dts = _au4GopDts[u1Pidx];
            }
        }
        else if (_afgSeqEnd[u1Pidx] && (!rPes.fgPtsDts))
        {
            rPes.fgPtsDts = _afgSeqEndPtsDts[u1Pidx];
            rPes.u4Pts = _au4SeqEndPts[u1Pidx];
            rPes.u4Dts = _au4SeqEndDts[u1Pidx];
        }
        if (!_DmxAddPictureHeader(&rPes))
        {
            return FALSE;
        }
        _afgSeqHeader[u1Pidx] = FALSE;
        _afgGop[u1Pidx] = FALSE;
        _afgSeqEnd[u1Pidx] = FALSE;
        break;

    case PIC_TYPE_SEQ_END:
        // This case is for supporting bit streams full of audio PES but
        // with scarce and concentrated (not scattered) video PES. It also deals
        // with cases in which the SEQUENCE_END is at the very end of a PES.
        if (rPes.fgPtsDts)
        {
            _afgSeqEnd[u1Pidx] = TRUE;
            _afgSeqEndPtsDts[u1Pidx] = TRUE;
            _au4SeqEndPts[u1Pidx] = rPes.u4Pts;
            _au4SeqEndDts[u1Pidx] = rPes.u4Dts;
        }
        if (!_DmxAddPictureHeader(&rPes))
        {
            return FALSE;
        }
        break;

    default:
        // Todo: count invalid picture headers
        break;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
static BOOL _DMX_SendH264PictureHeader(const DMX_PES_MSG_T* prPes)
{
    DMX_PES_MSG_T rPes;
    UINT8 u1Pidx;

    ASSERT(prPes != NULL);

    u1Pidx = prPes->u1Pidx;
    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    rPes = *prPes;
    rPes.eVideoType = DMX_VIDEO_H264;
/*
#ifdef TIME_MEASUREMENT
    if ((_fgWaitForFirstI == TRUE) && (rPes.u4FrameType == H264_PIC_TYPE_SEQ))
    {
        // Normally, the I frame appears right after SEQ.
        TMS_END_EX(TMS_FLAG_CHG_CHL, "DMX_FIRST_I", "DMX_First_SEQ");
        _fgWaitForFirstI = FALSE;
    }
#endif  // TIME_MEASUREMENT
*/
    // Store PTS/DTS if they exist.
    if(rPes.fgPtsDts)
    {
        _afgSeqHeaderPtsDts[u1Pidx] = rPes.fgPtsDts;
        _au4SeqHeaderPts[u1Pidx] = rPes.u4Pts;
        _au4SeqHeaderDts[u1Pidx] = rPes.u4Dts;
    }
    else if (_afgSeqHeaderPtsDts[u1Pidx])
    {
        // Make use the previous PTS/DTS values if rPes does not have PTS/DTS.
        rPes.fgPtsDts = _afgSeqHeaderPtsDts[u1Pidx];
        rPes.u4Pts = _au4SeqHeaderPts[u1Pidx];
        rPes.u4Dts = _au4SeqHeaderDts[u1Pidx];
    }

    // Clear the PTS/DTS values when a picture is found.
    if ((rPes.u4FrameType == H264_PIC_TYPE_IDR) ||
    	(rPes.u4FrameType == H264_PIC_TYPE_NONIDR))
    {
        _afgSeqHeaderPtsDts[u1Pidx] = FALSE;
        _au4SeqHeaderPts[u1Pidx] = 0;
        _au4SeqHeaderDts[u1Pidx]= 0;
    }

    if(rPes.u4FrameType == H264_PIC_TYPE_SEQ)
    {
        rPes.fgSeqHeader = TRUE;
    }

    // Queue the picture header only when certain frame types are found.
    // Two adjacent switch-case statements are used to avoid LINT warning when
    // fall-through is used.
    switch (rPes.u4FrameType)
    {
    case H264_PIC_TYPE_NONIDR:
    case H264_PIC_TYPE_IDR:
    case H264_PIC_TYPE_SEI:
    case H264_PIC_TYPE_SEQ:
    case H264_PIC_TYPE_PIC:
    case H264_PIC_TYPE_AU:
#ifdef CC_VDEC_FMT_DETECT
    case PIC_TYPE_I:
    case PIC_TYPE_P:
#endif  // CC_VDEC_FMT_DETECT
#if defined(CC_MVC_SUPPORT) || defined(CC_SW_MVC_ENABLE)
    case H264_PIC_TYPE_PREFIX:
    case H264_PIC_TYPE_SUB_SPS:
    case H264_PIC_TYPE_CODED_SLICE_EXT:
#endif
        if (!_DmxAddPictureHeader(&rPes))
        {
            return FALSE;
        }
        break;
    default:
        // Todo: count invalid picture headers
        break;
    }

    return TRUE;
}


#ifdef ENABLE_MULTIMEDIA
//-----------------------------------------------------------------------------
static BOOL _DMX_SendMPEG4PictureHeader(const DMX_PES_MSG_T* prPes)
{
    DMX_PES_MSG_T rPes;

    ASSERT(prPes != NULL);
    rPes = *prPes;

    if (!_DmxAddPictureHeader(&rPes))
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DMX_SendRVPictureHeader(const DMX_PES_MSG_T* prPes)
{
    DMX_PES_MSG_T rPes;

    ASSERT(prPes != NULL);
    rPes = *prPes;

    if (!_DmxAddPictureHeader(&rPes))
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DMX_SendVPPictureHeader(const DMX_PES_MSG_T* prPes)
{
    DMX_PES_MSG_T rPes;

    ASSERT(prPes != NULL);
    rPes = *prPes;

    if (!_DmxAddPictureHeader(&rPes))
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DMX_SendVC1PictureHeader(const DMX_PES_MSG_T* prPes)
{
    DMX_PES_MSG_T rPes;
    UINT8 u1Pidx;

    ASSERT(prPes != NULL);

    u1Pidx = prPes->u1Pidx;
    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    rPes = *prPes;
    rPes.eVideoType = DMX_VIDEO_VC1;

    // Store PTS/DTS if they exist.
    if(rPes.fgPtsDts)
    {
        _afgSeqHeaderPtsDts[u1Pidx] = rPes.fgPtsDts;
        _au4SeqHeaderPts[u1Pidx] = rPes.u4Pts;
        _au4SeqHeaderDts[u1Pidx] = rPes.u4Dts;
    }
    else if (_afgSeqHeaderPtsDts[u1Pidx])
    {
        // Make use the previous PTS/DTS values if rPes does not have PTS/DTS.
        rPes.fgPtsDts = _afgSeqHeaderPtsDts[u1Pidx];
        rPes.u4Pts = _au4SeqHeaderPts[u1Pidx];
        rPes.u4Dts = _au4SeqHeaderDts[u1Pidx];
    }

    // Clear the PTS/DTS values when a picture is found.
    switch (rPes.u4FrameType)
    {
    case VC1_PIC_TYPE_I:
    case VC1_PIC_TYPE_P:
    case VC1_PIC_TYPE_B:
    case VC1_PIC_TYPE_BI:
    case VC1_PIC_TYPE_SKIP:
        _afgSeqHeaderPtsDts[u1Pidx] = FALSE;
        _au4SeqHeaderPts[u1Pidx] = 0;
        _au4SeqHeaderDts[u1Pidx]= 0;
        break;
    default:
        break;
    }

    // Queue the picture header only when certain frame types are found.
    // Two adjacent switch-case statements are used to avoid LINT warning when
    // fall-through is used.
    switch (rPes.u4FrameType)
    {
    case VC1_PIC_TYPE_I:
    case VC1_PIC_TYPE_P:
    case VC1_PIC_TYPE_B:
    case VC1_PIC_TYPE_BI:
    case VC1_PIC_TYPE_SKIP:
    case VC1_PIC_TYPE_ENTRY:
    case VC1_PIC_TYPE_SEQ_START:
        if (!_DmxAddPictureHeader(&rPes))
        {
            return FALSE;
        }
        break;
    default:
        // Todo: count invalid picture headers
        break;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
static BOOL _DMX_SendWMVPictureHeader(const DMX_PES_MSG_T* prPes)
{
    DMX_PES_MSG_T rPes;

    ASSERT(prPes != NULL);

    if (! _DMX_MM_IsFrameHead())
    {
        return TRUE;        // To avoid the LOG message in _DmxProcessVideo*().
    }

    rPes = *prPes;
    rPes.u4FrameAddr = prPes->u4FrameAddr;

    if ((rPes.u4FrameAddr <  rPes.u4BufStart) ||
        (rPes.u4FrameAddr >= rPes.u4BufEnd ))
    {
        LOG(3, "The frame address is out of boundary!\n");
        return FALSE;
    }

    // Send the picture header if it (the data chunk) is the frame tail.
    if (!_DmxAddPictureHeader(&rPes))
    {
        return FALSE;
    }

    return TRUE;
}
#endif  // ENABLE_MULTIMEDIA


//-----------------------------------------------------------------------------
static BOOL _DMX_SendAVSPictureHeader(const DMX_PES_MSG_T* prPes)
{
    DMX_PES_MSG_T rPes;

    ASSERT(prPes != NULL);
    rPes = *prPes;

    if (!_DmxAddPictureHeader(&rPes))
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_SendPictureHeader
 *  Send picture header to MPEG decoder
 *
 *  @param  prPes           Video PES structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SendPictureHeader(DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    DMX_VIDEO_TYPE_T eVideoType;

    if(prPes == NULL)
    {
        return FALSE;
    }

    fgRet = FALSE;
    eVideoType = _DMX_GetPidxVideoType(prPes->u1Pidx);
    prPes->eVideoType = eVideoType;

    switch(eVideoType)
    {
    case DMX_VIDEO_MPEG:
        fgRet = _DMX_SendMPEGPictureHeader(prPes);
        break;

    case DMX_VIDEO_H264:
        fgRet = _DMX_SendH264PictureHeader(prPes);
        break;

#ifdef ENABLE_MULTIMEDIA
    case DMX_VIDEO_MPEG4:
        fgRet = _DMX_SendMPEG4PictureHeader(prPes);
        break;

    case DMX_VIDEO_RV:
        fgRet = _DMX_SendRVPictureHeader(prPes);
        break;

    case DMX_VIDEO_VC1:
        fgRet = _DMX_SendVC1PictureHeader(prPes);
        break;

    case DMX_VIDEO_WMV7:
    case DMX_VIDEO_WMV8:
    case DMX_VIDEO_WMV9:
    case DMX_VIDEO_MP4_IN_WMV:
        fgRet = _DMX_SendWMVPictureHeader(prPes);
        break;

    case DMX_VIDEO_VP6:
    case DMX_VIDEO_VP8:
        fgRet = _DMX_SendVPPictureHeader(prPes);
        break;

#endif  // ENABLE_MULTIMEDIA

    case DMX_VIDEO_AVS:
        fgRet = _DMX_SendAVSPictureHeader(prPes);
        break;

    default:
        LOG(3, "Unknown video type\n");
        ASSERT(0);
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_SendAudioPes
 *  Send audio PES to audio decoder
 *
 *  @param  prPes           The audio PES structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SendAudioPes(DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet = TRUE;
    PID_STRUCT_T *prPidStruct;

    if(prPes == NULL)
    {
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(prPes->u1PidIndex);
    if(prPidStruct == NULL)
    {
        return FALSE;
    }
    prPes->pvInstanceTag = prPidStruct->pvInstanceTag;

    if(prPidStruct->eInputType == DMX_IN_BROADCAST_TS)
    {
        if (_rDecoderCallbacks.pfnSendAudioPes != NULL)
        {
            fgRet = _rDecoderCallbacks.pfnSendAudioPes(prPes);
        }
    }
#ifdef ENABLE_MULTIMEDIA
    else
    {
        if(_DMX_MUL_GetMultipleInstEnabled())
        {
            DMX_DECODER_CALLBACKS_T *prCallback;
            prCallback = _DMX_MUL_GetPidxDecoderCallback(prPes->u1PidIndex);
            if ((prCallback != NULL) && (prCallback->pfnSendAudioPes != NULL))
            {
                fgRet = prCallback->pfnSendAudioPes(prPes);
            }
        }
        else
        {
            if (_rDecoderCallbacks.pfnSendAudioPes != NULL)
            {
                fgRet = _rDecoderCallbacks.pfnSendAudioPes(prPes);
            }
        }
    }
#endif  // ENABLE_MULTIMEDIA

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_SetDecoderCallbacks
 *  Set decoder callback functions
 *
 *  @param  prCallbacks     The decoder callback functions
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetDecoderCallbacks(const DMX_DECODER_CALLBACKS_T* prCallbacks)
{
    ASSERT(prCallbacks != NULL);

    _rDecoderCallbacks = *prCallbacks;
}


//-----------------------------------------------------------------------------
/** _DMX_GetDecoderCallbacks
 *  Get decoder callback functions
 */
//-----------------------------------------------------------------------------
DMX_DECODER_CALLBACKS_T* _DMX_GetDecoderCallbacks(void)
{
    return &_rDecoderCallbacks;
}


//-----------------------------------------------------------------------------
/** _DMX_SendPurgeMessage
 *  Send full message to demux thread
 *
 *  @param  u1Pidx          PID index
 *  @param  fgReenablePid   Re-enable PID or not
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 *
 *  @context                ISR or thread
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SendPurgeMessage(UINT8 u1Pidx, BOOL fgReenablePid)
{
    DMX_PES_MSG_T rPes;
    PID_STRUCT_T* prPidStruct;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    rPes.u1Pidx = u1Pidx;
    rPes.eMsgType = (fgReenablePid ? DMX_PES_MSG_TYPE_PURGE_REENABLE :
        DMX_PES_MSG_TYPE_PURGE);
    rPes.u1SerialNumber = prPidStruct->u1SerialNumber;
    if (x_msg_q_send(_hPurgeQueue, (VOID*)&rPes, sizeof (DMX_PES_MSG_T),
        DMX_MSG_PRIORITY_HIGH) != OSR_OK)
    {
        LOG(3, "Demux queue full!\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_NotifyEnablePid
 *  Be informed that a PID is enabling or disabling
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
void _DMX_NotifyEnablePid(UINT8 u1Pidx, BOOL fgEnable)
{
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    if (fgEnable)
    {

#ifdef TRACK_LAST_FRAME

        // Reset last-sent frame
        _DmxSetLastFrame(u1Pidx, 0);

#endif

        // Reset flags of sequence header and GOP
        _afgSeqHeader[u1Pidx] = FALSE;
        _afgGop[u1Pidx] = FALSE;
        _afgSeqEnd[u1Pidx] = FALSE;
    }
}


//-----------------------------------------------------------------------------
/** _DMX_InitHandler
 *  Initialize decoder interfaces
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_InitHandler(void)
{
    UINT32 i;

    static BOOL _fgInit = FALSE;

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        _afgSeqHeader[i] = FALSE;
        _afgGop[i] = FALSE;
        _afgSeqEnd[i] = FALSE;

#ifdef TRACK_LAST_FRAME
        _au4LastFrame[i] = 0;
#endif
    }

    if (!_fgInit)
    {
        // Set default MPEG callback handlers
        _rDecoderCallbacks.pfnSendVideoPes = _DmxSendVideoPes;
        _rDecoderCallbacks.pfnUpdateVideoWp = _DmxUpdateVideoWp;
        _rDecoderCallbacks.pfnSendAudioPes = _DmxSendAudioPes;

        // Create picture queues
        if (x_msg_q_create(&_hDmxQueue, DMX_QUEUE_NAME,
            sizeof (DMX_PES_MSG_T), DMX_QUEUE_SIZE) != OSR_OK)
        {
            LOG(1, "Fail to create picture queue!\n");
            return FALSE;
        }

        // Create purge queues
        if (x_msg_q_create(&_hPurgeQueue, DMX_PURGE_QUEUE_NAME,
            sizeof (DMX_PES_MSG_T), DMX_QUEUE_SIZE) != OSR_OK)
        {
            LOG(1, "Fail to create purge queue!\n");
            return FALSE;
        }

        // Create picture thread
        if (x_thread_create(&_hDmxThread, DMX_THREAD_NAME,
            DMX_THREAD_STACK_SIZE, DMX_THREAD_PRIORITY, _DmxThread, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX thread!\n");
            return FALSE;
        }

        // Create purge thread
        if (x_thread_create(&_hPurgeThread, DMX_PURGE_THREAD_NAME,
            DMX_THREAD_STACK_SIZE, DMX_THREAD_PRIORITY, _PurgeThread, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create purge thread!\n");
            return FALSE;
        }

        x_thread_set_affinity(_hDmxThread, 1);
        
#ifdef CC_DMX_ENABLE_MONTHREAD
        // Create picture thread
        if (x_thread_create(&_hDmxMonThread, DMX_MON_THREAD_NAME,
            DMX_MON_THREAD_STACK_SIZE, DMX_MON_THREAD_PRIORITY, _DmxMonThread, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX monitor thread!\n");
            return FALSE;
        }
#endif  // CC_DMX_ENABLE_MONTHREAD
        _fgInit = TRUE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
BOOL _DMX_SendEOSMessage(UINT8 u1Pidx)
{
    DMX_PES_MSG_T rPes;
    PID_STRUCT_T *prPidStruct;
    UINT8 u1Channel;

    if (!_DMX_IsToDecoder())
    {
        return TRUE;
    }

    // Most of the values are of no use except for fgEOS.
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if ((prPidStruct->ePidType != DMX_PID_TYPE_ES_VIDEO) &&
        (prPidStruct->ePidType != DMX_PID_TYPE_ES_AUDIO))
    {
        return FALSE;
    }
    VERIFY(_DMX_GetVideoChannel(u1Pidx, &u1Channel));

    x_memset((void*)&rPes, 0, sizeof(rPes));
    rPes.eMsgType = DMX_PES_MSG_TYPE_PES;
    rPes.ePidType = prPidStruct->ePidType;
    rPes.u4FrameType = PIC_TYPE_I;
    rPes.u1Pidx = u1Pidx;
    rPes.u1Channel = u1Channel;
    rPes.u1DeviceId = prPidStruct->u1DeviceId;          // map to ucMpvId
    rPes.u1SerialNumber = prPidStruct->u1SerialNumber;
    rPes.fgEOS = TRUE;
    rPes.pvInstanceTag = prPidStruct->pvInstanceTag;

    if (x_msg_q_send(_hDmxQueue, (VOID*)&rPes, sizeof(rPes),
        DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(3, "The Demux message queue is full!\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
void _DMX_ErrHanlde(void)
{
    DMX_PES_MSG_T rPes;

    x_memset((void*)&rPes, 0, sizeof(rPes));
    rPes.eMsgType = DMX_PES_MSG_TYPE_ERR_HANDLE;

#ifdef ENABLE_MULTIMEDIA
    // Important!!!  To avoid a potential deadlock...
    if (!_DMX_DDI_Unlock())
    {
        LOG(3, "Cannot unlock DDI!\n");
    }
/*
        if(eInputType == DMX_IN_PLAYBACK_PS)
        {
            return;  // Auto-reset is not performed for TS/PS file playback for the time being.
                     // Remember to remove the "return" statement if auto-reset is implemented.
        }
*/
#endif  // ENABLE_MULTIMEDIA

    if (x_msg_q_send(_hPurgeQueue, (VOID*)&rPes, sizeof(rPes),
        DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(3, "The Demux message queue is full!\n");
    }
}

// for vdec buffer control            
void _DMX_GetPicQSize(UINT16* pu2DmxPictureQSize, UINT16* pu2DmxMaxQSize)
{
    if(_hDmxQueue != NULL_HANDLE && pu2DmxPictureQSize != NULL && pu2DmxMaxQSize != NULL)
    {
        VERIFY(x_msg_q_num_msgs(_hDmxQueue, pu2DmxPictureQSize) == OSR_OK);
        *pu2DmxMaxQSize = DMX_QUEUE_SIZE;
    }
    else // error
    {
        if(pu2DmxPictureQSize != NULL && pu2DmxMaxQSize != NULL)
        {
            *pu2DmxPictureQSize = 0;
            *pu2DmxMaxQSize  = 0;
        }
    }
    return;
}
