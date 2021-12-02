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
/*! @file u_sb_dvb_eng.h 
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
 
#ifndef _U_SB_DVB_ENG_H_
#define _U_SB_DVB_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_sb.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVL_BLDR_SB_DVB_ENG SVL Builder DVB engine
 *
 *  @ingroup groupMW_SVL_BLDR
 *  @brief The DVB-T engine module is used to scan all digital channel in DVB-T spec.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could create scan engine
 *   or start scan from the exported API of DVB-T svl builder.
 *
 *  @see groupMW_SVL_BLDR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define NZL_DYNAMIC_MULTIPLEX_RECONFIG

#define FIN_DISABLE_UPDATE_IN_MANUAL_SCAN
#define RIKS_TV_SIGNAL_LOSS_IMPL  1
#define DVB_ENG_NAME                "sb_dvb"    /**< Define the DVB build engine name */
#define DVB_DEFAULT_TUNER           "CT_DVB"    /**< Define the default tuner in DVB build engine */
                                    
#if 0
#define SB_VNET_ALL                 ((UINT32)   (0x00000001))    /**<        */
#define SB_VNET_ACTIVE              MAKE_BIT_MASK_32 (1)    /**<        */
#define SB_VNET_EPG                 MAKE_BIT_MASK_32 (2)    /**<        */
#define SB_VNET_VISIBLE             MAKE_BIT_MASK_32 (3)    /**<        */
#define SB_VNET_FAVORITE1           MAKE_BIT_MASK_32 (4)    /**<        */
#define SB_VNET_FAVORITE2           MAKE_BIT_MASK_32 (5)    /**<        */
#define SB_VNET_FAVORITE3           MAKE_BIT_MASK_32 (6)    /**<        */
#define SB_VNET_FAVORITE4           MAKE_BIT_MASK_32 (7)    /**<        */
#define SB_VNET_BLOCKED             MAKE_BIT_MASK_32 (8)    /**<        */
#endif


#define SB_DVB_MAX_REGION_NAME_LEN  32 /*the max value is defined in d-book6.2 p.130*/
#define SB_DVB_MAX_REGION_DEPTH     3  /*there are only three levels so far*/

typedef struct _SB_DVB_REGION_CODE_T
{
    UINT8 ui1_level;     /*1: primary region, 2: secondary region, 3: tertiary region*/
    UINT16 aui2_code[SB_DVB_MAX_REGION_DEPTH];  /*code, (pri_code, sec_code, ter_code)*/
}SB_DVB_REGION_CODE_T;

typedef struct _SB_DVB_REGION_T{
    SB_DVB_REGION_CODE_T t_region_code;
    CHAR ac_name[SB_DVB_MAX_REGION_NAME_LEN + 1]; /* region name */
}SB_DVB_REGION_T;

/* The TSL record id of the low-priority channels in the hierarchical mode */
#define SB_MAKE_HIR_LOW_PRIO_TSL_REC_ID(x)      ((UINT16) ((x) + 150))    /**< Define the TSL record id of the low-priority channels in the hierarchical mode. */
#define SB_MAKE_NON_HIR_TSL_REC_ID(x)           (((UINT16) (x) >= (UINT16) 150)? ((UINT16) (x) - (UINT16) 150): (UINT16) (x))    /**< Define the TSL record id in non-hierarchical mode */


