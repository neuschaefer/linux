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
 *    This file implements service record ID query/next functions.
 *   
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "svl/_svl.h"
#include "svl/svl_search_parm.h"
#include "svl/svl_rec.h"

/**********************************************************************
   Following are private functions only known to the SVL library.
   The private functions are comparing search parameters.

***********************************************************************/
/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: svl_cmp_search_parm
 *
 * Description: 
 *
 * Inputs: 
 *    t_a:    
 *    t_b:
 *
 * Outputs: None
 *
 * Returns: 
 *   TRUE        the search parameter matched.
 *   FALSE       search parameter does not matched.
 *
 ----------------------------------------------------------------------------*/
BOOL svl_cmp_search_parm(SVL_SEARCH_PARM_T* t_a, SVL_SEARCH_PARM_T* t_b )
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
      compare each sub-fields of the search parameter.
    */
    switch (t_a->ui4_fld_type)
    {
        case SVL_CDB_TYPE_CHANNEL_ID:
        {
            if ( t_a->u.t_chan.ui4_channel_num == t_b->u.t_chan.ui4_channel_num  &&
                 t_a->u.t_chan.ui4_nw_mask     == t_b->u.t_chan.ui4_nw_mask
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
        

        case SVL_CDB_TYPE_BRDCST_T:
        {
            if ( t_a->u.t_brdcst.e_brdcst_type   == t_b->u.t_brdcst.e_brdcst_type  &&
                 t_a->u.t_brdcst.ui4_nw_mask     == t_b->u.t_brdcst.ui4_nw_mask     
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

        case SVL_CDB_TYPE_TSL_ID:
        {
            if ( t_a->u.t_ts.ui2_tsl_id     == t_b->u.t_ts.ui2_tsl_id      &&
                 t_a->u.t_ts.ui2_tsl_rec_id == t_b->u.t_ts.ui2_tsl_rec_id  &&
                 t_a->u.t_ts.ui4_nw_mask    == t_b->u.t_ts.ui4_nw_mask
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
        
        case SVL_CDB_TYPE_NWL_ID:
        {
            if ( t_a->u.t_nw.ui2_nwl_id     == t_b->u.t_nw.ui2_nwl_id      &&
                 t_a->u.t_nw.ui2_nwl_rec_id == t_b->u.t_nw.ui2_nwl_rec_id  &&
                 t_a->u.t_nw.ui4_nw_mask    == t_b->u.t_nw.ui4_nw_mask
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
        
        case SVL_CDB_TYPE_SATL_ID:
        {
            if ( t_a->u.t_sat.ui2_satl_id     == t_b->u.t_sat.ui2_satl_id     &&
                 t_a->u.t_sat.ui2_satl_rec_id == t_b->u.t_sat.ui2_satl_rec_id &&
                 t_a->u.t_sat.ui4_nw_mask     == t_b->u.t_sat.ui4_nw_mask
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
        
        default:
        {
            b_rc = FALSE;
        }
        break;
    }
    return b_rc;
}


/*-----------------------------------------------------------------------------
 * Name: svl_save_search_parm_and_cursor
 *
 * Description: Save the search parameters (keyfield, netmask value),
 *    the index value, and the corresponding current record position from
 *    in the internal sorted CDB address array.
 *
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
BOOL svl_save_search_parm_and_cursor
(
    SVL_OBJ_T* pt_svl,
    SVL_SEARCH_PARM_T* t_source,
    UINT16 ui2_idx
)
{
    SVL_SEARCH_PARM_T* t_dest;
    DB_RECORD_POS_T    t_pos;
    BOOL               b_rc;
    
    /* Save the current cursor location of the search parameter. */
    pt_svl->t_cursor.ui2_rec_idx=ui2_idx;
    db_get_cursor(pt_svl->h_cdb,&t_pos);
    pt_svl->t_cursor.t_pos=t_pos;


    t_dest=&(pt_svl->t_parm);
    /*
      copy search parameter field type
    */
    t_dest->ui4_fld_type = t_source->ui4_fld_type ;
    
    /*
      copy each sub-fields of the search parameter.
    */
    switch (t_dest->ui4_fld_type)
    {
        case SVL_CDB_TYPE_CHANNEL_ID:
        {
            t_dest->u.t_chan.ui4_channel_num = t_source->u.t_chan.ui4_channel_num ;
            t_dest->u.t_chan.ui4_nw_mask     = t_source->u.t_chan.ui4_nw_mask ;
            b_rc = TRUE;
        }
        break;

        case SVL_CDB_TYPE_BRDCST_T:
        {
            t_dest->u.t_brdcst.e_brdcst_type = t_source->u.t_brdcst.e_brdcst_type;
            t_dest->u.t_brdcst.ui4_nw_mask   = t_source->u.t_brdcst.ui4_nw_mask;
            b_rc = TRUE;
        }
        break;

        case SVL_CDB_TYPE_TSL_ID:
        {
            t_dest->u.t_ts.ui2_tsl_id     = t_source->u.t_ts.ui2_tsl_id;
            t_dest->u.t_ts.ui2_tsl_rec_id = t_source->u.t_ts.ui2_tsl_rec_id;
            t_dest->u.t_ts.ui4_nw_mask    = t_source->u.t_ts.ui4_nw_mask;
            b_rc = TRUE;
        }
        break;
        
        case SVL_CDB_TYPE_NWL_ID:
        {
            t_dest->u.t_nw.ui2_nwl_id     = t_source->u.t_nw.ui2_nwl_id;
            t_dest->u.t_nw.ui2_nwl_rec_id = t_source->u.t_nw.ui2_nwl_rec_id;
            t_dest->u.t_nw.ui4_nw_mask    = t_source->u.t_nw.ui4_nw_mask;
            b_rc = TRUE;
        }
        break;
        
        case SVL_CDB_TYPE_SATL_ID:
        {
            t_dest->u.t_sat.ui2_satl_id     = t_source->u.t_sat.ui2_satl_id;
            t_dest->u.t_sat.ui2_satl_rec_id = t_source->u.t_sat.ui2_satl_rec_id;
            t_dest->u.t_sat.ui4_nw_mask     = t_source->u.t_sat.ui4_nw_mask;
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
 * Name: svl_cmp_rec_search_parm
 *
 * Description: Compare a SVL record with search parameter
 *
 * Inputs: 
 *    pt_id:    
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           
 *   SVLR_INV_ARG      'ui2_max_num_svl_obj' is 0.
 *
 ----------------------------------------------------------------------------*/
BOOL svl_cmp_rec_search_parm(SVL_SEARCH_PARM_T* t_a, SVL_REC_T* t_b )
{
    /*
      compare SVL record fields with the specified search parameters.
    */
    switch (t_a->ui4_fld_type)
    {
        case SVL_CDB_TYPE_CHANNEL_ID:
        {
            if ( t_a->u.t_chan.ui4_channel_num == t_b->uheader.t_rec_hdr.ui4_channel_id  &&
                 ((t_a->u.t_chan.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0 )
                )
            {
                return TRUE;
            }
        }
        break;

        case SVL_CDB_TYPE_BRDCST_T:
        {
            /*
               Special case, if e_brdcst_type is BRDCST_TYPE_UNKNOWN, then match
               all broadcast type.
            */
            if ( t_a->u.t_brdcst.e_brdcst_type == BRDCST_TYPE_UNKNOWN )
            {
                if ( (t_a->u.t_brdcst.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0 )
                {
                    return TRUE;
                }
            }
            else
            {
                if ( t_a->u.t_brdcst.e_brdcst_type == t_b->uheader.t_rec_hdr.e_brdcst_type &&
                     ( (t_a->u.t_brdcst.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0 )
                   )
                {
                    return TRUE;
                }
            }
        }
        break;

        case SVL_CDB_TYPE_TSL_ID:
        {
            if ( t_a->u.t_ts.ui2_tsl_id     == t_b->uheader.t_rec_hdr.ui2_tsl_id      &&
                 t_a->u.t_ts.ui2_tsl_rec_id == t_b->uheader.t_rec_hdr.ui2_tsl_rec_id  &&
                 ( (t_a->u.t_ts.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0 ) 
                )
            {
                return TRUE;
            }
        }
        break;
        
        case SVL_CDB_TYPE_NWL_ID:
        {
            if ( t_a->u.t_nw.ui2_nwl_id     == t_b->uheader.t_rec_hdr.ui2_nwl_id      &&
                 t_a->u.t_nw.ui2_nwl_rec_id == t_b->uheader.t_rec_hdr.ui2_nwl_rec_id  &&
                 ( (t_a->u.t_nw.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0 ) 
                )
            {
                return TRUE;
            }
        }
        break;
        
        case SVL_CDB_TYPE_SATL_ID:
        {
            if ( t_a->u.t_sat.ui2_satl_id     == t_b->uheader.t_rec_hdr.ui2_satl_id      &&
                 t_a->u.t_sat.ui2_satl_rec_id == t_b->uheader.t_rec_hdr.ui2_satl_rec_id  &&
                 ( (t_a->u.t_sat.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0 ) 
                )
            {
                return TRUE;
            }
        }
        break;
        
        default:
            break;
    }
    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name: svl_cmp_rec_nwmask_parm
 *
 * Description: Compare a SVL record with search parameter
 *
 * Inputs: 
 *    pt_id:    
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           
 *   SVLR_INV_ARG      'ui2_max_num_svl_obj' is 0.
 *
 ----------------------------------------------------------------------------*/
BOOL svl_cmp_rec_nwmask_parm(SVL_SEARCH_PARM_T* t_a, SVL_REC_T* t_b )
{
    /*
      compare SVL record fields with the specified search parameters.
    */
    switch (t_a->ui4_fld_type)
    {
        case SVL_CDB_TYPE_CHANNEL_ID:
        {
            if (
                 (t_a->u.t_chan.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0 
                )
            {
                return TRUE;
            }
        }
        break;

        case SVL_CDB_TYPE_BRDCST_T:
        {
            if ( 
                     (t_a->u.t_brdcst.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0
                )
            {
                return TRUE;
            }
        }
        break;

        case SVL_CDB_TYPE_TSL_ID:
        {
            if ( 
                 (t_a->u.t_ts.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0 
                )
            {
                return TRUE;
            }
        }
        break;

        case SVL_CDB_TYPE_NWL_ID:
        {
            if ( 
                 (t_a->u.t_nw.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0 
                )
            {
                return TRUE;
            }
        }
        break;

        case SVL_CDB_TYPE_SATL_ID:
        {
            if ( 
                 (t_a->u.t_sat.ui4_nw_mask & t_b->uheader.t_rec_hdr.ui4_nw_mask) > 0 
                )
            {
                return TRUE;
            }
        }
        break;

        default:
            break;
    }
    return FALSE;
}


