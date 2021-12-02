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
 * $Revision: #4 $
 * $Author: jiangbo.wang $
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
#include "mtdrec.h"
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

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MTDREC_THREAD_NAME                 "MTDRECTHR"
#define MTDREC_THREAD_STACK_SIZE           2048
#define MTDREC_THREAD_PRIORITY             100
#define MTDREC_THUMBNAIL_THREAD_NAME       "MTDRECTHUMBTHR"
#define MTDREC_THUMBNAIL_THREAD_STACK_SIZE 2048
#define MTDREC_THUMBNAIL_THREAD_PRIORITY   100

#define MTDREC_GETSTREAM188_SIZE            (94 * 1024)
#define MTDREC_GETSTREAM192_SIZE            (96 * 1024)
#define MTDREC_GETSTREAM188_LBR_SIZE        (MTDREC_GETSTREAM188_SIZE)
#define MTDREC_GETSTREAM192_LBR_SIZE        (MTDREC_GETSTREAM192_SIZE)
#define MTDREC_GETSTREAM188_HBR_SIZE        (MTDREC_GETSTREAM188_SIZE * 2)
#define MTDREC_GETSTREAM192_HBR_SIZE        (MTDREC_GETSTREAM192_SIZE * 2)
#define MTDREC_GETSTREAM188_ALIGN_SIZE      (MTDREC_GETSTREAM188_SIZE * 8)
#define MTDREC_GETSTREAM192_ALIGN_SIZE      (MTDREC_GETSTREAM192_SIZE * 8)
#define MTDREC_HITRATE_THRESHOLD            (15 * 1024) // 15 * 1024 Kbps

#define MTDREC_PIC_INDEX_ARRAY_SIZE         (512)
#define MTDREC_PIC_INDEX_ARRAY_COUNT        (10)
#define MTDREC_STREAM_STRUCT_MAX            10
#define MTDREC_TICK_PIC_COUNT               (2048)

#define MTDREC_H264ES_BUFSIZE               (1536)  // 1.5KB

#define MTDREC_CIPHER_KEY_INDEX             (0)

#define MTDREC_FRAMERATE_SAMPLE_COUNT       (3)
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT32 u4Timeout;
} MTDREC_MSG_T;

typedef struct
{
    BOOL fgValid;
    UINT8 u1Count;
    UINT32 u4IndexAddr;
} MTDREC_INDEX_INFTO_T;


 typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4PktIdx;
    UINT8 u1Offset;
    BOOL fgDataValid;
    UINT8 u1DataLen;
    UINT8 au1Data[184];
} MTDREC_PARSER_T;

typedef enum
{
    GETSTREAM_NORET,
    GETSTREAM_TIMEOUT,
    GETSTREAM_DATA_ENOUGH,
    GETSTREAM_FRAME_ENOUGH,
} MTDREC_GETSTREAM_RET_T;

typedef struct
{
    VDEC_AVC_SEI_PIC_STRUCT ePicStruct;
    BOOL fgFrameMbsOnlyFlag;
    UINT32 u4Timescale;
    UINT32 u4NumOfUnit;
} MTDREC_H264_T;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

extern MT_RESULT_T _MTPVR_ChangeThumbVdecId(UCHAR ucVdecId);
extern BOOL MTDREC_NotifyThumbnailChecking(void);
extern BOOL MTDMX_SetSecondVideoPid(UINT8 u1Pidx, UINT32 u4Flags, MTDMX_PID_T* prPid);
extern BOOL _Mtpvr_SetPlayBackMode(UINT8 u1PacketSize);

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

#define COPY_FROM_USER_ARG_SIZE_K(ArgUsr, ArgKernel,size)              \
    if (copy_from_user(ArgKernel, (void __user *)ArgUsr,   \
                       size))          \
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
        printk("mtal_ioctl_mtdmx argument error 3\n");     \
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
        printk("mtal_ioctl_mtdmx argument error 5\n");      \
        return MTR_ERR_INV;                                          \
    } \

#define PICINDEXSIZE(rp, wp, size)      \
    (((rp) <= (wp)) ? ((wp) - (rp)) : (((wp) + (size)) - (rp)))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T _hMtDrecThread = NULL_HANDLE;
static HANDLE_T _hMtDrecQueue = NULL_HANDLE;
static HANDLE_T _hMtDrecThumbThread = NULL_HANDLE;
static HANDLE_T _hMtDrecThumbQueue = NULL_HANDLE;

static BOOL _fgMtDRecLocking = FALSE;
static CRIT_STATE_T _rMtDRecLock;

static MTDREC_PID_TYPE_T _eMtDrecVideoType;

static UINT32 _u4MtDRecPicWp = 0;
static UINT32 _u4MtDRecPicRp = 0;
static MTDREC_PVR_INDEX_T _arMtDRecPicAddr[MTDREC_TICK_PIC_COUNT];

static UINT32 _u4DRecBaseAddr = 0;
static UINT32 _u4DRecBaseSize = 0;
static UINT32 _u4DRecStreamAddr = 0;
static UINT32 _u4DRecStreamSize = 0;
static UINT32 _u4DRecStreamPktCount = 0;
static UINT32 _u4DRecIndexBufAddr = 0;
static UINT32 _u4DRecIndexBufSize = 0;
static UINT32 _u4MtDRecReadPointer = 0;

static BOOL _fgDRecStreamStop = TRUE;

static MTDREC_STREAM_INFO_T _arMtDRecStreamInfo[MTDREC_STREAM_STRUCT_MAX];
static MTDREC_STREAM_DATA_T _arMtDRecStreamData[MTDREC_STREAM_STRUCT_MAX];
static UINT8 _u1MtDRecStreamDataWp = 0;
static UINT8 _u1MtDRecStreamDataRp = 0;

static UINT16 _u2MtDRecVideoPid = 0x1FFF;
static UINT8 _u2MtDRecVideoPidx = 0xFF;
//static MTDREC_PARSER_T _rMtDRecParser;

static UINT32 _u4MtDRecH264ESBuf = 0;

static UINT32 _u4MtDRecKeyLen = 128;
static BOOL _fgMTDRecEncryptCBC = FALSE;
static BOOL _fgMTDRecEncrypt = FALSE;
static BOOL _fgMTDRecUseTimestamp = TRUE;
static FVR_KEY_TYPE_T _rMTDRecKeyType = FVR_KEY_EVEN;
static UINT32 _u4MtDRecDefineBufSize = MTDREC_GETSTREAM192_LBR_SIZE;

//static UINT8 au1FrameRate[MTDREC_FRAMERATE_SAMPLE_COUNT];
//static UINT8 u1FrameRateWp = 0;
//static UINT8 u1ValidSampleCount = 0;

static MTDREC_TIMESTAMP_T eMtDrecTimestamp = MTDREC_TIMESTAMP_TYPE_4BYTE;
static UCHAR _ucThumbVdecId = ES0;
static UINT32 u4H264ActualFrameRate = 30;
// Buffer in mmap area
static MTDREC_INDEX_INFTO_T _arDDIPVRIndexArray[MTDREC_PIC_INDEX_ARRAY_COUNT];
//static UINT32 _u4DDIPVRPicIndex = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _MtDRec_Lock
 */
//-----------------------------------------------------------------------------
static void _MtDRec_Lock(void)
{
    _rMtDRecLock = x_crit_start();

    if(_fgMtDRecLocking)
    {
        MTDREC_PRINT("Already lock\n");
        ASSERT(0);
    }

    _fgMtDRecLocking = TRUE;
}


//-----------------------------------------------------------------------------
/** _MtDRec_Unlock
 */
//-----------------------------------------------------------------------------
static void _MtDRec_Unlock(void)
{
    if(!_fgMtDRecLocking)
    {
        MTDREC_PRINT("Already Unlock\n");
        ASSERT(0);
    }

    _fgMtDRecLocking = FALSE;

    x_crit_end(_rMtDRecLock);
}


//-----------------------------------------------------------------------------
/** _MtDrecGetH264VdecInfo
 */
