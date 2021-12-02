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
 * $RCSfile: io_mtvdec.c,v $
 * $Revision: #7 $
 *---------------------------------------------------------------------------*/

/** @file mtvdec.c
 *  This file contains implementation of exported APIs of Video Decoder (VDEC) module,
 *  which inclued MPEG 1/2/4, H.264(AVC), and VC-1.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtvdec.h"
// 20110801
#include "mtpvr.h"
#include "vdec_if.h"
#include "vdec_drvif.h"
#include "mpv_if.h"
#include "vdp_if.h"
#include "dmx_if.h"
#include "stc_drvif.h"
#include "cb_data.h"
#include "cb_low.h"
#include "x_os.h"
#include "x_assert.h"
#include <linux/mtal_ioctl.h>
#include "x_bim.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MTDECODE_CANVAS_SIZE (8*1024*1024)

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MAX_USERDATA_TMPSIZE       256
// Maximum Elementary Stream number
//#define VDEC_MAX_ES              2
// Use MsgQ timeout
//#define IFRAME_MSGQ_TIMEOUT
#define IFRAME_TIMEOUT              5000

#define MTVDEC_TIMEOUT    0
#define MTVDEC_FINISH     1
#define MTVDEC_ERROR      2

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define MTVDEC_PRINT_INFO(rVdecInfo)                       \
    do{\
        if(IS_MTVDEC_LOG_ENABLE)\
        {                                        \
            printf("rVdecInfo.u2Height= %d\n",rVdecInfo.u2Height);\
            printf("rVdecInfo.u2Width= %d\n",rVdecInfo.u2Width);\
            printf("rVdecInfo.i2DHS= %d\n",rVdecInfo.i2DHS);\
            printf("rVdecInfo.i2DVS= %d\n",rVdecInfo.i2DVS);\
            printf("rVdecInfo.u2FrmRate= %d\n",rVdecInfo.u2FrmRate);\
            printf("rVdecInfo.eAspectRatio= %d\n",rVdecInfo.eAspectRatio);\
            printf("rVdecInfo.u4BitRate= %d\n",(int)rVdecInfo.u4BitRate);\
            printf("rVdecInfo.ucAfd= %d\n",rVdecInfo.ucAfd);\
            printf("rVdecInfo.fgProgressiveSeq = %d\n",rVdecInfo.fgProgressiveSeq );\
         }\
    }while(0)
#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }
// 20110801
#define USR_SPACE_ACCESS_VALIDATE_6ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_6ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_6ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_6ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }
    
#define USR_SPACE_ACCESS_VALIDATE_8ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_8ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_8ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define MTVDEC_NOTIFY_IFRAME        (1<<0)
#define MTVDEC_NOTIFY_THUMBNAIL     (1<<1)
#define MTVDEC_NOTIFY_DECODE_IFRAME (1<<2)  // 20110801

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT8    _au1UserDataTmpBuf[MAX_USERDATA_TMPSIZE];
static BOOL     _bPlayingIframe = 0;
UINT32   _u4NotifyMask = 0;
static BOOL _fgNotifyLocking = FALSE;
static CRIT_STATE_T _rNotifyLock;
static VDP_THUMBNAIL_INFO_T _rDecodeIFrameDstInfo;

#ifdef IFRAME_MSGQ_TIMEOUT
static HANDLE_T _hMtvdecTimer = 0;
static HANDLE_T _hMtvdecMsgQ = 0;
#else
static UINT32 _u4IFrameResult = 0;
static HANDLE_T _hIframeSema = 0;
#endif

UCHAR _aucMTVDECEsId[VDEC_MAX_ES] = {0};
static HANDLE_T _ahMTVDECVdecCtrlSema[VDEC_MAX_ES] = {0};

extern MT_RESULT_T _MTPVR_SetSpeedExt(UCHAR ucPvrId, INT32 i4Speed, UINT32 u4StepCmd);
extern void _MTPVR_VDEC_Nfy(
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2);
#ifdef TIME_SHIFT_SUPPORT
extern BOOL _MTDREC_IsLivePidEnable(VOID);
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void _MTVDEC_Notify_Lock(void)
{
    _rNotifyLock = x_crit_start();

    ASSERT(!_fgNotifyLocking);
    _fgNotifyLocking = TRUE;

    UNUSED(_fgNotifyLocking);          // Make Lint happy
}
void _MTVDEC_Notify_Unlock(void)
{
    ASSERT(_fgNotifyLocking);
    _fgNotifyLocking = FALSE;

    x_crit_end(_rNotifyLock);

    UNUSED(_fgNotifyLocking);          // Make Lint happy
}

#ifdef IFRAME_MSGQ_TIMEOUT
static void _MTVDEC_FlushDecMsgQ()
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize;
    
    zMsgSize = sizeof(INT32);   
    
    while (x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &_hMtvdecMsgQ, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }
    
    VERIFY(x_msg_q_num_msgs(_hMtvdecMsgQ, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);    
}


void _MTVDEC_TimerTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UINT32 u4DecFin;
    INT32 i4Ret;    
 
    if(_hMtvdecMsgQ == 0)
    {
        // MM playback detect PS case, i.e. MPV_Init() is not called.
        return;
    }
    
    u4DecFin = MTVDEC_TIMEOUT;
    i4Ret = x_msg_q_send(_hMtvdecMsgQ, (void *)&u4DecFin, sizeof(UINT32), 255);
  
    if ((i4Ret != OSR_OK)&&(i4Ret != OSR_TOO_MANY))
    {
        ASSERT(0);
    }
    
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);
}
#endif

static void _MTVDEC_CC_Nfy(DGUSERDATA_CB_T* prDecCcData)
{
    _CB_PutEvent(CB_MTAL_VDEC_USRDATA_TRIGGER, sizeof(DGUSERDATA_CB_T), (void*)prDecCcData);

}


static void _MTVDEC_ERR_Nfy(DECERR_CB_T* prDecErr)
{
    _CB_PutEvent(CB_MTAL_VDEC_DECERR_TRIGGER, sizeof(DECERR_CB_T), (void*)prDecErr);

}

static void _MTVDEC_PIC_TYPE_Nfy(PICTYPE_CB_T* prPicType)
{
    _CB_PutEvent(CB_MTAL_PIC_TYPE_TRIGGER, sizeof(PICTYPE_CB_T), (void*)prPicType);
}


static void _MTVDEC_STATUS_INT_Nfy(DEC_STATUS_CB_T* prStatus)
{
    _CB_PutEvent(CB_MTAL_VDEC_STATUS_TRIGGER, sizeof(DEC_STATUS_CB_T), (void*)prStatus);
}

static void _MTVDEC_VDEC_Iframe_Nfy(
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2)
{
    #ifdef IFRAME_MSGQ_TIMEOUT
    UINT32 u4DecFin = 0;
    INT32 i4Ret;
    #endif
    
    MTVDEC_PRINT("_MTVDEC_VDEC_Nfy\n");
    
    if((eNfyCond == VDEC_COND_I_FRAME_SET_BUFF_DONE) ||
        (eNfyCond == VDEC_COND_I_FRAME_DONE))
    {
        MTVDEC_PRINT(" - Vdec job done\n");
        #ifdef IFRAME_MSGQ_TIMEOUT
        u4DecFin = MTVDEC_FINISH;
        i4Ret = x_msg_q_send(_hMtvdecMsgQ, (void *)&u4DecFin, sizeof(UINT32), 255);

        if ((i4Ret != OSR_OK)&&(i4Ret != OSR_TOO_MANY))
        {
            MTVDEC_PRINT(" - Vdec job done OSR_TOO_MANY\n");
            //ASSERT(0);
        }
        #else
        _u4IFrameResult = MTVDEC_FINISH;
        VERIFY(x_sema_unlock(_hIframeSema) == OSR_OK);
        #endif
    }
    else if(eNfyCond == VDEC_COND_ERROR)
    {
        MTVDEC_PRINT(" - Vdec job error\n");
        #ifdef IFRAME_MSGQ_TIMEOUT
        u4DecFin = MTVDEC_ERROR;
        i4Ret = x_msg_q_send(_hMtvdecMsgQ, (void *)&u4DecFin, sizeof(UINT32), 255);

        if ((i4Ret != OSR_OK)&&(i4Ret != OSR_TOO_MANY))
        {
            MTVDEC_PRINT(" - Vdec job error OSR_TOO_MANY\n");
            //ASSERT(0);
        }
        #else
        _u4IFrameResult = MTVDEC_ERROR;
        VERIFY(x_sema_unlock(_hIframeSema) == OSR_OK);
        #endif
    }
}

static void _MTVDEC_VDEC_Decode_Iframe_Nfy(
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2)
{

    #ifdef IFRAME_MSGQ_TIMEOUT
    UINT32 u4DecFin = 0;
    INT32 i4Ret;
    #endif

    MTVDEC_PRINT("_MTVDEC_VDEC_Nfy\n");

    if((eNfyCond == VDEC_COND_I_FRAME_SET_BUFF_DONE) ||
        (eNfyCond == VDEC_COND_I_FRAME_DONE || eNfyCond == VDEC_COND_THUMBNAIL_DONE))
    {
        MTVDEC_PRINT(" - Vdec job done\n");
        #ifdef IFRAME_MSGQ_TIMEOUT
        u4DecFin = MTVDEC_FINISH;
        i4Ret = x_msg_q_send(_hMtvdecMsgQ, (void *)&u4DecFin, sizeof(UINT32), 255);

        if ((i4Ret != OSR_OK)&&(i4Ret != OSR_TOO_MANY))
        {
            MTVDEC_PRINT(" - Vdec job done OSR_TOO_MANY\n");
            //ASSERT(0);
        }
        #else
        _u4IFrameResult = MTVDEC_FINISH;
        VERIFY(x_sema_unlock(_hIframeSema) == OSR_OK);
        #endif
    }
    else if(eNfyCond == VDEC_COND_ERROR)
    {
        MTVDEC_PRINT(" - Vdec job error\n");
        #ifdef IFRAME_MSGQ_TIMEOUT
        u4DecFin = MTVDEC_ERROR;
        i4Ret = x_msg_q_send(_hMtvdecMsgQ, (void *)&u4DecFin, sizeof(UINT32), 255);

        if ((i4Ret != OSR_OK)&&(i4Ret != OSR_TOO_MANY))
        {
            MTVDEC_PRINT(" - Vdec job error OSR_TOO_MANY\n");
            //ASSERT(0);
        }
        #else
        _u4IFrameResult = MTVDEC_ERROR;
        VERIFY(x_sema_unlock(_hIframeSema) == OSR_OK);
        #endif
    }
}

void _MTVDEC_VDEC_Decode_Thumb_Nfy(
    UINT32 u4Arg1,
    UINT32 u4Arg2,
    UINT32 u4Arg3,
    UINT32 u4Arg4)
{

#ifdef IFRAME_MSGQ_TIMEOUT
    UINT32 u4DecFin = 0;
#endif
    if(u4Arg2 == VDEC_COND_THUMBNAIL_DONE)
    {
        MTVDEC_PRINT(" - Vdec job done\n");
        _u4IFrameResult = MTVDEC_FINISH;
#ifdef IFRAME_MSGQ_TIMEOUT
        u4DecFin = MTVDEC_FINISH;
        VERIFY(x_msg_q_send(_hMtvdecMsgQ, (void *)&u4DecFin, sizeof(UINT32), 1) == OSR_OK);
#else
        VERIFY(x_sema_unlock(_hIframeSema) == OSR_OK);
#endif
    }
    else
    {
        _u4IFrameResult = MTVDEC_ERROR;
#ifdef IFRAME_MSGQ_TIMEOUT
        u4DecFin = MTVDEC_ERROR;
        VERIFY(x_msg_q_send(_hMtvdecMsgQ, (void *)&u4DecFin, sizeof(UINT32), 1) == OSR_OK);
#else
        VERIFY (x_sema_unlock(_hIframeSema) == OSR_OK);
#endif
    }    
}

void _MTVDEC_VDEC_Nfy(
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2)
{
    UCHAR ucEsId = VDEC_MAX_ES;
    if(pvNfyTag > 0)
    {
        ucEsId= *((UCHAR*)pvNfyTag);
    }
    else
	{
        printf("pvNfyTag = 0\n");
	}
    if (eNfyCond == VDEC_COND_CTRL_DONE)
    {
        if((ucEsId < VDEC_MAX_ES) && (_ahMTVDECVdecCtrlSema[ucEsId]))
        {
            VERIFY (x_sema_unlock(_ahMTVDECVdecCtrlSema[ucEsId]) == OSR_OK);
        }
        else
        {
            MTVDEC_PRINT("ucEsId(%d) >= VDEC_MAX_ES\n", ucEsId);
        }
    }
    if(_u4NotifyMask | MTVDEC_NOTIFY_IFRAME)
    {
        _MTVDEC_VDEC_Iframe_Nfy(pvNfyTag, eNfyCond, u4Data1, u4Data2);
    }
    if(_u4NotifyMask | MTVDEC_NOTIFY_THUMBNAIL)
    {
        _MTPVR_VDEC_Nfy(pvNfyTag, eNfyCond, u4Data1, u4Data2);
    }
    // 20110801
    if(_u4NotifyMask | MTVDEC_NOTIFY_DECODE_IFRAME)
    {
         //TBD
         _MTVDEC_VDEC_Decode_Iframe_Nfy(pvNfyTag, eNfyCond, u4Data1, u4Data2);
    }
}

static MT_RESULT_T _MTVDEC_Init(void)
{
    UINT32 i;
    MTVDEC_PRINT(" - void\n");
    //add driver implementation code here
	x_os_drv_set_timestamp("_MTVDEC_Init start");

#ifdef IFRAME_MSGQ_TIMEOUT
    if(_hMtvdecMsgQ == 0)
    {
        VERIFY(x_msg_q_create(&_hMtvdecMsgQ, "MTVDec-Q", sizeof(UINT32), 5) == OSR_OK);
    }
    if(_hMtvdecTimer == 0)
    {
        VERIFY(x_timer_create(&_hMtvdecTimer) == OSR_OK);
    }
#else
    if(_hIframeSema == 0)
    {
        VERIFY(x_sema_create(&_hIframeSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK) == OSR_OK);
    }
#endif
    for(i=0;i<VDEC_MAX_ES;i++)
    {
        if(_ahMTVDECVdecCtrlSema[i] == 0)
        {
            VERIFY(x_sema_create(&_ahMTVDECVdecCtrlSema[i],
                             X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        }
        _aucMTVDECEsId[i] = i;
    }

    VDEC_Init();
    
	x_os_drv_set_timestamp("_MTVDEC_Init end");
    return MTR_OK;
}


static MT_RESULT_T _MTVDEC_Termint(void)
{
    MTVDEC_PRINT(" - void\n");
    //add driver implementation code here
    VDEC_Termint();
    
    return MTR_OK;
}


static MT_RESULT_T _MTVDEC_Reset(void)
{
    MTVDEC_PRINT(" - void\n");
    //add driver implementation code here
    return MTR_OK;
}


static MT_RESULT_T _MTVDEC_Play(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    MTVDEC_FMT_T eMtVdecFmt;
    ENUM_VDEC_FMT_T eVdecFmt;
    UCHAR ucEsId;
    BOOL fgRet;
    #ifdef CC_KR_3DDTV
    UINT8 u1Idx;
    DMX_PID_T rPid;
    #endif
	
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    eMtVdecFmt = (MTVDEC_FMT_T)rArg.ai4Arg[1];

    MTVDEC_PRINT(" - ucEsId = %d, eMtVdecFmt = %d\n",
		(int)ucEsId,
		(int)eMtVdecFmt);

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }

    #if 0//need it?
    // set to normal speed
    VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1X);
    VDP_SetFBNoTimeout(B2R_1, FALSE);
    VDP_SetStepForward(B2R_1, VDP_PLAY_NORMAL);
    VDP_SetPauseMm(B2R_1, FALSE);
    VDP_SetTrickMode(B2R_1,VDP_PLAY_NORMAL);
	#endif

    eVdecFmt = VDEC_FMT_MAX;
    if (eMtVdecFmt == MTVDEC_FMT_MPEG1_MPEG2)
    {
        eVdecFmt = VDEC_FMT_MPV;
        DMX_SetVideoType(DMX_VIDEO_MPEG);
        
        #ifdef CC_KR_3DDTV
        for (u1Idx = 0; u1Idx < DMX_NUM_PID_INDEX; u1Idx++)
        {
            // Get PID info
            if (DMX_GetPid(u1Idx, DMX_PID_FLAG_BUFFER, &rPid))
            {
                if((rPid.u1ChannelId == ucEsId) && (rPid.ePidType == DMX_PID_TYPE_ES_VIDEO))
                {
                    DMX_SetVideoTypeByPidx(u1Idx, DMX_VIDEO_MPEG);
                    break;
                }
            }
        }
        #endif
    }
    else if (eMtVdecFmt == MTVDEC_FMT_H264)
    {
        eVdecFmt = VDEC_FMT_H264;
        DMX_SetVideoType(DMX_VIDEO_H264);    
        
        #ifdef CC_KR_3DDTV
        for (u1Idx = 0; u1Idx < DMX_NUM_PID_INDEX; u1Idx++)
        {
            // Get PID info
            if (DMX_GetPid(u1Idx, DMX_PID_FLAG_BUFFER, &rPid))
            {
                if((rPid.u1ChannelId == ucEsId) && (rPid.ePidType == DMX_PID_TYPE_ES_VIDEO))
                {
                    DMX_SetVideoTypeByPidx(u1Idx, DMX_VIDEO_H264);
                    break;
                }
            }
        }
        #endif
    }
    else if (eMtVdecFmt == MTVDEC_FMT_MPEG4)
    {
        eVdecFmt = VDEC_FMT_MP4;
    }
    else if (eMtVdecFmt == MTVDEC_FMT_VC1)
    {
        eVdecFmt = VDEC_FMT_WMV;
    }
    else if (eMtVdecFmt == MTVDEC_FMT_RM)
    {
        eVdecFmt = VDEC_FMT_RV; 
    }
    else if (eMtVdecFmt == MTVDEC_FMT_MJPEG)
    {
        eVdecFmt = VDEC_FMT_MJPEG;
    }
    else if (eMtVdecFmt == MTVDEC_FMT_AVS)
    {
        eVdecFmt = VDEC_FMT_AVS;
        DMX_SetVideoType(DMX_VIDEO_AVS);
    }
	else if(eMtVdecFmt==MTVDEC_FMT_VP8)
	{
        eVdecFmt = VDEC_FMT_VP8;
	}
	else if(eMtVdecFmt==MTVDEC_FMT_VP6)
	{
        eVdecFmt = VDEC_FMT_VP6;
	}
    
    fgRet = VDEC_Play(ucEsId, eVdecFmt);

    #ifdef CC_KR_3DDTV_HARDCODE
    if(ucEsId == ES1)
    {
        VDEC_SetMMParam(0, VDEC_MM_3D_INFO, VDEC_3D_DUAL_STREAM, 0, 0);
        VDEC_SetMMParam(1, VDEC_MM_3D_INFO, VDEC_3D_DUAL_STREAM, 0, 0);
    }
    #endif
    
    if (fgRet)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;    
    }
}


static MT_RESULT_T _MTVDEC_PlayIFrame(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T    rArg;
    MTVDEC_FMT_T eVdecFmt;
    UCHAR ucEsId;
    //BOOL fgRet;
    UCHAR *pucData;
    UINT32 u4Size;
    INT32 i4Ret;
    MPV_DEC_I_FRAME_BUFF_T rMpvBuf;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    FBM_POOL_T *prFbmPool;
	
    #ifdef IFRAME_MSGQ_TIMEOUT
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize;
    #endif
    
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    eVdecFmt = (MTVDEC_FMT_T)rArg.ai4Arg[1];
    pucData = (UCHAR*)rArg.ai4Arg[2];
    u4Size = (UINT32)rArg.ai4Arg[3];

    MTVDEC_PRINT(" - ucEsId = %d, eVdecFmt = %d\n",
		(int)ucEsId,
		(int)eVdecFmt);

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        MTVDEC_PRINT(" - ucEsId = %d, _MTVDEC_PlayIFrame fail\n",(int)ucEsId);
        return MTR_NOT_SUPPORTED;
    }
    #if 0//need it?
    // set to normal speed
    VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1X);
    VDP_SetFBNoTimeout(B2R_1, FALSE);
    VDP_SetStepForward(B2R_1, VDP_PLAY_NORMAL);
    VDP_SetPauseMm(B2R_1, FALSE);
    VDP_SetTrickMode(B2R_1,VDP_PLAY_NORMAL);
    #endif

    _bPlayingIframe = TRUE;

    rNfyInfo.pvTag = &_aucMTVDECEsId[ucEsId];
    rNfyInfo.pfDecNfy = _MTVDEC_VDEC_Nfy;
    _MTVDEC_Notify_Lock();
    _u4NotifyMask |= MTVDEC_NOTIFY_IFRAME;
    _MTVDEC_Notify_Unlock();
    VDEC_SetDecNfy(ucEsId, &rNfyInfo);
    //VDEC_SetDecNfy(0, _MTVDEC_VDEC_Nfy);
    
    if(eVdecFmt == MTVDEC_FMT_MPEG1_MPEG2)
    {
        VDEC_Play_I_Frame(ucEsId, VDEC_FMT_MPV);
    }
    else if(eVdecFmt == MTVDEC_FMT_H264)
    {
        VDEC_Play_I_Frame(ucEsId, VDEC_FMT_H264);
    }
    else
    {
        MTVDEC_PRINT(" - eVdecFmt = %d, not support\n",(int)eVdecFmt);
        return MTR_NOT_SUPPORTED;
    }

    //MPV_AllocIFrameBuf();
	prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_FEEDER);
    rMpvBuf.pucIFrameBufAddr = (UCHAR*)(prFbmPool->u4Addr + pucData);
    rMpvBuf.u4Size = u4Size;

    // set write pointer
    VDEC_SetWptr(ucEsId, (UINT32)(rMpvBuf.pucIFrameBufAddr + rMpvBuf.u4Size));
    
    // clear lock
    #ifdef IFRAME_MSGQ_TIMEOUT
    _MTVDEC_FlushDecMsgQ();
    #else
    _u4IFrameResult = MTVDEC_TIMEOUT;
    x_sema_lock(_hIframeSema, X_SEMA_OPTION_NOWAIT);
    #endif

    // parsing
    MTVDEC_PRINT(" - MPV_SetIFrameBuf\n");
    i4Ret = MPV_SetIFrameBuf(ucEsId, &rMpvBuf);
    
    if(i4Ret != MPV_DRV_OK)
    {
        MTVDEC_PRINT(" - ucEsId = %d, MPV_SetIFrameBuf fail, 0x%x 0x%x 0x%x 0x%x\n",
            (int)ucEsId,
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+1))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+2))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+3))));
        return MTR_NOT_OK;
    }

    MTVDEC_PRINT(" - Wait Vdec parsing\n");
    
    #ifdef IFRAME_MSGQ_TIMEOUT
    VERIFY(x_timer_start(_hMtvdecTimer, IFRAME_TIMEOUT, X_TIMER_FLAG_ONCE,
            _MTVDEC_TimerTimeout, (void*)(UINT32)0) == OSR_OK);
    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &(_hMtvdecMsgQ), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
    VERIFY(x_timer_stop(_hMtvdecTimer) == OSR_OK);    
    
    if (u4DecFin != MTVDEC_FINISH)
    {
        MTVDEC_PRINT(" - Wait Vdec parsing error, 0x%x 0x%x 0x%x 0x%x\n",
            (int)u4DecFin,
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+1))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+2))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+3))));
        return MTR_NOT_OK;
    }
    #else
    if(x_sema_lock_timeout(_hIframeSema, IFRAME_TIMEOUT) != OSR_OK)
    {
        MTVDEC_PRINT(" - Wait Vdec parsing timeout, 0x%x 0x%x 0x%x 0x%x\n",
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+1))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+2))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+3))));
        return MTR_NOT_OK;
    }
    if(_u4IFrameResult != MTVDEC_FINISH)
    {
        MTVDEC_PRINT(" - Wait Vdec parsing error %d, 0x%x 0x%x 0x%x 0x%x\n",
            (int)_u4IFrameResult,
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+1))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+2))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+3))));
        return MTR_NOT_OK;
    }
    #endif

    if(!_bPlayingIframe)
    {
        MTVDEC_PRINT(" - _bPlayingIframe 1 FALSE\n");
        return MTR_DONT_CARE;
    }

    // Todo: get resolution and set backend
    
    // decoding
    MTVDEC_PRINT(" - MPV_SetIFrameDec\n");
    i4Ret = MPV_SetIFrameDec(ucEsId, &rMpvBuf);
    
    if(i4Ret != MPV_DRV_OK)
    {
        MTVDEC_PRINT(" - ucEsId = %d, MPV_SetIFrameDec fail\n",(int)ucEsId);
        return MTR_NOT_OK;
    }
    
    MTVDEC_PRINT(" - Wait Vdec decoding\n");
    #ifdef IFRAME_MSGQ_TIMEOUT
    VERIFY(x_timer_start(_hMtvdecTimer, IFRAME_TIMEOUT, X_TIMER_FLAG_ONCE,
            _MTVDEC_TimerTimeout, (void*)(UINT32)0) == OSR_OK);
    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &(_hMtvdecMsgQ), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
    VERIFY(x_timer_stop(_hMtvdecTimer) == OSR_OK);   
    
    if (u4DecFin != MTVDEC_FINISH)
    {
        MTVDEC_PRINT(" - Wait Vdec parsing error %d\n", u4DecFin);
        return MTR_NOT_OK;
    }
    #else
    if(x_sema_lock_timeout(_hIframeSema, IFRAME_TIMEOUT) != OSR_OK)
    {
        MTVDEC_PRINT(" - Wait Vdec decoding timeout\n");
        return MTR_NOT_OK;
    }
    if(_u4IFrameResult != MTVDEC_FINISH)
    {
        MTVDEC_PRINT(" - Wait Vdec parsing error %d\n", (int)_u4IFrameResult);
        return MTR_NOT_OK;
    }
    #endif
    
    if(!_bPlayingIframe)
    {
        MTVDEC_PRINT(" - _bPlayingIframe 2 FALSE\n");
        return MTR_DONT_CARE;
    }
    
    //MPV_FreeIFrameBuf();
    MTVDEC_PRINT(" - _MTVDEC_PlayIFrame return\n");

    return MTR_OK;
}

static MT_RESULT_T _MTVDEC_DecodeIFrame(unsigned long arg)
{
    MTAL_IOCTL_6ARG_T    rArg;
    INT32 i4Ret;
    
    UCHAR ucEsId;
    MTVDEC_FMT_T eVdecFmt;
    UCHAR *pucData;
    UINT32 u4Size;
    UINT32 *pu4DestSize;
    UINT32 u4DestSize;
    UINT32 *pu4Width;

    UCHAR *pDstBuffer;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4Pitch;
    UINT32 u4Line, u4Hpos;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    VDEC_HDR_INFO_T rHdrInfo;

    // fb buffer  
    MPV_DEC_I_FRAME_BUFF_T rMpvBuf;
    FBM_POOL_T *prFbmPool_input;
    FBM_POOL_T *prFbmPool_output;
    UINT32 u4OutputFbgAddr;
    UINT32 u4InputFbgAddr;
    
    #ifdef IFRAME_MSGQ_TIMEOUT
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize;
    #endif

    USR_SPACE_ACCESS_VALIDATE_6ARG(arg);
    COPY_FROM_USER_6ARG(arg, rArg);

    // parameters translation
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    eVdecFmt = (MTVDEC_FMT_T)rArg.ai4Arg[1];
    pucData = (UCHAR*)rArg.ai4Arg[2];
    u4Size = (UINT32)rArg.ai4Arg[3];
    pu4DestSize = (UINT32*)rArg.ai4Arg[4];
    pu4Width = (UINT32*)rArg.ai4Arg[5];

    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }

    rNfyInfo.pvTag = &_aucMTVDECEsId[ucEsId];
    rNfyInfo.pfDecNfy = _MTVDEC_VDEC_Nfy;

    
    // set notify bit mask
    VDEC_RegEventNotifyFunc(_MTVDEC_VDEC_Decode_Thumb_Nfy);
    _MTVDEC_Notify_Lock();
    _u4NotifyMask |= MTVDEC_NOTIFY_DECODE_IFRAME;
    _MTVDEC_Notify_Unlock();
    VDEC_SetDecNfy(ucEsId, &rNfyInfo);
    
    // disable display,
    VDEC_PlayNotDisplay(ucEsId,TRUE);

    if(eVdecFmt == MTVDEC_FMT_MPEG1_MPEG2)
    {
        VDEC_Play_I_Frame(ucEsId, VDEC_FMT_MPV);
    }
    else if(eVdecFmt == MTVDEC_FMT_H264)
    {
        VDEC_Play_I_Frame(ucEsId, VDEC_FMT_H264);
    }
    else
    {
        MTVDEC_PRINT(" - eVdecFmt = %d, not support\n",(int)eVdecFmt);
        printf(" - eVdecFmt = %d, not support\n");
        return MTR_NOT_SUPPORTED;
    }

    // input buffer
    prFbmPool_input = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_PVR2);
    
    // output buffer
    prFbmPool_output = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG);

    u4InputFbgAddr = (UINT32)((prFbmPool_input->u4Addr)) ;  
    rMpvBuf.pucIFrameBufAddr = (UCHAR*)(u4InputFbgAddr); 
    rMpvBuf.u4Size = u4Size;

    // set write pointer
    VDEC_SetWptr(ucEsId, (UINT32)(rMpvBuf.pucIFrameBufAddr + rMpvBuf.u4Size));

    // clear lock
    #ifdef IFRAME_MSGQ_TIMEOUT
    _MTVDEC_FlushDecMsgQ();
    #else
    _u4IFrameResult = MTVDEC_TIMEOUT;
    x_sema_lock(_hIframeSema, X_SEMA_OPTION_NOWAIT);
    #endif

    // parsing
    MTVDEC_PRINT(" - MPV_SetIFrameBuf\n");
    i4Ret = MPV_SetIFrameBuf(ucEsId, &rMpvBuf);
    
    if(i4Ret != MPV_DRV_OK)
    {
        MTVDEC_PRINT(" - ucEsId = %d, MPV_SetIFrameBuf fail, 0x%x 0x%x 0x%x 0x%x\n",
            (int)ucEsId,
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+1))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+2))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+3))));
        return MTR_NOT_OK;
    }

    MTVDEC_PRINT(" - Wait Vdec parsing\n");
    
    #ifdef IFRAME_MSGQ_TIMEOUT
    VERIFY(x_timer_start(_hMtvdecTimer, IFRAME_TIMEOUT, X_TIMER_FLAG_ONCE,
            _MTVDEC_TimerTimeout, (void*)(UINT32)0) == OSR_OK);
    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &(_hMtvdecMsgQ), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
    VERIFY(x_timer_stop(_hMtvdecTimer) == OSR_OK);

    if (u4DecFin != MTVDEC_FINISH)
    {
        MTVDEC_PRINT(" - Wait Vdec parsing error, 0x%x 0x%x 0x%x 0x%x\n",
            (int)u4DecFin,
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+1))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+2))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+3))));
        return MTR_NOT_OK;
    }
    #else
    
    if(x_sema_lock_timeout(_hIframeSema, IFRAME_TIMEOUT) != OSR_OK)
    {
         MTVDEC_PRINT(" - Wait Vdec parsing timeout, 0x%x 0x%x 0x%x 0x%x\n",
             (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr))),
             (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+1))),
             (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+2))),
             (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+3))));
         return MTR_NOT_OK;
    }
    if(_u4IFrameResult != MTVDEC_FINISH)
    {
        MTVDEC_PRINT(" - !!Wait Vdec parsing error %d, 0x%x 0x%x 0x%x 0x%x\n",
            (int)_u4IFrameResult,
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+1))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+2))),
            (int)(*(UCHAR*)VIRTUAL((UINT32)(rMpvBuf.pucIFrameBufAddr+3))));
        return MTR_NOT_OK;
    }
    #endif
    
    // decoding
    MTVDEC_PRINT("MPV_SetIFrameDec\n");
    
    i4Ret = MPV_SetIFrameDec(ucEsId, &rMpvBuf);
    
    if(i4Ret != MPV_DRV_OK)
    {
        MTVDEC_PRINT(" - ucEsId = %d, MPV_SetIFrameDec fail\n",(int)ucEsId);
        return MTR_NOT_OK;
    }
    
    MTVDEC_PRINT("Wait Vdec decoding\n");
    
    #ifdef IFRAME_MSGQ_TIMEOUT
    
    VERIFY(x_timer_start(_hMtvdecTimer, IFRAME_TIMEOUT, X_TIMER_FLAG_ONCE,
            _MTVDEC_TimerTimeout, (void*)(UINT32)0) == OSR_OK);
    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &(_hMtvdecMsgQ), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
    VERIFY(x_timer_stop(_hMtvdecTimer) == OSR_OK);
    
    if (u4DecFin != MTVDEC_FINISH)
    {
        MTVDEC_PRINT(" - Wait Vdec decoding error %d\n", u4DecFin);
        return MTR_NOT_OK;
    }
    #else
    
    if(x_sema_lock_timeout(_hIframeSema, IFRAME_TIMEOUT) != OSR_OK)
    {
        MTVDEC_PRINT(" - Wait Vdec decoding timeout\n");
        return MTR_NOT_OK;
    }
    
    if(_u4IFrameResult != MTVDEC_FINISH)
    {
        MTVDEC_PRINT(" - Wait Vdec decoding error %d\n", (int)_u4IFrameResult);
        return MTR_NOT_OK;
    }
    #endif
      
    // get seq header infomation
    VDEC_QueryInfo(ucEsId, &rHdrInfo);
    u4Pitch = rHdrInfo.u2Width;
    u4Width = rHdrInfo.u2Width;
    u4Height = rHdrInfo.u2Height;

    u4OutputFbgAddr = (UINT32)(PAGE_ALIGN(prFbmPool_output->u4Addr) + FIRST_DECODING_FBG_OFFSET);

    MTVDEC_PRINT("ADDR of CanvasBuffer ---> %d \n",u4OutputFbgAddr + DECODER_OUTPUT_OFFSET);
    
    if((u4Pitch*u4Height*4) > MTDECODE_CANVAS_SIZE)
    {
        MTVDEC_PRINT("%s buffer size not enought, u4Pitch(%d) u4Height(%d)\n",
            __FUNCTION__, u4Pitch, u4Height);
        return MTR_NOT_OK;
    }
    
    {
        x_memset((VOID*)VIRTUAL((u4OutputFbgAddr + DECODER_OUTPUT_OFFSET)),
            0, MTDECODE_CANVAS_SIZE);
        x_memset(&_rDecodeIFrameDstInfo, 0, sizeof(VDP_THUMBNAIL_INFO_T));
        _rDecodeIFrameDstInfo.eCanvasColormode = VDP_COLORMODE_ARGB_D8888;
        _rDecodeIFrameDstInfo.pucCanvasBuffer = (UCHAR*)(u4OutputFbgAddr + DECODER_OUTPUT_OFFSET);
        _rDecodeIFrameDstInfo.u4CanvasHeight = u4Height;
        _rDecodeIFrameDstInfo.u4CanvasPitch = u4Pitch * 4;
        _rDecodeIFrameDstInfo.u4CanvasWidth = u4Width * 4;
        _rDecodeIFrameDstInfo.u4ThumbnailHeight = u4Height;
        _rDecodeIFrameDstInfo.u4ThumbnailWidth = u4Width;
        _rDecodeIFrameDstInfo.u4ThumbnailX = 0; // offset
        _rDecodeIFrameDstInfo.u4ThumbnailY = 0;
    
        // clean thumbanil
        pDstBuffer = _rDecodeIFrameDstInfo.pucCanvasBuffer;
        
        for(u4Line=0;u4Line<_rDecodeIFrameDstInfo.u4ThumbnailHeight;u4Line++)
        {
            for(u4Hpos=0;u4Hpos<_rDecodeIFrameDstInfo.u4ThumbnailWidth;u4Hpos++)
            {
                *(((UINT32 *)((VOID*)VIRTUAL((UINT32)pDstBuffer)+0))+u4Hpos)=0xff000000;
            }
            pDstBuffer+=_rDecodeIFrameDstInfo.u4CanvasPitch;
        }  
        if(x_sema_lock(_hIframeSema, X_SEMA_OPTION_NOWAIT) != OSR_WOULD_BLOCK)
        {
            MTVDEC_PRINT(" - Wait Vdec thumbnail error \n" );
        }
        
        
        VDEC_GetVideoThumbnail3(ucEsId, 0, FALSE,
            &_rDecodeIFrameDstInfo, FALSE);
    }
    
    MTVDEC_PRINT(" - Wait VDEC_GetVideoThumbnail\n");
      
    #ifdef IFRAME_MSGQ_TIMEOUT
    
    VERIFY(x_timer_start(_hMtvdecTimer, IFRAME_TIMEOUT, X_TIMER_FLAG_ONCE,
            _MTVDEC_TimerTimeout, (void*)(UINT32)0) == OSR_OK);
    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &(_hMtvdecMsgQ), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
    VERIFY(x_timer_stop(_hMtvdecTimer) == OSR_OK);
    
    if (u4DecFin != MTVDEC_FINISH)
    {
        MTVDEC_PRINT(" - Wait Vdec thumbnail error %d\n", u4DecFin);
        return MTR_NOT_OK;
    }
    #else
    
    if(x_sema_lock_timeout(_hIframeSema, IFRAME_TIMEOUT) != OSR_OK)
    {
        MTVDEC_PRINT(" - Wait Vdec thumbnail timeout\n");
        return MTR_NOT_OK;
    }
    
    if(_u4IFrameResult != MTVDEC_FINISH)
    {
        MTVDEC_PRINT(" - Wait Vdec thumbnail error %d\n", (int)_u4IFrameResult);
        return MTR_NOT_OK;
    }
    #endif
    
    // fill feedback
    u4DestSize = u4Width * u4Height * 4;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4DestSize,UINT32);
    COPY_TO_USER_ARG(pu4DestSize,u4DestSize,UINT32);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Width,UINT32);
    COPY_TO_USER_ARG(pu4Width,u4Width,UINT32);

    return MTR_OK;
}

static MT_RESULT_T _MTVDEC_Pause(unsigned long arg)
{
    UCHAR ucEsId;
    ucEsId = (UCHAR)arg;
    MTVDEC_PRINT(" - ucEsId = %d\n",(int)ucEsId);

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
    VDEC_Pause(ucEsId);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDEC_Stop(unsigned long arg)
{
    UCHAR ucEsId;
    INT32 i4Ret;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    ucEsId = (UCHAR)arg;
    MTVDEC_PRINT(" - ucEsId = %d\n",(int)ucEsId);

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }

    // clear lock
    x_sema_lock(_ahMTVDECVdecCtrlSema[ucEsId], X_SEMA_OPTION_NOWAIT);

    rNfyInfo.pvTag = &_aucMTVDECEsId[ucEsId];
    rNfyInfo.pfDecNfy = _MTVDEC_VDEC_Nfy;
    VDEC_SetDecNfy(ucEsId, &rNfyInfo);

    VDEC_Stop(ucEsId);

    if(_bPlayingIframe)
    {
        _bPlayingIframe = FALSE;
        #ifdef IFRAME_MSGQ_TIMEOUT
        _MTVDEC_TimerTimeout(0, 0);
        #else
        x_sema_unlock(_hIframeSema);
        #endif
    }

    // wait ctrl done
    i4Ret = x_sema_lock_timeout(_ahMTVDECVdecCtrlSema[ucEsId], 5000);
    if (i4Ret != OSR_OK)
    {
        printf("_ahMTVDECVdecCtrlSema timeout\n");
    }

    return MTR_OK;
}

static MT_RESULT_T _MTVDEC_GetSrcState(unsigned long arg)
{
    MT_RESULT_T                 eRetvl = MTR_OK;
    MTAL_IOCTL_2ARG_T           rArg;
    UCHAR                       ucEsId;
    BOOL                        *pbScramble = NULL, bScramble = FALSE;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    ucEsId =(UCHAR)rArg.ai4Arg[0];
    pbScramble = (BOOL*)rArg.ai4Arg[1];
	
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
    if (!pbScramble)
    {
        eRetvl = MTR_PARAMETER_ERROR;
        goto EXIT_POINT;
    }
    
    VDEC_GetSrcState(ucEsId, &bScramble);
    USR_SPACE_ACCESS_VALIDATE_ARG(pbScramble, BOOL);
    COPY_TO_USER_ARG(pbScramble, bScramble, BOOL);
    
EXIT_POINT:
    return eRetvl;
}

static MT_RESULT_T _MTVDEC_GetAfdState(unsigned long arg)
{
    MT_RESULT_T                 eRetvl = MTR_OK;
    MTAL_IOCTL_2ARG_T           rArg;
    UCHAR                       ucEsId;
    BOOL                        *pbafdfmt = NULL, bafdfmt = FALSE;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    ucEsId =(UCHAR)rArg.ai4Arg[0];
    pbafdfmt = (BOOL*)rArg.ai4Arg[1];
	
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
    if (!pbafdfmt)
    {
        eRetvl = MTR_PARAMETER_ERROR;
        goto EXIT_POINT;
    }
    
    VDEC_GetAFDStatue(ucEsId, &bafdfmt);
    //printf(" VDEC_GetAFDStatue,bafdfmt=%d.\r\n",bafdfmt);
    USR_SPACE_ACCESS_VALIDATE_ARG(pbafdfmt, BOOL);
    COPY_TO_USER_ARG(pbafdfmt, bafdfmt, BOOL);
    
EXIT_POINT:
    return eRetvl;
}

static MT_RESULT_T _MTVDEC_GetInfo(unsigned long arg)
{
    MTVDEC_HDR_INFO_T*   prVdecInfo;
    MTAL_IOCTL_2ARG_T    rArg;
    VDEC_HDR_INFO_T rHdrInfo;
    
    UCHAR ucEsId;
    MTVDEC_HDR_INFO_T    rVdecInfo;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    ucEsId =(UCHAR)rArg.ai4Arg[0];
    prVdecInfo = (MTVDEC_HDR_INFO_T*)rArg.ai4Arg[1];
    x_memset(&rVdecInfo, 0, sizeof(MTVDEC_HDR_INFO_T));

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
    
    VDEC_QueryInfo(ucEsId, &rHdrInfo);
    
    rVdecInfo.fgConstrainedParameterFlag = rHdrInfo.fgContrained;
    rVdecInfo.fgLowDelay = rHdrInfo.fgLowDelay;
    rVdecInfo.fgMPEG2 = rHdrInfo.fgMPEG2;
    rVdecInfo.fgSeqDispExternValid = rHdrInfo.fgSeqDispExt;
    rVdecInfo.fgColorDescription = rHdrInfo.fgColorDescrip;
    rVdecInfo.ucColorPrimaries = rHdrInfo.ucColorPrimaries;
    rVdecInfo.ucTransferCharacteristics = rHdrInfo.ucTransferCharacteristics;
    rVdecInfo.ucMatrixCoefficients = rHdrInfo.ucMatrixCoefficients;
    rVdecInfo.ucProfileLevelIndication = rHdrInfo.ucProfileId;
    rVdecInfo.u4VbvBufSize = rHdrInfo.u4VbvSizeExt;
    rVdecInfo.u4ChromaFormat = rHdrInfo.u4ChromaFmt;
    
    rVdecInfo.u2Height = rHdrInfo.u2Height;
    rVdecInfo.u2Width = rHdrInfo.u2Width;
    rVdecInfo.u2FrmRate = rHdrInfo.u2FrmRate;
    rVdecInfo.eAspectRatio = rHdrInfo.eAspectRatio;
    rVdecInfo.u4BitRate = rHdrInfo.u4BitRate;
    rVdecInfo.ucAfd = rHdrInfo.ucAfd;
    rVdecInfo.fgProgressiveSeq = rHdrInfo.fgProgressiveSeq;
    rVdecInfo.ucVideoFmt = rHdrInfo.ucVideoFmt;    

    if (rHdrInfo.u2DHS == 0)
    {
        rVdecInfo.i2DHS = -1;
    }
    else
    {
        rVdecInfo.i2DHS = (INT16)rHdrInfo.u2DHS;    
    }

    if (rHdrInfo.u2DVS == 0)
    {
        rVdecInfo.i2DVS = -1;    
    }
    else
    {
        rVdecInfo.i2DVS = (INT16)rHdrInfo.u2DVS;        
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prVdecInfo, MTVDEC_HDR_INFO_T );
    COPY_TO_USER_ARG(prVdecInfo, rVdecInfo, MTVDEC_HDR_INFO_T );

    #if 0
    MTVDEC_PRINT(" - ucEsId = %d\n",(int)ucEsId);
    MTVDEC_PRINT_INFO(rVdecInfo);
    #endif

    return MTR_OK;
}


static MT_RESULT_T _MTVDEC_GetDbkInfo(unsigned long arg)
{
    MTVDEC_DBK_INFO_T*   prDbkInfo;
    MTAL_IOCTL_2ARG_T    rArg;
    VDEC_HDR_INFO_T rHdrInfo;
    
    UCHAR ucEsId;
    MTVDEC_DBK_INFO_T    rDbkInfo;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    ucEsId =(UCHAR)rArg.ai4Arg[0];
    prDbkInfo = (MTVDEC_DBK_INFO_T*)rArg.ai4Arg[1];
    x_memset(&rDbkInfo, 0, sizeof(MTVDEC_DBK_INFO_T));

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
    
    VDEC_QueryInfo(ucEsId, &rHdrInfo);
    
    rDbkInfo.fgDeblocking = rHdrInfo.fgDeblocking;
    rDbkInfo.u4Dbk1PixelCnt = rHdrInfo.u4Dbk1PixelCnt;
    rDbkInfo.u4Dbk2PixelCnt = rHdrInfo.u4Dbk2PixelCnt;
    rDbkInfo.u4Dbk3PixelCnt = rHdrInfo.u4Dbk3PixelCnt;

    USR_SPACE_ACCESS_VALIDATE_ARG(prDbkInfo, MTVDEC_DBK_INFO_T );
    COPY_TO_USER_ARG(prDbkInfo, rDbkInfo, MTVDEC_DBK_INFO_T );

    MTVDEC_PRINT(" - ucEsId = %d\n",(int)ucEsId);

    return MTR_OK;
}

static MT_RESULT_T _MTVDEC_SetAvSynMode(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
	  UCHAR ucEsId;
	  MTAV_SYNC_MODE_T eMode;
	
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
	
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    eMode = (MTAV_SYNC_MODE_T)rArg.ai4Arg[1];
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
    MTVDEC_PRINT(" - ucEsId = %d, eMode = %d\n",
		(int)ucEsId,
		(int)eMode);

    //add driver implementation code here
    VDEC_SyncStc(ucEsId, eMode, 0/*u2PcrId*/);
    
    return MTR_OK;
}


