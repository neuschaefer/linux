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
 * $RCSfile: dsm_avc_scart.c,v $
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
#include "dsm/dsm_avc_scart.h"
#include "dsm/dsm_nfy.h"
#include "res_mngr/drv/x_tv_dec.h"
#include "res_mngr/drv/x_avc.h"


/*--------------------------------------------------------------------------
 * Name:   avc_scart_drv_nfy_fct
 *
 * Description: This notification function called by AVC SCART driver.
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
static  VOID  avc_scart_drv_nfy_fct
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
            if ( e_nfy_cond == AVC_COND_SCART_PIN_8_STATUS ||
                 e_nfy_cond == AVC_COND_SCART_PIN_16_STATUS
                )
            {
                DBG_API(("{DSM} receive AVC SCART driver notification: "
                         "AVC_COND_SCART_PIN8_STATUS. "
                         "data_1:  %d  data_2: %d\n",
                         ui4_data_1, ui4_data_2));
            
                /*  send msg to DSM thread. */
                t_msg.e_msg_code = DSM_MSG_AVC_SCART_DRVT_NFY;
                t_msg.h_dsm_comp = h_dsm_comp ;
                t_msg.u.t_avc_scart.e_nfy_cond = e_nfy_cond;
                t_msg.u.t_avc_scart.ui4_data_1 = ui4_data_1;
                t_msg.u.t_avc_scart.ui4_data_2 = ui4_data_2;
                dsm_send_msg(&t_msg);
            }
            else 
            {
                DBG_API(("{DSM} receive AVC SCART driver notification: "
                         "[%d], this is ignored.\n", e_nfy_cond));
            }
        }
    }
}

/*--------------------------------------------------------------------------
 * Name:   avc_scart_pin_8_status_to_dsm_status
 *
 * Description: Convert AVC SCART driver PIN-8 status to DSM status.              
 *
 * Input -
 *    t_pin_8_status:   SCART PIN-8 status
 *    
 * Output -
 *    None
 *
 * Return -
 *    DSM video status
 *   
 --------------------------------------------------------------------------*/
static  UINT32  avc_scart_pin_8_status_to_dsm_status
(
    AVC_SCART_PIN_8_T         t_pin_8_status
)
{
    UINT32    ui4_dsm_status;
    
    switch (t_pin_8_status)
    {
        case AVC_SCART_PIN_8_UNKNOWN:
        {
            ui4_dsm_status = DSM_VIDEO_SIGNAL_UNKNOWN;
        }
        break;

        case AVC_SCART_PIN_8_NO_SIGNAL:
        {
            ui4_dsm_status = DSM_VIDEO_SIGNAL_OFF;
        }
        break;

        case AVC_SCART_PIN_8_ASPECT_16_9:
        {
            ui4_dsm_status = (UINT32) (DSM_VIDEO_SIGNAL_ASPECT_RATIO_16_9 |
                                       DSM_VIDEO_SIGNAL_ON );
            
        }
        break;
        
        case AVC_SCART_PIN_8_ASPECT_4_3 :
        {
            ui4_dsm_status = (UINT32) (DSM_VIDEO_SIGNAL_ASPECT_RATIO_4_3  |
                                       DSM_VIDEO_SIGNAL_ON );
        }
        break;

        default:
        {
            ui4_dsm_status = DSM_VIDEO_SIGNAL_UNKNOWN;
        }
        break;
    }
    return ui4_dsm_status;
}

#if 0
/*--------------------------------------------------------------------------
 * Name:   avc_scart_pin_16_status_to_dsm_status
 *
 * Description: Convert AVC SCART driver PIN-16 status to DSM video type.          
 *
 * Input -
 *    t_pin_16_status:   SCART PIN-16 status
 *    
 * Output -
 *    None
 *
 * Return -
 *    DSM video type
 *   
 --------------------------------------------------------------------------*/
