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
 * $RCSfile: sbtl_widget.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         This header file contains Subtitle Stream handler specific 
 *         events.
 *---------------------------------------------------------------------------*/

#ifndef _SBTL_WIDGET_H_
#define _SBTL_WIDGET_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "wgl/text/u_wgl_text.h"
#include "wgl/subtitle/u_wgl_sbtl.h"
#include "wgl/x_wgl.h"
#include "wgl/u_wgl_tools.h"
#include "wgl/u_wgl_obj_type.h"
#include "unicode/u_uc_str.h"
#else
#include "u_uc_str.h"
#endif

#ifndef __KERNEL__
#define SBTL_WGL_PACK(_x)                       WGL_PACK(_x)

#define SBTL_WGL_CMD_SBTL_FLIP                  WGL_CMD_SBTL_FLIP
#define SBTL_WGL_CMD_SBTL_CLEAR_ALL             WGL_CMD_SBTL_CLEAR_ALL
#define SBTL_WGL_CMD_GL_SET_FONT                WGL_CMD_GL_SET_FONT
#define SBTL_WGL_CMD_TEXT_SET_ALIGN             WGL_CMD_TEXT_SET_ALIGN
#define SBTL_WGL_CMD_TEXT_SET_TEXT              WGL_CMD_TEXT_SET_TEXT
#define SBTL_WGL_CMD_GL_SET_COLOR               WGL_CMD_GL_SET_COLOR
#define SBTL_WGL_CMD_TEXT_SET_STR               WGL_CMD_TEXT_SET_STRING
#define SBTL_WGL_CMD_TEXT_SET_PREV_STR          WGL_CMD_TEXT_SET_STRING_PREV
#define SBTL_WGL_CMD_TEXT_CLEAR_STR             WGL_CMD_TEXT_STRING_CLEAR
#define SBTL_WGL_CMD_TEXT_HLT_POS               WGL_CMD_TEXT_SET_HIGHLIGHT_POSTION
#define SBTL_WGL_CMD_TEXT_HLT_STR               WGL_CMD_TEXT_SET_HIGHLIGHT_STRING
#define SBTL_WGL_CMD_TEXT_PLAY_NEXT_STR         WGL_CMD_TEXT_PLAY_NEXT_STRING
#define SBTL_WGL_CMD_TEXT_PLAY_PREV_STR         WGL_CMD_TEXT_PLAY_PREV_STRING
#define SBTL_WGL_CMD_TEXT_SET_INTERLACED        WGL_CMD_TEXT_SET_INTERLACED
#define SBTL_WGL_CMD_TEXT_GET_INTERLACED        WGL_CMD_TEXT_GET_INTERLACED
#define SBTL_WGL_CMD_TEXT_SET_LINE_GAP          WGL_CMD_TEXT_SET_LINE_GAP
#define SBTL_WGL_CMD_TEXT_GET_LINE_GAP          WGL_CMD_TEXT_GET_LINE_GAP
#define SBTL_WGL_CMD_TEXT_GET_FONT_SIZE         WGL_CMD_TEXT_GET_FONT_POINT_SIZE_FROM_HEIGHT
#define SBTL_WGL_CMD_TEXT_SET_DISPLAY_START     WGL_CMD_TEXT_SET_DISPLAY_START_EX

/*Color Command Target*/
#define SBTL_WGL_CLR_BK                         WGL_CLR_BK
#define SBTL_WGL_CLR_TEXT                       WGL_CLR_TEXT
#define SBTL_WGL_CLR_EDGE                       WGL_CLR_EDGE
#define SBTL_WGL_CLR_TEXT_BG                    WGL_CLR_TEXT_BG
#define SBTL_WGL_CLR_WGT_SPF_START              WGL_CLR_WGT_SPF_START
#define SBTL_WGL_CLR_CUSTOM_START               WGL_CLR_CUSTOM_START


#define SBTL_WGL_WIDGET_TEXT                    HT_WGL_WIDGET_TEXT
#define SBTL_WGL_WIDGET_SBTL                    HT_WGL_WIDGET_SBTL

