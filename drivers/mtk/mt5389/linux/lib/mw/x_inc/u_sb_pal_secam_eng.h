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
/*! @file u_sb_pal_secam_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         SVL Builder PAL/SECAM engine.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVL_BLDR_SB_PAL_SECAM_ENG PAL/SECAM Engine.
 *  @ingroup groupMW_SVL_BLDR
 *  @brief Provides Analog PAL/SECAM scan function.
 *  %This module is SVL_BLDR PAL/SECAM instance.
 *  @see groupMW_SVL_BLDR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

 
#ifndef _U_SB_PAL_SECAM_ENG_H_
#define _U_SB_PAL_SECAM_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_vbi_fltr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define PAL_SECAM_ENG_NAME          "sb_pal_secam"    /**< Engine name       */


/* This data type contains different configuration flags for the 
   scan process. */
typedef UINT32      SB_PAL_SECAM_CONFIG_T;    /**< SB Pal Secam Engine config type       */
typedef UINT32      SCAN_MAPS_PAL_SECAM_CMD_T;


#define SB_PAL_SECAM_CONFIG_PLAY_VIDEO_ONLY                         (MAKE_BIT_MASK_32(0))    /**< To play the video or not during scan                                            */
#define SB_PAL_SECAM_CONFIG_RANGE_SCAN_NO_WRAP_AROUND               (MAKE_BIT_MASK_32(1))    /**< To wrap around to the lowest frequency or not while the range scan cannot find 
                                                                                                  a valid channel till the highest frquency                                       */

#define SB_PAL_SECAM_CONFIG_START_CH_NUM                            (MAKE_BIT_MASK_32(2))    /**< To customize the the start channel numbers                                      */

#define SB_PAL_SECAM_CONFIG_IGNORE_DVB_CH_ON_SORTING                (MAKE_BIT_MASK_32(3))    /**< To ignore the DVB channels or not while sorting the PAL/SECAM channels.
                                                                                                  Note that to set this flag might cause the channel number collision between
                                                                                                  the digital and analog channels                                                 */

#define SB_PAL_SECAM_CONFIG_UPDATE_TO_TEMP_SVL                      (MAKE_BIT_MASK_32(4))    /**< To store the service records to the given temporary SVL                         */

#define SB_PAL_SECAM_CONFIG_CUSTOMIZE_ANAS                          (MAKE_BIT_MASK_32(5))    /**< Enable the customization of Auto-Naming-Auto-Sorting. The engine shall report 
                                                                                                  the CNI codes as well as channel information to the user so that it could 
                                                                                                  determine the channel names and sorting by itself. Also the engine shall 
                                                                                                  store the records into the temporary TSL/SVL.                                   */

#define SB_PAL_SECAM_CONFIG_ENABLE_ACI                              (MAKE_BIT_MASK_32(6))    /**< Enable the ACI functionality.                                                   */

#define SB_PAL_SECAM_CONFIG_IGNORE_TV_SYS_NOT_IN_CANDIDATE_LIST     (MAKE_BIT_MASK_32(7))    /**< Ignore detected format(B/G,D/K/I,L) that is not in the candidate list           */

#define SB_PAL_SECAM_CONFIG_ACI_MULTILVL_RGN                        (MAKE_BIT_MASK_32(8))    /**< Enable multi-level ACI                                                          */

#define SB_PAL_SECAM_CONFIG_TIME_OUT_FINE_TUNE                      (MAKE_BIT_MASK_32(9))    /**< Enable fine tune time out length, unit in miliseconds                           */

#define SB_PAL_SECAM_CONFIG_IGNORE_START_CH_NUM_CUSTOMIZATION       (MAKE_BIT_MASK_32(10))   /**< To ignore the start channel number customization                                */

#define SB_PAL_SECAM_CONFIG_DISABLE_VBIF                            (MAKE_BIT_MASK_32(11))   /**< To disable VBIF for Pan-Asia                                                    */

#define SB_PAL_SECAM_CONFIG_DISABLE_SKIP_DVB_CH_FREQUENCY           (MAKE_BIT_MASK_32(12))   /**< To disable update scan skip DTV channels' frequencies                           */

#define SB_PAL_SECAM_CONFIG_RANGE_SCAN_INFINITE_LOOP                (MAKE_BIT_MASK_32(13))   /**< Enable manual scan with infinite loop (no termination)                          */

#define SB_PAL_SECAM_CONFIG_SCAN_REPLACE_EXISTING_CHANNEL           (MAKE_BIT_MASK_32(14))   /**< Enable manual scan result stored to a specific channel                          */

#define SB_PAL_SECAM_CONFIG_SCAN_WITH_DESIGNATED_TV_SYS             (MAKE_BIT_MASK_32(15))   /**< Scan with fixed TV system                                                       */

