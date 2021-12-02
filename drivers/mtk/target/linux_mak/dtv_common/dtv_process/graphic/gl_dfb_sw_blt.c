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
#include "gl_dfb.h"


#include "inc/u_common.h"
#include "u_gl.h"
#include "handle/u_handle.h"
#include "stdio.h"
#include "gl_dfb_sw_draw.h"
#if 0
#include <directfb.h>
#endif
BOOL gl_calc_blt_area(
    const GL_RECT_T*    pt_src_clip, 
    INT32               i4_src_x, 
    INT32               i4_src_y, 
    const GL_RECT_T*    pt_dst_clip, 
    INT32               i4_dst_x, 
    INT32               i4_dst_y,
    UINT32              ui4_width, 
    UINT32              ui4_height,
    GL_POINT_T*         ppt_src, 
    GL_POINT_T*         ppt_dst, 
    GL_POINT_T*         ppt_size);

extern UINT8     gl_bit_count(GL_COLORMODE_T e_mode);

static VOID _copy_buffer_D565_to_16b(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    INT32           i4_width,
    INT32           i4_height,
    GL_COLORMODE_T  e_dst_cm,
    UINT8           ui1_alpha)

{
    INT32   x, y;
    UINT32  ui4_color;
    UINT16* pui2_dst = (UINT16*) pv_dst;
    UINT16* pui2_src = (UINT16*) pv_src;
    UINT32  aval;
    UINT32  rmask, gmask, bmask;
    UINT32  rshift, gshift, bshift;
    
    switch (e_dst_cm)
    {
        case GL_COLORMODE_ARGB_D1555:
            aval   = (((UINT32) ui1_alpha >> 7) << 15);
            rmask  = 0xf800;
            rshift = 1;
            gmask  = 0x07c0;
            gshift = 1;
            bmask  = 0x1f;
            bshift = 0;
            break;

        case GL_COLORMODE_ARGB_D4444:
            aval   = (((UINT32) ui1_alpha >> 4) << 12);
            rmask  = 0xf000;
            rshift = 4;
            gmask  = 0x0780;
            gshift = 3;
            bmask  = 0x1e;
            bshift = 1;
            break;

        default:
            return;
    }
    
    for (y = 0; y < i4_height; y++)
    {
        for (x = 0; x < i4_width; x++)
        {
            ui4_color = *pui2_src;
            *pui2_dst = (UINT16) (aval | ((ui4_color & rmask) >> rshift) | ((ui4_color & gmask) >> gshift) | ((ui4_color & bmask) >> bshift));
            pui2_dst  = (UINT16*) ((UINT8*) pui2_dst + i4_inc_dx);
            pui2_src  = (UINT16*) ((UINT8*) pui2_src + i4_inc_sx);
        }

        pui2_dst = (UINT16*) ((UINT8*) pv_dst + i4_inc_dy);
        pui2_src = (UINT16*) ((UINT8*) pv_src + i4_inc_sy);

        pv_dst = (UINT8*) pv_dst + i4_inc_dy;
        pv_src = (UINT8*) pv_src + i4_inc_sy;
    }
}

static VOID _copy_buffer_D565_to_D8888(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    INT32           i4_width,
    INT32           i4_height,
    UINT8           ui1_alpha)

{
    INT32   x, y;
    UINT32  ui4_color;
    UINT32* pui4_dst = (UINT32*) pv_dst;
    UINT16* pui2_src = (UINT16*) pv_src;
    UINT32  aval     = ((UINT32) ui1_alpha << 24);
        
    for (y = 0; y < i4_height; y++)
    {
        for (x = 0; x < i4_width; x++)
        {
            ui4_color = *pui2_src;
            *pui4_dst = 
                (UINT32) (aval | 
                         (((ui4_color & 0xf800) << 8) + ((ui4_color & 0xe000) << 3)) | 
                         (((ui4_color & 0x07e0) << 5) + ((ui4_color & 0x0600) >> 1)) | 
                         (((ui4_color & 0x001f) << 3) + ((ui4_color & 0x001c) >> 2)));
            pui4_dst  = (UINT32*) ((UINT8*) pui4_dst + i4_inc_dx);
            pui2_src  = (UINT16*) ((UINT8*) pui2_src + i4_inc_sx);
        }

        pui4_dst = (UINT32*) ((UINT8*) pv_dst + i4_inc_dy);
        pui2_src = (UINT16*) ((UINT8*) pv_src + i4_inc_sy);

        pv_dst = (UINT8*) pv_dst + i4_inc_dy;
        pv_src = (UINT8*) pv_src + i4_inc_sy;
    }
}

