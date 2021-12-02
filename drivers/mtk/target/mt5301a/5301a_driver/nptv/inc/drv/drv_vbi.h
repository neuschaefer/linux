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
 * $RCSfile: drv_vbi.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _DRV_VBI_H_
#define _DRV_VBI_H_

#include "typedef.h"
#include "nptv_if.h"
#include "hw_vbi.h"


#if AUTO_CC_DETECT
#define SCANLINE_CC_RETRY   10
#define SCANLINE_CC_NUM 4
#define SCANLINE_CC_TESTLOOP    20
#define SCANLINE_CC_TH    8
#define AUTO_CC_NOT_DEFULT_TH 3
typedef enum
{
    SCANLINE_IDLE = 0,
    SCANLINE_INIT,
    SCANLINE_START,
    SCANLINE_FINISH,
    SCANLINE_NOT_FOUND
} SCANLINE_STATE_T;
#endif


/* VBI working modes */
typedef enum
{
    VBI_CC_VCHIP_MODE = 0,
    VBI_TTX_MODE      = 1,
    VBI_ANAS_MODE     = 2,
    VBI_WSS_ONLY_MODE = 3,
    VBI_MODE_NUM
}VBI_MODE_T;

typedef enum
{
    VBI_TTX_SERVICE = 0,
    VBI_CC_SERVICE,
    VBI_CC2X_SERVICE,
    VBI_WSS525_SERVICE,
    VBI_WSS625_SERVICE,
    VBI_VPS_SERVICE,
    VBI_SERVICE_NUM
}VBI_SERVICE_T;

typedef enum
{
    E_FIELD_NORMAL = 0,
    E_FIELD_INVERSED, 
    E_FIELD_UNKNOW,
}AV_FIELD_TYPE;

typedef enum
{
    E_AV_NTSC = 0,
    E_AV_PAL,
    E_AV_UNKNOW,
}AV_SRC_TYPE;

typedef enum
{
    CLINfy_Disable = 0,
    CLINfy_EnOnly  = 1,  //Turn On CLI Nfy Path Only
    CLINfy_EnAll   = 2,  //Turn On MW Nfy Path & CLI Nfy Path
}CLI_NFY_T;

typedef struct
{
    AV_FIELD_TYPE E_AVFieldType;
    AV_SRC_TYPE E_AVSrcType;
}__attribute__ ((packed)) VBI_AV_INFO;

typedef struct
{
    BOOL   bEnWSS525TOut; //Enable/Disable WSS525TimeOut Mechanism 
    BOOL   bEnWSS625TOut; //Enable/Disable WSS625TimeOut mechanism
    UINT8  u1ThresHold;   //Not Detected for last X(ThresHold) consecutive frames, 
    UINT32 u4NoWSSData;   //Notifyed u4NOWSSData to upperlayer.
}__attribute__ ((packed)) VBI_WSS_TIMEOUT_INFO;


typedef struct
{
    BOOL   bEnCCTimeOut;  //Enable/Disable CC TimeOut mechanism
    UINT8  u1ThresHold;   //Not Detected for last X(ThresHold) consecutive frames, 
    UINT8  u1NoCCData0;   //Notifyed u1NoCCData0 to upperlayer. 
    UINT8  u1NoCCData1;   //Notifyed u1NoCCData1 to upperlayer. 
}__attribute__ ((packed)) VBI_CC_TIMEOUT_INFO;


typedef struct
{
    UINT8 u1F0_SET;  // Filed0 Line Setting.
    UINT8 u1F1_SET;  // Field1 Line Setting.
}__attribute__ ((packed)) LINE;

typedef struct
{
    UINT8 u1F0_START; // Field0 Start Setting. 
    UINT8 u1F0_END;   // Field0 end Setting. 
    UINT8 u1F1_START; // Field1 Start Setting. 
    UINT8 u1F1_END;   // Field1 end Setting. 
}__attribute__ ((packed)) LINE_RANGE;


typedef struct
{
    BOOL bEnMenuLineSet;
    LINE AV_NTSC_CC;
    LINE AV_PAL_CC;
    LINE AV_NTSC_WSS;
    LINE AV_PAL_WSS;
    LINE AV_PAL_VPS;
    LINE_RANGE AV_PAL_TT;
    LINE_RANGE AV_NTSC_CC2X;
    LINE HDTV_NTSC_CC;
    LINE HDTV_PAL_CC;
    LINE HDTV_NTSC_WSS;
    LINE HDTV_PAL_WSS;      
    LINE HDTV_NTSC_WSS_PSCAN; //480P CGMS WSS
    LINE HDTV_PAL_WSS_PSAN;   //576P CGMS WSS
    LINE_RANGE HDTV_PAL_TT;
}__attribute__ ((packed)) VBI_LINE_INFO;


/** Function prototype for CLI cmd. */
typedef void (*VBI_PFN_CLI_NFY) (void *pInput);

/* VBI0 is with Composite for weak signal/eye-height processing */
/* VBI2 is with Component for there is no weak signal issue */

/* Two VBI slicers */
#define VBI0 1
#define VBI2 2

/* FIFO mode */
#define CC_FIFO		0
#define TTX_FIFO    1

/* CC2X Config */
#define SUPPORT_CLOSE_CAPTION_2X 0
#define EN_TVE_CB                1