//-----------------------------------------------------------------------------
static BOOL _MtDrecGetH264VdecInfo(MTDREC_H264_T *prH264, BOOL fgRefresh)
{
    static BOOL isFirst = TRUE;
    static MTDREC_H264_T rH264Info;
    VDEC_HDR_INFO_T rVdecHdr;

    if(isFirst)
    {
        rH264Info.ePicStruct = VDEC_AVC_SEI_FRAME;
        rH264Info.fgFrameMbsOnlyFlag = TRUE;
        rH264Info.u4Timescale = 1000;
        rH264Info.u4NumOfUnit = 30;

        isFirst = FALSE;
    }

    if(fgRefresh)
    {
        // can't update vdec info in ISR
        if(!VDEC_QueryInfo(_ucThumbVdecId, &rVdecHdr))
        {
            Printf("Fail to get h264 frame rate, use previous\n");
            prH264->ePicStruct = rH264Info.ePicStruct;
            prH264->fgFrameMbsOnlyFlag = rH264Info.fgFrameMbsOnlyFlag;
            prH264->u4Timescale = rH264Info.u4Timescale;
            prH264->u4NumOfUnit = rH264Info.u4NumOfUnit;
            return TRUE;
        }

        if(rVdecHdr.fgSPSValid)
        {
            rH264Info.fgFrameMbsOnlyFlag = rVdecHdr.fgSPSFrameMbsOnlyFlag;
        }

        if(rVdecHdr.fgVUIValid)
        {
            rH264Info.u4Timescale = rVdecHdr.u4VUITimeScale;
            rH264Info.u4NumOfUnit = rVdecHdr.u4VUINumUnitsInTick;
        }

        if(rVdecHdr.fgSEIValid)
        {
            rH264Info.ePicStruct = rVdecHdr.eSEIPicStruct;
        }
    }

    prH264->ePicStruct = rH264Info.ePicStruct;
    prH264->fgFrameMbsOnlyFlag = rH264Info.fgFrameMbsOnlyFlag;
    prH264->u4Timescale = rH264Info.u4Timescale;
    prH264->u4NumOfUnit = rH264Info.u4NumOfUnit;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MtDrecSetH264FrameInfo
 */
//-----------------------------------------------------------------------------
static void _MtDrecSetH264FrameInfo(UINT32 u4PicCount, HAL_TIME_T *prTime)
{
    static UINT8 u1ErrorStreamCounter = 0;
    UINT32 u4FloatFrameRate;
    UINT32 u4CalculateFrameRate;
    UINT32 u4ActualFrameRate;
    MTDREC_H264_T rH264;

    u4FloatFrameRate = (u4PicCount * 1000) /
        ((prTime->u4Seconds * 1000) + (prTime->u4Micros/1000));

    if(!_MtDrecGetH264VdecInfo(&rH264, FALSE))
    {
        return;
    }

    if(rH264.fgFrameMbsOnlyFlag)
    {
        u4CalculateFrameRate = rH264.u4Timescale / (2 * rH264.u4NumOfUnit);
    }
    else
    {
        u4CalculateFrameRate = rH264.u4Timescale / (rH264.u4NumOfUnit);
    }

    // check error stream
    if((10 * u4FloatFrameRate) > (15 * u4CalculateFrameRate))
    {
        if(u1ErrorStreamCounter < 3)
        {
            u1ErrorStreamCounter++;
        }
    }
    else
    {
        if(u1ErrorStreamCounter > 0)
        {
            u1ErrorStreamCounter--;
        }
    }

    if(u1ErrorStreamCounter >= 3)
    {
        u4ActualFrameRate = u4CalculateFrameRate * 2;
    }
    else
    {
        u4ActualFrameRate = u4CalculateFrameRate;
    }

    MTDREC_PRINT("u4ActualFrameRate: %d %d %d %d\n", u4ActualFrameRate, u4FloatFrameRate, u4CalculateFrameRate, u1ErrorStreamCounter);

    _MtDRec_Lock();
    u4H264ActualFrameRate = u4ActualFrameRate;
    _MtDRec_Unlock();
}

//-----------------------------------------------------------------------------
/** _MtDrecGetH264FrameRate
 */
//-----------------------------------------------------------------------------
static MTDREC_PVR_FRAMERATE_T _MtDrecGetH264FrameRate(void)
{
    UINT32 u4ActualFrameRate;
    MTDREC_PVR_FRAMERATE_T eFrameRate;
    MTDREC_H264_T rH264;
    _MtDRec_Lock();
    u4ActualFrameRate = u4H264ActualFrameRate;
    _MtDRec_Unlock();

    if(u4ActualFrameRate < 24)
    {
        eFrameRate = MTDREC_PVR_FR_23_976HZ;
    }
    else if(u4ActualFrameRate == 24)
    {
        eFrameRate = MTDREC_PVR_FR_24HZ;
    }
    else if(u4ActualFrameRate < 28)
    {
        eFrameRate = MTDREC_PVR_FR_25HZ;
    }
    else if(u4ActualFrameRate < 30)
    {
        eFrameRate = MTDREC_PVR_FR_29_97HZ;
    }
    else if(u4ActualFrameRate < 40)
    {
        eFrameRate = MTDREC_PVR_FR_30HZ;
    }
    else if(u4ActualFrameRate < 55)
    {
        eFrameRate = MTDREC_PVR_FR_50HZ;
    }
    else if(u4ActualFrameRate < 60)
    {
        eFrameRate = MTDREC_PVR_FR_59_94HZ;
    }
    else
    {
        eFrameRate = MTDREC_PVR_FR_60HZ;
    }

    if(!_MtDrecGetH264VdecInfo(&rH264, TRUE))
    {
        ;
    }

    return eFrameRate;
}

#if 0
//-----------------------------------------------------------------------------
/** _MtDrecCheckPicStructInSEI
 */
//-----------------------------------------------------------------------------
static UINT32 _MtDrecCheckPicStructInSEI(void)
{
    MTDREC_H264_T rH264;
    UINT32 u4IndexMultiple = 1;

    if(!_MtDrecGetH264VdecInfo(&rH264, FALSE))
    {
        return u4IndexMultiple;
    }

	switch (rH264.ePicStruct)
	{
	case VDEC_AVC_SEI_FRAME:
		if(rH264.fgFrameMbsOnlyFlag)
		{
			u4IndexMultiple += 0;
		}
		else
		{
			u4IndexMultiple += 1;
		}
		break;
	case VDEC_AVC_SEI_TOP_FIELD:
	case VDEC_AVC_SEI_BOTTOM_FIELD:
	case VDEC_AVC_SEI_TOP_BOTTOM:
	case VDEC_AVC_SEI_BOTTOM_TOP:
		u4IndexMultiple += 1;
		break;
	case VDEC_AVC_SEI_TOP_BOTTOM_TOP:
	case VDEC_AVC_SEI_BOTTOM_TOP_BOTTOM:
		u4IndexMultiple += 2;
		break;
	case VDEC_AVC_SEI_FRAME_DOUBLING:
		if(rH264.fgFrameMbsOnlyFlag)
		{
			u4IndexMultiple += 1;
		}
		else
		{
			u4IndexMultiple += 3;
		}
		break;
	case VDEC_AVC_SEI_FRAME_TRIPLING:
		if(rH264.fgFrameMbsOnlyFlag)
		{
			u4IndexMultiple += 2;
		}
		else
		{
			u4IndexMultiple += 5;
		}
		break;
    case VDEC_AVC_SEI_FRAME_UNKNOWN:
    default:
        Printf("Unknow 264 pic struct\n");
        u4IndexMultiple += 0;
        break;
	}

    MTDREC_PRINT("Index: %d %d %d\n", u4IndexMultiple, rH264.ePicStruct, rH264.fgFrameMbsOnlyFlag);

	return u4IndexMultiple;
}
#endif

//-----------------------------------------------------------------------------
/** _MtDrecAddPicCount
 */
//-----------------------------------------------------------------------------
static void _MtDrecAddPicCount(void)
{
    static BOOL fgFirst = TRUE;
    static UINT8 u4PicCount = 0;
    static HAL_TIME_T rStartTime;
    HAL_TIME_T rDiffTime;
    HAL_TIME_T rCurTime;

    if(fgFirst)
    {
        //init start time
        HAL_GetTime(&rStartTime);
        fgFirst = FALSE;
    }

    HAL_GetTime(&rCurTime);
    HAL_GetDeltaTime(&rDiffTime, &rStartTime, &rCurTime);
    if(rDiffTime.u4Seconds >= 1)
    {
        //save the current pic count, and caculate h264 frame rate
        FVR_VIDEO_TYPE_T eVideoType;
        eVideoType = FVR_GetVideoTypeByPidx(_u2MtDRecVideoPidx);
        if(eVideoType == FVR_VIDEO_H264)
        {
            _MtDrecSetH264FrameInfo(u4PicCount, &rDiffTime);
        }
        //reset pic count to 1
        u4PicCount = 1;
        //store the current time to next new start time
        x_memcpy(&rStartTime, &rCurTime, sizeof(HAL_TIME_T));
    }
    else
    {
        //accumulate pic count
        u4PicCount++;
    }
}


//-----------------------------------------------------------------------------
/** _MtTsDRecPicCallback
 */
//-----------------------------------------------------------------------------
static void _MtTsDRecPicCallback(UINT8 u1Pidx, UINT8 u1Type, UINT32 u4PktIdx,
                              const void* pvTag)
{
    UINT32 u4IndexMultiple = 1, i;
    FVR_VIDEO_TYPE_T eVideoType;
    MTDREC_PVR_INDEX_TYPE_T eType;

    u4PktIdx--;

    eVideoType = FVR_GetVideoTypeByPidx(_u2MtDRecVideoPidx);
    eType = MTDREC_PVR_INDEX_TYPE_NONE;
    if(eVideoType == FVR_VIDEO_MPEG)
    {
        if((u1Type & 0x9) == 0x9)
        {
            eType = MTDREC_PVR_INDEX_TYPE_SEQ_I_PIC;
        }
        else
        {
            switch(u1Type & 0xF)
            {
            case 0x1:
                eType = MTDREC_PVR_INDEX_TYPE_I_PIC;
                break;
            case 0x2:
                eType = MTDREC_PVR_INDEX_TYPE_P_PIC;
                break;
            case 0x4:
                eType = MTDREC_PVR_INDEX_TYPE_B_PIC;
                break;
            case 0x8:
                eType = MTDREC_PVR_INDEX_TYPE_SEQ;
                break;
            default:
                break;
            }
        }
        u4IndexMultiple = 1;
    }
    else if(eVideoType == FVR_VIDEO_H264)
    {
        if(((u1Type & 0x2) == 0x2) && ((u1Type & 0xD) != 0x0))
        {
            eType = MTDREC_PVR_INDEX_TYPE_SEQ_I_PIC;
        }
        else if((u1Type & 0xD) != 0x0)
        {
            eType = MTDREC_PVR_INDEX_TYPE_I_PIC;
        }
        else if((u1Type & 0x2) == 0x2)
            {
                eType = MTDREC_PVR_INDEX_TYPE_SEQ;
        }
        else if((u1Type & 0x30) != 0x0)
        {
                eType = MTDREC_PVR_INDEX_TYPE_P_PIC;
        }
        else if((u1Type & 0xC0) != 0x0)
        {
                eType = MTDREC_PVR_INDEX_TYPE_B_PIC;
            }
        else
        {
            Printf("Which pic type?:0x%x\n", u1Type);
        }

        //u4IndexMultiple = _MtDrecCheckPicStructInSEI();
        u4IndexMultiple = 1;
    }

    for(i = 0; i < u4IndexMultiple; i++)
    {
        _MtDRec_Lock();
        _arMtDRecPicAddr[_u4MtDRecPicWp].eIndexType= eType;
        _arMtDRecPicAddr[_u4MtDRecPicWp].u4Offset = (u4PktIdx % _u4DRecStreamPktCount) * 192;
        _u4MtDRecPicWp++;
        _u4MtDRecPicWp %= MTDREC_TICK_PIC_COUNT;
        _MtDRec_Unlock();
        _MtDrecAddPicCount();
    }

    UNUSED(pvTag);
}


//-----------------------------------------------------------------------------
/** _MtDRecFormatIndexArray
 */
//-----------------------------------------------------------------------------
static UINT8 _MtDRecFormatIndexArray(UINT32 u4RecordSize)
{
    UINT32 i, u4StartCodeIdx,u4PicDiff,  u4PicRp;
    UINT32 u4DataSize;
    MTDREC_PVR_INDEX_T *prPVRIndex;
    BOOL fgFoundSeqSPS;
    UINT32 u4Offset;

    //Printf("format idx:%d\n", u1RetArrayIdx);

    ASSERT(_u1MtDRecStreamDataWp < MTDREC_STREAM_STRUCT_MAX);
    if(_u1MtDRecStreamDataWp >= MTDREC_STREAM_STRUCT_MAX)
    {
        Printf("stread data WP is wrong: %d\n", _u1MtDRecStreamDataWp);
        return 0xFF;
    }

    _MtDRec_Lock();
    // Calcaulate frame offset
    u4PicDiff = PICINDEXSIZE(_u4MtDRecPicRp, _u4MtDRecPicWp, MTDREC_TICK_PIC_COUNT);
    u4StartCodeIdx = _u4MtDRecPicRp;
    prPVRIndex = (MTDREC_PVR_INDEX_T*)_arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexAddr;
    u4Offset = _u4MtDRecReadPointer - _u4DRecStreamAddr;
    u4PicRp = _u4MtDRecPicRp;
    _MtDRec_Unlock();

    fgFoundSeqSPS = FALSE;

    //Printf("prPVRIndex: 0x%x\n", prPVRIndex);

    ASSERT(u4PicDiff < MTDREC_PIC_INDEX_ARRAY_SIZE);
    if(u4PicDiff >= MTDREC_PIC_INDEX_ARRAY_SIZE)
    {
        Printf("wrong picdiff: %d\n", u4PicDiff);
        return 0xFF;
    }

    for(i=0; i<u4PicDiff; i++)
    {
        _MtDRec_Lock();
        u4DataSize = DATASIZE(u4Offset, _arMtDRecPicAddr[u4PicRp].u4Offset, _u4DRecStreamSize);
        _MtDRec_Unlock();

        if(u4DataSize > u4RecordSize)
        {
            // Out of record bound
            //Printf("Out of record bound\n");
            break;
        }

        prPVRIndex->u4Offset = u4DataSize;

        // Find SeqSPS
        _MtDRec_Lock();
        if(fgFoundSeqSPS)
        {
            // For H264 frame rate, calculate serach range
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4SPSNextStartLen = prPVRIndex->u4Offset -
                        _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4SeqSPSOffset;
        }

        prPVRIndex->eIndexType = _arMtDRecPicAddr[u4PicRp].eIndexType;
        _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexSize++;

        //Printf("got idx:%d, offset:0x%x Index: %d\n", u4PicRp, prPVRIndex->u4Offset, prPVRIndex->eIndexType);

        if(!fgFoundSeqSPS)
        {
            if((prPVRIndex->eIndexType == MTDREC_PVR_INDEX_TYPE_SEQ) ||
               (prPVRIndex->eIndexType == MTDREC_PVR_INDEX_TYPE_SEQ_I_PIC))
            {
                fgFoundSeqSPS = TRUE;
                _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].fgFoundSeqSPS = TRUE;
                _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4SeqSPSOffset = prPVRIndex->u4Offset;
                _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4SPSNextStartLen = 0;

                //Printf("Got SPS, %d\n", u4PicRp);
            }
        }
        _MtDRec_Unlock();

        u4PicRp++;
        u4PicRp %= MTDREC_TICK_PIC_COUNT;

        prPVRIndex++;
    }

    _MtDRec_Lock();
    _u4MtDRecPicRp = u4PicRp;
    _MtDRec_Unlock();

    return _u1MtDRecStreamDataWp;

}

