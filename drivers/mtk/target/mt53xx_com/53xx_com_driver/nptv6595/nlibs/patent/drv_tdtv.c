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

#include "util.h"
#include "video_def.h"
#include "source_select.h"

#include "vdo_misc.h"
#include "drv_video.h"

#include "drv_tdtv_drvif.h"
#include "drv_tdtv.h"
#include "drv_tdtv_def_tbl.h"

#include "drv_scaler_drvif.h"
#include "osd_drvif.h"

#include "srm_drvif.h"

#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_nr.h"
#include "drv_display.h"
#include "drv_dvi.h"

#include "drvcust_if.h"

#include "x_chip_id.h"
#include "panel.h"
#include "ostg_if.h"

#ifdef CC_MT5395
#include "drv_mjc.h"
#endif
#include "x_pinmux.h"

static E_TDTV_DECODER_INPUT eHDMIDetectPack,eMMDetectPack,eSCDetectPack;
static E_TDTV_DECODER_INPUT eSelectPack[NUM_OF_DECOER];

static S_TDTVVdoCfg sVdoCfg[NUM_OF_PATH];
static S_TDTVVdoCfg sPreVdoCfg[NUM_OF_PATH];

static E_TDTV_UI_3D_PACKING eUIPacking;
static E_TDTV_UI_3D_2_2D eUI3DTO2D=E_TDTV_UI_3D_2_2D_OFF;
static E_TDTV_UI_3D_FPR  eUI3DFPR=E_TDTV_UI_3D_FPR_OFF;

static E_TDTV_UI_LR eUILRInverse=E_TDTV_UI_LR_NORMAL;
static E_TDTV_UI_LR eB2RLRInv=E_TDTV_UI_LR_NORMAL;

static E_TDTV_UI_3D_2_2D ePreUI3DTO2D=E_TDTV_UI_3D_2_2D_OFF;

static UINT8 fgIsPackDbg=SV_OFF;
static UINT8 fg3DTO2DDbg=SV_OFF;
static UINT8 fgFPRDbg=SV_OFF;

//Function Declaration
static void vDrvTDTVMMConfig(void);
static void vDrvTDTVHDMIConfig(void);
static void vDrvTDTVOtherSrcConfig(void);

void vDrvTDTVOSDCallback(void *pvArgv);

#ifdef DRV_USE_EXTERNAL_3D_FRC
EXTERN VOID DRVCUST_SetExtFrc3DFormat(E_TDTV_UI_3D_PACKING eUIPackingSel);
EXTERN VOID DRVCUST_SetExtFrc3DDepth(UINT32 u4Depth);
EXTERN VOID DRVCUST_SetExtFrc3DInverse(UINT32 u4IsInverse);
EXTERN VOID DRVCUST_SetExtFrcProtrude(UINT32 u4Protrude);
EXTERN VOID DRVCUST_SetExtFrcDistanceFromTV(UINT32 u4Distant);
EXTERN VOID DRVCUST_SetExtFrcOSDDepth(UINT32 u4OSDDepth);
#endif

#ifdef CC_3DTV_AUDIO_LINK_SUPPORT
extern void AUD_3D_AVLINK_Config(UINT32 u4DepthField);
#endif

#ifdef CC_SCPOS_3DTV_SUPPORT
#if SUPPORT_3D_SG_CTRL
static UINT8 u1LRResetFlag = 1; // LR Reset flip-flop
static UINT8 u13DCurDispR = 2;
static UINT8 u13DPreDispR = 2;
#endif

EXTERN BOOL bInitPWM2;
#endif

//=============================================================================
// Mode Change Callback Function Register
//=============================================================================
vTDTVModeChangeCB TDTVCallback[E_TDTV_NUM_OF_CB]=
{
    NULL,//E_TDTV_CB_MM        =0,
    NULL,//E_TDTV_CB_HDMI      =1,
    NULL,//E_TDTV_CB_NR        =2,
    NULL,//E_TDTV_CB_DI        =3,
    NULL,//E_TDTV_CB_SCALER    =4,
    NULL,//E_TDTV_CB_MJC       =5
    NULL,//E_TDTV_CB_OSD
    NULL,//E_TDTV_CB_OSTG    
};

bTDTVQuery3DCropFuncCB TDTV3DCropCallback[E_TDTV_NUM_OF_CB]=
{
    NULL,//E_TDTV_CB_MM    
    NULL,//E_TDTV_CB_HDMI  
    NULL,//E_TDTV_CB_NR    
    NULL,//E_TDTV_CB_DI    
    NULL,//E_TDTV_CB_SCALER
    NULL,//E_TDTV_CB_MJC   
    NULL,//E_TDTV_CB_OSD
    NULL,//E_TDTV_CB_OSTG  
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
    return pfPreCB;
}

//=============================================================================
// TDTV Misc Utility for state chcking and Enum Tranlate
//=============================================================================
static E_TDTV_UI_3D_PACKING vDrvTDTVValidatePackMode(E_TDTV_UI_3D_PACKING eSelectPack,E_TDTV_DECODER_INPUT eDetectPack)
{
    if(IS_Support3D()!=SV_TRUE)
    {  
        return E_TDTV_UI_3D_MODE_OFF;
    }
    
    if((eDetectPack>=AUTO_ONLY_START)&&(eDetectPack<=AUTO_ONLY_END))
    {
        return E_TDTV_UI_3D_MODE_AUTO;
    }
    
#if SUPPORT_3D_FOR_HD_ONLY
        if((wDrvVideoInputHeight(SV_VP_MAIN)<720)&&(eSelectPack!=E_TDTV_UI_3D_MODE_TTD))
    {
            return E_TDTV_UI_3D_MODE_OFF;//2D to 3D Only
    }
#endif 

    return eSelectPack;
}

