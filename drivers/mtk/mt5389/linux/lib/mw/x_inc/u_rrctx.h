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
/*----------------------------------------------------------------------------*/
/*! @file u_rrctx.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This contains RRCTX library exported macros, definitons,
 *         and structures.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_RRCTX_H_
#define _U_RRCTX_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_RRCTX RRCTX 
 *
 *  @ingroup groupMW
 *  @brief The RRCTX  module is used to get rating information.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could get rating infromation
 *   from the exported API of RRCTX VCHIP engine.
 *
 *  @see groupMW_RRCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Notification Reason used in (*x_rrctx_nfy_fct)() */
#define RRCTX_REASON_UNKNOWN            ((UINT32) 0x00000000)   /**< Unknown reason. */
#define RRCTX_REASON_RATE_ENABLE        MAKE_BIT_MASK_32(1)     /**< The rating system is enabled. */
#define RRCTX_REASON_RATE_DISABLE       MAKE_BIT_MASK_32(2)     /**< The rating system is disabled. */
#define RRCTX_REASON_RATE_UPDATED       MAKE_BIT_MASK_32(3)     /**< The settings of the rating system are updated. */
#define RRCTX_REASON_RATE_AVAIL         MAKE_BIT_MASK_32(4)     /**< The schema/content of the rating system is available. */
#define RRCTX_REASON_RATE_UNAVAIL       MAKE_BIT_MASK_32(5)     /**< The schema/content of the rating system isn¡¦t available. */
#define RRCTX_REASON_HANDLE_CLOSED      MAKE_BIT_MASK_32(6)     /**< The opened rating handle was closed. */
#define RRCTX_REASON_STORAGE_CLEAR      MAKE_BIT_MASK_32(7)     /**< The opened rating handle was closed. */

/* Return values */
#define RRCTXR_OK                       ((INT32)    0)		/**< Every is OK. */
#define RRCTXR_NOT_INIT                 ((INT32)   -1)		/**< Not initialized. */
#define RRCTXR_ALREADY_INIT             ((INT32)   -2)		/**< Already initialized. */
#define RRCTXR_CMD_NOT_SUPPORT          ((INT32)   -3)		/**< The command is not supported. */
#define RRCTXR_DUP_RATING_TYPE          ((INT32)   -4)		/**< The message queue of RRCTX is exit. */
#define RRCTXR_INV_ARG                  ((INT32)   -5)		/**< Invalid parameters. */
#define RRCTXR_INV_HANDLE               ((INT32)   -6)		/**< Invalid handle */
#define RRCTXR_OUT_OF_HANDLE            ((INT32)   -7)		/**< Out of handle. */
#define RRCTXR_OUT_OF_MEM               ((INT32)   -8)		/**< Out of memery. */
#define RRCTXR_OUT_OF_RESOURCE          ((INT32)   -9)		/**< Out of resource. */
#define RRCTXR_ENG_NOT_FOUND            ((INT32)  -10)		/**< Engine not found. */
#define RRCTXR_INFO_NOT_FOUND           ((INT32)  -11)		/**< Event infomation not found. */
#define RRCTXR_INTL_ERROR               ((INT32)  -12)		/**< Internal error. */
#define RRCTXR_REC_NOT_FOUND            ((INT32)  -13)		/**< Record not found. */
#define RRCTXR_REC_ADD_FAIL             ((INT32)  -14)		/**< Recode added fail. */
#define RRCTXR_REC_DEL_FAIL             ((INT32)  -15)		/**< Recode deleted fail. */
#define RRCTXR_REC_GET_FAIL             ((INT32)  -16)		/**< Recode got fail. */
#define RRCTXR_DB_NOT_FOUND             ((INT32)  -17)		/**< Database not found. */
#define RRCTXR_DB_END_OF_ITERATION      ((INT32)  -18)		/**< Database end of iteration. */
#define RRCTXR_DB_CANT_CREATE_FILE      ((INT32)  -19)		/**< Can not create database. */
#define RRCTXR_DB_CANT_OPEN_FILE        ((INT32)  -20)		/**< Can not open database. */
#define RRCTXR_DB_MASTER_CHANGED        ((INT32)  -21)		/**< Database changed. */
#define RRCTXR_DB_NO_WRITE_LOCK         ((INT32)  -22)		/**< No write locked. */
#define RRCTXR_DB_WRITE_LOCKED          ((INT32)  -23)		/**< Write locked. */
#define RRCTXR_DB_MODIFIED              ((INT32)  -24)		/**< Database modified. */
#define RRCTXR_DB_DEV_ERR               ((INT32)  -25)		/**< Device error. */
#define RRCTXR_DB_OS_ERR                ((INT32)  -26)		/**< OS error. */
#define RRCTXR_DB_PERM_DENY             ((INT32)  -27)		/**< Permision denied. */
#define RRCTXR_DB_EOF                   ((INT32)  -28)		/**< End of file. */
#define RRCTXR_DB_FILESYS_FULL          ((INT32)  -29)		/**< File system is full. */
#define RRCTXR_DB_LIMIT_EXCEED          ((INT32)  -30)		/**< Reached maximum number of CDB objects . */
#define RRCTXR_DB_INV_NAME              ((INT32)  -31)		/**< Invalid database. */
#define RRCTXR_DB_INV_FILE              ((INT32)  -32)		/**< Invalid file. */
#define RRCTXR_DB_INV_FILE_PATH         ((INT32)  -33)		/**< Invalid file name. */
#define RRCTXR_DB_FILE_NOT_FOUND        ((INT32)  -34)		/**< File not found. */
#define RRCTXR_DB_FILE_READ_FAIL        ((INT32)  -35)		/**< File read fail. */
#define RRCTXR_DB_FILE_WRITE_FAIL       ((INT32)  -36)		/**< File write fail. */
#define RRCTXR_INV_DIM                  ((INT32)  -37)		/**< Invalid dimension. */
#define RRCTXR_INV_TABLE                ((INT32)  -38)		/**< Invalid table. */
#define RRCTXR_UNKNOWN                  ((INT32) -255)		/**< Unknown reason. */


