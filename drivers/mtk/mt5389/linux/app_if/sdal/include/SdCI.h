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
 * @file SdCI.h
 * @brief Common Interface header file.
 * @author S/W Platform Lab
 * @date 2007/05/23
 */

#ifndef	_SDCI_H_
#define	_SDCI_H_

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
	SD_CI_STATUS_CARDSTATE	= 0x01,
	SD_CI_STATUS_MAX,	
} SdCI_StatusMask_k;

typedef enum
{
	SD_CI_CARDSTATE_NONE = 0,	///< There is no Card in slot.
	SD_CI_CARDSTATE_IDLE,	///< Card State is IDLE.
	SD_CI_CARDSTATE_BUSY,	///< Card State is Busy.
	SD_CI_CARDSTATE_MAX,
} SdCI_CardState_k;

typedef enum
{
	SD_CI_STREAM_DISCONNECT = 0,
	SD_CI_STREAM_CARD_BYPASS,
	SD_CI_STREAM_CARD_THROUGH,	
} SdCI_StreamPath_k;

typedef enum
{
	SD_CI_REGISTER_DATA = 0,
	SD_CI_REGISTER_COMMAND = 1,
	SD_CI_REGISTER_STATUS = 1,
	SD_CI_REGISTER_LSB = 2,
	SD_CI_REGISTER_MSB = 3,
	SD_CI_REGISTER_MAX,	
} SdCI_Register_k;

typedef struct
{
	SdCI_CardState_k eCardState;
} SdCI_Status_t;

typedef struct
{
	SdCI_StreamPath_k ePath;
}SdCI_Settings_t;


/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
/**
 * @brief Initializes the SdCI module
 * @remarks 
 * @param ePowerState
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCI_Init (SdPowerState_k ePowerState);

/**
 * @brief Reset the CI Slot
 * @remarks  
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCI_Reset(void);

/**
 * @brief It takes the status information which is required for Common Interface.
 * @remarks
 * @param mask
 * @param pState
 * @param timeout
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCI_GetStatus(SdMask_t mask, SdCI_Status_t *pState, uint32_t timeout );

/**
 * @brief It takes the information which has been set in Common Interface. 
 * @remarks 
 * @param pSettings
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCI_Get(SdCI_Settings_t *pSettings);

/**
 * @brief It sets the information which is required for Common Interface. 
 * @remarks 
 * @param pSettings
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCI_Set(SdCI_Settings_t *pSettings);

/**
 * @brief Read CI Register 
 * @remarks 
 * @param eReg kind of Registers
 * @param data 8bit value from the register
 * @param length data length
 * @return SD_OK/SD_NOT_OK
 * @see 
 */	
SdResult SdCI_Read_Reg (SdCI_Register_k eReg, uint8_t* data, uint32_t length);

/**
 * @brief Write CI Register 
 * @remarks
 * @param eReg kind of Registers
 * @param data data to be written
 * @param length data length
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCI_Write_Reg (SdCI_Register_k eReg, uint8_t* data, uint32_t length);

/**
 * @brief Read CIS memory
 * @remarks 
 * @param buffer buffer for read memory
 * @param length data size for read
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCI_Read_CIs_Memory (uint8_t * buffer, uint32_t length);

/**
 * @brief Write FCR Value
 * @remarks
 * @param addr32 address offset
 * @param ccr_byte ccr_byte to be written
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCI_Write_FCR_Value (uint32_t addr32, uint8_t ccr_byte);

/**
 * @brief manual debug function for SdCI 
 * @remarks It is called by SdDebug() in SdDebug.h.
 * @return none
 * @see 
 */
void SdCI_Debug(void);

#endif

