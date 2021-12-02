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
 * $RCSfile: wgl_tools.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: ad3bc45f6ab000128a19eb45adfd0030 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "wgl/x_wgl.h"
#include "unicode/x_uc_str.h"
#include "font/x_fe.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/wgl_tools.h"
#include "wgl/u_wgl_bdr_common.h"
#include "wgl/animation/u_wgl_animation.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
#if 0
static WGL_ANIM_CB_T    _t_anim_cond_cb = {NULL, NULL};
static BOOL             _b_anim_running = FALSE;
static HANDLE_T         _h_anim_widget  = NULL_HANDLE;
#endif
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static BOOL _is_skip_char(
    UTF16_T             w2_ch, 
    const UTF16_T*      w2s_skip, 
    UINT16              ui2_skiplen);

#define _IS_CHINESE_CHAR(w2s_ch) ((w2s_ch >= 0x4E00 && w2s_ch <= 0x9FA5) ? TRUE : ((w2s_ch >= 0xF900 && w2s_ch <= 0xFA2D) ? TRUE : FALSE)) 


#if 0
static INT32 _app_anim_callback(
    HANDLE_T            h_widget,
    UINT16              ui2_anim_type,
    WGL_ANIM_COND_T     e_anim_cond,
    VOID*               pv_tag);
