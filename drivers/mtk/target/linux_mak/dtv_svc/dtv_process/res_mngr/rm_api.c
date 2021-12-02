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
 * $RCSfile: rm_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: Yan Wang $
 *
 * Description:
 *         This file contains all Resource Manager API related implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/drv/x_irrc.h"
#include "res_mngr/drv/x_avc.h"
#include "res_mngr/drv/x_tv_dec.h"


/* MHF Linux - yjg */
#include "res_mngr/drv/x_aud_dec.h"
#include "res_mngr/drv/x_buf_agt.h"
#include "res_mngr/drv/x_cec.h"
#include "res_mngr/drv/x_demux.h"
#include "res_mngr/drv/x_vid_dec.h"
#include "res_mngr/drv/x_pwr_ctrl.h"
#include "res_mngr/drv/x_vid_plane.h"
#if 1 /* MHF Linux - yjg */
#include "d_inc/panel_cust.h"
#endif /* MHF Linux - yjg */
#include "ioctl/adpt_ioctl.h"
#include "cli/x_cli.h"
#include "handle/handle.h"
#include "handle/x_handle_grp.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "res_mngr/_rm.h"
#include "res_mngr/rm_cli_api.h"
#include "res_mngr/rm_comp_api.h"
#include "res_mngr/rm_db_api.h"
#include "res_mngr/rm_elem_api.h"
#include "res_mngr/rm_fct_tbl_api.h"
#include "res_mngr/rm_lock_api.h"
#include "res_mngr/rm_map_api.h"
#include "res_mngr/rm_mc_nfy_api.h"
#include "res_mngr/rm_name_api.h"
#include "res_mngr/rm_op_api.h"
#include "res_mngr/rm_op_supp_api.h"
#include "res_mngr/rm_pipe_api.h"
#include "res_mngr/rm_port_api.h"
#include "res_mngr/rm_ref_api.h"
#include "res_mngr/rm_strm_api.h"
#include "res_mngr/rm_thread_api.h"
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include "include/cb_inc/cb_param.h"
#include "include/ioctl_inc/ioctl_param.h"
#ifdef DEBUG_RES_MNGR
#include "dbg/dbg.h"
#endif
#include "callback/mw_adpt_cb_table.h"
#include <fcntl.h>

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifndef PRT_DBG
#define PRT_DBG(x, ...)  
#endif

#define RM_DEBUG_INF(x, ...) 

/* Currently, we only support a single output port. */
#define MAX_NUM_INP_PORTS  MAX_NUM_PORTS
#define MAX_NUM_OUT_PORTS  MAX_NUM_PORTS

#define MAX_NUM_INP_PORTS_SUPPORT_COMP  1
#define MAX_NUM_OUT_PORTS_SUPPORT_COMP  1

#define RM_COMP_NFY 0
#define RM_SEL_NFY  1



/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*static VOID *pv_param_addr;*/
static VOID *pv_drv_nfy_param_buffer;
extern void cb_entry(void);
extern INT32 rm_swi_open_comp (
            HANDLE_T           h_obj,
            OBJ_TO_COMP_REL_T  e_obj_to_comp_rel,
            DRV_TYPE_T         e_type,
            UINT16             ui2_id,
            const CHAR*        ps_name,
            CTRL_TYPE_T        e_ctrl,
            VOID*              pv_comp_sel_data,
            rm_comp_sel_fct    pf_comp_sel,
            VOID*              pv_tag,
            rm_nfy_fct         pf_nfy,
            HANDLE_T*          ph_comp,
            RM_COND_T*         pe_cond);
extern  void*  ssma_alloc(size_t size);
extern void ssma_free(void * p);
extern int CB_InitMwAdptLib(void);

extern void register_cb_entry(void);

PARAM_OPEN_COMP *p_open_comp;
static HANDLE_T  h_rm_cb_sema;
static UINT8 open_share_buffer[1024];
/*static int dd2 = 1;*/

INT32 rm_open_comp (HANDLE_T           h_obj,
                    OBJ_TO_COMP_REL_T  e_obj_to_comp_rel,
                    DRV_TYPE_T         e_type,
                    UINT16             ui2_id,
                    const CHAR*        ps_name,
                    CTRL_TYPE_T        e_ctrl,
                    VOID*              pv_comp_sel_data,
                    rm_comp_sel_fct    pf_comp_sel,
                    VOID*              pv_tag,
                    rm_nfy_fct         pf_nfy,
                    HANDLE_T*          ph_comp,
                    RM_COND_T*         pe_cond)
{
    PARAM_RM_OPEN_COMP *pt_rm_open_comp;
    INT32 i4_ret;
    RM_DEBUG_INF("[MD:ResMgr] rm_open_comp\n\n");

#ifdef SWI_CALLBACK
    return    rm_swi_open_comp(h_obj,
                               e_obj_to_comp_rel,
                               e_type,
                               ui2_id,
                               ps_name,
                               e_ctrl,
                               pv_comp_sel_data,
                               pf_comp_sel,
                               pv_tag,
                               pf_nfy,
                               ph_comp,
                               pe_cond);
#endif

    x_sema_lock (h_rm_cb_sema, X_SEMA_OPTION_WAIT);
    pt_rm_open_comp = (PARAM_RM_OPEN_COMP *)ssma_alloc(sizeof(PARAM_RM_OPEN_COMP));
    if (pt_rm_open_comp == NULL)
    {
        x_sema_unlock (h_rm_cb_sema);
        return -1;
    }
    pt_rm_open_comp->h_obj = h_obj;
    pt_rm_open_comp->e_obj_to_comp_rel = e_obj_to_comp_rel;
    pt_rm_open_comp->e_type = e_type;
    pt_rm_open_comp->ui2_id = ui2_id;
    pt_rm_open_comp->ps_name = ps_name;
    pt_rm_open_comp->e_ctrl = e_ctrl;
    pt_rm_open_comp->pv_comp_sel_data = pv_comp_sel_data;
    pt_rm_open_comp->pf_comp_sel = pf_comp_sel;
    pt_rm_open_comp->pv_tag = pv_tag;
    pt_rm_open_comp->pf_nfy = pf_nfy;
    pt_rm_open_comp->ph_comp = ph_comp;
    pt_rm_open_comp->pe_cond = pe_cond;
    pt_rm_open_comp->pv_open_share_buffer = open_share_buffer;
    PRT_DBG("rm_api:open comp sel func=%lx\n\n", pt_rm_open_comp->pf_comp_sel);
    i4_ret = ADPT_IOCTL(IOCTL_RM_OPEN_COMP, pt_rm_open_comp);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_open_comp->i4_ret;
    }
    ssma_free(pt_rm_open_comp);
    x_sema_unlock (h_rm_cb_sema);
    return i4_ret;
}



