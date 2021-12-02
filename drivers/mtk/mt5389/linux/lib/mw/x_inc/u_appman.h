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
/*! @file u_appman.h 
 *  $RCSfile: u_appman.h,v$
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains AEE specific definitions, which are
 *         exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_AEE Application Manager
 *  @ingroup groupMW
 *  @brief Provides a unified interface for APPMAN
 *  %This module manages the lifecycle of applications.
 *   - including these sub modules:
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_APPMAN_H_
#define _U_APPMAN_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_aee.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @enum APP_NFY_REASON_T
 *  @brief  This enumeration specifies the different notification reasons.
 *  @code
 *  typedef enum
 *  {
 *      APP_NFY_PRE_INIT,
 *      APP_NFY_INIT_OK,
 *      APP_NFY_INIT_FAILED,
 *      APP_NFY_PRE_EXIT,
 *      APP_NFY_EXIT_OK,
 *      APP_NFY_EXIT_FAILED,
 *      APP_NFY_PRE_PAUSE,
 *      APP_NFY_PAUSE_OK,
 *      APP_NFY_PAUSE_FAILED,
 *      APP_NFY_PRE_RESUME,
 *      APP_NFY_RESUME_OK,
 *      APP_NFY_RESUME_FAILED,
 *      APP_NFY_PRE_PROCESS_MSG,
 *      APP_NFY_PROCESS_MSG_OK,
 *      APP_NFY_PROCESS_MSG_FAILED
 *  }   APP_NFY_REASON_T;
 *  @endcode
 *  @li@c  APP_NFY_PRE_INIT         -The notify reason is pre-init.
 *  @li@c  APP_NFY_INIT_OK          -The notify reason is init ok.
 *  @li@c  APP_NFY_INIT_FAILED    -The notify reason is init failed.
 *  @li@c  APP_NFY_PRE_EXIT        -The notify reason is pre-exit.
 *  @li@c  APP_NFY_EXIT_OK          -The notify reason is exit ok.
 *  @li@c  APP_NFY_EXIT_FAILED    -The notify reason is exit failed.
 *  @li@c  APP_NFY_PRE_PAUSE      -The notify reason is pre-pause.
 *  @li@c  APP_NFY_PAUSE_OK       -The notify reason is pause ok.
 *  @li@c  APP_NFY_PAUSE_FAILED  -The notify reason is pause failed.
 *  @li@c  APP_NFY_PRE_RESUME     -The notify reason is pre-resume.
 *  @li@c  APP_NFY_RESUME_OK      -The notify reason is resume ok.
 *  @li@c  APP_NFY_RESUME_FAILED   -The notify reason is resume failed.
 *  @li@c  APP_NFY_PRE_PROCESS_MSG  -The notify reason is pre-process message.
 *  @li@c  APP_NFY_PROCESS_MSG_OK   -The notify reason is process message ok.
 *  @li@c  APP_NFY_PROCESS_MSG_FAILED   -The notify reason is process message failed.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    APP_NFY_PRE_INIT,
    APP_NFY_INIT_OK,
    APP_NFY_INIT_FAILED,
    APP_NFY_PRE_EXIT,
    APP_NFY_EXIT_OK,
    APP_NFY_EXIT_FAILED,
    APP_NFY_PRE_PAUSE,
    APP_NFY_PAUSE_OK,
    APP_NFY_PAUSE_FAILED,
    APP_NFY_PRE_RESUME,
    APP_NFY_RESUME_OK,
    APP_NFY_RESUME_FAILED,
    APP_NFY_PRE_PROCESS_MSG,
    APP_NFY_PROCESS_MSG_OK,
    APP_NFY_PROCESS_MSG_FAILED
}   APP_NFY_REASON_T;

/*------------------------------------------------------------------*/
/*! @enum APP_EXIT_MODE_T
 *  @brief  This enumeration specifies the different exit modes for an application.
 *  @code
 *  typedef enum
 *  {
 *      APP_EXIT_FORCE,
 *      APP_EXIT_DEMAND
 *  }   APP_EXIT_MODE_T;
 *  @endcode
 *  @li@c  APP_EXIT_FORCE         -Force application exit.
 *  @li@c  APP_EXIT_DEMAND      -Application exit on demand.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    APP_EXIT_FORCE,
    APP_EXIT_DEMAND
}   APP_EXIT_MODE_T;

/*------------------------------------------------------------------*/
/*! @brief  This function is called in the context of the application message loop when a message 
 *              is received, and when a message is processed. When a message is received, this API 
 *              will be called with one of the APP_NFY_PRE_* notification reasons. In that case, this 
 *              API should return TRUE if the message is to be ignored, or FALSE if the message is to 
 *              be processed. If the message is processed, then this API will be called again, this time 
 *              after the message is processed, with one of the APP_NFY_*_OK or APP_NFY_*_FAILED 
 *              notification reasons.
 *  @param [in]     h_app   Contains the application handle.
 *  @param [in]     pv_tag  References a tag value (specified when calling x_app_send_msg).
 *  @param [in]     e_nfy_reason    Contains the notification reason.
 *  @param [out]    -
 *  @note
 *  @see
 *  @return      if the message is to be ignored.
 *  @retval      TRUE   The message is to be ignored.
 *  @retval      FALSE  The message is to be processed.
 */
