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
 * $RCSfile: drv_display.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_display.c
 *  Brief of file drv_display.c.
 *  Details of file drv_display.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "general.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_gpio.h"
#include "nptv_debug.h"

#include "hw_mlvds.h"
#include "hw_dvi.h"

#include "util.h"
#include "video_def.h"
#include "source_select.h"

#include "vdo_misc.h"
#include "drv_video.h"

#include "drv_tdtv_drvif.h"
#include "drv_tdtv.h"
#include "drv_ttd.h"

#include "drv_tdnavi.h"
#include "srm_drvif.h"
#include "drv_scaler_drvif.h"
#include "osd_drvif.h"

#include "drvcust_if.h"

#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_display.h"
#include "drv_dvi.h"
#include "drv_tdnavi.h"
#include "drv_ycproc.h"
#include "drv_mpeg.h"

#include "drv_di.h"
#include "drv_ttd.h"
#include "mute_if.h"

#ifdef CC_MT5396
#include "drv_mjc_if.h"
#endif

#include "drvcust_if.h"

#include "x_chip_id.h"
#include "x_os.h"
#include "panel.h"
#include "ostg_if.h"

#ifdef CC_MT5396
#include "drv_mjc.h"
#endif

#include "drv_tdtv_feature.h"

#if defined(CC_MT5396)
#include "drv_tdtv_5396_def_tbl.h"
#elif defined(CC_MT5368) || defined(CC_MT5389)
#include "drv_tdtv_5368_def_tbl.h"
#else
#include "drv_tdtv_def_tbl.h"
#endif

#define TDTV_PORTING_TODO 0

#define ENUM_TO_STR(x) #x

#define SCAN_PWM_START (250/4) //Delay Depend on Panel Raising , Falling Speed

static E_TDTV_DECODER_INPUT eDecDetectPack[NUM_OF_DECOER];
static E_TDTV_DECODER_INPUT eTDNAVIDetect;

static E_TDTV_DECODER_INPUT eSelectPack[NUM_OF_DECOER];

static S_TDTVVdoCfg sVdoCfg[NUM_OF_PATH];

static E_TDTV_UI_3D_PACKING eUIPacking;
static E_TDTV_UI_3D_2_2D eUI3DTO2D=E_TDTV_UI_3D_2_2D_OFF;
static E_TDTV_UI_3D_FPR  eUI3DFPR=E_TDTV_UI_3D_FPR_OFF;

static E_TDTV_UI_LR eUILRInverse=E_TDTV_UI_LR_NORMAL;
static E_TDTV_UI_LR eB2RLRInv=E_TDTV_UI_LR_NORMAL;

static UINT8 bISB2RContentFulled=0;

static UINT8 fgIsPackDbg=SV_OFF;
static UINT8 fg3DTO2DDbg=SV_OFF;

TDTV_3D_CROP_INTO_T eCropInfo={0,0,0,0,0,0};
static UINT8 fgIs3DPhoto=SV_OFF;
#ifdef IS_SUPPORT_3D_PHOTO
static UINT8 fgIsPhotoCropbyB2R=SV_OFF;
#endif

#ifdef CC_3DTV_FPR_SUPPORT
static UINT8 fgFPRDbg=SV_OFF;
#endif

static E_TDTV_UI_NAVI eUITDNavi = E_TDTV_UI_NAVI_MANAUL;  
UINT8 u1ImgSafetyLvl = 0;

static E_TDTV_UI_3D_2_2D eDecFix322Region[NUM_OF_DECOER]={E_TDTV_UI_3D_2_2D_OFF,E_TDTV_UI_3D_2_2D_OFF,E_TDTV_UI_3D_2_2D_OFF};

UINT8 u1OSDDepth = 16;

UINT8 u1LRFreeRun[NUM_OF_DECOER]={0,0,0};

UINT8 u1IsCustomized3DConfig=SV_OFF;


static inline void vDrvTDTVDecConfig(E_DEC_MOD eMode);
static inline void vDrvTDTVUpdate3D22(UINT32 bPath);
static inline void vDrvTDTVCropProc(void);

void vDrvTDTVOSDCallback(void *pvArgv);

EXTERN VOID DRVCUST_SetExtFrc3DFormat(E_TDTV_UI_3D_PACKING eUIPackingSel);

#ifdef DRV_USE_EXTERNAL_3D_FRC
EXTERN UINT64 u8VdoPathProcCfgExt[NUM_OF_PACKING_TYPE];
EXTERN VOID DRVCUST_SetExtFrc3DDepth(UINT32 u4Depth);
EXTERN VOID DRVCUST_SetExtFrc3DInverse(UINT32 u4IsInverse);
EXTERN VOID DRVCUST_SetExtFrcProtrude(UINT32 u4Protrude);
EXTERN VOID DRVCUST_SetExtFrcDistanceFromTV(UINT32 u4Distant);
EXTERN VOID DRVCUST_SetExtFrcOSDDepth(UINT32 u4OSDDepth);
#endif

#ifdef CC_3DTV_AUDIO_LINK_SUPPORT
extern void AUD_3D_AVLINK_Config(UINT32 u4DepthField);
#endif

#if SUPPORT_OSD_DEPTH
EXTERN INT32 OSD_BASE_Set3DPrLineShift( UINT32 u4Enable, UINT32 u4Shift);
EXTERN INT32 OSD_BASE_Set3DShutterLineShift( UINT32 u4Enable, UINT32 u4Shift);
#endif

EXTERN UINT8 _u1tvmode_scart;

static HANDLE_T _hTDTVModeChgIsrSema;
static UINT8 fgTDTVModeChgTrigger=SV_OFF;

#define LOCK_TDTV_STATE() VERIFY(x_sema_lock(_hTDTVModeChgIsrSema, X_SEMA_OPTION_WAIT) == OSR_OK)
#define UNLOCK_TDTV_STATE() x_sema_unlock(_hTDTVModeChgIsrSema)

#define PATH_CHECK(x) do{ x = x >= NUM_OF_PATH ? SV_VP_MAIN : x; }while(0)

//=============================================================================
// Mode Change Callback Function Register
//=============================================================================
static vTDTVModeChangeCB TDTVCallback[E_TDTV_NUM_OF_CB]=
{
    NULL,//E_TDTV_CB_MM         =0,
    NULL,//E_TDTV_CB_HDMI       =1,
    NULL,//E_TDTV_CB_OSD        =2,
    NULL,//E_TDTV_CB_OSTG       =3,
    NULL,//E_TDTV_CB_DQC        =4,
    NULL,//E_TDTV_CB_TTD        =5,
    NULL,//E_TDTV_CB_3D22D      =6, 
    NULL,//E_TDTV_CB_3D_CROP_MM =7
};


static bTDTVQuery3DCropFuncCB TDTV3DCropCallback[E_TDTV_NUM_OF_CB]=
{
    NULL,//E_TDTV_CB_MM         =0,
    NULL,//E_TDTV_CB_HDMI       =1,
    NULL,//E_TDTV_CB_OSD        =2,
    NULL,//E_TDTV_CB_OSTG       =3,
    NULL,//E_TDTV_CB_DQC        =4,
    NULL,//E_TDTV_CB_TTD        =5,
    NULL,//E_TDTV_CB_3D22D      =6, 
    NULL,//E_TDTV_CB_3D_CROP_MM =7
};

/**
 * @brief : Register 3DTV Mode Change Callback
 * @param eModule: The Callback module
 * @param fpTDTVModeChangeCB: The Callback Function
 * @retval: Return Previous Callback Function; 
 */
vTDTVModeChangeCB fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_MODULE eModule,vTDTVModeChangeCB fpTDTVModeChangeCB)
{
    vTDTVModeChangeCB pfPreCB = TDTVCallback[eModule];
    TDTVCallback[eModule] = fpTDTVModeChangeCB;
    return pfPreCB;
}

/**
 * @brief : Register Callback Function for TDTV check 3D Crop
 * @param eModule: The Callback module
 * @param fpTDTVModeChangeCB: The Callback Function
 * @retval: Return Previous Callback Function; 
 */
bTDTVQuery3DCropFuncCB fpDrvTDTVReg3DCropCB(E_TDTV_CB_MODULE eModule,bTDTVQuery3DCropFuncCB fpTDTVQuery3DCropFuncCB)
{
    bTDTVQuery3DCropFuncCB pfPreCB = TDTV3DCropCallback[eModule];
    TDTV3DCropCallback[eModule] = fpTDTVQuery3DCropFuncCB;

    if(NULL==TDTV3DCropCallback[E_TDTV_CB_3D_CROP_MM])
    {
        eCropInfo.fgIsFullResolution=0;
        eCropInfo.fgIsFixSrcAspectRatio=0;
        eCropInfo.u4CropLeftOffset=0;
        eCropInfo.u4CropRightOffset=0;
        eCropInfo.u4CropTopOffset=0;
        eCropInfo.u4CropBottomOffset=0;
    }
    
    return pfPreCB;
}

//=============================================================================
// TDTV Misc Utility for state chcking and Enum Tranlate
//=============================================================================

UINT32 u4DrvTDTVModelWithoutMJC(void)
{
#if defined(CC_MT5396) && defined(CC_SUPPORT_5396_WITHOUT_MJC)
    return (SRMFBM_GetConf()==FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC);
#elif defined(CC_MT5396)
    return SV_FALSE;
#else
    return SV_TRUE;
#endif
}

static inline void vDrvTDTVUpdateSRM(void)
{
    SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_UNSTABLE ,0,0);
}

static inline E_DEC_MOD eGetDecModule(UINT8 bPath)
{
    UINT32 u4SigType=bGetSignalType(bPath);
    if(SV_ST_MPEG==u4SigType)
    {
        return E_DEC_MOD_MM;        
    }
    else if(SV_ST_DVI==u4SigType)
    {
        return E_DEC_MOD_HDMI;
    }
    else
    {
        return E_DEC_MOD_OTHERS;
    } 
}

static inline E_TDTV_UI_3D_PACKING vDrvTDTVValidatePackMode(E_TDTV_UI_3D_PACKING eSelectPack,E_TDTV_DECODER_INPUT eDetectPack)
{
    if(IS_Support3D()!=SV_TRUE)
    {  
        return E_TDTV_UI_3D_MODE_OFF;
    }
    
    if(
        //Timing only support Auto   
        ((eDetectPack>=AUTO_ONLY_START)&&(eDetectPack<=AUTO_ONLY_END))
        //EPG
        ||(SRM_IsEpgMode()
#ifdef IS_SUPPORT_3D_PHOTO            
            &&(SV_OFF==fgIs3DPhoto)
#endif         
        )
        //Thumbnail
        ||(SRM_MM_MODE_TYPE_THUMBNAIL==SRM_GetMmMode())
        )
    {
        return E_TDTV_UI_3D_MODE_AUTO;
    }
    
    return eSelectPack;
}

static inline void vDrvTDTVSetDecFix322Region(E_DEC_MOD eDec,E_TDTV_UI_3D_2_2D eRgn)
{
    eDecFix322Region[eDec]=eRgn;
}

UINT32 fgDrvTDTVIsFix2DDisplay(UINT32 u4Path)
{
    if(SV_VP_MAIN==u4Path)
    {
        if(((SRM_MM_MODE_TYPE_THUMBNAIL==SRM_GetMmMode())
                ||(bDrvVideoSignalStatus(u4Path)!=SV_VDO_STABLE)

#if !SUPPORT_3D_EPG            
                ||(SRM_IsEpgMode()&&(SV_OFF==fgIs3DPhoto))
#endif

#if defined(CC_MT5389)
                //MT5389 68 Can not support LI Down
                ||((TDTV_TAG3D_LI==u4DrvTDTVGetTag3D(SV_VP_MAIN))
                    &&(540 < wDrvVideoInputHeight(SV_VP_MAIN)))
#endif                
                ||(SRM_IsDualVideo() &&(_u1tvmode_scart == 0)))            
            )                  
        {
            return SV_TRUE;
        }
        else
        {
            return SV_FALSE;
        }
    }    
    return SV_TRUE;
}


static inline E_TDTV_UI_3D_PACKING eDrvTDTVGetUIPacking(UINT32 u4Path)
{
    if(IS_Support3D()!=SV_TRUE)
    {  
        return E_TDTV_UI_3D_MODE_OFF;
    }

    if(fgDrvTDTVIsFix2DDisplay(u4Path))
    {
        return E_TDTV_UI_3D_MODE_AUTO;
    }
    else
    {
        return eUIPacking;
    }
}
    
static inline E_TDTV_UI_3D_2_2D eDrvTDTVGet3D22D(UINT32 u4Path)
{
    E_DEC_MOD eDecMod=eGetDecModule(u4Path);
    E_TDTV_UI_3D_2_2D e322Rgn = eUI3DTO2D;
        
    if(eDecFix322Region[eDecMod]==E_TDTV_UI_3D_2_2D_OFF)
    {
        if(fgDrvTDTVIsFix2DDisplay(u4Path))
        {
            e322Rgn=E_TDTV_UI_3D_2_2D_L;
        }
    }
    else if(eUI3DTO2D!=E_TDTV_UI_3D_2_2D_OFF)
    {
        e322Rgn=eDecFix322Region[eDecMod];
    }
    
    return e322Rgn;    
}

