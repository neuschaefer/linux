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
 * $RCSfile: satl_dbg.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains functions and internal variables to support
 *         CLI control of the DT library.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/def_dbg_level_mw.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "os/inc/x_os.h"

#include "satl/x_satl.h"
#include "svl/x_svl.h"
/*
  internal variable for the debug level.
*/
static UINT16  ui2_dbg_level = DBG_INIT_LEVEL_MW_SATL;

/*----------------------------------------------
  Public Function for DBG support.
------------------------------------------------*/
UINT16 satl_get_dbg_level(VOID)
{
    return ui2_dbg_level;
}

VOID   satl_set_dbg_level(UINT16 ui2_dbg)
{
    ui2_dbg_level = ui2_dbg;
}

/*-------------------------------------------------------------------------
 * Name: satl_print_info
 *
 * Description: This API prints information for the SATL object.
 *
 * Inputs:  
 *    ui2_satl_id:    SATL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID satl_print_info(UINT16   ui2_satl_id)
{
    HANDLE_T   h_satl;

    if ( x_satl_open(ui2_satl_id, NULL, NULL, &h_satl) != SATLR_OK )
    {
        x_dbg_stmt("Error: SATL ID %d is not valid.\n", ui2_satl_id);
        return;
    }

    x_satl_get_info(h_satl);
    
    x_satl_close(h_satl);
    
    return;
}

/*-------------------------------------------------------------------------
 * Name: e_lnb_type_name
 *
 * Description: This API converts TUNER_LNB_TYPE_T enum to a string name. 
 *
 * Inputs:  
 *    e_lnb_type_name   specifies TUNER_LNB_TYPE_T enum
 *
 * Outputs: -
 *
 * Returns: brdcst type name.
 --------------------------------------------------------------------------*/
static CHAR*  e_lnb_type_name(TUNER_LNB_TYPE_T   e_lnb_type)
{
    CHAR*    s_name;
    
    switch (e_lnb_type)
    {
        case LNB_UNKNOWN:
        {
            s_name="UNKNOWN";
        }
        break;

        case LNB_SINGLE_FREQ:
        {
            s_name="SINGLE_FREQ";
        }
        break;

        case LNB_DUAL_FREQ:
        {
            s_name="DUAL_FREQ";
        }
        break;

        default:
        {
            s_name="<invalid>";
        }
    }
    return s_name;
}

/*-------------------------------------------------------------------------
 * Name: e_diseqc_type_name
 *
 * Description: This API converts TUNER_DISEQC_TYPE_T enum to a string name. 
 *
 * Inputs:  
 *    e_diseqc_type_name    specifies TUNER_DISEQC_TYPE_T enum
 *
 * Outputs: -
 *
 * Returns: brdcst type name.
 --------------------------------------------------------------------------*/
static CHAR*  e_diseqc_type_name(TUNER_DISEQC_TYPE_T   e_diseqc_type)
{
    CHAR*    s_name;
    
    switch (e_diseqc_type)
    {
        case DISEQC_NONE:
        {
            s_name="none";
        }
        break;

        case DISEQC_2X1:
        {
            s_name="2x1";
        }
        break;

        case DISEQC_4X1:
        {
            s_name="4x1";
        }
        break;

        case DISEQC_8X1:
        {
            s_name="8x1";
        }
        break;

        case DISEQC_16X1:
        {
            s_name="16x1";
        }
        break;

        case DISEQC_1DOT2:
        {
            s_name="1.2";
        }
        break;

        case DISEQC_USALS:
        {
            s_name="usals";
        }
        break;

        default:
        {
            s_name="<invalid>";
        }
    }
    return s_name;
}

/*-------------------------------------------------------------------------
 * Name: e_polar_name
 *
 * Description: This API converts TUNER_POLARIZATION_T enum to a string name. 
 *
 * Inputs:  
 *    e_polar       specifies TUNER_POLARIZATION_T enum
 *
 * Outputs: -
 *
 * Returns: brdcst type name.
 --------------------------------------------------------------------------*/
