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
 * $RCSfile: drv_video.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_VIDEO_H_
#define _DRV_VIDEO_H_

#include "typedef.h"
#include "drv_ycproc.h"
// drv_adcset.h
extern void vDrvADCOffsetCal(void) ;

extern void vDrvSetAVMux(UINT8 vsrc) ;
extern void vDrvSetHDTVMux(UINT8 bVDOINPUT) ;
extern void vDrvSetTVADC(UINT8 bVDOINPUT) ;
extern void vDrvSetHDTVADC(UINT8 bVDOINPUT) ;
extern void vDrvAllHDADCPow(UINT8 bPow) ;

extern void vDrv10BitAdcPD(void) ;

extern void vDrvSVdoDisable(void) ;
extern void vDrvSVdoEnable(void) ;

//source_select.h
extern UINT8 bGetInternalDec(UINT8 bPath) ;


// drv_tvd.h
extern void vDrvTDCOnOff(UINT8 bOnOff) ;
extern void vDrvSetTDCGameMode(UINT8 bEnable);
extern void vDrvColorSystem(UINT8 bTVD, UINT8 bColSys) ;
extern void vTvd3dReset(void) ;
extern UINT8 bTvdLumaSignalRefreshRate(void) ;
extern void vDrvWhitePeakInit(void) ;
extern void vDrvTVDAutoCalibStart(void);

#define RW_STA_TDCNRVCR         0xc5	// TDC High Noise, VCR Still Frame flags
#define TDC3D_NONVCR          0x80

extern UINT8 _bSetChannelCnt; //Counter for tuner, set 60 when change channel number rock 20060324


#define fgIsTvd3dVCR()    ((bReadVDOIN(RW_STA_TDCNRVCR, 1) & TDC3D_NONVCR) == 0)

extern void vDrvVideoSetDispTop(UINT8 bPath) ;
extern void vDrvVideoSetWriteEnable(UINT8 bPath, UINT8 bOnOff) ;
extern void vSwitchDispMode(UINT32 u4OnOff) ;
//extern void vDrvVideoSetDispMask(UINT32 bPath, UINT32 bOnOff) ;


// mloop_video.c
extern UINT32 _u4BypassNptvMloop ;
#define BYPASS_NPTV_All	            (1 << 0)
#define BYPASS_NPTV_NR	            (1 << 1)
#define BYPASS_NPTV_HDMI	        (1 << 2)
#define BYPASS_NPTV_SCPOS	        (1 << 3)
#define BYPASS_NPTV_PSCAN	        (1 << 4)
#define BYPASS_NPTV_TVD	            (1 << 5)
#define BYPASS_NPTV_DDDS	        (1 << 6)
#define BYPASS_NPTV_CEC	            (1 << 7)
#define BYPASS_NPTV_TVE	            (1 << 8)
#define BYPASS_NPTV_SCART	        (1 << 9)
#define BYPASS_OUTPUT_ISR           (1 << 10)
#define BYPASS_INPUT_ISR            (1 << 11)
#define BYPASS_COLOR_PROC           (1 << 12)
#define BYPASS_NSD_PROC             (1 << 13)
#define BYPASS_NPTV_MJC             (1 << 14)
#define BYPASS_NPTV_TDTV            (1 << 15)
#define BYPASS_VDO_MAIN_STATE       (1 << 16)
#define BYPASS_VDO_PIP_STATE        (1 << 17)
#define BYPASS_SCAN_CHK             (1 << 18)
#define BYPASS_NPTV_YCPROC	        (1 << 19)
#define BYPASS_NPTV_FRAME_TRACK	    (1 << 20)
#define BYPASS_NPTV_LCM	            (1 << 21)
#define BYPASS_SRC_DETECT           (1 << 22)
#define BYPASS_NPTV_OD              (1 << 23)

#define fgIsBypassFlgSet(fgBypass)  (_u4BypassNptvMloop & fgBypass)
#define fgIsBypassFlgClr(fgBypass)  ((_u4BypassNptvMloop & fgBypass) == 0)

// drv_scpos.h

#define CC_MUTE_MASK_FORCE          (1 << 0)
#define CC_MUTE_MASK_BLUE           (1 << 1)
#define CC_MUTE_MASK_BLACK          (1 << 2)
#define CC_MUTE_MASK_MDDI01         (1 << 3)
#define CC_MUTE_MASK_SCPOS01        (1 << 4)
#define CC_MUTE_MASK_SCPOS02        (1 << 5)
#define CC_MUTE_MASK_SCPOS03        (1 << 6)
#define CC_MUTE_MASK_SCPOS04        (1 << 7)
#define CC_MUTE_MASK_MODECHG		(1 << 8)
#define CC_MUTE_MASK_SRM		    (1 << 9)
#define CC_MUTE_MASK_DTV            (1 << 10)
#define CC_MUTE_MASK_HDMI            (1 << 11)
#ifdef DRAM_TEST_DSP_CODE
#define CC_MUTE_MEMORY_TEST         (1 << 13)
#endif
#define CC_MUTE_MASK_PPCLOCK    (1 << 14)
#define CC_MUTE_MASK_NR             (1 << 15)

