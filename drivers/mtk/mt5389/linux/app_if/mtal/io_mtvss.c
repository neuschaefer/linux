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
 * $RCSfile: $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file io_mtvss.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "vdp_if.h"
#include "video_def.h"
#include "drv_auto.h"

#include "mtvdo.h"
#include "cb_low.h"

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
#include "vdo_misc.h"
#include "b2r_drvif.h"
#include "drv_display.h"
#include "nptv_drvif.h"
#endif

#include "drv_video.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"

#include "hw_di.h"
#include <linux/mtal_ioctl.h>
#include "fbm_drvif.h"
//#include "mtvss.h"
#include "drv_scaler_gfx.h"
#include "drv_scaler_drvif.h"
#include "source_select.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VSS_INDEX 2

// copy from mtvss.h
typedef enum
{
    MTVSS_INPUT_TYPE_HDMI      = 0,
    MTVSS_INPUT_TYPE_COMPOSITE = 1,
    MTVSS_INPUT_TYPE_COMPONENT = 2,
    MTVSS_INPUT_TYPE_COAXIAL   = 3,
    MTVSS_INPUT_TYPE_SVIDEO    = 4,
    MTVSS_INPUT_TYPE_VGA       = 5,
    MTVSS_INPUT_TYPE_MAX
} MTVSS_INPUT_TYPE_T;

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(INT32)))                          \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }                                                       \

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(INT32)))                      \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }                                                       \


#define USR_SPACE_ACCESS_VALIDATE_ARG_K(arg,size)           \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))                                   \
    {                                                       \
        printk("mtal_ioctl_mtvss valid argument error\n");  \
        return MTR_ERR_INV;                                 \
    }   

#define COPY_FROM_USER_ARG_K(ArgUsr, ArgKernel,size)        \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       size))                               \
    {                                                       \
        printk("mtal_ioctl_mtvss argument copy error\n");   \
        return MTR_ERR_INV;                                 \
    } 

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }                                                           

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }  
    
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }                                                           

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }  


#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }                                                           

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }  

#define USR_SPACE_ACCESS_VALIDATE_5ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_5ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }                                                       \
 
#define COPY_FROM_USER_5ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_5ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)             \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))                           \
    {                                                       \
        printk("mtal_ioctl_mtvss argument error\n");        \
        return MTR_ERR_INV;                                 \
    }    

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)            \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,     \
                       sizeof(type)))                       \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");        \
        return MTR_ERR_INV;                                 \
    }

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
UINT32 _mtvss_FBMinfo_u4Addr = 0;
UINT32 _mtvss_FBMinfo_u4Size = 0;
UINT8 _mtvss_init    = 0;
UINT8 _mtvss_connect = 0;
UINT16 _mtvss_width  = 0;
UINT16 _mtvss_height = 0;

UINT8 _VSS_Path = MTVDO_MAX;
UINT8 _VSS_Idx = VSS_INDEX;
UINT8 _VSS_Cnt = 0;
UINT8 _VSS_ConnectTrigger = 0;

//-----------------------------------------------------------------------------
// Local functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/**
 * @brief 
 It initialized the part pertaining to the Video auxiliary outputs. 
 */
static MT_RESULT_T _MTVSS_Init(void)
{
    FBM_POOL_T* prPool;

//    MTVSS_PRINT(" - void\n");

    // get VSS FBM Pool info
    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_VSS, NULL);

    // pool is valid, update dram info.
    if (prPool != NULL)
    {
        _mtvss_FBMinfo_u4Addr = prPool->u4Addr;
        _mtvss_FBMinfo_u4Size = prPool->u4Size;
    }
    else
    {
        printk(" fail to get pool\n");
        return MTR_NOT_OK;
    }

    _mtvss_init = 1;

    //for 5396 vesion check

    // Get FBM pool info, Set buffer number/address, Set write mode, Set alpha
    vScpip_GFX_init();
    u1Scpip_GFX_Set_Frame_Num(2);
    u1Scpip_GFX_Set_Dram_Address(_mtvss_FBMinfo_u4Addr, _mtvss_FBMinfo_u4Size/2, 2);
    u1Scpip_GFX_Set_Write_Mode(1);
    u1Scpip_GFX_Set_Dump_Format(RGBA8888);
    u1Scpip_GFX_Set_Alpha(0xFF);
