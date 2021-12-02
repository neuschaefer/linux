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

#include "x_hal_5381.h"
#include "x_util.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_debug.h"
#include "x_common.h"

#include "x_aud_dec.h"
//#include "DspInc.h"   //DANIEL_HACK
//#include "drv_aud.h"  //DANIEL_HACK
#include "aud_if.h"     //DANIEL_HACK
#include "aud_omx.h"

//DANIEL_HACK
#ifndef SYS_Printf
//#define SYS_Printf Printf
#define SYS_Printf(fmt...)
#endif

extern UINT32 u4GetAFIFOStart(UINT8 uDecIndex);
extern UINT32 u4GetAFIFOEnd(UINT8 uDecIndex);
extern UINT32 u4GetABufPnt(UINT8 uDecIndex);
extern void vSetAWritePnt(UINT32 u4WritePointer);
extern UINT32 u4GetUploadFIFOStart(void);
extern UINT32 u4GetUploadFIFOEnd(void);
extern UINT32 u4GetUploadWritePnt(void);
extern void DSP_FlushInvalidateDCacheFree(UINT32 u4Addr, UINT32 u4Len);
extern void vDspGetParameterMP3(UINT32* u4FreqIdx, UINT32* u4DspInputChCfg, UINT32* u4Bitrate);
extern INT32 i4AudAWBTaskCreate(AUD_DEC_HANDLE_T *pHandle);
extern INT32 i4AudWMALSLTaskSendCommand(AUD_DEC_HANDLE_T *pHandle, AUD_DEC_MSG_T *prMsg);
extern INT32 i4AudAMRTaskSendCommand(AUD_DEC_HANDLE_T *pHandle, AUD_DEC_MSG_T *prMsg);
extern INT32 i4AudAWBTaskSendCommand(AUD_DEC_HANDLE_T *pHandle, AUD_DEC_MSG_T *prMsg);

extern AUD_OMX_HANDLE_T _prAudOmxHandle[1];

UINT32 u4AOmxDbgMsk = 0;
UINT8 u1FillQueueNo = 0;
UINT8 u1EmptyQueueNo = 0;
UINT32 u4TotalFillQueueNo = 0;
UINT32 u4TotalEmptyQueueNo = 0;
UINT32 u4TotalFillQueueDoneNo = 0;
UINT32 u4TotalEmptyQueueDoneNo = 0;
UINT8 u1AudDspState = 0; // 0: Stop 1: Play 2: Pause

UINT32 u4BufferSA;
UINT32 u4BufferEA;
UINT32 u4BufferAP;

UINT32 u4OutputBufferSA;
UINT32 u4OutputBufferEA;
UINT32 u4OutputBufferRP;
#define u4OutputBufferWP VIRTUAL(u4GetUploadWritePnt())

UINT32 u4AfifoSA;
UINT32 u4AfifoEA;

//2011/7/28 for flush cmd
BOOL fgRecvFlushCmd = FALSE;
//End

#ifndef DISABLE_WMALSL_AMR
AUD_DEC_HANDLE_T  hriscdec;
extern INT32 i4AudWMALSLTaskCreate(AUD_DEC_HANDLE_T *pHandle);
extern INT32 i4AudAMRTaskCreate(AUD_DEC_HANDLE_T *pHandle);
static UINT32 u4AudOmxFillBSFIFO(void *pvHandle, UINT32 u4From, UINT32 u4Size);
#endif

//======================================================

//PTS

//#define RESERVE_FIFO_4K

//PTS_FREERUN
OMX_TICKS nTimeStampCur = -1;
#define PTS_INTERVAL (170667L) //This value is 32*1024/4/48000*1000000. If you change the upload buffer size, you also need to change this value.
//END

typedef struct
{
    UINT32 u4FifoWp;
    OMX_TICKS nTimeStamp;
    BOOL fgIsSet;
} AUD_OMX_PTS_INFO_T;

#define MAX_PTS_INFO_SZ 288 //for AFIFO1 288KB => 1KB/1 timestamp
#define PTS_INTERVAL_SZ 0x400
AUD_OMX_PTS_INFO_T _prAudOmxPtsInfo[MAX_PTS_INFO_SZ];

UINT32 u4LastChkPtsPtr = 0;
UINT32 u4NextChkPtsPtr = 0;
UINT32 u4LastOutputWP = 0;
UINT32 u4LastPtsInfoIdx = 0;
INT32 i4AOmxPtsOffset = 0;

static void vAudOmxSetPTS(AUD_OMX_HANDLE_T *pHandle, OMX_TICKS nTimeStamp)
{
    UINT32 u4FifoWp = 0;
    UINT32 u4PtsInfoIdx = 0;

    u4FifoWp = (UINT32) pHandle->pFifoWptr;
    u4PtsInfoIdx = ((UINT32)(pHandle->pFifoWptr) - (UINT32)(pHandle->pFifoPntr)) / PTS_INTERVAL_SZ;

    if (!_prAudOmxPtsInfo[u4PtsInfoIdx].fgIsSet)
    {
        _prAudOmxPtsInfo[u4PtsInfoIdx].u4FifoWp = u4FifoWp;
        _prAudOmxPtsInfo[u4PtsInfoIdx].nTimeStamp = nTimeStamp;
        _prAudOmxPtsInfo[u4PtsInfoIdx].fgIsSet = TRUE;
        #if 0
        //DBG Log        
        if (u4AOmxDbgMsk & 0x10)
        {
            Printf("INPTS(%04d): %08x - %08x%08x\n",
                    u4PtsInfoIdx,
                    (UINT32)(pHandle->pFifoWptr),
                    (UINT32)(((OMX_U64)(nTimeStamp)>>32)&0xffffffff),
                    (UINT32)((OMX_U64)(nTimeStamp)&0xffffffff));
        }
        //
        #endif

        while (u4LastPtsInfoIdx != u4PtsInfoIdx)
        {
            _prAudOmxPtsInfo[u4LastPtsInfoIdx].fgIsSet = TRUE;
            _prAudOmxPtsInfo[u4LastPtsInfoIdx].u4FifoWp = 0;    
            u4LastPtsInfoIdx++;
            if (u4LastPtsInfoIdx == MAX_PTS_INFO_SZ)
            {
                u4LastPtsInfoIdx = 0;
            }
        }
        u4LastPtsInfoIdx = u4PtsInfoIdx+1;
        if (u4LastPtsInfoIdx == MAX_PTS_INFO_SZ)
        {
            u4LastPtsInfoIdx = 0;
        }
    }
}
//END

UINT32 u4AudOmxUpBufFullness(void)
{
    UINT32 u4FifoFullness;
    UINT32 u4OutBufWp = u4OutputBufferWP;

    u4FifoFullness = 
        ((u4OutBufWp >= u4OutputBufferRP) ? (u4OutBufWp - u4OutputBufferRP) : (u4OutputBufferEA - u4OutputBufferSA - u4OutputBufferRP + u4OutBufWp));

    return (u4FifoFullness * 100 / (u4OutputBufferEA - u4OutputBufferSA));
}

void vAudOmxQuery(void)
{
    Printf("== OMX Status ==\n");
    Printf("  u1AudDspState: %d (0: stop 1: play 2: pause)\n", u1AudDspState);
    Printf("  eState: %d (1: load 2: idle 3: exec 4: pause)\n", _prAudOmxHandle[0].eState);
    Printf("  FMT: %d\n", _prAudOmxHandle[0].eFormat);
    Printf("  pHandle->fgEmptying: %d\n",_prAudOmxHandle[0].fgEmptying);
    Printf("  pHandle->fgFilling: %d\n", _prAudOmxHandle[0].fgFilling);
    Printf("  fgGotEOS: %d\n", _prAudOmxHandle[0].fgGotEOS);    
    Printf("  EmptyQueueNo: %d/%d/%d\n", u1EmptyQueueNo, u4TotalEmptyQueueDoneNo, u4TotalEmptyQueueNo);
    Printf("  FillQueueNo: %d/%d/%d\n", u1FillQueueNo, u4TotalFillQueueDoneNo, u4TotalFillQueueNo);
    Printf("  fgRecvFlushCmd: %d\n", fgRecvFlushCmd);

#ifndef DISABLE_WMALSL_AMR
    if ((_prAudOmxHandle[0].eFormat == AUD_FMT_AMRNB) ||
        (_prAudOmxHandle[0].eFormat == AUD_FMT_AMRWB))
    {
        Printf("== AMR Task Status ==\n");
        Printf("  u4DecState: %d (0: init 1: play 2: stop 3: idle)\n", hriscdec.u4decstate);
        Printf("  u4frmcnt: %d\n", hriscdec.u4frmcnt);

        Printf("== AFIFO3 Status ==\n");
        Printf("  SA: 0x%08x\n", hriscdec.pInBufSTRBLK);
        Printf("  EA: 0x%08x\n", hriscdec.pInBufENDBLK);
        Printf("  WP: 0x%08x\n", hriscdec.pInBufPRSPNT);
        Printf("  RP: 0x%08x\n", hriscdec.pInBufBUFPNT);
    }
#endif    

    Printf("== AFIFO1 Status ==\n");
    Printf("  SA: 0x%08x\n", _prAudOmxHandle[0].pFifoPntr);
    Printf("  EA: 0x%08x\n", _prAudOmxHandle[0].pFifoPntr + _prAudOmxHandle[0].u4FifoSize);
    Printf("  WP: 0x%08x\n", _prAudOmxHandle[0].pFifoWptr);
    Printf("  RP: 0x%08x\n", VIRTUAL(u4GetABufPnt(0)));

    Printf("== Upload Buffer Status ==\n");
    Printf("  SA: 0x%08x\n", u4OutputBufferSA);
    Printf("  EA: 0x%08x\n", u4OutputBufferEA);
    Printf("  RP: 0x%08x\n", u4OutputBufferRP);
    Printf("  WP: 0x%08x\n", u4OutputBufferWP);
}

static void vAudBufferInit(void)
{
    u4BufferSA = VIRTUAL(u4GetAFIFOStart(1));
    u4BufferEA = VIRTUAL(u4GetAFIFOEnd(1));
    u4BufferAP = u4BufferSA;

    u4OutputBufferSA = VIRTUAL(u4GetUploadFIFOStart());
    u4OutputBufferEA = VIRTUAL(u4GetUploadFIFOEnd());
    u4OutputBufferRP = u4OutputBufferSA;

    u4AfifoSA = VIRTUAL(u4GetAFIFOStart(0));
    u4AfifoEA = VIRTUAL(u4GetAFIFOEnd(0));
}

static UINT32 u4AudBufferAlloc(UINT32 u4Size)
{
    UINT32 u4AP;
    #if 0
    if (u4BufferAP + u4Size > u4BufferEA)
    {
        ASSERT(0);
    }
    #endif

    u4AP = u4BufferAP;
    //SYS_Printf("[u4AudBufferAlloc] SA: 0x%08x SIZE: 0x%08x\n", u4AP, u4Size);
    u4BufferAP += u4Size;

    return u4AP;
}

#define i4AsvSendFlushCmd(a)  //XiangMing said no support is ok?
#define BSP_InvDCacheRange(a,b) DSP_FlushInvalidateDCacheFree(a,b)
#define BSP_CleanDCacheRange(a,b) DSP_FlushInvalidateDCacheFree(a,b)
#define vAudHalSetBufWPtr(a,b,c) vSetAWritePnt(PHYSICAL(c))
#define u4AudHalGetBufRPtr(a,b) VIRTUAL(u4GetABufPnt(0))
#define x_free_aligned_afifo_mem(a)
#define x_alloc_aligned_afifo_mem(a,b) u4AudBufferAlloc(a)
//DANIEL_HACK

//CC_AUD_SUPPORT_OMX
UINT8 u1BypassChVol = 0;

UINT8 u1AudOmxGetBypassChVol(void)
{
    return u1BypassChVol;
}

void vAudOmxSetBypassChVol(UINT8 u1Enable)
{
    u1BypassChVol = u1Enable;
}
//CC_AUD_SUPPORT_OMX

// TODO: temp solution
//extern UINT32 _u4Dec4OutputOptVal; //DANIEL_HACK

static AUD_OMX_BUFFER_T _prAudOmxInputBuf[INPUT_BUF_MAX];
static AUD_OMX_BUFFER_T _prAudOmxOutputBuf[OUTPUT_BUF_MAX];

void vAudOmxBufferInit(void)
{
    UINT32 u4Index;
    //HANDLE_T hHeader = NULL_HANDLE;

    for (u4Index = 0; u4Index < INPUT_BUF_MAX; u4Index ++)
    {
        // can not do this on open ???
        //hHeader = x_kmem_create_handle(&(_prAudOmxInputBuf[u4Index].rHeader), sizeof(OMX_BUFFERHEADERTYPE));
        //ASSERT(NULL_HANDLE == hHeader);

        _prAudOmxInputBuf[u4Index].pOwner     = NULL;
        _prAudOmxInputBuf[u4Index].hBuffer    = NULL_HANDLE;
        _prAudOmxInputBuf[u4Index].pBufSA     = NULL;
        _prAudOmxInputBuf[u4Index].u4BufSize  = 0;
        _prAudOmxInputBuf[u4Index].hHeader    = NULL_HANDLE;
        _prAudOmxInputBuf[u4Index].u4Reserved = 0x5A5A5A5A;
    }

    for (u4Index = 0; u4Index < OUTPUT_BUF_MAX; u4Index ++)
    {
        //hHeader = x_kmem_create_handle(&(_prAudOmxOutputBuf[u4Index].rHeader), sizeof(OMX_BUFFERHEADERTYPE));
        //ASSERT(NULL_HANDLE == hHeader);

        _prAudOmxOutputBuf[u4Index].pOwner     = NULL;
        _prAudOmxOutputBuf[u4Index].hBuffer    = NULL_HANDLE;
        _prAudOmxOutputBuf[u4Index].pBufSA     = NULL;
        _prAudOmxOutputBuf[u4Index].u4BufSize  = 0;
        _prAudOmxOutputBuf[u4Index].hHeader    = NULL_HANDLE;
        _prAudOmxOutputBuf[u4Index].u4Reserved = 0xA5A5A5A5;
    }

    vAudBufferInit(); //DANIEL_HACK
}


void vAudOmxBufferUninit(void)
{
    UINT32 u4Index;

    for (u4Index = 0; u4Index < INPUT_BUF_MAX; u4Index ++)
    {
        if (NULL_HANDLE != _prAudOmxInputBuf[u4Index].hHeader)
        {
            SYS_Printf("%s %d: x_kmem_destroy_handle\n", __FUNCTION__, __LINE__);
            x_kmem_destroy_handle(_prAudOmxInputBuf[u4Index].hHeader);
        }
    }

    for (u4Index = 0; u4Index < OUTPUT_BUF_MAX; u4Index ++)
    {
        if (NULL_HANDLE != _prAudOmxOutputBuf[u4Index].hHeader)
        {
            SYS_Printf("%s %d: x_kmem_destroy_handle\n", __FUNCTION__, __LINE__);
            x_kmem_destroy_handle(_prAudOmxOutputBuf[u4Index].hHeader);
        }
    }
}


