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
 * $Date  $
 * $RCSfile: drv_hdmi.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _DRV_HDMI_H_
#define _DRV_HDMI_H_

#include "typedef.h"

#define HDMI_8CH 0
#define MUTE_TEMP 1

#define Support_EQ 				0
#define Support_HWAdaptive_EQ   0

#define PATCH_PIONEER_DV696_IR_OFF_ON_PURPLE_AND_SNOW_NOISE 1
	
#if defined(CC_MT5387) || defined(CC_MT5363)
	#define Support_EQ_HWAuto	1	// 1:Analog 0:Software
			#define PureAnalog_EQ  1	// 1: PureAnalog_EQ 0:SemiAnalog_EQ
#else
        #define Support_EQ_HWAuto	0	// 1:Analog 0:Software
		        #define PureAnalog_EQ  0	// 1: PureAnalog_EQ 0:SemiAnalog_EQ
#endif
#ifdef CC_Support_Sil9285
extern UINT8 _bHDMI9287switchstopflag;
#endif

extern UINT8 _bHPD_Indep_Ctrl;
extern UINT8 _bHDMIState;
// HDMI Event Flag
extern UINT8   _bHdmiFlag;
#define vSetHdmiFlg(arg) (_bHdmiFlag |= (arg))
#define vClrHdmiFlg(arg) (_bHdmiFlag &= (~(arg)))
#define fgIsHdmiFlgSet(arg) ((_bHdmiFlag & (arg)) > 0)
#define HDMI_CHECK (1<<0)
#define HDMI_OCLKDIV2 (1<<1)
#define HDMI_AUDIO_ON (1<<2)
#define HDMI_MODE_CHG (1<<3)
typedef struct
{
	UINT8 framesize;
	UINT32 framedata[10];
}HDMI_INFOFRAME_DESCRIPTION;
enum 
{
	HDMI_InfoFrame_AVI,
	HDMI_InfoFrame_SPD,
	HDMI_InfoFrame_AUDIO,
	HDMI_InfoFrame_MPEG,
	HDMI_InfoFrame_UNREC,
	HDMI_InfoFrame_GAMUT,
	HDMI_InfoFrame_ACP
};
enum 
{
	HDMI_STATE_NOTREADY,
	HDMI_STATE_INIT,
	HDMI_STATE_PWOFF,
	HDMI_STATE_PWON,
	HDMI_STATE_PWON2,
	HDMI_STATE_SCDT,
	HDMI_STATE_AUTH
};

#if 1 // HDMI_SWITCH
enum eHDMI_SWITCH_NO {
	HDMI_SWITCH_INIT = 0,
	HDMI_SWITCH_1,
	HDMI_SWITCH_2,
	HDMI_SWITCH_3,
	HDMI_SWITCH_4,
	HDMI_SWITCH_5,
	HDMI_SWITCH_6
};
#endif

#define HDMI_AUD_Length_Unknown  	0
#define HDMI_AUD_Length_16bits  	1
#define HDMI_AUD_Length_17bits  	2
#define HDMI_AUD_Length_18bits  	3
#define HDMI_AUD_Length_19bits  	4
#define HDMI_AUD_Length_20bits  	5
#define HDMI_AUD_Length_21bits  	6
#define HDMI_AUD_Length_22bits  	7
#define HDMI_AUD_Length_23bits  	8
#define HDMI_AUD_Length_24bits  	9

#define INFORM_MDCHG 1
#ifndef LIMITED_TIMING
#define LIMITED_TIMING 0
#endif
#if 1
#define CHECHCKDT 1
#define PWR5V_INT 0
#else
#define CHECHCKDT 0
#define PWR5V_INT 1
#endif

extern UINT8   _bHdmiMode;
extern UINT8 _bHdmiMD;
extern UINT8 _bHdmiCmd;
extern void vHDMIInit(void) ;
extern void vHDMIMainLoop(void) ;
extern void vHDMISetOclkDIV2(UINT8 fgEnable) ;
extern void vHDMISetOutputDrv(UINT8 bStr) ;
extern void vHDMIPowerOff(void) ;
extern void vHDMIVideoOutOn(void) ;
extern void vHDMIVideoOutOff(void) ;
extern void vHDMIAudioOutOn(void) ;
extern BOOL fgIsHDMIPlugOn(void) ;
extern BOOL fgHDMIAudioClkOk(void);
extern BOOL fgHDMISupportAudio(void);
extern UINT8 bHDMIGetAudSampleFreq(void);
extern void vHDMIHandleAudFmtChange(void);
extern UINT8 u1HDMIGetAudioSamplingFreq(void);


