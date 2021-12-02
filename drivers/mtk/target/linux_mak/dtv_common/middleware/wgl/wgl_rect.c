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
 * $RCSfile: wgl_rect.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 17e99585ff7bb3eae562d2e75ca80dd8 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "wgl/x_wgl_rect.h"
#include "wgl/x_wgl.h"
#include "wgl/wgl_sets_cli.h"
#include "dbg/def_dbg_level_mw.h"


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_sets_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/

 
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_set(
    GL_RECT_T*              pt_rect,
    INT32                   i4_left,
    INT32                   i4_top,
    INT32                   i4_right,
    INT32                   i4_bottom)
{
    DBG_API(("{WGL(RECT)} %s: pt_rect (l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n", 
            __FUNCTION__, pt_rect, i4_left, i4_top, i4_right, i4_bottom));

    if(NULL == pt_rect)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__))
        return WGLR_INV_ARG;
    }

    if(i4_left < i4_right)
    {
        pt_rect->i4_left = i4_left;
        pt_rect->i4_right = i4_right;
    }
    else
    {
        pt_rect->i4_left = i4_right;
        pt_rect->i4_right = i4_left;
    }

    if(i4_top < i4_bottom)
    {
        pt_rect->i4_top = i4_top;
        pt_rect->i4_bottom = i4_bottom;
    }
    else
    {
        pt_rect->i4_top = i4_bottom;
        pt_rect->i4_bottom = i4_top;
    }

    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_copy(
    GL_RECT_T*              pt_dest_rect,
    const GL_RECT_T*        pt_src_rect)
{

    DBG_API(("{WGL(RECT)} %s: pt_dest_rect = 0x%.8x   pt_src_rect = 0x%.8x\n", 
        __FUNCTION__, pt_dest_rect, pt_src_rect));

    if(NULL == pt_dest_rect || NULL == pt_src_rect)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    x_memcpy(pt_dest_rect, pt_src_rect, sizeof(GL_RECT_T));
    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_inflate(
    GL_RECT_T*              pt_rect,
    INT32                   i4_cx,
    INT32                   i4_cy)
{
    DBG_API(("{WGL(RECT)} %s: pt_rect = 0x%.8x   cx = %d   cy = %d\n", __FUNCTION__, pt_rect, i4_cx, i4_cy));

    if(NULL == pt_rect)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_rect->i4_right += i4_cx;
    pt_rect->i4_bottom += i4_cy;

    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_intersect(
    GL_RECT_T*              pt_intersect_rect,
    const GL_RECT_T*        pt_src_rect1,
    const GL_RECT_T*        pt_src_rect2)
{
    DBG_API(("{WGL(RECT)} %s: pt_intersect_rect = 0x%.8x   pt_src_rect1 = 0x%.8x   pt_src_rect2 = 0x%.8x\n", 
             __FUNCTION__, pt_intersect_rect, pt_src_rect1, pt_src_rect2));

    if(NULL == pt_intersect_rect || NULL == pt_src_rect1 || NULL == pt_src_rect2)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_intersect_rect->i4_left = (pt_src_rect1->i4_left > pt_src_rect2->i4_left) ? 
                                 pt_src_rect1->i4_left : 
                                 pt_src_rect2->i4_left;
    pt_intersect_rect->i4_right = (pt_src_rect1->i4_right < pt_src_rect2->i4_right) ? 
                                  pt_src_rect1->i4_right : 
                                  pt_src_rect2->i4_right;
    pt_intersect_rect->i4_top = (pt_src_rect1->i4_top > pt_src_rect2->i4_top) ? 
                                pt_src_rect1->i4_top : 
                                pt_src_rect2->i4_top;
    pt_intersect_rect->i4_bottom = (pt_src_rect1->i4_bottom < pt_src_rect2->i4_bottom) ? 
                                   pt_src_rect1->i4_bottom : 
                                   pt_src_rect2->i4_bottom;

    if(pt_intersect_rect->i4_left >= pt_intersect_rect->i4_right)
    {
        pt_intersect_rect->i4_right = pt_intersect_rect->i4_left;
    }

    if(pt_intersect_rect->i4_top >= pt_intersect_rect->i4_bottom)
    {
        pt_intersect_rect->i4_bottom = pt_intersect_rect->i4_top;
        /*x_memset(pt_intersect_rect, 0, sizeof(GL_RECT_T));*/
    }

    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_union(
    GL_RECT_T*              pt_union_rect,
    const GL_RECT_T*        pt_src_rect1,
    const GL_RECT_T*        pt_src_rect2)
{
    DBG_API(("{WGL(RECT)} %s: pt_union_rect = 0x%.8x   pt_src_rect1 = 0x%.8x   pt_src_rect2 = 0x%.8x\n", 
            __FUNCTION__, pt_union_rect, pt_src_rect1, pt_src_rect2));

    if(NULL == pt_union_rect || NULL == pt_src_rect1 || NULL == pt_src_rect2)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_union_rect->i4_left = (pt_src_rect1->i4_left < pt_src_rect2->i4_left) ? 
                             pt_src_rect1->i4_left : 
                             pt_src_rect2->i4_left;
    pt_union_rect->i4_right = (pt_src_rect1->i4_right > pt_src_rect2->i4_right) ? 
                              pt_src_rect1->i4_right : 
                              pt_src_rect2->i4_right;
    pt_union_rect->i4_top = (pt_src_rect1->i4_top < pt_src_rect2->i4_top) ? 
                            pt_src_rect1->i4_top : 
                            pt_src_rect2->i4_top;
    pt_union_rect->i4_bottom = (pt_src_rect1->i4_bottom > pt_src_rect2->i4_bottom) ? 
                               pt_src_rect1->i4_bottom : 
                               pt_src_rect2->i4_bottom;

    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_offset(
    GL_RECT_T*              pt_rect,
    INT32                   i4_x,
    INT32                   i4_y)
{
    DBG_API(("{WGL(RECT)} %s: pt_rect = 0x%.8x   i4_x = %d   i4_y = %d\n", 
            __FUNCTION__, pt_rect, i4_x, i4_y));
    
    if(NULL == pt_rect)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_rect->i4_left += i4_x;
    pt_rect->i4_right += i4_x;
    pt_rect->i4_top += i4_y;
    pt_rect->i4_bottom += i4_y;

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_inset(
    GL_RECT_T*              pt_rect,
    INT32                   i4_left,
    INT32                   i4_top,
    INT32                   i4_right,
    INT32                   i4_bottom)
{
    DBG_API(("{WGL(RECT)} %s: pt_rect (l, t, r, b) = 0x%.8x (%d, %d, %d, %d)\n", 
            __FUNCTION__, pt_rect, i4_left, i4_top, i4_right, i4_bottom));

    if(NULL == pt_rect)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    pt_rect->i4_left += i4_left;
    pt_rect->i4_right -= i4_right;
    pt_rect->i4_top += i4_top;
    pt_rect->i4_bottom -= i4_bottom;

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_empty(
    GL_RECT_T*              pt_rect)
{
    DBG_API(("{WGL(RECT)} %s: pt_rect = 0x%.8x\n", __FUNCTION__, pt_rect));

    if(NULL == pt_rect)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Output argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    x_memset(pt_rect, 0, sizeof(GL_RECT_T));
    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_normalize(
    GL_RECT_T*              pt_rect)
{
    INT32 i4_temp;

    DBG_API(("{WGL(RECT)} %s: pt_rect = 0x%.8x\n", __FUNCTION__, pt_rect));

    if(NULL == pt_rect)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Argument should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if(pt_rect->i4_left > pt_rect->i4_right)
    {
        i4_temp = pt_rect->i4_left;
        pt_rect->i4_left = pt_rect->i4_right;
        pt_rect->i4_right = i4_temp;
    }
    
    if(pt_rect->i4_top > pt_rect->i4_bottom)
    {
        i4_temp = pt_rect->i4_top;
        pt_rect->i4_top = pt_rect->i4_bottom;
        pt_rect->i4_bottom = i4_temp;
    }

    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_equal(
    BOOL*                   pb_is_equal,
    const GL_RECT_T*        pt_rect1,
    const GL_RECT_T*        pt_rect2)
{
    DBG_API(("{WGL(RECT)} %s: pb_is_equal = 0x%.8x   pt_rect1 = 0x%.8x   pt_rect2 = 0x%.8x\n", 
            __FUNCTION__, pb_is_equal, pt_rect1, pt_rect2));

    
    if(NULL == pb_is_equal || NULL == pt_rect1 || NULL == pt_rect2)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    
    if((pt_rect1->i4_left == pt_rect2->i4_left)
        && (pt_rect1->i4_right == pt_rect2->i4_right)
        && (pt_rect1->i4_top == pt_rect2->i4_top)
        && (pt_rect1->i4_bottom == pt_rect2->i4_bottom))
    {
        *pb_is_equal = TRUE;
    }   
    else
    {
        *pb_is_equal = FALSE;
    }

    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_is_empty(
    BOOL*                   pb_is_empty,
    const GL_RECT_T*        pt_rect)
{
    DBG_API(("{WGL(RECT)} %s: pb_is_empty = 0x%.8x   pt_rect = 0x%.8x\n", 
            __FUNCTION__, pb_is_empty, pt_rect));

    if(NULL == pb_is_empty || NULL == pt_rect)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if((pt_rect->i4_left == pt_rect->i4_right) || 
        (pt_rect->i4_top == pt_rect->i4_bottom) )
    {
        *pb_is_empty = TRUE;
    }   
    else
    {
        *pb_is_empty = FALSE;
    }

    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_is_cross(
    BOOL*                   pb_is_intersected,
    const GL_RECT_T*        pt_rect1,
    const GL_RECT_T*        pt_rect2)
{
    INT32 i4_left;
    INT32 i4_right;
    INT32 i4_top;
    INT32 i4_bottom;

    DBG_API(("{WGL(RECT)} %s: pb_is_intersected = 0x%.8x   pt_rect1 = 0x%.8x   pt_rect2 = 0x%.8x\n", 
            __FUNCTION__, pb_is_intersected, pt_rect1, pt_rect2));

    if(NULL == pb_is_intersected || NULL == pt_rect1 || NULL == pt_rect2)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    i4_left = (pt_rect1->i4_left > pt_rect2->i4_left) ? 
              pt_rect1->i4_left : 
              pt_rect2->i4_left;
    i4_right = (pt_rect1->i4_right < pt_rect2->i4_right) ? 
               pt_rect1->i4_right : 
               pt_rect2->i4_right;
    i4_top = (pt_rect1->i4_top > pt_rect2->i4_top) ? 
             pt_rect1->i4_top : 
             pt_rect2->i4_top;
    i4_bottom = (pt_rect1->i4_bottom < pt_rect2->i4_bottom) ? 
                pt_rect1->i4_bottom : 
                pt_rect2->i4_bottom;

    if((i4_left >= i4_right) || (i4_top >= i4_bottom))
    {
        *pb_is_intersected = FALSE;
    }
    else
    {
        *pb_is_intersected = TRUE;
    }

    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_is_included(
    BOOL*                   pb_is_included,
    const GL_RECT_T*        pt_rect1,
    const GL_RECT_T*        pt_rect2)
{
    DBG_API(("{WGL(RECT)} %s: pb_is_included = 0x%.8x   pt_rect1 = 0x%.8x   pt_rect2 = 0x%.8x\n", 
            __FUNCTION__, pb_is_included, pt_rect1, pt_rect2));

    if(NULL == pb_is_included || NULL == pt_rect1 || NULL == pt_rect2)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if( (pt_rect2->i4_left < pt_rect1->i4_left)
        || (pt_rect2->i4_right > pt_rect1->i4_right)
        || (pt_rect2->i4_top < pt_rect1->i4_top)
        || (pt_rect2->i4_bottom > pt_rect1->i4_bottom) )
    {
        *pb_is_included = FALSE;
    }
    else
    {
        *pb_is_included = TRUE;
    }

    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *      
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
INT32 x_wgl_rect_is_point_included (
    BOOL*                   pb_is_included,
    const GL_RECT_T*        pt_rect,
    const GL_POINT_T*       pt_point)
{
    DBG_API(("{WGL(RECT)} %s: pb_is_included = 0x%.8x   pt_rect = 0x%.8x   pt_point = 0x%.8x\n", 
            __FUNCTION__, pb_is_included, pt_rect, pt_point));

    if(NULL == pb_is_included || NULL == pt_rect || NULL == pt_point)
    {
        DBG_ERROR(("{WGL(RECT)} ERR: Invalid arguments. Arguments should not be NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }

    if( (pt_point->i4_x >= pt_rect->i4_left) 
        && (pt_point->i4_x <= pt_rect->i4_right)
        && (pt_point->i4_y >= pt_rect->i4_top)
        && (pt_point->i4_y <= pt_rect->i4_bottom) )
    {
        *pb_is_included = TRUE;
    }
    else
    {
        *pb_is_included = FALSE;
    }
    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * private VIEW methods implementations
 *---------------------------------------------------------------------------*/ 
