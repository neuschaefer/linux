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
#include "drv_scaler_drvif.h"
#include "vdp_display.h"
#include "scpos_debug.h"

#if defined(SCPIP_SUPPORT_POST_SCALER)

#ifndef CC_MTK_LOADER

#define PSC_FRAME_TRACK_TARGET  60

typedef struct
{
    UINT8 bPscModeChanged;
    RPicInfo* picInfo;
    RDispInfo* dispInfo;
    RDispPrm* dispPrm;
    VDP_SCALER_PRM_T* scalerInfo;
}PSC_INFO_T;

PSC_INFO_T pscInfo;
RPicInfo pscPicInfo;
RDispInfo pscDispInfo;
RDispPrm pscDispPrm;
VDP_SCALER_PRM_T pscScalerInfo;

UINT8 pscInitFlag = SV_FALSE;

//static function for post scaler initialize
static void _vPscInitPanelProtect(void);
static void _vPscUpdateDEAlignment(void);
static void _vPscInitWaterControl(void);
static void _vPscInitMjcDispmode(void);
static void _vPscInitSyncAdj(void);
static void _vPscInitDataFormat(void);

//static function for update display parameter
static void _vPscUpdateScaleFactor(void);
static void _vPscUpdateDisplayPrm(void);
static void _vPscUpdateHPorchPrm(void);
static void _vPscUpdateDisplayWindowPrm(void);    
static void _vPscUpdateInitPhasePrm(void);
static void _vPscUpdateDispmodePrm(void);

//static function for update post scaler register
static void _vPscUpdateReg(void);
static void _vPscSetImportOnOff(UINT8 bOnOff);                
static void _vPscSetUSReg(void);
static void _vPscSetHPorchReg(void);
static void _vPscSetDisplayWindowReg(void);
static void _vPscSetDispmodeReg(void);
static void _vPscModeSwitch(void);
static void _vPscUpdatePscanPrelen(void);
static void _vPscUpdateHlenPscan(void);
static void _vPscUpdateWaterControl(void);

#if defined(SUPPORT_TV3D_OVERSCAN)
static void _vPscUpdateInOutRegion3D(void);
#endif

static void _vPscUpdateInOutRegion2D(void);
static void _vPscUpdateInOutRegion(void);

EXTERN BOOL fgFrameTrackIsFlgSet(UINT32 u4Flag);
    
void vPscHwInit(void)
{
    //1. init psc panel protect
    _vPscInitPanelProtect();
    //2. init DE Alignment
    _vPscUpdateDEAlignment();
    //3. init line beffer water control
    _vPscInitWaterControl();
    //4. init MJC dispmode
    _vPscInitMjcDispmode();
    //5. init SyncAdj/Active Adj/Read Delay
    _vPscInitSyncAdj();
    //6. init psc data format
    _vPscInitDataFormat();

    //@5396 todo: remove this
    PSC_WRITE_FLD(PSSYSTEM_10, 1, PSSYSTEM_10_IMPORT_OFF_1);

    pscInitFlag = SV_TRUE;
}

void vPscFireModeChanged(void)
{
    pscInfo.bPscModeChanged = SV_TRUE;
}

void vPscClearModeChanged(void)
{
    pscInfo.bPscModeChanged = SV_FALSE;
}

UINT8 fgPscIsModeChanged(void)
{
    RVChannel* mainChannel;
    RVChannel* subChannel;
    UINT8 ret = SV_FALSE;

    mainChannel = getChannel(SV_VP_MAIN);
    subChannel = getChannel(SV_VP_PIP);
    
    if ( (mainChannel->bIsChannelOn ==SV_TRUE && mainChannel->bIsFreeze == SV_FALSE) || 
         (subChannel->bIsChannelOn ==SV_TRUE && subChannel->bIsFreeze == SV_FALSE) ||
         (pscInitFlag == SV_TRUE))   //channel is on & not freezed
    {
        if(pscInfo.bPscModeChanged == SV_TRUE)
        {
            ret = SV_TRUE;
        }
    }
    return ret;
}

void vPscSetImportOff(UINT8 bOnOff)
{
    if(bOnOff == SV_ON)
    {
        PSC_WRITE_FLD(PSSYSTEM_10, 1, PSSYSTEM_10_IMPORT_OFF_1);
    }
    else
    {
        PSC_WRITE_FLD(PSSYSTEM_10, 0, PSSYSTEM_10_IMPORT_OFF_1);    
    }
}

extern UINT32 GCD(UINT32 a, UINT32 b);

