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
 * $RCSfile: prc_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/2 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: e676eb15b9132c1dfd58e19f4f5fecc1 $
 *
 * Description: 
 *         PM internal utilities 
 * History:
 *                
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "prc_mngr/prc_mngr.h"
#include "prc_mngr/prc_common.h"
#include "prc_mngr/prc_lock.h"
#include "prc_mngr/prc_util.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: pmi_valid_request
 *
 * Description: This API is responsible to check the request is valid for the 
 *              current status of ES.
 *
 * Inputs:  pt_esdb_node     Contains the reference to the ESDB node.
 *          t_new_state      Contains the new request for state transition.
 * Outputs: -
 *
 * Returns: TRUE             Valid Request
 *          FALSE            Invalid Request
 -----------------------------------------------------------------------------*/     
BOOL pmi_valid_request(PM_COMPONENT_INFO_T* pt_esdb_node, PM_ES_STATE_T t_new_state)
{
    RM_COND_T   t_open_cond;
    RM_COND_T   t_conn_cond;
    
    if (pt_esdb_node != NULL)
    {
        t_open_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
        t_conn_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);
        switch (t_new_state)
        {
            case PM_ES_STATE_WAIT_OPEN:
                if (t_open_cond == RM_COND_IGNORE)
                    return TRUE;
                else
                    return FALSE;
                    
            case PM_ES_STATE_WAIT_CONNECT:
                switch (t_open_cond)
                {
                    case RM_COND_CLOSED:
                    case RM_COND_CLOSING:
                        return FALSE;
                        
                    case RM_COND_OPENED:
                        switch (t_conn_cond)
                        {
                            case RM_COND_DISCONNECTED:
                            case RM_COND_DISCONNECTING:                            
                                return TRUE;
                                
                            case RM_COND_CONNECTING:
                            case RM_COND_CONNECTED:
                            case RM_COND_HARD_WIRED:
                            case RM_COND_MULTI_PORT:
                            default:
                                return FALSE;
                        }  
                        
                    case RM_COND_OPENING:
                        return TRUE;                        
                    default:
                        return FALSE;
                }
                
            case PM_ES_STATE_WAIT_PLAY:
            case PM_ES_STATE_WAIT_STOP:
            case PM_ES_STATE_WAIT_DISCONNECT:
                switch (t_open_cond)
                {
                    case RM_COND_CLOSED:
                    case RM_COND_CLOSING:
                        return FALSE;
                        
                    case RM_COND_OPENED:                     
                    case RM_COND_OPENING:
                        return TRUE;      
                                          
                    default:
                        return FALSE;
                }
                  
            case PM_ES_STATE_WAIT_CLOSE:
                switch (t_open_cond)
                {
                    case RM_COND_CLOSED:                        
                    case RM_COND_CLOSING:
                        return FALSE;
                        
                    case RM_COND_OPENED:                      
                    case RM_COND_OPENING:     
                        return TRUE;           
                    default:
                        return FALSE;
                }
            default:
                return FALSE;
        }
    }
    else
        return FALSE;
}      

/*-----------------------------------------------------------------------------
 * Name: pmi_execute_request_now
 *
 * Description: This API is responsible to check the request could be executed 
 *              immediatelynow or not.
 *
 * Inputs:  pt_esdb_node     Contains the reference to the ESDB node.
 *          t_new_state      Contains the new request for state transition.
 * Outputs: -
 *
 * Returns: TRUE             Valid Request
 *          FALSE            Invalid Request
 -----------------------------------------------------------------------------*/  
