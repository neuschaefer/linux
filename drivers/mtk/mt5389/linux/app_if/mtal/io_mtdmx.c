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
 * $Revision: #11 $
 * $Author: pan.huang $
 * $SWAuthor: wen han $
 * $MD5HEX:  $
 *---------------------------------------------------------------------------*/

/** @file io_mtdmx.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mtdmx.h"
#include "dmx_if.h"
#include "gcpu_if.h"
#include "dmx_mm_if.h"
#include "fbm_drvif.h"
#include "mpv_drvif.h"
#include "vdec_drvif.h"
#include "aud_drvif.h"
#include "stc_drvif.h"
#include "cb_data.h"
#include "cb_low.h"
#include "x_bim.h"
#include <linux/mtal_ioctl.h>
#ifdef CC_TRUSTZONE_SUPPORT
#include "tz_if.h"
#endif
#include "x_assert.h"
#include "x_util.h"
#include "x_bim.h"

#ifdef CC_MT5392B
#define CC_IOMTDMX_MULTI2
#endif

extern int mtal_ioctl_mttsrec(unsigned int cmd, unsigned long arg);
#ifdef TIME_SHIFT_SUPPORT
extern int mtal_ioctl_mtdrec(unsigned int cmd, unsigned long arg);
extern BOOL MTDREC_NotifyThumbnailChecking(void);
#endif
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MAX_SECTION_SIZE            32768

#define MAX_PES_SIZE                65536
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


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

#define USR_SPACE_ACCESS_VALIDATE_5ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_5ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_5ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_5ARG_T)))          \
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

#ifdef MTDMX_PRINT_SCHAR
#undef MTDMX_PRINT_SCHAR
#endif
#define MTDMX_PRINT_SCHAR(func,str)   do{\
                                            if(IS_MTDMX_LOG_ENABLE)\
                                            {\
                                                UINT8 i=0;\
                                                do\
                                                {\
                                                    printf("%s[%d] = %d, "func,i,str[i]);\
                                                }while(str[i++] != 0);\
                                                printf("\n");\
                                            }\
                                        }while(0)

#ifdef MTDMX_PRINT_CHAR
#undef MTDMX_PRINT_CHAR
#endif
#define MTDMX_PRINT_CHAR(func,str)   do{\
                                            if(IS_MTDMX_LOG_ENABLE)\
                                            {\
                                                UINT8 i=0;\
                                                do\
                                                {\
                                                    printf("%s[%d] = %d, "func,i,str[i]);\
                                                }while(str[i++] != 0);\
                                                printf("");\
                                            }\
                                        }while(0)

#ifdef MTDMX_PRINT_ARRAY
#undef MTDMX_PRINT_ARRAY
#endif
#define MTDMX_PRINT_ARRAY(func,typeinfo,str,length) do{\
                                                            if(IS_MTDMX_LOG_ENABLE)\
                                                            {\
                                                                UINT8 i=0;\
                                                                while(length > i)\
                                                                {\
                                                                    printf("%s%s[%d] = %d\n",func,typeinfo,i,str[i]);\
                                                                    i++;\
                                                                }\
                                                                printf("\n");\
                                                            }\
                                                        }while(0)

#ifdef MTDMX_PRINT_SEC
#undef MTDMX_PRINT_SEC
#endif
#define MTDMX_PRINT_SEC(func ,rsec)  do{\
                                            if(IS_MTDMX_LOG_ENABLE)\
                                            {printf("%s u1TableID = 0x%x,\n",func,(rsec).u1TableID);\
                                                printf("%s u2SecLen = 0x%x,\n",func,(rsec).u2SecLen);\
                                                  printf("%s u1VerNum = 0x%x,\n",func,(rsec).u1VerNum);\
                                                   printf("%s u4CRC = 0x%x\n",func,(rsec).u4CRC);\
                                                }\
                                        }while(0)

#ifdef MTDMX_PID_CNT
#undef MTDMX_PID_CNT
#endif
#define MTDMX_PID_CNT(func,rcnt) do{\
                                        if(IS_MTDMX_LOG_ENABLE)\
                                        {printf("%s u4PesCount = 0x%x,\n",func,(rcnt).u4PesCount);\
                                            printf("%s u4PicCount = 0x%x,\n",func,(rcnt).u4PicCount);\
                                               printf("%s u4SecCount = 0x%x\n",func,(rcnt).u4SecCount);\
                                            }\
                                    }while(0)


#ifdef MTDMX_PRINT_P_PID
#undef  MTDMX_PRINT_P_PID
#endif
#define MTDMX_PRINT_P_PID(str,rpid)    do{\
                                        if(IS_MTDMX_LOG_ENABLE)\
                                        {printf("%s fgEnable = %u,\n",str,(rpid).fgEnable);\
                                            printf("%s fgAllocateBuffer = %u,\n",str,(rpid).fgAllocateBuffer);\
                                              printf("%s u2Pid = 0x%x,\n",str,(rpid).u2Pid);\
                                               printf("%s u4BufAddr = 0x%x,\n",str,(rpid).u4BufAddr);\
                                                 printf("%s u4BufSize = 0x%x,\n",str,(rpid).u4BufSize);\
                                                  printf("%s ePidType = 0x%x,\n",str,(rpid).ePidType);\
                                                   printf("%s pvNotifyTag = 0x%x,\n",str,(UINT32)((rpid).pvNotifyTag));\
                                                    printf("%s pfnScramble = 0x%x,\n",str,(UINT32)(rpid).pfnScramble);\
                                                     printf("%s pvScrambleTag = 0x%x\n",str,(UINT32)((rpid).pvScrambleTag));\
                                            }\
                                        }while(0)

 #ifdef MTDMX_PRINT_FTR
 #undef MTDMX_PRINT_FTR
 #endif
 #define MTDMX_PRINT_FTR(func,rftr)    do{\
                                            if(IS_MTDMX_LOG_ENABLE)\
                                            {printf("%s fgEnable = %d,\n",func,(rftr).fgEnable);\
                                                printf("%s fgCheckCrc = %d,\n",func,(rftr).fgCheckCrc);\
                                                  printf("%s u1Pidx = 0x%x,\n",func,(rftr).u1Pidx);\
                                                   printf("%s u1Offset = 0x%x,\n",func,(rftr).u1Offset);\
                                                     MTDMX_PRINT_ARRAY(func,"au1Data", (rftr).au1Data, 16);\
                                                      MTDMX_PRINT_ARRAY(func,"au1Mask", (rftr).au1Mask, 16);\
                                                }\
                                        }while(0)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// Temporary Section buffer
static UINT8 _au1SecBuf[MAX_SECTION_SIZE];

/// Temporary PES buffer
static UINT8 _au1PesBuf[MAX_PES_SIZE];

static BOOL _afgPidCreated[MTDMX_NUM_PID_INDEX];
static BOOL _afgSectionPid[MTDMX_NUM_PID_INDEX];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _MtDmxCallback(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvNotifyTag)
{
    DMX_NOTIFY_INFO_PSI_T* prPSIInfo;
    DMX_NOTIFY_INFO_PES_T* prPESInfo;
    DMX_NOTIFY_INFO_RAW_TS_T* prRawTsInfo;
    DMX_MTAL_CB_T rCB;

    // Check if PID is really active
    ASSERT(u1Pidx <= DMX_NUM_PID_INDEX);

    rCB.type = eCode;
    rCB.u1Pidx = u1Pidx;

    // Check notification code
    switch (eCode)
    {
    case DMX_NOTIFY_CODE_OVERFLOW:
        rCB.u4Data0 = u4Data;
        break;

    case DMX_NOTIFY_CODE_SCRAMBLE_STATE:
        rCB.u4Data0 = u4Data;
        break;

    case DMX_NOTIFY_CODE_PSI:
        prPSIInfo = (DMX_NOTIFY_INFO_PSI_T*)u4Data;
        ASSERT(prPSIInfo != NULL);

        rCB.u4Data0 = prPSIInfo->u1SerialNumber;
        rCB.u4Data1 = prPSIInfo->u4SecAddr;
        rCB.u4Data2 = prPSIInfo->u4SecLen;
        rCB.u4Data3 = prPSIInfo->u4MatchBitmap;
        break;

    case DMX_NOTIFY_CODE_PES:
        prPESInfo = (DMX_NOTIFY_INFO_PES_T*)u4Data;
        ASSERT(prPESInfo != NULL);

        rCB.u4Data0 = prPESInfo->u1SerialNumber;
        rCB.u4Data1 = prPESInfo->u4DataAddr;
        rCB.u4Data2 = prPESInfo->u4DataSize;
        break;

    case DMX_NOTIFY_CODE_RAW_TS:
        prRawTsInfo = (DMX_NOTIFY_INFO_RAW_TS_T*)u4Data;
        ASSERT(prRawTsInfo != NULL);

        rCB.u4Data0 = prRawTsInfo->u1SerialNumber;
        rCB.u4Data1 = prRawTsInfo->u4DataAddr;
        rCB.u4Data2 = prRawTsInfo->u4DataSize;
        break;

    default:
        // Unexpected notification
        ASSERT(0);
        break;
    }

    UNUSED(_CB_PutEvent(CB_MTAL_DMX_TRIGGER, sizeof (DMX_MTAL_CB_T), &rCB));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwMPEGVideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwMPEGVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rMpvPes;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(VDEC_PES_INFO_T));

    switch (prPes->u4FrameType)
    {
    case PIC_TYPE_I:
        rMpvPes.ucPicType = MPV_I_TYPE;
        break;

    case PIC_TYPE_P:
        rMpvPes.ucPicType = MPV_P_TYPE;
        break;

    case PIC_TYPE_B:
        rMpvPes.ucPicType = MPV_B_TYPE;
        break;

    default:
        return TRUE;
    }

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;
    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;
    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    rMpvPes.fgGop = prPes->fgGop;
    rMpvPes.fgEos = prPes->fgEOS;

    return (VDEC_SendEs(&rMpvPes) == 1);
}


//-----------------------------------------------------------------------------
/** _MwH264VideoCallback
 *  Callback handler of video PES
 */
//-----------------------------------------------------------------------------
static BOOL _MwH264VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T  rMpvPes;
    UINT32 u4Addr;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(VDEC_PES_INFO_T));

    if(!H264_PIC_VALID_TYPE(prPes->u4FrameType))
    {
        return TRUE;
    }

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;

    u4Addr = prPes->u4FrameAddr + 4;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.u4VldReadPtr = u4Addr;

    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    //rMpvPes.fgGop = prPes->fgGop;

    rMpvPes.fgEos = prPes->fgEOS;
    rMpvPes.u1SliceCount = prPes->u1SliceCount;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}


//-----------------------------------------------------------------------------
/** _AVSVideoCallback
 *  Callback handler of video PES
 */
//-----------------------------------------------------------------------------
static BOOL _AVSVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T  rMpvPes;
    UINT32 u4Addr;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;

    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;

    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    //rMpvPes.fgGop = prPes->fgGop;

    rMpvPes.fgEos = prPes->fgEOS;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}


//-----------------------------------------------------------------------------
/** _MwDmxVideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxVideoCallback(const DMX_PES_MSG_T* prPes)
{
#ifdef CC_MT5392B
    switch(DMX_GetVideoType())
#else
    switch(prPes->eVideoType)
#endif
    {
    case DMX_VIDEO_MPEG:
        return _MwMPEGVideoCallback(prPes);

    case DMX_VIDEO_H264:
        return _MwH264VideoCallback(prPes);

    case DMX_VIDEO_AVS:
        return _AVSVideoCallback(prPes);

    default:
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MtdmxSetPid
 */
//-----------------------------------------------------------------------------
static BOOL _MtdmxSetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid)
{
    DMX_PID_T rPid;

	x_memset((void *)&rPid, 0, sizeof(DMX_PID_T));

    rPid = *prPid;

    if(u4Flags & DMX_PID_FLAG_TS_INDEX)
    {
        rPid.u1TsIndex = DMX_GetFramerIndex();

    }

    return DMX_SetPid(u1Pidx, u4Flags, &rPid);
}


//-----------------------------------------------------------------------------
/** _MtdmxSetDecoder
 */
//-----------------------------------------------------------------------------
static void _MtdmxSetDecoder(void)
{
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;

    rDecoderCallbacks.pfnSendVideoPes = _MwDmxVideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
    rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;
    DMX_SetDecoderCallbacks(&rDecoderCallbacks);

    DMX_SetToDecoder(TRUE);
}

