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
/*! @file u_cdb.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/cdb/u_cdb.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains core database related API definitions,
 *         which are exported to other Middleware components.
 *         
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_CDB_H_
#define _U_CDB_H_
/*----------------------------------------------------------------------------*/
/*! @defgroup   groupMW_CDB Core Database
 *  @ingroup    groupMW
 *  @brief      Core database in the middleware, which provides services to
 *              store, search, sort data records, and to save / load records to
 *              / from file system.
 *  @see        groupMW_UTIL,
 *              groupMW_SVL, groupMW_TSL, groupMW_NWL, groupMW_LOL,
 *              groupMW_SCDB, groupMW_FLM, groupMW_RRCTX, groupMW_CONFIG
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle_grp.h"
/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/
    
/* Return code from API */
#define   DBR_OK                         ((INT32)     0)    /**< Operation succeeded. */
#define   DBR_FAIL                       ((INT32)    -1)    /**< Operation failed. */
#define   DBR_OUT_OF_MEMORY              ((INT32)    -2)    /**< Memory exhausted. */
#define   DBR_INV_HANDLE                 ((INT32)    -3)    /**< Bad handle. */
#define   DBR_OUT_OF_HANDLE              ((INT32)    -4)    /**< Handle allocation failed. */
#define   DBR_INV_ARG                    ((INT32)    -5)    /**< Invalid argument. */
#define   DBR_NOT_FOUND                  ((INT32)    -6)    /**< Record not found. */
#define   DBR_MODIFIED                   ((INT32)    -7)    /**< Database modified since last iteration. */
#define   DBR_NO_WRITE_LOCK              ((INT32)    -8)    /**< Failed initializing RW Lock. */
#define   DBR_SEARCH_KEY_MODIFIED        ((INT32)    -9)    /**< Search key parameters modified since last iteration. */
#define   DBR_NO_SORTED_LIST             ((INT32)   -10)    /**< DB not sorted. */
#define   DBR_END_OF_REC                 ((INT32)   -11)    /**< Iteration reached boundary. */
#define   DBR_WRITE_LOCKED               ((INT32)   -12)    /**< Failed to acquire lock as DB is write locked. */
#define   DBR_DB_LIMIT_EXCEED            ((INT32)   -13)    /**< Reached maximum number of CDB objects */
#define   DBR_REC_NOT_FOUND              ((INT32)   -14)    /**< Record not found with matching key values. */
#define   DBR_INV_NAME                   ((INT32)   -15)    /**< DB name already in use */
#define   DBR_FILE_READ_FAIL             ((INT32)   -16)    /**< Failed to read from file. */
#define   DBR_FILE_WRITE_FAIL            ((INT32)   -17)    /**< Failed to write to file. */
#define   DBR_BUF_TOO_SMALL              ((INT32)   -18)    /**< Buffer too small to fit the data size. */
#define   DBR_AEE_OUT_OF_RESOURCES       ((INT32)   -19)    /**< AEE run out of resources. */
#define   DBR_AEE_NO_RIGHTS              ((INT32)   -20)    /**< AEE no rights. */


#ifndef DB_NULL_RECORD
#define DB_NULL_RECORD  0    /**< Initial cursor value pointing to an invalid record. */
#endif

/*--------------------------------------
  NULL CDB version id.
----------------------------------------*/
#ifndef CDB_NULL_VER_ID
#define CDB_NULL_VER_ID  ((UINT32)0)    /**< NULL CDB version ID used at the start of the iteration. */
#endif

/*------------------------------------------
  Handle type for the database client object
--------------------------------------------*/
#define CDBT_COREDB  (HT_GROUP_CDB + ((HANDLE_TYPE_T) 0))    /**< Handle type for the database client object. */