//    u1Scpip_GFX_Write_Enable(1);
    
#ifdef CC_SUPPORT_VSS
		return MTR_OK;
#else
		return MTR_NOT_SUPPORTED;
#endif
}

/**
 * @brief 
 It connects the VSS component with the Input Source.
 * @param eVdpId
 */
static MT_RESULT_T _MTVSS_Connect(unsigned long arg)
{
    // Color space, Set Main/Sub path, Set VSS path. 
    // Main, scaled h/w
    // Sub,  original h/w
    MTVDO_PATH_T eVdpId;
    UINT32 index, status;
    UINT16 width, height;
    UINT8 bPath;
    RPicInfo* picInfo;
    VDP_REGION_T VSSSrcRegion, VSSOutRegion;

    eVdpId = (MTVDO_PATH_T)arg;

    // error handling for flow
    if (_mtvss_init == 0)
    {
        printk(" error \n");
        return MTR_NOT_OK;
    }
    
    if (_mtvss_connect == 1 && _VSS_Path != eVdpId)
    {
        printk(" wrong path...\n");
        return MTR_PARAMETER_ERROR;
    }
    else if (_mtvss_connect == 1 && _VSS_Path == eVdpId)
    {
        printk(" alrady connect...\n");
        //return MTR_OK;
    }
    
    //printk(" MTVDO path %d \n", eVdpId);
    if (eVdpId >= MTVDO_MAX) //  || _mtvss_connect == 1, must disconnect first, then connect again
    {
        printk(" error \n");
        return MTR_PARAMETER_ERROR;
    }

    _mtvss_connect = 1;
    _VSS_Path = eVdpId;
    
    // connect to main/sub
    //printk(" Main Src %d \n", bApiVideoGetSrcType(MTVDO_MAIN));
    if (eVdpId == MTVDO_SUB)
    {
        bApiVideoSetVideoSrc(MTVDO_SUB, bApiVideoGetSrcType(MTVDO_MAIN));
        VSSSrcRegion.u4X = 0;
        VSSSrcRegion.u4Y = 0;
        VSSSrcRegion.u4Width  = 10000;
        VSSSrcRegion.u4Height = 10000;
        VSSOutRegion.u4X = 0;
        VSSOutRegion.u4Y = 0;
        VSSOutRegion.u4Width  = 10000;
        VSSOutRegion.u4Height = 10000;
        VDP_SetEnable(VDP_2, TRUE);
        VDP_SetDisplayOff(VDP_2, TRUE);
        VDP_SetSrcRegion(VDP_2, 1, VSSSrcRegion); // Full screen
        VDP_SetOutRegion(VDP_2, 1, VSSOutRegion);
        
        vSetPipFlg(PIP_FLG_MODE_CHG);
        vSetPipFlg(PIP_FLG_MODE_DET_DONE);
        //printk(" Sub  Src %d \n", bApiVideoGetSrcType(MTVDO_SUB));
    }
    
    vRegWriteFldAlign(SCPIP_SYSTEM_10, 0, SYSTEM_10_IMPORT_1); // main or sub?
    vRegWriteFldAlign(SCPIP_SYSTEM_12, 0, SYSTEM_12_IMPORT_2);
    u1Scpip_GFX_Source_Sel(_VSS_Path);
    u1Scpip_GFX_Write_Enable(1);
    vRegWriteFldAlign(SCPIP_SYSTEM_10, 1, SYSTEM_10_IMPORT_1);
    vRegWriteFldAlign(SCPIP_SYSTEM_12, 1, SYSTEM_12_IMPORT_2);
    
    status = u1Scpip_GFX_GetWriteStatus();
    index = u1Scpip_GFX_GetCurrentFrameIdx();
    if (status != 0 || index != 0)
    {
        printk(" DEBUG init status & index != 0\n");
    }    

    // get video width/height
    bPath   = getScalerPath(0); // main
    picInfo = getPicInfo(bPath);
    if (_VSS_Path == MTVDO_MAIN)
    {
        width  = _arScalePrm[0].u4DownScaledWidth;
        height = _arScalePrm[0].u4DownScaledHeight;
        // FIXME
        if (BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC)
        {
        }
        else if (width & 0x3)
        {
            printk("DBG: NOT multiple of 4\n");
            width = (width>>2)<<2;
        }
    }
    else // MTVDO_SUB
    {
        width  = picInfo->wSrcW;
        height = picInfo->wSrcH;
    }
    _mtvss_width  = width;
    _mtvss_height = height;
    
    if (_VSS_Path == MTVDO_SUB)
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_10, 0, SYSTEM_10_IMPORT_1);
        vRegWriteFldAlign(SCPIP_SYSTEM_12, 0, SYSTEM_12_IMPORT_2);
        vRegWriteFldAlign(SCPIP_PIP2_00, _mtvss_height, PIP2_00_DRAM_WR_WIDTH_V_2); // DRAM write width
        vRegWriteFldAlign(SCPIP_PIP2_00, _mtvss_width,  PIP2_00_DRAM_WR_WIDTH_H_2); // DRAM write height
        vRegWriteFldAlign(SCPIP_SYSTEM_17, _mtvss_width, SYSTEM_17_REG_H_BOUND_3X3); // bound
        vRegWriteFldAlign(MCVP_KC_17, 1, MCVP_SUB_OUT_SEL); // DI SRC MUX: sub out from main
        vDrvDIStopCtrlFw(MTVDO_SUB, SV_OFF);                // DI stop normal FW routine
        vDrvDIOnOff(MTVDO_SUB, SV_ON);                      // DI sub path always on (for P-timing)
        vRegWriteFldAlign(SCPIP_SYSTEM_10, 1, SYSTEM_10_IMPORT_1);
        vRegWriteFldAlign(SCPIP_SYSTEM_12, 1, SYSTEM_12_IMPORT_2);
        }

    _VSS_ConnectTrigger = 30;
    _VSS_Idx = (index == 1)?0:1;

    // call back