//-----------------------------------------------------------------------------
/*
 *  Init demux driver
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_Init(unsigned long arg)
{
	UINT32 i;
	
	  MTDMX_PRINT(" - void\n");
	  x_os_drv_set_timestamp("_MTDMX_Init start");
	
	  if(!DMX_Init())
	  {
		  return MTR_NOT_OK;
	  }
	
	  for(i=0; i<MTDMX_NUM_PID_INDEX; i++)
	  {
		  _afgPidCreated[i] = FALSE;
		  _afgSectionPid[i] = FALSE;
	  }
	
	  _MtdmxSetDecoder();
	  x_os_drv_set_timestamp("_MTDMX_Init end");
	
	  return MTR_OK;

}


//-----------------------------------------------------------------------------
/*
 *  Reset demux driver
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_Reset(unsigned long arg)
{
    MTDMX_PRINT(" - void\n");

    if(!DMX_Reset())
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Start demux
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_Start (unsigned long arg)
{
    MTDMX_PRINT(" - void\n");

    if(!DMX_Start())
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Stop demux
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_Stop(unsigned long arg)
{
    MTDMX_PRINT(" - void\n");

    if(!DMX_Stop())
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Set Front end type
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetFrontEnd(unsigned long arg)
{
    DMX_FRONTEND_T eFrontEnd;
    INT32 Arg;
	x_os_drv_set_timestamp("_MTDMX_SetFrontEnd start");

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    MTDMX_PRINT(" - eFE: %lu\n", Arg);

    eFrontEnd = (DMX_FRONTEND_T)Arg;

    if(!DMX_SetFrontEnd(eFrontEnd))
    {
        return MTR_NOT_OK;
    }
	x_os_drv_set_timestamp("_MTDMX_SetFrontEnd end");

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/*
 *  Set Front end type
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetFrontEndEx(unsigned long arg)
{
    DMX_FRONTEND_T eFrontEnd;
    INT32 Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    MTDMX_PRINT(" - eFE: %lu\n", Arg);

    eFrontEnd = (DMX_FRONTEND_T)Arg;

    if(!DMX_SetFrontEnd(eFrontEnd))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/*
 *  Set Video type
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetVideoType(unsigned long arg)
{
    DMX_VIDEO_TYPE_T eType;
    INT32 Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    MTDMX_PRINT(" - eType: %lu\n", Arg);

    eType = (DMX_VIDEO_TYPE_T)Arg;

    if(!DMX_SetVideoType(eType))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/*
 *  Set a PID
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetPid(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1Pidx ;
    UINT32 u4Flags ;
    MTDMX_PID_T* prPid =NULL;
    MTDMX_PID_T rPid;
    DMX_PID_T rDrvPid;
    FBM_POOL_T* prFbmPool;
    UINT32 au4BufStart[3], au4BufEnd[3];

	x_memset((void *)&rPid,0,sizeof(MTDMX_PID_T));
    x_memset((void *)&rDrvPid,0,sizeof(DMX_PID_T));

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u4Flags = (UINT32)rArg.ai4Arg[1];
    prPid = (MTDMX_PID_T*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG(prPid, MTDMX_PID_T);
    COPY_FROM_USER_ARG(prPid, rPid, MTDMX_PID_T);

    MTDMX_PRINT(" -u1Pidx = %u, u4Flags = 0x%x\n ", u1Pidx, u4Flags);
    MTDMX_PRINT_P_PID("rPid.", rPid);

#if 1
    // Only enable/disable PID
    if(u4Flags == MTDMX_PID_FLAG_VALID)
    {
        rDrvPid.fgEnable = rPid.fgEnable;
        if(!_MtdmxSetPid(u1Pidx, u4Flags, &rDrvPid))
        {
            return MTR_NOT_OK;
        }

        return MTR_OK;
    }
#endif
    // Get from MW
    rDrvPid.fgEnable = rPid.fgEnable;
    rDrvPid.fgAllocateBuffer = rPid.fgAllocateBuffer;
    rDrvPid.u2Pid = rPid.u2Pid;
    rDrvPid.u4BufAddr = rPid.u4BufAddr;
    rDrvPid.u4BufSize = rPid.u4BufSize;
    rDrvPid.u1DeviceId = 0;
    rDrvPid.ePidType = rPid.ePidType;

    if(rPid.ePidType == MTDMX_PID_TYPE_ES_VIDEO)
    {
        // Allocate video buffer from FBM
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0);

        // Setup buffer attributes
        rDrvPid.fgAllocateBuffer = FALSE;
		#ifdef CC_DMX_A1
        if(rPid.u1ChannelId == 0)
        {
            rDrvPid.u4BufAddr = prFbmPool->u4Addr;
            rDrvPid.u4BufSize = prFbmPool->u4Size / 2;
        }
        else
        {
            rDrvPid.u4BufAddr = prFbmPool->u4Addr + (prFbmPool->u4Size / 2);
            rDrvPid.u4BufSize = prFbmPool->u4Size / 2;
        }
		#else
        rDrvPid.u4BufAddr = prFbmPool->u4Addr;
        rDrvPid.u4BufSize = prFbmPool->u4Size;
        #endif
		rDrvPid.u1DeviceId = rPid.u1DeviceId;
    }
    else if(rPid.ePidType == MTDMX_PID_TYPE_ES_AUDIO)
    {
#ifdef AUD_OLDFIFO_INTF
	if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
		&au4BufEnd[1]) != AUD_OK)
#else
	if ((AUD_GetAudFifo(0, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK) ||
		(AUD_GetAudFifo(1, &au4BufStart[1], &au4BufEnd[1]) != AUD_OK)
#ifndef CC_MT5392B
        || (AUD_GetAudFifo(2, &au4BufStart[2], &au4BufEnd[2]) != AUD_OK)
#endif
		)
#endif
        {
            return MTR_NOT_OK;
        }
        if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0)
#ifndef CC_MT5392B
            ||  (au4BufStart[2] == 0)
#endif
            )
        {
            return MTR_NOT_OK;
        }

        rDrvPid.fgAllocateBuffer = FALSE;
		#ifdef CC_DMX_A1
        if(0)  //   !!!remove country select for EU and ATSC with the same audio decoder number
        #else
		if(GetTargetCountry() == COUNTRY_US)
		#endif
        {
        	rPid.u1DeviceId = 0;			// force use dev 0 now
        	rDrvPid.u4BufAddr = au4BufStart[rPid.u1DeviceId];
        	rDrvPid.u4BufSize = au4BufEnd[rPid.u1DeviceId] - au4BufStart[rPid.u1DeviceId];
        }
        else
        {
            if(rPid.u1DeviceId > 2)
            {
                return MTR_NOT_OK;
            }
            rDrvPid.u4BufAddr = au4BufStart[rPid.u1DeviceId];
            rDrvPid.u4BufSize = au4BufEnd[rPid.u1DeviceId] -
            au4BufStart[rPid.u1DeviceId];
        }

        rDrvPid.u1DeviceId = rPid.u1DeviceId;
    }

    if((rPid.pfnNotify != NULL) && (u4Flags & MTDMX_PID_FLAG_CALLBACK))
    {
        rDrvPid.pfnNotify = _MtDmxCallback;
    }
    else
    {
        rDrvPid.pfnNotify = NULL;
    }

    if((rPid.pfnScramble != NULL) && (u4Flags & DMX_PID_FLAG_SCRAMBLE_STATE))
    {
        rDrvPid.pfnScramble = _MtDmxCallback;
    }
    else
    {
        rDrvPid.pfnScramble = NULL;
    }

    rDrvPid.pvNotifyTag = NULL;
    rDrvPid.pvScrambleTag = NULL;

    // Set by driver only
    rDrvPid.fgPrimary = TRUE;
    rDrvPid.u1TsIndex = 0;
    rDrvPid.u1KeyIndex = 0x0;
    rDrvPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rDrvPid.ePcrMode = DMX_PCR_MODE_NONE;
    rDrvPid.eDescMode = DMX_DESC_MODE_NONE;
    #ifndef CC_MT5392B
    rDrvPid.u1ChannelId = 0x0;
    #endif

    u4Flags |= (DMX_PID_FLAG_PRIMARY | DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_KEY_INDEX |
                DMX_PID_FLAG_DEVICE_ID | DMX_PID_FLAG_STEER | DMX_PID_FLAG_PCR |
                DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_SCRAMBLE_STATE);


    if(!_MtdmxSetPid(u1Pidx, u4Flags, &rDrvPid))
    {
        return MTR_NOT_OK;
    }


    return MTR_OK;
}


//-----------------------------------------------------------------------------
/*
 *  Get a PID
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_GetPid (unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1Pidx;
    UINT32 u4Flags;
    MTDMX_PID_T * prPid = NULL;
    MTDMX_PID_T rPid;
    DMX_PID_T rDrvPid;

    x_memset(&rDrvPid, 0, sizeof(rDrvPid));

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u4Flags = (UINT32)rArg.ai4Arg[1];
    prPid = (MTDMX_PID_T*)rArg.ai4Arg[2];

    if(!DMX_GetPid(u1Pidx, u4Flags, &rDrvPid))
    {
        return MTR_NOT_OK;
    }

    rPid.ePidType = rDrvPid.ePidType;
    rPid.fgAllocateBuffer = rDrvPid.fgAllocateBuffer;
    rPid.fgEnable = rDrvPid.fgEnable;
    rPid.pfnNotify = (PFN_MTDMX_NOTIFY)rDrvPid.pfnNotify;
    rPid.pfnScramble = (PFN_MTDMX_NOTIFY)rDrvPid.pfnScramble;
    rPid.pvNotifyTag = rDrvPid.pvNotifyTag;
    rPid.pvScrambleTag = rDrvPid.pvScrambleTag;
    rPid.u2Pid = rDrvPid.u2Pid;
    rPid.u4BufAddr = rDrvPid.u4BufAddr;
    rPid.u4BufSize = rDrvPid.u4BufSize;
    rPid.u1TsIndex = rDrvPid.u1TsIndex;

    USR_SPACE_ACCESS_VALIDATE_ARG(prPid, MTDMX_PID_T);
    COPY_TO_USER_ARG(prPid, rPid, MTDMX_PID_T);

    MTDMX_PRINT(" -u1Pidx = %u , u4Flags = 0x%x\n ", u1Pidx, u4Flags);
    MTDMX_PRINT_P_PID("rPid.", rPid);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Free a PID
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_FreePid(unsigned long arg)
{
    DMX_PID_T rDrvPid;
    UINT8 u1Pidx;
    INT32 Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u1Pidx = (UINT8)Arg;

    MTDMX_PRINT(" - u1Pidx = %d \n",u1Pidx);

    if(u1Pidx >= MTDMX_NUM_PID_INDEX)
    {
        return MTR_NOT_OK;
    }

    if(!DMX_GetPid(u1Pidx, DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_DEVICE_ID, &rDrvPid))
    {
        return MTR_NOT_OK;
    }

    if (rDrvPid.ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        AUD_WaitDspFlush(rDrvPid.u1DeviceId);
    }

    _afgPidCreated[u1Pidx] = FALSE;
    _afgSectionPid[u1Pidx] = FALSE;

    if(!DMX_FreePid(u1Pidx))
    {
        return MTR_NOT_OK;
    }

	#ifdef CC_DMX_A1
    if(rDrvPid.ePidType == (DMX_PID_TYPE_T)MTDMX_PID_TYPE_ES_VIDEO)
    {
    	#ifdef TIME_SHIFT_SUPPORT
        MTDREC_NotifyThumbnailChecking();
        #endif
    }
	#endif

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Find PID index for a given PID value
 *
 *  @param  u2Pid           PID
 *  @param  pu1PidIndex [out]
 *                          The PID index if found
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_GetPidIndex(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT16 u2Pid;
    UINT8 *pu1PidIndex = NULL;
    UINT8 u1PidIndex;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u2Pid = (UINT16)rArg.ai4Arg[0];
    pu1PidIndex = (UINT8*)rArg.ai4Arg[1];

    if(!DMX_GetPidIndex(u2Pid, &u1PidIndex))
    {
        return MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1PidIndex, UINT8);
    COPY_TO_USER_ARG(pu1PidIndex, u1PidIndex, UINT8);

    MTDMX_PRINT(" -u2Pid = 0x%x, u1PidIndex = %d\n",u2Pid,u1PidIndex);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Set a Video Pid
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u2PidNum        Video PID number
 *  @param  fgEnable        Enable video pid or not
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetVideoPid(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1Pidx ;
    UINT16 u2PidNum;
    BOOL fgEnable;
    DMX_PID_T rPid;
    FBM_POOL_T* prFbmPool;

    x_memset((void *)&rPid,0,sizeof(DMX_PID_T));

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u2PidNum = (UINT16)rArg.ai4Arg[1];
    fgEnable = (BOOL)rArg.ai4Arg[2];

    MTDMX_PRINT(" -u1Pidx =%d, u2PidNum = 0x%x, fgEnable  =%d\n ",u1Pidx,u2PidNum,fgEnable);

    _MtdmxSetDecoder();

    if(u1Pidx >= MTDMX_NUM_PID_INDEX)
    {
        return MTR_NOT_OK;
    }

    if(_afgPidCreated[u1Pidx])
    {
        rPid.fgEnable = fgEnable;
        if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            return MTR_NOT_OK;
        }

        return MTR_OK;
    }

    // Allocate video buffer from FBM
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);

    // Setup buffer attributes
    rPid.fgAllocateBuffer = FALSE;
    rPid.u4BufAddr = prFbmPool->u4Addr;
    rPid.u4BufSize = prFbmPool->u4Size;
    rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rPid.u1TsIndex = 0x0;
    rPid.pfnNotify = NULL;
    rPid.fgPrimary = TRUE;
    rPid.u2Pid = u2PidNum;
    rPid.fgEnable = fgEnable;
    #ifndef CC_MT5392B
    rPid.u1ChannelId = 0x0;
    #endif
    rPid.pfnScramble = NULL;
    rPid.pvScrambleTag = NULL;

    // Set by driver only
    rPid.fgPrimary = TRUE;
    rPid.u1TsIndex = 0;
    rPid.u1KeyIndex = 0x0;
    rPid.u1DeviceId = 0;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.eDescMode = DMX_DESC_MODE_NONE;

    if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_ALL, &rPid))
    {
        return MTR_NOT_OK;
    }

    _afgPidCreated[u1Pidx] = TRUE;

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Set a Audio Pid
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u2PidNum        Video PID number
 *  @param  fgEnable        Enable audio pid or not
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetAudioPid (unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1Pidx ;
    UINT16 u2PidNum;
    BOOL fgEnable;
    DMX_PID_T rPid;
    UINT32 au4BufStart[3], au4BufEnd[3];

    x_memset((void *)&rPid,0,sizeof(DMX_PID_T));

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u2PidNum = (UINT16)rArg.ai4Arg[1];
    fgEnable = (BOOL)rArg.ai4Arg[2];

    MTDMX_PRINT(" -u1Pidx =%d, u2PidNum = 0x%x, fgEnable  =%d\n ",u1Pidx,u2PidNum,fgEnable);

    _MtdmxSetDecoder();

    if(u1Pidx >= MTDMX_NUM_PID_INDEX)
    {
        return MTR_NOT_OK;
    }

    if(_afgPidCreated[u1Pidx])
    {
        rPid.fgEnable = fgEnable;
        if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            return MTR_NOT_OK;
        }

        return MTR_OK;
    }

#ifdef AUD_OLDFIFO_INTF
	if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
		&au4BufEnd[1]) != AUD_OK)
#else
	if ((AUD_GetAudFifo(0, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK) ||
		(AUD_GetAudFifo(1, &au4BufStart[1], &au4BufEnd[1]) != AUD_OK)
#ifndef CC_MT5392B
        || (AUD_GetAudFifo(2, &au4BufStart[2], &au4BufEnd[2]) != AUD_OK)
#endif
		)
#endif
    {
        return MTR_NOT_OK;
    }

    if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0)
#ifndef CC_MT5392B
            ||  (au4BufStart[2] == 0)
#endif
        )
    {
        return MTR_NOT_OK;
    }

    if(GetTargetCountry() == COUNTRY_US)
    {
    rPid.u1DeviceId = 0;        // Audio Device ID
    }
    else
    {
        rPid.u1DeviceId = 1;        // Audio Device ID
    }

    rPid.u4BufAddr = au4BufStart[rPid.u1DeviceId];
    rPid.u4BufSize = au4BufEnd[rPid.u1DeviceId] -
                     au4BufStart[rPid.u1DeviceId];
    rPid.fgAllocateBuffer = FALSE;

    rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
    rPid.u1TsIndex = 0x0;
    rPid.pfnNotify = NULL;
    rPid.fgPrimary = TRUE;
    rPid.u2Pid = u2PidNum;
    rPid.fgEnable = fgEnable;

    rPid.pfnScramble = NULL;
    rPid.pvScrambleTag = NULL;

    // Set by driver only
    rPid.fgPrimary = TRUE;
    rPid.u1TsIndex = 0;
    rPid.u1KeyIndex = 0x0;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.eDescMode = DMX_DESC_MODE_NONE;

    if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_ALL, &rPid))
    {
        return MTR_NOT_OK;
    }

    _afgPidCreated[u1Pidx] = TRUE;

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/*
 *  Set a Audio description Pid
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u2PidNum        Video PID number
 *  @param  fgEnable        Enable audio pid or not
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetAudioDescPid (unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1Pidx ;
    UINT16 u2PidNum;
    BOOL fgEnable;
    DMX_PID_T rPid;
    UINT32 au4BufStart[3], au4BufEnd[3];

    x_memset((void *)&rPid,0,sizeof(DMX_PID_T));

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u2PidNum = (UINT16)rArg.ai4Arg[1];
    fgEnable = (BOOL)rArg.ai4Arg[2];

    MTDMX_PRINT(" -u1Pidx =%d, u2PidNum = 0x%x, fgEnable  =%d\n ",u1Pidx,u2PidNum,fgEnable);

    _MtdmxSetDecoder();

    if(u1Pidx >= MTDMX_NUM_PID_INDEX)
    {
        return MTR_NOT_OK;
    }

    if(_afgPidCreated[u1Pidx])
    {
        rPid.fgEnable = fgEnable;
        if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            return MTR_NOT_OK;
        }

        return MTR_OK;
    }

#ifdef AUD_OLDFIFO_INTF
	if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
		&au4BufEnd[1]) != AUD_OK)
#else
	if ((AUD_GetAudFifo(0, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK) ||
		(AUD_GetAudFifo(1, &au4BufStart[1], &au4BufEnd[1]) != AUD_OK)
#ifndef CC_MT5392B
        || (AUD_GetAudFifo(2, &au4BufStart[2], &au4BufEnd[2]) != AUD_OK)
#endif
		)
#endif
    {
        return MTR_NOT_OK;
    }
        if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0)
#ifndef CC_MT5392B
            ||  (au4BufStart[2] == 0)
#endif
            )
    {
        return MTR_NOT_OK;
    }

    rPid.u1DeviceId = 2;        // Audio Description Device ID
#ifndef CC_MT5392B
    rPid.u4BufAddr = au4BufStart[rPid.u1DeviceId];
    rPid.u4BufSize = au4BufEnd[rPid.u1DeviceId] -
               au4BufStart[rPid.u1DeviceId];
#else
    rPid.u4BufAddr = au4BufStart[(((rPid.u1DeviceId)==1)?1:0)];
    rPid.u4BufSize = au4BufEnd[(((rPid.u1DeviceId)==1)?1:0)] -
               au4BufStart[(((rPid.u1DeviceId)==1)?1:0)];
#endif
    rPid.fgAllocateBuffer = FALSE;

    rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
    rPid.u1TsIndex = 0x0;
    rPid.pfnNotify = NULL;
    rPid.fgPrimary = TRUE;
    rPid.u2Pid = u2PidNum;
    rPid.fgEnable = fgEnable;

    // Set by driver only
    rPid.fgPrimary = TRUE;
    rPid.u1TsIndex = 0;
    rPid.u1KeyIndex = 0x0;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.eDescMode = DMX_DESC_MODE_NONE;

    if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_ALL, &rPid))
    {
        return MTR_NOT_OK;
    }

    _afgPidCreated[u1Pidx] = TRUE;

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/*
 *  Set a PCR Pid to one PID index
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u2PcrPid        PCR PID number
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetPcrPid  (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 u1Pidx ;
    UINT16 u2PcrPid;
    DMX_PID_T rDrvPid;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u2PcrPid = (UINT16)rArg.ai4Arg[1];

    MTDMX_PRINT(" - u1Pidx = %d, u2PcrPid = 0x%x\n",u1Pidx,u2PcrPid);

    x_memset((VOID*)&rDrvPid, 0, sizeof(DMX_PID_T));
    rDrvPid.fgEnable = TRUE;
    rDrvPid.fgAllocateBuffer = FALSE;
    rDrvPid.u2Pid = u2PcrPid;
    //rDrvPid.u4BufAddr = 0x0;
    //rDrvPid.u4BufSize = 0x0;
    rDrvPid.ePidType = DMX_PID_TYPE_NONE;
    rDrvPid.fgPrimary = FALSE;
    //rDrvPid.u1TsIndex = 0;
    //rDrvPid.u1KeyIndex = 0;
    //rDrvPid.u1DeviceId = 0;
    //rDrvPid.u1SteerMode = 0;
    rDrvPid.ePcrMode = DMX_PCR_MODE_NEW;
    rDrvPid.eDescMode = DMX_DESC_MODE_NONE;

    if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_ALL & ~DMX_PID_FLAG_BUFFER, &rDrvPid))
    {
        return MTR_NOT_OK;
    }

    if(DMX_SetStcSrc(u1Pidx, 0) != 0)    // Force use STC 0
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Set a PCR Pid to one PID index
 *
 *  @param  u1TsIdx         TS index (0 - 3)
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u1PidIndex      Stc set (0 - 1)
 *  @param  fgEnable        Enable PCR of the PID or not
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetPcrPidEx  (unsigned long arg)
{
    MTAL_IOCTL_4ARG_T    rArg;
    UINT8 u1TsIdx;
    UINT8 u1Pidx;
    UINT8 u1StcSet;
    UINT16 u2PcrPid;
    DMX_PID_T rDrvPid;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    u1TsIdx = (UINT8)rArg.ai4Arg[0];
    u1Pidx = (UINT8)rArg.ai4Arg[1];
    u1StcSet = (UINT8)rArg.ai4Arg[2];
    u2PcrPid = (UINT16)rArg.ai4Arg[3];

    MTDMX_PRINT(" - u1Pidx = %d, u2PcrPid = 0x%x\n",u1Pidx,u2PcrPid);

    if((u1TsIdx >= DMX_FRAMER_COUNT) || (u1Pidx >= DMX_NUM_PID_INDEX) ||
        u1StcSet >= DMX_NUM_STC_NUM)
    {
        MTDMX_PRINT(" wrong parameter - u1TsIdx = %d, u1Pidx = %d, u1StcSet = 0x%x\n",
            u1Pidx, u2PcrPid, u1StcSet);
        return MTR_NOT_OK;
    }

    x_memset((VOID*)&rDrvPid, 0, sizeof(DMX_PID_T));
    rDrvPid.fgEnable = TRUE;
    rDrvPid.fgAllocateBuffer = FALSE;
    rDrvPid.u2Pid = u2PcrPid;
    //rDrvPid.u4BufAddr = 0x0;
    //rDrvPid.u4BufSize = 0x0;
    rDrvPid.ePidType = DMX_PID_TYPE_NONE;
    rDrvPid.fgPrimary = FALSE;
    rDrvPid.u1TsIndex = u1TsIdx;
    //rDrvPid.u1KeyIndex = 0;
    //rDrvPid.u1DeviceId = 0;
    //rDrvPid.u1SteerMode = 0;
    rDrvPid.ePcrMode = DMX_PCR_MODE_NEW;
    rDrvPid.eDescMode = DMX_DESC_MODE_NONE;
    rDrvPid.u1DeviceId = u1StcSet;

    if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_ALL & ~DMX_PID_FLAG_BUFFER, &rDrvPid))
    {
        return MTR_NOT_OK;
    }

    if(DMX_SetStcSrc(u1Pidx, u1StcSet) != 0)
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/*
 *  Set a PCR
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  fgEnable        Enable PCR of the PID or not
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetPcr  (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 u1Pidx ;
    BOOL fgEnable;
    DMX_PID_T rDrvPid;

    x_memset((void *)&rDrvPid,0,sizeof(DMX_PID_T));

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTDMX_PRINT(" - u1Pidx = %d, fgEnable = %d\n",u1Pidx,fgEnable);

    rDrvPid.fgEnable = fgEnable;

    if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_VALID, &rDrvPid))
    {
        return MTR_NOT_OK;
    }

    if(fgEnable)
    {
        if(DMX_SetStcSrc(u1Pidx, 0) != 0)    // Force use STC 0
        {
            return MTR_NOT_OK;
        }
    }
    else
    {
        if(DMX_SetStcSrc(u1Pidx, STC_SRC_NS) != 0)    // Force use STC 0
        {
            return MTR_NOT_OK;
        }
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Set a PCR Ex
 *
 *  @param  u1TsIdx         TS index (0 - 3)
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u1PidIndex      Stc set (0 - 1)
 *  @param  fgEnable        Enable PCR of the PID or not
 */