INT32 rm_init (UINT16           ui2_num_comps,
               THREAD_DESCR_T*  pt_thread_descr)
{
    PARAM_RM_INIT *pt_rm_init;
    INT32 i4_ret;
    
    if ((x_sema_create (&h_rm_cb_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK)) != OSR_OK)
    {
        PRT_DBG("rm cb sema create error\n\n");
    }
    if(CB_InitMwAdptLib() != 0)   
    {
        PRT_DBG("CB_InitMwAdptLib Fail \n");
    }

    adpt_ioctl_init();
    
    i4_ret = ADPT_IOCTL(IOCTRL_INIT_SWI, 0);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        printf(" TO USER LEAVE :\n");
    }
    else
    {
        printf("rm_init: IOCTL_INIT_SWI failed.\n");
        return i4_ret;
    }
    register_cb_entry();
    
    pt_rm_init = (PARAM_RM_INIT *)ssma_alloc(sizeof(PARAM_RM_INIT));
    if (pt_rm_init == NULL)
    {
        return -1;
    }
    pt_rm_init->ui2_num_comps = ui2_num_comps;
    pt_rm_init->pt_thread_descr = pt_thread_descr;
    i4_ret = ADPT_IOCTL(IOCTL_RM_INIT, pt_rm_init);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_init->i4_ret;
    }
    ssma_free(pt_rm_init);

    if (i4_ret == 0)
    {
        pv_drv_nfy_param_buffer = malloc(2048000);
        if (pv_drv_nfy_param_buffer != NULL)
        {
            i4_ret = ADPT_IOCTL(IOCTRL_SET_DRV_NFY_PARAM_BUFFER, pv_drv_nfy_param_buffer);
        }

        rm_reg_cb_table();
    }

    return i4_ret;
}


INT32 rm_cancel_arb (HANDLE_T    h_obj,
                     RM_COND_T*  pe_cond)
{
    PARAM_RM_CANCEL_ARB_T *pt_rm_cancel_arb;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_cancel_arb\n\n");

    pt_rm_cancel_arb = (PARAM_RM_CANCEL_ARB_T *)ssma_alloc(sizeof(PARAM_RM_CANCEL_ARB_T));
    if (pt_rm_cancel_arb == NULL)
    {
        return -1;
    }
    pt_rm_cancel_arb->h_obj = h_obj;
    pt_rm_cancel_arb->pe_cond = pe_cond;
    i4_ret = ADPT_IOCTL(IOCTL_RM_CANCEL_ARB, pt_rm_cancel_arb);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_cancel_arb->i4_ret;
    }
    ssma_free(pt_rm_cancel_arb);

    return i4_ret;
}

INT32 rm_close (HANDLE_T    h_obj,
                BOOL        b_restore_conn,
                RM_COND_T*  pe_cond)
{
    PARAM_RM_CLOSE_T *pt_rm_close;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_close\n\n");
    x_sema_lock (h_rm_cb_sema, X_SEMA_OPTION_WAIT);
    
    pt_rm_close = (PARAM_RM_CLOSE_T *)ssma_alloc(sizeof(PARAM_RM_CLOSE_T));
    if (pt_rm_close == NULL)
    {
        x_sema_unlock (h_rm_cb_sema);
        return -1;
    }
    pt_rm_close->h_obj = h_obj;
    pt_rm_close->b_restore_conn = b_restore_conn;
    pt_rm_close->pe_cond = pe_cond;
    i4_ret = ADPT_IOCTL(IOCTL_RM_CLOSE, pt_rm_close);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_close->i4_ret;
    }
    ssma_free(pt_rm_close);

    x_sema_unlock(h_rm_cb_sema);
    return i4_ret;
}

RM_COND_T rm_cond_chg (RM_COND_T   e_new_cond,
                       RM_COND_T   e_old_cond,
                       COND_GRP_T  e_cond_grp)
{
    PARAM_RM_COND_CHG_T *pt_rm_cond_chg;
    RM_COND_T   t_rm_cond = RM_COND_IGNORE;
    INT32       i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_cond_chg\n\n");

    pt_rm_cond_chg = (PARAM_RM_COND_CHG_T *)ssma_alloc(sizeof(PARAM_RM_COND_CHG_T));
    if (pt_rm_cond_chg == NULL)
    {
        return -1;
    }
    pt_rm_cond_chg->e_new_cond = e_new_cond;
    pt_rm_cond_chg->e_old_cond = e_old_cond;
    pt_rm_cond_chg->e_cond_grp = e_cond_grp;
    i4_ret = ADPT_IOCTL(IOCTL_RM_COND_CHG, pt_rm_cond_chg);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        t_rm_cond = pt_rm_cond_chg->t_rm_cond;
    }
    ssma_free(pt_rm_cond_chg);

    return t_rm_cond;
}

INT32 rm_connect (HANDLE_T         h_obj,
                  DRV_CONN_TYPE_T  e_conn_type,
                  const VOID*      pv_conn_info,
                  SIZE_T           z_conn_info_len,
                  VOID*            pv_tag,
                  RM_COND_T*       pe_cond)
{
    PARAM_RM_CONNECT_T *pt_rm_connect;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_connect\n\n");
/*    x_sema_lock (h_rm_cb_sema, X_SEMA_OPTION_WAIT);*/
    
    pt_rm_connect = (PARAM_RM_CONNECT_T *)ssma_alloc(sizeof(PARAM_RM_CONNECT_T));
    if (pt_rm_connect == NULL)
    {
/*        x_sema_unlock (h_rm_cb_sema);*/
        return -1;
    }
    pt_rm_connect->h_obj = h_obj;
    pt_rm_connect->e_conn_type = e_conn_type;
    pt_rm_connect->pv_conn_info = pv_conn_info;
    pt_rm_connect->z_conn_info_len = z_conn_info_len;
    pt_rm_connect->pv_tag = pv_tag;
    pt_rm_connect->pe_cond = pe_cond;
    i4_ret = ADPT_IOCTL(IOCTL_RM_CONNECT, pt_rm_connect);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_connect->i4_ret;
    }
    ssma_free(pt_rm_connect);

/*    x_sema_unlock(h_rm_cb_sema);*/
    
    return i4_ret;
}

INT32 rm_disconnect (HANDLE_T         h_obj,
                     DRV_DISC_TYPE_T  e_disc_type,
                     const VOID*      pv_disc_info,
                     SIZE_T           z_disc_info_len,
                     RM_COND_T*       pe_cond)
{
    PARAM_RM_DISCONNECT_T *pt_rm_disconnect;
    INT32 i4_ret;
    
    RM_DEBUG_INF("[MD:ResMgr] rm_disconnect\n\n");
/*    x_sema_lock (h_rm_cb_sema, X_SEMA_OPTION_WAIT);*/

    pt_rm_disconnect = (PARAM_RM_DISCONNECT_T *)ssma_alloc(sizeof(PARAM_RM_DISCONNECT_T));
    if (pt_rm_disconnect == NULL)
    {
/*        x_sema_unlock (h_rm_cb_sema);*/
        return -1;
    }
    pt_rm_disconnect->h_obj = h_obj;
    pt_rm_disconnect->e_disc_type = e_disc_type;
    pt_rm_disconnect->pv_disc_info = pv_disc_info;
    pt_rm_disconnect->z_disc_info_len = z_disc_info_len;
    pt_rm_disconnect->pe_cond = pe_cond;
    i4_ret = ADPT_IOCTL(IOCTL_RM_DISCONNECT, pt_rm_disconnect);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_disconnect->i4_ret;
    }
    ssma_free(pt_rm_disconnect);

/*    x_sema_unlock(h_rm_cb_sema);*/

    return i4_ret;
}

