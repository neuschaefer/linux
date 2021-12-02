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
 * $RCSfile: wgl_string.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/6 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: 2c46162b759413f7bb637f26bd65e2f5 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "unicode/x_uc_str.h"
#include "font/x_fe.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/wgl_string.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "STR"

#define IN
#define OUT

/*-----------------------------------------------------------------------------
                    public functions
 ----------------------------------------------------------------------------*/
INT32 wgl_str_init(
    IN OUT  WGL_STRING_T                *pt_str,
    IN      UINT16                      ui2_size_buf_init,
    IN      UINT16                      ui2_inc_buf_size,
    IN      HFONT_T                     h_font)
{
    INT32       i4_ret;
    
    pt_str->h_font = NULL_HANDLE;
    pt_str->w2s_text = NULL;
    pt_str->ui2_len_text = 0;
    pt_str->ui2_size_buf = 0;
    pt_str->ui2_inc_buf_size = 0;
    pt_str->i4_width = 0;
    pt_str->i4_height = 0;
    
    CHECK_VALID(    ui2_size_buf_init > 0 
                 && ui2_inc_buf_size > 0 
                 && h_font != NULL_HANDLE, WGLR_INV_ARG);

    pt_str->h_font = h_font;
    pt_str->w2s_text = (UTF16_T *)WGL_MEM_ALLOC((SIZE_T)(ui2_size_buf_init * UTF16_MAX_LEN));
    if(pt_str->w2s_text==NULL)
    {
        INFORM_ERR(WGLR_OUT_OF_MEMORY);
        return WGLR_OUT_OF_MEMORY;
    }
    pt_str->ui2_len_text = 0;
    pt_str->ui2_size_buf = ui2_size_buf_init;
    pt_str->ui2_inc_buf_size = ui2_inc_buf_size;

    return WGLR_OK;
}

INT32 wgl_str_destroy(
    IN      WGL_STRING_T                *pt_str)
{
    if(pt_str->w2s_text!=NULL)
    {
        WGL_MEM_FREE(pt_str->w2s_text);
        pt_str->w2s_text = NULL;
    }

    return WGLR_OK;
}

INT32 wgl_str_set_font(
    IN OUT  WGL_STRING_T                *pt_str,
    IN      HFONT_T                     h_font)
{
    CHECK_VALID(h_font!=NULL_HANDLE, WGLR_INV_ARG);

    pt_str->h_font = h_font;
    pt_str->i4_height = 0;
    pt_str->i4_width = 0;

    return WGLR_OK;
}

INT32 wgl_str_set_text(
    IN OUT  WGL_STRING_T                *pt_str,
    IN      UTF16_T                     *w2s_text,
    IN      UINT16                      ui2_len_text)
{
    INT32       i4_ret;
    UINT16      ui2_eff_len_text = ui2_len_text + (UINT16)1;

    if(ui2_eff_len_text > pt_str->ui2_size_buf)
    {   /* expand buffer */
        UINT16      ui2_new_size;
        UTF16_T     *w2s_buf_new;

        ui2_new_size = ui2_eff_len_text + ((UINT16)1 + ((ui2_eff_len_text - pt_str->ui2_size_buf) / pt_str->ui2_inc_buf_size)) * pt_str->ui2_inc_buf_size;
        w2s_buf_new = (UTF16_T *)WGL_MEM_ALLOC((SIZE_T)(ui2_new_size * UTF16_MAX_LEN));
        if(w2s_buf_new==NULL)
        {
            INFORM_ERR(WGLR_OUT_OF_MEMORY);
            return WGLR_OUT_OF_MEMORY;
        }
        WGL_MEM_FREE((VOID *)pt_str->w2s_text);
        pt_str->w2s_text = w2s_buf_new;
        pt_str->ui2_size_buf = ui2_new_size;
    }

    x_uc_w2s_strncpy(pt_str->w2s_text, w2s_text, ui2_len_text);
    pt_str->ui2_len_text = ui2_len_text;
    pt_str->i4_width = 0;
    x_uc_w2s_set(pt_str->w2s_text, ui2_len_text, WGL_CHAR_NULL);

    return WGLR_OK;    
}

