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
 * $RCSfile: x_fe.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/8 $
 * $SWAuthor: Ben Tsai $
 * $MD5HEX: 1fe22ea3fb8889b33589635c58e29610 $
 *
 * Description:
 *         This header file contains font library specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "unicode/x_uc_str.h"
#include "font/fe.h"
#include "font/x_fe.h"
#include "graphic/u_gl.h"

#ifdef CLI_LVL_ALL
#include "font/fe_cli.h"
#endif

#ifndef FE_DISABLE_CACHE
#include "font/cache.h"
#endif /*FE_DISABLE_CACHE*/

#include "font/perf.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define FE_FNT_IS_SCALABLE(flags)   ( (flags) & FT_FACE_FLAG_SCALABLE )

#define FE_DIV_CEIL(A, B, type)     _fe_div((A), (B), type)

#define FE_FONT_NAME_LEN            64
#define FE_DEFAULT_CUSTOM_FONT_SIZE (12)
#define FE_DEFAULT_OUTLINE_WIDTH    (1)
#define FE_DEFAULT_UNIFORM_WIDTH    (1)

#define FE_ADV_EXCEPTION_SIZE       ((UINT32)(sizeof(_acc_adv_exception) / sizeof(_acc_adv_exception[0])))


/*-----------------------------------------------------------------------------
                    static variables
 ----------------------------------------------------------------------------*/
extern SLIST_T(_FE_FNT_RSC_T)          gt_fnt_rsc_q;   /* available font list */

static UINT32           gui4_hori_dpi = 72;     /* system horizontal dpi     */
static UINT32           gui4_vert_dpi = 72;     /* system vertical dpi       */
static FT_Library       gpt_library   = NULL;   /* system freetype library   */

/* Auto free functions for AEE */
static handle_autofree_fct apf_fnt_autofree_fcts[HT_GROUP_SIZE];

#ifdef FE_LOAD_BDF_FILE_AT_RENDERING
static DLIST_T(_FNT_ADD_BDF_MEM_FONT_CACHE_T)       t_add_bdf_mem_font_cache_list;
#endif

static CHAR_CODE        _acc_adv_exception[] = {
    0x5F, 0xA9, 0xAE, 'W', 0x6d, 0x77, 0xc0, 0xc1, 0xc2, 0xc3,
    0xc4, 0xc5, 0x2014, 0x2308, 0x230a, 0x2500, 0x2588, 0x41, 0x4d, 0x56,
    0x57};


#define _IS_CHINESE_CHARARTER(w2s_ch) ((w2s_ch >= 0x4E00 && w2s_ch <= 0x9FA5) ? TRUE : ((w2s_ch >= 0xF900 && w2s_ch <= 0xFA2D) ? TRUE : FALSE))


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _fe_verify_lib_handle(
    HFONT_T                 h_fnt,
    HANDLE_TYPE_T*          pe_type,
    FE_FNT_OBJ_T**          ppt_fnt_obj);

static INT32 _fe_verify_lib_handle_scale(
    HFONT_T                 h_fnt,
    HANDLE_TYPE_T*          pe_type,
    FE_FNT_OBJ_T**          ppt_fnt_obj);

static INT32 _fe_fnt_unit_to_pixel(
    INT32                   i4_size,
    const FE_FNT_OBJ_T*     pt_fnt_obj,
    UINT32                  ui4_dpi);

static INT32 _fe_fnt_unit_to_pixel_x(
    INT32                   i4_size,
    const FE_FNT_OBJ_T*     pt_fnt_obj);

static INT32 _fe_fnt_unit_to_pixel_y(
    INT32                   i4_size,
    const FE_FNT_OBJ_T*     pt_fnt_obj);

static INT32 _fe_add_font(
    const CHAR*             ps_font,
    const CHAR*             ps_file,
    const UINT8*            pui1_fontdata,
    UINT32                  ui4_size,
    UINT8                   ui1_small,
    UINT8                   ui1_medium,
    UINT8                   ui1_large);

static INT32 _fe_div(INT32 i4_a, INT32 i4_b, FE_ROUNDING_TYPE_T e_type);

static VOID _fe_replace_leading_char(CHAR_CODE *pt_code);

static VOID _fe_check_font_mode(FE_INIT_DATA at_init_data[], UINT8 ui1_count);
FT_EXPORT_DEF( void )
    FT_GlyphSlot_Blurred( FT_GlyphSlot  slot , UINT16 ui2_kernel_width);