/* The following are definitions of the rating types of the RRCTX engines. 
   They are used to identify the underlying rating protocol of the specific 
   RRCTX engine. */
/*------------------------------------------------------------------*/
/*! @enum		RRCTX_RATING_TYPE_T
 *	@brief		Rating type of the RRCTX engines
 
 *	@code
 *	typedef enum 
 *	{
 *		RRCTX_RATING_TYPE_UNKNOWN = 0,
 *		RRCTX_RATING_TYPE_VCHIP,
 *		RRCTX_RATING_TYPE_RRT,
 *		RRCTX_RATING_TYPE_DVB,
 *		RRCTX_RATIN_TYPE_TOTAL_NUMBER
 *	}RRCTX_RATING_TYPE_T; 
 *	@endcode
 
 *	@li@c	RRCTX_RATING_TYPE_UNKNOWN			- Unknown rating type
 *	@li@c	RRCTX_RATING_TYPE_VCHIP			- Rating type of VCHIP 
 *	@li@c	RRCTX_RATING_TYPE_RRT 				- Rating type of RRT 2.0
 *	@li@c	RRCTX_RATING_TYPE_DVB				- Rating type of DVB
 *	@li@c	RRCTX_RATIN_TYPE_TOTAL_NUMBER		- Total number of rating type
 */
/*------------------------------------------------------------------*/

typedef enum
{
    RRCTX_RATING_TYPE_UNKNOWN = 0,  /* unknown rating type. */
    RRCTX_RATING_TYPE_VCHIP,        /* for VCHIP engine */
    RRCTX_RATING_TYPE_RRT,          /* for RRT engine */
    RRCTX_RATING_TYPE_DVB,          /* for DVB engine */
    RRCTX_RATIN_TYPE_TOTAL_NUMBER   /* total number of rating types */
} RRCTX_RATING_TYPE_T;