#ifdef CC_SUPPORT_VSS
		return MTR_OK;
#else
		return MTR_NOT_SUPPORTED;
#endif
}
 
/**
 * @brief 
 It releases the VSS connection settings.
 * @param eVdpId
 */
static MT_RESULT_T _MTVSS_Disconnect(unsigned long arg)
{
    // Set Sub path
    
    MTVDO_PATH_T eVdpId;
    eVdpId = (MTVDO_PATH_T)arg;

 //   MTVSS_PRINT(" - eVdpId = %d\n", (int)eVdpId);
    //printk(" MTVDO path %d \n", eVdpId);
    if (_mtvss_connect == 0)
    {
        printk(" not connected \n");
        return MTR_NOT_OK;
    }

    if (eVdpId != _VSS_Path)
    {
        printk(" error \n");
        return MTR_PARAMETER_ERROR;
    }
    
    if (_VSS_Path == MTVDO_SUB) // the default value
    {
        VDP_REGION_T VSSSrcRegion, VSSOutRegion;
        VSSSrcRegion.u4X = 0;
        VSSSrcRegion.u4Y = 0;        
        VSSSrcRegion.u4Width  = 0;
        VSSSrcRegion.u4Height = 0;
        VSSOutRegion.u4X = 0;
        VSSOutRegion.u4Y = 0;
        VSSOutRegion.u4Width  = 0;
        VSSOutRegion.u4Height = 0;

        vRegWriteFldAlign(MCVP_KC_17, 0, MCVP_SUB_OUT_SEL); // DI SRC MUX: sub out
        vDrvDIStopCtrlFw(MTVDO_SUB, SV_ON);                 // DI normal FW routine
        VDP_SetEnable(VDP_2, FALSE);
        VDP_SetDisplayOff(VDP_2, FALSE);
        VDP_SetSrcRegion(VDP_2, 0, VSSSrcRegion);
        VDP_SetOutRegion(VDP_2, 0, VSSOutRegion);
        vSetPipFlg(PIP_FLG_MODE_CHG);
        vSetPipFlg(PIP_FLG_MODE_DET_DONE);
    }

    u1Scpip_GFX_Write_Enable(0);
    
    _mtvss_connect = 0;
    _VSS_Path  = MTVDO_MAX;

#ifdef CC_SUPPORT_VSS
		 return MTR_OK;
#else
		 return MTR_NOT_SUPPORTED;
#endif
}

