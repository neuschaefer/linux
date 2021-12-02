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

#ifndef _DRV_TVE_H_
#define _DRV_TVE_H_

#include "x_typedef.h"
#if 0	// johnny mark  20080320
#include "drv_pmx.h"
#endif	// johnny mark  20080320
#include "x_os.h"       /* using x_memset() */

/* Maximum number of plane mixer */
#define TVE_1				0	/* Main display */
#define TVE_2				1	/* Aux display */
#define TVE_MAX_NS			2

/* TVE configuration return value */
#define TVE_SET_ERROR		0
#define TVE_SET_OK			1
#define TVE_SET_VBI_OVERFLOW	2

/* TVE output */
#define TVE_OUT_CVBS		0
#define TVE_OUT_S_VIDEO	    1
#define TVE_OUT_YBR			2
#define TVE_OUT_RGB			3
#define TVE_MAX_OUT			4

/* TVE mode */
#define TVE_MOD_COMPOSITE		0
#define TVE_MOD_COMPONENT		1
#define TVE_MOD_RGB			    2
#define TVE_MOD_VGA             TVE_MOD_RGB
#define TVE_MAX_MOD			    3

/* TVE format */

#if 1	// johnny add  20080320
// Plane mixer mode
#define PMX_MODE_480I			0
#define PMX_MODE_576I			1
#define PMX_MODE_480P			2
#define PMX_MODE_576P			3
#define PMX_MODE_720P			4
#define PMX_MODE_1080I			5

#define PMX_MODE_768P			6
#define PMX_MODE_540P			7
#define PMX_MODE_768I			8
#define PMX_MODE_1536I			9
#define PMX_MODE_720I			10
#define PMX_MODE_1440I			11
#define PMX_MODE_UNKNOWN		12
#endif 

#define TVE_FMT_480I                    PMX_MODE_480I
#define TVE_FMT_576I                    PMX_MODE_576I
#define TVE_FMT_480P                    PMX_MODE_480P
#define TVE_FMT_576P                    PMX_MODE_576P
#define TVE_FMT_720P                    PMX_MODE_720P
#define TVE_FMT_1080I                   PMX_MODE_1080I
#define TVE_FMT_768P                    PMX_MODE_768P
#define TVE_MAX_FMT                     2

/* TVE field */
#define TVE_FIELD_ODD			0
#define TVE_FIELD_EVEN			1
#define TVE_FIELD_BOTH			2

/* TVE Macrovision APS types */
#define TVE_MV_APS_Type0		0
#define TVE_MV_APS_Type1		1
#define TVE_MV_APS_Type2		2
#define TVE_MV_APS_Type3		3
#define TVE_MAX_MV_APS			4

#define TVE_MAX_MV_TEST_CASES   6

/* TVE Low/High Impedence */
#define TVE_LOW_IMPEDENCE       0   /* for MT5351_DEMO_BOARD */
#define TVE_HIGH_IMPEDENCE      1   /* for MT5351_M1_BOARD */

/* TVE WSS/CGMS Aspect Ratio
   NOR: Normal, LB: LetterBox, C: Center, T: Top, LG: Larger than
   525-Line (CGMS) */
#define CGMS_NOR_4_3			0
#define CGMS_NOR_16_9			1
#define CGMS_LB_4_3             2
#define CGMS_LB_16_9			3
/* 625-Line (WSS)  */
#define WSS_LB_14_9_C			1
#define WSS_LB_14_9_T			2
#define WSS_LB_16_9_T			4
#define WSS_ANA_16_9			7
#define WSS_FULL_4_3			8
#define WSS_LB_16_9_C			11
#define WSS_LB_LG_16_9_C		13
#define WSS_FULL_14_9_C			14

/* TVE Mixer select */
#define TVE_VDOIN_MIXER	0
#define TVE_DEMOD_MIXER	1
#ifdef CC_MT5363
#define TVE_SIF_MIXER		2
#endif

//TVE field invert WA
//#ifndef CC_MT5363
#define TVE_WA //mt5387
//#else
//#endif

/******************************************************************************
* TVE API
******************************************************************************/
extern UINT32 TVE_Init(void);
extern UINT32 TVE_UnInit(void);
extern UINT32 TVE_Reset(UCHAR ucTveId);
extern UINT32 TVE_QueryStatus(void);

extern UINT32 TVE_SetEnable(UCHAR ucTveId, UCHAR ucEnable);
extern UINT32 TVE_GetEnable(UCHAR ucTveId, UCHAR* pucEnable);

