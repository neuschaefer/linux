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
 * $RCSfile: nwl_dbg.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains functions and internal variables to support
 *         CLI control of the NWL library.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/def_dbg_level_mw.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "os/inc/x_os.h"


#include "nwl/_nwl.h"
#include "nwl/nwl_dbg.h"

/*
  internal variable for the debug level.
*/
static UINT16  ui2_dbg_level = DBG_INIT_LEVEL_MW_NWL;

/*----------------------------------------------
  Public Function for DBG support.
------------------------------------------------*/
UINT16 nwl_get_dbg_level(VOID)
{
    return ui2_dbg_level;
}

VOID   nwl_set_dbg_level(UINT16  ui2_dbg)
{
    ui2_dbg_level = ui2_dbg;
}

/*-------------------------------------------------------------------------
 * Name: nwl_dump_rec
 *
 * Description: This API prints out NWL record entries for the
 *    specified NWL ID.
 *
 * Inputs:  
 *    ui2_nwl_id:     NWL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   nwl_dump_rec(UINT16    ui2_nwl_id)
{
    HANDLE_T   h_nwl;
    UINT32     ui4_ver_id ;
    UINT16     i2_num_rec;
    UINT16     ui2_idx;
    INT32      i4_rc;
    NWL_REC_T  t_nwl_rec;


    if ( x_nwl_open(ui2_nwl_id, NULL, NULL, &h_nwl) != NWLR_OK )
    {
        x_dbg_stmt("Error: NWL ID %d is not valid.\n", ui2_nwl_id);
        return;
    }
    
    ui4_ver_id = NWL_NULL_VER_ID;
    /*
      Get the total number of record.
    */
    i2_num_rec = 0;

    x_nwl_get_num_rec(h_nwl, &i2_num_rec, &ui4_ver_id);
    
    x_dbg_stmt("Total number of NWL record in [network list %d] are: %d\n",
               ui2_nwl_id, i2_num_rec);
    
    ui2_idx = 0;

    i4_rc = x_nwl_read_lock(h_nwl);
    
    i4_rc=nwl_sort_rec(h_nwl, &ui4_ver_id);

    if ( i4_rc == NWLR_OK )
    {
        i4_rc=nwl_set_cursor(h_nwl,NWL_FIRST_REC);

        while (i4_rc == NWLR_OK)
        {
            x_memset(&t_nwl_rec, 0 , sizeof(t_nwl_rec));
            i4_rc = nwl_read_cursor(h_nwl, &t_nwl_rec, &ui4_ver_id);
            if ( i4_rc == NWLR_OK )
            {
                ui2_idx++;
                /* print out record. */
                nwl_print_rec(&t_nwl_rec, ui2_idx);
            }
            else
            {
                x_dbg_stmt("Read error: %d\n", i4_rc);
                break;
            }

            /* goto next record. */
            i4_rc = nwl_set_cursor(h_nwl, NWL_NEXT_REC);
            if ( i4_rc == NWLR_END_OF_ITERATION )
            {
                /* done. */
                break;
            }
        }
    }

    x_nwl_read_unlock(h_nwl);
    x_nwl_close(h_nwl);
       
    return;
}


