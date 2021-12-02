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
 * $RCSfile: io_mtosd.c,v $
 * $Revision: #4 $
 *---------------------------------------------------------------------------*/

/** @file io_mtosd.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtosd.h"
#include "osd_drvif.h"
#include <linux/mtal_ioctl.h>


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
extern INT32 OSD_PLA_Set3DMode(UINT32 u4Plane, UINT8 u1Mode);

extern INT32 OSD_PLA_Set3DRegion(UINT32 u4Plane, UINT32 u4LeftRgn, UINT32 u4RightRgn);

extern INT32 OSD_BASE_EnableAutoLR(UINT32 u4Enable);

extern INT32 OSD_BASE_GetLRStatus(UINT32* pu4Status);

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                                             \

#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)             \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(type)))                        \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))                            \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                    \

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))                            \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))                        \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_5ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_5ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return -1;                                          \
    }
#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return -1;                                          \
    }

#define COPY_FROM_USER_5ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_5ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_8ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_8ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_8ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                                              \

#define USR_SPACE_ACCESS_VALIDATE_12ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_12ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_12ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_12ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                                             \

#define USR_SPACE_ACCESS_VALIDATE(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                                             \
    
/** reset osd driver and hardware status
 *  @param  VOID
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTOSD_Reset(void)
{
	MTOSD_PRINT(" - void\n");

    //add driver implementation code here
    OSD_Reset();
	return MTR_OK;
}

/** set osd plane position
 *  @param  u4Plane                 plane id
 *  @param  u4X                     x position(0~512)
 *  @param  u4Y                     y position(0~512)
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_BASE_SetOsdPosition(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;

	UINT32 u4Plane;
	UINT32 u4X;
	UINT32 u4Y;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    u4X = (UINT32)rArg.ai4Arg[1];
    u4Y = (UINT32)rArg.ai4Arg[2];

    MTOSD_PRINT(" - u4Plane = 0x%x, u4X = 0x%x, u4Y = 0x%x\n",
		u4Plane,
		u4X,
		u4Y);

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_PLANE)==OSD_BASE_SetOsdPosition(u4Plane,u4X,u4Y))
    {
        return MTR_OSD_INV_PLANE;
    }
    return MTR_OK;
}

/** get osd plane position
 *  @param  u4Plane                 plane id
 *  @param  pu4X                    x position pointer
 *  @param  pu4Y                    y position pointer
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_BASE_GetOsdPosition(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
	UINT32 u4Plane;
	UINT32 * pu4X;
	UINT32 * pu4Y;

	UINT32  u4X;
	UINT32  u4Y;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    pu4X = (UINT32*)rArg.ai4Arg[1];
    pu4Y = (UINT32*)rArg.ai4Arg[2];

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_PLANE)==OSD_BASE_GetOsdPosition(u4Plane,&u4X,&u4Y))
    {
        return MTR_OSD_INV_PLANE;
    }
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4X,UINT32);
    COPY_TO_USER_ARG(pu4X, u4X,UINT32);
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Y,UINT32);
    COPY_TO_USER_ARG(pu4Y, u4Y,UINT32);

    MTOSD_PRINT(" - u4Plane = 0x%x, u4X = 0x%x, u4Y = 0x%x\n",
		u4Plane,
		u4X,
		u4Y);

    return MTR_OK;
}

/** enable/disable osd plane, it will also disable scaler when disable plane
 *  @param  u4Plane                 plane id
 *  @param  fgEnable                enable=TRUE/disable=FALSE
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_PLA_Enable(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTOSD_PRINT(" - u4Plane = 0x%x, fgEnable = 0x%x\n",
		u4Plane,
		fgEnable);

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_PLANE)==OSD_PLA_Enable(u4Plane,fgEnable))
    {
        return MTR_OSD_INV_PLANE;
    }
    return MTR_OK;
}


/** attach a specific list to osd plane
 *  @param  u4Plane                 plane id
 *  @param  u4RgnList               list id
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 *  @retval MTR_OSD_INV_LIST        Invalid list
 */
static MT_RESULT_T _MTOSD_PLA_FlipTo(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    UINT32 u4RgnList;
    INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    u4RgnList = (UINT32)rArg.ai4Arg[1];

    MTOSD_PRINT(" - u4Plane = 0x%x, fgEnable = 0x%x\n",
		u4Plane,
		u4RgnList);

    //add driver implementation code here
    rVal=OSD_PLA_FlipTo(u4Plane,u4RgnList);
    if (rVal==(-(INT32)OSD_RET_INV_PLANE))
    {
        return MTR_OSD_INV_PLANE;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_LIST))
    {
        return MTR_OSD_INV_LIST;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_REGION))
    {
        return MTR_OSD_INV_REGION;
    }
    return MTR_OK;
}