void vPscConfigFrameTrack(UINT32 u4Step)
{
    UINT8 u1InFR;
    UINT8 u1OutFR;
    UINT8 u1TrackVNum;
    //UINT32 u4PreVofst;
    UINT32 u4PanelVTotal;
    UINT32 u4UpperBound;
    UINT32 u4LowerBound;
    UINT32 u4LockRange;

    u1InFR = bDrvVideoGetRefreshRate(SV_VP_MAIN);
    u1OutFR = vDrvGetLCDFreq();
    
    if ((bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE) || (u1InFR==0) || (u1OutFR==0))
    	return;
        
    u1TrackVNum = u1InFR / GCD(u1InFR, u1OutFR);
    
    u4PanelVTotal = PSC_READ_FLD(PSPIP_OA_01, PSPIP_OA_01_PANEL_VTOTAL);
    //u4PreVofst = PSC_READ_FLD(PSPIP_OA_03, PSPIP_OA_03_PRE_VOFST_BYPASS1);
    
    //1. set frame track interval
    PSC_WRITE_FLD(PSPIP_OA_0D, u1TrackVNum-1, PSPIP_OA_0D_FRAME_TRACK_VSYNC_NUM);  

    //2. set frame track target. current's target is PRE_VOFST_BYPASS
    PSC_WRITE_FLD(PSPIP_OA_0D, 1, PSPIP_OA_0D_FRAME_TRACK_DIST_TARGET_MODE);  
    //PSC_WRITE_FLD(PSPIP_OA_0D, u4PreVofst, PSPIP_OA_0D_FRAME_TRACK_DIST_TARGET_VAL);
    PSC_WRITE_FLD(PSPIP_OA_0D, PSC_FRAME_TRACK_TARGET, PSPIP_OA_0D_FRAME_TRACK_DIST_TARGET_VAL);

    //3. reference front scaler output since Post scaler is in MJC dispmode.
    PSC_WRITE_FLD(PSPIP_OA_0D, 2, PSPIP_OA_0D_FRAME_TRACK_REF_VSYNC_SEL);  // Ref from front scaler
    
    //4. set to mask mode
    PSC_WRITE_FLD(PSPIP_OA_0D, 1, PSPIP_OA_0D_FRAME_TRACK_VSYNC_DIST_MODE);  
    
    //5. set lock speed
    u4Step /= u1TrackVNum;
    
    if(u4Step == 0)
    {
        u4Step = 1;
    }

    if(fgFrameTrackIsFlgSet(FRAME_TRACK_MODE_DDDS_BY_SCALER)) // DDDS frame track mode
    {
        //set to DDDS Adjust mode
        PSC_WRITE_FLD(PSPIP_OA_0D, 1, PSPIP_OA_0D_FRAME_TRACK_MODE);          
    }
    else 
    {
        //set to V Total Adjust mode
        PSC_WRITE_FLD(PSPIP_OA_0D, 0, PSPIP_OA_0D_FRAME_TRACK_MODE);
        PSC_WRITE_FLD(PSPIP_OA_0E, u4PanelVTotal - u4Step, PSPIP_OA_0E_VTOTAL_ADJUST_FAST01);
        PSC_WRITE_FLD(PSPIP_OA_0E, u4PanelVTotal - u4Step*2, PSPIP_OA_0E_VTOTAL_ADJUST_FAST10);          
        PSC_WRITE_FLD(PSPIP_OA_0F, u4PanelVTotal + u4Step, PSPIP_OA_0F_VTOTAL_ADJUST_SLOW01);
        PSC_WRITE_FLD(PSPIP_OA_0F, u4PanelVTotal + u4Step*2, PSPIP_OA_0F_VTOTAL_ADJUST_SLOW10);          
    }
    
    //6. set track range
    u4LockRange = u4Step * ((u1OutFR/GCD(u1InFR, u1OutFR))/2) + 2; // +2 for bigger lock range
    //6.1 upper bound2
    //u4UpperBound = u4PreVofst + u4LockRange*3;
    u4UpperBound = PSC_FRAME_TRACK_TARGET + u4LockRange*3;
    if (u4UpperBound > u4PanelVTotal)
    {
        u4UpperBound -= u4PanelVTotal;
    }
    PSC_WRITE_FLD(PSPIP_OA_10, u4UpperBound, PSPIP_OA_10_UPPER_BOUND2);  
    //6.2 upper bound1
    //u4UpperBound = u4PreVofst + u4LockRange;
    u4UpperBound = PSC_FRAME_TRACK_TARGET + u4LockRange;
    if(u4UpperBound > u4PanelVTotal)
    {
        u4UpperBound -= u4PanelVTotal;
    }
    PSC_WRITE_FLD(PSPIP_OA_10, u4UpperBound, PSPIP_OA_10_UPPER_BOUND1);  
    //6.3 lower bound2
    //u4LowerBound = (u4PreVofst > u4LockRange*3)?(u4PreVofst-(u4LockRange*3)):(u4PreVofst+u4PanelVTotal-(u4LockRange*3));
    u4LowerBound = (PSC_FRAME_TRACK_TARGET > u4LockRange*3)?(PSC_FRAME_TRACK_TARGET-(u4LockRange*3)):(PSC_FRAME_TRACK_TARGET+u4PanelVTotal-(u4LockRange*3));
    PSC_WRITE_FLD(PSPIP_OA_11, u4LowerBound, PSPIP_OA_11_LOWER_BOUND2);      
    //6.4 lower bound1
    //u4LowerBound = (u4PreVofst > u4LockRange)?(u4PreVofst - u4LockRange):(u4PreVofst + u4PanelVTotal - u4LockRange);
    u4LowerBound = (PSC_FRAME_TRACK_TARGET > u4LockRange)?(PSC_FRAME_TRACK_TARGET - u4LockRange):(PSC_FRAME_TRACK_TARGET + u4PanelVTotal - u4LockRange);
    PSC_WRITE_FLD(PSPIP_OA_11, u4LowerBound, PSPIP_OA_11_LOWER_BOUND1);
}

