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
 * $RCSfile: cache.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Ben Tsai $
 * $MD5HEX: 7fc864b0cccd3aa9e1dfb0d542e29664 $
 *
 * Description:
 *         This header file contains font library specific functions,
 *         which are private.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef FE_DISABLE_CACHE

#include "font/cache.h"

#if !defined(FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT) && !defined(FE_DISABLE_EDGE_EFFECT_SUPPORT)
#include FT_SYNTHESIS_H
#endif


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef DEBUG
#define DEBUG_MEM_MONITOR
#endif

#ifndef DEBUG_MEM_MONITOR
#define FE_CACHE_ALLOC      x_mem_alloc
#define FE_CACHE_FREE       x_mem_free
#else
VOID*   FE_CACHE_ALLOC(SIZE_T z_size);
VOID    FE_CACHE_FREE(VOID* pv_buf);
#endif

#ifdef DEBUG_MEM_MONITOR
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
/* usable font cache memory */
static UINT32   gi4_avail_mem = FNT_CACHE_MEM_SIZE * 1024;


/* font cache list array */
static SLIST_T(_FE_CACHE_OBJ_T) gt_fnt_cache_q[FNT_CACHE_HASH_SIZE];
/* When cache memory is not enough, we want to free a cache object.
   The method is to round-robin all the hash list, and free the cache object
   in the end of the selected hash queu. This variable is the latest hash
   queue to free. */
static UINT8                    gui1_last_free_queue = 0;


/* global font cache lock */
static HANDLE_T h_fnt_cache_lock = NULL_HANDLE;

#ifdef DEBUG
/* count miss rate */
static UINT32   ui4_access=0, ui4_miss=0;
#endif
#ifdef DEBUG_MEM_MONITOR
static SIZE_T                   z_mem_usage;      /* system memory usage   */
static SLIST_T(_FE_MEM_RECORD)  gt_mem_record_q;    /* memory usage record   */
#endif


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: fe_cache_lock
 *
 * Description: API to lock the font cache list
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID fe_cache_lock(
    VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_lock( h_fnt_cache_lock, X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT(DBG_MOD_FONT);
    }
}


/*-----------------------------------------------------------------------------
 * Name: fe_cache_unlock
 *
 * Description: API to unlock the font cache list
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID fe_cache_unlock(
    VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock( h_fnt_cache_lock );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT(DBG_MOD_FONT);
    }
}


/*-----------------------------------------------------------------------------
 * Name: fe_cache_free_obj
 *
 * Description: To free a cache object in cache list
 *
 * Inputs:  pt_cache            Pointer to the cache object to be freed.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID fe_cache_free_obj(
    FE_CACHE_OBJ_T*     pt_cache)
{
    if(pt_cache == NULL)
    {
        return;
    }

    if(pt_cache->t_fnt_glyph.t_bmp.pui1_buf != NULL)
    {
#if 0
        x_memcpy(pt_cache->t_fnt_glyph.t_bmp.pui1_buf, "DBGCACHE", 8);
#endif
        FE_CACHE_FREE(pt_cache->t_fnt_glyph.t_bmp.pui1_buf);
        pt_cache->t_fnt_glyph.t_bmp.pui1_buf = NULL;
    }

    FE_CACHE_FREE(pt_cache);
}


/*-----------------------------------------------------------------------------
 * Name: fe_cache_search
 *
 * Description: Search a glyph in the cache list
 *
 * Inputs:  pt_fnt_obj          Pointer to the font object.
 *          ui4_glyph_idx       The glyph index to be searched.
 *          e_render_mode       The glyph render mode.
 *
 * Outputs: -
 *
 * Returns: if found, return the pointer of the glyph object.
 *          if not, return NULL.
 ----------------------------------------------------------------------------*/
