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
 * $RCSfile: drv_vbi.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file VBI driver
 * @entry point
 *  VBI_ResetSrc
 *  VBI_Init
 * @use cases
 * init -> VBI_Init -> VBI_Enable
 * mode detect done -> VBI_ResetSrc
 *
 * @author daniel_hsiao@mtk.com.tw
 * @author pibben_tung@mtk.com.tw  for porting 8202B to 537x
 * @author pibben.tung@mediatek.com  for porting 5392(Revision 1.32) to 5360
 * @author kevin.hsu@mediatek.com  Sync 5392B to 5360
 * @author kevin.hsu@mediatek.com  Sync 5362b to 5387
 */
 
/**************************************************************************
 * Header Files
 *************************************************************************/ 
#include "x_assert.h"
#include "x_os.h"
#include "general.h"

#if (SUPPORT_CLOSE_CAPTION_VCHIP ||   \
     SUPPORT_WSS ||                   \
     SUPPORT_TELETEXT ||              \
     SUPPORT_AUTO_NAMING)

#include "hw_vbi.h"
#include "util.h"
#include "drv_vbi.h"
#include "drv_video.h"
#include "video_def.h"
#include "drvcust_if.h"
#include "api_notify.h"
#include "drv_tvd.h"
#include "hw_ckgen.h"
#include "nptv_debug.h"
#include "fbm_drvif.h"
/* For judge oversample or not by Pibben at 2006 8 1 */
#include "video_timing.h" 
/* For Cache and DRAM consistence */
#include "x_hal_arm.h"
/* For IC version */
#include "x_hal_5381.h"
#include "x_bim.h"
#ifdef CC_ATV_PVR_SUPPORT
#include "muxer_if.h"
extern UINT32 _AUD_ReadPsrStc1(VOID);
#endif

/**************************************************************************
 * Constant/Configure Definitions
 *************************************************************************/
/* For IC verification */
#define IC_VERIFY           1

/* For WSS525 Supporting*/
#define SUPPORT_WSS525      1 

#define CC_PITCH  16
#define TTX_PITCH 48

#define CC_SUCC_THRES 6
#define CC2X_SUCC_THRES 6
#define CC2X_FAIL_THRES 30
#define TTX_SUCC_THRES 6
#define TTX_FAIL_THRES 25
#define VPS_SUCC_THRES 5
#define VPS_FAIL_THRES 25
#define WSS625_SUCC_THRES 4
#define WSS625_FAIL_THRES 20
#define WSS525_SUCC_THRES 8
#define WSS525_FAIL_THRES 20
#define MAX_SUCC_THRES 255

/* WSS Time Out definition*/ 
#define WSS_UNKNOW  0x0
#define WSS_TIMEOUT_THRES 10

/* Wrong field number definition */
#define WRONG_FIELD_ID 0xFFFFFFFF 

#define CC_VCR_TRICK_CHECK 1
#define CC_VCR_TRICK_THRE 5
/* Enable slice WSS525 & WSS625 data */ 
#define WSS_DUAL_SLICER    1

//--------- Counter for COCH Mode ------------------------------//
#define VBI_CC_COCH_MATCH    0x60
#define VBI_CC_COCH_RST      0x40
#define VBI_CC_COCH_FAIL     0x20

#define vDrvVbiCoChCntRst(bCnt)        ((bCnt) = VBI_CC_COCH_RST)
#define vDrvVbiCoChCntInc(bCnt)        ((bCnt) = ((bCnt) == 0xFF)?0xFF:((bCnt)+1))
#define vDrvVbiCoChCntDec(bCnt)        ((bCnt) = ((bCnt) == 0x0)?0x0:((bCnt)-1))

#define VBI_RST_FIFO_WAITING_TIME    20

#if SUPPORT_TOFU_DISPLAY
#define CC_TOfU_DATA    0xA0
BOOL _bEnableTofuDisplay=TRUE;
UINT8 _u1TofuHiTh = 75;
UINT8 _u1TofuLoTh = 50;
BOOL _bVBI0StartTofoDisplay[2]={TRUE, TRUE};
//BOOL _bVBI2StartTofoDisplay[2]={TRUE, TRUE};
UINT8 _u1VBI0CCRdyRatio[2] = {100, 100};
//UINT8 _u1VBI2CCRdyRatio[2] = {100, 100};
#define vDrvVbiIncVBI0CCRatio(field)      (_u1VBI0CCRdyRatio[field] = (_u1VBI0CCRdyRatio[field] == 100) ?100:(_u1VBI0CCRdyRatio[field]+1))
#define vDrvVbiDecVBI0CCRatio(field)      (_u1VBI0CCRdyRatio[field] = (_u1VBI0CCRdyRatio[field] == 0) ?0:(_u1VBI0CCRdyRatio[field]-1))
//#define vDrvVbiIncVBI2CCRatio(field)      (_u1VBI2CCRdyRatio[field] = (_u1VBI2CCRdyRatio[field] == 100) ?100:(_u1VBI2CCRdyRatio[field]+1))
//#define vDrvVbiDecVBI2CCRatio(field)      (_u1VBI2CCRdyRatio[field] = (_u1VBI2CCRdyRatio[field] == 0) ?0:(_u1VBI2CCRdyRatio[field]-1))
#endif
/**************************************************************************
 * Imported functions (optional)
 *************************************************************************/

/**************************************************************************
 * Imported variables (optional)
 *************************************************************************/
extern RTvdStatus  _rTvd3dStatus;
extern UINT8 _na_state;
/**************************************************************************
 * Type definitions
 *************************************************************************/
typedef enum
{
    WSS625_Pass = 0,
    WSS525_Pass = 1,
    WSS_Error  = 2,
}WSS_CHK_RST;

/**************************************************************************
 * Macro definitions
 *************************************************************************/
#define fgIsVdoStable(bPath) (bDrvVideoSignalStatus(bPath) == (UINT8)SV_VDO_STABLE)

/**************************************************************************
 * Local/Static variables
 *************************************************************************/
/* VBI Working Modes*/
static VBI_MODE_T  _t_VbiMode;

static VBI_AV_INFO _VBIAvInfo ={E_FIELD_NORMAL, E_AV_UNKNOW};

/* For VBI0 FIFO Management */
static UINT8    _u1Vbi0PitchSz;
static UINT32   _u4Vbi0FifoSa;
static UINT32   _u4Vbi0FifoSz;
static UINT32   _u4NewVbi0WritePtr; /* Shared between ISR and State Machine */
static UINT32   _u4Vbi0ReadPtr;     /* Shared between ISR and State Machine */

/* For VBI2 FIFO Management */
static UINT8    _u1Vbi2PitchSz;
static UINT32   _u4Vbi2FifoSa;
static UINT32   _u4Vbi2FifoSz;
static UINT32   _u4NewVbi2WritePtr; /* Shared between ISR and State Machine */
static UINT32   _u4Vbi2ReadPtr;     /* Shared between ISR and State Machine */

#if SUPPORT_AUTO_NAMING
static UINT8    _au1Vps2RawData[13];
static UINT8    _u1Vps2FailNo;
static UINT8    _u1Vps2SuccNo;
#endif

#if SUPPORT_CLOSE_CAPTION_VCHIP

/* This counter is for detect NO CC data */
static UINT8    _u1CCFailNo[2] = {0};
static UINT8    _u1CC2FailNo[2] = {0};
static VBI_CC_TIMEOUT_INFO _CCTOutInfo ={FALSE, 30, 0xFF, 0xFF};
#if CC_VCR_TRICK_CHECK
static UINT8   _u1TrickBlock; /* To Block CC data */
static UINT32  _u4TrickPCounter;
static UINT32  _u4TrickNCounter;
static UINT8   _u1TrickCheckOn; /* To Turn VCR Trick mode check function */
#endif

#endif

#if SUPPORT_CLOSE_CAPTION_2X
static UINT8    _u1CC2XFailNo;
static UINT8    _u1CC2XSuccNo;
#endif

#if (SUPPORT_TELETEXT || SUPPORT_AUTO_NAMING)
static UINT8    _u1TtxFailNo[2] ={0};  //Count for Field 0 and Field 1
static UINT8    _u1TtxSuccNo[2] = {0};
static UINT8    _u1Ttx2FailNo[2] = {0};
static UINT8    _u1Ttx2SuccNo[2] = {0};
#endif

#if SUPPORT_WSS
static UINT32   _u4Wss;
static UINT32   _u4WssPrev;
static UINT8    _u1WssFailNo;
static UINT8    _u1WssSuccNo;
static UINT32   _u4Wss2;
static UINT32   _u4Wss2Prev;
static UINT8    _u1Wss2FailNo;
static UINT8    _u1Wss2SuccNo;
static UINT8    _u1WssTimeOutNo = 0;
static UINT8    _u1Wss2TimeOutNo = 0;
static VBI_WSS_TIMEOUT_INFO _WSSTOutInfo = {TRUE, TRUE, WSS_TIMEOUT_THRES, WSS_UNKNOW};
#endif

/* For VBI FIFO usage */
static BOOL _fgVbiFifoOn;
static BOOL _fgVbi2FifoOn;
static UINT32 _u4TTXPktAvail; /* Number of Packet available in one field */
#if SUPPORT_CLOSE_CAPTION_2X
static UINT32 _u4CC2XPktAvail = 0;  /* Number of Packet available in one field */
#endif
static BOOL _fgDramDumpEn=0;

/* These flags are for Closed Caption notifications */
static UINT8 _u1VbiCCMainNotify;
static UINT8 _u1VbiCCSubNotify;
static UINT8 _u1VbiCC2XMainNotify;
static UINT8 _u1VbiCC2XSubNotify;

/* These flags are for TTX notifications */
static UINT8 _u1VbiTTXMainNotify;
static UINT8 _u1VbiTTXSubNotify;

/* These flags are for EU WSS notifications */
static UINT8 _u1VbiEUWSSMainNotify;
static UINT8 _u1VbiEUWSSSubNotify;

/* These flags are for US WSS525 notifications */
static UINT8 _u1VbiUSWSSMainNotify;
static UINT8 _u1VbiUSWSSSubNotify;

#ifdef CC_ATV_PVR_SUPPORT
static UINT8 _u1VbiPvrNotify = 0;
//CC SEI FIFO
static UINT8 _u1VbiPVRCCDataSz;
static UINT32  _u4VbiPVRCCFifoSa;
static UINT32 _u4VbiPVRCCFifoSz;
static UINT32 _u4VbiPVRCCWritePtrStart; /* Shared between ISR and State Machine */
static UINT32 _u4VbiPVRCCWritePtrEnd;     /* Shared between ISR and State Machine */
//AFD SEI FIFO
static UINT8 _u1VbiPVRAFDDataSz;
static UINT32  _u4VbiPVRAFDFifoSa;
static UINT32 _u4VbiPVRAFDFifoSz;
static UINT32 _u4VbiPVRAFDWritePtrStart; /* Shared between ISR and State Machine */
static UINT32 _u4VbiPVRAFDWritePtrEnd;     /* Shared between ISR and State Machine */
static VBI_PVR_CC_SEI_DATA _rVbiCCSEI;
static VBI_PVR_AFD_SEI_DATA _rVbiAFDSEI;
#endif

/* This flag is for VPS CNI notification */
static UINT8 _u1VbiVPSCNINotify;
static UINT8 _u1VBI0Path; /* Record VBI0 is at Main or Sub */
static UINT8 _u1VBI2Path; /* Record VBI2 is at Main or Sub */

static UINT32 _u4VbiInitOnce = 0;
static UINT8 _u1TTXOnOff[SV_VP_NA] = { FALSE };
static HANDLE_T _hVbiMutex = 0;			// VBI Extern Function safe protection

static VBI_LINE_INFO _VbiLineInfo =
{
    FALSE,                                                                     /*bEnMenuLineSet*/
    {.u1F0_SET = AV_NML_NTSC_CC_SEL0, .u1F1_SET = AV_NML_NTSC_CC_SEL1},        /*AV_NTSC_CC*/
    {.u1F0_SET = AV_NML_PAL_CC_SEL0,  .u1F1_SET = AV_NML_PAL_CC_SEL1},         /*AV_PAL_CC*/
    {.u1F0_SET = AV_NML_NTSC_WSS_SEL0,.u1F1_SET = AV_NML_NTSC_WSS_SEL1},       /*AV_NTSC_WSS*/
    {.u1F0_SET = AV_NML_PAL_WSS_SEL0, .u1F1_SET = AV_NML_PAL_WSS_SEL1},        /*AV_PAL_WSS*/
    {.u1F0_SET = AV_NML_PAL_VPS_SEL,  .u1F1_SET = AV_NML_PAL_VPS_SEL},         /*AV_PAL_VPS*/
    {.u1F0_START = AV_NML_PAL_TT0_START,.u1F0_END = AV_NML_PAL_TT0_END,\
     .u1F1_START = AV_NML_PAL_TT1_START,.u1F1_END = AV_NML_PAL_TT1_END},       /*AV_PAL_TT*/
    {.u1F0_START = AV_NML_NTSC_CC2X0_START,.u1F0_END = AV_NML_NTSC_CC2X0_END,\
     .u1F1_START = AV_NML_NTSC_CC2X1_START,.u1F1_END = AV_NML_NTSC_CC2X1_END}, /*AV_NTSC_CC2X*/
    {.u1F0_SET = HDTV_NTSC_CC_SEL0, .u1F1_SET = HDTV_NTSC_CC_SEL1},            /*HDTV_NTSC_CC*/
    {.u1F0_SET = HDTV_PAL_CC_SEL0,  .u1F1_SET = HDTV_PAL_CC_SEL1},             /*HDTV_PAL_CC*/
    {.u1F0_SET = HDTV_NTSC_WSS_SEL0,.u1F1_SET = HDTV_NTSC_WSS_SEL1},           /*HTTV_NTSC_WSS*/
    {.u1F0_SET = HDTV_PAL_WSS_SEL0, .u1F1_SET = HDTV_PAL_WSS_SEL1},            /*HDTV_PAL_WSS*/
    {.u1F0_SET = HDTV_NTSC_WSS_PSCAN_SEL,.u1F1_SET = HDTV_NTSC_WSS_PSCAN_SEL}, /*HTTV_NTSC_WSS_PSCAN*/
    {.u1F0_SET = HDTV_PAL_WSS_PSCAN_SEL, .u1F1_SET = HDTV_PAL_WSS_PSCAN_SEL},  /*HDTV_PAL_WSS_PSAN*/
    {.u1F0_START = HDTV_PAL_TT0_START,.u1F0_END = HDTV_PAL_TT0_END,\
     .u1F1_START = HDTV_PAL_TT1_START,.u1F1_END = HDTV_PAL_TT1_END},           /*HDTV_PAL_TT*/
};

static CLI_NFY_T _eCliNfy_T = CLINfy_Disable;

#if WSS_DUAL_SLICER
static UINT8 _u1VideoType[2] = {0xFF};
#endif

#if AUTO_CC_DETECT
static UINT32 ui4ScanlineCCState = SCANLINE_IDLE;
static UINT8 ui1Scanline_Field0_RdyCnt[SCANLINE_CC_NUM];
static UINT8 ui1Scanline_Field1_RdyCnt[SCANLINE_CC_NUM];
static LINE rScanlineSeq_Normal[SCANLINE_CC_NUM] = 
{
    {.u1F0_SET = 18, .u1F1_SET = 18},   //NTSC 21, PAL M, PAL N(Field)
    {.u1F0_SET = 17, .u1F1_SET = 17},   //NTSC
    {.u1F0_SET = 16, .u1F1_SET = 16},   //NTSC
    {.u1F0_SET = 19, .u1F1_SET = 19},   //NTSC
};
static LINE rScanlineSeq_Inverse[SCANLINE_CC_NUM] = 
{
    {.u1F0_SET = 19, .u1F1_SET = 18},   //NTSC, PAL M, PAL N(Field)
    {.u1F0_SET = 18, .u1F1_SET = 17},   //NTSC
    {.u1F0_SET = 17, .u1F1_SET = 16},   //NTSC
    {.u1F0_SET = 20, .u1F1_SET = 19},   //NTSC
};
static LINE rCurrScanCCLine;
static UINT8 ui1Scanline_TestCnt;
static UINT8 ui1CCLineIndex;
static UINT8 ui1Scanline_RetryCnt;
static UINT8 ui1Scanline_SuccessIdx;
static UINT8 ui1Scanline_SuccessCnt;
static UINT8 ui1AutoCCEnable=TRUE;
#endif

#if VBI_CLI_SUPPORT
static NPTV_PFN_CC_NFY   _pfnVerCcNotify   = NULL;
static NPTV_PFN_CC2X_NFY _pfnVerCc2XNotify = NULL;
static NPTV_PFN_TTX_NFY  _pfnVerTtxNotify  = NULL;
static NPTV_PFN_WSS_NFY  _pfnVerWssNotify  = NULL;
static NPTV_PFN_VPS_NFY  _pfnVerVpsNotify  = NULL;
static VBI_PFN_CLI_NFY   _pfnCLINotify     = NULL;
#endif

#if EN_TVE_CB
static NPTV_PFN_CC_NFY   _pfnTVECcNotify      = NULL;
static NPTV_PFN_WSS_NFY  _pfnTVEWss625Notify  = NULL;
static NPTV_PFN_WSS_NFY  _pfnTVEWss525Notify  = NULL;
#endif

static HAL_TIME_T _rVbiFifoRstTime;
static BOOL _fgVbiFifoRstDone;
/**************************************************************************
 * Global variables
 *************************************************************************/

/**************************************************************************
 * Local Functions
 *************************************************************************/

static void _VBI_ISRDisable(void)
{
    vVDOINIrqOff(MSK_CC1);
    vVDOINIrqOff(MSK_CC2);
}


static void _VBI_ISREnable(void)
{
    vVDOINIrqOn(MSK_CC1);
    vVDOINIrqOn(MSK_CC2);
}

#ifdef CC_ATV_PVR_SUPPORT
static void _VBI_PVRSetFIFO(UINT32 u4FifoBase, UINT32 u4FifoSize) 
{    
    _u1VbiPVRCCDataSz = sizeof(VBI_PVR_CC_SEI_DATA);
    _u4VbiPVRCCFifoSa = u4FifoBase + VBI_FIFO_SIZE;
    _u4VbiPVRCCFifoSz = _u1VbiPVRCCDataSz*VBI_PVR_SEI_UNIT;
    _u4VbiPVRCCWritePtrStart = _u4VbiPVRCCFifoSa;
    _u4VbiPVRCCWritePtrEnd = _u4VbiPVRCCFifoSa;

    _u1VbiPVRAFDDataSz = sizeof(VBI_PVR_AFD_SEI_DATA);
    _u4VbiPVRAFDFifoSa = _u4VbiPVRCCFifoSa + _u4VbiPVRCCFifoSz;
    _u4VbiPVRAFDFifoSz = _u1VbiPVRAFDDataSz*VBI_PVR_SEI_UNIT;
    _u4VbiPVRAFDWritePtrStart = _u4VbiPVRAFDFifoSa;
    _u4VbiPVRAFDWritePtrEnd = _u4VbiPVRAFDFifoSa;

    //NAL Unit
    _rVbiCCSEI.u4NALUStartCode = 0x01000000;
    _rVbiCCSEI.u1ForbiddenZeroBit = 0;
    _rVbiCCSEI.u1NalRefIdc = 0;
    _rVbiCCSEI.u1NalUnitType = 0x06; 
     _rVbiCCSEI.u1SEIPayloadType = 0x04;
     _rVbiCCSEI.u1SEIPayloadSize = 25;
    //CC SEI
    _rVbiCCSEI.u1CountryCode = 0xB5;
    _rVbiCCSEI.u2ProviderCode = 0x3100;
    _rVbiCCSEI.u4UserIdentifier = 0x34394147;
    //User Structure
    _rVbiCCSEI.u4UserDataStartCode = 0xB2010000;
    _rVbiCCSEI.u4ATSCIdentifier = 0x34394147;
    _rVbiCCSEI.u1UserDataTypeCode = 0x03;
    _rVbiCCSEI.u1ProcessEMFlag = 0;
    _rVbiCCSEI.u1ProcessCCFlag = 1;
    _rVbiCCSEI.u1AdditionalDataFlag = 0;
    _rVbiCCSEI.u1CCCount = 0;
    _rVbiCCSEI.u1EMData = 0;
    _rVbiCCSEI.arCCData[0].u1MarkerBits = _rVbiCCSEI.arCCData[1].u1MarkerBits = 0x1F;
    _rVbiCCSEI.arCCData[0].u1CCValid = _rVbiCCSEI.arCCData[1].u1CCValid = 1;
    _rVbiCCSEI.u1MarkerBits = 0xFF;

    _rVbiCCSEI.u1RbspTrailingBits = 0x80;
    
    //NAL Unit    
    _rVbiAFDSEI.u4NALUStartCode = 0x01000000;
    _rVbiAFDSEI.u1ForbiddenZeroBit = 0;
    _rVbiAFDSEI.u1NalRefIdc = 0;
    _rVbiAFDSEI.u1NalUnitType = 0x06; 
     _rVbiAFDSEI.u1SEIPayloadType = 0x04;
     _rVbiAFDSEI.u1SEIPayloadSize = 17;
    //AFD SEI
    _rVbiAFDSEI.u1CountryCode = 0xB5;
    _rVbiAFDSEI.u2ProviderCode = 0x3100;
    _rVbiAFDSEI.u4UserIdentifier = 0x31475444;
    //User Structure
    _rVbiAFDSEI.u4UserDataStartCode = 0xB2010000;
    _rVbiAFDSEI.u4AFDIdentifier = 0x31475444;
    _rVbiAFDSEI.u1Zero = 0;
    _rVbiAFDSEI.u1ActiveFormatFlag = 1;
    _rVbiAFDSEI.u1Reserved1= 0x01;
    _rVbiAFDSEI.u1Reserved2= 0xF;

    _rVbiAFDSEI.u1RbspTrailingBits = 0x80;
}