/* Key types to get rating information */
/*------------------------------------------------------------------*/
/*! @enum		RRCTX_KEY_TYPE_T
 *	@brief		Key types to get rating information
 
 *	@code
 *	typedef enum 
 *	{
 *		RRCTX_KEY_TYPE_UNKNOWN = 0,
 *		RRCTX_KEY_TYPE_RATE_TEXT,
 *		RRCTX_KEY_TYPE_RATE_TYPE,
 *		RRCTX_KEY_TYPE_RATE_ENABLE,
 *		RRCTX_KEY_TYPE_RATE_EXCEED,
 *		RRCTX_KEY_TYPE_RATE_SETTING,
 *		RRCTX_KEY_TYPE_RATE_VALID,
 *		RRCTX_KEY_TYPE_ENG_SPECIFIC,
 *		RRCTX_KEY_TYPE_TOTAL_NUMBER
 *	}RRCTX_KEY_TYPE_T; 
 *	@endcode
 
 *	@li@c	RRCTX_KEY_TYPE_UNKNOWN			- Unknown key type
 *	@li@c	RRCTX_KEY_TYPE_RATE_TEXT			- Key type of rating text 
 *	@li@c	RRCTX_KEY_TYPE_RATE_TYPE 			- Key type of rating type 
 *	@li@c	RRCTX_KEY_TYPE_RATE_ENABLE			- Key type of rating is enable or not 
 *	@li@c	RRCTX_KEY_TYPE_RATE_EXCEED			- Key type of rating exceeded or not 
 *	@li@c	RRCTX_KEY_TYPE_RATE_SETTING		- Key type of rating setting 
 *	@li@c	RRCTX_KEY_TYPE_RATE_VALID 			- Key type of rating is valid or not 
 *	@li@c	RRCTX_KEY_TYPE_ENG_SPECIFIC		- Key type of specific information
 *	@li@c	RRCTX_RATIN_TYPE_TOTAL_NUMBER		- Total number of key types
 */
/*------------------------------------------------------------------*/

typedef enum
{
    RRCTX_KEY_TYPE_UNKNOWN = 0,
    RRCTX_KEY_TYPE_RATE_TEXT,           /* UTF-16 string , get         */
    RRCTX_KEY_TYPE_RATE_TYPE,           /* UINT8         , get         */
    RRCTX_KEY_TYPE_RATE_ENABLE,         /* BOOL          , set         */
    RRCTX_KEY_TYPE_RATE_EXCEED,         /* BOOL          , get         */
    RRCTX_KEY_TYPE_RATE_SETTING,        /* UINT64        , set, get    */
    RRCTX_KEY_TYPE_RATE_VALID,          /* BOOL          , get         */
    RRCTX_KEY_TYPE_ENG_SPECIFIC,        /* engine specific information */
    RRCTX_KEY_TYPE_TOTAL_NUMBER         /* total number of key types   */
} RRCTX_KEY_TYPE_T;

/*
    VCHIP 1.0
*/
#define RATING_SETI_TYPE_UNRATED            MAKE_BIT_MASK_16(0)    /**<  Rating types of VCHIP 1.0  	   */
#define RATING_SETI_TYPE_VCHIP_US           MAKE_BIT_MASK_16(1)    /**<  Rating types of VCHIP 1.0       */
#define RATING_SETI_TYPE_VCHIP_CA           MAKE_BIT_MASK_16(2)    /**<  Rating types of VCHIP 1.0       */
#define RATING_SETI_TYPE_VCHIP_CA_EX        MAKE_BIT_MASK_16(3)    /**<  Rating types of VCHIP 1.0       */
#define RATING_SETI_TYPE_DVB_AGE            MAKE_BIT_MASK_16(4)    /**<  Rating types of DVB       */
#define RATING_SETI_TYPE_DVB_COUNT          MAKE_BIT_MASK_16(5)    /**<  Rating types of DVB       */
#define RATING_SETI_TYPE_DVB_OPERATOR       MAKE_BIT_MASK_16(6)    /**<  Rating types of DVB       */
#define RATING_SETI_TYPE_DVB_SPECIFIC       MAKE_BIT_MASK_16(7)    /**<  Rating types of DVB       */

/*
	SBTVD
*/
#define RATING_VALUE_SBTVD_AGE       		0xF   /**<  Bit mask of SBTVD       */
#define RATING_VALUE_SBTVD_CONTENT       	0x4    /**<  Bit mask of SBTVD       */