void vPscSetFrameTrackOnOff(UINT32 u4OnOff)
{
    #if defined(__MODEL_slt__) || defined(DRV_SUPPORT_EXTMJC) // Frame tracking always off for SLT and external MJC
    PSC_WRITE_FLD(PSPIP_OA_0D, 0, PSPIP_OA_0D_FRAME_TRACK_EN);
    #else
    if (u4OnOff)  // notice the order
    {    
         PSC_WRITE_FLD(PSPIP_OA_0D, SV_ON, PSPIP_OA_0D_FRAME_TRACK_EN);
    }
    else
    {
         PSC_WRITE_FLD(PSPIP_OA_0D, SV_OFF, PSPIP_OA_0D_FRAME_TRACK_EN);
    }
    #endif    
}

void vPscSwInit(void)
{
    x_memset(&pscPicInfo, 0, sizeof(pscPicInfo));
    x_memset(&pscDispInfo, 0, sizeof(pscDispInfo));
    x_memset(&pscDispPrm, 0, sizeof(pscDispPrm));
    x_memset(&pscScalerInfo, 0, sizeof(pscScalerInfo));
    
    pscInfo.bPscModeChanged = SV_FALSE;
    pscInfo.picInfo = &pscPicInfo;
    pscInfo.dispInfo = &pscDispInfo;
    pscInfo.dispPrm = &pscDispPrm;
    pscInfo.scalerInfo = &pscScalerInfo;    
}

static void _vPscInitPanelProtect(void)
{
    //init H panel protect (disable H protection)
    PSC_WRITE_FLD(PSPIP_OA_23, 1, PSPIP_OA_23_PANEL_HTOTAL_PROTECT_EN);
    PSC_WRITE_FLD(PSPIP_OA_23, 0x7FFF, PSPIP_OA_23_PANEL_HTOTAL_PROTECT);    
    //init H panel protect
    PSC_WRITE_FLD(PSPIP_OA_23, 1, PSPIP_OA_23_PANEL_VTOTAL_PROTECT_EN);
    PSC_WRITE_FLD(PSPIP_OA_23, 0x7FFF, PSPIP_OA_23_PANEL_VTOTAL_PROTECT);    
}

static void _vPscUpdateDEAlignment(void)
{
    UINT16 panelVActive;
    UINT8 usOffset = 1;
    panelVActive = wDISPLAY_HEIGHT;

    //1. set PANEL_VOFST
    PSC_WRITE_FLD(PSPIP_OA_0A, panelVActive, PSPIP_OA_0A_PANEL_VOFST);
    //2. set US_VOFST
    PSC_WRITE_FLD(PSPIP_OA_05, usOffset, PSPIP_OA_05_US_VOFST_BYPASS1);
    //3. set PRE_VOFST
    PSC_WRITE_FLD(PSPIP_OA_03, (panelVActive - usOffset), PSPIP_OA_03_PRE_VOFST_BYPASS1);    
    //4. set PRE_VOFST_DRAM
    PSC_WRITE_FLD(PSPIP1_0C, 0, PSPIP1_0C_PRE_VOFST_DRAM_1);
    //5. Set OSD_VOFST
    PSC_WRITE_FLD(PSPIP_OA_0A, panelVActive-7, PSPIP_OA_0A_OSD_VOFST);
}

