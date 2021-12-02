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
 */

// === INCLUDE =============================================================================
#include "x_assert.h"
#include "sv_const.h"
#include "nptv_debug.h"
#include "pe_if.h"
#include "drvcust_if.h"
#include "drv_nr.h"
#include "vdp_drvif.h"
#include "vdo_misc.h"
#include "mute_if.h"
#include "drv_contrast.h"
#include "drv_lcdim.h"
#include "drv_scpos.h"
#include "drv_video.h"
#include "drv_upscaler.h"
#include "video_def.h"
#include "drv_upscaler.h"
#include "drv_od.h"
#include "drv_tdc.h"
#include "drv_tdtv.h"


#ifdef DRV_SUPPORT_MT8283
#include "extmjc_if.h"
#endif

#ifdef CC_MT5395
#include "drv_mjc.h"
#endif

extern void vDrvDBLInit(void);

#ifdef CUSTOM_VIDEO_DRV_INCLUDE_FILE
	#include CUSTOM_VIDEO_DRV_INCLUDE_FILE
#endif // #ifndef CUSTOM_VIDEO_DRV_INCLUDE_FILE


// === DEFINE =============================================================================
#define GAMMA_INDEX 257

#ifndef SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM
    #define SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM    (0)
    #if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM
        #ifndef FLASH_PQ_GAMMA_NUM
            #define FLASH_PQ_GAMMA_NUM              (3)
        #endif
    #endif
#endif

#ifndef SUPPORT_EXTMJC_SEND_GAMMA_TABLE
    #define SUPPORT_EXTMJC_SEND_GAMMA_TABLE     (1)
#endif

// === FUNCTION PROTOTYPE =================================================================
static void vApiApplyVideoEvent(UINT8 bApplyTime);
// === EXTERN FUNCTION ====================================================================
extern void vHalVideoOPCResultUpdate(void);
extern void DRVCUST_VideoInit(void);
#if defined(CC_MT5395)
EXTERN UINT8 u1DrvMJCIsOnOff(void);
#endif
// === GLOBAL VARIABLE ====================================================================
UINT8 QUALITY_TBL[QUALITY_MAX];
// === EXTERN VARIABLE ====================================================================
extern UINT8 bBackLightCtrl;
extern UINT8 _bDviTiming;
extern const tagDFT_QTY  CODE aDefaultQtyTbl[QUALITY_MAX];
extern UINT8 GAMMA_256[3][GAMMA_INDEX];
// === STATIC VARIABLE ====================================================================
static UINT8 bRegisterList[PE_EVENT_NS][REGISTER_DIM];
static UINT8 u1AdaptiveBackLightPWM[ADP_BACKLIGHT_END_IDX];

// apply RGB offset 2 in Vsync
static UINT16 _u2ROfst2 = 0;
static UINT16 _u2GOfst2 = 0;
static UINT16 _u2BOfst2 = 0;
// apply xvYCC in Vsync
static UINT8 _u1XVOnOff;
static UINT8 _u1XVGammaOnOff;
static UINT16 _u2XVMatrix[9];
static UINT16 _u2XVRgbGain[3] = {0x200, 0x200, 0x200};

// read gamma table from flash PQ when initialization

#if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM 
static UINT8 _rGammaTbl[FLASH_PQ_GAMMA_NUM][3*GAMMA_INDEX];
#endif

// === BODY ===============================================================================

void vApiVideoSRMNRYCOnOff(UINT8 bYOnOff, UINT8 bCOnOff)
{
//    vDrvNRTIIRMotionYCOnOff(bYOnOff,bCOnOff);
}

void vApiVideoNRTIIROnOff(UINT8 bOnOff)
{
    vDrvNRSet3DNROnOff(bOnOff);
}

void vApiVideoNR10bitMode(UINT8 bOnOff)
{
//    vDrvNRSetDramMode(bOnOff);
}

void vApiVideo2DNROnOff(UINT8 bOnOff)
{
    vDrvNRSet2DNROnOff(bOnOff);
}

void vApiVideo3DNROnOff(UINT8 bOnOff)
{
    vDrvNRSet3DNRUIOnOff(bOnOff);
}

void vApiVideoMPEGNROnOff(UINT8 bOnOff)
{
    vDrvNRSetDeblockingOnOff(bOnOff);
    vDrvNRSetMosquitoOnOff(bOnOff);
}

static void _vVideoRGBGainVsync(UINT8 bPath)
{
    vHalVideoRGBGain(bPath, SV_RGB_R, wReadQualityTable(QUALITY_R_GAIN));
    vHalVideoRGBGain(bPath, SV_RGB_G, wReadQualityTable(QUALITY_G_GAIN));
    vHalVideoRGBGain(bPath, SV_RGB_B, wReadQualityTable(QUALITY_B_GAIN));
}

static void _vVideoRGBOfst1Vsync(UINT8 bPath)
{
    vHalVideoRGBOffset(bPath, SV_RGB_R, wReadQualityTable(QUALITY_R_OFFSET));
    vHalVideoRGBOffset(bPath, SV_RGB_G, wReadQualityTable(QUALITY_G_OFFSET));
    vHalVideoRGBOffset(bPath, SV_RGB_B, wReadQualityTable(QUALITY_B_OFFSET)); 
}

static void _vVideoRGBOfst2Vsync(UINT8 bPath)
{
    vDrvSetROffset12(bPath, _u2ROfst2);
    vDrvSetGOffset12(bPath, _u2GOfst2);
    vDrvSetBOffset12(bPath, _u2BOfst2);
}

/**
 * @brief Register video process event, wait for apply to hardware in output v-sync.
 * @param bVdpArg VDP_ARG_xx
 * @param bPath <SV_VP_MAIN/SV_VP_PIP>
 * @param bOnOff <SV_ON/SV_OFF>
 */
void vApiRegisterVideoEvent(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff)
{
#if defined(CC_MT5395)    
    if (bVdpArg < PE_EVENT_BEFORE_SCALER)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_INPUT_VSYNC_ISR);
    }
    else if ((bVdpArg < PE_EVENT_BEFORE_MJC) && (u1DrvMJCIsOnOff() == SV_ON))
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_MJC_INPUT_VSYNC_ISR);
    }
    else
    {        
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);
    }