/*------------------------------------------------------------------*/
/*! @enum		RRCTX_DVB_CSA_T
 *	@brief		Rating types of DVB
 
 *	@code
 *	typedef enum 
 *	{
 *		RRCTX_DVB_CSA_NONE = 0,
 *		RRCTX_DVB_CSA_UNIVERSAL,
 *		RRCTX_DVB_CSA_PARENTAL,
 *		RRCTX_DVB_CSA_12_AGE,
 *		RRCTX_DVB_CSA_16_AGE,
 *		RRCTX_DVB_CSA_18_AGE,
 *		RRCTX_DVB_CSA_LAST_VALID_ENTRY
 *	}RRCTX_DVB_CSA_T; 
 *	@endcode
 
 *	@li@c	RRCTX_DVB_CSA_NONE				- Unknown DVB rating type
 *	@li@c	RRCTX_DVB_CSA_UNIVERSAL			- Universal rating type
 *	@li@c	RRCTX_DVB_CSA_PARENTAL 			- Parental rating type
 *	@li@c	RRCTX_DVB_CSA_12_AGE				- 12 age rating type
 *	@li@c	RRCTX_DVB_CSA_16_AGE				- 16 age rating type
 *	@li@c	RRCTX_DVB_CSA_18_AGE				- 18 age rating type
 *	@li@c	RRCTX_DVB_CSA_LAST_VALID_ENTRY 	- Only for counting purpose
 */
/*------------------------------------------------------------------*/

typedef enum
{
    RRCTX_DVB_CSA_NONE = 0,
    RRCTX_DVB_CSA_UNIVERSAL,
    RRCTX_DVB_CSA_PARENTAL,
    RRCTX_DVB_CSA_12_AGE,
    RRCTX_DVB_CSA_16_AGE,
    RRCTX_DVB_CSA_18_AGE,

    RRCTX_DVB_CSA_LAST_VALID_ENTRY  /* only for counting purpose */
} RRCTX_DVB_CSA_T;

/*------------------------------------------------------------------*/
/*! @enum		RRCTX_DVB_DESCR_ESP_TYPE_T
 *	@brief		Rating types of DVB Spain
 
 *	@code
 *	typedef enum 
 *	{
 *		RRCTX_DVB_DESCR_ESP_NONE = 0,
 *		RRCTX_DVB_DESCR_ESP_SPECIAL_CHILD,
 *		RRCTX_DVB_DESCR_ESP_SPECIAL_ALL_AGE,
 *		RRCTX_DVB_DESCR_ESP_SPECIAL_X_RATING,
 *		RRCTX_DVB_DESCR_ESP_LAST_VALID_ENTRY
 *	}RRCTX_DVB_DESCR_ESP_TYPE_T; 
 *	@endcode
 
 *	@li@c	RRCTX_DVB_DESCR_ESP_NONE				- Unknown DVB rating type
 *	@li@c	RRCTX_DVB_DESCR_ESP_SPECIAL_CHILD		- Spain special child rating type
 *	@li@c	RRCTX_DVB_DESCR_ESP_SPECIAL_ALL_AGE 	- Spain special all age rating type
 *	@li@c	RRCTX_DVB_DESCR_ESP_SPECIAL_X_RATING 	- Spain special x rating type
 *	@li@c	RRCTX_DVB_DESCR_ESP_LAST_VALID_ENTRY 	- Only for counting purpose
 */
/*------------------------------------------------------------------*/

typedef enum
{
    RRCTX_DVB_DESCR_ESP_NONE = 0,
    RRCTX_DVB_DESCR_ESP_SPECIAL_CHILD,
    RRCTX_DVB_DESCR_ESP_SPECIAL_ALL_AGE,
    RRCTX_DVB_DESCR_ESP_SPECIAL_X_RATING,

    RRCTX_DVB_DESCR_ESP_LAST_VALID_ENTRY  /* only for counting purpose */
} RRCTX_DVB_DESCR_ESP_TYPE_T;

typedef enum
{
    RRCTX_DVB_DESCR_AUS_NONE = 0,
    RRCTX_DVB_DESCR_AUS_P,
    RRCTX_DVB_DESCR_AUS_C,
    RRCTX_DVB_DESCR_AUS_G,
    RRCTX_DVB_DESCR_AUS_PG,
    RRCTX_DVB_DESCR_AUS_M,
    RRCTX_DVB_DESCR_AUS_MA,
    RRCTX_DVB_DESCR_AUS_AV,
    RRCTX_DVB_DESCR_AUS_R,

    RRCTX_DVB_DESCR_AUS_LAST_VALID_ENTRY  /* only for counting purpose */
} RRCTX_DVB_DESCR_AUS_TYPE_T;