static E_TDTV_UI_3D_PACKING vDrvTDTVGetCurCfg()
{    
    UINT32 u4SigType=bGetSignalType(SV_VP_MAIN);
    E_DEC_MOD eDecMod;
        
    if((eUI3DTO2D!=E_TDTV_UI_3D_2_2D_OFF) ||
        (SRM_GetMmMode()==SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        return E_TDTV_UI_3D_MODE_OFF; // 2D Mode
    }
    
    if(SV_ST_MPEG==u4SigType)
    {
        eDecMod=E_DEC_MOD_MM;
        vDrvTDTVMMConfig();
    }
    else if(SV_ST_DVI==u4SigType)
    {
        eDecMod=E_DEC_MOD_HDMI;
        vDrvTDTVHDMIConfig();
    }
    else
    {
        eDecMod=E_DEC_MOD_OTHERS;
        vDrvTDTVOtherSrcConfig();
    } 
    
    switch(eSelectPack[eDecMod])
    {
        case E_TDTV_DECODER_INPUT_AUTO:     
        case E_TDTV_DECODER_INPUT_RESERVE:
        case E_TDTV_DECODER_INPUT_2D_I:     
        case E_TDTV_DECODER_INPUT_2D_P:     
        case E_TDTV_DECODER_INPUT_2D_Q_I:   
        case E_TDTV_DECODER_INPUT_2D_Q_P: 
            return E_TDTV_UI_3D_MODE_OFF;
        case E_TDTV_DECODER_INPUT_TTD_I:    
        case E_TDTV_DECODER_INPUT_TTD_P:
            return E_TDTV_UI_3D_MODE_TTD;
        case E_TDTV_DECODER_INPUT_FS_I:     
        case E_TDTV_DECODER_INPUT_FS_P:     
        case E_TDTV_DECODER_INPUT_FS_Q_I:   
        case E_TDTV_DECODER_INPUT_FS_Q_P:        
        case E_TDTV_DECODER_INPUT_FP_I:     
        case E_TDTV_DECODER_INPUT_FP_P:        
            return E_TDTV_UI_3D_MODE_FS;
        case E_TDTV_DECODER_INPUT_TB_I:     
        case E_TDTV_DECODER_INPUT_TB_P:
            return E_TDTV_UI_3D_MODE_TB;
        case E_TDTV_DECODER_INPUT_SBS_I:    
        case E_TDTV_DECODER_INPUT_SBS_P:
            return E_TDTV_UI_3D_MODE_SBS;        
        case E_TDTV_DECODER_INPUT_RD_I:     
        case E_TDTV_DECODER_INPUT_RD_P: 
            return E_TDTV_UI_3D_MODE_RD;        
        case E_TDTV_DECODER_INPUT_SS_I:     
        case E_TDTV_DECODER_INPUT_SS_P:       
        case E_TDTV_DECODER_INPUT_SBS_Q_I:  
        case E_TDTV_DECODER_INPUT_SBS_Q_P:
            return E_TDTV_UI_3D_MODE_SS;
        case E_TDTV_DECODER_INPUT_LI_I:
        case E_TDTV_DECODER_INPUT_LI_P:
            return E_TDTV_UI_3D_MODE_LI;
    }    
    return E_TDTV_UI_3D_MODE_OFF;
}

static UINT32 u4DrvTDTVIs3DTo2DByScaler(UINT32 u4Path ,E_TDTV_DECODER_INPUT ePackingType)
{
    UINT32 u4SigType=bGetSignalType(SV_VP_MAIN);
    E_DEC_MOD eDecMod;
    
    if(SV_ST_MPEG==u4SigType)
    {
        eDecMod=E_DEC_MOD_MM;
    }
    else if(SV_ST_DVI==u4SigType)
    {
        eDecMod=E_DEC_MOD_HDMI;
    }
    else
    {
        eDecMod=E_DEC_MOD_OTHERS;
    } 
   
    if( (SV_VP_MAIN==u4Path) &&
        ((E_DEC_MOD_HDMI==eDecMod)||(E_DEC_MOD_MM==eDecMod)) &&
        (  !(
            ((ePackingType&~0x1)== (E_TDTV_DECODER_INPUT_SS_I&~0x1))|| // Sensio
            ((ePackingType&~0x1)== (E_TDTV_DECODER_INPUT_LI_I&~0x1)) // Line Interleave
            )  
        )
        )
    {
        return SV_FALSE;
    }
    
    return SV_TRUE;
}


static UINT32 u4DrvTDTVIsILRInv()
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

//=============================================================================
// TDTV Packing Configuration
//=============================================================================

static void vDrvTDTVHDMIConfig()
{
    E_TDTV_UI_3D_PACKING eHDMIPacking=vDrvTDTVValidatePackMode(eUIPacking,eHDMIDetectPack);

    switch(eHDMIPacking)
    {
        case E_TDTV_UI_3D_MODE_OFF:
            eSelectPack[E_DEC_MOD_HDMI]=E_TDTV_DECODER_INPUT_2D_I;
            break;
        case E_TDTV_UI_3D_MODE_AUTO:
            if(eHDMIDetectPack>=NON_2D_PACKING_START)
            {
                eSelectPack[E_DEC_MOD_HDMI]=eHDMIDetectPack;
            }
            else if(eSCDetectPack>=NON_2D_PACKING_START)
            {
                eSelectPack[E_DEC_MOD_HDMI]=eSCDetectPack;
            }
            else
            {
                eSelectPack[E_DEC_MOD_HDMI]=E_TDTV_DECODER_INPUT_2D_I;
            }
            break;
        case E_TDTV_UI_3D_MODE_TTD:
            eSelectPack[E_DEC_MOD_HDMI]=E_TDTV_DECODER_INPUT_TTD_I;
            break;
        case E_TDTV_UI_3D_MODE_FS:
            eSelectPack[E_DEC_MOD_HDMI]=E_TDTV_DECODER_INPUT_FS_I;
            break;            
        case E_TDTV_UI_3D_MODE_TB:
            eSelectPack[E_DEC_MOD_HDMI]=E_TDTV_DECODER_INPUT_TB_I;
            break;
        case E_TDTV_UI_3D_MODE_SBS:
            eSelectPack[E_DEC_MOD_HDMI]=E_TDTV_DECODER_INPUT_SBS_I;
            break;
        case E_TDTV_UI_3D_MODE_RD:
            eSelectPack[E_DEC_MOD_HDMI]=E_TDTV_DECODER_INPUT_RD_I;
            break;              
        case E_TDTV_UI_3D_MODE_SS:
            eSelectPack[E_DEC_MOD_HDMI]=E_TDTV_DECODER_INPUT_SS_I;
            break;             
        case E_TDTV_UI_3D_MODE_LI:
            eSelectPack[E_DEC_MOD_HDMI]=E_TDTV_DECODER_INPUT_LI_I;
            break;             
        default:
            eSelectPack[E_DEC_MOD_HDMI]=E_TDTV_DECODER_INPUT_2D_I;
            break;
    }

    //Setting Interlace , Progessive
    eSelectPack[E_DEC_MOD_HDMI]=(E_TDTV_DECODER_INPUT)((UINT32)eSelectPack[E_DEC_MOD_HDMI] | ((UINT32)eHDMIDetectPack&0x1));

    if((eUI3DTO2D!=E_TDTV_UI_3D_2_2D_OFF)&&(eSelectPack[E_DEC_MOD_HDMI]>=NON_2D_PACKING_START))
    {
        if(u4DrvTDTVIs3DTo2DByScaler(SV_VP_MAIN,eSelectPack[E_DEC_MOD_HDMI]))
        {
            sVdoCfg[SV_VP_MAIN].HDMI =TTD_2ENUM_TO_INT(
                E_TDTV_DECODER_OUTPUT_NATIVE,
                eSelectPack[E_DEC_MOD_HDMI]
                );
        }
        else
        {                
            sVdoCfg[SV_VP_MAIN].HDMI=TTD_2ENUM_TO_INT(
                E_TDTV_DECODER_OUTPUT_3D_2_2D,
                eSelectPack[E_DEC_MOD_HDMI]
                );
        }            
    }
    else
    {
#if SUPPORT_FS_FREERUN
        //Force 2D Content to be FS
        if((eHDMIPacking==E_TDTV_UI_3D_MODE_FS)&&(eHDMIDetectPack<NON_2D_PACKING_START))
        {
            sVdoCfg[SV_VP_MAIN].HDMI=TTD_2ENUM_TO_INT(
                E_TDTV_DECODER_OUTPUT_NATIVE,
                eHDMIDetectPack
                );
        
            bHDMI3DLRFreeRunControl(SV_ON);
            return;
        }
        else
        {        
            bHDMI3DLRFreeRunControl(SV_OFF);
        }
#endif
        sVdoCfg[SV_VP_MAIN].HDMI=TTD_2ENUM_TO_INT(
            TTD_PROC_UNPACK_TBL_DECOUT(u8VdoPathProcCfg[E_DEC_MOD_HDMI][eSelectPack[E_DEC_MOD_HDMI]]),
            TTD_PROC_UNPACK_TBL_DECIN(u8VdoPathProcCfg[E_DEC_MOD_HDMI][eSelectPack[E_DEC_MOD_HDMI]])
            );        
    }    
}

static void vDrvTDTVMMConfig()
{

    E_TDTV_UI_3D_PACKING eMMPacking=vDrvTDTVValidatePackMode(eUIPacking,eMMDetectPack);

    switch(eMMPacking)
    {
        case E_TDTV_UI_3D_MODE_OFF:
            eSelectPack[E_DEC_MOD_MM]=E_TDTV_DECODER_INPUT_2D_I;
            break;
        case E_TDTV_UI_3D_MODE_AUTO:
            if(eMMDetectPack>=NON_2D_PACKING_START)
            {
                eSelectPack[E_DEC_MOD_MM]=eMMDetectPack;
            }
            else
            {
                eSelectPack[E_DEC_MOD_MM]=E_TDTV_DECODER_INPUT_2D_I;
            }
            break;
        case E_TDTV_UI_3D_MODE_TTD:
            eSelectPack[E_DEC_MOD_MM]=E_TDTV_DECODER_INPUT_TTD_I;
            break;
        case E_TDTV_UI_3D_MODE_FS:
            eSelectPack[E_DEC_MOD_MM]=E_TDTV_DECODER_INPUT_FS_I;
            break;            
        case E_TDTV_UI_3D_MODE_TB:
            eSelectPack[E_DEC_MOD_MM]=E_TDTV_DECODER_INPUT_TB_I;
            break;
        case E_TDTV_UI_3D_MODE_SBS:
            eSelectPack[E_DEC_MOD_MM]=E_TDTV_DECODER_INPUT_SBS_I;
            break;
        case E_TDTV_UI_3D_MODE_RD:
            eSelectPack[E_DEC_MOD_MM]=E_TDTV_DECODER_INPUT_RD_I;
            break;              
        case E_TDTV_UI_3D_MODE_SS:
            eSelectPack[E_DEC_MOD_MM]=E_TDTV_DECODER_INPUT_SS_I;
            break;               
        case E_TDTV_UI_3D_MODE_LI:
            eSelectPack[E_DEC_MOD_MM]=E_TDTV_DECODER_INPUT_LI_I;
            break;             
        default:
            eSelectPack[E_DEC_MOD_MM]=E_TDTV_DECODER_INPUT_2D_I;
            break;
    }

    //Setting Interlace , Progessive
    eSelectPack[E_DEC_MOD_MM]=(E_TDTV_DECODER_INPUT)((UINT32)eSelectPack[E_DEC_MOD_MM] | ((UINT32)eMMDetectPack&0x1));

    if((eUI3DTO2D!=E_TDTV_UI_3D_2_2D_OFF)&&(eSelectPack[E_DEC_MOD_MM]>=NON_2D_PACKING_START))
    {
        if(u4DrvTDTVIs3DTo2DByScaler(SV_VP_MAIN,eSelectPack[E_DEC_MOD_MM]))        
        {
            sVdoCfg[SV_VP_MAIN].B2R =TTD_2ENUM_TO_INT(
                E_TDTV_DECODER_OUTPUT_NATIVE,
                eSelectPack[E_DEC_MOD_MM]
                );
        }
        else
        {                
            sVdoCfg[SV_VP_MAIN].B2R=TTD_2ENUM_TO_INT(
                E_TDTV_DECODER_OUTPUT_3D_2_2D,
                eSelectPack[E_DEC_MOD_MM]
                );
        }    
    }
    else
    {
        sVdoCfg[SV_VP_MAIN].B2R=TTD_2ENUM_TO_INT(
            TTD_PROC_UNPACK_TBL_DECOUT(u8VdoPathProcCfg[E_DEC_MOD_MM][eSelectPack[E_DEC_MOD_MM]]),
            TTD_PROC_UNPACK_TBL_DECIN(u8VdoPathProcCfg[E_DEC_MOD_MM][eSelectPack[E_DEC_MOD_MM]])
            );
    }    
}

static void vDrvTDTVOtherSrcConfig()
{
    switch(eUIPacking)
    {

        case E_TDTV_UI_3D_MODE_TTD:
            eSelectPack[E_DEC_MOD_OTHERS]=E_TDTV_DECODER_INPUT_TTD_I;
            break;
        case E_TDTV_UI_3D_MODE_FS:
            eSelectPack[E_DEC_MOD_OTHERS]=E_TDTV_DECODER_INPUT_FS_I;
            break;            
        case E_TDTV_UI_3D_MODE_TB:
            eSelectPack[E_DEC_MOD_OTHERS]=E_TDTV_DECODER_INPUT_TB_I;
            break;
        case E_TDTV_UI_3D_MODE_SBS:
            eSelectPack[E_DEC_MOD_OTHERS]=E_TDTV_DECODER_INPUT_SBS_I;
            break;
        case E_TDTV_UI_3D_MODE_RD:
            eSelectPack[E_DEC_MOD_OTHERS]=E_TDTV_DECODER_INPUT_RD_I;
            break;              
        case E_TDTV_UI_3D_MODE_SS:
            eSelectPack[E_DEC_MOD_OTHERS]=E_TDTV_DECODER_INPUT_SS_I;
            break;               
        case E_TDTV_UI_3D_MODE_LI:
            eSelectPack[E_DEC_MOD_OTHERS]=E_TDTV_DECODER_INPUT_LI_I;
            break;               
        default:
            eSelectPack[E_DEC_MOD_OTHERS]=E_TDTV_DECODER_INPUT_2D_I;
            break;
    }

    if(bDrvVideoIsSrcInterlace(SV_VP_MAIN))
    { 
        eSelectPack[E_DEC_MOD_OTHERS]=eSelectPack[E_DEC_MOD_OTHERS];
    }
    else
    {
        eSelectPack[E_DEC_MOD_OTHERS]=(E_TDTV_DECODER_INPUT)((UINT32)eSelectPack[E_DEC_MOD_OTHERS] | 0x1);    
    }
}


static void vDrvTDTVNRConfig()
{    
    UINT32 u4SigType=bGetSignalType(SV_VP_MAIN);
    E_DEC_MOD eDecMod;
    
    if(SV_ST_MPEG==u4SigType)
    {
        eDecMod=E_DEC_MOD_MM;
    }
    else if(SV_ST_DVI==u4SigType)
    {
        eDecMod=E_DEC_MOD_HDMI;
    }
    else
    {
        eDecMod=E_DEC_MOD_OTHERS;
    } 

    if(eUI3DTO2D)
    {
        if(bDrvVideoIsSrcInterlace(SV_VP_MAIN))
        {        
            sVdoCfg[SV_VP_MAIN].NR= E_TDTV_NR_INTERLEAVE;
        }
        else
        {
            sVdoCfg[SV_VP_MAIN].NR= E_TDTV_NR_PROGRESSIVE;    
        }
    }
    else
    {
        sVdoCfg[SV_VP_MAIN].NR= 
            (UINT32)TTD_PROC_UNPACK_TBL_NR(u8VdoPathProcCfg[eDecMod][eSelectPack[eDecMod]]);
    }
}

static void vDrvTDTVDIConfig()
{   

    UINT32 u4SigType=bGetSignalType(SV_VP_MAIN);
    E_DEC_MOD eDecMod;
    
    if(SV_ST_MPEG==u4SigType)
    {
        eDecMod=E_DEC_MOD_MM;
    }
    else if(SV_ST_DVI==u4SigType)
    {
        eDecMod=E_DEC_MOD_HDMI;
    }
    else
    {
        eDecMod=E_DEC_MOD_OTHERS;
    } 

    if(eUI3DTO2D)
    {
        //For Scaler Sensio Interlace 3D to 2D WA
        if(sVdoCfg[SV_VP_MAIN].SC==TTD_2ENUM_TO_INT(
                    E_TDTV_DECODER_OUTPUT_3D_2_2D,
                    E_TDTV_DECODER_INPUT_SBS_Q_I)
                    )
        {               
            sVdoCfg[SV_VP_MAIN].DI= E_TDTV_DI_BYPASS;
        }    
        else
        {
        sVdoCfg[SV_VP_MAIN].DI= E_TDTV_DI_2D;
    }
    }
    else
    {
        sVdoCfg[SV_VP_MAIN].DI= 
            (UINT32)TTD_PROC_UNPACK_TBL_DI(u8VdoPathProcCfg[eDecMod][eSelectPack[eDecMod]]);

        if(sVdoCfg[SV_VP_MAIN].DI==E_TDTV_DI_BY_TIMMING)
        {
            if(IS_LVDS_DISP_3D_SHUTTER)
            {
                //Normal 2D Mode (3D Deinterlace under SD timing , 2D Deinterlace under HD Timing)
                sVdoCfg[SV_VP_MAIN].DI= (wDrvVideoInputHeight(SV_VP_MAIN) <=576) ? E_TDTV_DI_2D : E_TDTV_DI_3D_I;
            }
            else
            {
                sVdoCfg[SV_VP_MAIN].DI= E_TDTV_DI_2D;
            }
        }
    }
}

static void vDrvTDTVSCConfig(UINT32 u4Path)
{
    E_TDTV_DECODER_OUTPUT eSCOut;

#ifdef CC_3DTV_FPR_SUPPORT                
    UINT32 u4Timing;
#endif
    
    UINT32 u4SigType=bGetSignalType(u4Path);
    E_DEC_MOD eDecMod;
    
    if(SV_ST_MPEG==u4SigType)
    {
        eDecMod=E_DEC_MOD_MM;
    }
    else if(SV_ST_DVI==u4SigType)
    {
        eDecMod=E_DEC_MOD_HDMI;
    }
    else
    {
        eDecMod=E_DEC_MOD_OTHERS;
    } 

    if(SV_VP_MAIN==u4Path)
    {  
        if(eUI3DTO2D)
        {           
            if(u4DrvTDTVIs3DTo2DByScaler(SV_VP_MAIN,eSelectPack[E_DEC_MOD_MM]))        
            {            
                switch(eSelectPack[E_DEC_MOD_OTHERS])
                {
                    case E_TDTV_DECODER_INPUT_FS_I:                
                    case E_TDTV_DECODER_INPUT_FS_P:
                        sVdoCfg[SV_VP_MAIN].SC =TTD_2ENUM_TO_INT(
                            E_TDTV_DECODER_OUTPUT_3D_2_2D,
                            E_TDTV_DECODER_INPUT_FS_P
                            );
                        break;
                    case E_TDTV_DECODER_INPUT_TB_I:                        
                    case E_TDTV_DECODER_INPUT_TB_P:
                        sVdoCfg[SV_VP_MAIN].SC =TTD_2ENUM_TO_INT(
                            E_TDTV_DECODER_OUTPUT_3D_2_2D,
                            E_TDTV_DECODER_INPUT_TB_P
                            );
                        break;           
                    case E_TDTV_DECODER_INPUT_SBS_I:                    
                    case E_TDTV_DECODER_INPUT_SBS_P:   
                    case E_TDTV_DECODER_INPUT_RD_P:
                    case E_TDTV_DECODER_INPUT_RD_I:                    
                        sVdoCfg[SV_VP_MAIN].SC =TTD_2ENUM_TO_INT(
                            E_TDTV_DECODER_OUTPUT_3D_2_2D,
                            E_TDTV_DECODER_INPUT_SBS_P
                            );
                        break;             
                    case E_TDTV_DECODER_INPUT_SS_I:    
                    case E_TDTV_DECODER_INPUT_SBS_Q_I:
                        sVdoCfg[SV_VP_MAIN].SC =TTD_2ENUM_TO_INT(
                            E_TDTV_DECODER_OUTPUT_3D_2_2D,
                            E_TDTV_DECODER_INPUT_SBS_Q_I
                            );
                        break;  
                    case E_TDTV_DECODER_INPUT_SS_P:                
                    case E_TDTV_DECODER_INPUT_SBS_Q_P:
                        sVdoCfg[SV_VP_MAIN].SC =TTD_2ENUM_TO_INT(
                            E_TDTV_DECODER_OUTPUT_3D_2_2D,
                            E_TDTV_DECODER_INPUT_SBS_Q_P
                            );
                        break;                         
                    case E_TDTV_DECODER_INPUT_LI_I:                        
                    case E_TDTV_DECODER_INPUT_LI_P:
                        sVdoCfg[SV_VP_MAIN].SC =TTD_2ENUM_TO_INT(
                            E_TDTV_DECODER_OUTPUT_3D_2_2D,
                            E_TDTV_DECODER_INPUT_LI_P
                            );
                        break;           
                    default:
                        sVdoCfg[SV_VP_MAIN].SC= TTD_2ENUM_TO_INT(
                            E_TDTV_DECODER_OUTPUT_NATIVE,
                            E_TDTV_DECODER_INPUT_2D_P
                            );            
                        break;   
                }
            }
            else
            {                          
                sVdoCfg[SV_VP_MAIN].SC= TTD_2ENUM_TO_INT(E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_DECODER_INPUT_2D_P);
            }
        }
        else
        {
            //Config For MM     
            eSCOut=TTD_PROC_UNPACK_TBL_SCOUT(u8VdoPathProcCfg[eDecMod][eSelectPack[eDecMod]]);
#ifdef CC_3DTV_FPR_SUPPORT            
            u4Timing=bDrvVideoGetTiming(SV_VP_MAIN);
            if(IS_LVDS_DISP_3D_POLARIZED)
            {
                if((u4Timing==MODE_3D_1080p_24_FP)&&
                    (TTD_PROC_UNPACK_TBL_SCIN(u8VdoPathProcCfg[eDecMod][eSelectPack[eDecMod]])==E_TDTV_DECODER_INPUT_FS_P))
                {
                    if(eUI3DFPR==E_TDTV_UI_3D_FPR_V1)
                    {
                        eSCOut=E_TDTV_DECODER_OUTPUT_3D_FPR_V1;
                    }
                    else if(eUI3DFPR==E_TDTV_UI_3D_FPR_V2)
                    {
                        eSCOut=E_TDTV_DECODER_OUTPUT_3D_FPR_V2;
                    }
                }
            }
#endif            
            sVdoCfg[SV_VP_MAIN].SC=TTD_2ENUM_TO_INT(
                eSCOut,
                TTD_PROC_UNPACK_TBL_SCIN(u8VdoPathProcCfg[eDecMod][eSelectPack[eDecMod]]));            
        }
    }
    else //SV_VP_PIP
    {
        if(E_TDTV_UI_3D_2_2D_OFF==eUI3DTO2D)
        {
            switch(eSelectPack[eDecMod])
            {
                case E_TDTV_DECODER_INPUT_FS_P:
                case E_TDTV_DECODER_INPUT_FS_Q_P:                        
                    sVdoCfg[SV_VP_PIP].SC =TTD_2ENUM_TO_INT(
                        E_TDTV_DECODER_OUTPUT_3D_2_2D,
                        E_TDTV_DECODER_INPUT_FS_P
                        );
                    break;
                case E_TDTV_DECODER_INPUT_TB_P:
                    sVdoCfg[SV_VP_PIP].SC =TTD_2ENUM_TO_INT(
                        E_TDTV_DECODER_OUTPUT_3D_2_2D,
                        E_TDTV_DECODER_INPUT_TB_P
                        );
                    break;            
                case E_TDTV_DECODER_INPUT_SBS_P:   
                case E_TDTV_DECODER_INPUT_RD_P:
                    sVdoCfg[SV_VP_PIP].SC =TTD_2ENUM_TO_INT(
                        E_TDTV_DECODER_OUTPUT_3D_2_2D,
                        E_TDTV_DECODER_INPUT_SBS_P
                        );
                    break;             
                case E_TDTV_DECODER_INPUT_SS_I:    
                case E_TDTV_DECODER_INPUT_SBS_Q_I:
                    sVdoCfg[SV_VP_PIP].SC =TTD_2ENUM_TO_INT(
                        E_TDTV_DECODER_OUTPUT_3D_2_2D,
                        E_TDTV_DECODER_INPUT_SS_I
                        );
                    break;  
                case E_TDTV_DECODER_INPUT_SS_P:                
                case E_TDTV_DECODER_INPUT_SBS_Q_P:
                    sVdoCfg[SV_VP_PIP].SC =TTD_2ENUM_TO_INT(
                        E_TDTV_DECODER_OUTPUT_3D_2_2D,
                        E_TDTV_DECODER_INPUT_SBS_Q_P
                        );
                    break;                         
                case E_TDTV_DECODER_INPUT_LI_P:
                    sVdoCfg[SV_VP_PIP].SC =TTD_2ENUM_TO_INT(
                        E_TDTV_DECODER_OUTPUT_3D_2_2D,
                        E_TDTV_DECODER_INPUT_LI_P
                        );
                    break;            
                default:
                    sVdoCfg[SV_VP_PIP].SC= TTD_2ENUM_TO_INT(
                        E_TDTV_DECODER_OUTPUT_NATIVE,
                        E_TDTV_DECODER_INPUT_2D_P
                        );            
                    break;   
            }
        }
        else
        {
            sVdoCfg[SV_VP_PIP].SC= TTD_2ENUM_TO_INT(
                E_TDTV_DECODER_OUTPUT_NATIVE,
                E_TDTV_DECODER_INPUT_2D_P
                ); 
        }
    }    
}

static void vDrvTDTVMJCConfig()
{   
    UINT32 u4SigType=bGetSignalType(SV_VP_MAIN);
    E_DEC_MOD eDecMod;
   
    if(SV_ST_MPEG==u4SigType)
    {
        eDecMod=E_DEC_MOD_MM;        
    }
    else if(SV_ST_DVI==u4SigType)
    {
        eDecMod=E_DEC_MOD_HDMI;
    }
    else
    {
        eDecMod=E_DEC_MOD_OTHERS;
    } 


    if(eUI3DTO2D)
    {
        sVdoCfg[SV_VP_MAIN].MJC= E_TDTV_MJC_2D;
    }
    else
    {
        sVdoCfg[SV_VP_MAIN].MJC= (( u4DrvTDTVIsILRInv() ? E_TDTV_MJC_LR_INVERSE : E_TDTV_MJC_LR_NORMAL)<<16)|
            (UINT32)TTD_PROC_UNPACK_TBL_MJC(u8VdoPathProcCfg[eDecMod][eSelectPack[eDecMod]]);
    }
}

static void vDrvTDTVOSDConfig()
{
#ifdef DRV_USE_EXTERNAL_3D_FRC

    S_TDTV_OSTG_CONFIG sOstgCfg=eDrvTDTVOSTGModeQuery();
    E_TDTV_UI_3D_PACKING eTDTVMode = vDrvTDTVGetCurCfg();   
    
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
    UINT32 u4Width=PANEL_GetPanelWidth();
    UINT32 u4Height=PANEL_GetPanelHeight();

    sVdoCfg[SV_VP_MAIN].OSD.eOSDType=E_TDTV_OSD_2D;
    sVdoCfg[SV_VP_MAIN].OSD.u4PanelWidth=u4Width;
    sVdoCfg[SV_VP_MAIN].OSD.u4PanelHeight=u4Height;  
#endif
}

/*
* @brief :Mode Query for OSTG
* @retval:  S_TDTV_OSD_CONFIG
*/

static void vDrvTDTVOSTGConfig()
{
#ifdef DRV_USE_EXTERNAL_3D_FRC       
        E_TDTV_UI_3D_PACKING eTDTVMode = vDrvTDTVGetCurCfg();   
        UINT32 u4FrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
        UINT32 u4Width = wDrvVideoInputWidth(SV_VP_MAIN);
        UINT32 u4Height = wDrvVideoInputHeight(SV_VP_MAIN);
        UINT32 u4IsInterlace = bDrvVideoIsSrcInterlace(SV_VP_MAIN);
        UINT32 u4VTotal;
        UINT32 u4Clk;
        
        if((u4FrameRate==0)
            || (bDrvVideoSignalStatus(SV_VP_MAIN)!=SV_VDO_STABLE)
            || (SRM_GetMmMode()==SRM_MM_MODE_TYPE_THUMBNAIL))
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
                u4VTotal    = (u4Width <= 1280) ?  825 : 1125;
                u4Height    = (u4Width <= 1280) ?  720 : 1080;
                u4Width     = (u4Width <= 1280) ? 1280 : ((u4IsInterlace) ? 960 : 1920);
                u4Clk       = (u4VTotal == 825) ? PANEL_CLK_L : PANEL_CLK_H;
                u4FrameRate = (u4IsInterlace) ? (u4FrameRate << 1) : u4FrameRate;
                
               break;  
            case E_TDTV_UI_3D_MODE_LI:
                switch(u4FrameRate)
                {
                    case 25:
                    case 50:
                        u4FrameRate = 100;
                        break;
                    case 24:
                    case 30:
                    case 60:
                    default:
                        u4FrameRate = 120 ;
                        break;
                }
                u4VTotal = 625;
                u4Height = 540;
                u4Width  = 1920;
                u4Clk    = PANEL_CLK_H;
                
               break;  
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
        sVdoCfg[SV_VP_MAIN].OSTG.u4VTotal=u4VTotal-1;  // for panel table              
        sVdoCfg[SV_VP_MAIN].OSTG.u4HTotal=(u4Clk/u4FrameRate/u4VTotal)-1; // for panel table

        //Current FRC only Support Even HTotal
        sVdoCfg[SV_VP_MAIN].OSTG.u4HTotal |=1;
       
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


/**
 * @brief :3D Processing Mode Query
 * @param eDetectedMode: Set The Decoder detected Mode According to HDMI Flag
 * @retval:  (E_TDTV_HDMI_OUTPUT << 16) || E_TDTV_HDMI_INPUT
 */
UINT32 u4DrvTDTVHDMIModeQuery(E_TDTV_DECODER_INPUT eDetectedMode)
{
    //UINT32 u4Tmp=sVdoCfg[SV_VP_MAIN].HDMI;
    E_TDTV_DECODER_INPUT ePreDetectedMode;

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

    ePreDetectedMode = eHDMIDetectPack;    
    eHDMIDetectPack = eDetectedMode;

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
        vDrvTDTVHDMIConfig();
        
        if(SV_ST_DVI==bGetSignalType(SV_VP_MAIN))
        {
            //callback for 3D Display
            TTD_MODE_CHANGE_CB(E_TDTV_CB_NR);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_DI);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_SCALER);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_MJC);
        }
        else if(SV_ST_DVI==bGetSignalType(SV_VP_PIP))
        {
            //Callback for Scart Out
            TTD_MODE_CHANGE_CB(E_TDTV_CB_NR);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_DI);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_SCALER);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_MJC);
        }
    }

    return sVdoCfg[SV_VP_MAIN].HDMI;    
}


