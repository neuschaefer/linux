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
/*! @file u_sb_dvbc_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         SVL Builder DVB-C engine.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVL_BLDR_SB_DVBC_ENG SVL Builder DVB-C Engine
 *
 *  @ingroup groupMW_SVL_BLDR
 *  @brief The DVB-C engine module is used to scan all digital channel in DVB-C spec.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could create scan engine
 *   or start scan from the exported API of DVB-C svl builder.
 *
 *  @see groupMW_SVL_BLDR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

 
#ifndef _U_SB_DVBC_ENG_H_
#define _U_SB_DVBC_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_tuner.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MULTILINGUAL_SERVICE_NAME_SUPPORT
#define MULTIPLE_LCN_DESC_SUPPORT

#define DVBC_ENG_NAME                "sb_dvbc"    /**< dvb-c builder's name*/

#define TSL_REC_ID_FREQ_STEP        2000000/*6000000*/ /**< 6MHz */


typedef UINT32 OPERATOR_NAME_T;    /**<  operator name  */

#define DVBC_OPERATOR_NAME_OTHERS          ((OPERATOR_NAME_T) 0)
#define DVBC_OPERATOR_NAME_UPC             ((OPERATOR_NAME_T) 1)
#define DVBC_OPERATOR_NAME_COMHEM          ((OPERATOR_NAME_T) 2)
#define DVBC_OPERATOR_NAME_CANAL_DIGITAL   ((OPERATOR_NAME_T) 3)
#define DVBC_OPERATOR_NAME_TELE2           ((OPERATOR_NAME_T) 4)
#define DVBC_OPERATOR_NAME_STOFA           ((OPERATOR_NAME_T) 5)
#define DVBC_OPERATOR_NAME_YOUSEE          ((OPERATOR_NAME_T) 6)
#define DVBC_OPERATOR_NAME_ZIGGO           ((OPERATOR_NAME_T) 7)
#define DVBC_OPERATOR_NAME_UNITYMEDIA      ((OPERATOR_NAME_T) 8)
#define DVBC_OPERATOR_NAME_NUMERICABLE     ((OPERATOR_NAME_T) 9)
#define DVBC_OPERATOR_NAME_VOLIA     ((OPERATOR_NAME_T) 10) /* Ukraine */
#define DVBC_OPERATOR_NAME_TELEMACH     ((OPERATOR_NAME_T) 11) /* Slovenia */
#define DVBC_OPERATOR_NAME_ONLIME     ((OPERATOR_NAME_T) 12) /* Russia */




/* Command to get the scan map and info */
/*------------------------------------------------------------------*/
/*! @enum DVBC_SCAN_MAPS_CMD_T
 *  @brief commend list of scan map
 *  @code
 *  typedef enum {
 *      DVBC_SCAN_MAPS_CMD_SET_FINE_TUNE_RANGE,
 *      DVBC_SCAN_MAPS_CMD_GET_FIRST_FREQ,
 *      DVBC_SCAN_MAPS_CMD_GET_LAST_FREQ,
 *      DVBC_SCAN_MAPS_CMD_GET_NEXT_FREQ,
 *      DVBC_SCAN_MAPS_CMD_GET_PREV_FREQ,
 *      DVBC_SCAN_MAPS_CMD_GET_FIRST_PRIOR_FREQ,
 *      DVBC_SCAN_MAPS_CMD_GET_LAST_PRIOR_FREQ,
 *      DVBC_SCAN_MAPS_CMD_GET_NEXT_PRIOR_FREQ,
 *      DVBC_SCAN_MAPS_CMD_GET_NUMBER_PRIOR_FREQ,
 *      DVBC_SCAN_MAPS_CMD_GET_TUNER_PARA,
 *      DVBC_SCAN_MAPS_CMD_GET_TSL_ID_BY_FREQ,
 *      DVBC_SCAN_MAPS_CMD_GET_DEFAULT_FREQ,
 *      DVBC_SCAN_MAPS_CMD_GET_DEFAULT_NETWORK_ID,
 *      DVBC_SCAN_MAPS_CMD_GET_DEFAULT_SYMBOL_RATE,
 *      DVBC_SCAN_MAPS_CMD_GET_DEFAULT_QAM,
 *      DVBC_SCAN_MAPS_CMD_GET_DEFAULT_TUNER_PARA,
 *      DVBC_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING,
 *      DVBC_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING,
 *      DVBC_SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY,
 *      DVBC_SCAN_MAPS_CMD_DETERMINE_PRIVATE_DATA_SPECIFIER,
 *      DVBC_SCAN_MAPS_CMD_IS_SERVICE_REMOVAL_MUST_BE_CONFIRMED_COUNTRY,
 *      DVBC_SCAN_MAPS_CMD_IS_CUSTOMIZED_SCAN_MAP,
 *      DVBC_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME,
 *      DVBC_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME,
 *  }   DVBC_SCAN_MAPS_CMD_T;
 *  @endcode
 *  @li@c  DVBC_SCAN_MAPS_CMD_SET_FINE_TUNE_RANGE            - it provides eng to set the range of finetune. UINT32. Hz
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_FIRST_FREQ                 - to get the first frequency by country and operator. UINT32. Hz
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_LAST_FREQ                  - to get the last frequency by country and operator. UINT32. Hz
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_NEXT_FREQ                  - to get next frequency by country and operator. UINT32. Hz
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_PREV_FREQ                  - to get previous frequency by country and operator. UINT32. Hz
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_FIRST_PRIOR_FREQ           - to get the first "prior" frequency by country and operator, user can define some prior freq points to speed up the scan procedure. UINT32. Hz
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_LAST_PRIOR_FREQ            - to get the last "prior" frequency by country and operator. UINT32. Hz
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_NEXT_PRIOR_FREQ            - to get next "prior" frequency by country and operator.
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_NUMBER_PRIOR_FREQ          - to get the total number of prior frequency points.
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_TUNER_PARA                 - to get unknown tuner parameters by country, operator and frequency.  SB_DVBC_DLVR_INFO_T
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_TSL_ID_BY_FREQ             - to get tsl_rec_id by frequency.
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_DEFAULT_FREQ               - to get default home frequency for every country and operator
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_DEFAULT_NETWORK_ID         - to get default home network id for every country and operator
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_DEFAULT_SYMBOL_RATE        - to get default home symbol rate for every country and operator
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_DEFAULT_QAM                - to get default QAM for every country and operator
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_DEFAULT_TUNER_PARA         - to get unknown tuner parameters by default
 *  @li@c  DVBC_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING          - to check if it needs to apply customized sorting for this country
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING         - to get the ranking with svc_id
 *  @li@c  DVBC_SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY             - to check if the country is a non-lcn country
 *  @li@c  DVBC_SCAN_MAPS_CMD_DETERMINE_PRIVATE_DATA_SPECIFIER    - n.a.(to be deleted)
 *  @li@c  DVBC_SCAN_MAPS_CMD_IS_SERVICE_REMOVAL_MUST_BE_CONFIRMED_COUNTRY    - to check if the service removal has to be confirmed by user
 *  @li@c  DVBC_SCAN_MAPS_CMD_IS_CUSTOMIZED_SCAN_MAP         - to check if there is any customized scan map for the country. 
 *  @li@c  DVBC_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME  - to check if it needs to apply customized sorying by name for this country 
 *  @li@c  DVBC_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME - to get the ranking with service name.
 */
