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
/*-----------------------------------------------------------------------------
 * $RCSfile: x_ci.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Common Interface (CI) specific
 *         definitions, which are exported.
 *-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_CI
*  @{ 
*/
/*----------------------------------------------------------------------------*/

#ifndef _X_CI_H_
#define _X_CI_H_

/*-------------------------------------------------------------------------
                    include files
-------------------------------------------------------------------------*/
#include "u_ci.h"
#include "x_rm_dev_types.h"

/*-------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ------------------------------------------------------------------------*/

#ifdef CI_PLUS_SUPPORT
typedef UINT8 CI_KEY_TYPE_T; /**< indicates ci plus key type       */
#define CIPLUS_KEY_ROOT_CERT          ((CI_KEY_TYPE_T) 0x00)/**< indicates ci plus root certificate key type       */
#define CIPLUS_KEY_BRAND_CERT         ((CI_KEY_TYPE_T) 0x01)/**< indicates ci plus brand certificate key type     */
#define CIPLUS_KEY_DEVICE_CERT        ((CI_KEY_TYPE_T) 0x02)/**<indicates ci plus device certificate key type    */
#define CIPLUS_KEY_PRNG_KEY           ((CI_KEY_TYPE_T) 0x03)/**< indicates ci plus prng seed  key type    */
#define CIPLUS_KEY_DH_P               ((CI_KEY_TYPE_T) 0x04)/**< indicates ci plus DH_P key type       */
#define CIPLUS_KEY_DH_G               ((CI_KEY_TYPE_T) 0x05)/**< indicates ci plus DH_G key type       */
#define CIPLUS_KEY_DH_Q               ((CI_KEY_TYPE_T) 0x06)/**< indicates ci plus DH_Qkey type       */
#define CIPLUS_KEY_HDQ                ((CI_KEY_TYPE_T) 0x07)/**< indicates ci plus HDQ key type       */
#define CIPLUS_KEY_SIV                ((CI_KEY_TYPE_T) 0x08)/**< indicates ci plus SIV key type       */
#define CIPLUS_KEY_CLK                ((CI_KEY_TYPE_T) 0x09)/**< indicates ci plus CLK key type       */
#define CIPLUS_KEY_SLK                ((CI_KEY_TYPE_T) 0x0A)/**< indicates ci plus SLK key type       */

#define SIZE_CIPLUS_KEY_PRNG_SEED     ((UINT16) 16)/**< define ci plus PRNG SEED key size      */
#define SIZE_CIPLUS_KEY_PRNG_KEY      ((UINT16) 16)/**< define ci plus PRNG key size      */
#define SIZE_CIPLUS_KEY_DH_P          ((UINT16) 256)/**< define ci plus DH_P key size       */
#define SIZE_CIPLUS_KEY_DH_G          ((UINT16) 256)/**< define ci plus DH_G key size       */
#define SIZE_CIPLUS_KEY_DH_Q          ((UINT16) 32)/**< define ci plus DH_QP key size       */
#define SIZE_CIPLUS_KEY_SIV           ((UINT16) 16)/**< define ci plus SIV key size       */
#define SIZE_CIPLUS_KEY_CLK           ((UINT16) 16)/**< define ci plus CLK key size       */
#define SIZE_CIPLUS_KEY_SLK           ((UINT16) 16)/**< define ci plus SLK key size       */

/* Efuse data for CI+ usage */
#define CI_PLUS_DEVICE_ID_LEN    ((UINT16)(8))/**< define ci plus device id length     */
#define CI_PLUS_SECURE_ID_LEN    ((UINT16)(16))/**< define ci plus secure id length      */
#define CI_PLUS_DEVICE_DATA_LEN  ((UINT16)(128))/**< define ci plus device data length      */
#define CI_PLUS_MAX_CUSTOM_CODE  ((UINT16)(20))/**< define ci plus maximum custom code length      */

#define CI_FILE_CREDENTIALS (SN_DEV_PATH "/" SN_FLASH_CIPLUS_1)
#endif

