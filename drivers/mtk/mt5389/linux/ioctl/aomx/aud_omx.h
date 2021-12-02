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

#ifndef _AUD_OMX_H_
#define _AUD_OMX_H_

#include "OMX_Core.h"
#include "OMX_Component.h"

#include "openmaxil_ioctl.h"

//#include "drv_aud.h" //DANIEL_HACK

//The current input buf limitation is 8K*8 = 64KB
#define MAX_INPUT_BUF_SIZE 8192
#define MAX_INPUT_BUF_NO 8
//The current output buf limitation is 32K*4 = 128KB
#define MAX_OUTPUT_BUF_SIZE 32768
#define MAX_OUTPUT_BUF_NO 4

#define INPUT_BUF_MAX  MAX_INPUT_BUF_NO
#define OUTPUT_BUF_MAX MAX_OUTPUT_BUF_NO

typedef struct _AUD_OMX_BUFFER_T AUD_OMX_BUFFER_T;

typedef struct {
    BOOL              fgUsed;
    BOOL              fgEmptying;
    BOOL              fgFilling;
    BOOL              fgStateTransing;
//    BOOL              fgInPortEnabled;
//    BOOL              fgOutPortEnabled;
//    BOOL              fgInPortTransing;
//    BOOL              fgOutPortTransing;
    BOOL              fgGotEOS;
    UINT32            pid;
    HANDLE_T          hMainTask;
    HANDLE_T          hCommandQ;
    AUD_OMX_BUFFER_T *pEmptyQ[INPUT_BUF_MAX]; // always left aligned, -> 11OO0000
    AUD_OMX_BUFFER_T *pFillQ[OUTPUT_BUF_MAX];
    AUD_FMT_T         eFormat; //DANIEL_HACK AUD_DRV_FMT_T
    OMX_STATETYPE     eState;
    OMX_STATETYPE     eStateTo;
    OMX_PTR           pAppData;
    MTVDEC_PFN_OMX_IL_CB  pEventHandler;          //DANIEL_HACK
    MTVDEC_PFN_OMX_IL_CB  pEmptyBufferDone;       //DANIEL_HACK
    MTVDEC_PFN_OMX_IL_CB  pFillBufferDone;        //DANIEL_HACK
    void             *pFifoPntr;  // AFIFO SA for decoder
    UINT32            u4FifoSize; // AFIFO length for decoder
    void             *pFifoWptr;  // AFIFO Wptr for decoder
    void            * pDecInfo;      //RISC decoder required info
} AUD_OMX_HANDLE_T;

typedef enum {
    AUD_OMX_CMD_SET_STATE,
    AUD_OMX_CMD_EMPTY_BUFFER,
    AUD_OMX_CMD_FILL_BUFFER,
    AUD_OMX_CMD_FLUSH,
    AUD_OMX_CMD_PORT_ENABLE,
    AUD_OMX_CMD_PORT_DISABLE,
    AUD_OMX_CMD_BUFFER_ALLOCATED,
    AUD_OMX_CMD_BUFFER_FREED,
    AUD_OMX_CMD_TERMINATE,
    AUD_OMX_CMD_MAX = 0x7FFFFFFF,
} AUD_OMX_CMD_T;

typedef struct {
    AUD_OMX_CMD_T     eCmd;
    OMX_STATETYPE     eStateTo; // for AUD_OMX_CMD_SET_STATE
    HANDLE_T          hHeader;  // for AUD_OMX_CMD_EMPTY_BUFFER & AUD_OMX_CMD_FILL_BUFFER
    BOOL              fgInput;  // for AUD_OMX_CMD_FLUSH
    BOOL              fgOutput; // for AUD_OMX_CMD_FLUSH
} AUD_OMX_MSG_T;