/*------------------------------------------------------------------*/
typedef enum {
    DVBC_SCAN_MAPS_CMD_SET_FINE_TUNE_RANGE,
    DVBC_SCAN_MAPS_CMD_GET_FIRST_FREQ,
    DVBC_SCAN_MAPS_CMD_GET_LAST_FREQ,
    DVBC_SCAN_MAPS_CMD_GET_NEXT_FREQ,
    DVBC_SCAN_MAPS_CMD_GET_PREV_FREQ,
    DVBC_SCAN_MAPS_CMD_GET_FIRST_PRIOR_FREQ,
    DVBC_SCAN_MAPS_CMD_GET_LAST_PRIOR_FREQ,
    DVBC_SCAN_MAPS_CMD_GET_NEXT_PRIOR_FREQ,
    DVBC_SCAN_MAPS_CMD_GET_NUMBER_PRIOR_FREQ,
    DVBC_SCAN_MAPS_CMD_GET_TUNER_PARA,
    DVBC_SCAN_MAPS_CMD_GET_TSL_ID_BY_FREQ,
    DVBC_SCAN_MAPS_CMD_GET_DEFAULT_FREQ,
    DVBC_SCAN_MAPS_CMD_GET_DEFAULT_NETWORK_ID,
    DVBC_SCAN_MAPS_CMD_GET_DEFAULT_SYMBOL_RATE,
    DVBC_SCAN_MAPS_CMD_GET_DEFAULT_QAM,
    DVBC_SCAN_MAPS_CMD_GET_DEFAULT_TUNER_PARA,
    DVBC_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING,
    DVBC_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING,
    DVBC_SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY,
    DVBC_SCAN_MAPS_CMD_DETERMINE_PRIVATE_DATA_SPECIFIER,
    DVBC_SCAN_MAPS_CMD_IS_SERVICE_REMOVAL_MUST_BE_CONFIRMED_COUNTRY,
    DVBC_SCAN_MAPS_CMD_IS_CUSTOMIZED_SCAN_MAP,
    DVBC_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME,
    DVBC_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME,
    DVBC_SCAN_MAPS_CMD_IS_MULTIPLE_BANK_COUNTRY,
    DVBC_SCAN_MAPS_CMD_GET_RADIO_CH_NUM_OFFSET,
    DVBC_SCAN_MAPS_CMD_CHECK_SVC_TYPE
}   DVBC_SCAN_MAPS_CMD_T;

/*------------------------------------------------------------------*/
/*! @brief to acquire cusomized scan parameters
 *  @param [in] t_cmd                   - the acquire cmd type in DVBC_SCAN_MAPS_CMD_T
 *  @param [in] t_country_code      - the selected country
 *  @param [in] t_operator_name     - the selected operator 
 *  @param [in] pv_input                - the main input parameter 
 *  @param [in] pv_input2                - the additional input parameter (e.g. DVBC_SCAN_MAPS_CMD_GET_TUNER_PARA)
 *  @param [out] pv_output              - the desired result for different acquire cmd
 *  @param [out] pz_output_len          - the size of output object
 *  @note
 *  @see DVBC_SCAN_MAPS_CMD_T
 *  @return Return the error code
 *  @retval SBR_OK                                  - the routine was successful 
 *  @retval SBR_NOT_ENOUGH_SPACE        - the size of output object is not big enough
 *  @retval SBR_OUT_OF_RANGE     - the input parameter is out of range
 *  @retval SBR_NOT_FOUND     - the input country cannot be found
 *  @retval SBR_INV_ARG     - the commend is not supported
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_dvbc_scan_maps_fct) (DVBC_SCAN_MAPS_CMD_T     t_cmd,
                                         ISO_3166_COUNT_T    t_country_code,
                                         OPERATOR_NAME_T    t_operator_name,
                                         VOID*               pv_input,
                                         VOID*               pv_input2,
                                         VOID*               pv_output,
                                         SIZE_T*             pz_output_len);

typedef INT32 (*x_sb_dvbc_scan_maps_fct_prv) (DVBC_SCAN_MAPS_CMD_T     t_cmd,
                                         ISO_3166_COUNT_T    t_country_code,
                                         OPERATOR_NAME_T    t_operator_name,
                                         VOID*               pv_input,
                                         VOID*               pv_input2,
                                         VOID*               pv_output,
                                         SIZE_T*             pz_output_len,
                                         VOID*               pv_nfy_tag);


typedef UINT32      SB_DVBC_CONFIG_T;    /**< This data type contains different configuration flags for the scan process       */

