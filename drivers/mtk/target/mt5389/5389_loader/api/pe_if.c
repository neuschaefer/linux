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
#include "drv_lcdim.h"
#include "drv_di.h"
#include "drv_display.h"
#include "drv_scaler.h"
#if defined(CC_MT5396)
#include "drv_mjc.h"
#endif
#ifdef DRV_SUPPORT_MT8283
#include "extmjc_if.h"
#endif
#ifdef CUSTOM_VIDEO_DRV_INCLUDE_FILE
	#include CUSTOM_VIDEO_DRV_INCLUDE_FILE
#endif // #ifndef CUSTOM_VIDEO_DRV_INCLUDE_FILE


extern void _vMappingSrcTiming(void); //Add for vestel --- pingan 0606
// === DEFINE =============================================================================
#define GAMMA_INDEX 257

#ifndef SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM
    #define SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM    (0)
#endif

#ifndef FLASH_PQ_GAMMA_NUM
    #define FLASH_PQ_GAMMA_NUM                  (3)
#endif

#ifndef SUPPORT_EXTMJC_SEND_GAMMA_TABLE
    #define SUPPORT_EXTMJC_SEND_GAMMA_TABLE     (1)
#endif

#define SET_FLASH_PQ_READY(fgReady) vIO32WriteFldAlign(FLASH_PQ_00, ((fgReady)?1:0), FLASH_PQ_READY)

// === FUNCTION PROTOTYPE =================================================================
static void vHalVideoGammaTblInit(void);

// === EXTERN FUNCTION ====================================================================
extern UINT16 u2ScpipGetDispWidth(UINT8 bPath);
extern UINT16 u2ScpipGetDispHeight(UINT8 bPath); 

// === GLOBAL VARIABLE ====================================================================
UINT8 QUALITY_TBL[QUALITY_MAX];

// === EXTERN VARIABLE ====================================================================
extern UINT8 _bDviTiming;
extern const tagDFT_QTY  CODE aDefaultQtyTbl[QUALITY_MAX];
extern UINT8 GAMMA_256[3][GAMMA_INDEX];
extern INT32 const YCBCR2RGBOS[3][15];
extern INT32 COLOR_TRANSFORM_ADJ[15];

extern const FLASH_PQ_SW_INFO aFlashPqSWInfo[FLASHPQ_NUM];
extern const FLASH_PQ_FLASH_INFO aFlashPqFlashInfo[FLASHPQ_NUM];

// === STATIC VARIABLE ====================================================================
static UINT8 bRegisterList[PE_EVENT_NS][REGISTER_DIM];
static UINT8 u1AdaptiveBackLightPWM[ADP_BACKLIGHT_END_IDX];

#if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM 
static UINT8 _rGammaTbl[FLASH_PQ_GAMMA_NUM][3*GAMMA_INDEX];
#endif

#ifndef NDEBUG    
NPTV_LOG_T LogConfig[8];
UINT32 u4IsrLogEn= SV_FALSE;
#endif

// === Function ============================================================================

UINT8 vDrvVerfPETable(void)
{
	UINT16 i;
	UINT8 RetVal = SV_TRUE;
		
	for(i=0; i < QUALITY_MAX; i++)
	{
		if(aDefaultQtyTbl[i].wQtyItem != i)
		{
			RetVal = SV_FALSE;
			LOG(0, "aDefaultQtyTbl[%d].wQtyItem = %d was wrong\n", 
				i, aDefaultQtyTbl[i].wQtyItem);
		}
	}	

	if(RetVal)
	{
		LOG(0, "Verf PE Table Passed\n");
	}

	return RetVal;
}

void vApiGammaInit(void)
{
    vHalVideoGammaTblInit();
    vHalVideoGammaTblUpdate(0);
    vHalVideoGammaLoadTable();

#ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        vHalVideoExternalGammaInit();
    }
#endif
}

void vApiVideoInitSmartPicDft(void)
{
    INT16 u1PicMode;
    UINT8 i;
    
    if(DRVCUST_OptGet(eFlagFlashPqEnableQtySmartPic))
    {
        for(u1PicMode = 0; u1PicMode < PICTURE_MODE_MAX-1; u1PicMode++)  //temp modify
        {
            vApiFlashPqSetSmartPicId(u1PicMode);
            for (i=0; i<FLASHPQ_NUM ; i++)
            {
                bApiFlashPqUpdateQtyDft(i);
            }
            
            vApiSetUiDftRange(u1PicMode);
        }
    }
}

