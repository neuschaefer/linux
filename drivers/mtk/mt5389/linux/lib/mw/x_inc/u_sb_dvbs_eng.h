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
/*! @file u_sb_dvbs_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         SVL Builder DVB-S engine.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef _U_SB_DVBS_ENG_H_
#define _U_SB_DVBS_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_sb.h"
#include "u_tuner.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVL_BLDR_SB_DVBS_ENG SVL Builder DVB-S engine
 *
 *  @ingroup groupMW_SVL_BLDR
 *  @brief The DVB-S engine module is used to scan all digital channel in DVB-S spec.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could create scan engine
 *   or start scan from the exported API of DVB-S svl builder.
 *
 *  @see groupMW_SVL_BLDR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define DVBS_ENG_NAME                "sb_dvbs"    /**< Define the DVB-S build engine name */
#define DVBS_DEFAULT_TUNER           "CT_DVBS"    /**< Define the default tuner in DVB-S build engine */

                                    
#define SB_DVBS_PROGRESS_START              0    /**< progress when start scan      */
#define SB_DVBS_PROGRESS_NIT_FOUND          60    /**< progress number when NIT found      */
#define SB_DVBS_PROGRESS_FINISH             100    /**< progress when scan finnished       */

/* Command to get the scan map and info */
/*------------------------------------------------------------------*/
/*! @enum DVBS_SCAN_MAPS_CMD_T
 *  @brief Command to get the dvb-s info.
 *  @code
 *  typedef enum {
 *      DVBS_SCAN_MAPS_CMD_GET_DEFAULT_TSL_REC_PARA = 0,
 *      DVBS_SCAN_MAPS_CMD_GET_DEFAULT_SATL_REC_PARA,
 *      DVBS_SCAN_MAPS_CMD_GET_NUM_DEFAULT_LNB_CONFIG,   
 *      DVBS_SCAN_MAPS_CMD_GET_DEFAULT_LNB_CONFIG,
 *      DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING,
 *      DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING,
 *      DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME,
 *      DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME,
 *      DVBS_SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY,
 *      DVBS_SCAN_MAPS_CMD_IS_ASTRA_SCAN,
 *      DVBS_SCAN_MAPS_CMD_GET_FREQ_RANGE_BY_TSL_REC_ID,
 *  }   DVBS_SCAN_MAPS_CMD_T;
 *  @endcode
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_DEFAULT_TSL_REC_PARA                 - Get default parameters of TSL record. note: will memset to 0 filrst, and fill transmission related paras.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_DEFAULT_SATL_REC_PARA                - Get default parameters of SATL record. note: will memset to 0 filrst, and fill transmission related paras.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_NUM_DEFAULT_LNB_CONFIG               - Get number of default LNB configurations.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_DEFAULT_LNB_CONFIG                   - Get default LNB configuration by index
 *  @li@c  DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING                    - Check it support customized sorting or not.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING                   - Get the sorting rule with current country.
 *  @li@c  DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME            - Check it support customized sorting by name or not.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME           - Get the rule of sorting by name with current country.
 *  @li@c  DVBS_SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY                       - Check it support LCN or not by current country.
 *  @li@c  DVBS_SCAN_MAPS_CMD_IS_ASTRA_SCAN                            - Check the satellite is adopted astra scan.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_FREQ_RANGE_BY_TSL_REC_ID             - get upper/lower bound of the tsl record by record id
 */