static FE_GLYPH_T* fe_cache_search(
    FE_FNT_OBJ_T*       pt_fnt_obj,
    UINT32              ui4_glyph_idx,
    FE_RENDER_MODE_T    e_render_mode,
    CHAR_CODE			cc_char_code)
{
    FE_CACHE_OBJ_T* pt_cache;
    UINT8           ui1_hash;
    FE_GLYPH_T*     pt_glyph = NULL;

    ui1_hash = (UINT8)FNT_CACHE_HASH(ui4_glyph_idx);

    fe_cache_lock();

    /* lookup in cache queue */
    SLIST_FOR_EACH(pt_cache, &(gt_fnt_cache_q[ui1_hash]), t_fnt_cache_lnk)
    {
        if( pt_cache->pt_fnt_rsc    == pt_fnt_obj->pt_fnt_rsc &&
            pt_cache->ui1_font_id   == pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id &&
            pt_cache->t_fnt_size    == pt_fnt_obj->e_size     &&
#if !defined(FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT) && !defined(FE_DISABLE_EDGE_EFFECT_SUPPORT)
            pt_cache->t_fnt_style   == pt_fnt_obj->e_style    &&
            pt_cache->ui2_outline_width == pt_fnt_obj->ui2_outline_width &&
            pt_cache->ui2_uniform_width == pt_fnt_obj->ui2_uniform_width &&
            pt_cache->ui2_bld_w    == pt_fnt_obj->ui2_bld_w     &&
#endif
            pt_cache->e_render_mode == e_render_mode          &&
            pt_cache->ui4_glyph_idx == ui4_glyph_idx          &&
            pt_cache->cc_char_code  == cc_char_code)
        {
            if (pt_fnt_obj->e_size != FE_FNT_SIZE_CUSTOM ||
                (pt_cache->ui1_custom_size == pt_fnt_obj->ui1_custom_size))
            {
                /* found, move it to first */
                SLIST_REMOVE(pt_cache, t_fnt_cache_lnk);
                SLIST_INSERT_HEAD(pt_cache,
                                  &(gt_fnt_cache_q[ui1_hash]),
                                  t_fnt_cache_lnk);

                pt_glyph = &(pt_cache->t_fnt_glyph);
                break;
            }
        }
    }

    fe_cache_unlock();

    return pt_glyph;
}


/*-----------------------------------------------------------------------------
 * Name: fe_cache_new_obj
 *
 * Description: Create a new empty cache object
 *
 * Inputs:  ui4_bmp_size        Bitmap size to be cahced.
 *
 * Outputs: -
 *
 * Returns: if success, return the pointer of the cache object.
 *          if not, return NULL.
 ----------------------------------------------------------------------------*/
static FE_CACHE_OBJ_T* fe_cache_new_obj(
    UINT32      ui4_bmp_size)
{
    FE_CACHE_OBJ_T*     pt_cache;
    UINT8*              pui1_bmp;

    pt_cache = (FE_CACHE_OBJ_T*)FE_CACHE_ALLOC(sizeof(FE_CACHE_OBJ_T));
    if(pt_cache != NULL)
    {
        if (ui4_bmp_size == 0)
        {
            pt_cache->t_fnt_glyph.t_bmp.pui1_buf = NULL;
        }
        else
        {
            pui1_bmp = (UINT8*)FE_CACHE_ALLOC( ui4_bmp_size );
            if(pui1_bmp == NULL)
            {
                FE_CACHE_FREE(pt_cache);
                pt_cache = NULL;
            }
            else
            {
                pt_cache->t_fnt_glyph.t_bmp.pui1_buf = pui1_bmp;
            }
        }
    }

    return pt_cache;
}


