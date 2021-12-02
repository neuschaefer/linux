/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 2008 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file country.h
 *
 * This header file defines the macro/value definitions defining country/group.
 *
 *	@author 	David (DONG-IL) Lee
 *	@version	1.0
 *	@date		2008.05.09
 *	@note
 *	@see
 */
#ifndef	_COUNTRY_H_
#define	_COUNTRY_H_
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _COMMON_H_
//#include <common.h>
	#error	NOTE: "common.h" should be included before including "country.h"!
#endif	/* _COMMON_H_ */

#include "ascii.h"
#include "country_core.h"

/*---------------------------------------------------------
    Type 정의
    (Type Definitions)
---------------------------------------------------------*/
typedef UINT32	RMM_COMMON_CODE_T;
typedef UINT32	RMM_REGION_CODE_T;
typedef UINT8	RMM_GROUP_CODE_T;
typedef UINT32	RMM_COUNTRY_CODE_T;


typedef enum
{
	RMM_COUNTRY_ORDER_UK	= 0x00,	// united kingdom
	RMM_COUNTRY_ORDER_FR,			// france
	RMM_COUNTRY_ORDER_DE,			// germany
	RMM_COUNTRY_ORDER_IT,			// italy
	RMM_COUNTRY_ORDER_ES,			// spain
	RMM_COUNTRY_ORDER_SE,			// sweden
	RMM_COUNTRY_ORDER_FI,			// finland
	RMM_COUNTRY_ORDER_AT,			// austria
	RMM_COUNTRY_ORDER_BE,			// belgium
	RMM_COUNTRY_ORDER_CZ,			// czech repulbic
	RMM_COUNTRY_ORDER_DK,			// denmark
	RMM_COUNTRY_ORDER_GR,			// greece
	RMM_COUNTRY_ORDER_LU,			// luxembourg
	RMM_COUNTRY_ORDER_NL,			// netherlands
	RMM_COUNTRY_ORDER_BG,			// bulgaria
	RMM_COUNTRY_ORDER_HR,			// croatia
	RMM_COUNTRY_ORDER_HU,			// hungary
	RMM_COUNTRY_ORDER_IE,			// ireland
	RMM_COUNTRY_ORDER_NO,			// norway
	RMM_COUNTRY_ORDER_PL,			// poland
	RMM_COUNTRY_ORDER_PT,			// portugal
	RMM_COUNTRY_ORDER_RO,			// romania
	RMM_COUNTRY_ORDER_RU,			// russian federation
	RMM_COUNTRY_ORDER_RS,			// sebia
	RMM_COUNTRY_ORDER_SI,			// slovenia
	RMM_COUNTRY_ORDER_CH,			// switzerland
	RMM_COUNTRY_ORDER_TR,			// turkey
	RMM_COUNTRY_ORDER_MA,			// morocco
	RMM_COUNTRY_ORDER_UA,			// ukraine
	RMM_COUNTRY_ORDER_KZ,			// kazakhstan
	RMM_COUNTRY_ORDER_LT,			// lithuania
	RMM_COUNTRY_ORDER_EE,			// estonia
	RMM_COUNTRY_ORDER_LV,			// latvia
	RMM_COUNTRY_ORDER_SK,			// slovakia
	RMM_COUNTRY_ORDER_AL,			// albania
	RMM_COUNTRY_ORDER_BA,			// bosnia and herzegovina
	RMM_COUNTRY_ORDER___,			// __

	RMM_COUNTRY_ORDER_UNKNOWN	= 0xff

} RMM_COUNTRY_ORDER_T;


