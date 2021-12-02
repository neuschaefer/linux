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

/*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: vdo_misc.c $
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 * Video Misc Function
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 04/05/31 8:25p $
 *
 * $Revision: #1 $
*************************************************************************/

#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "fbm_drvif.h"
#include "x_bim.h"
#include "x_hal_5381.h"

#include "general.h"
#include "hw_vdoin.h"
#include "hw_scpos.h"
#include "hw_ycproc.h"
#include "drv_video.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_tvd.h"
#include "vga_auto.h"
#include "drv_di.h"
#include "drv_adcset.h"
#include "drv_vdoclk.h"
#include "drv_autocolor.h"
#include "drv_hdtv.h"
#include "drv_vga.h"
#include "drv_dvi.h"
#include "drv_ycproc.h"

#include "drv_upscaler.h"

#include "vga_table.h"
#include "drv_contrast.h"
#include "source_select.h"
#include "drv_display.h"
#include "vdo_misc.h"
#include "drv_nr.h"
#include "video_def.h"
#include "api_backlt.h"
#include "nptv_drvif.h"
#include "sv_const.h"
#include "drv_tdc.h"
#include "x_lint.h"
#include "x_assert.h"
#if SUPPORT_SCART
#include "drv_scart.h"
#endif
#include "drv_async.h"
#include "drv_hdmi.h"
#if SUPPORT_VBI
#include "drv_vbi.h"
#endif
#include "drv_di.h"
#include "sif_if.h"
#include "drv_lvds.h"
#include "mute_if.h"
#include "b2r_drvif.h"
// add for 5371
#include "drv_mpeg.h"
#include "x_os.h"
#include "c_model.h"
#include "x_hal_5381.h" //  BSP_GetIcVersion
#include "hw_ckgen.h"
#include "nptv_debug.h"
#include "x_gpio.h"
#include "panel.h"

#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#include "srm_drvif.h"
#include "hw_scpos.h"
#include "hw_lvds.h"
#include "u_drv_cust.h"
#endif

//drv_display.c
extern BOOL _fgFRS_ENA;
extern UINT8 bSI_DISPLAY_DCLK_TYPE;

#ifdef DRV_SUPPORT_EXTMJC
extern UINT32 _u48280Unmte;
//UINT8 _bExtmjcICVersion;
#endif
//mloop_video.c


//drv_dvi.c
extern UINT16 _wDviVTotal;
extern UINT16  _wDviHtotal;

#if !TDC_USE_FBM_POOL
/*lint -e(950) */

#ifdef __KERNEL__
    // TDC_BUF_SIZE=0x251ca0 is too big for kernel BSS
#else
    static UINT8 __align(32) bTdcArry[TDC_BUF_SIZE];
#endif
#endif

#ifdef CC_UP8032_ATV
UINT32 xdata u4SWRegArea[1]  _at_   0xdf00;
#endif

extern UINT32 _u4VPllFrequency;//for dynamic MDR

extern UINT8 _bFixColorSpace[2];
UINT8 bSUPPORT_VGA_422=FALSE;
void vSetVGA422En(UINT8 bOnOff)
{
    bSUPPORT_VGA_422 = bOnOff;
}

UINT8 u1GetVGA422En(void)
{
    return bSUPPORT_VGA_422;
}

UINT8 bIsScalerInput444(UINT8 bPath) reentrant
{
#if !SUPPORT_POP
#define bPath SV_VP_MAIN
#endif
    UINT8 bType;
    UINT8 bIs444;
    if (bPath >= SV_VP_NA)
    {
        LOG(0, "bIsScalerInput444 path error!\n");
        return SV_FALSE;
    }

    if(bSUPPORT_VGA_422 && (bGetSignalType(bPath) == SV_ST_VGA))
    {
        return (0);
    }
    else
    {
    switch(_bFixColorSpace[bPath]&FIXCOLORSPACE_MODEMASK)
    {
        case SV_FIXCOLORSPACE_OFF:
                break;
        case SV_FIXCOLORSPACE_RGB_444:
        case SV_FIXCOLORSPACE_YCBCR_TO_RGB:
                return SV_TRUE;
        default:
                return SV_FALSE;
    }

    bType = bGetVideoDecType(bPath);

    switch (bType)
    {
    #if SUPPORT_DVI
        case SV_VD_DVI:
            //if(bHDMIInputType() && fgIsVgaTiming(_bDviTiming))
            if(fgIsVgaTiming(_bDviTiming))
            {
                 bIs444 =SV_TRUE;
            }
            else
            {
                bIs444 =SV_FALSE;
            }
            break;
    #endif
        default:
        	if(fgApiVideoIsVgaMode(bPath, 0))
        	{
        		 bIs444 = SV_TRUE ;
        	}
        	else
        	{
        		bIs444 =SV_FALSE;
        	}
        	/*
            if (bPath == SV_VP_MAIN)
            {
                bIs444 = _rMChannel.bIs444;
            }
            else
            {
                bIs444 = _rPChannel.bIs444;
            }*/
            break;
    }
    return bIs444;
    }
#if !SUPPORT_POP
#undef bPath
#endif
}


// *********************************************************************
// Function : void vDrvVideoSwInit(void)
// Description : This function is used for video initialization
// Parameter :
// Return    :
// *********************************************************************
void vDrvVideoSwInit(void)
{
    /* Software     */
    x_memset(&_rMChannel,0,sizeof(_rMChannel));
#if SUPPORT_POP
    x_memset(&_rPChannel,0,sizeof(_rPChannel));
#endif

    x_memset(&_rTvd3dStat, 0, sizeof(_rTvd3dStat));
    x_memset(&_rVGAStat, 0, sizeof(_rVGAStat));
    x_memset(&_rYPBPRStat, 0, sizeof(_rYPBPRStat));
    x_memset(&_rDVIStat, 0, sizeof(_rDVIStat));

    // add for 5371
    x_memset(&_rMpegSDStat, 0, sizeof(_rDVIStat));
    x_memset(&_rMpegHDStat, 0, sizeof(_rDVIStat));

    x_memset(&_rMDispInfo, 0, sizeof(_rMDispInfo));
#if SUPPORT_POP
    x_memset(&_rPDispInfo, 0, sizeof(_rPDispInfo));
#endif
    x_memset(&_rMDispPrm, 0, sizeof(_rMDispPrm));
#if SUPPORT_POP
    x_memset(&_rPDispPrm, 0, sizeof(_rPDispPrm));
#endif
    x_memset(&_rMPicInfo, 0, sizeof(_rMPicInfo));
#if SUPPORT_POP
    x_memset(&_rPPicInfo, 0, sizeof(_rPPicInfo));
#endif
    _rMPicInfo.wUIHPorch = 1000;
    _rMPicInfo.wUIVPorch = 1000;
#if SUPPORT_POP
    _rPPicInfo.wUIHPorch = 1000;
    _rPPicInfo.wUIVPorch = 1000;
#endif

//#ifdef DRV_SUPPORT_EXTMJC
//    _bExtmjcICVersion= bGetICVersion();
    //Printf("_bExtmjcICVersion %d\n", _bExtmjcICVersion);
//#endif

    // Event Flag
  vClrMainFlg(0xff);
  vClrPipFlg(0xff);
  vClrScposFlg(0xffffffff);

    _wEventFlag = 0;

    // Vdo Main/Pip State
    _bMainState = VDO_STATE_IDLE;
#if SUPPORT_POP
    _bPipState = VDO_STATE_IDLE;
#endif
    // TVD
    _bSetChannelCnt = 0x40;

    // source_select.c
    _bSrcMainOld =  (UINT8)SV_VS_MAX;
    _bSrcSubOld =  (UINT8)SV_VS_MAX;
    _bSrcMainNew =  (UINT8)SV_VS_MAX;
    _bSrcSubNew =  (UINT8)SV_VS_MAX;

/* For Combi Component, we should keep monitored information */
    _bMonMainOld =  (UINT8)SV_VS_MAX;
    _bMonSubOld =  (UINT8)SV_VS_MAX;
    _bMonMainNew =  (UINT8)SV_VS_MAX;
    _bMonSubNew =  (UINT8)SV_VS_MAX;

    _bMainMonICIn =  (UINT8)P_FA;
    _bSubMonICIn =  (UINT8)P_FA;

#ifdef DRV_SUPPORT_EXTMJC
    _u48280Unmte = 0;
#endif

    // vga_auto.c
    _bAutoFlag = 0;
    _bVdoSP0AutoState = VDO_AUTO_NOT_BEGIN; //0;
    _bVgaDelayCnt = 0;

    //vdo_clk.c
    _bCLKSetFlag = 0;

    // drv_autocolor.c
    _bAutoColorState0 = VDO_AUTO_COLOR_NOT_BEGIN;
    // drv_tvd.c
    #if TVD_AUTO_CALIB
    _bTVDAutoCalibState = VDO_AUTO_COLOR_NOT_BEGIN;
    #endif

    // drv_hdtv.c
    _bHdtvTiming = NO_SIGNAL;

    // drv_vga.c
    #if SUPPORT_VGA_USERMODE
    _bVgaUserMode = 0;
    #endif

#if SUPPORT_DVI
    //drv_dvi.c
    _bDviExtSrc = SV_DV_NONE;
  vClrDviFlg(0xFF);
#endif

    // hw_clk.c
    _fgFRS_ENA = SV_OFF;

    // video_data.c

    // for mute control
    _fgMainBlueMute = FALSE;
    _fgSubBlueMute = FALSE;
    _fgMainBlackMute = FALSE;
    _fgSubBlackMute = FALSE;

    vDrvScpipInit();

        //#if SUPPORT_SCART_SFIRST
#if SUPPORT_SCART  //Gene Chang For the SCART Auto to remove the S first function 2006.05.12
        //_Enable_SCART_SF = 0;  //Gene Chang For the SCART Auto to remove the S first function 2006.05.12
        _bSFisSV =SV_FALSE;
#endif

#if SUPPORT_SCART
        _bTrigScartAuto= SV_FALSE;
#endif

    _fgVideoReady = FALSE;
    //_fgEnableMdChgMute = TRUE;

    _fgAutoSearch = FALSE;
    // drv_constrast.c
    vDrvAutoContrastInit();
    //mtk01140 20090916 move to HW init, after ycproc init
    //vDrvDBLInit();
    VSS_Support_Initial();

}

