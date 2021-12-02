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
 * $RCSfile: _sm_bypass_comp_vout.c,v $
 * $Revision: #2 $
 * $Date: 2012/11/03 $
 * $Author: xingju.wang $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/13 $
 * $SWAuthor: Ivan Wei $
 * $MD5HEX: 8d38e18bf00ed38b2cac204e3b835948 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/

#ifdef MW_TV_AV_BYPASS_SUPPORT

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_video_out.h"

#include "strm_mngr/bypass_hdlr/_sm_bypass_hdlr.h"
#include "strm_mngr/bypass_hdlr/_sm_bypass_comp_vout.h"
#include "strm_mngr/sm_drv_map.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

static SELECT_T _bypass_rm_comp_sel_fct_vout( DRV_TYPE_T     e_type,
                                              UINT16         ui2_id,
                                              VOID*          pv_comp_sel_data,
                                              const VOID*    pv_comp_data,
                                              SIZE_T         z_comp_data_len,
                                              UINT16         ui2_num_active,
                                              UINT16         ui2_num_wait,
                                              CTRL_TYPE_T    e_ctrl,
                                              BOOL           b_would_conflict );
     
static VOID _bypass_rm_nfy_fct_vout( HANDLE_T  h_obj,
                                     UINT8     ui1_port,
                                     RM_COND_T e_nfy_cond,
                                     VOID*     pv_tag,
                                     UINT32    ui4_data );

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _bypass_rm_comp_sel_fct_vout
 *
 * Description: <function description>
 *
 * Inputs:  e_type           References.
 *          ui2_id           References.
 *          pv_comp_sel_data References.
 *          pv_comp_data     References.
 *          z_comp_data_len  References.
 *          b_in_use         References.
 *
 * Outputs: - 
 *
 * Returns: SEL_COMP_BREAK  Success.
 *
 ----------------------------------------------------------------------------*/
static SELECT_T _bypass_rm_comp_sel_fct_vout( DRV_TYPE_T     e_type,
                                              UINT16         ui2_id,
                                              VOID*          pv_comp_sel_data,
                                              const VOID*    pv_comp_data,
                                              SIZE_T         z_comp_data_len,
                                              UINT16         ui2_num_active,
                                              UINT16         ui2_num_wait,
                                              CTRL_TYPE_T    e_ctrl,
                                              BOOL           b_would_conflict )
{
    SM_DRV_COMP_INFO_T    t_drv_descr = SM_DRV_COMP_INFO_INIT;
    
    t_drv_descr.e_type = e_type;
    t_drv_descr.ui2_comp_id = ui2_id;

    if ( sm_drv_map_exists( &t_drv_descr ) || b_would_conflict )
    {
        return SEL_NEXT;
    }
    
    return SEL_COMP_BREAK;
}

/*-----------------------------------------------------------------------------
 * Name: _bypass_rm_nfy_fct_vout
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
static VOID _bypass_rm_nfy_fct_vout( HANDLE_T     h_obj,
                                     UINT8        ui1_port,
                                     RM_COND_T    e_nfy_cond,
                                     VOID*        pv_tag,
                                     UINT32       ui4_data )
{
    SM_BYPASS_HDLR_MSG_T    t_msg;
    HANDLE_T                h_stream = (HANDLE_T)pv_tag;
    RM_COND_T               e_cond   = RM_COND_IGNORE;
    
    if( !x_handle_valid(h_stream))
    {
        SM_BYPASS_DBG_ERR(("[SM] Bypass, _bypass_rm_nfy_fct_vout(), h_stream(0x%.8X) is invalid!\r\n",
                            h_stream ));
        return;
    }
    
    /* Return before send message to queue, because not handle the OPERATION info in sm_slctr */
    e_cond = rm_cond_chg( e_nfy_cond,
                          RM_COND_IGNORE,
                          COND_GRP_OPERATION );
    if( e_cond != RM_COND_NO_CHG ) 
    {
        return;
    }
    
    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_BYPASS_HDLR_MSG_VOUT_RM_NFY;
    
    t_msg.u.t_msg_rm.h_comp = h_obj;
    t_msg.u.t_msg_rm.ui1_port = ui1_port;
    t_msg.u.t_msg_rm.e_cond = e_nfy_cond;
    t_msg.u.t_msg_rm.ui4_data = ui4_data;
    t_msg.u.t_msg_rm.b_should_update_comp_descr = TRUE;
    
    sm_bypass_hdlr_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_comp_vout_open
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          h_source      References.
 *          pt_comp_descr References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_comp_vout_open( HANDLE_T                h_stream,
                                HANDLE_T                h_source,
                                SM_DRV_COMP_DESCR_T*    pt_comp_descr )
{
    INT32    i4_ret;
    
    i4_ret = sm_drv_comp_open( h_source,
                               pt_comp_descr->t_info.e_type,
                               pt_comp_descr->t_info.ui2_comp_id,
                               pt_comp_descr->t_info.s_logical_name,
                               (VOID*)pt_comp_descr,
                               &_bypass_rm_comp_sel_fct_vout,
                               (VOID*)h_stream,
                               &_bypass_rm_nfy_fct_vout,
                               pt_comp_descr );
    if ( i4_ret != SMR_OK || pt_comp_descr->h_comp == NULL_HANDLE )
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }
    
    /* Send RM notify message */
    {
        SM_BYPASS_HDLR_MSG_T    t_msg;
                
        t_msg.h_stream = h_stream;
        t_msg.e_msg_type = SM_BYPASS_HDLR_MSG_VOUT_RM_NFY;
        
        t_msg.u.t_msg_rm.h_comp = pt_comp_descr->h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_comp_descr->t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_cond = pt_comp_descr->e_comp_cond;
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_bypass_hdlr_send_msg( &t_msg );
    }
    
    return SMR_OK;
}

#endif /* MW_TV_AV_BYPASS_SUPPORT */