#else
    if (bVdpArg < PE_EVENT_BEFORE_SCALER)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_INPUT_VSYNC_ISR);
    }
    else
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);
    }
#endif
}

void vApiRegisterVideoEventByApplyTime(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff, UINT8 bApplyTime)
{
    if(bVdpArg >= PE_EVENT_NS)
    {
        return;
    }
    
    bRegisterList[bVdpArg][REGISTER_CALLBACK] = SV_ON;
    bRegisterList[bVdpArg][REGISTER_PATH] = bPath;
    bRegisterList[bVdpArg][REGISTER_ON_OFF] = bOnOff;
    bRegisterList[bVdpArg][REGISTER_APPLY_TIME] = bApplyTime;
}

void vApiApplyVideoEventInputVSync(void)
{
    vApiApplyVideoEvent(PEQTY_APPLY_TIME_INPUT_VSYNC_ISR);
}

void vApiApplyVideoEventOutputVSync(void)
{
    vApiApplyVideoEvent(PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);

#if defined(CC_MT5395)
    if (u1DrvMJCIsOnOff() == SV_OFF)
#endif    
    {
        vDrvDummyToRegister();	
    }
}

void vApiApplyVideoEventMJCInputVSync(void)
{
    vApiApplyVideoEvent(PEQTY_APPLY_TIME_MJC_INPUT_VSYNC_ISR);  
    
#if defined(CC_MT5395)
    if (u1DrvMJCIsOnOff() == SV_ON)
    {
        vDrvDummyToRegister();	
    }
#endif
}

void vApiApplyVideoEvent(UINT8 bApplyTime)
{
    UINT8 bVdpArg;

    for (bVdpArg = 0; bVdpArg < PE_EVENT_NS; bVdpArg++)
    {
        if (bRegisterList[bVdpArg][REGISTER_CALLBACK] == SV_ON && bRegisterList[bVdpArg][REGISTER_APPLY_TIME] == bApplyTime)
        {
            switch (bVdpArg)
            {
                case PE_EVENT_GAMMA:
                    vDrvGammaWriteTable();                    
                    break;
                case PE_EVENT_R_GAIN:     
                    vHalVideoRGBGain(bRegisterList[bVdpArg][REGISTER_PATH],
                                        SV_RGB_R, wReadQualityTable(QUALITY_R_GAIN));                                
                    break;
                case PE_EVENT_G_GAIN:     
                    vHalVideoRGBGain(bRegisterList[bVdpArg][REGISTER_PATH],
                                        SV_RGB_G, wReadQualityTable(QUALITY_G_GAIN));
                    break;
                case PE_EVENT_B_GAIN:      
                    vHalVideoRGBGain(bRegisterList[bVdpArg][REGISTER_PATH],
                                        SV_RGB_B, wReadQualityTable(QUALITY_B_GAIN));
                    break;
                case PE_EVENT_R_OFFSET:    
                    vHalVideoRGBOffset(bRegisterList[bVdpArg][REGISTER_PATH],
                                        SV_RGB_R, wReadQualityTable(QUALITY_R_OFFSET));
                    break;
                case PE_EVENT_G_OFFSET:    
                    vHalVideoRGBOffset(bRegisterList[bVdpArg][REGISTER_PATH],
                                        SV_RGB_G, wReadQualityTable(QUALITY_G_OFFSET));
                    break;
                case PE_EVENT_B_OFFSET:    
                    vHalVideoRGBOffset(bRegisterList[bVdpArg][REGISTER_PATH],
                                        SV_RGB_B, wReadQualityTable(QUALITY_B_OFFSET));
                    break;
                case PE_EVENT_RGB_GAIN_OFST:
                    vHalVideoRGBGainOfstFromQtyTbl(bRegisterList[bVdpArg][REGISTER_PATH]);
                    break;

                case PE_EVENT_RGB_GAIN:
                    _vVideoRGBGainVsync(bRegisterList[bVdpArg][REGISTER_PATH]);
                    break;

                case PE_EVENT_RGB_OFST1:
                    _vVideoRGBOfst1Vsync(bRegisterList[bVdpArg][REGISTER_PATH]);
                    break;

                case PE_EVENT_RGB_OFST2:
                    _vVideoRGBOfst2Vsync(bRegisterList[bVdpArg][REGISTER_PATH]);
                    break;

                case PE_EVENT_XVYCC:
                    vDrvxvYCCOnOff(_u1XVOnOff);   
                    vDrvxvYCCGammaOnOff(_u1XVGammaOnOff);   
                    vDrvxvYCCAntiCGammaOnOff(_u1XVGammaOnOff); 
                    vDrvxvYCCLoad3x3Matrix(_u2XVMatrix);
                    break;
                    
                case PE_EVENT_XVRGB_GAIN:
                    vDrvxvYCCSetGain(_u2XVRgbGain);
                    break;

                case PE_EVENT_ADAPTIVE_LUMA:
                    vHalVideoAdaptiveLumaOnOff(bRegisterList[bVdpArg][REGISTER_PATH],
                                        bRegisterList[bVdpArg][REGISTER_ON_OFF]);
                    break;                    
                case PE_EVENT_LUMA_CURVE:
                    vDrvADLUpdateLumaCurve();
                    break;     
                case PE_EVENT_SCE:
                    vHalVideoSCELoadTable(bRegisterList[bVdpArg][REGISTER_PATH]);
                    break; 
               	case PE_EVENT_OPC_CURVE:
               	    vHalVideoOPCResultUpdate();
                    break;
                case PE_EVENT_SHARPNESS:
    		        vDrvLoadYLevTable();
                    break;
                case PE_EVENT_OD:
                    vDrvODLoadODTable();
                default:
                    break;
            }

            // After call video api, turn-off callback flag.
            bRegisterList[bVdpArg][REGISTER_CALLBACK] = SV_OFF;
        }
    }
}

/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/

