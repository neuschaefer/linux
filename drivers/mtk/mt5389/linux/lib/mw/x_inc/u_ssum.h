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
/*! @file u_ssum.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file contains SSU Manager macros, defines, typedefs, enums.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_SSU_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                    include files
--------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

#ifndef _U_SSUM_H_
#define _U_SSUM_H_


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 
/* handle type */
#define SSUM_STATE_MACHINE_HANDLE  (HT_GROUP_SSUM + ((HANDLE_TYPE_T) 0))    /**< SSU state machine handle type */
#define SSUM_DLD_FILE              (HT_GROUP_SSUM + ((HANDLE_TYPE_T) 1))    /**< SSU file download handle type */


/* Return Code of notify function*/
#define SSUMR_FAIL                              ((INT32) -100)  /**< SSUM failed */
#define SSUMR_CLI_ERROR                         ((INT32) -99)   /**< SSUM cli error */

#define SSUMR_UKOAD_FILE_NUM_OVER_MAX          ((INT32) -20)   /** < SSUM UKOAD File number is over max number */
#define SSUMR_ONID_AND_COUNTRY_NOT_MATCH        ((INT32) -19)   /**< Check ONID and country code do not match */
#define SSUMR_CHK_PMT_ALL_NOT_MATCH             ((INT32) -18)   /**< All PMT's are chekced and do not match */
#define SSUMR_GET_ASSOCIATION_TAG_FAIL          ((INT32) -17)   /**< Association tag does not exist */
#define SSUMR_GET_COMPATIBILITY_ENTRY_FAIL      ((INT32) -16)   /**< Not used */
#define SSUMR_GET_GROUP_INFO_ENTRY_FAIL         ((INT32) -15)   /**< Get group info entry failed */
#define SSUMR_CHK_GROUP_NOT_MATCH               ((INT32) -14)   /**< Group does not match */
#define SSUMR_CHK_COMPATIBILITY_HW_NOT_MATCH    ((INT32) -13)   /**< Not used */
#define SSUMR_CHK_COMPATIBILITY_SW_NOT_MATCH    ((INT32) -12)   /**< Not used */
#define SSUMR_CHK_COMPATIBILITY_NOT_MATCH       ((INT32) -11)   /**< Not used */
#define SSUMR_CHK_COMPATIBILITY_FAIL            ((INT32) -10)   /**< Not used */
#define SSUMR_GET_COMPATIBILITY_DESC_FAIL       ((INT32) -9)    /**< Not used */
#define SSUMR_GET_DSI_FAIL                      ((INT32) -8)    /**< Not used */
#define SSUMR_GET_FILE_INFO_FAIL                ((INT32) -7)    /**< Not used */
#define SSUMR_STM_INIT_FAIL		                ((INT32) -6)    /**< State machien initialization failed */
#define SSUMR_SEND_MSG_FAIL		                ((INT32) -5)    /**< Send message failed */
#define SSUMR_HANDLE_FAIL		                ((INT32) -4)    /**< Handle allocation failed */
#define SSUMR_INV_ARG			                ((INT32) -3)    /**< Invalid argument */
#define SSUMR_TM_FAIL                           ((INT32) -2)    /**< Failed while calling table manager API */
#define SSUMR_CORE                              ((INT32) -1)    /**< SSU internal error */
#define SSUMR_OK                                ((INT32)  0)    /**< OK */
#define SSUMR_NO_LINKAGE_DESC                   ((INT32)  1)    /**< No linkage descriptor */
#define SSUMR_SSU_LINKAGE_DESC_IN_CURRENT_TS    ((INT32)  2)    /**< Linkage descriptor points to the current TS */
#define SSUMR_SSU_LINKAGE_DESC_IN_OTHER_TS      ((INT32)  3)    /**< Linkage descriptor points to another TS */
#define SSUMR_CHK_COMPATIBILITY_MATCH           ((INT32)  4)    /**< Compatibility matched */
#define SSUMR_CHK_GROUP_MATCH                   ((INT32)  5)    /**< Group matched */
#define SSUMR_CHK_COMPATIBILITY_HW_MATCH        ((INT32)  6)    /**< Hardware compatibility matched */
#define SSUMR_CHK_COMPATIBILITY_SW_MATCH        ((INT32)  7)    /**< Software compatibility matched */

/*------------------------------------------------------------------*/
/*! @enum  SSU_GET_TYPE_T
 *  @brief Get type option
 *  @code
 *  typedef enum
 *  {
 *      SSU_GET_TYPE_TIMEOUT_LENGTH = 1,
 *      SSU_GET_TYPE_UKOAD_MIS,
 *      SSU_GET_TYPE_END
 *  } SSU_GET_TYPE_T;
 *  @endcode
 *  @li@c  SSU_GET_TYPE_TIMEOUT_LENGTH - Get SSU/OAD detection timeout length
 *  @li@c  SSU_GET_TYPE_END            - End
 */
/*------------------------------------------------------------------*/
typedef enum
{
    /* SSU/OAD Get Types */
    SSU_GET_TYPE_TIMEOUT_LENGTH = 1,
    SSU_GET_TYPE_UKOAD_MIS,
    SSU_GET_TYPE_END
} SSU_GET_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum  SSU_SET_TYPE_T
 *  @brief Get type option
 *  @code
 *  typedef enum
 *  {
 *      SSU_SET_TYPE_LINKAGE_PROCESSED = 1,
 *      SSU_SET_TYPE_END
 *  } SSU_SET_TYPE_T;
 *  @endcode
 *  @li@c  SSU_SET_TYPE_LINKAGE_PROCESSED - Inform SSU/OAD engine that linkage is processed
 *  @li@c  SSU_SET_TYPE_END               - End
 */
/*------------------------------------------------------------------*/
typedef enum
{
    /* SSU/OAD Get Types */
    SSU_SET_TYPE_LINKAGE_PROCESSED = 1,
    SSU_SET_TYPE_END
} SSU_SET_TYPE_T;

