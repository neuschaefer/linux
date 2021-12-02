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

#include <asm/uaccess.h>
#include <linux/sched.h>

#include "x_rm_dev_types.h"
#include "x_aud_dec.h"
//#include "drv_aud.h"  //DANIEL_HACK
#include "aud_if.h"     //DANIEL_HACK
#include "aud_omx.h"

//DANIEL_HACK
#ifndef SYS_Printf
//#define SYS_Printf Printf
#define SYS_Printf(fmt...)
#endif

// OpenMAX Audio Component interfaces

AUD_OMX_HANDLE_T _prAudOmxHandle[1]; // currently only support single instance
static UINT8 _prAudDecInfo[64];     //RISC decoder required info

static void vAudOmxCompDoFreeHandle(AUD_OMX_HANDLE_T *pHandle);


INT32 i4AudOmxCompInit(void)
{
    _prAudOmxHandle[0].fgUsed   = FALSE;
    _prAudOmxHandle[0].eState   = OMX_StateMax;
    _prAudOmxHandle[0].eStateTo = OMX_StateMax;

    return 0;
}


INT32 i4AudOmxCompUninit(void)
{
    if (_prAudOmxHandle[0].fgUsed)
    {
        vAudOmxCompDoFreeHandle(&_prAudOmxHandle[0]);
    }

    return 0;
}
//DANIEL_HACK

//DANIEL_HACK
extern UINT8 u1ChNo;            //Stereo
extern UINT8 u1SampleRateIdx;   //48K
//DANIEL_HACK

