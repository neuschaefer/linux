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
 * $RCSfile: wgl_bdr_transp.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: 6192f1e4ae10e34d4d73b0fcdb3e0c83 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "wgl/wgl_sets.h"
#include "wgl/u_wgl_sets.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/u_wgl_bdr_common.h"
#include "wgl/wgl_bdr_internal.h"
#include "wgl/bdr_transp/wgl_bdr_transp.h"



/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "BDRTP"

#define BDR_TRANSP_MIN_INSET_LEFT       0
#define BDR_TRANSP_MIN_INSET_RIGHT      0
#define BDR_TRANSP_MIN_INSET_TOP        0
#define BDR_TRANSP_MIN_INSET_BOTTOM     0

const static WGL_INSET_T BDR_TRANSP_MIN_INSET =
{
    BDR_TRANSP_MIN_INSET_LEFT, 
    BDR_TRANSP_MIN_INSET_RIGHT, 
    BDR_TRANSP_MIN_INSET_TOP, 
    BDR_TRANSP_MIN_INSET_BOTTOM
};

const static WGL_COLOR_INFO_T BDR_TRANSP_DUMMY_COLOR_INFO = 
{
    WGL_COLOR_SET_BASIC,
    { {{255, {0}, {0}, {0}}, {255, {0}, {0}, {0} }} }
};

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
typedef struct _BDR_TRANSP_CTX_T
{
#ifdef CLI_LVL_ALL     
    UINT32              ui4_mem_size;/*this field must be the first field in the structure*/
#endif   
    GL_SIZE_T           t_size;
    WGL_INSET_T         t_inset;  
    
} BDR_TRANSP_CTX_T;


/*-----------------------------------------------------------------------------
                    exported functions
 ----------------------------------------------------------------------------*/
INT32 x_wgl_bdr_transp_init(VOID)
{
    WGL_BORDER_CTRL_INTF_T      t_bdr_intf = {  wgl_bdr_transp_inst_create,
                                                wgl_bdr_transp_inst_destroy,
                                                wgl_bdr_transp_cmd_proc,
                                                wgl_bdr_transp_paint
                                             };
    
    return wgl_register_border_intf(WGL_BORDER_TRANSP, &t_bdr_intf);
}


/*-----------------------------------------------------------------------------
                    public functions
 ----------------------------------------------------------------------------*/
INT32 wgl_bdr_transp_inst_create(
                    WGL_BORDER_CONTEXT_HANDLE_T *ppv_ctx_bdr, 
                    WGL_BORDER_CREATE_INPUT_T   *pt_bdr_create)
{
    BDR_TRANSP_CTX_T    *pt_ctx;

    pt_ctx = WGL_MEM_ALLOC(sizeof(BDR_TRANSP_CTX_T));
    if(pt_ctx==NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }
#ifdef CLI_LVL_ALL   
    pt_ctx->ui4_mem_size = sizeof(BDR_TRANSP_CTX_T);
#endif      
    pt_ctx->t_size.ui4_width = (UINT32)pt_bdr_create->i4_width;
    pt_ctx->t_size.ui4_height = (UINT32)pt_bdr_create->i4_height;
    pt_ctx->t_inset.i4_bottom = BDR_TRANSP_MIN_INSET_BOTTOM;
    pt_ctx->t_inset.i4_top = BDR_TRANSP_MIN_INSET_TOP;
    pt_ctx->t_inset.i4_right = BDR_TRANSP_MIN_INSET_RIGHT;
    pt_ctx->t_inset.i4_left = BDR_TRANSP_MIN_INSET_LEFT;

    *ppv_ctx_bdr = (WGL_BORDER_CONTEXT_HANDLE_T)pt_ctx;
    return WGLR_OK;
}


INT32 wgl_bdr_transp_inst_destroy(
                     WGL_BORDER_CONTEXT_HANDLE_T        pv_ctx_bdr)
{

    WGL_MEM_FREE(pv_ctx_bdr);
    return WGLR_OK;
}

