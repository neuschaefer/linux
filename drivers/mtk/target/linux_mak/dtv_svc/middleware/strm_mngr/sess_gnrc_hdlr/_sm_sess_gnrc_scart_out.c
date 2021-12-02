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
 * $RCSfile: _sm_sess_gnrc_scart_out.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/10 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 9651a12385e0d96d6be213500a0475f8 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_demux.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_drv_comp.h"
#include "strm_mngr/sess_gnrc_hdlr/sm_sess_gnrc.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _sm_sess_gnrc_scart_rm_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  h_obj      References.
 *          ui1_port   References.
 *          e_nfy_cond References.
 *          pv_tag     References.
 *          ui4_data   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_sess_gnrc_scart_rm_nfy_fct( 
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data )
{
    if ( pv_tag == NULL )
    {
        return;
    }
   
    return;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_drv_comp_sel_scart_comp_fct
 *
 * Description: <function description>
 *
 * Inputs:  e_type              References.
 *          ui2_id              References.
 *          pv_comp_sel_data    References.
 *          pv_comp_data        References.
 *          z_comp_data_len     References.
 *          ui2_num_active      References.
 *          ui2_num_wait        References.
 *          e_ctrl              References.
 *          b_would_conflict    References.    
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static SELECT_T _sm_drv_comp_sel_scart_comp_fct(DRV_TYPE_T   e_type,
                                                UINT16       ui2_id,
                                                VOID*        pv_comp_sel_data,
                                                const VOID*  pv_comp_data,
                                                SIZE_T       z_comp_data_len,
                                                UINT16       ui2_num_active,
                                                UINT16       ui2_num_wait,
                                                CTRL_TYPE_T  e_ctrl,
                                                BOOL         b_would_conflict)
{
    INT32       i4_res;    
    SELECT_T    e_select;
    UINT32      ui4_comp_flags;
    UINT8       ui1_num_inp_ports;
    UINT8       ui1_num_out_ports;
    BOOL        b_ignore;
    
    i4_res = rm_get_comp_info_by_id(e_type,
                                    ui2_id,
                                    &ui1_num_inp_ports,
                                    &ui1_num_out_ports,
                                    &ui4_comp_flags);
    
    b_ignore = (i4_res != RMR_OK) || ((ui4_comp_flags & AVC_FLAG_SCART_SINK) != AVC_FLAG_SCART_SINK);
    
    if (!b_ignore)
    {
        if (!b_would_conflict)
        {
            if ((ui2_num_active == 0) && (ui2_num_wait == 0))
            {
                e_select = SEL_COMP_BREAK;
            }
            else
            {
                e_select = SEL_COMP_NEXT;
            }
        }        
        else
        {
            e_select = SEL_COMP_NEXT;
        }            
    }
    else
    {
        e_select = SEL_NEXT;
    }
    
    return e_select;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_scart_open_by_name
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          pt_sess_obj References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static SM_SESS_GNRC_SCART_INFO_T* sm_sess_gnrc_scart_open_by_name( 
    HANDLE_T                    h_sess, 
    SM_SESS_GNRC_T*             pt_sess_obj,
    CHAR*                       ps_scart_name)
{
    INT32                       i4_ret;
    SM_SESS_GNRC_SCART_INFO_T*  pt_scart;

    pt_scart = (SM_SESS_GNRC_SCART_INFO_T*)x_mem_alloc(
                                            sizeof(SM_SESS_GNRC_SCART_INFO_T));
    if (!pt_scart)
    {
        return NULL;
    }
    pt_scart->t_scart_descr             = t_sm_drv_comp_initor;
    pt_scart->t_scart_out.t_dev_type    = DRVT_UNKNOWN;
    pt_scart->t_scart_out.ui2_id        = 0;
    
    pt_scart->t_scart_descr.t_info.e_type = DRVT_AVC_SCART;

    x_strncpy(pt_scart->t_scart_descr.t_info.s_logical_name,
              ps_scart_name,
              sizeof(pt_scart->t_scart_descr.t_info.s_logical_name));

    i4_ret = sm_drv_comp_open(pt_sess_obj->h_source,
                              pt_scart->t_scart_descr.t_info.e_type,
                              ANY_COMP_ID,
                              ps_scart_name,
                              /*pt_sess_obj->t_base.s_dest_grp_name,*/
                              NULL,
                              _sm_drv_comp_sel_scart_comp_fct,
                              (VOID*)(h_sess),
                              _sm_sess_gnrc_scart_rm_nfy_fct,
                              &pt_scart->t_scart_descr);
    if (i4_ret != SMR_OK)
    {
        x_mem_free(pt_scart);
        return NULL;   
    }

    return pt_scart;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_scart_enable_by_id
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          pt_sess_obj References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static SM_SESS_GNRC_SCART_INFO_T* sm_sess_gnrc_scart_open_by_id( 
    HANDLE_T                    h_sess, 
    SM_SESS_GNRC_T*             pt_sess_obj,
    UINT16                      ui2_comp_id)
{
    INT32                       i4_ret;
    SM_SESS_GNRC_SCART_INFO_T*  pt_scart;

    pt_scart = (SM_SESS_GNRC_SCART_INFO_T*)x_mem_alloc(
                                            sizeof(SM_SESS_GNRC_SCART_INFO_T));
    if (!pt_scart)
    {
        return NULL;
    }
    pt_scart->t_scart_descr     = t_sm_drv_comp_initor;
    pt_scart->t_scart_out.t_dev_type    = DRVT_UNKNOWN;
    pt_scart->t_scart_out.ui2_id        = 0;
    
    pt_scart->t_scart_descr.t_info.e_type = DRVT_AVC_SCART;

    i4_ret = sm_drv_comp_open(pt_sess_obj->h_source,
                              pt_scart->t_scart_descr.t_info.e_type,
                              ui2_comp_id,
                              NULL,
                              /*pt_sess_obj->t_base.s_dest_grp_name,*/
                              NULL,
                              _sm_drv_comp_sel_scart_comp_fct,
                              (VOID*)(h_sess),
                              _sm_sess_gnrc_scart_rm_nfy_fct,
                              &pt_scart->t_scart_descr);
    if (i4_ret != SMR_OK)
    {
        x_mem_free(pt_scart);
        return NULL;   
    }

    return pt_scart;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_scart_out_enable
 *
 * Description: <function description>
 *
 * Inputs:  pt_sess_obj References.
 *          h_stream    References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 sm_sess_gnrc_scart_out_enable( 
    HANDLE_T                    h_sess,
    SM_SESS_GNRC_T*             pt_sess_obj,
    SM_SESS_GNRC_SCART_OUT_T*   pt_scart_info)
{
    INT32                       i4_ret;
    SM_SESS_GNRC_SCART_INFO_T*  pt_scart;
    UINT8                       ui1_num_inp_ports;
    UINT8                       ui1_num_out_ports;
    UINT32                      ui4_comp_flags;

    if (pt_scart_info->b_set_by_name)        
    {
        if (NULL == pt_scart_info->u.s_scart_name)
        {
            return SMR_DRV_COMP_ERROR;
        }

        /* check if scart is opened or not */
        pt_scart = NULL;
        SLIST_FOR_EACH(pt_scart, &pt_sess_obj->t_scart_list, t_link)
        {
            if (0 == x_strcmp(pt_scart_info->u.s_scart_name,
                              pt_scart->t_scart_descr.t_info.s_logical_name))
            {
                break;
            }
        }

        if (NULL == pt_scart)
        {
            /* enable specified scart output */
            pt_scart = sm_sess_gnrc_scart_open_by_name(
                                h_sess,
                                pt_sess_obj,
                                pt_scart_info->u.s_scart_name);
            if (NULL == pt_scart)
            {
                return SMR_DRV_COMP_ERROR;
            }

            i4_ret = sm_drv_comp_connect(&pt_scart->t_scart_descr,
                                         pt_sess_obj->h_source,
                                         (VOID*)h_sess);
            if (i4_ret != SMR_OK)
            {
                x_mem_free(pt_scart);
                return i4_ret;
            }

            SLIST_INSERT_HEAD(pt_scart, &pt_sess_obj->t_scart_list, t_link);
        }
    }
    else
    {
        /* enable specified scart output */
        pt_scart = NULL;
        SLIST_FOR_EACH(pt_scart, &pt_sess_obj->t_scart_list, t_link)
        {
            if (pt_scart_info->u.ui2_comp_id == 
                pt_scart->t_scart_descr.t_info.ui2_comp_id)
            {
                break;
            }
        }

        if (NULL == pt_scart)
        {
            pt_scart = sm_sess_gnrc_scart_open_by_id(
                                h_sess,
                                pt_sess_obj,
                                pt_scart_info->u.ui2_comp_id);
            if (NULL == pt_scart)
            {
                return SMR_DRV_COMP_ERROR;
            }

            i4_ret = sm_drv_comp_connect(&pt_scart->t_scart_descr,
                                         pt_sess_obj->h_source,
                                         (VOID*)h_sess);
            if (i4_ret != SMR_OK)
            {
                x_mem_free(pt_scart);
                return i4_ret;
            }

            SLIST_INSERT_HEAD(pt_scart, &pt_sess_obj->t_scart_list, t_link);
        }
    }

    /* TODO: set enable type here */
    i4_ret = rm_get_comp_info_from_handle(pt_sess_obj->h_source,
                                          &pt_scart->t_scart_out.t_dev_type,
                                          &pt_scart->t_scart_out.ui2_id,
                                          &ui1_num_inp_ports,
                                          &ui1_num_out_ports,
                                          &ui4_comp_flags);
    if (RMR_OK == i4_ret)
    {
        i4_ret = rm_set(pt_scart->t_scart_descr.h_comp,
                        FALSE,
                        ANY_PORT_NUM,
                        AVC_SET_TYPE_SCART_OUTPUT,
                        (VOID*)&pt_scart->t_scart_out,
                        sizeof(AVC_SCART_OUTPUT_T),
                        &pt_scart->t_scart_descr.e_comp_cond);
    }
    /*if ((i4_ret != RMR_OK) && (i4_ret != RMR_ASYNC_NFY))
    {
        return SMR_DRV_COMP_ERROR;    
    }*/
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_scart_disc_and_close
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          pt_sess_obj References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 sm_sess_gnrc_scart_disc_and_close( 
    SM_SESS_GNRC_SCART_INFO_T*  pt_scart)
{
    INT32                       i4_ret;

    pt_scart->t_scart_out.t_dev_type    = DRVT_UNKNOWN;
    pt_scart->t_scart_out.ui2_id        = 0;

    i4_ret = rm_set(pt_scart->t_scart_descr.h_comp,
                    FALSE,
                    ANY_PORT_NUM,
                    AVC_SET_TYPE_SCART_OUTPUT,
                    (VOID*)&pt_scart->t_scart_out,
                    sizeof(AVC_SCART_OUTPUT_T),
                    &pt_scart->t_scart_descr.e_comp_cond);
    if ((i4_ret != RMR_OK) && (i4_ret != RMR_ASYNC_NFY))
    {
        return SMR_DRV_COMP_ERROR;    
    }

    if ((pt_scart->t_scart_descr.h_comp != NULL_HANDLE) &&
        (pt_scart->t_scart_descr.e_comp_cond != RM_COND_CLOSED))
    {
        i4_ret = sm_drv_comp_disconnect(&pt_scart->t_scart_descr);
        if (i4_ret != SMR_OK)
        {
            return i4_ret;
        }
    }

    if ((pt_scart->t_scart_descr.h_comp != NULL_HANDLE) &&
        (pt_scart->t_scart_descr.e_comp_cond != RM_COND_CLOSED))
    {
        i4_ret = sm_drv_comp_close(&pt_scart->t_scart_descr);
        if (i4_ret != RMR_OK)
        {
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_scart_out_disable
 *
 * Description: <function description>
 *
 * Inputs:  pt_sess_obj References.
 *          h_stream    References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 sm_sess_gnrc_scart_out_disable( 
    HANDLE_T                    h_sess,
    SM_SESS_GNRC_T*             pt_sess_obj,
    SM_SESS_GNRC_SCART_OUT_T*   pt_scart_info)
{
    SM_SESS_GNRC_SCART_INFO_T*  pt_scart;
    INT32                       i4_ret;

    if (pt_scart_info->b_set_by_name)        
    {
        if (pt_scart_info->u.s_scart_name)
        {
            /* check if scart is opened or not */
            pt_scart = NULL;
            SLIST_FOR_EACH(pt_scart, &pt_sess_obj->t_scart_list, t_link)
            {
                if (0 == x_strcmp(pt_scart_info->u.s_scart_name,
                                  pt_scart->t_scart_descr.t_info.s_logical_name))
                {
                    break;
                }
            }

            if (NULL == pt_scart)
            {
                /* enable specified scart output */
                pt_scart = sm_sess_gnrc_scart_open_by_name(
                                    h_sess,
                                    pt_sess_obj,
                                    pt_scart_info->u.s_scart_name);
                if (NULL == pt_scart)
                {
                    return SMR_DRV_COMP_ERROR;
                }

                i4_ret = sm_drv_comp_connect(&pt_scart->t_scart_descr,
                                             pt_sess_obj->h_source,
                                             (VOID*)h_sess);
                if (i4_ret != SMR_OK)
                {
                    x_mem_free(pt_scart);
                    return i4_ret;
                }

                SLIST_INSERT_HEAD(pt_scart, &pt_sess_obj->t_scart_list, t_link);
            }
        }
        else
        {
            /* disable all scart output */
            SLIST_FOR_EACH(pt_scart, &pt_sess_obj->t_scart_list, t_link)
            {
                i4_ret = sm_sess_gnrc_scart_disc_and_close(pt_scart);
                if (i4_ret != SMR_OK)
                {
                }
            }

            while ((pt_scart = SLIST_FIRST(&pt_sess_obj->t_scart_list)) != NULL)
            {
                SLIST_REMOVE(pt_scart, t_link);
                x_mem_free(pt_scart);
            }

            return SMR_OK;
        }
    }
    else
    {
        /* disable scart by id */
        pt_scart = NULL;
        SLIST_FOR_EACH(pt_scart, &pt_sess_obj->t_scart_list, t_link)
        {
            if (pt_scart_info->u.ui2_comp_id == 
                pt_scart->t_scart_descr.t_info.ui2_comp_id)
            {
                break;
            }
        }

        if (NULL == pt_scart)
        {
            pt_scart = sm_sess_gnrc_scart_open_by_id(
                                h_sess,
                                pt_sess_obj,
                                pt_scart_info->u.ui2_comp_id);
            if (NULL == pt_scart)
            {
                return SMR_DRV_COMP_ERROR;
            }

            i4_ret = sm_drv_comp_connect(&pt_scart->t_scart_descr,
                                         pt_sess_obj->h_source,
                                         (VOID*)h_sess);
            if (i4_ret != SMR_OK)
            {
                x_mem_free(pt_scart);
                return i4_ret;
            }

            SLIST_INSERT_HEAD(pt_scart, &pt_sess_obj->t_scart_list, t_link);
        }
    }

    pt_scart->t_scart_out.t_dev_type    = DRVT_UNKNOWN;
    pt_scart->t_scart_out.ui2_id        = 0;

    i4_ret = rm_set(pt_scart->t_scart_descr.h_comp,
                    FALSE,
                    ANY_PORT_NUM,
                    AVC_SET_TYPE_SCART_OUTPUT,
                    (VOID*)&pt_scart->t_scart_out,
                    sizeof(AVC_SCART_OUTPUT_T),
                    &pt_scart->t_scart_descr.e_comp_cond);
    /*if ((i4_ret != RMR_OK) && (i4_ret != RMR_ASYNC_NFY))
    {
        return SMR_DRV_COMP_ERROR;    
    }*/

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_scart_out_set
 *
 * Description: <function description>
 *
 * Inputs:  pt_sess_obj References.
 *          h_stream    References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_scart_out_set( 
    HANDLE_T                    h_sess, 
    SM_SESS_GNRC_T*             pt_sess_obj,
    SM_SESS_GNRC_SCART_OUT_T*   pt_scart_info)
{
    if (pt_scart_info->b_enable)
    {
        /* enable scart */
        return sm_sess_gnrc_scart_out_enable(
                    h_sess,
                    pt_sess_obj,
                    pt_scart_info);
    }
    else
    {
        /* disable scart */       
        return sm_sess_gnrc_scart_out_disable(
                    h_sess,
                    pt_sess_obj,
                    pt_scart_info);
    }
}



