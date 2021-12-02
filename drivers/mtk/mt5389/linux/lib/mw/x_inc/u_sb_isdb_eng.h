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
/*! @file u_sb_isdb_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         SVL Builder DVB engine.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef _U_SB_ISDB_ENG_H_
#define _U_SB_ISDB_ENG_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVL_BLDR_SB_ISDB_ENG SVL Builder ISDB engine
 *
 *  @ingroup groupMW_SVL_BLDR
 *  @brief The ISDB-T engine module is used to scan all digital channels in ISDB-T spec.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could create scan engine
 *   or start scan from the exported API of ISDB-T svl builder.
 *
 *  @see groupMW_SVL_BLDR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define ISDB_ENG_NAME                "sb_isdb"    /**<  Define the ISDB build engine name */
#define ISDB_DEFAULT_TUNER           "CT_ISDB"    /**<  Define the default tuner in ISDB build engine */
                                    
#define ISDB_TS_NAME_LEN             ((UINT32)20)    /**<   Define the max length of ISDB ts name */
                                    

/*------------------------------------------------------------------*/
/*! @enum SCAN_MAPS_ISDB_CMD_T
 *  @brief Command to get the isdb scan map and info.
 *  @code
 *  typedef enum {
 *      SCAN_MAPS_ISDB_CMD_GET_FREQ = 0,
 *      SCAN_MAPS_ISDB_CMD_GET_BW,
 *      SCAN_MAPS_ISDB_CMD_GET_FIRST_SCAN_IDX,
 *      SCAN_MAPS_ISDB_CMD_GET_LAST_SCAN_IDX,
 *      SCAN_MAPS_ISDB_CMD_GET_NEXT_SCAN_IDX,
 *      SCAN_MAPS_ISDB_CMD_GET_PREV_SCAN_IDX,
 *      SCAN_MAPS_ISDB_CMD_GET_FIRST_UHF_SCAN_IDX,       
 *      SCAN_MAPS_ISDB_CMD_GET_RF_CHANNEL,
 *      SCAN_MAPS_ISDB_CMD_GET_COUNTRY_CODE_BY_IDX,
 *      SCAN_MAPS_ISDB_CMD_GET_TSL_REC_ID,               
 *      SCAN_MAPS_ISDB_CMD_GET_TSL_REC_ID_BY_FREQ,       
 *      SCAN_MAPS_ISDB_CMD_GET_FREQ_RANGE_BY_TSL_REC_ID, 
 *      SCAN_MAPS_ISDB_CMD_GET_SCAN_IDX_BY_FREQ,         
 *      SCAN_MAPS_ISDB_CMD_IS_CUSTOMIZED_SORTING,
 *      SCAN_MAPS_ISDB_CMD_GET_CUSTOMIZED_SORTING,
 *  }   SCAN_MAPS_ISDB_CMD_T;
 *  @endcode
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_FREQ                       - Get frequency.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_BW                         - Get bandwidth.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_FIRST_SCAN_IDX             - Get the first scan index.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_LAST_SCAN_IDX              - Get the last scan index.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_NEXT_SCAN_IDX              - Get the next scan index.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_PREV_SCAN_IDX              - Get the previous scan index.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_FIRST_UHF_SCAN_IDX         - Get the first scan index in UHF band.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_RF_CHANNEL                 - Get the RF channel.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_COUNTRY_CODE_BY_IDX        - Get the country code by index.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_TSL_REC_ID                 - Get the TSL record id by scan index.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_TSL_REC_ID_BY_FREQ         - Get the TSL record id by frequency.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_FREQ_RANGE_BY_TSL_REC_ID   - Get the range of frequency by TSL record id.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_SCAN_IDX_BY_FREQ           - Get the current scan index by frequency.
 *  @li@c  SCAN_MAPS_ISDB_CMD_IS_CUSTOMIZED_SORTING          - Check it support customized sorting or not.
 *  @li@c  SCAN_MAPS_ISDB_CMD_GET_CUSTOMIZED_SORTING         - Get the sorting rule with current country.
 */