void vApiVideoInitSmartPicMinMax(void)
{
    INT16 u1SrcTypTmg;
    
    if(DRVCUST_OptGet(eFlagFlashPqEnableQtySmartPic))
    {
        for(u1SrcTypTmg = 0; u1SrcTypTmg < FLASHPQ_TOTAL_TIMING; u1SrcTypTmg++)
        {
            bApiFlashPqUpdateQtyMinMaxAllSrc(SV_VP_MAIN, 0, u1SrcTypTmg);
            vApiSetUiMinMaxRange(u1SrcTypTmg);
        }
    }    
}

//Caihua 20120709
void vApiVideoInitBacklightDft(void)
{
    INT16 u1PicMode;
    UINT8 i;

    if(DRVCUST_OptGet(eFlagFlashPqEnableQtySmartPic))
    {
        for(u1PicMode = 0; u1PicMode < PICTURE_MODE_MAX-1; u1PicMode++)  //temp modify
        {
            vApiFlashPqSetSmartPicId(u1PicMode);
            for (i=0; i<FLASHPQ_NUM ; i++)
            {
                bApiFlashPqUpdateQtyDft(i);
            }
            
            vApiSetBacklightDft(u1PicMode);
        }
    }
}

void vApiVideoPeInit(void)
{
    UINT8 u1FlashPQReady = SV_TRUE;
    UINT8 bIndex;
    
    vDrvVerfPETable();

    // Restore quality table from flash if support FlashPQ function.
    for(bIndex=0; bIndex<FLASHPQ_NUM; bIndex++)           //------------------------
    {
        if (bApiFlashPqVerifyHeader(bIndex) == SV_TRUE)
        {
            // If not support SmartPic, update QtyTable at boot-up. Otherwise, it will be done at each mode change done.
            //Prevent 1st time boot up mode change done before flashPQ valued loaded.   

            if (DRVCUST_OptGet(aFlashPqSWInfo[bIndex].dwSwQtySmartPicEnable) == SV_FALSE) 
            {
                if (bApiFlashPqUpdateQtyDft(bIndex) == SV_FALSE)
                {
	                LOG(0, "Restore FlashPQ[%d] Qty fail\n",bIndex);
	            }
                else
                {
                    u1FlashPQReady = SV_TRUE; //any one flashPQ is true, set  u1FlashPQReady = true
                }
	        }
            else //if smart Picture, don't check bApiFlashPqUpdateQtyDft, set the u1FlashPQReady to True directly
            {
                u1FlashPQReady = SV_TRUE;
            }
        }
        else
        {
            //u1FlashPQReady = SV_FALSE;
            LOG(0, "FlashPQ<%d> verify Header fail\n", bIndex);
        }
    }
    
    SET_FLASH_PQ_READY(u1FlashPQReady);

    
    bApiParseCustomQtyItem();
    vApiVideoInitSmartPicDft();     //Add for vestel --- pingan 0606
    //_vMappingSrcTiming();           //Add for vestel --- pingan 0606
    vApiVideoInitSmartPicMinMax();  //Add for vestel --- pingan 0606
    //Caihua 20120709
    vApiVideoInitBacklightDft();

    // Set all UI video quality items' current to default value.---pingan 120712
    vApiUpdateUIDefaultRange(0,0);
    PE_SetAllUiVqItemDftValues();

    //bApiParseCustomQtyItem();     //todo vestel
    bApiSetPWMDft(); // Set the default value for PWM value.
    vDrvSCELoadTable();

    // Init Gamma
    vApiGammaInit();
    
    //init OD
    vDrvODInit();
    vApiRegisterVideoEvent(PE_EVENT_OD, SV_VP_MAIN, SV_ON);
}