/* Command to get the scan map and info */
/*------------------------------------------------------------------*/
/*! @enum SCAN_MAPS_CMD_T
 *  @brief Command to get the scan map and info.
 *  @code
 *  typedef enum {
 *      SCAN_MAPS_CMD_GET_FREQ = 0,
 *      SCAN_MAPS_CMD_GET_BW,
 *      SCAN_MAPS_CMD_GET_FIRST_SCAN_IDX,
 *      SCAN_MAPS_CMD_GET_LAST_SCAN_IDX,
 *      SCAN_MAPS_CMD_GET_NEXT_SCAN_IDX,
 *      SCAN_MAPS_CMD_GET_PREV_SCAN_IDX,
 *      SCAN_MAPS_CMD_GET_RF_CHANNEL,
 *      SCAN_MAPS_CMD_GET_COUNTRY_CODE_BY_IDX,
 *      SCAN_MAPS_CMD_GET_TSL_REC_ID,               
 *      SCAN_MAPS_CMD_GET_TSL_REC_ID_BY_FREQ,       
 *      SCAN_MAPS_CMD_GET_FREQ_RANGE_BY_TSL_REC_ID, 
 *      SCAN_MAPS_CMD_GET_SCAN_IDX_BY_FREQ,         
 *      SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING,
 *      SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING,
 *      SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY,
 *      SCAN_MAPS_CMD_IS_ONID_OF_COUNTRY,
 *      SCAN_MAPS_CMD_GET_OTHER_COUNTRY_ONID,
 *      SCAN_MAPS_CMD_DETERMINE_PRIVATE_DATA_SPECIFIER,
 *      SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME,
 *      SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME,
 *      SCAN_MAPS_CMD_IS_SERVICE_REMOVAL_MUST_BE_CONFIRMED_COUNTRY,
 *      SCAN_MAPS_CMD_IS_INSTALL_TEMP_NW_SVC_COUNTRY,   
 *      SCAN_MAPS_CMD_IS_DVB_LIGHT_COUNTRY
 *  }   SCAN_MAPS_CMD_T;
 *  @endcode
 *  @li@c  SCAN_MAPS_CMD_GET_FREQ                            - Get frequency.
 *  @li@c  SCAN_MAPS_CMD_GET_BW                              - Get bandwidth.
 *  @li@c  SCAN_MAPS_CMD_GET_FIRST_SCAN_IDX                  - Get the first scan index.
 *  @li@c  SCAN_MAPS_CMD_GET_LAST_SCAN_IDX                   - Get the last scan index.
 *  @li@c  SCAN_MAPS_CMD_GET_NEXT_SCAN_IDX                   - Get the next scan index.
 *  @li@c  SCAN_MAPS_CMD_GET_PREV_SCAN_IDX                   - Get the previous scan index.
 *  @li@c  SCAN_MAPS_CMD_GET_RF_CHANNEL                      - Get the RF channel.
 *  @li@c  SCAN_MAPS_CMD_GET_COUNTRY_CODE_BY_IDX             - Get the country code by index.
 *  @li@c  SCAN_MAPS_CMD_GET_TSL_REC_ID                      - Get the TSL record id by scan index.
 *  @li@c  SCAN_MAPS_CMD_GET_TSL_REC_ID_BY_FREQ              - Get the TSL record id by frequency.
 *  @li@c  SCAN_MAPS_CMD_GET_FREQ_RANGE_BY_TSL_REC_ID        - Get the range of frequency by TSL record id.
 *  @li@c  SCAN_MAPS_CMD_GET_SCAN_IDX_BY_FREQ                - Get the current scan index by frequency.
 *  @li@c  SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING               - Check it support customized sorting or not.
 *  @li@c  SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING              - Get the sorting rule with current country.
 *  @li@c  SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY                  - Check it support LCN or not by current country.
 *  @li@c  SCAN_MAPS_CMD_IS_ONID_OF_COUNTRY                  - Check the current original network id belongs to current country or not.
 *  @li@c  SCAN_MAPS_CMD_GET_OTHER_COUNTRY_ONID              - Get the original network id of other country.
 *  @li@c  SCAN_MAPS_CMD_DETERMINE_PRIVATE_DATA_SPECIFIER    - Get the determine private data specifier information by current country and ONID.
 *  @li@c  SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME       - Check it support customized sorting by name or not.
 *  @li@c  SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME      - Get the rule of sorting by name with current country.
 *  @li@c  SCAN_MAPS_CMD_IS_SERVICE_REMOVAL_MUST_BE_CONFIRMED_COUNTRY    - Check it support removing service must be confirmed or not.
 *  @li@c  SCAN_MAPS_CMD_IS_INSTALL_TEMP_NW_SVC_COUNTRY      - Check current country support temp network service or not.
 *  @li@c  SCAN_MAPS_CMD_IS_DVB_LIGHT_COUNTRY                - Check current country is light country or not.
 *  @li@c  SCAN_MAPS_CMD_IS_THREE_ID_LOCATOR                 - Check current country is three ID(ONID,TSID and ServiceID) for only one channel.
 */
/*------------------------------------------------------------------*/
typedef enum {
    SCAN_MAPS_CMD_GET_FREQ = 0,
    SCAN_MAPS_CMD_GET_BW,
    SCAN_MAPS_CMD_GET_FIRST_SCAN_IDX,
    SCAN_MAPS_CMD_GET_LAST_SCAN_IDX,
    SCAN_MAPS_CMD_GET_NEXT_SCAN_IDX,
    SCAN_MAPS_CMD_GET_PREV_SCAN_IDX,
    SCAN_MAPS_CMD_GET_RF_CHANNEL,
    SCAN_MAPS_CMD_GET_COUNTRY_CODE_BY_IDX,
    SCAN_MAPS_CMD_GET_TSL_REC_ID,               /* input: ui1_scan_idx; output: ui2_tsl_rec_id */
    SCAN_MAPS_CMD_GET_TSL_REC_ID_BY_FREQ,       /* input: ui4_freq; output: ui2_tsl_rec_id */
    SCAN_MAPS_CMD_GET_FREQ_RANGE_BY_TSL_REC_ID, /* input: ui2_tsl_rec_id; output: SB_DVB_TSL_REC_ID_FREQ_RANGE_T */
    SCAN_MAPS_CMD_GET_SCAN_IDX_BY_FREQ,         /* input: ui4_freq; output: ui1_scan_idx */
    SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING,
    SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING,
    SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY,
    SCAN_MAPS_CMD_IS_ONID_OF_COUNTRY,
    SCAN_MAPS_CMD_GET_OTHER_COUNTRY_ONID,
    SCAN_MAPS_CMD_DETERMINE_PRIVATE_DATA_SPECIFIER,
    SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME,
    SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME,
    SCAN_MAPS_CMD_IS_SERVICE_REMOVAL_MUST_BE_CONFIRMED_COUNTRY,
    SCAN_MAPS_CMD_IS_INSTALL_TEMP_NW_SVC_COUNTRY,   /* Install services of temporary network in this country */
    SCAN_MAPS_CMD_IS_DVB_LIGHT_COUNTRY,
    SCAN_MAPS_CMD_IS_THREE_ID_LOCATOR,
    SCAM_MAPS_CMD_IS_SORT_WITH_FAV_NET,
    SCAN_MAPS_CMD_GET_SCAN_IDX_BY_RF_CHANNEL,
    SCAN_MAPS_CMD_GET_SCAN_IDX_INFO_BY_RF_CHANNEL,
    SCAN_MAPS_CMD_GET_NEXT_RF_CHANNEL,
    SCAN_MAPS_CMD_GET_PREV_RF_CHANNEL,
    SCAN_MAPS_CMD_GET_CH_ID_BY_SCAN_IDX,    
    SCAN_MAPS_CMD_GET_SCAN_IDX_BY_FREQ_BW,
    SCAN_MAPS_CMD_GET_FIRST_RF_CHANNEL_BY_TYPE,
    SCAM_MAPS_CMD_IS_NEED_CHECK_SBAND_I
}   SCAN_MAPS_CMD_T;