/** get osd plane alpha level
 *  @param  u4Plane                 plane id
 *  @param  pu1BlendLevel           aplha level pointer
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_PLA_GetBlendLevel(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    UINT8* pu1BlendLevel;
    UINT8  u1BlendLevel;
    INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    pu1BlendLevel = (UINT8*)rArg.ai4Arg[1];

    //add driver implementation code here
    rVal=OSD_PLA_GetBlendLevel(u4Plane,&u1BlendLevel);
    if (rVal==(-(INT32)OSD_RET_INV_PLANE))
    {
        return MTR_OSD_INV_PLANE;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1BlendLevel,UINT8);
    COPY_TO_USER_ARG(pu1BlendLevel,u1BlendLevel,UINT8);

    MTOSD_PRINT(" - u4Plane = 0x%x, u1BlendLevel = 0x%x\n",
		u4Plane,u1BlendLevel);
    return MTR_OK;
}

/** set osd plane alpha level
 *  @param  u4Plane                 plane id
 *  @param  u1BlendLevel            aplha level(0~255)
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_PLA_SetBlendLevel(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    UINT8 u1BlendLevel;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    u1BlendLevel = (UINT8)rArg.ai4Arg[1];

    MTOSD_PRINT(" - u4Plane = 0x%x, u1BlendLevel = 0x%x\n",
		u4Plane,
		u1BlendLevel);

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_PLANE)==OSD_PLA_SetBlendLevel(u4Plane,u1BlendLevel))
    {
        return MTR_OSD_INV_PLANE;
    }
    return MTR_OK;
}

/** get osd plane fading value
 *  @param  u4Plane                 plane id
 *  @param  pu1Fading               fading value pointer
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_PLA_GetFading(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    UINT8* pu1Fading;
    UINT8  u1Fading;
    INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    pu1Fading = (UINT8*)rArg.ai4Arg[1];

    //add driver implementation code here
    rVal=OSD_PLA_GetFading(u4Plane,&u1Fading);
    if (rVal==(-(INT32)OSD_RET_INV_PLANE))
    {
        return MTR_OSD_INV_PLANE;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1Fading,UINT8);
    COPY_TO_USER_ARG(pu1Fading, u1Fading,UINT8);

    MTOSD_PRINT(" - u4Plane = 0x%x, *pu1Fading = 0x%x\n",
		u4Plane,
		u1Fading);
    return MTR_OK;
}

/** set osd plane fading value
 *  @param  u4Plane                 plane id
 *  @param  u1Fading                fading value{0~255}
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_PLA_SetFading(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    UINT8 u1Fading;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    u1Fading = (UINT8)rArg.ai4Arg[1];

    MTOSD_PRINT(" - u4Plane = 0x%x, u1Fading = 0x%x\n",
		u4Plane,
		u1Fading);

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_PLANE)==OSD_PLA_SetFading(u4Plane,u1Fading))
    {
        return MTR_OSD_INV_PLANE;
    }
    return MTR_OK;
}


static MT_RESULT_T _MTOSD_PLA_SetLineShift(unsigned long arg)
{
   #if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)

    MTAL_IOCTL_3ARG_T    rArg;
    UINT32 u4Plane;
    BOOL fgEnable;
	INT32 rShiftValue;
	
	USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
	COPY_FROM_USER_3ARG(arg, rArg);
	
    u4Plane = (UINT32)rArg.ai4Arg[0];
	fgEnable = (BOOL)rArg.ai4Arg[1];
	rShiftValue = (UINT32)rArg.ai4Arg[2];
			
    MTOSD_PRINT(" - u4Plane = 0x%x, fgEnable = 0x%x, rShiftValue = 0x%x\n",
		u4Plane,
		fgEnable, rShiftValue);

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_PLANE)==OSD_BASE_SetPlaneLineShift(u4Plane,fgEnable,rShiftValue))
    {
        return MTR_OSD_INV_PLANE;
    }
  #endif
	
	
    return MTR_OK;
}


/** print osd debug info on console
 *  @param  u4Plane                 plane id
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_PLA_Dump(unsigned long arg)
{
	INT32 u4Plane;
	INT32 rVal;

    u4Plane = (INT32)arg;

    MTOSD_PRINT(" - u4Plane = 0x%x\n", u4Plane);

    //add driver implementation code here
    rVal=OSD_PLA_Dump(u4Plane);
    if (rVal==(-(INT32)OSD_RET_INV_PLANE))
    {
        return MTR_OSD_INV_PLANE;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_SCALER))
    {
        return MTR_OSD_INV_SCALER;
    }
    return MTR_OK;
}

/** get the first region id of osd plane
 *  @param  u4Plane                 plane id
 *  @param  pu4RegionId             region id
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_NO_REGION       No region attach to OSD plane
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_PLA_GetFirstRegion(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    UINT32* pu4RegionId;
    INT32  u4RegionId;
    #ifdef CC_MT5360
    INT32 rVal;
    #endif

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    pu4RegionId = (UINT32*)rArg.ai4Arg[1];

		#ifdef CC_MT5360
		rVal = OSD_PLA_GetFirstRegion(u4Plane,&u4RegionId);
		if ((-(INT32)OSD_RET_INV_PLANE)==rVal)
    {
        return MTR_OSD_INV_PLANE;
    }
		#else
    //add driver implementation code here
    u4RegionId = OSD_PLA_GetFirstRegion(u4Plane);
    if ((-(INT32)OSD_RET_INV_PLANE)==u4RegionId)
    {
        return MTR_OSD_INV_PLANE;
    }
		#endif
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4RegionId,UINT32);
    COPY_TO_USER_ARG(pu4RegionId, u4RegionId,UINT32);

    MTOSD_PRINT(" - u4Plane = 0x%x, u4RegionId = 0x%x\n",
		u4Plane,
		u4RegionId);	
    return MTR_OK;
}

/** get the first region id of osd plane
 *  @param  u4Plane                 plane id
 *  @param  pu4RegionId             region id
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_NO_REGION       No region attach to OSD plane
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_PLA_GetValidCnt(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    UINT32* pu4ValidCnt;
    UINT8  u4ValidCnt;
    BOOL rVal;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    pu4ValidCnt = (UINT32*)rArg.ai4Arg[1];

    //add driver implementation code here
    rVal=OSD_PLA_OutputValidCnt(u4Plane,&u4ValidCnt);
    if (rVal==FALSE)
    {
        u4ValidCnt = 0;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4ValidCnt,UINT32);
    COPY_TO_USER_ARG(pu4ValidCnt, u4ValidCnt,UINT32);

    MTOSD_PRINT(" - u4Plane = 0x%x, *pu1Fading = 0x%x\n",
		u4Plane,
		u4ValidCnt);
    return MTR_OK;
}


/** wait osd plane stable
 *  @param  u4Plane                 plane id
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */

static MT_RESULT_T _MTOSD_PLA_WaitStable(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    BOOL rVal;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];

    //add driver implementation code here
    rVal= OSD_PLA_WaitStable(u4Plane);

    return MTR_OK;
}



/** get the first region address of osd plane
 *  @param  u4Plane                 plane id
 *  @param  pu4Address              region address
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_NO_REGION       No region attach
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_PLA_GetFirstRegionAddress(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    UINT32* pu4Address;
    UINT32  u4Address;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    pu4Address = (UINT32*)rArg.ai4Arg[1];

    //add driver implementation code here
    u4Address = OSD_PLA_GetFirstRegionAddress(u4Plane);
    if ((-(INT32)OSD_RET_INV_PLANE)==u4Address)
    {
        return MTR_OSD_INV_PLANE;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Address,UINT32);
    COPY_TO_USER_ARG(pu4Address, u4Address,UINT32);

    MTOSD_PRINT(" - u4Plane = 0x%x, u4Address = 0x%x\n",
		u4Plane,
		u4Address);
    return MTR_OK;
}

/** create an osd list
 *  @param pu4RgnList               list id pointer
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTOSD_RGN_LIST_Create(unsigned long arg)
{
    UINT32* pu4RgnList;
    UINT32 u4RgnList;
    INT32 rVal;

    pu4RgnList = (UINT32*)arg;

    //add driver implementation code here
    rVal = OSD_RGN_LIST_Create(&u4RgnList);
    if (rVal==(-(INT32)OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }
    else if (rVal==(-(INT32)OSD_RET_OUT_OF_LIST))
    {
        return MTR_NOT_OK;
    }
    
    USR_SPACE_ACCESS_VALIDATE(pu4RgnList,UINT32);
    COPY_TO_USER_ARG(pu4RgnList, u4RgnList, UINT32);

    MTOSD_PRINT(" u4RgnList = 0x%x\n", u4RgnList);
    return MTR_OK;
}

/** delete an osd list
 *  @param  u4RgnList               list id
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_LIST        Invalid list
 */
static MT_RESULT_T _MTOSD_RGN_LIST_Delete(unsigned long arg)
{
    UINT32 u4RgnList;

    u4RgnList = (UINT32)arg;

    MTOSD_PRINT(" u4RgnList = 0x%x\n", u4RgnList);

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_LIST)==OSD_RGN_LIST_Delete(u4RgnList))
    {
        return MTR_OSD_INV_LIST;
    }
    
    return MTR_OK;
}

/** get first region id of list
 *  @param  u4RgnList               list id
 *  @param  pu4RegionId             region id pointer
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_LIST        Invalid list
 *  @retval MTR_OSD_NO_REGION       No region attach
 */
static MT_RESULT_T _MTOSD_RGN_LIST_GetHead(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4RgnList;
    UINT32* pu4RegionId;
    UINT32  u4RegionId;
    INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4RgnList = (UINT32)rArg.ai4Arg[0];
    pu4RegionId = (UINT32*)rArg.ai4Arg[1];

    //add driver implementation code here
     rVal=OSD_RGN_LIST_GetHead(u4RgnList,&u4RegionId);
    if (rVal==(-(INT32)OSD_RET_INV_LIST))
    {
        return MTR_OSD_INV_LIST;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4RegionId,UINT32);
    COPY_TO_USER_ARG(pu4RegionId, u4RegionId,UINT32);

    MTOSD_PRINT(" - u4RgnList = 0x%x, *pu4RegionId = 0x%x\n",
		u4RgnList,
		u4RegionId);

    return MTR_OK;
}

/** get osd region attribute
 *  @param  u4Region                region id
 *  @param  i4Cmd                   attribute name, refer to MTOSD_RGN_CMD_T
 *  @param  pu4Value                attribute value pointer
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_REGION      Invalid region
 *  @retval MTR_OSD_INV_ATTRIBUTE   Invalid region attribute value
 */
static MT_RESULT_T _MTOSD_RGN_Get(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;

	UINT32 u4Region;
	MTOSD_RGN_CMD_T eCmd;
	UINT32 * pu4Value;
	UINT32   u4Value;
	INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u4Region = (UINT32)rArg.ai4Arg[0];
    eCmd = (MTOSD_RGN_CMD_T)rArg.ai4Arg[1];
    pu4Value = (UINT32*)rArg.ai4Arg[2];

    //add driver implementation code here
    rVal=OSD_RGN_Get(u4Region,eCmd,&u4Value);
    if (rVal==(-(INT32)OSD_RET_INV_REGION))
    {
        return MTR_OSD_INV_REGION;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Value,UINT32);
    COPY_TO_USER_ARG(pu4Value, u4Value,UINT32);
    
    MTOSD_PRINT(" - u4Region = 0x%x, eCmd = 0x%x, u4Value = 0x%x\n",
		u4Region,
		eCmd,
		u4Value);

    return MTR_OK;
}

/** set osd region attribute
 *  @param  u4Region                region id
 *  @param  i4Cmd                   attribute name, refer to MTOSD_RGN_CMD_T
 *  @param  u4Value                 attribute value
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_REGION      Invalid region
 *  @retval MTR_OSD_INV_ATTRIBUTE   Invalid region attribute value
 */