/**
 * @brief 
 It stops the Video auxiliary outputs (CVBS output).
 * @param eVssId
 */
static MT_RESULT_T _MTVSS_GetVideoInfo(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    UINT16 *pwidth;
    UINT16 *pheight;
    UINT16 *pcolor;
    UINT16 *psource;
    UINT16 width, height, color, source, signal;
    UINT8 bPath;
    RPicInfo* picInfo;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);
    pwidth  = (UINT16 *)rArg.ai4Arg[0];
    pheight = (UINT16 *)rArg.ai4Arg[1];
    pcolor  = (UINT16 *)rArg.ai4Arg[2];
    psource = (UINT16 *)rArg.ai4Arg[3];

    if (_mtvss_connect == 0)
    {
        printk(" not connected \n");
        return MTR_NOT_OK;
    }
    
    bPath   = getScalerPath(0); // main
    picInfo = getPicInfo(bPath);
    if (_VSS_Path == MTVDO_MAIN)
    {
        width  = _arScalePrm[0].u4DownScaledWidth;
        height = _arScalePrm[0].u4DownScaledHeight;
        // FIXME
        if (BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC)
        {
        }
        else if (width & 0x3)
        {
            printk("DBG: NOT multiple of 4\n");
            width = (width>>2)<<2;
        }
    }
    else // MTVDO_SUB
    {
        width  = picInfo->wSrcW;
        height = picInfo->wSrcH;
    }
    _mtvss_width  = width;
    _mtvss_height = height;
    color  = bHDMIClocrimetry();
    signal = (UINT16)bGetSignalType(bPath);
    switch(signal)
    {
        case SV_ST_TV:      source = MTVSS_INPUT_TYPE_COAXIAL;
                            break;
        case SV_ST_AV:      source = MTVSS_INPUT_TYPE_COMPOSITE;
                            break;
        case SV_ST_SV:      source = MTVSS_INPUT_TYPE_SVIDEO;
                            break;
        case SV_ST_YP:      source = MTVSS_INPUT_TYPE_COMPONENT;
                            break;
        case SV_ST_VGA:     source = MTVSS_INPUT_TYPE_VGA;
                            break;
        case SV_ST_DVI:     source = MTVSS_INPUT_TYPE_HDMI;
                            break;
        case SV_ST_MPEG:    source = MTVSS_INPUT_TYPE_COAXIAL;
                            break;
        default:            source = MTVSS_INPUT_TYPE_MAX;
                            break;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pwidth,  INT16);
    COPY_TO_USER_ARG(pwidth,  width,  INT16);
    USR_SPACE_ACCESS_VALIDATE_ARG(pheight, INT16);
    COPY_TO_USER_ARG(pheight, height, INT16);
    USR_SPACE_ACCESS_VALIDATE_ARG(pcolor,  INT16);
    COPY_TO_USER_ARG(pcolor,  color,  INT16);
    USR_SPACE_ACCESS_VALIDATE_ARG(psource, INT16);
    COPY_TO_USER_ARG(psource, source, INT16);
    
 //   MTVSS_PRINT(" Width = %d, Height = %d, Color = %d, Source = %d\n",
//	              width, height, color, source);

#ifdef CC_SUPPORT_VSS
        return MTR_OK;
#else
        return MTR_NOT_SUPPORTED;
#endif
}

/** * @brief Video 
 It makes the Video auxiliary outputs mute On/Off. 
 * @param eVssId 
 * @param bMuteOn TRUE : Mute On, FALSE : Mute Off 
 */
static MT_RESULT_T _MTVSS_GetPlatformInfo(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, rArg);

//    USR_SPACE_ACCESS_VALIDATE_ARG(prSrcRegion, MTVDO_REGION_T);
//    COPY_TO_USER_ARG(prSrcRegion, rSrcRegion, MTVDO_REGION_T);
    