/**
 * @brief :3D Processing Mode Query
 * @param eDetectedMode: Set the Decoder Detected mode
 * @retval:  (E_TDTV_MM_OUTPUT << 16) || E_TDTV_MM_INPUT
 */
UINT32 u4DrvTDTVMMModeQuery(E_TDTV_DECODER_INPUT eDetectedMode)
{   
    E_TDTV_DECODER_INPUT ePreDetectedMode;

    //Translate B2R Inverse
    eB2RLRInv=E_TDTV_UI_LR_INVERSE;
    switch(eDetectedMode)
    {
        case E_TDTV_DECODER_INPUT_TB_INV_I:
            eDetectedMode=E_TDTV_DECODER_INPUT_TB_I;
            break;
        case E_TDTV_DECODER_INPUT_TB_INV_P: 
            eDetectedMode=E_TDTV_DECODER_INPUT_TB_P;
            break;        
        case E_TDTV_DECODER_INPUT_SBS_INV_I:
            eDetectedMode=E_TDTV_DECODER_INPUT_SBS_I;
            break;        
        case E_TDTV_DECODER_INPUT_SBS_INV_P:
            eDetectedMode=E_TDTV_DECODER_INPUT_SBS_I;
            break;
        default:
            //For case beside INV , All should not be inversed.
            eB2RLRInv=E_TDTV_UI_LR_NORMAL;
    }

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
    
    ePreDetectedMode = eMMDetectPack;  
    eMMDetectPack = eDetectedMode;        
    //Force Auto mode when detected format change
    if((ePreDetectedMode&(~0x1)) != (eDetectedMode&(~0x1)))
    {
        if((SV_ST_MPEG==bGetSignalType(SV_VP_MAIN))        
            &&(IS_Support3D()==SV_TRUE))
        {
            eUIPacking = E_TDTV_UI_3D_MODE_AUTO;
        }
    }

    if(ePreDetectedMode != eDetectedMode)
    {
        vDrvTDTVMMConfig();

        if(SV_ST_MPEG==bGetSignalType(SV_VP_MAIN))
        {
           //callback for 3D Display
            TTD_MODE_CHANGE_CB(E_TDTV_CB_NR);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_DI);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_SCALER);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_MJC);
        }
        else if(SV_ST_MPEG==bGetSignalType(SV_VP_PIP))
        { 
            //Callback for Scart Out
            TTD_MODE_CHANGE_CB(E_TDTV_CB_NR);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_DI);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_SCALER);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_MJC);
        }    
    }
    return sVdoCfg[SV_VP_MAIN].B2R;
}

