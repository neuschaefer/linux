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
 * $RCSfile: wgl_frlm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: ae7541ea7b7f3f0da11a4eab327b714e $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "font/x_fe.h"
#include "handle/x_handle.h"
#include "wgl/wgl_tools.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/frlm/wgl_frlm.h"
#include "wgl/frlm/wgl_msfs.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "FRLM"

 
/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
VOID wgl_frlm_init( IN OUT WGL_FRLM_CTX_T *pt_ctx )
{
#ifdef CLI_LVL_ALL
	pt_ctx->ui4_mem_size = 0;
#endif

    pt_ctx->ui2_num_fonts = 0;
    pt_ctx->pt_font_list = NULL;
    pt_ctx->ui2_num_reg_fonts = 0;
    pt_ctx->at_font_reg_tbl = NULL;
}

VOID wgl_frlm_destroy( IN WGL_FRLM_CTX_T *pt_ctx )
{
    WGL_FRLM_FONT_RES_T *pt_font_res = pt_ctx->pt_font_list;
    WGL_FRLM_FONT_RES_T *pt_font_res_next;
    
    while(pt_font_res)
    {
        WGL_FONT_FREE(pt_font_res->h_font);
        pt_font_res_next = pt_font_res->pt_next;
        WGL_MEM_FREE((VOID *)pt_font_res);
        pt_font_res = pt_font_res_next;
    }
    
    pt_ctx->ui2_num_fonts = 0;
    pt_ctx->pt_font_list = NULL;
    pt_ctx->ui2_num_reg_fonts = 0;
    pt_ctx->at_font_reg_tbl = NULL;

#ifdef CLI_LVL_ALL
	pt_ctx->ui4_mem_size = 0;
#endif
}

INT32 wgl_frlm_get_handle_by_info(  IN WGL_FRLM_CTX_T   *pt_ctx,
                                    IN WGL_FONT_INFO_T  *pt_font_info, 
                                    OUT HFONT_T         *ph_font_out)
{
    WGL_FRLM_FONT_RES_T     *pt_font_res;
    WGL_FRLM_FONT_RES_T     *pt_font_res_last;
    WGL_FRLM_FONT_RES_T     *pt_font_res_new;
    HFONT_T                 h_new_font;
    INT32                   i4_temp, i4_ret;
    INT16                   i2_font_height;


    if(ph_font_out==NULL || pt_font_info==NULL || pt_ctx==NULL)
    {
        return FRLMR_INV_ARG;
    }
    pt_font_res = pt_ctx->pt_font_list;
    pt_font_res_last = pt_font_res;

    /*find the font handle that is already created*/
    while(pt_font_res)
    {
        if(pt_font_res->t_font_info.e_font_cmap == pt_font_info->e_font_cmap
            && pt_font_res->t_font_info.e_font_size == pt_font_info->e_font_size 
            && pt_font_res->t_font_info.e_font_style == pt_font_info->e_font_style
            && !x_strcmp(pt_font_res->t_font_info.a_c_font_name, pt_font_info->a_c_font_name)
            && (pt_font_res->t_font_info.e_font_size!=FE_FNT_SIZE_CUSTOM ||
            pt_font_res->t_font_info.ui1_custom_size== pt_font_info->ui1_custom_size)
          )
        {
            *ph_font_out = pt_font_res->h_font;
            return FRLMR_OK;
        }

        pt_font_res_last = pt_font_res;
        pt_font_res = pt_font_res->pt_next;
    }

    /*create the font*/
    if(MSFSR_OK != wgl_msfs_create_font(
                    pt_font_info,
                    pt_ctx->at_font_reg_tbl,
                    pt_ctx->ui2_num_reg_fonts,
                    &h_new_font))
    {
        INFORM_ERR(FRLMR_INV_FONT_INFO);
        return FRLMR_INV_FONT_INFO;
    }
    
    /*Check the validity of the font*/
    i2_font_height = 0;
    i4_ret = x_fe_get_height(h_new_font, &i2_font_height);
    if(i4_ret!=FER_OK || i2_font_height<=0 )
    {
        INFORM_ERR(FRLMR_INTERNAL_ERROR);
    }   
    
    /*allocate a new font resource instance*/
    pt_font_res_new = (WGL_FRLM_FONT_RES_T *)WGL_MEM_ALLOC(sizeof(WGL_FRLM_FONT_RES_T));
    if(pt_font_res_new==NULL)
    {
        INFORM_ERR(FRLMR_OUT_OF_MEMORY);
        return FRLMR_OUT_OF_MEMORY;
    }
#ifdef CLI_LVL_ALL
	pt_ctx->ui4_mem_size += sizeof(WGL_FRLM_FONT_RES_T);
#endif

    pt_font_res_new->h_font = h_new_font;
    x_strcpy(pt_font_res_new->t_font_info.a_c_font_name, pt_font_info->a_c_font_name);
    pt_font_res_new->t_font_info.e_font_cmap = pt_font_info->e_font_cmap;
    pt_font_res_new->t_font_info.e_font_size = pt_font_info->e_font_size;
    pt_font_res_new->t_font_info.e_font_style = pt_font_info->e_font_style;
    pt_font_res_new->t_font_info.ui1_custom_size = pt_font_info->ui1_custom_size;
    pt_font_res_new->pt_next = NULL;

    /*init for addtional font information*/
    i4_ret = wgl_msfs_get_glyph_advance (pt_font_res_new->h_font, 0x0020, NULL, 0, 0, &pt_font_res_new->i4_space_width, &i4_temp);
    
    if(!(i4_ret==FER_OK||i4_ret==FER_NO_CHAR_CODE))
    {
        INFORM_ERR(FRLMR_INTERNAL_ERROR);
        pt_font_res_new->i4_space_width = 24;
    }
    

    /*append the font resource into the list*/
    if(pt_ctx->pt_font_list==NULL)
    {
        pt_ctx->ui2_num_fonts = 1;
        pt_ctx->pt_font_list = pt_font_res_new;
    }
    else
    {
        if(!(pt_font_res_last && pt_font_res_last->pt_next==NULL))
        {
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            if(pt_font_res_last==NULL)
            {
                pt_font_res_last = pt_ctx->pt_font_list;
            }
        }
        
        pt_font_res_last->pt_next = pt_font_res_new;
        pt_ctx->ui2_num_fonts++;
    }

    *ph_font_out = pt_font_res_new->h_font;
    return FRLMR_OK;
}

