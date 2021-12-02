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
#include "vdec_omx_debug.h"

#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_assert.h"


#include "vdec_mod.h"
#include "vdec_drvif.h"


static VDO_OMX_BUFFER_T _prVDOOmxInputBuf[VDO_INPUT_BUF_MAX];
static VDO_OMX_BUFFER_T _prVDOOmxOutputBuf[VDO_OUTPUT_BUF_MAX];

void vVDOOmxBufferInit(void)
{
    UINT32 u4Index;

    for (u4Index = 0; u4Index < VDO_INPUT_BUF_MAX; u4Index ++)
    {

        _prVDOOmxInputBuf[u4Index].pOwner     = NULL;
        _prVDOOmxInputBuf[u4Index].hBuffer    = NULL_HANDLE;
        _prVDOOmxInputBuf[u4Index].pBufSA     = NULL;
        _prVDOOmxInputBuf[u4Index].u4BufSize  = 0;
        _prVDOOmxInputBuf[u4Index].hHeader    = NULL_HANDLE;
        _prVDOOmxInputBuf[u4Index].u4Reserved = 0x5A5A5A5A;
    }

    for (u4Index = 0; u4Index < VDO_OUTPUT_BUF_MAX; u4Index ++)
    {

        _prVDOOmxOutputBuf[u4Index].pOwner     = NULL;
        _prVDOOmxOutputBuf[u4Index].hBuffer    = NULL_HANDLE;
        _prVDOOmxOutputBuf[u4Index].pBufSA     = NULL;
        _prVDOOmxOutputBuf[u4Index].u4BufSize  = 0;
        _prVDOOmxOutputBuf[u4Index].hHeader    = NULL_HANDLE;
        _prVDOOmxOutputBuf[u4Index].u4Reserved = 0xA5A5A5A5;
    }
}


void vVDOOmxBufferUninit(void)
{
    UINT32 u4Index;

    for (u4Index = 0; u4Index < VDO_INPUT_BUF_MAX; u4Index ++)
    {
        if (NULL_HANDLE != _prVDOOmxInputBuf[u4Index].hHeader)
        {
        x_kmem_destroy_handle(_prVDOOmxInputBuf[u4Index].hHeader);
        }
    }

    for (u4Index = 0; u4Index < VDO_OUTPUT_BUF_MAX; u4Index ++)
    {
        if (NULL_HANDLE != _prVDOOmxOutputBuf[u4Index].hHeader)
        {
        x_kmem_destroy_handle(_prVDOOmxOutputBuf[u4Index].hHeader);
        }
    }
}


// OpenMAX client may allocate output buffer more than one time
INT32 i4VDOOmxAllocOutputBuf(VDO_OMX_HANDLE_T *pHandle, BOOL fgUseBuf, UINT32 u4BufSA, UINT32 u4Size, UINT32 u4Port, VDO_OMX_BUFFER_T **ppBufInfo)
{
    UINT32 u4Index;
    INT32 i4Ret = 0;

    HANDLE_T hHeader = NULL_HANDLE;
    HANDLE_T hMemHandle = NULL_HANDLE;
    VDO_OMX_BUFFER_T *pAvailBuf = NULL;

        //VOMX_Printf("[VDO][OMX] alloc Video output memory.\n");
    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    // STEP 0: get a free output buffer info node
    for (u4Index = 0; u4Index < VDO_OUTPUT_BUF_MAX; u4Index ++)
    {
        if (NULL == _prVDOOmxOutputBuf[u4Index].pOwner)
        {
            pAvailBuf = &(_prVDOOmxOutputBuf[u4Index]);
            break;
        }
    }
            //VOMX_Printf("[VDO][OMX] i4VDOOmxAllocOutputBuf : u4Index = 0x%x, pHandle = 0x%x, _prVDOOmxOutputBuf[u4Index].pOwner = 0x%x.\n", (UINT32)u4Index, (UINT32)pHandle,(UINT32)_prVDOOmxOutputBuf[u4Index].pOwner);

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

    hHeader = x_kmem_create_handle(&(pAvailBuf->rHeader), sizeof(OMX_BUFFERHEADERTYPE));

    if (NULL_HANDLE == hHeader)
    {
        goto lblErrorReturn;
    }
    else
    {
        pAvailBuf->hHeader = hHeader;
    }

    // STEP 1: allocate input buffer from FBM
    i4Ret = _i4VConv_AllocFbmBuf(_prVDOOmxHandle[pHandle->h_handle].h_handle,
                fgUseBuf, u4Size, 1, &u4BufSA);
            //VOMX_Printf("[VDO][OMX] i4VDOOmxAllocOutputBuf : u4Index = 0x%x, u4BufSA = 0x%x, _prVDOOmxInputBuf[u4Index].pOwner = 0x%x.\n", (UINT32)u4Index, u4BufSA,(UINT32)_prVDOOmxInputBuf[u4Index].pOwner);

    if (0 == u4BufSA)
    {
        VOMX_Printf("[VDO][OMX] alloc Video output memory failed, %d bytes.\n", u4Size);
        goto lblErrorReturn;
    }
    else
    {
        pAvailBuf->pBufSA    = (VOID*)u4BufSA;
        pAvailBuf->u4BufSize = u4Size;
    }

    hMemHandle = x_kmem_create_handle(pAvailBuf->pBufSA, u4Size);

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

    pAvailBuf->rHeader.pBuffer          = pAvailBuf->pBufSA;
    pAvailBuf->rHeader.nAllocLen        = u4Size;
    pAvailBuf->rHeader.nFilledLen       = 0;
    pAvailBuf->rHeader.nOffset          = 0;
    pAvailBuf->rHeader.nOutputPortIndex = u4Port;

    // release semaphore
    i4VDOOmxTaskUnlockSem();

    *ppBufInfo = pAvailBuf;
    return VDecOMX_OK;

    // error handling
lblErrorReturn:
    // release semaphore
    i4VDOOmxTaskUnlockSem();

    // resource release will be done on freeing component handle, not here
    *ppBufInfo = NULL;
    return VDecOMX_ERR;
}