#ifdef IS_SUPPORT_3D_PHOTO            
static inline void vDrvTDTVSet3DPhoto(const TDTV_3D_CROP_INTO_T* prCropInfo, UINT32 bFix)
{
    static TDTV_3D_CROP_INTO_T ePreCrop={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    static UINT32 bPreFix=0xFF;
    
    if((ePreCrop.fgIsFixSrcAspectRatio!=prCropInfo->fgIsFixSrcAspectRatio)
        ||(ePreCrop.fgIsFullResolution!=prCropInfo->fgIsFullResolution)
        ||(ePreCrop.u4CropLeftOffset!=prCropInfo->u4CropLeftOffset)
        ||(ePreCrop.u4CropRightOffset!=prCropInfo->u4CropRightOffset)
        ||(ePreCrop.u4CropTopOffset!=prCropInfo->u4CropTopOffset)
        ||(ePreCrop.u4CropBottomOffset!=prCropInfo->u4CropBottomOffset)
        ||(bPreFix!=bFix))
        {
            Set3DPhotoRegion(prCropInfo,bFix);
            ePreCrop = *prCropInfo;
            bPreFix=bFix;            
        }          
 }
#endif        

static inline E_TDTV_DECODER_INPUT eTranslateUIFmtToDecInputFmt(E_TDTV_UI_3D_PACKING eUISelectPack,E_TDTV_UI_3D_2_2D eUI322,E_TDTV_DECODER_INPUT eDecInputFmt)
{
    E_TDTV_DECODER_INPUT eFmt;

    if(eUI322)
    {
        eUISelectPack = E_TDTV_UI_3D_MODE_OFF;
    }
    
    switch(eUISelectPack)
    {
        case E_TDTV_UI_3D_MODE_OFF:
            eFmt =  E_TDTV_DECODER_INPUT_2D_I;
            break;
        case E_TDTV_UI_3D_MODE_AUTO:
            if((E_TDTV_UI_NAVI_FULL_AUTO==eUITDNavi)
                    &&(E_TDTV_DECODER_INPUT_2D_P==(E_TDTV_DECODER_INPUT)((UINT32)eDecInputFmt|1))
                    &&(!fgDrvTDTVIsFix2DDisplay(SV_VP_MAIN))
                )
            {
                eFmt = eTDNAVIDetect;
            }
            else
            {
                eFmt = eDecInputFmt;
            }    
            break;            
        case E_TDTV_UI_3D_MODE_TTD:
            eFmt =   E_TDTV_DECODER_INPUT_TTD_I;
            break;            
        case E_TDTV_UI_3D_MODE_FS:
            eFmt =   E_TDTV_DECODER_INPUT_FS_I;
            break;            
        case E_TDTV_UI_3D_MODE_TB:
            eFmt =   E_TDTV_DECODER_INPUT_TB_I;
            break;            
        case E_TDTV_UI_3D_MODE_SBS:
            eFmt =   E_TDTV_DECODER_INPUT_SBS_I;
            break;            
        case E_TDTV_UI_3D_MODE_RD:
            eFmt =   E_TDTV_DECODER_INPUT_RD_I;
            break;            
        case E_TDTV_UI_3D_MODE_SS:
            eFmt =   E_TDTV_DECODER_INPUT_SS_I;
            break;            
        case E_TDTV_UI_3D_MODE_LI:
            eFmt =   E_TDTV_DECODER_INPUT_LI_I;
            break;            
        case E_TDTV_UI_3D_MODE_CB:
            eFmt =   E_TDTV_DECODER_INPUT_CB_I;
            break;            
        case E_TDTV_UI_3D_MODE_DA:
            eFmt =   E_TDTV_DECODER_INPUT_DA_I;            
            break;            
        default:
            eFmt =   E_TDTV_DECODER_INPUT_2D_I;
    }

    //Setting Interlace , Progessive
    return (E_TDTV_DECODER_INPUT)((UINT32)eFmt | ((UINT32)eDecInputFmt&0x1));
}

static inline E_TDTV_UI_3D_PACKING eTranslateDecInputFmtToUIFmt(E_TDTV_DECODER_INPUT eDecInputFmt)
{
    switch(eDecInputFmt|1)
    {
        case E_TDTV_DECODER_INPUT_AUTO:     
        case E_TDTV_DECODER_INPUT_RESERVE:
        case E_TDTV_DECODER_INPUT_2D_P:     
            return E_TDTV_UI_3D_MODE_OFF;
        case E_TDTV_DECODER_INPUT_TTD_P:
            return E_TDTV_UI_3D_MODE_TTD;
        case E_TDTV_DECODER_INPUT_FS_P:     
        case E_TDTV_DECODER_INPUT_FP_P:        
            return E_TDTV_UI_3D_MODE_FS;
        case E_TDTV_DECODER_INPUT_TB_P:
            return E_TDTV_UI_3D_MODE_TB;
        case E_TDTV_DECODER_INPUT_SBS_P:
            return E_TDTV_UI_3D_MODE_SBS;        
        case E_TDTV_DECODER_INPUT_RD_P: 
            return E_TDTV_UI_3D_MODE_RD;        
        case E_TDTV_DECODER_INPUT_SS_P:       
            return E_TDTV_UI_3D_MODE_SS;
        case E_TDTV_DECODER_INPUT_LI_P:
            return E_TDTV_UI_3D_MODE_LI;
        case E_TDTV_DECODER_INPUT_CB_P:
            return E_TDTV_UI_3D_MODE_CB;            
        case E_TDTV_DECODER_INPUT_DA_P:
            return E_TDTV_UI_3D_MODE_DA;
        default:        
            return E_TDTV_UI_3D_MODE_OFF;
    }
}

static inline E_TDTV_UI_3D_PACKING eDrvTDTVGetCurCfg(void)
{       
    E_DEC_MOD eDecMod=eGetDecModule(SV_VP_MAIN);
    return eTranslateDecInputFmtToUIFmt(eSelectPack[eDecMod]);  
}

static inline UINT32 u4DrvTDTVIsILRInv(void)
{
    if(SV_ST_MPEG==bGetSignalType(SV_VP_MAIN))
    {
        return (eUILRInverse==eB2RLRInv)?E_TDTV_MJC_LR_NORMAL : E_TDTV_MJC_LR_INVERSE;
    }
    else
    {
        return eUILRInverse;
    }   
}

/**
 * @brief :Interface for MW to know the Decoder Detected 3D Tag
 * @param bPath: SV_VP_MAIN / SV_VP_PIP
 */
TDTV_TAG3D_TYPE_T u4DrvTDTVGetTag3D(UINT8 bPath)
{
   
    E_TDTV_DECODER_INPUT eDetectedPak;

    //MM Resolution notified before Signal Stable
    if(bGetSignalType(bPath)!=SV_ST_MPEG)
    {      
        if(bDrvVideoSignalStatus(bPath)!=SV_VDO_STABLE)
        {
            return TDTV_TAG3D_NOT_SUPPORT;
        }
    }

    if(bIsScalerInput444(bPath))
    {
        return TDTV_TAG3D_NOT_SUPPORT;
    }
     
    if(bGetSignalType(bPath)==SV_ST_MPEG)
    {
        eDetectedPak=eDecDetectPack[E_DEC_MOD_MM];
    }
    else if(bGetSignalType(bPath)==SV_ST_DVI)
    {
        eDetectedPak=eDecDetectPack[E_DEC_MOD_HDMI];
    }
    else
    {
#if  SUPPORT_3D_ANALOG_SRC
        return TDTV_TAG3D_2D;//2D to 3D Only
#else
        return TDTV_TAG3D_TTDO;//2D to 3D Only
#endif
    }

    switch(eDetectedPak|0x1)
    {
        case E_TDTV_DECODER_INPUT_FS_P:            
            return TDTV_TAG3D_FS;
        case E_TDTV_DECODER_INPUT_FP_P:
            return TDTV_TAG3D_FP;
        case E_TDTV_DECODER_INPUT_TB_P:
            return TDTV_TAG3D_TB;
        case E_TDTV_DECODER_INPUT_SBS_P:
            return TDTV_TAG3D_SBS;
        case E_TDTV_DECODER_INPUT_RD_P:
            return TDTV_TAG3D_REALD;
        case E_TDTV_DECODER_INPUT_SS_P:     
            return TDTV_TAG3D_SENSIO;
        case E_TDTV_DECODER_INPUT_LI_P:     
            return TDTV_TAG3D_LI;
        case E_TDTV_DECODER_INPUT_CB_P:     
            return TDTV_TAG3D_CB;
        case E_TDTV_DECODER_INPUT_DA_P:     
            return TDTV_TAG3D_DA;                            
        case E_TDTV_DECODER_INPUT_AUTO:
        case E_TDTV_DECODER_INPUT_RESERVE: 
        case E_TDTV_DECODER_INPUT_2D_P:     
        case E_TDTV_DECODER_INPUT_TTD_P:            
        default:
            return TDTV_TAG3D_2D;
      }             
}


TDTV_TAG3D_TYPE_T u4DrvTDTVGetNaviTag(void)
{   

    TDTV_TAG3D_TYPE_T eTag=u4DrvTDTVGetTag3D(SV_VP_MAIN);
    if(TDTV_TAG3D_2D!=eTag)
    {
        return eTag;
    }

    switch(eTDNAVIDetect|0x1)
    {
        case E_TDTV_DECODER_INPUT_FS_P:
            return TDTV_TAG3D_FS;
        case E_TDTV_DECODER_INPUT_FP_P:
            return TDTV_TAG3D_FP;
        case E_TDTV_DECODER_INPUT_TB_P:
            return TDTV_TAG3D_TB;
        case E_TDTV_DECODER_INPUT_SBS_P:
            return TDTV_TAG3D_SBS;
        case E_TDTV_DECODER_INPUT_RD_P:
            return TDTV_TAG3D_REALD;
        case E_TDTV_DECODER_INPUT_SS_P:     
            return TDTV_TAG3D_SENSIO;
        case E_TDTV_DECODER_INPUT_LI_P:     
            return TDTV_TAG3D_LI;
        case E_TDTV_DECODER_INPUT_CB_P:     
            return TDTV_TAG3D_CB;
        case E_TDTV_DECODER_INPUT_DA_P:     
            return TDTV_TAG3D_DA;                            
        case E_TDTV_DECODER_INPUT_AUTO:
        case E_TDTV_DECODER_INPUT_RESERVE: 
        case E_TDTV_DECODER_INPUT_2D_P:     
        case E_TDTV_DECODER_INPUT_TTD_P:            
        default:
            return TDTV_TAG3D_2D;
      }             
}

TDTV_TAG3D_TYPE_T u4DrvTDTVGetDecTag3D(E_DEC_MOD eDec)
{

    E_TDTV_DECODER_INPUT eDetectedPak;

    if(E_DEC_MOD_MM==eDec)
    {
     eDetectedPak=eDecDetectPack[E_DEC_MOD_MM];
    }
    else if(E_DEC_MOD_HDMI==eDec)
    {
     eDetectedPak=eDecDetectPack[E_DEC_MOD_MM];
    }
    else
    {
#if  SUPPORT_3D_ANALOG_SRC
     return TDTV_TAG3D_2D;//2D to 3D Only
#else
     return TDTV_TAG3D_TTDO;//2D to 3D Only
#endif
    }

    switch(eDetectedPak)
    {
     case E_TDTV_DECODER_INPUT_AUTO:
     case E_TDTV_DECODER_INPUT_RESERVE:
     case E_TDTV_DECODER_INPUT_2D_I:
     case E_TDTV_DECODER_INPUT_2D_P:
     case E_TDTV_DECODER_INPUT_TTD_I:
     case E_TDTV_DECODER_INPUT_TTD_P:
         return TDTV_TAG3D_2D;
     case E_TDTV_DECODER_INPUT_FS_I:
     case E_TDTV_DECODER_INPUT_FS_P:
         return TDTV_TAG3D_FS;
     case E_TDTV_DECODER_INPUT_FP_I:
     case E_TDTV_DECODER_INPUT_FP_P:
         return TDTV_TAG3D_FP;
     case E_TDTV_DECODER_INPUT_TB_I:
     case E_TDTV_DECODER_INPUT_TB_P:
         return TDTV_TAG3D_TB;
     case E_TDTV_DECODER_INPUT_SBS_I:
     case E_TDTV_DECODER_INPUT_SBS_P:
         return TDTV_TAG3D_SBS;
     case E_TDTV_DECODER_INPUT_RD_I:
     case E_TDTV_DECODER_INPUT_RD_P:
         return TDTV_TAG3D_REALD;
     case E_TDTV_DECODER_INPUT_SS_I:
     case E_TDTV_DECODER_INPUT_SS_P:
         return TDTV_TAG3D_SENSIO;
     default:
         return TDTV_TAG3D_2D;
    }
}

//=============================================================================
// TDTV Packing Configuration
//=============================================================================

static inline void vDrvTDTVTDNAVIProc(void)
{
#if SUPPORT_3D_NAVI
   UINT32 u4TDNAVIFmt = vDrvTdNaviGetCurrDetType();
   E_TDTV_DECODER_INPUT eFmt;
   
   E_TDTV_DECODER_INPUT ePreTDNAVIDetect=eTDNAVIDetect;
    if(E_TDTV_UI_NAVI_MANAUL==eUITDNavi)
    {
        eFmt = E_TDTV_DECODER_INPUT_2D_I;
    }
    else
    {
        switch(u4TDNAVIFmt)
        {
            case TD_NAVI_DETECT_TAB:
                eFmt =   E_TDTV_DECODER_INPUT_TB_I;
                break;            
            case TD_NAVI_DETECT_SBS:
                eFmt =   E_TDTV_DECODER_INPUT_SBS_I;
                break; 
            case TD_NAVI_DETECT_CB:
                eFmt =   E_TDTV_DECODER_INPUT_CB_I;
                break;
            case TD_NAVI_DETECT_LI:
                eFmt =   E_TDTV_DECODER_INPUT_LI_I;
                break; 
            case TD_NAVI_DETECT_CI:
                eFmt =   E_TDTV_DECODER_INPUT_DA_I;
                break;                 
            default:
                eFmt =   E_TDTV_DECODER_INPUT_2D_I;
        }
    }
#else
    E_TDTV_DECODER_INPUT eFmt = E_TDTV_DECODER_INPUT_2D_I;
    E_TDTV_DECODER_INPUT ePreTDNAVIDetect=eTDNAVIDetect;
#endif

    eTDNAVIDetect = (E_TDTV_DECODER_INPUT)((UINT32)eFmt | (!bDrvVideoIsSrcInterlace(SV_VP_MAIN)));

    if(ePreTDNAVIDetect!=eTDNAVIDetect)
    {
        LOCK_TDTV_STATE();               
        fgTDTVModeChgTrigger=SV_ON;
#ifdef CC_SUPPORT_TDNAVI_CB        
        VDP_TDNaviStatusNotify(SV_VP_MAIN,u4DrvTDTVGetNaviTag());
#endif        
        UNLOCK_TDTV_STATE();
    }
}

static inline void vDrvTDTVDecConfig(E_DEC_MOD eMode)
{
    E_TDTV_UI_3D_PACKING eValidUIPacking;

    if(E_DEC_MOD_OTHERS==eMode)
    {
        eDecDetectPack[eMode]=(E_TDTV_DECODER_INPUT)((UINT32) E_TDTV_DECODER_INPUT_2D_I | ((UINT32)!bDrvVideoIsSrcInterlace(SV_VP_MAIN)));
    }
    else if(E_DEC_MOD_MM==eMode)
    {   //Auto Correction 3D Tag if Input Decoder didn't send 3D Tag correctly due to cfg
        eDecDetectPack[eMode]=(E_TDTV_DECODER_INPUT)(((UINT32) eDecDetectPack[eMode] & (~0x1)) | (u1MpegHdInterlace()!=SV_TRUE));
    }
    
    eValidUIPacking =  vDrvTDTVValidatePackMode(eUIPacking,eDecDetectPack[eMode]);

    eSelectPack[eMode] = eTranslateUIFmtToDecInputFmt(eValidUIPacking,
        eDrvTDTVGet3D22D(SV_VP_MAIN)
        ,eDecDetectPack[eMode]);

    //Deocder Always Use Detected Tag for output instead of UI Setting in 5396/68
    sVdoCfg[SV_VP_MAIN].DEC[eMode]=TD_2ENUM_TO_INT(
        TD_DEC_OUT(TD_TBL_UNPACK_DEC(eDecDetectPack[(eMode)])),
        TD_DEC_IN(TD_TBL_UNPACK_DEC(eDecDetectPack[(eMode)]))
        );  

    if((eValidUIPacking==E_TDTV_UI_3D_MODE_FS)&&(eDecDetectPack[E_DEC_MOD_HDMI]<NON_2D_PACKING_START))
    {
        u1LRFreeRun[eMode]=SV_ON;
    }
    else
    {
        u1LRFreeRun[eMode]=SV_OFF;       
    }   
}

static inline void vDrvTDTVTTTConfig(UINT32 bPath)
{   
    E_DEC_MOD eDecMod=eGetDecModule(bPath);
    E_TDTV_UI_3D_2_2D eUI322 = eDrvTDTVGet3D22D(bPath);

    E_TD_IN eInputPack=E_TD_IN_2D_I;
    E_TD_OUT eOutputFmt;
    
    if(eUI322)
    {   
        switch(eTranslateDecInputFmtToUIFmt(eTranslateUIFmtToDecInputFmt(
            eDrvTDTVGetUIPacking(bPath),
            E_TDTV_UI_3D_2_2D_OFF,eDecDetectPack[eDecMod])))
        {        
            case E_TDTV_UI_3D_MODE_FS:
                eInputPack=E_TD_IN_FS_I;
                break;
            case E_TDTV_UI_3D_MODE_SBS:
            case E_TDTV_UI_3D_MODE_SS:
            case E_TDTV_UI_3D_MODE_RD:
                eInputPack=E_TD_IN_SBS_I;
                break;            
            case E_TDTV_UI_3D_MODE_TB:
                eInputPack=E_TD_IN_TB_I;
                break;
            //LI CB DA use Scaler to do 3D->2D because of FW NAVI
#if !defined(CC_MT5389)                            
            case E_TDTV_UI_3D_MODE_LI:            
#endif                
            case E_TDTV_UI_3D_MODE_CB:
            case E_TDTV_UI_3D_MODE_DA:
            default:
                eInputPack=E_TD_IN_2D_I;
        }

        eOutputFmt = E_TD_IN_2D_I == eInputPack? E_TD_OUT_NATIVE : E_TD_OUT_3D_2_2D;        
        eInputPack = (E_TD_IN)((UINT32)eInputPack | ((UINT32)eSelectPack[(eDecMod)]&0x1));
    }
    else
    {
        eInputPack = (E_TD_IN)((UINT32)eInputPack | ((UINT32)eSelectPack[(eDecMod)]&0x1));
        eOutputFmt = E_TD_OUT_NATIVE;        
    }

    sVdoCfg[bPath].TTT=(UINT32)TTT(eInputPack,eOutputFmt);                  
}

static inline void vDrvTDTVDIConfig(void)
{   
    E_DEC_MOD eDecMod=eGetDecModule(SV_VP_MAIN);
    sVdoCfg[SV_VP_MAIN].DI=(UINT32)TD_TBL_UNPACK_MIB(eSelectPack[(eDecMod)]);
}

static inline void vDrvTDTVSCConfig(UINT32 u4Path)
{   
    E_DEC_MOD eDecMod=eGetDecModule(u4Path);
    E_TD_IN eIn;
    E_TD_OUT eOut;    
    E_TDTV_RES_OUTPUT ePreH,ePreV;
    E_TDTV_UI_3D_2_2D eUI322 = eDrvTDTVGet3D22D(u4Path);
    
    if(eUI322)
    {
        eIn=E_TD_IN_2D_P;
        eOut=E_TD_OUT_NATIVE; 
        
        switch(eTranslateDecInputFmtToUIFmt(eTranslateUIFmtToDecInputFmt(
            eDrvTDTVGetUIPacking(u4Path),
            E_TDTV_UI_3D_2_2D_OFF,eDecDetectPack[eDecMod])))
        {        
            case E_TDTV_UI_3D_MODE_FS:
                ePreH=E_TD_RES_NORMAL;
                ePreV=E_TD_RES_NORMAL;
                break;
            case E_TDTV_UI_3D_MODE_SBS:
            case E_TDTV_UI_3D_MODE_SS:
            case E_TDTV_UI_3D_MODE_RD:
                ePreH=E_TD_RES_HALF;
                ePreV=E_TD_RES_NORMAL;
                break;               
            case E_TDTV_UI_3D_MODE_TB:
                ePreH=E_TD_RES_NORMAL;
                ePreV=E_TD_RES_HALF;
                break;
            //Below fmt do 3D to 2D in scaler side for FW 3D Navi
            case E_TDTV_UI_3D_MODE_LI:
#if defined(CC_MT5389)                                
                ePreH=E_TD_RES_NORMAL;
                ePreV=E_TD_RES_NORMAL;
                eIn =E_TD_IN_LI_P;
                eOut=E_TD_OUT_3D_2_2D;                                
#else             
                ePreH=E_TD_RES_DOUBLE;
                ePreV=E_TD_RES_HALF;
                eIn =E_TD_IN_SBS_P;
                eOut=E_TD_OUT_3D_2_2D;                   
#endif                
                break;            
            case E_TDTV_UI_3D_MODE_CB:
            case E_TDTV_UI_3D_MODE_DA:
                ePreH=E_TD_RES_NORMAL;
                ePreV=E_TD_RES_NORMAL;
                eIn=E_TD_IN_SBS_P;
                eOut=E_TD_OUT_3D_2_2D;
                break;                
            default:
                ePreH=E_TD_RES_NORMAL;
                ePreV=E_TD_RES_NORMAL;
        }
        sVdoCfg[u4Path].SC=SCL(ePreH,ePreV,eIn,eOut,E_TD_RES_NORMAL,E_TD_RES_NORMAL);        
    }
    else
    {
        sVdoCfg[u4Path].SC=TD_TBL_UNPACK_SCL(eSelectPack[(eDecMod)]);
    }
}

static inline void vDrvTDTVMJCConfig(void)
{   
    E_DEC_MOD eDecMod=eGetDecModule(SV_VP_MAIN);
    sVdoCfg[SV_VP_MAIN].MJC=  (UINT32)TD_TBL_UNPACK_MJC(eSelectPack[(eDecMod)]);
}

static inline void vDrvTDTVTTDConfig(void)
{   
    E_DEC_MOD eDecMod=eGetDecModule(SV_VP_MAIN);
    sVdoCfg[SV_VP_MAIN].TTD=  (UINT32)TD_TBL_UNPACK_TTD(eSelectPack[(eDecMod)]);
}

static inline void vDrvTDTVDQCConfig(void)
{   
    E_DEC_MOD eDecMod=eGetDecModule(SV_VP_MAIN);
    E_TD_IN eIn;
    E_TD_OUT eOut;
    E_TDTV_UI_3D_2_2D eUI322 = eDrvTDTVGet3D22D(SV_VP_MAIN);

    if(eUI322)
    {            
        switch(eTranslateDecInputFmtToUIFmt(eTranslateUIFmtToDecInputFmt(
            eDrvTDTVGetUIPacking(SV_VP_MAIN),
            E_TDTV_UI_3D_2_2D_OFF,eDecDetectPack[eDecMod])))
        {        
            case E_TDTV_UI_3D_MODE_CB:
                eIn=E_TD_IN_CB_I;
                eOut=E_TD_OUT_3D_SBS;
                break;                   
            case E_TDTV_UI_3D_MODE_DA:
                eIn=E_TD_IN_DA_I;
                eOut=E_TD_OUT_3D_SBS;
                break;
#if !defined(CC_MT5389)                                            
            case E_TDTV_UI_3D_MODE_LI:            
                eIn=E_TD_IN_LI_I;
                eOut=E_TD_OUT_3D_SBS;
                break;
#endif                
            default:
                eIn=E_TD_IN_2D_I;
                eOut=E_TD_OUT_NATIVE;
        }
        eIn = (E_TD_IN)((UINT32) eIn | ((UINT32)eDecDetectPack[eDecMod] & 0x1));
        sVdoCfg[SV_VP_MAIN].DQC = DQC(eIn,eOut);        
    }
    else
    {
        sVdoCfg[SV_VP_MAIN].DQC = (UINT32)TD_TBL_UNPACK_DQC(eSelectPack[(eDecMod)]);
    }
}

static inline void vDrvTDTVOSDConfig(void)
{
#ifdef DRV_USE_EXTERNAL_3D_FRC

    S_TDTV_OSTG_CONFIG sOstgCfg=eDrvTDTVOSTGModeQuery();
    E_TDTV_UI_3D_PACKING eTDTVMode = eDrvTDTVGetCurCfg();   
    
    UINT32 u4Width = sOstgCfg.u4Width;
    UINT32 u4Height = sOstgCfg.u4Height;

    switch(eTDTVMode)
    {
        case E_TDTV_UI_3D_MODE_FS:
        case E_TDTV_UI_3D_MODE_TTD:             
        case E_TDTV_UI_3D_MODE_LI:
            sVdoCfg[SV_VP_MAIN].OSD.eOSDType=E_TDTV_OSD_FS;
            sVdoCfg[SV_VP_MAIN].OSD.u4PanelWidth=u4Width;
            sVdoCfg[SV_VP_MAIN].OSD.u4PanelHeight=u4Height;  
            return;
            
        case E_TDTV_UI_3D_MODE_SS:
        case E_TDTV_UI_3D_MODE_SBS:
        case E_TDTV_UI_3D_MODE_RD:
            sVdoCfg[SV_VP_MAIN].OSD.eOSDType=E_TDTV_OSD_SBS;
            sVdoCfg[SV_VP_MAIN].OSD.u4PanelWidth=u4Width;
            sVdoCfg[SV_VP_MAIN].OSD.u4PanelHeight=u4Height;  
        
        case E_TDTV_UI_3D_MODE_TB:
            sVdoCfg[SV_VP_MAIN].OSD.eOSDType=E_TDTV_OSD_TB;
            sVdoCfg[SV_VP_MAIN].OSD.u4PanelWidth=u4Width;
            sVdoCfg[SV_VP_MAIN].OSD.u4PanelHeight=u4Height;    
            
        case E_TDTV_UI_3D_MODE_OFF:                 
        case E_TDTV_UI_3D_MODE_AUTO:
        default:
            sVdoCfg[SV_VP_MAIN].OSD.eOSDType=E_TDTV_OSD_2D;
            sVdoCfg[SV_VP_MAIN].OSD.u4PanelWidth=u4Width;
            sVdoCfg[SV_VP_MAIN].OSD.u4PanelHeight=u4Height;                    
    }    
#else

#if SUPPORT_OSD_DEPTH
    static UINT8 u1PreDepth=0xFF;
    static UINT8 u1PreOnOff=0xFF;
    UINT8 u1OnOff;
    UINT8 u1Depth;
#endif

    UINT32 u4Width=PANEL_GetPanelWidth();
    UINT32 u4Height=PANEL_GetPanelHeight();

    sVdoCfg[SV_VP_MAIN].OSD.eOSDType=E_TDTV_OSD_2D;
    sVdoCfg[SV_VP_MAIN].OSD.u4PanelWidth=u4Width;
    sVdoCfg[SV_VP_MAIN].OSD.u4PanelHeight=u4Height;

#ifndef CC_MEM_TOOL
#if SUPPORT_OSD_DEPTH
    
    //u1Depth = u1OSDDepth -16;
    if(u1OSDDepth > 15)
   	{
		u1Depth = 0x80 + (u1OSDDepth- 16 )*2;
	}
	else 
	{
		u1Depth = (16 - u1OSDDepth)*2;
	}
	
    if((E_TDTV_UI_3D_MODE_OFF==u4DrvTDTV3DModeQuery())|| (0x80==u1Depth))
    {
        u1OnOff = SV_OFF;
    }
    else
    {
        u1OnOff = SV_ON;
    }

    if((u1PreDepth!=u1Depth)||(u1PreOnOff!=u1OnOff))
    {
        u1PreDepth = u1Depth;
        u1PreOnOff = u1OnOff;        
        if(IS_LVDS_DISP_3D_POLARIZED)
        {
            OSD_BASE_Set3DPrLineShift(u1OnOff, u1Depth);
        }
        else
        {
            OSD_BASE_Set3DShutterLineShift(u1OnOff, u1Depth);
        }
    }
#endif
#endif

#endif

}

/*
* @brief :Mode Query for OSTG
* @retval:  S_TDTV_OSD_CONFIG
*/

static inline void vDrvTDTVOSTGConfig(void)
{
#ifdef DRV_USE_EXTERNAL_3D_FRC       
    E_TDTV_UI_3D_PACKING eTDTVMode = eDrvTDTVGetCurCfg();   
    UINT32 u4FrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
    UINT32 u4Width = wDrvVideoInputWidth(SV_VP_MAIN);
    UINT32 u4Height = wDrvVideoInputHeight(SV_VP_MAIN);
    UINT32 u4IsInterlace = bDrvVideoIsSrcInterlace(SV_VP_MAIN);
    UINT32 u4VTotal;
    UINT32 u4Clk;
    
	// In: E_TD_IN_FS_P	Out: E_TD_OUT_3D_2_2D
	UINT32 u4TTT;
    E_TD_IN eTTDIn;
    E_TD_OUT eTTDOut;
	
    u4TTT = u4DrvTDTVTTTModeQuery(SV_VP_MAIN);
    eTTDIn  = TD_TTT_IN(u4TTT);
    eTTDOut = TD_TTT_OUT(u4TTT);

    if (((E_TD_IN_FS_I == eTTDIn) || (E_TD_IN_FS_P == eTTDIn)) && (E_TD_OUT_3D_2_2D == eTTDOut))
    {
    	u4FrameRate = u4FrameRate >> 1;
    }
	
    if((u4FrameRate==0)
    	||(bDrvVideoSignalStatus(SV_VP_MAIN)!=SV_VDO_STABLE)
	||(SRM_MM_MODE_TYPE_THUMBNAIL==SRM_GetMmMode())
	    )
    {
    	return;
    }
    
    switch(eTDTVMode)
    {
        case E_TDTV_UI_3D_MODE_FS:				
            u4VTotal = (u4FrameRate <= 60) ? 1125 : ((u4IsInterlace) ? 1125 :  750);
            u4Height = (u4FrameRate <= 60) ? 1080 : ((u4IsInterlace) ? 1080 :  720);
            u4Width  = (u4FrameRate <= 60) ? 1920 : ((u4IsInterlace) ?  960 : 1280);
            u4Clk    = PANEL_CLK_H;
            break;  
        case E_TDTV_UI_3D_MODE_SS:  
        case E_TDTV_UI_3D_MODE_LI: 
        case E_TDTV_UI_3D_MODE_SBS:
        case E_TDTV_UI_3D_MODE_TB:   
        case E_TDTV_UI_3D_MODE_RD:
        case E_TDTV_UI_3D_MODE_OFF:                 
        case E_TDTV_UI_3D_MODE_TTD: 
        case E_TDTV_UI_3D_MODE_AUTO:
        default:

        if( bIsScalerInput444(SV_VP_MAIN)==SV_TRUE) // PC timing
        {
        		if(u4FrameRate>=60)
        			u4FrameRate=60;
        }
        u4VTotal = 1125;
        u4Height = 1080;
        u4Width = 1920;
        u4Clk = (u4FrameRate <= 30) ? PANEL_CLK_L : PANEL_CLK_H;
        break;  
    }
    
    sVdoCfg[SV_VP_MAIN].OSTG.eOSTGCfg=E_TDTV_OSTG_CFG_BY_TDTV;
    sVdoCfg[SV_VP_MAIN].OSTG.u4Clk   =u4Clk;
    sVdoCfg[SV_VP_MAIN].OSTG.u4Width =u4Width;
    sVdoCfg[SV_VP_MAIN].OSTG.u4Height=u4Height;
    sVdoCfg[SV_VP_MAIN].OSTG.u4VTotal=u4VTotal;  // for panel table              
    sVdoCfg[SV_VP_MAIN].OSTG.u4HTotal=(u4Clk/u4FrameRate/u4VTotal); // for panel table

    //Current FRC only Support Even HTotal
    sVdoCfg[SV_VP_MAIN].OSTG.u4HTotal &= ~0x1;       
    sVdoCfg[SV_VP_MAIN].OSTG.u4FrameRate=u4FrameRate;                
     
#else    

    UINT32 u4Width=PANEL_GetPanelWidth();
    UINT32 u4Height=PANEL_GetPanelHeight();

    sVdoCfg[SV_VP_MAIN].OSTG.eOSTGCfg=E_TDTV_OSTG_CFG_BY_PANEL_TBL;        
    sVdoCfg[SV_VP_MAIN].OSTG.u4Width=u4Width;
    sVdoCfg[SV_VP_MAIN].OSTG.u4Height=u4Height;         
#endif
}
//=============================================================================
// TDTV Handling Mode Query Interface
//=============================================================================
UINT32 u4DrvTDTVIsDecoderContentFull(E_DEC_MOD eDec)
{
    UINT32 u4Tag;
    if(E_DEC_MOD_MM==eDec)
    {
        u4Tag=u4DrvTDTVGetDecTag3D(eDec);
        if((TDTV_TAG3D_TB==u4Tag)
            ||(TDTV_TAG3D_SBS==u4Tag))
        {
            return bISB2RContentFulled;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

static inline UINT32 u4DrvTDTVDecoderModeQuery(E_DEC_MOD eDec,E_TDTV_DECODER_INPUT eDetectedMode)
{
    E_TDTV_DECODER_INPUT ePreDetectedMode;

    //Translate B2R Inverse
    if(E_DEC_MOD_MM==eDec)
    {
        eB2RLRInv=IS_CONTENT_INVERSED(eDetectedMode) ? E_TDTV_UI_LR_INVERSE : E_TDTV_UI_LR_NORMAL;    
        bISB2RContentFulled = IS_CONTENT_FULLED(eDetectedMode);
    }
    eDetectedMode = MSKED_DECODER_INPUT(eDetectedMode);

    //Error Correction of invalid Auto Type
    if(E_TDTV_DECODER_INPUT_AUTO==eDetectedMode)
    {
        if(bDrvVideoIsSrcInterlace(SV_VP_MAIN))
        {
            eDetectedMode=E_TDTV_DECODER_INPUT_2D_I;
        }
        else
        {
            eDetectedMode=E_TDTV_DECODER_INPUT_2D_P;
        }
    }

    ePreDetectedMode = eDecDetectPack[eDec];    
    eDecDetectPack[eDec] = eDetectedMode;

    //Force Auto mode when detected format change
    if((ePreDetectedMode &(~0x1))!= (eDetectedMode&(~0x1)))
    {
        if((SV_ST_DVI==bGetSignalType(SV_VP_MAIN))        
            &&(IS_Support3D()==SV_TRUE))
        {
            eUIPacking = E_TDTV_UI_3D_MODE_AUTO;
        }
    }

    //Force Auto mode when detected format change
    if(ePreDetectedMode != eDetectedMode)
    {    
        vDrvTDTVDecConfig(eDec);        
    }

    return sVdoCfg[SV_VP_MAIN].DEC[eDec];  
}

/**
 * @brief :3D Processing Mode Query
 * @param eDetectedMode: Set The Decoder detected Mode According to HDMI Flag
 * @retval:  (E_TDTV_HDMI_OUTPUT << 16) || E_TDTV_HDMI_INPUT
 */
UINT32 u4DrvTDTVHDMIModeQuery(E_TDTV_DECODER_INPUT eDetectedMode)
{
    UINT32 u4Cfg;

    LOCK_TDTV_STATE();
    
    fgTDTVModeChgTrigger=SV_ON;    
    u4Cfg=u4DrvTDTVDecoderModeQuery(E_DEC_MOD_HDMI,eDetectedMode);    
    UNLOCK_TDTV_STATE();
    
    return u4Cfg;
}

/**
 * @brief :3D Processing Mode Query
 * @param eDetectedMode: Set the Decoder Detected mode
 * @retval:  (E_TDTV_MM_OUTPUT << 16) || E_TDTV_MM_INPUT
 */
 
UINT32 u4DrvTDTVMMModeQuery(E_TDTV_DECODER_INPUT eDetectedMode)
{
    UINT32 u4Cfg; 
    LOCK_TDTV_STATE();

    fgTDTVModeChgTrigger=SV_ON;    
    u4Cfg = u4DrvTDTVDecoderModeQuery(E_DEC_MOD_MM,eDetectedMode);
    
    UNLOCK_TDTV_STATE();

#ifdef IS_SUPPORT_3D_PHOTO
    if((NULL!=TDTV3DCropCallback[E_TDTV_CB_3D_CROP_MM]))
    {
        TDTV3DCropCallback[E_TDTV_CB_3D_CROP_MM](&eCropInfo);
    }
    vDrvTDTVCropProc();
#endif
    
    return u4Cfg;
}

void vDrvTDTVSetMMFix322Region(E_TDTV_UI_3D_2_2D eRgn)
{
    LOCK_TDTV_STATE();

    vDrvTDTVSetDecFix322Region(E_DEC_MOD_MM,eRgn);
    fgTDTVModeChgTrigger=SV_ON;
    
    UNLOCK_TDTV_STATE();
}


UINT32 u4DrvTDTVDIModeQuery()
{
    vDrvTDTVDIConfig();
    return sVdoCfg[SV_VP_MAIN].DI;
}

/**
 * @brief :3D Processing Mode Query Extension for Scaler 
 * @param u4Path: Video Path Src
 * @param eDetectedMode: For HDMI Input Source, Scaler Detect RealD Tag, 
 * @retval:  (E_TDTV_MM_OUTPUT << 16) || E_TDTV_MM_INPUT
 */
UINT32 u4DrvTDTVScalerModeQueryEx(UINT32 u4Path)
{
    PATH_CHECK(u4Path);   
    vDrvTDTVSCConfig(u4Path); 

    vDrvTDTVTTTConfig(u4Path);
    vDrvTDTVUpdate3D22(u4Path);    
    return sVdoCfg[u4Path].SC;        
}

/**
 * @brief :3D Processing Mode Query for Scaler
 * @param eDetectedMode: For HDMI Input Source, Scaler Detect RealD Tag, 
 * @retval:  (E_TDTV_MM_OUTPUT << 16) || E_TDTV_MM_INPUT
 */
UINT32 u4DrvTDTVScalerModeQuery()
{
    return u4DrvTDTVScalerModeQueryEx(SV_VP_MAIN);
}

/*
* @brief :3D Processing Mode Query for MJC
* @retval:  (E_TDTV_MJC_LR << 16) || E_TDTV_MJC
*/
UINT32 u4DrvTDTVMJCModeQuery()
{
    vDrvTDTVMJCConfig();
    return sVdoCfg[SV_VP_MAIN].MJC;
}

/*
* @brief :MJC Enable on or off  Query for MJC
* @retval: E_TDTV_MJC_2D
*/
UINT8 u1DrvTDTVMJCEnableQuery(void)
{
    return SV_TRUE;
}


/*
* @brief : TTD Processing Mode Query
* @retval: E_TDTV_MJC_2D
*/
UINT32 u4DrvTDTVTTDModeQuery(void)
{
    vDrvTDTVTTDConfig();
    return sVdoCfg[SV_VP_MAIN].TTD;
}


/*
* @brief : De-QuenCunx Processing Mode Query
* @retval: E_TDTV_MJC_2D
*/
UINT32 u4DrvTDTVDQCModeQuery(void)
{
    vDrvTDTVDQCConfig();
    return sVdoCfg[SV_VP_MAIN].DQC;
}

/*
* @brief : 3D to 2D  Mode Query
* @retval: E_TDTV_MJC_2D
*/
UINT32 u4DrvTDTVTTTModeQuery(UINT32 u4Path)
{
    PATH_CHECK(u4Path);
    vDrvTDTVTTTConfig(u4Path);
    return sVdoCfg[u4Path].TTT;
}

/*
* @brief :Mode Query for OSD
* @retval:  S_TDTV_OSD_CONFIG
*/

S_TDTV_OSD_CONFIG eDrvTDTVOSDModeQuery(void)
{
    vDrvTDTVOSDConfig();
    return sVdoCfg[SV_VP_MAIN].OSD;
}

/*
* @brief :Mode Query for OSTG
* @retval:  S_TDTV_OSD_CONFIG
*/
S_TDTV_OSTG_CONFIG eDrvTDTVOSTGModeQuery(void)
{
    vDrvTDTVOSTGConfig();
    return sVdoCfg[SV_VP_MAIN].OSTG;
}

/**
 * @brief : UI Mode Query for General Model
 * @retval:  E_TDTV_UI_3D_PACKING
 */
E_TDTV_UI_3D_PACKING u4DrvTDTV3DModeQuery(void)
{
    return eDrvTDTVGetCurCfg();
}

UINT32 u4DrvTDTV3DFMTCapQuery(UINT32 bPath)
{
    UINT32 u4Cap=0;    
    UINT32 u43DTag=u4DrvTDTVGetTag3D(SV_VP_MAIN);

    //3D UI Enable
    if((SV_VP_MAIN==bPath)&&(
            (bDrvVideoSignalStatus(SV_VP_MAIN)!=SV_VDO_STABLE)
            ||(TDTV_TAG3D_NOT_SUPPORT==u43DTag)
            ||(bIsScalerInput444(SV_VP_MAIN))
            )) 
    {
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_OFF*2);    
        return u4Cap;
    }

    if(SV_VP_PIP==bPath)
    {
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_AUTO*2);
        return u4Cap;
    }

    if((SV_TRUE==fgIs3DPhoto)&&(TDTV_TAG3D_2D==u43DTag))
    {
        //3D Photo only support below 3D mode
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_OFF*2);
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_TTD*2);
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_SBS*2);
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_TB*2);
        return u4Cap;
    }
   
    if((E_TDTV_UI_NAVI_FULL_AUTO!=eUITDNavi)
        &&((TDTV_TAG3D_2D==u43DTag)
           ||(TDTV_TAG3D_TTDO==u43DTag))
        )
    {  
        if(E_TDTV_UI_NAVI_SEMI_AUTO==eUITDNavi)
        {
            switch(eTDNAVIDetect)
            {
                case E_TDTV_DECODER_INPUT_TB_I :
                case E_TDTV_DECODER_INPUT_TB_P :
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_OFF*2);                    
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_TB*2);
                    break;
                case E_TDTV_DECODER_INPUT_SBS_I:
                case E_TDTV_DECODER_INPUT_SBS_P:
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_OFF*2);                    
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_SBS*2);
                    break;                    
                case E_TDTV_DECODER_INPUT_RD_I :
                case E_TDTV_DECODER_INPUT_RD_P :
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_OFF*2);                    
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_RD *2);
                    break;                    
                case E_TDTV_DECODER_INPUT_CB_I :
                case E_TDTV_DECODER_INPUT_CB_P :
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_OFF*2);                    
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_CB*2);
                    break;
                case E_TDTV_DECODER_INPUT_LI_I :
                case E_TDTV_DECODER_INPUT_LI_P :
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_OFF*2);                    
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_LI*2);
                    break;
                case E_TDTV_DECODER_INPUT_DA_I :
                case E_TDTV_DECODER_INPUT_DA_P :
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_OFF*2);                    
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_DA*2);
                    break;                    
                default:                    
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_OFF*2);
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_TTD*2);                    
            } 
        }
        else
        {
            u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_OFF*2);
            u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_TTD*2);
            u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_SBS*2);
            u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_TB*2);

            if(SV_VD_TVD3D!=bGetVideoDecType(bPath))
            {
                u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_SS*2);
            }
            //3D Navi
            //E_TDTV_UI_3D_MODE_AUTO
            //E_TDTV_UI_3D_MODE_RD  

            if((!bDrvVideoIsSrcInterlace(SV_VP_MAIN)))
            {
#if SUPPORT_FS_FREERUN        
                u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_FS*2);
