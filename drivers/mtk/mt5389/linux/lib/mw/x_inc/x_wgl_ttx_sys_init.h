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
 * $RCSfile: x_wgl_ttx_sys_init.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains definitions for teletext widget system
 *         initialization.
 *---------------------------------------------------------------------------*/
#ifndef _X_WGL_TTX_SYS_INIT_H_
#define _X_WGL_TTX_SYS_INIT_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_sm_ttx_common.h"
#include "u_wgl_ttx.h"


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @struct  _WGL_TTX_DIMAP_BMP_MARGIN_T
 *  @brief  This struction define the header panel information.  
 *  @code
 *  typedef struct  _WGL_TTX_DIMAP_BMP_MARGIN_T
 *  {
 *   INT16                               i2_left;
 *   INT16                               i2_top;
 *   INT16                               i2_right;
 *   INT16                               i2_bottom;
 *  WGL_TTX_DIMAP_BMP_MARGIN_T*         pat_rows;
 *  } ;
 *  @endcode
 *  @li@c i2_left  -The left value
 *  @li@c i2_top  -The i2_top value
 *  @li@c i2_right  -The i2_right value
 *  @li@c i2_bottom  -The i2_bottom value
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_DIMAP_BMP_MARGIN_T
{
    INT16                               i2_left;
    INT16                               i2_top;
    INT16                               i2_right;
    INT16                               i2_bottom;
    
} WGL_TTX_DIMAP_BMP_MARGIN_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_CGID_INFO_T
 *  @brief  This struction define one grid information 
 *  @code
 *  typedef struct _WGL_TTX_CGID_INFO_T
 *  {
 *   UINT8               ui1_hgrid;
 *   UINT8               ui1_vgrid;
 *   BOOL                b_left_right;
 *   BOOL                b_top_btm;
 *  } WGL_TTX_CGID_INFO_T;
 *  @endcode
 *  @li@c ui1_hgrid  -horizontally grid value;
 *  @li@c ui1_vgrid  -vertically grid value
 *  @li@c b_left_right -The grid is on left or right flag
 *  @li@cb_top_btm  -The grid is on top or bottom flag.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_CGID_INFO_T
{
    UINT8               ui1_hgrid;
    
    UINT8               ui1_vgrid;
    
    /* b_left_right==FALSE -> left */
    /* b_left_right==TRUE  -> right */
    BOOL                b_left_right;

    /* b_top_btm==FALSE -> top */
    /* b_top_btm==TRUE  -> bottom */
    BOOL                b_top_btm;
    
} WGL_TTX_CGID_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  WGL_TTX_CGID_TBL_T
 *  @brief  This struction define a table mapping char grid id to
 *  WGL_TTX_CGRID_INFO_T 
 *  @code
 *  typedef struct _WGL_TTX_CGID_TBL_T
 *  {
 *   UINT8                               ui1_num_ch_grid_max;
 *   WGL_TTX_CGID_INFO_T*                aat_tbls_cgid[WGL_TTX_CGID_INFO_NUM];
 *  } WGL_TTX_CGID_TBL_T;
 *  @endcode
 *  @li@c ui1_num_ch_grid_max    -The max number of char grid.
 *  @li@c aat_tbls_cgid   -Point to grid table.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_CGID_TBL_T
{
    UINT8                               ui1_num_ch_grid_max;
    
    /* aat_ginfo[dbl_w_h][char_grid_id]  */
    /* dbl_w_h == 0: single-w & single-h */
    /* dbl_w_h == 1: double-w & single-h */
    /* dbl_w_h == 2: single-w & double-h */
    /* dbl_w_h == 3: double-w & double-h */
    /* ch_grid_id : refer to the document "SW Design - Teletext Engine" */
    /* ch_grid_id : 0 ~ (ui1_cgid_num-1) */
    WGL_TTX_CGID_INFO_T*                aat_tbls_cgid[WGL_TTX_CGID_INFO_NUM];
    
} WGL_TTX_CGID_TBL_T;

