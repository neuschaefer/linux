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
#include "handle/handle.h"
#include "strm_mngr/scc/c_scc.h"
#include "strm_mngr/scc/x_scc.h"
#include "strm_mngr/u_sm.h"
#include "strm_mngr/cc_hdlr/u_sm_cc_hdlr.h"
#include "strm_mngr/cc_hdlr/x_sm_cc_hdlr.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "strm_mngr/x_sm.h"
#include "inc/x_mm_common.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"
#include "rh_cb_wrapper.h"

INT32 _hndlr_x_scc_comp_close(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_comp_close(
        (HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}


INT32 _hndlr_x_scc_comp_open(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }   
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_comp_open(
        (SM_COMMAND_T *)pt_args[0].u.pv_desc,
            (HANDLE_T *)pt_args[1].u.pui4_arg);

    return RPCR_OK;
}


INT32 _hndlr_x_scc_comp_grp_set(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_comp_grp_set(
        (SM_COMMAND_T *)pt_args[0].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_comp_att_incr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_comp_att_incr(
                             (HANDLE_T)pt_args[0].u.ui4_arg,
                             (SM_CMD_CODE_T)pt_args[1].u.ui4_arg,
                                     (VOID*)pt_args[2].u.ui4_arg,
                                            pt_args[3].u.pui2_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_comp_att_decr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_comp_att_decr(
                                  (HANDLE_T)pt_args[0].u.ui4_arg,
                             (SM_CMD_CODE_T)pt_args[1].u.ui4_arg,
                                     (VOID*)pt_args[2].u.ui4_arg,
                                            pt_args[3].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_comp_att_get_num_steps(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_comp_att_get_num_steps(
                                  (HANDLE_T)pt_args[0].u.ui4_arg,
                             (SM_CMD_CODE_T)pt_args[1].u.ui4_arg,
                                     (VOID*)pt_args[2].u.ui4_arg,
                                            pt_args[3].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_vdp_atomic_cmd(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_vdp_atomic_cmd(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SM_COMMAND_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_pic_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_pic_mode(
                  (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_PIC_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}


INT32 _hndlr_x_scc_vid_set_hdmi_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_hdmi_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_HDMI_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_color_sys(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_color_sys(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_COLOR_SYS_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_scr_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_scr_mode(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_SCREEN_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_mode(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_mpeg_nr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_mpeg_nr(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_MPEG_NR_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_blue_stretch(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_blue_stretch(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_BLUE_STRETCH_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_game_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_game_mode(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_GAME_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_wcg(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_wcg(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_WCG_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_xvycc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_xvycc(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_XVYCC_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_di_edge_preserve(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_di_edge_preserve(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_DI_EDGE_PRESERVE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}


INT32 _hndlr_x_scc_vid_set_di_ma(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_di_ma(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_DI_MA_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_di_film_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_di_film_mode(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_DI_FILM_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_iframe_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_iframe_ctrl(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_IFRAME_CTRL_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_vdrips_play_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_vdrips_play_ctrl(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_VDRIPS_PLAY_CTRL_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_iframe_codec(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_iframe_codec(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_IFRAME_CODEC_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

#ifdef SYS_MHP_SUPPORT
INT32 _hndlr_x_scc_vid_set_iframe_not_display(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_iframe_not_display(
          (HANDLE_T)pt_args[0].u.ui4_arg,
                    pt_args[1].u.b_arg);

    return RPCR_OK;
}
#endif

INT32 _hndlr_x_scc_vid_set_split_scrn_demo(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_split_scrn_demo(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_SPLIT_SCRN_DEMO_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_reset_vga_timing_option(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_reset_vga_timing_option(
        (HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_h_position(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_h_position(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_v_position(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_v_position(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_mode_capability(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_mode_capability(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_h_position(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_h_position(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg,
                  pt_args[2].u.pui2_arg,
                  pt_args[3].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_v_position(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_v_position(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg,
                  pt_args[2].u.pui2_arg,
                  pt_args[3].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_h_size(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_h_size(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg,
                  pt_args[2].u.pui2_arg,
                  pt_args[3].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_v_size(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_v_size(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg,
                  pt_args[2].u.pui2_arg,
                  pt_args[3].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_enhance(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_enhance(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_blur(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_blur(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_cti(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_cti(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_eti(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_eti(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_vdrips_duration(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_vdrips_duration(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_sharpness(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_sharpness(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_color_suppress(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_color_suppress(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_nr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_nr(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3dnr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3dnr(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_black_lvl_ext(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_black_lvl_ext(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_flesh_tone(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_flesh_tone(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_white_peak_rdct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_white_peak_rdct(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_luma(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_luma(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_dynamic_scaling(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_dynamic_scaling(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_h_size(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_h_size(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_v_size(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_v_size(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_letter_box_detect(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_letter_box_detect(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_film_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_film_mode(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_force_unmute_on_no_sig(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_force_unmute_on_no_sig(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.b_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_set_srm_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_srm_mode(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_SRM_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_get_film_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_film_mode(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_hdcp_key_exist(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_hdcp_key_exist(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_letter_box_detect(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_letter_box_detect(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_force_unmute_on_no_sig(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_force_unmute_on_no_sig(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_ambig_vga_timing_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_ambig_vga_timing_ctrl(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_super_freeze(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_super_freeze(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_blur(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_blur(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_cti(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_cti(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_eti(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_eti(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_sharpness(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_sharpness(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_color_supress(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_color_supress(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_nr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_nr(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3dnr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3dnr(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_black_lvl_ext(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_black_lvl_ext(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_white_peak_rdct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_white_peak_rdct(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_flesh_tone(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_flesh_tone(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_luma(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_luma(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_dynamic_scaling(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_dynamic_scaling(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_enhance(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_enhance(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_iframe_resolution(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_iframe_resolution(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui4_arg,
                  pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_bg(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_bg(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (const SCC_BG_COLOR_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_super_blank(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_super_blank(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (const SCC_VID_SUPER_BLANK_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_overscan_clipper(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_overscan_clipper(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (const SCC_VID_OVER_SCAN_CLIPPER_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_lb_detect_config(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_lb_detect_config(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_LB_DETECT_CONFIG_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_deblocking(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_deblocking(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_DEBLOCKING_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_super_freeze(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_super_freeze(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (const SCC_VID_SUPER_FREEZE_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_mjc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_mjc(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (const SCC_VID_MJC_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_mode_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_mode_ex(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (const SCC_VID_MODE_EX_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_set_sig_booster(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_sig_booster(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.b_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_set_iframe_disp_region(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_iframe_disp_region(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (VSH_REGION_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_iframe_src_region(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_iframe_src_region(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (VSH_REGION_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_iframe_pla_order(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_iframe_pla_order(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (VSH_SET_PLANE_ORDER_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_iframe_alloc_buffer(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_iframe_alloc_buffer(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_IFRAME_BUFF_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_iframe_buffer(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_iframe_buffer(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_IFRAME_BUFF_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_iframe_decode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_iframe_decode(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_IFRAME_BUFF_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_iframe_free_buffer(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_iframe_free_buffer(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_IFRAME_BUFF_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_vdrips_feed(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_vdrips_feed(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_VDRIPS_BUFF_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_mode(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_bg(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_bg(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_BG_COLOR_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_pic_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_pic_mode(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_PIC_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}


INT32 _hndlr_x_scc_vid_get_hdmi_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_hdmi_mode(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_HDMI_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_color_sys(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_color_sys(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_COLOR_SYS_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_scr_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_scr_mode(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_SCREEN_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_overscan_clipper(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_overscan_clipper(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_OVER_SCAN_CLIPPER_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_mjc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_mjc(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_MJC_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_lb_detect_config(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_lb_detect_config(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_LB_DETECT_CONFIG_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_deblocking(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_deblocking(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_DEBLOCKING_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_iframe_disp_region(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_iframe_disp_region(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( VSH_REGION_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_iframe_src_region(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_iframe_src_region(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( VSH_REGION_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_split_scrn_demo(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_split_scrn_demo(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_VID_SPLIT_SCRN_DEMO_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_iframe_nfy(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_iframe_nfy(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
        (x_scc_vid_iframe_nfy_fct)pt_args[1].u.pv_func,
                           (VOID*)pt_args[2].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_vdrips_nfy(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_vdrips_nfy(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
        (x_scc_vid_vdrips_nfy_fct)pt_args[1].u.pv_func,
                           (VOID*)pt_args[2].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_iframe_surface(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_iframe_surface(
        (HANDLE_T)pt_args[0].u.ui4_arg,
        (HANDLE_T)pt_args[1].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_di_film_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_di_film_mode(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_DI_FILM_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_di_ma(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_di_ma(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_DI_MA_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_di_edge_preserve(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_di_edge_preserve(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_DI_EDGE_PRESERVE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_xvycc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_xvycc(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_XVYCC_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_wcg(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_wcg(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_WCG_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_game_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_game_mode(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_GAME_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_blue_stretch(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_blue_stretch(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_BLUE_STRETCH_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_mpeg_nr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_mpeg_nr(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        ( SCC_VID_MPEG_NR_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_set_black_stretch(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_black_stretch(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_VID_BLACK_STRETCH_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_get_black_stretch(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_black_stretch(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_VID_BLACK_STRETCH_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_set_white_stretch(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_white_stretch(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_VID_WHITE_STRETCH_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_get_white_stretch(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_white_stretch(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_VID_WHITE_STRETCH_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_black_stretch_val(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_black_stretch_val(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                       pt_args[1].u.ui2_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_get_black_stretch_val(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_black_stretch_val(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                       pt_args[1].u.pui2_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_set_white_stretch_val(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_white_stretch_val(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                       pt_args[1].u.ui2_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_get_white_stretch_val(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_white_stretch_val(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                       pt_args[1].u.pui2_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_set_colorspace(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_colorspace(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_VID_COLORSPACE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_set_flip(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_flip(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.b_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_set_mirror(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_mirror(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.b_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_get_flip(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_flip(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.pb_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_vid_get_mirror(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_mirror(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_local_dimming_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_local_dimming_mode(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                              (SCC_VID_LOCAL_DIMMING_TYPE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_local_dimming_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_local_dimming_mode(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                        (SCC_VID_LOCAL_DIMMING_TYPE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

static VOID _x_scc_vid_src_detect_nfy_fct_wrapper(
    HANDLE_T                         h_vid,
    SCC_VID_SRC_DETECT_NFY_COND_T    e_nfy_cond,
    VOID*                            pv_nfy_tag,
    UINT32                           ui4_data_1,
    UINT32                           ui4_data_2 )
{    
    RPC_DECL_VOID(5);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP( ARG_TYPE_UINT32,   h_vid );
    RPC_ARG_INP( ARG_TYPE_UINT32,   e_nfy_cond );    
    RPC_ARG_INP( ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag );
    RPC_ARG_INP( ARG_TYPE_UINT32,   ui4_data_1 );
    RPC_ARG_INP( ARG_TYPE_UINT32,   ui4_data_2 );

    RPC_DO_CB( pt_nfy_tag->t_id, "x_scc_vid_src_detect_nfy_fct", pt_nfy_tag->pv_cb_addr );
    
    RPC_RETURN_VOID;    
}

static INT32 _hndlr_x_scc_vid_set_src_detect_nfy(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *              pt_nfy_tag;
    x_scc_vid_src_detect_nfy_fct    pf_nfy;
    
    if( ui4_num_args != 3 )
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = RPCR_INV_ARGS;    
        return RPCR_INV_ARGS;
    }      
    if( pt_args[1].u.pv_func == NULL )
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = (x_scc_vid_src_detect_nfy_fct)_x_scc_vid_src_detect_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag( t_rpc_id, (VOID**)(&pt_args[1].u.pv_func), 1, pt_args[2].u.pv_arg );
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_src_detect_nfy(
                                (HANDLE_T)pt_args[0].u.ui4_arg,
                                pf_nfy,
                                pt_nfy_tag);

    if( pt_return->u.i4_arg == SMR_OK )
    {
        ri_bind_cb_tag( (HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag );
    }
    else if( pt_nfy_tag != NULL )
    {
        ri_free_cb_tag( pt_nfy_tag );
    }

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_nav(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_nav(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                  (SCC_3D_NAV_T)pt_args[1].u.i4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_nav(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_nav(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                 (SCC_3D_NAV_T*)pt_args[1].u.pi4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_mode(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                 (SCC_3D_MODE_T)pt_args[1].u.i4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_mode(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                (SCC_3D_MODE_T*)pt_args[1].u.pi4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_parallax(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_parallax(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.ui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_parallax(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_parallax(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.pui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_fld_depth(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_fld_depth(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.ui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_fld_depth(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_fld_depth(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.pui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_lr_switch(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_lr_switch(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
            (SCC_3D_LR_SWITCH_T)pt_args[1].u.i4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_lr_switch(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_lr_switch(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
           (SCC_3D_LR_SWITCH_T*)pt_args[1].u.pi4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_to_2d(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_to_2d(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                (SCC_3D_TO_2D_T)pt_args[1].u.i4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_to_2d(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_to_2d(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
               (SCC_3D_TO_2D_T*)pt_args[1].u.pi4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_fpr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_fpr(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                  (SCC_3D_FPR_T)pt_args[1].u.i4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_fpr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_fpr(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                 (SCC_3D_FPR_T*)pt_args[1].u.pi4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_protrude_scr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_protrude_scr(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.ui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_protrude_scr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_protrude_scr(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.pui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_distance_2_tv(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_distance_2_tv(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.ui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_distance_2_tv(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_distance_2_tv(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.pui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_osd_depth(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_osd_depth(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.ui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_osd_depth(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_osd_depth(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.pui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_fmt_cap(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_fmt_cap(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.pui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_ctrl_cap(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_ctrl_cap(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.pui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_nav_auto_chg_en(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_nav_auto_chg_en(
                          (HANDLE_T)pt_args[0].u.ui4_arg,
             (SCC_3D_NAV_AUTO_CHG_T)pt_args[1].u.i4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_nav_auto_chg_en(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_nav_auto_chg_en(
                          (HANDLE_T)pt_args[0].u.ui4_arg,
            (SCC_3D_NAV_AUTO_CHG_T*)pt_args[1].u.pi4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_3d_image_safety_en(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_3d_image_safety_en(
                          (HANDLE_T)pt_args[0].u.ui4_arg,
             (SCC_3D_IMAGE_SAFETY_T)pt_args[1].u.i4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_image_safety_en(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_image_safety_en(
                          (HANDLE_T)pt_args[0].u.ui4_arg,
            (SCC_3D_IMAGE_SAFETY_T*)pt_args[1].u.pi4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_nav_tag(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_nav_tag(
                          (HANDLE_T)pt_args[0].u.ui4_arg,
                 (SCC_3D_NAV_TAG_T*)pt_args[1].u.pi4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_pedestal(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_pedestal(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_hdmi_range(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_hdmi_range(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
              (SCC_HDMI_RANGE_T)pt_args[1].u.i4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_overscan_enable(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_overscan_enable(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_nonlinear_scaling_enable(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_nonlinear_scaling_enable(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_panel_type(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_panel_type(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
          (SCC_3D_PANEL_TYPE_T*)pt_args[1].u.pi4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_vga_detect(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
     if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_vga_detect(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
              (SCC_VGA_DETECT_T)pt_args[1].u.i4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_vga_status(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_vga_status(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
             (SCC_VGA_STATUS_T*)pt_args[1].u.pi4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_static_gamma(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
     if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_static_gamma(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.ui1_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_angular_sce(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_angular_sce(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
            (SCC_ANGULAR_SCE_T*)pt_args[1].u.pv_desc );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_down_bw_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_down_bw_mode(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.ui1_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_3d_mjc_enable(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_3d_mjc_enable(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_freq(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_freq(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
               (SCC_VID_FREQ_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_src_detect_status(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_src_detect_status(
                          (HANDLE_T)pt_args[0].u.ui4_arg,
         (SCC_VID_SRC_DET_STATUS_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_set_super_resolution(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_set_super_resolution(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_asp_ratio_enable(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_asp_ratio_enable(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_vid_get_screen_mode_enable(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_vid_get_screen_mode_enable(
                    (HANDLE_T)pt_args[0].u.ui4_arg,
         (SCC_VID_SCR_MODE_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_query_bootup_status(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_query_bootup_status(
                               (UINT8)pt_args[0].u.ui1_arg,
                        (UINT8*)pt_args[1].u.pui1_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_set_crnt_inp_src_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_set_crnt_inp_src_info(
                (SCC_CRNT_INP_SRC_TYPE_T)pt_args[0].u.i4_arg,
                                         pt_args[1].u.ui1_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_attrib(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_attrib(
                     (HANDLE_T)pt_args[0].u.ui4_arg,
            (SCC_DISP_ATTRIB_T)pt_args[1].u.i4_arg,
                               pt_args[2].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_adp_back_light_rng(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_adp_back_light_rng(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_DISP_RANGE_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_resolution(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_resolution(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_DISP_RESOLUTION_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_frame_rate(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_frame_rate(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.ui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_frame_rate(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_frame_rate(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.pui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_force_frame_rate(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_force_frame_rate(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                pt_args[1].u.ui4_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_aspect_ratio(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_aspect_ratio(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                     (SCC_DISP_ASPECT_RATIO_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_fmt(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_fmt(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                              (SCC_DISP_FMT_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_tv_type(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_tv_type(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                          (SCC_DISP_TV_TYPE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_bg(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_bg(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                             (SCC_BG_COLOR_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_digit_output_fmt(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_digit_output_fmt(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                 (SCC_DISP_DIGIT_OUTPUT_FMT_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_color_gain(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_color_gain(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_DISP_COLOR_GAIN_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_color_offset(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_color_offset(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_DISP_COLOR_GAIN_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_tv_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_tv_mode(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                               (SCC_TV_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_mjc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_mjc(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                              (SCC_DISP_MJC_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_test_pattern(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_test_pattern(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                     (SCC_DISP_TEST_PATTERN_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_set_back_light_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_set_back_light_ctrl(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                                              pt_args[1].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_back_light_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_back_light_ctrl(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                                         pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_test_pattern(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_test_pattern(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
               (SCC_DISP_TEST_PATTERN_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_mjc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_mjc(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                        (SCC_DISP_MJC_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_tv_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_tv_mode(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                         (SCC_TV_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_color_offset(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_color_offset(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                 (SCC_DISP_COLOR_GAIN_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_color_gain(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_color_gain(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                 (SCC_DISP_COLOR_GAIN_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_digit_output_fmt(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_digit_output_fmt(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
           (SCC_DISP_DIGIT_OUTPUT_FMT_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_dither_cap(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_dither_cap(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                                         pt_args[1].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_fmt_cap(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_fmt_cap(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                    (SCC_DISP_FMT_CAP_T*)pt_args[1].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_bg(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_bg(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                        (SCC_BG_COLOR_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_tv_type(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_tv_type(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                    (SCC_DISP_TV_TYPE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_fmt(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_fmt(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                        (SCC_DISP_FMT_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_aspect_ratio(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_aspect_ratio(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
               (SCC_DISP_ASPECT_RATIO_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_resolution(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_resolution(
                               (HANDLE_T)pt_args[0].u.ui4_arg,
                 (SCC_DISP_RESOLUTION_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_adp_back_light_rng(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_adp_back_light_rng(
                                    (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_DISP_RANGE_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_attrib(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_attrib(
                     (HANDLE_T)pt_args[0].u.ui4_arg,
            (SCC_DISP_ATTRIB_T)pt_args[1].u.i4_arg,
                               pt_args[2].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_max_gamma(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_max_gamma(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pui1_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_disp_get_screen_resolution(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_disp_get_screen_resolution(
                                      (HANDLE_T)pt_args[0].u.ui4_arg,
                 (SCC_DISP_SCREEN_RESOLUTION_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_volume(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_volume(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_VOLUME_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_volume_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_volume_ex(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_VOLUME_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_mute(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_mute(
                      (HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_unmute(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_unmute(
                      (HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_spdif_fmt(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_spdif_fmt(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_SPDIF_FMT_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_hdcd_filter(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_hdcd_filter(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_HDCD_FLTR_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_loudspeaker(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_loudspeaker(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_LOUD_SPEAKER_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_trim(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_trim(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_SE_TRIM_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_superbase(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_superbase(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.b_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_equalizer(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_equalizer(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_SE_EQ_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_reverb(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_reverb(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_SE_REVERB_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_base(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_base(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.ui1_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_treble(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_treble(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.ui1_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_balance(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_balance(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.ui1_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_postdr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_postdr(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.b_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_volume(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_volume(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_VOLUME_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_surround(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_surround(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_se_superwoofer(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_superwoofer(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_panfade(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_panfade(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_se_sound_mode_start( 
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_sound_mode_start(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_srs_bypass(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_srs_bypass(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.b_arg);

    return RPCR_OK;
}


INT32 _hndlr_x_scc_aud_set_se_sound_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_sound_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_SE_SOUND_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_surround_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_surround_ex(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_SE_SURROUND_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_downmix_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_downmix_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_DOWNMIX_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_dolby_drc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_dolby_drc(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_DOLBY_DRC_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_output_port(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_output_port(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
         (SCC_AUD_OUT_PORT_T)pt_args[1].u.i4_arg,
                             pt_args[2].u.b_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_output_port_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_output_port_ex(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.ui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_cmpss_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_cmpss_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_CMPSS_MDOE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_agc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_agc(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.b_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_mts(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_mts(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
              (SCC_AUD_MTS_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_eq_user_lvl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_eq_user_lvl(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_SE_QA_USER_LVL_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_eq_user_lvl_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_eq_user_lvl_ex(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_SE_QA_USER_LVL_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_chnl_delay(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_chnl_delay(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_CHNL_DELAY_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

static VOID _x_scc_aud_hdphone_detect_nfy_fct_wrapper (
    VOID*                     pv_nfy_tag,
    SCC_AUD_HDPHONE_COND_T    e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2)
{
    RPC_DECL_VOID(4);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_INT32,    e_nfy_cond);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data_1);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data_2);    

    RPC_DO_CB(pt_nfy_tag->t_id, "x_scc_aud_hdphone_detect_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID;    
}
static VOID _x_scc_aud_upload_data_nfy_fct_wrapper (
    VOID*                     pv_nfy_tag,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2)
{
    RPC_DECL_VOID(3);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data_1);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data_2);    

    RPC_DO_CB(pt_nfy_tag->t_id, "x_scc_aud_upload_data_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID;    
}


INT32 _hndlr_x_scc_aud_set_hdphone_detect_nfy(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    SCC_AUD_HDPHONE_DETECT_NFT_INFO_T   t_nfy_tmp;
    SCC_AUD_HDPHONE_DETECT_NFT_INFO_T * pt_nfy;
    RPC_CB_NFY_TAG_T *                  pt_nfy_tag = NULL;
       
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      

    pt_nfy = (SCC_AUD_HDPHONE_DETECT_NFT_INFO_T *)pt_args[1].u.pv_desc;

    if(pt_nfy->pf_hdphone_detect_nfy == NULL)
    {
        t_nfy_tmp.pf_hdphone_detect_nfy = NULL;
        t_nfy_tmp.pv_nfy_tag            = pt_nfy->pv_nfy_tag;
    }
    else
    {
        /* RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_nfy->pf_hdphone_detect_nfy, pt_nfy->pv_nfy_tag); */
        pt_nfy_tag = ri_create_cb_tag(t_rpc_id, (VOID*)&pt_nfy->pf_hdphone_detect_nfy, 1, pt_nfy->pv_nfy_tag);
        t_nfy_tmp.pf_hdphone_detect_nfy = _x_scc_aud_hdphone_detect_nfy_fct_wrapper;
        t_nfy_tmp.pv_nfy_tag            = pt_nfy_tag;    
    }
    
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_hdphone_detect_nfy(
                            (HANDLE_T)pt_args[0].u.ui4_arg,
                            &t_nfy_tmp);    
                     
    if(pt_return->u.i4_arg == SMR_OK)
    {
        ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag);
    }
    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_upload_data_nfy(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    SCC_AUD_UPLOAD_DATA_NFT_INFO_T   t_nfy_tmp;
    SCC_AUD_UPLOAD_DATA_NFT_INFO_T * pt_nfy;
    RPC_CB_NFY_TAG_T *                  pt_nfy_tag = NULL;
    
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      

    pt_nfy = (SCC_AUD_UPLOAD_DATA_NFT_INFO_T *)pt_args[1].u.pv_desc;

    if(pt_nfy->pf_upload_data_nfy== NULL)
    {
        t_nfy_tmp.pf_upload_data_nfy = NULL;
        t_nfy_tmp.pv_nfy_tag= pt_nfy->pv_nfy_tag;
    }
    else
    {
        /*RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_nfy->pf_upload_data_nfy, pt_nfy->pv_nfy_tag);*/
        pt_nfy_tag = ri_create_cb_tag(t_rpc_id, (VOID*)&pt_nfy->pf_upload_data_nfy, 1, pt_nfy->pv_nfy_tag);
        t_nfy_tmp.pf_upload_data_nfy =_x_scc_aud_upload_data_nfy_fct_wrapper;
        t_nfy_tmp.pv_nfy_tag            = pt_nfy_tag;    
    }
    
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_upload_data_nfy(
                            (HANDLE_T)pt_args[0].u.ui4_arg,
                            &t_nfy_tmp);    
                     
    if(pt_return->u.i4_arg == SMR_OK)
    {
        ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag);
    }
    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_get_upload_pcm_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_upload_pcm_info(
                           (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_UPLOAD_PCM_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_mute_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_mute_ex(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.ui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_volume(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_volume(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_VOLUME_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_is_muted(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_is_muted(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);
                  
    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_spdif_fmt(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_spdif_fmt(
                     (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_SPDIF_FMT_T *)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_hdcd_filter(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_hdcd_filter(
                     (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_HDCD_FLTR_T *)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_loudspeaker(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_loudspeaker(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_LOUD_SPEAKER_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_trim(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_trim(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_SE_TRIM_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_superbase(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_superbase(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.pb_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_equalizer(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_equalizer(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_AUD_SE_EQ_T*) pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_reverb(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_reverb(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                      (SCC_AUD_SE_REVERB_MODE_T*) pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_base(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_base(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.pb_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_treble(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_treble(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.pui1_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_balance(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_balance(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.pui1_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_postdr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_postdr(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.pb_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_volume(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_volume(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_VOLUME_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_surround(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_surround(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.pb_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_superwoofer(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_superwoofer(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
                                      pt_args[1].u.pb_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_get_downmix_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_downmix_mode(
                       (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_DOWNMIX_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_get_dolby_drc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_dolby_drc(
                     (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_DOLBY_DRC_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_get_output_port(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_output_port(
                       (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_OUT_PORT_T)pt_args[1].u.i4_arg,
                                            pt_args[2].u.pb_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_cmpss_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_cmpss_mode(
                       (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_CMPSS_MDOE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_agc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_agc(
                       (HANDLE_T)pt_args[0].u.ui4_arg,
                                       pt_args[1].u.pb_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_chnl_delay(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_chnl_delay(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_CHNL_DELAY_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_max_chnl_delay(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_max_chnl_delay(
                       (HANDLE_T)pt_args[0].u.ui4_arg,
                                       pt_args[1].u.pui2_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_mts(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_mts(
                       (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_MTS_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_eq_user_lvl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_eq_user_lvl(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_SE_QA_USER_LVL_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_se_eq_user_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_se_eq_user_info(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AU_SE_QA_USER_BAND_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
static INT32 _x_scc_aud_clip_nfy_fct_wrapper (
                                   HANDLE_T            h_aud,
                                   SCC_AUD_CLIP_COND_T e_nfy_cond,
                                   VOID*               pv_nfy_tag,
                                   UINT32              ui4_data_1,
                                   UINT32              ui4_data_2 )
{
    RPC_DECL_VOID(5);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,    e_nfy_cond);    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data_1);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data_2);    

    RPC_DO_CB(pt_nfy_tag->t_id, "x_scc_aud_clip_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    rpc_del(RPC_DEFAULT_ID);
    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_clip(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    x_scc_aud_clip_nfy_fct      pf_nfy;
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    } 
    if(pt_args[2].u.pv_func == NULL)
     {
         pf_nfy      = NULL;
         pt_nfy_tag  = NULL;
     }
     else
     {
         pf_nfy = (x_scc_aud_clip_nfy_fct)_x_scc_aud_clip_nfy_fct_wrapper;
         pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[2].u.pv_func, 1, pt_args[3].u.pv_arg);
     }
     pt_return->e_type   = ARG_TYPE_INT32;
     pt_return->u.i4_arg = x_scc_aud_set_clip(
                                                  pt_args[0].u.ui4_arg, 
                    (SCC_AUD_CLIP_T *)pt_args[1].u.ui4_arg,
                                                 pf_nfy,
                             (VOID*      )pt_nfy_tag);
     if(pt_return->u.i4_arg == SMR_OK)
     {
         ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag);
     }
     else if( pt_nfy_tag != NULL)
     {
         ri_free_cb_tag(pt_nfy_tag);
     }
    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_clip_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_clip_ctrl(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
              (SCC_AUD_CTRL_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_clip_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_clip_ctrl(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
              (SCC_AUD_CTRL_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_clip_gain(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_clip_gain(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.ui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_clip_duration(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_clip_duration(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.pui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_clip_position(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_clip_position(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.pui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_clip_position(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_clip_position(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                                   pt_args[1].u.ui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_clip_volume(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_clip_volume(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_VOLUME_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_clip_custom_setting(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_clip_custom_setting(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T)pt_args[1].u.i4_arg,
                   (SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T)pt_args[2].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_clip_custom_setting(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_clip_custom_setting(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T)pt_args[1].u.i4_arg,
                   (SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T*)pt_args[2].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_pl2_switch(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_pl2_switch(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_PL2_SWITCH_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_pl2_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_pl2_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_PL2_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_pl2_panorama(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_pl2_panorama(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_PL2_PANORAMA_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_pl2_dimension(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_pl2_dimension(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_PL2_DIMENSION_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_pl2_c_width(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_pl2_c_width(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_PL2_C_WIDTH_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_pl2_switch(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_pl2_switch(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_PL2_SWITCH_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_pl2_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_pl2_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_PL2_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_pl2_panorama(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_pl2_panorama(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_PL2_PANORAMA_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_pl2_dimension(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_pl2_dimension(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_PL2_DIMENSION_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_pl2_c_width(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_pl2_c_width(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_PL2_C_WIDTH_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_dolby_certi_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_dolby_certi_mode(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.pb_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_dolby_certi_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_dolby_certi_mode(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.b_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_output_port_adj(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_output_port_adj(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
         (SCC_AUD_OUT_PORT_T)pt_args[1].u.i4_arg,
                             pt_args[2].u.b_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_output_port_adj(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_output_port_adj(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
         (SCC_AUD_OUT_PORT_T)pt_args[1].u.i4_arg,
                             pt_args[2].u.pb_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_capability(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_capability(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_CAPABILITY_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_bbe_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_bbe_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_BBE_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_endian_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_endian_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_ENDIAN_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_out_device(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_out_device(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_OUT_DEVICE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_get_bbe_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_bbe_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_BBE_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_hdphone_detect_nfy(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_hdphone_detect_nfy(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_HDPHONE_DETECT_NFT_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_hdphone_cond(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_hdphone_cond(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_HDPHONE_COND_T *)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_mute_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_mute_ex(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                                        pt_args[1].u.pui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_lineout_vol_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_lineout_vol_mode(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                  (SCC_AUD_DEC_LINEOUT_VOL_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_sph_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_sph_mode(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_DEC_SE_SPH_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_se_volume_offset(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_volume_offset(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                                        pt_args[1].u.ui1_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_se_hp_volume_offset(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_se_hp_volume_offset(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                                        pt_args[1].u.ui1_arg);

    return RPCR_OK;
}




INT32 _hndlr_x_scc_aud_set_peq_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_peq_info(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                  (SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_ovm(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_ovm(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                                        pt_args[1].u.ui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_btnsnd_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_btnsnd_mode(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                        (SCC_AUD_BTNSND_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_btnsnd_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_btnsnd_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_BTNSND_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_btnsnd_play(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_btnsnd_play(
                      (HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_btnsnd_enable(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_btnsnd_enable(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                        (SCC_AUD_BTNSND_ENABLE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_btnsnd_enable(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_btnsnd_enable(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_BTNSND_ENABLE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_cmpss_drc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_cmpss_drc(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
                   (SCC_AUD_CMPSS_DRC_MODE_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_cmpss_drc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_cmpss_drc(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                        (SCC_AUD_CMPSS_DRC_MODE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_mpeg_level_adj(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_mpeg_level_adj(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                                        pt_args[1].u.i1_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_mpeg_level_adj(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_mpeg_level_adj(
                       (HANDLE_T)pt_args[0].u.ui4_arg,
                                       pt_args[1].u.pi1_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_set_av_sync_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_av_sync_mode(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
              (SCC_AUD_DEC_AV_SYNC_INFO_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_av_sync_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_av_sync_mode(
                       (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_DEC_AV_SYNC_INFO_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_scc_aud_get_power_on_off_music_volume(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_power_on_off_music_volume(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_VOLUME_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_upload_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_upload_ctrl(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
                                        pt_args[1].u.b_arg );

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_get_upload_data_buffer_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_get_upload_data_buffer_info(
                        (HANDLE_T)pt_args[0].u.ui4_arg,
        (SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_scc_aud_set_karaoke(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_scc_aud_set_karaoke(
                      (HANDLE_T)pt_args[0].u.ui4_arg,
      (SCC_AUD_KARAOKE_INFO_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

#ifndef NO_WGL_INIT
#endif
static INT32 _x_sess_nfy_fct_wrapper(
    HANDLE_T          h_sess,
    SM_SESS_COND_T    e_sess_cond,
    VOID*             pv_sess_tag,
    SM_EVN_TYPE_T     e_event,
    UINT32            ui4_data )
{
    RPC_DECL(5, INT32);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_sess_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_sess);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_sess_cond);        
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_event);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data);    

    RPC_DO_CB(pt_nfy_tag->t_id, "x_sess_nfy_fct", pt_nfy_tag->pv_cb_addr);

    /* Fix me here how to process error? */
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);            
}

INT32 _hndlr_x_sm_open_sess(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    x_sess_nfy_fct      pf_nfy;
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    
    if(ui4_num_args != 9)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    if(pt_args[6].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy = _x_sess_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[6].u.pv_func, 1, pt_args[7].u.pv_arg);
    }
    pt_return->u.i4_arg = x_sm_open_sess(
                            (HANDLE_T   )pt_args[0].u.ui4_arg,
                            (const CHAR*)pt_args[1].u.ps_str,
                            (const CHAR*)pt_args[2].u.ps_str,
                            (VOID*      )pt_args[3].u.pv_desc,
                            (SIZE_T     )pt_args[4].u.z_arg,
                            (INT32      )pt_args[5].u.i4_arg,
                                         pf_nfy,
                            (VOID*      )pt_nfy_tag,
                            (HANDLE_T*  )pt_args[8].u.pui4_arg);
    if(pt_return->u.i4_arg == SMR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[8].u.pui4_arg, pt_nfy_tag);
    }
    else if( pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }
    return RPCR_OK;
}

INT32 _hndlr_x_sm_close_sess(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_sm_close_sess((HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sm_play_sess(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{

    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_sm_play_sess((HANDLE_T)pt_args[0].u.ui4_arg, pt_args[1].u.i4_arg);

    return RPCR_OK;
}


INT32 _hndlr_x_sm_stop_sess(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_sm_stop_sess((HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sm_open_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_stream_nfy_fct    pf_nfy;
    
    if(ui4_num_args != 7)
    {
        return RPCR_INV_ARGS;
    }      
    
    pt_return->e_type   = ARG_TYPE_INT32;
    
    if(pt_args[3].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = ri_x_stream_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[3].u.pv_func, 1, pt_args[4].u.pv_arg);
    }
        
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_sm_open_stream(
                        (HANDLE_T                )pt_args[0].u.ui4_arg,
                        (const STREAM_COMP_ID_T* )pt_args[1].u.pv_desc,
                        (const SM_COMMAND_T*     )pt_args[2].u.pv_desc,
                        pf_nfy,
                        (VOID*                   )pt_nfy_tag,
                        (BOOL                    )pt_args[5].u.b_arg,
                        (HANDLE_T*               )pt_args[6].u.pui4_arg);
    if(pt_return->u.i4_arg == SMR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[6].u.pui4_arg, pt_nfy_tag);
    }
    else if( pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }
    return RPCR_OK;
}

INT32 _hndlr_x_sm_close_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_sm_close_stream((HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sm_play_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_sm_play_stream((HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sm_stop_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_sm_stop_stream((HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _x_mm_range_nfy_fct_wrapper(
    VOID*            pv_nfy_tag,
    MM_RANGE_COND_T  e_nfy_cond,
    UINT32           ui4_data_1,
    UINT32           ui4_data_2)
{
    RPC_DECL(4, BOOL);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_INT32,    e_nfy_cond);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data_1);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data_2);    

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_range_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN(ARG_TYPE_BOOL, FALSE);
}

INT32 _hndlr_x_sm_set(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    SM_SET_TYPE_T   e_type;
    HANDLE_T        h_sm;
    
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }      

    h_sm = (HANDLE_T)pt_args[0].u.ui4_arg;
    e_type = (SM_SET_TYPE_T)pt_args[1].u.ui4_arg;
    
    pt_return->e_type   = ARG_TYPE_INT32;
    switch(e_type)
    {
        case SM_SESS_GNRC_SET_TYPE_ADD_RANGE:
        {
            MM_RANGE_INFO_T*  pt_range_info = (MM_RANGE_INFO_T*) pt_args[2].u.pv_desc;
            if(pt_range_info != NULL)
            {                    
                if(pt_range_info->pt_range_nfy != NULL)
                {
                    pt_range_info->pv_tag       = ri_create_cb_tag(t_rpc_id, (VOID*)&pt_range_info->pt_range_nfy, 1, pt_range_info->pv_tag);
                    pt_range_info->pt_range_nfy = (x_mm_range_nfy_fct)_x_mm_range_nfy_fct_wrapper;
                    pt_return->u.i4_arg = x_sm_set(h_sm,                           
                                                   e_type,
                                                   pt_range_info,
                                                   pt_args[3].u.z_arg);
                    if(pt_return->u.i4_arg == SMR_OK)
                    {
                        ri_bind_cb_tag(h_sm, pt_range_info->pv_tag);
                    }
                    else if( pt_range_info->pv_tag != NULL)
                    {
                        ri_free_cb_tag(pt_range_info->pv_tag);
                    }
                }
                else
                {
                    pt_return->u.i4_arg = x_sm_set(h_sm,                           
                                                   e_type,
                                                   pt_range_info,
                                                   pt_args[3].u.z_arg);
                }
            }
        }        
        break;
        default:
            pt_return->u.i4_arg = x_sm_set(h_sm,                           
                                           e_type,  
                                           pt_args[2].u.pv_desc,                        
                                           pt_args[3].u.z_arg);
                    
    }
    
    

    return RPCR_OK;
}

INT32 _hndlr_x_sm_get(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }      
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_sm_get(
        (HANDLE_T     )pt_args[0].u.ui4_arg,                           
        (SM_GET_TYPE_T)pt_args[1].u.ui4_arg,                         
        (VOID*        )pt_args[2].u.pv_desc,                        
        (SIZE_T*      )pt_args[3].u.pz_arg);

    return RPCR_OK;
}

INT32 c_rpc_reg_sm_op_hndlrs()
{
    RPC_REG_OP_HNDLR(x_scc_comp_close);
    RPC_REG_OP_HNDLR(x_scc_comp_open);
    RPC_REG_OP_HNDLR(x_scc_comp_grp_set);
    RPC_REG_OP_HNDLR(x_scc_comp_att_incr);
    RPC_REG_OP_HNDLR(x_scc_comp_att_decr);
    RPC_REG_OP_HNDLR(x_scc_comp_att_get_num_steps);
    
    RPC_REG_OP_HNDLR(x_scc_vid_set_vdp_atomic_cmd);
    

     /*for APIs: INT32 func(HANDLE_T  h_vid, enum type   e_pic_mode )*/    
     RPC_REG_OP_HNDLR(x_scc_vid_set_pic_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_set_hdmi_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_set_color_sys);
     RPC_REG_OP_HNDLR(x_scc_vid_set_scr_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_set_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_set_mpeg_nr);
     RPC_REG_OP_HNDLR(x_scc_vid_set_blue_stretch);
     RPC_REG_OP_HNDLR(x_scc_vid_set_game_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_set_wcg);
     RPC_REG_OP_HNDLR(x_scc_vid_set_xvycc);
     RPC_REG_OP_HNDLR(x_scc_vid_set_di_edge_preserve);
     RPC_REG_OP_HNDLR(x_scc_vid_set_di_ma);
     RPC_REG_OP_HNDLR(x_scc_vid_set_di_film_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_set_iframe_ctrl);
     RPC_REG_OP_HNDLR(x_scc_vid_vdrips_play_ctrl);
     RPC_REG_OP_HNDLR(x_scc_vid_set_iframe_codec);
     RPC_REG_OP_HNDLR(x_scc_vid_set_split_scrn_demo);

     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_nav);
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_parallax);
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_fld_depth);
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_lr_switch);
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_to_2d);
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_fpr);
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_protrude_scr);
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_distance_2_tv);
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_osd_depth); 
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_nav_auto_chg_en); 
     RPC_REG_OP_HNDLR(x_scc_vid_set_3d_image_safety_en); 
     RPC_REG_OP_HNDLR(x_scc_vid_set_pedestal);
     RPC_REG_OP_HNDLR(x_scc_vid_set_hdmi_range);
     RPC_REG_OP_HNDLR(x_scc_vid_set_vga_detect);
     RPC_REG_OP_HNDLR(x_scc_vid_set_static_gamma);
     RPC_REG_OP_HNDLR(x_scc_vid_set_angular_sce);
     RPC_REG_OP_HNDLR(x_scc_vid_set_down_bw_mode); 
     RPC_REG_OP_HNDLR(x_scc_vid_set_src_detect_nfy);      
     RPC_REG_OP_HNDLR(x_scc_vid_set_super_resolution);      

     /*for APIs: INT32 func(HANDLE_T  h_vid)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_set_reset_vga_timing_option);
    
     /*for APIs: INT32 func(HANDLE_T  h_vid, UINT16 ui2_value)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_set_h_position);
     RPC_REG_OP_HNDLR(x_scc_vid_set_v_position);
     RPC_REG_OP_HNDLR(x_scc_vid_get_mode_capability);
     RPC_REG_OP_HNDLR(x_scc_vid_get_h_position);
     RPC_REG_OP_HNDLR(x_scc_vid_get_v_position);    
     RPC_REG_OP_HNDLR(x_scc_vid_get_h_size);
     RPC_REG_OP_HNDLR(x_scc_vid_get_v_size);
     
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_nav);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_parallax);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_fld_depth);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_lr_switch);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_to_2d);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_fpr);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_protrude_scr);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_distance_2_tv);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_osd_depth);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_fmt_cap); 
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_ctrl_cap); 
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_nav_auto_chg_en);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_image_safety_en);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_nav_tag);
     RPC_REG_OP_HNDLR(x_scc_vid_get_overscan_enable);
     RPC_REG_OP_HNDLR(x_scc_vid_get_nonlinear_scaling_enable);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_panel_type);
     RPC_REG_OP_HNDLR(x_scc_vid_get_vga_status);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3d_mjc_enable);
     RPC_REG_OP_HNDLR(x_scc_vid_get_freq);
     RPC_REG_OP_HNDLR(x_scc_vid_get_src_detect_status);
     RPC_REG_OP_HNDLR(x_scc_vid_get_asp_ratio_enable);  
     RPC_REG_OP_HNDLR(x_scc_vid_get_screen_mode_enable);  
     
     RPC_REG_OP_HNDLR(x_scc_vid_set_enhance);     
     RPC_REG_OP_HNDLR(x_scc_vid_set_blur);
     RPC_REG_OP_HNDLR(x_scc_vid_set_cti);
     RPC_REG_OP_HNDLR(x_scc_vid_set_eti);
     RPC_REG_OP_HNDLR(x_scc_vid_get_vdrips_duration);
     RPC_REG_OP_HNDLR(x_scc_vid_set_sharpness);
     RPC_REG_OP_HNDLR(x_scc_vid_set_color_suppress);
     RPC_REG_OP_HNDLR(x_scc_vid_set_nr);
     RPC_REG_OP_HNDLR(x_scc_vid_set_3dnr);
     RPC_REG_OP_HNDLR(x_scc_vid_set_black_lvl_ext);
     RPC_REG_OP_HNDLR(x_scc_vid_set_white_peak_rdct);
     RPC_REG_OP_HNDLR(x_scc_vid_set_flesh_tone);
     RPC_REG_OP_HNDLR(x_scc_vid_set_luma);
     RPC_REG_OP_HNDLR(x_scc_vid_set_dynamic_scaling);
     RPC_REG_OP_HNDLR(x_scc_vid_set_h_size);
     RPC_REG_OP_HNDLR(x_scc_vid_set_v_size);
     
     /*for APIs: INT32 func(HANDLE_T  h_vid, BOOL b_value)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_set_letter_box_detect);
     RPC_REG_OP_HNDLR(x_scc_vid_set_force_unmute_on_no_sig);
     RPC_REG_OP_HNDLR(x_scc_vid_set_film_mode);

     RPC_REG_OP_HNDLR(x_scc_vid_set_srm_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_set_sig_booster);
     RPC_REG_OP_HNDLR(x_scc_vid_set_black_stretch);
     RPC_REG_OP_HNDLR(x_scc_vid_get_black_stretch);
     RPC_REG_OP_HNDLR(x_scc_vid_set_white_stretch);
     RPC_REG_OP_HNDLR(x_scc_vid_get_white_stretch);
     RPC_REG_OP_HNDLR(x_scc_vid_set_black_stretch_val);
     RPC_REG_OP_HNDLR(x_scc_vid_get_black_stretch_val);
     RPC_REG_OP_HNDLR(x_scc_vid_set_white_stretch_val);
     RPC_REG_OP_HNDLR(x_scc_vid_get_white_stretch_val);
     RPC_REG_OP_HNDLR(x_scc_vid_set_colorspace);
     RPC_REG_OP_HNDLR(x_scc_vid_set_flip);
     RPC_REG_OP_HNDLR(x_scc_vid_set_mirror);
     RPC_REG_OP_HNDLR(x_scc_vid_get_flip);
     RPC_REG_OP_HNDLR(x_scc_vid_get_mirror);
     RPC_REG_OP_HNDLR(x_scc_vid_get_local_dimming_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_set_local_dimming_mode);
     
     /*for APIs: INT32 func(HANDLE_T  h_vid, BOOL* pb_value)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_get_film_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_get_hdcp_key_exist);
     RPC_REG_OP_HNDLR(x_scc_vid_get_letter_box_detect);
     RPC_REG_OP_HNDLR(x_scc_vid_set_ambig_vga_timing_ctrl);
     RPC_REG_OP_HNDLR(x_scc_vid_get_force_unmute_on_no_sig);
     RPC_REG_OP_HNDLR(x_scc_vid_get_super_freeze);
     
     /*for APIs: INT32 func(HANDLE_T  h_vid, UINT16* pui2_value)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_get_blur);
     RPC_REG_OP_HNDLR(x_scc_vid_get_cti);
     RPC_REG_OP_HNDLR(x_scc_vid_get_eti);
     RPC_REG_OP_HNDLR(x_scc_vid_get_sharpness);
     RPC_REG_OP_HNDLR(x_scc_vid_get_color_supress);
     RPC_REG_OP_HNDLR(x_scc_vid_get_nr);
     RPC_REG_OP_HNDLR(x_scc_vid_get_3dnr);
     RPC_REG_OP_HNDLR(x_scc_vid_get_black_lvl_ext);
     RPC_REG_OP_HNDLR(x_scc_vid_get_white_peak_rdct);
     RPC_REG_OP_HNDLR(x_scc_vid_get_flesh_tone);
     RPC_REG_OP_HNDLR(x_scc_vid_get_luma);
     RPC_REG_OP_HNDLR(x_scc_vid_get_dynamic_scaling);   
     RPC_REG_OP_HNDLR(x_scc_vid_get_enhance);   
    
     /*for APIs: INT32 func(HANDLE_T  h_vid, UINT32* pui4_value1,UINT32* pui4_value2)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_get_iframe_resolution);
     
     /*for APIs: INT32 set_func(HANDLE_T  h_vid, const struct* pt_value)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_set_bg);
     RPC_REG_OP_HNDLR(x_scc_vid_set_super_blank);
     RPC_REG_OP_HNDLR(x_scc_vid_set_overscan_clipper);
     RPC_REG_OP_HNDLR(x_scc_vid_set_lb_detect_config);
     RPC_REG_OP_HNDLR(x_scc_vid_set_deblocking);
     RPC_REG_OP_HNDLR(x_scc_vid_set_super_freeze);
     RPC_REG_OP_HNDLR(x_scc_vid_set_mjc);
     RPC_REG_OP_HNDLR(x_scc_vid_set_mode_ex);
    
     /*for APIs: INT32 set_func(HANDLE_T  h_vid, struct* pt_value)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_set_iframe_disp_region);
     RPC_REG_OP_HNDLR(x_scc_vid_set_iframe_src_region);
     RPC_REG_OP_HNDLR(x_scc_vid_set_iframe_pla_order);
    
     RPC_REG_OP_HNDLR(x_scc_vid_iframe_alloc_buffer);
     RPC_REG_OP_HNDLR(x_scc_vid_set_iframe_buffer);
     RPC_REG_OP_HNDLR(x_scc_vid_iframe_decode);
     RPC_REG_OP_HNDLR(x_scc_vid_iframe_free_buffer);
#ifdef SYS_MHP_SUPPORT
     RPC_REG_OP_HNDLR(x_scc_vid_set_iframe_not_display);     
#endif
     RPC_REG_OP_HNDLR(x_scc_vid_vdrips_feed);
     
     /*for APIs: INT32 get_func(HANDLE_T  h_vid, struct* pt_value)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_get_overscan_clipper);
     RPC_REG_OP_HNDLR(x_scc_vid_get_mjc);
     RPC_REG_OP_HNDLR(x_scc_vid_get_lb_detect_config);
     RPC_REG_OP_HNDLR(x_scc_vid_get_deblocking);
     RPC_REG_OP_HNDLR(x_scc_vid_get_iframe_disp_region);
     RPC_REG_OP_HNDLR(x_scc_vid_get_iframe_src_region);
    
     /*for APIs: INT32 get_func(HANDLE_T  h_vid, enum* pt_value)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_get_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_get_bg);
     RPC_REG_OP_HNDLR(x_scc_vid_get_pic_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_get_hdmi_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_get_color_sys);
     RPC_REG_OP_HNDLR(x_scc_vid_get_scr_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_get_di_film_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_get_di_ma);
     RPC_REG_OP_HNDLR(x_scc_vid_get_di_edge_preserve);
     RPC_REG_OP_HNDLR(x_scc_vid_get_xvycc);
     RPC_REG_OP_HNDLR(x_scc_vid_get_wcg);
     RPC_REG_OP_HNDLR(x_scc_vid_get_game_mode);
     RPC_REG_OP_HNDLR(x_scc_vid_get_blue_stretch);
     RPC_REG_OP_HNDLR(x_scc_vid_get_mpeg_nr);
     RPC_REG_OP_HNDLR(x_scc_vid_get_split_scrn_demo);
     
     /*for APIs:INT32 set_func_ptr( HANDLE_T    h_vid,function_pointer  pf_nfy,  VOID*  pv_nfy_tag )*/
     RPC_REG_OP_HNDLR(x_scc_vid_set_iframe_nfy);
     RPC_REG_OP_HNDLR(x_scc_vid_set_vdrips_nfy);
    
    /*for APIs: INT32 get_func(HANDLE_T  h_vid, HANDLE_T  h_value)*/    
     RPC_REG_OP_HNDLR(x_scc_vid_get_iframe_surface);


    RPC_REG_OP_HNDLR(x_scc_disp_set_attrib);
    RPC_REG_OP_HNDLR(x_scc_disp_set_adp_back_light_rng);
    RPC_REG_OP_HNDLR(x_scc_disp_set_resolution);
    RPC_REG_OP_HNDLR(x_scc_disp_set_aspect_ratio);
    RPC_REG_OP_HNDLR(x_scc_disp_set_fmt);
    RPC_REG_OP_HNDLR(x_scc_disp_set_tv_type);
    RPC_REG_OP_HNDLR(x_scc_disp_set_bg);
    RPC_REG_OP_HNDLR(x_scc_disp_set_digit_output_fmt);
    RPC_REG_OP_HNDLR(x_scc_disp_set_color_gain);
    RPC_REG_OP_HNDLR(x_scc_disp_set_color_offset);
    RPC_REG_OP_HNDLR(x_scc_disp_set_tv_mode);
    RPC_REG_OP_HNDLR(x_scc_disp_set_mjc);
    RPC_REG_OP_HNDLR(x_scc_disp_set_test_pattern);
    RPC_REG_OP_HNDLR(x_scc_disp_set_back_light_ctrl);
    RPC_REG_OP_HNDLR(x_scc_disp_set_frame_rate);
    RPC_REG_OP_HNDLR(x_scc_disp_set_force_frame_rate);
    RPC_REG_OP_HNDLR(x_scc_disp_get_back_light_ctrl);
    RPC_REG_OP_HNDLR(x_scc_disp_get_test_pattern);
    RPC_REG_OP_HNDLR(x_scc_disp_get_mjc);
    RPC_REG_OP_HNDLR(x_scc_disp_get_tv_mode);
    RPC_REG_OP_HNDLR(x_scc_disp_get_color_offset);
    RPC_REG_OP_HNDLR(x_scc_disp_get_color_gain);
    RPC_REG_OP_HNDLR(x_scc_disp_get_digit_output_fmt);
    RPC_REG_OP_HNDLR(x_scc_disp_get_dither_cap);
    RPC_REG_OP_HNDLR(x_scc_disp_get_fmt_cap);
    RPC_REG_OP_HNDLR(x_scc_disp_get_bg);
    RPC_REG_OP_HNDLR(x_scc_disp_get_tv_type);
    RPC_REG_OP_HNDLR(x_scc_disp_get_fmt);
    RPC_REG_OP_HNDLR(x_scc_disp_get_aspect_ratio);
    RPC_REG_OP_HNDLR(x_scc_disp_get_resolution);
    RPC_REG_OP_HNDLR(x_scc_disp_get_adp_back_light_rng);
    RPC_REG_OP_HNDLR(x_scc_disp_get_attrib);
    RPC_REG_OP_HNDLR(x_scc_disp_get_max_gamma);
    RPC_REG_OP_HNDLR(x_scc_query_bootup_status);
    RPC_REG_OP_HNDLR(x_scc_disp_get_screen_resolution);
    RPC_REG_OP_HNDLR(x_scc_disp_get_frame_rate);

    RPC_REG_OP_HNDLR(x_scc_aud_set_volume);
    RPC_REG_OP_HNDLR(x_scc_aud_set_volume_ex);
    RPC_REG_OP_HNDLR(x_scc_aud_mute);
    RPC_REG_OP_HNDLR(x_scc_aud_unmute);
    RPC_REG_OP_HNDLR(x_scc_aud_set_spdif_fmt);
    RPC_REG_OP_HNDLR(x_scc_aud_set_hdcd_filter);
    RPC_REG_OP_HNDLR(x_scc_aud_set_loudspeaker);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_trim);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_superbase);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_equalizer);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_reverb);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_base);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_treble);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_balance);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_postdr);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_volume);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_surround);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_superwoofer);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_sound_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_surround_ex);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_sound_mode_start);    
    RPC_REG_OP_HNDLR(x_scc_aud_set_downmix_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_set_dolby_drc);
    RPC_REG_OP_HNDLR(x_scc_aud_set_output_port);
    RPC_REG_OP_HNDLR(x_scc_aud_set_output_port_ex);
    RPC_REG_OP_HNDLR(x_scc_aud_set_cmpss_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_set_agc);
    RPC_REG_OP_HNDLR(x_scc_aud_set_mts);    
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_eq_user_lvl);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_eq_user_lvl_ex);
    RPC_REG_OP_HNDLR(x_scc_aud_set_chnl_delay);
    RPC_REG_OP_HNDLR(x_scc_aud_set_hdphone_detect_nfy);
    RPC_REG_OP_HNDLR(x_scc_aud_set_mute_ex);
    RPC_REG_OP_HNDLR(x_scc_aud_get_volume);
    RPC_REG_OP_HNDLR(x_scc_aud_is_muted);
    RPC_REG_OP_HNDLR(x_scc_aud_get_spdif_fmt);
    RPC_REG_OP_HNDLR(x_scc_aud_get_hdcd_filter);
    RPC_REG_OP_HNDLR(x_scc_aud_get_loudspeaker);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_trim);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_superbase);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_equalizer);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_reverb);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_base);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_treble);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_balance);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_postdr);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_volume);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_surround);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_superwoofer);
    RPC_REG_OP_HNDLR(x_scc_aud_get_downmix_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_get_dolby_drc);
    RPC_REG_OP_HNDLR(x_scc_aud_get_output_port);
    RPC_REG_OP_HNDLR(x_scc_aud_get_cmpss_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_get_agc);
    RPC_REG_OP_HNDLR(x_scc_aud_get_chnl_delay);
    RPC_REG_OP_HNDLR(x_scc_aud_get_max_chnl_delay);
    RPC_REG_OP_HNDLR(x_scc_aud_get_mts);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_eq_user_lvl);
    RPC_REG_OP_HNDLR(x_scc_aud_get_se_eq_user_info);
    RPC_REG_OP_HNDLR(x_scc_aud_set_clip);
    RPC_REG_OP_HNDLR(x_scc_aud_set_clip_ctrl);
    RPC_REG_OP_HNDLR(x_scc_aud_get_clip_ctrl);
    RPC_REG_OP_HNDLR(x_scc_aud_set_clip_gain);
    RPC_REG_OP_HNDLR(x_scc_aud_get_clip_duration);
    RPC_REG_OP_HNDLR(x_scc_aud_get_clip_position);
    RPC_REG_OP_HNDLR(x_scc_aud_set_clip_position);
    RPC_REG_OP_HNDLR(x_scc_aud_set_clip_volume);
    RPC_REG_OP_HNDLR(x_scc_aud_set_pl2_switch);
    RPC_REG_OP_HNDLR(x_scc_aud_set_pl2_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_set_pl2_panorama);
    RPC_REG_OP_HNDLR(x_scc_aud_set_pl2_dimension);
    RPC_REG_OP_HNDLR(x_scc_aud_set_pl2_c_width);
    RPC_REG_OP_HNDLR(x_scc_aud_get_pl2_switch);
    RPC_REG_OP_HNDLR(x_scc_aud_get_pl2_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_get_pl2_panorama);
    RPC_REG_OP_HNDLR(x_scc_aud_get_pl2_dimension);
    RPC_REG_OP_HNDLR(x_scc_aud_get_pl2_c_width);
    RPC_REG_OP_HNDLR(x_scc_aud_get_dolby_certi_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_set_dolby_certi_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_set_output_port_adj);
    RPC_REG_OP_HNDLR(x_scc_aud_get_output_port_adj);
    RPC_REG_OP_HNDLR(x_scc_aud_get_capability);
    RPC_REG_OP_HNDLR(x_scc_aud_set_endian_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_set_out_device);
    RPC_REG_OP_HNDLR(x_scc_aud_set_bbe_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_get_bbe_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_get_hdphone_detect_nfy);
    RPC_REG_OP_HNDLR(x_scc_aud_get_hdphone_cond);
    RPC_REG_OP_HNDLR(x_scc_aud_get_mute_ex);
    RPC_REG_OP_HNDLR(x_scc_aud_set_lineout_vol_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_sph_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_volume_offset);
    RPC_REG_OP_HNDLR(x_scc_aud_set_peq_info);
    RPC_REG_OP_HNDLR(x_scc_aud_set_ovm);
    RPC_REG_OP_HNDLR(x_scc_aud_set_btnsnd_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_get_btnsnd_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_set_btnsnd_play);
    RPC_REG_OP_HNDLR(x_scc_aud_set_btnsnd_enable);
    RPC_REG_OP_HNDLR(x_scc_aud_get_btnsnd_enable);
    RPC_REG_OP_HNDLR(x_scc_aud_set_clip_custom_setting);
    RPC_REG_OP_HNDLR(x_scc_aud_get_clip_custom_setting);
    RPC_REG_OP_HNDLR(x_scc_aud_get_cmpss_drc);
    RPC_REG_OP_HNDLR(x_scc_aud_set_cmpss_drc);
    RPC_REG_OP_HNDLR(x_scc_aud_set_mpeg_level_adj);
    RPC_REG_OP_HNDLR(x_scc_aud_get_mpeg_level_adj);
    RPC_REG_OP_HNDLR(x_scc_aud_set_av_sync_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_get_av_sync_mode);
    RPC_REG_OP_HNDLR(x_scc_aud_get_power_on_off_music_volume);
    RPC_REG_OP_HNDLR(x_scc_aud_set_upload_ctrl);
    RPC_REG_OP_HNDLR(x_scc_aud_set_upload_data_nfy);
    RPC_REG_OP_HNDLR(x_scc_aud_get_upload_data_buffer_info);
    RPC_REG_OP_HNDLR(x_scc_aud_get_upload_pcm_info);
    RPC_REG_OP_HNDLR(x_scc_aud_set_se_hp_volume_offset);
    RPC_REG_OP_HNDLR(x_scc_aud_set_karaoke);
  
    RPC_REG_OP_HNDLR(x_scc_aud_set_panfade);
    RPC_REG_OP_HNDLR(x_scc_aud_set_srs_bypass);
    
#ifndef NO_WGL_INIT
#endif
    RPC_REG_OP_HNDLR(x_sm_open_sess);
    RPC_REG_OP_HNDLR(x_sm_close_sess);
    RPC_REG_OP_HNDLR(x_sm_play_sess);
    RPC_REG_OP_HNDLR(x_sm_stop_sess);
    RPC_REG_OP_HNDLR(x_sm_open_stream);
    RPC_REG_OP_HNDLR(x_sm_close_stream);
    RPC_REG_OP_HNDLR(x_sm_play_stream);
    RPC_REG_OP_HNDLR(x_sm_stop_stream);
    RPC_REG_OP_HNDLR(x_sm_set);
    RPC_REG_OP_HNDLR(x_sm_get);

    return RPCR_OK;
}