#define SBTL_WGL_TEXT_POSITION_BY_STR_IDX       WGL_TEXT_POSITION_BY_STRING_INDEX
#define SBTL_WGL_TEXT_POSITION_BY_INDICATE      WGL_TEXT_POSITION_BY_INDICATE


#define SBTL_WGL_BORDER_NULL                    WGL_BORDER_NULL
#define SBTL_WGL_BORDER_TRANSP                  WGL_BORDER_TRANSP
#define SBTL_WGL_BORDER_UNIFORM                 WGL_BORDER_UNIFORM
#define SBTL_WGL_BORDER_RAISED                  WGL_BORDER_RAISED
#define SBTL_WGL_BORDER_DEPRESSED               WGL_BORDER_DEPRESSED
#define SBTL_WGL_BORDER_DROP_SHADOW_LEFT        WGL_BORDER_DROP_SHADOW_LEFT
#define SBTL_WGL_BORDER_DROP_SHADOW_RIGHT       WGL_BORDER_DROP_SHADOW_RIGHT
#define SBTL_WGL_BORDER_TIMG                    WGL_BORDER_TIMG


/* Applicatino Z order constant definitions */
#define SBTL_WGL_APP_NOT_TO_END                 WGL_APP_NOT_TO_END
#define SBTL_WGL_APP_TOPMOST                    WGL_APP_TOPMOST
#define SBTL_WGL_APP_BOTTOMMOST                 WGL_APP_BOTTOMMOST

#define SBTL_WGL_SBTL_COLOR_MODE_CLUT_2_BIT     WGL_SBTL_COLOR_MODE_CLUT_2_BIT
#define SBTL_WGL_SBTL_COLOR_MODE_CLUT_4_BIT     WGL_SBTL_COLOR_MODE_CLUT_4_BIT
#define SBTL_WGL_SBTL_COLOR_MODE_CLUT_8_BIT     WGL_SBTL_COLOR_MODE_CLUT_8_BIT

typedef WGL_SBTL_REGION_T SBTL_WGL_SBTL_REGION_T;
typedef WGL_SBTL_INIT_T SBTL_WGL_SBTL_INIT_T;
typedef wgl_app_callback_fct sbtl_wgl_app_callback_fct;


#define SBTL_WGL_STL_GL_NO_IMG_UI               WGL_STL_GL_NO_IMG_UI
#define SBTL_WGL_STL_GL_NO_BK                   WGL_STL_GL_NO_BK
#define SBTL_WGL_STL_GL_USE_SYS_IMG             WGL_STL_GL_USE_SYS_IMG
#define SBTL_WGL_STL_GL_BDR_FILL_CNT_BK         WGL_STL_GL_BDR_FILL_CNT_BK

#define SBTL_WGL_STL_WGT_SPF_START              WGL_STL_WGT_SPF_START

/*Widget Style*/
#define SBTL_WGL_STL_TEXT_MULTISTR              WGL_STL_TEXT_MULTISTRING
#define SBTL_WGL_STL_TEXT_MULTILINE             WGL_STL_TEXT_MULTILINE
#define SBTL_WGL_STL_TEXT_SMART_CUT             WGL_STL_TEXT_SMART_CUT
#define SBTL_WGL_STL_TEXT_MAX_32                WGL_STL_TEXT_MAX_32
#define SBTL_WGL_STL_TEXT_MAX_128               WGL_STL_TEXT_MAX_128
#define SBTL_WGL_STL_TEXT_MAX_512               WGL_STL_TEXT_MAX_512
#define SBTL_WGL_STL_TEXT_MAX_2048              WGL_STL_TEXT_MAX_2048
#define SBTL_WGL_STL_TEXT_MAX_4096              WGL_STL_TEXT_MAX_4096
#define SBTL_WGL_STL_TEXT_MAX_DIS_5_LINE        WGL_STL_TEXT_MAX_DIS_5_LINE
#define SBTL_WGL_STL_TEXT_MAX_DIS_10_LINE       WGL_STL_TEXT_MAX_DIS_10_LINE
#define SBTL_WGL_STL_TEXT_MAX_DIS_20_LINE       WGL_STL_TEXT_MAX_DIS_20_LINE
#define SBTL_WGL_STL_TEXT_MAX_DIS_40_LINE       WGL_STL_TEXT_MAX_DIS_40_LINE
#define SBTL_WGL_STL_TEXT_MAX_DIS_USER_DEF      WGL_STL_TEXT_MAX_DIS_USER_DEF
#define SBTL_WGL_STL_TEXT_SMART_CUT_OVER_MAX    WGL_STL_TEXT_SMART_CUT_OVER_MAX
#define SBTL_WGL_STL_TEXT_END_AS_NEWLINE        WGL_STL_TEXT_END_AS_NEWLINE
#define SBTL_WGL_STL_TEXT_IGNORE_NEWLINE        WGL_STL_TEXT_IGNORE_NEWLINE

