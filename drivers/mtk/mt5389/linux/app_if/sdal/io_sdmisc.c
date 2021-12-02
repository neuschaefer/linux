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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: io_sdmisc.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sdvideo.c
 *  SDAL SdVideo module ioctl interface of MT538X.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include <linux/sdal_ioctl.h>
#include <linux/wait.h>
#include "sd_vdopath.h"

#include "sv_const.h"
#include "nptv_if.h"
#include "hw_vdoin.h"
#include "drv_tvd.h"
#include "util.h"
#include "SdMisc.h"
#include "stc_drvif.h"

#include "source_table.h"
#include "drv_autocolor.h"
#include "hw_ycproc.h"
#include "drv_async.h"
#include "pmx_if.h"
#include "pmx_drvif.h"
#include "drv_tvd.h"
//mtk01140 20071016 SDAL sharpness interface
#include "drv_ycproc.h"
#include "hw_ycproc.h"
#include "hw_scpos.h"
#include "hw_nr.h"
#include "SdMisc.h"
#include "api_eep.h"
#include "eepdef.h"
//mtk01140 20071024 need to record saturation value here, for vga saturation
#include "vdp_if.h"
#include "aud_if.h"
#include "mpv_if.h"
#include "vdec_if.h"
#include "video_def.h"
#include "vdo_misc.h"
#include "drv_dvi.h"
#include "drv_hdmi.h"
#include "osd_drvif.h"

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \         
    {                                                       \        
        printk("_SdMisc_Allocate argument error\n");        \
        return -1;                                          \
    }                                                       \    

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(SDAL_IOCTL_2ARG_T)))              \         
    {                                                       \        
        printk("_SdMisc_Allocate argument error\n");        \
        return -1;                                          \
    }                                                       \    

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(SDAL_IOCTL_3ARG_T)))              \         
    {                                                       \        
        printk("_SdMisc_Allocate argument error\n");        \
        return -1;                                          \
    }                                                       \    

#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(SDAL_IOCTL_4ARG_T)))              \         
    {                                                       \        
        printk("_SdMisc_Allocate argument error\n");        \
        return -1;                                          \
    }                                                       \    

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("_SdMisc_Allocate argument error\n");        \    
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(SDAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("_SdMisc_Allocate argument error\n");        \    
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(SDAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("_SdMisc_Allocate argument error\n");        \    
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(SDAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("_SdMisc_Allocate argument error\n");        \    
        return -1;                                          \
    }                                                       \
    
//-----------------------------------------------------------------------------
// Import variables
//-----------------------------------------------------------------------------

/// Backup of decoder settings
extern SdAVDec_Settings_t rAvDecSettings;

//-----------------------------------------------------------------------------
// Local variables
//-----------------------------------------------------------------------------

static SdBool_t _bSdMiscVideoSyncOn = 255;      // default value
static SdBool_t _bSdMiscAudioSyncOn = 255;      // default value
    
//-----------------------------------------------------------------------------
// Externl function forward declarations
//-----------------------------------------------------------------------------
extern void AUD_SetAvSyncMode(UINT8 u1DecId, AV_SYNC_MODE_T eSynMode);

    
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static SdResult _SdMisc_SetOsdPosition(UINT32 u4X, UINT32 u4Y, UINT32 u4W, UINT32 u4H)
{
    INT32 i4Ret;
    UINT32 u4Rgn, u4SrcW, u4SrcH;

#ifdef CC_MT5360
    i4Ret = OSD_PLA_GetFirstRegion(OSD_PLANE_2, &u4Rgn);
#else
    u4Rgn = (UINT32)OSD_PLA_GetFirstRegion(OSD_PLANE_2);
#endif

    i4Ret = OSD_RGN_Get(u4Rgn, OSD_RGN_DISP_W, &u4SrcW);
    i4Ret = OSD_RGN_Get(u4Rgn, OSD_RGN_DISP_H, &u4SrcH);
    i4Ret = OSD_SC_Scale((UINT32)OSD_SCALER_2, TRUE, u4SrcW, u4SrcH, u4W, u4H);
    i4Ret = OSD_BASE_SetOsdPosition((UINT32)OSD_PLANE_2, u4X, u4Y);
    (void*)i4Ret;

    return SD_OK;
}

static SdResult _SdMisc_ExtIn_LockFast(void)
{
    UINT8 bTryLoopCnt = 0x30;
    vTvd3dReset(); 

      vTvdForceTvdManualVPres(SV_ON);
    while(bTryLoopCnt>0)
    {
        //TVD HSync Lock is used during channel change (7~20 ms).
        if (fgHwTvdHsyncLock()) 
        {
            vTvdForceTvdManualVPres(SV_OFF);
            return SD_OK;
        }
        vUtDelay1ms(2);
        bTryLoopCnt --;
    }
    vTvdForceTvdManualVPres(SV_OFF);
    return SD_NOT_OK;
}
static SdResult _SdMisc_ExtIn_LockMid(void)
{
    UINT8 bTryLoopCnt = 0x30;
    vTvd3dReset(); 

    while(bTryLoopCnt>0)
    {
        //TVD VSync Lock is used during channel scan(60~80 ms).
        if (fgHwTvdVLock()) 
        {
            return SD_OK;
        }
        vUtDelay1ms(2);
        bTryLoopCnt --;
    }

    return SD_NOT_OK;
}

static SdResult _SdMisc_ExtIn_Start(unsigned long arg)
{
    SdExtInID_k       eExtInId;
    SdResult          eSdResult = SD_OK;
    
    eExtInId    = (SdExtInID_k)arg;
    
    switch (eExtInId)
    {
    case SD_EXTIN0:
        _SD_AUD_ExtInPlay(0);
        break;
    case SD_EXTIN1:
        _SD_AUD_ExtInPlay(1);
        break;
    default:
        eSdResult = SD_PARAMETER_ERROR;
    }
    
    return eSdResult;
}

static SdResult _SdMisc_ExtIn_Stop(unsigned long arg)
{
    SdExtInID_k       eExtInId;
    SdResult          eSdResult = SD_OK;
    
    eExtInId    = (SdExtInID_k)arg;
    
    switch (eExtInId)
    {
    case SD_EXTIN0:
        _SD_AUD_ExtInStop(0);
        break;
    case SD_EXTIN1:
        _SD_AUD_ExtInStop(1);
        break;
    default:
        eSdResult = SD_PARAMETER_ERROR;
    }
    
    return eSdResult;
}

static SdResult _SdMisc_Sound_Connect(unsigned long arg)
{
    INT32             i4Ret;
    SDAL_IOCTL_3ARG_T rArg;
    SdSoundID_k       eSoundId;
    SdSource_k        eSrc;
    SdBackEndInID_k   eInputId;
    
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    
    eSoundId    = (SdSoundID_k)rArg.au4Arg[0];
    eSrc        = (SoundInputPort_k)rArg.au4Arg[1];
    eInputId    = (SdBackEndInID_k)rArg.au4Arg[2];

    i4Ret = _SD_AUD_MiscConnect(eSoundId, eSrc, eInputId);
    return i4Ret;
}

//mtk01140 20071015 load quality setting
static SdResult _SdMisc_LoadPictureData(unsigned long arg)
{
    INT32             i4Ret;
    SDAL_IOCTL_3ARG_T rArg;
    SdVideoID_k eVideoId;
    uint8_t *pData;
    uint32_t size;
    
    //parse arg
    //mtk01140 20071123 follow _AvDec_GetStatus()
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_3ARG_T)))
    {
        printk("_SdMisc_LoadPictureData argument error\n");
        return SD_ERR_INV;
    }

    // Copy user space argument
    if (copy_from_user(&rArg, (void __user *)arg,
                       sizeof(SDAL_IOCTL_3ARG_T)))
    {
        printk("_SdMisc_LoadPictureData argument error\n");
        return SD_ERR_INV;
    }
//    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
//    COPY_FROM_USER_3ARG(arg, rArg); 
    
    eVideoId    = (SdVideoID_k)rArg.au4Arg[0];
    pData   = (uint8_t)rArg.au4Arg[1];
    size    = (uint32_t)rArg.au4Arg[2];
    
}

//mtk01140 20071015 set gamma interface
static SdResult _SdMisc_SetGammaData(unsigned long arg)
{
    INT32             i4Ret;
    SDAL_IOCTL_2ARG_T rArg;
    UINT8 bMode;
    UINT8 **ppbTable;
    
    //parse arg
    //mtk01140 20071123 follow _AvDec_GetStatus()
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_SdMisc_LoadPictureData argument error\n");
        return SD_ERR_INV;
    }

    // Copy user space argument
    if (copy_from_user(&rArg, (void __user *)arg,
                       sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_SdMisc_LoadPictureData argument error\n");
        return SD_ERR_INV;
    }
//    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
//    COPY_FROM_USER_2ARG(arg, rArg);
    
    bMode    = (UINT8)rArg.au4Arg[0];
    ppbTable        = (UINT8 **)rArg.au4Arg[1];

    i4Ret = SD_OK;
    return i4Ret;    
}

//mtk01140 20071016 SDAL sharpness interface
//static SdResult _SdMisc_SetPictureEnhance(unsigned long arg)
#define SS_SATURATION_DFT           55

