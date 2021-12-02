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
 * $RCSfile: fe.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Ben Tsai $
 * $MD5HEX: ba95d506110e28c6edc01426fba96aa8 $
 *
 * Description:
 *         This header file contains font library specific functions,
 *         which are private.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "font/fe.h"
#include "dbg/def_dbg_level_mw.h"


#ifndef FE_DISABLE_CACHE
#include "font/cache.h"
#endif /*FE_DISABLE_CACHE*/

#if !defined(FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT) && !defined(FE_DISABLE_EDGE_EFFECT_SUPPORT)
#include FT_SYNTHESIS_H
#include "font/ft_wrapper/ft_synth.h"
#endif

#include "perf.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define FE_FNT_IS_SCALABLE(flags)   ( (flags) & FT_FACE_FLAG_SCALABLE )
#define FE_CURRENT_RSC(pt_rsc)                                                          \
    ((pt_rsc)->at_font_data[((pt_rsc)->ui1_current_font_id < FE_MAX_FONT_FILE_COUNT)?   \
        (pt_rsc)->ui1_current_font_id: 0])

#define FE_CHECK_FONT_ID(id)        (((id) < FE_MAX_FONT_FILE_COUNT)? (id): 0)

#ifdef DEBUG
typedef struct _FE_MEM_RECORD
{
    SLIST_ENTRY_T(_FE_MEM_RECORD)   t_mem_record_lnk;
    VOID*   pv_buf;
    SIZE_T  z_size;
} FE_MEM_RECORD_T;
#endif


/*-----------------------------------------------------------------------------
                    static variables
 ----------------------------------------------------------------------------*/
SLIST_T(_FE_FNT_RSC_T)          gt_fnt_rsc_q;   /* available font list       */

static HANDLE_T                 h_fnt_rsc_q_lock = NULL_HANDLE;
static HANDLE_T                 h_fnt_lock = NULL_HANDLE;
static FE_FNT_RSC_T*            gpt_dfl_fnt;    /* system default font       */
static BOOL                     b_fe_init = FALSE;
static UINT32                   gui4_fhdl_num = 0; /* font handle numbers    */


#ifndef FE_INIT_DBG_LEVEL
#define FE_INIT_DBG_LEVEL       0
#endif
static UINT16                   ui2_fe_dbg_level = DBG_INIT_LEVEL_MW_FONT;

#ifdef DEBUG
static SIZE_T                   z_mem_usage;      /* system memory usage   */
static SLIST_T(_FE_MEM_RECORD)  gt_mem_record_q;    /* memory usage record   */
#endif


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static FE_FNT_STYLE _fe_chk_rsc_style(
    FE_FNT_STYLE        e_style);

/*
static BOOL _fe_del_font_rsc(
    const CHAR*         ps_font,
    FE_FNT_STYLE        e_style);
*/

static VOID _fe_free_fnt_rsc(
    FE_FNT_RSC_T*       pt_rsc);


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  fe_free_handle_cb
 *
 * Description: Call back function (of handle) to free the font object.
 *
 ----------------------------------------------------------------------------*/
BOOL fe_free_handle_cb(
    HFONT_T             h_fnt,
    HANDLE_TYPE_T       e_type,
    VOID*               pv_obj,
    VOID*               pv_tag,
    BOOL                b_req_handle)
{
    FE_FNT_OBJ_T*   pt_fnt_obj;

    FNT_ASSERT(e_type == FE_FNT_OBJ);
    FNT_ASSERT(pv_obj != NULL);

    pt_fnt_obj = (FE_FNT_OBJ_T*)pv_obj;

    /* remove from font object list */
    fe_fnt_rsc_lock ();
    if (pt_fnt_obj->pt_adv_cache)
    {
        FE_FREE(pt_fnt_obj->pt_adv_cache);
    }
    SLIST_REMOVE(pt_fnt_obj, t_obj_lnk);
    fe_fnt_rsc_unlock ();

    FE_FREE(pt_fnt_obj);

    fe_fnt_stat_del();

    return (TRUE);
}


/*-----------------------------------------------------------------------------
 * Name: fe_fnt_stat_add
 *
 * Description: API to count for adding new font handle
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fe_fnt_stat_add(
    VOID)
{
    fe_fnt_lock();
    ++ gui4_fhdl_num;
    fe_fnt_unlock();
}


/*-----------------------------------------------------------------------------
 * Name: fe_fnt_stat_del
 *
 * Description: API to count for removing font handle
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fe_fnt_stat_del(
    VOID)
{
    fe_fnt_lock();
    -- gui4_fhdl_num;
    fe_fnt_unlock();
}


/*-----------------------------------------------------------------------------
 * Name: fe_fnt_stat_del
 *
 * Description: API to count for removing font handle
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
UINT32 fe_fnt_stat_get(
    VOID)
{
    return gui4_fhdl_num;
}


/*-----------------------------------------------------------------------------
 * Name: fe_fnt_rsc_lock
 *
 * Description: API to lock the font resource list
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fe_fnt_rsc_lock(
    VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_lock( h_fnt_rsc_q_lock, X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT(DBG_MOD_FONT);
    }
}


/*-----------------------------------------------------------------------------
 * Name: fe_fnt_rsc_unlock
 *
 * Description: API to unlock the font resource list.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fe_fnt_rsc_unlock(
    VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock( h_fnt_rsc_q_lock );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT(DBG_MOD_FONT);
    }
}


/*-----------------------------------------------------------------------------
 * Name: fe_fnt_lock
 *
 * Description: API to lock the font api
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fe_fnt_lock(
    VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_lock( h_fnt_lock, X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT(DBG_MOD_FONT);
    }
}


/*-----------------------------------------------------------------------------
 * Name: fe_fnt_unlock
 *
 * Description: API to unlock the font api.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fe_fnt_unlock(
    VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock( h_fnt_lock );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT(DBG_MOD_FONT);
    }
}


/*-----------------------------------------------------------------------------
 * Name:  fe_init_prv
 *
 * Description: Initialize font resource list.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: FER_OK              Success.
 *          FER_INTERNAL_ERR    System internal error.
 ----------------------------------------------------------------------------*/
