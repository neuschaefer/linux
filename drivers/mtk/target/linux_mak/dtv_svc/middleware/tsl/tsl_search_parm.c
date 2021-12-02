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
 * $RCSfile: svl_search_parm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements search function for a specified TSL record.
 *   
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "tsl/_tsl.h"
#include "tsl/tsl_search_parm.h"
#include "tsl/tsl_rec.h"

/**********************************************************************
   Following are private functions only known to the TSL library.
   The private functions are comparing search parameters.

***********************************************************************/
/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: tsl_cmp_search_parm
 *
 * Description: compare 2 search parm structure.
 *
 * Inputs: 
 *    t_a:      pointer to the first search parameter structure.
 *    t_b:      pointer to the second search parameter structure.
 *
 * Outputs: None
 *
 * Returns: 
 *   TRUE        the search parameter matched.
 *   FALSE       search parameter does not matched.
 *
 ----------------------------------------------------------------------------*/
BOOL tsl_cmp_search_parm(TSL_SEARCH_PARM_T* t_a, TSL_SEARCH_PARM_T* t_b )
{
    BOOL   b_rc;
    /*
      compare field type of the search parameter structure.
    */
    if ( t_a->ui4_fld_type != t_b->ui4_fld_type )
    {
        return FALSE;
    }

    /*
      Compare each sub-fields of the search parameter.
       
      For TSL record, currently we only one type of search parameter,
      which is to compare the nwl_id and nwl_record_id values
      in the header.
    
    */
    switch (t_a->ui4_fld_type)
    {
        case TSL_CDB_TYPE_HEADER:
        {
            if ( (t_a->u.t_nw.ui2_nwl_id == t_b->u.t_nw.ui2_nwl_id)  &&
                 (t_a->u.t_nw.ui2_nwl_rec_id == t_b->u.t_nw.ui2_nwl_rec_id)
                )
            {
                b_rc = TRUE;
            }
            else
            {
                b_rc = FALSE;
            }
        }
        break;

        case TSL_CDB_TYPE_REC_REFERENCE_DATA:
        {
            if ( (t_a->u.t_sat.ui4_mask        == t_b->u.t_sat.ui4_mask)     &&
                 (t_a->u.t_sat.ui2_satl_id     == t_b->u.t_sat.ui2_satl_id)  &&
                 (t_a->u.t_sat.ui2_satl_rec_id == t_b->u.t_sat.ui2_satl_rec_id)
                )
            {
                b_rc = TRUE;
            }
            else
            {
                b_rc = FALSE;
            }
        }
        break;

        case TSL_CDB_TYPE_UNKNOWN:
        {
            b_rc = TRUE;
        }
        break;

        default:
        {
            b_rc = FALSE;
        }
        break;
    }
    return b_rc;
}


/*-----------------------------------------------------------------------------
 * Name: tsl_save_search_parm_and_cursor
 *
 * Description: Save the search parameters (nwl_id, nwl_rec_id, etc),
 *    the index value, and the corresponding current record position from
 *    in the internal sorted CDB address array to the internal search
 *    parameter field and cursor field in TSL control block structure.
 *
 * Inputs: 
 *    pt_id:    
 *
 * Outputs: None
 *
 * Returns: 
 *   TRUE      The search parameters and cursor key are saved.
 *   FALSE     Error in saving search parameters and cursor key. 
 *
 ----------------------------------------------------------------------------*/