//-----------------------------------------------------------------------------
/*static MT_RESULT_T _MTDMX_SetPcrEx  (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 u1TsIdx;
    UINT8 u1Pidx;
    UINT8 u1StcSet;
    BOOL fgEnable;
    DMX_PID_T rDrvPid;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    u1TsIdx = (UINT8)rArg.ai4Arg[0];
    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u1StcSet = (UINT8)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTDMX_PRINT(" - u1Pidx = %d, fgEnable = %d\n",u1Pidx,fgEnable);

    rDrvPid.fgEnable = fgEnable;

    if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_VALID, &rDrvPid))
    {
        return MTR_NOT_OK;
    }

    if(fgEnable)
    {
        if(DMX_SetStcSrc(u1Pidx, 0) != 0)    // Force use STC 0
        {
            return MTR_NOT_OK;
        }
    }
    else
    {
        if(DMX_SetStcSrc(u1Pidx, STC_SRC_NS) != 0)    // Force use STC 0
        {
            return MTR_NOT_OK;
        }
    }

    return MTR_OK;
}*/

//-----------------------------------------------------------------------------
/*
 *  Setup a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetFilter (unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1FilterIndex;
    UINT32 u4Flags;
    MTDMX_FILTER_T *prFilter =NULL;
    MTDMX_FILTER_T rFilter;
    DMX_FILTER_T rDrvFilter;

    x_memset(&rDrvFilter, 0, sizeof(rDrvFilter));

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1FilterIndex = (UINT8)rArg.ai4Arg[0];
    u4Flags = (UINT32)rArg.ai4Arg[1];
    prFilter = (MTDMX_FILTER_T*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG(prFilter, MTDMX_FILTER_T);
    COPY_FROM_USER_ARG(prFilter, rFilter, MTDMX_FILTER_T);
    MTDMX_PRINT(" -u1FilterIndex = %u, u4Flags = 0x%x\n ",u1FilterIndex,u4Flags);
    MTDMX_PRINT_FTR("rFilter.", rFilter);

    // Driver here
    rDrvFilter.fgEnable = rFilter.fgEnable;
    rDrvFilter.fgCheckCrc = rFilter.fgCheckCrc;
    rDrvFilter.u1Pidx = rFilter.u1Pidx;
    rDrvFilter.u1Offset = rFilter.u1Offset;
    rDrvFilter.au4Data[0] = (rFilter.au1Data[0] << 24) | (rFilter.au1Data[1] << 16) |
                            (rFilter.au1Data[2] << 8) | rFilter.au1Data[3];
    rDrvFilter.au4Data[1] = (rFilter.au1Data[4] << 24) | (rFilter.au1Data[5] << 16) |
                            (rFilter.au1Data[6] << 8) | rFilter.au1Data[7];
    rDrvFilter.au4Mask[0] = (rFilter.au1Mask[0] << 24) | (rFilter.au1Mask[1] << 16) |
                            (rFilter.au1Mask[2] << 8) | rFilter.au1Mask[3];
    rDrvFilter.au4Mask[1] = (rFilter.au1Mask[4] << 24) | (rFilter.au1Mask[5] << 16) |
                            (rFilter.au1Mask[6] << 8) | rFilter.au1Mask[7];


	#if defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389)
	//rDrvFilter.eMode = DMX_FILTER_MODE_GENERIC;
	rDrvFilter.eMode = DMX_FILTER_MODE_POS_NEG;
	#else
    rDrvFilter.eMode = DMX_FILTER_MODE_LONG;
    #endif

    u4Flags |= DMX_FILTER_FLAG_MODE;
    if(!DMX_SetFilter(u1FilterIndex, 0, u4Flags, &rDrvFilter))
    {
        return MTR_NOT_OK;
    }

    rDrvFilter.u1Offset = 1;
    rDrvFilter.au4Data[0] = (rFilter.au1Data[8] << 24) | (rFilter.au1Data[9] << 16) |
                            (rFilter.au1Data[10] << 8) | rFilter.au1Data[11];
    rDrvFilter.au4Data[1] = (rFilter.au1Data[12] << 24) | (rFilter.au1Data[13] << 16) |
                            (rFilter.au1Data[14] << 8) | rFilter.au1Data[15];
    rDrvFilter.au4Mask[0] = (rFilter.au1Mask[8] << 24) | (rFilter.au1Mask[9] << 16) |
                            (rFilter.au1Mask[10] << 8) | rFilter.au1Mask[11];
    rDrvFilter.au4Mask[1] = (rFilter.au1Mask[12] << 24) | (rFilter.au1Mask[13] << 16) |
                            (rFilter.au1Mask[14] << 8) | rFilter.au1Mask[15];
    if(!DMX_SetFilter(u1FilterIndex, 1, u4Flags, &rDrvFilter))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Get a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_GetFilter (unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1FilterIndex;
    UINT32 u4Flags;
    MTDMX_FILTER_T *prFilter = NULL;
    MTDMX_FILTER_T rFilter;
    DMX_FILTER_T rDrvFilter;

    x_memset(&rDrvFilter, 0, sizeof(rDrvFilter));

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1FilterIndex = (UINT8)rArg.ai4Arg[0];
    u4Flags = (UINT32)rArg.ai4Arg[1];
    prFilter = (MTDMX_FILTER_T*)rArg.ai4Arg[2];

    if(!DMX_GetFilter(u1FilterIndex, 0, u4Flags, &rDrvFilter))
    {
        return MTR_NOT_OK;
    }

    rFilter.fgEnable = rDrvFilter.fgEnable;
    rFilter.fgCheckCrc = rDrvFilter.fgEnable;
    rFilter.u1Pidx = rDrvFilter.u1Pidx;
    rFilter.u1Offset = rDrvFilter.u1Offset;
    rFilter.au1Data[0] = (UINT8)((rDrvFilter.au4Data[0] >> 24) & 0xFF);
    rFilter.au1Data[1] = (UINT8)((rDrvFilter.au4Data[0] >> 16) & 0xFF);
    rFilter.au1Data[2] = (UINT8)((rDrvFilter.au4Data[0] >> 8) & 0xFF);
    rFilter.au1Data[3] = (UINT8)(rDrvFilter.au4Data[0] & 0xFF);
    rFilter.au1Data[4] = (UINT8)((rDrvFilter.au4Data[1] >> 24) & 0xFF);
    rFilter.au1Data[5] = (UINT8)((rDrvFilter.au4Data[1] >> 16) & 0xFF);
    rFilter.au1Data[6] = (UINT8)((rDrvFilter.au4Data[1] >> 8) & 0xFF);
    rFilter.au1Data[7] = (UINT8)(rDrvFilter.au4Data[1] & 0xFF);
    rFilter.au1Mask[0] = (UINT8)((rDrvFilter.au4Mask[0] >> 24) & 0xFF);
    rFilter.au1Mask[1] = (UINT8)((rDrvFilter.au4Mask[0] >> 16) & 0xFF);
    rFilter.au1Mask[2] = (UINT8)((rDrvFilter.au4Mask[0] >> 8) & 0xFF);
    rFilter.au1Mask[3] = (UINT8)(rDrvFilter.au4Mask[0] & 0xFF);
    rFilter.au1Mask[4] = (UINT8)((rDrvFilter.au4Mask[1] >> 24) & 0xFF);
    rFilter.au1Mask[5] = (UINT8)((rDrvFilter.au4Mask[1] >> 16) & 0xFF);
    rFilter.au1Mask[6] = (UINT8)((rDrvFilter.au4Mask[1] >> 8) & 0xFF);
    rFilter.au1Mask[7] = (UINT8)(rDrvFilter.au4Mask[1] & 0xFF);

    if(!DMX_GetFilter(u1FilterIndex, 1, u4Flags, &rDrvFilter))
    {
        return MTR_NOT_OK;
    }
    rFilter.au1Data[8] = (UINT8)((rDrvFilter.au4Data[0] >> 24) & 0xFF);
    rFilter.au1Data[9] = (UINT8)((rDrvFilter.au4Data[0] >> 16) & 0xFF);
    rFilter.au1Data[10] = (UINT8)((rDrvFilter.au4Data[0] >> 8) & 0xFF);
    rFilter.au1Data[11] = (UINT8)(rDrvFilter.au4Data[0] & 0xFF);
    rFilter.au1Data[12] = (UINT8)((rDrvFilter.au4Data[1] >> 24) & 0xFF);
    rFilter.au1Data[13] = (UINT8)((rDrvFilter.au4Data[1] >> 16) & 0xFF);
    rFilter.au1Data[14] = (UINT8)((rDrvFilter.au4Data[1] >> 8) & 0xFF);
    rFilter.au1Data[15] = (UINT8)(rDrvFilter.au4Data[1] & 0xFF);
    rFilter.au1Mask[8] = (UINT8)((rDrvFilter.au4Mask[0] >> 24) & 0xFF);
    rFilter.au1Mask[9] = (UINT8)((rDrvFilter.au4Mask[0] >> 16) & 0xFF);
    rFilter.au1Mask[10] = (UINT8)((rDrvFilter.au4Mask[0] >> 8) & 0xFF);
    rFilter.au1Mask[11] = (UINT8)(rDrvFilter.au4Mask[0] & 0xFF);
    rFilter.au1Mask[12] = (UINT8)((rDrvFilter.au4Mask[1] >> 24) & 0xFF);
    rFilter.au1Mask[13] = (UINT8)((rDrvFilter.au4Mask[1] >> 16) & 0xFF);
    rFilter.au1Mask[14] = (UINT8)((rDrvFilter.au4Mask[1] >> 8) & 0xFF);
    rFilter.au1Mask[15] = (UINT8)(rDrvFilter.au4Mask[1] & 0xFF);

    USR_SPACE_ACCESS_VALIDATE_ARG(prFilter, MTDMX_FILTER_T);
    COPY_TO_USER_ARG(prFilter, rFilter, MTDMX_FILTER_T);

    MTDMX_PRINT(" -u1FilterIndex = %u, u4Flags = 0x%x\n ",u1FilterIndex,u4Flags);
    MTDMX_PRINT_FTR("rFilter.", rFilter);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Allocate section buffer pool
 *
 *  @param  u4Size          Buffer size
 *  @param  pu4Addr         Return kernel address
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PSI_AllocateBuffer (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Size;
    UINT32 *pu4Addr = NULL;
    UINT32 u4Addr;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Size = (UINT32)rArg.ai4Arg[0];
    pu4Addr = (UINT32*)rArg.ai4Arg[1];

    u4Addr = (UINT32)x_mem_alloc(u4Size);
    if(u4Addr == 0)
    {
        return MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Addr, UINT32);
    COPY_TO_USER_ARG(pu4Addr, u4Addr, UINT32);

    MTDMX_PRINT(" - u4Size = 0x%x, u4Addr = 0x%x\n", u4Size, u4Addr);
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Free a section buffer pool
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PSI_FreeBuffer(unsigned long arg)
{
    UINT32 u4Addr;
    INT32 Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u4Addr = (UINT32)Arg;

    if(u4Addr == 0)
    {
        return MTR_NOT_OK;
    }

    x_mem_free((VOID*)u4Addr);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Copy data from a section buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address (Kernel address)
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param  pu1Dest         Destination buffer (User address)
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PSI_GetBuffer(unsigned long arg)
{
    MTAL_IOCTL_6ARG_T    rArg;
    UINT8 u1Pidx;
    UINT8 u1SerialNumber;
    UINT32 u4FrameAddr;
    UINT32 u4SkipSize;
    UINT32 u4CopySize;
    UCHAR* pu1Dest =NULL;

    USR_SPACE_ACCESS_VALIDATE_6ARG(arg);
    COPY_FROM_USER_6ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u1SerialNumber = (UINT8)rArg.ai4Arg[1];
    u4FrameAddr = (UINT32)rArg.ai4Arg[2];
    u4SkipSize = (UINT32)rArg.ai4Arg[3];
    u4CopySize = (UINT32)rArg.ai4Arg[4];
    pu1Dest = (UCHAR*)rArg.ai4Arg[5];

    if(!DMX_PSI_GetBuffer(u1Pidx, u1SerialNumber, u4FrameAddr,
                          u4SkipSize, u4CopySize, _au1SecBuf))
    {
        return MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu1Dest, u4CopySize);
    COPY_TO_USER_ARG_SIZE(pu1Dest, _au1SecBuf, u4CopySize);

    MTDMX_PRINT(" - u1Pidx = %u\n"
        "u1SerialNumber = 0x%x\n"
        "u4FrameAddr = 0x%x\n"
        "u4SkipSize = 0x%x\n"
        "u4CopySize = 0x%x\n",
              u1Pidx,
              u1SerialNumber,
              u4FrameAddr,
              u4SkipSize,
              u4CopySize);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Unlock a section buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address (Kernel address)
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PSI_UnlockBuffer(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T    rArg;
    UINT8 u1Pidx;
    UINT8 u1SerialNumber;
    UINT32 u4Size;
    UINT32 u4FrameAddr;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u1SerialNumber = (UINT8)rArg.ai4Arg[1];
    u4Size = (UINT32)rArg.ai4Arg[2];
    u4FrameAddr = (UINT32)rArg.ai4Arg[3];

    MTDMX_PRINT(" - u1Pidx = %u\n"
        "u1SerialNumber = 0x%x\n"
        "u4Size = 0x%x\n"
        "u4FrameAddr = 0x%x\n",
              u1Pidx,
              u1SerialNumber,
              u4Size,
              u4FrameAddr);

    if(!DMX_PSI_UnlockBuffer(u1Pidx, u1SerialNumber, u4Size, u4FrameAddr))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/*
 *  Flush a section buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PSI_FlushBuffer (unsigned long arg)
{
    UINT8 u1Pidx;
    INT32 Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u1Pidx = (UINT8)Arg;

    MTDMX_PRINT(" - u1Pidx = %d\n",u1Pidx);

    //add driver implementation code here
    if(!DMX_PSI_FlushBuffer(u1Pidx))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Get section header and CRC
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u1SerialNumber  The serial number
 *  @param  prHeaderCRC     Section header/CRC structure
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PSI_GetBufferSectionHeader(unsigned long arg)
{
    MTAL_IOCTL_6ARG_T rArg;
    UINT8 u1Pidx;
    UINT8 u1SerialNumber;
    UINT32 u4FrameAddr;
    UINT32 u4SkipSize;
    UINT32 u4CopySize;
    MTDMX_SECTION_HEADER_T *prHeader = NULL;
    MTDMX_SECTION_HEADER_T rHeader;
    UINT32 u4Offset;

    USR_SPACE_ACCESS_VALIDATE_6ARG(arg);
    COPY_FROM_USER_6ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u1SerialNumber = (UINT8)rArg.ai4Arg[1];
    u4FrameAddr = (UINT32)rArg.ai4Arg[2];
    u4SkipSize = (UINT32)rArg.ai4Arg[3];
    u4CopySize = (UINT32)rArg.ai4Arg[4];
    prHeader = (MTDMX_SECTION_HEADER_T *)rArg.ai4Arg[5];

    if(!DMX_PSI_GetBuffer(u1Pidx, u1SerialNumber, u4FrameAddr,
                          u4SkipSize, u4CopySize, _au1SecBuf))
    {
        return MTR_NOT_OK;
    }

    rHeader.u1TableID = _au1SecBuf[0];
    rHeader.u2SecLen = ((_au1SecBuf[1] & 0xf) << 8) | _au1SecBuf[2];
    rHeader.u1VerNum = (_au1SecBuf[5] & 0x3e) >> 1;
    u4Offset = (UINT32)rHeader.u2SecLen + 3;

    if(u4Offset > MAX_SECTION_SIZE)
    {
        return MTR_NOT_OK;
    }

    rHeader.u4CRC = (UINT32)_au1SecBuf[u4Offset - 1] |
                    ((UINT32)_au1SecBuf[u4Offset - 2] << 8) |
                    ((UINT32)_au1SecBuf[u4Offset - 3] << 16) |
                    ((UINT32)_au1SecBuf[u4Offset - 4] << 24);

    USR_SPACE_ACCESS_VALIDATE_ARG(prHeader, MTDMX_SECTION_HEADER_T);
    COPY_TO_USER_ARG(prHeader, rHeader, MTDMX_SECTION_HEADER_T);

    MTDMX_PRINT(" - u1Pidx = %u\n"
        "u1SerialNumber = 0x%x\n",
              u1Pidx,
              u1SerialNumber);
    MTDMX_PRINT_SEC("rHeader.",rHeader);
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Allocate PES buffer pool
 *
 *  @param  u4Size          Buffer size
 *  @param  pu4Addr         Return allocated buffer address (Kernel address)
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PES_AllocateBuffer(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Size;
    UINT32 *pu4Addr = NULL;
    UINT32 u4Addr;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Size = (UINT32)rArg.ai4Arg[0];
    pu4Addr = (UINT32*)rArg.ai4Arg[1];

    u4Addr = (UINT32)x_mem_alloc(u4Size);
    if(u4Addr == 0)
    {
        return MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Addr, UINT32);
    COPY_TO_USER_ARG(pu4Addr, u4Addr, UINT32);

    MTDMX_PRINT(" - u4Size = 0x%x, u4Addr = 0x%x\n", u4Size, u4Addr);
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Free a PES buffer pool
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PES_FreeBuffer(unsigned long arg)
{
    UINT32 u4Addr;
    INT32 Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u4Addr =(UINT32)Arg;

    if(u4Addr == 0)
    {
        return MTR_NOT_OK;
    }

    x_mem_free((VOID*)u4Addr);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Unlock a PES buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address (Kernel address)
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PES_UnlockBuffer(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T    rArg;
    UINT8 u1Pidx;
    UINT8 u1SerialNumber;
    UINT32 u4Size;
    UINT32 u4FrameAddr;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u1SerialNumber = (UINT8)rArg.ai4Arg[1];
    u4Size = (UINT32)rArg.ai4Arg[2];
    u4FrameAddr = (UINT32)rArg.ai4Arg[3];

    MTDMX_PRINT(" - u1Pidx = %u\n"
        "u1SerialNumber = 0x%x\n"
        "u4Size = 0x%x\n"
        "u4FrameAddr = 0x%x\n",
              u1Pidx,
              u1SerialNumber,
              u4Size,
              u4FrameAddr);

    if(!DMX_PES_UnlockBuffer(u1Pidx, u1SerialNumber, u4Size, u4FrameAddr))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Flush a PES buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PES_FlushBuffer(unsigned long arg)
{
    UINT8 u1Pidx;
    INT32 Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u1Pidx = (UINT8)Arg;

    MTDMX_PRINT(" - u1Pidx = %d\n",u1Pidx);

    if(!DMX_PES_FlushBuffer(u1Pidx))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Copy data from a PES buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address (Kernel mode address)
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param  pu1Dest         Destination buffer (User mode address)
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_PES_GetBuffer (unsigned long arg)
{
    MTAL_IOCTL_6ARG_T    rArg;
    UINT8 u1Pidx;
    UINT8 u1SerialNumber;
    UINT32 u4FrameAddr;
    UINT32 u4SkipSize;
    UINT32 u4CopySize;
    UCHAR* pu1Dest =NULL;

    USR_SPACE_ACCESS_VALIDATE_6ARG(arg);
    COPY_FROM_USER_6ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    u1SerialNumber = (UINT8)rArg.ai4Arg[1];
    u4FrameAddr = (UINT32)rArg.ai4Arg[2];
    u4SkipSize = (UINT32)rArg.ai4Arg[3];
    u4CopySize = (UINT32)rArg.ai4Arg[4];
    pu1Dest = (UCHAR*)rArg.ai4Arg[5];

    if(!DMX_PES_GetBuffer(u1Pidx, u1SerialNumber, u4FrameAddr,
                          u4SkipSize, u4CopySize, _au1PesBuf))
    {
        return MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1Dest, u4CopySize);
    COPY_TO_USER_ARG_SIZE(pu1Dest, _au1PesBuf, u4CopySize);

    MTDMX_PRINT(" - u1Pidx = %u\n"
        "u1SerialNumber = 0x%x\n"
        "u4FrameAddr = 0x%x\n"
        "u4SkipSize = 0x%x\n"
        "u4CopySize = 0x%x\n",
              u1Pidx,
              u1SerialNumber,
              u4FrameAddr,
              u4SkipSize,
              u4CopySize);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Get the counter structure of a given pid index
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  prCounters [output]
 *                          Pointer to the PID counter structure
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_GetPidCounters (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 u1Pidx;
    MTDMX_PID_COUNTERS_T * prCounters =NULL;
    MTDMX_PID_COUNTERS_T rCounters;
    DMX_PID_COUNTERS_T rDrvCounters;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    prCounters = (MTDMX_PID_COUNTERS_T*)rArg.ai4Arg[1];

    if(!DMX_GetPidCounters(u1Pidx, &rDrvCounters))
    {
        return MTR_NOT_OK;
    }

    rCounters.u4PesCount = rDrvCounters.u4PesCount;
    rCounters.u4PicCount = rDrvCounters.u4PicCount;
    rCounters.u4SecCount = rDrvCounters.u4SecCount;

    USR_SPACE_ACCESS_VALIDATE_ARG(prCounters, MTDMX_PID_COUNTERS_T);
    COPY_TO_USER_ARG(prCounters, rCounters, MTDMX_PID_COUNTERS_T);

    MTDMX_PRINT(" -u1Pidx = %u ", u1Pidx);
    MTDMX_PID_CNT("rCounters.", rCounters);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** MTDMX_ResetPidCounters
 *  Reset counters of a given pid index
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_ResetPidCounters (unsigned long arg)
{
    UINT8 u1Pidx;
    INT32 Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u1Pidx = (UINT8)Arg;

    MTDMX_PRINT(" -u1Pidx = %u \n",u1Pidx);

    if(!DMX_ResetPidCounters(u1Pidx))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Get lock state
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_GetLockState(unsigned long arg)
{
    BOOL *pfgLock = NULL;
    BOOL fgLock;

    pfgLock = (BOOL*)arg;

    fgLock = DMX_GetLockState();

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgLock, BOOL);
    COPY_TO_USER_ARG(pfgLock, fgLock, BOOL);

    MTDMX_PRINT(" -fgLock = %d\n", fgLock);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Get scramble state
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @return     The scramble state
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_GetScrambleState(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 u1Pidx ;
    MTDMX_SCRAMBLE_STATE_T* peScramble_State = NULL;
    MTDMX_SCRAMBLE_STATE_T eScramble_State;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    peScramble_State = (MTDMX_SCRAMBLE_STATE_T*)rArg.ai4Arg[1];

    eScramble_State = (MTDMX_SCRAMBLE_STATE_T)DMX_GetScrambleState(u1Pidx);

    USR_SPACE_ACCESS_VALIDATE_ARG(peScramble_State, MTDMX_SCRAMBLE_STATE_T);
    COPY_TO_USER_ARG(peScramble_State, eScramble_State, MTDMX_SCRAMBLE_STATE_T);

    MTDMX_PRINT(" -u1Pidx = %d, eScramble_State = %d\n",u1Pidx,eScramble_State);
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Get current STC
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_GetCurSTC(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    STC_CLOCK rClk;
    UINT32 u4StcHi, u4StcLo;
    UINT32 *pu4StcHi, *pu4StcLo;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu4StcHi = (UINT32*)rArg.ai4Arg[0];
    pu4StcLo = (UINT32*)rArg.ai4Arg[1];

    if(STC_GetSrc(0, &rClk) != 0)
    {
        return MTR_NOT_OK;
    }

    u4StcLo = rClk.u4Base;
    u4StcHi = (UINT32)rClk.ucBaseHi | ((UINT32)rClk.u2Ext << 1);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4StcHi, UINT32);
    COPY_TO_USER_ARG(pu4StcHi, u4StcHi, UINT32);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4StcLo, UINT32);
    COPY_TO_USER_ARG(pu4StcLo, u4StcLo, UINT32);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/*
 *  Set generic section filter
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetGenericFilter(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 i, u1FilterIndex;
    UINT32 u4Flags;
    MTDMX_GENERIC_FILTER_T *prFilter =NULL;
    MTDMX_GENERIC_FILTER_T rFilter;
    DMX_FILTER_GENERIC_T rDrvFilter;

    x_memset(&rDrvFilter, 0, sizeof(rDrvFilter));

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1FilterIndex = (UINT8)rArg.ai4Arg[0];
    u4Flags = (UINT32)rArg.ai4Arg[1];
    prFilter = (MTDMX_GENERIC_FILTER_T*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG(prFilter, MTDMX_GENERIC_FILTER_T);
    COPY_FROM_USER_ARG(prFilter, rFilter, MTDMX_GENERIC_FILTER_T);
    MTDMX_PRINT(" -u1FilterIndex = %u, u4Flags = 0x%x\n ",u1FilterIndex,u4Flags);
    MTDMX_PRINT_FTR("rFilter.", rFilter);

    // Driver here
    rDrvFilter.fgEnable = rFilter.fgEnable;
    rDrvFilter.fgCheckCrc = rFilter.fgCheckCrc;
    rDrvFilter.u1Pidx = rFilter.u1Pidx;
    rDrvFilter.u1Offset = rFilter.u1Offset;

    for(i=0; i<16; i++)
    {
        rDrvFilter.au1Data[i] = rFilter.au1Data[i];
        rDrvFilter.au1Mask[i] = rFilter.au1Mask[i];
        rDrvFilter.au1PosNeg[i] = rFilter.au1PosNeg[i];
    }

    if(!DMX_SetGenFilter(u1FilterIndex, u4Flags, &rDrvFilter))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/*
 *  Get generic section filter setting
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_GetGenFilter (unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1FilterIndex;
    UINT32 u4Flags;
    MTDMX_GENERIC_FILTER_T *prFilter = NULL;
    MTDMX_GENERIC_FILTER_T rFilter;

    x_memset(&rFilter, 0, sizeof(rFilter));

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1FilterIndex = (UINT8)rArg.ai4Arg[0];
    u4Flags = (UINT32)rArg.ai4Arg[1];
    prFilter = (MTDMX_GENERIC_FILTER_T*)rArg.ai4Arg[2];

    if(!DMX_GetGenFilter(u1FilterIndex, u4Flags, (DMX_FILTER_GENERIC_T*)&rFilter))
    {
        return MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prFilter, MTDMX_GENERIC_FILTER_T);
    COPY_TO_USER_ARG(prFilter, rFilter, MTDMX_GENERIC_FILTER_T);

    MTDMX_PRINT(" -u1FilterIndex = %u, u4Flags = 0x%x\n ",u1FilterIndex,u4Flags);
    MTDMX_PRINT_FTR("rFilter.", rFilter);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** Set DES desrambler key
 *
 *  @param  u1Index         The key index
 *  @param  fg3Des          TRUE: 3DES, FALSE: DES
 *  @param  fgEven          TRUE: even key, FALSE: odd key
 *  @param  au4Key          The key
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetDesKey(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T    rArg;
    UINT8 u1KeyIdx;
    BOOL fg3Des;
    BOOL fgEven;
    UINT32* pu4Key;
    UINT32 au4Key[6];

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    u1KeyIdx = (UINT8)rArg.ai4Arg[0];
    fg3Des = (BOOL)rArg.ai4Arg[1];
    fgEven = (BOOL)rArg.ai4Arg[2];
    pu4Key = (UINT32*)rArg.ai4Arg[3];

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu4Key, 24);
    COPY_FROM_USER_ARG_SIZE(pu4Key, au4Key, 24);

    MTDMX_PRINT(" -u1Index=%d, fg3Des=%d, fgEven=%d\n", u1KeyIdx, (int)fg3Des, (int)fgEven);

    if(!DMX_SetDesKey(u1KeyIdx, fg3Des, fgEven, au4Key))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** Set the initial vector of DES
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetDesIV(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4IvHi;
    UINT32 u4IvLo;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4IvHi = (UINT32)rArg.ai4Arg[0];
    u4IvLo = (UINT32)rArg.ai4Arg[1];

    MTDMX_PRINT(" -u4IvHi=0x%x, u4IvLo=0x%x\n", (unsigned int)u4IvHi,
        (unsigned int)u4IvLo);

    DMX_SetDesIV(u4IvHi, u4IvLo);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** Set the initial vector of DES
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetDesOddEvenIV(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T    rArg;
    UINT8 u1Idx;
    UINT32 u4IvHi;
    UINT32 u4IvLo;
    BOOL fgEven;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    u1Idx = (UINT8)rArg.ai4Arg[0];
    fgEven = (BOOL)rArg.ai4Arg[1];
    u4IvHi = (UINT32)rArg.ai4Arg[2];
    u4IvLo = (UINT32)rArg.ai4Arg[3];

    MTDMX_PRINT(" -u4IvHi=0x%x, u4IvLo=0x%x\n", (unsigned int)u4IvHi,
        (unsigned int)u4IvLo);

#ifndef CC_MT5392B
    DMX_SetDesIVEx2(u1Idx, u4IvHi, u4IvLo, fgEven);
#endif

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** Set RTB mode of DES
 *
 *  @param  eMode          MTDMX_DESC_RTB_MODE_T
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetDesRtbMode(unsigned long arg)
{
#ifndef CC_MT5392B
    UINT8 u1Idx;
    DMX_DESC_RTB_MODE_T eMode;
    MTAL_IOCTL_2ARG_T rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1Idx = (UINT8)rArg.ai4Arg[0];
    eMode = (DMX_DESC_RTB_MODE_T)rArg.ai4Arg[1];

    MTDMX_PRINT(" - u1Pidx = %d \n", eMode);

    DMX_SetDesRtbMode(u1Idx, eMode);
#endif

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** Set AES key len
 *
 *  @param  u4KeyLen        The key length
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetAesKeyLen(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 u1Idx;
    UINT32 u4KeyLen;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1Idx = (UINT8)rArg.ai4Arg[0];
    u4KeyLen = (UINT32)rArg.ai4Arg[1];

    MTDMX_PRINT(" - KeyLen=%d\n", (int)u4KeyLen);

    DMX_SetAesKeyLen(u1Idx, u4KeyLen);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** Set AES key
 *
 *  @param  u1Index         The key index
 *  @param  fgEven          If fgEven=TRUE means set even key
 *  @param  au4Key          The key value
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetAesKey(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1KeyIdx;
    BOOL fgEven;
    UINT32* pu4Key;
    UINT32 au4Key[8];

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1KeyIdx = (UINT8)rArg.ai4Arg[0];
    fgEven = (BOOL)rArg.ai4Arg[1];
    pu4Key = (UINT32*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu4Key, 32);
    COPY_FROM_USER_ARG_SIZE(pu4Key, au4Key, 32);

    MTDMX_PRINT(" - u1Index=%d, fgEven=%d\n", (int)u1KeyIdx, (int)fgEven);

    if(!DMX_SetAesKey(u1KeyIdx, fgEven, au4Key))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** Set the initial vector of AES
 *
 *  @param  au4Iv           The initial vector
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetAesIV(unsigned long arg)
{
    INT32    Arg;
    UINT32* pu4Iv;
    UINT32 au4Iv[4];

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    pu4Iv = (UINT32*)Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu4Iv, 16);
    COPY_FROM_USER_ARG_SIZE(pu4Iv, au4Iv, 16);

    MTDMX_PRINT(" - iv0=0x%x, iv1=0x%x, iv2=0x%x, iv3=0x%x\n",
        (unsigned int)au4Iv[0], (unsigned int)au4Iv[1], (unsigned int)au4Iv[2],
        (unsigned int)au4Iv[3]);

    DMX_SetAesIV(au4Iv);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** Set the odd/even initial vector
 *
 *  @param  au4Iv           The initial vector
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetAesOddEvenIV(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T  rArg;
    UINT8 u1Idx;
    BOOL fgEven;
    UINT32* pu4Iv;
    UINT32 au4Iv[4];

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Idx = (UINT8)rArg.ai4Arg[0];
    fgEven = (BOOL)rArg.ai4Arg[1];
    pu4Iv = (UINT32*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu4Iv, 16);
    COPY_FROM_USER_ARG_SIZE(pu4Iv, au4Iv, 16);

#ifndef CC_MT5392B
    DMX_SetAesIVEx2(u1Idx, au4Iv, fgEven);
#endif

    UNUSED(fgEven);
    UNUSED(au4Iv);

    return MTR_OK;
}

/** _MTDMX_SetDVBKey
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetDVBKey(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT8 u1Index;
    BOOL fgEven;
    UINT32 *pu4Key;
    UINT32 au4Key[2];

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Index = (UINT8)rArg.ai4Arg[0];
    fgEven = (BOOL)rArg.ai4Arg[1];
    pu4Key = (UINT32*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu4Key, 8);
    COPY_FROM_USER_ARG_SIZE(pu4Key, au4Key, 8);

    if(!DMX_SetDvbKey(u1Index, fgEven, au4Key))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}
//-----------------------------------------------------------------------------
/** Set the initial vector of DVB
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetDVBIV(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T    rArg;
    UINT8 u1Idx;
    UINT32 u4IvHi;
    UINT32 u4IvLo;
    BOOL fgEven;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    u1Idx = (UINT8)rArg.ai4Arg[0];
    fgEven = (BOOL)rArg.ai4Arg[1];
    u4IvHi = (UINT32)rArg.ai4Arg[2];
    u4IvLo = (UINT32)rArg.ai4Arg[3];

    MTDMX_PRINT(" -u4IvHi=0x%x, u4IvLo=0x%x\n", (unsigned int)u4IvHi,
        (unsigned int)u4IvLo);

    DMX_SetDvbIVEx(u4IvHi, u4IvLo, fgEven);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** Set the initial vector of AES
 *
 *  @param  u1Pidx           PID index value
 *  @param  eDescMode        Descramble mode
 *  @param  u1KyeIndex       Key index value
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetDescramblPid(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1Pidx;
    MTDMX_DESC_MODE_T eDescMode;
    UINT8 u1KeyIndex;
    DMX_PID_T rPid;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    eDescMode = (MTDMX_DESC_MODE_T)rArg.ai4Arg[1];
    u1KeyIndex = (UINT8)rArg.ai4Arg[2];

    MTDMX_PRINT(" - u1Pidx=%d, eDescMode=%d, u1KeyIndex=%d\n",
                        (int)u1Pidx, (int)eDescMode, (int)u1KeyIndex);

    rPid.eDescMode = (DMX_DESC_MODE_T)eDescMode;
    rPid.u1KeyIndex = u1KeyIndex;
    if(!DMX_SetPid(u1Pidx, DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_KEY_INDEX, &rPid))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** Set the initial vector of AES
 *
 *  @param  u1Pidx           PID index value
 *  @param  eDescMode        Descramble mode
 *  @param  u1KyeIndex       Key index value
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetFileDescramblPid(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 u1Pidx;
    MTDMX_DESC_MODE_T eDescMode;
    UINT8 u1KeyIndex;
    DMX_PID_T rPid;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    eDescMode = (MTDMX_DESC_MODE_T)rArg.ai4Arg[1];
    u1KeyIndex = (UINT8)rArg.ai4Arg[2];

    MTDMX_PRINT(" - u1Pidx=%d, eDescMode=%d, u1KeyIndex=%d\n",
                        (int)u1Pidx, (int)eDescMode, (int)u1KeyIndex);

    rPid.eDescMode = (DMX_DESC_MODE_T)eDescMode;
    rPid.u1KeyIndex = u1KeyIndex;
    if(!DMX_SetFilePid(u1Pidx, DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_KEY_INDEX, &rPid))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** Set RTB mode of AES
 *
 *  @param  eMode          MTDMX_DESC_RTB_MODE_T
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetAesRtbMode(unsigned long arg)
{
#ifndef CC_MT5392B
    UINT8 u1Idx;
    DMX_DESC_RTB_MODE_T eMode;
    MTAL_IOCTL_2ARG_T rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1Idx = (UINT8)rArg.ai4Arg[0];
    eMode = (DMX_DESC_RTB_MODE_T)rArg.ai4Arg[1];

    MTDMX_PRINT(" - u1Pidx = %d \n", eMode);

    DMX_SetAesRtbMode(u1Idx, eMode);
#endif

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** Set the initial vector of AES
 *
 *  @param  u1Pidx           PID index value
 *  @param  eDescMode        Descramble mode
 *  @param  u1KyeIndex       Key index value
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetAesOn(unsigned long arg)
{
    INT32 Arg;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    fgEnable = (BOOL)Arg;
/*
    if(fgEnable)
    {
        if(!DMX_CIPlus_Set(1))
        {
            return MTR_OK;
        }
    }
    else
    {
        if(!DMX_CIPlus_Free(1))
        {
            return MTR_OK;
        }
    }
*/
    UNUSED(fgEnable);

    return MTR_OK;
}