// allocate output buffer from AFIFO (temp solution?)
// OpenMAX client may allocate output buffer more than one time
INT32 i4AudOmxAllocOutputBuf(AUD_OMX_HANDLE_T *pHandle, UINT32 u4Size, UINT32 u4Port, AUD_OMX_BUFFER_T **ppBufInfo)
{
    UINT32 u4Index;

    void *pBufSA  = NULL;

    HANDLE_T hHeader = NULL_HANDLE;
    HANDLE_T hMemHandle = NULL_HANDLE;
    AUD_OMX_BUFFER_T *pAvailBuf = NULL;

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    // STEP 0: get a free output buffer info node
    for (u4Index = 0; u4Index < OUTPUT_BUF_MAX; u4Index ++)
    {
        if (NULL == _prAudOmxOutputBuf[u4Index].pOwner)
        {
            pAvailBuf = &(_prAudOmxOutputBuf[u4Index]);
            break;
        }
    }

    if (NULL == pAvailBuf)
    {
        // no more free nodes
        goto lblErrorReturn;
    }
    else
    {
        // initialize
        pAvailBuf->pOwner    = pHandle;
        pAvailBuf->hBuffer   = NULL_HANDLE;
        pAvailBuf->pBufSA    = NULL;
        pAvailBuf->u4BufSize = 0;
    }

    SYS_Printf("%s %d: x_kmem_create_handle\n", __FUNCTION__, __LINE__);
    hHeader = x_kmem_create_handle(&(pAvailBuf->rHeader), sizeof(OMX_BUFFERHEADERTYPE));

    if (NULL_HANDLE == hHeader)
    {
        goto lblErrorReturn;
    }
    else
    {
        pAvailBuf->hHeader = hHeader;
    }

    // STEP 1: allocate input buffer from AFIFO
    pBufSA = (void*)x_alloc_aligned_afifo_mem(u4Size, 256);

    if (NULL == pBufSA)
    {
        SYS_Printf("[AUD][OMX] alloc AFIFO memory failed, %d bytes.\n", u4Size);
        goto lblErrorReturn;
    }
    else
    {
        pAvailBuf->pBufSA    = pBufSA;
        pAvailBuf->u4BufSize = u4Size;
    }

    SYS_Printf("%s %d: x_kmem_create_handle\n", __FUNCTION__, __LINE__);
    hMemHandle = x_kmem_create_handle(pBufSA, u4Size);

    if (NULL_HANDLE == hMemHandle)
    {
        goto lblErrorReturn;
    }
    else
    {
        pAvailBuf->hBuffer = hMemHandle;
    }

    // fill buffer header
    x_memset(&(pAvailBuf->rHeader), 0x00, sizeof(OMX_BUFFERHEADERTYPE));

    pAvailBuf->rHeader.nSize             = sizeof(OMX_BUFFERHEADERTYPE);
    pAvailBuf->rHeader.nVersion.nVersion = OMX_VERSION;

    pAvailBuf->rHeader.pBuffer          = pBufSA;
    pAvailBuf->rHeader.nAllocLen        = u4Size;
    pAvailBuf->rHeader.nFilledLen       = 0;
    pAvailBuf->rHeader.nOffset          = 0;
    pAvailBuf->rHeader.nOutputPortIndex = u4Port;

    // release semaphore
    i4AudOmxTaskUnlockSem();

    *ppBufInfo = pAvailBuf;
    return AUD_OK;

    // error handling
lblErrorReturn:
    // release semaphore
    i4AudOmxTaskUnlockSem();

    // resource release will be done on freeing component handle, not here
    *ppBufInfo = NULL;
    return AUD_FAIL;
}


INT32 i4AudOmxFreeOutputBuf(AUD_OMX_HANDLE_T *pHandle, HANDLE_T hHeader)
{
    UINT32 u4Index;
    AUD_OMX_BUFFER_T *pBufNode = NULL;

    if ((NULL == pHandle) || (NULL_HANDLE == hHeader))
    {
        return AUD_OK;
    }

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    for (u4Index = 0; u4Index < OUTPUT_BUF_MAX; u4Index ++)
    {
        if (hHeader == _prAudOmxOutputBuf[u4Index].hHeader)
        {
            pBufNode = &(_prAudOmxOutputBuf[u4Index]);
            break;
        }
    }

    if (NULL != pBufNode)
    {
        if ((pBufNode->pOwner == pHandle) && (NULL != pBufNode->pBufSA))
        {
            // free memory
            x_free_aligned_afifo_mem(pBufNode->pBufSA);

            // destory the buffer handle
            SYS_Printf("%s %d: x_kmem_destroy_handle\n", __FUNCTION__, __LINE__);
            x_kmem_destroy_handle(pBufNode->hBuffer);

            // destory the header handle
            SYS_Printf("%s %d: x_kmem_destroy_handle\n", __FUNCTION__, __LINE__);
            x_kmem_destroy_handle(pBufNode->hHeader);

            // clear info
            pBufNode->pOwner    = NULL;
            pBufNode->hBuffer   = NULL_HANDLE;
            pBufNode->pBufSA    = NULL;
            pBufNode->u4BufSize = 0;
            pBufNode->hHeader   = NULL_HANDLE;
        }
    }

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return AUD_OK;
}

INT32 i4AudOmxQueueFillBuf(AUD_OMX_HANDLE_T *pHandle, HANDLE_T hHeader)
{
    INT32 i4Ret = AUD_FAIL;
    UINT32 u4Index;
    AUD_OMX_BUFFER_T *pBufNode = NULL;

    if ((NULL == pHandle) || (NULL_HANDLE == hHeader))
    {
        return AUD_OK;
    }

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    for (u4Index = 0; u4Index < OUTPUT_BUF_MAX; u4Index ++)
    {
        if (hHeader == _prAudOmxOutputBuf[u4Index].hHeader)
        {
            u1FillQueueNo++;
            u4TotalFillQueueNo++;
            if (u4AOmxDbgMsk & 0x40)
            {
                Printf("QueueFillBuf(%d)\n", u1FillQueueNo);
            }
            pBufNode = &(_prAudOmxOutputBuf[u4Index]);
            break;
        }
    }

    if (NULL != pBufNode)
    {
        if (pBufNode->pOwner == pHandle)
        {
            // add to queue
            for (u4Index = 0; u4Index < OUTPUT_BUF_MAX; u4Index ++)
            {
                if (NULL == pHandle->pFillQ[u4Index])
                {
                    i4Ret = AUD_OK;
                    pHandle->fgFilling = TRUE;
                    pHandle->pFillQ[u4Index] = pBufNode;
                    break;
                }
            }

            if (AUD_OK != i4Ret)
            {
                SYS_Printf("[AUD][OMX] WARNING: fill buffer queue is full.\n");
            }

            // initialize buffer filled len
            // (The OMX_FillThisBuffer macro will send an empty buffer to an output port of a component.)
            pBufNode->u4FilledLen = 0;
            pBufNode->u4Offset    = 0;
        }
    }

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return i4Ret;
}


static BOOL fgAudOmxOutputNotReady(void)
{
    UINT32 u4Size = (u4OutputBufferEA - u4OutputBufferSA); //wReadDspWORD(ADDR_D2RC_DECODING4_OUTPUT_PCMBUF_SIZE); //DANIEL_HACK
    UINT32 u4Rptr = u4OutputBufferRP; //wReadDspWORD(ADDR_RC2D_DECODING4_OUTPUT_PCMBUF_READ_PTR); //DANIEL_HACK
    UINT32 u4Wptr = u4OutputBufferWP; //wReadDspWORD(ADDR_D2RC_DECODING4_OUTPUT_PCMBUF_WRITE_PTR); //DANIEL_HACK

    // check the buffer size
    // it is filled to 0 before send play command, see i4AudOmxStartDecoding
    // check read pointer & write pointer
    // it is set to the same value
    if ((u4Size == 0x0000) && (u4Rptr == u4Wptr))
    {
        return TRUE;
    }

    return FALSE;
}


static BOOL fgAudOmxOutputNoData(void)
{
    UINT32 u4Rptr = u4OutputBufferRP; //wReadDspWORD(ADDR_RC2D_DECODING4_OUTPUT_PCMBUF_READ_PTR); //DANIEL_HACK
    UINT32 u4Wptr = u4OutputBufferWP; //wReadDspWORD(ADDR_D2RC_DECODING4_OUTPUT_PCMBUF_WRITE_PTR); //DANIEL_HACK

    if (u4Rptr == u4Wptr)
    {
        return TRUE;
    }

    return FALSE;
}


// get decoder output to output buffer
// u4From    - write output buffer from this address
// u4MaxSize - output buffer free space
static UINT32 u4AudOmxGetOutput(AUD_OMX_HANDLE_T *pHandle, UINT32 u4From, UINT32 u4MaxSize)
{
    INT32 i4Filled = 0, i4OutputAvail;
    UINT32 u4OutputSA, u4OutputEA, u4OutputRp, u4OutputWp;

    u4OutputSA = u4OutputBufferSA;
    u4OutputEA = u4OutputBufferEA;
    u4OutputRp = u4OutputBufferRP;
    u4OutputWp = u4OutputBufferWP;

    //PTS        
    if (u4AOmxDbgMsk & 0x20)
    {
        if ((pHandle->eFormat == AUD_FMT_WMA10LOSSLESS) ||
             (pHandle->eFormat == AUD_FMT_WMA10SPEECH) ||
             (pHandle->eFormat == AUD_FMT_AMRNB) ||
             (pHandle->eFormat == AUD_FMT_AMRWB) )
        {
            //TODO        
        }
        else
        {
            if (u4OutputWp < u4LastOutputWP)
            {
                u4NextChkPtsPtr = u4AudHalGetBufRPtr(FRTH_DEC, DSP_AFIFO);
                if (u4AOmxDbgMsk & 0x10)
                {
                    Printf("u4NextChkPtsPtr: %08x\n", u4NextChkPtsPtr);
                }
            }
            u4LastOutputWP = u4OutputWp;
        }
    }
    //END

    //SYS_Printf("[AUD][OMX] u4AudOmxGetOutput: u4OutputSA = 0x%X, u4OutputEA = 0x%X, u4OutputWp = 0x%X, u4OutputRp = 0x%X u4From = 0x%X\n",
    //           u4OutputSA, u4OutputEA, u4OutputWp, u4OutputRp, u4From);

    if (u4OutputWp == u4OutputRp)
    {
        // Wp == Rp, think the buffer is empty
        // DSP will not write buffer to full
    }
    else if (u4OutputWp > u4OutputRp)
    {
        // |--------|xxxxxxxxxxxxxx|--------------|
        // SA       Rp             Wp             EA
        //          |-----here-----|

        i4OutputAvail = u4OutputWp - u4OutputRp;

        if (i4OutputAvail < 0)
        {
            // get none
            i4Filled = 0;
        }
        else if (i4OutputAvail < u4MaxSize)
        {
            // get all decoded samples
            i4Filled = i4OutputAvail;
        }
        else
        {
            // get part
            i4Filled = u4MaxSize;
        }

        if (i4Filled > 0)
        {
            // invalidate cache
            BSP_InvDCacheRange(u4OutputRp, i4Filled);

            // copy PCM samples from decoder output buffer to OpenMAX output buffer
            x_memcpy((void*)u4From, (const void*)u4OutputRp, i4Filled);

            u4OutputRp += i4Filled;
        }
    }
    else if (u4OutputWp < u4OutputRp)
    {
        // |xxxxxxxx|--------------|xxxxxxxxxxxxxx|
        // SA       Wp             Rp             EA
        //                         |-----here-----|

        i4OutputAvail = u4OutputEA - u4OutputRp;

        if (i4OutputAvail < 0)
        {
            // get none
            i4Filled = 0;
        }
        else if (i4OutputAvail < u4MaxSize)
        {
            // get all decoded samples
            i4Filled = i4OutputAvail;
        }
        else
        {
            // get part
            i4Filled = u4MaxSize;
        }

        if (i4Filled > 0)
        {
            // invalidate cache
            BSP_InvDCacheRange(u4OutputRp, i4Filled);

            // copy PCM samples from decoder output buffer to OpenMAX output buffer
            x_memcpy((void*)u4From, (const void*)u4OutputRp, i4Filled);

            u4OutputRp += i4Filled;
        }

        // check if need roll back, make sure Rp != EA
        if (u4OutputRp == u4OutputEA)
        {
            u4OutputRp = u4OutputSA;
        }

        if ((i4Filled < u4MaxSize) && (u4OutputWp > u4OutputSA)) // avoid case u4OutputWp == u4OutputSA
        {
            INT32 i4Filled2;

            // |xxxxxxxx|--------------|xxxxxxxxxxxxxx|
            // SA       Wp             Rp             EA
            // |--here--|

            i4OutputAvail = u4OutputWp - u4OutputSA;

            if (i4OutputAvail < 0)
            {
                // get none
                i4Filled2 = 0;
            }
            else if (i4OutputAvail < (u4MaxSize - i4Filled))
            {
                // get all decoded samples
                i4Filled2 = i4OutputAvail;
            }
            else
            {
                // get part
                i4Filled2 = u4MaxSize - i4Filled;
            }

            if (i4Filled2 > 0)
            {
                // invalidate cache
                BSP_InvDCacheRange(u4OutputSA, i4Filled2);

                // copy PCM samples from decoder output buffer to OpenMAX output buffer
                x_memcpy((void*)(u4From + i4Filled), (const void*)u4OutputSA, i4Filled2);

                u4OutputRp += i4Filled2;
                i4Filled   += i4Filled2;
            }
        }
    }

    // check if need roll back, make sure Rp != EA
    if (u4OutputRp == u4OutputEA)
    {
        u4OutputRp = u4OutputSA;
    }

    // update new read pointer
    #if 1 //DANIEL_HACK
    u4OutputBufferRP = u4OutputRp;
    #else
    vWriteDspWORD(ADDR_RC2D_DECODING4_OUTPUT_PCMBUF_READ_PTR, u4OutputRp - u4OutputSA);
    #endif

    //SYS_Printf("i4Filled = %4d, SA=0x%x EA=0x%x WP=0x%x Rptr = 0x%X\n", i4Filled, u4OutputSA, u4OutputEA, u4OutputWp, u4OutputRp);

    return i4Filled;
}