#if 1 //DANIEL_HACK
INT32 i4AudOmxCompGetHandle(PARAMETER_OMX_GETHANDLE *prUArg, PARAMETER_OMX_CB *prOmxCb)
#else
INT32 i4AudOmxCompGetHandle(PARAMETER_OMX_GETHANDLE *prUArg)
#endif
{
    INT32 i4Ret = 0;
    UINT32 u4Index;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    char cCompName[255];
    PARAMETER_OMX_GETHANDLE rKArg;
    AUD_OMX_HANDLE_T *pHandle = NULL;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_GETHANDLE));
    i4Ret = copy_from_user(cCompName, rKArg.cComponentName, 255);

    if (_prAudOmxHandle[0].fgUsed)
    {
        rKArg.eReturnValue = OMX_ErrorInsufficientResources;
        i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_GETHANDLE));

        return i4Ret;
    }

    // check the component name
    if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.MP3", 25))
    {
        // MP3
        _prAudOmxHandle[0].eFormat = AUD_FMT_MP3;
        pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.AAC", 25))
    {
        // AAC
        _prAudOmxHandle[0].eFormat = AUD_FMT_AAC;
        pHandle = &_prAudOmxHandle[0];
        //DANIEL_HACK
        u1ChNo = 2;
        u1SampleRateIdx = 3;
        //DANIEL_HACK
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.PCM", 25))
    {
      // LPCM
      _prAudOmxHandle[0].eFormat = AUD_FMT_LPCM;
      pHandle = &_prAudOmxHandle[0];
    }
  //FOR_LG_2ND_ATTACK
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.MP12", 26))
    {
      // MP12
      _prAudOmxHandle[0].eFormat = AUD_FMT_MPEG;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.AC3", 25))
    {
      // AC3
      _prAudOmxHandle[0].eFormat = AUD_FMT_AC3;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.LPCM", 26))
    {
      // LPCM
      _prAudOmxHandle[0].eFormat = AUD_FMT_LPCM;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.MSADPCM", 29))
    {
      // LPCM - MSADPCM
      _prAudOmxHandle[0].eFormat = AUD_FMT_OMX_MSADPCM;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.IMAADPCM", 30))
    {
      // LPCM - IMAADPCM
      _prAudOmxHandle[0].eFormat = AUD_FMT_OMX_IMAADPCM;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.ALAW", 26))
    {
      // LPCM - ALAW
      _prAudOmxHandle[0].eFormat = AUD_FMT_OMX_ALAW;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.ULAW", 26))
    {
      // LPCM - ULAW
      _prAudOmxHandle[0].eFormat = AUD_FMT_OMX_ULAW;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.DTS", 25))
    {
      // DTS
      _prAudOmxHandle[0].eFormat = AUD_FMT_DTS;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.WMA10L", 28))
    {
      // WMA10 Lossless
      _prAudOmxHandle[0].eFormat = AUD_FMT_WMA10LOSSLESS;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.WMA10S", 28))
    {
      // WMA10 Speech
      _prAudOmxHandle[0].eFormat = AUD_FMT_WMA10SPEECH;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.WMA10", 27))
    {
      // WMA10
      //DANIEL_HACK
      AUD_WMA_CFG_T rWMAProInfo = {0};
      AUD_DspWMAProHeader(AUD_DEC_MAIN, &rWMAProInfo);
      //DANIEL_HACK
      _prAudOmxHandle[0].eFormat = AUD_FMT_WMAPRO;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.WMA", 25))
    {
      // WMA
      //DANIEL_HACK
      AUD_WMA_CFG_T rWMAInfo = {0};
      AUD_DspWMAHeader(AUD_DEC_MAIN, &rWMAInfo);
      //DANIEL_HACK
      _prAudOmxHandle[0].eFormat = AUD_FMT_WMA;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.COOK", 26))
    {
      // COOK
      _prAudOmxHandle[0].eFormat = AUD_FMT_COOK;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.VORBIS", 28))
    {
      // VORBIS
      _prAudOmxHandle[0].eFormat = AUD_FMT_VORBISDEC;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.AMRNB", 27))
    {
      // AMRNB
      _prAudOmxHandle[0].eFormat = AUD_FMT_AMRNB;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.AMRWB", 27))
    {
      // AMRWB
      _prAudOmxHandle[0].eFormat = AUD_FMT_AMRWB;
      pHandle = &_prAudOmxHandle[0];
    }
    else if (0 == strncmp(cCompName, "OMX.MTK.Audio.Decoder.DRA", 25))
    {
      // DRA
      _prAudOmxHandle[0].eFormat = AUD_FMT_DRA;
      pHandle = &_prAudOmxHandle[0];
    }
  //FOR_LG_2ND_ATTACK
    else
    {
        SYS_Printf("[AUD][OMX] component not found - %s\n", cCompName);
        omxRet = OMX_ErrorComponentNotFound;
    }

    if (OMX_ErrorNone == omxRet)
    {
        // initialize
        _prAudOmxHandle[0].fgUsed           = TRUE;
        _prAudOmxHandle[0].eState           = OMX_StateLoaded;
        _prAudOmxHandle[0].fgStateTransing  = FALSE;
        _prAudOmxHandle[0].eStateTo         = OMX_StateMax;

        _prAudOmxHandle[0].pid              = current_thread_info()->task->pid;

        _prAudOmxHandle[0].fgEmptying       = FALSE;
        _prAudOmxHandle[0].fgFilling        = FALSE;
        _prAudOmxHandle[0].fgGotEOS         = FALSE;
        _prAudOmxHandle[0].pAppData         = rKArg.pAppData;
        _prAudOmxHandle[0].pEventHandler    = prOmxCb->pEventHandler;       //DANIEL_HACK
        _prAudOmxHandle[0].pEmptyBufferDone = prOmxCb->pEmptyBufferDone;    //DANIEL_HACK
        _prAudOmxHandle[0].pFillBufferDone  = prOmxCb->pFillBufferDone;     //DANIEL_HACK
        _prAudOmxHandle[0].pFifoPntr        = NULL;
        _prAudOmxHandle[0].u4FifoSize       = 0;
        _prAudOmxHandle[0].pFifoWptr        = NULL;
        _prAudOmxHandle[0].pDecInfo         = NULL;

        for (u4Index = 0; u4Index < INPUT_BUF_MAX; u4Index ++)
        {
            _prAudOmxHandle[0].pEmptyQ[u4Index] = NULL;
        }

        for (u4Index = 0; u4Index < OUTPUT_BUF_MAX; u4Index ++)
        {
            _prAudOmxHandle[0].pFillQ[u4Index] = NULL;
        }

        // create task & command queue
        i4AudOmxTaskCreate(&_prAudOmxHandle[0]);
    }

    rKArg.pHandle = pHandle;
    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_GETHANDLE));

    return 0;
}


static void vAudOmxCompDoFreeHandle(AUD_OMX_HANDLE_T *pHandle)
{
    if (pHandle->fgUsed)
    {
        // TODO: for error handling, stop decoder in case not stopped

        // destroy task
        i4AudOmxTaskDestroy(pHandle);

        pHandle->fgUsed = FALSE;
    }
}


INT32 i4AudOmxCompFreeHandle(PARAMETER_OMX_FREEHANDLE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_FREEHANDLE rKArg;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_FREEHANDLE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        omxRet = OMX_ErrorNone;
        vAudOmxCompDoFreeHandle(rKArg.pHandle);
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_FREEHANDLE));

    return i4Ret;
}


INT32 i4AudOmxCompGetState(PARAMETER_OMX_GETSTATE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_GETSTATE rKArg;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_GETSTATE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        omxRet = OMX_ErrorNone;
        i4Ret = copy_to_user(rKArg.pState, (void *)&(_prAudOmxHandle[0].eState), sizeof(OMX_STATETYPE));
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_GETSTATE));

    return i4Ret;
}


static OMX_ERRORTYPE i4AudOmxCompSetStateFromLoaded(AUD_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxToState)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;

    if (OMX_StateIdle == omxToState)
    {
        // allowed
        omxRet = OMX_ErrorNone;

        pHandle->eState = OMX_StateIdle;

    #if 0
        pHandle->eStateTo = OMX_StateIdle;
        pHandle->fgStateTransing = TRUE;

        SYS_Printf("[AUD][OMX] Component state: Loaded -> Idle, asynchronous.\n");
    #endif

        // load decoder
        i4AudOmxPrepareDecoding(pHandle->eFormat,pHandle->pDecInfo);
    }

    return omxRet;
}


static OMX_ERRORTYPE i4AudOmxCompSetStateFromIdle(AUD_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxToState)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;

    if (OMX_StateExecuting == omxToState)
    {
        // allowed
        omxRet = OMX_ErrorNone;
        pHandle->eState = OMX_StateExecuting;

        SYS_Printf("[AUD][OMX] Component state: Idle -> Executing.\n");

        // error detection
        if (fgAudOmxAllInputBufferFreed(pHandle) || fgAudOmxAllOutputBufferFreed(pHandle))
        {
            SYS_Printf("[AUD][OMX] Input buffer or output buffer not allocated, pHandle = 0x%X.\n", pHandle);
        }

        i4AudOmxAllocateAudioFifo(pHandle);

        // start playback (before sending play command, AFIFO SA/EA must be ready)
        i4AudOmxStartDecoding(pHandle, pHandle->pFifoPntr, pHandle->u4FifoSize);
    }
    else if (OMX_StateLoaded == omxToState)
    {
        // 3.1.1.2.2.1 OMX_StateIdle to OMX_StateLoaded
        // On a transition from OMX_StateIdle to OMX_StateLoaded, each buffer supplier shall call
        // OMX_FreeBuffer on the non-supplier port for each buffer residing at the non-supplier port.
        // If the supplier allocated the buffer, it shall free the buffer before calling OMX_FreeBuffer.
        // If the non-supplier port allocated the buffer, it shall free the buffer upon receipt of an
        // OMX_FreeBuffer call. Furthermore, a non-supplier port shall always free the buffer header
        // upon receipt of an OMX_FreeBuffer call. When all of the buffers have been removed from the
        // component, the state transition is complete; the component communicates that the initiating
        // OMX_SendCommand call has completed via a callback event.
        pHandle->eStateTo = OMX_StateLoaded;
        pHandle->fgStateTransing = TRUE;

        SYS_Printf("[AUD][OMX] Component state: Idle -> Loaded, asynchronous.\n");
    }

    return omxRet;
}


static OMX_ERRORTYPE i4AudOmxCompSetStateFromExecuting(AUD_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxToState)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;

    if (OMX_StateIdle == omxToState)
    {
        // allowed
        SYS_Printf("[AUD][OMX] Component state: Executing -> Idle.\n");

        // stop playback
        i4AudOmxStopDecoding(pHandle->eFormat);

        // If the IL client requests a state transition from OMX_StateExecuting to OMX_StateIdle,
        // the component shall return all buffers to their respective suppliers and receive all buffers
        // belonging to its supplier ports before completing the transition. Any port communicating
        // with the IL client shall return any buffers it is holding via EmptyBufferDone and FillBufferDone
        // callbacks, which are used by input and output ports, respectively.
        i4AudOmxForceAllBufEmptyDone(pHandle);
        i4AudOmxForceAllBufFillDone(pHandle);

        omxRet = OMX_ErrorNone;
        pHandle->eState = OMX_StateIdle;
    }

    return omxRet;
}


INT32 i4AudOmxCompSetState(AUD_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxStateTo)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    OMX_STATETYPE omxStateFrom;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

    omxStateFrom = pHandle->eState;

    if (omxStateTo == omxStateFrom)
    {
        omxRet = OMX_ErrorSameState;
    }
    else
    {
        switch (omxStateFrom)
        {
        case OMX_StateLoaded:
            omxRet = i4AudOmxCompSetStateFromLoaded(pHandle, omxStateTo);
            break;
        case OMX_StateIdle:
            omxRet = i4AudOmxCompSetStateFromIdle(pHandle, omxStateTo);
            break;
        case OMX_StateExecuting:
            omxRet = i4AudOmxCompSetStateFromExecuting(pHandle, omxStateTo);
            break;
        default:
            omxRet = OMX_ErrorUndefined;
            break;
        }
    }

    if (pHandle->fgStateTransing)
    {
        // do not callback now, need wait for transiting finished
        return i4Ret;
    }

    // callback for state changed
    if (OMX_ErrorNone == omxRet)
    {
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandStateSet;
        rEvent.nData2     = pHandle->eState;
        rEvent.pEventData = NULL;
    }
    else
    {
        rEvent.eEvent     = OMX_EventError;
        rEvent.nData1     = omxRet;
        rEvent.nData2     = 0;
        rEvent.pEventData = NULL;
    }

    i4AudOmxCompNotifyEvent(pHandle, &rEvent);

    return i4Ret;
}


INT32 i4AudOmxCompCheckStateTransition(AUD_OMX_HANDLE_T *pHandle)
{
    INT32 i4Ret = 0;
    BOOL fgDone = FALSE;
    OMX_STATETYPE omxStateFrom;
    OMX_STATETYPE omxStateTo;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

    omxStateFrom = pHandle->eState;
    omxStateTo   = pHandle->eStateTo;

    if (OMX_StateLoaded == omxStateTo)
    {
        // need check if all buffers are freed
        if (fgAudOmxAllInputBufferFreed(pHandle) && fgAudOmxAllOutputBufferFreed(pHandle))
        {
            fgDone = TRUE;

            SYS_Printf("[AUD][OMX] Component state: asynchronous to Loaded done.\n");
        }
    }
    else if (OMX_StateIdle == omxStateTo)
    {
        // need check if buffers are allocated
        if (!fgAudOmxAllInputBufferFreed(pHandle) && !fgAudOmxAllOutputBufferFreed(pHandle))
        {
            fgDone = TRUE;

            SYS_Printf("[AUD][OMX] Component state: asynchronous to Idle done.\n");
        }
    }

    if (fgDone)
    {
        pHandle->eState          = pHandle->eStateTo;
        pHandle->eStateTo        = OMX_StateMax;
        pHandle->fgStateTransing = FALSE;

        // callback for state changed
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandStateSet;
        rEvent.nData2     = pHandle->eState;
        rEvent.pEventData = NULL;

        i4AudOmxCompNotifyEvent(pHandle, &rEvent);
    }

    return i4Ret;
}


INT32 i4AudOmxCompSetStateAsync(PARAMETER_OMX_COMMAND_STATESET *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_COMMAND_STATESET rKArg;
    AUD_OMX_MSG_T rMsgSetState;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_COMMAND_STATESET));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        // status is valid, proceed to send command to queue
        rMsgSetState.eCmd     = AUD_OMX_CMD_SET_STATE;
        rMsgSetState.eStateTo = rKArg.eStateType;

        i4Ret = i4AudOmxTaskSendCommand(rKArg.pHandle, &rMsgSetState);
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_COMMAND_STATESET));

    return i4Ret;
}


// When the IL client flushes a non-tunnelling port, that port shall return all buffers it is holding
// to the IL client using EmptyBufferDone and FillBufferDone (appropriate for an input port or an output
// port, respectively) to return the buffers.
// For each port that the component successfully flushes, the component shall send an OMX_EventCmdComplete
// event, indicating OMX_CommandFlush for nData1 and the individual port index for nData2, even if the
// flush resulted from using a value of OMX_ALL for nParam.
INT32 i4AudOmxCompFlush(AUD_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput)
{
    INT32 i4Ret = 0;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

    if (fgInput)
    {
        if (u4AOmxDbgMsk & 0x00000040)
        {
            Printf("i4AudOmxCompFlush(I)\n");
        }

        // flush all pending empty buffer request
        i4AudOmxForceAllBufEmptyDone(pHandle);
        //2011/7/28 for flush cmd
        i4AudOmxForceAllBufFillDone(pHandle);
        AUD_DSPCmdStop(AUD_DEC_MAIN);
        u1AudDspState = 0;
        fgRecvFlushCmd = TRUE;
        //End

        // callback for flush done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandFlush;
        rEvent.nData2     = 0;
        rEvent.pEventData = NULL;

        i4AudOmxCompNotifyEvent(pHandle, &rEvent);
    }

    if (fgOutput)
    {
        if (u4AOmxDbgMsk & 0x00000040)
        {
            Printf("i4AudOmxCompFlush(O)\n");
        }

        // flush all pending fill buffer request
        i4AudOmxForceAllBufFillDone(pHandle);

        // callback for flush done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandFlush;
        rEvent.nData2     = 1;
        rEvent.pEventData = NULL;

        i4AudOmxCompNotifyEvent(pHandle, &rEvent);
    }

    return i4Ret;
}


// This IL client calls this command to flush one or more component ports.
// nParam specifies the index of the port to flush. If the value of nParam is OMX_ALL, the component
// shall flush all ports.
INT32 i4AudOmxCompFlushAsync(PARAMETER_OMX_COMMAND_PORT *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_COMMAND_PORT rKArg;
    AUD_OMX_MSG_T rMsgFlush;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        // status is valid, proceed to send command to queue
        rMsgFlush.eCmd     = AUD_OMX_CMD_FLUSH;
        rMsgFlush.fgInput  = (0 == rKArg.nPorts) || (OMX_ALL == rKArg.nPorts);
        rMsgFlush.fgOutput = (1 == rKArg.nPorts) || (OMX_ALL == rKArg.nPorts);

        i4Ret = i4AudOmxTaskSendCommand(rKArg.pHandle, &rMsgFlush);
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    return i4Ret;
}


INT32 i4AudOmxCompPortDisable(AUD_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput)
{
    INT32 i4Ret = 0;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

    if (fgInput)
    {
        // flush all pending empty buffer request
        i4AudOmxForceAllBufEmptyDone(pHandle);

        // TODO: wait all buffers freed before callback command complete

        // callback for port disabling done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandPortDisable;
        rEvent.nData2     = 0;
        rEvent.pEventData = NULL;

        i4AudOmxCompNotifyEvent(pHandle, &rEvent);
    }

    if (fgOutput)
    {
        // flush all pending fill buffer request
        i4AudOmxForceAllBufFillDone(pHandle);

        // TODO: wait all buffers freed before callback command complete

        // callback for port disabling done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandPortDisable;
        rEvent.nData2     = 1;
        rEvent.pEventData = NULL;

        i4AudOmxCompNotifyEvent(pHandle, &rEvent);
    }

    return i4Ret;
}


INT32 i4AudOmxCompPortDisableAsync(PARAMETER_OMX_COMMAND_PORT *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_COMMAND_PORT rKArg;
    AUD_OMX_MSG_T rMsgFlush;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        // status is valid, proceed to send command to queue
        rMsgFlush.eCmd     = AUD_OMX_CMD_PORT_DISABLE;
        rMsgFlush.fgInput  = (0 == rKArg.nPorts) || (OMX_ALL == rKArg.nPorts);
        rMsgFlush.fgOutput = (1 == rKArg.nPorts) || (OMX_ALL == rKArg.nPorts);

        // TODO: modify port definition bEnabled flag

        i4Ret = i4AudOmxTaskSendCommand(rKArg.pHandle, &rMsgFlush);
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    return i4Ret;
}


INT32 i4AudOmxCompPortEnable(AUD_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput)
{
    INT32 i4Ret = 0;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

    if (fgInput)
    {
        // TODO:

        // callback for port enabling done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandPortEnable;
        rEvent.nData2     = 0;
        rEvent.pEventData = NULL;

        i4AudOmxCompNotifyEvent(pHandle, &rEvent);
    }

    if (fgOutput)
    {
        // TODO:

        // callback for port enabling done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandPortEnable;
        rEvent.nData2     = 1;
        rEvent.pEventData = NULL;

        i4AudOmxCompNotifyEvent(pHandle, &rEvent);
    }

    return i4Ret;
}


INT32 i4AudOmxCompPortEnableAsync(PARAMETER_OMX_COMMAND_PORT *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_COMMAND_PORT rKArg;
    AUD_OMX_MSG_T rMsgFlush;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        // status is valid, proceed to send command to queue
        rMsgFlush.eCmd     = AUD_OMX_CMD_PORT_ENABLE;
        rMsgFlush.fgInput  = (0 == rKArg.nPorts) || (OMX_ALL == rKArg.nPorts);
        rMsgFlush.fgOutput = (1 == rKArg.nPorts) || (OMX_ALL == rKArg.nPorts);

        // TODO: modify port definition bEnabled flag

        i4Ret = i4AudOmxTaskSendCommand(rKArg.pHandle, &rMsgFlush);
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    return i4Ret;
}


INT32 i4AudOmxCompAllocateBuffer(PARAMETER_OMX_ALLOCATEBUFFER *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;
    PARAMETER_OMX_ALLOCATEBUFFER rKArg;
    AUD_OMX_BUFFER_T *pBuffer = NULL; // internal buffer handle
    AUD_OMX_MSG_T rMsgAllocated;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_ALLOCATEBUFFER));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        if (0 == rKArg.nPortIndex)
        {
            // input port
            SYS_Printf("[AUD][OMX] To allocate input buffer, size = %d bytes\n", rKArg.nSizeBytes);
            i4AudOmxAllocInputBuf(rKArg.pHandle, rKArg.nSizeBytes, rKArg.nPortIndex, &pBuffer);
        }
        else if (1 == rKArg.nPortIndex)
        {
            // output port
            SYS_Printf("[AUD][OMX] To allocate output buffer, size = %d bytes\n", rKArg.nSizeBytes);
            i4AudOmxAllocOutputBuf(rKArg.pHandle, rKArg.nSizeBytes, rKArg.nPortIndex, &pBuffer);
        }

        if (NULL != pBuffer)
        {
            rKArg.hPBufHeaderBuf = pBuffer->hBuffer;
            rKArg.hPBufHeader    = pBuffer->hHeader;

            // fill the private data
            pBuffer->rHeader.pAppPrivate = rKArg.pAppPrivate;

            rMsgAllocated.eCmd = AUD_OMX_CMD_BUFFER_ALLOCATED;
            i4AudOmxTaskSendCommand(rKArg.pHandle, &rMsgAllocated);

            SYS_Printf("[AUD][OMX] Buffer allocated: SA = 0x%X, pBuffer->pOwner = 0x%X.\n", pBuffer->pBufSA, pBuffer->pOwner);

            omxRet = OMX_ErrorNone;
        }
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_ALLOCATEBUFFER));

    return i4Ret;
}


