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
 * $Author: hongjun.chu $
 * $Date  $
 * $RCSfile: drv_hdmi.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _DRV_HDMI_H_
#define _DRV_HDMI_H_

#include "typedef.h"
#include "x_timer.h"

#define HDMI_8CH 0
#define MUTE_TEMP 1
#define Support_HW_Adaptive_EQ 1
#define IC_5365_VERIFY 1
#define PATCH_PIONEER_DV696_IR_OFF_ON_PURPLE_AND_SNOW_NOISE 1
#define Support_3D 1
#ifdef CC_Support_Sil9285
extern UINT8 _bHDMI9287switchstopflag;
#endif

extern UINT8 _bHPD_Indep_Ctrl;
extern UINT8 _bHDMIColorSpace;
// HDMI Event Flag
extern UINT8   _bHdmiFlag;
extern UINT8 _bHPDCycle;
extern UINT8 _bHdmiSrcGood;
extern UINT8 _bHDMIFastSW;
#define vSetHdmiFlg(arg) (_bHdmiFlag |= (arg))
#define vClrHdmiFlg(arg) (_bHdmiFlag &= (~(arg)))
#define fgIsHdmiFlgSet(arg) ((_bHdmiFlag & (arg)) > 0)
#define HDMI_CHECK (1<<0)
#define HDMI_OCLKDIV2 (1<<1)
#define HDMI_AUDIO_ON (1<<2)
#define HDMI_MODE_CHG (1<<3)

#define HDMI_RST_ALL	1
#define HDMI_RST_EQ		2
#define HDMI_RST_DEEPCOLOR	3
#define HDMI_RST_FIXEQ		4
#define HDMI_RST_RTCK		5
#define HDMI_RST_DIGITALPHY 6

#define MAX_HDMI_PORTS 4

typedef enum VideoAFD
{
    ICE_afd_not_present                 = -1,  /**< AFD not present, or AFD not yet found. */

    ICE_afd_reserved_0                  = 0x0, /**< AFD 0000 - Reserved. */
    ICE_afd_reserved_1                  = 0x1, /**< AFD 0001 - Reserved. */
    ICE_afd_16_by_9_top                 = 0x2, /**< AFD 0010 - box 16:9 (top). */
    ICE_afd_14_by_9_top                 = 0x3, /**< AFD 0011 - box 14:9 (top). */
    ICE_afd_gt_16_by_9                  = 0x4, /**< AFD 0100 - box >16:9 (centre). */
    ICE_afd_reserved_5                  = 0x5, /**< AFD 0101 - Reserved. */
    ICE_afd_reserved_6                  = 0x6, /**< AFD 0110 - Reserved. */
    ICE_afd_reserved_7                  = 0x7, /**< AFD 0111 - Reserved. */

    ICE_afd_same_as_coded_frame         = 0x8, /**< AFD 1000 - Same as coded frame. */
    ICE_afd_4_by_3                      = 0x9, /**< AFD 1001 - 4:3. */
    ICE_afd_16_by_9                     = 0xa, /**< AFD 1010 - 16:9. */
    ICE_afd_14_by_9                     = 0xb, /**< AFD 1011 - 14:9. */
    ICE_afd_reserved_12                 = 0xc, /**< AFD 1100 - Reserved. */
    ICE_afd_4_by_3_with_14_by_9_centre  = 0xd, /**< AFD 1101 - 4:3 with shoot and protect 14:9 centre. */
    ICE_afd_16_by_9_with_14_by_9_centre = 0xe, /**< AFD 1110 - 16:9 with shoot and protect 14:9 centre. */
    ICE_afd_shoot_and_protect_4_by_3    = 0xf  /**< AFD 1111 - 16:9 with shoot and protect 4:3 centre. */
} VideoAFD;