static MT_RESULT_T _MTVDEC_GetStc(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UCHAR ucEsId;
    UINT64 *pu8Stc;
    UINT64  u8Stc;
    STC_CLOCK rStcClk;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    pu8Stc = (UINT64 *)rArg.ai4Arg[1];

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
    if (STC_GetSrc(0/*u2PcrId*/, &rStcClk) == STC_VALID)
    {
        u8Stc = (UINT64/*PTS_T*/)(rStcClk.u4Base);
    }
    else
    {
        return MTR_NOT_OK;
    }
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pu8Stc,UINT64);
    COPY_TO_USER_ARG(pu8Stc,u8Stc,UINT64);
	
    MTVDEC_PRINT(" - ucEsId = %d, u8Stc = 0x%08x08%x\n",
		(int)ucEsId,(unsigned int)(u8Stc>>32),(unsigned int)u8Stc); 

    return MTR_OK;
}


static MT_RESULT_T _MTVDEC_GetDecStatus(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    MTVDEC_DEC_STATUS_T*   prDecStatus;  
    MTVDEC_DEC_STATUS_T   rDecStatus; 
    UCHAR ucEsId;
    
    BOOL fgLock;                        ///<video decoder is lock.
    BOOL fgTopFldFirst;                 ///<top field first
    BOOL fgRepFirstFld;                 ///<repeat first field
    BOOL fgProgressiveFrm;              ///<progressive frame
    UCHAR ucPicType;                    ///<picture type decoded ok by video decoder.
    UCHAR ucPicStruture;                ///<picture struct
    UCHAR ucTemporalRef;               ///<temporal reference
    UINT32 u4ReceiveNs;                 ///<picture count received by video decoder.
    UINT32 u4DecOkNs;                   ///<picture count decoded ok by video decoder.
    
    VDEC_GNL_INFO_T rGnlInfo;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    prDecStatus = (MTVDEC_DEC_STATUS_T*)rArg.ai4Arg[1];
    x_memset(&rDecStatus, 0, sizeof(MTVDEC_DEC_STATUS_T));
    x_memset(&rGnlInfo, 0, sizeof(VDEC_GNL_INFO_T));

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
        
    VDEC_GetDecStatus(ucEsId, &fgLock, &fgTopFldFirst
        , &fgRepFirstFld, &fgProgressiveFrm, &ucPicType
        , &ucPicStruture, &ucTemporalRef, &u4ReceiveNs
        , &u4DecOkNs);
        
    rDecStatus.fgLock = fgLock;
    if(fgLock)
    {   
		rDecStatus.fgIsDisplaying = MPV_IsDisplay(ucEsId);
        rDecStatus.fgTopFldFirst = fgTopFldFirst;
        rDecStatus.fgRepFirstFld = fgRepFirstFld;
        rDecStatus.fgProgressiveFrm = fgProgressiveFrm;
        rDecStatus.ucPicType = ucPicType;
        rDecStatus.ucPicStruture = ucPicStruture;
        rDecStatus.ucTemporalRef = ucTemporalRef;
        rDecStatus.u4ReceiveNs = u4ReceiveNs;
        rDecStatus.u4DecOkNs = u4DecOkNs;
        rDecStatus.u4CurPTS = VDP_GetPts(VDP_Es2Vdp(ucEsId));
        if(VDEC_GetGnlInfo(ucEsId, &rGnlInfo))
        {
            rDecStatus.u4ESBufDataSize = rGnlInfo.u4ESBufDataSize;
            rDecStatus.u4DisplayQNum = rGnlInfo.u4DisplayQNum;
            rDecStatus.u4DecodingPTS = rGnlInfo.u4DecodingPTS;
        }
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prDecStatus, MTVDEC_DEC_STATUS_T);
    COPY_TO_USER_ARG(prDecStatus,rDecStatus,MTVDEC_DEC_STATUS_T);

    #if 0
    MTVDEC_PRINT(" - ucEsId = %d, fgLock = %d, fgTopFldFirst = %d\
        , fgRepFirstFld = %d, fgProgressiveFrm = %d, ucPicType = %d\
        , ucPicStruture = %d, ucTemporalRef = %d, u4ReceiveNs = %d\
        , u4DecOkNs = %d"\
        , (int)ucEsId, (int)fgLock, (int)fgTopFldFirst, (int)fgRepFirstFld\
        , (int)fgProgressiveFrm, (int)ucPicType, (int)ucPicStruture\
        , (int)ucTemporalRef, (int)u4ReceiveNs, (int)u4DecOkNs);    
    #endif
    
    return MTR_OK;
}


