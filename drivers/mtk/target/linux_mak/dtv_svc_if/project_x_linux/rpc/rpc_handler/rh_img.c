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
 * $RCSfile:  $
 * $Revision: #4 $
 * $Date: 2012/08/28 $
 * $Author: xiuqin.wang $
 * $SWAuthor: Jun Zuo  $
 *
 * Description:
 *         This file contains all ATSC-SI engine c_* version API implementations.
 *         function implementations.
 *-----------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
                    include files
 ------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "img/x_img.h"
#include "img/c_img.h"
#include "img/u_img.h"
#include "img/tfx/u_img_tfx.h"
#include "img/tfx/c_img_tfx.h"
#include "img/tfx/x_img_tfx.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

typedef struct _IMG_PULL_MEMMAP_T
{
    UINT32    	ui4_fm_handle;
    UINT32      ui4_phy_addr;
    UINT32      ui4_size;
    UINT32      ui4_virt_addr;
}  IMG_PULL_MEMMAP_T ;
INT32 _hndlr_x_img_fopen(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_fopen(
                           pt_args[0].u.pc_arg,
                           pt_args[1].u.ui1_arg,
            (x_img_nfy_fct)pt_args[2].u.pv_func,
                           pt_args[3].u.pv_arg,
                (HANDLE_T*)pt_args[4].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_mopen(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 6)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_mopen(
                           pt_args[0].u.pv_arg,
                           pt_args[1].u.ui4_arg,
                           pt_args[2].u.pc_arg,
            (x_img_nfy_fct)pt_args[3].u.pv_func,
                           pt_args[4].u.pv_arg,
                (HANDLE_T*)pt_args[5].u.pui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_img_close(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_close((HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_get_dimension(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_get_dimension(
				        (HANDLE_T)pt_args[0].u.ui4_arg,
				(IMG_INTRA_TYPE_T)pt_args[1].u.i4_arg,
				         (SIZE_T*)pt_args[2].u.pui4_arg,
						 (SIZE_T*)pt_args[3].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_stop(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_stop(
				        (HANDLE_T)pt_args[0].u.ui4_arg,
			  (IMG_STOP_OP_TYPE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_sync_load(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 13)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_sync_load(
				        (HANDLE_T)pt_args[0].u.ui4_arg,
			    (IMG_INTRA_TYPE_T)pt_args[1].u.i4_arg,
				(IMG_TYPE_DATA_T*)pt_args[2].u.ui4_arg,
		    (IMG_ROTATE_OP_TYPE_T)pt_args[3].u.i4_arg,
								  pt_args[4].u.i4_arg,
								  pt_args[5].u.i4_arg,
								  pt_args[6].u.i4_arg,
								  pt_args[7].u.i4_arg,
								  pt_args[8].u.i4_arg,
								  pt_args[9].u.i4_arg,
								 pt_args[10].u.i4_arg,
								 pt_args[11].u.i4_arg,
				  (GL_HSURFACE_T)pt_args[12].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_async_load(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 13)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_async_load(
				        (HANDLE_T)pt_args[0].u.ui4_arg,
			    (IMG_INTRA_TYPE_T)pt_args[1].u.i4_arg,
				(IMG_TYPE_DATA_T*)pt_args[2].u.ui4_arg,
		    (IMG_ROTATE_OP_TYPE_T)pt_args[3].u.i4_arg,
								  pt_args[4].u.i4_arg,
								  pt_args[5].u.i4_arg,
								  pt_args[6].u.i4_arg,
								  pt_args[7].u.i4_arg,
								  pt_args[8].u.i4_arg,
								  pt_args[9].u.i4_arg,
								 pt_args[10].u.i4_arg,
								 pt_args[11].u.i4_arg,
				  (GL_HSURFACE_T)pt_args[12].u.ui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_img_get_capability(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_get_capability(
				        (IMG_CAPS_T*)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_img_get_info_len(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_get_info_len(
				        (HANDLE_T)pt_args[0].u.ui4_arg,
				 (IMG_INFO_TYPE_T)pt_args[1].u.i4_arg,
				                  pt_args[2].u.pv_arg,
								  pt_args[3].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_get_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_get_info(
				        (HANDLE_T)pt_args[0].u.ui4_arg,
				 (IMG_INFO_TYPE_T)pt_args[1].u.i4_arg,
				                  pt_args[2].u.pv_arg,
								  pt_args[3].u.pv_arg,
								  pt_args[4].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_set_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_set_info(
				        (HANDLE_T)pt_args[0].u.ui4_arg,
				 (IMG_INFO_TYPE_T)pt_args[1].u.i4_arg,
				                  pt_args[2].u.pv_arg,
								  pt_args[3].u.pv_arg,
								  pt_args[4].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_get_exif_info_len(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_get_exif_info_len(
				        (HANDLE_T)pt_args[0].u.ui4_arg,
				(IMG_INTRA_TYPE_T)pt_args[1].u.i4_arg,
				   (EXIF_INFO_T *)pt_args[2].u.ui4_arg,
								  pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_get_exif_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_get_exif_info(
				        (HANDLE_T)pt_args[0].u.ui4_arg,
				(IMG_INTRA_TYPE_T)pt_args[1].u.i4_arg,
				   (EXIF_INFO_T *)pt_args[2].u.ui4_arg,
								  pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_set_exif_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_set_exif_info(
				        (HANDLE_T)pt_args[0].u.ui4_arg,
				(IMG_INTRA_TYPE_T)pt_args[1].u.i4_arg,
				   (EXIF_INFO_T *)pt_args[2].u.ui4_arg,
								  pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_open_exif(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_open_exif(
				  (IMG_SRC_TYPE_T)pt_args[0].u.i4_arg,
				                  pt_args[1].u.pv_arg,
				                  pt_args[2].u.ui4_arg,
					  (HANDLE_T *)pt_args[3].u.pui4_arg);

    return RPCR_OK;
}
INT32 _hndlr_x_img_reparse_exif(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_reparse_exif( (HANDLE_T *)pt_args[0].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_close_exif(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_close_exif( (HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_tfx_create(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_tfx_create( (HANDLE_T*)pt_args[0].u.ui4_arg,
                                  (IMG_TFX_NFY_FCT_T)pt_args[1].u.pv_func,
                                                      pt_args[2].u.pv_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_tfx_delete(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_tfx_delete( (HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_tfx_play(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 6)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_tfx_play( (HANDLE_T)pt_args[0].u.ui4_arg,
                                       (GL_HSURFACE_T)pt_args[1].u.ui4_arg,
                                       (GL_HSURFACE_T)pt_args[2].u.ui4_arg,
                                    (TFX_AUX_DATA_T *)pt_args[3].u.ps_str,
                                    (IMG_TFX_EFFECT_T)pt_args[4].u.i4_arg,
                                    (IMG_TFX_EFFECT_T)pt_args[5].u.i4_arg);

    return RPCR_OK;
}


INT32 _hndlr_x_img_tfx_stop(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_tfx_stop( (HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_tfx_pause(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_tfx_pause( (HANDLE_T)pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_img_tfx_get_effect_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_tfx_get_effect_info( (IMG_TFX_EFFECT_T)pt_args[0].u.i4_arg,
                                                    (TFX_FRAME_INFO_T*)pt_args[1].u.ps_str);

    return RPCR_OK;
}

INT32 _hndlr_x_img_set_get_effect_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_img_tfx_set_effect_info( (IMG_TFX_EFFECT_T)pt_args[0].u.i4_arg,
                                                    (TFX_FRAME_INFO_T*)pt_args[1].u.ps_str);

    return RPCR_OK;
}
INT32 _x_image_pull_associate_fct_wrapper(
                UINT32    	h_fm,
                VOID*       pv_app_tag,
                UINT32      ui4_phy_addr,
                UINT32      ui4_size)

{
    UINT8 ui1_idx;
    RPC_DECL(2, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_phy_addr);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_size);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_image_pull_associate_fct", pt_nfy_tag->apv_cb_addr_ex[7]);

    for (ui1_idx=10; ui1_idx<20; ui1_idx++)
    {
        if (NULL == pt_nfy_tag->apv_cb_addr_ex[ui1_idx])
        {
            IMG_PULL_MEMMAP_T* pv_tmp;
            pt_nfy_tag->apv_cb_addr_ex[ui1_idx] = malloc(sizeof(IMG_PULL_MEMMAP_T));
            pv_tmp = pt_nfy_tag->apv_cb_addr_ex[ui1_idx];
            pv_tmp->ui4_fm_handle = h_fm;
            pv_tmp->ui4_phy_addr = ui4_phy_addr;
            pv_tmp->ui4_size = ui4_size;
            pv_tmp->ui4_virt_addr = __t_ret.u.ui4_arg;
            break;
        }
    }
    if (ui1_idx >= 20)
    {
        printf("!!!!! _x_image_pull_associate_fct_wrapper too many association\n");
    }
    

    RPC_RETURN(ARG_TYPE_INT32, IMGR_RPC_ERROR);
}

INT32 _x_image_pull_dissociate_fct_wrapper(
                UINT32    h_fm,
                VOID*     pv_app_tag)
{
    UINT8 ui1_idx;
    IMG_PULL_MEMMAP_T* pv_tmp;

    RPC_DECL(2, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    for (ui1_idx=10; ui1_idx<20; ui1_idx++)
    {
        if (pt_nfy_tag->apv_cb_addr_ex[ui1_idx])
        {
            pv_tmp = pt_nfy_tag->apv_cb_addr_ex[ui1_idx];

            if (pv_tmp->ui4_fm_handle == h_fm)
            {
                break;
            }
        }
    }
    if (ui1_idx >= 20)
    {
        return 0;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32, pv_tmp->ui4_virt_addr);
    RPC_ARG_INP(ARG_TYPE_UINT32, pv_tmp->ui4_size);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_image_pull_dissociate_fct", pt_nfy_tag->apv_cb_addr_ex[8]);

    free(pv_tmp);
    pt_nfy_tag->apv_cb_addr_ex[ui1_idx] = NULL;

    RPC_RETURN(ARG_TYPE_INT32, IMGR_RPC_ERROR);

}


INT32 _x_image_pull_open_fct_wrapper(
	CHAR 		*puFileName, 
	UINT32 		*pui4FmHandle,
	VOID		*pv_app_tag)
{
	RPC_DECL(3, INT32);

	RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

	RPC_ARG_INP(ARG_TYPE_REF_STR, puFileName);
	RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4FmHandle);
	RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);

	RPC_DO_CB(pt_nfy_tag->t_id, "x_image_pull_open_fct", pt_nfy_tag->apv_cb_addr_ex[1]);

	RPC_RETURN(ARG_TYPE_INT32, IMGR_RPC_ERROR);
}

INT32 _x_image_pull_read_fct_wrapper(
	UINT32 			ui4FmHandle,  
	UINT8 			*pu1Dst, 
	UINT32 			ui4RequestLen, 
	UINT32 			*pui4ReadLen, 
	VOID			*pv_app_tag)
{
	VOID*   pv_addr;
	
	RPC_DECL(5, INT32);

	RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    IMG_PULL_MEMMAP_T* pv_tmp;
    UINT8 ui1_idx;

    for (ui1_idx=10; ui1_idx<20; ui1_idx++)
    {
        if (pt_nfy_tag->apv_cb_addr_ex[ui1_idx])
        {
            pv_tmp = pt_nfy_tag->apv_cb_addr_ex[ui1_idx];

            if (pv_tmp->ui4_fm_handle == ui4FmHandle)
            {
                break;
            }
        }
    }
    if (ui1_idx >= 20)
    {
        return IMGR_IO_ERROR;
    }

	pv_addr = (VOID *)(pv_tmp->ui4_virt_addr + ((UINT32)pu1Dst-pv_tmp->ui4_phy_addr));

	RPC_ARG_INP(ARG_TYPE_UINT32, ui4FmHandle);
	RPC_ARG_OUT(ARG_TYPE_VOID, pv_addr);
	RPC_ARG_INP(ARG_TYPE_UINT32, ui4RequestLen);
	RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4ReadLen);
	RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);

	RPC_DO_CB(pt_nfy_tag->t_id, "x_image_pull_read_fct", pt_nfy_tag->apv_cb_addr_ex[3]);

	RPC_RETURN(ARG_TYPE_INT32, IMGR_RPC_ERROR);
}

INT32 _x_image_pull_write_fct_wrapper(
	UINT32 			ui4FmHandle, 
	UINT8			*pu1Src, 
	UINT32 			ui4RequestLen, 
	UINT32 			*pui4WriteLen,
	VOID			*pv_app_tag)
{
	RPC_DECL(5, INT32);

	RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

	RPC_ARG_INP(ARG_TYPE_UINT32, ui4FmHandle);
	RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pu1Src, ui4RequestLen));
	RPC_ARG_INP(ARG_TYPE_REF_DESC, pu1Src);
	RPC_ARG_INP(ARG_TYPE_UINT32, ui4RequestLen);
	RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4WriteLen);
	RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);

	RPC_DO_CB(pt_nfy_tag->t_id, "x_image_pull_write_fct", pt_nfy_tag->apv_cb_addr_ex[4]);

	RPC_RETURN(ARG_TYPE_INT32, IMGR_RPC_ERROR);
}

INT32 _x_image_pull_seek_fct_wrapper(
	UINT32 			ui4FmHandle, 
	INT64 			i8SeekPos,
	UINT8 			u1Whence,
	UINT64 			*pu8CurPos, 
	VOID			*pv_app_tag)
{
	RPC_DECL(5, INT32);

	RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

	RPC_ARG_INP(ARG_TYPE_UINT32, ui4FmHandle);
	RPC_ARG_INP(ARG_TYPE_INT64, i8SeekPos);
	RPC_ARG_INP(ARG_TYPE_UINT8, u1Whence);
	RPC_ARG_OUT(ARG_TYPE_REF_UINT64, pu8CurPos);
	RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);

	RPC_DO_CB(pt_nfy_tag->t_id, "x_image_pull_seek_fct", pt_nfy_tag->apv_cb_addr_ex[5]);

	RPC_RETURN(ARG_TYPE_INT32, IMGR_RPC_ERROR);
}

INT32 _x_image_pull_close_fct_wrapper(
	UINT32 			ui4FmHandle, 
	VOID			*pv_app_tag)
{
	RPC_DECL(2, INT32);    

	RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;
    
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4FmHandle);
	RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_image_pull_close_fct", pt_nfy_tag->apv_cb_addr_ex[2]);

    RPC_RETURN(ARG_TYPE_INT32, IMGR_RPC_ERROR);
}
INT32 _x_image_pull_getInfo_fct_wrapper(
	CHAR 				*puFileName,
	IMAGE_FILE_INFO_T	*pt_fileInfo,
	VOID				*pv_app_tag)
{
	RPC_DECL(3, INT32);    

	RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;
    
    RPC_ARG_INP(ARG_TYPE_REF_STR, puFileName);
	RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_fileInfo, RPC_DESC_IMAGE_FILE_INFO_T, NULL);
	RPC_ARG_OUT(ARG_TYPE_REF_DESC,   pt_fileInfo);
	RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_image_pull_getInfo_fct", pt_nfy_tag->apv_cb_addr_ex[6]);

    RPC_RETURN(ARG_TYPE_INT32, IMGR_RPC_ERROR);
}

#if 0
static VOID _img_pull_tag_free_fnc(RPC_CB_NFY_TAG_T*  pt_cb_tag)
{
    UINT8 ui1_idx;
    
    for (ui1_idx=10; ui1_idx<20; ui1_idx++)
    {
        if (pt_cb_tag->apv_cb_addr_ex[ui1_idx])
        {
            free(pt_cb_tag->apv_cb_addr_ex[ui1_idx]);
        }
    }
}
#endif

INT32 _hndlr_x_img_set_fm_function(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
	RPC_CB_NFY_TAG_T*	pt_nfy_tag_pull=NULL;
	
	if (ui4_num_args != 1)
	{
		return RPCR_INV_ARGS;
	}

	IMAGE_PULL_FM_FUNC_T* pt_func = (IMAGE_PULL_FM_FUNC_T*)pt_args[0].u.pv_desc;
	VOID* av_cb[9];

    av_cb[0] = (VOID*)pt_func->pv_app_tag;
    av_cb[1] = (VOID*)pt_func->pfnOpen;
    av_cb[2] = (VOID*)pt_func->pfnClose;
    av_cb[3] = (VOID*)pt_func->pfnRead;
    av_cb[4] = (VOID*)pt_func->pfnWrite;
    av_cb[5] = (VOID*)pt_func->pfnSeek;
	av_cb[6] = (VOID*)pt_func->pfnGetInfo;
    av_cb[7] = (VOID*)_x_image_pull_associate_fct_wrapper;
    av_cb[8] = (VOID*)_x_image_pull_dissociate_fct_wrapper;

	pt_nfy_tag_pull = ri_create_cb_tag(t_rpc_id, av_cb, 9, pt_func->pv_app_tag);

	pt_func->pv_app_tag = pt_nfy_tag_pull;
	pt_func->pfAssociate = _x_image_pull_associate_fct_wrapper;
	pt_func->pfDissociate = _x_image_pull_dissociate_fct_wrapper;
	pt_func->pfnOpen = _x_image_pull_open_fct_wrapper;
	pt_func->pfnRead = _x_image_pull_read_fct_wrapper;
	pt_func->pfnWrite = _x_image_pull_write_fct_wrapper;
	pt_func->pfnSeek = _x_image_pull_seek_fct_wrapper;
	pt_func->pfnClose = _x_image_pull_close_fct_wrapper;
	pt_func->pfnGetInfo = _x_image_pull_getInfo_fct_wrapper;
	pt_return->e_type = ARG_TYPE_INT32;
	pt_return->u.i4_arg =x_img_set_fm_function(pt_func);

	if (pt_return->u.i4_arg == IMGR_OK && pt_nfy_tag_pull)
    {
        //ri_bind_cb_tag(*(HANDLE_T*)pt_args[4].u.pui4_arg, pt_nfy_tag_pull);
    }

	return RPCR_OK;
}

INT32 c_rpc_reg_img_op_hndlrs()
{
    RPC_REG_OP_HNDLR(x_img_fopen);
    RPC_REG_OP_HNDLR(x_img_mopen);
	RPC_REG_OP_HNDLR(x_img_close);
	RPC_REG_OP_HNDLR(x_img_get_dimension);
	RPC_REG_OP_HNDLR(x_img_stop);
	RPC_REG_OP_HNDLR(x_img_sync_load);
	RPC_REG_OP_HNDLR(x_img_async_load);
	RPC_REG_OP_HNDLR(x_img_get_capability);
	RPC_REG_OP_HNDLR(x_img_get_info_len);
	RPC_REG_OP_HNDLR(x_img_get_info);
	RPC_REG_OP_HNDLR(x_img_set_info);
	RPC_REG_OP_HNDLR(x_img_get_exif_info_len);
	RPC_REG_OP_HNDLR(x_img_get_exif_info);
	RPC_REG_OP_HNDLR(x_img_set_exif_info);
	RPC_REG_OP_HNDLR(x_img_open_exif);
	RPC_REG_OP_HNDLR(x_img_reparse_exif);
	RPC_REG_OP_HNDLR(x_img_close_exif);
	RPC_REG_OP_HNDLR(x_img_tfx_create);
	RPC_REG_OP_HNDLR(x_img_tfx_delete);
	RPC_REG_OP_HNDLR(x_img_tfx_play);
	RPC_REG_OP_HNDLR(x_img_tfx_stop);
	RPC_REG_OP_HNDLR(x_img_tfx_pause);
	RPC_REG_OP_HNDLR(x_img_tfx_get_effect_info);
	RPC_REG_OP_HNDLR(x_img_set_get_effect_info);
	RPC_REG_OP_HNDLR(x_img_set_fm_function);

	return RPCR_OK;
}