INT32 i4VDOOmxFreeOutputBuf(OMX_HANDLETYPE h_Handle, HANDLE_T hHeader)
{
    UINT32 u4Index;
    INT32 i4Ret = 0;
    VDO_OMX_BUFFER_T *pBufNode = NULL;
    VDO_OMX_HANDLE_T *Omx_Handle = h_Handle;

    if ((NULL == h_Handle) || (NULL_HANDLE == hHeader))
    {
        return VDecOMX_OK;
    }

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    for (u4Index = 0; u4Index < VDO_OUTPUT_BUF_MAX; u4Index ++)
    {
        if (hHeader == _prVDOOmxOutputBuf[u4Index].hHeader)
        {
            pBufNode = &(_prVDOOmxOutputBuf[u4Index]);
            break;
        }
    }

    if (NULL != pBufNode)
    {
        if ((pBufNode->pOwner == h_Handle) && (NULL != pBufNode->pBufSA))
        {
            // free memory
            i4Ret = _i4VConv_FreeFbmBuf(_prVDOOmxHandle[Omx_Handle->h_handle].h_handle, (UINT32)pBufNode->pBufSA);

            // destory the buffer handle
            x_kmem_destroy_handle(pBufNode->hBuffer);
            // destory the header handle
            x_kmem_destroy_handle(pBufNode->hHeader);

            // clear info
            pBufNode->pOwner      = NULL;
            pBufNode->hBuffer     = NULL_HANDLE;
            pBufNode->pBufSA      = NULL;
            pBufNode->u4BufSize   = 0;
            pBufNode->hHeader     = NULL_HANDLE;
        }
    }

    // release semaphore
    i4VDOOmxTaskUnlockSem();

    return VDecOMX_OK;
}