/*
  Enumeration flags for memory management policy.
*/
/*------------------------------------------------------------------*/
/*! @brief  Enumeration flags for memory management policy.
 *  @code
 *  typedef enum 
 *  {
 *      DB_MEM_STATIC = 0,
 *      DB_MEM_DYNAMIC 
 *  }  DB_MEM_POLICY_T;
 *  @endcode
 *  @li@c  DB_MEM_STATIC                 - Static memory policy
 *  @li@c  DB_MEM_DYNAMIC                - Dynamic memory policy
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    DB_MEM_STATIC = 0,
    DB_MEM_DYNAMIC 
}  DB_MEM_POLICY_T;

/*
  CDB notify condition flag
*/
/*------------------------------------------------------------------*/
/*! @brief  Database notification condition flag.
 *  @code
 *  typedef enum
 *  {
 *      DB_UPDATING = 1,
 *      DB_UPDATED,
 *      DB_CLOSED
 *  }  DB_COND_T ;
 *  @endcode
 *  @li@c  DB_UPDATING                   - DB is write locked by client or
 *                                         being cleaned.
 *  @li@c  DB_UPDATED                    - DB is write unlocked by client or
 *                                         has finished cleaning.
 *  @li@c  DB_CLOSED                     - Database object is deleted.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DB_UPDATING = 1,
    DB_UPDATED,
    DB_CLOSED
}  DB_COND_T ;

/*
  CDB notification reasons
*/
#define   DB_REASON_UNK       ((UINT32)(0x0000))    /**< Reason unknown */
#define   DB_RECORD_ADD       (MAKE_BIT_MASK_32(1))    /**< Reason for adding records */
#define   DB_RECORD_DEL       (MAKE_BIT_MASK_32(2))    /**< Reason for deleting records */
#define   DB_RECORD_MOD       (MAKE_BIT_MASK_32(3))    /**< Reason for modifying records */
#define   DB_COMMON_MOD       (MAKE_BIT_MASK_32(4))    /**< Reason for modifying common data */
 
/*-----------------------------------------------------------------------
   Field type description (C Lang types)  0-63 possible values.
 -----------------------------------------------------------------------*/
#define DBF_UNKNOWN             0    /**< Unknown field type */

#define DBF_UINT8_T             1    /**< Unsigned 8-bit interger field type */
#define DBF_INT8_T              2    /**< Signed 8-bit integer field type */
#define DBF_CHAR_T              3    /**< Character field type */
#define DBF_ENUM8_T             4    /**< 8-bit enumerator type */

#define DBF_UINT16_T            7    /**< Unsigned 16-bit interger field type */
#define DBF_INT16_T             8    /**< Signed 16-bit integer field type */
#define DBF_UTF16_T             9    /**< UTF-16 code field type */
#define DBF_ENUM16_T            10    /**< 16-bit enumerator type */

#define DBF_UINT32_T            13    /**< Unsigned 32-bit interger field type */
#define DBF_INT32_T             14    /**< Signed 32-bit integer field type */
#define DBF_UTF32_T             15    /**< UTF-32 code field type */
#define DBF_FLOAT32_T           16    /**< 32-bit float point field type */
#define DBF_ENUM32_T            17    /**< 32-bit enumerator type */

#define DBF_UINT64_T            19    /**< Unsigned 64-bit interger field type */
#define DBF_INT64_T             20    /**< Signed 64-bit integer field type */
#define DBF_FLOAT64_T           21    /**< 64-bit float point field type */

#define DBF_UNION_TAG_T         26    /**< Union tag field type (32-bit) */
#define DBF_STRING_T            27    /**< String field type (16-bit for ID) */
#define DBF_UNION_T             28    /**< Union field type (user specify length) */
#define DBF_MEMBUF_T            29    /**< Memory object field type (16-bit for ID) */

#define DBF_START_OF_ARRAY_TYPE 32    /**< Start of array field type */

#define DBF_ARRAY_OF_UINT8_T    32    /**< Array of unsigned 8-bit interger field type */
#define DBF_ARRAY_OF_INT8_T     33    /**< Array of signed 8-bit integer field type */
#define DBF_ARRAY_OF_CHAR_T     34    /**< Array of character field type */
#define DBF_ARRAY_OF_ENUM8_T    35    /**< Array of 8-bit enumerator type */

