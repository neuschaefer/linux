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

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifndef CC_MT5360
#include "x_linux.h"
#include "SdFeeder.h"
#include "feeder_if.h"
#include "playmgr_if.h"
#include <linux/sdal_ioctl.h>


//#define IO_FEEDER_DEBUG

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** Initialize SdFeeder module
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_Open(unsigned long arg)                      
{
    BOOL fgRet;
    INT32 i4Ret;
    FeederSourceType eMMType;
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(UINT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_Start : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }
    eMMType = (FeederSourceType)arg;
    if (eMMType == FEEDER_AUDIO_SOURCE)
    {
        i4Ret = FeederOpen(FEEDER_AUDIO_SOURCE);
    }
    else
    {
        if (PLAYMGR_Init())
        {
            return SD_OK;
        }
        else
        {
            return SD_NOT_OK;
        }
    }
    return ((i4Ret == FEEDER_E_OK) ? SD_OK : SD_NOT_OK);
}

//-----------------------------------------------------------------------------
/** Release SdFeeder module
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_Close(unsigned long arg)
{
    INT32 i4Ret;
    FeederSourceType eMMType;
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(UINT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_Start : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    eMMType = (FeederSourceType)arg;
    if (eMMType == FEEDER_AUDIO_SOURCE)
    {
        i4Ret = FeederClose(FEEDER_AUDIO_SOURCE);
    }
    else
    {
        if (PLAYMGR_Close())
        {
            return SD_OK;
        }
        else
        {
            return SD_NOT_OK;
        }
    }
    return ((i4Ret == FEEDER_E_OK) ? SD_OK : SD_NOT_OK);
}

//-----------------------------------------------------------------------------
/** Start SdFeeder module
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_Start(unsigned long arg)
{
    INT32  i4Ret;
    FeederSourceType eMMType;
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(UINT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_Start : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    eMMType = (FeederSourceType)arg;
    if (eMMType == FEEDER_AUDIO_SOURCE)
    {
        FeederSetAppQueryMode(FEEDER_AUDIO_SOURCE, FEEDER_PUSH_MODE);
        i4Ret = FeederStart(FEEDER_AUDIO_SOURCE);
        FeederSetRing(FEEDER_AUDIO_SOURCE, TRUE);
    }
    else
    {
        if (PLAYMGR_Start())
        {
            return SD_OK;
        }
        else
        {
            return SD_NOT_OK;
        }
    }
    return ((i4Ret == FEEDER_E_OK) ? SD_OK : SD_NOT_OK);
}

//-----------------------------------------------------------------------------
/** Stop SdFeeder module
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_Stop(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    FeederSourceType eDataType;
    BOOL   bFlush;
    INT32  i4Ret;
    
    // Check access of user space
    if (copy_from_user(&rArg2, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_Stop : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    eDataType = (FeederSourceType)rArg2.au4Arg[0];
    bFlush = (BOOL)rArg2.au4Arg[1];

#ifdef IO_FEEDER_DEBUG
    printk("_SdFeeder_Stop : Feeder stops\n");
#endif  // IO_FEEDER_DEBUG

    if (eDataType == FEEDER_AUDIO_SOURCE)
    {
    // Driver needs to wait for consuming all data if bFlush is true.
    // Otherwise you can stop audio path immediately
    if (bFlush)
    {
        // Wait the empty buffer
        FeederWaitBufferEmpty(eDataType);
    }
        i4Ret = FeederStop(FEEDER_AUDIO_SOURCE);
    }
    else
    {
        if (PLAYMGR_Stop())
        {
            return SD_OK;
        }
        else
        {
            return SD_NOT_OK;
        }
    }
            
    return ((i4Ret == FEEDER_E_OK) ? SD_OK : SD_NOT_OK);
}

//-----------------------------------------------------------------------------
/** Pause SdFeeder module
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_Pause(unsigned long arg)
{
    INT32  i4Ret;
    FeederSourceType eMMType;
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(UINT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_Start : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    eMMType = (FeederSourceType)arg;
    if (eMMType == FEEDER_AUDIO_SOURCE)
    {
        i4Ret = FeederPause(FEEDER_AUDIO_SOURCE);
    }
    else
    {
        if (PLAYMGR_Pause())
        {
            return SD_OK;
        }
        else
        {
            return SD_NOT_OK;
        }
    }
    
    return ((i4Ret == FEEDER_E_OK) ? SD_OK : SD_NOT_OK);
}

//-----------------------------------------------------------------------------
/** Resume SdFeeder module
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_Resume(unsigned long arg)
{
    INT32  i4Ret;
    FeederSourceType eMMType;
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(UINT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_Start : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    eMMType = (FeederSourceType)arg;
    if (eMMType == FEEDER_AUDIO_SOURCE)
    {
        i4Ret = FeederResume(FEEDER_AUDIO_SOURCE);
    }
    else
    {
        if (PLAYMGR_Resume())
        {
            return SD_OK;
        }
        else
        {
            return SD_NOT_OK;
        }
    }
    return ((i4Ret == FEEDER_E_OK) ? SD_OK : SD_NOT_OK);
}

//-----------------------------------------------------------------------------
/** Application transmits data through the buffer pointer
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_SubmitData(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    FEEDER_BUF_INFO_T rBufferInfo;
    FeederSourceType  eDataType;
    UINT32 u4Size;
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_SubmitData : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_SubmitData : copy argument from user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    eDataType  = (FeederSourceType)rArg2.au4Arg[0];
    u4Size     = (UINT32)rArg2.au4Arg[1];

    // Get the buffer information
    if (FeederGetBufferInfo(eDataType, &rBufferInfo) != FEEDER_E_OK)
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_SubmitData : get buufer information error\n");
#endif  // IO_FEEDER_DEBUG

        return (SD_NOT_OK);
    }

    if (rBufferInfo.u4FreeSize < u4Size)
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_SubmitData : no enough buffer space !\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_NOT_AVAIL);
    }
    
    if (FeederUpdateWritePointer(eDataType, u4Size) != FEEDER_E_OK)
    {
#ifdef IO_FEEDER_DEBUG
        printk("FeederUpdateWritePointer : update copy size %d error\n",
               u4Size);
#endif  // IO_FEEDER_DEBUG

        return (SD_NOT_OK);
    }
        
    return SD_OK;
}

//-----------------------------------------------------------------------------
/** Get information of the Feeder's local buffer
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_GetBuffer(unsigned long arg)
{
    SDAL_IOCTL_4ARG_T rArg4;
    FeederSourceType eDataType;
    FEEDER_BUF_INFO_T rBuffInfo;
    UINT32 u4StartAddr, u4SizeAddr;
    UINT32 u4BufferAddr, u4BufferSize;
    INT32  i4Ret;
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(SDAL_IOCTL_4ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetBuffer : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    // Copy user space argument
    if (copy_from_user(&rArg4, (void __user *)arg, sizeof(SDAL_IOCTL_4ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetBuffer : copy argument from user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    eDataType   = (FeederSourceType)rArg4.au4Arg[0];
    u4StartAddr = rArg4.au4Arg[1];
    u4SizeAddr  = rArg4.au4Arg[2];
    
    i4Ret = FeederGetBufferInfo(eDataType, &rBuffInfo);
    if (i4Ret != FEEDER_E_OK)
    {
        return SD_NOT_OK;
    }

    u4BufferAddr = rBuffInfo.u4WriteAddr;
    u4BufferSize = rBuffInfo.u4FreeSize;

    // Set the available buffer address
    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)u4StartAddr, sizeof(INT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetBuffer : u4StartAddr argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }
    
    if (copy_to_user((void __user *)u4StartAddr, &u4BufferSize, sizeof(INT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetBuffer : copy argument to user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }
    
    // Set the available buffer size
    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)u4SizeAddr, sizeof(UINT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetBuffer : u4SizeAddr argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }
    
    if (copy_to_user((void __user *)u4SizeAddr, &u4BufferSize, sizeof(UINT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetBuffer : copy argument to user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }
    
    return (SD_OK);
}

//-----------------------------------------------------------------------------
/** Get the local buffer information of SdFeeder module
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_GetBufferInfo(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    FeederSourceType eDataType;
    FEEDER_BUF_INFO_T rBuffInfo;
    UINT32 u4InfoAddr;
    INT32  i4Ret;
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetBufferInfo : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetBufferInfo : copy argument from user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    eDataType  = (FeederSourceType)rArg2.au4Arg[0];
    u4InfoAddr = rArg2.au4Arg[1];
    
    x_memset(&rBuffInfo, 0, sizeof(FEEDER_BUF_INFO_T));
    i4Ret = FeederGetBufferInfo(eDataType, &rBuffInfo);
    if (i4Ret == FEEDER_E_OK)
    {
        // Check access of user space
        if (!access_ok(VERIFY_WRITE, (void __user *)u4InfoAddr,
                       sizeof(FEEDER_BUF_INFO_T)))
        {
#ifdef IO_FEEDER_DEBUG
            printk("_SdFeeder_GetBufferInfo : pi4Size argument error\n");
#endif  // IO_FEEDER_DEBUG
            return (SD_ERR_SYS);
        }
    
        if (copy_to_user((void __user *)u4InfoAddr, &rBuffInfo,
                         sizeof(FEEDER_BUF_INFO_T)))
        {
#ifdef IO_FEEDER_DEBUG
            printk("_SdFeeder_GetBufferInfo : copy argument to user error\n");
#endif  // IO_FEEDER_DEBUG
            return (SD_ERR_SYS);
        }
    }
    
    return ((i4Ret == FEEDER_E_OK) ? SD_OK : SD_NOT_OK);
}

//-----------------------------------------------------------------------------
/** Get the status of the Feeder module
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_GetStatus(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    FeederSourceType eDataType;
    SdFeeder_Status_t rCurStatus;
    PLAYMGR_STATUS_T rPlayInfo;
    FEEDER_DECODE_STATUS rStatus;
    UINT32 *pu4State;
    INT32  i4Ret;
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetStatus : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetStatus : copy argument from user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    eDataType = (FeederSourceType)rArg2.au4Arg[0];
    pu4State = (UINT32 *)rArg2.au4Arg[1];

    x_memset(&rCurStatus, 0, sizeof(SdFeeder_Status_t));
#if 0    
    i4Ret = FeederGetDecodeStatus(eDataType, &rStatus);
    if (i4Ret != FEEDER_E_OK)
    {
        return (SD_NOT_OK);
    }
#endif   

    if (!PLAYMGR_GetInfo(&rPlayInfo))
    {
            return (SD_NOT_OK);
    }
    
    rCurStatus.position = rPlayInfo.u4CurPos;
    rCurStatus.bSeek = (rPlayInfo.u4PlayCap & PLAYMGR_CAP_SEEK)? TRUE : FALSE;
    rCurStatus.bTrick = (rPlayInfo.u4PlayCap & PLAYMGR_CAP_TRICKMODE)? TRUE : FALSE;
    
       
    // Set the state
    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pu4State,
                   sizeof(SdFeeder_Status_t)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetStatus : pu4Size argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }
    
    if (copy_to_user((void __user *)pu4State, &rCurStatus,
                     sizeof(SdFeeder_Status_t)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetStatus : copy argument to user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }
    
    return (SD_OK);
}


//-----------------------------------------------------------------------------
/** convert PLAYMGR_STATUS_T to SdFeeder_Settings_t
 *
 *  @param prPlay           input structure
 *  @param prFeederSetting  output structure
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_GetPlaymgrInfo(PLAYMGR_STATUS_T *prPlay, 
                                             SdFeeder_Settings_t *prFeederSetting)
{
    if((prFeederSetting == NULL) || (prPlay == NULL))
    {
        return SD_NOT_OK;
    }

    // FixMe: Must doing memset to clear data.
    // However. some stage will be fail.
    // ex. if call _SdFeeder_Start arg will be = FEEDER_AUDIO_SOURCE.
    //     But it should be FEEDER_VIDEO_SOURCE.
    
    //x_memset(prFeederSetting, 0, sizeof(SdFeeder_Settings_t));
        
    switch (prPlay->eMediaType)
    {        
    case PLAYMGR_MEDIA_TYPE_AVI:
        prFeederSetting->eContainterType = SD_CONTAINER_AVI;
        break;
    case PLAYMGR_MEDIA_TYPE_DIVX:
        prFeederSetting->eContainterType = SD_CONTAINER_DIVX;
        break;
    case PLAYMGR_MEDIA_TYPE_MPEG2_PS:
        prFeederSetting->eContainterType = SD_CONTAINER_PS;
        break;
    case PLAYMGR_MEDIA_TYPE_MPEG2_TS:
        prFeederSetting->eContainterType = SD_CONTAINER_TS;
        break;
    case PLAYMGR_MEDIA_TYPE_VC1_ES:
        prFeederSetting->eContainterType = SD_CONTAINER_AVI;
        break;
    case PLAYMGR_MEDIA_TYPE_VC1_WMV:
        prFeederSetting->eContainterType = SD_CONTAINER_ASF;
        break;
    case PLAYMGR_MEDIA_TYPE_VCD:
        prFeederSetting->eContainterType = SD_CONTAINER_VCD;
        break;
    case PLAYMGR_MEDIA_TYPE_VOB:
        prFeederSetting->eContainterType = SD_CONTAINER_VOB;
        break;
    case PLAYMGR_MEDIA_TYPE_VRO:
        prFeederSetting->eContainterType = SD_CONTAINER_VRO;
        break;
    case PLAYMGR_MEDIA_TYPE_MP3:
        prFeederSetting->eContainterType = SD_CONTAINER_MP3;
        break;
    case PLAYMGR_MEDIA_TYPE_MPA:
        prFeederSetting->eContainterType = SD_CONTAINER_AVI;
        break;
    default:
        prFeederSetting->eContainterType = SD_CONTAINER_NONE;
        break;
    }

    switch (prPlay->eVidFormat)
    {
    case PLAYMGR_VDEC_MPEG1_MPEG2:
        prFeederSetting->eVidFormat = SD_VIDEO_FORMAT_MPEG2;
        break;
    case PLAYMGR_VDEC_MPEG4:    
        prFeederSetting->eVidFormat = SD_VIDEO_FORMAT_MPEG4;
        break;
    case PLAYMGR_VDEC_H264:
        prFeederSetting->eVidFormat = SD_VIDEO_FORMAT_H264;
        break;
    case PLAYMGR_VDEC_VC1:
        prFeederSetting->eVidFormat = SD_VIDEO_FORMAT_WMV3;
        break;
    case PLAYMGR_VDEC_DIVX311:
        prFeederSetting->eVidFormat = SD_VIDEO_FORMAT_DIVX;
        break;
    default:   
        prFeederSetting->eVidFormat = SD_VIDEO_FORMAT_NONE;
        prFeederSetting->video.streamId = 0;
        break;
    }

    switch (prPlay->eAudFormat)
    {        
    case PLAYMGR_ADEC_MPEG:
        prFeederSetting->eAudFormat = SD_AUDIO_FORMAT_MPEG1;
        break;
    case PLAYMGR_ADEC_AC3: 
        prFeederSetting->eAudFormat = SD_AUDIO_FORMAT_AC3;
        break;
    case PLAYMGR_ADEC_PCM: 
        prFeederSetting->eAudFormat = SD_AUDIO_FORMAT_PCM;
        break;
    case PLAYMGR_ADEC_MP3:    
        prFeederSetting->eAudFormat = SD_AUDIO_FORMAT_MPEG1_L3;
        break;
    case PLAYMGR_ADEC_AAC:  
        prFeederSetting->eAudFormat = SD_AUDIO_FORMAT_AAC;
        break;
    case PLAYMGR_ADEC_DTS: 
        prFeederSetting->eAudFormat = SD_AUDIO_FORMAT_DTS;
        break;
    case PLAYMGR_ADEC_WMA:    
        prFeederSetting->eAudFormat = SD_AUDIO_FORMAT_WMA;
        break;
    default:
        prFeederSetting->eAudFormat = SD_AUDIO_FORMAT_NONE;
        prFeederSetting->audio.streamId = 0;
        break;
    }
    
    prFeederSetting->bReversePlay = FALSE;//(prPlay->u4PlayCap & PLAYMGR_CAP_TRICKMODE)? TRUE : FALSE;
    prFeederSetting->speed = prPlay->i4Speed;
    prFeederSetting->totalTime = prPlay->u4TotalDuration;
    prFeederSetting->totalSize = prPlay->u8FileSize;

    return 0;
}


//-----------------------------------------------------------------------------
/** Get the target decoder
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_GetSourceType(unsigned long arg)
{
    SdFeeder_Settings_t rCurFeederSetting;
    PLAYMGR_STATUS_T rPlayInfo;
    SdVideoFormat_k  eVideoFmt;
    SdAudioFormat_k  eAudioFmt;
    FeederSourceType eDataType;
#if 0    
    INT32  i4Ret;
    
    i4Ret = FeederGetDecodeFmt((UINT32 *)&eVideoFmt, (UINT32 *)&eAudioFmt);
    if (i4Ret != FEEDER_E_OK)
    {
        return SD_NOT_OK;
    }
#else
    // FixMe:
    // This function takes time, please only call it one time in one file.
    if (!PLAYMGR_Open(NULL))
    {
        return (SD_NOT_OK);
    }    
    if (!PLAYMGR_GetInfo(&rPlayInfo))
    {
        return (SD_NOT_OK);
    }
    _SdFeeder_GetPlaymgrInfo(&rPlayInfo, &rCurFeederSetting);
    eVideoFmt = rCurFeederSetting.eVidFormat;
    eAudioFmt = rCurFeederSetting.eAudFormat;
#endif

    if (eVideoFmt != SD_VIDEO_FORMAT_NONE)
    {
        eDataType = FEEDER_PROGRAM_SOURCE;//(eAudioFmt == SD_AUDIO_FORMAT_NONE) ?
                    //FEEDER_VIDEO_SOURCE : FEEDER_PROGRAM_SOURCE;
    }
    else
    {
        if (eAudioFmt == SD_AUDIO_FORMAT_NONE)
        {
            // SD_AUDIO_FORMAT_NONE & SD_VIDEO_FORMAT_NONE
            return SD_NOT_OK;
        }
        
        eDataType = FEEDER_AUDIO_SOURCE;
    }
        
    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)arg, sizeof(INT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetSourceType : u4StartAddr argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }
    
    if (copy_to_user((void __user *)arg, &eDataType, sizeof(INT32)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetSourceType : copy argument to user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }
    
    return (SD_OK);
}


//-----------------------------------------------------------------------------
/** Get or set the target setting
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_GetSetting(unsigned long arg)
{
    SDAL_IOCTL_3ARG_T rArg3;
    FeederSourceType eDataType;
    SdFeeder_Settings_t rCurFeederSetting;
    PLAYMGR_STATUS_T rPlayInfo;
    PLAYMGR_SETTING_T rPlaySettings;
    UINT32 u4Get;
    BOOL fgRet;
    UINT32 *pu4Setting;
    INT32 i4Speed;

        // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(SDAL_IOCTL_3ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetSetting : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    // Copy user space argument
    if (copy_from_user(&rArg3, (void __user *)arg, sizeof(SDAL_IOCTL_3ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetSetting : copy argument from user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    eDataType = (FeederSourceType)rArg3.au4Arg[0];
    pu4Setting = (UINT32 *)rArg3.au4Arg[1];
    u4Get = rArg3.au4Arg[2];
    
    x_memset((VOID *)&rPlayInfo, 0x0, sizeof(PLAYMGR_STATUS_T));   
    
    // Set the state
    // Check access of user space

    if (u4Get)                                  //get settings
    {
        // FixMe:
        // This function takes time, please only call it one time in one file.
        if (!PLAYMGR_GetInfo(&rPlayInfo))
        {
            return (SD_NOT_OK);
        }
        _SdFeeder_GetPlaymgrInfo(&rPlayInfo, &rCurFeederSetting);
        
        if (!access_ok(VERIFY_WRITE, (void __user *)pu4Setting,
                       sizeof(SdFeeder_Settings_t)))
        {
#ifdef IO_FEEDER_DEBUG
            printk("_SdFeeder_GetSetting : pu4Size argument error\n");
#endif  // IO_FEEDER_DEBUG
            return (SD_ERR_SYS);
        }
    
        if (copy_to_user((void __user *)pu4Setting, &rCurFeederSetting,
                         sizeof(SdFeeder_Settings_t)))
        {
#ifdef IO_FEEDER_DEBUG
            printk("_SdFeeder_GetSetting : copy argument to user error\n");
#endif  // IO_FEEDER_DEBUG
            return (SD_ERR_SYS);
        }
    }
    else                                       //set settings
    {
    	if(copy_from_user(&rCurFeederSetting, (void __user *)pu4Setting, 
    	                  sizeof(SdFeeder_Settings_t)))
	    {
#ifdef IO_FEEDER_DEBUG	    
		    printk("_SdFeeder_GetSetting : copy_from_user error\n");
#endif
		    return SD_ERR_SYS;
	    }
    	rPlaySettings.u4InfoMask = PLAYMGR_INFO_MASK_NONE;
    	// FixMe: To check if streamid is valid
    	/*
    	rPlaySettings.u4InfoMask |= PLAYMGR_INFO_MASK_VIDEO;
    	rPlaySettings.u2VideoId= (UINT16)rCurFeederSetting.video.streamId;
    	rPlaySettings.u4InfoMask |= PLAYMGR_INFO_MASK_AUDIO;
    	rPlaySettings.u2AudioId= (UINT16)rCurFeederSetting.audio.streamId;
    	rPlaySettings.u4InfoMask |= PLAYMGR_INFO_MASK_CAPTRACK;
    	rPlaySettings.u2CapTrackId = (UINT16)rCurFeederSetting.caption.streamId;
    	*/
    	rPlaySettings.u4InfoMask |= PLAYMGR_INFO_MASK_FILESIZE;
    	rPlaySettings.u8FileSize = rCurFeederSetting.totalSize;
    	fgRet = PLAYMGR_Set(&rPlaySettings);
    	if(!fgRet)
        {
            return (SD_NOT_OK);
        }

        i4Speed = rCurFeederSetting.speed;
        if(rCurFeederSetting.bReversePlay)
        {
            i4Speed = -i4Speed;
        }
        fgRet = PLAYMGR_Speed(rCurFeederSetting.speed);
    	if(!fgRet)
        {
            return (SD_NOT_OK);
        }
    	
    }
    return SD_OK;
}