#define SBTL_WGL_NO_AUTO_REPAINT     ((UINT8) 0)                           /**< Indicates the automatic repainting is disabled. */
#define SBTL_WGL_SYNC_AUTO_REPAINT   ((UINT8) 1)                           /**< Indicates the automatic repainting is performed synchronously.*/
#define SBTL_WGL_ASYNC_AUTO_REPAINT  ((UINT8) 2)                           /**<Indicates the automatic repainting is performed asynchronously. */
#define SBTL_WGL_STL_TEXT_MAX_DIS_LINE          WGL_STL_TEXT_MAX_DIS_40_LINE

#else /*__KERNEL__*/
#define SBTL_WGL_PACK(_x)                       ((VOID *)((UINT32)(_x)))    

#define SBTL_WGL_CMD_SBTL_FLIP                  0x10002800
#define SBTL_WGL_CMD_SBTL_CLEAR_ALL             0x10002801
#define SBTL_WGL_CMD_GL_SET_FONT                0x1007fc04
#define SBTL_WGL_CMD_TEXT_SET_ALIGN             0x400f1800
#define SBTL_WGL_CMD_TEXT_SET_TEXT              0x400f1802
#define SBTL_WGL_CMD_GL_SET_COLOR               0x1007fc02
#define SBTL_WGL_CMD_TEXT_SET_STR               0x400f180c
#define SBTL_WGL_CMD_TEXT_SET_PREV_STR          0x400f180d
#define SBTL_WGL_CMD_TEXT_CLEAR_STR             0x400f180e
#define SBTL_WGL_CMD_TEXT_HLT_POS               0x400f180f
#define SBTL_WGL_CMD_TEXT_HLT_STR               0x400f1810
#define SBTL_WGL_CMD_TEXT_PLAY_NEXT_STR         0x400f1811
#define SBTL_WGL_CMD_TEXT_PLAY_PREV_STR         0x400f1812
#define SBTL_WGL_CMD_TEXT_SET_INTERLACED        0x400f1813
#define SBTL_WGL_CMD_TEXT_GET_INTERLACED        0x400f1814
#define SBTL_WGL_CMD_TEXT_SET_LINE_GAP          0x400f1806
#define SBTL_WGL_CMD_TEXT_GET_LINE_GAP          0x400f1807
#define SBTL_WGL_CMD_TEXT_GET_FONT_SIZE         0x400f1818
/*Color Command Target*/
#define SBTL_WGL_CLR_BK                         ((UINT8) 0x01)
#define SBTL_WGL_CLR_TEXT                       ((UINT8) 0x02)
#define SBTL_WGL_CLR_EDGE                       ((UINT8) 0x03)
#define SBTL_WGL_CLR_TEXT_BG                    ((UINT8) 0x04)
#define SBTL_WGL_CLR_WGT_SPF_START              ((UINT8) 0x80)
#define SBTL_WGL_CLR_CUSTOM_START               ((UINT8) 0xF0)


#define SBTL_WGL_WIDGET_TEXT                    0
#define SBTL_WGL_WIDGET_SBTL                    1

#define SBTL_WGL_BORDER_NULL                    0
#define SBTL_WGL_BORDER_TRANSP                  1
#define SBTL_WGL_BORDER_UNIFORM                 2
#define SBTL_WGL_BORDER_RAISED                  3
#define SBTL_WGL_BORDER_DEPRESSED               4
#define SBTL_WGL_BORDER_DROP_SHADOW_LEFT        5
#define SBTL_WGL_BORDER_DROP_SHADOW_RIGHT       6
#define SBTL_WGL_BORDER_TIMG                    7

