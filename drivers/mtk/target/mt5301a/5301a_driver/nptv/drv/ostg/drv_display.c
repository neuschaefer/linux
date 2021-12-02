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
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "drv_vdoclk.h"
#include "util.h"
#include "hw_scpos.h"
#include "drv_scpos.h"
#include "drv_display.h"
#include "drv_scaler.h"
#include "drv_ycproc.h"
#include "drv_lvds.h"
#include "drv_di.h"
#include "vdo_misc.h"
#include "drv_hdtv.h"
#include "video_def.h"
#include "drv_video.h"
#include "nptv_debug.h"
#include "api_backlt.h"
#ifndef CC_COPLAT_MT82
#include "osd_drvif.h"
#include "pmx_drvif.h"
#endif
#include "x_assert.h"
#include "x_lint.h"
#include "c_model.h"
#include "x_timer.h"
#include "ostg_if.h"
#ifdef THE_3RD_PARTY_HEADER_FILE_1
#include THE_3RD_PARTY_HEADER_FILE_1
#endif
#ifdef CC_DRV_SUPPORT_SMICRO
#include "SMicro/SMicro_if.h"
#endif

//luis@2007/7/5, for vRegWriteFldXXX
/*lint -save -e666*/

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#ifndef CC_MOSYNCLEN_MIN
#define CC_MOSYNCLEN_MIN (99 + 15)
#endif
#define CC_RIGHTMASK_MIN (10)
#define CC_MOSYNCLEN_MAX (2047)
#define CC_RIGHTMASK_MAX (30)


#define OSTG_UPDATE_PIXEL_SHIFT_H 1
#define OSTG_UPDATE_PIXEL_SHIFT_V 2



// force the HTotal to be 8x-aligned in Dispmode + Dual panel
#define CC_HTOTAL_8X_LIMIT_WA

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Public variables
//-----------------------------------------------------------------------------

UINT32 dwSI_DISPLAY_DCLK_TYPICAL = 0x00;
UINT32 dwSI_DISPLAY_DCLK_50HZ;
#ifdef SUPPORT_PANEL_48HZ
UINT32 dwSI_DISPLAY_DCLK_48HZ;
#endif
UINT32 _u4OSTGDelta = 0;

#if 0
#ifdef CC_DVBT_SUPPORT
UINT8 bSI_DISPLAY_DCLK_TYPE = SV_DCLK_60HZ;
UINT8 bOUTPUT_REFRESH_RATE = 60;
#else
UINT8 bSI_DISPLAY_DCLK_TYPE = SV_DCLK_50HZ;
UINT8 bOUTPUT_REFRESH_RATE = 50;
#endif
#else
//Give initial value instead of using "CC_DVBT_SUPPORT" define
UINT8 bSI_DISPLAY_DCLK_TYPE = SV_DCLK_UNKNOW;
UINT8 bOUTPUT_REFRESH_RATE = 0;
#endif

UINT8 bDISPLAY_INTERLACE_ENABLE;
BOOL _fgFRS_ENA;

#define IMPORT_PROTECTION 1
#define IMPORT_PROTECTION_RECOVER 2
UINT8 _bIsImportProtection = IMPORT_PROTECTION;

UINT32 _u4ErrorLimit = 0x1f;

#if CC_FHD_FIXLENGH
#define FHDFixLength 98; //Full Screen workaround for FHD, Peggy
#endif

#if CC_SCPOS_VSYNC_WA1
//static UINT8 _u1TopMask = 0;
static UINT8 _u1BottomMask = 0;
static UINT8 _u1LeftMask = 0;
static UINT8 _u1RightMask = 0;
static UINT32 _u4VTotal = 0;
static UINT32 _u4OutHVChanged = 0;

static UINT32 _u4HTotal = 0;	//OSD falsh when HVToatl change
static UINT32 _u4HSyncLen = 0;//OSD falsh when HVToatl change
#endif

static UINT32 _u4OstgUpdateEvent = 0;

#ifdef SUPPORT_SW_FRAME_CHECK
//#define V_DISTANCE_RANGE 20
//#define COARSE_STEP_SIZE 20
//#define FINE_STEP_MIN 0
//#define FINE_STEP_CENTER 100
//#define FINE_STEP_MAX 200
//#define FINE_STEP_SIZE 5
//#define V_STABLE_COUNT 300

UINT32 _u4VDistUpperBound = 0x800 + 2;      // target in-out VSync distance
UINT32 _u4VDistLowerBound = 0x800 - 0;      // target in-out VSync distance
UINT32 _u4VDistTorance = 1;
UINT32 _u4VLossDistTorance = 15;
UINT32 _u4VDistTrackState = 0;          // 1: down, 2: up
UINT32 _u4UpTrackRange = 0;
UINT32 _u4DownTrackRange = 0;
UINT32 _u4VDistTrackMaxStep = 3;          
UINT32 _u4VDistTrackEn = 0;          
UINT8 _u4VDistTrackLocked = 0;
UINT8 Threadhold = 20;


//static UINT32 u4InOutVDistance = 0xffff;
//static UINT32 u4NoChangeCounter = 0x0;
//static UINT32 u4CoarseStepSize = 0;
//static UINT32 u4FineStepSize = 5;
//static UINT32 u4CurrentStep = FINE_STEP_CENTER;
#endif

#ifdef SUPPORT_CW_CHANGE_BY_STEP
UINT32 _u4TargetCW = 0;
UINT32 _u4CurrentCW = 0;
UINT32 _u4CWStep = 0x500000;
#endif

#ifdef SUPPORT_DDDS_STEP_TRACKING
UINT32 _u4CenterCW= 0;
UINT16 _u2DDDSStepTrackingEn = 0;
UINT32 _u4DDDSTrackingErrorLimit = 2;        
UINT32 _u4StepInCWTolerance = 0x1000;  
UINT32 _u4CurrentStepCW = 0;
UINT32 _u4StepInCWUpperBound = 0;
UINT32 _u4StepInCWLowerBound = 0;
UINT32 _u4DefaultErrorLimit = 0xf;
#endif


#if defined(CC_HVSYNC_ALIGN_WA) && defined(LOAD_PANEL_FROM_FLASH_CUSTOM)
extern UINT32 _u4HVAlignmentOffset;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
#ifdef SUPPORT_DDDS_STEP_TRACKING
static void vDrvDDDSTrackingNormProc(void);
#endif
#endif

// Output stage settings during output V-sync. 
static void vDrvOstgParaUpdate(void)
{
#if CC_SCPOS_VSYNC_WA1
    if (_u4OutHVChanged)
    {
        UINT32 u4OriginalV1;
        UINT32 u4DEPowerDown;

        u4OriginalV1 = RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_MOVLEN);
        u4DEPowerDown = RegReadFldAlign(OUTSTG_OS_15, OS_15_DEN_EN);

        if (u4DEPowerDown == 0)
        {
            LVDS_SwitchDataEnable(FALSE);
        }
            
        if (u4OriginalV1 <= _u4VTotal)
        {
            vRegWriteFldAlign(OUTSTG_OS_16, _u1BottomMask, OS_16_BOTTOM_MASK_7_0);
            vRegWriteFldAlign(SCPIP_PIP_OA_00, _u4VTotal, PIP_OA_00_MOVLEN); 
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_00, _u4VTotal, PIP_OA_00_MOVLEN); 
            vRegWriteFldAlign(OUTSTG_OS_16, _u1BottomMask, OS_16_BOTTOM_MASK_7_0);
        }
            /* update htotal & hsync len in blanking interval */
            vRegWriteFldAlign(SCPIP_PIP_OA_00, _u4HTotal, PIP_OA_00_MOHLEN);
            vRegWriteFldAlign(SCPIP_PIP_OA_01, _u4HSyncLen, PIP_OA_01_MOSYNCLEN);

            /* update left/right mask */
            vRegWriteFldAlign(OUTSTG_OS_17, _u1LeftMask, OS_17_LEFT_MASK_7_0);
            vRegWriteFldAlign(OUTSTG_OS_17, _u1RightMask, OS_17_RIGHT_MASK_7_0); 
            _u4OutHVChanged = 0;
            
        if (u4DEPowerDown == 0)
        {
            LVDS_SwitchDataEnable(TRUE);
        }
    }
#endif
}


/**
 * @brief vDrvSetDENAMask

 * Set the Top/Bottom/Left/Right Masking

 * @param  TOP: the value of TOP_MASK
 *         BOTTOM: the value of BOTTOM_MASK
 *         LEFT: the value of LEFT_MASK
 *         RIGHT: the value of RIGHT_MASK
 * @retval void
 */
#if (!CC_SCPOS_VSYNC_WA1) 
static void vDrvSetDENAMask(UINT16 TOP, UINT16 BOTTOM, UINT16 LEFT, UINT16 RIGHT)
{
    vRegWriteFldAlign(OUTSTG_OS_2B, TOP, OS_2B_TOP_MASK_9_0_);
    vRegWriteFldAlign(OUTSTG_OS_2B, BOTTOM, OS_2B_BOTTOM_MASK_9_0_);
    vRegWriteFldAlign(OUTSTG_OS_2C, LEFT+_u4OSTGDelta, OS_2C_LEFT_MASK_9_0_);
    vRegWriteFldAlign(OUTSTG_OS_2C, RIGHT, OS_2C_RIGHT_MASK_9_0_);
}
#endif


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/**
 * @brief wDrvGetOutputHTotal

 * Get Output H_Total

 * @param  void
 * @retval Output H_Total
 */
UINT16 wDrvGetOutputHTotal(void)
{
    UINT16 wTemp;
    //MOHLEN
    wTemp= RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_MOHLEN);
    return (wTemp + 1);
}


/**
 * @brief wDrvGetOutputVTotal

 * Get Output V_Total

 * @param  void
 * @retval Output V_Total
 */
UINT16 wDrvGetOutputVTotal(void)
{
    UINT16 wTemp;
    // MOVLEN
    wTemp= RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_MOVLEN);
    return (wTemp + 1);
}


/**
 * @brief bDrvCheckMTKGoodStatus

 * Check MTK Good Clock Lock or Unlock

 * @param  void
 * @retval TRUE/FALSE
 */
UINT8 bDrvCheckMTKGoodStatus(void)
{
    return (RegReadFldAlign(OUTSTG_OS_E5, OS_E5_DDDS_LOCK));
}


 #ifndef CC_MTK_LOADER