/* -------------------------------------------------------------------- */
/* Notify App Typedef */
/*------------------------------------------------------------------*/
/*! @enum  SSU_NFY_RET_T
 *  @brief Return type of SSU notification to AP
 *  @code
 *  typedef enum
 *  {
 *      SSU_COND_MISMATCHED = -1,
 *      SSU_COND_MATCHED,
 *      SSU_COND_MATCHED_AND_NEXT,
 *      SSU_COND_IGNORE_LINKAGE,
 *      SSU_COND_WAIT_PROC_LINKAGE,
 *      SSU_INFORM_OK,
 *      SSU_INFORM_ERROR
 *  } SSU_NFY_RET_T;
 *  @endcode
 *  @li@c  SSU_COND_MISMATCHED        - Inform SSU engine that condition is mismatched.
 *  @li@c  SSU_COND_MATCHED           - Inform SSU engine that condition is matched.
 *  @li@c  SSU_COND_MATCHED_AND_NEXT  - Inform SSU engine that condition is matched, and keep parsing.
 *  @li@c  SSU_COND_IGNORE_LINKAGE    - Inform SSU engine to ignore linkage descriptor.
 *  @li@c  SSU_COND_WAIT_PROC_LINKAGE - Inform SSU engine to wait for processing of linkage descriptor.
 *  @li@c  SSU_INFORM_OK              - Inform SSU engine that condition is OK.
 *  @li@c  SSU_INFORM_ERROR           - Inform SSU engine that condition is ERROR.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SSU_COND_MISMATCHED = -1,
    SSU_COND_MATCHED,
    SSU_COND_MATCHED_AND_NEXT,
    SSU_COND_IGNORE_LINKAGE,
    SSU_COND_WAIT_PROC_LINKAGE,
    SSU_INFORM_OK,
    SSU_INFORM_ERROR
} SSU_NFY_RET_T;

#if 0
/*------------------------------------------------------------------*/
/*! @enum  SSU_DOWNLOAD_NFY_RET_T
 *  @brief Not used.
 *  @code
 *  typedef enum
 *  {
 *      SSU_DOWNLOAD_FAIL = -1,
 *      SSU_DOWNLOAD_OK,
 *      SSU_DOWNLOAD_SKIP
 *  } SSU_DOWNLOAD_NFY_RET_T;
 *  @endcode
 *  @li@c  SSU_DOWNLOAD_FAIL                                 -
 *  @li@c  SSU_DOWNLOAD_OK                                   -
 *  @li@c  SSU_DOWNLOAD_SKIP                                 -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SSU_DOWNLOAD_FAIL = -1,
    SSU_DOWNLOAD_OK,
    SSU_DOWNLOAD_SKIP
} SSU_DOWNLOAD_NFY_RET_T;
#endif
/* -------------------------------------------------------------------- */


#define SSU_OPEN_CONFIG_OUI                    (1)    /**< Mask for configuring OUI */
#define SSU_OPEN_CONFIG_NETWORK_ID             (2)    /**< Mask for configuring Network ID */
#define SSU_OPEN_CONFIG_BROADCAST_TYPE         (4)    /**< Mask for configuring Broadcast type */