#ifdef CC_SUPPORT_VSS
    return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif
}

/**
 * @brief Get Frame Buffer from Scaler DRAM dump.
 Copy to user (previous frame)
 Check status, if index changed, call back
 After call back, trigger next frame, update current index
 * @param roi_x
 * @param roi_y
 * @param roi_width
 * @param roi_height
 * @param *buffer
 */
static MT_RESULT_T _MTVSS_GetFrameBuffer(unsigned long arg)
{
    MTAL_IOCTL_5ARG_T rArg;
    UINT16 roi_x;
    UINT16 roi_y;
    UINT16 roi_width;
    UINT16 roi_height;
    UINT8  *buffer;
    UINT8  *buffer_index;
    UINT16 row, dram_w, dram_h;

    USR_SPACE_ACCESS_VALIDATE_5ARG(arg);
    COPY_FROM_USER_5ARG(arg, rArg);
    roi_x      = rArg.ai4Arg[0];
    roi_y      = rArg.ai4Arg[1];
    roi_width  = rArg.ai4Arg[2];
    roi_height = rArg.ai4Arg[3];
    buffer     = (UINT8  *)rArg.ai4Arg[4];

    if (_mtvss_connect == 0)
    {
        printk(" not connected \n");
        return MTR_NOT_OK;
    }

    dram_w = _mtvss_width;
    dram_h = _mtvss_height;
    //MTVSS_PRINT(" [%d] - disp_w = %d disp_h = %d\n", _VSS_Idx, dram_w, dram_h);
    //printk(" debug index %d w %d h %d\n", _VSS_Idx, dram_w, dram_h);
    
    // error handling, real total WIDTH, HEIGHT
    if (roi_width * roi_height > dram_w * dram_h)
    {
        printk(" error roi %d %d > dram %d %d\n", roi_width, roi_height, dram_w, dram_h);
        return MTR_PARAMETER_ERROR;
    }
    
    if ((roi_x + roi_height > dram_w) || (roi_y + roi_height > dram_h))
    {
        printk(" error roi (%d %d %d %d) > dram %d %d\n", roi_x, roi_y, roi_width, roi_height, dram_w, dram_h);
        return MTR_PARAMETER_ERROR;
    }

    // COPY from (x, y) to (x+width, y+height)
    buffer_index  = (UINT8  *)_mtvss_FBMinfo_u4Addr + (_mtvss_FBMinfo_u4Size/2)*_VSS_Idx;
    buffer_index += 4*(roi_y * dram_w + roi_x);
    for (row = 0; row < roi_height; row++)
    {
//        printk("%d copy_to %X %X %X\n", row, buffer_index, VIRTUAL(buffer_index), buffer);                       
        // copy roi_width only
        USR_SPACE_ACCESS_VALIDATE_ARG(buffer, UINT8);
        if (copy_to_user((void __user *)buffer, (void *)(VIRTUAL((UINT32)buffer_index)), 4*(sizeof(UINT8)*roi_width)))
        {                                                                          
            printk("%s copy_to_user error\n", __FUNCTION__);                       
            return MTR_DONT_CARE;                                                    
        }
        buffer       += 4*roi_width;
        buffer_index += 4*dram_w;
    }

    if (_VSS_Path == MTVDO_SUB)
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_12, 0, SYSTEM_12_IMPORT_2);
        vRegWriteFldAlign(SCPIP_PIP2_00, _mtvss_height, PIP2_00_DRAM_WR_WIDTH_V_2); // DRAM write width
        vRegWriteFldAlign(SCPIP_PIP2_00, _mtvss_width,  PIP2_00_DRAM_WR_WIDTH_H_2); // DRAM write width       
        vRegWriteFldAlign(SCPIP_SYSTEM_12, 1, SYSTEM_12_IMPORT_2);
    }

    // call back

    // trigger next frame
    _VSS_Idx = (_VSS_Idx == 1)?0:1;
    u1Scpip_GFX_Write_Trigger(); // trigger WRITE

#ifdef CC_SUPPORT_VSS  
    return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif
}

/**
 * @brief 
 * @param eVssId
 * @param pData Data buffer pointer
 * @param length Data Length
 * @endcode
 */