/*------------------------------------------------------------------*/
typedef enum {
    SCAN_MAPS_ISDB_CMD_GET_FREQ = 0,
    SCAN_MAPS_ISDB_CMD_GET_BW,
    SCAN_MAPS_ISDB_CMD_GET_FIRST_SCAN_IDX,
    SCAN_MAPS_ISDB_CMD_GET_LAST_SCAN_IDX,
    SCAN_MAPS_ISDB_CMD_GET_NEXT_SCAN_IDX,
    SCAN_MAPS_ISDB_CMD_GET_PREV_SCAN_IDX,
    SCAN_MAPS_ISDB_CMD_GET_FIRST_UHF_SCAN_IDX,       /* For ISDB */ /* dyin: 081029 */
    SCAN_MAPS_ISDB_CMD_GET_RF_CHANNEL,
    SCAN_MAPS_ISDB_CMD_GET_COUNTRY_CODE_BY_IDX,
    SCAN_MAPS_ISDB_CMD_GET_TSL_REC_ID,               /* input: ui1_scan_idx; output: ui2_tsl_rec_id */
    SCAN_MAPS_ISDB_CMD_GET_TSL_REC_ID_BY_FREQ,       /* input: ui4_freq; output: ui2_tsl_rec_id */
    SCAN_MAPS_ISDB_CMD_GET_FREQ_RANGE_BY_TSL_REC_ID, /* input: ui2_tsl_rec_id; output: SB_DVB_TSL_REC_ID_FREQ_RANGE_T */
    SCAN_MAPS_ISDB_CMD_GET_SCAN_IDX_BY_FREQ,         /* input: ui4_freq; output: ui1_scan_idx */
    SCAN_MAPS_ISDB_CMD_IS_CUSTOMIZED_SORTING,
    SCAN_MAPS_ISDB_CMD_GET_CUSTOMIZED_SORTING,
}   SCAN_MAPS_ISDB_CMD_T;

/*------------------------------------------------------------------*/
/*! @brief The callback function for getting customized information from ISDB scan map.
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
typedef INT32 (*x_sb_isdb_scan_maps_fct) 
(
    SCAN_MAPS_ISDB_CMD_T        t_cmd,
    ISO_3166_COUNT_T            t_country_code,
    VOID*                       pv_input,
    VOID*                       pv_output,
    SIZE_T*                     pz_output_len
);


/* This data type contains different configuration flags for the 
   scan process. */
typedef UINT32                  SB_ISDB_CONFIG_T;    /**<    Define the different configuration flags for the ISDB scan process. */

/* To ignore the analog channels or not while sorting the ISDB channels.
   Note that to set this flag might cause the channel number collision between
   the digital and analog channels */
#define SB_ISDB_CONFIG_IGNORE_ANALOG_CH_ON_SORTING       (MAKE_BIT_MASK_32(0))    
/**< To ignore the analog channels or not while sorting the ISDB channels.
   Note that to set this flag might cause the channel number collision between
   the digital and analog channels */
/* Do not filter the data broadcast service */
#define SB_ISDB_CONFIG_SUPPORT_DATA_BRDCST_SERVICE       (MAKE_BIT_MASK_32(1))    
/**< Do not filter the data broadcast service */
/* Do not filter the one segment service*/
#define SB_ISDB_CONFIG_SUPPORT_ONE_SEGMENT_SERVICE       (MAKE_BIT_MASK_32(2))    
/**< Do not filter the one segment service */
/* To store the service records to the given temporary SVL */
#define SB_ISDB_CONFIG_UPDATE_TO_TEMP_SVL                (MAKE_BIT_MASK_32(3))    
/**<  To store the service records to the given temporary SVL */
/* Do not remove duplicate channels */
#define SB_ISDB_CONFIG_KEEP_DUPLICATE_CHANNELS           (MAKE_BIT_MASK_32(4))    
/**< Do not remove duplicate channels.*/
/* Support the system management descriptor, if the NIT without this descriptor, 
it will be discarded, if the NIT with this descriptor, it will parse it.*/
#define SB_ISDB_CONFIG_NON_BRDCSTING_SERVICE_SUPPORT     (MAKE_BIT_MASK_32(5))
/**< Support the system management descriptor, if the NIT without this descriptor, 
  *it will be discarded, if the NIT with this descriptor, it will parse it*/