/*------------------------------------------------------------------*/
/*! @brief TTX toget default font's private char code convertion funtion type. 
 *  @param[In]w2_char_std  -The UTF16 standard char that to convertion 
 *  @param[Out] pw2_char_prvt  -The convertioned char.
 *  @note    -
 *  @see  -
 *  @return  
 *  @retval     
 */
/*------------------------------------------------------------------*/
typedef BOOL (*wgl_ttx_fnt_candt_get_private_code_fct)(
    const UTF16_T                       w2_char_std,
    UTF16_T*                            pw2_char_prvt); 
    
/** flags : WGL_TTX_DIMAP_FONT_CANDT_T */
#define WGL_TTX_DIMAP_FONT_CANDT_FLAG_RESMPL_CHAR_TO_FIT_GRID        MAKE_BIT_MASK_8(0)


/*------------------------------------------------------------------*/
/*! @struct  WGL_TTX_DIMAP_FONT_CANDT_T 
 *  @brief  This struction define font cand information
 *  @code
 *  typedef struct _WGL_TTX_DIMAP_FONT_CANDT_T
 *  {
 *   UINT8                                    ui1_flag;
 *   CHAR*                                    ps_font_name;
 *   FE_FNT_SIZE                              e_font_size;
 *   wgl_ttx_fnt_candt_get_private_code_fct   pf_get_private_code;
 *   INT16                                    i2_margin_upper;
 *   INT16                                    i2_margin_lower;
 *   UINT8                                    ui1_num_hgrid_char;
 *   UINT8                                    ui1_num_vgrid_char;
 *   WGL_TTX_CGID_TBL_T*                      pt_tbl_cgid_info;
 *  } WGL_TTX_DIMAP_FONT_CANDT_T;
 *  @endcode
 *  @li@c ui1_flag            -The flag for cand.
 *  @li@c ps_font_name        -font name
 *  @li@c e_font_size          -font size
 *  @li@c pf_get_private_code -The private code getting funtion.
 *  @li@c i2_margin_upper      -upper margin. positive: add margin, negative: reduce margin
 *  @li@c i2_margin_lower      -lower marg.positive: add margin, negative: reduce margin
 *  @li@c ui1_num_hgrid_char   -logical hgrid number occupied by a char.
 *  @li@c ui1_num_vgrid_char   -logical vgrid number occupied by a char.
 *  @li@c pt_tbl_cgid_info     -The table mapping char_grid_id to
 *  WGL_TTX_CGID_INFO_T.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_DIMAP_FONT_CANDT_T
{
    /* flags */
    UINT8                                    ui1_flag;
    
    /* font name */
    CHAR*                                    ps_font_name;
    
    /* font size */
    FE_FNT_SIZE                              e_font_size;
    
    /* font's specific char code convertion function */
    wgl_ttx_fnt_candt_get_private_code_fct   pf_get_private_code;
    
    /* upper margin */
    /* positive: add margin, negative: reduce margin */
    INT16                                    i2_margin_upper;
    
    /* lower margin */
    /* positive: add margin, negative: reduce margin */
    INT16                                    i2_margin_lower;
    
    /* logical hgrid number occupied by a char */
    UINT8                                    ui1_num_hgrid_char;

    /* logical vgrid number occupied by a char */
    UINT8                                    ui1_num_vgrid_char;
    
    /* the table mapping char_grid_id to WGL_TTX_CGID_INFO_T */
    WGL_TTX_CGID_TBL_T*                      pt_tbl_cgid_info;

} WGL_TTX_DIMAP_FONT_CANDT_T;


/** ui1_flag : WGL_TTX_DIMAP_CHAR_BMP_T */
#define WGL_TTX_DIMAP_CHAR_BMP_FLAG_RESMPL_TO_FIT_GRID      MAKE_BIT_MASK_8(0)

