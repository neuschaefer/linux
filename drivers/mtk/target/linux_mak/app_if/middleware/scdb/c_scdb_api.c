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
 * $RCSfile: c_scdb_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains implementation of the stream component database
 *         application API's,  
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/


#include "scdb/c_scdb.h"
#include "scdb/x_scdb_api.h"
#include "handle/handle.h"
#include "aee/aee.h"
/*-------------------------------------------------------------------------
                    functions implementations
 --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  c_scdb_open
 *
 * Description: This API returns a handle to the SCDB object for a
 *              specified connection. 
 *
 * Inputs:  -  h_connection:   Specifies the connection handle that was 
 *                             used to create the SCDB object.
 *             pv_tag:         Private tag value associated with the 
 *                             calling client.
 *             pf_nfy:         Client notification function.
 *
 * Outputs: - ph_scdb:  pointer to a SCDB handle.
 *
 * Returns: -
 *    SCDBR_OK                    A new handle is created
 *    SCDBR_OUT_OF_HANDLE:        No more handle available.
 *    SCDBR_INV_HANDLE:           bad 'h_connection' handle.
 *    SCDBR_OUT_OF_MEM:           Out of memory.
 *    SCDBR_INV_ARG:              ph_scdb is NULL.
 *    SCDBR_NOT_FOUND:            SCDB database for the specified
 *                                connection is not found.
 *    SCDBR_AEE_OUT_OF_RESOURCES: Can not allocate AEE resources.
 *    SCDBR_AEE_NO_RIGHTS:        No right for the AEE resources. 
 ----------------------------------------------------------------------------*/
INT32 c_scdb_open
(
    HANDLE_T                  h_connection,
    HANDLE_T*                 ph_scdb,
    VOID*                     pv_tag,
    x_scdb_nfy_fct            pf_nfy
)
{
    HANDLE_T  h_aux;
    INT32     i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);

    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_scdb_open(h_connection, ph_scdb, pv_tag, pf_nfy);
            
            if (i4_rc == SCDBR_OK)
            {
                handle_link_to_aux(h_aux, *ph_scdb);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = SCDBR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = SCDBR_AEE_NO_RIGHTS;
        }
        break;
    }
    return i4_rc;
}

/*---------------------------------------------------------------------------
 * Name:  c_scdb_close
 *
 * Description: This API frees the specified handle to the SCDB database.
 *
 * Inputs:  - h_scdb:  handle to be freed.
 *
 * Outputs: - None.
 *
 * Returns: - SCDBR_OK    SCDB handle and its resource are free.
 *            SCDBR_INV_HANDLE  Bad SCDB handle.
 ----------------------------------------------------------------------------*/