static MT_RESULT_T _MTOSD_RGN_Set(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;

	UINT32 u4Region;
	MTOSD_RGN_CMD_T eCmd;
	UINT32 u4Value;
	INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u4Region = (UINT32)rArg.ai4Arg[0];
    eCmd = (MTOSD_RGN_CMD_T)rArg.ai4Arg[1];
    u4Value = (UINT32)rArg.ai4Arg[2];

    MTOSD_PRINT(" - u4Region = 0x%x, eCmd = 0x%x, u4Value = 0x%x\n",
		u4Region,
		eCmd,
		u4Value);

    //add driver implementation code here
    rVal=OSD_RGN_Set(u4Region,eCmd,u4Value);
    if (rVal==(-(INT32)OSD_RET_INV_REGION))
    {
        return MTR_OSD_INV_REGION;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }
    return MTR_OK;
}

/** create an osd region
 *  @param  pu4Region               region id pointer
 *  @param  u4BmpWidth              source bitmap width
 *  @param  u4BmpHeight             source bitmap height
 *  @param  pvBitmap                source bitmap address, align with 16bytes
 *  @param  u4ColorMode             color mode, refer to MTOSD_COLOR_MODE_T
 *  @param  u4BmpPitch              bytes per line, align with 16bytes
 *  @param  u4DispX                 region x position
 *  @param  u4DispY                 region y position
 *  @param  u4DispW                 output width
 *  @param  u4DispH                 output height
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_ATTRIBUTE   Invalid region attribute value
 */
static MT_RESULT_T _MTOSD_RGN_Create(unsigned long arg)
{
    MTAL_IOCTL_10ARG_T rArg;

	UINT32 * pu4Region;
	UINT32 u4BmpWidth;
	UINT32 u4BmpHeight;
    VOID* pvBitmap;
	MTOSD_COLOR_MODE_T eColorMode;
	UINT32 u4BmpPitch;
	UINT32 u4DispX;
	UINT32 u4DispY;
	UINT32 u4DispW;
	UINT32 u4DispH;
	INT32 rVal;
	UINT32  u4Region;

    USR_SPACE_ACCESS_VALIDATE_12ARG(arg);
    COPY_FROM_USER_12ARG(arg, rArg);

	pu4Region = (UINT32*)rArg.ai4Arg[0];
	u4BmpWidth = (UINT32)rArg.ai4Arg[1];
	u4BmpHeight = (UINT32)rArg.ai4Arg[2];
    pvBitmap = (VOID*)rArg.ai4Arg[3];
	eColorMode = (MTOSD_COLOR_MODE_T)rArg.ai4Arg[4];
	u4BmpPitch = (UINT32)rArg.ai4Arg[5];
	u4DispX = (UINT32)rArg.ai4Arg[6];
	u4DispY = (UINT32)rArg.ai4Arg[7];
	u4DispW = (UINT32)rArg.ai4Arg[8];
	u4DispH = (UINT32)rArg.ai4Arg[9];

    MTOSD_PRINT(" - u4BmpWidth = 0x%x\n"
		"u4BmpHeight = 0x%x\n"
		"eColorMode = 0x%x\n"
		"u4BmpPitch = 0x%x\n"
		"u4DispX = 0x%x\n"
		"u4DispY = 0x%x\n"
		"u4DispW = 0x%x\n"
		"u4DispH = 0x%x\n",
	       u4BmpWidth, 
	       u4BmpHeight,
	       eColorMode,
		u4BmpPitch, 
		u4DispX,
		u4DispY, 
		u4DispW,
		u4DispH);

    //add driver implementation code here
    rVal=OSD_RGN_Create(&u4Region,u4BmpWidth,u4BmpHeight,
                                       pvBitmap,eColorMode,u4BmpPitch,
                                       u4DispX,u4DispY,u4DispW,u4DispH);

    //COPY_TO_USER_ARG(arg, rArg, UINT32);
    
    if (rVal==(-(INT32)OSD_RET_OUT_OF_REGION))
    {
        return MTR_OSD_NO_REGION;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }
    COPY_TO_USER_ARG(pu4Region, u4Region, UINT32);
    return MTR_OK;
}

/** delete an osd region
 *  @param  u4Region                region id
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_REGION      Invalid region
 */
static MT_RESULT_T _MTOSD_RGN_Delete(unsigned long arg)
{
    UINT32 u4Region;

    u4Region = (UINT32)arg;

    MTOSD_PRINT(" - u4Region = 0x%x\n", (UINT32)u4Region);

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_REGION)==OSD_RGN_Delete(u4Region))
    {
        return MTR_OSD_INV_REGION;
    }
    return MTR_OK;
}

/** attach region to list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_REGION      Invalid region
 *  @retval MTR_OSD_INV_LIST        Invalid list
 */
static MT_RESULT_T _MTOSD_RGN_Insert(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Region;
    UINT32 u4RgnList;
    INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Region = (UINT32)rArg.ai4Arg[0];
    u4RgnList = (UINT32)rArg.ai4Arg[1];

    MTOSD_PRINT(" - u4Region = 0x%x, u4RgnList = 0x%x\n",
		u4Region,
		u4RgnList);

    //add driver implementation code here
    rVal=OSD_RGN_Insert(u4Region,u4RgnList);
    if (rVal==(-(INT32)OSD_RET_REGION_COLLISION))
    {
        return MTR_NOT_OK;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_LIST))
    {
        return MTR_OSD_INV_LIST;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_REGION))
    {
        return MTR_OSD_INV_REGION;
    }
	return MTR_OK;
}

