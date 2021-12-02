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
 * $RCSfile: x_wgl_ttx.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 *---------------------------------------------------------------------------*/

#ifndef _X_WGL_TTX_H_
#define _X_WGL_TTX_H_

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
/* commands */
/**
This command is to set teletext page information.

Input arguments

pv_param1     -Point the teltext page widget handle.

Returns

WGLR_OK      -The command is successful.
*/
 #define WGL_CMD_TTX_SET_PAGE             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 100)

/**
This command is to set header panel information. 

Input arguments

pv_param1     -Contain the header panel handle.

pv_param2     -Null

Returns

WGLR_OK      -The command is successful.
*/
#define WGL_CMD_TTX_SET_HDR_PNL          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 101)

/**
This command is to update the header panel. 

Input arguments

pv_param1     -Poit to the header panel handle

pv_param2     -Null

Returns

WGLR_OK      -The command is successful.
*/
#define WGL_CMD_TTX_UPDATE_HDR_PNL       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 102)

/**
This command is to clear all the ttx layer information 

Input arguments null

Returns

WGLR_OK      -The command is successful.
*/
#define WGL_CMD_TTX_CLEAR_TTX_LAYER      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 103)

/**
This command is to set the ttx display brightness.

Input arguments

pv_param1     -Null.

pv_param2     -Null.

Returns

WGLR_OK      -The command is successful.
*/
#define WGL_CMD_TTX_SET_BRIGHTNESS       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 104)

/*------------------------------------------------------------------*/
/*! @brief Define the teletext notify function.
 *  @param[In]h_widget   -widget handle
 *  @param[In]e_reason   -notify reason
 *  @param[In]pv_tag     -tag data
 *  @param[In]ui4_data   -notify data or data addr
 *  @note    -
 *  @see  -
 *  @return  WGLR_OK               -Successful.
 *               WGLR_INV_ARG      -Notify fail.
 *  @retval     
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_wgl_ttx_nfy_fct)(
    HANDLE_T                h_widget,
    WGL_TTX_NFY_REASON_T    e_reason,   
    VOID*                   pv_tag,
    UINT32                  ui4_data);

/*------------------------------------------------------------------*/
/*! @enum _WGL_TTX_PAGE_TYPE_T
 *  @brief Defines the ttx page type 
 *  @code
 *  typedef enum _WGL_TTX_PAGE_TYPE_T
 *  {
 *   WGL_TTX_PAGE_TYPE_NORMAL = 0,
 *   WGL_TTX_PAGE_TYPE_SBTL,
 *   WGL_TTX_PAGE_TYPE_NEWS_FLASH,
 *   WGL_TTX_PAGE_TYPE_SBTL_NEWS_FLASH
 *  } WGL_TTX_PAGE_TYPE_T;
 *  @endcode
 *  @li@c  WGL_TTX_PAGE_TYPE_NORMAL            -Define the normal teletext page
 *  @li@c  WGL_TTX_PAGE_TYPE_SBTL              -Define the teletext subtitle page
 *  @li@c  WGL_TTX_PAGE_TYPE_NEWS_FLASH        -Define the teletext new flash page
 *  @li@c  WGL_TTX_PAGE_TYPE_SBTL_NEWS_FLASH   -Define the teletext subtitle new
 *  flash  page
*/
typedef enum _WGL_TTX_PAGE_TYPE_T
{
    WGL_TTX_PAGE_TYPE_NORMAL = 0,
    WGL_TTX_PAGE_TYPE_SBTL,
    WGL_TTX_PAGE_TYPE_NEWS_FLASH,
    WGL_TTX_PAGE_TYPE_SBTL_NEWS_FLASH
    
} WGL_TTX_PAGE_TYPE_T;

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

/**The flag is that header panel background color is availible*/
#define WGL_TTX_HDR_PNL_FLAG_BK_CLR_SBST_AVAIL      MAKE_BIT_MASK_32(0)
/**The flag is that header panel enable clut*/
#define WGL_TTX_HDR_PNL_FLAG_CLUT_AVAIL             MAKE_BIT_MASK_32(1)
/**The flag is that header panel is enable DRCS */
#define WGL_TTX_HDR_PNL_FLAG_DRCS_AVAIL             MAKE_BIT_MASK_32(2)
/**The flag is that header panel is enable data*/
#define WGL_TTX_HDR_PNL_FLAG_DATA_AVAIL             MAKE_BIT_MASK_32(3)