/*-----------------------------------------------------------------------------
 * Name: fe_cache_cleanup_mem
 *
 * Description: Clean up cache to reserve enough memory for a new cache object.
 *              Note: This routine searchs only 255 times, so it doesn't make
 *              sure the result is ok.
 *              NOTE!!!: This fct should be called in cache_lock status!!!
 *
 * Inputs:  ui4_cache_obj_mem   Needed size of new cache object.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID fe_cache_cleanup_mem(
    UINT32      ui4_cache_obj_mem)
{
    FE_CACHE_OBJ_T*     pt_cache;
    UINT8               ui1_cnt=255;    /* avoid loop forever */

    /* get enough cache object memory */
    while(gi4_avail_mem<ui4_cache_obj_mem && --ui1_cnt)
    {
        /* round-robin to next queue */
        gui1_last_free_queue = (UINT8)FNT_CACHE_HASH(gui1_last_free_queue+1);
        if(SLIST_IS_EMPTY(&(gt_fnt_cache_q[gui1_last_free_queue])))
        {
            continue;
        }

        /* get the end item */
        pt_cache = SLIST_FIRST(&(gt_fnt_cache_q[gui1_last_free_queue]));
        while(SLIST_NEXT(pt_cache, t_fnt_cache_lnk))
        {
            pt_cache = SLIST_NEXT(pt_cache, t_fnt_cache_lnk);
        };

        /* free it */
        gi4_avail_mem += pt_cache->ui4_mem_size;
        SLIST_REMOVE(pt_cache, t_fnt_cache_lnk);
        fe_cache_free_obj(pt_cache);
    };

}

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: fe_cache_init
 *
 * Description: Initialize cache list.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: FER_OK              Success.
 *          FER_INTERNAL_ERR    Middleware internal error.
 ----------------------------------------------------------------------------*/
