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
 * $RCSfile: dmx_psi_handler.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_psi_handler.c
 *  Demux driver - psi handler module
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_debug.h"
#include "dmx_mm_if.h"

#ifdef CC_DMX_AUDIO_PREBUF

LINT_EXT_HEADER_BEGIN

#include "drv_common.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_hal_926.h"

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// Demux queue and thread paramenters
//
#define DMX_AUD_QUEUE_NAME                  "DMXAUD"
#define DMX_AUD_QUEUE_SIZE                  32

#define DMX_AUD_THREAD_NAME                 "DMXAUDTHREAD"
#define DMX_AUD_THREAD_STACK_SIZE           2048
#define DMX_AUD_THREAD_PRIORITY             60

#define DMX_AUD_MAX_PES_QUEUE_SIZE          65535
#define DMX_AUD_PES_QUEUE_SIZE              240//128//64

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT8 u1SN;                         ///< Serial Number
    DMX_AUDIO_PES_T rAudPes;
} DMX_AUD_MSG_T;

typedef struct
{
    UINT16 u2Capacity;
    UINT16 u2Front;
    UINT16 u2Rear;
    UINT16 u2Size;
    UINT16 u2CapacityToCreate;
    DMX_AUDIO_PES_T *prArray;
} DMXQ_RECORD_T;

typedef struct
{
    BOOL fgEnable;
    DMXQ_RECORD_T rQRec;
} DMX_AUDHANDLER_INFO_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// Picture queue for MPEG decoder
static HANDLE_T _hDmxAudQueue = NULL_HANDLE;

/// Picture thread
static HANDLE_T _hDmxAudThread = NULL_HANDLE;

static UINT8 _u1DmxAudActivePidx = 0xFF;

static UINT8 _u1DmxAudPreBufIdx = 0xFF;

static DMX_AUDHANDLER_INFO_T _arDmxAudQRec[DMX_MM_STRUCT_NUM];

