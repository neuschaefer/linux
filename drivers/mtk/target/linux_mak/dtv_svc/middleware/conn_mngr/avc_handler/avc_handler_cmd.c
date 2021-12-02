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
 * $RCSfile: avc_handler_cmd.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains the AVC Connection Handler functions that deal
 *         with commands (validation, parsing, etc).
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "conn_mngr/avc_handler/avc_handler_cmd.h"
#include "conn_mngr/avc_handler/u_avc_handler.h"
#include "conn_mngr/avc_handler/_avc_handler.h"
#include "res_mngr/x_rm_dev_types.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define AVC_HANDLER_CONNECTOR_TYPE_CONN_MASK (  \
AVC_HANDLER_CONNECTOR_TYPE_MASK)

#define AVC_HANDLER_CONNECTOR_TYPE_OPTIONAL_MASK (  \
AVC_HANDLER_CONNECTOR_TYPE_CONN_MASK             |  \
AVC_HANDLER_TV_DECODER_NAME_MASK                 |  \
AVC_HANDLER_DISC_IF_TV_SIG_LOST_MASK             |  \
AVC_HANDLER_SCART_INP_MODE_MASK                  |  \
AVC_HANDLER_COMBI_INDEX_MASK)

#define MANDATORY_OPTIONS_OK(_options, _mask) \
(((_options) & (_mask)) == (_mask))

#define OPTIONAL_OPTIONS_OK(_options, _mask) \
(((_options) ^ ((_options) & (_mask))) == 0)


/*-----------------------------------------------------------------------------
 * Name: avc_handler_validate_parse_info
 *
 * Description: This API validates the parse information built by parsing a
 *              command set.
 *
 * Inputs:  pt_info  References the parse information.
 *
 * Outputs: -
 *
 * Returns: CMR_OK           The parse information is valid.
 *          CMR_INV_COMMAND  The command set was invalid.
 ----------------------------------------------------------------------------*/