/* If there are the same channel numbers, extend a channel index, and user can used 
 for select service. If you define it, you mustnot be define the SB_ISDB_CONFIG_SUPPORT_DATA_BRDCST_SERVICE.*/
#define SB_ISDB_CONFIG_EXTEND_CH_IDX_SUPPORT     (MAKE_BIT_MASK_32(6))
/**< If there are the same channel numbers, extend a channel index, and user can used 
  * for select service. If you define it, you mustnot be define the SB_ISDB_CONFIG_SUPPORT_DATA_BRDCST_SERVICE.*/

/* If the service in SDT loop but not in NIT service list descriptor, if you define it, you can get this services.*/
#define SB_ISDB_CONFIG_SDT_ONLY_SERVICE_SUPPORT     (MAKE_BIT_MASK_32(7))
/**< If the service in SDT loop but not in NIT service list descriptor, if you define it, you can get this services.*/


/*
 *  The structure SB_ISDB_INIT_DATA_T is used as the argument of the API x_sb_dvb_eng_init()
 *  when initializing an SVL Builder with DVB engine.
 *
 *  t_eng_cfg_flag                  Contains the engine configuration flag. The options allowed
 *                                  are SB_ISDB_CONFIG_IGNORE_ANALOG_CH_ON_SORTING,
 *                                  SB_ISDB_CONFIG_START_CH_NUM_FOR_NON_LCN_CH..
 *
 *  pf_scan_map                     Contains the scan map callback function.
 */
/*------------------------------------------------------------------*/
/*! @struct SB_ISDB_INIT_DATA_T
 *  @brief It is used as the argument of the API x_sb_dvb_eng_init() when initializing an SVL Builder with DVB engine.
 *  @code
 *  typedef struct _SB_ISDB_INIT_DATA_T
 *  {
 *      SB_ISDB_CONFIG_T                t_eng_cfg_flag;
 *      x_sb_isdb_scan_maps_fct         pf_scan_maps;
 *  }   SB_ISDB_INIT_DATA_T;
 *  @endcode
 *  @li@c  t_eng_cfg_flag                                    - Contains the ISDB engine configuration flag.
 *  @li@c  pf_scan_maps                                      - Contains the scan map callback function.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_ISDB_INIT_DATA_T
{
    SB_ISDB_CONFIG_T                t_eng_cfg_flag;
    x_sb_isdb_scan_maps_fct         pf_scan_maps;
}   SB_ISDB_INIT_DATA_T;


/*
 *  The structure SB_ISDB_OPEN_DATA_T is used as the argument
 *  pv_open_data of the API c_sb_open_builder()/x_sb_open_builder()
 *  when opening an SVL Builder with ISDB engine.
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
/*! @struct SB_ISDB_OPEN_DATA_T
 *  @brief It is used as the argument pv_open_data of the API c_sb_open_builder()/x_sb_open_builder() when opening an SVL Builder with ISDB engine.
 *  @code
 *  typedef struct _SB_ISDB_OPEN_DATA_T
 *  {
 *      UINT16                          ui2_svl_id;
 *      CHAR*                           ps_svl_name;
 *      CHAR*                           ps_file_name;
 *      CHAR*                           ps_tuner_name;    
 *      BOOL                            b_use_orig;
 *  }   SB_ISDB_OPEN_DATA_T;
 *  @endcode
 *  @li@c  ui2_svl_id                                        - The id of SVL to be loaded.
 *  @li@c  ps_svl_name                                       - The name of SVL to be loaded.
 *  @li@c  ps_file_name                                      - The file name from which the SVL/TSL will be loaded.
 *  @li@c  ps_tuner_name                                     - The name of tuner to be connectted.
 *  @li@c  b_use_orig                                        - A flag. When set to true, use the existed SVL, instead of reloading it
 */