void vApiRegisterVideoEvent(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff)
{
#if defined(CC_MT5396)    
    if (bVdpArg < PE_EVENT_BEFORE_SCALER)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_INPUT_VSYNC_ISR);
    }
    else if ((bVdpArg < PE_EVENT_BEFORE_MJC) && (u1DrvMJCIsOnOff() == SV_ON))
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_MJC_INPUT_VSYNC_ISR);
    }
    else if (bVdpArg == PE_EVENT_OD)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OD_VSYNC_ISR);
    }
    else if (bVdpArg == PE_EVENT_DITHER)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);
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
    else if (bVdpArg == PE_EVENT_OD)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OD_VSYNC_ISR);
    }
    else if (bVdpArg == PE_EVENT_DITHER)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);
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

void vApiApplyVideoEvent(UINT8 bApplyTime)
{
    UINT8 bVdpArg;

    for (bVdpArg = 0; bVdpArg < PE_EVENT_NS; bVdpArg++)
    {
        if (bRegisterList[bVdpArg][REGISTER_CALLBACK] == SV_ON 
            && bRegisterList[bVdpArg][REGISTER_APPLY_TIME] == bApplyTime)
        {
            switch (bVdpArg)
            {
                case PE_EVENT_LUMA_CURVE:
                    vDrvADLUpdateLumaCurve();
                    break;     
                case PE_EVENT_LUMA_SEC_CURVE:
                    vDrvADLUpdateSecondLumaCurve();
                    break;                         
                case PE_EVENT_SCE:
                    vDrvSCELoadTable();
                    break;    				
                case PE_EVENT_SHARPNESS:
    		        vDrvLoadYLevTable();
                    break;
                case PE_EVENT_OD:
                    vDrvODSetTable();
                    break; 
#ifdef CC_MT5396    
                case PE_EVENT_OUT_3x3:
                    vDrvGlobalPQAdj();
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
                case PE_EVENT_BLUE_STRETCH:
                    vHalVideoBlueStretchOnOff(bRegisterList[bVdpArg][REGISTER_PATH], bRegisterList[bVdpArg][REGISTER_ON_OFF]);
                    vDrvSetBlueStretchParam();        
                    break;           
#endif  
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
void vApiVideoDemoMode(UINT8 bmode, Region DemoRec, UINT8 bDEMOModules)
{
    Region DemoRegion;
    UINT8 bSplitOnOff, bMovingOnOff;
    UINT8 bRegion=0;
    UINT16 wDispCenter;
    UINT16 wInputWidth = PANEL_GetPanelWidth();//u2DrvDIGetWidth();
    UINT16 wDispWidth = PANEL_GetPanelWidth();//u2ScpipGetDispWidth(SV_VP_MAIN);
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
    vDrvDIDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_MDDI_EN)), bRegion,(wInputWidth/2)+0x44);   // 0x44 is for MEMA Demo position alignment. why 0x44? just test the position, and got this difference value.
    vDrvNRSet2DNRDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_2DNR_EN)), bRegion, (wInputWidth/2));
    vDrvNRSet3DNRDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_3DNR_EN)), bRegion, (wInputWidth/2));

    //Rectangular demo
    vDrvSetPostSharpDemoRange(DemoRegion);
    
    vDrvDemoSetRegion(DemoRegion);
    vIO32WriteFldAlign(DEMO_00, bMovingOnOff, MOVINGEMO_ENABLE);
}

/*****************************************************************************************/
/*********************************      USPEI     *****************************************/
/*****************************************************************************************/
void vHalVideoUSPEIOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if(bIsScalerInputRGB(bPath))
    {	//turn off 2d edge smooth
        vDrvUsPeiOnOff(SV_OFF);
    }
    else
    {
    	//reload initial table
    	vDrvUsPeiInit();
    }
}

/*****************************************************************************************/
/**********************************  Sharpness  ******************************************/
/*****************************************************************************************/
void vHalVideoSharpnessOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if (bPath == SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bOnOff = SV_OFF;
		}	

        vDrvPostSharpOnOff(bOnOff);
	}
}

void vHalVideoSharpnessHorizontalOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if (bPath == SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bOnOff = SV_OFF;
		}	

        vDrvPostSharpHorizontalOnOff(bOnOff);
	}
}

void vHalVideoSharpnessVerticalOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if (bPath == SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bOnOff = SV_OFF;
		}	

        vDrvPostSharpVerticalOnOff(bOnOff);
	}
}

void vHalVideoSharpnessDiagonalOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if (bPath == SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bOnOff = SV_OFF;
		}	
        
       	vDrvPostSharpDiagonalOnOff(bOnOff);
	}
}

void vHalVideoSharpnessParam(UINT8 bPath)
{
	if (bPath == SV_VP_MAIN)
	{    
        if (bIsScalerInputRGB(bPath))
        {		
            vDrvPreSharpOnOff(bPath, SV_OFF);
			vDrvPostSharpOnOff(SV_OFF);
            return;
        }
        
        vDrvPreSharpParam(bPath);
	    vDrvPostSharpParam();
	}
}

void vHalVideoSharpnessParamHorizontal(UINT8 bPath)
{
	if (bPath == SV_VP_MAIN)
	{
        if (bIsScalerInputRGB(bPath))
        {
            vDrvPostSharpOnOff(SV_OFF);
            return;
        }
        
	    vDrvPostSharpParamHorizontal();
	}
}

void vHalVideoSharpnessParamVertical(UINT8 bPath)
{
	if (bPath == SV_VP_MAIN)
	{
        if (bIsScalerInputRGB(bPath))
        {
            vDrvPostSharpOnOff(SV_OFF);
            return;
        }
        
	    vDrvPostSharpParamVertical();
	}
}

void vHalVideoSharpnessParamDiagonal(UINT8 bPath)
{
	if (bPath == SV_VP_MAIN)
	{
        if (bIsScalerInputRGB(bPath))
        {
            vDrvPostSharpOnOff(SV_OFF);
            return;
        }
        
	    vDrvPostSharpParamDiagonal();
	}
}

void vHalVideoCBEParam(UINT8 bPath)
{
	#ifdef CC_MT5396
	if (bPath == SV_VP_MAIN)
	{
        vDrvCBELoadParam();	
	}
	#endif
}

void vHalVideoCBEOnOff(UINT8 bPath, UINT8 bOnOff)
{
	#ifdef CC_MT5396
	if (bPath == SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bOnOff = SV_OFF;
		}	
        
        vDrvCBEOnOff(bOnOff);
	}
	#endif
}

/*****************************************************************************************/
/********************************  Super Resolution   ************************************/
/*****************************************************************************************/
void vHalVideoSuperResolutionOnOff(UINT8 bPath, UINT8 bOnOff)
{
#if defined(CC_MT5396)
	if (bPath == SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bOnOff = SV_OFF;
		}	
        
		vDrvCBEOnOff(bOnOff);
		vDrvUsPeiOnOff(bOnOff);	
	}
#endif
}

/*****************************************************************************************/
/**********************************  ADAPTIVE LUMA  ***************************************/
/*****************************************************************************************/
void vHalVideoAdaptiveLumaOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if(bIsScalerInputRGB(bPath))
    {
    	bOnOff = SV_OFF;
    }
    
	if(bPath==SV_VP_MAIN)
	{
		vDrvADLOnOff(bOnOff);
	}
}

void vHalVideoAdaptiveLumaParam(void)
{
    vDrvADLUpdateParam();
}

/*****************************************************************************************/
/****************************   ADAPTIVE BACKLIGHT    ************************************/
/*****************************************************************************************/
void vHalSetAdpBacklight(UINT8 u1Level, UINT8 u1Value)
{
    u1AdaptiveBackLightPWM[u1Level] = u1Value;
}

UINT8 bHalGetAdpBacklight(UINT8 u1Level)
{
    return u1AdaptiveBackLightPWM[u1Level];
}

/*****************************************************************************************/
/********************************  LOCAL DIMMING    **************************************/
/*****************************************************************************************/
void vHalVideoLcDimOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if (bPath == SV_VP_MAIN)
	{
		vDrvSetLcDimOnOff(bOnOff);
	}
}

/*****************************************************************************************/
/***********************************   CONTRAST   ****************************************/
/*****************************************************************************************/
void vHalVideoContrast(UINT8 bPath, UINT16 u2Value)
{
    vDrvSetContrast(bPath, u2Value);
}

