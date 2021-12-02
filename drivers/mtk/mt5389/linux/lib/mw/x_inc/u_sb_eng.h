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
/*! @file u_sb_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains SVL Builder specific definitions,
 *         which are exported for customized engines.
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_SB_ENG_H_
#define _U_SB_ENG_H_

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_SVL_BLDR
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_sb.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*
    (*x_sb_eng_scan_nfy_fct)() is the notify function, called by builder
    engine to notify SB Selector of the scan status (see the pf_nfy argument
    of the API sb_start_scan_fct() below).
    
    Based on this notification, SB Selector then notifies the applicaton of 
    the same status info by calling x_sb_scan_nfy_fct().
*/
/*------------------------------------------------------------------*/
/*! @brief (*x_sb_eng_scan_nfy_fct)() is the notify function, called by builder
 *  engine to notify SB Selector of the scan status (see the pf_nfy argument
 *  of the API sb_start_scan_fct() below).
 *  
 *  
 *  @param [in] h_builder_base      -Builder base handler
 *  @param [in] ui4_nfy_reason      -Notify reason
 *  @param [in] i4_nfy_data         -Notify data
 *  @param [in] pv_nfy_tag          -Notify tag
 *  @note Based on this notification, SB Selector then notifies the applicaton of the same status info by calling x_sb_scan_nfy_fct(). 
 *  @return      VOID
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_eng_scan_nfy_fct) (
      HANDLE_T          h_builder_base,
      UINT32            ui4_nfy_reason,
      INT32             i4_nfy_data,
      VOID*             pv_nfy_tag);

/*
    Followings are function types in the function table 
    structure SB_ENG_FCT_TBL_T.
 */
/*------------------------------------------------------------------*/
/*! @brief function types in the function table,(*sb_create_builder_data_fct) type function.
 *  @param [in] h_builder_base      -The handle of builder base object.
 *  @param [in] pv_open_data        -Contains the engine open data type data.
 *  @param [out]ppv_builder_data    -Contains the engine builder data type data.
 *  @param [out]pe_builder_cond     -Contains the current builder state.
 *  @return Return the error code.
 *  @retval 
 *          SBR_OK              -Success.
 *          SBR_FILE_NOT_FOUND  -Fail. The ps_file_name was not found. 
 *          SBR_INTERNAL_ERROR  -Fail. Internal error happens.         
 *          SBR_INV_ARG         -Fail. Invalid arguments.              
 *          SBR_LIST_NOT_FOUND  -Fail. The named SVL doesn't exist.    
 *          SBR_OUT_OF_MEM      -Fail. Run out of memory.              
 */               
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_create_builder_data_fct) (
     HANDLE_T           h_builder,
     VOID*              pv_open_data,
     VOID**             ppv_builder_data,
     SB_COND_T*         pe_builder_cond);