#define SB_PAL_SECAM_CONFIG_SCAN_WITH_TV_BYPASS                     (MAKE_BIT_MASK_32(16))   /**< Scan with TV bypass enabled                                                     */

#define SB_PAL_SECAM_CONFIG_SCAN_WITH_MONITOR_BYPASS                (MAKE_BIT_MASK_32(17))   /**< Scan with Monitor bypass enabled                                                */

#define SB_PAL_SECAM_CONFIG_SCAN_CABLE_ANA                          (MAKE_BIT_MASK_32(18))   /**< To use cabel analog scan                                                        */

#if 1
#define SB_PAL_SECAM_CONFIG_SCAN_WITHOUT_DSP_DETECT_TV_SYS          (MAKE_BIT_MASK_32(19))   /**< Scan without DSP detect */
#define SB_PAL_SECAM_CONFIG_SCAN_ALL_CH_IN_RANGE_MODE               (MAKE_BIT_MASK_32(20))   /**< Scan all channel in range mode*/
#define SB_PAL_SECAM_CONFIG_FREQUENCY_AUTO_NFY                      (MAKE_BIT_MASK_32(21))   /**< Frequency automatic notify */
#define SB_PAL_SECAM_CONFIG_SCAN_WITH_DESIGNATED_COLOR_SYS          (MAKE_BIT_MASK_32(22))   /**< Scan with fixed TV system                                                       */
#endif

#define SB_PAL_SECAM_CONFIG_CUSTOM_SVL_CONFIG                       (MAKE_BIT_MASK_32(23))   /**< To use cabel analog scan                                                        */

#define SB_PAL_SECAM_CONFIG_USER_OPERATION                          (MAKE_BIT_MASK_32(24))   

#define SB_PAL_SECAM_CONFIG_ACTUAL_COLOR_SYS                        (MAKE_BIT_MASK_32(25))



/*------------------------------------------------------------------*/
/*! @enum  SCAN_MAPS_PAL_SECAM_CMD_T
 *  @brief Command to get the scan map and info
 *  @code
 *  typedef enum {
 *      SCAN_MAPS_PAL_SECAM_CMD_GET_TV_SYS_CHK_SEQ = 0,
 *  }   SCAN_MAPS_PAL_SECAM_CMD_T;
 *  @endcode
 *  @li@c  SCAN_MAPS_PAL_SECAM_CMD_GET_TV_SYS_CHK_SEQ - Get Scan Maps TV system check list
 */
/*------------------------------------------------------------------*/


#define SCAN_MAPS_PAL_SECAM_CMD_GET_TV_SYS_CHK_SEQ  ((SCAN_MAPS_PAL_SECAM_CMD_T) 0 )

#if 0
typedef enum {
    SCAN_MAPS_PAL_SECAM_CMD_GET_TV_SYS_CHK_SEQ = 0,

#ifdef SB_PAL_SECAM_CUSTOMIZED_SCANMAP /*Sony*/   
    SCAN_MAPS_PAL_SECAM_CMD_GET_FIRST_SCAN_IDX,
    SCAN_MAPS_PAL_SECAM_CMD_GET_LAST_SCAN_IDX,
    SCAN_MAPS_PAL_SECAM_CMD_GET_NEXT_SCAN_IDX,
    SCAN_MAPS_PAL_SECAM_CMD_GET_PREV_SCAN_IDX,
    SCAN_MAPS_PAL_SECAM_CMD_GET_FREQ_MAP_BY_SCAN_IDX,
    SCAN_MAPS_PAL_SECAM_CMD_GET_FREQ_MAP_BY_FREQ,
    SCAN_MAPS_PAL_SECAM_CMD_GET_FREQ_MAP_BY_CHANNEL_NAME,
#endif
}   SCAN_MAPS_PAL_SECAM_CMD_T;

#endif

