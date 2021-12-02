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
 * @file SdMCard.h
 * @brief 
 Samsung DAL에서 M-Card에 대한 API를 모아 놓음.
 Collection of M-Card releated APIs
 * @author S/W Platform Lab
 * @date 2007/03/14
 */

#ifndef	_SDMCARD_H_
#define	_SDMCARD_H_

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
	SDMCARD_TYPE_NONE = 0,
	SDMCARD_TYPE_PCCARD,
	SDMCARD_TYPE_S_POD,
	SDMCARD_TYPE_M_POD
} SdMCard_Type_k;

typedef enum
{
	SD_MCARD_STATUS_SIGNALSTATE		= 0x01, ///< Mask of Read Signal State
	SD_MCARD_STATUS_PODTYPE 	= 0x02, ///< Mask of Get Pod Type
	SD_MCARD_STATUS_CARDSTATUS	= 0x04, ///< Mask of Card Status (CR)
} SdMCard_StatusMask_k;

typedef enum
{
	SDMCARD_MSPI_FLAG_HR = 0x40,
	SDMCARD_MSPI_FLAG_CR = 0x40,
	SDMCARD_MSPI_FLAG_EC = 0x20,
	SDMCARD_MSPI_FLAG_L  = 0x10,
	SDMCARD_MSPI_FLAG_F  = 0x08,
	SDMCARD_MSPI_FLAG_DA = 0x04,
	SDMCARD_MSPI_FLAG_ER = 0x02
}SdMCard_MSPI_flag_k;

typedef struct
{
	uint8_t   *pPacket;             // pointer to the buffer holding the packet to be read or to be written   
	uint32_t  packetLength;     	// Length of packet, in bytes, max 4096    
	uint32_t  queryByteFlags; 		// Flags as defined in CCIF for the query byte
} SdMCard_Pkt_Descriptor_t;

typedef struct
{
	uint32_t step;
}SdMCard_Settings_t;

typedef struct
{
	uint32_t signals;
	SdMCard_Type_k eCardType;
	uint32_t cardStatus;
} SdMCard_Status_t;

typedef struct
{
	uint8_t LTSID[2];
	uint16_t HOSTres;
	uint8_t LTS;
	uint8_t CRC;
} SdMCard_Preheader_t;


/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
/**
 * @brief M-Card를 초기화 시킨다. 
 It initializes the M-Card. 
 * @remarks SdMCard_init은 최초 1회만 호출 되며, M-Card를 동작시키기 위한 값들을 초기값으로 셋팅한다. 
 The SdMCard_init will be called up once at the first time, and it will set the values to operate the M-Card to initial values.
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdMCard_Init (void);

/**
 * @brief M-Card Interface를 Open 한다.
 It opens the M-Card Interface. 
 * @remarks M-Card Interface를 이용하는 통신을 위한 파라미터 셋팅 및 준비 작업을 수행한다. 
 Open을 한 상태에서만 M-Card Interface사용이 가능하다. 
 It carries out the parameter setting and the prerequisites for the communication which uses the M-Card Interface.
 It is available to use the M-Card Interface only when it is opened.
 * @param eCardType Cable Card type to be open
 * @return SdResult SD_OK/SD_NOT_OK
 * @see SdMCard_Close
 */
SdResult SdMCard_Open(SdMCard_Type_k eCardType);

/**
 * @brief M-Card Interface를 Close 한다. 
 It closes the M-Card Interface.
 * @remarks M-Card Interface를 이용하는 통신을 위한 파라미터들을 기본 값으로 바꾸고 사용하지 못하도록 연결을 끊는다. 
 It changes the parameters for the communication which use the M-Card Interface to the basic value and disconnect it so that it can not be used.
 * @param eCardType Card Type to be closed, same as used in SdMCard_Open
 * @return SdResult SD_OK/SD_NOT_OK
 * @see SdMCard_Open
 */
SdResult SdMCard_Close(SdMCard_Type_k eCardType);

/**
 * @brief MSPI(Modified SPI) Channel을 이용하여 Data를 읽는다. 
 It reads in the Data by using MSPI(Modified SPI) Channel.
 * @remarks
 * @param pPktDesc MSPI Packet Descriptor
 * @param bBlocking block or non-block
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdMCard_MSPI_Read(SdMCard_Pkt_Descriptor_t* pPktDesc, SdBool_t bBlocking);

/**
 * @brief MSPI(Modified SPI) Channel을 이용하여 Data를 쓴다.
 It writes the Data by using the MSPI(Modified SPI) Channel.
 * @remarks
 * @param pPktDesc MSPI Packet Descriptor
 * @param bBlocking block or non-block
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdMCard_MSPI_Write(SdMCard_Pkt_Descriptor_t* pPktDesc, SdBool_t bBlocking);

/**
 * @brief M-Card Interface에 셋팅되어 있는 정보들을 가져온다. 
 It takes the information which has been set in M-Card Interface.
 * @remarks
 * @param pSettings 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdMCard_Get(SdMCard_Settings_t *pSettings);

/**
 * @brief M-Card Interface에 필요한 정보들을 셋팅한다.
 It sets the information which is required for M-Card Interface. 
 * @remarks
 * @param pSettings
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdMCard_Set(SdMCard_Settings_t *pSettings);

/**
 * @brief M-Card Interface에서 필요한 상태정보들을 가지고 온다. 
 It takes the status information which is required for M-Card Interface.
 * @remarks
 * @param mask
 * @param pState
 * @param timeout
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdMCard_GetStatus(SdMask_t mask, SdMCard_Status_t *pState, uint32_t timeout);

/**
 * @brief Stream Mux를 Control 하여 On/Off 시킨다. 
 It turns Stream Mux On/Off as controlling it.
 * @remarks
 * @param eSrc Source Mode
 * @param pPreheader Preheader value for Mux (LTSID, Host Reserved, LTS, Crc)
 * @param bOn On/Off
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdMCard_Mux_Enable(SdSource_k eSrc, SdMCard_Preheader_t* pPreheader, SdBool_t bOn);

/**
 * @brief Stream Demux를 Control 하여 On/Off 시킨다. 
 It turns Stream Demux On/Off as controlling it.
 * @remarks
 * @param dmxHndl Demux handle
 * @param eSrc Source Mode
 * @param pPreheader Preheader value for Demux (LTSID, Host Reserved, LTS, Crc)
 * @param bOn On/Off
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdMCard_Demux_Enable(SdAVDec_DmxHandle dmxHndl, SdSource_k eSrc, SdMCard_Preheader_t* pPreheader, SdBool_t bOn);

/**
 * @brief SdMCard API의 Manual Debug 함수
 Manual Debug function of SdMCard API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdMCard_Debug(void);

#endif