static INT32
avc_handler_validate_parse_info (const AVC_HANDLER_CONNECT_INFO_T*  pt_info,
                                 UINT32                             ui4_flags)
{
    UINT32  ui4_mask;
    UINT32  ui4_optional_mask;
    INT32   i4_res;
    
    ui4_mask          = 0;
    ui4_optional_mask = 0;
    
    i4_res = CMR_OK;

    if (ui4_flags & AVC_HANDLER_CONNECTOR_TYPE_MASK)
    {
        ui4_mask          = AVC_HANDLER_CONNECTOR_TYPE_CONN_MASK;
        ui4_optional_mask = AVC_HANDLER_CONNECTOR_TYPE_OPTIONAL_MASK;
        
        if ((pt_info->e_device_type != DEV_AVC_COMP_VIDEO) &&
            (pt_info->e_device_type != DEV_AVC_S_VIDEO)    &&
            (pt_info->e_device_type != DEV_AVC_Y_PB_PR)    &&
            (pt_info->e_device_type != DEV_AVC_VGA)        &&
            (pt_info->e_device_type != DEV_AVC_SCART)      &&
            (pt_info->e_device_type != DEV_AVC_DVI)        &&
            (pt_info->e_device_type != DEV_AVC_HDMI)       &&
            (pt_info->e_device_type != DEV_AVC_AUDIO_INP)  &&
            (pt_info->e_device_type != DEV_AVC_SPDIF)      &&
            (pt_info->e_device_type != DEV_AVC_COMBI))
        {
            i4_res = CMR_INV_COMMAND;
        }
        else if ((pt_info->e_device_type == DEV_AVC_COMBI)
                 &&
                 ! (ui4_flags & AVC_HANDLER_COMBI_INDEX_MASK))
        {
            /* when a combi component is used, an index has to be provided */
            i4_res = CMR_INV_COMMAND;
        }
    }
    else
    {
        i4_res = CMR_INV_COMMAND;
    }
    
    if (! ((i4_res == CMR_OK)
           &&
           MANDATORY_OPTIONS_OK(ui4_flags,
                                ui4_mask)
           &&
           OPTIONAL_OPTIONS_OK(ui4_flags,
                               ui4_optional_mask)))
    {
        if (! MANDATORY_OPTIONS_OK(ui4_flags, ui4_mask))
        {
            DBG_ERROR((DBG_PREFIX"Missing mandatory option\n"));
            DBG_ERROR((DBG_PREFIX" Required: %x\n", ui4_mask));
            DBG_ERROR((DBG_PREFIX" Provided: %x\n", ui4_flags));
        }
        
        if (! OPTIONAL_OPTIONS_OK(ui4_flags, ui4_optional_mask))
        {
            DBG_ERROR((DBG_PREFIX"Too many options\n"));
            DBG_ERROR((DBG_PREFIX" Accepted: %x\n", ui4_optional_mask));
            DBG_ERROR((DBG_PREFIX" Provided: %x\n", ui4_flags));
        }
        
        i4_res = CMR_INV_COMMAND;
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_parse_commands
 *
 * Description: This API parses an array of commands and validates it.
 *
 * Inputs:  pt_commands  References a CONN_END-terminated array of commands.
 *          b_sub_conn   Specifies whether the connection is a sub-connection.
 *
 * Outputs: pt_tsl_svl_info  Contains the information about the TSL/SVL.
 *          pt_info          Contains the information about the connection.
 *          pui4_flags       Contains the command flags.
 *
 * Returns: CMR_OK           The command set is valid.
 *          CMR_INV_COMMAND  The command set is invalid.
 ----------------------------------------------------------------------------*/
INT32 avc_handler_parse_commands (AVC_HANDLER_CONNECT_INFO_T*  pt_info,
                                  UINT32*                      pui4_flags,
                                  const CM_COMMAND_T*          pt_commands)
{
    const CM_COMMAND_T*  pt_connect_cmd;
    CM_COMMAND_CODE_T    e_code;
    UINT32               ui4_flags;
    INT32                i4_res;
    
    ui4_flags = *pui4_flags;
    
    i4_res = CMR_OK;

    pt_connect_cmd = pt_commands;
    
    while (((e_code = pt_connect_cmd->e_code) != CONN_END)
           &&
           (i4_res == CMR_OK))
    {
        switch (e_code)
        {
            case CONN_SRC_NAME:
                /*pt_info->ps_src_name = pt_connect_cmd->u.ps_name;*/
                break;

            case AVC_CONN_CONNECTOR_TYPE:
                pt_info->e_device_type =
                    (DEVICE_TYPE_T) pt_connect_cmd->u.ui4_number;
                ui4_flags |= AVC_HANDLER_CONNECTOR_TYPE_MASK;
                break;
            
            case AVC_CONN_DISC_IF_COMP_BUSY:
                pt_info->b_disc_if_comp_busy = pt_connect_cmd->u.b_boolean;
                break;
                
            case AVC_CONN_DISC_IF_TV_SIG_LOST:
                pt_info->b_disc_if_tv_signal_lost =
                    pt_connect_cmd->u.b_boolean;
                ui4_flags |= AVC_HANDLER_DISC_IF_TV_SIG_LOST_MASK;
                break;
                
            case AVC_CONN_TV_DECODER_NAME:
                x_strncpy(pt_info->s_tv_decoder_name,
                          pt_connect_cmd->u.ps_name,
                          RM_NAME_LEN);
                pt_info->s_tv_decoder_name[RM_NAME_LEN] = '\0';
                ui4_flags |= AVC_HANDLER_TV_DECODER_NAME_MASK;
                break;
                
            case AVC_CONN_COMBI_INDEX:
                pt_info->ui2_combi_index = pt_connect_cmd->u.ui2_number;
                ui4_flags |= AVC_HANDLER_COMBI_INDEX_MASK;
                break;
                
            case AVC_CONN_TV_SIGNAL_TIME_OUT:
                pt_info->t_time_outs.ui4_tv_signal_time_out =
                    pt_connect_cmd->u.ui4_number;
                break;
                
            case AVC_CONN_SCART_INP_MODE:
                pt_info->e_scart_mode =
                    (AVC_HANDLER_SCART_MODE_T) pt_connect_cmd->u.ui4_number;
                ui4_flags |= AVC_HANDLER_SCART_INP_MODE_MASK;
                break;
                
            case AVC_CONN_INPUT_NO_RIGHTS_OK:
                pt_info->b_input_no_rights_ok = pt_connect_cmd->u.b_boolean;
                break;
                
            case AVC_CONN_TV_DECODER_NO_RIGHTS_OK:
                pt_info->b_tv_decoder_no_rights_ok =
                    pt_connect_cmd->u.b_boolean;
                break;
                
            default:
                /* unknown command code */
                i4_res = CMR_INV_COMMAND;
                break;
        }
        
        if (i4_res != CMR_OK)
        {
            break;
        }
        
        pt_connect_cmd++;
    }
    
    if (i4_res == CMR_OK)
    {
        *pui4_flags = ui4_flags;
        
        i4_res = avc_handler_validate_parse_info(pt_info, ui4_flags);
    }
    
    return i4_res;
}