#if 0
//-----------------------------------------------------------------------------
/** _MtStreamGetBytes
 */
//-----------------------------------------------------------------------------
static BOOL _MtStreamGetBytes(UINT8* pu1Byte, BOOL fgFrameRate)
{
    UINT8 *pu1Data;
    UINT16 u2Pid;
    UINT8 u1AdaptionLen, u1PktSize;

    *pu1Byte = 0xFF;

    if(_rMtDRecParser.fgDataValid)
    {
        _rMtDRecParser.u1Offset++;
        ASSERT(_rMtDRecParser.u1Offset < 184);
        if(_rMtDRecParser.u1Offset >= 184)
        {
            return FALSE;
        }
        if(_rMtDRecParser.u1Offset < _rMtDRecParser.u1DataLen)
        {
            *pu1Byte = _rMtDRecParser.au1Data[_rMtDRecParser.u1Offset];
            return TRUE;
        }

        _rMtDRecParser.fgDataValid = FALSE;
        _rMtDRecParser.u1DataLen = 0;
        _rMtDRecParser.u1Offset = 0;
    }

    // ****************************
    // Handle TS packet
    // ****************************
    while(1)
    {
        u1PktSize = _fgMTDRecUseTimestamp ? 192 : 188;
        if(((_rMtDRecParser.u4PktIdx + 1) * u1PktSize) >= _rMtDRecParser.u4Size)
        {
            return FALSE;
        }

        if(fgFrameRate)   // Only H.264 frame rate need to check u4SPSNextStartLen
        {
            ASSERT(_u1MtDRecStreamDataRp < MTDREC_STREAM_STRUCT_MAX);
            if(_u1MtDRecStreamDataRp >= MTDREC_STREAM_STRUCT_MAX)
            {
                return FALSE;
            }
            
            if(((_rMtDRecParser.u4PktIdx + 1) * u1PktSize) >= 
                          _arMtDRecStreamInfo[_u1MtDRecStreamDataRp].u4SPSNextStartLen)
            {
                return FALSE;
            }
        }

        if(_fgMTDRecUseTimestamp)
        {
            // Skip 4-byte timestamp
            pu1Data = (UINT8*)(_rMtDRecParser.u4Addr + (192 * _rMtDRecParser.u4PktIdx) + 4);
        }
        else
        {
            pu1Data = (UINT8*)(_rMtDRecParser.u4Addr + (188 * _rMtDRecParser.u4PktIdx));
        }
        _rMtDRecParser.u4PktIdx++;

        u2Pid = ((UINT16)(pu1Data[1] & 0x1F) << 8) | (UINT16)pu1Data[2];
        if((u2Pid == 0x1FFF) || (u2Pid != _u2MtDRecVideoPid))        {
            
            continue;
        }

        u1AdaptionLen = 0;
        if((pu1Data[3] & 0x30) == 0x20) // Adaption only
        {
            continue;
        }
        
        if((pu1Data[3] & 0x30) == 0x30) // Contain adaption
        {
            u1AdaptionLen = pu1Data[4] + 1;
        }

        if(u1AdaptionLen >= 184)
        {
            continue;
        }

        _rMtDRecParser.u1DataLen = 184 - u1AdaptionLen;
        x_memcpy(_rMtDRecParser.au1Data, &(pu1Data[4 + u1AdaptionLen]), _rMtDRecParser.u1DataLen);
        _rMtDRecParser.fgDataValid = TRUE;
        _rMtDRecParser.u1Offset = 0;
        *pu1Byte = _rMtDRecParser.au1Data[0];
        return TRUE;
    }

    return FALSE;
}
#endif
//-----------------------------------------------------------------------------
/** _MtDRecSearchMPEG2FrameRate
 */
//-----------------------------------------------------------------------------
static MTDREC_PVR_FRAMERATE_T _MtDRecGetMPEG2FrameRate(void)
{
    VDEC_HDR_INFO_T rVdecHdr;
    static MTDREC_PVR_FRAMERATE_T ePreviousFrameRate = MTDREC_PVR_FR_23_976HZ;

    if(!VDEC_QueryInfo(_ucThumbVdecId, &rVdecHdr))
    {
        MTDREC_PRINT("Fail to get Mpeg frame rate, previous: %d\n", ePreviousFrameRate);
        return ePreviousFrameRate;
}

    switch(rVdecHdr.u2FrmRate)
        {
    case 2397:
        ePreviousFrameRate = MTDREC_PVR_FR_23_976HZ;
            break;
    case 2400:
        ePreviousFrameRate = MTDREC_PVR_FR_24HZ;
        break;
    case 2500:
        ePreviousFrameRate = MTDREC_PVR_FR_25HZ;
        break;
    case 2997:
        ePreviousFrameRate = MTDREC_PVR_FR_29_97HZ;
        break;
    case 3000:
        ePreviousFrameRate = MTDREC_PVR_FR_30HZ;
        break;
    case 5000:
        ePreviousFrameRate = MTDREC_PVR_FR_50HZ;
        break;
    case 5994:
        ePreviousFrameRate = MTDREC_PVR_FR_59_94HZ;
        break;
    case 6000:
        ePreviousFrameRate = MTDREC_PVR_FR_60HZ;
        break;
    default:
        Printf("Unknown frame rate %d\n", rVdecHdr.u2FrmRate);
        ePreviousFrameRate = MTDREC_PVR_FR_UNKNOWN;
        break;
    }

    MTDREC_PRINT("Get Mpeg2 frame rate: %d, id: %d\n", rVdecHdr.u2FrmRate, ePreviousFrameRate);

    return ePreviousFrameRate;
}

//-----------------------------------------------------------------------------
/** _MtDRecSearchH264FrameRate
 */
//-----------------------------------------------------------------------------
#if 0
static MTDREC_PVR_FRAMERATE_T _MtDRecSearchH264FrameRate(void)
{
    UINT32 u4CompareWord;
    UINT8 u1Byte;
    BOOL fgNextStage = FALSE;
    MTDREC_PVR_FRAMERATE_T eFrameRate;
    UINT32 i, u4CopySize, u4FrameLen;
    UINT8 *pu1Tmp, u1FrameRate;
    
    // ********************************
    // Find SEQ start code
    // ********************************
    eFrameRate = MTDREC_PVR_FR_UNKNOWN;
    u4CompareWord = 0;
    fgNextStage= FALSE;
    while(_MtStreamGetBytes(&u1Byte, FALSE))
    {
        u4CompareWord = ((u4CompareWord >> 8) & 0x00FFFFFF) | ((UINT32)u1Byte << 24);
        if((u4CompareWord & 0x1FFFFFFF) == 0x08010000)
        {
            fgNextStage = TRUE;
            break;
        }
        
    }

    if(!fgNextStage)
    {
        return MTDREC_PVR_FR_UNKNOWN;
    }

    ASSERT(_u1MtDRecStreamDataWp < MTDREC_STREAM_STRUCT_MAX);
    if(_u1MtDRecStreamDataWp >= MTDREC_STREAM_STRUCT_MAX)
    {
        return MTDREC_PVR_FR_UNKNOWN;
    }

    u4CopySize = _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4SPSNextStartLen;
    pu1Tmp = (UINT8*)_u4MtDRecH264ESBuf;
    u4FrameLen = 0;
    for(i=0; i<u4CopySize; i++)
    {
        if(_MtStreamGetBytes(&u1Byte,TRUE))
        {
            u4FrameLen++;
            pu1Tmp[i] = u1Byte;
        }
        else
        {
            break;
        }
    }

    //HalFlushInvalidateDCache();
    HalFlushInvalidateDCacheMultipleLine(_u4MtDRecH264ESBuf, u4FrameLen);

    // Call H264 frame rate search function
    // 0 as playback, 1 for record
#ifdef CC_NEW_PVR    
    u1FrameRate = VDEC_H264GetFRateFromSPS(1, (UINT8*)_u4MtDRecH264ESBuf, u4FrameLen);
#else
    u1FrameRate = 0;
#endif    
    switch(u1FrameRate)
    {
    case MPEG_FRAME_RATE_24_:
        eFrameRate = MTDREC_PVR_FR_23_976HZ;
        break;
    case MPEG_FRAME_RATE_24:
        eFrameRate = MTDREC_PVR_FR_24HZ;
        break;
    case MPEG_FRAME_RATE_25:
        eFrameRate = MTDREC_PVR_FR_25HZ;
        break;
    case MPEG_FRAME_RATE_30_:
        eFrameRate = MTDREC_PVR_FR_29_97HZ;
        break;
    case MPEG_FRAME_RATE_30:
        eFrameRate = MTDREC_PVR_FR_30HZ;
        break;
    case MPEG_FRAME_RATE_50:
        eFrameRate = MTDREC_PVR_FR_50HZ;
        break;
    case MPEG_FRAME_RATE_60_:
        eFrameRate = MTDREC_PVR_FR_59_94HZ;
        break;
    case MPEG_FRAME_RATE_60:
        eFrameRate = MTDREC_PVR_FR_60HZ;
        break;
    }

    return eFrameRate;
}
#endif

//-----------------------------------------------------------------------------
/** _MtDRecStreamSearchFrameRate
 */
