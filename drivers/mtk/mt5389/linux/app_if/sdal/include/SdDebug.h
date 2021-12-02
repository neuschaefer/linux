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
/**
 * @file SdDebug.h
 * @brief Debug Function Define Header File
 * @author S/W Platform Lab
 * @date 2007/03/16
 * @version		
 */

#ifndef	_SDDEBUG_H_
#define	_SDDEBUG_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SDAL.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
*********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/
/**
 * @brief Get the version of SDAL API.
 * @remarks
 * @param pVersion
 * @return
 * @see 
 */
void SdDebug_GetVersion(uint32_t *pVersion);

/**
 * @brief Set the Print Mask value.
 * @remarks NVRam에 저장된 PrintMask를 gSdPrintMask에 반영하기 위해서 호출한다. Set the PrintMask Data to gSdPrinteMask.
 * @param PrintMask
 * @return
 * @see 
 */
void SdDebug_SetPrintMask(uint32_t PrintMask); 

/**
 * @brief Change the Print Mask value.
 * @remarks 내부에 Menu를 두어 gSdPrintMask값을 변경할 수 있도록 하고, 그 변경된 값은 인자에 넣어준다. Change the gSdPrintMask Value in inner Debug Menu and return it.
 * @param pPrintMask [output]
 * @return
 * @see 
 */
void SdDebug_ChangePrintMask(uint32_t *pPrintMask); 

/**
 * @brief Set the Trace Mask value.
 * @remarks NVRam에 저장된 PrintMask를 gSdTraceMask에 반영하기 위해서 호출한다. Set the TraceMask Data to gSdTraceMask. 
 Trace macro like SDXXX_TRACE() is only for tracing Function Calls. If you want to print Debug messages, Use the Print Macro like SDXXX_PRINT().
 * @param TraceMask
 * @return
 * @see 
 */
void SdDebug_SetTraceMask(uint32_t TraceMask);

/**
 * @brief Change the Trace Mask value.
 * @remarks 내부에 Menu를 두어 gSdTraceMask값을 변경할 수 있도록 하고, 그 변경된 값은 인자에 넣어준다. Change the gSdTraceMask Value in inner Debug Menu and return it.
 * @param pPrintMask [output]
 * @return
 * @see 
 */
void SdDebug_ChangeTraceMask(uint32_t *pPrintMask); 

/** 
 * @brief SDAL Debug 메뉴 진입 함수. SDAL Debug Menu entry point
 * @remarks 각 모듈의 SdXXX_Debug()함수는 본 함수에서 모두 호출되어야 한다. 
본 함수만으로 모든 SDAL API를 메뉴얼로 디버깅 할 수 있다.
Each component's SdXXX_Debug() is called by this function.
SDAL API manual debugging is done only by this function.
 * @return 
 * @see 
 */
void SdDebug(void);

#endif