/*------------------------------------------------------------------*/
typedef enum {
    DVBS_SCAN_MAPS_CMD_GET_DEFAULT_TSL_REC_PARA = 0,
    DVBS_SCAN_MAPS_CMD_GET_DEFAULT_SATL_REC_PARA,
    DVBS_SCAN_MAPS_CMD_GET_NUM_DEFAULT_LNB_CONFIG,   
    DVBS_SCAN_MAPS_CMD_GET_DEFAULT_LNB_CONFIG,
    DVBS_SCAN_MAPS_CMD_DETERMINE_PRIVATE_DATA_SPECIFIER,
    DVBS_SCAN_MAPS_CMD_IS_INSTALL_TEMP_NW_SVC_COUNTRY,
    DVBS_SCAN_MAPS_CMD_IS_SERVICE_REMOVAL_MUST_BE_CONFIRMED_COUNTRY,
    DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING,
    DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING,
    DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME,
    DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME,
    DVBS_SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY,
    DVBS_SCAN_MAPS_CMD_GET_FREQUENCY_TOLERANCE,
    DVBS_SCAN_MAPS_CMD_SET_FREQUENCY_TOLERANCE,
    DVBS_SCAN_MAPS_CMD_IS_ASTRA_SCAN,
    DVBS_SCAN_MAPS_CMD_GET_FREQ_RANGE,
} DVBS_SCAN_MAPS_CMD_T;
 
/*------------------------------------------------------------------*/
/*! @brief The callback function for getting customized information from DVB-S scan map.
 *  @param [in] t_cmd   The command to get the scan map and info.
 *  @param [in] t_country_code  The current country information.
 *  @param [in] pv_input    The input parameter for getting customized info.
 *  @param [out] pv_output  The buffer for output value.
 *  @param [out] pz_output_len  The length for output buffer.
 *  @return
 *  @retval   - SBR_INV_ARG  The current input value is invalid.
 *  @retval   - SBR_OUT_OF_RANGE The input value is out of range.
 *  @retval   - SBR_NOT_ENOUGH_SPACE  The output length is out of space.
 *  @retval   - SBR_NOT_FOUND  It can not found in this prcess.
 *  @retval   - SBR_OK  Routine successful.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_dvbs_scan_maps_fct) (DVBS_SCAN_MAPS_CMD_T     t_cmd,
                                         ISO_3166_COUNT_T    t_country_code,
                                         VOID*               pv_input,
                                         VOID*               pv_output,
                                         SIZE_T*             pz_output_len);


/* This data type contains different configuration flags for the 
   scan process. */
typedef UINT32      SB_DVBS_CONFIG_T;    /**< Define the different configuration flags for the DVB-S scan process*/

#define SB_DVBS_CONFIG_SUPPORT_MHEG5_SERVICES            (MAKE_BIT_MASK_32(0))    
/**<  Install the MHEG-5 services as visible ones */

#define SB_DVBS_CONFIG_KEEP_DUPLICATE_CHANNELS           (MAKE_BIT_MASK_32(1))    
/**< Do not remove duplicate channels */

#define SB_DVBS_CONFIG_NOT_SUPPORT_HDTV                  (MAKE_BIT_MASK_32(2)) 
/**< Do not support HDTV (H.264 & MPEG-2) */

#define SB_DVBS_CONFIG_INSTALL_FREE_SVC_ONLY             (MAKE_BIT_MASK_32(3))
/*To install free service only*/

#define SB_DVBS_CONFIG_INSTALL_TV_SVC_ONLY               (MAKE_BIT_MASK_32(4))
/*To install TV service only, 
  SB_DVBS_CONFIG_INSTALL_RADIO_SVC_ONLY cannot be flagged at the same time*/

#define SB_DVBS_CONFIG_INSTALL_RADIO_SVC_ONLY            (MAKE_BIT_MASK_32(5))
/*To install TV service only, 
  SB_DVBS_CONFIG_INSTALL_TV_SVC_ONLY cannot be flagged at the same time*/

#define SB_DVBS_CONFIG_TV_RADIO_SEPARATE                 (MAKE_BIT_MASK_32(6)) 
/**< To support TV type and Raido type service in the its channel list */

#define SB_DVBS_CONFIG_COMP_OTHER_TS                     (MAKE_BIT_MASK_32(7))

#define SB_DVBS_CONFIG_RESERVE_CH_NUM_BEFORE_NON_LCN_CH  (MAKE_BIT_MASK_32(8))

#define SB_DVBS_CONFIG_SIMPLE_SORT_FOR_NON_LCN_CH        (MAKE_BIT_MASK_32(9))