//-----------------------------------------------------------------------------
/** unlock playmgr
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_FinishPullData(unsigned long arg)
{
    SDAL_IOCTL_3ARG_T rArg3;
    UINT32 u4PullSize;
    UINT64 u8Pos;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(SDAL_IOCTL_3ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetStatus : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    // Copy user space argument
    if (copy_from_user(&rArg3, (void __user *)arg, sizeof(SDAL_IOCTL_3ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetStatus : copy argument from user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    u4PullSize = rArg3.au4Arg[0];
    u8Pos = ((((UINT64)rArg3.au4Arg[1]) << 32) | (UINT64)rArg3.au4Arg[2]);

    if (!PLAYMGR_ReceiveData(u4PullSize, u8Pos))
    {
        return SD_NOT_OK;
    }
    
    return SD_OK;
}


//-----------------------------------------------------------------------------
/** unlock playmgr
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdFeeder_MovePos(unsigned long arg)
{
    SDAL_IOCTL_3ARG_T rArg3;
    UINT64 u8FileOffset;
    UINT32 u4Pos;
   
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(SDAL_IOCTL_3ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetStatus : argument error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    // Copy user space argument
    if (copy_from_user(&rArg3, (void __user *)arg, sizeof(SDAL_IOCTL_3ARG_T)))
    {
#ifdef IO_FEEDER_DEBUG
        printk("_SdFeeder_GetStatus : copy argument from user error\n");
#endif  // IO_FEEDER_DEBUG
        return (SD_ERR_SYS);
    }

    u4Pos = (UINT32)rArg3.au4Arg[1];
    u8FileOffset = (UINT64)rArg3.au4Arg[2];

    PLAYMGR_SeekTime(u4Pos);
    
    return SD_OK;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int sdal_ioctl_sdfeeder(struct inode *inode, struct file *file,
                        unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case SDAL_IO_FEEDER_OPEN:
        i4Ret = _SdFeeder_Open(arg);
        break;
    case SDAL_IO_FEEDER_CLOSE:
        i4Ret = _SdFeeder_Close(arg);
        break;
    case SDAL_IO_FEEDER_START:
        i4Ret = _SdFeeder_Start(arg);
        break;
    case SDAL_IO_FEEDER_STOP:
        i4Ret = _SdFeeder_Stop(arg);
        break;
    case SDAL_IO_FEEDER_PAUSE:
        i4Ret = _SdFeeder_Pause(arg);
        break;
    case SDAL_IO_FEEDER_RESUME:
        i4Ret = _SdFeeder_Resume(arg);
        break;
    case SDAL_IO_FEEDER_DATA_SUBMIT:
        i4Ret = _SdFeeder_SubmitData(arg);
        break;
    case SDAL_IO_FEEDER_GET_BUFF_INFO:
        i4Ret = _SdFeeder_GetBufferInfo(arg);
        break;
    case SDAL_IO_FEEDER_GET_BUFFER:
        i4Ret = _SdFeeder_GetBuffer(arg);
        break;
    case SDAL_IO_FEEDER_GET_STATUS:
        i4Ret = _SdFeeder_GetStatus(arg);
        break;
    case SDAL_IO_FEEDER_GET_SRC_TYPE:
        i4Ret = _SdFeeder_GetSourceType(arg);
        break;
    case SDAL_IO_FEEDER_GET_SETTING:
        i4Ret = _SdFeeder_GetSetting(arg);
        break;
    case SDAL_IO_FEEDER_FINISH_PULLDATA:
        i4Ret = _SdFeeder_FinishPullData(arg);
        break; 
    case SDAL_IO_FEEDER_MOVE:
        i4Ret = _SdFeeder_MovePos(arg);
        break;
    default:
#ifdef IO_FEEDER_DEBUG
        printk("SDFeeder: unknown IO command %u\n", cmd);
#endif  // IO_FEEDER_DEBUG
        i4Ret = SD_NOT_SUPPORTED;
    }

    return i4Ret;
}
#endif // CC_MT5360


