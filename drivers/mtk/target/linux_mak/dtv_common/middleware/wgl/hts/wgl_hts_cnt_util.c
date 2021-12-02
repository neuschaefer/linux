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
 * $RCSfile: wgl_hts_cnt_util.c,v $
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
#include "wgl/wgl_bdr_tools.h"
#include "wgl/hts/wgl_hts_cnt_util.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    internal functions declarations
 ----------------------------------------------------------------------------*/
static INT32 _hts_cnt_tool_get_line_cnt_height(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row);
static INT32 _hts_cnt_tool_get_line_gap_top(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row);
static INT32 _hts_cnt_tool_get_line_gap_bottom(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row);

/*-----------------------------------------------------------------------------
                    public functions implementation
 ----------------------------------------------------------------------------*/
INT32 wgl_hts_cnt_tool_get_line_height(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row)
{
    INT32 i4_line_h =   _hts_cnt_tool_get_line_cnt_height(pt_row)
                      + _hts_cnt_tool_get_line_gap_top(pt_row)
                      + _hts_cnt_tool_get_line_gap_bottom(pt_row);

    return MAX(i4_line_h, 0);                         
}        


/*-----------------------------------------------------------------------------
                    internal functions implementation
 ----------------------------------------------------------------------------*/
static INT32 _hts_cnt_tool_get_line_cnt_height(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row)
{
    INT32           i4_ret;
    INT32           i4_line_cnt_h;
    
    i4_line_cnt_h = (INT32)pt_row->ui2_height;
    
    switch(pt_row->e_highest_obj_type)
    {
        case HTMGR_ROW_HIGHEST_OBJ_TYPE_FONT:
        {
            /* do nothing */
        }
        break;
        
        case HTMGR_ROW_HIGHEST_OBJ_TYPE_IMG:
        {
            /* do nothing, since the row content height is exactly 
               the height (ui2_height) of the heightest image section */
        }
        break;
        
        default:
            INFORM_ERR(WGLR_INTERNAL_ERROR);
    }    
    
    return i4_line_cnt_h;    
}

static INT32 _hts_cnt_tool_get_line_gap_top(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row)
{
    INT32   i4_gap_top;
    
    i4_gap_top = pt_row->pt_parag->t_gap.i4_line_gap_top;
    if(WGL_HTMGR_IS_ROW_START_OF_PARAG(pt_row))
    {
        i4_gap_top += (INT32)pt_row->pt_parag->t_gap.ui2_parag_gap_top;
    }        
    return i4_gap_top;    
}

static INT32 _hts_cnt_tool_get_line_gap_bottom(
    IN      WGL_HTMGR_ROW_CACHE_T       *pt_row)
{
    INT32   i4_gap_btm;
    
    i4_gap_btm = pt_row->pt_parag->t_gap.i4_line_gap_bottom;
    if(WGL_HTMGR_IS_ROW_END_OF_PARAG(pt_row))
    {
        i4_gap_btm += (INT32)pt_row->pt_parag->t_gap.ui2_parag_gap_bottom;
    }
    return i4_gap_btm;
}                 