static void _VBI_NotifyPVRCCData(UINT8 u1VbiPath, UINT32 u4Data0, UINT32 u4Data1, UINT32 u4Data2)
{
    if(_rVbiCCSEI.u1CCCount == 0)
    {
        _rVbiCCSEI.arCCData[0].u1CCData1 = (UINT8)u4Data0;
        _rVbiCCSEI.arCCData[0].u1CCData2 = (UINT8)u4Data1;
        _rVbiCCSEI.arCCData[0].u1CCType = (UINT8)u4Data2;
        _rVbiCCSEI.u1CCCount = 1;
        if(_rVbiCCSEI.arCCData[0].u1CCType!=0)
        {
            LOG(1, "[ATV_PVR] CC Data Loss\n");
        }
    }
    else if(_rVbiCCSEI.u1CCCount == 1)
    {
        _rVbiCCSEI.arCCData[1].u1CCData1 = (UINT8)u4Data0;
        _rVbiCCSEI.arCCData[1].u1CCData2 = (UINT8)u4Data1;
        _rVbiCCSEI.arCCData[1].u1CCType = (UINT8)u4Data2;
        _rVbiCCSEI.u1CCCount = 2;
        if(_rVbiCCSEI.arCCData[0].u1CCType!=1)
        {
            LOG(1, "[ATV_PVR] CC Data Loss\n");
        }
    }

    if(_rVbiCCSEI.u1CCCount == 2)
    {
        _u4VbiPVRCCWritePtrStart = _u4VbiPVRCCWritePtrEnd;
        x_memcpy((VBI_PVR_CC_SEI_DATA *)_u4VbiPVRCCWritePtrEnd, &_rVbiCCSEI, _u1VbiPVRCCDataSz);
        _u4VbiPVRCCWritePtrEnd += _u1VbiPVRCCDataSz;
        if(_u4VbiPVRCCWritePtrEnd>=(_u4VbiPVRCCFifoSa+_u4VbiPVRCCFifoSz))
        {
            _u4VbiPVRCCWritePtrEnd = _u4VbiPVRCCFifoSa;
        }
        _rVbiCCSEI.u1CCCount = 0;
    }
    
}

static void _VBI_NotifyPVRAFDData(UINT8 u1VbiPath, UINT32 u4Data0)
{
    _rVbiAFDSEI.u1ActiveFormat = (UINT8)u4Data0;
    _u4VbiPVRAFDWritePtrStart = _u4VbiPVRAFDWritePtrEnd;
    x_memcpy((VBI_PVR_AFD_SEI_DATA *)_u4VbiPVRAFDWritePtrEnd, &_rVbiAFDSEI, _u1VbiPVRAFDDataSz);
    _u4VbiPVRAFDWritePtrEnd += _u1VbiPVRAFDDataSz;
    if(_u4VbiPVRAFDWritePtrEnd>=(_u4VbiPVRAFDFifoSa+_u4VbiPVRAFDFifoSz))
    {
        _u4VbiPVRAFDWritePtrEnd = _u4VbiPVRAFDFifoSa;
    }
}

static void _VBI_NotifyPVRTTXData(UINT32 u4UnitNs, UINT32 u4StartAddr, UINT32 u4Pts)
{
#ifdef CC_ATV_PVR_SUPPORT
    VBIENC_PES_INFO_T rTTXEncPes;
    extern UCHAR *_pucTtxBuf;
    extern UINT32 _u4TtxBufWp;
#endif

    //printf("TTX: sa = %08X  ns = %d  PTS = %d\n", u4StartAddr, u4UnitNs, u4Pts);

#ifdef CC_ATV_PVR_SUPPORT
    x_memcpy(_pucTtxBuf + _u4TtxBufWp, (VOID *)u4StartAddr, u4UnitNs * 48);
    _u4TtxBufWp += (u4UnitNs * 48);
    
    rTTXEncPes.u4FifoStart = 0;
    rTTXEncPes.u4FifoEnd = 0;
    rTTXEncPes.u4StartAddr = u4StartAddr;
    rTTXEncPes.u4FrameSize = u4UnitNs * 48;
    rTTXEncPes.u4Pts = u4Pts;
    MUXER_SendVBIPes(0, &rTTXEncPes);
#endif
}
#endif


static void _VBINotify(VBI_SERVICE_T eVbiType, 
                        UINT8 u1VbiPath,
                        UINT32 u4Data0,
                        UINT32 u4Data1,
                        UINT32 u4Data2)
{
    switch (eVbiType)
    {
        case VBI_TTX_SERVICE:
            if (_eCliNfy_T != CLINfy_Disable)
            {
                if (_pfnVerTtxNotify)
                {
                    _pfnVerTtxNotify(u1VbiPath, u4Data0, u4Data1);
                }
            }
            if (_eCliNfy_T != CLINfy_EnOnly)
            {
                if(fgHwTvdVPresTVD3D())
                {
                    //Printf("No signal: TTX notify\n");
                    vAPiNotifyTTXData(u1VbiPath, u4Data0, u4Data1);
                	LOG(9, "--VBI Notify [ReadAddr, WriteAddr, packet]=[%x,%x,%d]\n",_u4Vbi0ReadPtr,_u4NewVbi0WritePtr,u4Data0); 

                }
            } 
#ifdef CC_ATV_PVR_SUPPORT
            //Sample code
            if(u1VbiPath == _u1VBI0Path && _u1VbiPvrNotify==TRUE)
            {
                _VBI_NotifyPVRTTXData(u4Data0/*number of units*/, u4Data1/*start addr*/, _AUD_ReadPsrStc1()/*PTS*/);
            }
#endif
            break;
        case VBI_CC_SERVICE:
            if (_eCliNfy_T != CLINfy_Disable)
            {
                if (_pfnVerCcNotify)
                {
                    _pfnVerCcNotify(u1VbiPath, (UINT8)u4Data0, (UINT8)u4Data1, (UINT8)u4Data2);
                }
            }
            if (_eCliNfy_T != CLINfy_EnOnly)
            {
                vAPiNotifyCCData(u1VbiPath, (UINT8)u4Data0, (UINT8)u4Data1, (UINT8)u4Data2);                                                                    
            }
            break;
        case VBI_CC2X_SERVICE:
            if (_eCliNfy_T != CLINfy_Disable)
            {
                if (_pfnVerCc2XNotify)
                {
                    _pfnVerCc2XNotify(u1VbiPath, u4Data0, u4Data1);
                }
            }
            if (_eCliNfy_T != CLINfy_EnOnly)
            {
                vAPiNotifyCC2XData(u1VbiPath, u4Data0, u4Data1);                                                                                       
            }            
            break;
        case VBI_WSS525_SERVICE:
            if (_eCliNfy_T != CLINfy_Disable)
            {
                if (_pfnVerWssNotify)
                {
                    _pfnVerWssNotify(u1VbiPath, u4Data0);                        
                }
            }
            if (_eCliNfy_T != CLINfy_EnOnly)
            {
                #if SUPPORT_WSS525            
                vAPiNotifyUSWSSData(u1VbiPath,  u4Data0);
                #endif
            }
            #ifdef CC_ATV_PVR_SUPPORT
            if(u1VbiPath == _u1VBI0Path && _u1VbiPvrNotify==TRUE)
            {
                _VBI_NotifyPVRAFDData(u1VbiPath,  u4Data0);
            }
            #endif
            break;
        case VBI_WSS625_SERVICE:
            if (_eCliNfy_T != CLINfy_Disable)
            {
                if (_pfnVerWssNotify)
                {
                    _pfnVerWssNotify(u1VbiPath, u4Data0);
                }
            }
            if (_eCliNfy_T != CLINfy_EnOnly)
            {
                vAPiNotifyEUWSSData(u1VbiPath, u4Data0);
            }
            #ifdef CC_ATV_PVR_SUPPORT
            if(u1VbiPath == _u1VBI0Path && _u1VbiPvrNotify==TRUE)
            {
                _VBI_NotifyPVRAFDData(u1VbiPath,  u4Data0);
            }
            #endif
            break;
        case VBI_VPS_SERVICE:
            if (_eCliNfy_T != CLINfy_Disable)
            {
                if (_pfnVerVpsNotify)
                {
                    _pfnVerVpsNotify((UINT8 *)u4Data0);
                }
            }
            if (_eCliNfy_T != CLINfy_EnOnly)
            {
                vAPiNotifyVPSData((UINT8 *)u4Data0);                       
            }
            break;
        default:
            break;
    }

    return;
}





/**
 * @brief   Set VBI FIFO parameters
 *
 * @param   u1VBISuite: VBI0 / VBI2
 * @param   u4Addr: 
 * @param   u2UnitNum: 
 * @param   u1FifoMode: 
 *
 * @retval  None
 */
static void _VBISetFifoPara(UINT8 u1VBISuite, UINT32 u4Addr, UINT16 u2UnitNum, UINT8 u1FifoMode) 
{
    CRIT_STATE_T csState;
    
    switch (u1FifoMode)
    {
        #if SUPPORT_CLOSE_CAPTION_VCHIP
        case CC_FIFO:
            if (u1VBISuite == VBI0)
            {
                _u1Vbi0PitchSz = CC_PITCH;
            }
            else
            {
                _u1Vbi2PitchSz = CC_PITCH;
            }
            break;
        #endif // #if SUPPORT_CLOSE_CAPTION_VCHIP
        
        #if SUPPORT_TELETEXT
        case TTX_FIFO:
            if (u1VBISuite == VBI0)
            {
                _u1Vbi0PitchSz = TTX_PITCH;
            }
            else
            {
                _u1Vbi2PitchSz = TTX_PITCH;
            }
            break;
        #endif // #if SUPPORT_TELETEXT
        default:
            break;
    }

    csState = x_crit_start();
    
    if(u1VBISuite == VBI0)
    {
        _u4Vbi0FifoSa = u4Addr;
        _u4Vbi0FifoSz = (UINT32)u2UnitNum *_u1Vbi0PitchSz;
        _u4Vbi0ReadPtr = u4Addr;
        _u4NewVbi0WritePtr = u4Addr;
    }
    else
    {
        _u4Vbi2FifoSa = u4Addr;
        _u4Vbi2FifoSz = (UINT32)u2UnitNum *_u1Vbi2PitchSz;
        _u4Vbi2ReadPtr = u4Addr;
        _u4NewVbi2WritePtr = u4Addr;
    }
    
    x_crit_end(csState);
    
    if (u1VBISuite == VBI0)
    {
        vHwVbiSetMbase0(u1VBISuite, _u4Vbi0FifoSa);
        vHwVbiSetMOffset(u1VBISuite, _u4Vbi0FifoSz);
    }
    else
    {
        vHwVbiSetMbase0(u1VBISuite, _u4Vbi2FifoSa);
        vHwVbiSetMOffset(u1VBISuite, _u4Vbi2FifoSz);
    }
    
    vHwVbiSetFifoMode(u1VBISuite, VBI_FIFO_MODE);

}

/**
 * @brief   Enable VBI HW by path. (It is called by VBI_ResetSrc)
 *
 * @param   u1VBISuite: VBI0 / VBI2
 *
 * @retval  None
 */
static void _VBIResetFifo(UINT8 u1VBISuite) 
{
    CRIT_STATE_T csState;
    
    vHwVbiResetFifo(u1VBISuite);
    HAL_GetTime(&_rVbiFifoRstTime);
    _fgVbiFifoRstDone = FALSE;
    
    csState = x_crit_start();
    
    if(u1VBISuite == VBI0)
    {
        _u4Vbi0ReadPtr = _u4Vbi0FifoSa;
        _u4NewVbi0WritePtr = _u4Vbi0FifoSa;
        //x_memset((void *)_u4Vbi0FifoSa, 0, _u4Vbi0FifoSz);
    }
    else
    {
        _u4Vbi2ReadPtr = _u4Vbi2FifoSa;
        _u4NewVbi2WritePtr = _u4Vbi2FifoSa;
    }
    
    x_crit_end(csState);
}

#if EN_TVE_CB
static void _VBIEnCC_WSS_Slicer(UINT8 u1VBISuite)
{
    vHwVbiSetLineEnable(u1VBISuite, (WSS_LINE_EN | CC_LINE_EN));
}
#endif


/**
 * @brief   Enable VBI HW by path. (It is called by VBI_ResetSrc)
 *
 * @param   u1VBISuite: VBI0 / VBI2
 *
 * @retval  None
 */
static void _VBIEnableBySlicer(UINT8 u1VBISuite) 
{
    if (u1VBISuite == VBI0)
    {
        #if !IC_VERIFY
        vWriteCLKMsk(C_CLK_CFG_L, 0, VBI_CLK_STOP);
        vWriteCLKMsk(MCLK_EN_2, VBI_MCLK_EN, VBI_MCLK_EN);
        #endif

        //vHwVbiCoreReset(VBI0);    
    }
    else
    {
        #if !IC_VERIFY
        vWriteCLKMsk(C_CLK_CFG_L, 0, VBI2_CLK_STOP);    
        vWriteCLKMsk(MCLK_EN_2, VBI2_MCLK_EN, VBI2_MCLK_EN);        
        #endif
        
        //vHwVbiCoreReset(VBI2);    
    }
    
    switch (_t_VbiMode)
    {
        #if (SUPPORT_CLOSE_CAPTION_VCHIP || SUPPORT_CLOSE_CAPTION_2X)
        case VBI_CC_VCHIP_MODE:
            vHwVbiSetLineEnable(u1VBISuite, (CC_LINE_EN | WSS_LINE_EN));
            #if SUPPORT_CLOSE_CAPTION_2X
            vHwVbiSetLineEnable(u1VBISuite, (TT_LINE_EN | CC_LINE_EN | WSS_LINE_EN));
            #endif
            break;
        #endif
            
        #if SUPPORT_TELETEXT
        case VBI_TTX_MODE:
            vHwVbiSetLineEnable(u1VBISuite, (WSS_LINE_EN | TT_LINE_EN));
            break;
        #endif
            
        #if (SUPPORT_WSS &&             \
             !SUPPORT_CLOSE_CAPTION_VCHIP &&  \
             !SUPPORT_TELETEXT &&       \
             !SUPPORT_AUTO_NAMING)
        case VBI_WSS_ONLY_MODE:
            vHwVbiSetLineEnable(u1VBISuite, WSS_LINE_EN);
            break;
        #endif
        default:
            break;
    }        
}

/**
 * @brief    Enable VBI FIFO 
 * @param    u1VBISuite: VBI0 / VBI2
 * @retval    None
 */

static void _VBI_FifoEnable(UINT8 u1VBISuite) 
{
    vHwVbiSetOutputMode(u1VBISuite, VBI_OMODE_DEC);
    
    vHwVbiClrResetFifo(u1VBISuite);
    
    if(u1VBISuite == VBI0)
    {
        _fgVbiFifoOn = TRUE;
    }
    else
    {
        _fgVbi2FifoOn = TRUE;
    }
}

/**
 * @brief    Disable VBI FIFO 
 * @param    u1VBISuite: VBI0 / VBI2
 * @retval   None
 */

void _VBI_FifoDisable(UINT8 u1VBISuite) 
{
    vHwVbiSetOutputMode(u1VBISuite, VBI_OMODE_NO);
    
    if (u1VBISuite == VBI0)
    {
        _fgVbiFifoOn = FALSE;
    }
    else
    {
        _fgVbi2FifoOn = FALSE;
    }
}


/**
 * @brief   Get current field
 *
 * @param   u1VbiSuite: VBI0 / VBI2                    
 *
 * @retval  Field id
 */
static UINT32 _VBI_GetCurField(UINT8 u1VbiSuite)
{
    UINT32 u4CurField = WRONG_FIELD_ID;
    UINT8 u1VbiSrc;
    
    u1VbiSrc = bHwVbiGetSrc(u1VbiSuite);

    if (u1VbiSrc == VBI_VSRC_TVD3D)
    {
        if (u1VbiSuite == VBI0)
        {
            //Printf("_VBI_GetCurField: VBI0 --> TVD.\n");
            
            if (_VBIAvInfo.E_AVFieldType == E_FIELD_INVERSED)
            {
                u4CurField = (u4GetVbiField() == 0) ? 1 : 0;
            }
            else
            {
                u4CurField = (u4GetVbiField());
            }
        }
        else
        {
            //Printf("_VBI_GetCurField: VBI2 --> TVD.\n");
            
            if (_VBIAvInfo.E_AVFieldType == E_FIELD_INVERSED)
            {
                u4CurField = (u4GetVbi2Field() == 0) ? 1 : 0;
            }
            else
            {
                u4CurField = (u4GetVbi2Field());
            }

        }
    }
    else if (u1VbiSrc == VBI_VSRC_HDTV)
    {        
        if (u1VbiSuite == VBI0)
        {
            //Printf("_VBI_GetCurField: VBI0 --> HDTV.\n");
            #if HDTV_FIELD_INVERSE
                u4CurField = (u4GetVbiField() == 0) ? 1 : 0;
            #else
                u4CurField = (u4GetVbiField());
            #endif  
        }
        else
        {
            //Printf("_VBI_GetCurField: VBI2 --> HDTV.\n");
            #if HDTV_FIELD_INVERSE
                u4CurField = (u4GetVbi2Field() == 0) ? 1 : 0;
            #else
                u4CurField = (u4GetVbi2Field());
            #endif 
        }
    }

    return u4CurField;
}

/**
 * @brief   Check WSS data validation
 *
 * @param   u1VbiPath: SV_VP_MAIN / SV_VP_PIP
 * @param   u4Field: Field0 / Field1
 * @param   u4Wss: WSS data   
 * @param   u1WssMainNoftiyOnOff: Main Path Notify is On/Off 
 * @param   u1WssSubNofityOnOff: Sub Path Notify is On/Off
 * @param   u1WSSSuccCnt: WSS successful counter. 
 * @retval  WSS_CHK_RST 
 */
static WSS_CHK_RST _VBI_WSSValidityCheck(UINT8 u1VbiPath, 
                                    UINT32 u4Field, 
                                    UINT32 u4Wss,
                                    UINT8 u1WssMainNoftiyOnOff,
                                    UINT8 u1WssSubNofityOnOff,
                                    UINT8 u1WSSSuccCnt)
{    
    WSS_CHK_RST  WssChkRst = WSS_Error;// u4Result = FALSE;
    UINT8 u1VideoType = bDrvVideoGetType(u1VbiPath);

    // Check Source is stable and CB Notify is enabled
    if (((u1WssMainNoftiyOnOff == SV_ON) && 
        (u1VbiPath == SV_VP_MAIN)) 
        ||
        ((u1WssSubNofityOnOff == SV_ON) && 
        (u1VbiPath == SV_VP_PIP)))
    {
        //WSS Validity Check
        if (u1VideoType == VDOTYPE_NTSC) 
        {
            ///TODO:  Check CRC b16-19

            /* Check WSS Success Threshold */ 
            WssChkRst = (u1WSSSuccCnt >= WSS525_SUCC_THRES) ?  WSS525_Pass : WSS_Error;
        }
        else if (u1VideoType == VDOTYPE_PAL)
        {   
            UINT32 u4Cond0 = 0;
            UINT32 u4Cond1 = 0;
            UINT32 u4Cond2 = 0;
            
            ///TODO:  Check odd parity  b3        
            
            /* Bit7 should be always 0*/
            u4Cond0 = ((u4Wss & 0x80) == 0) ? TRUE : FALSE;	 

            /* Field check, wss625 only comes out in field 0 */
            /* In PScan Mode, don't care the field 0 or field 1 */
            if ((fgIsPScanTiming()))
                u4Cond1 = TRUE;
            else
            	u4Cond1 = (u4Field == 0) ? TRUE : FALSE; 

            /* Check WSS Success Threshold */ 
            u4Cond2 = (u1WSSSuccCnt >= WSS625_SUCC_THRES) ?  TRUE : FALSE;

            WssChkRst = (u4Cond0 & u4Cond1 & u4Cond2) ? WSS625_Pass : WSS_Error;

        }     
    }
    //Printf("_VBI_WSSValidityCheck: Check Result %d.\n", WssChkRst);
    return WssChkRst;   
}

/**
 * @brief   Get WSS data
 *
 * @param   u1VbiPath: SV_VP_MAIN / SV_VP_PIP
 * @param   u1VbiSuite: VBI0 / VBI2                   
 *
 * @retval  WSS Data
 */
static UINT32 _VBI_GetWSSData(UINT8 u1VbiPath, UINT8 bVBISuite)
{
    UINT32 u4WssData;
    UINT8 u1VideoType = bDrvVideoGetType(u1VbiPath);

    u4WssData = (bVBISuite == VBI0) ? u4GetWSS(): u4GetWSS2();
    
    if (u1VideoType == VDOTYPE_PAL) 
    {
        u4WssData = (u4WssData & 0x3FFF);  //WSS625 B0~B13 is valid
    }
    else if (u1VideoType == VDOTYPE_NTSC)
    {
        u4WssData = (u4WssData & 0xFFFFF); //WSS525 B0~B19 is valid
    }
    else
    {
        LOG(5, "Warning, Get WSS data in unexpected type [%d]", u1VideoType);
    }
    return u4WssData;
}

/**
 * @brief   Start TTX Notify
 *
 * @param   u1VbiPath: SV_VP_MAIN / SV_VP_PIP
 * @param   u1OnOff:  TRUE: Enable Notify / False: Disable Notify                  
 * @param   pfnTTXnotify : reserved. 
 * @retval  WSS Data
 */
static void _VBI_StartNotifyTTXOnOff(UINT8 u1Path, UINT8 u1OnOff, NPTV_PFN_TTX_NFY pfnTTXnotify)
{
    UINT8 u1MatchVBISuite; /* u1Path is matched with which VBI slicer. VBI0 or VBI2 */
   
    if(u1Path == SV_VP_MAIN)
    {    
        _u1VbiTTXMainNotify = u1OnOff;
    }
    else if(u1Path == SV_VP_PIP)
    {
        _u1VbiTTXSubNotify = u1OnOff;
    }

    if(_u1VBI0Path == u1Path)
    {
        u1MatchVBISuite = VBI0;
    }
    else
    {
        u1MatchVBISuite = VBI2;
    }
    if(u1OnOff == SV_ON)
    {
        _VBI_FifoEnable(u1MatchVBISuite);
    }
    else
    {
        _VBI_FifoDisable(u1MatchVBISuite); 
        _VBIResetFifo(u1MatchVBISuite);
    }

//    _pfnTtxNfy = pfnTTXnotify;
}