#define SB_DVBC_CONFIG_IGNORE_ANALOG_CH_ON_SORTING       (MAKE_BIT_MASK_32(0))    /**< To ignore the analog channels or not while sorting the DVB channels.
   Note that to set this flag might cause the channel number collision between
   the digital and analog channels       */
#define SB_DVBC_CONFIG_SUPPORT_MHEG5_SERVICES            (MAKE_BIT_MASK_32(1))    /**< Install the MHEG-5 services as visible ones       */

#define SB_DVBC_CONFIG_START_CH_NUM_FOR_NON_LCN_CH       (MAKE_BIT_MASK_32(2))    /**< To customize the the start of the channel numbers to be assigned to the non-LCN 
   channels while LCN need be applied.       */
#define SB_DVBC_CONFIG_ALWAYS_APPLY_LCN                  (MAKE_BIT_MASK_32(3))    /**<  To always apply the LCN. By default, the engine would allocate the services to 
   channel numbers starting from 1 if all of the services found at this time of scan
   are not attached to a LCN; and henceforth, all the LCN found is ignored. With 
   this flag set, the engine shall always allocate the non-LCN services to the 
   given (or default) start channel numbers for non-LCN services. For example, at
   a country where there is no LCN broadcasted, all the digital services are allocated
   to 800, 801, 802 ...; there will be no services at 1, 2, 3 ...       */
#define SB_DVBC_CONFIG_UPDATE_TO_TEMP_SVL                (MAKE_BIT_MASK_32(4))    /**< To store the service records to the given temporary SVL       */
#define SB_DVBC_CONFIG_KEEP_DUPLICATE_CHANNELS           (MAKE_BIT_MASK_32(5))    /**< Do not remove duplicate channels       */
#define SB_DVBC_CONFIG_SDT_NIT_TIMEOUT                   (MAKE_BIT_MASK_32(6))    /**< To customize the timeout value of loading SDT and NIT       */
#define SB_DVBC_CONFIG_SUPPORT_MHP_SERVICES              (MAKE_BIT_MASK_32(7))    /**< Install the MHP services as visible ones       */
#define SB_DVBC_CONFIG_RESERVE_CH_NUM_BEFORE_NON_LCN_CH  (MAKE_BIT_MASK_32(8))    /**< To reserve customizied number of channel numbers before allocating for the 
   non-LCN channels       */
#define SB_DVBC_CONFIG_NOT_SUPPORT_HDTV                  (MAKE_BIT_MASK_32(9))    /**< Do not support HDTV (H.264 & MPEG-2)       */
#define SB_DVBC_CONFIG_SIMPLE_SORT_FOR_NON_LCN_CH        (MAKE_BIT_MASK_32(10))    /**< Sort the non-LCN channels by the simple rule -service type;
   Otherwise, sort them by (1) service type, (2) LCN, (3) service name, (4) service id       */
#define SB_DVBC_CONFIG_EX_QUICK_BUILD_SVL_BY_SDT        (MAKE_BIT_MASK_32(11))    /**< when ex-quick mode, build service list by SDT not NIT       */
#define SB_DVBC_CONFIG_PRIOR_RF_SCAN_ENABLE             (MAKE_BIT_MASK_32(12))    /**< when scan with scan map, app can turn on this flag to scan the prior RF which 
   may speed up scan performance.*/
#define SB_DVBC_CONFIG_SCAN_WITHOUT_SCAN_MAP             (MAKE_BIT_MASK_32(13))    /**< when full scan, app can forcely give up scan map and scan with tuner step linearly*/

#define SB_DVBC_CONFIG_TV_RADIO_SEPARATE                 (MAKE_BIT_MASK_32(14))    /**< To support TV type and Raido type service in the its channel list in Nordig country,  
  and the SB_DVBC_CONFIG_IGNORE_ANALOG_CH_ON_SORTING must be defined before used this config*/ 

#define SB_DVBC_CONFIG_CUST_1                            (MAKE_BIT_MASK_32(15))    /**<*/ 

#define SB_DVBC_CONFIG_QAM_SR_AUTO_DETECT                 (MAKE_BIT_MASK_32(16))    /**<*/ 

#define SB_DVBC_CONFIG_INSTALL_FREE_SERVICES_ONLY         (MAKE_BIT_MASK_32(17))   /**<*/ 

#define SB_DVBC_CONFIG_TRUST_NIT_IN_EX_QUICK_SCAN         (MAKE_BIT_MASK_32(18))   /**<*/ 

#define SB_DVBC_CONFIG_QUICK_SCAN_IGNORE_SVC_OUT_OF_NETWORK    (MAKE_BIT_MASK_32(19))