static SdResult _SdMisc_SetPictureEnhanceStatus(SdMisc_PictureEnhance_t *pStatus)
{
    INT32             i4Ret;
#if 0 // SDAL_FIXME

    SdMisc_PictureEnhance_t *SdMisc_PictureEnhance_Param;
    strucHSharpFParam rHSharpFParam;
    struc2DSharpParam r2DSharpParam;
    structLTIParam rETIParam;
    structCTIParam rCTIFParam;
    structCTIParam rCTIRParam;        
    
    
    static SdMisc_PictureEnhance_t  sdal_picture_enhance;    
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)pStatus,  sizeof(SdMisc_PictureEnhance_t) ))
    {
	printk("_SdMisc_SetPictureEnhanceStatus arg access error\n");
	return SD_ERR_SYS;
    }

    // Copy user space argument
    if(copy_from_user(&sdal_picture_enhance, (void __user *)pStatus,
	       sizeof(SdMisc_PictureEnhance_t)))
    {
	printk("_SdMisc_SetPictureEnhanceStatus copy_from_user error\n");
	return SD_ERR_SYS;
    }    
    
    //parse arg
    SdMisc_PictureEnhance_Param = &sdal_picture_enhance;
        
    //set SDAL mode for each PE driver, ask driver only update SD_MAJOR_ONLY registers
    rHSharpFParam.bMode = SD_MAJOR_ONLY;
    r2DSharpParam.bMode = SD_MAJOR_ONLY;
    rETIParam.bMode = SD_MAJOR_ONLY;
    rCTIFParam.bMode = SD_MAJOR_ONLY;
    rCTIRParam.bMode = SD_MAJOR_ONLY;

        
    //extract each parameters in SdMisc_PictureEnhance_t
    //Sharp1 Global Gain
    rHSharpFParam.bGlobalLow =
    SdMisc_PictureEnhance_Param->lowGain;   // 0x23A10 [7:0]
    rHSharpFParam.bGlobalMiddle =
    SdMisc_PictureEnhance_Param->middleGain;    // 0x23A10 [15:8]
    rHSharpFParam.bGlobalHigh =
    SdMisc_PictureEnhance_Param->highGain;  // 0x23A10 [23:16]
   
    //Sharp1 Local Gain
    rHSharpFParam.bLocalLow =
    SdMisc_PictureEnhance_Param->localLow;  // 0x23A10 [31:24]
    rHSharpFParam.bLocalMiddle =
    SdMisc_PictureEnhance_Param->localMiddle;   // 0x23A14 [7:0]
    rHSharpFParam.bLocalHigh = 
    SdMisc_PictureEnhance_Param->localHigh; // 0x23A14 [15:8]
    rHSharpFParam.bGlobalClipPos= 
    SdMisc_PictureEnhance_Param->Clip_th; // 0x23A1C [31:24]
    
    //2D Sharp Gain (Main path only)
    r2DSharpParam.bTDSharpGain1 =
    SdMisc_PictureEnhance_Param->gain1;     // 0x25100 [31:24]
    r2DSharpParam.bTDSharpGain2 =
    SdMisc_PictureEnhance_Param->gain2;     // 0x25108 [31:24]
    r2DSharpParam.bTDSharpGain3 =
    SdMisc_PictureEnhance_Param->gain3;     // 0x25110 [31:24]
    r2DSharpParam.bTDSharpGain4 =
    SdMisc_PictureEnhance_Param->gain4;     // 0x25120 [31:24]
    r2DSharpParam.bTDSharpGain5 =
    SdMisc_PictureEnhance_Param->gain5;     // 0x2512C [31:24]
    r2DSharpParam.bTDSharpGain6 =
    SdMisc_PictureEnhance_Param->gain6;     // 0x25134 [31:24]
    r2DSharpParam.bTDSharpGain7 =
    SdMisc_PictureEnhance_Param->gain7;     // 0x2513C [31:24]
    r2DSharpParam.bTDSharpGain8 =
    SdMisc_PictureEnhance_Param->gain8;     // 0x25144 [31:24]
    r2DSharpParam.bTDSharpLimitPosAll =
    SdMisc_PictureEnhance_Param->limit_pos_all;     // 0x2513C [31:24]
    r2DSharpParam.bTDSharpLimitNegAll =
    SdMisc_PictureEnhance_Param->limit_neg_all;     // 0x25144 [31:24]
    //mtk01140 20071019 for SS SDAL test
    
    vRegWriteFldAlign(LCPROC_49, pStatus->LTIGain, LTI_GAIN);   // 0x2D324 [27:20]

    vRegWriteFldAlign(CTIF_03, pStatus->ECTIGain, ECTIF_T_SEL); // 0x23954 [6:4]

    vRegWriteFldAlign(LCPROC_23, pStatus->SCTICgain, CTI_T_SELECT); // 0x2D28C [2:0]

    vRegWriteFldAlign(LCPROC_23, pStatus->SCTIFgain, CTI_GAIN_SHARP);   // 0x2D28C [10:4]

    vDrvMainHSharpFParam(&rHSharpFParam);
    vDrv2DSharpParam(&r2DSharpParam);
/*  
    //ETI (LTI) Gain
    rETIParam.bLTIGain = 
    SdMisc_PictureEnhance_Param->LTIGain;   // 0x2D324 [27:20]

    //Front ECTI CoarseGain (CTIF_03)
    rCTIFParam.bCoarseGain =
    SdMisc_PictureEnhance_Param->ECTIGain;  // 0x23954 [6:4]
    
    //  Rear CTI CoarseGain (LCPROC_23 CTI_T_SELECT)
    rCTIRParam.bCoarseGain =
    SdMisc_PictureEnhance_Param->SCTICgain; // 0x2D28C [2:0]
    
    //  Rear CTI FineGain (LCPROC_23 CTI_GAIN_SHARP)
    rCTIRParam.bFineGain =
    SdMisc_PictureEnhance_Param->SCTIFgain; // 0x2D28C [10:4]
   
    //Do we also need to control On/Off driver here?????
   
    //call all PE driver function, Main path only
    vDrvMainHSharpFParam(&rHSharpFParam);
    vDrv2DSharpParam(&r2DSharpParam);
    vDrvLTIParam(&rETIParam);
    vDrvCTIFParam(&rCTIFParam);
    vDrvCTIRParam(&rCTIRParam);
*/    
    // UV delay, control main path only
    vDrvUVdelay(SV_VP_MAIN, 
    SdMisc_PictureEnhance_Param->Udelay,        // 0x2D260 [2:0]
    SdMisc_PictureEnhance_Param->Vdelay);       // 0x2D264 [2:0]
    
#if  0  
    //Main path Saturation
    //mtk01140 20071023 fix VGA saturation
    vApiVideoSaturation(SV_VP_MAIN,
    SdMisc_PictureEnhance_Param->MainColor);        
    //record the saturation value in register, in case VGA
    //need to read back this value
    vDrvMainSaturation(
    SdMisc_PictureEnhance_Param->MainColor);    // 0x2D270 [7:0]
#endif  
    //mtk01140 20071024 need to record saturation value here, for vga saturation
    VDP_SetArg(SV_VP_MAIN,VDP_ARG_SATURATION, 
        SdMisc_PictureEnhance_Param->MainColor);
    if(fgApiVideoIsVgaTiming(SV_VP_MAIN)&&bIsScalerInput444(SV_VP_MAIN))//for PC timing, set OSD Value
    {
        //mtk01140 20071112 for vga jeffreyx
        vApiVideoVGAColorMatrix(SV_VP_MAIN);
    }
    else//for video timing, set register value
    {
        vApiVideoSaturation(SV_VP_MAIN,
        SdMisc_PictureEnhance_Param->MainColor);
/*  
        UINT32 uMainColor; 
        if(SdMisc_PictureEnhance_Param->MainColor > 128)
        {
            uMainColor = SS_SATURATION_DFT + (((100-SS_SATURATION_DFT) * (SdMisc_PictureEnhance_Param->MainColor - 128))/128);
        }
        else
        {
            uMainColor = (SS_SATURATION_DFT * (SdMisc_PictureEnhance_Param->MainColor))/128;
        }
        VDP_SetArg(SV_VP_MAIN,VDP_ARG_SATURATION, 
            uMainColor);
*/      
    }

    
#if 0   
    //Sub path Saturation
    //mtk01140 20071023 fix VGA saturation
    vApiVideoSaturation(SV_VP_PIP,
    SdMisc_PictureEnhance_Param->SubColor);     
    //record the saturation value in register, in case VGA
    //need to read back this value
    vDrvPipSaturation(
    SdMisc_PictureEnhance_Param->SubColor);     // 0x2D2D4 [7:0]        
 #endif  
    //mtk01140 20071024 need to record saturation value here, for vga saturation
    VDP_SetArg(SV_VP_PIP,VDP_ARG_SATURATION, 
        SdMisc_PictureEnhance_Param->SubColor);
    if(fgApiVideoIsVgaTiming(SV_VP_PIP)&&bIsScalerInput444(SV_VP_PIP))//for PC timing, set OSD Value
    {
        //mtk01140 20071112 for vga jeffreyx
        vApiVideoVGAColorMatrix(SV_VP_PIP);
    }
    else//for video timing, set register value
    {
        vApiVideoSaturation(SV_VP_PIP,
        SdMisc_PictureEnhance_Param->SubColor);
/*
        UINT32 uSubColor; 
        if(SdMisc_PictureEnhance_Param->SubColor > 128)
        {
            uSubColor = SS_SATURATION_DFT + (((100-SS_SATURATION_DFT) * (SdMisc_PictureEnhance_Param->SubColor - 128))/128);
        }
        else
        {
            uSubColor = (SS_SATURATION_DFT * (SdMisc_PictureEnhance_Param->SubColor))/128;
        }
        VDP_SetArg(SV_VP_PIP,VDP_ARG_SATURATION, 
            uSubColor);
*/
    }

    //printf("DRV size = %d\n",sizeof(SdMisc_PictureEnhance_t));    
    //printf("maincolor=%d\n",SdMisc_PictureEnhance_Param->MainColor);
    //printf("SubColor=%d\n",SdMisc_PictureEnhance_Param->SubColor);
#endif    
    i4Ret = SD_OK;
    return i4Ret;    
}

//mtk01140 20071016 SDAL sharpness interface
//static SdResult _SdMisc_GetPictureEnhance(unsigned long arg)

static SdResult _SdMisc_GetPictureEnhanceStatus(SdMisc_PictureEnhance_t *pStatus)
{
    INT32             i4Ret;
#if 0 // SDAL_FIXME

    SdMisc_PictureEnhance_t sCurrStatus;
    //read every register   
    sCurrStatus.lowGain=
        RegReadFld(MAIN00, SHARP_LOW);  // 0x23A10 [7:0]
    sCurrStatus.middleGain=
        RegReadFld(MAIN00, SHARP_MID);  // 0x23A10 [15:8]
    sCurrStatus.highGain=
        RegReadFld(MAIN00, SHARP_HIGH); // 0x23A10 [23:16]
    sCurrStatus.localLow=
        RegReadFld(MAIN00, LC_SHARP_LOW);   // 0x23A10 [31:24]
    sCurrStatus.localMiddle=
        RegReadFld(MAIN01, LC_SHARP_MID);   // 0x23A14 [7:0]
    sCurrStatus.localHigh=
        RegReadFld(MAIN01, LC_SHARP_HIGH);  // 0x23A14 [15:8]
    sCurrStatus.Clip_th=	
     	RegReadFld(MAIN03, CLIP_POS);  // 0x23A1C [31:24]

    sCurrStatus.limit_pos_all=	
     	RegReadFld(SCPOS_US_41, SCPOS_LIMIT_POS_ALL_7_0);  // 0x2514C [23:16]
    sCurrStatus.limit_neg_all=	
     	RegReadFld(SCPOS_US_41, SCPOS_LIMIT_NEG_ALL_7_0);  // 0x2514C [15:8]
	
    sCurrStatus.gain1=
	 _ar2DSharpPrm.bTDSharpGain1;
        //RegReadFld(SCPOS_US_25, SCPOS_GAIN1_7_0);       // 0x25100 [31:24]
    sCurrStatus.gain2=
    	 _ar2DSharpPrm.bTDSharpGain2;
//        RegReadFld(SCPOS_US_27, SCPOS_GAIN2_7_0);       // 0x25108 [31:24]
    sCurrStatus.gain3=
	 _ar2DSharpPrm.bTDSharpGain3;
//        RegReadFld(SCPOS_US_29, SCPOS_GAIN3_7_0);       // 0x25110 [31:24]
    sCurrStatus.gain4=
    	_ar2DSharpPrm.bTDSharpGain4;
//        RegReadFld(SCPOS_US_31, SCPOS_GAIN4_7_0);       // 0x25120 [31:24]
    sCurrStatus.gain5=
	 _ar2DSharpPrm.bTDSharpGain5;
    //   RegReadFld(SCPOS_US_33, SCPOS_GAIN5_7_0);       // 0x2512C [31:24]
    sCurrStatus.gain6=
	 _ar2DSharpPrm.bTDSharpGain6;
       // RegReadFld(SCPOS_US_35, SCPOS_GAIN6_7_0);       // 0x25134 [31:24]
    sCurrStatus.gain7=
	_ar2DSharpPrm.bTDSharpGain7;
       //RegReadFld(SCPOS_US_37, SCPOS_GAIN7_7_0);       // 0x2513C [31:24]
    sCurrStatus.gain8=
	 _ar2DSharpPrm.bTDSharpGain8;
       // RegReadFld(SCPOS_US_39, SCPOS_GAIN8_7_0);       // 0x25144 [31:24]
    
    sCurrStatus.LTIGain=
        RegReadFldAlign(LCPROC_49, LTI_GAIN);   // 0x2D324 [27:20]
    
    sCurrStatus.ECTIGain=
        RegReadFldAlign(CTIF_03, ECTIF_T_SEL);  // 0x23954 [6:4]
    sCurrStatus.SCTICgain=
        RegReadFldAlign(LCPROC_23, CTI_T_SELECT);   // 0x2D28C [2:0]
    sCurrStatus.SCTIFgain=
        RegReadFldAlign(LCPROC_23, CTI_GAIN_SHARP); // 0x2D28C [10:4]
    sCurrStatus.Udelay=
        RegReadFld(LCPROC_18, MLC_DELAY_U);     // 0x2D260 [2:0]
    sCurrStatus.Vdelay=
        RegReadFld(LCPROC_19, MLC_DELAY_V);     // 0x2D264 [2:0]
//  pStatus->MainColor=
//      RegReadFld(LCPROC_1C, MLC_SATR);    // 0x2D270 [7:0]
//  pStatus->SubColor=
//      RegReadFld(LCPROC_35, PLC_SATR);    // 0x2D2D4 [7:0]

    INT16 bTemp;
    {
	VDP_GetArg(SV_VP_MAIN,VDP_ARG_SATURATION, 
	&bTemp);
	sCurrStatus.MainColor = bTemp;

	VDP_GetArg(SV_VP_PIP,VDP_ARG_SATURATION, 
	&bTemp);
	sCurrStatus.SubColor = bTemp;
    }

    //printf("_SdMisc_GetPictureEnhanceStatus = %d\n",sCurrStatus.MainColor);
    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pStatus,  sizeof(SdMisc_PictureEnhance_t) ))
    {
        printk("_SdMisc_GetPictureEnhanceStatus arg access error\n");
        return SD_ERR_SYS;
    }

    // Copy user space argument
    if(copy_to_user((void __user *)pStatus, &sCurrStatus,
           sizeof(SdMisc_PictureEnhance_t)))
    {
        printk("_SdMisc_GetPictureEnhanceStatus copy_to_user error\n");
        return SD_ERR_SYS;
    }   