/*------------------------------------------------------------------*/
/*! @struct SSU_OPEN_SETTING_T
 *  @brief  Used to indicate open setting when opening SSU.
 *  @code
 *  typedef struct
 *  {
 *      UINT32            ui4_open_setting_mask;
 *      UINT32            ui4_oui;
 *      UINT16            ui2_network_id;
 *      BRDCST_MEDIUM_T   e_brdcst_medium;
 *  } SSU_OPEN_SETTING_T;
 *  @endcode
 *  @li@c  ui4_open_setting_mask - Specifies which field is valid
 *  @li@c  ui4_oui               - Specifies OUI to detect       : SSU_OPEN_CONFIG_OUI
 *  @li@c  ui2_network_id        - Specifies network ID to detect: SSU_OPEN_CONFIG_NETWORK_ID
 *  @li@c  e_brdcst_medium       - Specifies broadcast medium    : SSU_OPEN_CONFIG_BROADCAST_TYPE
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32            ui4_open_setting_mask;
    UINT32            ui4_oui;
    UINT16            ui2_network_id;
    BRDCST_MEDIUM_T   e_brdcst_medium;
} SSU_OPEN_SETTING_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFO_COMPATIBILITY_T
 *  @brief  Used to inform AP of HW/SW Model/Version
 *  @code
 *  typedef struct
 *  {
 *      UINT8 ui1_hw_sw_flag; 
 *      UINT16 ui2_model;
 *      UINT16 ui2_version;
 *  } SSU_INFO_COMPATIBILITY_T;
 *  @endcode
 *  @li@c  ui1_hw_sw_flag - Indicate if the info is for hardware or software.
 *  @li@c  ui2_model      - Indicate the model number.
 *  @li@c  ui2_version    - Indicate the version number.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8 ui1_hw_sw_flag; /* 0: hw, 1: sw*/
    UINT16 ui2_model;
    UINT16 ui2_version;
} SSU_INFO_COMPATIBILITY_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_DOWNLOAD_INFO_T
 *  @brief  Inform AP of download information.
 *  @code
 *  typedef struct
 *  {
 *      UINT32  ui4_file_id;
 *      SIZE_T  z_buf_len;
 *      UINT8   *pui1_buf;
 *      SIZE_T  z_done_byte;
 *      SIZE_T  z_total_byte;
 *  }SSU_DOWNLOAD_INFO_T;
 *  @endcode
 *  @li@c  ui4_file_id  - Downloaded File ID
 *  @li@c  z_buf_len    - Buffer length of the file data.
 *  @li@c  *pui1_buf    - Buffer pointer of the file data.
 *  @li@c  z_done_byte  - Number of bytes downloaded.
 *  @li@c  z_total_byte - Total number of bytes in the file.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32  ui4_file_id;
    SIZE_T  z_buf_len;
    UINT8   *pui1_buf;
    SIZE_T  z_done_byte;
    SIZE_T  z_total_byte;
}SSU_DOWNLOAD_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum  SSU_COND_T
 *  @brief Notification type to AP.
 *  @code
 *  typedef enum
 *  {
 *      SSU_INFORM_FILE_FOUND = 1,
 *      SSU_INFORM_FILE_OUT_OF_TS,
 *      SSU_INFORM_FILE_MATCHED,
 *      SSU_INFORM_SSU_OAD_NOT_FOUND,
 *      SSU_INFORM_OUT_OF_RESOURCES,
 *      SSU_INFORM_CANDIDATE_FOUND,
 *      SSU_INFORM_PRIVATE_OUI,
 *      SSU_INFORM_READY_TO_DOWNLOAD,
 *      SSU_INFORM_LINKAGE,
 *      SSU_INFORM_SCHEDULE,        
 *      SSU_INFORM_UPDATE,          
 *      SSU_INFORM_LOCATION,        
 *      SSU_INFORM_MESSAGE,         
 *      SSU_INFORM_EVENT_NAME,      
 *      SSU_INFORM_TELEPHONE,       
 *      SSU_INFORM_COM_PRIVATE,     
 *      SSU_INFORM_OPE_SUB_GROUP,   
 *      SSU_INFORM_OPE_PRIVATE,     
 *      SSU_INFORM_END,
 *      SSU_COND_PRIVATE,
 *      SSU_COND_LINKAGE,
 *      SSU_COND_SELECTOR_BYTE,
 *      SSU_COND_COMPATIBILITY,
 *      SSU_COND_GROUP_INFO,
 *      SSU_COND_MODULE_INFO,
 *      SSU_COND_UKOAD_MANUFACTURER_ID,
 *      SSU_COND_UKOAD_MANUFACTURER_VERSION_ID,
 *      SSU_COND_UKOAD_PRIVATE,
 *      SSU_COND_UKOAD_DIR,
 *      SSU_COND_UKOAD_FILE,
 *      SSU_COND_TGT_SMART_CARD,    
 *      SSU_COND_TGT_MAC,           
 *      SSU_COND_TGT_SERIAL_NUM,    
 *      SSU_COND_TGT_IP,            
 *      SSU_COND_TGT_IPV6,          
 *      SSU_COND_TGT_PRIVATE,       
 *      SSU_COND_END
 *  } SSU_COND_T;
 *  @endcode
 *  @li@c  SSU_INFORM_FILE_FOUND                  - Inform AP that file is found.
 *  @li@c  SSU_INFORM_FILE_OUT_OF_TS              - Inform AP that file is not transmitted in this TS.
 *  @li@c  SSU_INFORM_FILE_MATCHED                - Inform AP that a file matches AP's criteria.
 *  @li@c  SSU_INFORM_SSU_OAD_NOT_FOUND           - Inform AP that file is not found.
 *  @li@c  SSU_INFORM_OUT_OF_RESOURCES            - Inform AP that there is not enough resource to continue.
 *  @li@c  SSU_INFORM_CANDIDATE_FOUND             - Inform AP that a candidate is found.
 *  @li@c  SSU_INFORM_PRIVATE_OUI                 - Inform AP that a private OUI is found.
 *  @li@c  SSU_INFORM_READY_TO_DOWNLOAD           - Inform AP that SSU is ready to download file.
 *  @li@c  SSU_INFORM_LINKAGE                     - Inform AP that a linkage descriptor is found.
 *  @li@c  SSU_INFORM_SCHEDULE                    - Inform AP that a schedule descriptor is found.
 *  @li@c  SSU_INFORM_UPDATE                      - Inform AP that a update descriptor is found.
 *  @li@c  SSU_INFORM_LOCATION                    - Inform AP that a location descriptor is found.
 *  @li@c  SSU_INFORM_MESSAGE                     - Inform AP that a message descriptor is found.
 *  @li@c  SSU_INFORM_EVENT_NAME                  - Inform AP that a event name descriptor is found.
 *  @li@c  SSU_INFORM_TELEPHONE                   - Inform AP that a telephone descriptor is found.
 *  @li@c  SSU_INFORM_COM_PRIVATE                 - Inform AP that a private data descriptor is found.
 *  @li@c  SSU_INFORM_OPE_SUB_GROUP               - Inform AP that a sub group descriptor is found.
 *  @li@c  SSU_INFORM_OPE_PRIVATE                 - Inform AP that a private descriptor is found.
 *  @li@c  SSU_INFORM_END                         - End of Inform types
 *  @li@c  SSU_COND_PRIVATE                       - Inform AP that a private descriptor is found.
 *  @li@c  SSU_COND_LINKAGE                       - Inform AP that a linkage descriptor is found.
 *  @li@c  SSU_COND_SELECTOR_BYTE                 - Inform AP that selector bytes are found.
 *  @li@c  SSU_COND_COMPATIBILITY                 - Inform AP that a compatibility descriptor is found.
 *  @li@c  SSU_COND_GROUP_INFO                    - Not used
 *  @li@c  SSU_COND_MODULE_INFO                   - Not used
 *  @li@c  SSU_COND_UKOAD_MANUFACTURER_ID         - Inform AP that a UKOAD manufacturer ID is found.
 *  @li@c  SSU_COND_UKOAD_MANUFACTURER_VERSION_ID - Inform AP that a UKOAD manufacturer version ID is found.
 *  @li@c  SSU_COND_UKOAD_PRIVATE                 - Inform AP that UKOAD private data is found.
 *  @li@c  SSU_COND_UKOAD_DIR                     - Inform AP that a UKOAD directory is found.
 *  @li@c  SSU_COND_UKOAD_FILE                    - Inform AP that a UKOAD file is found.
 *  @li@c  SSU_COND_TGT_SMART_CARD                - Inform AP that a smart card descriptor is found.
 *  @li@c  SSU_COND_TGT_MAC                       - Inform AP that a MAC address descriptor is found.
 *  @li@c  SSU_COND_TGT_SERIAL_NUM                - Inform AP that a serial number descriptor is found.
 *  @li@c  SSU_COND_TGT_IP                        - Inform AP that a IP descriptor is found.
 *  @li@c  SSU_COND_TGT_IPV6                      - Inform AP that a IPv6 descriptor is found.
 *  @li@c  SSU_COND_TGT_PRIVATE                   - Inform AP that a private descriptor is found.
 *  @li@c  SSU_COND_END                           - End
 */
/*------------------------------------------------------------------*/
typedef enum
{
    /* SSU/OAD Inform Types */
    SSU_INFORM_FILE_FOUND = 1,
    SSU_INFORM_FILE_OUT_OF_TS,
    SSU_INFORM_FILE_MATCHED,
    SSU_INFORM_SSU_OAD_NOT_FOUND,
    SSU_INFORM_OUT_OF_RESOURCES,
    SSU_INFORM_CANDIDATE_FOUND,
    SSU_INFORM_PRIVATE_OUI,
    SSU_INFORM_READY_TO_DOWNLOAD,
    SSU_INFORM_LINKAGE,
    SSU_INFORM_EXTEND_TIMER,

    SSU_INFORM_TOTAL_NUM_OF_GROUPS,
    SSU_INFORM_GROUP_NUM,

    /* SSU Enhanced Profile - Informs */
    SSU_INFORM_SCHEDULE,        /* Common and Operational Descriptors */
    SSU_INFORM_UPDATE,          /* Common and Operational Descriptors */
    SSU_INFORM_LOCATION,        /* Common and Operational Descriptors */
    SSU_INFORM_MESSAGE,         /* Common and Operational Descriptors */
    SSU_INFORM_EVENT_NAME,      /* Common and Operational Descriptors */
    SSU_INFORM_TELEPHONE,       /* Common and Operational Descriptors */
    SSU_INFORM_COM_PRIVATE,     /* Common Descriptors */
    SSU_INFORM_OPE_SUB_GROUP,   /* Operational Descriptors */
    SSU_INFORM_OPE_PRIVATE,     /* Operational Descriptors */

    SSU_INFORM_END,

    /* SSU/OAD Condition Types */
    SSU_COND_PRIVATE,
    SSU_COND_LINKAGE,
    SSU_COND_SELECTOR_BYTE,
    SSU_COND_COMPATIBILITY,
    SSU_COND_GROUP_INFO,
    SSU_COND_MODULE_INFO,
    SSU_COND_UKOAD_MANUFACTURER_ID,
    SSU_COND_UKOAD_MANUFACTURER_VERSION_ID,
    SSU_COND_UKOAD_PRIVATE,
    SSU_COND_UKOAD_DIR,
    SSU_COND_UKOAD_FILE,

    /* SSU Enhanced Profile - Conditions */
    SSU_COND_TGT_SMART_CARD,    /* Target Descriptors */
    SSU_COND_TGT_MAC,           /* Target Descriptors */
    SSU_COND_TGT_SERIAL_NUM,    /* Target Descriptors */
    SSU_COND_TGT_IP,            /* Target Descriptors */
    SSU_COND_TGT_IPV6,          /* Target Descriptors */
    SSU_COND_TGT_PRIVATE,       /* Target Descriptors */

    /* added by 2010.03.29 to support FreeView HD for multiple mis*/
    SSU_COND_UKOAD_MIS_IDS,
    SSU_COND_MULTIPLE_MIS_SUPPORT,

    SSU_COND_END
} SSU_COND_T;