typedef enum
{
    RRCTX_DVB_DESCR_NZ_NONE = 0,
    RRCTX_DVB_DESCR_NZ_G,
    RRCTX_DVB_DESCR_NZ_PGR,
    RRCTX_DVB_DESCR_NZ_AO,
    
    RRCTX_DVB_DESCR_NZ_LAST_VALID_ENTRY  /* only for counting purpose */
} RRCTX_DVB_DESCR_NZ_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum		RRCTX_SBTVD_AGE_TYPE_T
 *	@brief		Rating types of SBTVD age rating
 
 *	@code
 *	typedef enum 
 *	{
 *		RRCTX_SBTVD_AGE_NONE = 0,
 *		RRCTX_SBTVD_AGE_10,
 *		RRCTX_SBTVD_AGE_12,
 *		RRCTX_SBTVD_AGE_14,
 *		RRCTX_SBTVD_AGE_16,
 *		RRCTX_SBTVD_AGE_18,
 *		RRCTX_SBTVD_AGE_LAST_VALID_ENTRY
 *	}RRCTX_SBTVD_AGE_TYPE_T; 
 *	@endcode
 
 */
/*------------------------------------------------------------------*/

typedef enum
{
	RRCTX_SBTVD_AGE_RESERVED = 0,
    RRCTX_SBTVD_AGE_L,
    RRCTX_SBTVD_AGE_10,
    RRCTX_SBTVD_AGE_12,
    RRCTX_SBTVD_AGE_14,
    RRCTX_SBTVD_AGE_16,
    RRCTX_SBTVD_AGE_18,    

    RRCTX_SBTVD_AGE_LAST_VALID_ENTRY  /* only for counting purpose */
} RRCTX_SBTVD_AGE_TYPE_T;

typedef enum
{
	RRCTX_SBTVD_CONT_RESERVED = 0,
	RRCTX_SBTVD_CONT_D,
    RRCTX_SBTVD_CONT_V,
    RRCTX_SBTVD_CONT_S,
    RRCTX_SBTVD_CONT_V_D,
    RRCTX_SBTVD_CONT_S_D,
    RRCTX_SBTVD_CONT_V_S,
    RRCTX_SBTVD_CONT_V_S_D,    

    RRCTX_SBTVD_CONT_LAST_VALID_ENTRY  /* only for counting purpose */
} RRCTX_SBTVD_CONT_TYPE_T;



/*------------------------------------------------------------------*/
/*! @struct	RRCTX_VCHIP_TYPE_T
 *  @brief		VCHIP 1.0 type structure
 
 *  @code
 *	typedef struct _RRCTX_VCHIP_TYPE_T
 *	{
 *		UINT8           ui1_type;
 *   		BOOL            b_unrated;
 *		BOOL            b_rrt;
 *		BOOL            b_us_region;
 *		BOOL            b_ca_region;
 *  		UINT8           ui1_tv;   
 *		UINT8           ui1_d;     
 *   		UINT8           ui1_l;     
 *   		UINT8           ui1_s;      
 *   		UINT8           ui1_v;      
 *   		UINT8           ui1_fv;     
 *   		UINT8           ui1_mpaa;   
 *   		UINT8           ui1_en;    
 *   		UINT8           ui1_fr;     
 *		ISO_3166_COUNT_T 	t_country;
 *   		RRCTX_DVB_CSA_T 	e_dvb_csa; 
 *		RRCTX_DVB_DESCR_ESP_TYPE_T  e_dvb_esp_descr;
 *		UINT8           ui1_dvb_age;
 *	} RRCTX_VCHIP_TYPE_T;  
 *  @endcode
 *
 *  @li@c	ui1_type			-Rating type of the VCHIP
 *  @li@c	b_unrated		-Current rating is unrated or not
 *  @li@c	b_rrt			-Current rating is RRT 2.0 or not
 *  @li@c	b_us_region		-Current region is USA or not
 *  @li@c	b_ca_region		-Current region is Canada or not
 *  @li@c	ui1_tv			-U.S. TV level
 *  @li@c	ui1_d			-Sexually Suggestive Dialog
 *  @li@c	ui1_l				-Adult Language
 *  @li@c	ui1_s			-Sexual Situations
 *  @li@c	ui1_v			-Violence
 *  @li@c	ui1_fv			-Fantasy Violence
 *  @li@c	ui1_mpaa		-MPAA
 *  @li@c	ui1_en			-Canada english rating
 *  @li@c	ui1_fr			-Canada france rating
 *  @li@c	t_country			-DVB country code
 *  @li@c	e_dvb_esp_descr	-DVB Spain special rating
 *  @li@c	e_dvb_csa		-DVB age
 *  @li@c	ui1_dvb_age		-DVB age
 */