/*------------------------------------------------------------------*/
typedef BOOL (*x_app_nfy_fct) (HANDLE_T          h_app,
                               VOID*             pv_tag,
                               APP_NFY_REASON_T  e_nfy_reason);

/*------------------------------------------------------------------*/
/*! @brief  This function is called in the context of the application message loop when the 
 *               application starts.
 *  @param [in]     ps_name     References the application's name.
 *  @param [in]     h_app           Contains the application handle.
 *  @param [out]    -
 *  @note
 *  @see
 *  @return      the error code.
 *  @retval      AEER_OK    Routine successful.
 *  @retval      Any other value    Routine failed.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_app_init_fct) (const CHAR*  ps_name,
                                 HANDLE_T     h_app);

/*------------------------------------------------------------------*/
/*! @brief  This function is called in the context of the application message loop when the 
 *               application quits. This API should be responsible for saving data, clean-up, etc.
 *  @param [in]     h_app   Contains the application handle.
 *  @param [in]     e_exit_mode     Contains the exit mode.
 *  @param [out]    -
 *  @note
 *  @see
 *  @return      the error code.
 *  @retval      AEER_OK    Routine successful.
 *  @retval      Any other value    Routine failed.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_app_exit_fct) (HANDLE_T         h_app,
                                 APP_EXIT_MODE_T  e_exit_mode);

/*------------------------------------------------------------------*/
/*! @brief      This function is called in the context of the application message loop when the 
 *                  application is paused.
 *  @param [in]     h_app   Contains the application handle.
 *  @param [out]    -
 *  @note
 *  @see
 *  @return      the error code.
 *  @retval      AEER_OK    Routine successful.
 *  @retval      Any other value    Routine failed.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_app_pause_fct) (HANDLE_T  h_app);

/*------------------------------------------------------------------*/
/*! @brief  This function is called in the context of the application message loop when the 
 *               application is resumed.
 *  @param [in]     h_app   Contains the application handle.
 *  @param [out]    -
 *  @note
 *  @see
 *  @return      the error code.
 *  @retval      AEER_OK    Routine successful.
 *  @retval      Any other value    Routine failed.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_app_resume_fct) (HANDLE_T  h_app);

/*------------------------------------------------------------------*/
/*! @brief  This function is called in the context of the application message loop when a message 
 *               (sent by calling x_app_send_msg) is processed.
 *  @param [in]     h_app   Contains the application handle.
 *  @param [in]     ui4_type    Contains the type of the data contained in pv_msg, or simply a 
 *                                          request type. Values are defined by the application.
 *  @param [in]     pv_msg  References the message data.
 *  @param [in]     z_msg_len   Contains the length (in bytes) of pv_msg.
 *  @param [in]     b_paused    Specifies whether the application is currently paused.
 *  @param [out]    -
 *  @note
 *  @see
 *  @return      the error code.
 *  @retval      AEER_OK    Routine successful.
 *  @retval      Any other value    Routine failed.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_app_process_msg_fct) (HANDLE_T     h_app,
                                        UINT32       ui4_type,
                                        const VOID*  pv_msg,
                                        SIZE_T       z_msg_len,
                                        BOOL         b_paused);

/*------------------------------------------------------------------*/
/*! @struct APP_FCT_TBL_T
 *  @brief  This type specifies the functions that an application must implement so it can be 
 *              started by the application manager (see x_app_start).
 *  @code
 *  typedef struct
 *  {
 *      x_app_init_fct         pf_init;
 *      x_app_exit_fct         pf_exit;
 *      x_app_pause_fct        pf_pause;
 *      x_app_resume_fct       pf_resume;
 *      x_app_process_msg_fct  pf_process_msg;
 *  }   APP_FCT_TBL_T;
 *  @endcode
 *  @li@c  pf_init     -This function is called in the context of the application message loop when the 
 *                          application starts.
 *  @li@c  pf_exit    -This function is called in the context of the application message loop when the 
 *                      application quits. This API should be responsible for saving data, clean-up, etc.
 *  @li@c  pf_pause -This function is called in the context of the application message loop when the 
 *                          application is paused.
 *  @li@c  pf_resume  -This function is called in the context of the application message loop when the 
 *                              application is resumed.
 *  @li@c  pf_process_msg   -This function is called in the context of the application message loop 
 *                                      when a message (sent by calling x_app_send_msg) is processed.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    x_app_init_fct         pf_init;
#ifdef MW_FAST_INIT
    x_app_init_fct         pf_post_init;
#endif
    x_app_exit_fct         pf_exit;
    x_app_pause_fct        pf_pause;
    x_app_resume_fct       pf_resume;
    x_app_process_msg_fct  pf_process_msg;
}   APP_FCT_TBL_T;

/*------------------------------------------------------------------*/
/*! @struct APP_DESC_T
 *  @brief This type specifies the properties of an application transfer to AEE.
 *  @code
 *  typedef struct
 *  {
 *      UINT64               ui8_flags;
 *      THREAD_DESCR_T       t_thread_desc;
 *      UINT32               ui4_app_group_id;
 *      UINT32               ui4_app_id;
 *      AEE_MAX_RESOURCES_T  t_max_res;
 *      UINT16               ui2_msg_count;
 *      UINT16               ui2_max_msg_size;
 *  }   APP_DESC_T;
 *  @endcode
 *  @li@c  ui8_flags    -Specifies the authorizations flags, i.e. the operations an AEE is allowed to perform.
 *  @li@c  t_thread_desc  -Specifies the properties of the thread that will run the application's main function.
 *  @li@c  ui4_app_group_id   -Specifies the AEE group id.
 *  @li@c  ui4_app_id     -Specifies the AEE application id.
 *  @li@c  t_max_res     -Defines the maximum amount of resources the AEE can allocate.
 *  @li@c  ui2_msg_count   -Contains the maximum number of messages in the application's message loop.
 *  @li@c  ui2_max_msg_size   -Contains the maximum size of a message sent by calling x_app_send_msg.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT64               ui8_flags;
    THREAD_DESCR_T       t_thread_desc;
    UINT32               ui4_app_group_id;
    UINT32               ui4_app_id;
    AEE_MAX_RESOURCES_T  t_max_res;
    UINT16               ui2_msg_count;
    UINT16               ui2_max_msg_size;
}   APP_DESC_T;

#endif /* _U_APPMAN_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                         /* end of groupMW_AEE */
/*----------------------------------------------------------------------------*/