//-----------------------------------------------------------------------------
static MTDREC_PVR_FRAMERATE_T _MtDRecStreamSearchFrameRate(UINT32 u4Addr, UINT32 u4Size)
{
    FVR_VIDEO_TYPE_T eVideoType;
    MTDREC_PVR_FRAMERATE_T eFrameRate = MTDREC_PVR_FR_25HZ;

    eVideoType = FVR_GetVideoTypeByPidx(_u2MtDRecVideoPidx);

    if(eVideoType == FVR_VIDEO_MPEG)
    {
        eFrameRate = _MtDRecGetMPEG2FrameRate();
		//printf("MPEG2 FrameRate = %d \n", eFrameRate);
    }
    else if(eVideoType == FVR_VIDEO_H264)
    {
        eFrameRate = _MtDrecGetH264FrameRate();
		//printf("H264 FrameRate = %d \n", eFrameRate);
    }

    return eFrameRate;
}


//-----------------------------------------------------------------------------
/** _MtDRecStreamPostProcessing
 */
//-----------------------------------------------------------------------------
static void _MtDRecStreamPostProcessing(UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 i, u4BufStart, u4BufEnd, u4CopySize;
    UINT32 *pu4Tmp1, *pu4Tmp2;
    UINT32 u4Timestamp1, u4Timestamp2, u4TimeDiff;
    BOOL fgSeqSPS;
    UINT32 u4SeqSPSAddr, u4FRPreIndex;
    MTDREC_PVR_FRAMERATE_T eFrameRate;
    UINT8 *pu1Tmp;

    _MtDRec_Lock();
    u4BufStart = _u4DRecStreamAddr;
    u4BufEnd = _u4DRecStreamAddr + _u4DRecStreamSize;
    _MtDRec_Unlock();

    ASSERT((u4Addr >= u4BufStart) && (u4Addr < u4BufEnd));

    // *******************************
    // If ring, copy to linear
    // *******************************
    if((u4Addr + u4Size) > u4BufEnd)
    {
        Printf("should not have this case!!!\n");
        u4CopySize = u4Addr + u4Size - u4BufEnd;

        ASSERT(((u4Addr%4) == 0) && ((u4Size%4) == 0));

        // Flush cache, no need
        HalInvalidateDCacheMultipleLine(u4BufStart, u4CopySize);
        //HalInvalidateDCache();

        // Memory copy
        pu4Tmp1 = (UINT32*)u4BufStart;
        pu4Tmp2 = (UINT32*)u4BufEnd;
        for(i=0; i<(u4CopySize/4); i++)
        {
            *(UINT32*)(pu4Tmp2 + i) = *(UINT32*)(pu4Tmp1 + i);
        }

        HalFlushDCacheMultipleLine(u4BufEnd, u4CopySize);
    }

    // Flush cache
    //HalFlushInvalidateDCache();
    //HalFlushInvalidateDCacheMultipleLine(u4Addr, u4Size);

    if((u4Addr + u4Size) > u4BufEnd)
    {
        Printf("should not have this case!!!\n");
        u4CopySize = u4Addr + u4Size - u4BufEnd;
        HalInvalidateDCacheMultipleLine(u4Addr, u4Size - u4CopySize);
        HalInvalidateDCacheMultipleLine(u4BufStart, u4CopySize);
    }
    else
    {
        HalInvalidateDCacheMultipleLine(u4Addr, u4Size);
    }

    // *******************************
    // Calculate bitrate
    // *******************************
    pu1Tmp = (UINT8*)u4Addr;
    u4Timestamp1 = (((UINT32)pu1Tmp[0] << 24) | ((UINT32)pu1Tmp[1] << 16) | ((UINT32)pu1Tmp[2] << 8)
                    | (UINT32)pu1Tmp[3]);
    pu1Tmp = (UINT8*)(u4Addr + u4Size - 192);
    u4Timestamp2 = (((UINT32)pu1Tmp[0] << 24) | ((UINT32)pu1Tmp[1] << 16) | ((UINT32)pu1Tmp[2] << 8)
                    | (UINT32)pu1Tmp[3]);

    if(u4Timestamp2 < u4Timestamp1)
    {
        u4TimeDiff = u4Timestamp2 + (0xFFFFFFFF - u4Timestamp1);
    }
    else
    {
        u4TimeDiff = u4Timestamp2 - u4Timestamp1;
    }
//    Printf("time:0x%x, 0x%x, 0x%x, packet:0x%x\n", u4Timestamp1, u4Timestamp2, u4TimeDiff,
//                        *(UINT32*)(u4Addr+4));

    _MtDRec_Lock();

    ASSERT(_u1MtDRecStreamDataWp < MTDREC_STREAM_STRUCT_MAX);
    if(_u1MtDRecStreamDataWp >= MTDREC_STREAM_STRUCT_MAX)
    {
        _MtDRec_Unlock();
        return;
    }

    _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4BitRate = u4Size * (UINT32)(27000000 / u4TimeDiff) * 8 / 1024;
    fgSeqSPS = _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].fgFoundSeqSPS;
    u4SeqSPSAddr = u4Addr + _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4SeqSPSOffset;

    if(_arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4BitRate >= MTDREC_HITRATE_THRESHOLD)
    {
        _u4MtDRecDefineBufSize = (eMtDrecTimestamp == MTDREC_TIMESTAMP_TYPE_4BYTE)?
            MTDREC_GETSTREAM192_HBR_SIZE:MTDREC_GETSTREAM188_HBR_SIZE;
    }
    else
    {
        _u4MtDRecDefineBufSize = (eMtDrecTimestamp == MTDREC_TIMESTAMP_TYPE_4BYTE)?
            MTDREC_GETSTREAM192_LBR_SIZE:MTDREC_GETSTREAM188_LBR_SIZE;
    }

    // *******************************
    // Search frame rate
    // *******************************
    if(_u1MtDRecStreamDataWp == 0)
    {
        u4FRPreIndex = MTDREC_STREAM_STRUCT_MAX - 1;
    }
    else
    {
        u4FRPreIndex = _u1MtDRecStreamDataWp - 1;
    }
    _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].eFrameRate =
                            _arMtDRecStreamInfo[u4FRPreIndex].eFrameRate;
    _MtDRec_Unlock();

    if(fgSeqSPS)
    {
        eFrameRate = _MtDRecStreamSearchFrameRate(u4SeqSPSAddr, u4Size);
        _MtDRec_Lock();
        _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].eFrameRate = eFrameRate;
        _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate = 0;
        switch(eFrameRate)
        {
        case MTDREC_PVR_FR_23_976HZ:
        case MTDREC_PVR_FR_24HZ:
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate = 24;
            break;
        case MTDREC_PVR_FR_25HZ:
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate = 25;
            break;
        case MTDREC_PVR_FR_29_97HZ:
        case MTDREC_PVR_FR_30HZ:
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate = 30;
            break;
        case MTDREC_PVR_FR_50HZ:
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate = 50;
            break;
        case MTDREC_PVR_FR_59_94HZ:
        case MTDREC_PVR_FR_60HZ:
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate  = 60;
            break;
        default:
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate  = 30;
            break;
        }
        _MtDRec_Unlock();
        //Printf("search frame rate: %d\n", (UINT32)eFrameRate);
    }
}


//-----------------------------------------------------------------------------
/** _MtDRecFreeThumbnailPid
 */