#if CC_VCR_TRICK_CHECK
static void _VBI_TrickValidate(void)
{
    UINT32 u4TvdAvgVlen = wHwTvdAvgVlen();
    UINT32 u4VLenError = 0; /* Is Vertical Length counting error */

    if(_u1TrickCheckOn == 1)
    {
        if(fgHwTvdTrick())
        {
            if(_rTvd3dStatus.fgIs525)
            {
                if((u4TvdAvgVlen>(0x20C+2)) || (u4TvdAvgVlen<(0x20C-2)))
                {
                    u4VLenError = 1;
                }
            }
            else
            {
                if((u4TvdAvgVlen>(0x270+2)) || (u4TvdAvgVlen<(0x270-2)))
                {
                    u4VLenError = 1;
                }
            }
        }
        if(fgHwTvdTrick() && (u4VLenError == 1))
        {
            if (_u4TrickPCounter < CC_VCR_TRICK_THRE)
            {
                _u4TrickPCounter++;
            }
            else
            {
                if(_u1TrickBlock == SV_OFF)
                {                    
                    _u1TrickBlock = SV_ON;
                    LOG(5,"VCR TRICK mode Start\n");
                }
                _u4TrickNCounter = 0;
            }
        }
        else
        {
            if (_u4TrickNCounter < CC_VCR_TRICK_THRE)
            {
                _u4TrickNCounter++;
            }
            else
            {
                if(_u1TrickBlock == SV_ON)
                {                    
                    _u1TrickBlock = SV_OFF;
                    LOG(5,"VCR TRICK mode End\n");
                }
                _u4TrickPCounter = 0;
            }
        }
    }
}
#endif


/**
 * @brief   Set VBI Line Infomation.
 *
 * @param   u1VBISuite: VBI_0 / VBI_2
 * @param   u1SrcType : SV_VD_YPBPR / SV_VD_TVD3D
 * @param   u1VideoType: VDOTYPE_NTSC / VDOTYPE_PAL
 * @param   E_AVFieldType: Setting VBI Lines based on E_FIELD_NORMAL / E_FIELD_INVERSED
 *
 * @retval  None
 */
static void _VBI_SetVBILines(VBI_LINE_INFO* _pVbiLineInfo, UINT8 u1VBISuite, UINT8 u1SrcType, UINT8 u1VideoType, AV_FIELD_TYPE E_AVFieldType)
{
    VBI_LINE_INFO VbiLineInfo;

    x_memcpy(&VbiLineInfo, _pVbiLineInfo, sizeof (VBI_LINE_INFO));

    LOG(3, "_VBI_SetVBILines: Filed Type [%d]\n", E_AVFieldType);

    if (E_AVFieldType == E_FIELD_INVERSED)
    {
        VbiLineInfo.AV_NTSC_CC.u1F0_SET  += (AV_IVS_NTSC_CC_SEL0  - AV_NML_NTSC_CC_SEL0);
        VbiLineInfo.AV_NTSC_CC.u1F1_SET  += (AV_IVS_NTSC_CC_SEL1  - AV_NML_NTSC_CC_SEL1);
        VbiLineInfo.AV_PAL_CC.u1F0_SET   += (AV_IVS_PAL_CC_SEL0   - AV_NML_PAL_CC_SEL0);
        VbiLineInfo.AV_PAL_CC.u1F1_SET   += (AV_IVS_PAL_CC_SEL1   - AV_NML_PAL_CC_SEL1);
        VbiLineInfo.AV_NTSC_WSS.u1F0_SET += (AV_IVS_NTSC_WSS_SEL0 - AV_NML_NTSC_WSS_SEL0);
        VbiLineInfo.AV_NTSC_WSS.u1F1_SET += (AV_IVS_NTSC_WSS_SEL1 - AV_NML_NTSC_WSS_SEL1);
        VbiLineInfo.AV_PAL_WSS.u1F0_SET  += (AV_IVS_PAL_WSS_SEL0  - AV_NML_PAL_WSS_SEL0);
        VbiLineInfo.AV_PAL_WSS.u1F1_SET  += (AV_IVS_PAL_WSS_SEL1  - AV_NML_PAL_WSS_SEL1);
        VbiLineInfo.AV_PAL_VPS.u1F0_SET  += (AV_IVS_PAL_VPS_SEL   - AV_NML_PAL_VPS_SEL);
        VbiLineInfo.AV_PAL_VPS.u1F1_SET  += (AV_IVS_PAL_VPS_SEL   - AV_NML_PAL_VPS_SEL);
        VbiLineInfo.AV_PAL_TT.u1F0_START += (AV_IVS_PAL_TT0_START - AV_NML_PAL_TT0_START);
        VbiLineInfo.AV_PAL_TT.u1F0_END   += (AV_IVS_PAL_TT0_END   - AV_NML_PAL_TT0_END);
        VbiLineInfo.AV_PAL_TT.u1F1_START += (AV_IVS_PAL_TT1_START - AV_NML_PAL_TT1_START);
        VbiLineInfo.AV_PAL_TT.u1F1_END   += (AV_IVS_PAL_TT1_END   - AV_NML_PAL_TT1_END);
        #if SUPPORT_CLOSE_CAPTION_2X
        VbiLineInfo.AV_NTSC_CC2X.u1F0_START += (AV_IVS_NTSC_CC2X0_START - AV_NML_NTSC_CC2X0_START);
        VbiLineInfo.AV_NTSC_CC2X.u1F0_END   += (AV_IVS_NTSC_CC2X0_END   - AV_NML_NTSC_CC2X0_END);
        VbiLineInfo.AV_NTSC_CC2X.u1F1_START += (AV_IVS_NTSC_CC2X1_START - AV_NML_NTSC_CC2X1_START);
        VbiLineInfo.AV_NTSC_CC2X.u1F1_END   += (AV_IVS_NTSC_CC2X1_END   - AV_NML_NTSC_CC2X1_END);
        #endif
    }
            
    /* Line Selection */
    switch (u1VideoType)
    {
        case VDOTYPE_NTSC:

            LOG(5, "VBI_ResetSrc: NTSC mode \n");
            
            if (u1SrcType == (UINT8)SV_VD_YPBPR)
            {
                /* CC_SEL */
                LOG(5, "[VBI] NTSC HDTV CC line %d %d \n", VbiLineInfo.HDTV_NTSC_CC.u1F0_SET, VbiLineInfo.HDTV_NTSC_CC.u1F1_SET); 
                vHwVbiSetCCLine(u1VBISuite, VbiLineInfo.HDTV_NTSC_CC.u1F0_SET, VbiLineInfo.HDTV_NTSC_CC.u1F1_SET);
                
                /* WSS_SEL */
                if(fgIsPScanTiming())
                {
                    LOG(9, "[VBI] NTSC HDTV PSCAN WSS line %d %d \n", VbiLineInfo.HDTV_NTSC_WSS_PSCAN.u1F0_SET, VbiLineInfo.HDTV_NTSC_WSS_PSCAN.u1F1_SET);                           
                    vHwVbiSetWSSLine(u1VBISuite, VbiLineInfo.HDTV_NTSC_WSS_PSCAN.u1F0_SET, VbiLineInfo.HDTV_NTSC_WSS_PSCAN.u1F1_SET);
                }
                else
                {
                    LOG(9, "[VBI] NTSC HDTV WSS line %d %d \n", VbiLineInfo.HDTV_NTSC_WSS.u1F0_SET, VbiLineInfo.HDTV_NTSC_WSS.u1F1_SET);           
                    vHwVbiSetWSSLine(u1VBISuite, VbiLineInfo.HDTV_NTSC_WSS.u1F0_SET, VbiLineInfo.HDTV_NTSC_WSS.u1F1_SET);
                }

            }
            else
            {
                /* CC_SEL */
                LOG(9, "[VBI] NTSC TVD CC line %d %d \n", VbiLineInfo.AV_NTSC_CC.u1F0_SET, VbiLineInfo.AV_NTSC_CC.u1F1_SET);                 
                vHwVbiSetCCLine(u1VBISuite, VbiLineInfo.AV_NTSC_CC.u1F0_SET, VbiLineInfo.AV_NTSC_CC.u1F1_SET);
                
                /* Add a condition for PAL-M case (480) */
                /* WSS_SEL */
                LOG(9, "[VBI] NTSC TVD WSS line %d %d \n", VbiLineInfo.AV_NTSC_WSS.u1F0_SET, VbiLineInfo.AV_NTSC_WSS.u1F1_SET);  
                vHwVbiSetWSSLine(u1VBISuite, VbiLineInfo.AV_NTSC_WSS.u1F0_SET, VbiLineInfo.AV_NTSC_WSS.u1F1_SET);

                #if SUPPORT_CLOSE_CAPTION_2X
                /* CC2X interval in field 0 & 1 */
                vHwVbiSetTTInterval0(u1VBISuite, VbiLineInfo.AV_NTSC_CC2X.u1F0_START, VbiLineInfo.AV_NTSC_CC2X.u1F0_END);
                vHwVbiSetTTInterval1(u1VBISuite, VbiLineInfo.AV_NTSC_CC2X.u1F1_START, VbiLineInfo.AV_NTSC_CC2X.u1F1_END);
                #endif 
            }
            break;
            
        case VDOTYPE_PAL:

            LOG(5, "VBI_ResetSrc: PAL mode \n");
            
            if (u1SrcType == (UINT8)SV_VD_YPBPR)
            {
                /* CC_SEL */
                LOG(9, "[VBI] PAL HDTV CC line %d %d \n", VbiLineInfo.HDTV_PAL_CC.u1F0_SET, VbiLineInfo.HDTV_PAL_CC.u1F1_SET);
                vHwVbiSetCCLine(u1VBISuite, VbiLineInfo.HDTV_PAL_CC.u1F0_SET, VbiLineInfo.HDTV_PAL_CC.u1F1_SET);
                
                /* WSS_SEL */
                if(fgIsPScanTiming())
                {
                    LOG(9, "[VBI] PAL PSCAN HDTV WSS line %d %d \n", VbiLineInfo.HDTV_PAL_WSS_PSAN.u1F0_SET, VbiLineInfo.HDTV_PAL_WSS_PSAN.u1F1_SET);
                    vHwVbiSetWSSLine(u1VBISuite, VbiLineInfo.HDTV_PAL_WSS_PSAN.u1F0_SET, VbiLineInfo.HDTV_PAL_WSS_PSAN.u1F1_SET);
                }
                else
                {
                    LOG(9, "[VBI] PAL HDTV WSS line %d %d \n", VbiLineInfo.HDTV_PAL_WSS.u1F0_SET, VbiLineInfo.HDTV_PAL_WSS.u1F1_SET);
                    vHwVbiSetWSSLine(u1VBISuite, VbiLineInfo.HDTV_PAL_WSS.u1F0_SET, VbiLineInfo.HDTV_PAL_WSS.u1F1_SET);
                }
                /* TTX interval in field 0 */
                LOG(9, "[VBI] PAL HDTV TT F0 line %d %d \n", VbiLineInfo.HDTV_PAL_TT.u1F0_START, VbiLineInfo.HDTV_PAL_TT.u1F0_END);
                vHwVbiSetTTInterval0(u1VBISuite, VbiLineInfo.HDTV_PAL_TT.u1F0_START, VbiLineInfo.HDTV_PAL_TT.u1F0_END);
                
                /* TTX interval in field 1 */
                LOG(9, "[VBI] PAL HDTV TT F1 line %d %d \n", VbiLineInfo.HDTV_PAL_TT.u1F1_START, VbiLineInfo.HDTV_PAL_TT.u1F1_END);
                vHwVbiSetTTInterval1(u1VBISuite, VbiLineInfo.HDTV_PAL_TT.u1F1_START, VbiLineInfo.HDTV_PAL_TT.u1F1_END);
            }
            else
            {
                /* CC_SEL */
                LOG(9, "[VBI] PAL TVD CC line %d %d \n", VbiLineInfo.AV_PAL_CC.u1F0_SET, VbiLineInfo.AV_PAL_CC.u1F1_SET);
                vHwVbiSetCCLine(u1VBISuite, VbiLineInfo.AV_PAL_CC.u1F0_SET, VbiLineInfo.AV_PAL_CC.u1F1_SET);
                
                /* WSS_SEL */
                LOG(9, "[VBI] PAL TVD CC line %d %d \n", VbiLineInfo.AV_PAL_WSS.u1F0_SET, VbiLineInfo.AV_PAL_WSS.u1F1_SET);
                vHwVbiSetWSSLine(u1VBISuite, VbiLineInfo.AV_PAL_WSS.u1F0_SET, VbiLineInfo.AV_PAL_WSS.u1F1_SET);
                
                /* TTX interval in field 0 */
                LOG(9, "[VBI] PAL TVD TT F0 line %d %d \n", VbiLineInfo.AV_PAL_TT.u1F0_START, VbiLineInfo.AV_PAL_TT.u1F0_END);
                vHwVbiSetTTInterval0(u1VBISuite, VbiLineInfo.AV_PAL_TT.u1F0_START, VbiLineInfo.AV_PAL_TT.u1F0_END);
                
                /* TTX interval in field 1 */
                LOG(9, "[VBI] PAL TVD TT F1 line %d %d \n", VbiLineInfo.AV_PAL_TT.u1F1_START, VbiLineInfo.AV_PAL_TT.u1F1_END);
                vHwVbiSetTTInterval1(u1VBISuite, VbiLineInfo.AV_PAL_TT.u1F1_START, VbiLineInfo.AV_PAL_TT.u1F1_END);
            }
            break;
            
        default:
            LOG(3, "Error Video Type [%d]\n", u1VideoType);
            break;
    }

    // ANAS Mode Setting. 
    if (_t_VbiMode == VBI_ANAS_MODE)
    {
        vHwVbiSetVPSInterval(VBI2, VbiLineInfo.AV_PAL_VPS.u1F0_SET , VbiLineInfo.AV_PAL_VPS.u1F0_SET);
    }

}


/**
 * @brief   Disable VBI
 *
 * @param   u1VBISuite: VBI0 / VBI2
 *
 * @retval  None
 */
static void _VBI_SetFIFO(VBI_MODE_T E_VBI_Mode, UINT32 u4FifoBase, UINT32 u4FifoSize) 
{

    if ((u4FifoBase ==0) || (u4FifoSize < VBI_FIFO_SIZE))
    {
        LOG(3, "Invalid VBI FIFO [Base,Size] [%x][%x]\n", u4FifoBase, u4FifoSize);
        //Printf("Invalid VBI FIFO [Base,Size] [%x][%x]\n", u4FifoBase, u4FifoSize);
        return;
    }

    #if SUPPORT_CLOSE_CAPTION_VCHIP
    if (E_VBI_Mode == VBI_CC_VCHIP_MODE)
    {
        _VBISetFifoPara(VBI0, 
                        u4FifoBase, 
                        (VBI_FIFO_SIZE/2/CC_PITCH), 
                        CC_FIFO);
        
        _VBIResetFifo(VBI0);
        
        _VBISetFifoPara(VBI2, 
                        (u4FifoBase + (VBI_FIFO_SIZE/2)), 
                        (VBI_FIFO_SIZE/2/CC_PITCH), 
                        CC_FIFO);
        
        _VBIResetFifo(VBI2);
    }
    #endif

    #if (SUPPORT_TELETEXT || SUPPORT_AUTO_NAMING)
    if ((E_VBI_Mode == VBI_TTX_MODE) || (E_VBI_Mode == VBI_ANAS_MODE))
    {
        _VBISetFifoPara(VBI0, 
                        (u4FifoBase),
                        (VBI_FIFO_SIZE/TTX_PITCH/2), 
                        TTX_FIFO);
        
        _VBIResetFifo(VBI0);
        
        _VBISetFifoPara(VBI2, 
                        (u4FifoBase + (VBI_FIFO_SIZE/2)), 
                        (VBI_FIFO_SIZE/2/TTX_PITCH), 
                        TTX_FIFO);
        
        _VBIResetFifo(VBI2);        
    }
    #endif

    #ifdef CC_ATV_PVR_SUPPORT
    _VBI_PVRSetFIFO(u4FifoBase, u4FifoSize);
    #endif

}

/**
 * @brief   CC Coch Patch
 *
 * @param   u1VBISuite: VBI0 / VBI2
 *
 * @retval  None
 */
static void _VBI_CC_Coch_Patch(UINT8 bVBISuite) 
{
    static BOOL fgPreCochStatus = FALSE; 
    static UINT8 u1CoChCnt = 0;
    
    if (fgPreCochStatus != fgHwTvdCoChannel())
    {
        vDrvVbiCoChCntRst(u1CoChCnt);
        fgPreCochStatus = fgHwTvdCoChannel();
    }

    if (fgHwTvdCoChannel())
    {
        vDrvVbiCoChCntInc(u1CoChCnt);
    }
    else
    {
        vDrvVbiCoChCntDec(u1CoChCnt);
    }

    if (u1CoChCnt == VBI_CC_COCH_MATCH)
    {
        vHwVbiSetCCInitSel(bVBISuite, 0);
    }
    else if (u1CoChCnt == VBI_CC_COCH_FAIL)
    {
        vHwVbiSetCCInitSel(bVBISuite, 1);
    }
}

#if AUTO_CC_DETECT 
/**
 * @brief     Reset VBI2 src related setting when AUTO_CC_DONE. (VBI_ISR)
 * @param     bPath: SV_VP_PIP
 * @retval    None
 */
static void VBIAutoCCResetSrc(UINT8 u1Path) 
{
    UINT8 u1VideoType;
    UINT8 u1Src;
    UINT8 u1MatchVBISuite = VBI2; /* u1Path is matched with which VBI slicer. VBI0 or VBI2 */
    UINT8 u1i = 0;

    /* Check Video Type and reset VBI setting. */
    u1VideoType = bDrvVideoGetType(u1Path);
    u1Src = bGetVideoDecType(SV_VP_PIP);

    
    LOG(5, "VBIAutoCCResetSrc: %d %d\n", u1Path, u1MatchVBISuite);

    if(u1MatchVBISuite == VBI2)
    {
        LOG(9, "[VBI]VBI2 Core Reset \n");
        vHwVbiCoreReset(VBI2);
    }       

    /*
    if(u1MatchVBISuite == VBI0)
    {
        if(_t_VbiMode == VBI_CC_VCHIP_MODE)
        {
            vHwVbiSetPEGain(VBI0, 1);
        }
        else
        {
            vHwVbiSetPEGain(VBI0, 0);
        }
    }
    else if(u1MatchVBISuite == VBI2)
    */
    if(u1MatchVBISuite == VBI2)
    {
        vHwVbiSetPEGain(VBI2, 2);
    }

    /* When doing auto-naming service, do not modify the driver setting. */
    if (_t_VbiMode == VBI_ANAS_MODE)
    {
        return;
    }

    /* Enable Main Window VBI FIFO */
    if (_u1TTXOnOff[SV_VP_MAIN])
    {
        _VBI_StartNotifyTTXOnOff(SV_VP_MAIN, TRUE, NULL);
    }

    /* Enable Sub Window VBI FIFO */
    if (_u1TTXOnOff[SV_VP_PIP])
    {
        _VBI_StartNotifyTTXOnOff(SV_VP_PIP, TRUE, NULL);
    }

    
    if (u1VideoType == VDOTYPE_PAL)
    {
        vHwVbiSetVbiMode(u1MatchVBISuite, VBI_PAL_MODE);
    }
    else
    {
        vHwVbiSetVbiMode(u1MatchVBISuite, VBI_NTSC_MODE);
    }

    /* Check Video Src */
    if (u1MatchVBISuite == VBI2)
    {
        #if SUPPORT_WSS
        _u4Wss2 = 0xffffffff;
        _u1Wss2TimeOutNo = 0;
        #endif

        #if SUPPORT_TELETEXT || SUPPORT_AUTO_NAMING
        for (u1i = 0; u1i < 2; u1i ++)
        {
            _u1Ttx2FailNo[u1i] = TTX_FAIL_THRES;
            _u1Ttx2SuccNo[u1i] = 0;
        }
        #endif        

        u1Src = bGetVideoDecType(_u1VBI2Path);
    
        /* Line Selection & Setting */
        _VBI_SetVBILines(&_VbiLineInfo, VBI2, u1Src, u1VideoType, _VBIAvInfo.E_AVFieldType);

        /* Set Initial Zero-Crossing threshold */
        vHwVbiSetInitZCThr(VBI2, HDTV_ZC_TH);
        vHwVbiSetClrPScan(VBI2, 0);

        if (u1Src == (UINT8)SV_VD_YPBPR)
        {
            /* Line Start Code */
            vHwVbiSetLSC(VBI2, HDTV_LSC);

            /* VSRC */
            vHwVbiSetSrc(VBI2, VBI_VSRC_HDTV);
            
            if(fgIsOversampleTiming()) /* determine is YPbPr oversampled */
            {
                /* Oversampled, divide frequency */
                vHwVbiHDTVSubSample(1);
            }
            else
            {
                /* No oversample, 27 MHz */
                vHwVbiHDTVSubSample(0);
            }
            /* Progressive Scan may send WSS, tested using BOK player */
            if(fgIsPScanTiming())
            {
                /* 1 LSC, HSAV half */
                /* 2 PScan set */
                /* 3 Freq half */
                /* 4 STC_ST half */
                /* 5 Line selection is by setting SEL0 and LSB of SEL1 */
                vHwVbiSetInitZCThr(VBI2, HDTV_PSCAN_ZC_TH);
                vHwVbiHDTVSubSample(0); /* Let VBI slicer work in 54 MHz */
                vHwVbiSetInitZC(VBI2, 0);

                vHwVbiSetClrPScan(VBI2, 1);
                if (u1VideoType == VDOTYPE_PAL)
                {
                    vHwVbiSetLSC(VBI2, HDTV_LSC);
                    vHwVbiSetWSSFreq(VBI2, 0x1CCD);
                }
                else
                {
                    vHwVbiSetLSC(VBI2, HDTV_PSCAN_480P_LSC);
                    vHwVbiSetWSSFreq(VBI2, 0x228E);
                }
            }
        }
        else
        {
            /* Line Start Code, measured via debug port as 48.8 us / 37 ns */
            if (u1VideoType == VDOTYPE_PAL)
            {
                vHwVbiSetLSC(VBI2, AV_EU_LSC);
                //_VBIAvInfo.E_AVSrcType = E_AV_PAL;
            }
            else //NTSC
            {
                vHwVbiSetLSC(VBI2, AV_US_LSC);
                //_VBIAvInfo.E_AVSrcType = E_AV_NTSC;
            }
            /* VSRC */
            vHwVbiSetSrc(VBI2, VBI_VSRC_TVD3D);
        }
    }    

    if (u1Src == (UINT8)SV_VD_TVD3D)
    {
        /* Check slicer and path matching */
        _VBIEnableBySlicer(u1MatchVBISuite);
    }
    else if (u1Src == (UINT8)SV_VD_YPBPR)
    {
        if(fgIsVBISupportTiming())
        {
            _VBIEnableBySlicer(u1MatchVBISuite);
        }
        else
        {
            LOG(3, "VBI_ResetSrc: Not Supported Timing \n");
            VBI_DisableBySlicer(u1MatchVBISuite);
        }
    }
}