extern UINT16 wDrvDIGetWidth(void);
extern UINT16 u2ScpipGetDispWidth(UINT8 bPath);
extern UINT16 u2ScpipGetDispHeight(UINT8 bPath); 
void vApiVideoDemoMode(UINT8 bmode, Region DemoRec, UINT8 bDEMOModules)
{
    Region DemoRegion;
    UINT8 bSplitOnOff, bMovingOnOff;
    UINT8 bRegion=0;
    UINT16 wDispCenter;
    UINT16 wInputWidth = wDrvDIGetWidth();
    UINT16 wDispWidth = u2ScpipGetDispWidth(SV_VP_MAIN);
    wDispCenter = (UINT16)u8Div6432(wDispWidth, 2, NULL);
    DemoRegion.wVStart = 0x0;
    DemoRegion.wVEnd = u2ScpipGetDispHeight(SV_VP_MAIN);
    //Fix Klocwork
    DemoRegion.wHStart = 0x0;
    DemoRegion.wHEnd = wDispCenter;
    DemoRegion.u1OnOff = SV_ON;
    
    switch (bmode)
    {
    case VDP_PQ_DEMO_ON_LEFT://Split demo
        bSplitOnOff = SV_ON;        
        bRegion = SV_LEFT;
        DemoRegion.u1OnOff = SV_ON;        
        DemoRegion.wHStart = 0x0;
        DemoRegion.wHEnd = wDispCenter;
        bMovingOnOff = SV_OFF;
        break;
    case VDP_PQ_DEMO_ON_RIGHT://Split demo
        bSplitOnOff = SV_ON;    
        bRegion = SV_RIGHT;
        DemoRegion.u1OnOff = SV_ON;        
        DemoRegion.wHStart = wDispCenter;
        DemoRegion.wHEnd = wDispWidth;
        bMovingOnOff = SV_OFF;
        break;
    case VDP_PQ_DEMO_STILL_REC://Rectangular demo
        bSplitOnOff = SV_OFF;
        DemoRegion.u1OnOff = SV_ON;
        DemoRegion.wHStart = DemoRec.wHStart;
        DemoRegion.wHEnd = DemoRec.wHEnd;
        DemoRegion.wVStart = DemoRec.wVStart;
        DemoRegion.wVEnd = DemoRec.wVEnd;
        bMovingOnOff = SV_OFF;
        break;
    case VDP_PQ_DEMO_MOVING_REC:
        bSplitOnOff = SV_OFF;
        DemoRegion.u1OnOff = SV_ON;
        DemoRegion.wHStart = DemoRec.wHStart;
        DemoRegion.wHEnd = DemoRec.wHEnd;
        DemoRegion.wVStart = DemoRec.wVStart;
        DemoRegion.wVEnd = DemoRec.wVEnd;
        bMovingOnOff = SV_ON;
    	 break;
    case VDP_PQ_DEMO_OFF:
    default:
         bSplitOnOff = SV_OFF;
         DemoRegion.u1OnOff = SV_OFF;
         bMovingOnOff = SV_OFF;
        break;
    }
    //Set Demo modules	
    vIO32WriteFldAlign(DEMO_02, bDEMOModules, DEMO_MODULES);
    
    //Split demo
    vDrvHSharpDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_HSHARP_EN)), bRegion, (wInputWidth/2)/8);
    vDrvDIDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_MDDI_EN)), bRegion,(wInputWidth/2)+0x44);   // 0x44 is for MEMA Demo position alignment. why 0x44? just test the position, and got this difference value.
    vDrv2DNRDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_2DNR_EN)), bRegion, (wInputWidth/2));
    vDrvTDCDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_TDC_EN)), bRegion, (wInputWidth/2));

    //Rectangular demo
    vDemoSetRegion(DemoRegion);
    vIO32WriteFldAlign(DEMO_00, bMovingOnOff, MOVINGEMO_ENABLE);
}

/*****************************************************************************************/
/*********************************      TDES     *****************************************/
/*****************************************************************************************/
/**
 * @brief TDES Control
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoTDESOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if(bIsScalerInput444(bPath))
    {	//turn off 2d edge smooth
        vDrv2DEdgeSmoothOnOff(SV_OFF);
    }
    else
    {
    	//reload initial table
    	vDrv2DEdgeSmoothInit();
    }
}

/*****************************************************************************************/
/*********************************  HSHARP FRONT *****************************************/
/*****************************************************************************************/
/**
 * @brief Sharpness Control
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoSharpnessOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bIsScalerInput444(bPath))
    {
        bOnOff = SV_OFF;
    }

    vDrvPreSharpOnOff(bPath, bOnOff);

    if (bPath == SV_VP_MAIN)
    {
        vDrvPostSharpOnOff(bOnOff);
    }
}

void vHalVideoSharpnessHorizontalOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bIsScalerInput444(bPath))
    {
        bOnOff = SV_OFF;
    }

    if (bPath == SV_VP_MAIN)
    {
        vDrvPostSharpOnOff(bOnOff);
    }
}

void vHalVideoSharpnessVerticalOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bIsScalerInput444(bPath))
    {
        bOnOff = SV_OFF;
    }

    if (bPath == SV_VP_MAIN)
    {
        vDrvPostSharpOnOff(bOnOff);
    }
}


/**
 * @brief Sharpness Control
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue sharpness mode
 */
void vHalVideoSharpnessParam(UINT8 bPath)
{
    if (bIsScalerInput444(bPath))
    {
		vDrvPreSharpOnOff(bPath, SV_OFF);
		if (bPath == SV_VP_MAIN)
		{
			vDrvPostSharpOnOff(SV_OFF);
		}
        return;
    }

    vDrvPreSharpParam(bPath);
	if (bPath == SV_VP_MAIN)
	{
	    vDrvPostSharpParam();
	}
}

void vHalVideoSharpnessParamHorizontal(UINT8 bPath)
{
    if (bIsScalerInput444(bPath))
    {
		if (bPath == SV_VP_MAIN)
		{
			vDrvPostSharpOnOff(SV_OFF);
		}
        return;
    }

	if (bPath == SV_VP_MAIN)
	{
	    vDrvPostSharpParamHorizontal();
	}
}

void vHalVideoSharpnessParamVertical(UINT8 bPath)
{
    if (bIsScalerInput444(bPath))
    {
		if (bPath == SV_VP_MAIN)
		{
			vDrvPostSharpOnOff(SV_OFF);
		}
        return;
    }

	if (bPath == SV_VP_MAIN)
	{
	    vDrvPostSharpParamVertical();
	}
}

