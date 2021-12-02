/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_AOMX)

#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_assert.h"

#include "drv_common.h"     //DANIEL_HACK

#include "x_aud_dec.h"
//#include "drv_aud.h"      //DANIEL_HACK
//#include "drv_thread.h"   //DANIEL_HACK

#include "aud_if.h"         //DANIEL_HACK
#include "aud_omx.h"

//DANIEL_HACK
#ifndef SYS_Printf
//#define SYS_Printf Printf
#define SYS_Printf(fmt...)
#endif

static void vAudOmxThread(void* pvArg)
{
    INT32 i4Ret = 0;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize;
    AUD_OMX_MSG_T rMsg;

    AUD_OMX_HANDLE_T *pHandle = *((AUD_OMX_HANDLE_T **)pvArg);

    SYS_Printf("[AUD][OMX] vAudOmxThread: pHandle = 0x%X.\n", pHandle);

    while (TRUE)
    {
        // wait FillThisBuffer/EmptyThisBuffer/SendCommand event
        if ((pHandle->fgEmptying || pHandle->fgFilling) || ((u1AudDspState == 1) && !(u4AOmxDbgMsk & 0x80)))
        {
            i4Ret = x_msg_q_receive_timeout(&u2MsgIdx, (void *)&rMsg, &zMsgSize, &(pHandle->hCommandQ), 1, 5);
        }
        else
        {
            i4Ret = x_msg_q_receive(&u2MsgIdx, (void *)&rMsg, &zMsgSize, &(pHandle->hCommandQ), 1, X_MSGQ_OPTION_WAIT);
        }

        if (OSR_OK == i4Ret)
        {
            if (AUD_OMX_CMD_TERMINATE == rMsg.eCmd)
            {
                // exiting
                break;
            }

            switch (rMsg.eCmd)
            {
            case AUD_OMX_CMD_SET_STATE:
                i4AudOmxCompSetState(pHandle, rMsg.eStateTo);
                break;
            case AUD_OMX_CMD_EMPTY_BUFFER:
                i4AudOmxQueueEmptyBuf(pHandle, rMsg.hHeader);
                break;
            case AUD_OMX_CMD_FILL_BUFFER:
                i4AudOmxQueueFillBuf(pHandle, rMsg.hHeader);
                break;
            case AUD_OMX_CMD_FLUSH:
                i4AudOmxCompFlush(pHandle, rMsg.fgInput, rMsg.fgOutput);
                break;
            case AUD_OMX_CMD_PORT_ENABLE:
                i4AudOmxCompPortEnable(pHandle, rMsg.fgInput, rMsg.fgOutput);
                break;
            case AUD_OMX_CMD_PORT_DISABLE:
                i4AudOmxCompPortDisable(pHandle, rMsg.fgInput, rMsg.fgOutput);
                break;
            default:
                break;
            }
        }

        // check if state transition done
        if (pHandle->fgStateTransing)
        {
            i4AudOmxCompCheckStateTransition(pHandle);
        }

        // do empty buffer
        if (pHandle->fgEmptying)
        {
            i4AudOmxDoEmptyBuf(pHandle);
        }

        // do fill buffer
        if (pHandle->fgFilling)
        {
            i4AudOmxDoFillBuf(pHandle);
        }

        // check if any EmptyThisBuffer has really done
        i4AudOmxCheckBufEmptyDone(pHandle);

        if (!(u4AOmxDbgMsk & 0x80))
        {
            UINT32 u4UpBufFullness = u4AudOmxUpBufFullness();
            if ((u1AudDspState == 1) && (u4UpBufFullness > 60))
            {
                Printf("AOMX Pause(%d)\n", u4UpBufFullness);
                AUD_DSPCmdPause(AUD_DEC_MAIN);
                u1AudDspState = 2;
            }
            else if ((u1AudDspState == 2) && (u4UpBufFullness < 30))
            {
                Printf("AOMX Resume(%d)\n", u4UpBufFullness);
                AUD_DSPCmdResume(AUD_DEC_MAIN);
                u1AudDspState = 1;
            }
        }
    }

    // delete message queue
    x_msg_q_delete(pHandle->hCommandQ);

    SYS_Printf("[AUD][OMX] vAudOmxThread: pHandle = 0x%X. Task terminated.\n", pHandle);

    x_thread_exit();
}


INT32 i4AudOmxTaskCreate(AUD_OMX_HANDLE_T *pHandle)
{
    // create command queue
    x_msg_q_create(&(pHandle->hCommandQ), "AudOmxCmdQ", sizeof(AUD_OMX_MSG_T), MAX_INPUT_BUF_NO*2);

    SYS_Printf("[AUD][OMX] i4AudOmxTaskCreate: pHandle = 0x%X.\n", pHandle);

    // create audio OpenMAX main thread
    x_thread_create(&(pHandle->hMainTask), "AudOmxTask",
                    AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
                    vAudOmxThread, sizeof(void *), &pHandle);

    return AUD_OK;
}


INT32 i4AudOmxTaskDestroy(AUD_OMX_HANDLE_T *pHandle)
{
    AUD_OMX_MSG_T rMsgTerminate;

    rMsgTerminate.eCmd = AUD_OMX_CMD_TERMINATE;
    i4AudOmxTaskSendCommand(pHandle, &rMsgTerminate);

    return AUD_OK;
}


INT32 i4AudOmxTaskSendCommand(AUD_OMX_HANDLE_T *pHandle, AUD_OMX_MSG_T *prMsg)
{
    INT32 i4Ret = 0;

    if (NULL_HANDLE != pHandle->hCommandQ)
    {
        i4Ret = x_msg_q_send(pHandle->hCommandQ, prMsg, sizeof(AUD_OMX_MSG_T), 0/*not used*/);
    }

    return i4Ret;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
static HANDLE_T _hAudOmxSem = NULL_HANDLE;

INT32 i4AudOmxTaskSemInit(void)
{
    INT32 i4Ret = 0;

    // create semaphore, only one semaphore for this module
    if (NULL_HANDLE == _hAudOmxSem)
    {
        i4Ret = x_sema_create(&_hAudOmxSem, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    }

    return i4Ret;
}


INT32 i4AudOmxTaskSemUninit(void)
{
    INT32 i4Ret = 0;

    // delete semaphore
    if (NULL_HANDLE != _hAudOmxSem)
    {
        i4Ret = x_sema_delete(_hAudOmxSem);
        _hAudOmxSem = NULL_HANDLE;
    }

    return i4Ret;
}


INT32 i4AudOmxTaskLockSem(void)
{
    INT32 i4Ret = 0;

    if (NULL_HANDLE != _hAudOmxSem)
    {
        i4Ret = x_sema_lock(_hAudOmxSem, X_SEMA_OPTION_WAIT);
    }

    return i4Ret;
}


INT32 i4AudOmxTaskUnlockSem(void)
{
    INT32 i4Ret = 0;

    if (NULL_HANDLE != _hAudOmxSem)
    {
        i4Ret = x_sema_unlock(_hAudOmxSem);
    }

    return i4Ret;
}

#endif

