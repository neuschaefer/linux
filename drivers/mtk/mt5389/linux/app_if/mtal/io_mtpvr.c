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
 * $RCSfile: io_mtpvr.c,v $
 * $Revision: #2 $
 * $Author: hongjun.chu $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *---------------------------------------------------------------------------*/

/** @file io_mtpvr.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtpvr.h"
#include "dmx_if.h"
#include "dmx_mm_if.h"
#include "fvr_if.h"
#include "vdec_drvif.h"
#include "feeder_if.h"

#include "cb_data.h"
#include "cb_low.h"
#include <linux/mtal_ioctl.h>

#include "x_assert.h"
#include "swdmx_drvif.h"

LINT_EXT_HEADER_BEGIN
#define DEFINE_IS_LOG    SWDMX_IsLog
#include "x_debug.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define PVR0                    (0)
#define PVR_MAX_ID              (1)
#define OUTPUT_SIZE_188             (188 * 128)
#define OUTPUT_SIZE_192             (192 * 128)
#define INPUT_SIZE_188              (376 * 1024)
#define INPUT_SIZE_192              (384 * 1024)
#define RESERVED_BUFFER_SIZE_188    ((INPUT_SIZE_188) + (OUTPUT_SIZE_188) + (1024 * 240))
#define RESERVED_BUFFER_SIZE_192    ((INPUT_SIZE_192) + (OUTPUT_SIZE_192) + (1024 * 240))

// INPUT_SIZE is for MTPVR write, OUTPUT_SIZE is for swdmx_mpg_pvr
#define PVRBUF_NUM              ((MTPVR_PVRDATASIZE/INPUT_SIZE) + 2)
#define MTPVR_DATA_TIMER_THRSD          16000

#define MTPVR_THREAD_NAME               "MTPVR"
#define MTPVR_THREAD_STACK_SIZE         4096
#define MTPVR_THREAD_PRIORITY           60
#define MTPVR_CMD_NAME                  "MTPVR_CMD"
#define MTPVR_CMD_Q_SIZE	        	16

#define MTVDEC_NOTIFY_IFRAME        (1<<0)
#define MTVDEC_NOTIFY_THUMBNAIL     (1<<1)
#define MTVDEC_NOTIFY_DECODE_IFRAME (1<<2)  // 20110801

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum
{
    ePVR_STATE_STOP,			// stop
    ePVR_STATE_PAUSE,       	// pause
    ePVR_STATE_PLAY,         	// play
    ePVR_CMD_FLUSH,         	// flush
    ePVR_STATE_MAX
} ENUM_MTPVR_PLAY_STATE_T;

typedef enum
{
    ePVR_STEP_DONTCARE,		// not in step
    ePVR_STEP_START,		// in step
    ePVR_STEP_DONE,       	// step end
    ePVR_STEP_END,       	// step end
    ePVR_STEP_MAX
} ENUM_MTPVR_STEP_STATE_T;

typedef struct PVR_STATUS
{
	BOOL        fgStarted;
	
	BOOL        fgFirst;
	BOOL        fgSpeedChange;
    BOOL        fgThumbTimerStarted;

    UINT32  u4PvrBaseAddr;
    UINT32  u4PvrBufSize;
    UINT32  u4StartAddr;
    UINT32  u4EndAddr;
    UINT32  u4ReadAddr;
    UINT32  u4WriteAddr;
    UINT32  u4QueuedSize;
    INT32  i4Speed;
    INT32  i4SpeedNew;

    UINT32  u4FeederAddrPhy;
    UINT32  u4FeederAddrVir;
    
    HANDLE_T hPVRMainThread;
    HANDLE_T hPVRPlayCmdQ;
    HANDLE_T hThumbTimer;       // timer to keep watching input data
    
    DMX_PVR_PLAY_T rPvrPlay;
    ENUM_MTPVR_PLAY_STATE_T ePVRPrevousStatus;
    ENUM_MTPVR_PLAY_STATE_T ePVRStatus;
    ENUM_MTPVR_STEP_STATE_T ePVRStep;
} MTPVR_STATUS_T;


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(INT32)))                            \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_ARG_SIZE(ArgUsr, ArgKernel, size)       \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       (size)))                             \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(INT32)))                        \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_6ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_6ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_6ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_6ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_8ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_8ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_8ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");      \
        return MTR_ERR_INV;                                          \
    } \

#define COPY_TO_USER_ARG_SIZE(ArgUsr, ArgKernel,size)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_mtpvr argument error\n");      \
        return MTR_ERR_INV;                                          \
    } \


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static BOOL _fgPVRInit = FALSE;
static MTPVR_STATUS_T _rPVRStatus = {0};
static UINT32 _u4ThumbnailId = 0;
static BOOL _fgThumbnail = FALSE;
static HANDLE_T _hQueueSema = 0;
static BOOL _fgPVRLocking = FALSE;
//static BOOL _fgPVRThumbnailLocking = FALSE;
static CRIT_STATE_T _rPVRLock;
static VDP_THUMBNAIL_INFO_T _rThumbDstInfo;
static UCHAR _ucThumbVdecId = ES0;
extern UINT32 _u4NotifyMask;
extern UCHAR _aucMTVDECEsId[VDEC_MAX_ES];

static UINT32 _u4OutputSize = OUTPUT_SIZE_192;
static UINT32 _u4InputSize = INPUT_SIZE_192;
static UINT32 _u4ReservedBufferSize = RESERVED_BUFFER_SIZE_192;
static BOOL _fgContainTimeStamp = TRUE;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifdef VDEC_IS_POST_MT5368
extern VOID _VDP_SetTimeShiftSpeed(UINT8 u1B2rId,UINT32 u4SpeedValue);
extern VOID _VDP_SetTimeShiftMode(UINT8 u1B2rId,BOOL fgEnable);
#endif
#ifdef PVR_RANDOM_BUF
static UINT32 _au4WriteAddr[PVRBUF_NUM];
static UINT32 _au4WriteSize[PVRBUF_NUM];
#endif

extern void _MTVDEC_Notify_Lock(void);
extern void _MTVDEC_Notify_Unlock(void);

extern MT_RESULT_T  _MTAUD_Play (UINT8 u1DecId);
extern MT_RESULT_T  _MTAUD_Stop (UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** _MTPVR_Lock
 *  Enter demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _MTPVR_Lock(void)
{
    _rPVRLock = x_crit_start();

    ASSERT(!_fgPVRLocking);
    _fgPVRLocking = TRUE;

    UNUSED(_fgPVRLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** _MTPVR_Unlock
 *  Leave demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _MTPVR_Unlock(void)
{
    ASSERT(_fgPVRLocking);
    _fgPVRLocking = FALSE;

    x_crit_end(_rPVRLock);

    UNUSED(_fgPVRLocking);          // Make Lint happy
}
#if 0
static void _MTPVR_ThumbnailLock(void)
{
    if(_hQueueSema)
    {
        VERIFY(x_sema_lock(_hQueueSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        _fgPVRThumbnailLocking = TRUE;
    }
}
static void _MTPVR_ThumbnailUnlock(void)
{
    if(_hQueueSema)
    {
        _fgPVRThumbnailLocking = FALSE;
    VERIFY(x_sema_unlock(_hQueueSema) == OSR_OK);
}
}
#endif

static VOID _MTPVR_CleanBuf(VOID)
{
    _rPVRStatus.fgFirst = TRUE;
    _rPVRStatus.u4ReadAddr = _rPVRStatus.u4WriteAddr;
    _MTPVR_Lock();
    _rPVRStatus.u4QueuedSize = 0;
    _MTPVR_Unlock();

    MTPVR_PRINT("%s done\n", __FUNCTION__);
}
extern void _MTVDEC_VDEC_Nfy(
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2);

void _MTPVR_VDEC_Nfy(
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2)
{
    MTPVR_THUMBNAIL_CB_T rThumbCb;
    MTVDEC_PRINT("_MTPVR_VDEC_Nfy\n");

    x_memset(&rThumbCb, 0, sizeof(MTPVR_THUMBNAIL_CB_T));
    //_MTPVR_ThumbnailLock();
    if(eNfyCond == VDEC_COND_THUMBNAIL_DONE)
    {
        MTVDEC_PRINT(" - Vdec thumbnail done\n");
        if(_fgThumbnail)
        {
            LOG(5, "%s(L:%d) DONE\n", __FUNCTION__, __LINE__);
            // notify
            rThumbCb.fgOK = TRUE;
            rThumbCb.u4Id = _u4ThumbnailId;
            rThumbCb.u4DeltaMs = u4Data1;
            _CB_PutEvent(CB_MTAL_PVR_THUMBNAIL_TRIGGER,
                sizeof(MTPVR_THUMBNAIL_CB_T),
                (void*)&rThumbCb);
            _fgThumbnail = FALSE;
        }
    }
    else if(eNfyCond == VDEC_COND_THUMBNAIL_FAIL)
    {
        MTVDEC_PRINT(" - Vdec thumbnail error(%d)\n", u4Data1);
        if(_fgThumbnail)
        {
            LOG(3, "%s(L:%d) FAIL\n", __FUNCTION__, __LINE__);
            // notify a black image
            rThumbCb.fgOK = TRUE;
            rThumbCb.u4Id = _u4ThumbnailId;
            _CB_PutEvent(CB_MTAL_PVR_THUMBNAIL_TRIGGER,
                sizeof(MTPVR_THUMBNAIL_CB_T),
                (void*)&rThumbCb);
            _fgThumbnail = FALSE;
        }
    }
    //_MTPVR_ThumbnailUnlock();

    UNUSED(pvNfyTag);
    UNUSED(eNfyCond);
    UNUSED(u4Data1);
    UNUSED(u4Data2);
}


static VOID _MTPVR_ThumbTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    if(_fgThumbnail)
    {
        LOG(3, "Notify thumbnail timeout\n");
        _MTPVR_VDEC_Nfy((void*)pv_tag,
            VDEC_COND_THUMBNAIL_FAIL,  0, 0);
    }

    UNUSED(pt_tm_handle);
}

static VOID _MTPVR_StartTimer(UCHAR ucPvrId, UCHAR ucVdecId)
{
    if(ucVdecId >= VDEC_MAX_ES)
    {
        LOG(1, "%s(L:%d) ucVdecId(%d) error\n", __FUNCTION__, __LINE__, ucVdecId);
        return;
    }
    if(_rPVRStatus.hThumbTimer && !_rPVRStatus.fgThumbTimerStarted)
    {
        VERIFY(x_timer_start(_rPVRStatus.hThumbTimer, MTPVR_DATA_TIMER_THRSD, X_TIMER_FLAG_ONCE,
            _MTPVR_ThumbTimeout, (void*)&_aucMTVDECEsId[ucVdecId]) == OSR_OK);
        _rPVRStatus.fgThumbTimerStarted = TRUE;
    }
    UNUSED(ucPvrId);
}

static VOID _MTPVR_StopTimer(UCHAR ucPvrId)
{
    if(_rPVRStatus.hThumbTimer && _rPVRStatus.fgThumbTimerStarted)
    {
        VERIFY(x_timer_stop(_rPVRStatus.hThumbTimer) == OSR_OK);
        _rPVRStatus.fgThumbTimerStarted = FALSE;
    }
    UNUSED(ucPvrId);
}

static BOOL _MTPVR_ReceivePlayCmd(UCHAR ucPvrId, VOID *pvData, UINT32 u4CurState)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    //ENUM_MTPVR_PLAY_STATE_T eStatus;
    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("_PVR_MainLoop (ucPvrId >= PVR_SOURCE_MAX)\n");
    }

    zMsgSize = sizeof(UINT32);
    if (u4CurState != ePVR_STATE_PLAY)
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(_rPVRStatus.hPVRPlayCmdQ), 1, X_MSGQ_OPTION_WAIT);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(_rPVRStatus.hPVRPlayCmdQ), 1, X_MSGQ_OPTION_NOWAIT);
    }

    ASSERT(zMsgSize == sizeof(UINT32));

    if (i4Ret != OSR_OK)
    {
        return FALSE;
    }

    return TRUE;
}


static BOOL _MTPVR_SendPlayCmd(UCHAR ucPvrId, ENUM_MTPVR_PLAY_STATE_T ePlayState, UCHAR ucPriority)
{
    UINT32 u4TryCnt = 0;
    INT32 i4Ret;
    UINT32 u4PlayCmd = (UINT32)ePlayState;
    SIZE_T zMsgSize = sizeof(UINT32);

    i4Ret = x_msg_q_send(_rPVRStatus.hPVRPlayCmdQ, &u4PlayCmd, zMsgSize, ucPriority);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(100);
        MTPVR_PRINT("PVR Cmd(%d) Queue, OSR_TOO_MANY\n", u4PlayCmd);
        i4Ret = x_msg_q_send(_rPVRStatus.hPVRPlayCmdQ, &u4PlayCmd, zMsgSize, ucPriority);
        u4TryCnt ++;
        if(u4TryCnt > 50)
        {
            MTPVR_PRINT("PVR Cmd(%d) Queue, OSR_TOO_MANY -> exit\n", u4PlayCmd);
            break;
        }
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;

}

BOOL _Mtpvr_SetPlayBackMode(UINT8 u1PacketSize)
{
    if(u1PacketSize == 188)
    {
        _u4OutputSize = OUTPUT_SIZE_188;
        _u4InputSize = INPUT_SIZE_188;
        _u4ReservedBufferSize = RESERVED_BUFFER_SIZE_188;
        _fgContainTimeStamp = FALSE;
    }
    else if(u1PacketSize == 192)
    {
        _u4OutputSize = OUTPUT_SIZE_192;
        _u4InputSize = INPUT_SIZE_192;
        _u4ReservedBufferSize = RESERVED_BUFFER_SIZE_192;
        _fgContainTimeStamp = TRUE;
    }
    else
    {
        MTPVR_PRINT("Not support packet size: %d\n", u1PacketSize);
        return FALSE;
    }

    return TRUE;
}

MT_RESULT_T _MTPVR_SendData(UCHAR ucPvrId)
{
    MT_RESULT_T eRet = MTR_OK;
    SWDMX_PVR_SENDDATA_RET_T ePVRRet;
    DMX_MM_DATA_T rDmxMMData;
    UINT32 u4SendSize = 0;

    u4SendSize = _rPVRStatus.u4QueuedSize;
    if(u4SendSize > _u4OutputSize)
    {
        u4SendSize = _u4OutputSize;
    }
    if(u4SendSize == 0)
    {
        x_thread_delay(5);
        return MTR_OK;
    }
    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    if(_rPVRStatus.fgFirst)
    {
        rDmxMMData.fgFrameHead = TRUE;
        LOG(5, "%s(L:%d) First Move\n", __FUNCTION__, __LINE__);
    }
    rDmxMMData.u4BufStart = _rPVRStatus.u4PvrBaseAddr + _rPVRStatus.u4StartAddr;
    rDmxMMData.u4BufEnd = _rPVRStatus.u4PvrBaseAddr + _rPVRStatus.u4EndAddr;
    rDmxMMData.u4StartAddr = _rPVRStatus.u4PvrBaseAddr + _rPVRStatus.u4ReadAddr;
    rDmxMMData.u4FrameSize = u4SendSize;

{
    BOOL fgFast = FALSE;

        if(_fgContainTimeStamp == FALSE)
        {
            fgFast = TRUE;
        }

    if(!((_rPVRStatus.i4Speed == 25) ||
         (_rPVRStatus.i4Speed == 50) ||
         (_rPVRStatus.i4Speed == 100) ||
         (_rPVRStatus.i4Speed == 200)))
    {
        fgFast = TRUE;

        #ifdef VDEC_IS_POST_MT5368
        {
	        UINT32 u4DmxAvailSizeVideo;
	        // 2 stands for record path.
	        u4DmxAvailSizeVideo = DMX_GetMinFreeBufferSizeEx(
	            2, DMX_PID_TYPE_ES_VIDEO);
    	        if(u4DmxAvailSizeVideo < _u4OutputSize)
	        {
	            x_thread_delay(5);
	            return MTR_OK;
	        }
        }
        #endif
    }
    //printf("_SWDMX_PVR_SendData\n");
        ePVRRet = SWDMX_PVR_SendData(ucPvrId, &rDmxMMData, _u4InputSize, fgFast);
    //printf("_SWDMX_PVR_SendData -> done\n");
}

    if(ePVRRet == SWDMX_PVR_SENDDATA_FULL)
    {
        //printf("_SWDMX_PVR_SendData -> delay\n");
        x_thread_delay(5);
        return MTR_OK;
    }
    else if(ePVRRet != SWDMX_PVR_SENDDATA_OK)
    {
        //printf("_SWDMX_PVR_SendData -> not ok\n");
        LOG(5, "%s(L:%d) (ePVRRet(%d) != PVR_PVR_SENDDATA_OK) error\n", __FUNCTION__, __LINE__,
            (UINT32)ePVRRet);
        MTPVR_PRINT("%s (ePVRRet(%d) != PVR_PVR_SENDDATA_OK) error\n",
            __FUNCTION__, (UINT32)ePVRRet);
        x_thread_delay(10);
        return MTR_NOT_OK;
    }
    if(_rPVRStatus.fgFirst)
    {
        _rPVRStatus.fgFirst = FALSE;
    }
    _rPVRStatus.u4ReadAddr += u4SendSize;
    _MTPVR_Lock();
    _rPVRStatus.u4QueuedSize -= u4SendSize;
    _MTPVR_Unlock();
    if(_rPVRStatus.u4ReadAddr >= _rPVRStatus.u4EndAddr)
    {
        _rPVRStatus.u4ReadAddr -= (_rPVRStatus.u4EndAddr - _rPVRStatus.u4StartAddr);
    }
    #if 0
    else if(_rPVRStatus.u4ReadAddr > _rPVRStatus.u4EndAddr)
    {
        MTPVR_PRINT("%s u4ReadAddr(%d) > u4EndAddr(%d)\n",
            __FUNCTION__, _rPVRStatus.u4ReadAddr, _rPVRStatus.u4EndAddr);
        ASSERT(0);
        return MTR_NOT_OK;
    }
    #endif
    return eRet;
}

static VOID _MTPVR_MainLoop(VOID *pvArg)
{
    UCHAR ucPvrId;
    UINT32 u4PlayCmd;
    DMX_PVR_PLAY_T rPvrPlay;

    ucPvrId = (UCHAR)(*((UINT8*)pvArg));
    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("_PVR_MainLoop (ucPvrId >= PVR_SOURCE_MAX)\n");
    }

    while (1)
    {
        // block if SwDmx status is not ePVR_STATE_PLAY
        u4PlayCmd = (UINT32)ePVR_STATE_MAX;
        if (_MTPVR_ReceivePlayCmd(ucPvrId, &u4PlayCmd, _rPVRStatus.ePVRStatus) == TRUE)
        {
            if ((_rPVRStatus.ePVRStatus == ePVR_STATE_STOP) &&
                    (u4PlayCmd == (UINT32)ePVR_STATE_PLAY))
            {
                // start play
                #ifdef VDEC_IS_POST_MT5368
                _VDP_SetTimeShiftMode(0,1);
                #endif
            }

            if ((_rPVRStatus.ePVRStatus == ePVR_STATE_PAUSE) &&
                    (u4PlayCmd == (UINT32)ePVR_STATE_PLAY))
            {
                // pause to play
                //_SWDMX_PVR_Resume(ucPvrId);
            }

            if ((_rPVRStatus.ePVRStatus == ePVR_STATE_PLAY) &&
                    (u4PlayCmd == (UINT32)ePVR_STATE_PAUSE))
            {
                // play to pause
                //_SWDMX_PVR_Pause(ucPvrId);
            }

            if ((_rPVRStatus.ePVRStatus != ePVR_STATE_STOP) &&
                    (u4PlayCmd == (UINT32)ePVR_STATE_STOP))
            {
                #ifdef VDEC_IS_POST_MT5368
                _VDP_SetTimeShiftMode(0,0);
                #endif
                
                // stop, clear data
                if(!SWDMX_PVR_Stop(ucPvrId))
                {
                    MTPVR_PRINT("%s SWDMX_PVR_Stop fail\n",
                        __FUNCTION__);
                }

                if(!SWDMX_PVR_Close(ucPvrId))
                {
                    MTPVR_PRINT("%s SWDMX_PVR_Close fail\n",
                        __FUNCTION__);
                }

                UNUSED(DMX_SetTSMode(ucPvrId, DMX_TSFMT_188));       // Default

                VDEC_SetMMParam(ES0, VDEC_MM_CONTAINER_TYPE, SWDMX_FMT_UNKNOWN, 0, 0);
            }

            if(u4PlayCmd == (UINT32)ePVR_CMD_FLUSH)
            {
                if(!SWDMX_PVR_Stop(ucPvrId))
                {
                    MTPVR_PRINT("%s _SWDMX_PVR_Stop fail\n", __FUNCTION__);
                }

                if(!DMX_PVRPlay_RequestReset())
                {
                    MTPVR_PRINT("%s DMX_PVRPlay_RequestReset fail\n", __FUNCTION__);
                }

                _MTPVR_CleanBuf();
                continue;
            }

            _rPVRStatus.ePVRPrevousStatus = _rPVRStatus.ePVRStatus;
            UNUSED(_rPVRStatus.ePVRPrevousStatus);
            _rPVRStatus.ePVRStatus = (ENUM_MTPVR_PLAY_STATE_T)u4PlayCmd;
            continue;
        }

        if (_rPVRStatus.ePVRStatus == ePVR_STATE_PLAY)
        {
            if(_rPVRStatus.fgSpeedChange)
            {
                _rPVRStatus.i4Speed = _rPVRStatus.i4SpeedNew;
                _rPVRStatus.fgSpeedChange = FALSE;

                // DMX
     
                // Set Speed to swdmx 

                //_MTPVR_Lock();
                
                //set speed pvr speed to display module
                #ifdef VDEC_IS_POST_MT5368
                _VDP_SetTimeShiftSpeed(0,_rPVRStatus.i4Speed);
                #endif

                if(((_rPVRStatus.i4Speed == 25) ||
                    (_rPVRStatus.i4Speed == 50) ||
                    (_rPVRStatus.i4Speed == 100) ||
                    (_rPVRStatus.i4Speed == 200)))
                {
                    SWDMX_PVR_Pause(ucPvrId);                       
                    _rPVRStatus.rPvrPlay.fgContainTimeStamp = _fgContainTimeStamp;
                    _rPVRStatus.rPvrPlay.fgIgnoreTimeStamp = FALSE;
                    _rPVRStatus.rPvrPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE * 100 / _rPVRStatus.i4Speed;

                    if(_rPVRStatus.i4Speed == 100)
                {
                    SWDMX_PVR_FlushBuf(ucPvrId);
                }

                    rPvrPlay.fgContainTimeStamp = _rPVRStatus.rPvrPlay.fgContainTimeStamp;
                    rPvrPlay.fgIgnoreTimeStamp = _rPVRStatus.rPvrPlay.fgIgnoreTimeStamp;
                    rPvrPlay.u2TimeStampFreqDiv = _rPVRStatus.rPvrPlay.u2TimeStampFreqDiv;

                    if(!DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_TIMESTAMP), &rPvrPlay))
                    {
                        MTPVR_PRINT("%s fail to set timestamp.\n", __FUNCTION__);
                    }
                    
                    SWDMX_PVR_Resume(ucPvrId);
                    
        }
                else if(_rPVRStatus.i4Speed == 0)
                {
                    // do nothing
    }
                else
                {
                    if(!DMX_PVRPlay_Get((UINT32)(PVRPLAY_FLAGS_TIMESTAMP), &rPvrPlay))
                    {
                        MTPVR_PRINT("%s fail to set timestamp.\n", __FUNCTION__);
}

                    if(rPvrPlay.fgIgnoreTimeStamp == FALSE)
                    {
                        SWDMX_PVR_Pause(ucPvrId);                       
                        _rPVRStatus.rPvrPlay.fgContainTimeStamp = _fgContainTimeStamp;
                        _rPVRStatus.rPvrPlay.fgIgnoreTimeStamp = TRUE;
                        _rPVRStatus.rPvrPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;

                        //SWDMX_PVR_FlushBuf(ucPvrId);

                        rPvrPlay.fgContainTimeStamp = _rPVRStatus.rPvrPlay.fgContainTimeStamp;
                        rPvrPlay.fgIgnoreTimeStamp = _rPVRStatus.rPvrPlay.fgIgnoreTimeStamp;
                        rPvrPlay.u2TimeStampFreqDiv = _rPVRStatus.rPvrPlay.u2TimeStampFreqDiv;

                        if(!DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_TIMESTAMP), &rPvrPlay))
{
                            MTPVR_PRINT("%s fail to set timestamp.\n", __FUNCTION__);
                        }
                        
                        SWDMX_PVR_Resume(ucPvrId);       
                    } 
                }
                // io_mtpvr use 100
                // SWDMX_PVR use 1000
                /*
                if(!SWDMX_PVR_SetDataMode(ucPvrId,  _rPVRStatus.i4Speed * 10))
    {
                    MTPVR_PRINT("%s, SWDMX_PVR_SetDataMode fail\n", __FUNCTION__);
    }
                */
                //_MTPVR_Unlock();
}

            _MTPVR_SendData(ucPvrId);
        }
        else if(_rPVRStatus.ePVRStatus == ePVR_STATE_PAUSE)
        {
            x_thread_delay(50);
        }
    }
}
#ifdef PVR_RANDOM_BUF
static VOID _MTPVR_CleanBuf()
{
    UINT32 i;
    for(i=0;i<PVRBUF_NUM;i++)
    {
        _au4WriteAddr[i] = 0;
        _au4WriteAddr[i] = 0;
    }
}
#endif