#define SB_DVBS_CONFIG_SORTING_BY_FREQ                   (MAKE_BIT_MASK_32(10))

#define SB_DVBS_CONFIG_SUPPORT_MHP_SERVICES              (MAKE_BIT_MASK_32(11))

#define SB_DVBS_CONFIG_NOT_INSTALL_DATA_SERVICE          (MAKE_BIT_MASK_32(12))

/**< If the onid, tsid and serviceID is the same, But the frequency is not same do not filter this service.*/
#define SB_DVBS_CONFIG_UNCHECK_SVC_LOC_WITH_FREQ           (MAKE_BIT_MASK_32(13))



/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_ON_TS_SVC_ID_T
 *  @brief The locator of service in DVB system.
 *  @code
 *  typedef struct _SB_DVBS_ON_TS_SVC_ID_T
 *  {
 *      UINT16                  ui2_on_id;
 *      UINT16                  ui2_ts_id;
 *      UINT16                  ui2_svc_id;
 *  } SB_DVBS_ON_TS_SVC_ID_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         - The original network id.
 *  @li@c  ui2_ts_id                                         - The transport stream id.
 *  @li@c  ui2_svc_id                                        - The service identify.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_ON_TS_SVC_ID
{
    UINT16                  ui2_on_id;
    UINT16                  ui2_ts_id;
    UINT16                  ui2_svc_id;
} SB_DVBS_ON_TS_SVC_ID_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_INIT_DATA_T
 *  @brief It is used as the argument of the API x_sb_dvbs_eng_init() when initializing an SVL Builder with DVB-S engine.
 *  @code
 *  typedef struct _SB_DVBS_INIT_DATA_T
 *  {
 *      SB_DVBS_CONFIG_T         t_eng_cfg_flag;
 *      x_sb_dvbs_scan_maps_fct  pf_scan_maps;
 *  } SB_DVBS_INIT_DATA_T;
 *  @endcode
 *  @li@c  t_eng_cfg_flag                                    - Contains the engine configuration flag.
 *  @li@c  pf_scan_maps                                      - Contains the scan map callback function.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_INIT_DATA_T
{
    SB_DVBS_CONFIG_T         t_eng_cfg_flag;
    x_sb_dvbs_scan_maps_fct  pf_scan_maps;
} SB_DVBS_INIT_DATA_T;