#if 0
void vSetMTKGoodDclkNSTD(void) // Called by Video
{
    UINT32 dwDCLK;
    UINT32 DISPLAY_SIZE, HLEN_INT, HLEN_NUM;
    UINT16 wVERT_LINE;
    UINT8 bInputFrameRate, bType;
    RTvdNSTDStatus rStatus;

    wVERT_LINE = wDrvVideoGetVTotal(SV_VP_MAIN);
    if(wVERT_LINE==0)
        return;

    bType = bGetVideoDecType(SV_VP_MAIN);
    if (bType == SV_VD_TVD3D)
    {
        vTvd3dGetNSTDStatus(&rStatus);
    }
    else if (bType == SV_VD_YPBPR)
    {
        vHDTVGetNSTDStatus((RHDTVNSTDStatus*)&rStatus);
    }
    else
        return;

    if (rStatus.fgIsNSTD)
    {
        bInputFrameRate = rStatus.bRefreshRate;
        wVERT_LINE = rStatus.wVTotal;

	  if((bInputFrameRate!=50)&&(bInputFrameRate!=60))
	  {
            vDrvLCDFreqSet(SV_DCLK_OTHERS);
	  }
        bOUTPUT_REFRESH_RATE = bInputFrameRate;
        if (bInputFrameRate < 55)
        {
            dwDCLK = (dwSI_DISPLAY_DCLK_50HZ /50) * bInputFrameRate;
        }
        else
        {
            dwDCLK = (dwSI_DISPLAY_DCLK_TYPICAL /60) * bInputFrameRate;
        }
        vDrvVOFreqSet(dwDCLK);

        DISPLAY_SIZE = (UINT32)wDrvGetOutputHTotal()*(UINT32)wDrvGetOutputVTotal();
        //DISPLAY_SIZE = DISPLAY_SIZE * bOUTPUT_REFRESH_RATE / bInputFrameRate; 
        DISPLAY_SIZE = bDrvVideoIsSrcInterlace(SV_VP_MAIN) ? DISPLAY_SIZE * 2 : DISPLAY_SIZE;

        HLEN_INT = (UINT32) (DISPLAY_SIZE / wVERT_LINE);
        HLEN_NUM = DISPLAY_SIZE - (HLEN_INT * (UINT32) wVERT_LINE);
        LOG(3, "[NSTD] inVTaol: 0x%X, outV: 0x%X, outH: 0x%X, HLEN_INT: 0x%X, HLEN_NUM: 0x%X\n",
        wVERT_LINE, wDrvGetOutputVTotal(), wDrvGetOutputHTotal(), HLEN_INT, HLEN_NUM);

        vRegWrite4B(OUTSTG_OS_E4, (_u4ErrorLimit<<24)|((UINT16) wVERT_LINE <<12)|(UINT16)HLEN_NUM);
        vRegWriteFldAlign(OUTSTG_OS_E3, HLEN_INT, OS_E3_HLEN_INT);
    }
    else
    {
        vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN));
    }
}
#endif

/**
 * @brief vDrvSetMTKGoodDclk

 * Set MTK Good Dclk Parameters

 * @param  wVERT_LINE: Input Total Lines in the Vertical Direction
 * @retval void
 */ 