static VOID _copy_buffer_16b_to_D565(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    INT32           i4_width,
    INT32           i4_height,
    GL_COLORMODE_T  e_src_cm)

{
    INT32   x, y;
    UINT32  ui4_color;
    UINT16* pui2_dst = (UINT16*) pv_dst;
    UINT16* pui2_src = (UINT16*) pv_src;
    UINT32  rmask1, gmask1, bmask1;
    UINT32  rmask2, gmask2, bmask2;
    UINT32  rshift1, gshift1, bshift1;
    UINT32  gshift2, bshift2;
        
    switch (e_src_cm)
    {
        case GL_COLORMODE_ARGB_D1555:
            rmask1  = 0x7c00;
            rmask2  = 0;
            rshift1 = 1;
            gmask1  = 0x03e0;
            gmask2  = 0x0200;
            gshift1 = 1;
            gshift2 = 4;
            bmask1  = 0x1f;
            bmask2  = 0;
            bshift1 = 0;
            bshift2 = 0;
            break;

        case GL_COLORMODE_ARGB_D4444:
            rmask1  = 0x0f00;
            rmask2  = 0x0800;
            rshift1 = 4;
            gmask1  = 0xf0;
            gmask2  = 0xc0;
            gshift1 = 3;
            gshift2 = 1;
            bmask1  = 0xf;
            bmask2  = 0x8;
            bshift1 = 1;
            bshift2 = 3;
            break;

        default:
            return;
    }

    for (y = 0; y < i4_height; y++)
    {
        for (x = 0; x < i4_width; x++)
        {
            ui4_color = *pui2_src;
            *pui2_dst = 
                (UINT16) ((((ui4_color & rmask1) << rshift1) + (ui4_color & rmask2)) | 
                          (((ui4_color & gmask1) << gshift1) + ((ui4_color & gmask2) >> gshift2)) | 
                          (((ui4_color & bmask1) << bshift1) + ((ui4_color & bmask2) >> bshift2)));
            pui2_dst  = (UINT16*) ((UINT8*) pui2_dst + i4_inc_dx);
            pui2_src  = (UINT16*) ((UINT8*) pui2_src + i4_inc_sx);
        }

        pui2_dst = (UINT16*) ((UINT8*) pv_dst + i4_inc_dy);
        pui2_src = (UINT16*) ((UINT8*) pv_src + i4_inc_sy);

        pv_dst = (UINT8*) pv_dst + i4_inc_dy;
        pv_src = (UINT8*) pv_src + i4_inc_sy;
    }
}


static VOID _copy_buffer_16b_to_D8888(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    INT32           i4_width,
    INT32           i4_height,
    GL_COLORMODE_T  e_src_cm)