// *********************************************************************
// Function : void vDrvVideoHwInit(void)
// Description :
// Parameter :
// Return    :
// *********************************************************************
void vDrvVideoHwInit(void)
{
#ifdef COUNTRY_FROM_EEPROM
	UINT8 u1TargetCountryInfo;
#endif
    #ifndef CC_UP8032_ATV
    FBM_POOL_T*  _prFbmPool;
    #endif

   #if 0
   #ifdef DRV_SUPPORT_EXTMJC
   if (IS_COMPANION_CHIP_ON())
   {
        UINT8 u1MsgErr;

        // mute for transition garbage
        //drv_extmjc_set_lvds_enable(SV_FALSE, &u1MsgErr);
        d_custom_extmjc_set_fastmute(TRUE, &u1MsgErr);
        // NSRC & PSRC setting (by ACD designer)
#ifndef DRV_SUPPORT_EXTMJC_MODE0
#ifdef DRV_SUPPORT_EXTMJC_MODE3
        vDrvSetCurLvdsMode(eLvdsVidYuv422TwoPort);
#else
        vDrvSetCurLvdsMode(eLvdsVidRgb444OnePort);
#endif //DRV_SUPPORT_EXTMJC_MODE3
#endif //DRV_SUPPORT_EXTMJC_MODE0

//        u1MsgErr = 0x60;
//        SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2 , 0x55, (UINT8 *)&u1MsgErr, 0x1);
//        LOG(2, "\nTurn off 8280 LVDS DE\n\n");
//        Printf("Fast Blank Mute!\n\nTurn off 8280 LVDS DE\n\n");

        _u48280Unmte = 25;
   }
   #endif
   #endif

    // Clock
    // Video ADC testing
    vDrvADCDefaultSetting();//5371

    #ifndef CC_UP8032_ATV
    _prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_VBI);
    // ASSERT(_prFbmPool != NULL);
    VBI_DRAM_BASE = _prFbmPool->u4Addr;
    VBI_DRAM_SIZE = _prFbmPool->u4Size;

    _prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_NR);
    // ASSERT(_prFbmPool != NULL);
    NR_DRAM_BASE = _prFbmPool->u4Addr;
    NR_DRAM_SIZE = _prFbmPool->u4Size;

#if /*__KERNEL__ ||*/ TDC_USE_FBM_POOL
    _prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_TDC);
    // ASSERT(_prFbmPool != NULL);
    TDC_DRAM_BASE = _prFbmPool->u4Addr;
    TDC_DRAM_SIZE = _prFbmPool->u4Size;
#else
    TDC_DRAM_BASE = (UINT32)bTdcArry;
    TDC_DRAM_SIZE = TDC_BUF_SIZE;
#endif

/*
	// Put SW Reg base address into 0x2700
	_prFbmPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_PQ_TOOL, NULL);
	SW_DRAM_BASE = _prFbmPool->u4Addr;
	SW_DRAM_SIZE = _prFbmPool->u4Size;
	x_memset((void *)SW_DRAM_BASE, 0, SW_DRAM_SIZE);
	vRegWrite4B(0x2700, SW_DRAM_BASE);
*/
    // TDC Dram
    vDrvTDCSetDramBase();
    #endif

    // DI Init
    vDrvDIInit(SV_VP_MAIN);
//#if SUPPORT_POP
    vDrvDIInit(SV_VP_PIP);
//#endif
    // TDC init
    vDrvTDCInit();
    // vDrvTDCOnOff(SV_ON);

    // NR Init
    vDrvNRInit();
    //vDrvNROnOff(SV_ON);

    // Adaptive control Init
    vDrvAdaptiveFlgInit();
    vDrvAdaptivePSCANFlgInit();

    //vSetMainMuteColor(SV_MUTE_BLACK);
    //vSetPipMuteColor(SV_MUTE_BLACK);
    vScposSetMuteColor(SV_VP_MAIN, 0);
#if SUPPORT_POP
    vScposSetMuteColor(SV_VP_PIP, 0);
#endif
    // 5371 -- MUTE OFF
    //// Marvin Start
    _vDrvVideoSetMute(MUTE_MODULE_BLUE, SV_VP_MAIN, SV_OFF, FALSE);
#if SUPPORT_POP
    _vDrvVideoSetMute(MUTE_MODULE_BLUE, SV_VP_PIP, SV_OFF, FALSE);
#endif
    _vDrvVideoSetMute(MUTE_MODULE_BLACK, SV_VP_MAIN, SV_OFF, FALSE);
#if SUPPORT_POP
    _vDrvVideoSetMute(MUTE_MODULE_BLACK, SV_VP_PIP, SV_OFF, FALSE);
#endif
    //// Marvin End

    #ifdef COUNTRY_FROM_EEPROM
    u1TargetCountryInfo = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;;
    if (u1TargetCountryInfo == COUNTRY_EU)
//    #ifndef CC_UP8032_ATV
    #else
    if(GetTargetCountry()==COUNTRY_EU)
    #endif
    {
        vDrvLCDFreqSet(SV_DCLK_50HZ, 50);
    }
    else
    {
     // if panel already on, don't set again.
       if((GPIO_Enable(DRVCUST_PanelGet(eBackLightGpio), NULL) != 1) ||
        (GPIO_Output(DRVCUST_PanelGet(eBackLightGpio), NULL) != DRVCUST_PanelGet(eBackLightOnVal)))
        {
            vDrvLCDFreqSet(SV_DCLK_60HZ, 60);
        }
        else
        {
            bSI_DISPLAY_DCLK_TYPE = SV_DCLK_60HZ;
            bOUTPUT_REFRESH_RATE = 60;
            _u4VPllFrequency = dwSI_DISPLAY_DCLK_TYPICAL;//for dynamic MDR
            #ifndef CC_MTK_LOADER
            vDrvSetSSMDRAsInputHsync();//for dynamic MDR
            #endif
            vDrvSwitchImportProtection(0);
            #ifndef CC_MTK_LOADER
            #ifdef SUPPORT_DDDS_STEP_TRACKING
            vDrvUpdateDisplayCenterCW(RegReadFldAlign(OUTSTG_OS_E2, OS_E2_DDDS_FREQ_CW));
            #endif
            #endif
    
        }
    }

    //----Main Vsync inverse
    #if SUPPORT_POP
    vRegWriteFldAlign(SCPIP_PIP2_01, 0, PIP2_01_HL_2);
    vRegWriteFldAlign(SCPIP_PIP2_01, 0, PIP2_01_HP_2);
    vRegWriteFldAlign(SCPIP_PIP2_00, 0, PIP2_00_VL_2);
    vRegWriteFldAlign(SCPIP_PIP2_00, 0, PIP2_00_VP_2);
    #endif

    #if TVD_DFT_PAL
    //fg525 = FALSE;
    //vTvd3dSetDefType(fg525);//MT5381TVDPort Removed
    // PreSet Firmware TVD Status, for TVD mode reference
    _rTvd3dStatus.bTvdMode = SV_CS_PAL;
    _rTvd3dStatus.fgIs525 = FALSE;
    _rTvd3dStatus.fgVPres = FALSE;
    #else
    //fg525 = TRUE;
    //vTvd3dSetDefType(fg525);//MT5381TVDPort Removed
    // PreSet Firmware TVD Status, for TVD mode reference
    _rTvd3dStatus.bTvdMode = SV_CS_NTSC358;
    _rTvd3dStatus.fgIs525 = TRUE;
    _rTvd3dStatus.fgVPres = FALSE;
    #endif

    _rTvd3dStatus.fgSVPres = FALSE;
    _rTvd3dStatus.bColSys = SV_CS_MAX;
    _bTvdHoldMode = SV_FALSE;

    // YCPROC Init
    vDrvYcprocInit();
    //mtk01140 20090916 move to HW init, after ycproc init
    vDrvDBLInit();    

    vScposInit(SV_VP_MAIN);
