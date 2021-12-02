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
 * $RCSfile: _sm_vs_cmd.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/8 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: d0f65370788204bd023e2e7e671a8cbb $
 *
 * Description: 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "strm_mngr/x_sm.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/_sm_utils.h"

#include "strm_mngr/video_hdlr/x_sm_video_hdlr.h"
#include "strm_mngr/video_hdlr/_sm_video_hdlr.h"
#include "strm_mngr/video_hdlr/_sm_vs_cmd.h"

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
 * Name: _sm_vs_undo_exec_cmd
 *
 * Description: <function description>
 *
 * Inputs:  pt_vs_comp_grp References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID _sm_vs_undo_exec_cmd( SM_VS_COMP_GRP_T*  pt_vs_comp_grp )
{
    /* to-do */
}

/* subsequence commands */
/*-----------------------------------------------------------------------------
 * Name: _sm_vs_prc_sub_cmd
 *
 * Description: <function description>
 *
 * Inputs:  ppt_cmd_sets References.
 *          pt_comp_desc References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 _sm_vs_prc_sub_cmd( SM_COMMAND_T**       ppt_cmd_sets,
                          SM_DRV_COMP_DESCR_T*  pt_comp_desc )
{
    INT32                 i = 0, i4_ret;
    SM_COMMAND_T*         pt_cmd_sets = *ppt_cmd_sets;
    SM_COMMAND_T*         pt_iter = pt_cmd_sets;
    HANDLE_T              h_ref_hdl = NULL_HANDLE;

    while ( pt_iter->e_code != SM_CMD_CODE_END )
    {
        if ( (pt_iter->e_code & SM_CMD_CODE_PRIMARY_BIT_FLAG) )
        {
            *ppt_cmd_sets = pt_iter;
            break;
        }
        
        switch ( pt_iter->e_code )
        {
        case SM_VS_CMD_CODE_SET_LOGICAL_DESC:
            if ( x_strlen( pt_iter->u.ps_text) > _SM_VS_COMP_NAME_MAX_LEN )
            {
                return SMR_INV_COMMAND;    
            }
            x_strcpy( pt_comp_desc->t_info.s_logical_name, pt_iter->u.ps_text );
            break;
            
        case SM_VS_CMD_CODE_SET_HANDLE:
            h_ref_hdl = sm_drv_hdl_clone_get_hdl( pt_iter->u.h_obj );
            if ( h_ref_hdl == NULL_HANDLE )
            {
                return SMR_INV_COMMAND;    
            }
            break;
            
        case SM_VS_CMD_CODE_SET_COMP_ID:
            pt_comp_desc->t_info.ui2_comp_id = pt_iter->u.ui2_data;
            break;
            
        case SM_VS_CMD_CODE_SET_IN_PORT:
            pt_comp_desc->t_info.ui1_in_port = pt_iter->u.ui1_data;
            break;
            
        default:
            return SMR_INV_COMMAND;
        }
        
        pt_iter = &pt_cmd_sets[++i];
    }

    /* over write all other parameters if handle is setted */
    if ( h_ref_hdl != NULL_HANDLE )
    {
        DRV_TYPE_T  e_type;
        UINT8       ui1_num_inp_ports;
        UINT8       ui1_num_out_ports;
        UINT32      ui4_comp_flags;
        
        i4_ret = rm_get_comp_info_from_handle( h_ref_hdl,
                                               &e_type,
                                               &pt_comp_desc->t_info.ui2_comp_id,
                                               &ui1_num_inp_ports,
                                               &ui1_num_out_ports,
                                               &ui4_comp_flags );
        if ( i4_ret != RMR_OK || e_type != pt_comp_desc->t_info.e_type )
        {
            return SMR_INV_COMMAND;
        }

    }
    
    *ppt_cmd_sets = pt_iter;

    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_prc_prim_cmd
 *
 * Description: <function description>
 *
 * Inputs:  h_stream         References.
 *          pt_vs_comp_desc  References.
 *          ppt_cmd_sets     References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 _sm_vs_prc_prim_cmd( HANDLE_T             h_stream,
                           SM_DRV_COMP_DESCR_T* pt_vs_comp_desc,
                           SM_COMMAND_T**       ppt_cmd_sets )
{
    INT32                 i = 0, i4_ret;
    SM_COMMAND_T*         pt_cmd_sets = *ppt_cmd_sets;
    SM_COMMAND_T*         pt_iter = pt_cmd_sets;
    
    /* process primary command */
    
    
    /* process subsequence commands */
    pt_iter = &pt_cmd_sets[++i];

    i4_ret = _sm_vs_prc_sub_cmd( &pt_iter,
                                 pt_vs_comp_desc );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
    *ppt_cmd_sets = pt_iter;

    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: sm_vs_exec_cmd
 *
 * Description: <function description>
 *
 * Inputs:  h_stream       References.
 *          pt_vs_comp_grp References.
 *          pt_cmd_sets    References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_COMMAND     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_exec_cmd( HANDLE_T            h_stream,
                      SM_VIDEO_STREAM_T*  pt_vs_obj,
                      const SM_COMMAND_T* pt_cmd_sets )
{
    SM_COMMAND_T* pt_iter = (SM_COMMAND_T*)pt_cmd_sets;
    INT32         i4_ret = SMR_OK;
    
    while ( pt_iter->e_code != SM_CMD_CODE_END )
    {
        if ( !(pt_iter->e_code & SM_CMD_CODE_PRIMARY_BIT_FLAG) )
        {
            _sm_vs_undo_exec_cmd( &pt_vs_obj->t_vs_comp_grp );
            return SMR_INV_COMMAND;
        }
        
        switch ( pt_iter->e_code )
        {
        case SM_VS_CMD_CODE_SET_VIDEO_DECODER:
            if ( pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type != DRVT_UNKNOWN )
            {
                return SMR_INV_COMMAND;
            }
            pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type = DRVT_VID_DEC;
            i4_ret = _sm_vs_prc_prim_cmd( h_stream, &(pt_vs_obj->t_vs_comp_grp.t_dec), &pt_iter );
            break;
            
        case SM_VS_CMD_CODE_SET_TV_DECODER:
            if ( pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type != DRVT_UNKNOWN )
            {
                return SMR_INV_COMMAND;
            }
            pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type = DRVT_TV_DEC;
            i4_ret = _sm_vs_prc_prim_cmd( h_stream, &(pt_vs_obj->t_vs_comp_grp.t_dec), &pt_iter );
            break;

        case SM_VS_CMD_CODE_SET_VIDEO_PLANE:        
            i4_ret = _sm_vs_prc_prim_cmd( h_stream, &(pt_vs_obj->t_vs_comp_grp.t_vp), &pt_iter );
            break;
            
        case SM_VS_CMD_CODE_SET_PLANE_MIXER:    
            i4_ret = _sm_vs_prc_prim_cmd( h_stream, &(pt_vs_obj->t_vs_comp_grp.t_pmx), &pt_iter );
            break;
            
        case SM_VS_CMD_CODE_SET_TV_ENCODER:
            i4_ret = _sm_vs_prc_prim_cmd( h_stream, &(pt_vs_obj->t_vs_comp_grp.t_tve), &pt_iter );
            break;

        case SM_VS_CMD_CODE_SET_DISP_REGION:
            {
                VSH_REGION_INFO_T* pt_region = (VSH_REGION_INFO_T*)pt_iter->u.pv_data;
                
                if ( !pt_region )
                {
                    return SMR_INV_COMMAND;
                }
                pt_vs_obj->t_disp_region = *pt_region;
                i4_ret = SMR_OK;
                pt_iter = &pt_iter[1];
            }
            break;
            
        case SM_VS_CMD_CODE_SET_VIDEO_REGION:
            {
                VSH_REGION_INFO_T* pt_region = (VSH_REGION_INFO_T*)pt_iter->u.pv_data;
                
                if ( !pt_region )
                {
                    return SMR_INV_COMMAND;
                }
                pt_vs_obj->t_vid_region = *pt_region;
                i4_ret = SMR_OK;
                pt_iter = &pt_iter[1];
            }
            break;

        case SM_VS_CMD_CODE_SET_FORCE_PID:
            {
                if ( pt_vs_obj->t_base.t_strm_comp_id.e_type != ST_VIDEO )
                {
                    return SMR_INV_COMMAND;
                }
                pt_vs_obj->ui2_force_pid = pt_iter->u.ui2_data;
                i4_ret = SMR_OK;
                pt_iter = &pt_iter[1];
            }
            break;

        case SM_VS_CMD_CODE_SET_FORCE_CODEC:
            {
                if ( pt_vs_obj->t_base.t_strm_comp_id.e_type != ST_VIDEO )
                {
                    return SMR_INV_COMMAND;
                }
                pt_vs_obj->ui1_force_enc = pt_iter->u.ui1_data;
                i4_ret = SMR_OK;
                pt_iter = &pt_iter[1];
            }
            break;

#ifdef SYS_MHP_SUPPORT
        case SM_VS_CMD_CODE_SET_FORCE_NO_CA_REQ:
            {
                if ( pt_vs_obj->t_base.t_strm_comp_id.e_type != ST_VIDEO )
                {
                    return SMR_INV_COMMAND;
                }
                pt_vs_obj->e_ca_req_lvl = PM_CA_REQUIRE_NO;
                i4_ret = SMR_OK;
                pt_iter = &pt_iter[1];
            }
            break;
#endif

        case SM_VS_CMD_CODE_SET_MM_CRYPT_INFO:
            {
                MM_CRYPT_INFO_T* pt_crypt_info = (MM_CRYPT_INFO_T*)pt_iter->u.pv_data;
                
                if ( !pt_crypt_info )
                {
                    return SMR_INV_COMMAND;
                }
                pt_vs_obj->t_crypt_info = *pt_crypt_info;
                i4_ret = SMR_OK;
                pt_iter = &pt_iter[1];
            }
            break;

        default:
            return SMR_INV_COMMAND;
        }
        
        if ( i4_ret != SMR_OK )
        {
            break;   
        }
    }
    
    return i4_ret;    
}