/*------------------------------------------------------------------*/
/*! @brief function point definition of PAL SECAM scan map
 *  @param [in]     t_cmd           - Contains the command
 *  @param [in]     t_country_code  - Contains the country code
 *  @param [in]     pv_input        - Not used
 *
 *  @param [out]    pv_output       - Contains the designated check system by ehe country
 *  @param [out]    pz_output_len   - Contains then length of pv_output
 *  @return  
 *  @retval SBR_OK               - OK
 *  @retval SBR_NOT_ENOUGH_SPACE - Fail
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_pal_secam_scan_maps_fct) (SCAN_MAPS_PAL_SECAM_CMD_T     t_cmd,
                                               ISO_3166_COUNT_T              t_country_code,
                                               VOID*                         pv_input,
                                               VOID*                         pv_output,
                                               SIZE_T*                       pz_output_len);

typedef INT32 (*sb_pal_secam_scan_maps_fct_prv) (SCAN_MAPS_PAL_SECAM_CMD_T     t_cmd,
                                               ISO_3166_COUNT_T              t_country_code,
                                               VOID*                         pv_input,
                                               VOID*                         pv_output,
                                               SIZE_T*                       pz_output_len,
                                               VOID*                         pv_tag);




/*------------------------------------------------------------------*/
/*! @struct SB_PAL_SECAM_INIT_DATA_T
 *  @brief  The structure SB_PAL_SECAM_INIT_DATA_T is used as the argument 
 *          of the API x_sb_pal_secam_eng_init(). 
 *  @code
 *  typedef struct _SB_PAL_SECAM_INIT_DATA_T
 *  {
 *      SB_PAL_SECAM_CONFIG_T           t_eng_cfg_flag;
 *      UINT16                          ui2_start_ch_num;
 *      UINT16                          ui2_timeout_fine_tune;
 *      x_sb_pal_secam_scan_maps_fct    pf_scan_maps;
 *  } SB_PAL_SECAM_INIT_DATA_T;
 *  @endcode
 *  @li@c  t_eng_cfg_flag        - Contains the engine configuration flag. The option allowed is 
 *                                 SB_PAL_SECAM_CONFIG_IGNORE_DVB_CH_ON_SORTING, 
 *                                 SB_PAL_SECAM_CONFIG_START_CH_NUM.
 *  @li@c  ui2_start_ch_num      - Contains the start channel number. This argument is used only 
 *                                 when the SB_PAL_SECAM_CONFIG_START_CH_NUM is set.
 *  @li@c  ui2_timeout_fine_tune - Time out of fine tune
 *  @li@c  pf_scan_maps          - PF of scan map function
 */
/*------------------------------------------------------------------*/
typedef struct _SB_PAL_SECAM_INIT_DATA_T
{
    SB_PAL_SECAM_CONFIG_T           t_eng_cfg_flag;
    UINT16                          ui2_start_ch_num;
    UINT16                          ui2_timeout_fine_tune;
    x_sb_pal_secam_scan_maps_fct    pf_scan_maps;
} SB_PAL_SECAM_INIT_DATA_T;



/*------------------------------------------------------------------*/
/*! @struct SB_PAL_SECAM_OPEN_DATA_T
 *  @brief  The structure SB_PAL_SECAM_OPEN_DATA_T is used as the argument
 *          pv_open_data of the API c_sb_open_builder()/x_sb_open_builder()
 *          when opening an SVL Builder with the PAL/SECAM engine.
 *  @code
 *  typedef struct _SB_PAL_SECAM_OPEN_DATA_T
 *  {
 *      UINT16      ui2_svl_id;
 *      CHAR*       ps_svl_name;
 *      CHAR*       ps_file_name;
 *      CHAR*       ps_tuner_name;
 *      BOOL        b_use_orig;
 *  } SB_PAL_SECAM_OPEN_DATA_T;
 *  @endcode
 *  @li@c  ui2_svl_id    - The id of SVL to be loaded.
 *  @li@c  ps_svl_name   - The name of SVL to be loaded.
 *  @li@c  ps_file_name  - The file name from which the SVL/TSL will be loaded.
 *  @li@c  ps_tuner_name - Tuner name
 *  @li@c  b_use_orig    - A flag. When set to true, use the existed SVL,
 *                         instead of reloading it.  When set to false,
 *                         overwrite the SVL (if exists) by reloading.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_PAL_SECAM_OPEN_DATA_T
{
    UINT16      ui2_svl_id;
    CHAR*       ps_svl_name;
    CHAR*       ps_file_name;
    CHAR*       ps_tuner_name;
    BOOL        b_use_orig;
} SB_PAL_SECAM_OPEN_DATA_T;



/*------------------------------------------------------------------*/
/*! @enum   SB_PAL_SECAM_SCAN_TYPE_T
 *  @brief  PAL/SECAM scan type
 *  @code
 *  typedef enum
 *  {
 *      SB_PAL_SECAM_SCAN_TYPE_UNKNOWN = 0,
 *      SB_PAL_SECAM_SCAN_TYPE_FULL_MODE,
 *      SB_PAL_SECAM_SCAN_TYPE_RANGE_MODE,
 *      SB_PAL_SECAM_SCAN_TYPE_UPDATE_MODE,
 *      SB_PAL_SECAM_SCAN_TYPE_NUM 
 *  } SB_PAL_SECAM_SCAN_TYPE_T;
 *  @endcode
 *  @li@c  SB_PAL_SECAM_SCAN_TYPE_UNKNOWN     - Scan type:unknow
 *  @li@c  SB_PAL_SECAM_SCAN_TYPE_FULL_MODE   - Scan type:full mode
 *  @li@c  SB_PAL_SECAM_SCAN_TYPE_RANGE_MODE  - Scan type:range mode
 *  @li@c  SB_PAL_SECAM_SCAN_TYPE_UPDATE_MODE - Scan type:updae mode
 *  @li@c  SB_PAL_SECAM_SCAN_TYPE_NUM         - Number of scan types
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_PAL_SECAM_SCAN_TYPE_UNKNOWN = 0,
    SB_PAL_SECAM_SCAN_TYPE_FULL_MODE,
    SB_PAL_SECAM_SCAN_TYPE_RANGE_MODE,
    SB_PAL_SECAM_SCAN_TYPE_UPDATE_MODE,
    SB_PAL_SECAM_SCAN_TYPE_NUM /*  */
} SB_PAL_SECAM_SCAN_TYPE_T;



