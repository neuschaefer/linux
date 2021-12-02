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

#ifndef _TVE_CTRL_H_
#define _TVE_CTRL_H_

#include "x_typedef.h"
#include "u_common.h"

/* TVE OUT Config */
enum
{
	SV_TVE_NA,
	SV_TVE_DAC_BYPASS_CVBS,		
	SV_TVE_DAC_BYPASS_DEMOD,
	SV_TVE_DAC_PIP,
	SV_TVE_DAC_VIDEO_IN, // Video In
	SV_TVE_HW_BYPASS, // Only for TV SCART
	SV_TVE_D2S_BYPASS, // Only for Monitor SCART
#ifdef CC_MT5363	
	SV_TVE_DAC_BYPASS_SIF,
#endif
	SV_TVE_MAX
};

/* TVE Src Type */
enum
{
	SV_TVE_SRC_ORI,
	SV_TVE_SRC_CVBS0,
	SV_TVE_SRC_CVBS1,
	SV_TVE_SRC_PREV,	
	SV_TVE_SRC_MAX
};

/* TVE Video Status Notify Type */
enum
{	
	SV_TVE_NOTIFY_VIDEO_NO_SIGNAL,
	SV_TVE_NOTIFY_VIDEO_STABLE_SIGNAL,		
	SV_TVE_NOTIFY_VIDEO_MODE_CHANGE,
	SV_TVE_NOTIFY_VIDEO_UNMUTE,
	SV_TVE_NOTIFY_MAX
};

/* Tuner Type */
enum
{
	SV_TVE_TUNER_ATV_DEFAULT,
	SV_TVE_TUNER_ATV,
	SV_TVE_TUNER_DTV,
	SV_TVE_TUNER_MAX,
        SV_TVE_TUNER_LAST_TYPE = SV_TVE_TUNER_MAX
};

/* Scart Type */
enum
{
	SV_TVE_SCART_DAC,
	SV_TVE_SCART_HW,
	SV_TVE_SCART_D2S,
	SV_TVE_SCART_MAX
};

/* Scart Out ID */
enum
{
	SV_TVE_SCART_OUT_1,
	SV_TVE_SCART_OUT_2,
	SV_TVE_SCART_OUT_MAX
};

/* Scart Out Fmt */
enum
{
	SV_TVE_OFMT_PAL,
	SV_TVE_OFMT_NTSC,
	SV_TVE_OFMT_MAX,
	SV_TVE_OFMT_NONE = SV_TVE_OFMT_MAX
};

/* SCART Src type */
#define TVE_SCART_DECODER 7

typedef struct {
	UINT8  type;
	UINT8  src;
	UINT8  reset_src_change;		
}  rtve_out_t;

typedef struct {
	rtve_out_t tve_out1;
	rtve_out_t tve_out2;	
       UINT8 tve_out_fmt_576i;	
#ifdef CC_SUPPORT_TVE_CTRL_BY_DRIVER_SLT
       UINT8 remap_msrc;
#endif	
}  rtve_config_t;

typedef struct TveUsage_s
{
    UINT8    TveIsUsedBy : 4;
    UINT8    TveReferenceCnt : 4;
} TveUsage_t;

#define TVE_DAC_MUTE_MASK_MW  (1)
#define TVE_DAC_MUTE_MASK_DRV (1<<1)

#define TVE_BYP_EXT_PATH   (1)
#define TVE_BYP_AVMUX_PATH (2)
#define TVE_BYP_PIP_PATH   (3)

#define TVE_BYP_EXT_MUX_PATH (0x80)

/******************************************************************************
* TVE CTRL API
******************************************************************************/
 void vApiTVECtrlEnable(UINT8 bEnable);
 UINT8 vApiTVEIsPIPEnable(void);
void vApiTVEPreCtrl(UINT8 bPath, UINT8 *bSrc,UINT8 bMainSrcOld,UINT8 bSubSrcOld);
void vApiTVEPostCtrl(UINT8 bSrc);
void vApiTVEVideoStatusNotify(UINT8 bPath, UINT8 bMode);
void vApiTVESetDacMuteMask(UINT8 u1DacId, UINT8 u1Mask, UINT8 u1OnOff);
UINT8 bApiTVEGetBypPath(UINT8 u1DacId);
void vApiTVESetTvScartOutCtrl(UINT8 u1SrcType, rtve_config_t *tTveConfig, void* pv_extra_arg);
void vApiTVESetMonitorScartOutCtrl(UINT8 u1SrcType, rtve_config_t *tTveConfig, void* pv_extra_arg, void* pv_extra_arg2);
void vApiTVESetScartOutCtrl(UINT8 u1Scart_id, UINT8 *u1SrcType, void* pv_extra_arg);
void vApiTVESetOutputFormat(UINT32 u4fmt);
void vApiTVEQueryStatus(void);

/* customization API */
extern void vApiTVEOutPathSelect(UINT8 out_id, UINT8 out_type);
extern void vApiTVEGetConfig(UINT8 u1SrcType, UINT8* pu1Type);
#endif /* _TVE_CTRL_H_ */