INT32 fe_init_prv(
    VOID)
{
    INT32   i4_ret;

    i4_ret = x_sema_create( &h_fnt_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret!=OSR_OK || h_fnt_lock==NULL_HANDLE )
    {
        return FER_INTERNAL_ERR;
    }

    i4_ret = x_sema_create( &h_fnt_rsc_q_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret!=OSR_OK || h_fnt_rsc_q_lock==NULL_HANDLE )
    {
        x_sema_delete(h_fnt_lock);
        return FER_INTERNAL_ERR;
    }

    SLIST_INIT(&gt_fnt_rsc_q);

#ifndef FE_DISABLE_CACHE

    i4_ret = fe_cache_init();
    if(i4_ret != FER_OK)
    {
        x_sema_delete(h_fnt_lock);
        x_sema_delete(h_fnt_rsc_q_lock);
        h_fnt_rsc_q_lock = NULL_HANDLE;
    }

#endif

#ifdef DEBUG
    /* initialize memory usage monitor */
    z_mem_usage = 0;
    SLIST_INIT(&gt_mem_record_q);
#endif

    b_fe_init = TRUE;

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  fe_done_prv
 *
 * Description: Clean up font resources.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fe_done_prv(
    VOID)
{
    FE_FNT_RSC_T*   pt_rsc;

    while( (pt_rsc=SLIST_FIRST(&gt_fnt_rsc_q)) != NULL )
    {
        SLIST_REMOVE(pt_rsc, t_rsc_lnk);
        _fe_free_fnt_rsc(pt_rsc);
    }

    if(h_fnt_rsc_q_lock != NULL_HANDLE)
    {
        x_sema_delete(h_fnt_rsc_q_lock);
        h_fnt_rsc_q_lock = NULL_HANDLE;
    }

    if(h_fnt_lock != NULL_HANDLE)
    {
        x_sema_delete(h_fnt_lock);
        h_fnt_lock = NULL_HANDLE;
    }

#ifndef FE_DISABLE_CACHE
    /* free cache */
    fe_cache_done();
#endif

    b_fe_init = FALSE;
}


/*-----------------------------------------------------------------------------
 * Name:  _fe_free_fnt_rsc
 *
 * Description: Delete specified font resource from list, and free the memory.
 *
 * Inputs:  pt_rsc              Font resource object.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _fe_free_fnt_rsc(
    FE_FNT_RSC_T*       pt_rsc)
{
    UINT8               ui1_i;

    FNT_ASSERT(pt_rsc != NULL);

    /* if the font to be removed is the default font, change default */
    if(gpt_dfl_fnt == pt_rsc)
    {
        gpt_dfl_fnt = SLIST_FIRST(&gt_fnt_rsc_q);
    }

    for (ui1_i = 0; ui1_i < pt_rsc->ui1_font_number; ui1_i++)
    {
        FE_PER_FNT_RSC_T    *pt_per_rsc = &pt_rsc->at_font_data[FE_CHECK_FONT_ID(ui1_i)];

        if(pt_per_rsc->pt_face)
        {
            FT_Done_Face(pt_per_rsc->pt_face);
        }

        if(pt_rsc->ps_fnt_name)
        {
            FE_FREE(pt_rsc->ps_fnt_name);
            pt_rsc->ps_fnt_name = NULL;
        }
        if(pt_per_rsc->ps_file_name)
        {
            FE_FREE(pt_per_rsc->ps_file_name);
            pt_per_rsc->ps_file_name = NULL;
        }
        if(pt_per_rsc->i4_num_cmaps && pt_per_rsc->pe_cmaps)
        {
            FE_FREE(pt_per_rsc->pe_cmaps);
            pt_per_rsc->i4_num_cmaps = 0;
            pt_per_rsc->pe_cmaps = NULL;
        }
    }

    FE_FREE(pt_rsc);
}


/*-----------------------------------------------------------------------------
 * Name:  fe_fnt_file_exist
 *
 * Description: Check if the font file exists.
 *
 * Inputs:  pt_file             Font file name.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font file exists.
 *          FALSE               Otherwise
 ----------------------------------------------------------------------------*/