INT32 i4AudOmxDoFillBuf(AUD_OMX_HANDLE_T *pHandle)
{
    INT32 i4Ret = AUD_FAIL;
    UINT32 u4FilledSize;
    UINT32 u4Index;
    BOOL fgAllFilled = TRUE;
    AUD_OMX_BUFFER_T *pToFillBuf = NULL;
    static int retry_cnt = 0; //DANIEL_HACK

    // always left aligned
    pToFillBuf = pHandle->pFillQ[0];

    if (NULL == pToFillBuf)
    {
        // error handling
        pHandle->fgFilling = FALSE;

        return AUD_OK;
    }

    // if decoder is not ready, can not fill buffer now
    if (fgAudOmxOutputNotReady())
    {
        SYS_Printf("[AUD][OMX] i4AudOmxDoFillBuf: decoder not ready.\n");

        return AUD_OK;
    }

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    u4FilledSize = u4AudOmxGetOutput(pHandle,
                                     (UINT32) pToFillBuf->pBufSA + pToFillBuf->u4Offset,
                                     pToFillBuf->u4BufSize - pToFillBuf->u4FilledLen);
    pToFillBuf->u4Offset += u4FilledSize;
    pToFillBuf->u4FilledLen += u4FilledSize;

    //DANIEL_HACK
    if (u4FilledSize == 0)
    {
        retry_cnt++;
        #if 0
        if (retry_cnt >= 10)
        {
            printf("retry %d\n", retry_cnt);
        }
        #endif
    }
    else
    {
        retry_cnt = 0;
    }
    //DANIEL_HACK

    if ((pToFillBuf->u4FilledLen == pToFillBuf->u4BufSize) || (pHandle->fgGotEOS && (0 == u4FilledSize) && (retry_cnt >= 10))) //DANIEL_HACK
    {
        // if fill is done, align queue to left and check if there is pending fill event
        for (u4Index = 0; u4Index < OUTPUT_BUF_MAX - 1; u4Index ++)
        {
            pHandle->pFillQ[u4Index] = pHandle->pFillQ[u4Index + 1];

            if (NULL != pHandle->pFillQ[u4Index])
            {
                fgAllFilled = FALSE;
            }
        }

        // clear the tail node
        pHandle->pFillQ[OUTPUT_BUF_MAX - 1] = NULL;

        if (fgAllFilled)
        {
            // fill queue is empty
            pHandle->fgFilling = FALSE;
        }

        //DANIEL_HACK
        if (pHandle->fgGotEOS && (0 == u4FilledSize) && (retry_cnt >= 10))
        {
            Printf("[AUD][OMX] NofiyFillDone EOS\n");
            pToFillBuf->rHeader.nFlags |= OMX_BUFFERFLAG_EOS;
        }
        else
        {
            pToFillBuf->rHeader.nFlags &= ~OMX_BUFFERFLAG_EOS;
        }
        //DANIEL_HACK

        //PTS
        //PTS_FREERUN
        {
            if (u4AOmxDbgMsk & 0x10)
            {
                Printf("OUTPTS: %08x%08x\n",
                        (UINT32)(((OMX_U64)(nTimeStampCur)>>32)&0xffffffff),
                        (UINT32)((OMX_U64)(nTimeStampCur)&0xffffffff));
            }
            pToFillBuf->rHeader.nTimeStamp = nTimeStampCur;                
            nTimeStampCur += PTS_INTERVAL;
        }
        //END
        if (u4AOmxDbgMsk & 0x20)
        {
            if ((pHandle->eFormat == AUD_FMT_WMA10LOSSLESS) ||
                 (pHandle->eFormat == AUD_FMT_WMA10SPEECH) ||
                 (pHandle->eFormat == AUD_FMT_AMRNB) ||
                 (pHandle->eFormat == AUD_FMT_AMRWB) )
            {
                //TODO
            }
            else
            {        
                UINT32 u4PtsInfoIdx = (u4LastChkPtsPtr - (UINT32)(pHandle->pFifoPntr))/PTS_INTERVAL_SZ;
                UINT32 u4NextPtsInfoIdx = (u4NextChkPtsPtr - (UINT32)(pHandle->pFifoPntr))/PTS_INTERVAL_SZ;
                UINT32 u4PtsInfoIdx_org = u4PtsInfoIdx;
                UINT32 u4NextPtsInfoIdx_org = u4NextPtsInfoIdx;
                UINT32 u4PtsInfoIdxAddOne;
                UINT32 u4PtsDiff;
                UINT32 u4WpDiff;
              
                //To Prevent from using old value
                while ((!_prAudOmxPtsInfo[u4PtsInfoIdx].fgIsSet) || (_prAudOmxPtsInfo[u4PtsInfoIdx].u4FifoWp==0))
                {
                    u4PtsInfoIdx++;
                    if (u4PtsInfoIdx == MAX_PTS_INFO_SZ)
                    {
                        u4PtsInfoIdx = 0;
                    }
                }
                
                u4PtsInfoIdxAddOne = u4PtsInfoIdx+1;                
                if (u4PtsInfoIdxAddOne == MAX_PTS_INFO_SZ)
                {
                    u4PtsInfoIdxAddOne = 0;
                }
                while ((!_prAudOmxPtsInfo[u4PtsInfoIdxAddOne].fgIsSet) || (_prAudOmxPtsInfo[u4PtsInfoIdxAddOne].u4FifoWp==0))
                {
                    u4PtsInfoIdxAddOne++;
                    if (u4PtsInfoIdxAddOne == MAX_PTS_INFO_SZ)
                    {
                        u4PtsInfoIdxAddOne = 0;
                    }
                }

                u4PtsDiff = (UINT32)(_prAudOmxPtsInfo[u4PtsInfoIdxAddOne].nTimeStamp - _prAudOmxPtsInfo[u4PtsInfoIdx].nTimeStamp);
                if (_prAudOmxPtsInfo[u4PtsInfoIdxAddOne].u4FifoWp > _prAudOmxPtsInfo[u4PtsInfoIdx].u4FifoWp)
                {
                    u4WpDiff = (_prAudOmxPtsInfo[u4PtsInfoIdxAddOne].u4FifoWp - _prAudOmxPtsInfo[u4PtsInfoIdx].u4FifoWp);
                }
                else
                {
                    u4WpDiff = (pHandle->u4FifoSize + _prAudOmxPtsInfo[u4PtsInfoIdxAddOne].u4FifoWp - _prAudOmxPtsInfo[u4PtsInfoIdx].u4FifoWp);
                }
                
                while ((!_prAudOmxPtsInfo[u4NextPtsInfoIdx].fgIsSet)|| (_prAudOmxPtsInfo[u4NextPtsInfoIdx].u4FifoWp==0))
                {
                    u4NextPtsInfoIdx++;
                    if (u4NextPtsInfoIdx == MAX_PTS_INFO_SZ)
                    {
                        u4NextPtsInfoIdx = 0;
                    }                    
                }
                    
                if (u4LastChkPtsPtr < _prAudOmxPtsInfo[u4PtsInfoIdx].u4FifoWp)
                {
                    //u4LastChkPtsPtr   PTS/WP[u4PtsInfoIdx]   PTS/WP[u4PtsInfoIdx+1]
                    pToFillBuf->rHeader.nTimeStamp = _prAudOmxPtsInfo[u4PtsInfoIdx].nTimeStamp - 
                        ((_prAudOmxPtsInfo[u4PtsInfoIdx].u4FifoWp - u4LastChkPtsPtr)*u4PtsDiff / u4WpDiff);
                }
                else
                {
                    pToFillBuf->rHeader.nTimeStamp = _prAudOmxPtsInfo[u4PtsInfoIdx].nTimeStamp + 
                        ((u4LastChkPtsPtr - _prAudOmxPtsInfo[u4PtsInfoIdx].u4FifoWp)*u4PtsDiff / u4WpDiff);
                }

                if (pToFillBuf->rHeader.nTimeStamp < 0)
                {
                    Printf("u4LastChkPtsPtr: %08x\n", u4LastChkPtsPtr);                
                    Printf("u4PtsInfoIdx: %d u4PtsInfoIdxAddOne: %d\n", u4PtsInfoIdx, u4PtsInfoIdxAddOne);
                    Printf("_prAudOmxPtsInfo[u4PtsInfoIdx]: %08x %08x%08x\n", _prAudOmxPtsInfo[u4PtsInfoIdx].u4FifoWp,
                            (UINT32)(((OMX_U64)(_prAudOmxPtsInfo[u4PtsInfoIdx].nTimeStamp)>>32)&0xffffffff),
                            (UINT32)((OMX_U64)(_prAudOmxPtsInfo[u4PtsInfoIdx].nTimeStamp)&0xffffffff));
                    Printf("_prAudOmxPtsInfo[u4PtsInfoIdxAddOne]: %08x %08x%08x\n", _prAudOmxPtsInfo[u4PtsInfoIdxAddOne].u4FifoWp,
                        (UINT32)(((OMX_U64)(_prAudOmxPtsInfo[u4PtsInfoIdxAddOne].nTimeStamp)>>32)&0xffffffff),
                        (UINT32)((OMX_U64)(_prAudOmxPtsInfo[u4PtsInfoIdxAddOne].nTimeStamp)&0xffffffff));
                    Printf("u4PtsDiff: 0x%08x\n", u4PtsDiff);
                    Printf("u4WpDiff: 0x%08x\n", u4WpDiff);
                }
                
                if (u4AOmxDbgMsk & 0x10)
                {
                    Printf("OUTPTS(%04d): %08x - %08x%08x\n",
                            u4PtsInfoIdx,
                            u4LastChkPtsPtr,
                            (UINT32)(((OMX_U64)(pToFillBuf->rHeader.nTimeStamp)>>32)&0xffffffff),
                            (UINT32)((OMX_U64)(pToFillBuf->rHeader.nTimeStamp)&0xffffffff));
                }

                //Update ReadIdx
                #if 1
                while (u4PtsInfoIdx_org != u4NextPtsInfoIdx_org)
                {
                    _prAudOmxPtsInfo[u4PtsInfoIdx_org].fgIsSet = FALSE;
                    u4PtsInfoIdx_org++;
                    if (u4PtsInfoIdx_org == MAX_PTS_INFO_SZ)
                    {
                        u4PtsInfoIdx_org = 0;
                    }
                }
                #else
                while (u4PtsInfoIdx != u4NextPtsInfoIdx)
                {
                    _prAudOmxPtsInfo[u4PtsInfoIdx].fgIsSet = FALSE;
                    u4PtsInfoIdx++;
                    if (u4PtsInfoIdx == MAX_PTS_INFO_SZ)
                    {
                        u4PtsInfoIdx = 0;
                    }
                }
                #endif
                
                u4LastChkPtsPtr = u4NextChkPtsPtr;
            }
        }
        pToFillBuf->rHeader.nTimeStamp += i4AOmxPtsOffset;
        //END

         // if fill is done, callback now
        i4AudOmxCompNotifyFillDone(pHandle, pToFillBuf);

        u1FillQueueNo--;
        u4TotalFillQueueDoneNo++;
        if (u4AOmxDbgMsk & 0x40)
        {
            Printf("QueueFillBuf Done(%d)\n", u1FillQueueNo);
        }
    }

    if (fgAllFilled && pHandle->fgGotEOS && fgAudOmxOutputNoData() && (retry_cnt >= 10)) //DANIEL_HACK
    {
        // if EOS received and no output sample now, callback
        PARAMETER_OMX_CB_EVENTHANDLER rEvent;

        rEvent.eEvent     = OMX_EventBufferFlag;
        rEvent.nData1     = 1; // output port index
        rEvent.nData2     = OMX_BUFFERFLAG_EOS;
        rEvent.pEventData = NULL;

        Printf("[AUD][OMX] notify OMX_BUFFERFLAG_EOS\n");

        i4AudOmxCompNotifyEvent(pHandle, &rEvent);
    }

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return i4Ret;
}


INT32 i4AudOmxForceAllBufFillDone(AUD_OMX_HANDLE_T *pHandle)
{
    INT32 i4Ret = AUD_FAIL;
    UINT32 u4Index;
    AUD_OMX_BUFFER_T *pToFillBuf = NULL;

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    if (u4AOmxDbgMsk & 0x40)
    {
        Printf("i4AudOmxForceAllBufFillDone\n");
    }
    SYS_Printf("[AUD][OMX] i4AudOmxForceAllBufFillDone\n");

    for (u4Index = 0; u4Index < OUTPUT_BUF_MAX; u4Index ++)
    {
        pToFillBuf = pHandle->pFillQ[u4Index];

        if (NULL != pToFillBuf)
        {
            i4AudOmxCompNotifyFillDone(pHandle, pToFillBuf);

            // clear
            pHandle->pFillQ[u4Index] = NULL;
        }
    }

    pHandle->fgFilling = FALSE;

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return i4Ret;
}


INT32 i4AudOmxAllocInputBuf(AUD_OMX_HANDLE_T *pHandle, UINT32 u4Size, UINT32 u4Port, AUD_OMX_BUFFER_T **ppBufInfo)
{
    // allocate input buffer from AFIFO
    // allocate continous buffer from AFIFO for decoder

    // to maintain the continuity of allocated buffers, we always free previously allocated buffers
    // and allocate a larger one containing both previous and current buffer

    UINT32 u4Index;

    void *pBufSA  = NULL;
    //void *pFifoSA = NULL;

    HANDLE_T hHeader = NULL_HANDLE;
    HANDLE_T hMemHandle = NULL_HANDLE;
    AUD_OMX_BUFFER_T *pAvailBuf = NULL;

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    // STEP 0: get a free input buffer info node
    for (u4Index = 0; u4Index < INPUT_BUF_MAX; u4Index ++)
    {
        if (NULL == _prAudOmxInputBuf[u4Index].pOwner)
        {
            pAvailBuf = &(_prAudOmxInputBuf[u4Index]);
            break;
        }
    }

    if (NULL == pAvailBuf)
    {
        // no more free nodes
        goto lblErrorReturn;
    }
    else
    {
        // initialize
        pAvailBuf->pOwner    = pHandle;
        pAvailBuf->hBuffer   = NULL_HANDLE;
        pAvailBuf->pBufSA    = NULL;
        pAvailBuf->u4BufSize = 0;
        pAvailBuf->hHeader   = NULL_HANDLE;
    }

    SYS_Printf("%s %d: x_kmem_create_handle\n", __FUNCTION__, __LINE__);
    hHeader = x_kmem_create_handle(&(pAvailBuf->rHeader), sizeof(OMX_BUFFERHEADERTYPE));

    if (NULL_HANDLE == hHeader)
    {
        goto lblErrorReturn;
    }
    else
    {
        pAvailBuf->hHeader = hHeader;
    }

    // STEP 1: allocate input buffer from AFIFO
    pBufSA = (void*)x_alloc_aligned_afifo_mem(u4Size, 256);

    if (NULL == pBufSA)
    {
        SYS_Printf("[AUD][OMX] alloc AFIFO memory failed, %d bytes.\n", u4Size);
        goto lblErrorReturn;
    }
    else
    {
        pAvailBuf->pBufSA    = pBufSA;
        pAvailBuf->u4BufSize = u4Size;
    }

    SYS_Printf("%s %d: x_kmem_create_handle\n", __FUNCTION__, __LINE__);
    hMemHandle = x_kmem_create_handle(pBufSA, u4Size);

    if (NULL_HANDLE == hMemHandle)
    {
        goto lblErrorReturn;
    }
    else
    {
        pAvailBuf->hBuffer = hMemHandle;
    }

    // fill buffer header
    x_memset(&(pAvailBuf->rHeader), 0x00, sizeof(OMX_BUFFERHEADERTYPE));

    pAvailBuf->rHeader.nSize             = sizeof(OMX_BUFFERHEADERTYPE);
    pAvailBuf->rHeader.nVersion.nVersion = OMX_VERSION;

    pAvailBuf->rHeader.pBuffer           = pBufSA;
    pAvailBuf->rHeader.nAllocLen         = u4Size;
    pAvailBuf->rHeader.nFilledLen        = 0;
    pAvailBuf->rHeader.nOffset           = 0;
    pAvailBuf->rHeader.nOutputPortIndex  = u4Port;

    // release semaphore
    i4AudOmxTaskUnlockSem();

    *ppBufInfo = pAvailBuf;
    return AUD_OK;

    // error handling
lblErrorReturn:
    // release semaphore
    i4AudOmxTaskUnlockSem();

    // resource release will be done on freeing component handle, not here
    *ppBufInfo = NULL;
    return AUD_FAIL;
}