extern UINT32 TVE_SetMode(UCHAR ucTveId, UCHAR ucMode);
extern UINT32 TVE_GetMode(UCHAR ucTveId, UCHAR* pucMode);

extern UINT32 TVE_SetFmt(UCHAR ucTveId, UCHAR ucFmt);
extern UINT32 TVE_GetFmt(UCHAR ucTveId, UCHAR* pucFmt);

extern UINT32 TVE_SetImpedence(UCHAR ucTveId, UCHAR ucImpedence);
extern UINT32 TVE_GetImpedence(UCHAR ucTveId, UCHAR* pucImpedence);

extern UINT32 TVE_SetColorBar(UCHAR ucTveId, UCHAR ucOn);
extern UINT32 TVE_SetCcEnable(UCHAR ucTveId, UCHAR ucEnable);
extern UINT32 TVE_GetCcEnable(UCHAR ucTveId, UCHAR* pucCcEnable);
extern UINT32 TVE_SendCc(UCHAR ucTveId, UCHAR ucField, const UCHAR *pucData, PTS_T u8Pts);
extern UINT32 TVE_SendCcIsr(UCHAR ucTveId, UCHAR ucIsOddField, UINT32 u4PmxPts);
/*
extern UINT32 TVE_SetMvEnable(UCHAR ucTveId, UCHAR ucEnable);
extern UINT32 TVE_GetMvEnable(UCHAR ucTveId, UCHAR* pucMvEnable);
extern UINT32 TVE_WriteMvValue(UCHAR ucTveId, UINT32 u4MvReg, UINT32 u4Value);
extern UINT32 TVE_ReadMvValues(UCHAR ucTveId);
extern UINT32 TVE_MvTest(UCHAR ucTveId, UCHAR ucCaseNum);
extern UINT32 TVE_GetMvTest(UCHAR ucTveId, UCHAR* pucValue);
*/
extern UINT32 TVE_SetWssEnable(UCHAR ucTveId, UCHAR ucEnable);
extern UINT32 TVE_GetWssEnable(UCHAR ucTveId, UCHAR* pucWssEnable);
extern UINT32 TVE_SendWss(UCHAR ucTveId,const UCHAR *pu1Buf, UINT32 length);
extern UINT32 TVE_SetAspectRatio(UCHAR ucTveId, UCHAR ucAspectRatio);
extern UINT32 TVE_GetAspectRatio(UCHAR ucTveId, UCHAR* pucAspectRatio);
extern UINT32 TVE_SetCgmsaValue(UCHAR ucTveId, UCHAR ucValue);
extern UINT32 TVE_GetCgmsaValue(UCHAR ucTveId, UCHAR* pucValue);

extern UINT32 TVE_PlayCc(UCHAR ucTveId, UCHAR ucEsId);
extern UINT32 TVE_StopCc(UCHAR ucTveId, UCHAR ucEsId);
extern UINT32 TVE_SetTTXEnable(UCHAR ucTveId, UCHAR ucEnable);
extern UINT32 TVE_GetTTXEnable(UCHAR ucTveId, UCHAR* pucTTXEnable);
extern UINT32 TVE_SendTTX(UCHAR ucTveId,const UCHAR *pu1Buf, UINT32 length);

extern UINT32 TVE_DACPower(UCHAR ucTveId, UCHAR DAC, UCHAR ucEnable);
extern UINT32 TVE_OnInputVSync(void);

extern void Bypass_Init(void);
extern UINT32 TVE_GetFieldInfo(UCHAR ucTveId);
extern void  _TVE_Mainloop(void);
#if 0
extern UINT32 AuxOutConnect(UCHAR ucDACid, UCHAR ucDACSrc);
extern UINT32 AuxOutDisconnect(UCHAR ucDACid, UCHAR ucDACSrc);
extern UINT32 AuxOut_VideoStart(UCHAR ucDACid);
extern UINT32 AuxOut_VideoStop(UCHAR ucDACid);
extern UINT32 AuxOut_VideoMute(UCHAR ucDACid, UCHAR ucOn);
extern UINT32 AuxOut_SendVbiData(UCHAR ucDACid, UCHAR eType, UCHAR *pu1Buf, UINT32 length);
extern UINT32 AuxOut_VbiEnable(UCHAR ucDACid, UCHAR eType, UCHAR ucOn);
#endif
extern UINT32 TVE_SelectInput(UCHAR ucTveSrc);
extern UINT32 TVE_OSDpath(UCHAR ucTveId, UCHAR ucOn);
#ifdef CC_MT5363
extern UINT32 TVE_PIPMixer(UCHAR ucTveId, UCHAR ucEnable);
#endif
#endif /* _DRV_TVE_H_ */