#if SUPPORT_POP
       vScposInit(SV_VP_PIP);
#endif

    vDrvUpScalerInit();

#ifdef CC_MT5391
	vDrvMJCInit();
#endif

    //min valid h-sync width 48->40.
    vRegWriteFldAlign(TG_00, 0x0A, MIN_HWIDTH); //MT5381TVDPort
    // vRegWriteFldAlign(SYS_00, 1, DATA_SYNC_HIGH); // 1: Fail cause VGA 1920x1080@60Hz flash.
    vRegWriteFldAlign(SYS_00, 0, DATA_SYNC_HIGH);
    // when mpclk < main clk, set DATA_SYNC_HIGH to 0 to avoid overflow (send request every 2 pixels)
    // when mpclk > main clk, set DATA_SYNC_HIGH to 1 to reduce HSync jitter

#if MANUAL_MAIN_RST
    //default value is 1
    vRegWriteFldAlign(SYS_00, 0, DATA_SYNC_AUTO);
#endif
	// enable omux V align with H
    vRegWriteFldAlign(SYS_05, 1, FV_OUT_ALIGN);


    #if !defined(CC_MT8223)
    // 180 = TVDPLL /3 , 162 (216) = SYSPLL/2 = 324/2 = 162
    SET_VDOIN_MPCLK(MPCLK_180MHZ);
    #endif

    vIO32Write4B(CKGEN_VPCLK_STOP, 0);  //enable all videoin clock




#ifdef __MODEL_slt__
    //turn off dither
    vRegWrite4BMsk(0x20027454, 0x00800000, 0x00800000); //LSB_OFF	23
    vRegWrite4BMsk(0x20027454, 0x00000000, 0x00400000); //DRMOD1	22
    vRegWrite4BMsk(0x20027454, 0x00000000, 0x00008000); //DRMOD0	15
    vRegWrite4BMsk(0x20027454, 0x00000000, 0x00004000); //ROUND_EN	14
    vRegWrite4BMsk(0x20027454, 0x00000000, 0x00002000); //RDITHER_EN	13
    vRegWrite4BMsk(0x20027454, 0x00000000, 0x00001000); //EDITHER_EN    12
    vRegWrite4BMsk(0x20027464, 0x00000000, 0x04000000); //LFSR_DITHER_EN26

    //turn off error diffusion
    vRegWrite4BMsk(0x2002381C, 0x00000000, 0x00000200); //Front3x3(Main)_En     9
    vRegWrite4BMsk(0x20023C1C, 0x00000000, 0x00000200); //Front3x3(Sub)_En  	9
    vRegWrite4BMsk(0x200244C0, 0x00000000, 0x00010000); //PSCAN_DITHER_EN	16
    vRegWrite4BMsk(0x200244C0, 0x00000000, 0x00C00000); //BGDldmod		23:22
    vRegWrite4BMsk(0x200244C0, 0x00000000, 0x00200000); //BGDledither_en        21
    vRegWrite4BMsk(0x200244C0, 0x00000000, 0x00100000); //BGDlsfr_en      	20
    vRegWrite4BMsk(0x200244C0, 0x00000000, 0x00080000); //BGDlrdither_en	19
    vRegWrite4BMsk(0x200244C0, 0x00000000, 0x00040000); //BGDlround_en		18
    vRegWrite4BMsk(0x200244C0, 0x00020000, 0x00020000); //BGDllsb_off		17
#endif

#ifdef CC_PSCAN_FIELD_TOGGLE_WA
    if (IS_IC_5363() && (BSP_GetIcVersion() >= IC_VER_5363_AB))
    {
        vRegWrite4BMsk(0x487C, 0x00000002, 0x00000002); //field toggle
    }
#endif

}


// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
BOOL fgIsTvdHLock(void)
{
    return fgHwTvdHLock();
}

BOOL fgIsTvdVLock(void)
{
    return fgHwTvdVLock();
}

// *********************************************************************
// Function :
// Description :
//  (1) It is used for VBI parameter setting.
//  (2) The VDOSRC check should be reviewed.
//      Possible Setting:
//          VDOSRC_TVD3D - NTSC/PAL
//          VDOSRC_HDTV - 480i/576i/480p/576p
//          VDOSRC_DVI - 480i/576i/480p/576p (AD9883, MST9888 Digital Port input).
//  (3) The Video Size check should be reviewed.
//      Possible Setting
//          VDOSRC_TVD3D - by _rTVD3dStatus
//          VDOSRC_HDTV - by _bHdtvTiming
//          VDOSRC_DVI - by _bDviTiming
//  (4) (2)+(3) Merge to VDOSRC and timing check
//  (5) The return type should review again.
//          We only support VDOTYPE_NTSC and VDOTYPE_PAL two VBI system.
//          But we also need to set different setting for TVD/HDTV/DVI input.
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoGetType(UINT8 bPath)
{
    UINT8 bDecType;
    UINT8 u1Ret;
    // 1. Check Channel Decoder
#if SUPPORT_POP
    bDecType = bGetVideoDecType(bPath);
#else
    bDecType = bGetVideoDecType(SV_VP_MAIN);
#endif

    // 2. Check VDOSRC and Timing
    switch (bDecType)
    {
        case SV_VD_TVD3D:
            if (_rTvd3dStatus.fgIs525)
            {
                u1Ret = VDOTYPE_NTSC;
            }
            else
            {
                u1Ret = VDOTYPE_PAL;
            }

            break;
        case SV_VD_YPBPR:
            if ((_bHdtvTiming==MODE_525I) || (_bHdtvTiming==MODE_525I_OVERSAMPLE) ||
            (_bHdtvTiming==MODE_480P) || (_bHdtvTiming==MODE_480P_OVERSAMPLE))
            {
                u1Ret = VDOTYPE_NTSC;
            }
            else if ((_bHdtvTiming==MODE_625I) || (_bHdtvTiming==MODE_625I_OVERSAMPLE) ||
            (_bHdtvTiming==MODE_576P) || (_bHdtvTiming==MODE_576P_OVERSAMPLE))
            {
                u1Ret = VDOTYPE_PAL;
            }
            else
            {
                u1Ret = VDOTYPE_OTHER;
            }

            break;

        default:
            u1Ret = VDOTYPE_OTHER;

            break;
    }

    return (u1Ret);


}

// *********************************************************************
// Function : void vDrvVideoAllocateDram(void)
// Description :
// Parameter :
// Return :
// *********************************************************************
void vDrvVideoAllocateDram(void)
{
}


// *********************************************************************
// The following are video call-back functions.
// *********************************************************************

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
void vDrvVideoConnect(UINT8 bPath, UINT8 bOnOff)
{
#if !SUPPORT_POP
#define bPath SV_VP_MAIN
#endif
    UINT8 bType;

    bType = bGetVideoDecType(bPath);

    #ifndef CC_UP8032_ATV
    ASSERT(BIM_DisableIrq(VECTOR_VDOIN));
    #else
    vDisableSysInt2(VDOIN_INT_EN);
    #endif

    vDrvSetPorchTune(bPath,SV_PORCHTUNE_DEC);
    switch (bType)
    {
      case SV_VD_TVD3D:
#if SUPPORT_SCART
          /* considering SCART-RGB */
          if(_bMainICIn == P_FB0)
          {
              vDrvSetPorchTune(bPath, SV_PORCHTUNE_SCPOS);
          }
#endif
          vTvd3dConnect(bPath, bOnOff);
          break;
      case SV_VD_YPBPR:
          vHdtvConnect(bPath, bOnOff);
          break;
      case SV_VD_VGA:
          vVgaConnect(bPath, bOnOff);
#ifdef DRV_SUPPORT_EXTMJC_MODE7
    vDrvExtmjcRedoTwoChipConnection(SV_VP_MAIN,SV_TRUE);
#endif          
          break;
    #if SUPPORT_DVI
      case SV_VD_DVI:
          vDviConnect(bPath, bOnOff);
          break;
    #endif
    #ifndef COPLAT_EXCLUDE_DTV
    // Add for 5371
      case SV_VD_MPEGHD:
        vDrvSetPorchTune(bPath,SV_PORCHTUNE_SCPOS);
        vMpegHdConnect(bPath, bOnOff);
        break;
      case SV_VD_MPEGSD:
        vDrvSetPorchTune(bPath,SV_PORCHTUNE_SCPOS);
        vMpegSdConnect(bPath, bOnOff);
        break;
    #endif
    default:
        break;
    }

#if 0 //def DRV_SUPPORT_EXTMJC_MODE7
    vDrvExtmjcRedoTwoChipConnection(SV_VP_MAIN,SV_TRUE);
#endif

    #ifndef CC_UP8032_ATV
    ASSERT(BIM_EnableIrq(VECTOR_VDOIN));
    #else
    vEnableSysInt2(VDOIN_INT_EN);
    #endif
#if !SUPPORT_POP
#undef bPath
#endif
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
void vDrvVideoTrigModeDetect(UINT8 bPath)
{
    UINT8 bType;
#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    if (bType == (UINT8)SV_VD_TVD3D)
    {
        vTvd3dTrigModeDet();
    }
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoInputWidth(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wRet = 0;
    // irq_save(flags);

#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
        case SV_VD_TVD3D:
            wRet = wTvdWidth();
            break;

        case SV_VD_YPBPR:
            wRet = wHdtvInputWidth();
            break;

        case SV_VD_VGA:
            wRet = wVgaInputWidth();
            break;

        #if SUPPORT_DVI
        case SV_VD_DVI:
            wRet = wDviInputWidth();
            break;
        #endif

        #ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
          wRet = u2MpegHdInputValidDataWidth();
          break;

        case SV_VD_MPEGSD:
          wRet = u2MpegSdInputValidDataWidth();
          break;
        #endif
        default:
            break;

    }

    // irq_restore(flags);

    return wRet;
}
#if 0
// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoInputValidDataWidth(UINT8 bPath)
{
    UINT8 bType;

    UINT16 wRet = 0;

    bType = bGetVideoDecType(bPath);

    if ((bType != (UINT8)SV_VD_MPEGHD) &&
        (bType != (UINT8)SV_VD_MPEGSD))
    {
        return wDrvVideoInputWidth(bPath);
    }

    #ifndef COPLAT_EXCLUDE_DTV
    switch (bType)
    {
        case SV_VD_MPEGHD:
          wRet = u2MpegHdInputValidDataWidth();
          break;

        case SV_VD_MPEGSD:
          wRet = u2MpegSdInputValidDataWidth();
          break;

        default:
          break;
    }
    #endif


    return wRet;
}
#endif
// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoInputDEWidth(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wRet = 0;

    bType = bGetVideoDecType(bPath);

    switch (bType)
    {
        case SV_VD_DVI:
            wRet = wDviGetAvWidth();
            break;

        case SV_VD_YPBPR:
        case SV_VD_VGA:
            wRet = wHdtvDEInputWidth();
            break;

        default:
        	wRet = wDrvVideoInputWidth(bPath);
            break;
    }

    return wRet;
}
// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoInputHeight(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wRet = 0;