//-----------------------------------------------------------------------------
/*
 *  Init demux driver
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPVR_Init(unsigned long arg)
{
    UINT32 i = 0;
    FBM_POOL_T *prFbmPool;

    if(!_fgPVRInit)
    {
        _fgPVRInit = TRUE;
        VERIFY(x_thread_create(&_rPVRStatus.hPVRMainThread,
            MTPVR_THREAD_NAME,
            MTPVR_THREAD_STACK_SIZE,
            MTPVR_THREAD_PRIORITY,
            (x_os_thread_main_fct)_MTPVR_MainLoop, sizeof(UINT8), (VOID*)&i) == OSR_OK);

        VERIFY(x_timer_create(&_rPVRStatus.hThumbTimer) == OSR_OK);

        // create play command queue
        VERIFY(x_msg_q_create(&_rPVRStatus.hPVRPlayCmdQ,
            MTPVR_CMD_NAME,
            sizeof(UINT32),
            MTPVR_CMD_Q_SIZE) == OSR_OK);

        VERIFY(x_sema_create(&_hQueueSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_UNLOCK) == OSR_OK);
    
    	prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_PVR2);
    	if(!prFbmPool)
	    {
            MTPVR_PRINT("%s prFbmPool null\n",
                __FUNCTION__);
            ASSERT(0);
            return MTR_NOT_OK;
	    }
        _rPVRStatus.u4FeederAddrPhy = (prFbmPool->u4Addr);
        if((prFbmPool->u4Size == 0) || (prFbmPool->u4Addr == 0))
        {
            MTPVR_PRINT("%s prFbmPool buffer null\n",
                __FUNCTION__);
            ASSERT(0);
            return MTR_NOT_OK;
        }
        _rPVRStatus.u4FeederAddrVir = (VIRTUAL(_rPVRStatus.u4FeederAddrPhy));
        if(_rPVRStatus.u4FeederAddrVir == 0)
        {
            MTPVR_PRINT("%s u4FeederAddrVir null\n",
                __FUNCTION__);
            ASSERT(0);
            return MTR_NOT_OK;
        }
        if(prFbmPool->u4Size <
            (MTPVR_THUMBOUTSIZE + MTPVR_THUMBWORKSIZE + MTPVR_PVRDATASIZE))
        {
            MTPVR_PRINT("%s buffer not enough\n",
                __FUNCTION__);
            ASSERT(0);
            return MTR_NOT_OK;
        }
        _rPVRStatus.u4PvrBaseAddr = (_rPVRStatus.u4FeederAddrVir + MTPVR_PVRDATAOFST);
        _rPVRStatus.u4PvrBufSize = (MTPVR_PVRDATASIZE - _u4InputSize); // the last one is the buffer for wrap around temp buffer

        _MTPVR_CleanBuf();
    
        _rPVRStatus.i4Speed = 100;
        _rPVRStatus.i4SpeedNew = 100;
        MTPVR_PRINT(" - _MTPVR_Init\n");
    }

    return MTR_OK;
}



//-----------------------------------------------------------------------------
/*
 *  Start demux
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPVR_Start (unsigned long arg)
{
    UCHAR ucPvrId;
    DMX_PVR_PLAY_T rPlay;
    //UINT8 i;
    
    ucPvrId = (UCHAR)arg;

    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("%s ucPvrId(%d) > PVR_MAX_ID\n", __FUNCTION__, ucPvrId);
        return MTR_NOT_OK;
    }
    
    if(_rPVRStatus.fgStarted)
    {
        MTPVR_PRINT("%s, already started\n", __FUNCTION__);
        return MTR_OK;
    }

    MTPVR_PRINT("%s\n", __FUNCTION__);
    
    if(!DMX_SelectInputType(DMX_IN_PLAYBACK_TS))
    {
        MTPVR_PRINT("%s, DMX_SelectInputType fail\n", __FUNCTION__);
        return FALSE;
    }
    
    /*
    for(i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        // change each pid index to PVR
        DMX_PID_T rPid;
        DMX_GetPid(i, DMX_PID_FLAG_VALID, &rPid);
        if(rPid.fgEnable == TRUE)
        {
            rPid.eInputType = DMX_IN_PLAYBACK_TS;
            rPid.u1TsIndex = 2;
            DMX_SetPid(i, DMX_PID_FLAG_INPUTTYPE | DMX_PID_FLAG_TS_INDEX, &rPid);
        }
    }
    */

    if(!SWDMX_PVR_Init(ucPvrId))
    {
        MTPVR_PRINT("%s, SWDMX_PVR_Init fail\n", __FUNCTION__);
        return FALSE;
    }
    if(!SWDMX_PVR_OPEN2(ucPvrId, _rPVRStatus.u4PvrBaseAddr, _rPVRStatus.u4PvrBufSize))
    {
        MTPVR_PRINT("%s, SWDMX_PVR_OPEN2 fail\n", __FUNCTION__);
        return FALSE;
    }

    // Make it single move in the begining
    if(!SWDMX_PVR_SetDataMode(ucPvrId, 1))
    {
        MTPVR_PRINT("%s, SWDMX_PVR_SetDataMode fail\n", __FUNCTION__);
    }
    UNUSED(DMX_SetTSMode(ucPvrId, DMX_TSFMT_TIMESHIFT));

    // change to 188 bytes
    if(_fgContainTimeStamp == FALSE)
    {
        rPlay.eMode = DMX_PVR_PLAY_SINGLEMOVE;
        rPlay.fgContainTimeStamp = _fgContainTimeStamp;
        rPlay.fgIgnoreTimeStamp = TRUE;
        rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;

        if(!DMX_PVRPlay_Set((UINT32)(PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP), &rPlay))
        {
            MTPVR_PRINT("%s fail to set timestamp.\n", __FUNCTION__);
        }
    }

    // set pid will call it in io_mtdmx.c
    //_MtdmxSetDecoder();

    _rPVRStatus.u4StartAddr = 0;
    _rPVRStatus.u4ReadAddr = 0;
    _rPVRStatus.u4WriteAddr = 0;
    _rPVRStatus.u4QueuedSize = 0;
    _rPVRStatus.u4EndAddr = _rPVRStatus.u4PvrBufSize;

    _rPVRStatus.fgFirst = TRUE;

    MTPVR_PRINT("%s - OK\n", __FUNCTION__);

    _rPVRStatus.fgStarted = TRUE;
    _MTPVR_SendPlayCmd(ucPvrId, ePVR_STATE_PLAY, 255);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Stop demux
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPVR_Stop(unsigned long arg)
{
    UCHAR ucPvrId;
    
    ucPvrId = (UCHAR)arg;

    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("%s ucPvrId(%d) > PVR_MAX_ID\n", __FUNCTION__, ucPvrId);
        return MTR_NOT_OK;
    }
    
    if(!_rPVRStatus.fgStarted)
    {
        MTPVR_PRINT("%s not started\n", __FUNCTION__);
        return MTR_OK;
    }

    MTPVR_PRINT("%s\n", __FUNCTION__);
    
    _rPVRStatus.fgStarted = FALSE;
    _rPVRStatus.fgSpeedChange = FALSE;
    _rPVRStatus.i4Speed = 100;
    _rPVRStatus.i4SpeedNew = 100;
    _MTPVR_SendPlayCmd(ucPvrId, ePVR_STATE_STOP, 255);
        
    return MTR_OK;
}