static MT_RESULT_T _MTVDEC_RegDecErrCb(unsigned long arg)
{
    UINT32 u4Duration;

    u4Duration = (UINT32)arg;

    VDEC_RegDecErrCb((PFN_VDEC_DECERR_CB)_MTVDEC_ERR_Nfy, u4Duration);

    MTVDEC_PRINT(" - pfDecErrCb = 0x%x, u4Duration = %d ms\n",(int)_MTVDEC_ERR_Nfy, (int)u4Duration);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDEC_PicTypeCb(void)
{
    //VDEC_RegPicTypeCb((PFN_VDEC_PIC_TYPE_CB)_MTVDEC_PIC_TYPE_Nfy);
    VDP_RegCbFunc(VDP_CB_FUNC_Get_PIC_TYPE,(UINT32)_MTVDEC_PIC_TYPE_Nfy,0,0);

    MTVDEC_PRINT(" - pfPicTypeCb = 0x%x\n",(int)_MTVDEC_PIC_TYPE_Nfy);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDEC_StatusIntCb(void)
{
    VDEC_RegStatusCb((PFN_VDEC_STATUS_CB)_MTVDEC_STATUS_INT_Nfy);

    MTVDEC_PRINT(" - pfStatusCb = 0x%x\n",(int)_MTVDEC_STATUS_INT_Nfy);
    
    return MTR_OK;
}

#if 0
static MT_RESULT_T _MTVDEC_GetDecFifo(unsigned long arg)
{
    UCHAR ucEsId;
    UINT32 *pu4FifoAddr;
    UINT32 *pu4FifoSize;
    MTAL_IOCTL_3ARG_T    rArg;
    UINT32 u4FifoAddr;
    UINT32 u4FifoSize;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    pu4FifoAddr = (UINT32 *)rArg.ai4Arg[1];
    pu4FifoSize = (UINT32 *)rArg.ai4Arg[2];
    MTVDEC_PRINT(" - ucEsId = %d, pu4FifoAddr = %d, pu4FifoSize = %d\n",
		ucEsId,
		pu4FifoAddr,
		pu4FifoSize);

    //add driver implementation code here


    USR_SPACE_ACCESS_VALIDATE_ARG(pu4FifoAddr,UINT32);
    COPY_TO_USER_ARG(pu4FifoAddr,u4FifoAddr,UINT32);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4FifoSize,UINT32);
    COPY_TO_USER_ARG(pu4FifoSize,u4FifoSize,UINT32);

    return MTR_OK;
}
#endif

static MT_RESULT_T _MTVDEC_RegUserDataCb(unsigned long arg)
{
    MTVDEC_PFN_USERDATA_CB pfUserDataCb;
    pfUserDataCb = (MTVDEC_PFN_USERDATA_CB)arg;
    MTVDEC_PRINT(" - pfUserDataCb = 0x%x\n",(int)pfUserDataCb);

    //add driver implementation code here
    MPV_RegUserDataCb((PFN_MPV_USERDATA_CB)_MTVDEC_CC_Nfy);
    return MTR_OK;
}


static MT_RESULT_T _MTVDEC_StartUserData(unsigned long arg)
{
    UCHAR ucEsId;
    ucEsId = (UCHAR)arg;
    
    MTVDEC_PRINT(" - ucEsId = %d\n",(int)ucEsId);

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
    MPV_StartUserData(ucEsId);
    
    return MTR_OK;
}


static MT_RESULT_T _MTVDEC_StopUserData(unsigned long arg)
{
    UCHAR ucEsId;
    ucEsId = (UCHAR)arg;
    MTVDEC_PRINT(" - ucEsId = %d\n",(int)ucEsId);

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }    
    MPV_StopUserData(ucEsId);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDEC_GetUserData(unsigned long arg)
{
    UCHAR *pucUserDataTmpBuf;
    UINT32 u4Rptr;
    UINT32 u4DataLen;
    MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    pucUserDataTmpBuf = (UCHAR *)rArg.ai4Arg[0];
    u4Rptr = (UINT32)rArg.ai4Arg[1];
    u4DataLen = (UINT32)rArg.ai4Arg[2];
    MTVDEC_PRINT(" - pucUserDataTmpBuf = %d, u4Rptr = %d, u4DataLen = %d\n",
		(int)pucUserDataTmpBuf,
		(int)u4Rptr,
		(int)u4DataLen);

    //add driver implementation code here
    if(!MPV_GetUserData(_au1UserDataTmpBuf, u4Rptr, u4DataLen))
    {
        return -1;
    }
    
    if (copy_to_user((void __user *)pucUserDataTmpBuf, (void*)_au1UserDataTmpBuf, u4DataLen))
    {
        printk("%s copy_to_user error\n", __FUNCTION__);
        return MTR_ERR_INV;
    }
    
    return MTR_OK;
}
#ifdef CC_ITIMING_ALWAYS
static MT_RESULT_T _MTVDEC_SetFixFHDDisplay(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UCHAR ucEsId;
    BOOL fgInterlace;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    fgInterlace = (BOOL)rArg.ai4Arg[1];

    MTVDEC_PRINT(" - ucEsId = %d, fgEnable = %d\n",
		(int)ucEsId,
		(int)fgInterlace);

    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }

    VDEC_SetFixFHDDisplay(ucEsId, fgInterlace);
    return MTR_OK;
}
#endif
static MT_RESULT_T _MTVDEC_SetSpeed(unsigned long arg)
{
    UCHAR ucEsId;
    MTVDEC_DEC_SPEED_T eSpeed;
    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    eSpeed = (MTVDEC_DEC_SPEED_T)rArg.ai4Arg[1];
    MTVDEC_PRINT(" - eSpeed = %d\n",
		(int)eSpeed);

    if(ucEsId == ES0) //assume ES0 map to B2R_1
    {   
        MTVDEC_PRINT(" - eSpeed(%d)\n", (UINT32)eSpeed);
        switch(eSpeed)
        {
            case MTVDEC_DEC_SPEED_PAUSE:
                #ifdef TIME_SHIFT_SUPPORT
                if(_MTDREC_IsLivePidEnable())
                {
                    VDP_FrcSetFreeze(B2R_1, TRUE);
                }
                else
                #endif
                {
	                if(VDP_SetPauseMm(B2R_1, TRUE) != VDP_SET_OK)
	                {
	                    MTVDEC_PRINT(" - pause mm fail\n");
	                }
                }
                break;
            case MTVDEC_DEC_SPEED_SLOW:
                if(VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1_DIV_2X) != VDP_SET_OK)
                {
                    MTVDEC_PRINT(" - 1/2x fail\n");
                }
                VDP_FrcSetFreeze(B2R_1, FALSE);
                if(VDP_SetPauseMm(B2R_1, FALSE) != VDP_SET_OK)
                {
                    MTVDEC_PRINT(" - pause mm fail\n");
                }
                break;
            case MTVDEC_DEC_SPEED_NORMAL:
                if(VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1X) != VDP_SET_OK)
                {
                    MTVDEC_PRINT(" - 1x fail\n");
                }
                VDP_FrcSetFreeze(B2R_1, FALSE);
                if(VDP_SetPauseMm(B2R_1, FALSE) != VDP_SET_OK)
                {
                    MTVDEC_PRINT(" - pause mm fail\n");
                }
                break;
            default:
                MTVDEC_PRINT(" - invalid speed\n");
                return MTR_ERR_INV;
        }
    }
    else
    {
        MTVDEC_PRINT(" - invalid ucEsId(%d)\n", ucEsId);
        return MTR_ERR_INV;
    }
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDEC_SetDecodeMode(unsigned long arg)
{
    UCHAR ucEsId;
    MTVDEC_DEC_MODE_T eMode;
    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    eMode = (MTVDEC_DEC_MODE_T)rArg.ai4Arg[1];
    MTVDEC_PRINT(" - eMode = %d\n",
		(int)eMode);

    if(ucEsId == ES0)
    {   
        switch(eMode)
        {
            case MTVDEC_DEC_ALL:
                VDEC_SetMMParam(ucEsId, VDEC_MM_PLAY_SPEED,
                    (UINT32)STC_SPEED_TYPE_FORWARD_1X, 0, 0);
                break;
            case MTVDEC_DEC_IP:
                VDEC_SetMMParam(ucEsId, VDEC_MM_PLAY_SPEED,
                    (UINT32)STC_SPEED_TYPE_FORWARD_2X, 0, 0);
                break;
            case MTVDEC_DEC_I:
                VDEC_SetMMParam(ucEsId, VDEC_MM_PLAY_SPEED,
                    (UINT32)STC_SPEED_TYPE_FORWARD_4X, 0, 0);
                break;
            default:
                MTVDEC_PRINT(" - invalid mode\n");
                return MTR_ERR_INV;
        }
    }
    else
    {
        MTVDEC_PRINT(" - invalid ucEsId(%d)\n", ucEsId);
        return MTR_ERR_INV;
    }
    
    return MTR_OK;
}

