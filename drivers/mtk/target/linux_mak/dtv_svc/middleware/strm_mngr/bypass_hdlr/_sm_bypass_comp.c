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
 * $RCSfile: _sm_bypass_comp.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/13 $
 * $SWAuthor: Ivan Wei $
 * $MD5HEX: 6675f7c47e43ed7293c6622b95e16f85 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/

#ifdef MW_TV_AV_BYPASS_SUPPORT

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "strm_mngr/x_sm.h"

#include "strm_mngr/bypass_hdlr/_sm_bypass_hdlr.h"
#include "strm_mngr/bypass_hdlr/_sm_bypass_comp_tvd.h"
#include "strm_mngr/bypass_hdlr/_sm_bypass_comp_vd.h"
#include "strm_mngr/bypass_hdlr/_sm_bypass_comp_tve.h"
#include "strm_mngr/bypass_hdlr/_sm_bypass_comp_vout.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

const SM_BYPASS_COMP_GRP_T GT_SM_BYPASS_COMP_GRP_INITOR = {
    SM_DRV_COMP_DESCR_INIT,
    SM_DRV_COMP_DESCR_INIT,
    SM_DRV_COMP_DESCR_INIT,
    SM_BYPASS_COMP_GRP_COND_IGNORE,
    SM_BYPASS_COMP_GRP_STATUS_IGNORE
};

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_comp_grp_close
 *
 * Description: <function description>
 *
 * Inputs: h_stream    References.
 *         pt_comp_grp References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_comp_grp_close( 
    HANDLE_T                 h_stream,
    SM_BYPASS_COMP_GRP_T*    pt_comp_grp )
{
    INT32 i4_ret;
    
    /* close Decoder */
    if ( x_handle_valid(pt_comp_grp->t_dec.h_comp) == TRUE )
    {
        i4_ret = sm_drv_comp_close( &pt_comp_grp->t_dec );
        if ( i4_ret != SMR_OK )
        {
            SM_BYPASS_DBG_ERR(( "[SM] BYPASS: sm_drv_comp_close(DEC) FAIL return %d\r\n",
                                i4_ret ));
        }
    }
    else
    {
        pt_comp_grp->t_dec.e_comp_cond = RM_COND_CLOSED;
    }

    /* Send RM notify message */
    {
        SM_BYPASS_HDLR_MSG_T    t_msg;
                
        t_msg.h_stream = h_stream;
        t_msg.e_msg_type = SM_BYPASS_HDLR_MSG_DEC_RM_NFY;
        
        t_msg.u.t_msg_rm.h_comp = pt_comp_grp->t_dec.h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_comp_grp->t_dec.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_cond = pt_comp_grp->t_dec.e_comp_cond;
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_bypass_hdlr_send_msg( &t_msg );
    }
    
    /* close TV Encoder */
    if ( x_handle_valid(pt_comp_grp->t_tve.h_comp) == TRUE )
    {
        i4_ret = sm_drv_comp_close( &pt_comp_grp->t_tve );
        if ( i4_ret != SMR_OK )
        {
            SM_BYPASS_DBG_ERR(( "[SM] BYPASS: sm_drv_comp_close(TVE) FAIL return %d\r\n",
                                i4_ret ));
        }
    }
    else
    {
        pt_comp_grp->t_tve.e_comp_cond = RM_COND_CLOSED;
    }

    /* Send RM notify message */
    {
        SM_BYPASS_HDLR_MSG_T    t_msg;
                
        t_msg.h_stream = h_stream;
        t_msg.e_msg_type = SM_BYPASS_HDLR_MSG_TVE_RM_NFY;
        
        t_msg.u.t_msg_rm.h_comp = pt_comp_grp->t_tve.h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_comp_grp->t_tve.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_cond = pt_comp_grp->t_tve.e_comp_cond;
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_bypass_hdlr_send_msg( &t_msg );
    }

    /* close Video Out */
    if ( x_handle_valid(pt_comp_grp->t_vout.h_comp) == TRUE )
    {
        i4_ret = sm_drv_comp_close( &pt_comp_grp->t_vout );
        if ( i4_ret != SMR_OK )
        {
            SM_BYPASS_DBG_ERR(( "[SM] BYPASS: sm_drv_comp_close(VOUT) FAIL return %d\r\n",
                                i4_ret ));
        }
    }
    else
    {
        pt_comp_grp->t_tve.e_comp_cond = RM_COND_CLOSED;
    }

    /* Send RM notify message */
    {
        SM_BYPASS_HDLR_MSG_T    t_msg;
                
        t_msg.h_stream = h_stream;
        t_msg.e_msg_type = SM_BYPASS_HDLR_MSG_VOUT_RM_NFY;
        
        t_msg.u.t_msg_rm.h_comp = pt_comp_grp->t_vout.h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_comp_grp->t_vout.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_cond = pt_comp_grp->t_vout.e_comp_cond;
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_bypass_hdlr_send_msg( &t_msg );
    }    

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_comp_grp_connect
 *
 * Description: <function description>
 *
 * Inputs:  h_stream    References.
 *          pt_comp_grp References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_comp_grp_connect( HANDLE_T                 h_stream,
                                  SM_BYPASS_COMP_GRP_T*    pt_comp_grp )
{
    INT32       i4_ret;
    
    /*************************************************************************
        !!!CAUTION!!! connection all drivers individually,
        Better use auto connect feature of RM.
    *************************************************************************/
    
    /* connect Video Out to TV Encoder */
    i4_ret = sm_drv_comp_connect( &pt_comp_grp->t_vout, 
                                  pt_comp_grp->t_tve.h_comp,
                                  (VOID*)h_stream );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;    
    }

    /* Send RM notify message */
    {
        SM_BYPASS_HDLR_MSG_T    t_msg;
                
        t_msg.h_stream = h_stream;
        t_msg.e_msg_type = SM_BYPASS_HDLR_MSG_VOUT_RM_NFY;
        
        t_msg.u.t_msg_rm.h_comp = pt_comp_grp->t_vout.h_comp;
        
        t_msg.u.t_msg_rm.ui1_port = pt_comp_grp->t_vout.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_cond = ( ( pt_comp_grp->t_vout.t_info.ui1_in_port == ANY_PORT_NUM )?
                                    pt_comp_grp->t_vout.e_comp_cond:
                                    pt_comp_grp->t_vout.e_comp_port_cond );
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_bypass_hdlr_send_msg( &t_msg );
    }
    
    /* connect TV Encoder to Decoder */
    i4_ret = sm_drv_comp_connect( &pt_comp_grp->t_tve, 
                                  pt_comp_grp->t_dec.h_comp,
                                  (VOID*)h_stream );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;    
    }

    /* Send RM notify message */
    {
        SM_BYPASS_HDLR_MSG_T    t_msg;
                
        t_msg.h_stream = h_stream;
        t_msg.e_msg_type = SM_BYPASS_HDLR_MSG_TVE_RM_NFY;
        
        t_msg.u.t_msg_rm.h_comp = pt_comp_grp->t_tve.h_comp;
        
        t_msg.u.t_msg_rm.ui1_port = pt_comp_grp->t_tve.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_cond = ( ( pt_comp_grp->t_tve.t_info.ui1_in_port == ANY_PORT_NUM )?
                                    pt_comp_grp->t_tve.e_comp_cond:
                                    pt_comp_grp->t_tve.e_comp_port_cond );
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_bypass_hdlr_send_msg( &t_msg );
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_comp_grp_disconnect
 *
 * Description: <function description>
 *
 * Inputs:  h_stream    References.
 *          pt_comp_grp References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_comp_grp_disconnect( 
    HANDLE_T                 h_stream,
    SM_BYPASS_COMP_GRP_T*    pt_comp_grp )
{
    INT32    i4_ret;
    
    /* disconnect Video Out to TV Encoder */
    i4_ret = sm_drv_comp_disconnect( &pt_comp_grp->t_vout );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;    
    }

    /* Send RM notify message */
    {
        SM_BYPASS_HDLR_MSG_T    t_msg;
                
        t_msg.h_stream = h_stream;
        t_msg.e_msg_type = SM_BYPASS_HDLR_MSG_VOUT_RM_NFY;
        
        t_msg.u.t_msg_rm.h_comp = pt_comp_grp->t_vout.h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_comp_grp->t_vout.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_cond = ( ( pt_comp_grp->t_vout.t_info.ui1_in_port == ANY_PORT_NUM )?
                                    pt_comp_grp->t_vout.e_comp_cond:
                                    pt_comp_grp->t_vout.e_comp_port_cond );
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_bypass_hdlr_send_msg( &t_msg );
    }
    
    /* disconnect TV Encoder to Decoder */
    i4_ret = sm_drv_comp_disconnect( &pt_comp_grp->t_tve );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;    
    }
    
    /* Send RM notify message */
    {
        SM_BYPASS_HDLR_MSG_T    t_msg;
                
        t_msg.h_stream = h_stream;
        t_msg.e_msg_type = SM_BYPASS_HDLR_MSG_TVE_RM_NFY;
        
        t_msg.u.t_msg_rm.h_comp = pt_comp_grp->t_tve.h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_comp_grp->t_tve.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_cond = ( ( pt_comp_grp->t_tve.t_info.ui1_in_port == ANY_PORT_NUM )?
                                    pt_comp_grp->t_tve.e_comp_cond:
                                    pt_comp_grp->t_tve.e_comp_port_cond );
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_bypass_hdlr_send_msg( &t_msg );
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_comp_grp_open
 *
 * Description: <function description>
 *
 * Inputs:  h_stream       References.
 *          h_source       References.
 *          pt_vs_comp_grp References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_comp_grp_open( HANDLE_T                 h_stream,
                               HANDLE_T                 h_source,
                               SM_BYPASS_COMP_GRP_T*    pt_comp_grp )
{
    INT32 i4_ret;
    
    if ( x_handle_valid(pt_comp_grp->t_dec.h_comp) == FALSE )
    {
        /* open the component */
        if ( pt_comp_grp->t_dec.t_info.e_type == DRVT_VID_DEC )
        {
            i4_ret = sm_bypass_comp_vd_open( h_stream,
                                             h_source,
                                             &pt_comp_grp->t_dec );
        }
        else if ( pt_comp_grp->t_dec.t_info.e_type == DRVT_TV_DEC )
        {
            i4_ret = sm_bypass_comp_tvd_open( h_stream,
                                              h_source,
                                              &pt_comp_grp->t_dec );
        }
        else
        {
            return SMR_INTERNAL_ERROR;
        }

        if ( i4_ret != SMR_OK )
        {
            return i4_ret;    
        }
    }
    
    if ( x_handle_valid(pt_comp_grp->t_tve.h_comp) == FALSE )
    {
        /* open the component */
        i4_ret = sm_bypass_comp_tve_open( h_stream,
                                          h_source,
                                          &pt_comp_grp->t_tve );
        if ( i4_ret != SMR_OK )
        {
            sm_bypass_comp_grp_close( h_stream, pt_comp_grp );
            return i4_ret;    
        }
    }

    if ( x_handle_valid(pt_comp_grp->t_vout.h_comp) == FALSE )
    {
        /* open the component */
        i4_ret = sm_bypass_comp_vout_open( h_stream,
                                           h_source,
                                           &pt_comp_grp->t_vout );
        if ( i4_ret != SMR_OK )
        {
            sm_bypass_comp_grp_close( h_stream, pt_comp_grp );
            return i4_ret;    
        }    
    }

    return SMR_OK;
}

#endif /* MW_TV_AV_BYPASS_SUPPORT */