extern void vScposSetMuteColor(UINT8 bPath, UINT32 u4Bg) reentrant;
extern void vDrvSetRGain(UINT8 bPath, UINT16 wValue);
extern void vDrvSetGGain(UINT8 bPath, UINT16 wValue);
extern void vDrvSetBGain(UINT8 bPath, UINT16 wValue);
extern void vDrvSetROffset(UINT8 bPath, UINT16 wValue);
extern void vDrvSetGOffset(UINT8 bPath, UINT16 wValue);
extern void vDrvSetBOffset(UINT8 bPath, UINT16 wValue);

// drv_ycproc.h
extern void vDrvGetb4ScalerRec(void);
extern void vDrvSetQFactor(UINT8 bFactor) ;
extern void vDrvSetContrast(UINT8 bPath, UINT16 u2Value) ;
extern void vDrvSetBrightness(UINT8 bPath, UINT16 wValue) ;
extern void vDrvSetSaturation(UINT8 bPath, UINT16 u2Value) ;
extern void vDrvSetHue(UINT8 bPath, UINT16 wValue) ;
extern void vDrvWhitePeakLimit(UINT8 bOnOff) ;
extern void vDrvMlcReset(void) ;
extern void vDrvPlcReset(void) ;

extern void vDrvColorTransform(UINT8 bPath) ;
extern void vDrvMain422(UINT8 bOnOff) ;
extern void vDrvPip422(UINT8 bOnOff) ;
extern void vDrvSetUniColorDomain(UINT8 bColorDomain, UINT8 bLvdsMode);
extern void vDrvColorDemo(UINT8 bOnOff, Region rRegion);
extern void vDrv2DSharpDemo(UINT8 bOnOff, Region rRegion);
extern void vDrvECTIDemo(UINT8 bOnOff, Region rRegion);
extern void vDemoSetRegion(Region rDemoRegion);
extern void vDrvMain10BitColorOnOff(UINT8 bOnOff) ;
extern void vDrvPip10BitColorOnOff(UINT8 bOnOff) ;

extern UINT8   _bMAIN_COLOR_SYS;
extern UINT8   _bPIP_COLOR_SYS;

// drv_di.h
extern void vDrvDIDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter) ;
extern void vDrvDISetFRCParam(UINT8 bOnOff) ;

// drv_display.h
extern void vDrvDisplayInit(void) ;
extern UINT8 bDrvDispSupportFlag(UINT16 DV_TOTAL) ;
extern void vDrvSetDynamicDisplay(UINT8 BiMode) ;
extern void vDrvLCDFreqSet(UINT8 bDCLKType, UINT8 u1FrameRate) ;
extern void vDrvTubeFreqSet(UINT8 bDCLKType) ;
extern void vDrvTube169Set(UINT8 bDCLKType) ;
extern void vDrvSetMTKGoodDclk(UINT16 wVERT_LINE, UINT8 u1FrameRate) ;
extern void vDrvSwitchMTKGoodDclk(UINT8 bOn_Off) ;
extern UINT8 bDrvCheckMTKGoodStatus(void) ;
extern UINT16 wDrvGetOutputHTotal(void) ;
extern UINT16 wDrvGetOutputVTotal(void) ;

//extern UINT16   wDISPLAY_WIDTH;
//extern UINT16   wDISPLAY_HEIGHT;
extern UINT8   bDISPLAY_INTERLACE_ENABLE;
extern UINT8   bOUTPUT_REFRESH_RATE;
extern UINT8   bOutput_Timing_No;
extern UINT32   dwSI_DISPLAY_DCLK_TYPICAL;

// drv_lvds.h
extern void vDrvLVDSOn(void) ;
extern void vDrvLVDSOff(void) ;
extern void vDrvLVDSConfig(void) ;

// drv_contrast.h
extern void vDrvADLInit(void) ;
extern void vDrvADLOnOff(UINT8 bOnOff);
extern void vDrvADLProc(void);

// vdo_misc.h
extern void vDrvVideoSwInit(void) ;
extern void vDrvVideoHwInit(void) ;

extern UINT8 bDrvVideoGetType(UINT8 bPath) ;
#define VDOTYPE_NTSC 0
#define VDOTYPE_PAL 1
#define VDOTYPE_OTHER 2

extern void vDrvVideoAllocateDram(void) ;