void vDrvSetMTKGoodDclk(UINT16 u2InputVTotal, UINT8 bInputFrameRate) // Called by Video
{
    UINT32 DISPLAY_SIZE, HLEN_INT, HLEN_NUM;
 //   UINT8 bInputFrameRate;
    UINT32 u4HTotal, u4VTotal;
#ifdef SUPPORT_SW_FRAME_CHECK
    UINT8 u4VStep;
    UINT32  u4VDistance;
#endif

    if ((u2InputVTotal == 0) || (bInputFrameRate == 0))
    {
//        ASSERT(wVERT_LINE != 0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }

    #if CC_SCPOS_VSYNC_WA1
    if (_u4OutHVChanged)
    {
        u4HTotal = (_u4HTotal + 1);
        u4VTotal = (_u4VTotal + 1);
    }
    else
    #endif// CC_SCPOS_VSYNC_WA1
    {
         u4HTotal = (UINT32)wDrvGetOutputHTotal();
         u4VTotal = (UINT32)wDrvGetOutputVTotal();
    }

    #ifdef SUPPORT_SW_FRAME_CHECK
    if (_u4VDistTrackState == 1) // down tracking
    {   
        u4VStep =  (_u4DownTrackRange > _u4VDistTrackMaxStep)? _u4VDistTrackMaxStep: _u4DownTrackRange;
        u4VTotal = (u4VTotal > u4VStep)? (u4VTotal -u4VStep): 0; 
    }
    else if (_u4VDistTrackState == 2)  // up tracking
    {
        u4VStep =  (_u4UpTrackRange > _u4VDistTrackMaxStep)? _u4VDistTrackMaxStep: _u4UpTrackRange;
        u4VTotal = u4VTotal +u4VStep; 
    }
    else
    {
        u4VStep =0;
    }

    u4VDistance = u4ScposGetInOutVDistance();
    if ((u4VDistance >= (RegReadFldAlign(SCPIP_PIP_OA_00,  PIP_OA_00_MOVLEN) / 2))
            && (u4VDistance <= RegReadFldAlign(SCPIP_PIP_OA_00,  PIP_OA_00_MOVLEN)))
    {
        u4VDistance = RegReadFldAlign(SCPIP_PIP_OA_00,  PIP_OA_00_MOVLEN) - u4VDistance;
    }
    //LogSB("u4VStep+++++",u4VStep);
    if ((u4VStep <= _u4VDistTrackMaxStep) && (u4VStep >= 2) && (u4VDistance <= Threadhold))
    {
        _u4VDistTrackMaxStep = u4VStep / 2;
    }
//  LogS("set mtkgoodclokkkkkkkkkkkkkkk.");
    #endif     
    {
        DISPLAY_SIZE = u4HTotal * u4VTotal;
    }
    
    //bInputFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
    #if SUPPORT_DDDS_ON_24P_25P_30P
    if (((bInputFrameRate== 24) || (bInputFrameRate == 25) || (bInputFrameRate== 30)) 
    	&& (bDrvVideoIsSrcInterlace(SV_VP_MAIN) == SV_FALSE))
    {
        DISPLAY_SIZE = DISPLAY_SIZE * bOUTPUT_REFRESH_RATE / bInputFrameRate; 
        LOG(3, "DDDS %dHz --> %dHz\n", bInputFrameRate, bOUTPUT_REFRESH_RATE);
    }
    #endif

    DISPLAY_SIZE = bDrvVideoIsSrcInterlace(SV_VP_MAIN) ? DISPLAY_SIZE * 2 : DISPLAY_SIZE;

    HLEN_INT = (UINT32) (DISPLAY_SIZE / u2InputVTotal);
    HLEN_NUM = DISPLAY_SIZE - (HLEN_INT * (UINT32) u2InputVTotal);
    LOG(9, "inVTaol: 0x%X, outV: 0x%X, outH: 0x%X, HLEN_INT: 0x%X, HLEN_NUM: 0x%X\n",
    u2InputVTotal, wDrvGetOutputVTotal(), wDrvGetOutputHTotal(), HLEN_INT, HLEN_NUM);
    
    #ifdef SUPPORT_SW_FRAME_CHECK
    #if 0
    HLEN_INT += u4CoarseStepSize;
    
    if(u4CurrentStep!=0 && u4CurrentStep<=200)
    {
        LOG(5,"Chnage HLEN_NUM from (%d) to ",HLEN_NUM);
        if(u4CurrentStep>=FINE_STEP_CENTER)
        {
            HLEN_NUM = HLEN_NUM + (u4FineStepSize * (u4CurrentStep - FINE_STEP_CENTER));
        }
        else
        {
            HLEN_NUM = HLEN_NUM - (u4FineStepSize * (FINE_STEP_CENTER - u4CurrentStep));
        }
        LOG(5,"(%d)\n",HLEN_NUM);        
    }
    #endif    
    #endif    
    
//    vRegWrite4B(VFE_1F,0x3f000000|((UINT16) wVERT_LINE <<12)|(UINT16)HLEN_NUM);
//    vRegWrite2B(VFE_1E,(UINT16)HLEN_INT);
    vRegWrite4B(OUTSTG_OS_E4, (_u4ErrorLimit<<24)|((UINT16) u2InputVTotal <<12)|(UINT16)HLEN_NUM);
    vRegWriteFldAlign(OUTSTG_OS_E3, HLEN_INT, OS_E3_HLEN_INT);
    
}
#endif // end of #ifndef CC_MTK_LOADER


/**
 * @brief vDrvSwitchMTKGoodDclk

 * MTK Good Dclk ON/OFF

 * @param  bOn_Off: SV_ON/SV_OFF
 * @retval void
 */
void vDrvSwitchMTKGoodDclk(UINT8 bOn_Off)
{
#ifndef CC_MTK_LOADER

    if (bOn_Off == SV_ON)
    {
//            vRegWrite1B(VFE_1E+3, u1RegRead1B(VFE_1E+3) | 0xc0);
        vRegWriteFldAlign(OUTSTG_OS_E3, 1, OS_E3_DISP_EN);
        vRegWriteFldAlign(OUTSTG_OS_E3, 1, OS_E3_FIX_FS_DDDS_SEL);
    }
    else
    {
        #ifdef SUPPORT_SW_FRAME_CHECK
            _u4UpTrackRange = 0;
            _u4DownTrackRange = 0;
            _u4VDistTrackState = 0;
        #endif
    
//            vRegWrite1B(VFE_1E+3, u1RegRead1B(VFE_1E+3) & 0x3f);
        vRegWriteFldAlign(OUTSTG_OS_E3, 0, OS_E3_DISP_EN);
        vRegWriteFldAlign(OUTSTG_OS_E3, 0, OS_E3_FIX_FS_DDDS_SEL);

//            vDrvVoDclkSpreadInit(1);
//            HAL_Delay_us(100);
//            vDrvVoDclkSpreadInit(0);                      
        vRegWriteFldAlign(OUTSTG_OS_E0, 1, OS_E0_SPREAD_INIT);
        #ifdef CC_COPLAT_MT82
        vUtDelay2us(50);
        #else
        HAL_Delay_us(100);
        #endif
        vRegWriteFldAlign(OUTSTG_OS_E0, 0, OS_E0_SPREAD_INIT);
    }
    #endif //end of #ifndef CC_MTK_LOADER 
}


void vDrvUpdateHPosition(void)
{
    if ((bSI_DISPLAY_DCLK_TYPE == SV_DCLK_60HZ) || 
        ((bSI_DISPLAY_DCLK_TYPE == SV_DCLK_OTHERS) && (bOUTPUT_REFRESH_RATE >= 55)))
    {
    #ifndef CC_COPLAT_MT82
        _OSD_AlwaysUpdateReg(TRUE);
        vRegWriteFldAlign(OUTSTG_OS_16, H_POS_60HZ, OS_16_H_POS);
        _OSD_AlwaysUpdateReg(FALSE);
    #else
        vRegWriteFldAlign(OUTSTG_OS_16, H_POS_60HZ, OS_16_H_POS);
    #endif
    }
    #ifdef SUPPORT_PANEL_48HZ
    else if(bSI_DISPLAY_DCLK_TYPE == SV_DCLK_48HZ)
    {
    	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        vRegWriteFldAlign(OUTSTG_OS_16, PANEL_GetHPositionByTiming(SV_DCLK_48HZ), OS_16_H_POS);
    	#else
    	vRegWriteFldAlign(OUTSTG_OS_16, H_POS_60HZ, OS_16_H_POS);
    	#endif
    }
    #endif    
    else
    {
        #ifndef CC_COPLAT_MT82
        _OSD_AlwaysUpdateReg(TRUE);
        vRegWriteFldAlign(OUTSTG_OS_16, H_POS_50HZ, OS_16_H_POS);
        _OSD_AlwaysUpdateReg(FALSE);
       #else
        vRegWriteFldAlign(OUTSTG_OS_16, H_POS_50HZ, OS_16_H_POS);
        #endif
    }
}

void vDrvSetLVDSVPosition(UINT32 u4Pos)
{
//    vRegWriteFldAlign(OUTSTG_OS_16, (u4Pos & 0xff), OUTSTG_V_POS_7_0);
    vRegWriteFldAlign(OUTSTG_OS_18, u4Pos, OS_18_V_POS);
}


// LVDS power------------------------------------------------------------------
void vDrvTurnOnPanel()
{
    do 
    { 
        UINT32 gpio, value; 
        gpio = DRVCUST_PanelGet(ePanelPowerGpio); 
        value = DRVCUST_PanelGet(ePanelPowerOnVal); 
        vDrvGpioSet(gpio, value); 
    } while (0);
}

void vDrvTurnOffPanel()
{
    do 
    { 
        UINT32 gpio, value; 
        gpio = DRVCUST_PanelGet(ePanelPowerGpio); 
        value = DRVCUST_PanelGet(ePanelPowerOnVal); 
        vDrvGpioSet((INT32)gpio, (INT32)!value); 
    } while (0);
}
//-----------------------------------------------------------------------------
/** turn on/off panel lvds signal.
 *  @param fgEnable: Control mode. It could be TRUE or FALSE
 *  @return
 */
//-----------------------------------------------------------------------------

void vDrvLVDSSignalOnOff(BOOL fgOnOff)
{
	if(fgOnOff)
	{
		vDrvLVDSOn();
	}
	else
	{
		vDrvLVDSOff();
	}
}

//-----------------------------------------------------------------------------
/** turn on/off panel lvds powe.
 *  @param fgEnable: Control mode. It could be TRUE or FALSE
 *  @return
 */
//-----------------------------------------------------------------------------

void vDrvLVDSPowerOnOff(BOOL fgOnOff)
{
	if(fgOnOff)
	{
		vDrvTurnOnPanel();
	}
	else
	{
		vDrvTurnOffPanel();
	}
}

//-----------------------------------------------------------------------------
/** turn on/off panel lvds power and signal.
 *  @param fgEnable: Control mode. It could be PANEL_ON or PANEL_OFF
 *  @return
 */
//-----------------------------------------------------------------------------

void vApiLvdsONOFF(BOOL fgEnable)
{
    if (fgEnable == PANEL_ON)
    {
        vDrvTurnOnPanel();
        x_thread_delay(10 * PANEL_GetLvdsOnDalay());
        vDrvLVDSOn();
    }
    else
    {
        vDrvLVDSOff();
        x_thread_delay(10 * PANEL_GetLvdsOffDalay());
        vDrvTurnOffPanel();
    }
}

/**
 * @brief vDrvSetLCDTiming

 * Set the Display Output Timing for LCD Panel

 * @param  void
 * @retval void
 */
void vDrvSetLCDTiming(void)
{
    UINT32 u4syncLen;
    UINT32 u4LeftMask;
    vDrvSwitchMTKGoodDclk(SV_OFF);

    if( _bIsImportProtection )
    {
//        _bIsImportProtection = IMPORT_PROTECTION_RECOVER;
        vDrvSwitchImportProtection(1);
    }

    if ((bSI_DISPLAY_DCLK_TYPE == SV_DCLK_60HZ) || 
        ((bSI_DISPLAY_DCLK_TYPE == SV_DCLK_OTHERS) && (bOUTPUT_REFRESH_RATE >= 55)))
    {
        //Set display output timing
        #ifndef CC_COPLAT_MT82
        _OSD_AlwaysUpdateReg(TRUE);
        #endif
        u4syncLen = HSYNCLEN_TYPI;
        u4LeftMask = LEFT_MASK_TYPI;
        
        #if CC_FHD_FIXLENGH
        if ((wDISPLAY_WIDTH >= 1920) && (wDISPLAY_HEIGHT >= 1080)) // Full Screen workaround for FHD, Peggy
        {
            u4syncLen = HSYNCLEN_TYPI - FHDFixLength;
            u4LeftMask = u4LeftMask + FHDFixLength;
        }
        #endif
        
        #if CC_SCPOS_VSYNC_WA1
        _u4HSyncLen = u4syncLen;
        _u4HTotal = HLEN_TOTAL_TYPI;
        _u1LeftMask = u4LeftMask; 
        _u1RightMask = RIGHT_MASK_TYPI;
        _u1BottomMask = BOTTOM_MASK_TYPI;
        vRegWriteFldAlign(OUTSTG_OS_16, TOP_MASK_TYPI, OS_16_TOP_MASK_7_0);
        #else        
        vRegWriteFldAlign(SCPIP_PIP_OA_01, u4syncLen-_u4OSTGDelta, PIP_OA_01_MOSYNCLEN);
        vRegWriteFldAlign(SCPIP_PIP_OA_00, HLEN_TOTAL_TYPI, PIP_OA_00_MOHLEN);
        vDrvSetDENAMask(TOP_MASK_TYPI, BOTTOM_MASK_TYPI, u4LeftMask, RIGHT_MASK_TYPI);
        #endif

//        vRegWriteFld(OUTSTG_OS_16, H_POS_60HZ, OUTSTG_H_POS_7_0);
//        vRegWriteFldAlign(OUTSTG_OS_18, H_POS_60HZ>>8, OUTSTG_H_POS_11_8);
        vRegWriteFldAlign(OUTSTG_OS_16, H_POS_60HZ, OS_16_H_POS);
        #ifndef CC_COPLAT_MT82
        _OSD_AlwaysUpdateReg(FALSE);
        #endif
        #if CC_SCPOS_VSYNC_WA1
        _u4VTotal = VLEN_TOTAL_TYPI;
        _u4OutHVChanged = 1;
        #else
        vRegWriteFldAlign(SCPIP_PIP_OA_00, VLEN_TOTAL_TYPI, PIP_OA_00_MOVLEN);
        #endif

	 //vDrvOstgParaUpdate();
        // VDOIN ISR cannot work correctly, work around , should be remove , Yulia 2008.05.19
        //vRegWriteFldAlign(SCPIP_PIP_OA_00, VLEN_TOTAL_TYPI, MOVLEN);
        //vRegWriteFldAlign(OUTSTG_OS_16, _u1BottomMask, OUTSTG_BOTTOM_MASK_7_0);
        // end of work-around
    }
    
    #ifdef SUPPORT_PANEL_48HZ
    else if (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_48HZ)
    {
            //Set display output timing
            _OSD_AlwaysUpdateReg(TRUE);
    
            u4syncLen = HSYNCLEN_48HZ;
            u4LeftMask = LEFT_MASK_48HZ;
    #if CC_FHD_FIXLENGH
            if ((wDISPLAY_WIDTH >= 1920) && (wDISPLAY_HEIGHT >= 1080)) // Full Screen workaround for FHD, Peggy
            {
                u4syncLen = HSYNCLEN_48HZ - FHDFixLength;
                u4LeftMask = u4LeftMask + FHDFixLength;
            }
    #endif
            
    #if (CC_SCPOS_VSYNC_WA1)
            _u4HSyncLen = u4syncLen;
            _u4HTotal = HLEN_TOTAL_48HZ;
            _u1LeftMask = u4LeftMask; 
            _u1RightMask = RIGHT_MASK_48HZ;
            _u1BottomMask = BOTTOM_MASK_48HZ;
            vRegWriteFldAlign(OUTSTG_OS_2B, TOP_MASK_48HZ, OS_2B_TOP_MASK_9_0_);
    #else        
            vRegWriteFldAlign(SCPIP_PIP_OA_01, u4syncLen-_u4OSTGDelta, PIP_OA_01_MOSYNCLEN);
            vRegWriteFldAlign(SCPIP_PIP_OA_00, HLEN_TOTAL_48HZ, PIP_OA_00_MOHLEN);
            vDrvSetDENAMask(TOP_MASK_48HZ, BOTTOM_MASK_48HZ,(UINT16)u4LeftMask, RIGHT_MASK_48HZ);
    #endif
    
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        vRegWriteFldAlign(OUTSTG_OS_16, PANEL_GetHPositionByTiming(SV_DCLK_48HZ), OS_16_H_POS);
        #else
        vRegWriteFldAlign(OUTSTG_OS_16, H_POS_60HZ, OS_16_H_POS);
        #endif
        _OSD_AlwaysUpdateReg(FALSE);
    
    #if CC_SCPOS_VSYNC_WA1        
            _u4VTotal = VLEN_TOTAL_48HZ;
            _u4OutHVChanged = 1;
    #else
            vRegWriteFldAlign(SCPIP_PIP_OA_00, VLEN_TOTAL_48HZ, PIP_OA_00_MOVLEN);
    #endif
    
    }
    #endif //SUPPORT_PANEL_48HZ
    else
    {
        //Set display output timing
        #ifndef CC_COPLAT_MT82
        _OSD_AlwaysUpdateReg(TRUE);
        #endif
        
        u4syncLen = HSYNCLEN_50HZ;
        u4LeftMask = LEFT_MASK_50HZ;

        #if CC_FHD_FIXLENGH
        if ((wDISPLAY_WIDTH >= 1920) && (wDISPLAY_HEIGHT >= 1080)) // Full Screen workaround for FHD, Peggy
        {
            u4syncLen = HSYNCLEN_50HZ - FHDFixLength;
            u4LeftMask = u4LeftMask + FHDFixLength;
        }
        #endif

        #if CC_SCPOS_VSYNC_WA1
        _u4HSyncLen = u4syncLen;
        _u4HTotal = HLEN_TOTAL_50HZ;
        _u1LeftMask = u4LeftMask; 
        _u1RightMask = RIGHT_MASK_50HZ;
        _u1BottomMask = BOTTOM_MASK_50HZ;
        vRegWriteFldAlign(OUTSTG_OS_16, TOP_MASK_50HZ, OS_16_TOP_MASK_7_0);
        #else        
        vRegWriteFldAlign(SCPIP_PIP_OA_01, u4syncLen-_u4OSTGDelta, PIP_OA_01_MOSYNCLEN);
        vRegWriteFldAlign(SCPIP_PIP_OA_00, HLEN_TOTAL_50HZ, PIP_OA_00_MOHLEN);
        vDrvSetDENAMask(TOP_MASK_50HZ, BOTTOM_MASK_50HZ, u4LeftMask, RIGHT_MASK_50HZ);
        #endif
        
        
        if (H_POS_50HZ > 0)
        {
//            vRegWriteFld(OUTSTG_OS_16, H_POS_50HZ, OUTSTG_H_POS_7_0);
//            vRegWriteFldAlign(OUTSTG_OS_18, H_POS_50HZ>>8, OUTSTG_H_POS_11_8);
            vRegWriteFldAlign(OUTSTG_OS_16, H_POS_50HZ, OS_16_H_POS);
        }
        else
        {
//            vRegWriteFld(OUTSTG_OS_16, H_POS_60HZ, OUTSTG_H_POS_7_0);
//            vRegWriteFldAlign(OUTSTG_OS_18, H_POS_60HZ>>8, OUTSTG_H_POS_11_8);
            vRegWriteFldAlign(OUTSTG_OS_16, H_POS_60HZ, OS_16_H_POS);
        }

        #ifndef CC_COPLAT_MT82
        _OSD_AlwaysUpdateReg(FALSE);
        #endif
        #if CC_SCPOS_VSYNC_WA1
        _u4VTotal = VLEN_TOTAL_50HZ;
        _u4OutHVChanged = 1;
        #else
        vRegWriteFldAlign(SCPIP_PIP_OA_00, VLEN_TOTAL_50HZ, PIP_OA_00_MOVLEN);
        #endif
    }

    #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    vRegWriteFldAlign(OUTSTG_OS_18, PANEL_GetVSyncWidthByTiming(bSI_DISPLAY_DCLK_TYPE), OS_18_VS_WIDTH);
    vRegWriteFldAlign(OUTSTG_OS_17, PANEL_GetHSyncWidthByTiming(bSI_DISPLAY_DCLK_TYPE), OS_17_HS_WIDTH_11_0_);
    vDrvSetLVDSVPosition(PANEL_GetVPositionByTiming(bSI_DISPLAY_DCLK_TYPE));
    #else
    vRegWriteFldAlign(OUTSTG_OS_18, VS_WIDTH, OS_18_VS_WIDTH);
    vRegWriteFldAlign(OUTSTG_OS_17, HS_WIDTH, OS_17_HS_WIDTH_11_0_);
    vDrvSetLVDSVPosition(V_POS);
    #endif

#ifdef CC_MTK_LOADER 
    #if CC_SCPOS_VSYNC_WA1
    OSTG_OnOutputVSync();
    #endif
#endif

    if( _bIsImportProtection )
    {
	    vDrvFireImportPortection();
	       // vDrvSwitchImportProtection(0);
           _bIsImportProtection = IMPORT_PROTECTION_RECOVER;
    }


}


/**
 * @brief bDrvDispSupportTiming

 * Check if Dispmdoe supports the input timing

 * @param  wHeight: Source height
 * @param  wVTotal: Total Lines in the Vertical Direction
 * @param wFrameRate: Frame rate
 * @retval SV_SUCCESS/SV_FAIL
 */
UINT8 bDrvDispSupportTiming(UINT16 wHeight, UINT16 wVTotal, UINT16 wFrameRate)
{
    UINT32 u4OutputVTotal;
    UINT32 u4PanelHeight;

    u4PanelHeight = TV_HEIGHT;
    u4OutputVTotal = (u4PanelHeight * (UINT32)wVTotal + (wHeight >> 1))/(UINT32)wHeight;

    return bDrvDispSupport((UINT16)u4OutputVTotal, wFrameRate);
}


/**
 * @brief bDrvDispSupport

 * Check if Dispmdoe supports the output setting

 * @param  wOutputVTotal: Output Total Lines in the Vertical Direction
 * @param wOutputFrameRate: Output frame rate
 * @retval SV_SUCCESS/SV_FAIL
 */
UINT8 bDrvDispSupport(UINT16 wOutputVTotal, UINT16 wOutputFrameRate)
{
    UINT32 u4RefreshRate;
    UINT32 u4HTotal;
    UINT32 u4HTotalMinBySyncMin;
    UINT32 u4HTotalMinByPixelClock;
    UINT32 u4HTotalMaxByPixelClock;

    LOG(1, "DM Panel V(%d %d %d) H(%d %d %d) P(%d %d %d) F(%d %d)\n",
        DVS_TOTAL_MIN, VLEN_TOTAL_TYPI, DVS_TOTAL_MAX,
        DHS_TOTAL_MIN, HLEN_TOTAL_TYPI, DHS_TOTAL_MAX,
        DCLK_MIN, dwSI_DISPLAY_DCLK_TYPICAL, DCLK_MAX,
        DVS_FREQ_MIN, DVS_FREQ_MAX);

    if ((wOutputVTotal < DVS_TOTAL_MIN) ||
        (wOutputVTotal > DVS_TOTAL_MAX))
    {
        LOG(1, "DM FAIL V(%d)\n", wOutputVTotal);

        return SV_FAIL;
    }

    u4RefreshRate = (UINT32)wOutputFrameRate;

    if (u4RefreshRate == 0)
    {
        LOG(1, "DM FAIL Refresh(%d)\n", u4RefreshRate);

        return SV_FAIL;
    }

    u4HTotalMinByPixelClock = (((UINT32) DCLK_MIN) / u4RefreshRate) / (UINT32) wOutputVTotal;
    u4HTotalMinByPixelClock = ((u4HTotalMinByPixelClock + 1) & 0xfffffffe);     //make as even value
    u4HTotalMaxByPixelClock = (((UINT32) DCLK_MAX) / u4RefreshRate) / (UINT32) wOutputVTotal;
    u4HTotalMaxByPixelClock = ((u4HTotalMaxByPixelClock - 1) & 0xfffffffe);     //make as even value
    u4HTotalMinBySyncMin = (UINT32) (wDISPLAY_WIDTH + CC_MOSYNCLEN_MIN + CC_RIGHTMASK_MIN);
    u4HTotalMinBySyncMin = ((u4HTotalMinBySyncMin + 1) & 0xfffffffe);     //make as even value

#ifdef CC_HTOTAL_8X_LIMIT_WA
    if (LVDS_OUTPUT_PORT == DUAL_PORT)
    {
        u4HTotalMinByPixelClock = (u4HTotalMinByPixelClock + 7) & (~7);
        u4HTotalMinBySyncMin = (u4HTotalMinBySyncMin + 7) & (~7);
        u4HTotalMaxByPixelClock = u4HTotalMaxByPixelClock & (~7);
    }
#endif

    LOG(1, "DM H MIN(%d %d) H MAX(%d) \n",
        u4HTotalMinByPixelClock,
        u4HTotalMinBySyncMin,
        u4HTotalMaxByPixelClock);

    // Find H-Total such that,
    // 1) Bigger than H-Total MIN calculate by DCLK_MIN
    // 2) Small than H-Total MAX calculate by DCLK_MAX
    // 3) Bigger than DHS_TOTAL_MIN
    // 4) Small than DHS_TOTAL_MAX
    // 5) Bigger than H-Total MIN calculate by CC_MOSYNCLEN_MIN
    // 6) Close to HLEN_TOTAL_TYPI

#ifdef CC_HTOTAL_8X_LIMIT_WA
    if (LVDS_OUTPUT_PORT == DUAL_PORT)
    {
        u4HTotal = (UINT32) ((HLEN_TOTAL_TYPI + 1) + 7) & (~7);
    }
    else
#endif
    {
        u4HTotal = (UINT32) (HLEN_TOTAL_TYPI + 1);
    }

    if (u4HTotal > u4HTotalMaxByPixelClock)
    {
        u4HTotal = u4HTotalMaxByPixelClock;
    }

#ifdef CC_HTOTAL_8X_LIMIT_WA
    if (LVDS_OUTPUT_PORT == DUAL_PORT)
    {
        if (u4HTotal > (UINT32) ((DHS_TOTAL_MAX + 1) & (~7)))
        {
            u4HTotal = (UINT32)((DHS_TOTAL_MAX + 1) & (~7));
        }
    }
    else
#endif
    {
        if (u4HTotal > (UINT32)(DHS_TOTAL_MAX + 1))
        {
            u4HTotal = (UINT32) (DHS_TOTAL_MAX + 1);
        }
    }

    if (u4HTotal < u4HTotalMinByPixelClock)
    {
        u4HTotal = u4HTotalMinByPixelClock;
    }

    if (u4HTotal < u4HTotalMinBySyncMin)
    {
        u4HTotal = u4HTotalMinBySyncMin;
    }

    if (u4HTotal < (UINT32) (DHS_TOTAL_MIN + 1))
    {
        u4HTotal = (UINT32) (DHS_TOTAL_MIN + 1);
    }

    if ((u4HTotal > u4HTotalMaxByPixelClock) ||
        (u4HTotal > (UINT32) DHS_TOTAL_MAX))
    {
        LOG(1, "DM FAIL V(%d) H(%d) F(%d)\n",
            wOutputVTotal,
            u4HTotal,
            u4RefreshRate);

        return SV_FAIL;
    }

    LOG(1, "DM OK V(%d) H(%d) F(%d)\n",
        wOutputVTotal,
        u4HTotal,
        u4RefreshRate);

    return SV_SUCCESS;

}

/**
 * @brief bDrvDispSupportFlag

 * When DISP_MODE_USE is DISP_MODE_ON, Check V_TOTAL and Pixel Clock Rate and Calculate H_Total.

 * @param  DV_TOTAL: Output Total Lines in the Vertical Direction
 * @retval SV_SUCCESS/SV_FAIL
 */
#ifndef CC_MTK_LOADER
UINT8 bDrvDispSupportFlag(UINT16 DV_TOTAL)
{
    UINT16 u2RefreshRate;

    u2RefreshRate = (UINT16) bDrvVideoGetRefreshRate(SV_VP_MAIN);

    return bDrvDispSupport(DV_TOTAL, u2RefreshRate);
}
#endif //end of #ifndef  CC_MTK_LOADER


/**
 * @brief vDrvOutputStageInit

 * Init the Register Settings of Output Stage Logic

 * @param  void
 * @retval void
 */
void vDrvOutputStageInit(void)
{
    UINT32 u4PanelDither = 0xffffffff;

    #ifdef CC_HVSYNC_ALIGN_WA
    #ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
    _u4OSTGDelta = _u4HVAlignmentOffset;
    #else
    if (((GetCurrentPanelIndex() == PANEL_SONY_X_GY_1_FHD) ||(GetCurrentPanelIndex() == PANEL_SONY_X_GY_5_FHD) ||(GetCurrentPanelIndex() == PANEL_SONY_X_GY_6_FHD))
        || ((DRVCUST_PanelGet(ePanelFlashTableSize) > 0) && ((PANEL_GetEepromPanelIndex() == PANEL_SONY_X_GY_1_FHD) || (PANEL_GetEepromPanelIndex() == PANEL_SONY_X_GY_5_FHD) || (PANEL_GetEepromPanelIndex() == PANEL_SONY_X_GY_6_FHD))))
    {
        _u4OSTGDelta = 127;
    }
    else
    {
        _u4OSTGDelta = 0;
    }
    #endif   //LOAD_PANEL_FROM_FLASH_CUSTOM
    #else  // CC_HVSYNC_ALIGN
    _u4OSTGDelta = 0;
    #endif  // CC_HVSYNC_ALIGN

    dwSI_DISPLAY_DCLK_TYPICAL = PANEL_GetPixelClk60Hz();
    dwSI_DISPLAY_DCLK_50HZ = PANEL_GetPixelClk50Hz();
    #ifdef SUPPORT_PANEL_48HZ
    dwSI_DISPLAY_DCLK_48HZ = PANEL_GetPixelClk48Hz();
    #endif

    #if (SRGB_FUNCTION_EN == SRGB_EN_ON)
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_DEN_EN);
    }
    #else
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 0, OS_15_DEN_EN);
    }
    #endif

    #if (DEN_TYPE == DEN_ACIVE)
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_DENTPY);
    }
    #else
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 0, OS_15_DENTPY);
    }
    #endif

    #if (DEN_POLARITY == DEN_LOW)
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_DENP);
    }
    #else
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 0, OS_15_DENP);
    }
    #endif

    #if (FIELD_POLARITY == FIELD_LOW)
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_FIELDP);
    }
    #else
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 0, OS_15_FIELDP);
    }
    #endif
    if (HSYNC_POLARITY == HSYNC_LOW)
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_HSYNCP);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 0, OS_15_HSYNCP);
    }

    if (VSYNC_POLARITY == VSYNC_LOW)
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_VSYNCP);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 0, OS_15_VSYNCP);
    }

    u4PanelDither = PANEL_GetDither();
    if (DISP_BIT == DISP_30BIT)
    {
        UINT32 u4Round, u4RDither, u4EDither, u4LFSR;
        vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_DRMOD_1_0_);     
        if (u4PanelDither != 0xffffffff)
        {
            u4Round = (u4PanelDither & ROUND_FUNCTION)? 1: 0;
            u4RDither = (u4PanelDither & RDITHER_FUNCTION)? 1 : 0;
            u4EDither = (u4PanelDither & ERROR_DITHER_FUNCTION)? 1 : 0;
            u4LFSR = (u4PanelDither & LFSR_FUNCTION)? 1 : 0;
        }
        else
        {
            u4Round = 0;
            u4RDither = 0;
            u4EDither = 0;
            u4LFSR = 0;        
        }
        vRegWriteFldAlign(OUTSTG_OS_15, u4Round, OS_15_ROUND_EN);
        vRegWriteFldAlign(OUTSTG_OS_15, u4RDither, OS_15_RDITHER_EN);
        vRegWriteFldAlign(OUTSTG_OS_15, u4EDither, OS_15_EDITHER_EN);
        vRegWriteFldAlign(OUTSTG_OS_15, u4LFSR, OS_15_DITHER_LFSR_EN);
    }
    else if (DISP_BIT == DISP_24BIT)
    {
        UINT32 u4Round, u4RDither, u4EDither, u4LFSR;
        vRegWriteFldAlign(OUTSTG_OS_15, 2, OS_15_DRMOD_1_0_);
        if (u4PanelDither != 0xffffffff)
        {
            u4Round = (u4PanelDither & ROUND_FUNCTION)? 1: 0;
            u4RDither = (u4PanelDither & RDITHER_FUNCTION)? 1 : 0;
            u4EDither = (u4PanelDither & ERROR_DITHER_FUNCTION)? 1 : 0;
            u4LFSR = (u4PanelDither & LFSR_FUNCTION)? 1 : 0;
        }
        else
        {
            u4Round = 0;
            u4RDither = 1;
            u4EDither = 0;
            u4LFSR = 0;        
        }
        vRegWriteFldAlign(OUTSTG_OS_15, u4Round, OS_15_ROUND_EN);
        vRegWriteFldAlign(OUTSTG_OS_15, u4RDither, OS_15_RDITHER_EN);        
        vRegWriteFldAlign(OUTSTG_OS_15, u4EDither, OS_15_EDITHER_EN);
        vRegWriteFldAlign(OUTSTG_OS_15, u4LFSR, OS_15_DITHER_LFSR_EN);        
    }
    else
    {
        Printf("DISP_BIT=%d error\n", DISP_BIT);
    }
    if (u4PanelDither != 0xffffffff)
    {
        vRegWriteFldAlign(OUTSTG_OS_15, FPHASE_VALUE, OS_15_FPHASE_3_0_);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_15, 0, OS_15_FPHASE_3_0_);
    }

    #if (RB_CHANNEL_INVERSE == RB_CHANNEL_INVERSE_ON)
    {
        vRegWriteFldAlign(OUTSTG_OS_17, 1, OS_17_RB_INV);
    }
    #else
    {
        vRegWriteFldAlign(OUTSTG_OS_17, 0, OS_17_RB_INV);
    }
    #endif

    #if (BIT_ORDER_INVERSE == BIT_ORDER_INVERSE_ON)
    {
        vRegWriteFldAlign(OUTSTG_OS_17, 1, OS_17_MLSB_INV);
    }
    #else
    {
        vRegWriteFldAlign(OUTSTG_OS_17, 0, OS_17_MLSB_INV);
    }
    #endif

    if ((DISP_BIT == DISP_30BIT)
        #ifdef DRV_SUPPORT_EXTMJC
        || (IS_COMPANION_CHIP_ON() && (vDrvGetCurLvdsMode() !=  eLvdsVidNormal))
        #endif
        )
    {
        vRegWriteFldAlign(OUTSTG_OS_29, 1, OS_29_10B_EN);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_29, 0, OS_29_10B_EN);
    }
}