static void _vPscInitWaterControl(void)
{
    //0. Init water control option. When set to 1, scaler will start to fill line buffer to init_thr once pscan or mjc return sync signal.
    PSC_WRITE_FLD(PSPIP1_13, 1, PSPIP1_13_WATER_INI_OPTION2_1);

    //1. init speed
    PSC_WRITE_FLD(PSPIP1_14, 0X80, PSPIP1_14_WTOKEN_VB_DS_1);
    PSC_WRITE_FLD(PSPIP1_14, 0X80, PSPIP1_14_WTOKEN_INI_DS_1);
    PSC_WRITE_FLD(PSPIP1_13, 0X80, PSPIP1_13_WTOKEN0_DS_1);
    PSC_WRITE_FLD(PSPIP1_13, 0X80, PSPIP1_13_WTOKEN1_DS_1);
    PSC_WRITE_FLD(PSPIP1_13, 0X80, PSPIP1_13_WTOKEN2_DS_1);    
    //2. init thr
    PSC_WRITE_FLD(PSPIP1_15, PSC_WATER_THR_INIT, PSPIP1_15_WTHR_INI_DS_1);
    PSC_WRITE_FLD(PSPIP1_15, PSC_WATER_THR3, PSPIP1_15_WTHR3_DS_1);
    PSC_WRITE_FLD(PSPIP1_16, PSC_WATER_THR2, PSPIP1_16_WTHR2_DS_1);
    PSC_WRITE_FLD(PSPIP1_16, PSC_WATER_THR1, PSPIP1_16_WTHR1_DS_1);    
}

static void _vPscInitMjcDispmode(void)
{
    //1. init HLEN_PSCAN
    PSC_WRITE_FLD(PSPIP1_0A, HLEN_TOTAL_TYPI-1, PSPIP1_0A_HLEN_PSCAN_1);
    
    //2. init VS_PSCAN_PRELEN
    PSC_WRITE_FLD(PSPIP1_0A, 0x20, PSPIP1_0A_VS_PSCAN_PRELEN_1);
    
    //3. post scaler in 5396 should always configured as MJC dispmode
    PSC_WRITE_FLD(PSPIP1_09, 1, PSPIP1_09_FMODESEL_1);
    
    //4. line buffer should be always configured as 1 line
    PSC_WRITE_FLD(PSPIP1_11, 0, PSPIP1_11_C_LINE_1);
    PSC_WRITE_FLD(PSPIP1_11, 0, PSPIP1_11_CFG_LINE_1);   

    //5. init RFIFO_PRO to 0
    PSC_WRITE_FLD(PSPIP1_11, 0, PSPIP1_11_RFIFO_PRO_1);
}

static void _vPscInitSyncAdj(void)
{
    PSC_WRITE_FLD(PSPIP_OA_02, PSC_HSYNC_ADJ, PSPIP_OA_02_HSYNC_ADJ);
    PSC_WRITE_FLD(PSPIP1_06, PSC_ACTIVE_ADJ, PSPIP1_06_ACTIVE_ADJ_1);
    PSC_WRITE_FLD(PSPIP1_06, PSC_READ_DELAY, PSPIP1_06_READ_DELAY_1);
}

static void _vPscInitDataFormat(void)
{
    //always set to 10 bit mode
    PSC_WRITE_FLD(PSSYSTEM_00, 1, PSSYSTEM_00_VD10BSEL);
}

static void _vPscUpdateScaleFactor(void) 
{
    RPicInfo* picInfo;
    RDispInfo* dispInfo;
    RDispPrm* dispPrm;
    UINT32 in;
    UINT32 out;
    
    picInfo = getPscPicInfo();
    dispInfo = getPscDispInfo();
    dispPrm = getPscDispPrm();
    
    in = picInfo->wTargetW;
    out = dispInfo->wWidth;

    
    if ( in == 0 || out == 0 || out <= in)
    {
        dispPrm->u4USclH = DFT_US_FACTOR;
    }
    else   // up scaling or down+up scaling case
    {     
        dispPrm->u4USclH =(in << DFT_US_BIT_NUM)/out;        

        if (dispPrm->u4USclH > DFT_US_FACTOR)
        {
            dispPrm->u4USclH = DFT_US_FACTOR;
        }
    }
}