static  UINT32  avc_scart_pin_16_status_to_dsm_status
(
    AVC_SCART_PIN_16_T         t_pin_16_status
)
{
    UINT32    ui4_dsm_status;
    
    switch (t_pin_16_status)
    {
        case AVC_SCART_PIN_16_COMPOSITE:
        {
             ui4_dsm_status = DSM_VIDEO_TYPE_COMPOSITE;
        }
        break;

        case AVC_SCART_PIN_16_RGB:
        {
             ui4_dsm_status = DSM_VIDEO_TYPE_RGB;
        }
        break;

        case AVC_SCART_PIN_16_TOGGLE:
        {
             ui4_dsm_status= DSM_VIDEO_TYPE_MIXED;
        }
        break;

        default:
        {
            ui4_dsm_status = DSM_VIEDO_TYPE_UNKNOWN;
        }
        break;
    }
    return ui4_dsm_status;
}
#endif
/*--------------------------------------------------------------------------
 * Name:   avc_scart_input_mode_dsm_video_type
 *
 * Description: Convert AVC SCART driver input mode to DSM video type.
 *
 * Input -
 *    t_input_mode:   SCART driver input mode.
 *    
 * Output -
 *    None
 *
 * Return -
 *    DSM video type
 *   
 --------------------------------------------------------------------------*/