/**The flag is that header panel last flag*/
#define WGL_TTX_HDR_PNL_FLAG_LAST_FLAG              WGL_TTX_HDR_PNL_FLAG_DATA_AVAIL

/*------------------------------------------------------------------*/
/*! @struct  WGL_TTX_HDR_PNL_T
 *  @brief  This struction define the header panel information.  
 *  @code
 *  typedef struct _WGL_TTX_HDR_PNL_T
 *  {
 *   UINT32                          ui4_flag;    
 *   WGL_TTX_PAGE_TYPE_T             e_type;             
 *   SM_TTX_PAGE_ADDR_T              t_page_num;         
 *   UINT8                           ui1_start_cell; 
 *   UINT8                           ui1_end_cell;   
 *   BOOL                            b_bk_clr_sbst;
 *   SM_TTX_WGL_CLUTS_T*             pt_cluts;
 *   SM_TTX_WGL_DRCS_INFO_T*         pt_drcs;           
 *   SM_TTX_WGL_HDR_PNL_T*           pt_hdr_pnl;
 *  } WGL_TTX_HDR_PNL_T;
 *  @endcode
 *  @li@c ui4_flag  -Contain the header panel flag. 
 *  @li@c e_type -Define this teletext page type.
 *  @li@c t_page_num -Contain this ttx page number information.
 *  @li@c ui1_start_cell -The start cell of this header panel.
 *  @li@c ui1_end_cell -The end cell of this header panel.
 *  @li@c b_bk_clr_sbst -The subtitle will use background or not flag.
 *  @li@c pt_cluts -Point to the CLUTS information,the max is 32.
 *  @li@c pt_drcs -Point to the DRCS ifnormation.
 *  @li@c pt_hdr_pnl -Point to the header panel information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_HDR_PNL_T
{
    UINT32                          ui4_flag;
    
    WGL_TTX_PAGE_TYPE_T             e_type;             
    
    SM_TTX_PAGE_ADDR_T              t_page_num;         

    UINT8                           ui1_start_cell; /* range: 0~39 */

    UINT8                           ui1_end_cell;   /* range: 0~39 */

    BOOL                            b_bk_clr_sbst;

    SM_TTX_WGL_CLUTS_T*             pt_cluts;

    SM_TTX_WGL_DRCS_INFO_T*         pt_drcs;           
    
    SM_TTX_WGL_HDR_PNL_T*           pt_hdr_pnl;
    
} WGL_TTX_HDR_PNL_T;

/**Define the main panel max row number*/
#define WGL_TTX_MAIN_PNL_MAX_ROW_NUM            ((UINT8) 25)  /* Main Panel contains row 1 ~ 25 */

/*------------------------------------------------------------------*/
/*! @struct  _WGL_TTX_MAIN_PNL_T
 *  @brief  This struction define the header panel information.  
 *  @code
 *  typedef struct  _WGL_TTX_MAIN_PNL_T
 *  {
 *   UINT8                                   ui1_eff_row_num;
 *  SM_TTX_WGL_MAIN_PNL_ROW_DATA_T*         pat_rows;
 *  } ;
 *  @endcode
 *  @li@c ui1_eff_row_num  -The effect row number.
 *  @li@c pat_rows -Point to the row data information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_MAIN_PNL_T
{
    UINT8                                   ui1_eff_row_num;
    SM_TTX_WGL_MAIN_PNL_ROW_DATA_T*         pat_rows; /* Max row number is SM_TTX_WGL_MAIN_PNL_MAX_ROW_NUM */
    
} WGL_TTX_MAIN_PNL_T;