/*------------------------------------------------------------------*/
/*! @brief  Notify function pointer.
 *  @param  [in]  h_dld   - Handle of the download.
 *  @param  [in]  pv_tag  - Private tag.
 *  @param  [in]  e_cond  - Notification condition.
 *  @param  [in]  pv_data - Notification data related to the notification condition.
 *  @note   User should implement such function pointer and register to SSU.
 *  @see    c_ssum_open, c_ssum_open_by_type_nid
 *  @return Return the error code.
 *  @retval SSU_COND_MISMATCHED        - Inform SSU engine that condition is mismatched.
 *  @retval SSU_COND_MATCHED           - Inform SSU engine that condition is matched.
 *  @retval SSU_COND_MATCHED_AND_NEXT  - Inform SSU engine that condition is matched, and keep parsing.
 *  @retval SSU_COND_IGNORE_LINKAGE    - Inform SSU engine to ignore linkage descriptor.
 *  @retval SSU_COND_WAIT_PROC_LINKAGE - Inform SSU engine to wait for processing of linkage descriptor.
 *  @retval SSU_INFORM_OK              - Inform SSU engine that condition is OK.
 *  @retval SSU_INFORM_ERROR           - Inform SSU engine that condition is ERROR.
 */
/*------------------------------------------------------------------*/
typedef SSU_NFY_RET_T (*x_ssum_nfy_fct)(
    HANDLE_T h_dld,
    VOID    *pv_tag,
    SSU_COND_T e_cond,
    VOID    *pv_data);

/*------------------------------------------------------------------*/
/*! @enum  SSU_DOWNLOAD_COND_T
 *  @brief Condition to let AP know the status of the download.
 *  @code
 *  typedef enum
 *  {
 *      DOWNLOAD_FAIL_UNKNOWN = -100,
 *      DOWNLOAD_FAIL_RESOURCE_NOT_ENOUGH,
 *      DOWNLOAD_FAIL_BUF_SIZE_NOT_ENOUGH = -1,
 *      DOWNLOAD_OK = 0,
 *      DOWNLOAD_PROGRESS
 *  } SSU_DOWNLOAD_COND_T;
 *  @endcode
 *  @li@c  DOWNLOAD_FAIL_UNKNOWN             - Download failed for unknown reasion.
 *  @li@c  DOWNLOAD_FAIL_RESOURCE_NOT_ENOUGH - Download failed due to not enough resource.
 *  @li@c  DOWNLOAD_FAIL_BUF_SIZE_NOT_ENOUGH - Download failed due to insufficient buffer size.
 *  @li@c  DOWNLOAD_OK                       - Download OK.
 *  @li@c  DOWNLOAD_PROGRESS                 - Download progress.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DOWNLOAD_FAIL_UNKNOWN = -100,
    DOWNLOAD_FAIL_RESOURCE_NOT_ENOUGH,
    DOWNLOAD_FAIL_BUF_SIZE_NOT_ENOUGH = -1,
    DOWNLOAD_OK = 0,
    DOWNLOAD_PROGRESS
} SSU_DOWNLOAD_COND_T;

/*------------------------------------------------------------------*/
/*! @brief  Call back function pointer to notify download condition.
 *  @param  [in] h_dld       - Handle of the download.
 *  @param  [in] pv_tag      - Private tag.
 *  @param  [in] e_cond      - Download condition.
 *  @param  [in] pt_dld_info - Additional info related to download condition.
 *  @note   User should implement such function pointer and register to SSU.
 *  @see    c_ssum_download_by_id, c_ssum_download_by_name
 *  @return Return the error code.
 *  @retval Not used.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_ssum_download_nfy_fct)(
    HANDLE_T                h_dld, /* handle to dld state machine */
    VOID                    *pv_tag,
    SSU_DOWNLOAD_COND_T     e_cond,
    SSU_DOWNLOAD_INFO_T     *pt_dld_info);