#endif        

#ifdef CC_3D_SUPPORT_LI_DA        
                u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_CB*2);

                if(IS_ECO_IC())
                {
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_LI*2);            
                    u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_DA*2);
                }
#endif                
            }
        }
    }
    else if(TDTV_TAG3D_NOT_SUPPORT!=u4DrvTDTVGetTag3D(SV_VP_MAIN))
    {
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_3D_MODE_AUTO*2);
    }
    
    return u4Cap;    
}

UINT32 u4DrvTDTV3DCtrlCapQuery(UINT32 bPath)
{
    UINT32 u4Cap=0;
    UINT32 u43DTag=u4DrvTDTVGetTag3D(SV_VP_MAIN);
    //Set to all grey out    
    u4Cap |= (UINT32)E_TDTV_UI_GREYOUT<<((UINT8)E_TDTV_UI_CTRL_3D_MODE*2);
    u4Cap |= (UINT32)E_TDTV_UI_GREYOUT<<((UINT8)E_TDTV_UI_CTRL_LR_INVERSE*2);
    u4Cap |= (UINT32)E_TDTV_UI_GREYOUT<<((UINT8)E_TDTV_UI_CTRL_DEPTH_CTRL*2);            
    u4Cap |= (UINT32)E_TDTV_UI_GREYOUT<<((UINT8)E_TDTV_UI_CTRL_PROTRUDE*2);
    u4Cap |= (UINT32)E_TDTV_UI_GREYOUT<<((UINT8)E_TDTV_UI_CTRL_DISTANCE_TV*2);
    u4Cap |= (UINT32)E_TDTV_UI_GREYOUT<<((UINT8)E_TDTV_UI_CTRL_3D_TO_2D*2);
    
#if SUPPORT_OSD_DEPTH
    u4Cap |= (UINT32)E_TDTV_UI_GREYOUT<<((UINT8)E_TDTV_UI_CTRL_OSD_DEPTH*2);
#else
    u4Cap |= (UINT32)E_TDTV_UI_HIDE<<((UINT8)E_TDTV_UI_CTRL_OSD_DEPTH*2);
#endif
        
    if( ((SV_VP_MAIN==bPath)&&(
            (bDrvVideoSignalStatus(SV_VP_MAIN)!=SV_VDO_STABLE)
            ||(TDTV_TAG3D_NOT_SUPPORT==u43DTag)
            ||(bIsScalerInput444(SV_VP_MAIN))))
        ||fgDrvTDTVIsFix2DDisplay(bPath)
        ||(!IS_Support3D())
        )
    {
        return u4Cap;
    }
    
#if SUPPORT_3D_NAVI
    if((TDTV_TAG3D_2D==u43DTag)&&(SV_FALSE==fgIs3DPhoto))
    {
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_CTRL_TDNAVI*2);
    }    