#endif    
    i4Ret = SD_OK;
    return i4Ret;    
}

//mtk01140 20071016 SDAL LNA+
static SdResult _SdMisc_GetLNAPlusStatus(SdMisc_LNAPlusSettings_t *pStatus)
{
        INT32             i4Ret;
#if 0 // SDAL_FIXME

    SdMisc_LNAPlusSettings_t sCurrStatus;
    
    //read every register
    
    sCurrStatus.tnr.Y_Gain0=
        RegReadFld(NR08, MOTION_GAIN0); // 0x238A0  [7:0]
    sCurrStatus.tnr.Y_Gain1=
        RegReadFld(NR08, MOTION_GAIN1); // 0x238A0  [15:8]
    sCurrStatus.tnr.Y_Gain2=
        RegReadFld(NR08, MOTION_GAIN2); // 0x238A0  [23:16]
    sCurrStatus.tnr.Y_Gain3=
        RegReadFld(NR08, MOTION_GAIN3); // 0x238A0  [31:24]
    sCurrStatus.tnr.Y_Gain4=
        RegReadFld(NR09, MOTION_GAIN4); // 0x238A4  [7:0]
    sCurrStatus.tnr.Y_Gain5=
        RegReadFld(NR09, MOTION_GAIN5); // 0x238A4  [15:8]
    sCurrStatus.tnr.Y_Gain6=
        RegReadFld(NR09, MOTION_GAIN6); // 0x238A4  [23:16]
        
    sCurrStatus.tnr.C_Gain0=
        RegReadFld(NR10, MOTION_GAIN0_C);   // 0x238C0  [7:0]
    sCurrStatus.tnr.C_Gain1=
        RegReadFld(NR10, MOTION_GAIN1_C);   // 0x238C0  [15:8]
    sCurrStatus.tnr.C_Gain2=
        RegReadFld(NR10, MOTION_GAIN2_C);   // 0x238C0  [23:16]
    sCurrStatus.tnr.C_Gain3=
        RegReadFld(NR10, MOTION_GAIN3_C);   // 0x238C0  [31:24]
    sCurrStatus.tnr.C_Gain4=
        RegReadFld(NR11, MOTION_GAIN4_C);   // 0x238C4  [7:0]
    sCurrStatus.tnr.C_Gain5=
        RegReadFld(NR11, MOTION_GAIN5_C);   // 0x238C4  [15:8]
    sCurrStatus.tnr.C_Gain6=
        RegReadFld(NR11, MOTION_GAIN6_C);   // 0x238C4  [23:16] 

    
    sCurrStatus.snr.Y1=
        RegReadFldAlign(NR00, TDN_DIR_ENA); // 0x23880  [27]
    sCurrStatus.snr.Y2=
        RegReadFldAlign(NR02, DIR_FILTER_TYPE); // 0x23888  [22:20]
    sCurrStatus.snr.C=
        RegReadFldAlign(NR02, DIR_FILTER_TYPE_C);   // 0x23888  [18:16]
        

    sCurrStatus.sharpG.LOW_Coring=
        RegReadFld(MAIN02, SHARP_LOW_CORING);   // 0x23A18  [7:0]
    sCurrStatus.sharpG.MID_Coring=
        RegReadFld(MAIN02, SHARP_MID_CORING);   // 0x23A18  [15:8]
    sCurrStatus.sharpG.HIGH_Coring=
        RegReadFld(MAIN02, SHARP_HIGH_CORING);  // 0x23A18  [23:16]     
            

    sCurrStatus.sharpL.LOW_Coring=
        RegReadFld(MAIN02, LC_SHARP_LOW_CORING);    // 0x23A18  [31:24]
    sCurrStatus.sharpL.MID_Coring=
        RegReadFld(MAIN03, LC_SHARP_MID_CORING);    // 0x23A1C  [7:0]
    sCurrStatus.sharpL.HIGH_Coring=
        RegReadFld(MAIN03, LC_SHARP_HIGH_CORING);   // 0x23A1C  [15:8]  
            

    sCurrStatus.sharp2D.Coring1=
        RegReadFld(SCPOS_US_25, SCPOS_CORING1_7_0); // 0x25100  [7:0]
    sCurrStatus.sharp2D.Coring2=
        RegReadFld(SCPOS_US_27, SCPOS_CORING2_7_0); // 0x25108  [7:0]
    sCurrStatus.sharp2D.Coring3=
        RegReadFld(SCPOS_US_29, SCPOS_CORING3_7_0); // 0x25110  [7:0]
    sCurrStatus.sharp2D.Coring4=
        RegReadFld(SCPOS_US_31, SCPOS_CORING4_7_0); // 0x25120  [7:0]
    sCurrStatus.sharp2D.Coring5=
        RegReadFld(SCPOS_US_33, SCPOS_CORING5_7_0); // 0x2512C  [7:0]
    sCurrStatus.sharp2D.Coring6=
        RegReadFld(SCPOS_US_35, SCPOS_CORING6_7_0); // 0x25134  [7:0]
    sCurrStatus.sharp2D.Coring7=
        RegReadFld(SCPOS_US_37, SCPOS_CORING7_7_0); // 0x2513C  [7:0]
    sCurrStatus.sharp2D.Coring8=
        RegReadFld(SCPOS_US_39, SCPOS_CORING8_7_0); // 0x25144  [7:0]           
        
    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pStatus,  sizeof(SdMisc_LNAPlusSettings_t) ))
    {
        printk("_SdMisc_GetPictureEnhanceStatus arg access error\n");
        return SD_ERR_SYS;
    }

    // Copy user space argument
    if(copy_to_user((void __user *)pStatus, &sCurrStatus,
           sizeof(SdMisc_LNAPlusSettings_t)))
    {
        printk("_SdMisc_GetPictureEnhanceStatus copy_to_user error\n");
        return SD_ERR_SYS;
    } 
#endif // SDAL_FIXME
            
    i4Ret = SD_OK;
    return i4Ret;    
}

//mtk01140 20071016 SDAL LNA+
static SdResult _SdMisc_SetLNAPlusStatus(SdMisc_LNAPlusSettings_t *pStatus)
{
        INT32             i4Ret;
#if 0 // SDAL_FIXME

    static SdMisc_LNAPlusSettings_t sdal_lna_setting;
    SdMisc_LNAPlusSettings_t * SdMisc_LNAPlusSettings_Param;
    
	    // Check access of user space
	    if (!access_ok(VERIFY_READ, (void __user *)pStatus,  sizeof(SdMisc_LNAPlusSettings_t) ))
	    {
		printk("_SdMisc_SetLNAPlusStatus arg access error\n");
		return SD_ERR_SYS;
	    }

	    // Copy user space argument
	    if(copy_from_user(&sdal_lna_setting, (void __user *)pStatus,
		       sizeof(SdMisc_LNAPlusSettings_t)))
	    {
		printk("_SdMisc_SetLNAPlusStatus copy_from_user error\n");
		return SD_ERR_SYS;
	    }     
    SdMisc_LNAPlusSettings_Param = &sdal_lna_setting;

    //write every register
    
    vRegWriteFldAlign(NR08, SdMisc_LNAPlusSettings_Param->tnr.Y_Gain0, MOTION_GAIN0);    // 0x238A0  [7:0]

    vRegWriteFldAlign(NR08, SdMisc_LNAPlusSettings_Param->tnr.Y_Gain1, MOTION_GAIN1);    // 0x238A0  [15:8]

    vRegWriteFldAlign(NR08, SdMisc_LNAPlusSettings_Param->tnr.Y_Gain2, MOTION_GAIN2);    // 0x238A0  [23:16]

    vRegWriteFldAlign(NR08, SdMisc_LNAPlusSettings_Param->tnr.Y_Gain3, MOTION_GAIN3);    // 0x238A0  [31:24]

    vRegWriteFldAlign(NR09, SdMisc_LNAPlusSettings_Param->tnr.Y_Gain4, MOTION_GAIN4);    // 0x238A4  [7:0]

    vRegWriteFldAlign(NR09, SdMisc_LNAPlusSettings_Param->tnr.Y_Gain5, MOTION_GAIN5);    // 0x238A4  [15:8]

    vRegWriteFldAlign(NR09, SdMisc_LNAPlusSettings_Param->tnr.Y_Gain6, MOTION_GAIN6);    // 0x238A4  [23:16]
    

    vRegWriteFldAlign(NR10, SdMisc_LNAPlusSettings_Param->tnr.C_Gain0, MOTION_GAIN0_C);  // 0x238C0  [7:0]

    vRegWriteFldAlign(NR10, SdMisc_LNAPlusSettings_Param->tnr.C_Gain1, MOTION_GAIN1_C);  // 0x238C0  [15:8]

    vRegWriteFldAlign(NR10, SdMisc_LNAPlusSettings_Param->tnr.C_Gain2, MOTION_GAIN2_C);  // 0x238C0  [23:16]

    vRegWriteFldAlign(NR10, SdMisc_LNAPlusSettings_Param->tnr.C_Gain3, MOTION_GAIN3_C);  // 0x238C0  [31:24]

    vRegWriteFldAlign(NR11, SdMisc_LNAPlusSettings_Param->tnr.C_Gain4, MOTION_GAIN4_C);  // 0x238C4  [7:0]

    vRegWriteFldAlign(NR11, SdMisc_LNAPlusSettings_Param->tnr.C_Gain5, MOTION_GAIN5_C);  // 0x238C4  [15:8]

    vRegWriteFldAlign(NR11, SdMisc_LNAPlusSettings_Param->tnr.C_Gain6, MOTION_GAIN6_C);  // 0x238C4  [23:16] 


    vRegWriteFldAlign(NR00, SdMisc_LNAPlusSettings_Param->snr.Y1, TDN_DIR_ENA);  // 0x23880  [27]

    vRegWriteFldAlign(NR02, SdMisc_LNAPlusSettings_Param->snr.Y2, DIR_FILTER_TYPE);  // 0x23888  [22:20]

    vRegWriteFldAlign(NR02, SdMisc_LNAPlusSettings_Param->snr.C, DIR_FILTER_TYPE_C); // 0x23888  [18:16]
    

    vRegWriteFldAlign(MAIN02, SdMisc_LNAPlusSettings_Param->sharpG.LOW_Coring, SHARP_LOW_CORING);    // 0x23A18  [7:0]

    vRegWriteFldAlign(MAIN02, SdMisc_LNAPlusSettings_Param->sharpG.MID_Coring, SHARP_MID_CORING);    // 0x23A18  [15:8]

    vRegWriteFldAlign(MAIN02, SdMisc_LNAPlusSettings_Param->sharpG.HIGH_Coring, SHARP_HIGH_CORING);  // 0x23A18  [23:16]     
        
    vRegWriteFldAlign(MAIN02, SdMisc_LNAPlusSettings_Param->sharpL.LOW_Coring, LC_SHARP_LOW_CORING); // 0x23A18  [31:24]

    vRegWriteFldAlign(MAIN03, SdMisc_LNAPlusSettings_Param->sharpL.MID_Coring, LC_SHARP_MID_CORING); // 0x23A1C  [7:0]

    vRegWriteFldAlign(MAIN03, SdMisc_LNAPlusSettings_Param->sharpL.HIGH_Coring, LC_SHARP_HIGH_CORING);   // 0x23A1C  [15:8]  
        

    vRegWriteFldAlign(SCPOS_US_25, SdMisc_LNAPlusSettings_Param->sharp2D.Coring1, SCPOS_CORING1_7_0);    // 0x25100  [7:0]

    vRegWriteFldAlign(SCPOS_US_27, SdMisc_LNAPlusSettings_Param->sharp2D.Coring2, SCPOS_CORING2_7_0);    // 0x25108  [7:0]

    vRegWriteFldAlign(SCPOS_US_29, SdMisc_LNAPlusSettings_Param->sharp2D.Coring3, SCPOS_CORING3_7_0);    // 0x25110  [7:0]

    vRegWriteFldAlign(SCPOS_US_31, SdMisc_LNAPlusSettings_Param->sharp2D.Coring4, SCPOS_CORING4_7_0);    // 0x25120  [7:0]

    vRegWriteFldAlign(SCPOS_US_33, SdMisc_LNAPlusSettings_Param->sharp2D.Coring5, SCPOS_CORING5_7_0);    // 0x2512C  [7:0]

    vRegWriteFldAlign(SCPOS_US_35, SdMisc_LNAPlusSettings_Param->sharp2D.Coring6, SCPOS_CORING6_7_0);    // 0x25134  [7:0]

    vRegWriteFldAlign(SCPOS_US_37, SdMisc_LNAPlusSettings_Param->sharp2D.Coring7, SCPOS_CORING7_7_0);    // 0x2513C  [7:0]

    vRegWriteFldAlign(SCPOS_US_39, SdMisc_LNAPlusSettings_Param->sharp2D.Coring8, SCPOS_CORING8_7_0);    // 0x25144  [7:0]           
#endif        
    i4Ret = SD_OK;
    return i4Ret;    
}