/** dettach region from list
 *  @param  u4Region                region id
 *  @param  u4RgnList               list id
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_REGION      Invalid region
 *  @retval MTR_OSD_INV_LIST        Invalid list
 */
static MT_RESULT_T _MTOSD_RGN_Detach(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Region;
    UINT32 u4RgnList;
    INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Region = (UINT32)rArg.ai4Arg[0];
    u4RgnList = (UINT32)rArg.ai4Arg[1];

    MTOSD_PRINT(" - u4Region = 0x%x, u4RgnList = 0x%x\n",
		u4Region,
		u4RgnList);

    //add driver implementation code here
    rVal=OSD_RGN_Detach(u4Region,u4RgnList);
    if (rVal==(-(INT32)OSD_RET_INV_LIST))
    {
        return MTR_OSD_INV_LIST;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_REGION))
    {
        return MTR_OSD_INV_REGION;
    }
    return MTR_OK;
}

/** cinfugure osd scaler
 *  @param  u4Scaler                scaler id
 *  @param  fgEnable                enable=TRUE/disable=FALSE
 *  @param  u4SrcWidth              osd input width
 *  @param  u4SrcHeight             osd input height
 *  @param  u4DstWidth              osd output width
 *  @param  u4DstHeight             osd output height
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_SCALER      Invalid scaler
 */
static MT_RESULT_T _MTOSD_SC_Scale(unsigned long arg)
{
    MTAL_IOCTL_8ARG_T rArg;

	UINT32 u4Scaler;
	UINT32 u4Enable;
	UINT32 u4SrcWidth;
	UINT32 u4SrcHeight;
	UINT32 u4DstWidth;
	UINT32 u4DstHeight;

    USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_8ARG(arg, rArg);

	u4Scaler = (UINT32)rArg.ai4Arg[0];
	u4Enable = (UINT32)rArg.ai4Arg[1];
	u4SrcWidth = (UINT32)rArg.ai4Arg[2];
	u4SrcHeight = (UINT32)rArg.ai4Arg[3];
	u4DstWidth = (UINT32)rArg.ai4Arg[4];
	u4DstHeight = (UINT32)rArg.ai4Arg[5];

    MTOSD_PRINT(" - u4Scaler = 0x%x\n"
		"u4Enable = 0x%x\n"
		"u4SrcWidth = 0x%x\n"
		"u4SrcHeight = 0x%x\n"
		"u4DstWidth = 0x%x\n"
		"u4DstHeight = 0x%x\n",
		u4Scaler,
		u4Enable,
		u4SrcWidth,
		u4SrcHeight,
		u4DstWidth,
		u4DstHeight);

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_SCALER)==OSD_SC_Scale(u4Scaler,u4Enable,u4SrcWidth,u4SrcHeight,u4DstWidth,u4DstHeight))
    {
        return MTR_OSD_INV_SCALER;
    }
    return MTR_OK;
}

/** get osd scaler infomation
 *  @param  u4Scaler                scaler id
 *  @param  pu4Enable               enable pointer
 *  @param  pu4SrcW                 osd input width pointer
 *  @param  pu4SrcH                 osd input height pointer
 *  @param  pu4DstW                 osd output width pointer
 *  @param  pu4DstH                 osd output height pointer
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_SCALER      Invalid scaler
 */
static MT_RESULT_T _MTOSD_SC_GetScalerInfo(unsigned long arg)
{
    MTAL_IOCTL_8ARG_T rArg;

	UINT32 u4Scaler;
	UINT32* pu4Enable;
	UINT32* pu4SrcW;
	UINT32* pu4SrcH;
	UINT32* pu4DstW;
	UINT32* pu4DstH;
	UINT32* pu4Is16Bpp;

	UINT32  u4Enable = 0;
	UINT32  u4SrcW;
	UINT32  u4SrcH;
	UINT32  u4DstW;
	UINT32  u4DstH;
	UINT32  u4Is16Bpp;
	INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_8ARG(arg, rArg);

	u4Scaler = (UINT32)rArg.ai4Arg[0];
	pu4Enable = (UINT32*)rArg.ai4Arg[1];
	pu4SrcW = (UINT32*)rArg.ai4Arg[2];
	pu4SrcH = (UINT32*)rArg.ai4Arg[3];
	pu4DstW = (UINT32*)rArg.ai4Arg[4];
	pu4DstH = (UINT32*)rArg.ai4Arg[5];
	pu4Is16Bpp = (UINT32*)rArg.ai4Arg[6];

    //add driver implementation code here
    rVal=OSD_SC_GetScalerInfo(u4Scaler,&u4Enable,&u4SrcW,&u4SrcH,&u4DstW,&u4DstH,&u4Is16Bpp);
    if (rVal==(-(INT32)OSD_RET_INV_SCALER))
    {
        return MTR_OSD_INV_SCALER;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Enable,UINT32);
    COPY_TO_USER_ARG(pu4Enable, u4Enable,UINT32);
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4SrcW,UINT32);
    COPY_TO_USER_ARG(pu4SrcW, u4SrcW,UINT32);
	USR_SPACE_ACCESS_VALIDATE_ARG(pu4SrcH,UINT32);
	COPY_TO_USER_ARG(pu4SrcH, u4SrcH,UINT32);
	USR_SPACE_ACCESS_VALIDATE_ARG(pu4DstW,UINT32);
	COPY_TO_USER_ARG(pu4DstW, u4DstW,UINT32);
	USR_SPACE_ACCESS_VALIDATE_ARG(pu4DstH,UINT32);
	COPY_TO_USER_ARG(pu4DstH, u4DstH,UINT32);
	USR_SPACE_ACCESS_VALIDATE_ARG(pu4Is16Bpp,UINT32);
	COPY_TO_USER_ARG(pu4Is16Bpp, u4Is16Bpp,UINT32);

    MTOSD_PRINT(" - u4Scaler = 0x%x\n"
		"u4Enable = 0x%x\n"
		"u4SrcW = 0x%x\n"
		"u4SrcH = 0x%x\n"
		"u4DstW = 0x%x\n"
		"u4DstH = 0x%x\n"
		"u4Is16Bpp = 0x%x\n",
		 u4Scaler,
		 u4Enable,
		 u4SrcW,
		 u4SrcH,
		 u4DstW,
		 u4DstH,
		 u4Is16Bpp);

    return MTR_OK;
}

