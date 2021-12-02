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
/*! @file u_aee.h 
 *  $RCSfile: u_aee.h,v $
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
/*! @defgroup groupMW_AEE Application Execution Environment
 *  @ingroup groupMW
 *  @brief Provides a unified interface for AEE
 *  %This module manages the resources an application or component can use and control
 *  the basic lifecycle of applications.
 *   - including these sub modules:
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_AEE_H_
#define _U_AEE_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* AEE API return values */

#define AEER_OK                    ((INT32)  0)    /**<   return ok     */
#define AEER_FAIL                  ((INT32) -1)    /**<   return fail     */
#define AEER_INV_ARG               ((INT32) -2)    /**<   return invalid argument   */
#define AEER_INV_HANDLE            ((INT32) -3)    /**<   return invalid handle     */
#define AEER_INV_NAME              ((INT32) -4)    /**<   return invalid name     */
#define AEER_OUT_OF_HANDLES        ((INT32) -5)    /**<   return out of handles     */
#define AEER_OUT_OF_MEMORY         ((INT32) -6)    /**<   return out of memory     */
#define AEER_OUT_OF_RESOURCES      ((INT32) -7)    /**<   return out of resources   */
#define AEER_NOT_AUTHORIZED        ((INT32) -8)    /**<   return not authorized     */
#define AEER_AEE_NO_RIGHTS         ((INT32) -9)    /**<   return no rights     */
#define AEER_AEE_OUT_OF_RESOURCES  ((INT32) -10)    /**<  return out of resources   */

/* AEE authorization flags */
#define AEE_FLAG               ((UINT64) 0)    /**<    authorization flags    */
#define AEE_FLAG_WRITE_SVL     MAKE_BIT_MASK_64(0)    /**<   flag of write SVL     */
#define AEE_FLAG_WRITE_TSL     MAKE_BIT_MASK_64(1)    /**<   flag of write TSL     */
#define AEE_FLAG_WRITE_FLM     MAKE_BIT_MASK_64(2)    /**<   flag of write FLM     */
#define AEE_FLAG_WRITE_CONFIG  MAKE_BIT_MASK_64(3)    /**<   flag of write configure  */


/*------------------------------------------------------------------*/
/*! @brief  This function is called in a new dedicated thread when the AEE is created. 
 *  @param [in]     h_app       Contains the application handle. Such handle cannot be freed (it 
 *                                         will be freed automatically when the AEE is terminated)
 *  @param [in]     pv_arg      Contains a reference the some data used by the main function. 
 *                                         May be NULL.
 *  @param [out]    -
 *  @note
 *  @see
 *  @return     -
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (* x_aee_app_main_fct) (HANDLE_T  h_app,
                                     VOID*     pv_arg);

/*------------------------------------------------------------------*/
/*! @brief  This function is called when the AEE is sent some data.
 *  @param [in]     h_app       Contains the application handle.
 *  @param [in]     ui4_type    Contains the type of the data. This value is defined by the AEE and 
 *                                          should be documented in the AEE communication protocol.
 *  @param [in]     pv_data     Contains a reference to data. May be NULL.
 *  @param [in]     z_data_len  Contains the length (in bytes) of the data referenced by the 
 *                                          pv_data pointer. May be zero. This value is irrelevant if pv_data is NULL.
 *  @param [out]    -
 *  @note
 *  @see
 *  @return      the error code
 *  @retval      AEER_OK             Routine successful.
 *  @retval      AEER_OUT_OF_MEMORY  Out of memory.
 *  @retval      AEER_FAIL           Routine failed.
 */
/*------------------------------------------------------------------*/
typedef INT32 (* x_aee_app_receive_data_fct) (HANDLE_T  h_app,
                                              UINT32    ui4_type,
                                              VOID*     pv_data,
                                              SIZE_T    z_data_len);