/* Initialize VBI slicer parameters */
EXTERN void VBI_Init(VBI_MODE_T t_VbiMode);

#ifdef CC_MT5363
EXTERN void VBIISR_Init(void);
#else
/* VBI Interrupt Service Routine */
EXTERN void VBI_ISR(void);
#endif

/* Enable VBI slicer */
EXTERN void VBI_Enable(void);

/* Disable VBI slicer */
EXTERN void VBI_Disable(void);

/* Disabe VBI slicer individually */
EXTERN void VBI_DisableBySlicer(UINT8 u1VBISuite);

/* Reset VBI setting in mode change */
EXTERN void VBI_ResetSrc(UINT8 u1Path);

/* Qeery VBI AV INFO */
EXTERN void VBI_QueryAVInfo(VBI_AV_INFO *pVBIAvInfo);

/* Set VBI AV Filed INFO */
EXTERN void VBI_SetAVFieldType(BOOL bIs525, BOOL bIsFieldIvs, UINT8 u1Mode);

/* Set VBI WSS TIME OUT INFO */
EXTERN void VBI_SetWSSTimeOutInfo(VBI_WSS_TIMEOUT_INFO *pWssTOutInfo);

/* Set VBI CC TIME OUT INFO */
EXTERN void VBI_SetCCTimeOutInfo(VBI_CC_TIMEOUT_INFO *pCcTOutInfo);

/* Set VBI LINE INFO */
EXTERN void VBI_SetLineInfo(VBI_LINE_INFO * pVbiLineInfo);

/* Set VBI Slicer Path */
EXTERN void VBI_SetSlicerPath(UINT8 u1VBISuite, UINT8 u1Path);

/* Get VBI Slicer Path */
EXTERN UINT8 VBI_GetSlicerPath(UINT8 u1VBISuite);

/* Set VBI ECC OnOff */
EXTERN void VBI_SetCCECC(UINT8 u1OnOff);

#if VBI_CLI_SUPPORT
EXTERN BOOL VBI_QueryNotifyOnOffStatus(VBI_SERVICE_T t_vbi_svs, UINT8 u1Path);
EXTERN void VBI_SetCLINotifyTTXFP(NPTV_PFN_TTX_NFY pfnTTXnotify);
EXTERN void VBI_SetCLINotifyCCFP(NPTV_PFN_CC_NFY pfnCCnotify);
EXTERN void VBI_SetCLINotifyCC2XFP(NPTV_PFN_CC2X_NFY pfnCC2Xnotify);
EXTERN void VBI_SetCLINotifyWSSFP(NPTV_PFN_WSS_NFY pfnWSSnotify);
EXTERN void VBI_SetCLINotifyVPSFP(NPTV_PFN_VPS_NFY pfnVPSnotify);
EXTERN void VBI_SetCLINotifyPFN(VBI_PFN_CLI_NFY pfnCLINotify);
EXTERN void VBI_CoreRst(UINT8 bVBISuite);
EXTERN UINT32 VBI_GetLineEn(UINT8 bVBISuite);
EXTERN void VBI_SetLineEn(UINT8 bVBISuite, UINT32 u4LineEn);
EXTERN UINT32 VBI_GetLSC(UINT8 bVBISuite);
EXTERN void VBI_SetLSC(UINT8 bVBISuite, UINT32 u4LSC);
EXTERN UINT8 VBI_GetSrc(UINT8 bVBISuite);
EXTERN void VBI_SetSrc(UINT8 bVBISuite, UINT8 bSrc);
EXTERN UINT32 VBI_GetOutputMode(UINT8 bVBISuite);
EXTERN void VBI_SetOutputMode(UINT8 bVBISuite, UINT32 u4Mode);
EXTERN UINT8 VBI_GetZC(UINT8 bVBISuite);
EXTERN void VBI_SetZC(UINT8 bVBISuite, UINT32 u4ZC);
EXTERN void VBI_GetCcLine(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1CcStart, UINT8* pu1CcEnd);
EXTERN void VBI_SetCcLine(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1CcStart, UINT8 u1CcEnd);
EXTERN void VBI_GetWssLine(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Line);
EXTERN void VBI_SetWssLine(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Line);
EXTERN void VBI_GetTtxLine(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End);
EXTERN void VBI_SetTtxLine(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Start, UINT8 u1End);
EXTERN void VBI_GetVpsLine(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End);
EXTERN void VBI_SetVpsLine(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Start, UINT8 u1End);
EXTERN void VBI_SetDbgPort(UINT32 u4Mode);
EXTERN UINT32 VBI_GetMbase0(UINT8 bVBISuite);
EXTERN UINT32 VBI_GetMOffset(UINT8 bVBISuite);
EXTERN VBI_MODE_T VBI_GetVBIMode(void);
EXTERN void VBI_SetCLINotifyMode(UINT8 u1NfyMode);
EXTERN BOOL VBI_AdjVBIGain(UINT8 u1Gain);
#if AUTO_CC_DETECT
EXTERN void VBI_AutoCCOnOff(UINT8 u1OnOff);
#endif
EXTERN void VBI_DramDumpOnOff(UINT8 u1OnOff);
#endif // end of VBI_CLI_SUPPORT

#endif