/*------------------------------------------------------------------*/
/*! @struct SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA
 *  @brief  The structure SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA is used as the argument 
 *          of the frequency progress notification function. 
 *  @code
 *  typedef struct _SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA
 *  {
 *      UINT32              ui4_freq;
 *  } SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA;
 *  @endcode
 *  @li@c  ui4_freq                                          - Frequency
 */
/*------------------------------------------------------------------*/
typedef struct _SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA
{
    UINT32              ui4_freq;
} SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA;




/*------------------------------------------------------------------*/
/*! @brief  The prototype of the frequency progress notification function
 *  @param [in] pt_nfy_data - Contains notify data
 *  @param [in] pv_nfy_tag  - Contains notify tag
 *  @return
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_freq_progress_nfy_fct) (
        SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA*    pt_nfy_data,
        VOID*                                   pv_nfy_tag);



/*------------------------------------------------------------------*/
/*! @struct SB_PAL_SECAM_ANAS_NFY_DATA
 *  @brief  The structure SB_PAL_SECAM_ANAS_NFY_DATA is used as the argument of the 
 *          CNI and channel information notification function. 
 *  @code
 *  typedef struct _SB_PAL_SECAM_ANAS_NFY_DATA
 *  {
 *      UINT16              ui2_svl_rec_id;
 *      FLTR_NI_SRC_TYPE_T  t_ni_src_mask;
 *      UINT16              ui2_vps_ni_code;
 *      UINT16              ui2_ttx_fmt_1_ni_code;
 *      UINT16              ui2_ttx_fmt_2_cni_code;
 *      NI_VPS_DATA_T       t_vps_data;
 *  } SB_PAL_SECAM_ANAS_NFY_DATA;
 *  @endcode
 *  @li@c  ui2_svl_rec_id         - Contains SVL record id
 *  @li@c  t_ni_src_mask          - Contains NI MASK
 *  @li@c  ui2_vps_ni_code        - Contains VPS NI code
 *  @li@c  ui2_ttx_fmt_1_ni_code  - Contains TTX format1 NI code
 *  @li@c  ui2_ttx_fmt_2_cni_code - Contains TTX format2 CNI code
 *  @li@c  t_vps_data             - Contains VPS data
 */
/*------------------------------------------------------------------*/
typedef struct _SB_PAL_SECAM_ANAS_NFY_DATA
{
    UINT16              ui2_svl_rec_id;

    FLTR_NI_SRC_TYPE_T  t_ni_src_mask;
    UINT16              ui2_vps_ni_code;
    UINT16              ui2_ttx_fmt_1_ni_code;
    UINT16              ui2_ttx_fmt_2_cni_code;
    NI_VPS_DATA_T       t_vps_data;
} SB_PAL_SECAM_ANAS_NFY_DATA;

/*------------------------------------------------------------------*/
/*! @struct SB_PAL_SECAM_CUSTOM_SVL_CONFIG
 *  @brief  The structure SB_PAL_SECAM_CUSTOMIZE_SVL_CONFIG is used for customizing TV system mask and audio system mask.
 *  @code
 *  typedef struct _SB_PAL_SECAM_CUSTOM_SVL_CONFIG
 *  {
 *      UINT32  ui4_tv_sys_mask;
 *      UINT32  ui4_aud_sys_mask;
 *  } SB_PAL_SECAM_CUSTOM_SVL_CONFIG;
 *  @endcode
 *  @li@c  ui4_tv_sys_mask         - Contains TV system mask
 *  @li@c  ui4_aud_sys_mask        - Contains Audio system mask
 */
/*------------------------------------------------------------------*/
typedef struct _SB_PAL_SECAM_CUSTOM_SVL_CONFIG
{
    UINT32  ui4_tv_sys_mask;
    UINT32  ui4_aud_sys_mask;
} SB_PAL_SECAM_CUSTOM_SVL_CONFIG;