/**
 * @brief Y Level Sharpness table
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bTbl[65]
 */
void vHalVideoYLevSharpness(UINT8 bPath, const UINT8* u1YLevSharp)
{
    if (bPath == SV_VP_MAIN)
    {
        x_memcpy(TDS_YLEV, u1YLevSharp, sizeof(UINT8)*65);
        vApiRegisterVideoEvent(PE_EVENT_SHARPNESS, SV_VP_MAIN, SV_ON);
    }
}

void vHalVideoCBEParam(void)
{
	#ifdef CC_MT5395
		vDrvCBEOnOff(SV_ON);	
		vDrvCBELoadParam();	
		if (bIsScalerInput444(SV_VP_MAIN))
		{
			vDrvCBEOnOff(SV_OFF);
		       return;
		}
	#endif
}

/*****************************************************************************************/
/********************************  ADAPTIVE LUMA    **************************************/
/*****************************************************************************************/
/**
 * @brief enable/disable adaptive luma
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoAdaptiveLumaOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if(bIsScalerInput444(bPath))
    {
    	bOnOff = SV_OFF;
    }
	if(bPath==SV_VP_MAIN)
	{
		vDrvADLOnOff(bOnOff);
	}
}


/**
 * @brief load default setting of adaptive luma
 */
void vHalVideoAdaptiveLumaParam(void)
{
    vDrvADLUpdateParam();
}

/**
 * @brief Dynamic Back Light param, call by UI
 * @param bMode DYNAMIC_CONTRAST_RATIO_?PERCENT
 * @warning related drivers are in drv_contrast.c
 */
void vHalVideoAdaptiveBackLightParam(UINT8 bMode)
{
    DRVCUST_AdaptiveBacklightParam(bMode);
}

/**
 * @brief Set the default value for back light.
 * @param u1Level
 * @param u1Value default value
 */
void vHalSetAdpBacklight(UINT8 u1Level, UINT8 u1Value)
{
    u1AdaptiveBackLightPWM[u1Level] = u1Value;
}

/**
 * @brief get the default value for back light.
 * @param u1Level
 * @param u1Value default value
 */
UINT8 bHalGetAdpBacklight(UINT8 u1Level)
{
    return u1AdaptiveBackLightPWM[u1Level];
}

void vHalVideoSetAdaptiveBacklightMode(UINT8 bMode)
{
    DRVCUST_AdaptiveBacklightMode(bMode);
}

/*****************************************************************************************/
/********************************  LOCAL DIMMING    **************************************/
/*****************************************************************************************/
/**
 * @brief enable/disable local dimming
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoLcDimOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if(bPath==SV_VP_MAIN)
	{
		vDrvSetLcDimOnOff(bOnOff);
	}
}

/*****************************************************************************************/
/***********************************   CONTRAST   ****************************************/
/*****************************************************************************************/
/**
 * @brief set contrast value
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue contrast value
 */
void vHalVideoContrast(UINT8 bPath, UINT16 u2Value)
{
    vDrvSetContrast(bPath, u2Value);
}

/*****************************************************************************************/
/**********************************   BRIGHTNESS   ***************************************/
/*****************************************************************************************/
/**
 * @brief set brightness value
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue brightness value
 */
void vHalVideoBrightness(UINT8 bPath, UINT8 bValue)
{
    vDrvSetBrightness(bPath, bValue);
}

/*****************************************************************************************/
/**********************************   SATURATION   ***************************************/
/*****************************************************************************************/
/**
 * @brief set saturation value
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue saturation value
 */
void vHalVideoSaturation(UINT8 bPath, UINT16 u2Value)
{
    if(fgDrvGetAutoNROnOff() && (bPath == SV_VP_MAIN))
    {
        u2Value = (u2Value <= bReducedSaturation) ? 
            0 : (u2Value - bReducedSaturation);
    }
    vDrvSetSaturation(bPath, u2Value);
}

/*****************************************************************************************/
/*************************************   HUE   *******************************************/
/*****************************************************************************************/
/**
 * @brief set hue value
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue hue value
 */
void vHalVideoHue(UINT8 bPath, UINT8 bValue)
{
    vDrvSetHue(bPath, bValue);
}

/*****************************************************************************************/
/*************************************   SCE   *******************************************/
/*****************************************************************************************/
/**
 * @brief Load SCE table by function index and hue index
 * @param u4FunIdx function index from 0~21
 * @param u4HueIdx hue index from 0~27
 * @param u4Value
 */
UINT32 vApiSCELoadIdx(UINT32 u4FunIdx, UINT32 u4HueIdx, UINT32 u4Value)
{
    return vDrvSCELoadIdx(u4FunIdx, u4HueIdx, u4Value);
}


/**
 * @brief enable/disable SCE
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoSCEOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bIsScalerInput444(bPath))
    {
        //SCE cannot be enabled under RGB444 mode
        vDrvSCEAllOnOff(SV_OFF);
    }
    else if (bPath == SV_VP_MAIN)
    {
        vDrvSCEAllOnOff(bOnOff);
    }
}

/**
 * @brief load SCE table
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue SCE table number
 * @dependency See MAIN_SCE_DEFAULT_VALUE[] in color_param.c
 */
void vHalVideoSCELoadTable(UINT8 bPath)
{
    vDrvSCELoadTable(bPath);
}

/**
 * @brief Set SCE pattern
 * @param bOnOff SV_ON/SV_OFF
 * @param wYdata SCE pattern Y level
 * @return NONE
 * @retval void
 */