/**
 * @brief vDrvDisplayInit

 * Init the Display Settings

 * @param  void
 * @retval void
 */
void vDrvDisplayInit(void)
{
    vDrvOutputStageInit();

    // fixme!!
    #ifdef TTL24_OUTPUT
    // for TTL output
    IO_WRITE32(0x2000d404, 0, 0x50260224);
    IO_WRITE32(0x20025004, 0, 0x0fff9c30);
    IO_WRITE32(0x20025008, 0, 0x00100000);
    IO_WRITE32(0x2000d334, 0, ((IO_READ32(0x2000d334, 0)&0xffffff00)|0xff));
    IO_WRITE32(0x2000d320, 0, ((IO_READ32(0x2000d320, 0)&0xffff3fff)|0xc000));
    IO_WRITE32(0x20025154, 0, ((IO_READ32(0x20025154, 0)&0xffefffff)|0x00100000));
    #endif
    //SET DDDS_RESETB TO 1:normal operation
    vRegWriteFldAlign(OUTSTG_OS_E0, 1, OS_E0_DDDS_RESETB);

    //for osd alpha mix in both DE ON and DE OFF Region. by Linghui.zeng 	
    vRegWriteFldAlign(OUTSTG_OS_70, 1, OS_70_ALPHA_MIX_GRP1_0);

    vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_LSB_OFF);

    //SET P/I Gain default value
	vRegWriteFldAlign(OUTSTG_OS_E3, 1, OS_E3_DDDS_KI_1);
	vRegWriteFldAlign(OUTSTG_OS_E3, 3, OS_E3_DDDS_KP_1);
	vRegWriteFldAlign(OUTSTG_OS_E3, 3, OS_E3_DDDS_KI_0);
	vRegWriteFldAlign(OUTSTG_OS_E3, 1, OS_E3_DDDS_KP_0);

    //SET SCPOS_H_SEL to original vdoin: 0:mddi, 1:blk-ip, 2:vdoin
    vRegWriteFldAlign(OUTSTG_OS_E3, 2, OS_E3_SCPOS_H_SEL);

    //SET main/pip mute works immediately
    vRegWriteFldAlign(OUTSTG_OS_1A, 1, OS_1A_REG_MUTE_SEL);

    // Set error limit
    {
        UINT32 u4Error;
        if (DRVCUST_PanelQuery(eDDDSErrorLimit, &u4Error) == 0)
        {
            vDrvSetErrorLimit(u4Error);
        }
    }
    #ifndef CC_MTK_LOADER
    #ifdef SUPPORT_DDDS_STEP_TRACKING
    vDrvDDDSStepTrackingOnOff(SV_ON);
    #endif
    #endif
}