static void _vPscUpdateInOutRegion2D(void)
{
    RDispInfo* pscDispInfo;
    RPicInfo* pscPicInfo;
	
    E_TDTV_RES_OUTPUT outType;

    pscPicInfo = getPscPicInfo();           //post scaler input
    pscDispInfo = getPscDispInfo();         //post scaler output

    //in 2D case, the input video of PSC is main-sub merged video. We can't just use main FSC's output as PSC's input.
    pscPicInfo->wXOffset = 0;
    pscPicInfo->wTargetW = ((wDISPLAY_WIDTH> u2FscGetMaxOutWidth())?  u2FscGetMaxOutWidth():wDISPLAY_WIDTH);

    if(_arScalePrm[VDP_1].u4121MapEn)   //for 2560 panel??
    {
    	if(_arScalePrm[VDP_1].u2121MapHPos == SCPIP_121MAP_H_POS_LEFT)
    	{
    	    pscDispInfo->wXOffset = 0;
    	}
    	else if(_arScalePrm[VDP_1].u2121MapHPos == SCPIP_121MAP_H_POS_RIGHT)
    	{
    	    pscDispInfo->wXOffset   = wDISPLAY_WIDTH -pscPicInfo->wTargetW ;
    	}
    	else // middle
    	{
    	    pscDispInfo->wXOffset   = (wDISPLAY_WIDTH -pscPicInfo->wTargetW)/2 ;
    	}
    	
        pscDispInfo->wWidth = pscPicInfo->wTargetW;
    }
    else
    {
	    //default set to full panel size
	    pscDispInfo->wXOffset = 0;
	    pscDispInfo->wWidth = wDISPLAY_WIDTH;
    }
    
    pscDispInfo->wYOffset = 0;
    pscDispInfo->wHeight = wDISPLAY_HEIGHT;

    //PST V
    outType = eScpipGetTdtvResolutionType(VDP_1, SCALER_TDTV_RES_POSITION_PST_H);
    if(outType == E_TD_RES_HALF)    //means MJC will make vertical to half, so scaler have to up scale to double height of panel.
    {
        pscPicInfo->wTargetW = pscPicInfo->wTargetW>>1;
    }
    else if(outType == E_TD_RES_DOUBLE) //means MJC will make vertical double, so scaler have to down to half of panel size
    {
        pscPicInfo->wTargetW = pscPicInfo->wTargetW<<1;    
    }
}

#if defined(SUPPORT_TV3D_OVERSCAN)
static void _vPscUpdateInOutRegion3D(void)
{
    RDispInfo* pscDispInfo;
    RPicInfo* pscPicInfo;
    RDispInfo* dispInfo;
    RPicInfo* picInfo;
	
    E_TDTV_RES_OUTPUT outType;

    pscPicInfo = getPscPicInfo();           //post scaler input
    pscDispInfo = getPscDispInfo();         //post scaler output
    dispInfo = getDispInfo(SV_VP_MAIN);
    picInfo = getPicInfo(SV_VP_MAIN);
        
    pscPicInfo->wXOffset = DIVIDE_WITH_ROUND( picInfo->wXOffset * dispInfo->wWidth, picInfo->wSrcW);
    pscPicInfo->wTargetW = DIVIDE_WITH_ROUND( picInfo->wTargetW * dispInfo->wWidth, picInfo->wSrcW);    
    pscPicInfo->wHTotal = wDrvGetOutputHTotal();

    //default set to full panel size
    pscDispInfo->wXOffset = dispInfo->wXOffset;
    pscDispInfo->wWidth = dispInfo->wWidth;    
    pscDispInfo->wYOffset = dispInfo->wYOffset;
    pscDispInfo->wHeight = dispInfo->wHeight;

    //PST H
    outType = eScpipGetTdtvResolutionType(VDP_1, SCALER_TDTV_RES_POSITION_PST_H);
    if(outType == E_TD_RES_HALF)    //means MJC will make vertical to half, so scaler have to up scale to double height of panel.
    {
        pscPicInfo->wXOffset = pscPicInfo->wXOffset>>1;
        pscPicInfo->wTargetW = pscPicInfo->wTargetW>>1;
        pscPicInfo->wHTotal = pscPicInfo->wHTotal>>1;
    }
    else if(outType == E_TD_RES_DOUBLE) //means MJC will make vertical double, so scaler have to down to half of panel size
    {
        pscPicInfo->wXOffset = pscPicInfo->wXOffset<<1;
        pscPicInfo->wTargetW = pscPicInfo->wTargetW<<1;    
        pscPicInfo->wHTotal = pscPicInfo->wHTotal<<1;
    }

    //PST V
    outType = eScpipGetTdtvResolutionType(VDP_1, SCALER_TDTV_RES_POSITION_PST_V);
    if(outType == E_TD_RES_HALF)    //means MJC will make vertical to half, so scaler have to up scale to double height of panel.
    {
        pscDispInfo->wYOffset = pscDispInfo->wYOffset>>1;
        pscDispInfo->wHeight = pscDispInfo->wHeight>>1;
    }
    else if(outType == E_TD_RES_DOUBLE) //means MJC will make vertical double, so scaler have to down to half of panel size
    {
        pscDispInfo->wYOffset = pscDispInfo->wYOffset<<1;
        pscDispInfo->wHeight = pscDispInfo->wHeight<<1;    
    }    
}
#endif