INT32 rm_get (HANDLE_T        h_obj,
              BOOL            b_sel_out_port,
              UINT8           ui1_port,
              DRV_GET_TYPE_T  e_get_type,
              VOID*           pv_get_info,
              SIZE_T*         pz_get_info_len,
              RM_COND_T*      pe_cond)
{
    PARAM_RM_GET_T *pt_rm_get;
    INT32 i4_ret;
    RM_DEBUG_INF("[MD:ResMgr] rm_get-0\n\n");

    pt_rm_get = (PARAM_RM_GET_T *)ssma_alloc(sizeof(PARAM_RM_GET_T));
    if (pt_rm_get == NULL)
    {
        return -1;
    }
    pt_rm_get->h_obj = h_obj;
    pt_rm_get->b_sel_out_port = b_sel_out_port;
    pt_rm_get->ui1_port = ui1_port;
    pt_rm_get->e_get_type = e_get_type;
    pt_rm_get->pv_get_info = pv_get_info;
    pt_rm_get->pz_get_info_len = pz_get_info_len;
    pt_rm_get->pe_cond = pe_cond;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET, pt_rm_get);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get->i4_ret;
    }
    ssma_free(pt_rm_get);

    return i4_ret;
}

INT32 rm_get_comp_info (DRV_TYPE_T  e_type,
                        UINT16      ui2_idx,
                        UINT16*     pui2_id,
                        UINT8*      pui1_num_inp_ports,
                        UINT8*      pui1_num_out_ports,
                        UINT32*     pui4_comp_flags)
{
    INT32 i4_ret;

    PARAM_RM_GET_COMP_INFO_T *pt_rm_get_comp_info;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_comp_info\n\n");

    pt_rm_get_comp_info = (PARAM_RM_GET_COMP_INFO_T *)ssma_alloc(sizeof(PARAM_RM_GET_COMP_INFO_T));
    if (pt_rm_get_comp_info == NULL)
    {
        return -1;
    }
    pt_rm_get_comp_info->e_type = e_type;
    pt_rm_get_comp_info->ui2_idx = ui2_idx;
    pt_rm_get_comp_info->pui2_id = pui2_id;
    pt_rm_get_comp_info->pui1_num_inp_ports = pui1_num_inp_ports;
    pt_rm_get_comp_info->pui1_num_out_ports = pui1_num_out_ports;
    pt_rm_get_comp_info->pui4_comp_flags = pui4_comp_flags;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_COMP_INFO, pt_rm_get_comp_info);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_comp_info->i4_ret;
    }
    ssma_free(pt_rm_get_comp_info);

    return i4_ret;
}

INT32 rm_get_comp_info_by_id (DRV_TYPE_T  e_type,
                              UINT16      ui2_id,
                              UINT8*      pui1_num_inp_ports,
                              UINT8*      pui1_num_out_ports,
                              UINT32*     pui4_comp_flags)
{
    PARAM_RM_GET_COMP_INFO_BY_ID_T *pt_rm_get_comp_info_by_id;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_comp_info_by_id\n\n");
    
    pt_rm_get_comp_info_by_id = (PARAM_RM_GET_COMP_INFO_BY_ID_T *)ssma_alloc(sizeof(PARAM_RM_GET_COMP_INFO_BY_ID_T));
    if (pt_rm_get_comp_info_by_id == NULL)
    {
        return -1;
    }
    pt_rm_get_comp_info_by_id->e_type = e_type;
    pt_rm_get_comp_info_by_id->ui2_id = ui2_id;
    pt_rm_get_comp_info_by_id->pui1_num_inp_ports = pui1_num_inp_ports;
    pt_rm_get_comp_info_by_id->pui1_num_out_ports = pui1_num_out_ports;
    pt_rm_get_comp_info_by_id->pui4_comp_flags = pui4_comp_flags;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_COMP_INFO_BY_ID, pt_rm_get_comp_info_by_id);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_comp_info_by_id->i4_ret;
    }
    ssma_free(pt_rm_get_comp_info_by_id);

    return i4_ret;
}

INT32 rm_get_comp_info_from_handle (HANDLE_T     h_obj,
                                    DRV_TYPE_T*  pe_type,
                                    UINT16*     pui2_id,
                                    UINT8*       pui1_num_inp_ports,
                                    UINT8*       pui1_num_out_ports,
                                    UINT32*      pui4_comp_flags )
{
    PARAM_RM_GET_COMP_INFO_FROM_HANDLE_T *pt_rm_get_comp_info_from_handle;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_comp_info_from_handle\n\n");
    
    pt_rm_get_comp_info_from_handle = (PARAM_RM_GET_COMP_INFO_FROM_HANDLE_T *)ssma_alloc(sizeof(PARAM_RM_GET_COMP_INFO_FROM_HANDLE_T));
    if (pt_rm_get_comp_info_from_handle == NULL)
    {
        return -1;
    }
    pt_rm_get_comp_info_from_handle->h_obj = h_obj;
    pt_rm_get_comp_info_from_handle->pe_type = pe_type;
    pt_rm_get_comp_info_from_handle->pui2_id = pui2_id;
    pt_rm_get_comp_info_from_handle->pui1_num_inp_ports = pui1_num_inp_ports;
    pt_rm_get_comp_info_from_handle->pui1_num_out_ports = pui1_num_out_ports;
    pt_rm_get_comp_info_from_handle->pui4_comp_flags = pui4_comp_flags;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_COMP_INFO_FROM_HANDLE, pt_rm_get_comp_info_from_handle);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_comp_info_from_handle->i4_ret;
    }
    ssma_free(pt_rm_get_comp_info_from_handle);

    return i4_ret;
}

INT32 rm_get_cond (HANDLE_T    h_obj,
                   RM_COND_T*  pe_cond)
{
    PARAM_RM_GET_COND_T *pt_rm_get_cond;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_cond\n\n");
    
    pt_rm_get_cond = (PARAM_RM_GET_COND_T *)ssma_alloc(sizeof(PARAM_RM_GET_COND_T));
    if (pt_rm_get_cond == NULL)
    {
        return -1;
    }
    pt_rm_get_cond->h_obj = h_obj;
    pt_rm_get_cond->pe_cond = pe_cond;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_COND, pt_rm_get_cond);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_cond->i4_ret;
    }
    ssma_free(pt_rm_get_cond);

    return i4_ret;
}

INT32 rm_get_conn_info_from_handle (HANDLE_T     h_obj,
                                    DRV_TYPE_T*  pe_type,
                                    UINT16*      pui2_id,
                                    UINT8*       pui1_out_port)
{
    PARAM_RM_GET_CONN_INFO_FROM_HANDLE_T *pt_rm_get_conn_info_from_handle;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_conn_info_from_handle\n\n");

    pt_rm_get_conn_info_from_handle = (PARAM_RM_GET_CONN_INFO_FROM_HANDLE_T *)ssma_alloc(sizeof(PARAM_RM_GET_CONN_INFO_FROM_HANDLE_T));
    if (pt_rm_get_conn_info_from_handle == NULL)
    {
        return -1;
    }
    pt_rm_get_conn_info_from_handle->h_obj = h_obj;
    pt_rm_get_conn_info_from_handle->pe_type = pe_type;
    pt_rm_get_conn_info_from_handle->pui2_id = pui2_id;
    pt_rm_get_conn_info_from_handle->pui1_out_port = pui1_out_port;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_CONN_INFO_FROM_HANDLE, pt_rm_get_conn_info_from_handle);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_conn_info_from_handle->i4_ret;
    }
    ssma_free(pt_rm_get_conn_info_from_handle);

    return i4_ret;
}