#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
        case SV_VD_TVD3D:
            wRet = wTvd3dHeight();
            break;

        case SV_VD_YPBPR:
            wRet = wHdtvInputHeight();
            break;

        case SV_VD_VGA:
            wRet = wVgaInputHeight();
            break;

        #if SUPPORT_DVI
        case SV_VD_DVI:
            wRet = wDviInputHeight();
            break;
        #endif

        #ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
          wRet = u2MpegHdInputHeight();
          break;

        case SV_VD_MPEGSD:
          wRet = u2MpegSdInputHeight();
          break;
        #endif

        default:
            break;
    }


    return wRet;
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoGetRefreshRate(UINT8 bPath)
{
    UINT8 bType;
    UINT8 bRet = 0;

#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
        case SV_VD_TVD3D:
                bRet = bTvd3dRefreshRate();
            break;

        case SV_VD_YPBPR:
            bRet = bHdtvRefreshRate();
            break;

        case SV_VD_VGA:
            bRet = bVgaRefreshRate();
            break;

        #if SUPPORT_DVI
        case SV_VD_DVI:
	      //bRet = bDviRefreshRate();
	      bRet = bHDMIRefreshRate();
            break;
        #endif

        #ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
          bRet = u1MpegHdRefreshRate();
          break;

        case SV_VD_MPEGSD:
          bRet = u1MpegSdRefreshRate();
          break;
        #endif

        default:
          break;
    }

    return bRet;
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoIsSrcInterlace(UINT8 bPath)
{
    UINT8 bType;
    UINT8 bRet = 0;

#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif
    switch (bType)
    {
        case SV_VD_TVD3D:
            bRet = SV_TRUE;
            break;

        case SV_VD_YPBPR:
            bRet = bHdtvInterlace();
            break;

        case SV_VD_VGA:
            bRet = bVgaInterlace();
            break;

        #if SUPPORT_DVI
        case SV_VD_DVI:
            bRet = bDviInterlace();
            break;
        #endif

        #ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
          bRet = u1MpegHdInterlace();
          break;

        case SV_VD_MPEGSD:
          bRet = u1MpegSdInterlace();
          break;
        #endif

        default:
            break;
    }

    return bRet;
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoGetVPorch(UINT8 bPath)
{
    UINT8 bType;

#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
        case SV_VD_YPBPR:
            return wHdtvVPorch(bPath);

        case SV_VD_VGA:
            return Displaymode_delay; //Modify for Disaplymode delay

              case SV_VD_DVI:
                return 0;

        default:
            return 0;
    }
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoGetHPorch(UINT8 bPath)
{
    UINT8 bType;

#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
#ifdef MT5360B_WA4
    case SV_VD_VGA:
        return MT5360B_WA4_DLY1;

   case SV_VD_DVI:
        return MT5360B_WA4_DLY1;
#else
        case SV_VD_YPBPR:
            return wHdtvHPorch(bPath);
#endif
        default:
            return 0;
    }
}
// *********************************************************************
// Function : For dynamic backlight get status with different criteria (Snow screen is no signal)
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoSignalStatus_DBL(UINT8 bPath)
{
    UINT8 bRet = (UINT8)SV_VDO_NOSIGNAL;

    //irq_save(flags);
    if (bGetVideoDecType(bPath) == SV_VD_TVD3D)
    {
        bRet = _rTvd3dStatus.bSigStatus;
    }
    else
    {
        bRet = bDrvVideoSignalStatus(bPath);
    }
    return bRet;
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoSignalStatus(UINT8 bPath)
{
    UINT8 bType;
    UINT8 bRet = (UINT8)SV_VDO_NOSIGNAL;

    //irq_save(flags);
#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
      case SV_VD_TVD3D:
          bRet = bTvd3dSignalStatus();
          break;
      case SV_VD_YPBPR:
          bRet = bHdtvSigStatus();
          break;
      case SV_VD_VGA:
                bRet = bVgaSigStatus();
          break;
    #if SUPPORT_DVI
      case SV_VD_DVI:
                bRet = bDviSigStatus();
          break;
    #endif

        #ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
          bRet = u1MpegHdSigStatus();
          break;

        case SV_VD_MPEGSD:
          bRet = u1MpegSdSigStatus();
          break;
        #endif

          default:
            break;

    }

    //irq_restore(flags);

    return bRet;
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoGetHTotal(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wHTotal = 0;
#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
        case SV_VD_TVD3D:
                wHTotal =  wTvdHTotal();
            break;

        case SV_VD_YPBPR:
            if (_bHdtvTiming < MAX_TIMING_FORMAT)
            {
                wHTotal =  Get_VGAMODE_IHTOTAL(_bHdtvTiming);
                if(bHdtvCheckCenEnable())
                {
                    wHTotal = wHTotal / 2 ;
                }
            }
            break;

        case SV_VD_VGA:
            if (_bVgaTiming < MAX_TIMING_FORMAT)
            {
                wHTotal = Get_VGAMODE_IHTOTAL(_bVgaTiming);
                if(bHdtvCheckCenEnable())
                {
                    wHTotal = wHTotal / 2 ;
                }
            }
            break;

    #if SUPPORT_DVI
        case SV_VD_DVI:
            if(_bDviTiming == MODE_DE_MODE)
            {
                wHTotal =  _wDviHtotal ;
            }
            else if (_bDviTiming < MAX_TIMING_FORMAT)
            {
                wHTotal =  Get_VGAMODE_IHTOTAL(_bDviTiming);
            }
            if(bDVICENStatus())
            {
                wHTotal = wHTotal / 2 ;
            }
            break;
    #endif

        #ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
          wHTotal = u2MpegHdHTotal();
          break;

        case SV_VD_MPEGSD:
          wHTotal = u2MpegSdHTotal();
          break;
        #endif

        default:
            break;
    }

    return wHTotal;
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
#define VGA_YPBPR_DECODER_VTOTAL     1
UINT16 wDrvVideoGetVTotal(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wVTotal = 0;
#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
        case SV_VD_TVD3D:
            wVTotal =wTvdVTotal();
            break;

        case SV_VD_YPBPR:
            if (_bHdtvTiming < MAX_TIMING_FORMAT)
            {
                wVTotal =  Get_VGAMODE_IVTOTAL(_bHdtvTiming);
            }
            break;

        case SV_VD_VGA:
            if (_bVgaTiming < MAX_TIMING_FORMAT)
            {
#if VGA_YPBPR_DECODER_VTOTAL
                //wVTotal = Get_VGAMODE_IVTOTAL(_bVgaTiming);
                if(fgIsVideoTiming(_bVgaTiming))
                {
                    wVTotal = Get_VGAMODE_IVTOTAL(_bVgaTiming);
                }
                else
                {
                	wVTotal = _wSP0StableVtotal;
				}
#else
                // get from measured vtotal
                if(fgIsVideoTiming(_bVgaTiming))
                {
                    wVTotal = Get_VGAMODE_IVTOTAL(_bVgaTiming);
                }
                else
                {
                    if(bPath ==  SV_VP_MAIN)
                    {
                        wVTotal = RegReadFldAlign(SCPIP_DS1_RO_00, DS1_RO_00_DS1_INVLEN_1);
                    }
                    else // SuB
                    {
                        wVTotal = RegReadFldAlign(SCPIP_DS2_RO_00, DS2_RO_00_DS1_INVLEN_2);
                    }
                }
#endif
            }
            break;

    #if SUPPORT_DVI
        case SV_VD_DVI:
            if(_bDviTiming == MODE_DE_MODE)
            {
                wVTotal = _wDviVTotal ;
            }
            else if (_bDviTiming < MAX_TIMING_FORMAT)
            {
                wVTotal =  Get_VGAMODE_IVTOTAL(_bDviTiming);
            }
            break;
    #endif

        #ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
          wVTotal = u2MpegHdVTotal();
          break;

        case SV_VD_MPEGSD:
          wVTotal = u2MpegSdVTotal();
          break;
        #endif
        default:
            break;

    }

    return wVTotal;
}
#if 0
// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoIsSrc1080i(UINT8 bPath)
{
    UINT8 bType;
       UNUSED(bPath);
    bType = bGetVideoDecType(bPath);

    switch (bType)
    {
        case SV_VD_YPBPR:
            if ((_bHdtvTiming == MODE_1080i) || (_bHdtvTiming == MODE_1080i_50))
            {
                return TRUE;
            }
            break;

        case SV_VD_VGA:
            if ((_bVgaTiming == MODE_1080i) || (_bVgaTiming == MODE_1080i_50))
            {
                return TRUE;
            }
            break;

        #if SUPPORT_DVI
        case SV_VD_DVI:
            if ((_bDviTiming == MODE_1080i) || (_bDviTiming == MODE_1080i_50))
            {
                return TRUE;
            }
            break;
        #endif
        default:
        break;

    }

    return FALSE;
}
#endif
// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoGetTiming(UINT8 bPath)
{
    UINT8 bType;
#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
        case SV_VD_TVD3D:
            if (_rTvd3dStatus.fgIs525)
            {
                return MODE_525I;
            }
            else
            {
                return MODE_625I;
            }

        case SV_VD_YPBPR:
            return _bHdtvTiming;

        case SV_VD_VGA:
            return _bVgaTiming;

        #if SUPPORT_DVI
        case SV_VD_DVI:
            return _bDviTiming;
        #endif

        #ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
          return u1MpegHdTiming();

        case SV_VD_MPEGSD:
          return u1MpegSdTiming();
        #endif

        default:
            break;
    }

    return MODE_NOSIGNAL;
}