/*------------------------------------------------------------------*/
/*! @struct   WGL_TTX_DIMAP_CHAR_BMP_T
 *  @brief  This struction define the char bitmap information.
 *  @code
 *  typedef struct _WGL_TTX_DIMAP_CHAR_BMP_T
 *  {
 *   UINT8                               ui1_flag;
 *   UTF16_T                             w2_char;
 *  UINT16                              ui2_w_bmp;
 *   UINT16                              ui2_h_bmp;
 *   UINT16                              ui2_cb_line;
 *   VOID*                               pv_bits;
 *   WGL_TTX_DIMAP_BMP_MARGIN_T          t_margin;
 *   UINT8                               ui1_num_hgrid_cbmp;
 *   UINT8                               ui1_num_vgrid_cbmp;
 *   WGL_TTX_CGID_TBL_T*                 pt_tbl_cgid_info;
 *  } WGL_TTX_DIMAP_CHAR_BMP_T;    
 *  @endcode
 *  @li@c ui1_flag            -The flag for cand.
 *  @li@c w2_char             -The char bitmap value.
 *  @li@c ui2_w_bmp           -The char bitmap width pixels.
 *  @li@c ui2_h_bmp          -The char bitmap height pixels
 *  @li@c ui2_cb_line        -The callback line value. 
 *  @li@c pv_bits            -Point to the start addr of the bitmap.
 *  @li@c t_margin           -The bitmap display margin information.
 *  @li@c ui1_num_hgrid_cbmp  -The bitmap horizotal grid value.
 *  @li@c ui1_num_vgrid_cbmp  -The char bitmap vertical grid value.
 *  @li@c pt_tbl_cgid_info    -The grid table information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_DIMAP_CHAR_BMP_T
{
    /* flags */
    UINT8                               ui1_flag;
    
    /* char code */
    UTF16_T                             w2_char;
    
    /* bmp width */
    UINT16                              ui2_w_bmp;

    /* bmp height */
    UINT16                              ui2_h_bmp;

    /* bytes count per line in pv_bits */
    UINT16                              ui2_cb_line;
    
    /* bmp bits */
    VOID*                               pv_bits;

    /* bmp margin */
    WGL_TTX_DIMAP_BMP_MARGIN_T          t_margin;

    /* logical hgrid number occupied by the char BMP */
    UINT8                               ui1_num_hgrid_cbmp;

    /* logical vgrid number occupied by the char BMP */
    UINT8                               ui1_num_vgrid_cbmp;
    
    /* the table mapping char_grid_id to WGL_TTX_CGID_INFO_T */
    WGL_TTX_CGID_TBL_T*                 pt_tbl_cgid_info;
    
} WGL_TTX_DIMAP_CHAR_BMP_T;    

/*------------------------------------------------------------------*/
/*! @struct  WGL_TTX_DIMAP_ITEM_KEY_T
 *  @brief  This struction define 
 *  @code
 *  typedef struct _WGL_TTX_DIMAP_ITEM_KEY_T
 *  {
 *  GL_SIZE_T                           t_sz_vp_logical;
 *  WGL_TTX_HGRID_NUM_T                 e_num_hgrid;  
 *  UINT8                               ui1_num_vgrid;
 *  } WGL_TTX_DIMAP_ITEM_KEY_T;
 *  @endcode
 *  @li@c t_sz_vp_logical
 *  @li@c e_num_hgrid
 *  @li@c ui1_num_vgrid
 */
/*------------------------------------------------------------------*/

/* WGL_TTX_DIMAP_ITEM_KEY_T */
typedef struct _WGL_TTX_DIMAP_ITEM_KEY_T
{
    /* size of viewport */
    GL_SIZE_T                           t_sz_vp_logical;
    
    /* hgrid number in the viewport */
    WGL_TTX_HGRID_NUM_T                 e_num_hgrid;  
    
    /* vgrid number in the viewport */
    UINT8                               ui1_num_vgrid;
    
} WGL_TTX_DIMAP_ITEM_KEY_T;