#define DBF_ARRAY_OF_UINT16_T   38    /**< Array of unsigned 16-bit interger field type */
#define DBF_ARRAY_OF_INT16_T    39    /**< Array of signed 16-bit integer field type */
#define DBF_ARRAY_OF_UTF16_T    40    /**< Array of UTF-16 code field type */
#define DBF_ARRAY_OF_ENUM16_T   41    /**< Array of 16-bit enumerator type */

#define DBF_ARRAY_OF_UINT32_T   44    /**< Array of unsigned 32-bit interger field type */
#define DBF_ARRAY_OF_INT32_T    45    /**< Array of signed 32-bit integer field type */
#define DBF_ARRAY_OF_UTF32_T    46    /**< Array of UTF-32 code field type */
#define DBF_ARRAY_OF_FLOAT32_T  47    /**< Array of 32-bit float point field type */
#define DBF_ARRAY_OF_ENUM32_T   48    /**< Array of 32-bit enumerator type */

#define DBF_ARRAY_OF_UINT64_T   50    /**< Array of unsigned 64-bit interger field type */
#define DBF_ARRAY_OF_INT64_T    51    /**< Array of signed 64-bit integer field type */
#define DBF_ARRAY_OF_FLOAT64_T  52    /**< Array of 64-bit float point field type */

#define DBF_END_OF_ARRAY_TYPE   63    /**< End of array field type */

/* Macros for defining field type. */ 
#define DB_MAKE_FIELD_TYPE(_C_LANG_TYPE_, _FIELD_TYPE_DESCRIPTOR_)  \
          ( (_FIELD_TYPE_DESCRIPTOR_ << 6) | _C_LANG_TYPE_ )    /**< Macros for defining field type. For field type specified by CrystalMedia, the field type descriptor parameter (2nd argument) should be between 1 and 33554431.   Manufacture should specify value between 33554432 and 67108863. */

/*---------------------------------------------------------------
  Maximum char length for the database object name.
 ----------------------------------------------------------------*/
#define CDB_MAX_DATABASE_NAME_LEN     31    /**< Maximum length of CDB database names */


/*---------------------------------------------------------------
  Enumeration flags for controlling iteration, sorting, updating and
  write operation
-----------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief  Enumeration flags for write, read and search, and sort operations
 *  @code
 *  typedef enum  
 *  {
 *      DB_FIRST_RECORD = 1 ,
 *      DB_LAST_RECORD,
 *      DB_NEXT_RECORD,
 *      DB_PREV_RECORD,
 *      DB_GOTO_RECORD,
 *      DB_UPDATE_RECORD,
 *      DB_APPEND_RECORD,
 *      DB_MODIFY_RECORD
 *  }  DB_CTRL_FLAG_T;
 *  @endcode
 *  @li@c  DB_FIRST_RECORD               - position the iteration cursor at the
 *                                         first entry of the sorted list
 *  @li@c  DB_LAST_RECORD                - position the iteration cursor at the
 *                                         last entry of the sorted list
 *  @li@c  DB_NEXT_RECORD                - move the cursor to the next entry in
 *                                         the sorted list
 *  @li@c  DB_PREV_RECORD                - move the cursor to the previous entry
 *                                         in the sorted list
 *  @li@c  DB_GOTO_RECORD                - move the cursor to specified position
 *  @li@c  DB_UPDATE_RECORD              - if the record is already in the
 *                                         database, it is over-written with new
 *                                         values; otherwise a new record is
 *                                         created
 *  @li@c  DB_APPEND_RECORD              - record is added to the database
 *                                         object
 *  @li@c  DB_MODIFY_RECORD              - an existing record is modified with
 *                                         new values.  It is an error if the
 *                                         specified record is not found
 */
/*------------------------------------------------------------------*/
typedef enum  
{
    DB_FIRST_RECORD = 1 ,
    DB_LAST_RECORD,
    DB_NEXT_RECORD,
    DB_PREV_RECORD,
    DB_GOTO_RECORD,
    DB_UPDATE_RECORD,
    DB_APPEND_RECORD,
    DB_MODIFY_RECORD
}  DB_CTRL_FLAG_T;