typedef struct RMM_COUNTRY_ELMT_T {
	RMM_COUNTRY_CODE_T	code;
	char			*code_name;
	struct RMM_COUNTRY_SPEC_STRUCT_T {
		UINT32		patpmtsdt_map			:1,			// (00) support Service Map Using PAT,PMT/SDT
					mheg5					:1,			// (01) support MHEG-5
					ci						:1,			// (02) support CI
					oad						:1,			// (03) support OAD
					lcn						:1,			// (04) support LCN
					vhf						:1,			// (05) support DTV VHF tuning
					nordig					:1,			// (06) support Nordig Spec.
					dtv						:1,			// (07) support DTV Broadcasting (ATSC/DVB/DTMB/ISDB)
					acap					:1,			// (08) support ACAP
					dvr_ready				:1,			// (09) support DVR Ready
					ttx_mode				:2,			// (10) support Teletext Mode Type
														// (11) ...
					acms					:1,			// (12) support ACMS
					dAboost					:1,			// (13) support Digital Auto Booster
					mono					:1,			// (14) support Forced Mono Mode
					i_ii_save				:1,			// (15) support Audio I/II Selection Saving
				    init_menu_lang  		:1,			// (16) specify initial MENU Language
					satellite				:1,			// (17) support Satellite
					cable_digital			:1,			// (18) support Digital Cable
					cable_analog			:1,			// (19) support Analog Cable
					acc						:1,			// (20) support ATVCC (for ATSC)
					dcc						:1,			// (21) support DTVCC (for ATSC)
					exclusive_dtv_cadtv		:1,			// (22) support exclusive_dtv_cadtv(for EU)
					wss_support				:1,			// (23) support WSS
					attx					:1,			// (24) support Analog Teletext
					dttx					:1,			// (25) support Digital Teletext
					ttx_subt				:1,			// (26) support Teletext Subtitle
					dvb_subt				:1,			// (27) support DVB Subtitle
					adcc					:1,			// (28) support adcc for BR
					psip					:1,			// (29) support psip
					si						:1,			// (30) support si
					ci_plus					:1;			// (31) support CI Plus(for DVB)
	} spec;
	UINT8			country_order;	// it is used for ACMS search
									// : priority is required
	char			*full_name;
} RMM_COUNTRY_ELMT_T;

typedef struct RMM_GROUP_ELMT_T {
	RMM_GROUP_CODE_T	code;
	char				*code_name;
	char				*full_name;
	RMM_COUNTRY_ELMT_T	*pList;
} RMM_GROUP_ELMT_T;

typedef enum {
	RMM_ERR_NO_ERR	= 0,
	RMM_ERR_INIT_FAIL,
	RMM_ERR_INIT_FAIL_ALREADY_INITIALIZED,
	RMM_ERR_NOT_INITIALIZED,
	RMM_ERR_UNKNOWN_CODE_TYPE,
	RMM_ERR_UNKNOWN_REGION_CODE,
	RMM_ERR_UNKNOWN_GROUP_CODE,
	RMM_ERR_UNKNOWN_COUNTRY_CODE,
	RMM_ERR_INVALID
} RMM_ERR_T;

typedef enum {
	RMM_CODE_TYPE_REGION = 0,
	RMM_CODE_TYPE_GROUP,
	RMM_CODE_TYPE_COUNTRY
} RMM_CODE_TYPE_T;

typedef enum {
	RMM_PRINT_DEFAULT				= 0x00000000,
	RMM_PRINT_ALL_GROUP				= 0x10000000,
	RMM_PRINT_THIS_GROUP			= 0x01000000,
	RMM_PRINT_THIS_COUNTRY			= 0x00100000,
	RMM_PRINT_CRNT_GROUP			= 0x02000000,
	RMM_PRINT_CRNT_COUNTRY			= 0x00200000,
	RMM_RETURN_THIS_GROUP			= 0x00001000,
	RMM_RETURN_CRNT_GROUP			= 0x00002000,
	RMM_RETURN_THIS_GROUP_NAME		= 0x00000100,
	RMM_RETURN_CRNT_GROUP_NAME		= 0x00000200,
	RMM_RETURN_THIS_COUNTRY_NAME	= 0x00000010,
	RMM_RETURN_CRNT_COUNTRY_NAME	= 0x00000020,
	RMM_PRINT_MASK					= 0xffff0000,
	RMM_RETURN_MASK					= 0x0000ffff
} RMM_GENERAL_FLG_T;

/*---------------------------------------------------------
    매크로 함수 정의
    (Macro Definitions)
---------------------------------------------------------*/
//#define	RMM_INVALID_GROUP_CODE				0xffff
//#define	RMM_INVALID_COUNTRY_CODE			0xffff

#define	RMM_GROUP_CODE					RMM_GROUP_OF(gnRmmRegion)
#define	RMM_COUNTRY_CODE				RMM_COUNTRY_OF(gnRmmRegion)

#define	RMM_SET_GROUP_CODE(code)		RMM_SetCode(RMM_CODE_TYPE_GROUP,	(RMM_COMMON_CODE_T)code)
#define	RMM_SET_COUNTRY_CODE(code)		RMM_SetCode(RMM_CODE_TYPE_COUNTRY,	(RMM_COMMON_CODE_T)code)