/*
 *  The structure SB_DVBC_INIT_DATA_T is used as the argument of the API x_sb_dvb_eng_init()
 *  when initializing an SVL Builder with DVB engine.
 *
 *  t_eng_cfg_flag                  Contains the engine configuration flag. The options allowed
 *                                  are SB_DVBC_CONFIG_IGNORE_ANALOG_CH_ON_SORTING,
 *                                  SB_DVBC_CONFIG_SUPPORT_MHEG5_SERVICES,
 *                                  SB_DVBC_CONFIG_SUPPORT_MHP_SERVICES,
 *                                  SB_DVBC_CONFIG_START_CH_NUM_FOR_NON_LCN_CH,
 *                                  SB_DVBC_CONFIG_ALWAYS_APPLY_LCN,
 *                                  SB_DVBC_CONFIG_SDT_NIT_TIMEOUT,
 *                                  SB_DVBC_CONFIG_RESERVE_CH_NUM_BEFORE_NON_LCN_CH,
 *                                  SB_DVBC_CONFIG_NOT_SUPPORT_HDTV,
 *                                  SB_DVBC_CONFIG_SIMPLE_SORT_FOR_NON_LCN_CH.
 *
 *  ui2_start_ch_num_for_non_lcn    Contains the start channel number to be assigned to non-LCN
 *                                  channels when the SB_DVBC_CONFIG_START_CH_NUM_FOR_NON_LCN_CH 
 *                                  is set. For SB_DVBC_CONFIG_RESERVE_CH_NUM_BEFORE_NON_LCN_CH,
 *                                  it is the number of reserved channel numbers.
 *
 *  ui4_sdt_timeout_mseconds        Contains the timeout value for loading the SDT. A value less
 *                                  than 2 seconds is not allowed. This argument is used only 
 *                                  when the SB_DVBC_CONFIG_SDT_NIT_TIMEOUT is set.
 *
 *  ui4_nit_timeout_mseconds        Contains the timeout value for loading the NIT. A value less
 *                                  than 10 seconds is not allowed. This argument is used only 
 *                                  when the SB_DVBC_CONFIG_SDT_NIT_TIMEOUT is set.
 *
 *  pf_scan_map                     Contains the scan map callback function.
 */
/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_INIT_DATA_T
 *  @brief The structure SB_DVBC_INIT_DATA_T is used as the argument of the API x_sb_dvb_eng_init()
 *  when initializing an SVL Builder with DVB engine.
 *  @code
 *  typedef struct _SB_DVBC_INIT_DATA_T
 *  {
 *      SB_DVBC_CONFIG_T         t_eng_cfg_flag;
 *      UINT16                  ui2_start_ch_num_for_non_lcn;
 *      UINT32                  ui4_sdt_timeout_mseconds;
 *      UINT32                  ui4_nit_timeout_mseconds;
 *      x_sb_dvbc_scan_maps_fct  pf_scan_maps;
 *  } SB_DVBC_INIT_DATA_T;
 *  @endcode
 *  @li@c  t_eng_cfg_flag                                    - Contains the engine configuration flag. The options allowed
 *                                  are the types in SB_DVBC_CONFIG_T
 *  @li@c  ui2_start_ch_num_for_non_lcn                      - Contains the start channel number to be assigned to non-LCN
 *                                  channels when the SB_DVBC_CONFIG_START_CH_NUM_FOR_NON_LCN_CH 
 *                                  is set. For SB_DVBC_CONFIG_RESERVE_CH_NUM_BEFORE_NON_LCN_CH,
 *                                  it is the number of reserved channel numbers.
 *  @li@c  ui4_sdt_timeout_mseconds                          - Contains the timeout value for loading the SDT. A value less
 *                                  than 2 seconds is not allowed. This argument is used only 
 *                                  when the SB_DVBC_CONFIG_SDT_NIT_TIMEOUT is set.
 *  @li@c  ui4_nit_timeout_mseconds                          - Contains the timeout value for loading the NIT. A value less
 *                                  than 10 seconds is not allowed. This argument is used only 
 *                                  when the SB_DVBC_CONFIG_SDT_NIT_TIMEOUT is set.
 *  @li@c  pf_scan_maps                                      - Contains the scan map callback function.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_INIT_DATA_T
{
    SB_DVBC_CONFIG_T         t_eng_cfg_flag;
    UINT16                  ui2_start_ch_num_for_non_lcn;
    UINT32                  ui4_sdt_timeout_mseconds;
    UINT32                  ui4_nit_timeout_mseconds;
    #ifndef LINUX_TURNKEY_SOLUTION_PHASE2
    x_sb_dvbc_scan_maps_fct  pf_scan_maps;
    #endif
} SB_DVBC_INIT_DATA_T;


/*
 *  The structure SB_DVBC_OPEN_DATA_T is used as the argument
 *  pv_open_data of the API c_sb_open_builder()/x_sb_open_builder()
 *  when opening an SVL Builder with DVB-C engine.
 *
 *  ui4_svl_id                      The id of SVL to be loaded.
 *  ps_svl_name                     The name of SVL to be loaded.
 *  ps_file_name                    The file name from which the SVL/TSL
 *                                  will be loaded.
 *  b_use_orig                      A flag. When set to true, use the existed SVL,
 *                                  instead of reloading it.  When set to false,
 *                                  overwrite the SVL (if exists) by reloading.
 */