#define LCN_CHANNEL_LIST_NAME_LENGTH       24
typedef struct _SB_DVB_CH_LST_LCN_INFO
{    
    CHAR            ac_channel_list_name[LCN_CHANNEL_LIST_NAME_LENGTH];
    UINT8           ui1_channel_list_id;    
} SB_DVB_CH_LST_LCN_INFO;

#define RF_NAME_LENGTH 4/*3*/

#define SB_DVB_SCAN_INDEX_COUNT_RF_CH  5
typedef struct _SB_DVB_SCAN_INDEX_INFO
{
    UINT8                   ui1_count;
    UINT8                   a_scan_idx[SB_DVB_SCAN_INDEX_COUNT_RF_CH];
} SB_DVB_SCAN_INDEX_INFO;


typedef struct _SB_DVB_FREQ_BW_T
{
    UINT32                      ui4_freq;
    UINT8                       e_bandwidth;
} SB_DVB_FREQ_BW_T;

/*------------------------------------------------------------------*/
/*! @brief The callback function for getting customized information from DVB scan map.
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


typedef INT32 (*x_sb_dvb_scan_maps_fct) (SCAN_MAPS_CMD_T     t_cmd,
                                         ISO_3166_COUNT_T    t_country_code,
                                         VOID*               pv_input,
                                         VOID*               pv_output,
                                         SIZE_T*             pz_output_len);

typedef INT32 (*x_sb_dvb_scan_maps_fct_prv) (SCAN_MAPS_CMD_T     t_cmd,
                                         ISO_3166_COUNT_T    t_country_code,
                                         VOID*               pv_input,
                                         VOID*               pv_output,
                                         SIZE_T*             pz_output_len,
                                         VOID*               pv_nfy_tag);



/* This data type contains different configuration flags for the 
   scan process. */
typedef UINT32      SB_DVB_CONFIG_T;    /**< Define the different configuration flags for the DVB scan process*/

/* To ignore the analog channels or not while sorting the DVB channels.
   Note that to set this flag might cause the channel number collision between
   the digital and analog channels */
#define SB_DVB_CONFIG_IGNORE_ANALOG_CH_ON_SORTING       (MAKE_BIT_MASK_32(0))    
/**< To ignore the analog channels or not while sorting the DVB channels.
   Note that to set this flag might cause the channel number collision between
   the digital and analog channels */
/* Install the MHEG-5 services as visible ones */
#define SB_DVB_CONFIG_SUPPORT_MHEG5_SERVICES            (MAKE_BIT_MASK_32(1))    
/**<  Install the MHEG-5 services as visible ones */
/* To customize the the start of the channel numbers to be assigned to the non-LCN 
   channels while LCN need be applied. The default value is 1000. */
#define SB_DVB_CONFIG_START_CH_NUM_FOR_NON_LCN_CH       (MAKE_BIT_MASK_32(2))    
/**< To customize the the start of the channel numbers to be assigned to the non-LCN 
   channels while LCN need be applied. The default value is 1000. */
/* To always apply the LCN. By default, the engine would allocate the services to 
   channel numbers starting from 1 if all of the services found at this time of scan
   are not attached to a LCN; and henceforth, all the LCN found is ignored. With 
   this flag set, the engine shall always allocate the non-LCN services to the 
   given (or default) start channel numbers for non-LCN services. For example, at
   a country where there is no LCN broadcasted, all the digital services are allocated
   to 800, 801, 802 ...; there will be no services at 1, 2, 3 ... */
#define SB_DVB_CONFIG_ALWAYS_APPLY_LCN                  (MAKE_BIT_MASK_32(3))    
/**< To always apply the LCN. By default, the engine would allocate the services to 
   channel numbers starting from 1 if all of the services found at this time of scan
   are not attached to a LCN; and henceforth, all the LCN found is ignored. With 
   this flag set, the engine shall always allocate the non-LCN services to the 
   given (or default) start channel numbers for non-LCN services. For example, at
   a country where there is no LCN broadcasted, all the digital services are allocated
   to 800, 801, 802 ...; there will be no services at 1, 2, 3 ...  */
/* To store the service records to the given temporary SVL */
#define SB_DVB_CONFIG_UPDATE_TO_TEMP_SVL                (MAKE_BIT_MASK_32(4))    
/**<  To store the service records to the given temporary SVL */
/* Do not remove duplicate channels */
#define SB_DVB_CONFIG_KEEP_DUPLICATE_CHANNELS           (MAKE_BIT_MASK_32(5))    
/**< Do not remove duplicate channels */
/* To customize the timeout value of loading SDT and NIT */
#define SB_DVB_CONFIG_SDT_NIT_TIMEOUT                   (MAKE_BIT_MASK_32(6))    
/**< To customize the timeout value of loading SDT and NIT */
/* Install the MHP services as visible ones */
#define SB_DVB_CONFIG_SUPPORT_MHP_SERVICES              (MAKE_BIT_MASK_32(7))    
/**< Install the MHP services as visible ones */
/* To ignore the LCN that is of an original network ID that does not belong to the
   given country */