/* app -> ssu download by id/filename */
/*------------------------------------------------------------------*/
/*! @struct SSU_DOWNLOAD_SETTING_T
 *  @brief
 *  @code
 *  typedef struct
 *  {
 *      UINT32	ui4_file_id;
 *  	CHAR	ac_filename[256];
 *  	SIZE_T  z_buf_len;
 *      UINT8   *pui1_buf;
 *      UINT32  ui4_offset;
 *      x_ssum_download_nfy_fct pf_download_nfy_fct;
 *  } SSU_DOWNLOAD_SETTING_T;
 *  @endcode
 *  @li@c  ui4_file_id         - ID of the file to download.
 *  @li@c  ac_filename[256]    - File name of the file to download.
 *  @li@c  z_buf_len           - Buffer length available.
 *  @li@c  *pui1_buf           - Pointer to the buffer.
 *  @li@c  ui4_offset          - File offset within the file to download from.
 *  @li@c  pf_download_nfy_fct - Download status notification function.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32	ui4_file_id;
	CHAR	ac_filename[256];
	SIZE_T  z_buf_len;
    UINT8   *pui1_buf;
    UINT32  ui4_offset;
    /*
    x_ssum_download_nfy_fct pf_download_nfy_fct;
    */
} SSU_DOWNLOAD_SETTING_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_FILE_FOUND_T
 *  @brief  Inform file is found in SSU/OAD bitstream.
 *  @code
 *  typedef struct
 *  {
 *      UINT32  ui4_file_idx;
 *      UINT32  ui4_file_size;
 *      SIZE_T  z_file_name_size;
 *      CHAR    *s_file_name;
 *      UINT16  ui2_sw_model;
 *      UINT16  ui2_sw_version;
 *      UINT16  ui2_hw_model;
 *      UINT16  ui2_hw_version;   
 *  } SSU_INFORM_FILE_FOUND_T;
 *  @endcode
 *  @li@c  ui4_file_idx     - Index of the file.
 *  @li@c  ui4_file_size    - Size of the file.
 *  @li@c  z_file_name_size - Size of the file name string.
 *  @li@c  *s_file_name     - File name.
 *  @li@c  ui2_sw_model     - Software model of the file.
 *  @li@c  ui2_sw_version   - Software version of the file.
 *  @li@c  ui2_hw_model     - Hardware model of the file.
 *  @li@c  ui2_hw_version   - Hardware version of the file.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32  ui4_file_idx;
    UINT32  ui4_file_size;
    SIZE_T  z_file_name_size;
    CHAR    *s_file_name;

    UINT16  ui2_sw_model;
    UINT16  ui2_sw_version;
    UINT16  ui2_hw_model;
    UINT16  ui2_hw_version;   
} SSU_INFORM_FILE_FOUND_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_EXTEND_TIMER_T
 *  @brief  Inform AP to extend timer.
 *  @code
 *  typedef struct
 *  {
 *      UINT32  ui4_time_out_len;
 *  } SSU_INFORM_FILE_FOUND_T;
 *  @endcode
 *  @li@c  ui4_time_out_len   - Timeout value in milli-seconds.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32  ui4_time_out_len;  
} SSU_INFORM_EXTEND_TIMER_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_FILE_OUT_OF_TS_T
 *  @brief  Used to inform APP that file is transmitted in another TS.
 *  @code
 *  typedef struct
 *  {
 *      UINT16    ui2_original_network_id;
 *      UINT16    ui2_ts_id;
 *      UINT16    ui2_program_num;
 *  } SSU_INFORM_FILE_OUT_OF_TS_T;
 *  @endcode
 *  @li@c  ui2_original_network_id - ONID of the other TS.
 *  @li@c  ui2_ts_id               - TSID of the other TS.
 *  @li@c  ui2_program_num         - Program number within the other TS.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT16    ui2_original_network_id;
    UINT16    ui2_ts_id;
    UINT16    ui2_program_num;
} SSU_INFORM_FILE_OUT_OF_TS_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_TOTAL_NUM_OF_GROUPS_T
 *  @brief  Used to inform APP the total number of groups in this DSI.
 *  @code
 *  typedef struct
 *  {
 *      UINT16    ui2_total_num_of_groups;
 *  } SSU_INFORM_TOTAL_NUM_OF_GROUPS_T;
 *  @endcode
 *  @li@c  ui2_total_num_of_groups - Total number of groups.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT16    ui2_total_num_of_groups;
} SSU_INFORM_TOTAL_NUM_OF_GROUPS_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_GROUP_NUM_T
 *  @brief  Used to inform APP the group number being checked.
 *  @code
 *  typedef struct
 *  {
 *      UINT16    ui2_group_num;
 *  } SSU_INFORM_GROUP_NUM_T;
 *  @endcode
 *  @li@c  ui2_group_num - Group number being checked.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT16    ui2_group_num;
} SSU_INFORM_GROUP_NUM_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_COND_LINKAGE_T
 *  @brief  Used to inform APP that file is transmitted in another TS.
 *  @code
 *  typedef struct
 *  {
 *      UINT16    ui2_original_network_id;
 *      UINT16    ui2_ts_id;
 *      UINT16    ui2_program_num;
 *  } SSU_COND_LINKAGE_T;
 *  @endcode
 *  @li@c  ui2_original_network_id - ONID of the other TS.
 *  @li@c  ui2_ts_id               - TSID of the other TS.
 *  @li@c  ui2_program_num         - Program number within the other TS.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT16    ui2_original_network_id;
    UINT16    ui2_ts_id;
    UINT16    ui2_program_num;
} SSU_COND_LINKAGE_T;

/*------------------------------------------------------------------*/
/*! @struct DOWNLOAD_PROGRESS_T
 *  @brief  Used to inform APP of the download progress.
 *  @code
 *  typedef struct
 *  {
 *      UINT32  ui4_file_id;
 *      UINT32  ui4_curr;
 *      UINT32  ui4_total;
 *  } DOWNLOAD_PROGRESS_T;
 *  @endcode
 *  @li@c  ui4_file_id - File ID
 *  @li@c  ui4_curr    - Number of bytes downloaded.
 *  @li@c  ui4_total   - Total number of bytes to download.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32  ui4_file_id;
    UINT32  ui4_curr;
    UINT32  ui4_total;
} DOWNLOAD_PROGRESS_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_COND_PRIVATE_T
 *  @brief  Used to inform APP of the private data.
 *  @code
 *  typedef struct 
 *  {
 *      SIZE_T    z_size;
 *      UINT8*    pui1_data;
 *  } SSU_COND_PRIVATE_T;
 *  @endcode
 *  @li@c  z_size    - Size of the private data.
 *  @li@c  pui1_data - Private data.
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    SIZE_T    z_size;
    UINT8*    pui1_data;
} SSU_COND_PRIVATE_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_COND_SELECTOR_BYTE_T
 *  @brief  Used to inform APP of the selector bytes.
 *  @code
 *  typedef struct
 *  {
 *      SIZE_T z_size;
 *      UINT8* pui1_selector_byte;
 *  } SSU_COND_SELECTOR_BYTE_T;
 *  @endcode
 *  @li@c  z_size             - Size of the selector bytes.
 *  @li@c  pui1_selector_byte - Selector bytes.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    SIZE_T z_size;
    UINT8* pui1_selector_byte;
} SSU_COND_SELECTOR_BYTE_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_COND_COMPATIBILITY_T
 *  @brief  Used to inform APP of the compatibility information.
 *  @code
 *  typedef struct
 *  {
 *      UINT8   ui1_hw_sw_flag; 
 *      UINT16  ui2_model;
 *      UINT16  ui2_version;
 *      UINT8   *pui1_selector_byte;
 *  } SSU_COND_COMPATIBILITY_T;
 *  @endcode
 *  @li@c  ui1_hw_sw_flag      - Indicates if this is for hardware or software.
 *  @li@c  ui2_model           - Hardware or software model.
 *  @li@c  ui2_version         - Hardware or software version.
 *  @li@c  *pui1_selector_byte - Selector bytes.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8   ui1_hw_sw_flag; /*0x1: hw, 0x2: sw*/
    UINT16  ui2_model;
    UINT16  ui2_version;
    UINT8   *pui1_selector_byte;
} SSU_COND_COMPATIBILITY_T;

