/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   Permission of LG Electronics Inc.
 *****************************************************************************/

/** @file 	cmnio_type.h
 *
 *  Definitions for global configurations
 *  DESCRIPTION : In this file, all global configurations, enumerations, data
			   or structures should be shared between modules positioned in
			   different layers.
			   For example, enumeration for external video supported in the system,
			   should be referenced in User Interface module (APP layer) and,
			   at the same time, referenced Video module(Driver Layer).
			   So, these kind of definition or declaration should be stated
			   here.
 *  @author	Baekwon Choi (최배권, bk1472@lge.com)
 *  @version 	1.1
 *  @date	2008.06.20
 *  @see
 */

#ifndef _CMNIO_TYPE_H_

#define _CMNIO_TYPE_H_

/*-----------------------------------------------------------------------------
	제어 상수
	(Control Constants)
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	#include 파일들
	(File Inclusions)
------------------------------------------------------------------------------*/
#include "branches.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	매크로 함수  정의
	(Macros Definitions)
------------------------------------------------------------------------------*/
#ifdef _CMN_VAR_CONF_C_
#define CMN_VAR(vType, vName, vValue)	vType vName = (vType) vValue
#define CMN_EXT
#else
#define CMN_VAR(vType, vName, vValue)	extern vType vName
#define CMN_EXT                         extern
#endif

/*-----------------------------------------------------------------------------
	상수 정의
	(Constant Definitions)
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * NVRAM 저장 장치
 *   - NVM_EEPROM_ONLY   : Support only I2C-EEPROM
 *   - NVM_EEPROM_N_NAND : Support both I2C-EEPROM and NAND flash
 *
 * 2010.07.27. by gunho.lee
 *-------------------------------------------------------------------------*/
#define	NVM_EEPROM_ONLY					0
#define	NVM_EEPROM_N_NAND				1
#define TNVM_MAGIC_CODE					0x20100118

#define EEPROM_SIZE						0x8000 // 32K

/*------------------------------------------
 * NVM DB Map
 *------------------------------------------*/
/*
 * Jackee's Note 2008/11/21: Size 관련 주의 사항.
 * 모든 DB는 NVMDRV_PAGE_SIZE로 align 처리하는게 좋다. 그렇지 않으면,
 * NVRAM read/write 시 속도 저하 문제가 있을 수 있다.
 */

/* NVM Header */
#define NVM_HEADER_SIZE					16

/* NVM Magic */
#define TNVM_MAGIC_SIZE					4

/* NVM DB */
#define SYS_DB_SIZE						400		/** 0180(384)  : System DB, should be first DB	**/
#define ANA_DB_SIZE						832		/** 0340(832) : Analog Driver + Calibration DB	**/
#define TOOL_OPTION_DB_SIZE				128		/** 0080(128)  : Tool Option DB, reserved space = 108	**/
#define FACTORY_DB_SIZE					288		/** 0100(256)  : Factory DB, reserved space = 8	256-288-320**/
#define SUMODE_10POINTWB_DB_SIZE		128		/** 0080(128) : Sevice Mode 의 10 Point White Balance DB **/
#define MODEL_INFO_DB_SIZE				48		/** 0030(48) : Model Info DB **/
#define PRESERVE_DB_SIZE				16		/** 0010(16) : Preserve DB **/
#define EMP_DB_SIZE 					128		/** 0080(128)  : DivX DRM Information  			**/
#define SWU_DB_SIZE						4096
#define MHP_DB_SIZE						4171
#define MICOM_DB_SIZE					128		/** 0080(128)  : Internal MICOM DB				**/
#define THX_DB_SIZE						68		/** 0080(128) : THX 의 10 Point White Balance DB **/
#define NEW_FACTORY_DB_SIZE				64		/** 0040(64)  : New Factory DB		**/
#define	NETLOG_DB_SIZE					804		/** 0194(404) : Net Server Log DB. size 잘못됨. 804로 변경. jaehan.park 101213 **/
#define	TMP_CPLIST_DB_SIZE				700		/** 700  TEMP_CPLIST_DB. GP4에서 UI DB로 이동 **/
#define	MAC_ADDRESS_SIZE				32		/** MAC Address를 eeprom에 backup **/
#define SOC_VENDOR_SIZE					64		/** 0040(64)  : Vendor data 		**/

#define UI_DB_SIZE						50000	//38888	/** 9400(37888) : UI DB(edge_enhancer,xvYCC,thx)	**/	//23284 --> 37888 -->38888
#define UI_EXPERT_DB_SIZE				38000	//33792	/** 8400(33792) : UI Expert DB(edge, xvYVV,			**/	//11700 --> 28672 --> 33792