//mtk01140 20071101 SS want to control PE setting, instead of always set it in modechg done.
static SdResult _SdMisc_SetPEDefaultValue(UINT8 bPath)
{
#if 0 // SDAL_FIXME
    vApiSetPEAll(bPath);
#endif    
    return SD_OK;
}

#if 0 // move to io_sdextin.c
static UINT16 GetScalerDataValue(UINT16 x,UINT16 y,
            UINT8 index)
{
    UINT16 val[3];
    bGetScalerDRAM(x, y, &val[0],&val[1],&val[2]);

    switch(index)
    {
        default:
        case SD_OUT_FORMAT_Y:   return val[2];
        case SD_OUT_FORMAT_CB:  return val[1];
        case SD_OUT_FORMAT_CR:  return val[0];  
        case SD_OUT_FORMAT_R:   return val[0];
        case SD_OUT_FORMAT_G:   return val[2];
        case SD_OUT_FORMAT_B:   return val[1];  
    }
    
}

SdResult _SdMisc_GetFirstCalibRegion(SdSource_k eInputSrc, SdMisc_OutFormat_k eOutputFormat,
SdMisc_CapturePosition_k eCapturePos, uint8_t* pData)
{
    uint8_t  retval;
    UINT16 panel_width,panel_height;
    UINT16 src_width,src_height;

    if ((!access_ok(VERIFY_WRITE, (void __user *)pData,
                   sizeof(uint8_t ))))
    {
        printk("_SdExtIn_GetFirstCalibRegion argument error\n");
        return SD_NOT_OK;
    }

//   PMX_GetResolution(PMX_1, &panel_width, &panel_height);
    panel_width = PANEL_GetPanelPhysicalWidth();
    panel_height = PANEL_GetPanelPhysicalHeight();

    if((eCapturePos.SD_CAPTURE_POS_X>=panel_width)||    (eCapturePos.SD_CAPTURE_POS_Y>=panel_height))
        return SD_NOT_OK;

    //calibration must using SV_VP_MAIN
    src_width=wDrvVideoInputWidth(SV_VP_MAIN);
    src_height=wDrvVideoInputHeight(SV_VP_MAIN);
    eCapturePos.SD_CAPTURE_POS_X=(eCapturePos.SD_CAPTURE_POS_X*src_width)/panel_width;
    eCapturePos.SD_CAPTURE_POS_Y=(eCapturePos.SD_CAPTURE_POS_Y*src_height)/panel_height;

    switch(VSS_MAJOR(Sd_VdoPathSrcMap(eInputSrc)))
    {
        case VSS_VGA:   /* RGB*/
            if((eOutputFormat<SD_OUT_FORMAT_R)||(eOutputFormat>SD_OUT_FORMAT_B))
                return SD_NOT_OK;
            bInitScalerDump();
            retval=GetScalerDataValue(eCapturePos.SD_CAPTURE_POS_X,
                    eCapturePos.SD_CAPTURE_POS_Y,
                    eOutputFormat);
            bReleaseScalerDump();
            break;

        default:        /* YCBCR*/
            if(eOutputFormat>=SD_OUT_FORMAT_R)
                return SD_NOT_OK;
            bInitScalerDump();
            retval=GetScalerDataValue(eCapturePos.SD_CAPTURE_POS_X,
                    eCapturePos.SD_CAPTURE_POS_Y,
                    eOutputFormat)>>2;
            bReleaseScalerDump();
            break;
    }

    if (copy_to_user((void __user *)pData,&retval, sizeof(uint8_t )))
    {
        return SD_NOT_OK;
    }   
    return SD_OK;
}

SdResult _SdMisc_SetFirstCalibValue(SdSource_k eInputSrc, SdMisc_OutFormat_k eOutputFormat,
 SdMisc_ControlValue_k eGainOffset, uint16_t Data)
{
    /* following code not really write to EEPROM, in currecnt config, it's dram shadow*/
    switch( VSS_MAJOR(Sd_VdoPathSrcMap(eInputSrc)))
    {
        case VSS_ATV:
        case VSS_ATD:
        case VSS_CVBS:
        case VSS_SV:
            /* assume eOutputFormat always SD_OUT_FORMAT_Y */
            switch(eGainOffset)
            {
                case SD_CTRL_VALUE_OFFSET:
                    bDrvSetTVDOffset(Data);
                    break;
                case SD_CTRL_VALUE_GAIN:
                    bDrvSetTVDGain(Data);
                    break;
            }
            break;
        case VSS_YPBPR:
            switch(eOutputFormat)
            {
                case SD_OUT_FORMAT_Y:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(0,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_HDTV_CLAMP0,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(0,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN0,(UINT8)Data);
                            break;
                    }
                    break;

                case SD_OUT_FORMAT_CB:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(1,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_HDTV_CLAMP1,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(1,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN1,(UINT8)Data);
                            break;
                    }
                    break;

                    case SD_OUT_FORMAT_CR:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(2,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_HDTV_CLAMP2,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(2,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN2,(UINT8)Data);
                            break;
                    }
                    break;

                    default:
                        return SD_NOT_OK;
            }
            break;
        case VSS_VGA:
            switch(eOutputFormat)
            {
                case SD_OUT_FORMAT_R:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(2,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_VGA_CLAMP2,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(2,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN2,(UINT8)Data);
                            break;
                    }
                    break;
                case SD_OUT_FORMAT_G:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(1,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_VGA_CLAMP1,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(1,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN1,(UINT8)Data);
                            break;
                    }
                    break;
                    case SD_OUT_FORMAT_B:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(0,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_VGA_CLAMP0,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(0,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN0,(UINT8)Data);
                            break;
                    }
                    break;
                    default:
                        return SD_NOT_OK;
            }
            break;

        default:
                return SD_NOT_OK;       
    }


    return SD_OK;

}


static UINT16 GetLastStageDataValue(UINT16 x,UINT16 y,
            UINT8 index)
{
    UINT16 val[3];
    bGetLastStageDRAM(x, y, &val[0],&val[1],&val[2]);
    switch(index)
    {
        default:
        case SD_OUT_FORMAT_R:   return val[0];
        case SD_OUT_FORMAT_G:   return val[2];
        case SD_OUT_FORMAT_B:   return val[1];  
    }

}

SdResult _SdMisc_GetSecondCalibRegion(SdSource_k eInputSrc, SdMisc_OutFormat_k eOutputFormat,
SdMisc_CapturePosition_k eCapturePos, uint8_t* pData)
{
    uint8_t  retval;
    UINT16 panel_width,panel_height;

    //uint16_t x,y;
    // Check access of user space
    if((eOutputFormat<SD_OUT_FORMAT_R)||(eOutputFormat>SD_OUT_FORMAT_B))
        return SD_NOT_OK;
    if ((!access_ok(VERIFY_WRITE, (void __user *)pData,
                   sizeof(uint16_t ))))
    {
        printk("_SdExtIn_GetFirstCalibRegion argument error\n");
        return SD_NOT_OK;
    }
//    PMX_GetResolution(PMX_1, &panel_width, &panel_height);
    panel_width = PANEL_GetPanelPhysicalWidth();
    panel_height = PANEL_GetPanelPhysicalHeight();

    if((eCapturePos.SD_CAPTURE_POS_X>=panel_width)||    (eCapturePos.SD_CAPTURE_POS_Y>=panel_height))
        return SD_NOT_OK;

    //mtk01140 20071020 don't bypass MLC while caliberation
    //vDrvMainOnOff(SV_OFF);
    bInitLastStageDump();
    retval=GetLastStageDataValue(eCapturePos.SD_CAPTURE_POS_X,
            eCapturePos.SD_CAPTURE_POS_Y,
            eOutputFormat)>>2;
    bReleaseLastStageDump();

    if (copy_to_user((void __user *)pData,&retval, sizeof(uint8_t )))
    {
        return SD_NOT_OK;
    }   
    return SD_OK;

}
SdResult _SdMisc_SetSecondCalibValue(SdSource_k eInputSrc, SdMisc_OutFormat_k eOutputFormat,
 SdMisc_ControlValue_k eGainOffset, uint8_t Data)
{
    if(eOutputFormat<SD_OUT_FORMAT_R)
        return SD_NOT_OK;
    switch(eGainOffset)
    {
        case SD_CTRL_VALUE_OFFSET:
            if(Data<0x80) 
                Data+=0x80 ;
            else    Data-=0x80 ;
            
            vApiVideoRGBOffset(SV_VP_MAIN,eOutputFormat-SD_OUT_FORMAT_R,(UINT8)Data);
            break;
        case SD_CTRL_VALUE_GAIN:
            vApiVideoRGBGain(SV_VP_MAIN,eOutputFormat-SD_OUT_FORMAT_R,(UINT8)Data);
            break;
    }
    return SD_OK;
}
#endif