INT32 rm_get_conn_state (HANDLE_T          h_obj_1,
                         HANDLE_T          h_obj_2,
                         CONN_STATE_T*     pt_conn_state,
                         UINT16*           pui2_num_entries,
                         CONN_DIR_TYPE_T*  pe_conn_dir)
{
    PARAM_RM_GET_CONN_STATE_T *pt_rm_get_conn_state;
    INT32 i4_ret;
    
    pt_rm_get_conn_state = (PARAM_RM_GET_CONN_STATE_T *)ssma_alloc(sizeof(PARAM_RM_GET_CONN_STATE_T));
    if (pt_rm_get_conn_state == NULL)
    {
        return -1;
    }
    pt_rm_get_conn_state->h_obj_1 = h_obj_1;
    pt_rm_get_conn_state->h_obj_2 = h_obj_2;
    pt_rm_get_conn_state->pt_conn_state = pt_conn_state;
    pt_rm_get_conn_state->pui2_num_entries = pui2_num_entries;
    pt_rm_get_conn_state->pe_conn_dir = pe_conn_dir;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_CONN_STATE, pt_rm_get_conn_state);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_conn_state->i4_ret;
    }
    ssma_free(pt_rm_get_conn_state);

    RM_DEBUG_INF("[MD:ResMgr] rm_get_conn_state\n\n");

    return i4_ret;
}

INT32 rm_get_len (HANDLE_T        h_obj,
                  BOOL            b_sel_out_port,
                  UINT8           ui1_port,
                  DRV_GET_TYPE_T  e_get_type,
                  SIZE_T*         pz_get_info_len,
                  RM_COND_T*      pe_cond)
{
    PARAM_RM_GET_LEN_T *pt_rm_get_len;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_len\n\n");

    pt_rm_get_len = (PARAM_RM_GET_LEN_T *)ssma_alloc(sizeof(PARAM_RM_GET_LEN_T));
    if (pt_rm_get_len == NULL)
    {
        return -1;
    }
    pt_rm_get_len->h_obj = h_obj;
    pt_rm_get_len->b_sel_out_port = b_sel_out_port;
    pt_rm_get_len->ui1_port = ui1_port;
    pt_rm_get_len->e_get_type = e_get_type;
    pt_rm_get_len->pz_get_info_len = pz_get_info_len;
    pt_rm_get_len->pe_cond = pe_cond;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_LEN, pt_rm_get_len);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_len->i4_ret;
    }
    ssma_free(pt_rm_get_len);

    return i4_ret;
}

INT32 rm_get_num_conn_states (HANDLE_T  h_obj_1,
                              HANDLE_T  h_obj_2,
                              UINT16*   pui2_num_entries)
{
    PARAM_RM_GET_NUM_CONN_STATES_T *pt_rm_get_num_conn_states;
    INT32 i4_ret;

    pt_rm_get_num_conn_states = (PARAM_RM_GET_NUM_CONN_STATES_T *)ssma_alloc(sizeof(PARAM_RM_GET_NUM_CONN_STATES_T));
    if (pt_rm_get_num_conn_states == NULL)
    {
        return -1;
    }
    pt_rm_get_num_conn_states->h_obj_1 = h_obj_1;
    pt_rm_get_num_conn_states->h_obj_2 = h_obj_2;
    pt_rm_get_num_conn_states->pui2_num_entries = pui2_num_entries;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_NUM_CONN_STATES, pt_rm_get_num_conn_states);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_num_conn_states->i4_ret;
    }
    ssma_free(pt_rm_get_num_conn_states);

    RM_DEBUG_INF("[MD:ResMgr] rm_get_num_conn_states\n\n");

    return i4_ret;
}

INT32 rm_get_pipe_flags (HANDLE_T h_obj,
                         UINT32*  pui4_flags)
{
    PARAM_RM_GET_PIPE_FLAGS_T *pt_rm_get_pipe_flags;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr]rm_get_pipe_flags\n\n");

    pt_rm_get_pipe_flags = (PARAM_RM_GET_PIPE_FLAGS_T *)ssma_alloc(sizeof(PARAM_RM_GET_PIPE_FLAGS_T));
    if (pt_rm_get_pipe_flags == NULL)
    {
        return -1;
    }
    pt_rm_get_pipe_flags->h_obj = h_obj;
    pt_rm_get_pipe_flags->pui4_flags = pui4_flags;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_PIPE_FLAGS, pt_rm_get_pipe_flags);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_pipe_flags->i4_ret;
    }
    ssma_free(pt_rm_get_pipe_flags);

    return i4_ret;
}

INT32 rm_get_pipe_comp_info (HANDLE_T    h_obj,
                             DRV_TYPE_T  e_type,
                             UINT16      ui2_idx,
                             UINT16*     pui2_id,
                             UINT8*      pui1_num_inp_ports,
                             UINT8*      pui1_num_out_ports,
                             UINT32*     pui4_comp_flags)
{
    PARAM_RM_GET_PIPE_COMP_INFO_T *pt_rm_get_pipe_comp_info;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_pipe_comp_info\n\n");

    pt_rm_get_pipe_comp_info = (PARAM_RM_GET_PIPE_COMP_INFO_T *)ssma_alloc(sizeof(PARAM_RM_GET_PIPE_COMP_INFO_T));
    if (pt_rm_get_pipe_comp_info == NULL)
    {
        return -1;
    }
    pt_rm_get_pipe_comp_info->h_obj = h_obj;
    pt_rm_get_pipe_comp_info->e_type = e_type;
    pt_rm_get_pipe_comp_info->ui2_idx = ui2_idx;
    pt_rm_get_pipe_comp_info->pui2_id = pui2_id;
    pt_rm_get_pipe_comp_info->pui1_num_inp_ports = pui1_num_inp_ports;
    pt_rm_get_pipe_comp_info->pui1_num_out_ports = pui1_num_out_ports;
    pt_rm_get_pipe_comp_info->pui4_comp_flags = pui4_comp_flags;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_PIPE_COMP_INFO, pt_rm_get_pipe_comp_info);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_pipe_comp_info->i4_ret;
    }
    ssma_free(pt_rm_get_pipe_comp_info);

    return i4_ret;
}

INT32 rm_get_pipe_num_comps_of_type (HANDLE_T    h_obj,
                                     DRV_TYPE_T  e_type,
                                     UINT16*     pui2_num_comps)
{
    PARAM_RM_GET_PIPE_NUM_COMPS_OF_TYPE_T *pt_rm_get_pipe_num_comps_of_type;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_pipe_num_comps_of_type\n\n");

    pt_rm_get_pipe_num_comps_of_type = (PARAM_RM_GET_PIPE_NUM_COMPS_OF_TYPE_T *)ssma_alloc(sizeof(PARAM_RM_GET_PIPE_NUM_COMPS_OF_TYPE_T));
    if (pt_rm_get_pipe_num_comps_of_type == NULL)
    {
        return -1;
    }
    pt_rm_get_pipe_num_comps_of_type->h_obj = h_obj;
    pt_rm_get_pipe_num_comps_of_type->e_type = e_type;
    pt_rm_get_pipe_num_comps_of_type->pui2_num_comps = pui2_num_comps;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_PIPE_NUM_COMPS_OF_TYPE, pt_rm_get_pipe_num_comps_of_type);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_pipe_num_comps_of_type->i4_ret;
    }
    ssma_free(pt_rm_get_pipe_num_comps_of_type);

    return i4_ret;
}