#define SBTL_WGL_TEXT_POSITION_BY_STR_IDX    ((UINT32) 0x00000001)
#define SBTL_WGL_TEXT_POSITION_BY_INDICATE   ((UINT32) 0x00000002)


#define SBTL_WGL_STL_GL_NO_IMG_UI            ((UINT32) 0x00000001)           /*for widget content*/
#define SBTL_WGL_STL_GL_NO_BK                ((UINT32) 0x00000002)           /*for widget content*/
#define SBTL_WGL_STL_GL_USE_SYS_IMG          ((UINT32) 0x00000004)           /*for widget content*/
#define SBTL_WGL_STL_GL_BDR_FILL_CNT_BK      ((UINT32) 0x00000008)           /*for widget border*/

#define SBTL_WGL_STL_WGT_SPF_START           ((UINT32) 0x00010000)

/*Widget Style*/
#define SBTL_WGL_STL_TEXT_MULTILINE          ((UINT32) SBTL_WGL_STL_WGT_SPF_START)
#define SBTL_WGL_STL_TEXT_SMART_CUT          ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 1)
#define SBTL_WGL_STL_TEXT_MAX_32             ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 2)
#define SBTL_WGL_STL_TEXT_MAX_128            ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 3)
#define SBTL_WGL_STL_TEXT_MAX_512            ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 4)
#define SBTL_WGL_STL_TEXT_MAX_2048           ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 5)
#define SBTL_WGL_STL_TEXT_MAX_4096           ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 6)
#define SBTL_WGL_STL_TEXT_MAX_DIS_5_LINE     ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 7)
#define SBTL_WGL_STL_TEXT_MAX_DIS_10_LINE    ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 8)
#define SBTL_WGL_STL_TEXT_MAX_DIS_20_LINE    ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 9)
#define SBTL_WGL_STL_TEXT_MAX_DIS_40_LINE    ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 10)
#define SBTL_WGL_STL_TEXT_MAX_DIS_USER_DEF   ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 11)
#define SBTL_WGL_STL_TEXT_SMART_CUT_OVER_MAX ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 12)
#define SBTL_WGL_STL_TEXT_END_AS_NEWLINE     ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 13)          /*will be ignored if WGL_STL_TEXT_MULTILINE is applied.*/
#define SBTL_WGL_STL_TEXT_IGNORE_NEWLINE     ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 14)          /*If WGL_STL_TEXT_IGNORE_NEWLINE is used, WGL_STL_TEXT_MULTILINE will be ignored.*/

#define SBTL_WGL_STL_TEXT_MULTISTR           ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 16)

#define SBTL_WGL_STL_TEXT_MAX_DIS_LINE       ((UINT32) SBTL_WGL_STL_WGT_SPF_START << 10)
#define SBTL_WGL_NO_AUTO_REPAINT     ((UINT8) 0)                           /**< Indicates the automatic repainting is disabled. */
#define SBTL_WGL_SYNC_AUTO_REPAINT   ((UINT8) 1)                           /**< Indicates the automatic repainting is performed synchronously.*/
#define SBTL_WGL_ASYNC_AUTO_REPAINT  ((UINT8) 2)                           /**<Indicates the automatic repainting is performed asynchronously. */



/* Applicatino Z order constant definitions */
#define SBTL_WGL_APP_NOT_TO_END      ((UINT8) 0x0)
#define SBTL_WGL_APP_TOPMOST         ((UINT8) 0x1)
#define SBTL_WGL_APP_BOTTOMMOST      ((UINT8) 0x2)


typedef enum _SBTL_WGL_SBTL_COLOR_MODE_T
{
    SBTL_WGL_SBTL_COLOR_MODE_CLUT_2_BIT,
    SBTL_WGL_SBTL_COLOR_MODE_CLUT_4_BIT,
    SBTL_WGL_SBTL_COLOR_MODE_CLUT_8_BIT
} SBTL_WGL_SBTL_COLOR_MODE_T;    


