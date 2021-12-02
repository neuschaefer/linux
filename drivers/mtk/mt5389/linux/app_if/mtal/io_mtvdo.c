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
 * $RCSfile: io_mtvdo.c,v $
 * $Revision: #9 $
 *---------------------------------------------------------------------------*/

/** @file mtvdo.c
 *  This file contains implementation of exported APIs of Video (VDO) module.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_MT5395
#define CC_MT5365
#endif
#ifdef CC_MT5389
#define CC_MT5368
#endif

#include "vdp_if.h"
#include "video_def.h"
#include "drv_auto.h"

#ifdef CC_SUPPORT_TVE_OSD_OUT
#include "tve_ctrl.h"
#endif
#include "mtvdo.h"
#include "cb_low.h"
#include <linux/mtal_ioctl.h>

#ifndef CC_MT5392B
#include "pe_if.h"
#include "mute_if.h"
#endif

#if defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5396) ||defined(CC_MT5389) || defined(CC_MT5368)
#include "nptv_drvif.h"
#include "vdp_drvif.h"
#include "api_srch_if.h"
#include "drv_scaler.h"
#include "drv_tdtv_drvif.h"
#include "drv_di.h"
#endif

#if defined(CC_MT5396)
#include "drv_mjc_if.h"
#include "drv_display.h"
#include "nptv_drvif.h"
#endif

#if defined(CC_MT5396) || defined(CC_MT5368)
#include "vdo_misc.h"
#include "b2r_drvif.h"
#endif


#ifdef CC_MT5365
#include "drv_meter.h"
#endif

#include "osd_drvif.h"
#include "srm_drvif.h"
#include "panel.h"
#include "pe_table.h"
#include "drv_video.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_bim.h"
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
// Define definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
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
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_ARG_K(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_video valid argument error\n");     \
        return MTR_ERR_INV;                                          \
    }   

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
    
#define COPY_TO_USER_DATA(ArgUsr, ArgKernel, size)              \
    if (copy_to_user((void __user *)ArgUsr, (void *)ArgKernel,   \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_DATA(ArgUsr, ArgKernel, size)              \
    if (copy_from_user((void *)ArgKernel,(void __user *)ArgUsr,    \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return MTR_ERR_INV;                                          \
    }


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
extern UINT32 _u4OversampleDependent;  // io_mtvdecex.c

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static MT_RESULT_T _MTVDO_Init(void)
{
    MTVDO_PRINT(" - void\n");
	x_os_drv_set_timestamp("_MTVDO_Init start");

    VDP_Init();
    VdoMLInit();
	
	x_os_drv_set_timestamp("_MTVDO_Init end");
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_Stop(VOID)
{
    MTVDO_PRINT(" - void\n");

    VDP_Stop();
    
    return MTR_OK;
}


static MT_RESULT_T _MTVDO_Reset(unsigned long arg)
{
    MTVDO_PATH_T eVdpId;
    eVdpId = (MTVDO_PATH_T)arg;

    MTVDO_PRINT(" - eVdpId = %d\n", (int)eVdpId);

    VDP_Reset((UCHAR)eVdpId);
    
    return MTR_OK;
}


static MT_RESULT_T _MTVDO_SetEnable(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTVDO_PRINT(" - eVdpId = %d, fgEnable = %d\n", 
		(int)eVdpId, (int)fgEnable);

    VDP_SetEnable((UCHAR)eVdpId, (UCHAR)fgEnable);
    
    return MTR_OK;
}


static MT_RESULT_T _MTVDO_GetEnable(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    UCHAR *pucEnable;
    UCHAR ucEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    pucEnable = (UCHAR*)rArg.ai4Arg[1];

    VDP_GetEnable((UCHAR)eVdpId, &ucEnable);

    USR_SPACE_ACCESS_VALIDATE_ARG(pucEnable,UCHAR);
    COPY_TO_USER_ARG(pucEnable, ucEnable, UCHAR)
    MTVDO_PRINT(" - eVdpId = %d, ucEnable = %d\n", (int)eVdpId, (int)ucEnable);
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetMode(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    UCHAR ucMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    ucMode = (UCHAR)rArg.ai4Arg[1];

    MTVDO_PRINT(" - eVdpId = %d, ucMode = %d\n", (int)eVdpId, (int)ucMode);

    VDP_SetMode((UCHAR)eVdpId, ucMode);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_Set3dMode(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_3DMode eVdo3dMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    eVdo3dMode = (MTVDO_3DMode)rArg.ai4Arg[1];

    MTVDO_PRINT(" - eVdpId = %d, ucMode = %d\n", (int)eVdpId, (int)eVdo3dMode);
    
	vUi_TDTV_PACKING((UINT8) eVdpId, (UINT8) eVdo3dMode);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_Set2DLR(unsigned long arg)
{
    E_TDTV_UI_3D_2_2D e3D22DMode;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(arg,E_TDTV_UI_3D_2_2D);
    COPY_FROM_USER_ARG(arg,e3D22DMode,E_TDTV_UI_3D_2_2D); 

    vDrvTDTV3D22D(e3D22DMode);
        
    return MTR_OK;
}


static MT_RESULT_T _MTVDO_Set3DInverse(unsigned long arg)
{
     E_TDTV_UI_LR eTDTVLRInverse;
     
    USR_SPACE_ACCESS_VALIDATE_ARG(arg,E_TDTV_UI_LR);
    COPY_FROM_USER_ARG(arg,eTDTVLRInverse,E_TDTV_UI_LR); 

    vDrvTDTVLRInverse(eTDTVLRInverse);
        
    return MTR_OK;
}


static MT_RESULT_T _MTVDO_Set3DDepth(unsigned long arg)
{
    MTVDO_3DDepth e3DDepth;
	UINT32 u4DepthField;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(arg,MTVDO_3DDepth);
    COPY_FROM_USER_ARG(arg,e3DDepth,MTVDO_3DDepth); 
    switch ((UINT8)e3DDepth)
    {
    case 0 :
		u4DepthField =14;
		break;
	case 1:
		u4DepthField=16;
		break;
	case 2:
		u4DepthField=18;
		break;
	default :
		u4DepthField=0;
		break;
	}
    vDrvTDTVDepth( u4DepthField);
        
    return MTR_OK;
	
}

static MT_RESULT_T _MTVDO_GetISHDMIFP(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    UCHAR *pucMode;
    UCHAR ucMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    pucMode = (UCHAR*)rArg.ai4Arg[1];
	
    ucMode=(UCHAR) u4DrvTDTVGetTag3D((UINT8)eVdpId);
	
    USR_SPACE_ACCESS_VALIDATE_ARG(pucMode,UCHAR);
    COPY_TO_USER_ARG(pucMode, ucMode, UCHAR)
    MTVDO_PRINT(" - eVdpId = %d, ucMode = %d\n", (int)eVdpId, (int)ucMode);
    return MTR_OK;
}
static MT_RESULT_T _MTVDO_SetBacklightMode(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    UCHAR ucMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    ucMode = (INT16)rArg.ai4Arg[1];

    MTVDO_PRINT(" - eVdpId = %d, ucMode = %d\n", (int)eVdpId, (int)ucMode);

    VDP_SetBacklightMode((UCHAR)eVdpId, ucMode);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_GetMode(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    UCHAR *pucMode;
    UCHAR ucMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    pucMode = (UCHAR*)rArg.ai4Arg[1];

    VDP_GetMode((UCHAR)eVdpId, &ucMode);

    USR_SPACE_ACCESS_VALIDATE_ARG(pucMode,UCHAR);
    COPY_TO_USER_ARG(pucMode, ucMode, UCHAR)
    MTVDO_PRINT(" - eVdpId = %d, ucMode = %d\n", (int)eVdpId, (int)ucMode);
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetFreeze(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTVDO_PRINT(" - eVdpId = %d, fgEnable = %d\n", (int)eVdpId, (int)fgEnable);

    VDP_SetFreeze((UCHAR)eVdpId, (UCHAR)fgEnable);

    return MTR_OK;
}


static MT_RESULT_T  _MTVDO_GetFreeze(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    UCHAR *pucFreeze;
    UCHAR ucFreeze;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    pucFreeze = (UCHAR*)rArg.ai4Arg[1];

    VDP_GetFreeze((UCHAR)eVdpId, &ucFreeze);
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pucFreeze,UCHAR);
    COPY_TO_USER_ARG(pucFreeze, ucFreeze, UCHAR)
    MTVDO_PRINT(" - eVdpId = %d, ucFreeze = %d\n", (int)eVdpId, (int)ucFreeze);
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetDotByDot(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTVDO_PRINT(" - eVdpId = %d, fgEnable = %d\n", (int)eVdpId, (int)fgEnable);

    VDP_Set121Map((UCHAR)eVdpId, (UCHAR)fgEnable);

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetNonLinearMode(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    UINT8 u1Mode;
    VDP_AUTO_NONLINEAR_PRM_T rNonlinearAutoPara;
    VDP_NONLINEAR_PRM_T rNonlinearManualPara;
    MTVDO_NONLINEAR_PRM_T rNonlinearPara;
    MTVDO_NONLINEAR_PRM_T *prNonlinearPara;
	VDP_NEW_NONLINEAR_PRM_T rNonlPrm;

    x_memset(&rNonlinearPara, 0, sizeof(MTVDO_NONLINEAR_PRM_T));
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    u1Mode = (UINT8)rArg.ai4Arg[1];
    prNonlinearPara = (MTVDO_NONLINEAR_PRM_T*)rArg.ai4Arg[2];

    if(prNonlinearPara != NULL)
    {
    	USR_SPACE_ACCESS_VALIDATE_ARG(prNonlinearPara, MTVDO_NONLINEAR_PRM_T);
    	COPY_FROM_USER_ARG(prNonlinearPara, rNonlinearPara, MTVDO_NONLINEAR_PRM_T);
    }
    MTVDO_PRINT(" - eVdpId = %d, u1Mode = %d\n", (int)eVdpId, (int)u1Mode);
    MTVDO_PRINT(" - rNonlinearPara start= %d, end = %d, slope = %d, middle = %d\n", 
    	(int)rNonlinearPara.u4Start, (int)rNonlinearPara.u4End, (int)rNonlinearPara.u4Slope, (int)rNonlinearPara.u4MiddlePoint);

    switch(u1Mode)
    {
        case MTVDO_NONLINEAR_OFF:
        	VDP_SetNonlinear((UCHAR)eVdpId, (UCHAR)FALSE);
        	break;
	 case MTVDO_NONLINEAR_AUTO_1:
		rNonlinearAutoPara.u1Mode = MTVDO_NONLINEAR_AUTO_1;
		rNonlinearAutoPara.u4Adaptive =0; //no use
        	rNonlinearAutoPara.u4EndFactor =rNonlinearPara.u4End;
        	rNonlinearAutoPara.u4Slope = rNonlinearPara.u4Slope;
        	VDP_SetNonlinear((UCHAR)eVdpId, (UCHAR)TRUE);
        	VDP_SetAutoNonLinear((UINT32)eVdpId, rNonlinearAutoPara);
        	break;
        case MTVDO_NONLINEAR_AUTO:
		rNonlinearAutoPara.u1Mode = MTVDO_NONLINEAR_AUTO;
        	rNonlinearAutoPara.u4Adaptive =0; //no use
        	rNonlinearAutoPara.u4EndFactor =rNonlinearPara.u4End;
        	rNonlinearAutoPara.u4Slope = rNonlinearPara.u4Slope;
        	VDP_SetNonlinear((UCHAR)eVdpId, (UCHAR)TRUE);
        	VDP_SetAutoNonLinear((UINT32)eVdpId, rNonlinearAutoPara);
        	break;
        case MTVDO_NONLINEAR_MANUAL:
        	rNonlinearManualPara.u4End=rNonlinearPara.u4End;
        	rNonlinearManualPara.u4Slope = rNonlinearPara.u4Slope;
        	rNonlinearManualPara.u4MiddlePoint = rNonlinearPara.u4MiddlePoint;
        	rNonlinearManualPara.u4Start= rNonlinearPara.u4Start;
        	VDP_SetNonlinear((UCHAR)eVdpId, (UCHAR)TRUE);
        	VDP_SetManualNonLinear((UINT32)eVdpId, rNonlinearManualPara);
        	break;
		case MTVDO_NONLINEAR_NEW:
			rNonlPrm.u4SrcMiddleRatio = rNonlinearPara.u4MiddlePoint;
			rNonlPrm.u4OutMiddleRatio = rNonlinearPara.u4Slope;
			VDP_SetNonlinear((UCHAR)eVdpId, (UCHAR)TRUE);
			VDP_SetNewNonLinear((UCHAR)eVdpId, rNonlPrm);
			break;
        default:
        	return MTR_NOT_OK;
    }
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetArg(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_ARG_TYPE_T eArgType;
    INT16 i2Value;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    eArgType = (MTVDO_ARG_TYPE_T)rArg.ai4Arg[1];
    i2Value = (INT16)rArg.ai4Arg[2];

    MTVDO_PRINT(" - eVdpId = %d, eArgType = %d, i2Value = 0x%x\n", 
        (int)eVdpId, 
        (int)eArgType,
        i2Value);   

    PE_SetArg((UCHAR)eVdpId, (UCHAR)eArgType, i2Value);

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_GetArg(unsigned long arg)
{

    MTAL_IOCTL_3ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_ARG_TYPE_T eArgType;
    INT16 *pi2Value;
    INT16 i2Value;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    eArgType = (MTVDO_ARG_TYPE_T)rArg.ai4Arg[1];
    pi2Value = (INT16*)rArg.ai4Arg[2];

    PE_GetArg((UCHAR)eVdpId, (UCHAR)eArgType, &i2Value);

    USR_SPACE_ACCESS_VALIDATE_ARG(pi2Value,INT16);
    COPY_TO_USER_ARG(pi2Value, i2Value, INT16);
    MTVDO_PRINT(" - eVdpId = %d, eArgType = %d, i2Value = 0x%x\n", 
        (int)eVdpId, 
        (int)eArgType, 
        i2Value);

    return MTR_OK;
}


static MT_RESULT_T _MTVDO_SetOverScan(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_OVERSCAN_REGION_T *prRegion;
    MTVDO_OVERSCAN_REGION_T rRegion;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    prRegion = (MTVDO_OVERSCAN_REGION_T*)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG(prRegion, MTVDO_OVERSCAN_REGION_T);
    COPY_FROM_USER_ARG(prRegion, rRegion, MTVDO_OVERSCAN_REGION_T);

    MTVDO_PRINT(" - eVdpId = %d\n"
	 "rRegion.u4Top = %d\n"
	 "rRegion.u4Bottom = %d\n"
        "rRegion.u4Left = %d\n"
        "rRegion.u4Right = %d\n", 
        eVdpId,
        (int)rRegion.u4Top,
        (int)rRegion.u4Bottom,
        (int)rRegion.u4Left,
        (int)rRegion.u4Right);

    VDP_SetOverScan((UCHAR)eVdpId, *(VDP_OVERSCAN_REGION_T*)&rRegion);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_GetOverScan(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_OVERSCAN_REGION_T *prRegion;
    MTVDO_OVERSCAN_REGION_T rRegion;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    prRegion = (MTVDO_OVERSCAN_REGION_T*)rArg.ai4Arg[1];

    VDP_GetOverScan((UCHAR)eVdpId, (VDP_OVERSCAN_REGION_T*)&rRegion);
    
    USR_SPACE_ACCESS_VALIDATE_ARG(prRegion, MTVDO_OVERSCAN_REGION_T);
    COPY_TO_USER_ARG(prRegion, rRegion, MTVDO_OVERSCAN_REGION_T);

    MTVDO_PRINT(" - eVdpId = %d\n"
	 "rRegion.u4Top = %d\n"
	 "rRegion.u4Bottom = %d\n"
        "rRegion.u4Left = %d\n"
        "rRegion.u4Right = %d\n", 
        eVdpId,
        (int)rRegion.u4Top,
        (int)rRegion.u4Bottom,
        (int)rRegion.u4Left,
        (int)rRegion.u4Right);

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetSrcRegion(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_REGION_T *prSrcRegion;
    MTVDO_REGION_T rSrcRegion;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    prSrcRegion = (MTVDO_REGION_T*)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG(prSrcRegion, MTVDO_REGION_T);
    COPY_FROM_USER_ARG(prSrcRegion, rSrcRegion, MTVDO_REGION_T);

    MTVDO_PRINT(" - eVdpId = %d\n"
	 "rSrcRegion.u4Y = %d\n"
	 "rSrcRegion.u4Y = %d\n"
        "rSrcRegion.u4Width = %d\n"
        "rSrcRegion.u4Height = %d\n", 
        eVdpId,
        (int)rSrcRegion.u4X,
        (int)rSrcRegion.u4Y,
        (int)rSrcRegion.u4Width,
        (int)rSrcRegion.u4Height);    

    if (VDP_GetScalerPixelBased())
    {
        if (bDrvVideoGetWidthOverSample(eVdpId) && (_u4OversampleDependent == 0))
        {
            rSrcRegion.u4X = rSrcRegion.u4X << 1;
            rSrcRegion.u4Width = rSrcRegion.u4Width << 1;            
        }
    }
    VDP_SetSrcRegion((UCHAR)eVdpId, (UCHAR)0, *(VDP_REGION_T*)&rSrcRegion);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_GetSrcRegion(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_REGION_T *prSrcRegion;
    MTVDO_REGION_T rSrcRegion;
    UCHAR ucSrcFullRegion = 0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    prSrcRegion = (MTVDO_REGION_T*)rArg.ai4Arg[1];

    VDP_GetSrcRegion((UCHAR)eVdpId, &ucSrcFullRegion, (VDP_REGION_T*)&rSrcRegion);

    MTVDO_PRINT(" - eVdpId = %d\n"
	 "rSrcRegion.u4Y = %d\n"
	 "rSrcRegion.u4Y = %d\n"
        "rSrcRegion.u4Width = %d\n"
        "rSrcRegion.u4Height = %d\n", 
        eVdpId,
        (int)rSrcRegion.u4Y,
        (int)rSrcRegion.u4Y,
        (int)rSrcRegion.u4Width,
        (int)rSrcRegion.u4Height);
    
    if (VDP_GetScalerPixelBased())
    {
        if (bDrvVideoGetWidthOverSample(eVdpId) && (_u4OversampleDependent == 0))
        {
            rSrcRegion.u4X = rSrcRegion.u4X >> 1;
            rSrcRegion.u4Width = rSrcRegion.u4Width >> 1;            
        }
    }    

    USR_SPACE_ACCESS_VALIDATE_ARG(prSrcRegion, MTVDO_REGION_T);
    COPY_TO_USER_ARG(prSrcRegion, rSrcRegion, MTVDO_REGION_T);

    return MTR_OK;
}


static MT_RESULT_T _MTVDO_SetOutRegion(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_REGION_T *prOutRegion;
    MTVDO_REGION_T rOutRegion;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    prOutRegion = (MTVDO_REGION_T*)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG(prOutRegion, MTVDO_REGION_T);
    COPY_FROM_USER_ARG(prOutRegion, rOutRegion, MTVDO_REGION_T);

    MTVDO_PRINT(" - eVdpId = %d\n"
	 "rOutRegion.u4Y = %d\n"
	 "rOutRegion.u4Y = %d\n"
        "rOutRegion.u4Width = %d\n"
        "rOutRegion.u4Height = %d\n", 
        eVdpId,
        (int)rOutRegion.u4Y,
        (int)rOutRegion.u4Y,
        (int)rOutRegion.u4Width,
        (int)rOutRegion.u4Height);  
    
    VDP_SetOutRegion((UCHAR)eVdpId, (UCHAR)0, *(VDP_REGION_T*)&rOutRegion);

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_GetOutRegion(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_REGION_T *prOutRegion;
    MTVDO_REGION_T rOutRegion;
    UCHAR ucOutFullRegion = 0;    

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    prOutRegion = (MTVDO_REGION_T*)rArg.ai4Arg[1];

    VDP_GetOutRegion((UCHAR)eVdpId, &ucOutFullRegion, (VDP_REGION_T*)&rOutRegion);

    USR_SPACE_ACCESS_VALIDATE_ARG(prOutRegion, MTVDO_REGION_T);
    COPY_TO_USER_ARG(prOutRegion, rOutRegion, MTVDO_REGION_T);

    MTVDO_PRINT(" - eVdpId = %d\n"
	 "rOutRegion.u4Y = %d\n"
	 "rOutRegion.u4Y = %d\n"
        "rOutRegion.u4Width = %d\n"
        "rOutRegion.u4Height = %d\n", 
        eVdpId,
        (int)rOutRegion.u4Y,
        (int)rOutRegion.u4Y,
        (int)rOutRegion.u4Width,
        (int)rOutRegion.u4Height);    

    return MTR_OK;
}
static MT_RESULT_T _MTVDO_SetOutputWindow(unsigned long arg)
{
	MTAL_IOCTL_2ARG_T rArg;
	MTVDO_PATH_T eVdpId;
	MTVDO_REGION_T *prOutRegion;
	MTVDO_REGION_T rOutRegion;
	MTVDO_REGION_T rSrcRegion;
	
	USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
	COPY_FROM_USER_2ARG(arg, rArg);
	eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
	prOutRegion = (MTVDO_REGION_T*)rArg.ai4Arg[1];
	
	USR_SPACE_ACCESS_VALIDATE_ARG(prOutRegion, MTVDO_REGION_T);
	COPY_FROM_USER_ARG(prOutRegion, rOutRegion, MTVDO_REGION_T);

	rSrcRegion = rOutRegion;

	MTVDO_PRINT(" - eVdpId = %d\n"
	"rOutRegion.u4Y = %d\n"
	"rOutRegion.u4Y = %d\n"
	"rOutRegion.u4Width = %d\n"
	"rOutRegion.u4Height = %d\n", 
		eVdpId,
		(int)rOutRegion.u4Y,
		(int)rOutRegion.u4Y,
		(int)rOutRegion.u4Width,
		(int)rOutRegion.u4Height);	

	if (VDP_GetScalerPixelBased())
    {
        if (bDrvVideoGetWidthOverSample(eVdpId) && (_u4OversampleDependent == 0))
        {
            rSrcRegion.u4X = rSrcRegion.u4X << 1;
            rSrcRegion.u4Width = rSrcRegion.u4Width << 1;            
        }
    }
    VDP_SetSrcRegion((UCHAR)eVdpId, (UCHAR)1, *(VDP_REGION_T*)&rSrcRegion);
	VDP_SetOutRegion((UCHAR)eVdpId, (UCHAR)0, *(VDP_REGION_T*)&rOutRegion);  

	return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetMuteOnOff(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4VdpId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    u4VdpId = (UINT32)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTVDO_PRINT(" - eVdpId = %ul, fgEnable = %d\n", u4VdpId, fgEnable);

    VDP_SetMuteOnOff((UINT32)u4VdpId, (UINT8)fgEnable);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetBg(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    UINT32 u4BgColor;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    u4BgColor = (UINT32)rArg.ai4Arg[1];

    MTVDO_PRINT(" -eVdpId = %d, u4BgColor = 0x%ul \n", eVdpId, u4BgColor);

    VDP_SetBg((UCHAR)eVdpId, u4BgColor);
    
    return MTR_OK;
}


static MT_RESULT_T _MTVDO_GetBg(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    UINT32 *pu4BgColor;
    UINT32 u4BgColor;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    pu4BgColor = (UINT32*)rArg.ai4Arg[1];

    VDP_GetBg((UCHAR)eVdpId, &u4BgColor);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4BgColor, UINT32);
    COPY_TO_USER_ARG(pu4BgColor, u4BgColor, UINT32);

    MTVDO_PRINT(" -eVdpId = %d, u4BgColor = 0x%ul\n", eVdpId, u4BgColor);
    return MTR_OK;
}


static void _VdoTDNaviChgCb(UINT32 u4VdpId, UINT32 u4NaviTag)
{
    MTVDO_CB_T  rVdoCbT;

    rVdoCbT.u4Arg1 = u4VdpId;
    rVdoCbT.u4Arg2 = u4NaviTag;
    
    _CB_PutEvent(CB_MTAL_VDO_TDNAVI_CHG, sizeof(MTVDO_CB_T), (void*)&rVdoCbT);
}

static void _VdoSrcChgCb(UINT32 u4VdpId, VDP_SOURCE_INFO_T rSrcInfo)
{
    MTVDO_CB_T  rVdoCbT;

    rVdoCbT.u4Arg1 = u4VdpId;
    rVdoCbT.u4Arg2 = rSrcInfo.u4SrcWidth;
    rVdoCbT.u4Arg3 = rSrcInfo.u4SrcHeight;
    rVdoCbT.u4Arg4 = rSrcInfo.u4OutWidth;
    rVdoCbT.u4Arg5 = rSrcInfo.u4OutWidth;
    rVdoCbT.u4Arg6 = rSrcInfo.u4FrameRate;
    rVdoCbT.u4Arg7 = rSrcInfo.u4Interlace;    
    
    _CB_PutEvent(CB_MTAL_VDO_SRCCHG_TRIGGER, sizeof(MTVDO_CB_T), (void*)&rVdoCbT);
}

static void _VdoPtsCb(UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Arg)
{
    MTVDO_CB_T  rVdoCbT;

    rVdoCbT.u4Arg1 = u4VdpId;
    rVdoCbT.u4Arg2 = u4Pts;
    rVdoCbT.u4Arg3 = u4Arg;
    
    _CB_PutEvent(CB_MTAL_VDO_PTS_TRIGGER, sizeof(MTVDO_CB_T), (void*)&rVdoCbT);
}


#if !defined(CC_MT5396) && !defined(CC_MT5368)
static void _VdoAfdCb(UINT32 u4VdpId, UINT32 u4Afd, FBM_ASPECT_RATIO_T* prAspectRatio)
{
    MTVDO_CB_T  rVdoCbT;

    rVdoCbT.u4Arg1 = u4VdpId;
    rVdoCbT.u4Arg2 = u4Afd;
    rVdoCbT.u4Arg3 = prAspectRatio->u1AspectRatioType;

    _CB_PutEvent(CB_MTAL_VDO_AFD_TRIGGER, sizeof(MTVDO_CB_T), (void*)&rVdoCbT);
}
#endif


static void _VdoUnMuteCb(UINT32 u4VdpId, BOOL fgUnMute)
{
    MTVDO_CB_T  rVdoCbT;

    rVdoCbT.u4Arg1 = u4VdpId;
    rVdoCbT.u4Arg2 = (UINT32)fgUnMute;

    _CB_PutEvent(CB_MTAL_VDO_UNMUTE_TRIGGER, sizeof(MTVDO_CB_T), (void*)&rVdoCbT);
}


static void _VdoLipSyncCb(void)
{
    MTVDO_CB_T  rVdoCbT;

    _CB_PutEvent(CB_MTAL_VDO_LIPSYNC_TRIGGER, sizeof(MTVDO_CB_T), (void*)&rVdoCbT);
}

static void _VdoLipSyncNg2OkCb(void)
{
    MTVDO_CB_T  rVdoCbT;

    _CB_PutEvent(CB_MTAL_VDO_LIPSYNC_NG_TO_OK_TRIGGER, sizeof(MTVDO_CB_T), (void*)&rVdoCbT);
}

static void _VdoLipSyncOk2NgCb(void)
{
    MTVDO_CB_T  rVdoCbT;

    _CB_PutEvent(CB_MTAL_VDO_LIPSYNC_OK_TO_NG_TRIGGER, sizeof(MTVDO_CB_T), (void*)&rVdoCbT);
}

void _VdoVSSCb(void)
{
    MTVDO_CB_T  rVdoCbT;

    _CB_PutEvent(CB_MTAL_VDO_VSS_TRIGGER, sizeof(MTVDO_CB_T), (void*)&rVdoCbT);
}

#if !defined(CC_MT5396) && !defined(CC_MT5368) 
static void _OutVSyncNfy(void)
{
    MTVDO_CB_T  rVdoCbT;
    
    OSD_Vsync_UpdateScroll();    
    _CB_PutEvent(CB_MTAL_VDO_OUT_VSYNC_TRIGGER, sizeof(MTVDO_CB_T), (void*)&rVdoCbT);
}
#endif

static VDP_CB_FUNC_ENUM_T _MTVDO_CBEnum_to_VDPCBEnum(MTVDO_CB_FUNC_ENUM_T eMTVDOEnum)
{
    VDP_CB_FUNC_ENUM_T eReturn = VDP_CB_FUNC_NS;

    switch (eMTVDOEnum)
    {
        case VDO_CB_FUNC_SRC_CHG_IND:
			eReturn = VDP_CB_FUNC_SRC_CHG_IND_2;
            break;

        case VDO_CB_FUNC_PTS_IND:
            eReturn = VDP_CB_FUNC_PTS_IND;
            break;

        case VDO_CB_FUNC_AFD_IND:
            eReturn = VDP_CB_FUNC_AFD_IND_2;
            break;

        case VDO_CB_FUNC_UNMUTE_IND:
            eReturn = VDP_CB_FUNC_UNMUTE_IND;
            break;

        case VDO_CB_FUNC_LIPSYNC_IND:
            eReturn = VDP_CB_FUNC_LIPSYNC_IND;
            break;
            
        case VDO_CB_FUNC_LIPSYNC_OK_TO_NG_IND:
            eReturn = VDP_CB_FUNC_LIPSYNC_OK_TO_NG_IND;
            break;
            
        case VDO_CB_FUNC_LIPSYNC_NG_TO_OK_IND:
            eReturn = VDP_CB_FUNC_LIPSYNC_NG_TO_OK_IND;
            break;
            
        case VDO_CB_FUNC_VSS_FIN_IND:
            eReturn = VDP_CB_FUNC_VSS_FIN_IND;
            break;
            
        case VDO_CB_FUNC_TDNAVI_IND:
            eReturn = VDP_CB_FUNC_TDNAVI_IND;
            break;
            
        default:
            eReturn = VDP_CB_FUNC_NS;
            break;
    }

    return eReturn;
}

static MT_RESULT_T _MTVDO_RegCbFunc(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    MTVDO_CB_FUNC_ENUM_T eFuncType;
    UINT32 u4FuncPtr;
    UINT32 u4Arg1;
    UINT32 u4Arg2;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);
    eFuncType = (MTVDO_CB_FUNC_ENUM_T)rArg.ai4Arg[0];
    u4FuncPtr = (UINT32)rArg.ai4Arg[1];
    u4Arg1 = (UINT32)rArg.ai4Arg[2];
    u4Arg2 = (UINT32)rArg.ai4Arg[3];

    MTVDO_PRINT(" - eFuncType = %d, u4FuncPtr = %ul, u4Arg1 = %ul, u4Arg2 = %ul\n",
        eFuncType, u4FuncPtr, u4Arg1, u4Arg2);

    switch (eFuncType)
    {
        case VDO_CB_FUNC_SRC_CHG_IND:
            VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)_VdoSrcChgCb, u4Arg1, u4Arg2);
            break;

        case VDO_CB_FUNC_PTS_IND:
            VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)_VdoPtsCb, u4Arg1, u4Arg2);
            break;

        case VDO_CB_FUNC_AFD_IND:
            #if !defined(CC_MT5396) && !defined(CC_MT5368) 
            VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)_VdoAfdCb, u4Arg1, u4Arg2);
            #endif
            break;

        case VDO_CB_FUNC_UNMUTE_IND:
            VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)_VdoUnMuteCb, u4Arg1, u4Arg2);
            break;

        case VDO_CB_FUNC_LIPSYNC_IND:
            VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)_VdoLipSyncCb, u4Arg1, u4Arg2);
            break;
            
        case VDO_CB_FUNC_LIPSYNC_OK_TO_NG_IND:
            if((u4Arg1==0) && (u4Arg2==0))
            {
                /* Remove register callback */
                VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)NULL, u4Arg1, u4Arg2);
            }
            else
            {
                VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)_VdoLipSyncOk2NgCb, u4Arg1, u4Arg2);
            }
            break;
            
        case VDO_CB_FUNC_LIPSYNC_NG_TO_OK_IND:
            if((u4Arg1==0) && (u4Arg2==0))
            {
                /* Remove register callback */
                VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)NULL, u4Arg1, u4Arg2);
            }
            else
            {
                VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)_VdoLipSyncNg2OkCb, u4Arg1, u4Arg2);
            }
            break;

        case VDO_CB_FUNC_OUT_VSYNC_IND:
              #if defined(CC_MT5392B) || defined(CC_MT5365) || defined(CC_MT5395)
        	NPTV_SetOutVSyncNfyFct((UINT32)_OutVSyncNfy);
              #else
              return MTR_NOT_OK;
              #endif
        	break;

        case VDO_CB_FUNC_VSS_FIN_IND:
            VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)_VdoVSSCb, u4Arg1, u4Arg2);
            break;

        case VDO_CB_FUNC_TDNAVI_IND:
            VDP_RegCbFunc(_MTVDO_CBEnum_to_VDPCBEnum(eFuncType), (UINT32)_VdoTDNaviChgCb, u4Arg1, u4Arg2);
            break;
                    
        default:
            return MTR_NOT_OK;
            break;
    }
    
    return MTR_OK;
}