static CHAR*  e_polar_name(TUNER_POLARIZATION_T   e_polar)
{
    CHAR*    s_name;
    
    switch (e_polar)
    {
        case POL_UNKNOWN:
        {
            s_name="UNKNOWN";
        }
        break;

        case POL_LIN_HORIZONTAL:
        {
            s_name="LIN_HOR";
        }
        break;

        case POL_LIN_VERTICAL:
        {
            s_name="LIN_VER";
        }
        break;

        case POL_CIR_LEFT:
        {
            s_name="CIR_LEFT";
        }
        break;

        case POL_CIR_RIGHT:
        {
            s_name="CIR_RIGHT";
        }
        break;

        default:
        {
            s_name="<invalid>";
        }
    }
    return s_name;
}

/*-------------------------------------------------------------------------
 * Name: satl_print_rec
 *
 * Description: This API prints out SATL record entries for the
 *    specified SATL ID.
 *
 * Inputs:  
 *    ui2_satl_id:    SATL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   satl_print_rec(const SATL_REC_T* pt_satl_rec, UINT16  ui2_idx)
{
    x_dbg_stmt("%d => "
               "satl_rec id: [%d] "
               "mask: [0x%x] "
               "LNB type: [%s] "
               "low freq: [%d Mhz] "
               "high freq: [%d Mhz] "
               "switch freq: [%d Mhz] "
               "DiSEqC type: [%s] "
               "port: [%d] "
               "casade type: [%d] "
               "casade port: [%d] "
               "22K: [%d] "
               "tone burst: [%d] "
               "polar: [%s] "
               "wxyz: [%d] "
               "Orbit position: [%d] "
               "satellite name: [%s] "
               "\n",
               ui2_idx,
               pt_satl_rec->ui2_satl_rec_id,
               pt_satl_rec->ui4_mask,
               e_lnb_type_name(pt_satl_rec->ulnb.t_lnb.e_lnb_type),
               pt_satl_rec->ulnb.t_lnb.ui2_lnb_low_freq,
               pt_satl_rec->ulnb.t_lnb.ui2_lnb_high_freq,
               pt_satl_rec->ulnb.t_lnb.ui2_lnb_switch_freq,
               e_diseqc_type_name(pt_satl_rec->udiseqc.t_diseqc_info.e_diseqc_type),
               pt_satl_rec->udiseqc.t_diseqc_info.ui1_port,
               pt_satl_rec->udiseqc.t_diseqc_info.ui1_cascade_type,
               pt_satl_rec->udiseqc.t_diseqc_info.ui1_cascade_port,
               pt_satl_rec->udiseqc.t_diseqc_info.ui1_22k,
               pt_satl_rec->udiseqc.t_diseqc_info.ui1_tone_burst,
               e_polar_name(pt_satl_rec->udiseqc.t_diseqc_info.e_polar),
               pt_satl_rec->udiseqc.t_diseqc_info.ui2_wxyz,
               pt_satl_rec->i2_orb_pos,
               pt_satl_rec->ac_sat_name);

    return;
}

/*-------------------------------------------------------------------------
 * Name: satl_print_all_rec
 *
 * Description: This API prints out SATL record entries for the
 *    specified satl ID.
 *
 * Inputs:  
 *    ui2_SATL_id:    SATL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   satl_print_all_rec(UINT16   ui2_satl_id)
{
    HANDLE_T   h_satl;
    UINT32     ui4_ver_id ;
    UINT32     i4_num_rec = 0;
    INT32      i4_rc;
    SATL_REC_T t_satl_rec = {0}; /* try to fix Klocwork issue */

    if ( x_satl_open(ui2_satl_id, NULL, NULL, &h_satl) != SATLR_OK )
    {
        x_dbg_stmt("Error: SATL ID %d is not valid.\n", ui2_satl_id);
        return;
    }
    
    ui4_ver_id = SATL_NULL_VER_ID;

    i4_rc = x_satl_read_lock(h_satl);
    
    i4_rc=x_satl_sort_rec(h_satl, &ui4_ver_id);

    if ( i4_rc == SATLR_OK )
    {
        i4_rc=x_satl_set_cursor(h_satl,SATL_FIRST_REC);

        while (i4_rc == SATLR_OK)
        {
            i4_rc = x_satl_read_cursor(h_satl, &t_satl_rec, &ui4_ver_id);
            if ( i4_rc == SATLR_OK )
            {
                i4_num_rec++;
                /* print out record. */
                satl_print_rec(&t_satl_rec,(UINT16)i4_num_rec);
            }
            else
            {
                x_dbg_stmt("Read error: %d\n", i4_rc);
                break;
            }

            /* goto next record. */
            i4_rc = x_satl_set_cursor(h_satl, SATL_NEXT_REC);
            if ( i4_rc == SATLR_END_OF_ITERATION )
            {
                /* done. */
                break;
            }
        }
    }

    x_satl_read_unlock(h_satl);
    x_satl_close(h_satl);
 
    x_dbg_stmt("Total number of SATL recrods in handle %d are: %d\n",
               h_satl, i4_num_rec);
    
    return;
}


    
/*-------------------------------------------------------------------------
 * Name: satl_modify_rec_value
 *
 * Description: This API deletes a SATL record entry for the
 *    specified satl ID.
 *
 * Inputs:  
 *    ui2_SATL_id:    SATL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   satl_delete_rec(UINT16   ui2_satl_id,
                       UINT16 ui2_satl_rec_id)
{
    HANDLE_T   h_satl;

    if ( x_satl_open(ui2_satl_id, NULL, NULL, &h_satl) != SATLR_OK )
    {
        x_dbg_stmt("Error: SATL ID %d is not valid.\n", ui2_satl_id);
        return;
    }

    x_dbg_stmt("deleted SATL record [%d] \n", ui2_satl_rec_id);

    x_satl_lock(h_satl);

    x_satl_del_rec(h_satl,
                   ui2_satl_rec_id);
    

    x_satl_unlock(h_satl);
    x_satl_close(h_satl);
 
    x_dbg_stmt("SATL record %d deleted. \n", ui2_satl_rec_id);
    
    return;
}

/*-------------------------------------------------------------------------
 * Name: satl_create_test_data
 *
 * Description: This API creates a SATL objects and write out SATL record to
 *              this object.
 *
 * Inputs:  
 *    ui2_satl_id:     SATL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   satl_create_test_data(UINT16           ui2_satl_id )
{
    INT32            i4_i;
    INT32            i4_rc;
    SATL_REC_T       t_satl_rec;
    HANDLE_T         h_satl;
    CHAR*            s_name = "SATL_TEST";
    
    i4_rc = x_satl_create ( s_name,
                           ui2_satl_id,
                           SATL_MEM_DYNAMIC,  /* e_policy */
                           20,              /* ui2_max_rec */
                           NULL,
                           NULL,            /* satl_nfy_fct_wr, */
                           &h_satl );

    if ( i4_rc == SATLR_OK )
    {
        x_dbg_stmt("<satl> SATL object created: [%d %s]\n", ui2_satl_id, s_name);
    }
    else
    {
        x_dbg_stmt("<satl> can not create SATL object.\n");
        return;
    }

    x_satl_lock(h_satl);
    
    for ( i4_i=1;  i4_i < 10; i4_i++ )
    {
        x_satl_rec_init(&t_satl_rec);

        t_satl_rec.ui2_satl_rec_id = (UINT16) i4_i;
        t_satl_rec.ui4_mask = 0xFFFFABCD;
        t_satl_rec.ulnb.t_lnb.e_lnb_type = LNB_SINGLE_FREQ;
        t_satl_rec.ulnb.t_lnb.ui2_lnb_low_freq = (UINT16) (i4_i * 50);
        t_satl_rec.udiseqc.t_diseqc_info.e_diseqc_type = DISEQC_NONE;
        t_satl_rec.i2_orb_pos = (INT16) ((i4_i - 5) * 20);
        x_strcpy(t_satl_rec.ac_sat_name,"test_sat");

        i4_rc = x_satl_add_rec(h_satl, &t_satl_rec);
        if ( i4_rc == SATLR_OK )
        {
            x_dbg_stmt("satl rec added: %x %d %d %d %s \n",
                       t_satl_rec.ui4_mask,
                       t_satl_rec.ui2_satl_rec_id,
                       t_satl_rec.ulnb.t_lnb.ui2_lnb_low_freq,
                       t_satl_rec.i2_orb_pos,
                       t_satl_rec.ac_sat_name);
        }
        else
        {
            x_dbg_stmt("Add SATL record failed.\n");
        }
    }

    x_satl_unlock(h_satl);

    x_satl_close(h_satl);
    
    return;
}