static BOOL _fgDmxAudEnable = FALSE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
static BOOL _DmxQIsEmpty(DMXQ_RECORD_T *prQ)
{
    if(prQ == NULL)
    {
        return FALSE;
    }

    if(prQ->u2Size == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
static BOOL _DmxQIsFull(DMXQ_RECORD_T *prQ)
{
    if(prQ == NULL)
    {
        return FALSE;
    }

    if(prQ->u2Size == prQ->u2Capacity)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
static BOOL _DmxQMakeEmpty(DMXQ_RECORD_T *prQ)
{
    if(prQ == NULL)
    {
        return FALSE;
    }

    prQ->u2Size = 0;
    prQ->u2Front = 1;
    prQ->u2Rear = 0;

    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DmxQSucc(DMXQ_RECORD_T *prQ, UINT16 u2Value, UINT16* pu2Value)
{
    if((prQ == NULL) || (pu2Value == NULL))
    {
        return FALSE;
    }

    if((++u2Value) == prQ->u2Capacity)
    {
        u2Value = 0;
    }

    *pu2Value = u2Value;

    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DmxQEnqueue(DMX_AUDIO_PES_T *prElement, DMXQ_RECORD_T *prQ)
{
    UINT16 u2Val;

    if((prElement == NULL) || (prQ == NULL))
    {
        return FALSE;
    }

    if(_DmxQIsFull(prQ))
    {
        LOG(3, "%s:%d, Queue is full, %d\n", __FUNCTION__, __LINE__, prQ->u2Size);
        return FALSE;
    }
    else
    {
        prQ->u2Size++;
        if(!_DmxQSucc(prQ, prQ->u2Rear, &u2Val))
        {
            return FALSE;
        }
        prQ->u2Rear = u2Val;
        prQ->prArray[prQ->u2Rear] = *prElement;
    }

    return TRUE;
}

#if 0
//-----------------------------------------------------------------------------
static BOOL _DmxQFront(DMXQ_RECORD_T *prQ, DMX_AUDIO_PES_T* prElement)
{
    if((prQ == NULL) || (prElement == NULL))
    {
        return FALSE;
    }

    if(!_DmxQIsEmpty(prQ))
    {
        *prElement = prQ->prArray[prQ->Front];
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DmxQDequeue(DMXQ_RECORD_T *prQ)
{
    UINT8 u1Val;

    if(prQ == NULL)
    {
        return FALSE;
    }

    if(_DmxQIsEmpty(prQ))
    {
        return FALSE;
    }
    else
    {
        prQ->u2Size--;
        if(!_DmxQSucc(prQ, prQ->u2Front, &u1Val))
        {
            return FALSE;
        }
        prQ->u2Front = u1Val;
    }

    return TRUE;
}
#endif

//-----------------------------------------------------------------------------
static BOOL _DmxQFrontAndDequeue(DMXQ_RECORD_T *prQ, DMX_AUDIO_PES_T* prElement)
{
    UINT16 u2Val;

    if((prQ == NULL) || (prElement == NULL))
    {
        return FALSE;
    }

    if(_DmxQIsEmpty(prQ))
    {
        return FALSE;
    }
    else
    {
        prQ->u2Size--;
        *prElement = prQ->prArray[prQ->u2Front];
        if(!_DmxQSucc(prQ, prQ->u2Front, &u2Val))
        {
            return FALSE;
        }
        prQ->u2Front = u2Val;
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DmxQCreateQueue(DMXQ_RECORD_T *prQ, UINT16 u2MaxElements)
{
    if(prQ == NULL)
    {
        return FALSE;
    }

    if( u2MaxElements > DMX_AUD_MAX_PES_QUEUE_SIZE)
    {
        return FALSE;
    }

    prQ->prArray = (DMX_AUDIO_PES_T*)x_mem_alloc(sizeof(DMX_AUDIO_PES_T) * u2MaxElements);
    if(prQ->prArray == NULL)
    {
        return FALSE;
    }

    prQ->u2Capacity = u2MaxElements;

    if(!_DmxQMakeEmpty(prQ))
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static void _DmxQDisposeQueue(DMXQ_RECORD_T *prQ)
{
    if((prQ != NULL) && (prQ->prArray != NULL))
    {
        x_mem_free(prQ->prArray);
        prQ->prArray = NULL;
    }
}

//-----------------------------------------------------------------------------
#if 0
static BOOL _DmxQDump(DMXQ_RECORD_T *prQ)
{
    DMX_AUDIO_PES_T rElement;
    UINT8 u1Val, i;

    if(prQ == NULL)
    {
        return FALSE;
    }

    if(_DmxQIsEmpty(prQ))
    {
        return FALSE;
    }
    else
    {
        u1Val = prQ->u2Front;
        for(i=0; i<prQ->u2Size; i++)
        {
            rElement = prQ->prArray[u1Val];
            LOG(3, "Pts:0x%x, wp:0x%x\n", rElement.u4Pts, rElement.u4Wp);

            if(!_DmxQSucc(prQ, u1Val, &u1Val))
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}
#endif

//-----------------------------------------------------------------------------
/** _DmxAudActHandler
 */
//-----------------------------------------------------------------------------
static BOOL _DmxAudActHandler(DMX_AUDIO_PES_T* prPes)
{
    DMX_AUDIO_PES_T rAudPes;
    PID_STRUCT_T *prSrcPid, *prDstPid;
    UINT32 u4SrcSize, u4DstSize, u4DataSize;
    DMX_AUDIO_PES_T rElement;

    if(prPes == NULL)
    {
        return FALSE;
    }

    if(prPes->u1PidIndex >= DMX_MM_STRUCT_NUM)
    {
        return FALSE;
    }

    if(!_arDmxAudQRec[prPes->u1PidIndex].fgEnable)
    {
        return FALSE;
    }

    prSrcPid = _DMX_GetPidStruct(prPes->u1PidIndex);
    prDstPid = _DMX_GetPidStruct(_u1DmxAudPreBufIdx);
    if((prSrcPid == NULL) || (prDstPid == NULL))
    {
        return FALSE;
    }

    if(!prPes->fgEOS)
    {
        // Copy data
        //u4SrcSize = DATASIZE(prSrcPid->u4Rp, prSrcPid->u4Wp, prSrcPid->u4BufLen);
        u4SrcSize = DATASIZE(VIRTUAL(prPes->u4PesRp), VIRTUAL(prPes->u4Wp), prSrcPid->u4BufLen);
        u4DstSize = DATASIZE(prDstPid->u4Rp, prDstPid->u4Wp, prDstPid->u4BufLen);
        if(u4SrcSize > (prDstPid->u4BufLen - u4DstSize))
        {
            LOG(3, "%s:%d, Audio buffer not enough\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        UNUSED(_DMX_CopyRingBuffer(prDstPid->u4Wp, prDstPid->u4BufStart, prDstPid->u4BufEnd + 1,
                                VIRTUAL(prPes->u4PesRp), prSrcPid->u4BufStart, prSrcPid->u4BufEnd + 1,
                                u4SrcSize));

        prDstPid->u4Wp += u4SrcSize;
        if(prDstPid->u4Wp > prDstPid->u4BufEnd)
        {
            prDstPid->u4Wp -= prDstPid->u4BufLen;
        }

        u4DataSize = DATASIZE(prSrcPid->u4Rp, prSrcPid->u4Wp, prSrcPid->u4BufLen);
        if((u4DataSize > ((prSrcPid->u4BufLen * 9) / 10)) ||
           (_arDmxAudQRec[prPes->u1PidIndex].rQRec.u2Size > (_arDmxAudQRec[prPes->u1PidIndex].rQRec.u2Capacity - 2)))
        {
            if(!_DmxQFrontAndDequeue(&_arDmxAudQRec[prPes->u1PidIndex].rQRec, &rElement))
            {
                return FALSE;
            }

            if(!_DMX_UpdateReadPointer(prPes->u1PidIndex, rElement.u4Wp, 0))
            {
                return FALSE;
            }
        }
    }

    // Enqueue
    if(!_DmxQEnqueue(prPes, &_arDmxAudQRec[prPes->u1PidIndex].rQRec))
    {
        return FALSE;
    }

    //-------------------------------------------------
    rAudPes = *prPes;
    rAudPes.u1PidIndex = _u1DmxAudPreBufIdx;
    rAudPes.u4Wp = prDstPid->u4Wp;

    if(!_DMX_SendAudioPes(&rAudPes))
    {
        LOG(3, "DMX audio handler send audio fail\n");
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DmxAudUpdateHandler
 */
//-----------------------------------------------------------------------------
static BOOL _DmxAudUpdateHandler(DMX_AUDIO_PES_T* prPes)
{
    PID_STRUCT_T *prPidStruct;
    UINT32 u4DataSize;
    DMX_AUDIO_PES_T rElement;

    prPidStruct = _DMX_GetPidStruct(prPes->u1PidIndex);
    if(prPidStruct == NULL)
    {
        return FALSE;
    }

    if(prPes->u1PidIndex >= DMX_MM_STRUCT_NUM)
    {
        return FALSE;
    }

    if(!_arDmxAudQRec[prPes->u1PidIndex].fgEnable)
    {
        return FALSE;
    }

    u4DataSize = DATASIZE(prPidStruct->u4Rp, prPidStruct->u4Wp, prPidStruct->u4BufLen);
    if((u4DataSize > ((prPidStruct->u4BufLen * 9) / 10)) ||
       (_arDmxAudQRec[prPes->u1PidIndex].rQRec.u2Size > (_arDmxAudQRec[prPes->u1PidIndex].rQRec.u2Capacity - 2)))

    {
        if(!_DmxQFrontAndDequeue(&_arDmxAudQRec[prPes->u1PidIndex].rQRec, &rElement))
        {
            return FALSE;
        }

        if(!_DMX_UpdateReadPointer(prPes->u1PidIndex, rElement.u4Wp, 0))
        {
            return FALSE;
        }
    }

    // Enqueue
    //LOG(4, "Enqueue, pts:0x%x\n", prPes->u4Pts);
    if(!_DmxQEnqueue(prPes, &_arDmxAudQRec[prPes->u1PidIndex].rQRec))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxAudHandler
 */
//-----------------------------------------------------------------------------
static BOOL _DmxAudHandler(DMX_AUDIO_PES_T* prPes)
{
    if(_u1DmxAudActivePidx == prPes->u1PidIndex)
    {
        return _DmxAudActHandler(prPes);
    }

    return _DmxAudUpdateHandler(prPes);
}


//-----------------------------------------------------------------------------
/** _DmxAudThread
 *  The Audio thread
 *
 *  @param  prArg           The thread argument
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _DmxAudThread(VOID* pvArg)
{
    static DMX_AUD_MSG_T rAudMsg;
    PID_STRUCT_T *prPidStruct;
    INT32 i4Ret;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex;

    UNUSED(pvArg);

    while (1)
    {
        zMsgSize = sizeof (rAudMsg);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rAudMsg, &zMsgSize,
                                &_hDmxAudQueue, 1, X_MSGQ_OPTION_WAIT);

        // Inhibit API function calls during processing message
        _DMX_LockApi();

        if (i4Ret != OSR_OK)
        {
            _DMX_UnlockApi();
            LOG(1, "Error on receiving DMX audio message!\n");
            continue;
        }

        prPidStruct = _DMX_GetPidStruct(rAudMsg.rAudPes.u1PidIndex);
        if(prPidStruct == NULL)
        {
            _DMX_UnlockApi();
            continue;
        }

        if (rAudMsg.u1SN != prPidStruct->u1SerialNumber)
        {
            _DMX_UnlockApi();
            LOG(3, "Overdue DMX Audio message! (pidx: %d)\n", rAudMsg.rAudPes.u1PidIndex);
            continue;
        }

        if(!prPidStruct->fgEnable)
        {
            _DMX_UnlockApi();
            continue;
        }

        ASSERT(zMsgSize == sizeof (rAudMsg));
        ASSERT(u2QueueIndex == 0);

        if(!_DmxAudHandler(&rAudMsg.rAudPes))
        {
            _DMX_UnlockApi();
            LOG(3, "%s, DMX audio handler send audio fail\n", __FUNCTION__);
            continue;
        }

        // Resume API calls
        _DMX_UnlockApi();
    }   // while (1)
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_InitAudHandler
 *  Initialize Audio handler
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_InitAudHandler(void)
{
    static BOOL _fgInit = FALSE;
    UINT8 i;
    
    _u1DmxAudActivePidx = 0xFF;
    _u1DmxAudPreBufIdx = 0xFF;
    _fgDmxAudEnable = FALSE;

    x_memset((void*)&_arDmxAudQRec, 0, sizeof(_arDmxAudQRec));
    //set default capacity
    for(i = 0; i < DMX_MM_STRUCT_NUM; i++) 
    {
        _arDmxAudQRec[i].rQRec.u2CapacityToCreate = DMX_AUD_PES_QUEUE_SIZE;
    }

    if (!_fgInit)
    {
        // Create picture queues
        if (x_msg_q_create(&_hDmxAudQueue, DMX_AUD_QUEUE_NAME,
                           sizeof(DMX_AUD_MSG_T), DMX_AUD_QUEUE_SIZE) != OSR_OK)
        {
            LOG(1, "Fail to create DMX audio queue!\n");
            return FALSE;
        }

        // Create PSI thread
        if (x_thread_create(&_hDmxAudThread, DMX_AUD_THREAD_NAME,
            DMX_AUD_THREAD_STACK_SIZE, DMX_AUD_THREAD_PRIORITY, _DmxAudThread, 0, NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX audio thread!\n");
            return FALSE;
        }

        _fgInit = TRUE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_AudHandler_AddPes
 */
//-----------------------------------------------------------------------------
BOOL _DMX_AudHandler_AddPes(const DMX_AUDIO_PES_T* prPes)
{
    PID_STRUCT_T *prPidStruct;
    DMX_AUD_MSG_T rAudMsg;
    BOOL fgEnable;

    if(prPes == NULL)
    {
        return FALSE;
    }

    _DMX_Lock();
    fgEnable = _fgDmxAudEnable;
    _DMX_Unlock();

    if(!fgEnable)
    {
        return _DMX_SendAudioPes(prPes);
    }

    prPidStruct = _DMX_GetPidStruct(prPes->u1PidIndex);
    if(prPidStruct == NULL)
    {
        return FALSE;
    }

    rAudMsg.u1SN = prPidStruct->u1SerialNumber;
    rAudMsg.rAudPes = *prPes;

    if (x_msg_q_send(_hDmxAudQueue, (VOID*)&rAudMsg, sizeof(DMX_AUD_MSG_T),
                     DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(3, "Demux audio queue full!\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_AudHandler_SetActivePidx
 */
//-----------------------------------------------------------------------------
BOOL _DMX_AudHandler_SetActivePidx(UINT8 u1Idx)
{
    //PID_STRUCT_T *prSrcPid, *prDstPid;
    //UINT32 u4SrcSize, u4DstSize;

    if(u1Idx == DMX_AUDIO_HANDLER_NULL_PIDX)
    {
        _u1DmxAudActivePidx = DMX_AUDIO_HANDLER_NULL_PIDX;
        return TRUE;
    }

    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        return FALSE;
    }

    _u1DmxAudActivePidx = u1Idx;

    // Copy remainder
    if((_arDmxAudQRec[u1Idx].fgEnable) && (_arDmxAudQRec[u1Idx].rQRec.u2Size > 0))
    {
#if 0
        prSrcPid = _DMX_GetPidStruct(u1Idx);
        prDstPid = _DMX_GetPidStruct(_u1DmxAudPreBufIdx);
        if(prSrcPid == NULL)
        {
            return FALSE;
        }

        u4SrcSize = DATASIZE(prSrcPid->u4Rp, prSrcPid->u4Wp, prSrcPid->u4BufLen);
        u4DstSize = DATASIZE(prDstPid->u4Rp, prDstPid->u4Wp, prDstPid->u4BufLen);
        if(u4SrcSize > (prDstPid->u4BufLen - u4DstSize))
        {
            LOG(3, "%s:%d, Audio buffer not enough\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        UNUSED(_DMX_CopyRingBuffer(prDstPid->u4Wp, prDstPid->u4BufStart, prDstPid->u4BufEnd + 1,
                                prSrcPid->u4Rp, prSrcPid->u4BufStart, prSrcPid->u4BufEnd + 1,
                                u4SrcSize));

        if(!_DmxQMakeEmpty(&_arDmxAudQRec[u1Idx].rQRec))
        {
            return FALSE;
        }
#endif
        DMX_AUDIO_PES_T rElement;
        UINT16 i, u2QSize;

        u2QSize = _arDmxAudQRec[u1Idx].rQRec.u2Size;
        for(i=0; i<u2QSize; i++)
        {
            if(!_DmxQFrontAndDequeue(&_arDmxAudQRec[u1Idx].rQRec, &rElement))
            {
                return FALSE;
            }

            ASSERT(rElement.u1PidIndex == u1Idx);

            if(!_DmxAudActHandler(&rElement))
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_AudHandler_Set
 */
//-----------------------------------------------------------------------------
BOOL _DMX_AudHandler_Set(UINT8 u1Idx, BOOL fgToDec)
{
    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        return FALSE;
    }

    if(fgToDec)
    {
        _u1DmxAudPreBufIdx = u1Idx;
    }
    else
    {
        if(_arDmxAudQRec[u1Idx].fgEnable)
        {
            LOG(3, "%s:%d, already enable\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        if(!_DmxQCreateQueue(&_arDmxAudQRec[u1Idx].rQRec, _arDmxAudQRec[u1Idx].rQRec.u2CapacityToCreate))
        {
            return FALSE;
        }

        _arDmxAudQRec[u1Idx].fgEnable = TRUE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_AudHandler_Free
 */
//-----------------------------------------------------------------------------
BOOL _DMX_AudHandler_Free(UINT8 u1Idx)
{
    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        return FALSE;
    }

    if(!_arDmxAudQRec[u1Idx].fgEnable)
    {
        return TRUE;
    }

    _DmxQDisposeQueue(&_arDmxAudQRec[u1Idx].rQRec);
    _arDmxAudQRec[u1Idx].fgEnable = FALSE;
    _arDmxAudQRec[u1Idx].rQRec.u2CapacityToCreate = DMX_AUD_PES_QUEUE_SIZE;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_AudHandler_SeekAndUpdate
 */
//-----------------------------------------------------------------------------
BOOL _DMX_AudHandler_SeekAndUpdate(UINT8 u1Idx, UINT32 u4Pts, UINT32 *pu4Pts)
{
    DMX_AUDIO_PES_T rElement;
    UINT16 i, u2QSize;

    if(pu4Pts == NULL)
    {
        return FALSE;
    }

    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        return FALSE;
    }

    if(!_arDmxAudQRec[u1Idx].fgEnable)
    {
        return FALSE;
    }

    if(_DmxQIsEmpty(&_arDmxAudQRec[u1Idx].rQRec))
    {
        return FALSE;
    }

    LOG(5, "%s, queue size:%d, original pts:0x%x\n", __FUNCTION__,
                _arDmxAudQRec[u1Idx].rQRec.u2Size, u4Pts);
    //VERIFY(_DmxQDump(&_arDmxAudQRec[u1Idx].rQRec));

    *pu4Pts = 0;
    u2QSize = _arDmxAudQRec[u1Idx].rQRec.u2Size;
    for(i=0; i<u2QSize; i++)
    {
        if(!_DmxQFrontAndDequeue(&_arDmxAudQRec[u1Idx].rQRec, &rElement))
        {
            return FALSE;
        }

        ASSERT(rElement.u1PidIndex == u1Idx);

        LOG(5, "%s, pts:0x%x\n", __FUNCTION__, rElement.u4Pts);

        if(rElement.u4Pts >= u4Pts)
        {
            *pu4Pts = rElement.u4Pts;
            if(!_DMX_UpdateReadPointer(rElement.u1PidIndex, rElement.u4Wp, 0))
            {
                return FALSE;
            }
            return TRUE;
        }

        if(i == (u2QSize - 1))
        {
            if(!_DMX_UpdateReadPointer(rElement.u1PidIndex, rElement.u4Wp, 0))
            {
                return FALSE;
            }
        }
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
/** _DMX_AudHandler_SetEnable
 */
//-----------------------------------------------------------------------------
void _DMX_AudHandler_SetEnable(BOOL fgEnable)
{
    _DMX_Lock();
    _fgDmxAudEnable = fgEnable;
    _DMX_Unlock();
}

//-----------------------------------------------------------------------------
/** _DMX_AudHandler_SetQSize
 */
//-----------------------------------------------------------------------------
BOOL _DMX_AudHandler_SetQSize(UINT8 u1Idx, UINT16 u2Size)
{
    if(u1Idx >= DMX_MM_STRUCT_NUM)
    {
        return FALSE;
    }
    
    _arDmxAudQRec[u1Idx].rQRec.u2CapacityToCreate = u2Size;

    return TRUE;
}
#endif    // CC_DMX_AUDIO_PREBUF