/*------------------------------------------------------------------*/
/*! @brief The prototype of the CNI and channel information notification function
 *  @param [in]     pt_nfy_data - Contains notify data
 *  @param [in]     pv_nfy_tag  - Contains notify tag
 *  @return
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_anas_nfy_fct) (
        SB_PAL_SECAM_ANAS_NFY_DATA* pt_nfy_data,
        VOID*                       pv_nfy_tag);



/*------------------------------------------------------------------*/
/*! @enum  SB_PAL_SECAM_ACI_NFY_CODE
 *  @brief ACI notification code
 *  @code
 *  typedef enum
 *  {
 *      SB_PAL_SECAM_ACI_NFY_DETECTED = 0,
 *      SB_PAL_SECAM_ACI_NFY_SLCTN_PAGE_NUM,
 *      SB_PAL_SECAM_ACI_NFY_DB_UPDATED,
 *      SB_PAL_SECAM_ACI_NFY_COMPLETE,
 *      SB_PAL_SECAM_ACI_NFY_ABORT
 *  } SB_PAL_SECAM_ACI_NFY_CODE;
 *  @endcode
 *  @li@c  SB_PAL_SECAM_ACI_NFY_DETECTED       - ACI code Detected
 *  @li@c  SB_PAL_SECAM_ACI_NFY_SLCTN_PAGE_NUM - ACI code Selection page number
 *  @li@c  SB_PAL_SECAM_ACI_NFY_DB_UPDATED     - ACI code Database updated
 *  @li@c  SB_PAL_SECAM_ACI_NFY_COMPLETE       - ACI code Complete
 *  @li@c  SB_PAL_SECAM_ACI_NFY_ABORT          - ACI code Abort
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_PAL_SECAM_ACI_NFY_DETECTED = 0,
    SB_PAL_SECAM_ACI_NFY_SLCTN_PAGE_NUM,
    SB_PAL_SECAM_ACI_NFY_DB_UPDATED,
    SB_PAL_SECAM_ACI_NFY_COMPLETE,
    SB_PAL_SECAM_ACI_NFY_ABORT
} SB_PAL_SECAM_ACI_NFY_CODE;


/*------------------------------------------------------------------*/
/*! @enum  SB_PAL_SECAM_ACI_ABORT_REASON
 *  @brief ACI operation abort reason code
 *  @code
 *  typedef enum
 *  {
 *      SB_PAL_SECAM_ACI_ABORT_TIMEOUT = 0,
 *      SB_PAL_SECAM_ACI_ABORT_PROCESS_DB_FAIL,
 *      SB_PAL_SECAM_ACI_ABORT_CANCELLED,
 *  } SB_PAL_SECAM_ACI_ABORT_REASON;
 *  @endcode
 *  @li@c  SB_PAL_SECAM_ACI_ABORT_TIMEOUT         - ACI ABORT reason: timeout for retrieve ACI information
 *  @li@c  SB_PAL_SECAM_ACI_ABORT_PROCESS_DB_FAIL - ACI ABORT reason: process ACI database fail
 *  @li@c  SB_PAL_SECAM_ACI_ABORT_CANCELLED       - ACI ABORT reason: cancel
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_PAL_SECAM_ACI_ABORT_TIMEOUT = 0,
    SB_PAL_SECAM_ACI_ABORT_PROCESS_DB_FAIL,
    SB_PAL_SECAM_ACI_ABORT_CANCELLED,
} SB_PAL_SECAM_ACI_ABORT_REASON;

/*  */
/*------------------------------------------------------------------*/
/*! @struct SB_PAL_SECAM_ACI_NFY_DATA
 *  @brief  The structure SB_PAL_SECAM_ACI_NFY_DATA is used as the argument of the 
 *          ACI notification function.
 *  @code
 *  typedef struct _SB_PAL_SECAM_ACI_NFY_DATA
 *  {
 *      SB_PAL_SECAM_ACI_NFY_CODE                   e_nfy_code;
 *      union {
 *          FLTR_NI_NFY_ACI_DETECTED_T              t_detected;     
 *          FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T     t_slctn_page;   
 *          FLTR_NI_NFY_ACI_DB_READY_T              t_db_updated;   
 *          UINT16                                  ui2_num_ch;     
 *          SB_PAL_SECAM_ACI_ABORT_REASON           e_abort_reason; 
 *      } u;
 *  } SB_PAL_SECAM_ACI_NFY_DATA;
 *  @endcode
 *  @li@c  e_nfy_code     - Reference SB_PAL_SECAM_ACI_NFY_CODE
 *  @li@c  t_detected     - Refefence FLTR_NI_NFY_ACI_DETECTED_T
 *  @li@c  t_slctn_page   - Refefence FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T
 *  @li@c  t_db_updated   - Refefence FLTR_NI_NFY_ACI_DB_READY_T
 *  @li@c  ui2_num_ch     - Channel number
 *  @li@c  e_abort_reason - About reason reference SB_PAL_SECAM_ACI_ABORT_REASON
 */
