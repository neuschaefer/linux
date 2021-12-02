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
 *---------------------------------------------------------------------------*/

/** @file mtvfe.h
 *  This header file declares exported APIs of ADC module.
 */
#ifndef MT_VFE_H
#define MT_VFE_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"
#include "mtvdecex.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
/** ADC channel select
*/
typedef enum
{
	HDADC1_CHANNEL_B_Y,
	HDADC2_CHANNEL_G_PB,
	HDADC3_CHANNEL_R_PR,
} MTVFE_HDADC_CHANNEL_T;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/**Reset ADC module to default.
*@param  void
*@retval void
*/
EXTERN MT_RESULT_T MTVFE_ADCSettingReset(void);

/** Do ADC calibration(autocolor)
*@param  void
*@retval void
*This function is for product calibration. Every TV set need calibration once in factory. 
*Input source must be YPbPr and VGA and use specific pattern.
*/
EXTERN MT_RESULT_T MTVFE_ADCCalibration(void);

/**Read ADC_Offset
*@param eChannelSel	HDADC Channel
*@param pu2Val Gain Value
*/
EXTERN MT_RESULT_T MTVFE_GetHDADCOffset(MTVFE_HDADC_CHANNEL_T eChannelSel, UINT16* pu2Val);

/**Read ADC_Gain
*@param eChannelSel	HDADC Channel
*@param pu2Val Gain Value
*/
EXTERN MT_RESULT_T MTVFE_GetHDADCGain(MTVFE_HDADC_CHANNEL_T eChannelSel, UINT16* pu2Val);

/**Write ADC_Offset
*@param eChannelSel	HDADC Channel
*@param u2Offset		Offset Value
*@retval void
*/
EXTERN MT_RESULT_T MTVFE_SetHDADCOffset(MTVFE_HDADC_CHANNEL_T eChannelSel,UINT16 u2Offset);

/**Write ADC_Gain
*@param eChannelSel	HDADC Channel
*@param u2Gain		Gain Value
*@retval void
*/
EXTERN MT_RESULT_T MTVFE_SetHDADCGain(MTVFE_HDADC_CHANNEL_T eChannelSel,UINT16 u2Gain);

/**Get maximum Value in a frame
*@param eChannelSel	HDADC Channel
*@param pu2Val Max value
*/
EXTERN MT_RESULT_T MTVFE_GetADCMaxValue(MTVFE_HDADC_CHANNEL_T eChannelSel, UINT16* pu2Val);

/**Get balnking Value
*@param eChannelSel	HDADC Channel
*@param pu2Val Blank value
*/
EXTERN MT_RESULT_T MTVFE_GetBlankVal(MTVFE_HDADC_CHANNEL_T eChannelSel, UINT16* pu2Val);

/**Set sync slicer level Value
*@param bVTL  : low level for hsyterisis
*@param bVTH  : High level for hsyterisis
*@ return void
*/
EXTERN MT_RESULT_T MTVFE_SetSYNCSlicerLevel(UINT16 wSVTL, UINT16 wSVTH);

/**Set ADC_Gain from NVM
*@param inputType	: HDMI/COMP/VGA/SCART
*@retval void
*/
EXTERN MT_RESULT_T MTVFE_SetPGAGainOffsetInitVal(UINT8 inputType);

EXTERN MT_RESULT_T MTVFE_GetGainStorage(MTVDECEX_SRC_T eType,UINT32 *pu4Value);

EXTERN MT_RESULT_T MTVFE_EBGainOverRule(UINT8 OnOff);
EXTERN MT_RESULT_T MTVFE_Embedded_Gain(UINT8 Type);


#ifdef __cplusplus
}
#endif

#endif