static void _vPscUpdateInOutRegion(void)
{
    #if defined(SUPPORT_TV3D_OVERSCAN)
    if(vScpipIs3DDoneByMJC() == SV_TRUE)
    {
        _vPscUpdateInOutRegion3D();
    }
    else
    #endif
    {
        _vPscUpdateInOutRegion2D();
    }
}

static void _vPscUpdateHPorchPrm(void)
{
    RPicInfo* picInfo;
    RDispPrm* dispPrm;

    picInfo = getPscPicInfo();
    dispPrm = getPscDispPrm();

    dispPrm->wHPosOfst = picInfo->wXOffset;
}

static void _vPscUpdateDisplayWindowPrm(void)
{
    RDispInfo* dispInfo;
    RDispPrm* dispPrm;

    dispInfo = getPscDispInfo();
    dispPrm = getPscDispPrm();

    dispPrm->wRHPos = dispInfo->wXOffset;
    dispPrm->wRHLen = dispInfo->wWidth;
    dispPrm->wRVPos = dispInfo->wYOffset;
    //RVL should not bigger than DISPLAY_HEIGHT
    dispPrm->wRVLen = MIN(dispInfo->wHeight, wDISPLAY_HEIGHT);
}

static void _vPscUpdateInitPhasePrm(void)
{

}

static void _vPscUpdateDispmodePrm(void)
{
    RDispPrm* dispPrm;
    RPicInfo* picInfo;

    dispPrm = getPscDispPrm();
    picInfo = getPscPicInfo();
    
    //1. update DISP_WIDTH_W (post scaler's data format is always 444 10bits. Divider = (120/30) = 4)

    #if defined(SUPPORT_TV3D_OVERSCAN)
    if(vScpipIs3DDoneByMJC() == SV_TRUE)
    {
        E_TDTV_RES_OUTPUT pstHType = eScpipGetTdtvResolutionType(SV_VP_MAIN, SCALER_TDTV_RES_POSITION_PST_H);        
        dispPrm->wHLen = DIVIDE_WITH_CARRIER(wDISPLAY_WIDTH, 4);

        if(pstHType == E_TD_RES_HALF)
        {
            dispPrm->wHLen = dispPrm->wHLen>>1;
        }
        else if(pstHType == E_TD_RES_DOUBLE)
        {
            dispPrm->wHLen = dispPrm->wHLen<<1;
        }
        else
        {
            dispPrm->wHLen = DIVIDE_WITH_CARRIER(picInfo->wTargetW, 4);
        }
    }
    else
    #endif
    {
        dispPrm->wHLen = DIVIDE_WITH_CARRIER(picInfo->wTargetW, 4);
    }
}

static void _vPscUpdateDisplayPrm()
{    
    //1. update H porch for overscan (HP_OFST)
    _vPscUpdateHPorchPrm();
        
    //2. update display window (RHP/ RVP/ RHL/ RVL)
    _vPscUpdateDisplayWindowPrm();
    
    //3. update H&V boundary protection setting (BOUNDARY_H_POS_1 & UP_VBOUND_Y1)
    //_vScpipUpdateHBoundaryPrm(u4VdpId);
    
    //4. update initial phase
    _vPscUpdateInitPhasePrm();

    //5. update display mode prm
    _vPscUpdateDispmodePrm();
}

static void _vPscSetImportOnOff(UINT8 bOnOff)
{
    LOG(1,"Set PSC import(%d)\n",bOnOff);
    PSC_WRITE_FLD(PSSYSTEM_10, bOnOff, PSSYSTEM_10_IMPORT_1);
}