MT_RESULT_T _MTPVR_GetAvailSize(UCHAR ucPvrId, UINT32* pu4Addr, UINT32* pu4Size)
{
    if((!pu4Size) || (!pu4Addr))
    {
        MTPVR_PRINT("%s parameter error\n", __FUNCTION__);
        return MTR_NOT_OK;
    }
    if((_rPVRStatus.u4PvrBufSize - _rPVRStatus.u4QueuedSize) > _u4ReservedBufferSize)
    {
        *pu4Addr = _rPVRStatus.u4WriteAddr;
        #if 0
        if((_rPVRStatus.u4EndAddr - _rPVRStatus.u4WriteAddr) <= _u4InputSize)
        {
            *pu4Size = (_rPVRStatus.u4EndAddr - _rPVRStatus.u4WriteAddr);
        }
        else
        #endif
        if((_rPVRStatus.u4PvrBufSize - _rPVRStatus.u4QueuedSize) >= _u4InputSize)
        {
            *pu4Size = _u4InputSize;
        }
        else
        {
            return MTR_BUFFER_FULL;
        }
    }
    else
    {
        return MTR_BUFFER_FULL;
    }
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Get buffer
 *
 *  @param  pu4Addr         Return buffer size
 *  @param  pu4Addr         Return allocated buffer address (Kernel address)
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPVR_GetBuffer(unsigned long arg)
{
    MT_RESULT_T eRet = MTR_NOT_OK;
    MTAL_IOCTL_3ARG_T    rArg;
    UCHAR ucPvrId;
    UINT32 *pu4Size;
    UINT32 *pu4Addr = NULL;
    UINT32 u4Size;
    UINT32 u4Addr;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    ucPvrId = (UCHAR)rArg.ai4Arg[0];
    pu4Addr = (UINT32*)rArg.ai4Arg[1];
    pu4Size = (UINT32*)rArg.ai4Arg[2];

    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("%s ucPvrId(%d) > PVR_MAX_ID\n", __FUNCTION__, ucPvrId);
        return MTR_NOT_OK;
    }
    
    if((!pu4Size) || (!pu4Addr))
    {
        MTPVR_PRINT("%s pu4Size or pu4Addr null\n", __FUNCTION__);
        return MTR_NOT_OK;
    }

    u4Size = 0;
    u4Addr = 0;
    eRet = _MTPVR_GetAvailSize(ucPvrId, &u4Addr, &u4Size);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Size, UINT32);
    COPY_TO_USER_ARG(pu4Size, u4Size, UINT32);
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Addr, UINT32);
    COPY_TO_USER_ARG(pu4Addr, u4Addr, UINT32);

    #if 0
    if(eRet == MTR_OK)
    {
        MTPVR_PRINT("ret(%d) - u4Size = 0x%lx, u4Addr = 0x%lx\n", eRet, *pu4Size, *pu4Addr);
    }
    #endif

    return eRet;
}