static void VBIInitCCScan(void) 
{  
    if(!ui1AutoCCEnable)
        return;
    _VBIEnableBySlicer(VBI2);
    ui4ScanlineCCState = SCANLINE_INIT;
    x_memset(ui1Scanline_Field0_RdyCnt, 0, sizeof(UINT8)*(SCANLINE_CC_NUM));
    x_memset(ui1Scanline_Field1_RdyCnt, 0, sizeof(UINT8)*(SCANLINE_CC_NUM));
    ui1Scanline_TestCnt = 0;
    ui1CCLineIndex=0;
    if(bDrvVideoGetType(SV_VP_MAIN)==VDOTYPE_PAL)
    {
        rScanlineSeq_Normal[0].u1F0_SET = 18; rScanlineSeq_Normal[0].u1F1_SET = 19;//Field
        rScanlineSeq_Normal[1].u1F0_SET = 21; rScanlineSeq_Normal[1].u1F1_SET = 22;//Fluke
        rScanlineSeq_Normal[2].u1F0_SET = 22; rScanlineSeq_Normal[2].u1F1_SET = 23;//Tara
        rScanlineSeq_Normal[3].u1F0_SET = 16; rScanlineSeq_Normal[3].u1F1_SET = 17;//Dummy

        rScanlineSeq_Inverse[0].u1F0_SET = 19; rScanlineSeq_Inverse[0].u1F1_SET = 19;//Field
        rScanlineSeq_Inverse[1].u1F0_SET = 22; rScanlineSeq_Inverse[1].u1F1_SET = 22;//Fluke
        rScanlineSeq_Inverse[2].u1F0_SET = 23; rScanlineSeq_Inverse[2].u1F1_SET = 23;//Tara
        rScanlineSeq_Inverse[3].u1F0_SET = 17; rScanlineSeq_Inverse[3].u1F1_SET = 17;//Dummy
    }
    else
    {
        rScanlineSeq_Normal[0].u1F0_SET = 18; rScanlineSeq_Normal[0].u1F1_SET = 18;//Field
        rScanlineSeq_Normal[1].u1F0_SET = 17; rScanlineSeq_Normal[1].u1F1_SET = 17;
        rScanlineSeq_Normal[2].u1F0_SET = 16; rScanlineSeq_Normal[2].u1F1_SET = 16;//Field
        rScanlineSeq_Normal[3].u1F0_SET = 19; rScanlineSeq_Normal[3].u1F1_SET = 19;

        rScanlineSeq_Inverse[0].u1F0_SET = 19; rScanlineSeq_Inverse[0].u1F1_SET = 18;//Field
        rScanlineSeq_Inverse[1].u1F0_SET = 18; rScanlineSeq_Inverse[1].u1F1_SET = 17;
        rScanlineSeq_Inverse[2].u1F0_SET = 17; rScanlineSeq_Inverse[2].u1F1_SET = 16;//Field
        rScanlineSeq_Inverse[3].u1F0_SET = 20; rScanlineSeq_Inverse[3].u1F1_SET = 19;
    }
}

static void VBIAutoCCScan(UINT32 u4CurVbi2Field)
{  
    if(!ui1AutoCCEnable || !fgIsVdoStable(SV_VP_MAIN))
        return;
    if(ui4ScanlineCCState == SCANLINE_START && ui1CCLineIndex<SCANLINE_CC_NUM)
    {
        vHwVbiSetInitZCThr(VBI2, bHwVbiGetInitZCThr(VBI0));
        vHwVbiSetLSC(VBI2, AV_US_LSC);
        if (IsVBI2ISR() && (u4CurVbi2Field != WRONG_FIELD_ID) && _t_VbiMode == VBI_CC_VCHIP_MODE && IsCC2Enable()&& IsCC2Rdy())
        {
            if (u4CurVbi2Field == 0)
            {
                ui1Scanline_Field0_RdyCnt[ui1CCLineIndex]++;
            }
            else
            {
                ui1Scanline_Field1_RdyCnt[ui1CCLineIndex]++;
            }
        }
        if(IsVBI2ISR())
            ui1Scanline_TestCnt++;
        if(ui1Scanline_TestCnt==SCANLINE_CC_TESTLOOP)
        {
            LOG(3, "===== Line %d/%d =====\n", rCurrScanCCLine.u1F0_SET, rCurrScanCCLine.u1F1_SET);
            LOG(3, "Field0RdyCnt=%d\n", ui1Scanline_Field0_RdyCnt[ui1CCLineIndex]);
            LOG(3, "Field1RdyCnt=%d\n", ui1Scanline_Field1_RdyCnt[ui1CCLineIndex]);
            ui1Scanline_TestCnt=0;

            //CC Found
            if(ui1Scanline_Field0_RdyCnt[ui1CCLineIndex] >= SCANLINE_CC_TH || ui1Scanline_Field1_RdyCnt[ui1CCLineIndex] >= SCANLINE_CC_TH)
            {
                UINT8 ui1DefalutF0CCLineStart;
                UINT8 ui1DefalutF0CCLineEnd;
                UINT8 ui1DefalutF1CCLineStart;
                UINT8 ui1DefalutF1CCLineEnd;
                vHwVbiGetCCLine(VBI0, 0, &ui1DefalutF0CCLineStart, &ui1DefalutF0CCLineEnd);
                vHwVbiGetCCLine(VBI0, 1, &ui1DefalutF1CCLineStart, &ui1DefalutF1CCLineEnd);
                LOG(3, "[VBIAutoCCScan] Default Line: %d/%d\n", ui1DefalutF0CCLineStart, ui1DefalutF1CCLineStart);
                if(rCurrScanCCLine.u1F0_SET==ui1DefalutF0CCLineStart && rCurrScanCCLine.u1F1_SET==ui1DefalutF1CCLineStart)
                {    
                    ui4ScanlineCCState = SCANLINE_FINISH;
                }
                else
                {
                    if(ui1Scanline_SuccessIdx == ui1CCLineIndex)
                    {
                        ui1Scanline_SuccessCnt++;
                        if(ui1Scanline_SuccessCnt>=AUTO_CC_NOT_DEFULT_TH)
                        {
                            ui4ScanlineCCState = SCANLINE_FINISH;
                        }
                        else
                        {
                            ui4ScanlineCCState = SCANLINE_NOT_FOUND;
                        }
                    }
                    else
                    { 
                        ui1Scanline_SuccessIdx = ui1CCLineIndex;
                        ui1Scanline_SuccessCnt = 1;
                        ui4ScanlineCCState = SCANLINE_NOT_FOUND;
                    }
                    LOG(3, "[VBIAutoCCScan] SuccIndex=%d, Cnt=%d\n", ui1Scanline_SuccessIdx, ui1Scanline_SuccessCnt);
                }
            }
            else
            {
                if(ui1CCLineIndex==SCANLINE_CC_NUM-1)
                {
                    ui4ScanlineCCState = SCANLINE_NOT_FOUND;
                }
                else
                {
                    ui1CCLineIndex++;
                    if(ui1CCLineIndex < SCANLINE_CC_NUM)
                    {
                        if(_VBIAvInfo.E_AVFieldType == E_FIELD_NORMAL)
                        {
                            rCurrScanCCLine = rScanlineSeq_Normal[ui1CCLineIndex];
                        }
                        else
                        {
                            rCurrScanCCLine = rScanlineSeq_Inverse[ui1CCLineIndex];
                        }
                        vHwVbiSetCCLine(VBI2, rCurrScanCCLine.u1F0_SET, rCurrScanCCLine.u1F1_SET);
                    }
                }
            }
        }
    }
    else if(ui4ScanlineCCState == SCANLINE_FINISH)
    {
        LOG(3, "[VBIAutoCCScan] CC Line %d/%d Found\n", rCurrScanCCLine.u1F0_SET, rCurrScanCCLine.u1F1_SET);
        ui4ScanlineCCState = SCANLINE_IDLE;
        if(bGetVideoDecType(SV_VP_MAIN) == (UINT8)SV_VD_TVD3D)
        {
            if(_VBIAvInfo.E_AVFieldType == E_FIELD_INVERSED)
            {
                if(bDrvVideoGetType(_u1VBI0Path)==VDOTYPE_NTSC)
                {
                    rCurrScanCCLine.u1F0_SET  -= (AV_IVS_NTSC_CC_SEL0  - AV_NML_NTSC_CC_SEL0);
                    rCurrScanCCLine.u1F1_SET  -= (AV_IVS_NTSC_CC_SEL1  - AV_NML_NTSC_CC_SEL1);
                }
                else if(bDrvVideoGetType(_u1VBI0Path)==VDOTYPE_PAL)
                {
                    rCurrScanCCLine.u1F0_SET   -= (AV_IVS_PAL_CC_SEL0   - AV_NML_PAL_CC_SEL0);
                    rCurrScanCCLine.u1F1_SET   -= (AV_IVS_PAL_CC_SEL1   - AV_NML_PAL_CC_SEL1);                  
                }
            }
            if(bDrvVideoGetType(_u1VBI0Path)==VDOTYPE_NTSC)
            {
                _VbiLineInfo.AV_NTSC_CC = rCurrScanCCLine;
            }
            else if(bDrvVideoGetType(_u1VBI0Path)==VDOTYPE_PAL)
            {
                _VbiLineInfo.AV_PAL_CC = rCurrScanCCLine;
            }
            _VBI_SetVBILines(&_VbiLineInfo, VBI0, SV_VD_TVD3D, _VBIAvInfo.E_AVSrcType, _VBIAvInfo.E_AVFieldType);
            VBIAutoCCResetSrc(SV_VP_PIP);
        }
        
    }
    else if(ui4ScanlineCCState == SCANLINE_NOT_FOUND)
    {
        ui1Scanline_RetryCnt++;
        if(ui1Scanline_RetryCnt<SCANLINE_CC_RETRY)
        {
            VBIInitCCScan();
            if(_VBIAvInfo.E_AVFieldType == E_FIELD_NORMAL)
            {
                rCurrScanCCLine = rScanlineSeq_Normal[0];
            }
            else
            {
                rCurrScanCCLine = rScanlineSeq_Inverse[0];
            }
            vHwVbiSetCCLine(VBI2, rCurrScanCCLine.u1F0_SET, rCurrScanCCLine.u1F1_SET);
            
            ui4ScanlineCCState = SCANLINE_START;
            LOG(3, "Scan Line Retry\n");        
        }
        else
        {
            LOG(3, "[VBIAutoCCScan] CC Line Not Found !!\n");
            ui4ScanlineCCState = SCANLINE_IDLE;
            if(bGetVideoDecType(SV_VP_MAIN) == (UINT8)SV_VD_TVD3D)
            {
                VBIAutoCCResetSrc(SV_VP_PIP);
            }
        }
    }
}

#endif



/**************************************************************************
 * Global Functions
 *************************************************************************/
#ifdef CC_FAST_INIT
void VBI_pm_suspend(void)
{
    LOG(3, "VBI suspend\n");
    _u4VbiInitOnce=0;
}

void VBI_pm_resume(void)
{
    LOG(3, "VBI resume\n");
    VBI_Init(_t_VbiMode);
    VBI_Enable();
}
#endif

/**
 * @brief   Enable VBI
 *
 * @param   u1VBISuite: VBI0 / VBI2
 *
 * @retval  None
 */
void VBI_EnableBySlicer(UINT8 u1VBISuite) 
{
    _VBIEnableBySlicer(u1VBISuite);
}

/**
 * @brief   Disable VBI
 *
 * @param   u1VBISuite: VBI0 / VBI2
 *
 * @retval  None
 */
void VBI_DisableBySlicer(UINT8 u1VBISuite) 
{
    #if SUPPORT_AUTO_NAMING
    if (_t_VbiMode == VBI_ANAS_MODE)
    {
        return;
    }
    #endif
    
    if (u1VBISuite == VBI0)
    {
        #if !IC_VERIFY
        vWriteCLKMsk(C_CLK_CFG_L, VBI_CLK_STOP, VBI_CLK_STOP);
        vWriteCLKMsk(MCLK_EN_2, 0, VBI_MCLK_EN);
        #endif
        vHwVbiSetLineEnable(VBI0, 0);
    }
    else
    {
        #if !IC_VERIFY
        vWriteCLKMsk(C_CLK_CFG_L, VBI2_CLK_STOP, VBI2_CLK_STOP);    
        vWriteCLKMsk(MCLK_EN_2, 0, VBI2_MCLK_EN);        
        #endif
        vHwVbiSetLineEnable(VBI2, 0);
    }
}


/**
 * @brief   Initialize VBI driver.
 *
 * @param   bVbiMode: VBI_CC_VCHIP_MODE / VBI_WSS_ONLY_MODE / VBI_TTX_MODE / VBI_ANAS_MODE
 *                    VBI_CC_VCHIP_MODE - for CC/V-Chip + WSS Auto application
 *                    VBI_WSS_ONLY_MODE - for no CC/TT only WSS Auto application
 *                    VBI_TTX_MODE - for TTX display + WSS auto application
 *                    VBI_ANAS_MODE - for euro auto-naming/sorting service.
 *
 * @retval  None
 */
