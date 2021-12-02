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
 * $RCSfile: tshift_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file ts_manual.c
 *  TimeShift demo application
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_hal_926.h"

#include "dmx_mm_if.h"
#include "fvr_if.h"
#include "fbm_drvif.h"
#include "mpv_drvif.h"

#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
// Capture
#define TSHIFT_CAP_THREAD_NAME                 "TSH_CAP_TH"
#define TSHIFT_CAP_THREAD_STACK_SIZE           2048
#define TSHIFT_CAP_THREAD_PRIORITY             100

#define TSHIFT_CAP_QUEUE_NAME                  "TSH_CAP_QU"
#define TSHIFT_CAP_QUEUE_SIZE                  32

// Playback
#define TSHIFT_PLAY_THREAD_NAME                "TSH_PLAY"
#define TSHIFT_PLAY_THREAD_STACK_SIZE          2048
#define TSHIFT_PLAY_THREAD_PRIORITY            100

#define TSHIFT_PLAY_QUEUE_NAME                 "TSH_QUEUE"
#define TSHIFT_PLAY_QUEUE_SIZE                 32

#define TSHIFT_MSG_PRIORITY_HIGH	           0

// Buffer
#define TSHIFT_RECBUF_SIZE          		   0x200000
#define TSHIFT_PLAYBUF_SIZE         		   0x200000

#define TSHIFT_REC_PKTCOUNT                    0x200
#define TSHIFT_PLAY_THRESHOLD                  0x20000

#define TSHIFT_DATA_SIZE			           0x1000000
#define TSHIFT_DATA_CHUNK_SIZE		           0x200000
#define TSHIFT_CHUNK_NUM			           (TSHIFT_DATA_SIZE / TSHIFT_DATA_CHUNK_SIZE)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
    TSHIFT_CAPTURE_OP_SAVE,
    TSHIFT_CAPTURE_OP_DISCARD,
} TSHIFT_CAPTURE_OP_T;

typedef struct
{
    TSHIFT_CAPTURE_OP_T rOp;
    UINT32 u4Addr;
    UINT32 u4Size;
} TSHIFT_CAPTURE_MSG_T;

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Bytes;
} TSHIFT_PLAY_MSG_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
// Record
static HANDLE_T _hTShiftCaptureThread = NULL_HANDLE;
static HANDLE_T _hTShiftCaptureQueue = NULL_HANDLE;

// Playback
static HANDLE_T _hTShiftPlayThread = NULL_HANDLE;
static HANDLE_T _hTShiftPlayQueue = NULL_HANDLE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _TShiftRecNotify
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _TShiftRecNotify(UINT32 u4Addr, UINT32 u4Size, UINT32 u4PktCount)
{
    TSHIFT_CAPTURE_MSG_T rMsg;

    rMsg.rOp = TSHIFT_CAPTURE_OP_SAVE;
    rMsg.u4Addr = u4Addr;
    rMsg.u4Size = u4Size;
    
    if (x_msg_q_send(_hTShiftCaptureQueue, (VOID*)&rMsg, sizeof (TSHIFT_CAPTURE_MSG_T),
                     TSHIFT_MSG_PRIORITY_HIGH) != OSR_OK)
    {
        LOG(3, "Timeshift capture queue full!\n");
    }

}


//-----------------------------------------------------------------------------
/** _TShiftCapThread
 */
//-----------------------------------------------------------------------------
static VOID _TShiftCapThread(VOID* pvArg)
{
    TSHIFT_CAPTURE_MSG_T rMsg;
    INT32 i4Ret;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex; 
    UINT32 u4RetSize;

    zMsgSize = sizeof(TSHIFT_CAPTURE_MSG_T);
    while (1)
    {
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
                                &_hTShiftCaptureQueue, 1, X_MSGQ_OPTION_WAIT);    

        if (i4Ret != OSR_OK)
        {
            ASSERT(0);
        }

        if(rMsg.rOp == TSHIFT_CAPTURE_OP_SAVE)
        {
            if(!TShift_Buf_Write(rMsg.u4Addr, rMsg.u4Size, &u4RetSize))
            {
                LOG(3, "Record: write buffer fail\n");
            }

            if(rMsg.u4Size != u4RetSize)
            {
                LOG(3, "Record: read buffer size not match\n");
                ASSERT(0);
            }
            
        }

        if(!FVR_UnlockBuffer(rMsg.u4Addr, rMsg.u4Size))
        {
            LOG(3, "Record: unlock buffer fail 0x%x, 0x%x\n", rMsg.u4Addr, rMsg.u4Size);
        }
    }

}