INT32 wgl_frlm_set_handle_by_info(  IN WGL_FRLM_CTX_T   *pt_ctx,
                                    IN WGL_FONT_INFO_T  *pt_font_info, 
                                    OUT HFONT_T         *ph_font_out)
{
    WGL_FRLM_FONT_RES_T     *pt_font_res;
    WGL_FRLM_FONT_RES_T     *pt_font_res_last;
    WGL_FRLM_FONT_RES_T     *pt_font_res_new;
    HFONT_T                 h_new_font;
    INT32                   i4_temp, i4_ret;
    INT16                   i2_font_height;


    if(ph_font_out==NULL || pt_font_info==NULL || pt_ctx==NULL)
    {
        return FRLMR_INV_ARG;
    }

    /*create the font*/
    if(MSFSR_OK != wgl_msfs_create_font(
                    pt_font_info,
                    pt_ctx->at_font_reg_tbl,
                    pt_ctx->ui2_num_reg_fonts,
                    &h_new_font))
    {
        INFORM_ERR(FRLMR_INV_FONT_INFO);
        return FRLMR_INV_FONT_INFO;
    }
    
    /*Check the validity of the font*/
    i2_font_height = 0;
    i4_ret = x_fe_get_height(h_new_font, &i2_font_height);
    if(i4_ret!=FER_OK || i2_font_height<=0 )
    {
        INFORM_ERR(FRLMR_INTERNAL_ERROR);
    }   

    pt_font_res = pt_ctx->pt_font_list;
    pt_font_res_last = pt_font_res;

    /*find the font handle that is already created*/
    while(pt_font_res)
    {
        if(pt_font_res->h_font == h_new_font)
        {
            *ph_font_out = pt_font_res->h_font;           
            WGL_FONT_FREE(h_new_font);
            return FRLMR_OK;
        }

        pt_font_res_last = pt_font_res;
        pt_font_res = pt_font_res->pt_next;
    }

    
    /*allocate a new font resource instance*/
	pt_font_res_new = (WGL_FRLM_FONT_RES_T *)WGL_MEM_ALLOC(sizeof(WGL_FRLM_FONT_RES_T));
	if(pt_font_res_new==NULL)
    {
        INFORM_ERR(FRLMR_OUT_OF_MEMORY);
        WGL_FONT_FREE(h_new_font);
        return FRLMR_OUT_OF_MEMORY;
    }
#ifdef CLI_LVL_ALL
	pt_ctx->ui4_mem_size += sizeof(WGL_FRLM_FONT_RES_T);
#endif

	
    pt_font_res_new->h_font = h_new_font;
    x_strcpy(pt_font_res_new->t_font_info.a_c_font_name, pt_font_info->a_c_font_name);
    pt_font_res_new->t_font_info.e_font_cmap = pt_font_info->e_font_cmap;
    pt_font_res_new->t_font_info.e_font_size = pt_font_info->e_font_size;
    pt_font_res_new->t_font_info.e_font_style = pt_font_info->e_font_style;
    pt_font_res_new->t_font_info.ui1_custom_size = pt_font_info->ui1_custom_size;
    pt_font_res_new->pt_next = NULL;

    /*init for addtional font information*/
    i4_ret = wgl_msfs_get_glyph_advance (pt_font_res_new->h_font, 0x0020, NULL, 0, 0, &pt_font_res_new->i4_space_width, &i4_temp);
    
    if(!(i4_ret==FER_OK||i4_ret==FER_NO_CHAR_CODE))
    {
        INFORM_ERR(FRLMR_INTERNAL_ERROR);
        pt_font_res_new->i4_space_width = 24;
    }
    

    /*append the font resource into the list*/
    if(pt_ctx->pt_font_list==NULL)
    {
        pt_ctx->ui2_num_fonts = 1;
        pt_ctx->pt_font_list = pt_font_res_new;
    }
    else
    {
        if(!(pt_font_res_last && pt_font_res_last->pt_next==NULL))
        {
            INFORM_ERR(WGLR_INTERNAL_ERROR);
            if(pt_font_res_last==NULL)
            {
                pt_font_res_last = pt_ctx->pt_font_list;
            }
        }
        
        pt_font_res_last->pt_next = pt_font_res_new;
        pt_ctx->ui2_num_fonts++;
    }

    *ph_font_out = pt_font_res_new->h_font;
    return FRLMR_OK;
}