#ifdef CC_53XX_SWDMX_V2
static VOID _MTPVR_StepFinNotify(UINT32 u4VdpId, UINT32 u4PTS, UINT32 u4Ret, UINT32 u4AttachedId)
#else
static VOID _MTPVR_StepFinNotify(UINT32 u4VdpId, UINT32 u4PTS, UINT32 u4Ret)
#endif
{
    UNUSED(u4VdpId);
    UNUSED(u4PTS);
    MTVDEC_PRINT("_MTPVR_StepFinNotify, _MTPVR_SetSpeed set to 0\n");
    // block mode
    //VERIFY (x_sema_unlock(_hVDPMWStepMutex) == OSR_OK);
    // notify mw that we finish step.

    // PVR0 0, speed 0
    _MTPVR_SetSpeedExt(0, 0, 2); // step done
}

static MT_RESULT_T _MTVDEC_SetFrameAdvance(unsigned long arg)
{
    UCHAR ucEsId;
    BOOL fgSetFrameAdvance;
    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    ucEsId = (UCHAR)rArg.ai4Arg[0];
    fgSetFrameAdvance = (BOOL)rArg.ai4Arg[1];
    MTVDEC_PRINT(" - fgSetFrameAdvance = %d\n",
		(int)fgSetFrameAdvance);

    if(ucEsId == ES0) //assume ES0 map to B2R_1
    {
        if(fgSetFrameAdvance)
        {
            // PVR0 0, speed 200
            //_MTPVR_SetSpeedExt(0, 200);

            VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1X);

            /* Pause VDP and just display 1 frame */
            VDP_SetFBNoTimeout(B2R_1, TRUE);
            VDP_SetPauseMm(B2R_1, TRUE);
            VDP_SetStepForward(B2R_1, VDP_PLAY_STEP_FORWARD);

            /* VDP will play one frame and then Pause if VDP_PLAY_STEP_FORWARD is set */
            VDP_FrcSetFreeze(B2R_1, FALSE);
            VDP_SetPauseMm(B2R_1, FALSE);

            VDP_SetCbAttachedSrcId(B2R_1, ucEsId);
            VDP_RegCbFunc(VDP_CB_FUNC_MM_STEP_FIN_IND, (UINT32)_MTPVR_StepFinNotify, 0, 0);

            _MTPVR_SetSpeedExt(0, 0, 1); // step start, move some data from PVR
        }
        else
        {
            _MTPVR_SetSpeedExt(0, 0, 3); // step end
            
            VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1X);

            VDP_SetFBNoTimeout(B2R_1, FALSE);
            VDP_SetStepForward(B2R_1, VDP_PLAY_NORMAL);
            VDP_FrcSetFreeze(B2R_1, FALSE);
            VDP_SetPauseMm(B2R_1, FALSE);

            VDP_SetTrickMode(B2R_1,VDP_PLAY_NORMAL);

            VDP_SetCbAttachedSrcId(B2R_1, ucEsId);
            VDP_RegCbFunc(VDP_CB_FUNC_MM_STEP_FIN_IND, (UINT32)0, 0, 0);
        }
    }
    else
    {
        MTVDEC_PRINT(" - invalid ucEsId(%d)\n", ucEsId);
        return MTR_ERR_INV;
    }
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDEC_SetFlushEsData(unsigned long arg)
{
    UCHAR ucEsId;
    ucEsId = (UCHAR)arg;
    MTVDEC_PRINT(" - ucEsId = %d\n",(int)ucEsId);

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }

    MPV_FlushEsmQ(ucEsId, FALSE);
    
    return MTR_OK;
}