/*------------------------------------------------------------------*/
/*!@struct CI_CONFIG_T
 *  @brief CI config structure. 
 * typedef struct _CI_CONFIG_T
 *{
 *   UINT16  ui2_num_services;
 *   UINT16  ui2_num_comps;
 *   UINT16  ui2_num_src;
 *   DRV_TYPE_T* at_src_type; 
 *} CI_CONFIG_T;
 *  @endcode
  *  @li@c  ui2_num_services -Number of CA_SVDB node.
 *  @li@c  ui2_num_comps -Number of CA_ESDB node. 
 *  @li@c  ui2_num_src      -at_tuner_type array length
 *  @li@c  at_src_type      -tuner source array. 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CONFIG_T
{
    UINT16  ui2_num_services;
    UINT16  ui2_num_comps;
    UINT16  ui2_num_src;
    DRV_TYPE_T* at_src_type;
} CI_CONFIG_T;

#ifdef CI_PLUS_SUPPORT

/*------------------------------------------------------------------*/
/*! @enum CIPLUS_CRED_DESC_ALG_T
 *  @brief  specifies credential descrambler algorithm type
 *  @code
 *  typedef enum
 *{
 *   CIPLUS_CRED_DESC_ALG_NONE
 *   CIPLUS_CRED_DESC_ALG_AES_128_CBC
 *} CIPLUS_CRED_DESC_ALG_T;
 *  @endcode
 *  @li@c  CIPLUS_CRED_DESC_ALG_NONE            -  none algorigthm
 *  @li@c  CIPLUS_CRED_DESC_ALG_AES_128_CBC            -  algorithm is AES_128_CBC type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CIPLUS_CRED_DESC_ALG_NONE,
    CIPLUS_CRED_DESC_ALG_AES_128_CBC
}CIPLUS_CRED_DESC_ALG_T;

/*------------------------------------------------------------------*/
/*!@struct CI_PLUS_CRED_INFO_T
 *  @brief This structure defines ci plus credential infomation data.
 *typedef struct _CIPLUS_CONFIG_T
 *{
 *   BOOL b_fix_key;
 *   CIPLUS_CRED_DESC_ALG_T t_desc_alg; 
 *   union
 *   {
 *       struct
 *       {
 *            SIZE_T          z_key_len;    
 *           UINT8           aui1_key[16];
 *            SIZE_T          z_iv_len;    
 *           UINT8           aui1_iv[16];      
 *      }aes_128_cbc;
 *   }u_data;
 *    BOOL b_custom_code;       
 *   UINT8 ui1_max_len_code;   
 *  UINT8 ui1_len_code;      
 *  CHAR  *pc_custom_code;   
 *   UINT8 ui1_max_search_pattern_len;   
 *   CI_SEARCH_MODE_DATA_T t_search_mode_data;   
 *    CHAR  *pc_dst_path;    
 *    BOOL  b_rename_after_write;     
 *   UINT8 ui1_max_rename_prefix_len;  
 *   UINT8 ui1_rename_prefix_len;   
 *   CHAR  *pc_rename_prefix;       
 *    UINT8 ui1_max_serial_num_len;   
 *}CIPLUS_CONFIG_T;
 *  @endcode
 *  @li@c  b_fix_key      -contains the flag it is true if valid.
 *  @li@c  t_desc_alg      -algorithm used for encrypt the ciplus credentials.
 *  @li@c  z_key_len      -size of key in byte.
 *  @li@c  aui1_key      - pointer of key buffer
 *  @li@c  z_iv_len      -size of iv in byte 
 *  @li@c  aui1_iv      -pointer of iv buffer
 *  @li@c  b_custom_code      -valid flag of custom code setting.
 *  @li@c  ui1_max_len_code      -the maxium length of custom code
 *  @li@c  ui1_len_code      - length of custom code
 *  @li@c  pc_custom_code      -poniter of custom code 
 *  @li@c  ui1_max_search_pattern_len      -the maxium string length of search pattern 
 *  @li@c  t_search_mode_data      -search mode and pattern of files in external storage
 *  @li@c  b_preburn_cred_separated      - flag to identify to separted the store partition of preburn credentials
 *  @li@c  pc_pre_burn_dst_path      -destination file for pre-burn the ci plus data into flash
 *  @li@c  pc_dst_path      -destination file for writting ci plus data in flash
 *  @li@c  b_rename_after_write      -valid flag of rename file after written done
 *  @li@c  ui1_max_rename_prefix_len      -the maxium length of custom code 
 *  @li@c  ui1_rename_prefix_len      -length of rename prefix  
 *  @li@c  pc_rename_prefix      -pointer of string for rename prefix
 *  @li@c  ui1_max_serial_num_len      -the maxium length of serial number 
 */
