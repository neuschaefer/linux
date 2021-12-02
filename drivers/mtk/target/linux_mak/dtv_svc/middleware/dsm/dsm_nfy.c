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
 * $RCSfile: dsm_nfy.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This header file contains DSM function to notify client of
 *    device status change.
 *
 *---------------------------------------------------------------------------*/
#include "dsm/_dsm.h"
#include "dsm/dsm_nfy.h"

/*--------------------------------------------------------------------------
 * Name: dsm_handle_parse_dev_nfy
 *
 * Description - This API is called by the handle_parse function to invoke each
 *               client object's notification function associated with a device
 *               component.
 *
 * Input  - 
 *    ui2_count      Contains the current count, starting at '0'.
 *    ui2_max_count  Contains the maximum count value.
 *    h_elem         Contains the handle referencing an client object.
 *    e_type         Contains the handle type for the handle being travers..
 *    pv_obj         Contains the pointer references client handle control block.
 *    pv_tag         Contains the tag value stored in client handle control block.
 *    pv_parse_data  Contains ref to the private data passed to this function
 *                   by the caller of handle_parse()
 *
 * Output - None
 *
 * Return - 
 *    HP_NEXT:       goto next handle in the handle_link() structure.
 *    HP_BREAK:      done parsing the handle_list.
 --------------------------------------------------------------------------*/
HP_TYPE_T    dsm_handle_parse_dev_nfy
(
    UINT16         ui2_count,
    UINT16         ui2_max_count,
    HANDLE_T       h_elem,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    VOID*          pv_parse_data   
)
{
    HP_TYPE_T                 e_return;
    DSM_CTRL_BLK_T*           pt_co;
    DSM_HL_PARSE_DEV_DATA_T*  pt_parse_data;
    BOOL                      b_sent_nfy ;

    b_sent_nfy = TRUE ;
    e_return = HP_NEXT;

    if (
        (e_type == DSMT_DEVICE_T)                       &&
        ((pt_co = ((DSM_CTRL_BLK_T*) pv_obj)) != NULL)  &&
        (pt_co->u.pf_dev_nfy != NULL)
        )
    {
        pt_parse_data = (DSM_HL_PARSE_DEV_DATA_T*) pv_parse_data;

        /*
          Invoke the callback function only if this client
          handle is not the originator of the handle_parse
          call.
        */
        if ( h_elem != pt_parse_data->h_dev_comp )
        {
            /*
              Check if the client has enable signal monitoring
              to receive notification of audio/video signal change.
            */
            if ( pt_co->b_mon_enabled == FALSE )
            {
                b_sent_nfy = FALSE;
            }
            else
            {
                b_sent_nfy = TRUE;
            }

            /*
               If notify condition is monitoring has started (e.g., notification
               originates from the TV decoder), check if the signal notification
               needs to be sent to client to notify the initial signal status.
               Only send notification to client if it has not already
               been notified that signal monitoring has started.
            */
            if ( pt_parse_data->e_cond == DSM_COND_DEV_MONITOR_START )
            {
                if ( pt_co->b_mon_enabled == TRUE  )
                {
                    if ( pt_co->b_send_nfy_once == TRUE )
                    {
                        b_sent_nfy = TRUE;
                        pt_co->b_send_nfy_once = FALSE;
                    }
                    else
                    {
                        b_sent_nfy = FALSE;
                    }
                }
            }

            /*
              If notify condition is monitoring stop  (the monitor
              stop come from the Driver layer), must remember to reset the
              send notify once to TRUE, otherwise the next monitor start
              notification will not be sent. (CR00004022) because
              the 'b_send_nfy_once' flag is FALSE.
            */
            if ( pt_parse_data->e_cond == DSM_COND_DEV_MONITOR_STOP )
            {
                if ( pt_co->b_mon_enabled == TRUE )
                {
                    pt_co->b_send_nfy_once = TRUE ;
                }
            }       
            
            if ( b_sent_nfy == TRUE                 &&
                 pt_co->u.pf_dev_nfy != NULL )
            {
                DBG_INFO(("{DSM} calling client device notify function-> cond: %d\n",
                         pt_parse_data->e_cond));
                
                /* invoke the client object notification function. */
                pt_co->u.pf_dev_nfy(h_elem,
                                    pt_parse_data->e_cond,
                                    pv_tag,
                                    pt_parse_data->ui4_data_1,
                                    pt_parse_data->ui4_data_2);

#if 0                       
                /*
                  If notify condition is DSM_COND_DEV_MONITOR_START,
                  send the signal status as well.
                */
                if ( pt_parse_data->e_cond == DSM_COND_DEV_MONITOR_START )
                {
                    SIZE_T     z_size;
                    UINT32     ui4_signal;

                    z_size = sizeof(UINT32);
                    if ( x_dsm_dev_get(pt_co->h_client,
                                  DSM_GET_AVC_VIDEO_SIG_STATUS,
                                  NULL,
                                  &z_size,
                                       &ui4_signal) == DSMR_OK )
                    {
                        pt_co->u.pf_dev_nfy(h_elem,
                                            DSM_COND_DEV_VIDEO_SIG_STATUS,
                                            pv_tag,
                                            ui4_signal,
                                            ui4_signal);
                    }

                    z_size = sizeof(UINT32);
                    if ( x_dsm_dev_get(pt_co->h_client,
                                  DSM_GET_AVC_VIDEO_SIG_STATUS,
                                  NULL,
                                  &z_size,
                                  &ui4_signal) == DSMR_OK )
                    {
                        pt_co->u.pf_dev_nfy(h_elem,
                                            DSM_COND_DEV_AUDIO_SIG_STATUS,
                                            pv_tag,
                                            ui4_signal,
                                            ui4_signal);
                    }
                }
#endif
                
            }
            else
            {
                DBG_INFO(( "dsm_handle_parse_dev_nfy() b_sent_nfy != TRUE or pt_co->u.pf_dev_nfy == NULL \r\n" ));
            }
        }
        else
        {
            DBG_INFO(( "dsm_handle_parse_dev_nfy() h_elem == pt_parse_data->h_dev_comp\r\n" ));
        }
    }

    return (e_return);
}