void VBI_Init(VBI_MODE_T t_VbiMode) 
{
    UINT8 u1i = 0;
    FBM_POOL_T*  prFbmPool;

    LOG( 9,"[VBI]Run VBI_Init() ...\n");
        
    #if !IC_VERIFY
    /* Enable VBI0 and VBI2 Clock */
    vWriteCLKMsk(C_CLK_CFG_L, 0x00, VBI_CLK_STOP|VBI2_CLK_STOP);
    vWriteCLKOr(MCLK_EN_2, VBI_MCLK_EN|VBI2_MCLK_EN);
    #endif

    //Create Samefore protection. 
    if (_hVbiMutex == 0)
    {
        // create semaphore
        VERIFY(x_sema_create(&_hVbiMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }    

    //Disable ALL VBI Serivice.
    vHwVbiSetLineEnable(VBI0, 0);
    vHwVbiSetLineEnable(VBI2, 0);

    _t_VbiMode = t_VbiMode;

    vHwVbiHVPosAdjust(1);

    vRegWriteFldAlign(VBI_01, 0, VBI_ALL_ADAPTIVE);
    vRegWriteFldAlign(VBI2_01, 0, VBI2_ALL_ADAPTIVE);

    switch (_t_VbiMode)
    {
        #if (SUPPORT_CLOSE_CAPTION_VCHIP)
        case VBI_CC_VCHIP_MODE:
            LOG(5,"[VBI]Set VBI to CC_VCHIP mode. \n");            
            vHwVbiSetSrc(VBI0, VBI_VSRC_TVD3D);
            vHwVbiSetSrc(VBI2, VBI_VSRC_TVD3D);
            vHwVbiSetSrc(VBI2, VBI_VSRC_HDTV);
            vHwVbiSetVbiMode(VBI0, VBI_NTSC_MODE);
            vHwVbiSetVbiMode(VBI2, VBI_NTSC_MODE);
            break;
        #endif // #if (SUPPORT_CLOSE_CAPTION_VCHIP)
            
        #if SUPPORT_TELETEXT
        case VBI_TTX_MODE:
            LOG(5,"[VBI]Set VBI to TTX mode. \n");            
            vHwVbiSetSrc(VBI0, VBI_VSRC_TVD3D);
            vHwVbiSetSrc(VBI2, VBI_VSRC_TVD3D);
            vHwVbiSetSrc(VBI2, VBI_VSRC_HDTV);
            vHwVbiSetVbiMode(VBI0, VBI_PAL_MODE);
            vHwVbiSetVbiMode(VBI2, VBI_PAL_MODE);
            break;
        #endif // #if SUPPORT_TELETEXT

        #if SUPPORT_AUTO_NAMING
        /* VBI0 decodes TTX+WSS, VBI2 decodes VPS+WSS */
        case VBI_ANAS_MODE:
            LOG(5, "[VBI]Set VBI to ANAS mode. \n");
            vHwVbiSetSrc(VBI0, VBI_VSRC_TVD3D);
            vHwVbiSetSrc(VBI2, VBI_VSRC_TVD3D);
            vHwVbiSetVbiMode(VBI0, VBI_CNI_MODE);
            vHwVbiSetVbiMode(VBI2, VBI_CNI_MODE);
            break;
        #endif // #if SUPPORT_AUTO_NAMING

        #if (SUPPORT_WSS &&             \
            !SUPPORT_CLOSE_CAPTION_VCHIP &&   \
            !SUPPORT_TELETEXT &&        \
            !SUPPORT_AUTO_NAMING)
        case VBI_WSS_ONLY_MODE:
            vHwVbiSetSrc(VBI0, VBI_VSRC_TVD3D);
            vHwVbiSetSrc(VBI2, VBI_VSRC_TVD3D);
            vHwVbiSetSrc(VBI2, VBI_VSRC_HDTV);
            vHwVbiSetVbiMode(VBI0, VBI_NTSC_MODE);
            vHwVbiSetVbiMode(VBI2, VBI_NTSC_MODE);
            break;    
        #endif            
        default:
            break;
    }

    /* Initialize static variables */
   
    /* for FIFO Management */
    _u1Vbi0PitchSz = 0;
    _u4Vbi0FifoSa = 0;
    _u4Vbi0FifoSz = 0;
    _u4NewVbi0WritePtr = 0;
    _u4Vbi0ReadPtr = 0;
    
    _u1Vbi2PitchSz = 0;
    _u4Vbi2FifoSa = 0;
    _u4Vbi2FifoSz = 0;
    _u4NewVbi2WritePtr = 0;
    _u4Vbi2ReadPtr = 0;

    #if SUPPORT_AUTO_NAMING
    _u1Vps2FailNo = VPS_FAIL_THRES;
    _u1Vps2SuccNo = 0;    
    #endif

    #if SUPPORT_CLOSE_CAPTION_VCHIP
    _u1CCFailNo[0] = 0;
    _u1CCFailNo[1] = 0;
    _u1CC2FailNo[0] = 0;
    _u1CC2FailNo[1] = 0;    
    #endif
    
    #if SUPPORT_CLOSE_CAPTION_VCHIP
    #if CC_VCR_TRICK_CHECK
    _u1TrickCheckOn = 1;
    _u4TrickPCounter = 0;
    _u4TrickNCounter = 0;
    _u1TrickBlock = SV_OFF;
    #endif
    #endif

    #if (SUPPORT_TELETEXT || SUPPORT_AUTO_NAMING)
    for (u1i = 0; u1i <2; u1i++)
    {
        _u1TtxFailNo[u1i] = TTX_FAIL_THRES; 
        _u1TtxSuccNo[u1i] = 0; 
        _u1Ttx2FailNo[u1i] = TTX_FAIL_THRES;
        _u1Ttx2SuccNo[u1i] = 0;
    }
    #endif

    #if SUPPORT_WSS
    _u4Wss = 0xffffffff;
    _u4WssPrev = 0;
    _u1WssFailNo = 0;
    _u4Wss2 = 0xffffffff;
    _u4Wss2Prev = 0;
    _u1Wss2FailNo = 0;
    #endif

    _u4Vbi0ReadPtr = 0;
    _u4Vbi2ReadPtr = 0;
    _u4TTXPktAvail = 0;

    _fgVbiFifoOn = FALSE;
    _fgVbi2FifoOn = FALSE;

    _u1VbiCCMainNotify = SV_OFF;
    _u1VbiCCSubNotify = SV_OFF;
    _u1VbiCC2XMainNotify = SV_OFF;
    _u1VbiCC2XSubNotify = SV_OFF;    
    _u1VbiTTXMainNotify = SV_OFF;
    _u1VbiTTXSubNotify = SV_OFF;
    _u1VbiEUWSSMainNotify = SV_OFF;
    _u1VbiEUWSSSubNotify = SV_OFF;

    /* US WSS525 */
    _u1VbiUSWSSMainNotify = SV_OFF;
    _u1VbiUSWSSSubNotify = SV_OFF;

    _u1VbiVPSCNINotify = SV_OFF;

    /* Set VBI Fifo Address */
    prFbmPool = FBM_GetPoolInfoAuto((UINT8)FBM_POOL_TYPE_VBI, NULL);
    if (prFbmPool)
    {
        _VBI_SetFIFO(_t_VbiMode, prFbmPool->u4Addr, prFbmPool->u4Size); 
        //Printf("VBI_ResetFifo: [Base, Size] [%x, %x]\n", prFbmPool->u4Addr, prFbmPool->u4Size);
        LOG(3, "VBI_ResetFifo: [Base, Size] [%x, %x]\n", prFbmPool->u4Addr, prFbmPool->u4Size);
    }
        
    if (_u4VbiInitOnce == 0)
    {
        LOG(3, "VBi Init Once\n");
        vIO32WriteFldAlign(CKGEN_VPCLK_STOP, 0, FLD_VBI_CLK_STOP);               
        vIO32WriteFldAlign(CKGEN_VPCLK_STOP, 0, FLD_VBI2_CLK_STOP);               
        _u1VBI0Path = SV_VP_MAIN;
        _u1VBI2Path = SV_VP_PIP;
        _u4VbiInitOnce = 1;
    }
}

/**
 * @brief   VBI Interrupt service routine
 *
 * @param   None
 *
 * @retval  None
 */
//VOID static VBI_ISR(UINT16 u2Vector)
void VBI_ISR(void) 
{
    UINT32 u4CurVbi0Field;
    UINT32 u4CurVbi2Field;
    HAL_TIME_T rCurTime, rDeltaTime;
    UINT32 u4MilliSec;

    //ASSERT(u2Vector == VECTOR_VBI);
    if((!IsVBI0ISR()) && (!IsVBI2ISR()))
        return;
    
    if(_fgVbiFifoRstDone == FALSE)
    {
        HAL_GetTime(&rCurTime);
        HAL_GetDeltaTime(&rDeltaTime, &_rVbiFifoRstTime, &rCurTime);
        u4MilliSec = (rDeltaTime.u4Seconds * 1000) + (rDeltaTime.u4Micros / 1000);
        if(u4MilliSec >= VBI_RST_FIFO_WAITING_TIME)
        {
            _fgVbiFifoRstDone = TRUE;
        }
        if(_fgVbiFifoRstDone == FALSE)
        {
            LOG(3, "VBI_ISR() wait for FIFO RST DONE(%d/%d ms)\n", u4MilliSec, VBI_RST_FIFO_WAITING_TIME);
            return;
        }
    }

    /* Get current field from HW VBI */
    u4CurVbi0Field = _VBI_GetCurField(VBI0); 
    u4CurVbi2Field = _VBI_GetCurField(VBI2);  

    /* VBI0 Interrupt */
    if (IsVBI0ISR() && (u4CurVbi0Field != WRONG_FIELD_ID))
    {
        _u4NewVbi0WritePtr = u4HwVbiReadWritePtr(VBI0);
        
        /* VBI0 Fifo Mode write pointer error check */
        if (_fgVbiFifoOn && (((_u4NewVbi0WritePtr - _u4Vbi0FifoSa) % _u1Vbi0PitchSz) != 0))
        {
            if(_fgDramDumpEn==FALSE)
            {
                _VBIResetFifo(VBI0);
                vHwVbiClrResetFifo(VBI0);
                return;
            }
        }

        LOG(7, "------VBI ISR [ReadAddr, WriteAddr]=[%x,%x]\n",_u4Vbi0ReadPtr,_u4NewVbi0WritePtr); 

        
    #if (SUPPORT_CLOSE_CAPTION_VCHIP)
        #if CC_VCR_TRICK_CHECK
        if (_t_VbiMode == VBI_CC_VCHIP_MODE)
        {
            _VBI_TrickValidate();
        }
        #endif
    #endif
    
        /* VBI0: CC, V-Chip */
    #if (SUPPORT_CLOSE_CAPTION_VCHIP)
        if (_t_VbiMode == VBI_CC_VCHIP_MODE)
        {
            _VBI_CC_Coch_Patch(VBI0);
            
            if (IsCCEnable() && IsCCRdy())
            {
                if ((fgIsVdoStable(SV_VP_MAIN) && 
                    (_u1VbiCCMainNotify == SV_ON) && 
                    (_u1VBI0Path == SV_VP_MAIN)) 
                    ||
                    (fgIsVdoStable(SV_VP_PIP) && 
                    (_u1VbiCCSubNotify == SV_ON) && 
                    (_u1VBI0Path == SV_VP_PIP)))
                {  
                    #if CC_VCR_TRICK_CHECK
                    if(_u1TrickBlock == SV_OFF)
                    #endif
                    {
                        _VBINotify(VBI_CC_SERVICE, _u1VBI0Path, u1GetCC_B0(), u1GetCC_B1(), u4CurVbi0Field);
                    }
                    #if SUPPORT_TOFU_DISPLAY
                    if(_rTvd3dStatus.eSourceType==SV_ST_TV && bTvdSignalStrengthLevel(1)<50)
                    {
                        vDrvVbiIncVBI0CCRatio(u4CurVbi0Field);
                        LOG(3, "[VBI] VBI0 CCRdyRatio = %d , Notify CC\n", _u1VBI0CCRdyRatio[u4CurVbi0Field]);
                        if (_bVBI0StartTofoDisplay[u4CurVbi0Field]==FALSE && _u1VBI0CCRdyRatio[u4CurVbi0Field]>_u1TofuHiTh)
                        {
                            LOG(3, "[VBI] VBI0 START to display ToFu Field=%d\n", u4CurVbi0Field);
                            if(_u1CCFailNo[u4CurVbi0Field] > _CCTOutInfo.u1ThresHold)
                                LOG(3, "[VBI] VBI0 _u1CCFailNo[u4CurVbi0Field]=%d, _CCTOutInfo.u1ThresHold=%d\n", _u1CCFailNo[u4CurVbi0Field], _CCTOutInfo.u1ThresHold);
                            _bVBI0StartTofoDisplay[u4CurVbi0Field] = TRUE;
                        }
                    }
                    #endif
                } 
                //Count CC Fail Number
                _u1CCFailNo[u4CurVbi0Field] = 0;
                #ifdef CC_ATV_PVR_SUPPORT
                if(_u1VbiPvrNotify==TRUE)
                {
                    _VBI_NotifyPVRCCData(_u1VBI0Path, u1GetCC_B0(), u1GetCC_B1(), u4CurVbi0Field);
                }
                #endif
            }
            else
            {
                /* Count NO CC data. */
                #if SUPPORT_TOFU_DISPLAY
                if ((fgIsVdoStable(SV_VP_MAIN) && 
                    (_u1VbiCCMainNotify == SV_ON) && 
                    (_u1VBI0Path == SV_VP_MAIN)) 
                    ||
                    (fgIsVdoStable(SV_VP_PIP) && 
                    (_u1VbiCCSubNotify == SV_ON) && 
                    (_u1VBI0Path == SV_VP_PIP)))
                {
                    if(_rTvd3dStatus.eSourceType==SV_ST_TV && bTvdSignalStrengthLevel(1)<50)
                    {
                        vDrvVbiDecVBI0CCRatio(u4CurVbi0Field);
                        if (_bVBI0StartTofoDisplay[u4CurVbi0Field]==TRUE && _u1VBI0CCRdyRatio[u4CurVbi0Field]<_u1TofuLoTh)
                        {
                            LOG(3, "[VBI] VBI0 STOP to display ToFu Field=%d\n", u4CurVbi0Field);
                            _bVBI0StartTofoDisplay[u4CurVbi0Field] = FALSE;
                        }
                        
                        if (_bVBI0StartTofoDisplay[u4CurVbi0Field] && _bEnableTofuDisplay)
                        {
                            LOG(3, "[VBI] VBI0 CCRdyRatio = %d , Notify ToFu\n", _u1VBI0CCRdyRatio[u4CurVbi0Field]);
                            _VBINotify(VBI_CC_SERVICE, _u1VBI0Path, CC_TOfU_DATA, CC_TOfU_DATA, u4CurVbi0Field);
                        }      
                    }
                }
                #else
                if (_u1CCFailNo[u4CurVbi0Field] < _CCTOutInfo.u1ThresHold)
                {
                    _u1CCFailNo[u4CurVbi0Field]++;
                }
                else if (_CCTOutInfo.bEnCCTimeOut) //Notify No CC data.
                {
                    _u1CCFailNo[u4CurVbi0Field] = 0;
                    _VBINotify(VBI_CC_SERVICE, _u1VBI0Path, _CCTOutInfo.u1NoCCData0, _CCTOutInfo.u1NoCCData1, u4CurVbi0Field);
                }
                #endif
                #ifdef CC_ATV_PVR_SUPPORT
                if(_u1VbiPvrNotify==TRUE)
                {
                    _VBI_NotifyPVRCCData(_u1VBI0Path, _CCTOutInfo.u1NoCCData0, _CCTOutInfo.u1NoCCData1, u4CurVbi0Field);
                }
                #endif
            }
        }
        
    #endif // #if (SUPPORT_CLOSE_CAPTION_VCHIP)

        /* VBI0: CC2X */
    #if SUPPORT_CLOSE_CAPTION_2X
        if (_t_VbiMode == VBI_CC_VCHIP_MODE)
        {
            if (IsCC2XEnable() && IsCC2XRdy())
            {
                if (_u1CC2XSuccNo < CC2X_SUCC_THRES)
                {
                    _u1CC2XSuccNo++;
                }
                else
                {
                    if ((fgIsVdoStable(SV_VP_MAIN) &&
                        (_u1VbiCC2XMainNotify == SV_ON) && 
                        (_u1VBI0Path == SV_VP_MAIN))                     
                        ||
                        (fgIsVdoStable(SV_VP_PIP) &&
                        (_u1VbiCC2XSubNotify == SV_ON) && 
                        (_u1VBI0Path == SV_VP_PIP)))
                    {
                        if (_u4NewVbi0WritePtr > _u4Vbi0ReadPtr)
                        {
                            _u4CC2XPktAvail = (_u4NewVbi0WritePtr-_u4Vbi0ReadPtr)/_u1Vbi0PitchSz;            
                            _VBINotify(VBI_CC2X_SERVICE, _u1VBI0Path, _u4CC2XPktAvail, _u4Vbi0ReadPtr, NULL);

                        }
                        else if(_u4NewVbi0WritePtr < _u4Vbi0ReadPtr)
                        {
                            _u4CC2XPktAvail = ((_u4Vbi0FifoSa + _u4Vbi0FifoSz) - _u4Vbi0ReadPtr)/_u1Vbi0PitchSz;
                            _VBINotify(VBI_CC2X_SERVICE, _u1VBI0Path, _u4CC2XPktAvail, _u4Vbi0ReadPtr, NULL);

                            _u4CC2XPktAvail = (_u4NewVbi0WritePtr -_u4Vbi0FifoSa)/_u1Vbi0PitchSz;
                            _VBINotify(VBI_CC2X_SERVICE, _u1VBI0Path, _u4CC2XPktAvail, _u4Vbi0FifoSa, NULL);
                        }
                    }
                    _u1CC2XFailNo = 0;
                }
                /* Move forward Read pointer */
                _u4Vbi0ReadPtr = _u4NewVbi0WritePtr;
            }
            else
            {
                if (_u1CC2XFailNo < CC2X_FAIL_THRES)
                {
                    _u1CC2XFailNo++;
                }
                else
                {
                    _u1CC2XSuccNo = 0;             
                }
            }
        }     
    #endif // #if SUPPORT_CLOSE_CAPTION_2X

        /* VBI0: TTX */
    #if (SUPPORT_TELETEXT || SUPPORT_AUTO_NAMING)
        if ((_t_VbiMode == VBI_TTX_MODE) || (_t_VbiMode == VBI_ANAS_MODE))
        {
            if (IsTTRdy())
            {
                // Successful when one field have TTX data > TTX_SUCC_THRES
                if ((_u1TtxSuccNo[0] < TTX_SUCC_THRES) && (_u1TtxSuccNo[1] < TTX_SUCC_THRES))
                {
                    _u1TtxSuccNo[u4CurVbi0Field]++;
                }
                else
                {
                    if ((fgIsVdoStable(SV_VP_MAIN) &&
                        (_u1VbiTTXMainNotify == SV_ON) && 
                        (_u1VBI0Path == SV_VP_MAIN))                     
                        ||
                        (fgIsVdoStable(SV_VP_PIP) &&
                        (_u1VbiTTXSubNotify == SV_ON) && 
                        (_u1VBI0Path == SV_VP_PIP)))
                    {
#ifdef CC_ATV_PVR_SUPPORT
                        ASSERT(_u1Vbi0PitchSz == 48);
#endif
                        if(_u4NewVbi0WritePtr > _u4Vbi0ReadPtr)
                        {
                            _u4TTXPktAvail = (_u4NewVbi0WritePtr-_u4Vbi0ReadPtr)/_u1Vbi0PitchSz;            
#ifdef CC_ATV_PVR_SUPPORT
                            ASSERT((_u4NewVbi0WritePtr-_u4Vbi0ReadPtr) % _u1Vbi0PitchSz == 0);
#endif
                            _VBINotify(VBI_TTX_SERVICE, _u1VBI0Path, _u4TTXPktAvail, _u4Vbi0ReadPtr, 0);
                        }
                        else if(_u4NewVbi0WritePtr < _u4Vbi0ReadPtr)
                        {
                            _u4TTXPktAvail = ((_u4Vbi0FifoSa + _u4Vbi0FifoSz) -_u4Vbi0ReadPtr)/_u1Vbi0PitchSz;
#ifdef CC_ATV_PVR_SUPPORT
                            ASSERT(((_u4Vbi0FifoSa + _u4Vbi0FifoSz) -_u4Vbi0ReadPtr) % _u1Vbi0PitchSz == 0);
#endif
                            _VBINotify(VBI_TTX_SERVICE, _u1VBI0Path, _u4TTXPktAvail, _u4Vbi0ReadPtr, 0);

                            _u4TTXPktAvail = (_u4NewVbi0WritePtr -_u4Vbi0FifoSa)/_u1Vbi0PitchSz;
#ifdef CC_ATV_PVR_SUPPORT
                            ASSERT((_u4NewVbi0WritePtr -_u4Vbi0FifoSa) % _u1Vbi0PitchSz == 0);
#endif
                            _VBINotify(VBI_TTX_SERVICE, _u1VBI0Path, _u4TTXPktAvail, _u4Vbi0FifoSa, 0);
                        }
                    }
                    _u1TtxFailNo[u4CurVbi0Field] = 0;
                }
                /* Move forward Read pointer */
                _u4Vbi0ReadPtr = _u4NewVbi0WritePtr;
            }
            else
            {
                if (_u1TtxFailNo[u4CurVbi0Field] < TTX_FAIL_THRES)
                {
                    _u1TtxFailNo[u4CurVbi0Field]++;
                }
                else
                {
                    _u1TtxSuccNo[u4CurVbi0Field] = 0;
                    #if SUPPORT_AUTO_NAMING                
                    if (_t_VbiMode == VBI_ANAS_MODE)
                    {
                        /* VPres is down => Flush TTX data */
                        _u4Vbi0ReadPtr = _u4NewVbi0WritePtr;
                    }
                    #endif                
                }
            }
        }
        
    #endif // #if (SUPPORT_TELETEXT || SUPPORT_AUTO_NAMING)

        /* VBI0: WSS */
    #if SUPPORT_WSS
        #if WSS_DUAL_SLICER
        _u1VideoType[0] = bDrvVideoGetType(_u1VBI0Path);
        #endif
        
        if (IsWSSEnable() && IsWSSRdy())
        {
            _u4Wss = _VBI_GetWSSData(_u1VBI0Path, VBI0);                

            /* WSS debounce counter */
            if (_u4Wss != _u4WssPrev)
                _u1WssSuccNo = 0;
            else if (_u1WssSuccNo < MAX_SUCC_THRES)
                _u1WssSuccNo++;

            //Printf("VBI_ISR: VBI0 wss 0x%X, Field %d.\n", _u4Wss, u4CurVbi0Field);
                
            //EU WSS625 
            #if WSS_DUAL_SLICER
            if (_u1VideoType[0] == VDOTYPE_PAL)
            #else
            if((_t_VbiMode == VBI_TTX_MODE)||(_t_VbiMode == VBI_ANAS_MODE))
            #endif
            {
                
                if (_VBI_WSSValidityCheck(  _u1VBI0Path, 
                                            u4CurVbi0Field, 
                                            _u4Wss, 
                                            _u1VbiEUWSSMainNotify, 
                                            _u1VbiEUWSSSubNotify, 
                                            _u1WssSuccNo) == WSS625_Pass)
                {
                    _VBINotify(VBI_WSS625_SERVICE, _u1VBI0Path, _u4Wss, 0, 0);
                }                        
            }
            //US WSS525 
            #if WSS_DUAL_SLICER
            else if ((_u1VideoType[0]) == VDOTYPE_NTSC)
            #else
            else if (_t_VbiMode == VBI_CC_VCHIP_MODE)
            #endif
        	{
                if (_VBI_WSSValidityCheck(  _u1VBI0Path, 
                                            u4CurVbi0Field, 
                                            _u4Wss, 
                                            _u1VbiUSWSSMainNotify, 
                                            _u1VbiUSWSSSubNotify, 
                                            _u1WssSuccNo) == WSS525_Pass)
                {
                    _VBINotify(VBI_WSS525_SERVICE, _u1VBI0Path, _u4Wss, 0, 0);
                }

            }
            
            _u4WssPrev = _u4Wss;
            _u1WssFailNo = 0;
        }
        else
        {
            if(fgHwTvdVPresTVD3D())
            {
                UINT8 u1Thres;
                u1Thres = ((_t_VbiMode == VBI_TTX_MODE)||(_t_VbiMode == VBI_ANAS_MODE)) ? WSS625_FAIL_THRES : WSS525_FAIL_THRES;
                _u1WssFailNo = (_u1WssFailNo < u1Thres) ? (_u1WssFailNo+1) : _u1WssFailNo; 
                _u4Wss = (_u1WssFailNo >= u1Thres) ? _WSSTOutInfo.u4NoWSSData : _u4Wss;
                _u1WssSuccNo = (_u1WssFailNo >= u1Thres) ? 0 : _u1WssSuccNo;
    
                _u1WssTimeOutNo = ((_u4Wss == _WSSTOutInfo.u4NoWSSData) && (_u1WssTimeOutNo < _WSSTOutInfo.u1ThresHold)) ? (_u1WssTimeOutNo+1) : 0;
                
                if (_u1WssTimeOutNo >=  _WSSTOutInfo.u1ThresHold)
                {
                    #if WSS_DUAL_SLICER
                    if ((_u1VideoType[0] == VDOTYPE_PAL)&& (_WSSTOutInfo.bEnWSS625TOut))
                    #else
                    if(((_t_VbiMode == VBI_TTX_MODE)||(_t_VbiMode == VBI_ANAS_MODE)) && (_WSSTOutInfo.bEnWSS625TOut))
                    #endif
                    {
                        if (((_u1VbiEUWSSMainNotify == SV_ON) && 
                            (_u1VBI0Path == SV_VP_MAIN)) 
                            ||
                            ((_u1VbiEUWSSSubNotify == SV_ON) && 
                            (_u1VBI0Path == SV_VP_PIP)))
                        {
                            _VBINotify(VBI_WSS625_SERVICE, _u1VBI0Path, _u4Wss, 0, 0);                
                        }
                    }
                    #if WSS_DUAL_SLICER
                    else if ((_u1VideoType[0] == VDOTYPE_NTSC) && (_WSSTOutInfo.bEnWSS525TOut))
                    #else
                    else if ((_t_VbiMode == VBI_CC_VCHIP_MODE) && (_WSSTOutInfo.bEnWSS525TOut))
                    #endif
                    {
                        if (((_u1VbiUSWSSMainNotify == SV_ON) && 
                            (_u1VBI0Path == SV_VP_MAIN))
                            ||
                            ((_u1VbiUSWSSSubNotify == SV_ON) && 
                            (_u1VBI0Path == SV_VP_PIP)))
                        {
                            _VBINotify(VBI_WSS525_SERVICE, _u1VBI0Path, _u4Wss, 0, 0);
                        }
                    }
                }
            }
        }

    #endif // #if SUPPORT_WSS
    }
        
    /* VBI0 ISR End */

  /*======================================================================
     VBI2 Interrupt
    =======================================================================*/
#if (SUPPORT_CLOSE_CAPTION_VCHIP)
#if AUTO_CC_DETECT
    VBIAutoCCScan(u4CurVbi2Field);
#endif
#endif

    if (IsVBI2ISR() && (u4CurVbi2Field != WRONG_FIELD_ID))
    {
        _u4NewVbi2WritePtr = u4HwVbiReadWritePtr(VBI2);
        
        /* VBI0 Fifo Mode write pointer error check */
        if (_fgVbi2FifoOn && (((_u4NewVbi2WritePtr - _u4Vbi2FifoSa) % _u1Vbi2PitchSz) != 0))
        {
            if(_fgDramDumpEn==FALSE)
            {
                _VBIResetFifo(VBI2);
                vHwVbiClrResetFifo(VBI2);
                return;
            }
        }

        /* VBI2: CC, V-Chip */       
    #if (SUPPORT_CLOSE_CAPTION_VCHIP)

        if (_t_VbiMode == VBI_CC_VCHIP_MODE)
        {
            if (IsCC2Enable()&& IsCC2Rdy())
            {              
                if ((fgIsVdoStable(SV_VP_MAIN) && 
                    (_u1VbiCCMainNotify == SV_ON) && 
                    (_u1VBI2Path == SV_VP_MAIN)) 
                    ||
                    (fgIsVdoStable(SV_VP_PIP) && 
                    (_u1VbiCCSubNotify == SV_ON) && 
                    (_u1VBI2Path == SV_VP_PIP)))
                {  
                    _VBINotify(VBI_CC_SERVICE, _u1VBI2Path, u1GetCC2_B0(), u1GetCC2_B1(), u4CurVbi2Field);                    
                    #if 0//#if SUPPORT_TOFU_DISPLAY
                    vDrvVbiIncVBI2CCRatio(u4CurVbi2Field);
                    LOG(3, "[VBI] VBI2 CCRdyRatio = %d , Notify CC\n", _u1VBI2CCRdyRatio[u4CurVbi2Field]);
                    if (_bVBI2StartTofoDisplay[u4CurVbi2Field]==FALSE && _u1VBI2CCRdyRatio[u4CurVbi2Field]>_u1TofuHiTh)
                    {
                        LOG(3, "[VBI] VBI2 START to display ToFu Field=%d\n", u4CurVbi2Field);
                        if (_u1CC2FailNo[u4CurVbi2Field] > _CCTOutInfo.u1ThresHold)
                            LOG(3, "[VBI] VBI2 _u1CC2FailNo[u4CurVbi2Field]=%d, _CCTOutInfo.u1ThresHold=%d\n", _u1CC2FailNo[u4CurVbi2Field], _CCTOutInfo.u1ThresHold);
                        _bVBI2StartTofoDisplay[u4CurVbi2Field] = TRUE;
                    }
                    #endif
                }
                _u1CC2FailNo[u4CurVbi2Field] = 0;
            }
            else
            {
                #if 0 //#if SUPPORT_TOFU_DISPLAY
                if ((fgIsVdoStable(SV_VP_MAIN) && 
                    (_u1VbiCCMainNotify == SV_ON) && 
                    (_u1VBI2Path == SV_VP_MAIN)) 
                    ||
                    (fgIsVdoStable(SV_VP_PIP) && 
                    (_u1VbiCCSubNotify == SV_ON) && 
                    (_u1VBI2Path == SV_VP_PIP)))
                {                    
                    vDrvVbiDecVBI2CCRatio(u4CurVbi2Field);
                    if (_bVBI2StartTofoDisplay[u4CurVbi2Field]==TRUE && _u1VBI2CCRdyRatio[u4CurVbi2Field]<_u1TofuLoTh)
                    {
                        LOG(3, "[VBI] VBI2 STOP to display ToFu Field=%d\n", u4CurVbi2Field);
                        _bVBI2StartTofoDisplay[u4CurVbi2Field] = FALSE;
                    }
                    
                    if (_bVBI2StartTofoDisplay[u4CurVbi2Field] && _bEnableTofuDisplay)
                    {
                        LOG(3, "[VBI] VBI2 CCRdyRatio = %d , Notify ToFu\n", _u1VBI2CCRdyRatio[u4CurVbi2Field]);
                        //_VBINotify(VBI_CC_SERVICE, _u1VBI2Path, CC_TOfU_DATA, CC_TOfU_DATA, u4CurVbi2Field);
                    }      
                }
                #else
                /* Count NO CC data. */
                if (_u1CC2FailNo[u4CurVbi2Field] < _CCTOutInfo.u1ThresHold)
                {
                    _u1CC2FailNo[u4CurVbi2Field]++;
                }
                else if (_CCTOutInfo.bEnCCTimeOut) //Notify No CC data.
                {
                    _u1CC2FailNo[u4CurVbi2Field] = 0;
                    _VBINotify(VBI_CC_SERVICE, _u1VBI2Path, _CCTOutInfo.u1NoCCData0, _CCTOutInfo.u1NoCCData1, u4CurVbi2Field);   
                }
                #endif
            }
        }
        #if EN_TVE_CB 
        if ((IsCC2Enable()&& IsCC2Rdy()))
        {
            if (_pfnTVECcNotify)
            {
                _pfnTVECcNotify(_u1VBI2Path, u1GetCC2_B0(), u1GetCC2_B1(), u4CurVbi2Field);
            }
            //Printf("TVE NFY [Path,CCB0,CCB1, Field] [%d, %X,%X, %d]\n",_u1VBI2Path, u1GetCC2_B0(), u1GetCC2_B1(),u4CurVbi2Field );
        }
        #endif
        
    #endif // #if (SUPPORT_CLOSE_CAPTION_VCHIP)

        /* VBI2: TTX */
    #if SUPPORT_TELETEXT

        if (IsTT2Rdy())
        {
            if ((_u1Ttx2SuccNo[0] < TTX_SUCC_THRES) && (_u1Ttx2SuccNo[1] < TTX_SUCC_THRES))
            {
                _u1Ttx2SuccNo[u4CurVbi2Field]++;
            }
            else
            {
            if ((fgIsVdoStable(SV_VP_MAIN) &&
                (_u1VbiTTXMainNotify == SV_ON) && 
                (_u1VBI2Path == SV_VP_MAIN)) 
                ||
                (fgIsVdoStable(SV_VP_PIP) &&
                (_u1VbiTTXSubNotify == SV_ON) && 
                (_u1VBI2Path == SV_VP_PIP)))
            {
                    if(_u4NewVbi2WritePtr > _u4Vbi2ReadPtr)
                    {
                        _u4TTXPktAvail = (_u4NewVbi2WritePtr-_u4Vbi2ReadPtr)/_u1Vbi2PitchSz;                       
                        _VBINotify(VBI_TTX_SERVICE, _u1VBI2Path, _u4TTXPktAvail, _u4Vbi2ReadPtr, 0);
                    }
                    else if(_u4NewVbi2WritePtr < _u4Vbi2ReadPtr)
                    {
                        _u4TTXPktAvail = ((_u4Vbi2FifoSa + _u4Vbi2FifoSz) -_u4Vbi2ReadPtr)/_u1Vbi2PitchSz;   
                        _VBINotify(VBI_TTX_SERVICE, _u1VBI2Path, _u4TTXPktAvail, _u4Vbi2ReadPtr, 0);
                        
                        _u4TTXPktAvail = (_u4NewVbi2WritePtr -_u4Vbi2FifoSa)/_u1Vbi2PitchSz;
                        _VBINotify(VBI_TTX_SERVICE, _u1VBI2Path, _u4TTXPktAvail, _u4Vbi2FifoSa, 0);
                    }
                }
                _u1Ttx2FailNo[u4CurVbi2Field] = 0;
            }
            /* Move forward Read pointer */
            _u4Vbi2ReadPtr = _u4NewVbi2WritePtr;
        }
        else
        {
            if (_u1Ttx2FailNo[u4CurVbi2Field] < TTX_FAIL_THRES)
            {
                _u1Ttx2FailNo[u4CurVbi2Field]++;
            }
            else
            {
                _u1Ttx2SuccNo[u4CurVbi2Field] = 0;
            }
        }
        
    #endif // #if SUPPORT_TELETEXT

        /* VBI2: WSS */
    #if SUPPORT_WSS
        #if WSS_DUAL_SLICER
        _u1VideoType[1] = bDrvVideoGetType(_u1VBI2Path);
        #endif
    
        if (IsWSS2Enable() && IsWSS2Rdy())
        {
            _u4Wss2 = _VBI_GetWSSData(_u1VBI2Path, VBI2); 

            /* WSS debounce counter */
            if (_u4Wss2 != _u4Wss2Prev)
                _u1Wss2SuccNo = 0;
            else if (_u1Wss2SuccNo < MAX_SUCC_THRES)
                _u1Wss2SuccNo++;  
            
            //EU WSS625
            #if WSS_DUAL_SLICER
            if (_u1VideoType[1] == VDOTYPE_PAL)
            #else
            if ((_t_VbiMode == VBI_TTX_MODE)||(_t_VbiMode == VBI_ANAS_MODE))
            #endif            
            {
                if (_VBI_WSSValidityCheck(_u1VBI2Path, 
                                            u4CurVbi2Field, 
                                            _u4Wss2,
                                            _u1VbiEUWSSMainNotify,
                                            _u1VbiEUWSSSubNotify,
                                            _u1Wss2SuccNo) == WSS625_Pass)
                {
                    _VBINotify(VBI_WSS625_SERVICE, _u1VBI2Path, _u4Wss2, 0, 0);
                } 
                #if EN_TVE_CB 
                if (_pfnTVEWss625Notify)
                {
                    _pfnTVEWss625Notify(_u1VBI2Path, _u4Wss2);
                }
                //Printf("TVE NFY [Path,WSS625] [%d,%X]\n",_u1VBI2Path, _u4Wss2);
                #endif
            }

            //US WSS525
            #if WSS_DUAL_SLICER
            else if (_u1VideoType[1] == VDOTYPE_NTSC)
            #else
            else if (_t_VbiMode == VBI_CC_VCHIP_MODE)
            #endif            
            {
                if (_VBI_WSSValidityCheck(_u1VBI2Path, 
                                            u4CurVbi2Field, 
                                            _u4Wss2,
                                            _u1VbiUSWSSMainNotify,
                                            _u1VbiUSWSSSubNotify,
                                            _u1Wss2SuccNo) == WSS525_Pass)
                {
                    _VBINotify(VBI_WSS525_SERVICE, _u1VBI2Path, _u4Wss2, 0, 0);
                }

                #if EN_TVE_CB 
                if (_pfnTVEWss525Notify)
                {
                    _pfnTVEWss525Notify(_u1VBI2Path, _u4Wss2);
                }
                //Printf("TVE NFY [Path,WSS525] [%d,%X]\n",_u1VBI2Path, _u4Wss2);
                #endif
            }
            
            _u4Wss2Prev = _u4Wss2;
            _u1Wss2FailNo = 0;
        }
        else
        {
            UINT8 u1Thres;
            u1Thres = ((_t_VbiMode == VBI_TTX_MODE) || (_t_VbiMode == VBI_ANAS_MODE)) ? WSS625_FAIL_THRES : WSS525_FAIL_THRES;
            _u1Wss2FailNo = (_u1Wss2FailNo < u1Thres) ? (_u1Wss2FailNo+1) : _u1Wss2FailNo; 
            _u4Wss2 = (_u1Wss2FailNo >= u1Thres) ? _WSSTOutInfo.u4NoWSSData : _u4Wss2;
            _u1Wss2SuccNo = (_u1Wss2FailNo >= u1Thres) ? 0 : _u1Wss2SuccNo; 

            _u1Wss2TimeOutNo = ((_u4Wss2 == _WSSTOutInfo.u4NoWSSData) && (_u1Wss2TimeOutNo < _WSSTOutInfo.u1ThresHold)) ? (_u1Wss2TimeOutNo+1) : 0;
            
            if (_u1Wss2TimeOutNo >=  _WSSTOutInfo.u1ThresHold)
            {
                #if WSS_DUAL_SLICER
                if ((_u1VideoType[1] == VDOTYPE_PAL) && (_WSSTOutInfo.bEnWSS625TOut))
                #else
                if (((_t_VbiMode == VBI_TTX_MODE) || (_t_VbiMode == VBI_ANAS_MODE)) && (_WSSTOutInfo.bEnWSS625TOut))
                #endif
                {

                    if (((_u1VbiEUWSSMainNotify == SV_ON) && 
                        (_u1VBI2Path == SV_VP_MAIN)) 
                        ||
                        ((_u1VbiEUWSSSubNotify == SV_ON) && 
                        (_u1VBI2Path == SV_VP_PIP)))
                    {
                        _VBINotify(VBI_WSS625_SERVICE, _u1VBI2Path, _u4Wss2, 0, 0);                         
                    }
                }

                #if WSS_DUAL_SLICER
                if ((_u1VideoType[1] == VDOTYPE_NTSC) && (_WSSTOutInfo.bEnWSS525TOut))
                #else
                else if ((_t_VbiMode == VBI_CC_VCHIP_MODE)&& (_WSSTOutInfo.bEnWSS525TOut))
                #endif
                {
                    if (((_u1VbiUSWSSMainNotify == SV_ON) && 
                        (_u1VBI2Path == SV_VP_MAIN)) 
                        ||
                        ((_u1VbiUSWSSSubNotify == SV_ON) && 
                        (_u1VBI2Path == SV_VP_PIP)))
                    {
                        _VBINotify(VBI_WSS525_SERVICE, _u1VBI2Path, _u4Wss2, 0, 0);                         
                    }
                }
            }
        }

    #endif

        /* VBI2: VPS */
    #if SUPPORT_AUTO_NAMING

        if (_t_VbiMode == VBI_ANAS_MODE)
        {
            if(u4CurVbi2Field == 0)
            {
                if (IsVPS2Enable() && IsVPS2Rdy())
                {
                    if (_u1Vps2SuccNo < VPS_SUCC_THRES)
                    {
                        _u1Vps2SuccNo++;
                    }
                    else
                    {
                        _u1Vps2FailNo = 0;
                        
                        if (_u1VbiVPSCNINotify == SV_ON)
                        {                               
                            _au1Vps2RawData[0] = u1GetVPS_B0();
                            _au1Vps2RawData[1] = u1GetVPS_B1();
                            _au1Vps2RawData[2] = u1GetVPS_B2();
                            _au1Vps2RawData[3] = u1GetVPS_B3();
                            _au1Vps2RawData[4] = u1GetVPS_B4();
                            _au1Vps2RawData[5] = u1GetVPS_B5();
                            _au1Vps2RawData[6] = u1GetVPS_B6();
                            _au1Vps2RawData[7] = u1GetVPS_B7();
                            _au1Vps2RawData[8] = u1GetVPS_B8();
                            _au1Vps2RawData[9] = u1GetVPS_B9();
                            _au1Vps2RawData[10] = u1GetVPS_B10();
                            _au1Vps2RawData[11] = u1GetVPS_B11();
                            _au1Vps2RawData[12] = u1GetVPS_B12();                                                                
                             _VBINotify(VBI_VPS_SERVICE, _u1VBI2Path, (UINT32)_au1Vps2RawData, 0, 0); 
                        }
                    }
                }
                else
                {
                    if (_u1Vps2FailNo < VPS_FAIL_THRES)
                    {
                        _u1Vps2FailNo++;
                    }
                    else
                    {
                        _u1Vps2SuccNo = 0;                        
                    }
                }
            }
        }

    #endif      

    }

    /* VBI2 ISR End */
#if VBI_CLI_SUPPORT
if (_pfnCLINotify)
    _pfnCLINotify(NULL);
#endif
// Clear VBI interrupt
//VERIFY(BIM_ClearIrq(VECTOR_VBI));

}

/**
 * @brief   Init VBI ISR
 *
 * @param   none
 *
 * @retval  None
 */
/*
void VBIISR_Init(void) 
{
    x_os_isr_fct pfnOldIsr;
//	VBI_Disable();		  
//	VERIFY(BIM_ClearIrq(VECTOR_VBI));
	// Register VBI_ISR ISR
	if (x_reg_isr(VECTOR_VBI, VBI_ISR, &pfnOldIsr) != OSR_OK)
	{
				LOG(1, "Error: fail to register VBI_ISR!\n");
	}

}
*/
/**
 * @brief   Enable VBI
 *
 * @param   none
 *
 * @retval  None
 */
void VBI_Enable(void) 
{
    /* Enable VBI Interrupt */
    _VBI_ISREnable();

    switch (_t_VbiMode)
    {
        #if (SUPPORT_CLOSE_CAPTION_VCHIP || SUPPORT_CLOSE_CAPTION_2X)
        case VBI_CC_VCHIP_MODE:
            vHwVbiSetLineEnable(VBI0, (CC_LINE_EN | WSS_LINE_EN));
            #if SUPPORT_CLOSE_CAPTION_2X
            vHwVbiSetLineEnable(VBI0, (TT_LINE_EN | CC_LINE_EN | WSS_LINE_EN));
            #endif
            vHwVbiSetOutputMode(VBI0, VBI_OMODE_NO);    
        
            vHwVbiSetLineEnable(VBI2, (CC_LINE_EN | WSS_LINE_EN));
            #if SUPPORT_CLOSE_CAPTION_2X
            vHwVbiSetLineEnable(VBI2, (TT_LINE_EN | CC_LINE_EN | WSS_LINE_EN));
            #endif
            vHwVbiSetOutputMode(VBI2, VBI_OMODE_NO);
            break;
        #endif
        
        #if SUPPORT_TELETEXT
        case VBI_TTX_MODE:
            vHwVbiSetLineEnable(VBI0, (WSS_LINE_EN | TT_LINE_EN));
            /* TTX data output to DRAM, OMODE is turned on in FIFO enable function */
            vHwVbiSetOutputMode(VBI0, VBI_OMODE_NO);

            vHwVbiSetLineEnable(VBI2, (WSS_LINE_EN | TT_LINE_EN));
            /* TTX data output to DRAM, OMODE is turned on in FIFO enable function */
            vHwVbiSetOutputMode(VBI2, VBI_OMODE_NO);
            break;
        #endif
        
        #if SUPPORT_AUTO_NAMING
        case VBI_ANAS_MODE:
            /* VBI working in scaning is changed for VBI2's multi-slicing */
            vHwVbiSetLineEnable(VBI0, (WSS_LINE_EN | TT_LINE_EN));
            vHwVbiSetOutputMode(VBI0, VBI_OMODE_DEC);

            vHwVbiSetLineEnable(VBI2, VPS_LINE_EN);
            vHwVbiSetOutputMode(VBI2, VBI_OMODE_NO);
            break;
        #endif
        
        #if (SUPPORT_WSS &&             \
            !SUPPORT_CLOSE_CAPTION_VCHIP &&   \
            !SUPPORT_TELETEXT &&        \
            !SUPPORT_AUTO_NAMING)
        case VBI_WSS_ONLY_MODE:
            vHwVbiSetLineEnable(VBI0, WSS_LINE_EN);
            /* WSS data is read from register directly */
            vHwVbiSetOutputMode(VBI0, VBI_OMODE_NO);    

            vHwVbiSetLineEnable(VBI2, WSS_LINE_EN);
            /* WSS data is read from register directly */
            vHwVbiSetOutputMode(VBI2, VBI_OMODE_NO);
            break;
        #endif
        default:
            break;
    }
}

