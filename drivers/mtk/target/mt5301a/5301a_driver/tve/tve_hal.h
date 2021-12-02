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
#ifndef _TVE_HAL_5351_H_
#define _TVE_HAL_5351_H_

#include "x_typedef.h"
#include "u_common.h"

/******************************************************************************
* Defines
******************************************************************************/
#define CGMSA_COPY_CTL_INFO         0xC3FFF
#define CGMSA_COPY_CTL_DEFAULT      0x3C000

#define TVE_TTX_BUF_LINENUM 18
#define TVE_TTX_BUF_DATAPERLINE 12 //dobule word  // 4 control + 42 data + 2 dummy; control by tt_bufr_width 
#define TVE_TTX_BUF_SIZE (TVE_TTX_BUF_LINENUM*TVE_TTX_BUF_DATAPERLINE) // 4bytes/unit

/******************************************************************************
* Typedef
******************************************************************************/
typedef struct _TVE_CC_DATA
{
    PTS_T u8Pts;
    UCHAR ucData1;
    UCHAR ucData2;
    UCHAR ucField;
} TVE_CC_DATA_T;

typedef struct _TVE_TTX_DATA
{
    UCHAR ucField;
    UINT32 __attribute__((aligned(16))) ucData[TVE_TTX_BUF_SIZE];
} TVE_TTX_DATA_T;

typedef struct _TTX_DATA_FIELD
{
    UCHAR field_parity;
    UCHAR line_offset;
    UCHAR framing_code;
    UCHAR txt_data_block[42];
} TTX_DATA_FIELD_T;

typedef struct _TVE_ENUM2TEXT_T
{
    UCHAR ucValue;
	CHAR *szText;
} TVE_ENUM2TEXT_T;

typedef struct _TVE_RegDwrdTbl
{
	UINT16 wReg;
	UINT32 dwValue;
}TVE_RRegDwrdTbl;

enum
{
    DAC_1 = 0,
    DAC_2, 
    DAC_MAX
};

enum
{
    TVE_Mixer1 = 0,
    TVE_Mixer2, 
    TVE_Mixer_MAX
};

enum
{
    CVBS_ADC = 0,
    DEMOD_ADC, 
    ADC_MAX
};

//TVE vbi type
enum
{
    TVE_VBI_DATA_CC,
    TVE_VBI_DATA_XDS, ///< Extended Data Service
    TVE_VBI_DATA_TTX, ///< Analog SdUserData is Teletext
    TVE_VBI_DATA_WSS, ///< Analog SdUserData is WSS	
    TVE_VBI_DATA_VPS, ///< Analog SdUserData is VPS - included in NTSC line 21 XDS
    TVE_VBI_DATA_PDC, ///< Analog SdUserData is PDC
    TVE_VBI_DATA_MAX
};


/******************************************************************************
* API
******************************************************************************/
extern void TVE_HalEnable(UCHAR ucTveId);
extern void TVE_HalDisable(UCHAR ucTveId);
extern void TVE_HalReset(UCHAR ucTveId);
extern void TVE_HalColorBar(UCHAR ucTveId, UCHAR ucOn);
extern void TVE_HalOSDpath(UCHAR ucTveId, UCHAR ucOn);
extern void TVE_HalSetMode(UCHAR ucTveId, UCHAR ucFmt, UCHAR ucMode);
extern void TVE_HalSetFmt(UCHAR ucTveId, UCHAR ucFmt, UCHAR ucMode);
extern void TVE_HalSetFmt_VerA(UCHAR ucTveId, UCHAR ucFmt, UCHAR ucMode);
extern void TVE_HalSetFmt_VerB(UCHAR ucTveId, UCHAR ucFmt, UCHAR ucMode);
extern void TVE_HalSetImpedence(UCHAR ucTveId, UCHAR ucImpedence);
extern void TVE_HalCcInit(UCHAR ucTveId);
extern void TVE_HalEnableCc(UCHAR ucTveId, UCHAR ucEnable);
extern UCHAR TVE_HalSend2CcQue(UCHAR ucTveId, UCHAR ucField, const UCHAR *pucData, PTS_T u8Pts);
extern void TVE_HalSendCcIsr(UCHAR ucTveId, UCHAR ucIsOddField, UINT32 u4PmxPts);
extern UINT32 TVE_HalEnableMv(UCHAR ucTveId);
extern UINT32 TVE_HalDisableMv(UCHAR ucTveId);
extern UINT32 TVE_HalWriteMvValue(UCHAR ucTveId, UINT32 u4MvReg, UINT32 u4Value);
extern UINT32 TVE_HalReadMvValues(UCHAR ucTveId);
#if 0
extern UINT32 TVE_HalMvTest(UCHAR ucTveId, UCHAR ucCaseNum);
#endif
extern UINT32 TVE_HalEnableWss(UCHAR ucTveId, UCHAR ucTveFmt);
extern UINT32 TVE_HalDisableWss(UCHAR ucTveId, UCHAR ucTveFmt);
extern UINT32 TVE_HalSetAspectRatio(UCHAR ucTveId, UCHAR ucTveFmt,
                                    UCHAR ucAspectRatio);
extern UINT32 TVE_HalSetCgmsaValue(UCHAR ucTveId, UCHAR ucTveFmt, UCHAR ucValue);
#ifdef CC_MT5301A
extern UINT8 TVE_HalDACPowerOn(UCHAR ucTveId, UCHAR ucDAC, UCHAR ucOn);
#else
extern UINT32 TVE_HalDACPowerOn(UCHAR ucTveId, UCHAR DAC, UCHAR ucOn);
#endif
extern UINT32 TVE_HalResetSyncEn(UCHAR ucTveId);

extern UINT32 TVE_HalResetCcFifo(UCHAR ucTveId);
extern void TVE_HalResetVChip(void);

extern UINT32 TVE_HalSrcSelect(UCHAR ucTveSrc);
extern void Bypass_HalClear2Default(void);
extern void Bypass_HalSetCommon(void);
extern void Bypass_HalSrcTestPattern(void);
extern UINT32 TVE_HalGetFieldInfo(UCHAR ucTveId);
extern void TVE_HalEnableTTX(UCHAR ucTveId, UCHAR ucEnable);
extern UCHAR TVE_HalSend2TTXQue(UCHAR ucTveId, const UCHAR *pucData, UINT32 length);
extern void TVE_HalSendTTXIsr(UCHAR ucTveId, UCHAR ucIsOddField, UINT32 u4PmxPts);
extern UINT32 TVE_HalSetWSSValue(UCHAR ucTveId, UCHAR ucTveFmt, const UCHAR *pucData);

extern void Bypass_HalPIPClk(UCHAR ucOn);
extern void Bypass_HalSrcBypass(UCHAR DAC, UCHAR bSrc);
extern void Bypass_HalSrcPIP(UCHAR DAC, UCHAR bSrc);
extern UINT32 Bypass_HalTVEMode(UCHAR bSrc);
extern void Bypass_HalSrcVdoin(UCHAR DAC, UCHAR bSrc);
extern void Bypass_HalTVESyncTime(UCHAR bSrc, UCHAR ucFmt);
extern UINT32 TVE_HalSetInputSrc(UCHAR* bSrc);
#if defined(CC_MT5363) || defined(CC_TVE6595)
extern UINT32 TVE_HalPIPMixerOn(UCHAR ucTveId, UCHAR ucOn);
#endif
#endif  /* _TVE_HAL_5351_H_ */


