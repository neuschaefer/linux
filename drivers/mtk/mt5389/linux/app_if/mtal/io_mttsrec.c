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
 * $RCSfile: io_mtdmx.c,v $
 * $Revision: #8 $
 * $Author: lifa.cao $
 * $SWAuthor: wen han $
 * $MD5HEX:  $
 *---------------------------------------------------------------------------*/

/** @file io_mttsrec.c
 */

#ifdef TIME_SHIFT_SUPPORT
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mtdmx.h"
#include "mttsrec.h"
#include "dmx_if.h"
#include "fvr_if.h"
#include "fbm_drvif.h"
#include "mpv_drvif.h"
#include "vdec_drvif.h"
#include "aud_drvif.h"
#include "stc_drvif.h"
#include "cb_data.h"
#include "cb_low.h"
#include <linux/mtal_ioctl.h>

#include "x_assert.h"
#include "drvcust_if.h"
#include "x_bim.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MTTSREC_STREAM_SIZE                 (0x380000)
#define MTTSREC_META_SIZE                   (0x80000)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT32 au4Key[8];
} MTTSREC_KEY_VAL_T;

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
        printk("mtal_ioctl_mtdmx argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_ARG_SIZE(ArgUsr, ArgKernel, size)       \
    if (copy_from_user(ArgKernel, (void __user *)ArgUsr,   \
                       (size)))                             \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(INT32)))                        \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_6ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_6ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_6ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_6ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_8ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_8ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_8ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");      \
        return MTR_ERR_INV;                                          \
    } \