static SdResult _SdMisc_ReadReg(unsigned long arg)
{
    INT32             i4Ret = 0;
    SDAL_IOCTL_2ARG_T rArg;
    UINT32            *addr, *val;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    addr = (UINT32)rArg.au4Arg[0];
    val  = (UINT32)rArg.au4Arg[1];

    if(addr > 0x30000)
    {
        *val = 0;
        return -1;
    }

    printk("_SdMisc_ReadReg, addr = 0x%08x\n", addr);

    addr = (UINT32 *)((UINT32)addr | 0xF0000000);

    *val = *addr;
    
    return i4Ret;
}

SdResult _SdMisc_GetAGCStatus(SdMisc_AGCInfo_t* pStatus)
{
            SdMisc_AGCInfo_t  retval;
        
            if ((!access_ok(VERIFY_WRITE, (void __user *)pStatus,
                   sizeof(SdMisc_AGCInfo_t ))))
            {
                printk("_SdMisc_GetAGCStatus argument error\n");
                return SD_NOT_OK;
            }
            GetTVDAGCstatus();
            retval.bmanualAGC=  _rTvd3dAGCInfo.bmanualAGC;
            retval.minHWidth=   _rTvd3dAGCInfo.minHWidth;
            retval.maxHWidth=   _rTvd3dAGCInfo.maxHWidth;
            retval.thHigh=  _rTvd3dAGCInfo.thHigh;
            retval.thSuper= _rTvd3dAGCInfo.thSuper;
            if (copy_to_user((void __user *)pStatus,&retval, sizeof(SdMisc_AGCInfo_t )))
            {
                return SD_NOT_OK;
            }   
            return SD_OK;
        }

SdResult _SdMisc_SetAGCStatus(SdMisc_AGCInfo_t* pStatus)
{

            SdMisc_AGCInfo_t  retval;
            copy_from_user(&retval,(void __user *)pStatus, sizeof(SdMisc_AGCInfo_t ));
    _rTvd3dAGCInfo.bmanualAGC=  retval.bmanualAGC;
    _rTvd3dAGCInfo.minHWidth=   retval.minHWidth;
    _rTvd3dAGCInfo.maxHWidth=   retval.maxHWidth;
    _rTvd3dAGCInfo.thHigh=  retval.thHigh;
    _rTvd3dAGCInfo.thSuper= retval.thSuper;

    SetTVDAGCstatus();

    return SD_OK;

}
SdResult _SdMisc_GetNoiseLevel(UINT32* pStatus)
{

            UINT32  retval;
            retval=(UINT32)bHwTvdNRLevel();
            if (copy_to_user((void __user *)pStatus,&retval, sizeof(UINT32)))
            {
                return SD_NOT_OK;
            }   
            return SD_OK;

}

static void _SdMisc_GetPcr(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    STC_CLOCK rClock;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetPcr argument error\n");
        return;
    }

    STC_GetPcr(&rClock);

    rArg2.au4Arg[0] = rClock.u4Base;                    // Bit0~31 of Base
    rArg2.au4Arg[1] = rClock.u2Ext;                     // Externsion
    rArg2.au4Arg[1] <<= 1;
    rArg2.au4Arg[1] += (rClock.ucBaseHi & 1);           // Bit32 of Base

    if (copy_to_user((void __user *)arg, &rArg2,
                     sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetPcr argument error\n");
        return;
    }
}

static void _SdMisc_GetStc(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    STC_CLOCK rClock;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetStc argument error\n");
        return;
    }

    if (STC_GetSrc(0, &rClock) != STC_VALID)
    {
        printk("SdMisc_GetStc argument error\n");
        return;
    }

    rArg2.au4Arg[0] = rClock.u4Base;                    // Bit0~31 of Base
    rArg2.au4Arg[1] = rClock.u2Ext;                     // Externsion
    rArg2.au4Arg[1] <<= 1;
    rArg2.au4Arg[1] += (rClock.ucBaseHi & 1);           // Bit32 of Base

    if (copy_to_user((void __user *)arg, &rArg2,
                     sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetStc argument error\n");
        return;
    }
}    

static void _SdMisc_GetStcPcrDelta(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    INT64 i8Delta;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetStcPcrDelta argument error\n");
        return;
    }

    STC_GetPcrStcDelta(&i8Delta);

    rArg2.au4Arg[0] = (((UINT64)i8Delta) & 0xFFFFFFFF);
    rArg2.au4Arg[1] = ((((UINT64)i8Delta) >> 32) & 0xFFFFFFFF);

    if (copy_to_user((void __user *)arg, &rArg2,
                     sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetStcPcrDelta argument error\n");
        return;
    }
}

static void _SdMisc_IsVideoFreeRun(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_IsVideoFreeRun argument error\n");
        return;
    }

    if (_bSdMiscVideoSyncOn == 255)
    {
        // _bSdMiscVideoSyncOn = default value, check SdAvDec API
        if (rAvDecSettings.bAVSyncOn == 1)
        {
            if (STC_GetVideoSync())
            {
                rArg2.au4Arg[0] = 0;
            }
            else
            {
                rArg2.au4Arg[0] = 1;
            }
        }
        else
        {
            rArg2.au4Arg[0] = 1;
        }
    }
    else
    {
        if (_bSdMiscVideoSyncOn == 1)
        {
            if (STC_GetVideoSync())
            {
                rArg2.au4Arg[0] = 0;
            }
            else
            {
                rArg2.au4Arg[0] = 1;
            }
        }
        else
        {
            rArg2.au4Arg[0] = 1;
        }
    }

    if (copy_to_user((void __user *)arg, &rArg2,
                     sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_IsVideoFreeRun argument error\n");
        return;
    }
}
    
static void _SdMisc_VideoSynchronize(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_VideoSynchronize argument error\n");
        return;
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg,
                       sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_VideoSynchronize argument error\n");
        return;
    }

    if (rArg2.au4Arg[0] != 0)
    {
        _bSdMiscVideoSyncOn = 1;
    }
    else
    {
        _bSdMiscVideoSyncOn = 0;
    }


    VDEC_SyncStc(0, _bSdMiscVideoSyncOn, 0);

}

static void _SdMisc_GetVideoPts(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    STC_CLOCK rClock;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetVideoPts argument error\n");
        return;
    }

    STC_GetVideoPts(&rClock);

    rArg2.au4Arg[0] = rClock.u4Base;                    // Bit0~31 of Base
    rArg2.au4Arg[1] = (rClock.ucBaseHi & 1);           // Bit32 of Base

    if (copy_to_user((void __user *)arg, &rArg2,
                     sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetVideoPts argument error\n");
        return;
    }
}

static void _SdMisc_GetVideoPtsStcDelta(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    INT64 i8Delta;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetVideoPtsStcDelta argument error\n");
        return;
    }

    STC_GetVideoPtsStcDelta(&i8Delta);

    rArg2.au4Arg[0] = (((UINT64)i8Delta) & 0xFFFFFFFF);
    rArg2.au4Arg[1] = ((((UINT64)i8Delta) >> 32) & 0xFFFFFFFF);

    if (copy_to_user((void __user *)arg, &rArg2,
                     sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetVideoPtsStcDelta argument error\n");
        return;
    }
}

static void _SdMisc_IsAudioFreeRun(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_IsAudioFreeRun argument error\n");
        return;
    }

    if (_bSdMiscAudioSyncOn == 255)
    {
        // _bSdMiscVideoSyncOn = default value, check SdAvDec API
        if (rAvDecSettings.bAVSyncOn == 1)
        {
            if (STC_GetAudioSync())
            {
                rArg2.au4Arg[0] = 0;
            }
            else
            {
                rArg2.au4Arg[0] = 1;
            }
        }
        else
        {
            rArg2.au4Arg[0] = 1;
        }
    }
    else
    {
        if (_bSdMiscAudioSyncOn == 1)
        {
            if (STC_GetAudioSync())
            {
                rArg2.au4Arg[0] = 0;
            }
            else
            {
                rArg2.au4Arg[0] = 1;
            }
        }
        else
        {
            rArg2.au4Arg[0] = 1;
        }
    }

    if (copy_to_user((void __user *)arg, &rArg2,
                     sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_IsAudioFreeRun argument error\n");
        return;
    }
}

static void _SdMisc_AudioSynchronize(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_AudioSynchronize argument error\n");
        return;
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg,
                       sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_AudioSynchronize argument error\n");
        return;
    }

    if (rArg2.au4Arg[0] != 0)
    {
        _bSdMiscAudioSyncOn = TRUE;
        AUD_SetAvSynMode(AUD_DEC_MAIN, AV_SYNC_SLAVE);
    }
    else
    {
        _bSdMiscAudioSyncOn = FALSE;
        AUD_SetAvSynMode(AUD_DEC_MAIN, AV_SYNC_FREE_RUN);
    }

}

static void _SdMisc_GetAudioPts(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    STC_CLOCK rClock;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetAudioPts argument error\n");
        return;
    }

    STC_GetAudioPts(&rClock);

    rArg2.au4Arg[0] = rClock.u4Base;                    // Bit0~31 of Base
    rArg2.au4Arg[1] = (rClock.ucBaseHi & 1);           // Bit32 of Base

    if (copy_to_user((void __user *)arg, &rArg2,
                     sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetAudioPts argument error\n");
        return;
    }
}

static void _SdMisc_GetAudioPtsStcDelta(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    INT64 i8Delta;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetAudioPtsStcDelta argument error\n");
        return;
    }

    STC_GetAudioPtsStcDelta(&i8Delta);

    rArg2.au4Arg[0] = (((UINT64)i8Delta) & 0xFFFFFFFF);
    rArg2.au4Arg[1] = ((((UINT64)i8Delta) >> 32) & 0xFFFFFFFF);

    if (copy_to_user((void __user *)arg, &rArg2,
                     sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("SdMisc_GetAudioPtsStcDelta argument error\n");
        return;
    }
}

