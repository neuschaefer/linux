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
 * $RCSfile: dsm_avc_combi.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains DSM function interfacing with the
 *         AVC (audio-video connect) COMBI driver.
 *
 *---------------------------------------------------------------------------*/

#include "dsm/_dsm.h"
#include "dsm/dsm_avc_combi.h"
#include "dsm/dsm_nfy.h"
#include "res_mngr/drv/x_tv_dec.h"

CHAR*  avc_combi_cond_info_txt(AVC_COND_T   t_avc_cond)
{
    CHAR*   ps_txt=NULL;
    
    switch ( t_avc_cond )
    {
        case AVC_COND_COMBI_SIGNAL_STATUS:
        {
            ps_txt="AVC_COND_COMBI_SIGNAL_STATUS";
        }
        break;

        case AVC_COND_COMBI_CABLE_STATUS:
        {
            ps_txt="AVC_COND_COMBI_CABLE_STATUS";
        }
        break;

        default:
        {
            ps_txt=NULL;
        }
        break;
    }
    return ps_txt;
}

/*--------------------------------------------------------------------------
 * Name:   avc_combi_drv_nfy_fct
 *
 * Description: This notification function called by AVC COMBI driver.
 *              This function will send a message to the DSM thread.
 *              The DSM thread will notify client's notication.              
 *
 * Input -
 *    pv_nfy_tag:   pointer to a DSM component structure for the
 *                  notification.
 *    e_nfy_cond:   The driver notification condition.
 *                   
 *    ui4_data_1    Contains the signal status value.
 *                       e.g.:  AVC_SIG_NONE
 *                              AVC_STATE_TV_SIGNAL
 *                              AVC_STATE_AUDIO_SIGNAL
 *
 *    ui4_data_2    Contain the index to the sub-component in the AVC
 *                  COMBI driver that signal status has changed.
 *
 * Output -
 *    None
 *
 * Return -
 *    VOID
 *   
 --------------------------------------------------------------------------*/
static  VOID  avc_combi_drv_nfy_fct
(
    VOID*               pv_nfy_tag,
    AVC_COND_T          e_nfy_cond,
    UINT32              ui4_data_1,
    UINT32              ui4_data_2
)
{
    DSM_MSG_T           t_msg;
    HANDLE_T            h_dsm_comp = NULL_HANDLE;

    if ( pv_nfy_tag != NULL )
    {
        h_dsm_comp = (HANDLE_T) pv_nfy_tag;

        if ( x_handle_valid(h_dsm_comp) == TRUE )
        {
            /*
              Send up signal change notification
            */
            if ( (e_nfy_cond == AVC_COND_COMBI_SIGNAL_STATUS) ||
                 (e_nfy_cond == AVC_COND_COMBI_CABLE_STATUS)
                )
            {
                
#ifdef DEBUG                
                CHAR*  ps_txt = avc_combi_cond_info_txt(e_nfy_cond);
                
                DBG_API(("{DSM} receive AVC Combi driver notification: %s data_1: %d data_2: %d\n", ps_txt, ui4_data_1, ui4_data_2));

#endif                
                /*  send msg to DSM thread. */
                t_msg.e_msg_code = DSM_MSG_AVC_COMBI_DRVT_NFY;
                t_msg.h_dsm_comp = h_dsm_comp ;
                t_msg.u.t_avc_combi.e_nfy_cond = e_nfy_cond;
                t_msg.u.t_avc_combi.ui4_data_1 = ui4_data_1;
                t_msg.u.t_avc_combi.ui4_data_2 = ui4_data_2;
                dsm_send_msg(&t_msg);
            }
            else
            {
                DBG_API(("{DSM} receive AVC combi driver notification: "
                         "[%d], this is ignored.\n", e_nfy_cond));
            }
        }
    }
}
/*--------------------------------------------------------------------------
 * Name: avc_rm_pipe_nfy_fct
 *
 * Description - Notification function for the RM pipe condition.
 *
 * Input  - h_obj:  handle to the RM pipe component
 *          ui1_port:  Not used by the AVC component. 
 *          e_nfy_cond:  Condition of RM pipe
 *          pv_tag:    Tag pass into Resource Manager when opening
 *                     this RM pipe.
 *          ui4_data:  Private data, not used by the AVC component.
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
static  VOID avc_rm_pipe_nfy_fct 
(
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data
)
{
    switch (e_nfy_cond)
    {
        case RM_COND_OPENED:
        {
            DBG_INFO(("{DSM}: pipe comp [AVC combi <-> TV decoder] [%d] opened.\n",h_obj));
        }
        break;
        
        case RM_COND_OPENING:
        {
            DBG_INFO(("{DSM}: pipe comp [AVC combi <-> TV decoder] [%d] opening.\n",h_obj));
        }
        break;
        
        case RM_COND_CLOSING:
        {
            DBG_INFO(("{DSM}: pipe comp [AVC combi <-> TV decoder] [%d] closing.\n",h_obj));
        }
        break;

        case RM_COND_CLOSED:
        {
            DBG_INFO(("{DSM}: pipe comp [AVC combi <-> TV decoder] [%d] closed.\n",h_obj));
        }
        break;


        case RM_COND_CONNECTING:
        {
            DBG_INFO(("{DSM}: pipe comp [AVC combi <-> TV decoder] [%d] connecting.\n",h_obj));
        }
        break;
        
        case RM_COND_CONNECTED:
        {
            DBG_INFO(("{DSM}: pipe comp [AVC combi <-> TV decoder] [%d] connected.\n",h_obj));
        }
        break;

        
        case RM_COND_DISCONNECTING:
        {
            DBG_INFO(("{DSM}: pipe comp [AVC combi <-> TV decoder] [%d] disconnecting.\n",h_obj));
        }
        break;
        
        case RM_COND_DISCONNECTED:
        {
            DBG_INFO(("{DSM}: pipe comp [AVC combi <-> TV decoder] [%d] disconnected.\n",h_obj));
        }
        break;
        
        default:
        {
            
        }
        break;
    }
    return ;
}

/*-----------------------------------------------------------------------------
 * Name: tv_decoder_drv_nfy
 *
 * Description: This API is called by the TV decoder driver to notify the
 *              changes in the state or condition of the TV decoder
 *              driver component.  This is API conforms to 
 *              (*x_tv_dec_nfy_fct).
 *
 * Inputs:  
 *    pv_nfy_tag  Contains a tag value that was specified in the 
 *                pv_tag field of the TV_DEC_NFY_INFO_T structure passed
 *                to TV decoder driver during set TV_DEC_SET_TYPE_NFY_FCT
 *                operation.
 *    e_nfy_cond  Contains the notify condition.
 *    ui4_data_1  Contains additional data for the notify condition.
 *    ui4_data_2  Contains additional data for the notify condition.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID tv_decoder_drv_nfy
(
    VOID*           	pv_nfy_tag,
    TV_DEC_COND_T       e_nfy_cond,
    UINT32          	ui4_data_1,
    UINT32			    ui4_data_2
)
{
    DSM_MSG_T          t_msg;
    
    t_msg.h_dsm_comp  = NULL_HANDLE;
    t_msg.e_msg_code  = DSM_MSG_TVD_DRV_NFY;    
    t_msg.e_cond      = (DSM_COND_DEV_T)0;

    t_msg.u.t_tvd_nfy.pv_nfy_tag = pv_nfy_tag;
    t_msg.u.t_tvd_nfy.e_nfy_cond = e_nfy_cond;
    t_msg.u.t_tvd_nfy.ui4_data_1 = ui4_data_1;
    t_msg.u.t_tvd_nfy.ui4_data_2 = ui4_data_2;

    dsm_send_msg( &t_msg );
}

VOID tv_decoder_drv_nfy_msg_hdlr
(
    VOID*           	pv_nfy_tag,
    TV_DEC_COND_T       e_nfy_cond,
    UINT32          	ui4_data_1,
    UINT32			    ui4_data_2
)
{
    HANDLE_T            h_dsm_comp;
    DSM_COMP_T*         pt_dsm_comp = NULL ;
    DSM_AVC_COMBI_T*    pt_avc_comp = NULL ;

    h_dsm_comp = (HANDLE_T) pv_nfy_tag;
    pt_dsm_comp = dsm_get_ref_to_comp(h_dsm_comp);
    if ( pt_dsm_comp == NULL )
    {
        return;
    }
    
    pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_comp->pv_private ;
    
    switch (e_nfy_cond)
    {
        case TV_DEC_COND_TV_SIGNAL:
        {
            if ( ((TV_DEC_TV_SIG_T)ui4_data_1) == TV_DEC_TV_SIG_STABLE )
            {
                DBG_INFO(("{DSM} tv_decoder driver: got TV signal. \n"));
            }
            else if ( ((TV_DEC_TV_SIG_T)ui4_data_1) == TV_DEC_TV_SIG_NONE )
            {
                DBG_INFO(("{DSM} tv_decoder driver: lost TV signal. \n"));
                pt_avc_comp->b_video_sig_set = FALSE;
                pt_avc_comp->b_audio_sig_set = FALSE;
            }
            else if ( ((TV_DEC_TV_SIG_T)ui4_data_1) == TV_DEC_TV_SIG_UNKNOWN )
            {
                DBG_INFO(("{DSM} tv decoder driver: state unknow. \n"));
                pt_avc_comp->b_video_sig_set = FALSE;
                pt_avc_comp->b_audio_sig_set = FALSE;
            }    
        }
        break;

        case TV_DEC_COND_ERROR:
        {
            if ( ui4_data_1 == TV_DEC_SET_TYPE_CTRL  &&
                 ui4_data_2 == TV_DEC_CTRL_PLAY )
            {
                /* Set 'control-play' on TV decoder failed. */
                DBG_INFO(("{DSM} tv_decoder driver: play command failed.\n"));
                pt_avc_comp->b_mon_enabled = FALSE;
            }
        }
        break;
            
        case TV_DEC_COND_CTRL_DONE:
        {
            DSM_MSG_T           t_msg;
    
            DBG_INFO(("{DSM} tv_decoder driver: CTRL_DONE nfy received \n"));
            if ( ui4_data_1 == TV_DEC_SET_TYPE_CTRL  &&
                 ui4_data_2 == TV_DEC_CTRL_PLAY )
            {
                /* Set 'control-play' on TV decoder succeeded. */
                DBG_INFO(("{DSM} tv_decoder driver: play started \n"));
                pt_avc_comp->b_mon_enabled = TRUE;

                /*
                   TV decoder connected and playing,
                   Notify the DSM client with signal status.
                
                   Send message to DSM notification thread
                   to notify client with the signal
                   status of AVC combi.
                */
                DBG_API(("{DSM} send msg to nfy thread with "
                         "DSM_MSG_TV_DECODER_PLAYED\n"));
            
                /*  send msg to DSM thread. */
                t_msg.e_msg_code = DSM_MSG_TV_DECODER_PLAYED;
                t_msg.h_dsm_comp = pt_dsm_comp->h_this_dsm_comp ;
                dsm_send_msg(&t_msg);
            } 
            if ( ui4_data_1 == TV_DEC_SET_TYPE_CTRL  &&
                 ui4_data_2 == TV_DEC_CTRL_STOP )
            {
                DBG_API(("{DSM} send msg to nfy thread with "
                         "DSM_MSG_TV_DECODER_STOPPED\n"));

                /*  send msg to DSM thread. */
                t_msg.e_msg_code = DSM_MSG_TV_DECODER_STOPPED;
                t_msg.h_dsm_comp = pt_dsm_comp->h_this_dsm_comp ;
                dsm_send_msg(&t_msg);
            }
        }
        break;

        case TV_DEC_COND_COLOR_SYS:
        {
            DBG_INFO(("{DSM} tv_decoder driver: color system nfy. \n"));
        }
        break;
            
        default:
        {
            DBG_INFO(("{DSM} tv_decoder driver: unknown notify cond [%d] received.\n",
                      e_nfy_cond)); 
        }
        break;
    } 
    return;
}