static  UINT32  avc_scart_input_mode_dsm_video_type
(
    AVC_SCART_INPUT_MODE_T         t_input_mode
)
{
    UINT32    ui4_dsm_video_type;
    
    switch (t_input_mode)
    {
        case AVC_SCART_INPUT_MODE_CVBS:
        {
             ui4_dsm_video_type = DSM_VIDEO_TYPE_COMPOSITE;
        }
        break;

        case AVC_SCART_INPUT_MODE_SV:
        {
             ui4_dsm_video_type = DSM_VIDEO_TYPE_S_VIDEO;
        }
        break;

        case AVC_SCART_INPUT_MODE_RGB:
        {
             ui4_dsm_video_type= DSM_VIDEO_TYPE_RGB;
        }
        break;

        case AVC_SCART_INPUT_MODE_MIX:
        {
             ui4_dsm_video_type= DSM_VIDEO_TYPE_MIXED;
        }
        break;

        default:
        {
            ui4_dsm_video_type = DSM_VIEDO_TYPE_UNKNOWN;
        }
        break;
    }
    return ui4_dsm_video_type;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_scart_get_signal_status
 *
 * Description: This API query the signal status (PIN-8 and PIN-16 status) for
 *              SCART device.
 *
 * Input -
 *    pt_dsm_obj:  pointer to a DSM component structure.
 *    
 *    pz_size      Contains the length of buffer which is referenced
 *                 by pv_data.
 *
 * Output -
 *    pui4_video_signal:   Contains the video signal.
 *    pui4_video_type:     contains data return for this 'get' operation.
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   
 --------------------------------------------------------------------------*/
static INT32   dsm_avc_scart_get_signal_status
(
    DSM_COMP_T*         pt_dsm_obj,
    UINT32*             pui4_video_signal,
    UINT32*             pui4_video_type
)
{
    INT32                    i4_rc;
    SIZE_T                   z_size;
    AVC_SCART_PIN_8_T        t_pin_8_status;
    /* 
    AVC_SCART_PIN_16_T       t_pin_16_status;
    */
    AVC_SCART_INPUT_MODE_T   t_input_mode;
    RM_COND_T                e_cond;

    *pui4_video_signal = DSM_VIDEO_SIGNAL_UNKNOWN;
    *pui4_video_type   = DSM_VIEDO_TYPE_UNKNOWN;
    
    /*
      Get SCART PIN-8 status
    */
    z_size = sizeof(AVC_SCART_PIN_8_T);

    i4_rc = rm_get(pt_dsm_obj->h_rm_hdle,
                   FALSE,
                   ANY_PORT_NUM,
                   AVC_GET_TYPE_SCART_PIN_8_STATUS,
                   (VOID*)&t_pin_8_status,
                   &z_size,
                   &e_cond);

    if ( i4_rc == RMR_OK )
    {
        /*
          Convert the PIN-8 and PIN-16 status value to
          video type and aspect ratio.
        */
        *pui4_video_signal =
            avc_scart_pin_8_status_to_dsm_status(t_pin_8_status);
    }
    else
    {
        DBG_ERROR(("{DSM} rm_get(AVC_GET_TYPE_SCART_PIN_8_STATUS) "
                   "failed. RM error code: %d\n", i4_rc));
    }


    /*
      IF the video signal is on, then read the SCART
      input mode, which indicates video signal type.
    */
    if ( (*pui4_video_signal) & DSM_VIDEO_SIGNAL_ON )
    {
        /*
          new method, use SCART input mode to get the
          video type.
        */
        z_size = sizeof(AVC_SCART_INPUT_MODE_T);
        i4_rc = rm_get(pt_dsm_obj->h_rm_hdle,
                   FALSE,
                   ANY_PORT_NUM,
                   AVC_GET_TYPE_SCART_INPUT_MODE,
                   (VOID*)&t_input_mode,
                   &z_size,
                   &e_cond);

        /*
          old method is to get SCART PIN 16
          status, which indicates video signal type.
        */
        /*
        z_size = sizeof(AVC_SCART_PIN_16_T);
        i4_rc = rm_get(pt_dsm_obj->h_rm_hdle,
                   FALSE,
                   ANY_PORT_NUM,
                   AVC_GET_TYPE_SCART_PIN_16_STATUS,
                   (VOID*)&t_pin_16_status,
                   &z_size,
                   &e_cond);
        */

        if ( i4_rc == RMR_OK )
        {
            /*
            *pui4_video_type =
                avc_scart_pin_16_status_to_dsm_status(t_pin_16_status);
            */

            *pui4_video_type =
                avc_scart_input_mode_dsm_video_type(t_input_mode);
            
        }
        else
        {
            DBG_ERROR(("{DSM} rm_get(AVC_GET_TYPE_SCART_INPUT_MODE) "
                       "failed. RM error code: %d\n", i4_rc));
        }
    }
    i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
        
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_scart_get_ops
 *
 * Description: This API executed specific 'GET' operation on AVC SCART
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
INT32   dsm_avc_scart_get_ops
(
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_GET_T    e_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{
    INT32            i4_rc;
    UINT32           ui4_video_signal;
    UINT32           ui4_video_type;
    
    /*
       Convert the get type and set up the request structure to
       call the RM_GET() function.
    */
    switch (e_get_type )
    {
        case DSM_GET_AVC_VIDEO_SIG_STATUS:
        {
            /*
               Arguments checking:
      
              'pz size' should 8 bytes.
              'pv_data' is a ref to UINT32[2].  
            */
            if ( pz_size          != NULL                &&
                 ( (*pz_size)==8  || (*pz_size)==4 )     &&
                 pv_data          != NULL
                )
            {
                /*
                   Get the SCART PIN-8 and PIN-16 status. Use this
                   status to determine the type of video signal and
                   format.
                */
                i4_rc = dsm_avc_scart_get_signal_status(
                    pt_dsm_obj, &ui4_video_signal, &ui4_video_type);

                if ( i4_rc == DSMR_OK )
                {
                    /*
                      copy the video signal status to user buffer.
                    */
                    (((UINT32*)pv_data)[0]) = ui4_video_signal ;

                    /*
                      copy the video type to the user buffer.
                    */
                    if ( *pz_size == 8 )
                    {
                        (((UINT32*)pv_data)[1]) = ui4_video_type ;
                    }
                }
            }
            else
            {
                return DSMR_INV_ARG;
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
 * Name: dsm_avc_scart_set_ops
 *
 * Description: This API executed specific 'SET' operation on AVC SCART
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
INT32   dsm_avc_scart_set_ops
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
              Nothing to perform, because SCART notification function is
              already registered. 
            */
            i4_rc = DSMR_OK;
        }
        break;

        case DSM_SET_AVC_STOP_MONITOR:
        {
            /*
               No action to perform here. The stop monitoring is
               stored on a per client structure.  The driver
               notification continue to be received at the layer
               between AVC SCART driver component and DSM layer, but
               notification is not pass up to the client.
            */
            i4_rc = DSMR_OK;
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
 * Name: dsm_avc_scart_post_open_action
 *
 * Description: This function is executed after an AVC SCART component
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
INT32   dsm_avc_scart_post_open_action
(
    DSM_COMP_T*      pt_dsm_comp
)
{
    INT32            i4_rc;
    DSM_AVC_SCART_T* pt_avc_comp = NULL;
    BOOL             b_flag;
    
    i4_rc = DSMR_FAIL;
    
    if ( pt_dsm_comp->t_dev_type_name == DEV_AVC_SCART   &&
         pt_dsm_comp->h_rm_hdle   != NULL_HANDLE
        )
    {
        /*
          Initialize the private data for the AVC SCART device If the
          AVC COMBIN specific data has not been initialized,
        */
        if ( pt_dsm_comp->pv_private == NULL )
        {
            /*
              create a AVC SCART specific structure to store
              AVC SCART specific information.
            */
            pt_avc_comp = (DSM_AVC_SCART_T*)
                x_mem_alloc(sizeof(DSM_AVC_SCART_T));

            if ( pt_avc_comp != NULL )
            {
                SIZE_T             z_size;
                RM_COND_T          e_cond;
                AVC_NFY_INFO_T     t_avc_nfy;

                pt_dsm_comp->pv_private = pt_avc_comp ;
                
                pt_avc_comp->ui4_video_signal = DSM_VIDEO_SIGNAL_UNKNOWN;
                pt_avc_comp->ui4_video_type = DSM_VIEDO_TYPE_UNKNOWN;
                pt_avc_comp->b_need_tv_decoder = FALSE;
                pt_avc_comp->b_need_audio_decoder = FALSE;
                pt_avc_comp->b_audio_sig_set  = FALSE;
                pt_avc_comp->b_video_sig_set  = FALSE;

                /*
                  Set the AVC notification function.
                  and flag value. The initial default
                  is not to notify the client.
                */

                t_avc_nfy.pv_tag     = (VOID*)pt_dsm_comp->h_this_dsm_comp;
                t_avc_nfy.pf_avc_nfy = avc_scart_drv_nfy_fct;
                z_size = sizeof(AVC_NFY_INFO_T);
                
                /*
                  Register AVC SCART notification fct with
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
                          Set the AVC SCART connection option:
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
                            DBG_API(("{DSM} AVC SCART component's ( %d) "
                                     "private data initialized.\n",
                                     pt_dsm_comp->h_rm_hdle));
                        }
                        else
                        {
                            DBG_ERROR(("{DSM} Error: AVC SCART set AVC_SET_TYPE_NO_CONN_DISC_NFY "
                                       "failed, RM error code: %d\n", i4_rc));
                                
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
                    DBG_ERROR(("{DSM} Error: register notify function with SCART driver failed, "
                               "RM error code: %d\n", i4_rc));
                    
                    i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
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
            DBG_API(("{DSM} AVC SCART component's (rm handle: %d) "
                     "private data already initialized.\n",
                     pt_dsm_comp->h_rm_hdle));
        }
    }
    return i4_rc;
}


/*--------------------------------------------------------------------------
 * Name:   dsm_avc_scart_proc_client_nfy
 *
 * Description:
 *   This notification function is called in the DSM thread
 *   context to process SCART driver notification.
 *
 *   SCART driver notification condition and data are:
 *
 *   condition are: AVC_COND_SCART_PIN_8_STATUS
 *                  AVC_COND_SCART_PIN_16_STATUS
 *
 *   data 1:  For AVC_COND_SCART_PIN_8_STATUS, data_1 value are:
 *
 *                  AVC_SCART_PIN_8_UNKNOWN = 0,
 *                  AVC_SCART_PIN_8_NO_SIGNAL,    
 *                  AVC_SCART_PIN_8_ASPECT_16_9,
 *                  AVC_SCART_PIN_8_ASPECT_4_3
 *
 *            For AVC_COND_SCART_PIN_16_STATUS, data_1 value are:
 *
 *                  AVC_SCART_PIN_16_UNKNOWN
 *                  AVC_SCART_PIN_16_COMPOSITE
 *                  AVC_SCART_PIN_16_RGB
 *                  AVC_SCART_PIN_16_TOGGLE
 *                                                           
 *   data 2: 0
 *
 *
 * Input -
 *    pv_data:  Pointer to DSM message structure contains AVC SCART
 *              driver notification data.
 *
 * Output -
 *    None.
 *
 * Return -
 *   DSMR_OK:   process signal notify operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32  dsm_avc_scart_proc_client_nfy
(
    VOID*           pv_data
)
{
    INT32                       i4_rc;
    DSM_MSG_T*                  pt_msg;
    DSM_HL_PARSE_DEV_DATA_T     t_parse_data;
    SIZE_T                      z_size;
    UINT32                      ui4_data_1;
    DSM_COMP_T*                 pt_dsm_comp = NULL;
    DSM_AVC_SCART_T*            pt_avc_scart = NULL;
    DSM_AVC_SCART_NFY_DATA_T*   pt_avc_drv_data;
    BOOL                        b_vid_notify ;

    b_vid_notify = FALSE;
    i4_rc = DSMR_OK;
    pt_msg = (DSM_MSG_T*) pv_data;

    pt_dsm_comp = dsm_get_ref_to_comp(pt_msg->h_dsm_comp) ;
    if ( pt_dsm_comp == NULL )
    {
        return DSMR_FAIL;
    }
    
    pt_avc_scart = (DSM_AVC_SCART_T*) pt_dsm_comp->pv_private ;
    pt_avc_drv_data = &(pt_msg->u.t_avc_scart);
    ui4_data_1 = pt_avc_drv_data->ui4_data_1 ;
    /*
      SCART Driver notification condition and data:
    */
    if ( pt_avc_drv_data->e_nfy_cond == AVC_COND_SCART_PIN_8_STATUS )
    {
        if ( ui4_data_1 == AVC_SCART_PIN_8_UNKNOWN )
        {
            DBG_ERROR(("{DSM} receive AVC SCART video signal notification, "
                       "but notification indicates signal unknown: %d\n",
                       ui4_data_1));
            return DSMR_FAIL;
        }
        else
        {
            /*
              process Video signal notify, determine if the video signal has
              changed since last notification.
            */
            if ( pt_avc_scart->b_video_sig_set == FALSE )
            {
                b_vid_notify = TRUE;
            }
            else
            {
                UINT32  ui4_old_status ;
                UINT32  ui4_new_status ;

                ui4_old_status = pt_avc_scart->ui4_video_signal ;
                ui4_new_status = avc_scart_pin_8_status_to_dsm_status(
                    (AVC_SCART_PIN_8_T) ui4_data_1);
        
                if (  ui4_old_status != ui4_new_status )
                {
                    b_vid_notify = TRUE;
                }
            }
        }
    }
    
    if ( b_vid_notify == TRUE ) 
    {
        UINT32    aui4_data[2] ;
        
        z_size=8;
        
        i4_rc = dsm_avc_scart_get_ops(pt_dsm_comp,
                                      DSM_GET_AVC_VIDEO_SIG_STATUS,
                                      NULL,
                                      &z_size,
                                      aui4_data);

        pt_avc_scart->b_video_sig_set   = TRUE;
        pt_avc_scart->ui4_video_signal  = aui4_data[0];
        pt_avc_scart->ui4_video_type    = aui4_data[1];
        
        /*
          Set  up the data to call client's notification.
        */
        t_parse_data.h_dev_comp = pt_dsm_comp->h_rm_hdle;
        t_parse_data.e_cond = DSM_COND_DEV_VIDEO_SIG_STATUS;
        t_parse_data.pv_tag = 0;
        t_parse_data.ui4_data_1 = pt_avc_scart->ui4_video_signal;
        t_parse_data.ui4_data_2 = pt_avc_scart->ui4_video_type;
        /*
          call client's notification function.
        */
        DBG_API(("{DSM} send scart driver notify to client, video signal: %d "
                 "video type: %d\n",
                 t_parse_data.ui4_data_1,t_parse_data.ui4_data_2));
        
        dsm_dev_notify_all(pt_dsm_comp, &t_parse_data);
    }
    
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name:   dsm_avc_close_scart_comp
 *
 * Description: Free the resource for the AVC scart component.
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
INT32  dsm_avc_close_scart_comp
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

