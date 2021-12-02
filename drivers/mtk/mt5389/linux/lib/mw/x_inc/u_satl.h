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
/*! @file u_satl.h
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/satl/u_satl.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *    This header file contains Satellite list (SATL) database
 *    API definitions, which are exported to other Middleware components
 *    and applications.
 *         
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SATL_H_
#define _U_SATL_H_
/*----------------------------------------------------------------------------*/
/*! @defgroup   groupMW_SATL Satellite List
 *  @ingroup    groupMW
 *  @brief      The SATL module is used to store and retrieve the tuning
 *              parameter information for the broadcast streams.
 *  @see        groupMW_CDB
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"
#include "u_tuner.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/

/* Return code status from API */
#define     SATLR_OK                   ((INT32)  0)    /**< Operation succeeded. */
#define	    SATLR_FAIL                 ((INT32) -1)    /**< Operation failed. */
#define     SATLR_OUT_OF_HANDLE        ((INT32) -2)    /**< Handle allocation failed. */
#define	    SATLR_OUT_OF_MEM           ((INT32) -3)    /**< Memory exhausted. */
#define	    SATLR_INV_HANDLE           ((INT32) -4)    /**< Bad handle. */
#define     SATLR_INV_ARG              ((INT32) -5)    /**< Invalid argument. */
#define	    SATLR_REC_NOT_FOUND        ((INT32) -6)    /**< Record not found. */
#define	    SATLR_NO_WRITE_LOCK        ((INT32) -7)    /**< Failed initializing RW Lock. */
#define     SATLR_NOT_FOUND            ((INT32) -8)    /**< Specified SATL object not found. */
#define     SATLR_END_OF_ITERATION     ((INT32) -9)    /**< Iteration reached end. */
#define     SATLR_MODIFIED             ((INT32) -10)    /**< Database modified since last iteration. */
#define     SATLR_DUPLICATE_REC        ((INT32) -11)    /**< Existing record with the same key value is found. */
#define     SATLR_INV_FILE_PATH        ((INT32) -12)    /**< Invalid file path. */
#define     SATLR_OUT_OF_SPACE         ((INT32) -13)    /**< Failed to allocate space on file system. */
#define     SATLR_AEE_OUT_OF_RESOURCES ((INT32) -14)    /**< Can not allocate AEE resources. */
#define     SATLR_AEE_NO_RIGHTS        ((INT32) -15)    /**< No right for the AEE resources. */
#define     SATLR_FILE_READ_FAIL       ((INT32) -16)    /**< Failed to read from file. */
#define     SATLR_FILE_WRITE_FAIL      ((INT32) -17)    /**< Failed to write to file. */
#define     SATLR_INV_FILE_VER         ((INT32) -18)    /**< Incompatible file version. */


/* Handle type for the SATL handle. */
#define SATLT_SATELLITE_LIST (HT_GROUP_SATL + ((HANDLE_TYPE_T) 0))    /**< Handle type for the Satellite List. */

#define MAX_SAT_NAME_LEN   25    /**< Maximum length for satellite name. */

/* Enumeration for SATL database condition */
/*------------------------------------------------------------------*/
/*!  @brief  Enumeration for SATL Database conditions
 *  @code
 *  typedef  enum
 *  {
 *      SATL_UPDATING = 1,  
 *      SATL_UPDATED,      
 *      SATL_CLOSED,       
 *      SATL_UNKNOWN
 *  } SATL_COND_T;
 *  @endcode
 *  @li@c  SATL_UPDATING                 - When satl_lock is called.
 *  @li@c  SATL_UPDATED                  - When satl_unlock is called.
 *  @li@c  SATL_CLOSED                   - When satl_delete is called.
 *  @li@c  SATL_UNKNOWN                  - Unknown condition.
 */
/*------------------------------------------------------------------*/
typedef  enum
{
    SATL_UPDATING = 1,  /* When satl_lock is called. */
    SATL_UPDATED,      /* When satl_unlock is called. */
    SATL_CLOSED,       /* When satl_delete is called. */
    SATL_UNKNOWN
} SATL_COND_T;