BOOL pmi_execute_request_now(PM_COMPONENT_INFO_T* pt_esdb_node, PM_ES_STATE_T t_new_state)
{
    RM_COND_T   t_open_cond;
    RM_COND_T   t_conn_cond;
    
    if (pt_esdb_node != NULL)
    {
        t_open_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
        t_conn_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);
        switch (t_new_state)
        {
            case PM_ES_STATE_WAIT_OPEN:
                if (t_open_cond == RM_COND_IGNORE)
                    return TRUE;
                else
                    return FALSE;
                    
            case PM_ES_STATE_WAIT_CONNECT:
                if ((t_open_cond == RM_COND_OPENED) && (t_conn_cond == RM_COND_DISCONNECTED))
                    return TRUE;
                else
                    return FALSE;
                    
            case PM_ES_STATE_WAIT_PLAY:
            case PM_ES_STATE_WAIT_STOP:
            case PM_ES_STATE_WAIT_DISCONNECT:
                if ((t_open_cond == RM_COND_OPENED) && ((t_conn_cond == RM_COND_CONNECTED) || (t_conn_cond == RM_COND_HARD_WIRED)))
                    return TRUE;
                else
                    return FALSE;
                                                    
            case PM_ES_STATE_WAIT_CLOSE:
                if (t_open_cond == RM_COND_OPENED)
                    return TRUE;
                else
                    return FALSE;
            default:
                return FALSE;
        }
    }
    else
        return FALSE;
}               

/*-----------------------------------------------------------------------------
 * Name: pmi_speed_valid
 *
 * Description: This API is responsible to check the speed.
 *
 * Inputs:  pt_svdb_node     Contains the reference to service node. 
 *          i4_speed        Contains the speed value.
 * Outputs: -
 *
 * Returns: TRUE           Valid
 *          FALSE          Invalid
 ----------------------------------------------------------------------------*/   
BOOL pmi_speed_valid(PM_SERVICE_INFO_T *pt_svdb_node, INT32 i4_speed)
{
    if (pt_svdb_node != NULL)
    {
        switch (SVNODE_PROC_TYPE(pt_svdb_node))
        {
            case PM_PRESENT:
            case PM_DETECTION:
        #if (PM_ENABLE_MULTIMEDIA)            
            #if (PM_ENABLE_RECORD)
            case PM_RECORDING:
            #endif
            case PM_MULTIMEDIA:
        #endif
                /* ToDo: check */
                return TRUE;                
            default:
                /* ToDo: Other mode */
                break;                                
        }
    }
    
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: pmi_streamtype_valid
 *
 * Description: This API is responsible to check the stream type.
 *
 * Inputs:  t_stream_type    Contains the stream type. 
 * Outputs: -
 *
 * Returns: TRUE           Valid
 *          FALSE          Invalid
 ----------------------------------------------------------------------------*/   
BOOL pmi_streamtype_valid(STREAM_TYPE_T t_stream_type)
{
    switch (t_stream_type)
    {
       case ST_AUDIO:
       case ST_VIDEO:
       case ST_CLOSED_CAPTION:
       case ST_TELETEXT:
       case ST_SUBTITLE:
       case ST_DATA:
            return TRUE;                
       default:
            return FALSE;                       
       
    }
}
#if (PM_ENABLE_MULTIMEDIA)
/*-----------------------------------------------------------------------------
 * Name: pmi_streamidtype_valid
 *
 * Description: This API is used to validate the stream ID type.
 *
 * Inputs:  t_stream_type    Contains the stream type to be validated. 
 * Outputs: -
 *
 * Returns: TRUE           Valid
 *          FALSE          Invalid
 ----------------------------------------------------------------------------*/
BOOL pmi_streamidtype_valid(STREAM_ID_TYPE_T t_stream_id_type)
{
    switch (t_stream_id_type)
    {
        case STREAM_ID_TYPE_UNKNOWN:
            return FALSE;
            
        case STREAM_ID_TYPE_AVI:
        case STREAM_ID_TYPE_ASF:
        case STREAM_ID_TYPE_MP4:
        case STREAM_ID_TYPE_MKV:
        case STREAM_ID_TYPE_PS:
        case STREAM_ID_TYPE_TS:
        case STREAM_ID_TYPE_RM:
        case STREAM_ID_TYPE_ES:                
        case STREAM_ID_TYPE_OGG:
            return TRUE;

        default:
            return FALSE;
    }     
}
#endif