INT32 i4AudOmxFreeInputBuf(AUD_OMX_HANDLE_T *pHandle, HANDLE_T hHeader)
{
    UINT32 u4Index;
    BOOL fgAllFreed = TRUE;
    AUD_OMX_BUFFER_T *pBufNode = NULL;

    if ((NULL == pHandle) || (NULL_HANDLE == hHeader))
    {
        return AUD_OK;
    }

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    for (u4Index = 0; u4Index < INPUT_BUF_MAX; u4Index ++)
    {
        if (hHeader == _prAudOmxInputBuf[u4Index].hHeader)
        {
            pBufNode = &(_prAudOmxInputBuf[u4Index]);
            break;
        }
    }

    if (NULL != pBufNode)
    {
        if ((pBufNode->pOwner == pHandle) && (NULL != pBufNode->pBufSA))
        {
            // free memory
            x_free_aligned_afifo_mem(pBufNode->pBufSA);

            // destory the buffer handle
            SYS_Printf("%s %d: x_kmem_destroy_handle\n", __FUNCTION__, __LINE__);
            x_kmem_destroy_handle(pBufNode->hBuffer);

            // destory the header handle
            SYS_Printf("%s %d: x_kmem_destroy_handle\n", __FUNCTION__, __LINE__);
            x_kmem_destroy_handle(pBufNode->hHeader);

            // clear info
            pBufNode->pOwner    = NULL;
            pBufNode->hBuffer   = NULL_HANDLE;
            pBufNode->pBufSA    = NULL;
            pBufNode->u4BufSize = 0;
            pBufNode->hHeader   = NULL_HANDLE;
        }
    }

    // check if all input buffers have been freed, then free the AFIFO memory
    for (u4Index = 0; u4Index < INPUT_BUF_MAX; u4Index ++)
    {
        if (pHandle == _prAudOmxInputBuf[u4Index].pOwner)
        {
            fgAllFreed = FALSE;
            break;
        }
    }

    if (fgAllFreed)
    {
        x_free_aligned_afifo_mem(pHandle->pFifoPntr);
        pHandle->pFifoPntr = NULL;
        pHandle->u4FifoSize = 0;
    }

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return AUD_OK;
}

INT32 i4AudOmxQueueEmptyBuf(AUD_OMX_HANDLE_T *pHandle, HANDLE_T hHeader)
{
    INT32 i4Ret = AUD_FAIL;
    UINT32 u4Index;
    AUD_OMX_BUFFER_T *pBufNode = NULL;

    if ((NULL == pHandle) || (NULL_HANDLE == hHeader))
    {
        return AUD_OK;
    }

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    for (u4Index = 0; u4Index < INPUT_BUF_MAX; u4Index ++)
    {
        if (hHeader == _prAudOmxInputBuf[u4Index].hHeader)
        {
            pBufNode = &(_prAudOmxInputBuf[u4Index]);
            break;
        }
    }

    if (NULL != pBufNode)
    {
        if (pBufNode->pOwner == pHandle)
        {
            // fill current empty parameters (copy from client filled memory)
            pBufNode->u4FilledLen = pBufNode->rHeader.nFilledLen;
            pBufNode->u4Offset    = pBufNode->rHeader.nOffset;
            pBufNode->nTimeStamp  = pBufNode->rHeader.nTimeStamp;
            pBufNode->fgEmptied   = FALSE;

            // add to queue
            for (u4Index = 0; u4Index < INPUT_BUF_MAX; u4Index ++)
            {
                if (NULL == pHandle->pEmptyQ[u4Index])
                {
                    i4Ret = AUD_OK;
                    pHandle->fgEmptying = TRUE;
                    pHandle->pEmptyQ[u4Index] = pBufNode;
                    u1EmptyQueueNo++;
                    u4TotalEmptyQueueNo++;
                    if (u4AOmxDbgMsk & 0x4)
                    {
                        Printf("Empty Queue(%d): %x %x %08x%08x\n", u1EmptyQueueNo, pBufNode->pBufSA, pBufNode->u4FilledLen, 
                                (UINT32)(((OMX_U64)(pBufNode->nTimeStamp)>>32)&0xffffffff),
                                (UINT32)((OMX_U64)(pBufNode->nTimeStamp)&0xffffffff));
                    }
                    break;
                }
            }

            if (AUD_OK != i4Ret)
            {
                if (u4AOmxDbgMsk & 0x4)
                {
                    Printf("Empty Queue full(%d) !!!!\n", u1EmptyQueueNo);
                }
            }
        }
    }

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return i4Ret;
}

//20110622
UINT16 u2RaFrameByteIdx = 0;
extern UINT16 u2RaFrameSizeInBytes;
//20110622

#if 0
static UINT32 u4AudOmxFillFIFO(AUD_OMX_HANDLE_T *pHandle, UINT32 u4From, UINT32 u4Size)
{
    UINT32 u4FifoSA, u4FifoEA;
    UINT32 u4FifoRp, u4FifoWp;
    INT32 i4FifoFree, i4Copied = 0;

    if (0 == u4Size)
    {
        return 0;
    }

    u4FifoSA = (UINT32) pHandle->pFifoPntr;
    u4FifoEA = (UINT32) pHandle->pFifoPntr + pHandle->u4FifoSize;
    u4FifoWp = (UINT32) pHandle->pFifoWptr;
    u4FifoRp = u4AudHalGetBufRPtr(FRTH_DEC, DSP_AFIFO);

    //printf("[AUD][OMX] u4AudOmxFillFIFO222: u4FifoWp = 0x%X, u4FifoRp = 0x%X\n", u4FifoWp, u4FifoRp);

    //calc i4FifoFree
    if (u4FifoRp > u4FifoWp){
            i4FifoFree = u4FifoRp - u4FifoWp - 8; // - 8 to avoid Wp == Rp after updated
     }else if (u4FifoRp < u4FifoWp){
            i4FifoFree = (u4FifoEA-u4FifoSA) - (u4FifoWp-u4FifoRp) -8;
     }else{ //u4FifoRp == u4FifoWp //Empty not really the first time!!!!
            i4FifoFree = u4FifoEA - u4FifoSA - 8; // - 8 to avoid Wp == Rp after updated
     }

    //calc i4Copied
     if (i4FifoFree >= u4Size){
            i4Copied = u4Size;    //copy all
     }else if (i4FifoFree > 0){
            i4Copied = i4FifoFree; //copy part
     }else{
            i4Copied = 0;  // copy none
     }

    //memcoy
    if (u4FifoRp > u4FifoWp){
            // copy bitstream from input buffer to FIFO
            //20110622
            if (pHandle->eFormat == AUD_FMT_COOK)
            {
                INT32 i;
                for (i=0;i<i4Copied;i++)
                {
                    switch (u2RaFrameByteIdx&0x3)
                    {
                    case 0:
                        *((UINT8*)(u4From+i)) ^= 0x37;
                        break;
                    case 1:
                        *((UINT8*)(u4From+i)) ^= 0xc5;
                        break;
                    case 2:
                        *((UINT8*)(u4From+i)) ^= 0x11;
                        break;
                    case 3:
                        *((UINT8*)(u4From+i)) ^= 0xf2;
                        break;
                    }
                    u2RaFrameByteIdx++;
                    if (u2RaFrameByteIdx == u2RaFrameSizeInBytes)
                        u2RaFrameByteIdx = 0;
                }
            }
            //20110622
            x_memcpy((void*)u4FifoWp, (const void*)u4From, i4Copied);
            // flush cache
            BSP_CleanDCacheRange(u4FifoWp, i4Copied);
            u4FifoWp += i4Copied;
     }else{ //u4FifoRp <= u4FifoWp
            i4FifoFree =  u4FifoEA - u4FifoWp; //i4FifoFree is temp: free sopt to the end
            if(i4FifoFree >= i4Copied){
                // copy bitstream from input buffer to FIFO
                //20110622
                if (pHandle->eFormat == AUD_FMT_COOK)
                {
                    INT32 i;
                    for (i=0;i<i4Copied;i++)
                    {
                        switch (u2RaFrameByteIdx&0x3)
                        {
                        case 0:
                            *((UINT8*)(u4From+i)) ^= 0x37;
                            break;
                        case 1:
                            *((UINT8*)(u4From+i)) ^= 0xc5;
                            break;
                        case 2:
                            *((UINT8*)(u4From+i)) ^= 0x11;
                            break;
                        case 3:
                            *((UINT8*)(u4From+i)) ^= 0xf2;
                            break;
                        }
                        u2RaFrameByteIdx++;
                        if (u2RaFrameByteIdx == u2RaFrameSizeInBytes)
                            u2RaFrameByteIdx = 0;
                    }
                }

                x_memcpy((void*)u4FifoWp, (const void*)u4From, i4Copied);
                // flush cache
                BSP_CleanDCacheRange(u4FifoWp, i4Copied);
                u4FifoWp += i4Copied;
            }else{
                // 1st part: copy to the u4FifoEA
                //20110622
                if (pHandle->eFormat == AUD_FMT_COOK)
                {
                    INT32 i;
                    for (i=0;i<i4FifoFree;i++)
                    {
                        switch (u2RaFrameByteIdx&0x3)
                        {
                        case 0:
                            *((UINT8*)(u4From+i)) ^= 0x37;
                            break;
                        case 1:
                            *((UINT8*)(u4From+i)) ^= 0xc5;
                            break;
                        case 2:
                            *((UINT8*)(u4From+i)) ^= 0x11;
                            break;
                        case 3:
                            *((UINT8*)(u4From+i)) ^= 0xf2;
                            break;
                        }
                        u2RaFrameByteIdx++;
                        if (u2RaFrameByteIdx == u2RaFrameSizeInBytes)
                            u2RaFrameByteIdx = 0;
                    }
                }

                x_memcpy((void*)u4FifoWp, (const void*)u4From, i4FifoFree);
                BSP_CleanDCacheRange(u4FifoWp, i4FifoFree);
                u4FifoWp = u4FifoSA;
                // 2nd part: copy the rest
                i4FifoFree = i4Copied-i4FifoFree;

                //20110622
                if (pHandle->eFormat == AUD_FMT_COOK)
                {
                    INT32 i;
                    for (i=0;i<i4FifoFree;i++)
                    {
                        switch (u2RaFrameByteIdx&0x3)
                        {
                        case 0:
                            *((UINT8*)(u4From+i)) ^= 0x37;
                            break;
                        case 1:
                            *((UINT8*)(u4From+i)) ^= 0xc5;
                            break;
                        case 2:
                            *((UINT8*)(u4From+i)) ^= 0x11;
                            break;
                        case 3:
                            *((UINT8*)(u4From+i)) ^= 0xf2;
                            break;
                        }
                        u2RaFrameByteIdx++;
                        if (u2RaFrameByteIdx == u2RaFrameSizeInBytes)
                            u2RaFrameByteIdx = 0;
                    }
                }

                x_memcpy((void*)u4FifoWp, (const void*)u4From, i4FifoFree);
                BSP_CleanDCacheRange(u4FifoWp, i4FifoFree);
                u4FifoWp+=i4FifoFree;
            }
     }

     // check if need roll back, make sure Wp != EA
     if (u4FifoWp == u4FifoEA){
            u4FifoWp = u4FifoSA;
      }


    // set new Wptr
    vAudHalSetBufWPtr(FRTH_DEC, DSP_AFIFO, u4FifoWp);
    //Printf("u4FifoWp: %x\n", u4FifoWp);
    pHandle->pFifoWptr = (void *)u4FifoWp;

    //SYS_Printf("[AUD][OMX] Fill FIFO: i4Copied = %4d, Wptr = 0x%X\n", i4Copied, u4FifoWp);

    return i4Copied;
}

#else