INT32 i4AudOmxCompFreeBuffer(PARAMETER_OMX_FREEBUFFER *prUArg)
{
    INT32 i4Ret = AUD_FAIL;
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;
    PARAMETER_OMX_FREEBUFFER rKArg;
    AUD_OMX_MSG_T rMsgFreed;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_FREEBUFFER));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        if (0 == rKArg.nPortIndex)
        {
            // input port
            i4Ret = i4AudOmxFreeInputBuf(rKArg.pHandle, rKArg.hPBufHeader);
        }
        else if (1 == rKArg.nPortIndex)
        {
            // output port
            i4Ret = i4AudOmxFreeOutputBuf(rKArg.pHandle, rKArg.hPBufHeader);
        }

        if (AUD_OK == i4Ret)
        {
            // trigger main task for checking if state transition is done
            rMsgFreed.eCmd = AUD_OMX_CMD_BUFFER_FREED;
            i4AudOmxTaskSendCommand(rKArg.pHandle, &rMsgFreed);

            omxRet = OMX_ErrorNone;
        }
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_FREEBUFFER));

    return i4Ret;
}


// The OMX_EmptyThisBuffer macro will send a filled buffer to an input port of a component.
// When the buffer contains data, the value of the nFilledLen field of the buffer header will not be zero.
// If the buffer contains no data, the value of nFilledLen is 0x0. The OMX_EmptyThisBuffer macro is
// invoked to pass buffers containing data when the component is in or making a transition to the
// OMX_StateExecuting or in the OMX_StatePause state.
INT32 i4AudOmxCompEmptyBuffer(PARAMETER_OMX_EMPTYTHISBUFFER *prUArg)
{
    INT32 i4Ret = AUD_FAIL;
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;
    PARAMETER_OMX_EMPTYTHISBUFFER rKArg;
    AUD_OMX_MSG_T rMsgEmptyBuf;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_EMPTYTHISBUFFER));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        // status is valid, proceed to send command to queue
        rMsgEmptyBuf.eCmd    = AUD_OMX_CMD_EMPTY_BUFFER;
        rMsgEmptyBuf.hHeader = rKArg.hPBufHeader;

        // this will trigger i4AudOmxQueueEmptyBuf() in main task
        i4Ret = i4AudOmxTaskSendCommand(rKArg.pHandle, &rMsgEmptyBuf);

        omxRet = OMX_ErrorNone;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_EMPTYTHISBUFFER));

    return i4Ret;
}


// The OMX_FillThisBuffer macro will send an empty buffer to an output port of a component.
// The OMX_FillThisBuffer macro is invoked to pass buffers containing no data when the component is in
// or making a transition to the OMX_StateExecuting state or is in the OMX_StatePaused state.
INT32 i4AudOmxCompFillBuffer(PARAMETER_OMX_FILLTHISBUFFER *prUArg)
{
    INT32 i4Ret = AUD_FAIL;
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;
    PARAMETER_OMX_FILLTHISBUFFER rKArg;
    AUD_OMX_MSG_T rMsgFillBuf;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_FILLTHISBUFFER));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        // status is valid, proceed to send command to queue
        rMsgFillBuf.eCmd    = AUD_OMX_CMD_FILL_BUFFER;
        rMsgFillBuf.hHeader = rKArg.hPBufHeader;

        i4Ret = i4AudOmxTaskSendCommand(rKArg.pHandle, &rMsgFillBuf);

        omxRet = OMX_ErrorNone;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_FILLTHISBUFFER));

    return i4Ret;
}