/*------------------------------------------------------------------*/
typedef struct _CIPLUS_CONFIG_T
{
    BOOL b_fix_key;
    CIPLUS_CRED_DESC_ALG_T t_desc_alg; 
    union
    {
        struct
        {
            SIZE_T          z_key_len;    
            UINT8           aui1_key[16];
            SIZE_T          z_iv_len;    
            UINT8           aui1_iv[16];      
        }aes_128_cbc;
    }u_data;

    BOOL b_custom_code;       
    UINT8 ui1_max_len_code;   
    UINT8 ui1_len_code;      
    CHAR  *pc_custom_code;   

    UINT8 ui1_max_search_pattern_len;   
    CI_SEARCH_MODE_DATA_T t_search_mode_data;   

    BOOL  b_preburn_cred_separated;
    CHAR  *pc_preburn_dst_path;
    CHAR  *pc_dst_path;    
    
    BOOL  b_rename_after_write;     
    UINT8 ui1_max_rename_prefix_len;  
    UINT8 ui1_rename_prefix_len;   
    CHAR  *pc_rename_prefix;       

    UINT8 ui1_max_serial_num_len;   
}CIPLUS_CONFIG_T;
#endif

/*-------------------------------------------------------------------------
                    Functions
 ------------------------------------------------------------------------*/
#ifdef ENABLE_CA_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief config ci engine this api should be called before x_ci_init().
 *  @param [in] ui2_num_services -Number of CA_SVDB node.
 *  @param [in] ui2_num_comps -Number of CA_ESDB node. 
 *  @param [in] ui2_num_src -at_tuner_type array length
 *  @param [in] at_tuner_type -tuner source array. 
 *  @note
 *  @see c_ci_unreg_nfy
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_NOT_REGISTERED -ci engine has not registered 
 *  @retval      CIR_NOT_INIT -ci engine has not initialized 
 *  @retval      CIR_NOT_ENOUGH_SPACE -allocated memory failed
 */
/*------------------------------------------------------------------*/
extern VOID x_ci_config(UINT16  ui2_num_services, 
                   UINT16  ui2_num_comps,
                   UINT16  ui2_num_src,
                   DRV_TYPE_T *at_tuner_type);

/*------------------------------------------------------------------*/
/*! @brief initialize ci engine, called when system startup
 *  @param [in] ui2_sw_version -software version
 *  @note
 *  @see x_ci_config()
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_ALREADY_INIT -ci engine has already initialized 
 *  @retval      CIR_INIT_FAILED -ci engine initialized failed
 */
/*------------------------------------------------------------------*/
extern INT32 x_ci_init(UINT16 ui2_sw_version);

#ifdef CI_PLUS_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief config ci plus engine this api should be called before x_ci_init().
 *  @param [in] t_ciplu_cfg -configure data
 *  @note
 *  @see CIPLUS_CONFIG_T
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_NOT_REGISTERED -ci engine has not registered 
 *  @retval      CIR_NOT_INIT -ci engine has not initialized 
 *  @retval      CIR_NOT_ENOUGH_SPACE -allocated memory failed
 */
/*------------------------------------------------------------------*/
extern VOID x_ciplus_config(CIPLUS_CONFIG_T *t_ciplu_cfg);
#endif
#endif
/*------------------------------------------------------------------*/
/*! @brief register notification callback function
 *  @param [in] pv_nfy_tag -private tag for caller who will be set in x_ci_nfy_fct() callback when notify caller
 *  @param [in] pv_nfy -callback function pointer,CI engine will notify event through this function.
 *  @param [out] ph_ci -handle for this register action who will be used in x_ci_nfy_fct() and x_ci_unreg_nfy()
 *  @note
 *  @see c_ci_unreg_nfy
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_NOT_REGISTERED -ci engine has not registered 
 *  @retval      CIR_NOT_INIT -ci engine has not initialized 
 *  @retval      CIR_NOT_ENOUGH_SPACE -allocated memory failed
 */