INT32 rm_get_pipe_priority (HANDLE_T h_obj,
                            UINT8*   pui1_priority)
{
    PARAM_RM_GET_PIPE_PRIORITY_T *pt_rm_get_pipe_priority;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_pipe_priority\n\n");
    
    pt_rm_get_pipe_priority = (PARAM_RM_GET_PIPE_PRIORITY_T *)ssma_alloc(sizeof(PARAM_RM_GET_PIPE_PRIORITY_T));
    if (pt_rm_get_pipe_priority == NULL)
    {
        return -1;
    }
    pt_rm_get_pipe_priority->h_obj = h_obj;
    pt_rm_get_pipe_priority->pui1_priority = pui1_priority;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_PIPE_PRIORITY, pt_rm_get_pipe_priority);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_pipe_priority->i4_ret;
    }
    ssma_free(pt_rm_get_pipe_priority);

    return i4_ret;
}

INT32 rm_get_pipe_snk_name (HANDLE_T  h_obj,
                            CHAR*     ps_snk_name,
                            SIZE_T*   pz_snk_name_len)
{
    PARAM_RM_GET_PIPE_SNK_NAME_T *pt_rm_get_pipe_snk_name;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_pipe_snk_name\n\n");

    pt_rm_get_pipe_snk_name = (PARAM_RM_GET_PIPE_SNK_NAME_T *)ssma_alloc(sizeof(PARAM_RM_GET_PIPE_SNK_NAME_T));
    if (pt_rm_get_pipe_snk_name == NULL)
    {
        return -1;
    }
    pt_rm_get_pipe_snk_name->h_obj = h_obj;
    pt_rm_get_pipe_snk_name->ps_snk_name = ps_snk_name;
    pt_rm_get_pipe_snk_name->pz_snk_name_len = pz_snk_name_len;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_PIPE_SNK_NAME, pt_rm_get_pipe_snk_name);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_pipe_snk_name->i4_ret;
    }
    ssma_free(pt_rm_get_pipe_snk_name);

    return i4_ret;
}

INT32 rm_get_pipe_src_name (HANDLE_T  h_obj,
                            CHAR*     ps_src_name,
                            SIZE_T*   pz_src_name_len)
{
    PARAM_RM_GET_PIPE_SRC_NAME_T *pt_rm_get_pipe_src_name;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_get_pipe_src_name\n\n");
    
    pt_rm_get_pipe_src_name = (PARAM_RM_GET_PIPE_SRC_NAME_T *)ssma_alloc(sizeof(PARAM_RM_GET_PIPE_SRC_NAME_T));
    if (pt_rm_get_pipe_src_name == NULL)
    {
        return -1;
    }
    pt_rm_get_pipe_src_name->h_obj = h_obj;
    pt_rm_get_pipe_src_name->ps_src_name = ps_src_name;
    pt_rm_get_pipe_src_name->pz_src_name_len = pz_src_name_len;
    i4_ret = ADPT_IOCTL(IOCTL_RM_GET_PIPE_SRC_NAME, pt_rm_get_pipe_src_name);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_pipe_src_name->i4_ret;
    }
    ssma_free(pt_rm_get_pipe_src_name);

    return i4_ret;
}

BOOL rm_matching_pipe (HANDLE_T  h_obj_1,
                       HANDLE_T  h_obj_2)
{
    PARAM_RM_MATCHING_PIPE_T *pt_rm_matching_pipe;
    INT32 i4_ret;
    BOOL b_ret = FALSE;

    RM_DEBUG_INF("[MD:ResMgr] rm_matching_pipe\n\n");

    pt_rm_matching_pipe = (PARAM_RM_MATCHING_PIPE_T *)ssma_alloc(sizeof(PARAM_RM_MATCHING_PIPE_T));
    if (pt_rm_matching_pipe == NULL)
    {
        return -1;
    }
    pt_rm_matching_pipe->h_obj_1 = h_obj_1;
    pt_rm_matching_pipe->h_obj_2 = h_obj_2;
    i4_ret = ADPT_IOCTL(IOCTL_RM_MATCHING_PIPE, pt_rm_matching_pipe);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        b_ret = pt_rm_matching_pipe->b_ret;
    }
    ssma_free(pt_rm_matching_pipe);

    return b_ret;
}


INT32 rm_num_comps (DRV_TYPE_T  e_type,
                    UINT16*     pui2_num_comps)
{
    PARAM_RM_NUM_COMPS_T *pt_rm_num_comps;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_num_comps\n\n");
    pt_rm_num_comps = (PARAM_RM_NUM_COMPS_T *)ssma_alloc(sizeof(PARAM_RM_NUM_COMPS_T));
    if (pt_rm_num_comps == NULL)
    {
        return -1;
    }
    pt_rm_num_comps->e_type = e_type;
    pt_rm_num_comps->pui2_num_comps = pui2_num_comps;
    i4_ret = ADPT_IOCTL(IOCTL_RM_NUM_COMPS, pt_rm_num_comps);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_num_comps->i4_ret;
    }
    ssma_free(pt_rm_num_comps);

    return i4_ret;
}

INT32 rm_open_pipe (const CHAR*  ps_src_name,
                    const CHAR*  ps_snk_name,
                    UINT8        ui1_priority,
                    UINT32       ui4_flags,
                    VOID*        pv_tag,
                    rm_nfy_fct   pf_nfy,
                    HANDLE_T*    ph_pipe,
                    RM_COND_T*   pe_cond)
{
    PARAM_RM_OPEN_PIPE *pt_rm_open_pipe;
    INT32 i4_ret, i4_retold = 0;
    x_sema_lock (h_rm_cb_sema, X_SEMA_OPTION_WAIT);

    RM_DEBUG_INF("[MD:ResMgr] rm_open_pipe\n\n");

    pt_rm_open_pipe = (PARAM_RM_OPEN_PIPE *)ssma_alloc(sizeof(PARAM_RM_OPEN_PIPE));

    if (pt_rm_open_pipe == NULL)
    {
        x_sema_unlock (h_rm_cb_sema);
        return -1;
    }
    pt_rm_open_pipe->ps_src_name = (CHAR*)ps_src_name;
    pt_rm_open_pipe->ps_snk_name = (CHAR*)ps_snk_name;
    pt_rm_open_pipe->ui1_priority = ui1_priority;
    pt_rm_open_pipe->ui4_flags = ui4_flags;
    pt_rm_open_pipe->pv_tag = pv_tag;
    pt_rm_open_pipe->pf_nfy = pf_nfy;
    pt_rm_open_pipe->ph_pipe = ph_pipe;
    pt_rm_open_pipe->pe_cond = pe_cond;   

    i4_ret = ADPT_IOCTL(IOCTL_RM_OPEN_PIPE, pt_rm_open_pipe);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_retold = i4_ret;
        i4_ret = pt_rm_open_pipe->i4_ret;
    }
    ssma_free(pt_rm_open_pipe);

    if (i4_ret != 0)
    {
        printf("^^^^^open pipe fail\n\n");
        printf("[MD:ResMgr] pipe: ret:%d, retold:%d, cond1:0x%x, cond2:0x%x\n", (int)i4_ret, (int)i4_retold, 
                (int)*pt_rm_open_pipe->pe_cond, (int)*pe_cond);
    }
    x_sema_unlock(h_rm_cb_sema);
    return i4_ret;
}