#ifdef ENABLE_MULTIMEDIA
static MT_RESULT_T _MTDMX_AES_SetDecryptInfo(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 i;
    UINT32 u4Flags;
    MTDMX_DRM_AES_T* prDrm =NULL;
    MTDMX_DRM_AES_T rDrm;
    DMX_MM_DRM_AES_T rDrvDrm;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Flags = (UINT32)rArg.ai4Arg[0];
    prDrm = (MTDMX_DRM_AES_T*)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG(prDrm, MTDMX_DRM_AES_T);
    COPY_FROM_USER_ARG(prDrm, rDrm, MTDMX_DRM_AES_T);

    rDrvDrm.fgCBC = rDrm.fgCBC;
    rDrvDrm.u4KeyBitLen = rDrm.u4KeyBitLen;
    rDrvDrm.u4Offset = rDrm.u4Offset;
    rDrvDrm.u4EncryLen = rDrm.u4EncryLen;

#if !defined(CC_MT5392B) && !defined(CC_MT5363)
    rDrvDrm.fgEncrypt = rDrm.fgEncrypt;
#elif defined(CC_MT5363)
    // Our MTAL interface does not support encrypt now.
    // Let it always be FALSE to avoid from ambiguous
    rDrvDrm.fgEncrypt = FALSE;
#endif

    for(i=0; i <16; i++)
    {
        rDrvDrm.au1IV[i] = rDrm.au1IV[i];
    }

    for(i=0; i <32; i++)
    {
        rDrvDrm.au1Key[i] = rDrm.au1Key[i];
    }

    if(!DMX_AES_SetDecryptInfo(u4Flags, &rDrvDrm))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


static MT_RESULT_T _MTDMX_AES_MoveBlockData(unsigned long arg)
{
    INT32    Arg;
    MTDMX_DRM_AES_BLKDATA_T* prBlkData =NULL;
    MTDMX_DRM_AES_BLKDATA_T rBlkData;
    //DMX_AES_BLKDATA_T rDrvBlkData;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    prBlkData = (MTDMX_DRM_AES_BLKDATA_T*)Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prBlkData, MTDMX_DRM_AES_BLKDATA_T);
    COPY_FROM_USER_ARG(prBlkData, rBlkData, MTDMX_DRM_AES_BLKDATA_T);

    if(!DMX_AES_MoveBlockData((DMX_AES_BLKDATA_T*)&rBlkData))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


static MT_RESULT_T _MTDMX_AES_SetDecryptInfoEx(unsigned long arg)
{
#ifndef CC_MT5392B
    MTAL_IOCTL_3ARG_T    rArg;
    UINT8 i;
    UINT32 u4Flags;
    MTDMX_DRM_AES_T* prDrm =NULL;
    MTDMX_DRM_AES_T rDrm;
    DMX_MM_DRM_AES_T rDrvDrm;
    UINT8 u1Idx;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Idx = (UINT8)rArg.ai4Arg[0];
    u4Flags = (UINT32)rArg.ai4Arg[1];
    prDrm = (MTDMX_DRM_AES_T*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG(prDrm, MTDMX_DRM_AES_T);
    COPY_FROM_USER_ARG(prDrm, rDrm, MTDMX_DRM_AES_T);

    rDrvDrm.fgCBC = rDrm.fgCBC;
    rDrvDrm.u4KeyBitLen = rDrm.u4KeyBitLen;
    rDrvDrm.u4Offset = rDrm.u4Offset;
    rDrvDrm.u4EncryLen = rDrm.u4EncryLen;

#if !defined(CC_MT5392B) && !defined(CC_MT5363)
    rDrvDrm.fgEncrypt = rDrm.fgEncrypt;
#elif defined(CC_MT5363)
    // Our MTAL interface does not support encrypt now.
    // Let it always be FALSE to avoid from ambiguous
    rDrvDrm.fgEncrypt = FALSE;
#endif

    for(i=0; i <16; i++)
    {
        rDrvDrm.au1IV[i] = rDrm.au1IV[i];
    }

    for(i=0; i <32; i++)
    {
        rDrvDrm.au1Key[i] = rDrm.au1Key[i];
    }

    if(!DMX_AES_SetDecryptInfoEx(u1Idx, u4Flags, &rDrvDrm))
    {
        return MTR_NOT_OK;
    }
#endif

    return MTR_OK;
}

static MT_RESULT_T _MTDMX_AES_AllocEngine(unsigned long arg)
{
#ifndef CC_MT5392B
    INT32    Arg;
    UINT8 u1Idx;
    UINT8 *pu1Idx;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    pu1Idx = (UINT8 *)Arg;

    u1Idx = DMX_AES_AllocEngine();

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1Idx, UINT8);
    COPY_TO_USER_ARG(pu1Idx, u1Idx, UINT32);
#endif

    return MTR_OK;
}


static MT_RESULT_T _MTDMX_AES_FreeEngine(unsigned long arg)
{
#ifndef CC_MT5392B
    INT32    Arg;
    UINT8 u1Idx;
    //UINT8 *pu1Idx;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u1Idx = (UINT8)Arg;

    if(!DMX_AES_FreeEngine(u1Idx))
    {
        return MTR_NOT_OK;
    }
#endif

    return MTR_OK;
}


static MT_RESULT_T _MTDMX_AES_MoveBlockDataEx(unsigned long arg)
{
#ifndef CC_MT5392B
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 u1Idx;
    MTDMX_DRM_AES_BLKDATA_T* prBlkData =NULL;
    MTDMX_DRM_AES_BLKDATA_T rBlkData;
    //DMX_AES_BLKDATA_T rDrvBlkData;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1Idx = (UINT32)rArg.ai4Arg[0];
    prBlkData = (MTDMX_DRM_AES_BLKDATA_T*)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG(prBlkData, MTDMX_DRM_AES_BLKDATA_T);
    COPY_FROM_USER_ARG(prBlkData, rBlkData, MTDMX_DRM_AES_BLKDATA_T);

    if(!DMX_AES_MoveBlockDataEx(u1Idx, (DMX_AES_BLKDATA_T*)&rBlkData))
    {
        return MTR_NOT_OK;
    }
#endif

    return MTR_OK;
}

#endif //ENABLE_MULTIMEDIA


static MT_RESULT_T _MTDMX_GCPU_Cmd(unsigned long arg)
{
#if defined(CC_MT5392B) || defined(CC_MT5365) || defined(CC_MT5395)
    return MTR_NOT_OK;
#else
    MTAL_IOCTL_5ARG_T    rArg;
    UINT32 u4Handle;
    UINT32 u4Cmd;
    void* pvUserParam;
    void* pvParam;
    UINT32 u4Size;
	BOOL fgIommu;

    USR_SPACE_ACCESS_VALIDATE_5ARG(arg);
    COPY_FROM_USER_5ARG(arg, rArg);

    u4Handle = (UINT32)rArg.ai4Arg[0];
    u4Cmd = (UINT32)rArg.ai4Arg[1];
    pvUserParam = (void *)rArg.ai4Arg[2];
    u4Size = (UINT32)rArg.ai4Arg[3];
	fgIommu = (BOOL)rArg.ai4Arg[4];

    pvParam = x_mem_alloc(u4Size);
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pvUserParam, u4Size);
    COPY_FROM_USER_ARG_SIZE(pvUserParam, pvParam, u4Size);

    MTDMX_PRINT(" -u4Handle=%d, u4Cmd=%d, u4ParamSize=%d\n", u4Handle, (int)u4Cmd, (int)u4Size);

    if(GCPU_CmdEx(u4Handle, u4Cmd, pvParam, fgIommu) != S_GCPU_OK)
    {
        return MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pvUserParam, u4Size);
    COPY_TO_USER_ARG_SIZE(pvUserParam, pvParam, u4Size);
    x_mem_free(pvParam);

    return MTR_OK;
#endif
}


//-----------------------------------------------------------------------------
/** Set PID input source type
 *
 *  @param  u1Pidx           PID index value
 *  @param  eType            PID input source type
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetPidInputSource(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 u1Pidx ;
    MTDMX_PID_INSRC_T eType;
    DMX_PID_T rPid;
#ifndef CC_MT5392B
    DMX_SOURCE_T eSource;
#endif

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1Pidx = (UINT8)rArg.ai4Arg[0];
    eType = (MTDMX_PID_INSRC_T)rArg.ai4Arg[1];
#ifndef CC_MT5392B
    if(eType == MTDMX_PID_INSRC_DTV)
    {
        eSource = DMX_SOURCE_DTV0;
    }
    else if((eType == MTDMX_PID_INSRC_TIMESHIFT_PLAY) || (eType == MTDMX_PID_INSRC_MM))
    {
        eSource = DMX_SOURCE_TSFILE0;
    }
    else
    {
        return MTR_NOT_OK;
    }

    if(!DMX_SetPidEx(u1Pidx, DMX_PID_FLAG_NONE, &rPid, eSource))
    {
        return MTR_NOT_OK;
    }
#else
    if(eType == MTDMX_PID_INSRC_DTV)
    {
        rPid.u1TsIndex = DMX_GetFramerIndex();
    }
    else if(eType == MTDMX_PID_INSRC_TIMESHIFT_PLAY)
    {
        rPid.u1TsIndex = 2;
    }
    else if(eType == MTDMX_PID_INSRC_MM)
    {
        rPid.u1TsIndex = 0;
    }
    else
    {
        return MTR_NOT_OK;
    }

    if(!DMX_SetPid(u1Pidx, DMX_PID_FLAG_TS_INDEX, &rPid))
    {
        return MTR_NOT_OK;
    }
#endif

    return MTR_OK;
}

#ifdef CC_IOMTDMX_MULTI2
//-----------------------------------------------------------------------------
/** _MTDMX_SetMulti2Config
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetMulti2Config(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    BOOL fgOfbFreeRun;
    UINT16 u2Iteration;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    fgOfbFreeRun = (BOOL)rArg.ai4Arg[0];
    u2Iteration = (UINT16)rArg.ai4Arg[1];

    DMX_SetMulti2Config(fgOfbFreeRun, u2Iteration);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDMX_SetMulti2Key
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetMulti2Key(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT8 u1Index;
    BOOL fgEven;
    UINT32 *pu4Key;
    UINT32 au4Key[2];

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1Index = (UINT8)rArg.ai4Arg[0];
    fgEven = (BOOL)rArg.ai4Arg[1];
    pu4Key = (UINT32*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu4Key, 8);
    COPY_FROM_USER_ARG_SIZE(pu4Key, au4Key, 8);

    if(!DMX_SetMulti2Key(u1Index, fgEven, au4Key))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDMX_SetMulti2CbcIV
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetMulti2CbcIV(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4IvHi;
    UINT32 u4IvLo;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4IvHi = (UINT32)rArg.ai4Arg[0];
    u4IvLo = (UINT32)rArg.ai4Arg[1];

    DMX_SetMulti2CbcIV(u4IvHi, u4IvLo);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDMX_SetMulti2OfbIV
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetMulti2OfbIV(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4IvHi;
    UINT32 u4IvLo;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4IvHi = (UINT32)rArg.ai4Arg[0];
    u4IvLo = (UINT32)rArg.ai4Arg[1];

    DMX_SetMulti2OfbIV(u4IvHi, u4IvLo);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDMX_SetMulti2SysKey
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetMulti2SysKey(unsigned long arg)
{
    INT32 Arg;
    UINT32 *pu4Key;
    UINT32 au4Key[8];

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    pu4Key = (UINT32*)Arg;

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu4Key, 32);
    COPY_FROM_USER_ARG_SIZE(pu4Key, au4Key, 32);

    DMX_SetMulti2SysKey(au4Key);

    return MTR_OK;
}

#else
//-----------------------------------------------------------------------------
/** _MTDMX_SetMulti2Config
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetMulti2Config(unsigned long arg)
{
    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDMX_SetMulti2Key
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetMulti2Key(unsigned long arg)
{
    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDMX_SetMulti2CbcIV
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetMulti2CbcIV(unsigned long arg)
{
    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDMX_SetMulti2OfbIV
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetMulti2OfbIV(unsigned long arg)
{
    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDMX_SetMulti2SysKey
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetMulti2SysKey(unsigned long arg)
{
    return MTR_OK;
}
#endif  // CC_IOMTDMX_MULTI2


static MT_RESULT_T _MTDRM_WVSetAssetKey(unsigned long arg)
{
#ifdef CC_TRUSTZONE_SUPPORT
#ifndef CC_MT5392B
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 *pu1Key;
    UINT8 au1Key[16];
    UINT32 u4KeyLen;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu1Key = (UINT8*)rArg.ai4Arg[0];
    u4KeyLen = (UINT32)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu1Key, 16);
    COPY_FROM_USER_ARG_SIZE(pu1Key, au1Key, 16);

    if(!TZ_DRM_WV_SetAssetKey(au1Key, u4KeyLen))
    {
        return MTR_NOT_OK;
    }
#endif
#endif
    return MTR_OK;
}


static MT_RESULT_T _MTDRM_WVDeriveCW(unsigned long arg)
{
#ifdef CC_TRUSTZONE_SUPPORT
#ifndef CC_MT5392B
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 *pu1Ecm;
    UINT32 *pu4Flags;
    UINT8 au1Ecm[32];
    UINT32 u4Flags;
    DMX_MM_DRM_AES_T rDRM;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu1Ecm = (UINT8 *)rArg.ai4Arg[0];
    pu4Flags = (UINT32 *)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu1Ecm, 32);
    COPY_FROM_USER_ARG_SIZE(pu1Ecm, au1Ecm, 32);

    if(!TZ_DRM_WV_DeriveCW(au1Ecm, &u4Flags))
    {
        return MTR_NOT_OK;
    }

    Printf("ECM in io_mtdmx: 0x%x\n", u4Flags);

    x_memcpy(rDRM.au1Key, au1Ecm + 4, 16);
    rDRM.u4KeyBitLen = 128;
    DMX_AES_SetDecryptInfoEx(2, DMX_MM_DRM_FLAG_KEY, &rDRM);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Flags, UINT32);
    COPY_TO_USER_ARG(pu4Flags, u4Flags, UINT32);
#endif
#endif
    return MTR_OK;
}


static MT_RESULT_T _MTDRM_WVPassKeybox(unsigned long arg)
{
#ifdef CC_TRUSTZONE_SUPPORT
#ifndef CC_MT5392B
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 *pu1Keybox;
    UINT8 au1Keybox[256];
    UINT32 u4Size;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu1Keybox = (UINT8*)rArg.ai4Arg[0];
    u4Size = (UINT32)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu1Keybox, 256);
    COPY_FROM_USER_ARG_SIZE(pu1Keybox, au1Keybox, 256);

    if(!TZ_DRM_WV_PassKeybox(au1Keybox, u4Size))
    {
        return MTR_NOT_OK;
    }
#endif
#endif

    return MTR_OK;
}

#define DEVICE_DATA_LEN                    (128)
#define DM_KEY_LEN                         (16)
#define ULPK_LEN                           (32)


static MT_RESULT_T _MTDRM_BimGetDeviceData(unsigned long arg)
{
#ifndef CC_MT5392B
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 *pu1Buf;
    UINT32 u4Size;
    UINT8 au1Data[DEVICE_DATA_LEN];

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu1Buf = (UINT8 *)rArg.ai4Arg[0];
    u4Size = (UINT32)rArg.ai4Arg[1];

    HalFlushInvalidateDCacheMultipleLine((UINT32)au1Data, DEVICE_DATA_LEN);
    
    if(BIM_GetDeviceData(au1Data, DEVICE_DATA_LEN) != 0) 
    {
        return MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu1Buf, DEVICE_DATA_LEN);
    COPY_TO_USER_ARG_SIZE(pu1Buf, au1Data, DEVICE_DATA_LEN);
#endif
    return MTR_OK;
}

static MT_RESULT_T _MTDRM_BimGetDmKey(unsigned long arg)
{
#ifndef CC_MT5392B
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 *pu1Buf;
    UINT32 u4Size;
    UINT8 au1Data[DM_KEY_LEN];

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu1Buf = (UINT8 *)rArg.ai4Arg[0];
    u4Size = (UINT32)rArg.ai4Arg[1];

    HalFlushInvalidateDCacheMultipleLine((UINT32)au1Data, DM_KEY_LEN);
    
    if(BIM_GetDmKey(au1Data, DM_KEY_LEN) != 0) 
    {
        return MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu1Buf, DM_KEY_LEN);
    COPY_TO_USER_ARG_SIZE(pu1Buf, au1Data, DM_KEY_LEN);
#endif
    return MTR_OK;
}

static MT_RESULT_T _MTDRM_BimGetULPK(unsigned long arg)
{
#ifndef CC_MT5392B
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 *pu1Buf;
    UINT32 u4Size;
    UINT8 au1Data[ULPK_LEN];

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu1Buf = (UINT8 *)rArg.ai4Arg[0];
    u4Size = (UINT32)rArg.ai4Arg[1];

    HalFlushInvalidateDCacheMultipleLine((UINT32)au1Data, ULPK_LEN);
    
    if(BIM_GetULPK(au1Data, ULPK_LEN) != 0) 
    {
        return MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(pu1Buf, ULPK_LEN);
    COPY_TO_USER_ARG_SIZE(pu1Buf, au1Data, ULPK_LEN);
#endif
    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** SetPidxInstance
 *
 *  @param  u4KeyLen        The key length
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDMX_SetPidxInstance(unsigned long arg)
{
    UINT8 u1Instance;	
	MTAL_IOCTL_2ARG_T	 rArg;
	UINT8 u1Pidx ;
	MTDMX_PID_INSRC_T eType;

	USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
	COPY_FROM_USER_2ARG(arg, rArg);

	u1Pidx = (UINT8)rArg.ai4Arg[0];
	eType = (MTDMX_PID_INSRC_T)rArg.ai4Arg[1];
	
	if(eType == MTDMX_PID_INSRC_DTV)
	{
		u1Instance = DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_TUNER_0);
		if(!DMX_MUL_SetTSIdx(u1Instance))
		{
		    return MTR_NOT_OK;
		}
	}
	else if((eType == MTDMX_PID_INSRC_TIMESHIFT_PLAY) || (eType == MTDMX_PID_INSRC_MM))
	{
		u1Instance = DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_BUFAGENT_0);
	}
	else
	{
		return MTR_NOT_OK;
	}


    if(!DMX_MUL_SetPidxInstance(u1Instance,(UINT8)u1Pidx))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

static MT_RESULT_T _MTDMX_FreePidx(unsigned long arg)
{
    INT32 Arg;
    UINT32 u1Pidx;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, INT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u1Pidx = (UINT32)Arg;

    MTDMX_PRINT("u1Pidx %d\n",(int)u1Pidx);

    if(!DMX_MUL_FreePidx((UINT8)u1Pidx))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

#ifdef CC_ENABLE_SKB
extern MT_RESULT_T MTDMX_MWIF_Call(unsigned long arg);
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int mtal_ioctl_mtdmx(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    INT32 i4Ret = MTR_OK;

#ifdef TIME_SHIFT_SUPPORT
    if((i4Ret = mtal_ioctl_mtdrec(cmd, arg)) != MTR_NOT_SUPPORTED)
    {
        return i4Ret;
    }
#endif

    switch (cmd)
    {
    case MTAL_IO_DMX_INIT:
        i4Ret = _MTDMX_Init(arg);
        break;

    case MTAL_IO_DMX_RESET:
        i4Ret = _MTDMX_Reset(arg);
        break;

    case MTAL_IO_DMX_SETFRONTEND:
        i4Ret = _MTDMX_SetFrontEnd(arg);
        break;

    case MTAL_IO_DMX_SETFRONTENDEX:
        i4Ret = _MTDMX_SetFrontEndEx(arg);
        break;

    case MTAL_IO_DMX_SETVIDEOTYPE:
        i4Ret = _MTDMX_SetVideoType(arg);
        break;

    case MTAL_IO_DMX_START:
        i4Ret = _MTDMX_Start(arg);
        break;

    case MTAL_IO_DMX_STOP:
        i4Ret = _MTDMX_Stop(arg);
        break;

    case MTAL_IO_DMX_SETPID:
        i4Ret = _MTDMX_SetPid(arg);
        break;

    case MTAL_IO_DMX_GETPID:
        i4Ret = _MTDMX_GetPid(arg);
        break;

    case MTAL_IO_DMX_FREEPID:
        i4Ret = _MTDMX_FreePid(arg);
        break;

    case MTAL_IO_DMX_GETPIDIDX:
        i4Ret = _MTDMX_GetPidIndex(arg);
        break;

    case MTAL_IO_DMX_SETVDEPID:
        i4Ret = _MTDMX_SetVideoPid(arg);
        break;

    case MTAL_IO_DMX_SETAUDPID:
        i4Ret = _MTDMX_SetAudioPid(arg);
        break;

    case MTAL_IO_DMX_SETAUDDESPID:
        i4Ret = _MTDMX_SetAudioDescPid(arg);
        break;

    case MTAL_IO_DMX_SETPCRPID:
        i4Ret = _MTDMX_SetPcrPid(arg);
        break;

    case MTAL_IO_DMX_SETPCRPIDEX:
        i4Ret = _MTDMX_SetPcrPidEx(arg);
        break;

    case MTAL_IO_DMX_SETPCR:
        i4Ret = _MTDMX_SetPcr(arg);
        break;

    case MTAL_IO_DMX_SETFTR:
        i4Ret = _MTDMX_SetFilter(arg);
        break;

    case MTAL_IO_DMX_GETFTR:
        i4Ret = _MTDMX_GetFilter(arg);
        break;

    case MTAL_IO_DMX_PSI_ALCBFR:
        i4Ret = _MTDMX_PSI_AllocateBuffer(arg);
        break;

    case MTAL_IO_DMX_PSI_FREEBFR:
        i4Ret = _MTDMX_PSI_FreeBuffer(arg);
        break;

    case MTAL_IO_DMX_PSI_GETBFR:
        i4Ret = _MTDMX_PSI_GetBuffer(arg);
        break;

    case MTAL_IO_DMX_PSI_ULKBFR:
        i4Ret = _MTDMX_PSI_UnlockBuffer(arg);
        break;

    case MTAL_IO_DMX_PSI_FLSBFR:
        i4Ret = _MTDMX_PSI_FlushBuffer(arg);
        break;

    case MTAL_IO_DMX_PSI_GETBFRSECHEAD:
        i4Ret = _MTDMX_PSI_GetBufferSectionHeader(arg);
        break;

    case MTAL_IO_DMX_PES_ALKBFR:
        i4Ret = _MTDMX_PES_AllocateBuffer(arg);
        break;

    case MTAL_IO_DMX_PES_FREEBFR:
        i4Ret = _MTDMX_PES_FreeBuffer(arg);
        break;

    case MTAL_IO_DMX_PES_ULKBFR:
        i4Ret = _MTDMX_PES_UnlockBuffer(arg);
        break;

    case MTAL_IO_DMX_PES_FLSBFR:
        i4Ret = _MTDMX_PES_FlushBuffer(arg);
        break;

    case MTAL_IO_DMX_PES_GETBFR:
        i4Ret = _MTDMX_PES_GetBuffer(arg);
        break;

    case MTAL_IO_DMX_GETPIDCNT:
        i4Ret = _MTDMX_GetPidCounters(arg);
        break;

    case MTAL_IO_DMX_RSTPIDCNT:
        i4Ret = _MTDMX_ResetPidCounters(arg);
        break;

    case MTAL_IO_DMX_GETLOKSTE:
        i4Ret = _MTDMX_GetLockState(arg);
        break;

    case MTAL_IO_DMX_GETSCRMSTE:
        i4Ret = _MTDMX_GetScrambleState(arg);
        break;

    case MTAL_IO_DMX_GETCURSTC:
        i4Ret = _MTDMX_GetCurSTC(arg);
        break;

    case MTAL_IO_DMX_SETGENERICFILTER:
        i4Ret = _MTDMX_SetGenericFilter(arg);
        break;

    case MTAL_IO_DMX_GETGENERICFILTER:
        i4Ret = _MTDMX_GetGenFilter(arg);
        break;

    case MTAL_IO_DMX_SETDESKEY:
        i4Ret = _MTDMX_SetDesKey(arg);
        break;

    case MTAL_IO_DMX_SETDESIV:
        i4Ret = _MTDMX_SetDesIV(arg);
        break;

    case MTAL_IO_DMX_SETDESEVENODDIV:
        i4Ret = _MTDMX_SetDesOddEvenIV(arg);
        break;

    case MTAL_IO_DMX_SETDESRTBMODE:
        i4Ret = _MTDMX_SetDesRtbMode(arg);
        break;

    case MTAL_IO_DMX_SETAESKEYLEN:
        i4Ret = _MTDMX_SetAesKeyLen(arg);
        break;

    case MTAL_IO_DMX_SETAESKEY:
        i4Ret = _MTDMX_SetAesKey(arg);
        break;

    case MTAL_IO_DMX_SETAESIV:
        i4Ret = _MTDMX_SetAesIV(arg);
        break;

    case MTAL_IO_DMX_SETAESEVENODDIV:
        i4Ret = _MTDMX_SetAesOddEvenIV(arg);
        break;

    case MTAL_IO_DMX_SETDESCPID:
        i4Ret = _MTDMX_SetDescramblPid(arg);
        break;
	//DVB key and IV
	case MTAL_IO_DMX_DVB_KEY:
        i4Ret = _MTDMX_SetDVBKey(arg);
        break;

    case MTAL_IO_DMX_SETFILEDESCPID:
        i4Ret = _MTDMX_SetFileDescramblPid(arg);
        break;
	case MTAL_IO_DMX_DVB_IV:
        i4Ret = _MTDMX_SetDVBIV(arg);
        break;

#ifdef ENABLE_MULTIMEDIA
    case MTAL_IO_DMX_AES_SETDCRINFO:
        i4Ret = _MTDMX_AES_SetDecryptInfo(arg);
        break;
#endif

    case MTAL_IO_DMX_SETAESRTBMODE:
        i4Ret = _MTDMX_SetAesRtbMode(arg);
        break;

    case MTAL_IO_DMX_SETAESON:
        i4Ret = _MTDMX_SetAesOn(arg);
        break;

#ifdef ENABLE_MULTIMEDIA
    case MTAL_IO_DMX_AES_MVBLKDATA:
        i4Ret = _MTDMX_AES_MoveBlockData(arg);
        break;

   case MTAL_IO_DMX_AES_ALLOCENGINE:
        i4Ret = _MTDMX_AES_AllocEngine(arg);
        break;

    case MTAL_IO_DMX_AES_FREEENGINE:
        i4Ret = _MTDMX_AES_FreeEngine(arg);
        break;

    case MTAL_IO_DMX_AES_SETDCRINFOEX:
        i4Ret = _MTDMX_AES_SetDecryptInfoEx(arg);
        break;

    case MTAL_IO_DMX_AES_MVBLKDATAEX:
        i4Ret = _MTDMX_AES_MoveBlockDataEx(arg);
        break;
#endif

    case MTAL_IO_DMX_GCPU_CMD:
        i4Ret = _MTDMX_GCPU_Cmd(arg);
        break;

    case MTAL_IO_DMX_SETPID_INSRC:
        i4Ret = _MTDMX_SetPidInputSource(arg);
        break;

    case MTAL_IO_DMX_MULTI2_CFG:
        i4Ret = _MTDMX_SetMulti2Config(arg);
        break;

    case MTAL_IO_DMX_MULTI2_KEY:
        i4Ret = _MTDMX_SetMulti2Key(arg);
        break;

    case MTAL_IO_DMX_MULTI2_CBCIV:
        i4Ret = _MTDMX_SetMulti2CbcIV(arg);
        break;

    case MTAL_IO_DMX_MULTI2_OFBIV:
        i4Ret = _MTDMX_SetMulti2OfbIV(arg);
        break;

    case MTAL_IO_DMX_MULTI2_SYSKEY:
        i4Ret = _MTDMX_SetMulti2SysKey(arg);
        break;

   case MTAL_IO_DRM_WV_SET_ASSET_KEY:
        i4Ret = _MTDRM_WVSetAssetKey(arg);
        break;

    case MTAL_IO_DRM_WV_DERIVE_CW:
        i4Ret = _MTDRM_WVDeriveCW(arg);
        break;

    case MTAL_IO_DRM_WV_PASS_KEYBOX:
        i4Ret = _MTDRM_WVPassKeybox(arg);
        break;

    case MTAL_IO_BIM_GET_DEVICE_DATA:
        i4Ret = _MTDRM_BimGetDeviceData(arg);
        break;

    case MTAL_IO_BIM_DM_GET_KEY:
        i4Ret = _MTDRM_BimGetDmKey(arg);
        break;

    case MTAL_IO_BIM_GET_ULPK:
        i4Ret = _MTDRM_BimGetULPK(arg);
        break;

    case MTAL_IO_DMX_SET_PIDX_INSTANCE:
        i4Ret = _MTDMX_SetPidxInstance(arg);
        break;

    case MTAL_IO_DMX_FREE_PIDX:
        i4Ret = _MTDMX_FreePidx(arg);
        break;

#ifdef CC_ENABLE_SKB
    case MTAL_IO_DMX_MWIF_CALL:
        i4Ret = MTDMX_MWIF_Call(arg);
        break;
#endif
#ifdef TIME_SHIFT_SUPPORT
    default:
        i4Ret = mtal_ioctl_mttsrec(cmd, arg);
        break;
#endif

    }

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** MTDMX_SetPCR
 */
//-----------------------------------------------------------------------------
BOOL MTDMX_SetPCR(UINT8 u1Pidx, UINT16 u2PidNum, MTDMX_PID_INSRC_T eInType, BOOL fgEnable)
{
    DMX_PID_T rPid;

    x_memset((VOID*)&rPid, 0, sizeof(DMX_PID_T));
    rPid.fgAllocateBuffer = FALSE;
    rPid.u2Pid = u2PidNum;
    //rPid.u4BufAddr = 0x0;
    //rPid.u4BufSize = 0x0;
    rPid.ePidType = DMX_PID_TYPE_NONE;
    rPid.fgPrimary = FALSE;
    rPid.ePcrMode = DMX_PCR_MODE_NEW;
    rPid.eDescMode = DMX_DESC_MODE_NONE;

    rPid.fgEnable = fgEnable;

    if(eInType == MTDMX_PID_INSRC_DTV)
    {
        if(!_MtdmxSetPid(u1Pidx, DMX_PID_FLAG_ALL & (~DMX_PID_FLAG_BUFFER), &rPid))
        {
            return FALSE;
        }
    }
    else if(eInType == MTDMX_PID_INSRC_TIMESHIFT_PLAY)
    {
        rPid.u1TsIndex = 2;
        if(!DMX_SetPid(u1Pidx, DMX_PID_FLAG_ALL & (~DMX_PID_FLAG_BUFFER), &rPid))
        {
            return FALSE;
        }
    }
    else if(eInType == MTDMX_PID_INSRC_MM)
    {
        rPid.u1TsIndex = 0;
        if(!DMX_SetPid(u1Pidx, DMX_PID_FLAG_ALL & (~DMX_PID_FLAG_BUFFER), &rPid))
        {
            return FALSE;
        }
    }
    else
    {
        return  FALSE;
    }

    if(fgEnable)
    {
        if(DMX_SetStcSrc(u1Pidx, 0) != 0)    // Force use STC 0
        {
            return MTR_NOT_OK;
        }
    }
    else
    {
        if(DMX_SetStcSrc(u1Pidx, STC_SRC_NS) != 0)    // Set to other PCR to disable
        {
            return MTR_NOT_OK;
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** MTDMX_SetSecondVideoPid
 */
//-----------------------------------------------------------------------------
BOOL MTDMX_SetSecondVideoPid(UINT8 u1Pidx, UINT32 u4Flags, MTDMX_PID_T* prPid)
{
    MTDMX_PID_T rPid;
    DMX_PID_T rDrvPid;
    FBM_POOL_T* prFbmPool;

    rPid = *prPid;

    // Only enable/disable PID
    if(u4Flags == MTDMX_PID_FLAG_VALID)
    {
        rDrvPid.fgEnable = rPid.fgEnable;
        if(!_MtdmxSetPid(u1Pidx, u4Flags, &rDrvPid))
        {
            return MTR_NOT_OK;
        }

        return MTR_OK;
    }

    // Get from MW
    rDrvPid.fgEnable = rPid.fgEnable;
    rDrvPid.fgAllocateBuffer = rPid.fgAllocateBuffer;
    rDrvPid.u2Pid = rPid.u2Pid;
    rDrvPid.u4BufAddr = rPid.u4BufAddr;
    rDrvPid.u4BufSize = rPid.u4BufSize;
    rDrvPid.u1DeviceId = 0;
    rDrvPid.ePidType = rPid.ePidType;

    if(rPid.ePidType == MTDMX_PID_TYPE_ES_VIDEO)
    {
        // Allocate video buffer from FBM
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0);

        // Setup buffer attributes
        rDrvPid.fgAllocateBuffer = FALSE;
        rDrvPid.u4BufAddr = prFbmPool->u4Addr + (prFbmPool->u4Size / 2);
        rDrvPid.u4BufSize = prFbmPool->u4Size / 2;
        rDrvPid.u1DeviceId = rPid.u1DeviceId;
    }

    if((rPid.pfnNotify != NULL) && (u4Flags & MTDMX_PID_FLAG_CALLBACK))
    {
        rDrvPid.pfnNotify = _MtDmxCallback;
    }
    else
    {
        rDrvPid.pfnNotify = NULL;
    }

    if((rPid.pfnScramble != NULL) && (u4Flags & DMX_PID_FLAG_SCRAMBLE_STATE))
    {
        rDrvPid.pfnScramble = _MtDmxCallback;
    }
    else
    {
        rDrvPid.pfnScramble = NULL;
    }

    rDrvPid.pvNotifyTag = NULL;
    rDrvPid.pvScrambleTag = NULL;

    // Set by driver only
    rDrvPid.fgPrimary = TRUE;
    rDrvPid.u1TsIndex = 0;
    rDrvPid.u1KeyIndex = 0x0;
    rDrvPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rDrvPid.ePcrMode = DMX_PCR_MODE_NONE;
    rDrvPid.eDescMode = DMX_DESC_MODE_NONE;
    rDrvPid.u1ChannelId = 0x1;

    u4Flags |= (DMX_PID_FLAG_PRIMARY | DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_KEY_INDEX |
                DMX_PID_FLAG_DEVICE_ID | DMX_PID_FLAG_STEER | DMX_PID_FLAG_PCR |
                DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_SCRAMBLE_STATE);

    if(!_MtdmxSetPid(u1Pidx, u4Flags, &rDrvPid))
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