#endif

    if((!((E_TDTV_UI_NAVI_FULL_AUTO==eUITDNavi)
            &&(E_TDTV_UI_3D_MODE_AUTO==eDrvTDTVGetUIPacking(bPath))))
        && (!((TDTV_TAG3D_2D!=u43DTag)
            &&(E_TDTV_UI_3D_MODE_AUTO==eDrvTDTVGetUIPacking(bPath))))   
        &&((TDTV_TAG3D_2D==u43DTag)
            ||(E_TDTV_UI_3D_MODE_AUTO!=eDrvTDTVGetUIPacking(bPath))))
    { 
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_CTRL_3D_MODE*2);
    }    

    if((E_TDTV_UI_3D_MODE_OFF!=eDrvTDTVGetUIPacking(bPath))
        &&(E_TDTV_UI_3D_MODE_TTD!=eDrvTDTVGetUIPacking(bPath))
        &&(!((E_TDTV_UI_NAVI_FULL_AUTO==eUITDNavi)&&(TDTV_TAG3D_2D==u43DTag)
            &&(TDTV_TAG3D_2D==u43DTag)&&(E_TDTV_DECODER_INPUT_2D_P==(eTDNAVIDetect|0x1))))        
        )
    {
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_CTRL_3D_TO_2D*2);
    }
    
    if((E_TDTV_UI_3D_MODE_OFF!=eDrvTDTVGetUIPacking(bPath))
        &&(E_TDTV_UI_3D_2_2D_OFF==eDrvTDTVGet3D22D(bPath))
        &&(!((E_TDTV_UI_NAVI_FULL_AUTO==eUITDNavi)&&(TDTV_TAG3D_2D==u43DTag)
                &&(TDTV_TAG3D_2D==u43DTag)&&(E_TDTV_DECODER_INPUT_2D_P==(eTDNAVIDetect|0x1))))
        )
    {
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_CTRL_LR_INVERSE*2);
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_CTRL_PROTRUDE*2);