/*-----------------------------------------------------------------------------
 * Name: avc_rm_tv_decoder_nfy
 *
 * Description: This API is called by the Resource Manager to notify the
 *              handler of changes in the state or condition of the TV decoder
 *              driver component.
 *
 * Inputs:  h_comp      Contains a handle to the decoder driver component.
 *          ui1_port    Contains the port number.
 *          e_nfy_cond  Contains the notify condition.
 *          pv_tag      Contains the tag value specified in rm_open_comp.
 *          ui4_data    Contains additional data (depends on e_nfy_cond).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_rm_tv_decoder_nfy
(
    HANDLE_T   h_comp,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data
)
{
    DSM_MSG_T          t_msg;

    t_msg.h_dsm_comp  = NULL_HANDLE;
    t_msg.e_msg_code  = DSM_MSG_RM_TVD_NFY;    
    t_msg.e_cond      = (DSM_COND_DEV_T)0;

    t_msg.u.t_rm_tvd_nfy.h_comp = h_comp;
    t_msg.u.t_rm_tvd_nfy.ui1_port = ui1_port;
    t_msg.u.t_rm_tvd_nfy.e_nfy_cond = e_nfy_cond;
    t_msg.u.t_rm_tvd_nfy.pv_tag = pv_tag;
    t_msg.u.t_rm_tvd_nfy.ui4_data = ui4_data;

    dsm_send_msg( &t_msg );
}

VOID avc_rm_tv_decoder_nfy_msg_hdlr
(
    HANDLE_T   h_comp,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data
)
{
    RM_COND_T           e_rm_cond;
    RM_COND_T           e_old_cond;
    RM_COND_T           e_rm_share_cond;
    RM_COND_T           e_rm_conn_cond;
    RM_COND_T           e_rm_orig_cond;
    HANDLE_T            h_dsm_comp;
    DSM_COMP_T*         pt_dsm_comp ;
    DSM_AVC_COMBI_T*    pt_avc_comp;
    DSM_MSG_T           t_msg;

    h_dsm_comp = (HANDLE_T) pv_tag;
    pt_dsm_comp = dsm_get_ref_to_comp(h_dsm_comp);
    if ( pt_dsm_comp == NULL )
    {
        return ;
    }
    
    pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_comp->pv_private ;
    
    DBG_INFO(("{DSM} TV decoder RM notification (e_nfy_cond=%x)\n",
              e_nfy_cond));

    e_old_cond = pt_avc_comp->e_rm_tv_decoder_cond;
    pt_avc_comp->e_rm_tv_decoder_cond = e_nfy_cond;

    dsm_rm_cond_code(h_comp, e_nfy_cond);

    /*
      check if the share condition has been changed and current share
      condition type.
    */
    e_rm_cond = rm_cond_chg(e_nfy_cond,
                            e_old_cond,
                            COND_GRP_SHARE_STATE);

    e_rm_share_cond = rm_cond_chg(e_nfy_cond,
                                  RM_COND_IGNORE,
                                  COND_GRP_SHARE_STATE);
    /*
       If the condition has not changed, but the current and last
       share cond is RM_COND_SLAVE, then we need to check if the
       originator of the current and last connection has changed.
    */
    if ( e_rm_cond == RM_COND_NO_CHG         &&
         e_rm_share_cond == RM_COND_SLAVE )
    {
        DBG_INFO(("{DSM} last and curr cond is SLAVE.\n"));
        
        /*
          check if the originator has change between the last
          SLAVE condition.
        */
        if ( rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_CONN_ORIG)
             !=
             rm_cond_chg(e_old_cond, RM_COND_IGNORE, COND_GRP_CONN_ORIG)
            )
        {
            DBG_INFO(("{DSM} connection originator has changed.\n"));
            /*
              The originator of connection has changed, we need to
              do additional evaluation to determine if we need to
              handle this, so set the rm_cond to RM_COND_SLAVE
            */
            e_rm_cond = RM_COND_SLAVE ;
        }
        else
        {
            DBG_INFO(("{DSM} connection originator has not changed.\n"));
        }   
    }
    
    switch (e_rm_cond)
    {
        /*
          Share state conditions.
        */
        case RM_COND_SLAVE:
        {
            DBG_INFO(("{DSM} TV decoder [%d] condition is slave.\n",
                      h_comp));
            /*
              check if the state is connected, if the
              state is connected and DSM is not the originator,
              then, it means the DSM connection for TV decoder and
              AVC COMBI device has been superceded by CM or other.
              Send a message to DSM thread itself to determine
              if monitoring can still continue.  
            */
            e_rm_conn_cond = rm_cond_chg(e_nfy_cond,
                                         RM_COND_IGNORE,
                                         COND_GRP_CONN_STATE);
            
            e_rm_orig_cond = rm_cond_chg(e_nfy_cond,
                                         RM_COND_IGNORE,
                                         COND_GRP_CONN_ORIG);

            if ( e_rm_conn_cond == RM_COND_CONNECTED  &&
                 e_rm_orig_cond != RM_COND_CONN_ORIGINATOR )
            {
                DBG_INFO(("{DSM}: current state is connected but DSM is not "
                          "the originator. \n"));
                
                t_msg.e_msg_code = DSM_MSG_TV_DECODER_SLAVE;
                t_msg.h_dsm_comp = pt_dsm_comp->h_this_dsm_comp ;
                dsm_send_msg(&t_msg);
            }
        }
        break;

        case RM_COND_MASTER:
        {
            DBG_INFO(("{DSM}: TV decoder [%d] condition is master.\n",
                      h_comp));
            /*
              regain control of the TV decoder, reconnect and play.
            */
            t_msg.e_msg_code = DSM_MSG_TV_DECODER_MASTER;
            t_msg.h_dsm_comp = pt_dsm_comp->h_this_dsm_comp ;
            dsm_send_msg(&t_msg);
        }
        break;

        case RM_COND_SHARED:
        {
            DBG_INFO(("{DSM}: TV decoder [%d] condition is shared.\n",
                      h_comp));
        }
        break;

        case RM_COND_NO_CHG:
        default:
            break;
    }   
    return;
}


/*-----------------------------------------------------------------------------
 * Name: dsm_rm_tv_decoder_select
 *
 * Description: This API is called when a component is being opened. This API
 *              will find a component to open (if any).
 *
 *              For selecting the TV decoder, the following logic is used:
 *
 *                  1) if (no-conflict) and (is free) then pick this one
 *                     and terminate the search.
 *
 *                  2) if (no-conflict) and (not free), but the control-type
 *                     is master-slave, mark this one but continue searching
 *                     for a TV decoder that satisfy condition (1).  If
 *                     condition (1) never prevail, then the marked TV decoder
 *                     is selected.
 *
 *                  3) if (has conflic) then go to next one.
 *
 *              At the end of this process, the possible outcomes are:
 *
 *                  1) If condition (1) prevail, then we open TV
 *                     master-slave mode, we are the master,
 *
 *                  2) If condition (2) prevail and we open a rm  handle to
 *                     the TV decoder in master-slave mode, we are the slave.
 *
 *                  3) No TV decoder is available.
 *                
 *
 * Inputs:  e_type            Contains the driver type.
 *          ui2_id            Contains the driver component id.
 *          pv_comp_sel_data  References private data passed in rm_open_comp.
 *          pv_comp_data      References the component's private data.
 *          z_comp_data_len   Contains the length of *pv_comp_data.
 *          ui2_num_active    Contains the number of handles to this component
 *                            in OPENED state.
 *          ui2_num_wait      Contains the number of handles to this component
 *                            in OPENING state.
 *          e_ctrl            Contains the control type of the component.
 *          b_would_conflict  Specifies if a conflict could occur.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static SELECT_T dsm_rm_tv_decoder_select
(
    DRV_TYPE_T   e_type,
    UINT16       ui2_id,
    VOID*        pv_comp_sel_data,
    const VOID*  pv_comp_data,
    SIZE_T       z_comp_data_len,
    UINT16       ui2_num_active,
    UINT16       ui2_num_wait,
    CTRL_TYPE_T  e_ctrl,
    BOOL         b_would_conflict
)
{
    SELECT_T              e_select;
    
    if (! b_would_conflict)
    {
        if ((ui2_num_active == 0) && (ui2_num_wait == 0))
        {
            /*
              component is free and no conflict -> search is over
            */
            e_select = SEL_COMP_BREAK;
        }
        else
        {
            /*
              The ctrl-type is master-slave.
              component is not free, but no conflict -> component is selected
              but the search continues (a free component could still be found)
            */
            if ( e_ctrl == CTRL_TYPE_MASTER_SLAVE )
            {
                e_select = SEL_COMP_NEXT;
            }
            else
            {
                e_select = SEL_NEXT;
            }
        }
    }
    else
    {
        if ( ui2_num_active != 0 )
        {
            /* TV decoder has been opened by other component, so we mark this
               component but contiue the search.
            */
            e_select = SEL_COMP_NEXT;
        }
        else
        {
            /*
              conflict exists -> go to next one
            */
            e_select = SEL_NEXT;
        }
    }
    
    return e_select;
}
/*--------------------------------------------------------------------------
 * Name:   avc_combi_open_tv_decoder
 *
 * Description: This function opens a RM handle to the TV decoder.
 *
 * Input -
 *    pt_dsm_comp:  Ref to a AVC COMBI structure.
 *
 * Output -
 *    None
 *
 * Return -
 *    DSMR_OK:    TV decoder is connected.
 *   
 --------------------------------------------------------------------------*/