{
    const static UINT8 s_aui1_alpha_D1555[2]  = { 0, 0xff };
    const static UINT8 s_aui1_alpha_D4444[16] = { 
           0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    
    INT32   x, y;
    UINT32  ui4_color;
    UINT32* pui4_dst = (UINT32*) pv_dst;
    UINT16* pui2_src = (UINT16*) pv_src;
    UINT8*  pui1_alpha;
    UINT32  rmask1, gmask1, bmask;
    UINT32  rmask2, gmask2;
    UINT32  ashift, rshift1, gshift1, bshift1;
    UINT32  rshift2, gshift2, bshift2;
        
    switch (e_src_cm)
    {
        case GL_COLORMODE_ARGB_D1555:
            pui1_alpha = (UINT8*) s_aui1_alpha_D1555;
            ashift  = 15;
            rmask1  = 0x7c00;
            rmask2  = 0x7000;
            rshift1 = 9;
            rshift2 = 4;
            gmask1  = 0x03e0;
            gmask2  = 0x0380;
            gshift1 = 6;
            gshift2 = 1;
            bmask   = 0x1f;
            bshift1 = 3;
            bshift2 = 2;
            break;

        case GL_COLORMODE_ARGB_D4444:
            pui1_alpha = (UINT8*) s_aui1_alpha_D4444;
            ashift  = 12;
            rmask1  = 0x0f00;
            rmask2  = 0x0f00;
            rshift1 = 12;
            rshift2 = 8;
            gmask1  = 0xf0;
            gmask2  = 0xf0;
            gshift1 = 8;
            gshift2 = 4;
            bmask   = 0xf;
            bshift1 = 4;
            bshift2 = 0;
            break;

        default:
            return;
    }

    for (y = 0; y < i4_height; y++)
    {
        for (x = 0; x < i4_width; x++)
        {
            ui4_color = *pui2_src;
            *pui4_dst = 
                (UINT32) ((((UINT32) pui1_alpha[ui4_color >> ashift]) << 24) | 
                         (((ui4_color & rmask1) << rshift1) + ((ui4_color & rmask2) << rshift2)) | 
                         (((ui4_color & gmask1) << gshift1) + ((ui4_color & gmask2) << gshift2)) | 
                         (((ui4_color & bmask)  << bshift1) + ((ui4_color & bmask) >> bshift2)));
            pui4_dst  = (UINT32*) ((UINT8*) pui4_dst + i4_inc_dx);
            pui2_src  = (UINT16*) ((UINT8*) pui2_src + i4_inc_sx);
        }

        pui4_dst = (UINT32*) ((UINT8*) pv_dst + i4_inc_dy);
        pui2_src = (UINT16*) ((UINT8*) pv_src + i4_inc_sy);

        pv_dst = (UINT8*) pv_dst + i4_inc_dy;
        pv_src = (UINT8*) pv_src + i4_inc_sy;
    }
}

static VOID _copy_buffer_D1555_to_D4444(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    INT32           i4_width,
    INT32           i4_height)

{
    const static UINT8 s_aui1_alpha_D1555[2]  = { 0, 0xf };
    
    INT32   x, y;
    UINT32  ui4_color;
    UINT16* pui2_dst = (UINT16*) pv_dst;
    UINT16* pui2_src = (UINT16*) pv_src;
    
    for (y = 0; y < i4_height; y++)
    {
        for (x = 0; x < i4_width; x++)
        {
            ui4_color = *pui2_src;
            *pui2_dst = 
                (UINT16) ((((UINT32) s_aui1_alpha_D1555[ui4_color >> 15]) << 12) | 
                          ((ui4_color & 0x7800) >> 3) | 
                          ((ui4_color & 0x03c0) >> 2) | 
                          ((ui4_color & 0x001e) >> 1));
            pui2_dst  = (UINT16*) ((UINT8*) pui2_dst + i4_inc_dx);
            pui2_src  = (UINT16*) ((UINT8*) pui2_src + i4_inc_sx);
        }

        pui2_dst = (UINT16*) ((UINT8*) pv_dst + i4_inc_dy);
        pui2_src = (UINT16*) ((UINT8*) pv_src + i4_inc_sy);

        pv_dst = (UINT8*) pv_dst + i4_inc_dy;
        pv_src = (UINT8*) pv_src + i4_inc_sy;
    }
}

static VOID _copy_buffer_D4444_to_D1555(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    INT32           i4_width,
    INT32           i4_height)

{
    INT32   x, y;
    UINT32  ui4_color;
    UINT16* pui2_dst = (UINT16*) pv_dst;
    UINT16* pui2_src = (UINT16*) pv_src;
    
    for (y = 0; y < i4_height; y++)
    {
        for (x = 0; x < i4_width; x++)
        {
            ui4_color = *pui2_src;
            *pui2_dst = 
                (UINT16) ((ui4_color & 0x8000) | 
                          (((ui4_color & 0x0f00) << 3) + ((ui4_color & 0x0800) >> 1)) | 
                          (((ui4_color & 0x00f0) << 2) + ((ui4_color & 0x0080) >> 2)) | 
                          (((ui4_color & 0x000f) << 1) + ((ui4_color & 0x0008) >> 3)));
            pui2_dst  = (UINT16*) ((UINT8*) pui2_dst + i4_inc_dx);
            pui2_src  = (UINT16*) ((UINT8*) pui2_src + i4_inc_sx);
        }

        pui2_dst = (UINT16*) ((UINT8*) pv_dst + i4_inc_dy);
        pui2_src = (UINT16*) ((UINT8*) pv_src + i4_inc_sy);

        pv_dst = (UINT8*) pv_dst + i4_inc_dy;
        pv_src = (UINT8*) pv_src + i4_inc_sy;
    }
}

static VOID _copy_buffer_D8888_to_16b(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    INT32           i4_width,
    INT32           i4_height,
    GL_COLORMODE_T  e_dst_cm)

{
    INT32   x, y;
    UINT32  ui4_color;
    UINT16* pui2_dst = (UINT16*) pv_dst;
    UINT32* pui4_src = (UINT32*) pv_src;
    UINT32  amask, rmask, gmask, bmask;
    UINT32  ashift, rshift, gshift, bshift;
    
    switch (e_dst_cm)
    {
        case GL_COLORMODE_RGB_D565:
            amask  = 0;
            ashift = 0;
            rmask  = 0xf80000;
            rshift = 8;
            gmask  = 0xfc00;
            gshift = 5;
            bmask  = 0xf8;
            bshift = 3;
            break;
            
        case GL_COLORMODE_ARGB_D1555:
            amask  = 0x80000000;
            ashift = 16;
            rmask  = 0xf80000;
            rshift = 9;
            gmask  = 0xf800;
            gshift = 6;
            bmask  = 0xf8;
            bshift = 3;
            break;

        case GL_COLORMODE_ARGB_D4444:
            amask  = 0xf0000000;
            ashift = 16;
            rmask  = 0xf00000;
            rshift = 12;
            gmask  = 0xf000;
            gshift = 8;
            bmask  = 0xf0;
            bshift = 4;
            break;

        default:
            return;
    }
    
    for (y = 0; y < i4_height; y++)
    {
        for (x = 0; x < i4_width; x++)
        {
            ui4_color = *pui4_src;
            *pui2_dst = (UINT16) (((ui4_color & amask) >> ashift) | ((ui4_color & rmask) >> rshift) | ((ui4_color & gmask) >> gshift) | ((ui4_color & bmask) >> bshift));
            pui2_dst  = (UINT16*) ((UINT8*) pui2_dst + i4_inc_dx);
            pui4_src  = (UINT32*) ((UINT8*) pui4_src + i4_inc_sx);
        }

        pui2_dst = (UINT16*) ((UINT8*) pv_dst + i4_inc_dy);
        pui4_src = (UINT32*) ((UINT8*) pv_src + i4_inc_sy);

        pv_dst = (UINT8*) pv_dst + i4_inc_dy;
        pv_src = (UINT8*) pv_src + i4_inc_sy;
    }
}

static VOID _copy_buffer_8b(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    INT32           i4_width,
    INT32           i4_height)

{
    INT32   x, y;
    UINT8*  pui1_dst = (UINT8*) pv_dst;
    UINT8*  pui1_src = (UINT8*) pv_src;
    
    for (y = 0; y < i4_height; y++)
    {
        for (x = 0; x < i4_width; x++)
        {
            *pui1_dst = *pui1_src;
            
            pui1_dst += i4_inc_dx;
            pui1_src += i4_inc_sx;
        }

        pui1_dst = (UINT8*) pv_dst + i4_inc_dy;
        pui1_src = (UINT8*) pv_src + i4_inc_sy;

        pv_dst = (UINT8*) pv_dst + i4_inc_dy;
        pv_src = (UINT8*) pv_src + i4_inc_sy;
    }
}

static VOID _copy_buffer_16b(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    INT32           i4_width,
    INT32           i4_height)

{
    INT32   x, y;
    UINT16* pui2_dst = (UINT16*) pv_dst;
    UINT16* pui2_src = (UINT16*) pv_src;
    
    for (y = 0; y < i4_height; y++)
    {
        for (x = 0; x < i4_width; x++)
        {
            *pui2_dst = *pui2_src;
            
            pui2_dst = (UINT16*) ((UINT8*) pui2_dst + i4_inc_dx);
            pui2_src = (UINT16*) ((UINT8*) pui2_src + i4_inc_sx);
        }

        pui2_dst = (UINT16*) ((UINT8*) pv_dst + i4_inc_dy);
        pui2_src = (UINT16*) ((UINT8*) pv_src + i4_inc_sy);

        pv_dst = (UINT8*) pv_dst + i4_inc_dy;
        pv_src = (UINT8*) pv_src + i4_inc_sy;
    }
}

static VOID _copy_buffer_32b(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    INT32           i4_width,
    INT32           i4_height)

{
    INT32   x, y;
    UINT32* pui4_dst = (UINT32*) pv_dst;
    UINT32* pui4_src = (UINT32*) pv_src;
    
    for (y = 0; y < i4_height; y++)
    {
        for (x = 0; x < i4_width; x++)
        {
            *pui4_dst = *pui4_src;
            
            pui4_dst = (UINT32*) ((UINT8*) pui4_dst + i4_inc_dx);
            pui4_src = (UINT32*) ((UINT8*) pui4_src + i4_inc_sx);
        }

        pui4_dst = (UINT32*) ((UINT8*) pv_dst + i4_inc_dy);
        pui4_src = (UINT32*) ((UINT8*) pv_src + i4_inc_sy);

        pv_dst = (UINT8*) pv_dst + i4_inc_dy;
        pv_src = (UINT8*) pv_src + i4_inc_sy;
    }
}


static INT32 _copy_buffer(
    VOID*           pv_dst,
    INT32           i4_inc_dx,
    INT32           i4_inc_dy,
    GL_COLORMODE_T  e_dst_cm,
    VOID*           pv_src,
    INT32           i4_inc_sx,
    INT32           i4_inc_sy,
    GL_COLORMODE_T  e_src_cm,
    INT32           i4_width,
    INT32           i4_height,
    UINT8           ui1_alpha)
{
    if (e_src_cm == GL_COLORMODE_ARGB_CLUT8)
    {
        if (e_dst_cm == GL_COLORMODE_ARGB_CLUT8)
        {
            _copy_buffer_8b(
                pv_dst,
                i4_inc_dx,
                i4_inc_dy,
                pv_src,
                i4_inc_sx,
                i4_inc_sy,
                i4_width,
                i4_height);
        }
        else
        {
            return GLR_INV_COLORMODE;
        }
    }
    else if (e_src_cm == GL_COLORMODE_RGB_D565)
    {
        switch (e_dst_cm)
        {
            case GL_COLORMODE_RGB_D565:
                _copy_buffer_16b(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height);
                break;
                
            case GL_COLORMODE_ARGB_D1555:
            case GL_COLORMODE_ARGB_D4444:
                _copy_buffer_D565_to_16b(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height,
                    e_dst_cm,
                    ui1_alpha);
                break;
                
            case GL_COLORMODE_ARGB_D8888:
                _copy_buffer_D565_to_D8888(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height,
                    ui1_alpha);
                break;
                
            default:
                return GLR_INV_COLORMODE;
        }
    }
    else if (e_src_cm == GL_COLORMODE_ARGB_D1555)
    {
        switch (e_dst_cm)
        {
            case GL_COLORMODE_RGB_D565:
                _copy_buffer_16b_to_D565(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height,
                    GL_COLORMODE_ARGB_D1555);
                break;
                
            case GL_COLORMODE_ARGB_D1555:
                _copy_buffer_16b(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height);
                break;
                
            case GL_COLORMODE_ARGB_D4444:
                _copy_buffer_D1555_to_D4444(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height);
                break;
                
            case GL_COLORMODE_ARGB_D8888:
                _copy_buffer_16b_to_D8888(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height,
                    GL_COLORMODE_ARGB_D1555);
                break;
                
            default:
                return GLR_INV_COLORMODE;
        }
    }
    else if (e_src_cm == GL_COLORMODE_ARGB_D4444)
    {
        switch (e_dst_cm)
        {
            case GL_COLORMODE_RGB_D565:
                _copy_buffer_16b_to_D565(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height,
                    GL_COLORMODE_ARGB_D4444);
                break;
                
            case GL_COLORMODE_ARGB_D1555:
                _copy_buffer_D4444_to_D1555(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height);
                break;
                
            case GL_COLORMODE_ARGB_D4444:
                _copy_buffer_16b(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height);
                break;
                
            case GL_COLORMODE_ARGB_D8888:
                _copy_buffer_16b_to_D8888(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height,
                    GL_COLORMODE_ARGB_D4444);
                break;
                
            default:
                return GLR_INV_COLORMODE;
        }
    }
#if 0 /* MHF - Linux Basara for mmp */
    else if (e_src_cm == GL_COLORMODE_ARGB_D8888)
#else
    else if (e_src_cm == GL_COLORMODE_ARGB_D8888 || e_src_cm == GL_COLORMODE_AYUV_D8888)
#endif
    {
        switch (e_dst_cm)
        {
            case GL_COLORMODE_RGB_D565:
            case GL_COLORMODE_ARGB_D1555:
            case GL_COLORMODE_ARGB_D4444:
                _copy_buffer_D8888_to_16b(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height,
                    e_dst_cm);
                break;
                
            case GL_COLORMODE_ARGB_D8888:
#if 1 /* MHF - Linux Basara for mmp */
            case GL_COLORMODE_AYUV_D8888:
#endif
                _copy_buffer_32b(
                    pv_dst,
                    i4_inc_dx,
                    i4_inc_dy,
                    pv_src,
                    i4_inc_sx,
                    i4_inc_sy,
                    i4_width,
                    i4_height);
                break;
                
            default:
                return GLR_INV_COLORMODE;
        }
    }

    return GLR_OK;
}


INT32 gl_sw_blt_surface_with_rotation(
    GL_DFB_SURFACE_T    * p_dst_surf,
    GL_DFB_SURFACE_T    * p_src_surf,
    INT32               i4_src_x,
    INT32               i4_src_y,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    BOOL                b_flip,
    UINT32              ui4_angle,
    UINT32              ui4_flag)
{
    #define NUM_SURFACE     2
    
    GL_RECT_T               t_src_clip, t_dst_clip;
    GL_POINT_T              t_src, t_dst, t_size;
    INT32                   i4_ret = GLR_OK;
/*    UINT8                   ui1_src, ui1_dst;*/
    /*UINT8                   ui1_num;*/
    
    void *                  pv_dst_buff;
    void *                  pv_src_buff;
       
    GL_RECT_T   t_rc_trfm;
    INT32       i4_surf_w, i4_surf_h;
    INT32       x, y;
    UINT32      w, h;
    BOOL        b_90_degree = (ui4_angle == GL_BLT_ROTATE_CW90 || ui4_angle == GL_BLT_ROTATE_CW270);
    
    i4_surf_w = (INT32) (p_src_surf->t_desc.ui4_width);
    i4_surf_h = (INT32) (p_src_surf->t_desc.ui4_height);
    
    t_src_clip = p_src_surf->t_clip_rect;
    t_dst_clip = p_dst_surf->t_clip_rect;
    
    t_rc_trfm = t_src_clip;
    w = ui4_width;
    h = ui4_height;
    
    if (ui4_angle == GL_BLT_ROTATE_NONE || ui4_angle == GL_BLT_ROTATE_CW180)
    {
        if (ui4_angle == GL_BLT_ROTATE_NONE)
        {
            x  = i4_src_x;
            y  = i4_src_y;
            t_rc_trfm = t_src_clip;
        }
        else /* ui4_angle == GL_BLT_ROTATE_CW180 */
        {
            x = i4_surf_w - i4_src_x - (INT32) ui4_width;
            y = i4_surf_h - i4_src_y - (INT32) ui4_height;
            SET_RECT_BY_SIZE(
                &t_rc_trfm, 
                i4_surf_w - t_src_clip.i4_right, 
                i4_surf_h - t_src_clip.i4_bottom, 
                RECT_W(&t_src_clip), 
                RECT_H(&t_src_clip));
        }
    }
    else /* ui4_angle == GL_BLT_ROTATE_CW90 || ui4_angle == GL_BLT_ROTATE_CW270 */
    {    

        x = i4_src_x;
        y = i4_src_y;
        if (ui4_angle == GL_BLT_ROTATE_CW90)
        {
            SET_RECT_BY_SIZE(
                &t_rc_trfm, 
                i4_surf_h - t_src_clip.i4_bottom, 
                t_src_clip.i4_left, 
                RECT_W(&t_src_clip), 
                RECT_H(&t_src_clip));
        }
        else  /* ui4_angle == GL_BLT_ROTATE_CW270 */
        {
            SET_RECT_BY_SIZE(
                &t_rc_trfm, 
                t_src_clip.i4_top,
                i4_surf_w - t_src_clip.i4_right, 
                RECT_W(&t_src_clip), 
                RECT_H(&t_src_clip));
        }
    }

    if(gl_calc_blt_area_with_rotation(
            &t_rc_trfm, 
            x, 
            y, 
            &t_dst_clip, 
            i4_dst_x, 
            i4_dst_y, 
            w, 
            h, 
            ui4_angle,
            &t_src, 
            &t_dst, 
            &t_size))
    {
        INT32           i4_inc_sx, i4_inc_sy;
        INT32           i4_inc_dx, i4_inc_dy;
        int             i4_src_pitch;
        int             i4_dst_pitch;

        VOID            *pv_src, *pv_dst;
		#if 0
        DFBCHECK(p_dst_surf->p_dfb_surf->Lock(p_dst_surf->p_dfb_surf, DSLF_WRITE|DSLF_READ, &pv_dst_buff, &i4_dst_pitch), &i4_ret);
		#else
		i4_dst_pitch = (INT32)p_dst_surf->ui4_pitch;
		pv_dst_buff  = (VOID*)p_dst_surf->p_dfb_surf->bmp_buffer.u4VirtAddr;
		#endif
        RETURN_RET_IF_FAIL(i4_ret);

        if (p_dst_surf == p_src_surf)
        {
            pv_src_buff = pv_dst_buff;
            i4_src_pitch = i4_dst_pitch;
        }
        else
        {
        	#if 0
            DFBCHECK(p_src_surf->p_dfb_surf->Lock(p_src_surf->p_dfb_surf, DSLF_READ, &pv_src_buff, &i4_src_pitch), &i4_ret);
            if(i4_ret != GLR_OK)
            {
    
                p_dst_surf->p_dfb_surf->Unlock(p_src_surf->p_dfb_surf);
                return i4_ret;
            }
			#else
			i4_src_pitch = (INT32)p_src_surf->ui4_pitch;
			pv_src_buff  = (VOID*)p_src_surf->p_dfb_surf->bmp_buffer.u4VirtAddr;
			#endif
        }
        
        i4_inc_dx = gl_bit_count(p_dst_surf->t_desc.e_clrmode) / 8;
        i4_inc_dy = i4_dst_pitch;

        pv_dst = ((UINT8*) pv_dst_buff) + t_dst.i4_y * i4_inc_dy + t_dst.i4_x * i4_inc_dx;

        w = (UINT32) gl_bit_count(p_src_surf->t_desc.e_clrmode) / 8;
        h = i4_src_pitch;
        
        switch (ui4_angle)
        {
            case GL_BLT_ROTATE_NONE:
                if (b_flip)
                {
                    i4_inc_sx = - (INT32) w;
                    i4_inc_sy =   (INT32) h;
                    pv_src    = 
                        ((UINT8*) pv_src_buff) + 
                        t_src.i4_y * h + (t_src.i4_x + t_size.i4_x - 1) * w;
                }
                else
                {
                    i4_inc_sx = (INT32) w;
                    i4_inc_sy = (INT32) h;
                    pv_src    = 
                        ((UINT8*) pv_src_buff) + 
                        t_src.i4_y * h + t_src.i4_x * w;
                }
                break;

            case GL_BLT_ROTATE_CW90:
                if (b_flip)
                {
                    i4_inc_sx = - (INT32) h;
                    i4_inc_sy = - (INT32) w;
                    pv_src    = 
                        ((UINT8*) pv_src_buff) + 
                        (t_src.i4_y + t_size.i4_y - 1) * h + (t_src.i4_x + t_size.i4_x) * w;
                }
                else
                {
                    i4_inc_sx = - (INT32) h;
                    i4_inc_sy =   (INT32) w;
                    pv_src    = 
                        ((UINT8*) pv_src_buff) + 
                        (t_src.i4_y + t_size.i4_y - 1) * h + t_src.i4_x * w;
                }
                break;

            case GL_BLT_ROTATE_CW180:
                if (b_flip)
                {
                    i4_inc_sx =   (INT32) w;
                    i4_inc_sy = - (INT32) h;
                    pv_src    = 
                        ((UINT8*) pv_src_buff) + 
                        (i4_surf_h - 1 - t_src.i4_y) * h + (i4_surf_w - 1 - t_src.i4_x - t_size.i4_x) * w;
                }
                else
                {
                    i4_inc_sx = - (INT32) w;
                    i4_inc_sy = - (INT32) h;
                    pv_src    = 
                        ((UINT8*) pv_src_buff) + 
                        (i4_surf_h - 1 - t_src.i4_y) * h + (i4_surf_w - 1 - t_src.i4_x) * w;
                }
                break;

            case GL_BLT_ROTATE_CW270:
                if (b_flip)
                {
                    i4_inc_sx =   (INT32) h;
                    i4_inc_sy =   (INT32) w;
                    pv_src    = 
                        ((UINT8*) pv_src_buff) + 
                        t_src.i4_y * h + (t_src.i4_x - 1) * w;
                }
                else
                {
                    i4_inc_sx =   (INT32) h;
                    i4_inc_sy = - (INT32) w;
                    pv_src    = 
                        ((UINT8*) pv_src_buff) + 
                        t_src.i4_y * h + (t_src.i4_x + t_size.i4_x - 1) * w;
                }
                break;

            default: /* should not arrive here */
                return GLR_INV_ARG;
        }
        if (!b_90_degree)
        {
            i4_ret = _copy_buffer(
                pv_dst,
                i4_inc_dx,
                i4_inc_dy,
                p_dst_surf->t_desc.e_clrmode,
                pv_src,
                i4_inc_sx,
                i4_inc_sy,
                p_src_surf->t_desc.e_clrmode,
                t_size.i4_x,
                t_size.i4_y,
                ui1_alpha_assigned);
        }
        else
        {
        i4_ret = _copy_buffer(
                pv_dst,
                i4_inc_dx,
                i4_inc_dy,
                p_dst_surf->t_desc.e_clrmode,
                pv_src,
                i4_inc_sx,
                i4_inc_sy,
                p_src_surf->t_desc.e_clrmode,
                t_size.i4_y,
                t_size.i4_x,
                ui1_alpha_assigned);
        }
    }
    #if 0
    if (p_src_surf != p_dst_surf)
    {
        p_src_surf->p_dfb_surf->Unlock(p_src_surf->p_dfb_surf);
    }
    p_dst_surf->p_dfb_surf->Unlock(p_dst_surf->p_dfb_surf);
    #endif 
    return (i4_ret);
}