void vDrvLCDSetFrame(UINT16 wFrameRate)
{
	if(wFrameRate==50)
	{
		vDrvLCDFreqSet(SV_DCLK_50HZ, 50);
	}
	else if (wFrameRate==60)
	{
		vDrvLCDFreqSet(SV_DCLK_60HZ, 60);
	}
	else
	{
		vDrvLCDFreqSet(SV_DCLK_OTHERS, (UINT8)wFrameRate);
	}
}

/**
 * @brief vDrvLCDFreqSet

 * wDCLK Setting, MTK Good Dclk ON/OFF and the Display Output Timing for LCD panel

 * @param  bDCLKType: SV_DCLK_50HZ/SV_DCLK_60HZ
 * @param  bFrameRate: frame rate
 * @retval void
 */
void vDrvLCDFreqSet(UINT8 bDCLKType, UINT8 bFrameRate)    // called by Video
{
    static BOOL fgInit = FALSE;
    UINT32 dwDCLK = dwSI_DISPLAY_DCLK_TYPICAL;

    //LogSB("bDCLKType=", bDCLKType);

    if ((bDCLKType == bSI_DISPLAY_DCLK_TYPE)
        #ifndef CC_MTK_LOADER		
        && ((bOUTPUT_REFRESH_RATE == bFrameRate) || (bDCLKType != SV_DCLK_OTHERS))
        #endif        
        )
    {
        #ifndef CC_MTK_LOADER		
        vDrvSetSSMDRAsInputHsync();//for dynamic MDR
        #endif
        return;
    }

    // Update Current DCLK Type
    bSI_DISPLAY_DCLK_TYPE = bDCLKType;

    //  wDCLK  Setting
    switch(bDCLKType)
    {
    case SV_DCLK_50HZ:
        dwDCLK = dwSI_DISPLAY_DCLK_50HZ;
        bOUTPUT_REFRESH_RATE = 50;
        bSI_DISPLAY_DCLK_TYPE = SV_DCLK_50HZ;
        #ifndef CC_MTK_LOADER
        #ifdef CC_DRV_SUPPORT_SMICRO
        vSMicroPreSetFreq(FREQ_50HZ);
        #endif
        #endif        
        break;

    case SV_DCLK_60HZ:
        dwDCLK = dwSI_DISPLAY_DCLK_TYPICAL;
        bOUTPUT_REFRESH_RATE = 60;
        bSI_DISPLAY_DCLK_TYPE= SV_DCLK_60HZ;
        #ifndef CC_MTK_LOADER
        #ifdef CC_DRV_SUPPORT_SMICRO
        vSMicroPreSetFreq(FREQ_60HZ);
        #endif
        #endif        
        break;
#ifndef CC_MTK_LOADER		
#ifdef SUPPORT_PANEL_48HZ
    case SV_DCLK_48HZ:    
        dwDCLK = dwSI_DISPLAY_DCLK_48HZ;
        bOUTPUT_REFRESH_RATE = 48;
        #ifdef CC_DRV_SUPPORT_SMICRO
        vSMicroPreSetFreq(FREQ_48HZ);
        #endif
        break;
#endif
    case SV_DCLK_OTHERS:
        bOUTPUT_REFRESH_RATE = bFrameRate;
        if (bOUTPUT_REFRESH_RATE < 55)
        {
            dwDCLK = (dwSI_DISPLAY_DCLK_50HZ /50) * bOUTPUT_REFRESH_RATE;
            #ifndef CC_MTK_LOADER
            #ifdef CC_DRV_SUPPORT_SMICRO
            vSMicroPreSetFreq(FREQ_50HZ);
            #endif
            #endif        
        }
        else
        {
            dwDCLK = (dwSI_DISPLAY_DCLK_TYPICAL /60) * bOUTPUT_REFRESH_RATE;
            #ifndef CC_MTK_LOADER
            #ifdef CC_DRV_SUPPORT_SMICRO
            vSMicroPreSetFreq(FREQ_60HZ);
            #endif
            #endif        
        }
        bSI_DISPLAY_DCLK_TYPE= SV_DCLK_OTHERS;
        break;
#endif // end of #ifndef CC_MTK_LOADER		
    default:
        break;
    }

    if (fgInit)
    {
        BeforeOutputFrameRateChange(bOUTPUT_REFRESH_RATE);
    }

    vDrvSwitchMTKGoodDclk(SV_OFF);  //turn off DDDS 8202 (Dragon)

#ifndef CC_COPLAT_MT82
    OSD_PLA_Mute();
#endif

    vDrvSetLCDTiming();

#ifdef CC_MTK_LOADER
    vDrvVOPLLSet(dwDCLK);
#else
    vDrvVOFreqSet(dwDCLK);
    vDrvSetSSMDRAsInputHsync();//for dynamic MDR
#endif

    if (_fgFRS_ENA == SV_ON)
    {
        vDrvSwitchMTKGoodDclk(SV_ON);
    }

    if (fgInit)
    {
        AfterOutputFrameRateChange();
    }

    #ifndef CC_MTK_LOADER
    #ifdef CC_DRV_SUPPORT_SMICRO
    {
        SMICRO_VERTICAL_FREQ eFreq;
        SMICRO_MSG rMsg;
        #ifdef SUPPORT_PANEL_48HZ
        if (bOUTPUT_REFRESH_RATE == 48)
        {
            eFreq = FREQ_48HZ;
        }
        else 
        #endif    
        if (bOUTPUT_REFRESH_RATE < 55)
        {
            eFreq = FREQ_50HZ;
        }
        else
        {
            eFreq = FREQ_60HZ;
        }
        rMsg.u4ModuleId = MODULEID_PANEL_PARAM;
        rMsg.u4CommandId = CMD17_SET_VERTICAL_FREQ;
        rMsg.aPrm[0] = (UINT8)eFreq;
        rMsg.pCallbackFcn = NULL;
        vSMicroSendEvent(rMsg);  
    }
    #endif
    #endif        

    if (!fgInit)
    {
        fgInit = TRUE;
    }
}

/**
 * @brief wDrvCalOutputVTotal
 *
 * When DISP_MODE_USE is DISP_MODE_ON, Get the recommended V_TOTAL with additional VTotal limitation.
 *
 * @param  u1OutFrameRate: Output frame rate
 * @param  u2OutVTotalMin: Minimum Output Total Lines in the Vertical Direction
 * @param  u2OutVTotalMax: Maximum Output Total Lines in the Vertical Direction
 * @retval recommended V_TOTAL
 */