extern INT32 const YCBCR2RGBOS[3][15];
extern INT32 COLOR_TRANSFORM_ADJ[15];
void vHalSetSCEPat(UINT8 bOnOff, UINT16 wYdata)
{
    BOOL fgBSOnOff = SV_OFF;
    BOOL fgxvOnOff = SV_OFF;
    INT32 COLOR_TRANSFORM_Y2R[15];

    vDrvSCEPatGen(bOnOff, wYdata);
    vHalVideoSCEOnOff(SV_VP_MAIN, bOnOff);
    if (bOnOff) 
    { 
        fgBSOnOff = fgDrvGetBlueStretchOnOff(); /* store latest setting of blue stretch*/
        if (fgBSOnOff)
        {
            vDrvSetBlueStretchOnOff(SV_OFF); /* bypass blue stretch */
        }

        fgxvOnOff = fgDrvGetxvYCCOnOff(); /* store latest setting of xvYCC*/
        if (fgxvOnOff)
        {
            vDrvxvYCCOnOff(SV_OFF); /* bypass xvYCC */
        }
        
        x_memcpy(COLOR_TRANSFORM_Y2R, YCBCR2RGBOS[2], sizeof(COLOR_TRANSFORM_Y2R));
        vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_Y2R);
        vOSTGVideoPlaneHWMute(SV_VP_MAIN, SV_OFF); //Disable HDMI mute during pattern on
        _vDrvVideoForceUnMute(SV_VP_MAIN, SV_ON);
    }
    else
    { 
        if (fgBSOnOff != fgDrvGetBlueStretchOnOff())
        {
            vDrvSetBlueStretchOnOff(fgBSOnOff); /* restore user blue stretch setting */
        }
        
        if (fgxvOnOff != fgDrvGetxvYCCOnOff())
        {
            vDrvxvYCCOnOff(fgBSOnOff); /* restore user blue stretch setting */
        }

        vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_ADJ);
        vOSTGVideoPlaneHWMute(SV_VP_MAIN, SV_ON); //Enable HDMI mute
        _vDrvVideoForceUnMute(SV_VP_MAIN, SV_OFF);
    }	
}

/*****************************************************************************************/
/********************************** ANGULAR SCE MODEL ************************************/
/*****************************************************************************************/
void vHalSCEAngularReset(void)
{
    vDrvMainSCEAngularReset();
}

void vHalSetSCEAngular(UINT8 bmodule, UINT8 bColor, UINT8 bValue)
{
    switch (bmodule)
    {
        case 0:
            vDrvMainSCEAngularSetBriVal(bColor, bValue);
            break;
        case 1:            
            vDrvMainSCEAngularSetSatVal(bColor, bValue);
            break;
        case 2:            
            vDrvMainSCEAngularSetHueVal(bColor, bValue);
            break;
        default:
            break;
    }
}

/*****************************************************************************************/
/********************************** BLUE STRETCH  ****************************************/
/*****************************************************************************************/

void vHalVideoBlueStretchSetDefault()
{
    BSParam sBSParam;

    sBSParam.bRGain = wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_R);
    sBSParam.bGGain = wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_G);
    sBSParam.bBGain = wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_B);
    sBSParam.bLumaThr = wReadQualityTable(QUALITY_BLUE_STRETCH_LUMA_THD);    
    sBSParam.bChromaThr = wReadQualityTable(QUALITY_BLUE_STRETCH_SAT_THD);            

    vDrvSetBlueStretchParam(sBSParam);
}

void vHalVideoBlueStretchOnOff(UINT8 bOnOff)
{
    if (bIsScalerInput444(SV_VP_MAIN) || bIsScalerInput444(SV_VP_PIP))
    {
        bOnOff = SV_OFF;
    }

#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_blue_stretch(
            bOnOff,
            wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_R),
            wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_G),
            wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_B),
            wReadQualityTable(QUALITY_BLUE_STRETCH_LUMA_THD), 
            wReadQualityTable(QUALITY_BLUE_STRETCH_SAT_THD)          
            );
    }
    else
#endif
    {
        vDrvSetBlueStretchOnOff(bOnOff);
    }
}
/*****************************************************************************************/
/************************************   CTI   *******************************************/
/*****************************************************************************************/
/**
 * @brief Chroma transient improvement
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 * @warning VGA/DVI/HDMI should turn off
 */
void vHalVideoCTIOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bIsScalerInput444(bPath))
    {
        bOnOff = SV_OFF;
    }

    if (bPath == SV_VP_MAIN)
    {
        vDrvCTIFOnOff(bOnOff);
        vDrvCTIROnOff(bOnOff);
    }
}

/**
 * @brief Chroma transient improvement
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 * @warning VGA/DVI/HDMI should turn off
 */
void vHalVideoCTIParam(UINT8 bPath)
{
    if (bIsScalerInput444(bPath))
    {
        vDrvCTIFOnOff(SV_OFF);
        vDrvCTIROnOff(SV_OFF);
        return;
    }

    if (bPath == SV_VP_MAIN)
    {
        vDrvCTIFParam();
        vDrvCTIRParam();
    }
}

/*****************************************************************************************/
/************************************   LTI   *******************************************/
/*****************************************************************************************/
/**
 * @brief Color dependent sharpness
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 * @warning VGA/DVI/HDMI should turn off
 */
void vHalVideoLTIOnOff(UINT8 u1VidPath, UINT8 bOnOff)
{
    if (bIsScalerInput444(SV_VP_MAIN))
    {
        bOnOff = SV_OFF;
    }

    if (u1VidPath == SV_VP_MAIN)
    {
	    vDrvLTIOnOff(bOnOff);
    }
}

/**
 * @brief LTI
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 * @warning VGA/DVI/HDMI should turn off
 */
void vHalVideoLTIParam(UINT8 u1VidPath)
{
    if (bIsScalerInput444(SV_VP_MAIN))
    {
        vDrvLTIOnOff(SV_OFF);
        return;
    }

    if (u1VidPath == SV_VP_MAIN)
    {
	    vDrvLTIParam();
    }
}

/*****************************************************************************************/
/***********************************   RGB GAIN   ****************************************/
/*****************************************************************************************/

/**
 * @brief set RGB gain
 * @param bRGB SV_RGB_R/SV_RGB_G/SV_RGB_B
 * @param bValue gain value
 */
void vHalVideoRGBGain(UINT8 bPath, UINT8 bRGB, UINT16 wValue)
{
    switch (bRGB)
    {
        case SV_RGB_R:
            vDrvSetRGain1(bPath, wValue);
            break;
        case SV_RGB_G:
            vDrvSetGGain1(bPath, wValue);
            break;
        case SV_RGB_B:
            vDrvSetBGain1(bPath, wValue);
            break;
        default:
            ASSERT(0);
            break;
    }
}