/*------------------------------------------------------------------*/
/*! @struct   WGL_TTX_DIMAP_ITEM_INFO_T
 *  @brief  This struction define dim item data information. 
 *  @code
 *  typedef struct _WGL_TTX_DIMAP_ITEM_INFO_T
 *  {
 *      GL_SIZE_T                           t_sz_grid_logical;    
 *      UINT16                              ui2_num_candts;    
 *      WGL_TTX_DIMAP_FONT_CANDT_T*         pat_candts;    
 *      UINT16                              ui2_num_bmp;    
 *      WGL_TTX_DIMAP_CHAR_BMP_T*           pat_bmps;
 *  } WGL_TTX_DIMAP_ITEM_INFO_T;
 *  @endcode
 *  @li@c t_sz_grid_logical  -logical char size
 *  @li@c ui2_num_candts     -Candidates number of font file candidates registration
 *  @li@c pat_candts         -The font file candidate font ifnormation.
 *  @li@c ui2_num_bmp        -Char bmp number of char bitmap registration.
 *  @li@c pat_bmps           -The char bmp information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_DIMAP_ITEM_INFO_T
{
    /* 1) logical char size */
    GL_SIZE_T                           t_sz_grid_logical;
    
    /* 2) font file candidates registration */
    /*   2.1) candidates number */
    UINT16                              ui2_num_candts;
    /*   2.2) candidates */
    WGL_TTX_DIMAP_FONT_CANDT_T*         pat_candts;
    
    /* 3) char bitmap registration */
    /*   3.1) char bmp number */
    UINT16                              ui2_num_bmp;
    /*   3.2) char bmps */
    WGL_TTX_DIMAP_CHAR_BMP_T*           pat_bmps;
    
} WGL_TTX_DIMAP_ITEM_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  WGL_TTX_DIMAP_ITEM_T
 *  @brief  This struction define item information. 
 *  @code
 *  typedef struct _WGL_TTX_DIMAP_ITEM_T
 *  {
 *    WGL_TTX_DIMAP_ITEM_KEY_T            t_key;
 *    WGL_TTX_DIMAP_ITEM_INFO_T           t_info;
 *  } WGL_TTX_DIMAP_ITEM_T;
 *  @endcode
 *  @li@c t_key     -Key field. 
 *  @li@c t_info    -Dim map item iformation.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_DIMAP_ITEM_T
{
    /* key field */
    WGL_TTX_DIMAP_ITEM_KEY_T            t_key;
    
    /* info field */
    WGL_TTX_DIMAP_ITEM_INFO_T           t_info;
    
} WGL_TTX_DIMAP_ITEM_T;

/*------------------------------------------------------------------*/
/*! @struct   WGL_TTX_DIM_INFO_MAP_T
 *  @brief  This struction define itmer map information. 
 *  @code
 *  typedef struct _WGL_TTX_DIM_INFO_MAP_T
 *  {
 *      UINT16                              ui2_num_item;
 *      WGL_TTX_DIMAP_ITEM_T*               pat_items;
 *  } WGL_TTX_DIM_INFO_MAP_T;
 *  @endcode
 *  @li@c ui2_num_item   -Item number. 
 *  @li@c pat_items      -Pat_items must point to a block of static / global memory.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_DIM_INFO_MAP_T
{
    /* item number */
    UINT16                              ui2_num_item;
    
    /* items */
    /* pat_items must point to a block of static / global memory */
    WGL_TTX_DIMAP_ITEM_T*               pat_items;
    
} WGL_TTX_DIM_INFO_MAP_T;

/* ui4_flag : WGL_TTX_SYS_INIT_T */