struct _AUD_OMX_BUFFER_T {
    AUD_OMX_HANDLE_T    *pOwner;
    HANDLE_T             hBuffer; // handle for below buffer
    void                *pBufSA;
    UINT32               u4BufSize;
    UINT32               u4FifoWp; // corresponding Wptr of AFIFO
    UINT32               u4FifoRp; // corresponding Rptr of AFIFO when finish copying this buffer to AFIFO
    UINT32               u4FifoFreezeCnt; // FIFO freeze (Rptr not changed) count
    HANDLE_T             hHeader; // handle for below header structure
    UINT32               u4FilledLen; // used on empty, for counting down filled length, not duplicate with header
    UINT32               u4Offset;    // used on empty, for counting down filled length, not duplicate with header
    BOOL                 fgEmptied;   // TRUE: this buffer has been emptied (if orginial size = 0, means has been processed)
                                      // FALSE: has not been processed (just queued)
    OMX_BUFFERHEADERTYPE rHeader; // client will read/write this area
    OMX_TICKS            nTimeStamp;    
    UINT32               u4Reserved; // for error detection
};

INT32 i4AudOmxTaskCreate(AUD_OMX_HANDLE_T *pHandle);
INT32 i4AudOmxTaskDestroy(AUD_OMX_HANDLE_T *pHandle);

INT32 i4AudOmxTaskSendCommand(AUD_OMX_HANDLE_T *pHandle, AUD_OMX_MSG_T *prMsg);

INT32 i4AudOmxTaskSemInit(void);
INT32 i4AudOmxTaskSemUninit(void);
INT32 i4AudOmxTaskLockSem(void);
INT32 i4AudOmxTaskUnlockSem(void);

INT32 i4AudOmxCompInit(void);
INT32 i4AudOmxCompUninit(void);

#if 1 //DANIEL_HACK
INT32 i4AudOmxCompGetHandle(PARAMETER_OMX_GETHANDLE *prUArg, PARAMETER_OMX_CB *prOmxCb);
#else
INT32 i4AudOmxCompGetHandle(PARAMETER_OMX_GETHANDLE *prUArg);
#endif
INT32 i4AudOmxCompFreeHandle(PARAMETER_OMX_FREEHANDLE *prUArg);

INT32 i4AudOmxCompGetState(PARAMETER_OMX_GETSTATE *prUArg);
INT32 i4AudOmxCompSetStateAsync(PARAMETER_OMX_COMMAND_STATESET *prUArg);
INT32 i4AudOmxCompFlushAsync(PARAMETER_OMX_COMMAND_PORT *prUArg);

INT32 i4AudOmxCompCheckStateTransition(AUD_OMX_HANDLE_T *pHandle);
INT32 i4AudOmxCompSetState(AUD_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxStateTo);
INT32 i4AudOmxCompFlush(AUD_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput);

INT32 i4AudOmxCompGetParamAudioInit(PARAMETER_OMX_PORT_PARAM_TYPE *prUArg);
INT32 i4AudOmxCompParamPortDef(PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE *prUArg);

INT32 i4AudOmxCompParamMP3(PARAMETER_OMX_AUDIO_PARAM_MP3TYPE *prUArg);
INT32 i4AudOmxCompParamAAC(PARAMETER_OMX_AUDIO_PARAM_AACPROFILETYPE *prUArg);
INT32 i4AudOmxCompParamPCM(PARAMETER_OMX_AUDIO_PARAM_PCMMODETYPE * prUArg);
INT32 i4AudOmxCompParamWMA(PARAMETER_OMX_AUDIO_PARAM_WMATYPE *prUArg);
INT32 i4AudOmxCompParamWMAPRO(PARAMETER_OMX_AUDIO_PARAM_WMAPROTYPE *prUArg);
INT32 i4AudOmxCompParamRA(PARAMETER_OMX_AUDIO_PARAM_RATYPE *prUArg);
INT32 i4AudOmxCompParamADPCM(PARAMETER_OMX_AUDIO_PARAM_ADPCMTYPE *prUArg);
INT32 i4AudOmxCompParamVORBIS(PARAMETER_OMX_AUDIO_PARAM_VORBISTYPE *prUArg);
INT32 i4AudOmxCompParamAMR(PARAMETER_OMX_AUDIO_PARAM_AMRTYPE *prUArg);

INT32 i4AudOmxCompAllocateBuffer(PARAMETER_OMX_ALLOCATEBUFFER *parg);
INT32 i4AudOmxCompFreeBuffer(PARAMETER_OMX_FREEBUFFER *prUArg);