/**
 * @brief   Disable VBI
 *
 * @param   none
 *
 * @retval  None
 */
void VBI_Disable(void) 
{
    // Disable CC Interrupt
    _VBI_ISRDisable();

    vHwVbiSetLineEnable(VBI0, 0);
    vHwVbiSetOutputMode(VBI0, VBI_OMODE_NO);
    vHwVbiSetLineEnable(VBI2, 0);
    vHwVbiSetOutputMode(VBI2, VBI_OMODE_NO);
}

/**
 * @brief     Reset VBI src related setting when MODE_CHG_DONE. (mloop_video.c)
 * @param     bPath: SV_VP_MAIN / SV_VP_PIP
 * @retval    None
 */
void VBI_ResetSrc(UINT8 u1Path) 
{
    UINT8 u1VideoType;
    UINT8 u1Src;
    UINT8 u1MatchVBISuite = VBI0; /* u1Path is matched with which VBI slicer. VBI0 or VBI2 */
    UINT8 u1i = 0;
    FBM_POOL_T*  prFbmPool;
    
    /* VBI Mutex Protection */
    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    _VBI_ISRDisable();

    /* Check Video Type and reset VBI setting. */
    u1VideoType = bDrvVideoGetType(u1Path);

    _u1VBI0Path = SV_VP_MAIN;
    _u1VBI2Path = SV_VP_PIP;
    
    if(u1Path == SV_VP_MAIN)
    {
        u1MatchVBISuite = VBI0;
    }
    else if(u1Path == SV_VP_PIP)
    {
        u1MatchVBISuite = VBI2;
    }
    
    //VBI0 deal with TVD3D Input 
    //VBI2 deal with Component Input
    if ((bGetVideoDecType(SV_VP_MAIN) == (UINT8)SV_VD_YPBPR) ||
        (bGetVideoDecType(SV_VP_PIP) == (UINT8)SV_VD_TVD3D))
    { 
        /* VBI Slicer and Src remapping */
        _u1VBI0Path = SV_VP_PIP;
        _u1VBI2Path = SV_VP_MAIN;
        if (u1Path == SV_VP_MAIN)
        {
            u1MatchVBISuite = VBI2;
        }
        else if(u1Path == SV_VP_PIP)
        {
            u1MatchVBISuite = VBI0;
        }
    }
    LOG(5, "VBI_ResetSrc: %d %d\n", u1Path, u1MatchVBISuite);

    if(u1MatchVBISuite == VBI0)
    {
        LOG(9, "[VBI]VBI0 Core Reset \n");
        vHwVbiCoreReset(VBI0);
    }
    else if(u1MatchVBISuite == VBI2)
    {
        LOG(9, "[VBI]VBI2 Core Reset \n");
        vHwVbiCoreReset(VBI2);
    }       

    if(u1MatchVBISuite == VBI0)
    {
        if(_t_VbiMode == VBI_CC_VCHIP_MODE)
        {
            vHwVbiSetPEGain(VBI0, 1);
        }
        else
        {
            vHwVbiSetPEGain(VBI0, 0);
        }
    }
    else if(u1MatchVBISuite == VBI2)
    {
        vHwVbiSetPEGain(VBI2, 2);
    }
    
    /* When doing auto-naming service, do not modify the driver setting. */
    if (_t_VbiMode == VBI_ANAS_MODE)
    {
        _VBIResetFifo(VBI0); // Need reset fifo after CoreReset 
        vHwVbiClrResetFifo(VBI0);
        goto EXIT;
    }

    /* VBI FIFO Reset */ 
    prFbmPool = FBM_GetPoolInfoAuto((UINT8)FBM_POOL_TYPE_VBI, NULL);
    if (prFbmPool)
    {
        _VBI_SetFIFO(_t_VbiMode, prFbmPool->u4Addr, prFbmPool->u4Size); 
        LOG(3, "VBI_ResetFifo: [Base, Size] [%x, %x]\n", prFbmPool->u4Addr, prFbmPool->u4Size);
        //Printf("VBI_ResetFifo: [Base, Size] [%x, %x]\n", prFbmPool->u4Addr, prFbmPool->u4Size);
    }

    /* Enable Main Window VBI FIFO */
    if (_u1TTXOnOff[SV_VP_MAIN])
    {
        _VBI_StartNotifyTTXOnOff(SV_VP_MAIN, TRUE, NULL);
    }

    /* Enable Sub Window VBI FIFO */
    if (_u1TTXOnOff[SV_VP_PIP])
    {
        _VBI_StartNotifyTTXOnOff(SV_VP_PIP, TRUE, NULL);
    }

    
    if (u1VideoType == VDOTYPE_PAL)
    {
        vHwVbiSetVbiMode(u1MatchVBISuite, VBI_PAL_MODE);
    }
    else
    {
        vHwVbiSetVbiMode(u1MatchVBISuite, VBI_NTSC_MODE);
    }

#if AUTO_CC_DETECT
    if(ui1AutoCCEnable && _t_VbiMode==VBI_CC_VCHIP_MODE)
    {
        //reset to original LineInfo
        if(!_VbiLineInfo.bEnMenuLineSet)
            VBI_SetLineInfo(&_VbiLineInfo);
        if (bGetVideoDecType(SV_VP_MAIN) == (UINT8)SV_VD_TVD3D)
        {
            if(u1MatchVBISuite == VBI0)
            {
                LOG(3, "Scan Line Init\n");
                ui1Scanline_SuccessCnt=0;
                ui1Scanline_SuccessIdx=0xFF;
                ui1Scanline_RetryCnt=0;
                VBIInitCCScan();

                if(u1VideoType == VDOTYPE_PAL)
                    vHwVbiSetVbiModeForAutoCC(VBI_PAL_MODE);
                else
                    vHwVbiSetVbiModeForAutoCC(VBI_NTSC_MODE);

                if(_VBIAvInfo.E_AVFieldType == E_FIELD_NORMAL)
                {
                    rCurrScanCCLine = rScanlineSeq_Normal[0];
                }
                else
                {
                    rCurrScanCCLine = rScanlineSeq_Inverse[0];
                }
                vHwVbiSetCCLine(VBI2, rCurrScanCCLine.u1F0_SET, rCurrScanCCLine.u1F1_SET);
                
                ui4ScanlineCCState = SCANLINE_START;
                LOG(3, "[VBI_ResetSrc] Start Auto CC Detection\n");
            }
        }
        else
        {
            ui4ScanlineCCState = SCANLINE_IDLE;
        }
    }
#endif


    /* Check Video Src */
    if (u1MatchVBISuite == VBI0)
    {
        #if SUPPORT_WSS
        _u4Wss = 0xffffffff;
        _u1WssTimeOutNo = 0;
        #endif
        
        #if SUPPORT_CLOSE_CAPTION_VCHIP
        _u1CCFailNo[0] = 0;
        _u1CCFailNo[1] = 0;
        _u1CC2FailNo[0] = 0;
        _u1CC2FailNo[1] = 0;
        #if CC_VCR_TRICK_CHECK
        _u4TrickPCounter = 0;
        _u4TrickNCounter = 0;
        _u1TrickBlock= SV_OFF;
        #endif
        #if SUPPORT_TOFU_DISPLAY        
        _bVBI0StartTofoDisplay[0] = _bVBI0StartTofoDisplay[1] = TRUE;
        _u1VBI0CCRdyRatio[0] = _u1VBI0CCRdyRatio[1] = 100;
        //_bVBI2StartTofoDisplay[0] = _bVBI2StartTofoDisplay[1] = TRUE;
        //_u1VBI2CCRdyRatio[0] = _u1VBI2CCRdyRatio[1] = 100;
        #endif
        #endif
        
        #if SUPPORT_TELETEXT || SUPPORT_AUTO_NAMING
        for (u1i = 0; u1i <2; u1i++)
        {
            _u1TtxFailNo[u1i] = TTX_FAIL_THRES;
            _u1TtxSuccNo[u1i] = 0;
        }
        #endif  
        
        #if SUPPORT_AUTO_NAMING
        _u1Vps2FailNo = VPS_FAIL_THRES;
        _u1Vps2SuccNo = 0;        
        #endif

        u1Src = bGetVideoDecType(_u1VBI0Path);
    
        /* Line Selection & Setting */
        _VBI_SetVBILines(&_VbiLineInfo, VBI0, u1Src, u1VideoType, _VBIAvInfo.E_AVFieldType);

        /* Set Initial VBI Gain */
        vHwVbiSetVBIGain(VBI0, AV_DFT_GAIN);
        /* Set Initial Zero-Crossing threshold */
        vHwVbiSetInitZCThr(VBI0, AV_ZC_TH);
        vHwVbiSetClrPScan(VBI0, 0);

        /* Line Start Code, VSRC and HDTV sub-sample setting */        
        if (u1Src == (UINT8)SV_VD_YPBPR)
        {
            /* Line Start Code */
            vHwVbiSetLSC(VBI0, HDTV_LSC);

            /* VSRC */
            vHwVbiSetSrc(VBI0, VBI_VSRC_HDTV);
            
            if(fgIsOversampleTiming()) /* determine is YPbPr oversampled */
            {/* Oversampled, divide frequency */
                vHwVbiHDTVSubSample(1);
            }
            else
            {/* No oversample, 27 MHz */
                vHwVbiHDTVSubSample(0);
            }
            /* Progressive Scan may send WSS, tested using BOK player */
            if(fgIsPScanTiming())
            {
                /* 1 LSC, HSAV half */
                /* 2 PScan set */
                /* 3 Freq half */
                /* 4 STC_ST half */
                /* 5 Line selection is by setting SEL0 and LSB of SEL1 */

                vHwVbiSetInitZCThr(VBI0, HDTV_PSCAN_ZC_TH);
                vHwVbiHDTVSubSample(0); /* Let VBI slicer work in 54 MHz */
                vHwVbiSetInitZC(VBI0, 0);
                
                vHwVbiSetClrPScan(VBI0, 1);
                if (u1VideoType == VDOTYPE_PAL)
                {
                    vHwVbiSetLSC(VBI0, HDTV_LSC);
                    vHwVbiSetWSSFreq(VBI0, 0x1CCD);
                }
                else
                {
                    vHwVbiSetLSC(VBI0, HDTV_PSCAN_480P_LSC);
                    vHwVbiSetWSSFreq(VBI0, 0x228E);
                }
            }
            
        }
        else  // AV PATH
        {
            /* Line Start Code, measured via debug port as 48.8 us / 37 ns */
            if (u1VideoType == VDOTYPE_PAL)
            {
                vHwVbiSetLSC(VBI0, AV_EU_LSC);
                _VBIAvInfo.E_AVSrcType = E_AV_PAL;
            }
            else
            {
                vHwVbiSetLSC(VBI0, AV_US_LSC);
                _VBIAvInfo.E_AVSrcType = E_AV_NTSC;
                //Set VBI gain to 6 for Improve AV CC performance.
                VBI_AdjVBIGain(AV_US_GAIN); 
            }
            /* VSRC */
            vHwVbiSetSrc(VBI0, VBI_VSRC_TVD3D);
        }        
    }
    else
    {
        #if SUPPORT_WSS
        _u4Wss2 = 0xffffffff;
        _u1Wss2TimeOutNo = 0;
        #endif

        #if SUPPORT_TELETEXT || SUPPORT_AUTO_NAMING
        for (u1i = 0; u1i < 2; u1i ++)
        {
            _u1Ttx2FailNo[u1i] = TTX_FAIL_THRES;
            _u1Ttx2SuccNo[u1i] = 0;
        }
        #endif        

        u1Src = bGetVideoDecType(_u1VBI2Path);
    
        /* Line Selection & Setting */
        _VBI_SetVBILines(&_VbiLineInfo, VBI2, u1Src, u1VideoType, _VBIAvInfo.E_AVFieldType);

        /* Set Initial Zero-Crossing threshold */
        vHwVbiSetInitZCThr(VBI2, HDTV_ZC_TH);
        vHwVbiSetClrPScan(VBI2, 0);

        if (u1Src == (UINT8)SV_VD_YPBPR)
        {
            /* Line Start Code */
            vHwVbiSetLSC(VBI2, HDTV_LSC);

            /* VSRC */
            vHwVbiSetSrc(VBI2, VBI_VSRC_HDTV);
            
            if(fgIsOversampleTiming()) /* determine is YPbPr oversampled */
            {
                /* Oversampled, divide frequency */
                vHwVbiHDTVSubSample(1);
            }
            else
            {
                /* No oversample, 27 MHz */
                vHwVbiHDTVSubSample(0);
            }
            /* Progressive Scan may send WSS, tested using BOK player */
            if(fgIsPScanTiming())
            {
                /* 1 LSC, HSAV half */
                /* 2 PScan set */
                /* 3 Freq half */
                /* 4 STC_ST half */
                /* 5 Line selection is by setting SEL0 and LSB of SEL1 */
                vHwVbiSetInitZCThr(VBI2, HDTV_PSCAN_ZC_TH);
                vHwVbiHDTVSubSample(0); /* Let VBI slicer work in 54 MHz */
                vHwVbiSetInitZC(VBI2, 0);

                vHwVbiSetClrPScan(VBI2, 1);
                if (u1VideoType == VDOTYPE_PAL)
                {
                    vHwVbiSetLSC(VBI2, HDTV_LSC);
                    vHwVbiSetWSSFreq(VBI2, 0x1CCD);
                }
                else
                {
                    vHwVbiSetLSC(VBI2, HDTV_PSCAN_480P_LSC);
                    vHwVbiSetWSSFreq(VBI2, 0x228E);
                }
            }
        }
        else
        {
            /* Line Start Code, measured via debug port as 48.8 us / 37 ns */
            if (u1VideoType == VDOTYPE_PAL)
            {
                vHwVbiSetLSC(VBI2, AV_EU_LSC);
                //_VBIAvInfo.E_AVSrcType = E_AV_PAL;
            }
            else //NTSC
            {
                vHwVbiSetLSC(VBI2, AV_US_LSC);
                //_VBIAvInfo.E_AVSrcType = E_AV_NTSC;
            }
            /* VSRC */
            vHwVbiSetSrc(VBI2, VBI_VSRC_TVD3D);
        }
    }    

    if (u1Src == (UINT8)SV_VD_TVD3D)
    {
        /* Check slicer and path matching */
        _VBIEnableBySlicer(u1MatchVBISuite);
    }
    else if (u1Src == (UINT8)SV_VD_YPBPR)
    {
        if(fgIsVBISupportTiming())
        {
            _VBIEnableBySlicer(u1MatchVBISuite);
            #if EN_TVE_CB
            if (fgIsVBISupportCCTiming())
            {
                _VBIEnCC_WSS_Slicer(u1MatchVBISuite);
            }
            #endif
        }
        else
        {
            LOG(3, "VBI_ResetSrc: Not Supported Timing \n");
            VBI_DisableBySlicer(u1MatchVBISuite);
        }
    }
    
EXIT:
    _VBI_ISREnable();
    /* VBI Mutex protection */
    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}