/*****************************************************************************************/
/**********************************   RGB OFFSET   ***************************************/
/*****************************************************************************************/
/**
 * @brief Adjust RGB offset
 * @param bRGB SV_RGB_R/SV_RGB_G/SV_RGB_B
 * @param bValue Gain value to set
 * @warning Not used yet!!!
 */
void vHalVideoRGBOffset(UINT8 bPath, UINT8 bRGB, UINT16 wValue)
{
    wValue = DRVCUST_RGBOfstRemap(wValue);
    switch (bRGB)
    {
        // 10 bit RGB offset
        case SV_RGB_R:
            vDrvSetROffset11(bPath, (wValue));
            break;
        case SV_RGB_G:
            vDrvSetGOffset11(bPath, (wValue));
            break;
        case SV_RGB_B:
            vDrvSetBOffset11(bPath, (wValue));
            break;
        default:
            ASSERT(0);
            break;
    }
}

/*****************************************************************************************/
/**********************************   RGB Gain/OFFSET   **********************************/
/*****************************************************************************************/
/**
 * @brief Apply RGB gain/offset at the same time from quality table
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @warning Not used yet!!!
 */
void vHalVideoRGBGainOfstFromQtyTbl(UINT8 bPath)
{
    vHalVideoRGBGain(bPath, SV_RGB_R, wReadQualityTable(QUALITY_R_GAIN));
    vHalVideoRGBGain(bPath, SV_RGB_G, wReadQualityTable(QUALITY_G_GAIN));
    vHalVideoRGBGain(bPath, SV_RGB_B, wReadQualityTable(QUALITY_B_GAIN));
    vHalVideoRGBOffset(bPath, SV_RGB_R, wReadQualityTable(QUALITY_R_OFFSET));
    vHalVideoRGBOffset(bPath, SV_RGB_G, wReadQualityTable(QUALITY_G_OFFSET));
    vHalVideoRGBOffset(bPath, SV_RGB_B, wReadQualityTable(QUALITY_B_OFFSET));    
}

void vHalOutRGBColorOnOff(UINT8 bOnOff)
{
    vScpipOutRGBColorOnOff(bOnOff);
}

void vHalOutRGBColor(UINT16 RValue, UINT16 GValue, UINT16 BValue)
{
    vScpipOutRGBColor(RValue, GValue, BValue);
}

void vHalVideoSetRGBGain(UINT8 bPath, UINT16 u2RGain, UINT16 u2GGain, UINT16 u2BGain)
{
    wWriteQualityTable(QUALITY_R_GAIN, u2RGain);
    wWriteQualityTable(QUALITY_G_GAIN, u2GGain);
    wWriteQualityTable(QUALITY_B_GAIN, u2BGain);
    vApiRegisterVideoEvent(PE_EVENT_RGB_GAIN, bPath, SV_ON);
}

void vHalVideoSetRGBOfst1(UINT8 bPath, UINT16 u2ROfst, UINT16 u2GOfst, UINT16 u2BOfst)
{
    wWriteQualityTable(QUALITY_R_OFFSET, u2ROfst);
    wWriteQualityTable(QUALITY_G_OFFSET, u2GOfst);
    wWriteQualityTable(QUALITY_B_OFFSET, u2BOfst);
    vApiRegisterVideoEvent(PE_EVENT_RGB_OFST1, bPath, SV_ON);
}

void vHalVideoSetRGBOfst2(UINT8 bPath, UINT16 u2ROfst, UINT16 u2GOfst, UINT16 u2BOfst)
{
    _u2ROfst2 = u2ROfst;
    _u2GOfst2 = u2GOfst;
    _u2BOfst2 = u2BOfst;
    vApiRegisterVideoEvent(PE_EVENT_RGB_OFST2, bPath, SV_ON);
}

void vHalVideoSetXVRGBGain(UINT8 bPath, const UINT16* u2RgbGain)
{
    _u2XVRgbGain[0] = u2RgbGain[0];
    _u2XVRgbGain[1] = u2RgbGain[1];
    _u2XVRgbGain[2] = u2RgbGain[2];    
    vApiRegisterVideoEvent(PE_EVENT_XVRGB_GAIN, bPath, SV_ON);
}

void vHalVideoSetXVYCC(UINT8 bPath, UINT8 u1OnOff, UINT8 u1GammaOnOff, const UINT16* u2Matrix)
{
    _u1XVGammaOnOff = u1OnOff;
    _u1XVOnOff = u1GammaOnOff;
    x_memcpy(&_u2XVMatrix, u2Matrix, sizeof(_u2XVMatrix));   

    //printf("vHalVideoSetXVYCC [%d] %d %d \n", bPath, u1OnOff, u1GammaOnOff);
    vApiRegisterVideoEvent(PE_EVENT_XVYCC, bPath, SV_ON);
}

/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
/**
 * @brief select gamma mode
 * @param bMode SV_OFF/SV_ON
 */
void vHalVideoGammaLoadTable(UINT8 bMode)
{
#if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM
    if (GAMMA_IN_DRAM)
    {
        switch (bMode)
        {
        case 1:
        case 2:
        case 3:
            x_memcpy(&GAMMA_256[0][0], &_rGammaTbl[bMode-1][0], 3*GAMMA_INDEX);
            break;
        default:
            break;
        }
    }
#endif

    vDrvGammaDecodeTable();
    // change note : only gamma write table procedure in vsync
    vApiRegisterVideoEvent(PE_EVENT_GAMMA, SV_VP_MAIN, SV_ON);
}

#define ENABLE_GAMMA_DELTA_FUNC 0

#if ENABLE_GAMMA_DELTA_FUNC
void vHalSetGammaSaveDelta(DRV_CUSTOM_GAMMA_DELTA_T *pv_set_info)
{
		DRVCUST_SetGammaDelta( SV_RGB_R,
				(INT16)pv_set_info->ui2_delta_r[0]-0x80,
				(INT16)pv_set_info->ui2_delta_r[1]-0x80,
				(INT16)pv_set_info->ui2_delta_r[2]-0x80,
				(INT16)pv_set_info->ui2_delta_r[3]-0x80);
		DRVCUST_SetGammaDelta( SV_RGB_B,
				(INT16)pv_set_info->ui2_delta_b[0]-0x80,
				(INT16)pv_set_info->ui2_delta_b[1]-0x80,
				(INT16)pv_set_info->ui2_delta_b[2]-0x80,
				(INT16)pv_set_info->ui2_delta_b[3]-0x80);
		DRVCUST_SetGammaLevel(
				pv_set_info->ui2_green_level[0],
				pv_set_info->ui2_green_level[1],
				pv_set_info->ui2_green_level[2],
				pv_set_info->ui2_green_level[3]);
}
#endif