/*------------------------------------------------------------------*/

typedef struct _RRCTX_VCHIP_TYPE_T
{
    UINT8           ui1_type;
    BOOL            b_unrated;
    /* Rating */
    BOOL            b_rrt;
    BOOL            b_us_region;
    BOOL            b_ca_region;
    /* US */
    UINT8           ui1_tv;     /* TV */
    UINT8           ui1_d;      /* Sexually Suggestive Dialog */
    UINT8           ui1_l;      /* Adult Language */
    UINT8           ui1_s;      /* Sexual Situations */
    UINT8           ui1_v;      /* Violence */
    UINT8           ui1_fv;     /* Fantasy Violence */
    UINT8           ui1_mpaa;   /* MPAA */
    /* CA */
    UINT8           ui1_en;     /* English */
    UINT8           ui1_fr;     /* French */

    ISO_3166_COUNT_T t_country;
    union
    {
	    /* French */
	    RRCTX_DVB_CSA_T e_dvb_csa;  /* For TNT CSA (French DVB) */
        /* Spanish */
        RRCTX_DVB_DESCR_ESP_TYPE_T  e_dvb_esp_descr;  /* For Spanish DVB */
		/* Australia*/
		RRCTX_DVB_DESCR_AUS_TYPE_T  e_dvb_aus_descr;  /* For Australia DVB */
		/* New Zealand*/
		RRCTX_DVB_DESCR_NZ_TYPE_T  e_dvb_nz_descr;  /* For New Zealand DVB */
    } u_dvb_type;

	/* SBTVD */
	RRCTX_SBTVD_AGE_TYPE_T e_sbtvd_age;
	RRCTX_SBTVD_CONT_TYPE_T e_sbtvd_cont;

    UINT8           ui1_dvb_age;
} RRCTX_VCHIP_TYPE_T;

#define RRCTX_DVB_COUNTRY_SPEC_RATING_BASE              (0x10)  /* from 0x10 ~ 0x12 */    /**<    DVB country code mask base    */
#define RRCTX_DVB_COUNTRY_SPEC_MASK(_rating)            MAKE_BIT_MASK_16((_rating) - RRCTX_DVB_COUNTRY_SPEC_RATING_BASE)    /**<   DVB country code mask    */

#define RRCTX_DVB_COUNTRY_SPEC_MASK_FRA_U               RRCTX_DVB_COUNTRY_SPEC_MASK(0x10)    /**<   DVB France_U mask     */
#define RRCTX_DVB_COUNTRY_SPEC_MASK_FRA_PA              RRCTX_DVB_COUNTRY_SPEC_MASK(0x11)    /**<   DVB France_PA mask     */
#define RRCTX_DVB_COUNTRY_SPEC_MASK_ESP_X               RRCTX_DVB_COUNTRY_SPEC_MASK(0x12)    /**<   DVB Spain_X mask    */

typedef UINT32 RRCTX_OPERATOR_NAME_T;    /**<  operator name  */

#define RRCTX_OPERATOR_NAME_OTHERS          ((RRCTX_OPERATOR_NAME_T) 0)
#define RRCTX_OPERATOR_NAME_UPC             ((RRCTX_OPERATOR_NAME_T) 1)
#define RRCTX_OPERATOR_NAME_COMHEM          ((RRCTX_OPERATOR_NAME_T) 2)
#define RRCTX_OPERATOR_NAME_CANAL_DIGITAL   ((RRCTX_OPERATOR_NAME_T) 3)
#define RRCTX_OPERATOR_NAME_TELE2           ((RRCTX_OPERATOR_NAME_T) 4)
#define RRCTX_OPERATOR_NAME_STOFA           ((RRCTX_OPERATOR_NAME_T) 5)
#define RRCTX_OPERATOR_NAME_YOUSEE          ((RRCTX_OPERATOR_NAME_T) 6)
#define RRCTX_OPERATOR_NAME_ZIGGO           ((RRCTX_OPERATOR_NAME_T) 7)