//#if 0 //#if(NVRAM_DEV  == NVM_EEPROM_ONLY)
#if 0 //#if (PLATFORM_TYPE == BCM_PLATFORM)
#define CH_DB_SIZE						(40000)	// in case of 128KB EEPROM
#else
#define CH_DB_SIZE						(82432   + 0x8000)	/** a200(41472): Channel DB(3k for DRRT)		**/
#endif
#define DVBS_DB_SIZE					(0xC8000 - 0x8000)	/* 800 K*/
#ifdef INCLUDE_SYS_ISDB
#define CA_MAIL_DB_SIZE					800*30
#endif




#define	BASIC_DB_SIZE					( NVM_HEADER_SIZE				\
										+ TNVM_MAGIC_SIZE				\
										+ SYS_DB_SIZE 					\
										+ ANA_DB_SIZE 					\
										+ TOOL_OPTION_DB_SIZE			\
										+ FACTORY_DB_SIZE				\
										+ SUMODE_10POINTWB_DB_SIZE		\
										+ MODEL_INFO_DB_SIZE			\
										+ PRESERVE_DB_SIZE				\
										+ EMP_DB_SIZE					\
										+ SWU_DB_SIZE					\
										+ MHP_DB_SIZE					\
										+ MICOM_DB_SIZE					\
										+ THX_DB_SIZE					\
										+ NEW_FACTORY_DB_SIZE			\
										+ NETLOG_DB_SIZE				\
										+ TMP_CPLIST_DB_SIZE			\
										+ MAC_ADDRESS_SIZE)


#define EXTEND_DVBS_DB_SIZE    			( UI_DB_SIZE					\
										+ UI_EXPERT_DB_SIZE				\
										+ CH_DB_SIZE					\
										+ DVBS_DB_SIZE )
#ifdef INCLUDE_SYS_ISDB
#define EXTEND_DB_SIZE    				( UI_DB_SIZE					\
										+ UI_EXPERT_DB_SIZE				\
										+ CA_MAIL_DB_SIZE				\
										+ CH_DB_SIZE)

#else
#define EXTEND_DB_SIZE    				( UI_DB_SIZE					\
										+ UI_EXPERT_DB_SIZE				\
										+ CH_DB_SIZE )
#endif

#define TOTAL_DB_SIZE					(BASIC_DB_SIZE + EXTEND_DB_SIZE)

#define TOTAL_DVBS_DB_SIZE				(BASIC_DB_SIZE + EXTEND_DVBS_DB_SIZE)


/*
 * ANA_DB와 FACTORY_DB는 NVM MAP이 변경되더라도, 값을 유지해야 합니다.
 * NVM MAP 변경시 ANA_DB와 FACTORY_DB가 Reset되지 않도록 신중히 고려해 주십시오.
 */
/* BASE OFFSET */
#define NVM_HEADER_BASE   			( 0                                     )
#define TNVM_MAGIC_BASE				( NVM_HEADER_BASE   + NVM_HEADER_SIZE 	)
#define SYS_DB_BASE       			( TNVM_MAGIC_BASE 	+ TNVM_MAGIC_SIZE 	)
#define ANA_DB_BASE       			( SYS_DB_BASE 		+ SYS_DB_SIZE 		)
#define TOOL_OPTION_DB_BASE   		( ANA_DB_BASE       + ANA_DB_SIZE       )
#define FACTORY_DB_BASE   			( TOOL_OPTION_DB_BASE+ TOOL_OPTION_DB_SIZE )
#define	SUMODE_10POINTWB_DB_BASE	( FACTORY_DB_BASE	+ FACTORY_DB_SIZE		)
#define	MODEL_INFO_DB_BASE			(SUMODE_10POINTWB_DB_BASE	+ SUMODE_10POINTWB_DB_SIZE	)
#define	PRESERVE_DB_BASE			(MODEL_INFO_DB_BASE	+ MODEL_INFO_DB_SIZE	)
#define	EMP_DB_BASE     			( PRESERVE_DB_BASE  + PRESERVE_DB_SIZE 	)
#define	SWU_DB_BASE	 				( EMP_DB_BASE		+ EMP_DB_SIZE 		)
#define	MHP_DB_BASE	 				( SWU_DB_BASE		+ SWU_DB_SIZE 		)
#define	MICOM_DB_BASE     			( MHP_DB_BASE		+ MHP_DB_SIZE		)
#define	THX_DB_BASE					(MICOM_DB_BASE		+ MICOM_DB_SIZE		)
#define NEW_FACTORY_DB_BASE   		( THX_DB_BASE		+ THX_DB_SIZE 		)
#define	NETLOG_DB_BASE				( NEW_FACTORY_DB_BASE + NEW_FACTORY_DB_SIZE	)
#define	TMP_CPLIST_DB_BASE			( NETLOG_DB_BASE	  + NETLOG_DB_SIZE )
#define	MAC_ADDRESS_BASE			( TMP_CPLIST_DB_BASE  + TMP_CPLIST_DB_SIZE )