#define RMM_NUM_OF_GROUP				gnRmmNumOfGroup
#define	RMM_NUM_OF_COUNTRY_IN_GROUP		(gnRmmNumOfCountryInGroup)
#define	RMM_SUPPORT_SPEC(a)				((gpRmmCountry)?(gpRmmCountry->spec.a):0)

#define RMM_CURR_COUNTRY_SHORT_NAME		((gpRmmCountry)?(gpRmmCountry->code_name):"")
#define RMM_CURR_COUNTRY_FULL_NAME		((gpRmmCountry)?(gpRmmCountry->full_name):"")
#define RMM_CURR_GROUP_SHORT_NAME		((gpRmmGroup)?(gpRmmGroup->code_name):"")
#define RMM_CURR_GROUP_FULL_NAME		((gpRmmGroup)?(gpRmmGroup->full_name):"")

#define	RMM_GROUP_CODE_OF(idx)				(gapGroupTable[idx]->code)
#define	RMM_COUNTRY_CODE_OF(idx)			((gpRmmGroup)?(gpRmmGroup->pList[idx].code):RMM_COUNTRY_CODE_ZZ)
#define RMM_GROUP_SHORT_NAME_OF(idx)		(gapGroupTable[idx]->code_name)
#define RMM_GROUP_FULL_NAME_OF(idx)			(gapGroupTable[idx]->full_name)
#define RMM_COUNTRY_SHORT_NAME_OF(idx)		((gpRmmGroup)?(gpRmmGroup->pList[idx].code_name):"")
#define RMM_COUNTRY_FULL_NAME_OF(idx)		((gpRmmGroup)?(gpRmmGroup->pList[idx].full_name):"")

#define RMM_NUM_OF_GROUP_IN(sys)				(sizeof(gapGroupTable##sys)/sizeof(gapGroupTable##sys[0])-1)
#define	RMM_SYS_GROUP_TABLE(sys)				gapGroupTable##sys
#define	RMM_SYS_GROUP_CODE_OF(tbl, idx)			(tbl[idx]->code)
#define RMM_SYS_GROUP_SHORT_NAME_OF(tbl, idx)	(tbl[idx]->code_name)
#define RMM_SYS_GROUP_FULL_NAME_OF(tbl, idx)	(tbl[idx]->full_name)

/*---------------------------------------------------------
	함수 선언 (Function Declaration)
---------------------------------------------------------*/
/**
 * @fn			RMM_ERR_T RMM_Init(void)
 * @brief		Initialize RMM module
 * @param		void
 * @exception	void
 * @return		RMM_ERR_T
 *
 * RMM_Init() should be called ONCE very early time in booting. It initialize
 * RMM module which enables country based runtime branching.
 *
 * @sa RMM_ERR_T
 */
RMM_ERR_T RMM_Init(void);

/**
 * @fn			RMM_ERR_T RMM_SetCode(RMM_CODE_TYPE_T code_type, RMM_COMMON_CODE_T code)
 * @brief		Set Region / Group / Country code
 * @param		RMM_CODE_TYPE_T code_type (in)
 * @param		RMM_COMMON_CODE_T code (in)
 * @exception	void
 * @return		RMM_ERR_T
 *
 * It set Region / Group / Country code. It should be used in changing or
 * setting Region / Group / Country code. It is proper to be call this function
 * once very early time in booting to initialize the country code for
 * branching.
 *
 * @sa RMM_ERR_T, RMM_CODE_TYPE_T, RMM_COMMON_CODE_T
 */
RMM_ERR_T RMM_SetCode(RMM_CODE_TYPE_T code_type, RMM_COMMON_CODE_T code);

/**
 * @fn			RMM_ERR_T RMM_GetNumOfCountryIn(RMM_GROUP_CODE_T group_code)
 * @brief		Get number of countries in country group
 * @param		RMM_GROUP_CODE_T group_code (in)
 * @exception	0
 * @return		UINT32
 *
 * It returns number of countries which is included in the country group
 * specified by 'group_code'. If it returns '0', then it means that there is
 * not the specified country group or the specified country group includes no
 * country at all.
 *
 * @sa RMM_GROUP_CODE_T
 */