#define SB_DVB_CONFIG_IGNORE_LCN_OF_UNMATCHED_COUNTRY   (MAKE_BIT_MASK_32(8))    
/**< To ignore the LCN that is of an original network ID that does not belong to the
   given country */
/* To ignore the LCN of Finland radio services */
#define SB_DVB_CONFIG_IGNORE_LCN_OF_FINLAND_RADIO       (MAKE_BIT_MASK_32(9))    
/**< To ignore the LCN of Finland radio services */
/* To reserve customizied number of channel numbers before allocating for the 
   non-LCN channels */
#define SB_DVB_CONFIG_RESERVE_CH_NUM_BEFORE_NON_LCN_CH  (MAKE_BIT_MASK_32(10))    
/**< To reserve customizied number of channel numbers before allocating for the 
   non-LCN channels  */
/* Do not support HDTV (H.264 & MPEG-2) */
#define SB_DVB_CONFIG_NOT_SUPPORT_HDTV                  (MAKE_BIT_MASK_32(11))    
/**< Do not support HDTV (H.264 & MPEG-2) */
/* Sort the non-LCN channels by the simple rule - first by country and then service type;
   Otherwise, sort them by (1) country, (2) service type, (3) LCN, (4) service name, (5) service id */
#define SB_DVB_CONFIG_SIMPLE_SORT_FOR_NON_LCN_CH        (MAKE_BIT_MASK_32(12))    
/**< Sort the non-LCN channels by the simple rule - first by country and then service type;
   Otherwise, sort them by (1) country, (2) service type, (3) LCN, (4) service name, (5) service id  */
/* To ignore the LCN of Sweden radio services */
#define SB_DVB_CONFIG_IGNORE_LCN_OF_SWEDEN_RADIO        (MAKE_BIT_MASK_32(13))  
/**< To ignore the LCN of Sweden radio services */
/* To extend channel number to 9999 for nordig countries */
#define SB_DVB_CONFIG_4_DIGITS_FOR_NORDIG_COUNTRIES        (MAKE_BIT_MASK_32(14))    
/**< To extend channel number to 9999 for nordig countries */
/* To sort non-lcn channels by frequency order (low --> high) */
#define SB_DVB_CONFIG_SORTING_BY_FREQ                   (MAKE_BIT_MASK_32(15))    
/**< To sort non-lcn channels by frequency order (low --> high) */
/* To disable data service in scan process */
#define SB_DVB_CONFIG_NOT_INSTALL_DATA_SERVICE                   (MAKE_BIT_MASK_32(16))   
/**< To disable data service in scan process */
/* To disable hierarchical mode demodulation*/
#define SB_DVB_CONFIG_HIER_MODE_DISABLE                 (MAKE_BIT_MASK_32(17))    
/**< To disable hierarchical mode demodulation */
/* To disable force apply HD simulcast LCN in France*/
#define SB_DVB_CONFIG_DISABLE_FORCE_APPLY_HD_SIMULCAST_LCN  (MAKE_BIT_MASK_32(18))    
/**< To disable force apply HD simulcast LCN in France */
/* To apply smaller LCN when only found one HD simulcast channel in France*/
#define SB_DVB_CONFIG_APPLY_SMALLER_FOR_SINGLE_HD_SIMULCAST_LCN    (MAKE_BIT_MASK_32(19))    
/**<  To apply smaller LCN when only found one HD simulcast channel in France */
/*To config minimun channel number for non lcn country*/
#define SB_DVB_CONFIG_MIN_CH_NUM_FOR_NON_LCN_COUNTRY    (MAKE_BIT_MASK_32(20))    
/**< To config minimun channel number for non lcn country */
/* To support scan the non-standard DVBT bitstream */
#define SB_DVB_CONFIG_NON_STANDARD_BITSTREAM             (MAKE_BIT_MASK_32(21))    
/**<  To support scan the non-standard DVBT bitstream */

/* To support TV type and Raido type service in the its channel list in Nordig country, 
 and the SB_DVB_CONFIG_IGNORE_ANALOG_CH_ON_SORTING must be defined before used this config*/
#define SB_DVB_CONFIG_TV_RADIO_SEPARATE                 (MAKE_BIT_MASK_32(22)) 
/**< To support TV type and Raido type service in the its channel list in Nordig country, 
 and the SB_DVB_CONFIG_IGNORE_ANALOG_CH_ON_SORTING must be defined before used this config*/

/* To support customer 1 feature.*/
#define SB_DVB_CONFIG_CUSTOMER_1_SUPPORT                (MAKE_BIT_MASK_32(23)) 
/**< To support customer 1 feature.*/

/* To support DTG HD freeview feature.*/
#define SB_DVB_CONFIG_DTG_HD_FREEVIEW_SUPPORT               (MAKE_BIT_MASK_32(24)) 
/**< To support DTG HD freeview feature.*/

/* To scan DVB without frequency maps.*/
#define SB_DVB_CONFIG_SCAN_WITHOUT_SCAN_MAP               (MAKE_BIT_MASK_32(25)) 
/**< To scan DVB without frequency maps.*/

/* This config is for Antenna Ready, if this is enabled, 
  we will do the Finland Radio channel number from 1000+LCN.*/
#define SB_DVB_CONFIG_FIN_ANTENNA_READY_HD_SUPPORT        (MAKE_BIT_MASK_32(26)) 