BOOL fe_fnt_file_exist(
    const CHAR*         ps_file)
{
    /* ToDo: do check the file */
    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name:  _fe_has_fixed_size_of
 *
 * Description: Check if font contains specifed fixed size bitmap.
 *
 * Inputs:  pt_face             Font face object.
 *          ui1_size            Font size.
 *
 * Outputs: -
 *
 * Returns: TRUE                Has the specified fixed font size bitmap.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL _fe_has_fixed_size_of(
    const FT_Face       pt_face,
    UINT8               ui1_size)
{
    INT32   i4_i;

    FNT_ASSERT(pt_face != NULL);

    for(i4_i=0; i4_i<pt_face->num_fixed_sizes; i4_i++)
    {
        if(pt_face->available_sizes[i4_i].height == (FT_Short)ui1_size)
        {
            return TRUE;
        }
    }

    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name:  fe_size_supported
 *
 * Description: Check if the specified font sizes are all supported.
 *
 * Inputs:  pt_face             Font face object.
 *          ui1_small           Size of small font.
 *          ui1_medium          Size of medium font.
 *          ui1_large           Size of large font.
 *
 * Outputs: -
 *
 * Returns: TRUE                All font sizes are supported.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL fe_size_supported(
    const FT_Face       pt_face,
    UINT8               ui1_small,
    UINT8               ui1_medium,
    UINT8               ui1_large)
{
    FNT_ASSERT(pt_face != NULL);

    if( FT_IS_SCALABLE( pt_face )                ||
        ( _fe_has_fixed_size_of(pt_face, ui1_small)  &&
          _fe_has_fixed_size_of(pt_face, ui1_medium) &&
          _fe_has_fixed_size_of(pt_face, ui1_large) ) )
    {
        return TRUE;
    }

    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name:  fe_add_fnt_rsc
 *
 * Description: Add a font resource into list.
 *
 * Inputs:  pt_face             Font face object.
 *          pt_rsc              Font resource object.
 *          pt_font             Font name.
 *          pt_file             Font file name.
 *          pui1_fontdata       Font data in memory.
 *          ui4_datasize        Font data size.
 *          ui1_small           Size of small font.
 *          ui1_medium          Size of medium font.
 *          ui1_large           Size of large font.
 *
 * Outputs: -
 *
 * Returns: FER_OK              Font resource is added successfully.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_FONT_EXIST      The same font existed.
 ----------------------------------------------------------------------------*/
INT32 fe_add_fnt_rsc(
    const FT_Face       pt_face,
    FE_FNT_RSC_T*       pt_rsc,
    const CHAR*         ps_font,
    const CHAR*         ps_file,
    const UINT8*        pui1_fontdata,
    UINT32              ui4_datasize,
    UINT8               ui1_small,
    UINT8               ui1_medium,
    UINT8               ui1_large)
{
    INT32               i4_i;
    UINT32              ui4_size;

    FNT_ASSERT(pt_face != NULL);
    FNT_ASSERT(pt_rsc  != NULL);
    FNT_ASSERT(ps_font != NULL);
    FNT_ASSERT(ps_file != NULL || pui1_fontdata != NULL);
    FNT_ASSERT(ps_file == NULL || pui1_fontdata == NULL);

#if 0
    /* clear pt_rsc data first */
    FE_MEMSET(pt_rsc, 0, sizeof(FE_FNT_RSC_T));
#endif

    /* allocate all needed memory */
    if(ps_file != NULL)
    {
         FE_CURRENT_RSC(pt_rsc).ps_file_name = (CHAR*)FE_ALLOC(FE_STRLEN(ps_file)+1);
    }
    else
    {
         FE_CURRENT_RSC(pt_rsc).ps_file_name = NULL;
    }
    pt_rsc->ps_fnt_name = (CHAR*)FE_ALLOC(FE_STRLEN(ps_font)+1);
    ui4_size = pt_face->num_charmaps * sizeof(FE_CMAP_ENCODING);
    FE_CURRENT_RSC(pt_rsc).pe_cmaps = (FE_CMAP_ENCODING*)FE_ALLOC(ui4_size);
    if( (ps_file!=NULL && FE_CURRENT_RSC(pt_rsc).ps_file_name==NULL) ||
        pt_rsc->ps_fnt_name==NULL  ||
        FE_CURRENT_RSC(pt_rsc).pe_cmaps==NULL )
    {
        if(pt_rsc->ps_fnt_name)
        {
            FE_FREE(pt_rsc->ps_fnt_name);
        }
        if( FE_CURRENT_RSC(pt_rsc).ps_file_name)
        {
            FE_FREE( FE_CURRENT_RSC(pt_rsc).ps_file_name);
        }
        if(FE_CURRENT_RSC(pt_rsc).pe_cmaps)
        {
            FE_FREE(FE_CURRENT_RSC(pt_rsc).pe_cmaps);
        }
        return FER_NO_MEMORY;
    }

    /* create face only when needed */
    FE_CURRENT_RSC(pt_rsc).pt_face = NULL;

    /* copy font file name */
    if( FE_CURRENT_RSC(pt_rsc).ps_file_name != NULL && ps_file != NULL)
    {
        FE_STRCPY( FE_CURRENT_RSC(pt_rsc).ps_file_name, ps_file);
    }

    /* copy face index */
    FE_CURRENT_RSC(pt_rsc).ui1_face_idx = (UINT8)pt_face->face_index;

    /* copy font name */
    FE_STRCPY(pt_rsc->ps_fnt_name, ps_font);

#ifdef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    /* copy style flag */
    if( pt_face->style_flags & FT_STYLE_FLAG_ITALIC )
    {
        FE_CURRENT_RSC(pt_rsc).e_style |= FE_FNT_STYLE_ITALIC;
    }
    if( pt_face->style_flags & FT_STYLE_FLAG_BOLD )
    {
        FE_CURRENT_RSC(pt_rsc).e_style |= FE_FNT_STYLE_BOLD;
    }
    if( pt_rsc->e_style == 0 )  /* neither italic nor bold */
    {
        FE_CURRENT_RSC(pt_rsc).e_style = FE_FNT_STYLE_REGULAR;
    }
#else
    pt_rsc->e_style = FE_FNT_STYLE_REGULAR;
#endif /*FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT*/

    /* determine font size */
    FE_CURRENT_RSC(pt_rsc).t_fnt_size[FE_FNT_SIZE_SMALL].ui1_size  = ui1_small;
    FE_CURRENT_RSC(pt_rsc).t_fnt_size[FE_FNT_SIZE_MEDIUM].ui1_size = ui1_medium;
    FE_CURRENT_RSC(pt_rsc).t_fnt_size[FE_FNT_SIZE_LARGE].ui1_size  = ui1_large;

    FE_CURRENT_RSC(pt_rsc).t_fnt_size[FE_FNT_SIZE_SMALL].e_unit  =
        _fe_has_fixed_size_of(pt_face, ui1_small)  ?
            FE_SIZE_UNIT_PIXEL : FE_SIZE_UNIT_POINT;
    FE_CURRENT_RSC(pt_rsc).t_fnt_size[FE_FNT_SIZE_MEDIUM].e_unit =
        _fe_has_fixed_size_of(pt_face, ui1_medium) ?
            FE_SIZE_UNIT_PIXEL : FE_SIZE_UNIT_POINT;
    FE_CURRENT_RSC(pt_rsc).t_fnt_size[FE_FNT_SIZE_LARGE].e_unit  =
        _fe_has_fixed_size_of(pt_face, ui1_large)  ?
            FE_SIZE_UNIT_PIXEL : FE_SIZE_UNIT_POINT;

    /* copy face attributes */
    FE_CURRENT_RSC(pt_rsc).i4_face_flags    = pt_face->face_flags;
    FE_CURRENT_RSC(pt_rsc).i2_ascender      = pt_face->ascender;
    FE_CURRENT_RSC(pt_rsc).i2_descender     = pt_face->descender;
    FE_CURRENT_RSC(pt_rsc).i2_height        = pt_face->height;
    FE_CURRENT_RSC(pt_rsc).i2_udl_pos       = pt_face->underline_position;
    FE_CURRENT_RSC(pt_rsc).i2_udl_thk       = pt_face->underline_thickness;
    FE_CURRENT_RSC(pt_rsc).ui2_units_per_EM = pt_face->units_per_EM;
    FE_CURRENT_RSC(pt_rsc).i2_max_adv_w     = pt_face->max_advance_width;
    FE_CURRENT_RSC(pt_rsc).i2_max_adv_h     = pt_face->max_advance_height;

    /* Do not allow values of these attributes be zeros. Some patches to the FreeType
       might be needed, especially the bitmap (e.g. BDF) libraries. */
    FNT_ASSERT(FE_CURRENT_RSC(pt_rsc).i2_ascender != 0);
    FNT_ASSERT(FE_CURRENT_RSC(pt_rsc).i2_height != 0);

    /* copy charmap */
    FE_CURRENT_RSC(pt_rsc).i4_num_cmaps = pt_face->num_charmaps;
    for(i4_i=0; i4_i<pt_face->num_charmaps; i4_i++)
    {
        FE_CURRENT_RSC(pt_rsc).pe_cmaps[i4_i] =
            (FE_CMAP_ENCODING)(pt_face->charmaps[i4_i]->encoding);
    }

    /* set font resource type */
    if(ps_file != NULL)
    {
        pt_rsc->t_fnt_rsc_type               = FE_FNT_RSC_TYPE_FILE;
        FE_CURRENT_RSC(pt_rsc).pui1_fnt_data = NULL;
        FE_CURRENT_RSC(pt_rsc).ui4_rsc_size  = 0;
    }
    else
    {
        pt_rsc->t_fnt_rsc_type               = FE_FNT_RSC_TYPE_MEM;
        FE_CURRENT_RSC(pt_rsc).pui1_fnt_data = (UINT8 *)pui1_fontdata;
        FE_CURRENT_RSC(pt_rsc).ui4_rsc_size  = ui4_datasize;
    }

    /* init object link */
    SLIST_INIT(&pt_rsc->t_obj_link_head);

    /* add the font if it's new */
    if(fe_find_font(pt_rsc->ps_fnt_name, pt_rsc->e_style) == NULL)
    {
        fe_fnt_rsc_lock ();

        /* if no any font remains, this new one will be default font */
        if(SLIST_IS_EMPTY(&gt_fnt_rsc_q))
        {
            gpt_dfl_fnt = pt_rsc;
        }

        /* add into font resource list */
        SLIST_INSERT_HEAD(pt_rsc, &gt_fnt_rsc_q, t_rsc_lnk);

        fe_fnt_rsc_unlock ();

        return FER_OK;
    }
    else if (pt_rsc->ui1_font_number <= 1)
    {
        if(pt_rsc->ps_fnt_name)
        {
            FE_FREE(pt_rsc->ps_fnt_name);
        }
        if( FE_CURRENT_RSC(pt_rsc).ps_file_name)
        {
            FE_FREE( FE_CURRENT_RSC(pt_rsc).ps_file_name);
        }
        if(FE_CURRENT_RSC(pt_rsc).pe_cmaps)
        {
            FE_FREE(FE_CURRENT_RSC(pt_rsc).pe_cmaps);
        }
        return FER_FONT_EXIST;
    }
    else
    {
        return FER_OK;
    }
}


/*-----------------------------------------------------------------------------
 * Name:  fe_add_file_fnt_rsc
 *
 * Description: Add a font resource into list.
 *
 * Inputs:  pt_face             Font face object.
 *          pt_rsc              Font resource object.
 *          pt_font             Font name.
 *          pt_file             Font file name.
 *          ui1_small           Size of small font.
 *          ui1_medium          Size of medium font.
 *          ui1_large           Size of large font.
 *
 * Outputs: -
 *
 * Returns: FER_OK              Font resource is added successfully.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_FONT_EXIST      The same font existed.
 ----------------------------------------------------------------------------*/
INT32 fe_add_file_fnt_rsc(
    const FT_Face       pt_face,
    FE_FNT_RSC_T*       pt_rsc,
    const CHAR*         ps_font,
    const CHAR*         ps_file,
    UINT8               ui1_small,
    UINT8               ui1_medium,
    UINT8               ui1_large)
{
    return fe_add_fnt_rsc(pt_face,
                          pt_rsc,
                          ps_font,
                          ps_file,
                          NULL,
                          0,
                          ui1_small,
                          ui1_medium,
                          ui1_large);
}


/*-----------------------------------------------------------------------------
 * Name:  fe_add_mem_fnt_rsc
 *
 * Description: Add a memory font resource into list.
 *
 * Inputs:  pt_face             Font face object.
 *          pt_rsc              Font resource object.
 *          pt_font             Font name.
 *          pui1_fontdata       Font data in memory.
 *          ui4_datasize        Font data size.
 *          ui1_small           Size of small font.
 *          ui1_medium          Size of medium font.
 *          ui1_large           Size of large font.
 *
 * Outputs: -
 *
 * Returns: FER_OK              Font resource is added successfully.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_FONT_EXIST      The same font existed.
 ----------------------------------------------------------------------------*/
INT32 fe_add_mem_fnt_rsc(
    const FT_Face       pt_face,
    FE_FNT_RSC_T*       pt_rsc,
    const CHAR*         ps_font,
    const UINT8*        pui1_fontdata,
    UINT32              ui4_datasize,
    UINT8               ui1_small,
    UINT8               ui1_medium,
    UINT8               ui1_large)
{
    return fe_add_fnt_rsc(pt_face,
                          pt_rsc,
                          ps_font,
                          NULL,
                          pui1_fontdata,
                          ui4_datasize,
                          ui1_small,
                          ui1_medium,
                          ui1_large);
}

UINT32 fe_get_char_index(FE_FNT_OBJ_T *pt_obj, CHAR_CODE cc_code, UINT8 *pui1_idx)
{
    UINT8   ui1_i = 0, ui1_ori = 0;
    UINT32  ui4_idx = 0;

    if (cc_code == 0xa || cc_code == 0xd)
    {
        return 0;
    }

    fe_fnt_lock();

    /* check original font*/
    ui4_idx = FT_Get_Char_Index(FE_CURRENT_FONT(pt_obj).pt_face, cc_code);

    /* found */
    if (ui4_idx != 0 || (ui4_idx ==0 && cc_code == 0))
    {
        if (pui1_idx)
        {
            *pui1_idx = pt_obj->pt_fnt_rsc->ui1_current_font_id;
        }

        fe_fnt_unlock();
        return ui4_idx;
    }

    ui1_ori = pt_obj->pt_fnt_rsc->ui1_current_font_id;

    ui4_idx = 0;

    fe_fnt_rsc_lock();

    /* not found */
    for (ui1_i = 0; ui1_i < pt_obj->pt_fnt_rsc->ui1_font_number; ui1_i++)
    {
        if (ui1_i != ui1_ori)
        {
            pt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_i;
            fe_verify_create_face(pt_obj);

            ui4_idx = FT_Get_Char_Index(pt_obj->pt_fnt_rsc->at_font_data[FE_CHECK_FONT_ID(ui1_i)].pt_face, cc_code);

            /* found */
            if (ui4_idx != 0)
            {
                if (pui1_idx)
                {
                    *pui1_idx = ui1_i;
                }

                break;
            }
        }
    }

    pt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori;

    fe_fnt_rsc_unlock();

    fe_fnt_unlock();

    return ui4_idx;
}

/*-----------------------------------------------------------------------------
 * Name:  fe_remove_font
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
 *          FER_FONT_IN_USE     One or more handle(s) are associated with this
 *                              font.
 ----------------------------------------------------------------------------*/
INT32 fe_remove_font(
    const CHAR*             ps_font,        /* font name                     */
    FE_REMOVE_OPTION_T      e_option)
{
    FE_FNT_RSC_T*   pt_rsc;

    FNT_ASSERT(ps_font != NULL);

    fe_fnt_rsc_lock ();
    SLIST_FOR_EACH(pt_rsc, &gt_fnt_rsc_q, t_rsc_lnk)
    {
        if( FE_STRCMP(pt_rsc->ps_fnt_name, ps_font)==0 )
        {
            /* found */
            break;
        }
    }
    if (pt_rsc == NULL)
    {
        /* cannot find */
        fe_fnt_rsc_unlock ();
        return FER_NO_FONT;
    }

    if (SLIST_IS_EMPTY(&pt_rsc->t_obj_link_head))
    {
        SLIST_REMOVE(pt_rsc, t_rsc_lnk);
        _fe_free_fnt_rsc(pt_rsc);
        fe_fnt_rsc_unlock ();
        return FER_OK;
    }
    else
    {
        if (!(e_option & X_FE_OPTION_FORCE))
        {
            /* cannot remove resource because there are still handles associated */
            fe_fnt_rsc_unlock ();
            return FER_FONT_IN_USE;
        }
        else
        {
            SLIST_REMOVE(pt_rsc, t_rsc_lnk);
            _fe_free_fnt_rsc(pt_rsc);
            fe_fnt_rsc_unlock ();
            return FER_OK;
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name:  fe_link_obj_to_rsc
 *
 * Description: Add a font resource into list.
 *
 * Inputs:  pt_rsc              Font resource.
 *          pt_obj              Font object.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fe_link_obj_to_rsc(
    FE_FNT_RSC_T*       pt_rsc,
    FE_FNT_OBJ_T*       pt_obj)
{
    FNT_ASSERT(pt_rsc  != NULL);
    FNT_ASSERT(pt_obj != NULL);

    fe_fnt_rsc_lock ();

    /* add into font object list */
    SLIST_INSERT_HEAD(pt_obj, &pt_rsc->t_obj_link_head, t_obj_lnk);

    fe_fnt_rsc_unlock ();
}


/*-----------------------------------------------------------------------------
 * Name:  fe_del_fnt_rsc
 *
 * Description: Delete a font resource.
 *
 * Inputs:  pt_rsc              Font resource object.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fe_del_fnt_rsc(
    FE_FNT_RSC_T*       pt_rsc)
{
    FNT_ASSERT(pt_rsc  != NULL);

    /* remove from list */
    fe_fnt_rsc_lock ();
    SLIST_REMOVE(pt_rsc, t_rsc_lnk);
    fe_fnt_rsc_unlock ();

    _fe_free_fnt_rsc(pt_rsc);
    return;
}


/*-----------------------------------------------------------------------------
 * Name:  fe_find_font
 *
 * Description: Find a font resource in global font resource list.
 *
 * Inputs:  ps_font             Specify the font name to find.
 *          e_style             Specify the font style to find.
 *
 * Outputs: -
 *
 * Returns: If found, return the resource pointer.
 *          Otherwise, return NULL.
 ----------------------------------------------------------------------------*/
FE_FNT_RSC_T* fe_find_font(
    const CHAR*         ps_font,
    FE_FNT_STYLE        e_style)
{
    FE_FNT_RSC_T*   pt_rsc;
    FE_FNT_STYLE    e_style_;

    if(ps_font == NULL)
    {
        return gpt_dfl_fnt;
    }

    /* No support StrikeOut now */
    if(e_style & FE_FNT_STYLE_STRIKEOUT)
    {
        return NULL;
    }

    e_style_ = _fe_chk_rsc_style(e_style);

    fe_fnt_rsc_lock ();
    SLIST_FOR_EACH(pt_rsc, &gt_fnt_rsc_q, t_rsc_lnk)
    {
        if( FE_STRCMP(pt_rsc->ps_fnt_name, ps_font)==0 &&
            pt_rsc->e_style == e_style_ )
        {
            fe_fnt_rsc_unlock ();
            return pt_rsc;
        }
    }

#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    /* support real-time embolden and italic transform, so just search the name */
    if (e_style_ &
        (FE_FNT_STYLE_BOLD | FE_FNT_STYLE_ITALIC | FE_FNT_STYLE_UNDERLINE))
    {
        SLIST_FOR_EACH(pt_rsc, &gt_fnt_rsc_q, t_rsc_lnk)
        {
            if(FE_STRCMP(pt_rsc->ps_fnt_name, ps_font)==0)
            {
                fe_fnt_rsc_unlock ();
                return pt_rsc;
            }
        }
    }
#endif

    fe_fnt_rsc_unlock ();

    return NULL;
}


FE_FNT_RSC_T* fe_find_font_ex(
    const CHAR*         ps_font)
{
    FE_FNT_RSC_T*   pt_rsc;

    if(ps_font == NULL)
    {
        return gpt_dfl_fnt;
    }

    fe_fnt_rsc_lock ();
    SLIST_FOR_EACH(pt_rsc, &gt_fnt_rsc_q, t_rsc_lnk)
    {
        if( FE_STRCMP(pt_rsc->ps_fnt_name, ps_font)==0)
        {
            fe_fnt_rsc_unlock ();
            return pt_rsc;
        }
    }

    fe_fnt_rsc_unlock ();

    return NULL;
}


INT32   fe_get_fontdata_by_name(const CHAR*         ps_font,
                                        UINT8**        		ppui1_fontdata,
                                        UINT32*             pui4_fontsize,
                                        FE_FNT_RSC_TYPE_T*  pt_src_type)
{
    FE_FNT_RSC_T*   pt_fnt_src;

    pt_fnt_src = fe_find_font_ex(ps_font);
    if (pt_fnt_src == NULL)
        return FER_NO_FONT;

    *pt_src_type = pt_fnt_src->t_fnt_rsc_type;
    if (*pt_src_type == FE_FNT_RSC_TYPE_FILE)
    {
        *ppui1_fontdata = (UINT8*)FE_CURRENT_RSC(pt_fnt_src).ps_file_name;
		*pui4_fontsize = 0;
    }
    else
    {
        *ppui1_fontdata = (UINT8*)FE_CURRENT_RSC(pt_fnt_src).pui1_fnt_data;
        *pui4_fontsize = FE_CURRENT_RSC(pt_fnt_src).ui4_rsc_size;
    }

    return FER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  fe_has_font_name
 *
 * Description: Find a font name in global font resource list.
 *
 * Inputs:  ps_font             Specify the font name to find.
 *
 * Outputs: -
 *
 * Returns: If found, return TRUE.
 *          Otherwise, return FALSE.
 ----------------------------------------------------------------------------*/
BOOL fe_has_font_name(
    const CHAR*         ps_font)
{
    FE_FNT_RSC_T*   pt_rsc;

    if(ps_font == NULL)
    {
        return FALSE;
    }

    fe_fnt_rsc_lock ();
    SLIST_FOR_EACH(pt_rsc, &gt_fnt_rsc_q, t_rsc_lnk)
    {
        if( FE_STRCMP(pt_rsc->ps_fnt_name, ps_font)==0 )
        {
            fe_fnt_rsc_unlock ();
            return TRUE;
        }
    }
    fe_fnt_rsc_unlock ();

    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name:  _fe_chk_rsc_style
 *
 * Description: The style format should be the same with which we store in font
 *              resource.
 *              I.E, if italic or bold, => no regular flag
 *                   if neither italic nor bold => regular flag must exist
 *
 * Inputs:  e_style             Specify the font style to validated.
 *
 * Outputs: -
 *
 * Returns: Corrected font style.
 *
 ----------------------------------------------------------------------------*/
static FE_FNT_STYLE _fe_chk_rsc_style(
    FE_FNT_STYLE        e_style)
{
    if( (e_style&FE_FNT_STYLE_ITALIC) || (e_style&FE_FNT_STYLE_BOLD) )
    {
        e_style &= ~FE_FNT_STYLE_REGULAR;
    }
    else
    {
        e_style |= FE_FNT_STYLE_REGULAR;
    }

    /* these styles are rather features of rendering, not of font */
    e_style &= ~FE_FNT_STYLE_UNDERLINE;
    e_style &= ~FE_FNT_STYLE_STRIKEOUT;
    e_style &= ~FE_FNT_STYLE_OUTLINE;
    e_style &= ~FE_FNT_STYLE_BLURRED;

#ifndef FE_DISABLE_EDGE_EFFECT_SUPPORT
    e_style &= ~FE_FNT_STYLE_EDGE_EFFECT;
#endif

    return e_style;
}


/*-----------------------------------------------------------------------------
 * Name:  _fe_del_font_rsc
 *
 * Description: Remove font resource from list, and free it.
 *
 * Inputs:  ps_font             Font name
 *          e_style             Font style.
 *
 * Outputs: -
 *
 * Returns: TRUE                Font resource is removed successfully.
 *          FALSE               Font is not found.
 ----------------------------------------------------------------------------*/
/*
static BOOL _fe_del_font_rsc(
    const CHAR*         ps_font,
    FE_FNT_STYLE        e_style)
{
    FE_FNT_RSC_T*   pt_rsc;

    pt_rsc = fe_find_font(ps_font, e_style);
    if(pt_rsc == NULL)
    {
        return FALSE;
    }

    SLIST_REMOVE(pt_rsc, t_rsc_lnk);
    _fe_free_fnt_rsc(pt_rsc);

    return TRUE;
}
*/


/*-----------------------------------------------------------------------------
 * Name:  fe_set_dfl_font
 *
 * Description: Set the system default font.
 *
 * Inputs:  ps_font             Font name
 *          e_style             Font style.
 *
 * Outputs: -
 *
 * Returns: TRUE                Success.
 *          FALSE               Font is not found.
 ----------------------------------------------------------------------------*/
BOOL fe_set_dfl_font(
    const CHAR*         ps_font,
    FE_FNT_STYLE        e_style)
{
    FE_FNT_RSC_T*   pt_rsc;

    pt_rsc = fe_find_font(ps_font, e_style);
    if(pt_rsc == NULL)
    {
        return FALSE;
    }

    gpt_dfl_fnt = pt_rsc;
    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name:  fe_has_cmap
 *
 * Description: Check if the character map is supported in this font.
 *
 * Inputs:  ps_rsc              Font resource object.
 *          e_cmap              Character map.
 *
 * Outputs: -
 *
 * Returns: TRUE                The character map is supported.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL fe_has_cmap(
    const FE_FNT_RSC_T* pt_rsc,
    FE_CMAP_ENCODING    e_cmap)
{
    UINT32  ui4_i;

    FNT_ASSERT(pt_rsc != NULL);

    for(ui4_i = 0; ui4_i < (UINT32)FE_CURRENT_RSC(pt_rsc).i4_num_cmaps; ui4_i++)
    {
        if(FE_CURRENT_RSC(pt_rsc).pe_cmaps[ui4_i] == e_cmap)
        {
            return TRUE;
        }
    }

    return FALSE;
}


/*add for set_font_size */
INT32 fe_set_fnt_accurate_pixel_size(
    FE_FNT_RSC_T        *pt_fnt_rsc,
    INT32               i4_size)
{
#if 0
    FT_Error        e_error;

    FNT_ASSERT(pt_fnt_rsc != NULL);
    FNT_ASSERT(pt_fnt_rsc->pt_face != NULL);



    fe_fnt_rsc_lock();
    e_error = FT_Set_Pixel_Sizes(pt_fnt_rsc->pt_face, 0, (FT_UInt)i4_size);
    fe_fnt_rsc_unlock();

    if(e_error)
    {
        return FER_NO_FONT_SIZE;
    }
#endif
    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  fe_set_fnt_size
 *
 * Description: set font size
 *
 * Inputs:  pt_fnt_obj          Specify the font object.
 *          ui4_h_dpi           Horizontal dpi.
 *          ui4_v_dpi           Vertical dpi.
 *
 * Outputs: -
 *
 * Returns: FER_OK              Success.
 *          FER_NO_FONT_SIZE    Specified font size is not supported.
 ----------------------------------------------------------------------------*/
INT32 fe_set_fnt_size(
    FE_FNT_RSC_T*       pt_fnt_rsc,
    FE_FNT_SIZE         e_size,
    UINT32              ui4_h_dpi,
    UINT32              ui4_v_dpi)
{
    FE_SIZE_UNIT_T  e_fnt_unit;
    UINT8           ui1_fnt_size;
    FT_Error        e_error;

    FNT_ASSERT(pt_fnt_rsc != NULL);
    FNT_ASSERT(FE_CURRENT_RSC(pt_fnt_rsc).pt_face != NULL);

    e_fnt_unit   = FE_CURRENT_RSC(pt_fnt_rsc).t_fnt_size[e_size].e_unit;
    ui1_fnt_size = FE_CURRENT_RSC(pt_fnt_rsc).t_fnt_size[e_size].ui1_size;

    /* check unit */
    if(e_fnt_unit == FE_SIZE_UNIT_POINT)
    {
        fe_fnt_rsc_lock();
        e_error = FT_Set_Char_Size(FE_CURRENT_RSC(pt_fnt_rsc).pt_face,
                                   ui1_fnt_size*64,
                                   ui1_fnt_size*64,
                                   (FT_UInt)ui4_h_dpi,
                                   (FT_UInt)ui4_v_dpi);
        fe_fnt_rsc_unlock();
    }
    else /* FE_SIZE_UNIT_PIXEL */
    {
        fe_fnt_rsc_lock();
        e_error = FT_Set_Pixel_Sizes(FE_CURRENT_RSC(pt_fnt_rsc).pt_face, 0, ui1_fnt_size);
        fe_fnt_rsc_unlock();
    }
    if(e_error)
    {
        return FER_NO_FONT_SIZE;
    }

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fe_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 fe_get_dbg_level(
    VOID)
{
    if (b_fe_init)
    {
        return ui2_fe_dbg_level;
    }
    else
    {
        return 0;
    }
}


/*-----------------------------------------------------------------------------
 * Name: fe_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/
BOOL fe_set_dbg_level(
    UINT16 ui2_level)
{
    if (b_fe_init)
    {
        ui2_fe_dbg_level = ui2_level;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*-----------------------------------------------------------------------------
 * Name: fe_get_first_rsc
 *
 * Description: This API gets the first font resource.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: Font resource in system.
 ----------------------------------------------------------------------------*/
FE_FNT_RSC_T* fe_get_first_rsc(
    VOID)
{
    return SLIST_FIRST(&gt_fnt_rsc_q);
}

#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: fe_chk_synthesize_style_prolog
 *
 * Description: This API checks if a bold or italic style of glyph need be
 *              synthesized and calls corresponding utility functions to
 *              accomplish it.
 *
 * Inputs:  pt_fnt_obj          Specify the font object.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fe_chk_synthesize_style_prolog(
    FE_FNT_OBJ_T*       pt_fnt_obj)
{
    if ((pt_fnt_obj->e_style & FE_FNT_STYLE_ITALIC) &&
        !(pt_fnt_obj->pt_fnt_rsc->e_style & FE_FNT_STYLE_ITALIC) &&
        FE_FNT_IS_SCALABLE(FE_CURRENT_FONT(pt_fnt_obj).i4_face_flags))
    {
        /* a scalable font could be obliqued by matrix modification */
        FT_Matrix    transform = {  0x10000L,
                                    0x03333L,
                                    0x00000L,
                                    0x10000L };
        FT_Set_Transform (FE_CURRENT_FONT(pt_fnt_obj).pt_face, &transform, 0);
    }
    else
    {
        /* face might be other object using oblique effect */
        FT_Matrix    transform = {  0x10000L,
                                    0x00000L,
                                    0x00000L,
                                    0x10000L };
        FT_Set_Transform (FE_CURRENT_FONT(pt_fnt_obj).pt_face, &transform, 0);
    }
    return;
}


/*-----------------------------------------------------------------------------
 * Name: fe_chk_synthesize_style_epilog
 *
 * Description: This API checks if a bold or italic style of glyph need be
 *              synthesized and calls corresponding utility functions to
 *              accomplish it.
 *
 * Inputs:  pt_fnt_obj          Specify the font object.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fe_chk_synthesize_style_epilog(
    FE_FNT_OBJ_T*       pt_fnt_obj)
{
    if ((pt_fnt_obj->e_style & FE_FNT_STYLE_BOLD) &&
        !(pt_fnt_obj->pt_fnt_rsc->e_style & FE_FNT_STYLE_BOLD))
    {
        /* embolden the glyph */
        FT_GlyphSlot_Embolden(FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph);
    }
    if ((pt_fnt_obj->e_style & FE_FNT_STYLE_ITALIC) &&
        !(pt_fnt_obj->pt_fnt_rsc->e_style & FE_FNT_STYLE_ITALIC) &&
        !FE_FNT_IS_SCALABLE(FE_CURRENT_FONT(pt_fnt_obj).i4_face_flags))
    {
        /* oblique the glyph */
        FT_GlyphSlot_Oblique(FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph);
    }
    return;
}


extern INT16 FT_GlyphSlot_ShadowRight_Width_Inc( void );
extern INT16 FT_GlyphSlot_ShadowLeft_Width_Inc( void );
extern INT16 FT_GlyphSlot_Depressed_Width_Inc( void );
extern INT16 FT_GlyphSlot_Raised_Width_Inc( void );
extern INT16 FT_GlyphSlot_Uniform_Width_Inc( void );
extern INT16 FT_GlyphSlot_Outline_Width_Inc( void );

/*-----------------------------------------------------------------------------
 * Name: fe_get_width_increased_by_synthesis
 *
 * Description: This API checks if a bold or italic style of glyph need be
 *              synthesized and calculates the increased width accordingly.
 *
 * Inputs:  pt_fnt_obj          Specify the font object.
 *
 * Outputs: -
 *
 * Returns: Width increment.
 ----------------------------------------------------------------------------*/
INT16 fe_get_width_increased_by_synthesis(
    FE_FNT_OBJ_T*       pt_fnt_obj,
    INT16               i2_height)
{
    INT16  ui2_wid = 0;

    if ((pt_fnt_obj->e_style & FE_FNT_STYLE_ITALIC) &&
        !(pt_fnt_obj->pt_fnt_rsc->e_style & FE_FNT_STYLE_ITALIC) &&
        FE_FNT_IS_SCALABLE(FE_CURRENT_FONT(pt_fnt_obj).i4_face_flags))
    {
        ui2_wid = (i2_height * 0x03333L) / 0x10000L;
    }
    else if (pt_fnt_obj->e_style & FE_FNT_STYLE_SHADOW_RIGHT)
    {
        ui2_wid = FT_GlyphSlot_ShadowRight_Width_Inc();
    }
    else if (pt_fnt_obj->e_style & FE_FNT_STYLE_SHADOW_LEFT)
    {
        ui2_wid = FT_GlyphSlot_ShadowLeft_Width_Inc();
    }
    else if (pt_fnt_obj->e_style & FE_FNT_STYLE_DEPRESSED)
    {
        ui2_wid = FT_GlyphSlot_Raised_Width_Inc();
        /*ui2_wid = FT_GlyphSlot_Depressed_Width_Inc();*/
    }
    else if (pt_fnt_obj->e_style & FE_FNT_STYLE_RAISED)
    {
        ui2_wid = FT_GlyphSlot_Depressed_Width_Inc();
        /*ui2_wid = FT_GlyphSlot_Raised_Width_Inc();*/
    }
    else if (pt_fnt_obj->e_style & FE_FNT_STYLE_UNIFORM)
    {
        ui2_wid = FT_GlyphSlot_Uniform_Width_Inc();
    }
    else if (pt_fnt_obj->e_style & FE_FNT_STYLE_OUTLINE)
    {
        ui2_wid = FT_GlyphSlot_Outline_Width_Inc();
    }
     else if (pt_fnt_obj->e_style & FE_FNT_STYLE_BLURRED)
    {
        ui2_wid = pt_fnt_obj->ui2_bld_w * 2;
    }
    return ui2_wid;
}
#endif


#ifndef FE_DISABLE_EDGE_EFFECT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: fe_synthesize_edge_effect
 *
 * Description: This API checks if a bold or italic style of glyph need be
 *              synthesized and calls corresponding utility functions to
 *              accomplish it.
 *
 * Inputs:  pt_fnt_obj          Specify the font object.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fe_synthesize_edge_effect(
    FE_FNT_OBJ_T*       pt_fnt_obj)
{
    BOOL fg_double_buffer = FALSE;
    _PERF_START();
    if (pt_fnt_obj->e_style & FE_FNT_STYLE_OUTLINE)
    {
        FT_GlyphSlot_Custom_Outline(FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph, pt_fnt_obj->ui2_outline_width);
        /*FT_GlyphSlot_Outline(pt_fnt_obj->pt_fnt_rsc->pt_face->glyph);*/
    }
    if (pt_fnt_obj->e_style & FE_FNT_STYLE_SHADOW_RIGHT)
    {
        FT_GlyphSlot_ShadowRight(FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph, fg_double_buffer);
        fg_double_buffer = TRUE;
    }
    if (pt_fnt_obj->e_style & FE_FNT_STYLE_SHADOW_LEFT)
    {
        FT_GlyphSlot_ShadowLeft(FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph,fg_double_buffer);
        fg_double_buffer = TRUE;
    }
    if (pt_fnt_obj->e_style & FE_FNT_STYLE_DEPRESSED)
    {
        FT_GlyphSlot_Raised(FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph, fg_double_buffer);
        fg_double_buffer = TRUE;
        /*FT_GlyphSlot_Depressed(pt_fnt_obj->pt_fnt_rsc->pt_face->glyph);*/
    }
    if (pt_fnt_obj->e_style & FE_FNT_STYLE_RAISED)
    {
        FT_GlyphSlot_Depressed(FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph, fg_double_buffer);
        fg_double_buffer = TRUE;
        /*FT_GlyphSlot_Raised(pt_fnt_obj->pt_fnt_rsc->pt_face->glyph);*/
    }
    if (pt_fnt_obj->e_style & FE_FNT_STYLE_UNIFORM)
    {
        FT_GlyphSlot_Custom_Uniform(FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph, pt_fnt_obj->ui2_uniform_width, fg_double_buffer);
        fg_double_buffer = TRUE;
        /*FT_GlyphSlot_Uniform(pt_fnt_obj->pt_fnt_rsc->pt_face->glyph);*/
    }    
    if (pt_fnt_obj->e_style & FE_FNT_STYLE_BLURRED)
    {       
        FT_GlyphSlot_Blurred(FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph, pt_fnt_obj->ui2_bld_w,fg_double_buffer);
    }

    _PERF_END();
    return;
}
#endif


#ifdef DEBUG
/*-----------------------------------------------------------------------------
 * Name: FE_ALLOC
 *
 * Description: For debug mode, this function monitors memory usage as well as
 *              allocates z_size bytes.
 *
 * Inputs:  z_size              Specify the size.
 *
 * Outputs: -
 *
 * Returns: Reference the allocated memory.
 ----------------------------------------------------------------------------*/
VOID*   FE_ALLOC(
    SIZE_T z_size)
{
    VOID*               pv_buf;
    FE_MEM_RECORD_T*    pt_mem_record;

    FNT_ASSERT( z_size );

    pv_buf = x_mem_alloc(z_size + sizeof(FE_MEM_RECORD_T));
    if (pv_buf != NULL)
    {
        /* put the record in front of allocated buffer */
        pt_mem_record = (FE_MEM_RECORD_T*) pv_buf;
        pv_buf = (VOID*) (pt_mem_record + 1);

        /* setup record */
        z_mem_usage += z_size;
        pt_mem_record->pv_buf = pv_buf;
        pt_mem_record->z_size = z_size;
        SLIST_INSERT_HEAD(pt_mem_record, &gt_mem_record_q, t_mem_record_lnk);
    }

    return pv_buf;
}


/*-----------------------------------------------------------------------------
 * Name: FE_REALLOC
 *
 * Description: For debug mode, this function monitors memory usage as well as
 *              re-allocates the memory space pointed to by pv_buf.
 *
 * Inputs:  pv_buf              Specify the memory space.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID*   FE_REALLOC(
    VOID* pv_buf,
    SIZE_T z_new_size)
{
    FE_MEM_RECORD_T*    pt_mem_record;

    FNT_ASSERT( pv_buf );
    FNT_ASSERT( z_new_size );

    /* verify record */
    pt_mem_record = ((FE_MEM_RECORD_T*) pv_buf) - 1;
    FNT_ASSERT(pt_mem_record->pv_buf == pv_buf);

    /* clear record */
    z_mem_usage -= pt_mem_record->z_size;
    pt_mem_record->pv_buf = NULL;
    pt_mem_record->z_size = 0;
    SLIST_REMOVE(pt_mem_record, t_mem_record_lnk);

    /* free record */
    pv_buf = (VOID*) pt_mem_record;

    pv_buf = x_mem_realloc(pv_buf, z_new_size + sizeof(FE_MEM_RECORD_T));
    if (pv_buf != NULL)
    {
        /* put the record in front of allocated buffer */
        pt_mem_record = (FE_MEM_RECORD_T*) pv_buf;
        pv_buf = (VOID*) (pt_mem_record + 1);

        /* setup record */
        z_mem_usage += z_new_size;
        pt_mem_record->pv_buf = pv_buf;
        pt_mem_record->z_size = z_new_size;
        SLIST_INSERT_HEAD(pt_mem_record, &gt_mem_record_q, t_mem_record_lnk);
    }

    return pv_buf;
}


/*-----------------------------------------------------------------------------
 * Name: FE_FREE
 *
 * Description: For debug mode, this function monitors memory usage as well as
 *              frees the memory space pointed to by pv_buf.
 *
 * Inputs:  pv_buf              Specify the memory space.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID    FE_FREE(
    VOID* pv_buf)
{
    FE_MEM_RECORD_T*    pt_mem_record;

    FNT_ASSERT( pv_buf );

    /* verify record */
    pt_mem_record = ((FE_MEM_RECORD_T*) pv_buf) - 1;
    FNT_ASSERT(pt_mem_record->pv_buf == pv_buf);

    /* clear record */
    z_mem_usage -= pt_mem_record->z_size;
    pt_mem_record->pv_buf = NULL;
    pt_mem_record->z_size = 0;
    SLIST_REMOVE(pt_mem_record, t_mem_record_lnk);

    /* free buffer as well as record */
    pv_buf = (VOID*) pt_mem_record;
    x_mem_free(pv_buf);
}

SIZE_T fe_dbg_get_mem_usage(
    VOID)
{
    return z_mem_usage;
}
#endif