//-----------------------------------------------------------------------------
/*
 *  Put buffer
 *
 *  @param  u4PutSize          Buffer size
 *  @param  pu4PutAddr         Buffer address (Kernel address)
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPVR_PutBuffer(unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T    rArg;
    UCHAR ucPvrId;
    UINT32 u4Size;
    UINT32 u4Addr;
    UINT32 u4PutSize;
    UINT32 u4PutAddr;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);


    ucPvrId = (UCHAR)rArg.ai4Arg[0];
    u4PutAddr = (UINT32)rArg.ai4Arg[1];
    u4PutSize = (UINT32)rArg.ai4Arg[2];

    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("%s ucPvrId(%d) > PVR_MAX_ID\n", __FUNCTION__, ucPvrId);
        return MTR_NOT_OK;
    }

    u4Addr = 0;
    eRet = _MTPVR_GetAvailSize(ucPvrId, &u4Addr, &u4Size);
    if(eRet == MTR_OK)
    {
        if(((UINT32)u4PutAddr) != u4Addr)
        {
            MTPVR_PRINT("%s u4PutAddr(%d) != u4Addr(%d)\n",
                __FUNCTION__, u4PutAddr, u4Addr);
            return MTR_NOT_OK;
        }
        if(u4PutSize > _u4InputSize)
        {
            MTPVR_PRINT("%s u4PutSize(%d) > buffer size(%d)\n",
                __FUNCTION__, u4PutSize, _u4InputSize);
            ASSERT(0);
            return MTR_NOT_OK;
        }
        if(((UINT64)_rPVRStatus.u4WriteAddr + (UINT64)u4PutSize) >= (UINT64)0xFFFFFFFF)
        {
            MTPVR_PRINT("%s address incorrect(%d)\n",
                __FUNCTION__, _rPVRStatus.u4WriteAddr);
            ASSERT(0);
            return MTR_NOT_OK;
        }
        if((_rPVRStatus.u4WriteAddr + u4PutSize) >= _rPVRStatus.u4EndAddr)
        {
            x_memcpy((VOID*)(_rPVRStatus.u4StartAddr + _rPVRStatus.u4PvrBaseAddr),
                (VOID*)(_rPVRStatus.u4EndAddr + _rPVRStatus.u4PvrBaseAddr),
                ((_rPVRStatus.u4WriteAddr + u4PutSize) - _rPVRStatus.u4EndAddr));
        }
        if(u4PutSize > u4Size)
        {
            MTPVR_PRINT("%s u4PutSize(%d) > u4Size(%d)n",
                __FUNCTION__, u4PutSize, u4Size);
            return MTR_NOT_OK;
        }
    }
    else
    {
        MTPVR_PRINT("%s _MTPVR_GetAvailSize failn",
            __FUNCTION__);
        return MTR_NOT_OK;
    }
    
    /*
    for(i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        // change each pid index to PVR
        DMX_PID_T rPid;
        DMX_GetPid(i, DMX_PID_FLAG_VALID, &rPid);
        if(rPid.fgEnable == TRUE)
        {
            rPid.eInputType = DMX_IN_PLAYBACK_TS;
            rPid.u1TsIndex = 2;
            DMX_SetPid(i, DMX_PID_FLAG_INPUTTYPE | DMX_PID_FLAG_TS_INDEX, &rPid);
        }
    }
    */
    
    VDEC_SetMMParam(ES0, VDEC_MM_CONTAINER_TYPE, SWDMX_FMT_MPEG2_TS_TIME_SHIFT, 0, 0);