/*
 *  The structure SB_DVB_INIT_DATA_T is used as the argument of the API x_sb_dvb_eng_init()
 *  when initializing an SVL Builder with DVB engine.
 *
 *  t_eng_cfg_flag                  Contains the engine configuration flag. The options allowed
 *                                  are SB_DVB_CONFIG_IGNORE_ANALOG_CH_ON_SORTING,
 *                                  SB_DVB_CONFIG_SUPPORT_MHEG5_SERVICES,
 *                                  SB_DVB_CONFIG_SUPPORT_MHP_SERVICES,
 *                                  SB_DVB_CONFIG_START_CH_NUM_FOR_NON_LCN_CH,
 *                                  SB_DVB_CONFIG_ALWAYS_APPLY_LCN,
 *                                  SB_DVB_CONFIG_SDT_NIT_TIMEOUT,
 *                                  SB_DVB_CONFIG_IGNORE_LCN_OF_UNMATCHED_COUNTRY,
 *                                  SB_DVB_CONFIG_RESERVE_CH_NUM_BEFORE_NON_LCN_CH,
 *                                  SB_DVB_CONFIG_NOT_SUPPORT_HDTV,
 *                                  SB_DVB_CONFIG_SIMPLE_SORT_FOR_NON_LCN_CH,
 *                                  SB_DVB_CONFIG_IGNORE_LCN_OF_FINLAND_RADIO.
 *                                  SB_DVB_CONFIG_IGNORE_LCN_OF_SWEDEN_RADIO.
 *
 *  ui2_start_ch_num_for_non_lcn    Contains the start channel number to be assigned to non-LCN
 *                                  channels when the SB_DVB_CONFIG_START_CH_NUM_FOR_NON_LCN_CH 
 *                                  is set. For SB_DVB_CONFIG_RESERVE_CH_NUM_BEFORE_NON_LCN_CH,
 *                                  it is the number of reserved channel numbers.
 *
 *  ui4_sdt_timeout_mseconds        Contains the timeout value for loading the SDT. A value less
 *                                  than 2 seconds is not allowed. This argument is used only 
 *                                  when the SB_DVB_CONFIG_SDT_NIT_TIMEOUT is set.
 *
 *  ui4_nit_timeout_mseconds        Contains the timeout value for loading the NIT. A value less
 *                                  than 10 seconds is not allowed. This argument is used only 
 *                                  when the SB_DVB_CONFIG_SDT_NIT_TIMEOUT is set.
 *
 *  pf_scan_map                     Contains the scan map callback function.
 */
/*------------------------------------------------------------------*/
/*! @struct SB_DVB_INIT_DATA_T
 *  @brief It is used as the argument of the API x_sb_dvb_eng_init() when initializing an SVL Builder with DVB engine.
 *  @code
 *  typedef struct _SB_DVB_INIT_DATA_T
 *  {
 *      SB_DVB_CONFIG_T         t_eng_cfg_flag;
 *      UINT16                  ui2_start_ch_num_for_non_lcn;
 *      UINT32                  ui4_sdt_timeout_mseconds;
 *      UINT32                  ui4_nit_timeout_mseconds;
 *      x_sb_dvb_scan_maps_fct  pf_scan_maps;
 *  } SB_DVB_INIT_DATA_T;
 *  @endcode
 *  @li@c  t_eng_cfg_flag                                    - Contains the engine configuration flag.
 *  @li@c  ui2_start_ch_num_for_non_lcn                      - Contains the start channel number to be assigned to non-LCN channels when the SB_DVB_CONFIG_START_CH_NUM_FOR_NON_LCN_CH is set.
 *  @li@c  ui4_sdt_timeout_mseconds                          - Contains the timeout value for loading the SDT.
 *  @li@c  ui4_nit_timeout_mseconds                          - Contains the timeout value for loading the NIT. 
 *  @li@c  pf_scan_maps                                      - Contains the scan map callback function.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVB_INIT_DATA_T
{
    SB_DVB_CONFIG_T         t_eng_cfg_flag;
    UINT16                  ui2_start_ch_num_for_non_lcn;
    UINT32                  ui4_sdt_timeout_mseconds;
    UINT32                  ui4_nit_timeout_mseconds;
    #ifndef LINUX_TURNKEY_SOLUTION_PHASE2
    x_sb_dvb_scan_maps_fct  pf_scan_maps;
    #endif
} SB_DVB_INIT_DATA_T;


/*
 *  The structure SB_DVB_OPEN_DATA_T is used as the argument
 *  pv_open_data of the API c_sb_open_builder()/x_sb_open_builder()
 *  when opening an SVL Builder with DVB engine.
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
/*! @struct SB_DVB_OPEN_DATA_T
 *  @brief It is used as the argument pv_open_data of the API c_sb_open_builder() when opening an SVL Builder with DVB engine
 *  @code
 *  typedef struct _SB_DVB_OPEN_DATA_T
 *  {
 *      UINT16                  ui2_svl_id;
 *      CHAR*                   ps_svl_name;
 *      CHAR*                   ps_file_name;
 *      CHAR*                   ps_tuner_name;    
 *      BOOL                    b_use_orig;
 *  } SB_DVB_OPEN_DATA_T;
 *  @endcode
 *  @li@c  ui2_svl_id                                        - The id of SVL to be loaded.
 *  @li@c  ps_svl_name                                       - The name of SVL to be loaded.
 *  @li@c  ps_file_name                                      - The file name from which the SVL/TSL will be loaded.
 *  @li@c  ps_tuner_name                                     - The name of tuner to be connectted.
 *  @li@c  b_use_orig                                        - A flag. When set to true, use the existed SVL, instead of reloading it
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVB_OPEN_DATA_T
{
    UINT16                  ui2_svl_id;
    CHAR*                   ps_svl_name;
    CHAR*                   ps_file_name;
    CHAR*                   ps_tuner_name;    
    BOOL                    b_use_orig;
} SB_DVB_OPEN_DATA_T;


/* DVB scan type */
/*------------------------------------------------------------------*/
/*! @enum SB_DVB_SCAN_TYPE_T
 *  @brief It contains different scan types that are used to initialize scan parameters.
 *  @code
 *  typedef enum
 *  {
 *      SB_DVB_SCAN_TYPE_UNKNOWN = 0,
 *      SB_DVB_SCAN_TYPE_FULL_MODE,
 *      SB_DVB_SCAN_TYPE_QUICK_MODE,
 *      SB_DVB_SCAN_TYPE_ADD_ON_MODE,
 *      SB_DVB_SCAN_TYPE_SINGLE_RF_CHANNEL,
 *      SB_DVB_SCAN_TYPE_RANGE_RF_CHANNEL,    
 *      SB_DVB_SCAN_TYPE_MANUAL_FREQ,
 *      SB_DVB_SCAN_TYPE_NUM 
 *  } SB_DVB_SCAN_TYPE_T;
 *  @endcode
 *  @li@c  SB_DVB_SCAN_TYPE_UNKNOWN                          - Unknown.
 *  @li@c  SB_DVB_SCAN_TYPE_FULL_MODE                        - Scan all frequency. It means from first scan index to last.
 *  @li@c  SB_DVB_SCAN_TYPE_QUICK_MODE                       - Just scan the exist and not fake channels in TSL .
 *  @li@c  SB_DVB_SCAN_TYPE_ADD_ON_MODE                      - Update scan type. Scan the PTCs which are fake or not found in TSL.
 *  @li@c  SB_DVB_SCAN_TYPE_SINGLE_RF_CHANNEL                - Scan one specified frequency by the scan index.
 *  @li@c  SB_DVB_SCAN_TYPE_RANGE_RF_CHANNEL                 - Scan the range of start scan index to end. 
 *  @li@c  SB_DVB_SCAN_TYPE_MANUAL_FREQ                      - Scan one specified frequency. 
 *  @li@c  SB_DVB_SCAN_TYPE_NUM                              - The number of scan type.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_DVB_SCAN_TYPE_UNKNOWN = 0,
    SB_DVB_SCAN_TYPE_FULL_MODE,
    SB_DVB_SCAN_TYPE_QUICK_MODE,
    SB_DVB_SCAN_TYPE_ADD_ON_MODE,
    SB_DVB_SCAN_TYPE_SINGLE_RF_CHANNEL,
    SB_DVB_SCAN_TYPE_RANGE_RF_CHANNEL,    
    SB_DVB_SCAN_TYPE_MANUAL_RF_CHANNEL,   
    SB_DVB_SCAN_TYPE_MANUAL_FREQ,
    SB_DVB_SCAN_TYPE_NUM /* number of scan types */
} SB_DVB_SCAN_TYPE_T;