#if !defined(CC_SUPPORT_3D_DEPTH_CTRL)
        if(E_TDTV_UI_3D_MODE_TTD==eDrvTDTVGetUIPacking(bPath))
#endif            
        {
            u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_CTRL_DEPTH_CTRL*2);
        }

#ifdef CC_3D_SUPPORT_IMG_SAFETY
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_CTRL_IMG_SAFETY*2);            
        if(u1ImgSafetyLvl!=0)
        {
            u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_CTRL_DISTANCE_TV*2);            
        }
#endif 

        
#if SUPPORT_OSD_DEPTH
        u4Cap |= (UINT32)E_TDTV_UI_ENABLE<<((UINT8)E_TDTV_UI_CTRL_OSD_DEPTH*2);        
#endif       
    }
    
    return u4Cap;
}

/**
 * @brief : Query Function for SRM check force Scaler Dram Mode Extension
 * @Param u4Path: SV_VP_MAIN/ SV_VP_PIP
 * @retval: SV_TRUE => Force Scaler Dram Mode
 */
UINT32 u4DrvTDTVForceDramModeQueryEx(UINT32 u4Path)
{  
    if(SV_VP_MAIN==u4Path)
    {    
        UINT32 u4Scl = u4DrvTDTVScalerModeQueryEx(u4Path);
#if defined(CC_MT5396)  

        E_TD_IN eSclIN = TD_SCL_IN(u4Scl);   
#if defined(SUPPORT_3D_OVSN_BY_SCL)
        E_TD_OUT eSclOUT = TD_SCL_OUT(u4Scl);   
#endif

        if((E_TD_IN_FS_I==eSclIN)||(E_TD_IN_FS_P==eSclIN))
        {
            #if defined(SUPPORT_3D_OVSN_BY_SCL)
            return E_TDTV_DRAM_3FB;
            #else
            return E_TDTV_DRAM_6FB;
            #endif
        }
        else
        {
            if((u1DrvDIIsSupportedSignal(u4Path))
#if defined(IS_SUPPORT_3D_PHOTO)
                &&(SV_OFF==fgIs3DPhoto)
#endif // defined(DRV_USE_EXTERNAL_3D_FRC)
                &&(SV_TRUE==bScpipIsLineSyncDispmodeSupported(u4Path))
#if defined(SUPPORT_3D_OVSN_BY_SCL)
                &&(E_TD_OUT_3D_FS!=eSclOUT)
#endif
              )
            {
                return E_TDTV_DRAM_NORMAL;
            }
            else
            {
                return E_TDTV_DRAM_3FB;
            }
        }
#else //defined(CC_MT5396)    

        E_TD_OUT eSclOut = TD_SCL_OUT(u4Scl);        
        if((E_TD_OUT_NATIVE==eSclOut)
#if defined(IS_SUPPORT_3D_PHOTO)
                &&(SV_OFF==fgIs3DPhoto)
#endif//defined(IS_SUPPORT_3D_PHOTO)
          )
        {
            if(u1DrvDIIsSupportedSignal(u4Path))
            {
                return E_TDTV_DRAM_NORMAL;
            }
            else
            {
                return E_TDTV_DRAM_3FB;
            }
        }
        else
        {
            return E_TDTV_DRAM_3FB;       
        }
#endif
    }
    else
    {   //SV_VP_PIP
        return E_TDTV_DRAM_NORMAL;         
    }
}

/**
 * @brief : Query Function for SRM check force Scaler Dram Mode
 * @retval: SV_TRUE => Force Scaler Dram Mode
 */
UINT32 u4DrvTDTVForceDramModeQuery(void)
{
    return u4DrvTDTVForceDramModeQueryEx(SV_VP_MAIN);
}

/**
 * @brief : Query Function for SRM check force PSCAN Dram Mode
 * @retval: SV_TRUE => Force Scaler Dram Mode
 */

UINT32 u4DrvTDTVForceDIDramModeQueryEx(UINT32 u4Path)
{   

    if((!u1DrvDIIsSupportedSignal(u4Path))
        || (SV_TRUE==fgIs3DPhoto))
    {
        return E_TDTV_DI_DRAM_0FB;
    }

    if(SV_VP_MAIN==u4Path)
    {
        UINT32 u4DIMode = u4DrvTDTVDIModeQuery();
        E_TD_IN eDIIn = TD_MIB_IN(u4DIMode);
        
        if((E_TD_IN_FS_I==eDIIn)||(E_TD_IN_FS_P==eDIIn))
        {
            return E_TDTV_DI_DRAM_DOUBLE;
        }
        else
        {
            return E_TDTV_DI_DRAM_NORMAL;
        }
    }
    else
    {//SV_VP_PIP
        return E_TDTV_DI_DRAM_NORMAL;         
    }
}

/**
 * @brief : Query Function for SRM check force PSCAN Dram Mode
 * @retval: SV_TRUE => Force Scaler Dram Mode
 */

UINT32 u4DrvTDTVForceDIDramModeQuery(void)
{    
    return u4DrvTDTVForceDIDramModeQueryEx(SV_VP_MAIN);
}

/**
 * @brief : Query Function for TDNavi Mode
 * @retval: E_TDTV_UI_NAVI
 */

E_TDTV_UI_NAVI eDrvTDTVTDNavimModeQuery(void)
{    
    return eUITDNavi;
}

/**
 * @brief : Query Function for Img Safety
 * @retval: E_TDTV_UI_NAVI
 */

UINT8 eDrvTDTVTDImgSafetyLvlQuery(void)
{    
    return u1ImgSafetyLvl;
}

//=============================================================================
// TDTV  DBG Interface 
//=============================================================================
void vDrvTDTVPackingDbg(E_TDTV_UI_3D_PACKING eTDTVUI3DMode)
{    
    if(IS_Support3D()==SV_TRUE)
    {     
        LOCK_TDTV_STATE();
        
        if(eTDTVUI3DMode==E_TDTV_UI_3D_MODE_END)
        {
            fgIsPackDbg=SV_FALSE;
        }
        else
        {
            fgIsPackDbg=SV_TRUE;
            eUIPacking=eTDTVUI3DMode;
        }
        fgTDTVModeChgTrigger=SV_ON;

        UNLOCK_TDTV_STATE();
    }
}

void vDrvTDTV3D22Ddbg(E_TDTV_UI_3D_2_2D e3D22DMode)
{
    if(IS_Support3D()==SV_TRUE)
    {    
        LOCK_TDTV_STATE();
        
        fg3DTO2DDbg=SV_TRUE;
        eUI3DTO2D=e3D22DMode;
        fgTDTVModeChgTrigger=SV_ON;

        UNLOCK_TDTV_STATE();
        
        return;
    }
}

void vDrvTDTV3DFPRdbg(E_TDTV_UI_3D_FPR e3DFPRMode)
{   
#ifdef CC_3DTV_FPR_SUPPORT
    if(IS_Support3D()==SV_TRUE)
    {
        LOCK_TDTV_STATE();
        
        fgTDTVModeChgTrigger=SV_ON;    
        fgFPRDbg=SV_TRUE;
        eUI3DFPR=e3DFPRMode;
        
        UNLOCK_TDTV_STATE();
        return;
    }
#endif    
}

static const char* szDecInName[]=
{
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_AUTO   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_RESERVE), 
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_2D_I   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_2D_P   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_TTD_I  ),   
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_TTD_P  ),   
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_FS_I   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_FS_P   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_FP_I   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_FP_P   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_TB_I   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_TB_P   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_SBS_I  ),   
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_SBS_P  ),   
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_RD_I   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_RD_P   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_SS_I   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_SS_P   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_LI_I   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_LI_P   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_CB_I   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_CB_P   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_DA_I   ),    
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_DA_P   ),
    ENUM_TO_STR(E_TDTV_DECODER_INPUT_END    ),       
};

static const char* szDecOutName[]=
{
    ENUM_TO_STR(E_TDTV_DECODER_OUTPUT_NATIVE    ),     
    ENUM_TO_STR(E_TDTV_DECODER_OUTPUT_3D_FS     ),      
    ENUM_TO_STR(E_TDTV_DECODER_OUTPUT_3D_2_2D   ),    
    ENUM_TO_STR(E_TDTV_DECODER_OUTPUT_END       ), 
};


static const char* szVdoInName[]=
{
    ENUM_TO_STR(E_TD_IN_AUTO),   
    ENUM_TO_STR(E_TD_IN_RESERVE),
    ENUM_TO_STR(E_TD_IN_2D_I),   
    ENUM_TO_STR(E_TD_IN_2D_P),   
    ENUM_TO_STR(E_TD_IN_TTD_I),  
    ENUM_TO_STR(E_TD_IN_TTD_P),  
    ENUM_TO_STR(E_TD_IN_FS_I),   
    ENUM_TO_STR(E_TD_IN_FS_P),   
    ENUM_TO_STR(E_TD_IN_FP_I),   
    ENUM_TO_STR(E_TD_IN_FP_P),   
    ENUM_TO_STR(E_TD_IN_TB_I),   
    ENUM_TO_STR(E_TD_IN_TB_P),   
    ENUM_TO_STR(E_TD_IN_SBS_I),  
    ENUM_TO_STR(E_TD_IN_SBS_P),  
    ENUM_TO_STR(E_TD_IN_RD_I),   
    ENUM_TO_STR(E_TD_IN_RD_P),   
    ENUM_TO_STR(E_TD_IN_SS_I),   
    ENUM_TO_STR(E_TD_IN_SS_P),   
    ENUM_TO_STR(E_TD_IN_LI_I),   
    ENUM_TO_STR(E_TD_IN_LI_P),   
    ENUM_TO_STR(E_TD_IN_CB_I),   
    ENUM_TO_STR(E_TD_IN_CB_P),   
    ENUM_TO_STR(E_TD_IN_DA_I),   
    ENUM_TO_STR(E_TD_IN_DA_P),   
    ENUM_TO_STR(E_TD_IN_END)   
};


static const char* szVdoOutName[]=
{
    ENUM_TO_STR(E_TD_OUT_NATIVE),
    ENUM_TO_STR(E_TD_OUT_3D_FS),    
    ENUM_TO_STR(E_TD_OUT_3D_TB),    
    ENUM_TO_STR(E_TD_OUT_3D_SBS),   
    ENUM_TO_STR(E_TD_OUT_3D_LI),    
    ENUM_TO_STR(E_TD_OUT_3D_LN_REP), 
    ENUM_TO_STR(E_TD_OUT_3D_FPR),
    ENUM_TO_STR(E_TD_OUT_3D_2_2D),        
    ENUM_TO_STR(E_TD_OUT_END)
};

static const char* szResName[]=
{
    ENUM_TO_STR(E_TD_RES_NORMAL),
    ENUM_TO_STR(E_TD_RES_HALF),
    ENUM_TO_STR(E_TD_RES_DOUBLE),
    ENUM_TO_STR(E_TD_RES_END),
};