/*------------------------------------------------------------------*/
typedef struct _SB_ISDB_OPEN_DATA_T
{
    UINT16                          ui2_svl_id;
    CHAR*                           ps_svl_name;
    CHAR*                           ps_file_name;
    CHAR*                           ps_tuner_name;    
    BOOL                            b_use_orig;
}   SB_ISDB_OPEN_DATA_T;


/* ISDB scan type */
/*------------------------------------------------------------------*/
/*! @enum SB_ISDB_SCAN_TYPE_T
 *  @brief It contains different scan types that are used to initialize scan parameters.
 *  @code
 *  typedef enum
 *  {
 *      SB_ISDB_SCAN_TYPE_UNKNOWN = 0,
 *      SB_ISDB_SCAN_TYPE_FULL_MODE,
 *      SB_ISDB_SCAN_TYPE_UHF_MODE,
 *      SB_ISDB_SCAN_TYPE_QUICK_MODE,
 *      SB_ISDB_SCAN_TYPE_ADD_ON_FULL_MODE,
 *      SB_ISDB_SCAN_TYPE_ADD_ON_UHF_MODE,
 *      SB_ISDB_SCAN_TYPE_SINGLE_RF_CHANNEL,
 *      SB_ISDB_SCAN_TYPE_RANGE_RF_CHANNEL,    
 *      SB_ISDB_SCAN_TYPE_NUM 
 *  }   SB_ISDB_SCAN_TYPE_T;
 *  @endcode
 *  @li@c  SB_ISDB_SCAN_TYPE_UNKNOWN                         - Unknown.
 *  @li@c  SB_ISDB_SCAN_TYPE_FULL_MODE                       - Scan all frequency of all bands. It means from first scan index to last.
 *  @li@c  SB_ISDB_SCAN_TYPE_UHF_MODE                        - Scan all frequency in UHF. It means from first scan index to last.
 *  @li@c  SB_ISDB_SCAN_TYPE_QUICK_MODE                      - Just scan the exist and not fake channels in TSL .
 *  @li@c  SB_ISDB_SCAN_TYPE_ADD_ON_FULL_MODE                - Update scan type in all bands. Scan the PTCs which are fake or not found in TSL.
 *  @li@c  SB_ISDB_SCAN_TYPE_ADD_ON_UHF_MODE                 - Update scan type in UHF band. Scan the PTCs which are fake or not found in TSL.
 *  @li@c  SB_ISDB_SCAN_TYPE_SINGLE_RF_CHANNEL               - Scan one specified frequency by the scan index.
 *  @li@c  SB_ISDB_SCAN_TYPE_RANGE_RF_CHANNEL                - Scan the range of start scan index to end.
 *  @li@c  SB_ISDB_SCAN_TYPE_NUM                             - The number of scan type.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_ISDB_SCAN_TYPE_UNKNOWN = 0,
    SB_ISDB_SCAN_TYPE_FULL_MODE,
    SB_ISDB_SCAN_TYPE_UHF_MODE,
    SB_ISDB_SCAN_TYPE_QUICK_MODE,
    SB_ISDB_SCAN_TYPE_ADD_ON_FULL_MODE,
    SB_ISDB_SCAN_TYPE_ADD_ON_UHF_MODE,
    SB_ISDB_SCAN_TYPE_SINGLE_RF_CHANNEL,
    SB_ISDB_SCAN_TYPE_RANGE_RF_CHANNEL,    
    SB_ISDB_SCAN_TYPE_NUM /* number of scan types */
}   SB_ISDB_SCAN_TYPE_T;


