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
 * $RCSfile: svl_rec_id.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements service record ID query/next functions.
 *   
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "cdb/cdb_api.h"
#include "svl/_svl.h"
#include "svl/svl_rec_id.h"

/**********************************************************************
   Following are private functions only known to the SVL library.
   The private functions are for adding new SVL id/name, deleting
   SVL id/name, from the internal SVL id/name arrays.  They are
   in this file because these private functions need to access
   the internal SVL id/name arrays

***********************************************************************/
/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: svl_reset_rec_id
 *
 * Description: This function reset the SVL record ID library. 
 *
 * Inputs: 
 *    pt_svl:   SVL structure containing the smallest and largest record id
 *              currently in used for the database object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL library is initialized successfully.
 *   SVLR_INV_ARG      'ui2_max_num_svl_obj' is 0.
 *
 ----------------------------------------------------------------------------*/
BOOL svl_reset_rec_id(SVL_OBJ_T*    	pt_svl)
{
    /*
      An UINT16 array to put the minimum and maximum
      record id value.
    */
    UINT16  aui2_min_max[2];
    INT32   i4_rc;
    
    /*
      Reset the SVL id values.
    */
    aui2_min_max[0]=SVL_SMALLEST_INIT_VAL;
    aui2_min_max[1]=SVL_NULL_REC_ID;

    /*
      Save the min and max record id value to the
      database object.
    */
    i4_rc=db_write_common(pt_svl->h_cdb,aui2_min_max,
                          (UINT16)sizeof(aui2_min_max));
    if ( i4_rc != DBR_OK )
    {
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL );
    }
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: svl_get_next_rec_id
 *
 * Description: This function get the next available SVL record ID.
 *
 * Inputs:  
 *    pt_svl:   SVL structure containing the smallest and largest record id
 *              currently in used.
 *
 * Outputs:
 *    pui2_next_id:  the next available record id.
 *
 * Returns: 
 *   SVLR_OK          SVL ID/Name is added.
 *   SVLR_OUT_OF_RES  Can not add the new SVL ID/Name.
 ----------------------------------------------------------------------------*/
BOOL svl_get_next_rec_id(const SVL_OBJ_T* pt_svl, UINT16* pui2_next_id)
{
    UINT16  aui2_min_max[2];
    UINT16  ui2_i;

    /*
      Get the min and max record id used by the database.
    */
    db_read_common(pt_svl->h_cdb, aui2_min_max,
                   (UINT16)sizeof(aui2_min_max));
    
    ui2_i=(UINT16)(aui2_min_max[1]+1);
    /*
      Check for overflow. If overflow occurs, return FALSE.
    */
    if ( ui2_i > aui2_min_max[1] )
    {
        *pui2_next_id = ui2_i;
    }
    else
    {
        *pui2_next_id = SVL_NULL_REC_ID;
        return FALSE;
    }
    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name: svl_get_smallest_rec_id
 *
 * Description: This function returns the smallest rec id value.
 *
 * Inputs: 
 *   pt_svl:    Handle to the SVL object.
 *              
 *
 * Outputs:
 *   pui2_next_id: The smallest record id value.
 *
 * Returns: 
 *   TRUE      Smallest record id is returned
 *   FALSE     A Null record id is returned.
 ----------------------------------------------------------------------------*/
BOOL svl_get_smallest_rec_id(const SVL_OBJ_T* pt_svl, UINT16* pui2_next_id)
{
    UINT16  aui2_min_max[2];
    
    /*
      Get the min and max record id used by the database.
    */
    db_read_common(pt_svl->h_cdb, aui2_min_max,
                   (UINT16)sizeof(aui2_min_max));
    
    if ( aui2_min_max[0] != SVL_SMALLEST_INIT_VAL )
    {
        *pui2_next_id=aui2_min_max[0];
        return TRUE;
    }
    else
    {
        *pui2_next_id=SVL_NULL_REC_ID;
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: svl_set_smallest_rec_id
 *
 * Description: This function sets the smallest rec id value.
 *
 * Inputs:
 *   pt_svl:  Handle to the SVL object.
 *   pui2_next_id:    containing the new smallest record id value.
 *           
 *
 * Outputs: None
 *
 * Returns: 
 *   TRUE      SVL ID/Name is not currently in the database..
 *   FALSE     Invalid SVL ID/Name.
 ----------------------------------------------------------------------------*/
BOOL svl_set_smallest_rec_id(SVL_OBJ_T* pt_svl, UINT16  ui2_next_id)
{
    UINT16  aui2_min_max[2];
    
    /*
      Get the min and max record id used by the database.
    */
    db_read_common(pt_svl->h_cdb, aui2_min_max,
                   (UINT16)sizeof(aui2_min_max));

    /*
      Set the minimum record id value.
    */
    aui2_min_max[0] = ui2_next_id;

    db_write_common(pt_svl->h_cdb, aui2_min_max,
                    (UINT16)sizeof(aui2_min_max));
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: svl_get_largest_rec_id
 *
 * Description: This function returns the largest record id value. 
 *
 * Inputs: 
 *    t_id:    structure containing the smallest and largest record id
 *              currently in used.
 * Outputs: 
 *
 * Returns: 
 *   TRUE
 *
 ----------------------------------------------------------------------------*/
BOOL svl_get_largest_rec_id(const SVL_OBJ_T* pt_svl, UINT16* pui2_next_id)
{
    UINT16  aui2_min_max[2];
    
    /*
      Get the min and max record id used by the database.
    */
    db_read_common(pt_svl->h_cdb, aui2_min_max,
                   (UINT16)sizeof(aui2_min_max));
    
    if ( aui2_min_max[1] != SVL_NULL_REC_ID )
    {
        *pui2_next_id=aui2_min_max[1];
        return TRUE;
    }
    else
    {
        *pui2_next_id=SVL_NULL_REC_ID;
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: svl_set_largest_rec_id
 *
 * Description: This function sets the largest record id value. 
 *
 * Inputs: 
 *   pui2_next_id:    structure containing the new largest record id
 *       
 * Outputs: 
 *
 * Returns: 
 *   SVLR_OK       SVL entry is deleted.
 *
 ----------------------------------------------------------------------------*/
BOOL svl_set_largest_rec_id(SVL_OBJ_T* pt_svl, UINT16  ui2_next_id)
{
    UINT16  aui2_min_max[2];
    
    /*
      Get the min and max record id used by the database.
    */
    db_read_common(pt_svl->h_cdb, aui2_min_max,
                   (UINT16)sizeof(aui2_min_max));
    /*
      Set the max record id
    */
    aui2_min_max[1] = ui2_next_id;

    db_write_common(pt_svl->h_cdb, aui2_min_max,
                    (UINT16)sizeof(aui2_min_max));
    
    return TRUE;
}