static MT_RESULT_T _MTVSS_SetFrameCallback(unsigned long arg)
{    
    // Kernel space, user space

#ifdef CC_SUPPORT_VSS
    return MTR_OK;    
#else
    return MTR_NOT_SUPPORTED;
#endif
}

static MT_RESULT_T _MTVSS_SetColorFormat(unsigned long arg)
{    
    // Set ARGB8888 format
    UINT32 ColorFmt;
    UINT8 channelA, channelR, channelG, channelB;
    
    ColorFmt = (UINT32)arg;

 //   MTVSS_PRINT(" - eVdpId = %d\n", (int)ColorFmt);
    //printk(" Color Format %X \n", ColorFmt);

    channelA = (ColorFmt >> 24) & 0xFF;
    channelR = (ColorFmt >> 16) & 0xFF;
    channelG = (ColorFmt >>  8) & 0xFF;
    channelB =  ColorFmt & 0xFF;
    //printk(" A %d R %d G %d B %d\n", channelA, channelR, channelG, channelB);

#if 0
    vRegWriteFldAlign(SCPIP_DRAM_M_14, 1, DRAM_M_14_DA_GFX_MODE);          // ARGB8888
    vRegWriteFldAlign(SCPIP_DRAM_M_14, 0, DRAM_M_14_DA_GFX_ALPHA_POS_SEL); // 0:ARGB, 1:RGBA
    vRegWriteFldAlign(SCPIP_DRAM_M_14, 2, DRAM_M_14_DA_GFX_MUX_B);         // B channel
    vRegWriteFldAlign(SCPIP_DRAM_M_14, 1, DRAM_M_14_DA_GFX_MUX_R);         // R channel
    _arGfxInf.Format = RGBA8888;
    _arGfxInf.u1AlphaPos = 0; // Scaler format (0:ARGB , 1:RGBA) different order with OpenGL.
    _arGfxInf.u1MuxB = 2;
    _arGfxInf.u1MuxR = 1;
#endif

    vRegWriteFldAlign(SCPIP_SYSTEM_10, 0, SYSTEM_10_IMPORT_1); // main or sub?
    vRegWriteFldAlign(SCPIP_DRAM_M_14, channelA, DRAM_M_14_DA_GFX_ALPHA_POS_SEL); // A channel
    vRegWriteFldAlign(SCPIP_DRAM_M_14, channelR, DRAM_M_14_DA_GFX_MUX_R);         // R channel
    vRegWriteFldAlign(SCPIP_DRAM_M_14, channelG, DRAM_M_14_DA_GFX_MUX_G);         // G channel
    vRegWriteFldAlign(SCPIP_DRAM_M_14, channelB, DRAM_M_14_DA_GFX_MUX_B);         // B channel
    vRegWriteFldAlign(SCPIP_SYSTEM_10, 1, SYSTEM_10_IMPORT_1);

#ifdef CC_SUPPORT_VSS
    return MTR_OK;    
#else
    return MTR_NOT_SUPPORTED;
#endif
}

int mtal_ioctl_mtvss(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    int i4Ret = MTR_OK;
	   
    switch (cmd) 
    {				
        case MTAL_IO_VSS_INIT:
            return _MTVSS_Init();
        case MTAL_IO_VSS_CONNECT:
            return _MTVSS_Connect(arg);
        case MTAL_IO_VSS_DISCONNECT:
             return _MTVSS_Disconnect(arg);
        case MTAL_IO_VSS_GET_VIDEO_INFO:
            return _MTVSS_GetVideoInfo(arg);
        case MTAL_IO_VSS_GET_PLATFORM_INFO:
            return _MTVSS_GetPlatformInfo(arg);
        case MTAL_IO_VSS_GET_FRAME_BUFFER:
            return _MTVSS_GetFrameBuffer(arg);
        case MTAL_IO_VSS_CALLBACK:
            return _MTVSS_SetFrameCallback(arg);
        case MTAL_IO_VSS_COLOR_FORMAT:
            return _MTVSS_SetColorFormat(arg);
        default:
            return MTR_PARAMETER_ERROR;
    }
    return i4Ret;
}