typedef struct
{
  UINT8 _bHdmiCmd;
  UINT8 _bHDMIState;
  UINT8 _bHDMIStateOld;  
  UINT8 _u1HdmiPwrOn;
  UINT8 _u1HdmiSrcIn;
  UINT8 _u8SkipHpd;
  UINT8 _bHdmiPlug;
  UINT8 _bUnplugFlag;
  UINT8 _u8HpdLow;
  UINT8 _u1Tx5VActive;
  UINT8 _u1RxTermOn;
  
  UINT8 _bHdmiSyncActive;
  UINT8 _bCKDTMonitor;
  UINT8 _u1ClkChg;
  UINT8 _bSCDTdelay;

  UINT8 _bResStableCnt;
  
  UINT8 _u1HdcpGood;
  UINT8 _bHDMIFastSW;

  UINT8 dReadDataOld;

  UINT8 _bRxShared;
  UINT8 _bEqTracking;
  
  UINT8 _bSyncLose;
  UINT8 _bTxSTBY;
  
  HAL_TIME_T _rHdmiUnplugTime;
  HAL_TIME_T _rHdmiLowPlugWaitTime;
  HAL_TIME_T _rHdmiSyncWaitTime;

#ifdef CC_SUPPORT_HDMI_CLK5VDET
	UINT8  _bNonStdDev;
	UINT8  _b5VActive;
	UINT8  _b5VDetCnt;
	UINT8  _bTmdsActive;
	UINT8  _bTmdsDetCnt;
	UINT8  _bHpdPulse;

    UINT32 _wHpdCnt;
    UINT32 _wDeltaCur;
#endif
} HDMI_STATUS_T;

typedef struct
{
  UINT8 Ch0;
  UINT8 Ch1;
  UINT8 Ch2;  
} HDMI_EQCH_T;

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
	HDMI_InfoFrame_ACP,
	HDMI_InfoFrame_VSDB
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


typedef enum eHDMI_SWITCH_NO {
	HDMI_SWITCH_INIT = 0,
	HDMI_SWITCH_1,
	HDMI_SWITCH_2,
	HDMI_SWITCH_3,
	HDMI_SWITCH_4,
	HDMI_SWITCH_MAX
}E_HDMI_SWITCH_NUM;


enum eHDMI_EQ_CHANNEL {
	HDMI_EQ_CH0 = 0,
	HDMI_EQ_CH1,
	HDMI_EQ_CH2
};

enum 
{
	HDMI_3D_Structure_FramePacking=0,
	HDMI_3D_Structure_FieldAlternative,
	HDMI_3D_Structure_LineAlternative,
	HDMI_3D_Structure_SideBySideFull,
	HDMI_3D_Structure_LDepth,
	HDMI_3D_Structure_LDepthGraph,
	HDMI_3D_Structure_TopBottom,
	HDMI_3D_Structure_SideBySideHalf=8,
	HDMI_3D_Structure_Unknow
};
typedef struct
{
	UINT8 HDMI_3D_Enable;
	UINT8 HDMI_3D_Video_Format;
	UINT8 HDMI_3D_Structure;
	UINT8 HDMI_3D_EXTDATA;
}HDMI_3D_INFOFRAME;
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
extern BOOL  _fgForce3D;
extern BOOL  _fgForce2D;
extern UINT8 _bForce3DStructure;
extern UINT8 _bForce3DExtData;
extern UINT8 _bEQTuning;
extern UINT8 _bKPGain;
extern UINT8 _bRXMode;
extern UINT8 _bEQGain;
extern UINT8 _bZSel;
extern UINT8 _bVRef;

extern UINT32 u4ActiveAnalogRegBase;
extern UINT32 u4ActiveHdmiRegBase;
extern UINT32 u4AnalogRegBase;
extern UINT32 u4HdmiRegBase;
extern E_HDMI_SWITCH_NUM eActiveHdmiPort;
extern E_HDMI_SWITCH_NUM eHDMICurrSwitch;
extern E_HDMI_SWITCH_NUM eCurrHdmiPort;

extern void vHDMIInit(void) ;
extern void vHDMI_pm_resume(void) ;
void vHDMISel(E_HDMI_SWITCH_NUM eSel);
UINT8 u1IsHdcpGood(E_HDMI_SWITCH_NUM eSrc);
UINT8 u1IsResStable(E_HDMI_SWITCH_NUM eSrc);
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
extern UINT8 bHDMIColorFormat(UINT8 method);


