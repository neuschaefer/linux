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
 * @file SdNVRam.h
 * @brief NVRam header file.
NVRAM(Non-Volatile Random Access Memory) 는 전원 off 후에도 데이터가 소실되지 않는다. 
NVRAM 내에는 하드웨어를 초기화, Start up Configuration, boot parameter 정보를 가지고 있다. 
SDAL에서 EEPROM Read/Write에 대한 API를 제공한다.
EEPROM에 Data를 읽거나 쓰는 기능을 수행한다.	

NVRAM(Non-Volatile Random Access Memory) does not lose its data after power off.
NVRAM contains information of HW initialization, Start up Configuration, boot parameter.
SDAL provides EEPROM Read/Write APIs.
 * @author S/W Platform Lab
 * @date 2006/08/31
 */

#ifndef	_SDNVRAM_H_
#define	_SDNVRAM_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdUtil.h"

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
	SD_NVRAM_STATUS_WCOUNT			= 0x0001,	///< Write Count

} SdNVRam_StatusMask_k;

typedef struct
{
	uint32_t		*sWriteCountInfo;
} SdNVRam_Status_t;

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**
 * @brief NVram과 관련 세마포어관련 변수 및 상태 변수(NVRAM Write Count등)를  초기화 한다. 
 It initializes the NVram-related semaphore variables.
 * @remarks 
 * @param ePowerState Power State 정보를 하기 enum값 중 하나로 넘겨준다. 최초 Default 값은 SD_PWROFF_TO_NORMAL 이다. Power state. Default is SD_PWROFF_TO_NORMAL.
 * @code
	 typedef enum
	{
		SD_PWROFF_TO_NORMAL,		///< Power off to Normal transition 
		SD_PWROFF_TO_STANDBY,		///< Power off to Standby transition
		SD_STANDBY_TO_NORMAL		///< Standby off to Normal transition
	} SdPowerState_k;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdNVRam_Init(SdPowerState_k ePowerState);