INT32 wgl_str_clear_text(
    IN OUT  WGL_STRING_T                *pt_str)
{
    pt_str->ui2_len_text = 0;

    pt_str->i4_width = 0;

    return WGLR_OK;
}


INT32 wgl_str_get_height(
    IN OUT  WGL_STRING_T                *pt_str,
    OUT     INT32                       *pi4_height)
{
    INT32       i4_ret;
    
    *pi4_height = 0;
    if(pt_str->i4_height==0)
    {
        INT16   i2_font_h;
        CATCH_FAIL(x_fe_get_height(pt_str->h_font, &i2_font_h));
        pt_str->i4_height = i2_font_h;
    }

    *pi4_height = pt_str->i4_height;
    
    return WGLR_OK;
}

INT32 wgl_str_get_width(
    IN OUT  WGL_STRING_T                *pt_str,
    OUT     INT32                       *pi4_width)
{
    INT32       i4_ret;
    *pi4_width = 0;

    if(pt_str->ui2_len_text==0)
    {
        return WGLR_OK;
    }

    if(pt_str->i4_width==0)
    {
        INT32   i4_w, i4_h;
        CATCH_FAIL(x_fe_get_string_size(
            pt_str->h_font, 
            pt_str->w2s_text, 
            (INT32)pt_str->ui2_len_text,
            &i4_w,
            &i4_h));

        pt_str->i4_width = i4_w;
        pt_str->i4_height = i4_h;
    }

    *pi4_width = pt_str->i4_width;

    return WGLR_OK;
}

INT32 wgl_str_get_height_ex(
    IN OUT  WGL_STRING_T                *pt_str,
    IN      HFONT_T                     h_font,
    OUT     INT32                       *pi4_height)
{
    INT32       i4_ret;
    INT16       i2_font_h;

    if(pi4_height == NULL || pt_str == NULL)
    {
        return WGLR_OK;
    }
    
    CATCH_FAIL(x_fe_get_height(h_font, &i2_font_h));

    *pi4_height = i2_font_h;
    return WGLR_OK;
}

INT32 wgl_str_get_width_ex(
    IN OUT  WGL_STRING_T                *pt_str,
    IN      HFONT_T                     h_font,
    OUT     INT32                       *pi4_width)
{
    INT32       i4_ret;
    INT32       i4_w, i4_h;

    if(pi4_width == NULL || pt_str == NULL || pt_str->ui2_len_text==0)
    {
        return WGLR_OK;
    }

    CATCH_FAIL(x_fe_get_string_size(
        h_font, 
        pt_str->w2s_text, 
        (INT32)pt_str->ui2_len_text,
        &i4_w,
        &i4_h));

    *pi4_width = i4_w;

    return WGLR_OK;
}


BOOL wgl_str_check_font(
    IN      WGL_STRING_T                *pt_str,
    IN      WGL_FONT_INFO_T             *pt_finfo)
{
    INT32           i4_ret;
    FE_FNT_SIZE     e_size;
      
    if(pt_str->h_font==NULL_HANDLE)
    {
        return FALSE;
    }  

    i4_ret = x_fe_get_size(pt_str->h_font, &e_size);
    CHECK_VALID(i4_ret==FER_OK, FALSE);
    if(pt_finfo->e_font_size==e_size)
    {
        FE_FNT_STYLE    e_style;
        
        i4_ret = x_fe_get_style(pt_str->h_font, &e_style);
        CHECK_VALID(i4_ret==FER_OK, FALSE);
        if(pt_finfo->e_font_style==e_style)
        {
            FE_CMAP_ENCODING    e_cmap;
            
            i4_ret = x_fe_get_cmap(pt_str->h_font, &e_cmap);
            CHECK_VALID(i4_ret==FER_OK, FALSE);
            if(pt_finfo->e_font_cmap==e_cmap)
            {
                CHAR    s_fname[WGL_MAX_FONT_NAME+1];
                i4_ret = x_fe_get_name(pt_str->h_font, s_fname, WGL_MAX_FONT_NAME);
                CHECK_VALID(i4_ret==FER_OK, FALSE);
                return x_strcmp(pt_finfo->a_c_font_name, s_fname) ? FALSE : TRUE;
            }
        }
    }
    
    return FALSE;
}    
