/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2009 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

 /** @file pql_uid.h
  *
  *  Pql unique identification for each model.
  *
  *  @author	 Jin, Dukyoung(dyjin@lge.com)
  *  @version	 1.0
  *  @date		 2009. 8. 07
  *  @note
  *  @see
  */
#ifndef _PQL_UID_H_
#define _PQL_UID_H_



/******************************************************************************
	#include 파일들
	(File Inclusions)
*******************************************************************************/
#include "osa_modeldef.h"



/******************************************************************************
    상수 정의
    (Constant Definitions)
*******************************************************************************/

#define OPT_DISPLAY_WIDTH			1
#define OPT_INCH_WIDTH				5
#define OPT_COUNTRY_GROUP_WIDTH		5
#define OPT_FRC_ONOFF_WIDTH			4	// 3 -> 4
#define OPT_RESOLUTION_WIDTH		2
#define OPT_MODULE_MAKER_WIDTH		5
#define OPT_TUNER_TYPE_WIDTH		4
#define OPT_MODULE_TYPE_WIDTH		3
#define OPT_TOOL_WIDTH				2


#define OPT_DISPLAY_BIT				31 			 // LCD : 0, PDP : 1
#define OPT_INCH_BIT				( OPT_DISPLAY_BIT		- OPT_INCH_WIDTH			)	// 26
#define OPT_COUNTRY_GROUP_BIT		( OPT_INCH_BIT	 		- OPT_COUNTRY_GROUP_WIDTH	)	// 21
#define OPT_FRC_ONOFF_BIT			( OPT_COUNTRY_GROUP_BIT - OPT_FRC_ONOFF_WIDTH		)	// 17 // 18 PDP 항상 OFF
#define OPT_RESOLUTION_BIT			( OPT_FRC_ONOFF_BIT		- OPT_RESOLUTION_WIDTH		)	// 15 // 16
#define OPT_MODULE_MAKER_BIT		( OPT_RESOLUTION_BIT	- OPT_MODULE_MAKER_WIDTH	)	// 10 // 11
#define OPT_TUNER_TYPE_BIT			( OPT_MODULE_MAKER_BIT	- OPT_TUNER_TYPE_WIDTH		)	// 6 //  7	PDP OFF
#define OPT_MODULE_TYPE_BIT			( OPT_TUNER_TYPE_BIT	- OPT_MODULE_TYPE_WIDTH		)	// 3 // 4
#define OPT_TOOL_BIT				( OPT_MODULE_TYPE_BIT	- OPT_TOOL_WIDTH			)	// 1 // 2


#define OPT_INCH_MASK				(0x7FFF >> (15-OPT_INCH_WIDTH			))
#define	OPT_COUNTRY_GROUP_MASK		(0x7FFF >> (15-OPT_COUNTRY_GROUP_WIDTH	))
#define	OPT_FRC_ONOFF_MASK			(0x7FFF >> (15-OPT_FRC_ONOFF_WIDTH		))
#define OPT_RESOLUTION_MASK			(0x7FFF >> (15-OPT_RESOLUTION_WIDTH		))
#define	OPT_MODULE_MAKER_MASK		(0x7FFF >> (15-OPT_MODULE_MAKER_WIDTH	))
#define	OPT_TUNER_TYPE_MASK			(0x7FFF >> (15-OPT_TUNER_TYPE_WIDTH		))
#define OPT_MODULE_TYPE_MASK		(0x7FFF >> (15-OPT_MODULE_TYPE_WIDTH	))
#define OPT_TOOL_MASK				(0x7FFF >> (15-OPT_TOOL_WIDTH			))



#define OPT_MANDATORY_LCD_MASK		( 	(OPT_MODULE_MAKER_MASK	<< OPT_MODULE_MAKER_BIT	) |	\
										(OPT_MODULE_TYPE_MASK 	<< OPT_MODULE_TYPE_BIT	) |	\
										(OPT_RESOLUTION_MASK	<< OPT_RESOLUTION_BIT	) |	\
										(OPT_TUNER_TYPE_MASK	<< OPT_TUNER_TYPE_BIT	) )

#define OPT_MANDATORY_PDP_MASK		( 	(OPT_MODULE_MAKER_MASK	<< OPT_MODULE_MAKER_BIT	) |	\
										(OPT_RESOLUTION_MASK	<< OPT_RESOLUTION_BIT	) )

#define UID_MATCH_SHIFT		3	/*  PQL_DB_ELEMENT_MATCH_T의 size보다 크거나 같아야 함.*/


#define	RMM_GROUP_CODE_ALL	OPT_COUNTRY_GROUP_MASK
#define	INCH_ALL			OPT_INCH_MASK
#define	FRC_CHIP_ALL		OPT_FRC_ONOFF_MASK
#define PQL_TOOL_ALL		OPT_TOOL_MASK
#define TUNER_ALL			OPT_TUNER_TYPE_MASK