void vHalTdtvVideoContrast(UINT8 bPath, UINT16 u2Value)  //Add for vestel --- pingan 0606
{
    vDrvSetTdtvContrast(bPath, u2Value);
}
/*****************************************************************************************/
/**********************************   BRIGHTNESS   ***************************************/
/*****************************************************************************************/
void vHalVideoBrightness(UINT8 bPath, UINT8 bValue)
{
    vDrvSetBrightness(bPath, bValue);
}

void vHalTdtvVideoBrightness(UINT8 bPath, UINT8 bValue)  //Add for vestel --- pingan 0606
{
    vDrvSetTdtvBrightness(bPath, bValue);
}

/*****************************************************************************************/
/**********************************   SATURATION   ***************************************/
/*****************************************************************************************/
void vHalVideoSaturation(UINT8 bPath, UINT16 u2Value)
{
    //if(fgDrvGetAutoNROnOff() && (bPath == SV_VP_MAIN))  //TODO => add or remove ?
    //{
    //    u2Value = (u2Value <= bReducedSaturation) ? 
    //        0 : (u2Value - bReducedSaturation);
    //}
    vDrvSetSaturation(bPath, u2Value);
}

void vHalTdtvVideoSaturation(UINT8 bPath, UINT16 u2Value)  //Add for vestel --- pingan 0606
{
    //if(fgDrvGetAutoNROnOff() && (bPath == SV_VP_MAIN))  //TODO => add or remove ?
    //{
    //    u2Value = (u2Value <= bReducedSaturation) ? 
    //        0 : (u2Value - bReducedSaturation);
    //}
    vDrvSetTdtvSaturation(bPath, u2Value);
}
/*****************************************************************************************/
/*************************************   HUE   *******************************************/
/*****************************************************************************************/
void vHalVideoHue(UINT8 bPath, UINT8 bValue)
{
    vDrvSetHue(bPath, bValue);
}

void vHalTdtvVideoHue(UINT8 bPath, UINT8 bValue)  //Add for vestel --- pingan 0606
{
    vDrvSetTdtvHue(bPath, bValue);
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

void vHalVideoSCEOnOff(UINT8 bPath)
{
    if (bPath == SV_VP_MAIN)
    {
        vDrvSCEAllOnOff(!bIsScalerInputRGB(SV_VP_MAIN));
    }
}

void vHalSetSCEAngular(UINT8 bmodule, UINT8 bColor, UINT8 bValue)
{
    if (bmodule < SCE_MAX)
    {
        vDrvSCEAngularSetVal(bmodule, bColor, bValue);
    }
    else
    {
        vDrvSCEAngularReset();
    }
}

/**
 * @brief Set SCE pattern
 * @param bOnOff SV_ON/SV_OFF
 * @param wYdata SCE pattern Y level
 * @return NONE
 * @retval void
 */
void vHalSetSCEPat(UINT8 bOnOff, UINT16 wYdata)
{
    static UINT8 bBSOnOff = SV_OFF;
    static UINT8 bxvOnOff = SV_OFF;
    INT32 COLOR_TRANSFORM_Y2R[15];

    vDrvSCEPatGen(bOnOff, wYdata);
    vDrvSCEAllOnOff(!bOnOff);
    if (bOnOff) 
    { 
        bBSOnOff = bDrvGetBlueStretchOnOff(); /* store latest setting of blue stretch*/
        if (bBSOnOff)
        {
            vDrvSetBlueStretchOnOff(SV_VP_MAIN, SV_OFF); /* bypass blue stretch */
        }

        bxvOnOff = bDrvGetxvYCCOnOff(); /* store latest setting of xvYCC*/
        if (bxvOnOff)
        {
            vDrvxvYCCOnOff(SV_OFF); /* bypass xvYCC */
        }
        
        x_memcpy(COLOR_TRANSFORM_Y2R, YCBCR2RGBOS[2], sizeof(COLOR_TRANSFORM_Y2R));
        vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_Y2R);
    }
    else
    { 
        if (bBSOnOff != bDrvGetBlueStretchOnOff())
        {
            vDrvSetBlueStretchOnOff(SV_VP_MAIN, bBSOnOff); /* restore user blue stretch setting */
        }
        
        if (bxvOnOff != bDrvGetxvYCCOnOff())
        {
            vDrvxvYCCOnOff(bBSOnOff); /* restore user blue stretch setting */
        }

        vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_ADJ);
    }	
}