void vHalVideoGammaTblInit(void)
{
    SET_GAMMA_IN_DRAM(0);

#if defined(DRV_SUPPORT_MT8283)
    UINT32 i;
    for (i = 1; i <= 3; i++)
    {
        UINT8 fgOK = 0;
        #if (SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM || SUPPORT_EXTMJC_SEND_GAMMA_TABLE)
        	vApiFlashPqSetGammaId(i);
            fgOK = bApiFlashPqUpdateGamma();
        #endif

        if (fgOK)
        {
        #if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM
            x_memcpy(&_rGammaTbl[i-1][0], &GAMMA_256[0][0], 3*GAMMA_INDEX);
        #endif
        
        #ifdef DRV_SUPPORT_MT8283
            #if (SUPPORT_EXTMJC_SEND_GAMMA_TABLE)
                drv_extmjc_gamma_write(&GAMMA_256[0][0], 3*GAMMA_INDEX, i-1);
            #endif
        #endif
        }
        else
        {
            LOG(0, "Gamma Table Initialized from FlashPQ fails. Use default table\n");
            return;
        }
    }

    if (EXTMJC_SW_TO_FRONT == 0)
    {
        // use 8283 gamma, so set linear gamma in scaler chip
        for (i = 0; i < GAMMA_INDEX; i++)
        {
            GAMMA_256[0][i] = (i*16) & 0xFF;
            GAMMA_256[1][i] = (i*16) & 0xFF;
            GAMMA_256[2][i] = (i*16) & 0xFF;
            vHalVideoGammaLoadTable(SV_ON);
        }
    }
#else
    #if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM
    UINT8 i;
    for (i = 1; i <= FLASH_PQ_GAMMA_NUM; i++)
    {
    	vApiFlashPqSetGammaId(i);
        if (bApiFlashPqUpdateGamma())
        {
            x_memcpy(&_rGammaTbl[i-1][0], &GAMMA_256[0][0], 3*GAMMA_INDEX);
        }
        else
        {
            LOG(0, "Gamma Table Initialized from FlashPQ fails. Use default table\n");
            return;
        }
    }
    #endif
#endif

    SET_GAMMA_IN_DRAM(SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM);
}

#ifdef DRV_SUPPORT_EXTMJC
void vHalVideoExternalGammaInit(void)
{
#if 0
	//UINT8 bData, bChannel;
    UINT32 ui4_start_time;
    UINT8 *pDark[3],*pMiddle[3],*pBright[3];

    ui4_start_time = x_os_get_sys_tick();
	//CRIT_STATE_T csState;
    pDark[0]=GAMMA_1024_DARK[SV_RGB_R];
    pDark[1]=GAMMA_1024_DARK[SV_RGB_G];
    pDark[2]=GAMMA_1024_DARK[SV_RGB_B];
    pMiddle[0]=GAMMA_1024_MIDDLE[SV_RGB_R];
    pMiddle[1]=GAMMA_1024_MIDDLE[SV_RGB_G];
    pMiddle[2]=GAMMA_1024_MIDDLE[SV_RGB_B];
    pBright[0]=GAMMA_1024_BRIGHT[SV_RGB_R];
    pBright[1]=GAMMA_1024_BRIGHT[SV_RGB_G];
    pBright[2]=GAMMA_1024_BRIGHT[SV_RGB_B];
    drv_extmjc_gamma_write(pDark,pMiddle,pBright,1024);
    drv_extmjc_set_gamma(SV_V_GAMMA_DARK, NULL);

    Printf( "\r\n vHalVideoExternalGammaInit() cost %u ms\r\n\r\n", (x_os_get_sys_tick()-ui4_start_time)*5 );
#endif
}
#endif

void vApiSetStaticGamma(UINT32 u4Index)
{
    DRVCUST_PANEL_GAMMA_REMAP(u4Index);
}


/**
 * @brief enable/disable white peak limit
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoWhitePeakLimit(UINT8 bOnOff)
{
    vDrvWhitePeakLimit(bOnOff);
}

/*****************************************************************************************/
/************************************  Noise Reduction  **********************************/
/*****************************************************************************************/
void vHalVideoAutoNROnOff(UINT8 bOnOff)
{

}

void vHalVideo2DNRParam(UINT8 bCurUI)
{
    vDrv2DNRParam(bCurUI);
}

void vHalVideoMPEGNRParam(UINT8 bCurUI)
{
    vDrvMPEGNRParam(bCurUI);
}

void vHalVideo3DNRParam(void)
{
    vDrv3DNRParam();
}

void vHalVideoIRRParam(void)
{
    vDrvIRRSetOnOff(SV_ON);    
    vDrvIRRLoadParam();
    
    if(bIsScalerInput444(SV_VP_MAIN))    
    {
	vDrvIRRSetOnOff(SV_OFF);
       return;
    }    
}

/***************************************************************************/
/********************************** Video level extend**********************/
/***************************************************************************/

/**
 * @brief Extend video level to max.
 * @param u1VidPath SV_VP_MAIN/SV_VP_PIP
 * @param fgIsModeChange SV_TRUE/SV_FALSE
 */