static INT32  avc_combi_open_tv_decoder
(
    DSM_COMP_T*         pt_dsm_comp 
)
{
    INT32               i4_rc ;
    DSM_AVC_COMBI_T*    pt_avc_comp;
    DSM_GROUP_BLK_T*    pt_grp_ctrl;
    CHAR*               ps_name = NULL;
    
    pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_comp->pv_private ;

    if ( pt_avc_comp->h_rm_tv_decoder == NULL_HANDLE )
    {
        DBG_INFO(("{DSM} open TV decoder component.\n"));

        /*
          check if TV decoder group name has been specifiedto
          restrict AVC COMBI device to use a specific
          TV decoder
        */
        if ( (i4_rc = dsm_comp_get_grp_ctrl_blk(pt_dsm_comp,&pt_grp_ctrl))
             != DSMR_OK )
        {
            return i4_rc ;
        }
        else
        {
            /* Check if TV decoder name is specified */
            if ( x_strlen(pt_grp_ctrl->ps_buf) > 0 )
            {
                ps_name=pt_grp_ctrl->ps_buf;
                DBG_INFO(("Use TV decoder: %s\n",ps_name));
                /* x_dbg_stmt("Use TV decoder: %s\n",ps_name); */
            }
        }
        
        /*
          Open a rm handle to TV decoder.
        */
        i4_rc = rm_open_comp(pt_dsm_comp->h_rm_hdle,
                             OBJ_COMP_REL_SRC,
                             DRVT_TV_DEC,
                             ANY_COMP_ID,
                             ps_name,
                             CTRL_TYPE_MASTER_SLAVE,
                             NULL, /* pv_comp_sel_data */
                             dsm_rm_tv_decoder_select, /* pf_comp_sel */
                             (VOID*)(pt_dsm_comp->h_this_dsm_comp), /* tag */
                             avc_rm_tv_decoder_nfy,
                             &(pt_avc_comp->h_rm_tv_decoder),
                             &(pt_avc_comp->e_rm_tv_decoder_cond)
            );
        
        dsm_rm_cond_code(pt_avc_comp->h_rm_tv_decoder,
                         pt_avc_comp->e_rm_tv_decoder_cond);

        if ( i4_rc != RMR_OK             &&
             i4_rc != RMR_ASYNC_NFY      &&
             i4_rc != RMR_ARBITRATION
            )
        {
            DBG_ERROR(("{DSM} Can not open an RM handle to TV decoder. RM error %d\n",
                       i4_rc));

            /* x_dbg_stmt("{DSM} Can not open an RM handle to TV decoder. RM error %d\n",
               i4_rc); */
        }
        i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
    }
    else
    {
        /*
          The TV decoder has already been opened, and connected, so
          just return OK.
        */
        i4_rc = DSMR_OK;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: dsm_check_rm_cond_code_for_conn_to_tv_decoder
 *
 * Description: This API checks the RM TV decoder handle condition.
 *              If the conditon is opened, connect, and slaved, then
 *              we assume the monitoring can proceeded.
 *
 * Inputs: 
 *   e_nfy_cond  Contains the notify condition.
 *          
 *
 * Outputs: -
 *
 * Returns:
 *   DSMR_OK    TV decoder condition is connected, opened, and slave.
 *   DSMR_FAIL  TV decoder's condition does not matched 'opened, connected, slave'.
 *
 ----------------------------------------------------------------------------*/
static INT32 dsm_check_rm_cond_code_for_conn_to_tv_decoder
(
    RM_COND_T      e_nfy_cond
)
{
    BOOL           b_can_monitor;
    RM_COND_T      e_rm_cond;
    INT32          i4_rc ;
    

    b_can_monitor = FALSE;
    i4_rc         = DSMR_FAIL;
    
    e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_OBJ_STATE);
    switch  (e_rm_cond)
    {
        
        case RM_COND_OPENED:
        {
            DBG_INFO(("{DSM} opened "));
            b_can_monitor = TRUE ;
        }
        break;

        default:
        {
            b_can_monitor = FALSE;
        }
        break;
    }
    
    e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_CONN_STATE);
    switch (e_rm_cond)
    {
        case RM_COND_CONNECTED:
        {
            DBG_INFO((" connected "));
            b_can_monitor = TRUE ;
        }
        break;

        default:
        {
            b_can_monitor = FALSE;
        }
        break;   
    }
    
    e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_SHARE_STATE);
    switch (e_rm_cond)
    {
        case RM_COND_SLAVE:
        {
            DBG_INFO((" slave "));
            b_can_monitor = TRUE ;
        }
        break;
        
        default:
        {
            b_can_monitor = FALSE;
        }
        break;   
    }

    DBG_INFO(("\n"));

    if ( b_can_monitor == TRUE )
    {
        i4_rc = DSMR_OK;
    }
    
    return i4_rc ;
}

/*--------------------------------------------------------------------------
 * Name:   avc_combi_connect_tv_decoder
 *
 * Description: This function connects the AVC combi device to the TV decoder.
 *
 * Input -
 *    pt_dsm_comp:  Ref to a AVC COMBI structure.
 *
 * Output -
 *    None
 *
 * Return -
 *    DSMR_OK:    TV decoder is connected and splaying has started.
 *
 *    DSMR_ASYNC: TV decoder is connected and driver will send a notification
 *                when it has started playing
 *
 *    DSMR_FAIL:  Can not connect to TV decoder, or can not (NO RIGHT) play
 *                TV decoder, or can not use TV decoder for monitoring because
 *                it used by another component.
 *                
 --------------------------------------------------------------------------*/
static INT32  avc_combi_connect_tv_decoder
(
    DSM_COMP_T*         pt_dsm_comp 
)
{
    INT32               i4_rc ;
    DSM_AVC_COMBI_T*    pt_avc_comp;
    
    pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_comp->pv_private ;

    DBG_INFO(("{DSM} connect AVC COMBI component (%d) with "
              "TV decoder component (%d)\n",
              pt_dsm_comp->h_rm_hdle,
              pt_avc_comp->h_rm_tv_decoder));

    pt_avc_comp->e_rm_tv_decoder_cond = 0 ;
    pt_avc_comp->ui4_video_signal = 0;
    pt_avc_comp->ui4_audio_signal = 0;
    pt_avc_comp->b_video_sig_set = FALSE;
    pt_avc_comp->b_audio_sig_set = FALSE;

    /*
      Connect TV decoder and AVC component togeter.
    */
    i4_rc = rm_connect(pt_avc_comp->h_rm_tv_decoder,
                       RM_CONN_TYPE_HANDLE,
                       (VOID*) pt_dsm_comp->h_rm_hdle,
                       sizeof(pt_dsm_comp->h_rm_hdle),
                       (VOID*)(pt_dsm_comp->h_this_dsm_comp), /* tag */
                       &(pt_avc_comp->e_rm_tv_decoder_cond));

    dsm_rm_cond_code(pt_avc_comp->h_rm_tv_decoder,
                     pt_avc_comp->e_rm_tv_decoder_cond);

    if ( i4_rc == RMR_OK             ||
         i4_rc == RMR_ASYNC_NFY      ||
         i4_rc == RMR_ARBITRATION
        )
    {
        TV_DEC_NFY_INFO_T  t_nfy_info;
        /*
          set the TV decoder notify fct.
        */
        DBG_INFO(("{DSM} Register rm nfy fct wit TV decoder.\n"));

        t_nfy_info.pv_tag        = (VOID*) pt_dsm_comp->h_this_dsm_comp;
        t_nfy_info.pf_tv_dec_nfy = tv_decoder_drv_nfy;

        /* rm_set TV_DEC_SET_TYPE_NFY_FCT */
        i4_rc = rm_set(pt_avc_comp->h_rm_tv_decoder,
                       FALSE,
                       ANY_PORT_NUM,
                       TV_DEC_SET_TYPE_NFY_FCT,
                       (VOID*) & t_nfy_info,
                       sizeof(TV_DEC_NFY_INFO_T),
                       &(pt_avc_comp->e_rm_tv_decoder_cond));

        dsm_rm_cond_code(pt_avc_comp->h_rm_tv_decoder,
                         pt_avc_comp->e_rm_tv_decoder_cond);

        if ( i4_rc == RMR_OK )
        {
            /*
              Start playing the TV decoder.
            */
            i4_rc = rm_set(pt_avc_comp->h_rm_tv_decoder,
                           FALSE,
                           ANY_PORT_NUM,
                           TV_DEC_SET_TYPE_CTRL,
                           (VOID*) TV_DEC_CTRL_PLAY,
                           sizeof(TV_DEC_CTRL_T),
                           &(pt_avc_comp->e_rm_tv_decoder_cond));

            dsm_rm_cond_code(pt_avc_comp->h_rm_tv_decoder,
                             pt_avc_comp->e_rm_tv_decoder_cond);
            
            if ( (i4_rc == RMR_OK)       ||
                 (i4_rc == RMR_ASYNC_NFY)
                )
            {
                DBG_INFO(("{DSM} connected to TV decoder and enable playing.\n"));
                i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
            }
            else
            {
                DBG_ERROR(("{DSM} TV decoder play failed. RM error: %d\n",
                           i4_rc));
                i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
            }
        }
        else
        {
            DBG_ERROR(("{DSM} cannot set nfy fct with TV decoder: RM error: %d\n",
                       i4_rc));
            i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
        }
    }
    else
    {
        /*
          If we receive RMR_NO_RIGHTS, it is due to another higher-priority
          component taking control of TV decoder.  We check the
          condition of TV decoder. If it is 'opened, connected, and slave',
          we assume we can still continue monitoring signal status
          of the AV Combi.
        */
        if ( i4_rc == RMR_NO_RIGHTS)
        {
            if ( (i4_rc = dsm_check_rm_cond_code_for_conn_to_tv_decoder
                  (pt_avc_comp->e_rm_tv_decoder_cond)  )
                 == DSMR_OK )
            {
                DBG_INFO(("{DSM} TV decoder is opened, connected and " 
                          "AV COMBI is a slave, monitoring can continue.\n"));
            }
        }
        else
        {
            DBG_ERROR(("{DSM} can not connect to TV decoder: RM error %d "
                       " TV decoder cond: %d\n",
                       i4_rc, pt_avc_comp->e_rm_tv_decoder_cond));
            i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
        }
    }
            
    return i4_rc;
}