/*------------------------------------------------------------------*/
extern INT32 x_ci_reg_nfy(
    HANDLE_T*           ph_ci,
    VOID*               pv_nfy_tag,
    UINT32              ui4_nfy_category_tag,
    x_ci_nfy_fct        pv_nfy);

/*------------------------------------------------------------------*/
/*! @brief unregister notification callback function
 *  @param [in] h_ci -handle for unregister which is set in x_ci_reg_nfy()
 *  @param [out] NONE
 *  @note
 *  @see x_ci_reg_nfy
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_NOT_REGISTERED -no related callback function registered 
 *  @retval      CIR_NOT_INIT -ci engine has not initialized 
 */
/*------------------------------------------------------------------*/

extern INT32 x_ci_unreg_nfy(
    HANDLE_T            h_ci);

#ifdef CI_PLUS_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief register query callback function
 *  @param [in] pv_tag -private tag for caller who will be set in x_ci_query_info_fct() callback when query data
 *  @param [in] pf_query_info_fct -callback function pointer,CI engine will query data through this function.
 *  @param [out] ph_ci -handle for this register action who will be used in x_ci_query_info_fct() 
 *  @note
 *  @see c_ci_unreg_nfy
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_NOT_REGISTERED -ci engine has not registered 
 *  @retval      CIR_NOT_INIT -ci engine has not initialized 
 *  @retval      CIR_NOT_ENOUGH_SPACE -allocated memory failed
 */
/*------------------------------------------------------------------*/
extern INT32 x_ci_reg_query_nfy(
    HANDLE_T*           ph_ci,
    VOID*               pv_tag,
    UINT32              ui4_nfy_category_tag,
    x_ci_query_info_fct pf_query_info_fct);
#endif


/*------------------------------------------------------------------*/
/*! @brief query CI  info
 *  @param [in] e_get_type -query type
 *  @param [in,out] pv_data -queried data, concrete type depends on e_get_type
 *  @param [in,out] pz_data_len -data's length
 *  @note
 *  @see
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_INV_GET_INFO -pv_get_info value is invalid
 *  @retval      CIR_INV_GET_DATA -pv_data value is invalid
 *  @retval      CIR_NOT_REGISTERED -no related callback function registered 
 *  @retval      CIR_NOT_INIT -ci engine has not initialized 
 *  @retval      CIR_NOT_ENOUGH_SPACE -allocated memory failed
 */
/*------------------------------------------------------------------*/

extern INT32 x_ci_get(
    CI_GET_TYPE_T       e_get_type,
    VOID*               pv_get_info,
    VOID*               pv_data,
    SIZE_T*             pz_data_len);
/*------------------------------------------------------------------*/
/*! @brief set CI  info
 *  @param [in] e_set_type -set type
 *  @param [in] pv_set_info -set info data
 *  @param [in] pv_data -set data
 *  @note
 *  @see
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_INV_SET_TYPE -e_set_type value is invalid
 *  @retval      CIR_INV_SET_INFO -pv_set_info value is invalid
 *  @retval      CIR_INV_SET_DATA -pv_data value is invalid
 *  @retval      CIR_NOT_REGISTERED -no related callback function registered 
 *  @retval      CIR_NOT_INIT -ci engine has not initialized 
 *  @retval      CIR_ALREADY_SET_ENTER_MENU -menu has been already entered
 *  @retval      CIR_FAILED -the routine is failed
 */
/*------------------------------------------------------------------*/

extern INT32 x_ci_set(
    CI_SET_TYPE_T       e_set_type,
    VOID*               pv_set_info,
    VOID*               pv_data);

#ifdef DVBT_CI_ENABLE
/*------------------------------------------------------------------*/
/*! @brief set ts path route 
 *  @param [in] b_enable_ts - True means ts data pass through CAM, False means ts data bypass CAM
 *  @note
 *  @see
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_FAILED -the routine is failed
 */
/*------------------------------------------------------------------*/
extern INT32 x_ci_set_ts_route_path(BOOL b_enable_ts);

/*------------------------------------------------------------------*/
/*! @brief get CAM if is insert on the Host
 *  @param NONE
 *  @note
 *  @see
 *  @return CAM insert state
 *  @retval      True - means CAM is inserted
 *  @retval      False - means CAM is not inserted
 */
/*------------------------------------------------------------------*/
extern BOOL x_ci_get_cam_insert(VOID);