/* NAND FLASH 영역 */
#define UI_DB_BASE        			( MAC_ADDRESS_BASE	  + MAC_ADDRESS_SIZE)
#define UI_EXPERT_DB_BASE 			( UI_DB_BASE 		+ UI_DB_SIZE 		)
#define	CH_DB_BASE        			( UI_EXPERT_DB_BASE	+ UI_EXPERT_DB_SIZE	)
#ifdef INCLUDE_SYS_ISDB
#define	CA_MAIL_DB_BASE        		( CH_DB_BASE		+ CH_DB_SIZE	)
#define DVBS_DB_BASE				( CA_MAIL_DB_BASE  	+ CA_MAIL_DB_SIZE		)
#else
#define DVBS_DB_BASE				( CH_DB_BASE     	+ CH_DB_SIZE		)
#endif

/*ALEF EEPROM 영역 */
#define ALEF_DB_BASE				( 0														)
#define ALEF_THX_DB_BASE			( ALEF_DB_BASE				+ ANA_DB_SIZE				)


// request from L9 SIC 
#define	SOC_VENDOR_BASE				( EEPROM_SIZE		- SOC_VENDOR_SIZE - 10)

#if ( BASIC_DB_SIZE > SOC_VENDOR_BASE)
#error "We need more EEPROM NVM size !!!!!"
#endif

/*
 * *** 새로운 영역 추가는 이곳에 해주세요. ***
 * EMP_DB_SIZE, ACAP_DB와 MICOM_DB는 NVM 마지막 부분으로 고정시킨 상태입니다.
 * 또한, EMP_DB_SIZE, ACAP_DB와 MICOM_DB는 total Magic Number가 바뀌었을 때도 지워지지 않게 했습니다.
 */

#if 0

#define  DVBS_DB_BASE				(PRESERVE_DB_BASE     + PRESERVE_DB_SIZE)
#define  DVBS_DB_SIZE				0xC8000 /* 800K */ //0xAF000 /* 700 K*/
#define NVRAM_SIZE        			( DVBS_DB_BASE     + DVBS_DB_SIZE - 1		)

#endif
/*
* stonedef - 091208
* 	SUMODE_10POINTWB_DB 와 THX_DB 의 valid mark 는 구조체 변경 시 자동으로 변경되지
* 	않도록 하였음 - 변경이 필요하면 valid mark 를 수동으로 바꿔줘야 함
*/


/*
 * Jackee's Note 2008/11/21: Magic 관련 처리 변경.
 *		TOTAL_MAGIC에 의한 DB Reset은 지원하지 않도록 한다.
 *		대신 구조체 변경에 따른 자동 RESET을 활용한다.
 *		특정 DB를 Reset하고 싶으면, 해당 DB에 크기가 0은 array를 선언하거나
 *		기존에 선언했던 array member의 이름을 변경하면 된다.
 *		이름 변경시 기존의 매직 번호 할당에 사용했던 방식과 비슷하게, 날짜를 포함한
 *		일련 번호를 포함하게 하면 가독성을 높일 수 있다.
 *		당분간 시험을 위해 여기에 선언되었던 상수는 C 소스로 이동한다.
 *		신규 방식이 사용된 예제는 drivers/include/nvm_ddi.h 의 SYS_DB_T 를 참고한다.
 *
 * Jackee's Note 2008/08/13: Valid Mark 관리 방식 변경
 *		SYS_DB_VALID_MARK, ANA_DB_VALID_MARK, FACTORY_DB_VALID_MARK
 * 에 의해 결정되던 gSysNvmDB, gFactoryDB, gAnaNvmDB 값의 초기화 여부 결정방식이
 * 자동 감지 방식으로 변경되었슴. 감지 가능한 변경 내용은 다음과 같음.
 * 		1. Element의 추가 혹은 삭제.
 * 		2. 기존 element의 이름, Type 또는 크기 변경
 * 		3. 기존 element의 선언 위치 변경.
 * 		4. Structue에서 사용한 user defined type의 변경
 * 이외에도 Structe 구성 결과에 영향을 미치는 모든 항목의 변화에 대흥하여, valid
 * mark가 자동으로 변경되어 nvm에 저장된 값을 reset하게 됩니다.
 * 초기값의 변경에 따른 reset은 더이상 허용하지 않습니다.
 */

#define EDID_NVMDRV_PAGE_SIZE			8		/** for RGB/HDMI edid nvram(AT24C02BN) */
#define EDID_NVMDRV_TOTAL_SIZE			256		/** for RGB/HDMI edid nvram(AT24C02BN) */

/*-----------------------------------------------------------------------------
	형 정의
	(Type Definitions)
------------------------------------------------------------------------------*/
typedef struct
{
	char		*pName;
	UINT32		portNum;
	UINT32		slvAddr;
	UINT32		speed;
} I2C_DESC_T;

typedef struct
{
	I2C_DESC_T **ppI2cDev;
	char		*pName;
} I2C_DEV_LIST_TBL;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /*_CMNIO_TYPE_H_*/