INT32 wgl_frlm_change_font_attr(
    HANDLE_T*                   ph_font_handle,
    const WGL_FONT_ATTR_T*      pt_attr,
    UINT32                      ui4_mask)
{
    if(NULL == ph_font_handle || NULL_HANDLE == *ph_font_handle || NULL == pt_attr || 0 == ui4_mask)
    {
        return WGLR_INV_ARG;
    }
    
    wgl_font_change_attr(ph_font_handle,pt_attr,ui4_mask);

    return FRLMR_OK;
}

INT32 wgl_frlm_get_info(IN WGL_FRLM_CTX_T      *pt_ctx,
                        IN HFONT_T             h_font, 
                        OUT WGL_FONT_INFO_T    *pt_font_info_out)
{
    WGL_FRLM_FONT_RES_T *pt_font_res;

    if(h_font==NULL_HANDLE || pt_font_info_out==NULL || pt_ctx==NULL)
    {
        return FRLMR_INV_ARG;
    }

    pt_font_res = pt_ctx->pt_font_list;

    while(pt_font_res)
    {
        if( h_font==pt_font_res->h_font)
        {
            x_strcpy(pt_font_info_out->a_c_font_name, pt_font_res->t_font_info.a_c_font_name);
            pt_font_info_out->e_font_cmap = pt_font_res->t_font_info.e_font_cmap;
            pt_font_info_out->e_font_size = pt_font_res->t_font_info.e_font_size;
            pt_font_info_out->e_font_style = pt_font_res->t_font_info.e_font_style;
            pt_font_info_out->i2_width = pt_font_res->t_font_info.i2_width;
            pt_font_info_out->ui1_custom_size = pt_font_res->t_font_info.ui1_custom_size;
            return FRLMR_OK;
        }

        pt_font_res = pt_font_res->pt_next;
    }
     
    return FRLMR_HANDLE_NOT_FOUND;
}