UINT16 wDrvCalOutputVTotal(UINT8 u1OutFrameRate, UINT16 u2OutVTotalMin, UINT16 u2OutVTotalMax)
{
    UINT32 u4VTotalMinByPixelClock;
    UINT32 u4VTotalMaxByPixelClock;
    UINT32 u4FrameRate;
    UINT32 u4HTotalMinBySyncMin;
    UINT32 u4HTotalMaxBySyncMax;

    if ((u2OutVTotalMin < DVS_TOTAL_MIN) && (u2OutVTotalMin != SCALE_VTOTAL_DO_NOT_CARE))
    {
        u2OutVTotalMin = DVS_TOTAL_MIN;
    }

    if ((u2OutVTotalMax > DVS_TOTAL_MAX) && (u2OutVTotalMax != SCALE_VTOTAL_DO_NOT_CARE))
    {
        u2OutVTotalMax = DVS_TOTAL_MAX;
    }

    u4FrameRate = (UINT32)u1OutFrameRate;

    u4HTotalMinBySyncMin = (UINT32) (wDISPLAY_WIDTH + CC_MOSYNCLEN_MIN + CC_RIGHTMASK_MIN);
    u4HTotalMinBySyncMin = ((u4HTotalMinBySyncMin + 1) & 0xfffffffe);     //make as even value
    u4HTotalMaxBySyncMax = (UINT32) (wDISPLAY_WIDTH + CC_MOSYNCLEN_MAX + CC_RIGHTMASK_MAX);
    u4HTotalMaxBySyncMax = ((u4HTotalMaxBySyncMax + 1) & 0xfffffffe);           //make as even value       

    if (u4HTotalMinBySyncMin < DHS_TOTAL_MIN)
    {
        u4HTotalMinBySyncMin = DHS_TOTAL_MIN;
    }
    if (u4HTotalMaxBySyncMax > DHS_TOTAL_MAX)
    {
        u4HTotalMaxBySyncMax = DHS_TOTAL_MAX;
    }    

    if (u4FrameRate ==0)
    {
        return  (0);
    }
    
    u4VTotalMinByPixelClock = (((UINT32) DCLK_MIN) / u4FrameRate) / u4HTotalMaxBySyncMax;
    u4VTotalMaxByPixelClock = (((UINT32) DCLK_MAX) / u4FrameRate) / u4HTotalMinBySyncMin;

    if ((u2OutVTotalMin < u4VTotalMinByPixelClock) && (u2OutVTotalMin != SCALE_VTOTAL_DO_NOT_CARE))
    {
        u2OutVTotalMin = u4VTotalMinByPixelClock;
    }

    if ((u2OutVTotalMax > u4VTotalMaxByPixelClock) && (u2OutVTotalMax != SCALE_VTOTAL_DO_NOT_CARE))
    {
        u2OutVTotalMax = u4VTotalMaxByPixelClock;
    }

    if (u2OutVTotalMin == SCALE_VTOTAL_DO_NOT_CARE)
    {
        u2OutVTotalMin = DVS_TOTAL_MIN;
    }   
    if (u2OutVTotalMax == SCALE_VTOTAL_DO_NOT_CARE)
    {
        u2OutVTotalMax = DVS_TOTAL_MAX;
    }

    if (u2OutVTotalMin < u2OutVTotalMax)
    {
        u2OutVTotalMin = ((u2OutVTotalMax+u2OutVTotalMin)>>1);
    }
    else
    {
        u2OutVTotalMin = 0;
    }

    return u2OutVTotalMin;
}


UINT8 vDrvGetLCDFreq(void)   
{
	return bOUTPUT_REFRESH_RATE;
}

/**
 * @brief vDrvSetDynamicDisplay

 * When DISP_MODE_USE is DISP_MODE_ON:
 * According to V_TOTAL That the Scaler Driver Calculates and HLEN_TOTAL_TYPI, Calculate Output Clock.
   If BiMode is SV_SUCCESS, Set Video Output Clock and the New Display Output Timing.

 * @param  BiMode: SV_SUCCESS/SV_FAIL
 * @retval void
 */
 #ifndef CC_MTK_LOADER
void  vDrvSetDynamicDisplay(UINT8 BiMode)
{
        static BOOL fgPreviousMode = FALSE;


    if (fgPreviousMode == BiMode)
    {
        if (BiMode)
        {
        }
        else
        {
            return;
        }
    }    

    if( _bIsImportProtection )
    {
 //       _bIsImportProtection = IMPORT_PROTECTION_RECOVER;
        vDrvSwitchImportProtection(1);
    }

    if (BiMode)
    {
        UINT32 u4PixelClock;
        UINT32 u4HSyncLen;
        UINT32 u4HTotalFinal;
        UINT32 u4VTotalFinal;

        UINT32 u4RefreshRate;
        UINT32 u4VTotal;
        UINT32 u4HTotal;

        UINT32 u4HTotalMinBySyncMin;
        UINT32 u4HTotalMinByPixelClock;
        UINT32 u4HTotalMaxByPixelClock;
        UINT32 u4RightMask;
        UINT32 u4BottomMask;
        UINT32 u4LeftMask;
        UINT32 wOldHTotal, wOldVTotal;

        u4RefreshRate = (UINT16) bDrvVideoGetRefreshRate(SV_VP_MAIN);
        u4VTotal = (UINT32) wCalculateOutputVTotal();

        if ((u4VTotal < DVS_TOTAL_MIN) ||
            (u4VTotal > DVS_TOTAL_MAX) ||
            (u4RefreshRate == 0))
        {

            if (WFB_PANEL_RESET == WFB_PANEL_RESET_ON)
            {
                LVDS_SwitchDataEnable(TRUE);
            }


            if( _bIsImportProtection )
            {
 //               _bIsImportProtection = IMPORT_PROTECTION;
                vDrvSwitchImportProtection(0);
            }
            return;
        }
        
        u4HTotalMinByPixelClock = (((UINT32) DCLK_MIN) / u4RefreshRate) / u4VTotal;
        u4HTotalMinByPixelClock = ((u4HTotalMinByPixelClock + 1) & 0xfffffffe);     //make as even value
        u4HTotalMaxByPixelClock = (((UINT32) DCLK_MAX) / u4RefreshRate) / u4VTotal;
        u4HTotalMaxByPixelClock = ((u4HTotalMaxByPixelClock - 1) & 0xfffffffe);     //make as even value
        u4HTotalMinBySyncMin = (UINT32) (wDISPLAY_WIDTH + CC_MOSYNCLEN_MIN + CC_RIGHTMASK_MIN);
        u4HTotalMinBySyncMin = ((u4HTotalMinBySyncMin + 1) & 0xfffffffe);           //make as even value

#ifdef CC_HTOTAL_8X_LIMIT_WA
        if (LVDS_OUTPUT_PORT == DUAL_PORT)
        {
            u4HTotalMinByPixelClock = (u4HTotalMinByPixelClock + 7) & (~7);
            u4HTotalMinBySyncMin = (u4HTotalMinBySyncMin + 7) & (~7);
            u4HTotalMaxByPixelClock = u4HTotalMaxByPixelClock & (~7);
        }
#endif

        // Find H-Total such that,
        // 1) Bigger than H-Total MIN calculate by DCLK_MIN
        // 2) Small than H-Total MAX calculate by DCLK_MAX
        // 3) Bigger than DHS_TOTAL_MIN
        // 4) Small than DHS_TOTAL_MAX
        // 5) Bigger than H-Total MIN calculate by CC_MOSYNCLEN_MIN
        // 6) Close to HLEN_TOTAL_TYPI

#ifdef CC_HTOTAL_8X_LIMIT_WA
        if (LVDS_OUTPUT_PORT == DUAL_PORT)
        {
            u4HTotal = (UINT32) ((HLEN_TOTAL_TYPI + 1) + 7) & (~7);
        }
        else
#endif
        {
            u4HTotal = (UINT32) (HLEN_TOTAL_TYPI + 1);
        }

        if (u4HTotal > u4HTotalMaxByPixelClock)
        {
            u4HTotal = u4HTotalMaxByPixelClock;
        }

#ifdef CC_HTOTAL_8X_LIMIT_WA
        if (LVDS_OUTPUT_PORT == DUAL_PORT)
        {
            if (u4HTotal > (UINT32) ((DHS_TOTAL_MAX + 1) & (~7)))
            {
                u4HTotal = (UINT32)((DHS_TOTAL_MAX + 1) & (~7));
            }
        }
        else
#endif
        {
            if (u4HTotal > (UINT32)(DHS_TOTAL_MAX + 1))
            {
                u4HTotal = (UINT32) (DHS_TOTAL_MAX + 1);
            }
        }

        if (u4HTotal < u4HTotalMinByPixelClock)
        {
            u4HTotal = u4HTotalMinByPixelClock;
        }

        if (u4HTotal < u4HTotalMinBySyncMin)
        {
            u4HTotal = u4HTotalMinBySyncMin;
        }

        if (u4HTotal < (UINT32) (DHS_TOTAL_MIN + 1))
        {
            u4HTotal = (UINT32) (DHS_TOTAL_MIN + 1);
        }

        // Pixel Clock
        u4PixelClock = (u4RefreshRate * u4VTotal) * u4HTotal;

        // [DTV00111681] Update related register and Do OSD_PLA_Mute only when u4VTotal or u4HTotal changed.
        // Peggy 20080109
        wOldHTotal = wDrvGetOutputHTotal();
        wOldVTotal = wDrvGetOutputVTotal(); 
        	
        if ((wOldHTotal != u4HTotal) || (wOldVTotal != u4VTotal))
        {        	
        #ifndef CC_COPLAT_MT82
            OSD_PLA_Mute();
        #endif
        
            if (WFB_PANEL_RESET == WFB_PANEL_RESET_ON)
            {
                LVDS_SwitchDataEnable(FALSE);
            }
    
            vDrvVOFreqSet(u4PixelClock);
           #ifndef CC_MTK_LOADER		
           vDrvSetSSMDRAsInputHsync();//for dynamic MDR
	    #endif	
    
            // HSync Length
            u4HSyncLen = (u4HTotal - wDISPLAY_WIDTH) - 1;
            if (u4HSyncLen >= CC_MOSYNCLEN_MAX)
            {
                u4HSyncLen = CC_MOSYNCLEN_MAX -1;
            }
    
            // H Total
            u4HTotalFinal = u4HTotal - 1;
    
            // V Total
            u4VTotalFinal = u4VTotal - 1;

            #if CC_FHD_FIXLENGH
            if ((wDISPLAY_WIDTH >= 1920) && (wDISPLAY_HEIGHT >= 1080)) // Full Screen workaround for FHD, Peggy
            {
                u4HSyncLen = u4HSyncLen -FHDFixLength;
            }
            #endif
    
            #if CC_SCPOS_VSYNC_WA1
            _u4VTotal = u4VTotalFinal;
//            _u4HSyncLen = _u4HSyncLen - (_u4HTotal - u4HTotalFinal);
            _u4HSyncLen = u4HSyncLen;
            _u4HTotal = u4HTotalFinal;
            #else
            vRegWriteFldAlign(SCPIP_PIP_OA_00, u4VTotalFinal, PIP_OA_00_MOVLEN);
            #ifndef CC_COPLAT_MT82
            _OSD_AlwaysUpdateReg(TRUE);
            vRegWriteFldAlign(SCPIP_PIP_OA_00, u4HTotalFinal, PIP_OA_00_MOHLEN);
            _OSD_AlwaysUpdateReg(FALSE);
            #else
            vRegWriteFldAlign(SCPIP_PIP_OA_00, u4HTotalFinal, PIP_OA_00_MOHLEN);
            #endif
            vRegWriteFldAlign(SCPIP_PIP_OA_01, u4HSyncLen-_u4OSTGDelta, PIP_OA_01_MOSYNCLEN);            
            #endif


//            u4LeftMask = RegReadFldAlign(OUTSTG_OS_17, OUTSTG_LEFT_MASK_7_0);        
            u4LeftMask = 0;
            #if CC_FHD_FIXLENGH
            if ((wDISPLAY_WIDTH >= 1920) && (wDISPLAY_HEIGHT >= 1080) && (u4LeftMask == 0)) // Full Screen workaround for FHD, Peggy
            {
                u4LeftMask += FHDFixLength;
            }
            #endif
            
            u4RightMask = ((u4HTotalFinal - u4HSyncLen) - wDISPLAY_WIDTH - u4LeftMask);
            u4BottomMask = (u4VTotalFinal - wDISPLAY_HEIGHT);
            
            #if CC_SCPOS_VSYNC_WA1
            _u1BottomMask = u4BottomMask;
            _u1LeftMask = u4LeftMask;
	    _u1RightMask = u4RightMask;
            _u4OutHVChanged = 1;
            vRegWriteFldAlign(OUTSTG_OS_16, 0, OS_16_TOP_MASK_7_0);
            #else        
            vDrvSetDENAMask(0, (UINT8)u4BottomMask, (UINT8)u4LeftMask, (UINT8)u4RightMask);
            #endif
    
            LOG(1, "DM OK HS(%d)\n", u4HSyncLen);
    
        }
        vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN), bDrvVideoGetRefreshRate(SV_VP_MAIN));
        vDrvSwitchMTKGoodDclk(SV_ON);    
        bSI_DISPLAY_DCLK_TYPE = SV_DCLK_OTHERS;
    }
    else
    {
        if ((bDrvVideoGetRefreshRate(SV_VP_MAIN) == 25) || (bDrvVideoGetRefreshRate(SV_VP_MAIN) == 50))
        {
            vDrvLCDFreqSet(SV_DCLK_50HZ, 50);
        }
        else if ((bDrvVideoGetRefreshRate(SV_VP_MAIN) == 30) || (bDrvVideoGetRefreshRate(SV_VP_MAIN) == 60))
        {
            vDrvLCDFreqSet(SV_DCLK_60HZ, 60);
        }
        else if (bDrvVideoGetRefreshRate(SV_VP_MAIN) == 24)
	 {
           #ifdef SUPPORT_PANEL_48HZ
    	    vDrvLCDFreqSet(SV_DCLK_48HZ, 48);
           #else
           vDrvLCDFreqSet(SV_DCLK_60HZ, 60);
           #endif
	 }
        vDrvSwitchMTKGoodDclk(SV_OFF);
    }
	
    if (WFB_PANEL_RESET == WFB_PANEL_RESET_ON)
    {
        LVDS_SwitchDataEnable(TRUE);
    }

    fgPreviousMode = BiMode;

    if( _bIsImportProtection )
    {
        vDrvFireImportPortection();
	//    vDrvSwitchImportProtection(0);
        _bIsImportProtection = IMPORT_PROTECTION_RECOVER;
    }
}
#endif // end of #ifndef CC_MTK_LOADER