// A component uses the EmptyBufferDone callback to pass a buffer from an input port back to the IL client.
// A component sets the nOffset and nFilledLen values of the buffer header to reflect the portion of the
// buffer it consumed; for example, nFilledLen is set equal to 0x0 if completely consumed.
// The EmptyBufferDone call is a blocking call that should return from within five milliseconds.
INT32 i4AudOmxCompNotifyEmptyDone(AUD_OMX_HANDLE_T *pHandle, AUD_OMX_BUFFER_T *pBufEmptied)
{
    PARAMETER_OMX_CB_BUFFER rCbInfo;

    pBufEmptied->rHeader.nFilledLen = pBufEmptied->u4FilledLen;

    x_memset(&rCbInfo, 0, sizeof(PARAMETER_OMX_CB_BUFFER)); //New Added
    rCbInfo.hPBufHeader = pBufEmptied->hHeader;
    rCbInfo.pPlatformPrivate = pBufEmptied->rHeader.pPlatformPrivate; //New Added

    //SYS_Printf("[AUD][OMX] i4AudOmxCompNotifyEmptyDone: pBufEmptied = 0x%X, pid = %d, pHandle->pEmptyBufferDone = 0x%X.\n",
    //           pBufEmptied, pHandle->pid, pHandle->pEmptyBufferDone);

    #if 1 //DANIEL_HACK
    if(pHandle && pHandle->pEmptyBufferDone)
    {
        pHandle->pEmptyBufferDone(&rCbInfo, pHandle->pAppData);
    }
    #else
    // since it is a blocking callback, use sync callback agent
    vCBAgent_SyncCb_Nfy_EX(pHandle->pEmptyBufferDone,
                           pHandle->pAppData,
                           &rCbInfo,
                           pHandle->pid,
                           DRVT_AUD_DEC,
                           sizeof(PARAMETER_OMX_CB_BUFFER));
    #endif

    return AUD_OK;
}


// A component uses the FillBufferDone callback to pass a buffer from an output port back to the IL client.
// A component sets the nOffset and nFilledLen of the buffer header to reflect the portion of the buffer
// it filled (for example, nFilledLen is equal to 0x0 if it contains no data).
// The FillBufferDone call is a blocking call that should return from within five milliseconds.
INT32 i4AudOmxCompNotifyFillDone(AUD_OMX_HANDLE_T *pHandle, AUD_OMX_BUFFER_T *pBufFilled)
{
    PARAMETER_OMX_CB_BUFFER rCbInfo;

    pBufFilled->rHeader.nFilledLen = pBufFilled->u4FilledLen;
    pBufFilled->rHeader.nOffset    = 0;

    x_memset(&rCbInfo, 0, sizeof(PARAMETER_OMX_CB_BUFFER)); //New Added
    rCbInfo.hPBufHeader = pBufFilled->hHeader;
    rCbInfo.pPlatformPrivate = pBufFilled->rHeader.pPlatformPrivate; //New Added

    //SYS_Printf("[AUD][OMX] i4AudOmxCompNotifyFillDone: pBufFilled = 0x%X, pid = %d, pHandle->pFillBufferDone = 0x%X.\n",
    //           pBufFilled, pHandle->pid, pHandle->pFillBufferDone);

    //SYS_Printf("[AUD][OMX] i4AudOmxCompNotifyFillDone: nFilledLen = %d.\n", pBufFilled->rHeader.nFilledLen);

    #if 1 //DANIEL_HACK
    if(pHandle && pHandle->pFillBufferDone)
    {
        pHandle->pFillBufferDone(&rCbInfo, pHandle->pAppData);
    }
    #else
    vCBAgent_SyncCb_Nfy_EX(pHandle->pFillBufferDone,
                           pHandle->pAppData,
                           &rCbInfo,
                           pHandle->pid,
                           DRVT_AUD_DEC,
                           sizeof(PARAMETER_OMX_CB_BUFFER));
    #endif

    return AUD_OK;
}


// A call to EventHandler is a blocking call.
INT32 i4AudOmxCompNotifyEvent(AUD_OMX_HANDLE_T *pHandle, PARAMETER_OMX_CB_EVENTHANDLER *pEventInfo)
{
    //SYS_Printf("[AUD][OMX] i4AudOmxCompNotifyEvent: pid = %d, pHandle->pEventHandler = 0x%X.\n",
    //           pHandle->pid, pHandle->pEventHandler);

    #if 1 //DANIEL_HACK
    if(pHandle && pHandle->pEventHandler)
    {
        //SYS_Printf("[i4AudOmxCompNotifyEvent: pHandle(0x%08x) pEventInfo(0x%08x) pAppData(0x%08x)]\n", pHandle, pEventInfo, pHandle->pAppData);
        pHandle->pEventHandler(pEventInfo, pHandle->pAppData);
    }
    #else
    vCBAgent_SyncCb_Nfy_EX(pHandle->pEventHandler,
                           pHandle->pAppData,
                           pEventInfo,
                           pHandle->pid,
                           DRVT_AUD_DEC,
                           sizeof(PARAMETER_OMX_CB_EVENTHANDLER));
    #endif

    return AUD_OK;
}


INT32 i4AudOmxCompGetParamAudioInit(PARAMETER_OMX_PORT_PARAM_TYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;
    PARAMETER_OMX_PORT_PARAM_TYPE rKArg;
    OMX_PORT_PARAM_TYPE rPortInfo;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_PORT_PARAM_TYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rPortInfo, rKArg.pPortParamType, sizeof(OMX_PORT_PARAM_TYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            // Set Parameter
            SYS_Printf("[AUD][OMX] set OMX_IndexParamAudioInit?\n");
        }
        else
        {
            // Get Parameter
            rPortInfo.nPorts = 2; // 2 ports: 0 - input, 1 - output
            rPortInfo.nStartPortNumber = 0;

            omxRet = OMX_ErrorNone;

            i4Ret = copy_to_user(rKArg.pPortParamType, (void *)&rPortInfo, sizeof(OMX_PORT_PARAM_TYPE));
        }
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_PORT_PARAM_TYPE));

    return i4Ret;
}


INT32 i4AudOmxCompParamPortDef(PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE rKArg;
    OMX_PARAM_PORTDEFINITIONTYPE rPortDef;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rPortDef, rKArg.pPortDefinitionType, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            // Set Parameter
            SYS_Printf("[AUD][OMX] set OMX_IndexParamPortDefinition, not processed.\n");
        }
        else
        {
            // Get Parameter
            //SYS_Printf("[AUD][OMX] get OMX_IndexParamPortDefinition, part implemented.\n");

            if (0 == rPortDef.nPortIndex)
            {
                rPortDef.nBufferCountActual = MAX_INPUT_BUF_NO; //DANIEL_HACK, 2011/5/16, change to 8/8 for Leon GST AVI playback performance bad issue.
                rPortDef.nBufferCountMin = MAX_INPUT_BUF_NO;

                // input port
                switch (_prAudOmxHandle[0].eFormat)
                {
                case AUD_FMT_MP3:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingMP3;
                    break;
                case AUD_FMT_AAC:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingAAC;
                    break;
              //FOR_LG_2ND_ATTACK
                case AUD_FMT_PCM:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingPCM;
                    break;
                case AUD_FMT_MPEG:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingMPEG;
                    break;
                case AUD_FMT_AC3:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingAC3;
                    break;
                case AUD_FMT_LPCM:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingADPCM;
                    break;
                case AUD_FMT_OMX_MSADPCM:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingADPCM;
                    break;
                case AUD_FMT_OMX_IMAADPCM:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingADPCM;
                    break;
                case AUD_FMT_OMX_ALAW:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingPCM;
                    break;
                case AUD_FMT_OMX_ULAW:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingPCM;
                    break;
                case AUD_FMT_DTS:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingDTS;
                    break;
                case AUD_FMT_WMA:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingWMA;
                    break;
                case AUD_FMT_WMAPRO:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingWMA;
                    break;
                case AUD_FMT_WMA10LOSSLESS:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingWMA;
                    break;
                case AUD_FMT_WMA10SPEECH:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingWMA;
                    break;
                case AUD_FMT_COOK:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingRA;
                    break;
                case AUD_FMT_VORBISDEC:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingVORBIS;
                    break;
                case AUD_FMT_AMRNB:
                case AUD_FMT_AMRWB:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingAMR;
                    break;
                case AUD_FMT_DRA:
                    rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingDRA;
                    break;
              //FOR_LG_2ND_ATTACK
                default:
                    ASSERT(0);
                    break;
                }

                rPortDef.eDir = OMX_DirInput;
                rPortDef.nBufferSize = MAX_INPUT_BUF_SIZE;
                rPortDef.eDomain = OMX_PortDomainAudio; //DANIEL_HACK
            }
            else
            {
                rPortDef.nBufferCountActual = MAX_OUTPUT_BUF_NO; //DANIEL_HACK
                rPortDef.nBufferCountMin = MAX_OUTPUT_BUF_NO;

                // output port
                rPortDef.format.audio.eEncoding = OMX_AUDIO_CodingPCM;
                rPortDef.eDir = OMX_DirOutput;
                rPortDef.nBufferSize = MAX_OUTPUT_BUF_SIZE; //DANIEL_HACK
                rPortDef.eDomain = OMX_PortDomainAudio; //DANIEL_HACK
            }

            i4Ret = copy_to_user(rKArg.pPortDefinitionType, (void *)&rPortDef, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE));

    return i4Ret;
}