SdResult _SdMisc_FacModeGetMaxMin(arg)
{
    SDAL_IOCTL_3ARG_T rArg;
    SdMisc_FacMode_k  eFacMode;
    UINT16            *pMin;
    UINT16            *pMax;
    UINT16            u2Min = 0;
    UINT16            u2Max = 0;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    
    eFacMode = (UINT32)rArg.au4Arg[0];
    pMax     = (UINT32)rArg.au4Arg[1];
    pMin     = (UINT32)rArg.au4Arg[2];

    switch (eFacMode)
    {
        /* MTS factory mode */    
        case SD_FAC_MODE_MTS_NUM_CHECK:
            u2Min = 30;
            u2Max = 80;
            break;    
        case SD_FAC_MODE_MTS_NUM_PILOT:
            u2Min = 0;
            u2Max = 50;
            break;   
        case SD_FAC_MODE_MTS_PILOT_HIGH:
            u2Min = 120;
            u2Max = 200;
            break;    
        case SD_FAC_MODE_MTS_PILOT_LOW:
            u2Min = 80;
            u2Max = 150;
            break;   
        case SD_FAC_MODE_MTS_NUM_SAP:
            u2Min = 0;
            u2Max = 50;
            break;    
        case SD_FAC_MODE_MTS_SAP_HIGH:
            u2Min =150;
            u2Max = 210;
            break;    
        case SD_FAC_MODE_MTS_SAP_LOW:
            u2Min = 70;
            u2Max = 130;
            break;   
        case SD_FAC_MODE_MTS_HDEV:
            u2Min = 0;
            u2Max = 1;
            break;    
        case SD_FAC_MODE_MTS_CARRIER_SHIFT:
            u2Min = 0;
            u2Max = 1;
            break;   
        case SD_FAC_MODE_MTS_FM_SATU:
            u2Min = 0;
            u2Max = 1;
            break;
        case SD_FAC_MODE_FM_SATU_MUTE:    
            u2Min = 0;
            u2Max = 1;
            break;
        case SD_FAC_MODE_MTS_FM_SATU_HIGH:
            u2Min = 25;
            u2Max = 255;
            break;    
        case SD_FAC_MODE_MTS_FM_SATU_LOW:
            u2Min = 1;
            u2Max = 25;
            break;    
        case SD_FAC_MODE_MTS_FM_MUTE:
            u2Min = 0;
            u2Max = 1;
            break;    
        case SD_FAC_MODE_MTS_FM_MUTE_HIGH:
            u2Min = 20;
            u2Max = 80;
            break;    
        case SD_FAC_MODE_MTS_FM_MUTE_LOW:
            u2Min = 110;
            u2Max = 180;
            break;                
        case SD_FAC_MODE_MTS_FINE_VOLUME:
            u2Min = 0;
            u2Max = 40;
            break;    
        case SD_FAC_MODE_SAP_FINE_VOLUME:
            u2Min = 0;
            u2Max = 40;
            break; 
        case SD_FAC_MODE_MTS_PILOT_OFFSET:
            u2Min = 0;
            u2Max = 1;
            break; 
        case SD_FAC_MODE_SAP_MUTE_LOW:
            u2Min = 0;
            u2Max = 110;
            break;
        case SD_FAC_MODE_SAP_MUTE_HIGH:
            u2Min = 0;
            u2Max = 255;
            break; 
        /* A2 factory mode */    
        case SD_FAC_MODE_A2_NUM_CHECK:
            u2Min = 1;
            u2Max = 60;
            break;    
        case SD_FAC_MODE_A2_NUM_DOUBLE:
            u2Min = 5;
            u2Max = 60;
            break;   
        case SD_FAC_MODE_A2_MONO_WEIGHT:
            u2Min = 1;
            u2Max = 20;
            break;    
        case SD_FAC_MODE_A2_STEREO_WEIGHT:
            u2Min = 1;
            u2Max = 20;
            break;   
        case SD_FAC_MODE_A2_DUAL_WEIGHT:
            u2Min = 1;
            u2Max = 20;
            break;   
        case SD_FAC_MODE_A2_HDEV:
            u2Min = 0;
            u2Max = 1;
            break;    
        case SD_FAC_MODE_A2_CARRIER_SHIFT:
            u2Min = 0;
            u2Max = 1;
            break;   
        case SD_FAC_MODE_A2_FM_MUTE:
            u2Min = 0;
            u2Max = 1;
            break;    
        case SD_FAC_MODE_A2_FM_MUTE_HIGH:
            u2Min = 0;
            u2Max = 96;
            break;    
        case SD_FAC_MODE_A2_FM_MUTE_LOW:
            u2Min = 0;
            u2Max = 96;
            break;        
        case SD_FAC_MODE_A2_FINE_VOLUME:
            u2Min = 0;
            u2Max = 40;
            break;   
        /* PAL factory mode */    
        case SD_FAC_MODE_PAL_CORRECT:
            u2Min = 1;
            u2Max = 7;
            break;    
        case SD_FAC_MODE_PAL_SYNC_LOOP:
            u2Min = 1;
            u2Max = 1001;
            break;    
        case SD_FAC_MODE_PAL_ERROR:
            u2Min = 2;
            u2Max = 40;
            break;    
        case SD_FAC_MODE_PAL_PARITY_ERROR:
            u2Min = 1;
            u2Max = 255;
            break;    
        case SD_FAC_MODE_PAL_NUM_FRAMES:
            u2Min = 0;
            u2Max = 2048;
            break;    
        case SD_FAC_MODE_PAL_HDEV:
            u2Min = 0;
            u2Max = 1;
            break;   
        case SD_FAC_MODE_PAL_AM_MUTE:
            u2Min = 0;
            u2Max = 1;
            break;    
        case SD_FAC_MODE_PAL_AM_MUTE_HIGH:
            u2Min = 0;
            u2Max = 255;
            break;    
        case SD_FAC_MODE_PAL_AM_MUTE_LOW:
            u2Min = 0;
            u2Max = 255;
            break;                  
        case SD_FAC_MODE_PAL_CARRIER_SHIFT:
            u2Min = 0;
            u2Max = 1;
            break;   
        case SD_FAC_MODE_PAL_FM_MUTE:
            u2Min = 0;
            u2Max = 1;
            break;    
        case SD_FAC_MODE_PAL_FM_MUTE_HIGH:
            u2Min = 0;
            u2Max = 96;
            break;    
        case SD_FAC_MODE_PAL_FM_MUTE_LOW:
            u2Min = 0;
            u2Max = 96;
            break;        
        case SD_FAC_MODE_PAL_FINE_VOLUME:
            u2Min = 0;
            u2Max = 40;
            break;        
        case SD_FAC_MODE_AM_FINE_VOLUME:
            u2Min = 0;
            u2Max = 40;
            break;    
        case SD_FAC_MODE_NICAM_FINE_VOLUME:
            u2Min = 0;
            u2Max = 40;
            break;
        default:
            break;
    }

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pMax,
                   sizeof(UINT16)))
    {
        printk("_SdMisc_FacModeGetMaxMin argument error\n");
        return -1;
    }
    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pMin,
                   sizeof(UINT16)))
    {
        printk("_SdMisc_FacModeGetMaxMin argument error\n");
        return -1;
    }

    if (copy_to_user((void __user *)pMax, &u2Max,
                     sizeof(UINT16)))
    {
        printk("_SdMisc_FacModeGetMaxMin argument error\n");
        return -1;
    }

    if (copy_to_user((void __user *)pMin, &u2Min,
                     sizeof(UINT16)))
    {
        printk("_SdMisc_FacModeGetMaxMin argument error\n");
        return -1;
    }

    return SD_OK;
}

SdResult _SdMisc_FacModeSet(arg)
{
    SDAL_IOCTL_2ARG_T rArg;
    SdMisc_FacMode_k  eFacMode;
    UINT16            u2Val;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    
    eFacMode = (UINT32)rArg.au4Arg[0];
    u2Val    = (UINT32)rArg.au4Arg[1];

    switch (eFacMode)
    {
        /* MTS factory mode */    
        case SD_FAC_MODE_MTS_NUM_CHECK:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_NUM_CHECK,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_MTS_NUM_PILOT:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_NUM_PILOT,(UINT16)u2Val); 
            break;   
        case SD_FAC_MODE_MTS_PILOT_HIGH:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_PILOT_HIGH,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_MTS_PILOT_LOW:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_PILOT_LOW,(UINT16)u2Val); 
            break;   
        case SD_FAC_MODE_MTS_NUM_SAP:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_NUM_SAP,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_MTS_SAP_HIGH:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_SAP_HIGH,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_MTS_SAP_LOW:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_SAP_LOW,(UINT16)u2Val); 
            break;   
        case SD_FAC_MODE_MTS_HDEV:
            AUD_DspSetHdevMode(AUD_ATV_DECODER_MTS, (BOOL)u2Val);
            break;    
        case SD_FAC_MODE_MTS_CARRIER_SHIFT:
            AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_MTS, (BOOL)u2Val);
            break;   
        case SD_FAC_MODE_MTS_FM_SATU:
            AUD_DspSetFMSatuMuteMode(AUD_ATV_DECODER_MTS, (BOOL)u2Val);
            break;    
        case SD_FAC_MODE_MTS_FM_SATU_HIGH:
            AUD_DspSetFMSatuMuteTh(AUD_FACTORY_SATU_MUTE_HIGHER,(UINT8)u2Val);
            break;    
        case SD_FAC_MODE_MTS_FM_SATU_LOW:
            AUD_DspSetFMSatuMuteTh(AUD_FACTORY_SATU_MUTE_LOWER,(UINT8)u2Val);
            break;                
        case SD_FAC_MODE_MTS_FM_MUTE:
            AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE,(UINT8)u2Val);
            break;    
        case SD_FAC_MODE_MTS_FM_MUTE_HIGH:
            AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)u2Val);
            break;    
        case SD_FAC_MODE_MTS_FM_MUTE_LOW:
            AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_LOW,(UINT8)u2Val);
            break;                
        case SD_FAC_MODE_MTS_FINE_VOLUME:
            AUD_DspSetMtsMonoFineVolume((UINT8)u2Val);
            break;    
        case SD_FAC_MODE_SAP_FINE_VOLUME:
            AUD_DspSetSAPFineVolume((UINT8)u2Val);
            break; 
        case SD_FAC_MODE_MTS_PILOT_OFFSET:
            AUD_DspSetMtsPilotDetection((BOOL)u2Val);
            break; 
        case SD_FAC_MODE_SAP_MUTE_LOW:
            AUD_DspSetSapMute(AUD_FACTORY_SAP_MUTE_LOWER,(UINT8)u2Val);
            break; 
        case SD_FAC_MODE_SAP_MUTE_HIGH:
            AUD_DspSetSapMute(AUD_FACTORY_SAP_MUTE_HIGHER,(UINT8)u2Val);
            break; 
        /* A2 factory mode */    
        case SD_FAC_MODE_A2_NUM_CHECK:
            AUD_DspSetA2Detection (AUD_FACTORY_A2_NUM_CHECK,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_A2_NUM_DOUBLE:
            AUD_DspSetA2Detection (AUD_FACTORY_A2_NUM_DOUBLE,(UINT16)u2Val); 
            break;   
        case SD_FAC_MODE_A2_MONO_WEIGHT:
            AUD_DspSetA2Detection (AUD_FACTORY_A2_MONO_WEIGHT,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_A2_STEREO_WEIGHT:
            AUD_DspSetA2Detection (AUD_FACTORY_A2_STEREO_WEIGHT,(UINT16)u2Val); 
            break;   
        case SD_FAC_MODE_A2_DUAL_WEIGHT:
            AUD_DspSetA2Detection (AUD_FACTORY_A2_DUAL_WEIGHT,(UINT16)u2Val); 
            break;   
        case SD_FAC_MODE_A2_HDEV:
            AUD_DspSetHdevMode(AUD_ATV_DECODER_A2, (BOOL)u2Val);
            break;    
        case SD_FAC_MODE_A2_CARRIER_SHIFT:
            AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_A2, (BOOL)u2Val);
            break;   
        case SD_FAC_MODE_A2_FM_MUTE:
            AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE,(UINT8)u2Val);
            break;    
        case SD_FAC_MODE_A2_FM_MUTE_HIGH:
            AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)u2Val);
            break;    
        case SD_FAC_MODE_A2_FM_MUTE_LOW:
            AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_LOW,(UINT8)u2Val);
            break;        
        case SD_FAC_MODE_A2_FINE_VOLUME:
            AUD_DspSetA2FineVolume((UINT8)((UINT32)u2Val));
            break;   
        /* PAL factory mode */    
        case SD_FAC_MODE_PAL_CORRECT:
            AUD_DspSetPalDetection (AUD_FACTORY_PAL_CORRECT,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_PAL_SYNC_LOOP:
            AUD_DspSetPalDetection (AUD_FACTORY_PAL_SYNC_LOOP,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_PAL_ERROR:
            AUD_DspSetPalDetection (AUD_FACTORY_PAL_ERROR,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_PAL_PARITY_ERROR:
            AUD_DspSetPalDetection (AUD_FACTORY_PAL_PARITY_ERROR,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_PAL_NUM_FRAMES:
            AUD_DspSetPalDetection (AUD_FACTORY_PAL_NUM_FRAMES,(UINT16)u2Val); 
            break;    
        case SD_FAC_MODE_PAL_HDEV:
            AUD_DspSetHdevMode(AUD_ATV_DECODER_PAL, (BOOL)u2Val);
            break;   
        case SD_FAC_MODE_PAL_AM_MUTE:
            AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE,(UINT8)u2Val);
            break;    
        case SD_FAC_MODE_PAL_AM_MUTE_HIGH:
            AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE_HIGH,(UINT8)u2Val);
            break;    
        case SD_FAC_MODE_PAL_AM_MUTE_LOW:
            AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE_LOW,(UINT8)u2Val);
            break;                  
        case SD_FAC_MODE_PAL_CARRIER_SHIFT:
            AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_PAL, (BOOL)u2Val);
            break;   
        case SD_FAC_MODE_PAL_FM_MUTE:
            AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE,(UINT8)u2Val);
            break;    
        case SD_FAC_MODE_PAL_FM_MUTE_HIGH:
            AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)u2Val);
            break;    
        case SD_FAC_MODE_PAL_FM_MUTE_LOW:
            AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_LOW,(UINT8)u2Val);
            break;        
        case SD_FAC_MODE_PAL_FINE_VOLUME:
            AUD_DspSetPALFineVolume((UINT8)u2Val);
            break;        
        case SD_FAC_MODE_AM_FINE_VOLUME:
            AUD_DspSetAMFineVolume((UINT8)u2Val);   
            break;    
        case SD_FAC_MODE_NICAM_FINE_VOLUME:
            AUD_DspSetNicamFineVolume((UINT8)u2Val);   
            break;
        case SD_FAC_MODE_FM_SATU_MUTE:    
            AUD_DspSetFMSatuMuteMode(AUD_ATV_DECODER_PAL, (BOOL)u2Val);
            break;  
        
        default:
            break;
    }
    return SD_OK;
}