/* ISDB scan info */
typedef union 
{
/*------------------------------------------------------------------*/
/*! @struct t_range
 *  @brief The sub information for the range of scan index in ISDB scan process.
 *  @code
 *      struct {
 *          UINT16              ui2_start_scan_idx;
 *          UINT16              ui2_end_scan_idx;
 *      } t_range;
 *  @endcode
 *  @li@c  ui2_start_scan_idx                                - The start index in this scan process.
 *  @li@c  ui2_end_scan_idx                                  - The end index in this scan process.
 */
/*------------------------------------------------------------------*/
    struct 
    {
        UINT16                      ui2_start_scan_idx;
        UINT16                      ui2_end_scan_idx;
    }   t_range;


/*------------------------------------------------------------------*/
/*! @struct t_single
 *  @brief The sub information for the single scan index in ISDB scan process.
 *  @code
 *      struct {
 *          UINT16              ui2_scan_idx;
 *      } t_single;
 *  @endcode
 *  @li@c  ui2_scan_idx                                      - The current single scan index.
 */
/*------------------------------------------------------------------*/
    struct 
    {
        UINT16                      ui2_scan_idx;
    }   t_single;
}   SB_ISDB_SCAN_INFO_T;


/*
 *  The structure SB_ISDB_SCAN_DATA_T is used as the argument 
 *  pv_scan_data of the API x_sb_start_scan() when ISDB scaning 
 *  a media.
 *
 *  pt_country_code                 Contains an ISO 3166 3-character country code.
 *
 *  e_scan_type                     Specifies the way to scan.
 *
 *  t_scan_info                     Contains the required scan information. 
 *
 *  t_eng_cfg_flag                  Contains the engine configuration flag. The option allowed is 
 *                                  SB_ISDB_CONFIG_UPDATE_TO_TEMP_SVL.
 *
 *  ui2_temp_svl_id                 Contains the id of the temporary SVL to be updated. This
 *                                  argument is used only when the SB_ISDB_CONFIG_UPDATE_TO_TEMP_SVL
 *                                  is set. This list must exist and be an empty one.
 */
/*------------------------------------------------------------------*/
/*! @struct SB_ISDB_SCAN_DATA_T
 *  @brief It is used as the argument pv_scan_data of the API x_sb_start_scan() when ISDB scaning 
 *  @code
 *  typedef struct _SB_ISDB_SCAN_DATA_T
 *  {
 *      ISO_3166_COUNT_T                t_country_code;
 *      SB_ISDB_SCAN_TYPE_T             e_scan_type;
 *      SB_ISDB_SCAN_INFO_T             t_scan_info;
 *      SB_ISDB_CONFIG_T                t_eng_cfg_flag;
 *      UINT16                          ui2_temp_svl_id;
 *  }   SB_ISDB_SCAN_DATA_T;
 *  @endcode
 *  @li@c  t_country_code                                    - Contains an ISO 3166 3-character country code.
 *  @li@c  e_scan_type                                       - Specifies the way to scan.
 *  @li@c  t_scan_info                                       - Contains the required scan information.
 *  @li@c  t_eng_cfg_flag                                    - Contains the engine configuration flag. 
 *  @li@c  ui2_temp_svl_id                                   - Contains the id of the temporary SVL to be updated.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_ISDB_SCAN_DATA_T
{
    ISO_3166_COUNT_T                t_country_code;
    SB_ISDB_SCAN_TYPE_T             e_scan_type;
    SB_ISDB_SCAN_INFO_T             t_scan_info;
    SB_ISDB_CONFIG_T                t_eng_cfg_flag;
    UINT16                          ui2_temp_svl_id;
}   SB_ISDB_SCAN_DATA_T;


/* Macros for ISDB channel number */
/* These macros are designed to conform to the ATSC ones so that using the ATSC macros can
   also retrieve the channel numbers */
#define SB_NO_ISDB_START_CHANNEL_ID     ((UINT32)899)    /**<  Define the start channel number of non ISDB channel.*/

#define SB_ISDB_MAX_CHANNEL_INDEX       ((UINT32) 0xff)    /**< Define the max channel index in ISDB channel.*/