static UINT32 u4AudOmxFillFIFO(AUD_OMX_HANDLE_T *pHandle, UINT32 u4From, UINT32 u4Size)
{
    UINT32 u4FifoSA, u4FifoEA;
    UINT32 u4FifoRp, u4FifoWp;
    INT32 i4FifoFree, i4Copied = 0;

    if (0 == u4Size)
    {
        return 0;
    }

    u4FifoSA = (UINT32) pHandle->pFifoPntr;
    u4FifoEA = (UINT32) pHandle->pFifoPntr + pHandle->u4FifoSize;
    u4FifoWp = (UINT32) pHandle->pFifoWptr;
    u4FifoRp = u4AudHalGetBufRPtr(FRTH_DEC, DSP_AFIFO);

    //SYS_Printf("[AUD][OMX] u4AudOmxFillFIFO: u4FifoSA = 0x%X, u4FifoEA = 0x%X, u4FifoWp = 0x%X, u4FifoRp = 0x%X\n",
    //           u4FifoSA, u4FifoEA, u4FifoWp, u4FifoRp);

    #ifdef RESERVE_FIFO_4K
    {
        UINT32 u4FifoLeftSize = ((u4FifoWp >= u4FifoRp) ? (u4FifoEA - u4FifoSA + u4FifoRp - u4FifoWp) : (u4FifoRp - u4FifoWp));
        if (u4FifoLeftSize <= MAX_INPUT_BUF_SIZE)
        {
            return 0;
        }
    }
    #endif

    if (u4FifoRp >= u4FifoWp)
    {
        // |xxxxxxxx|--------------|xxxxxxxxxxxxxx|
        // SA       Wp             Rp             EA
        //          |--u4FifoFree--|
        if (u4FifoRp == u4FifoWp)
        {
            // if Wp == Rp == SA, it is the first time write FIFO
            // else if Wp == Rp, means FIFO is pure empty
            // it should not be FIFO full case, since we allways avoid filling FIFO to full
            i4FifoFree = u4FifoEA - u4FifoSA - 8; // - 8 to avoid Wp == Rp after updated
            SYS_Printf("[AUD][OMX] u4AudOmxFillFIFO: first time write or empty.\n");
        }
        else
        {
            i4FifoFree = u4FifoRp - u4FifoWp - 8; // - 8 to avoid Wp == Rp after updated
        }

        if (i4FifoFree >= u4Size)
        {
            // copy all
            i4Copied = u4Size;
        }
        else if (i4FifoFree > 0)
        {
            // copy part
            i4Copied = i4FifoFree;
        }
        else
        {
            // copy none
            i4Copied = 0;
        }

        if (i4Copied > 0)
        {
            // copy bitstream from input buffer to FIFO
            //20110622
            if (pHandle->eFormat == AUD_FMT_COOK)
            {
                INT32 i;
                for (i=0;i<i4Copied;i++)
                {
                    switch (u2RaFrameByteIdx&0x3)
                    {
                    case 0:
                        *((UINT8*)(u4From+i)) ^= 0x37;
                        break;
                    case 1:
                        *((UINT8*)(u4From+i)) ^= 0xc5;
                        break;
                    case 2:
                        *((UINT8*)(u4From+i)) ^= 0x11;
                        break;
                    case 3:
                        *((UINT8*)(u4From+i)) ^= 0xf2;
                        break;
                    }
                    u2RaFrameByteIdx++;
                    if (u2RaFrameByteIdx == u2RaFrameSizeInBytes)
                        u2RaFrameByteIdx = 0;
                }
            }
            //20110622
            x_memcpy((void*)u4FifoWp, (const void*)u4From, i4Copied);

            // flush cache
            BSP_CleanDCacheRange(u4FifoWp, i4Copied);

            u4FifoWp += i4Copied;
        }
    }
    else if (u4FifoRp < u4FifoWp)
    {
        // |--------|xxxxxxxxxxxxxx|--------------|
        // SA       Rp             Wp             EA
        //                         |--u4FifoFree--|

        if (u4FifoRp > u4FifoSA)
        {
            i4FifoFree = u4FifoEA - u4FifoWp;
        }
        else
        {
            // Rp == SA, - 8 to avoid Wp == Rp after updated
            i4FifoFree = u4FifoEA - u4FifoWp - 8;
        }

        if (i4FifoFree >= u4Size)
        {
            // copy all
            i4Copied = u4Size;
        }
        else if (i4FifoFree > 0)
        {
            // copy part
            i4Copied = i4FifoFree;
        }
        else
        {
            // copy none
            i4Copied = 0;
        }

        if (i4Copied > 0)
        {
            // copy bitstream from input buffer to FIFO
            //20110622
            if (pHandle->eFormat == AUD_FMT_COOK)
            {
                INT32 i;
                for (i=0;i<i4Copied;i++)
                {
                    switch (u2RaFrameByteIdx&0x3)
                    {
                    case 0:
                        *((UINT8*)(u4From+i)) ^= 0x37;
                        break;
                    case 1:
                        *((UINT8*)(u4From+i)) ^= 0xc5;
                        break;
                    case 2:
                        *((UINT8*)(u4From+i)) ^= 0x11;
                        break;
                    case 3:
                        *((UINT8*)(u4From+i)) ^= 0xf2;
                        break;
                    }
                    u2RaFrameByteIdx++;
                    if (u2RaFrameByteIdx == u2RaFrameSizeInBytes)
                        u2RaFrameByteIdx = 0;
                }
            }
            //20110622
            x_memcpy((void*)u4FifoWp, (const void*)u4From, i4Copied);

            // flush cache
            BSP_CleanDCacheRange(u4FifoWp, i4Copied);

            u4FifoWp += i4Copied;
        }

        // check if need roll back, make sure Wp != EA
        if (u4FifoWp == u4FifoEA)
        {
            u4FifoWp = u4FifoSA;
        }

        // check if need copy part 2
        if ((i4Copied < u4Size) && (u4FifoRp > u4FifoSA))
        {
            INT32 i4Copied2;

            // |--------|xxxxxxxxxxxxxx|--------------|
            // SA       Rp             Wp             EA
            // |--here--|

            i4FifoFree = u4FifoRp - u4FifoSA - 8;

            if (i4FifoFree >= (u4Size - i4Copied))
            {
                // copy all
                i4Copied2 = u4Size - i4Copied;
            }
            else if (i4FifoFree > 0)
            {
                // copy part
                i4Copied2 = i4FifoFree;
            }
            else
            {
                // copy none
                i4Copied2 = 0;
            }

            if (i4Copied2 > 0)
            {
                // copy bitstream from input buffer to FIFO
                //20110622
                if (pHandle->eFormat == AUD_FMT_COOK)
                {
                    INT32 i;
                    for (i=0;i<i4Copied2;i++)
                    {
                        switch (u2RaFrameByteIdx&0x3)
                        {
                        case 0:
                            *((UINT8*)(u4From + i4Copied+i)) ^= 0x37;
                            break;
                        case 1:
                            *((UINT8*)(u4From + i4Copied+i)) ^= 0xc5;
                            break;
                        case 2:
                            *((UINT8*)(u4From + i4Copied+i)) ^= 0x11;
                            break;
                        case 3:
                            *((UINT8*)(u4From + i4Copied+i)) ^= 0xf2;
                            break;
                        }
                        u2RaFrameByteIdx++;
                        if (u2RaFrameByteIdx == u2RaFrameSizeInBytes)
                            u2RaFrameByteIdx = 0;
                    }
                }
                //20110622

                x_memcpy((void*)u4FifoSA, (const void*)(u4From + i4Copied), i4Copied2);

                // flush cache
                BSP_CleanDCacheRange(u4FifoSA, i4Copied2);

                u4FifoWp += i4Copied2;
                i4Copied += i4Copied2;
            }
        }
    }

    // check if need roll back
    if (u4FifoWp == u4FifoEA)
    {
        u4FifoWp = u4FifoSA;
    }

    // set new Wptr
    vAudHalSetBufWPtr(FRTH_DEC, DSP_AFIFO, u4FifoWp);
    //Printf("u4FifoWp: %x\n", u4FifoWp);
    pHandle->pFifoWptr = (void *)u4FifoWp;

    //SYS_Printf("[AUD][OMX] Fill FIFO: i4Copied = %4d, Wptr = 0x%X\n", i4Copied, u4FifoWp);

    return i4Copied;
}
#endif

//DANIEL_HACK
UINT8 u1SampleRateIdx = 3;   //48K
UINT8 u1ChNo = 2;            //Stereo
#define SWDMX_AVI_AAC_FAKE_HDR_SIZE 7
static BOOL fgIsFirstChk = TRUE;
static BOOL fgEnableAdtsInsert = FALSE;
static UINT32 _u4VorbisPage = 0;
//DANIEL_HACK