/**The flag define that only partial update on the page*/
#define WGL_TTX_PAGE_FLAG_PARTIAL_UPT           MAKE_BIT_MASK_32(0)
/**The flag define header panel is suppress format on the page*/
#define WGL_TTX_PAGE_FLAG_SUPPRESS_HDR_PNL      MAKE_BIT_MASK_32(1)
/**The flag define that suppress body on the page */
#define WGL_TTX_PAGE_FLAG_SUPPRESS_BODY         MAKE_BIT_MASK_32(2)
/**The flag define that suppress row 24 display*/
#define WGL_TTX_PAGE_FLAG_SUPPRESS_ROW_24       MAKE_BIT_MASK_32(3)

/**The flag define that the screen color is enable*/
#define WGL_TTX_PAGE_FLAG_SCRN_CLR_AVAIL        MAKE_BIT_MASK_32(4)
/**The flag define that the row color is enable*/
#define WGL_TTX_PAGE_FLAG_ROW_CLR_AVAIL         MAKE_BIT_MASK_32(5)
/**The flag define that subtitle background color is enable*/
#define WGL_TTX_PAGE_FLAG_BK_CLR_SBST_AVAIL     MAKE_BIT_MASK_32(6)
/**The flag define that CLUT color is enable*/
#define WGL_TTX_PAGE_FLAG_CLUT_AVAIL            MAKE_BIT_MASK_32(7)
/**The flag define that DRCS is enable*/
#define WGL_TTX_PAGE_FLAG_DRCS_AVAIL            MAKE_BIT_MASK_32(8)
/**The flag define that header panel is enable*/
#define WGL_TTX_PAGE_FLAG_HDR_PNL_AVAIL         MAKE_BIT_MASK_32(9)
/**The flag define that main panel is enable*/
#define WGL_TTX_PAGE_FLAG_MAIN_PNL_AVAIL        MAKE_BIT_MASK_32(10)
/**The flag define that side panel is enable*/
#define WGL_TTX_PAGE_FLAG_SIDE_PNL_AVAIL        MAKE_BIT_MASK_32(11)
/**The flag define that FLOF is enable*/
#define WGL_TTX_PAGE_FLAG_FLOF_AVAIL            MAKE_BIT_MASK_32(12)
/**The flag define that TOP is enable*/
#define WGL_TTX_PAGE_FLAG_TOP_AVAIL             MAKE_BIT_MASK_32(13)
/**The flag define that the FLOF is first priority*/
#define WGL_TTX_PAGE_FLAG_FLOF_FIRST             MAKE_BIT_MASK_32(14)

/**The flag define that the last page flag*/
#define WGL_TTX_PAGE_FLAG_LAST_FLAG             WGL_TTX_PAGE_FLAG_FLOF_FIRST

