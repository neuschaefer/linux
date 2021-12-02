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
 * $RCSfile: wgl_msfs.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: 29458c8cd5bddbddc7b1f29bd77b7894 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "font/x_fe.h"
#include "unicode/x_uc_str.h"
#include "handle/x_handle.h"
#include "wgl/wgl_tools.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/frlm/wgl_msfs.h"
#include "wgl/wgl_bdr_tools.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "MSFS"


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    global static data
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _msfs_get_nms_chars_width(
    IN      HFONT_T                     h_font, 
    IN      UTF16_T                     *w2s_chars, 
    IN      UINT16                      ui2_num_chars,
    IN      UTF16_T                     *w2s_skip,
    IN      UINT16                      ui2_num_skip,
    IN      UINT16                      ui2_w_skip,
    OUT     INT32                       *pi4_width);
    
static BOOL _msfs_is_skip_char(
    IN      UTF16_T                     *w2s_skip,
    IN      UINT16                      ui2_num_skip,
    IN      UTF16_T                     w2_ch);
    
static INT32 _msfs_update_font_reg_info(
    IN OUT  HFONT_T                     h_font,
    IN      WGL_MSFS_FONT_REG_INFO_T    *at_font_reg_tbl,
    IN      UINT16                      ui2_num_reg_fonts);
    
static INT32 _msfs_get_max_char_in_box(
    IN  HFONT_T                     h_font,
    IN  INT32                       i4_msfs_font_w,
    IN  const UTF16_T*              w2s_str,
    IN  INT32                       i4_max_len,
    IN  BOOL                        b_check_length,
    IN  INT32                       i4_box_w,
    IN  const UTF16_T*              w2s_can_break,
    OUT UTF16_T**                   pw2s_next,
    OUT UINT32*                     pui4_num_ch,
    OUT INT32*                      pi4_width);

/*-----------------------------------------------------------------------------
                    public functions
 ----------------------------------------------------------------------------*/
INT32 wgl_msfs_create_font(
    IN  WGL_FONT_INFO_T             *pt_finfo,
    IN  WGL_MSFS_FONT_REG_INFO_T    *at_font_reg_tbl,
    IN  UINT16                      ui2_num_reg_fonts,         
    OUT HFONT_T*                    ph_font)         
{
    INT32               i4_ret;

    *ph_font = NULL_HANDLE;
    
    i4_ret = WGL_FONT_CREATE (pt_finfo, ph_font);
    if(i4_ret != WGLR_OK)
    {
        FE_FNT_STYLE        e_style_new = FE_FNT_STYLE_REGULAR;
        WGL_FONT_INFO_T     t_finfo_new = *pt_finfo;
        
        DBG_ERROR(("{WGL(%s)} ERR: Create Font Fail(%d) (Name:%s Size:%d Style:%d)\n", DBG_MODULE_NAME, i4_ret, pt_finfo->a_c_font_name, pt_finfo->e_font_size, pt_finfo->e_font_style));
        
        if(HAS_FLAG(pt_finfo->e_font_style, FE_FNT_STYLE_UNDERLINE))
        {
            WGL_SET_FLAG(e_style_new, FE_FNT_STYLE_UNDERLINE);
        }
        
        t_finfo_new.e_font_style = e_style_new;
        
        i4_ret = WGL_FONT_CREATE (&t_finfo_new, ph_font);
        
        if(i4_ret != WGLR_OK)
        {
            DBG_ERROR(("{WGL(%s)} ERR: Create Backup Font Fail(%d) (Name:%s Size:%d Style:%d)\n", DBG_MODULE_NAME, i4_ret, pt_finfo->a_c_font_name, pt_finfo->e_font_size, FE_FNT_STYLE_REGULAR));
            
            t_finfo_new.a_c_font_name[0] = '\0';
            
            i4_ret = WGL_FONT_CREATE (&t_finfo_new, ph_font);
            
            if(i4_ret != WGLR_OK)
            {
                DBG_ERROR(("{WGL(%s)} ERR: Create System Default Font Fail(%d) (Size:%d Style:%d)\n", DBG_MODULE_NAME, i4_ret, pt_finfo->e_font_size, FE_FNT_STYLE_REGULAR));
                
                return WGLR_INV_ARG;
            }
            
        }
    }

    if(at_font_reg_tbl!=NULL)
    {
        CATCH_FAIL(_msfs_update_font_reg_info(*ph_font, at_font_reg_tbl, ui2_num_reg_fonts));
    }
        
    return MSFSR_OK;
}