/* DVB scan info */
typedef union {
/*------------------------------------------------------------------*/
/*! @struct t_range
 *  @brief The sub information for the range of scan index in DVB scan process.
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
    struct {
        UINT16              ui2_start_scan_idx;
        UINT16              ui2_end_scan_idx;
    } t_range;

/*------------------------------------------------------------------*/
/*! @struct t_single
 *  @brief The sub information for the single scan index in DVB scan process.
 *  @code
 *      struct {
 *          UINT16              ui2_scan_idx;
 *      } t_single;
 *  @endcode
 *  @li@c  ui2_scan_idx                                      - The current single scan index.
 */
/*------------------------------------------------------------------*/
    struct {
        UINT16              ui2_scan_idx;
    } t_single;

/*------------------------------------------------------------------*/
/*! @struct t_freq
 *  @brief  The sub information for the frequency in scan process.
 *  @code
 *      struct {
 *          UINT32              ui4_freq;
 *      } t_freq;
 *  @endcode
 *  @li@c  ui4_freq                                          -
 */
/*------------------------------------------------------------------*/
    struct {
        UINT32              ui4_freq;
    } t_freq;

/*------------------------------------------------------------------*/
/*! @struct t_rf_channel
 *  @brief  The sub information for the rf channel name in scan process.
 *  @code
 *      struct {
 *          CHAR                   s_rf_name[RF_NAME_LENGTH];
 *      } t_rf_channel;
 *  @endcode
 *  @li@c  t_rf_channel                                          -
 */
/*------------------------------------------------------------------*/
    struct{
        CHAR                   s_rf_name[RF_NAME_LENGTH];
    } t_rf_channel;
}   SB_DVB_SCAN_INFO_T;


/* The structure SB_DVB_SVC_UPDATE_NFY_DATA is used as the argument 
   of the service update notification function. */