//-----------------------------------------------------------------------------
/** _TShiftPlayNotify
 */
//-----------------------------------------------------------------------------
static BOOL _TShiftPlayNotify(UINT32 u4Addr, UINT32 u4Size)
{
	TSHIFT_PLAY_MSG_T rMsg;
	
    Printf("_TSLPPlayNotify: 0x%x\n", u4Size);

    rMsg.u4Addr = u4Addr;
    rMsg.u4Bytes = u4Size;    

    if (x_msg_q_send(_hTShiftPlayQueue, (VOID*)&rMsg, sizeof (TSHIFT_PLAY_MSG_T),
                     TSHIFT_MSG_PRIORITY_HIGH) != OSR_OK)
    {
        LOG(3, "Timeshift play queue full!\n");
        return FALSE;
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _TShiftPlayThread
 */
//-----------------------------------------------------------------------------
static VOID _TShiftPlayThread(VOID* pvArg)
{
    TSHIFT_PLAY_MSG_T rMsg;
    INT32 i4Ret;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex; 
    UINT32 u4RetSize;

    zMsgSize = sizeof(TSHIFT_PLAY_MSG_T);
    while (1)
    {        
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
                                &_hTShiftPlayQueue, 1, X_MSGQ_OPTION_WAIT);    

        if (i4Ret != OSR_OK)
        {
            ASSERT(0);
        }

        Printf("size: 0x%x\n", rMsg.u4Bytes);       // Debug

        if(!TShift_Buf_Read(rMsg.u4Addr, rMsg.u4Bytes, &u4RetSize))
        {
            LOG(3, "Play: read buffer fail\n");
        }

        if(rMsg.u4Bytes != u4RetSize)
        {
            LOG(3, "Play: read buffer size not match\n");
            ASSERT(0);
        }

        if(!DMX_PVRPlay_UpdateBuf(rMsg.u4Addr, u4RetSize))
        {
            LOG(3, "Play: update read pointer fail\n");
        }
    }

}


//-----------------------------------------------------------------------------
/** _TShiftHWInit
 */
//-----------------------------------------------------------------------------
static BOOL _TShiftHWInit(void)
{
    DMX_PVR_PLAY_T rPlay;
    FVR_GBL_T rGBL;

    if(!FVR_FreeBuffer())
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_FreeBuf())
    {
        return FALSE;
    }

    // Capture init
    rGBL.fgAllocateBuffer = TRUE;
    rGBL.u2TargetPktCount = TSHIFT_REC_PKTCOUNT;
    rGBL.u4BufSize = TSHIFT_RECBUF_SIZE;
    rGBL.pfnPktCountNotify = _TShiftRecNotify;
    
    if(!FVR_SetGBL(FVR_GBL_FLAG_TARGET_PKT | FVR_GBL_FLAG_BUFFER | FVR_GBL_FLAG_NOTIFY, &rGBL))
    {
        return FALSE;
    }

    if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return FALSE;
    }
    
    // Play init
    rPlay.fgAllocBuf = TRUE;
    rPlay.u4BufStart = rGBL.u4BufStart;
    rPlay.u4BufSize = TSHIFT_PLAYBUF_SIZE;
    rPlay.u4ThresholdSize = TSHIFT_PLAY_THRESHOLD;
    rPlay.pfnFillDataNotify = _TShiftPlayNotify;

    if(!DMX_PVRPlay_Set(PVRPLAY_FLAGS_THRESHOLD |PVRPLAY_FLAGS_BUFFER | PVRPLAY_FLAGS_CALLBACK, 
                    &rPlay))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _TShiftSWInit
 */