INT32 rm_open_port (HANDLE_T    h_obj,
                    UINT8       ui1_port,
                    VOID*       pv_tag,
                    HANDLE_T*   ph_port,
                    RM_COND_T*  pe_cond)
{
    PARAM_RM_OPEN_PORT_T *pt_rm_open_port;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_open_port\n\n");

    pt_rm_open_port = (PARAM_RM_OPEN_PORT_T *)ssma_alloc(sizeof(PARAM_RM_OPEN_PORT_T));
    if (pt_rm_open_port == NULL)
    {
        return -1;
    }
    pt_rm_open_port->h_obj = h_obj;
    pt_rm_open_port->ui1_port = ui1_port;
    pt_rm_open_port->pv_tag = pv_tag;
    pt_rm_open_port->ph_port = ph_port;
    pt_rm_open_port->pe_cond = pe_cond;
    i4_ret = ADPT_IOCTL(IOCTL_RM_OPEN_PORT, pt_rm_open_port);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_open_port->i4_ret;
    }
    ssma_free(pt_rm_open_port);

    return i4_ret;
}

INT32 rm_reg_comp_val (rm_val_comp_fct   pf_new_val,
                       rm_val_comp_fct*  ppf_old_val)
{
    PARAM_RM_REG_COMP_VAL *pt_rm_reg_comp_val;
    INT32 i4_ret;
    
    RM_DEBUG_INF("[MD:ResMgr] rm_reg_comp_val\n\n");

    pt_rm_reg_comp_val = (PARAM_RM_REG_COMP_VAL *)ssma_alloc(sizeof(PARAM_RM_REG_COMP_VAL));
    if (pt_rm_reg_comp_val == NULL)
    {
        return -1;
    }
    pt_rm_reg_comp_val->pf_new_val = pf_new_val;
    pt_rm_reg_comp_val->ppf_old_val = ppf_old_val;
    i4_ret = ADPT_IOCTL(IOCTL_RM_REG_COMP_VAL, pt_rm_reg_comp_val);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_reg_comp_val->i4_ret;
    }
    ssma_free(pt_rm_reg_comp_val);

    return i4_ret;
}

INT32 rm_resolve_comp (HANDLE_T         h_obj,
                       VOID*            pv_comp_sel_data,
                       rm_comp_sel_fct  pf_comp_sel,
                       RM_COND_T*       pe_cond)
{

    /*TODO: Add SWI to resolve*/
    
    PARAM_RM_RESOLVE_COMP *pt_rm_resolve_comp;
    INT32 i4_ret;
    x_sema_lock (h_rm_cb_sema, X_SEMA_OPTION_WAIT);
    RM_DEBUG_INF("[MD:ResMgr] rm_resolve_comp\n\n");

    pt_rm_resolve_comp = (PARAM_RM_RESOLVE_COMP *)ssma_alloc(sizeof(PARAM_RM_RESOLVE_COMP));

    if (pt_rm_resolve_comp == NULL)
    {
        x_sema_unlock (h_rm_cb_sema);
        return -1;
    }
    pt_rm_resolve_comp->h_obj = h_obj;
    pt_rm_resolve_comp->pv_comp_sel_data = pv_comp_sel_data;
    pt_rm_resolve_comp->pf_comp_sel = pf_comp_sel;
    pt_rm_resolve_comp->pe_cond = pe_cond;
    i4_ret = ADPT_IOCTL(IOCTL_RM_RESOLVE_COMP, pt_rm_resolve_comp);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_resolve_comp->i4_ret;
    }
    ssma_free(pt_rm_resolve_comp);
    x_sema_unlock (h_rm_cb_sema);
    return i4_ret;
}

INT32 rm_respond_ctrl_query (HANDLE_T    h_obj,
                             BOOL        b_response,
                             VOID*       pv_tag,
                             RM_COND_T*  pe_cond)
{
    PARAM_RM_RESPOND_CTRL_QUERY_T *pt_rm_respond_ctrl_query;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_respond_ctrl_query\n\n");

    pt_rm_respond_ctrl_query = (PARAM_RM_RESPOND_CTRL_QUERY_T *)ssma_alloc(sizeof(PARAM_RM_RESPOND_CTRL_QUERY_T));
    if (pt_rm_respond_ctrl_query == NULL)
    {
        return -1;
    }
    pt_rm_respond_ctrl_query->h_obj = h_obj;
    pt_rm_respond_ctrl_query->b_response = b_response;
    pt_rm_respond_ctrl_query->pv_tag = pv_tag;
    pt_rm_respond_ctrl_query->pe_cond = pe_cond;
    i4_ret = ADPT_IOCTL(IOCTL_RM_RESPOND_CTRL_QUERY, pt_rm_respond_ctrl_query);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_respond_ctrl_query->i4_ret;
    }
    ssma_free(pt_rm_respond_ctrl_query);
 
    return i4_ret;
}
INT32 rm_set (HANDLE_T        h_obj,
              BOOL            b_sel_out_port,
              UINT8           ui1_port,
              DRV_SET_TYPE_T  e_set_type,
              const VOID*     pv_set_info,
              SIZE_T          z_set_info_len,
              RM_COND_T*      pe_cond)
{
    PARAM_RM_SET_T *pt_rm_set;
    INT32 i4_ret;
    /*DRV_TYPE_T  e_type;*/
    /*rm_get_comp_info_from_handle(h_obj,&e_type,0,0,0,0);*/
    RM_DEBUG_INF("[MD:ResMgr] rm_set\n\n");
    x_sema_lock (h_rm_cb_sema, X_SEMA_OPTION_WAIT);

    pt_rm_set = (PARAM_RM_SET_T *)ssma_alloc(sizeof(PARAM_RM_SET_T));
    if (pt_rm_set == NULL)
    {
        x_sema_unlock (h_rm_cb_sema);
        return -1;
    }
    pt_rm_set->h_obj = h_obj;
    pt_rm_set->b_sel_out_port = b_sel_out_port;
    pt_rm_set->ui1_port = ui1_port;
    pt_rm_set->e_set_type = e_set_type;
    pt_rm_set->pv_set_info = pv_set_info;
    pt_rm_set->z_set_info_len = z_set_info_len;
    pt_rm_set->pe_cond = pe_cond;
    
    i4_ret = ADPT_IOCTL(IOCTL_RM_SET, pt_rm_set);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_set->i4_ret;
        if(i4_ret < 0)
        {
            printf("[MD:ResMgr] rm_set:e_set_type=%u, h_obj=0x%08x, RET:%d, caller:0x%08x\r\n",
                    (unsigned int)e_set_type, (int)h_obj, (int)i4_ret, (int)(__builtin_return_address(0)));
        }
    }
    else
    {
        printf("[MD:ResMgr] rm_set failed. ioctl return %d\n", (int)i4_ret);
    }
    ssma_free(pt_rm_set);
    x_sema_unlock (h_rm_cb_sema);

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_rm_set_comp
 *
 * Description: This API sets information for a component without having to
 *              open the it first.
 *
 * Inputs:  e_type          Specifies the component type.
 *          ui2_id          Contains a specific component id or ANY_COMP_ID.
 *          b_sel_out_port  Specifies if argument "ui1_port" references an
 *                          input or output port.
 *          ui1_port        Contains the input or output port number.
 *          e_set_type      Specifies the type of information which is
 *                          referenced by argument pv_set_info.
 *          pv_set_info     Contains the set information.
 *          z_set_info_len  Indicates the length of the data in the buffer
 *                          referenced by argument pv_set_info.
 *
 * Outputs: -
 *
 * Returns: RMR_OK                Routine successful.
 *          RMR_INV_ARG           One or more of the arguments contain
 *                                invalid data.
 *          RMR_NOT_ENOUGH_SPACE  Argument pv_get_info is not big enough to
 *                                hold the requested information.
 *          RMR_DRV_ERROR         A driver error occured.
 *          RMR_INV_PORT_NUMBER   Invalid port number specified.
 *          RMR_INV_SET           Unknown get operation.
 ----------------------------------------------------------------------------*/