#define COPY_TO_USER_ARG_SIZE(ArgUsr, ArgKernel,size)              \
    if (copy_to_user((void __user *)ArgUsr, ArgKernel,   \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");      \
        return MTR_ERR_INV;                                          \
    } \

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgTsRecMwEnable = FALSE;

static UINT32 _u4TsRecPktCount = 0;

static UINT32 _u4TsRecStreamAddr = 0;
static UINT32 _u4TsRecStreamSize = 0;

static UINT32 _u4TsRecMetaAddr = 0;
static UINT32 _u4TsRecMetaSize = 0;

static MTTSREC_KEY_VAL_T _arTsRecKeyVal[MTTSREC_MAX_KEY_INDEX];

static UINT8 _au1TsRecPidKeyIdx[MTTSREC_MAX_PID];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _mtTsRecTimerCallback
 */
//-----------------------------------------------------------------------------
static void _MtTsRecTimerCallback(TSREC_MTAL_CB_T *prCB, FVR_GBL_TIMENOTIFY_INFO_T *prTimeInfo)
{
    UINT32 u4PktCount;

    if((prCB == NULL) || (prTimeInfo == NULL))
    {
        return;
    }

    if(prTimeInfo->u4PktCount > 0)
    {
        if(prTimeInfo->u4PktCount > _u4TsRecPktCount)
        {
            u4PktCount = prTimeInfo->u4PktCount - _u4TsRecPktCount;
        }
        else
        {
            // 32 bit overflow
            u4PktCount = 0xFFFFFFFF - (_u4TsRecPktCount - prTimeInfo->u4PktCount - 1);
        }
    }
    else
    {
        u4PktCount = 0;
    }
    _u4TsRecPktCount = prTimeInfo->u4PktCount;

    prCB->u4Data0 = prTimeInfo->u4Addr;
    prCB->u4Data1 = prTimeInfo->u4Size;
    prCB->u4Data2 = u4PktCount;

    ASSERT((prTimeInfo->u4Size / 192) == u4PktCount);
}


//-----------------------------------------------------------------------------
/** _mtTsRecOverflowCallback
 */
//-----------------------------------------------------------------------------
static void _MtTsRecOverflowCallback(TSREC_MTAL_CB_T *prCB)
{
    if(!FVR_Stop())
    {
        MTDMX_PRINT("%s, FVR stop error\n", __FUNCTION__);
    }
}


//-----------------------------------------------------------------------------
/** _MtTsRecGBLCallback
 */
//-----------------------------------------------------------------------------
static void _MtTsRecGBLCallback(FVR_GBL_NOTIFY_CODE_T eCode,
                                       UINT32 u4Data, const void* pvTag)
{
    TSREC_MTAL_CB_T rCB;

    switch(eCode)
    {
    case FVR_GBL_NFY_TIME:
        rCB.type = MTTSREC_GBL_NFY_TIME;
        _MtTsRecTimerCallback(&rCB, (FVR_GBL_TIMENOTIFY_INFO_T*)u4Data);
        break;

    case FVR_GBL_NFY_PKTCOUNT:
        // Don't use
        ASSERT(0);
        break;

    case FVR_GBL_NFY_OVERFLOW:
        rCB.type = MTTSREC_GBL_NFY_OVERFLOW;
        _MtTsRecOverflowCallback(&rCB);
        break;

    default:
        // Unexpected
        ASSERT(0);
        break;
    }

    UNUSED(_CB_PutEvent(CB_MTAL_TSREC_TRIGGER, sizeof (TSREC_MTAL_CB_T), &rCB));
}


//-----------------------------------------------------------------------------
/** MTTSREC_Init
 *  Initialize
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_Init(unsigned long arg)
{
    UINT8 i, j;
	x_os_drv_set_timestamp("_MTTSREC_Init start");

    _u4TsRecPktCount = 0;
    _u4TsRecStreamAddr = 0;
    _u4TsRecStreamSize = 0;
    _u4TsRecMetaAddr = 0;
    _u4TsRecMetaSize = 0;

    //x_memset((void*)_arTsRecKeyVal, 0, sizeof(_arTsRecKeyVal));
    for(i=0; i<MTTSREC_MAX_KEY_INDEX; i++)
    {
        for(j=0; j<8; j++)
        {
            _arTsRecKeyVal[i].au4Key[j] = 0x0;
        }
    }

    for(i=0; i<MTTSREC_MAX_PID ; i++)
    {
        _au1TsRecPidKeyIdx[i] = 0xFF;
    }

    if(!FVR_Init())
    {
        return MTR_NOT_OK;
    }
	x_os_drv_set_timestamp("_MTTSREC_Init end");
    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_SetGBL
 * Set record buffer, notify and callback time
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_SetGBL(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTTSREC_GBL_T rGBL;
    MTTSREC_GBL_T *prGBL;
    FVR_GBL_T rDrvGBL;
    UINT32 u4Flags, u4FVRFlags;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Flags = (UINT32)rArg.ai4Arg[0];
    prGBL = (MTTSREC_GBL_T*)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG(prGBL, MTTSREC_GBL_T);
    COPY_FROM_USER_ARG(prGBL, rGBL, MTTSREC_GBL_T);

    x_memset((void*)&rDrvGBL, 0, sizeof(FVR_GBL_T));
    u4FVRFlags = FVR_GBL_FLAG_TARGET_PKT;

    if((u4Flags & MTTSREC_GBL_FLAG_NOTIFY) && (rGBL.pfnNotifyFunc != NULL))
    {
        rDrvGBL.pfnNotifyFunc = _MtTsRecGBLCallback;
        rDrvGBL.pvNotifyTag = NULL;

        u4FVRFlags |= FVR_GBL_FLAG_NOTIFY;
    }

    if(u4Flags & MTTSREC_GBL_FLAG_TIME_INFO)
    {
        rDrvGBL.u4TimeToNotify = rGBL.u4TimeToNotify;

        u4FVRFlags |= FVR_GBL_FLAG_TIME_INFO;
    }

    if(!FVR_SetGBL(u4FVRFlags, &rDrvGBL))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTTSREC_FreeGBL
 *  Free record resource
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_FreeGBL(unsigned long arg)
{
    if(!FVR_FreeGBL())
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_GetGBL
 *  Get record buffer, notify and callbacktime
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_GetGBL(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTTSREC_GBL_T rGBL;
    MTTSREC_GBL_T *prGBL;
    FVR_GBL_T rDrvGBL;
    UINT32 u4Flags;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Flags = (UINT32)rArg.ai4Arg[0];
    prGBL = (MTTSREC_GBL_T*)rArg.ai4Arg[1];

    if(!FVR_GetGBL(FVR_GBL_FLAG_ALL, &rDrvGBL))
    {
        return MTR_NOT_OK;
    }

    if(u4Flags & MTTSREC_GBL_FLAG_TIME_INFO)
    {
        rGBL.u4TimeToNotify = rDrvGBL.u4TimeToNotify;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prGBL, MTTSREC_GBL_T);
    COPY_TO_USER_ARG(prGBL, rGBL, MTTSREC_GBL_T);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_SetPid
 *  Set record PID information
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_SetPid(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT8 u1Pidx;
    UINT32 u4Flags, u4PidFlags;
    FVR_PID_T rFvrPid;
    MTTSREC_PID_T rPid;
    MTTSREC_PID_T *prPid;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u4Flags = (UINT32)rArg.ai4Arg[1];
    prPid = (MTTSREC_PID_T*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG(prPid, MTTSREC_PID_T);
    COPY_FROM_USER_ARG(prPid, rPid, MTTSREC_PID_T);

    x_memset((void*)&rFvrPid, 0, sizeof(FVR_PID_T));

#ifdef CC_MT5392B
    u4PidFlags = FVR_PID_FLAG_TSINDEX | FVR_PID_FLAG_PICSEARCH | FVR_PID_FLAG_PKTSWAP |
                 FVR_PID_FLAG_ENCRYPT_METHOD | FVR_PID_FLAG_CBC_MODE;
#else
    u4PidFlags = FVR_PID_FLAG_SOURCE | FVR_PID_FLAG_PICSEARCH | FVR_PID_FLAG_PKTSWAP |
                 FVR_PID_FLAG_KEY_TYPE | FVR_PID_FLAG_CBC_MODE;
#endif
    rFvrPid.u1TsIndex = DMX_GetFramerIndex();
    rFvrPid.fgPktSwap = FALSE;
    rFvrPid.pfnSwapNotify = NULL;
    rFvrPid.pvSwapNotifyTag = NULL;
    rFvrPid.fgPicSearch = FALSE;
    rFvrPid.u1PicSearchMask = 0;
    rFvrPid.pfnPicSearchNotify = NULL;
    rFvrPid.pvPicSearchNotifyTag = NULL;
#ifdef CC_MT5392B
    rFvrPid.fg3DES = TRUE;
#else
    rFvrPid.u4KeyLen = 128;
#endif
    rFvrPid.fgCBCMode = FALSE;
    rFvrPid.eKeyType = FVR_KEY_BOTH;

    if(u4Flags & MTTSREC_PID_FLAG_PID)
    {
        rFvrPid.u2Pid = rPid.u2Pid;
        u4PidFlags |= FVR_PID_FLAG_PID;
    }

    if(u4Flags & MTTSREC_PID_FLAG_SCRAMBLE_ENABLE)
    {
        rFvrPid.fgScramble = rPid.fgScramble;
        u4PidFlags |= FVR_PID_FLAG_SCRAMBLE_ENABLE;
#ifndef CC_MT5392B
        u4PidFlags |= FVR_PID_FLAG_KEY_INDEX;
#endif
        // Set key
        if(rPid.u1KeyIdx >= MTTSREC_MAX_KEY_INDEX)
        {
            return MTR_NOT_OK;
        }
        if(u1Pidx >= MTTSREC_MAX_PID)
        {
            return MTR_NOT_OK;
        }
        _au1TsRecPidKeyIdx[u1Pidx] = rPid.u1KeyIdx;
#ifdef CC_MT5392B
        rFvrPid.u4EvenKey1High = _arTsRecKeyVal[rPid.u1KeyIdx].au4Key[0];
        rFvrPid.u4EvenKey1Low = _arTsRecKeyVal[rPid.u1KeyIdx].au4Key[1];
        rFvrPid.u4EvenKey2High = _arTsRecKeyVal[rPid.u1KeyIdx].au4Key[2];
        rFvrPid.u4EvenKey2Low = _arTsRecKeyVal[rPid.u1KeyIdx].au4Key[3];
        rFvrPid.u4OddKey1High = _arTsRecKeyVal[rPid.u1KeyIdx].au4Key[4];
        rFvrPid.u4OddKey1Low = _arTsRecKeyVal[rPid.u1KeyIdx].au4Key[5];
        rFvrPid.u4OddKey2High = _arTsRecKeyVal[rPid.u1KeyIdx].au4Key[6];
        rFvrPid.u4OddKey2Low = _arTsRecKeyVal[rPid.u1KeyIdx].au4Key[7];

        u4PidFlags |= (FVR_PID_FLAG_EVEN_KEY_1 | FVR_PID_FLAG_EVEN_KEY_2);
        u4PidFlags |= (FVR_PID_FLAG_ODD_KEY_1 | FVR_PID_FLAG_ODD_KEY_2);
#else
        FVR_SetAesKey(rPid.u1KeyIdx, TRUE, (UINT8 *)&(_arTsRecKeyVal[rPid.u1KeyIdx]));
        FVR_SetAesKey(rPid.u1KeyIdx, FALSE, (UINT8 *)&(_arTsRecKeyVal[rPid.u1KeyIdx]));
#endif
    }

    if(u4Flags & MTTSREC_PID_FLAG_ENABLE)
    {
        rFvrPid.fgEnable = rPid.fgEnable;
        u4PidFlags |= FVR_PID_FLAG_ENABLE;
    }

    if(u4Flags & MTTSREC_PID_FLAG_DESCRAMBLE)
    {
        rFvrPid.fgDesc = rPid.fgDesc;
        rFvrPid.u1DescKeyidx = rPid.u1DescKeyidx;
        rFvrPid.eDescMode =(DMX_DESC_MODE_T)(rPid.eDescMode);
        u4PidFlags |= FVR_PID_FLAG_DESC;
    }

    if(!FVR_SetPid(u1Pidx, u4PidFlags, &rFvrPid))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_GetPid
 *  Get record PID information
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_GetPid(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT8 u1Pidx;
    UINT32 u4Flags;
    FVR_PID_T rFvrPid;
    MTTSREC_PID_T rPid;
    MTTSREC_PID_T *prPid;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u4Flags = (UINT32)rArg.ai4Arg[1];
    prPid = (MTTSREC_PID_T*)rArg.ai4Arg[2];

    if(!FVR_GetPid(u1Pidx, FVR_PID_FLAG_ALL, &rFvrPid))
    {
        return MTR_NOT_OK;
    }

    if(u4Flags & MTTSREC_PID_FLAG_ENABLE)
    {
        rPid.fgEnable = rFvrPid.fgEnable;
    }

    if(u4Flags & MTTSREC_PID_FLAG_PID)
    {
        rPid.u2Pid = rFvrPid.u2Pid;
    }

    if(u4Flags & MTTSREC_PID_FLAG_SCRAMBLE_ENABLE)
    {
        rPid.fgScramble = rFvrPid.fgScramble;
        if(u1Pidx < MTTSREC_MAX_PID)
        {
            rPid.u1KeyIdx = _au1TsRecPidKeyIdx[u1Pidx];
        }
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prPid, MTTSREC_PID_T);
    COPY_TO_USER_ARG(prPid, rPid, MTTSREC_PID_T);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_FreePid
 *  Free record PID resource
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_FreePid(unsigned long arg)
{
    UINT8 u1Pidx;
    INT32 Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u1Pidx = (UINT8)Arg;

    if(!FVR_FreePid(u1Pidx))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_UnlockBuffer
 *  Unlock record buffer to driver
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_UnlockBuffer(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4Addr, u4Size;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Addr = (UINT32)rArg.ai4Arg[0];
    u4Size = (UINT32)rArg.ai4Arg[1];

    if(!FVR_UnlockBuffer(u4Addr, u4Size))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_FlushBuffer
 *  Update read/write pointer to buffer start
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_FlushBuffer(unsigned long arg)
{
    if(FVR_IsRecording())
    {
        MTDMX_PRINT("%s, still recording\n", __FUNCTION__);
        return MTR_NOT_OK;
    }

    if(!FVR_FlushBuffer())
    {
        return MTR_NOT_OK;
    }

    _u4TsRecPktCount = 0;

    if(_fgTsRecMwEnable)
    {
        if(!FVR_Start())
        {
            MTDMX_PRINT("%s:%d, fail\n", __FUNCTION__, __LINE__);
            return MTR_NOT_OK;
        }
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_Start
 *  Start recording
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_Start(unsigned long arg)
{
    INT32 Arg;
    BOOL fgCiPlusTimeshift;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    fgCiPlusTimeshift = (BOOL)Arg;
    FVR_TIMESTAMP_T rTimestamp;

    if(_u4TsRecStreamAddr == 0)
    {
        MTDMX_PRINT("%s, stream addr = 0\n", __FUNCTION__);
        return MTR_NOT_OK;
    }

    _u4TsRecPktCount = 0;

    rTimestamp.fgEnable = TRUE;
    rTimestamp.u2ClkDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
    rTimestamp.u4Counter = 0;
    rTimestamp.u4Mask = 0xFFFFFFFF;

    FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimestamp);

    if(!FVR_Start())
    {
        return MTR_NOT_OK;
    }

    _fgTsRecMwEnable = TRUE;

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_Stop
 *  Stop record
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_Stop(unsigned long arg)
{
    INT32 Arg;
    BOOL fgCiPlusTimeshift;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    fgCiPlusTimeshift = (BOOL)Arg;
    
    if(!FVR_Stop())
    {
        return MTR_NOT_OK;
    }

    _fgTsRecMwEnable = FALSE;

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_IsRecording
 *  Check record status
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_IsRecording(unsigned long arg)
{
    INT32 Arg;
    BOOL fgRecording;
    BOOL *pfgRecording;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    pfgRecording = (BOOL*)Arg;

    //fgRecording = FVR_IsRecording();
    fgRecording = _fgTsRecMwEnable;

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgRecording, BOOL);
    COPY_TO_USER_ARG(pfgRecording, fgRecording, BOOL);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_SetBuffer
 * Set record stream and meta buffer size
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_SetBuffer(unsigned long arg)
{
    UINT32 u4FbmSize, u4FbmAddr;
    FVR_GBL_T rGBL;
    FBM_POOL_T* prFbmPool;

    // Allocate video buffer from FBM
    ////prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX_REC_TS);
    //prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_SCPOS_PIP);
   // prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_PVR);
   prFbmPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_PVR, NULL);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);

    u4FbmAddr = prFbmPool->u4Addr;
    u4FbmSize = prFbmPool->u4Size;
    if(u4FbmSize < (MTTSREC_STREAM_SIZE + MTTSREC_META_SIZE))
    {
        MTDMX_PRINT("%s %s, buf not enough, 0x%x, 0x%x\n", __FUNCTION__, __LINE__, u4FbmSize,
                                                 (MTTSREC_STREAM_SIZE + MTTSREC_META_SIZE));
        return MTR_NOT_OK;
    }

    rGBL.fgAllocateBuffer = FALSE;
    // Linux kernel start address must page alignment
    rGBL.u4BufStart = (u4FbmAddr + (PAGE_SIZE - 1)) & PAGE_MASK;
    rGBL.u4BufSize = MTTSREC_STREAM_SIZE;
    rGBL.fgSetAlign = TRUE;
    rGBL.u4BufAlign = PAGE_SIZE;

    if(!FVR_SetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return MTR_NOT_OK;
    }

    // Get stream addr and size after align handle
    if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return MTR_NOT_OK;
    }

    _u4TsRecStreamAddr = rGBL.u4BufStart;
    _u4TsRecStreamSize = rGBL.u4BufSize;
    _u4TsRecMetaAddr = _u4TsRecStreamAddr + _u4TsRecStreamSize;
    _u4TsRecMetaSize = (VIRTUAL(u4FbmAddr + u4FbmSize) - _u4TsRecMetaAddr) & PAGE_MASK;
    if(_u4TsRecMetaSize > (MTTSREC_META_SIZE & PAGE_MASK))
    {
        _u4TsRecMetaSize = MTTSREC_META_SIZE & PAGE_MASK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** MTTSREC_GetBufferPtr
 * Get record stream and meta buffer addr
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_GetBufferPtr(unsigned long arg)
{
    INT32 Arg;
    MTTSREC_BUFFERPTR_T rBufPtr;
    MTTSREC_BUFFERPTR_T *prBufPtr;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    prBufPtr = (MTTSREC_BUFFERPTR_T*)Arg;

    rBufPtr.u4StreamAddr = _u4TsRecStreamAddr;
    rBufPtr.u4StreamSize = _u4TsRecStreamSize;
    rBufPtr.u4MetaAddr = _u4TsRecMetaAddr;
    rBufPtr.u4MetaSize = _u4TsRecMetaSize;

    USR_SPACE_ACCESS_VALIDATE_ARG(prBufPtr, MTTSREC_BUFFERPTR_T);
    COPY_TO_USER_ARG(prBufPtr, rBufPtr, MTTSREC_BUFFERPTR_T);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTTSREC_SetKey
 *  Set timeshift key
 *  Key word 0 ~ 3: even key
 *  Key word 4 ~ 7: odd key
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTTSREC_SetKey(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 i, u1KeyIdx;
    UINT32 au4Key[8];
    UINT32 *pu4Key;
#ifdef CC_MT5392B
    UINT32 au4DesKey[6];
#endif

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1KeyIdx = (UINT8)rArg.ai4Arg[0];
    pu4Key = (UINT32*)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu4Key, 32);
    COPY_FROM_USER_ARG_SIZE(pu4Key, au4Key, 32);

    if(u1KeyIdx >= MTTSREC_MAX_KEY_INDEX)
    {
        return MTR_NOT_OK;
    }

    //----------------------------------------------
    // Store key value
    //----------------------------------------------
    for(i=0; i<8; i++)
    {
        _arTsRecKeyVal[u1KeyIdx].au4Key[i] = au4Key[i];
    }

#ifdef CC_MT5392B
    //----------------------------------------------
    // Play even key
    //----------------------------------------------
    for(i=0; i<4; i++)
    {
        au4DesKey[i] = au4Key[i];
    }
    au4DesKey[4] = au4Key[0];
    au4DesKey[5] = au4Key[1];

    if(!DMX_SetDesKey(u1KeyIdx + 8, TRUE, TRUE, au4DesKey))
    {
        return MTR_NOT_OK;
    }

    //----------------------------------------------
    // Play odd key
    //----------------------------------------------
    for(i=0; i<4; i++)
    {
        au4DesKey[i] = au4Key[i+4];
    }
    au4DesKey[4] = au4Key[4];
    au4DesKey[5] = au4Key[5];

    if(!DMX_SetDesKey(u1KeyIdx + 8, TRUE, FALSE, au4DesKey))
    {
        return MTR_NOT_OK;
    }
#else
	/*For Vestel case, when set record key no need to set play back key.*/
/*

    //----------------------------------------------
    // Play even key
    //----------------------------------------------
    if(!DMX_SetAesKey(u1KeyIdx, TRUE, au4Key))
    {
        return MTR_NOT_OK;
    }
    //----------------------------------------------
    // Play odd key
    //----------------------------------------------
    if(!DMX_SetAesKey(u1KeyIdx, FALSE, au4Key))
    {
        return MTR_NOT_OK;
    }
*/
#endif

    return MTR_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int mtal_ioctl_mttsrec(unsigned int cmd, unsigned long arg)
{
    INT32 i4Ret = MTR_OK;

    switch (cmd)
    {
    case MTAL_IO_DMX_FVR_INIT:
        i4Ret = _MTTSREC_Init(arg);
        break;

    case MTAL_IO_DMX_FVR_SETGBL:
        i4Ret = _MTTSREC_SetGBL(arg);
        break;

    case MTAL_IO_DMX_FVR_GETGBL:
        i4Ret = _MTTSREC_GetGBL(arg);
        break;

    case MTAL_IO_DMX_FVR_SETPID:
        i4Ret = _MTTSREC_SetPid(arg);
        break;

    case MTAL_IO_DMX_FVR_GETPID:
        i4Ret = _MTTSREC_GetPid(arg);
        break;

    case MTAL_IO_DMX_FVR_FREEPID:
        i4Ret = _MTTSREC_FreePid(arg);
        break;

    case MTAL_IO_DMX_FVR_UNLOCK_BUF:
        i4Ret = _MTTSREC_UnlockBuffer(arg);
        break;

    case MTAL_IO_DMX_FVR_FLUSH_BUF:
        i4Ret = _MTTSREC_FlushBuffer(arg);
        break;

    case MTAL_IO_DMX_FVR_START:
        i4Ret = _MTTSREC_Start(arg);
        break;

    case MTAL_IO_DMX_FVR_STOP:
        i4Ret = _MTTSREC_Stop(arg);
        break;

    case MTAL_IO_DMX_FVR_ISRECORDING:
        i4Ret = _MTTSREC_IsRecording(arg);
        break;

    case MTAL_IO_DMX_FVR_SETBUFFER:
        i4Ret = _MTTSREC_SetBuffer(arg);
        break;

    case MTAL_IO_DMX_FVR_GETBUFFERPTR:
        i4Ret = _MTTSREC_GetBufferPtr(arg);
        break;

    case MTAL_IO_DMX_FVR_FREEGBL:
        i4Ret = _MTTSREC_FreeGBL(arg);
        break;

    case MTAL_IO_DMX_FVR_SETKEY:
        i4Ret = _MTTSREC_SetKey(arg);
        break;

    default:
        i4Ret = MTR_PARAMETER_ERROR;
        break;
    }

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** _MTTSREC_GetBufferPtr
 */
//-----------------------------------------------------------------------------
void MTTSREC_GetBufferAddr(MTTSREC_BUFFERPTR_T *prBufAddr)
{
    if(prBufAddr != NULL)
    {
        prBufAddr->u4StreamAddr = _u4TsRecStreamAddr;
        prBufAddr->u4MetaAddr = _u4TsRecMetaAddr;
    }
}

#endif // TIME_SHIFT_SUPPORT