/*------------------------------------------------------------------*/
/*! @struct AEE_MAX_RESOURCES_T
 *  @brief  This type specifies the maximum amount of resources an AEE can use.
 *  @code
 *  typedef struct
 *  {
 *      SIZE_T  z_min_memory;
 *      SIZE_T  z_max_memory;
 *      UINT64  ui8_max_files_size;
 *      UINT16  ui2_max_files;
 *      UINT16  ui2_max_handles;
 *      UINT16  ui2_max_threads;
 *      UINT16  ui2_max_semaphores;
 *      UINT16  ui2_max_msg_queues;
 *      UINT16  ui2_max_nb_msgs;
 *      UINT16  ui2_max_msg_size;
 *  }   AEE_MAX_RESOURCES_T;
 *  @endcode
 *  @li@c  z_min_memory   -Specifies the minimum amount of memory (in bytes) the AEE needs 
 *                                      after its creation.
 *  @li@c  z_max_memory  -Specifies the maximum amount of memory (in bytes) the AEE can 
 *                                      allocate.
 *  @li@c  ui8_max_files_size    -Specifies the maximum combined size of the files the AEE owns 
 *                                            (i.e. the files having the AEE group and application ids).
 *  @li@c  ui2_max_files   -Specifies the maximum number of files the AEE can own.
 *  @li@c  ui2_max_handles  -Specifies the maximum number of handles the AEE can allocate.
 *  @li@c  ui2_max_threads  -Specifies the maximum number of threads the AEE can have at any 
 *                                        given time.
 *  @li@c  ui2_max_semaphores  -Specifies the maximum number of semaphores the AEE can 
 *                                              allocate at any given time.
 *  @li@c  ui2_max_msg_queues  -Specifies the maximum number of message queues the AEE 
 *                                               can allocate at any given time.
 *  @li@c  ui2_max_nb_msgs  -Specifies the maximum number of messages in the message 
 *                                          queues the AEE can create.
 *  @li@c  ui2_max_msg_size  -Specifies the maximum size of the messages in the message 
 *                                          queues the AEE can create.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    SIZE_T  z_min_memory;
    SIZE_T  z_max_memory;
    UINT64  ui8_max_files_size;
    UINT16  ui2_max_files;
    UINT16  ui2_max_handles;
    UINT16  ui2_max_threads;
    UINT16  ui2_max_semaphores;
    UINT16  ui2_max_msg_queues;
    UINT16  ui2_max_nb_msgs;
    UINT16  ui2_max_msg_size;
}   AEE_MAX_RESOURCES_T;

/*------------------------------------------------------------------*/
/*! @struct AEE_RESOURCES_T
 *  @brief  This type specifies the resources an AEE is using.
 *  @code
 *  typedef struct
 *  {
 *      SIZE_T  z_memory;
 *      UINT64  ui8_files_size;
 *      UINT16  ui2_files;
 *      UINT16  ui2_handles;
 *      UINT16  ui2_threads;
 *      UINT16  ui2_semaphores;
 *      UINT16  ui2_msg_queues;
 *  }   AEE_RESOURCES_T;
 *  @endcode
 *  @li@c  z_memory         -Specifies the amount of memory (in bytes) the AEE is using.
 *  @li@c  ui8_files_size     -Specifies the combined size of the files the AEE owns (i.e. the files 
 *                                      having the AEE group and application ids).
 *  @li@c  ui2_files            -Specifies the number of files the AEE owns.
 *  @li@c  ui2_handles      -Specifies the number of handles (all types combined) the AEE allocated.
 *  @li@c  ui2_threads       -Specifies the number of threads that "belongs" to the AEE.
 *  @li@c  ui2_semaphores   -Specifies the number of semaphores allocated by the AEE.
 *  @li@c  ui2_msg_queues   -Specifies the number of message queues allocated by the AEE.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    SIZE_T  z_memory;
    UINT64  ui8_files_size;
    UINT16  ui2_files;
    UINT16  ui2_handles;
    UINT16  ui2_threads;
    UINT16  ui2_semaphores;
    UINT16  ui2_msg_queues;
}   AEE_RESOURCES_T;

/*------------------------------------------------------------------*/
/*! @struct AEE_APP_DESC_T
 *  @brief  This type specifies the properties of an AEE when it is created.
 *  @code
 *  typedef struct
 *  {
 *      UINT64                      ui8_flags;
 *      THREAD_DESCR_T              t_thread_desc;
 *      x_aee_app_main_fct          pf_main;
 *      VOID*                       pv_arg;
 *      SIZE_T                      z_arg_size;
 *      x_aee_app_receive_data_fct  pf_receive_data;
 *      UINT32                      ui4_app_group_id;
 *      UINT32                      ui4_app_id;
 *      AEE_MAX_RESOURCES_T         t_max_res;
 *  }   AEE_APP_DESC_T;
 *  @endcode
 *  @li@c  ui8_flags          -Specifies the authorizations flags, i.e. the operations an AEE is allowed to perform.
 *  @li@c  t_thread_desc   -Specifies the properties of the thread that will run the application's main function.
 *  @li@c  pf_main            -Specifies the application's main function.
 *  @li@c  pv_arg              -Specifies the argument that is passed to the application's main function.
 *  @li@c  z_arg_size        -Specifies the size of the argument passed to the application's main function.
 *  @li@c  pf_receive_data    -Specifies the hook function that is called when an AEE receives data.
 *  @li@c  ui4_app_group_id    -Specifies the AEE group id.
 *  @li@c  ui4_app_id            -Specifies the AEE application id.
 *  @li@c  t_max_res        -Defines the maximum amount of resources the AEE can allocate.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT64                      ui8_flags;
    THREAD_DESCR_T              t_thread_desc;
    x_aee_app_main_fct          pf_main;
    VOID*                       pv_arg;
    SIZE_T                      z_arg_size;
    x_aee_app_receive_data_fct  pf_receive_data;
    UINT32                      ui4_app_group_id;
    UINT32                      ui4_app_id;
    AEE_MAX_RESOURCES_T         t_max_res;
}   AEE_APP_DESC_T;

/*------------------------------------------------------------------*/
/*! @enum AEE_RESOURCE_ID_T
 *  @brief  This enumeration specifies the different resource ids.
 *  @code
 *  typedef enum
 *  {
 *      AEE_RES_MIN_MEMORY_ID,
 *      AEE_RES_MAX_MEMORY_ID,
 *      AEE_RES_MAX_FILES_SIZE_ID,
 *      AEE_RES_MAX_FILES_ID,
 *      AEE_RES_MAX_HANDLES_ID,
 *      AEE_RES_MAX_THREADS_ID,
 *      AEE_RES_MAX_SEMAPHORES_ID,
 *      AEE_RES_MAX_MSG_QUEUES_ID,
 *      AEE_RES_MAX_NB_MSGS_ID,
 *      AEE_RES_MAX_MSG_SIZE_ID,
 *      AEE_RES_MEMORY_ID,
 *      AEE_RES_FILES_SIZE_ID,
 *      AEE_RES_FILES_ID,
 *      AEE_RES_HANDLES_ID,
 *      AEE_RES_THREADS_ID,
 *      AEE_RES_SEMAPHORES_ID,
 *      AEE_RES_MSG_QUEUES_ID
 *  }   AEE_RESOURCE_ID_T;
 *  @endcode
 *  @li@c  AEE_RES_MIN_MEMORY_ID        -id for min memory.
 *  @li@c  AEE_RES_MAX_MEMORY_ID       -id for max memory.
 *  @li@c  AEE_RES_MAX_FILES_SIZE_ID   -id for max file size.
 *  @li@c  AEE_RES_MAX_FILES_ID           -id for max files.
 *  @li@c  AEE_RES_MAX_HANDLES_ID      -id for max handles.
 *  @li@c  AEE_RES_MAX_THREADS_ID      -id for max threads.
 *  @li@c  AEE_RES_MAX_SEMAPHORES_ID  -id for max semaphores.
 *  @li@c  AEE_RES_MAX_MSG_QUEUES_ID  -id for max message queues.
 *  @li@c  AEE_RES_MAX_NB_MSGS_ID       -id for max NB messages.
 *  @li@c  AEE_RES_MAX_MSG_SIZE_ID      -id for max message size.
 *  @li@c  AEE_RES_MEMORY_ID        -id for memory.
 *  @li@c  AEE_RES_FILES_SIZE_ID    -id for file size.
 *  @li@c  AEE_RES_FILES_ID            -id for files.
 *  @li@c  AEE_RES_HANDLES_ID      -id for handles.
 *  @li@c  AEE_RES_THREADS_ID      -id for threads.
 *  @li@c  AEE_RES_SEMAPHORES_ID  -id for semaphores.
 *  @li@c  AEE_RES_MSG_QUEUES_ID  -id for message queues.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    AEE_RES_MIN_MEMORY_ID,
    AEE_RES_MAX_MEMORY_ID,
    AEE_RES_MAX_FILES_SIZE_ID,
    AEE_RES_MAX_FILES_ID,
    AEE_RES_MAX_HANDLES_ID,
    AEE_RES_MAX_THREADS_ID,
    AEE_RES_MAX_SEMAPHORES_ID,
    AEE_RES_MAX_MSG_QUEUES_ID,
    AEE_RES_MAX_NB_MSGS_ID,
    AEE_RES_MAX_MSG_SIZE_ID,
    AEE_RES_MEMORY_ID,
    AEE_RES_FILES_SIZE_ID,
    AEE_RES_FILES_ID,
    AEE_RES_HANDLES_ID,
    AEE_RES_THREADS_ID,
    AEE_RES_SEMAPHORES_ID,
    AEE_RES_MSG_QUEUES_ID
}   AEE_RESOURCE_ID_T;

#endif /* _U_AEE_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                         /* end of groupMW_AEE */
/*----------------------------------------------------------------------------*/