UINT16 wgl_msfs_get_ms_font_w(
    IN  HFONT_T                     h_font)
{
    VOID        *pv_tag=NULL;

    if( HR_OK==x_handle_get_tag(h_font, &pv_tag) )
    {
        if(pv_tag)
        {
            WGL_MSFS_FONT_REG_INFO_T *pt_font_reg_info = (WGL_MSFS_FONT_REG_INFO_T *)pv_tag;
            return pt_font_reg_info->ui2_mono_space_width;
        }
    }

    return 0;
}

INT32 wgl_msfs_get_font_reg_info(
    IN      HFONT_T                     h_font,
    OUT     WGL_MSFS_FONT_REG_INFO_T    **ppt_font_reg_info)
{
    INT32       i4_ret;
    VOID        *pv_tag=NULL;

    *ppt_font_reg_info = NULL;
    
    CATCH_FAIL(x_handle_get_tag(h_font, &pv_tag));
    
    if(pv_tag)
    {
        *ppt_font_reg_info = (WGL_MSFS_FONT_REG_INFO_T *)pv_tag;
    }
    
    return MSFSR_OK;    
}    

INT32 wgl_msfs_update_font_reg_tbl(
    IN OUT  HANDLE_T                    h_font,
    IN      WGL_MSFS_FONT_REG_INFO_T    *at_font_reg_tbl,
    IN      UINT16                      ui2_num_reg_fonts)
{
    INT32 i4_ret;
    
    if(at_font_reg_tbl != NULL)
    {
        CATCH_FAIL(_msfs_update_font_reg_info(h_font, at_font_reg_tbl, ui2_num_reg_fonts));
    }
    
    return MSFSR_OK;
}    


INT32 wgl_msfs_get_max_advance(
    HFONT_T                         h_font,
    INT16*                          pi2_max_adv_w,  /* maximum advance width         */
    INT16*                          pi2_max_adv_h)  /* maximum advance height        */
{
    INT32                           i4_ret;
    INT16                           i2_max_adv_w = 0;

    *pi2_max_adv_h = 0;
    if( FER_OK != (i4_ret = x_fe_get_max_advance(h_font, &i2_max_adv_w, pi2_max_adv_h)) )
    {
        *pi2_max_adv_w = i2_max_adv_w;
        return i4_ret;
    }

    *pi2_max_adv_w = (INT16)wgl_msfs_get_ms_font_w(h_font);
    if(*pi2_max_adv_w<=0)
    {
        *pi2_max_adv_w = i2_max_adv_w;
    }

    return WGLR_OK;
}

    
INT32 wgl_msfs_get_chars_width(
    IN  HFONT_T                   h_font,
    IN  UTF16_T*                  w2s_chars,
    IN  UINT16                    ui2_num_chars,
    IN  UTF16_T*                  w2s_skip,
    IN  UINT16                    ui2_num_skip,
    IN  UINT16                    ui2_w_skip,
    OUT INT32*                    pi4_width)
{
    INT32                       i4_ms_font_w, i4_ret;
    
    i4_ms_font_w = (INT32)wgl_msfs_get_ms_font_w(h_font);
    if(i4_ms_font_w<=0)
    {
        i4_ret = _msfs_get_nms_chars_width(
            h_font, 
            w2s_chars, 
            ui2_num_chars, 
            w2s_skip,
            ui2_num_skip,
            ui2_w_skip,
            pi4_width);
        if(!(i4_ret==FER_OK || i4_ret==FER_NO_CHAR_CODE))
        {
            return i4_ret;
        }
    }
    else
    {
        *pi4_width = i4_ms_font_w * (INT32)ui2_num_chars;
    }

    return WGLR_OK;
}

INT32 wgl_msfs_get_glyph_advance(
    HFONT_T                 h_font,
    UTF16_T                 w2_char,
    UTF16_T                 *w2s_skip,
    UINT16                  ui2_num_skip,
    UINT16                  ui2_w_skip,
    INT32*                  pi4_hori,       
    INT32*                  pi4_vert)      
{
    INT32       i4_ms_font_w;
    INT32       i4_ret;

    *pi4_hori = *pi4_vert = 0;
    
    i4_ret = x_fe_get_character_size(h_font, (CHAR_CODE)w2_char, pi4_hori, pi4_vert);
    if(!(i4_ret==FER_OK || i4_ret==FER_NO_CHAR_CODE))
    {
        return i4_ret;
    }
    i4_ms_font_w = (INT32)wgl_msfs_get_ms_font_w(h_font);
    if(i4_ms_font_w > 0)
    {
        *pi4_hori = i4_ms_font_w;
    }
    /* check if skip char */
    else if(w2s_skip && ui2_num_skip > 0)
    {
        UINT16  ui2_i;
        for(ui2_i = 0; ui2_i < ui2_num_skip; ui2_i++)
        {
            if(w2_char == w2s_skip[ui2_i])
            {
                *pi4_hori = (INT32)ui2_w_skip;
                break;
            }
        }
    }

    return WGLR_OK;
}