/**
 * @brief    SET SV_VP_MAIN/SV_VP_PIP Path to VBI0/VBI2 
 * @param    u1VBISuite: VBI0 / VBI2
 * @param    u1Path: SV_VP_MAIN / SV_VP_PIP
 * @retval   None
 */
void VBI_SetSlicerPath(UINT8 u1VBISuite, UINT8 u1Path)
{
    if(u1VBISuite == VBI0)
    {
        _u1VBI0Path = u1Path;
    }
    else
    {
        _u1VBI2Path = u1Path;
    }
}

/**
 * @brief    Get SV_VP_MAIN/SV_VP_PIP Path to VBI0/VBI2 
 * @param    u1VBISuite: VBI0 / VBI2
 * @retval   SV_VP_MAIN / SV_VP_PIP
 */
UINT8 VBI_GetSlicerPath(UINT8 u1VBISuite)
{
    if (u1VBISuite == VBI0)
    {
        return _u1VBI0Path;
    }
    else
    {
        return _u1VBI2Path;
    }
}

#if EN_TVE_CB
/**
 * @brief Set TV Encoder Closed Caption Nofity 
 * @param pfnCCnotify: CC notify CallBack
 * @Note Only for Component Path
 */
void VBI_SetTVECCNfy(NPTV_PFN_CC_NFY pfnCCnotify)
{
    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    _pfnTVECcNotify = pfnCCnotify;

    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}


/**
 * @brief Set TV Encoder WSS625 Nofity 
 * @param pfnWSS625notify: WSS625 notify CallBack
 * @Note Only for Component Path
 */
void VBI_SetTVEWSS625Nfy(NPTV_PFN_WSS_NFY pfnWSS625notify)
{
    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    _pfnTVEWss625Notify = pfnWSS625notify;

    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}

/**
 * @brief Set TV Encoder WSS525 Nofity 
 * @param pfnWSS525notify: WSS525 notify CallBack
 * @Note Only for Component Path
 */
void VBI_SetTVEWSS525Nfy(NPTV_PFN_WSS_NFY pfnWSS525notify)
{
    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    _pfnTVEWss525Notify = pfnWSS525notify;

    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}
#endif

/**
 * @brief    Enable/Disable CC Notify 
 * @param    u1Path: SV_VP_MAIN / SV_VP_PIP
 * @param    u1OnOff: TRUE: Enable Notfiy / FALSE: Disable Notify 
 * @param    pfnCCnotify: Reserved.
 * @retval   Noon
 */
void VBI_StartNotifyCCOnOff(UINT8 u1Path, UINT8 u1OnOff, NPTV_PFN_CC_NFY pfnCCnotify)
{

    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if(u1Path == SV_VP_MAIN)
    {    
        _u1VbiCCMainNotify = u1OnOff;
    }
    else if(u1Path == SV_VP_PIP)
    {
        _u1VbiCCSubNotify = u1OnOff;    
    }
    
    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}

/**
 * @brief    Enable/Disable CC2x Notify 
 * @param    u1Path: SV_VP_MAIN / SV_VP_PIP
 * @param    u1OnOff: TRUE: Enable Notfiy / FALSE: Disable Notify  
 * @param    pfnCC2Xnotify: Reserved.
 * @retval   Noon
 */
void VBI_StartNotifyCC2XOnOff(UINT8 u1Path, UINT8 u1OnOff, NPTV_PFN_CC2X_NFY pfnCC2Xnotify)
{
    UINT8 u1MatchVBISuite; /* u1Path is matched with which VBI slicer. VBI0 or VBI2 */

    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
   
    if(u1Path == SV_VP_MAIN)
    {    
        _u1VbiCC2XMainNotify = u1OnOff;
    }
    else if(u1Path == SV_VP_PIP)
    {
        _u1VbiCC2XSubNotify = u1OnOff;    
    }

    u1MatchVBISuite = (_u1VBI0Path == u1Path) ? VBI0 : VBI2;
    
    if(u1OnOff == SV_ON)
    {
        _VBI_FifoEnable(u1MatchVBISuite);
    }
    else
    {
        _VBI_FifoDisable(u1MatchVBISuite); 
        _VBIResetFifo(u1MatchVBISuite);
    }
    
    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}

/**
 * @brief    Enable/Disable TTX Notify 
 * @param    u1Path: SV_VP_MAIN / SV_VP_PIP
 * @param    u1OnOff: TRUE: Enable Notfiy / FALSE: Disable Notify 
 * @param    pfnTTXnotify: Reserved.
 * @retval   Noon
 */
void VBI_StartNotifyTTXOnOff(UINT8 u1Path, UINT8 u1OnOff, NPTV_PFN_TTX_NFY pfnTTXnotify)
{

    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (u1Path >= SV_VP_NA)
    {
        LOG(1, "VBI_StartNotifyTTXOnOff Error Parameters u1Path = %d\n", u1Path);
        return;
    }

    //Keep TTX OnOff Information for Reset Src.  
    _u1TTXOnOff[u1Path] = u1OnOff;
 
    _VBI_StartNotifyTTXOnOff(u1Path, u1OnOff, NULL);

    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK); 
}

/**
 * @brief    Enable/Disable EU WSS625 Notify 
 * @param    u1Path: SV_VP_MAIN / SV_VP_PIP
 * @param    u1OnOff: TRUE: Enable Notfiy / FALSE: Disable Notify 
 * @param    pfnWSSnotify: Reserved.
 * @retval   Noon
 */

void VBI_StartNotifyWSSOnOff(UINT8 u1Path, UINT8 u1OnOff, NPTV_PFN_WSS_NFY pfnWSSnotify)
{
    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if(u1Path == SV_VP_MAIN)
    {    
        _u1VbiEUWSSMainNotify = u1OnOff;
    }
    else if(u1Path == SV_VP_PIP)
    {
        _u1VbiEUWSSSubNotify = u1OnOff;    
    }

    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}

/**
 * @brief    Enable/Disable US WSS525 Notify 
 * @param    u1Path: SV_VP_MAIN / SV_VP_PIP
 * @param    u1OnOff: TRUE: Enable Notfiy / FALSE: Disable Notify 
 * @param    pfnWSSnotify: Reserved.
 * @retval   Noon
 */
void VBI_StartNotifyUSWSSOnOff(UINT8 u1Path, UINT8 u1OnOff, NPTV_PFN_WSS_NFY pfnWSSnotify)
{ 
    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    if(u1Path == SV_VP_MAIN)
    {    
        _u1VbiUSWSSMainNotify = u1OnOff;
    }
    else if(u1Path == SV_VP_PIP)
    {
        _u1VbiUSWSSSubNotify = u1OnOff;    
    }

    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}

/**
 * @brief    Enable/Disable VPS Notify 
 * @param    u1Path: SV_VP_MAIN / SV_VP_PIP
 * @param    u1OnOff: TRUE: Enable Notfiy / FALSE: Disable Notify 
 * @param    pfnVPSnotify: Reserved.
 * @retval   Noon
 */
void VBI_StartNotifyVPSOnOff(UINT8 u1OnOff, NPTV_PFN_VPS_NFY pfnVPSnotify)
{

    /* vDrvVbiInit will initialize all global variables, 
       so the status need to be recovered */
    UINT8 u1VbiTTXMainNotify = _u1VbiTTXMainNotify;
    UINT8 u1VbiTtxSubNotfy = _u1VbiTTXSubNotify;

    UINT8 u1WSSMainOnOff = _u1VbiEUWSSMainNotify;
    UINT8 u1WSSSubOnOff = _u1VbiEUWSSSubNotify;

    /* US WSS525 */
    UINT8 u1USWSSMainOnOff = _u1VbiUSWSSMainNotify;
    UINT8 u1USWSSSubOnOff = _u1VbiUSWSSSubNotify;

    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    if (u1OnOff ==  SV_ON)
    { 
        /* Driver expects this behavior only occurs in entering Channel Searching */
        VBI_Disable();  
        VBI_Init(VBI_ANAS_MODE);
        //Set VBI Lines. 
        _VBI_SetVBILines(&_VbiLineInfo, VBI0, SV_VD_TVD3D, VDOTYPE_PAL, _VBIAvInfo.E_AVFieldType);
        _VBI_SetVBILines(&_VbiLineInfo, VBI2, SV_VD_TVD3D, VDOTYPE_PAL, _VBIAvInfo.E_AVFieldType);
        VBI_Enable(); //Enable for ANAS Mode.
    }
    else
    {
        VBI_Disable();  
        VBI_Init(VBI_TTX_MODE);
        //Set VBI Lines. 
        _VBI_SetVBILines(&_VbiLineInfo, VBI0, SV_VD_TVD3D, VDOTYPE_PAL, _VBIAvInfo.E_AVFieldType);
        _VBI_SetVBILines(&_VbiLineInfo, VBI2, SV_VD_TVD3D, VDOTYPE_PAL, _VBIAvInfo.E_AVFieldType);
        VBI_Enable();//Enable for TTX Mode.
    }
    _u1VbiVPSCNINotify = u1OnOff;

    /* Record previous notify status */
    _u1VbiTTXMainNotify = u1VbiTTXMainNotify;
    _u1VbiTTXSubNotify = u1VbiTtxSubNotfy;
    if (_u1VbiTTXMainNotify == SV_ON)
    {
        _VBI_FifoEnable(VBI0);
    }
    if (_u1VbiTTXSubNotify == SV_ON)
    {
        _VBI_FifoEnable(VBI2);
    }
    
    _u1VbiEUWSSMainNotify = u1WSSMainOnOff;
    _u1VbiEUWSSSubNotify = u1WSSSubOnOff;

    /* US WSS525 */
    _u1VbiUSWSSMainNotify = u1USWSSMainOnOff;
    _u1VbiUSWSSSubNotify = u1USWSSSubOnOff;

    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);   
}

/**
 * @brief    Query VBI AV Input Information 
 * @param    OUT: pAvInfo: refer to VBI_AV_INFO structure.
 * @retval   Noon
 */
void VBI_QueryAVInfo(VBI_AV_INFO *pVBIAvInfo)
{
    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    pVBIAvInfo->E_AVFieldType = _VBIAvInfo.E_AVFieldType;
    pVBIAvInfo->E_AVSrcType = _VBIAvInfo.E_AVSrcType;

    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}

/**
 * @brief    Set VBI AV Input Information 
 * @param    IN: bIs525: TRUE: 525 / FALSED: 625.
 * @param    IN: bIsFieldIvs: TRUE: FieldInversed / FALSED: Field Normal.
 * @param    IN: u1Mode: TVD Standard Mode.
 * @retval   Noon
 */