#ifdef CI_PLUS_SUPPORT

/*API's between MHEG5 module and CI module*/

/*------------------------------------------------------------------*/
/*! @brief register mheg5 notification callback function
 *  @param [in] pv_nfy_tag -private tag for caller who will be set in x_ci_mheg5_nfy_fct() callback when notify caller
 *  @param [in] pv_nfy -callback function pointer,CI engine will notify event through this function.
 *  @param [out] ph_ci -handle for this register action who will be used in x_ci_mheg5_nfy_fct()
 *  @note
 *  @see x_ci_mheg5_nfy_fct
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_NOT_REGISTERED -ci engine has not registered 
 *  @retval      CIR_NOT_INIT -ci engine has not initialized 
 *  @retval      CIR_NOT_ENOUGH_SPACE -allocated memory failed
 */
/*------------------------------------------------------------------*/
extern INT32 x_ci_mheg5_reg_nfy(
    HANDLE_T*           ph_ci,
    VOID*               pv_nfy_tag,
    x_ci_mheg5_nfy_fct  pv_nfy);

/*------------------------------------------------------------------*/
/*! @brief set mheg5 info
 *  @param [in] e_set_type -set type
 *  @param [in] pv_data -set data
 *  @note
 *  @see CI_MHEG5_SET_TYPE
 *  @return
 *  @retval      CIR_OK -the routine is successful
 *  @retval      CIR_INV_SET_TYPE -e_set_type value is invalid
 *  @retval      CIR_INV_SET_DATA -pv_data value is invalid
 *  @retval      CIR_NOT_REGISTERED -no related callback function registered 
 *  @retval      CIR_NOT_INIT -ci engine has not initialized 
 *  @retval      CIR_FAILED -the routine is failed
 */
/*------------------------------------------------------------------*/
extern INT32 x_ci_mheg5_set(
    CI_MHEG5_SET_TYPE   e_set_type,
    VOID*               pv_data);
#endif

/*------------------------------------------------------------------*/
/*! @brief This API return the max slot number
 *  @param [in]
 *  @param [out] 
 *  @note
 *  @see
 *  @return
 *  @retval max slot number
 */
 /*------------------------------------------------------------------*/
extern UINT8 x_ci_fac_get_max_slot_num(VOID);

/*------------------------------------------------------------------*/
/*! @brief This API return the cam insert status
 *  @param [in] slot number, start from 0
 *  @param [out] pointer to keep the CAM appear status
 *  @note
 *  @see
 *  @return 
 *  @retval    CIR_OK-                  The routine was successful.
 *  @retval    CIR_FAILED-              Tthe routine is failed.
 */
 /*------------------------------------------------------------------*/
extern INT32 x_ci_fac_get_cam_appear_status(UINT8 ui1_slot, CI_APPEAR_COND_T *pt_status);

/*------------------------------------------------------------------*/
/*! @brief This API performs set TS path goes through CAM.
 *  @param [in] TRUE  - Set TS through CI module. FALSE - Set TS bypass CI module. 
 *  @note
 *  @see
 *  @return 
 *  @retval    CIR_OK-                  The routine was successful.
 *  @retval    CIR_FAILED-              Tthe routine is failed.
 */
 /*------------------------------------------------------------------*/
extern INT32 x_ci_set_force_through_cam(BOOL b_through_cam);

/*------------------------------------------------------------------*/
/*! @brief This API performs getting TS path pass or pass though CAM
 *  @param [in] 
 *  @note
 *  @see
 *  @return 
 *  @retval    TRUE  - TS through CI module. 
 *  @retval    FALSE - S bypass CI module. 
 */
 /*------------------------------------------------------------------*/
extern BOOL x_ci_get_enablets_status(VOID);

#endif

#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST)
#ifdef CI_PLUS_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief This API performs getting CI+ key store status
 *  @param [in] 
 *  @note
 *  @see
 *  @return 
 *  @retval    TRUE  - ciplus key is installed.
 *  @retval    FALSE - ciplus key store is empty.
 */
 /*------------------------------------------------------------------*/
extern BOOL x_ci_get_ciplus_key_store_status(VOID);
#endif
#endif
#endif /* _X_CI_H_ */ 

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_CI*/
/*----------------------------------------------------------------------------*/