UINT32 u4DrvTDTVNRModeQuery()
{
    vDrvTDTVNRConfig(); 
    return sVdoCfg[SV_VP_MAIN].NR;
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
UINT32 u4DrvTDTVScalerModeQueryEx(UINT32 u4Path ,E_TDTV_DECODER_INPUT eDetectedMode)
{
    eSCDetectPack = eDetectedMode;
    vDrvTDTVSCConfig(u4Path);
    return sVdoCfg[u4Path].SC;        
}


/**
 * @brief :3D Processing Mode Query for Scaler
 * @param eDetectedMode: For HDMI Input Source, Scaler Detect RealD Tag, 
 * @retval:  (E_TDTV_MM_OUTPUT << 16) || E_TDTV_MM_INPUT
 */
UINT32 u4DrvTDTVScalerModeQuery(E_TDTV_DECODER_INPUT eDetectedMode)
{
    return u4DrvTDTVScalerModeQueryEx(SV_VP_MAIN,eDetectedMode);
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
UINT8 u1DrvTDTVMJCEnableQuery()
{
#ifdef DRV_USE_EXTERNAL_3D_FRC    

    //For 8283, 3D MEMC Support
    return SV_TRUE;
#else
    //MT5395 only support 2D MEMC
    if((sVdoCfg[SV_VP_MAIN].MJC == E_TDTV_MJC_2D) || (eUI3DTO2D != E_TDTV_UI_3D_2_2D_OFF))
    {
        return SV_TRUE;
    }
	else
	{
        return SV_FALSE;
	}
#endif	
}


/*
* @brief :Mode Query for OSD
* @retval:  S_TDTV_OSD_CONFIG
*/

S_TDTV_OSD_CONFIG eDrvTDTVOSDModeQuery()
{
    vDrvTDTVOSDConfig();
    return sVdoCfg[SV_VP_MAIN].OSD;
}

/*
* @brief :Mode Query for OSTG
* @retval:  S_TDTV_OSD_CONFIG
*/
S_TDTV_OSTG_CONFIG eDrvTDTVOSTGModeQuery()
{
    vDrvTDTVOSTGConfig();
    return sVdoCfg[SV_VP_MAIN].OSTG;
}

/**
 * @brief : UI Mode Query for General Model
 * @retval:  E_TDTV_UI_3D_PACKING
 */
E_TDTV_UI_3D_PACKING u4DrvTDTV3DModeQuery()
{
    return vDrvTDTVGetCurCfg();
}

/**
 * @brief : Query Function for SRM check force Scaler Dram Mode Extension
 * @Param u4Path: SV_VP_MAIN/ SV_VP_PIP
 * @retval: SV_TRUE => Force Scaler Dram Mode
 */
UINT32 u4DrvTDTVForceDramModeQueryEx(UINT32 u4Path)
{
#ifdef  CC_SCPOS_3DTV_SUPPORT

    UINT32 u4SigType=bGetSignalType(u4Path);
    E_DEC_MOD eDecMod;
    
    if(SV_ST_MPEG==u4SigType)
    {
        eDecMod=E_DEC_MOD_MM;
    }
    else if(SV_ST_DVI==u4SigType)
    {
        eDecMod=E_DEC_MOD_HDMI;
    }
    else
    {
        eDecMod=E_DEC_MOD_OTHERS;
    } 

    if(SV_VP_MAIN==u4Path)
    {   
        //Update Module Info before FBM Callback , 
        if(E_DEC_MOD_OTHERS==eDecMod)
        {
            TTD_MODE_CHANGE_CB(E_TDTV_CB_NR);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_DI);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_SCALER);
            TTD_MODE_CHANGE_CB(E_TDTV_CB_MJC);        
        }
   
        if(u4DrvTDTV3DModeQuery()!=E_TDTV_UI_3D_MODE_OFF)
        {//If Current is 3D MODE
            #if defined(DRV_USE_EXTERNAL_3D_FRC)

            if(TTD_PROC_UNPACK_TBL_SCIN(u8VdoPathProcCfg[eDecMod][eSelectPack[eDecMod]])
                ==E_TDTV_DECODER_INPUT_FS_P)
            {
                return E_TDTV_DRAM_6FB;
            }
            else if (TTD_PROC_UNPACK_TBL_SCIN(u8VdoPathProcCfg[eDecMod][eSelectPack[eDecMod]])
                ==E_TDTV_DECODER_INPUT_LI_P)
            {
                return E_TDTV_DRAM_3FB;
            }
            else
            {
                return E_TDTV_DRAM_NORMAL;
            } 
        
            #elif (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))

            return E_TDTV_DRAM_6FB;

            #else
   
            if(TTD_PROC_UNPACK_TBL_SCIN(u8VdoPathProcCfg[eDecMod][eSelectPack[eDecMod]])
                ==E_TDTV_DECODER_INPUT_FS_P)
            {
                return E_TDTV_DRAM_6FB;
            }
            else
            {
                return E_TDTV_DRAM_3FB;
            }                
            #endif
        }
        else
        {//If Current is 2D mode        
                switch(eSelectPack[eDecMod])
                {
                    case E_TDTV_DECODER_INPUT_FS_I:
                    case E_TDTV_DECODER_INPUT_FS_P:
                    case E_TDTV_DECODER_INPUT_FP_I:
                    case E_TDTV_DECODER_INPUT_FP_P:
                    case E_TDTV_DECODER_INPUT_SS_I :
                    case E_TDTV_DECODER_INPUT_SS_P :                  
                    case E_TDTV_DECODER_INPUT_2D_Q_I :
                    case E_TDTV_DECODER_INPUT_2D_Q_P :
                    case E_TDTV_DECODER_INPUT_SBS_Q_I:
                    case E_TDTV_DECODER_INPUT_SBS_Q_P:                    
                    case E_TDTV_DECODER_INPUT_LI_I :
                    case E_TDTV_DECODER_INPUT_LI_P :                  
                        return E_TDTV_DRAM_3FB;//Frame Packing I PSCAN Display Mode Not Support
                    default:
                        return E_TDTV_DRAM_NORMAL;            
                }
            }
    }
    else
    {// Scart Out     
        if((eSelectPack[eDecMod]>NON_2D_PACKING_START)&&(eUI3DTO2D==E_TDTV_UI_3D_2_2D_OFF))
        {            
            switch(eSelectPack[eDecMod])
            {
                case E_TDTV_DECODER_INPUT_FS_I:
                case E_TDTV_DECODER_INPUT_FS_P: 
                case E_TDTV_DECODER_INPUT_FP_I:  
                case E_TDTV_DECODER_INPUT_FP_P:
                    return E_TDTV_DRAM_6FB;
                default:
                    return E_TDTV_DRAM_3FB;
            }
        }
        else
        {
            return E_TDTV_DRAM_NORMAL;
        }
    }
    
#else

    return E_TDTV_DRAM_NORMAL;
    
#endif
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
    if(SV_VP_MAIN==u4Path)
    {
        UINT32 u4DIMode = u4DrvTDTVDIModeQuery();
        if((u4DIMode==E_TDTV_DI_2D)||(u4DIMode==E_TDTV_DI_3D_P))
        {
            return E_TDTV_DI_DRAM_NORMAL;
        }
        else
        {
            return E_TDTV_DI_DRAM_0FB;
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

//=============================================================================
// TDTV  DBG Interface 
//=============================================================================
void vDrvTDTVPackingDbg(E_TDTV_UI_3D_PACKING eTDTVUI3DMode)
{    
    if(IS_Support3D()==SV_TRUE)
    {
        if(eTDTVUI3DMode==E_TDTV_UI_3D_MODE_END)
        {
            fgIsPackDbg=SV_FALSE;
            return;
        }
        fgIsPackDbg=SV_TRUE;
        eUIPacking=eTDTVUI3DMode;
    }
}

void vDrvTDTV3D22Ddbg(E_TDTV_UI_3D_2_2D e3D22DMode)
{
    if(IS_Support3D()==SV_TRUE)
    {
        fg3DTO2DDbg=SV_TRUE;
        eUI3DTO2D=e3D22DMode;
        return;
    }
}

void vDrvTDTV3DFPRdbg(E_TDTV_UI_3D_FPR e3DFPRMode)
{   
    if(IS_Support3D()==SV_TRUE)
    {
        fgFPRDbg=SV_TRUE;
        eUI3DFPR=e3DFPRMode;
        return;
    }
}

static const char* szDecInName[]=
{
    "E_TDTV_DECODER_INPUT_AUTO   ",
    "E_TDTV_DECODER_INPUT_RESERVE",
    "E_TDTV_DECODER_INPUT_2D_I   ",
    "E_TDTV_DECODER_INPUT_2D_P   ",
    "E_TDTV_DECODER_INPUT_TTD_I  ",
    "E_TDTV_DECODER_INPUT_TTD_P  ",
    "E_TDTV_DECODER_INPUT_FS_I   ",
    "E_TDTV_DECODER_INPUT_FS_P   ",
    "E_TDTV_DECODER_INPUT_FP_I   ",
    "E_TDTV_DECODER_INPUT_FP_P   ",
    "E_TDTV_DECODER_INPUT_TB_I   ",
    "E_TDTV_DECODER_INPUT_TB_P   ",
    "E_TDTV_DECODER_INPUT_SBS_I  ",
    "E_TDTV_DECODER_INPUT_SBS_P  ",
    "E_TDTV_DECODER_INPUT_RD_I   ",
    "E_TDTV_DECODER_INPUT_RD_P   ",
    "E_TDTV_DECODER_INPUT_SS_I   ",
    "E_TDTV_DECODER_INPUT_SS_P   ",
    "E_TDTV_DECODER_INPUT_2D_Q_I ",
    "E_TDTV_DECODER_INPUT_2D_Q_P ",
    "E_TDTV_DECODER_INPUT_SBS_Q_I",
    "E_TDTV_DECODER_INPUT_SBS_Q_P",
    "E_TDTV_DECODER_INPUT_FS_Q_I ",
    "E_TDTV_DECODER_INPUT_FS_Q_P ",
    "E_TDTV_DECODER_INPUT_LI_I",
    "E_TDTV_DECODER_INPUT_LI_P",
    "E_TDTV_DECODER_INPUT_END"   
};

static const char* szDecOutName[]=
{
    "E_TDTV_DECODER_OUTPUT_NATIVE   ",
    "E_TDTV_DECODER_OUTPUT_3D_FS    ",
    "E_TDTV_DECODER_OUTPUT_3D_2_2D  ",
    "E_TDTV_DECODER_OUTPUT_3D_FPR_V1",
    "E_TDTV_DECODER_OUTPUT_3D_FPR_V2",
    "E_TDTV_DECODER_OUTPUT_3D_REPEAT",
    "E_TDTV_DECODER_OUTPUT_3D_PREDOWN",
    "E_TDTV_DECODER_OUTPUT_3D_60_PR  ",
    "E_TDTV_DECODER_OUTPUT_END"
};


static const char* szNRName[]=
{
    "E_TDTV_NR_PROGRESSIVE ",
    "E_TDTV_NR_INTERLEAVE  ",
    "E_TDTV_NR_SYNC_PADDING"
};

static const char* szDIName[]=
{
    "E_TDTV_DI_2D  ",
    "E_TDTV_DI_3D_I",
    "E_TDTV_DI_3D_Q",
    "E_TDTV_DI_3D_P",
    "E_TDTV_DI_BYPASS",
    "E_TDTV_DI_3D_FPI",
    "E_TDTV_DI_BY_TIMMING"
};

static const char* szMJCName[]=
{
    "E_TDTV_MJC_2D ",
    "E_TDTV_MJC_3D ",
    "E_TDTV_MJC_TTD",
    "E_TDTV_MJC_BYPASS"
};

void vDrvTDTVQueryConfig()
{   

    E_TDTV_DECODER_INPUT eDecInput;
    E_TDTV_DECODER_OUTPUT eDecOutput;
    UINT32 u4Path;
    UINT32 u4CfgTmp;
    
    printf("\n===================================================\n");        
    printf("Is Support 3d:%d\n",IS_Support3D());   
    printf("CB registration\n[  MM]0x%08x\n[HDMI]0x%08x\n[  NR]0x%08x\n[  DI]0x%08x\n[  SC]0x%08x\n[ MJC]0x%08x\n",
        (UINT32 *)TDTVCallback[0],
        (UINT32 *)TDTVCallback[1],
        (UINT32 *)TDTVCallback[2],
        (UINT32 *)TDTVCallback[3],
        (UINT32 *)TDTVCallback[4],
        (UINT32 *)TDTVCallback[5]
        );

    #define DEC_IN_NAME(x) szDecInName[x < E_TDTV_DECODER_INPUT_END ? x: E_TDTV_DECODER_INPUT_END]
    #define DEC_OUT_NAME(x) szDecOutName[x < E_TDTV_DECODER_OUTPUT_END ? x: E_TDTV_DECODER_OUTPUT_END]
    
    for(u4Path=0;u4Path<=SV_VP_PIP;++u4Path)
    {    
        printf("\n===================================================\n");        
        printf("===================== VDP:%d========================\n",u4Path);   

        printf("[   SRC] %d \n",(UINT32) bGetSignalType(u4Path));
        printf("[ 3DTAG] %d \n",(UINT32)u4DrvTDTVGetTag3D(u4Path));

        printf("[    UI] Set as [MODE]%d [322]%d [FPR]%d [INV] %d\n",eUIPacking,eUI3DTO2D,eUI3DFPR,eUILRInverse);

        eDecInput = TDTV_UINT32_DEC_INPUT(sVdoCfg[u4Path].HDMI);
        eDecOutput = TDTV_UINT32_DEC_OUTPUT(sVdoCfg[u4Path].HDMI);
        printf("[  HDMI] Detected As %s ,Input Config As %s, Output Config As %s\n",
            DEC_IN_NAME(eHDMIDetectPack),
            DEC_IN_NAME(eDecInput),
            DEC_OUT_NAME(eDecOutput));

        eDecInput = TDTV_UINT32_DEC_INPUT(sVdoCfg[u4Path].B2R);
        eDecOutput = TDTV_UINT32_DEC_OUTPUT(sVdoCfg[u4Path].B2R);

        printf("[   B2R] Detected As %s ,Input Config As %s, Output Config As %s ,[Inv]%d\n",
            DEC_IN_NAME(eMMDetectPack),    
            DEC_IN_NAME(eDecInput),
            DEC_OUT_NAME(eDecOutput),
            eB2RLRInv);

        printf("[    NR] Config As %d:%s\n",sVdoCfg[u4Path].NR,szNRName[sVdoCfg[u4Path].NR]);
        printf("[    DI] Config As %d:%s\n",sVdoCfg[u4Path].DI,szDIName[sVdoCfg[u4Path].DI]);

        eDecInput = TDTV_UINT32_DEC_INPUT(sVdoCfg[u4Path].SC);
        eDecOutput = TDTV_UINT32_DEC_OUTPUT(sVdoCfg[u4Path].SC);

        printf("[    SC] Detected as %s ,Input Config As %s , Output Config As %s\n",
            DEC_IN_NAME(eSCDetectPack),
            DEC_IN_NAME(eDecInput),
            DEC_OUT_NAME(eDecOutput));

        u4CfgTmp=(sVdoCfg[u4Path].MJC&0xF);
        u4CfgTmp=u4CfgTmp<E_TDTV_MJC_BYPASS ? u4CfgTmp:0;       
        printf("[   MJC] Config As %s\n",szMJCName[u4CfgTmp]);   

    	printf("[   OSTG]\n"); 
        Printf("eOSTGCfg %d\n", sVdoCfg[u4Path].OSTG.eOSTGCfg);
        Printf("u4Clk    %d\n", sVdoCfg[u4Path].OSTG.u4Clk);
        Printf("u4Width  %d\n", sVdoCfg[u4Path].OSTG.u4Width);
        Printf("u4Height %d\n", sVdoCfg[u4Path].OSTG.u4Height);
        Printf("u4VTotal %d\n", sVdoCfg[u4Path].OSTG.u4VTotal);
        Printf("u4HTotal %d\n", sVdoCfg[u4Path].OSTG.u4HTotal);
        Printf("u4FrameRate %d\n", sVdoCfg[u4Path].OSTG.u4FrameRate);    
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

    if((u4Tag==TDTV_TAG3D_FP)||(u4Tag==TDTV_TAG3D_FS))//FS or FP
    {
        eUIPacking=E_TDTV_UI_3D_MODE_AUTO;
    }
    else
    {
        eUIPacking=eTDTVUI3DMode;
    }     
}
/**
 * @brief :Interface for UI to inverse L R View
 * @param eTDTVLRInverse: E_TDTV_UI_LR_NORMAL , E_TDTV_UI_LR_INVERSE
 */
void vDrvTDTVLRInverse(E_TDTV_UI_LR eTDTVLRInverse)
{
    eUILRInverse=eTDTVLRInverse;
    
    return;
}
/**
 * @brief :Interface for UI to decide Depth difference of 3D Content
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVDepth(UINT32 u4DepthField)
{
    #if defined(CC_SCPOS_3DTV_SUPPORT) && !defined(CC_MT5365)
	u1PseudoTtdDepth = u4DepthField;
    #endif
    #if defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR)
    vScpipSetDepthWA((UINT8)u4DepthField);
    #endif   

    #ifdef DRV_USE_EXTERNAL_3D_FRC    
    DRVCUST_SetExtFrc3DDepth(u4DepthField);
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
    if(fg3DTO2DDbg==SV_FALSE)
    {
        eUI3DTO2D=e3D22DMode;
    }    
    return;
}

/**
 * @brief :Interface for UI to Chose FPR Mode
 * @param e3D22DMode:E_TDTV_UI_3D_FPR_OFF,E_TDTV_UI_3D_FPR_V1,E_TDTV_UI_3D_FPR_V2
 */
void vDrvTDTV3DFPR(E_TDTV_UI_3D_FPR e3DFPRMode)
{
    if(fgFPRDbg==SV_FALSE)
    {
        eUI3DFPR=e3DFPRMode;
    }
#ifndef CC_3DTV_FPR_SUPPORT
    eUI3DFPR=E_TDTV_UI_3D_FPR_OFF;
#endif
    return;
}

/**
 * @brief :Interface for UI to decide Protrude Sceen for 2D to 3D
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVProtrude(UINT32 u4Protrude)
{
    #ifdef DRV_USE_EXTERNAL_3D_FRC    
    DRVCUST_SetExtFrcProtrude(u4Protrude);
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
    #endif    
}

/**
 * @brief :Interface for UI to decide OSD depth
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVOSDDepth(UINT32 u4OSDDepth)
{
    #ifdef DRV_USE_EXTERNAL_3D_FRC    
    DRVCUST_SetExtFrcOSDDepth(u4OSDDepth);
    #endif    
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
    
#if SUPPORT_3D_FOR_HD_ONLY
    if(wDrvVideoInputHeight(SV_VP_MAIN)<720)
    {
        return TDTV_TAG3D_TTDO;//2D to 3D Only
    }
#endif    

    if(bGetSignalType(bPath)==SV_ST_MPEG)
    {
        eDetectedPak=eMMDetectPack;
    }
    else if(bGetSignalType(bPath)==SV_ST_DVI)
    {
        eDetectedPak=eHDMIDetectPack;
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
        case E_TDTV_DECODER_INPUT_2D_Q_I:  
        case E_TDTV_DECODER_INPUT_2D_Q_P:            
            return TDTV_TAG3D_2D;
        case E_TDTV_DECODER_INPUT_FS_I:    
        case E_TDTV_DECODER_INPUT_FS_P:
        case E_TDTV_DECODER_INPUT_FS_Q_I: 
        case E_TDTV_DECODER_INPUT_FS_Q_P:     
            return TDTV_TAG3D_FS;
        case E_TDTV_DECODER_INPUT_FP_I:     
        case E_TDTV_DECODER_INPUT_FP_P:
            return TDTV_TAG3D_FP;
        case E_TDTV_DECODER_INPUT_TB_I:
        case E_TDTV_DECODER_INPUT_TB_P:
            return TDTV_TAG3D_TB;
        case E_TDTV_DECODER_INPUT_SBS_I:    
        case E_TDTV_DECODER_INPUT_SBS_P:
        case E_TDTV_DECODER_INPUT_SBS_Q_I:  
        case E_TDTV_DECODER_INPUT_SBS_Q_P:    
            return TDTV_TAG3D_SBS;
        case E_TDTV_DECODER_INPUT_RD_I:     
        case E_TDTV_DECODER_INPUT_RD_P:
            return TDTV_TAG3D_REALD;
        case E_TDTV_DECODER_INPUT_SS_I:     
        case E_TDTV_DECODER_INPUT_SS_P:     
            return TDTV_TAG3D_SENSIO;
        case E_TDTV_DECODER_INPUT_LI_I:     
        case E_TDTV_DECODER_INPUT_LI_P:     
            return TDTV_TAG3D_LI;
        default:
            return TDTV_TAG3D_2D;
      }          
   
}

/**
 * @brief :Interface for MW to know the Decoder Detected 3D Tag
 * @param bPath: SV_VP_MAIN / SV_VP_PIP
 */
TDTV_TAG3D_TYPE_T u4DrvTDTVGetDecTag3D(E_DEC_MOD eDec)
{
   
    E_TDTV_DECODER_INPUT eDetectedPak;
    
    if(E_DEC_MOD_MM==eDec)
    {
        eDetectedPak=eMMDetectPack;
    }
    else if(E_DEC_MOD_HDMI==eDec)
    {
        eDetectedPak=eHDMIDetectPack;
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
        case E_TDTV_DECODER_INPUT_2D_Q_I:  
        case E_TDTV_DECODER_INPUT_2D_Q_P:            
            return TDTV_TAG3D_2D;
        case E_TDTV_DECODER_INPUT_FS_I:    
        case E_TDTV_DECODER_INPUT_FS_P:
        case E_TDTV_DECODER_INPUT_FS_Q_I: 
        case E_TDTV_DECODER_INPUT_FS_Q_P:     
            return TDTV_TAG3D_FS;
        case E_TDTV_DECODER_INPUT_FP_I:     
        case E_TDTV_DECODER_INPUT_FP_P:
            return TDTV_TAG3D_FP;
        case E_TDTV_DECODER_INPUT_TB_I:
        case E_TDTV_DECODER_INPUT_TB_P:
            return TDTV_TAG3D_TB;
        case E_TDTV_DECODER_INPUT_SBS_I:    
        case E_TDTV_DECODER_INPUT_SBS_P:
        case E_TDTV_DECODER_INPUT_SBS_Q_I:  
        case E_TDTV_DECODER_INPUT_SBS_Q_P:    
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
//Obsolete Interface , Maybe Changed in the Future
//=============================================================================

void vTV3DGetForce2D(UINT8 bPath, UINT8 *pu1Force2D, UINT8 *pu1LR)
{
    ASSERT(pu1Force2D != NULL);
    ASSERT(pu1LR != NULL);

    if(bPath==SV_VP_MAIN)
    {
        *pu1Force2D = (eUI3DTO2D !=E_TDTV_UI_3D_2_2D_OFF);
        *pu1LR = (eUI3DTO2D != E_TDTV_UI_3D_2_2D_L);
    }
    else
    {
        *pu1Force2D = 0;
        *pu1LR = 0;     
    }
}

void vTV3DGetStatus(UINT8 bPath, TV3D_STATUS_T *pr3DStatus)
{
    ASSERT(bPath < VDP_NS);    
    ASSERT(pr3DStatus != NULL);
    
    vTV3DGetForce2D(bPath, &(pr3DStatus->u1Force2D), &(pr3DStatus->u1DisplayLR));
    pr3DStatus->u13DEnable = eUIPacking!=E_TDTV_UI_3D_MODE_OFF;

    E_TDTV_DECODER_INPUT eDecInputPack;
    if(bPath!=SV_VP_MAIN)
    {
        pr3DStatus->u13DEnable = 0;            
        pr3DStatus->r3DType = TV3D_TYPE_NS;
        return;//No 3D Support
    }
    switch (bGetVideoDecType(SV_VP_MAIN))
    {
        case SV_VD_DVI:
            eDecInputPack=TDTV_UINT32_DEC_INPUT(sVdoCfg[SV_VP_MAIN].HDMI);
            break;
        case SV_VD_MPEGHD:
            eDecInputPack=TDTV_UINT32_DEC_INPUT(sVdoCfg[SV_VP_MAIN].B2R);
            break;        
        default:       
            pr3DStatus->u13DEnable = 0;            
            pr3DStatus->r3DType = TV3D_TYPE_NS;
            return;//No 3D Support
    }

    switch(eDecInputPack)
    {
        case E_TDTV_DECODER_INPUT_FS_I:
        case E_TDTV_DECODER_INPUT_FS_P:
        case E_TDTV_DECODER_INPUT_FP_I:
        case E_TDTV_DECODER_INPUT_FP_P:
        case E_TDTV_DECODER_INPUT_FS_Q_I:
        case E_TDTV_DECODER_INPUT_FS_Q_P:
            pr3DStatus->r3DType = TV3D_TYPE_FRAME_SEQUENTIAL;
            break;
        case E_TDTV_DECODER_INPUT_SBS_I:
        case E_TDTV_DECODER_INPUT_SBS_P:
        case E_TDTV_DECODER_INPUT_RD_I:
        case E_TDTV_DECODER_INPUT_RD_P:
        case E_TDTV_DECODER_INPUT_SS_I:
        case E_TDTV_DECODER_INPUT_SS_P: 
        case E_TDTV_DECODER_INPUT_SBS_Q_I:
        case E_TDTV_DECODER_INPUT_SBS_Q_P: 
            pr3DStatus->r3DType = TV3D_TYPE_SIDE_BY_SIDE_HALF;
            break;                
        case E_TDTV_DECODER_INPUT_TB_I:
        case E_TDTV_DECODER_INPUT_TB_P:            
            pr3DStatus->r3DType = TV3D_TYPE_TOP_N_BOTTOM;
            break;
        case E_TDTV_DECODER_INPUT_LI_I:
        case E_TDTV_DECODER_INPUT_LI_P:            
            pr3DStatus->r3DType = TV3D_TYPE_LINE_INTERLEAVE;
            break;
        default:
            //pr3DStatus->r3DType = TV3D_TYPE_FIELD_ALTERNATIVE;  Current Not support
            //pr3DStatus->r3DType = TV3D_TYPE_SIDE_BY_SIDE_FULL;  Current Not Support
            pr3DStatus->r3DType = TV3D_TYPE_NS;
    }   
}

//=============================================================================
//                                 Main Thread
//=============================================================================


UINT8 u1LRSwitch;
UINT16  u2Delay2usTimes;

void vDrvTDTVInit()
{
    static UINT8 u1Initialized=0;
#ifdef DRV_USE_EXTERNAL_3D_FRC
    UINT32 i,j;
#endif
    if(u1Initialized==0)
    {
        //Intialize FW Register
#ifdef CC_SCPOS_3DTV_SUPPORT        
        vIO32WriteFldAlign(TDTV_FW_00,0x1,BL_MLOOP);
#endif
#ifdef DRV_USE_EXTERNAL_3D_FRC
        for(i=0;i<NUM_OF_DECOER;++i)
        {
            for(j=0;j<NUM_OF_PACKING_TYPE;++j)
            {
                u8VdoPathProcCfg[i][j]=
                    u8VdoPathProcCfgExt[i][j];
            }
        }
#endif

#if SUPPORT_3D_EMBEDED_OSD
        fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_OSD,vDrvTDTVOSDCallback);
#endif

        vDrvTDTVHDMIConfig();
        vDrvTDTVMMConfig();
        vDrvTDTVOtherSrcConfig();
        vDrvTDTVNRConfig();
        vDrvTDTVDIConfig();
        vDrvTDTVMJCConfig();
        vDrvTDTVSCConfig(SV_VP_MAIN);  
        vDrvTDTVOSDConfig();
        vDrvTDTVOSTGConfig();

        sVdoCfg[SV_VP_PIP]=sVdoCfg[SV_VP_MAIN];
        
        u1Initialized=1;
    }
}


static void vDrvTDTVCheckModeChange()
{
    UINT32 u4HDMIModeChange=0;
    UINT32 u4B2RModeChange=0;
    UINT32 u4VdoMainPathNeedModeChange=0;
    UINT32 u4VdoPipPathNeedModeChange=0;
    
    //Reconfig All Video Module Setting
    vDrvTDTVHDMIConfig();
    vDrvTDTVMMConfig();
    vDrvTDTVOtherSrcConfig();
    vDrvTDTVNRConfig();
    vDrvTDTVDIConfig();
    vDrvTDTVMJCConfig();
    vDrvTDTVSCConfig(SV_VP_MAIN);
    vDrvTDTVSCConfig(SV_VP_PIP);    
    vDrvTDTVOSDConfig();
    vDrvTDTVOSTGConfig();

    //Checking HDMI Callback
    if(sPreVdoCfg[SV_VP_MAIN].HDMI!=sVdoCfg[SV_VP_MAIN].HDMI)
    {       
        if(bGetSignalType(SV_VP_MAIN)==SV_ST_DVI)
        {                    
            u4HDMIModeChange=1;        
        }
        TTD_MODE_CHANGE_CB(E_TDTV_CB_HDMI);        
    }

    //Checking B2R Callback
    if((sPreVdoCfg[SV_VP_MAIN].B2R!=sVdoCfg[SV_VP_MAIN].B2R)||
        ((TDTV_UINT32_DEC_OUTPUT(sVdoCfg[SV_VP_MAIN].B2R)==E_TDTV_DECODER_OUTPUT_3D_2_2D)&&(ePreUI3DTO2D!=eUI3DTO2D))
        )
    {    
        if(bGetSignalType(SV_VP_MAIN)==SV_ST_MPEG)
        {   
            if(!((TDTV_UINT32_DEC_OUTPUT(sVdoCfg[SV_VP_MAIN].B2R)==TDTV_UINT32_DEC_OUTPUT(sPreVdoCfg[SV_VP_MAIN].B2R))&&
                (TDTV_UINT32_DEC_OUTPUT(sVdoCfg[SV_VP_MAIN].B2R)==E_TDTV_DECODER_OUTPUT_NATIVE)))
            {
                u4B2RModeChange=1;
            }
        }
        TTD_MODE_CHANGE_CB(E_TDTV_CB_MM);        
    }

    //Checking NR Callback
    if(sPreVdoCfg[SV_VP_MAIN].NR!=sVdoCfg[SV_VP_MAIN].NR)
    {
        u4VdoMainPathNeedModeChange=1;
        TTD_MODE_CHANGE_CB(E_TDTV_CB_NR);
    }    

    //Checking DI Callback
    if(sPreVdoCfg[SV_VP_MAIN].DI!=sVdoCfg[SV_VP_MAIN].DI)
    {    
        u4VdoMainPathNeedModeChange=1;
        TTD_MODE_CHANGE_CB(E_TDTV_CB_DI);
    }   

    //Checking Scaler Main Path Callback
    if(sPreVdoCfg[SV_VP_MAIN].SC!=sVdoCfg[SV_VP_MAIN].SC)
    {
        u4VdoMainPathNeedModeChange=1;    
        TTD_MODE_CHANGE_CB(E_TDTV_CB_SCALER);
    } 

    //Checking Scaler Sub Path Callback for scart out
    if(sPreVdoCfg[SV_VP_PIP].SC!=sVdoCfg[SV_VP_PIP].SC)
    {
        u4VdoPipPathNeedModeChange=1;        
        TTD_MODE_CHANGE_CB(E_TDTV_CB_SCALER);
    } 

    //Checking MJC Callback
    if(sPreVdoCfg[SV_VP_MAIN].MJC!=sVdoCfg[SV_VP_MAIN].MJC)
    {
        u4VdoMainPathNeedModeChange=1;    
        TTD_MODE_CHANGE_CB(E_TDTV_CB_MJC);        
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
    ePreUI3DTO2D=eUI3DTO2D;

    //Trigger Main Mode Change if no Decoder Mode Change
    if(u4VdoMainPathNeedModeChange)
    {
        if(bDrvVideoSignalStatus(SV_VP_MAIN)==SV_VDO_STABLE)
        {
            if((bGetSignalType(SV_VP_MAIN)==SV_ST_DVI)&&(u4HDMIModeChange==0))        
            {
                vSetMainFlg(MAIN_FLG_MODE_CHG);
                vSetMainFlg(MAIN_FLG_MODE_DET_DONE);       
            }
            else if((bGetSignalType(SV_VP_MAIN)==SV_ST_MPEG)&&(u4B2RModeChange==0))
            {    
                vSetMainFlg(MAIN_FLG_MODE_CHG);
                vSetMainFlg(MAIN_FLG_MODE_DET_DONE); 
            }
            else
            {    
                vSetMainFlg(MAIN_FLG_MODE_CHG);
                vSetMainFlg(MAIN_FLG_MODE_DET_DONE); 
            }        
        }
    }
    //Trigger PIP Mode Change if no Decoder mode Change
    if(u4VdoPipPathNeedModeChange)
    {   
        if(bDrvVideoSignalStatus(SV_VP_PIP)==SV_VDO_STABLE)
        {
            if((bGetSignalType(SV_VP_PIP)==SV_ST_DVI)&&(u4HDMIModeChange==0))        
            {
                vSetMainFlg(PIP_FLG_MODE_CHG);
                vSetMainFlg(PIP_FLG_MODE_DET_DONE);       
            }
            else if((bGetSignalType(SV_VP_PIP)==SV_ST_MPEG)&&(u4B2RModeChange==0))
            {    
                vSetMainFlg(PIP_FLG_MODE_CHG);
                vSetMainFlg(PIP_FLG_MODE_DET_DONE); 
            }
            else
            {    
                vSetMainFlg(MAIN_FLG_MODE_CHG);
                vSetMainFlg(MAIN_FLG_MODE_DET_DONE); 
            }
        }
    }
}

#define SCAN_PWM_START (250/4) //Delay Depend on Panel Raising , Falling Speed

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

static void vDrv3DBacklightConfing()
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

/**
 * @brief :Reset of LR Sync to Shutter Glass
 */
void vDrvTDTVLRReset(void)
{
#if SUPPORT_3D_SG_CTRL
    UINT32 u4LRResetGPIO;
    DRVCUST_OptQuery(e3GlassResetGPIO,&u4LRResetGPIO);
    
    if ((bInitPWM2 == TRUE) && IS_LVDS_DISP_3D_SHUTTER)
    {
        u13DPreDispR = u13DCurDispR;
        u13DCurDispR = IO32ReadFldAlign(MLVDS_SO8, ST_LVDS_DISP_R);

        if (u1LRResetFlag && (u13DPreDispR == 0) && u13DCurDispR)  //  if(flag) &&(L->R)  reset , flag = 0
        {
            u1LRResetFlag = 0;
            GPIO_SetOut(u4LRResetGPIO, 0); // pull reset,  waveform always high or low.
        }
        else
        {
            GPIO_SetOut(u4LRResetGPIO, 1); // release reset, waveform toggle
        }
        
        if (!(u13DPreDispR ^ u13DCurDispR)) // if(RR or LL) flag = 1
        {
            u1LRResetFlag = 1;
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
/**
 * @brief :Process to Communicate with Exnternal Companion Chip
 */
static void vDrvTDTVExtProc(void)
{
#if SUPPORT_3D_EXT_COMP_TRL

    UINT32 u4Ext3DEnableGPIO;
    DRVCUST_OptQuery(e3DEnableGPIO,&u4Ext3DEnableGPIO);
    
    if((u4DrvTDTV3DModeQuery()!=E_TDTV_UI_3D_MODE_OFF)&&
        (bDrvVideoSignalStatus(SV_VP_MAIN)==SV_VDO_STABLE))
    {
        GPIO_SetOut(u4Ext3DEnableGPIO, 1);
        //Output LR Singal Sync    
#if defined(CC_MT5365)
		if (DRVCUST_InitGet(eFlagDDRQfp))
		{
			BSP_PinSet(OPCTRL(4),PINMUX_FUNCTION1); // 5365
		}
		else
		{
			BSP_PinSet(GPIO(19),PINMUX_FUNCTION1); // 5366
		}
#else
        vIO32WriteFldAlign(CKGEN_PMUX2, 0x1, Fld(1,24,AC_MSKB3));
#endif
    }
    else
    {
        GPIO_SetOut(u4Ext3DEnableGPIO, 0);
        //Disable LR Signal Sync
#if defined(CC_MT5365)
		if (DRVCUST_InitGet(eFlagDDRQfp))
		{
			BSP_PinSet(OPCTRL(4),PINMUX_FUNCTION0); // 5365 
		}
		else
		{
			BSP_PinSet(GPIO(19),PINMUX_FUNCTION0); // 5366
		}
#else
		vIO32WriteFldAlign(CKGEN_PMUX2, 0x0, Fld(1,24,AC_MSKB3));
#endif

    }
#endif  

#ifdef DRV_USE_EXTERNAL_3D_FRC
    DRVCUST_SetExtFrc3DFormat(u4DrvTDTV3DModeQuery());
#endif

}

static void vDrvTDTVInvPrco(void)
{
    static E_TDTV_UI_LR ePreLRInv;
    E_TDTV_UI_LR eLRInv = (E_TDTV_UI_LR)u4DrvTDTVIsILRInv();
    
    if(ePreLRInv != eLRInv)
    {    
#if defined(CC_MT5395) && SUPPORT_3D_WITHOUT_MJC
        vScpipSetOutputDispRInv(eTDTVLRInverse!=E_TDTV_UI_LR_INVERSE);
#elif defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR)
        vScpipLRInverseWA((UINT8)eTDTVLRInverse);
#endif

#ifdef DRV_USE_EXTERNAL_3D_FRC
        DRVCUST_SetExtFrc3DInverse((UINT32)eLRInv);
#endif
        ePreLRInv = eLRInv;
    }
}

void vDrvTDTVCropProc()
{
#ifdef IS_SUPPORT_3D_PHOTO
    TDTV_3D_CROP_INTO_T eCropInfo;
    //Check Cropping info
    if((NULL!=TDTV3DCropCallback[E_TDTV_CB_3D_CROP_MM])
        &&(bGetSignalType(SV_VP_MAIN)==SV_ST_MPEG))
    {
        TDTV3DCropCallback[E_TDTV_CB_3D_CROP_MM](&eCropInfo);

        LboxSetPhotoPosSize(eCropInfo.u4CropTopOffset,
            eCropInfo.u4CropBottomOffset,
            eCropInfo.u4CropLeftOffset,
            eCropInfo.u4CropRightOffset,SV_ON);

        Set3DPhotoRegion(
            eCropInfo.fgIsFullResolution, 
            eCropInfo.fgIsFixSrcAspectRatio,
            eCropInfo.u4CropTopOffset,
            eCropInfo.u4CropBottomOffset,
            eCropInfo.u4CropLeftOffset,
            eCropInfo.u4CropRightOffset,
            SV_ON);            
    }
    else
    {
        LboxSetPhotoPosSize(0,0,0,0,SV_OFF);

        Set3DPhotoRegion(
            0,0,0,0,0,0,
            SV_OFF);                    
    } 
#endif    
}

VOID vDrvTDTVProc()
{
    vDrvTDTVCheckModeChange();
    vDrvTDTVInvPrco();
    vDrvTDTVExtProc();
    vDrvTDTVCropProc();   
    vDrv3DBacklightConfing();    
}