#if 0
/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct
 *  {
 *      SIZE_T    z_size;
 *      UINT8    *pui1_data;
 *  } SSU_COND_GROUP_INFO_T;
 *  @endcode
 *  @li@c  z_size                                            -
 *  @li@c  *pui1_data                                        -
 */
/*------------------------------------------------------------------*/
typedef struct
{
    SIZE_T    z_size;
    UINT8    *pui1_data;
} SSU_COND_GROUP_INFO_T;
#endif

#if 0
/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct
 *  {
 *      UINT16    ui2_module_id;
 *      UINT16    ui2_module_version;
 *      SIZE_T    z_size;
 *      UINT8    *pui1_data;
 *  } SSU_COND_MODULE_INFO_T;
 *  @endcode
 *  @li@c  ui2_module_id                                     -
 *  @li@c  ui2_module_version                                -
 *  @li@c  z_size                                            -
 *  @li@c  *pui1_data                                        -
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT16    ui2_module_id;
    UINT16    ui2_module_version;
    SIZE_T    z_size;
    UINT8    *pui1_data;
} SSU_COND_MODULE_INFO_T;
#endif

/* ------------------------------------------------------------- */
/*                            UK-OAD                             */
/*------------------------------------------------------------------*/
/*! @struct SSU_COND_UKOAD_PRIVATE_T
 *  @brief  Used to inform APP of UK-OAD private data.
 *  @code
 *  typedef struct
 *  {
 *      SIZE_T    z_size;
 *      const UINT8    *pui1_data;
 *  } SSU_COND_UKOAD_PRIVATE_T;
 *  @endcode
 *  @li@c  z_size     - Size of the private data.
 *  @li@c  *pui1_data - Private data.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    SIZE_T    z_size;
    const UINT8    *pui1_data;
} SSU_COND_UKOAD_PRIVATE_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_COND_UKOAD_DIR_T
 *  @brief  Used to inform APP of UK-OAD directory
 *  @code
 *  typedef struct
 *  {
 *      SIZE_T    z_dir_name_size;
 *      CHAR    *s_name;
 *      UINT32    ui4_idx;
 *  } SSU_COND_UKOAD_DIR_T;
 *  @endcode
 *  @li@c  z_dir_name_size - Size of the directory name.
 *  @li@c  *s_name         - Directory name.
 *  @li@c  ui4_idx         - Index of the directory.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    SIZE_T    z_dir_name_size;
    CHAR    *s_name;
    UINT32    ui4_idx;
} SSU_COND_UKOAD_DIR_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_COND_UKOAD_FILE_T
 *  @brief  Used to inform APP of UK-OAD file
 *  @code
 *  typedef struct
 *  {
 *      SIZE_T    z_dir_name_size;
 *      CHAR    *s_name;
 *      SIZE_T    z_file_size;
 *  } SSU_COND_UKOAD_FILE_T;
 *  @endcode
 *  @li@c  z_dir_name_size - Size of the file name.
 *  @li@c  *s_name         - File name.
 *  @li@c  z_file_size     - File size.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    SIZE_T    z_dir_name_size;
    CHAR    *s_name;
    SIZE_T    z_file_size;
} SSU_COND_UKOAD_FILE_T;
/* ------------------------------------------------------------- */