INT32 x_rm_set_comp (DRV_TYPE_T      e_type,
                     UINT16          ui2_id,
                     BOOL            b_sel_out_port,
                     UINT8           ui1_port,
                     DRV_SET_TYPE_T  e_set_type,
                     const VOID*     pv_set_info,
                     SIZE_T          z_set_info_len)
{
    PARAM_RM_SET_COMP_T *pt_rm_set_comp;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] x_rm_set_comp\n\n");
    x_sema_lock (h_rm_cb_sema, X_SEMA_OPTION_WAIT);

    pt_rm_set_comp = (PARAM_RM_SET_COMP_T *)ssma_alloc(sizeof(PARAM_RM_SET_COMP_T));
    if (pt_rm_set_comp == NULL)
    {
        x_sema_unlock (h_rm_cb_sema);
        return -1;
    }
    pt_rm_set_comp->e_type = e_type;
    pt_rm_set_comp->ui2_id = ui2_id;
    pt_rm_set_comp->b_sel_out_port = b_sel_out_port;
    pt_rm_set_comp->ui1_port = ui1_port;
    pt_rm_set_comp->e_set_type = e_set_type;
    pt_rm_set_comp->pv_set_info = pv_set_info;
    pt_rm_set_comp->z_set_info_len = z_set_info_len;
    
    i4_ret = ADPT_IOCTL(IOCTL_X_RM_SET_COMP, pt_rm_set_comp);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_set_comp->i4_ret;
        if(i4_ret != 0)
        {
            printf("[MD:ResMgr] rm_set_comp:e_set_type=%u drv_type=%d, RET:%d, caller:%x\n\n",
                    (unsigned int)e_set_type, (int)e_type, (int)i4_ret, (int)(__builtin_return_address(0)));
        }
    }
    else
    {
        printf("[MD:ResMgr] rm_set_comp failed. ioctl return %d.\n", (int)i4_ret);
    }
    ssma_free(pt_rm_set_comp);
    x_sema_unlock (h_rm_cb_sema);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_rm_get_comp
 *
 * Description: This API gets information about a component without having to
 *              open the component.
 *
 * Inputs:  e_type           Specifies the component type.
 *          ui2_id           Contains a specific component id or ANY_COMP_ID.
 *          b_sel_out_port   Specifies if argument "ui1_port" references an
 *                           input or output port.
 *          ui1_port         Contains the input or output port number.
 *          e_get_type       Specifies the type of information which shall be
 *                           returned in the buffer, referenced by argument
 *                           pv_get_info.
 *          pz_get_info_len  Contains the length of the buffer which is
 *                           referenced by argument pv_get_info.
 *
 * Outputs: pv_get_info      Contains the retrieved information.
 *          pz_get_info_len  Indicates the length of the valid data in the
 *                           buffer referenced by argument pv_get_info.
 *
 * Returns: RMR_OK                Routine successful.
 *          RMR_INV_ARG           One or more of the arguments contain
 *                                invalid data.
 *          RMR_NOT_ENOUGH_SPACE  Argument pv_get_info is not big enough to
 *                                hold the requested information.
 *          RMR_DRV_ERROR         A driver error occured.
 *          RMR_INV_PORT_NUMBER   Invalid port number specified.
 *          RMR_INV_GET           Unknown get operation.
 ----------------------------------------------------------------------------*/
INT32 x_rm_get_comp (DRV_TYPE_T      e_type,
                     UINT16          ui2_id,
                     BOOL            b_sel_out_port,
                     UINT8           ui1_port,
                     DRV_GET_TYPE_T  e_get_type,
                     VOID*           pv_get_info,
                     SIZE_T*         pz_get_info_len)
{
    PARAM_RM_GET_COMP_T *pt_rm_get_comp;
    INT32 i4_ret;
    RM_DEBUG_INF("[MD:ResMgr] rm_get_comp\n\n");

    pt_rm_get_comp = (PARAM_RM_GET_COMP_T *)ssma_alloc(sizeof(PARAM_RM_GET_COMP_T));
    if (pt_rm_get_comp == NULL)
    {
        return -1;
    }
    pt_rm_get_comp->e_type = e_type;
    pt_rm_get_comp->ui2_id = ui2_id;
    pt_rm_get_comp->b_sel_out_port = b_sel_out_port;
    pt_rm_get_comp->ui1_port = ui1_port;
    pt_rm_get_comp->e_get_type = e_get_type;
    pt_rm_get_comp->pv_get_info = pv_get_info;
    pt_rm_get_comp->pz_get_info_len = pz_get_info_len;
    i4_ret = ADPT_IOCTL(IOCTL_X_RM_GET_COMP, pt_rm_get_comp);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_get_comp->i4_ret;
        if(i4_ret != 0)
        {
            printf("[MD:ResMgr] rm_get_comp:e_get_type=%d drv_type=%d, RET:%d, caller:%x\n\n",
                    (unsigned int)e_get_type, (int)e_type, (int)i4_ret,(int)( __builtin_return_address(0)));
        }
    }
    else
    {
        printf("[MD:ResMgr] rm_get_comp failed. ioctl return %d\n", (int)i4_ret);
    }
    ssma_free(pt_rm_get_comp);

    return i4_ret;
}


INT32 rm_set_pipe_priority (HANDLE_T  h_pipe,
                            UINT8     ui1_priority)
{
    PARAM_RM_SET_PIPE_PRIORITY_T *pt_rm_set_pipe_priority;
    INT32 i4_ret;

    RM_DEBUG_INF("[MD:ResMgr] rm_set_pipe_priority\n\n");
    
    pt_rm_set_pipe_priority = (PARAM_RM_SET_PIPE_PRIORITY_T *)ssma_alloc(sizeof(PARAM_RM_SET_PIPE_PRIORITY_T));
    if (pt_rm_set_pipe_priority == NULL)
    {
        return -1;
    }
    pt_rm_set_pipe_priority->h_pipe = h_pipe;
    pt_rm_set_pipe_priority->ui1_priority = ui1_priority;
    i4_ret = ADPT_IOCTL(IOCTL_RM_SET_PIPE_PRIORITY, pt_rm_set_pipe_priority);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_set_pipe_priority->i4_ret;
    }
    ssma_free(pt_rm_set_pipe_priority);

    return i4_ret;
}

VOID rm_dump_db (VOID)
{
    RM_DEBUG_INF("[MD:ResMgr] rm_dump_db\n\n");
    return;
}

extern VOID* g_pv_drv_config;
INT32 rm_drv_init(GEN_CONFIG_T*  pt_config)
{
    INT32 i4_ret;   
    PARAM_X_DRV_INIT *pt_x_drv_init;
    
    /*UINT32 ui4_panel_id;*/
    static GEN_CONFIG_T t_config;
    t_config = *pt_config;
    t_config.pv_config = g_pv_drv_config;
    pt_x_drv_init = (PARAM_X_DRV_INIT *)ssma_alloc(sizeof(PARAM_X_DRV_INIT));   
    if (pt_x_drv_init == NULL)     
    {
        return -1;  
    }
    pt_x_drv_init->pt_config = &t_config; 
#ifdef DEFAULT_PANEL_SELECT /* MHF Linux - yjg */
    pt_x_drv_init->ui4_panel_id = (UINT32)(DEFAULT_PANEL_SELECT);
#else
    pt_x_drv_init->ui4_panel_id = (UINT32)0;
#endif /* MHF Linux end - yjg */
    i4_ret = ADPT_IOCTL(IOCTL_X_DRV_INIT, pt_x_drv_init); 
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_x_drv_init->i4_ret; 
    }
    ssma_free(pt_x_drv_init);   

    return i4_ret;
}