UINT32 RMM_GetNumOfCountryIn(RMM_GROUP_CODE_T group_code);

/**
 * @fn			RMM_GROUP_CODE_T RMM_GetGroupCodeOf(RMM_COUNTRY_CODE_T country_code)
 * @brief		Get group code which includes the specified country code
 * @param		RMM_COUNTRY_CODE_T country_code (in)
 * @exception	RMM_GROUP_CODE_ZZ
 * @return		RMM_GROUP_CODE_T
 *
 * It returns group code which specifies country group which includes the
 * country which is specified by 'country_code'. If it returns
 * RMM_GROUP_CODE_ZZ then it means that the country which is specified by
 * 'country_code' is not included any country group in the system.
 *
 * @sa RMM_GROUP_CODE_T, RMM_COUNTRY_CODE_T, RMM_GROUP_CODE_ZZ
 */
RMM_GROUP_CODE_T RMM_GetGroupCodeOf(RMM_COUNTRY_CODE_T country_code);

/**
 * @fn			void RMM_GetShortNameOf(RMM_COMMON_CODE_T code, char *pShortName)
 * @brief		Get short name of the specified code
 * @param		RMM_COMMON_CODE_T code (in)
 * @param		char *pShortName (out)
 * @exception	void
 * @return		void
 *
 * It build short name of the code which is specified by 'code' in the buffer
 * specified by 'pShortName'.
 *
 * @note		The buffer specified by 'pShortName' must be allocated before
 *				calling this function and outside of this function by caller.
 *				The size of buffer must be more than '3' because it must store
 *				2 bytes for shortname and 1 byte for null terminator.
 *
 * @sa RMM_COMMON_CODE_T
 */
void RMM_GetShortNameOf(RMM_COMMON_CODE_T code, char *pShortName);

/**
 * @fn			char *RMM_GetLongNameOf(RMM_GENERAL_FLG_T flg, RMM_COMMON_CODE_T code)
 * @brief		Get long name of the specified code
 * @param		RMM_GENERAL_FLG_T flg (in)
 * @param		RMM_COMMON_CODE_T code (in)
 * @exception	void
 * @return		char *
 *
 * It returns a pointer to string of long name which is specified by 'flg' and
 * 'code'. The kind of code is specified by flg (flag).
 *
 * @note		Becase it returns a pointer to constant string data, the
 *				returned name string must not be modified.
 *
 * @sa RMM_COMMON_CODE_T, RMM_GENERAL_FLG_T
 */
char *RMM_GetLongNameOf(RMM_GENERAL_FLG_T flg, RMM_COMMON_CODE_T code);

/**
 * @fn			RMM_GROUP_CODE_T RMM_PrintRegionCode(RMM_GENERAL_FLG_T flag4extra_info, RMM_REGION_CODE_T region_code)
 * @brief		Print Region / Group / Country code in general
 * @param		RMM_GENERAL_FLG_T flag4extra_info (in)
 * @param		RMM_REGION_CODE_T region_code
 * @exception	RMM_GROUP_CODE_ZZ
 * @return		RMM_GROUP_CODE_T
 *
 * It prints region / group / country code and its relations. It also returns
 * group code of the region which is specified by 'region_code'.
 * 'flag4extra_info' specifies the print option and return option.
 *
 * @sa RMM_GENERAL_FLG_T, RMM_REGION_CODE_T, RMM_GROUP_CODE_T
 */
RMM_GROUP_CODE_T RMM_PrintRegionCode(RMM_GENERAL_FLG_T flag4extra_info, RMM_REGION_CODE_T region_code);

/**
 * @fn			void RMM_PrintCurrRegionCode(void)
 * @brief		Print current Region code
 * @param		void
 * @exception	void
 * @return		void
 *
 * It prints current region code.
 *
 */
void RMM_PrintCurrRegionCode(void);



/*******************************************************************************
 *
 *
 *			  <<< GROUP / COUNTRY SPEC. CONFIGURATION SECTION >>>
 *
 *
 ******************************************************************************/

