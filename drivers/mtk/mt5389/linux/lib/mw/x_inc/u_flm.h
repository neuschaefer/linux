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
/*! @file u_flm.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/flm/u_flm.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Favorite List Manager (FLM) specific
 *         definitions, which are exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_FLM_H_
#define _U_FLM_H_
/*----------------------------------------------------------------------------*/
/*! @defgroup   groupMW_FLM Favorite List Manager
 *  @ingroup    groupMW
 *  @brief      The Favorites List Manager (FLM) allows the client application
 *              to create, modify, and erase one or multiple lists of favorite
 *              channels.
 *  @see        groupMW_CDB
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
                    include files
-------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_cdb.h"
#include "u_sys_name.h"

/*-------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ------------------------------------------------------------------------*/

#define FLM_NAME_LEN SYS_NAME_LEN    /**< Specifies the maximum number of characters in a favorite list name (excluding the terminating NULL character). */
#define FLM_NULL_VER_ID  (CDB_NULL_VER_ID)    /**< Specifies an invalid or irrelevant version ID. */

/* Return code status from API */
#define FLMR_OK                   ((INT32)  0)    /**< Operation succeeded. */
#define FLMR_OUT_OF_HANDLE        ((INT32) -1)    /**< Handle allocation failed. */
#define FLMR_OUT_OF_MEM           ((INT32) -2)    /**< Memory exhausted. */
#define FLMR_INV_HANDLE           ((INT32) -3)    /**< Bad handle. */
#define FLMR_INV_ARG              ((INT32) -4)    /**< Invalid argument. */
#define FLMR_REC_NOT_FOUND        ((INT32) -5)    /**< Record not found. */
#define FLMR_END_OF_ITERATION     ((INT32) -6)    /**< Iteration reached end. */
#define FLMR_CANT_CREATE_FILE     ((INT32) -7)    /**< Cannot create file. */
#define FLMR_CANT_OPEN_FILE       ((INT32) -8)    /**< Cannot open file. */
#define FLMR_MASTER_CHANGED       ((INT32) -9)    /**< Master list changed. */
#define FLMR_NO_WRITE_LOCK        ((INT32) -10)   /**< Failed initializing RW Lock. */
#define FLMR_UNKNOWN              ((INT32) -11)   /**< Unknown error. */
#define FLMR_WRITE_LOCKED         ((INT32) -12)   /**< Failed to acquire lock as DB is write locked. */
#define FLMR_MODIFIED             ((INT32) -13)   /**< Database modified since last iteration. */
#define FLMR_DEV_ERR              ((INT32) -14)   /**< File manager device error. */
#define FLMR_OS_ERR               ((INT32) -15)   /**< File manager internal error. */
#define FLMR_PERM_DENY            ((INT32) -16)   /**< File manager permission denied. */
#define FLMR_EOF                  ((INT32) -17)   /**< File manager reached end of file. */
#define FLMR_FILESYS_FULL         ((INT32) -18)   /**< File system is full. */
#define FLMR_LIMIT_EXCEED         ((INT32) -19)   /**< Reached maximum number of FLM objects. */
#define FLMR_INV_NAME             ((INT32) -20)   /**< DB name already in use. */
#define FLMR_INV_FILE             ((INT32) -21)   /**< Invalid file. */
#define FLMR_CANT_CREATE_LIST     ((INT32) -22)   /**< Cannot add name to master list. */
#define FLMR_LIST_NOT_FOUND       ((INT32) -23)   /**< List not found by given name. */
#define FLMR_INV_FILE_PATH        ((INT32) -24)   /**< Invalid file path. */
#define FLMR_FILE_NOT_FOUND       ((INT32) -25)   /**< File not found. */
#define FLMR_AEE_OUT_OF_RESOURCES ((INT32) -26)   /**< Can not allocate AEE resources. */
#define FLMR_AEE_NO_RIGHTS        ((INT32) -27)   /**< No right for the AEE resources. */
#define FLMR_FILE_READ_FAIL       ((INT32) -28)   /**< Failed to read from file. */
#define FLMR_FILE_WRITE_FAIL      ((INT32) -29)   /**< Failed to write to file. */

