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
 * $Author: jifeng.fan $
 * $Date: 2012/11/16 $
 * $RCSfile: io_ice_vga.c,v $
 * $Revision: #10 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_ice_vga.c
 *  ICE VGA driver interface.
 */

#include "mttype.h"
#include "ice.h"
#include "ice_video.h"
#include "drv_video.h"
#include "drv_async.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "vdec_if.h"
#include "vdec_drvif.h"
#include "mpv_if.h"
#include "vdp_if.h"
#include "dmx_if.h"
#include "ice_ioctl.h"
//#include "panel.h"
#include "drv_display.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define USR_SPACE_ACCESS_VALIDATE_2AR(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_2AR(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3AR(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_3AR(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_4AR(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_4AR(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_8AR(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_8AR(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_8ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_AR(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_AR(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_ARG_K(ArgUsr, ArgKernel,size)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_ci argument error7\n");      \
        return MTR_ERR_INV;                                          \
    }  

#define USR_SPACE_ACCESS_VALIDATE_ARG_K(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_ci argument error6\n");     \
        return MTR_ERR_INV;                                          \
    } 



//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
UINT8 bTestMode = 0;
//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static MT_RESULT_T _ICE_Video_GetAFD(unsigned long arg)
{
    ICEDEC_HDR_AFD_INFO_T*   prIceVdecInfo;
    ICE_IOCTL_2ARG_T    rArg;
    VDEC_HDR_AFD_INFO_T rHdrInfo;
    
    UCHAR ucEsId;
    ICEDEC_HDR_AFD_INFO_T    rVdecInfo;

    USR_SPACE_ACCESS_VALIDATE_2AR(arg);
    COPY_FROM_USER_2AR(arg, rArg);
    ucEsId =(UCHAR)rArg.ai4Arg[0];
    prIceVdecInfo = (ICEDEC_HDR_AFD_INFO_T*)rArg.ai4Arg[1];
    x_memset(&rVdecInfo, 0, sizeof(ICEDEC_HDR_AFD_INFO_T));

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
    
    VDEC_QueryAFDInfo(ucEsId, &rHdrInfo);
    
    rVdecInfo.fgAfdPrs = rHdrInfo.fgAfdPrs;
    rVdecInfo.fgHdrInvalid = rHdrInfo.fgHdrInvalid;
    rVdecInfo.eAspectRatio = rHdrInfo.eAspectRatio;
    rVdecInfo.ucAfd = rHdrInfo.ucAfd;

    USR_SPACE_ACCESS_VALIDATE_AR(prIceVdecInfo, ICEDEC_HDR_AFD_INFO_T );
    COPY_TO_USER_AR(prIceVdecInfo, rVdecInfo, ICEDEC_HDR_AFD_INFO_T );

    ICE_VDO_PRINT(" - ucEsId = %d\n",(int)ucEsId);

    return MTR_OK;
}

static MT_RESULT_T _ICE_Video_ResetAFD(unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
   // ICE_IOCTL_2ARG_T    rArg;
    UCHAR ucEsId;

    ucEsId = (UCHAR)arg;

    VDEC_ResetAFD(ucEsId);
    return eRet;
}

static MT_RESULT_T _ICE_Video_CHKVideoCodecSupport(unsigned long arg)
{
	//    ICE_IOCTL_2ARG_T    rArg; 
    UCHAR ucEsId;
    BOOL fgIsCodecSup;

    ucEsId = (UCHAR)arg;

    //add driver implementation code here
    if(ucEsId >= VDEC_MAX_ES)
    {
        return MTR_NOT_SUPPORTED;
    }
    
    fgIsCodecSup = VDEC_ChkCodecCap(ucEsId, 2);
    if(fgIsCodecSup)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_SUPPORTED;
    }

}

static MT_RESULT_T _ICE_Video_SetTestMode(unsigned long arg)
{
    printf("io_ice_video:_ICE_Video_SetTestMode\n");
 //   WritePanelIndexToEeprom(3);
//    PDWNC_Reboot();
    SetPanel(3);
    return MTR_OK;
}

static MT_RESULT_T _ICE_Video_GetTestMode(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pTestMode = (UINT8 *)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG_K(pTestMode, 1);
    COPY_TO_USER_ARG_K(pTestMode, bTestMode, 1);
    return i4Ret;
}
static MT_RESULT_T _ICE_Video_SetCROPMode(unsigned long arg)
{
    BOOL bSetCROPMode = 0;

    bSetCROPMode = (BOOL)arg;
//    printf("\n ------------- bSetCROPMode = %d  \n",bSetCROPMode);
    VDEC_EableCrop(0,bSetCROPMode);

    return MTR_OK;
    
}

static MT_RESULT_T _ICE_Video_Init(unsigned long arg)
{
    return MTR_OK;
}

static MT_RESULT_T _ICE_Video_SetMpeg4Flag(unsigned long arg)
{
 
  UINT32 fgFlag;
  fgFlag = (UINT32)arg;
  
  printf("_ICE_Video_SetMpeg4Flag fgFlag=%d.\n",fgFlag);

  VDEC_SetMpeg4Flag(fgFlag);

  return MTR_OK;
}

static MT_RESULT_T _ICE_Video_GetDVDModeSignalStatus(unsigned long arg)
{
	UINT32 u32DVDMode =0, u32SigStatus=0;
	UINT16 u16DVDMode =0;

	/* driver func*/
	bApiVideoGetDVDModeSigStatus(& u32DVDMode, &u32SigStatus);
	u16DVDMode = (u32DVDMode<<8) + u32SigStatus;

	printf("_ICE_Video_GetDVDModeSignalStatus DVD Mode(%d), Sig(%d)\n",
		u32DVDMode, u32SigStatus);
	
	USR_SPACE_ACCESS_VALIDATE_ARG_K(arg, 2 );
    COPY_TO_USER_ARG_K(arg, u16DVDMode, 2 );

	return MTR_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int ice_ioctl_video(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_NOT_OK;
    switch (cmd)
    {
        case ICE_IO_VIDEO_GETAFD:
            i4Ret = _ICE_Video_GetAFD(arg);  
            break;
        case ICE_IO_VIDEO_RESETAFD:
            i4Ret = _ICE_Video_ResetAFD(arg);  
            break;
        case ICE_IO_VIDEO_CHKVIDEOCODECSUPPORT:
            i4Ret = _ICE_Video_CHKVideoCodecSupport(arg);  
            break; 
        case ICE_IO_VIDEO_SET_TEST_MODE:
            i4Ret = _ICE_Video_SetTestMode(arg);  
            break;
        case ICE_IO_VIDEO_GET_TEST_MODE:
            i4Ret = _ICE_Video_GetTestMode(arg);  
            break;
        case ICE_IO_VIDEO_INIT:
            i4Ret = _ICE_Video_Init(arg);  
            break;
        case ICE_IO_VIDEO_SET_MPEG4_FLAG:
            i4Ret = _ICE_Video_SetMpeg4Flag(arg);
            break;        
        case ICE_IO_VIDEO_SET_CROP_MODE:
            i4Ret = _ICE_Video_SetCROPMode(arg);  
            break;
        case ICE_IO_VIDEO_GET_DVDMODE_SIG:
            i4Ret = _ICE_Video_GetDVDModeSignalStatus(arg);
            break;
        default:
            break;
    }
    return i4Ret;
}