//-----------------------------------------------------------------------------
BOOL _MtDRecFreeThumbnailPid(UINT16 u2Pid, BOOL fgForce)
{
    DMX_PID_T rDrvPid;

    if(fgForce)
        {
        Printf("Force to free a thumbnail video PID\n");
        DMX_FreePid(31);
        VDEC_Stop(ES1);
        _MTPVR_ChangeThumbVdecId(ES0);
            _MtDRec_Lock();
        _ucThumbVdecId = ES0;
                _MtDRec_Unlock();
            }
    else
    {
        DMX_GetPid(31, MTDMX_PID_FLAG_ALL, &rDrvPid);
        if((rDrvPid.fgEnable == TRUE) && (rDrvPid.u2Pid == u2Pid))
        {
            Printf("Free a specific thumbnail video PID\n");
            DMX_FreePid(31);

            // isaac
            VDEC_Stop(ES1);
            _MTPVR_ChangeThumbVdecId(ES0);
            _MtDRec_Lock();
            _ucThumbVdecId = ES0;
                _MtDRec_Unlock();
        }
            }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MtDRecSetThumbnailPid
 */
//-----------------------------------------------------------------------------
BOOL _MtDRecSetThumbnailPid(UINT8 u1Pidx, BOOL fgEnable)
            {
    MTDMX_PID_T rPid;
    BOOL fgRet;
    ENUM_VDEC_FMT_T eVdecFmt;

    // It is not recording time
    if(_fgDRecStreamStop)
            {
        return TRUE;
            }

    x_memset(&rPid, 0, sizeof(MTDMX_PID_T));

    if(fgEnable)
            {
        // free the original thumbnail pidx & set the current pid
        _MtDRecFreeThumbnailPid(0, TRUE);

        rPid.fgEnable = fgEnable;
        rPid.u2Pid = _u2MtDRecVideoPid;
        rPid.ePidType = MTDMX_PID_TYPE_ES_VIDEO;

        fgRet = MTDMX_SetSecondVideoPid(u1Pidx, MTDMX_PID_FLAG_ALL, &rPid);

        // call a Isaac function
        if(_eMtDrecVideoType == DREC_TYPE_VIDEO_MPEG2)
            {
            eVdecFmt = VDEC_FMT_MPV;
            }
        else if(_eMtDrecVideoType == DREC_TYPE_VIDEO_H264)
            {
            eVdecFmt = VDEC_FMT_H264;
            }
        else if(_eMtDrecVideoType == DREC_TYPE_VIDEO_AVS)
            {
            eVdecFmt = VDEC_FMT_AVS;
            }
        else
            {
            Printf("Unknow video format(%d)\n", _eMtDrecVideoType);
            return fgRet;
        }
        VDEC_PlayNotDisplay(ES1, TRUE);
        VDEC_SetMMParam(ES1, VDEC_MM_PARSING_INFO, (UINT32)TRUE, 0, 0);
        VDEC_Play(ES1, eVdecFmt);
        _MTPVR_ChangeThumbVdecId(ES1);
                _MtDRec_Lock();
        _ucThumbVdecId = ES1;
        _MtDRec_Unlock();
                }
                else
                {
        rPid.fgEnable = fgEnable;
        rPid.u2Pid = _u2MtDRecVideoPid;
        rPid.ePidType = MTDMX_PID_TYPE_ES_VIDEO;

        fgRet = MTDMX_SetSecondVideoPid(u1Pidx, MTDMX_PID_FLAG_VALID, &rPid);
                }

    return fgRet;
                }


static VOID _MtDRecThread(VOID* pvArg)
{
    MTDREC_MSG_T rMsg;
    INT32 i4Ret;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex;
    FVR_GBL_T rGBL;
    UINT32 u4RecSize, u4Timeout;
    UINT32   u4ReadPointer;
    MTDREC_GETSTREAM_RET_T eReturn = GETSTREAM_NORET;
    DREC_MTAL_CB_T rCB;    
    UINT32 u4ElapseTime = 0;

    UNUSED(pvArg);

    zMsgSize = sizeof (rMsg);
    while (1)
    {
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
            &_hMtDrecQueue, 1, X_MSGQ_OPTION_WAIT);

        // If not start, continue
        u4Timeout = rMsg.u4Timeout;
        eReturn = GETSTREAM_NORET;
        u4ElapseTime = 0;
        while(1)
        {
            _MtDRec_Lock();
            if(_fgDRecStreamStop)
            {
                _MtDRec_Unlock();
                x_memset((void*)&rCB, 0, sizeof(rCB));
                rCB.result = TRUE;
                // return not ok
                UNUSED(_CB_PutEvent(CB_MTAL_DREC_TRIGGER, sizeof (DREC_MTAL_CB_T), &rCB));
                break;
            }

            ASSERT(_u1MtDRecStreamDataWp < MTDREC_STREAM_STRUCT_MAX);
            if(_u1MtDRecStreamDataWp >= MTDREC_STREAM_STRUCT_MAX)
            {
                _MtDRec_Unlock();
                break;
            }
            
            ASSERT(!_arMtDRecStreamData[_u1MtDRecStreamDataWp].fgValid);
            if(_arMtDRecStreamData[_u1MtDRecStreamDataWp].fgValid)
            {
                _MtDRec_Unlock();
                MTDREC_PRINT("No available stream array\n");
                x_thread_delay(20);
                continue;
            }

            // Reset stream info
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].eFrameRate = MTDREC_PVR_FR_UNKNOWN;
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].fgFoundSeqSPS = FALSE;
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4BitRate = 0;
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate = 0;
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4SeqSPSOffset = 0;
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4SPSNextStartLen = 0;
            
            u4ReadPointer = _u4MtDRecReadPointer;
            _MtDRec_Unlock();

            x_memset((void*)&rGBL, 0, sizeof(rGBL));
            if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
            {
                ASSERT(0);
                //break;
            }
#ifdef CC_NEW_PVR            
            if(!FVR_UpdateWritePointer(rGBL.u4Wp))
            {
                ASSERT(0);
                //break;
            }
#endif
            u4RecSize = DATASIZE(u4ReadPointer, rGBL.u4Wp, rGBL.u4BufSize);

            if(u4ElapseTime > u4Timeout)
            {
                Printf("%s, timeout\n", __FUNCTION__);
                if(u4RecSize < (24 * 1024))
                {
                    x_memset((void*)&rCB, 0, sizeof(rCB));
                    rCB.result = FALSE;
                    // return not ok
                    UNUSED(_CB_PutEvent(CB_MTAL_DREC_TRIGGER, sizeof (DREC_MTAL_CB_T), &rCB));
                    break;
                }
                else
                {
                    eReturn = GETSTREAM_TIMEOUT;
                }
            }

/*
            _MtDRec_Lock();
            u4PicDiff = PICINDEXSIZE(_u4MtDRecPicRp, _u4MtDRecPicWp, MTDREC_TICK_PIC_COUNT);
            if((_arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate > 0) && 
                (_u4MtDRecPicRp >= (_arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate/2)))
            {
                eReturn = GETSTREAM_FRAME_ENOUGH;
            }
            _MtDRec_Unlock();

            if(eReturn == GETSTREAM_FRAME_ENOUGH)
            {
                Printf("%s, frame enough\n", __FUNCTION__);
            }
*/

            if(u4RecSize >= _u4MtDRecDefineBufSize)
            {
                eReturn = GETSTREAM_DATA_ENOUGH;
            }

            if(eReturn != GETSTREAM_NORET)
            {
                UINT8 u1RetIndex;

                u4RecSize = (u4RecSize >= _u4MtDRecDefineBufSize) ? _u4MtDRecDefineBufSize : u4RecSize;
                //u4RecSize = _u4MtDRecDefineBufSize;
                
                // Format index table
                u1RetIndex = _MtDRecFormatIndexArray(u4RecSize);
                ASSERT(u1RetIndex != 0xFF);
                
                // ****************************
                // 1.Transfer ring buffer to linear buffer
                // 2.Fill stream info
                // ****************************
                _MtDRecStreamPostProcessing(u4ReadPointer, u4RecSize);

                // Update new read pointer
                _MtDRec_Lock();
                _arMtDRecStreamData[_u1MtDRecStreamDataWp].fgValid = TRUE;
                _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4Addr = _u4MtDRecReadPointer;
                _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4Size = u4RecSize;
                 
                if(u1RetIndex == 0xFF)
                {
                    _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexAddr = 0;
                    _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexSize = 0;
                }
                else
                {
                    _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexAddr = 
                        _arDDIPVRIndexArray[u1RetIndex].u4IndexAddr;
                    _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexSize = 
                        (_arDDIPVRIndexArray[u1RetIndex].u1Count * sizeof(MTDREC_PVR_INDEX_T));
                }

                rCB.result = TRUE;
                rCB.u4Data0 = _u4MtDRecReadPointer;
                rCB.u4Data1 = u4RecSize;
                rCB.u4Data2 = _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexAddr;
                rCB.u4Data3 = _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexSize;

                // Update new value
                _u4MtDRecReadPointer += u4RecSize;
                if(_u4MtDRecReadPointer >= (rGBL.u4BufStart + rGBL.u4BufSize))
                {
                    _u4MtDRecReadPointer -= rGBL.u4BufSize;
                }
                
                _u1MtDRecStreamDataWp++;
                _u1MtDRecStreamDataWp %= MTDREC_STREAM_STRUCT_MAX;
                _MtDRec_Unlock(); 

                UNUSED(_CB_PutEvent(CB_MTAL_DREC_TRIGGER, sizeof (DREC_MTAL_CB_T), &rCB));
                break;
            }
            
            x_thread_delay(20);
            u4ElapseTime += 20;
        }
    }
}

//-----------------------------------------------------------------------------
/** _MtDRecThumbThread
 */
//-----------------------------------------------------------------------------
static VOID _MtDRecThumbThread(VOID* pvArg)
{
    MTDREC_MSG_T rMsg;
    INT32 i4Ret;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex;
    UINT32 i;

    FVR_PID_T rFvrPid;
    DMX_PID_T rPid;

    UNUSED(pvArg);

    zMsgSize = sizeof (rMsg);
    while (1)
    {
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
            &_hMtDrecThumbQueue, 1, X_MSGQ_OPTION_WAIT);

        // query record video pid
        for(i =0; i < FVR_NUM_PID_INDEX; i++)
        {
            FVR_GetPid(i, FVR_PID_FLAG_ALL, &rFvrPid);
            if((rFvrPid.fgEnable == TRUE) && (rFvrPid.fgPicSearch == TRUE))
            {
                MTDREC_PRINT("found a recorded video pid %d\n", rFvrPid.u2Pid);
                // this is a video PID
                break;
            }
        }

        // there is no video record PID, free the additional thumbnail video pid
        if(i >= FVR_NUM_PID_INDEX)
        {
            MTDREC_PRINT("there is no video record PID, free thumbnail pid\n");
            _MtDRecFreeThumbnailPid(0, TRUE);
        }

        // check if the record video pid is already playing, except ts index 2
        for(i =0; i < DMX_NUM_PID_INDEX; i++)
        {
            DMX_GetPid(i, FVR_PID_FLAG_ALL, &rPid);
            if((rPid.fgEnable == TRUE) && (rPid.u2Pid == rFvrPid.u2Pid) &&
                (rPid.u1TsIndex != 2) && (rPid.ePidType == DMX_PID_TYPE_ES_VIDEO))
            {
                // the video pid is already set
                MTDREC_PRINT("the playing video pid is already set\n");
                if(i != 31)
                {
                    // we already has another playing video pid, free thumbnail one
                    _MtDRecFreeThumbnailPid(0, TRUE);
                }
                break;
            }
        }

        // set the thumbnail video pid
        if(i >= DMX_NUM_PID_INDEX)
        {
            MTDREC_PRINT("set the thumbnail video pid.\n");
            _MtDRecSetThumbnailPid(31, TRUE);
        }
    }
}

#if 0
static VOID _MtDRecThread(VOID* pvArg)
{
    MTDREC_MSG_T rMsg;
    INT32 i4Ret;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex;
    FVR_GBL_T rGBL;
    UINT32 u4RecSize, u4Timeout;
    UINT32 i, u4PicDiff, u4ReadPointer;
    MTDREC_GETSTREAM_RET_T eReturn = GETSTREAM_NORET;
    DREC_MTAL_CB_T rCB;    
    UINT32 u4ElapseTime = 0;

    UNUSED(pvArg);

    zMsgSize = sizeof (rMsg);
    while (1)
    {
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
            &_hMtDrecQueue, 1, X_MSGQ_OPTION_WAIT);

        // If not start, continue
        u4Timeout = rMsg.u4Timeout;
        eReturn = GETSTREAM_NORET;
        u4ElapseTime = 0;
        while(1)
        {
            _MtDRec_Lock();
            if(_fgDRecStreamStop)
            {
                _MtDRec_Unlock();
                x_memset((void*)&rCB, 0, sizeof(rCB));
                rCB.result = TRUE;
                // return not ok
                UNUSED(_CB_PutEvent(CB_MTAL_DREC_TRIGGER, sizeof (DREC_MTAL_CB_T), &rCB));
                break;
            }

            ASSERT(_u1MtDRecStreamDataWp < MTDREC_STREAM_STRUCT_MAX);
            if(_u1MtDRecStreamDataWp >= MTDREC_STREAM_STRUCT_MAX)
            {
                _MtDRec_Unlock();
                break;
            }
            
            ASSERT(!_arMtDRecStreamData[_u1MtDRecStreamDataWp].fgValid);
            if(_arMtDRecStreamData[_u1MtDRecStreamDataWp].fgValid)
            {
                _MtDRec_Unlock();
                MTDREC_PRINT("No available stream array\n");
                x_thread_delay(20);
                continue;
            }

            // Reset stream info
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].eFrameRate = MTDREC_PVR_FR_UNKNOWN;
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].fgFoundSeqSPS = FALSE;
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4BitRate = 0;
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate = 0;
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4SeqSPSOffset = 0;
            _arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4SPSNextStartLen = 0;
            
            u4ReadPointer = _u4MtDRecReadPointer;
            _MtDRec_Unlock();

            x_memset((void*)&rGBL, 0, sizeof(rGBL));
            if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
            {
                ASSERT(0);
                //break;
            }
#ifdef CC_NEW_PVR            
            if(!FVR_UpdateWritePointer(rGBL.u4Wp))
            {
                ASSERT(0);
                //break;
            }