/* FLM notification reason bit masks */
#define FLM_REASON_UNK          ((UINT32) 0)             /**< Reason unknown */
#define FLM_RECORD_ADD          (MAKE_BIT_MASK_32(1))    /**< Reason for adding records */
#define FLM_RECORD_DEL          (MAKE_BIT_MASK_32(2))    /**< Reason for deleting records */
#define FLM_RECORD_MOD          (MAKE_BIT_MASK_32(3))    /**< Reason for modifying records */

/* Notification condition */
/*------------------------------------------------------------------*/
/*! @brief  Enumeration for notification conditions.
 *  @code
 *  typedef  enum
 *  {
 *      FLM_NO_COND,
 *      FLM_UPDATING,      
 *      FLM_UPDATED,       
 *      FLM_CLOSED         
 *  } FLM_COND_T;
 *  @endcode
 *  @li@c  FLM_NO_COND                   - Unknown condition.
 *  @li@c  FLM_UPDATING                  - When flm_lock is called.
 *  @li@c  FLM_UPDATED                   - When flm_unlock is called.
 *  @li@c  FLM_CLOSED                    - When flm_delete is called.
 */
/*------------------------------------------------------------------*/
typedef  enum
{
    FLM_NO_COND,
    FLM_UPDATING,      /* When _lock is called. */
    FLM_UPDATED,       /* When _unlock is called. */
    FLM_CLOSED         /* When _delete is called. */
} FLM_COND_T;

/*------------------------------------------------------------------*/
/*! @brief  This function is called by the favorite list manager to notify of
 *          changes in the state of the favorite list.
 *  @param  [in]  h_fav_handle      - User name associated with this list.
 *  @param  [in]  e_cond            - Specifies the notification condition.
 *  @param  [in]  ui4_reason        - Specifies the notification reason.
 *  @param  [in]  pv_tag            - Contains the tag passed when the favorite
 *                                    list was created or opened.
 *  @param  [in]  ui4_data          - Contains some additional data.
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef  VOID (*pf_flm_nfy_func)
(
    HANDLE_T            h_fav_handle,
    FLM_COND_T          e_cond,
    UINT32              ui4_reason,
    VOID*               pv_tag,
    UINT32              ui4_data
);

/*------------------------------------------------------------------*/
/*! @brief  Structure for FLM list object.
 *  @code
 *  typedef struct {
 *      HANDLE_T        h_db;      
 *      HANDLE_T        h_self;    
 *      pf_flm_nfy_func pf_flm_notify;
 *      CHAR            as_name[FLM_NAME_LEN+1];
 *  } FLM_LIST_T;
 *  @endcode
 *  @li@c  h_db                          - Handle to client database object.
 *  @li@c  h_self                        - Handle to self.
 *  @li@c  pf_flm_notify                 - Registered notification function.
 *  @li@c  as_name                       - List name.
 */
/*------------------------------------------------------------------*/
typedef struct {
    HANDLE_T        h_db;      /* handle to client database object */
    HANDLE_T        h_self;    /* handle to self */
    pf_flm_nfy_func pf_flm_notify;
    CHAR            as_name[FLM_NAME_LEN+1];
} FLM_LIST_T;

/*------------------------------------------------------------------*/
/*! @brief  Structure for FLM records.
 *  @code
 *  typedef  struct {
 *      UINT16 ui2_svc_id;
 *      UINT16 ui2_svc_rec_id;
 *  } FLM_REC_T;
 *  @endcode
 *  @li@c  ui2_svc_id                    - SVL ID of the favorite channel.
 *  @li@c  ui2_svc_rec_id                - SVL record ID of the favorite
 *                                         channel.
 */
/*------------------------------------------------------------------*/
typedef  struct {
    UINT16 ui2_svc_id;
    UINT16 ui2_svc_rec_id;
} FLM_REC_T;

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
#endif
