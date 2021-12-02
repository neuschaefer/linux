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
 * $RCSfile: _sm_sess_gnrc_dmx.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/10 $
 * $SWAuthor: Ivan Wei $
 * $MD5HEX: 9651a12385e0d96d6be213500a0475f8 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_drv_comp.h"
#include "strm_mngr/sess_gnrc_hdlr/sm_sess_gnrc.h"
#include "strm_mngr/sess_gnrc_hdlr/_sm_sess_gnrc_dmx.h"

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
 * Name: _sm_sess_gnrc_dmx_rm_nfy_fct
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
static VOID _sm_sess_gnrc_dmx_rm_nfy_fct( 
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data )
{
    SM_SESS_GNRC_MSG_T     t_msg;
    
    if ( pv_tag == NULL )
    {
        return;
    }
    
    t_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_DMX;
    t_msg.h_sess = (HANDLE_T)pv_tag;
    
    t_msg.t_dmx_msg.h_comp = h_obj;
    t_msg.t_dmx_msg.ui1_port = ui1_port;
    t_msg.t_dmx_msg.e_nfy_cond = e_nfy_cond;
    t_msg.t_dmx_msg.ui4_data = ui4_data;
    t_msg.t_dmx_msg.b_should_update_comp_descr = TRUE;
    
    sm_gs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_dmx_open
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
INT32 sm_sess_gnrc_dmx_open( 
    HANDLE_T        h_sess, 
    SM_SESS_GNRC_T* pt_sess_obj )
{
    INT32                  i4_ret;
    RM_COND_T              e_cond;
    
    x_strcpy( pt_sess_obj->t_dmx_descr.t_info.s_logical_name, 
              pt_sess_obj->t_base.s_dest_grp_name );
    
    pt_sess_obj->t_dmx_descr.t_info.e_type = DRVT_DEMUX_CONTROL;
    i4_ret = sm_drv_comp_open( NULL_HANDLE,
                               pt_sess_obj->t_dmx_descr.t_info.e_type,
                               ANY_COMP_ID,
                               NULL,
                               NULL,
                               sm_drv_comp_sel_unused_comp_fct,
                               (VOID*)(h_sess),
                               _sm_sess_gnrc_dmx_rm_nfy_fct,
                               &pt_sess_obj->t_dmx_descr );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;   
    }

    e_cond = sm_rm_cond_filter( pt_sess_obj->t_dmx_descr.e_comp_cond,
                                COND_GRP_OBJ_STATE );
    if ( e_cond == RM_COND_OPENED )
    {
        SM_SESS_GNRC_MSG_T     t_msg;
        
        t_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_DMX;
        t_msg.h_sess = h_sess;
        
        t_msg.t_dmx_msg.h_comp = pt_sess_obj->t_dmx_descr.h_comp;
        t_msg.t_dmx_msg.ui1_port = pt_sess_obj->t_dmx_descr.t_info.ui1_in_port;
        t_msg.t_dmx_msg.e_nfy_cond = pt_sess_obj->t_dmx_descr.e_comp_cond;
        t_msg.t_dmx_msg.ui4_data = 0;
        t_msg.t_dmx_msg.b_should_update_comp_descr = FALSE;
        
        sm_gs_send_msg( &t_msg );
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_dmx_close
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          pt_sess_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          i4_ret              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_dmx_close( 
    HANDLE_T        h_sess, 
    SM_SESS_GNRC_T* pt_sess_obj )
{
    INT32                  i4_ret = SMR_OK;
    RM_COND_T              e_cond;
    
    if ( pt_sess_obj->t_dmx_descr.h_comp != NULL_HANDLE &&
         pt_sess_obj->t_dmx_descr.e_comp_cond != RM_COND_CLOSED )
    {
        i4_ret = sm_drv_comp_close( &pt_sess_obj->t_dmx_descr );
        if ( i4_ret != RMR_OK )
        {
        }
    }
    else
    {
        /* Pretend closed, if handle is not existed */
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_DMX_CLOSED,
            h_sess,
            0 );
    }
    
    e_cond = sm_rm_cond_filter( pt_sess_obj->t_dmx_descr.e_comp_cond,
                                COND_GRP_OBJ_STATE );
    if ( e_cond == RM_COND_CLOSED )
    {
        SM_SESS_GNRC_MSG_T     t_msg;
        
        t_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_DMX;
        t_msg.h_sess = h_sess;
        
        t_msg.t_dmx_msg.h_comp = pt_sess_obj->t_dmx_descr.h_comp;
        t_msg.t_dmx_msg.ui1_port = pt_sess_obj->t_dmx_descr.t_info.ui1_in_port;
        t_msg.t_dmx_msg.e_nfy_cond = pt_sess_obj->t_dmx_descr.e_comp_cond;
        t_msg.t_dmx_msg.ui4_data = 0;
        t_msg.t_dmx_msg.b_should_update_comp_descr = FALSE;
        
        sm_gs_send_msg( &t_msg );
    }
        
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_dmx_set_mode
 *
 * Description: <function description>
 *
 * Inputs:  pt_sess_obj References.
 *          e_dmx_mode  References.
 *
 * Outputs: - 
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          i4_ret              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_dmx_set_mode( 
    SM_SESS_GNRC_T* pt_sess_obj,
    DEMUX_MODE_T    e_dmx_mode )
{
    INT32                  i4_ret;
    
    if ( pt_sess_obj->t_dmx_descr.h_comp != NULL_HANDLE &&
         pt_sess_obj->t_dmx_descr.e_comp_cond != RM_COND_CLOSED )
    {
        i4_ret = rm_set( pt_sess_obj->t_dmx_descr.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         DEMUX_SET_TYPE_DMX_MODE,
                         (VOID*)(UINT32)e_dmx_mode,
                         sizeof(DEMUX_MODE_T),
                         &pt_sess_obj->t_dmx_descr.e_comp_cond );
        if ( i4_ret != RMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
    }

    return SMR_OK;
}