INT32 c_scdb_close
(
    HANDLE_T                  h_scdb
)
{
    INT32     i4_rc;
    
    i4_rc = x_scdb_close(h_scdb);    

    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name:  c_scdb_get_common
 *
 * Description: This API get the shared metadata about the records stored
 *              in the SCDB database object.
 *
 * Inputs: 
 *     h_scdb:  handle to the SCDB object.
 *
 * Outputs: 
 *     pt_scdb_common: pointer to a structure containing SCDB common data.
 *
 * Returns: -
 *     SCDBR_OK          Common data is copied.
 *     SCDBR_INV_HANDLE  Bad SCDB handle.
 --------------------------------------------------------------------------*/
INT32 c_scdb_get_common
(
    HANDLE_T                  h_scdb,
    SCDB_COMMON_T*            pt_scdb_common 
)
{
    return x_scdb_get_common(h_scdb, pt_scdb_common);
} 

/*-----------------------------------------------------------------------------
 * Name:  c_scdb_get_rec
 *
 * Description: This API gets a record with a specified stream type
 *              and stream_id value from the database.
 *
 * Inputs:  -
 *    h_scdb          handle to the SCDB object.
 *    pt_stream_desc  pointer to the structure specifying the
 *                    stream type and stream id.
 *
 * Outputs: -
 *    pt_scdb_rec  pointer to the structure containing the
 *                 stream component data.
 *
 * Returns: -
 *    SCDBR_OK:             A SCDB_REC_T structure is returned
 *    SCDBR_INV_HANDLE: Bad 'h_scdb' handle.
 *    SCDBR_INVA_ARG:       'pt_stream_descr' or 'pt_scdb_rec' 
 *                          is NULL.
 *    SCDBR_REC_NOT_FOUND:  Specified record is not found.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scdb_get_rec
(
    HANDLE_T                  h_scdb,
    const STREAM_COMP_ID_T*   pt_stream_descr,
    SCDB_REC_T*               pt_scdb_rec
)
{
    return x_scdb_get_rec(h_scdb, pt_stream_descr, pt_scdb_rec);
}

/*-----------------------------------------------------------------------------
 * Name:  c_scdb_get_rec_by_idx
 *
 * Description: This API gets a record from the SCDB database object. The
 *              record is specified by the stream_type_t value and an index
 *              value.  If the specified 'stream_type_t' is ST_UNKONWN,
 *              then SCDB record matching any stream type is returned to
 *              the caller.
 *              To get the next record, the index counter 'ui2_idx'
 *              is incremented. 
 *  
 *
 * Inputs:  - h_scdb          client handle 
 *            e_stream_type   specify the stream type.
 *            ui2_idx         index value for selecting the record that
 *                            match the specified stream type. 0 being 
 *                            the first match, 1 the second match, etc.
 *            
 * Outputs: - pt_stream_desc: pointer to a stream descriptor structure
 *                            holding the stream type and stream id of
 *                            the record returned.
 *
 *            pt_scdb_rec:    pointer to a SCDB record.
 *            pui4_db_version: current version number of the database.
 *
 * Returns: - SCDBR_OK:              A SCDB_REC_T structure is returned
 *            SCDBR_INV_HANDLE:      Bad 'h_scdb' handle.
 *            SCDBR_INVALIE_ARG:     'pt_stream_descr' or 'pt_scdb_rec' 
 *                                   is NULL.
 *            SCDBR_REC_NOT_FOUND:   Specified record is not found.
 *            SCDBR_DB_MODIFIED:     Between read operations of the
 *                                   stream component database using
 *                                   different idx value, the
 *                                   records has been modified.  The
 *                                   application should re-starts
 *                                   the read operation with idx=0
 ----------------------------------------------------------------------------*/
INT32 c_scdb_get_rec_by_idx
(
    HANDLE_T            h_scdb,
    STREAM_TYPE_T       e_stream_type,
    UINT16              ui2_idx,
    STREAM_COMP_ID_T*   pt_stream_descr,
    SCDB_REC_T*         pt_scdb_rec,
    UINT32*             pui4_db_version    
)
{
    return x_scdb_get_rec_by_idx(h_scdb, e_stream_type, ui2_idx,
                                 pt_stream_descr, pt_scdb_rec,
                                 pui4_db_version);
}

/*-----------------------------------------------------------------------------
 * Name:  c_scdb_get_num_recs
 *
 * Description: This API returns the number of records matching the specified
 *              stream type.
 *
 * Inputs:  - h_scdb:  handle to the SCDB database.
 *            e_stream_type:  specify the stream type.
 *
 * Outputs: - pui2_num_recs:  pointer to a variable holding the result.
 *            pui4_db_version: current version number of the SCDB database.
 *
 * Returns: - SCDBR_OK          Number of records found is returned
 *            SCDBR_INV_ARG     'pui2_num_recs' is a NULL pointer.
 *            SCDBR_INV_HANDLE   Bad 'h_scdb' handle. 
 ----------------------------------------------------------------------------*/
INT32 c_scdb_get_num_recs
(
    HANDLE_T            h_scdb,
    STREAM_TYPE_T       e_stream_type,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_db_version    
)
{
    return x_scdb_get_num_recs(h_scdb, e_stream_type, pui2_num_recs,
                               pui4_db_version);
}