INT32 i4AudOmxCompEmptyBuffer(PARAMETER_OMX_EMPTYTHISBUFFER *prUArg);
INT32 i4AudOmxCompFillBuffer(PARAMETER_OMX_FILLTHISBUFFER *prUArg);

INT32 i4AudOmxCompNotifyEmptyDone(AUD_OMX_HANDLE_T *pHandle, AUD_OMX_BUFFER_T *pBufEmptied);
INT32 i4AudOmxCompNotifyFillDone(AUD_OMX_HANDLE_T *pHandle, AUD_OMX_BUFFER_T *pBufFilled);
INT32 i4AudOmxCompNotifyEvent(AUD_OMX_HANDLE_T *pHandle, PARAMETER_OMX_CB_EVENTHANDLER *pEventInfo);

INT32 i4AudOmxCompPortDisable(AUD_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput);
INT32 i4AudOmxCompPortDisableAsync(PARAMETER_OMX_COMMAND_PORT *prUArg);
INT32 i4AudOmxCompPortEnable(AUD_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput);
INT32 i4AudOmxCompPortEnableAsync(PARAMETER_OMX_COMMAND_PORT *prUArg);

INT32 i4AudOmxAllocateAudioFifo(AUD_OMX_HANDLE_T *pHandle);

INT32 i4AudOmxPrepareDecoding(AUD_FMT_T eFormat, void* pvArg); //DANIEL_HACK
INT32 i4AudOmxStartDecoding(AUD_OMX_HANDLE_T *pHandle, void *pFifoSA, UINT32 u4FifoSz);
INT32 i4AudOmxStopDecoding(AUD_FMT_T eFormat);

INT32 i4AudOmxGetParameterMP3(OMX_AUDIO_PARAM_MP3TYPE *prMp3Info);

void vAudOmxBufferInit(void);
void vAudOmxBufferUninit(void);

INT32 i4AudOmxAllocOutputBuf(AUD_OMX_HANDLE_T *pHandle, UINT32 u4Size, UINT32 u4Port, AUD_OMX_BUFFER_T **ppBufInfo);
INT32 i4AudOmxFreeOutputBuf(AUD_OMX_HANDLE_T *pHandle, HANDLE_T hHeader);
INT32 i4AudOmxAllocInputBuf(AUD_OMX_HANDLE_T *pHandle, UINT32 u4Size, UINT32 u4Port, AUD_OMX_BUFFER_T **ppBufInfo);
INT32 i4AudOmxFreeInputBuf(AUD_OMX_HANDLE_T *pHandle, HANDLE_T hHeader);

INT32 i4AudOmxQueueFillBuf(AUD_OMX_HANDLE_T *pHandle, HANDLE_T hHeader);
INT32 i4AudOmxQueueEmptyBuf(AUD_OMX_HANDLE_T *pHandle, HANDLE_T hHeader);

INT32 i4AudOmxDoEmptyBuf(AUD_OMX_HANDLE_T *pHandle);
INT32 i4AudOmxDoFillBuf(AUD_OMX_HANDLE_T *pHandle);

INT32 i4AudOmxCheckBufEmptyDone(AUD_OMX_HANDLE_T *pHandle);

INT32 i4AudOmxForceAllBufEmptyDone(AUD_OMX_HANDLE_T *pHandle);
INT32 i4AudOmxForceAllBufFillDone(AUD_OMX_HANDLE_T *pHandle);

BOOL fgAudOmxAllInputBufferFreed(AUD_OMX_HANDLE_T *pHandle);
BOOL fgAudOmxAllOutputBufferFreed(AUD_OMX_HANDLE_T *pHandle);
void vAudOmxRestartDecoding(AUD_OMX_HANDLE_T *pHandle);
UINT32 u4AudOmxUpBufFullness(void);

//extern INT32 i4AsvSendFlushCmd(AUD_DEC_ID_T eDecId); //DANIEL_HACK 
extern UINT32 u4AOmxDbgMsk;
//2011/7/28 for flush cmd
extern BOOL fgRecvFlushCmd;
//End
extern UINT8 u1AudDspState; // 0: stop, 1: play, 2: pause

#define DISABLE_WMALSL_AMR //for Turnkey argument path too long problem

#endif // _AUD_OMX_H_