INT32 fe_cache_init(
    VOID)
{
    INT32   i4_i, i4_ret;

    i4_ret = x_sema_create( &h_fnt_cache_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret!=OSR_OK || h_fnt_cache_lock==NULL_HANDLE )
    {
        return FER_INTERNAL_ERR;
    }

    for(i4_i=0; i4_i<FNT_CACHE_HASH_SIZE; i4_i++)
    {
        SLIST_INIT(&(gt_fnt_cache_q[i4_i]));
    }

#ifdef DEBUG_MEM_MONITOR
    /* initialize memory usage monitor */
    z_mem_usage = 0;
    SLIST_INIT(&gt_mem_record_q);
#endif

    return FER_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fe_cache_done
 *
 * Description: Finalize cache list.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: FER_OK              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 fe_cache_done(
    VOID)
{
    INT32           i4_i;
    FE_CACHE_OBJ_T* pt_cache;

    for(i4_i=0; i4_i<FNT_CACHE_HASH_SIZE; i4_i++)
    {
        while( (pt_cache=SLIST_FIRST(&(gt_fnt_cache_q[i4_i]))) != NULL )
        {
            SLIST_REMOVE(pt_cache, t_fnt_cache_lnk);
            fe_cache_free_obj(pt_cache);
        }
    }

    if(h_fnt_cache_lock != NULL_HANDLE)
    {
        x_sema_delete(h_fnt_cache_lock);
        h_fnt_cache_lock = NULL_HANDLE;
    }

    return FER_OK;
}



/*-----------------------------------------------------------------------------
 * Name: fe_cache_preload
 *
 * Description: Lookup a character in cache list.
 *              If found, return the glyph bitmap.
 *              If not, load the glyph, cache it, and return the glyph.
 *
 * Inputs:  pt_fnt_obj          Pointer of the font object.
 *             cc_char_code        Character to search.
 * Outputs: pt_glyph            Glyph object.
 *
 * Returns: FER_OK              Success.
 *          FER_NO_CHAR_CODE    The char_code does not exist.
 ----------------------------------------------------------------------------*/
INT32 fe_cache_preload(
    FE_FNT_OBJ_T*       pt_fnt_obj,
    CHAR_CODE           cc_char_code,
    FE_RENDER_MODE_T    e_render_mode)
{
    UINT32          ui4_idx;
    UINT8           ui1_hash;
    FE_CACHE_OBJ_T* pt_cache;
    UINT32          ui4_cache_obj_mem;
    INT32           i4_ret = FER_OK;
    FT_Error        e_error;
    FT_GlyphSlot    pt_ft_glyph;
    UINT32          ui4_bmp_size;
    UINT8*          pui1_bmp;
    FE_GLYPH_T*     pt_cache_glyph;
    UINT8           ui1_fid = 0, ui1_ori_fid = 0;

    FNT_ASSERT(pt_fnt_obj!=NULL);

#ifdef DEBUG
    ui4_access ++;
#endif

    fe_fnt_lock();

    ui4_idx = fe_get_char_index(pt_fnt_obj, cc_char_code, &ui1_fid);

    if (ui4_idx == 0 && cc_char_code != 0 && pt_fnt_obj->pt_fnt_rsc->t_unknown_char.b_usr_set)
    {
        fe_fnt_unlock();
        return FER_NO_CHAR_CODE;
    }    
    
    ui1_ori_fid = pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;

    pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_fid;

    ui1_hash = (UINT8)FNT_CACHE_HASH(ui4_idx);

    pt_cache_glyph = fe_cache_search(pt_fnt_obj, ui4_idx, e_render_mode, cc_char_code);
    if(pt_cache_glyph != NULL)
    {
        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;      
        fe_fnt_unlock();
        return FER_OK;
    }
#ifdef DEBUG
    ui4_miss ++;
#endif
    
#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    fe_chk_synthesize_style_prolog(pt_fnt_obj);
#endif
    e_error = FT_Load_Char(FE_CURRENT_FONT(pt_fnt_obj).pt_face,
                           cc_char_code,
                           (e_render_mode & FE_RENDER_MODE_MONO)?
                                (FT_LOAD_RENDER | FT_LOAD_MONOCHROME):
                                FT_LOAD_RENDER);
    if(e_error || (ui4_idx==0 && cc_char_code != 0))
    {
        i4_ret = FER_INTERNAL_ERR; 
        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;
        fe_fnt_unlock();
        return i4_ret;             
    }

    /* copy the data for client */
    pt_ft_glyph = FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph;

#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    fe_chk_synthesize_style_epilog(pt_fnt_obj);
#endif

#ifndef FE_DISABLE_EDGE_EFFECT_SUPPORT
    fe_synthesize_edge_effect(pt_fnt_obj);
#endif
    
    /* the following is to cache it */
    pt_ft_glyph = FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph;
    ui4_bmp_size = (UINT32)(pt_ft_glyph->bitmap.pitch * pt_ft_glyph->bitmap.rows);
#ifndef FE_DISABLE_EDGE_EFFECT_SUPPORT
    /* edge effect takes twice the size */
    if (pt_fnt_obj->e_style & FE_FNT_STYLE_EDGE_EFFECT)
    {
        ui4_bmp_size *= 2;
    }
#endif
    ui4_cache_obj_mem = sizeof(FE_CACHE_OBJ_T) + ui4_bmp_size;

    /* get enough cache object memory */
    fe_cache_lock();

    if(gi4_avail_mem < ui4_cache_obj_mem)
    {
        /* We alreay have data for client, but have no memory to cache it.
           However, it's ok to run correctly in this situation. */

        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;
        fe_cache_unlock();
        fe_fnt_unlock();
        return FER_CACHE_FULL;
    }
    

    /* we have enough memory now, create a new cache object */
    pt_cache = fe_cache_new_obj(ui4_bmp_size);
    
    if(pt_cache != NULL)
    {
        pt_cache->pt_fnt_rsc = pt_fnt_obj->pt_fnt_rsc;
        pt_cache->t_fnt_size = pt_fnt_obj->e_size;
        pt_cache->ui4_glyph_idx = ui4_idx;
        pt_cache->ui4_mem_size = ui4_cache_obj_mem;
#if !defined(FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT) || !defined(FE_DISABLE_EDGE_EFFECT_SUPPORT)
        pt_cache->t_fnt_style = pt_fnt_obj->e_style;
        pt_cache->ui2_outline_width = pt_fnt_obj->ui2_outline_width;
        pt_cache->ui2_uniform_width = pt_fnt_obj->ui2_uniform_width;
        if (pt_cache->t_fnt_style & FE_FNT_STYLE_BLURRED)
        {
            pt_cache->ui2_bld_w = pt_fnt_obj->ui2_bld_w ; 
        }
        else
        {
             pt_cache->ui2_bld_w = 0;
        }
#endif
        pt_cache->e_render_mode = e_render_mode;
        pt_cache->ui1_font_id = pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;

        if (pt_fnt_obj->e_size == FE_FNT_SIZE_CUSTOM)
        {
            pt_cache->ui1_custom_size = pt_fnt_obj->ui1_custom_size;
        }

        pui1_bmp = pt_cache->t_fnt_glyph.t_bmp.pui1_buf;
        
        pt_cache->t_fnt_glyph.i4_left         = pt_ft_glyph->bitmap_left;
        pt_cache->t_fnt_glyph.i4_top          = pt_ft_glyph->bitmap_top;
        pt_cache->t_fnt_glyph.t_bmp.i4_height = (INT32)(pt_ft_glyph->bitmap.rows);
        pt_cache->t_fnt_glyph.t_bmp.i4_width  = (INT32)(pt_ft_glyph->bitmap.width);
        pt_cache->t_fnt_glyph.t_bmp.i4_pitch  = (INT32)(pt_ft_glyph->bitmap.pitch);
        pt_cache->t_fnt_glyph.t_bmp.e_mode    = (FE_PIXEL_MODE_T)(pt_ft_glyph->bitmap.pixel_mode);
        pt_cache->t_fnt_glyph.t_bmp.pui1_buf  = (UINT8*)(pt_ft_glyph->bitmap.buffer);
        pt_cache->t_fnt_glyph.t_advance.i4_x  = (INT32)pt_ft_glyph->advance.x;
        pt_cache->t_fnt_glyph.t_advance.i4_y  = (INT32)pt_ft_glyph->advance.y;
        pt_cache->t_fnt_glyph.t_bmp.pui1_buf = pui1_bmp;
        x_memcpy(pui1_bmp, pt_ft_glyph->bitmap.buffer, ui4_bmp_size);
        gi4_avail_mem -= ui4_cache_obj_mem;

        /* add into cache queue */
        SLIST_INSERT_HEAD(pt_cache,
                          &(gt_fnt_cache_q[ui1_hash]),
                          t_fnt_cache_lnk);
    }
    
    pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;
    fe_cache_unlock();
    fe_fnt_unlock();
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fe_cache_lookup
 *
 * Description: Lookup a character in cache list.
 *              If found, return the glyph bitmap.
 *              If not, load the glyph, cache it, and return the glyph.
 *
 * Inputs:  pt_fnt_obj          Pointer of the font object.
 *          cc_char_code        Character to search.
 *
 * Outputs: pt_glyph            Glyph object.
 *
 * Returns: FER_OK              Success.
 *          FER_NO_CHAR_CODE    The char_code does not exist.
 ----------------------------------------------------------------------------*/
INT32 fe_cache_lookup(
    FE_FNT_OBJ_T*   pt_fnt_obj,
    CHAR_CODE       cc_char_code,
    FE_GLYPH_T*     pt_glyph)
{
    UINT32          ui4_idx;
    UINT8           ui1_hash;
    FE_CACHE_OBJ_T* pt_cache;
    UINT32          ui4_cache_obj_mem;
    INT32           i4_ret = FER_OK;
    FT_Error        e_error;
    FT_GlyphSlot    pt_ft_glyph;
    UINT32          ui4_bmp_size;
    UINT8*          pui1_bmp;
    FE_GLYPH_T*     pt_cache_glyph;
    UINT8           ui1_fid = 0, ui1_ori_fid = 0;

    FNT_ASSERT(pt_fnt_obj!=NULL && pt_glyph!=NULL);

#ifdef DEBUG
    ui4_access ++;
#endif

    fe_fnt_lock();

    ui4_idx = fe_get_char_index(pt_fnt_obj, cc_char_code, &ui1_fid);

    if (ui4_idx == 0 && cc_char_code != 0 && pt_fnt_obj->pt_fnt_rsc->t_unknown_char.b_usr_set)
    {
        fe_fnt_unlock();
        return FER_NO_CHAR_CODE;
    }    
    
    ui1_ori_fid = pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;

    pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_fid;

    ui1_hash = (UINT8)FNT_CACHE_HASH(ui4_idx);

    pt_cache_glyph = fe_cache_search(pt_fnt_obj, ui4_idx, pt_glyph->e_render_mode, cc_char_code);
    if(pt_cache_glyph != NULL)
    {
        *pt_glyph = *pt_cache_glyph;    /* data copy */
        pt_glyph->b_from_cache = TRUE;
        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;      
        fe_fnt_unlock();
        return FER_OK;
    }
#ifdef DEBUG
    ui4_miss ++;
#endif
    pt_glyph->b_from_cache = FALSE;

#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    fe_chk_synthesize_style_prolog(pt_fnt_obj);
#endif
    e_error = FT_Load_Char(FE_CURRENT_FONT(pt_fnt_obj).pt_face,
                           cc_char_code,
                           (pt_glyph->e_render_mode & FE_RENDER_MODE_MONO)?
                                (FT_LOAD_RENDER | FT_LOAD_MONOCHROME):
                                FT_LOAD_RENDER);
    if(e_error || (ui4_idx==0 && cc_char_code != 0))
    {
        i4_ret = FER_OK;       
        /* Pass through to show an empty square as bitmap */
        pt_glyph->i4_left = 0;
        pt_glyph->i4_top  = 0;
        pt_glyph->t_advance.i4_x = 0;
        pt_glyph->t_advance.i4_y = 0;
        
        x_memset((VOID*) &pt_glyph->t_bmp, 0, sizeof(FE_BITMAP_T));
        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;
        fe_fnt_unlock();
        return i4_ret;             
    }

    /* copy the data for client */
    pt_ft_glyph = FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph;

#ifndef FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT
    fe_chk_synthesize_style_epilog(pt_fnt_obj);
#endif

#ifndef FE_DISABLE_EDGE_EFFECT_SUPPORT
    fe_synthesize_edge_effect(pt_fnt_obj);
#endif

    pt_glyph->i4_left         = pt_ft_glyph->bitmap_left;
    pt_glyph->i4_top          = pt_ft_glyph->bitmap_top;
    pt_glyph->t_bmp.i4_height = (INT32)(pt_ft_glyph->bitmap.rows);
    pt_glyph->t_bmp.i4_width  = (INT32)(pt_ft_glyph->bitmap.width);
    pt_glyph->t_bmp.i4_pitch  = (INT32)(pt_ft_glyph->bitmap.pitch);
    pt_glyph->t_bmp.e_mode    = (FE_PIXEL_MODE_T)(pt_ft_glyph->bitmap.pixel_mode);
    pt_glyph->t_bmp.pui1_buf  = (UINT8*)(pt_ft_glyph->bitmap.buffer);
    pt_glyph->t_advance.i4_x  = (INT32)pt_ft_glyph->advance.x;
    pt_glyph->t_advance.i4_y  = (INT32)pt_ft_glyph->advance.y;

    /* the following is to cache it */
    pt_ft_glyph = FE_CURRENT_FONT(pt_fnt_obj).pt_face->glyph;
    ui4_bmp_size = (UINT32)(pt_ft_glyph->bitmap.pitch * pt_ft_glyph->bitmap.rows);
#ifndef FE_DISABLE_EDGE_EFFECT_SUPPORT
    /* edge effect takes twice the size */
    if (pt_fnt_obj->e_style & FE_FNT_STYLE_EDGE_EFFECT)
    {
        ui4_bmp_size *= 2;
    }
#endif
    ui4_cache_obj_mem = sizeof(FE_CACHE_OBJ_T) + ui4_bmp_size;

    /* get enough cache object memory */
    fe_cache_lock();

    fe_cache_cleanup_mem(ui4_cache_obj_mem);
    if(gi4_avail_mem < ui4_cache_obj_mem)
    {
        /* We alreay have data for client, but have no memory to cache it.
           However, it's ok to run correctly in this situation. */

        pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;
        fe_cache_unlock();
        fe_fnt_unlock();
        return i4_ret;
    }

    /* we have enough memory now, create a new cache object */
    pt_cache = fe_cache_new_obj(ui4_bmp_size);
    if(pt_cache != NULL)
    {
        pt_cache->pt_fnt_rsc = pt_fnt_obj->pt_fnt_rsc;
        pt_cache->t_fnt_size = pt_fnt_obj->e_size;
        pt_cache->ui4_glyph_idx = ui4_idx;
        pt_cache->cc_char_code = cc_char_code;
        pt_cache->ui4_mem_size = ui4_cache_obj_mem;
#if !defined(FE_DISABLE_EMBOLDEN_OBLIQUE_SUPPORT) || !defined(FE_DISABLE_EDGE_EFFECT_SUPPORT)
        pt_cache->t_fnt_style = pt_fnt_obj->e_style;
        pt_cache->ui2_outline_width = pt_fnt_obj->ui2_outline_width;
        pt_cache->ui2_uniform_width = pt_fnt_obj->ui2_uniform_width;
        if (pt_cache->t_fnt_style & FE_FNT_STYLE_BLURRED)
        {
            pt_cache->ui2_bld_w = pt_fnt_obj->ui2_bld_w ; 
        }
        else
        {
             pt_cache->ui2_bld_w = 0;
        }
#endif
        pt_cache->e_render_mode = pt_glyph->e_render_mode;
        pt_cache->ui1_font_id = pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id;

        if (pt_fnt_obj->e_size == FE_FNT_SIZE_CUSTOM)
        {
            pt_cache->ui1_custom_size = pt_fnt_obj->ui1_custom_size;
        }

        pui1_bmp = pt_cache->t_fnt_glyph.t_bmp.pui1_buf;
        pt_cache->t_fnt_glyph = *pt_glyph;
        pt_cache->t_fnt_glyph.t_bmp.pui1_buf = pui1_bmp;
        x_memcpy(pui1_bmp, pt_ft_glyph->bitmap.buffer, ui4_bmp_size);

        gi4_avail_mem -= ui4_cache_obj_mem;

        /* add into cache queue */
        SLIST_INSERT_HEAD(pt_cache,
                          &(gt_fnt_cache_q[ui1_hash]),
                          t_fnt_cache_lnk);
    }

    pt_fnt_obj->pt_fnt_rsc->ui1_current_font_id = ui1_ori_fid;
    fe_cache_unlock();
    fe_fnt_unlock();
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fe_cache_flush
 *
 * Description: Flush the font cache.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fe_cache_flush(
    VOID)
{
    FE_CACHE_OBJ_T*     pt_cache = NULL;
    UINT16              ui2_idx;

    fe_cache_lock();
    for (ui2_idx = 0; ui2_idx < (UINT16)FNT_CACHE_HASH_SIZE; ui2_idx++)
    {
        if(SLIST_IS_EMPTY(&(gt_fnt_cache_q[ui2_idx])))
        {
            continue;
        }

        while ((pt_cache = SLIST_FIRST(&(gt_fnt_cache_q[ui2_idx]))) != NULL)
        {
            SLIST_REMOVE(pt_cache, t_fnt_cache_lnk);
            gi4_avail_mem += pt_cache->ui4_mem_size;
            fe_cache_free_obj(pt_cache);
        };
    }
    fe_cache_unlock();
}


/*-----------------------------------------------------------------------------
 * Name: fe_cache_get_mem_size
 *
 * Description: Query available memory left.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: Available memory left.
 ----------------------------------------------------------------------------*/
UINT32 fe_cache_get_mem_size(
    void)
{
    return gi4_avail_mem;
}

void fe_cache_get_hit_rate(UINT32* pui4_miss, UINT32* pui4_access)
{
#ifdef DEBUG
    *pui4_miss = ui4_miss;
    *pui4_access = ui4_access;
#else
    *pui4_miss = 0;
    *pui4_access = 0;
#endif
}


#ifdef DEBUG_MEM_MONITOR
/*-----------------------------------------------------------------------------
 * Name: FE_CACHE_ALLOC
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
VOID*   FE_CACHE_ALLOC(
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
 * Name: FE_CACHE_FREE
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
VOID    FE_CACHE_FREE(
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

SIZE_T fe_dbg_get_cache_usage(
    VOID)
{
    return z_mem_usage;
}
#else
#ifdef DEBUG
SIZE_T fe_dbg_get_cache_usage(
    VOID)
{
    return z_mem_usage;
}
#endif
#endif


#endif /*FE_DISABLE_CACHE*/