/*------------------------------------------------------------------*/
typedef struct _SB_PAL_SECAM_ACI_NFY_DATA
{
    SB_PAL_SECAM_ACI_NFY_CODE                   e_nfy_code;

    union {
        FLTR_NI_NFY_ACI_DETECTED_T              t_detected;     /* SB_PAL_SECAM_ACI_NFY_DETECTED */
        FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T     t_slctn_page;   /* SB_PAL_SECAM_ACI_NFY_SLCTN_PAGE_NUM */
        FLTR_NI_NFY_ACI_DB_READY_T              t_db_updated;   /* SB_PAL_SECAM_ACI_NFY_DB_UPDATED */
        UINT16                                  ui2_num_ch;     /* SB_PAL_SECAM_ACI_NFY_COMPLETE */
        SB_PAL_SECAM_ACI_ABORT_REASON           e_abort_reason; /* SB_PAL_SECAM_ACI_NFY_ABORT */
    } u;
} SB_PAL_SECAM_ACI_NFY_DATA;


/*------------------------------------------------------------------*/
/*! @brief The prototype of the ACI notification function
 *  @param [in]     pt_nfy_data - Contains notify data
 *  @param [in]     pv_nfy_tag  - Contains notify tag
 *  @return
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_aci_nfy_fct) (
        SB_PAL_SECAM_ACI_NFY_DATA*  pt_nfy_data,
        VOID*                       pv_nfy_tag);


typedef struct _SB_PAL_SECAM_USER_OPERATION_DATA
{
    UINT32                      ui4_curr_freq;
    UINT32                      ui4_found_ch_nums;
    UINT32                      ui4_data;
} SB_PAL_SECAM_USER_OPERATION_DATA;


typedef VOID (*x_sb_user_operation_nfy_fct) (
        SB_PAL_SECAM_USER_OPERATION_DATA*   pt_user_operation,
        VOID*                               pv_nfy_tag);


/*------------------------------------------------------------------*/
/*! @struct SB_PAL_SECAM_SCAN_DATA_T
 *  @brief  The structure SB_PAL_SECAM_SCAN_DATA_T is used as the argument pv_scan_data of the API 
 *          x_sb_start_scan() when scaning a media.
 *  @code
 *  typedef struct _SB_PAL_SECAM_SCAN_DATA_T
 *  {
 *      ISO_3166_COUNT_T                    t_country_code;
 *      SB_PAL_SECAM_SCAN_TYPE_T            e_scan_type;
 *      UINT32                              ui4_start_freq; 
 *      UINT32                              ui4_end_freq;
 *      UINT32                              ui2_start_ch_id_to_scan;
 *      UINT32                              ui2_end_ch_id_to_scan;
 *      UINT32                              ui4_designated_chk_tv_sys;  
 *      BOOL                                b_neg_dir;
 *      SB_PAL_SECAM_CONFIG_T               t_eng_cfg_flag;
 *      HANDLE_T                            h_svctx;
 *      HANDLE_T                            h_svctx_monitor_scart_bypass;
 *      HANDLE_T                            h_svctx_tv_scart_bypass;
 *      CHAR*                               ps_aud_name;
 *      UINT16                              ui2_temp_svl_id;
 *      x_sb_freq_progress_nfy_fct          pf_freq_nfy;
 *      VOID*                               pv_freq_nfy_tag;
 *      x_sb_anas_nfy_fct                   pf_anas_nfy;
 *      VOID*                               pv_anas_nfy_tag;
 *      FLTR_NI_INIT_TTX_HDR_CH_NAME_T      t_ttx_hdr_ch_name;
 *      BOOL                                b_anas_8_30_fmt_1_prg_ttl;
 *      ISO_639_LANG_T                      t_lang;
 *      FLTR_NI_ACI_ORIGR_TYPE_MASK_T       t_aci_origr_mask;
 *      FLTR_NI_ACI_NETWORK_TYPE_MASK_T     t_aci_network_mask;
 *      UINT16                              ui2_aci_timeout;
 *      x_sb_aci_nfy_fct                    pf_aci_nfy;
 *      VOID*                               pv_aci_nfy_tag;
 *  } SB_PAL_SECAM_SCAN_DATA_T;
 *  @endcode
 *  @li@c  t_country_code               - Contains an ISO 3166 3-character country code.
 *  @li@c  e_scan_type                  - Specifies the way to scan.
 *  @li@c  ui4_start_freq               - Contains the scan starting frequency that is only meaningful if it is 
 *                                        a range scan requested.
 *  @li@c  ui4_end_freq                 - Contains the scan ending frequency that is only meaningful if it is a 
 *                                        range scan requested.
 *  @li@c  ui2_start_ch_id_to_scan      - start channel id indicator by caller
 *  @li@c  ui2_end_ch_id_to_scan        - end channel id indicator by caller
 *  @li@c  ui4_designated_chk_tv_sys    - designated check system
 *  @li@c  b_neg_dir                    - Specifies scanning in the negative direction.
 *  @li@c  t_eng_cfg_flag               - Contains the engine configuration flag. The options allowed are 
 *                                        SB_PAL_SECAM_CONFIG_PLAY_VIDEO_ONLY, 
 *                                        SB_PAL_SECAM_CONFIG_RANGE_SCAN_NO_WRAP_AROUND,
 *                                        SB_PAL_SECAM_CONFIG_UPDATE_TO_TEMP_SVL,
 *                                        SB_PAL_SECAM_CONFIG_CUSTOMIZE_ANAS,
 *                                        SB_PAL_SECAM_CONFIG_ENABLE_ACI.
 *  @li@c  h_svctx                      - Contains the handle of the SVCTX. This argument is mandatory when the 
 *                                        SB_PAL_SECAM_CONFIG_PLAY_VIDEO_ONLY is set.
 *  @li@c  h_svctx_monitor_scart_bypass - Contains handle of svctx monitor scart 
 *  @li@c  h_svctx_tv_scart_bypass      - Contains handle of svctx TV scart 
 *  @li@c  ps_aud_name                  - References the name of the audio decoder. This argument is mandatory 
 *                                        when the SB_PAL_SECAM_CONFIG_PLAY_VIDEO_ONLY is not set.
 *  @li@c  ui2_temp_svl_id              - Contains the id of the temporary SVL/TSL to be updated. This argument 
 *                                        is used only when the SB_PAL_SECAM_CONFIG_UPDATE_TO_TEMP_SVL and/or 
 *                                        SB_PAL_SECAM_CONFIG_CUSTOMIZE_ANAS is set. This list must exist and be 
 *                                        an empty one. Note that if the configuration flag 
 *                                        SB_PAL_SECAM_CONFIG_CUSTOMIZE_ANAS is defined, then in this field a 
 *                                        valid SVL id MUST be provided, i.e. to support the ANAS customization, 
 *                                        the engine always store the records into a temporary SVL/TSL.
 *  @li@c  pf_freq_nfy                  - Contains the frequency progress notification function.
 *  @li@c  pv_freq_nfy_tag              - Contains the tag for the frequency progress notification function.
 *  @li@c  pf_anas_nfy                  - Contains the CNI notification function.
 *  @li@c  pv_anas_nfy_tag              - Contains the tag for the CNI notification function.
 *  @li@c  t_ttx_hdr_ch_name            - Contains the parser function and tag for the Teletext header channel name.
 *  @li@c  b_anas_8_30_fmt_1_prg_ttl    - Contains 8_30_fmt_1_prg_ttl detect flag when SB_PAL_SECAM_CONFIG_CUSTOMIZE_ANAS been set.
 *  @li@c  t_lang                       - Contains the language code.
 *  @li@c  t_aci_origr_mask             - Contains the mask of ACI originator network.
 *  @li@c  t_aci_network_mask           - Contains the mask of ACI network.
 *  @li@c  ui2_aci_timeout              - Contains the timeout value of the ACI retrieval in seconds; a value of 
 *                                        indicates a timeout of 20 seconds.
 *  @li@c  pf_aci_nfy                   - Contains the ACI notification function.
 *  @li@c  pv_aci_nfy_tag               - Contains the tag for the ACI notification function.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_PAL_SECAM_SCAN_DATA_T
{
    ISO_3166_COUNT_T                      t_country_code;
    SB_PAL_SECAM_SCAN_TYPE_T              e_scan_type;
    UINT32                                ui4_start_freq; 
    UINT32                                ui4_end_freq;
    UINT32                                ui2_start_ch_id_to_scan;
    UINT32                                ui2_end_ch_id_to_scan;
    UINT32                                ui4_designated_chk_tv_sys;  /* For TV sys specified by application to check */
    UINT32                                ui4_designated_chk_tv_audio_sys;  /* For TV sys specified by application to check */
    INT32                                 i4_designated_chk_color_sys;  /* For color sys specified by application to check */
    BOOL                                  b_neg_dir;
    SB_PAL_SECAM_CONFIG_T                 t_eng_cfg_flag;
    HANDLE_T                              h_svctx;
    HANDLE_T                              h_svctx_monitor_scart_bypass;
    HANDLE_T                              h_svctx_tv_scart_bypass;
    CHAR*                                 ps_aud_name;
    UINT16                                ui2_temp_svl_id;
    x_sb_freq_progress_nfy_fct            pf_freq_nfy;
    VOID*                                 pv_freq_nfy_tag;
    x_sb_user_operation_nfy_fct           pf_user_operation_nfy;
    VOID*                                 pv_user_operation_nfy_tag;
    x_sb_anas_nfy_fct                     pf_anas_nfy;
    VOID*                                 pv_anas_nfy_tag;
    FLTR_NI_INIT_TTX_HDR_CH_NAME_T        t_ttx_hdr_ch_name;
    BOOL                                  b_anas_8_30_fmt_1_prg_ttl;
    ISO_639_LANG_T                        t_lang;
    FLTR_NI_ACI_ORIGR_TYPE_MASK_T         t_aci_origr_mask;
    FLTR_NI_ACI_NETWORK_TYPE_MASK_T       t_aci_network_mask;
    UINT16                                ui2_aci_timeout;
    x_sb_aci_nfy_fct                      pf_aci_nfy;
    VOID*                                 pv_aci_nfy_tag;
    SB_PAL_SECAM_CUSTOM_SVL_CONFIG        t_custom_svl_config;
} SB_PAL_SECAM_SCAN_DATA_T;