/*****************************************************************************************/
/********************************** BLUE STRETCH  ****************************************/
/*****************************************************************************************/
void vHalVideoBlueStretchOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bIsScalerInputRGB(bPath))
    {
        bOnOff = SV_OFF;
    }
    vDrvSetBlueStretchOnOff(bPath, bOnOff);
}

/*****************************************************************************************/
/************************************   CTI   *******************************************/
/*****************************************************************************************/
void vHalVideoCTIOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {        
        if (bIsScalerInputRGB(bPath))
        {
            bOnOff = SV_OFF;
        }
        
        vDrvCTIFOnOff(bOnOff);
        vDrvCTIROnOff(bOnOff);
    }
}

void vHalVideoCTIParam(UINT8 bPath)
{
    if (bPath == SV_VP_MAIN)
    {        
        if (bIsScalerInputRGB(bPath))
        {
            vDrvCTIFOnOff(SV_OFF);
            vDrvCTIROnOff(SV_OFF);
            return;
        }
        
        vDrvCTIFParam();
        vDrvCTIRParam();
    }
}

/*****************************************************************************************/
/************************************   LTI   *******************************************/
/*****************************************************************************************/
void vHalVideoLTIOnOff(UINT8 u1VidPath, UINT8 bOnOff)
{
    if (u1VidPath == SV_VP_MAIN)
    {
        if (bIsScalerInputRGB(SV_VP_MAIN))
        {
            bOnOff = SV_OFF;
        }
	    vDrvLTIOnOff(bOnOff);
    }
}

void vHalVideoLTIParam(UINT8 u1VidPath)
{
    if (u1VidPath == SV_VP_MAIN)
    {
        if (bIsScalerInputRGB(SV_VP_MAIN))
        {
            vDrvLTIOnOff(SV_OFF);
            return;
        }
	    vDrvLTIParam();
    }
}