#if 0 // check for error data
    {
        UINT32 u4Value;
        static UINT32 u4PreValue = 0;
        UINT32 u4Parsedsize = 0;
        UINT32 u4Pos = _rPVRStatus.u4WriteAddr;
        static UINT32 u4FilePos = 0;
        UCHAR *pUploadBuffer = (UCHAR*)_rPVRStatus.u4PvrBaseAddr;
        while(u4Parsedsize < u4PutSize)
        {
            if(u4Pos >= _rPVRStatus.u4PvrBufSize)
            {
                 u4Pos -= _rPVRStatus.u4PvrBufSize;
            }
            u4Value = pUploadBuffer[u4Pos];
            u4Value <<= 8;
            u4Value += pUploadBuffer[u4Pos+1];
            u4Value <<= 8;
            u4Value += pUploadBuffer[u4Pos+2];
            u4Value <<= 8;
            u4Value += pUploadBuffer[u4Pos+3];
            if((u4Value - u4PreValue) > 0x10000)
            {
                while(1)
                {
                    if(!u4PreValue)
                        break;
                    Printf("%s Cur(0x%x) Pre(0x%x) u4FilePos(%d)\n", __FUNCTION__, u4Value, u4PreValue, u4FilePos);
                    x_thread_delay(1000);//(1000000);
                    UNUSED(1000);
                }
            }
            u4PreValue = u4Value;
            u4Parsedsize += 192;
            u4FilePos += 192;
            u4Pos += 192;
        }
        Printf("%s Put byte Cur(0x%x) Pre(0x%x) u4FilePos(%d) u4PutSize(%d)\n",  __FUNCTION__, u4Value, u4PreValue, u4FilePos, u4PutSize);
    }
    #endif
    _rPVRStatus.u4WriteAddr += u4PutSize;
    _MTPVR_Lock();
    _rPVRStatus.u4QueuedSize += u4PutSize;
    _MTPVR_Unlock();
    if(_rPVRStatus.u4WriteAddr >= _rPVRStatus.u4EndAddr)
    {
        _rPVRStatus.u4WriteAddr -= (_rPVRStatus.u4EndAddr - _rPVRStatus.u4StartAddr);
    }
    if(_rPVRStatus.fgFirst)
    {
        //LOG(3, "%s(L:%d) PutBuffer\n", __FUNCTION__, __LINE__);
    }
    #if 0
    else if(_rPVRStatus.u4WriteAddr > _rPVRStatus.u4EndAddr)
    {
        MTPVR_PRINT("%s u4WriteAddr(%d) > u4EndAddr(%d)\n",
            __FUNCTION__, _rPVRStatus.u4WriteAddr, _rPVRStatus.u4EndAddr);
        ASSERT(0);
        return MTR_NOT_OK;
    }
    #endif

    #if 0
    MTPVR_PRINT("ret(%d) - u4PutSize = 0x%lx, u4Addr = 0x%lx\n", eRet, u4PutSize, u4Addr);
    #endif

    return eRet;
}

