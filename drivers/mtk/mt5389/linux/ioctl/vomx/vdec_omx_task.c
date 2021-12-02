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

#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_VOMX)

#include "vomx_if.h"

#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_assert.h"
#include "vdec_mod.h"
#include "vdec_omx_debug.h"


//#include "drv_thread.h"

static void vVDOOmxThread(void* pvArg)
{
    INT32 i4Ret = 0;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize;
    VDO_OMX_MSG_T rMsg;

    VDO_OMX_HANDLE_T *pHandle = *((VDO_OMX_HANDLE_T **)pvArg);
    VOMX_Printf("[VDO][OMX] vVDOOmxThread: pHandle = 0x%X.\n", pHandle);

    while (TRUE)
    {
        // wait FillThisBuffer/EmptyThisBuffer/SendCommand event
   //     if (pHandle->fgEmptying || pHandle->fgFilling)
   //     {
  //          i4Ret = x_msg_q_receive_timeout(&u2MsgIdx, (void *)&rMsg, &zMsgSize, &(pHandle->hCommandQ), 1, 5);
  //          VOMX_Printf("[VDO][OMX] x_msg_q_receive_timeout: i4Ret = 0x%X.\n", i4Ret);
 //       }
 //       else
        {
            zMsgSize = sizeof(VDO_OMX_MSG_T);
            i4Ret = x_msg_q_receive(&u2MsgIdx, (void *)&rMsg, &zMsgSize, &(pHandle->hCommandQ), 1, X_MSGQ_OPTION_WAIT);
            VOMX_Printf("[VDO][OMX] x_msg_q_receive: i4Ret = 0x%X.\n", i4Ret);
        }

        if (OSR_OK == i4Ret)
        {
            if (VDO_OMX_CMD_TERMINATE == rMsg.eCmd)
            {
                // exiting
                break;
            }

            switch (rMsg.eCmd)
            {
            case VDO_OMX_CMD_SET_STATE:
                i4VDOOmxCompSetState(pHandle, rMsg.eStateTo);
                break;
            case VDO_OMX_CMD_EMPTY_BUFFER:
//                i4VDOOmxQueueEmptyBuf(pHandle, rMsg.hHeader);
                break;
            case VDO_OMX_CMD_FILL_BUFFER:
//                i4VDOOmxQueueFillBuf(pHandle, rMsg.hHeader);
                break;
            case VDO_OMX_CMD_FLUSH:
   //             i4VDOOmxCompFlush(pHandle, rMsg.fgInput, rMsg.fgOutput);
                break;
            case VDO_OMX_CMD_BUFFER_ALLOCATED:
            case VDO_OMX_CMD_BUFFER_FREED:
                break;
            case VDO_OMX_CMD_PORT_ENABLE:
                i4VDOOmxCompPortEnable(pHandle, rMsg.fgInput, rMsg.fgOutput);
                break;
            case VDO_OMX_CMD_PORT_DISABLE:
                i4VDOOmxCompPortDisable(pHandle, rMsg.fgInput, rMsg.fgOutput);
                break;
            default:
                break;
            }
        }

        // check if state transition done
        if (pHandle->fgStateTransing)
        {
            i4VDOOmxCompCheckStateTransition(pHandle);
        }

        // do empty buffer
        if (pHandle->fgEmptying)
        {
//            i4VDOOmxDoEmptyBuf(pHandle);
        }

        // do fill buffer
        if (pHandle->fgFilling)
        {
 //           i4VDOOmxDoFillBuf(pHandle);
        }
    }

    // delete message queue
    x_msg_q_delete(pHandle->hCommandQ);

    x_thread_exit();
}


INT32 i4VDOOmxTaskCreate(VDO_OMX_HANDLE_T *pHandle)
{
    char acBuf[32];
    static UINT32 u4Cnt = 0;

    // create command queue
    x_memset(acBuf, 0, 32 * sizeof(char));
    x_snprintf(acBuf, sizeof(acBuf), "VOMXCQ%d", u4Cnt);
    x_msg_q_create(&(pHandle->hCommandQ), acBuf, sizeof(VDO_OMX_MSG_T), 8);
    
    VOMX_Printf("[VDO][OMX] i4VDOOmxTaskCreate: pHandle = 0x%X.\n", pHandle);
    // create video OpenMAX main thread
    x_memset(acBuf, 0, 32 * sizeof(char));
    x_snprintf(acBuf, sizeof(acBuf), "VOMXTsk%d", u4Cnt);
    x_thread_create(&(pHandle->hMainTask), acBuf,
                    VDOOMX_STACK_SIZE, VDOOMX_THREAD_PRIORITY,
                    vVDOOmxThread, sizeof(void*), &pHandle);

    u4Cnt++;
    if(u4Cnt > 2*VDO_COMP_MAX)
    {
        u4Cnt = 0;
    }
    return VDecOMX_OK;
}


INT32 i4VDOOmxTaskDestroy(VDO_OMX_HANDLE_T *pHandle)
{
    VDO_OMX_MSG_T rMsgTerminate;

    rMsgTerminate.eCmd = VDO_OMX_CMD_TERMINATE;
    i4VDOOmxTaskSendCommand(pHandle, &rMsgTerminate);

    return VDecOMX_OK;
}

INT32 i4VDOOmxTaskSendCommand(VDO_OMX_HANDLE_T *pHandle, VDO_OMX_MSG_T *prMsg)
{
    INT32 i4Ret = 0;

    if (NULL_HANDLE != pHandle->hCommandQ)
    {
        i4Ret = x_msg_q_send(pHandle->hCommandQ, prMsg, sizeof(VDO_OMX_MSG_T), 0);
    }

    return i4Ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
static HANDLE_T _hVDOOmxSem = NULL_HANDLE;

INT32 i4VDOOmxTaskSemInit(void)
{
    INT32 i4Ret = 0;

    // create semaphore, only one semaphore for this module
    if (NULL_HANDLE == _hVDOOmxSem)
    {
        i4Ret = x_sema_create(&_hVDOOmxSem, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    }

    return i4Ret;
}


INT32 i4VDOOmxTaskSemUninit(void)
{
    INT32 i4Ret = 0;

    // delete semaphore
    if (NULL_HANDLE != _hVDOOmxSem)
    {
        i4Ret = x_sema_delete(_hVDOOmxSem);
        _hVDOOmxSem = NULL_HANDLE;
    }

    return i4Ret;
}


INT32 i4VDOOmxTaskLockSem(void)
{
    INT32 i4Ret = 0;

    if (NULL_HANDLE != _hVDOOmxSem)
    {
        i4Ret = x_sema_lock(_hVDOOmxSem, X_SEMA_OPTION_WAIT);
    }

    return i4Ret;
}


INT32 i4VDOOmxTaskUnlockSem(void)
{
    INT32 i4Ret = 0;

    if (NULL_HANDLE != _hVDOOmxSem)
    {
        i4Ret = x_sema_unlock(_hVDOOmxSem);
    }

    return i4Ret;
}

#endif