void vDrvTDTVQueryConfig()
{   
    #define TDTV_LOG_PATH 1
    #define DEC_IN_NAME(x) szDecInName[x < E_TDTV_DECODER_INPUT_END ? x: (E_TDTV_DECODER_INPUT_END-1)]
    #define DEC_OUT_NAME(x) szDecOutName[x < E_TDTV_DECODER_OUTPUT_END ? x: (E_TDTV_DECODER_OUTPUT_END-1)]

    #define VDO_IN_NAME(x) szVdoInName[x < E_TD_IN_END ? x: (E_TD_IN_END-1)]
    #define VDO_OUT_NAME(x) szVdoOutName[x < E_TD_OUT_END ? x: (E_TD_OUT_END-1)]
    #define RES_NAME(x)  szResName[x < E_TD_RES_END ? x : (E_TD_RES_END-1)]
    
    E_TDTV_DECODER_INPUT eDecInput;
    E_TDTV_DECODER_OUTPUT eDecOutput;
    UINT32 u4Path;
    
    printf("\n===================================================\n");        
    printf("Is Support 3d           : %d\n",IS_Support3D());
    
#ifdef CC_SCPOS_3DTV_SUPPORT
    printf("CC_SCPOS_3DTV_SUPPORT   : %d\n"  ,1);
#else
    printf("CC_SCPOS_3DTV_SUPPORT   : %d\n"  ,0);
#endif
    printf("\n===================================================\n");        
    printf("SUPPORT_3D_NAVI         : %d\n",SUPPORT_3D_NAVI);
    printf("SUPPORT_REALD_TAG       : %d\n",SUPPORT_REALD_TAG);
    printf("SUPPORT_SW_NAVI         : %d\n",SUPPORT_SW_NAVI);
    printf("SUPPORT_OSD_DEPTH       : %d\n",SUPPORT_OSD_DEPTH);
    printf("SUPPORT_FS_FREERUN      : %d\n",SUPPORT_FS_FREERUN);
    printf("SUPPORT_3D_EXT_COMP_TRL : %d\n",SUPPORT_3D_EXT_COMP_TRL);
    printf("SUPPORT_3D_SG_CTRL      : %d\n",SUPPORT_3D_SG_CTRL);
    printf("SUPPORT_3D_BL_CTRL      : %d\n",SUPPORT_3D_BL_CTRL);
    printf("SUPPORT_3D_WITHOUT_MJC  : %d\n",SUPPORT_3D_WITHOUT_MJC);
    printf("SUPPORT_3D_EMBEDED_OSD  : %d\n",SUPPORT_3D_EMBEDED_OSD);
    printf("SUPPORT_3D_ANALOG_SRC   : %d\n",SUPPORT_3D_ANALOG_SRC);
    printf("SUPPORT_PSEUDO_MVC      : %d\n",SUPPORT_PSEUDO_MVC);
    
    printf("CB registration\n\t[  MM]0x%08x\n\t[HDMI]0x%08x\n\t[ OSD]0x%08x\n\t[OSTG]0x%08x\n\t[CROP]0x%08x\n",
        (UINT32 *)TDTVCallback[E_TDTV_CB_MM],
        (UINT32 *)TDTVCallback[E_TDTV_CB_HDMI],
        (UINT32 *)TDTVCallback[E_TDTV_CB_OSD],
        (UINT32 *)TDTVCallback[E_TDTV_CB_OSTG], 
        (UINT32 *)TDTV3DCropCallback[E_TDTV_CB_3D_CROP_MM]
        );
    
    printf("UI SPEC\n\t[ FMT]0x%08x\n\t[CTRL]0x%08x\n",
        u4DrvTDTV3DFMTCapQuery(SV_VP_MAIN),
        u4DrvTDTV3DCtrlCapQuery(SV_VP_MAIN)
        );
    
    printf("[CROP]\n\t[FULL]0x%08x\n\t[ Fix]0x%08x \n\t[   L]0x%08x \n\t[   R]0x%08x \n\t[   T]0x%08x \n\t[   B]0x%08x \n\t\n",
            eCropInfo.fgIsFullResolution,
            eCropInfo.fgIsFixSrcAspectRatio,
            eCropInfo.u4CropLeftOffset,
            eCropInfo.u4CropRightOffset,
            eCropInfo.u4CropTopOffset,
            eCropInfo.u4CropBottomOffset
        );
    
    for(u4Path=0;u4Path<=TDTV_LOG_PATH;++u4Path)
    {    
        printf("===================== VDP:%d========================\n",u4Path);   

        printf("[   SRC] %d \n",(UINT32) bGetSignalType(u4Path));
        printf("[ 3DTAG] %d \n",(UINT32)u4DrvTDTVGetTag3D(u4Path));
        printf("[Cust3D] %d \n", u1IsCustomized3DConfig);

        printf("[    UI] Set as [MODE]%d [322]%d [FPR]%d [INV] %d [NAVI]%d [IMG SFT]%d\n",eUIPacking
            ,eDrvTDTVGet3D22D(u4Path)
            ,eUI3DFPR
            ,eUILRInverse
            ,eUITDNavi
            ,u1ImgSafetyLvl);

        eDecInput = TDTV_UINT32_DEC_INPUT(sVdoCfg[u4Path].DEC[E_DEC_MOD_HDMI]);
        eDecOutput = TDTV_UINT32_DEC_OUTPUT(sVdoCfg[u4Path].DEC[E_DEC_MOD_HDMI]);
        printf("[  HDMI] Detected As %s ,\n\tInput Config As %s,\t Output Config As %s\n",
            DEC_IN_NAME(eDecDetectPack[E_DEC_MOD_HDMI]),
            DEC_IN_NAME(eDecInput),
            DEC_OUT_NAME(eDecOutput));

        eDecInput = TDTV_UINT32_DEC_INPUT(sVdoCfg[u4Path].DEC[E_DEC_MOD_MM]);
        eDecOutput = TDTV_UINT32_DEC_OUTPUT(sVdoCfg[u4Path].DEC[E_DEC_MOD_MM]);
        printf("[   B2R] Detected As %s  ,\t[Inv] %d,\n\tInput Config As %s,\t Output Config As %s \n",
            DEC_IN_NAME(eDecDetectPack[E_DEC_MOD_MM]),
            eB2RLRInv,
            DEC_IN_NAME(eDecInput),
            DEC_OUT_NAME(eDecOutput)
            );

        printf("[  NAVI] Detected As %s\n"
            ,DEC_IN_NAME(eTDNAVIDetect));
        
        printf("[   TTT] In: %s\tOut: %s\n",
            VDO_IN_NAME(TD_DQC_IN(sVdoCfg[u4Path].TTT)),
            VDO_OUT_NAME(TD_DQC_OUT(sVdoCfg[u4Path].TTT)));            

        printf("[    DQ] In: %s\tOut: %s\n",
            VDO_IN_NAME(TD_DQC_IN(sVdoCfg[u4Path].DQC)),
            VDO_OUT_NAME(TD_DQC_OUT(sVdoCfg[u4Path].DQC)));


        printf("[    DI] In: %s\tOut: %s\n",
            VDO_IN_NAME(TD_MIB_IN(sVdoCfg[u4Path].DI)),
            VDO_OUT_NAME(TD_MIB_OUT(sVdoCfg[u4Path].DI)));
            
        printf("[    SC] In: %s\tH:%s \tV:%s\n\tOut: %s \tH:%s \tV:%s\n",
            VDO_IN_NAME(TD_SCL_IN(sVdoCfg[u4Path].SC)),
            RES_NAME(TD_SCL_PRE_H(sVdoCfg[u4Path].SC)),
            RES_NAME(TD_SCL_PRE_V(sVdoCfg[u4Path].SC)),       
            VDO_OUT_NAME(TD_SCL_OUT(sVdoCfg[u4Path].SC)),
            RES_NAME(TD_SCL_PST_H(sVdoCfg[u4Path].SC)),
            RES_NAME(TD_SCL_PST_V(sVdoCfg[u4Path].SC)));

        printf("[   MJC] In: %s\tOut: %s\n",
            VDO_IN_NAME(TD_MJC_IN(sVdoCfg[u4Path].MJC)),
            VDO_OUT_NAME(TD_MJC_OUT(sVdoCfg[u4Path].MJC)));

        printf("[   TTD] In: %s\tOut: %s\n",
            VDO_IN_NAME(TD_TTD_IN(sVdoCfg[u4Path].TTD)),
            VDO_OUT_NAME(TD_TTD_OUT(sVdoCfg[u4Path].TTD)));
            
    	printf("[   OSTG]\n"); 
        Printf("\teOSTGCfg %d\n", sVdoCfg[u4Path].OSTG.eOSTGCfg);
        Printf("\tu4Clk    %d\n", sVdoCfg[u4Path].OSTG.u4Clk);
        Printf("\tu4Width  %d\n", sVdoCfg[u4Path].OSTG.u4Width);
        Printf("\tu4Height %d\n", sVdoCfg[u4Path].OSTG.u4Height);
        Printf("\tu4VTotal %d\n", sVdoCfg[u4Path].OSTG.u4VTotal);
        Printf("\tu4HTotal %d\n", sVdoCfg[u4Path].OSTG.u4HTotal);
        Printf("\tu4FrameRate %d\n", sVdoCfg[u4Path].OSTG.u4FrameRate);


    	printf("[   FBM]\n");
        Printf("\tSCL DRAM MODE %d\n", u4DrvTDTVForceDramModeQueryEx(u4Path));
        Printf("\tMIB_DRAM_MODE %d\n", u4DrvTDTVForceDIDramModeQueryEx(u4Path));
    }
}

//=============================================================================
// TDTV  UI Interface 
//=============================================================================

void vDrvTDTVPacking(E_TDTV_UI_3D_PACKING eTDTVUI3DMode)
{
    TDTV_TAG3D_TYPE_T u4Tag=u4DrvTDTVGetTag3D(SV_VP_MAIN);


    if(IS_Support3D()!=SV_TRUE)
    {  
        eUIPacking = E_TDTV_UI_3D_MODE_OFF;
        return;
    }

    if(fgIsPackDbg==SV_TRUE)
    {
        return;
    }    

    LOCK_TDTV_STATE();
    
    if((u4Tag==TDTV_TAG3D_FP)||(u4Tag==TDTV_TAG3D_FS))//FS or FP
    {
        eUIPacking=E_TDTV_UI_3D_MODE_AUTO;
    }
    else
    {
        eUIPacking=eTDTVUI3DMode;
    }
    fgTDTVModeChgTrigger=SV_ON;
    
    UNLOCK_TDTV_STATE();
}
/**
 * @brief :Interface for UI to inverse L R View
 * @param eTDTVLRInverse: E_TDTV_UI_LR_NORMAL , E_TDTV_UI_LR_INVERSE
 */
void vDrvTDTVLRInverse(E_TDTV_UI_LR eTDTVLRInverse)
{
    eUILRInverse=eTDTVLRInverse;
#ifdef CC_MT5396 
    MJC_SetLRSwitch(u4DrvTDTVIsILRInv());  
#endif
    return;
}
/**
 * @brief :Interface for UI to decide Depth difference of 3D Content
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVDepth(UINT32 u4DepthField)
{
    #ifdef DRV_USE_EXTERNAL_3D_FRC    
    DRVCUST_SetExtFrc3DDepth(u4DepthField);
    #else
    vDrvTTDSetDepthOfField((UINT8)u4DepthField);
    #endif   

     //audio depths links with video depths, by gallen/sammy, 20110526
    #ifdef CC_3DTV_AUDIO_LINK_SUPPORT
    AUD_3D_AVLINK_Config(u4DepthField);
    #endif    
     
    return;
}
/**
 * @brief :Interface for UI to force convert 3D Video into 2D
 * @param e3D22DMode:E_TDTV_UI_3D_2_2D_ON,E_TDTV_UI_3D_2_2D_OFF
 */
void vDrvTDTV3D22D(E_TDTV_UI_3D_2_2D e3D22DMode)
{
    LOCK_TDTV_STATE();
    if(fg3DTO2DDbg==SV_FALSE)
    {
        eUI3DTO2D=e3D22DMode;
    }
    
    fgTDTVModeChgTrigger=SV_ON;
    UNLOCK_TDTV_STATE();
    return;
}

/**
 * @brief :Interface for UI to Chose FPR Mode
 * @param e3D22DMode:E_TDTV_UI_3D_FPR_OFF,E_TDTV_UI_3D_FPR_V1,E_TDTV_UI_3D_FPR_V2
 */
void vDrvTDTV3DFPR(E_TDTV_UI_3D_FPR e3DFPRMode)
{
#ifdef CC_3DTV_FPR_SUPPORT
    UINT32 j;
    static UINT8 u1PrePRMode=0;
    UINT64 u8Tmp;

    
    LOCK_TDTV_STATE();
    if(fgFPRDbg==SV_FALSE)
    {
        eUI3DFPR=e3DFPRMode;
    }

    if((IS_LVDS_DISP_3D_POLARIZED)&&(u1PrePRMode!=eUI3DFPR))
    {
        for(j=0;j<NUM_OF_PACKING_TYPE;++j)
        {   
            //SWAP Configuration
            u8Tmp = u8VdoPathProcCfg[j];
            u8VdoPathProcCfg[j]= u8VdoPathProcCfgFullHDPR[j];
            u8VdoPathProcCfgFullHDPR[j]= u8Tmp;                
        }
    }
    
    fgTDTVModeChgTrigger=SV_ON;
    u1PrePRMode=eUI3DFPR;
    UNLOCK_TDTV_STATE();
    
#endif    
}

/**
 * @brief :Interface for UI to decide Protrude Sceen for 2D to 3D
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVProtrude(UINT32 u4Protrude)
{
    #ifdef DRV_USE_EXTERNAL_3D_FRC    
    //DRVCUST_SetExtFrc3DDepth(u4Protrude);
    DRVCUST_SetExtFrcProtrude(u4Protrude);
    #else
    vDrvTTDSetProtrudeFromScreen((UINT8)u4Protrude);
    #endif
}


/**
 * @brief :Interface for UI to decide Distance beteen TV and Viewer
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVDistanceFromTV(UINT32 u4Distant)
{
    #ifdef DRV_USE_EXTERNAL_3D_FRC    
    DRVCUST_SetExtFrcDistanceFromTV(u4Distant);
    #else
    vDrvTTDSetDistanceToTV((UINT8)u4Distant);
    #endif
}

/**
 * @brief :Interface for UI to decide OSD depth
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVOSDDepth(UINT32 u4OSDDepth)
{
    LOCK_TDTV_STATE();

    #if defined(DRV_USE_EXTERNAL_3D_FRC)
    DRVCUST_SetExtFrcOSDDepth(u4OSDDepth);
    #else
    u1OSDDepth = (UINT8)u4OSDDepth;     
    #endif

    fgTDTVModeChgTrigger=SV_ON;
    UNLOCK_TDTV_STATE();
    
}

/**
 * @brief :Interface for UI to set 3D Navi
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVTDNAVI(E_TDTV_UI_NAVI eTDTVNavi)
{
    eUITDNavi = eTDTVNavi;   
}

/**
* @brief :Interface for UI to set 3D Depth Control 
* @param SV_ON, SV_OFF 
*/
void vDrvTDTVTDDC(UINT8 u1OnOff)
{    
	#ifdef DRV_USE_EXTERNAL_3D_FRC        
	//DRVCUST_SetExtFrcDepthControl(u1OnOff);    
	#else    
	vDrvTTDSetDepthControl(u1OnOff);    
	#endif
}