const CHAR *strDrvVideoGetTimingString(UINT8 bTiming)
{
    switch (bTiming)
    {
        case MODE_525I_OVERSAMPLE:
            return "MODE_525I_OVERSAMPLE";
        case MODE_625I_OVERSAMPLE:
            return "MODE_625I_OVERSAMPLE";
        case MODE_480P_OVERSAMPLE:
            return "MODE_480P_OVERSAMPLE";
        case MODE_576P_OVERSAMPLE:
            return "MODE_576P_OVERSAMPLE";
        case MODE_720p_50:
            return "MODE_720p_50";
        case MODE_720p_60:
            return "MODE_720p_60";
        case MODE_1080i_48:
            return "MODE_1080i_48";
        case MODE_1080i_50:
            return "MODE_1080i_50";
        case MODE_1080i:
            return "MODE_1080i";
        case MODE_1080p_24:
            return "MODE_1080p_24";
        case MODE_1080p_25:
            return "MODE_1080p_25";
        case MODE_1080p_30:
            return "MODE_1080p_30";
        case MODE_1080p_50:
            return "MODE_1080p_50";
        case MODE_1080p_60:
            return "MODE_1080p_60";
        case MODE_525I:
            return "MODE_525I";
        case MODE_625I:
            return "MODE_625I";
        case MODE_480P:
            return "MODE_480P";
        case MODE_576P:
            return "MODE_576P";
        case MODE_720p_24:
            return "MODE_720p_24";
        case MODE_720p_25:
            return "MODE_720p_25";
        case MODE_720p_30:
            return "MODE_720p_30";
        case MODE_240P:
            return "MODE_240P";
        case MODE_540P:
            return "MODE_540P";
        case MODE_480P_24:
            return "MODE_480P_24";
        case MODE_480P_30:
            return "MODE_480P_30";
        case MODE_576P_25:
            return "MODE_576P_25";
        case MODE_MAX:
        default:
            return "Not Support OR No Signal";
    }
}