/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_OPEN_DATA_T
 *  @brief The structure SB_DVBC_OPEN_DATA_T is used as the argument
 *  pv_open_data of the API c_sb_open_builder()/x_sb_open_builder()
 *  when opening an SVL Builder with DVB-C engine.
 *  @code
 *  typedef struct _SB_DVBC_OPEN_DATA_T
 *  {
 *      UINT16                  ui2_svl_id;
 *      CHAR*                   ps_svl_name;
 *      CHAR*                   ps_file_name;
 *      CHAR*                   ps_tuner_name;    
 *      BOOL                    b_use_orig;
 *  } SB_DVBC_OPEN_DATA_T;
 *  @endcode
 *  @li@c  ui2_svl_id                                        - The id of SVL to be loaded.
 *  @li@c  ps_svl_name                                       - The name of SVL to be loaded.
 *  @li@c  ps_file_name                                      - The file name from which the SVL/TSL
 *                                  will be loaded.
 *  @li@c  ps_tuner_name                                     - the name of tuner to be used.
 *  @li@c  b_use_orig                                        - A flag. When set to true, use the existed SVL,
 *                                  instead of reloading it.  When set to false,
 *                                  overwrite the SVL (if exists) by reloading.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_OPEN_DATA_T
{
    UINT16                  ui2_svl_id;
    CHAR*                   ps_svl_name;
    CHAR*                   ps_file_name;
    CHAR*                   ps_tuner_name;    
    BOOL                    b_use_orig;
} SB_DVBC_OPEN_DATA_T;


/* DVB-C scan type */
/*------------------------------------------------------------------*/
/*! @enum SB_DVBC_SCAN_TYPE_T
 *  @brief DVB-C scan type
 *  @code
 *  typedef enum
 *  {
 *      SB_DVBC_SCAN_TYPE_UNKNOWN = 0,
 *      SB_DVBC_SCAN_TYPE_FULL_MODE,
 *      SB_DVBC_SCAN_TYPE_MANUAL_FREQ,  
 *      SB_DVBC_SCAN_TYPE_UPDATE,
 *      SB_DVBC_SCAN_TYPE_NUM 
 *  } SB_DVBC_SCAN_TYPE_T;
 *  @endcode
 *  @li@c  SB_DVBC_SCAN_TYPE_UNKNOWN                         - unknown scan type. should not assign with this value.
 *  @li@c  SB_DVBC_SCAN_TYPE_FULL_MODE                       - full scan. user can setup the scan range.
 *  @li@c  SB_DVBC_SCAN_TYPE_MANUAL_FREQ                     - manually scan with a signal frequency. manual scan doesn't support nit search
 *  @li@c  SB_DVBC_SCAN_TYPE_UPDATE                          - update scan with previously successful scan parameters.
 *  @li@c  SB_DVBC_SCAN_TYPE_NUM                             - total number of scan types
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_DVBC_SCAN_TYPE_UNKNOWN = 0,
    SB_DVBC_SCAN_TYPE_FULL_MODE,
    SB_DVBC_SCAN_TYPE_MANUAL_FREQ,  /*manual scan doesn't support nit search*/
    SB_DVBC_SCAN_TYPE_UPDATE,
    SB_DVBC_SCAN_TYPE_NUM /* number of scan types */
} SB_DVBC_SCAN_TYPE_T;


/* DVB-C NIT search mode */
/*------------------------------------------------------------------*/
/*! @enum SB_DVBC_NIT_SEARCH_MODE_T
 *  @brief DVB-C NIT search mode
 *  @code
 *  typedef enum
 *  {
 *      SB_DVBC_NIT_SEARCH_MODE_OFF = 0,
 *      SB_DVBC_NIT_SEARCH_MODE_QUICK,
 *      SB_DVBC_NIT_SEARCH_MODE_EX_QUICK,
 *      SB_DVBC_NIT_SEARCH_MODE_NUM 
 *  } SB_DVBC_NIT_SEARCH_MODE_T;
 *  @endcode
 *  @li@c  SB_DVBC_NIT_SEARCH_MODE_OFF                       - NIT search off
 *  @li@c  SB_DVBC_NIT_SEARCH_MODE_QUICK                     - Quick scan
 *  @li@c  SB_DVBC_NIT_SEARCH_MODE_EX_QUICK                  - Extra quick scan
 *  @li@c  SB_DVBC_NIT_SEARCH_MODE_NUM                       - number of scan types
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_DVBC_NIT_SEARCH_MODE_OFF = 0,
    SB_DVBC_NIT_SEARCH_MODE_QUICK,
    SB_DVBC_NIT_SEARCH_MODE_EX_QUICK,
    SB_DVBC_NIT_SEARCH_MODE_NUM /* number of nit search types */
} SB_DVBC_NIT_SEARCH_MODE_T;

#define SB_DVBC_SCAN_INFO_NW_ID_VALID                         (UINT16)    0x1    /**< bit mask to indicate nw_id valid        */
#define SB_DVBC_SCAN_INFO_BW_VALID                            (UINT16)    0x2    /**< bit mask to indicate bandwidth valid       */
#define SB_DVBC_SCAN_INFO_MOD_VALID                           (UINT16)     0x4    /**< bit mask to indicate modulation valid       */
#define SB_DVBC_SCAN_INFO_SYM_VALID                           (UINT16)    0x8    /**< bit mask to indicate symbol rate valid        */
#define SB_DVBC_SCAN_INFO_START_FREQ_VALID                    (UINT16)    0x10    /**< bit mask to indicate start frequency valid       */
#define SB_DVBC_SCAN_INFO_END_FREQ_VALID                      (UINT16)    0x20    /**< bit mask to inidicate end frequency valid       */


#define NW_ID_VALID(mask)   \
    (mask&SB_DVBC_SCAN_INFO_NW_ID_VALID)    /**< marco for checking nw_id valid       */