/*------------------------------------------------------------------*/
/*! @struct SB_DVB_SVC_UPDATE_NFY_DATA
 *  @brief It is used as the argument of the service update notification function.
 *  @code
 *  typedef struct _SB_DVB_SVC_UPDATE_NFY_DATA
 *  {
 *      UINT16                  ui2_num_added_svc;
 *      UINT16                  ui2_num_deleted_svc;
 *  } SB_DVB_SVC_UPDATE_NFY_DATA;
 *  @endcode
 *  @li@c  ui2_num_added_svc                                 - The count of added service in this progress.
 *  @li@c  ui2_num_deleted_svc                               - The count of deleted service in this progress.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVB_SVC_UPDATE_NFY_DATA
{
    UINT16                  ui2_num_added_svc;
    UINT16                  ui2_num_deleted_svc;
} SB_DVB_SVC_UPDATE_NFY_DATA;

/* The prototype of the service update notification function */
/*------------------------------------------------------------------*/
/*! @brief The prototype of the service update notification function
 *  @param [in] pt_nfy_data The information for added and deleted services.
 *  @param [in] pv_nfy_tag The tag for notifying user.
 *  @param [out] None.
 *  @return
 *  @retval      - None.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_svc_update_nfy_fct) (
        SB_DVB_SVC_UPDATE_NFY_DATA* pt_nfy_data,
        VOID*                       pv_nfy_tag);

/* The prototype of the network modification notification function */
/*------------------------------------------------------------------*/
/*! @brief It is used for notifying user the updating network.
 *  @param [in] pv_nfy_tag The tag for notifying user.
 *  @param [out] None.
 *  @return
 *  @retval      - None.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_nw_change_nfy_fct) (
        VOID*                       pv_nfy_tag);


/*
 *  The structure SB_DVB_SCAN_DATA_T is used as the argument 
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
 *                                  SB_DVB_CONFIG_UPDATE_TO_TEMP_SVL.
 *
 *  ui2_temp_svl_id                 Contains the id of the temporary SVL to be updated. This
 *                                  argument is used only when the SB_DVB_CONFIG_UPDATE_TO_TEMP_SVL
 *                                  is set. This list must exist and be an empty one.
 *
 *  pf_svc_update_nfy               Contains the service update notification function.
 *
 *  pv_svc_update_nfy_tag           Contains the tag for the service update notification function.
 *
 * pf_new_svc_nfy            notify app the information of every new service
 *
 * pv_new_svc_nfy_tag       Contains the taf for the new service notification function
 *
 */
/*------------------------------------------------------------------*/
/*! @struct SB_DVB_SCAN_DATA_T
 *  @brief It is used as the argument pv_scan_data of the API x_sb_start_scan() when scaning a media.
 *  @code
 *  typedef struct _SB_DVB_SCAN_DATA_T
 *  {
 *      ISO_3166_COUNT_T        t_country_code;
 *      SB_DVB_SCAN_TYPE_T      e_scan_type;
 *      SB_DVB_SCAN_INFO_T      t_scan_info;
 *      SB_DVB_CONFIG_T         t_eng_cfg_flag;
 *      UINT16                  ui2_temp_svl_id;
 *      UINT16                  ui2_min_ch_num;
 *      x_sb_svc_update_nfy_fct pf_svc_update_nfy;
 *      VOID*                   pv_svc_update_nfy_tag;
 *      x_sb_new_svc_nfy_fct    pf_new_svc_nfy;
 *      VOID*                   pv_new_svc_nfy_tag;
 *  } SB_DVB_SCAN_DATA_T;
 *  @endcode
 *  @li@c  t_country_code                                    - Contains an ISO 3166 3-character country code.
 *  @li@c  e_scan_type                                       - Specifies the way to scan.
 *  @li@c  t_scan_info                                       - Contains the required scan information.
 *  @li@c  t_eng_cfg_flag                                    - Contains the engine configuration flag.
 *  @li@c  ui2_temp_svl_id                                   - Contains the id of the temporary SVL to be updated.
 *  @li@c  ui2_min_ch_num                                    - Contains the service update notification function.
 *  @li@c  pf_svc_update_nfy                                 - Contains the tag for the service update notification function.
 *  @li@c  pv_svc_update_nfy_tag                             - Contains the tag for the service update notification function.
 *  @li@c  pf_new_svc_nfy                                    - Notify app the information of every new service
 *  @li@c  pv_new_svc_nfy_tag                                - Contains the taf for the new service notification function.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVB_SCAN_DATA_T
{
    ISO_3166_COUNT_T        t_country_code;
    SB_DVB_SCAN_TYPE_T      e_scan_type;
    SB_DVB_SCAN_INFO_T      t_scan_info;
    SB_DVB_CONFIG_T         t_eng_cfg_flag;
    UINT16                  ui2_temp_svl_id;
    UINT16                  ui2_min_ch_num;
    x_sb_svc_update_nfy_fct pf_svc_update_nfy;
    VOID*                   pv_svc_update_nfy_tag;
    x_sb_new_svc_nfy_fct    pf_new_svc_nfy;
    VOID*                   pv_new_svc_nfy_tag;
} SB_DVB_SCAN_DATA_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVB_TSL_REC_ID_FREQ_RANGE_T
 *  @brief The sub information for range of lower and upper bound.
 *  @code
 *  typedef struct _SB_DVB_TSL_REC_ID_FREQ_RANGE
 *  {
 *      UINT32                  ui4_freq_range_lbound;
 *      UINT32                  ui4_freq_range_ubound;
 *  } SB_DVB_TSL_REC_ID_FREQ_RANGE_T;
 *  @endcode
 *  @li@c  ui4_freq_range_lbound                             - The frequency of lower bound.
 *  @li@c  ui4_freq_range_ubound                             - The frequency of upper bound.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVB_TSL_REC_ID_FREQ_RANGE
{
    UINT32                  ui4_freq_range_lbound;
    UINT32                  ui4_freq_range_ubound;
} SB_DVB_TSL_REC_ID_FREQ_RANGE_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVB_ON_TS_SVC_ID_T
 *  @brief The locator of service in DVB system.
 *  @code
 *  typedef struct _SB_DVB_ON_TS_SVC_ID
 *  {
 *      UINT16                  ui2_on_id;
 *      UINT16                  ui2_ts_id;
 *      UINT16                  ui2_svc_id;
 *  } SB_DVB_ON_TS_SVC_ID_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         - The original network id.
 *  @li@c  ui2_ts_id                                         - The transport stream id.
 *  @li@c  ui2_svc_id                                        - The service identify.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVB_ON_TS_SVC_ID
{
    UINT16                  ui2_on_id;
    UINT16                  ui2_ts_id;
    UINT16                  ui2_svc_id;
} SB_DVB_ON_TS_SVC_ID_T;


/*
    SB_DVB_COUNTRY_ONID_T is used to store the original 
    network id of various countries
 */