void vHalVideoExtLevel(UINT8 u1VidPath, BOOL fgIsModeChange)
{
    UINT16 u2Gain, u2Offset;

    u2Gain = VIDEO_GAIN_DFT;
    u2Offset = VIDEO_OFST_DFT;

    // On mode change done, use one time setting to extend video level.
    if (fgIsModeChange == SV_TRUE)
    {
        // Use output stage 3x3 matrix to extend video level since there is only one set of RGB gain/offset
        u2Gain = VIDEO_GAIN_DFT;
        u2Offset = VIDEO_OFST_DFT;
    }
    else
    {
        // Only apply dynamic RGB gain when adaptive backlight is active.
        if (bBackLightCtrl == SV_OFF)
        {
        	return;
        }
    }

    vDrvSetRGain1(u1VidPath, u2Gain);
    vDrvSetGGain1(u1VidPath, u2Gain);
    vDrvSetBGain1(u1VidPath, u2Gain);
    vDrvSetROffset11(u1VidPath, u2Offset);
    vDrvSetGOffset11(u1VidPath, u2Offset);
    vDrvSetBOffset11(u1VidPath, u2Offset);
}

void vHalHDMIBlackControl(UINT8 bOnOff)
{
    if ((_rMChannel.bIs444 == SV_TRUE) && (fgIsVgaTiming(_bDviTiming)))
    {
        if (bOnOff != SV_TRUE)
        {
            // RGB 0 ~ 255
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, 0x80);
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, 0x80);
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, 0x80);

            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R, 0);
            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G, 0);
            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B, 0);
        }
        else
        {
            // RGB 16 ~ 235
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, 0x5B);
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, 0x5B);
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, 0x5B);

            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R, 0x10);
            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G, 0x10);
            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B, 0x10);
        }
    }
    else
    {
        LOG(9, "Not Dvi timing RGB444\n");
    }
}

/**************************************************************************************/
/***********************************Bypass All PQ Item****************************************/
/**************************************************************************************/

void vVideoAllPQItemOnOff(UINT8 bOnOff)
{
    PE_ARG_TYPE i;

    vHalVideoCTIOnOff(SV_VP_MAIN, bOnOff);
    vHalVideoLTIOnOff(SV_VP_MAIN, bOnOff);
    vHalVideoSharpnessOnOff(SV_VP_MAIN, bOnOff);
    vHalVideoBlueStretchOnOff(bOnOff);
    vDrvSCEAllOnOff(bOnOff);
    vDrvGammaOnOff(bOnOff);

    if (bOnOff)
    {
        vDrvNRBypass(SV_OFF);
        for (i = PE_ARG_R_GAIN; i <= PE_ARG_B_OFFSET; i++)
        {
            bApiVideoProc(SV_VP_MAIN, i);
        }
    }
    else
    {
        vDrvNRBypass(SV_ON);

        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, 0x200);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, 0x200);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, 0x200);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R ,0);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G ,0);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B ,0);
    }
}

static BOOL _bVerfPETable(void)
{
	UINT16 i;
	BOOL RetVal = SV_TRUE;
		
	for(i=0; i < QUALITY_MAX; i++)
	{
		if(aDefaultQtyTbl[i].wQtyItem != i)
		{
			RetVal = SV_FALSE;
			LOG(0, "aDefaultQtyTbl[%d].wQtyItem = %d was wrong\n", 
				i, aDefaultQtyTbl[i].wQtyItem);
		}
	}	

    LOG(0, "===> Verify PE Table %d\n", RetVal);

	return RetVal;
}

// =======================================================================
// Init FlashPQ
// call from PE_HwInit() 
// =======================================================================
void vApiVideoPeInit(void)
{
    _bVerfPETable();
    
    // Restore quality table from flash if support FlashPQ function.
    if (bApiFlashPqVerifyHeader() == SV_TRUE)
    {
        // If not support SmartPic, update QtyTable at boot-up. Otherwise, it will be done at each mode change done.
        if (DRVCUST_OptGet(eFlagFlashPqEnableQtySmartPic) == SV_FALSE)
        {
            if (bApiFlashPqUpdateQtyDft() == SV_FALSE)
            {
                LOG(0, "Restore FlashPQ Qty fail\n");
            }
        }
    }
    
    UNUSED(bApiParseCustomQtyItem());

    vHalVideoGammaTblInit();     
    
#ifdef DRV_SUPPORT_EXTMJC
     if (IS_COMPANION_CHIP_ON())
     {
         vHalVideoExternalGammaInit();
     }
#endif
     
     // Restore quality table from EEPROM if support factory mode.
     UNUSED(bApiSetQtyTblDft());
 
     // Load the SCE table
     vHalVideoSCELoadTable(SV_VP_MAIN);
 
     // force gamma apply in vsync even in system boot.
     if(DRVCUST_OptGet(eNVMGamma))
     {
         DRVCUST_SET_GAMMA(); //Read Gamma from EEPROM
     }
     else
     {
         //vApiRegisterVideoEvent(PE_EVENT_GAMMA, SV_VP_MAIN, SV_ON);
         vHalVideoGammaLoadTable(SV_ON);
     }
 
    // initial PQ setting
    vDrvSetBrightness(SV_VP_MAIN, 0x80);
    vDrvSetContrast(SV_VP_MAIN, 0x80);
    vDrvSetSaturation(SV_VP_MAIN, 0x80);
    vDrvSetHue(SV_VP_MAIN, 0x80);
    vDrvSetBrightness(SV_VP_PIP, 0x80);
    vDrvSetContrast(SV_VP_PIP, 0x80);
    vDrvSetSaturation(SV_VP_PIP, 0x80);
    vDrvSetHue(SV_VP_PIP, 0x80);
    
    // Adaptive control Init
    vDrvAdaptiveFlgInit();
    vDrvAdaptivePSCANFlgInit();
}

// =======================================================================
// Init PE HW Register
// call from vDrvVideoHwInit() in vdo_misc.c
// =======================================================================
void PE_Init()
{
#ifdef CC_SCPOS_3DTV_SUPPORT        
    vDrvTDTVInit();
#endif

	// PSCAN Init
	vDrvDIInit(VDP_1);
	vDrvDIInit(VDP_2);

	vDrvTDCInit();
	vDrvNRInit();
	vDrvYcprocInit();
	vDrvDBLInit(); 

#ifdef CC_MT5395
	vDrvMJCInit();
#endif

	vDrvODInit();
	vApiVideoPeInit();
}

EXTERN UINT32 SW_DRAM_BASE;

#if defined(CC_FAST_INIT)
void PE_pm_resume(void)
{
	vRegWrite4B(0x2700, SW_DRAM_BASE);
	PE_Init();
}
#endif

// =======================================================================