/*------------------------------------------------------------------*/
/*! @brief the (*sb_delete_builder_data_fct) type function.
 *  @param [in] pv_builder_data    -Contains the builder engine specific data.
 *  @return Return the error code.
 *  @retval SBR_OK              -Success.
 *          SBR_INTERNAL_ERROR  -Fail. Internal error happens.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_delete_builder_data_fct) (
     VOID*              pv_builder_data);

/*------------------------------------------------------------------*/
/*! @brief the (*sb_match_builder_data_fct) type function.
 *  @param [in] pv_open_data        -The first operand to matched.
 *  @param [in] pv_builder_data     -Contains the builder engine specific data, in which contains the 2nd operand to be matched.
 *                              
 *  @return Return the error code.
 *  @retval         SBR_OK              -The SVL id matches.
 *                  SBR_INV_ARG         -Fail. Invalid arguments.
 *                  SBR_NOT_MATCH       -Fail. the SVL id in the pv_open_data doesn't match the SVL id in the pv_builder_data.
 *                                       
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_match_builder_data_fct) (
     const VOID*        pv_open_data,
     const VOID*        pv_builder_data);

/*------------------------------------------------------------------*/
/*! @brief the (*sb_engine_task_fct) type function.
 *  @return
 *  @retval      SBR_OK              -Success.
 *               SBR_INTERNAL_ERROR  -Fail. Internal error happens.
 *               SBR_INV_COND        -Fail. Invalid builder condition to perform the request.
 *                                    
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_engine_task_fct) (VOID);

/*------------------------------------------------------------------*/
/*! @brief (*sb_start_scan_fct)() type function
 *  @param [in] h_builder_base  -The handle of builder base object.
 *  @param [in] pv_scan_data    -Contains the SB_ATSC_SCAN_DATA_T type data.
 *  @param [in] pv_nfy_tag      -Contains the caller private tag.
 *  @param [in] pf_nfy          -The notification function, which will be called during the scan. 
 *                          
 *  @return
 *  @retval     SBR_OK         -Success.
 *              SBR_INTERNAL_ERROR  -Fail. Internal error happens.
 *              SBR_INV_ARG         -Fail. Invalid arguments.
 *              SBR_INV_HANDLE      -Fail. Invalid builder handle.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_start_scan_fct) (
     HANDLE_T               h_builder,
     VOID*                  pv_scan_data,
     VOID*                  pv_nfy_tag,
     x_sb_eng_scan_nfy_fct  pf_nfy);

/*------------------------------------------------------------------*/
/*! @brief the (*sb_cancel_scan_fct)() type function.
 *  @param [in] h_builder_base  -The handle of builder base object.
 * 
 *  @return
 *  @retval      SBR_OK              -Success.
 *               SBR_INTERNAL_ERROR  -Fail. Internal error happens.
 *               SBR_INV_ARG         -Fail. Invalid arguments.
 *               SBR_INV_HANDLE      -Fail. Invalid builder handle.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_cancel_scan_fct) (
     HANDLE_T           h_builder);

/*------------------------------------------------------------------*/
/*! @brief the (*sb_save_lists_fct)() type function.
 *  @param [in] h_builder_base  -The handle of builder base object.
 *  @param [in] ps_file_name    -The file in which the lists are stored.
 *  
 *  @return
 *  @retval SBR_OK                   -Success.
 *          SBR_ERR_SAVE_LIST        -Fail. An error happened when saving SVL or TSL.
 *          SBR_INV_ARG              -Fail. Invalid arguments.
 *          SBR_INV_HANDLE           -Fail. Invalid builder handle.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_save_lists_fct) (
     HANDLE_T           h_builder,
     const CHAR*        ps_file_name);

/*------------------------------------------------------------------*/
/*! @brief the (*sb_load_lists_fct)() type function.
 *  @param [in] h_builder_base  -The handle of builder base object.
 *  @param [in  ps_file_name    -The file from which the lists are loaded.]
 *  
 *  @return
 *  @retval SBR_OK              -Success.
 *          SBR_ERR_LOAD_LIST   -Fail. An error happened when loading SVL or TSL. 
 *          SBR_FILE_NOT_FOUND  -Fail. The specified ps_file_name was not found.
 *          SBR_INV_ARG         -Fail. Invalid arguments.
 *          SBR_INV_HANDLE      -Fail. Invalid builder handle.
 *          SBR_LIST_NOT_FOUND  -FAIL. The SVL doesn't exist in ps_file_name.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_load_lists_fct) (
     HANDLE_T           h_builder,
     const CHAR*        ps_file_name);

/*------------------------------------------------------------------*/
/*! @brief the (*sb_clean_lists_fct)() type function.
 *  @param [in] h_builder_base  -The handle of builder base object.
 *  @param [in] ps_file_name    -The file in which the lists are stored.
 *  
 *  @return
 *  @retval SBR_OK              -Success.
 *          SBR_ERR_CLEAN_LIST  -Fail. An error happened when cleaning SVL or TSL.
 *          SBR_FILE_NOT_FOUND  -Fail. The specified ps_file_name was not found.
 *          SBR_INV_ARG         -Fail. Invalid arguments.
 *          SBR_INV_HANDLE      -Fail. Invalid builder handle.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_clean_lists_fct) (
     HANDLE_T           h_builder,
     const CHAR*        ps_file_name);

/*------------------------------------------------------------------*/
/*! @brief  the (*x_sb_reg_scan_map_fct)() type function.
 *  @param [in] pv_nfy_fct      The scan map function.
 *  @param [in] pt_nfy_tag      The notify data.
 *  
 *  @return
 *  @retval SBR_OK                -Success.
 *          SBR_INV_ARG           -Operation fail.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_reg_scan_map_fct) (
     VOID*              pv_nfy_fct,
     VOID*              pt_nfy_tag);

/*------------------------------------------------------------------*/
/*! @brief the (*x_sb_get_fct)() type function.
 *  @param [in] h_builder_base  The handle of builder base object.
 *  @param [in] e_key_type      The requested type to get.
 *  @param [in] pv_key_info     The auxiliary info for the requested get type.
 *  @param [out]pz_key_data_len Contains the real length of retrieved key data if the API successfully returns.
 *  @param [out]pv_key_data     Contains the requested get data.
 *  
 *  @return
 *  @retval SBR_OK                -Success.
 *          SBR_INV_KEY_TYPE      -Invalid get operation.
 *          SBR_INV_KEY_INFO      -Invalid get operation data.
 *          SBR_INV_HANDLE        -Fail. Invalid builder handle.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_get_fct) (
     HANDLE_T           h_builder,
     SB_KEY_TYPE_T      e_key_type,
     VOID*              pv_key_info,
     SIZE_T*            pz_key_data_len,
     VOID*              pv_key_data);

/*------------------------------------------------------------------*/
/*! @brief  the (*x_sb_set_fct)() type function.
 *  @param [in] h_builder_base  The handle of builder base object.
 *  @param [in] e_key_type      The requested type to set.
 *  @param [in] pv_key_info     The auxiliary info for the requested set type.
 *  @param [in] z_key_data_len  Contains length of the buffer to which pv_key_data points.
 *  @param [in] pv_key_data     Contains the requested set data.
 *  
 *  @return
 *  @retval SBR_OK                -Success.
 *          SBR_INV_KEY_TYPE      -Invalid set operation.
 *          SBR_INV_KEY_INFO      -Invalid set operation data.
 *          SBR_INV_HANDLE        -Fail. Invalid builder handle.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_set_fct) (
     HANDLE_T           h_builder,
     SB_KEY_TYPE_T      e_key_type,
     VOID*              pv_key_info,
     SIZE_T             z_key_data_len,
     VOID*              pv_key_data);

/*------------------------------------------------------------------*/
/*! @struct SB_ENG_FCT_TBL_T
 *  @brief Internal use
 *  @code
 *  typedef struct _SB_ENG_FCT_TBL_T
 *  {
 *      x_sb_create_builder_data_fct    pf_create_builder_data;
 *      x_sb_delete_builder_data_fct    pf_delete_builder_data;
 *      x_sb_match_builder_data_fct     pf_match_builder_data;
 *      x_sb_engine_task_fct            pf_engine_task;
 *      x_sb_start_scan_fct             pf_start_scan;
 *      x_sb_cancel_scan_fct            pf_cancel_scan;
 *      x_sb_save_lists_fct             pf_save_lists;
 *      x_sb_load_lists_fct             pf_load_lists;
 *      x_sb_clean_lists_fct            pf_clean_lists;
 *      x_sb_get_fct                    pf_get;
 *      x_sb_set_fct                    pf_set;
 *      x_sb_cancel_scan_fct            pf_cancel_scan_sync;
 *  } SB_ENG_FCT_TBL_T;
 *  @endcode
 *  @li@c  pf_create_builder_data                            - Create SB engine handler
 *  @li@c  pf_delete_builder_data                            - Delete SB engine handler
 *  @li@c  pf_match_builder_data                             - Check the SB engine handler is
 *  @li@c  pf_engine_task                                    - The core state machine of engine task
 *  @li@c  pf_start_scan                                     - Start scan function of each engine implementation
 *  @li@c  pf_cancel_scan                                    - Cancel scan function of each engine implementation
 *  @li@c  pf_save_lists                                     - Save channel list function of each engine implementation
 *  @li@c  pf_load_lists                                     - Load channel list function of each engine implementation
 *  @li@c  pf_clean_lists                                    - Clean channel list function of each engine implemnetation
 *  @li@c  pf_get                                            - get function of each engine implementation
 *  @li@c  pf_set                                            - set function of each engine implementation
 *  @li@c  pf_cancel_scan_sync                               - Synchronism cancel scan function of each engine implementation
 */
/*------------------------------------------------------------------*/
typedef struct _SB_ENG_FCT_TBL_T
{
    x_sb_create_builder_data_fct    pf_create_builder_data;
    x_sb_delete_builder_data_fct    pf_delete_builder_data;
    x_sb_match_builder_data_fct     pf_match_builder_data;
    x_sb_engine_task_fct            pf_engine_task;
    x_sb_start_scan_fct             pf_start_scan;
    x_sb_cancel_scan_fct            pf_cancel_scan;
    x_sb_save_lists_fct             pf_save_lists;
    x_sb_load_lists_fct             pf_load_lists;
    x_sb_clean_lists_fct            pf_clean_lists;
    x_sb_get_fct                    pf_get;
    x_sb_set_fct                    pf_set;
    /* cliu 20090706, sync cancel scan function pointer */    
    x_sb_cancel_scan_fct            pf_cancel_scan_sync;
    x_sb_reg_scan_map_fct           pf_reg_scan_map;

} SB_ENG_FCT_TBL_T;

/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_SVL_BLDR */
/*----------------------------------------------------------------------------*/

#endif /* _U_SB_ENG_H_ */