/*****************************************************************************************/
/***********************************   RGB GAIN OFFSET ***********************************/
/*****************************************************************************************/
void vHalVideoRGBGain(UINT8 bPath, UINT8 bRGB, UINT16 wValue)
{    
    if ((bPath != SV_VP_MAIN) && (bPath != SV_VP_PIP))
    {
        LOG(0, "vHalVideoRGBGain Path Error %d !", bPath);
        ASSERT(0);
    }
    
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

void vHalVideoRGBOffset(UINT8 bPath, UINT8 bRGB, UINT16 wValue)
{
    wValue = DRVCUST_RGBOfstRemap(wValue);

    if ((bPath != SV_VP_MAIN) && (bPath != SV_VP_PIP))
    {
        LOG(0, "vHalVideoRGBOffset Path Error %d !", bPath);
        ASSERT(0);
    }
    
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

void vHalVideoRGBOffset2(UINT8 bPath, UINT8 bRGB, UINT16 wValue)
{
    wValue = DRVCUST_RGBOfstRemap(wValue);
    
    if ((bPath != SV_VP_MAIN) && (bPath != SV_VP_PIP))
    {
        LOG(0, "vHalVideoRGBOffset2 Path Error %d !", bPath);
        ASSERT(0);
    }
    
    switch (bRGB)
    {
        // 10 bit RGB offset
        case SV_RGB_R:
            vDrvSetROffset12(bPath, (wValue));
            break;
        case SV_RGB_G:
            vDrvSetGOffset12(bPath, (wValue));
            break;
        case SV_RGB_B:
            vDrvSetBOffset12(bPath, (wValue));
            break;
        default:
            ASSERT(0);
            break;
    }
}

void vHalOutRGBColorOnOff(UINT8 bOnOff)
{
    vScpipOutRGBColorOnOff(bOnOff);
}

void vHalOutRGBColor(UINT8 RValue, UINT8 GValue, UINT8 BValue)
{
    vScpipOutRGBColor(RValue, GValue, BValue);
}

/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
void vHalVideoGammaLoadTable(void)
{
    vDrvGammaDecodeTable();

    // remapp for customer specification. [optional]
    DRVCUST_SET_GAMMA();

    vDrvSetGammaWritePingPongFlag(SV_TRUE);
}

void vHalVideoGammaTblInit(void)
{    
    #if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM    
    UINT32 i;    
    
    for (i = 0; i < FLASH_PQ_GAMMA_NUM; i++)
    {
        vApiFlashPqSetGammaId(i+1);

        if (bApiFlashPqUpdateGamma())
        {
            x_memcpy(&_rGammaTbl[i][0], &GAMMA_256[0][0], 3*GAMMA_INDEX);
            
            #if (defined(DRV_SUPPORT_MT8283) && SUPPORT_EXTMJC_SEND_GAMMA_TABLE)
                drv_extmjc_gamma_write(&GAMMA_256[0][0], 3*GAMMA_INDEX, i);
            #endif
        }
    }
    #endif
    
    // if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM == 1
    // then it will only read Gamma from Flash once
    // otherwise, every time when we load Gamma, it will read Flash
    SET_GAMMA_IN_DRAM(SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM);
}

void vHalVideoGammaTblUpdate(UINT8 u1Mode)
{         
#if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM
    if ((GAMMA_IN_DRAM) && (u1Mode >= 1) && (u1Mode <= FLASH_PQ_GAMMA_NUM))
    {
        x_memcpy(&GAMMA_256[0][0], &_rGammaTbl[u1Mode-1][0], 3*GAMMA_INDEX);
    }
    else
#endif        
    {
        vApiFlashPqSetGammaId(u1Mode);    
        bApiFlashPqUpdateGamma();
    }
}


#ifdef DRV_SUPPORT_EXTMJC
void vHalVideoExternalGammaInit(void)
{
//	//UINT8 bData, bChannel;
//    UINT32 ui4_start_time;
//    UINT8 *pDark[3],*pMiddle[3],*pBright[3];
//
//    ui4_start_time = x_os_get_sys_tick();
//	//CRIT_STATE_T csState;
//    pDark[0]=GAMMA_1024_DARK[SV_RGB_R];
//    pDark[1]=GAMMA_1024_DARK[SV_RGB_G];
//    pDark[2]=GAMMA_1024_DARK[SV_RGB_B];
//    pMiddle[0]=GAMMA_1024_MIDDLE[SV_RGB_R];
//    pMiddle[1]=GAMMA_1024_MIDDLE[SV_RGB_G];
//    pMiddle[2]=GAMMA_1024_MIDDLE[SV_RGB_B];
//    pBright[0]=GAMMA_1024_BRIGHT[SV_RGB_R];
//    pBright[1]=GAMMA_1024_BRIGHT[SV_RGB_G];
//    pBright[2]=GAMMA_1024_BRIGHT[SV_RGB_B];
//    drv_extmjc_gamma_write(pDark,pMiddle,pBright,1024);
//    drv_extmjc_set_gamma(SV_V_GAMMA_DARK, NULL);

//    Printf( "\r\n vHalVideoExternalGammaInit() cost %u ms\r\n\r\n", (x_os_get_sys_tick()-ui4_start_time)*5 );
}
#endif


/*****************************************************************************************/
/**************************************  White Peak  *************************************/
/*****************************************************************************************/
void vHalVideoWhitePeakLimit(UINT8 bOnOff)
{
    vDrvWhitePeakLimit(bOnOff);
}

/*****************************************************************************************/
/************************************  Noise Reduction  **********************************/
/*****************************************************************************************/
void vHalVideo2DAutoNROnOff(UINT8 bOnOff)
{
    vDrvNRSet2DNRAuto(bOnOff);
}

void vHalVideo3DAutoNROnOff(UINT8 bOnOff)
{
    vDrvNRSet3DNRAuto(bOnOff);
}

void vHalVideo2DNRParam(UINT8 bCurUI)
{
    vDrvNRSetSNRParam();
    vDrvNRSet2DNRSNROnOff(bCurUI);
}

void vHalVideo3DNRParam(UINT8 bCurUI)
{
    vDrvNRSet3DNRParam();
    vDrvNRSet3DNROnOff(bCurUI);
}

void vHalVideoMPEGNRParam(UINT8 bCurUI)
{
    vDrvNRSetMpegNRParam();
    vDrvNRSet2DNRBNROnOff(bCurUI);
    vDrvNRSet2DNRMNROnOff(bCurUI);
}

/******************************************************************************************/
/***********************************  Bypass PQ Item  *************************************/
/******************************************************************************************/
void vApiVideoAllPQItemOnOff(UINT8 bOnOff)
{
    PE_ARG_TYPE i;
        
    if (bOnOff)
    {
        for (i = PE_ARG_BRIGHTNESS; i < PE_ARG_NS; i++)
        {
            bApiVideoProc(SV_VP_MAIN, i);
        }
        vDrvSetDefaultMatrix(SV_VP_MAIN, bOnOff);        
    }
    else
    {        
        vHalVideoCTIOnOff(SV_VP_MAIN, bOnOff);
        vHalVideoLTIOnOff(SV_VP_MAIN, bOnOff);
        vHalVideoSharpnessOnOff(SV_VP_MAIN, bOnOff);
        vHalVideoCBEOnOff(SV_VP_MAIN, bOnOff);        
        vHalVideoBlueStretchOnOff(SV_VP_MAIN, bOnOff);
        vDrvSCEAllOnOff(bOnOff);
        vDrvGammaOnOff(bOnOff);
        vDrvSetLcDimOnOff(bOnOff);
        vDrvxvYCCOnOff(bOnOff);
        vDrvNRSetForceBypass(!bOnOff);
        
        vDrvODOnOff(bOnOff);
        vDrvODBypass(!bOnOff);
        
        vDrvDeRingOnOff(SV_VP_MAIN, bOnOff);
        vDrvUsPeiOnOff(bOnOff);
        //vDrvNRBypass(SV_OFF);
        vDrvSetDefaultMatrix(SV_VP_MAIN, bOnOff);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, 0x200);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, 0x200);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, 0x200);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R ,0);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G ,0);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B ,0);
    }
}

