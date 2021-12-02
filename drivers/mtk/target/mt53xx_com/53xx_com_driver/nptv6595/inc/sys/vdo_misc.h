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
 *   $Workfile: vdo_misc.h $
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

#ifndef _VDO_MISC_H_
#define _VDO_MISC_H_

#include "hw_sw.h"


#define MT8280 0
#define MT8280_E1 1
//extern UINT8 bGetICVersion(void);
//extern UINT8 _bExtmjcICVersion;

extern void vDrvVideoSwInit(void) ;
extern void vDrvVideoHwInit(void) ;
extern UINT8 bIsScalerInput444(UINT8 bPath) reentrant;

extern UINT8 bDrvVideoGetType(UINT8 bPath) ;
#define VDOTYPE_NTSC 0
#define VDOTYPE_PAL 1
#define VDOTYPE_OTHER 2
#ifndef CC_UP8032_ATV
extern void vDrvVideoAllocateDram(void) ;
#else
void vDrvVideoSRMChk(UINT8 bPath);
#endif

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
UINT8 bDrvVideoSignalStatus_DBL(UINT8 bPath);
UINT8 bDrvVideoSignalStatus(UINT8 bPath)   ;
UINT16 wDrvVideoGetHTotal(UINT8 bPath) ;
UINT16 wDrvVideoGetVTotal(UINT8 bPath) ;
UINT8 bDrvVideoGetTiming(UINT8 bPath) ;
const CHAR* strDrvVideoGetTimingString(UINT8 bTiming);
UINT8 bDrvVideoGetWidthOverSample(UINT8 bPath) ;

extern UINT8 bDrvVideoIsMainSrcTV(void);
extern UINT8 bDrvVideoGetSourceTypeTiming(UINT8 bPath) reentrant;
extern UINT16 wDrvVideoInputDEWidth(UINT8 bPath);
extern UINT16 bDrvVideoGetFrameDelay(UINT8 bPath);

extern UINT8 bDrvPorchTune[2]; //porch tune mode
extern UINT8 bDrvGetPorchTune(UINT8 bPath);
extern void vDrvSetPorchTune(UINT8 bPath, UINT8 bModemsk);

//#define bDrvGetPorchTune(bPath)			bDrvPorchTune[bPath]
//#define vDrvSetPorchTune(bPath,bModemsk)	(bDrvPorchTune[bPath]=(bModemsk))

extern UINT16 wDrvVideoPorchStd(UINT8 bPath, UINT8 bPorchType);
extern UINT16 wDrvVideoGetPorch(UINT8 bPath, UINT8 bPorchType);
extern UINT16 wDrvVideoSetPorch(UINT8 bPath, UINT8 bPorchType, UINT16 wValue);
#if SUPPORT_SET_SLICER
extern UINT16 wDrvSOGSetSlicer(UINT8 bPath, UINT16 wValue);//wu add
#endif

extern UINT16 wTvdGetPorch(UINT8 bPath, UINT8 bPorchType); /* bPath is for wDrvVideoPorchStd()*/
extern UINT16 wVgaGetPorch(UINT8 bPath, UINT8 bPorchType);
extern UINT16 wHdtvGetPorch(UINT8 bPath, UINT8 bPorchType);
extern UINT16 wDviGetPorch(UINT8 bPath, UINT8 bPorchType);
extern UINT8 u1DrvDIGetBob (UINT8 u1VdpId);
//extern void vVDOIN_MPCLK_Setting(void);
//extern void vTvdSetPorch(UINT8 bPath,UINT8 bPorchType, UINT16 wValue);
extern void vVgaSetPorch(UINT8 bPath,UINT8 bPorchType, UINT16 wValue);
extern void vHdtvSetPorch(UINT8 bPath,UINT8 bPorchType, UINT16 wValue);
extern void vDviSetPorch(UINT8 bPath,UINT8 bPorchType, UINT16 wValue);
extern void vDrvMainSubFifoReset(UINT8 bPath);

#if 0
// DXX_HV_POS
UINT16 wTVDecGetHPos(UINT8 bPath);
UINT16 wTVDecAdjHPos(UINT8 bPath, UINT8 dir, UINT16 value);
UINT16 wTVDecGetVPos(UINT8 bPath);
UINT16 wTVDecAdjVPos(UINT8 bPath, UINT8 dir, UINT16 value);
INT16 wDrvAdjHPos(UINT8 bPath, UINT8 dir, UINT16 value);
INT16 wDrvAdjVPos(UINT8 bPath, UINT8 dir, UINT16 value);
#endif
//extern void vApiRedoTwoChipConnection(BOOL fgEnable, UINT8 u1Path);
extern UINT8 bDrvVideoGetWidthOverSample(UINT8 bPath);
extern UINT32 u4DrvVideoGetNSDOffset(UINT8 bPath);

UINT8 u1GetVGA422En(void);
void vSetVGA422En(UINT8 bOnOff);



typedef struct _RegDwrdTbl
{
	UINT16 wReg;
	UINT32 dwValue;
	UINT32 dwMask;
}RRegDwrdTbl;

#define REG_END	0xFFFF

void vDrvLoadRegDwrdTbl(RRegDwrdTbl const * pRegDwrdTbl) reentrant;
#ifdef  CC_SCPOS_PATTERN_GENERATOR
void vSetScposPatternGen(UINT8 u1Enable);
UINT8 u1GetScposPtGenEnable(void); 
#endif

UINT32 u43DGetInputLR(void);
UINT32 u43DGetOutputLR(void);

#endif