/* 1. GROUP DEFINITIONS : MODIFY this section */
#define	RMM_GROUP_CODE_KR	RMM_CODE_VAL_GG(0x01, K,R)			// group Korea, Republic of
#define	RMM_GROUP_CODE_US	RMM_CODE_VAL_GG(0x02, U,S)			// group United States
#define	RMM_GROUP_CODE_BR	RMM_CODE_VAL_GG(0x03, B,R)			// group Brazil
#define	RMM_GROUP_CODE_EU	RMM_CODE_VAL_GG(0x04, E,U)			// group EU
#define	RMM_GROUP_CODE_CN	RMM_CODE_VAL_GG(0x05, C,N)			// group China
#define	RMM_GROUP_CODE_AU	RMM_CODE_VAL_GG(0x06, A,U)			// group Australia
#define	RMM_GROUP_CODE_SG	RMM_CODE_VAL_GG(0x07, S,G)			// group Singapore
#define	RMM_GROUP_CODE_ZA	RMM_CODE_VAL_GG(0x08, Z,A)			// group South Africa
#define	RMM_GROUP_CODE_VN	RMM_CODE_VAL_GG(0x09, V,N)			// group Viet Nam
#define	RMM_GROUP_CODE_TW	RMM_CODE_VAL_GG(0x0a, T,W)			// group Taiwan
#define	RMM_GROUP_CODE_XA	RMM_CODE_VAL_GG(0x0b, X,A)			// group [VIRTUAL] 중남미 아날로그 국가, NTSC
#define	RMM_GROUP_CODE_XB	RMM_CODE_VAL_GG(0x0c, X,B)			// group [VIRTUAL] 중아,아주 아날로그 국가, PAL
#define	RMM_GROUP_CODE_IL	RMM_CODE_VAL_GG(0x0d, I,L)			// group Israel
#define	RMM_GROUP_CODE_ID	RMM_CODE_VAL_GG(0x0e, I,D)			// group Indonesia
#define	RMM_GROUP_CODE_MY	RMM_CODE_VAL_GG(0x0f, M,Y)			// group Malaysia
#define RMM_GROUP_CODE_IR	RMM_CODE_VAL_GG(0x10, I,R)			// group Iran
#define RMM_GROUP_CODE_HK	RMM_CODE_VAL_GG(0x11, H,K)			// group HongKong
#define RMM_GROUP_CODE_JP	RMM_CODE_VAL_GG(0x12, J,P)			// group Japan
#define	RMM_GROUP_CODE_ZZ	RMM_CODE_VAL_GG(0xff, Z,Z)			// group [VIRTUAL] for termination (invalid group code)

/* 2. GROUP CONDITION DEFINITIONS : MODIFY this section */
#define	RMM_GROUP_IS_KR		(RMM_GROUP_CODE==RMM_GROUP_CODE_KR)	// group == Korea, Republic of
#define	RMM_GROUP_IS_US		(RMM_GROUP_CODE==RMM_GROUP_CODE_US)	// group == United States
#define	RMM_GROUP_IS_BR		(RMM_GROUP_CODE==RMM_GROUP_CODE_BR)	// group == Brazil
#define	RMM_GROUP_IS_JP		(RMM_GROUP_CODE==RMM_GROUP_CODE_JP)	// group == Japan

#define	RMM_GROUP_IS_EU		(RMM_GROUP_CODE==RMM_GROUP_CODE_EU)	// group == EU
#define	RMM_GROUP_IS_CN		(RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK)//group ==China
#define	RMM_GROUP_IS_AU		(RMM_GROUP_CODE==RMM_GROUP_CODE_AU)	// group == Australia
#define	RMM_GROUP_IS_SG		(RMM_GROUP_CODE==RMM_GROUP_CODE_SG)	// group == Singapore
#define	RMM_GROUP_IS_ZA		(RMM_GROUP_CODE==RMM_GROUP_CODE_ZA)	// group == South Africa
#define	RMM_GROUP_IS_VN		(RMM_GROUP_CODE==RMM_GROUP_CODE_VN)	// group == Viet Nam

#define	RMM_GROUP_IS_TW		(RMM_GROUP_CODE==RMM_GROUP_CODE_TW)	// group == Taiwan