/*! @struct SB_DVBS_OPEN_DATA_T
 *  @brief It is used as the argument pv_open_data of the API c_sb_open_builder() when opening an SVL Builder with DVB-S engine
 *  @code
 *  typedef struct _SB_DVBS_OPEN_DATA_T
 *  {
 *      UINT16                  ui2_svl_id;
 *      CHAR*                   ps_svl_name;
 *      CHAR*                   ps_file_name;
 *      CHAR*                   ps_tuner_name;    
 *      BOOL                    b_use_orig;
 *  } SB_DVBS_OPEN_DATA_T;
 *  @endcode
 *  @li@c  ui2_svl_id                                        - The id of SVL to be loaded.
 *  @li@c  ps_svl_name                                       - The name of SVL to be loaded.
 *  @li@c  ps_file_name                                      - The file name from which the SVL/TSL will be loaded.
 *  @li@c  ps_tuner_name                                     - The name of tuner to be connectted.
 *  @li@c  b_use_orig                                        - A flag. When set to true, use the existed SVL, instead of reloading it
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_OPEN_DATA_T
{
    UINT16                  ui2_svl_id;
    CHAR*                   ps_svl_name;
    CHAR*                   ps_file_name;
    CHAR*                   ps_tuner_name;    
    BOOL                    b_use_orig;
} SB_DVBS_OPEN_DATA_T;


/*------------------------------------------------------------------*/
/*! @enum SB_DVBS_SCAN_TYPE_T
 *  @brief It contains different scan types that are used to initialize scan parameters.
 *  @code
 *  typedef enum
 *  {
 *      SB_DVBS_SCAN_TYPE_UNKNOWN = 0,
 *      SB_DVBS_SCAN_TYPE_AUTO_MODE,
 *      SB_DVBS_SCAN_TYPE_PRESET_MODE,
 *      SB_DVBS_SCAN_TYPE_SINGLE_TP_SCAN,
 *      SB_DVBS_SCAN_TYPE_UPDATE_SCAN,
 *      SB_DVBS_SCAN_TYPE_NUM
 *  } SB_DVBS_SCAN_TYPE_T;
 *  @endcode
 *  @li@c  SB_DVBS_SCAN_TYPE_UNKNOWN                          - Unknown.
 *  @li@c  SB_DVBS_SCAN_TYPE_AUTO_MODE                        - Blind Scan.
 *  @li@c  SB_DVBS_SCAN_TYPE_PRESET_MODE                      - Scan with preset transponders.
 *  @li@c  SB_DVBS_SCAN_TYPE_SINGLE_TP_SCAN                   - Scan single transponder.
 *  @li@c  SB_DVBS_SCAN_TYPE_UPDATE_SCAN                      - BGM scan
 *  @li@c  SB_DVBS_SCAN_TYPE_NUM                 				 		  - Number of scan types. 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_DVBS_SCAN_TYPE_UNKNOWN = 0,
    SB_DVBS_SCAN_TYPE_AUTO_MODE,
    SB_DVBS_SCAN_TYPE_PRESET_MODE,
    SB_DVBS_SCAN_TYPE_SINGLE_TP_SCAN,
    SB_DVBS_SCAN_TYPE_UPDATE_SCAN,
    SB_DVBS_SCAN_TYPE_NUM
} SB_DVBS_SCAN_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_PRESET_SCAN_INFO_T
 *  @brief the required information of DVB-S preset scan
 *  @code
 *  typedef struct _SB_DVBS_PRESET_SCAN_INFO_T{
 *        BOOL    b_network_search;
 *        UINT16  ui2_nw_id;
 *  }   SB_DVBS_PRESET_SCAN_INFO_T;
 *  @endcode
 *  @li@c  b_network_search                               - network search on/off
 *  @li@c  ui2_nw_id                                      - network id, if ui2_nw_id ==0, engine would load NIT actual
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_PRESET_SCAN_INFO_T{
    BOOL    b_network_search;
    UINT16  ui2_nw_id;
}   SB_DVBS_PRESET_SCAN_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SINGLE_TP_SCAN_INFO_T
 *  @brief the required information of DVB-S preset scan
 *  @code
 *  typedef struct _SB_DVBS_SINGLE_TP_SCAN_INFO_T{
 *        BOOL    b_network_search;
 *        UINT16  ui2_nw_id;
 *        UINT16  ui2_tsl_rec_id;
 *  }   SB_DVBS_SINGLE_TP_SCAN_INFO_T;
 *  @endcode
 *  @li@c  b_network_search                               - network search on/off
 *  @li@c  ui2_nw_id                                      - network id, if ui2_nw_id ==0, engine would load NIT actual
 *  @li@c  ui2_tsl_rec_id                                 - record id of the target transponder. the tsl id should be the same as satl id.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SINGLE_TP_SCAN_INFO_T{
    BOOL    b_network_search;
    UINT16  ui2_nw_id;
    UINT16  ui2_tsl_rec_id;
}   SB_DVBS_SINGLE_TP_SCAN_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SCAN_INFO_T
 *  @brief DVB-S scan info
 *  @code
 *  typedef struct _SB_DVBS_SCAN_INFO_T{
 *      UINT16                  ui2_satl_id;
 *      UINT16                  ui2_satl_rec_id;
 *      union{
 *          SB_DVBS_PRESET_SCAN_INFO         t_preset_scan_info;
 *          SB_DVBS_SINGLE_TP_SCAN_INFO      t_single_tp_scan_info;  
 *      }uinfo;
 *  }   SB_DVBS_SCAN_INFO_T;
 *  @endcode
 *  @li@c  ui2_satl_id                                    - SATL ID
 *  @li@c  ui2_satl_rec_id                                - the record id of selected satellite
 *  @li@c  t_preset_scan_info                             - the required information of preset scan
 *  @li@c  t_single_tp_scan_info                          - the required information of single TP scan        
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SCAN_INFO_T{
    UINT16                  ui2_satl_id;
    UINT16                  ui2_satl_rec_id;
    union{
        SB_DVBS_PRESET_SCAN_INFO_T         t_preset_scan_info;
        SB_DVBS_SINGLE_TP_SCAN_INFO_T      t_single_tp_scan_info;  
    }uinfo;
}   SB_DVBS_SCAN_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_DLVR_INFO_T
 *  @brief The structure SB_DVBS_DLVR_INFO_T is the basic information about
 *   front end transmission
 *  @code
 *  typedef struct _SB_DVBS_DLVR_INFO_T{
 *      UINT32                  ui4_freq;
 *      UINT8                   e_pol;
 *      UINT32                  ui4_sym_rate;
 *  }SB_DVBC_DLVR_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          - frequency
 *  @li@c  e_pol                                             - polarization
 *  @li@c  ui4_sym_rate                                      - symbol rate
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_DLVR_INFO_T{
    UINT32                  ui4_freq;
    TUNER_POLARIZATION_T    e_pol;
    UINT32                  ui4_sym_rate;
}SB_DVBS_DLVR_INFO_T;
/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SVC_UPDATE_NFY_DATA
 *  @brief It is used as the argument of the service update notification function.
 *  @code
 *  typedef struct _SB_DVBS_SVC_UPDATE_NFY_DATA
 *  {
 *      UINT16                  ui2_num_added_svc;
 *      UINT16                  ui2_num_deleted_svc;
 *  } SB_DVBS_SVC_UPDATE_NFY_DATA;
 *  @endcode
 *  @li@c  ui2_num_added_svc                                 - The count of added service in this progress.
 *  @li@c  ui2_num_deleted_svc                               - The count of deleted service in this progress.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SVC_UPDATE_NFY_DATA
{
    UINT16                  ui2_num_added_svc;
    UINT16                  ui2_num_deleted_svc;
} SB_DVBS_SVC_UPDATE_NFY_DATA;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_AUTO_FREQ_RANGE
 *  @brief It is used as the argument of get the dvbs middle frequency range.
 *  @code
 *  typedef struct _SB_DVBS_AUTO_FREQ_RANGE
 *  {
 *      UINT16                  ui2_start_freq;
 *      UINT16                  ui2_end_freq;
 *  } SB_DVBS_AUTO_FREQ_RANGE;
 *  @endcode
 *  @li@c  ui2_start_freq                             - The start frequncy of the range in MHZ.
 *  @li@c  ui2_end_freq                               - The end frequncy of the range in MHZ.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_AUTO_FREQ_RANGE
{
    UINT16                  ui2_start_freq; /*MHZ*/
    UINT16                  ui2_end_freq;/*MHZ*/
} SB_DVBS_AUTO_FREQ_RANGE;

