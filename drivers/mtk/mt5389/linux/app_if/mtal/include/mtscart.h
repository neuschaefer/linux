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
 * $RCSfile: mtscart.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/ 
/** @file mtscart.h
 *  This header file declares exported APIs of Scart.
 */

#ifndef MT_SCART_H
#define MT_SCART_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"
   
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** Scart Input Mode
*/
typedef enum _MTSCART_INPUT_MODE_T 
{
	   MTSCART_AUTO,
	   MTSCART_CVBS_RGB,
	   MTSCART_CVBS,
	   MTSCART_RGB,
	   MTSCART_SV,
	   MTSRC_MAX,
}MTSCART_INPUT_MODE_T;

/** Scart Pin8 Status
*/
typedef enum _MTSCART_PIN_8_T
{
	MTSCART_PIN_8_NO_SIGNAL,    
	MTSCART_PIN_8_ASPECT_16_9,
	MTSCART_PIN_8_ASPECT_4_3,
	MTSCART_PIN_8_UNKNOWN
} MTSCART_PIN_8_T;

/** Scart ID
*/
typedef enum _MTSCART_ID_T
{
	MTSCART_1 =0,
	MTSCART_2,
	MTSCART_ID_MAX
}MTSCART_ID_T;

/** Video signal status callback data structure
*/
typedef struct _MTSCART_CB_T_
{
    UINT32 u4Arg1;
    UINT32 u4Arg2;
}MTSCART_CB_T;

/** Video signal status callback function
*/
typedef VOID (*MTSCART_PFN_PIN8_NFY_CB)(
    MTSCART_ID_T					eScartId,
    MTSCART_PIN_8_T				ePin8Status
);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/**
 * @Set Scart Input Mode
 * @param  eScartMode 
 	   MTSCART_AUTO,	   MTSCART_CVBS_RGB,   MTSCART_CVBS,
	   MTSCART_RGB,	   MTSCART_SV
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTVDECEX_SCART_SetInputMode(MTSCART_INPUT_MODE_T eScartMode);

/**
* @brief Register input video signal change callback function
* @This callback function must be registered before MTVDO_Init()
* @param pfnFct input video signal decteion done callback function 
*@retval MTR_OK          Success.
*@retval MTR_NOT_OK  Fail.
*/
EXTERN MT_RESULT_T MTVDECEX_SCART_RegPin8ChgNfyCb(const MTSCART_PFN_PIN8_NFY_CB pfnFct);

/**
 * @Get Scart Pin8 Status
 * @param eScartId MTSCART_1, MTSCART_2
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTVDECEX_SCART_GetPin8Status(MTSCART_ID_T eScartId, MTSCART_PIN_8_T *ePin8Status);

/**
 * @Get Scart Input Mode
 * @param  eScartMode 
 	   MTSCART_CVBS_RGB, MTSCART_CVBS, MTSCART_RGB, MTSCART_SV
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTVDECEX_SCART_GetInputMode(MTSCART_INPUT_MODE_T *peScartMode);

/** 
 * @Get Scart HW Input Mode Status
 * @param  eScartMode 
 * 	   MTSCART_AUTO,	   MTSCART_CVBS_RGB,   MTSCART_CVBS,
 *	   MTSCART_RGB,	   MTSCART_SV
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTVDECEX_SCART_GetHWInputMode(MTSCART_INPUT_MODE_T *peScartMode);

#ifdef __cplusplus
}
#endif

/**
 * @Set Scart Matrix Enable
 * @param u1OnOff     
 * @retval MTR_OK                    Success.
 * @retval MTR_NOT_OK                Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_SetScartMatrixEnable(UINT8 u1OnOff);

/**
 * @Set Scart Matrix Y offset
 * @param u1Value 	range 0- 63
 * @retval MTR_OK                    Success.
 * @retval MTR_NOT_OK                Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_SetScartMatrixYoffset(UINT8 u1Value);

/**
 * @Set Scart Matrix RGB offset
 * @param u1Value 	range 0- 31
 * @retval MTR_OK                    Success.
 * @retval MTR_NOT_OK                Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_SetScartMatrixRgbOffset(UINT8 u1Value);

/**
 * @Set Scart RGB porch
 * @param u2Value Porch value	
 * @retval MTR_OK                    Success.
 * @retval MTR_NOT_OK                Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_SetScartRgbPorch(UINT16 u2Value);

/**
 * @Get Scart RGB porch
 * @param u2Value Porch value	
 * @retval MTR_OK                    Success.
 * @retval MTR_NOT_OK                Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_GetScartRgbPorch(UINT16 *u2Value);

#endif //MT_SCART_H