INT32 i4AudOmxDoEmptyBuf(AUD_OMX_HANDLE_T *pHandle)
{
    INT32 i4Ret = AUD_FAIL;
    UINT32 u4EmptySize;
    UINT32 u4FifoRp;
    UINT32 u4Index;
    BOOL fgNeedEmpty = FALSE;
    AUD_OMX_BUFFER_T *pToEmptyBuf = NULL;
    //AUD_DEC_MSG_T wmalsl_msg; //ian wmalsl hack will modify later
    //Printf("i4AudOmxDoEmptyBuf\n");
    // check if this buffer has been copied to FIFO
    for (u4Index = 0; u4Index < INPUT_BUF_MAX - 1; u4Index ++)
    {
        pToEmptyBuf = pHandle->pEmptyQ[u4Index];

        if (NULL != pToEmptyBuf)
        {
            if (!pToEmptyBuf->fgEmptied)
            {
                fgNeedEmpty = TRUE;
                break;
            }
        }
    }

    if (!fgNeedEmpty)
    {
        return AUD_OK;
    }

    //Debug Code
    if (pToEmptyBuf->u4Offset == 0)
    {
        if (u4AOmxDbgMsk & 0x2)
        {
            Printf("(%d)Consume: %x %x\n", u4Index,
                       pToEmptyBuf->pBufSA,
                       pToEmptyBuf->u4FilledLen);
        }

        //PTS
        if (pToEmptyBuf->u4FilledLen)
        {
            //PTS_FREERUN
            if (nTimeStampCur == -1)
            {
                nTimeStampCur = pToEmptyBuf->nTimeStamp;
            }
            //END
            
            if (u4AOmxDbgMsk & 0x20)
            {
                if ((pHandle->eFormat == AUD_FMT_WMA10LOSSLESS) ||
                     (pHandle->eFormat == AUD_FMT_WMA10SPEECH) ||
                     (pHandle->eFormat == AUD_FMT_AMRNB) ||
                     (pHandle->eFormat == AUD_FMT_AMRWB) )
                {
                    //TODO
                }
                else
                {            
                    vAudOmxSetPTS(pHandle,pToEmptyBuf->nTimeStamp);
                }
            }

            //DBG Log
            if (u4AOmxDbgMsk & 0x100)
            {
                if ((pHandle->eFormat == AUD_FMT_WMA10LOSSLESS) ||
                     (pHandle->eFormat == AUD_FMT_WMA10SPEECH) ||
                     (pHandle->eFormat == AUD_FMT_AMRNB) ||
                     (pHandle->eFormat == AUD_FMT_AMRWB) )
                {
                    //TODO
                }
                else
                {
                    Printf("u4FifoWp: %08x: %08x%08x\n",
                            (UINT32)(pHandle->pFifoWptr),
                            (UINT32)(((OMX_U64)(pToEmptyBuf->nTimeStamp)>>32)&0xffffffff),
                            (UINT32)((OMX_U64)(pToEmptyBuf->nTimeStamp)&0xffffffff));
                }
            }
            //
        }
        //End
    }

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

#ifndef DISABLE_WMALSL_AMR
    if ((pHandle->eFormat == AUD_FMT_WMA10LOSSLESS) ||
         (pHandle->eFormat == AUD_FMT_WMA10SPEECH) ||
         (pHandle->eFormat == AUD_FMT_AMRNB) ||
         (pHandle->eFormat == AUD_FMT_AMRWB) )
    { //ian wmalsl hack will modify later
        /*
        if (fgIsFirstChk)
        {
            wmalsl_msg.eCmd = AUD_DEC_CMD_SET_STATE;
            wmalsl_msg.eStateTo = AUD_DEC_STATE_PLAY;
            i4AudWMALSLTaskSendCommand(&hriscdec, &wmalsl_msg);
            pHandle->pFifoPntr = VIRTUAL(u4GetAFIFOStart(2));
            pHandle->u4FifoSize = (UINT32)(VIRTUAL(u4GetAFIFOEnd(2)) - VIRTUAL(u4GetAFIFOStart(2)));
            pHandle->pFifoWptr = pHandle->pFifoPntr;
            fgIsFirstChk = FALSE;
        }
        */
        u4FifoRp = (UINT32)(hriscdec.pInBufBUFPNT);
        u4EmptySize = u4AudOmxFillBSFIFO(&hriscdec, (UINT32) pToEmptyBuf->pBufSA + pToEmptyBuf->u4Offset, pToEmptyBuf->u4FilledLen);
        pHandle->pFifoWptr = hriscdec.pInBufPRSPNT;

    }
    else
#endif        
    {    //ian wmalsl hack will modify later

        u4FifoRp = u4AudHalGetBufRPtr(FRTH_DEC, DSP_AFIFO);

        //DANIEL_HACK
        if (fgIsFirstChk)
        {
            ////////////////////////////////////////////////////////////
            //AAC Check
            if ((pToEmptyBuf->u4Offset == 0) &&
                (pToEmptyBuf->u4FilledLen < MAX_INPUT_BUF_SIZE) &&
                (pHandle->eFormat == AUD_FMT_AAC) &&
                !((((UINT8*)(pToEmptyBuf->pBufSA))[0] == 0xff) && ((((UINT8*)(pToEmptyBuf->pBufSA))[1] & 0xf0) == 0xf0)) &&
                !((((UINT8*)(pToEmptyBuf->pBufSA))[0] == 0x56) && ((((UINT8*)(pToEmptyBuf->pBufSA))[1] & 0xf0) == 0xe0))
                )
            {
                fgEnableAdtsInsert = TRUE;
                Printf("Enable AAC ADTS insert !!!!\n");
            }
            //End
            ////////////////////////////////////////////////////////////

            ////////////////////////////////////////////////////////////
            //VORBIS Check
            if ((pToEmptyBuf->u4Offset == 0) &&
                (pToEmptyBuf->u4FilledLen < MAX_INPUT_BUF_SIZE) &&
                (pHandle->eFormat == AUD_FMT_VORBISDEC) &&
                !((((UINT8*)(pToEmptyBuf->pBufSA))[0] == 0x4F) && (((UINT8*)(pToEmptyBuf->pBufSA))[1]  == 0x67) && (((UINT8*)(pToEmptyBuf->pBufSA))[2]  == 0x67) && (((UINT8*)(pToEmptyBuf->pBufSA))[3]  == 0x53)))
            {
                _u4VorbisPage = 0;
                fgEnableAdtsInsert = TRUE;
                Printf("Enable Vorbis OggS insert !!!!\n");
            }
            //End
            ////////////////////////////////////////////////////////////

            ////////////////////////////////////////////////////////////
            //LPCM Check
            if ((pToEmptyBuf->u4Offset == 0) &&
                (pToEmptyBuf->u4FilledLen < MAX_INPUT_BUF_SIZE) &&
                (pToEmptyBuf->u4FilledLen > 3) &&
                (pHandle->eFormat == AUD_FMT_LPCM))
            {
                Printf("LPCM delay play: %02x%02x\n",
                            ((UINT8*)(pToEmptyBuf->pBufSA))[1],
                            ((UINT8*)(pToEmptyBuf->pBufSA))[2]);
                if(((UINT8*)(pToEmptyBuf->pBufSA))[0]==0) // audio/x-private-ts-lpcm
                {
                    AUD_SetLPCMFmtTS(AUD_DEC_MAIN,
                                    ((UINT16)((((UINT8*)(pToEmptyBuf->pBufSA))[1])<<8) | (((UINT8*)(pToEmptyBuf->pBufSA))[2])));
                }
                else if(((UINT8*)(pToEmptyBuf->pBufSA))[0]==1) // audio/x-private1-lpcm
                {
                    AUD_SetLPCMFmtPS(AUD_DEC_MAIN,
                                    ((UINT16)((((UINT8*)(pToEmptyBuf->pBufSA))[1])<<8) | (((UINT8*)(pToEmptyBuf->pBufSA))[2])));
                }
                AUD_DSPCmdPlay(AUD_DEC_MAIN);
                u1AudDspState = 1;
                pToEmptyBuf->u4Offset += 3;
                pToEmptyBuf->u4FilledLen -= 3;
            }
            //End
            ////////////////////////////////////////////////////////////

            fgIsFirstChk = FALSE;
        }

        if (fgEnableAdtsInsert && pToEmptyBuf->u4FilledLen)
        {
            if ((pToEmptyBuf->u4Offset == 0) &&
                (pToEmptyBuf->u4FilledLen != MAX_INPUT_BUF_SIZE) &&
                (pHandle->eFormat == AUD_FMT_AAC) &&
                !((((UINT8*)(pToEmptyBuf->pBufSA))[0] == 0xff) && ((((UINT8*)(pToEmptyBuf->pBufSA))[1] & 0xf0) == 0xf0)))
            {
                UINT8 u1AAC_ADTS[7];
                //UINT32 u4EmptySize;
                UINT32 u4LeftSize = 7;

                u1AAC_ADTS[0] = 0xff;
                u1AAC_ADTS[1] = 0xf9;
                u1AAC_ADTS[2] = (1<<6) | ((u1SampleRateIdx<<2) & 0x3c) | ((u1ChNo>>2) & 0x1);
                u1AAC_ADTS[3] = ((u1ChNo & 0x3)<<6) | (((pToEmptyBuf->u4FilledLen + SWDMX_AVI_AAC_FAKE_HDR_SIZE)>>11)&0x3);
                u1AAC_ADTS[4] = (((pToEmptyBuf->u4FilledLen + SWDMX_AVI_AAC_FAKE_HDR_SIZE)>> 3) & 0xff);
                u1AAC_ADTS[5] = (((pToEmptyBuf->u4FilledLen + SWDMX_AVI_AAC_FAKE_HDR_SIZE)<<5) & 0xE0) | ((0x7FF>>6)&0x1F);
                u1AAC_ADTS[6] = ((0x7FF<<2)&0xFC);

                //first of buffer start
                while (u4LeftSize)
                {
                    //Printf("AAC lack of ADTS 7 bytes: %d\n", u4LeftSize);
                    u4LeftSize -= u4AudOmxFillFIFO(pHandle, (UINT32)u1AAC_ADTS+7-u4LeftSize, u4LeftSize);
                }
            }

            if ((pToEmptyBuf->u4Offset == 0) &&
                (pToEmptyBuf->u4FilledLen != MAX_INPUT_BUF_SIZE) &&
                (pHandle->eFormat == AUD_FMT_VORBISDEC) &&
                !((((UINT8*)(pToEmptyBuf->pBufSA))[0] == 0x4F) && (((UINT8*)(pToEmptyBuf->pBufSA))[1]  == 0x67) && (((UINT8*)(pToEmptyBuf->pBufSA))[2]  == 0x67) && (((UINT8*)(pToEmptyBuf->pBufSA))[3]  == 0x53)))
            {
                UINT8 u1VORBIS_OggS[27+255];    ///[255] for Segment Table.
                UINT32 u4EmptySize1;
                UINT32 u4LeftSize1 = 27;        ///Header basic size.
                UINT8 u1Temp;

                ///"OggS"
                u1VORBIS_OggS[0] = 0x4F;
                u1VORBIS_OggS[1] = 0x67;
                u1VORBIS_OggS[2] = 0x67;
                u1VORBIS_OggS[3] = 0x53;
                ///"Version" must be 0x00
                u1VORBIS_OggS[4] = 0x00;
                ///"Type": =1 continue page, =2 BOS, =4 BOE.
                u1VORBIS_OggS[5] = 0x00;
                ///"Granule=8, S/N=4, Page no=4, CRC=4" bytes
                for(u1Temp=6;u1Temp<26;u1Temp++)
                {
                    u1VORBIS_OggS[u1Temp]=0;
                }

                /// Set "Page" Information
                u1VORBIS_OggS[18] = (UINT8)(_u4VorbisPage & 0xFF);
                u1VORBIS_OggS[19] = (UINT8)((_u4VorbisPage>>8) & 0xFF);
                u1VORBIS_OggS[20] = (UINT8)((_u4VorbisPage>>16) & 0xFF);
                u1VORBIS_OggS[21] = (UINT8)((_u4VorbisPage>>24) & 0xFF);
                _u4VorbisPage += 1;

                if((pToEmptyBuf->u4FilledLen)<= 0xFF)
                {
                    u1Temp = 1;     /// Only one segment
                    u1VORBIS_OggS[26] = 0x01;
                    u1VORBIS_OggS[27] = (pToEmptyBuf->u4FilledLen);
                }
                else
                {
                    u1Temp = 1;     /// Need accumulate segments number.
                    u4EmptySize1 = (pToEmptyBuf->u4FilledLen);
                    while(u4EmptySize1 > 0xFF)
                    {
                       u4EmptySize1 -= 0xFF;
                       /// Set Segments table
                       u1VORBIS_OggS[26+u1Temp] = 0xFF;  ///[26+1]~[26+255]
                       u1Temp ++;
                    }
                    /// "Segments Number"=1
                    u1VORBIS_OggS[26] = u1Temp;
                    /// Last segment of "Segment Table"
                    u1VORBIS_OggS[26+u1Temp] = u4EmptySize1;    ///Update last segment table.
                }

                /// Fill pseudo "OggS" header to AFIFO.
                u4LeftSize1 += u1Temp;      /// Total add OggS Byte Size
                u1Temp = u4LeftSize1;
                while (u4LeftSize1)
                {
                    u4LeftSize1 -= u4AudOmxFillFIFO(pHandle, (UINT32)u1VORBIS_OggS+u1Temp-u4LeftSize1, u4LeftSize1);
                }

        #ifdef VORBIS_DBG
                if((pHandle->eFormat == AUD_FMT_VORBISDEC) && (_u4VorbisPage<10))
                {
                    Printf("@@@@@@@@@@@@@@@@@@@@@\n");
                    Printf("pHandle->eFormat = 0x%x\n", (UINT32)(pHandle->eFormat));
                    Printf("pToEmptyBuf->u4FilledLen = 0x%x\n", (UINT32)(pToEmptyBuf->u4FilledLen));
                    Printf("pToEmptyBuf->u4Offset = 0x%x\n", (UINT32)(pToEmptyBuf->u4Offset));
                    Printf("@@@@@@@@@@@@@@@@@@@@@\n");

                    Printf("OggS Head Bytes: 0x%x\n", u1Temp);  ///u1Temp = u4LeftSize1

                    /// ### Printf OggS Header and +5 Segments table
                    Printf("OggS-START !!!\n");
                    for(u1Temp=0;u1Temp<(27+5);u1Temp++)
                    {
                        Printf("0x%x, ", u1VORBIS_OggS[u1Temp]);
                    }
                    Printf("\nOggS-END !!!\n");
                    /// ### Printf OggS Header End
                }
        #endif
            }

        }

        //for audio/x-lpcm
#if 0
        if ((pToEmptyBuf->u4Offset == 0) &&
            (pToEmptyBuf->u4FilledLen < MAX_INPUT_BUF_SIZE) &&
            (pToEmptyBuf->u4FilledLen > 5) &&
            (pHandle->eFormat == AUD_FMT_LPCM))
        {
            pToEmptyBuf->u4Offset += 5;
            pToEmptyBuf->u4FilledLen -= 5;
        }
        //DANIEL_HACK
#endif
        u4EmptySize = u4AudOmxFillFIFO(pHandle, (UINT32) pToEmptyBuf->pBufSA + pToEmptyBuf->u4Offset, pToEmptyBuf->u4FilledLen);
    } //ian wmalsl hack will modify later
    ASSERT(u4EmptySize <= pToEmptyBuf->u4FilledLen);
//    if (u4EmptySize != pToEmptyBuf->u4FilledLen)
//        printf("u4EmptySize FAILED!\n");

    pToEmptyBuf->u4Offset += u4EmptySize;
    pToEmptyBuf->u4FilledLen -= u4EmptySize;

    //2011/7/28 for flush cmd
    if (fgRecvFlushCmd)
    {
        vAudOmxRestartDecoding(pHandle);
    }
    //End

    if (0 == pToEmptyBuf->u4FilledLen)
    {
        if (u4AOmxDbgMsk & 0x2)
        {
            Printf("(%d)Finish: %x\n", u4Index,
                       pToEmptyBuf->pBufSA);
        }

        // if original buffer filled len is 0, set this flag means it has been processed (althrough
        // it does not contain valid bitstream data, it may bring some flags, e.g. EOS flag)
        pToEmptyBuf->fgEmptied = TRUE;

        // this buffer is full emptied, now save current FIFO Rptr & Wptr
        // when Rptr is over this Wptr later, this buffer is really consumed, we will notify to client
        pToEmptyBuf->u4FifoRp = u4FifoRp;
        pToEmptyBuf->u4FifoWp = (UINT32) pHandle->pFifoWptr;
        pToEmptyBuf->u4FifoFreezeCnt = 0;

        // check if contains EOS flag
        if (OMX_BUFFERFLAG_EOS & pToEmptyBuf->rHeader.nFlags)
        {
            Printf("[AUD][OMX] EOS flag received, send flush command.\n");

            pHandle->fgGotEOS = TRUE;
            i4AsvSendFlushCmd(FRTH_DEC);
        }
    }

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return i4Ret;
}


INT32 i4AudOmxCheckBufEmptyDone(AUD_OMX_HANDLE_T *pHandle)
{
    INT32 i4Ret = AUD_FAIL;
    UINT32 u4Index;
    UINT32 u4FifoNewRp;
    UINT32 u4FifoOldRp;
    UINT32 u4FifoOldWp;
    BOOL fgEmptyDone = FALSE;
    BOOL fgAllEmptied = TRUE;
    AUD_OMX_BUFFER_T *pToEmptyBuf = NULL;

    // always left aligned
    pToEmptyBuf = pHandle->pEmptyQ[0];

    if (NULL == pToEmptyBuf)
    {
        // error handling
        pHandle->fgEmptying = FALSE;

        return AUD_OK;
    }

    if (pToEmptyBuf->u4FilledLen > 0)
    {
        // still not full copied to AFIFO
        return AUD_OK;
    }

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    // get current Rptr
#ifndef DISABLE_WMALSL_AMR    
    if ((pHandle->eFormat == AUD_FMT_WMA10LOSSLESS) ||
         (pHandle->eFormat == AUD_FMT_WMA10SPEECH) ||
         (pHandle->eFormat == AUD_FMT_AMRNB) ||
         (pHandle->eFormat == AUD_FMT_AMRWB) )
    {
        u4FifoNewRp = (UINT32)(hriscdec.pInBufBUFPNT);
    }
    else
#endif        
    {
        u4FifoNewRp = u4AudHalGetBufRPtr(FRTH_DEC, DSP_AFIFO);
    }

    if (!(u4AOmxDbgMsk & 0x8))
    {
        fgEmptyDone = TRUE;
    }
    else
    {
    u4FifoOldRp = pToEmptyBuf->u4FifoRp;
    u4FifoOldWp = pToEmptyBuf->u4FifoWp;

    //SYS_Printf("u4FifoNewRp = 0x%X, u4FifoOldRp = 0x%X, u4FifoOldWp = 0x%X\n", u4FifoNewRp, u4FifoOldRp, u4FifoOldWp);

    // check if all the data has been consumed
    if (u4FifoOldRp < u4FifoOldWp)
    {
        // SA     OldRp          OldWp            EA
        // |--------|xxxxxxxxxxxxxx|--------------|
        //     ^           ^              ^
        //     1           2              3

        if (u4FifoNewRp >= u4FifoOldWp)
        {
            // case 3
            fgEmptyDone = TRUE;
        }
        else if (u4FifoNewRp < u4FifoOldRp)
        {
            // case 1
            fgEmptyDone = TRUE;
        }
    }
    else
    {
        // SA     OldWp          OldRp            EA
        // |xxxxxxxx|--------------|xxxxxxxxxxxxxxx|
        //     ^           ^              ^
        //     1           2              3

        if ((u4FifoNewRp >= u4FifoOldWp) && (u4FifoNewRp < u4FifoOldRp))
        {
            // case 2
            fgEmptyDone = TRUE;
        }
    }

    }

    if (!fgEmptyDone)
    {
        // if data not fully consumed, need check if the decoding is frozen (input data is not enough
        // to decode a frame)
        if (pToEmptyBuf->u4FifoRp == u4FifoNewRp)
        {
            // if output buffer is empty, decoding is frozen
            if (fgAudOmxOutputNoData())
            {
                pToEmptyBuf->u4FifoFreezeCnt ++;

                if (pToEmptyBuf->u4FifoFreezeCnt > 4)
                {
                    // special case
                    fgEmptyDone = TRUE;
                }
            }
        }
        else
        {
            // save newest Rptr (for checking if FIFO freeze)
            pToEmptyBuf->u4FifoRp = u4FifoNewRp;
            pToEmptyBuf->u4FifoFreezeCnt = 0;
        }
    }

    if (fgEmptyDone)
    {
        // if empty is done, align queue to left and check if there is pending empty event
        for (u4Index = 0; u4Index < INPUT_BUF_MAX - 1; u4Index ++)
        {
            pHandle->pEmptyQ[u4Index] = pHandle->pEmptyQ[u4Index + 1];

            if (NULL != pHandle->pEmptyQ[u4Index])
            {
                fgAllEmptied = FALSE;
            }
        }

        // clear the tail node
        pHandle->pEmptyQ[INPUT_BUF_MAX - 1] = NULL;

        if (fgAllEmptied)
        {
            // empty queue is empty
            pHandle->fgEmptying = FALSE;
        }

        // if empty is done, callback now
        i4AudOmxCompNotifyEmptyDone(pHandle, pToEmptyBuf);

        u1EmptyQueueNo--;
        u4TotalEmptyQueueDoneNo++;
        if (u4AOmxDbgMsk & 0x4)
        {
            Printf("Empty Queue Done(%d)\n", u1EmptyQueueNo);
        }
    }

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return i4Ret;
}


INT32 i4AudOmxForceAllBufEmptyDone(AUD_OMX_HANDLE_T *pHandle)
{
    INT32 i4Ret = AUD_FAIL;
    UINT32 u4Index;
    AUD_OMX_BUFFER_T *pToEmptyBuf = NULL;

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    if (u4AOmxDbgMsk & 0x40)
    {
        Printf("i4AudOmxForceAllBufEmptyDone\n");
    }
    SYS_Printf("[AUD][OMX] i4AudOmxForceAllBufEmptyDone\n");

    for (u4Index = 0; u4Index < INPUT_BUF_MAX; u4Index ++)
    {
        pToEmptyBuf = pHandle->pEmptyQ[u4Index];

        if (NULL != pToEmptyBuf)
        {
            i4AudOmxCompNotifyEmptyDone(pHandle, pToEmptyBuf);

            // clear
            pHandle->pEmptyQ[u4Index] = NULL;
        }
    }

    pHandle->fgEmptying = FALSE;

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return i4Ret;
}