#define	RMM_GROUP_IS_XA		(RMM_GROUP_CODE==RMM_GROUP_CODE_XA)	// group [VIRTUAL] 중남미 아날로그 국가, NTSC
#define	RMM_GROUP_IS_XB		(RMM_GROUP_CODE==RMM_GROUP_CODE_XB)	// group [VIRTUAL] 중아,아주 아날로그 국가, PAL
#define	RMM_GROUP_IS_IL		(RMM_GROUP_CODE==RMM_GROUP_CODE_IL)	// group == Israel
#define	RMM_GROUP_IS_ID		(RMM_GROUP_CODE==RMM_GROUP_CODE_ID)	// group == Indonesia
#define	RMM_GROUP_IS_MY		(RMM_GROUP_CODE==RMM_GROUP_CODE_MY)	// group == Malaysia
#define RMM_GROUP_IS_IR		(RMM_GROUP_CODE==RMM_GROUP_CODE_IR) //group ==Iran
#define RMM_GROUP_IS_HK		(RMM_GROUP_CODE==RMM_GROUP_CODE_HK) //group == HongKong
#define	RMM_GROUP_IS_ZZ		(RMM_GROUP_CODE==RMM_GROUP_CODE_ZZ)	// group == ZZ (invalid)

#define	RMM_GROUP_IS_ATSC	(RMM_GROUP_IS_KR || RMM_GROUP_IS_US || RMM_GROUP_IS_BR)
#define	RMM_GROUP_IS_DVB	(RMM_GROUP_IS_EU || RMM_GROUP_IS_CN || RMM_GROUP_IS_AU || RMM_GROUP_IS_SG || RMM_GROUP_IS_ZA || RMM_GROUP_IS_VN \
							||RMM_GROUP_IS_IL ||RMM_GROUP_IS_ID ||RMM_GROUP_IS_MY || RMM_GROUP_IS_IR || RMM_GROUP_IS_HK/*zhangze 20101120 added HongKong group*/)
#define	RMM_GROUP_IS_ETC	(RMM_GROUP_IS_TW || RMM_GROUP_IS_ZZ || RMM_GROUP_IS_XA  || RMM_GROUP_IS_XB)
#define	RMM_GROUP_IS_EMERGING_MARKET	(RMM_GROUP_IS_AU || RMM_GROUP_IS_SG || RMM_GROUP_IS_ZA)
#define RMM_GROUP_IS_EMERGING_MARKET_ALL (RMM_GROUP_IS_AU || RMM_GROUP_IS_SG || RMM_GROUP_IS_ZA || RMM_GROUP_IS_VN \
										   ||RMM_GROUP_IS_IL ||RMM_GROUP_IS_ID ||RMM_GROUP_IS_MY || RMM_GROUP_IS_IR||RMM_GROUP_IS_XB )

/* 3. COUNTRY DEFINITIONS : MODIFY this section */
#include "country_codes.h"		// : do not modify this file (refer to http://en.wikipedia.org/wiki/ISO_3166-1)
#define	RMM_COUNTRY_CODE___	RMM_CODE_VAL_CC(_,_,_)	// [VIRTUAL] __
#define	RMM_COUNTRY_CODE_XA	RMM_CODE_VAL_CC(X,A,A)	// [VIRTUAL] 중남미 아날로그 국가, NTSC
#define	RMM_COUNTRY_CODE_XB	RMM_CODE_VAL_CC(X,A,B)	// [VIRTUAL] 중아,아주 아날로그 국가, PAL
//XCA~XFZ(26*4) : reserved for CN City Option