SdResult _SdMisc_FacModeGet(arg)
{
    SDAL_IOCTL_2ARG_T rArg;
    SdMisc_FacMode_k  eFacMode;
    UINT16            *pVal;
    UINT16            u2Val = 0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    eFacMode = (UINT32)rArg.au4Arg[0];
    pVal     = (UINT32)rArg.au4Arg[1];
  
    // Implementation
    switch (eFacMode)
    {
        /* MTS factory mode */    
        case SD_FAC_MODE_MTS_NUM_CHECK:
            u2Val = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_NUM_CHECK); 
            break;    
        case SD_FAC_MODE_MTS_NUM_PILOT:
            u2Val = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_NUM_PILOT); 
            break;   
        case SD_FAC_MODE_MTS_PILOT_HIGH:
            u2Val = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_PILOT_HIGH); 
            break;    
        case SD_FAC_MODE_MTS_PILOT_LOW:
            u2Val = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_PILOT_LOW); 
            break;   
        case SD_FAC_MODE_MTS_NUM_SAP:
            u2Val = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_NUM_SAP); 
            break;    
        case SD_FAC_MODE_MTS_SAP_HIGH:
            u2Val = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_SAP_HIGH); 
            break;    
        case SD_FAC_MODE_MTS_SAP_LOW:
            u2Val = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_SAP_LOW); 
            break;   
        case SD_FAC_MODE_MTS_HDEV:
            u2Val = AUD_DspGetHdevMode(AUD_ATV_DECODER_MTS);
            break;    
        case SD_FAC_MODE_MTS_CARRIER_SHIFT:
            u2Val = AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_MTS);
            break;   
        case SD_FAC_MODE_MTS_FM_SATU:
            u2Val = AUD_DspGetFMSatuMuteMode(AUD_ATV_DECODER_MTS);
            break; 
        case SD_FAC_MODE_MTS_FM_SATU_HIGH:
            u2Val = AUD_DspGetFMSatuMuteTh(AUD_FACTORY_SATU_MUTE_HIGHER);
            break;    
        case SD_FAC_MODE_MTS_FM_SATU_LOW:
            u2Val = AUD_DspGetFMSatuMuteTh(AUD_FACTORY_SATU_MUTE_LOWER);
            break;                
        case SD_FAC_MODE_MTS_FM_MUTE:
            u2Val = AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE);
            break;    
        case SD_FAC_MODE_MTS_FM_MUTE_HIGH:
            u2Val = AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_HIGH);
            break;    
        case SD_FAC_MODE_MTS_FM_MUTE_LOW:
            u2Val = AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_LOW);
            break;                
        case SD_FAC_MODE_MTS_FINE_VOLUME:
            u2Val = AUD_DspGetMtsMonoFineVolume();
            break;    
        case SD_FAC_MODE_SAP_FINE_VOLUME:
            u2Val = AUD_DspGetSapFineVolume();
            break; 
        case SD_FAC_MODE_MTS_PILOT_OFFSET:
            u2Val = AUD_DspGetMtsPilotDetection();
            break; 
        case SD_FAC_MODE_SAP_MUTE_LOW:
            u2Val = AUD_DspGetSapMute(AUD_FACTORY_SAP_MUTE_LOWER);
            break; 
        case SD_FAC_MODE_SAP_MUTE_HIGH:
            u2Val = AUD_DspGetSapMute(AUD_FACTORY_SAP_MUTE_HIGHER);
            break; 
        /* A2 factory mode */    
        case SD_FAC_MODE_A2_NUM_CHECK:
            u2Val = AUD_DspGetA2Detection (AUD_FACTORY_A2_NUM_CHECK); 
            break;    
        case SD_FAC_MODE_A2_NUM_DOUBLE:
            u2Val = AUD_DspGetA2Detection (AUD_FACTORY_A2_NUM_DOUBLE); 
            break;   
        case SD_FAC_MODE_A2_MONO_WEIGHT:
            u2Val = AUD_DspGetA2Detection (AUD_FACTORY_A2_MONO_WEIGHT); 
            break;    
        case SD_FAC_MODE_A2_STEREO_WEIGHT:
            u2Val = AUD_DspGetA2Detection (AUD_FACTORY_A2_STEREO_WEIGHT); 
            break;   
        case SD_FAC_MODE_A2_DUAL_WEIGHT:
            u2Val = AUD_DspGetA2Detection (AUD_FACTORY_A2_DUAL_WEIGHT); 
            break;   
        case SD_FAC_MODE_A2_HDEV:
            u2Val = AUD_DspGetHdevMode(AUD_ATV_DECODER_A2);
            break;    
        case SD_FAC_MODE_A2_CARRIER_SHIFT:
            u2Val = AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_A2);
            break;   
        case SD_FAC_MODE_A2_FM_MUTE:
            u2Val = AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE);
            break;    
        case SD_FAC_MODE_A2_FM_MUTE_HIGH:
            u2Val = AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_HIGH);
            break;    
        case SD_FAC_MODE_A2_FM_MUTE_LOW:
            u2Val = AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_LOW);
            break;        
        case SD_FAC_MODE_A2_FINE_VOLUME:
            u2Val = AUD_DspGetA2FineVolume();
            break;   
        /* PAL factory mode */    
        case SD_FAC_MODE_PAL_CORRECT:
            u2Val = AUD_DspGetPalDetection (AUD_FACTORY_PAL_CORRECT); 
            break;    
        case SD_FAC_MODE_PAL_SYNC_LOOP:
            u2Val = AUD_DspGetPalDetection (AUD_FACTORY_PAL_SYNC_LOOP); 
            break;    
        case SD_FAC_MODE_PAL_ERROR:
            u2Val = AUD_DspGetPalDetection (AUD_FACTORY_PAL_ERROR); 
            break;    
        case SD_FAC_MODE_PAL_PARITY_ERROR:
            u2Val = AUD_DspGetPalDetection (AUD_FACTORY_PAL_PARITY_ERROR); 
            break;    
        case SD_FAC_MODE_PAL_NUM_FRAMES:
            u2Val = AUD_DspGetPalDetection (AUD_FACTORY_PAL_NUM_FRAMES); 
            break;    
        case SD_FAC_MODE_PAL_HDEV:
           u2Val =  AUD_DspGetHdevMode(AUD_ATV_DECODER_PAL);
            break;   
        case SD_FAC_MODE_PAL_AM_MUTE:
            u2Val = AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE);
            break;    
        case SD_FAC_MODE_PAL_AM_MUTE_HIGH:
            u2Val = AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE_HIGH);
            break;    
        case SD_FAC_MODE_PAL_AM_MUTE_LOW:
            u2Val = AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE_LOW);
            break;                  
        case SD_FAC_MODE_PAL_CARRIER_SHIFT:
            u2Val =  AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_PAL);
            break;   
        case SD_FAC_MODE_PAL_FM_MUTE:
            u2Val = AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE);
            break;    
        case SD_FAC_MODE_PAL_FM_MUTE_HIGH:
            u2Val = AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_HIGH);
            break;    
        case SD_FAC_MODE_PAL_FM_MUTE_LOW:
            u2Val = AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_LOW);
            break;        
        case SD_FAC_MODE_PAL_FINE_VOLUME:
            u2Val = AUD_DspGetPALFineVolume();
            break;        
        case SD_FAC_MODE_AM_FINE_VOLUME:
            u2Val = AUD_DspGetAMFineVolume();   
            break;    
        case SD_FAC_MODE_NICAM_FINE_VOLUME:
            u2Val = AUD_DspGetNicamFineVolume();   
            break;
        case SD_FAC_MODE_FM_SATU_MUTE:    
            u2Val = AUD_DspGetFMSatuMuteMode(AUD_ATV_DECODER_PAL);
            break;  
                
        default:
            break;
    }

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pVal,
                   sizeof(UINT16)))
    {
        printk("_SdMisc_FacModeGet argument error\n");
        return -1;
    }
    if (copy_to_user((void __user *)pVal, &u2Val,
                     sizeof(UINT16)))
    {
        printk("_SdMisc_FacModeGet argument error\n");
        return -1;
    }

    return SD_OK;
}


static void _SdMisc_EnableDbk(unsigned long arg)
{
    SDAL_IOCTL_2ARG_T rArg2;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_SdMisc_EnableDbk argument error\n");
        return;
    }

    // Copy user space argument
    if (copy_from_user(&rArg2, (void __user *)arg,
                       sizeof(SDAL_IOCTL_2ARG_T)))
    {
        printk("_SdMisc_EnableDbk argument error\n");
        return;
    }

    if (rArg2.au4Arg[0] != 0)
    {
        MPV_SetDbk(1, 0);
    }
    else
    {
        MPV_SetDbk(0, 0);
    }

}

static SdResult _SdMisc_AFTDone(SdBool_t bAFTDone)
{
    if(bAFTDone)
    {
        _bSetChannelCnt = 0x40;
    }
    else
    {
        vTvdResetChromaLock();      
        vTvd3dFastChannelChange(SV_ON);
        vTvd3dReset();
    }		
    vTvdHoldMode(!bAFTDone);

    return SD_OK;
}


static SdResult _SdMisc_HDMIForce422(SdBool_t bForce422OnOff)
{
	UINT8 bPath;

	if(fgIsMainDVI())
		bPath=SV_VP_MAIN;
	 else if(fgIsPipDVI())
		 bPath=SV_VP_PIP;
	else	return SD_NOT_OK; 

	if((bDrvVideoSignalStatus(SV_VP_MAIN)==(UINT8)SV_VDO_STABLE)&&bHDMIInputType())
        {
            if(bForce422OnOff)
		{
	            vDviSetAvWidth(wHDMIResoWidth()-3);
			vApiVideoFixColorSpace(bPath,18);//RGB force to Ycbcr 422
		}
            else
            {
			if(!bDrvVideoIsSrcInterlace(bPath))
			{
				vDviSetAvWidth(wHDMIResoWidth());
				vApiVideoFixColorSpace(bPath,17);//force toRGB 444
			}
        }
    }
    return SD_OK;
}