#define BW_VALID(mask)  \
    ((mask&SB_DVBC_SCAN_INFO_BW_VALID)>>1)    /**< marco for checking BW valid       */
#define MOD_VALID(mask) \
    ((mask&SB_DVBC_SCAN_INFO_MOD_VALID)>>2)    /**< marco for checking modulation valid       */
#define SYM_VALID(mask) \
    ((mask&SB_DVBC_SCAN_INFO_SYM_VALID)>>3)    /**< marco for checking symbol rate valid       */
#define START_FREQ_VALID(mask) \
    ((mask&SB_DVBC_SCAN_INFO_START_FREQ_VALID)>>4)    /**< marco for checking start frequency valid       */
#define END_FREQ_VALID(mask) \
    ((mask&SB_DVBC_SCAN_INFO_END_FREQ_VALID)>>5)    /**< marco for checking end frequency valid       */


#define SB_DVBC_PROGRESS_START              0    /**< progress when start scan      */
#define SB_DVBC_PROGRESS_PRIOR_SCAN_DONE    20    /**< progress when prior scan done       */
#define SB_DVBC_PROGRESS_NIT_FOUND(e_nit_search_mode)      \
                (UINT8)((e_nit_search_mode == SB_DVBC_NIT_SEARCH_MODE_EX_QUICK) ? 80 : 60)    /**< progress number when NIT found      */
#define SB_DVBC_PROGRESS_FINISH             100    /**< progress when scan finnished       */


/*mf type, for dvb-c multi-feed function, the value in ui1_mf_type*/
#define SB_DVB_MF_SERVICE_TYPE_NONE     ((UINT8) 0)
#define SB_DVB_MF_SERVICE_TYPE_PORTAL   ((UINT8) 1)
#define SB_DVB_MF_SERVICE_TYPE_SUBFEED  ((UINT8) 2)



/* DVB-C scan info */
/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_SCAN_INFO_T
 *  @brief DVB-C scan info
 *  @code
 *  typedef struct _SB_DVBC_SCAN_INFO_T{
 *      UINT16                  ui2_valid_mask;
 *      UINT16                  ui2_nw_id;
 *      TUNER_BANDWIDTH_T       e_bw;
 *      TUNER_MODULATION_T      e_mod;
 *      UINT32                  ui4_sym_rate;
 *      UINT32                  ui4_start_freq;
 *      UINT32                  ui4_end_freq;
 *  }   SB_DVBC_SCAN_INFO_T;
 *  @endcode
 *  @li@c  ui2_valid_mask                                    - the bit masks to indicate if the following parameters are valid or not
 *  @li@c  ui2_nw_id                                         - network id
 *  @li@c  e_bw                                              - bandwidth
 *  @li@c  e_mod                                             - modulation
 *  @li@c  ui4_sym_rate                                      - symbol rate
 *  @li@c  ui4_start_freq                                    - start frequency 
 *  @li@c  ui4_end_freq                                      - end frequency
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_SCAN_INFO_T{
    UINT16                  ui2_valid_mask;
    UINT16                  ui2_nw_id;
    TUNER_BANDWIDTH_T       e_bw;
    TUNER_MODULATION_T      e_mod;
    UINT32                  ui4_sym_rate;
    UINT32                  ui4_start_freq;
    UINT32                  ui4_end_freq;
}   SB_DVBC_SCAN_INFO_T;

/* The structure SB_DVBC_DLVR_INFO_T is the basic information about
    front end transmission. */
/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_DLVR_INFO_T
 *  @brief The structure SB_DVBC_DLVR_INFO_T is the basic information about
 *   front end transmission
 *  @code
 *  typedef struct _SB_DVBC_DLVR_INFO_T{
 *      UINT32                  ui4_freq;
 *      TUNER_MODULATION_T      e_mod;
 *      UINT32                  ui4_sym_rate;
 *  }SB_DVBC_DLVR_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          - frequency
 *  @li@c  e_mod                                             - modulation
 *  @li@c  ui4_sym_rate                                      - symbol rate
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_DLVR_INFO_T{
    UINT32                  ui4_freq;
    TUNER_MODULATION_T      e_mod;
    UINT32                  ui4_sym_rate;
}SB_DVBC_DLVR_INFO_T;

/* The structure SB_DVBC_DLVR_INFO_T is the basic information about
    front end transmission. */
/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_TUNER_PARA_T
 *  @brief The structure SB_DVBC_TUNER_PARA_T is the basic information about
 *   front end transmission
 *  @code
 *  typedef struct _SB_DVBC_TUNER_PARA_T{
 *      SB_DVBC_DLVR_INFO_T    t_dlvr_info;
 *      BOOL                   b_auto_qam_enable;
 *      BOOL                   b_auto_sr_enable;
 *  }SB_DVBC_TUNER_PARA_T;
 *  @endcode
 *  @li@c
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_TUNER_PARA_T{
    SB_DVBC_DLVR_INFO_T    t_dlvr_info;
    BOOL                   b_auto_qam_enable;
    BOOL                   b_auto_sr_enable;
}SB_DVBC_TUNER_PARA_T;


/* The structure SB_DVBC_SVC_UPDATE_NFY_DATA is used as the argument 
   of the service update notification function. */