/**
 * @brief :Interface for UI to set Image Safety
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVTImgSafety(UINT8 u1Level)
{
#ifdef CC_3D_SUPPORT_IMG_SAFETY
    u1ImgSafetyLvl=u1Level;
    vDrvTTDSetImageSafetyLevel(u1ImgSafetyLvl);
#endif    
}


/**
 * @brief :Apply Special Customize 3D Cfg, 
 * @param None , this operation can not be inverse
 */
void vDrvTDTVSetCustomize3DCfg(void)
{
    #if defined(SUPPORT_CUSTOMIZED_3D_CFG)    
    UINT32 j;
    for(j=0;j<NUM_OF_PACKING_TYPE;++j)
    {
        u8VdoPathProcCfg[j]=u8VdoPathProcCustomizedCfg[j];
    }
    #endif
    
    u1IsCustomized3DConfig=SV_ON;
}

//=============================================================================
//Obsolete Interface , Maybe Changed in the Future
//=============================================================================

void vTV3DGetForce2D(UINT8 bPath, UINT8 *pu1Force2D, UINT8 *pu1LR)
{
    //Legacy API, From MT5396/68 ,
    //3D to 2D will be done by specific Engine,
    //No longer need Deocder Support
    ASSERT(pu1Force2D != NULL);
    ASSERT(pu1LR != NULL);
    *pu1Force2D = 0;
    *pu1LR = 0;     
}
//=============================================================================
//                                 Main Thread
//=============================================================================