#endif
            u4RecSize = DATASIZE(u4ReadPointer, rGBL.u4Wp, rGBL.u4BufSize);

            if(u4ElapseTime > u4Timeout)
            {
                Printf("%s, timeout\n", __FUNCTION__);
                if(u4RecSize < (24 * 1024))
                {
                    x_memset((void*)&rCB, 0, sizeof(rCB));
                    rCB.result = FALSE;
                    // return not ok
                    UNUSED(_CB_PutEvent(CB_MTAL_DREC_TRIGGER, sizeof (DREC_MTAL_CB_T), &rCB));
                    break;
                }
                else
                {
                    eReturn = GETSTREAM_TIMEOUT;
                }
            }

/*
            _MtDRec_Lock();
            u4PicDiff = PICINDEXSIZE(_u4MtDRecPicRp, _u4MtDRecPicWp, MTDREC_TICK_PIC_COUNT);
            if((_arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate > 0) && 
                (_u4MtDRecPicRp >= (_arMtDRecStreamInfo[_u1MtDRecStreamDataWp].u4FrameRate/2)))
            {
                eReturn = GETSTREAM_FRAME_ENOUGH;
            }
            _MtDRec_Unlock();

            if(eReturn == GETSTREAM_FRAME_ENOUGH)
            {
                Printf("%s, frame enough\n", __FUNCTION__);
            }
*/

            if(u4RecSize >= _u4MtDRecDefineBufSize)
            {
                eReturn = GETSTREAM_DATA_ENOUGH;
            }

            if(eReturn != GETSTREAM_NORET)
            {
                UINT8 u1RetIndex;

                u4RecSize = (u4RecSize >= _u4MtDRecDefineBufSize) ? _u4MtDRecDefineBufSize : u4RecSize;
                //u4RecSize = _u4MtDRecDefineBufSize;
                
                // Format index table
                u1RetIndex = _MtDRecFormatIndexArray(u4RecSize);
                ASSERT(u1RetIndex != 0xFF);
                
                // ****************************
                // 1.Transfer ring buffer to linear buffer
                // 2.Fill stream info
                // ****************************
                _MtDRecStreamPostProcessing(u4ReadPointer, u4RecSize);

                // Update new read pointer
                _MtDRec_Lock();
                _arMtDRecStreamData[_u1MtDRecStreamDataWp].fgValid = TRUE;
                _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4Addr = _u4MtDRecReadPointer;
                _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4Size = u4RecSize;
                 
                if(u1RetIndex == 0xFF)
                {
                    _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexAddr = 0;
                    _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexSize = 0;
                }
                else
                {
                    _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexAddr = 
                        _arDDIPVRIndexArray[u1RetIndex].u4IndexAddr;
                    _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexSize = 
                        (_arDDIPVRIndexArray[u1RetIndex].u1Count * sizeof(MTDREC_PVR_INDEX_T));
                }

                rCB.result = TRUE;
                rCB.u4Data0 = _u4MtDRecReadPointer;
                rCB.u4Data1 = u4RecSize;
                rCB.u4Data2 = _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexAddr;
                rCB.u4Data3 = _arMtDRecStreamData[_u1MtDRecStreamDataWp].u4IndexSize;

                // Update new value
                _u4MtDRecReadPointer += u4RecSize;
                if(_u4MtDRecReadPointer >= (rGBL.u4BufStart + rGBL.u4BufSize))
                {
                    _u4MtDRecReadPointer -= rGBL.u4BufSize;
                }
                
                _u1MtDRecStreamDataWp++;
                _u1MtDRecStreamDataWp %= MTDREC_STREAM_STRUCT_MAX;
                _MtDRec_Unlock(); 

                UNUSED(_CB_PutEvent(CB_MTAL_DREC_TRIGGER, sizeof (DREC_MTAL_CB_T), &rCB));
                break;
            }
            
            x_thread_delay(20);
            u4ElapseTime += 20;
        }
    }
}


#endif
//-----------------------------------------------------------------------------
/** _MtDRecSetBuffer
 */
//-----------------------------------------------------------------------------
static BOOL _MtDRecSetBuffer(void)
{
    UINT32  u4FbmSize, u4FbmAddr,i;
    FVR_GBL_T rGBL;
    FBM_POOL_T* prFbmPool;

    // Allocate video buffer from FBM
    //prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX_REC_TS);
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_PVR);
    //prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG_OSD2);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);

    u4FbmAddr = VIRTUAL(prFbmPool->u4Addr);
    u4FbmSize = prFbmPool->u4Size;