//add by lihuifeng for city option
#define RMM_COUNTRY_CODE_CNBJ 			RMM_CODE_VAL_CC(X,C,A)// [VIRTUAL] China Mainland - Beijing
#define RMM_COUNTRY_CODE_CNCC			RMM_CODE_VAL_CC(X,C,B)// [VIRTUAL] China Mainland - changchun
#define RMM_COUNTRY_CODE_CNCS		RMM_CODE_VAL_CC(X,C,C)// [VIRTUAL] China Mainland - changsha
#define RMM_COUNTRY_CODE_CNCD		RMM_CODE_VAL_CC(X,C,D)// [VIRTUAL] China Mainland -chengdu
#define RMM_COUNTRY_CODE_CNDL 		RMM_CODE_VAL_CC(X,C,E)// [VIRTUAL] China Mainland - dalian
#define RMM_COUNTRY_CODE_CNFZ 		RMM_CODE_VAL_CC(X,C,F)// [VIRTUAL] China Mainland - fuzhou
#define RMM_COUNTRY_CODE_CNGZ 			RMM_CODE_VAL_CC(X,C,G)// [VIRTUAL] China Mainland - guangzhou
#define RMM_COUNTRY_CODE_CNHB 		RMM_CODE_VAL_CC(X,C,H)// [VIRTUAL] China Mainland - hebei
#define RMM_COUNTRY_CODE_CNHF 		RMM_CODE_VAL_CC(X,C,I)// [VIRTUAL] China Mainland - hefei
#define RMM_COUNTRY_CODE_CNJL 		RMM_CODE_VAL_CC(X,C,J)// [VIRTUAL] China Mainland - jilin       added by yinxianghua 20091117
#define RMM_COUNTRY_CODE_CNNB 		RMM_CODE_VAL_CC(X,C,K)// [VIRTUAL] China Mainland - ningbo
#define RMM_COUNTRY_CODE_CNSY 		RMM_CODE_VAL_CC(X,C,L)// [VIRTUAL] China Mainland - shenyang
#define RMM_COUNTRY_CODE_CNWH 		RMM_CODE_VAL_CC(X,C,M)// [VIRTUAL] China Mainland - Wuhan
#define RMM_COUNTRY_CODE_CNXA 			RMM_CODE_VAL_CC(X,C,N)// [VIRTUAL] China Mainland -xian
#define RMM_COUNTRY_CODE_CNXM 	RMM_CODE_VAL_CC(X,C,O)// [VIRTUAL] China Mainland -xiamen
#define RMM_COUNTRY_CODE_CNZZ 	RMM_CODE_VAL_CC(X,C,P)// [VIRTUAL] China Mainland -zhengzhou     added by yinxianghua 20091203
#define RMM_COUNTRY_CODE_CNZH 		RMM_CODE_VAL_CC(X,C,Q)// [VIRTUAL] China Mainland - zhuhai
//end
#define RMM_COUNTRY_CODE_CNO RMM_CODE_VAL_CC(X,F,Z)	// [VIRTUAL] China Mainland Others.
#define	RMM_COUNTRY_CODE_ZZ	RMM_CODE_VAL_CC(Z,Z,Z)	// [VIRTUAL] invalid country (terminator)

/* 4. COUNTRY CONDITION DEFINITIONS : MODIFY this section */
#include "country_conditions.h"	// : do not modify this file (refer to http://en.wikipedia.org/wiki/ISO_3166-1)
#define	RMM_COUNTRY_IS_XA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_XA)	// country == [VIRTUAL] 중남미 아날로그 국가, NTSC
#define	RMM_COUNTRY_IS_XB	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_XB)	// country == [VIRTUAL] 중아,,아주 아날로그 국가, PAL
#define	RMM_COUNTRY_IS___	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE___)	// country == [VIRTUAL] Analogue Only

#define	GROUP_TABLE_ATSC_SIZE	6	// : MODIFY this definition
#define	GROUP_TABLE_DVB_SIZE	14	// : MODIFY this definition
#define	GROUP_TABLE_SIZE		(GROUP_TABLE_ATSC_SIZE+GROUP_TABLE_DVB_SIZE-1)


enum {
	ASIA_Default		= 0,
	LOCATION_INDIA		= 1,
	LOCATION_THAILAND	= 2,
	LOCATION_BBTV_OFF  = 3,
};
#ifdef	_COUNTRY_C_

/* 5. GROUP ASSIGNMENT in C : MODIFY this section */
#include	"country_specs.h"			// : MODIFY this file
ASSIGN_COUNTRY_GROUP(0x01, K,R, "Korea, Republic of");
ASSIGN_COUNTRY_GROUP(0x02, U,S, "United States");
ASSIGN_COUNTRY_GROUP(0x03, B,R, "Brazil");
ASSIGN_COUNTRY_GROUP(0x04, E,U, "EU");
ASSIGN_COUNTRY_GROUP(0x05, C,N, "China");
ASSIGN_COUNTRY_GROUP(0x06, A,U, "Australia");
ASSIGN_COUNTRY_GROUP(0x07, S,G, "Singapore");
ASSIGN_COUNTRY_GROUP(0x08, Z,A, "South Africa");
ASSIGN_COUNTRY_GROUP(0x09, V,N, "Viet Nam");
ASSIGN_COUNTRY_GROUP(0x0a, T,W, "Taiwan");
ASSIGN_COUNTRY_GROUP(0x0b, X,A, "[VIRTUAL] A-CSA");
ASSIGN_COUNTRY_GROUP(0x0c, X,B, "[VIRTUAL] A-ASIA");
ASSIGN_COUNTRY_GROUP(0x0d, I,L, "Israel");
ASSIGN_COUNTRY_GROUP(0x0e, I,D, "Indonesia");
ASSIGN_COUNTRY_GROUP(0x0f, M,Y, "Malaysia");
ASSIGN_COUNTRY_GROUP(0x10, I,R, "Iran");
ASSIGN_COUNTRY_GROUP(0x11, H,K, "Hong Kong");
ASSIGN_COUNTRY_GROUP(0x12, J,P, "Japan");
ASSIGN_COUNTRY_GROUP(0xff, Z,Z, "[VIRTUAL] invalid group (terminator)");