MT_RESULT_T _MTPVR_ChangeThumbVdecId(UCHAR ucVdecId)
{
    UINT32 u4WorkingBufAddr;
    UINT32 u4WorkingBufSize;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    if(_fgThumbnail)
    {
        // stop
        x_memset(&rNfyInfo, 0, sizeof(VDEC_DEC_NFY_INFO_T));
        _MTVDEC_Notify_Lock();
        _u4NotifyMask &= ~MTVDEC_NOTIFY_THUMBNAIL;
        _MTVDEC_Notify_Unlock();
        VDEC_StopVideoThumbnail2(_ucThumbVdecId);

        if(ucVdecId < VDEC_MAX_ES)
        {
            // change vdec id
            _ucThumbVdecId = ucVdecId;
        
            // start
            u4WorkingBufAddr = (_rPVRStatus.u4FeederAddrPhy + MTPVR_THUMBWORKOFST);
            u4WorkingBufSize = MTPVR_THUMBWORKSIZE;
            rNfyInfo.pvTag = (void*)&_aucMTVDECEsId[_ucThumbVdecId];
            rNfyInfo.pfDecNfy = _MTVDEC_VDEC_Nfy;
            _MTVDEC_Notify_Lock();
            _u4NotifyMask |= MTVDEC_NOTIFY_THUMBNAIL;
            _MTVDEC_Notify_Unlock();
            VDEC_SetDecNfy(_ucThumbVdecId, &rNfyInfo);
            VDEC_GetVideoThumbnail2(_ucThumbVdecId, &_rThumbDstInfo,
                u4WorkingBufAddr, u4WorkingBufSize, FALSE);
        }
        else
        {
            MTPVR_PRINT("%s ucVdecId(%d) >= VDEC_MAX_ES\n", __FUNCTION__, ucVdecId);
        }
    }
    if(ucVdecId < VDEC_MAX_ES)
    {
        _ucThumbVdecId = ucVdecId;
    }
    else
    {
        MTPVR_PRINT("%s ucVdecId(%d) >= VDEC_MAX_ES\n", __FUNCTION__, ucVdecId);
        LOG(1, "%s ucVdecId(%d) >= VDEC_MAX_ES\n", __FUNCTION__, ucVdecId);
    }
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Get thumbnail
 *
 *  @param  u4PutSize          Buffer size
 *  @param  pu4PutAddr         Buffer address (Kernel address)
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPVR_GetThumbnail(unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTAL_IOCTL_6ARG_T    rArg;
    UCHAR ucPvrId;
    UCHAR *pDstBuffer;
    //UINT32 u4Size;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4Pitch;
    UINT32 u4AddrOffset;
    UINT32 u4WorkingBufAddr;
    UINT32 u4WorkingBufSize;
    UINT32 u4Line, u4Hpos;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    //VDP_CAPTURE_INTO_T rCapInfo;
    
    USR_SPACE_ACCESS_VALIDATE_6ARG(arg);
    COPY_FROM_USER_6ARG(arg, rArg);

    _u4ThumbnailId ++;

    ucPvrId = (UCHAR)rArg.ai4Arg[0];
    u4Width = (UINT32)rArg.ai4Arg[1];
    u4Height = (UINT32)rArg.ai4Arg[2];
    u4Pitch = (UINT32)rArg.ai4Arg[3];
    u4AddrOffset = (UINT32)rArg.ai4Arg[4];

    LOG(5, "%s(L:%d)\n", __FUNCTION__, __LINE__);
    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("%s ucPvrId(%d) > PVR_MAX_ID\n", __FUNCTION__, ucPvrId);
        return MTR_NOT_OK;
    }

    if((u4Width == 0) || (u4Height == 0) || (u4Pitch == 0))
    {
        MTPVR_PRINT("%s width/height/pitch = 0\n", __FUNCTION__);
        return MTR_NOT_OK;
    }
    if(u4AddrOffset != MTPVR_THUMBOUTOFST)
    {
        MTPVR_PRINT("%s (u4AddrOffset != MTPVR_THUMBOUTOFST)\n", __FUNCTION__);
        return MTR_NOT_OK;
    }
    if((u4Pitch*u4Height*4) > MTPVR_THUMBOUTSIZE)
    {
        MTPVR_PRINT("%s out buffer size not enought, u4Pitch(%d) u4Height(%d)\n",
            __FUNCTION__, u4Pitch, u4Height);
        return MTR_NOT_OK;
    }
    
    u4WorkingBufAddr = (_rPVRStatus.u4FeederAddrPhy + MTPVR_THUMBWORKOFST);
    u4WorkingBufSize = MTPVR_THUMBWORKSIZE;
    if((u4Pitch*u4Height*4) > MTPVR_THUMBWORKSIZE)
    {
        MTPVR_PRINT("%s working buffer size not enought, u4Pitch(%d) u4Height(%d)\n",
            __FUNCTION__, u4Pitch, u4Height);
        return MTR_NOT_OK;
    }

    {
        x_memset((VOID*)VIRTUAL(_rPVRStatus.u4FeederAddrPhy + MTPVR_THUMBOUTOFST),
            0, MTPVR_THUMBOUTSIZE);
        x_memset(&_rThumbDstInfo, 0, sizeof(VDP_THUMBNAIL_INFO_T));
        _rThumbDstInfo.eCanvasColormode = VDP_COLORMODE_ARGB_D8888;
        _rThumbDstInfo.pucCanvasBuffer = (UCHAR*)(_rPVRStatus.u4FeederAddrPhy + MTPVR_THUMBOUTOFST);
        _rThumbDstInfo.u4CanvasHeight = u4Height;
        _rThumbDstInfo.u4CanvasPitch = u4Pitch * 4;
        _rThumbDstInfo.u4CanvasWidth = u4Width * 4;
        _rThumbDstInfo.u4ThumbnailHeight = u4Height;
        _rThumbDstInfo.u4ThumbnailWidth = u4Width;
        _rThumbDstInfo.u4ThumbnailX = 0; // offset
        _rThumbDstInfo.u4ThumbnailY = 0;

        // clean thumbanil
        pDstBuffer = _rThumbDstInfo.pucCanvasBuffer;
    	for(u4Line=0;u4Line<_rThumbDstInfo.u4ThumbnailHeight;u4Line++)
        {
            for(u4Hpos=0;u4Hpos<_rThumbDstInfo.u4ThumbnailWidth;u4Hpos++)
            {
                *(((UINT32 *)((VOID*)VIRTUAL((UINT32)pDstBuffer)+0))+u4Hpos)=0xff000000;
            }
            pDstBuffer+=_rThumbDstInfo.u4CanvasPitch;
        }
    	
        _MTPVR_StopTimer(ucPvrId);
        
        _fgThumbnail = TRUE;
        rNfyInfo.pvTag = (void*)&_ucThumbVdecId;
        rNfyInfo.pfDecNfy = _MTVDEC_VDEC_Nfy;
        _MTVDEC_Notify_Lock();
        _u4NotifyMask |= MTVDEC_NOTIFY_THUMBNAIL;
        _MTVDEC_Notify_Unlock();
        VDEC_SetDecNfy(_ucThumbVdecId, &rNfyInfo);
        VDEC_GetVideoThumbnail2(_ucThumbVdecId, &_rThumbDstInfo,
            u4WorkingBufAddr, u4WorkingBufSize, FALSE);
        
        _MTPVR_StartTimer(ucPvrId, _ucThumbVdecId);
    }

    MTPVR_PRINT(" - Wait VDEC_GetVideoThumbnail2\n");
    LOG(5, "%s(L:%d) return(MTR_OK)\n", __FUNCTION__, __LINE__);

    //MTPVR_PRINT(" - u4Size = 0x%lx, u4Addr = 0x%lx\n", u4Size, u4Addr);
    return eRet;
}