UINT8 bDrvVideoGetSourceTypeTiming(UINT8 bPath) reentrant
{
#if !SUPPORT_POP
#define bPath SV_VP_MAIN
#endif
#if SUPPORT_SCART
    if ((bPath == SV_VP_MAIN && VSS_MAJOR(_bSrcMainNew) == VSS_SCART)
#if SUPPORT_POP
    || (bPath == SV_VP_PIP && VSS_MAJOR(_bSrcSubNew) == VSS_SCART)
#endif
    )
    {
        switch (bDrvGetScartInputMode())
        {
        case SV_SCART_RGB:
            return SOURCE_TYPE_TIMING_SCART_RGB;
        case SV_SCART_SV:
            switch ((UINT8)bHwTvdMode())  // penggang add uint8
            {
            case (UINT8)SV_CS_SECAM:
                return SOURCE_TYPE_TIMING_SCART_SV_SECAM;
            case (UINT8)SV_CS_NTSC358:
                return SOURCE_TYPE_TIMING_SCART_SV_NTSC_358;
            case (UINT8)SV_CS_NTSC443:
                return SOURCE_TYPE_TIMING_SCART_SV_NTSC_443;
            case (UINT8)SV_CS_PAL_M:
                return SOURCE_TYPE_TIMING_SCART_SV_PAL_M;
            case (UINT8)SV_CS_PAL_N:
                return SOURCE_TYPE_TIMING_SCART_SV_PAL_N;
            case (UINT8)SV_CS_PAL_60:
                return SOURCE_TYPE_TIMING_SCART_SV_PAL_60;
            case (UINT8)SV_CS_PAL:
                return SOURCE_TYPE_TIMING_SCART_SV_PAL;
            default:
                return SOURCE_TYPE_TIMING_MAX;
            }
        default: //AUTO, MIX, AV
        case SV_SCART_CVBS:
            switch ((UINT8)bHwTvdMode())// penggang add uint8
            {
            case (UINT8)SV_CS_SECAM:
                return SOURCE_TYPE_TIMING_SCART_AV_SECAM;
            case (UINT8)SV_CS_NTSC358:
                return SOURCE_TYPE_TIMING_SCART_AV_NTSC_358;
            case (UINT8)SV_CS_NTSC443:
                return SOURCE_TYPE_TIMING_SCART_AV_NTSC_443;
            case (UINT8)SV_CS_PAL_M:
                 return SOURCE_TYPE_TIMING_SCART_AV_PAL_M;
            case (UINT8)SV_CS_PAL_N:
                return SOURCE_TYPE_TIMING_SCART_AV_PAL_N;
            case (UINT8)SV_CS_PAL_60:
                return SOURCE_TYPE_TIMING_SCART_AV_PAL_60;
            case (UINT8)SV_CS_PAL:
                return SOURCE_TYPE_TIMING_SCART_AV_PAL;
            default:
                return SOURCE_TYPE_TIMING_MAX;
            }
        }
    }
#endif


    switch (bGetSignalType(bPath))
    {
    case SV_ST_VGA:
        return (bIsScalerInput444(bPath)?
            SOURCE_TYPE_TIMING_PC_RGB : SOURCE_TYPE_TIMING_PC_YCBCR);
    case SV_ST_TV:
        if (!fgHwTvdVPres())
        {
            return SOURCE_TYPE_TIMING_MAX;
        }
        switch ((UINT8)bHwTvdMode())
        {
        case (UINT8)SV_CS_SECAM:
            return SOURCE_TYPE_TIMING_SECAM_RFTV;
        case (UINT8)SV_CS_NTSC358:
            return SOURCE_TYPE_TIMING_RFTV_NTSC_358;
        case (UINT8)SV_CS_NTSC443:
            return SOURCE_TYPE_TIMING_RFTV_NTSC_443;
        case (UINT8)SV_CS_PAL_M:
            return SOURCE_TYPE_TIMING_RFTV_PAL_M;
        case (UINT8)SV_CS_PAL_N:
            return SOURCE_TYPE_TIMING_RFTV_PAL_N;
        case (UINT8)SV_CS_PAL_60:
            return SOURCE_TYPE_TIMING_RFTV_PAL_60;
        case (UINT8)SV_CS_PAL:
            return SOURCE_TYPE_TIMING_RFTV_PAL;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case SV_ST_SV:
        if (!((UINT8)fgHwTvdVPres()))
        {
            return SOURCE_TYPE_TIMING_MAX;
        }
        switch ((UINT8)bHwTvdMode())
        {
        case (UINT8)SV_CS_SECAM:
            return SOURCE_TYPE_TIMING_SECAM_SV;
        case (UINT8)SV_CS_NTSC358:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_SV_NTSC_358_OS : SOURCE_TYPE_TIMING_SV_NTSC_358);
        case (UINT8)SV_CS_NTSC443:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_SV_NTSC_443_OS : SOURCE_TYPE_TIMING_SV_NTSC_443);
        case (UINT8)SV_CS_PAL_M:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_SV_PAL_M_OS : SOURCE_TYPE_TIMING_SV_PAL_M);
        case (UINT8)SV_CS_PAL_N:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_SV_PAL_N_OS : SOURCE_TYPE_TIMING_SV_PAL_N);
        case (UINT8)SV_CS_PAL_60:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_SV_PAL_60_OS : SOURCE_TYPE_TIMING_SV_PAL_60);
        case (UINT8)SV_CS_PAL:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_SV_PAL_OS : SOURCE_TYPE_TIMING_SV_PAL);
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case SV_ST_AV:
        if (!((UINT8)fgHwTvdVPres()))
        {
            return SOURCE_TYPE_TIMING_MAX;
        }
        switch ((UINT8)bHwTvdMode())
        {
        case (UINT8)SV_CS_SECAM:
            return SOURCE_TYPE_TIMING_SECAM_CVBS;
        case (UINT8)SV_CS_NTSC358:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_CVBS_NTSC_358_OS : SOURCE_TYPE_TIMING_CVBS_NTSC_358);
        case (UINT8)SV_CS_NTSC443:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_CVBS_NTSC_443_OS : SOURCE_TYPE_TIMING_CVBS_NTSC_443);
        case (UINT8)SV_CS_PAL_M:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_CVBS_PAL_M_OS : SOURCE_TYPE_TIMING_CVBS_PAL_M);
        case (UINT8)SV_CS_PAL_N:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_CVBS_PAL_N_OS : SOURCE_TYPE_TIMING_CVBS_PAL_N);
        case (UINT8)SV_CS_PAL_60:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_CVBS_PAL_60_OS : SOURCE_TYPE_TIMING_CVBS_PAL_60);
        case (UINT8)SV_CS_PAL:
            return (vTvdGetOverSampleEnable()?
                SOURCE_TYPE_TIMING_CVBS_PAL_OS : SOURCE_TYPE_TIMING_CVBS_PAL);
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    default:
        break;
    }

    switch (bDrvVideoGetTiming(bPath))
    {
    case MODE_1080i:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_1080I_60;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_1080IP;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_1080I_60;
            }
        case (UINT8)SV_ST_YP:
            return SOURCE_TYPE_TIMING_YPBPR_1080I_60;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_1080i_48:
    case MODE_1080i_50:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_1080I_50;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_1080IP;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_1080I_50;
            }
        case (UINT8)SV_ST_YP:
            return SOURCE_TYPE_TIMING_YPBPR_1080I_50;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_1080p_25:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_1080P_25;
        case (UINT8)SV_ST_MPEG:
 			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_1080IP;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_1080P_25;
            }
        case (UINT8)SV_ST_YP:
            return SOURCE_TYPE_TIMING_YPBPR_1080P_25;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_1080p_50:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_1080P_50;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_1080IP;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_1080P_50;
            }
        case (UINT8)SV_ST_YP:
            return SOURCE_TYPE_TIMING_YPBPR_1080P_50;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_1080p_24:
    case MODE_1080p_30:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_1080P_24;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_1080IP;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_1080P_24;
            }
        case (UINT8)SV_ST_YP:
            return SOURCE_TYPE_TIMING_YPBPR_1080P_24;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_1080p_60:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_1080P_60;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_1080IP;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_1080P_60;
            }
        case (UINT8)SV_ST_YP:
            return SOURCE_TYPE_TIMING_YPBPR_1080P_60;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_720p_25:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_720P_25;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_720P;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_720P_25;
            }
        case (UINT8)SV_ST_YP:
            return SOURCE_TYPE_TIMING_YPBPR_720P_25;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_720p_50:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_720P_50;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_720P;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_720P_50;
            }
        case (UINT8)SV_ST_YP:
            return SOURCE_TYPE_TIMING_YPBPR_720P_50;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_720p_24:
    case MODE_720p_30:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_720P_24;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_720P;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_720P_24;
            }
        case (UINT8)SV_ST_YP:
            return SOURCE_TYPE_TIMING_YPBPR_720P_24;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_720p_60:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_720P_60;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_720P;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_720P_60;
            }
        case (UINT8)SV_ST_YP:
            return SOURCE_TYPE_TIMING_YPBPR_720P_60;
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_625I:
    case MODE_625I_OVERSAMPLE:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_SD_I_50;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_SD_I;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_SD_I_50;
            }
        case (UINT8)SV_ST_YP:
            return (vHdtvGetOversampleForSD()?
                SOURCE_TYPE_TIMING_YPBPR_SD_I_50_OS : SOURCE_TYPE_TIMING_YPBPR_SD_I_50);
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_525I:
    case MODE_525I_OVERSAMPLE:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_SD_I_60;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_SD_I;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_SD_I_60;
            }
        case (UINT8)SV_ST_YP:
            return (vHdtvGetOversampleForSD()?
                SOURCE_TYPE_TIMING_YPBPR_SD_I_60_OS : SOURCE_TYPE_TIMING_YPBPR_SD_I_60);
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_576P_25:
    case MODE_576P:
    case MODE_576P_OVERSAMPLE:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_SD_P_50;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_SD_P;
            }
            else
 			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_SD_P_50;
            }
        case (UINT8)SV_ST_YP:
            return (vHdtvGetOversampleForSD()?
                SOURCE_TYPE_TIMING_YPBPR_SD_P_50_OS : SOURCE_TYPE_TIMING_YPBPR_SD_P_50);
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_480P:
    case MODE_480P_30:
    case MODE_480P_24:
    case MODE_480P_OVERSAMPLE:
	case MODE_HDMI_640_480P:
        switch (bGetSignalType(bPath))
        {
        case (UINT8)SV_ST_DVI:
            return SOURCE_TYPE_TIMING_DIGI_SD_P_60;
        case (UINT8)SV_ST_MPEG:
			#ifndef CC_UP8032_ATV
            if(VDP_GetPlayMode(bPath) == FBM_FBG_MM_MODE)
            {
                return SOURCE_TYPE_TIMING_MM_SD_P;
            }
            else
			#endif
            {
                return SOURCE_TYPE_TIMING_DTV_SD_P_60;
            }
        case (UINT8)SV_ST_YP:
            return (vHdtvGetOversampleForSD()?
                SOURCE_TYPE_TIMING_YPBPR_SD_P_60_OS : SOURCE_TYPE_TIMING_YPBPR_SD_P_60);
        default:
            return SOURCE_TYPE_TIMING_MAX;
        }
    case MODE_DE_MODE:
        return SOURCE_TYPE_TIMING_PC_RGB;
    default:
        assert(0);
        return SOURCE_TYPE_TIMING_MAX;
    }
#if !SUPPORT_POP
#undef bPath
#endif
}


/* new API to get/set eaten porch decoder+nr+di+scpos
    accrording to current video timing.
    20070612 sv_porch_t*/
UINT16 wDrvVideoPorchStd(UINT8 bPath, UINT8 bPorchType)
{
    switch(bPorchType)
    {
        case SV_HPORCH_MAX:
            return  (wDrvVideoGetHTotal(bPath)-wDrvVideoInputWidth(bPath));
        case SV_VPORCH_MAX:
            return  ((wDrvVideoGetVTotal(bPath)-wDrvVideoInputHeight(bPath))*4)/3;//*1.33
        case SV_VPORCH_MIN:
            return  2;
        case SV_HPORCH_MIN:
        default:
            return 0;
    }
}

UINT8 bDrvPorchTune[2]={
    SV_PORCHTUNE_DEC,
    SV_PORCHTUNE_DEC
    };