/* The TSL record id of the PAL/SECAM channels must be separated from that of the 
   DVB channels, which is actually their RF channel number. */
#define SB_MAKE_PAL_SECAM_TSL_REC_ID(x)     ((UINT16) (x) + 600)    /**< The TSL record id of the PAL/SECAM channels must be separated from that of the DVB channels, which is actually their RF channel number.       */



/*------------------------------------------------------------------*/
/*! @struct SB_ACI_SLCT_CITY_T
 *  @brief  Key data structure for the key SB_KEY_TYPE_ACI_SLCT_CITY 
 *  @code
 *  typedef struct _SB_ACI_SLCT_CITY_T
 *  {
 *      UINT8   ui1_slct_city;
 *      UINT16  ui2_page_clst;
 *  }   SB_ACI_SLCT_CITY_T;
 *  @endcode
 *  @li@c  ui1_slct_city - Contains select city
 *  @li@c  ui2_page_clst - Contains page selected
 */
/*------------------------------------------------------------------*/
typedef struct _SB_ACI_SLCT_CITY_T
{
    UINT8   ui1_slct_city;
    UINT16  ui2_page_clst;
}   SB_ACI_SLCT_CITY_T;


typedef struct _SB_USER_OPERATION_T
{
    UINT32              ui4_data;
}   SB_USER_OPERATION_T;