/*------------------------------------------------------------------*/
/*! @struct	RRCTX_VCHIP_RATE_SETTING_T
 *  @brief		VCHIP base setting structure
 
 *  @code
 *	typedef struct _RRCTX_VCHIP_RATE_SETTING_T
 *	{
 *		UINT16                 			ui2_type_flag;
 *   		BOOL                    		b_unrated;
 *    		UINT64                 			ui8_vchip_us;
 *   		UINT8                   			ui1_vchip_ca;
 *   		UINT32                 			ui4_vchip_ca_ex;
 *  		UINT8                   			ui1_dvb_age;
 *   		ISO_3166_COUNT_T        	t_count_code;
 *   		UINT16                  		ui2_dvb_spec_mask;
 *	} RRCTX_VCHIP_RATE_SETTING_T;  
 *  @endcode
 *
 *  @li@c	ui2_type_flag			-Flag of rating type
 *  @li@c	b_unrated			-Current rating is unrated or not
 *  @li@c	ui8_vchip_us			-U.S. rating type
 *  @li@c	ui1_vchip_ca			-Canada raing type
 *  @li@c	ui4_vchip_ca_ex		-Exempt Canada raing type
 *  @li@c	ui1_dvb_age			-DVB age rating type
 *  @li@c	t_count_code			-Country code
 *  @li@c	ui2_dvb_spec_mask	-DVB check mask
 */
/*------------------------------------------------------------------*/

typedef struct _RRCTX_VCHIP_RATE_SETTING_T
{
    UINT16                  ui2_type_flag;
    BOOL                    b_unrated;
    UINT64                  ui8_vchip_us;
    UINT8                   ui1_vchip_ca;
    UINT32                  ui4_vchip_ca_ex;
    UINT8                   ui1_dvb_age;
    ISO_3166_COUNT_T        t_count_code;
	RRCTX_OPERATOR_NAME_T   t_operator_name;
    UINT16                  ui2_dvb_spec_mask;
} RRCTX_VCHIP_RATE_SETTING_T;


/*------------------------------------------------------------------*/
/*! @struct	RRCTX_RRT_RATE_SETTING_T
 *	@brief	RRT 2.0 base setting structure
 
 *	@code
 *	typedef struct _RRCTX_VCHIP_RATE_SETTING_T
 *	{
 *		BOOL    	b_set_get;              
 *   		UINT8   	ui1_rgn_idx;
 *   		UINT8   	ui1_dim_idx;
 *  		UINT8*  	pui1_bu;                
 *   		BOOL*   	pb_lvl_block;
 *	} RRCTX_RRT_RATE_SETTING_T;  
 *	@endcode
 *
 *	@li@c	b_set_get			-Rating infomation is setting or getting
 *	@li@c	ui1_rgn_idx			-Region index
 *	@li@c	ui1_dim_idx			-Dimension index
 *	@li@c	pui1_bu				-Pointer to variable to hold value of the block unrate
 *	@li@c	pb_lvl_block 			-Block flags of the rating levels 
 */
/*------------------------------------------------------------------*/

typedef struct _RRCTX_RRT_RATE_SETTING_T
{
    BOOL    b_set_get;              /* TRUE: Set , FALSE: Get */
    UINT8   ui1_rgn_idx;
    UINT8   ui1_dim_idx;
    UINT8*  pui1_bu;                /* Block Unrate */
    BOOL*   pb_lvl_block;
} RRCTX_RRT_RATE_SETTING_T;

/*------------------------------------------------------------------*/
/*! @fn			VOID (*x_rrctx_nfy_fct) (
					HANDLE_T          h_rating, 
					VOID*             pv_nfy_tag, 
					UINT32            ui4_reason)

 *  @brief		 	Callback function type, used to notify middleware components and applications 
   			 	of the condition changes 
   			
 *  @param [in]	h_rating			-Handle of RRCTX
 				pv_nfy_tag		-Tag for the notify function
 				ui4_reason		-Reason to notification
 				
 *  @param [out]	VOID
 
 *  @note			Callback function type, used to notify middleware components and applications 
   				of the condition changes 
 *  @see
 *  @return		VOID
 *  @retval		VOID
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_rrctx_nfy_fct) (
    HANDLE_T          h_rating,
    VOID*             pv_nfy_tag,
    UINT32            ui4_reason);

#endif /* _U_RRCTX_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_RRCTX                            */
/*----------------------------------------------------------------------------*/