UINT16 wDrvVideoGetPorch(UINT8 bPath, UINT8 bPorchType)
{
    UINT8 bDecType;
    UINT16  wDecPorch=0,wScposPorch=0;

    /* implemention try to make DI/NR don't eat porch,scops eat porch
       for overscan or lbox*/
    if(bDrvPorchTune[bPath]&SV_PORCHTUNE_DEC)
    {
        bDecType = bGetVideoDecType(bPath);
        switch (bDecType)
        {
            case SV_VD_TVD3D:
                wDecPorch=wTvdGetPorch(bPath,bPorchType);
                break;
            case SV_VD_VGA:
                wDecPorch=wVgaGetPorch(bPath,bPorchType);
                break;
            case SV_VD_YPBPR:
                wDecPorch=wHdtvGetPorch(bPath,bPorchType);
                break;
            case SV_VD_DVI:
                wDecPorch=wDviGetPorch(bPath,bPorchType);
                break;
            #ifndef COPLAT_EXCLUDE_DTV
            case SV_VD_MPEGHD:
            case SV_VD_MPEGSD:
            #endif
            default:
                wDecPorch=0;    //rely on scpos's porch
                break;
        }
    }

    if(bDrvPorchTune[bPath]&SV_PORCHTUNE_SCPOS)
    {
        wScposPorch=_VDP_ScposGetPorch(bPath,bPorchType);
    }
    return wDecPorch+wScposPorch;

}
UINT16 wDrvVideoSetPorch(UINT8 bPath, UINT8 bPorchType, UINT16 wValue)
{
    UINT8 bDecType;
    UINT16  wDecPorch=0,wScposPorch=0,min,max;
    INT32 tmp;
    //INT32 i4VBIadj; /* VBI LSC adjustion */


    if((bPorchType!=SV_HPORCH_CURRENT)&&(bPorchType!=SV_VPORCH_CURRENT))
            return 0;


    /* only tune porch to decoder, SV_HPORCH_CURRENT|SV_HPORCH_CURRENT */

    if(bDrvPorchTune[bPath]==SV_PORCHTUNE_MIX)
    {
        wScposPorch=_VDP_ScposGetPorch(bPath,  bPorchType);
    }
    bDecType = bGetVideoDecType(bPath);
    tmp=wValue-wScposPorch;
    if(tmp<0)  tmp=0;

    if(bDrvPorchTune[bPath]&SV_PORCHTUNE_DEC)
    {
        switch (bDecType)
        {
            case SV_VD_TVD3D:
                min=wTvdGetPorch(bPath,bPorchType+2);
                max=wTvdGetPorch(bPath,bPorchType+3);
                if (tmp<min) tmp=min;
                if (tmp>max) tmp=max;
                vTvdSetPorch(bPorchType,tmp);
                wDecPorch=wTvdGetPorch(bPath,bPorchType);
                vTvd3dGetUIDelay();
                break;
            case SV_VD_VGA:
                min=wVgaGetPorch(bPath,bPorchType+2);
                max=wVgaGetPorch(bPath,bPorchType+3);
                if (tmp<min) tmp=min;
                if (tmp>max) tmp=max;
                vVgaSetPorch(bPath,bPorchType,tmp);
                wDecPorch=wVgaGetPorch(bPath,bPorchType);
                break;
            case SV_VD_YPBPR:
                min=wHdtvGetPorch(bPath,bPorchType+2);
                max=wHdtvGetPorch(bPath,bPorchType+3);
                if (tmp<min) tmp=min;
                if (tmp>max) tmp=max;
                vHdtvSetPorch(bPath,bPorchType,tmp);
                wDecPorch=wHdtvGetPorch(bPath,bPorchType);
                break;
            case SV_VD_DVI:
                min=wDviGetPorch(bPath,bPorchType+2);
                max=wDviGetPorch(bPath,bPorchType+3);
                if (tmp<min) tmp=min;
                if (tmp>max) tmp=max;
                vDviSetPorch(bPath,bPorchType,tmp);
                wDecPorch=wDviGetPorch(bPath,bPorchType);
                break;
            #ifndef COPLAT_EXCLUDE_DTV
            case SV_VD_MPEGHD:
            case SV_VD_MPEGSD:
            #endif
            default:
                wDecPorch=0;
                break;
        }

        if(bDrvPorchTune[bPath]==SV_PORCHTUNE_DEC)
        {
            return wDecPorch;
        }

        #if NR_ADAPTIVE_DEBA_ENABLED
            vDrvAdaptiveDebaSetState(NR_ADAPTIVE_DEBA_STATE_RESET);
        #endif
    }

    /*  SV_PORCHTUNE_MIX  or SV_PORCHTUNE_SCPOS */
    tmp=wValue-wDecPorch;

    /* scpos's min/max constrain is controlled inside _VDP_ScposSetPorch */
    _VDP_ScposSetPorch(bPath,bPorchType,tmp);
    wScposPorch=_VDP_ScposGetPorch(bPath,bPorchType);

    return wDecPorch+wScposPorch;
}


UINT8 bDrvVideoGetWidthOverSample(UINT8 bPath)
{
    UINT8 bType;

#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
        case SV_VD_YPBPR:
        case SV_VD_VGA:
            return (bHdtvInputWidthOverSample(bDrvVideoGetTiming(bPath)));
        case SV_VD_DVI:

                return 0;

    	case SV_VD_TVD3D:
             return (vTvdGetOverSampleEnable());

        default:
            return 0;
    }
}

#if MANUAL_MAINSUB_FIFO_RST
/**
 * @brief Get the video path frame dealy time (DI, Scaler)
 * @param Path: 1: PIP 0: Main
 * @return : none
 */
void vDrvMainSubFifoReset(UINT8 bPath)
{
    #if !defined(CC_MT8223)
    if (IS_IC_5363()&& (!(BSP_GetIcVersion() < IC_VER_5363_AB)))// 5363 ECO IC or newer IC in future.
    #endif
    {
        if(bPath == 1)
        {
            #if !defined(CC_MT8223)
            vPipFIFOResetOn();
            vUtDelay2us(1);
            vPipFIFOResetOff();
            #endif
        }
        else
        {
            vMainFIFOResetOn();
            vUtDelay2us(1);
            vMainFIFOResetOff();
        }
    }
}
#endif


/**
 * @brief Get the video path frame dealy time (DI, Scaler)
 * @param u1Path
 * @return delay time in mini-sencond.
 */
UINT16 bDrvVideoGetFrameDelay(UINT8 bPath)
{
	UINT8 bFrameDelay =0;
	UINT8 bScalerFrame;
	UINT8 bPanelFrameDelay =0 ;

	UINT8 bFrameRate;;

	if(bPath == SV_VP_MAIN)
	{
		bFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);

		if(bFrameRate ==0) // prevent divide by 0 case
		{
			return 0;
		}
		bFrameDelay += vDrvDIGetFrameDelay(bPath);// DI frame delay
		//bFrameDelay += bDrvMJCGetFrameDelay();// MJC frame delay
		//bScalerFrame = bGetScalerFrameNum(bPath);
		bScalerFrame = u4ScpipSRMGetFrameDelay(bPath);
#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
             bPanelFrameDelay =  PANEL_GetFrameDelay();
#endif

		//return ((bFrameDelay + bScalerFrame/2 + bPanelFrameDelay/10) *1000 / bFrameRate);
		return (((UINT32)(bFrameDelay*10) + bScalerFrame*5 + bPanelFrameDelay) * 100 / bFrameRate);
	}
	else
		return 0;
}

#ifdef CC_AUD_QUERY_VDO_INFO
/**
 * Usage: include vdo_if.h and video_timing.h
 */
void DrvVideoGetInputTimingInfo(UINT8 bPath, VDO_INPUT_INFO* info)
{
    info->u2Width = wDrvVideoInputWidth(bPath);
    info->u2Height = wDrvVideoInputHeight(bPath);
    info->u1Timing = bDrvVideoGetTiming(bPath);
    info->u1Interlace = bDrvVideoIsSrcInterlace(bPath);
    info->u1RefreshRate = bDrvVideoGetRefreshRate(bPath);
    #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    info->u1PanelDelay = PANEL_GetFrameDelay();
    #endif
}
#endif

#if 0
/**
 * @brief Redo two chip (5382p + 8280) connection
 *
 * @param fgStart
 * @param u1Path
 * @param u1FrameRate
 * @param u1LvdsMode
 */

UINT8 bGetICVersion(void)
{
#ifdef DRV_SUPPORT_EXTMJC

    UINT8 ui1_data;
    UINT8 u1ICVersion, u1ICRead;

    u1ICVersion = MT8280;
    u1ICRead =SIF_ReadMultipleSubAddr(0x100, 0xc, 2, 0x60, &ui1_data, 1);

    if (ui1_data== 0x0001)
    {
        u1ICVersion = MT8280_E1;
    }

    UNUSED(u1ICRead);
    return u1ICVersion;
#else
    return 0;
#endif
}
#endif
#if 0
typedef struct _REDO_TWO_CHIP_CONN_INFO_T
{
    UINT8 u1Path;
    #ifndef CC_UP8032_ATV
    BOOL fgEnable;
    #else
    UINT8 fgEnable;
    #endif
    UINT8 u1ColorDomain;
    UINT8 u1RefreshRate;
    UINT32 u4HTotal;
    UINT32 u4VTotal;
} REDO_TWO_CHIP_CONN_INFO_T;