static SdResult _SdMisc_SetADCPhase(uint32_t phase)
{
	vDrvVGASetPhase(phase & 0x1f);
	return SD_OK;
}

static SdResult _SdMisc_SetAtvSoundType(unsigned long arg)
{
    INT32             i4Ret;
    INT32             rArg;
    SdMisc_AtvSoundType_k  eAtvType;
    
    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, rArg);
    
    eAtvType    = (SdMisc_AtvSoundType_k)rArg;

#if 0 // SDAL_FIXME
    return _SD_AUD_SetAtvType(eAtvType);
#else
    return 0;
#endif
}

static SdResult _SdMisc_GetAoutEnableStatus(unsigned long arg)
{
    UNUSED(arg);

    return DSP_GetAoutEnable();
}

static SdResult _SdMisc_SetChromaCorning(unsigned long arg)
{
    BOOL fgEnable;

    fgEnable = (BOOL)arg;
    
#if 0 // SDAL_FIXME
    vDrvChromaCoringOnOff(fgEnable);
#endif

    return SD_OK;
}

static SdResult _SdMisc_SetHdmiZero(unsigned long arg)
{
    UINT32 u4Value;

    u4Value = (UINT32)arg;
    
	vHDMISetEQRsel(u4Value);

    return SD_OK;
}

static SdResult _SdMisc_SetHdmiBoost(unsigned long arg)
{
    UINT32 u4Value;

    u4Value = (UINT32)arg;
    
	vHDMISetEQCsel(u4Value);

    return SD_OK;
}

static SdResult _SdMisc_SetHdmiBias(unsigned long arg)
{
    UINT32 u4Value;

    u4Value = (UINT32)arg;
    
#if 0 // SDAL_FIXME
    vHDMISetEqBias(u4Value);
#endif

    return SD_OK;
}

static SdResult _SdMisc_ShowTestPattern(SdTestPatternType_k ePatternType, SdBool_t bOnOff)
{
    switch(ePatternType){
        case PatternType_S:
#ifndef CC_MT5360        
            vRegWriteFldAlign(SCPOS_DS_03, bOnOff, SCPOS_PTGEN1);
#endif // CC_MT5360            
            break;
        case PatternType_M:
#if 0 // SDAL_FIXME        	
            vRegWriteFldAlign(LCPROC_01, bOnOff, MLC_PTN_ENA);
            vRegWriteFldAlign(LCPROC_17, bOnOff, MLC_PTNV_ENA);
            vRegWriteFldAlign(LCPROC_17, bOnOff, MLC_PTNU_ENA);
#endif            
            break;
        case PatternType_L:   
#if 0 // SDAL_FIXME        	         
            vRegWriteFldAlign(SCPOS_OS_09, bOnOff, SCPOS_PATGEN_EN);
#endif            
            break;
        default:
            return SD_NOT_OK;
        }
        
        return SD_OK;

}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int sdal_ioctl_sdmisc(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    SDAL_IOCTL_8ARG_T rArg12;   //current use SDAL_IOCTL_4ARG_T instead of SDAL_IOCTL_12ARG_T

    if (_IOC_SIZE(cmd)>sizeof(unsigned long))
    {
        // Check access of user space
        if (!access_ok(VERIFY_READ, (void __user *)arg,  _IOC_SIZE(cmd) ))
        {
        printk("sdal_ioctl_sdmissc arg access error\n");
        return SD_ERR_SYS;
        }
        // Copy user space argument
        if(copy_from_user(&rArg12, (void __user *)arg,
               _IOC_SIZE(cmd)))
        {
        printk("sdal_ioctl_sdmisc copy_from_user error\n");
        return SD_ERR_SYS;
        }
    }

    switch (cmd) 
    {
        case SDAL_IO_MISC_EXTIN_LOCKFAST:
            return _SdMisc_ExtIn_LockFast();
        case SDAL_IO_MISC_EXTIN_LOCKMID:
            return _SdMisc_ExtIn_LockMid();
        case SDAL_IO_MISC_SOUND_CONNECT:
            return _SdMisc_Sound_Connect(arg);
        case SDAL_IO_MISC_EXTIN_START:
            return _SdMisc_ExtIn_Start(arg);
        case SDAL_IO_MISC_EXTIN_STOP:
            return _SdMisc_ExtIn_Stop(arg); 
#if 0   // move back to io_sdextin.c          
        case SDAL_IO_MISC_GETFIRSTCALIBREGION:
        {
            SdMisc_CapturePosition_k  cappos;
                cappos.SD_CAPTURE_POS_X=(uint32_t)rArg12.au4Arg[2];
                cappos.SD_CAPTURE_POS_Y=(uint32_t)rArg12.au4Arg[3];
            return _SdMisc_GetFirstCalibRegion((SdSource_k) rArg12.au4Arg[0],
                (SdMisc_OutFormat_k) rArg12.au4Arg[1],
                cappos, 
                (uint8_t*) rArg12.au4Arg[4]);
        }
        case SDAL_IO_MISC_SETFIRSTCALIBVALUE:
            return _SdMisc_SetFirstCalibValue((SdSource_k) rArg12.au4Arg[0], 
                (SdMisc_OutFormat_k) rArg12.au4Arg[1],
                (SdMisc_ControlValue_k) rArg12.au4Arg[2],
                (uint16_t) rArg12.au4Arg[3]);
        case SDAL_IO_MISC_GETSECONDCALIBREGION:
        {
                SdMisc_CapturePosition_k  cappos;
                    cappos.SD_CAPTURE_POS_X=(uint32_t)rArg12.au4Arg[2];
                    cappos.SD_CAPTURE_POS_Y=(uint32_t)rArg12.au4Arg[3];
            return _SdMisc_GetSecondCalibRegion((SdSource_k) rArg12.au4Arg[0],
                (SdMisc_OutFormat_k) rArg12.au4Arg[1],
                cappos, 
                (uint8_t*) rArg12.au4Arg[4]);
        }
        case SDAL_IO_MISC_SETSECONDCALIBVALUE:
            return _SdMisc_SetSecondCalibValue((SdSource_k) rArg12.au4Arg[0], 
                (SdMisc_OutFormat_k) rArg12.au4Arg[1],
                (SdMisc_ControlValue_k) rArg12.au4Arg[2],
                (uint8_t) rArg12.au4Arg[3]);
#endif        

        //mtk01140 20071015 load quality setting    
        case SDAL_IO_MISC_LOADPICTUREDATA:
            return _SdMisc_LoadPictureData(arg);
        //mtk01140 20071015 set gamma interface
        case SDAL_IO_MISC_SETGAMMADATA:
            return _SdMisc_SetGammaData(arg);
        //mtk01140 20071016 SDAL sharpness interface
        case SDAL_IO_MISC_SETPICTUREENHANCE:
            return _SdMisc_SetPictureEnhanceStatus(arg);
        //mtk01140 20071016 SDAL sharpness interface
        case SDAL_IO_MISC_GETPICTUREENHANCE:
            return _SdMisc_GetPictureEnhanceStatus(arg);            
        //mtk01140 20071016 SDAL LNA+
        case SDAL_IO_MISC_SETLNAPLUS:
            return _SdMisc_SetLNAPlusStatus(arg);
        //mtk01140 20071016 SDAL LNA+
        case SDAL_IO_MISC_GETLNAPLUS:
            return _SdMisc_GetLNAPlusStatus(arg);
        //mtk01140 20071101 SS want to control PE setting, instead of always set it in modechg done.
        case SDAL_IO_MISC_SETPEDEFAULTVALUE:
            return _SdMisc_SetPEDefaultValue(arg);
        case SDAL_IO_MISC_READREG:
            return _SdMisc_ReadReg(arg);
        case SDAL_IO_MISC_GETAGCSTATUS:
        {
            return _SdMisc_GetAGCStatus((SdMisc_AGCInfo_t*) rArg12.au4Arg[0]);
        }
        case SDAL_IO_MISC_SETAGCSTATUS:
        {
            return _SdMisc_SetAGCStatus((SdMisc_AGCInfo_t*) rArg12.au4Arg[0]);
        }
        case SDAL_IO_MISC_READNOISELEVEL:
        {
            return _SdMisc_GetNoiseLevel((UINT32*) rArg12.au4Arg[0]);
        }
        case SDAL_IO_MISC_GETPCR:
        {
            _SdMisc_GetPcr(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_GETSTC:
        {
            _SdMisc_GetStc(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_GETPCRSTCDELTA:
        {
            _SdMisc_GetStcPcrDelta(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_GETVIDEOSYNC:
        {
            _SdMisc_IsVideoFreeRun(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_SETVIDEOSYNC:
        {
            _SdMisc_VideoSynchronize(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_GETVIDEOPTS:
        {
            _SdMisc_GetVideoPts(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_GETVPTSSTCDELTA:
        {
            _SdMisc_GetVideoPtsStcDelta(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_GETAUDIOSYNC:
        {
            _SdMisc_IsAudioFreeRun(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_SETAUDIOSYNC:
        {
            _SdMisc_AudioSynchronize(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_GETAUDIOPTS:
        {
            _SdMisc_GetAudioPts(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_GETAPTSSTCDELTA:
        {
            _SdMisc_GetAudioPtsStcDelta(arg);
            return SD_OK;
        }
        case SDAL_IO_MISC_FACMODEGETMAXMIN:
            return _SdMisc_FacModeGetMaxMin(arg);
        case SDAL_IO_MISC_FACMODEGET:
            return _SdMisc_FacModeGet(arg);
        case SDAL_IO_MISC_FACMODESET:
            return _SdMisc_FacModeSet(arg);
        
        case SDAL_IO_MISC_ENABLE_DBK:
        {
            _SdMisc_EnableDbk(arg);
            return SD_OK;            
        }
        case SDAL_IO_MISC_AFTDONE:
            return _SdMisc_AFTDone((SdBool_t)arg);

        case SDAL_IO_MISC_SET_OSD_POSITION:
        {
            _SdMisc_SetOsdPosition(rArg12.au4Arg[0], rArg12.au4Arg[1],
                                   rArg12.au4Arg[2], rArg12.au4Arg[3]);
            return SD_OK;
        }

        case SDAL_IO_MISC_HDMIFORCE422:
            return _SdMisc_HDMIForce422((SdBool_t)arg);
        case SDAL_IO_MISC_SETADCPHASE:
        	return _SdMisc_SetADCPhase((uint32_t) arg);
         case SDAL_IO_MISC_SET_ATV_TYPE:
            return _SdMisc_SetAtvSoundType(arg);

        case SDAL_IO_MISC_GET_AOUT_ENABLE:
            return _SdMisc_GetAoutEnableStatus(arg);    
        case SDAL_IO_MISC_SETCHROMACORNING:
            return _SdMisc_SetChromaCorning(arg);
        case SDAL_IO_MISC_SETHDMIZERO:
            return _SdMisc_SetHdmiZero(arg);
        case SDAL_IO_MISC_SETHDMIEQBOOST:
            return _SdMisc_SetHdmiBoost(arg); 
        case SDAL_IO_MISC_SETHDMIEQBIAS:
            return _SdMisc_SetHdmiBias(arg); 
        case SDAL_IO_MISC_SHOWTESTPATTERN:
            return _SdMisc_ShowTestPattern((SdTestPatternType_k)rArg12.au4Arg[0],(SdBool_t)rArg12.au4Arg[1]); 
            
        default:
            return SD_NOT_SUPPORTED;
    }
    return SD_OK;
}