INT32 wgl_frlm_get_space_width(IN WGL_FRLM_CTX_T    *pt_ctx,
                               IN HFONT_T           h_font,
                               OUT INT32            *pi4_space_width)
{
    WGL_FRLM_FONT_RES_T *pt_font_res;

    if(h_font==NULL_HANDLE || pi4_space_width==NULL || pt_ctx==NULL)
    {
        return FRLMR_INV_ARG;
    }

    *pi4_space_width = 0;
    pt_font_res = pt_ctx->pt_font_list;
    
    while(pt_font_res)
    {
        if( h_font==pt_font_res->h_font)
        {
            *pi4_space_width = pt_font_res->i4_space_width;
            if(*pi4_space_width==0)
            {
                return FRLMR_INTERNAL_ERROR;
            }
            return FRLMR_OK;
        }

        pt_font_res = pt_font_res->pt_next;
    }

    return FRLMR_HANDLE_NOT_FOUND;
      
}

UINT16 wgl_frlm_get_max_font_height(IN WGL_FRLM_CTX_T *pt_ctx)
{
    INT16                   i2_max_font_h;
    INT16                   i2_font_h = 0;
    WGL_FRLM_FONT_RES_T     *pt_fres;
    INT32                   i4_ret;
    
    i2_max_font_h = 0;
    pt_fres = pt_ctx->pt_font_list;
    while(pt_fres)
    {
        if(FER_OK!=(i4_ret=x_fe_get_height(pt_fres->h_font, &i2_font_h)))
        {
            INFORM_ERR(i4_ret);
            continue;
        }
        
        if(i2_font_h > i2_max_font_h)
        {
            i2_max_font_h = i2_font_h;
        }
        pt_fres = pt_fres->pt_next;
    }

    return (UINT16)i2_max_font_h;

}

VOID wgl_frlm_set_font_reg_table(
    IN OUT  WGL_FRLM_CTX_T              *pt_ctx,
    IN      WGL_MSFS_FONT_REG_INFO_T    *at_font_reg_tbl,
    IN      UINT16                      ui2_num_reg_fonts)
{
    UINT16                  ui2_i;
    WGL_FRLM_FONT_RES_T     *pt_font_res;
    INT32                   i4_temp,i4_ret;
    
    pt_ctx->ui2_num_reg_fonts = ui2_num_reg_fonts;
    pt_ctx->at_font_reg_tbl   = at_font_reg_tbl;
    
    pt_font_res = pt_ctx->pt_font_list;
    for(ui2_i = 0; ui2_i < pt_ctx->ui2_num_fonts && pt_font_res; ui2_i++ )
    {
        /* update the additional font information */
        i4_ret = wgl_msfs_get_glyph_advance (pt_font_res->h_font, 0x0020, NULL, 0, 0, &pt_font_res->i4_space_width, &i4_temp);
        
        if(!(i4_ret==FER_OK || i4_ret==FER_NO_CHAR_CODE))
        {
            INFORM_ERR(i4_ret);
            pt_font_res->i4_space_width = 0;
        }
        
        /* update the reg font info */
        INFORM_ERR(wgl_msfs_update_font_reg_tbl(pt_font_res->h_font, at_font_reg_tbl, ui2_num_reg_fonts));
        
        pt_font_res = pt_font_res->pt_next;
    }    
    
}

VOID wgl_frlm_get_font_reg_table(
    IN OUT  WGL_FRLM_CTX_T              *pt_ctx,
    OUT     WGL_MSFS_FONT_REG_INFO_T    **pat_font_reg_tbl,
    OUT     UINT16                      *pui2_num_reg_fonts)
{
    *pat_font_reg_tbl = pt_ctx->at_font_reg_tbl;
    *pui2_num_reg_fonts = pt_ctx->ui2_num_reg_fonts;
}