/*------------------------------------------------------------------*/
/*! @struct  _WGL_TTX_MAIN_PNL_T
 *  @brief  This struction define the header panel information.  
 *  @code
 *  typedef struct _WGL_TTX_PAGE_T
 *  {
 *   UINT32                          ui4_flag;           
 *   WGL_TTX_PAGE_TYPE_T             e_type;             
 *   SM_TTX_PAGE_ADDR_T              t_page_num;         
 *   SM_TTX_WGL_CLR_IDX_T            t_clri_scrn;
 *   SM_TTX_WGL_CLR_IDX_T            t_clri_row;
 *   BOOL                            b_bk_clr_sbst;
 *   SM_TTX_WGL_CLUTS_T*             pt_cluts;
 *   SM_TTX_WGL_DRCS_INFO_T*         pt_drcs;           
 *   SM_TTX_WGL_HDR_PNL_T*           pt_hdr_pnl;
 *   WGL_TTX_MAIN_PNL_T*             pt_main_pnl;
 *   SM_TTX_WGL_SIDE_PNL_T*          pt_side_pnl;
 *  } WGL_TTX_PAGE_T;    
 *  @endcode
 *  @li@c ui4_flag  -The ttx page flag 
 *  @li@c e_type  -the ttx page type. 
 *  @li@c t_page_num  - The page number information.
 *  @li@c t_clri_scrn  - The screen color.
 *  @li@c t_clri_row  - The row color.
 *  @li@c b_bk_clr_sbst  - The subtitle background color is enable or not flag.
 *  @li@c pt_cluts  - Point to the CLUT information.
 *  @li@c pt_drcs  - Point to the DRCS data.
 *  @li@c pt_hdr_pnl  - Point to the header panel.
 *  @li@c pt_main_pnl  - Point to main panel information.
 *  @li@c pt_side_pnl  - Point to the side panel information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_PAGE_T
{
    UINT32                          ui4_flag;           
    
    WGL_TTX_PAGE_TYPE_T             e_type;             
    
    SM_TTX_PAGE_ADDR_T              t_page_num;         
    
    SM_TTX_WGL_CLR_IDX_T            t_clri_scrn;
    
    SM_TTX_WGL_CLR_IDX_T            t_clri_row;

    BOOL                            b_bk_clr_sbst;
    
    SM_TTX_WGL_CLUTS_T*             pt_cluts;

    SM_TTX_WGL_DRCS_INFO_T*         pt_drcs;           
    
    SM_TTX_WGL_HDR_PNL_T*           pt_hdr_pnl;
    
    WGL_TTX_MAIN_PNL_T*             pt_main_pnl;

    SM_TTX_WGL_SIDE_PNL_T*          pt_side_pnl;

} WGL_TTX_PAGE_T;    

/**Define the return value for OK*/
#define WGL_TTXR_OK                             ((INT32) 0)
/**Define the return value for lock error case*/
#define WGL_TTXR_LOCK_ERROR                     ((INT32) -1000)
/**Define the return value for not found widget case*/
#define WGL_TTXR_WGT_NOT_FOUND                  ((INT32) -1001)
/**Define the return value for not found ttx page case*/
#define WGL_TTXR_PAGE_NOT_FOUND                 ((INT32) -1002)
/**Define the return value for not found header row case*/
#define WGL_TTXR_HDR_ROW_NOT_FOUND              ((INT32) -1003)
/**Define the return value for unknow update notify*/
#define WGL_TTXR_UNKNOWN_UPDATE_NFY             ((INT32) -1004)
/**Define the return value not found font case*/
#define WGL_TTXR_FONT_NOT_FOUND             	((INT32) -1005)
/**Define the return value invalid argument*/
#define WGL_TTXR_INV_ARG                    	((INT32) -1006)
/**Define the return value internal err case*/
#define WGL_TTXR_INTERNAL_ERROR                 ((INT32) -1007)