static void _vPscSetUSReg(void)
{
    RDispPrm* dispPrm;
    dispPrm = getPscDispPrm();

    PSC_WRITE_FLD(PSUS1_00, dispPrm->u4USclH, PSUS1_00_USCALE_H_1);
    LOG(3,"PSC US(%x)\n",dispPrm->u4USclH);
}
static void _vPscSetHPorchReg(void)
{
    RDispPrm* dispPrm;
    dispPrm = getPscDispPrm();

    PSC_WRITE_FLD(PSPIP1_04, dispPrm->wHPosOfst, PSPIP1_04_HP_OFST_1);    
    LOG(3,"PSC HP_OFST(%d)\n",dispPrm->wHPosOfst);
}

static void _vPscSetDisplayWindowReg(void)
{
    RDispPrm* dispPrm;
    dispPrm = getPscDispPrm();

    PSC_WRITE_FLD(PSPIP1_03, dispPrm->wRHPos, PSPIP1_03_RHP_1);
    PSC_WRITE_FLD(PSPIP1_03, dispPrm->wRHLen, PSPIP1_03_RHL_1);
    PSC_WRITE_FLD(PSPIP1_02, dispPrm->wRVPos, PSPIP1_02_RVP_1);
    PSC_WRITE_FLD(PSPIP1_02, dispPrm->wRVLen, PSPIP1_02_RVL_1);    
    LOG(3,"PSC RHP(%d)\n",dispPrm->wRHPos);
    LOG(3,"PSC RHL(%d)\n",dispPrm->wRHLen);
    LOG(3,"PSC RVP(%d)\n",dispPrm->wRVPos);
    LOG(3,"PSC RVL(%d)\n",dispPrm->wRVLen);    
}

static void _vPscSetDispmodeReg(void)
{
    RDispPrm* dispPrm;
    dispPrm = getPscDispPrm();

    PSC_WRITE_FLD(PSPIP1_07, dispPrm->wHLen, PSPIP1_07_DISP_WIDTH_H_1);
    LOG(3,"PSC DISP_WIDTH_H(%d)\n",dispPrm->wHLen);    
}

static void _vPscUpdateReg(void)
{
    /**** start import protect ****/
    _vPscSetImportOnOff(SV_OFF);                

    //1. set H up scaling factor
    _vPscSetUSReg();               
    
    //2. set H porch
    _vPscSetHPorchReg();  

    //3. set display window (RHP,RVP,RHL,RVL)
    _vPscSetDisplayWindowReg();

    //4. set dispmode H width (DISP_WIDTH_H)
    _vPscSetDispmodeReg();   

    //5. set DE Alignment register
    _vPscUpdateDEAlignment();
    
    _vPscSetImportOnOff(SV_ON);
    /**** end of import protect ****/

    if(pscInitFlag == SV_TRUE)
    {
        PSC_WRITE_FLD(PSSYSTEM_10, 0, PSSYSTEM_10_IMPORT_OFF_1);
        pscInitFlag = SV_FALSE;
    }
}

static void _vPscModeSwitch(void)
{
    //1. update MJC dispmode VS_PSCAN_PRELEN
    _vPscUpdatePscanPrelen();
    //2. update MJC dispmode HLEN_PSCAN
    _vPscUpdateHlenPscan();
    //3. update MJC dispmode water control
    _vPscUpdateWaterControl();
}

static void _vPscUpdatePscanPrelen(void)
{
    UINT32 u4VBlanking;
    u4VBlanking = wDrvGetOutputVTotal() - wDISPLAY_HEIGHT;
    ASSERT((u4VBlanking-5) >= 6); // if PRELEN is set to < 6 it must be failed
    PSC_WRITE_FLD(PSPIP1_0A, MIN(u4VBlanking-5,0x20), PSPIP1_0A_VS_PSCAN_PRELEN_1);
}

static void _vPscUpdateHlenPscan(void)
{
    UINT16 hlenPscan;
    RPicInfo* picInfo;
    
    picInfo= getPscPicInfo();
    
    #if defined(SUPPORT_TV3D_OVERSCAN)
    if(vScpipIs3DDoneByMJC()== SV_TRUE)
    {
        hlenPscan = picInfo->wHTotal;
    }
    else
    #endif
    {
        hlenPscan = (picInfo->wTargetW * wDrvGetOutputHTotal()) / wDrvGetOutputHActive();
    }


    PSC_WRITE_FLD(PSPIP1_0A, hlenPscan - 1 ,PSPIP1_0A_HLEN_PSCAN_1);        
}