void OSTG_SetPixelShift(BOOL fgEnable, INT32 i4HPixelShift, INT32 i4VPixelShift)
{
    BOOL fgOriPixelShiftEnable;
    
    if ((ABS(i4HPixelShift) > PIXEL_SHIFT_H_MAX) || (ABS(i4VPixelShift) > PIXEL_SHIFT_V_MAX))
    {
        //LOG(1, "pixel shift out of range, i4HPixelShift=%d, i4VPixelShift=%d\n",  i4HPixelShift, i4VPixelShift);
        return;
    }
    //LOG(5, "PMX_SetPixelShift, i4HPixelShift=%d, i4VPixelShift=%d\n",  i4HPixelShift, i4VPixelShift);

    fgOriPixelShiftEnable =  PANEL_IsPixelShift();
    PANEL_SetPixelShift(fgEnable, i4HPixelShift, i4VPixelShift);
     
    // turn on pixel shift
    if (fgEnable)
    {
//        _i4HPixelShift = i4HPixelShift;
        if (!fgOriPixelShiftEnable)
        {
        #ifndef CC_COPLAT_MT82
            IGNORE_RET(OSD_UpdateTiming());
        #endif
            //LOG(5, "turn on pixel shift, width=%d, height=%d\n", PANEL_GetPanelWidth(), PANEL_GetPanelHeight());
        }
        _u4OstgUpdateEvent |= OSTG_UPDATE_PIXEL_SHIFT_H;
        _u4OstgUpdateEvent |= OSTG_UPDATE_PIXEL_SHIFT_V;
    }
    else if (!fgEnable && fgOriPixelShiftEnable)     // previous is enable, restore timing
    {
#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        vDrvSetLVDSVPosition(PANEL_GetVPositionByTiming(bSI_DISPLAY_DCLK_TYPE));    
#else
        vDrvSetLVDSVPosition(V_POS);
#endif

        vDrvUpdateHPosition();
        #ifndef CC_COPLAT_MT82
        IGNORE_RET(OSD_UpdateTiming());
        #endif

      //  LOG(5, "turn off pixel shift, width=%d, height=%d\n", PANEL_GetPanelWidth(), PANEL_GetPanelHeight());
    }
}

void vDrvVideoSetMuteColor(UINT8 bPath, UINT32 u4Bg) 
{
    // luis@2006/8/14, set bgcolor at blanking
    //vScposSetBg(bPath, u4Bg);
    RDispInfo *prDispInfo;
#if SUPPORT_POP    
    if (bPath == SV_VP_MAIN)
#endif        
    {
        prDispInfo = &_rMDispInfo;
    }
#if SUPPORT_POP
    else
    {
        prDispInfo = &_rPDispInfo;
    }
#endif
    if (prDispInfo->u4MuteColor != u4Bg)
    {
        prDispInfo->u4MuteColor = u4Bg;
        if (bPath == SV_VP_MAIN)
        {
            vSetScposFlg(MAIN_MUTECOLOR_CHG);
        }
#if SUPPORT_POP
        else
        {
            vSetScposFlg(PIP_MUTECOLOR_CHG);
        }
#endif
    }
}

/**
* @brief Output stage normproc
*/
void vOstgNormProc(void)
{
#ifdef SUPPORT_CW_CHANGE_BY_STEP
    static UINT32 u4Val = 0;
    static UINT32 u4LocalTargetCW = 0;
    static BOOL isIncrease;
    static BOOL isFirstStep = TRUE;


    if( (u4LocalTargetCW != _u4TargetCW) && isFirstStep )
    {
        u4Val = _u4CurrentCW; //inital the first CW
        isIncrease = (_u4TargetCW > _u4CurrentCW ) ? TRUE : FALSE;
        isFirstStep = FALSE;
    }


    if( (_u4CurrentCW == _u4TargetCW) || (u4Val == _u4TargetCW) )
    {
        return;
    }



    if( 0xfffffff == _u4CWStep )
    {
        u4Val = _u4TargetCW;
    }
    else
    {
        if (isIncrease)
        {
            u4Val = ( (u4Val+_u4CWStep) >= _u4TargetCW ) ? _u4TargetCW : (u4Val+_u4CWStep);
        }
        else
        {
            u4Val = ( (u4Val-_u4CWStep) <= _u4TargetCW ) ? _u4TargetCW : (u4Val-_u4CWStep);
        }
    }

    if( u4Val == _u4TargetCW )
    {
        u4LocalTargetCW = u4Val;
        isFirstStep = TRUE;
    }

    
    #if defined(CC_MT5387) || defined(CC_MT5363)
    vRegWriteFldAlign(OUTSTG_OS_E2, u4Val, OS_E2_DDDS_FREQ_CW);
    #else
    #define REG_RW_VPLL_SPREAD_FREQ_CTRL    (0x04a4)
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_VPLL_SPREAD_FREQ_CTRL, u4Val);
    #endif

    
    vDrvVoDclkSpreadInit(1);
    HAL_Delay_us(100);
    vDrvVoDclkSpreadInit(0);   
#endif

    #ifndef CC_MTK_LOADER
    #ifdef SUPPORT_DDDS_STEP_TRACKING
    vDrvDDDSTrackingNormProc();
    #endif
    #endif  

    return;
}


// OSTG Event on Output VSync
// Note: Keep this function simple since this function is called by ISR
void OSTG_OnOutputVSync(void)
{
    vDrvOstgParaUpdate();
    
#ifndef CC_MTK_PRELOADER
    if (_u4OstgUpdateEvent & OSTG_UPDATE_PIXEL_SHIFT_H)
    {
        vDrvUpdateHPosition();
        _u4OstgUpdateEvent &= ~OSTG_UPDATE_PIXEL_SHIFT_H;
    }
    if (_u4OstgUpdateEvent & OSTG_UPDATE_PIXEL_SHIFT_V)
    {
#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        vDrvSetLVDSVPosition(PANEL_GetVPositionByTiming(bSI_DISPLAY_DCLK_TYPE));
#else
        vDrvSetLVDSVPosition(V_POS);
#endif

        _u4OstgUpdateEvent &= ~OSTG_UPDATE_PIXEL_SHIFT_V;
    }    
#endif

#ifndef CC_MTK_LOADER
    if (fgIsScposFlgSet(MAIN_MUTECOLOR_CHG))
    {
        vScposSetMuteColor(SV_VP_MAIN, _rMDispInfo.u4MuteColor);
        vClrScposFlg(MAIN_MUTECOLOR_CHG);
    }
#if SUPPORT_POP
    if (fgIsScposFlgSet(PIP_MUTECOLOR_CHG))
    {
        vScposSetMuteColor(SV_VP_PIP, _rPDispInfo.u4MuteColor);
        vClrScposFlg(PIP_MUTECOLOR_CHG);
    }
#endif
    if (_bIsImportProtection == IMPORT_PROTECTION_RECOVER)
    {
        _bIsImportProtection = IMPORT_PROTECTION;
        vDrvSwitchImportProtection(0);
    }
#endif 
}


void vDrvSwitchImportProtection(UINT8 bOn_Off)
{
    if ( bOn_Off )
    {
        vRegWriteFldAlign(OUTSTG_OS_E8, 0, OS_E8_SET_DATA_RDY);
        vRegWriteFldAlign(SCPIP_PIP_OA_01, 0, PIP_OA_01_SET_DATA_SEL);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_E8, 1, OS_E8_SET_DATA_RDY);
        vRegWriteFldAlign(SCPIP_PIP_OA_01, 1, PIP_OA_01_SET_DATA_SEL);
    }
}


void vDrvFireImportPortection(void)
{
	static UINT32 u4Toggle = 0;

    u4Toggle = (u4Toggle == 0)? 1 : 0;
	vRegWriteFldAlign(OUTSTG_OS_1E, u4Toggle, OS_1E_SET_DATA);
}

void vDrvSetErrorLimit(UINT32 u4Error)
{
    if (u4Error != _u4ErrorLimit)
    {
        _u4ErrorLimit = u4Error;
        vRegWriteFldAlign(OUTSTG_OS_E4, _u4ErrorLimit, OS_E4_DDDS_ERR_LIM);
    }
}

#ifndef CC_MTK_LOADER
#ifdef SUPPORT_SW_FRAME_CHECK

void vSetSWFrameTrackOnOff(UINT32 u4OnOff)
{
    if (_u4VDistTrackEn != u4OnOff)
    {
        LOG(3, "SW Frame Track (%d)\n", u4OnOff);
        _u4VDistTrackEn = u4OnOff;
    }
}

