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
/*! @file x_scdb.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/scdb/x_scdb_api.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains stream component database API definitions,
 *         which are exported to other Middleware components and applications.
 *         
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_SCDB_API_H_
#define _X_SCDB_API_H_
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_SCDB
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/


#include "u_scdb.h"

/*-------------------------------------------------------------------------
                    functions declarations
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief  This API returns a handle to the SCDB object for a
 *          specified connection.
 *  @param  [in]  h_connection      - Specifies the connection handle that was
 *                                    used to create the SCDB object.
 *  @param  [in]  pv_tag            - Private tag value associated with the
 *                                    calling client.
 *  @param  [in]  pf_nfy            - Client notification function.
 *  @param  [out] ph_scdb           - pointer to a SCDB handle.
 *  @return Return the error code
 *  @retval SCDBR_OK            - A new handle is created
 *  @retval SCDBR_OUT_OF_HANDLE - No more handle available.
 *  @retval SCDBR_INV_HANDLE    - bad 'h_connection' handle.
 *  @retval SCDBR_OUT_OF_MEM    - Out of memory.
 *  @retval SCDBR_INV_ARG       - ph_scdb is NULL.
 *  @retval SCDBR_NOT_FOUND     - SCDB database for the specified connection is
 *                                not found.
 *  @retval SCDBR_AEE_OUT_OF_RESOURCES  - Can not allocate AEE resources.
 *  @retval SCDBR_AEE_NO_RIGHTS - No right for the AEE resources.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scdb_open
(
    HANDLE_T                  h_connection,
    HANDLE_T*                 ph_scdb,
    VOID*                     pv_tag,
    x_scdb_nfy_fct            pf_nfy
);

/*------------------------------------------------------------------*/
/*! @brief  This API frees the specified handle to the SCDB database.
 *  @param  [in]  h_scdb            - Handle to be freed.
 *  @return Return the error code
 *  @retval SCDBR_OK            - SCDB handle and its resource are free.
 *  @retval SCDBR_INV_HANDLE    - Bad SCDB handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scdb_close
(
    HANDLE_T                  h_scdb
);

/*------------------------------------------------------------------*/
/*! @brief  This API get the shared metadata about the records stored
 *          in the SCDB database object.
 *  @param  [in]  h_scdb            - Handle to the SCDB object.
 *  @param  [out] pt_scdb_common    - Pointer to a structure containing SCDB
 *                                    common data.
 *  @return Return the error code
 *  @retval SCDBR_OK            - Common data is copied.
 *  @retval SCDBR_INV_HANDLE    - Bad SCDB handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scdb_get_common
(
    HANDLE_T                  h_scdb,
    SCDB_COMMON_T*            pt_scdb_common 
); 


/*------------------------------------------------------------------*/
/*! @brief  This API gets a record with a specified stream type
 *          and stream_id value from the database.
 *  @param  [in]  h_scdb            - Handle to the SCDB object.
 *  @param  [in]  pt_stream_descr   - Pointer to the structure specifying the
 *                                    stream type and stream id.
 *  @param  [out] pt_scdb_rec       - Pointer to the structure containing the
 *                                    stream component data.
 *  @return Return the error code
 *  @retval SCDBR_OK                - A SCDB_REC_T structure is returned.
 *  @retval SCDBR_INV_HANDLE        - Bad 'h_scdb' handle.
 *  @retval SCDBR_INVA_ARG          - 'pt_stream_descr' or 'pt_scdb_rec'
 *                                    is NULL.
 *  @retval SCDBR_REC_NOT_FOUND     - Specified record is not found.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scdb_get_rec
(
    HANDLE_T                  h_scdb,
    const STREAM_COMP_ID_T*   pt_stream_descr,
    SCDB_REC_T*               pt_scdb_rec
);


/*------------------------------------------------------------------*/
/*! @brief  This API gets a record from the SCDB database object. The
 *          record is specified by the stream_type_t value and an index
 *          value.  If the specified 'stream_type_t' is ST_UNKONWN,
 *          then SCDB record matching any stream type is returned to
 *          the caller.
 *          To get the next record, the index counter 'ui2_idx'
 *          is incremented.
 *  @param  [in]  h_scdb            - Client handle.
 *  @param  [in]  e_stream_type     - Specify the stream type.
 *  @param  [in]  ui2_idx           - Index value for selecting the record that
 *                                    match the specified stream type. 0 being
 *                                    the first match, 1 the second match, etc.
 *  @param  [out] pt_stream_descr   - Pointer to a stream descriptor structure
 *                                    holding the stream type and stream id of
 *                                    the record returned.
 *  @param  [out] pt_scdb_rec       - Pointer to a SCDB record.
 *  @param  [out] pui4_db_version   - Current version number of the database.
 *  @see    x_scdb_get_num_recs
 *  @return Return the error code
 *  @retval SCDBR_OK            - A SCDB_REC_T structure is returned.
 *  @retval SCDBR_INV_HANDLE    - Bad 'h_scdb' handle.
 *  @retval SCDBR_INVALIE_ARG   - 'pt_stream_descr' or 'pt_scdb_rec'
 *                                is NULL.
 *  @retval SCDBR_REC_NOT_FOUND - Specified record is not found.
 *  @retval SCDBR_DB_MODIFIED   - Between read operations of the
 *                                stream component database using
 *                                different idx value, the
 *                                records has been modified.  The
 *                                application should re-starts
 *                                the read operation with idx=0
 */
/*------------------------------------------------------------------*/
extern INT32 x_scdb_get_rec_by_idx
(
    HANDLE_T            h_scdb,
    STREAM_TYPE_T       e_stream_type,
    UINT16              ui2_idx,
    STREAM_COMP_ID_T*   pt_stream_descr,
    SCDB_REC_T*         pt_scdb_rec,
    UINT32*             pui4_db_version    
);

/*------------------------------------------------------------------*/
/*! @brief  This API returns the number of records matching the specified
 *          stream type.
 *  @param  [in]  h_scdb            - Handle to the SCDB database.
 *  @param  [in]  e_stream_type     - Specify the stream type.
 *  @param  [out] pui2_num_recs     - Pointer to a variable holding the result.
 *  @param  [out] pui4_db_version   - Current version number of the SCDB
 *                                    database.
 *  @see    x_scdb_get_rec_by_idx
 *  @return Return the error code
 *  @retval SCDBR_OK            - Number of records found is returned.
 *  @retval SCDBR_INV_ARG       - 'pui2_num_recs' is a NULL pointer.
 *  @retval SCDBR_INV_HANDLE    - Bad 'h_scdb' handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scdb_get_num_recs
(
    HANDLE_T            h_scdb,
    STREAM_TYPE_T       e_stream_type,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_db_version    
);

/*------------------------------------------------------------------*/
/*! @brief  This function register compare functions for sorting
 *          operation. The compare function are SCDB handle specific.
 *  @param  [in]  h_scdb            - Handle to the SCDB database.
 *  @param  [in]  pv_tag            - Tag for the compare function.
 *  @param  [in]  pf_sort_cmp       - Compare function used for sorting.
 *  @return Return the error code
 *  @retval SCDBR_OK            - Operation is successful.
 *  @retval SCDBR_INV_ARG       - 'pf_sort_cmp' is a NULL pointer.
 *  @retval SCDBR_INV_HANDLE    - Bad 'h_scdb' handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scdb_reg_cmp_fcts
(
    HANDLE_T               h_scdb,
    VOID*                  pv_tag,
    x_scdb_sort_cmp        pf_sort_cmp
);

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
#endif /* _X_SCDB_API_H_ */