BOOL fgAudOmxAllInputBufferFreed(AUD_OMX_HANDLE_T *pHandle)
{
    UINT32 u4Index;
    BOOL fgAllFreed = TRUE;

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    for (u4Index = 0; u4Index < INPUT_BUF_MAX; u4Index ++)
    {
        if (pHandle == _prAudOmxInputBuf[u4Index].pOwner)
        {
            fgAllFreed = FALSE;
            break;
        }
    }

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return fgAllFreed;
}

BOOL fgAudOmxAllOutputBufferFreed(AUD_OMX_HANDLE_T *pHandle)
{
    UINT32 u4Index;
    BOOL fgAllFreed = TRUE;

    // multi-instance protection, take semaphore
    i4AudOmxTaskLockSem();

    for (u4Index = 0; u4Index < OUTPUT_BUF_MAX; u4Index ++)
    {
        if (pHandle == _prAudOmxOutputBuf[u4Index].pOwner)
        {
            fgAllFreed = FALSE;
            break;
        }
    }

    // release semaphore
    i4AudOmxTaskUnlockSem();

    return fgAllFreed;
}

extern BOOL skip_create_feed_stream; //DANIEL_HACK


static UINT32 u4AudOmxFillBSFIFO(void *pvHandle, UINT32 u4From, UINT32 u4Size)
{
    AUD_DEC_HANDLE_T * pHandle  = (AUD_DEC_HANDLE_T *)pvHandle;
    UINT32 u4FifoSA, u4FifoEA;
    UINT32 u4FifoRp, u4FifoWp;
    INT32 i4FifoFree;
    UINT32 u4Copied = 0;
    UINT32 u4CopiedPart = 0;


    if (0 == u4Size)
    {
        return 0;
    }

    u4FifoSA = (UINT32) pHandle->pInBufSTRBLK;
    u4FifoEA = (UINT32) pHandle->pInBufENDBLK;
    u4FifoWp = (UINT32) pHandle->pInBufPRSPNT;
    u4FifoRp = (UINT32) pHandle->pInBufBUFPNT;

//    printf("[AUD][OMX] u4AudOmxFillFIFO111: u4FifoWp = 0x%X, u4FifoRp = 0x%X, FillSize = 0x%X\n", u4FifoWp, u4FifoRp,u4Size);

    //calc i4FifoFree
    if (u4FifoRp > u4FifoWp){
            i4FifoFree = u4FifoRp - u4FifoWp - 8; // - 8 to avoid Wp == Rp after updated
     }else if (u4FifoRp < u4FifoWp){
            i4FifoFree = (u4FifoEA-u4FifoSA) - (u4FifoWp-u4FifoRp) -8;
     }else{ //u4FifoRp == u4FifoWp //Empty not really the first time!!!!
            i4FifoFree = u4FifoEA - u4FifoSA - 8; // - 8 to avoid Wp == Rp after updated
     }

    //calc i4Copied
     if (i4FifoFree >= u4Size){
            u4Copied = u4Size;    //copy all
     }else if (i4FifoFree > 0){
            u4Copied = i4FifoFree; //copy part
     }else{
            u4Copied = 0;  // copy none
     }

    //memcoy
    if (u4FifoRp > u4FifoWp){
            // copy bitstream from input buffer to FIFO
            x_memcpy((void*)u4FifoWp, (const void*)u4From, u4Copied);
            // flush cache
//R to R            BSP_CleanDCacheRange(u4FifoWp, i4Copied);
            u4FifoWp += u4Copied;
     }else{ //u4FifoRp <= u4FifoWp
            u4CopiedPart =  u4FifoEA - u4FifoWp; //i4FifoFree is temp: free sopt to the end
            if(u4CopiedPart >= u4Copied){
                // copy bitstream from input buffer to FIFO
                x_memcpy((void*)u4FifoWp, (const void*)u4From, u4Copied);
                // flush cache
//R to R                BSP_CleanDCacheRange(u4FifoWp, i4Copied);
                u4FifoWp += u4Copied;
            }else{
                // 1st part: copy to the u4FifoEA
                x_memcpy((void*)u4FifoWp, (const void*)u4From, u4CopiedPart);
//R to R                BSP_CleanDCacheRange(u4FifoWp, i4FifoFree);
                u4FifoWp = u4FifoSA;
                u4From += u4CopiedPart;
                // 2nd part: copy the rest
                u4CopiedPart = u4Copied-u4CopiedPart;
                x_memcpy((void*)u4FifoWp, (const void*)u4From, u4CopiedPart);
//R to R                BSP_CleanDCacheRange(u4FifoWp, i4FifoFree);
                u4FifoWp+=u4CopiedPart;
            }
     }

     // check if need roll back, make sure Wp != EA
     if (u4FifoWp == u4FifoEA){
            u4FifoWp = u4FifoSA;
      }

    pHandle->pInBufPRSPNT  = (void *)u4FifoWp;

    //SYS_Printf("[AUD][OMX] Fill FIFO: i4Copied = %4d, Wptr = 0x%X\n", i4Copied, u4FifoWp);

    return u4Copied;
}

static UINT32 u4AudOmxFillPCM2FIFO(void *pvHandle, UINT32 u4From, UINT32 u4Size)
{
    AUD_DEC_HANDLE_T * pHandle  = (AUD_DEC_HANDLE_T *)pvHandle;
    UINT32 u4FifoSA, u4FifoEA;
    UINT32 u4FifoRp, u4FifoWp;
    INT32 i4FifoFree;
    UINT32 u4Copied = 0;
    UINT32 u4CopiedPart = 0;

    if (0 == u4Size)
    {
        return 0;
    }

    u4FifoSA = (UINT32) pHandle->pOutBufSTRBLK;
    u4FifoEA = (UINT32) pHandle->pOutBufSTRBLK + pHandle->u4OutBufSize;
    u4FifoWp = (UINT32) pHandle->pOutBufPRSPNT;
    u4FifoRp = u4AudHalGetBufRPtr(FRTH_DEC, DSP_AFIFO);
//    u4FifoRp = u4FifoSA;

    //printf("[AUD][OMX] u4AudOmxFillFIFO222: u4FifoWp = 0x%X, u4FifoRp = 0x%X\n", u4FifoWp, u4FifoRp);

        //calc i4FifoFree
    if (u4FifoRp > u4FifoWp){
            i4FifoFree = u4FifoRp - u4FifoWp - 8; // - 8 to avoid Wp == Rp after updated
     }else if (u4FifoRp < u4FifoWp){
            i4FifoFree = (u4FifoEA-u4FifoSA) - (u4FifoWp-u4FifoRp) -8;
     }else{ //u4FifoRp == u4FifoWp //Empty not really the first time!!!!
            i4FifoFree = u4FifoEA - u4FifoSA - 8; // - 8 to avoid Wp == Rp after updated
     }

    //calc i4Copied
     if (i4FifoFree >= u4Size){
            u4Copied = u4Size;    //copy all
     }else if (i4FifoFree > 0){
            u4Copied = i4FifoFree; //copy part
     }else{
            u4Copied = 0;  // copy none
     }

    //memcoy
    if (u4FifoRp > u4FifoWp){
            // copy bitstream from input buffer to FIFO
            x_memcpy((void*)u4FifoWp, (const void*)u4From, u4Copied);
            // flush cache
            BSP_CleanDCacheRange(u4FifoWp, u4Copied);
            u4FifoWp += u4Copied;
     }else{ //u4FifoRp <= u4FifoWp
            u4CopiedPart =  u4FifoEA - u4FifoWp; //i4FifoFree is temp: free sopt to the end
            if(u4CopiedPart >= u4Copied){
                // copy bitstream from input buffer to FIFO
                x_memcpy((void*)u4FifoWp, (const void*)u4From, u4Copied);
                // flush cache
                BSP_CleanDCacheRange(u4FifoWp, u4Copied);
                u4FifoWp += u4Copied;
            }else{
                // 1st part: copy to the u4FifoEA
                x_memcpy((void*)u4FifoWp, (const void*)u4From, u4CopiedPart);
                BSP_CleanDCacheRange(u4FifoWp, u4CopiedPart);
                u4FifoWp = u4FifoSA;
                u4From += u4CopiedPart;
                // 2nd part: copy the rest
                u4CopiedPart = u4Copied-u4CopiedPart;
                x_memcpy((void*)u4FifoWp, (const void*)u4From, u4CopiedPart);
                BSP_CleanDCacheRange(u4FifoWp, u4CopiedPart);
                u4FifoWp+=u4CopiedPart;
            }
     }

     // check if need roll back, make sure Wp != EA
     if (u4FifoWp == u4FifoEA){
            u4FifoWp = u4FifoSA;
      }

    // set new Wptr
    vAudHalSetBufWPtr(FRTH_DEC, DSP_AFIFO, u4FifoWp);
    //Printf("u4FifoWp: %x\n", u4FifoWp);
    pHandle->pOutBufPRSPNT  = (void *)u4FifoWp;

    //SYS_Printf("[AUD][OMX] Fill FIFO: i4Copied = %4d, Rptr = 0x%X, Wptr = 0x%X\n", i4Copied, u4FifoRp,u4FifoWp);

    return u4Copied;
}
INT32 i4AudOmxPrepareDecoding(AUD_FMT_T eFormat, void* pvArg)
{
#if 1 //DANIEL_HACK
    AUD_LPCM_INFO_T rLPCMInfo = {0};
    AUD_AMR_CFG_T rAMRInfo = {0};

    AUD_FMT_T eFormatDecoder = eFormat;
    skip_create_feed_stream = TRUE;

#ifndef DISABLE_WMALSL_AMR
    //Prepare setting for RISC decoder
    //AFIFO3 as RISC decoder input
    hriscdec.pInBufSTRBLK = (void*)VIRTUAL(u4GetAFIFOStart(AUD_DEC_THIRD));
    hriscdec.pInBufENDBLK = (void*)VIRTUAL(u4GetAFIFOEnd(AUD_DEC_THIRD));

    //AFIFO1 as RISC decoder output
    hriscdec.pOutBufSTRBLK = (void*)u4AfifoSA;
    hriscdec.u4OutBufSize = u4AfifoEA - u4AfifoSA;
    hriscdec.pOutBufPRSPNT = hriscdec.pOutBufSTRBLK;
    hriscdec.pu4AudDecFillFIFO = u4AudOmxFillPCM2FIFO;
#endif

    //FOR_LG_2ND_ATTACK
      switch (eFormat)
      {
          default:
              break;
          case AUD_FMT_OMX_MSADPCM:
              eFormatDecoder = AUD_FMT_LPCM;
              break;
          case AUD_FMT_OMX_IMAADPCM:
              eFormatDecoder = AUD_FMT_LPCM;
              break;
          case AUD_FMT_OMX_ULAW:
              eFormatDecoder = AUD_FMT_LPCM;
              break;
          case AUD_FMT_OMX_ALAW:
              eFormatDecoder = AUD_FMT_LPCM;
              break;
#ifndef DISABLE_WMALSL_AMR              
          case AUD_FMT_WMA10LOSSLESS:
          case AUD_FMT_WMA10SPEECH:
              //copy info setting
              hriscdec.pDecInfo = pvArg;
              hriscdec.eFormat = eFormat;
              i4AudWMALSLTaskCreate(&hriscdec);
              eFormatDecoder = AUD_FMT_LPCM;
              break;
          case AUD_FMT_AMRNB:
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
            //TODO
            hriscdec.pDecInfo = pvArg;
            hriscdec.eFormat = eFormat;
            i4AudAMRTaskCreate(&hriscdec);
            eFormatDecoder = AUD_FMT_LPCM;
            break;
          case AUD_FMT_AMRWB:
            //TODO
            rAMRInfo.u2BitsPerSample = 16;
            rAMRInfo.u2Numch = 1;
            rAMRInfo.u4Sampersec = 16000;

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
            //TODO
            hriscdec.pDecInfo = pvArg;
            hriscdec.eFormat = eFormat;
            i4AudAWBTaskCreate(&hriscdec);
            eFormatDecoder = AUD_FMT_LPCM;
            break;
#endif            
      }
    //FOR_LG_2ND_ATTACK
    AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_MEMORY, eFormatDecoder);
#else
    AUD_DRV_FMT_INFO_T rFormat;

    rFormat.e_fmt = eFormat;
    rFormat.pv_info = NULL;

    AUD_SetDecType(FRTH_DEC, AUD_STREAM_FROM_MEMORY, &rFormat);

    // Set output opt to driver bit -> select 4th Decoder output driver
    _u4Dec4OutputOptVal |= (1<<11);
#endif

    return 0;
}


// Allocate FIFO according to format type.
INT32 i4AudOmxAllocateAudioFifo(AUD_OMX_HANDLE_T *pHandle)
{
    UINT32 u4FifoSize = 0;
    void *pFifoSA = 0;

#if 1 //DANIEL_HACK
    u4FifoSize = u4AfifoEA - u4AfifoSA;
#else
    switch (pHandle->eFormat)
    {
    case AUD_FMT_MP3:
        u4FifoSize = 64 * 1024;
        break;
    case AUD_FMT_AAC:
        u4FifoSize = 64 * 1024;
        break;
    default:
        ASSERT(0);
        break;
    }
#endif

    pFifoSA = (void*)u4AfifoSA; // x_alloc_aligned_afifo_mem(u4FifoSize, 256); //DANIEL_HACK
    ASSERT(NULL != pFifoSA);

    SYS_Printf("[AUD][OMX] Allocated FIFO SA = 0x%X, Size = %d bytes.\n", (UINT32)pFifoSA, u4FifoSize);
    if ((pHandle->eFormat == AUD_FMT_WMA10LOSSLESS) ||
         (pHandle->eFormat == AUD_FMT_WMA10SPEECH) ||
         (pHandle->eFormat == AUD_FMT_AMRNB) ||
         (pHandle->eFormat == AUD_FMT_AMRWB))
    {
        //RISC decoders use AFIFO3
        pHandle->pFifoPntr = (void*)VIRTUAL(u4GetAFIFOStart(AUD_DEC_THIRD));
        pHandle->u4FifoSize = (UINT32)(VIRTUAL(u4GetAFIFOEnd(AUD_DEC_THIRD)) - VIRTUAL(u4GetAFIFOStart(AUD_DEC_THIRD)));
    }
    else
    {
        pHandle->pFifoPntr  = pFifoSA;
        pHandle->u4FifoSize = u4FifoSize;
    }


    return 0;
}

static UINT8 u1ChannelVolume[10] = {100,100,100,100,100,100,100,100,100,100};
static UINT8 u1SpkCfg = 0;
extern void AUD_LineInCtrlEnable(UINT8 u1DecId, BOOL fgEnable);