/*
    ---------------------------------------------------------------------------------
     remote_ctrl_key_id(8bit)  | service type(2bit) | service ID(3bit) |  .....
    ---------------------------------------------------------------------------------
    
    ------------------------------------------------------
     .... | repeat channel index(4bit) | scan_idx(8bit)  
    ------------------------------------------------------
*/
#define SB_ISDB_GEN_CHANNEL_ID(remote_key, minor_number, channel_idx ) \
    ((((UINT32) (remote_key) & 0xff) << 18)  |    \
      (((UINT32) (minor_number) & 0xff) << 8)  |   \
      ((UINT32) (channel_idx)&0xff))    /**< Define the generated ISDB channel ID with remote control key ID. */

#define SB_ISDB_GET_REMOTE_KEY_ID(_ch_id) \
    ((UINT8)(((UINT32)(_ch_id) >> 18) & 0xff))    /**<  Define the process of getting remote control key ID by channel ID*/


#define SB_ISDB_GET_MINOR_NUMBER(_ch_id) \
    ((UINT8)((((UINT32)(_ch_id) >> 8) & 0xff)))    /**<  Define the process of getting minor number by channel ID */

#define SB_ISDB_GET_SERVICE_TYPE(_ch_id) \
    ((UINT8)((SB_ISDB_GET_MINOR_NUMBER(_ch_id)/10)&0x03))    /**< Define the process of getting ISDB service type by channel ID */

#define SB_ISDB_GET_SERVICE_ID(_ch_id) \
    ((UINT8)(SB_ISDB_GET_MINOR_NUMBER(_ch_id)%10))    /**<  Define the process of getting ISDB service ID by channel ID      */

#define SB_ISDB_GEN_MINOR_NUMBER(service_type, service_id) \
    ((UINT8)(service_type*10+service_id))    /**< Define the generated ISDB channel ID without remote control key ID. */

#define SB_ISDB_GET_CHANNEL_INDEX(_ch_id) \
    ((UINT8)((UINT32)(_ch_id) & 0xff))    /**< Define the process of getting ISDB service index by channel ID. */

#define SB_ISDB_GET_CHANNEL_NUM(_ch_id) \
    ((SB_ISDB_GET_SERVICE_ID(_ch_id)) +        \
     (SB_ISDB_GET_REMOTE_KEY_ID(_ch_id))*10 +  \
     (SB_ISDB_GET_SERVICE_TYPE(_ch_id))*200 )    /**< Define the process of getting ISDB channel number by channel ID.  */

#define SB_ISDB_IS_HAVE_INDEX(_ch_id)                   \
    (((UINT8) (((UINT32)(_ch_id)) & SB_ISDB_MAX_CHANNEL_INDEX)) ? TRUE : FALSE)    /**< Check if it has channel index. */
#if 0
#define SB_ISDB_GEN_CHANNEL_ID(major, index)            \
    ((((UINT32) (major) & 0x3fff) << 18) |              \
    (((UINT32) (index) & SB_ISDB_MAX_CHANNEL_INDEX)))    /**<        */
    
#define SB_ISDB_GET_CHANNEL_NUM(_ch_id)                 \
    ((UINT16)(((UINT32)(_ch_id) >> 18) & 0x3fff))    /**<        */

#define SB_ISDB_GET_CHANNEL_INDEX(_ch_id)               \
    ((UINT8) (((UINT32)_ch_id) & SB_ISDB_MAX_CHANNEL_INDEX))    /**<        */

#define SB_ISDB_IS_HAVE_INDEX(_ch_id)                   \
    (((UINT8) (((UINT32)(_ch_id)) & SB_ISDB_MAX_CHANNEL_INDEX)) ? TRUE : FALSE)    /**<        */

/*Macro for ISDB channel sort*/
#define SB_ISDB_GEN_SORT_KEY(_key, _ch_num)             \
    ((((UINT32)(_key) &0x3fff) << 4)|((UINT32)(_ch_num)))    /**<        */
     
#define SB_ISDB_GET_KEY(_sort_key)                      \
    ((UINT16)(((UINT32)(_sort_key) >> 4) & 0x3fff))    /**<        */

#endif
    
#endif /* _U_SB_ISDB_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                              /* end of groupMW_SVL_BLDR_SB_ISDB_ENG */
/*----------------------------------------------------------------------------*/