/*--------------------------------------------------------------------------
 * Name:   dsm_avc_combi_enable_tv_decoder
 *
 * Description: This function opens a RM handle to the TV decoder,
 *              and connect the AVC combi device to the TV decoder,
 *              set the play and nfy fct on the TV decoder.
 *
 * Input -
 *    pt_dsm_comp:  Ref to a AVC COMBI structure.
 *
 * Output -
 *    None
 *
 * Return -
 *    DSMR_OK:    This component does not need a TV decoder to monitor signal,
 *                or a TV decoder is connected and splaying has started.
 *
 *    DSMR_ASYNC: TV decoder is connected and driver will send a notification
 *                when it has started playing
 *
 *    DSMR_FAIL:  Can not connect to TV decoder, or can not (NO RIGHT) play
 *                TV decoder, or can not use TV decoder for monitoring because
 *                it used by another component.
 --------------------------------------------------------------------------*/
INT32  dsm_avc_combi_enable_tv_decoder
(
    DSM_COMP_T*         pt_dsm_comp 
)
{
    INT32               i4_rc ;
    DSM_AVC_COMBI_T*    pt_avc_comp;
    
    pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_comp->pv_private ;

    if ( pt_avc_comp->b_need_tv_decoder == TRUE )
    {
        i4_rc = avc_combi_open_tv_decoder(pt_dsm_comp);

        if ( i4_rc == DSMR_OK )
        {
            i4_rc = avc_combi_connect_tv_decoder(pt_dsm_comp);
            /*
              If TV decoder connect and play ops failed, then
              free the TV decoder handle.
            */
            if ( i4_rc != DSMR_OK && i4_rc != DSMR_ASYNC_NFY )
            {
                /*
                  free the tv decoder handle.
                */
                rm_close(pt_avc_comp->h_rm_tv_decoder,FALSE,NULL);
                pt_avc_comp->h_rm_tv_decoder = NULL_HANDLE;
            }
        }
        else
        {
            DBG_INFO(( "avc_combi_open_tv_decoder fail\r\n" ));
        }
    }
    else
    {
        /*
          This AVC component does not need TV decoder, so just return OK.
        */
        i4_rc = DSMR_OK;
    }
    
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name:   dsm_avc_combi_monitor_with_tv_decoder
 *
 * Description: This function process the loss of the TV decoder (e.g., when
 *              the SLAVE condition is received) and when DSM no longer
 *              is the originator of the connection.
 *
 *              We first check if the AVC combi can still provide
 *              monitoring of signal.
 *
 * Input -
 *    pt_dsm_comp:  Ref to a AVC COMBI structure.
 *
 * Output -
 *    None
 *
 * Return -
 *    DSMR_OK:    AVC combi can still monitor TV signal.
 *    DSMR_FAIL:  AVC combi can not monitor TV signal.
 --------------------------------------------------------------------------*/
INT32  dsm_avc_combi_monitor_with_tv_decoder
(
    DSM_COMP_T*         pt_dsm_comp 
)
{
    INT32               i4_rc ;
    RM_COND_T           e_cond;
    DSM_AVC_COMBI_T*    pt_avc_comp;
    
    
    pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_comp->pv_private ;
    
    if ( pt_avc_comp->b_need_tv_decoder == TRUE )
    {
        DRV_TYPE_T                e_drv_src_type;
        UINT32                    ui4_cont_mon;
        SIZE_T                    z_get_info_len;
        INT32                     i4_idx ;
        UINT16                    ui2_src_comp_id;
        UINT8                     ui1_src_out_port;
        BOOL                      b_cont_mon_flag;
        AVC_INDEX_COMP_ID_T       t_drv_comp_id;

        ui4_cont_mon = 0 ;
        b_cont_mon_flag = FALSE;
        /*
          Determine the source device to which the TV-Decoder
          is connected.
        */
        if ( rm_get_conn_info_from_handle(
                 pt_avc_comp->h_rm_tv_decoder,
                 &e_drv_src_type,
                 &ui2_src_comp_id,
                 &ui1_src_out_port) == RMR_OK )
        {
            DBG_INFO(("{DSM} current driver type id and comp id "
                      "for the source to the TV decoder is: [%d] [%d]\n"
                      ,e_drv_src_type, ui2_src_comp_id));
                     
            z_get_info_len = sizeof (ui4_cont_mon);
            if ( rm_get(pt_dsm_comp->h_rm_hdle,
                        FALSE,
                        ANY_PORT_NUM,
                        AVC_GET_TYPE_CONT_COMBI_MON,
                        &ui4_cont_mon,
                        &z_get_info_len,
                        &e_cond) == RMR_OK
                )
            {
                /*
                   And now check if the re-connected TV-Decoder
                   does allow the AVC_COMBI to continue monitoring.
                */
                for ( i4_idx= 0 ; i4_idx < ((INT32)(pt_avc_comp->ui2_nb_dev)) ; i4_idx++ )
                {
                    if ( (ui4_cont_mon & (0x00000001<<i4_idx)) != 0 )
                    {
                        /*
                           Check if this AVC input device allows continued
                           monitoring. 
                        */
                        z_get_info_len = sizeof(AVC_INDEX_COMP_ID_T);
                        t_drv_comp_id.ui2_index= (UINT16)i4_idx;
                        /*
                          get the component id for each sub-component of
                          the AVC Combi device.
                        */
                        if ( rm_get(pt_dsm_comp->h_rm_hdle,
                                    FALSE,
                                    ANY_PORT_NUM,
                                    AVC_GET_TYPE_COMP_ID,
                                    &t_drv_comp_id,
                                    &z_get_info_len,
                                    &e_cond)
                             == RMR_OK
                            )
                        {
                            b_cont_mon_flag = (
                                (t_drv_comp_id.t_comp_id.e_type ==
                                 e_drv_src_type)                      &&
                                (t_drv_comp_id.t_comp_id.ui2_id ==
                                 ui2_src_comp_id)
                                ) ;
                            if ( b_cont_mon_flag == TRUE )
                            {
                                break;
                            }
                        }
                        else
                        {
                            DBG_INFO(("{DSM} rm_get(..AVC_GET_TYPE_COMP_ID) "
                                      "fails, can not get info on AVC "
                                      "sub-component.\n"));
                        }    
                    }
                }
            }
            else
            {
                DBG_INFO(("{DSM} rm_get(..AVC_GET_TYPE_CONT_COMBI_MON..) "
                          "fails, can not get info on AVC sub-component.\n"));
            }
        }
        else
        {
            DBG_INFO(("{DSM} rm_get_conn_info_from_handle() fails. "
                      "Can not get info on the TV decoder source.\n"));
        }
        
        if ( b_cont_mon_flag == FALSE )
        {
            /*
               Set the monitor flag for this AVC combi
               component to FALSE.
            */
            pt_avc_comp->b_mon_enabled = FALSE ;

            /*
              clear the data for the AVC COMBI structure.
            */
            pt_avc_comp->ui4_video_signal = 0;
            pt_avc_comp->ui4_audio_signal = 0;
            pt_avc_comp->b_audio_sig_set  = FALSE;
            pt_avc_comp->b_video_sig_set  = FALSE;

            /* Monitoring discontinued, return a FAIL code so the
               calling function can signal clients. */
            i4_rc = DSMR_FAIL;
        }
        else
        {
            i4_rc = DSMR_OK;
        }
    }
    else
    {
        /*
          This AVC component does not need TV decoder, so just return OK.
        */
        i4_rc = DSMR_OK;
    }
    
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_combi_get_signal_status
 *
 * Description: This API query the signal status (audio or video) for
 *              each component within the COMBI device.
 *
 * Input -
 *    pt_dsm_obj:  pointer to a DSM component structure.
 *    ui4_bit_mask:  bit mask used to extract the signal (video or audio)
 *                   status for a specific sub-component of AVC combi device.
 *    pz_size      Contains the length of buffer which is referenced
 *                 by pv_data.
 *
 * Output -
 *    pz_size:     Contains the length of the valid data in the
 *                 buffer referenced by the 'pv_data' argument.
 *    pv_data:     contains data return for this 'get' operation.
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   
 --------------------------------------------------------------------------*/
static INT32   dsm_avc_combi_get_signal_status
(
    DSM_COMP_T*         pt_dsm_obj,
    UINT32              ui4_bit_mask,
    SIZE_T*		        pz_size,
    VOID*               pv_data
)
{
    INT32               i4_rc;
    INT32               i4_i;
    INT32               i4_count;
    SIZE_T              z_size;
    UINT32              ui4_signal_value;
    UINT32              ui4_detect_status;
    UINT32              ui4_mask;
    UINT32              ui4_detect_mask;
    DSM_AVC_COMBI_T*    pt_combi;
    RM_COND_T           e_cond;
    AVC_COMBI_SIGNAL_STATUS_T  t_avc_signal;

    i4_rc = DSMR_FAIL;
    
    /*
      Arguments checking.

      For the pz size, it can be 4 or 8 bytes.

      if 4:  only the signal status is returned:  pv_data is a ref to
             UINT32[1]

      if 8:  the second UINT32 value contains the device status
             'detection' status.  If the device is un-stable (e.g.,
             can not determine the TV-Signal, then this bit is NOT SET.

             pv_data is a ref to UINT32[2].  If the device is stable
             (e.g. detection can be perform on the signal), then this
             bit for the corresponding device is set.
    */
    if ( pz_size     != NULL    &&
         (  (*pz_size)   == 4 ||  (*pz_size) == 8 )     &&
         pv_data     != NULL
        )
    {
        pt_combi = (DSM_AVC_COMBI_T*) pt_dsm_obj->pv_private;
                
        i4_count = (INT32)(pt_combi->ui2_nb_dev);

        /* clear the component signal mask.*/
        ui4_mask = 0 ;
        ui4_detect_mask = 0 ;
        
        /*
          Loop through each sub-component and get the signal
          level for each component.
        */
        for (i4_i = 0 ; i4_i < i4_count ; i4_i++ )
        {
            t_avc_signal.ui2_index = (UINT16)i4_i;
            z_size = sizeof(AVC_COMBI_SIGNAL_STATUS_T);
                    
            i4_rc = rm_get(pt_dsm_obj->h_rm_hdle,
                           FALSE,
                           ANY_PORT_NUM,
                           AVC_GET_TYPE_COMBI_SIGNAL_STATUS,
                           (VOID*)&t_avc_signal,
                           &z_size,
                           &e_cond);

            if ( i4_rc == RMR_OK )
            {
                /*
                  First check if the device can detect signal or not,
                  We do this check for both Video or Audio signal
                  get operation, but really Video signal has this
                  issue with signal detection.
                */
                
                if ( (t_avc_signal.ui4_signal_status & AVC_STATE_TV_SIGNAL_UNKNOWN) == 0 )
                {
                    ui4_detect_status = 1  ;  /* can detect TV signal. */
                }
                else
                {
                    ui4_detect_status = 0 ;  /* can not detect TV signal. */
                }

                ui4_detect_mask |= ( ui4_detect_status << i4_i ) ;
                
                /*
                  Save the value to the signal bitmask
                */
                if ( t_avc_signal.ui4_signal_status & ui4_bit_mask )
                {
                    ui4_signal_value = 1 ;
                }
                else
                {
                    ui4_signal_value = 0 ;
                }

                ui4_mask |= ( ui4_signal_value << i4_i );
            }
            else
            {
                DBG_ERROR(("{DSM} rm_get(AVC_GET_TYPE_COMBI_SIGNAL_STATUS) "
                          "failed. error code=> %d\n", i4_rc));
            }
            i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
        }

        /* store the signal value. */
        ((UINT32*)pv_data)[0] = ui4_mask;
        
        if ( (*pz_size) == 8 )
        {
            /* store the device detection status. */
            ((UINT32*)pv_data)[1] = ui4_detect_mask ;
        }
    }
    else
    {
        i4_rc = DSMR_INV_ARG;
    }
    
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_combi_get_cable_status
 *
 * Description: This API query the cable status (plug-in, unplug, or unknown)
 *              for the audio or video components within the COMBI device.
 *
 * Input -
 *    pt_dsm_obj:  pointer to a DSM component structure.
 *    ui4_bit_mask:  bit mask used to extract the signal (video or audio)
 *                   status for a specific sub-component of AVC combi device.
 *    pz_size      Contains the length of buffer which is referenced
 *                 by pv_data.
 *
 * Output -
 *    pz_size:     Contains the length of the valid data in the
 *                 buffer referenced by the 'pv_data' argument.
 *    pv_data:     contains data return for this 'get' operation.
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   
 --------------------------------------------------------------------------*/
static INT32   dsm_avc_combi_get_cable_status
(
    DSM_COMP_T*         pt_dsm_obj,
    UINT32              ui4_bit_mask,
    SIZE_T*		        pz_size,
    VOID*               pv_data
)
{
    INT32               i4_rc;
    INT32               i4_i;
    INT32               i4_count;
    SIZE_T              z_size;
    UINT32              ui4_plug_status_value;
    UINT32              ui4_detect_status;
    UINT32              ui4_mask;
    UINT32              ui4_detect_mask;
    DSM_AVC_COMBI_T*    pt_combi;
    RM_COND_T           e_cond;
    AVC_COMBI_CABLE_STATUS_T  t_avc_cable;

    i4_rc = DSMR_FAIL;
    
    /*
      Arguments checking.

      For the pz size, it can be 4 or 8 bytes.

      4:  Only the cable status is returned:  pv_data is a ref to
          UINT32[1]

      8:  The second UINT32 value contains the device cable status
          'detection' status.  If the device is un-stable (e.g.,
          can not determine the Cable plug-in status, then this bit
          is NOT SET.

          pv_data is a ref to UINT32[2].  If the device can detect cable
          plug-in status, then this bit for the corresponding device is set.
    */
    if ( pz_size     != NULL    &&
         (  (*pz_size)   == 4 ||  (*pz_size) == 8 )     &&
         pv_data     != NULL
        )
    {
        pt_combi = (DSM_AVC_COMBI_T*) pt_dsm_obj->pv_private;
                
        i4_count = (INT32)(pt_combi->ui2_nb_dev);

        /* clear the component signal mask.*/
        ui4_mask = 0 ;
        ui4_detect_mask = 0 ;
        
        /*
          Loop through each sub-component and get the signal
          level for each component.
        */
        for (i4_i = 0 ; i4_i < i4_count ; i4_i++ )
        {
            t_avc_cable.ui2_index = (UINT16)i4_i;
            z_size = sizeof(AVC_COMBI_CABLE_STATUS_T);
                    
            i4_rc = rm_get(pt_dsm_obj->h_rm_hdle,
                           FALSE,
                           ANY_PORT_NUM,
                           AVC_GET_TYPE_COMBI_CABLE_STATUS,
                           (VOID*)&t_avc_cable,
                           &z_size,
                           &e_cond);

            if ( i4_rc == RMR_OK )
            {
                /*
                  First check if the device can detect cable plug
                  state or not.
                */
                if ( (t_avc_cable.ui4_cable_status & AVC_STATE_CABLE_UNKNOWN) == 0 )
                {
                    ui4_detect_status = 1  ;  /* can detect cable status. */
                }
                else
                {
                    ui4_detect_status = 0 ;  /* can not detect cabke statys. */
                }

                ui4_detect_mask |= ( ui4_detect_status << i4_i ) ;
                
                /*
                  Save the value to the signal bitmask
                */
                if ( t_avc_cable.ui4_cable_status & ui4_bit_mask )
                {
                    ui4_plug_status_value = 1 ;
                }
                else
                {
                    ui4_plug_status_value = 0 ;
                }

                ui4_mask |= ( ui4_plug_status_value << i4_i );
            }
            else
            {
                DBG_ERROR(("{DSM} rm_get(AVC_GET_TYPE_COMBI_CABLE_STATUS) "
                          "to detect cable plugged-in status failed. error code=> %d\n", i4_rc));
            }
            i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
        }

        /* store the signal value. */
        ((UINT32*)pv_data)[0] = ui4_mask;
        
        if ( (*pz_size) == 8 )
        {
            /* store the device detection status. */
            ((UINT32*)pv_data)[1] = ui4_detect_mask ;
        }
    }
    else
    {
        i4_rc = DSMR_INV_ARG;
    }
    
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_combi_get_ops
 *
 * Description: This API executed specific 'GET' operation on AVC COMBI
 *              device.
 *
 * Input -
 *    pt_dsm_obj:  pointer to a DSM component structure.
 *    e_get_type:  specifies the type of get operation for which the
 *    pv_get_info: contains 'get' specific info.
 *    pz_size      Contains the length of buffer which is referenced
 *                 by pv_data.
 *
 * Output -
 *    pz_size:     Contains the length of the valid data in the
 *                 buffer referenced by the 'pv_data' argument.
 *    pv_data:     contains data return for this 'get' operation.
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   DSMR_FAIL: get ops failed. 
 --------------------------------------------------------------------------*/
INT32   dsm_avc_combi_get_ops
(
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_GET_T    e_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{
    DSM_AVC_COMBI_T* pt_avc_comp;
    INT32            i4_rc;
    RM_COND_T        e_cond;
    SIZE_T           pz_rm_size;
    UINT32           ui4_idx;

    pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_obj->pv_private;
            
    i4_rc = DSMR_OK;
            
    /*
      Convert the get type and set up the request structure to
      call the RM_GET() function.
    */
    switch (e_get_type )
    {
        case DSM_GET_AVC_DEV_TYPE:
        {
            /*
               AVC COMBI specific get_info structure.
            */
            
            AVC_INDEX_COMP_ID_T        t_drv_comp_id;
            
            /*
              Check arguments.
              pv_data: should be an address to a DEVICE_TYPE_T variable.
            */
            if ( 
                 pz_size     != NULL    &&
                 *pz_size    >  0       &&
                 pv_data     != NULL
                )
            {
                ui4_idx = (UINT32) pv_get_info;
                t_drv_comp_id.ui2_index= (UINT16)ui4_idx;
                pz_rm_size = sizeof(AVC_INDEX_COMP_ID_T);
                i4_rc = rm_get(pt_dsm_obj->h_rm_hdle,
                               FALSE,
                               ANY_PORT_NUM,
                               AVC_GET_TYPE_COMP_ID,
                               &t_drv_comp_id,
                               &pz_rm_size,
                               &e_cond);

                i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
                /*
                  Post processing after RM_GET() call.
                */
                if ( i4_rc == DSMR_OK )
                {
                    DEVICE_TYPE_T    e_dev_type;
                    i4_rc = dsm_avc_map_drv_type_to_dev_type(
                        t_drv_comp_id.t_comp_id.e_type, &e_dev_type);

                    *((DEVICE_TYPE_T*)pv_data) = e_dev_type;
                    *pz_size = sizeof(DEVICE_TYPE_T);
                }
            }
            else
            {
                i4_rc = DSMR_INV_ARG;
            }
        }
        break;

        case DSM_GET_AVC_NUM_OF_DEV:
        {
            /*
              Check arguments.
              
              pv_data should be an address to an UINT16 variable.
              pz_size should be 2 bytes.
            */
            if ( pz_size     != NULL    &&
                 *pz_size    == 2       &&
                 pv_data     != NULL
                )
            {
                i4_rc = rm_get(pt_dsm_obj->h_rm_hdle,
                               FALSE,
                               ANY_PORT_NUM,
                               AVC_GET_TYPE_NUM_COMP_IDS,
                               pv_data,
                               pz_size,
                               &e_cond);

                i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
            }
            else
            {
                i4_rc = DSMR_INV_ARG;
            }
        }
        break;
        
        case DSM_GET_AVC_VIDEO_SIG_STATUS:
        {
            UINT32    ui4_bit_mask = 0;
            /*
              Set the type of signal (video or audio) status
               to query from the driver.
            */
            ui4_bit_mask = AVC_STATE_TV_SIGNAL ;
            
            if ( pt_avc_comp->b_mon_enabled == TRUE )
            {
                i4_rc = dsm_avc_combi_get_signal_status(
                    pt_dsm_obj, ui4_bit_mask, pz_size, pv_data);
            }
            else
            {
                i4_rc = DSMR_FAIL;
            }
        }
        break;
        
        case DSM_GET_AVC_AUDIO_SIG_STATUS:
        {
            UINT32    ui4_bit_mask = 0;
            /*
              Set the type of signal (video or audio) status
               to query from the driver.
            */
            ui4_bit_mask =  AVC_STATE_AUDIO_SIGNAL;

            if ( pt_avc_comp->b_mon_enabled == TRUE )
            {
                i4_rc = dsm_avc_combi_get_signal_status(
                    pt_dsm_obj, ui4_bit_mask, pz_size, pv_data);
            }
            else
            {
                i4_rc = DSMR_FAIL;
            }
        }
        break;

        case DSM_GET_AVC_CABLE_STATUS:
        {
            UINT32    ui4_bit_mask = 0;
            /*
              Set the type of signal (video or audio) status
               to query from the driver.
            */
            ui4_bit_mask =  AVC_STATE_CABLE_PLUGGED;

            if ( pt_avc_comp->b_mon_enabled == TRUE )
            {
                i4_rc = dsm_avc_combi_get_cable_status(
                    pt_dsm_obj, ui4_bit_mask, pz_size, pv_data);
            }
            else
            {
                i4_rc = DSMR_FAIL;
            }
        }
        break;
        
        default:
        {
            i4_rc = DSMR_INV_ARG;
        }
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_combi_set_ops
 *
 * Description: This API executed specific 'SET' operation on AVC COMBI
 *              device.
 *
 * Input -
 *    pt_dsm_obj:   pointer to a DSM component structure. 
 *    e_set_type:   specifies the 'set' operation.
 *    pv_set_info:  pointer to a structure containing 'set' specific info.
 *    z_size        size (in bytes) of the pv_set_info structure.
 *
 * Output -
 *    None. 
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32   dsm_avc_combi_set_ops
(
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_GET_T    e_set_type,
    VOID*            pv_set_info,
    SIZE_T		     z_size
)
{
    INT32            i4_rc;
    
    i4_rc = DSMR_OK;
            
    /*
      Convert the get type and set up the request structure to
      call the RM_GET() function.
    */
    switch (e_set_type )
    {
        case DSM_SET_AVC_START_MONITOR:
        {   
            /*
              enable to TV decoder for monitoring.
            */
            i4_rc = dsm_avc_combi_enable_tv_decoder(pt_dsm_obj);

            /* CR DTV00135386
               
               Check the status of the TV decoder:
               If the TV decoder is enabled and playing, then query
               the driver for the current signal status.  If current
               status is not unknown, then send a notification to the
               app with its video/audio status.
            */
            if ( i4_rc == DSMR_OK )
            {
                DSM_DEV_GET_T   e_get_type     = DSM_GET_AVC_VIDEO_SIG_STATUS;
                UINT32          aui4_signal[2];
                SIZE_T		    z_size         = 8;
                INT32           i4_result;
                DSM_MSG_T       t_msg;
                AVC_COND_T      e_nfy_cond;

                aui4_signal[0]=0;
                aui4_signal[1]=0;
                i4_result = dsm_avc_combi_get_ops(pt_dsm_obj,
                                                  e_get_type,
                                                  NULL,
                                                  &z_size,
                                                  (VOID*)(aui4_signal));
                if ( i4_result == DSMR_OK )
                {
                    UINT32  ui4_dev_comp=0;
                    UINT32  i;
                    UINT32  posit=0x1;
                    DSM_AVC_COMBI_T*    pt_avc_comp;
    
                    pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_obj->pv_private ;
                    
                    DBG_INFO(("{DSM} Get AVC COMBI signal OK, notify client.\n"));
                    /* check the second byte to see if  video signal status can
                       be determined.
                    */
                    if ( aui4_signal[1] != 0  )
                    {
                        DBG_INFO(("{DSM} AVC COMBI signal can be determined.\n"));
                    }

                    /* Determine which bit is set to indicate presence of video signal
                       (e.g, bit 0 for device id 0, bit 1 for device id 1, etc
                    */
                    for (i =0 ; i < 32; i++ )
                    {
                        if (  aui4_signal[0] & posit )
                        {
                            ui4_dev_comp= i;
                            break;
                        }
                        posit = posit << 1 ;
                    }

                    /*
                         Send signal change notification to client.
                    */
                    e_nfy_cond =AVC_COND_COMBI_SIGNAL_STATUS;

                    /*
                         Clear the signal status flag, this will
                         make sure the notification will always be
                         sent to application.
                    */
                    pt_avc_comp->b_video_sig_set = FALSE;
                    
                    /*  send msg to DSM notification thread. */
                    t_msg.e_msg_code = DSM_MSG_AVC_COMBI_DRVT_NFY;
                    t_msg.h_dsm_comp = pt_dsm_obj->h_this_dsm_comp ;
                    t_msg.u.t_avc_combi.e_nfy_cond = e_nfy_cond;
                    t_msg.u.t_avc_combi.ui4_data_1 = AVC_STATE_TV_SIGNAL;
                    t_msg.u.t_avc_combi.ui4_data_2 = ui4_dev_comp;
                    dsm_send_msg(&t_msg);
                }
            }
        }
        break;

        case DSM_SET_AVC_STOP_MONITOR:
        {
            /*
               No action to perform here. The stop monitoring is
               stored on a per client structure.  The driver
               notification continue to be received at the layer
               between AVC COMBI driver component and DSM layer, but
               notification is not pass up to the client.
            */
        }
        break;

        default:
        {
            i4_rc = DSMR_INV_ARG;
        }
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_combi_open_pipe
 *
 * Description: This API creates a RM pipe to be used for connecting
 *              AVC combi component and TV decoder component.
 *
 * Input -
 *    None
 *
 * Output -
 *    ph_rm_pipe:   handle to the the RM pipe.
 *
 * Return -
 *   DSMR_OK:   pipe open operation success. 
 *   DSMR_FAIL: internal error.
 --------------------------------------------------------------------------*/
INT32   dsm_avc_combi_open_pipe
(
    HANDLE_T*      ph_rm_pipe
)
{
    INT32            i4_rc;
    UINT32           ui4_flag;
    RM_COND_T        e_rm_cond;
    
    i4_rc = DSMR_INV_ARG;
            
    if ( ph_rm_pipe != NULL )
    {
        *ph_rm_pipe=NULL_HANDLE;
        ui4_flag = RM_PIPE_FLAG_OPEN_ARB_QUERY |
                   RM_PIPE_FLAG_CONN_ARB_QUERY;
        
        i4_rc = rm_open_pipe(
            NULL,                       /* avc_combi ->   source name. */
            NULL,                       /* tv_decoder -> sink name.   */
#ifdef MW_TV_AV_BYPASS_SUPPORT
            254,
#else            
            255,                        /* priority.   0 highest
                                                     255 lowest.
                                        */
#endif                                        
            ui4_flag,                   /* flag. */
            NULL,                       /* tag value, will be set later
                                           after the AVC Combi component 
                                           is created from this pipe.
                                        */
            avc_rm_pipe_nfy_fct,        /* nfy function for the pipe. */  
            ph_rm_pipe,                 /* handle to the pipe object. */
            &e_rm_cond                  /* pipe condition.            */
            );

        if ( i4_rc == RMR_OK )
        {
             DBG_INFO(("{DSM} RM pipe [%d] for AVC COMBI and TV decoder opened\n",
                       *ph_rm_pipe));
        }
        else
        {
             DBG_INFO(("{DSM} open RM pipe failed.\n"));
        }
        
        i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_combi_post_open_action
 *
 * Description: This function is executed after an AVC COMBI component
 *              has been opened to perform specific initialization.
 *          
 *
 * Input -
 *    pt_dsm_comp:  pointer to DSM component structure.
 *    
 * Output -
 *    None
 *
 * Return -
 *   DSMR_OK:   AVC COMBI component is successfully initialized. 
 *   DSMR_FAIL: Internal error, initialization fails.
 --------------------------------------------------------------------------*/
INT32   dsm_avc_combi_post_open_action
(
    DSM_COMP_T*      pt_dsm_comp
)
{
    INT32            i4_rc;
    DSM_AVC_COMBI_T* pt_avc_comp = NULL;
    BOOL             b_flag;
    
    i4_rc = DSMR_FAIL;
    
    if ( pt_dsm_comp->t_dev_type_name == DEV_AVC_COMBI   &&
         pt_dsm_comp->h_rm_hdle   != NULL_HANDLE
        )
    {
        /*
          Initialize the private data for the AVC COMBI device If the
          AVC COMBIN specific data has not been initialized,
        */
        if ( pt_dsm_comp->pv_private == NULL )
        {
            /*
              create a AVC COMBI specific structure to store
              AVC COMBI specific information.
            */
            pt_avc_comp = (DSM_AVC_COMBI_T*)
                x_mem_alloc(sizeof(DSM_AVC_COMBI_T));

            if ( pt_avc_comp != NULL )
            {
                UINT16             ui2_nb_dev;
                SIZE_T             z_size;
                RM_COND_T          e_cond;
                AVC_NFY_INFO_T     t_avc_nfy;

                pt_dsm_comp->pv_private = pt_avc_comp ;
                
                pt_avc_comp->ui2_nb_dev = 0 ;
                pt_avc_comp->ui4_video_signal = 0;
                pt_avc_comp->ui4_audio_signal = 0;
                pt_avc_comp->ui4_cable_status = 0;
                pt_avc_comp->b_need_tv_decoder = FALSE;
                pt_avc_comp->b_need_audio_decoder = FALSE;
                pt_avc_comp->b_audio_sig_set  = FALSE;
                pt_avc_comp->b_video_sig_set  = FALSE;
                pt_avc_comp->b_cable_status_set = FALSE;
                pt_avc_comp->b_mon_enabled = FALSE;
                pt_avc_comp->h_rm_tv_decoder = NULL_HANDLE;
                pt_avc_comp->h_rm_pipe = NULL_HANDLE;
                
                /*
                  Get the number of devices for this AVC_COMBI device
                */
                z_size = 2 ;
                i4_rc = dsm_avc_combi_get_ops(
                    pt_dsm_comp,
                    DSM_GET_AVC_NUM_OF_DEV,
                    NULL,
                    &z_size,
                    (VOID*)&ui2_nb_dev);
            
                if ( i4_rc == DSMR_OK )
                {
                    pt_avc_comp->ui2_nb_dev = ui2_nb_dev ;

                    /*
                      Set the AVC notification function.
                      and flag value. The initial default
                      is not to notify the client.
                    */
                
                    t_avc_nfy.pv_tag     = (VOID*) pt_dsm_comp->h_this_dsm_comp;
                    t_avc_nfy.pf_avc_nfy = avc_combi_drv_nfy_fct;
                    z_size = sizeof(AVC_NFY_INFO_T);
                
                    /*
                      Register AVC Combi notification fct with
                      the driver.
                    */
                    i4_rc = rm_set(pt_dsm_comp->h_rm_hdle,
                                   FALSE,
                                   ANY_PORT_NUM,
                                   AVC_SET_TYPE_NFY_FCT,
                                   (VOID*)(&t_avc_nfy),
                                   z_size,
                                   &e_cond);
                    if ( i4_rc == RMR_OK )
                    {
                        /*
                          Get the driver flag for this device.
                        */
                        UINT32        ui4_comp_flags;
                        DRV_TYPE_T    t_drv_type;
                        UINT16        ui2_id;
                        UINT8         ui1_num_inp_ports;
                        UINT8         ui1_num_out_ports;
                        
                        i4_rc = rm_get_comp_info_from_handle (
                            pt_dsm_comp->h_rm_hdle,
                            &t_drv_type,
                            &ui2_id,
                            &ui1_num_inp_ports,
                            &ui1_num_out_ports,
                            &ui4_comp_flags);
                        
                        /*
                          check if decoder is need to detect TV signal.
                        */
                        if ( i4_rc == RMR_OK )
                        {
                            if ( (ui4_comp_flags &
                                  AVC_FLAG_DEC_REQ_FOR_TV_SIG_DETECTION)  != 0
                                )
                            {
                                /*
                                  Set the flag to indicate the TV decoder
                                  is needed to monitor signal.  The
                                  connection to the TV decoder will be
                                  performed when client request 'monitoring'
                                  start.
                                */
                                pt_avc_comp->b_need_tv_decoder = TRUE;

                                DBG_API(("{DSM} this component require TV decoder.\n"));
                            }

                            if ( (ui4_comp_flags &
                                  AVC_FLAG_DEC_REQ_FOR_AUD_SIG_DETECTION)  != 0
                                )
                            {
                                /*
                                  Set the flag to indicate the audio decoder
                                  is needed to monitor signal.  The
                                  connection to the audio decoder will be
                                  performed when client request 'monitoring'
                                  start.
                                */
                                pt_avc_comp->b_need_audio_decoder = TRUE;

                                DBG_API(("{DSM} this component require audio decoder.\n"));
                            }
                            
                            /*
                              Set the AVC combi connection option:
                              don't notify disconnect when signal loss is
                              detected.
                            */
                            b_flag = TRUE;
                            z_size = sizeof(BOOL);
                            
                            i4_rc = rm_set(pt_dsm_comp->h_rm_hdle,
                                           FALSE,
                                           ANY_PORT_NUM,
                                           AVC_SET_TYPE_NO_CONN_DISC_NFY,
                                           (VOID*)((INT32) b_flag),
                                           z_size,
                                           &e_cond);
                            if ( i4_rc == RMR_OK )
                            {
                                i4_rc = DSMR_OK;
                                DBG_API(("{DSM} AVC COMBI component's ( %d) "
                                         "private data initialized.\n",
                                         pt_dsm_comp->h_rm_hdle));
                            }
                            else
                            {
                                DBG_API(("{DSM} AVC COMBI set "
                                         "AVC_SET_TYPE_NO_CONN_DISC_NFY "
                                         "failed, error code: %d\n", i4_rc));
                                
                                i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
                                x_mem_free(pt_dsm_comp->pv_private);
                                pt_dsm_comp->pv_private = NULL ;
                            }   
                        }
                        else
                        {
                            i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
                            x_mem_free(pt_dsm_comp->pv_private);
                            pt_dsm_comp->pv_private = NULL ;
                        }
                    }
                    else
                    {
                        i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
                        x_mem_free(pt_dsm_comp->pv_private);
                        pt_dsm_comp->pv_private = NULL ;
                    }
                }
                else
                {
                    x_mem_free(pt_dsm_comp->pv_private);
                    pt_dsm_comp->pv_private = NULL ;
                }
            }
            else
            {
                i4_rc = DSMR_OUT_OF_MEM;
            }   
        }
        else
        {
            i4_rc = DSMR_OK;
            DBG_API(("{DSM} AVC COMBI component's (rm handle: %d) "
                     "private data already initialized.\n",
                     pt_dsm_comp->h_rm_hdle));
        }
    }
    return i4_rc;
}


/*--------------------------------------------------------------------------
 * Name:   dsm_avc_combi_proc_signal_status_nfy
 *
 * Description: This notification function is called in the DSM thread
 *              context to process COMBI driver signal notification.       
 *
 * Input -
 *    pv_data:  Pointer to DSM message structure contains AVC COMBI
 *              driver notification data.
 *
 * Output -
 *    None.
 *
 * Return -
 *   DSMR_OK:   process signal notify operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32  dsm_avc_combi_proc_signal_status_nfy
(
    VOID*           pv_data
)
{
    INT32                       i4_rc;
    DSM_MSG_T*                  pt_msg;
    DSM_HL_PARSE_DEV_DATA_T     t_parse_data;
    SIZE_T                      z_size;
    UINT32                      ui4_data_1;
    UINT32                      ui4_comp_idx;
    UINT32                      ui4_bit_mask;
    DSM_COMP_T*                 pt_dsm_comp = NULL;
    DSM_AVC_COMBI_T*            pt_avc_comp = NULL;
    DSM_AVC_COMBI_NFY_DATA_T*   pt_avc_drv_data;
    BOOL                        b_vid_notify ;
    BOOL                        b_aud_notify ;
    

    i4_rc = DSMR_OK;
    pt_msg = (DSM_MSG_T*) pv_data;

    pt_dsm_comp = dsm_get_ref_to_comp(pt_msg->h_dsm_comp) ;
    if ( pt_dsm_comp == NULL )
    {
        DBG_ERROR(("{DSM} dsm_avc_combi_proc_client_nfy, pt_dsm_comp == NULL\r\n" ));
        return DSMR_FAIL;
    }
    
    pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_comp->pv_private ;
    pt_avc_drv_data = &(pt_msg->u.t_avc_combi);

    /*
      Driver notification condition and data.
  
      data 1:  signal status:  AVC_STATE_TV_SIGNAL: 0 off
                                                    1 on
                                                 
                               AVC_STATE_AUDIO_SIGNAL: 0 off
                                                       1 on

                               AVC_STATE_TV_SIGNAL_UNKNOWN: 0 Signal known
                                                            1 Signal unknown
      data 2:  index to sub-comp

      Note: Driver will only notify DSM when data signal status is known.
      So we should never receive AVC_STATE_TV_SIGNAL_UNKNOWN in this
      function.
    */
    ui4_data_1 = pt_avc_drv_data->ui4_data_1;
    ui4_comp_idx = pt_avc_drv_data->ui4_data_2;
    
    ui4_bit_mask = (UINT32)(0x01 << ui4_comp_idx) ;

    if ( ui4_data_1 & AVC_STATE_TV_SIGNAL_UNKNOWN )
    {
        DBG_ERROR(("{DSM} receive AVC COMBI notification, but notification indicates TV signal unknown: %d\n",
                 ui4_data_1));
        pt_avc_comp->b_video_sig_set = FALSE;
        pt_avc_comp->b_audio_sig_set = FALSE;
        
        return DSMR_FAIL;
    }

    DBG_INFO(( "{DSM} dsm_avc_combi_proc_client_nfy, ui4_data_1=0x%X, ui4_comp_idx=0x%X\r\n",
        ui4_data_1, ui4_comp_idx ));

    /*
      process Video signal notify, determine if the video signal has
      changed since last notification.
    */
    b_vid_notify = FALSE;
    if ( pt_avc_comp->b_video_sig_set == FALSE )
    {
        b_vid_notify = TRUE;
    }
    else
    {
        BOOL b_old ;
        BOOL b_new ;

        b_old = ((pt_avc_comp->ui4_video_signal & ui4_bit_mask) != 0 ) ;
        b_new = ((ui4_data_1 & AVC_STATE_TV_SIGNAL) != 0 ) ;
        
        if ( b_old ^ b_new )
        {
            b_vid_notify = TRUE;
        }
    }

    if ( b_vid_notify == TRUE ) 
    {
        /*
           Query the AVC driver the current status value.
        */
        z_size=sizeof(UINT32);
        i4_rc = dsm_avc_combi_get_ops(pt_dsm_comp,
                                      DSM_GET_AVC_VIDEO_SIG_STATUS,
                                      NULL,
                                      &z_size,
                                      &(pt_avc_comp->ui4_video_signal));

        pt_avc_comp->b_video_sig_set = TRUE;
        /*
          Set  up the data to call client's notification.
        */
        t_parse_data.h_dev_comp = pt_dsm_comp->h_rm_hdle;
        t_parse_data.e_cond = DSM_COND_DEV_VIDEO_SIG_STATUS;
        t_parse_data.pv_tag = 0;
        t_parse_data.ui4_data_1 = pt_avc_comp->ui4_video_signal;
        t_parse_data.ui4_data_2 = 0;
        /*
          call client's notification function.
        */
        DBG_INFO(("{DSM} send video nfy to client, video signal: %d\n",
                 t_parse_data.ui4_data_1));
        
        dsm_dev_notify_all(pt_dsm_comp, &t_parse_data);
    }
    else
    {
        DBG_INFO(("{DSM} skip send video nfy to client, video signal: %d, vdo set:%s\n",
                pt_avc_comp->ui4_video_signal, 
                (pt_avc_comp->b_video_sig_set)?"TRUE":"FALSE" ));
    }    
    /*
      process audio signal notify, determine if the audio signal has
      changed since last notification.
    */
    b_aud_notify = FALSE;
    if ( pt_avc_comp->b_audio_sig_set == FALSE )
    {
        b_aud_notify = TRUE;
    }
    else
    {
        BOOL b_old ;
        BOOL b_new ;

        b_old = ((pt_avc_comp->ui4_audio_signal & ui4_bit_mask) != 0 ) ;
        b_new = ((ui4_data_1 & AVC_STATE_AUDIO_SIGNAL) != 0 ) ;
        
        if (  b_old ^ b_new )
        {
            b_aud_notify = TRUE;
        }
    }
    
    if ( b_aud_notify == TRUE )
    {
        /*
           Query the ATV driver the current status value.
        */
        z_size=sizeof(UINT32);
        i4_rc = dsm_avc_combi_get_ops(pt_dsm_comp,
                                      DSM_GET_AVC_AUDIO_SIG_STATUS,
                                      NULL,
                                      &z_size,
                                      &(pt_avc_comp->ui4_audio_signal));

        pt_avc_comp->b_audio_sig_set = TRUE;
        /*
          Set  up the data to call client's notification.
        */
        t_parse_data.h_dev_comp = pt_dsm_comp->h_rm_hdle;
        t_parse_data.e_cond = DSM_COND_DEV_AUDIO_SIG_STATUS;
        t_parse_data.pv_tag = 0;
        t_parse_data.ui4_data_1 = pt_avc_comp->ui4_audio_signal;
        t_parse_data.ui4_data_2 = 0;
        /*
          call client's notification function.
        */
        DBG_INFO(("{DSM} send audio nfy to client, audio signal: %d\n",
                 t_parse_data.ui4_data_1));
        dsm_dev_notify_all(pt_dsm_comp, &t_parse_data);
    }
    else
    {
        DBG_INFO(("{DSM} skip send audio nfy to client, audio signal: %d, aud set:%s\n",
                pt_avc_comp->ui4_audio_signal,
                (pt_avc_comp->b_audio_sig_set)?"TRUE":"FALSE"));
    }   
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name:   dsm_avc_combi_proc_cable_status_nfy
 *
 * Description: This notification function is called in the DSM thread
 *              context to process COMBI driver cable status notification.    
 *
 * Input -
 *    pv_data:  Pointer to DSM message structure contains AVC COMBI
 *              driver notification data.
 *
 * Output -
 *    None.
 *
 * Return -
 *   DSMR_OK:   process signal notify operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32  dsm_avc_combi_proc_cable_status_nfy
(
    VOID*           pv_data
)
{
    INT32                       i4_rc;
    DSM_MSG_T*                  pt_msg;
    DSM_HL_PARSE_DEV_DATA_T     t_parse_data;
    SIZE_T                      z_size;
    UINT32                      ui4_data_1;
    UINT32                      ui4_comp_idx;
    UINT32                      ui4_bit_mask;
    DSM_COMP_T*                 pt_dsm_comp = NULL;
    DSM_AVC_COMBI_T*            pt_avc_comp = NULL;
    DSM_AVC_COMBI_NFY_DATA_T*   pt_avc_drv_data;
    BOOL                        b_cable_notify ;

    i4_rc = DSMR_OK;
    pt_msg = (DSM_MSG_T*) pv_data;

    pt_dsm_comp = dsm_get_ref_to_comp(pt_msg->h_dsm_comp) ;
    if ( pt_dsm_comp == NULL )
    {
        return DSMR_FAIL;
    }
    
    pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_comp->pv_private ;
    pt_avc_drv_data = &(pt_msg->u.t_avc_combi);

    /*
      Driver notification condition and data.
  
      data 1: cable status:  AVC_STATE_CABLE_PLUGGED: 0 un-plugged
                                                      1 plugged

                             AVC_STATE_CABLE_UNKNOWN: 0 cable status known
                                                      1 cable status unknown
      data 2: index to sub-comp

      Note: Driver will only notify DSM when cable status is known.
      So we should never receive AVC_STATE_CABLE_UNKNOWN in this
      function.
    */
    ui4_data_1 = pt_avc_drv_data->ui4_data_1;
    if ( ui4_data_1 & AVC_STATE_CABLE_UNKNOWN )
    {
        DBG_ERROR(("{DSM} receive AVC COMBI cable status notification, but notification data indicates cable status is unknown: %d\n", ui4_data_1));
        return DSMR_FAIL;
    }

    ui4_comp_idx = pt_avc_drv_data->ui4_data_2;
    ui4_bit_mask = (UINT32)(0x01 << ui4_comp_idx) ;
    /*
      process cable status notify, determine if the cable status has
      changed since last notification.
    */
    b_cable_notify = FALSE;
    if ( pt_avc_comp->b_cable_status_set == FALSE )
    {
        b_cable_notify = TRUE;
    }
    else
    {
        BOOL b_old ;
        BOOL b_new ;

        b_old = ((pt_avc_comp->ui4_cable_status & ui4_bit_mask) != 0 ) ;
        b_new = ((ui4_data_1 & AVC_STATE_CABLE_PLUGGED) != 0 ) ;
        
        if ( b_old ^ b_new )
        {
            b_cable_notify = TRUE;
        }
    }

    if ( b_cable_notify == TRUE ) 
    {
         /*
           Query the AVC driver the current status value.
         */
        z_size=sizeof(UINT32);
        i4_rc = dsm_avc_combi_get_ops(pt_dsm_comp,
                                      DSM_GET_AVC_CABLE_STATUS,
                                      NULL,
                                      &z_size,
                                      &(pt_avc_comp->ui4_cable_status));

        pt_avc_comp->b_cable_status_set = TRUE;
        /*
          Set  up the data to call client's notification.
        */
        t_parse_data.h_dev_comp = pt_dsm_comp->h_rm_hdle;
        t_parse_data.e_cond = DSM_COND_DEV_CABLE_STATUS;
        t_parse_data.pv_tag = 0;
        t_parse_data.ui4_data_1 = pt_avc_comp->ui4_cable_status;
        t_parse_data.ui4_data_2 = 0;
        /*
          call client's notification function.
        */
        DBG_API(("{DSM} send cable status nfy to client, cable status: %d\n",
                 t_parse_data.ui4_data_1));
        
        dsm_dev_notify_all(pt_dsm_comp, &t_parse_data);
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name:   dsm_avc_combi_proc_client_nfy
 *
 * Description: This notification function is called in the DSM thread
 *              context to process COMBI driver notification.             
 *
 * Input -
 *    pv_data:  Pointer to DSM message structure contains AVC COMBI
 *              driver notification data.
 *
 * Output -
 *    None.
 *
 * Return -
 *   DSMR_OK:   process signal notify operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32  dsm_avc_combi_proc_client_nfy
(
    VOID*           pv_data
)
{
    INT32                       i4_rc;
    DSM_MSG_T*                  pt_msg;

    i4_rc = DSMR_OK;
    pt_msg = (DSM_MSG_T*) pv_data;

    if ( pt_msg->u.t_avc_combi.e_nfy_cond == AVC_COND_COMBI_SIGNAL_STATUS )
    {
        i4_rc=dsm_avc_combi_proc_signal_status_nfy(pv_data);
    }

    if ( pt_msg->u.t_avc_combi.e_nfy_cond == AVC_COND_COMBI_CABLE_STATUS )
    {
        i4_rc=dsm_avc_combi_proc_cable_status_nfy(pv_data);
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name:   dsm_avc_combi_proc_tv_decoder_nfy
 *
 * Description: This notification function is called in the DSM thread
 *              context to notify client signal monitoring status.
 *              The monitoring status can be change due to lost/gain of
 *              control to the TV decoder and whether the TV decoder
 *              can be shared by different component for monitoring
 *              purpose.
 *
 * Input -
 *    pv_dsm_comp:  Pointer to DSM message structure contains AVC COMBI
 *              driver notification data.
 *
 * Output -
 *    None.
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32  dsm_avc_combi_proc_tv_decoder_nfy
(
    DSM_COMP_T*                 pt_dsm_comp,
    DSM_COND_DEV_T              t_cond
)
{
    INT32                       i4_rc;
    DSM_HL_PARSE_DEV_DATA_T     t_parse_data;
    
    /*
      Set  up the data to call client's notification.
    */
    t_parse_data.h_dev_comp = pt_dsm_comp->h_rm_hdle;
    t_parse_data.e_cond = t_cond;
    t_parse_data.pv_tag = 0;
    t_parse_data.ui4_data_1 = 0;
    t_parse_data.ui4_data_2 = 0;
    /*
      call client's notification function.
    */
    i4_rc = dsm_dev_notify_all(pt_dsm_comp, &t_parse_data);
    
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name:   dsm_avc_close_combi_comp
 *
 * Description: Free the resource for the AVC combi component.
 *
 * Input -
 *    pt_dsm_comp:  Pointer to AVC combi structure..
 *
 * Output -
 *    None.
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32  dsm_avc_close_combi_comp
(
    DSM_COMP_T*                 pt_dsm_comp
)
{
    DSM_AVC_COMBI_T*            pt_avc_comp = NULL;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_rc;
    
    i4_rc = dsm_avc_delink_comp_obj(pt_dsm_comp);

    if ( i4_rc == DSMR_OK )
    {
        x_handle_free(pt_dsm_comp->h_this_dsm_comp);
        
        pt_avc_comp = (DSM_AVC_COMBI_T*) pt_dsm_comp->pv_private ;

        /*
          close the rm handle to TV decoder and AVC combi device.
        */
        i4_rc = rm_disconnect(pt_avc_comp->h_rm_tv_decoder,
                              RM_DISC_TYPE_IGNORE,
                              NULL,
                              0,
                              &e_rm_cond);
        
        i4_rc = rm_close(pt_avc_comp->h_rm_tv_decoder, FALSE, &e_rm_cond);
        
        i4_rc = rm_close(pt_dsm_comp->h_rm_hdle, FALSE, &e_rm_cond);
    
        /*
          For AVC Combi, close the pipeline for the
          AVC combi to the TV decoder.
        */
        i4_rc = rm_close(pt_avc_comp->h_rm_pipe, FALSE, &e_rm_cond);

        /*
          free the AVC combi structure.
        */
        x_mem_free(pt_dsm_comp->pv_private);

        handle_link_init( &(pt_dsm_comp->t_link));
        
        x_mem_free(pt_dsm_comp);

    }
    return i4_rc;
}