/* ------------------------------------------------------------- */
/*                    SSU Enhanced Profile                       */
/* Common and Operational Descriptors */
/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_SCHEDULING_T
 *  @brief  Used to inform SSU of schedule information.
 *  @code
 *  typedef struct  
 *  {
 *      UINT8   aui1_start_date_time[5];
 *      UINT8   aui1_end_date_time[5];
 *      BOOL    b_final_avail;
 *      BOOL    b_periodic_flag;
 *      UINT8   ui1_periodic_unit;         
 *      UINT8   ui1_duration_unit;         
 *      UINT8   ui1_cycle_time_unit;       
 *      UINT8   ui1_period;
 *      UINT8   ui1_duration;
 *      UINT8   ui1_cycle_time;
 *      SIZE_T  z_private_data_size;
 *      UINT8*  pui1_private_data;
 *  } SSU_INFORM_SCHEDULING_T;
 *  @endcode
 *  @li@c  aui1_start_date_time[5] - Start date & time when SSU bitstream is transmitted.
 *  @li@c  aui1_end_date_time[5]   - End date & time when SSU bitstream is transmitted.
 *  @li@c  b_final_avail           - If this update is the final availability.
 *  @li@c  b_periodic_flag         - If this update is transmitted periodically.
 *  @li@c  ui1_periodic_unit       - Unit of the period.
 *  @li@c  ui1_duration_unit       - Unit of the duration of the update from the beginning of a period.
 *  @li@c  ui1_cycle_time_unit     - Unit of the cycle time of each update.
 *  @li@c  ui1_period              - Length of the period in period unit.
 *  @li@c  ui1_duration            - Length of the duration in duration unit.
 *  @li@c  ui1_cycle_time          - Length of the cycle time in cycle time unit.
 *  @li@c  z_private_data_size     - Size of the private data.
 *  @li@c  pui1_private_data       - Private data.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_INFORM_SCHEDULING */
{
    UINT8   aui1_start_date_time[5];
    UINT8   aui1_end_date_time[5];
    BOOL    b_final_avail;
    BOOL    b_periodic_flag;
    UINT8   ui1_periodic_unit;         /* bottom 2 bits */
    UINT8   ui1_duration_unit;         /* bottom 2 bits */
    UINT8   ui1_cycle_time_unit;       /* bottom 2 bits */
    UINT8   ui1_period;
    UINT8   ui1_duration;
    UINT8   ui1_cycle_time;
    SIZE_T  z_private_data_size;
    UINT8*  pui1_private_data;
} SSU_INFORM_SCHEDULING_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_UPDATE_T
 *  @brief  Used to inform APP of SSU update.
 *  @code
 *  typedef struct  
 *  {
 *      UINT8   ui1_update_flag;            
 *      UINT8   ui1_update_method;          
 *      UINT8   ui1_update_priority;        
 *      SIZE_T  z_private_data_size;
 *      UINT8*  pui1_private_data;
 *  } SSU_INFORM_UPDATE_T;
 *  @endcode
 *  @li@c  ui1_update_flag     - Indicates if the update should be done automaticlly or manually.
 *  @li@c  ui1_update_method   - Indiates when the update should be done.
 *  @li@c  ui1_update_priority - Indicates the priority of the update.
 *  @li@c  z_private_data_size - Private data size.
 *  @li@c  pui1_private_data   - Private data.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_INFORM_UPDATE */
{
    UINT8   ui1_update_flag;            /* bottom 2 bits */
    UINT8   ui1_update_method;          /* bottom 4 bits */
    UINT8   ui1_update_priority;        /* bottom 2 bits */
    SIZE_T  z_private_data_size;
    UINT8*  pui1_private_data;
} SSU_INFORM_UPDATE_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_SSU_LOCATION
 *  @brief  Used to record the location of SSU update within this TS.
 *  @code
 *  typedef struct  
 *  {
 *      UINT16  ui2_data_broadcast_id;
 *      UINT16  ui2_association_tag;
 *      SIZE_T  z_private_data_size;
 *      UINT8*  pui1_private_data;
 *  } SSU_INFORM_SSU_LOCATION;
 *  @endcode
 *  @li@c  ui2_data_broadcast_id - Data broadcast ID of the update.
 *  @li@c  ui2_association_tag   - Least significant byte indicates the comopnent tag of the update.
 *  @li@c  z_private_data_size   - Size of the private data.
 *  @li@c  pui1_private_data     - Private data.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_INFORM_SSU_LOCATION */
{
    UINT16  ui2_data_broadcast_id;
    UINT16  ui2_association_tag;
    SIZE_T  z_private_data_size;
    UINT8*  pui1_private_data;
} SSU_INFORM_SSU_LOCATION;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_MESSAGE_T
 *  @brief  Used to inform APP of the message descriptor.
 *  @code
 *  typedef struct  
 *  {
 *      UINT8   ui1_desc_number;            
 *      UINT8   ui1_last_desc_number;       
 *      UINT8   aui1_iso_639_lang[3];
 *      UINT8   ui1_text_length;
 *      UINT8*  pui1_text;
 *  } SSU_INFORM_MESSAGE_T;
 *  @endcode
 *  @li@c  ui1_desc_number      - Descriptor number.
 *  @li@c  ui1_last_desc_number - Last descriptor number.
 *  @li@c  aui1_iso_639_lang[3] - Language of the message.
 *  @li@c  ui1_text_length      - Message text length.
 *  @li@c  pui1_text            - Message text.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_INFORM_MESSAGE */
{
    UINT8   ui1_desc_number;            /* bottom 4 bits */
    UINT8   ui1_last_desc_number;       /* bottom 4 bits */
    UINT8   aui1_iso_639_lang[3];
    UINT8   ui1_text_length;
    UINT8*  pui1_text;
} SSU_INFORM_MESSAGE_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_EVENT_NAME_T
 *  @brief  Used to inform APP of the event name.
 *  @code
 *  typedef struct  
 *  {
 *      UINT8   aui1_iso_639_lang[3];
 *      UINT8   ui1_name_length;
 *      UINT8*  pui1_name;
 *      UINT8   ui1_text_length;
 *      UINT8*  pui1_text;
 *  } SSU_INFORM_EVENT_NAME_T;
 *  @endcode
 *  @li@c  aui1_iso_639_lang[3] - Language of the event name.
 *  @li@c  ui1_name_length      - Length of the event name.
 *  @li@c  pui1_name            - Event name.
 *  @li@c  ui1_text_length      - Length of the text.
 *  @li@c  pui1_text            - Text.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_INFORM_EVENT_NAME */
{
    UINT8   aui1_iso_639_lang[3];
    UINT8   ui1_name_length;
    UINT8*  pui1_name;
    UINT8   ui1_text_length;
    UINT8*  pui1_text;
} SSU_INFORM_EVENT_NAME_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_TELEPHONE_T
 *  @brief  Used to inform APP of the telephone information.
 *  @code
 *  typedef struct  
 *  {
 *      BOOL    b_foreign_avail;
 *      UINT8   ui1_connection_type;                
 *      UINT8   ui1_country_prefix_length;          
 *      UINT8   ui1_international_area_code_length; 
 *      UINT8   ui1_operator_code_length;           
 *      UINT8   ui1_national_area_code_length;      
 *      UINT8   ui1_core_number_length;             
 *      UINT8*  pui1_country_prefix;
 *      UINT8*  pui1_international_area_code;
 *      UINT8*  pui1_operator_code;
 *      UINT8*  pui1_national_area_code;
 *      UINT8*  pui1_core_number;
 *  } SSU_INFORM_TELEPHONE_T;
 *  @endcode
 *  @li@c  b_foreign_avail                    - Indicates if the number can be called outside of the country.
 *  @li@c  ui1_connection_type                - Indicates the connection type.
 *  @li@c  ui1_country_prefix_length          - Length of the country prefix.
 *  @li@c  ui1_international_area_code_length - Lengtho of the international area code.
 *  @li@c  ui1_operator_code_length           - Length of the operator code.
 *  @li@c  ui1_national_area_code_length      - Length of the national area code.
 *  @li@c  ui1_core_number_length             - Length of the core number.
 *  @li@c  pui1_country_prefix                - Country prefix.
 *  @li@c  pui1_international_area_code       - International area code.
 *  @li@c  pui1_operator_code                 - Operator code.
 *  @li@c  pui1_national_area_code            - National area code.
 *  @li@c  pui1_core_number                   - Core number.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_INFORM_TELEPHONE */
{
    BOOL    b_foreign_avail;
    UINT8   ui1_connection_type;                /* bottom 5 bits */
    UINT8   ui1_country_prefix_length;          /* bottom 2 bits */
    UINT8   ui1_international_area_code_length; /* bottom 3 bits */
    UINT8   ui1_operator_code_length;           /* bottom 2 bits */
    UINT8   ui1_national_area_code_length;      /* bottom 3 bits */
    UINT8   ui1_core_number_length;             /* bottom 4 bits */
    UINT8*  pui1_country_prefix;
    UINT8*  pui1_international_area_code;
    UINT8*  pui1_operator_code;
    UINT8*  pui1_national_area_code;
    UINT8*  pui1_core_number;
} SSU_INFORM_TELEPHONE_T;


/* For use with SSU_INFORM_COM_PRIVATE, 
                SSU_INFORM_OPE_PRIVATE */
/*------------------------------------------------------------------*/
/*! @struct SSU_INFORM_PRIVATE_T
 *  @brief  Used to inform APP of the private data.
 *  @code
 *  typedef struct
 *  {
 *      SIZE_T    z_size;
 *      UINT32    ui4_data;
 *  } SSU_INFORM_PRIVATE_T;
 *  @endcode
 *  @li@c  z_size   - Size of the private data.
 *  @li@c  ui4_data - Private data.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    SIZE_T    z_size;
    UINT32    ui4_data;
} SSU_INFORM_PRIVATE_T;


/* Operational Descriptors */
/*------------------------------------------------------------------*/
/*! @struct 
 *  @brief
 *  @code
 *  typedef struct  
 *  {
 *      UINT8   aui1_subgroup_tag[5];
 *  } SSU_INFORM_SUB_GROUP_ASSOC_T;
 *  @endcode
 *  @li@c  aui1_subgroup_tag[5]                              -
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_INFORM_OPE_SUB_GROUP_ASSOC */
{
    UINT8   aui1_subgroup_tag[5];
} SSU_INFORM_SUB_GROUP_ASSOC_T;