/*
    if(u4FbmSize < (MTDREC_STREAM_SIZE + MTDREC_META_SIZE))
    {
        MTDMX_PRINT("%s, buf not enough, 0x%x, 0x%x\n", __FUNCTION__, __LINE__, u4FbmSize,
                                                 (MTDREC_STREAM_SIZE + MTDREC_META_SIZE));
        return MTR_NOT_OK;
    }
*/
    _MtDRec_Lock();
    _u4DRecBaseAddr = u4FbmAddr;
    _u4DRecBaseSize = u4FbmSize;
    _u4DRecIndexBufAddr = (u4FbmAddr + (PAGE_SIZE - 1)) & PAGE_MASK;
    _u4DRecIndexBufSize = (MTDREC_PIC_INDEX_ARRAY_SIZE * sizeof(MTDREC_PVR_INDEX_T)) * MTDREC_STREAM_STRUCT_MAX;
    for(i=0; i<MTDREC_STREAM_STRUCT_MAX; i++)
    {
        _arMtDRecStreamData[i].u4IndexAddr =
          VIRTUAL((_u4DRecIndexBufAddr + ((MTDREC_PIC_INDEX_ARRAY_SIZE * sizeof(MTDREC_PVR_INDEX_T)) * i)));
    }

    x_memset((void*)&rGBL, 0, sizeof(rGBL));
    rGBL.fgAllocateBuffer = FALSE;
    // Linux kernel start address must page alignment
    rGBL.u4BufStart = _u4DRecIndexBufAddr + _u4DRecIndexBufSize;
    rGBL.u4BufSize = u4FbmSize - (_u4DRecIndexBufSize + MTDREC_GETSTREAM192_ALIGN_SIZE);
    rGBL.u4BufSize -= (rGBL.u4BufSize % MTDREC_GETSTREAM192_ALIGN_SIZE);
    rGBL.fgSetAlign = TRUE;
    rGBL.u4BufAlign = 192;
    _MtDRec_Unlock();

    if(!FVR_SetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return FALSE;
    }

    // Get stream addr and size after align handle
    if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return FALSE;
    }

    _MtDRec_Lock();
    _u4DRecStreamAddr = rGBL.u4BufStart;
    _u4DRecStreamSize = rGBL.u4BufSize;
    _u4DRecStreamPktCount = _u4DRecStreamSize / 192;
    _MtDRec_Unlock();

    Printf("fbm, base:0x%x, size:0x%x, stream addr:0x%x, streamsize:0x%x\n",
        _u4DRecBaseAddr, _u4DRecBaseSize, _u4DRecStreamAddr, _u4DRecStreamSize);
    Printf("index size: %d\n", sizeof(MTDREC_PVR_INDEX_T));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MTDREC_Init
 *  Initialize
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_Init(unsigned long arg)
{
    static BOOL _fgInit = FALSE;
    FVR_GBL_T rGBL;
    UINT32 i;

    if(!_fgInit)
    {
        if (x_thread_create(&_hMtDrecThread, MTDREC_THREAD_NAME,
            MTDREC_THREAD_STACK_SIZE, MTDREC_THREAD_PRIORITY, _MtDRecThread, 0,
            NULL) != OSR_OK)
        {
            return MTR_NOT_OK;
        }

        if (x_msg_q_create(&_hMtDrecQueue, "MTDRECQ",
                        sizeof (MTDREC_MSG_T), 32) != OSR_OK)
        {
            return MTR_NOT_OK;
        }

        if (x_thread_create(&_hMtDrecThumbThread, MTDREC_THUMBNAIL_THREAD_NAME,
            MTDREC_THUMBNAIL_THREAD_STACK_SIZE, MTDREC_THUMBNAIL_THREAD_PRIORITY,
            _MtDRecThumbThread, 0, NULL) != OSR_OK)
        {
            return MTR_NOT_OK;
        }

        if (x_msg_q_create(&_hMtDrecThumbQueue, "MTTHMQ",
                        sizeof (MTDREC_MSG_T), 32) != OSR_OK)
        {
            return MTR_NOT_OK;
        }

        _u4MtDRecH264ESBuf = (UINT32)x_mem_alloc(MTDREC_H264ES_BUFSIZE);
        if(_u4MtDRecH264ESBuf == 0)
        {
            return MTR_NOT_OK;
        }

        _fgInit = TRUE;
    }

    _MtDRec_Lock();
    _eMtDrecVideoType = DREC_TYPE_VIDEO_MPEG2;
    _u4MtDRecPicWp = 0;
    _u4MtDRecPicRp = 0;
    _MtDRec_Unlock();

    for(i=0; i<MTDREC_STREAM_STRUCT_MAX; i++)
    {
        _arMtDRecStreamData[i].fgValid = FALSE;
        _arMtDRecStreamData[i].u4Addr = 0;
        _arMtDRecStreamData[i].u4IndexAddr = 0;
        _arMtDRecStreamData[i].u4IndexSize = 0;
        _arMtDRecStreamData[i].u4Size = 0;
    }

    for(i=0; i<MTDREC_STREAM_STRUCT_MAX; i++)
    {
        _arMtDRecStreamInfo[i].u4BitRate = 0;
        _arMtDRecStreamInfo[i].eFrameRate = MTDREC_PVR_FR_FORBIDDEN;
        _arMtDRecStreamInfo[i].fgFoundSeqSPS = FALSE;
        _arMtDRecStreamInfo[i].u4FrameRate = 0;
        _arMtDRecStreamInfo[i].u4SeqSPSOffset = 0;
        _arMtDRecStreamInfo[i].u4SPSNextStartLen = 0;
    }

    if(!FVR_Init())
    {
        return MTR_NOT_OK;
    }

    x_memset((void*)&rGBL, 0, sizeof(rGBL));
    rGBL.u4TimeToNotify = 0;
    rGBL.pfnNotifyFunc = NULL;
    rGBL.pvNotifyTag = NULL;
    if(!FVR_SetGBL(FVR_GBL_FLAG_TARGET_PKT | FVR_GBL_FLAG_NOTIFY | FVR_GBL_FLAG_TIME_INFO, &rGBL))
    {
        return MTR_NOT_OK;
    }

    if(!_MtDRecSetBuffer())
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_GetBufferPtr
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_GetBufferPtr(unsigned long arg)
{
    INT32 Arg;
    MTDREC_BUFPTR_T *prBuf, rBuf;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    prBuf = (MTDREC_BUFPTR_T*)Arg;

    rBuf.u4Addr = _u4DRecBaseAddr;
    rBuf.u4Size = _u4DRecBaseSize;

    USR_SPACE_ACCESS_VALIDATE_ARG(prBuf, MTDREC_BUFPTR_T);
    COPY_TO_USER_ARG(prBuf, rBuf, MTDREC_BUFPTR_T);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_SetPid
 *  Set record PID information
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_SetPid(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT8 u1Pidx;
    UINT16 u2Pid;
    FVR_PID_T rFvrPid;
    MTDREC_PID_TYPE_T eType;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u2Pid = (UINT16)rArg.ai4Arg[1];
    eType= (MTDREC_PID_TYPE_T)rArg.ai4Arg[2];

    x_memset((void*)&rFvrPid, 0, sizeof(FVR_PID_T));

    rFvrPid.fgEnable = TRUE;
    rFvrPid.u2Pid = u2Pid;
    rFvrPid.u1TsIndex = DMX_GetFramerIndex();
    rFvrPid.eSource = DMX_SOURCE_DTV0;

    rFvrPid.fgPktSwap = FALSE;
    rFvrPid.pfnSwapNotify = NULL;
    rFvrPid.pvSwapNotifyTag = NULL;

    if((eType == DREC_TYPE_VIDEO_MPEG2) || (eType == DREC_TYPE_VIDEO_MPEG4) ||
       (eType == DREC_TYPE_VIDEO_H264))
    {
        _eMtDrecVideoType = eType;
        _u2MtDRecVideoPid = u2Pid;
        _u2MtDRecVideoPidx = u1Pidx;
        rFvrPid.fgPicSearch = TRUE;
        rFvrPid.u1PicSearchMask = 0xFF;
        rFvrPid.pfnPicSearchNotify = _MtTsDRecPicCallback;
        rFvrPid.pvPicSearchNotifyTag = NULL;
    }
    else
    {
        rFvrPid.fgPicSearch = FALSE;
        rFvrPid.u1PicSearchMask = 0;
        rFvrPid.pfnPicSearchNotify = NULL;
        rFvrPid.pvPicSearchNotifyTag = NULL;
    }

    rFvrPid.ePidType = DMX_PID_TYPE_NONE;

    switch(eType)
    {
    case DREC_TYPE_VIDEO_MPEG2:
    case DREC_TYPE_VIDEO_MPEG4:
    case DREC_TYPE_VIDEO_H264:
    case DREC_TYPE_VIDEO_AVS:
    	rFvrPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
    	break;

    case DREC_TYPE_AUDIO:
    	rFvrPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
    	break;

    default:
    	break;
    }


    rFvrPid.fgScramble = _fgMTDRecEncrypt;
    rFvrPid.fgCBCMode = _fgMTDRecEncryptCBC;

    rFvrPid.eKeyType = _rMTDRecKeyType;
    rFvrPid.u1KeyIdx = MTDREC_CIPHER_KEY_INDEX;
    rFvrPid.u4KeyLen = _u4MtDRecKeyLen;

    // Not not set scramble detect function

    if(!FVR_SetPid(u1Pidx, FVR_PID_FLAG_ALL, &rFvrPid))
    {
        return MTR_NOT_OK;
    }

    if(rFvrPid.ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        FVR_VIDEO_TYPE_T eVideoType = FVR_VIDEO_MPEG;

        switch(_eMtDrecVideoType)
        {
        case DREC_TYPE_VIDEO_MPEG2:
            eVideoType = FVR_VIDEO_MPEG;
            break;
        case DREC_TYPE_VIDEO_MPEG4:
            Printf("MPEG4 Video Type??\n");
            break;
        case DREC_TYPE_VIDEO_H264:
            eVideoType = FVR_VIDEO_H264;
            break;
        default:
            Printf("Wrong Video Type\n");
            return MTR_NOT_OK;
        }

        if(!FVR_SetVideoTypeByPidx(u1Pidx, eVideoType))
        {
            return MTR_NOT_OK;
        }

        MTDREC_NotifyThumbnailChecking();
    }

    return MTR_OK;
}


static MT_RESULT_T _MTDREC_GetPid(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT8 u1Pidx;
    UINT32 u4Flag;
    FVR_PID_T rFvrPid;
    MTDREC_PID_T *prPid = NULL;
    MTDREC_PID_T rPid;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u4Flag = (UINT32)rArg.ai4Arg[1];
    prPid = (MTDREC_PID_T *)rArg.ai4Arg[2];

    x_memset((void*)&rFvrPid, 0, sizeof(FVR_PID_T));

    if(!FVR_GetPid(u1Pidx, u4Flag, &rFvrPid))
    {
        return MTR_NOT_OK;
    }

    rPid.fgEnable = rFvrPid.fgEnable;
    rPid.u2Pid = rFvrPid.u2Pid;
    rPid.ePidType = rFvrPid.ePidType;
    rPid.u1TsIndex = rFvrPid.u1TsIndex;

    USR_SPACE_ACCESS_VALIDATE_ARG(prPid, MTDREC_PID_T);
    COPY_TO_USER_ARG(prPid, rPid, MTDREC_PID_T);

    MTDREC_PRINT(" -u1Pidx = %u , u4Flags = 0x%x\n ", u1Pidx, u4Flag);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_FreePid
 *  Free record PID
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_FreePid(unsigned long arg)
{
    UINT8 u1Pidx;
    FVR_PID_T rFvrPid;
    INT32 Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u1Pidx = (UINT8)Arg;

    x_memset((void*)&rFvrPid, 0, sizeof(FVR_PID_T));
    FVR_GetPid(u1Pidx, FVR_PID_FLAG_ALL, &rFvrPid);

    if(!FVR_FreePid(u1Pidx))
    {
        return MTR_NOT_OK;
    }

    if(rFvrPid.ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        MTDREC_NotifyThumbnailChecking();
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_UnlockBuffer
 *  Unlock record buffer to driver
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_UnlockBuffer(unsigned long arg)
{
    INT32 Arg;
    UINT32 i, u4Addr;
    UINT32 u4StreamAddr, u4StreamSize;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u4Addr = (UINT32)Arg;

    //Printf("kernel, unlock: 0x%x\n", u4Addr);

    for(i=0; i<MTDREC_STREAM_STRUCT_MAX; i++)
    {
        _MtDRec_Lock();
        if(!_arMtDRecStreamData[i].fgValid)
        {
            _MtDRec_Unlock();
            continue;
        }
        u4StreamAddr = _arMtDRecStreamData[i].u4Addr;
        u4StreamSize = _arMtDRecStreamData[i].u4Size;
        _MtDRec_Unlock();

        if(u4StreamAddr == u4Addr)
        {
            _MtDRec_Lock();
            if(_fgDRecStreamStop == FALSE)
            {
            if(!FVR_UnlockBuffer(u4Addr, u4StreamSize))
            {
                    Printf("Not unlock buffer:0x%x size: 0x%x\n", u4Addr, u4StreamSize);
                }
            }
            _MtDRec_Unlock();

            _MtDRec_Lock();
            _arMtDRecStreamData[i].fgValid = FALSE;
            _arMtDRecStreamData[i].u4Addr = 0;
            _arMtDRecStreamData[i].u4Size = 0;
            _arMtDRecStreamData[i].u4IndexSize = 0;
            _MtDRec_Unlock();
            break;
        }
    }

    return MTR_OK;
}



//-----------------------------------------------------------------------------
/** _MTDREC_Start
 *  Start recording
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_Start(unsigned long arg)
{
    FVR_TIMESTAMP_T rTimestamp;
    UINT8 i;

    rTimestamp.fgEnable = _fgMTDRecUseTimestamp;
    rTimestamp.u2ClkDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
    rTimestamp.u4Counter = 0;
    rTimestamp.u4Mask = 0xFFFFFFFF;

    FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimestamp);

    _MtDRec_Lock();
    _u4MtDRecReadPointer = _u4DRecStreamAddr;   // Set read pointer to buffer start
    _u4MtDRecPicWp = 0;
    _u4MtDRecPicRp = 0;
    _u1MtDRecStreamDataWp = 0;
    _u1MtDRecStreamDataRp = MTDREC_STREAM_STRUCT_MAX - 1; //0;
    _fgDRecStreamStop = FALSE;
    _MtDRec_Unlock();

    _MtDRec_Lock();
    for(i=0; i<MTDREC_STREAM_STRUCT_MAX; i++)
    {
        _arMtDRecStreamData[i].fgValid = FALSE;
        _arMtDRecStreamData[i].u4Addr = 0;
        _arMtDRecStreamData[i].u4IndexSize = 0;
        _arMtDRecStreamData[i].u4Size = 0;
    }

    for(i=0; i<MTDREC_STREAM_STRUCT_MAX; i++)
    {
        _arMtDRecStreamInfo[i].u4BitRate = 0;
        _arMtDRecStreamInfo[i].eFrameRate = MTDREC_PVR_FR_FORBIDDEN;
        _arMtDRecStreamInfo[i].fgFoundSeqSPS = FALSE;
        _arMtDRecStreamInfo[i].u4FrameRate = 0;
        _arMtDRecStreamInfo[i].u4SeqSPSOffset = 0;
        _arMtDRecStreamInfo[i].u4SPSNextStartLen = 0;
    }
    _MtDRec_Unlock();

    if(!FVR_Start())
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_Stop
 *  Stop record
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_Stop(unsigned long arg)
{
    _MtDRec_Lock();
    _fgDRecStreamStop = TRUE;
    _MtDRec_Unlock();

    if(!FVR_Stop())
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_GetDownloadStreamInfo
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_GetDownloadStreamInfo(unsigned long arg)
{
    INT32 Arg;
    MTDREC_STREAM_INFO_T *prInfo;
    MTDREC_STREAM_INFO_T rInfo;
    UINT8 u1Index;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);
    prInfo = (MTDREC_STREAM_INFO_T*)Arg;

    if(!FVR_IsRecording())
    {
        return MTR_NOT_OK;
    }

    _MtDRec_Lock();
    u1Index = _u1MtDRecStreamDataRp;
    _u1MtDRecStreamDataRp++;
    _u1MtDRecStreamDataRp %= MTDREC_STREAM_STRUCT_MAX;

    ASSERT(u1Index < MTDREC_STREAM_STRUCT_MAX);
    if(u1Index >= MTDREC_STREAM_STRUCT_MAX)
    {
        _MtDRec_Unlock();
        return MTR_NOT_OK;
    }

    rInfo = _arMtDRecStreamInfo[u1Index];
    _MtDRec_Unlock();

    MTDREC_PRINT("Return SPS: 0x%x, Frate: 0x%x, bRate: 0x%x\n", rInfo.fgFoundSeqSPS, rInfo.eFrameRate, rInfo.u4BitRate);

    USR_SPACE_ACCESS_VALIDATE_ARG(prInfo, MTDREC_STREAM_INFO_T);
    COPY_TO_USER_ARG(prInfo, rInfo, MTDREC_STREAM_INFO_T);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_GetDownloadStream
 */
//-----------------------------------------------------------------------------
/*
static MT_RESULT_T _MTDREC_GetDownloadStream(unsigned long arg)
{
    INT32 Arg;
    MTDREC_STREAM_DATA_T rStreamData, *prStreamData;
    UINT32 u4StreamDataSize;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, MTDREC_STREAM_DATA_T);
    COPY_FROM_USER_1ARG(arg, Arg);

    prStreamData = (MTDREC_STREAM_DATA_T*)Arg;

    x_memset((void*)&rStreamData, 0, sizeof(MTDREC_STREAM_DATA_T));
    
    // Check record data array
    _MtDRec_Lock();
    u4StreamDataSize = DATASIZE(_u1MtDRecStreamDataRp, _u1MtDRecStreamDataWp, _u1MtDRecStreamDataWp);
    if(u4StreamDataSize > 0)
    {
        if(_arMtDRecStreamData[_u1MtDRecStreamDataRp].fgValid)
        {
            rStreamData = _arMtDRecStreamData[_u1MtDRecStreamDataRp];
            _u1MtDRecStreamDataRp++;
            _u1MtDRecStreamDataRp %= MTDREC_STREAM_STRUCT_MAX;
        }
    }
    _MtDRec_Unlock();

    USR_SPACE_ACCESS_VALIDATE_ARG(prStreamData, MTDREC_STREAM_DATA_T);
    COPY_TO_USER_ARG(prStreamData, rStreamData, MTDREC_STREAM_DATA_T);

    return MTR_OK;
}
*/
static MT_RESULT_T _MTDREC_GetDownloadStream(unsigned long arg)
{
    INT32 Arg;
    MTDREC_MSG_T rMsg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    if(!FVR_IsRecording())
    {
        return MTR_NOT_OK;
    }

    rMsg.u4Timeout = (UINT32)Arg;
    if (x_msg_q_send(_hMtDrecQueue, (VOID*)&rMsg, sizeof (MTDREC_MSG_T), 128) != OSR_OK)
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_StopStream
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_StopStream(unsigned long arg)
{
    _MtDRec_Lock();
    _fgDRecStreamStop = TRUE;
    _MtDRec_Unlock();

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_SetCipher
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_SetCipher(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    MTDREC_CRYPTO_INFO_T rInfo, *prInfo;
    UINT8 *pu1Key, *pu1IV;
    UINT8 au1Key[32], au1IV[16];
    BOOL fgEven = FALSE;
    UINT8 i;
    FVR_PID_T rFvrPid;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    prInfo = (MTDREC_CRYPTO_INFO_T*)rArg.ai4Arg[0];
    pu1IV = (UINT8*)rArg.ai4Arg[1];
    pu1Key = (UINT8*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG(prInfo, MTDREC_CRYPTO_INFO_T);
    COPY_FROM_USER_ARG(prInfo, rInfo, MTDREC_CRYPTO_INFO_T);

    if(rInfo.u4KeyLen > 256)
    {
        return MTR_NOT_OK;
    }

    // Fix klocwork
    for(i=0; i<16; i++)
    {
        au1IV[i] = 0;
    }

    if(rInfo.eAlgorithm == MTDREC_CIPHER_AES_CBC && pu1IV != NULL)
    {
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu1IV, 16);
    COPY_FROM_USER_ARG_SIZE_K(pu1IV, au1IV, 16);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu1Key, rInfo.u4KeyLen/8);
    COPY_FROM_USER_ARG_SIZE_K(pu1Key, au1Key, rInfo.u4KeyLen/8);

    if(rInfo.eKeyType == MTDREC_CIPHER_KEY_EVEN)
    {
        _rMTDRecKeyType = FVR_KEY_EVEN;
        fgEven = TRUE;
    }
    else
    {
        _rMTDRecKeyType = FVR_KEY_ODD;
        fgEven = FALSE;
    }

    if(!FVR_SetAesKey(MTDREC_CIPHER_KEY_INDEX, fgEven, au1Key))
    {
        return MTR_NOT_OK;
    }

    if(!FVR_SetAesEvenOddIV(MTDREC_CIPHER_KEY_INDEX, fgEven, au1IV))
    {
        return MTR_NOT_OK;
    }

    _u4MtDRecKeyLen = rInfo.u4KeyLen;

    if(rInfo.eAlgorithm == MTDREC_CIPHER_AES_CBC)
    {
        _fgMTDRecEncryptCBC = TRUE;
        _fgMTDRecEncrypt = TRUE;
    }
    else
    {
        _fgMTDRecEncryptCBC = FALSE;
        _fgMTDRecEncrypt = TRUE;
    }

    x_memset((void*)&rFvrPid, 0, sizeof(FVR_PID_T));
    rFvrPid.u4KeyLen = rInfo.u4KeyLen;
    rFvrPid.fgCBCMode = _fgMTDRecEncryptCBC;
    rFvrPid.fgScramble = _fgMTDRecEncrypt;
    rFvrPid.eKeyType = _rMTDRecKeyType;
    rFvrPid.u1KeyIdx = MTDREC_CIPHER_KEY_INDEX;

    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(!FVR_SetPid(i, FVR_PID_FLAG_SCRAMBLE_ENABLE | FVR_PID_FLAG_KEY_LEN |
            FVR_PID_FLAG_CBC_MODE | FVR_PID_FLAG_KEY_INDEX, &rFvrPid))
        {
            return MTR_NOT_OK;
        }
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_SetConfig
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_SetConfig(unsigned long arg)
{
    INT32 Arg;
    MTDREC_CONFIG_T *prConfig, rConfig;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    prConfig = (MTDREC_CONFIG_T*)Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prConfig, MTDREC_CONFIG_T);
    COPY_FROM_USER_ARG(prConfig, rConfig, MTDREC_CONFIG_T);

    _MtDRec_Lock();
    eMtDrecTimestamp = rConfig.eTimestamp;
    if(rConfig.eTimestamp == MTDREC_TIMESTAMP_TYPE_4BYTE)
    {
        _fgMTDRecUseTimestamp = TRUE;
        _u4MtDRecDefineBufSize = MTDREC_GETSTREAM192_LBR_SIZE;
        _Mtpvr_SetPlayBackMode(192);
    }
    else
    {
        // we don't want to use 188 bytes for recording
        /*
        _fgMTDRecUseTimestamp = FALSE;
        _u4MtDRecDefineBufSize = MTDREC_GETSTREAM188_LBR_SIZE;
        */
        _Mtpvr_SetPlayBackMode(188);
    }

    if(rConfig.eAlgorithm == MTDREC_CIPHER_AES_CBC)
    {
        _fgMTDRecEncryptCBC = TRUE;
        _fgMTDRecEncrypt = TRUE;
    }
    else if(rConfig.eAlgorithm == MTDREC_CIPHER_AES_ECB)
    {
        _fgMTDRecEncryptCBC = FALSE;
        _fgMTDRecEncrypt = TRUE;
    }
    else
    {
        _fgMTDRecEncryptCBC = FALSE;
        _fgMTDRecEncrypt = FALSE;
    }
    _MtDRec_Unlock();

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_GetTotalDownloadBufferSize
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_GetTotalDownloadBufferSize(unsigned long arg)
    {
    INT32 Arg;
    UINT32 *pu4Size;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    pu4Size = (UINT32 *)Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Size, UINT32);
    COPY_TO_USER_ARG(pu4Size, _u4DRecStreamSize, UINT32);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDREC_SetDescramblPid
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDREC_SetDescramblPid(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1Pidx;
    MTDMX_DESC_MODE_T eDescMode;
    UINT8 u1KeyIndex;
    FVR_PID_T rPid;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    eDescMode = (MTDMX_DESC_MODE_T)rArg.ai4Arg[1];
    u1KeyIndex = (UINT8)rArg.ai4Arg[2];

    MTDMX_PRINT(" - u1Pidx=%d, eDescMode=%d, u1KeyIndex=%d\n",
                        (int)u1Pidx, (int)eDescMode, (int)u1KeyIndex);

    rPid.fgDesc = TRUE;
    rPid.eDescMode = (DMX_DESC_MODE_T)eDescMode;
    rPid.u1DescKeyidx = u1KeyIndex;
	if(!FVR_SetPid(u1Pidx, FVR_PID_FLAG_DESC, &rPid))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int mtal_ioctl_mtdrec(unsigned int cmd, unsigned long arg)
{
    INT32 i4Ret = MTR_OK;

    switch (cmd)
    {
    case MTAL_IO_DMX_DREC_INIT:
        i4Ret = _MTDREC_Init(arg);
        break;

    case MTAL_IO_DMX_DREC_SETPID:
        i4Ret = _MTDREC_SetPid(arg);
        break;

    case MTAL_IO_DMX_DREC_GETPID:
        i4Ret = _MTDREC_GetPid(arg);
        break;

    case MTAL_IO_DMX_DREC_FREEPID:
        i4Ret = _MTDREC_FreePid(arg);
        break;

    case MTAL_IO_DMX_DREC_UNLOCK_BUF:
        i4Ret = _MTDREC_UnlockBuffer(arg);
        break;

    case MTAL_IO_DMX_DREC_START:
        i4Ret = _MTDREC_Start(arg);
        break;

    case MTAL_IO_DMX_DREC_STOP:
        i4Ret = _MTDREC_Stop(arg);
        break;

    case MTAL_IO_DMX_DREC_GETSTREAMINFO:
        i4Ret = _MTDREC_GetDownloadStreamInfo(arg);
        break;

    case MTAL_IO_DMX_DREC_GETSTREAM:
        i4Ret = _MTDREC_GetDownloadStream(arg);
        break;

    case MTAL_IO_DMX_DREC_STOPSTREAM:
        i4Ret = _MTDREC_StopStream(arg);
        break;

    case MTAL_IO_DMX_DREC_GETBUFFERPTR:
        i4Ret = _MTDREC_GetBufferPtr(arg);
        break;

    case MTAL_IO_DMX_DREC_SETCIPHER:
        i4Ret = _MTDREC_SetCipher(arg);
        break;

    case MTAL_IO_DMX_DREC_SETCONFIG:
        i4Ret = _MTDREC_SetConfig(arg);
        break;

    case MTAL_IO_DMX_DREC_GETTOTALDNBUFFERSIZE:
        i4Ret = _MTDREC_GetTotalDownloadBufferSize(arg);
        break;
        
    case MTAL_IO_DMX_DREC_SETDESCPID:
        i4Ret = _MTDREC_SetDescramblPid(arg);
        break;

    default:
        i4Ret = MTR_NOT_SUPPORTED;
        break;
    }

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** _MTDREC_NotifyThumbnailChecking
 */
//-----------------------------------------------------------------------------
BOOL MTDREC_NotifyThumbnailChecking(void)
{
    MTDREC_MSG_T rMsg;

    rMsg.u4Timeout = (UINT32)0;
    if (x_msg_q_send(_hMtDrecThumbQueue, (VOID*)&rMsg, sizeof (MTDREC_MSG_T), 128) != OSR_OK)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL _MTDREC_IsLivePidEnable(void)
{
    DMX_PID_T rPid;
    UINT8 i;

    // check if the record video pid is already playing, except ts index 2
    for(i =0; i < DMX_NUM_PID_INDEX; i++)
    {
        DMX_GetPid(i, FVR_PID_FLAG_ALL, &rPid);
        if((rPid.fgEnable == TRUE) &&
            (rPid.u1TsIndex == 2) && (rPid.ePidType == DMX_PID_TYPE_ES_VIDEO))
        {
            return FALSE;
        }
    }

    return TRUE;
}

#endif // TIME_SHIFT_SUPPORT