/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_SVC_UPDATE_NFY_DATA
 *  @brief The structure SB_DVBC_SVC_UPDATE_NFY_DATA is used as the argument 
 *  of the service update notification function.
 *  @code
 *  typedef struct _SB_DVBC_SVC_UPDATE_NFY_DATA
 *  {
 *      UINT16                  ui2_num_added_svc;
 *      UINT16                  ui2_num_deleted_svc;
 *  } SB_DVBC_SVC_UPDATE_NFY_DATA;
 *  @endcode
 *  @li@c  ui2_num_added_svc                                 - the number of added services
 *  @li@c  ui2_num_deleted_svc                               - the number of deleted services
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_SVC_UPDATE_NFY_DATA
{
    UINT16                  ui2_num_added_svc;
    UINT16                  ui2_num_deleted_svc;
} SB_DVBC_SVC_UPDATE_NFY_DATA;


typedef struct _SB_DVB_SCANNED_CH_T
{
    UINT16                  ui2_num_tv_ch;
    UINT16                  ui2_num_radio_ch;
    UINT16                  ui2_num_app_ch;
} SB_DVB_SCANNED_CH_T;


/* The structure SB_DVBC_NW_CHANGE_NFY_DATA is used as the argument 
   of the network change notification function. */
/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_NW_CHANGE_NFY_DATA
 *  @brief The structure SB_DVBC_NW_CHANGE_NFY_DATA is used as the argument 
 *  of the network change notification function.
 *  @code
 *  typedef struct _SB_DVBC_NW_CHANGE_NFY_DATA
 *  {
 *      SB_DVBC_NIT_SEARCH_MODE_T   e_nit_search_mode;
 *  } SB_DVBC_NW_CHANGE_NFY_DATA;
 *  @endcode
 *  @li@c  e_nit_search_mode                                 - NIT search mode: ex-quick, quick or off
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_NW_CHANGE_NFY_DATA
{
    SB_DVBC_NIT_SEARCH_MODE_T   e_nit_search_mode;
} SB_DVBC_NW_CHANGE_NFY_DATA;


/* The prototype of the service update notification function */
/*------------------------------------------------------------------*/
/*! @brief The prototype of the service update notification function
 *  @param [in] pt_nfy_data     - pointer to a structure which contains information of service update.
 *  @param [in] pv_nfy_tag      - a tag which was provided at the setup time.
 *  @param [out]
 *  @note
 *  @see
 *  @return VOID
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_dvbc_svc_update_nfy_fct) (
        SB_DVBC_SVC_UPDATE_NFY_DATA* pt_nfy_data,
        VOID*                       pv_nfy_tag);

/* The prototype of the network modification notification function */
/*------------------------------------------------------------------*/
/*! @brief The prototype of the network modification notification function
 *  @param [in] pt_nfy_data     - pointer to a structure which contains information of network update
 *  @param [in] pv_nfy_tag       - a tag which was provide at the setup time
 *  @param [out]
 *  @note
 *  @see
 *  @return VOID
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_dvbc_nw_change_nfy_fct) (
        VOID*                       pv_nfy_tag);


/*
 *  The structure SB_DVBC_SCAN_DATA_T is used as the argument 
 *  pv_scan_data of the API x_sb_start_scan() when scaning 
 *  a media.
 *
 *  pt_country_code                 Contains an ISO 3166 3-character country code.
 *
 *  e_scan_type                     Specifies the way to scan.
 *
 *  t_scan_info                     Contains the required scan information. 
 *
 *  t_eng_cfg_flag                  Contains the engine configuration flag. The option allowed is 
 *                                  SB_DVBC_CONFIG_UPDATE_TO_TEMP_SVL.
 *
 *  ui2_temp_svl_id                 Contains the id of the temporary SVL to be updated. This
 *                                  argument is used only when the SB_DVBC_CONFIG_UPDATE_TO_TEMP_SVL
 *                                  is set. This list must exist and be an empty one.
 *
 *  pf_svc_update_nfy               Contains the service update notification function.
 *
 *  pv_svc_update_nfy_tag           Contains the tag for the service update notification function.
 *
 */