/*--------------------------------------------------------------------------
 * Name: dsm_cond_code_convert
 *
 * Description - API that maps the RM notification condition and
 *               reason to the DSM notification condition and reason.
 *
 * Input  - 
 *    pt_data:   pointer to the component data structure.
 *    ui4_idx:   index in the component array for this component.  
 *    pf_action: client provided function.
 *    pv_tag:  Tag value to pass to client provided action function.
 *
 * Output - None
 *
 * Return
 *    DSMR_OK:   all element are visited and client action is apply to it.
 *
 --------------------------------------------------------------------------*/
INT32 dsm_cond_code_convert
(
    DSM_COMP_T*           pt_data,
    UINT32                ui4_idx,
    dsm_comp_action_fct   pf_action,
    VOID*                 pv_tag
)
{
    INT32          i4_rc = DSMR_OK;



    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_dev_notify
 *
 * Description - API that calls the notification of each client handle for
 *               to the device.
 *
 * Input  - 
 *    pt_data:   pointer to the component data structure.
 *    ui4_idx:   index in the component array for this component.  
 *    pf_action: client provided function.
 *    pv_tag:  Tag value to pass to client provided action function.
 *
 * Output - None|
 *
 * Return
 *    DSMR_OK:   all element are visited and client action is apply to it.
 *
 --------------------------------------------------------------------------*/
INT32 dsm_dev_notify_all
(
    DSM_COMP_T*               pt_dsm_comp,
    DSM_HL_PARSE_DEV_DATA_T*  pt_parse_data
)
{
    INT32                    i4_rc;

    i4_rc = DSMR_FAIL;
    
    if (  pt_parse_data != NULL )
    {
        
        /* protect the call to handle_parse function. */
        DSM_GRAB_SEMA();

        /*
          interate using handle parse to send notification to DSM handle.
        */
        handle_parse(&(pt_dsm_comp->t_link),
                     dsm_handle_parse_dev_nfy,
                     pt_parse_data);

        /* Don't forget to unlock */
        DSM_UNGRAB_SEMA();

        i4_rc = DSMR_OK;
    }
    return i4_rc;
}