/*------------------------------------------------------------------*/
/*! @brief  Structure of tuner TUNER information for DVB-S
 *  @code
 *  typedef struct _SB_DVBS_TUNER_DATA_T
 *  {
 *      TUNER_LNB_TYPE_T    e_lnb_type; 
 *      UINT16              ui2_lnb_low_freq; 
 *      UINT16              ui2_lnb_high_freq; 
 *      UINT16              ui2_lnb_switch_freq; 
 *      UINT8               ui1_22k;
 *      UINT8               ui1_polarity_13v_18v;  
 *  } SB_DVBS_TUNER_DATA_T;
 *  @endcode
 *  @li@c  e_lnb_type                    - LNB type
 *  @li@c  ui2_lnb_low_freq          - LNB low frequency in MHz
 *  @li@c  ui2_lnb_high_freq         - LNB high frequency in MHz
 *  @li@c  ui2_lnb_switch_freq       - LNB switch frequency in MHz
 *  @li@c  ui1_22k                   - 22k on or off
 *  @li@c  ui1_polarity_13v_18v      - 13V or 18V
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_TUNER_DATA_T
{
    TUNER_LNB_TYPE_T    e_lnb_type; /*LNB_UNKNOWN, LNB_SINGLE_FREQ, LNB_DUAL_FREQ*/
    UINT16              ui2_lnb_low_freq; /*in MHz*/
    UINT16              ui2_lnb_high_freq; /*in MHz*/
    UINT16              ui2_lnb_switch_freq; /*in MHz, for dual freq*/
    UINT8               ui1_22k;/* 0-> 22k off,1->22k on */
    UINT8               ui1_polarity_13v_18v;   /* 0->V(13v) ,1->H(18v)*/
} SB_DVBS_TUNER_DATA_T;