#endif
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_GET_FOCUS and WGL_MSG_LOSE_FOCUS message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_notify_parent_focus(
    HANDLE_T                    h_widget,
    BOOL                        b_is_get)
{
    /*local varaible declaration*/
    WGL_NOTIFY_DATA_T           t_nfy_data = {0};
    HANDLE_T                    h_parent = NULL_HANDLE;
    INT32                       i4_ret;

    /*get parent*/
    i4_ret = x_wgl_get_parent(h_widget, &h_parent);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    if(h_parent)
    {
        /*prepare notify data*/
        t_nfy_data.ui4_nc_code = (b_is_get) ? WGL_NC_GL_GET_FOCUS : WGL_NC_GL_LOSE_FOCUS;

        /*send notify to its parent*/
        return x_wgl_send_notify(h_parent, t_nfy_data.ui4_nc_code, h_widget, &t_nfy_data);
    }

    return WMPR_DONE;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          WGL_MSG_KEY_DOWN and WGL_MSG_KEY_UP message handler
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_notify_parent_key(
    HANDLE_T                    h_widget,
    UINT32                      ui4_code,
    UINT32                      ui4_state)
{
    /*local varaible declaration*/
    WGL_NOTIFY_DATA_T           t_nfy_data = {0};
    HANDLE_T                    h_parent = NULL_HANDLE;
    INT32                       i4_ret;

    /*get parent*/
    i4_ret = x_wgl_get_parent(h_widget, &h_parent);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    if(h_parent)
    {
        /*prepare notify data*/
        t_nfy_data.ui4_nc_code = (WGL_KEY_STATE_UP == ui4_state) ? WGL_NC_GL_KEY_UP : WGL_NC_GL_KEY_DOWN;
        t_nfy_data.u.t_key.ui4_key_state = ui4_state;
        t_nfy_data.u.t_key.ui4_key_code = ui4_code;

        /*send notify to its parent*/
        return x_wgl_send_notify(h_parent, t_nfy_data.ui4_nc_code, h_widget, &t_nfy_data);
    }

    return WGLR_KEY_MSG_NOT_PROCESSED;
}

/*-----------------------------------------------------------------------------
 * Name:  wgl_get_max_char_in_box_ex
 *
 * Description: This API evaluate how many characters could be shown in a box.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          w2s_str             The string to be evaluated.
 *          i4_max_len          Maximum length of string to check.
 *          i4_box_w            The widht of required bounding box
 *          w2s_can_break       Array of 'Can Break' characters
 *          w2s_skip            Array of 'Should-Be-Skipped' characters, can be NULL
 *          ui2_num_skip        Number of 'Should-Be-Skipped' characters in w2s_skip
 *          ui2_w_skip          Pixel size of 'Should-Be-Skipped' characters
 *
 * Outputs: pw2s_next           Pointer of the next character outside box
 *          pui4_num_ch         How many characters in the box
 *          pi4_width           References the width of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The Font Engine is not initialized yet.
 *          WGLR_INTERNAL_ERROR The argument h_fnt is an invalid font handle.
 *          WGLR_INV_ARG        One or more characters in the string is invalid
 *                              or not in font.
 ----------------------------------------------------------------------------*/
INT32 wgl_get_max_char_in_box_ex(
    HFONT_T                     h_fnt,
    const UTF16_T*              w2s_str,
    INT32                       i4_max_len,
    BOOL                        b_check_length,
    INT32                       i4_box_w,
    const UTF16_T*              w2s_can_break,
    const UTF16_T*              w2s_skip,
    UINT16                      ui2_num_skip,
    UINT16                      ui2_w_skip,
    BOOL                        b_skip_as_break,
    UTF16_T**                   pw2s_next,
    UINT32*                     pui4_num_ch,
    INT32*                      pi4_width)
{
    INT32                       i4_ret, i4_len, i4_i;
    UTF16_T                     w2_ch_prv=(UTF16_T)0, w2_ch=(UTF16_T)0, w2_skip_prv=(UTF16_T)0;
    INT32                       i4_adv_x=0, i4_adv_y=0, i4_kern_x=0, i4_kern_y=0;
    BOOL                        b_has_kern;
    INT32                       i4_last_break_pos=-1, i4_last_break_width=0, i4_last_char_width=0, i4_skip_width = 0;
    INT32                       i4_2rd_last_break_pos=-1, i4_2rd_last_break_width=0, i4_2rd_skip_width = 0;
    BOOL                        b_is_skip = FALSE;
    INT32                       i4_w_add_synth;
    
    i4_ret = x_fe_get_add_width_by_synthesis (h_fnt, &i4_w_add_synth);
        
    if (i4_ret != FER_OK)
    {
        i4_w_add_synth = 0;
    }
        
    i4_box_w -= i4_w_add_synth;
    i4_box_w = MAX (0, i4_box_w);
    *pi4_width = 0;
    
    if(b_check_length)
    {
        if( (i4_len=(INT32)(x_uc_w2s_strlen(w2s_str))) == 0)
        {
            return WGLR_OK;
        }
        i4_len = i4_len > i4_max_len ? i4_max_len : i4_len;
    }
    else
    {
        i4_len = i4_max_len;
    }
    
    if(w2s_skip==NULL && ui2_num_skip != 0)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = FER_OK;
    b_has_kern = x_fe_has_kerning(h_fnt);
    for(i4_i=0; i4_i<i4_len; i4_i++)
    {
        w2_ch = x_uc_w2s_get(w2s_str, (UINT32)i4_i);

        if(w2s_skip && _is_skip_char(w2_ch, w2s_skip, ui2_num_skip))
        {
            *pi4_width          += (INT32)ui2_w_skip;
            i4_last_break_width += (INT32)ui2_w_skip;
            i4_last_char_width   = (INT32)ui2_w_skip;
            w2_ch_prv            = 0;
        }
        #if 0
        else if (!x_fe_has_fixed_sizes(h_fnt) && !_IS_CHINESE_CHAR(w2_ch))
        {
            
            FE_GLYPH_T * pt_glyph;
            i4_kern_x = 0;
            i4_kern_y = 0;
            if(b_has_kern)
            {
                i4_ret = x_fe_get_kerning( h_fnt,
                                           (CHAR_CODE)w2_ch_prv,
                                           (CHAR_CODE)w2_ch,
                                           &i4_kern_x,
                                           &i4_kern_y);
                
            }
    
            /* count advance */
            pt_glyph = (FE_GLYPH_T*)WGL_MEM_ALLOC(sizeof(FE_GLYPH_T));
            if (pt_glyph == NULL)
            {
                return FER_NO_MEMORY;
            }
            i4_ret = x_fe_load_char(h_fnt, w2_ch, pt_glyph);
    
            if (i4_ret == FER_OK)
            {
                if (((pt_glyph->t_bmp.i4_width == 0) || (pt_glyph->t_bmp.i4_height == 0)))
                {
                    *pi4_width += (pt_glyph->t_advance.i4_x >> 6);
                
                    i4_last_break_width += (pt_glyph->t_advance.i4_x >> 6);
                }
                else
                {
                    if (pt_glyph->i4_left < 0)
                    {
                        i4_kern_x += pt_glyph->i4_left;
                    }
                    else if (pt_glyph->i4_left + pt_glyph->t_bmp.i4_width != (pt_glyph->t_advance.i4_x >> 6))
                    {
                        i4_kern_x = pt_glyph->i4_left + pt_glyph->t_bmp.i4_width - (pt_glyph->t_advance.i4_x >> 6);
                    }
                    
                    if (i4_i == 0 && i4_kern_x < 0)
                    {
                        *pi4_width = pt_glyph->t_bmp.i4_width;
                        i4_last_break_width = pt_glyph->t_bmp.i4_width;
                    }
                    else
                    {
                        *pi4_width += i4_kern_x + (pt_glyph->t_advance.i4_x >> 6) ;
                        i4_last_break_width += i4_kern_x + (pt_glyph->t_advance.i4_x >> 6);
                    }
                }
            }

            WGL_MEM_FREE(pt_glyph);
            /* to be left char */
            w2_ch_prv = w2_ch;

        }
        #endif
        else
        {
            if(b_has_kern)
            {
                i4_ret = x_fe_get_kerning( h_fnt,
                                           (CHAR_CODE)w2_ch_prv,
                                           (CHAR_CODE)w2_ch,
                                           &i4_kern_x,
                                           &i4_kern_y);
                if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
                {
                    *pi4_width += i4_kern_x;
                    i4_last_break_width += i4_kern_x;
                }
            }
    
            /* count advance */
            i4_ret = x_fe_get_character_size(h_fnt, (CHAR_CODE)w2_ch, &i4_adv_x, &i4_adv_y);
            if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
            {
                *pi4_width += i4_adv_x;
                i4_last_break_width += i4_adv_x;
                i4_last_char_width = i4_adv_x;
            }
            
            /* to be left char */
            w2_ch_prv = w2_ch;
        }
        
        /* check box */
        if(*pi4_width > i4_box_w)
        {
            break;
        }

        /* remember the last 'can_break' character position 
        *//*if not assign w2s_can_break characters. Always keep the last pos*/
        if(((NULL == w2s_can_break) || (w2s_can_break && x_uc_w2s_chr(w2s_can_break, w2_ch)))
            && (!(i4_i == 0 && w2_ch == 0x20)) &&  !_IS_CHINESE_CHAR(w2_ch))
        {
            if(FALSE == b_skip_as_break)
            {
                /*the previous character of this break character isn't a skip character*/
                if(NULL == x_uc_w2s_chr(w2s_skip, w2_skip_prv))
                {
                    i4_2rd_last_break_pos = i4_last_break_pos;
                    i4_2rd_last_break_width = i4_last_break_width;

                    i4_2rd_skip_width = i4_skip_width;
                }
                
                if(w2s_skip && x_uc_w2s_chr(w2s_skip, w2_ch))
                {
                    b_is_skip = TRUE;

                    /*get this skip character's kerning*/
                    if(b_has_kern)
                    {
                        i4_ret = x_fe_get_kerning(h_fnt,
                                                 (CHAR_CODE)w2_skip_prv,
                                                 (CHAR_CODE)w2_ch,
                                                 &i4_kern_x,
                                                 &i4_kern_y);
                        i4_skip_width = (i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret) ? i4_kern_x : 0;
                    }
                    else
                    {
                        i4_skip_width = 0;
                    }

                    /*get this skip character's width*/
                    i4_ret = x_fe_get_character_size(h_fnt, (CHAR_CODE)w2_ch,&i4_adv_x, &i4_adv_y);
                    i4_skip_width = (i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret) ? (i4_skip_width + i4_adv_x) : 0;
                }
                else
                {
                    i4_skip_width = 0;
                    b_is_skip = FALSE;
                }
            }

            i4_last_break_pos = i4_i;
            i4_last_break_width = 0;
        }

        w2_skip_prv = w2_ch;
    }
    
    if(i4_i >= i4_len) /*the box can contain whole string*/
    {
        i4_last_break_pos = i4_i;
        i4_last_break_width = 0;
    }
    else if(i4_last_break_width >= *pi4_width) /*no break character in this scan*/
    {
        i4_last_break_width = i4_last_char_width;
        i4_last_break_pos = i4_i - 1;
    }

    if(i4_last_break_pos < 0)   /* no any can-break char */
    {
        *pi4_width = 0;
        *pui4_num_ch = 0;
        *pw2s_next = NULL;
        return WGLR_OK;
    }

    *pi4_width -= i4_last_break_width;
    *pui4_num_ch = (UINT32)(i4_last_break_pos+1);
    *pui4_num_ch = (*pui4_num_ch > (UINT32)i4_len) ? (UINT32)i4_len : *pui4_num_ch;
    

    if(b_is_skip)  /*last break is a skip character*/
    {
        if((*pi4_width + i4_skip_width) > i4_box_w) /*skip character is too big*/
        {
            if(i4_2rd_last_break_pos < 0)   /* no any can-break char */
            {
                *pi4_width = 0;
                *pui4_num_ch = 0;
                *pw2s_next = NULL;
                return WGLR_OK;
            }
            
            *pi4_width -= i4_2rd_last_break_width;
            *pui4_num_ch = (UINT32)(i4_2rd_last_break_pos+1);
            *pui4_num_ch = (*pui4_num_ch > (UINT32)i4_len) ? (UINT32)i4_len : *pui4_num_ch;

            *pi4_width += i4_2rd_skip_width;
        }
        else
        {
            *pi4_width += i4_skip_width;
        }
    }

    *pi4_width += i4_w_add_synth;

    *pw2s_next = x_uc_w2s_get_ptr(w2s_str, *pui4_num_ch);

    switch (i4_ret)
    {
    case FER_OK:
    case FER_NO_CHAR_CODE:
        i4_ret = WGLR_OK;
        break;

    case FER_INV_HANDLE:
        i4_ret = WGLR_INV_HANDLE;
        break;

    case FER_INV_ARG:
        i4_ret = WGLR_INV_ARG;
        break;

    default:
        i4_ret = WGLR_INTERNAL_ERROR;
        break;
    }
    
    return i4_ret;
}

INT32 wgl_get_max_char_in_box_cc(
    HFONT_T                     h_fnt,
    const UTF16_T*              w2s_str,
    INT32                       i4_max_len,
    BOOL                        b_check_length,
    INT32                       i4_box_w,
    const UTF16_T*              w2s_can_break,
    const UTF16_T*              w2s_skip,
    UINT16                      ui2_num_skip,
    UINT16                      ui2_w_skip,
    BOOL                        b_skip_as_break,
    UTF16_T**                   pw2s_next,
    UINT32*                     pui4_num_ch,
    INT32*                      pi4_width)
{
    INT32                       i4_ret, i4_len, i4_i;
    UTF16_T                     w2_ch_prv=(UTF16_T)0, w2_ch=(UTF16_T)0, w2_skip_prv=(UTF16_T)0;
    INT32                       i4_adv_x=0, i4_adv_y=0, i4_kern_x=0, i4_kern_y=0;
    BOOL                        b_has_kern;
    INT32                       i4_last_break_pos=-1, i4_last_break_width=0, i4_last_char_width=0, i4_skip_width = 0;
    INT32                       i4_2rd_last_break_pos=-1, i4_2rd_last_break_width=0, i4_2rd_skip_width = 0;
    BOOL                        b_is_skip = FALSE;
    INT32                       i4_w_add_synth;
    
    i4_ret = x_fe_get_add_width_by_synthesis (h_fnt, &i4_w_add_synth);
        
    if (i4_ret != FER_OK)
    {
        i4_w_add_synth = 0;
    }
        
    i4_box_w -= i4_w_add_synth;
    i4_box_w = MAX (0, i4_box_w);
    *pi4_width = 0;
    
    if(b_check_length)
    {
        if( (i4_len=(INT32)(x_uc_w2s_strlen(w2s_str))) == 0)
        {
            return WGLR_OK;
        }
        i4_len = i4_len > i4_max_len ? i4_max_len : i4_len;
    }
    else
    {
        i4_len = i4_max_len;
    }
    
    if(w2s_skip==NULL && ui2_num_skip != 0)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = FER_OK;
    b_has_kern = x_fe_has_kerning(h_fnt);
    for(i4_i=0; i4_i<i4_len; i4_i++)
    {
        w2_ch = x_uc_w2s_get(w2s_str, (UINT32)i4_i);

        if(w2s_skip && _is_skip_char(w2_ch, w2s_skip, ui2_num_skip))
        {
            *pi4_width          += (INT32)ui2_w_skip;
            i4_last_break_width += (INT32)ui2_w_skip;
            i4_last_char_width   = (INT32)ui2_w_skip;
            w2_ch_prv            = 0;
        }
        else
        {
            if(b_has_kern)
            {
                i4_ret = x_fe_get_kerning( h_fnt,
                                           (CHAR_CODE)w2_ch_prv,
                                           (CHAR_CODE)w2_ch,
                                           &i4_kern_x,
                                           &i4_kern_y);
                if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
                {
                    *pi4_width += i4_kern_x;
                    i4_last_break_width += i4_kern_x;
                }
            }
    
            /* count advance */
            i4_ret = x_fe_get_glyph_advance(h_fnt, (CHAR_CODE)w2_ch, &i4_adv_x, &i4_adv_y);
            if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
            {
                *pi4_width += i4_adv_x;
                i4_last_break_width += i4_adv_x;
                i4_last_char_width = i4_adv_x;
            }
            
            /* to be left char */
            w2_ch_prv = w2_ch;

        }
        
        /* check box */
        if(*pi4_width > i4_box_w)
        {
            break;
        }

        /* remember the last 'can_break' character position 
        *//*if not assign w2s_can_break characters. Always keep the last pos*/
        if((NULL == w2s_can_break) || (w2s_can_break && x_uc_w2s_chr(w2s_can_break, w2_ch)))
        {
            if(FALSE == b_skip_as_break)
            {
                /*the previous character of this break character isn't a skip character*/
                if(NULL == x_uc_w2s_chr(w2s_skip, w2_skip_prv))
                {
                    i4_2rd_last_break_pos = i4_last_break_pos;
                    i4_2rd_last_break_width = i4_last_break_width;

                    i4_2rd_skip_width = i4_skip_width;
                }
                
                if(w2s_skip && x_uc_w2s_chr(w2s_skip, w2_ch))
                {
                    b_is_skip = TRUE;

                    /*get this skip character's kerning*/
                    if(b_has_kern)
                    {
                        i4_ret = x_fe_get_kerning(h_fnt,
                                                 (CHAR_CODE)w2_skip_prv,
                                                 (CHAR_CODE)w2_ch,
                                                 &i4_kern_x,
                                                 &i4_kern_y);
                        i4_skip_width = (i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret) ? i4_kern_x : 0;
                    }
                    else
                    {
                        i4_skip_width = 0;
                    }

                    /*get this skip character's width*/
                    i4_ret = x_fe_get_glyph_advance(h_fnt, (CHAR_CODE)w2_ch,&i4_adv_x, &i4_adv_y);
                    i4_skip_width = (i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret) ? (i4_skip_width + i4_adv_x) : 0;
                }
                else
                {
                    i4_skip_width = 0;
                    b_is_skip = FALSE;
                }
            }

            i4_last_break_pos = i4_i;
            i4_last_break_width = 0;
        }

        w2_skip_prv = w2_ch;
    }
    
    if(i4_i >= i4_len) /*the box can contain whole string*/
    {
        i4_last_break_pos = i4_i;
        i4_last_break_width = 0;
    }
    else if(i4_last_break_width >= *pi4_width) /*no break character in this scan*/
    {
        i4_last_break_width = i4_last_char_width;
        i4_last_break_pos = i4_i - 1;
    }

    if(i4_last_break_pos < 0)   /* no any can-break char */
    {
        *pi4_width = 0;
        *pui4_num_ch = 0;
        *pw2s_next = NULL;
        return WGLR_OK;
    }

    *pi4_width -= i4_last_break_width;
    *pui4_num_ch = (UINT32)(i4_last_break_pos+1);
    *pui4_num_ch = (*pui4_num_ch > (UINT32)i4_len) ? (UINT32)i4_len : *pui4_num_ch;
    

    if(b_is_skip)  /*last break is a skip character*/
    {
        if((*pi4_width + i4_skip_width) > i4_box_w) /*skip character is too big*/
        {
            if(i4_2rd_last_break_pos < 0)   /* no any can-break char */
            {
                *pi4_width = 0;
                *pui4_num_ch = 0;
                *pw2s_next = NULL;
                return WGLR_OK;
            }
            
            *pi4_width -= i4_2rd_last_break_width;
            *pui4_num_ch = (UINT32)(i4_2rd_last_break_pos+1);
            *pui4_num_ch = (*pui4_num_ch > (UINT32)i4_len) ? (UINT32)i4_len : *pui4_num_ch;

            *pi4_width += i4_2rd_skip_width;
        }
        else
        {
            *pi4_width += i4_skip_width;
        }
    }

    *pi4_width += i4_w_add_synth;

    *pw2s_next = x_uc_w2s_get_ptr(w2s_str, *pui4_num_ch);

    switch (i4_ret)
    {
    case FER_OK:
    case FER_NO_CHAR_CODE:
        i4_ret = WGLR_OK;
        break;

    case FER_INV_HANDLE:
        i4_ret = WGLR_INV_HANDLE;
        break;

    case FER_INV_ARG:
        i4_ret = WGLR_INV_ARG;
        break;

    default:
        i4_ret = WGLR_INTERNAL_ERROR;
        break;
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  wgl_get_max_char_in_box_for_arab
 *
 * Description: This API evaluate how many characters could be shown in a box.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          w2s_str             The string to be evaluated.
 *          i4_max_len          Maximum length of string to check.
 *          i4_box_w            The widht of required bounding box
 *          w2s_can_break       Array of 'Can Break' characters
 *          w2s_skip            Array of 'Should-Be-Skipped' characters, can be NULL
 *          ui2_num_skip        Number of 'Should-Be-Skipped' characters in w2s_skip
 *          ui2_w_skip          Pixel size of 'Should-Be-Skipped' characters
 *
 * Outputs: pw2s_next           Pointer of the next character outside box
 *          pui4_num_ch         How many characters in the box
 *          pi2_width           References the width of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The Font Engine is not initialized yet.
 *          WGLR_INTERNAL_ERROR The argument h_fnt is an invalid font handle.
 *          WGLR_INV_ARG        One or more characters in the string is invalid
 *                              or not in font.
 ----------------------------------------------------------------------------*/
INT32 wgl_get_max_char_in_box_for_arab(
    INT32                       i4_string_len,
    HFONT_T                     h_fnt,
    const UTF16_T*              w2s_string_right,
    INT32                       i4_max_len,
    BOOL                        b_check_length,
    INT32                       i4_box_w,
    const UTF16_T*              w2s_can_break,
    const UTF16_T*              w2s_skip,
    UINT16                      ui2_num_skip,
    UINT16                      ui2_w_skip,
    BOOL                        b_skip_as_break,
    UTF16_T**                   pw2s_next,
    UINT32*                     pui4_num_ch,
    INT32*                      pi4_width)
{
    INT32                       i4_ret, i4_len, i4_i;
    UTF16_T                     w2_ch_right=(UTF16_T)0, w2_ch=(UTF16_T)0, w2_skip_right=(UTF16_T)0;
    INT32                       i4_adv_x=0, i4_adv_y=0, i4_kern_x=0, i4_kern_y=0;
    BOOL                        b_has_kern;
    INT32                       i4_last_break_pos=-1, i4_last_break_width=0, i4_last_char_width=0, i4_skip_width = 0;
    INT32                       i4_2rd_last_break_pos=-1, i4_2rd_last_break_width=0, i4_2rd_skip_width = 0;
    BOOL                        b_is_skip = FALSE;
    INT32                       i4_w_add_synth;
    
    i4_ret = x_fe_get_add_width_by_synthesis (h_fnt, &i4_w_add_synth);
        
    if (i4_ret != FER_OK)
    {
        i4_w_add_synth = 0;
    }
        
    i4_box_w -= i4_w_add_synth;
    i4_box_w = MAX (0, i4_box_w);
       
    i4_len = i4_max_len;    
    *pi4_width = 0;
    
    if(w2s_skip == NULL && ui2_num_skip != 0)
    {
        return WGLR_INV_ARG;
    }

    i4_ret = FER_OK;
    b_has_kern = x_fe_has_kerning(h_fnt);
    for(i4_i=0; i4_i < i4_len; i4_i++)
    {
        w2_ch = *(w2s_string_right - i4_i);

        if(w2s_skip && _is_skip_char(w2_ch, w2s_skip, ui2_num_skip))
        {
            *pi4_width          += (INT32)ui2_w_skip;
            i4_last_break_width += (INT32)ui2_w_skip;
            i4_last_char_width   = (INT32)ui2_w_skip;
            w2_ch_right            = 0;
        }
        else
        {
            if(b_has_kern)
            {
                i4_ret = x_fe_get_kerning( h_fnt,
                                           (CHAR_CODE)w2_ch,
                                           (CHAR_CODE)w2_ch_right,                                           
                                           &i4_kern_x,
                                           &i4_kern_y);
                if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
                {
                    *pi4_width += i4_kern_x;
                    i4_last_break_width += i4_kern_x;
                }
            }
    
            /* count advance */
            i4_ret = x_fe_get_character_size(h_fnt, (CHAR_CODE)w2_ch, &i4_adv_x, &i4_adv_y);
            if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
            {
                *pi4_width += i4_adv_x;
                i4_last_break_width += i4_adv_x;
                i4_last_char_width = i4_adv_x;
            }
            
            /* to be right char */
            w2_ch_right = w2_ch;

        }
        
        /* check box */
        if(*pi4_width > i4_box_w)
        {
            break;
        }

        /* remember the last 'can_break' character position */
        /*if not assign w2s_can_break characters. Always keep the last pos*/
        if((NULL == w2s_can_break) || (w2s_can_break && x_uc_w2s_chr(w2s_can_break, w2_ch)))
        {
            if(FALSE == b_skip_as_break)
            {
                /*the right character of this break character isn't a skip character*/
                if(NULL == x_uc_w2s_chr(w2s_skip, w2_skip_right))
                {
                    i4_2rd_last_break_pos = i4_last_break_pos;
                    i4_2rd_last_break_width = i4_last_break_width;

                    i4_2rd_skip_width = i4_skip_width;
                }
                
                if(w2s_skip && x_uc_w2s_chr(w2s_skip, w2_ch))
                {
                    b_is_skip = TRUE;

                    /*get this skip character's kerning*/
                    if(b_has_kern)
                    {
                        i4_ret = x_fe_get_kerning(h_fnt,
                                                 (CHAR_CODE)w2_ch,
                                                 (CHAR_CODE)w2_skip_right,                                                
                                                 &i4_kern_x,
                                                 &i4_kern_y);
                        i4_skip_width = (i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret) ? i4_kern_x : 0;
                    }
                    else
                    {
                        i4_skip_width = 0;
                    }

                    /*get this skip character's width*/
                    i4_ret = x_fe_get_character_size(h_fnt, (CHAR_CODE)w2_ch,&i4_adv_x, &i4_adv_y);
                    i4_skip_width = (i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret) ? (i4_skip_width + i4_adv_x) : 0;
                }
                else
                {
                    i4_skip_width = 0;
                    b_is_skip = FALSE;
                }
            }

            i4_last_break_pos = i4_i;
            i4_last_break_width = 0;
        }

        w2_skip_right = w2_ch;
    }
    
    if(i4_i >= i4_len) /*the box can contain whole string*/
    {
        i4_last_break_pos = i4_i;
        i4_last_break_width = 0;
    }
    else if(i4_last_break_width >= *pi4_width) /*no break character in this scan*/
    {
        i4_last_break_width = i4_last_char_width;
        i4_last_break_pos = i4_i - 1;
    }

    if(i4_last_break_pos < 0)   /* no any can-break char */
    {
        *pi4_width = 0;
        *pui4_num_ch = 0;
        *pw2s_next = NULL;
        return WGLR_OK;
    }

    *pi4_width -= i4_last_break_width;
    *pui4_num_ch = (UINT32)(i4_last_break_pos+1);
    *pui4_num_ch = (*pui4_num_ch > (UINT32)i4_len) ? (UINT32)i4_len : *pui4_num_ch;
    

    if(b_is_skip && i4_i < i4_len)  /*last break is a skip character*/
    {
        if((*pi4_width + i4_skip_width) > i4_box_w) /*skip character is too big*/
        {
            if(i4_2rd_last_break_pos < 0)   /* no any can-break char */
            {
                *pi4_width = 0;
                *pui4_num_ch = 0;
                *pw2s_next = NULL;
                return WGLR_OK;
            }
            
            *pi4_width -= i4_2rd_last_break_width;
            *pui4_num_ch = (UINT32)(i4_2rd_last_break_pos+1);
            *pui4_num_ch = (*pui4_num_ch > (UINT32)i4_len) ? (UINT32)i4_len : *pui4_num_ch;

            *pi4_width += i4_2rd_skip_width;
        }
        else
        {           
            *pi4_width += i4_skip_width;
        }
    }
    if ((UINT32)i4_string_len == *pui4_num_ch){
        *pw2s_next =NULL;
    }
    else
    {
        *pw2s_next =(UTF16_T*)(w2s_string_right -*pui4_num_ch);
    }

    *pi4_width += i4_w_add_synth;
    
    switch (i4_ret)
    {
    case FER_OK:
    case FER_NO_CHAR_CODE:
        i4_ret = WGLR_OK;
        break;

    case FER_INV_HANDLE:
        i4_ret = WGLR_INV_HANDLE;
        break;

    case FER_INV_ARG:
        i4_ret = WGLR_INV_ARG;
        break;

    default:
        i4_ret = WGLR_INTERNAL_ERROR;
        break;
    }
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name:  wgl_get_max_char_in_box
 *
 * Description: This API evaluate how many characters could be shown in a box.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          w2s_str             The string to be evaluated.
 *          i4_max_len          Maximum length of string to check.
 *          i4_box_w            The widht of required bounding box
 *          w2s_can_break       Array of 'Can Break' characters
 *
 * Outputs: pw2s_next           Pointer of the next character outside box
 *          pui4_num_ch         How many characters in the box
 *          pi2_width           References the width of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The Font Engine is not initialized yet.
 *          WGLR_INTERNAL_ERROR The argument h_fnt is an invalid font handle.
 *          WGLR_INV_ARG        One or more characters in the string is invalid
 *                              or not in font.
 ----------------------------------------------------------------------------*/
INT32 wgl_get_max_char_in_box(
    HFONT_T                     h_fnt,
    const UTF16_T*              w2s_str,
    INT32                       i4_max_len,
    BOOL                        b_check_length,
    INT32                       i4_box_w,
    const UTF16_T*              w2s_can_break,
    UTF16_T**                   pw2s_next,
    UINT32*                     pui4_num_ch,
    INT32*                      pi4_width)
{
    return wgl_get_max_char_in_box_ex(
        h_fnt,
        w2s_str,
        i4_max_len,
        b_check_length,
        i4_box_w,
        w2s_can_break,
        NULL,
        0,
        0,
        TRUE,
        pw2s_next,
        pui4_num_ch,
        pi4_width);
}
        
/*-----------------------------------------------------------------------------
 * Name:  wgl_rget_max_char_in_box
 *
 * Description: This API evaluate how many characters could be shown in a box.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          w2s_str             The string to be evaluated.
 *          i4_max_len          Maximum length of string to check.
 *          i4_box_w            The widht of required bounding box
 *          w2s_can_break       Array of 'Can Break' characters
 *
 * Outputs: pw2s_next           Pointer of the next character outside box
 *          pui4_num_ch         How many characters in the box
 *          pi2_width           References the width of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: WGLR_OK             The routine was successful.
 *          WGLR_INV_HANDLE     The Font Engine is not initialized yet.
 *          WGLR_INTERNAL_ERROR The argument h_fnt is an invalid font handle.
 *          WGLR_INV_ARG        One or more characters in the string is invalid
 *                              or not in font.
 ----------------------------------------------------------------------------*/
INT32 wgl_rget_max_char_in_box(
    HFONT_T                     h_fnt,
    const UTF16_T*              w2s_str,
    INT32                       i4_max_len,
    BOOL                        b_check_length,
    INT32                       i4_box_w,
    const UTF16_T*              w2s_can_break,
    UTF16_T**                   pw2s_next,
    UINT32*                     pui4_num_ch,
    INT32*                      pi4_width)
{
    INT32                       i4_ret, i4_len, i4_i;
    UTF16_T                     w2_ch_prv=(UTF16_T)0, w2_ch=(UTF16_T)0;
    INT32                       i4_adv_x=0, i4_adv_y=0, i4_kern_x=0, i4_kern_y=0;
    BOOL                        b_has_kern;
    INT32                       i4_last_break_pos=-1, i4_last_break_width=0, i4_last_char_width=0;

    *pi4_width = 0;

    if(b_check_length)
    {
        if( (i4_len=(INT32)(x_uc_w2s_strlen(w2s_str))) == 0)
        {
            return WGLR_OK;
        }
        i4_len = i4_len > i4_max_len ? i4_max_len : i4_len;
    }
    else
    {
        i4_len = i4_max_len;
    }

    i4_ret = FER_OK;
    b_has_kern = x_fe_has_kerning(h_fnt);
    for(i4_i = i4_len-1; i4_i >= 0; i4_i--)
    {
        w2_ch = x_uc_w2s_get(w2s_str, (UINT32)i4_i);

        /* adjust kern */
        if(b_has_kern)
        {
            i4_ret = x_fe_get_kerning( h_fnt,
                                       (CHAR_CODE)w2_ch,
                                       (CHAR_CODE)w2_ch_prv,
                                       &i4_kern_x,
                                       &i4_kern_y);
            if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
            {
                *pi4_width += i4_kern_x;
                i4_last_break_width += i4_kern_x;
            }
        }

        /* count advance */
        i4_ret = x_fe_get_character_size(h_fnt, (CHAR_CODE)w2_ch,&i4_adv_x, &i4_adv_y);
        if(i4_ret == FER_OK || FER_NO_CHAR_CODE == i4_ret)
        {
            *pi4_width += i4_adv_x;
            i4_last_break_width += i4_adv_x;
            i4_last_char_width = i4_adv_x;
        }

        /* check box */
        if(*pi4_width > i4_box_w)
        {
            break;
        }

        /* remember the last 'can_break' character position */
        if( (NULL == w2s_can_break) || (w2s_can_break && x_uc_w2s_chr(w2s_can_break, w2_ch)))
        {
            i4_last_break_pos = i4_i;
            i4_last_break_width = 0;
        }

        /* to be left char */
        w2_ch_prv = w2_ch;
    }

    if(i4_last_break_width >= *pi4_width) /*no break character in this scan*/
    {
        i4_last_break_width = i4_last_char_width;
        i4_last_break_pos = i4_i + 1;
    }

    if(i4_last_break_pos < 0)   /* no any can-break char */
    {
        *pi4_width = 0;
        *pui4_num_ch = 0;
        *pw2s_next = NULL;
        return WGLR_OK;
    }

    *pi4_width -= i4_last_break_width;
    *pui4_num_ch = (UINT32)(i4_len - i4_last_break_pos);
    if(i4_last_break_pos > 0)
    {
        *pw2s_next = x_uc_w2s_get_ptr(w2s_str, (UINT32)(i4_last_break_pos - 1));
    }
    else
    {
        *pw2s_next = NULL;
    }

    switch (i4_ret)
    {
    case FER_OK:
    case FER_NO_CHAR_CODE:
        i4_ret = WGLR_OK;
        break;

    case FER_INV_HANDLE:
        i4_ret = WGLR_INV_HANDLE;
        break;

    case FER_INV_ARG:
        i4_ret = WGLR_INV_ARG;
        break;

    default:
        i4_ret = WGLR_INTERNAL_ERROR;
        break;
    }
    
    return i4_ret;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_get_break_line_pos(
    const WGL_BREAK_LINE_INFO_T*    pt_Info,
    UINT16*                         pui2_first_token_pos,
    WGL_BREAK_LINE_RESULT_T*        pt_result)
{
    /*local variable declaration*/
    UTF16_T*                        w2s_next_start = NULL;
    UINT32                          ui4_chars_in_line = 0;
    INT32                           i4_break_width = 0;
    INT32                           i4_check_length;
    INT32                           i4_ret;
    UINT32                          ui4_temp;

    BOOL                            b_token = FALSE;
    /*check parameter*/
    if(NULL == pt_Info || NULL == pt_result)
    {
        return WGLR_INV_ARG;
    }

    /*get the position of '\n'*/
    if(NULL == pui2_first_token_pos || 0 == *pui2_first_token_pos)
    {
        if(NULL == x_uc_w2s_chr_ex(pt_Info->w2s_string, pt_Info->w2c_force_token, &ui4_temp)) 
        {
            i4_check_length = pt_Info->i4_string_length;
        }
        else
        {
            WGL_ASSERT(ui4_temp <= 0x0000FFFF);
            i4_check_length = (INT32)ui4_temp; /*ui4_temp is Zero-based*/
            b_token = TRUE;
        }
    }
    else
    {
        i4_check_length = (INT32)(*pui2_first_token_pos); /*pui2_first_token_pos is Zero-based*/
        b_token = TRUE;
    }
    
    if(i4_check_length > 0)
    {
        /*calculate the character number*/
#if 0
        i4_ret = wgl_get_max_char_in_box(pt_Info->h_font, 
                                         pt_Info->w2s_string, 
                                         i4_check_length,
                                         TRUE,
                                         pt_Info->i4_box_width, 
                                         pt_Info->w2s_can_break, 
                                         &w2s_next_start, 
                                         &ui4_chars_in_line,
                                         &i4_break_width);

        
#else
        i4_ret = wgl_get_max_char_in_box_ex(pt_Info->h_font,
                                            pt_Info->w2s_string,
                                            i4_check_length,
                                            TRUE,
                                            pt_Info->i4_box_width,
                                            pt_Info->w2s_can_break,
                                            pt_Info->w2s_skip,
                                            pt_Info->ui2_skip_count,
                                            pt_Info->ui2_skip_width,
                                            pt_Info->b_skip_as_break,
                                            &w2s_next_start,
                                            &ui4_chars_in_line,
                                            &i4_break_width);
#endif

        if(i4_ret != WGLR_OK)
        {
            return i4_ret;
        }
        
        /*check the boundary*/
        WGL_ASSERT(i4_break_width <= 0x0000FFFF && i4_break_width >= 0);
        WGL_ASSERT(ui4_chars_in_line <= 0x0000FFFF);
    }
    else /*because the check length is 0, the next start is the same to current*/
    {
        w2s_next_start = pt_Info->w2s_string;
    }
    

    pt_result->ui2_char_number = (UINT16)ui4_chars_in_line;
    pt_result->ui2_width = (UINT16)i4_break_width;

    if(i4_check_length > (INT32)(pt_result->ui2_char_number))
    {
        pt_result->w2s_next_start = w2s_next_start;
    }
    else /*i4_check_length == pt_result->ui2_char_number*/
    {
        /*check if the next is force-break character*/
        pt_result->w2s_next_start = (i4_check_length == pt_Info->i4_string_length) ? 
        NULL : x_uc_w2s_get_ptr(w2s_next_start, 1);

        /*if there's '\n' and the break is caused by this '\n', this '\n' should be included.*/
        pt_result->ui2_char_number += (b_token) ? 1 : 0;
    }

    if(pui2_first_token_pos)
    {
        WGL_ASSERT(i4_check_length <= 0x0000FFFF && i4_check_length >= 0);
        *pui2_first_token_pos = (UINT16)i4_check_length;
    }

    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 *          
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_get_break_line_pos_for_arab(
    UTF16_T*                        w2s_string_right,
    const WGL_BREAK_LINE_INFO_T*    pt_Info,
    UINT16*                         pui2_first_token_pos,
    WGL_BREAK_LINE_RESULT_T*        pt_result)
{
    /*local variable declaration*/
    UTF16_T*                        w2s_next_start = NULL;
    UINT32                          ui4_chars_in_line = 0;
    INT32                           i4_break_width = 0;
    INT32                           i4_check_length;
    INT32                           i4_ret;
    UINT32                          ui4_temp;

    BOOL                            b_token = FALSE;
    /*check parameter*/
    if(NULL == pt_Info || NULL == pt_result)
    {
        return WGLR_INV_ARG;
    }

    /*get the position of '\n' from string end*/
    if(NULL == pui2_first_token_pos || 0 == *pui2_first_token_pos)
    {
        if(NULL == x_uc_w2s_rchr_ex(w2s_string_right,(UINT32)(pt_Info->i4_string_length), pt_Info->w2c_force_token, &ui4_temp)) 
        {
            i4_check_length = pt_Info->i4_string_length;
        }
        else
        {
            WGL_ASSERT(ui4_temp <= 0x0000FFFF);
            i4_check_length = (INT32)ui4_temp; /*ui4_temp is Zero-based*/
            b_token = TRUE;
        }
    }
    else
    {
        i4_check_length = (INT32)(*pui2_first_token_pos); /*pui2_first_token_pos is Zero-based*/
        b_token = TRUE;
    }
    
    if(i4_check_length > 0)
    {
        /*calculate the character number*/

        i4_ret = wgl_get_max_char_in_box_for_arab(
                                            pt_Info->i4_string_length,
                                            pt_Info->h_font,
                                            pt_Info->w2s_string,
                                            i4_check_length,
                                            TRUE,
                                            pt_Info->i4_box_width,
                                            pt_Info->w2s_can_break,
                                            pt_Info->w2s_skip,
                                            pt_Info->ui2_skip_count,
                                            pt_Info->ui2_skip_width,
                                            pt_Info->b_skip_as_break,
                                            &w2s_next_start,
                                            &ui4_chars_in_line,
                                            &i4_break_width);

        if(i4_ret != WGLR_OK)
        {
            return i4_ret;
        }
        
        /*check the boundary*/
        WGL_ASSERT(i4_break_width <= 0x0000FFFF && i4_break_width >= 0);
        WGL_ASSERT(ui4_chars_in_line <= 0x0000FFFF);
    }
    else /*because the check length is 0, the next start is the same to current*/
    {
        w2s_next_start = pt_Info->w2s_string;
    }
    

    pt_result->ui2_char_number = (UINT16)ui4_chars_in_line;
    pt_result->ui2_width = (UINT16)i4_break_width;

    if(i4_check_length > (INT32)(pt_result->ui2_char_number))
    {
        pt_result->w2s_next_start = w2s_next_start;
    }
    else /*i4_check_length == pt_result->ui2_char_number*/
    {
        /*check if the next is force-break character*/
        pt_result->w2s_next_start = (i4_check_length == pt_Info->i4_string_length) 
        ? NULL : (UTF16_T*)(w2s_next_start-1);

        if (w2s_next_start != NULL){
            /*if there's '\n' and the break is caused by this '\n',
            this '\n' should be included.*/
            pt_result->ui2_char_number += (b_token) ? 1 : 0;
        }
    }

    if(pui2_first_token_pos)
    {
        WGL_ASSERT(i4_check_length <= 0x0000FFFF && i4_check_length >= 0);
        *pui2_first_token_pos = (UINT16)i4_check_length;
    }

    return WGLR_OK;
}



/*******************************************************************************
 * Name
 *      
 * Description
 *          
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
#if 0    
INT32 wgl_get_break_line_token(
    UTF16_T                     a_w2s_break_chars[BREAK_LINE_TOKEN_UTF16_MAX_LENGTH],
    UTF16_T                     a_w2s_force_chars[BREAK_LINE_TOKEN_UTF16_MAX_LENGTH])
{
    /*local variable declaration*/
    CHAR*                       s_break_chars = WGL_CAN_BREAK_CHARS;
    CHAR*                       s_force_chars = WGL_FORCE_BREAK_CHARS;
    INT32                       i4_ret;
    
    /*get UTF16 foramt for break and force characters*/
    i4_ret = x_uc_ps_to_w2s(s_break_chars, a_w2s_break_chars, BREAK_LINE_TOKEN_UTF16_MAX_LENGTH);
    if(UCR_OK != i4_ret)
    {
        return WGLR_INTERNAL_ERROR;
    }
    
    i4_ret = x_uc_ps_to_w2s(s_force_chars, a_w2s_force_chars, BREAK_LINE_TOKEN_UTF16_MAX_LENGTH);
    if(UCR_OK != i4_ret)
    {
        return WGLR_INTERNAL_ERROR;
    }
    
    return WGLR_OK;
}
#endif

/*******************************************************************************
 * Name
 *      
 * Description
 *          
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
BOOL wgl_is_legal_alignment(
    UINT8                       ui1_align)
{
    switch (ui1_align)
    {
    case WGL_AS_LEFT_TOP:
    case WGL_AS_LEFT_CENTER:
    case WGL_AS_LEFT_BOTTOM:
    case WGL_AS_CENTER_TOP:
    case WGL_AS_CENTER_CENTER:
    case WGL_AS_CENTER_BOTTOM:
    case WGL_AS_RIGHT_TOP:
    case WGL_AS_RIGHT_CENTER:
    case WGL_AS_RIGHT_BOTTOM:
        return TRUE;

    default:
        return FALSE;
    }
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Don't check pt_wgl_rect and pt_inset. They cannot be NULL.
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
BOOL wgl_is_legal_inset(
    const GL_RECT_T*            pt_wgl_rect,
    const WGL_INSET_T*          pt_inset)
{
    GL_RECT_T                   t_result;

    WGL_RECT_COPY(&t_result, pt_wgl_rect);
    WGL_RECT_INSET(&t_result, pt_inset->i4_left, pt_inset->i4_top, pt_inset->i4_right, pt_inset->i4_bottom);

    if( (t_result.i4_left >= t_result.i4_right) || (t_result.i4_top >= t_result.i4_bottom) )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Don't check pt_img_info. It cannot be NULL.
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
BOOL wgl_is_legal_img_info(
    const WGL_IMG_INFO_T*       pt_img_info)
{
    /*local variable declaration*/
    INT32                       i4_index;
    HANDLE_TYPE_T               e_handle_type;
    WGL_HIMG_TPL_T              t_tpl[5] = { NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE};

    switch (pt_img_info->e_type)
    {
    case WGL_IMG_SET_BASIC:
        {
            t_tpl[0] = pt_img_info->u_img_data.t_basic.t_enable;
            t_tpl[1] = pt_img_info->u_img_data.t_basic.t_disable;
        }
        break;

    case WGL_IMG_SET_STANDARD:
        {
            t_tpl[0] = pt_img_info->u_img_data.t_standard.t_enable;
            t_tpl[1] = pt_img_info->u_img_data.t_standard.t_disable;
            t_tpl[2] = pt_img_info->u_img_data.t_standard.t_highlight;
        }
        break;

    case WGL_IMG_SET_EXTEND:
        {
            t_tpl[0] = pt_img_info->u_img_data.t_extend.t_enable;
            t_tpl[1] = pt_img_info->u_img_data.t_extend.t_disable;
            t_tpl[2] = pt_img_info->u_img_data.t_extend.t_highlight;
            t_tpl[3] = pt_img_info->u_img_data.t_extend.t_highlight_inactive;
            t_tpl[4] = pt_img_info->u_img_data.t_extend.t_push;
        }
        break;

    default:
        WGL_ASSERT(0); /*illegal type*/
        return FALSE;
    }
    
    /*check handle*/
    for(i4_index = 0; i4_index < 5; i4_index++)
    {
        if(NULL_HANDLE != t_tpl[i4_index])
        {
            /*check valid*/
            if(FALSE == x_handle_valid(t_tpl[i4_index]))
            {
                return FALSE;
            }

            /*check type*/
            if(HR_OK != x_handle_get_type(t_tpl[i4_index], &e_handle_type))
            {
                return FALSE;
            }
            if(HT_WGL_IMG_TPL != e_handle_type)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
#if 0 
INT32 wgl_update_font_handle(
    WGL_FONT_INFO_T*            pt_dest_info,
    HFONT_T*                    ph_font,
    const WGL_FONT_INFO_T*      pt_src_info)
{
    HFONT_T                     h_new_font;
    INT32                       i4_ret;
    const CHAR*                 ps_font_name;

    if(NULL == pt_src_info || NULL == pt_dest_info || NULL == ph_font)
    {
        return WGLR_INV_ARG;
    }

    ps_font_name = (0 != x_strlen(pt_src_info->a_c_font_name)) ? pt_src_info->a_c_font_name : NULL;
    if(FER_OK != x_fe_create_font(ps_font_name, 
                                  pt_src_info->e_font_size, 
                                  pt_src_info->e_font_style, 
                                  pt_src_info->e_font_cmap, 
                                  &h_new_font))
    {
        return WGLR_INV_ARG;
    }

    if(0 != pt_src_info->i2_width)
    {
        i4_ret = x_fe_set_fixed_advance(h_new_font, pt_src_info->i2_width, 0);
        if(FER_OK != i4_ret)
        {
            x_handle_free(h_new_font);
            return WGLR_GET_FONT_INFO_FAIL;
        }
    }

    if(*ph_font)
    {
        i4_ret = x_handle_free(*ph_font);
        *ph_font = NULL_HANDLE;
        WGL_ASSERT(HR_OK == i4_ret);
    }

    x_memcpy(pt_dest_info, pt_src_info, sizeof(WGL_FONT_INFO_T));
    
    *ph_font = h_new_font;
    return WGLR_OK;
}
#else
INT32 wgl_update_font_handle(
    WGL_FONT_INFO_T*            pt_dest_info,
    HFONT_T*                    ph_font,
    const WGL_FONT_INFO_T*      pt_src_info)
{
    HFONT_T                     h_new_font;
    INT32                       i4_ret;

    if(NULL == pt_src_info || NULL == pt_dest_info || NULL == ph_font)
    {
        return WGLR_INV_ARG;
    }

    if(0 != WGL_FONT_CREATE(pt_src_info, &h_new_font))
    {
        return WGLR_INV_ARG;
    }

    if(0 != pt_src_info->i2_width)
    {
        i4_ret = x_fe_set_fixed_advance(h_new_font, pt_src_info->i2_width, 0);
        if(FER_OK != i4_ret)
        {
            WGL_FONT_FREE(h_new_font);
            return WGLR_GET_FONT_INFO_FAIL;
        }
    }

    if(*ph_font)
    {
        i4_ret = WGL_FONT_FREE(*ph_font);
        *ph_font = NULL_HANDLE;
        WGL_ASSERT(0 == i4_ret);
    }

    x_memcpy(pt_dest_info, pt_src_info, sizeof(WGL_FONT_INFO_T));
    
    *ph_font = h_new_font;
    return WGLR_OK;
}
#endif

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_update_text_surface(
    INT32                       i4_width,
    INT32                       i4_height,
    UINT16                      ui2_font_height,
    const WGL_CANVAS_STATUS_T*  pt_cvsst,
    GL_HSURFACE_T*              pt_surface)
{
    /*local variable*/
    GL_SURFACE_DESC_T           t_surf_dest = {0};
    INT32                       i4_ret;

    /*check canvas color mode*/
    if(wgl_cvsst_check_palette_mode(pt_cvsst))
    {
        return WGLR_INV_COLORMODE;
    }

    /*check parameter*/
    if(NULL == pt_surface)
    {
        return WGLR_INV_ARG;
    }

    /*free old surface*/
    if(NULL_HANDLE != *pt_surface)
    {
        i4_ret = x_gl_obj_free(*pt_surface);
        *pt_surface = NULL_HANDLE;
        WGL_ASSERT(GLR_OK == i4_ret);
    }

    if(i4_width <= 0 || i4_height <= 0)
    {
        return WGLR_OK;
    }

    /*prepare information to create surface*/
    t_surf_dest.ui4_width = (UINT32)(i4_width);
    t_surf_dest.ui4_height = (i4_height > (INT32)ui2_font_height) ? (UINT32)ui2_font_height : (UINT32)i4_height;
    t_surf_dest.e_clrmode = GL_COLORMODE_ARGB_D8888;
    t_surf_dest.h_palette = NULL_HANDLE;
    t_surf_dest.h_clipper = NULL_HANDLE;
    
    /*create new surface*/
    i4_ret = x_gl_surface_create(&t_surf_dest, pt_surface);
    if(GLR_OK != i4_ret)
    {
        return WGLR_TEXT_SURF_CREATE_FAIL;
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_draw_string(
                             GL_HGRAPHICS_T              h_graphics,
                             const WGL_CANVAS_STATUS_T*  pt_cvsst,
                             GL_HSURFACE_T               h_surf,
                             const UTF16_T*              w2s_str,
                             UINT16                      ui2_draw_len,
                             INT32                       i4_offset_x,
                             INT32                       i4_offset_y,                                
                             HFONT_T                     h_font,
                             const GL_COLOR_T*           pt_text_clr,
                             const GL_COLOR_T*           pt_bk_clr,
                             INT32                       i4_dest_x,
                             INT32                       i4_dest_y,
                             UINT32                      ui4_dest_width,
                             UINT32                      ui4_dest_height,
                             const WGL_INSET_T*          pt_text_bk_inset)
{
    /*local variable*/
    INT32                       i4_ret;
    
    if(wgl_cvsst_check_palette_mode(pt_cvsst))
    {
        /* this function not support palette mode now */
        return WGLR_INV_COLORMODE;
    }
    else
    {
        GL_COLOR_T                  t_clean_clr;
        GL_RECT_T                   t_text_bk_rect;
        UINT8                       ui1_pre_alpha = 255;

        wgl_cvsst_get_transparent_color(pt_cvsst, &t_clean_clr);
        
        if(pt_bk_clr)
        {
            /*calc string bk rectangle*/
            WGL_RECT_SET(&t_text_bk_rect, 0, 0, (INT32)ui4_dest_width, (INT32)ui4_dest_height);
            if(pt_text_bk_inset)
            {
                WGL_RECT_INSET(&t_text_bk_rect, 
                               pt_text_bk_inset->i4_left, 
                               pt_text_bk_inset->i4_top, 
                               pt_text_bk_inset->i4_right, 
                               pt_text_bk_inset->i4_bottom);
            }
                    
            /*fill string bk color*/
            i4_ret = x_gl_surface_fill(h_surf, pt_bk_clr, 0);
            if(GLR_OK != i4_ret)
            {
                return WGLR_TEXT_DRAW_FAIL;
            }
    
            i4_ret = x_gl_compose(h_graphics, 
                                  h_surf,
                                  i4_dest_x + t_text_bk_rect.i4_left,
                                  i4_dest_y + t_text_bk_rect.i4_top, 
                                  0, 
                                  0, 
                                  (UINT32)WGL_RECT_GET_WIDTH(&t_text_bk_rect), 
                                  (UINT32)WGL_RECT_GET_HEIGHT(&t_text_bk_rect), 
                                  255, 
                                  GL_SRC_OVER);
            if(GLR_OK != i4_ret)
            {
                return WGLR_TEXT_DRAW_FAIL;
            }
        }
    
        /*clean the surface first*/
        if(ui2_draw_len > 0)
        {
            i4_ret = x_gl_surface_fill(h_surf, &t_clean_clr, 0/*GL_SYNC*//*GL_NOWAIT*/);
            if(GLR_OK != i4_ret)
            {
                return WGLR_TEXT_DRAW_FAIL;
            }
            
            i4_ret = x_gl_surface_draw_ustring(h_surf, i4_offset_x, i4_offset_y, (UTF16_T*)w2s_str, ui2_draw_len, h_font, pt_text_clr, NULL, NULL);
            if(GLR_OK != i4_ret)
            {
                return WGLR_TEXT_DRAW_FAIL;
            }
    
            if(pt_text_clr)
            {
                ui1_pre_alpha = pt_text_clr->a;
            }
            
            i4_ret = x_gl_compose(h_graphics, 
                h_surf,
                i4_dest_x,
                i4_dest_y, 
                0, 
                0, 
                ui4_dest_width, 
                ui4_dest_height, 
                ui1_pre_alpha, 
                GL_SRC_OVER);
            if(GLR_OK != i4_ret)
            {
                return WGLR_TEXT_DRAW_FAIL;
            }
        }
    }
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_fill_widget_bk(
    GL_HGRAPHICS_T                h_graphics,
    const GL_COLOR_T*             pt_clr,
    const GL_RECT_T*              pt_rect,
    const WGL_CANVAS_STATUS_T*    pt_cvsst)
{
    if(NULL == pt_clr || NULL == pt_rect || NULL_HANDLE == h_graphics || NULL == pt_cvsst)
    {
        return WGLR_INV_ARG;
    }

    if (wgl_cvsst_check_color_transparent(pt_cvsst, pt_clr))
    {
        return WGLR_OK;
    }

    if(WGL_RECT_IS_EMPTY(pt_rect))
    {
        return WGLR_OK;
    }

    if(GLR_OK != x_gl_set_color(h_graphics, pt_clr))
    {
        return WGLR_INTERNAL_ERROR;
    }
    
    if(GLR_OK != x_gl_fill_rect(h_graphics, pt_rect))
    {
        return WGLR_INTERNAL_ERROR;
    }
    
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          Not check parameter pt_cnt_data
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_calc_cnt_rect(
   wgl_border_cmd_proc_fct      pf_border_cmd_proc,
   WGL_BORDER_CONTEXT_HANDLE_T  h_bdr_data,
   const GL_RECT_T*             pt_widget_rect,
   BOOL                         b_new_inset,
   const WGL_INSET_T*           pt_inset,
   GL_RECT_T*                   pt_cnt_rect)
{
    WGL_INSET_T                 t_bdr_inset = {0};
    INT32                       i4_ret;
    
    /*check param*/
    if (!pt_widget_rect || !pt_cnt_rect)
    {
        return WGLR_INV_ARG;
    }
    
    /*not NULL-Border*/
    if(pf_border_cmd_proc)
    {
        if(b_new_inset)
        {
            /*set the new setting to border*/
            WGL_ASSERT(h_bdr_data);
            i4_ret = pf_border_cmd_proc(h_bdr_data, WGL_CMD_GL_SET_INSET, (VOID*)pt_inset, NULL);
            if(WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
        }
        
        /*get the new border inset*/
        i4_ret = pf_border_cmd_proc(h_bdr_data, WGL_CMD_GL_GET_INSET, &t_bdr_inset, NULL);
        if(WGLR_OK != i4_ret)
        {
            WGL_ASSERT(0);
            return i4_ret;
        }
    }
    else /*NULL-Border*/
    {
        if(b_new_inset && pt_inset)
        {
            WGL_COPY_INSET(&t_bdr_inset, pt_inset);
        }
    }
    
    /*init Content rectangle*/
    WGL_RECT_COPY(pt_cnt_rect, pt_widget_rect);
    
    /*subtract border inset*/
    WGL_RECT_INSET(pt_cnt_rect, t_bdr_inset.i4_left, t_bdr_inset.i4_top, t_bdr_inset.i4_right, t_bdr_inset.i4_bottom);

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 *          
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_update_cvvst(
    const WGL_CVS_CLR_CFG_T*    pt_cvs_cfg,
    WGL_CANVAS_STATUS_T*        pt_cvsst)
{
    if(NULL == pt_cvs_cfg || NULL == pt_cvsst)
    {
        return WGLR_INV_ARG;
    }
            
    /*copy canvas config*/
    if(WGL_CANVAS_CFG_PALETTE & pt_cvs_cfg->ui4_flag)
    {
        pt_cvsst->h_palette = pt_cvs_cfg->h_palette;
    }
    if(WGL_CANVAS_CFG_COLORKEY & pt_cvs_cfg->ui4_flag)
    {
        pt_cvsst->t_colorkey = pt_cvs_cfg->t_colorkey;
    }
    if(WGL_CANVAS_CFG_COLORMODE & pt_cvs_cfg->ui4_flag)
    {
        pt_cvsst->e_clr_mode = pt_cvs_cfg->e_clr_mode;
    }
    return WGLR_OK;
}
#if 0
BOOL wgl_get_anim_running(VOID)
{
    return _b_anim_running;
}

INT32 wgl_replace_anim_condition_callback(HANDLE_T h_widget)
{
    INT32               i4_ret;
    WGL_ANIM_CB_T       t_cb;

    i4_ret = x_wgl_do_cmd(h_widget, WGL_CMD_ANIM_GET_COND_CB, &_t_anim_cond_cb, NULL);

    if (i4_ret == WGLR_OK)
    {
        _h_anim_widget        = h_widget;
        t_cb.pv_tag           = _t_anim_cond_cb.pv_tag;
        t_cb.pf_anim_callback = _app_anim_callback;

        i4_ret = x_wgl_do_cmd(h_widget, WGL_CMD_ANIM_SET_COND_CB, &t_cb, NULL);
    }

    return i4_ret;
}

INT32 wgl_restore_anim_condition_callback(HANDLE_T h_widget)
{
    INT32               i4_ret;

    _h_anim_widget = NULL_HANDLE;
    
    i4_ret = x_wgl_do_cmd(h_widget, WGL_CMD_ANIM_SET_COND_CB, &_t_anim_cond_cb, NULL);

    return i4_ret;
}

VOID wgl_set_anim_state(BOOL b_running)
{
    _b_anim_running = b_running;
}
#endif
/*-----------------------------------------------------------------------------
 * private VIEW methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static BOOL _is_skip_char(
    UTF16_T             w2_ch, 
    const UTF16_T*      w2s_skip, 
    UINT16              ui2_skiplen)
{
    UINT16      ui2_i;

    for (ui2_i = 0; ui2_i < ui2_skiplen; ui2_i++)
    {
        if (x_uc_w2s_get (w2s_skip, ui2_i) == w2_ch)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}
#if 0
static INT32 _app_anim_callback(
    HANDLE_T            h_widget,
    UINT16              ui2_anim_type,
    WGL_ANIM_COND_T     e_anim_cond,
    VOID*               pv_tag)
{
    if (h_widget == _h_anim_widget)
    {
        if (_t_anim_cond_cb.pf_anim_callback)
        {
            /* re-direct to the original callback */
            _t_anim_cond_cb.pf_anim_callback(h_widget, ui2_anim_type, e_anim_cond, _t_anim_cond_cb.pv_tag);
        }

        /* if multiple anim_type are specified, each single stop would end the whole animation */
        if (e_anim_cond == WGL_ANIM_TOTAL_STEP_END ||
            e_anim_cond == WGL_ANIM_ERROR)
        {
            wgl_set_anim_state(FALSE);
        }
    }

    return WGLR_OK;
}
#endif