/** if canvas color mode is palette mode, this flag must be set */
#define WGL_TTX_SYS_INIT_FLAG_PALETTE_MODE                      MAKE_BIT_MASK_32(0)
/** valid only if WGL_TTX_SYS_INIT_FLAG_PALETTE_MODE is ON, suggest to turn on this flag */
#define WGL_TTX_SYS_INIT_FLAG_PALETTE_SUPPORT_PARTIAL_MIX_MODE  MAKE_BIT_MASK_32(1)
/** turn on character resampling (not supported yet) */
#define WGL_TTX_SYS_INIT_FLAG_SUPPORT_RESMPL_CHAR               MAKE_BIT_MASK_32(2)
/** turn on software scaler (not supported yet) */
#define WGL_TTX_SYS_INIT_FLAG_SW_SCALER                         MAKE_BIT_MASK_32(3)
/** adjust brightness */
#define WGL_TTX_SYS_INIT_FLAG_BRIGHTNESS_FLTR                   MAKE_BIT_MASK_32(4)


/*------------------------------------------------------------------*/
/*! @struct   WGL_TTX_SYS_INIT_T
 *  @brief  This struction define teletext widget system initial information. 
 *  @code
 *  typedef struct _WGL_TTX_SYS_INIT_T 
 *  {
 *      UINT32                              ui4_flag;
 *      WGL_TTX_DIM_INFO_MAP_T              t_dimap;
 *      UINT8                               ui1_cnvs_plt_ttx_start;    
 *      UINT8                               ui1_cnvs_plt_ttx_end;    
 *      UINT8                               ui1_fctr_brightness;
 *  } WGL_TTX_SYS_INIT_T;
 *  @endcode
 *  @li@c ui4_flag                  -The widget initial flag.
 *  @li@c t_dimap                   -The dim map information.
 *  @li@c ui1_cnvs_plt_ttx_start    -canvas palette entry range for teletext - start,
 *                                   valid only when WGL_TTX_SYS_INIT_FLAG_PALETTE_MODE is ON 
 *  @li@c ui1_cnvs_plt_ttx_end      -canvas palette entry range for teletext - end
                                     valid only when WGL_TTX_SYS_INIT_FLAG_PALETTE_MODE is ON
                                     if WGL_TTX_SYS_INIT_FLAG_PALETTE_SUPPORT_PARTIAL_MIX_MODE is ON,
                                        (ui1_cnvs_plt_ttx_end - ui1_cnvs_plt_ttx_start + 1) must be 64 
                                     else                            
                                        (ui1_cnvs_plt_ttx_end - ui1_cnvs_plt_ttx_start + 1) must be 32.
 *  @li@c ui1_fctr_brightness        - brightness factor (valid only when WGL_TTX_SYS_INIT_FLAG_BRIGHTNESS_FLTR is ON)
 *                                     value range:50-100.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_SYS_INIT_T 
{
    /* flags */
    UINT32                              ui4_flag;
    
    /* dim info map */
    WGL_TTX_DIM_INFO_MAP_T              t_dimap;
    
    /* canvas palette entry range for teletext - start */
    /* valid only when WGL_TTX_SYS_INIT_FLAG_PALETTE_MODE is ON */
    UINT8                               ui1_cnvs_plt_ttx_start;
    
    /* canvas palette entry range for teletext - end */
    /* valid only when WGL_TTX_SYS_INIT_FLAG_PALETTE_MODE is ON */
    /* if WGL_TTX_SYS_INIT_FLAG_PALETTE_SUPPORT_PARTIAL_MIX_MODE is ON,   */
    /*      (ui1_cnvs_plt_ttx_end - ui1_cnvs_plt_ttx_start + 1) must be 64 */
    /* else                                                                */
    /*      (ui1_cnvs_plt_ttx_end - ui1_cnvs_plt_ttx_start + 1) must be 32 */
    UINT8                               ui1_cnvs_plt_ttx_end;
    
    /* brightness factor (valid only when WGL_TTX_SYS_INIT_FLAG_BRIGHTNESS_FLTR is ON) */
    /* value range: 50~100 */
    UINT8                               ui1_fctr_brightness;
    
} WGL_TTX_SYS_INIT_T;

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
extern INT32 x_wgl_ttx_init(
    WGL_TTX_SYS_INIT_T*                 pt_sys_init);


#ifdef __cplusplus
}
#endif

#endif
 