/* Target Descriptors */
/*------------------------------------------------------------------*/
/*! @struct SSU_COND_TGT_SMART_CARD_T
 *  @brief  Indicates the target smartcard information.
 *  @code
 *  typedef struct  
 *  {
 *      UINT32  ui4_super_ca_system_id;
 *      SIZE_T  z_size;
 *      UINT8*  pui1_private_data;
 *  } SSU_COND_TGT_SMART_CARD_T;
 *  @endcode
 *  @li@c  ui4_super_ca_system_id - CA system ID.
 *  @li@c  z_size                 - Size of the private data.
 *  @li@c  pui1_private_data      - Private data.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_COND_TGT_SMART_CARD */
{
    UINT32  ui4_super_ca_system_id;
    SIZE_T  z_size;
    UINT8*  pui1_private_data;
} SSU_COND_TGT_SMART_CARD_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_COND_TGT_MAC_T
 *  @brief  Used to inform APP of the target MAC address.
 *  @code
 *  typedef struct  
 *  {
 *      UINT8   aui1_mac_addr_mask[6];
 *      SIZE_T  z_size;
 *      UINT8*  pui1_mac_addr_match;
 *  } SSU_COND_TGT_MAC_T;
 *  @endcode
 *  @li@c  aui1_mac_addr_mask[6] - MAC address mask
 *  @li@c  z_size                - Size of the MAC address list.
 *  @li@c  pui1_mac_addr_match   - List of MAC addresses.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_COND_TGT_MAC_ADDRESS */
{
    UINT8   aui1_mac_addr_mask[6];
    SIZE_T  z_size;
    UINT8*  pui1_mac_addr_match;
} SSU_COND_TGT_MAC_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_CNOD_TGT_SERIAL_T
 *  @brief  Used to inform APP of the target serial number.
 *  @code
 *  typedef struct  
 *  {
 *      SIZE_T  z_size;
 *      UINT8   *pui1_serial;
 *  } SSU_CNOD_TGT_SERIAL_T;
 *  @endcode
 *  @li@c  z_size       - Size of the serial number.
 *  @li@c  *pui1_serial - Serial number.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_COND_TGT_SERIAL_NUM */
{
    SIZE_T  z_size;
    UINT8   *pui1_serial;
} SSU_CNOD_TGT_SERIAL_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_COND_TGT_IP_T
 *  @brief  Used to inform APP of the target IP address.
 *  @code
 *  typedef struct  
 *  {
 *      UINT32  ui4_ip_mask;
 *      SIZE_T  z_size;
 *      UINT8*  pui1_ip;
 *  } SSU_COND_TGT_IP_T;
 *  @endcode
 *  @li@c  ui4_ip_mask - IP address mask.
 *  @li@c  z_size      - Size of the IP addresses list.
 *  @li@c  pui1_ip     - IP address list.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_COND_TGT_IP_ADDRESS */
{
    UINT32  ui4_ip_mask;
    SIZE_T  z_size;
    UINT8*  pui1_ip;
} SSU_COND_TGT_IP_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_COND_TGT_IPV6_T
 *  @brief  Used to inform APP of the target IPv6 address.
 *  @code
 *  typedef struct  
 *  {
 *      UINT8   aui1_ipv6_mask[16];
 *      SIZE_T  z_size;
 *      UINT8*  pui1_ipv6;
 *  } SSU_COND_TGT_IPV6_T;
 *  @endcode
 *  @li@c  aui1_ipv6_mask[16] - IPv6 address mask.
 *  @li@c  z_size             - Size of the IPv6 address list.
 *  @li@c  pui1_ipv6          - IPv6 address list.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_COND_TGT_IPV6_ADDRESS */
{
    UINT8   aui1_ipv6_mask[16];
    SIZE_T  z_size;
    UINT8*  pui1_ipv6;
} SSU_COND_TGT_IPV6_T;


/*------------------------------------------------------------------*/
/*! @struct SSU_COND_UNT_PRIVATE_T
 *  @brief  Used to inform APP of SSU Enhanced profile private data.
 *  @code
 *  typedef struct  
 *  {
 *      SIZE_T    z_size;
 *      UINT32    ui4_data;
 *  } SSU_COND_UNT_PRIVATE_T;
 *  @endcode
 *  @li@c  z_size   - Size of the private data.
 *  @li@c  ui4_data - Private data.
 */
/*------------------------------------------------------------------*/
typedef struct  /* For use with SSU_COND_TGT_PRIVATE */
{
    SIZE_T    z_size;
    UINT32    ui4_data;
} SSU_COND_UNT_PRIVATE_T;
/* ------------------------------------------------------------- */

/*------------------------------------------------------------------*/
/*! @struct SSU_COND_UKOAD_MIS_IDS
 *  @brief  Used to inform APP of UKOAD MIS ID's.
 *  @code
 *  typedef struct
 *  {
 *      UINT32    ui4_manufacturer_id;
 *      UINT32    ui4_version_id;
 *      UINT32    ui4_index;
 *      UINT32    ui4_mis_size;
 *  } SSU_COND_UKOAD_MIS_IDS_T;
 *  @endcode
 *  @li@c  ui4_manufacturer_id - manufacturer id.
 *  @li@c  ui4_version_id - version id.
 *  @li@c  ui4_index - index to ui4_index-th mis data.
 *  @li@c  ui4_mis_size - mis data size.
 */

typedef struct /* For use with SSU_COND_UKOAD_MIS_IDS */
{
     UINT32    ui4_manufacturer_id;
     UINT32    ui4_version_id;
     UINT32    ui4_index;
     UINT32    ui4_msd_size;
} SSU_COND_UKOAD_MIS_IDS_T;

/*------------------------------------------------------------------*/
/*! @struct SSU_UKOAD_GET_DATA_T
 *  @brief  Used to command MW to copy MIS data to APP prepared buf
 *  @code
 *  typedef struct
 *  {
 *      SIZE_T    z_size;
 *      UINT32    ui4_data;
 *  } SSU_COND_UNT_PRIVATE_T;
 *  @endcode
 *  @li@c  z_size   - Size of the private data.
 *  @li@c  ui4_data - Private data.
 */
/*------------------------------------------------------------------*/

typedef struct 
{
    UINT32 ui4_index;
    SIZE_T sz_size;
    UINT8  *pui1_buf;
} SSU_UKOAD_GET_DATA_T;

/*------------------------------------------------------------------*/


#endif /* #endif _U_SSUM_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                         /* end of groupMW_SSU_MNGR */
/*----------------------------------------------------------------------------*/