void vApiRedoTwoChipConnection(UINT8 u1Path,BOOL fgEnable)
{
#ifdef DRV_SUPPORT_EXTMJC

    if (!IS_COMPANION_CHIP_ON())
    {
        return;
    }

    INT32 i4Ret;
    UINT8 u1MsgErr;
    #ifndef CC_MT8281_SLT
    static SRM_TV_MODE_T _ePoPMode;
    #endif
    static REDO_TWO_CHIP_CONN_INFO_T t_new_conn_info={ SV_VP_MAIN, SV_TRUE, 0xff , 0xff ,0, 0};
    static UINT8 _u1IsMJCOn = 0xff;

    // 5382p + 8280
    if ((FALSE == fgEnable) && (t_new_conn_info.fgEnable == SV_TRUE))
    {
        //drv_extmjc_set_lvds_enable(SV_FALSE, &u1MsgErr);
        //LOG(2,"drv_extmjc_set_lvds_enable: 0\n");
        // Change 538x and 8280 OSD to normal mode
        #ifndef DRV_SUPPORT_EXTMJC_MODE0
        i4Ret =d_custom_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_NORMAL, &u1MsgErr);
        #endif //DRV_SUPPORT_EXTMJC_MODE0
        t_new_conn_info.u1Path          = u1Path;
        t_new_conn_info.fgEnable        = fgEnable;
        LOG(2,"[vApiRedoTwoChipConnection]: OSD normal mode\n");
    }
    else if (TRUE == fgEnable)
    {
        UINT32  u4MjcVTotal;
        UINT32  u4MjcHTotal;
        UINT8   u1MjcRefreshRate;
  //      UINT8   u1ColorDomain;
        UINT8   u1LvdsMode;
        UINT8   u1PreLvdsMode;
        UINT8 u1IsMJCOn;
        UINT8 u1ModeChanged;

        u1MjcRefreshRate = bOUTPUT_REFRESH_RATE;//bDrvVideoGetRefreshRate(SV_VP_MAIN);
        u1PreLvdsMode = vDrvGetCurLvdsMode();
        #ifndef CC_MT8281_SLT
        _ePoPMode = SRM_GetTvMode();

        if((_ePoPMode == SRM_TV_MODE_TYPE_NORMAL) && (bIsScalerInput444(SV_VP_MAIN)==SV_FALSE))
        {
            #ifdef DRV_SUPPORT_EXTMJC_MODE0
            u1IsMJCOn = 1;
            u1LvdsMode = eLvdsVidNormal;
            #else
            #ifdef DRV_SUPPORT_EXTMJC_MODE3
            u1IsMJCOn = 1;
            u1LvdsMode = eLvdsVidYuv422TwoPort;
            #else
            u1IsMJCOn = 1;
            u1LvdsMode = eLvdsVidRgb444OnePort;
            #endif //DRV_SUPPORT_EXTMJC_MODE3
            #endif //DRV_SUPPORT_EXTMJC_MODE0
        }
        else
        #endif
        {
            #ifdef DRV_SUPPORT_EXTMJC_MODE0
            u1IsMJCOn = 0;
            u1LvdsMode = eLvdsVidNormal;
            #else
            #ifdef DRV_SUPPORT_EXTMJC_MODE3
            u1IsMJCOn = 0;
            u1LvdsMode = eLvdsVidRgb444TwoPort;
            #else
            u1IsMJCOn = 0;
            u1LvdsMode = eLvdsVidRgb444OnePort;
            #endif //DRV_SUPPORT_EXTMJC_MODE3
            #endif //DRV_SUPPORT_EXTMJC_MODE0
        }

        u4MjcHTotal = wDrvGetOutputHTotal() -1;
        u4MjcVTotal = wDrvGetOutputVTotal() -1;

         if ((u4MjcHTotal !=t_new_conn_info.u4HTotal)   ||
            (u4MjcVTotal !=t_new_conn_info.u4VTotal)   ||
            (t_new_conn_info.u1RefreshRate != u1MjcRefreshRate) ||
            (_u1IsMJCOn  != u1IsMJCOn) ||
            (u1PreLvdsMode != u1LvdsMode)
            )
        {
            u1ModeChanged = 1;
        }
        else
        {
            u1ModeChanged = 0;
        }

        if (u1ModeChanged)
        {
            _u48280Unmte = 15;
            //LOG(2,"drv_extmjc_set_lvds_enable(PIP): mode changd done : 0\n");
            //drv_extmjc_set_lvds_enable(SV_FALSE, &u1MsgErr);
            d_custom_extmjc_set_fastmute(TRUE, &u1MsgErr);
        }

        vDrvSetCurLvdsMode(u1LvdsMode);

        if (u1LvdsMode == eLvdsVidYuv422TwoPort)
        {
            vDrvSetUniColorDomain(eColorDomainYUV);
            vDrvSetLvdsColorDomain(eColorDomainYUV);
        }
        else
        {
            vDrvSetUniColorDomain(eColorDomainRGB);
            vDrvSetLvdsColorDomain(eColorDomainRGB);
        }

        if (u1ModeChanged)
        {
            // Change 8280 LVDS mode
            i4Ret = drv_extmjc_set_video_fmt(u1LvdsMode, u1MjcRefreshRate, (UINT16) u4MjcHTotal, (UINT16) u4MjcVTotal, u1IsMJCOn, &u1MsgErr);
            LOG(0, "Reset5382 LVDS: ( Enable(%d), Path(%d), u1LvdsMode(%d), "
            "rate(%d), H(%d), V(%d), MJC On(%d) )\r\n",
            (UINT8)fgEnable, (UINT8)u1Path, (UINT8)u1LvdsMode,
            (UINT32)u1MjcRefreshRate, (UINT32)u4MjcHTotal, (UINT32)u4MjcVTotal, (UINT32)u1IsMJCOn );

//            t_new_conn_info.u1ColorDomain     = u1ColorDomain;
            t_new_conn_info.u1RefreshRate   = u1MjcRefreshRate;
            t_new_conn_info.u4HTotal        = u4MjcHTotal;
            t_new_conn_info.u4VTotal        = u4MjcVTotal;
            _u1IsMJCOn = u1IsMJCOn;
        }

        if(t_new_conn_info.fgEnable==SV_FALSE)
        {
            #ifndef DRV_SUPPORT_EXTMJC_MODE0
            // Change 538x and 8280 OSD to DMA mode
            i4Ret = d_custom_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_DMA, &u1MsgErr);
            LOG(2,"[vApiRedoTwoChipConnection]: OSD DMA mode\n");
            #endif //DRV_SUPPORT_EXTMJC_MODE0
            t_new_conn_info.u1Path          = u1Path;
            t_new_conn_info.fgEnable        = fgEnable;
        }
    }

    UNUSED(u1MsgErr);
    UNUSED(i4Ret);
#endif

}
#endif
#if 0
/**
 * Determine the video clock source of main/sub path.
 */
void vVDOIN_MPCLK_Setting(void)
{
	UINT32 u4PixelClk;
	u4PixelClk = wDrvVideoGetHTotal(SV_VP_MAIN)*wDrvVideoGetVTotal(SV_VP_MAIN)*bDrvVideoGetRefreshRate(SV_VP_MAIN);
	Printf("pixel clk%d\n", u4PixelClk);
        //todo
	if((u4PixelClk > 160000000) && (bDrvVideoSignalStatus(SV_VP_MAIN) == SV_VDO_STABLE)) //162MHz
	{
		SET_VDOIN_MPCLK(MPCLK_MAINCLK);
	}
	else
	{
		SET_VDOIN_MPCLK(MPCLK_162MHZ);
	}
}
#endif
#ifndef CC_UP8032_ATV
void vDrvLoadRegDwrdTbl(RRegDwrdTbl const *pRegDwrdTbl) reentrant
{
    if (pRegDwrdTbl == NULL)
    {
        return;
    }

    while (pRegDwrdTbl->wReg != REG_END)
    {
        if (pRegDwrdTbl->dwMask == 0xFFFFFFFF)
        {
            vRegWrite4B(pRegDwrdTbl->wReg, pRegDwrdTbl->dwValue);
        }
        else
        {
            vRegWrite4BMsk(pRegDwrdTbl->wReg, pRegDwrdTbl->dwValue,
                           pRegDwrdTbl->dwMask);
        }
        pRegDwrdTbl++;
    }
}
#endif
UINT32 u4DrvVideoGetNSDOffset(UINT8 bPath)
{
    UINT8 bType;
    UINT32 u4Ret = (UINT32)0x800;

#if SUPPORT_POP
    bType = bGetVideoDecType(bPath);
#else
    bType = bGetVideoDecType(SV_VP_MAIN);
#endif

    switch (bType)
    {
        case SV_VD_TVD3D:
        {
            RTvdNSTDStatus rStatus;
            vTvd3dGetNSTDStatus(&rStatus);
            if (rStatus.fgIsNSTD)
            {
                u4Ret = (UINT32)rStatus.wVTotalDiff;
            }
            break;
        }
        case SV_VD_YPBPR:
            {
                RHDTVNSTDStatus rHDTVStatus;
                vHDTVGetNSTDStatus(&rHDTVStatus);
                if (rHDTVStatus.fgIsNSTD)
                {
                    u4Ret = (UINT32)rHDTVStatus.wVTotalDiff;
                }
                break;
            }

        #if SUPPORT_DVI
        case SV_VD_DVI:
        #endif
        #ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
        case SV_VD_MPEGSD:
        #endif
        default:
            break;
    }

    return u4Ret;
}
#ifdef CC_SCPOS_PATTERN_GENERATOR
UINT8 u1ScposPtGen = 0;
void vSetScposPatternGen(UINT8 u1Enable)
{
	// be sure to enable this before change to VGA source
	u1ScposPtGen = u1Enable;
    vSetMainFlg(MAIN_FLG_MODE_CHG);
    vSetMainFlg(MAIN_FLG_MODE_DET_DONE);

}
UINT8 u1GetScposPtGenEnable(void)
{
	return u1ScposPtGen;	
}
#endif