BOOL tsl_save_search_parm_and_cursor
(
    TSL_OBJ_T* pt_tsl,
    TSL_SEARCH_PARM_T* t_source,
    UINT16 ui2_idx
)
{
    TSL_SEARCH_PARM_T* t_dest;
    DB_RECORD_POS_T    t_pos;
    BOOL               b_rc;
    
    /* Save the current cursor location of the search parameter. */
    pt_tsl->t_cursor.ui2_rec_idx=ui2_idx;
    db_get_cursor(pt_tsl->h_cdb,&t_pos);
    pt_tsl->t_cursor.t_pos=t_pos;


    t_dest=&(pt_tsl->t_parm);
    /*
      copy search parameter field type
    */
    t_dest->ui4_fld_type = t_source->ui4_fld_type ;
    
    /*
      copy each sub-fields of the search parameter.
    */
    switch (t_dest->ui4_fld_type)
    {
        case TSL_CDB_TYPE_HEADER:
        {
            t_dest->u.t_nw.ui2_nwl_id = t_source->u.t_nw.ui2_nwl_id ;
            t_dest->u.t_nw.ui2_nwl_rec_id = t_source->u.t_nw.ui2_nwl_rec_id ;
            b_rc = TRUE;
        }
        break;
        
        case TSL_CDB_TYPE_REC_REFERENCE_DATA:
        {
            t_dest->u.t_sat.ui4_mask        = t_source->u.t_sat.ui4_mask ;
            t_dest->u.t_sat.ui2_satl_id     = t_source->u.t_sat.ui2_satl_id ;
            t_dest->u.t_sat.ui2_satl_rec_id = t_source->u.t_sat.ui2_satl_rec_id ;
            b_rc = TRUE;
        }
        break;
        
        case TSL_CDB_TYPE_UNKNOWN:
        {
            b_rc = TRUE;
        }
        break;

        default:
        {
            b_rc = FALSE;
        }
        break;
    }
    return b_rc;
}

/*-----------------------------------------------------------------------------
 * Name: tsl_cmp_rec_search_parm
 *
 * Description: Compare a TSL record with search parameter
 *
 * Inputs: 
 *    t_a:      pointer to a search param structure.    
 *    t_b:      pointer to a TSL_REC_T structure.
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           
 *   TSLR_INV_ARG      'ui2_max_num_tsl_obj' is 0.
 *
 ----------------------------------------------------------------------------*/
BOOL tsl_cmp_rec_search_parm(TSL_SEARCH_PARM_T* t_a, TSL_REC_T* t_b )
{
    /*
      compare TSL record fields with the specified search parameters.
    */
    switch (t_a->ui4_fld_type)
    {
        case TSL_CDB_TYPE_HEADER:
        {
            UINT16    ui2_nwl_id;
            UINT16    ui2_nwl_rec_id;

#ifndef MW_DVBS_DEV_ENABLE
            ui2_nwl_id = TS_DESCR_GET_NWL_ID(t_b->uheader.t_desc);
            ui2_nwl_rec_id = TS_DESCR_GET_NWL_REC_ID(t_b->uheader.t_desc);
#else
            ui2_nwl_id      = t_b->uref.t_ref.ui2_nwl_id;
            ui2_nwl_rec_id  = t_b->uref.t_ref.ui2_nwl_rec_id;
#endif
            
            if ( (t_a->u.t_nw.ui2_nwl_id == ui2_nwl_id)          &&
                 (t_a->u.t_nw.ui2_nwl_rec_id == ui2_nwl_rec_id)
                )
            {
                return TRUE;
            }
        }
        break;

        case TSL_CDB_TYPE_REC_REFERENCE_DATA:
        {
            if ( (t_a->u.t_sat.ui4_mask == 0 ||
                  ((t_a->u.t_sat.ui4_mask & t_b->uref.t_ref.ui4_mask) > 0) ||
                  ((t_a->u.t_sat.ui4_mask == (UINT32) 0xFFFFFFFF) &&
                   (t_b->uref.t_ref.ui4_mask == 0))
                 ) &&
                 (t_a->u.t_sat.ui2_satl_id     == t_b->uref.t_ref.ui2_satl_id)  &&
                 (t_a->u.t_sat.ui2_satl_rec_id == t_b->uref.t_ref.ui2_satl_rec_id)
                )
            {
                return TRUE;
            }
        }
        break;

        case TSL_CDB_TYPE_UNKNOWN:
        {
            return TRUE;
        }

        default:
        {
            break;
        }
    }
    return FALSE;
}