INT32 i4AudOmxCompParamMP3(PARAMETER_OMX_AUDIO_PARAM_MP3TYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_AUDIO_PARAM_MP3TYPE rKArg;
    OMX_AUDIO_PARAM_MP3TYPE rMp3Param;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_MP3TYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rMp3Param, rKArg.pMP3Type, sizeof(OMX_AUDIO_PARAM_MP3TYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            // Set Parameter
            SYS_Printf("[AUD][OMX] set MP3 parameters? port = %d (not yet implement)\n", rMp3Param.nPortIndex);
        }
        else
        {
            // Get Parameter
            SYS_Printf("[AUD][OMX] get MP3 parameter, port = %d\n", rMp3Param.nPortIndex);

            i4AudOmxGetParameterMP3(&rMp3Param);

            i4Ret = copy_to_user(rKArg.pMP3Type, (void *)&rMp3Param, sizeof(OMX_AUDIO_PARAM_MP3TYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_MP3TYPE));

    return i4Ret;
}

INT32 i4AudOmxCompParamAAC(PARAMETER_OMX_AUDIO_PARAM_AACPROFILETYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_AUDIO_PARAM_AACPROFILETYPE rKArg;
    OMX_AUDIO_PARAM_AACPROFILETYPE rAacParam;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_AACPROFILETYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rAacParam, rKArg.pAacProfFileType, sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            // Set Parameter
            if (rAacParam.nChannels)
            {
                u1ChNo = rAacParam.nChannels;
            }
            else
            {
                u1ChNo = 2;
            }

            switch (rAacParam.nSampleRate)
            {
                case 96000:
                    u1SampleRateIdx = 0;
                    break;
                case 88200:
                    u1SampleRateIdx = 1;
                    break;
                case 64000:
                    u1SampleRateIdx = 2;
                    break;
                case 48000:
                    u1SampleRateIdx = 3;
                    break;
                case 44100:
                    u1SampleRateIdx = 4;
                    break;
                case 32000:
                    u1SampleRateIdx = 5;
                    break;
                case 24000:
                    u1SampleRateIdx = 6;
                    break;
                case 22050:
                    u1SampleRateIdx = 7;
                    break;
                case 16000:
                    u1SampleRateIdx = 8;
                    break;
                case 12000:
                    u1SampleRateIdx = 9;
                    break;
                case 11025:
                    u1SampleRateIdx = 10;
                    break;
                case 8000:
                    u1SampleRateIdx = 11;
                    break;
                default: //48K
                    u1SampleRateIdx = 3;
                    break;
            }
            Printf("Set AAC Type (nChannels: %d/nSampleRate: %d) ChNo: %d/SampleRate: %d\n",
                    rAacParam.nChannels,
                    rAacParam.nSampleRate,
                    u1ChNo, u1SampleRateIdx);
        }
        else
        {
            // Get Parameter
            SYS_Printf("[AUD][OMX] get AAC parameter, port = %d (not yet implement)\n", rAacParam.nPortIndex);

            i4Ret = copy_to_user(rKArg.pAacProfFileType, (void *)&rAacParam, sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_AACPROFILETYPE));

    return i4Ret;
}