//-----------------------------------------------------------------------------
static BOOL _TShiftSWInit()
{
    static BOOL _fgInited = FALSE;

    if(_fgInited)
    {
        return TRUE;
    }

    // Capture
    if (x_msg_q_create(&_hTShiftCaptureQueue, TSHIFT_CAP_QUEUE_NAME, sizeof(TSHIFT_CAPTURE_MSG_T),
                        TSHIFT_CAP_QUEUE_SIZE) != OSR_OK)
    {
        LOG(1, "Fail to create timeshift capture message queue!\n");
        return FALSE;
    }    
    if (x_thread_create(&_hTShiftCaptureThread, TSHIFT_CAP_THREAD_NAME,
                        TSHIFT_CAP_THREAD_STACK_SIZE, TSHIFT_CAP_THREAD_PRIORITY, 
                        _TShiftCapThread, 0, NULL) != OSR_OK)
    {
        LOG(1, "Fail to create timeshift capture thread!\n");
        return FALSE;
    }

    // Record
    if (x_msg_q_create(&_hTShiftPlayQueue, TSHIFT_PLAY_QUEUE_NAME, sizeof(TSHIFT_PLAY_MSG_T),
                        TSHIFT_PLAY_QUEUE_SIZE) != OSR_OK)
    {
        Printf("Fail to create timeshift play message queue!\n");
        return FALSE;
    }    
    if (x_thread_create(&_hTShiftPlayThread, TSHIFT_PLAY_THREAD_NAME,
                        TSHIFT_PLAY_THREAD_STACK_SIZE, TSHIFT_PLAY_THREAD_PRIORITY, 
                        _TShiftPlayThread, 0, NULL) != OSR_OK)
    {
        Printf("Fail to create timeshift Play thread!\n");
        return FALSE;
    }
    
    _fgInited = TRUE;

    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** TSHIFT_Init
 */
//-----------------------------------------------------------------------------
BOOL TSHIFT_Init(void)
{

    if((FVR_GetState() == FVR_STATE_RECORDING) || 
       (DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING))
    {
        LOG(3, "Please stop first\n");
        return FALSE;
    }

    // Data buffer init    
    if(!TShift_Buf_Init(TSHIFT_DATA_SIZE))
    {
        LOG(3, "Buffer init fail\n");
        return FALSE;
    }

    if(!_TShiftHWInit())
    {
        LOG(3, "HW init fail\n");
        return FALSE;
    }

    if(!_TShiftSWInit())
    {
        LOG(3, "SW init fail\n");
        return FALSE;
    }
            
    return TRUE;
}


//-----------------------------------------------------------------------------
/** TSHIFT_SetPid
 */
//-----------------------------------------------------------------------------
BOOL TSHIFT_SetPid(UINT8 u1Pidx, UINT16 u2PidNum, DMX_PID_TYPE_T eType)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** TSHIFT_LifeStop
 */
//-----------------------------------------------------------------------------
BOOL TSHIFT_LifeStop(void)
{
    return TRUE;
}

//-----------------------------------------------------------------------------
/** TSHIFT_PVRStart
 */
//-----------------------------------------------------------------------------
BOOL TSHIFT_PVRStart(void)
{
    if((FVR_GetState() == FVR_STATE_RECORDING) || 
       (DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING))
    {
        LOG(3, "Please stop first\n");
        return FALSE;
    }

    if(!DMX_PVRPlay_Start())
    {
        return FALSE;
    }
    
    if(!FVR_Start())
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** TSHIFT_PVRStop
 */
//-----------------------------------------------------------------------------
BOOL TSHIFT_PVRStop(void)
{
    if(!FVR_Stop())
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_Stop())
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** TSHIFT_Pause
 */
//-----------------------------------------------------------------------------
BOOL TSHIFT_Pause()
{
    // Stop play
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** TSHIFT_Forward
 */
//-----------------------------------------------------------------------------
BOOL TSHIFT_Forward(void)
{
    // Stop play

    // Search position

    // Start play

    return TRUE;
}


//-----------------------------------------------------------------------------
/** TSHIFT_Backward
 */
//-----------------------------------------------------------------------------
BOOL TSHIFT_Backward(void)
{
    // Stop play

    // Search position

    // Start play
    
    return TRUE;
}