static MT_RESULT_T _MTVDO_SetGameMode(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4VdpId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    u4VdpId = (UINT32)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTVDO_PRINT(" - eVdpId = %ul, fgEnable = %d\n", u4VdpId, fgEnable);

    VDP_SetGameMode((UINT32)u4VdpId, (UINT8)fgEnable);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetHDMITrueBlack(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4VdpId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    u4VdpId = (UINT32)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTVDO_PRINT(" - eVdpId = %ul, fgEnable = %d\n", u4VdpId, fgEnable);

    VDP_SetHDMITrueBlack((UINT32)u4VdpId, (UINT8)fgEnable);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_Set3DGameMode(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4VdpId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    u4VdpId = (UINT32)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTVDO_PRINT(" - eVdpId = %ul, fgEnable = %d\n", u4VdpId, fgEnable);

    VDP_Set3DGameMode((UINT32)u4VdpId, (UINT8)fgEnable);

    return MTR_OK;
}


static MT_RESULT_T _MTVDO_GetUnMuteStatus (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    BOOL *pfgEnable;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    pfgEnable = (BOOL*)rArg.ai4Arg[1];

    fgEnable = VDP_GetUnMuteStatus((UCHAR)eVdpId);

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgEnable, BOOL);
    COPY_TO_USER_ARG(pfgEnable, fgEnable, BOOL);

    MTVDO_PRINT(" -eVdpId = %d, fgEnable = %d\n", eVdpId, fgEnable);
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_Scalar_EnableGetBuffer(unsigned long arg)
{
    BOOL fgEnable;
    UINT8 u1Ret;
    fgEnable = (BOOL)arg;
    
    MTVDO_PRINT(" - fgEnable = %d\n", fgEnable);

#ifdef CC_MT5392B
    vSupportScalerDramDump(VDP_1, fgEnable);
    return MTR_OK;
#elif defined(CC_MT5395) || defined(CC_MT5365)

    if(fgForceScalerDramDump(VDP_1, fgEnable))
    {
        u1Ret = MTR_OK;
    }
    else
    {
        u1Ret = MTR_NOT_SUPPORTED;
    }

    MTVDO_PRINT(" - u1Ret = %d\n", u1Ret);
#else
    u1Ret = MTR_NOT_SUPPORTED;
#endif
    return u1Ret;    
}

static MT_RESULT_T _MTVDO_Scalar_GetBufferAverage(unsigned long arg)
{
    UINT8 u1Ret;
    #if defined(CC_MT5392B) || defined(CC_MT5395) || defined(CC_MT5365)
    MTVDO_REGION_T rRegion;
    MTVDO_REGION_T *prRegion;
    MTVDO_SCLAR_BUFFER_DATA rData;
    MTVDO_SCLAR_BUFFER_DATA *prData;

    MTAL_IOCTL_2ARG_T    rArg;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    prRegion= (MTVDO_REGION_T*) rArg.ai4Arg[0];
    prData= (MTVDO_SCLAR_BUFFER_DATA*) rArg.ai4Arg[1];
    USR_SPACE_ACCESS_VALIDATE_ARG(prRegion, MTVDO_REGION_T);
    COPY_FROM_USER_ARG(prRegion, rRegion, MTVDO_REGION_T);

    //add driver implementation code here
    UNUSED(bInitScalerDump());       
    u1Ret = bGetScalerDRAMAverage(rRegion.u4X, rRegion.u4Y, rRegion.u4Width, rRegion.u4Height,  
    									&(rData.u4VR),  &(rData.u4UB),  &(rData.u4YG));
    UNUSED(bReleaseScalerDump());

    USR_SPACE_ACCESS_VALIDATE_ARG(prData, MTVDO_SCLAR_BUFFER_DATA);
    COPY_TO_USER_ARG(prData, rData, MTVDO_SCLAR_BUFFER_DATA);

    MTVDO_PRINT(" - u2VR = 0x%ul ,u2UB = 0x%ul, u2YG = 0x%ul \n", rData.u4VR, rData.u4UB, rData.u4YG);
    MTVDO_PRINT(" - u1Ret = %d\n", u1Ret);

    #else
    u1Ret = FALSE;
    #endif

    if(u1Ret == FALSE)
    	return MTR_NOT_OK;
    else
        return MTR_OK;
}

static MT_RESULT_T _MTVDO_GetTimingInfo(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;

    MTVDO_TIMING_INFO_T *prTiming;
    MTVDO_TIMING_INFO_T rTiming;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    prTiming = (MTVDO_OVERSCAN_REGION_T*)rArg.ai4Arg[1];

    // get timing info
    rTiming.u4HTotal = (UINT32)wDrvVideoGetHTotal(eVdpId);
    rTiming.u4VTotal = (UINT32)wDrvVideoGetVTotal(eVdpId);
    rTiming.u4FrameRate = (UINT32)bDrvVideoGetRefreshRate(eVdpId);
#ifdef CC_MT5392B
    rTiming.u4HStart = (UINT32)wTVDecGetHPos(eVdpId);
    rTiming.u4VStart = (UINT32)wTVDecGetVPos(eVdpId);
#else
    rTiming.u4HStart = (UINT32)wDrvVideoGetPorch(eVdpId, SV_HPORCH_CURRENT);
    rTiming.u4VStart = (UINT32)wDrvVideoGetPorch(eVdpId, SV_VPORCH_CURRENT);
#endif
    rTiming.u4ActiveWidth = (UINT32)wDrvVideoInputWidth(eVdpId);
    if (bDrvVideoGetWidthOverSample(eVdpId) && (_u4OversampleDependent == 0))
    {
        rTiming.u4ActiveWidth = rTiming.u4ActiveWidth >> 1;
        rTiming.u4HStart= rTiming.u4HStart >> 1;//cheerz        
    }
    rTiming.u4ActiveHeight = (UINT32)wDrvVideoInputHeight(eVdpId); 
    rTiming.u4Interlace = (UINT32)bDrvVideoIsSrcInterlace(eVdpId);
    if(rTiming.u4Interlace)//cheerz
    {
        rTiming.u4VStart = rTiming.u4VStart << 1;
    }
    
    #if defined(CC_MT5368) || defined(CC_MT5396)
    if (bGetVideoDecType((SV_VP_T)eVdpId) == SV_VD_VGA)
    #else
    if (bGetVideoDecType(eVdpId) == SV_VD_VGA)
    #endif
    {
        rTiming.u4Phase = u1DrvVGAGetPhase();
    }
    else
    {
        rTiming.u4Phase = 0; 
    }
    
    USR_SPACE_ACCESS_VALIDATE_ARG(prTiming, MTVDO_TIMING_INFO_T);
    COPY_TO_USER_ARG(prTiming, rTiming, MTVDO_TIMING_INFO_T);

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetFactoryMode(unsigned long arg)
{
    BOOL fgEnable;

    fgEnable = (BOOL)arg;
    
    MTVDO_PRINT(" - fgEnable = %d\n", fgEnable);
#ifdef CC_SUPPORT_TVE_OSD_OUT
    vApiTVESetOSDOutMode((UINT8)fgEnable);
#endif    

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetAutoMuteOnOff(unsigned long  arg)
{
    BOOL fgOnOff;
    fgOnOff = ((BOOL)arg)? FALSE: TRUE;

#ifdef CC_MT5392B
#ifdef CC_SCPOS_FORCE_UNMUTE
    vScposForceUnMute(fgOnOff);
#else
    UNUSED(fgOnOff);
#endif
#else
    _vDrvVideoForceUnMute(VDP_1, fgOnOff);
    _vDrvVideoForceUnMute(VDP_2, fgOnOff);
#endif
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetScalerPixelBased(unsigned long arg) // drmyung 081226
{
    BOOL fgEnable;

    fgEnable = (BOOL)arg;

    VDP_SetScalerPixelBased(fgEnable);

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetxvYCCInnerPatGen(unsigned long arg)
{
#if defined(CC_MT5396) ||defined(CC_MT5389) || defined(CC_MT5368)	
		// xvYCC Pattern Gen Removed in 96 or Later
#else
    MTAL_IOCTL_4ARG_T rArg;
    UINT8 bOnOff;
    UINT16 wPatGenR;
    UINT16 wPatGenG;
    UINT16 wPatGenB;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);
    
    bOnOff = (UINT8)rArg.ai4Arg[0];
    wPatGenR = (UINT16)rArg.ai4Arg[1];
    wPatGenG = (UINT16)rArg.ai4Arg[2];
    wPatGenB = (UINT16)rArg.ai4Arg[3];

    vDrvSetxvYCCPatGenRGB(wPatGenR, wPatGenG, wPatGenB);
    vDrvSetxvYCCPatGenOnOff(bOnOff);    
#endif
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetPeUiRangeParam(unsigned long arg)
{    
    MTAL_IOCTL_3ARG_T rArg;
    MTVDO_UI_RANGE_T  rUIParam;	
    UINT8 eVdpId;	
    PE_ARG_TYPE eUiVqItem;    
    UINT8 u1NptvSrcType, u1VdpVisType;
    MTVDO_UI_RANGE_T* prUIParam;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    
    eVdpId = (UINT8) rArg.ai4Arg[0];
    eUiVqItem = (PE_ARG_TYPE) rArg.ai4Arg[1];
    prUIParam = (MTVDO_UI_RANGE_T*) rArg.ai4Arg[2];

    u1NptvSrcType = bApiVideoGetSrcType(eVdpId);
    u1VdpVisType = u1ApiVidInSrcType2VisType(u1NptvSrcType);
    
    COPY_FROM_USER_DATA(prUIParam, &rUIParam, sizeof(MTVDO_UI_RANGE_T));
    PE_SetUiRange(u1VdpVisType, eUiVqItem, (PE_UI_RANGE_T *)&rUIParam);	

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_GetPeUiRangeParam(unsigned long arg)
{    
    MTAL_IOCTL_3ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_UI_RANGE_T  rUIParam;        
    MTVDO_UI_RANGE_T* prUIParam;
    PE_ARG_TYPE eUiVqItem;
    
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);    
    
    eVdpId = (MTVDO_PATH_T) rArg.ai4Arg[0];
    eUiVqItem = (PE_ARG_TYPE) rArg.ai4Arg[1];
    prUIParam = (MTVDO_UI_RANGE_T*) rArg.ai4Arg[2];
    
    PE_GetUiRange(eVdpId, eUiVqItem, (PE_UI_RANGE_T *)&rUIParam);	        
    COPY_TO_USER_DATA(prUIParam, &rUIParam, sizeof(MTVDO_UI_RANGE_T));

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_GetPcfPqfInfo(unsigned long arg)
{    
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_PCF_PQF_INFO_T rPcfPqfInfo;
    MTVDO_PCF_PQF_INFO_T *pPcfPqfInfo;


    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    pPcfPqfInfo = (UCHAR*)rArg.ai4Arg[1];

    bApiGetPcfPqfVersion(&rPcfPqfInfo);
    COPY_TO_USER_DATA(pPcfPqfInfo, &rPcfPqfInfo, sizeof(MTVDO_PCF_PQF_INFO_T));

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_GetPtfInfo(unsigned long arg)
{    
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    PTF_INFO rPtfInfo;
    PTF_INFO *pPtfInfo;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    pPtfInfo = (UCHAR*)rArg.ai4Arg[1];

    bApiGetPtfVersion((UINT8 *)&rPtfInfo);
    COPY_TO_USER_DATA(pPtfInfo, &rPtfInfo, sizeof(PTF_INFO));

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetPWM(unsigned long arg)
{
    UINT8 u1PWM;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(arg,UINT8);
    COPY_FROM_USER_ARG(arg,u1PWM,UINT8); 

    vApiSetPanelBright(u1PWM);
        
    return MTR_OK;
}


static MT_RESULT_T _MTVDO_SetScanMode(unsigned long arg)
{
    UINT32 u4scan;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(arg,UINT32);
    COPY_FROM_USER_ARG(arg,u4scan,UINT32); 
    
 #ifdef CC_MT5365
	  vSetAutoSearchType(u4scan);
 #endif
	
	return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetDSFactor(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    UINT32 u4Factor;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    u4Factor = (UINT32)rArg.ai4Arg[1];

    MTVDO_PRINT("  - eVdpId = %d, u4Factor = %d\n", (int)eVdpId, (int)u4Factor);

    #if defined(CC_MT5368) || defined(CC_MT5396)    
    vScpipSetForcedHpsFactor((UINT32)eVdpId, u4Factor);
    #else
    _VDP_SetHorizontalPreScaleDownFactor((UINT32)eVdpId, u4Factor);
    #endif    
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetPQ(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_PQ_TYPE_T ePqType;
    void* pkvPqSetInfo = NULL;
    UINT32 u4Size = 0;
    MT_RESULT_T eRet = MTR_NOT_SUPPORTED;
        
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);
    
    eVdpId = (MTVDO_PATH_T) rArg.ai4Arg[0];
    ePqType = (MTVDO_PQ_TYPE_T) rArg.ai4Arg[1];
    u4Size = (UINT32) rArg.ai4Arg[3];

    USR_SPACE_ACCESS_VALIDATE_ARG_K(rArg.ai4Arg[2], u4Size);
    pkvPqSetInfo = kmalloc(u4Size, GFP_KERNEL);

    COPY_FROM_USER_DATA(rArg.ai4Arg[2], pkvPqSetInfo, u4Size);

    eRet = (MT_RESULT_T) PE_SetPqInfo((UINT8)eVdpId, (UINT32)ePqType, pkvPqSetInfo, u4Size);

    kfree(pkvPqSetInfo);

    return eRet;
}

static MT_RESULT_T _MTVDO_GetPQ(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_PQ_TYPE_T ePqType;
    void* pkvPqGetInfo;
    UINT32 u4Size = 0;
    MT_RESULT_T eRet = MTR_NOT_SUPPORTED;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);
    
    eVdpId = (MTVDO_PATH_T) rArg.ai4Arg[0];
    ePqType = (MTVDO_PQ_TYPE_T) rArg.ai4Arg[1];
    u4Size = (UINT32) rArg.ai4Arg[3];

    USR_SPACE_ACCESS_VALIDATE_ARG_K(rArg.ai4Arg[2], u4Size);
    pkvPqGetInfo = kmalloc(u4Size, GFP_KERNEL);

    COPY_FROM_USER_DATA(rArg.ai4Arg[2], pkvPqGetInfo, u4Size);

    eRet = PE_GetPqInfo(eVdpId, ePqType, pkvPqGetInfo, u4Size);

    COPY_TO_USER_DATA(rArg.ai4Arg[2], pkvPqGetInfo, u4Size);    

    kfree(pkvPqGetInfo);

    return eRet;
}

static MT_RESULT_T _MTVDO_GET_3D_TAG(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 bPath;
    UINT32 dw3DTag;
    // 1. take pointer from user layper
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    // 2. pass pointer from user layer to kernel layer
    
    // 3. do the driver function
    bPath = rArg.ai4Arg[0];       
    dw3DTag = u4DrvTDTVGetTag3D(bPath);
    
    // 4. pass pointer from kernel layer to user layer
    USR_SPACE_ACCESS_VALIDATE_ARG(rArg.ai4Arg[1],UINT32);    
    COPY_TO_USER_ARG(rArg.ai4Arg[1], dw3DTag, UINT32);    
    
    return MTR_OK;
} 

static MT_RESULT_T _MTVDO_GET_3D_NAVI_DETECT(unsigned long arg)
{
#if defined(CC_MT5395) && defined(CC_MT5365)
    return MTR_OK;
#else
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 bPath;
    UINT32 dw3DTag;
    // 1. take pointer from user layper
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    // 2. pass pointer from user layer to kernel layer
    
    // 3. do the driver function
    bPath = rArg.ai4Arg[0];       
    dw3DTag = u4DrvTDTVGetNaviTag();
    
    // 4. pass pointer from kernel layer to user layer
    USR_SPACE_ACCESS_VALIDATE_ARG(rArg.ai4Arg[1],UINT32);    
    COPY_TO_USER_ARG(rArg.ai4Arg[1], dw3DTag, UINT32);    
    return MTR_OK;
#endif        
}

static MT_RESULT_T _MTVDO_Set3DNaviMode(unsigned long arg)
{
    E_TDTV_UI_NAVI eNavi;

    if(E_TDTV_UI_NAVI_FULL_AUTO==arg)
    {
        eNavi=E_TDTV_UI_NAVI_FULL_AUTO;
    }
    else if (E_TDTV_UI_NAVI_SEMI_AUTO==arg)
    {
        eNavi=E_TDTV_UI_NAVI_SEMI_AUTO;
    }
    else
    {
        eNavi=E_TDTV_UI_NAVI_MANAUL;
    }

    vDrvTDTVTDNAVI(eNavi);
    return MTR_OK;
}


static MT_RESULT_T _MTVDO_Set3DDCOnOff(unsigned long arg)
{
    UINT8 u1OnOff;

	u1OnOff = (arg > 0) ? SV_ON : SV_OFF;
    vDrvTDTVTDDC(u1OnOff);
    return MTR_OK;
}


#if !defined(CC_MT5396) && !defined(CC_MT5368)
static MT_RESULT_T _MTVDO_Set3DFramePacking(unsigned long arg)
{
    BOOL fgOnOff;
    fgOnOff = (BOOL)arg;

    MTVDO_PRINT(" - fgOnOff = %d\n", fgOnOff);
    vDrvTDTVPackingDbg(fgOnOff ? 3 : 1); // On: FP mode; Off: Auto mode
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetSRAMDramMode(unsigned long arg)
{
    BOOL fgOnOff;
    MTVDO_PATH_T eVdpId;
    MTAL_IOCTL_2ARG_T rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    fgOnOff = (BOOL)rArg.ai4Arg[1];

    MTVDO_PRINT("  - eVdpId = %d, fgOnOff = %d\n", (int)eVdpId, (int)fgOnOff);

    if (fgOnOff) // force dram mode
    {
        SRM_CliSetDispMode(eVdpId, 0);
        SRM_TriggerCliSrm(eVdpId);
    }
    else // roll back
    {
        SRM_ClearCliSetting();
        SRM_TriggerCliSrm(eVdpId);
    }
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetFlipMirror(unsigned long arg)
{
    BOOL fgMirrorEn, fgFlipEn;
    MTAL_IOCTL_2ARG_T rArg;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    fgMirrorEn = (MTVDO_PATH_T)rArg.ai4Arg[0];
    fgFlipEn = (BOOL)rArg.ai4Arg[1];

    MTVDO_PRINT("  - fgMirrorEn = %d, fgFlipEn = %d\n", (int)fgMirrorEn, (int)fgFlipEn);

    #ifdef CC_MT5396
        return u1SetFlipMirrorConfig(fgMirrorEn, fgFlipEn);
    #else
        return MTR_OK;
    #endif
}
#endif

static MT_RESULT_T _MTVDO_Set_FlipMirror(unsigned long arg)
{
    BOOL fgMirrorEn, fgFlipEn;
    MTAL_IOCTL_2ARG_T rArg;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    fgMirrorEn = (MTVDO_PATH_T)rArg.ai4Arg[0];
    fgFlipEn = (BOOL)rArg.ai4Arg[1];

    MTVDO_PRINT("  - fgMirrorEn = %d, fgFlipEn = %d\n", (int)fgMirrorEn, (int)fgFlipEn);

    return u1SetFlipMirrorConfig(fgMirrorEn, fgFlipEn);
}

static MT_RESULT_T _MTVDO_SRM_SetTvMode(unsigned long arg)
{
	INT32 u4mode;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(arg,INT32);
    COPY_FROM_USER_ARG(arg,u4mode,INT32);
	SRM_SetTvMode((SRM_TV_MODE_T)u4mode);
	return MTR_OK;
}

static MT_RESULT_T _MTVDO_GetPlaneWH(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 width;
	UINT32 height;
    UINT32 *pWidth;
    UINT32 *pHeight;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    pWidth = (UINT32 *)rArg.ai4Arg[0];
    pHeight = (UINT32*)rArg.ai4Arg[1];

    width = PANEL_GetPanelWidth();
	height = PANEL_GetPanelHeight();
	USR_SPACE_ACCESS_VALIDATE_ARG(pWidth, UINT32);
    COPY_TO_USER_ARG(pWidth, width, UINT32);
    USR_SPACE_ACCESS_VALIDATE_ARG(pHeight, UINT32);
    COPY_TO_USER_ARG(pHeight, height, UINT32);
	return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetMJCInitialize(unsigned long arg)
{
    #ifdef CC_MT5396
    vDrvMJCInit(FALSE);
    #endif

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetMJCResetSW(unsigned long arg)
{
    #ifdef CC_MT5396
    vDrvMJCResetSW();
    #endif

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetMJCVideoBlock(unsigned long arg)
{
    BOOL fgOnOff = SV_OFF;
        
    USR_SPACE_ACCESS_VALIDATE_ARG(arg, BOOL);
    COPY_TO_USER_ARG(arg, fgOnOff, BOOL);

    #ifdef CC_MT5396
    vDrvMJCSetOutMuteOnOff(fgOnOff);
    #endif

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetMJCEffectLevel(unsigned long arg)
{
    UINT8 bLevel;
    bLevel = (UINT8) arg;
    
    #ifdef CC_MT5396
    MJC_SetEffect(bLevel);
    #endif
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetMJCDemoMode(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    BOOL fgOnOff;
    UINT8 bMode;
    UINT16 wCenter;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    fgOnOff = (BOOL) rArg.ai4Arg[0];
    bMode = (UINT8) rArg.ai4Arg[1];
    wCenter = (UINT16) rArg.ai4Arg[2];
    
    MTVDO_PRINT(" - fgOnOff = %d, bMode = %d, wCenter = %d\n", fgOnOff, bMode, wCenter);

    #ifdef CC_MT5396
    if (fgOnOff)
    {
        MJC_SetDemo(bMode, 0, 2);
    }
    else
    {
        MJC_SetDemo(0, 2, 2);
    }
    #endif

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetMJCBypassWindow(unsigned long arg)
{
    MTAL_IOCTL_6ARG_T   rArg;
    MTAL_IOCTL_6ARG_T * prArg;
    UINT8 bWindow;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4StartX;
    UINT32 u4StartY;

    prArg = (MTAL_IOCTL_6ARG_T *) arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prArg, MTAL_IOCTL_6ARG_T);
    COPY_FROM_USER_ARG(prArg, rArg, MTAL_IOCTL_6ARG_T);
    bWindow = (UINT8) rArg.ai4Arg[0];
    u4Width = (UINT32) rArg.ai4Arg[1];
    u4Height = (UINT32) rArg.ai4Arg[2];
    u4StartX = (UINT32) rArg.ai4Arg[3];
    u4StartY = (UINT32) rArg.ai4Arg[4];

    #ifdef CC_MT5396
    MJC_SetBypassWindow(bWindow, u4Width, u4Height, u4StartX, u4StartY);
    #endif

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetMJCDemoBar(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T   rArg;
    BOOL   fgOnOff;
    UINT8  u1R;
    UINT8  u1G;
    UINT8  u1B;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    fgOnOff = (BOOL) rArg.ai4Arg[0];
    u1R = (UINT8) rArg.ai4Arg[1];
    u1G = (UINT8) rArg.ai4Arg[2];
    u1B = (UINT8) rArg.ai4Arg[3];

    #ifdef CC_MT5396
    MJC_SetDemoBar(fgOnOff, u1R, u1G, u1B);
    #endif
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_SetMJCMCOnOff(unsigned long arg)
{
    BOOL fgOnOff;
        
    USR_SPACE_ACCESS_VALIDATE_ARG(arg, BOOL);
    COPY_TO_USER_ARG(arg, fgOnOff, BOOL);
    
    #ifdef CC_MT5396
    MJC_SetMCOnOff(fgOnOff);
    #endif

    return MTR_OK;
}



MT_RESULT_T _MTVDO_DI_GetFormatInfo(unsigned long arg) 
{
    MT_RESULT_T eRet = MTR_OK ;
    MTVDO_FRAME_FORMAT_DATA rFormat;
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    MTVDO_FRAME_FORMAT_DATA* prFormat;
    UINT8 bType;
    	
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    prFormat = (MTVDO_FRAME_FORMAT_DATA*)rArg.ai4Arg[1];
    
    rFormat.fgIsInterlace = bDrvVideoIsSrcInterlace(eVdpId);
    rFormat.fgIs10bit      = u1DrvDiIs10Bit(eVdpId);
    rFormat.fgIs422        = u1DrvDiIs422Mode(eVdpId);
    rFormat.fgIsTopField = u1DrvDiIsTopField(eVdpId);

    bType = bGetVideoDecType((UINT8)eVdpId);
    if (SV_VD_DVI == bType)
    {
        rFormat.fgIsYUV = (bHDMIInputType()) ? 0 : 1; 
    }
    else if (SV_VD_VGA == bType)
    {
        rFormat.fgIsYUV = 0;
    }
    else
    {
        rFormat.fgIsYUV = 1;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prFormat, MTVDO_FRAME_FORMAT_DATA);
    COPY_TO_USER_ARG(prFormat, rFormat, MTVDO_FRAME_FORMAT_DATA);
    
    return eRet;
}

MT_RESULT_T _MTVDO_DI_SetPscanMode(unsigned long arg) 
{
    MT_RESULT_T eRet = MTR_OK ;
    MTVDO_PSCAN_MODE_T eMode;
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    BOOL fgIsInterlace;
    UINT16 u2InterlaceMode;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    eMode = (MTVDO_PSCAN_MODE_T)rArg.ai4Arg[1];
    fgIsInterlace = bDrvVideoIsSrcInterlace(eVdpId);
    
    switch (eMode)
    {
        case MTVDO_PSCAN_INPUT_SYNC_YC3D:
        	u2InterlaceMode = (eVdpId == MTVDO_SUB) ? 5 : 6;
              vDrvDiSetDataMode(eVdpId, (fgIsInterlace == SV_TRUE) ? u2InterlaceMode : 16);
        	break;
        case MTVDO_PSCAN_OUTPUT_SYNC_YC3D:
        	if (eVdpId == MTVDO_SUB)
        	{
        	    return MTR_NOT_SUPPORTED;
        	}
        	else
        	{
                  vDrvDiSetDataMode(eVdpId, (fgIsInterlace==SV_TRUE) ? 13 : 19);
        	}	
        	break;
        case MTVDO_PSCAN_INPUT_SYNC_BOB:
              vDrvDiSetDataMode(eVdpId, (fgIsInterlace == SV_TRUE) ? 2 : 7);
        	break;
        case MTVDO_PSCAN_OUTPUT_SYNC_BOB:
        	if (eVdpId == MTVDO_SUB)
        	{
        	    return MTR_NOT_SUPPORTED;
        	}
        	else
        	{
                  vDrvDiSetDataMode(eVdpId, (fgIsInterlace==SV_TRUE) ? 7 : 17);
        	}	
        	break;
        default:        	
              vDrvDiSetDataMode(eVdpId, 0);
        	break;	
    }

    return eRet;
}

MT_RESULT_T _MTVDO_DI_FBMDiTestMode(unsigned long arg) 
{
    BOOL fgEnable;
    fgEnable = (BOOL)arg;
    
    FBM_SetPscanTestMode(fgEnable);
    vSetMainFlg(MAIN_FLG_MODE_CHG);
    vSetMainFlg(MAIN_FLG_MODE_DET_DONE);

    return MTR_OK;
}


MT_RESULT_T _MTVDO_GET_NET_SWAP_Buffer(unsigned long arg) 
{

	MT_RESULT_T eRet = MTR_OK ;
    MTVDO_VIDEO_BUFFER rBuffer;
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_VIDEO_BUFFER* prBuffer;
	FBM_POOL_T* prFbmPool;
    	
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_1ARG(arg, rArg);
    prBuffer = (MTVDO_VIDEO_BUFFER*)rArg.ai4Arg[0];
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_NET_SWAP);
	rBuffer.u4Addr= prFbmPool->u4Addr;
	rBuffer.u4Size= prFbmPool->u4Size;

    USR_SPACE_ACCESS_VALIDATE_ARG(prBuffer, MTVDO_VIDEO_BUFFER);
    COPY_TO_USER_ARG(prBuffer, rBuffer, MTVDO_VIDEO_BUFFER);
	
    return eRet;
}

MT_RESULT_T _MTVDO_SET_FBMMode(unsigned long arg) 
{
    UINT32 u4Mode;
    u4Mode = (UINT32)arg;
    
    SRM_SetCurrentFBMMode(u4Mode);
    return MTR_OK;
}

MT_RESULT_T _MTVDO_DI_GetPscanMode(unsigned long arg) 
{
    MT_RESULT_T eRet = MTR_OK ;
    UINT16 u2Mode;
    MTVDO_PATH_T eVdpId;
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PSCAN_MODE_T* prMode;
    MTVDO_PSCAN_MODE_T rMode;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    prMode = (MTVDO_PSCAN_MODE_T *)rArg.ai4Arg[1];

    u2Mode = u2DrvDIGetDebugDataMode(eVdpId);

    switch (u2Mode)
    {
        case 0:
        	rMode = MTVDO_PSCAN_DEFAULT;
        	break;
        case 5:
        case 6:
        case 16:
        	rMode = MTVDO_PSCAN_INPUT_SYNC_YC3D;
        	break;
        case 13:
        case 19:
        	rMode = MTVDO_PSCAN_OUTPUT_SYNC_YC3D;
        	break;
        case 2:
        case 14:
        	rMode = MTVDO_PSCAN_INPUT_SYNC_BOB;
        	break;
        case 7:
        case 17:
        	rMode = MTVDO_PSCAN_OUTPUT_SYNC_BOB;
        	break;
        default:
        	rMode = MTVDO_PSCAN_UNKNOWN;
        	break;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prMode, MTVDO_PSCAN_MODE_T);
    COPY_TO_USER_ARG(prMode, rMode, MTVDO_PSCAN_MODE_T);
    
    return eRet;
}

static MT_RESULT_T _MTVDO_SetFreezeDIFrameBuffer(unsigned long arg)    //for DDI DI Dram Dump
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    vDrvDIFreezeOnOff(eVdpId, fgEnable);

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_DI_DumpDramInitRelease(unsigned long arg)    //for DDI DI Dram Dump
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    if (fgEnable)
    {
        u1DrvDIInitDump(eVdpId);
    }
    else
    {
        u1DrvDIReleaseDump(eVdpId);
    }

    return MTR_OK;
}

extern UINT8 u1HdmiColorMode;
static MT_RESULT_T _MTVDO_BYPASS_PREPROC(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVDO_PATH_T eVdpId;
    BOOL fgEnable;
    static UINT8 fgIsGraphic = 0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    eVdpId = (MTVDO_PATH_T)rArg.ai4Arg[0];
    fgEnable = (BOOL)rArg.ai4Arg[1];

    if (fgEnable)
    {
		if (SV_VD_DVI == bGetVideoDecType((UINT8)eVdpId) && bHDMIInputType() == 1 && !bIsScalerInput444((UINT8)eVdpId))
        {
            u1HdmiColorMode = SV_HDMI_MODE_GRAPHIC ;
		    vApiHdmiColorModeChg(eVdpId);
		    fgIsGraphic = 1;
        }
        //vDrvMatrixCtrlSel(eVdpId, 4);
    }
    else
    {   
   
        if (SV_VD_DVI == bGetVideoDecType((UINT8)eVdpId) && bHDMIInputType()== 1 && fgIsGraphic)
        {
            u1HdmiColorMode = SV_HDMI_MODE_AUTO;
		    vApiHdmiColorModeChg(eVdpId);
		    fgIsGraphic = 0;
        }
        //vDrvMatrixCtrlSel(eVdpId, 0);
    }

    return MTR_OK;
}

static MT_RESULT_T _MTVDO_DI_GetPixel(unsigned long arg)
{
    MTVDO_SCLAR_BUFFER_DATA rData;
    MTVDO_SCLAR_BUFFER_DATA *prData;
    
    MTVDO_PATH_T eVdpId;
    UINT16 x;
    UINT16 y;
    
    MTAL_IOCTL_4ARG_T    rArg;
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    eVdpId = (MTVDO_PATH_T) rArg.ai4Arg[0];
    x = (UINT16) rArg.ai4Arg[1];
    y = (UINT16) rArg.ai4Arg[2];
    prData = (MTVDO_SCLAR_BUFFER_DATA*)rArg.ai4Arg[3];

    //fix klocwork warning
    rData.u4YG = 0;
    rData.u4UB = 0;
    rData.u4VR = 0;

    //getPixelValue
    vDrvDIGetPixelValue(eVdpId,x,y,(DI_BUFFER_DATA*)&rData);
    
    USR_SPACE_ACCESS_VALIDATE_ARG(prData, MTVDO_SCLAR_BUFFER_DATA);
    COPY_TO_USER_ARG(prData, rData, MTVDO_SCLAR_BUFFER_DATA);

    MTVDO_PRINT(" - u2YG = 0x%ul, u2UB = 0x%ul, u2VR = 0x%ul \n", rData.u4YG, rData.u4UB, rData.u4VR);

    return MTR_OK;
}


static MT_RESULT_T _MTVDO_DI_GetDramPixels(unsigned long arg)
{
    DI_BUFFER_DATA rPixel;
    MTVDO_DDI_PIXEL_DATA *prAllData;
    MTVDO_DDI_PIXEL_DATA *prData;
    MTVDO_REGION_T *prRegion;
    MTVDO_REGION_T rRegion;
    
    MTVDO_PATH_T eVdpId;
    UINT32 u4I, u4J;
    UINT16 x = 0, y = 0;
    UINT16 u2TCb = 0;
    UINT8 fgIs422;
    MTAL_IOCTL_3ARG_T    rArg;
    
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    eVdpId = (MTVDO_PATH_T) rArg.ai4Arg[0];
    prRegion = (MTVDO_REGION_T *) rArg.ai4Arg[1];
    prData = (MTVDO_DDI_PIXEL_DATA*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG(prRegion, MTVDO_REGION_T);
    COPY_FROM_USER_ARG(prRegion, rRegion, MTVDO_REGION_T);
    fgIs422 = u1DrvDiIs422Mode(eVdpId);

    /*#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)               
    if (!access_ok(VERIFY_READ, (void __user *)prData, sizeof(MTVDO_DDI_PIXEL_DATA)*rRegion.u4Width*rRegion.u4Height))           
    {                                                       
        printk("mtal_ioctl_pal argument error\n");    
        return MTR_ERR_INV;                                    
    }  
    */ 	
    USR_SPACE_ACCESS_VALIDATE_ARG(prData, MTVDO_DDI_PIXEL_DATA);
    
    prAllData = kmalloc(sizeof(MTVDO_DDI_PIXEL_DATA)*rRegion.u4Width*rRegion.u4Height, GFP_KERNEL);

    if (prAllData == NULL)
    {
        return MTR_NOT_OK;
    }
    HalFlushInvalidateDCache();

    /// need to get information of 422
    
    for (u4I = 0; u4I < rRegion.u4Height; u4I++ )
    {
        y = (UINT16)(u4I + rRegion.u4Y);
        if (fgIs422 && rRegion.u4X % 2 == 1)/// for 422
        {
            x = (UINT16)(rRegion.u4X - 1);
            vDrvDIGetPixelValue(eVdpId,x,y,(DI_BUFFER_DATA*)&rPixel);
            u2TCb = (UINT16)rPixel.u4UB;
        }	
        
        for (u4J = 0; u4J < rRegion.u4Width; u4J++ )
        {
            x = (UINT16)(u4J + rRegion.u4X);
            vDrvDIGetPixelValue(eVdpId,x,y,(DI_BUFFER_DATA*)&rPixel);
            prAllData[u4I * rRegion.u4Width + u4J].u2YG = (UINT16) rPixel.u4YG;
            if (fgIs422)
            {
            	if (x%2 == 0)
            	{
                	u2TCb = (UINT16)rPixel.u4UB;
            	}
            	else
            	{
                	prAllData[u4I * rRegion.u4Width + u4J].u2VR = (UINT16) rPixel.u4UB;
                	prAllData[u4I * rRegion.u4Width + u4J].u2UB = u2TCb;
                	prAllData[u4I * rRegion.u4Width + u4J - 1].u2VR = (UINT16) rPixel.u4UB;
                	prAllData[u4I * rRegion.u4Width + u4J - 1].u2UB = u2TCb;
            	}
            }
            else
            {
                    prAllData[u4I * rRegion.u4Width + u4J].u2UB= (UINT16) rPixel.u4UB;
                	prAllData[u4I * rRegion.u4Width + u4J].u2VR = (UINT16) rPixel.u4VR;
            }
        }	

        if (fgIs422 && x%2 == 0) //422
        {
            vDrvDIGetPixelValue(eVdpId,x + 1,y,(DI_BUFFER_DATA*)&rPixel);
            prAllData[u4I * rRegion.u4Width + u4J - 1].u2VR = (UINT16) rPixel.u4UB;
            prAllData[u4I * rRegion.u4Width + u4J - 1].u2UB = u2TCb;
        }
    }

    //COPY_TO_USER_ARG(prData, *prAllData, sizeof(MTVDO_DDI_PIXEL_DATA)*rRegion.u4Width*rRegion.u4Height);
    if (copy_to_user((void __user *)prData, prAllData, sizeof(MTVDO_DDI_PIXEL_DATA)*rRegion.u4Width*rRegion.u4Height ))      
    {                                                                          
        printk("%s copy_to_user error\n", __FUNCTION__);    
        kfree(prAllData);
        return MTR_ERR_INV;                                                    
    }
    kfree(prAllData);
    
    return MTR_OK;
}


static MT_RESULT_T _MTVDO_DI_WriteDramPixels(unsigned long arg)
{
    DI_BUFFER_DATA rPixel;
    MTVDO_DDI_PIXEL_DATA *prAllData;
    MTVDO_DDI_PIXEL_DATA *prData;
    MTVDO_REGION_T *prRegion;
    MTVDO_REGION_T rRegion;
    
    MTVDO_PATH_T eVdpId;
    UINT32 u4I, u4J;
    UINT16 x, y;
    UINT8 fgIs422;
    MTAL_IOCTL_3ARG_T    rArg;
    
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    eVdpId = (MTVDO_PATH_T) rArg.ai4Arg[0];
    prRegion = (MTVDO_REGION_T *) rArg.ai4Arg[1];
    prData = (MTVDO_DDI_PIXEL_DATA*)rArg.ai4Arg[2];

    USR_SPACE_ACCESS_VALIDATE_ARG(prRegion, MTVDO_REGION_T);
    COPY_FROM_USER_ARG(prRegion, rRegion, MTVDO_REGION_T);

    fgIs422 = u1DrvDiIs422Mode(eVdpId);
    prAllData = kmalloc(sizeof(MTVDO_DDI_PIXEL_DATA)*rRegion.u4Width*rRegion.u4Height, GFP_KERNEL);
    if (prAllData == NULL)
    {
        return MTR_NOT_OK;
    }

    /*if (!access_ok(VERIFY_READ, (void __user *)prData, sizeof(MTVDO_DDI_PIXEL_DATA)*rRegion.u4Width*rRegion.u4Height))           
    {                                                       
        printk("mtal_ioctl_pal argument error\n");    
        return MTR_ERR_INV;                                    
    } */ 
    if (copy_from_user(prAllData, prData, sizeof(MTVDO_DDI_PIXEL_DATA)*rRegion.u4Width*rRegion.u4Height))    
    {                                                                          
        printk("%s copy_from_user error\n", __FUNCTION__);                     
        return MTR_ERR_INV;                                                    
    }
    
    for (u4I = 0; u4I < rRegion.u4Height; u4I++ )
    {
        y = (UINT16)(u4I + rRegion.u4Y);
        for (u4J = 0; u4J < rRegion.u4Width; u4J++ )
        {
            x = (UINT16)(u4J + rRegion.u4X);
            rPixel.u4YG = (UINT32) prAllData[u4I * rRegion.u4Width + u4J].u2YG;
            if (fgIs422)
            {
                if (x % 2 == 0)
                {
                    rPixel.u4UB = (UINT32) prAllData[u4I * rRegion.u4Width + u4J].u2UB;
                    rPixel.u4VR = 0;
                }
                else
                {
                    rPixel.u4UB = (UINT32) prAllData[u4I * rRegion.u4Width + u4J].u2VR;
                    rPixel.u4VR = 0;
                }
            }
            else
            {
                rPixel.u4UB = (UINT32) prAllData[u4I * rRegion.u4Width + u4J].u2UB;
                rPixel.u4VR = (UINT32) prAllData[u4I * rRegion.u4Width + u4J].u2VR;
            }
            vDrvDIWritePixelValue(eVdpId,x,y,(DI_BUFFER_DATA*)&rPixel);
        }	
    }
    HalFlushInvalidateDCache();
    kfree(prAllData);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDO_Set_ColorMode(unsigned long arg)
{
	UINT32 u4colormode;
    UINT32 u4MuteColor;
	 
    USR_SPACE_ACCESS_VALIDATE_ARG(arg,INT32);
    COPY_FROM_USER_ARG(arg,u4colormode,INT32);

    Printf(" %s %d   u4colormode = %d",__FUNCTION__,__LINE__,u4colormode);
	switch(u4colormode)
    {
         case 0: //WHITE
			 Printf(" %s %d   White here %d",__FUNCTION__,__LINE__);
			 u4MuteColor = (0xFF <<16)|(0xFF <<8)|(0xFF);
             vDrvVideoMuteOnOff(0,TRUE);
 	         vDrvVideoMuteColor(0,u4MuteColor);
			break;

		case 1: //RED
			Printf(" %s %d	 Red here %d",__FUNCTION__,__LINE__);
			u4MuteColor = (0xFF <<16)|(0x0 <<8)|(0x0);
			vDrvVideoMuteOnOff(0,TRUE);
			vDrvVideoMuteColor(0,u4MuteColor);
		   break;
		
		case 2://GREEN
			Printf(" %s %d	 Green here %d",__FUNCTION__,__LINE__);
			u4MuteColor = (0x0 <<16)|(0xFF <<8)|(0x0);
			vDrvVideoMuteOnOff(0,TRUE);
			vDrvVideoMuteColor(0,u4MuteColor);
		   break;
		
		case 3: //BLUE
			Printf(" %s %d	 Blue here %d",__FUNCTION__,__LINE__);
			u4MuteColor = (0x0 <<16)|(0x0 <<8)|(0xFF);
			vDrvVideoMuteOnOff(0,TRUE);
			vDrvVideoMuteColor(0,u4MuteColor);
		   break;
		
		case 4://BLACK
			Printf(" %s %d	 Black here %d",__FUNCTION__,__LINE__);
			u4MuteColor = (0x0 <<16)|(0x0<<8)|(0x0);
			vDrvVideoMuteOnOff(0,TRUE);
			vDrvVideoMuteColor(0,u4MuteColor);
		   break;
		
		case 5://GREY
			Printf(" %s %d	 Grey here %d",__FUNCTION__,__LINE__);
			u4MuteColor = (0x80 <<16)|(0x80 <<8)|(0x80);
			vDrvVideoMuteOnOff(0,TRUE);
			vDrvVideoMuteColor(0,u4MuteColor);
		   break;

		default://Disable screen mute
			vDrvVideoMuteOnOff(0,FALSE);
            break;			
  
	}
	return MTR_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int mtal_ioctl_mtvdo(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = 0;

	switch (cmd)
	{
		case MTAL_IO_VDO_INIT:
			return _MTVDO_Init();
		case MTAL_IO_VDO_STOP:
			return _MTVDO_Stop();
		case MTAL_IO_VDO_RESET:
		    return _MTVDO_Reset( arg);
		case MTAL_IO_VDO_SET_ENABLE:
		    return _MTVDO_SetEnable( arg);
		case MTAL_IO_VDO_GET_ENABLE:
		    return _MTVDO_GetEnable( arg);
		case MTAL_IO_VDO_SET_MODE:
		    return _MTVDO_SetMode( arg);
		case MTAL_IO_VDO_SET_3DMODE:
		    return _MTVDO_Set3dMode( arg);
		case MTAL_IO_VDO_2DSETLR:
		    return _MTVDO_Set2DLR( arg);
		case MTAL_IO_VDO_3DINVERSE:
		    return _MTVDO_Set3DInverse( arg);
		case MTAL_IO_VDO_SET3DDEPTH:
		    return _MTVDO_Set3DDepth( arg);			
		case MTAL_IO_VDO_GET_ISHDMIFP:
			return _MTVDO_GetISHDMIFP( arg);
		case MTAL_IO_VDO_GET_MODE:
			return _MTVDO_GetMode( arg);
		case MTAL_IO_VDO_SET_FREEZE:
			return _MTVDO_SetFreeze( arg);
		case MTAL_IO_VDO_GET_FREEZE:
		    return _MTVDO_GetFreeze( arg);
		case MTAL_IO_VDO_SET_NONLINEAR:
		    return _MTVDO_SetNonLinearMode(arg);
		case MTAL_IO_VDO_SET_DOTBYDOT:
		    return _MTVDO_SetDotByDot(arg);
		case MTAL_IO_VDO_SET_ARG:
		    return _MTVDO_SetArg( arg);
		case MTAL_IO_VDO_GET_ARG:
		    return _MTVDO_GetArg( arg);
		case MTAL_IO_VDO_SET_OVERSCAN:
		    return _MTVDO_SetOverScan( arg);
		case MTAL_IO_VDO_GET_OVERSCAN:
			return _MTVDO_GetOverScan( arg);
		case MTAL_IO_VDO_SET_SRCREGION:
		    return _MTVDO_SetSrcRegion( arg);
		case MTAL_IO_VDO_GET_SRCREGION:
		    return _MTVDO_GetSrcRegion( arg);
		case MTAL_IO_VDO_SET_OUTREGION:
		    return _MTVDO_SetOutRegion( arg);
		case MTAL_IO_VDO_GET_OUTREGION:
		    return _MTVDO_GetOutRegion( arg);
		case MTAL_IO_VDO_SET_BG:
			return _MTVDO_SetBg( arg);
		case MTAL_IO_VDO_GET_BG:
			return _MTVDO_GetBg( arg);
		case MTAL_IO_VDO_REGCB:
		    return _MTVDO_RegCbFunc( arg);
		case MTAL_IO_VDO_SET_GAMEMODE:
		    return _MTVDO_SetGameMode( arg);
        case MTAL_IO_VDO_SET_HDMITRUEBLACK:
            return _MTVDO_SetHDMITrueBlack(arg);
        case MTAL_IO_VDO_SET_BACKLIGHT_MODE:
            return _MTVDO_SetBacklightMode(arg);
		case MTAL_IO_VDO_SET_3DGAMEMODE:
		    return _MTVDO_Set3DGameMode( arg);
		case MTAL_IO_VDO_GET_UNMUTE:
		    return _MTVDO_GetUnMuteStatus( arg);
		case MTAL_IO_VDO_GET_FRAMEBUFFER_AVG:
			return _MTVDO_Scalar_GetBufferAverage( arg);
		case MTAL_IO_VDO_GET_FRAMEBUFFER_ONOFF:
			return _MTVDO_Scalar_EnableGetBuffer(arg);
        case MTAL_IO_VDO_GET_TIMINGINFO:
        	return _MTVDO_GetTimingInfo(arg);
        case MTAL_IO_VDO_SET_FACTORY_MODE:
            return _MTVDO_SetFactoryMode( arg);  
        case MTAL_IO_VDO_SET_AUTOMUTE_ONOFF:
            return _MTVDO_SetAutoMuteOnOff( arg);
		case MTAL_IO_VDO_SET_SCALER_PIXELBASED: 
			return _MTVDO_SetScalerPixelBased(arg);
		case MTAL_IO_VDO_SET_XVYCC_INNER_PTGEN:
		    return _MTVDO_SetxvYCCInnerPatGen(arg);
		case MTAL_IO_VDO_SET_PE_UI_MIN_MAX_DFT:
		    return _MTVDO_SetPeUiRangeParam(arg);		
        case MTAL_IO_VDO_GET_PE_UI_MIN_MAX_DFT:
            return _MTVDO_GetPeUiRangeParam(arg);                
        case MTAL_IO_VDO_SET_SCANMODE:
		    return _MTVDO_SetScanMode(arg);	
        case MTAL_IO_VDO_SET_DS_FACTOR:
            return _MTVDO_SetDSFactor( arg);  		    
        case MTAL_IO_VDO_SET_PQ:
            return _MTVDO_SetPQ(arg);
        case MTAL_IO_VDO_GET_PQ:
            return _MTVDO_GetPQ(arg);
        case MTAL_IO_VDO_GET_3D_TAG:
            return _MTVDO_GET_3D_TAG(arg);
		case MTAL_IO_VDO_SET_OUTPUTWINDOW:
			return _MTVDO_SetOutputWindow(arg);
		case MTAL_IO_VDO_SRM_SETTVMODE:
			return _MTVDO_SRM_SetTvMode(arg);
		case MTAL_IO_VDO_GetPlaneWH:
			return _MTVDO_GetPlaneWH(arg);	
        case MTAL_IO_VDO_SET_MJC_INITIALIZE:
            return _MTVDO_SetMJCInitialize(arg);
        case MTAL_IO_VDO_SET_MJC_RESETSW:
            return _MTVDO_SetMJCResetSW(arg);
        case MTAL_IO_VDO_SET_MJC_VIDEO_BLOCK:
            return _MTVDO_SetMJCVideoBlock(arg);
        case MTAL_IO_VDO_SET_MJC_EFCT_LVL:
            return _MTVDO_SetMJCEffectLevel( arg);
        case MTAL_IO_VDO_SET_MJC_DEMO_MODE:
            return _MTVDO_SetMJCDemoMode( arg);
        case MTAL_IO_VDO_SET_MJC_BYPASS_WINDOW:
            return _MTVDO_SetMJCBypassWindow(arg);
        case MTAL_IO_VDO_SET_MJC_DEMO_BAR:
            return _MTVDO_SetMJCDemoBar(arg);
        case MTAL_IO_VDO_SET_MJC_MC_ONOFF:
            return _MTVDO_SetMJCMCOnOff(arg);
        case MTAL_IO_VDO_GET_3D_NAVI_DETECT:
            return _MTVDO_GET_3D_NAVI_DETECT(arg);
        case MTAL_IO_VDO_SET_3D_NAVI_MODE:
            return _MTVDO_Set3DNaviMode(arg);
        case MTAL_IO_VDO_SET_3D_DEPTH_CTRL_ON_OFF:
            return _MTVDO_Set3DDCOnOff(arg);
        case MTAL_IO_VDO_SET_DI_FREEZE_ONOFF:
            return _MTVDO_SetFreezeDIFrameBuffer(arg);
        case MTAL_IO_VDO_DI_GET_PIXEL:
            return _MTVDO_DI_GetPixel(arg);
        case MTAL_IO_VDO_DI_GET_DRAM_PIXELS:
            return _MTVDO_DI_GetDramPixels(arg);    
        case MTAL_IO_VDO_DI_WRITE_DRAM_PIXELS:
            return _MTVDO_DI_WriteDramPixels(arg);  
       case MTAL_IO_VDO_DI_GET_PSCAN_MODE:
            return _MTVDO_DI_GetPscanMode(arg);   
        case MTAL_IO_VDO_DI_SET_PSCAN_MODE:
            return _MTVDO_DI_SetPscanMode(arg);       
        case MTAL_IO_VDO_DI_GET_FORMAT_INFO:
            return _MTVDO_DI_GetFormatInfo(arg);    
        case MTAL_IO_VDO_SET_DI_DUMP_DRAM_INIT_RELEASE:
            return _MTVDO_DI_DumpDramInitRelease(arg);    
        case MTAL_IO_VDO_FBM_DI_TEST_MODE:
            return _MTVDO_DI_FBMDiTestMode(arg);    
        case MTAL_IO_VDO_BYPASS_PREPROC:
            return _MTVDO_BYPASS_PREPROC(arg);    	            
		case MTAL_IO_VDO_SET_FBM_MODE:
            return _MTVDO_SET_FBMMode(arg);
		case MTAL_IO_VDO_GET_NET_SWAP_BUFFER:
            return _MTVDO_GET_NET_SWAP_Buffer(arg);
        case MTAL_IO_VDO_GET_PCF_PQF_INFO:
            return _MTVDO_GetPcfPqfInfo(arg);
        case MTAL_IO_VDO_GET_PTF_INFO:
            return _MTVDO_GetPtfInfo(arg);
        case MTAL_IO_VDO_SET_PWM:
            return _MTVDO_SetPWM(arg);
        case MTAL_IO_VDO_SET_VIDEO_MUTE: //Caihua 20120601
            return _MTVDO_SetMuteOnOff(arg);
        case MTAL_IO_VDO_SET_FLIPMIRROR:  
            return _MTVDO_Set_FlipMirror(arg);
		case MTAL_IO_VDO_SET_COLORMODE:
				return _MTVDO_Set_ColorMode(arg);
		default:
		    break;
	}
	return i4Ret;
}