/*------------------------------------------------------------------*/
/*! @brief The prototype of the service update notification function
 *  @param [in] pt_nfy_data The information for added and deleted services.
 *  @param [in] pv_nfy_tag The tag for notifying user.
 *  @param [out] None.
 *  @return
 *  @retval      - None.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_dvbs_svc_update_nfy_fct) (
        SB_DVBS_SVC_UPDATE_NFY_DATA* pt_nfy_data,
        VOID*                       pv_nfy_tag);


/*------------------------------------------------------------------*/
/*! @brief It is used for notifying user the updating network.
 *  @param [in] pv_nfy_tag The tag for notifying user.
 *  @param [out] None.
 *  @return
 *  @retval      - None.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_dvbs_nw_change_nfy_fct) (
        VOID*                       pv_nfy_tag);


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SCAN_DATA_T
 *  @brief It is used as the argument pv_scan_data of the API x_sb_start_scan() when scaning a media.
 *  @code
 *  typedef struct _SB_DVBS_SCAN_DATA_T
 *  {
 *      ISO_3166_COUNT_T                t_country_code;
 *      SB_DVBS_SCAN_TYPE_T             e_scan_type;
 *      SB_DVBS_SCAN_INFO_T             t_scan_info;
 *      SB_DVBS_CONFIG_T                t_eng_cfg_flag;
 *      x_sb_dvbs_svc_update_nfy_fct    pf_svc_update_nfy;
 *      VOID*                           pv_svc_update_nfy_tag;
 *      x_sb_new_svc_nfy_fct        pf_new_svc_nfy;
 *      VOID*                           pv_new_svc_nfy_tag;
 *  } SB_DVBS_SCAN_DATA_T;
 *  @endcode
 *  @li@c  t_country_code                                    - Contains an ISO 3166 3-character country code.
 *  @li@c  e_scan_type                                       - Specifies the way to scan.
 *  @li@c  t_scan_info                                       - Contains the required scan information.
 *  @li@c  t_eng_cfg_flag                                    - Contains the engine configuration flag.
 *  @li@c  pf_svc_update_nfy                                 - Contains the tag for the service update notification function.
 *  @li@c  pv_svc_update_nfy_tag                             - Contains the tag for the service update notification function.
 *  @li@c  pf_new_svc_nfy                                    - Notify app the information of every new service
 *  @li@c  pv_new_svc_nfy_tag                                - Contains the taf for the new service notification function.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SCAN_DATA_T
{
    ISO_3166_COUNT_T        t_country_code;
    SB_DVBS_SCAN_TYPE_T      e_scan_type;
    SB_DVBS_SCAN_INFO_T      t_scan_info;
    SB_DVBS_CONFIG_T         t_eng_cfg_flag;
    x_sb_dvbs_svc_update_nfy_fct pf_svc_update_nfy;
    VOID*                   pv_svc_update_nfy_tag;
    x_sb_new_svc_nfy_fct    pf_new_svc_nfy;
    VOID*                   pv_new_svc_nfy_tag;
} SB_DVBS_SCAN_DATA_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_TSL_REC_ID_FREQ_RANGE_T
 *  @brief The sub information for range of lower and upper bound.
 *  @code
 *  typedef struct _SB_DVBS_TSL_REC_ID_FREQ_RANGE
 *  {
 *      UINT32                  ui4_freq_range_lbound;
 *      UINT32                  ui4_freq_range_ubound;
 *  } SB_DVBS_TSL_REC_ID_FREQ_RANGE_T;
 *  @endcode
 *  @li@c  ui4_freq_range_lbound                             - The frequency of lower bound.
 *  @li@c  ui4_freq_range_ubound                             - The frequency of upper bound.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_TSL_REC_ID_FREQ_RANGE
{
    UINT32                  ui4_freq_range_lbound;
    UINT32                  ui4_freq_range_ubound;
} SB_DVBS_TSL_REC_ID_FREQ_RANGE_T;




/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_NW_CHANGE_NFY_T
 *  @brief The structure for the notify information when network updated.
 *  @code
 *  typedef struct _SB_DVBS_NW_CHANGE_NFY_T {
 *      x_sb_dvbs_nw_change_nfy_fct  pf_nw_change_nfy;
 *      VOID*                        pv_nw_change_nfy_tag;
 *  } SB_DVBS_NW_CHANGE_NFY_T;
 *  @endcode
 *  @li@c  pf_nw_change_nfy                                  - The callback function for network updated.
 *  @li@c  pv_nw_change_nfy_tag                              - The private tag in notify progress.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_NW_CHANGE_NFY_T {
    x_sb_dvbs_nw_change_nfy_fct  pf_nw_change_nfy;
    VOID*                   pv_nw_change_nfy_tag;
} SB_DVBS_NW_CHANGE_NFY_T;


/* Macros for DVB-S channel number */
/* These macros are designed to conform to the ATSC ones so that using the ATSC macros can
   also retrieve the channel numbers */