#define FRC_CHIP_U2		FRC_CHIP_URSA2
#define FRC_CHIP_U3I	FRC_CHIP_URSA3_INTERNAL
#define FRC_CHIP_U3E	FRC_CHIP_URSA3_EXTERNAL


#define CONVERT_TUNER_R(tuner)	((tuner==LGT10_R)?LGT10:tuner)	 // ((tuner==SI2173)?CHB:tuner))

/*******************************************************************************
    매크로 함수 정의
    (Macro Definitions)
*******************************************************************************/
#define LCD_UID( display, module_maker, module_type, resolution, tuner_type, country, frc_type, slim, inch )	\
	(	((0x01						& display	)	<< OPT_DISPLAY_BIT		)	|	\
		((OPT_MODULE_MAKER_MASK		& module_maker) << OPT_MODULE_MAKER_BIT	)	|	\
		((OPT_MODULE_TYPE_MASK		& module_type )	<< OPT_MODULE_TYPE_BIT	)	|	\
		((OPT_RESOLUTION_MASK		& resolution) 	<< OPT_RESOLUTION_BIT	)	|	\
		((OPT_COUNTRY_GROUP_MASK	& country	) 	<< OPT_COUNTRY_GROUP_BIT)	|	\
		((OPT_FRC_ONOFF_MASK		& frc_type	)	<< OPT_FRC_ONOFF_BIT	)	|	\
		((OPT_TOOL_MASK				& slim		)	<< OPT_TOOL_BIT			)	|	\
		((OPT_TUNER_TYPE_MASK		& CONVERT_TUNER_R(tuner_type)) 	<< OPT_TUNER_TYPE_BIT	)	|	\
		((OPT_INCH_MASK				& inch		)	<< OPT_INCH_BIT			)	)

#define PDP_UID( display, module_maker, resolution, tuner_type, country, frc_type, slim, inch )	\
	(	((0x01						& display	)	<< OPT_DISPLAY_BIT		)	|	\
		((OPT_MODULE_MAKER_MASK		& module_maker) << OPT_MODULE_MAKER_BIT	)	|	\
		((OPT_RESOLUTION_MASK		& resolution) 	<< OPT_RESOLUTION_BIT	)	|	\
		((OPT_TUNER_TYPE_MASK		& CONVERT_TUNER_R(tuner_type)) 	<< OPT_TUNER_TYPE_BIT	)	|	\
		((OPT_COUNTRY_GROUP_MASK	& country	) 	<< OPT_COUNTRY_GROUP_BIT)	|	\
		((OPT_FRC_ONOFF_MASK		& frc_type	)	<< OPT_FRC_ONOFF_BIT	)	|	\
		((OPT_TOOL_MASK				& slim		)	<< OPT_TOOL_BIT			)	|	\
		((OPT_INCH_MASK				& inch		)	<< OPT_INCH_BIT			)	)


#define _COUNTRY(country)	RMM_GROUP_CODE_##country
#define _INCH(inch)			INCH_##inch
#define _FRC(frc)			FRC_CHIP_##frc
#define _TOOL(tool)			PQL_TOOL_##tool
#define _TUNER(tuner)		TUNER_##tuner

#define _OPT_MANDATORY_MASK(display)	OPT_MANDATORY_##display##_MASK


/*******************************************************************************
    형 정의
    (Type Definitions)
*******************************************************************************/
typedef UINT32 PQL_DB_UID_T;

/**
 * PQL DB Table 구성을 위한 struct
 * @see pql_db_index_tbl_lcd.c
 */
typedef struct {
	PQL_DB_UID_T	uid;
	UINT32			subDbUid;
	const char		*debugString;
} PQL_DB_INDEX_T;

/**
 * PQL DB Table의 UID 구성 요소와 Tool Option 과 비교 후 return 값.
 * @note BEST > GOOD > MISMATCH 이어야 함.
 * @see pql_ddi.c:_CheckUidElementMatch()
 */
typedef enum {
	PQL_ELEMENT_MISMATCH	= 0,
	PQL_ELEMENT_GOOD_MATCH	= 1,
	PQL_ELEMENT_BEST_MATCH	= 2
} PQL_DB_ELEMENT_MATCH_T;

/**
 * NORMAL과 SLIM TOOL 구분하기 위한 typedef
 * @note 현재 사용 안함. 추후 SLIM/NORMAL Tool에 따른 화질 분기를 위해 대응.
 * 		 추후 사용이 필요할 경우 modeldef.c:OSA_MD_GetPQLDBIndex() 에서 Tool option에 따라 구분 필요함.
 */
typedef enum {
	PQL_TOOL_NORMAL,
	PQL_TOOL_SLIM
} PQL_TOOL_TYPE_T;

#endif /* #ifndef _PQL_UID_H_ */
