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
 * $RCSfile: c_scc.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Dennis Wang $
 * $MD5HEX: 83bea9f5f49e0aaa8fb0caffd8827ade $
 *
 * Description:
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "aee/aee.h"
#include "handle/handle.h"
#include "strm_mngr/scc/c_scc.h"
#include "strm_mngr/scc/x_scc.h"

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
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_close
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_comp_close( HANDLE_T               h_comp )
{
    return x_scc_comp_close( h_comp );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_open
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_comp_open( SM_COMMAND_T*           pt_cmd_sets,
                       HANDLE_T*               ph_comp )
{
    HANDLE_T  h_aux;
    INT32     i4_ret;

    i4_ret = aee_grab_handle_resource( AEE_FLAG,
                                       1,
                                       &h_aux);
    if ( i4_ret != AEER_OK )
    {
        return SMR_OUT_OF_HANDLES;
    }

    i4_ret = x_scc_comp_open( pt_cmd_sets,
                              ph_comp );

    if ( i4_ret == SMR_OK )
    {
        handle_link_to_aux(h_aux, *ph_comp);
    }
    else
    {
        aee_release_handle_resource(1);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_grp_set
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_comp_grp_set( SM_COMMAND_T*        pt_cmd_sets )
{
    return x_scc_comp_grp_set( pt_cmd_sets );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_grp_set
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_comp_att_incr(
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_curr_value )
{
    return x_scc_comp_att_incr( 
        h_comp,
        e_scc_cmd_code,
        pv_cmd_arg,
        pui2_curr_value );
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_grp_set
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_comp_att_decr( 
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_curr_value )
{
    return x_scc_comp_att_decr( 
        h_comp,
        e_scc_cmd_code,
        pv_cmd_arg,
        pui2_curr_value );
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_grp_set
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_comp_att_get_num_steps( 
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_num_steps )
{
    return x_scc_comp_att_get_num_steps( 
        h_comp,
        e_scc_cmd_code,
        pv_cmd_arg,
        pui2_num_steps );
}


/* Video related API *********************************************************/
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_mode( HANDLE_T                   h_vid,
                          SCC_VID_MODE_T             e_mode )
{
    return x_scc_vid_set_mode( h_vid,
                               e_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_mode_ex
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_mode_ex( HANDLE_T                   h_vid,
                             const SCC_VID_MODE_EX_T*   pt_mode )
{
    return x_scc_vid_set_mode_ex( h_vid,
                                  pt_mode );    
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_sig_booster
 *
 * Description: <function description>
 *
 * Inputs: 
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_sig_booster( HANDLE_T                   h_vid,
                                      BOOL                       b_is_on )
{
    return x_scc_vid_set_sig_booster( h_vid, b_is_on);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_bg( HANDLE_T                     h_vid,
                        const SCC_BG_COLOR_T*        pt_bg )
{
    return x_scc_vid_set_bg( h_vid,
                             pt_bg );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_super_blank
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_super_blank( HANDLE_T                     h_vid,
                                 const SCC_VID_SUPER_BLANK_T* pt_blank )
{
    return x_scc_vid_set_super_blank( h_vid,
                                      pt_blank );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_pic_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_pic_mode( HANDLE_T               h_vid,
                              SCC_VID_PIC_MODE_T     e_pic_mode )
{
    return x_scc_vid_set_pic_mode( h_vid,
                                   e_pic_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_hdmi_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_hdmi_mode( HANDLE_T               h_vid,
                               SCC_VID_HDMI_MODE_T    e_hdmi_mode )
{
    return x_scc_vid_set_hdmi_mode( h_vid,
                                   e_hdmi_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_color_sys
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_color_sys( HANDLE_T               h_vid,
                               SCC_VID_COLOR_SYS_T    e_color_sys )
{
    return x_scc_vid_set_color_sys( h_vid,
                                    e_color_sys );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_scr_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_scr_mode( HANDLE_T               h_vid,
                              SCC_VID_SCREEN_MODE_T  e_scr_mode )
{
    return x_scc_vid_set_scr_mode( h_vid,
                                   e_scr_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_enhance
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_enhance( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_enhance )
{
    return x_scc_vid_set_enhance( h_vid, ui2_enhance );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_blur
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_blur( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_blur )
{
    return x_scc_vid_set_blur( h_vid, ui2_blur );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_overscan_clipper
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_overscan_clipper( 
    HANDLE_T                   h_vid,
    const SCC_VID_OVER_SCAN_CLIPPER_T* pt_clipper )
{
    return x_scc_vid_set_overscan_clipper( h_vid, pt_clipper );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_cti
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_cti( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_cti )
{
    return x_scc_vid_set_cti( h_vid, ui2_cti );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_eti
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_eti( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_eti )
{
    return x_scc_vid_set_eti( h_vid, ui2_eti );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_sharpness
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_sharpness( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_sharpness )
{
    return x_scc_vid_set_sharpness( h_vid, ui2_sharpness );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_color_suppress
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_color_suppress( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_color_supress )
{
    return x_scc_vid_set_color_suppress( h_vid, ui2_color_supress );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_nr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_nr( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_nr )
{
    return x_scc_vid_set_nr( h_vid, ui2_nr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3dnr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3dnr( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_3dnr )
{
    return x_scc_vid_set_3dnr( h_vid, ui2_3dnr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_black_lvl_ext
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_black_lvl_ext( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_black_lvl_ext )
{
    return x_scc_vid_set_black_lvl_ext( h_vid, ui2_black_lvl_ext );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_white_peak_rdct
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_white_peak_rdct( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_white_peak_rdct )
{
    return x_scc_vid_set_white_peak_rdct( h_vid, ui2_white_peak_rdct );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_flesh_tone
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_flesh_tone( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_flesh_tone )
{
    return x_scc_vid_set_flesh_tone( h_vid, ui2_flesh_tone );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_luma
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_luma( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_luma )
{
    return x_scc_vid_set_luma( h_vid, ui2_luma );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_letter_box_detect
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_letter_box_detect( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    return x_scc_vid_set_letter_box_detect( h_vid, b_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_lb_detect_config
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_lb_detect_config( 
    HANDLE_T                   h_vid,
    SCC_VID_LB_DETECT_CONFIG_T* pt_config )
{
    return x_scc_vid_set_lb_detect_config( h_vid, pt_config );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_dynamic_scaling
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_dynamic_scaling( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_effect_time )
{
    return x_scc_vid_set_dynamic_scaling( h_vid, ui2_effect_time );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_deblocking
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_deblocking( 
    HANDLE_T                   h_vid,
    SCC_VID_DEBLOCKING_INFO_T* pt_deblocking )
{
    return x_scc_vid_set_deblocking( h_vid, pt_deblocking );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_split_scrn_demo
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_split_scrn_demo( 
    HANDLE_T                   h_vid,
    SCC_VID_SPLIT_SCRN_DEMO_T  e_splt_scrn_demo )
{
    return x_scc_vid_set_split_scrn_demo( h_vid, e_splt_scrn_demo );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_h_position
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_h_position( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    return x_scc_vid_set_h_position( h_vid, ui2_offset );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_v_position
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_v_position( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    return x_scc_vid_set_v_position( h_vid, ui2_offset );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_h_size
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_h_size( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    return x_scc_vid_set_h_size( h_vid, ui2_offset );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_v_size
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_v_size( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    return x_scc_vid_set_v_size( h_vid, ui2_offset );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_film_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_film_mode( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    return x_scc_vid_set_film_mode( h_vid, b_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_reset_vga_timing_option
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_reset_vga_timing_option( 
    HANDLE_T                   h_vid )
{
    return x_scc_vid_set_reset_vga_timing_option( h_vid );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_ambig_vga_timing_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_ambig_vga_timing_ctrl( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    return x_scc_vid_set_ambig_vga_timing_ctrl( h_vid, b_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_force_unmute_on_no_sig
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_force_unmute_on_no_sig( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    return x_scc_vid_set_force_unmute_on_no_sig( h_vid, b_is_on );
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_srm_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_srm_mode( 
    HANDLE_T                   h_vid,
    SCC_VID_SRM_MODE_T         e_srm_mode )
{
    return x_scc_vid_set_srm_mode( h_vid, e_srm_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_super_freeze
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_super_freeze( 
    HANDLE_T                   h_vid,
    const SCC_VID_SUPER_FREEZE_T*   pt_freeze )
{
    return x_scc_vid_set_super_freeze( h_vid, pt_freeze );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_vdp_atomic_cmd
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_vdp_atomic_cmd( 
    HANDLE_T                       h_vid,
    SM_COMMAND_T*                  pt_atomic_cmd )
{
    return x_scc_vid_set_vdp_atomic_cmd( h_vid, pt_atomic_cmd );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_super_freeze
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_super_freeze( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    return x_scc_vid_get_super_freeze( h_vid, pb_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_force_unmute_on_no_sig
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_force_unmute_on_no_sig( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    return x_scc_vid_get_force_unmute_on_no_sig( h_vid, pb_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_mode( HANDLE_T                   h_vid,
                          SCC_VID_MODE_T*            pe_mode )
{
    return x_scc_vid_get_mode( h_vid,
                               pe_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_mode_capability
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_NOT_SUPPORT     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_mode_capability( HANDLE_T                   h_vid,
                                     SCC_VID_MODE_T             e_mode )
{
    return x_scc_vid_get_mode_capability( h_vid,
                                          e_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_bg( HANDLE_T                     h_vid,
                        SCC_BG_COLOR_T*              pt_bg )
{
    return x_scc_vid_get_bg( h_vid,
                             pt_bg );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_pic_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_pic_mode( HANDLE_T               h_vid,
                              SCC_VID_PIC_MODE_T*    pe_pic_mode )
{
    return x_scc_vid_get_pic_mode( h_vid,
                                   pe_pic_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_hdmi_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_hdmi_mode( HANDLE_T               h_vid,
                               SCC_VID_HDMI_MODE_T*   pe_hdmi_mode )
{
    return x_scc_vid_get_hdmi_mode( h_vid,
                                    pe_hdmi_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_color_sys
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_color_sys( HANDLE_T               h_vid,
                               SCC_VID_COLOR_SYS_T*   pe_color_sys )
{
    return x_scc_vid_get_color_sys( h_vid,
                                    pe_color_sys );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_scr_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_scr_mode( HANDLE_T               h_vid,
                              SCC_VID_SCREEN_MODE_T* pe_scr_mode )
{
    return x_scc_vid_get_scr_mode( h_vid,
                                   pe_scr_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_enhance
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_enhance( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_enhance )
{
    return x_scc_vid_get_enhance( h_vid, pui2_enhance );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_blur
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_blur( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_blur )
{
    return x_scc_vid_get_blur( h_vid, pui2_blur );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_overscan_clipper
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_overscan_clipper( 
    HANDLE_T                     h_vid,
    SCC_VID_OVER_SCAN_CLIPPER_T* pt_clipper )
{
    return x_scc_vid_get_overscan_clipper( h_vid, pt_clipper );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_cti
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_cti( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_cti )
{
    return x_scc_vid_get_cti( h_vid, pui2_cti );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_eti
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_eti( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_eti )
{
    return x_scc_vid_get_eti( h_vid, pui2_eti );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_sharpness
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_sharpness( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_sharpness )
{
    return x_scc_vid_get_sharpness( h_vid, pui2_sharpness );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_color_supress
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_color_supress( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_color_supress )
{
    return x_scc_vid_get_color_supress( h_vid, pui2_color_supress );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_nr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_nr( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_nr )
{
    return x_scc_vid_get_nr( h_vid, pui2_nr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3dnr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3dnr( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_3dnr )
{
    return x_scc_vid_get_3dnr( h_vid, pui2_3dnr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_black_lvl_ext
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_black_lvl_ext( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_black_lvl_ext )
{
    return x_scc_vid_get_black_lvl_ext( h_vid, pui2_black_lvl_ext );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_white_peak_rdct
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_white_peak_rdct( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_white_peak_rdct )
{
    return x_scc_vid_get_white_peak_rdct( h_vid, pui2_white_peak_rdct );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_flesh_tone
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_flesh_tone( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_flesh_tone )
{
    return x_scc_vid_get_flesh_tone( h_vid, pui2_flesh_tone );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_luma
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_luma( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_luma )
{
    return x_scc_vid_get_luma( h_vid, pui2_luma );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_letter_box_detect
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_letter_box_detect( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    return x_scc_vid_get_letter_box_detect( h_vid, pb_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_lb_detect_config
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_lb_detect_config( 
    HANDLE_T                   h_vid,
    SCC_VID_LB_DETECT_CONFIG_T* pt_config )
{
    return x_scc_vid_get_lb_detect_config( h_vid, pt_config );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_dynamic_scaling
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_dynamic_scaling( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_effect_time )
{
    return x_scc_vid_get_dynamic_scaling( h_vid, pui2_effect_time );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_deblocking
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_deblocking( 
    HANDLE_T                   h_vid,
    SCC_VID_DEBLOCKING_INFO_T* pt_deblocking )
{
    return x_scc_vid_get_deblocking( h_vid, pt_deblocking );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_split_scrn_demo
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_split_scrn_demo( 
    HANDLE_T                   h_vid,
    SCC_VID_SPLIT_SCRN_DEMO_T* pe_splt_scrn_demo )
{
    return x_scc_vid_get_split_scrn_demo( h_vid, pe_splt_scrn_demo );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_h_position
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_h_position( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    return x_scc_vid_get_h_position( h_vid, pui2_min, pui2_max, pui2_curr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_v_position
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_v_position( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    return x_scc_vid_get_v_position( h_vid, pui2_min, pui2_max, pui2_curr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_h_size
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_h_size( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    return x_scc_vid_get_h_size( h_vid, pui2_min, pui2_max, pui2_curr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_v_size
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_v_size( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    return x_scc_vid_get_v_size( h_vid, pui2_min, pui2_max, pui2_curr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_film_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_film_mode( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    return x_scc_vid_get_film_mode( h_vid, pb_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_nfy
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_nfy(
    HANDLE_T                     h_vid, 
    x_scc_vid_iframe_nfy_fct     pf_scc_iframe_nfy, 
    VOID*                        pv_nfy_tag )
{
    return x_scc_vid_set_iframe_nfy(h_vid, pf_scc_iframe_nfy, pv_nfy_tag);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_iframe_alloc_buffer
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_iframe_alloc_buffer(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    return x_scc_vid_iframe_alloc_buffer(h_vid, pt_iframe_buf);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_iframe_decode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_buffer(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    return x_scc_vid_set_iframe_buffer(h_vid, pt_iframe_buf);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_iframe_decode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_iframe_decode(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    return x_scc_vid_iframe_decode(h_vid, pt_iframe_buf);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_iframe_free_buffer
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_iframe_free_buffer(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    return x_scc_vid_iframe_free_buffer(h_vid, pt_iframe_buf);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_codec
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_codec(
    HANDLE_T                      h_vid, 
    SCC_VID_IFRAME_CODEC_T        e_codec )
{
    return x_scc_vid_set_iframe_codec(h_vid, e_codec);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_ctrl(
    HANDLE_T                      h_vid, 
    SCC_VID_IFRAME_CTRL_T         e_ctrl_mode )
{
    return x_scc_vid_set_iframe_ctrl(h_vid, e_ctrl_mode);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_iframe_resolution
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_iframe_resolution(
    HANDLE_T                       h_vid, 
    UINT32*                        pui4_width, 
    UINT32*                        pui4_height)

{
    return x_scc_vid_get_iframe_resolution(h_vid, pui4_width, pui4_height);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_pla_order
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_pla_order(
    HANDLE_T                       h_vid, 
    VSH_SET_PLANE_ORDER_INFO_T*    pt_order_info)
{
    return x_scc_vid_set_iframe_pla_order(h_vid, pt_order_info);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_src_region
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_src_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info)
{
    return x_scc_vid_set_iframe_src_region(h_vid, pt_region_info);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_disp_region
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_disp_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info)
{
    return x_scc_vid_set_iframe_disp_region(h_vid, pt_region_info);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_iframe_src_region
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_iframe_src_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info)
{
    return x_scc_vid_get_iframe_src_region(h_vid, pt_region_info);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_iframe_disp_region
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_iframe_disp_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info)
{
    return x_scc_vid_get_iframe_disp_region(h_vid, pt_region_info);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_iframe_surface
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_iframe_surface(
    HANDLE_T                       h_vid,
    HANDLE_T                       h_surface)
{
    return x_scc_vid_get_iframe_surface(h_vid, h_surface);
}

#ifdef SYS_MHP_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_not_display
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_not_display(
    HANDLE_T                      h_vid, 
    BOOL                          b_not_display )
{
    return x_scc_vid_set_iframe_not_display(h_vid, b_not_display);
}
#endif

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_vdrips_nfy
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_vdrips_nfy(
    HANDLE_T                     h_vid,  
    x_scc_vid_vdrips_nfy_fct     pf_scc_vdrips_nfy, 
    VOID*                        pv_nfy_tag )
{
    return x_scc_vid_set_vdrips_nfy(h_vid, pf_scc_vdrips_nfy, pv_nfy_tag);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_vdrips_feed
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_vdrips_feed(
    HANDLE_T                     h_vid, 
    SCC_VID_VDRIPS_BUFF_T*       pt_vdrips_buf)
{
    return x_scc_vid_vdrips_feed(h_vid, pt_vdrips_buf);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_vdrips_play_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_vdrips_play_ctrl(
    HANDLE_T                      h_vid, 
    SCC_VID_VDRIPS_PLAY_CTRL_T    e_ctrl_mode )
{
    return x_scc_vid_vdrips_play_ctrl(h_vid, e_ctrl_mode);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_vdrips_duration
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_vdrips_duration(
    HANDLE_T                       h_vid, 
    UINT32                         ui4_duration )
{
    return x_scc_vid_get_vdrips_duration(h_vid, ui4_duration);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_di_film_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_di_film_mode( HANDLE_T                  h_vid,
                                  SCC_VID_DI_FILM_MODE_T    e_film_mode )
{
    return x_scc_vid_set_di_film_mode( h_vid, e_film_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_di_film_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_di_film_mode( HANDLE_T                   h_vid,
                                  SCC_VID_DI_FILM_MODE_T*    pe_film_mode )
{
    return x_scc_vid_get_di_film_mode( h_vid, pe_film_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_di_ma
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_di_ma( HANDLE_T           h_vid,
                           SCC_VID_DI_MA_T    e_di_ma )
{
    return x_scc_vid_set_di_ma( h_vid, e_di_ma );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_di_ma
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_di_ma( HANDLE_T            h_vid,
                           SCC_VID_DI_MA_T*    pe_di_ma )
{
    return x_scc_vid_get_di_ma( h_vid, pe_di_ma );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_di_edge_preserve
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_di_edge_preserve( HANDLE_T                      h_vid,
                                      SCC_VID_DI_EDGE_PRESERVE_T    e_edge )
{
    return x_scc_vid_set_di_edge_preserve( h_vid, e_edge );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_di_edge_preserve
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_di_edge_preserve( HANDLE_T                       h_vid,
                                      SCC_VID_DI_EDGE_PRESERVE_T*    pe_edge )
{
    return x_scc_vid_get_di_edge_preserve( h_vid, pe_edge );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_mjc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_mjc( HANDLE_T                h_vid,
                         const SCC_VID_MJC_T*    pt_mjc )
{
    return x_scc_vid_set_mjc( h_vid, pt_mjc );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_mjc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_mjc( HANDLE_T          h_vid,
                         SCC_VID_MJC_T*    pt_mjc )
{
    return x_scc_vid_get_mjc( h_vid, pt_mjc );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_xvycc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_xvycc( HANDLE_T           h_vid,
                           SCC_VID_XVYCC_T    e_xvycc )
{
    return x_scc_vid_set_xvycc( h_vid, e_xvycc );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_xvycc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_xvycc( HANDLE_T            h_vid,
                           SCC_VID_XVYCC_T*    pe_xvycc )
{
    return x_scc_vid_get_xvycc( h_vid, pe_xvycc );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_wcg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_wcg( HANDLE_T         h_vid,
                         SCC_VID_WCG_T    e_wcg )
{
    return x_scc_vid_set_wcg( h_vid, e_wcg );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_wcg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_wcg( HANDLE_T          h_vid,
                         SCC_VID_WCG_T*    pe_wcg )
{
    return x_scc_vid_get_wcg( h_vid, pe_wcg );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_game_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_game_mode( HANDLE_T               h_vid,
                               SCC_VID_GAME_MODE_T    e_game )
{
    return x_scc_vid_set_game_mode( h_vid, e_game );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_game_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_game_mode( HANDLE_T                h_vid,
                               SCC_VID_GAME_MODE_T*    pe_game )
{
    return x_scc_vid_get_game_mode( h_vid, pe_game );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_blue_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_blue_stretch( HANDLE_T                  h_vid,
                                  SCC_VID_BLUE_STRETCH_T    e_stretch )
{
    return x_scc_vid_set_blue_stretch( h_vid, e_stretch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_blue_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_blue_stretch( HANDLE_T                   h_vid,
                                  SCC_VID_BLUE_STRETCH_T*    pe_stretch )
{
    return x_scc_vid_get_blue_stretch( h_vid, pe_stretch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_mpeg_nr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_mpeg_nr( HANDLE_T             h_vid,
                             SCC_VID_MPEG_NR_T    e_nr )
{
    return x_scc_vid_set_mpeg_nr( h_vid, e_nr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_mpeg_nr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_mpeg_nr( HANDLE_T              h_vid,
                             SCC_VID_MPEG_NR_T*    pe_nr )
{
    return x_scc_vid_get_mpeg_nr( h_vid, pe_nr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_hdcp_key_exist
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_hdcp_key_exist( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_exist )
{
    return x_scc_vid_get_hdcp_key_exist( h_vid, pb_is_exist );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_black_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_black_stretch( HANDLE_T                  h_vid,
                                   SCC_VID_BLACK_STRETCH_T   e_stretch )
{
    return x_scc_vid_set_black_stretch( h_vid, e_stretch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_black_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_black_stretch( HANDLE_T                   h_vid,
                                   SCC_VID_BLACK_STRETCH_T*   pe_stretch )
{
    return x_scc_vid_get_black_stretch( h_vid, pe_stretch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_white_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_white_stretch( HANDLE_T                  h_vid,
                                   SCC_VID_WHITE_STRETCH_T   e_stretch )
{
    return x_scc_vid_set_white_stretch( h_vid, e_stretch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_white_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_white_stretch( HANDLE_T                   h_vid,
                                   SCC_VID_WHITE_STRETCH_T*   pe_stretch )
{
    return x_scc_vid_get_white_stretch( h_vid, pe_stretch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_black_stretch_val
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_black_stretch_val( HANDLE_T            h_vid,
                                       UINT16              ui2_stretch )
{
    return x_scc_vid_set_black_stretch_val( h_vid, ui2_stretch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_black_stretch_val
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_black_stretch_val( HANDLE_T            h_vid,
                                       UINT16*             pui2_stretch )
{
    return x_scc_vid_get_black_stretch_val( h_vid, pui2_stretch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_white_stretch_val
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_white_stretch_val( HANDLE_T           h_vid,
                                       UINT16             ui2_stretch )
{
    return x_scc_vid_set_white_stretch_val( h_vid, ui2_stretch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_white_stretch_val
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_white_stretch_val( HANDLE_T            h_vid,
                                       UINT16*             pui2_stretch )
{
    return x_scc_vid_get_white_stretch_val( h_vid, pui2_stretch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_colorspace
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_colorspace           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_colorspace( HANDLE_T                  h_vid,
                                  SCC_VID_COLORSPACE_T    e_colorspace )
{
    return x_scc_vid_set_colorspace( h_vid, e_colorspace);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_flip
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_flip( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    return x_scc_vid_set_flip( h_vid, b_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_mirror
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_mirror( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    return x_scc_vid_set_mirror( h_vid, b_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_flip
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_flip( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    return x_scc_vid_get_flip( h_vid, pb_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_mirror
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_mirror( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    return x_scc_vid_get_mirror( h_vid, pb_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_local_dimming_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_local_dimming_mode(
    HANDLE_T                        h_vid,
    SCC_VID_LOCAL_DIMMING_TYPE_T    e_lcdim_mode )
{
    return x_scc_vid_set_local_dimming_mode( h_vid, e_lcdim_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_local_dimming_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_local_dimming_mode( 
    HANDLE_T                        h_vid,
    SCC_VID_LOCAL_DIMMING_TYPE_T*   pe_lcdim_mode )
{
    return x_scc_vid_get_local_dimming_mode( h_vid, pe_lcdim_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_src_detect
 *
 * Description: set video source detect notify function
 *
 * Inputs:  h_vid                video component handle
 *          pf_scc_vid_src_detect_nfy  SCC source detect notify function callback
 *          pv_nfy_tag           notify callback tag
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_src_detect_nfy(
    HANDLE_T                        h_vid, 
    x_scc_vid_src_detect_nfy_fct    pf_scc_vid_src_detect_nfy, 
    VOID*                           pv_nfy_tag) 
{
    return x_scc_vid_set_src_detect_nfy( h_vid, pf_scc_vid_src_detect_nfy, pv_nfy_tag );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_nav
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_nav                 References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_nav( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_T                e_nav )
{        
    return x_scc_vid_set_3d_nav( h_vid, e_nav );
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_nav
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_nav             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_nav( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_T*               pe_nav )
{        
    return x_scc_vid_get_3d_nav( h_vid, pe_nav);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_3d_mode           References.
 *
 * Outputs: 
 *
 *Returns: SMR_OK              Success.
 *         SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_mode( 
    HANDLE_T                    h_vid,
    SCC_3D_MODE_T               e_3d_mode )
{        
    return x_scc_vid_set_3d_mode( h_vid, e_3d_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pe_3d_mode            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK               Success.
 *          SMR_INTERNAL_ERROR   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_mode( 
    HANDLE_T                    h_vid,
    SCC_3D_MODE_T*              pe_3d_mode )
{       
    return x_scc_vid_get_3d_mode( h_vid, pe_3d_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_parallax
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          ui4_prlx               References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_parallax( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_prlx )
{        
    return x_scc_vid_set_3d_parallax( h_vid, ui4_prlx );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_parallax
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          pui4_prlx              References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_parallax( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_prlx )
{        
    return x_scc_vid_get_3d_parallax( h_vid, pui4_prlx );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_fld_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          ui4_fld_depth          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_fld_depth( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_fld_depth )
{        
    return x_scc_vid_set_3d_fld_depth( h_vid, ui4_fld_depth );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_fld_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_fld_depth          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_fld_depth( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_fld_depth )
{        
    return x_scc_vid_get_3d_fld_depth( h_vid, pui4_fld_depth );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_lr_switch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_lr_switch           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_lr_switch( 
    HANDLE_T                    h_vid,
    SCC_3D_LR_SWITCH_T          e_lr_switch )
{        
    return x_scc_vid_set_3d_lr_switch( h_vid, e_lr_switch );
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_lr_switch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_lr_switch       References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_lr_switch( 
    HANDLE_T                    h_vid,
    SCC_3D_LR_SWITCH_T*         pe_lr_switch )
{        
    return x_scc_vid_get_3d_lr_switch( h_vid, pe_lr_switch );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_to_2d
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                References.
 *           e_3d_to_2d          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK               Success.
 *          SMR_INTERNAL_ERROR   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_to_2d( 
    HANDLE_T                    h_vid,
    SCC_3D_TO_2D_T              e_3d_to_2d )
{        
    return x_scc_vid_set_3d_to_2d( h_vid, e_3d_to_2d );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_to_2d
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_3d_to_2d            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_to_2d( 
    HANDLE_T                    h_vid,
    SCC_3D_TO_2D_T*             pe_3d_to_2d )
{        
    return x_scc_vid_get_3d_to_2d( h_vid, pe_3d_to_2d );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_fpr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_fpr                 References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_fpr( 
    HANDLE_T                    h_vid,
    SCC_3D_FPR_T                e_fpr )
{        
    return x_scc_vid_set_3d_fpr( h_vid, e_fpr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_fpr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_fpr             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_fpr( 
    HANDLE_T                    h_vid,
    SCC_3D_FPR_T*               pe_fpr )
{        
    return x_scc_vid_get_3d_fpr( h_vid, pe_fpr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_protrude_scr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          ui4_pro_scr             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_protrude_scr( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_pro_scr )
{
    return x_scc_vid_set_3d_protrude_scr( h_vid, ui4_pro_scr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_protrude_scr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_pro_scr            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_protrude_scr( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_pro_scr )
{
    return x_scc_vid_get_3d_protrude_scr( h_vid, pui4_pro_scr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_distance_2_tv
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          ui4_dis_2_tv            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_distance_2_tv( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_dis_2_tv )
{
    return x_scc_vid_set_3d_distance_2_tv( h_vid, ui4_dis_2_tv );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_distance_2_tv
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_dis_2_tv           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_distance_2_tv( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_dis_2_tv )
{
    return x_scc_vid_get_3d_distance_2_tv( h_vid, pui4_dis_2_tv );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_osd_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          ui4_osd_depth           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_osd_depth( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_osd_depth )
{
    return x_scc_vid_set_3d_osd_depth( h_vid, ui4_osd_depth );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_osd_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_osd_depth          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_osd_depth( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_osd_depth )
{
    return x_scc_vid_get_3d_osd_depth( h_vid, pui4_osd_depth );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_fmt_cap
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pui4_fmt_cap          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_fmt_cap( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_fmt_cap )
{        
    return x_scc_vid_get_3d_fmt_cap( h_vid, pui4_fmt_cap );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_ctrl_cap
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          pui4_ctrl_cap          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_ctrl_cap( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_ctrl_cap )
{        
    return x_scc_vid_get_3d_ctrl_cap( h_vid, pui4_ctrl_cap );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_nav_auto_chg_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          e_auto_chg             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_nav_auto_chg_en( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_AUTO_CHG_T       e_auto_chg )
{
    return x_scc_vid_set_3d_nav_auto_chg_en( h_vid, e_auto_chg );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_nav_auto_chg_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                References.
 *          
 * Outputs: 
 *             pe_auto_chg       References.
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_nav_auto_chg_en( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_AUTO_CHG_T*      pe_auto_chg )
{
    return x_scc_vid_get_3d_nav_auto_chg_en( h_vid, pe_auto_chg );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_image_safety_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          e_img_safe             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_image_safety_en( 
    HANDLE_T                    h_vid,
    SCC_3D_IMAGE_SAFETY_T       e_img_safe )
{
    return x_scc_vid_set_3d_image_safety_en( h_vid, e_img_safe );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_nav_auto_chg_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          
 * Outputs: 
 *          pe_img_safe         References.
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_image_safety_en( 
    HANDLE_T                    h_vid,
    SCC_3D_IMAGE_SAFETY_T*      pe_img_safe )
{
    return x_scc_vid_get_3d_image_safety_en( h_vid, pe_img_safe );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_nav_tag
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                References.
 *          
 * Outputs: 
 *          pe_nav_tag           References.
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_nav_tag( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_TAG_T*           pe_nav_tag )
{
    return x_scc_vid_get_3d_nav_tag( h_vid, pe_nav_tag );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_pedestal
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *              b_is_on             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_pedestal(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    return x_scc_vid_set_pedestal( h_vid, b_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_hdmi_range
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_hdmi_range       References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_hdmi_range( 
    HANDLE_T                    h_vid,
    SCC_HDMI_RANGE_T            e_hdmi_range )
{
    return x_scc_vid_set_hdmi_range( h_vid, e_hdmi_range );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_overscan_enable
 *
 * Description: <function description>
 *
 * Inputs:    h_vid              References.
 *          pb_is_enable        References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_overscan_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{
    return x_scc_vid_get_overscan_enable( h_vid, pb_is_enable );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_nonlinear_scaling_enable
 *
 * Description: <function description>
 *
 * Inputs:    h_vid              References.
 *          pb_is_enable        References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_nonlinear_scaling_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{ 
    return x_scc_vid_get_nonlinear_scaling_enable( h_vid, pb_is_enable );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_panel_type
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_panel_type      References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_panel_type( 
    HANDLE_T                    h_vid,
    SCC_3D_PANEL_TYPE_T*        pe_panel_type )
{
    return x_scc_vid_get_3d_panel_type( h_vid, pe_panel_type );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_vga_detect
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          e_vga_detect     References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_vga_detect( 
    HANDLE_T                    h_vid,
    SCC_VGA_DETECT_T            e_vga_detect )
{
    return x_scc_vid_set_vga_detect( h_vid, e_vga_detect );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_panel_type
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_vga_status   References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_vga_status( 
    HANDLE_T                    h_vid,
    SCC_VGA_STATUS_T*           pe_vga_status )
{
    return x_scc_vid_get_vga_status( h_vid, pe_vga_status );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_static_gamma
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          ui1_static_gamma      References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_static_gamma( 
    HANDLE_T                    h_vid,
    UINT8                       ui1_static_gamma )
{
    return x_scc_vid_set_static_gamma( h_vid, ui1_static_gamma );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_angular_sce
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pt_angular_sce        References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_angular_sce( 
    HANDLE_T                    h_vid,
    SCC_ANGULAR_SCE_T*          pt_angular_sce )
{
    return x_scc_vid_set_angular_sce( h_vid, pt_angular_sce );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_down_bw_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui1_down_bw_mode    References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_down_bw_mode( 
    HANDLE_T                    h_vid,
    UINT8                       ui1_down_bw_mode )
{    
    return x_scc_vid_set_down_bw_mode( h_vid, ui1_down_bw_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_mjc_enable
 *
 * Description: For AP query whether support 3D mjc in driver
 *
 * Inputs:  h_vid              References.
 *          pb_is_enable       References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_mjc_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{
    return x_scc_vid_get_3d_mjc_enable( h_vid, pb_is_enable );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_freq
 *
 * Description: For AP query video v-freqency and h-freqency
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_freq( 
    HANDLE_T                h_vid,
    SCC_VID_FREQ_T*         pt_freq )
{
    return x_scc_vid_get_freq( h_vid, pt_freq );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_src_detect_status
 *
 * Description: For AP query source detect status.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_src_detect_status( 
    HANDLE_T                    h_vid,
    SCC_VID_SRC_DET_STATUS_T*   pt_src_status )
{
    return x_scc_vid_get_src_detect_status( h_vid, pt_src_status );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_asp_ratio_enable
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_asp_ratio_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{
    return x_scc_vid_get_asp_ratio_enable( h_vid, pb_is_enable );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_screen_mode_enable
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_screen_mode_enable( 
    HANDLE_T                   h_vid,
    SCC_VID_SCR_MODE_T*        pt_scr_enable )
{
    return x_scc_vid_get_screen_mode_enable( h_vid, pt_scr_enable );
}
    
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_super_resolution
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_super_resolution( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    return x_scc_vid_set_super_resolution( h_vid, b_is_on );
}

/* Display related API *******************************************************/
/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_attrib
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_attrib( HANDLE_T             h_disp,
                             SCC_DISP_ATTRIB_T    e_attrib,
                             UINT16               ui2_value )
{
    return x_scc_disp_set_attrib( h_disp,
                                  e_attrib,
                                  ui2_value );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_adp_back_light_rng
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_adp_back_light_rng( 
    HANDLE_T                       h_disp,
    const SCC_DISP_RANGE_INFO_T*   pt_range )
{
    return x_scc_disp_set_adp_back_light_rng( h_disp,
                                              pt_range );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_resolution
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_resolution( 
    HANDLE_T                       h_disp,
    const SCC_DISP_RESOLUTION_T*   pt_resolution )
{
    return x_scc_disp_set_resolution( h_disp,
                                      pt_resolution );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_frame_rate
 *
 * Description: <function description>
 *
 * Inputs:  h_disp                   References.
 *          ui4_frame_rate           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32                      ui4_frame_rate )
{
    return x_scc_disp_set_frame_rate( h_disp, ui4_frame_rate );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_frame_rate
 *
 * Description: <function description>
 *
 * Inputs:  h_disp                   References.
 *          pui4_frame_rate          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32*                     pui4_frame_rate )
{
    return x_scc_disp_get_frame_rate( h_disp, pui4_frame_rate );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_force_frame_rate
 *
 * Description: <function description>
 *
 * Inputs:  h_disp                 References.
 *          ui4_force_fr           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_force_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32                      ui4_force_fr )
{
    return x_scc_disp_set_force_frame_rate( h_disp, ui4_force_fr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_aspect_ratio
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_aspect_ratio( 
    HANDLE_T                h_disp,
    SCC_DISP_ASPECT_RATIO_T e_aspect_ratio )
{
    return x_scc_disp_set_aspect_ratio( h_disp,
                                        e_aspect_ratio );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_fmt
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_fmt( HANDLE_T                h_disp,
                          SCC_DISP_FMT_T          e_fmt )
{
    return x_scc_disp_set_fmt( h_disp,
                               e_fmt );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_tv_type
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_tv_type( HANDLE_T            h_disp,
                              SCC_DISP_TV_TYPE_T  e_tv_type )
{
    return x_scc_disp_set_tv_type( h_disp,
                                   e_tv_type );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_bg( HANDLE_T                 h_disp,
                         const SCC_BG_COLOR_T*    pt_bg )
{
    return x_scc_disp_set_bg( h_disp,
                              pt_bg );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_digit_output_fmt( 
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUTPUT_FMT_T     e_digit_out_fmt )
{
    return c_scc_disp_set_digit_output_fmt( h_disp, e_digit_out_fmt );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_color_gain
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          pt_color_gain       References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_color_gain( 
    HANDLE_T                        h_disp,
    const SCC_DISP_COLOR_GAIN_T*    pt_color_gain )
{
    return x_scc_disp_set_color_gain( h_disp, pt_color_gain );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_color_offset
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          pt_color_gain       References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_color_offset( 
    HANDLE_T                        h_disp,
    const SCC_DISP_COLOR_GAIN_T*    pt_color_offset )
{
    return x_scc_disp_set_color_offset( h_disp, pt_color_offset );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_tv_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: e_tv_mode           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_tv_mode(
    HANDLE_T                        h_disp,
    SCC_TV_MODE_T                   e_tv_mode )
{
    return x_scc_disp_set_tv_mode( h_disp, e_tv_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_mjc
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: b_mjc               References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_mjc(
    HANDLE_T                        h_disp,
    SCC_DISP_MJC_T                  e_mjc )
{
    return x_scc_disp_set_mjc( h_disp, e_mjc );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_test_pattern
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          e_pattern           References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_test_pattern(
    HANDLE_T                        h_disp,
    SCC_DISP_TEST_PATTERN_T         e_pattern )
{
    return x_scc_disp_set_test_pattern( h_disp, e_pattern );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_back_light_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_back_light_ctrl( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    return x_scc_disp_set_back_light_ctrl( h_vid, b_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_back_light_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_back_light_ctrl( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    return x_scc_disp_get_back_light_ctrl( h_vid, pb_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_max_gamma
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_max_gamma( HANDLE_T          h_disp,
                                UINT8*            pui1_max_gamma )
{
    return x_scc_disp_get_max_gamma( h_disp,
                                     pui1_max_gamma );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_attrib
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_attrib( HANDLE_T             h_disp,
                             SCC_DISP_ATTRIB_T    e_attrib,
                             UINT16*              pui2_value )
{
    return x_scc_disp_get_attrib( h_disp,
                                  e_attrib,
                                  pui2_value );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_adp_back_light_rng
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_adp_back_light_rng( 
    HANDLE_T                       h_disp,
    SCC_DISP_RANGE_INFO_T*         pt_range )
{
    return x_scc_disp_get_adp_back_light_rng( h_disp,
                                              pt_range );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_resolution
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_resolution( 
    HANDLE_T                       h_disp,
    SCC_DISP_RESOLUTION_T*         pt_resolution )
{
    return x_scc_disp_get_resolution( h_disp,
                                      pt_resolution );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_aspect_ratio
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
extern INT32 c_scc_disp_get_aspect_ratio( 
    HANDLE_T                 h_disp,
    SCC_DISP_ASPECT_RATIO_T* pe_aspect_ratio )
{
    return x_scc_disp_get_aspect_ratio( h_disp,
                                        pe_aspect_ratio );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_fmt
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_fmt( HANDLE_T                h_disp,
                          SCC_DISP_FMT_T*         pe_fmt )
{
    return x_scc_disp_get_fmt( h_disp,
                               pe_fmt );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_tv_type
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_tv_type( HANDLE_T            h_disp,
                              SCC_DISP_TV_TYPE_T* pe_tv_type )
{
    return x_scc_disp_get_tv_type( h_disp,
                                   pe_tv_type );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_bg( HANDLE_T                 h_disp,
                         SCC_BG_COLOR_T*          pt_bg )
{
    return x_scc_disp_get_bg( h_disp,
                              pt_bg );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_digit_output_fmt( 
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUTPUT_FMT_T*    pe_digit_out_fmt )
{
    return c_scc_disp_get_digit_output_fmt( h_disp, pe_digit_out_fmt );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_dither_cap( 
    HANDLE_T          h_disp,
    UINT32*           pui4_dither_cap )
{
    return x_scc_disp_get_dither_cap( h_disp, pui4_dither_cap );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_fmt_cap
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_fmt_cap( HANDLE_T            h_disp,
                              SCC_DISP_FMT_CAP_T* pui4_fmt_cap )
{
    return x_scc_disp_get_fmt_cap( h_disp,
                                   pui4_fmt_cap );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_color_gain
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pt_color_gain       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_color_gain( 
    HANDLE_T                        h_disp,
    SCC_DISP_COLOR_GAIN_T*          pt_color_gain )
{
    return x_scc_disp_get_color_gain( h_disp, pt_color_gain);    
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_color_offset
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pt_color_gain       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_color_offset( 
    HANDLE_T                        h_disp,
    SCC_DISP_COLOR_GAIN_T*          pt_color_offset )
{
    return x_scc_disp_get_color_offset( h_disp, pt_color_offset);    
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_tv_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_tv_mode          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_tv_mode(
    HANDLE_T                        h_disp,
    SCC_TV_MODE_T*                  pe_tv_mode )
{
    return x_scc_disp_get_tv_mode( h_disp, pe_tv_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_mjc
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_mjc              References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_mjc(
    HANDLE_T                        h_disp,
    SCC_DISP_MJC_T*                 pe_mjc )
{
    return x_scc_disp_get_mjc( h_disp, pe_mjc );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_test_pattern
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_pattern          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_test_pattern(
    HANDLE_T                        h_disp,
    SCC_DISP_TEST_PATTERN_T*        pe_pattern )
{
    return x_scc_disp_get_test_pattern( h_disp, pe_pattern );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_screen_resolution
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_screen_resolution( 
    HANDLE_T                       h_disp,
    SCC_DISP_SCREEN_RESOLUTION_T*  pt_scr_resolution )
{
    return x_scc_disp_get_screen_resolution( h_disp, pt_scr_resolution );
}

/* Audio related API *********************************************************/
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_volume
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_volume( HANDLE_T                      h_aud,
                            const SCC_AUD_VOLUME_INFO_T*  pt_volume )
{
    return x_scc_aud_set_volume( h_aud,
                                 pt_volume );
}

/* Audio related API *********************************************************/
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_volume_ex
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_volume_ex( HANDLE_T                      h_aud,
                            const SCC_AUD_VOLUME_INFO_T*  pt_volume )
{
    return x_scc_aud_set_volume_ex( h_aud,
                                    pt_volume );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_mute
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_mute( HANDLE_T                            h_aud )
{
    return x_scc_aud_mute( h_aud );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_unmute
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_unmute( HANDLE_T                          h_aud )
{
    return x_scc_aud_unmute( h_aud );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_spdif_fmt
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_spdif_fmt( HANDLE_T                   h_aud,
                               SCC_AUD_SPDIF_FMT_T        e_spdif_fmt )
{
    return x_scc_aud_set_spdif_fmt( h_aud,
                                    e_spdif_fmt );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_hdcd_filter
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_hdcd_filter( HANDLE_T                 h_aud,
                                 SCC_AUD_HDCD_FLTR_T      e_hdcd_fltr )
{
    return x_scc_aud_set_hdcd_filter( h_aud,
                                      e_hdcd_fltr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_loudspeaker
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_loudspeaker( HANDLE_T                      h_aud,
                                 const SCC_AUD_LOUD_SPEAKER_T* pt_ls_type )
{
    return x_scc_aud_set_loudspeaker( h_aud,
                                      (SCC_AUD_LOUD_SPEAKER_T*)pt_ls_type );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_trim
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_se_trim          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_trim( 
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_TRIM_T*        pt_se_trim )
{
    return x_scc_aud_set_se_trim( h_aud, pt_se_trim);
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_superbase
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_superbase( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    return x_scc_aud_set_se_superbase( h_aud, b_is_on);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_equalizer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_equalizer         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_equalizer( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_EQ_T                 e_equalizer )
{
    return x_scc_aud_set_se_equalizer( h_aud, e_equalizer);
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_reverb
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_reverb            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_reverb( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_REVERB_MODE_T        e_reverb )
{
    return x_scc_aud_set_se_reverb( h_aud, e_reverb);
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_base
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_base            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_base( 
    HANDLE_T                        h_aud,
    UINT8                           ui1_base )
{
    return x_scc_aud_set_se_base( h_aud, ui1_base);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_treble
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_treble          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_treble( 
    HANDLE_T                        h_aud,
    UINT8                           ui1_treble )
{
    return x_scc_aud_set_se_base( h_aud, ui1_treble);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_balance
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_balance         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_balance( 
    HANDLE_T                        h_aud,
    UINT8                           ui1_balance )
{
    return x_scc_aud_set_se_balance( h_aud, ui1_balance);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_postdr
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_postdr( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    return x_scc_aud_set_se_postdr( h_aud, b_is_on);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_volume
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_volume           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_volume( 
    HANDLE_T                        h_aud,
    const SCC_AUD_VOLUME_INFO_T*    pt_volume )
{
    return x_scc_aud_set_se_volume( h_aud, pt_volume);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_surround
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_surround( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    return x_scc_aud_set_se_surround( h_aud, b_is_on );
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_superwoofer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_superwoofer( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    return x_scc_aud_set_se_superwoofer( h_aud, b_is_on );
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_panfade
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_panfade( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    return x_scc_aud_set_panfade( h_aud, b_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_sound_mode_start
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_sound_mode_start( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    return x_scc_aud_set_se_sound_mode_start( h_aud, b_is_on );
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_srs_bypass
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_srs_bypass( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    return x_scc_aud_set_srs_bypass( h_aud, b_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_sound_mde
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_sound_mode        References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_sound_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_SOUND_MODE_T         e_sound_mode )
{
    return x_scc_aud_set_se_sound_mode( h_aud, e_sound_mode);
}
#if 0
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_surround_ex
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui4_surround_mask             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_surround_ex( 
    HANDLE_T                        h_aud,
    UINT32                          ui4_surround_mode)
{
    return x_scc_aud_set_se_surround_ex( h_aud, ui4_surround_mode );
}
#endif
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_surround_ex
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_surround_ex             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_surround_ex( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_SURROUND_INFO_T*                pt_surround_ex)
{
    return x_scc_aud_set_se_surround_ex( h_aud, pt_surround_ex );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_downmix_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_downmix_mode( HANDLE_T                h_aud,
                                  SCC_AUD_DOWNMIX_MODE_T  e_dm_mode )
{
    return x_scc_aud_set_downmix_mode( h_aud,
                                       e_dm_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_dolby_drc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_dolby_drc( HANDLE_T                   h_aud,
                               SCC_AUD_DOLBY_DRC_T        e_drc )
{
    return x_scc_aud_set_dolby_drc( h_aud,
                                    e_drc );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_cmpss_drc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_cmpss_drc( 
    HANDLE_T                            h_aud,
    SCC_AUD_CMPSS_DRC_MODE_T            e_cmpss_drc )
{
    return x_scc_aud_set_cmpss_drc( h_aud, e_cmpss_drc);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_mpeg_level_adj
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_mpeg_level_adj(
    HANDLE_T                        h_aud,
    INT8                            i1_level )
{
    return x_scc_aud_set_mpeg_level_adj( h_aud, i1_level );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_output_port
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_output_port( HANDLE_T                 h_aud,
                                 SCC_AUD_OUT_PORT_T       e_port,
                                 BOOL                     b_is_active )
{
    return x_scc_aud_set_output_port( h_aud,
                                      e_port,
                                      b_is_active );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_output_port_ex
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_output_port_ex( HANDLE_T                 h_aud,
                                 UINT32                          ui4_enable_mask )
{
    return x_scc_aud_set_output_port_ex( h_aud,
                                         ui4_enable_mask );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_cmpss_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_cmpss_mode( HANDLE_T                 h_aud,
                                SCC_AUD_CMPSS_MDOE_T     e_cmpss_mode )
{
    return x_scc_aud_set_cmpss_mode( h_aud, e_cmpss_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_agc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_agc( HANDLE_T                 h_aud,
                         BOOL                     b_agc )
{
    return x_scc_aud_set_agc( h_aud, b_agc );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_vdo_picture_off
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_vdo_picture_off( 
    HANDLE_T                 h_aud,
    BOOL                     b_is_on )
{
    return x_scc_aud_set_vdo_picture_off( h_aud, b_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_mts
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_mts( 
    HANDLE_T                        h_aud,
    SCC_AUD_MTS_T                   e_mts )
{
    return x_scc_aud_set_mts( h_aud, e_mts );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_eq_user_lvl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_eq_user_lvl( 
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_QA_USER_LVL_T* pt_eq_user_lvl )
{
    return x_scc_aud_set_se_eq_user_lvl( h_aud, pt_eq_user_lvl );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_eq_user_lvl_ex
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_eq_user_lvl_ex( 
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_QA_USER_LVL_T* pt_eq_user_lvl )
{
    return x_scc_aud_set_se_eq_user_lvl_ex( h_aud, pt_eq_user_lvl );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_chnl_delay
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_chnl_delay( HANDLE_T                  h_aud,
                                SCC_AUD_CHNL_DELAY_T*     pt_chnl_delay )
{
    return x_scc_aud_set_chnl_delay( h_aud, pt_chnl_delay );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_hdphone_detect_nfy
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_hdphone_detect_nfy( 
    HANDLE_T                           h_aud,
    SCC_AUD_HDPHONE_DETECT_NFT_INFO_T* pt_hdph_nfy_info )
{
    return x_scc_aud_set_hdphone_detect_nfy( h_aud, pt_hdph_nfy_info );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_mute_ex
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_mute_ex( 
    HANDLE_T                        h_aud,
    UINT32                          ui4_muted_out_port_msk )
{
    return x_scc_aud_set_mute_ex( h_aud, ui4_muted_out_port_msk );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_mute_single_port
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_mute_single_port( 
    HANDLE_T                 h_aud,
    SCC_AUD_OUT_PORT_T       e_port,
    BOOL                     b_is_active )
{
    return x_scc_aud_set_mute_single_port( h_aud,
                                           e_port,
                                           b_is_active );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_volume
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_volume( HANDLE_T                      h_aud,
                            SCC_AUD_VOLUME_INFO_T*        pt_volume )
{
    return x_scc_aud_get_volume( h_aud,
                                 pt_volume );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_is_muted
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_is_muted( HANDLE_T                        h_aud,
                          BOOL*                           pb_is_muted )
{
    return x_scc_aud_is_muted( h_aud,
                               pb_is_muted );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_spdif_fmt
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_spdif_fmt( HANDLE_T                   h_aud,
                               SCC_AUD_SPDIF_FMT_T*       pe_spdif_fmt )
{
    return x_scc_aud_get_spdif_fmt( h_aud,
                                    pe_spdif_fmt );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_hdcd_filter
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_hdcd_filter( HANDLE_T                 h_aud,
                                 SCC_AUD_HDCD_FLTR_T*     pe_hdcd_fltr )
{
    return x_scc_aud_get_hdcd_filter( h_aud,
                                      pe_hdcd_fltr );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_loudspeaker
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_loudspeaker( HANDLE_T                 h_aud,
                                 SCC_AUD_LOUD_SPEAKER_T*  pt_ls_type )
{
    return x_scc_aud_get_loudspeaker( h_aud,
                                      pt_ls_type );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_trim
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_se_trim          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_trim( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_TRIM_T*              pt_se_trim )
{
    return x_scc_aud_get_se_trim( h_aud, pt_se_trim );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_superbase
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_superbase( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    return x_scc_aud_get_se_superbase( h_aud, pb_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_equalizer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_se_trim          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_equalizer( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_EQ_T*                pe_equalizer )
{
    return x_scc_aud_get_se_equalizer( h_aud, pe_equalizer );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_reverb
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pe_reverb           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_reverb( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_REVERB_MODE_T*       pe_reverb )
{
    return x_scc_aud_get_se_reverb( h_aud, pe_reverb );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_base
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pui1_base           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_base( 
    HANDLE_T                        h_aud,
    BOOL*                           pui1_base )
{
    return x_scc_aud_get_se_base( h_aud, pui1_base );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_treble
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_treble( 
    HANDLE_T                        h_aud,
    UINT8*                          pui1_treble )
{
    return x_scc_aud_get_se_treble( h_aud, pui1_treble );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_balance
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_balance( 
    HANDLE_T                        h_aud,
    UINT8*                          pui1_balance )
{
    return x_scc_aud_get_se_balance( h_aud, pui1_balance );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_postdr
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_postdr( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    return x_scc_aud_get_se_postdr( h_aud, pb_is_on );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_volume
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_volume           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_volume( 
    HANDLE_T                        h_aud,
    SCC_AUD_VOLUME_INFO_T*          pt_volume )
{
    return x_scc_aud_get_se_volume( h_aud, pt_volume );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_surround
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_volume           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_surround( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    return x_scc_aud_get_se_surround( h_aud, pb_is_on );
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_superwoofer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_superwoofer( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    return x_scc_aud_get_se_superwoofer( h_aud, pb_is_on );
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_downmix_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_downmix_mode( HANDLE_T                h_aud,
                                  SCC_AUD_DOWNMIX_MODE_T* pe_dm_mode )
{
    return x_scc_aud_get_downmix_mode( h_aud,
                                       pe_dm_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_dolby_drc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_dolby_drc( HANDLE_T                   h_aud,
                               SCC_AUD_DOLBY_DRC_T*       pe_drc )
{
    return x_scc_aud_get_dolby_drc( h_aud,
                                    pe_drc );
}

/*------------------------------------------------------------------*/
/*! @brief   get audio compression drc
 *  @param [in]   h_aud               Audio component handle.
 *  @param [out] pe_cmpss_drc         specify compression drc mode.
 *  @note
 *  @see  c_scc_aud_get_cmpss_drc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
INT32 c_scc_aud_get_cmpss_drc( 
    HANDLE_T                   h_aud,
    SCC_AUD_CMPSS_DRC_MODE_T*  pe_cmpss_drc )
{
    return x_scc_aud_get_cmpss_drc( h_aud,
                                    pe_cmpss_drc );
}

/*------------------------------------------------------------------*/
/*! @brief   get audio mpeg level adjustment
 *  @param [in]   h_aud               Audio component handle.
 *  @param [out] pi1_level            specify adjustment level.
 *  @note
 *  @see  c_scc_aud_get_mpeg_level_adj
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
INT32 c_scc_aud_get_mpeg_level_adj( 
    HANDLE_T                    h_aud,
    INT8*                       pi1_level )
{
    return x_scc_aud_get_mpeg_level_adj( h_aud, 
                                         pi1_level );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_output_port
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_output_port( HANDLE_T                 h_aud,
                                 SCC_AUD_OUT_PORT_T       e_port,
                                 BOOL*                    pb_is_active )
{
    return x_scc_aud_get_output_port( h_aud,
                                      e_port,
                                      pb_is_active );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_cmpss_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_cmpss_mode( HANDLE_T                  h_aud,
                                SCC_AUD_CMPSS_MDOE_T*     pe_cmpss_mode )
{
    return x_scc_aud_get_cmpss_mode( h_aud, pe_cmpss_mode );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_agc
 *
 * Description: <function description>
 *
 * Inputs:  h_aud    Audio component handle.
 *
 * Outputs: pb_agc   References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_agc( HANDLE_T                         h_aud,
                         BOOL*                            pb_agc )
{
    return x_scc_aud_get_agc( h_aud, pb_agc );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_chnl_delay
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pt_chnl_delay   References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_chnl_delay( HANDLE_T                  h_aud,
                                SCC_AUD_CHNL_DELAY_T*     pt_chnl_delay )
{
    return x_scc_aud_get_chnl_delay( h_aud, pt_chnl_delay );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_max_chnl_delay
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pui1_max_delay  References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_max_chnl_delay( HANDLE_T              h_aud,
                                    UINT16*               pui2_max_delay )
{
    return x_scc_aud_get_max_chnl_delay( h_aud, pui2_max_delay );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_mts
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pe_mts          References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_mts( 
    HANDLE_T                        h_aud,
    SCC_AUD_MTS_T*                  pe_mts )
{
    return x_scc_aud_get_mts( h_aud, pe_mts );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_eq_user_lvl
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pt_eq_user_lvl  References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_eq_user_lvl( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_QA_USER_LVL_T*       pt_eq_user_lvl )
{
    return x_scc_aud_get_se_eq_user_lvl( h_aud, pt_eq_user_lvl );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_eq_user_info
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pt_eq_user_info References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_eq_user_info( 
    HANDLE_T                        h_aud,
    SCC_AU_SE_QA_USER_BAND_INFO_T*  pt_eq_user_info )
{
    return x_scc_aud_get_se_eq_user_info( h_aud, pt_eq_user_info );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_clip
 *
 * Description: set audio clip playback data
 *
 * Inputs:  h_aud                Audio component handle.
 *          pt_aud_clip          Audio clip data structure     
 *          pf_scc_aud_clip_nfy  SCC Audio clip notify function callback
 *          pv_nfy_tag           notify callback tag
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip(
    HANDLE_T                     h_aud, 
    SCC_AUD_CLIP_T*              pt_aud_clip, 
    x_scc_aud_clip_nfy_fct       pf_scc_aud_clip_nfy, 
    VOID*                        pv_nfy_tag) 
{
    return x_scc_aud_set_clip(h_aud, pt_aud_clip, pf_scc_aud_clip_nfy, pv_nfy_tag);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_clip_ctrl
 *
 * Description: trigger audio clip play control
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_ctrl_mode          play control mode(play/pause/stop)     
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip_ctrl(
    HANDLE_T                      h_aud, 
    SCC_AUD_CTRL_MODE_T           e_ctrl_mode)
{
    return x_scc_aud_set_clip_ctrl(h_aud, e_ctrl_mode);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_clip_ctrl
 *
 * Description: trigger audio clip play control
 *
 * Inputs:  h_aud                Audio component handle.
 *
 * Outputs: pe_ctrl_mode         retrive play control mode(play/pause/stop)     
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_clip_ctrl(
    HANDLE_T                      h_aud, 
    SCC_AUD_CTRL_MODE_T*          pe_ctrl_mode)
{
    return x_scc_aud_get_clip_ctrl(h_aud, pe_ctrl_mode);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_clip_gain
 *
 * Description: The linear gain converted to a fractional fixed point unsigned integer, as described 
 *              in [BD-ROM3] part 1 I.4. The value to pass is the linear gain multiplied by 8192, and 
 *              rounded to the closest integer. 
 *
 * Inputs:  h_aud       Audio component handle.
 *
 * Outputs: ui4_gain    b0 ~b15 => fraction portion, b16~b31 => integer portion
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip_gain(
    HANDLE_T                      h_aud, 
    UINT32                        ui4_gain )
{
    return x_scc_aud_set_clip_gain(h_aud, ui4_gain);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_clip_duration
 *
 * Description: get audio clip duration
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *
 * Outputs: pui4_duration        retrieve the duration of the audio clip(ms)
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_clip_duration(
    HANDLE_T                       h_aud, 
    UINT32*                        pui4_duration) 
{
    return x_scc_aud_get_clip_duration(h_aud, pui4_duration);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_clip_position
 *
 * Description: get audio clip duration
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *
 * Outputs: pui4_duration        retrieve the current position of the audio clip(ms)
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_clip_position(
    HANDLE_T                        h_aud, 
    UINT32*                         pui4_position) 
{
    return x_scc_aud_get_clip_position(h_aud, pui4_position);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_clip_position
 *
 * Description: get audio clip duration
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *
 * Outputs: ui4_duration         the current position of the audio clip(ms)
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip_position(
    HANDLE_T                        h_aud, 
    UINT32                          ui4_position) 
{
    return x_scc_aud_set_clip_position(h_aud, ui4_position);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_clip_volume
 *
 * Description: 
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *             pt_volume          
 * 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip_volume( HANDLE_T                      h_aud,
                                const SCC_AUD_VOLUME_INFO_T*  pt_volume )
{
    return x_scc_aud_set_clip_volume(h_aud, pt_volume);
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_clip_custom_setting
 *
 * Description: 
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *             e_setting_type          
 *             e_setting_mode
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip_custom_setting(
    HANDLE_T                        h_aud,
    SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T              e_setting_type,
    SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T              e_setting_mode )
{
    return x_scc_aud_set_clip_custom_setting(h_aud, e_setting_type,e_setting_mode);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_clip_custom_setting
 *
 * Description: 
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *             e_setting_type          
 * Output:            pe_setting_mode
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_clip_custom_setting(
    HANDLE_T                        h_aud,
    SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T              e_setting_type,
    SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T*              pe_setting_mode )
{
    return x_scc_aud_get_clip_custom_setting(h_aud, e_setting_type,pe_setting_mode);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_pl2_switch
 *
 * Description: set audio Pro Logic II switch
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value               specify mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_pl2_switch( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_SWITCH_T            e_value )
{
    return x_scc_aud_set_pl2_switch(h_aud, e_value);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_pl2_mode
 *
 * Description: set audio Pro Logic II mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_pl2_mode( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_MODE_T              e_value )
{
    return x_scc_aud_set_pl2_mode(h_aud, e_value);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_pl2_panorama
 *
 * Description: set audio Pro Logic II panorama
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value               specify mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_pl2_panorama( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_PANORAMA_T          e_value )
{
    return x_scc_aud_set_pl2_panorama(h_aud, e_value);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_pl2_dimension
 *
 * Description: set audio Pro Logic II dimension
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_pl2_dimension( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_DIMENSION_T         e_value )
{
    return x_scc_aud_set_pl2_dimension(h_aud, e_value);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_pl2_c_width
 *
 * Description: set audio Pro Logic II center width
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_pl2_c_width( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_C_WIDTH_T           e_value )
{
    return x_scc_aud_set_pl2_c_width(h_aud, e_value);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_pl2_switch
 *
 * Description: get audio Pro Logic II switch
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_pl2_switch( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_SWITCH_T*           pe_value )
{
    return x_scc_aud_get_pl2_switch(h_aud, pe_value);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_pl2_mode
 *
 * Description: get audio Pro Logic II mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_pl2_mode( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_MODE_T*             pe_value )
{
    return x_scc_aud_get_pl2_mode(h_aud, pe_value);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_pl2_panorama
 *
 * Description: get audio Pro Logic II panorama
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_pl2_panorama( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_PANORAMA_T*         pe_value )
{
    return x_scc_aud_get_pl2_panorama(h_aud, pe_value);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_pl2_dimension
 *
 * Description: get audio Pro Logic II dimenstion
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_pl2_dimension( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_DIMENSION_T*        pe_value )
{
    return x_scc_aud_get_pl2_dimension(h_aud, pe_value);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_pl2_c_width
 *
 * Description: get audio Pro Logic II center width
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_pl2_c_width( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_C_WIDTH_T*          pe_value )
{
    return x_scc_aud_get_pl2_dimension(h_aud, (SCC_AUD_PL2_DIMENSION_T*)pe_value);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_dolby_certi_mode
 *
 * Description: get dolby certify mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          pb_mode              retrive mode
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_dolby_certi_mode( 
   HANDLE_T                        h_aud,
   BOOL*                           pb_mode )
{
    return x_scc_aud_get_dolby_certi_mode(h_aud, pb_mode);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_dolby_certi_mode
 *
 * Description: set dolby certify mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          b_mode               set mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_dolby_certi_mode( 
    HANDLE_T                        h_aud,
    BOOL                            b_mode )
{
    return x_scc_aud_set_dolby_certi_mode(h_aud, b_mode);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_output_port_adj
 *
 * Description: set output port whether is adjust or not
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_port               specify set outport  
 *          b_is_fixed           specify is fixed or not for the port
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_output_port_adj( 
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_PORT_T              e_port,
    BOOL                            b_is_fixed )
{
    return x_scc_aud_set_output_port_adj(h_aud, e_port, b_is_fixed);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_output_port_adj
 *
 * Description: get output port whether is adjust or not
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_port               specify set outport  
 *
 * Outputs: 
 *          pb_is_fixed          retrive is fixed or not for the port
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_output_port_adj( 
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_PORT_T              e_port,
    BOOL*                           pb_is_fixed )
{
    return x_scc_aud_get_output_port_adj(h_aud, e_port, pb_is_fixed);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_capability
 *
 * Description: get audio capability
 *
 * Inputs:  h_aud                Audio component handle.
 *          pt_capability        retrive the audio capability
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_capability( 
    HANDLE_T                        h_aud,
    SCC_AUD_CAPABILITY_T*           pt_capability )
{
     return x_scc_aud_get_capability(h_aud, pt_capability);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_bbe_mode
 *
 * Description: set bbe mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_mode               set bbe mode.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_bbe_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_BBE_MODE_T              e_mode )
{
     return x_scc_aud_set_bbe_mode(h_aud, e_mode);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_bbe_mode
 *
 * Description: set bbe mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_mode              get bbe mode.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_get_bbe_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_BBE_MODE_T*             pe_mode )
{
     return x_scc_aud_get_bbe_mode(h_aud, pe_mode);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_hdphone_detect_nfy
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_hdphone_detect_nfy( 
    HANDLE_T                           h_aud,
    SCC_AUD_HDPHONE_DETECT_NFT_INFO_T* pt_hdph_nfy_info )
{
     return x_scc_aud_get_hdphone_detect_nfy(h_aud, pt_hdph_nfy_info);
}
    
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_hdphone_cond
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_hdphone_cond( 
    HANDLE_T                        h_aud,
    SCC_AUD_HDPHONE_COND_T*         pe_hdphone_cond )
{
     return x_scc_aud_get_hdphone_cond(h_aud, pe_hdphone_cond);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_mute_ex
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_mute_ex( 
    HANDLE_T                        h_aud,
    UINT32*                         pui4_muted_out_port_msk )
{
     return x_scc_aud_get_mute_ex(h_aud, pui4_muted_out_port_msk);
} 
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_lineout_vol_mode
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.    
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_lineout_vol_mode( 
    HANDLE_T                        h_aud,
     SCC_AUD_DEC_LINEOUT_VOL_T*  pt_lineout_vol_mode)
{
     return x_scc_aud_set_lineout_vol_mode(h_aud, pt_lineout_vol_mode);
} 
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_sph_mode
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.    
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_sph_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_DEC_SE_SPH_MODE_T        e_mode )
{
    return x_scc_aud_set_se_sph_mode(h_aud, e_mode);
}

 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_se_volume_offset
  *
  * Description: 
  *
  * Inputs:  
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.    
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_se_volume_offset(
     HANDLE_T                        h_aud,
     UINT8                           ui1_offset )
 {
     return x_scc_aud_set_se_volume_offset(h_aud, ui1_offset);
 }
 
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_se_hp_volume_offset
  *
  * Description: 
  *
  * Inputs:  
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.    
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_se_hp_volume_offset(
     HANDLE_T                        h_aud,
     UINT8                           ui1_offset )
 {
     return x_scc_aud_set_se_hp_volume_offset(h_aud, ui1_offset);
 }

 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_peq_info
  *
  * Description: 
  *
  * Inputs:  
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.    
  *
  ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_peq_info( 
    HANDLE_T                 h_aud,
   SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T*  pt_peq_info)
{
    return x_scc_aud_set_peq_info(h_aud, pt_peq_info);
}
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_ovm
  *
  * Description: 
  *
  * Inputs:  
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.    
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_ovm(
     HANDLE_T                        h_aud,
     UINT32                           ui4_value )
{
     return x_scc_aud_set_ovm(h_aud, ui4_value);
}
/*-----------------------------------------------------------------------------
* Name: c_scc_aud_set_btnsnd_mode
*
* Description: 
*
* Inputs:  
*
* Outputs: 
*
* Returns: SMR_OK              Success.
*          SMR_INTERNAL_ERROR  Fail.    
*
----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_btnsnd_mode( HANDLE_T                  h_aud,
                          SCC_AUD_BTNSND_MODE_T             e_btnsnd_mode )
{
    return x_scc_aud_set_btnsnd_mode(h_aud, e_btnsnd_mode);
}
/*-----------------------------------------------------------------------------
* Name: c_scc_aud_get_btnsnd_mode
*
* Description: 
*
* Inputs:  
*
* Outputs: 
*
* Returns: SMR_OK              Success.
*          SMR_INTERNAL_ERROR  Fail.    
*
----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_btnsnd_mode( HANDLE_T                  h_aud,
                         SCC_AUD_BTNSND_MODE_T*             pe_btnsnd_mode )
{
    return x_scc_aud_get_btnsnd_mode(h_aud, pe_btnsnd_mode);
}

/*-----------------------------------------------------------------------------
* Name: c_scc_aud_set_btnsnd_play
*
* Description: 
*
* Inputs:  
*
* Outputs: 
*
* Returns: SMR_OK              Success.
*          SMR_INTERNAL_ERROR  Fail.    
*
----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_btnsnd_play( HANDLE_T                  h_aud )
{
    return x_scc_aud_set_btnsnd_play(h_aud);
}
/*-----------------------------------------------------------------------------
* Name: c_scc_aud_set_btnsnd_enable
*
* Description: 
*
* Inputs:  
*
* Outputs: 
*
* Returns: SMR_OK              Success.
*          SMR_INTERNAL_ERROR  Fail.    
*
----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_btnsnd_enable( HANDLE_T                  h_aud,
                          SCC_AUD_BTNSND_ENABLE_T             e_btnsnd_enable )
{
    return x_scc_aud_set_btnsnd_enable(h_aud, e_btnsnd_enable);
}
/*-----------------------------------------------------------------------------
* Name: c_scc_aud_get_btnsnd_enable
*
* Description: 
*
* Inputs:  
*
* Outputs: 
*
* Returns: SMR_OK              Success.
*          SMR_INTERNAL_ERROR  Fail.    
*
----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_btnsnd_enable( HANDLE_T                  h_aud,
                         SCC_AUD_BTNSND_ENABLE_T*             pe_btnsnd_enable )
{
    return x_scc_aud_get_btnsnd_enable(h_aud, pe_btnsnd_enable);
}

/*-----------------------------------------------------------------------------
* Name: c_scc_aud_set_av_sync_mode
*
* Description: 
*
* Inputs:  
*
* Outputs: 
*
* Returns: SMR_OK              Success.
*          SMR_INTERNAL_ERROR  Fail.    
*
----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_av_sync_mode( HANDLE_T                  h_aud,
                             SCC_AUD_DEC_AV_SYNC_INFO_T             e_av_sync_mode )
{
    return x_scc_aud_set_av_sync_mode(h_aud, e_av_sync_mode);
}
/*-----------------------------------------------------------------------------
* Name: c_scc_aud_get_av_sync_mode
*
* Description: 
*
* Inputs:  
*
* Outputs: 
*
* Returns: SMR_OK              Success.
*          SMR_INTERNAL_ERROR  Fail.    
*
----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_av_sync_mode(
        HANDLE_T                        h_aud,
        SCC_AUD_DEC_AV_SYNC_INFO_T*             pe_av_sync_mode )
{
    return x_scc_aud_get_av_sync_mode(h_aud, pe_av_sync_mode);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_power_on_off_music_volume
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_power_on_off_music_volume( HANDLE_T                      h_aud,
                            SCC_AUD_VOLUME_INFO_T*        pt_volume )
{
    return x_scc_aud_get_power_on_off_music_volume( h_aud,
                                 pt_volume );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_upload_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_upload_ctrl(
        HANDLE_T                        h_aud,
        BOOL                            b_is_on )
{
    return x_scc_aud_set_upload_ctrl( h_aud,
                                 b_is_on );
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_upload_data_buffer_info
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_upload_data_buffer_info(
        HANDLE_T                        h_aud,
        SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T*           pt_buffer_info )
{
    return x_scc_aud_get_upload_data_buffer_info( h_aud,
                                 pt_buffer_info );
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_upload_data_nfy
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_upload_data_nfy( 
    HANDLE_T                           h_aud,
    SCC_AUD_UPLOAD_DATA_NFT_INFO_T* pt_upload_data_nfy_info )
{
    return x_scc_aud_set_upload_data_nfy( h_aud, pt_upload_data_nfy_info );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_upload_pcm_info
 *
 * Description: <function description>
 *
 * Inputs:  h_aud        Audio component handle.
 *
 * Outputs: pt_pcm_info  References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_upload_pcm_info( 
    HANDLE_T                        h_aud,
    SCC_AUD_UPLOAD_PCM_INFO_T*      pt_pcm_info )
{
    return x_scc_aud_get_upload_pcm_info( h_aud, pt_pcm_info );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_karaoke
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_karaoke(
        HANDLE_T                        h_aud,
        SCC_AUD_KARAOKE_INFO_T*        pt_karaoke_info )
{
    return x_scc_aud_set_karaoke( h_aud,
                                  pt_karaoke_info );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_out_port_link
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_out_port_link(
    HANDLE_T                        h_aud,
    BOOL                            b_enable )
{
    return x_scc_aud_set_out_port_link( h_aud,
                                        b_enable );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_dc_power_off
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_dc_power_off(
    HANDLE_T                        h_aud,
    BOOL                            b_enable )
{
    return x_scc_aud_set_dc_power_off( h_aud, b_enable );
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_endian_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_endian_mode(
    HANDLE_T                           h_aud,
    SCC_AUD_ENDIAN_MODE_T              e_mode )
{
    return x_scc_aud_set_endian_mode(h_aud, e_mode);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_out_device
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_out_device(
    HANDLE_T                           h_aud,
    SCC_AUD_OUT_DEVICE_T              e_out_device )
{
    return x_scc_aud_set_out_device(h_aud, e_out_device);
}

/*-----------------------------------------------------------------------------
* Name: c_scc_query_bootup_status
*
* Description: 
*
* Inputs:  
*
* Outputs: 
*
* Returns: SMR_OK              Success.
*          SMR_INTERNAL_ERROR  Fail.    
*
----------------------------------------------------------------------------*/
INT32 c_scc_query_bootup_status( UINT8  ui1_mode, UINT8* pui1_status )
{
    return x_scc_query_bootup_status(ui1_mode, pui1_status);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_set_crnt_inp_src_info
 *
 * Description: <function description> for set current input source info
 *
 * Inputs: e_type               References
 *            ui1_port_num     References
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_set_crnt_inp_src_info(
    SCC_CRNT_INP_SRC_TYPE_T             e_inp_src_type, 
    UINT8                               ui1_port_num)
{
    return x_scc_set_crnt_inp_src_info(e_inp_src_type, ui1_port_num);
}