INT32 wgl_bdr_transp_paint(
    WGL_BORDER_CONTEXT_HANDLE_T         pv_ctx_bdr,
    GL_HGRAPHICS_T                      h_graphics,
    UINT32                              ui4_upt_rect,
    const GL_RECT_T*                    pt_upt_rect,
    UINT16                              ui2_state)
{
    /*Because this is an transparent border, don't need to paint anything*/

    return WGLR_OK;
}


/*border command procedure type*/              
INT32 wgl_bdr_transp_cmd_proc(
                         WGL_BORDER_CONTEXT_HANDLE_T    pv_ctx_bdr,
                         UINT32                         ui4_cmd_id, 
                         VOID                           *pv_param1, 
                         VOID                           *pv_param2)
{
    BDR_TRANSP_CTX_T    *pt_ctx = (BDR_TRANSP_CTX_T *)pv_ctx_bdr;
    INT32               i4_ret, i4_ret_val = WGLR_OK;

    if(!(WGL_IS_CMD_BELONG_BORDER(WGL_BORDER_BASE, ui4_cmd_id) ||
         WGL_IS_CMD_BELONG_BORDER(WGL_BORDER_TRANSP, ui4_cmd_id)) )
    {
        INFORM_ERR(WGLR_INV_CMD);
        return WGLR_INV_CMD;
    }
    
    switch(ui4_cmd_id)
    {
        /*Internal Border Commands*/
        case WGL_BDR_GL_SET_SIZE:
        {
            INT32       i4_size_w = (INT32)pv_param1;
            INT32       i4_size_h = (INT32)pv_param2;
            pt_ctx->t_size.ui4_width = (UINT32)i4_size_w;
            pt_ctx->t_size.ui4_height = (UINT32)i4_size_h;
            break;
        }
        
        /*Common Border Commands*/
        case WGL_CMD_GL_SET_INSET:
        {
            WGL_INSET_T *pt_inset = (WGL_INSET_T *)pv_param1;
            if(  
                 (pt_inset->i4_right<BDR_TRANSP_MIN_INSET_RIGHT || pt_inset->i4_left<BDR_TRANSP_MIN_INSET_LEFT 
                           || pt_inset->i4_top<BDR_TRANSP_MIN_INSET_TOP || pt_inset->i4_bottom<BDR_TRANSP_MIN_INSET_BOTTOM )
                      ||
                 !_IS_INSET_FIT_SIZE(*pt_inset, pt_ctx->t_size)
              )
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }

            x_memcpy( &pt_ctx->t_inset, pv_param1, sizeof(WGL_INSET_T) );
        }
        break;
        
        case WGL_CMD_GL_GET_INSET:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            x_memcpy(pv_param1, &pt_ctx->t_inset, sizeof(WGL_INSET_T));
        }
        break;

        case WGL_CMD_GL_GET_MIN_INSET:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            x_memcpy(pv_param1, &BDR_TRANSP_MIN_INSET, sizeof(WGL_INSET_T));
        }
        break;

        case WGL_CMD_GL_SET_IS_BDR_DRAW_INTERIOR:
        {
            BOOL b_is_draw_intr = WGL_POINTER_TO_BOOL(pv_param1);
            if(!_IS_VALID_BOOL(b_is_draw_intr))
            {
                i4_ret_val = WGLR_INV_ARG;
                break;
            }
        }
        break;
            
        case WGL_CMD_GL_GET_IS_BDR_DRAW_INTERIOR:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            /*Because this is a transparent border, always return FALSE*/
            *((BOOL *)pv_param1) = FALSE; 
        }
        break;

        case WGL_CMD_GL_SET_BDR_COLOR:
        /*transparent border doesn't have to keep any color information*/
        break;
        
        case WGL_CMD_GL_GET_BDR_COLOR:
        {
            CHECK_NULL(pv_param1, WGLR_INV_ARG);
            /*returns the dummy color*/
            x_memcpy(((WGL_COLOR_INFO_T *)pv_param1), &BDR_TRANSP_DUMMY_COLOR_INFO, sizeof(WGL_COLOR_INFO_T));
        }
        break;
        case WGL_CMD_GL_SET_BDR_TEXT_DIRECTION:
           break;

        default:
            i4_ret_val = WGLR_INV_CMD;          
            
    }

    return i4_ret_val;
}