/* Notification reasons */
#define     SATL_REASON_UNKNOWN  ((UINT32)(0x0000))    /**< Reason unknown */
#define     SATL_RECORD_ADD      (MAKE_BIT_MASK_32(1))    /**< Reason for adding records */
#define     SATL_RECORD_DEL      (MAKE_BIT_MASK_32(2))    /**< Reason for deleting records */
#define     SATL_RECORD_MOD      (MAKE_BIT_MASK_32(3))    /**< Reason for modifying records */

/* Max length of the satellite list name (bytes) */
#define     MAX_SATL_NAME_LEN           16    /**< Max length of the satellite list name (bytes). */

/* Size (byte) of SATL record header and data structure. */
#define	    SATL_LNB_DATA_PADDING       16    /**< Padding size (byte) of SATL LNB information structure. */
#define	    SATL_DISEQC_DATA_PADDING    24    /**< Padding size (byte) of SATL DiSEqC structure. */

/* Pre-set size of the SATL record structure. */
#define     SATL_REC_SIZE  (8+SATL_LNB_DATA_PADDING+SATL_DISEQC_DATA_PADDING+4+MAX_SAT_NAME_LEN+3) /**< Pre-set size of the SATL record structure. */


/* Typedef and constants for SATL record id query flag. */
typedef     UINT8                 SATL_REC_ID_QRY_T;   /**< Typedef for SATL record id query flag. */

#define     SATL_FIRST_REC_ID     ((SATL_REC_ID_QRY_T) 1)    /**< First record ID. */
#define     SATL_LAST_REC_ID      ((SATL_REC_ID_QRY_T) 2)    /**< Last record ID. */
#define     SATL_NEXT_REC_ID      ((SATL_REC_ID_QRY_T) 3)    /**< Next record ID. */

/* Typedef and constants for SATL memory mangement policy. */
typedef	    UINT8	              SATL_MEM_POLICY_T;   /**< Typedef for SATL memory mangement policy. */

#define	    SATL_MEM_STATIC       ((SATL_MEM_POLICY_T) 0)    /**< Static memory policy. */
#define	    SATL_MEM_DYNAMIC      ((SATL_MEM_POLICY_T) 1)    /**< Dynamic memory policy. */

/* Null SATL database version id. */
#define     SATL_NULL_VER_ID      ((UINT32)0)    /**< Null SATL database version id. */

/* NULL SATL record ID value */
#define     SATL_NULL_REC_ID      ((UINT16)0)    /**< NULL SATL record ID value */

/* NULL SATL ID value. */
#define     SATL_NULL_ID          ((UINT16)0)    /**< NULL SATL ID value. */

/* Typedef and constants for SATL record iteration flag. */
typedef	    UINT8                 SATL_ITERATE_T;   /**< Typedef for SATL record iteration flag. */

#define     SATL_NEXT_REC         ((SATL_ITERATE_T) 1)    /**< Next record */
#define     SATL_PREV_REC         ((SATL_ITERATE_T) 2)    /**< Previous record */
#define     SATL_FIRST_REC        ((SATL_ITERATE_T) 3)    /**< First record */
#define     SATL_LAST_REC         ((SATL_ITERATE_T) 4)    /**< Last record */

/*---------------------------------------------------------------------------
 Structures
----------------------------------------------------------------------------*/

/* Structure for the SATL data record */
/*------------------------------------------------------------------*/
/*! @brief  Structure for SATL data record
 *  @code
 *  typedef struct _SATL_REC_T
 *  {
 *      UINT16      ui2_satl_rec_id;
 *      UINT32      ui4_mask;                       
 *      union
 *      {
 *          TUNER_LNB_INFO_T            t_lnb;
 *          UINT8  _lnb_data_padding[SATL_LNB_DATA_PADDING];
 *      } ulnb;
 *      union
 *      {
 *          TUNER_DISEQC_INFO_T         t_diseqc_info;
 *          UINT8  _diseqc_data_padding[SATL_DISEQC_DATA_PADDING];
 *      } udiseqc;
 *      INT16       i2_orb_pos;                     
 *      CHAR        ac_sat_name[MAX_SAT_NAME_LEN];  
 *  } SATL_REC_T;
 *  @endcode
 *  @li@c  ui2_satl_rec_id               - SATL record ID
 *  @li@c  ui4_mask                      - User defined bit mask
 *  @li@c  t_lnb                         - LNB information
 *  @li@c  _lnb_data_padding             - LNB data padding
 *  @li@c  t_diseqc_info                 - DiSEqC information
 *  @li@c  _diseqc_data_padding          - DiSEqC data padding
 *  @li@c  i2_orb_pos                    - Orbit position
 *  @li@c  ac_sat_name[MAX_SAT_NAME_LEN] - Satellite name
 */