/******************************************************************************************/
/***********************************  ISR Log function  ***********************************/
/******************************************************************************************/
void vApiIsrLogEnable(UINT32 u4Enable)
{    
#ifndef NDEBUG    
    u4IsrLogEn = u4Enable;
#endif
}

void vApiConfigIsrLog(UINT32 u4Isr, UINT32 u4Addr, UINT32 u4Low, UINT32 u4High, UINT32 u4Count)
{
#ifndef NDEBUG    
    UINT32 i;

    for (i = 0; i < 8; i++)
    {
        if (LogConfig[i].u4Count == 0)
        {
            LogConfig[i].u4Isr = u4Isr;
            LogConfig[i].u4Addr = u4Addr;
            LogConfig[i].u4Low = u4Low;
            LogConfig[i].u4High = u4High;
            LogConfig[i].u4Mask = (1<<(u4High-u4Low+1))-1;
            LogConfig[i].u4Count = u4Count;
            
            break;
        }
    }

    if (i == 8)
    {        
        Printf("ISR Log full\n");
    }    

    for (i = 0; i < 8; i++)
    {
        Printf("ISR LOG[%d] : ISR %d Addr 0x%08x  bit %d ~ %d  Mask 0x%08x  Count %d\n", i,
            LogConfig[i].u4Isr, LogConfig[i].u4Addr, LogConfig[i].u4Low, 
            LogConfig[i].u4High, LogConfig[i].u4Mask, LogConfig[i].u4Count);
    }
    
#endif
}

void vApiIsrLog(UINT32 u4Isr)
{
#ifndef NDEBUG
    UINT32 i;

    if (u4IsrLogEn)
    {
        for (i = 0; i < 8; i++)
        {       
            if ((u4Isr == LogConfig[i].u4Isr) && (LogConfig[i].u4Count != 0))
            {
                LogConfig[i].u4Count--;
                Printf("[%d] ISR %d  Addr 0x%08x   Value %x \n", 
                    i, LogConfig[i].u4Isr, LogConfig[i].u4Addr, 
                    (u4IO32Read4B(LogConfig[i].u4Addr)>>LogConfig[i].u4Low)&LogConfig[i].u4Mask);
            }
        }        
    }
#endif    
}