/*-------------------------------------------------------------------------
 * Type definition
 --------------------------------------------------------------------------*/

/*
  typedef for the CDB callback function. This function is to be registered
  with the CDB library.
*/
/*------------------------------------------------------------------*/
/*! @brief  Prototype of the notification function to be called when the
 *          database object is modified
 *  @param  [in]  h_client              - Specifies the handle to the database
 *                                        object.
 *  @param  [in]  e_cond                - Specifies the condition of the
 *                                        database object: DB_UPDATING,
 *                                        DB_UPDATED,  DB_CLOSED.
 *  @param  [in]  ui4_reason            - Reason for the notification.
 *                                        Application can check the reason by
 *                                        doing an AND operation with following
 *                                        bit mask DB_RECORD_ADD, DB_RECORD_DEL,
 *                                        DB_RECORD_MOD, and DB_UNKNOWN.
 *  @param  [in]  pv_tag                - Private tag for the client.
 *  @param  [in]  ui4_version           - The version number of the database
 *                                        object.  The version number is
 *                                        incremented each time when database is
 *                                        modified.  Application should check
 *                                        this version with the client version
 *                                        to see if any action needs to be done.
 *  @note   This function is called from the context of the thread that modifies
 *          the database.  The client MUST NOT try to call API's that read the
 *          database, otherwise the `Dead Lock' would result. Furthermore, in
 *          the notification function, the client application should do minimum
 *          work to avoid holding this thread for long time.  If it is necessary
 *          for an application to do a lots of data processing in the event of
 *          an notification, for example, such as updating OSD, then it is
 *          recommended that sending a message (or set a flag) in the
 *          notification function.  The real data processing is done in the
 *          client application thread.
 *  @see    db_create, db_get_handle_by_name
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef VOID (*db_notify_fct)
(
    HANDLE_T	h_db_obj,
    DB_COND_T   e_cond,
    UINT32		ui4_reason,
    VOID*       pv_tag,
    UINT32		ui4_db_version
);

/* typedef for Field type */
typedef  UINT32  DB_FIELD_T;    /**< typedef for field type */

/* typedef for a field description. */
/*------------------------------------------------------------------*/
/*! @brief  Structure for field description (Field type and element count)
 *  @code
 *  typedef struct _DB_FIELD_INFO_T
 *  { 
 *  	DB_FIELD_T	ui4_field_type; 
 *  	UINT16	    ui2_field_element_count; 
 *  } DB_FIELD_INFO_T;
 *  @endcode
 *  @li@c  ui4_field_type                - Specifying the field type for this
 *                                         field.
 *  @li@c  ui2_field_element_count       - Specifying the number of element
 *                                         counts for this field. If this field
 *                                         is not an array type, then the
 *                                         database library ignores the
 *                                         aui2_field_element_count value.
 */
/*------------------------------------------------------------------*/
typedef struct _DB_FIELD_INFO_T
{ 
	DB_FIELD_T	ui4_field_type; 
	UINT16	    ui2_field_element_count; 
} DB_FIELD_INFO_T;

/*--------------------------------------
  typedef for compare funtion opcode.
----------------------------------------*/
typedef  UINT8   DB_CMP_OPCODE_T;    /**< typedef for compare funtion operation code. */

#define  DB_OPS_EQUAL			((DB_CMP_OPCODE_T)1)    /**< Equality test comparson */
#define  DB_OPS_BITWISE_AND		((DB_CMP_OPCODE_T)2)    /**< Bitwise AND comparison */

/*----------------------------------
   typedef for the  UNION tag type.
-----------------------------------*/
typedef  INT32   DB_UNION_TAG_T;    /**< typedef for the INT32 value store in the union tag field of the record */