INT32 i4VDOOmxQueueFillBuf(VDO_OMX_HANDLE_T *pHandle, HANDLE_T hHeader)
{
    INT32 i4Ret = VDecOMX_ERR;
    UINT32 u4Index = VDO_OUTPUT_BUF_MAX;
    VDO_OMX_BUFFER_T *pBufNode = NULL;

    if ((NULL == pHandle) || (NULL_HANDLE == hHeader))
    {
        return VDecOMX_OK;
    }

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    for (u4Index = 0; u4Index < VDO_OUTPUT_BUF_MAX; u4Index ++)
    {
        if (hHeader == _prVDOOmxOutputBuf[u4Index].hHeader)
        {
            pBufNode = &(_prVDOOmxOutputBuf[u4Index]);
            break;
        }
    }

    if (NULL != pBufNode)
    {
        if (pBufNode->pOwner == pHandle)
        {
            // add to queue
            if (pHandle->pFillQ[pHandle->u4FillWritePos] != NULL)
            {
                LOG(1, "[VDO][OMX] WARNING: fill buffer is not empty.\n");
                pHandle->fgFilling = FALSE;
            }
            else
            {
                i4Ret = VDecOMX_OK;
                pHandle->fgFilling = TRUE;
                u4Index = pHandle->u4FillWritePos;
                pHandle->pFillQ[u4Index] = pBufNode;
                pHandle->u4FillWritePos++;
                if(pHandle->u4FillWritePos >= VDO_OUTPUT_BUF_MAX)
                {
                    pHandle->u4FillWritePos = 0;
                }
                // initialize buffer filled len
                // (The OMX_FillThisBuffer macro will send an empty buffer to an output port of a component.)
                pBufNode->u4FilledLen   = 0;
                pBufNode->u4Offset      = 0;
                pBufNode->nTimeStamp    = 0;
                pBufNode->rHeader.nTimeStamp    = 0;
                pBufNode->rHeader.nFlags        = 0;
                LOG(6, "%s(%d), u4FillWritePos(%d) \n", __FUNCTION__, __LINE__, pHandle->u4FillWritePos);
            }

        }
    }
    if (pHandle->fgFilling)
    {
        i4VDOOmxDoFillBuf(pHandle, u4Index);
    }

    // release semaphore
    i4VDOOmxTaskUnlockSem();

    return i4Ret;
}


INT32 i4VDOOmxDoFillBuf(VDO_OMX_HANDLE_T *pHandle, UINT32 u4Index)
{
    INT32 i4Ret = VDecOMX_OK;
    VDO_OMX_BUFFER_T *pToFillBuf = NULL;
    VCONV_FILL_FBM_BUF_PARAM_T stFillBufPara;

    if((!pHandle) || (u4Index >= VDO_OUTPUT_BUF_MAX))
    {
        ASSERT(0);
        return VDecOMX_ERR;
    }
    pToFillBuf = pHandle->pFillQ[u4Index];
    if(!pToFillBuf)
    {
        // error handling
        pHandle->fgFilling = FALSE;
        ASSERT(0);
        return VDecOMX_ERR;
    }

    //VOMX_Printf("[VDO][i4VDOOmxDoFillBuf] BufSA = 0x%x\n", pToFillBuf->pBufSA);
    stFillBufPara.u4FbmBufAddr= (UINT32)pToFillBuf->pBufSA;
   _i4VConv_SendCmd(pHandle->h_handle, VCONV_CMD_FILLFBMBUF, &stFillBufPara);
    
    return i4Ret;
}


INT32 i4VDOOmxForceAllBufFillDone(VDO_OMX_HANDLE_T *pHandle)
{
    INT32 i4Ret = VDecOMX_ERR;
    UINT32 u4Index;
    VDO_OMX_BUFFER_T *pToFillBuf = NULL;

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    for (u4Index = 0; u4Index < VDO_OUTPUT_BUF_MAX; u4Index ++)
    {
        pToFillBuf = pHandle->pFillQ[u4Index];

        if (NULL != pToFillBuf)
        {
            pToFillBuf->rHeader.nFlags = 0;
            pToFillBuf->rHeader.nTimeStamp = 0;
            i4VDOOmxCompNotifyFillDone(pHandle, pToFillBuf);

            // clear
            pHandle->pFillQ[u4Index] = NULL;
        }
    }

    pHandle->u4FillCounter = 0;
    pHandle->u4FillWritePos = 0;
    pHandle->u4FillReadPos = 0;
    pHandle->fgFilling = FALSE;

    // release semaphore
    i4VDOOmxTaskUnlockSem();

    return i4Ret;
}