void vDrvTDTVInit(UINT8 forceinit)
{
    static UINT8 u1Initialized=0;
#if defined(CC_MT5396) || defined(DRV_USE_EXTERNAL_3D_FRC) || defined(SUPPORT_CUSTOMIZED_3D_CFG)                   
    UINT32 j;
#endif

    if(u1Initialized==0)
    {  
        VERIFY(x_sema_create(&_hTDTVModeChgIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    if(u1Initialized==0 || forceinit == TRUE)
    {  
        //Intialize FW Register
        vIO32WriteFldAlign(TDTV_FW_00,0x1,BL_MLOOP);

#ifdef DRV_USE_EXTERNAL_3D_FRC
        for(j=0;j<NUM_OF_PACKING_TYPE;++j)
        {
            u8VdoPathProcCfg[j]=
                u8VdoPathProcCfgExt[j];
        }
#else //DRV_USE_EXTERNAL_3D_FRC

#if defined(SUPPORT_CUSTOMIZED_3D_CFG)
        if(SV_ON==u1IsCustomized3DConfig)
        {
            for(j=0;j<NUM_OF_PACKING_TYPE;++j)
            {
                u8VdoPathProcCfg[j]=u8VdoPathProcCustomizedCfg[j];
            }
        }
        else
#endif            
        {
#if defined(CC_MT5396)                
            if(u4DrvTDTVModelWithoutMJC())
            {
                for(j=0;j<NUM_OF_PACKING_TYPE;++j)
                {
                    u8VdoPathProcCfg[j]=
                        u8VdoPathProcCfgWithoutMJC[j];
                }
            }
            else if(IS_LVDS_DISP_3D_SHUTTER)
            {
                for(j=0;j<NUM_OF_PACKING_TYPE;++j)
                {
                    u8VdoPathProcCfg[j]=
                        u8VdoPathProcCfgShutter[j];
                }
            }
#endif  //defined(CC_MT5396)        
        }

#endif //DRV_USE_EXTERNAL_3D_FRC

#if SUPPORT_3D_NAVI    	
    	vDrvTdNaviInit();
#endif

#if SUPPORT_REALD_TAG    	
    	vDrvRealDInit();
#endif    	

    	vDrvTD22DInit();

#if SUPPORT_3D_EMBEDED_OSD
        fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_OSD,vDrvTDTVOSDCallback);
#endif

        fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_TTD,vDrvTDTVOSDCallback);

        vDrvTDTVDecConfig(E_DEC_MOD_HDMI);
        vDrvTDTVDecConfig(E_DEC_MOD_MM);
        vDrvTDTVDecConfig(E_DEC_MOD_OTHERS);
        
        vDrvTDTVDIConfig();
        vDrvTDTVMJCConfig();
        vDrvTDTVSCConfig(SV_VP_MAIN);  
        vDrvTDTVOSDConfig();
        vDrvTDTVOSTGConfig();

        sVdoCfg[SV_VP_PIP]=sVdoCfg[SV_VP_MAIN];        
        u1Initialized=1;
    }
}

static void vDrvTDTVSysCfg(void)
{
    static UINT8 u1PreSCLDramMode=0xFF;
    static UINT8 u1PreDIDramMode=0xFF;
    
    UINT32 u4VdoMainPathNeedModeChange=0;
    UINT32 u4VdoPipPathNeedModeChange=0;
    UINT32 u4VdoNeedResendSRMCfg=0;
    UINT32 u4SClDramMode;
    UINT32 u4DIDramMode;
    
    E_DEC_MOD eDecMod=eGetDecModule(SV_VP_MAIN);
    S_TDTVVdoCfg sPreVdoCfg[NUM_OF_PATH];

    sPreVdoCfg[SV_VP_MAIN]=sVdoCfg[SV_VP_MAIN];
    sPreVdoCfg[SV_VP_PIP]=sVdoCfg[SV_VP_PIP];
       
    //Reconfig All Video Module Setting
    vDrvTDTVDecConfig(E_DEC_MOD_HDMI);
    vDrvTDTVDecConfig(E_DEC_MOD_MM);
    vDrvTDTVDecConfig(E_DEC_MOD_OTHERS);

    vDrvTDTVTTTConfig(SV_VP_MAIN);
    vDrvTDTVTTTConfig(SV_VP_PIP);    
    
    vDrvTDTVDIConfig();
    vDrvTDTVMJCConfig();
    vDrvTDTVDQCConfig();     
    vDrvTDTVTTDConfig();    
    vDrvTDTVSCConfig(SV_VP_MAIN);
    vDrvTDTVSCConfig(SV_VP_PIP);    
    vDrvTDTVOSDConfig();
    vDrvTDTVOSTGConfig();

    u4SClDramMode=u4DrvTDTVForceDramModeQueryEx(SV_VP_MAIN);
    u4DIDramMode=u4DrvTDTVForceDIDramModeQueryEx(SV_VP_MAIN);

    //Checking Scaler Main Path Callback
    if(u4SClDramMode!=u1PreSCLDramMode)
    {
        u4VdoNeedResendSRMCfg=1;
    } 

    if(u4DIDramMode!=u1PreDIDramMode)
    {
        u4VdoNeedResendSRMCfg=1;
    } 

    u1PreSCLDramMode=u4SClDramMode;
    u1PreDIDramMode=u4DIDramMode;

    //Checking Scaler Main Path Callback
    if(sPreVdoCfg[SV_VP_MAIN].TTT!=sVdoCfg[SV_VP_MAIN].TTT)
    {
        u4VdoNeedResendSRMCfg=1;
    } 

    if(sPreVdoCfg[SV_VP_PIP].TTT!=sVdoCfg[SV_VP_PIP].TTT)
    {
        u4VdoNeedResendSRMCfg=1;
    } 


    //Checking Scaler Main Path Callback
    if(sPreVdoCfg[SV_VP_MAIN].SC!=sVdoCfg[SV_VP_MAIN].SC)
    {
        u4VdoNeedResendSRMCfg=1;
    } 

    //Checking Scaler Sub Path Callback for scart out
    if(sPreVdoCfg[SV_VP_PIP].SC!=sVdoCfg[SV_VP_PIP].SC)
    {
        u4VdoNeedResendSRMCfg=1;
    } 

    //Checking MJC Callback
    if(sPreVdoCfg[SV_VP_MAIN].MJC!=sVdoCfg[SV_VP_MAIN].MJC)
    {
        u4VdoNeedResendSRMCfg=1;
    }

    //Checking DQC Callback
    if(sPreVdoCfg[SV_VP_MAIN].DQC!=sVdoCfg[SV_VP_MAIN].DQC)
    {
        TTD_MODE_CHANGE_CB(E_TDTV_CB_DQC);        
    }

    //Checking TTD Callback
    if(sPreVdoCfg[SV_VP_MAIN].TTD!=sVdoCfg[SV_VP_MAIN].TTD)
    {
        TTD_MODE_CHANGE_CB(E_TDTV_CB_TTD);        
    }

#ifdef DRV_USE_EXTERNAL_3D_FRC
    //Checking OSD Callback
    if((sPreVdoCfg[SV_VP_MAIN].OSD.eOSDType !=sVdoCfg[SV_VP_MAIN].OSD.eOSDType)||
        (sPreVdoCfg[SV_VP_MAIN].OSD.u4PanelWidth !=sVdoCfg[SV_VP_MAIN].OSD.u4PanelWidth)||
        (sPreVdoCfg[SV_VP_MAIN].OSD.u4PanelHeight !=sVdoCfg[SV_VP_MAIN].OSD.u4PanelHeight))    
    {
        u4VdoMainPathNeedModeChange=1;    
        TTD_MODE_CHANGE_CB(E_TDTV_CB_OSD);        
    }

    //Checking LVDS callback
    if((sPreVdoCfg[SV_VP_MAIN].OSTG.eOSTGCfg !=sVdoCfg[SV_VP_MAIN].OSTG.eOSTGCfg)||
        (sPreVdoCfg[SV_VP_MAIN].OSTG.u4Clk !=sVdoCfg[SV_VP_MAIN].OSTG.u4Clk)||
        (sPreVdoCfg[SV_VP_MAIN].OSTG.u4HTotal !=sVdoCfg[SV_VP_MAIN].OSTG.u4HTotal)||
        (sPreVdoCfg[SV_VP_MAIN].OSTG.u4VTotal !=sVdoCfg[SV_VP_MAIN].OSTG.u4VTotal)||
        (sPreVdoCfg[SV_VP_MAIN].OSTG.u4FrameRate !=sVdoCfg[SV_VP_MAIN].OSTG.u4FrameRate)||
        (sPreVdoCfg[SV_VP_MAIN].OSTG.u4Width !=sVdoCfg[SV_VP_MAIN].OSTG.u4Width)||
        (sPreVdoCfg[SV_VP_MAIN].OSTG.u4Height !=sVdoCfg[SV_VP_MAIN].OSTG.u4Height))    
    {
        u4VdoMainPathNeedModeChange=1;    
        TTD_MODE_CHANGE_CB(E_TDTV_CB_OSTG);        
    }
#endif

    //Update Storage Config setting
    sPreVdoCfg[SV_VP_MAIN]=sVdoCfg[SV_VP_MAIN];
    sPreVdoCfg[SV_VP_PIP]=sVdoCfg[SV_VP_PIP];    

    if(u4VdoNeedResendSRMCfg)
    {
        vDrvTDTVUpdateSRM();
    }    



    if(SV_TRUE==u1LRFreeRun[eDecMod])
    {
        vRegWriteFldAlign(LPF_PIP_00,SV_TRUE ,DISP_R_MAIN_FREERUN);
    }
    else
    {
        vRegWriteFldAlign(LPF_PIP_00,SV_FALSE,DISP_R_MAIN_FREERUN);
    }

    //Trigger Main Mode Change if no Decoder Mode Change
    if(u4VdoMainPathNeedModeChange)
    {
        if(bDrvVideoSignalStatus(SV_VP_MAIN)==SV_VDO_STABLE)
        {   
            vSetMainFlg(MAIN_FLG_MODE_CHG);
            vSetMainFlg(MAIN_FLG_MODE_DET_DONE); 
        }        
    }
    //Trigger PIP Mode Change if no Decoder mode Change
    if(u4VdoPipPathNeedModeChange)
    {   
        if(bDrvVideoSignalStatus(SV_VP_PIP)==SV_VDO_STABLE)
        {   
            vSetMainFlg(PIP_FLG_MODE_CHG);
            vSetMainFlg(PIP_FLG_MODE_DET_DONE);             
        }
    }
}

UINT32 u4DrvTDTVIsScanningBLEnabled()
{
#if SUPPORT_3D_BL_CTRL      
    if((u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF) && IS_LVDS_DISP_3D_SHUTTER)
    {
        return SV_ON;
    }
    else
    {
        return SV_OFF;
    }
#else
    return SV_OFF;
#endif
}

static inline void vDrv3DBLCfg(void)
{
#if SUPPORT_3D_BL_CTRL      
    if(IO32ReadFldAlign(TDTV_FW_00, BL_MLOOP))
    {
        if(u4DrvTDTVIsScanningBLEnabled())
        {        
            vApiSetScanningPWM(wDrvGetOutputHTotal(), wDrvGetOutputVTotal()
                , SCAN_PWM_START,  wDrvGetOutputVTotal() - PANEL_GetPanelHeight());
        }
    }
#endif
}


void vDrvTDTVOSDCallback(void *pvArgv)
{
#if SUPPORT_3D_EMBEDED_OSD
    S_TDTV_OSD_CONFIG eOSDCfg = eDrvTDTVOSDModeQuery();

    switch(eOSDCfg.eOSDType)
    {
        case E_TDTV_OSD_SBS:      
            OSD_PLA_Redraw3D(OSD_3D_SBSHALF);
            break;
        case E_TDTV_OSD_TB:      
            OSD_PLA_Redraw3D(OSD_3D_TPANDBT);
            break;
        default:
            OSD_PLA_Redraw3D(OSD_3D_NONE);
    }
#endif
}

static inline void vDrvTDTVDQCCfg(void)
{
    UINT32 u4DQC= u4DrvTDTVDQCModeQuery();
    E_TD_IN eDQCIn= TD_DQC_IN(u4DQC);
    E_TD_OUT eDQCOut= TD_DQC_OUT(u4DQC);

    sDQConfig sDQCfg;
    sCBConfig sCBCfg;

#ifdef CC_3D_SUPPORT_LI_DA    
    sLIConfig sLICfg;
#endif

    sDQCfg.u2SrcWidth= wDrvVideoInputWidth(SV_VP_MAIN);
    sDQCfg.u2SrcHeight=wDrvVideoInputHeight(SV_VP_MAIN);

    sCBCfg.u2SrcWidth=wDrvVideoInputWidth(SV_VP_MAIN);

    if(E_TD_OUT_3D_SBS==eDQCOut)
    {
        switch(eDQCIn)
        {
            case E_TD_IN_SS_I:
            case E_TD_IN_SS_P:
                //DQ
                sDQCfg.u1OnOff=SV_ON;
                sDQCfg.u1X2Mode=SV_OFF;
                sDQCfg.u1SrcMode=0;
                //CB
                sCBCfg.u1OnOff=SV_OFF;
                sCBCfg.u1VI = SV_OFF;
                //LI
#ifdef CC_3D_SUPPORT_LI_DA                    
                sLICfg.u1OnOff=SV_OFF;                
#endif
                break;
            case E_TD_IN_CB_I:
            case E_TD_IN_CB_P:
                //DQ
                sDQCfg.u1OnOff=SV_OFF;
                sDQCfg.u1X2Mode=SV_OFF;
                sDQCfg.u1SrcMode=0;
                //CB
                sCBCfg.u1OnOff=SV_ON;
                sCBCfg.u1VI = SV_OFF;                
                //LI
#ifdef CC_3D_SUPPORT_LI_DA                    
                sLICfg.u1OnOff=SV_OFF;                
#endif
                break;
            case E_TD_IN_DA_I:
            case E_TD_IN_DA_P:
                //DQ
                sDQCfg.u1OnOff=SV_OFF;
                sDQCfg.u1X2Mode=SV_OFF;
                sDQCfg.u1SrcMode=0;
                //CB
                sCBCfg.u1OnOff=SV_ON;
                sCBCfg.u1VI = SV_ON;                
                //LI
#ifdef CC_3D_SUPPORT_LI_DA                    
                sLICfg.u1OnOff=SV_OFF;                
#endif
                break;
            case E_TD_IN_LI_P:
                //DQ
                sDQCfg.u1OnOff=SV_OFF;
                sDQCfg.u1X2Mode=SV_OFF;
                sDQCfg.u1SrcMode=0;
                //CB
                sCBCfg.u1OnOff=SV_OFF;
                sCBCfg.u1VI = SV_OFF;                
                //LI
#ifdef CC_3D_SUPPORT_LI_DA                    
                sLICfg.u1OnOff=SV_ON;                
#endif
                break;
            default:
                //DQ
                sDQCfg.u1OnOff=SV_OFF;
                sDQCfg.u1X2Mode=SV_OFF;
                sDQCfg.u1SrcMode=0;
                //CB
                sCBCfg.u1OnOff=SV_OFF;
                sCBCfg.u1VI = SV_OFF;              
                //LI
#ifdef CC_3D_SUPPORT_LI_DA                    
                sLICfg.u1OnOff=SV_OFF;                
#endif
        }
    }
    else
    {
        sDQCfg.u1OnOff=SV_OFF;
        sDQCfg.u1X2Mode=SV_OFF;
        sDQCfg.u1SrcMode=0;
        
        sCBCfg.u1OnOff=SV_OFF;
        sCBCfg.u1VI = SV_OFF;
         
        //LI
#ifdef CC_3D_SUPPORT_LI_DA                            
        sLICfg.u1OnOff=SV_OFF;
#endif
    }

    vDrvSetDQ(&sDQCfg);
    vDrvSetCB(&sCBCfg);
#ifdef CC_3D_SUPPORT_LI_DA        
    vDrvSetLI(&sLICfg);
#endif
}

static inline void vDrvTDTVPQCfg(void)
{
#if defined(CC_MT5389) || defined(CC_MT5368)
    E_TD_OUT eDQCOut= TD_SCL_OUT(u4DrvTDTVScalerModeQuery());
    vDrvLIPostSharpOnOff((E_TD_OUT_3D_LI==eDQCOut)||(E_TD_OUT_3D_LN_REP==eDQCOut));   
#endif    
}


void vDrvTDTVTTDCfg(void)
{    
#if defined(CC_MT5368) || defined(CC_MT5389)
    static UINT32 u4TTDPrev = 0;
    static UINT16 u2WidthPrev, u2HeightPrev;

	UINT32 u4TTD;
	UINT16 u2Width, u2Height;
    E_TD_IN eTTDIn;
    E_TD_OUT eTTDOut;
	
    u4TTD = u4DrvTDTVTTDModeQuery();

	u2Width  = PANEL_GetPanelWidth();
	u2Height = PANEL_GetPanelHeight();

    if(IS_LVDS_DISP_3D_POLARIZED)
    {
    	//u2Height = u2Height >> 1;
    }

	u2Width  = ((u2Width + 7) >> 3) << 3;
    u2Height = ((u2Height+7) >> 3) << 3; // alignment 8


    if ((u4TTD != u4TTDPrev) || (u2Width != u2WidthPrev) || (u2Height != u2HeightPrev))
    {
    	// Use eTTDIn, eTTDOut to determine vDrvTTDSetEnable & vDrvTTDSetMode
	    eTTDIn  = TD_TTD_IN(u4TTD);
	    eTTDOut = TD_TTD_OUT(u4TTD);
	    LOG(0,"TTDIn = %d, TTDOut = %d\n", eTTDIn, eTTDOut);
	    
	#ifdef DRV_USE_EXTERNAL_3D_FRC
		//vDrvTTDSetMode(SCL_2D_TO_FS);
		vDrvTTDSetEnable(SV_OFF);
	#else
	    if ((eTTDIn == E_TD_IN_2D_P) && (eTTDOut == E_TD_OUT_3D_LI))
	    {
	    	vDrvTTDSetEnable(SV_ON);
	    	vDrvTTDSetMode(SCL_2D_TO_PR);
	    }
	    else if ((eTTDIn == E_TD_IN_LI_P) && ((eTTDOut == E_TD_OUT_3D_LI)|| (eTTDOut == E_TD_OUT_NATIVE)))
		{
			vDrvTTDSetEnable(SV_ON);
			vDrvTTDSetMode(SCL_PR_TO_PR);
		}
	    else
	    {
			vDrvTTDSetEnable(SV_OFF);
	    }
	#endif

	    vDrvTTDSetPicSize(u2Width, u2Height);

		u2WidthPrev = u2Width;
		u2HeightPrev = u2Height;
	    u4TTDPrev = u4TTD;
    }
#endif    

    u1DrvRealDMaskOnOff(SV_VP_MAIN, (TDTV_TAG3D_REALD==u4DrvTDTVGetTag3D(SV_VP_MAIN)));
    u1DrvRealDMaskOnOff(SV_VP_PIP, (TDTV_TAG3D_REALD==u4DrvTDTVGetTag3D(SV_VP_PIP)));
}


/**
 * @brief :Process to Communicate with Exnternal Companion Chip
 */
static inline void vDrvTDTVExtProc(void)
{
#if defined(DRV_USE_EXTERNAL_3D_FRC) || SUPPORT_3D_EXT_COMP_TRL
    DRVCUST_SetExtFrc3DFormat(u4DrvTDTV3DModeQuery());
#endif
}

static inline void vDrvTDTVInvProc(void)
{
#ifdef DRV_USE_EXTERNAL_3D_FRC
    static E_TDTV_UI_LR ePreLRInv;
    E_TDTV_UI_LR eLRInv = (E_TDTV_UI_LR)u4DrvTDTVIsILRInv();    
    if(ePreLRInv != eLRInv)
    {    
        DRVCUST_SetExtFrc3DInverse((UINT32)eLRInv);
        ePreLRInv = eLRInv;
    }    
#elif defined(CC_MT5396)
    UINT32 u4IsInv = u4DrvTDTVIsILRInv();
 
    if(u4DrvTDTVModelWithoutMJC())
    {
        if(LVDS_DISP_3D==LVDS_DISP_3D_POLARIZED_RLRL)
        {
            u4IsInv = u4IsInv!=SV_ON;
        }
        
        if(E_TDTV_UI_3D_MODE_TTD==eDrvTDTVGetCurCfg())
        {
            vDrvTTDSetLREyeInverse(u4IsInv);
        }
        else
        {
            vDrvTTDSetLREyeInverse(SV_OFF);
        }
    }
#else

    UINT32 u4IsInv = u4DrvTDTVIsILRInv();

    if(LVDS_DISP_3D==LVDS_DISP_3D_POLARIZED_RLRL)
    {
        u4IsInv = !u4IsInv;
    }

    if(E_TDTV_UI_3D_MODE_FS==eDrvTDTVGetCurCfg())
    {    
        vDrvTTDSetLREyeInverse(u4IsInv);
        vScpipSetDispROutInv(SV_OFF);        
    }
    else
    {
        if(E_TDTV_UI_3D_MODE_FS==eDrvTDTVGetCurCfg())
        {
            u4IsInv = !u4IsInv;
        }
        vDrvTTDSetLREyeInverse(SV_OFF);        
        vScpipSetDispROutInv(u4IsInv);
    }
#endif
}

static inline void vDrvTDTVUpdate3D22(UINT32 bPath)
{
    E_TD_IN eTTDIn=TD_TTT_IN(sVdoCfg[bPath].TTT);
    E_TD_OUT eTTDOut=TD_TTT_OUT(sVdoCfg[bPath].TTT);
    E_TDTV_UI_3D_2_2D e322 =  eDrvTDTVGet3D22D(bPath);

    UINT32 u4OnOff,u4ModelSel,u4MskSel;

    eTTDIn = (E_TD_IN)((UINT32)eTTDIn & ~0x1);

    if(E_TD_OUT_3D_2_2D==eTTDOut)
    {    
        u4OnOff=SV_ON;
        switch(eTTDIn)
        {        
            case E_TD_IN_FS_I:
                u4ModelSel=TD22D_MODE_FRAME_SEQUENTIAL;
                u4MskSel= E_TDTV_UI_3D_2_2D_L == e322
                    ? TD22D_FS_LEFT : TD22D_FS_RIGHT;
                break;
            case E_TD_IN_SBS_I:
            case E_TD_IN_SS_I:
            case E_TD_IN_RD_I:
                u4ModelSel=TD22D_MODE_SIDE_BY_SIDE;
                u4MskSel= E_TDTV_UI_3D_2_2D_L == e322
                    ? TD22D_SBS_LEFT : TD22D_SBS_RIGHT;
                break;            
            case E_TD_IN_TB_I:
                u4ModelSel=TD22D_MODE_TOP_AND_BOTTOM;
                u4MskSel= E_TDTV_UI_3D_2_2D_L == e322          
                    ? TD22D_TB_TOP : TD22D_TB_BOTTOM;
                break;
            case E_TD_IN_LI_I:
                u4ModelSel=TD22D_MODE_LINE_INTERLEAVE;
                u4MskSel= E_TDTV_UI_3D_2_2D_L == e322       
                    ? TD22D_LI_EVEN_LINE : TD22D_LI_ODD_LINE;
                break;            
            default:
                u4ModelSel=TD22D_MODE_FRAME_SEQUENTIAL;
                u4MskSel= E_TDTV_UI_3D_2_2D_L == e322        
                    ? TD22D_FS_LEFT : TD22D_FS_RIGHT;
        }
    }
    else
    {
        u4OnOff=SV_OFF;
        u4ModelSel=TD22D_MODE_FRAME_SEQUENTIAL;
        u4MskSel=TD22D_FS_LEFT;
    }
    
    u1DrvTD22DSetMode(bPath, u4OnOff , u4ModelSel, u4MskSel);
}

static inline void vDrvTDTVCropProc(void)
{
#ifdef IS_SUPPORT_3D_PHOTO
    TDTV_3D_CROP_INTO_T eTurnOff={1,1,0,0,0,0};

    if((SV_TRUE==eCropInfo.fgIsFixSrcAspectRatio)
         &&(bGetSignalType(SV_VP_MAIN)==SV_ST_MPEG))
    {
        if(E_TDTV_UI_3D_MODE_OFF!=u4DrvTDTV3DModeQuery()
            ||(E_TDTV_UI_3D_2_2D_OFF!=eDrvTDTVGet3D22D(SV_VP_MAIN)))
        {
            fgIsPhotoCropbyB2R=SV_OFF;
        }
        else
        {
            fgIsPhotoCropbyB2R=SV_TRUE;
        }
        
        if(SV_TRUE==fgIsPhotoCropbyB2R)
        {
            vDrvTDTVSet3DPhoto(&eTurnOff,SV_TRUE);
        }
        else
        {
            vDrvTDTVSet3DPhoto(&eCropInfo,SV_TRUE);
        }
        
        fgIs3DPhoto=SV_TRUE;
    }
    else
    {
        vDrvTDTVSet3DPhoto(&eCropInfo,SV_OFF);
        fgIs3DPhoto=SV_OFF;
        fgIsPhotoCropbyB2R=SV_OFF;
    }   
#endif
}

static inline UINT32 u4DrvTDTVCheckModeChange(void)
{
    static UINT8 preIsFix322=0xFF;
    static UINT8 preIsLineSyncSupport=0xFF;
    UINT32 u4Is322=fgDrvTDTVIsFix2DDisplay(SV_VP_MAIN);
    UINT32 u4LineSyncSupport=bScpipIsLineSyncDispmodeSupported(SV_VP_MAIN);

    if((preIsFix322!=u4Is322)
        ||(u4LineSyncSupport!=preIsLineSyncSupport))
    {
        preIsFix322=u4Is322;
        preIsLineSyncSupport=u4LineSyncSupport;        
        return SV_TRUE;
    }
    else
    {    
        preIsFix322=u4Is322;
        preIsLineSyncSupport=u4LineSyncSupport;
        return SV_FALSE;
    }
}

VOID vDrvTDTVProc()
{
    
    if(IO32ReadFldAlign(TDTV_FW_00, BL_MLOOP))
    {                
        LOCK_TDTV_STATE();
        if((SV_ON==fgTDTVModeChgTrigger)
            ||u4DrvTDTVCheckModeChange())
        {
            vDrvTDTVSysCfg();

            vDrvTDTVUpdate3D22(SV_VP_MAIN);
            vDrvTDTVUpdate3D22(SV_VP_PIP);

            vDrv3DBLCfg();  
            vDrvTDTVDQCCfg();
            vDrvTDTVPQCfg();
            vDrvTDTVTTDCfg();

            
            fgTDTVModeChgTrigger=SV_OFF;            
        }
        UNLOCK_TDTV_STATE();

        vDrvTDTVInvProc();
        vDrvTDTVExtProc();
        vDrvTDTVCropProc();     
        vDrvTTDProc();        
        // Output 3D NAVI status
        
        vDrvTDTVTDNAVIProc();
        #if SUPPORT_3D_NAVI
    	vDrvTdNaviMainloop();
        #endif
        
        #if SUPPORT_REALD_TAG
    	vDrvRealDMainloop(SV_VP_MAIN);
    	vDrvRealDMainloop(SV_VP_PIP);
        #endif
    }    
}