INT32 wgl_msfs_get_max_char_in_box(
    HFONT_T                     h_font,
    const UTF16_T*              w2s_str,
    INT32                       i4_max_len,
    BOOL                        b_check_length,
    INT32                       i4_box_w,
    const UTF16_T*              w2s_can_break,
    const UTF16_T*              w2s_skip,
    UINT16                      ui2_num_skip,
    UINT16                      ui2_w_skip,
    UTF16_T**                   pw2s_next,
    UINT32*                     pui4_num_ch,
    INT32*                      pi4_width)
{
    INT32   i4_ms_font_w = (INT32) wgl_msfs_get_ms_font_w(h_font);
    INT32   i4_ret_fnl;

    if(i4_ms_font_w<=0)
    {
        i4_ret_fnl = wgl_get_max_char_in_box_cc(
            h_font, 
            w2s_str, 
            i4_max_len, 
            b_check_length, 
            i4_box_w, 
            w2s_can_break, 
            w2s_skip,
            ui2_num_skip,
            ui2_w_skip,
            TRUE,
            pw2s_next, 
            pui4_num_ch, 
            pi4_width);
    }
    else
    {
        i4_ret_fnl = _msfs_get_max_char_in_box(
            h_font, 
            i4_ms_font_w, 
            w2s_str, 
            i4_max_len, 
            b_check_length, 
            i4_box_w, 
            w2s_can_break, 
            pw2s_next, 
            pui4_num_ch, 
            pi4_width);
    }
    
    return i4_ret_fnl;
}

BOOL wgl_msfs_has_kerning(
    HFONT_T                     h_font)
{
    INT32   i4_ms_font_w = (INT32) wgl_msfs_get_ms_font_w(h_font);

    if(i4_ms_font_w<=0)
    {
        return x_fe_has_kerning(h_font);
    }
    
    return FALSE;
}


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _msfs_get_nms_chars_width(
    IN      HFONT_T                     h_font, 
    IN      UTF16_T                     *w2s_chars, 
    IN      UINT16                      ui2_num_chars,
    IN      UTF16_T                     *w2s_skip,
    IN      UINT16                      ui2_num_skip,
    IN      UINT16                      ui2_w_skip,
    OUT     INT32                       *pi4_width)
{
    INT32                       i4_kerning, i4_adv, i4_temp, i4_ret;
    UINT16                      ui2_i;
    UTF16_T                     w2_ch, w2_ch_prev;
    BOOL                        b_has_kerning;
    INT32                       i4_w_add_synth;

    *pi4_width = 0;
    b_has_kerning = x_fe_has_kerning(h_font);
    w2_ch_prev = (UTF16_T)0;
    for(ui2_i=0; ui2_i<ui2_num_chars; ui2_i++)
    {
        w2_ch = x_uc_w2s_get(w2s_chars, (UINT32)ui2_i);

        if(w2s_skip && _msfs_is_skip_char(w2s_skip, ui2_num_skip, w2_ch))
        {
            *pi4_width += ui2_w_skip;
            w2_ch_prev = 0;
        }
        else
        {        
            if(b_has_kerning)
            {
                i4_ret = x_fe_get_kerning(h_font, w2_ch_prev, w2_ch, &i4_kerning, &i4_temp);
                if(!(i4_ret==FER_OK || i4_ret==FER_NO_CHAR_CODE))
                {
                    INFORM_ERR(i4_ret);
                    return i4_ret;
                }
                *pi4_width += i4_kerning;
            }
    
            i4_ret = x_fe_get_character_size(h_font, w2_ch, &i4_adv, &i4_temp);
            if(!(i4_ret==FER_OK || i4_ret==FER_NO_CHAR_CODE))
            {
                INFORM_ERR(i4_ret);
                return i4_ret;
            }
            *pi4_width += i4_adv;
    
            w2_ch_prev = w2_ch;
        }
    }
    
    i4_ret = x_fe_get_add_width_by_synthesis (h_font, &i4_w_add_synth);
        
    if (i4_ret != FER_OK)
    {
        INFORM_ERR (i4_ret);
        i4_w_add_synth = 0;
    }
        
    *pi4_width += i4_w_add_synth;

    return WGLR_OK;
}