/*----------------------------------------------------------*/
INT32 x_rm_get_comp_info_from_name (const CHAR*  ps_name,
                                    UINT16       ui2_idx,
                                    DRV_TYPE_T*  pe_type,
                                    UINT16*      pui2_id,
                                    UINT8*       pui1_num_inp_ports,
                                    UINT8*       pui1_num_out_ports,
                                    UINT32*      pui4_comp_flags)
{
    PARAM_X_RM_GET_COMP_INFO_FROM_NAME *pt_x_rm_get_comp_info_from_name;
    INT32 i4_ret;
    
    RM_DEBUG_INF("[MD:ResMgr] x_rm_get_comp_info_from_name\n\n");

    pt_x_rm_get_comp_info_from_name = (PARAM_X_RM_GET_COMP_INFO_FROM_NAME *)ssma_alloc(sizeof(PARAM_X_RM_GET_COMP_INFO_FROM_NAME));
    if (pt_x_rm_get_comp_info_from_name == NULL)
    {
        return -1;
    }
    pt_x_rm_get_comp_info_from_name->ps_name = (CHAR*)ps_name;
    pt_x_rm_get_comp_info_from_name->ui2_idx = ui2_idx;
    pt_x_rm_get_comp_info_from_name->pe_type = pe_type;
    pt_x_rm_get_comp_info_from_name->pui2_id = pui2_id;
    pt_x_rm_get_comp_info_from_name->pui1_num_inp_ports = pui1_num_inp_ports;
    pt_x_rm_get_comp_info_from_name->pui1_num_out_ports = pui1_num_out_ports;
    pt_x_rm_get_comp_info_from_name->pui4_comp_flags = pui4_comp_flags;
    i4_ret = ADPT_IOCTL(IOCTL_X_RM_GET_COMP_INFO_FROM_NAME, pt_x_rm_get_comp_info_from_name);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_x_rm_get_comp_info_from_name->i4_ret;
    }
    ssma_free(pt_x_rm_get_comp_info_from_name);
 
    return i4_ret;
}


INT32  rm_reg_mc_nfy (const MC_NFY_FCT_TBL_T*  pt_mc_nfy_fct_tbl)
{
    PARMA_RM_REG_MC_NFY *pt_rm_reg_mc_nfy;
    INT32 i4_ret;
    static UINT32 mc_share_buffer[512]; 
    RM_DEBUG_INF("[MD:ResMgr] rm_reg_mc_nfy\n\n");
    x_sema_lock (h_rm_cb_sema, X_SEMA_OPTION_WAIT);
    if (pt_mc_nfy_fct_tbl == NULL)
    {
        x_sema_unlock (h_rm_cb_sema);
        return 0;   
    }

    PRT_DBG("---mw:rm_reg_mc_nfy:pt_mc_nfy_fct_tbl->pf_add_dev:%d---", pt_mc_nfy_fct_tbl->pf_add_dev);
    
    pt_rm_reg_mc_nfy = (PARMA_RM_REG_MC_NFY *)ssma_alloc(sizeof(PARMA_RM_REG_MC_NFY));
    if (pt_rm_reg_mc_nfy == NULL)
    {
        x_sema_unlock (h_rm_cb_sema);
        return -1;
    }
    pt_rm_reg_mc_nfy->pt_mc_nfy_fct_tbl = pt_mc_nfy_fct_tbl;
    pt_rm_reg_mc_nfy->pf_add_dev = pt_mc_nfy_fct_tbl->pf_add_dev;
    pt_rm_reg_mc_nfy->pf_del_dev = pt_mc_nfy_fct_tbl->pf_del_dev;
    pt_rm_reg_mc_nfy->pf_rst_dev = pt_mc_nfy_fct_tbl->pf_rst_dev;
    pt_rm_reg_mc_nfy->pf_add_inst = pt_mc_nfy_fct_tbl->pf_add_inst;
    pt_rm_reg_mc_nfy->pf_del_inst = pt_mc_nfy_fct_tbl->pf_del_inst;
    pt_rm_reg_mc_nfy->pf_set = pt_mc_nfy_fct_tbl->pf_set;
    pt_rm_reg_mc_nfy->pv_share_buffer = (VOID *)&mc_share_buffer[0];
    i4_ret = ADPT_IOCTL(IOCTL_RM_REG_MC_NFY, pt_rm_reg_mc_nfy);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_rm_reg_mc_nfy->i4_ret;
    }
    ssma_free(pt_rm_reg_mc_nfy);

    x_sema_unlock (h_rm_cb_sema);
    return i4_ret;
}


INT32 x_rm_get_num_comps_of_type_from_name (const CHAR*  ps_name,
                                            DRV_TYPE_T   e_type,
                                            UINT16*      pui2_num_comps)
{
    PARAM_X_RM_GET_NUM_COMPS_OF_TYPE_FROM_NAME *pt_x_rm_get_num_comps_of_type_from_name;
    INT32 i4_ret;
    
    RM_DEBUG_INF("[MD:ResMgr] x_rm_get_num_comps_of_type_from_name\n\n");

    pt_x_rm_get_num_comps_of_type_from_name = (PARAM_X_RM_GET_NUM_COMPS_OF_TYPE_FROM_NAME *)ssma_alloc(sizeof(PARAM_X_RM_GET_NUM_COMPS_OF_TYPE_FROM_NAME));
    if (pt_x_rm_get_num_comps_of_type_from_name == NULL)
    {
        return -1;
    }
    pt_x_rm_get_num_comps_of_type_from_name->ps_name = (CHAR*)ps_name;
    pt_x_rm_get_num_comps_of_type_from_name->e_type = e_type;
    pt_x_rm_get_num_comps_of_type_from_name->pui2_num_comps = pui2_num_comps;
    i4_ret = ADPT_IOCTL(IOCTL_X_RM_GET_NUM_COMPS_OF_TYPE_FROM_NAME, pt_x_rm_get_num_comps_of_type_from_name);
    if (i4_ret >= 0) /* ioctl successfully */
    {
        i4_ret = pt_x_rm_get_num_comps_of_type_from_name->i4_ret;
    }
    ssma_free(pt_x_rm_get_num_comps_of_type_from_name);

    return i4_ret;
}

/* MHF linux - hc jiang del 
#define IOCTL_CRIT_START 0x8001
#define IOCTL_CRIT_END 0x8002
#define IOCTL_MSLEEP     0x8003
#define IOCTL_THREAD_END 0x8004 */

/* moved to dtv_common, thread_linux_drv.c
 * MHF Linux, -lzhang, Jun/30/2009 
 */
/*
void ioctl_msleep(UINT32 sleep_time)
{
   ADPT_IOCTL(IOCTL_MSLEEP, sleep_time);
}

void ioctl_thread_end()
{
   ADPT_IOCTL(IOCTL_THREAD_END, 0);
}
*/