RMM_GROUP_ELMT_T	*gapGroupTableATSC[GROUP_TABLE_ATSC_SIZE] = {	// : MODIFY this array
	// you need to modify GROUP_TABLE_ATSC_SIZE when you modify this array
	// you need to modify gapGroupTable when you modify this array
	&Group_KR,	&Group_US,	&Group_BR,	&Group_XA,&Group_JP,
	&Group_ZZ
};
RMM_GROUP_ELMT_T	*gapGroupTableDVB[GROUP_TABLE_DVB_SIZE] = {	// : MODIFY this array
	// you need to modify GROUP_TABLE_DVB_SIZE when you modify this array
	// you need to modify gapGroupTable when you modify this array
	&Group_EU,	&Group_CN,	&Group_XB,	&Group_AU,	&Group_SG,
	&Group_ZA,	&Group_VN,	&Group_TW,	&Group_IL,	&Group_ID,
	&Group_MY,	&Group_IR, 	&Group_HK,	&Group_ZZ
};

RMM_GROUP_ELMT_T	*gapGroupTable[] = {	// : MODIFY this array
	&Group_KR,	&Group_US,	&Group_BR,	&Group_XA,  &Group_JP,
	&Group_EU,	&Group_CN,	&Group_XB,	&Group_AU,	&Group_SG,
	&Group_ZA,	&Group_VN,	&Group_TW,	&Group_IL,	&Group_ID,
	&Group_MY,	&Group_IR, 	&Group_HK,	&Group_ZZ
};

#if	(SYS_DVB)
UINT32				gnRmmRegion					= RMM_CODE_VAL_GG_CC(0x04, E,U, G,B,R);
RMM_GROUP_ELMT_T	*gpRmmGroup					= &(Group_EU);
RMM_COUNTRY_ELMT_T	*gpRmmCountry				= &(aCountryList_EU[0]);
#elif (SYS_ATSC)
UINT32				gnRmmRegion					= RMM_CODE_VAL_GG_CC(0x01, K,R, K,O,R);
RMM_GROUP_ELMT_T	*gpRmmGroup					= &(Group_KR);
RMM_COUNTRY_ELMT_T	*gpRmmCountry				= &(aCountryList_KR[0]);
#endif
UINT32				gnRmmNumOfGroup				= (sizeof(gapGroupTable)/sizeof(gapGroupTable[0])-1);
UINT32				gnRmmNumOfGroupInATSC		= (sizeof(gapGroupTableATSC)/sizeof(gapGroupTableATSC[0])-1);
UINT32				gnRmmNumOfGroupInDVB		= (sizeof(gapGroupTableDVB)/sizeof(gapGroupTableDVB[0])-1);
UINT32				gnRmmNumOfCountryInGroup	= 0;

#else	/* !_COUNTRY_C_ */

extern RMM_GROUP_ELMT_T		*gapGroupTableATSC[GROUP_TABLE_ATSC_SIZE];
extern RMM_GROUP_ELMT_T		*gapGroupTableDVB[GROUP_TABLE_DVB_SIZE];
extern RMM_GROUP_ELMT_T		*gapGroupTable[GROUP_TABLE_SIZE];
extern	UINT32				gnRmmRegion;
extern	RMM_GROUP_ELMT_T	*gpRmmGroup;
extern	RMM_COUNTRY_ELMT_T	*gpRmmCountry;
extern	UINT32				gnRmmNumOfGroup;
extern	UINT32				gnRmmNumOfGroupInATSC;
extern	UINT32				gnRmmNumOfGroupInDVB;
extern	UINT32				gnRmmNumOfCountryInGroup;

#endif	/* _COUNTRY_C_ */


#ifdef __cplusplus
}
#endif
#endif	/* _COUNTRY_H_ */