/** enable low pass filter
 *  @param  u4Scaler                scaler id
 *  @param  fgEnable                enable=TRUE/disable=FALSE
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_SCALER      Invalid scaler
 */
static MT_RESULT_T _MTOSD_SC_SetLpf(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Scaler;
    UINT32 u4Enable;
    INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Scaler = (UINT32)rArg.ai4Arg[0];
    u4Enable = (UINT32)rArg.ai4Arg[1];

    MTOSD_PRINT(" - u4Scaler = 0x%x, u4Enable = 0x%x\n",
		u4Scaler,
		u4Enable);

    //add driver implementation code here
    rVal=OSD_SC_SetLpf(u4Scaler,u4Enable);
    if (rVal==(-(INT32)OSD_RET_INV_SCALER))
    {
        return MTR_OSD_INV_SCALER;
    }
    else if (rVal==(-(INT32)OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }
    return MTR_OK;
}

/** get region list of frame buffer
 *  @param  pu4RgnList              region list value pointer
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_REGION      Invalid region
 */
 static MT_RESULT_T _MTOSD_GetFBRegionList(unsigned long arg)
{
     UINT32* pu4RgnList = NULL;
     UINT32 u4RgnList;
     INT32 rVal;

    pu4RgnList = (UINT32*)arg;

    rVal=OSD_Get_FrameBuffer_RGN_LIST(&u4RgnList);
    	
    if (rVal==(OSD_RET_UNINIT))
    {
        return MTR_NOT_OK;
    }
    else if (rVal==(OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4RgnList, UINT32);
    COPY_TO_USER_ARG(pu4RgnList, u4RgnList, UINT32);
    MTOSD_PRINT(" -u4RgnList = %d\n", u4RgnList);
    return MTR_OK;
}

/** get resolution of frame buffer
 *  @param  pu4Width              frame buffer width
 *  @param  pu4Height              frame buffer height
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_ERR_INV         Invalid Arguement
 */
 static MT_RESULT_T _MTOSD_GetFBResolution(unsigned long arg)
{
		MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Width,u4Height;
    UINT32 *pu4Width,*pu4Height;
    INT32 rVal;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu4Width = (UINT32*)rArg.ai4Arg[0];
    pu4Height = (UINT32*)rArg.ai4Arg[1];
   
    rVal=OSD_Get_FrameBuffer_Resolution(&u4Width,&u4Height);
    	
    if (rVal==(OSD_RET_UNINIT))
    {
        return MTR_NOT_OK;
    }
    else if (rVal==(OSD_RET_INV_ARG))
    {
        return MTR_ERR_INV;
    }
    COPY_TO_USER_ARG(pu4Width, u4Width, UINT32);
    COPY_TO_USER_ARG(pu4Height, u4Height, UINT32);
    MTOSD_PRINT(" IO -u4Width = %d, u4Hieght= %d\n", u4Width,u4Height);
    return MTR_OK;
}

/** Initial osd driver and hardware
 *  @param  VOID
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTOSD_Init(void)
{
	MTOSD_PRINT(" - void\n");

    //add driver implementation code here
    OSD_Init();
	return MTR_OK;
}

#if 0
/** set 3D osd L/R address
 *  @param  u4Plane                 plane id
 *  @param  pu4L_Address        L address
 *  @param  pu4R_Address        R address
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_PLA_Set3DAddress(unsigned long arg)
{
	MTAL_IOCTL_3ARG_T    rArg;
    	UINT32 u4Plane;
    	UINT32 u4L_Address,u4R_Address;

    	USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    	COPY_FROM_USER_3ARG(arg, rArg);

    	u4Plane = (UINT32)rArg.ai4Arg[0];
    	u4L_Address = (UINT32)rArg.ai4Arg[1];
    	u4R_Address = (UINT32)rArg.ai4Arg[2];

    	MTOSD_PRINT(" - u4Plane = 0x%x, u4L_Address = 0x%x, u4R_Address = 0x%x\n",
		u4Plane,
		u4L_Address,
		u4R_Address);

    	//add driver implementation code here
    	if ((-(INT32)OSD_RET_INV_PLANE)==OSD_PLA_Set3DAddress(u4Plane,u4L_Address,u4R_Address))
    	{
        	return MTR_OSD_INV_PLANE;
    	}
    	return MTR_OK;
}
#endif


/** set osd scroll information
 *  @param  u4Rgn               region id
 *  @param  u4BaseAddr          base address
 *  @param  u4Pitch             region pitch
 *  @param  u4Step              scroll step
 *
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_OSD_INV_PLANE       Invalid plane
 */
static MT_RESULT_T _MTOSD_RGN_SetScrollInfo(unsigned long arg)
{
	MTAL_IOCTL_4ARG_T    rArg;
	UINT32 u4Rgn;
	UINT32 u4BaseAddr, u4Pitch, u4Step;

	USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
	COPY_FROM_USER_4ARG(arg, rArg);

	u4Rgn = (UINT32)rArg.ai4Arg[0];
	u4BaseAddr = (UINT32)rArg.ai4Arg[1];
	u4Pitch = (UINT32)rArg.ai4Arg[2];
	u4Step = (UINT32)rArg.ai4Arg[3];

	MTOSD_PRINT(" - u4Rgn = 0x%x, u4BaseAddr = 0x%x, u4Pitch = 0x%x, u4Step = 0x%x\n",
		u4Rgn,
		u4BaseAddr,
		u4Pitch,
		u4Step );

	//add driver implementation code here
	if (((INT32)OSD_RET_OK)!=OSD_RGN_SetScrollInfo(u4Rgn,u4BaseAddr,u4Pitch,u4Step))
	{
		return MTR_NOT_OK;
	}
	
	return MTR_OK;
}


/** set osd scroll mode
 *  @param  u1ScrollMode            1: Scroll on; 0:Scroll off
 *  @retval MTR_OK                  Success
 */
static MT_RESULT_T _MTOSD_SetScrollMode(unsigned long arg)
{
	UINT8 u1ScrollMode;

	u1ScrollMode = (UINT8)arg;

	MTOSD_PRINT(" - u1ScrollMode = 0x%x\n", u1ScrollMode);

	//add driver implementation code here
	OSD_SetScrollMode(u1ScrollMode);

	return MTR_OK;
}


static MT_RESULT_T _MTOSD_PLA_Set3DMode(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    UINT8 u13DMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    u13DMode = (UINT8)rArg.ai4Arg[1];

    MTOSD_PRINT(" - u4Plane = 0x%x, u1_3DMode = 0x%x\n", u4Plane, u13DMode);

    //add driver implementation code here
    OSD_PLA_Set3DMode(u4Plane, u13DMode);

    return MTR_OK;
}


static MT_RESULT_T _MTOSD_PLA_Set3DRegion(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T    rArg;
    UINT32 u4Plane;
    UINT32 u4LeftRgn;
    UINT32 u4RightRgn;    

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    u4LeftRgn = (UINT32)rArg.ai4Arg[1];
    u4RightRgn = (UINT32)rArg.ai4Arg[2];    

    MTOSD_PRINT(" - u4Plane = 0x%x, u4LeftRgn = 0x%x, u4RightRgn = 0x%x\n",
		u4Plane,
		u4LeftRgn,u4RightRgn);

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_PLANE) == OSD_PLA_Set3DRegion(u4Plane, u4LeftRgn, u4RightRgn))
    {
        return MTR_OSD_INV_PLANE;
    }
    return MTR_OK;
}