void VBI_SetAVFieldType(BOOL bIs525, BOOL bIsFieldIvs, UINT8 u1Mode)
{
    AV_SRC_TYPE E_Src_type = E_AV_UNKNOW;
    AV_FIELD_TYPE E_FieldType = E_FIELD_UNKNOW;

    E_Src_type = (bIs525) ? E_AV_NTSC : E_AV_PAL;
    E_FieldType = (bIsFieldIvs) ? E_FIELD_INVERSED : E_FIELD_NORMAL;

    //VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (E_Src_type == _VBIAvInfo.E_AVSrcType)//Check for mode change done.
    {
        if (E_FieldType != _VBIAvInfo.E_AVFieldType)
        {        
            _VBI_SetVBILines(&_VbiLineInfo, VBI0, SV_VD_TVD3D, _VBIAvInfo.E_AVSrcType, E_FieldType);
            _VBIAvInfo.E_AVFieldType = E_FieldType;
            
            /* Notify VBI Field Type */
            if (E_FieldType == E_FIELD_INVERSED)
            {
                vAPiNotifyVbiInfoData (TRUE);
            }
            else
            {
                vAPiNotifyVbiInfoData(FALSE);
            }
        }
    }

    //VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}

void VBI_SetLineInfoByAP(BOOL bIs525, UINT8 u1Mode, UINT8 u1Line)
{
    AV_SRC_TYPE E_Src_type = E_AV_UNKNOW;
    AV_FIELD_TYPE E_FieldType = E_FIELD_UNKNOW;

    E_Src_type = (bIs525) ? E_AV_NTSC : E_AV_PAL;
    E_FieldType = _VBIAvInfo.E_AVFieldType;

    //VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (E_Src_type == _VBIAvInfo.E_AVSrcType)//Check for mode change done.
    {
        switch(u1Mode)
        {
            case VBI_TTX_SERVICE:
                break;
            case VBI_CC_SERVICE:
                if (E_FieldType == E_FIELD_NORMAL)
                {
                    if(bIs525)
                    {
                        _VbiLineInfo.AV_NTSC_CC.u1F0_SET += u1Line - AV_NML_NTSC_CC_SEL0;
                        _VbiLineInfo.AV_NTSC_CC.u1F1_SET += u1Line - AV_NML_NTSC_CC_SEL1;
                    }
                    else
                    {
                        _VbiLineInfo.AV_PAL_CC.u1F0_SET += u1Line - AV_NML_PAL_CC_SEL0;
                        _VbiLineInfo.AV_PAL_CC.u1F1_SET += u1Line - AV_NML_PAL_CC_SEL1;
                    }
                }
                else if(E_FieldType == E_FIELD_INVERSED)
                {
                    if(bIs525)
                    {
                        _VbiLineInfo.AV_NTSC_CC.u1F0_SET += u1Line - AV_IVS_NTSC_CC_SEL0;
                        _VbiLineInfo.AV_NTSC_CC.u1F1_SET += u1Line - AV_IVS_NTSC_CC_SEL1;
                    }
                    else
                    {
                        _VbiLineInfo.AV_PAL_CC.u1F0_SET += u1Line - AV_IVS_PAL_CC_SEL0;
                        _VbiLineInfo.AV_PAL_CC.u1F1_SET += u1Line - AV_IVS_PAL_CC_SEL1;
                    }
                }
                _VBI_SetVBILines(&_VbiLineInfo, VBI0, SV_VD_TVD3D, _VBIAvInfo.E_AVSrcType, E_FieldType);
                break;
            case VBI_CC2X_SERVICE:
            case VBI_WSS525_SERVICE:
            case VBI_WSS625_SERVICE:
            case VBI_VPS_SERVICE:
            default:
                break;
        }
    }

    //VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}


/**
 * @brief    Set VBI WSS TimeOut Information 
 * @param    IN: pWssTOutInfo: refer to VBI_WSS_TIMEOUT_INFO structure.
 * @retval   Noon
 */
void VBI_SetWSSTimeOutInfo(VBI_WSS_TIMEOUT_INFO *pWssTOutInfo)
{

    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (pWssTOutInfo)
    {
        _WSSTOutInfo.bEnWSS525TOut = pWssTOutInfo->bEnWSS525TOut;
        _WSSTOutInfo.bEnWSS625TOut = pWssTOutInfo->bEnWSS625TOut;
        _WSSTOutInfo.u1ThresHold   = pWssTOutInfo->u1ThresHold;
        _WSSTOutInfo.u4NoWSSData   = pWssTOutInfo->u4NoWSSData;
    }
    else
    {
        Printf("Error! VBI_SetWSSTimeOutInfo: NULL Pointer\n");
    }
    
    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);        
}


/**
 * @brief    Set VBI CC TimeOut Information 
 * @param    IN: pCcTOutInfo: refer to VBI_CC_TIMEOUT_INFO structure.
 * @retval   Noon
 */
void VBI_SetCCTimeOutInfo(VBI_CC_TIMEOUT_INFO *pCcTOutInfo)
{
    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (pCcTOutInfo)
    {
        _CCTOutInfo.bEnCCTimeOut = pCcTOutInfo->bEnCCTimeOut;
        _CCTOutInfo.u1ThresHold  = pCcTOutInfo->u1ThresHold;
        _CCTOutInfo.u1NoCCData0  = pCcTOutInfo->u1NoCCData0;
        _CCTOutInfo.u1NoCCData1  = pCcTOutInfo->u1NoCCData1;
    }
    else
    {
        Printf("Error! VBI_SetCCTimeOutInfo: NULL Pointer\n");
    }
    
    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);        
}

/**
 * @brief    Set VBI Line Information 
 * @param    IN: pVbiLineInfo: refer to VBI_LINE_INFO.
 * @retval   Noon
 */
void VBI_SetLineInfo(VBI_LINE_INFO * pVbiLineInfo)
{

    if (pVbiLineInfo == NULL)
    {
        Printf("Error! VBI_SetLineInfo: NULL Pointer\n");
        return;
    }

    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (pVbiLineInfo->bEnMenuLineSet)
    {
        x_memcpy(&_VbiLineInfo, pVbiLineInfo, sizeof (VBI_LINE_INFO));
    }
    else
    {
        //Reset to Original setting.
        VBI_LINE_INFO VbiLineInfo =
        {
            FALSE,                                                                     /*bEnMenuLineSet*/
            {.u1F0_SET = AV_NML_NTSC_CC_SEL0, .u1F1_SET = AV_NML_NTSC_CC_SEL1},        /*AV_NTSC_CC*/
            {.u1F0_SET = AV_NML_PAL_CC_SEL0,  .u1F1_SET = AV_NML_PAL_CC_SEL1},         /*AV_PAL_CC*/
            {.u1F0_SET = AV_NML_NTSC_WSS_SEL0,.u1F1_SET = AV_NML_NTSC_WSS_SEL1},       /*AV_NTSC_WSS*/
            {.u1F0_SET = AV_NML_PAL_WSS_SEL0, .u1F1_SET = AV_NML_PAL_WSS_SEL1},        /*AV_PAL_WSS*/
            {.u1F0_SET = AV_NML_PAL_VPS_SEL,  .u1F1_SET = AV_NML_PAL_VPS_SEL},         /*AV_PAL_VPS*/
            {.u1F0_START = AV_NML_PAL_TT0_START,.u1F0_END = AV_NML_PAL_TT0_END,\
             .u1F1_START = AV_NML_PAL_TT1_START,.u1F1_END = AV_NML_PAL_TT1_END},       /*AV_PAL_TT*/
            {.u1F0_START = AV_NML_NTSC_CC2X0_START,.u1F0_END = AV_NML_NTSC_CC2X0_END,\
             .u1F1_START = AV_NML_NTSC_CC2X1_START,.u1F1_END = AV_NML_NTSC_CC2X1_END}, /*AV_NTSC_CC2X*/
            {.u1F0_SET = HDTV_NTSC_CC_SEL0, .u1F1_SET = HDTV_NTSC_CC_SEL1},            /*HDTV_NTSC_CC*/
            {.u1F0_SET = HDTV_PAL_CC_SEL0,  .u1F1_SET = HDTV_PAL_CC_SEL1},             /*HDTV_PAL_CC*/
            {.u1F0_SET = HDTV_NTSC_WSS_SEL0,.u1F1_SET = HDTV_NTSC_WSS_SEL1},           /*HTTV_NTSC_WSS*/
            {.u1F0_SET = HDTV_PAL_WSS_SEL0, .u1F1_SET = HDTV_PAL_WSS_SEL1},            /*HDTV_PAL_WSS*/
            {.u1F0_SET = HDTV_NTSC_WSS_PSCAN_SEL,.u1F1_SET = HDTV_NTSC_WSS_PSCAN_SEL}, /*HTTV_NTSC_WSS_PSCAN*/
            {.u1F0_SET = HDTV_PAL_WSS_PSCAN_SEL, .u1F1_SET = HDTV_PAL_WSS_PSCAN_SEL},  /*HDTV_PAL_WSS_PSAN*/
            {.u1F0_START = HDTV_PAL_TT0_START,.u1F0_END = HDTV_PAL_TT0_END,\
             .u1F1_START = HDTV_PAL_TT1_START,.u1F1_END = HDTV_PAL_TT1_END},           /*HDTV_PAL_TT*/
        };
        x_memcpy(&_VbiLineInfo, &VbiLineInfo, sizeof (VBI_LINE_INFO));
    }

    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);        

}

/**
 * @brief    Set VBI Gain Information for AV path.
 * @param    u1Gain: UNIT: 0.25, ex: 4 = 0.25*4 = 1, Range 0~F
 * @retval   1:Success / 0:Fail
 */
BOOL VBI_AdjVBIGain(UINT8 u1Gain)
{
    UINT8 u1Src;
    UINT8 u1AV_ZC_Th = AV_ZC_TH;
    u1Src = bGetVideoDecType(_u1VBI0Path);

    if (u1Src == (UINT8)SV_VD_TVD3D)
    {
        u1AV_ZC_Th = ((u1Gain<<2) + 4)<<2;
        vHwVbiSetInitZCThr(VBI0, u1AV_ZC_Th);
        vHwVbiSetVBIGain(VBI0, u1Gain);
        return 1;
    }
    return 0;
}


/**
 * @brief    Enable/Disable CC ECC  
 * @param    u1OnOff: TRUE: Enable ECC / FALSE: Disable ECC 
 * @retval   Noon
 */
void VBI_SetCCECC(UINT8 u1OnOff)
{
    vHwVBISetCCECC(u1OnOff);
}


#ifdef CC_ATV_PVR_SUPPORT
BOOL VBI_QuryTTXOnOffStatus(void)
{
    if ((_u1TtxSuccNo[0] < TTX_SUCC_THRES) && (_u1TtxSuccNo[1] < TTX_SUCC_THRES))
        return FALSE;
    else
        return TRUE;
}

BOOL VBI_QuryCCOnOffStatus(void)
{
    return TRUE;
}

BOOL VBI_QuryWSSOnOffStatus(void)
{
    return TRUE;
}

/**
 * @brief    Enable/Disable PVR Notify 
 * @param    u1Path: SV_VP_MAIN / SV_VP_PIP
 * @param    u1OnOff: TRUE: Enable Notfiy / FALSE: Disable Notify 
 * @param    pfnCCnotify: Reserved.
 * @retval   Noon
 */
void VBI_PVRStartNotifyCCOnOff(UINT8 u1Path, UINT8 u1OnOff)
{
    VERIFY(x_sema_lock(_hVbiMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    _u1VbiPvrNotify = u1OnOff;
    
    VERIFY(x_sema_unlock(_hVbiMutex) == OSR_OK);    
}

BOOL VBI_PVRRetrieveCCData(VBI_PVR_NOTIFY_DATA *pNotifyData)
{
    LOG(1, "_u4VbiPVRCCWritePtrStart=%08X\n", _u4VbiPVRCCWritePtrStart);
    LOG(1, "_u4VbiPVRCCWritePtrEnd  =%08X\n", _u4VbiPVRCCWritePtrEnd);
    if(_u4VbiPVRCCWritePtrStart==_u4VbiPVRCCWritePtrEnd)
    {
        return FALSE;
    }
    else
    {
        pNotifyData->u1NotifyType = E_PVR_CC;
        pNotifyData->u4StartAddr = _u4VbiPVRCCWritePtrStart;
        pNotifyData->u4Size = _u1VbiPVRCCDataSz;
    }
    return TRUE;
}
BOOL VBI_PVRRetrieveAFDData(VBI_PVR_NOTIFY_DATA *pNotifyData)
{
    if(_u4VbiPVRAFDWritePtrStart==_u4VbiPVRAFDWritePtrEnd)
    {
        return FALSE;
    }
    else
    {
        pNotifyData->u1NotifyType = E_PVR_AFD;
        pNotifyData->u4StartAddr = _u4VbiPVRAFDWritePtrStart;
        pNotifyData->u4Size = _u1VbiPVRAFDDataSz;
    }
    return TRUE;
}
BOOL VBI_PVRRetrieveTTXData(VBI_PVR_NOTIFY_DATA *pNotifyData)
{
    return TRUE;
}

BOOL VBI_PVRPrintCCSEIData(VBI_PVR_CC_SEI_DATA *pData)
{
    /*
    LOG(1, "u1CountryCode:%02X\n", pData->u1CountryCode);
    LOG(1, "u2ProviderCode:%04X\n", pData->u2ProviderCode);
    LOG(1, "u4UserIdentifier:%08X\n", pData->u4UserIdentifier);
    LOG(1, "u4UserDataStartCode:%08X\n", pData->u4UserDataStartCode);
    LOG(1, "u4ATSCIdentifier:%08X\n", pData->u4ATSCIdentifier);
    LOG(1, "u1UserDataTypeCode:%02X\n", pData->u1UserDataTypeCode);
    LOG(1, "u1ProcessEMFlag:%d\n", pData->u1ProcessEMFlag);
    LOG(1, "u1ProcessCCFlag:%d\n", pData->u1ProcessCCFlag);
    LOG(1, "u1AdditionalDataFlag:%d\n", pData->u1AdditionalDataFlag);
    LOG(1, "u1CCCount:%d\n", pData->u1CCCount);
    LOG(1, "u1MarkerBits:%02X\n", pData->arCCData[0].u1MarkerBits);
    LOG(1, "u1CCValid:%d\n", pData->arCCData[0].u1CCValid);
    */
    LOG(6, "u1CCType:%d\n", pData->arCCData[0].u1CCType);
    LOG(6, "u1CCData1:0x%02x\n", pData->arCCData[0].u1CCData1);
    LOG(6, "u1CCData2:0x%02x\n", pData->arCCData[0].u1CCData2);
    //LOG(1, "u1MarkerBits:%02X\n", pData->arCCData[1].u1MarkerBits);
    //LOG(1, "u1CCValid:%d\n", pData->arCCData[1].u1CCValid);
    LOG(6, "u1CCType:%d\n", pData->arCCData[1].u1CCType);
    LOG(6, "u1CCData1:0x%02x\n", pData->arCCData[1].u1CCData1);
    LOG(6, "u1CCData2:0x%02x\n", pData->arCCData[1].u1CCData2);
    LOG(6, "====================\n\n");
    return TRUE;
}

BOOL VBI_PVRPrintAFDSEIData(VBI_PVR_AFD_SEI_DATA *pData)
{
    LOG(6, "u1CountryCode:%02X\n", pData->u1CountryCode);
    LOG(6, "u2ProviderCode:%04X\n", pData->u2ProviderCode);
    LOG(6, "u4UserIdentifier:%08X\n", pData->u4UserIdentifier);
    LOG(6, "u4UserDataStartCode:%08X\n", pData->u4UserDataStartCode);
    LOG(6, "u4AFDIdentifier:%08X\n", pData->u4AFDIdentifier);
    LOG(6, "u1Zero:%d\n", pData->u1Zero);
    LOG(6, "u1ActiveFormatFlag:%d\n", pData->u1ActiveFormatFlag);
    LOG(6, "u1Reserved1:%02X\n", pData->u1Reserved1);
    LOG(6, "u1Reserved2:%02X\n", pData->u1Reserved2);
    LOG(6, "u1ActiveFormat:%02X\n", pData->u1ActiveFormat);
    LOG(6, "====================\n\n");
    return TRUE;
}

#endif

#if VBI_CLI_SUPPORT
BOOL VBI_QueryNotifyOnOffStatus(VBI_SERVICE_T t_vbi_svs, UINT8 u1Path)
{
    BOOL bOn = FALSE;
    switch(t_vbi_svs)
    {
        case VBI_TTX_SERVICE:
            bOn = (u1Path == SV_VP_MAIN)? _u1VbiTTXMainNotify : _u1VbiTTXSubNotify; 
            break;
        case VBI_CC_SERVICE:
            bOn = (u1Path == SV_VP_MAIN)? _u1VbiCCMainNotify : _u1VbiCCSubNotify; 
            break;
        case VBI_CC2X_SERVICE:
            bOn = (u1Path == SV_VP_MAIN)? _u1VbiCC2XMainNotify : _u1VbiCC2XSubNotify; 
            break;
        case VBI_WSS525_SERVICE:
            bOn = (u1Path == SV_VP_MAIN)? _u1VbiUSWSSMainNotify : _u1VbiUSWSSSubNotify; 
            break;
        case VBI_WSS625_SERVICE:
            bOn = (u1Path == SV_VP_MAIN)? _u1VbiEUWSSMainNotify : _u1VbiEUWSSSubNotify; 
            break;
        case VBI_VPS_SERVICE:
            bOn = _u1VbiVPSCNINotify; 
            break;
        default:
            break;
    }
    return bOn;
}

void VBI_SetCLINotifyMode(UINT8 u1NfyMode)
{
    _eCliNfy_T = (CLI_NFY_T)u1NfyMode;
}

void VBI_SetCLINotifyTTXFP(NPTV_PFN_TTX_NFY pfnTTXnotify)
{
    _pfnVerTtxNotify = pfnTTXnotify;
}

void VBI_SetCLINotifyCCFP(NPTV_PFN_CC_NFY pfnCCnotify)
{
    _pfnVerCcNotify = pfnCCnotify;
}

void VBI_SetCLINotifyCC2XFP(NPTV_PFN_CC2X_NFY pfnCC2Xnotify)
{
    _pfnVerCc2XNotify = pfnCC2Xnotify;
}

void VBI_SetCLINotifyWSSFP(NPTV_PFN_WSS_NFY pfnWSSnotify)
{
    _pfnVerWssNotify = pfnWSSnotify;
}

void VBI_SetCLINotifyVPSFP(NPTV_PFN_VPS_NFY pfnVPSnotify)
{
    _pfnVerVpsNotify = pfnVPSnotify;
}

void VBI_SetCLINotifyPFN(VBI_PFN_CLI_NFY pfnCLINotify)
{
    _pfnCLINotify = pfnCLINotify;
}

void VBI_CoreRst(UINT8 bVBISuite)
{
    vHwVbiCoreReset(bVBISuite);
}

UINT32 VBI_GetLineEn(UINT8 bVBISuite)
{
    return u4HwVbiGetLineEnable(bVBISuite);
}

void VBI_SetLineEn(UINT8 bVBISuite, UINT32 u4LineEn)
{
    vHwVbiSetLineEnable(bVBISuite, u4LineEn);
}

UINT32 VBI_GetLSC(UINT8 bVBISuite)
{
    return vHwVbiGetLSC(bVBISuite);
}

void VBI_SetLSC(UINT8 bVBISuite, UINT32 u4LSC)
{
    vHwVbiSetLSC(bVBISuite, u4LSC);
}

UINT32 VBI_GetOutputMode(UINT8 bVBISuite)
{
    return u4HwVbiGetOutputMode(bVBISuite);
}

void VBI_SetOutputMode(UINT8 bVBISuite, UINT32 u4Mode)
{
    vHwVbiSetOutputMode(bVBISuite, u4Mode);
}

UINT8 VBI_GetZC(UINT8 bVBISuite)
{
    return bHwVbiGetInitZCThr(bVBISuite);
}

void VBI_SetZC(UINT8 bVBISuite, UINT32 u4ZC)
{
    vHwVbiSetInitZCThr(bVBISuite, u4ZC);
}

void VBI_SetSrc(UINT8 bVBISuite, UINT8 bSrc)
{
    vHwVbiSetSrc(bVBISuite, bSrc);
}

UINT8 VBI_GetSrc(UINT8 bVBISuite)
{
    return bHwVbiGetSrc(bVBISuite);
}

void VBI_GetTtxLine(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End)
{
    vHwVbiGetTTXLineRange(bVBISuite, u1Field, pu1Start, pu1End);
}

void VBI_SetTtxLine(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Start, UINT8 u1End)
{
    if (u1Field == 0)
        vHwVbiSetTTInterval0(bVBISuite, u1Start, u1End);
    else
        vHwVbiSetTTInterval1(bVBISuite, u1Start, u1End);    
}

void VBI_GetCcLine(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End)
{
    vHwVbiGetCCLineRange(bVBISuite, u1Field, pu1Start, pu1End);
}

void VBI_SetCcLine(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Start, UINT8 u1End)
{
    vHwVbiSetCCLineRange(bVBISuite, u1Field, u1Start, u1End);
}

void VBI_GetWssLine(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Line)
{
    vHwVbiGetWSSLineRange(bVBISuite, u1Field, pu1Line);
}

void VBI_SetWssLine(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Line)
{
    vHwVbiSetWSSLineRange(bVBISuite, u1Field, u1Line);
}

void VBI_GetVpsLine(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End)
{
    vHwVbiGetVPSLineRange(bVBISuite, u1Field, pu1Start, pu1End);
}

void VBI_SetVpsLine(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Start, UINT8 u1End)
{
    vHwVbiSetVPSLineRange(bVBISuite, u1Field, u1Start, u1End);
}

void VBI_SetDbgPort(UINT32 u4Mode)
{
    vHwVbiSetDbgPort(u4Mode);
}

UINT32 VBI_GetMbase0(UINT8 bVBISuite)
{
    return u4HwVbiGetMbase0(bVBISuite);
}

UINT32 VBI_GetMOffset(UINT8 bVBISuite)
{
    return u4HwVbiGetMOffset(bVBISuite);
}

VBI_MODE_T VBI_GetVBIMode(void)
{
    return _t_VbiMode;
}

#if AUTO_CC_DETECT
void VBI_AutoCCOnOff(UINT8 u1OnOff)
{
    ui1AutoCCEnable = u1OnOff;
}
#endif

void VBI_DramDumpOnOff(UINT8 u1OnOff)
{
    _fgDramDumpEn = u1OnOff;
}

#if SUPPORT_TOFU_DISPLAY
void VBI_CCToFuOnOff(UINT8 u1OnOff, UINT8 u1LoTh, UINT8 u1HiTh)
{
    _bEnableTofuDisplay = u1OnOff;
    _u1TofuLoTh = u1LoTh;
    _u1TofuHiTh = u1HiTh;
    return;
}
#endif

#endif //End of VBI_CLI_SUPPORT

#endif