/*-------------------------------------------------------------------------
 * Name: nwl_print_info
 *
 * Description: This API prints information for the NWL object.
 *
 * Inputs:  
 *    ui2_nwl_id:     NWL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID nwl_print_info(UINT16    ui2_nwl_id)
{
    HANDLE_T   h_nwl;

    if ( x_nwl_open(ui2_nwl_id, NULL, NULL, &h_nwl) != NWLR_OK )
    {
        x_dbg_stmt("Error: NWL ID %d is not valid.\n", ui2_nwl_id);
        return;
    }

    x_nwl_get_info(h_nwl);
    
    x_nwl_close(h_nwl);
    
    return;
}


/*-------------------------------------------------------------------------
 * Name: nwl_print_rec
 *
 * Description: This API prints out NWL record entries for the
 *    specified NWL ID.
 *
 * Inputs:  
 *    ui2_nwl_id:     NWL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   nwl_print_rec(NWL_REC_T*  pt_nwl_rec, UINT16  ui2_idx)
{
    x_dbg_stmt("%d => "
               "nwl_rec id: [%d] "
               "satl id: [%d] "
               "satl_rec id: [%d] "
               "network id: [%d] "
               "nit ver id: [%d] "
               "network mask: [%x] "
               "nw_name:[%s] \n",
               ui2_idx,
               pt_nwl_rec->ui2_nwl_rec_id,
               pt_nwl_rec->ui2_satl_id,
               pt_nwl_rec->ui2_satl_rec_id,
               pt_nwl_rec->ui2_nw_id,
               pt_nwl_rec->ui1_nit_ver,
               pt_nwl_rec->ui4_nw_mask,
               pt_nwl_rec->s_nw_name);

    return;
}

/*-------------------------------------------------------------------------
 * Name: nwl_create_test_data
 *
 * Description: This API creates a NWL objects and writ-out NWL record to
 *              this object.
 *
 * Inputs:  
 *    ui2_nwl_id:     NWL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   nwl_create_test_data(UINT16           ui2_nwl_id )
{
    INT32            i4_i;
    INT32            i4_rc;
    NWL_REC_T        t_nwl_rec;
    HANDLE_T         h_nwl;
    CHAR*            s_name = "NWL_TEST";
    
    i4_rc = x_nwl_create ( s_name,
                           ui2_nwl_id,
                           NWL_MEM_DYNAMIC,  /* e_policy */
                           20,              /* ui2_max_rec */
                           NULL,
                           NULL,            /* nwl_nfy_fct_wr, */
                           &h_nwl );

    if ( i4_rc == NWLR_OK )
    {
        x_dbg_stmt("<nwl> NWL object created: [%d %s]\n", ui2_nwl_id, s_name);
    }
    else
    {
        x_dbg_stmt("<nwl> can not create NWL object.\n");
        return;
    }

    x_nwl_lock(h_nwl);
    
    for ( i4_i=1;  i4_i < 10; i4_i++ )
    {
        x_nwl_rec_init(&t_nwl_rec);

        t_nwl_rec.ui4_nw_mask = 0xFFFFABCD;
        t_nwl_rec.ui2_nwl_rec_id = (UINT16) i4_i;
        t_nwl_rec.ui2_satl_id = ui2_nwl_id;
        t_nwl_rec.ui2_satl_rec_id = (UINT16) (i4_i / 2);
        t_nwl_rec.ui2_nw_id = (UINT16) (i4_i + 10 + i4_i) ;
        t_nwl_rec.ui1_nit_ver = (UINT8) (i4_i + 100 + i4_i);
        x_strcpy(t_nwl_rec.s_nw_name,"test_nw");

        i4_rc = x_nwl_add_rec(h_nwl, &t_nwl_rec);
        if ( i4_rc == NWLR_OK )
        {
            x_dbg_stmt("nwl rec added: %x %d %d %d %d %d %s \n",
                       t_nwl_rec.ui4_nw_mask,
                       t_nwl_rec.ui2_nwl_rec_id,
                       t_nwl_rec.ui2_satl_id,
                       t_nwl_rec.ui2_satl_rec_id,
                       t_nwl_rec.ui2_nw_id,
                       t_nwl_rec.ui1_nit_ver,
                       t_nwl_rec.s_nw_name);
        }
        else
        {
            x_dbg_stmt("Add NWL record failed.\n");
        }
    }

    x_nwl_unlock(h_nwl);

    x_nwl_close(h_nwl);
    
    return;
}


#if 0
/*-------------------------------------------------------------------------
 * Name: e_brdcst_type_name
 *
 * Description: This API converts BRDCST_TYPE_T enum to a string name. 
 *
 * Inputs:  
 *    e_brdcst_type   specifies BRDCST_TYPE_T enum
 *
 * Outputs: -
 *
 * Returns: brdcst type name.
 --------------------------------------------------------------------------*/
static CHAR*  e_brdcst_type_name(BRDCST_TYPE_T   e_brdcst_type)
{
    CHAR*    s_name;
    
    switch (e_brdcst_type)
    {
        case BRDCST_TYPE_UNKNOWN:
        {
            s_name="UNKNOWN";
        }
        break;

        case BRDCST_TYPE_ANALOG:
        {
            s_name="ANALOG";
        }
        break;

        case BRDCST_TYPE_DVB:
        {
            s_name="DVB";
        }
        break;

        case BRDCST_TYPE_ATSC:
        {
            s_name="ATSC";
        }
        break;

        case BRDCST_TYPE_SCTE:
        {
            s_name="SCTE";
        }
        break;

        default:
        {
            s_name=NULL;
        }
    }
    return s_name;
}


/*-------------------------------------------------------------------------
 * Name: e_brdcst_medium_name
 *
 * Description: This API converts BRDCST_MEDIUM_T enum to a string name. 
 *
 * Inputs:  
 *    e_brdcst_medium   specifies BRDCST_MEDIUM_T enum
 *
 * Outputs: -
 *
 * Returns: brdcst medium name.
 --------------------------------------------------------------------------*/
static CHAR*  e_brdcst_medium_name(BRDCST_MEDIUM_T   e_brdcst_medium)
{
    CHAR*    s_name;
    
    switch (e_brdcst_medium)
    {
        case BRDCST_MEDIUM_UNKNOWN:
        {
            s_name="UNKNOWN";
        }
        break;

        case BRDCST_MEDIUM_DIG_TERRESTRIAL:
        {
            s_name="DIG_TERRESTRIAL";
        }
        break;

        case BRDCST_MEDIUM_DIG_CABLE:
        {
            s_name="DIG_CABLE";
        }
        break;

        case BRDCST_MEDIUM_DIG_SATELLITE:
        {
            s_name="DIG_SATELLITE";
        }
        break;

        case BRDCST_MEDIUM_ANA_TERRESTRIAL:
        {
            s_name="ANA_TERRESTRIAL";
        }
        break;

        case BRDCST_MEDIUM_ANA_CABLE:
        {
            s_name="ANA_CABLE";
        }
        break;

        case BRDCST_MEDIUM_ANA_SATELLITE:
        {
            s_name="ANA_SATELLITE";
        }
        break;

        case BRDCST_MEDIUM_1394:
        {
            s_name="1934";
        }
        break;
        
        default:
        {
            s_name=NULL;
        }
        break;
    }
    return s_name;
}