/*-----------------------------------------------------------------------------
 * Name:  _fe_verify_lib_handle
 *
 * Description: make sure that lib is inited and handle is valid
 *
 * Inputs:  h_fnt               Specify the font hanlde.
 *
 * Outputs: pe_type             Get the handle type of the font handle.
 *          ppt_fnt_obj         Get the font object.
 *
 * Returns: FER_OK              Success.
 *          FER_NOT_INIT        Font Engine is not initialized.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
static INT32 _fe_verify_lib_handle(
    HFONT_T                 h_fnt,
    HANDLE_TYPE_T*          pe_type,
    FE_FNT_OBJ_T**          ppt_fnt_obj)
{
#ifdef FE_LOAD_BDF_FILE_AT_RENDERING
    INT32           i4_ret = FER_OK;
#endif /* FE_LOAD_BDF_FILE_AT_RENDERING */

    FNT_ASSERT(pe_type!=NULL && ppt_fnt_obj!=NULL);

    if(gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    if( handle_get_type_obj(h_fnt, pe_type, (VOID**)ppt_fnt_obj) != HR_OK  ||
        *pe_type != FE_FNT_OBJ )
    {
        return FER_INV_HANDLE;
    }

#ifdef FE_LOAD_BDF_FILE_AT_RENDERING
    if( *ppt_fnt_obj != NULL                &&
        (*ppt_fnt_obj)->pt_fnt_rsc == NULL  &&
        (*ppt_fnt_obj)->pt_add_bdf_mem_font_cache != NULL)
    {   /* Load the BDF font */
        FNT_ADD_BDF_MEM_FONT_CACHE_T*   pt_cache;
        FT_Error        e_error;
        FE_FNT_RSC_T    *pt_rsc = NULL;
        FT_Face         pt_face = NULL;

        DLIST_FOR_EACH(pt_cache, &t_add_bdf_mem_font_cache_list, t_lnk)
        {
            if (pt_cache == (*ppt_fnt_obj)->pt_add_bdf_mem_font_cache)
            {
                break;
            }
        }
        if (pt_cache == NULL)
        {
            return FER_INV_HANDLE;
        }

        /* Create first face */
        fe_fnt_lock();
        e_error = FT_New_Memory_Face( gpt_library,
                                      pt_cache->pui1_fontdata,
                                      pt_cache->ui4_fontsize,
                                      0,
                                      &pt_face );
        fe_fnt_unlock();
        if(e_error)
        {
            if(e_error == FT_Err_Out_Of_Memory)
            {
                return FER_NO_MEMORY;
            }
            else
            {
                return FER_INV_FONT;
            }
        }
        /* make sure it is a non-scalable font */
        if (FT_IS_SCALABLE( pt_face ))
        {
            return FER_INV_FONT;
        }

        /* Create a FE_FNT_RSC_T object to store the needed information */
        pt_rsc = (FE_FNT_RSC_T*)FE_ALLOC(sizeof(FE_FNT_RSC_T));
        if(pt_rsc == NULL)
        {
            return FER_NO_MEMORY;
        }
        x_memset(pt_rsc, 0, sizeof(FE_FNT_RSC_T));
        pt_rsc->ui1_font_number = 1;

        i4_ret = fe_add_mem_fnt_rsc(pt_face,
                                    pt_rsc,
                                    pt_cache->ps_fontname,
                                    pt_cache->pui1_fontdata,
                                    pt_cache->ui4_fontsize,
                                    pt_face->available_sizes[0].height,
                                    pt_face->available_sizes[0].height,
                                    pt_face->available_sizes[0].height);
        if(i4_ret != FER_OK)
        {
            return FER_INV_FONT;
        }

        /* free cureent face object */
        fe_fnt_lock();
        FT_Done_Face( pt_face );
        fe_fnt_unlock();

        (*ppt_fnt_obj)->pt_fnt_rsc   = pt_rsc;

        fe_link_obj_to_rsc(pt_rsc, *ppt_fnt_obj);

        fe_fnt_stat_add();

        DLIST_REMOVE(pt_cache, &t_add_bdf_mem_font_cache_list, t_lnk);
        FE_FREE(pt_cache);
    }
#endif /* FE_LOAD_BDF_FILE_AT_RENDERING */

    /* should not happen */
    if( *ppt_fnt_obj == NULL  ||
        (*ppt_fnt_obj)->pt_fnt_rsc == NULL )
    {
        FNT_ASSERT(0);
        return FER_INV_HANDLE;
    }

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _fe_verify_lib_handle_scale
 *
 * Description: check that lib is inited, handle is valid and is scaleable font
 *
 * Inputs:  h_fnt               Specify the font hanlde.
 *
 * Outputs: pe_type             Get the handle type of the font handle.
 *          ppt_fnt_obj         Get the font object.
 *
 * Returns: FER_OK              Success.
 *          FER_NOT_INIT        Font Engine is not initialized.
 *          FER_INV_HANDLE      Invalid handle.
 *          FER_NO_INFO         Can't get scaleable info in font.
 ----------------------------------------------------------------------------*/
static INT32 _fe_verify_lib_handle_scale(
    HFONT_T                 h_fnt,
    HANDLE_TYPE_T*          pe_type,
    FE_FNT_OBJ_T**          ppt_fnt_obj)
{
    INT32   i4_ret;

    FNT_ASSERT(pe_type!=NULL && ppt_fnt_obj!=NULL);

    i4_ret = _fe_verify_lib_handle(h_fnt, pe_type, ppt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }

    /* only valid for scaleable font */
    if(! FE_FNT_IS_SCALABLE(FE_CURRENT_FONT(*ppt_fnt_obj).i4_face_flags))
    {
        return FER_NO_INFO;
    }

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _fe_fnt_unit_to_pixel
 *
 * Description: scale font unit to pixel.
 *
 * Inputs:  i4_size             Size in font unit.
 *          pt_fnt_obj          Reference font object.
 *          ui4_dpi             Resolution in dpi to be refered.
 *
 * Outputs: -
 *
 * Returns: the scaled pixel size
 ----------------------------------------------------------------------------*/
static INT32 _fe_fnt_unit_to_pixel(
    INT32                   i4_size,
    const FE_FNT_OBJ_T*     pt_fnt_obj,
    UINT32                  ui4_dpi)
{
    UINT8           ui1_point_size;
    INT32           i4_em_size;
    INT32           i4_ret;

    FNT_ASSERT(pt_fnt_obj!=NULL && pt_fnt_obj->pt_fnt_rsc!=NULL);

    i4_em_size     = FE_CURRENT_FONT(pt_fnt_obj).ui2_units_per_EM;

    if ( pt_fnt_obj->e_size < FE_FNT_SIZE_NUMBER)
    {
        ui1_point_size =
            FE_CURRENT_FONT(pt_fnt_obj).t_fnt_size[pt_fnt_obj->e_size].ui1_size;

        /* 72 is the fixed constant dpi, NEVER to be changed */
        /* Todo: how to avoid overrun? */
        i4_ret = FE_DIV_CEIL(i4_size * (INT32)ui1_point_size * (INT32)ui4_dpi,
            i4_em_size * (INT32)72, pt_fnt_obj->e_rnd_type);
    }
    else
    {
       ui1_point_size =
            FE_CURRENT_FONT(pt_fnt_obj).t_fnt_size[FE_FNT_SIZE_SMALL].ui1_size;

        /* 72 is the fixed constant dpi, NEVER to be changed */
        /* Todo: how to avoid overrun? */
        i4_ret = FE_DIV_CEIL(i4_size * (INT32)ui1_point_size * (INT32)ui4_dpi,
            i4_em_size * (INT32)72, pt_fnt_obj->e_rnd_type);
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name:  _fe_fnt_unit_to_pixel_x
 *
 * Description: Scale font unit to pixel. According to horizontal resolution.
 *
 * Inputs:  i4_size             Size in font unit.
 *          pt_fnt_obj          Reference font object.
 *
 * Outputs: -
 *
 * Returns: the scaled pixel size
 ----------------------------------------------------------------------------*/
static INT32 _fe_fnt_unit_to_pixel_x(
    INT32                   i4_size,
    const FE_FNT_OBJ_T*     pt_fnt_obj)
{
    FNT_ASSERT(pt_fnt_obj != NULL);

    return _fe_fnt_unit_to_pixel(i4_size, pt_fnt_obj, gui4_hori_dpi);
}


/*-----------------------------------------------------------------------------
 * Name:  _fe_fnt_unit_to_pixel_y
 *
 * Description: Scale font unit to pixel. According to vertical resolution.
 *
 * Inputs:  i4_size             Size in font unit.
 *          pt_fnt_obj          Reference font object.
 *
 * Outputs: -
 *
 * Returns: the scaled pixel size
 ----------------------------------------------------------------------------*/
static INT32 _fe_fnt_unit_to_pixel_y(
    INT32                   i4_size,
    const FE_FNT_OBJ_T*     pt_fnt_obj)
{
    FNT_ASSERT(pt_fnt_obj != NULL);

    return _fe_fnt_unit_to_pixel(i4_size, pt_fnt_obj, gui4_vert_dpi);
}


/*-----------------------------------------------------------------------------
 * Name:  _fe_add_font
 *
 * Description: Wrapper function for x_fe_add_font and x_fe_add_mem_font.
 *
 * Inputs:  ps_font             Specify the Font name. User can define the font
 *                              name. To use the real font family name is not
 *                              necessary.
 *          ps_file             Specify the real font file name.
 *          pui1_fontdata       Specify the font data.
 *          ui4_size            Size of font data.
 *          ui1_small           Font size for 'Small Font'.
 *          ui1_medium          Font size for 'Medium Font'.
 *          ui1_large           Font size for 'Large Font'.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_NO_FONT_FILE    The ps_file font file doesn't exist.
 *          FER_INV_FONT        The ps_file file is not a valid font file.
 *          FER_NO_FONT_SIZE    Some specified font sizes are not available.
 *          FER_NO_MEMORY       Out of system memory
 ----------------------------------------------------------------------------*/
static INT32 _fe_add_font(
    const CHAR*             ps_font,
    const CHAR*             ps_file,
    const UINT8*            pui1_fontdata,
    UINT32                  ui4_size,
    UINT8                   ui1_small,
    UINT8                   ui1_medium,
    UINT8                   ui1_large)
{
    FT_Error        e_error;
    FT_Face         pt_face = NULL;
    INT32           i4_ret = FER_OK;
    FE_FNT_RSC_T    *pt_rsc = NULL;
    INT32           i4_num_faces = 0, i4_cur_face;

    if(gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    /* Check parameters */
    if( ps_font == NULL ||
        (ps_file == NULL && pui1_fontdata == NULL) ||
        (ps_file != NULL && pui1_fontdata != NULL) ||
        ui1_small  == 0 ||
        ui1_medium == 0 ||
        ui1_large  == 0 )
    {
        return FER_INV_ARG;
    }

    /* Check if the font file exist */
    if(ps_file!=NULL && fe_fnt_file_exist(ps_file)!=TRUE)
    {
        return FER_NO_FONT_FILE;
    }

    /* Create first face */
    i4_cur_face = 0;
    if(ps_file != NULL)
    {
        fe_fnt_lock();
        e_error = FT_New_Face( gpt_library, ps_file, i4_cur_face, &pt_face );
        fe_fnt_unlock();
    }
    else
    {
        fe_fnt_lock();
        e_error = FT_New_Memory_Face( gpt_library,
                                      pui1_fontdata,
                                      ui4_size,
                                      i4_cur_face,
                                      &pt_face );
        fe_fnt_unlock();
    }
    if(e_error)
    {
        if(e_error == FT_Err_Out_Of_Memory)
        {
            return FER_NO_MEMORY;
        }
        /* ToDo: Could be other errors... */
        return FER_INV_FONT;
    }

    if (pt_face != NULL)
    {
        i4_num_faces = pt_face->num_faces;
    }


    while(TRUE)     /* control exit point inside */
    {
        /*
         *  Check if the font sizes are supported
         */
        if(! fe_size_supported(pt_face, ui1_small, ui1_medium, ui1_large))
        {
            i4_ret = FER_NO_FONT_SIZE;
            break;
        }

        /*
         *  Create a FE_FNT_RSC_T object to store the needed information
         */
        pt_rsc = (FE_FNT_RSC_T*)FE_ALLOC(sizeof(FE_FNT_RSC_T));
        if(pt_rsc == NULL)
        {
            i4_ret = FER_NO_MEMORY;
            break;
        }
        x_memset(pt_rsc, 0, sizeof(FE_FNT_RSC_T));
        pt_rsc->ui1_font_number = 1;

        if(ps_file != NULL)
        {
            i4_ret = fe_add_file_fnt_rsc(pt_face,
                                         pt_rsc,
                                         ps_font,
                                         ps_file,
                                         ui1_small,
                                         ui1_medium,
                                         ui1_large);
        }
        else
        {
            i4_ret = fe_add_mem_fnt_rsc(pt_face,
                                        pt_rsc,
                                        ps_font,
                                        pui1_fontdata,
                                        ui4_size,
                                        ui1_small,
                                        ui1_medium,
                                        ui1_large);
        }
        if(i4_ret != FER_OK)
        {
            FE_FREE(pt_rsc);
            pt_rsc = NULL;
            break;
        }

        /* free cureent face object */
        fe_fnt_lock();
        FT_Done_Face( pt_face );
        fe_fnt_unlock();
        pt_face = NULL;

        /* no other faces */
        if(++i4_cur_face >= i4_num_faces)
        {
            i4_ret = FER_OK;
            break;
        }

        /* Create next face */
        if(ps_file != NULL)
        {
            fe_fnt_lock();
            e_error = FT_New_Face( gpt_library,
                                   ps_file,
                                   i4_cur_face,
                                   &pt_face );
            fe_fnt_unlock();
        }
        else
        {
            fe_fnt_lock();
            e_error = FT_New_Memory_Face( gpt_library,
                                          pui1_fontdata,
                                          ui4_size,
                                          i4_cur_face,
                                          &pt_face );
            fe_fnt_unlock();
        }
        if(e_error)
        {
            if(e_error == FT_Err_Out_Of_Memory)
            {
                i4_ret = FER_NO_MEMORY;
            }
            else
            {
                /* ToDo: Could be other errors... */
                i4_ret = FER_INV_FONT;
            }
            break;
        }
    };

    /* Free the face */
    if( pt_face )
    {
        fe_fnt_lock();
        FT_Done_Face( pt_face );
        fe_fnt_unlock();
        pt_face = NULL;
    }

    return i4_ret;
}

/* only for memory font */
static INT32 _fe_add_font_ex(
    const CHAR*             ps_font,
    FE_INIT_DATA            at_data[],
    UINT8                   ui1_data_count)
{
    FT_Error        e_error;
    FT_Face         pt_face = NULL;
    INT32           i4_ret = FER_OK;
    FE_FNT_RSC_T    *pt_rsc = NULL;
    INT32           i4_num_faces = 0, i4_cur_face;
    UINT8           ui1_i;

    if (gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    if (ui1_data_count > FE_MAX_FONT_FILE_COUNT)
    {
        return FER_INV_ARG;
    }

    for (ui1_i = 0; ui1_i < ui1_data_count; ui1_i++)
    {
        FE_INIT_DATA    *pt_data = &at_data[ui1_i];

        /* Check parameters */
        if( ps_font             == NULL ||
            pt_data->ui1_small  == 0    ||
            pt_data->ui1_medium == 0    ||
            pt_data->ui1_large  == 0)
        {
            return FER_INV_ARG;
        }

        /* Create first face */
        i4_cur_face = 0;

        fe_fnt_lock();
        e_error = FT_New_Memory_Face( gpt_library,
                                      pt_data->pui1_fontdata,
                                      pt_data->ui4_size,
                                      i4_cur_face,
                                      &pt_face );
        fe_fnt_unlock();

        if(e_error)
        {
            if(e_error == FT_Err_Out_Of_Memory)
            {
                return FER_NO_MEMORY;
            }
            /* ToDo: Could be other errors... */
            return FER_INV_FONT;
        }

        if (pt_face != NULL)
        {
            i4_num_faces = pt_face->num_faces;
        }

        while (TRUE)     /* control exit point inside */
        {
            /*
             *  Check if the font sizes are supported
             */
            if(! fe_size_supported(pt_face, pt_data->ui1_small, pt_data->ui1_medium, pt_data->ui1_large))
            {
                i4_ret = FER_NO_FONT_SIZE;
                break;
            }

            /*
             *  Create a FE_FNT_RSC_T object to store the needed information
             */
            if (pt_rsc == NULL)
            {
                pt_rsc = (FE_FNT_RSC_T *)FE_ALLOC(sizeof(FE_FNT_RSC_T));

                if (pt_rsc == NULL)
                {
                    i4_ret = FER_NO_MEMORY;
                    break;
                }

                x_memset(pt_rsc, 0, sizeof(FE_FNT_RSC_T));

                pt_rsc->ui1_font_number = ui1_data_count;
            }

            pt_rsc->ui1_current_font_id = ui1_i;

            i4_ret = fe_add_mem_fnt_rsc(
                pt_face,
                pt_rsc,
                ps_font,
                pt_data->pui1_fontdata,
                pt_data->ui4_size,
                pt_data->ui1_small,
                pt_data->ui1_medium,
                pt_data->ui1_large);

            if (i4_ret != FER_OK)
            {
                FE_FREE(pt_rsc);
                pt_rsc = NULL;
                break;
            }

            /* free cureent face object */
            fe_fnt_lock();
            FT_Done_Face( pt_face );
            fe_fnt_unlock();
            pt_face = NULL;

            /* no other faces */
            if(++i4_cur_face >= i4_num_faces)
            {
                i4_ret = FER_OK;
                break;
            }

            /* Create next face */
            fe_fnt_lock();
            e_error = FT_New_Memory_Face( gpt_library,
                                          pt_data->pui1_fontdata,
                                          pt_data->ui4_size,
                                          i4_cur_face,
                                          &pt_face );
            fe_fnt_unlock();

            if(e_error)
            {
                if(e_error == FT_Err_Out_Of_Memory)
                {
                    i4_ret = FER_NO_MEMORY;
                }
                else
                {
                    /* ToDo: Could be other errors... */
                    i4_ret = FER_INV_FONT;
                }
                break;
            }
        };

        /* Free the face */
        if( pt_face )
        {
            fe_fnt_lock();
            FT_Done_Face( pt_face );
            fe_fnt_unlock();
            pt_face = NULL;
        }

        if (i4_ret != FER_OK)
        {
            break;
        }
    }

    if (pt_rsc != NULL)
    {
        pt_rsc->ui1_current_font_id = 0;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _fnt_aee_handle_autofree
 *
 * Description: This API calls x_handle_free to free font handle.
 *
 * Inputs:  h_handle  Contains the handle to delete
 *          e_type    Contains the type of the handle.
 *
 * Outputs: -
 *
 * Returns: HR_OK  Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _fnt_aee_handle_autofree(HANDLE_T       h_handle,
                                      HANDLE_TYPE_T  e_type )
{
    if ( e_type == FE_FNT_OBJ )
    {
        x_handle_free(h_handle);
    }
    else
    {
        FNT_ASSERT(0);
    }

    return HR_OK;
}

static VOID _fe_replace_leading_char(CHAR_CODE *pt_code)
{
    if (pt_code)
    {
        if (*pt_code == FE_CC_LEADING_CHAR ||
            (*pt_code >= FE_UTF16_SPACE_USER_START && *pt_code <= FE_UTF16_SPACE_USER_END))
        {
                /* fake character */
                *pt_code = (CHAR_CODE)0x20;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _fe_div
 *
 * Description: return the division (rounding in the absolute value) of
 *              the input values
 *
 * Inputs:  i4_a                numerator
 *          i4_b                denominator
 *          e_type              type of division
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32 _fe_div(INT32 i4_a, INT32 i4_b, FE_ROUNDING_TYPE_T e_type)
{
    INT32   i4_aux;

    #define GET_AUX()               \
    {                               \
        switch (e_type)             \
        {                           \
            case FE_RND_CEILING:    \
                i4_aux = i4_b - 1;  \
                break;              \
            case FE_RND_MIDDLE:     \
                i4_aux = i4_b >> 1; \
                break;              \
            default: /* FLOOR */    \
                i4_aux = 0;         \
        }                           \
    }

    if (i4_a >= 0 && i4_b >= 0)
    {
        GET_AUX();

        return (i4_a + i4_aux) / i4_b;
    }
    else if (i4_a < 0 && i4_b < 0)
    {
        i4_a = -i4_a;
        i4_b = -i4_b;

        GET_AUX();

        return (i4_a + i4_aux) / i4_b;
    }
    else if (i4_a < 0)
    {
        i4_a = -i4_a;

        GET_AUX();

        return -((i4_a + i4_aux) / i4_b);
    }
    else /* if (i4_b < 0) */
    {
        i4_b = -i4_b;

        GET_AUX();

        return -((i4_a + i4_aux) / i4_b);
    }
}

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  fe_init
 *
 * Description: This function initializes the Font Engine and must be called
 *              before invocation of any font-related APIs.
 *
 * Inputs:  ui4_dpi_hori        Horizontal resolution (dpi) of display.
 *          ui4_dpi_vert        Vertical resolution (dpi) of display.
 *
 * Outputs: -
 *
 * Returns: FER_OK              Font Engine is successfully initialized.
 *          FER_INIT_FAILED     The Font Engine initialization failed.
 *          FER_ALREADY_INIT    Font Engine is already initialized.
 *          FER_INTERNAL_ERR    System internal error.
 *          FER_CLI_ERROR       CLI operation is failed.
 ----------------------------------------------------------------------------*/
#ifdef ANDROID

#define FONT_SIZE_SMALL                 ( 30)
#define FONT_SIZE_MEDIUM                ( 30)
#define FONT_SIZE_LARGE                 ( 30)

static FE_INIT_FILE  at_g_android_font_default[] =
{  
    {"/system/fonts/DroidSansFallback.ttf",             FONT_SIZE_SMALL, FONT_SIZE_MEDIUM, FONT_SIZE_LARGE },
    {"/system/fonts/DroidSans.ttf",           			FONT_SIZE_SMALL, FONT_SIZE_MEDIUM, FONT_SIZE_LARGE }
};
#endif
INT32 fe_init(
    UINT32                  ui4_dpi_hori,   /* horizontal resolution in dpi  */
    UINT32                  ui4_dpi_vert)   /* vertical   resolution in dpi  */
                                            /* To do: enable cache manager   */
{
    FT_Error    e_error;
    INT32       i4_ret, i4_i;

    DBG_API(("{FE} init: dpi_hori=%d, dpi_vert=%d\n", ui4_dpi_hori, ui4_dpi_vert));

    if(gpt_library)
    {
        return FER_ALREADY_INIT;
    }

    e_error = FT_Init_FreeType( &gpt_library );
    if ( e_error )
    {
        return FER_INIT_FAILED;
    }

    /* save the arguments as static variables */
    gui4_hori_dpi = ui4_dpi_hori;
    gui4_vert_dpi = ui4_dpi_vert;

    /* initialize internal data structure */
    i4_ret = fe_init_prv();
    if(i4_ret != FER_OK)
    {
        fe_done();
        gpt_library = NULL;
        return i4_ret;
    }

    /* Setup AEE for "c_" APIs */
    for(i4_i=0; i4_i < HT_GROUP_SIZE; ++i4_i)
    {
        apf_fnt_autofree_fcts[i4_i] = NULL;
    }

    apf_fnt_autofree_fcts[ (FE_FNT_OBJ - HT_GROUP_FONT) ] = &_fnt_aee_handle_autofree;

    i4_ret = handle_set_autofree_tbl( HT_GROUP_FONT, apf_fnt_autofree_fcts );
    if ( i4_ret != HR_OK )
    {
        fe_done();
        gpt_library = NULL;
        return FER_INTERNAL_ERR;
    }

#ifdef FE_LOAD_BDF_FILE_AT_RENDERING
    DLIST_INIT(&t_add_bdf_mem_font_cache_list);
#endif

    /* Setup CLI */
#ifdef CLI_LVL_ALL
    i4_ret = fe_cli_init();
    if(i4_ret != FER_OK)
    {
        fe_done();
        gpt_library = NULL;
        return FER_CLI_ERROR;
    }
#endif

#ifdef ANDROID
	i4_ret = x_fe_add_multiple_file_font(SN_FONT_DEFAULT, at_g_android_font_default,2);
	if (i4_ret != FER_OK)
	{
		return FER_INIT_FAILED;
	}
#endif

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  fe_set_resolution
 *
 * Description: This function changes the resolution (dpi).
 *
 * Inputs:  ui4_dpi_hori        Horizontal resolution (dpi) of display.
 *          ui4_dpi_vert        Vertical resolution (dpi) of display.
 *
 * Outputs: -
 *
 * Returns: FER_OK              Font Engine is successfully initialized.
 ----------------------------------------------------------------------------*/
INT32 fe_set_resolution(
    UINT32                  ui4_dpi_hori,   /* horizontal resolution in dpi  */
    UINT32                  ui4_dpi_vert)   /* vertical   resolution in dpi  */
{
    DBG_API(("{FE} set_resolution: dpi_hori=%d, dpi_vert=%d\n",
             ui4_dpi_hori,
             ui4_dpi_vert));

#ifndef FE_DISABLE_CACHE
    if (gpt_library &&
       ((gui4_hori_dpi != ui4_dpi_hori) ||
        (gui4_vert_dpi != ui4_dpi_vert)))
    {
        fe_cache_flush();
    }
#endif

    gui4_hori_dpi = ui4_dpi_hori;
    gui4_vert_dpi = ui4_dpi_vert;

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  fe_verify_create_face
 *
 * Description: if no FT_Face, create it.
 *
 * Inputs:  pt_fnt_obj          Specify the font object to be verified.
 *
 * Outputs: -
 *
 * Returns: FER_OK              Success.
 *          FER_INV_HANDLE      Invalid handle.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_NO_FONT_CMAP    Specified cmap is not supported.
 *          FER_NO_FONT_SIZE    Specified font size is not supported.
 ----------------------------------------------------------------------------*/
INT32 fe_verify_create_face(
    FE_FNT_OBJ_T*           pt_fnt_obj)
{
    FT_Error    e_error;
    INT32       i4_ret;

    FNT_ASSERT(pt_fnt_obj!=NULL && pt_fnt_obj->pt_fnt_rsc!=NULL);

    /* Notice that once FT_Face is created, it will only be deleted
     * when whole font object is deleted by Handle_free */
    if( FE_CURRENT_FONT(pt_fnt_obj).pt_face == NULL )       /* not created yet */
    {
        if(pt_fnt_obj->pt_fnt_rsc->t_fnt_rsc_type == FE_FNT_RSC_TYPE_MEM)
        {
            fe_fnt_lock();

            e_error = FT_New_Memory_Face(gpt_library,
                    FE_CURRENT_FONT(pt_fnt_obj).pui1_fnt_data,
                    (FT_Long)FE_CURRENT_FONT(pt_fnt_obj).ui4_rsc_size,
                    0,
                    &(FE_CURRENT_FONT(pt_fnt_obj).pt_face) );

            fe_fnt_unlock();
        }
        else
        {
            fe_fnt_lock();
            e_error = FT_New_Face( gpt_library,
                    FE_CURRENT_FONT(pt_fnt_obj).ps_file_name,
                    0,
                    &(FE_CURRENT_FONT(pt_fnt_obj).pt_face) );
            fe_fnt_unlock();
        }
        if(e_error)
        {
            /* Todo : Could be other reason? */
            return FER_NO_MEMORY;
        }
    }

    /* always setup parameters because faces are shared */
    /* set character map */
    e_error = FT_Select_Charmap(FE_CURRENT_FONT(pt_fnt_obj).pt_face,
            (FT_Encoding)(pt_fnt_obj->e_cmap));
    if(e_error)
    {
        return FER_NO_FONT_CMAP;
    }
#if 0
	/* mod for bd_subtitle */
	if (pt_fnt_obj->e_size == FE_FNT_SIZE_CUSTOM)
	{
		i4_ret = fe_set_fnt_accurate_pixel_size(pt_fnt_obj->pt_fnt_rsc,
                            pt_fnt_obj->ui1_custom_size);
	}
	else
#endif
	{

	    /* set font size */
    	i4_ret = fe_set_fnt_size(pt_fnt_obj->pt_fnt_rsc,
        	                    pt_fnt_obj->e_size,
            	                gui4_hori_dpi,
                	            gui4_vert_dpi);
	}

    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }

    pt_fnt_obj->ui4_h_dpi = gui4_hori_dpi;
    pt_fnt_obj->ui4_v_dpi = gui4_vert_dpi;

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  fe_done
 *
 * Description: This function terminate the Font Engine.
 *              Only call this when no more any font requirements.
 *              Note: In normal case, this will *NOT* be called forever!
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fe_done(
    VOID)
{
    DBG_API(("{FE} done:\n"));

    if( gpt_library )
    {
        /* release freetype library */
        FT_Done_FreeType( gpt_library );
        gpt_library = NULL;

        /* free internal data */
        fe_done_prv();
    }
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_add_font
 *
 * Description: In system initialization, integrator should call this function
 *              to add a font into system. There must be at least one font in
 *              whole system to display text. When this function is called,
 *              font engine will read and validate the specified font file.
 *              Clients should also specify three font sizes
 *              (Small / Medium / Large) for this font. For vector font,
 *              the font size is assumed to be 'Point Size', and for bitmap
 *              font, 'Pixel Size' instead.  If the specified font size inside
 *              a vector font is an embedded bitmap, the size is turned to be
 *              'Pixel Size' also.
 *
 * Inputs:  ps_font             Specify the Font name. User can define the font
 *                              name. To use the real font family name is not
 *                              necessary.
 *          ps_file             Specify the real font file name.
 *          ui1_small           Font size for 'Small Font'.
 *          ui1_medium          Font size for 'Medium Font'.
 *          ui1_large           Font size for 'Large Font'.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_NO_FONT_FILE    The ps_file font file doesn't exist.
 *          FER_INV_FONT        The ps_file file is not a valid font file.
 *          FER_NO_FONT_SIZE    Some specified font sizes are not available.
 *          FER_NO_MEMORY       Out of system memory
 ----------------------------------------------------------------------------*/
INT32 x_fe_add_file_font(
    const CHAR*             ps_font,        /* font name                     */
    const CHAR*             ps_file,        /* font file name                */
    UINT8                   ui1_small,      /* size of FE_FNT_SIZE_SMALL     */
    UINT8                   ui1_medium,     /* size of FE_FNT_SIZE_MEDIUM    */
    UINT8                   ui1_large)      /* size of FE_FNT_SIZE_LARGE     */
{
    DBG_API(("{FE} add_font: font=%s, file=%s, size(S/M/L)=(%d/%d/%d)\n",
             ps_font, ps_file, ui1_small, ui1_medium, ui1_large));

    return _fe_add_font( ps_font,
                         ps_file,
                         NULL,
                         0,
                         ui1_small,
                         ui1_medium,
                         ui1_large);
}

INT32 x_fe_add_multiple_file_font(
    const CHAR*             ps_font,
    FE_INIT_FILE            at_init_file[],
    UINT8                   ui1_file_count)
{
    FT_Error        e_error;
    FT_Face         pt_face = NULL;
    INT32           i4_ret = FER_OK;
    FE_FNT_RSC_T    *pt_rsc = NULL;
    INT32           i4_num_faces = 0, i4_cur_face;
    UINT8           ui1_i;

    if (gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    if (ui1_file_count > FE_MAX_FONT_FILE_COUNT)
    {
        return FER_INV_ARG;
    }

    for (ui1_i = 0; ui1_i < ui1_file_count; ui1_i++)
    {
        FE_INIT_FILE    *pt_data = &at_init_file[ui1_i];

        /* Check parameters */
        if( ps_font             == NULL ||
            pt_data->ui1_small  == 0    ||
            pt_data->ui1_medium == 0    ||
            pt_data->ui1_large  == 0)
        {
            return FER_INV_ARG;
        }

        /* Create first face */
        i4_cur_face = 0;

        fe_fnt_lock();
        e_error = FT_New_Face( gpt_library,
                                      pt_data->ps_file,
                                      i4_cur_face,
                                      &pt_face );
        fe_fnt_unlock();

        if(e_error)
        {
            if(e_error == FT_Err_Out_Of_Memory)
            {
                return FER_NO_MEMORY;
            }
            /* ToDo: Could be other errors... */
            return FER_INV_FONT;
        }

        if (pt_face != NULL)
        {
            i4_num_faces = pt_face->num_faces;
        }

        while (TRUE)     /* control exit point inside */
        {
            /*
             *  Check if the font sizes are supported
             */
            if(! fe_size_supported(pt_face, pt_data->ui1_small, pt_data->ui1_medium, pt_data->ui1_large))
            {
                i4_ret = FER_NO_FONT_SIZE;
                break;
            }

            /*
             *  Create a FE_FNT_RSC_T object to store the needed information
             */
            if (pt_rsc == NULL)
            {
                pt_rsc = (FE_FNT_RSC_T *)FE_ALLOC(sizeof(FE_FNT_RSC_T));

                if (pt_rsc == NULL)
                {
                    i4_ret = FER_NO_MEMORY;
                    break;
                }

                x_memset(pt_rsc, 0, sizeof(FE_FNT_RSC_T));

                pt_rsc->ui1_font_number = ui1_file_count;
            }

            pt_rsc->ui1_current_font_id = ui1_i;

            i4_ret = fe_add_file_fnt_rsc(
                         pt_face,
                         pt_rsc,
                         ps_font,
                         pt_data->ps_file,
                         pt_data->ui1_small,
                         pt_data->ui1_medium,
                         pt_data->ui1_large);

            if (i4_ret != FER_OK)
            {
                FE_FREE(pt_rsc);
                pt_rsc = NULL;
                break;
            }

            /* free cureent face object */
            fe_fnt_lock();
            FT_Done_Face( pt_face );
            fe_fnt_unlock();
            pt_face = NULL;

            /* no other faces */
            if(++i4_cur_face >= i4_num_faces)
            {
                i4_ret = FER_OK;
                break;
            }

            /* Create next face */
            fe_fnt_lock();
           e_error = FT_New_Face( gpt_library,
                                      pt_data->ps_file,
                                      i4_cur_face,
                                      &pt_face );
            fe_fnt_unlock();

            if(e_error)
            {
                if(e_error == FT_Err_Out_Of_Memory)
                {
                    i4_ret = FER_NO_MEMORY;
                }
                else
                {
                    /* ToDo: Could be other errors... */
                    i4_ret = FER_INV_FONT;
                }
                break;
            }
        };

        /* Free the face */
        if( pt_face )
        {
            fe_fnt_lock();
            FT_Done_Face( pt_face );
            fe_fnt_unlock();
            pt_face = NULL;
        }

        if (i4_ret != FER_OK)
        {
            break;
        }
    }

    if (pt_rsc != NULL)
    {
        pt_rsc->ui1_current_font_id = 0;
    }

    return i4_ret;

}



/*-----------------------------------------------------------------------------
 * Name:  x_fe_add_mem_font
 *
 * Description: In system initialization, integrator should call this function
 *              to add a font into system. There must be at least one font in
 *              whole system to display text. When this function is called,
 *              font engine will read and validate the specified font file.
 *              Clients should also specify three font sizes
 *              (Small / Medium / Large) for this font. For vector font,
 *              the font size is assumed to be 'Point Size', and for bitmap
 *              font, 'Pixel Size' instead.  If the specified font size inside
 *              a vector font is an embedded bitmap, the size is turned to be
 *              'Pixel Size' also.
 *
 * Inputs:  ps_font             Specify the Font name. User can define the font
 *                              name. To use the real font family name is not
 *                              necessary.
 *          pui1_fontdata       Specify the font data.
 *          ui4_size            Size of font data.
 *          ui1_small           Font size for 'Small Font'.
 *          ui1_medium          Font size for 'Medium Font'.
 *          ui1_large           Font size for 'Large Font'.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_NO_FONT_FILE    The ps_file font file doesn't exist.
 *          FER_INV_FONT        The ps_file file is not a valid font file.
 *          FER_NO_FONT_SIZE    Some specified font sizes are not available.
 *          FER_NO_MEMORY       Out of system memory
 *          FER_FONT_EXIST      The same font existed.
 ----------------------------------------------------------------------------*/
INT32 x_fe_add_mem_font(
    const CHAR*             ps_font,        /* font name                     */
    const UINT8*            pui1_fontdata,  /* font data                     */
    UINT32                  ui4_size,       /* size of font data             */
    UINT8                   ui1_small,      /* size of FE_FNT_SIZE_SMALL     */
    UINT8                   ui1_medium,     /* size for FE_FNT_SIZE_MEDIUM   */
    UINT8                   ui1_large)      /* size for FE_FNT_SIZE_LARGE    */
{
    FE_INIT_DATA            t_data;

    DBG_API(("{FE} add_mem_font: font=%s, size(S/M/L)=(%d/%d/%d)\n",
             ps_font, ui1_small, ui1_medium, ui1_large));

    t_data.pui1_fontdata = (UINT8 *)pui1_fontdata;
    t_data.ui1_large     = ui1_large;
    t_data.ui1_medium    = ui1_medium;
    t_data.ui1_small     = ui1_small;
    t_data.ui4_size      = ui4_size;

    _fe_check_font_mode(&t_data, 1);

    return _fe_add_font( ps_font,
                         NULL,
                         t_data.pui1_fontdata,
                         t_data.ui4_size,
                         t_data.ui1_small,
                         t_data.ui1_medium,
                         t_data.ui1_large);
}


INT32 x_fe_add_multiple_mem_font(
    const CHAR*             ps_font,
    FE_INIT_DATA            at_init_data[],
    UINT8                   ui1_data_count)
{
    _fe_check_font_mode(at_init_data, ui1_data_count);

    return _fe_add_font_ex(
        ps_font,
        at_init_data,
        ui1_data_count);
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_add_combine_bmp_mem_font
 *
 * Description: Client should specify three font files, of which each one must
 *              be a bitmap font of one and only one font size.
 *
 * Inputs:  ps_font             Specify the Font name. User can define the font
 *                              name. To use the real font family name is not
 *                              necessary.
 *          pui1_fontdata_small     Specify the data of small-sized font.
 *          ui4_size_small          Specify the data size of small-sized font.
 *          pui1_fontdata_medium    Specify the data of medium-sized font.
 *          ui4_size_medium         Specify the data size of medium-sized font.
 *          pui1_fontdata_large     Specify the data of large-sized font.
 *          ui4_size_large          Specify the data size of large-sized font.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_NO_FONT_FILE    The ps_file font file doesn't exist.
 *          FER_INV_FONT        The ps_file file is not a valid font file.
 *          FER_NO_FONT_SIZE    Some specified font sizes are not available.
 *          FER_NO_MEMORY       Out of system memory
 *          FER_FONT_EXIST      The same font existed.
 ----------------------------------------------------------------------------*/
INT32 x_fe_add_combine_bmp_mem_font(
    const CHAR*             ps_font,                /* font name                     */
    const UINT8*            pui1_fontdata_small,    /* data of small-sized font      */
    UINT32                  ui4_size_small,         /* data size of small-sized font */
    const UINT8*            pui1_fontdata_medium,   /* data of medium-sized font      */
    UINT32                  ui4_size_medium,        /* data size of medium-sized font */
    const UINT8*            pui1_fontdata_large,    /* data of large-sized font      */
    UINT32                  ui4_size_large)         /* data size of large-sized font */
{
#ifndef FE_LOAD_BDF_FILE_AT_RENDERING
    FT_Error        e_error;
    FT_Face         pt_face = NULL;
    INT32           i4_ret = FER_OK;
    FE_FNT_RSC_T    *pt_rsc = NULL;
    CHAR            s_fontname[FE_FONT_NAME_LEN];
    CHAR*           ps_fontname = NULL;
    const UINT8*    apui1_fontdata[3];
    UINT32          aui4_fontsize[3];
    INT16           i2_i;

    DBG_API(("{FE} add_mem_bitmap_font: font=%s\n", ps_font));

    if(gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    /* Check parameters */
    if( ps_font              == NULL ||
        pui1_fontdata_small  == NULL ||
        pui1_fontdata_medium == NULL ||
        pui1_fontdata_large  == NULL)
    {
        return FER_INV_ARG;
    }

    apui1_fontdata[0] = pui1_fontdata_small;
    apui1_fontdata[1] = pui1_fontdata_medium;
    apui1_fontdata[2] = pui1_fontdata_large;
    aui4_fontsize[0]  = ui4_size_small;
    aui4_fontsize[1]  = ui4_size_medium;
    aui4_fontsize[2]  = ui4_size_large;

    if (FE_STRLEN(ps_font) + 3 <= FE_FONT_NAME_LEN)
    {
        ps_fontname = s_fontname;
    }
    else
    {
        ps_fontname = (CHAR*) FE_ALLOC(FE_STRLEN(ps_font) + 3);

    }

    if (ps_fontname == NULL)
    {
        return FER_NO_MEMORY;
    }

    FE_STRCPY(ps_fontname, ps_font);
    ps_fontname[FE_STRLEN(ps_font) + 2] ='\0';

    for (i2_i = 0; i2_i < 3; i2_i++)
    {
        /* Create first face */
        fe_fnt_lock();
        e_error = FT_New_Memory_Face( gpt_library,
                                      apui1_fontdata[i2_i],
                                      aui4_fontsize[i2_i],
                                      0,
                                      &pt_face );
        fe_fnt_unlock();
        if(e_error)
        {
            if(e_error == FT_Err_Out_Of_Memory)
            {
                i4_ret = FER_NO_MEMORY;
            }
            else
            {
                i4_ret = FER_INV_FONT;
            }
            goto _EXIT;
        }

        if (pt_face == NULL)
        {
            i4_ret = FER_INV_FONT;

            goto _EXIT;
        }

        /* make sure it is a non-scalable font */
        if (FT_IS_SCALABLE( pt_face ))
        {
            i4_ret = FER_INV_FONT;
            goto _EXIT;
        }

        /* create name string for the specific sized font */
        switch (i2_i)
        {
            case 0:
                ps_fontname[FE_STRLEN(ps_font)] = '\0'; /* use the given name directly */
                break;
            case 1:
                ps_fontname[FE_STRLEN(ps_font)] = 0x1B; /* to avoid name collision */
                ps_fontname[FE_STRLEN(ps_font) + 1] = 'm';
                break;
            case 2:
                ps_fontname[FE_STRLEN(ps_font)] = 0x1B; /* to avoid name collision */
                ps_fontname[FE_STRLEN(ps_font) + 1] = 'l';
                break;
        }

        /* Create a FE_FNT_RSC_T object to store the needed information */
        pt_rsc = (FE_FNT_RSC_T*)FE_ALLOC(sizeof(FE_FNT_RSC_T));
        if(pt_rsc == NULL)
        {
            i4_ret = FER_NO_MEMORY;
            goto _EXIT;
        }
        x_memset(pt_rsc, 0, sizeof(FE_FNT_RSC_T));
        pt_rsc->ui1_font_number = 1;

        i4_ret = fe_add_mem_fnt_rsc(pt_face,
                                    pt_rsc,
                                    ps_fontname,
                                    apui1_fontdata[i2_i],
                                    aui4_fontsize[i2_i],
                                    pt_face->available_sizes[0].height,
                                    pt_face->available_sizes[0].height,
                                    pt_face->available_sizes[0].height);
        if(i4_ret != FER_OK)
        {
            i4_ret = FER_INV_FONT;
            goto _EXIT;
        }

        if (ps_fontname != s_fontname)
        {
            FE_FREE(ps_fontname);
        }

        /* free cureent face object */
        fe_fnt_lock();
        FT_Done_Face( pt_face );
        fe_fnt_unlock();
    }

_EXIT:
    if (i4_ret != FER_OK)
    {
        for (i2_i = 0; i2_i < 3; i2_i++)
        {
            switch (i2_i)
            {
                case 0:
                    ps_fontname[FE_STRLEN(ps_font) + 1] = 's';
                    break;
                case 1:
                    ps_fontname[FE_STRLEN(ps_font) + 1] = 'm';
                    break;
                case 2:
                    ps_fontname[FE_STRLEN(ps_font) + 1] = 'l';
                    break;
            }
            fe_remove_font(ps_fontname, X_FE_OPTION_NORMAL);
        }

        if (ps_fontname != s_fontname && ps_fontname != NULL)
        {
            FE_FREE(ps_fontname);
        }
        if (pt_rsc != NULL)
        {
            FE_FREE(pt_rsc);
        }
    }

    return i4_ret;
#else
    FNT_ADD_BDF_MEM_FONT_CACHE_T*   pt_cache_small;
    FNT_ADD_BDF_MEM_FONT_CACHE_T*   pt_cache_medium;
    FNT_ADD_BDF_MEM_FONT_CACHE_T*   pt_cache_large;

    DBG_API(("{FE} add_mem_bitmap_font: font=%s\n", ps_font));

    if(gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    /* Check parameters */
    if( ps_font              == NULL ||
        pui1_fontdata_small  == NULL ||
        pui1_fontdata_medium == NULL ||
        pui1_fontdata_large  == NULL)
    {
        return FER_INV_ARG;
    }

    /* Allocate memory first */
    if ((pt_cache_small = FE_ALLOC(
                    sizeof(FNT_ADD_BDF_MEM_FONT_CACHE_T) + FE_STRLEN(ps_font) + 1)) == NULL)
    {
        return FER_NO_MEMORY;
    }
    if ((pt_cache_medium = FE_ALLOC(
                    sizeof(FNT_ADD_BDF_MEM_FONT_CACHE_T) + FE_STRLEN(ps_font) + 3)) == NULL)
    {
        FE_FREE(pt_cache_small);
        return FER_NO_MEMORY;
    }
    if ((pt_cache_large = FE_ALLOC(
                    sizeof(FNT_ADD_BDF_MEM_FONT_CACHE_T) + FE_STRLEN(ps_font) + 3)) == NULL)
    {
        FE_FREE(pt_cache_small);
        FE_FREE(pt_cache_medium);
        return FER_NO_MEMORY;
    }

    /* Insert cache for the small font */
    pt_cache_small->ps_fontname = (CHAR*) (pt_cache_small + 1);
    FE_STRCPY(pt_cache_small->ps_fontname, ps_font);
    pt_cache_small->ps_fontname[FE_STRLEN(ps_font)] = '\0';
    pt_cache_small->pui1_fontdata = (UINT8*) pui1_fontdata_small;
    pt_cache_small->ui4_fontsize  = ui4_size_small;
    DLIST_INSERT_TAIL(pt_cache_small, &t_add_bdf_mem_font_cache_list, t_lnk);

    /* Insert cache for the medium font */
    pt_cache_medium->ps_fontname = (CHAR*) (pt_cache_medium + 1);
    FE_STRCPY(pt_cache_medium->ps_fontname, ps_font);
    pt_cache_medium->ps_fontname[FE_STRLEN(ps_font)] = 0x1B; /* to avoid name collision */
    pt_cache_medium->ps_fontname[FE_STRLEN(ps_font) + 1] = 'm';
    pt_cache_medium->ps_fontname[FE_STRLEN(ps_font) + 2] = '\0';
    pt_cache_medium->pui1_fontdata = (UINT8*) pui1_fontdata_medium;
    pt_cache_medium->ui4_fontsize  = ui4_size_medium;
    DLIST_INSERT_TAIL(pt_cache_medium, &t_add_bdf_mem_font_cache_list, t_lnk);

    /* Insert cache for the large font */
    pt_cache_large->ps_fontname = (CHAR*) (pt_cache_large + 1);
    FE_STRCPY(pt_cache_large->ps_fontname, ps_font);
    pt_cache_large->ps_fontname[FE_STRLEN(ps_font)] = 0x1B; /* to avoid name collision */
    pt_cache_large->ps_fontname[FE_STRLEN(ps_font) + 1] = 'l';
    pt_cache_large->ps_fontname[FE_STRLEN(ps_font) + 2] = '\0';
    pt_cache_large->pui1_fontdata = (UINT8*) pui1_fontdata_large;
    pt_cache_large->ui4_fontsize  = ui4_size_large;
    DLIST_INSERT_TAIL(pt_cache_large, &t_add_bdf_mem_font_cache_list, t_lnk);

    return FER_OK;
#endif /* FE_LOAD_BDF_FILE_AT_RENDERING */
}

INT32 x_fe_change_font_resource(
    const CHAR          *ps_font,
    UINT8               ui1_font_id)
{
    FE_FNT_RSC_T            *pt_rsc = NULL;
    BOOL                    b_found = FALSE;

    fe_fnt_rsc_lock ();

    SLIST_FOR_EACH(pt_rsc, &gt_fnt_rsc_q, t_rsc_lnk)
    {
        if (FE_STRCMP(pt_rsc->ps_fnt_name, ps_font) == 0)
        {
            b_found = TRUE;
            break;
        }
    }

    fe_fnt_rsc_unlock ();

    if (b_found && ui1_font_id < pt_rsc->ui1_font_number)
    {
        pt_rsc->ui1_current_font_id = ui1_font_id;

        return FER_OK;
    }
    else
    {
        return FER_NO_FONT;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _fe_check_font_mode
 *
 * Description: TTF font might be with encryption
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
VOID _fe_check_font_mode(FE_INIT_DATA at_init_data[], UINT8 ui1_count)
{
    /* these three values must be identical to what specified in tools/fontcnv/fontcnv.c */
    #define TTF_ENCRYPT_MODE    (0xC)
    #define ENCRYPT_BYTE        (0x53)
    #define ENCRYPT_KEY         {ENCRYPT_BYTE, ENCRYPT_BYTE & 0xF0, ENCRYPT_BYTE & 0x0F, 0xFF}

    UINT8   ui1_i;
    UINT32  ui4_size;

    for (ui1_i = 0; ui1_i < ui1_count; ui1_i++)
    {
        ui4_size = GET_UINT32_FROM_PTR_BIG_END((at_init_data[ui1_i].pui1_fontdata - 4));

        if (((ui4_size >> 28) & 0xff) != TTF_ENCRYPT_MODE)
        {
            continue;
        }
        else
        {
            UINT32      ui4_i;
            UINT8       ui1_encrypt[] = ENCRYPT_KEY;

            at_init_data[ui1_i].ui4_size = (at_init_data[ui1_i].ui4_size & 0x0fffffff);
            /* hmmm... */
            *(at_init_data[ui1_i].pui1_fontdata - 4) = ((*(at_init_data[ui1_i].pui1_fontdata - 4)) & 0x0F);
            ui4_size = at_init_data[ui1_i].ui4_size;

            for (ui4_i = 0; ui4_i < ui4_size; ui4_i++)
            {
                at_init_data[ui1_i].pui1_fontdata[ui4_i] = (at_init_data[ui1_i].pui1_fontdata[ui4_i] ^ ui1_encrypt[ui4_i & 0x3]);
            }
        }
    }
}

INT32 x_fe_get_cur_font_resource(HFONT_T h_fnt,  UINT8  *pui1_cur_font_id)
{
	HANDLE_TYPE_T	e_type;
	FE_FNT_OBJ_T*   pt_fnt_obj;
	INT32           i4_ret;

	if( pui1_cur_font_id == NULL || h_fnt == 0)
    {
        return FER_INV_ARG;
    }

	i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
	if(i4_ret != FER_OK)
	{
		return i4_ret;
	}
	*pui1_cur_font_id =  pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;
	return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_fe_remove_mem_font
 *
 * Description: This function removes a font from system.
 *
 * Inputs:  ps_font             Specify the Font name.
 *          e_option            Specify X_FE_OPTION_FORCE to force the removal
 *                              of specified font; the font engine will free
 *                              all the associated objects, handles and other
 *                              resources; conversely, if existed any handle
 *                              associated with this font, the font engine
 *                              will return the error status FER_FONT_IN_USE.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_FONT_IN_USE     One or more handle(s) are associated with this
 *                              font.
 *
 ----------------------------------------------------------------------------*/
INT32 x_fe_remove_font(
    const CHAR*             ps_font,        /* font name                     */
    FE_REMOVE_OPTION_T      e_option)
{
    INT32   i4_ret;

    DBG_API(("{FE} remove_mem_font: font=%s\n", ps_font));

    if(gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    /* Check parameters */
    if( ps_font == NULL )
    {
        return FER_INV_ARG;
    }

#ifndef FE_LOAD_BDF_FILE_AT_RENDERING
    if ((i4_ret = fe_remove_font(ps_font, e_option)) == FER_OK)
    {
        /* check combined bitmap font */
        CHAR            s_fontname[FE_FONT_NAME_LEN];
        CHAR*           ps_fontname = NULL;
        INT16           i2_i;

        if (FE_STRLEN(ps_font) + 3 <= FE_FONT_NAME_LEN)
        {
            ps_fontname = s_fontname;
        }
        else
        {
            ps_fontname = (CHAR*) FE_ALLOC(FE_STRLEN(ps_font) + 3);

        }

        if (ps_fontname == NULL)
        {
            return FER_NO_MEMORY;
        }

        FE_STRCPY(ps_fontname, ps_font);
        ps_fontname[FE_STRLEN(ps_font)] = 0x1B; /* to avoid name collision */
        ps_fontname[FE_STRLEN(ps_font) + 2] ='\0';

        for (i2_i = 0; i2_i < 2; i2_i++)
        {
            if (i2_i == 0)
            {
                ps_fontname[FE_STRLEN(ps_font) + 1] = 'm';
            }
            else
            {
                ps_fontname[FE_STRLEN(ps_font) + 1] = 'l';
            }

            i4_ret = fe_remove_font(ps_fontname, e_option);
            if (i4_ret == FER_NO_FONT)
            {
                i4_ret = FER_OK; /* Not a combined bmp font */
                break;
            }
            else if (i4_ret != FER_OK)
            {
                break;
            }
        }

        if (ps_fontname != s_fontname && ps_fontname != NULL)
        {
            FE_FREE(ps_fontname);
        }
    }
#else
    if ((i4_ret = fe_remove_font(ps_font, e_option)) != FER_OK)
    {
        FNT_ADD_BDF_MEM_FONT_CACHE_T*   pt_cache;

        DLIST_FOR_EACH(pt_cache, &t_add_bdf_mem_font_cache_list, t_lnk)
        {
            if (FE_STRCMP(pt_cache->ps_fontname,
                          ps_font) == 0)
            {
                break;
            }
        }
        if (pt_cache != NULL)
        {
            i4_ret = FER_OK;
        }
        else
        {
            i4_ret = FER_NO_FONT;
        }
    }

    if (i4_ret == FER_OK)
    {   /* check combined bitmap font */
        FNT_ADD_BDF_MEM_FONT_CACHE_T*   pt_cache;
        CHAR            s_fontname[FE_FONT_NAME_LEN];
        CHAR*           ps_fontname = NULL;
        INT16           i2_i;

        if (FE_STRLEN(ps_font) + 3 <= FE_FONT_NAME_LEN)
        {
            ps_fontname = s_fontname;
        }
        else
        {
            ps_fontname = (CHAR*) FE_ALLOC(FE_STRLEN(ps_font) + 3);
            if (ps_fontname == NULL)
            {
                return FER_NO_MEMORY;
            }
        }
        FE_STRCPY(ps_fontname, ps_font);
        ps_fontname[FE_STRLEN(ps_font)] = 0x1B; /* to avoid name collision */
        ps_fontname[FE_STRLEN(ps_font) + 2] ='\0';

        for (i2_i = 0; i2_i < 2; i2_i++)
        {
            if (i2_i == 0)
            {
                ps_fontname[FE_STRLEN(ps_font) + 1] = 'm';
            }
            else
            {
                ps_fontname[FE_STRLEN(ps_font) + 1] = 'l';
            }

            i4_ret = fe_remove_font(ps_fontname, e_option);
            if (i4_ret == FER_NO_FONT)
            {   /* Check BDF add cache */
                DLIST_FOR_EACH(pt_cache, &t_add_bdf_mem_font_cache_list, t_lnk)
                {
                    if (FE_STRCMP(pt_cache->ps_fontname,
                                  ps_fontname) == 0)
                    {
                        break;
                    }
                }
                if (pt_cache != NULL)
                {
                    DLIST_REMOVE(pt_cache, &t_add_bdf_mem_font_cache_list, t_lnk);
                    FE_FREE(pt_cache);

                    i4_ret = FER_OK;
                }
                else
                {
                    i4_ret = FER_OK; /* Not a combined bmp font */
                    break;
                }
            }
            else if (i4_ret != FER_OK)
            {
                break;
            }
        }

        if (ps_fontname != s_fontname && ps_fontname != NULL)
        {
            FE_FREE(ps_fontname);
        }
    }
#endif /* FE_LOAD_BDF_FILE_AT_RENDERING */

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_dfl_font
 *
 * Description: This function will set the specified font as system default
 *              font. When calling x_fe_create_font without specify font name,
 *              the system default font will be used.
 *
 * Inputs:  ps_font             Font name to be used as system default font.
 *          e_style             System default font style.
 *                              (name,style) pair is used to specify a font.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_FONT        The font is not in system. The font should be
 *                              added into system by calling x_fe_add_font.
 *          FER_NO_FONT_STYLE   The font style is not supported by this font.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_dfl_font(
    const CHAR*             ps_font,        /* font name                     */
    FE_FNT_STYLE            e_style)        /* font style                    */
{
    DBG_API(("{FE} set_dfl_font: font=%s, style=%d\n", ps_font, e_style));

    if(gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    if(! fe_set_dfl_font(ps_font, e_style))
    {
        if(fe_has_font_name(ps_font))
        {
            return FER_NO_FONT_STYLE;
        }
        return FER_INV_FONT;
    }

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_font
 *
 * Description: Applications can use this function to check if the font is in
 *              this system.
 *
 * Inputs:  ps_font             Contains the font name.
 *          e_style             Font style.
 *
 * Outputs: -
 *
 * Returns: TRUE                This system contains the font.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_font(
    const CHAR*             ps_font,
    FE_FNT_STYLE            e_style)
{
    DBG_API(("{FE} has_font: font=%s, style=%d\n", ps_font, e_style));

    if(gpt_library==NULL || ps_font==NULL)
    {
        return FALSE;
    }

    if (fe_find_font(ps_font, e_style) != NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



INT32   x_fe_get_fontdata_by_name(const CHAR*         ps_font,
                                       	UINT8**    			ppui1_fontdata,
                                                    UINT32*             pui4_fontsize,
                                                    FE_FNT_RSC_TYPE_T*  pt_src_type)
{
    return fe_get_fontdata_by_name(ps_font,
                                    ppui1_fontdata,
                                    pui4_fontsize,
                                    pt_src_type);
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_create_font
 *
 * Description: This function creates a font object according to the specified
 *              font face, size, style and charmap.
 *
 * Inputs:  ps_font             Contains the font name.
 *                              If ps_font is NULL, use system default font,
 *                              and e_style parameter is ignored.
 *          e_size              Contains the font size. Can be
 *                              FE_FNT_SIZE_SMALL,
 *                              FE_FNT_SIZE_MEDIUM or
 *                              FE_FNT_SIZE_LARGE.
 *          e_style             Contains the font style, and can be bit-ored.
 *                              Style can be
 *                              FE_FNT_STYLE_REGULAR,
 *                              FE_FNT_STYLE_ITALIC,
 *                              FE_FNT_STYLE_BOLD,
 *                              FE_FNT_STYLE_UNDERLINE and
 *                              FE_FNT_STYLE_STRIKEOUT.
 *          e_cmap              Contains the character map encoding method.
 *                              Can be
 *                              FE_CMAP_ENC_MS_SYMBOL,
 *                              FE_CMAP_ENC_UNICODE,
 *                              FE_CMAP_ENC_SJIS,
 *                              FE_CMAP_ENC_GB2312,
 *                              FE_CMAP_ENC_BIG5,
 *                              FE_CMAP_ENC_WANSUNG,
 *                              FE_CMAP_ENC_JOHAB,
 *                              FE_CMAP_ENC_ADOBE_STANDARD,
 *                              FE_CMAP_ENC_ADOBE_EXPERT,
 *                              FE_CMAP_ENC_ADOBE_CUSTOM,
 *                              FE_CMAP_ENC_ADOBE_LATIN_1,
 *                              FE_CMAP_ENC_OLD_LATIN_2,
 *                              FE_CMAP_ENC_APPLE_ROMAN.
 *
 * Outputs: ph_fnt              References the handle of the new font object if
 *                              the routine was successful. In all other cases,
 *                              the value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_INV_FONT        The font is not in system.
 *          FER_NO_FONT_CMAP    The font character map is not supported.
 *          FER_NO_FONT_STYLE   The font style is not supported by this font.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_OUT_OF_HANDLES  There are no more handles available.
 ----------------------------------------------------------------------------*/
INT32 x_fe_create_font(
    const CHAR*             ps_font,        /* font name                     */
    FE_FNT_SIZE             e_size,         /* size of font                  */
    FE_FNT_STYLE            e_style,        /* italic/bold/underline/...     */
    FE_CMAP_ENCODING        e_cmap,         /* character map identifier      */
    HFONT_T*                ph_fnt)         /* return : font hadle           */
{
    FE_FNT_RSC_T*   pt_rsc = NULL;
    FE_FNT_OBJ_T*   pt_fnt_obj = NULL;

    DBG_API(("{FE} create_font: font=%s, size=%d, style=%d, cmap=%d\n",
             ps_font, e_size, e_style, e_cmap));

    if(gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    if(ph_fnt == NULL)
    {
        return FER_INV_ARG;
    }

    /* check font */
    pt_rsc = fe_find_font(ps_font, e_style);
    if(pt_rsc == NULL)
    {
#ifdef FE_LOAD_BDF_FILE_AT_RENDERING
        /* Check the BDF cache now */
        FNT_ADD_BDF_MEM_FONT_CACHE_T*   pt_cache;
        CHAR            s_fontname[FE_FONT_NAME_LEN];
        CHAR*           ps_fontname = NULL;
        INT32           i4_tmp_ret = FER_OK;

        if (FE_STRLEN(ps_font) + 3 <= FE_FONT_NAME_LEN)
        {
            ps_fontname = s_fontname;
        }
        else
        {
            ps_fontname = (CHAR*) FE_ALLOC(FE_STRLEN(ps_font) + 3);
            if (ps_fontname == NULL)
            {
                return FER_NO_MEMORY;
            }
        }

        if (ps_font != NULL)
        {
            FE_STRCPY(ps_fontname, ps_font);
        }
        else
        {
            FE_STRCPY(ps_fontname, "");
        }

        switch (e_size)
        {
            case FE_FNT_SIZE_SMALL:
                ps_fontname[FE_STRLEN(ps_font)] = 0;
                break;
            case FE_FNT_SIZE_MEDIUM:
                ps_fontname[FE_STRLEN(ps_font)] = 0x1B; /* to avoid name collision */
                ps_fontname[FE_STRLEN(ps_font) + 1] = 'm';
                ps_fontname[FE_STRLEN(ps_font) + 2] ='\0';
                break;
            case FE_FNT_SIZE_LARGE:
                ps_fontname[FE_STRLEN(ps_font)] = 0x1B; /* to avoid name collision */
                ps_fontname[FE_STRLEN(ps_font) + 1] = 'l';
                ps_fontname[FE_STRLEN(ps_font) + 2] ='\0';
                break;
            default:
                i4_tmp_ret = FER_INV_ARG;
        }

        if (i4_tmp_ret == FER_OK)
        {
            i4_tmp_ret = FER_NO_FONT;

            DLIST_FOR_EACH(pt_cache, &t_add_bdf_mem_font_cache_list, t_lnk)
            {
                if (FE_STRCMP(pt_cache->ps_fontname,
                              ps_font) == 0)
                {
                    /* allocate runtime font object structure */
                    pt_fnt_obj = (FE_FNT_OBJ_T*) FE_ALLOC(sizeof(FE_FNT_OBJ_T));
                    if (pt_fnt_obj == NULL)
                    {
                        i4_tmp_ret = FER_NO_MEMORY;
                    }
                    else
                    {
                        FE_MEMSET(((VOID*) pt_fnt_obj), 0, sizeof(FE_FNT_OBJ_T));

                        /* set font object attributes */
                        pt_fnt_obj->pt_fnt_rsc   = NULL;
                        pt_fnt_obj->e_size       = e_size;
                        pt_fnt_obj->e_cmap       = e_cmap;
                        pt_fnt_obj->e_style      = e_style;
                        pt_fnt_obj->ui4_h_dpi    = gui4_hori_dpi;
                        pt_fnt_obj->ui4_v_dpi    = gui4_vert_dpi;
                        pt_fnt_obj->e_rnd_type   = FE_RND_MIDDLE;   /* default rounding type */
                        pt_fnt_obj->e_encoding   = FE_ENCODING_UCS2;
                        pt_fnt_obj->i2_udl_thk   = FE_DEFAULT_UNDERLINE_THICKNESS;

                        /* add for bd_subtitle */
                        pt_fnt_obj->ui2_outline_width = FE_DEFAULT_OUTLINE_WIDTH;
                        pt_fnt_obj->ui2_uniform_width = FE_DEFAULT_UNIFORM_WIDTH;

                        pt_fnt_obj->pt_add_bdf_mem_font_cache = pt_cache;

                        /* allocate a new handle to keep this font object */
                        if (handle_alloc( FE_FNT_OBJ,
                                          ((VOID*) pt_fnt_obj),
                                          NULL,
                                          fe_free_handle_cb,
                                          &(pt_fnt_obj->h_fnt) ) != HR_OK)
                        {
                            FE_FREE(pt_fnt_obj);
                            i4_tmp_ret = FER_OUT_OF_HANDLES;
                        }
                        else
                        {
                            /* return the handle */
                            *ph_fnt = pt_fnt_obj->h_fnt;

                            if (e_style & FE_FNT_STYLE_BLURRED)
                            {
                                x_fe_set_blurred_width(pt_fnt_obj->h_fnt,0);
                            }

                            i4_tmp_ret = FER_OK;
                        }
                    }

                    break;
                }
            }
        }

        if (ps_fontname != s_fontname && ps_fontname != NULL)
        {
            FE_FREE(ps_fontname);
        }

        if (i4_tmp_ret == FER_OK)
        {
            return FER_OK;
        }
#endif /* FE_LOAD_BDF_FILE_AT_RENDERING */

        if(fe_has_font_name(ps_font))
        {
            return FER_NO_FONT_STYLE;
        }
        else
        {
            return FER_INV_FONT;
        }
    }

    /* check combined bitmap font */
    if (e_size != FE_FNT_SIZE_SMALL && e_size != FE_FNT_SIZE_NUMBER)
    {
        CHAR            s_fontname[FE_FONT_NAME_LEN];
        CHAR*           ps_fontname = NULL;
        FE_FNT_RSC_T*   pt_rsc_tmp = NULL;

        if (FE_STRLEN(ps_font) + 3 <= FE_FONT_NAME_LEN)
        {
            ps_fontname = s_fontname;
        }
        else
        {
            ps_fontname = (CHAR*) FE_ALLOC(FE_STRLEN(ps_font) + 3);
        }

        if (ps_fontname == NULL)
        {
            return FER_NO_MEMORY;
        }

        if (ps_font != NULL)
        {
            FE_STRCPY(ps_fontname, ps_font);
        }
        else
        {
            FE_STRCPY(ps_fontname, "");
        }

        ps_fontname[FE_STRLEN(ps_font)] = 0x1B; /* to avoid name collision */
        if (e_size == FE_FNT_SIZE_MEDIUM)
        {
            ps_fontname[FE_STRLEN(ps_font) + 1] = 'm';
        }
        else
        {
            ps_fontname[FE_STRLEN(ps_font) + 1] = 'l';
        }
        ps_fontname[FE_STRLEN(ps_font) + 2] ='\0';

        /* check font */
        pt_rsc_tmp = fe_find_font(ps_fontname, e_style);
        if(pt_rsc_tmp != NULL)
        {
            pt_rsc = pt_rsc_tmp;
        }

        if (ps_fontname != s_fontname && ps_fontname != NULL)
        {
            FE_FREE(ps_fontname);
        }
    }

    /* check character map */
    if(! fe_has_cmap(pt_rsc, e_cmap))
    {
        return FER_NO_FONT_CMAP;
    }

    /* allocate runtime font object structure */
    pt_fnt_obj = (FE_FNT_OBJ_T*) FE_ALLOC(sizeof(FE_FNT_OBJ_T));
    if (pt_fnt_obj == NULL)
    {
        return FER_NO_MEMORY;
    }
    FE_MEMSET(((VOID*) pt_fnt_obj), 0, sizeof(FE_FNT_OBJ_T));

    /* set font object attributes */
    pt_fnt_obj->pt_fnt_rsc   = pt_rsc;
    pt_fnt_obj->e_size       = e_size;
    pt_fnt_obj->e_cmap       = e_cmap;
    pt_fnt_obj->e_style      = e_style;
    pt_fnt_obj->ui4_h_dpi    = gui4_hori_dpi;
    pt_fnt_obj->ui4_v_dpi    = gui4_vert_dpi;
    pt_fnt_obj->e_rnd_type   = FE_RND_MIDDLE;   /* default rounding type */
    pt_fnt_obj->e_encoding   = FE_ENCODING_UCS2;
    pt_fnt_obj->i2_udl_thk   = FE_DEFAULT_UNDERLINE_THICKNESS;

    /* add for bd_subtitle */
    pt_fnt_obj->ui1_custom_size   = FE_DEFAULT_CUSTOM_FONT_SIZE;
    pt_fnt_obj->ui2_outline_width = FE_DEFAULT_OUTLINE_WIDTH;
    pt_fnt_obj->ui2_uniform_width = FE_DEFAULT_UNIFORM_WIDTH;

    /* allocate memory for the advanced cache */
    pt_fnt_obj->pt_adv_cache = (FE_ADV_CACHE_T *)FE_ALLOC(FE_ADV_CACHE_ENTRY * sizeof(FE_ADV_CACHE_T));

    if (pt_fnt_obj->pt_adv_cache == NULL)
    {
    	FE_FREE(pt_fnt_obj);
        return FER_NO_MEMORY;
    }
    /* reset cache */
    FE_MEMSET(pt_fnt_obj->pt_adv_cache, 0, FE_ADV_CACHE_ENTRY * sizeof(FE_ADV_CACHE_T));

    /* allocate a new handle to keep this font object */
    if (handle_alloc( FE_FNT_OBJ,
                      ((VOID*) pt_fnt_obj),
                      NULL,
                      fe_free_handle_cb,
                      &(pt_fnt_obj->h_fnt) ) != HR_OK)
    {
        FE_FREE(pt_fnt_obj);
        return FER_OUT_OF_HANDLES;
    }

    /* return the handle */
    *ph_fnt = pt_fnt_obj->h_fnt;

    if (e_style & FE_FNT_STYLE_BLURRED)
    {
        x_fe_set_blurred_width(pt_fnt_obj->h_fnt,0);
    }

    fe_link_obj_to_rsc(pt_rsc, pt_fnt_obj);

    fe_fnt_stat_add();

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_cmap
 *
 * Description: This function is used to check if the font contains the
 *              character map.
 *
 * Inputs:  ps_font             The font name to be checked.
 *          e_style             Font style.
 *          e_cmap              Character Map to be checked.
 *
 * Outputs: -
 *
 * Returns: TRUE                The character map is supported by this font.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_cmap(
    const CHAR*             ps_font,
    FE_FNT_STYLE            e_style,
    FE_CMAP_ENCODING        e_cmap)
{
    FE_FNT_RSC_T*   pt_rsc;

    DBG_API(("{FE} has_cmap: font=%s, style=%d, cmap=%d\n",
             ps_font, e_style, e_cmap));

    if(gpt_library == NULL)
    {
        return FALSE;
    }

    pt_rsc = fe_find_font(ps_font, e_style);
    if(pt_rsc == NULL)
    {
        return FALSE;
    }

    if(! fe_has_cmap(pt_rsc, e_cmap))
    {
        return FALSE;
    }

    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_sel_cmap
 *
 * Description: This API selects the specified charmap for the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          e_cmap              Contains the character map encoding method.
 *                              Can be
 *                              FE_CMAP_ENC_MS_SYMBOL,
 *                              FE_CMAP_ENC_UNICODE,
 *                              FE_CMAP_ENC_SJIS,
 *                              FE_CMAP_ENC_GB2312,
 *                              FE_CMAP_ENC_BIG5,
 *                              FE_CMAP_ENC_WANSUNG,
 *                              FE_CMAP_ENC_JOHAB,
 *                              FE_CMAP_ENC_ADOBE_STANDARD,
 *                              FE_CMAP_ENC_ADOBE_EXPERT,
 *                              FE_CMAP_ENC_ADOBE_CUSTOM,
 *                              FE_CMAP_ENC_ADOBE_LATIN_1,
 *                              FE_CMAP_ENC_OLD_LATIN_2,
 *                              FE_CMAP_ENC_APPLE_ROMAN.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_FONT_CMAP    The charmap encoding is not supported.
 ----------------------------------------------------------------------------*/
INT32 x_fe_sel_cmap(
    HFONT_T                 h_fnt,
    FE_CMAP_ENCODING        e_cmap)         /* character map identifier      */
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} sel_cmap: handle=%d, cmap=%d\n", h_fnt, e_cmap));

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }

    if(! fe_has_cmap(pt_fnt_obj->pt_fnt_rsc, e_cmap))
    {
        return FER_NO_FONT_CMAP;
    }

    pt_fnt_obj->e_cmap = e_cmap;

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_size
 *
 * Description: This API sets the size of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          e_size              Contains the new font size. Can be
 *                              FE_FNT_SIZE_SMALL,
 *                              FE_FNT_SIZE_MEDIUM or
 *                              FE_FNT_SIZE_LARGE.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_size(
    HFONT_T                 h_fnt,
    FE_FNT_SIZE             e_size)         /* size of font                  */
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} set_size: handle=%d, size=%d\n", h_fnt, e_size));

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }

    pt_fnt_obj->e_size = e_size;
    pt_fnt_obj->i4_chinese_character_adv = 0;

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_custom_size
 *
 * Description: This API sets the custom size of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          ui1_custom_size     Contains the new custom font size.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_custom_size(
    HFONT_T                 h_fnt,
    UINT8                   ui1_custom_size)         /* size of font                  */
{
    HANDLE_TYPE_T   e_type;
    FT_Error        e_error;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret = FER_OK;

    DBG_API(("{FE} set_size: handle=%d, size=%d\n", h_fnt, ui1_custom_size));

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);

    if (pt_fnt_obj->e_size != FE_FNT_SIZE_CUSTOM || ui1_custom_size == 0)
    {
        x_dbg_stmt("{Font Library} this font handle can not be set a custom size.\n");
        return FER_INV_HANDLE;
    }

    if (i4_ret != FER_OK)
    {
        return i4_ret;
    }
     /* critical section for shared face */
     fe_fnt_lock();
    #if 0
    if( FE_CURRENT_FONT(pt_fnt_obj).pt_face == NULL )       /* not created yet */
    {
        if(pt_fnt_obj->pt_fnt_rsc->t_fnt_rsc_type == FE_FNT_RSC_TYPE_MEM)
        {
            e_error = FT_New_Memory_Face(gpt_library,
                    FE_CURRENT_FONT(pt_fnt_obj).pui1_fnt_data,
                    (FT_Long)FE_CURRENT_FONT(pt_fnt_obj).ui4_rsc_size,
                    0,
                    &(FE_CURRENT_FONT(pt_fnt_obj).pt_face) );

        }
        else
        {
            e_error = FT_New_Face( gpt_library,
                    FE_CURRENT_FONT(pt_fnt_obj).ps_file_name,
                    0,
                    &(FE_CURRENT_FONT(pt_fnt_obj).pt_face) );
        }
        if(e_error)
        {
            /* Todo : Could be other reason? */
            fe_fnt_unlock();
            return FER_NO_MEMORY;
        }
    }
    #endif
    i4_ret = fe_verify_create_face(pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        fe_fnt_unlock(); /* critical section for shared face */
        return i4_ret;
    }
    if (! FT_IS_SCALABLE( FE_CURRENT_FONT(pt_fnt_obj).pt_face ))
    {
        i4_ret = FER_NO_FONT_SIZE;
        fe_fnt_unlock();
        return i4_ret;
    }

    /* always setup parameters because faces are shared */
    /* set character map */
    e_error = FT_Select_Charmap(FE_CURRENT_FONT(pt_fnt_obj).pt_face,
            (FT_Encoding)(pt_fnt_obj->e_cmap));
    if(e_error)
    {
        i4_ret = FER_NO_FONT_CMAP;
        fe_fnt_unlock();
        return i4_ret;
    }

    FE_CURRENT_FONT(pt_fnt_obj).t_fnt_size[FE_FNT_SIZE_CUSTOM].ui1_size  = ui1_custom_size;

    FE_CURRENT_FONT(pt_fnt_obj).t_fnt_size[FE_FNT_SIZE_CUSTOM].e_unit  =
    FE_CURRENT_FONT(pt_fnt_obj).t_fnt_size[FE_FNT_SIZE_SMALL].e_unit;

    pt_fnt_obj->ui1_custom_size = ui1_custom_size;

    pt_fnt_obj->i4_chinese_character_adv = 0;
    /*

CUSTOM_EXIT:

        Free the face */
    fe_fnt_unlock();
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_outline_width
 *
 * Description: This API sets the outline width of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          ui2_width           Contains the width of the outline
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_outline_width(
    HFONT_T                 h_fnt,
    UINT16                  ui2_width)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    if (h_fnt == NULL_HANDLE || ui2_width == 0)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }

    pt_fnt_obj->ui2_outline_width = ui2_width;

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_rendering_offset
 *
 * Description: This function set the rendering offset of a font.
 *
 * Inputs:  ps_font             font name
 *          ui1_font_id         font ID
 *          i4_offset_small     offset for size SMALL
 *          i4_offset_medium    offset for size MEDIUM
 *          i4_offset_large     offset for size LARGE
 *
 * Outputs: -
 *
 * Returns: FER_OK              Font Engine is successfully initialized.
 *          FER_NO_FONT         No such font name or font ID is found.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_rendering_offset(
    const CHAR          *ps_font,
    UINT8               ui1_font_id,
    INT32               i4_offset_small,
    INT32               i4_offset_medium,
    INT32               i4_offset_large)
{
    FE_FNT_RSC_T            *pt_rsc = NULL;
    BOOL                    b_found = FALSE;

    fe_fnt_rsc_lock ();

    SLIST_FOR_EACH(pt_rsc, &gt_fnt_rsc_q, t_rsc_lnk)
    {
        if (FE_STRCMP(pt_rsc->ps_fnt_name, ps_font) == 0)
        {
            b_found = TRUE;
            break;
        }
    }

    if (b_found && ui1_font_id < pt_rsc->ui1_font_number)
    {
        FE_PER_FNT_RSC_T    *pt_per_rsc = &(pt_rsc->at_font_data[((ui1_font_id < FE_MAX_FONT_FILE_COUNT)? ui1_font_id: 0)]);

        pt_per_rsc->ai4_rendering_offset[FE_FNT_SIZE_SMALL ] = i4_offset_small;
        pt_per_rsc->ai4_rendering_offset[FE_FNT_SIZE_MEDIUM] = i4_offset_medium;
        pt_per_rsc->ai4_rendering_offset[FE_FNT_SIZE_LARGE ] = i4_offset_large;

        fe_fnt_rsc_unlock ();
        return FER_OK;
    }
    else
    {
        fe_fnt_rsc_unlock ();
        return FER_NO_FONT;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_style
 *
 * Description: This API sets the style of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          pt_style            References the style if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_style(
    HFONT_T                 h_fnt,
    FE_FNT_STYLE*           pt_style)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    if (pt_style == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK && i4_ret != FER_NO_INFO)
    {
        return i4_ret;
    }

    pt_fnt_obj->e_style = *pt_style;

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_uniform_width
 *
 * Description: This API sets the uniform width of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          ui2_width           Contains the width of the uniform
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_uniform_width(
    HFONT_T                 h_fnt,
    UINT16                  ui2_width)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    if (h_fnt == NULL_HANDLE || ui2_width == 0)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }

    pt_fnt_obj->ui2_uniform_width = ui2_width;

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_fixed_advance
 *
 * Description: This API sets the horizontal and vertical advance of the font
 *              object to fixed values; the glyph advance and kerning
 *              information contained in the font file would then be ignored.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          i2_hori             Contains the horizontal advance. A value of
 *                              zero indicates that the fixed horizontal
 *                              advance is canceled.
 *          i2_vert             Contains the vertical advance. A value of zero
 *                              indicates that the fixed vertical advance is
 *                              canceled.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_fixed_advance(
    HFONT_T                 h_fnt,
    INT16                   i2_hori,
    INT16                   i2_vert)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} set_fixed_advance: handle=%d, hori=%d, vert=%d\n", h_fnt, i2_hori, i2_vert));

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }

    pt_fnt_obj->i2_fix_h_adv = i2_hori;
    pt_fnt_obj->i2_fix_v_adv = i2_vert;

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_vertical
 *
 * Description: This API determines if the font object has vertical glyph.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font has vertical glyph.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_vertical(
    HFONT_T                 h_fnt)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} has_vertical: handle=%d\n", h_fnt));

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return FALSE;
    }

    if(FE_CURRENT_FONT(pt_fnt_obj).i4_face_flags & FT_FACE_FLAG_VERTICAL)
    {
        return TRUE;
    }

    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_is_scalable
 *
 * Description: This API determines if the font is scaleable, in other words,
 *              vectorial and outline fonts.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font is scaleable.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_is_scalable(
    HFONT_T                 h_fnt)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} is_scalable: handle=%d\n", h_fnt));

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return FALSE;
    }

    if( FE_FNT_IS_SCALABLE(FE_CURRENT_FONT(pt_fnt_obj).i4_face_flags) )
    {
        return TRUE;
    }

    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_is_fixed_width
 *
 * Description: This API determines if the font is fixed width or proportional
 *              width font.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font is fixed width font.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_is_fixed_width(
    HFONT_T                 h_fnt)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} is_fixed_width: handle=%d\n", h_fnt));

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return FALSE;
    }

    if (FE_CURRENT_FONT(pt_fnt_obj).i4_face_flags & FT_FACE_FLAG_FIXED_WIDTH)
    {
        return TRUE;
    }

    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_kerning
 *
 * Description: This API determines if the font contains kerning information.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font contains kerning information.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_kerning(
    HFONT_T                 h_fnt)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} has_kerning: handle=%d\n", h_fnt));

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return FALSE;
    }

    fe_fnt_rsc_lock();
    if (FE_CURRENT_FONT(pt_fnt_obj).i4_face_flags & FT_FACE_FLAG_KERNING)
    {
        fe_fnt_rsc_unlock();
        return TRUE;
    }

    fe_fnt_rsc_unlock();
    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_fixed_sizes
 *
 * Description: This API determines if the font contains fixed size bitmap.
 *              For non-scaleable fonts, this function will always return TRUE.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font contains fixed size bitmap.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_fixed_sizes(
    HFONT_T                 h_fnt)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} has_fixed_sizes: handle=%d\n", h_fnt));

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return FALSE;
    }

    if (FE_CURRENT_FONT(pt_fnt_obj).i4_face_flags & FT_FACE_FLAG_FIXED_SIZES)
    {
        return TRUE;
    }

    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_ascender
 *
 * Description: This API retrieves the ascent value of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_asc             References the ascent value if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_ascender(
    HFONT_T                 h_fnt,
    INT16*                  pi2_asc)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} get_ascender: handle=%d\n", h_fnt));

    if(pi2_asc == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret == FER_OK)
    {
        fe_fnt_rsc_lock();
        *pi2_asc = FE_CURRENT_FONT(pt_fnt_obj).i2_ascender;
        *pi2_asc = _fe_fnt_unit_to_pixel_y(*pi2_asc, pt_fnt_obj);
        fe_fnt_rsc_unlock();
    }
    else if(i4_ret == FER_NO_INFO)
    {
        fe_fnt_rsc_lock();
        *pi2_asc = FE_CURRENT_FONT(pt_fnt_obj).i2_ascender;
        fe_fnt_rsc_unlock();
    }
    else
    {
        return i4_ret;
    }

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_descender
 *
 * Description: This API retrieves the descent value of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_desc            References the descent value if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_descender(
    HFONT_T                 h_fnt,
    INT16*                  pi2_desc)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} get_descender: handle=%d\n", h_fnt));

    if(pi2_desc == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);

    if (i4_ret == FER_OK)
    {
        if (pt_fnt_obj->e_rnd_type == FE_RND_MIDDLE)
        {
            i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
            if (i4_ret == FER_OK)
            {
                fe_fnt_rsc_lock();
                *pi2_desc = FE_CURRENT_FONT(pt_fnt_obj).i2_descender;
                *pi2_desc = _fe_fnt_unit_to_pixel_y(*pi2_desc, pt_fnt_obj);
                fe_fnt_rsc_unlock();
            }
            else if (i4_ret == FER_NO_INFO)
            {
                fe_fnt_rsc_lock();
                *pi2_desc = FE_CURRENT_FONT(pt_fnt_obj).i2_descender;
                fe_fnt_rsc_unlock();
            }
            else
            {
                return i4_ret;
            }
        }
        else
        {
            INT16   i2_height, i2_ascender;

            x_fe_get_height(h_fnt, &i2_height);
            x_fe_get_ascender(h_fnt, &i2_ascender);

            *pi2_desc = -(i2_height - i2_ascender);
        }

        return FER_OK;
    }
    else
    {
        return i4_ret;
    }
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_height
 *
 * Description: This API retrieves the height of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_height          References the height if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_height(
    HFONT_T                 h_fnt,
    INT16*                  pi2_height)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} get_height: handle=%d\n", h_fnt));

    if(pi2_height == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret == FER_OK)
    {
        fe_fnt_rsc_lock();
    	*pi2_height = FE_CURRENT_FONT(pt_fnt_obj).i2_height;
    	*pi2_height = _fe_fnt_unit_to_pixel_y(*pi2_height, pt_fnt_obj);
        fe_fnt_rsc_unlock();
    }
    else if(i4_ret == FER_NO_INFO)
    {
        fe_fnt_rsc_lock();
        *pi2_height = FE_CURRENT_FONT(pt_fnt_obj).i2_height;
        fe_fnt_rsc_unlock();
    }
    else
    {
        return i4_ret;
    }

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_underline_position
 *
 * Description: This API retrieves the underline position of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_udl_pos         References the underline position if the
 *                              routine was successful. In all other cases,
 *                              the value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_underline_position(
    HFONT_T                 h_fnt,
    INT16*                  pi2_udl_pos)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} get_underline_position: handle=%d\n", h_fnt));

    if(pi2_udl_pos == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret == FER_OK)
    {
        *pi2_udl_pos = FE_CURRENT_FONT(pt_fnt_obj).i2_udl_pos;
        *pi2_udl_pos = _fe_fnt_unit_to_pixel_y(*pi2_udl_pos, pt_fnt_obj);
    }
    else if(i4_ret == FER_NO_INFO)
    {
        *pi2_udl_pos = FE_CURRENT_FONT(pt_fnt_obj).i2_udl_pos;
    }
    else
    {
        return i4_ret;
    }

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_underline_thickness
 *
 * Description: This API retrieves the underline thickness of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_udl_thk         References the underline thickness if the
 *                              routine was successful. In all other cases,
 *                              the value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_underline_thickness(
    HFONT_T                 h_fnt,
    INT16*                  pi2_udl_thk)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} get_underline_thickness: handle=%d\n", h_fnt));

    if(pi2_udl_thk == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret == FER_OK)
    {
        if (pt_fnt_obj->i2_udl_thk == FE_DEFAULT_UNDERLINE_THICKNESS)
        {
            *pi2_udl_thk = FE_CURRENT_FONT(pt_fnt_obj).i2_udl_thk;
            *pi2_udl_thk = _fe_fnt_unit_to_pixel_y(*pi2_udl_thk, pt_fnt_obj);
        }
        else
        {
            *pi2_udl_thk = pt_fnt_obj->i2_udl_thk;
        }
    }
    else if(i4_ret == FER_NO_INFO)
    {
        *pi2_udl_thk = FE_CURRENT_FONT(pt_fnt_obj).i2_udl_thk;
    }
    else
    {
        return i4_ret;
    }

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_underline_thickness
 *
 * Description: This API sets the underline thickness of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          i2_udl_thk          References the underline thickness if the
 *                              routine was successful. In all other cases,
 *                              the value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_underline_thickness(
    HFONT_T                 h_fnt,
    INT16                   i2_udl_thk)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    if (i2_udl_thk < 0 && i2_udl_thk != FE_DEFAULT_UNDERLINE_THICKNESS)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);

    if (i4_ret == FER_OK)
    {
        pt_fnt_obj->i2_udl_thk = i2_udl_thk;
    }
    else
    {
        return i4_ret;
    }

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_name
 *
 * Description: This API retrieves the font name.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          i4_len              Specify the size of buffer allocated for the
 *                              name string.
 *
 * Outputs: ps_name             References the name if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_name(
    HFONT_T                 h_fnt,
    CHAR*                   ps_name,
    INT32                   i4_len)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} get_name: handle=%d\n", h_fnt));

    if(ps_name == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK && i4_ret != FER_NO_INFO)
    {
        return i4_ret;
    }

    if(i4_len < (FE_STRLEN(pt_fnt_obj->pt_fnt_rsc->ps_fnt_name) + 1))
    {
        return FER_INV_ARG;
    }

    /* copy font name */
    FE_STRCPY(ps_name, pt_fnt_obj->pt_fnt_rsc->ps_fnt_name);

    return FER_OK;
}

INT32 x_fe_get_rendering_offset(
    HFONT_T                 h_font,
    INT32                   *pi4_offset)
{
    INT32           i4_ret = FER_OK;
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T    *pt_fnt_obj = NULL;

    if (h_font == NULL_HANDLE || pi4_offset == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle(h_font, &e_type, &pt_fnt_obj);

    if (i4_ret == FER_OK && e_type == FE_FNT_OBJ && pt_fnt_obj != NULL)
    {
        fe_fnt_rsc_lock();
        *pi4_offset = FE_CURRENT_FONT(pt_fnt_obj).ai4_rendering_offset[((pt_fnt_obj->e_size < FE_FNT_SIZE_NUMBER)? pt_fnt_obj->e_size: 0)];
        fe_fnt_rsc_unlock();
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_size
 *
 * Description: This API retrieves the size of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pe_size             References the font size. Can be
 *                              FE_FNT_SIZE_SMALL,
 *                              FE_FNT_SIZE_MEDIUM or
 *                              FE_FNT_SIZE_LARGE.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_size(
    HFONT_T                 h_fnt,
    FE_FNT_SIZE*            pe_size)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} get_size: handle=%d\n", h_fnt));

    if(pe_size == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK && i4_ret != FER_NO_INFO)
    {
        return i4_ret;
    }

    *pe_size = pt_fnt_obj->e_size;
    return FER_OK;
}
INT32 x_fe_get_font_size(
    HFONT_T                 h_fnt,
    FE_FNT_SIZE*            pe_size,
    UINT8*                  pu1_size)
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;

    DBG_API(("{FE} get_size: handle=%d\n", h_fnt));

    if(pe_size == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK && i4_ret != FER_NO_INFO)
    {
        return i4_ret;
    }

    *pe_size = pt_fnt_obj->e_size;

    if (pu1_size)
    {
        if((pt_fnt_obj->e_size < FE_FNT_SIZE_CUSTOM))
        {
            *pu1_size = FE_CURRENT_FONT(pt_fnt_obj).t_fnt_size[pt_fnt_obj->e_size].ui1_size;
        }
        else
        {
            *pu1_size = 0;
        }
    }
    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_style
 *
 * Description: This API retrieves the style of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pt_style            References the style if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_style(
    HFONT_T                 h_fnt,
    FE_FNT_STYLE*           pt_style)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} get_style: handle=%d\n", h_fnt));

    if(pt_style == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK && i4_ret != FER_NO_INFO)
    {
        return i4_ret;
    }

    *pt_style = pt_fnt_obj->e_style;

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_cmap
 *
 * Description: This API retrieves the character map encoding method of the font
 *              object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pe_cmap             Contains the character map encoding method.
 *                              Can be
 *                              FE_CMAP_ENC_MS_SYMBOL,
 *                              FE_CMAP_ENC_UNICODE,
 *                              FE_CMAP_ENC_SJIS,
 *                              FE_CMAP_ENC_GB2312,
 *                              FE_CMAP_ENC_BIG5,
 *                              FE_CMAP_ENC_WANSUNG,
 *                              FE_CMAP_ENC_JOHAB,
 *                              FE_CMAP_ENC_ADOBE_STANDARD,
 *                              FE_CMAP_ENC_ADOBE_EXPERT,
 *                              FE_CMAP_ENC_ADOBE_CUSTOM,
 *                              FE_CMAP_ENC_ADOBE_LATIN_1,
 *                              FE_CMAP_ENC_OLD_LATIN_2,
 *                              FE_CMAP_ENC_APPLE_ROMAN.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_cmap(
    HFONT_T                 h_fnt,
    FE_CMAP_ENCODING*       pe_cmap)         /* character map identifier      */
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;

    DBG_API(("{FE} get_cmap: handle=%d\n", h_fnt));

    if(pe_cmap == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK && i4_ret != FER_NO_INFO)
    {
        return i4_ret;
    }

    *pe_cmap = pt_fnt_obj->e_cmap;

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_max_advance
 *
 * Description: This API retrieves the max advance width and height of the
 *              font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_max_adv_w       References the max advance width if the routine
 *                              was successful. In all other cases, the value
 *                              of this argument is undetermined.
 *          pi2_max_adv_h       References the max advance height if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_max_advance(
    HFONT_T                 h_fnt,
    INT16*                  pi2_max_adv_w,  /* maximum advance width         */
    INT16*                  pi2_max_adv_h)  /* maximum advance height        */
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;
    UINT32          ui4_i;

    DBG_API(("{FE} get_max_advance: handle=%d\n", h_fnt));

    if (pi2_max_adv_w == NULL || pi2_max_adv_h == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if (i4_ret != FER_OK && i4_ret != FER_NO_INFO)
    {
        return i4_ret;
    }

    /* user does not set fixed advance */
    if (pt_fnt_obj->i2_fix_h_adv == 0)
    {
        if (pt_fnt_obj->i2_max_adv_h != 0)
        {
            *pi2_max_adv_w = pt_fnt_obj->i2_max_adv_h;
        }
        else
        {
            *pi2_max_adv_w = FE_CURRENT_FONT(pt_fnt_obj).i2_max_adv_w;
            if (i4_ret != FER_NO_INFO)
            {
                *pi2_max_adv_w = _fe_fnt_unit_to_pixel_x(*pi2_max_adv_w, pt_fnt_obj);
            }
        }
    }
    else
    {
        *pi2_max_adv_w = pt_fnt_obj->i2_fix_h_adv;
        pt_fnt_obj->i2_max_adv_h = *pi2_max_adv_w;
    }

    /* user does not set fixed advance */
    if (pt_fnt_obj->i2_fix_v_adv == 0)
    {
        if (pt_fnt_obj->i2_max_adv_v != 0)
        {
            *pi2_max_adv_h = pt_fnt_obj->i2_max_adv_v;
        }
        else
        {
            *pi2_max_adv_h = FE_CURRENT_FONT(pt_fnt_obj).i2_max_adv_h;
            if(i4_ret != FER_NO_INFO)
            {
                *pi2_max_adv_h = _fe_fnt_unit_to_pixel_y(*pi2_max_adv_h, pt_fnt_obj);
            }
        }
    }
    else
    {
        *pi2_max_adv_h = pt_fnt_obj->i2_fix_v_adv;
        pt_fnt_obj->i2_max_adv_v = *pi2_max_adv_h;
    }


    if (pt_fnt_obj->i2_max_adv_h == 0 || pt_fnt_obj->i2_max_adv_v == 0)
    {
        for (ui4_i = 0; ui4_i < FE_ADV_EXCEPTION_SIZE; ui4_i++)
        {
            INT32       i4_h = 0, i4_v = 0;
            #define     FE_CHECK_ADV_EXCEPTION_ID(id)       (((id) < FE_ADV_EXCEPTION_SIZE)? (id): 0)

            if (x_fe_get_glyph_advance(
                    h_fnt,
                    _acc_adv_exception[FE_CHECK_ADV_EXCEPTION_ID(ui4_i)],
                    &i4_h,
                    &i4_v) == FER_OK)
            {
                if (*pi2_max_adv_w < (INT16)i4_h)
                {
                    *pi2_max_adv_w = (INT16)i4_h;
                }

                if (*pi2_max_adv_h < (INT16)i4_v)
                {
                    *pi2_max_adv_h = (INT16)i4_v;
                }
            }
        }

        if (pt_fnt_obj->i2_max_adv_h == 0)
        {
            pt_fnt_obj->i2_max_adv_h = *pi2_max_adv_w;
        }

        if (pt_fnt_obj->i2_max_adv_v == 0)
        {
            pt_fnt_obj->i2_max_adv_v = *pi2_max_adv_h;
        }
    }

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_char_code
 *
 * Description: This function is used to check if the font contains the
 *              character code.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          cc_char_code        The character code.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font contains this character code.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_char_code(
    HFONT_T                 h_fnt,
    CHAR_CODE               cc_char_code)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    UINT32          ui4_idx;
    INT32           i4_ret;

    DBG_API(("{FE} has_char_code: handle=%d, cc=%x\n",
             h_fnt, cc_char_code));

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return FALSE;
    }

    _fe_replace_leading_char(&cc_char_code);

    fe_fnt_lock();
#if 0
    ui4_idx = FT_Get_Char_Index(FE_CURRENT_FONT(pt_fnt_obj).pt_face, (FT_ULong)cc_char_code);
#else
    ui4_idx = fe_get_char_index(pt_fnt_obj, cc_char_code, NULL);
#endif
    fe_fnt_unlock();

    if (ui4_idx == 0)
    {
        return FALSE;
    }

    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_kerning
 *
 * Description: This API retrieves the kerning of the specified two character
 *              codes.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          cc_lchar_code       The left character code.
 *          cc_rchar_code       The right character code.
 *
 * Outputs: pi4_x               References the horizontal kerning distance if
 *                              the routine was successful. In all other cases,
 *                              the value of this argument is undetermined.
 *          pi4_y               References the vertical kerning distance if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no kerning information of this font or
 *                              of these two char code.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_NO_CHAR_CODE    left or right char_code do not exist
 *          FER_NO_FONT_CMAP    The font character map is not supported.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_kerning(
    HFONT_T                 h_fnt,
    CHAR_CODE               cc_lchar_code,  /* left char_code                */
    CHAR_CODE               cc_rchar_code,  /* right char_code               */
    INT32*                  pi4_x,          /* return: horizontal kerning    */
    INT32*                  pi4_y)          /* return: vertical kerning      */
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    FT_Error        e_error;
    UINT32          ui4_lidx, ui4_ridx;
    FT_Vector       t_vec;
    INT32           i4_ret;

    DBG_API(("{FE} get_kerning: handle=%d, left=%x, right=%x\n",
             h_fnt, cc_lchar_code, cc_rchar_code));

    if(pi4_x==NULL || pi4_y==NULL)
    {
        return FER_INV_ARG;
    }
	*pi4_x = 0;
	*pi4_y = 0;
    if (_IS_CHINESE_CHARARTER(cc_lchar_code) || _IS_CHINESE_CHARARTER(cc_rchar_code))
    {
        return FER_OK;
    }
    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }

    _fe_replace_leading_char(&cc_lchar_code);
    _fe_replace_leading_char(&cc_rchar_code);

    if ((pt_fnt_obj->i2_fix_h_adv == 0) || (pt_fnt_obj->i2_fix_v_adv == 0))
    {
        UINT8       ui1_lf_id = 0, ui1_rf_id = 0, ui1_ori_id = 0;

        /* user does not set fixed advance */
        fe_fnt_lock(); /* critical section for shared face */
        i4_ret = fe_verify_create_face(pt_fnt_obj);
        if(i4_ret != FER_OK)
        {
            fe_fnt_unlock(); /* critical section for shared face */
            return i4_ret;
        }

    #if 0
        ui4_lidx = FT_Get_Char_Index(FE_CURRENT_FONT(pt_fnt_obj).pt_face, cc_lchar_code);
        ui4_ridx = FT_Get_Char_Index(FE_CURRENT_FONT(pt_fnt_obj).pt_face, cc_rchar_code);
    #else
        ui4_lidx = fe_get_char_index(pt_fnt_obj, cc_lchar_code, &ui1_lf_id);
        ui4_ridx = fe_get_char_index(pt_fnt_obj, cc_rchar_code, &ui1_rf_id);

        if (ui1_lf_id != ui1_rf_id)
        {
            fe_fnt_unlock();
            return FER_NO_INFO;
        }
    #endif
        if(ui4_lidx==0 || ui4_ridx==0)
        {
            fe_fnt_unlock();
            return FER_NO_CHAR_CODE;
        }

        if (ui4_lidx != 0 && ui4_ridx != 0)
        {
            ui1_ori_id = pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;
            pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_lf_id;
        }

        e_error = FT_Get_Kerning(FE_CURRENT_FONT(pt_fnt_obj).pt_face,
                                 ui4_lidx,
                                 ui4_ridx,
                                 FT_KERNING_UNSCALED,
                                 &t_vec);

        if (e_error != 0 && ui4_lidx != 0 && ui4_ridx != 0)
        {
            pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_id;
        }
        if(e_error)
        {
            pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_id;
            fe_fnt_unlock(); /* critical section for shared face */
            return FER_NO_INFO;
        }

        *pi4_x = t_vec.x;
        *pi4_y = t_vec.y;

        /* For scalable font, convert font unit to pixel */
        if(FE_FNT_IS_SCALABLE(FE_CURRENT_FONT(pt_fnt_obj).i4_face_flags))
        {
            *pi4_x = _fe_fnt_unit_to_pixel_x(*pi4_x, pt_fnt_obj);
            *pi4_y = _fe_fnt_unit_to_pixel_y(*pi4_y, pt_fnt_obj);
        }

        if (ui4_lidx != 0 && ui4_ridx != 0)
        {
            pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_id;
        }
    }
    fe_fnt_unlock();

    if (pt_fnt_obj->i2_fix_h_adv != 0)
    {   /* user sets fixed advance */
        *pi4_x = 0;
    }

    if (pt_fnt_obj->i2_fix_v_adv != 0)
    {   /* user sets fixed advance */
        *pi4_y = 0;
    }
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_glyph_advance
 *
 * Description: This API retrieves the glyph advance of the specified character
 *              code.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          cc_char_code        The character code to be refered.
 *
 * Outputs: pi4_hori            References the horizontal glyph advance if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *          pi4_vert            References the vertical glyph advance if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_INV_ARG         The specified char code is invalid.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_NO_CHAR_CODE    The char_code does not exist.
 *          FER_NO_FONT_CMAP    The font character map is not supported.
 *          FER_NO_INFO         Fail to load glyph info.
 ----------------------------------------------------------------------------*/
    INT32 x_fe_get_glyph_advance(
        HFONT_T                 h_fnt,
        CHAR_CODE               cc_char_code,
        INT32*                  pi4_hori,       /* advance for horizontal layout */
        INT32*                  pi4_vert)       /* advance for vertical   layout */
    {
        HANDLE_TYPE_T   e_type;
        FE_FNT_OBJ_T*   pt_fnt_obj;
        UINT32          ui4_idx = 0;
        INT32           i4_ret;
        UINT16          ui2_char_code;
        FE_ADV_CACHE_T  *pt_cache = NULL;
        UINT8           ui1_fidx = 0, ui1_ori_fid = 0;

        DBG_API(("{FE} get_glyph_advance: handle=%d, char=%x\n",
                 h_fnt, cc_char_code));

        if(pi4_hori==NULL || pi4_vert==NULL)
        {
            return FER_INV_ARG;
        }
        i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
        if(i4_ret != FER_OK)
        {
            return i4_ret;
        }
        fe_fnt_lock();
          /* critical section for shared face */
        i4_ret = fe_verify_create_face(pt_fnt_obj);
        if(i4_ret != FER_OK)
        {
            fe_fnt_unlock();
            return i4_ret;
        }
        _fe_replace_leading_char(&cc_char_code);

        ui4_idx = fe_get_char_index(pt_fnt_obj, cc_char_code, &ui1_fidx);

        if (ui4_idx != 0 && _IS_CHINESE_CHARARTER(cc_char_code) && pt_fnt_obj->i4_chinese_character_adv != 0)
        {
            *pi4_hori = (INT32)pt_fnt_obj->i4_chinese_character_adv;
            *pi4_vert = 0;
                fe_fnt_unlock();
            return FER_OK;
        }
        else if (ui4_idx == 0  && cc_char_code != 0)
        {
            if (pt_fnt_obj->pt_fnt_rsc->t_unknown_char.b_usr_set)
            {
                CHAR_CODE   t_replaced_ch = 0;

                if (x_fe_get_unrecognizable_char(h_fnt, &t_replaced_ch) == FER_OK)
                {
                    cc_char_code = (t_replaced_ch & 0xffff);

                    ui4_idx = fe_get_char_index(pt_fnt_obj, cc_char_code, &ui1_fidx);
                    if (ui4_idx == 0 && cc_char_code != 0)
                    {
                        *pi4_hori = 0;
                        *pi4_vert = 0;
                        fe_fnt_unlock();
                        return FER_OK;
                    }
                }
            }
            else
            {
                *pi4_hori = 0;
                *pi4_vert = 0;
                fe_fnt_unlock();
                return FER_OK;
            }
        }
        ui1_ori_fid = pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;


        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_fidx;

        ui2_char_code = (UINT16)(cc_char_code % FE_ADV_CACHE_ENTRY);

        /* check cache */
        {
            pt_cache = &(pt_fnt_obj->pt_adv_cache[ui2_char_code]);

            if (pt_cache->b_available &&
                pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id == pt_cache->ui1_font_id &&
                pt_cache->cc_code == cc_char_code &&
                pt_cache->e_size == pt_fnt_obj->e_size &&
                pt_cache->i4_font_size == FE_CURRENT_FONT(pt_fnt_obj).t_fnt_size[pt_fnt_obj->e_size].ui1_size)
            {
                *pi4_hori = pt_cache->i4_adv_x;
                *pi4_vert = pt_cache->i4_adv_y;
                i4_ret = FER_OK;
                goto ERROR_EXIT;
            }
        }

        /*cache miss*/
        if ((pt_fnt_obj->i2_fix_h_adv == 0) || (pt_fnt_obj->i2_fix_v_adv == 0))
        {
            FE_GLYPH_T * pt_glyph;
            pt_glyph = (FE_GLYPH_T*)x_mem_alloc(sizeof(FE_GLYPH_T));  

            if (pt_glyph == NULL)
            {
                *pi4_hori = 0;
                *pi4_vert = 0;
                i4_ret = FER_NO_MEMORY;
                goto ERROR_EXIT;
            }
            x_memset(pt_glyph, 0, sizeof(FE_GLYPH_T));
            i4_ret = x_fe_load_char(h_fnt, cc_char_code, pt_glyph);

            if (i4_ret == FER_OK)
            {

                *pi4_hori = pt_glyph->t_advance.i4_x >> 6;

                *pi4_vert = pt_glyph->t_advance.i4_y >> 6;

                /*Such as character 0xE34 advence is 0 but glphy left is negative, Width of this character is 0*/
                /*Such as character 'j' advence is 6 but glphy left is -2 , Width of this character is 8*/
                if (pt_glyph->i4_left < 0 && *pi4_hori != 0)
                {
                    *pi4_hori = *pi4_hori - pt_glyph->i4_left;
                }

				if (*pi4_hori < (pt_glyph->i4_left + pt_glyph->t_bmp.i4_width))
				{
					*pi4_hori = pt_glyph->i4_left + pt_glyph->t_bmp.i4_width;
				}

            }
            else
            {
                *pi4_hori = 0;
                *pi4_vert = 0;
                x_mem_free(pt_glyph);
                goto ERROR_EXIT;
            }
            x_mem_free(pt_glyph);
        }

        if (pt_fnt_obj->i2_fix_h_adv != 0)
        {   /* user sets fixed advance */
            *pi4_hori = pt_fnt_obj->i2_fix_h_adv;
        }

        if (pt_fnt_obj->i2_fix_v_adv != 0)
        {   /* user sets fixed advance */
            *pi4_vert = pt_fnt_obj->i2_fix_v_adv;
        }

        /* check cache */
        if (ui4_idx != 0)
        {
            pt_cache = &(pt_fnt_obj->pt_adv_cache[ui2_char_code]);

            pt_cache->b_available = TRUE;
            pt_cache->i4_adv_x    = *pi4_hori;
            pt_cache->i4_adv_y    = *pi4_vert;
            pt_cache->ui1_font_id = pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;
            pt_cache->cc_code     = cc_char_code;
            pt_cache->e_size      = pt_fnt_obj->e_size;
            pt_cache->i4_font_size= FE_CURRENT_FONT(pt_fnt_obj).t_fnt_size[pt_fnt_obj->e_size].ui1_size;
        }

        if (_IS_CHINESE_CHARARTER(cc_char_code) )
        {
            pt_fnt_obj->i4_chinese_character_adv = *pi4_hori;
        }

    ERROR_EXIT:
        /*modify for thai character 0x0E1B and 0x0E34 , 0x0E34 must lefe flush 1 pixel*/
        if (cc_char_code == 0x0E1B)
        {
            *pi4_hori = *pi4_hori + 1;
        }
        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;
        fe_fnt_unlock(); /* critical section for shared face */
        return i4_ret;
    }

    INT32 x_fe_get_glyph_advance_cc(
        HFONT_T                 h_fnt,
        CHAR_CODE               cc_char_code,
        INT32*                  pi4_hori,       /* advance for horizontal layout */
        INT32*                  pi4_vert)       /* advance for vertical   layout */
    {
        HANDLE_TYPE_T   e_type;
        FE_FNT_OBJ_T*   pt_fnt_obj;
        UINT32          ui4_idx = 0;
        INT32           i4_ret;
        UINT16          ui2_char_code;
        FE_ADV_CACHE_T  *pt_cache = NULL;
        UINT8           ui1_fidx = 0, ui1_ori_fid = 0;

        DBG_API(("{FE} get_glyph_advance: handle=%d, char=%x\n",
                 h_fnt, cc_char_code));

        if(pi4_hori==NULL || pi4_vert==NULL)
        {
            return FER_INV_ARG;
        }
        *pi4_hori = 0;
        *pi4_vert = 0;
        i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
        if(i4_ret != FER_OK)
        {
            return i4_ret;
        }
        /* critical section for shared face */
        i4_ret = fe_verify_create_face(pt_fnt_obj);
        if(i4_ret != FER_OK)
        {
            return i4_ret;
        }
        _fe_replace_leading_char(&cc_char_code);

        ui4_idx = fe_get_char_index(pt_fnt_obj, cc_char_code, &ui1_fidx);
        if (ui4_idx == 0  && cc_char_code != 0)
        {
            if (pt_fnt_obj->pt_fnt_rsc->t_unknown_char.b_usr_set)
            {
                CHAR_CODE   t_replaced_ch = 0;

                if (x_fe_get_unrecognizable_char(h_fnt, &t_replaced_ch) == FER_OK)
                {
                    cc_char_code = (t_replaced_ch & 0xffff);

                    ui4_idx = fe_get_char_index(pt_fnt_obj, cc_char_code, &ui1_fidx);
                    if (ui4_idx == 0 && cc_char_code != 0)
                    {
                        return FER_OK;
                    }
                }
            }
            else
            {
                return FER_OK;
            }
        }
        ui1_ori_fid = pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;

        fe_fnt_lock(); /* critical section for shared face */

        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_fidx;

        ui2_char_code = (UINT16)(cc_char_code % FE_ADV_CACHE_ENTRY);

        /* check cache */
        {
            pt_cache = &(pt_fnt_obj->pt_adv_cache[ui2_char_code]);

            if (pt_cache->b_available &&
                pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id == pt_cache->ui1_font_id &&
                pt_cache->cc_code == cc_char_code)
            {
                *pi4_hori = pt_cache->i4_adv_x;
                *pi4_vert = pt_cache->i4_adv_y;
                i4_ret = FER_OK;
                goto ERROR_EXIT;
            }
        }

        /*cache miss*/
        if ((pt_fnt_obj->i2_fix_h_adv == 0) || (pt_fnt_obj->i2_fix_v_adv == 0))
        {
            FE_GLYPH_T * pt_glyph;
            pt_glyph = (FE_GLYPH_T*)x_mem_alloc(sizeof(FE_GLYPH_T));
            if (pt_glyph == NULL)
            {
                i4_ret = FER_NO_MEMORY;
                goto ERROR_EXIT;
            }
            i4_ret = x_fe_load_char(h_fnt, cc_char_code, pt_glyph);

            if (i4_ret == FER_OK)
            {

                *pi4_hori = pt_glyph->t_advance.i4_x >> 6;

                *pi4_vert = pt_glyph->t_advance.i4_y >> 6;

                /*Such as character 0xE34 advence is 0 but glphy left is negative, Width of this character is 0*/
                /*Such as character 'j' advence is 6 but glphy left is -2 , Width of this character is 8*/
                if (pt_glyph->i4_left < 0 && *pi4_hori != 0)
                {
                    *pi4_hori = *pi4_hori - pt_glyph->i4_left;
                }
                else if(pt_glyph->i4_left > 0)
                {
                    *pi4_hori = *pi4_hori + pt_glyph->i4_left;
                }

            }
            else
            {
                x_mem_free(pt_glyph);
                goto ERROR_EXIT;
            }
            x_mem_free(pt_glyph);
        }

        if (pt_fnt_obj->i2_fix_h_adv != 0)
        {   /* user sets fixed advance */
            *pi4_hori = pt_fnt_obj->i2_fix_h_adv;
        }

        if (pt_fnt_obj->i2_fix_v_adv != 0)
        {   /* user sets fixed advance */
            *pi4_vert = pt_fnt_obj->i2_fix_v_adv;
        }

        /* check cache */
        if (ui4_idx != 0)
        {
            pt_cache = &(pt_fnt_obj->pt_adv_cache[ui2_char_code]);

            pt_cache->b_available = TRUE;
            pt_cache->i4_adv_x    = *pi4_hori;
            pt_cache->i4_adv_y    = *pi4_vert;
            pt_cache->ui1_font_id = pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;
            pt_cache->cc_code     = cc_char_code;
        }

    ERROR_EXIT:

        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;
        fe_fnt_unlock(); /* critical section for shared face */
        return i4_ret;
    }


    INT32 x_fe_get_character_size(
        HFONT_T                 h_fnt,
        CHAR_CODE               cc_char_code,
        INT32*                  pi4_width,       /* advance for width layout */
        INT32*                  pi4_height)      /* advance for vertical   layout */
    {
        INT32 i4_ret;

        i4_ret = x_fe_get_glyph_advance(h_fnt, cc_char_code, pi4_width, pi4_height);
        if (i4_ret != FER_OK)
        {
            *pi4_width = 0;
            *pi4_height = 0;
            return i4_ret;
        }
#if 0
        /*In natural, the width of the string is the pixels lay out.
          but in embeding system we can not do this, may be will do it in feature.
        */
        if (ui4_idx == 0 && *pi4_width != 0)
        {
            FE_GLYPH_T*     pt_glyph;
            pt_glyph = (FE_GLYPH_T*)x_mem_alloc(sizeof(FE_GLYPH_T));
            if (pt_glyph == NULL)
            {
                return FER_NO_MEMORY;
            }

            i4_ret = x_fe_load_char(h_fnt, cc_char_code, pt_glyph);
            if (i4_ret != FER_OK)
            {
                    x_mem_free(pt_glyph);
                return i4_ret;
            }

            /*Such as character 'j' advence is 6 but glphy left is -2 , Width of this character is 8*/
                if (pt_glyph->i4_left < 0 )
            {
                *pi4_width = *pi4_width - pt_glyph->i4_left;
            }
            x_mem_free(pt_glyph);
       }
#endif
        return FER_OK;
    }


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_string_size
 *
 * Description: This API calculates the pixel size of the string
 *              (when displayed on screen).
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          pv_str              The string to be evaluated.
 *          i4_length           Maximum length to be evaluated.
 *                              If <0, use whole pv_str. (NULL-terminated)
 *
 * Outputs: pi2_width           References the width of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *          pi2_height          References the height of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_INV_ARG         One or more characters in the string is invalid
 *                              or not in font.
 *          FER_NO_MEMORY       Out of system memory.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_string_size(
    HFONT_T                 h_fnt,
    const VOID*             pv_str,         /* string to be evaluated        */
    INT32                   i4_length,      /* max length to be evaluated    */
    INT32*                  pi4_width,      /* width of string               */
    INT32*                  pi4_height)     /* height of string              */
{
    INT32               i4_ret, i4_i;
    CHAR_CODE           t_ch_prv = 0, t_ch = 0;
    INT32               i4_adv_x = 0, i4_adv_y = 0, i4_kern_x = 0, i4_kern_y = 0;
    INT16               i2_height;
    BOOL                b_has_kern;
    FE_ENCODING_TYPE_T  e_encoding;
    INT32               i4_strlen;

    if (gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    _PERF_START();

    DBG_API(("{FE} get_string_size: handle=%d\n", h_fnt));

    if (pi4_width == NULL || pi4_height == NULL)
    {
        return FER_INV_ARG;
    }

    if (x_fe_get_encoding(h_fnt, &e_encoding) != FER_OK)
    {
        return FER_INV_FONT;
    }

    *pi4_width  = 0;
    *pi4_height = 0;

    i4_strlen = x_fe_strlen((UINT8 *)pv_str, e_encoding, (SIZE_T)i4_length);

    if (i4_length < 0 || i4_length > i4_strlen)
    {
        i4_length = i4_strlen;
    }

    if (i4_length == 0)
    {
        return FER_OK;
    }

    b_has_kern = x_fe_has_kerning(h_fnt);

    for (i4_i = 0; i4_i < i4_length; i4_i++)
    {
        t_ch = x_fe_get_char(pv_str, i4_i, e_encoding);

        if (t_ch == 0)
        {
            break;
        }

        /* adjust kern */
        if (b_has_kern)
        {
            i4_ret = x_fe_get_kerning( h_fnt,
                                       t_ch_prv,
                                       t_ch,
                                       &i4_kern_x,
                                       &i4_kern_y);
            if (i4_ret == FER_OK)
            {
                *pi4_width += i4_kern_x;
            }
        }


           /* count advance */
            i4_ret = x_fe_get_character_size(h_fnt,
                                            t_ch,
                                            &i4_adv_x,
                                            &i4_adv_y);

        if (i4_ret != FER_OK)
        {
            return i4_ret;
        }

        *pi4_width += i4_adv_x;

        /* to be left char */
        t_ch_prv = t_ch;
    }

    i4_ret = x_fe_get_height(h_fnt, &i2_height);

    *pi4_height = (INT32)i2_height;

    if (i4_ret == FER_NO_INFO)
    {
        return FER_INV_ARG;
    }

#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    do {
        HANDLE_TYPE_T   e_type;
        FE_FNT_OBJ_T*   pt_fnt_obj;

        _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);

        *pi4_width += fe_get_width_increased_by_synthesis(pt_fnt_obj, i2_height);
    } while (0);
#endif

    _PERF_END();

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_top_2_down_string_size
 *
 * Description: This API calculates the pixel size of the string
 *              (when displayed on screen) drawn vertically.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          pv_str              The string to be evaluated.
 *          i4_length           Maximum length to be evaluated.
 *                              If <0, use whole pv_str. (NULL-terminated)
 *          aui2_ch_orient_idx  id array for laying-down characters
 *          ui2_ch_orient_num   size of the id array
 *          e_ch_orientation    laying-down orientation
 *
 * Outputs: pi2_width           References the width of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *          pi2_height          References the height of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_INV_ARG         One or more characters in the string is invalid
 *                              or not in font.
 *          FER_NO_MEMORY       Out of system memory.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_top_2_down_string_size(
    HFONT_T                 h_fnt,
    const VOID*             pv_str,              /* string to be evaluated              */
    INT32                   i4_length,           /* max length to be evaluated          */
    UINT16                  *aui2_ch_orient_idx, /* id array for laying-down characters */
    UINT16                  ui2_ch_orient_num,   /* size of the id array                */
    GL_CHAR_ORIENT_T        e_ch_orientation,    /* laying-down orientation             */
    INT32*                  pi4_width,           /* width of string                     */
    INT32*                  pi4_height)          /* height of string                    */
{
    INT32               i4_ret, i4_i;
    CHAR_CODE           t_ch_prv = 0, t_ch = 0;
    INT32               i4_kern_x = 0, i4_kern_y = 0;
    INT16               i2_height, i2_width;
    BOOL                b_has_kern;
    FE_ENCODING_TYPE_T  e_encoding;
    INT32               i4_strlen;

    if (gpt_library == NULL)
    {
        return FER_NOT_INIT;
    }

    _PERF_START();

    DBG_API(("{FE} get_string_size: handle=%d\n", h_fnt));

    if (pi4_width == NULL || pi4_height == NULL)
    {
        return FER_INV_ARG;
    }

    if (x_fe_get_encoding(h_fnt, &e_encoding) != FER_OK)
    {
        return FER_INV_FONT;
    }

    *pi4_width  = 0;
    *pi4_height = 0;

    /* check string length */
    i4_strlen = x_fe_strlen((UINT8 *)pv_str, e_encoding, (SIZE_T)i4_length);

    if (i4_length < 0 || i4_length > i4_strlen)
    {
        i4_length = i4_strlen;
    }

    if (i4_length == 0)
    {
        return FER_OK;
    }

    /* get width */
    i4_ret = x_fe_get_max_advance(h_fnt, &i2_width, &i2_height);

    if (i4_ret != FER_OK)
    {
        return FER_INV_FONT;
    }

    *pi4_width = (INT32)i2_width;

    /* get height */
    i4_ret = x_fe_get_height(h_fnt, &i2_height);

    if (i4_ret == FER_NO_INFO)
    {
        return FER_INV_FONT;
    }

    b_has_kern = x_fe_has_kerning(h_fnt);

    /* for each character */
    for (i4_i = 0; i4_i < i4_length; i4_i++)
    {
        FE_GLYPH_T      t_glyph;

        t_ch = x_fe_get_char(pv_str, i4_i, e_encoding);

        if (t_ch == 0)
        {
            break;
        }

        /* adjust kern */
        if (b_has_kern)
        {
            i4_ret = x_fe_get_kerning(h_fnt, t_ch_prv, t_ch, &i4_kern_x, &i4_kern_y);

            if (i4_ret == FER_OK)
            {
                *pi4_height += i4_kern_y;
            }
        }

        /* get char height */
        i4_ret = x_fe_load_char(h_fnt, t_ch, &t_glyph);

        if (i4_ret != FER_OK)
        {
            return i4_ret;
        }

        *pi4_height += ((INT32)i2_height + t_glyph.t_bmp.i4_height) / 2;

        /* to be left char */
        t_ch_prv = t_ch;
    }

#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    do {
        HANDLE_TYPE_T   e_type;
        FE_FNT_OBJ_T*   pt_fnt_obj;

        _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);

        *pi4_height += fe_get_width_increased_by_synthesis(pt_fnt_obj, i2_height);
    } while (0);
#endif

    _PERF_END();

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_add_width_by_synthesis
 *
 * Description: This API retrieves the width increased after synthesis
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi4_width           References to the width.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_add_width_by_synthesis(HFONT_T h_font, INT32 *pi4_width)
{
    INT32           i4_ret;
    INT16           i2_height;
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T    *pt_fnt_obj;

    if (h_font == NULL_HANDLE || pi4_width == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = x_fe_get_height(h_font, &i2_height);

    if (i4_ret != FER_OK)
    {
        return i4_ret;
    }

    i4_ret = _fe_verify_lib_handle(h_font, &e_type, &pt_fnt_obj);

    if (i4_ret != FER_OK)
    {
        return i4_ret;
    }

    *pi4_width = fe_get_width_increased_by_synthesis(pt_fnt_obj, i2_height);

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_load_char
 *
 * Description: This API retrieves the bitmap of the specified char code.
 *              Note: The pui1_buf of t_bmp which inside pt_glyph is allocated
 *              and maintained by font engine itself. Client need to allocate
 *              FE_GLYPH_T and free it, but not the pui1_buf pointer.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          cc_char_code        The character code to be loaded.
 *
 * Outputs: pt_glyph            References the bitmap if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_INV_ARG         The char code is invalid.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_NO_CHAR_CODE    The char_code does not exist.
 *          FER_NO_FONT_CMAP    The font character map is not supported.
 ----------------------------------------------------------------------------*/

INT32 x_fe_load_char(
    HFONT_T                 h_fnt,
    CHAR_CODE               cc_char_code,
    FE_GLYPH_T*             pt_glyph)
{
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT32           i4_ret;
    CHAR_CODE       t_replaced_ch = 0;
#ifdef FE_DISABLE_CACHE
    FT_Error        e_error;
    FT_GlyphSlot    pt_ft_glyph;
    UINT32          ui4_idx = 0;
    UINT8           ui1_fid = 0, ui1_ori_fid = 0;
#endif

    _PERF_START();

    DBG_API(("{FE} load_char: handle=%d, char=%x\n", h_fnt, cc_char_code));

    if(pt_glyph == NULL)
    {
        i4_ret = FER_INV_ARG;
        goto LOAD_CHAR_EXIT;
    }

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        pt_glyph->i4_left = 0;
        pt_glyph->i4_top  = 0;
        x_memset((VOID*) &pt_glyph->t_bmp, 0, sizeof(FE_BITMAP_T));
        goto LOAD_CHAR_EXIT;
    }

    fe_fnt_lock(); /* critical section for shared face */
    i4_ret = fe_verify_create_face(pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        pt_glyph->i4_left = 0;
        pt_glyph->i4_top  = 0;
        x_memset((VOID*) &pt_glyph->t_bmp, 0, sizeof(FE_BITMAP_T));
        fe_fnt_unlock(); /* critical section for shared face */
        goto LOAD_CHAR_EXIT;
    }

    _fe_replace_leading_char(&cc_char_code);

#ifndef FE_DISABLE_CACHE

    i4_ret = fe_cache_lookup(pt_fnt_obj, cc_char_code, pt_glyph);
    if(i4_ret!=FER_OK && i4_ret!=FER_NO_CHAR_CODE)
    {
        pt_glyph->i4_left = 0;
        pt_glyph->i4_top  = 0;
        x_memset((VOID*) &pt_glyph->t_bmp, 0, sizeof(FE_BITMAP_T));
        fe_fnt_unlock(); /* critical section for shared face */
        goto LOAD_CHAR_EXIT;
    }
    /* replace the unrecognizable character */
    if (i4_ret == FER_NO_CHAR_CODE)
    {
#if !defined(FE_HIDE_UNRECOGNIZABLE_CHAR)
        if (x_fe_get_unrecognizable_char(h_fnt, &t_replaced_ch) == FER_OK)
        {
            cc_char_code = (t_replaced_ch & 0xffff);

            i4_ret = fe_cache_lookup(pt_fnt_obj, cc_char_code, pt_glyph);

            if (i4_ret != FER_OK)
            {
                pt_glyph->i4_left = 0;
                pt_glyph->i4_top  = 0;
                x_memset((VOID*) &pt_glyph->t_bmp, 0, sizeof(FE_BITMAP_T));
                fe_fnt_unlock(); /* critical section for shared face */
				i4_ret =  FER_OK;
                goto LOAD_CHAR_EXIT;
            }
        }
#else
        pt_glyph->i4_left = 0;
        pt_glyph->i4_top  = 0;
        x_memset((VOID*) &pt_glyph->t_bmp, 0, sizeof(FE_BITMAP_T));
        i4_ret =  FER_OK;
#endif
    }
    fe_fnt_unlock(); /* critical section for shared face */

#else

#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    fe_chk_synthesize_style_prolog(pt_fnt_obj);
#endif
    ui4_idx = fe_get_char_index(pt_fnt_obj, cc_char_code, &ui1_fid);

    if (ui4_idx == 0 && cc_char_code != 0 && pt_fnt_obj->pt_fnt_rsc->t_unknown_char.b_usr_set)
    {
         if (x_fe_get_unrecognizable_char(h_fnt, &t_replaced_ch) == FER_OK)
        {
            cc_char_code = (t_replaced_ch & 0xffff);
            ui4_idx = fe_get_char_index(pt_fnt_obj, cc_char_code, &ui1_fid);
        }
    }
#ifdef FE_HIDE_UNRECOGNIZABLE_CHAR
    if (ui4_idx == 0 && cc_char_code != 0)
    {
        pt_glyph->i4_left = 0;
        pt_glyph->i4_top  = 0;
        x_memset((VOID*) &pt_glyph->t_bmp, 0, sizeof(FE_BITMAP_T));
        fe_fnt_unlock(); /* critical section for shared face */
        i4_ret = FER_OK;
        goto LOAD_CHAR_EXIT;
    }
#endif
    ui1_ori_fid = pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;
    pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_fid;
    e_error = FT_Load_Char(FE_CURRENT_FONT(pt_fnt_obj).pt_face,
                           cc_char_code,
                           (pt_glyph->e_render_mode & FE_RENDER_MODE_MONO)?
                                (FT_LOAD_RENDER | FT_LOAD_MONOCHROME):
                                FT_LOAD_RENDER);

    if(e_error)
    {

        i4_ret = FER_INTERNAL_ERR;
        /* Pass through to show an empty square as bitmap */
        pt_glyph->i4_left = 0;
        pt_glyph->i4_top  = 0;
        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;
        x_memset((VOID*) &pt_glyph->t_bmp, 0, sizeof(FE_BITMAP_T));
        fe_fnt_unlock(); /* critical section for shared face */
        goto LOAD_CHAR_EXIT;

    }
    pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;
    fe_fnt_unlock(); /* critical section for shared face */

    pt_ft_glyph = FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph;

#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    fe_chk_synthesize_style_epilog(pt_fnt_obj);
#endif

#ifndef FE_DISABLE_EDGE_EFFECT_SUPPORT
    fe_synthesize_edge_effect(pt_fnt_obj);
#endif

    pt_glyph->i4_left         = pt_ft_glyph->bitmap_left;
    pt_glyph->i4_top          = pt_ft_glyph->bitmap_top;
    pt_glyph->t_bmp.i4_height =(INT32)(pt_ft_glyph->bitmap.rows);
    pt_glyph->t_bmp.i4_width  =(INT32)(pt_ft_glyph->bitmap.width);
    pt_glyph->t_bmp.i4_pitch  =(INT32)(pt_ft_glyph->bitmap.pitch);
    pt_glyph->t_bmp.e_mode    =(FE_PIXEL_MODE_T)(pt_ft_glyph->bitmap.pixel_mode);
    pt_glyph->t_bmp.pui1_buf  =(UINT8*)(pt_ft_glyph->bitmap.buffer);

#endif

LOAD_CHAR_EXIT:
    _PERF_END();

    return i4_ret;
}

INT32 x_fe_set_rounding_type(
    HFONT_T                 h_fnt,
    FE_ROUNDING_TYPE_T      e_rnd_type)
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T    *pt_fnt_obj;

    if (h_fnt == NULL_HANDLE)
    {
        return FER_INV_HANDLE;
    }

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);

    if (i4_ret == FER_OK)
    {
        pt_fnt_obj->e_rnd_type = e_rnd_type;
    }

    return i4_ret;
}

INT32 x_fe_get_rounding_type(
    HFONT_T                 h_fnt,
    FE_ROUNDING_TYPE_T      *pe_rnd_type)
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T    *pt_fnt_obj;

    if (h_fnt == NULL_HANDLE)
    {
        return FER_INV_HANDLE;
    }

    if (pe_rnd_type == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);

    if (i4_ret == FER_OK)
    {
        *pe_rnd_type = pt_fnt_obj->e_rnd_type;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_char_range
 *
 * Description: This function gets the character code range of the current character amp.
 *
 * Inputs:  h_fnt               Font handle.
 *          at_char_code[2]     The array to store the character code range.
 *
 * Outputs: b_available         If this is TRUE, the range is specified in at_char_code[].
 *                              Otherwise, this information is not available.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 *          FER_INV_ARG         Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_char_range(
    HFONT_T     h_fnt,
    CHAR_CODE   at_char_code[2],
    BOOL        *pb_available)
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T    *pt_fnt_obj;
    FT_ULong        char_code;
    FT_UInt         index;

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);

    if (i4_ret != FER_OK)
    {
        return FER_INV_HANDLE;
    }

    if (at_char_code == NULL || pb_available == NULL)
    {
        return FER_INV_ARG;
    }

    _fe_replace_leading_char(&at_char_code[0]);
    _fe_replace_leading_char(&at_char_code[1]);

    fe_fnt_lock();

    char_code = FT_Get_First_Char(FE_CURRENT_FONT(pt_fnt_obj).pt_face, &index);

    if (index == 0)
    {
        *pb_available = FALSE;
        fe_fnt_unlock();

        return FER_OK;
    }

    *pb_available = TRUE;

    at_char_code[0] = char_code;

    while (index != 0)
    {
        at_char_code[1] = char_code;
        char_code = FT_Get_Next_Char(FE_CURRENT_FONT(pt_fnt_obj).pt_face, char_code, &index);
    }

    fe_fnt_unlock();

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_delete_font
 *
 * Description: This function deletes a font object.
 *
 * Inputs:  h_fnt               Font handle.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_delete_font(
    HFONT_T     h_fnt)
{
    INT32   i4_ret;

    i4_ret = FER_INV_HANDLE;

    if (HR_OK == handle_free(h_fnt, TRUE))
    {
        i4_ret = FER_OK;
    }
    else
    {
        DBG_ERROR(("{Font} ERR: Invalid handle (%d) at L%d\r\n\r\n", i4_ret, __LINE__));
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_strlen
 *
 * Description: This function retrieves the length of a string with a specific
 *              encoding.
 *
 * Inputs:  pui1_str            Pointer to the string.
 *          e_encoding          Encoding of this string.
 *
 * Outputs: -
 *
 * Returns: The string length.
 ----------------------------------------------------------------------------*/
SIZE_T x_fe_strlen(const UINT8 *pui1_str, FE_ENCODING_TYPE_T e_encoding, SIZE_T z_max_size)
{
    if (z_max_size <= 0)
    {
        if (e_encoding == FE_ENCODING_UCS2)
        {
            return x_uc_w2s_strlen((UTF16_T *)pui1_str);
        }
        else if (e_encoding == FE_ENCODING_ASCII)
        {
            return x_strlen((CHAR *)pui1_str);
        }
        else
        {
            return fe_strlen(pui1_str, e_encoding);
        }
    }
    else
    {
        SIZE_T z_size = 0;
        while (x_fe_get_char(pui1_str,z_size,e_encoding)!= (CHAR_CODE)(0) && z_size != z_max_size)
        {
            z_size ++;
        }
        return z_size;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_char
 *
 * Description: This function retrieves the character code of a specific character.
 *              of a string.
 *
 * Inputs:  pui1_str            Pointer to the string.
 *          ui4_index           index of the character.
 *          e_encoding          Encoding of this string.
 *
 * Outputs: -
 *
 * Returns: The character code.
 ----------------------------------------------------------------------------*/
CHAR_CODE x_fe_get_char(const UINT8 *pui1_str, UINT32 ui4_index, FE_ENCODING_TYPE_T e_encoding)
{
    if (e_encoding == FE_ENCODING_UCS2)
    {
        return (CHAR_CODE)x_uc_w2s_get((UTF16_T *)pui1_str, ui4_index);
    }
    else if (e_encoding == FE_ENCODING_ASCII)
    {
        return ((pui1_str == NULL)? 0: pui1_str[ui4_index]);
    }
    else
    {
        return fe_get_char(pui1_str, ui4_index, e_encoding);
    }
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_encoding
 *
 * Description: This function sets the encoding of this font
 *
 * Inputs:  h_font              font handle
 *          e_encoding          encoding specified
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_encoding(
    HFONT_T             h_font,
    FE_ENCODING_TYPE_T  e_encoding)
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T    *pt_fnt_obj;

    if (h_font == NULL_HANDLE)
    {
        return FER_INV_HANDLE;
    }

    i4_ret = _fe_verify_lib_handle(h_font, &e_type, &pt_fnt_obj);

    if (i4_ret == FER_OK)
    {
        pt_fnt_obj->e_encoding = e_encoding;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_encoding
 *
 * Description: This function gets the encoding of this font
 *
 * Inputs:  h_font              font handle
 *
 * Outputs: pe_encoding         font encoding
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_encoding(
    HFONT_T             h_font,
    FE_ENCODING_TYPE_T  *pe_encoding)
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T    *pt_fnt_obj;

    if (h_font == NULL_HANDLE)
    {
        return FER_INV_HANDLE;
    }

    if (pe_encoding== NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle(h_font, &e_type, &pt_fnt_obj);

    if (i4_ret == FER_OK)
    {
        *pe_encoding = pt_fnt_obj->e_encoding;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_unrecognizable_char
 *
 * Description: This function sets the character to be displayed when a unrecognizable character is met
 *
 * Inputs:  h_font              font handle
 *          w2_ch               desired character
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_unrecognizable_char(
    HFONT_T             h_font,
    CHAR_CODE           t_ch)
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T    *pt_fnt_obj;

    if (h_font == NULL_HANDLE)
    {
        return FER_INV_HANDLE;
    }

    i4_ret = _fe_verify_lib_handle(h_font, &e_type, &pt_fnt_obj);

    if (i4_ret == FER_OK)
    {
        if(pt_fnt_obj->pt_fnt_rsc->t_unknown_char.b_usr_set != TRUE)
        {
            pt_fnt_obj->pt_fnt_rsc->t_unknown_char.b_usr_set    = TRUE;
            pt_fnt_obj->pt_fnt_rsc->t_unknown_char.t_display_ch = t_ch;/*Global unkown char*/
        }
        pt_fnt_obj->t_unknown_char.b_usr_set    = TRUE;
        pt_fnt_obj->t_unknown_char.t_display_ch = t_ch;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_unrecognizable_char
 *
 * Description: This function sets the character to be displayed when a unrecognizable character is met
 *
 * Inputs:  h_font              font handle
 *
 * Outputs: pw2_ch              pointer to the desired character
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_unrecognizable_char(
    HFONT_T             h_font,
    CHAR_CODE           *pt_ch)
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T    *pt_fnt_obj;

    if (h_font == NULL_HANDLE || pt_ch == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle(h_font, &e_type, &pt_fnt_obj);

    if (i4_ret == FER_OK)
    {
        if (!pt_fnt_obj->t_unknown_char.b_usr_set && !pt_fnt_obj->pt_fnt_rsc->t_unknown_char.b_usr_set)
        {
            i4_ret = FER_NO_INFO;
        }
        else if (pt_fnt_obj->t_unknown_char.b_usr_set)
        {
            *pt_ch = pt_fnt_obj->t_unknown_char.t_display_ch;
        }
        else
        {
            *pt_ch = pt_fnt_obj->pt_fnt_rsc->t_unknown_char.t_display_ch;
        }
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_bld_width
 *
 * Description: This API sets the blurred width of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          ui2_width           Contains the width of the uniform
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_blurred_width(
    HFONT_T                 h_fnt,
    UINT16                  ui2_width)
{

#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
    INT16           i2_height;
    UINT16          ui2_max_bld_w;
    INT32           i4_ret;

    if (h_fnt == NULL_HANDLE)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }
    if(!(pt_fnt_obj->e_style & FE_FNT_STYLE_BLURRED))
    {
        return FER_INV_ARG;
    }
    i4_ret = x_fe_get_height(h_fnt,&i2_height);
    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }
    ui2_max_bld_w = (i2_height > 10)?(i2_height / 10): 1;
    if(ui2_max_bld_w > 8)
    {
        ui2_max_bld_w = 8;
    }
    if(ui2_width < ui2_max_bld_w && ui2_width > 0)
    {
        ui2_max_bld_w = ui2_width;
    }
    pt_fnt_obj->ui2_bld_w = (UINT16)ui2_max_bld_w;
#endif

    return FER_OK;
}

INT32 x_fe_cache_flush(VOID)
{
    fe_cache_flush();
    return FER_OK;
}

INT32 x_fe_preload_char(
    HFONT_T                     h_fnt,
    FE_RENDER_MODE_T            e_render_mode,
    UTF16_T*                    w2s_preload_string,
    UINT32                      ui4_str_size,
    UINT32*                     pui4_str_size_loaded)
{
    HANDLE_TYPE_T               e_type;
    FE_FNT_OBJ_T*               pt_fnt_obj;
    INT32                       i4_ret;
    UINT32                      i;

    if (h_fnt == NULL_HANDLE || w2s_preload_string == NULL || ui4_str_size == 0 || pui4_str_size_loaded == NULL)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
        return i4_ret;
    }

    ui4_str_size = MIN((UINT32)(x_uc_w2s_strlen(w2s_preload_string)), ui4_str_size);

    for(i = 0; i < ui4_str_size; ++i)
    {
        i4_ret = fe_cache_preload(pt_fnt_obj, w2s_preload_string[i], e_render_mode);
        if (i4_ret != FER_OK)
        {
            ++i;
            *pui4_str_size_loaded = i;
            return i4_ret;
        }
    }

    *pui4_str_size_loaded = ui4_str_size;

    return FER_OK;
}

INT32	x_fe_get_size_from_height(
					HFONT_T		h_fnt, 
					UINT16		ui2_height,
					UINT8*		pui1_point_size)
{
	HANDLE_TYPE_T   e_type;
    FE_FNT_OBJ_T*   pt_fnt_obj;
	INT32			i4_em_size;
	INT32			i4_ret;
	INT16			i2_font_height;

    DBG_API(("{FE} get_size_from_height: handle=%d\n", h_fnt));

    if(ui2_height == 0)
    {
        return FER_INV_ARG;
    }

    i4_ret = _fe_verify_lib_handle_scale(h_fnt, &e_type, &pt_fnt_obj);
    if(i4_ret != FER_OK)
    {
    	return i4_ret;
    }
	
	i4_em_size     = FE_CURRENT_FONT(pt_fnt_obj).ui2_units_per_EM;
	i2_font_height = FE_CURRENT_FONT(pt_fnt_obj).i2_height;

    /* 72 is the fixed constant dpi, NEVER to be changed */
    /* Todo: how to avoid overrun? */
    *pui1_point_size = (UINT8)FE_DIV_CEIL(ui2_height * i4_em_size * (INT32)72,
    							i2_font_height * (INT32)gui4_vert_dpi,
        						pt_fnt_obj->e_rnd_type);
	
    return FER_OK;
}

