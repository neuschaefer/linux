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
 * @file SdPod.h
 * @brief SdPod header file
 * @author S/W Platform Lab
 * @date 2006/07/10
 */

#ifndef	_SDPOD_H_
#define	_SDPOD_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdMisc.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/
/* Signal number for DRV_TSIGNAL command */
#define DRV_CARD_DETECT		1
#define DRV_READY_BUSY		2

/* Signal number for DRV_SSIGNAL command */
#define DRV_EMSTREAM	0
//#define DRV_ENSLOT		1
#define DRV_RSTSLOT		2

/* Special commands for PDH_IOControl */
//#define DRV_ADDR		1						  /* Set I/O Base Address   */
#define DRV_READMEM		2						  /* Read memory			*/
#define DRV_WRITEMEM	3						  /* Write memory			*/
#define DRV_READIO		4						  /* Read a I/O Register	*/
#define DRV_WRITEIO		5						  /* Write a I/O Register	*/	
#define DRV_TSIGNAL		6						  /* Check a Signal			*/
#define DRV_SSIGNAL		7						  /* Set / Clear a Signal	*/
#define DRV_READIO_EX	8						  /* Read a I/O Register, Extended Channel	*/
#define DRV_WRITEIO_EX	9						  /* Write a I/O Register	*/

#define SLOT_ADDRESS	0x06020000
#define CIMAX	   		0x84					// CIMaX I2C address
#define DRV_MEMADDR	0						// Base Memory Address
#define DRV_CISLEN		256						// CIS size

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/
	
typedef enum {
	SD_VPP1_33,
	SD_VPP1_50,
	SD_VPP2_33,
	SD_VPP2_50,
}SdPod_Vpp_k;

typedef struct {
	uint16_t addr;			/* address to read/write				*/
	uint16_t len;				/* number of bytes to read/write		*/
	uint8_t *pbytes;			/* pointer to bytes to read/write		*/
	uint16_t rlen;			/* number of bytes actually read/written	*/
} SdPod_stMem;  	 /* structure for DRV_READMEM and DRV_WRITEMEM commands */
	
typedef struct {
	uint16_t registr;			/* register address to read/write		*/
	uint8_t *pvalue;			/* pointer to the value to read/write	*/
} SdPod_stIO;  	   /* structure for DRV_READIO and DRV_WRITEIO commands */

typedef struct {
	uint8_t sig;				/* signal number						*/
	uint8_t value;			/* value(1 : signal present ; 0 missing)*/
} SdPod_stSignal;					/* structure for DRV_TSIGNAL command	*/

typedef struct {
	uint8_t pin;				/* pin code								*/
	uint8_t cs;				/* value(1 : Set ; 0 clear)				*/
} SdPod_ssSignal;					/* structure for DRV_SSIGNAL command	*/

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**
 * @brief SdPod Module Init
 * @remarks initializes the hardware platform
 * @param ePowerState
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPod_Init(SdPowerState_k ePowerState);

/**
 * @brief open the specified physical slot
 * @remarks open the driver for a module slot
 * @param pDeviceName physical slot name
 * @return logical slot id or error (-1)
 * @see 
 */
int32_t SdPod_DriverOpen(uint8_t *pDeviceName);

/**
 * @brief Get Pod map
 * @remarks get the pod channel map 
 * @param pFp
 * @return SD_OK/SD_NOT_OK
 * @see 
 */

SdResult SdPod_GetPodMap(int32_t *pFp);

/**
 * @brief SdPod TS In On
 * @remarks
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPod_TSInOn(void);

/**
 * @brief SdPod TS In Off
 * @remarks 
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPod_TSInOff(void);

/**
 * @brief Reads bytes from the specified slot through IO Access
 * @remarks read data from the module
 * @param fd logical slot id
 * @param len numer of bytes to be read
 * @param pBytes buffer address
 * @return Actual number of read bytes or error (-1)
 * @see 
 */
int32_t SdPod_ReadDataChannel(int32_t fd, uint16_t len, uint8_t *pBytes);

/**
 * @brief Reads bytes from the specified slot at extednded channel
 * @remarks read data from the module through EX channel
 * @param fd logical slot id
 * @param len numer of bytes to be read
 * @param pBytes buffer address
 * @return Actual number of read bytes or error (-1)
 * @see 
 */
int32_t SdPod_ReadExtendedChannel(int32_t fd, uint16_t len, uint8_t *pBytes);

/**
 * @brief Writes bytes to the specified slot
 * @remarks send data to the module
 * @param fd logical slot id
 * @param len numer of bytes to write
 * @param pBytes buffer address
 * @return Actual number of written bytes or error (-1)
 * @see 
 */
int32_t SdPod_WriteDataChannel(int32_t fd, uint16_t len, uint8_t *pBytes);

/**
 * @brief Writes bytes to the specified slot at extednded channel
 * @remarks send data to the module through EX channel
 * @param fd logical slot id
 * @param len numer of bytes to write
 * @param pBytes buffer address
 * @return Actual number of written bytes or error (-1)
 * @see 
 */
int32_t SdPod_WriteExtendedChannel(int32_t fd, uint16_t len, uint8_t *pBytes);

/**
 * @brief Controls special functions
 * @remarks execute special commands
 * @param fd logical slot id
 * @param command numer of bytes to write
 * @param pArg additional information requested by the specific command
 * @return success (1) or error (-1)
 * @see 
 */
int32_t SdPod_IOControl(int32_t fd, uint32_t command, void *pArg);

/**
 * @brief closes the specified logical slot
 * @remarks close the driver for a module slot
 * @param fd logical slot id
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPod_DriverClose(int32_t fd);

/**
 * @brief Free Pod Map
 * @remarks free the pod channel map 
 * @param fp
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPod_FreePodMap(int32_t fp);

/**
 * @brief Oob Enable
 * @remarks enable getting the oob data
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPod_OobEnable(void);

/**
 * @brief Oob Disable
 * @remarks disable getting the oob data
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPod_OobDisable(void);

/**
 * @brief Set Vpp
 * @remarks setting vpp
 * @param eVpp PodVpp
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPod_SetVpp(SdPod_Vpp_k eVpp);

/**
 * @brief Cable Card Detect
 * @remarks detecting the cable card inserted
 * @return 0(false)/1(true)
 * @see 
 */
SdBool_t SdPod_CardDetect (void);

/**
 * @brief SdPod API의 Manual Debug 함수
 Manual Debug function of SdPod API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdPod_Debug(void);

#ifdef __cplusplus
}
#endif

#endif