/*------------------------------------------------------------------*/
/*! @struct SB_DVB_COUNTRY_ONID_T
 *  @brief It is used to store the original network id of various countries
 *  @code
 *  typedef struct {
 *      ISO_3166_COUNT_T        t_country_code;
 *      UINT8                   ui1_num_onid;
 *      UINT16*                 pui2_onid;
 *  } SB_DVB_COUNTRY_ONID_T;
 *  @endcode
 *  @li@c  t_country_code                                    - The country code.
 *  @li@c  ui1_num_onid                                      - The count of ONID.
 *  @li@c  pui2_onid                                         - The buffer for all ONID.
 */
/*------------------------------------------------------------------*/
typedef struct {
    ISO_3166_COUNT_T        t_country_code;
    UINT8                   ui1_num_onid;
    UINT16*                 pui2_onid;
} SB_DVB_COUNTRY_ONID_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVB_ONID_COUNT_INFO_T
 *  @brief The sub informaton for current all ONID.
 *  @code
 *  typedef struct _SB_DVB_ONID_COUNT_INFO_T {
 *      UINT16                  ui2_on_id;
 *      UINT16                  ui2_count;
 *      INT32                   i4_signal_ber;
 *  } SB_DVB_ONID_COUNT_INFO_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         - The original network id.
 *  @li@c  ui2_count                                         - The count of all ONID.
 *  @li@c  i4_signal_ber                                     - The signal ber.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVB_ONID_COUNT_INFO_T {
    UINT16                  ui2_on_id;
    UINT16                  ui2_count;
    INT32                   i4_signal_ber;
} SB_DVB_ONID_COUNT_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVB_GET_OTHER_COUNTRY_ONID_INFO_T
 *  @brief The buffer for current all SB_DVB_ONID_COUNT_INFO_T.
 *  @code
 *  typedef struct _SB_DVB_GET_OTHER_COUNTRY_ONID_INFO_T {
 *      SB_DVB_ONID_COUNT_INFO_T*   pt_onid_info;
 *      UINT16                      ui2_num_onid_info;
 *  } SB_DVB_GET_OTHER_COUNTRY_ONID_INFO_T;
 *  @endcode
 *  @li@c  pt_onid_info                                      - The buffer for current all ONID information.
 *  @li@c  ui2_num_onid_info                                 - The count of all ONID information.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVB_GET_OTHER_COUNTRY_ONID_INFO_T {
    SB_DVB_ONID_COUNT_INFO_T*   pt_onid_info;
    UINT16                      ui2_num_onid_info;
} SB_DVB_GET_OTHER_COUNTRY_ONID_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVB_NW_CHANGE_NFY_T
 *  @brief The structure for the notify information when network updated.
 *  @code
 *  typedef struct _SB_DVB_NW_CHANGE_NFY_T {
 *      x_sb_nw_change_nfy_fct  pf_nw_change_nfy;
 *      VOID*                   pv_nw_change_nfy_tag;
 *  } SB_DVB_NW_CHANGE_NFY_T;
 *  @endcode
 *  @li@c  pf_nw_change_nfy                                  - The callback function for network updated.
 *  @li@c  pv_nw_change_nfy_tag                              - The private tag in notify progress.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVB_NW_CHANGE_NFY_T {
    x_sb_nw_change_nfy_fct  pf_nw_change_nfy;
    VOID*                   pv_nw_change_nfy_tag;
} SB_DVB_NW_CHANGE_NFY_T;

#include "x_lnk_list.h"
typedef struct _REGION_CODE_INFO
{
    DLIST_ENTRY_T(_REGION_CODE_INFO)  t_link;
    UINT8       ui1_level;     /*1: primary region, 2: secondary region, 3: tertiary region*/
    UINT16      aui2_code[SB_DVB_MAX_REGION_DEPTH];  /*code, (pri_code, sec_code, ter_code)*/ 
} REGION_CODE_INFO;
typedef DLIST_T(_REGION_CODE_INFO)     REGION_CODE_INFO_LIST;

/*DGTVi: */
typedef struct _SB_DVB_LCN_CONFLICT_GROUP_T {
    UINT8        ui1_index;
    UINT8        ui1_num_channels;
    UINT16       ui2_lcn;
    BOOL         b_visible_flag;
    BOOL         b_current_country_flag;
}SB_DVB_LCN_CONFLICT_GROUP_T;

typedef struct _SB_DVB_LCN_CONFLICT_CHANNEL_T {
    UINT8         ui1_index;
    UINT8         ui1_service_type;
    INT8          i1_signal_level;
    UINT16        ui2_svl_rec_id;
    CHAR          ac_name[MAX_PROG_NAME_LEN+1];
    REGION_CODE_INFO_LIST t_list;
}SB_DVB_LCN_CONFLICT_CHANNEL_T;


#endif /* _U_SB_DVB_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_SVL_BLDR_SB_DVB_ENG */
/*----------------------------------------------------------------------------*/