INT32 i4VDOOmxAllocInputBuf(VDO_OMX_HANDLE_T *pHandle, UINT32 u4Size, UINT32 u4Port, VDO_OMX_BUFFER_T **ppBufInfo)
{
    // allocate input buffer from VFIFO
    // allocate continous buffer from VFIFO for decoder

    // to maintain the continuity of allocated buffers, we always free previously allocated buffers
    // and allocate a larger one containing both previous and current buffer

    UINT32 u4Index;
    //INT32 i4Ret = 0;

    void *pBufSA  = NULL;

    HANDLE_T hHeader = NULL_HANDLE;
    HANDLE_T hMemHandle = NULL_HANDLE;
    VDO_OMX_BUFFER_T *pAvailBuf = NULL;

    VDO_OMX_HANDLE_T *prVDOOmxHandle = &_prVDOOmxHandle[pHandle->h_handle];

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    // STEP 0: get a free input buffer info node
    for (u4Index = 0; u4Index < VDO_INPUT_BUF_MAX; u4Index ++)
    {
        if (NULL == _prVDOOmxInputBuf[u4Index].pOwner)
        {
            pAvailBuf = &(_prVDOOmxInputBuf[u4Index]);
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
    //VOMX_Printf("[VDO][OMX] i4VDOOmxAllocInputBuf : u4Index = 0x%x, pAvailBuf->pOwner = 0x%x, _prVDOOmxInputBuf[u4Index].pOwner = 0x%x.\n", (UINT32)u4Index, (UINT32)pAvailBuf->pOwner,(UINT32)_prVDOOmxInputBuf[u4Index].pOwner);

    hHeader = x_kmem_create_handle(&(pAvailBuf->rHeader), sizeof(OMX_BUFFERHEADERTYPE));

    if (NULL_HANDLE == hHeader)
    {
        goto lblErrorReturn;
    }
    else
    {
        pAvailBuf->hHeader = hHeader;
    }


    // allocate input buffer from ESM
    #if DTV_VOMX
    {
        ENUM_VDEC_FMT_T eFmt = VDEC_FMT_MAX;
        if(prVDOOmxHandle->eCodecType == VC_H264)
        {
            eFmt = VDEC_FMT_H264;
        }
        else if(prVDOOmxHandle->eCodecType == VC_VC1)
        {
            _VPUSH_SetPushType(prVDOOmxHandle->pVdecPush, VDEC_PUSH_FMT_VC1);
            eFmt = VDEC_FMT_WMV;
        }
        else if(prVDOOmxHandle->eCodecType == VC_WMV1)
        {
            _VPUSH_SetPushType(prVDOOmxHandle->pVdecPush, VDEC_PUSH_FMT_WMV7);
            eFmt = VDEC_FMT_WMV;
        }
        else if(prVDOOmxHandle->eCodecType == VC_WMV2)
        {
            _VPUSH_SetPushType(prVDOOmxHandle->pVdecPush, VDEC_PUSH_FMT_WMV8);
            eFmt = VDEC_FMT_WMV;
        }
        else if(prVDOOmxHandle->eCodecType == VC_WMV3)
        {
            _VPUSH_SetPushType(prVDOOmxHandle->pVdecPush, VDEC_PUSH_FMT_WMV9);
            eFmt = VDEC_FMT_WMV;
        }        
        else if(prVDOOmxHandle->eCodecType == VC_MPEG2)
        {
            eFmt = VDEC_FMT_MPV;
        }
        else if(prVDOOmxHandle->eCodecType == VC_MPEG4)
        {
            eFmt = VDEC_FMT_MP4;
        }
#ifdef VDEC_IS_POST_MT5368
        else if(prVDOOmxHandle->eCodecType == VC_VP6)
        {
            eFmt = VDEC_FMT_VP6;
        }
        else if(prVDOOmxHandle->eCodecType == VC_VP8)
        {
            eFmt = VDEC_FMT_VP8;
        }
#endif
        else if(prVDOOmxHandle->eCodecType == VC_MJPG)
        {
            eFmt = VDEC_FMT_MJPEG;
        }
        else if(prVDOOmxHandle->eCodecType == VC_AVS)
        {
            eFmt = VDEC_FMT_AVS;
        }
        else if(prVDOOmxHandle->eCodecType == VC_RV30)
        {
            eFmt = VDEC_FMT_RV;
        }
        else if(prVDOOmxHandle->eCodecType == VC_RV40)
        {
            eFmt = VDEC_FMT_RV;
        }
        else if(prVDOOmxHandle->eCodecType == VC_JPG)
        {
            eFmt = VDEC_FMT_JPG;
        } 
        else if(prVDOOmxHandle->eCodecType == VC_PNG)
        {
            eFmt = VDEC_FMT_PNG;
        }         
        else
        {
            ASSERT(0);
        }
        if(!_VPUSH_AllocVFifo(prVDOOmxHandle->pVdecPush,
            (UINT32*)&pBufSA, u4Size,  eFmt))
        {
            VOMX_Printf("[VDO][OMX] alloc VFIFO memory failed, %x bytes.\n", u4Size);
            goto lblErrorReturn;
        }
    }
    #else
    i4Ret = _i4Vdec_OMX_AllocVFifo(prVDOOmxHandle->h_handle, &pBufSA, u4Size,  prVDOOmxHandle->eCodecType);
    #endif


    if (NULL == pBufSA)
    {
        VOMX_Printf("[VDO][OMX] alloc VFIFO memory failed, %x bytes.\n", u4Size);
        goto lblErrorReturn;
    }
    else
    {
        pAvailBuf->pBufSA    = pBufSA;
        pAvailBuf->u4BufSize = u4Size;
    }
            //VOMX_Printf("[VDO][OMX] i4VDOOmxAllocInputBuf : pBufSA = 0x%x\n", (UINT32)pBufSA);
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
    i4VDOOmxTaskUnlockSem();

    *ppBufInfo = pAvailBuf;
    return VDecOMX_OK;

    // error handling
lblErrorReturn:
    // release semaphore
    i4VDOOmxTaskUnlockSem();

    // resource release will be done on freeing component handle, not here
    *ppBufInfo = NULL;
    return VDecOMX_ERR;
}


INT32 i4VDOOmxFreeInputBuf(OMX_HANDLETYPE pHandle, HANDLE_T hHeader)
{
    UINT32 u4Index;
   // BOOL fgAllFreed = TRUE;
    VDO_OMX_BUFFER_T *pBufNode = NULL;
    VDO_OMX_HANDLE_T *Omx_Handle = pHandle;

    if ((NULL == pHandle) || (NULL_HANDLE == hHeader))
    {
        return VDecOMX_OK;
    }

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    for (u4Index = 0; u4Index < VDO_INPUT_BUF_MAX; u4Index ++)
    {
        if (hHeader == _prVDOOmxInputBuf[u4Index].hHeader)
        {
            pBufNode = &(_prVDOOmxInputBuf[u4Index]);
            break;
        }
    }

    if (NULL != pBufNode)
    {
        if ((pBufNode->pOwner == pHandle) && (NULL != pBufNode->pBufSA))
        {
            // free memory
            #if DTV_VOMX
            _VPUSH_FreeVFifo(_prVDOOmxHandle[Omx_Handle->h_handle].pVdecPush, (UINT32)pBufNode->pBufSA);
            #else
            i4Ret = _i4Vdec_OMX_FreeVFifo(_prVDOOmxHandle[Omx_Handle->h_handle].h_handle);
            #endif

            // destory the buffer handle
            x_kmem_destroy_handle(pBufNode->hBuffer);
            // destory the header handle
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
    i4VDOOmxTaskUnlockSem();

    return VDecOMX_OK;
}


INT32 i4VDOOmxQueueEmptyBuf(VDO_OMX_HANDLE_T *pHandle, HANDLE_T hHeader)
{
    INT32 i4Ret = VDecOMX_ERR;
    UINT32 u4Index = VDO_INPUT_BUF_MAX;
    VDO_OMX_BUFFER_T *pBufNode = NULL;

    if ((NULL == pHandle) || (NULL_HANDLE == hHeader))
    {
        return VDecOMX_OK;
    }
    //VOMX_Printf("[VDO][OMX] i4VDOOmxQueueEmptyBuf pHandle =0x%x.\n", pHandle);

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    for (u4Index = 0; u4Index < VDO_INPUT_BUF_MAX; u4Index ++)
    {
        if (hHeader == _prVDOOmxInputBuf[u4Index].hHeader)
        {
            pBufNode = &(_prVDOOmxInputBuf[u4Index]);
            break;
        }
    }
    //VOMX_Printf("[VDO][OMX] i4VDOOmxQueueEmptyBuf : u4Index = 0x%x, hHeader = 0x%x, _prVDOOmxInputBuf[u4Index].pOwner = 0x%x.\n", (UINT32)u4Index, (UINT32)hHeader,(UINT32)_prVDOOmxInputBuf[u4Index].pOwner);

    if (NULL != pBufNode)
    {
        if (pBufNode->pOwner == pHandle)
        {
            // fill current empty parameters (copy from client filled memory)
            pBufNode->u4FilledLen = pBufNode->rHeader.nFilledLen;
            pBufNode->u4Offset    = pBufNode->rHeader.nOffset;
            pBufNode->nTimeStamp    = pBufNode->rHeader.nTimeStamp;

            // add to queue
            if (pHandle->pEmptyQ[pHandle->u4EmptyWritePos] != NULL)
            {
                LOG(1, "[VDO][OMX] WARNING: empty buffer is not empty.\n");
                pHandle->fgEmptying = FALSE;
            }
            else
            {
                i4Ret = VDecOMX_OK;
                pHandle->fgEmptying = TRUE;
                u4Index = pHandle->u4EmptyWritePos;
                pHandle->pEmptyQ[u4Index] = pBufNode;
                if (OMX_BUFFERFLAG_EOS & pBufNode->rHeader.nFlags)
            	{
                    LOG(3, "%s(L:%d) OMX_BUFFERFLAG_EOS\n", __FUNCTION__, __LINE__);
                	pBufNode->pOwner->fgGotEOS = TRUE;

                	// clear EOS flag
                	pBufNode->rHeader.nFlags &= ~OMX_BUFFERFLAG_EOS;
            	}
                else
            	{
                	//VOMX_Printf("[VDO][OMX] EOS flag received, 0.\n");
                	pBufNode->pOwner->fgGotEOS = FALSE;
            	}
                pHandle->u4EmptyCounter++;
                pHandle->u4EmptySize += pBufNode->u4FilledLen;
                pHandle->u4EmptyWritePos++;
                if(pHandle->u4EmptyWritePos >= VDO_INPUT_BUF_MAX)
                {
                    pHandle->u4EmptyWritePos = 0;
                }                        
            }
        }
    }
    if (pHandle->fgEmptying)
    {
        i4VDOOmxDoEmptyBuf(pHandle, u4Index);
    }
    // release semaphore
    i4VDOOmxTaskUnlockSem();

    return i4Ret;
}


INT32 i4VDOOmxDoEmptyBuf(VDO_OMX_HANDLE_T *pHandle, UINT32 u4Index)
{
    INT32 i4Ret = VDecOMX_OK;
    VDO_OMX_BUFFER_T *pToEmptyBuf = NULL;
    VDEC_BYTES_INTO_T rByteInfo;
    UINT64 u8Tmp;
    UINT64 u8Remainder;

    // always left aligned

    if((!pHandle) || (u4Index >= VDO_INPUT_BUF_MAX))
    {
        ASSERT(0);
        return VDecOMX_ERR;
    }
    pToEmptyBuf = pHandle->pEmptyQ[u4Index];
    if(!pToEmptyBuf)
    {
        // error handling
        pHandle->fgEmptying = FALSE;
        ASSERT(0);
        return VDecOMX_ERR;
    }

    //VOMX_Printf("[VDO][OMX] i4VDOOmxDoEmptyBuf pHandle = 0x%x.\n", pHandle);

    //VOMX_Printf("[VDO][OMX] pu4VfifoBufAddr = 0x%x, u4Size = 0x%x.\n", pToEmptyBuf->pBufSA, pToEmptyBuf->u4BufSize);

    // multi-instance protection, take semaphore
    x_memset(&rByteInfo, 0, sizeof(VDEC_BYTES_INTO_T));
    rByteInfo.u4BytesAddr = (UINT32)pToEmptyBuf->pBufSA + pToEmptyBuf->u4Offset;
    rByteInfo.u4BytesSize = pToEmptyBuf->u4FilledLen;
    rByteInfo.fgEos = pHandle->fgGotEOS;
    if(pHandle->fgGotEOS)
    {
	    LOG(5, "[VDO][OMX] _VPUSH_PutData EOS flag received, 1.\n");
    }

    if (pHandle->u8TimeStampBase == VDO_OMX_INV_PTS)
    {
	    pHandle->u8TimeStampBase = pToEmptyBuf->nTimeStamp;
	    LOG(0, "[VDO][OMX] BASE u8Pts 0x%llx\n", pHandle->u8TimeStampBase);
    }

    if (pToEmptyBuf->nTimeStamp >= pHandle->u8TimeStampBase)
    {
	    u8Tmp = pToEmptyBuf->nTimeStamp - pHandle->u8TimeStampBase;
    }
    else
    {
	    u8Tmp = pHandle->u8TimeStampBase - pToEmptyBuf->nTimeStamp;
    }

    rByteInfo.u4BytesPTS = (UINT32)u8Div6432((u8Tmp*9), 100, &u8Remainder);

    if (pToEmptyBuf->nTimeStamp < pHandle->u8TimeStampBase)
    {
	    rByteInfo.u4BytesPTS |= ((UINT32)0x80000000);
	    LOG(5, "[VDO][OMX] *** NEGATIVE INPUT ***\n");
    }

    if (pToEmptyBuf->nTimeStamp == 0x4189374bc6a7ef)
    {
	    rByteInfo.u4BytesPTS = 0;
    }

    LOG(5, "[VDO][OMX] INPUT u8Pts 0x%llx, u4Pts 0x%x\n", pToEmptyBuf->nTimeStamp, rByteInfo.u4BytesPTS);

    UNUSED(_VPUSH_PutData(pHandle->pVdecPush, &rByteInfo));

    return i4Ret;
}

INT32 i4VDOOmxForceAllBufEmptyDone(VDO_OMX_HANDLE_T *pHandle)
{
    INT32 i4Ret = VDecOMX_ERR;
    UINT32 u4Index;
    VDO_OMX_BUFFER_T *pToEmptyBuf = NULL;

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    for (u4Index = 0; u4Index < VDO_INPUT_BUF_MAX; u4Index ++)
    {
        pToEmptyBuf = pHandle->pEmptyQ[u4Index];

        if (NULL != pToEmptyBuf)
        {
            pToEmptyBuf->rHeader.nFlags = 0;
            pToEmptyBuf->rHeader.nTimeStamp = 0;
            i4VDOOmxCompNotifyEmptyDone(pHandle, pToEmptyBuf);

            // clear
            pHandle->pEmptyQ[u4Index] = NULL;
        }
    }

    pHandle->u4EmptySize = 0;
    pHandle->u4EmptyCounter = 0;
    pHandle->u4EmptyWritePos = 0;
    pHandle->u4EmptyReadPos = 0;
    pHandle->fgEmptying = FALSE;

    // release semaphore
    i4VDOOmxTaskUnlockSem();

    return i4Ret;
}


BOOL fgVDOOmxAllInputBufferFreed(VDO_OMX_HANDLE_T *pHandle)
{
    UINT32 u4Index;
    BOOL fgAllFreed = TRUE;

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    for (u4Index = 0; u4Index < VDO_INPUT_BUF_MAX; u4Index ++)
    {
        if (pHandle == _prVDOOmxInputBuf[u4Index].pOwner)
        {
            fgAllFreed = FALSE;
            break;
        }
    }

    // release semaphore
    i4VDOOmxTaskUnlockSem();

    return fgAllFreed;
}

BOOL fgVDOOmxAllInputBufferAlloced(VDO_OMX_HANDLE_T *pHandle)
{
    UINT32 u4Index;
    BOOL fgAllAlloced = TRUE;

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    for (u4Index = 0; u4Index < VDO_INPUT_BUF_MAX; u4Index ++)
    {
        if (!_prVDOOmxInputBuf[u4Index].pOwner)
        {
            fgAllAlloced = FALSE;
            break;
        }
    }

    // release semaphore
    i4VDOOmxTaskUnlockSem();

    return fgAllAlloced;
}

BOOL fgVDOOmxAllOutputBufferFreed(VDO_OMX_HANDLE_T *pHandle)
{
    UINT32 u4Index;
    BOOL fgAllFreed = TRUE;

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    for (u4Index = 0; u4Index < VDO_OUTPUT_BUF_MAX; u4Index ++)
    {
        if (pHandle == _prVDOOmxOutputBuf[u4Index].pOwner)
        {
            fgAllFreed = FALSE;
            break;
        }
    }

    // release semaphore
    i4VDOOmxTaskUnlockSem();

    return fgAllFreed;
}


BOOL fgVDOOmxAllOutputBufferAlloced(VDO_OMX_HANDLE_T *pHandle)
{
    UINT32 u4Index;
    BOOL fgAllAlloced = TRUE;

    // multi-instance protection, take semaphore
    i4VDOOmxTaskLockSem();

    for (u4Index = 0; u4Index < VDO_OUTPUT_BUF_MAX; u4Index ++)
    {
        if (!_prVDOOmxOutputBuf[u4Index].pOwner)
        {
            fgAllAlloced = FALSE;
            break;
        }
    }

    // release semaphore
    i4VDOOmxTaskUnlockSem();

    return fgAllAlloced;
}

BOOL i4VDOOmxSetInfo(UINT32 u4InstId, UINT32 u4Info, UINT32 u4Para1, UINT32 u4Para2, UINT32 u4Para3)
{
    VDO_OMX_HANDLE_T *pHandle;

    if(u4InstId >= VDO_COMP_MAX)
    {
        LOG(0, "[VOMX] u4InstId(%d) >= VDO_COMP_MAX\n", u4InstId);
        return TRUE;
    }
    pHandle = &_prVDOOmxHandle[u4InstId];

    switch((VOMX_IO_TYPE)u4Info)
    {
        case VOMX_IO_SET_OUTPUT_DELAY:
            pHandle->nOutputDelay = u4Para1;
            LOG(0, "[VOMX] VOMX_IO_SET_OUTPUT_DELAY (%d)\n", u4Para1);
            break;
        default:
            break;
    }
    return TRUE;
}

BOOL i4VDOOmxPrintStatus(UINT32 u4InstId)
{
    VDO_OMX_HANDLE_T *pHandle;

    if(u4InstId >= VDO_COMP_MAX)
    {
        LOG(0, "[VOMX] u4InstId(%d) >= VDO_COMP_MAX\n", u4InstId);
        return TRUE;
    }
    pHandle = &_prVDOOmxHandle[u4InstId];

    LOG(0, "[VOMX] eCodecType: ");
    switch(pHandle->eCodecType)
    {
        case VC_UNKNOW:
            LOG(0, "VC_UNKNOW ");
            break;
        case VC_MPEG2:
            LOG(0, "VC_MPEG2 ");
            break;
        case VC_MPEG4:
            LOG(0, "VC_MPEG4 ");
            break;
        case VC_DIVX311:
            LOG(0, "VC_DIVX311 ");
            break;
        case VC_DIVX4:
            LOG(0, "VC_DIVX4 ");
            break;
        case VC_DIVX6:
            LOG(0, "VC_DIVX6 ");
            break;
        case VC_WMV1:
            LOG(0, "VC_WMV1 ");
            break;
        case VC_WMV2:
            LOG(0, "VC_WMV2 ");
            break;
        case VC_WMV3:
            LOG(0, "VC_WMV3 ");
            break;
        case VC_VC1:
            LOG(0, "VC_VC1 ");
            break;
        case VC_H263:
            LOG(0, "VC_H263 ");
            break;
        case VC_H263_SORENSON:
            LOG(0, "VC_H263_SORENSON ");
            break;
        case VC_H264:
            LOG(0, "VC_H264 ");
            break;
        case VC_RV30:
            LOG(0, "VC_RV30 ");
            break;
        case VC_RV40:
            LOG(0, "VC_RV40 ");
            break;
        case VC_MJPG:
            LOG(0, "VC_MJPG ");
            break;
        case VC_VP6:
            LOG(0, "VC_VP6 ");
            break;
        case VC_VP8:
            LOG(0, "VC_VP8 ");
            break;
        case VC_AVS:
            LOG(0, "VC_AVS ");
            break;
        case VC_JPG:
            LOG(0, "VC_JPG ");
            break;
        case VC_PNG:
            LOG(0, "VC_PNG ");
            break;
        default:
            LOG(0, "Unknown ");
            break;
    }
    LOG(0, "\n");
    LOG(0, "[VOMX] eState(0x%x) eStateTo(0x%x) Transing(%d) eStop(%d) eFlush(%d)\n",
        pHandle->eState, pHandle->eStateTo, pHandle->fgStateTransing,
        pHandle->eStopStatus, pHandle->eFlushStatus);
    LOG(0, "[VOMX] fgGotEOS(%d) fgSentEOS(%d) fgFlush[0](%d) fgFlush[1](%d)\n",
        pHandle->fgGotEOS, pHandle->fgSentEOS,
        pHandle->fgFlush[0], pHandle->fgFlush[1]);
    
    LOG(0, "VOMX, Empty Total(%2d) Cnt(%d) Size(%d) Cur(%2d) Wr(%2d) Rd(%2d)\n",
        VDO_INPUT_BUF_MAX,
        (pHandle->u4EmptyCounter),
        (pHandle->u4EmptySize),
        (pHandle->u4EmptyWritePos >= pHandle->u4EmptyReadPos) ? \
        (pHandle->u4EmptyWritePos - pHandle->u4EmptyReadPos) : \
        (VDO_INPUT_BUF_MAX - pHandle->u4EmptyReadPos + pHandle->u4EmptyWritePos),
        pHandle->u4EmptyWritePos,
        pHandle->u4EmptyReadPos);

    LOG(0, "VOMX, Fill Total(%2d) Cnt(%d) Cur(%2d) Wr(%2d) Rd(%2d)\n",
        VDO_OUTPUT_BUF_MAX,
        (pHandle->u4FillCounter),
        (pHandle->u4FillWritePos >= pHandle->u4FillReadPos) ? \
        (pHandle->u4FillWritePos - pHandle->u4FillReadPos) : \
        (VDO_OUTPUT_BUF_MAX - pHandle->u4FillReadPos + pHandle->u4FillWritePos),
        pHandle->u4FillWritePos,
        pHandle->u4FillReadPos);

    return TRUE;
}
#endif