INT32 i4AudOmxStartDecoding(AUD_OMX_HANDLE_T *pHandle, void *pFifoSA, UINT32 u4FifoSz)
{
#if 1 //DANIEL_HACK
    // set FIFO SA/EA
    //DANIEL_HACK? support arbitray pFifoSA, u4FifoSZ?
    AUD_DEC_MSG_T riscdec_msg; //ian wmalsl hack will modify later

    //20110622
    u2RaFrameByteIdx = 0;
    //20110622

    AUD_DSPCmdStop(AUD_DEC_MAIN);
    AUD_DSPCmdStop(AUD_DEC_AUX);
#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DSPCmdStop(AUD_DEC_THIRD);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    AUD_DSPCmdStop(AUD_DEC_4TH);
#endif
    AUD_DspIECConfig(AUD_IEC_CFG_PCM, TRUE);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);

    if (!u1BypassChVol) //CC_AUD_SUPPORT_OPENMAX
    {
        u1ChannelVolume[0] = AUD_DspGetChannelVolume(AUD_DEC_MAIN,AUD_CH_FRONT_LEFT);
        u1ChannelVolume[1] = AUD_DspGetChannelVolume(AUD_DEC_MAIN,AUD_CH_FRONT_RIGHT);
        u1ChannelVolume[2] = AUD_DspGetChannelVolume(AUD_DEC_MAIN,AUD_CH_REAR_LEFT);
        u1ChannelVolume[3] = AUD_DspGetChannelVolume(AUD_DEC_MAIN,AUD_CH_REAR_RIGHT);
        u1ChannelVolume[4] = AUD_DspGetChannelVolume(AUD_DEC_MAIN,AUD_CH_CENTER);
        u1ChannelVolume[5] = AUD_DspGetChannelVolume(AUD_DEC_MAIN,AUD_CH_SUB_WOOFER);
        u1ChannelVolume[6] = AUD_DspGetChannelVolume(AUD_DEC_MAIN,AUD_CH_BYPASS_LEFT);
        u1ChannelVolume[7] = AUD_DspGetChannelVolume(AUD_DEC_MAIN,AUD_CH_BYPASS_RIGHT);
        u1ChannelVolume[8] = AUD_DspGetChannelVolume(AUD_DEC_MAIN,AUD_CH_DMX_LEFT);
        u1ChannelVolume[9] = AUD_DspGetChannelVolume(AUD_DEC_MAIN,AUD_CH_DMX_RIGHT);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_FRONT_LEFT, 0);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_FRONT_RIGHT, 0);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_REAR_LEFT, 0);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_REAR_RIGHT, 0);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_CENTER, 0);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_SUB_WOOFER, 0);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_BYPASS_LEFT, 0);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_BYPASS_RIGHT, 0);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_DMX_LEFT, 0);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_DMX_RIGHT, 0);
    }
    else
    {
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_FRONT_LEFT, 100);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_FRONT_RIGHT, 100);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_REAR_LEFT, 100);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_REAR_RIGHT, 100);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_CENTER, 100);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_SUB_WOOFER, 100);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_BYPASS_LEFT, 100);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_BYPASS_RIGHT, 100);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_DMX_LEFT, 100);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_DMX_RIGHT, 100);
    }

    fgIsFirstChk = TRUE;
    fgEnableAdtsInsert = FALSE;

    //PTS
    //PTS_FREERUN
    nTimeStampCur = -1;
    //END
    x_memset(_prAudOmxPtsInfo, 0x00, sizeof(AUD_OMX_PTS_INFO_T) * MAX_PTS_INFO_SZ);
    u4LastChkPtsPtr = u4NextChkPtsPtr = (UINT32)(pHandle->pFifoPntr);
    u4LastOutputWP = u4OutputBufferSA;    
    u4LastPtsInfoIdx = 0;
    //End

    //2011/7/28 for flush cmd
    fgRecvFlushCmd = FALSE;
    //End

    u1FillQueueNo = 0;
    u1EmptyQueueNo = 0;
    u4TotalFillQueueNo = 0;
    u4TotalEmptyQueueNo = 0;
    u4TotalFillQueueDoneNo = 0;
    u4TotalEmptyQueueDoneNo = 0;
    u1AudDspState = 0;

    // set Wptr
    pHandle->pFifoWptr = pFifoSA;
    vAudHalSetBufWPtr(FRTH_DEC, DSP_AFIFO, u4AfifoSA);

    //Enable Upload Path
    AUD_DspSetUploadMode(4);
    AUD_DspUploadDataEnable();

    //Bypass Post-Proc
    AUD_DspBypassPostProcess(AUD_DEC_MAIN, 0x2000);

    //DDT line-mode
    AUD_DspDdtDmxRfMode(0,1,FALSE);

    //Speaker Config
    u1SpkCfg = AUD_DspGetSpeakerOutputConfig(0);
    AUD_DspSpeakerOutputConfig(0, 0);

#ifndef DISABLE_WMALSL_AMR
    //Start RISC Decoders
    riscdec_msg.eCmd = AUD_DEC_CMD_SET_STATE;
    riscdec_msg.eStateTo = AUD_DEC_STATE_PLAY;
    if ((pHandle->eFormat == AUD_FMT_WMA10LOSSLESS) ||
         (pHandle->eFormat == AUD_FMT_WMA10SPEECH))
    {
        i4AudWMALSLTaskSendCommand(&hriscdec, &riscdec_msg);
    }
    else if (pHandle->eFormat == AUD_FMT_AMRNB)
    {
        i4AudAMRTaskSendCommand(&hriscdec, &riscdec_msg);
    }
    else if (pHandle->eFormat == AUD_FMT_AMRWB)
    {
        i4AudAWBTaskSendCommand(&hriscdec, &riscdec_msg);
    }
#endif    

    if (pHandle->eFormat == AUD_FMT_LPCM)
    {
        Printf("AUD_FMT_LPCM: delay play wait 5 bytes header to do config\n");
    }
    else
    {
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
        u1AudDspState = 1;
    }

#else
    // set FIFO SA/EA
    vAudHalSetBufStartAddr(FRTH_DEC, DSP_AFIFO, (UINT32)pFifoSA);
    vAudHalSetBufEndAddr(FRTH_DEC, DSP_AFIFO, (UINT32)pFifoSA + u4FifoSz);

    // set Wptr
    pHandle->pFifoWptr = pFifoSA;
    vAudHalSetBufWPtr(FRTH_DEC, DSP_AFIFO, (UINT32)pFifoSA);

    // initialize DRAM info for later used for identifying "decoder ready" state
    vWriteDspWORD(ADDR_D2RC_DECODING4_OUTPUT_PCMBUF_ADDRESS,   0xFFFF);
    vWriteDspWORD(ADDR_D2RC_DECODING4_OUTPUT_PCMBUF_SIZE,      0x0000);
    vWriteDspWORD(ADDR_RC2D_DECODING4_OUTPUT_PCMBUF_READ_PTR,  0xFFFF);
    vWriteDspWORD(ADDR_D2RC_DECODING4_OUTPUT_PCMBUF_WRITE_PTR, 0xFFFF);

    // issue play command (synchronous command)
    AUD_DSPCmdPlay(FRTH_DEC);
#endif

    return 0;
}

INT32 i4AudOmxStopDecoding(AUD_FMT_T eFormat)
{
#if 1 //DANIEL_HACK
    AUD_DEC_MSG_T riscdec_msg; //ian wmalsl hack will modify later

    //Disable Upload Path
    AUD_DspSetUploadMode(0);
    AUD_DspUploadDataEnable();

    AUD_DSPCmdStop(AUD_DEC_MAIN);
    u1AudDspState = 0;

    skip_create_feed_stream = FALSE;
   
    if (!u1BypassChVol) //CC_AUD_SUPPORT_OPENMAX
    {
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_FRONT_LEFT, u1ChannelVolume[0]);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_FRONT_RIGHT, u1ChannelVolume[1]);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_REAR_LEFT, u1ChannelVolume[2]);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_REAR_RIGHT, u1ChannelVolume[3]);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_CENTER, u1ChannelVolume[4]);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_SUB_WOOFER, u1ChannelVolume[5]);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_BYPASS_LEFT, u1ChannelVolume[6]);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_BYPASS_RIGHT, u1ChannelVolume[7]);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_DMX_LEFT, u1ChannelVolume[8]);
        AUD_DspChannelVolume(AUD_DEC_MAIN,AUD_CH_DMX_RIGHT, u1ChannelVolume[9]);
    }

    //DDT RF-mode
    AUD_DspDdtDmxRfMode(1,1,FALSE);

    //Speaker Config
    AUD_DspSpeakerOutputConfig(0, u1SpkCfg);    

#ifndef DISABLE_WMALSL_AMR
    if ((eFormat == AUD_FMT_WMA10LOSSLESS) ||
         (eFormat == AUD_FMT_WMA10SPEECH))
    {
        riscdec_msg.eCmd = AUD_DEC_CMD_SET_STATE;
        riscdec_msg.eStateTo = AUD_DEC_STATE_STOP;
        i4AudWMALSLTaskSendCommand(&hriscdec, &riscdec_msg);
    }
    else if (eFormat == AUD_FMT_AMRNB)
    {
        riscdec_msg.eCmd = AUD_DEC_CMD_TERMINATE;
        riscdec_msg.eStateTo = AUD_DEC_STATE_STOP;
        i4AudAMRTaskSendCommand(&hriscdec, &riscdec_msg);
    }
    else if (eFormat == AUD_FMT_AMRWB)
    {
        riscdec_msg.eCmd = AUD_DEC_CMD_TERMINATE;
        riscdec_msg.eStateTo = AUD_DEC_STATE_STOP;
        i4AudAWBTaskSendCommand(&hriscdec, &riscdec_msg);
    }
#endif

#else
    // issue stop command (synchronous command)
    AUD_DSPCmdStop(FRTH_DEC);
#endif

    return 0;
}

void vAudOmxRestartDecoding(AUD_OMX_HANDLE_T *pHandle)
{
    u2RaFrameByteIdx = 0;

    //PTS
    //PTS_FREERUN
    nTimeStampCur = -1;
    //END
    x_memset(_prAudOmxPtsInfo, 0x00, sizeof(AUD_OMX_PTS_INFO_T) * MAX_PTS_INFO_SZ);
    u4LastChkPtsPtr = u4NextChkPtsPtr = (UINT32)(pHandle->pFifoPntr);
    u4LastOutputWP = u4OutputBufferSA;
    u4LastPtsInfoIdx = 0;
    //END

    fgRecvFlushCmd = FALSE;

    pHandle->pFifoWptr = pHandle->pFifoPntr;
    vAudHalSetBufWPtr(FRTH_DEC, DSP_AFIFO, u4AfifoSA);

    AUD_DSPCmdPlay(AUD_DEC_MAIN);
    u1AudDspState = 1;
}

//DANIEL_HACK
#define SFREQ_16K                   (0x02)
#define SFREQ_32K                   (0x03)
#define SFREQ_8K                    (0x04)
#define SFREQ_11K                   (0x05)
#define SFREQ_12K                   (0x06)
#define SFREQ_22K                   (0x07)
#define SFREQ_44K                   (0x08)
#define SFREQ_88K                   (0x09)
#define SFREQ_176K                  (0x0A)
#define SFREQ_24K                   (0x0C)
#define SFREQ_48K                   (0x0D)
#define SFREQ_96K                   (0x0E)
#define SFREQ_192K                  (0x0F)
#define SFREQ_52K                   (0x10)
#define SFREQ_62K                   (0x11)
#define SFREQ_64K                   (0x12)
//DANIEL_HACK

static UINT32 u4ConvertSamplingRate(UINT32 u4FreqIdx)
{
    UINT32 u4SampleRate = 48000;

    switch (u4FreqIdx)
    {
    case SFREQ_8K:
        u4SampleRate = 8000;
        break;
    case SFREQ_16K:
        u4SampleRate = 16000;
        break;
    case SFREQ_32K:
        u4SampleRate = 32000;
        break;
    case SFREQ_64K:
        u4SampleRate = 64000;
        break;
    case SFREQ_11K:
        u4SampleRate = 11025;
        break;
    case SFREQ_22K:
        u4SampleRate = 22050;
        break;
    case SFREQ_44K:
        u4SampleRate = 44100;
        break;
    case SFREQ_88K:
        u4SampleRate = 88200;
        break;
    case SFREQ_176K:
        u4SampleRate = 176400;
        break;
    case SFREQ_12K:
        u4SampleRate = 12000;
        break;
    case SFREQ_24K:
        u4SampleRate = 24000;
        break;
    case SFREQ_48K:
        u4SampleRate = 48000;
        break;
    case SFREQ_96K:
        u4SampleRate = 96000;
        break;
    case SFREQ_192K:
        u4SampleRate = 192000;
        break;
    default:
        break;
    }

    return u4SampleRate;
}

INT32 i4AudOmxGetParameterMP3(OMX_AUDIO_PARAM_MP3TYPE *prMp3Info)
{
  #if 1 //DANIEL_HACK
    UINT32 u4FreqIdx;
    UINT32 u4DspInputChCfg;
    UINT32 u4Bitrate;

    UINT32 u4Channels;
    OMX_AUDIO_CHANNELMODETYPE eChannelMode;

    vDspGetParameterMP3(&u4FreqIdx, &u4DspInputChCfg, &u4Bitrate);

    switch (u4DspInputChCfg)
    {
    case 1:
        u4Channels = 1;
        eChannelMode = OMX_AUDIO_ChannelModeMono;
        break;
    case 0:
        u4Channels = 2;
        eChannelMode = OMX_AUDIO_ChannelModeDual;
        break;
    case 2:
    default:
        u4Channels = 2;
        eChannelMode = OMX_AUDIO_ChannelModeStereo;
        break;
    }
  #else
    // get audio information
    UINT32 u4FreqIdx = wReadDspWORD(ADDR_D2RC_DECODING4_SAMPLING_RATE);
    UINT32 u4DspInputChCfg = wReadDspWORD(ADDR_D2RC_DECODING4_INPUT_CH_CFG) & 0x03F;
    UINT32 u4Bitrate = dReadDspCommDram(ADDR_D2RC_DECODING4_BIT_RATE);

    UINT32 u4Channels;
    OMX_AUDIO_CHANNELMODETYPE eChannelMode;

    switch (u4DspInputChCfg)
    {
    case DSP_CH_CFG_MONO:
        u4Channels = 1;
        eChannelMode = OMX_AUDIO_ChannelModeMono;
        break;
    case DSP_CH_CFG_DUAL_MONO:
        u4Channels = 2;
        eChannelMode = OMX_AUDIO_ChannelModeDual;
        break;
    case DSP_CH_CFG_STEREO:
    default:
        u4Channels = 2;
        eChannelMode = OMX_AUDIO_ChannelModeStereo;
        break;
    }
  #endif

    prMp3Info->nChannels    = u4Channels;
    prMp3Info->eChannelMode = eChannelMode;
    prMp3Info->nSampleRate  = u4ConvertSamplingRate(u4FreqIdx);
    prMp3Info->nBitRate     = u4Bitrate;

    return 0;
}

#endif