/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_SCAN_DATA_T
 *  @brief The structure SB_DVBC_SCAN_DATA_T is used as the argument 
 *  pv_scan_data of the API x_sb_start_scan() when scaning 
 *  a media.
 *  @code
 *  typedef struct _SB_DVBC_SCAN_DATA_T
 *  {
 *      ISO_3166_COUNT_T        t_country_code;
 *      OPERATOR_NAME_T         t_operator_name;
 *      SB_DVBC_SCAN_TYPE_T      e_scan_type;
 *      SB_DVBC_NIT_SEARCH_MODE_T   e_nit_search_mode;
 *      SB_DVBC_SCAN_INFO_T      t_scan_info;
 *      SB_DVBC_CONFIG_T         t_eng_cfg_flag;
 *      UINT16                  ui2_temp_svl_id;
 *      x_sb_dvbc_svc_update_nfy_fct pf_svc_update_nfy;
 *      VOID*                   pv_svc_update_nfy_tag;
 *  } SB_DVBC_SCAN_DATA_T;
 *  @endcode
 *  @li@c  t_country_code                                    - Contains an ISO 3166 3-character country code.
 *  @li@c  t_operator_name                                   - Contains an Operation name 
 *  @li@c  e_scan_type                                       - Specifies the way to scan.
 *  @li@c  e_nit_search_mode                                 - Specifies the mode of NIT search
 *  @li@c  t_scan_info                                       - Contains the required scan information. 
 *  @li@c  t_eng_cfg_flag                                    - Contains the engine configuration flag.
 *  @li@c  ui2_temp_svl_id                                   - Contains the id of the temporary SVL to be updated. This
 *                                  argument is used only when the SB_DVBC_CONFIG_UPDATE_TO_TEMP_SVL
 *                                  is set. This list must exist and be an empty one.
 *  @li@c  pf_svc_update_nfy                                 - Contains the service update notification function.
 *  @li@c  pv_svc_update_nfy_tag                             - Contains the tag for the service update notification function.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_SCAN_DATA_T
{
    ISO_3166_COUNT_T        t_country_code;
    OPERATOR_NAME_T         t_operator_name;
    SB_DVBC_SCAN_TYPE_T      e_scan_type;
    SB_DVBC_NIT_SEARCH_MODE_T   e_nit_search_mode;
    SB_DVBC_SCAN_INFO_T      t_scan_info;
    SB_DVBC_CONFIG_T         t_eng_cfg_flag;
    UINT16                  ui2_temp_svl_id;
    x_sb_dvbc_svc_update_nfy_fct pf_svc_update_nfy;
    VOID*                   pv_svc_update_nfy_tag;
} SB_DVBC_SCAN_DATA_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_TSL_REC_ID_FREQ_RANGE_T
 *  @brief the frequency range of certain TSL record ID
 *  @code
 *  typedef struct _SB_DVBC_TSL_REC_ID_FREQ_RANGE
 *  {
 *      UINT32                  ui4_freq_range_lbound;
 *      UINT32                  ui4_freq_range_ubound;
 *  } SB_DVBC_TSL_REC_ID_FREQ_RANGE_T;
 *  @endcode
 *  @li@c  ui4_freq_range_lbound                             - the lower bound of the range
 *  @li@c  ui4_freq_range_ubound                             - the upper bound of the range
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_TSL_REC_ID_FREQ_RANGE
{
    UINT32                  ui4_freq_range_lbound;
    UINT32                  ui4_freq_range_ubound;
} SB_DVBC_TSL_REC_ID_FREQ_RANGE_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_ON_TS_SVC_ID_T
 *  @brief the triplet of DVB IDS, on_id, ts_id and service_id
 *  @code
 *  typedef struct _SB_DVBC_ON_TS_SVC_ID
 *  {
 *      UINT16                  ui2_on_id;
 *      UINT16                  ui2_ts_id;
 *      UINT16                  ui2_svc_id;
 *  } SB_DVBC_ON_TS_SVC_ID_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         - original network ID
 *  @li@c  ui2_ts_id                                         - Transport stream ID
 *  @li@c  ui2_svc_id                                        - Service ID
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_ON_TS_SVC_ID
{
    UINT16                  ui2_on_id;
    UINT16                  ui2_ts_id;
    UINT16                  ui2_svc_id;
} SB_DVBC_ON_TS_SVC_ID_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBC_NW_CHANGE_NFY_T
 *  @brief SB_DVBC_NW_CHANGE_NFY_T is used to setup a notification function with c_sb_set and keytype is SB_KEY_TYPE_NW_CHANGE_NFY
 *  @code
 *  typedef struct _SB_DVBC_NW_CHANGE_NFY_T {
 *      x_sb_dvbc_nw_change_nfy_fct  pf_nw_change_nfy;
 *      VOID*                   pv_nw_change_nfy_tag;
 *  } SB_DVBC_NW_CHANGE_NFY_T;
 *  @endcode
 *  @li@c  pf_nw_change_nfy                                  - the function pointer of network change notifcation
 *  @li@c  pv_nw_change_nfy_tag                              - the tag would be used when invoke the notification function.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBC_NW_CHANGE_NFY_T {
    x_sb_dvbc_nw_change_nfy_fct  pf_nw_change_nfy;
    VOID*                   pv_nw_change_nfy_tag;
} SB_DVBC_NW_CHANGE_NFY_T;


/* Macros for DVB-C channel number */
/* These macros are designed to conform to the ATSC ones so that using the ATSC macros can
   also retrieve the channel numbers */
#define SB_DVBC_CH_NUM_FLAG                       SB_DVB_CH_NUM_FLAG/**<  to indicate it's one-part channel number     */

#define SB_DVBC_CH_TYPE_RANGE                     SB_DVB_CH_TYPE_RANGE         /*The last 3bit is used for service type*/ 
#define SB_DVBC_MAX_CHANNEL_INDEX_WITH_CH_TYPE    SB_DVB_MAX_CHANNEL_INDEX_WITH_CH_TYPE


#define SB_DVBC_MAX_CHANNEL_INDEX                 SB_DVB_MAX_CHANNEL_INDEX/**<  bit masks for channel index */

#define SB_DVBC_GEN_CHANNEL_ID(major, index)      SB_DVB_GEN_CHANNEL_ID(major, index) /**< to generate channel id with major number and index */

#define SB_DVBC_GEN_CHANNEL_ID_WITH_LCN(major, lcn, index) \
    SB_DVB_GEN_CHANNEL_ID_WITH_LCN(major, lcn, index)/**< to generate channel id with major number, lcn and index */
                     
#define SB_DVBC_GET_CHANNEL_NUM(channel_id) \
    SB_DVB_GET_CHANNEL_NUM(channel_id)    /**< to get the major channel number with channel_id       */
                    
#define SB_DVBC_GET_LCN(channel_id)               SB_DVB_GET_LCN(channel_id)/**< to get LCN with channel_id       */


#define SB_DVBC_GET_CHANNEL_INDEX(channel_id)     SB_DVB_GET_CHANNEL_INDEX(channel_id)/**< to get channel index with channel_id       */


#endif /* _U_SB_DVB_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_SVL_BLDR_SB_DVBC_ENG */
/*----------------------------------------------------------------------------*/