/*-------------------------------------------------------------------------
 * Name: nwl_print_rec
 *
 * Description: This API prints out NWL record entries for the
 *    specified NWL ID.
 *
 * Inputs:  
 *    ui2_nwl_id:     NWL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   nwl_print_rec(NWL_REC_T*  pt_nwl_rec, UINT16  ui2_idx)
{
    x_dbg_stmt("%d => "
               "nwl_rec id: [%d] "
               "brdcst type: [%s] "
               "brdcst medium: [%s] "
               "nw id: [%d] "
               "orig nw id: [%d] "
               "ts: [%d] "
               "nw_name:[%s] \n",
               ui2_idx,
               pt_nwl_rec->ui2_nwl_rec_id,
               e_brdcst_type_name(pt_nwl_rec->uheader.t_desc.e_bcst_type),
               e_brdcst_medium_name(pt_nwl_rec->uheader.t_desc.e_bcst_medium),
               pt_nwl_rec->uheader.t_desc.ui2_nw_id,
               pt_nwl_rec->uheader.t_desc.ui2_on_id,
               pt_nwl_rec->uheader.t_desc.ui2_ts_id,
               pt_nwl_rec->s_nw_name);
            
    switch(pt_nwl_rec->uheader.t_desc.e_bcst_type)
    {
        case BRDCST_TYPE_ANALOG :
        {

        }
        break;

        case BRDCST_TYPE_DVB :
        {
            if ( pt_nwl_rec->uheader.t_desc.e_bcst_medium ==
                 BRDCST_MEDIUM_DIG_TERRESTRIAL )
            {
                TUNER_TER_DIG_TUNE_INFO_T*   pt_ter_dig;
                pt_ter_dig=&(pt_nwl_rec->udata.t_ter_dig);
                x_dbg_stmt("<DVB terr> freq: [%d]\n",pt_ter_dig->ui4_freq);
            }
        }
        break;

        case BRDCST_TYPE_ATSC :
        {
            if ( pt_nwl_rec->uheader.t_desc.e_bcst_medium ==
                 BRDCST_MEDIUM_DIG_TERRESTRIAL )
            {
                TUNER_TER_DIG_TUNE_INFO_T*   pt_ter_dig;
                pt_ter_dig=&(pt_nwl_rec->udata.t_ter_dig);
                x_dbg_stmt("<ATSC terr> freq: [%d]\n",pt_ter_dig->ui4_freq);
            }
        }
        break;

        case BRDCST_TYPE_SCTE :
        {
        }
        break;

        case BRDCST_TYPE_UNKNOWN :
        {
            
        }
        break;
    }

    return;
}


/*-------------------------------------------------------------------------
 * Name: nwl_print_all_rec
 *
 * Description: This API prints out NWL record entries for the
 *    specified nwl ID.
 *
 * Inputs:  
 *    ui2_NWL_id:     NWL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   nwl_print_all_rec(UINT16    ui2_nwl_id)
{
    HANDLE_T   h_nwl;
    UINT32     ui4_ver_id ;
    UINT32     i4_num_rec = 0;
    INT32      i4_rc;
    NWL_REC_T  t_nwl_rec;

    if ( x_nwl_open(ui2_nwl_id, NULL, NULL, &h_nwl) != NWLR_OK )
    {
        x_dbg_stmt("Error: NWL ID %d is not valid.\n", ui2_nwl_id);
        return;
    }
    
    ui4_ver_id = NWL_NULL_VER_ID;

    i4_rc = x_nwl_read_lock(h_nwl);
    
    i4_rc=x_nwl_sort_rec(h_nwl, &ui4_ver_id);

    if ( i4_rc == NWLR_OK )
    {
        i4_rc=x_nwl_set_cursor(h_nwl,NWL_FIRST_REC);

        while (i4_rc == NWLR_OK)
        {
            i4_rc = x_nwl_read_cursor(h_nwl, &t_nwl_rec, &ui4_ver_id);
            if ( i4_rc == NWLR_OK )
            {
                i4_num_rec++;
                /* print out record. */
                nwl_print_rec(&t_nwl_rec,i4_num_rec);
            }
            else
            {
                x_dbg_stmt("Read error: %d\n", i4_rc);
                break;
            }

            /* goto next record. */
            i4_rc = x_nwl_set_cursor(h_nwl, NWL_NEXT_REC);
            if ( i4_rc == NWLR_END_OF_ITERATION )
            {
                /* done. */
                break;
            }
        }
    }

    x_nwl_read_unlock(h_nwl);
    x_nwl_close(h_nwl);
 
    x_dbg_stmt("Total number of NWL recrods in handle %d are: %d\n",
               h_nwl, i4_num_rec);
    
    return;
}

#endif