//DANIEL_HACK
INT32 i4AudOmxCompParamPCM(PARAMETER_OMX_AUDIO_PARAM_PCMMODETYPE * prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_AUDIO_PARAM_PCMMODETYPE rKArg;
    OMX_AUDIO_PARAM_PCMMODETYPE rPcmParam;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_PCMMODETYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rPcmParam, rKArg.pPcmModeType, sizeof(OMX_AUDIO_PARAM_PCMMODETYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            // Set Parameter
            Printf("[AUD][OMX] set PCM parameter: Rate:%d/CH:%d\n",
                    rPcmParam.nSamplingRate, rPcmParam.nChannels);

            {
                AUD_PCM_SETTING_T rPCMInfo = {0};
                AUD_LPCM_INFO_T rLPCMInfo = {0};

                //eSampleFreq
                switch (rPcmParam.nSamplingRate/1000)
                {
                     case 48:
                         rLPCMInfo.eSampleFreq = FS_48K;
                         break;
                     case 32:
                         rLPCMInfo.eSampleFreq = FS_32K;
                         break;
                     case 44:
                         rLPCMInfo.eSampleFreq = FS_44K;
                         break;
                     case 8:
                         rLPCMInfo.eSampleFreq = FS_8K;
                         break;
                     case 11:
                         rLPCMInfo.eSampleFreq = FS_11K;
                         break;
                     case 12:
                         rLPCMInfo.eSampleFreq = FS_12K;
                         break;
                    case 16:
                         rLPCMInfo.eSampleFreq = FS_16K;
                         break;
                     case 22:
                         rLPCMInfo.eSampleFreq = FS_22K;
                         break;
                     case 24:
                         rLPCMInfo.eSampleFreq = FS_24K;
                         break;
                     default:
                         rLPCMInfo.eSampleFreq = FS_48K;
                         break;
                }

                rLPCMInfo.ui2_num_ch = rPcmParam.nChannels;

                switch (_prAudOmxHandle[0].eFormat)
                {
                    case AUD_FMT_LPCM:
                        Printf("[AUD][OMX] AUD_FMT_LPCM\n");
                        rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                        if (rPcmParam.nChannels==1)
                        {
                            rLPCMInfo.ui2_block_align = 2;
                        }
                        else
                        {
                            rLPCMInfo.ui2_block_align = 4;
                        }
                        rLPCMInfo.ui2_bits_per_sample = 16;
                        rLPCMInfo.b_big_endian = 1;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.b_signed_data = FALSE;
                        break;
                    case AUD_FMT_OMX_MSADPCM:
                        Printf("[AUD][OMX] AUD_FMT_OMX_MSADPCM\n");
                        rLPCMInfo.eLPcmType = LPCM_TYPE_MS_ADPCM;
                        rLPCMInfo.ui2_block_align = 2048; //TODO
                        switch (rPcmParam.nSamplingRate)
                        {
                            case 24000:
                            case 22050:
                            case 16000:
                                rLPCMInfo.ui2_block_align /= 2;
                                break;
                            case 12000:
                            case 11025:
                            case 8000:
                                rLPCMInfo.ui2_block_align /= 4;
                                break;
                        }
                        if (rPcmParam.nChannels == 1)
                        {
                            rLPCMInfo.ui2_block_align /= 2;
                        }
                        rLPCMInfo.ui2_bits_per_sample = 4;
                        rLPCMInfo.b_big_endian = 1;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.b_signed_data = FALSE;
                        break;
                    case AUD_FMT_OMX_IMAADPCM:
                        Printf("[AUD][OMX] AUD_FMT_OMX_IMAADPCM\n");
                        rLPCMInfo.eLPcmType = LPCM_TYPE_IMA_ADPCM;
                        rLPCMInfo.ui2_block_align = 2048; //TODO
                        switch (rPcmParam.nSamplingRate)
                        {
                            case 24000:
                            case 22050:
                            case 16000:
                                rLPCMInfo.ui2_block_align /= 2;
                                break;
                            case 12000:
                            case 11025:
                            case 8000:
                                rLPCMInfo.ui2_block_align /= 4;
                                break;
                        }
                        if (rPcmParam.nChannels == 1)
                        {
                            rLPCMInfo.ui2_block_align /= 2;
                        }

                        rLPCMInfo.ui2_bits_per_sample = 4;
                        rLPCMInfo.b_big_endian = 1;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.b_signed_data = FALSE;
                        break;
                    case AUD_FMT_OMX_ALAW:
                        Printf("[AUD][OMX] AUD_FMT_OMX_ALAW\n");
                        rLPCMInfo.eLPcmType = LPCM_TYPE_A_LAW;
                        if (rPcmParam.nChannels==1)
                        {
                            rLPCMInfo.ui2_block_align = 1;
                        }
                        else
                        {
                            rLPCMInfo.ui2_block_align = 2;
                        }
                        rLPCMInfo.ui2_bits_per_sample = 8;
                        rLPCMInfo.b_big_endian = 1;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.b_signed_data = FALSE;
                        break;
                    case AUD_FMT_OMX_ULAW:
                        Printf("[AUD][OMX] AUD_FMT_OMX_ULAW\n");
                        rLPCMInfo.eLPcmType = LPCM_TYPE_U_LAW;
                        if (rPcmParam.nChannels==1)
                        {
                            rLPCMInfo.ui2_block_align = 1;
                        }
                        else
                        {
                            rLPCMInfo.ui2_block_align = 2;
                        }
                        rLPCMInfo.ui2_bits_per_sample = 8;
                        rLPCMInfo.b_big_endian = 1;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.b_signed_data = FALSE;
                        break;                   
                    default:
                        break;
                }

                //MTK G-Streamer Proprietary Interface
                if ((((rPcmParam.nVersion.nVersion>>24)&0xff) == 'M') &&
                   (((rPcmParam.nVersion.nVersion>>16)&0xff) == 'T'))
                {
                    Printf("Get MTK GST ui2_block_align interface\n");
                    rLPCMInfo.ui2_block_align = (rPcmParam.nVersion.nVersion&0xffff);
                }
                //End

                rPCMInfo.eSampleFreq = rLPCMInfo.eSampleFreq;
                if (rLPCMInfo.b_big_endian)
                {
                    rPCMInfo.ePcmDataInfo = PCM_16_BITS_BIG_ENDIAN;
                }
                else
                {
                    rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                }

                Printf("eLPcmType: %d\n", rLPCMInfo.eLPcmType);
                Printf("ui2_num_ch: %d\n", rLPCMInfo.ui2_num_ch);
                Printf("ui2_block_align: %d\n", rLPCMInfo.ui2_block_align);
                Printf("ui2_bits_per_sample: %d\n", rLPCMInfo.ui2_bits_per_sample);
                Printf("b_big_endian: %d\n", rLPCMInfo.b_big_endian);
                Printf("b_info_not_specified: %d\n", rLPCMInfo.b_info_not_specific);
                Printf("b_signed_data: %d\n", rLPCMInfo.b_signed_data);


                AUD_PcmSetting(AUD_DEC_MAIN, &rPCMInfo);
                AUD_LPcmSetting(AUD_DEC_MAIN, &rLPCMInfo);
            }
        }
        else
        {
            // Get Parameter
            SYS_Printf("[AUD][OMX] get PCM parameter, port = %d\n", rPcmParam.nPortIndex);

            rPcmParam.nChannels = 2;
            rPcmParam.eNumData = OMX_NumericalDataSigned;
            rPcmParam.eEndian = OMX_EndianLittle;
            rPcmParam.bInterleaved = OMX_TRUE;
            rPcmParam.nBitPerSample = 16;
            rPcmParam.nSamplingRate = 48000;
            rPcmParam.ePCMMode = OMX_AUDIO_PCMModeLinear;

            i4Ret = copy_to_user(rKArg.pPcmModeType, (void *)&rPcmParam, sizeof(OMX_AUDIO_PARAM_PCMMODETYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_PCMMODETYPE));

    return i4Ret;

}

UINT8 bOmxSettingBypass = 0;
INT32 i4AudOmxCompParamWMA(PARAMETER_OMX_AUDIO_PARAM_WMATYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_AUDIO_PARAM_WMATYPE rKArg;
    OMX_AUDIO_PARAM_WMATYPE rWmaParam;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_WMATYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rWmaParam, rKArg.pWMAType, sizeof(OMX_AUDIO_PARAM_WMATYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            if (bOmxSettingBypass == 1)
            {
                Printf("[AUD][OMX] Bypass WMA Setting\n");
            }
            else
            {
                AUD_WMA_CFG_T rWMAInfo = {0};

                // Set Parameter
                Printf("[AUD][OMX] set WMA parameters? port = %d (not yet implement)\n", rWmaParam.nPortIndex);
                Printf("  nBlockAlign: %x\n", rWmaParam.nBlockAlign);
                Printf("  nChannels: %d\n", rWmaParam.nChannels);
                Printf("  nSamplingRate: %d\n", rWmaParam.nSamplingRate);
                Printf("  nBitrate: %d\n", rWmaParam.nBitRate);
                Printf("  nEncodeOptions: %x\n", rWmaParam.nEncodeOptions);

                rWMAInfo.u2HdrSrc = 1;
                rWMAInfo.u4Pktsz = rWmaParam.nBlockAlign;
                rWMAInfo.u2Numch = rWmaParam.nChannels;
                rWMAInfo.u4Sampersec = rWmaParam.nSamplingRate;
                rWMAInfo.u4Bpersec = rWmaParam.nBitRate/8;
                rWMAInfo.u4Blocksz = rWmaParam.nBlockAlign;
                rWMAInfo.u2Encoderopt = rWmaParam.nEncodeOptions;
                if (rWMAInfo.u2Encoderopt == 0)
                {
                    Printf("  u2Encoderopt not specified use default 0xf\n");
                    rWMAInfo.u2Encoderopt = 0xf;
                }
                AUD_DspWMAHeader(AUD_DEC_MAIN, &rWMAInfo);
            }
        }
        else
        {
            // Get Parameter
            Printf("[AUD][OMX] get WMA parameter?, port = %d (not yet implement)\n", rWmaParam.nPortIndex);
            i4Ret = copy_to_user(rKArg.pWMAType, (void *)&rWmaParam, sizeof(OMX_AUDIO_PARAM_WMATYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_WMATYPE));

    return i4Ret;
}

INT32 i4AudOmxCompParamWMAPRO(PARAMETER_OMX_AUDIO_PARAM_WMAPROTYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_AUDIO_PARAM_WMAPROTYPE rKArg;
    OMX_AUDIO_PARAM_WMAPROTYPE rWmaProParam;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_WMAPROTYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rWmaProParam, rKArg.pWMAPROType, sizeof(OMX_AUDIO_PARAM_WMAPROTYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            if (bOmxSettingBypass == 1)
            {
                Printf("[AUD][OMX] Bypass WMAPRO Setting\n");
            }
            else
            {
                AUD_WMA_CFG_T rWMAPROInfo = {0};
                AUD_LPCM_INFO_T rLPCMInfo = {0};

                // Set Parameter
                Printf("[AUD][OMX] set WMAPRO parameters? port = %d (not yet implement)\n", rWmaProParam.nPortIndex);
                Printf("  nBlockAlign: %x\n", rWmaProParam.nBlockAlign);
                Printf("  nChannels: %d\n", rWmaProParam.nChannels);
                Printf("  nSamplingRate: %d\n", rWmaProParam.nSamplingRate);
                Printf("  nBitrate: %d\n", rWmaProParam.nBitRate);
                Printf("  nEncodeOptions: %x\n", rWmaProParam.nEncodeOptions);
                Printf("  nChannelMask: %x\n", rWmaProParam.nChannelMask);
                Printf("  nBitsPerSample: %x\n", rWmaProParam.nBitsPerSample);
                Printf("  nAdvEncodeOpt: %x\n", rWmaProParam.nAdvEncodeOpt);
                Printf("  nAdvEncodeOpt2: %x\n", rWmaProParam.nAdvEncodeOpt2);
                Printf("  u4VoiceEncodeOptions1: %x\n", rWmaProParam.nVoiceEncodeOptions1);
                Printf("  u4VoiceEncodeOptions2[0] : %x\n", rWmaProParam.nVoiceEncodeOptions2[0]);
                Printf("  u4VoiceEncodeOptions2[1] : %x\n", rWmaProParam.nVoiceEncodeOptions2[1]);

                rWMAPROInfo.u2HdrSrc = 1;
                rWMAPROInfo.u4Pktsz = rWmaProParam.nBlockAlign;
                rWMAPROInfo.u2Numch = rWmaProParam.nChannels;
                rWMAPROInfo.u4Sampersec = rWmaProParam.nSamplingRate;
                rWMAPROInfo.u4Bpersec = rWmaProParam.nBitRate/8;
                rWMAPROInfo.u4Blocksz = rWmaProParam.nBlockAlign;
                rWMAPROInfo.u2Encoderopt = rWmaProParam.nEncodeOptions;
                rWMAPROInfo.u4ChannelMask = rWmaProParam.nChannelMask;
                rWMAPROInfo.u2BitsPerSample = rWmaProParam.nBitsPerSample;
                rWMAPROInfo.u2AdvEncoderopt = rWmaProParam.nAdvEncodeOpt;
                rWMAPROInfo.u4AdvEncoderopt2= rWmaProParam.nAdvEncodeOpt2;
                //for WMA Voice
                rWMAPROInfo.u4VoiceEncodeOptions1= rWmaProParam.nVoiceEncodeOptions1;
                rWMAPROInfo.u4VoiceEncodeOptions2[0] = rWmaProParam.nVoiceEncodeOptions2[0];
                rWMAPROInfo.u4VoiceEncodeOptions2[1] = rWmaProParam.nVoiceEncodeOptions2[1];
                if (_prAudOmxHandle[0].eFormat == AUD_FMT_WMAPRO)
                {
                    AUD_DspWMAProHeader(AUD_DEC_MAIN, &rWMAPROInfo);
                }
                else if ((_prAudOmxHandle[0].eFormat == AUD_FMT_WMA10LOSSLESS) ||
                             (_prAudOmxHandle[0].eFormat == AUD_FMT_WMA10SPEECH))
                {
                    //RISC decoder, set LPCM
                    if (rWmaProParam.nSamplingRate == 44100)
                    {
                        rLPCMInfo.eSampleFreq = FS_44K;
                    }
                    else if (rWmaProParam.nSamplingRate == 48000)
                    {
                        rLPCMInfo.eSampleFreq = FS_48K;
                    }
                    else if (rWmaProParam.nSamplingRate == 88200)
                    {
                        rLPCMInfo.eSampleFreq = FS_88K;
                    }
                    else if (rWmaProParam.nSamplingRate == 96000)
                    {
                        rLPCMInfo.eSampleFreq = FS_96K;
                    }
                    else if (rWmaProParam.nSamplingRate == 8000)
                    {
                        rLPCMInfo.eSampleFreq = FS_8K;
                    }
                    else if (rWmaProParam.nSamplingRate == 11025)
                    {
                        rLPCMInfo.eSampleFreq = FS_11K;
                    }
                    else if (rWmaProParam.nSamplingRate == 16000)
                    {
                        rLPCMInfo.eSampleFreq = FS_16K;
                    }
                    else if (rWmaProParam.nSamplingRate == 22050)
                    {
                        rLPCMInfo.eSampleFreq = FS_22K;
                    }
                    else
                    {
                         omxRet = OMX_ErrorUndefined;
                    }
                    rLPCMInfo.eLPcmType = 0;
                    rLPCMInfo.ui2_num_ch = rWmaProParam.nChannels;
                    rLPCMInfo.ui2_block_align = 0;
                    rLPCMInfo.ui2_bits_per_sample = 16;
                    rLPCMInfo.b_big_endian = 0;
                    rLPCMInfo.b_info_not_specific = 0;
                    rLPCMInfo.b_signed_data = 0 ;
                    AUD_LPcmSetting(AUD_DEC_MAIN,&rLPCMInfo);

                    //setting for RISC decoder
                     x_memcpy((void*)&_prAudDecInfo, (const void*)&rWMAPROInfo, sizeof(rWMAPROInfo));
                     _prAudOmxHandle[0].pDecInfo = _prAudDecInfo;

                }
                else
                {
                    omxRet = OMX_ErrorUndefined;
                }
            }
        }
        else
        {
            // Get Parameter
            Printf("[AUD][OMX] get WMAPRO parameter?, port = %d (not yet implement)\n", rWmaProParam.nPortIndex);
            i4Ret = copy_to_user(rKArg.pWMAPROType, (void *)&rWmaProParam, sizeof(OMX_AUDIO_PARAM_WMAPROTYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_WMATYPE));

    return i4Ret;
}

UINT16 u2RaFrameSizeInBytes = 0x80;
INT32 i4AudOmxCompParamRA(PARAMETER_OMX_AUDIO_PARAM_RATYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_AUDIO_PARAM_RATYPE rKArg;
    OMX_AUDIO_PARAM_RATYPE rRaParam;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_RATYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rRaParam, rKArg.pRAType, sizeof(OMX_AUDIO_PARAM_RATYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            // Set Parameter
            if (bOmxSettingBypass == 1)
            {
                Printf("[AUD][OMX] Bypass COOK Setting\n");
            }
            else
            {
                AUD_COOK_CFG_T rCookCfg = {0};

                Printf("[AUD][OMX] set RA parameters? port = %d\n", rRaParam.nPortIndex);

                rCookCfg.u2SampPerFrame = rRaParam.nSamplePerFrame;
                rCookCfg.u2ChanNum = rRaParam.nChannels;
                rCookCfg.u2SampRate = rRaParam.nSamplingRate;
                rCookCfg.u2FrameSzInBytes = u2RaFrameSizeInBytes = rRaParam.nBitsPerFrame/8;
                rCookCfg.u2RegnNum = rRaParam.nNumRegions;
                rCookCfg.u2StartRegn = rRaParam.nCouplingStartRegion;
                rCookCfg.u2CplQBits = rRaParam.nCouplingQuantBits;
                Printf("u2SampPerFrame: %d\n", rCookCfg.u2SampPerFrame);
                Printf("u2ChanNum: %d\n", rCookCfg.u2ChanNum);
                Printf("u2SampRate: %d\n", rCookCfg.u2SampRate);
                Printf("u2FrameSzInBytes: %d\n", rCookCfg.u2FrameSzInBytes);
                Printf("u2RegnNum: %d\n", rCookCfg.u2RegnNum);
                Printf("u2StartRegn: %d\n", rCookCfg.u2StartRegn);
                Printf("u2CplQBits: %d\n", rCookCfg.u2CplQBits);
                AUD_DspCOOKParameter(AUD_DEC_MAIN, &rCookCfg);
            }
        }
        else
        {
            // Get Parameter
            Printf("[AUD][OMX] get RA parameter?, port = %d (not yet implement)\n", rRaParam.nPortIndex);
            i4Ret = copy_to_user(rKArg.pRAType, (void *)&rRaParam, sizeof(OMX_AUDIO_PARAM_RATYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_RATYPE));

    return i4Ret;
}

INT32 i4AudOmxCompParamADPCM(PARAMETER_OMX_AUDIO_PARAM_ADPCMTYPE * prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_AUDIO_PARAM_ADPCMTYPE rKArg = {0};
    OMX_AUDIO_PARAM_ADPCMTYPE rAdpcmParam = {0};

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_ADPCMTYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rAdpcmParam, rKArg.pADPCMType, sizeof(OMX_AUDIO_PARAM_ADPCMTYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            // Set Parameter
            Printf("[AUD][OMX] set ADPCM parameter: Rate:%d/CH:%d\n",
                    rAdpcmParam.nSampleRate, rAdpcmParam.nChannels);

            {
                AUD_PCM_SETTING_T rPCMInfo = {0};
                AUD_LPCM_INFO_T rLPCMInfo = {0};

                //eSampleFreq
                switch (rAdpcmParam.nSampleRate/1000)
                {
                     case 48:
                         rLPCMInfo.eSampleFreq = FS_48K;
                         break;
                     case 32:
                         rLPCMInfo.eSampleFreq = FS_32K;
                         break;
                     case 44:
                         rLPCMInfo.eSampleFreq = FS_44K;
                         break;
                     case 8:
                         rLPCMInfo.eSampleFreq = FS_8K;
                         break;
                     case 11:
                         rLPCMInfo.eSampleFreq = FS_11K;
                         break;
                     case 12:
                         rLPCMInfo.eSampleFreq = FS_12K;
                         break;
                    case 16:
                         rLPCMInfo.eSampleFreq = FS_16K;
                         break;
                     case 22:
                         rLPCMInfo.eSampleFreq = FS_22K;
                         break;
                     case 24:
                         rLPCMInfo.eSampleFreq = FS_24K;
                         break;
                     default:
                         rLPCMInfo.eSampleFreq = FS_48K;
                         break;
                }

                rLPCMInfo.ui2_num_ch = rAdpcmParam.nChannels;

                switch (_prAudOmxHandle[0].eFormat)
                {
                    case AUD_FMT_LPCM:
                        Printf("[AUD][OMX] AUD_FMT_LPCM\n");
                        rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                        if (rAdpcmParam.nChannels==1)
                        {
                            rLPCMInfo.ui2_block_align = 2;
                        }
                        else
                        {
                            rLPCMInfo.ui2_block_align = 4;
                        }
                        rLPCMInfo.ui2_bits_per_sample = 16;
                        rLPCMInfo.b_big_endian = 1;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.b_signed_data = FALSE;
                        break;
                    case AUD_FMT_OMX_MSADPCM:
                        Printf("[AUD][OMX] AUD_FMT_OMX_MSADPCM\n");
                        rLPCMInfo.eLPcmType = LPCM_TYPE_MS_ADPCM;
                        rLPCMInfo.ui2_block_align = 2048; //TODO
                        switch (rAdpcmParam.nSampleRate)
                        {
                            case 24000:
                            case 22050:
                            case 16000:
                                rLPCMInfo.ui2_block_align /= 2;
                                break;
                            case 12000:
                            case 11025:
                            case 8000:
                                rLPCMInfo.ui2_block_align /= 4;
                                break;
                        }
                        if (rAdpcmParam.nChannels == 1)
                        {
                            rLPCMInfo.ui2_block_align /= 2;
                        }
                        rLPCMInfo.ui2_bits_per_sample = 4;
                        rLPCMInfo.b_big_endian = 1;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.b_signed_data = FALSE;
                        break;
                    case AUD_FMT_OMX_IMAADPCM:
                        Printf("[AUD][OMX] AUD_FMT_OMX_IMAADPCM\n");
                        rLPCMInfo.eLPcmType = LPCM_TYPE_IMA_ADPCM;
                        rLPCMInfo.ui2_block_align = 2048; //TODO
                        switch (rAdpcmParam.nSampleRate)
                        {
                            case 24000:
                            case 22050:
                            case 16000:
                                rLPCMInfo.ui2_block_align /= 2;
                                break;
                            case 12000:
                            case 11025:
                            case 8000:
                                rLPCMInfo.ui2_block_align /= 4;
                                break;
                        }
                        if (rAdpcmParam.nChannels == 1)
                        {
                            rLPCMInfo.ui2_block_align /= 2;
                        }
                        rLPCMInfo.ui2_bits_per_sample = 4;
                        rLPCMInfo.b_big_endian = 1;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.b_signed_data = FALSE;
                        break;
                    case AUD_FMT_OMX_ALAW:
                        Printf("[AUD][OMX] AUD_FMT_OMX_ALAW\n");
                        rLPCMInfo.eLPcmType = LPCM_TYPE_A_LAW;
                        if (rAdpcmParam.nChannels==1)
                        {
                            rLPCMInfo.ui2_block_align = 1;
                        }
                        else
                        {
                            rLPCMInfo.ui2_block_align = 2;
                        }
                        rLPCMInfo.ui2_bits_per_sample = 8;
                        rLPCMInfo.b_big_endian = 1;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.b_signed_data = FALSE;
                        break;
                    case AUD_FMT_OMX_ULAW:
                        Printf("[AUD][OMX] AUD_FMT_OMX_ULAW\n");
                        rLPCMInfo.eLPcmType = LPCM_TYPE_U_LAW;
                        if (rAdpcmParam.nChannels==1)
                        {
                            rLPCMInfo.ui2_block_align = 1;
                        }
                        else
                        {
                            rLPCMInfo.ui2_block_align = 2;
                        }
                        rLPCMInfo.ui2_bits_per_sample = 8;
                        rLPCMInfo.b_big_endian = 1;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.b_signed_data = FALSE;
                        break;
                    default:
                        break;
                }

                //MTK G-Streamer Proprietary Interface
                if ((((rAdpcmParam.nVersion.nVersion>>24)&0xff) == 'M') &&
                   (((rAdpcmParam.nVersion.nVersion>>16)&0xff) == 'T'))
                {
                    Printf("Get MTK GST ui2_block_align interface\n");
                    rLPCMInfo.ui2_block_align = (rAdpcmParam.nVersion.nVersion&0xffff);
                }
                //End

                rPCMInfo.eSampleFreq = rLPCMInfo.eSampleFreq;
                if (rLPCMInfo.b_big_endian)
                {
                    rPCMInfo.ePcmDataInfo = PCM_16_BITS_BIG_ENDIAN;
                }
                else
                {
                    rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                }

                Printf("eLPcmType: %d\n", rLPCMInfo.eLPcmType);
                Printf("ui2_num_ch: %d\n", rLPCMInfo.ui2_num_ch);
                Printf("ui2_block_align: %d\n", rLPCMInfo.ui2_block_align);
                Printf("ui2_bits_per_sample: %d\n", rLPCMInfo.ui2_bits_per_sample);
                Printf("b_big_endian: %d\n", rLPCMInfo.b_big_endian);
                Printf("b_info_not_specified: %d\n", rLPCMInfo.b_info_not_specific);
                Printf("b_signed_data: %d\n", rLPCMInfo.b_signed_data);

                AUD_PcmSetting(AUD_DEC_MAIN, &rPCMInfo);
                AUD_LPcmSetting(AUD_DEC_MAIN, &rLPCMInfo);
            }
        }
        else
        {
            // Get Parameter
            Printf("[AUD][OMX] get ADPCM parameter?, port = %d (strange)\n", rAdpcmParam.nPortIndex);
            rAdpcmParam.nChannels = 2;
            rAdpcmParam.nSampleRate = 48000;
            rAdpcmParam.nBitsPerSample = 16;
            i4Ret = copy_to_user(rKArg.pADPCMType, (void *)&rAdpcmParam, sizeof(OMX_AUDIO_PARAM_ADPCMTYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_ADPCMTYPE));

    return i4Ret;

}

INT32 i4AudOmxCompParamVORBIS(PARAMETER_OMX_AUDIO_PARAM_VORBISTYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_AUDIO_PARAM_VORBISTYPE rKArg;
    OMX_AUDIO_PARAM_VORBISTYPE rVorbisParam;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_VORBISTYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rVorbisParam, rKArg.pVORBISType, sizeof(OMX_AUDIO_PARAM_VORBISTYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            if (bOmxSettingBypass == 1)
            {
                Printf("[AUD][OMX] Bypass VORBIS Setting\n");
            }
            else
            {
                //TODO
                // Set Parameter
                Printf("[AUD][OMX] set VORBIS parameters port = %d (not yet implement)\n", rVorbisParam.nPortIndex);
                #if 0
                Printf("  nChannels: %d\n", rVorbisParam.nChannels);
                Printf("  nBitRate: %d\n", rVorbisParam.nBitRate);
                Printf("  nMinBitRate: %d\n", rVorbisParam.nMinBitRate);
                Printf("  nMaxBitRate: %x\n", rVorbisParam.nMaxBitRate);
                Printf("  nSampleRate: %x\n", rVorbisParam.nSampleRate);
                Printf("  nAudioBandWidth: %x\n", rVorbisParam.nAudioBandWidth);
                Printf("  nQuality: %x\n", rVorbisParam.nQuality);
                Printf("  bManaged: %d\n", rVorbisParam.bManaged);
                Printf("  bDownmix: %d\n", rVorbisParam.bDownmix);
                Printf("(Wait Joel Provide interface)\n");
                #endif
            }
        }
        else
        {
            // Get Parameter
            Printf("[AUD][OMX] get VORBIS parameter?, port = %d (not yet implement)\n", rVorbisParam.nPortIndex);
            i4Ret = copy_to_user(rKArg.pVORBISType, (void *)&rVorbisParam, sizeof(OMX_AUDIO_PARAM_VORBISTYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_VORBISTYPE));

    return i4Ret;
}

INT32 i4AudOmxCompParamAMR(PARAMETER_OMX_AUDIO_PARAM_AMRTYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_AUDIO_PARAM_AMRTYPE rKArg;
    OMX_AUDIO_PARAM_AMRTYPE rAmrParam;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_AMRTYPE));

    if (_prAudOmxHandle[0].fgUsed && (rKArg.pHandle == &_prAudOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rAmrParam, rKArg.pAMRType, sizeof(OMX_AUDIO_PARAM_AMRTYPE));

        if (OMX_PARAM_SET == rKArg.eDir)
        {
            if (bOmxSettingBypass == 1)
            {
                Printf("[AUD][OMX] Bypass AMR Setting\n");
            }
            else
            {
                // Set Parameter
                if (_prAudOmxHandle[0].eFormat == AUD_FMT_AMRNB)
                {
                    AUD_LPCM_INFO_T rLPCMInfo = {0};
                    AUD_AMR_CFG_T rAMRInfo = {0};

                    rAMRInfo.u2BitsPerSample = 16;
                    rAMRInfo.u2Numch = 1;
                    rAMRInfo.u4Sampersec = 8000;

                    //RISC decoder, set LPCM
                    rLPCMInfo.eSampleFreq = FS_8K;
                    rLPCMInfo.eLPcmType = 0;
                    rLPCMInfo.ui2_num_ch = 1;
                    rLPCMInfo.ui2_block_align = 0;
                    rLPCMInfo.ui2_bits_per_sample = 16;
                    rLPCMInfo.b_big_endian = 0;
                    rLPCMInfo.b_info_not_specific = 0;
                    rLPCMInfo.b_signed_data = 0 ;
                    AUD_LPcmSetting(AUD_DEC_MAIN,&rLPCMInfo);

                    //setting for RISC decoder
                    x_memcpy((void*)&_prAudDecInfo, (const void*)&rAMRInfo, sizeof(rAMRInfo));
                    _prAudOmxHandle[0].pDecInfo = _prAudDecInfo;
                }
                else if (_prAudOmxHandle[0].eFormat == AUD_FMT_AMRWB)
                {
                    AUD_LPCM_INFO_T rLPCMInfo = {0};
                    AUD_AMR_CFG_T rAWBInfo = {0};

                    rAWBInfo.u2BitsPerSample = 16;
                    rAWBInfo.u2Numch = 1;
                    rAWBInfo.u4Sampersec = 16000;

                    //RISC decoder, set LPCM
                    rLPCMInfo.eSampleFreq = FS_16K;
                    rLPCMInfo.eLPcmType = 0;
                    rLPCMInfo.ui2_num_ch = 1;
                    rLPCMInfo.ui2_block_align = 0;
                    rLPCMInfo.ui2_bits_per_sample = 16;
                    rLPCMInfo.b_big_endian = 0;
                    rLPCMInfo.b_info_not_specific = 0;
                    rLPCMInfo.b_signed_data = 0 ;
                    AUD_LPcmSetting(AUD_DEC_MAIN,&rLPCMInfo);

                    //setting for RISC decoder
                    x_memcpy((void*)&_prAudDecInfo, (const void*)&rAWBInfo, sizeof(rAWBInfo));
                    _prAudOmxHandle[0].pDecInfo = _prAudDecInfo;
                }
                else
                {
                    omxRet = OMX_ErrorUndefined;
                }
                Printf("[AUD][OMX] set AMR parameters port = %d (not yet implement)\n", rAmrParam.nPortIndex);
            }
        }
        else
        {
            // Get Parameter
            Printf("[AUD][OMX] get AMR parameter?, port = %d (not yet implement)\n", rAmrParam.nPortIndex);
            i4Ret = copy_to_user(rKArg.pAMRType, (void *)&rAmrParam, sizeof(OMX_AUDIO_PARAM_AMRTYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_AUDIO_PARAM_AMRTYPE));

    return i4Ret;
}

#endif

