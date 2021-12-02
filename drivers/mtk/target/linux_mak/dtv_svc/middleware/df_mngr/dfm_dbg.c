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
 * $RCSfile: dfm_dbg.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains functions and internal variables to support
 *         CLI control of the DFM library.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/def_dbg_level_mw.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"

#include "x_df_mngr.h"
#include "os/inc/x_os.h"
#include "_dfm_eng_data.h"
#include "df_mngr.h"
#include "dfm_lock.h"
#include "dfm_hdlr.h"
#include "dfm_dbg.h"
/*
  internal variable for the debug level.
*/
static UINT16  ui2_dfm_dbg_level = DBG_INIT_LEVEL_MW_DFM;

/*----------------------------------------------
  Public Function for DBG support.
------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: dfm_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -  
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/ 
UINT16 dfm_get_dbg_level(VOID)
{
    return ui2_dfm_dbg_level;    
}


/*-----------------------------------------------------------------------------
 * Name: dfm_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level        The new setting of debug level.  
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/ 
BOOL dfm_set_dbg_level(UINT16 ui2_level)
{    
   ui2_dfm_dbg_level = ui2_level;    

    return TRUE;
}

#ifdef DEBUG
/*-------------------------------------------------------------------------
 * Name: dfm_print_rec_req
 *
 * Description: This API prints out dfm record entries for the
 *    specified logo_data.
 *
 * Inputs:  
 *    ui2_svl_id:     SVL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static VOID  dfm_print_rec_req(LOGO_CTRL_T*  pt_logo_req, INT32  i4_idx)
{
    
    x_dbg_stmt("[%d] \n"
               "On id: %d[0x%X] \n"
               "Service id: %d[0x%X] \n"
               "Scan index: %d[0x%X] \n"
               "Is sdt ready: %d \n"
               "Is simple logo: %d \n",
               i4_idx,
               pt_logo_req->ui2_onid, pt_logo_req->ui2_onid,               
               pt_logo_req->ui2_svc_id, pt_logo_req->ui2_svc_id,
               pt_logo_req->ui2_scan_idx, pt_logo_req->ui2_scan_idx,
               pt_logo_req->b_sdt_ready_flag,
               pt_logo_req->b_simp_logo
               );
    
    x_dbg_stmt("Request condition :[%d]",pt_logo_req->e_cond);

    switch(pt_logo_req->e_cond)
    {
        case DFM_COND_AVAILABLE:
        {
            x_dbg_stmt(" DFM_COND_AVAILABLE\r\n");                    
        }
        break;

        case DFM_COND_UNAVAILABLE :
        {
            x_dbg_stmt(" DFM_COND_UNAVAILABLE\r\n ");
        }
        break;

        case DFM_COND_UPDATE:
        {
            x_dbg_stmt(" DFM_COND_UPDATE\r\n ");
        }
        break;

        case DFM_COND_TRANSITION:
        {
            x_dbg_stmt(" DFM_COND_TRANSITION\r\n ");
        }
        break;
        
        default:
        {
            x_dbg_stmt(" Unknow\r\n ");
        }
        break;
    }
    

    
}


/*-------------------------------------------------------------------------
 * Name: dfm_print_rec
 *
 * Description: This API prints out dfm record entries for the
 *    specified logo_data.
 *
 * Inputs:  
 *    ui2_svl_id:     SVL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static VOID  dfm_print_rec(LOGO_DATA_T*  pt_logo_rec, INT32  i4_idx)
{
    LOGO_TYPE_ENTRY_T*  pt_logo_type_entry = NULL;
    LOGO_CTRL_T*        pt_temp_ctrl_data = NULL;
    INT32               i4_pos ;
    
    x_dbg_stmt("[%d] \n"
               "On id: %d[0x%X] \n"
               "Download data id: %d[0x%X] \n"
               "Logo id: %d[0x%X] \n"
               "Logo version: %d[0x%X] \n"
               "Scan index: %d[0x%X] \n",
               i4_idx,
               pt_logo_rec->ui2_onid, pt_logo_rec->ui2_onid,
               pt_logo_rec->ui2_dd_data_id, pt_logo_rec->ui2_dd_data_id,
               pt_logo_rec->ui2_logo_id, pt_logo_rec->ui2_logo_id,
               pt_logo_rec->ui2_logo_ver, pt_logo_rec->ui2_logo_ver,
               pt_logo_rec->ui2_scan_idx, pt_logo_rec->ui2_scan_idx
               );
    x_dbg_stmt("Relative logo request: \n");
    pt_temp_ctrl_data = DLIST_HEAD(&(pt_logo_rec->t_logo_ctrl_list));
    i4_pos = 1;
    while (pt_temp_ctrl_data != NULL)
    {
        x_dbg_stmt("(%d) onid: %d[0x%X], Service id: %d[0x%X], scan index: %d[0x%X]\n", i4_pos,
            pt_temp_ctrl_data->ui2_onid,pt_temp_ctrl_data->ui2_onid,
            pt_temp_ctrl_data->ui2_svc_id,pt_temp_ctrl_data->ui2_svc_id,
            pt_temp_ctrl_data->ui2_scan_idx,pt_temp_ctrl_data->ui2_scan_idx);
        pt_temp_ctrl_data = DLIST_NEXT(pt_temp_ctrl_data, t_link_2);
        i4_pos++;
    }

    x_dbg_stmt("Relative logo data: \n");
    i4_pos=1;
    pt_logo_type_entry = pt_logo_rec->pt_logo_type_entry_list;
    while (pt_logo_type_entry != NULL)
    {
        x_dbg_stmt("(%d) Logo data length: %d; \t", i4_pos++,
               pt_logo_type_entry->ui2_logo_data_len);
        x_dbg_stmt("Logo type:");

        switch(pt_logo_type_entry->ui1_logo_type)
        {
            case DFM_LOGO_TYPE_SD4_3_SMAlL :
            {
                x_dbg_stmt(" SD4:3 small\r\n");                    
            }
            break;

            case DFM_LOGO_TYPE_SD16_9_SMALL :
            {
                x_dbg_stmt(" SD16:9 small\r\n ");
            }
            break;

            case DFM_LOGO_TYPE_HD_SMALL :
            {
                x_dbg_stmt(" HD small\r\n ");
            }
            break;

            case DFM_LOGO_TYPE_SD4_3_LARGE :
            {
                x_dbg_stmt(" SD4:3 large\r\n ");
            }
            break;

            case DFM_LOGO_TYPE_SD16_9_LARGE :
            {
                x_dbg_stmt(" SD16:9 large\r\n ");
            }
            break;
                        
            case DFM_LOGO_TYPE_HD_LARGE :
            {
                x_dbg_stmt(" HD large\r\n ");
            }
            break;
            
            case DFM_LOGO_TYPE_SIMP :
            {
                x_dbg_stmt(" Simple logo\r\n ");
            }
            break;
            
            case DFM_LOGO_TYPE_UNKNOWN:
            default:
            {
                x_dbg_stmt(" Unknow\r\n ");
            }
            break;
        }
        pt_logo_type_entry = pt_logo_type_entry->pt_next;
    }

    
}
#endif

/*-------------------------------------------------------------------------
 * Name: dfm_dump_rec_req
 *
 * Description: This API dumps out dfm logo record request.
 *
 * Inputs:  
 *    -
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID dfm_dump_rec_req()
{
#ifdef DEBUG
    HANDLE_T   h_dfm;
    INT32      i4_rc;
    HANDLE_TYPE_T e_obj_type;
    DFM_T*        pt_dfm = NULL;
    
    LOGO_CTRL_T*  pt_logo_req = NULL;
    INT32              i4_idx;
    
    LOGO_CTRL_LIST_T* pt_head_logo_req = NULL;

    i4_rc = x_dfm_open(SN_MAIN_DFM, &h_dfm);
    if (i4_rc != DFMR_OK)
    {
        return;
    }
    
    dfm_class_lock();
    if (((handle_get_type_obj (h_dfm, &e_obj_type, (VOID**)(&pt_dfm))) != HR_OK) || 
        (e_obj_type != HT_DF_MNGR))
    {
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
        dfm_class_unlock();
        return;
    }

    i4_idx = 0;
    pt_head_logo_req = dfm_get_logo_ctrl();
    
    pt_logo_req = DLIST_HEAD( pt_head_logo_req );
    while (NULL != pt_logo_req)
    {
        i4_idx++;
        dfm_print_rec_req(pt_logo_req, i4_idx);

        pt_logo_req = DLIST_NEXT(pt_logo_req, t_link_1);
    }

    dfm_class_unlock();
    return;
#endif
}
/*-------------------------------------------------------------------------
 * Name: dfm_dump_rec
 *
 * Description: This API dumps out dfm logo record entries.
 *
 * Inputs:  
 *    -
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID dfm_dump_rec()
{
#ifdef DEBUG
    HANDLE_T   h_dfm;
    INT32      i4_rc;
    HANDLE_TYPE_T e_obj_type;
    DFM_T*        pt_dfm = NULL;
    
    LOGO_DATA_T*  pt_logo_link = NULL;
    INT32              i4_idx;
    
    LOGO_DATA_LIST_T* pt_head_logo = NULL;

    i4_rc = x_dfm_open(SN_MAIN_DFM, &h_dfm);
    if (i4_rc != DFMR_OK)
    {
        return;
    }
    
    dfm_class_lock();
    if (((handle_get_type_obj (h_dfm, &e_obj_type, (VOID**)(&pt_dfm))) != HR_OK) || 
        (e_obj_type != HT_DF_MNGR))
    {
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
        dfm_class_unlock();
        return;
    }

    i4_idx = 0;
    pt_head_logo = dfm_get_logo_data();
    
    pt_logo_link = DLIST_HEAD( pt_head_logo );
    while (NULL != pt_logo_link)
    {
        i4_idx++;
        dfm_print_rec(pt_logo_link, i4_idx);

        pt_logo_link = DLIST_NEXT(pt_logo_link, t_link);
    }

    dfm_class_unlock();
    return;
#endif
}

    