int mtal_ioctl_mtvdec(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_OK;

	switch (cmd)
	{
       case MTAL_IO_VDEC_INIT:
       	i4Ret = _MTVDEC_Init();
        break;
       case MTAL_IO_VDEC_TERMINT:
       	i4Ret = _MTVDEC_Termint();
        break;
       case MTAL_IO_VDEC_RESET:
       	i4Ret = _MTVDEC_Reset();
        break;
       case MTAL_IO_VDEC_PLAY:
       	i4Ret = _MTVDEC_Play(arg);
        break;
       case MTAL_IO_VDEC_PLAY_I_FRAME:
       	i4Ret = _MTVDEC_PlayIFrame(arg);
        break;
       case MTAL_IO_VDEC_DECODE_I_FRAME: // 20110801
       	i4Ret = _MTVDEC_DecodeIFrame(arg);
        break;
       case MTAL_IO_VDEC_PAUSE:
       	i4Ret = _MTVDEC_Pause(arg);
        break;
       case MTAL_IO_VDEC_STOP:
       case MTAL_IO_VDEC_STOP_I_FRAME:
       	i4Ret = _MTVDEC_Stop(arg);
        break;
       case MTAL_IO_VDEC_GETRESOLUTION:
       	i4Ret = _MTVDEC_GetInfo(arg);
        break;
       case MTAL_IO_VDEC_GETDBKINFO:
       	i4Ret = _MTVDEC_GetDbkInfo(arg);
        break;

       case MTAL_IO_VDEC_SETAVSYNMODE:
       	i4Ret = _MTVDEC_SetAvSynMode(arg);
        break;
       case MTAL_IO_VDEC_GETSTC:
       	i4Ret = _MTVDEC_GetStc(arg);
        break;
       case MTAL_IO_VDEC_GETDECSTATUS:
       	i4Ret = _MTVDEC_GetDecStatus(arg);
        break;
       case MTAL_IO_VDEC_REGDECERRCB:
       	i4Ret = _MTVDEC_RegDecErrCb(arg);
        break;        
       case MTAL_IO_VDEC_REGPICTYPECB:
       	i4Ret = _MTVDEC_PicTypeCb();
        break;      
       case MTAL_IO_VDEC_REGSTATUSCB:
       	i4Ret = _MTVDEC_StatusIntCb();
        break;      
        
#if 0

       case MTAL_IO_VDEC_GETDECFIFO:
       	 i4Ret = _MTVDEC_GetDecFifo(arg);
        break;
#endif
       case MTAL_IO_VDEC_REGUSERDATACB:
       	i4Ret = _MTVDEC_RegUserDataCb(arg);
        break;
       case MTAL_IO_VDEC_STARTUSERDATA:
       	i4Ret = _MTVDEC_StartUserData(arg);
        break;
       case MTAL_IO_VDEC_STOPUSERDATA:
       	i4Ret = _MTVDEC_StopUserData(arg);
        break;
       case MTAL_IO_VDEC_GETUSERDATA:
       	i4Ret = _MTVDEC_GetUserData(arg);
        break;
       #ifdef CC_ITIMING_ALWAYS
       case MTAL_IO_VDEC_SETFHDDISPLAY:
              i4Ret = _MTVDEC_SetFixFHDDisplay(arg);
        break;
       #endif
       case MTAL_IO_VDEC_SETSPEED:
       	i4Ret = _MTVDEC_SetSpeed(arg);
        break;
       case MTAL_IO_VDEC_SETDECODEMODE:
       	i4Ret = _MTVDEC_SetDecodeMode(arg);
        break;
       case MTAL_IO_VDEC_SETFRAMEADVANCE:
       	i4Ret = _MTVDEC_SetFrameAdvance(arg);
        break;
       case MTAL_IO_VDEC_SETFLUSHESDATA:
       	i4Ret = _MTVDEC_SetFlushEsData(arg);
        break;
        case MTAL_IO_VDEC_GETSRCSTATE:
            i4Ret = _MTVDEC_GetSrcState(arg);
            break;
        case MTAL_IO_VDEC_GETAFDSTATE:
            i4Ret = _MTVDEC_GetAfdState(arg);
            break;
        default:
            i4Ret = MTR_NOT_OK;
        break;
	}
	return i4Ret;
}