struct _SBTL_WGL_SBTL_REGION_T
{
    UINT32                      ui4_rid;
    BOOL                        b_dirty;
    INT32                       i4_left;
    INT32                       i4_top;
    UINT32                      ui4_width;
    UINT32                      ui4_height;
    SBTL_WGL_SBTL_COLOR_MODE_T  e_clr_mode;
    VOID*                       pv_bitmap;
    GL_COLOR_T*                 at_clut;
    GL_RECT_T                   t_rc_dirty;
};    


/* PIB locking function */
typedef INT32 (*sbtl_wgl_sbtl_lock_pib_fct)(
    HANDLE_T                h_page_inst,
    VOID*                   pv_tag);

/* PIB unlocking function */
typedef INT32 (*sbtl_wgl_sbtl_unlock_pib_fct)(
    HANDLE_T                h_page_inst,
    VOID*                   pv_tag);

typedef struct _SBTL_WGL_SBTL_REGION_T SBTL_WGL_SBTL_REGION_T;
typedef INT32 (*sbtl_wgl_sbtl_get_page_inst_rgn_fct)(
    HANDLE_T                    h_page_inst,
    VOID*                       pv_tag,
    UINT16                      ui2_rgn_idx,
    SBTL_WGL_SBTL_REGION_T*     pt_rgn);

typedef struct _SBTL_WGL_SBTL_INIT_T
{
    UINT32                                  ui4_page_width;
    UINT32                                  ui4_page_height;
    sbtl_wgl_sbtl_lock_pib_fct              pf_lock_pib;
    VOID*                                   pv_tag_lock_pib;
    sbtl_wgl_sbtl_unlock_pib_fct            pf_unlock_pib;
    VOID*                                   pv_tag_unlock_pib;
    sbtl_wgl_sbtl_get_page_inst_rgn_fct     pf_get_page_inst_rgn;
    VOID*                                   pv_tag_get_rgn;
} SBTL_WGL_SBTL_INIT_T;    


typedef INT32 (*sbtl_wgl_app_callback_fct)(
                UINT32              ui4_msg,
                VOID*               pv_param1,
                VOID*               pv_param2);

#endif /*__KERNEL__*/

extern INT32 sbtl_wgl_register(
    sbtl_wgl_app_callback_fct   pf_callback,
    HANDLE_T*                   ph_client);

extern INT32 sbtl_wgl_set_app_order(HANDLE_T h_thread, UINT8 ui1_order);

extern INT32 sbtl_wgl_create_widget(
    HANDLE_T                h_container,
    HANDLE_TYPE_T           e_type_widget,
    UINT32                  ui4_type_border,
    const GL_RECT_T*        pt_rect,
    UINT8                   ui1_alpha,
    VOID*                   pv_param,
    VOID*                   pv_tag,
    HANDLE_T *              ph_widget);

extern INT32 sbtl_wgl_destroy_widget(HANDLE_T h_widget);


extern INT32 sbtl_wgl_do_cmd(
    HANDLE_T            h_widget,
    UINT32              ui4_cmd, 
    VOID*               pv_param1,
    VOID*               pv_param2);

extern INT32 sbtl_wgl_show(
    HANDLE_T            h_widget,
    BOOL                b_show);

extern INT32 sbtl_wgl_get_canvas_size(
    HANDLE_T    h_canvas,
    UINT32*     pui4_width,
    UINT32*     pui4_height);

extern INT32 sbtl_wgl_move(
    HANDLE_T            h_widget,
    const GL_RECT_T*    pt_rect,
    UINT8               ui1_repaint);

extern INT32 sbtl_wgl_float(
    HANDLE_T            h_widget,
    BOOL                b_float,
    UINT8               ui1_repaint);

extern INT32 sbtl_multi_byte_to_unicode(
    UINT16*                     pui2_multi_byte, 
    UINT32                      ui4_len, 
    UINT32*                     pui4_cut_pos,
    E_MUL_BYTE_TYPE             e_multi_type, 
    UINT16*                     pui2_unicode,
    UINT32*                     pui4_count);
#endif /* _SBTL_WIDGET_H_ */