/*------------------------------------------------------------------*/
typedef struct _SATL_REC_T
{
    /* Satellite record id */
    UINT16      ui2_satl_rec_id;

    UINT32      ui4_mask;                       /* Bit mask */

    union
    {
        /* Following structure for LNB information is defined in
           'u_tuner.h'. */
        TUNER_LNB_INFO_T            t_lnb;

        /* Padding to make 'ulnb' union 16 bytes.  */
        UINT8  _lnb_data_padding[SATL_LNB_DATA_PADDING];
    } ulnb;

    union
    {
        /* Following structure for DiSEqC information is defined in
           'u_tuner.h'. */
        TUNER_DISEQC_INFO_T         t_diseqc_info;

        /* Padding to make 'udiseqc' union 24 bytes.  */
        UINT8  _diseqc_data_padding[SATL_DISEQC_DATA_PADDING];
    } udiseqc;

    INT16       i2_orb_pos;                     /* degree*10 and range will be -1800~+1800, minus is West, plus is East*/

    CHAR        ac_sat_name[MAX_SAT_NAME_LEN];  /* satellite name */

} SATL_REC_T;

/*
 * Structure for memory dump.
 */
/*------------------------------------------------------------------*/
/*!  @brief  Structure for memory dump
 *  @code
 *  typedef struct _SATL_MEM_DUMP_T_
 *  {
 *      UINT16      ui2_satl_id;
 *      CHAR        ps_satl_name[MAX_SATL_NAME_LEN+1];
 *      VOID*       pv_data;
 *      UINT32      ui4_data_len;
 *  }  SATL_MEM_DUMP_T ;
 *  @endcode
 *  @li@c  ui2_satl_id                   - ID of the Satellite List
 *  @li@c  ps_satl_name                  - Name of the Satellite List
 *  @li@c  pv_data                       - Memory dump of SATL database
 *  @li@c  ui4_data_len                  - Size of memory allocated for pv_data
 */
/*------------------------------------------------------------------*/
typedef struct _SATL_MEM_DUMP_T_
{
    UINT16      ui2_satl_id;
    CHAR        ps_satl_name[MAX_SATL_NAME_LEN+1];
    VOID*       pv_data;
    UINT32      ui4_data_len;
}  SATL_MEM_DUMP_T ;

/*---------------------------------------------------------------------------
 Type definition
----------------------------------------------------------------------------*/

/*
  typedef for the SATL handle callback function.  This fucntion
  is implemented by the client of SATL. 
*/
/*------------------------------------------------------------------*/
/*! @brief  Typedef for the SATL handle callback function.  This fucntion
 *          is implemented by the client of SATL.
 *  @param  [in]  h_satl            - Handle to the SATL object.
 *  @param  [in]  e_cond            - The status flag for the SATL object.
 *  @param  [in]  ui4_reason        - The reason for the notification.
 *  @param  [in]  pv_tag            - Private tag for the client.
 *  @param  [in]  ui4_data          - Data pass back to the application.
 *  @note   It is very important that the client MUST NOT try to read the SATL in
 *          the context of notification function.  Otherwise, a `Dead lock'
 *          situation would arise.  The client should only send a message to
 *          itself from the notification function or set a condition flag, and
 *          execute the read operation outside the context of the notification
 *          function.
 *          @par
 *          Client should also avoid doing CPU intensive task in the
 *          notification function.  If CPU intensive task needs to be performed,
 *          then the notification function should send a message to the client
 *          thread, and have the CPU intensive task executed in the client
 *          thread context.
 *  @see    c_satl_create, c_satl_open, c_satl_open_by_name, c_satl_fs_load

 */
/*------------------------------------------------------------------*/
typedef VOID (*x_satl_nfy_fct)
(
    HANDLE_T            h_satl,
    SATL_COND_T         e_cond,
    UINT32              ui4_reason,
    VOID*               pv_tag,
    UINT32              ui4_data
);

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
#endif /* _U_SATL_H_ */