extern void vHDMISpdifOutEnable(BOOL fgEnable);
UINT8 bHDMIReadDevH(UINT8 bReg) ;
void vHDMIWriteDevH(UINT8 bReg, UINT8 bValue);
UINT8 bHDMIReadDevL(UINT8 bReg);
void vHDMIWriteDevL(UINT8 bReg, UINT8 bValue);
void vHDMISetSwitch(UINT8 bSwitch);
#ifdef SUPPORT_HDMI_SWITCH_INIT
void vHDMISwitchInit(void);
#endif
void vHDMIGPIOInit(void);
// debug usage
BOOL fgHDMICRC(INT16 ntry);
BOOL fgDVICRC(INT16 ntry);
void vHDMIBypassVdo(void);
void vHDMIHDCPKey(void);
void vHDMIReloadHDCPKey(void);
void vHDMIHDCPSelfBist(UINT8 mode);
#if 0
void vHDMIvLoadColorMatrix(BYTE idx);
#endif
void vHDMISetHDMIState(UINT8 tmp);
void vHDMIStopFw(void);
void vHDMIResumeFw(void);
void vHDMIDebugMsg(void);
UINT16 wHDMIResoWidth(void);
UINT16 wHDMIResoHeight(void);
UINT16 wHDMIHTotal(void);
BOOL fgHDMIinterlaced(void);
BOOL fgHDMIHsyncPolarity(void);
BOOL fgHDMIVsyncPolarity(void);
UINT32 dwHDMILineFreq(void);
UINT8 bHDMIRefreshRate(void);
UINT8 bHDMIAVIPixelCount(void);
BOOL fgHDMIHsyncAct(void);
void  vHDMIGetInfoFrame(UINT8);
UINT32 bHDMIGetHPDAdjust(void);
void vHDMIHPDAdjust(UINT32);
UINT16 wHDMIInfoFrameMask(void);
UINT8 bHDMIDeepColorStatus(void);
UINT8 bHDMIMCMHdcpWrite(void);
UINT8 _bIsXpcStable(void);

UINT8 vHDMIEQ(UINT8);

UINT8 bHDMIInputType(void);
UINT8 bHDMIClocrimetry(void);
#if INFORM_MDCHG
BOOL fgHDMIQueryModeChange(void);
#endif
void vHDMIHPDIndepCtrl(UINT8);
 void vHDMITMDSIndepCTRL(UINT8);
extern void vHDMIHPDLow(UINT8);
extern void vHDMIHPDHigh(UINT8);
extern UINT16 wHDMIVTotal(void);
//HDMI switch related
void vHDMITMDSFloating(void);
void vHDMITMDSPullHigh(void);
UINT8 bHDMIScanInfo(void);
UINT8 bHDMIAspectRatio(void);
UINT8 bHDMIAFD(void);
UINT8 bHDMIRgbRange(void);
UINT8 bGetHDMIRangeMode(void);
void vSetHDMIRangeMode(UINT8 bMode);
HDMI_INFOFRAME_DESCRIPTION* API_HDMI_Get_InfoFrame(UINT8);
extern UINT8 _bHDMICurrSwitch ;
void bHDMIEQTwoGear(UINT8);
UINT8 bHDMI422Input(void);
UINT8 bHDMIITCFlag(void);
UINT8 bHDMIITCContent(void);
void vHDMISetEQRsel(UINT8);
void vHDMIPowerOnOff(UINT8);
UINT8 bHDMIHDCPKeyCheck(void);
UINT8 bHDMIHDCPStatusGet(void);
void vHDMIDebug(UINT8,UINT32);

 #ifdef CC_BYPASS_AVI_INFOFRAME
void vHDMIDVIBypassAVIinfo(UINT8);
UINT8 bHDMIGetDVIBypassAVIinfo(void);
#endif

#if 1
extern UINT8   _bHDMIAudioInit ;
extern UINT8   _bHDMISampleChange;


#endif
#if 0
extern UINT8   _bHDMIAudioForm;
extern UINT8   _bHDMIAudioForm2;
extern UINT8   _bHDMIAudioForm3;
#endif
//extern UINT8   _bHDMIAudioForm4;
extern void vHDMIUpdateAudParm(void) ;
#endif