typedef enum {
    SB_INSERT_SVL_TSL  = 0,
    SB_UPDATE_SVL_TSL,
}   SB_INSERT_DEFAULT_SVL_TSL_ACTION_T;


/*------------------------------------------------------------------*/
/*! @struct SB_INSERT_DEFAULT_SVL_TSL_T
 *  @brief  Key data structure for the key SB_KEY_TYPE_INSERT_DEFAULT_SVL_TSL
 *  @code
 *  typedef struct _SB_INSERT_DEFAULT_SVL_TSL_T
 *  {
 *      UINT16  ui2_start_ch_num;
 *      UINT16  ui2_end_ch_num;
 *      UINT32  ui4_default_freq;
 *  }   SB_INSERT_DEFAULT_SVL_TSL_T;
 *  @endcode
 *  @li@c  ui2_start_ch_num - Contains start channel number
 *  @li@c  ui2_end_ch_num   - Contains end  channel number
 *  @li@c  ui4_default_freq - Contains defaule frequency
 */
/*------------------------------------------------------------------*/
typedef struct _SB_INSERT_DEFAULT_SVL_TSL_T
{
    UINT16  ui2_start_ch_num;        /* For TSL and SVL */
    UINT16  ui2_end_ch_num;          /* For TSL and SVL */
    UINT32  ui4_default_freq;        /* For TSL */
    UINT32  ui4_default_tv_sys;      /* For SVL */
    UINT32  ui4_default_audio_sys;   /* For SVL */
    UINT32  ui4_nw_mask;             /* For default SVL mask*/
    SB_INSERT_DEFAULT_SVL_TSL_ACTION_T  e_action;/* For insert or update  default SVL TSL*/
}   SB_INSERT_DEFAULT_SVL_TSL_T;


#endif /* _U_SB_PAL_SECAM_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_CONN_MNGR_BRDCST */
/*----------------------------------------------------------------------------*/