static MT_RESULT_T _MTOSD_EnableAutoLR(unsigned long arg)
{
    UINT32 u4Enable;

    u4Enable = (UINT32)arg;


    MTOSD_PRINT(" -u4Enable = 0x%x\n", u4Enable);

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_PLANE) == OSD_BASE_EnableAutoLR(u4Enable))
    {
        return MTR_OSD_INV_PLANE;
    }
    return MTR_OK;

}


static MT_RESULT_T _MTOSD_GetLRStatus(unsigned long arg)
{
    UINT32* pu4Status;
    UINT32 u4Status;    

    pu4Status = (UINT32*)arg;

    //add driver implementation code here
    if ((-(INT32)OSD_RET_INV_PLANE) == OSD_BASE_GetLRStatus(&u4Status))
    {
        return MTR_OSD_INV_PLANE;
    }

    USR_SPACE_ACCESS_VALIDATE(pu4Status,UINT32);
    COPY_TO_USER_ARG(pu4Status, u4Status, UINT32);

    MTOSD_PRINT(" - u4Status = 0x%x\n", u4Status);
    
    return MTR_OK;

}

#ifndef CC_DFB_APP_DEVELOPMENT
//extern int  mtk_fb_set_property(unsigned int fb_id, 
	//unsigned int front_addr, unsigned int back_addr, unsigned int width, unsigned int height);
#endif

static MT_RESULT_T _MTOSD_SetFramebufferProperty(unsigned long arg)
{
	MTAL_IOCTL_5ARG_T    rArg;
       UINT32 u4Pla, u4FrontBufferPhyAddr, u4BackBufferPhyAddr, u4Width, u4Height;
       INT32 i4Ret = 0;

	USR_SPACE_ACCESS_VALIDATE_5ARG(arg);
	COPY_FROM_USER_5ARG(arg, rArg);

	u4Pla = (UINT32)rArg.ai4Arg[0];
	u4FrontBufferPhyAddr = (UINT32)rArg.ai4Arg[1];
	u4BackBufferPhyAddr = (UINT32)rArg.ai4Arg[2];
	u4Width = (UINT32)rArg.ai4Arg[3];  
	u4Height = (UINT32)rArg.ai4Arg[4];

#ifndef CC_DFB_APP_DEVELOPMENT
    //i4Ret = mtk_fb_set_property(u4Pla, 
	//u4FrontBufferPhyAddr, u4BackBufferPhyAddr, u4Width, u4Height);
#endif

    return i4Ret;

}
static MT_RESULT_T _MTOSD_BASE_SetTopPlane(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Plane;
    UINT8 u1TopPlane;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Plane = (UINT32)rArg.ai4Arg[0];
    u1TopPlane = (UINT8)rArg.ai4Arg[1];

    MTOSD_PRINT(" - u4Plane = 0x%x, u1TopPlane = 0x%x\n", u4Plane, u1TopPlane);

    //add driver implementation code here
    if(u1TopPlane ==  TRUE)
    {
      OSD_BASE_SetTopPlane(u4Plane);
     }
    else if(u1TopPlane ==  FALSE)
    	{
    	   if(u4Plane == MTOSD_PLANE_1)
    	   	{
    	   		OSD_BASE_SetTopPlane(MTOSD_PLANE_2);
    	   	}
	  else  if(u4Plane == MTOSD_PLANE_2)
    	   	{
    	   		OSD_BASE_SetTopPlane(MTOSD_PLANE_1);
    	   	}
    	}
    return MTR_OK;
}


