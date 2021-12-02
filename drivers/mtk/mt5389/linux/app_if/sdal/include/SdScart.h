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
 * @file SdScart.h
 * @brief Scart header file.
 * @author S/W Platform Lab
 * @date 2007/05/23
 */

#ifndef	_SDSCART_H_
#define	_SDSCART_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdMisc.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/
typedef enum
{
	SD_SCART_STATUS_8PIN	= 0x01,	
	SD_SCART_STATUS_MAX,	
} SdScart_StatusMask_k;

typedef enum
{
    SD_SCART_LEVEL_LOW, ///<8th Pin : 0 ~ 2.5V,                         
    SD_SCART_LEVEL_MID, ///<8th Pin : 4 ~ 7.5V, AV input 16:9 mode                             
    SD_SCART_LEVEL_HIGH, ///<8th Pin : 9 ~ 12V, AV input 16:9 mode                         
    SD_SCART_LEVEL_UNKNOWN
} SdScart_Level_k;

typedef enum
{
	SD_SCART_MODE_RGB,
	SD_SCART_MODE_CVBS,
	SD_SCART_MODE_MAX,
} SdScart_InputMode_k;

typedef struct
{
	SdScart_Level_k e8PinLevel; ///<8th Pin Voltage Level	
} SdScart_Status_t;

typedef struct
{
	SdScart_InputMode_k eInput;
	SdBool_t	bOut; ///< TRUE : Monitor/Tv Out, FALSE : Ext.In
} SdScart_Settings_t;


/**
 * @brief Initialize the Scart Module.
 * @remarks
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdScart_Init(void);


/**
 * @brief  It reads in the Scart-related setting values.
 * @remarks
 * @param eScartId
 * @param pSettings 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdScart_Get(SdScartID_k eScartId, SdScart_Settings_t* pSettings);


/**
 * @brief  It sets the Scart-related setting values.
 * @remarks
 * @param eScartId
 * @param pSettings 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdScart_Set(SdScartID_k eScartId, SdScart_Settings_t* pSettings);


/**
 * @brief  It takes the Scart-related Information.
 * @remarks
 * @param eScartId
 * @param mask 
 * @param pScartLevel 
 * @param timeout 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdScart_GetStatus(SdScartID_k eScartId, SdMask_t mask, SdScart_Level_k* pScartLevel, uint32_t timeout );

/**
 * @brief manual debug function for SdScart
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return none
 * @see 
 */
void SdScart_Debug(void);

#endif