static void _vPscUpdateWaterControl(void)
{
    #if defined(SUPPORT_TV3D_OVERSCAN)
    if(vScpipIs3DDoneByMJC() == SV_TRUE)
    {
        UINT16 divider = wDISPLAY_WIDTH;
        E_TDTV_RES_OUTPUT pstHType = eScpipGetTdtvResolutionType(SV_VP_MAIN, SCALER_TDTV_RES_POSITION_PST_H);        

        if(pstHType == E_TD_RES_HALF)
        {
            divider = wDISPLAY_WIDTH>>1;
        }
        else if(pstHType == E_TD_RES_DOUBLE)
        {
            divider = wDISPLAY_WIDTH<<1;
        }
        else
        {
            divider = wDISPLAY_WIDTH;
        }

        PSC_WRITE_FLD(PSPIP1_15, (divider * PSC_WATER_THR_INIT) / wDISPLAY_WIDTH, PSPIP1_15_WTHR_INI_DS_1);
        PSC_WRITE_FLD(PSPIP1_15, (divider * PSC_WATER_THR3) / wDISPLAY_WIDTH, PSPIP1_15_WTHR3_DS_1);
        PSC_WRITE_FLD(PSPIP1_16, (divider * PSC_WATER_THR2) / wDISPLAY_WIDTH, PSPIP1_16_WTHR2_DS_1);
        PSC_WRITE_FLD(PSPIP1_16, (divider * PSC_WATER_THR1) / wDISPLAY_WIDTH, PSPIP1_16_WTHR1_DS_1);        
    }
    else
    #endif
    {
        RPicInfo* picInfo;
        picInfo = getPscPicInfo();
        PSC_WRITE_FLD(PSPIP1_15, ((UINT32)picInfo->wTargetW * PSC_WATER_THR_INIT) / wDISPLAY_WIDTH, PSPIP1_15_WTHR_INI_DS_1);
        PSC_WRITE_FLD(PSPIP1_15, ((UINT32)picInfo->wTargetW * PSC_WATER_THR3) / wDISPLAY_WIDTH, PSPIP1_15_WTHR3_DS_1);
        PSC_WRITE_FLD(PSPIP1_16, ((UINT32)picInfo->wTargetW * PSC_WATER_THR2) / wDISPLAY_WIDTH, PSPIP1_16_WTHR2_DS_1);
        PSC_WRITE_FLD(PSPIP1_16, ((UINT32)picInfo->wTargetW * PSC_WATER_THR1) / wDISPLAY_WIDTH, PSPIP1_16_WTHR1_DS_1);
    }
}

void vPSCDumpResInfo(void)
{
    RPicInfo* pscPicInfo;
    RDispInfo* pscDispInfo;

    pscPicInfo = getPscPicInfo();
    pscDispInfo = getPscDispInfo();
    
    Printf("[PSC picInfo]\n");
    Printf("PSC input xOffset........%5d\n",pscPicInfo->wXOffset);
    Printf("PSC input yOffset........%5d\n",pscPicInfo->wYOffset);
    Printf("PSC input TargetW........%5d\n",pscPicInfo->wTargetW);
    Printf("PSC input TargetH........%5d\n",pscPicInfo->wTargetH);
        
    Printf("[PSC dispInfo]\n");
    Printf("PSC output xOffset.......%5d\n",pscDispInfo->wXOffset);
    Printf("PSC output yOffset.......%5d\n",pscDispInfo->wYOffset);
    Printf("PSC output width.........%5d\n",pscDispInfo->wWidth);
    Printf("PSC output height........%5d\n",pscDispInfo->wHeight);
}

void vPscNormProc(void)
{
    if(fgPscIsModeChanged()==SV_TRUE)
    {    
        //0. update input & output size
        _vPscUpdateInOutRegion();
        //1. update H up scaling factor
        _vPscUpdateScaleFactor();
        //2. update display parameter
        _vPscUpdateDisplayPrm();
        //3. update MJC dispmode parameter
        _vPscModeSwitch();
        //4. update related register
        _vPscUpdateReg();
        //clear mode change
        vPscClearModeChanged();
    }
}
#endif

void vPscPresetDisplayWidth(void)
{
    UINT16 pscDisplayWidth;
    static UINT32 _pscPreset = 0;

    if (_pscPreset == 0)
    {
        _pscPreset = 1;

        // (post scaler's data format is always 444 10bits. Divider = (120/30) = 4)
        pscDisplayWidth = wDISPLAY_WIDTH / 4;
        PSC_WRITE_FLD(PSPIP1_07, pscDisplayWidth, PSPIP1_07_DISP_WIDTH_H_1);
    }   
}

#endif