/**
 * @brief NVram에 Data를 저장한다.
 It saves Data in NVram.
 * @remarks  NVRam에 Write 수행시 CRITICAL_SESSION 처리가 필요하다.NVRam Write 하기 전에 NVRam 을 사용하는 모든 모듈들에 대해 사용 가능 Address를 각각 할당 한다. 
 Address를 할당 받은 각 모듈은 SdNVRam 함수를 바로 사용하지 않고 모듈에 맞는 NVRAM ACCESS 주소를 매핑하고 주소의 Range Check를 하여 자신의 Size에서 벗어나는 ACCESS가 없도록 사용한다.
 Read/Write 효율을 높이기 위해 Cache를 운영하도록 하고, NVRAM Write Count를 관리한다. NVRAM Write Count값은 GetStatus()를 통해 얻어간다.
 The CRITICAL_SESSION process is required upon writing data in NVRam. 
 Before writing, allot the available address to all modules which use the NVRam respectively. 
 Each module which is allotted and get the address does not use the SdNVRam function directly, 
 but by mapping the NVRAMACCESS address suitable to the module and checking the range of address, 
 use the function not to have the ACCESS which exceeds its own size.
 It uses cache memory to increase the speed of Read/Write Operation and manages a write counter. The write count of NVRAM can be obtained by calling GetStatus().
 * @param address [in] EEPROM에 DATA 를  쓰고자 하는 주소값. NVRam Address 할당 예는 하기 코드와 같다. NVRAM address to be written. Below is an example of NVRam address allocation table.
 * @param length [in] EEPROM에 쓰고자 하는 DATA의 길이. length of data buffer.
 * @param pDataBuff [in] EEPROM에 쓰고자 하는 DATA가 들어있는 버퍼의 시작주소. start address of data buffer.
 * @code
	#define	NVRAM_CP_SIZE			0x1E50
	#define	NVRAM_SCL_SIZE			0x1000
	#define	NVRAM_HCL_SIZE			0x0700
	#define	NVRAM_DLINK_SIZE		0x0800
	#define	NVRAM_DSP_SIZE			0x3000
	#define	NVRAM_SUBSYSTEM_SIZE	0x0500
	#define	NVRAM_DEV_SIZE		4
	#define	NVRAM_START_ADDR		0x100		
	#define	NVRAM_BASE_ADDR		NVRAM_START_ADDR + 0x300				///<  0x400
	#define	NVRAM_CP_ADDR			(NVRAM_BASE_ADDR)						///<  0x400 ~ 0x2250
																			///< 0x2250 ~ 0x2400 (0x1B0 bytes empty)
	#define	NVRAM_DLINK_ADDR		(NVRAM_CP_ADDR + 0x2000)				///< 0x2400 ~ 0x2C00
	#define	NVRAM_SCL_ADDR		(NVRAM_DLINK_ADDR + NVRAM_DLINK_SIZE)	///< 0x2C00 ~ 0x3C00
	#define	NVRAM_HCL_ADDR		(NVRAM_SCL_ADDR + NVRAM_SCL_SIZE)		///<  0x3C00 ~ 0x4300
	#define	NVRAM_DSP_ADDR		(NVRAM_HCL_ADDR + NVRAM_HCL_SIZE)		///<  0x4300 ~ 0x7300
	#define	NVRAM_SUBSYSTEM_ADDR	(NVRAM_DSP_ADDR + NVRAM_DSP_SIZE)		///<  0x7300 ~ 0x7800	
 * @endcode
 * @code
  
		//ENTER_CRITICAL_SESSION
		//...
	
		// Write Count Update				
		for (i=0; i<length; i++)
		{
			if (++NVRamCount[start+i] > MAX_EEPROM_ACCESS) // Write Count는 블록별로 관리한다. Write Count is stored for each block.
				printf("eeprom access count over : addr=%ld, count=%ld\n",	start+i, NVRamCount[start+i]);
		}
				
		// NVRAM Cache Update
		memcpy(NVRamCache+start,dataBuff,length); 
		
		// NVRAM Write
		retVar = DRV_NvramWrite(address, length, pDataBuff);
	
		//LEAVE_CRITICAL_SESSION
		//...
	
 * @endcode 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdNVRam_Write(uint16_t address, uint16_t length, uint8_t *pDataBuff);

/**
 * @brief NVram에 Data를 읽어온다.
 It read in the data on to the NVram. 
 * @remarks NVRam에 Read 수행시 CRITICAL_SESSION 처리가 필요하다. NVRam Read 하기 전에 NVRam 을 사용하는 모든 모듈들에 대해 사용 가능 Address를 각각 할당 한다. 
 Address를 할당 받은 각 모듈은 SdNVRam 함수를 바로 사용하지 않고 모듈에 맞는 NVRAM ACCESS 주소를 매핑하고 주소의 Range Check를 하여 자신의 Size에서 벗어나는 ACCESS가 없도록 사용한다.
 Cach를 운영하여 사용하는 경우에는 Cache Miss가 발생하지 않는한 Cache에서 읽은 데이타를 넘겨준다.
 The CRITICAL_SESSION process is required upon reading the data on to NVRam. 
 Before reading the NVRam, allot the available Address for all modules which use the NVRam respectively. 
 Each module which is allotted and get the Address does not use the SdNVRam function directly, 
 but by mapping the NVRAM ACCESS address which is suitable to module and checking the range of address, 
 use the function without any ACCESS which exceeds the its own size.
 In case of using cache memory, it actually read in the data from the cache memory as far as there are no cache misses.
 * @param address [in] EEPROM에 DATA 를  READ 하는 주소값. NVRam Address 할당 예는 하기 코드와 같다. NVRAM address to be read. Below is an example of NVRam address allocation table.
 * @param length [in] EEPROM에 READ 하는 DATA의 길이. length of data buffer.
 * @param pDataBuff [out] EEPROM에서 읽은 DATA를 받아올 버퍼의 시작주소. start address of data buffer.
 * @code
	#define	NVRAM_CP_SIZE			0x1E50
	#define	NVRAM_SCL_SIZE			0x1000
	#define	NVRAM_HCL_SIZE			0x0700
	#define	NVRAM_DLINK_SIZE		0x0800
	#define	NVRAM_DSP_SIZE			0x3000
	#define	NVRAM_SUBSYSTEM_SIZE	0x0500
	#define	NVRAM_DEV_SIZE		4
	#define	NVRAM_START_ADDR		0x100		
	#define	NVRAM_BASE_ADDR		NVRAM_START_ADDR + 0x300				///<  0x400
	#define	NVRAM_CP_ADDR			(NVRAM_BASE_ADDR)						///<  0x400 ~ 0x2250
																			///< 0x2250 ~ 0x2400 (0x1B0 bytes empty)
	#define	NVRAM_DLINK_ADDR		(NVRAM_CP_ADDR + 0x2000)				///<  0x2400 ~ 0x2C00
	#define	NVRAM_SCL_ADDR		(NVRAM_DLINK_ADDR + NVRAM_DLINK_SIZE)	///<  0x2C00 ~ 0x3C00
	#define	NVRAM_HCL_ADDR		(NVRAM_SCL_ADDR + NVRAM_SCL_SIZE)		///<  0x3C00 ~ 0x4300
	#define	NVRAM_DSP_ADDR		(NVRAM_HCL_ADDR + NVRAM_HCL_SIZE)		///<  0x4300 ~ 0x7300
	#define	NVRAM_SUBSYSTEM_ADDR	(NVRAM_DSP_ADDR + NVRAM_DSP_SIZE)		///<  0x7300 ~ 0x7800
 * @endcode	
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdNVRam_Read(uint16_t address, uint16_t length, uint8_t *pDataBuff);

/**
 * @brief NVRam의 상태 정보를 가져온다.
 It takes the status of NVRAM.
 * @remarks
 * @param mask
 * @param pStatus 
 * @return 
 * @see 
 */
SdResult SdNVRam_GetStatus(SdMask_t mask, SdNVRam_Status_t *pStatus);

/**
 * @brief SdNVRam API의 Manual Debug 함수
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdNVRam_Debug(void);

#endif