void vSetHdmiState(UINT8 u1State);
UINT8 u1GetHdmiState(void);
UINT8 u1GetHdmiCmd(void);
UINT8 u1IsHdmiUnplug(E_HDMI_SWITCH_NUM ePort);
void vClrHdmiUnplug(E_HDMI_SWITCH_NUM ePort);
void vSetHdmiUnplug(E_HDMI_SWITCH_NUM ePort);
UINT8 u1IsHdmiTxSTBY(E_HDMI_SWITCH_NUM ePort);
void vSetHdmiFastSw(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val);
UINT8 u1IsHdmiFastSw(E_HDMI_SWITCH_NUM ePort);
void vSetHdmiSrcIn(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val);
UINT8 u1IsHdmiSrcIn(E_HDMI_SWITCH_NUM ePort);
UINT8 u1HdmiHdcpCnt(E_HDMI_SWITCH_NUM ePort);
UINT8 u1IsHdmiResStable(E_HDMI_SWITCH_NUM ePort);
UINT8 u1IsHdmiRxShared(E_HDMI_SWITCH_NUM ePort);
UINT8 u1IsHdmiMode(void);	
UINT8 u1IsHdmiHpdPulse(E_HDMI_SWITCH_NUM ePort);
void vHdmiSetHpdStatus(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val);
UINT8 u1HDMI_Get5VStatus(E_HDMI_SWITCH_NUM ePort);
extern void vHDMISpdifOutEnable(BOOL fgEnable);
UINT8 bHDMIReadDevH(UINT8 bReg) ;
void vHDMIWriteDevH(UINT8 bReg, UINT8 bValue);
UINT8 bHDMIReadDevL(UINT8 bReg);
void vHDMIWriteDevL(UINT8 bReg, UINT8 bValue);
extern void vHDMIHDCPKeyInit(UINT8* pu1Ptr);

void vHDMISetSwitch(E_HDMI_SWITCH_NUM eSwitch);
extern UINT8 bHDMI3DPacketVaild(void);
#ifdef SUPPORT_HDMI_SWITCH_INIT
void vHDMISwitchInit(void);
#endif
void vHDMIGPIOInit(void);
// debug usage
BOOL fgHDMICRC(INT16 ntry);
BOOL fgHDMIPHYCRC(INT16 ntry);
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
void vHDMIHPDIndepCtrl(UINT8 cmd);
void vHDMITMDSIndepCTRL(E_HDMI_SWITCH_NUM ePort, UINT8 bOnOff);
void vHDMITMDSCTRL(E_HDMI_SWITCH_NUM ePort, UINT8 bOnOff);
extern void vHDMIHPDLow(E_HDMI_SWITCH_NUM eSwitch);
extern void vHDMIHPDHigh(E_HDMI_SWITCH_NUM eSwitch);
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
HDMI_3D_INFOFRAME* API_HDMI_3D_Get_InfoFrame(void);
void bHDMIEQTwoGear(UINT8);
UINT8 bHDMI422Input(void);
UINT8 bHDMIITCFlag(void);
UINT8 bHDMIITCContent(void);
void vHDMISetEQRsel(UINT8);
void vHDMIPowerOnOff(UINT8);
UINT8 bHDMIHDCPKeyCheck(void);
UINT8 bHDMIHDCPStatusGet(void);
void vHDMIDebug(UINT8,UINT32);
void bHdmiPhyReset(E_HDMI_SWITCH_NUM ePort, UINT8 resettype);
void bHDMIAudioSigmaDeltaSetting(UINT32,UINT8,UINT8);
void bHDMIAPPL2Reset(void);
UINT32 wDVIDecoder3DForce2D(UINT8, UINT8);
 #ifdef CC_BYPASS_AVI_INFOFRAME
void vHDMIDVIBypassAVIinfo(UINT8);
UINT8 bHDMIGetDVIBypassAVIinfo(void);
#endif

#ifdef CC_AUD_HDMI_SPDIF_CFG
extern UINT8 bHDMIACPType(void);
extern UINT8 bHDMIChannelStCategoryCode(void);
extern UINT8 bHDMIChannelStCopyright(void);
extern UINT8 bHDMI_IsPCM(void);
#endif

extern void vHDMISwitchInit(void);
extern UINT8 bHDMIPort5VStatus(E_HDMI_SWITCH_NUM ePort);

extern UINT8 u1IsSyncLose(E_HDMI_SWITCH_NUM ePort);
extern void vClrSyncLose(E_HDMI_SWITCH_NUM ePort);
extern void vSetSyncLose(E_HDMI_SWITCH_NUM ePort);

UINT8 u1HDMI_GetEdidData(UINT8 u1Port, UINT8 *pData, UINT16 u2Length);
UINT8 u1HDMI_GetHdcpKey(UINT8 *pData, UINT16 u2Length);

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