/*------------------------------------------------------------------*/
/*! @brief  Prototype for the caller provided function to interpret and compare
 *          two DBF_UNION_T record fields for sort and search operation.  This
 *          function must be provided when caller specifies key field that is of
 *          DBF_UNION_T field for sort operation
 *  @param  [in]  pv_rec_fld_A      - Pointer to the first field to be compared.
 *                                    For search operation, this will pointer to
 *                                    the caller specified search value.  For
 *                                    sorting operation, this will be a pointer
 *                                    to a field of a record entry in the
 *                                    database.
 *  @param  [in]  pv_rec_fld_B      - Pointer to the second field to be
 *                                    compared. For search or sort operation,
 *                                    this will be the pointer a field of a
 *                                    record entry in the database.
 *  @param  [in]  e_union_tag       - Specifies the UNION TAG value of the UNION
 *                                    field.  The UNION TAG is defined by the
 *                                    caller when it created the database record
 *                                    definition.  This function use the UNION
 *                                    TAG value to decide how to interpret the
 *                                    pointers 'pv_rec_fld_A' and
 *                                    'pv_rec_fld_B'.
 *  @param  [in]  ui4_size          - Size (in bytes) of the UNION elements
 *  @param  [in]  e_opcode          - Flag for specifying how the two fields
 *                                    should be evaluated.
 *  @param  [in]  pv_tag            - Private caller data.
 *  @note   When this function is called during search (e.g., read record)
 *          operation, `pv_rec_fld_A' and `ui4_union_tag_A' are values from the
 *          internal CDB record, where as `pv_rec_fld_B' and `ui4_union_tag_B'
 *          are the key field values specified by the caller.  The record is
 *          considered a positive match only if the return value is 0.  
 *  @see    DB_SEARCH_T
 *  @return Return the comparison result
 *  @retval 0               - Two record field values are the same
 *  @retval Greater than 0  - content of pv_rec_fld_A > content of pv_rec_fld_B
 *  @retval Less than 0     - content of pv_rec_fld_A < content of pv_rec_fld_B
 */
/*------------------------------------------------------------------*/
typedef  INT32 (*db_union_cmp_fct)
(
    const VOID*        pv_rec_fld_A, 
    const VOID*        pv_rec_fld_B,
    DB_UNION_TAG_T     e_union_tag,
    UINT32             ui4_size, 
    DB_CMP_OPCODE_T    e_opcode,
    VOID*              pv_tag
);

/*--------------------------------------------------------------
 *  Typedefs for specifying the search operation performed on 
 *  record field.
---------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief  Structure for specifying the search operation performed on record
 *          field
 *  @code
 *  typedef struct _DB_SEARCH_T
 *  {
 *      DB_CMP_OPCODE_T	        e_opcode;
 *      db_union_cmp_fct        pf_fct;
 *      DB_UNION_TAG_T          e_tag_val;
 *      VOID*                   pv_tag;
 *  } DB_SEARCH_T;
 *  @endcode
 *  @li@c  e_opcode                      - Specifying search operation for the
 *                                         given record field. If this value is
 *                                         `DB_OPS_DEFAULT', then default
 *                                         comparison function is used, which
 *                                         does `==' comparison between the
 *                                         record fields and key field values.
 *                                         Additional argument is then ignored.
 *  @li@c  pf_fct                        - If the field specified in the search
 *                                         parameters is an DBF_UNION_T type,
 *                                         then caller must provides a search
 *                                         function to interpret the meaning of
 *                                         the field. The CDB library will call
 *                                         this function when evaluating the
 *                                         UNION_T field that has the same
 *                                         DB_UNION_TAG_T value. 
 *  @li@c  e_tag_val                     - Specifies the union tag value.  This
 *                                         value is passed to the `pf_fct' for
 *                                         interpreting the key field that is
 *                                         type DBF_UNION_T.
 *  @li@c  pv_tag                        - Private tag to pass to the caller
 *                                         provided compare/search function.
 */
/*------------------------------------------------------------------*/
typedef struct _DB_SEARCH_T
{
    DB_CMP_OPCODE_T	        e_opcode;
    db_union_cmp_fct        pf_fct;
    DB_UNION_TAG_T          e_tag_val;
    VOID*                   pv_tag;
} DB_SEARCH_T;

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
#endif /* _U_CDB_H_ */