static BOOL _msfs_is_skip_char(
    IN      UTF16_T                     *w2s_skip,
    IN      UINT16                      ui2_num_skip,
    IN      UTF16_T                     w2_ch)
{
    UINT16  ui2_i;
    CHECK_VALID(w2s_skip != NULL, FALSE);
    
    for(ui2_i = 0; ui2_i < ui2_num_skip; ui2_i ++)
    {
        if(w2_ch == w2s_skip[ui2_i])
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

static INT32 _msfs_update_font_reg_info(
    IN OUT  HFONT_T                     h_font,
    IN      WGL_MSFS_FONT_REG_INFO_T    *at_font_reg_tbl,
    IN      UINT16                      ui2_num_reg_fonts)
{
    UINT16              ui2_i;
    INT32               i4_ret;
    FE_FNT_SIZE         e_size;
    CHAR                s_font_name[WGL_MAX_FONT_NAME];
    
    CATCH_FAIL(x_fe_get_size(h_font, &e_size));
    CATCH_FAIL(x_fe_get_name(h_font, s_font_name, WGL_MAX_FONT_NAME));
    
    for(ui2_i=0; ui2_i<ui2_num_reg_fonts; ui2_i++)
    {
        if(    e_size == at_font_reg_tbl[ui2_i].e_font_size
            && !x_strcmp(s_font_name, at_font_reg_tbl[ui2_i].s_font_name))
        {
            if(HR_OK != (i4_ret=x_handle_set_tag(h_font, &at_font_reg_tbl[ui2_i])))
            {
                INFORM_ERR(i4_ret);
                return i4_ret;
            }
            break;
        }
    }
    
    return MSFSR_OK;
    
}    

INT32 _msfs_get_max_char_in_box(
    IN  HFONT_T                     h_font,
    IN  INT32                       i4_msfs_font_w,
    IN  const UTF16_T*              w2s_str,
    IN  INT32                       i4_max_len,
    IN  BOOL                        b_check_length,
    IN  INT32                       i4_box_w,
    IN  const UTF16_T*              w2s_can_break,
    OUT UTF16_T**                   pw2s_next,
    OUT UINT32*                     pui4_num_ch,
    OUT INT32*                      pi4_width)
{
    INT32                       i4_len, i4_i;
    UTF16_T                     w2_ch;
    INT32                       i4_last_break_pos=-1, i4_last_break_width=0;

    *pw2s_next = NULL;
    *pui4_num_ch = 0;
    *pi4_width = 0;

    if(b_check_length)
    {
        if( (i4_len=(INT32) x_uc_w2s_strlen(w2s_str)) == 0)
        {
            return WGLR_OK;
        }
        i4_len = i4_len > i4_max_len ? i4_max_len : i4_len;
    }
    else
    {
        i4_len = i4_max_len;
    }

    for(i4_i=0; i4_i<i4_len; i4_i++)
    {
        w2_ch = x_uc_w2s_get(w2s_str, (UINT32) i4_i);

        /* count advance */
        *pi4_width += i4_msfs_font_w;
        i4_last_break_width += i4_msfs_font_w;

        /* check box */
        if(*pi4_width > i4_box_w)
        {
            break;
        }

        /* remember the last 'can_break' character position */ /*if not assign w2s_can_break characters. Always keep the last pos*/
        if( (NULL == w2s_can_break) || (w2s_can_break && x_uc_w2s_chr(w2s_can_break, w2_ch)))
        {
            i4_last_break_pos = i4_i;
            i4_last_break_width = 0;
        }
    }
    
    if(i4_i >= i4_len) /*the box can contain whole string*/
    {
        i4_last_break_pos = i4_i;
        i4_last_break_width = 0;
    }

    if(i4_last_break_pos < 0)   /* no any can-break char */
    {
        *pi4_width = 0;
        *pui4_num_ch = 0;
        *pw2s_next = NULL;
        return WGLR_OK;
    }

    *pi4_width -= i4_last_break_width;
    *pui4_num_ch = (UINT32) (i4_last_break_pos+1);
    *pui4_num_ch = (((INT32)*pui4_num_ch) > i4_len) ? (UINT32) i4_len : *pui4_num_ch;
    *pw2s_next = x_uc_w2s_get_ptr(w2s_str, *pui4_num_ch);

    return WGLR_OK;
}