/*------------------------------------------------------------------*/
/*! @brief TTX to get page information funciton.
 *  @param[In]h_page   -The page handle
 *  @param[In]pv_tag   -tag data
 *  @param[In]pt_page     -point to the page information.
 *  @note    -
 *  @see  -
 *  @return  WGL_TTXR_OK               -Successful.
 *               WGL_TTXR_PAGE_NOT_FOUND      -Page not found..
 *  @retval     
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_ttx_get_page_info_fct)(
    HANDLE_T                    h_page,
    VOID*                       pv_tag,
    WGL_TTX_PAGE_T              *pt_page);

/*------------------------------------------------------------------*/
/*! @brief TTX to free page information funciton.
 *  @param[In]h_page   -The page handle
 *  @param[In]pv_tag   -tag data
 *  @note    -
 *  @see  -
 *  @return  WGL_TTXR_OK               -Successful.
 *  @retval     
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_ttx_free_page_fct)(
    HANDLE_T                    h_page,
    VOID*                       pv_tag);

/*------------------------------------------------------------------*/
/*! @brief TTX to get header panel information funciton.
 *  @param[In]h_page   -The header panel handle
 *  @param[In]pv_tag   -tag data
 *  @param[In]pt_page     -point to the header panel information.
 *  @note    -
 *  @see  -
 *  @return  WGL_TTXR_OK               -Successful.
 *               WGL_TTXR_HDR_ROW_NOT_FOUND      -header panel not found..
 *  @retval     
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_ttx_get_hdr_pnl_info_fct)(
    HANDLE_T                    h_hdr_pnl,
    VOID*                       pv_tag,
    WGL_TTX_HDR_PNL_T           *pt_hdr_pnl);

/*------------------------------------------------------------------*/
/*! @brief TTX to free header panel information funciton.
 *  @param[In]h_page   -The header panel handle
 *  @param[In]pv_tag   -tag data
 *  @note    -
 *  @see  -
 *  @return  WGL_TTXR_OK               -Successful.
 *  @retval     
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_ttx_free_hdr_pnl_fct)(
    HANDLE_T                    h_hdr_pnl,
    VOID*                       pv_tag);

/*------------------------------------------------------------------*/
/*! @struct  _WGL_TTX_MAIN_PNL_T
 *  @brief  This struction define the header panel information.  
 *  @code
 *  typedef struct _WGL_TTX_INIT_T
 *  {
 *     wgl_ttx_get_page_info_fct       pf_get_page_info;
 *     VOID*                           pv_tag_get_page_info;
 *
 *     wgl_ttx_free_page_fct           pf_free_page;
 *     VOID*                           pv_tag_free_page;
 * 
 *   wgl_ttx_get_hdr_pnl_info_fct    pf_get_hdr_pnl_info;
 *   VOID*                           pv_tag_get_hdr_pnl_info;
 *
 *   wgl_ttx_free_hdr_pnl_fct        pf_free_hdr_pnl;
 *   VOID*                           pv_tag_free_hdr_pnl;
 *
 *   x_wgl_ttx_nfy_fct               pf_ttx_nfy;
 *   VOID*                           pv_tag_ttx_nfy;
 *
 *   WGL_TTX_CUST_INIT_T             t_cust_init;
 *  } WGL_TTX_INIT_T;    
 *  @endcode
 *  @li@c pf_get_page_info  -The get page information function.
 *  @li@c pv_tag_get_page_info  -The get page information tag
 *  @li@c pf_free_page  - The free page function.
 *  @li@c pv_tag_free_page  - The free page tag.
 *  @li@c pf_get_hdr_pnl_info  - The get header panel information function.
 *  @li@c pv_tag_get_hdr_pnl_info  - The tag for getting header panel information.
 *  @li@c pf_free_hdr_pnl  - The fee header panel function.
 *  @li@c pv_tag_free_hdr_pnl  - The tag for free header panel.
 *  @li@c pf_ttx_nfy  - The ttx notify function.
 *  @li@c pv_tag_ttx_nfy  - The ttx notify function tag.
 */
/*------------------------------------------------------------------*/

typedef struct _WGL_TTX_INIT_T
{
    /* init parameters for handler */
    wgl_ttx_get_page_info_fct       pf_get_page_info;
    VOID*                           pv_tag_get_page_info;

    wgl_ttx_free_page_fct           pf_free_page;
    VOID*                           pv_tag_free_page;
    
    wgl_ttx_get_hdr_pnl_info_fct    pf_get_hdr_pnl_info;
    VOID*                           pv_tag_get_hdr_pnl_info;
    
    wgl_ttx_free_hdr_pnl_fct        pf_free_hdr_pnl;
    VOID*                           pv_tag_free_hdr_pnl;

    x_wgl_ttx_nfy_fct               pf_ttx_nfy;
    VOID*                           pv_tag_ttx_nfy;

    /* init parameters for customization */
    WGL_TTX_CUST_INIT_T             t_cust_init;
    
} WGL_TTX_INIT_T;    

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief Transfer the g0 default charset to UTF16 data.
 *  @param[In]ui1_lang_code   -The language code
 *  @param[In]paui1_chars   -Point to the transfer char.
 *  @param[In]ui2_num_chars   -The number of to be transferred chars.
 *  @param[Out]pw2s_chars   -Point to UTF16 chars that has transferred.
 *  @note    -
 *  @see  -
 *  @return  WGL_TTXR_OK               -Successful.
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_ttx_util_g0_dft_chs_to_w2s (
    UINT8                   ui1_lang_code,
    const UINT8*            paui1_chars,
    UINT16                  ui2_num_chars,
    UTF16_T*                pw2s_chars);



#ifdef __cplusplus
}
#endif


#endif