int mtal_ioctl_mtosd(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
       int i4Ret = MTR_OK;

	switch (cmd)
	{
		case MTAL_IO_OSD_RESET:
			return _MTOSD_Reset();
		case MTAL_IO_OSD_BASE_SETOSDPOSITION:
			return _MTOSD_BASE_SetOsdPosition(arg);
		case MTAL_IO_OSD_BASE_GETOSDPOSITION:
			return _MTOSD_BASE_GetOsdPosition(arg);
		case MTAL_IO_OSD_PLA_ENABLE:
		    return _MTOSD_PLA_Enable(arg);
		case MTAL_IO_OSD_PLA_FLIPTO:
			return _MTOSD_PLA_FlipTo(arg);
		case MTAL_IO_OSD_PLA_GETBLENDLEVEL:
			return _MTOSD_PLA_GetBlendLevel(arg);
		case MTAL_IO_OSD_PLA_SETBLENDLEVEL:
		    return _MTOSD_PLA_SetBlendLevel(arg);
		case MTAL_IO_OSD_PLA_GETFADING:
			return _MTOSD_PLA_GetFading(arg);
		case MTAL_IO_OSD_PLA_SETFADING:
			return _MTOSD_PLA_SetFading(arg);
		case MTAL_IO_OSD_PLA_DUMP:
			return _MTOSD_PLA_Dump(arg);
		case MTAL_IO_OSD_PLA_GETFIRSTREGION:
		    return _MTOSD_PLA_GetFirstRegion(arg);
		case MTAL_IO_OSD_GET_PLAN_VALID_COUNTER:
		    return _MTOSD_PLA_GetValidCnt(arg);
		case MTAL_IO_OSD_WAIT_PLAN_STABLE:
			return _MTOSD_PLA_WaitStable(arg);
		case MTAL_IO_OSD_PLA_GETFIRSTREGIONADDRESS:
			return _MTOSD_PLA_GetFirstRegionAddress(arg);
		case MTAL_IO_OSD_RGN_LIST_CREATE:
			return _MTOSD_RGN_LIST_Create(arg);
		case MTAL_IO_OSD_RGN_LIST_DELETE:
			return _MTOSD_RGN_LIST_Delete(arg);
		case MTAL_IO_OSD_RGN_LIST_GETHEAD:
		    return _MTOSD_RGN_LIST_GetHead(arg);
		case MTAL_IO_OSD_RGN_GET:
			return _MTOSD_RGN_Get(arg);
		case MTAL_IO_OSD_RGN_SET:
			return _MTOSD_RGN_Set(arg);
		case MTAL_IO_OSD_RGN_CREATE:
		    return _MTOSD_RGN_Create(arg);
		case MTAL_IO_OSD_RGN_DELETE:
			return _MTOSD_RGN_Delete(arg);
		case MTAL_IO_OSD_RGN_INSERT:
			return _MTOSD_RGN_Insert(arg);
		case MTAL_IO_OSD_RGN_DETACH:
		    return _MTOSD_RGN_Detach(arg);
		case MTAL_IO_OSD_SC_SCALE:
			return _MTOSD_SC_Scale(arg);
		case MTAL_IO_OSD_SC_GETSCALERINFO:
			return _MTOSD_SC_GetScalerInfo(arg);
		case MTAL_IO_OSD_SC_SETLPF:
		    return _MTOSD_SC_SetLpf(arg);
		case MTAL_IO_OSD_GET_FB_RGN_LIST:
		    return _MTOSD_GetFBRegionList(arg);
		case MTAL_IO_OSD_GET_FB_RESOLUTION:
		    return _MTOSD_GetFBResolution(arg);
		case MTAL_IO_OSD_INIT:
			return _MTOSD_Init();
#if 0			
		case MTAL_IO_OSD_SET_3D_ADDRESS:
			return _MTOSD_PLA_Set3DAddress(arg);
#endif			
		case MTAL_IO_OSD_PLA_SET_3D_MODE:
			return _MTOSD_PLA_Set3DMode(arg);
                case MTAL_IO_OSD_SET_SCROLLINFO:
			return _MTOSD_RGN_SetScrollInfo(arg);
		case MTAL_IO_OSD_SET_SCROLL_MODE:
			return _MTOSD_SetScrollMode(arg);			

		case MTAL_IO_OSD_PLA_SET_3DREGION:
			return _MTOSD_PLA_Set3DRegion(arg);
		case MTAL_IO_OSD_ENABLE_AUTOLR:
			return _MTOSD_EnableAutoLR(arg);
		case MTAL_IO_OSD_GET_LRSTATUS:
			return _MTOSD_GetLRStatus(arg);
		case MTAL_IO_OSD_SET_PLA_LINESHIFT:	
			return _MTOSD_PLA_SetLineShift(arg);
              case MTAL_IO_OSD_SET_FRAMEBUFFER_PROPERTY:
			return _MTOSD_SetFramebufferProperty(arg);              	
		case MTAL_IO_OSD_SET_TOPPLANE:
			 return _MTOSD_BASE_SetTopPlane(arg);

		default:
		    return MTR_NOT_SUPPORTED;
	}
	return i4Ret;
}