void vSetSWFrameTrackStep(UINT32 u4Step)
{
    if (_u4VDistTrackMaxStep != u4Step)
    {
        LOG(3, "SW Frame Track Step (%d)\n", u4Step);
        _u4VDistTrackMaxStep = u4Step;
    }
}

void vSetSWFrameTrackLockRange(UINT32 u4Upper, UINT32 u4Lower)
{
    if (_u4VDistUpperBound != u4Upper)
    {
        LOG(3, "SW Frame Track Upper Bound (%d --> %d)\n", _u4VDistUpperBound, u4Upper);
        _u4VDistUpperBound = u4Upper;
    }
    if (_u4VDistLowerBound != u4Lower)
    {
        LOG(3, "SW Frame Track Lower Bound (%d --> %d)\n", _u4VDistLowerBound, u4Lower);
        _u4VDistLowerBound = u4Lower;
    }
}

void vDrvVDistanceCheck(void)
{
    UINT32 u4VDistance, u4InputFrameRate;
    UINT32 u4LowerBound, u4UpperBound;
    UINT32 u4VTotal;
    UINT32 u4UpTrackRange, u4DownTrackRange;

    if (RegReadFldAlign(OUTSTG_OS_E3, OS_E3_DISP_EN) == 0)
    {
        return;
    }
    
    u4VDistance = u4ScposGetInOutVDistance();
    u4InputFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);

    #if 1
    u4VTotal = (UINT32)wDrvGetOutputVTotal();
    
    u4LowerBound = (_u4VDistLowerBound > 0x800)? (_u4VDistLowerBound-0x800) : (u4VTotal + _u4VDistLowerBound - 0x800);
    u4UpperBound = (_u4VDistUpperBound > 0x800)? (_u4VDistUpperBound-0x800) : (u4VTotal + _u4VDistLowerBound - 0x800);

    // get tracking distance
    u4UpTrackRange = 0;
    u4DownTrackRange = 0;
    if (u4UpperBound >= u4LowerBound)
    {
        if (u4VDistance > u4UpperBound)
        {
            u4DownTrackRange = u4VDistance - u4UpperBound;
            u4UpTrackRange = u4VTotal + u4LowerBound - u4VDistance;
        }
        else if (u4VDistance < u4LowerBound)
        {
            u4DownTrackRange = u4VDistance + u4VTotal - u4UpperBound;
            u4UpTrackRange = u4LowerBound - u4VDistance;        
        }
    }
    else  // u4UpperBound < u4LowerBound
    {
        if ((u4VDistance > u4UpperBound) && (u4VDistance < u4LowerBound))
        {
            u4DownTrackRange = u4VDistance - u4UpperBound;
            u4UpTrackRange = u4LowerBound - u4VDistance;        
        }
    }

    if (_u4VDistTrackEn)
    {
        if (_u4VDistTrackLocked < 2)
        {
            if ((u4UpTrackRange > _u4VDistTorance) && (u4DownTrackRange > _u4VDistTorance))
    {
        // start tracking
                _u4VDistTrackLocked = 0;
        _u4UpTrackRange = u4UpTrackRange;
        _u4DownTrackRange = u4DownTrackRange;
        if (u4UpTrackRange > u4DownTrackRange)
        {  // down tracking
            _u4VDistTrackState = 1;            
        }
        else
        {  // up tracking
            _u4VDistTrackState = 2;            
        }
                vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN), u4InputFrameRate);
            }
            else
            {
                _u4VDistTrackLocked ++;
                if (_u4VDistTrackLocked == 2)
                {
                    _u4VDistTrackState = 3;// SW frame track locked.
                    vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN),u4InputFrameRate);
                }
            }
        }
        else  if ((u4UpTrackRange > _u4VLossDistTorance) && (u4DownTrackRange > _u4VLossDistTorance))
        {
            _u4VDistTrackLocked = 0;
            vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_F_SYNC_1);
        }

    }
    else 
    {
        if (_u4VDistTrackState)
        {
            // recover
            _u4UpTrackRange = 0;
            _u4DownTrackRange = 0;
            _u4VDistTrackState = 0;
            _u4VDistTrackLocked = 0;
            vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN), u4InputFrameRate);
        }
    }
    
    #else
    
    if(u4VDistance >= V_DISTANCE_RANGE)
    {
        LOG(5,"u4VDistance(%d)\n",u4VDistance);
        u4CoarseStepSize = COARSE_STEP_SIZE;
        u4FineStepSize = FINE_STEP_SIZE;
        vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN));
    }
    else
    {   
        u4CoarseStepSize = 0;
        
        if(u4InOutVDistance != u4VDistance)
        {
            LOG(5,"IN OUT V DISTANCE CHANGED FROM (%d) TO (%d)\n",u4InOutVDistance,u4VDistance);

            if(u4VDistance < u4InOutVDistance)
            {
                if(u4CurrentStep<FINE_STEP_MAX)
                {
                    u4CurrentStep++;
                }
                else
                {
                    u4CurrentStep = FINE_STEP_MAX;
                }            
            }
            else
            {
                if(u4CurrentStep>FINE_STEP_MIN)
                {
                    u4CurrentStep--;
                }
                else
                {
                    u4CurrentStep = FINE_STEP_MIN;
                }        
            }
            
            LOG(5,"u4CurrentStep(%d)\n",u4CurrentStep);        
            
            //update current distance
            u4InOutVDistance = u4VDistance;
            
            vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN));
        }
        else
        {                
            u4NoChangeCounter++;
            if(u4NoChangeCounter == V_STABLE_COUNT)
            {
                LOG(5,"V DISTANCE IS STABLE(%d)\n",u4VDistance);
                u4NoChangeCounter = 0;
                u4CurrentStep = FINE_STEP_CENTER;
            }
        }
    }
    #endif
}

#endif
#endif
#ifdef CC_FLIP_MIRROR_SUPPORT
#include "eeprom_if.h"
UINT8 u1GetFlipMirrorConfig(void)
{
    static UINT8 u1Init = 0 ;
    static UINT8 u1FlipMirrorEnable= 0 ;
#ifdef CC_FLIP_MIRROR_FROM_EEPROM
	DTVCFG_T rDtvCfg;
#endif

	if(u1Init == 0 )
	{
#ifdef CC_FLIP_MIRROR_FROM_EEPROM
		if(EEPDTV_GetCfg(&rDtvCfg))  
		{
			Printf(" Get DTV config fail\n");
		}
		else
		{
			if(rDtvCfg.u1Flags2 & DTVCFG_FLAG2_FLIP_ON)	
			{
				u1FlipMirrorEnable |= SYS_FLIP_CONFIG_ON;
			}
			if(rDtvCfg.u1Flags2 & DTVCFG_FLAG2_MIRROR_ON)
			{
				u1FlipMirrorEnable |= SYS_MIRROR_CONFIG_ON;
			}
		}
#else
		#ifdef CC_FLIP_ENABLE
		u1FlipMirrorEnable |= SYS_FLIP_CONFIG_ON ;
		#endif
		#ifdef CC_MIRROR_ENABLE
		u1FlipMirrorEnable |= SYS_MIRROR_CONFIG_ON ;
		#endif
#endif
        if(IS_IC_5387() || (IS_IC_5363() && (BSP_GetIcVersion() < IC_VER_5363_AB)))
	{
		// 5387, skip mirror
		// 5363 before ECO, skip mirror
		u1FlipMirrorEnable &= ~(SYS_MIRROR_CONFIG_ON);
	}
		u1Init = 1 ;
	}                
    return u1FlipMirrorEnable;
}
#endif  //CC_FLIP_MIRROR_SUPPORT

#ifndef CC_MTK_LOADER
#ifdef SUPPORT_DDDS_STEP_TRACKING
void vDrvDDDSStepTrackingOnOff(UINT16 u2OnOff)
{
    if (_u2DDDSStepTrackingEn == u2OnOff)
    {
        return;
    }
    LOG(3, "DDDSStepTrackingOnOff: %d\n", u2OnOff);
    _u2DDDSStepTrackingEn = u2OnOff;

    // update error limit
    if (u2OnOff)
    {
        _u4DefaultErrorLimit =  RegReadFldAlign(OUTSTG_OS_E4, OS_E4_DDDS_ERR_LIM);
        vDrvSetErrorLimit(_u4DDDSTrackingErrorLimit);
    }
    else
    {
        vDrvSetErrorLimit(_u4DefaultErrorLimit);
    }
    
}

void vDrvUpdateDisplayCenterCW(UINT32 u4CW)
{

    if (_u4CenterCW == u4CW)
    {
        return;
    }    

    // update center frequence
    _u4CenterCW = u4CW;
    _u4CurrentStepCW = u4CW;

    // calculate CW upper/lower bound
    _u4StepInCWUpperBound = _u4CenterCW + ((_u4DefaultErrorLimit - _u4DDDSTrackingErrorLimit) * 32768);    
    _u4StepInCWLowerBound = _u4CenterCW - ((_u4DefaultErrorLimit - _u4DDDSTrackingErrorLimit) * 32768);   

    // if target freq delta is over  _u4CenterFreqStep, FW will update display freq.  
//    _u4StepInCW = _u4CenterFreq * (_u2DDDSTrackingSpeed - _u4CenterFreqStepInTolerance)/1000000;   
}

static void vDrvDDDSTrackingNormProc(void)
{
    if (_u2DDDSStepTrackingEn)
    {
        UINT32 u4CW;
        UINT32 u4CWUpperBound, u4CWLowerBound;
        
        // only step tracking with DDDS close loop
        if (RegReadFldAlign(OUTSTG_OS_E3, OS_E3_DISP_EN) == 0)
        {
            return;
        }
        if ((_u4CenterCW == 0) || (_u4CurrentStepCW == 0))
        {
            LOG(1, "Invalid DDDS center CW\n");
            return;
        }

        u4CW = RegReadFldAlign(OUTSTG_OS_E5, OS_E5_DDDS_FREQ_CW);
        u4CWUpperBound = _u4CurrentStepCW + (_u4DDDSTrackingErrorLimit * 32768) - _u4StepInCWTolerance;
        u4CWLowerBound = _u4CurrentStepCW - (_u4DDDSTrackingErrorLimit * 32768) + _u4StepInCWTolerance;
        if (u4CWUpperBound <= u4CWLowerBound)
        {
            LOG(2, "Invalid DDDS Step Tracking Bound (%d, %d)\n", u4CWLowerBound, u4CWUpperBound);
            return;
        }
        
        if ((u4CW > u4CWUpperBound) || (u4CW < u4CWLowerBound))
        {
            UINT32 u4NewCW;

            if (u4CW > u4CWUpperBound)
            {
                u4NewCW = _u4CurrentStepCW + (_u4DDDSTrackingErrorLimit * 32768); 
            }
            else //             (u4CW < u4CWLowerBound))
            {
                u4NewCW = _u4CurrentStepCW - (_u4DDDSTrackingErrorLimit * 32768); 
            }

            if (u4NewCW > _u4StepInCWUpperBound)
            {               
                u4NewCW = _u4StepInCWUpperBound;               
            }
            else if (u4NewCW < _u4StepInCWLowerBound)
            {
                u4NewCW = _u4StepInCWLowerBound;               
            }

            if (u4NewCW != _u4CurrentStepCW)
            {
                LOG(3, "DDDS Step Tracking: CW (0x%X) --> (0x%X)\n", _u4CurrentStepCW, u4NewCW);
                _u4CurrentStepCW = u4NewCW;
                vRegWriteFldAlign(OUTSTG_OS_E2, u4NewCW, OS_E2_DDDS_FREQ_CW);                                       
            }
            
            
        }

    }
}
#endif
#endif

LINT_RESTORE