#define SB_DVBS_CH_TYPE_RANGE                     3         /*The last 3bit is used for service type*/ 
#define SB_DVBS_MAX_CHANNEL_INDEX_WITH_CH_TYPE  ((UINT8) 0x7)
#define SB_DVBS_CH_NUM_FLAG              ((UINT32) 0x80)    /**< Define the flag for used DVB channel number. */

#define SB_DVBS_MAX_CHANNEL_INDEX        ((UINT8) 0x3f)    /**<  Define the max DVB channel index. */

#define SB_DVBS_GEN_CHANNEL_ID(major, index) \
    ((((UINT32) (major) & 0x3fff) << 18) | \
     ((index & SB_DVBS_MAX_CHANNEL_INDEX)) | \
     SB_DVBS_CH_NUM_FLAG)    /**< Define the generated DVB channel ID without LCN.*/

#define SB_DVBS_GEN_CHANNEL_ID_WITH_LCN(major, lcn, index) \
    ((((UINT32) (major) & 0x3fff) << 18) | \
     (((UINT32) (lcn) & 0x3ff)  <<  8) | \
     ((index & SB_DVBS_MAX_CHANNEL_INDEX)) | \
     SB_DVBS_CH_NUM_FLAG)    /**<  Define the generated DVB channel ID with LCN.*/
                     
#define SB_DVBS_GET_CHANNEL_NUM(channel_id) \
    ((UINT16)(((UINT32)(channel_id) >> 18) & 0x3fff))    /**<  Define the process of getting channel number by channel ID.*/
                    
#define SB_DVBS_GET_LCN(channel_id) \
    ((UINT16)(((UINT32)(channel_id) >> 8) & 0x3ff))    /**<    Define the process of getting LCN by channel ID.*/

#define SB_DVBS_GET_CHANNEL_INDEX(channel_id) \
    ((UINT8) (((UINT32)(channel_id)) & SB_DVBS_MAX_CHANNEL_INDEX))    /**<  Define the process of getting channel index by channel ID.*/


#endif /* _U_SB_DVBS_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_SVL_BLDR_SB_DVBS_ENG */
/*----------------------------------------------------------------------------*/