// video call-back function
void vDrvVideoConnect(UINT8 bPath, UINT8 bOnOff) ;
void vDrvVideoTrigModeDetect(UINT8 bPath) ;
UINT16 wDrvVideoInputWidth(UINT8 bPath) ;
UINT16 wDrvVideoInputHeight(UINT8 bPath) ;
UINT8 bDrvVideoGetRefreshRate(UINT8 bPath)   ;
UINT8 bDrvVideoIsSrcInterlace(UINT8 bPath)   ;
UINT16 wDrvMDDIGetVPorch(UINT8 bPath) ;
UINT16 wDrvVideoGetVPorch(UINT8 bPath) ;
UINT16 wDrvVideoGetHPorch(UINT8 bPath) ;
UINT8 bDrvVideoSignalStatus(UINT8 bPath)   ;
UINT16 wDrvVideoGetHTotal(UINT8 bPath) ;
UINT16 wDrvVideoGetVTotal(UINT8 bPath) ;
UINT8 bDrvVideoIsSrc1080i(UINT8 bPath) ;
UINT8 bDrvVideoGetTiming(UINT8 bPath) ;

// drv_autocolor.h
extern void vDrvIntAutoColorStart(void) ;
extern void vDrvIntAutoColorState(void) ;

extern void vDrvExtAutoColorStart(void) ;
extern void vDrvExtAutoColorLoad(void) ;
extern void vDrvExtAutoColorState(void) ;

// vga_auto.h
extern void vDrvVGASetPhase(UINT8 bVal) ;
// Added by W.C for H/V position setting(2004/09/06)
extern void vDrvSetHPosition(UINT16 wValue) ;
extern void vDrvSetVPosition(UINT16 wValue) ;
extern void vVdoSP0AutoState(void) ;
//extern void vVdoSP1AutoState(void) ;
extern void vDrvVgaAutoStart(void) ;
extern void vDrvVgaAutoStop(void);
extern void vDrvYPbPrAutoStart(void); //YPbPr Auto Phase 2006/11/07

// drv_vdoclk.h
extern void vDrvVOFreqSet(UINT32 dwDCLK) ;
extern void vDrvSetSSMDRAsInputHsync(void);//for dynamic MDR
extern void vDrvVGASetClock(UINT16 wHtotal) ;
extern void vDrvSYSPLLPWUPIni(void) ;

// drv_vga.h
extern void vVgaModeDetect(void) ;
extern void vVgaChkModeChange(void) ;
#ifndef CC_UP8032_ATV
extern BOOL   _IsVgaDetectDone;
#else
extern UINT8   _IsVgaDetectDone;
#endif
extern UINT8   _bVgaTiming;
extern UINT8   _bVgaUserMode;
extern UINT8   bSP0Vclk;
extern UINT8  fgSP0Hpol; // VGA Politary Check Modify by W.C Shih2006/10/30
extern UINT8  fgSP0Vpol; // VGA Politary Check Modify by W.C Shih2006/10/30

// drv_dvi.h
extern void vDviModeDetect(void) ;
extern void vDviChkModeChange(void) ;
extern BOOL   _IsDviDetectDone;
extern UINT8   _bDviExtSrc;
extern UINT8   _bDviTiming;
extern UINT16   _wDviWidth;
extern UINT16   _wDviHeight;
extern UINT8   _bDviVclk;
#define fgIsExtSrcDVI() (_bDviExtSrc&SV_DV_DVI)
#define fgIsExtSrcHDMI() (_bDviExtSrc&SV_DV_HDMI)
#define fgIsExtSrcAdYPbPr() (_bDviExtSrc&SV_DV_AD_YPbPr)
#define fgIsExtSrcAdRGB() (_bDviExtSrc&SV_DV_AD_RGB)
#define fgIsExtSrcCCIR() (_bDviExtSrc&SV_DV_CCIR)
#define fgIsExtSrcFromCCIR() (_bDviExtSrc&SV_DV_FROM_CCIR)
#define fgIsExtSrcDTV() (_bDviExtSrc&SV_DV_DTV)

// drv_hdtv.h
extern void vHdtvModeDetect(void) ;
extern void vHdtvChkModeChange(void) ;
#if 1 // SUPPORT_SCART
extern void vScartRGBChkModeChange(void);
extern void vDrvScartRGBFreeRun(BYTE bOnOff) ;
#endif
extern UINT8   _IsHdtvDetectDone;
extern UINT8   _bHdtvTiming;

// vdo_isr.c
extern void vVdoInISR(void) ;
extern void vVdoOutISR(void);

// vga_table.h
extern UINT8 Get_VGAMODE_IVF(UINT8 mode) ;	// Refresh Rate
extern UINT16 Get_VGAMODE_IHTOTAL(UINT8 mode) ;
extern UINT16 Get_VGAMODE_IPH_WID(UINT8 mode) ; // WIDTH
extern UINT16 Get_VGAMODE_IPH_BP(UINT8 mode) ;
extern UINT8 Get_VGAMODE_IPV_STA(UINT8 mode) ;
extern UINT16 Get_VGAMODE_IPV_LEN(UINT8 mode) ; // HEIGHT

// drv_async.h
extern UINT16 wASVtotalMeasure(void) ;

// drv_tuner.h
extern BOOL _fgAutoSearch;

// drv_lbox
UINT8 bDrvLboxGetActiveImageArea(UINT16 *x, UINT16 *y, UINT16 *w, UINT16 *h);

#endif