//-----------------------------------------------------------------------------
/*
 *  Stop thumbnail
 *
 *  @param  u4PutSize          Buffer size
 *  @param  pu4PutAddr         Buffer address (Kernel address)
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPVR_StopThumbnail(unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T    rArg;
    UCHAR ucPvrId;
    //UINT32 u4Size;
    //UINT32 u4Addr;
    //UINT32 u4PutSize;
    //UINT32 *pu4PutAddr = NULL;
    //VDEC_DEC_NFY_INFO_T rNfyInfo;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    LOG(5, "%s(L:%d)\n", __FUNCTION__, __LINE__);
    ucPvrId = (UCHAR)rArg.ai4Arg[0];
    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("%s ucPvrId(%d) > PVR_MAX_ID\n", __FUNCTION__, ucPvrId);
        LOG(3, "%s(L:%d) (ucPvrId(%d) >= PVR_MAX_ID)\n", __FUNCTION__, __LINE__, ucPvrId);
        return MTR_NOT_OK;
    }

    _MTPVR_StopTimer(ucPvrId);
        
    _MTVDEC_Notify_Lock();
    _u4NotifyMask &= ~MTVDEC_NOTIFY_THUMBNAIL;
    _MTVDEC_Notify_Unlock();
    VDEC_StopVideoThumbnail2(_ucThumbVdecId);
    if(_fgThumbnail)
    {
        _MTPVR_VDEC_Nfy((void*)&_ucThumbVdecId, VDEC_COND_THUMBNAIL_FAIL, 0, 0);
		_fgThumbnail = FALSE;
    }

    LOG(5, "%s(L:%d) return(MTR_OK)\n", __FUNCTION__, __LINE__);
    //MTPVR_PRINT(" - u4Size = 0x%lx, u4Addr = 0x%lx\n", u4Size, u4Addr);
    return eRet;
}


MT_RESULT_T _MTPVR_SetSpeedExt(UCHAR ucPvrId, INT32 i4Speed, UINT32 u4StepCmd)
{
    MT_RESULT_T eRet = MTR_OK;
    //MTAL_IOCTL_3ARG_T    rArg;
    //DMX_PVR_PLAY_T rPvrPlay;

    ucPvrId = (UCHAR)ucPvrId;
    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("%s ucPvrId(%d) > PVR_MAX_ID\n", __FUNCTION__, ucPvrId);
        return MTR_NOT_OK;
    }


    if(u4StepCmd == (UINT32)ePVR_STEP_START)     // step start
    {
        _rPVRStatus.fgSpeedChange = TRUE;
        _rPVRStatus.i4SpeedNew = 25;
        _rPVRStatus.ePVRStep = ePVR_STEP_START;
        _MTPVR_SendPlayCmd(ucPvrId, ePVR_STATE_PLAY, 255);
    }
    else if(u4StepCmd == (UINT32)ePVR_STEP_DONE) // step done
    {
        if(_rPVRStatus.ePVRStep != ePVR_STEP_START)
        {
            // do nothing
            return eRet;
        }
        _rPVRStatus.ePVRStep = ePVR_STEP_DONE;
        _MTPVR_SendPlayCmd(ucPvrId, ePVR_STATE_PAUSE, 254);
        // higher priority, so AP speed would be handle later.
        // our final speed would be the one AP set.
    }
    else if(u4StepCmd == (UINT32)ePVR_STEP_END)  // step end
    {
        _rPVRStatus.ePVRStep = ePVR_STEP_DONTCARE;
        _MTPVR_SendPlayCmd(ucPvrId, ePVR_STATE_PLAY, 254);
        // higher priority, so AP speed would be handle later.
        // our final speed would be the one AP set.
    }
    else if(i4Speed == 0)
    {
        _MTPVR_SendPlayCmd(ucPvrId, ePVR_STATE_PAUSE, 255);
    }
    else if((_rPVRStatus.ePVRStatus == ePVR_STATE_PAUSE) ||
            (_rPVRStatus.ePVRStatus == ePVR_STATE_PLAY))
    {
        _rPVRStatus.fgSpeedChange = TRUE;
        _rPVRStatus.i4SpeedNew = i4Speed;
        _MTPVR_SendPlayCmd(ucPvrId, ePVR_STATE_PLAY, 255);
    }
    else
    {
        _rPVRStatus.fgSpeedChange = TRUE;
        _rPVRStatus.i4SpeedNew = i4Speed;
    }

    return eRet;
}
//-----------------------------------------------------------------------------
/*
 *  Set speed
 *
 *  @param  u4Speed            Speed
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPVR_SetSpeed(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UCHAR ucPvrId;
    INT32 i4Speed;
    //DMX_PVR_PLAY_T rPlay;
    static UINT8 u1StopAudio=0;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    ucPvrId = (UCHAR)rArg.ai4Arg[0];
    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("%s ucPvrId(%d) > PVR_MAX_ID\n", __FUNCTION__, ucPvrId);
        return MTR_NOT_OK;
    }
    i4Speed = (INT32)rArg.ai4Arg[1];

    if(i4Speed != 100 && u1StopAudio != 1)
    {
        _MTAUD_Stop(1);
        u1StopAudio = 1;
    }
    else if(u1StopAudio==1)
    {
        _MTAUD_Play(1);
        u1StopAudio = 0;
    }

    return _MTPVR_SetSpeedExt(ucPvrId, i4Speed, (UINT32)ePVR_STEP_DONTCARE);
}

//-----------------------------------------------------------------------------
/*
 *  _MTPVR_ResetBuffer
 *
 *  @param  u4Speed            Speed
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPVR_ResetBuffer(unsigned long arg)
{
    UCHAR ucPvrId;

    ucPvrId = (UCHAR)arg;

    if(ucPvrId >= PVR_MAX_ID)
    {
        MTPVR_PRINT("%s ucPvrId(%d) > PVR_MAX_ID\n", __FUNCTION__, ucPvrId);
        return MTR_NOT_OK;
    }

    if(!_rPVRStatus.fgStarted)
    {
        MTPVR_PRINT("%s not started\n", __FUNCTION__);
        return MTR_OK;
    }

    MTPVR_PRINT("%s\n", __FUNCTION__);

    _MTPVR_SendPlayCmd(ucPvrId, ePVR_CMD_FLUSH, 255);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int mtal_ioctl_mtpvr(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    INT32 i4Ret = MTR_OK;

    switch (cmd)
    {
    case MTAL_IO_PVR_INIT:
        i4Ret = _MTPVR_Init(arg);
        break;

    case MTAL_IO_PVR_START:
        i4Ret = _MTPVR_Start(arg);
        break;

    case MTAL_IO_PVR_STOP:
        i4Ret = _MTPVR_Stop(arg);
        break;

    case MTAL_IO_PVR_GETBUFFER:
        i4Ret = _MTPVR_GetBuffer(arg);
        break;

    case MTAL_IO_PVR_PUTBUFFER:
        i4Ret = _MTPVR_PutBuffer(arg);
        break;

    case MTAL_IO_PVR_GETTHUMBNAIL:
        i4Ret = _MTPVR_GetThumbnail(arg);
        break;
        
    case MTAL_IO_PVR_STOPTHUMBNAIL:
        i4Ret = _MTPVR_StopThumbnail(arg);
        break;
        
    case MTAL_IO_PVR_SETSPEED:
        i4Ret = _MTPVR_SetSpeed(arg);
        break;

    case MTAL_IO_PVR_RESETBUFFER:
        i4Ret = _MTPVR_ResetBuffer(arg);
        break;
        
    default:
        break;
    }

    //printf("pvr: 0x%x ret: %d\n", cmd, i4Ret);

    return i4Ret;
}




