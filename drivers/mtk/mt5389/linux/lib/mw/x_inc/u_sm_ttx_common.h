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
/*----------------------------------------------------------------------------*/
/*! @file u_sm_ttx_common.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         
 *         
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_STRM_MNGR_TTX_HDLR Teletext Stream Handler
 *
 *  @ingroup groupMW_STRM_MNGR
 *  @brief Provides teletext  function.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_STRM_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_TTX_COMMON_H_
#define _U_SM_TTX_COMMON_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_sys_name.h"
#include "u_handle.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
/* Language Code */
/*------------------------------------------------------------------*/
/*! @enum   SM_TTX_LANG_CODE_T
 *  @brief  TTX language code type
 *  @code
 *  typedef enum _SM_TTX_LANG_CODE_T
 *  {
 *      SM_TTX_LANG_NONE,
 *      SM_TTX_LANG_LATIN,
 *      SM_TTX_LANG_CYRILLIC_1,
 *      SM_TTX_LANG_CYRILLIC_2,
 *      SM_TTX_LANG_CYRILLIC_3,
 *      SM_TTX_LANG_ARABIC,
 *      SM_TTX_LANG_HEBREW,
 *      SM_TTX_LANG_GREEK
 *  } SM_TTX_LANG_CODE_T;
 *  @endcode
 *  @li@c  SM_TTX_LANG_NONE                               -None language code
 *  @li@c  SM_TTX_LANG_LATIN                               -Latin language
 *  @li@c  SM_TTX_LANG_CYRILLIC_1                       -Cyrillic 1 Serbian/Croatian language
 *  @li@c  SM_TTX_LANG_CYRILLIC_2                       -Cyrillic 2 Russian/Bulgarian language
 *  @li@c  SM_TTX_LANG_CYRILLIC_3                       -Cyrillic 3 Ukrainian language
 *  @li@c  SM_TTX_LANG_ARABIC                             -Arabic language
 *  @li@c  SM_TTX_LANG_HEBREW                            -Hebrew lanugage
 *  @li@c  SM_TTX_LANG_GREEK                               -Greek language
 */
/*------------------------------------------------------------------*/
typedef enum _SM_TTX_LANG_CODE_T
{
    SM_TTX_LANG_NONE,
    SM_TTX_LANG_LATIN,
    SM_TTX_LANG_CYRILLIC_1,
    SM_TTX_LANG_CYRILLIC_2,
    SM_TTX_LANG_CYRILLIC_3,
    SM_TTX_LANG_ARABIC,
    SM_TTX_LANG_HEBREW,
    SM_TTX_LANG_GREEK
} SM_TTX_LANG_CODE_T;

/* Teletext character set remap table */
/**<ttx charset mapping table size */
#define SM_TTX_CHARSET_MAP_TBL_SIZE         8    
/*------------------------------------------------------------------*/
/*! @struct  SM_TTX_CHARSET_MAP_TBL_T
 *  @brief  The ttx charset mapping table
 *  @code
 *  typedef struct _SM_TTX_CHARSET_MAP_TBL_T
 *  {
 *      UINT8       aui1_default_g0g2_map[SM_TTX_CHARSET_MAP_TBL_SIZE];
 *      UINT8       aui1_second_g0_map[SM_TTX_CHARSET_MAP_TBL_SIZE];
 *  } SM_TTX_CHARSET_MAP_TBL_T;
 *  @endcode
 *  @li@c  aui1_default_g0g2_map[SM_TTX_CHARSET_MAP_TBL_SIZE]    -For default g0g2 mapping table
 *  @li@c  aui1_second_g0_map[SM_TTX_CHARSET_MAP_TBL_SIZE]       -For second g0 mapping table
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTX_CHARSET_MAP_TBL_T
{
    UINT8       aui1_default_g0g2_map[SM_TTX_CHARSET_MAP_TBL_SIZE];

    UINT8       aui1_second_g0_map[SM_TTX_CHARSET_MAP_TBL_SIZE];
} SM_TTX_CHARSET_MAP_TBL_T;

/* Page type */
/*------------------------------------------------------------------*/
/*! @enum SM_TTX_PAGE_TYPE_T
 *  @brief TTX page types,now it does not used.
 *  @code
 *  typedef enum _SM_TTX_PAGE_TYPE_T
 *  {
 *      SM_TTX_PAGE_TYPE_NORMAL_SINGLE = 0,
 *      SM_TTX_PAGE_TYPE_NORMAL_MULTI,
 *      SM_TTX_PAGE_TYPE_NEWSFLASH,
 *      SM_TTX_PAGE_TYPE_SUBTITLE,
 *      SM_TTX_PAGE_TYPE_SUBTITLE_NEWSFLASH,
 *      SM_TTX_PAGE_TYPE_WARNING,
 *      SM_TTX_PAGE_TYPE_NOW_EPG,
 *      SM_TTX_PAGE_TYPE_NOT_SHOW_SINGLE,
 *      SM_TTX_PAGE_TYPE_NOT_SHOW_MULTI
 *  } SM_TTX_PAGE_TYPE_T;
 *  @endcode
 *  @li@c  SM_TTX_PAGE_TYPE_NORMAL_SINGLE              -Normal single page
 *  @li@c  SM_TTX_PAGE_TYPE_NORMAL_MULTI                -Normal multi page
 *  @li@c  SM_TTX_PAGE_TYPE_NEWSFLASH                     -Flash page
 *  @li@c  SM_TTX_PAGE_TYPE_SUBTITLE                         -Subtitle page
 *  @li@c  SM_TTX_PAGE_TYPE_SUBTITLE_NEWSFLASH      -Subtitle and flash page
 *  @li@c  SM_TTX_PAGE_TYPE_WARNING                         -Warning
 *  @li@c  SM_TTX_PAGE_TYPE_NOW_EPG                         -Epg page
 *  @li@c  SM_TTX_PAGE_TYPE_NOT_SHOW_SINGLE           -Not show single page
 *  @li@c  SM_TTX_PAGE_TYPE_NOT_SHOW_MULTI             -Not show multi page
 */
/*------------------------------------------------------------------*/
typedef enum _SM_TTX_PAGE_TYPE_T
{
    SM_TTX_PAGE_TYPE_NORMAL_SINGLE = 0,
    SM_TTX_PAGE_TYPE_NORMAL_MULTI,
    SM_TTX_PAGE_TYPE_NEWSFLASH,
    SM_TTX_PAGE_TYPE_SUBTITLE,
    SM_TTX_PAGE_TYPE_SUBTITLE_NEWSFLASH,
    SM_TTX_PAGE_TYPE_WARNING,
    SM_TTX_PAGE_TYPE_NOW_EPG,
    SM_TTX_PAGE_TYPE_NOT_SHOW_SINGLE,
    SM_TTX_PAGE_TYPE_NOT_SHOW_MULTI
} SM_TTX_PAGE_TYPE_T;

#define SM_TTX_WGL_MAIN_PNL_COL_NUM            40    /**<The column num of main panel  */
#define SM_TTX_WGL_SIDE_PNL_MAX_COL_NUM        16    /**<The max column num of side panel */
#define SM_TTX_WGL_PNL_TOTAL_COL_NUM    (SM_TTX_WGL_MAIN_PNL_COL_NUM + SM_TTX_WGL_SIDE_PNL_MAX_COL_NUM)    
/**<The total column num of panel(both main and side panel)        */

#define SM_TTX_MAX_MAG_NUM          0x08    /**<The max magzine num*/

#define SM_TTX_NULL_PAGE_NUM        0xFF    /**<Define the null page value        */
#define SM_TTX_ANY_PAGE_SUB_CODE    0x3F7F    /**<Define the subpage code value  */
#define SM_TTX_IS_NULL_PAGE_ADDR(t_page_addr, b_is_null)        \
    if ( ((t_page_addr).ui2_sub_code >= 0x3F7F) &&                \
         ( ((t_page_addr).ui2_page_num & 0x00FF) == 0x00FF) )   \
    {                                                           \
        b_is_null = TRUE;                                       \
    }                                                           \
    else                                                        \
    {                                                           \
        b_is_null = FALSE;                                      \
    }    /**< Check the page is null page addr or not       */

/****************************************************************************** 
    Page Information
******************************************************************************/
/* Page address */
/*------------------------------------------------------------------*/
/*! @struct  SM_TTX_PAGE_ADDR_T
 *  @brief TTX page addr struct,include the page num and subpage code
 *  @code
 *  typedef struct _SM_TTX_PAGE_ADDR_T
 *  {
 *      UINT16      ui2_page_num;
 *      UINT16      ui2_sub_code;
 *  } SM_TTX_PAGE_ADDR_T;
 *  @endcode
 *  @li@c  ui2_page_num                        -TTX Page num value
 *  @li@c  ui2_sub_code                         -TTX subpage code value
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTX_PAGE_ADDR_T
{
    UINT16      ui2_page_num;
    UINT16      ui2_sub_code;
} SM_TTX_PAGE_ADDR_T;

/* Cache policy */
/*------------------------------------------------------------------*/
/*! @struct SM_TTX_CACHE_POLICY_T
 *  @brief  TTX cache hex page or not information
 *  @code
 *  typedef struct _SM_TTX_CACHE_POLICY_T
 *  {
 *      BOOL                            b_cache_hex_page;
 *      UINT16                          ui2_page_cache_cnt;
 *  } SM_TTX_CACHE_POLICY_T;
 *  @endcode
 *  @li@c  b_cache_hex_page                                  -True:cache the hex page. False:Do not cache hex page
 *  @li@c  ui2_page_cache_cnt                                -The numbers of cached hex page
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTX_CACHE_POLICY_T
{
    /*Cache Hex page*/
    BOOL                            b_cache_hex_page;
    /* Cache page count */
    UINT16                          ui2_page_cache_cnt;
} SM_TTX_CACHE_POLICY_T;

/* Basic Info */
/*------------------------------------------------------------------*/
/*! @struct SM_TTX_PAGE_BASIC_INFO_T
 *  @brief TTX basic information of the page
 *  @code
 *  typedef struct _SM_TTX_PAGE_BASIC_INFO_T
 *  {
 *      BOOL                                    b_row24_exist;
 *      BOOL                                    b_existed;
 *      BOOL                                    b_newsflash;
 *      BOOL                                    b_subtitle;
 *      BOOL                                    b_suppress_header;
 *      BOOL                                    b_inhibit_display;
 *      BOOL                                    b_display_row24;
 *      BOOL                                    b_has_side_pnl;
 *      BOOL                                    b_has_flof_info;
 *      BOOL                                    b_has_top_info;
 *      UINT8                                   ui1_left_side_pnl_col_num;
 *      UINT16                                  ui2_subpage_cnt;
 *      SM_TTX_PAGE_ADDR_T                      t_next_subpage_addr;
 *      SM_TTX_PAGE_ADDR_T                      t_prev_subpage_addr;
 *  } SM_TTX_PAGE_BASIC_INFO_T;
 *  @endcode
 *  @li@c  b_row24_exist                                 -The row 24 is exist or not
 *  @li@c  b_existed                                        -The special page is exist or not
 *  @li@c  b_newsflash                                    -The page is the newflash page or not
 *  @li@c  b_subtitle                                        -The page is the subtitle page or not
 *  @li@c  b_suppress_header                           -The page is the suppress header or not
 *  @li@c  b_inhibit_display                              -The page is inhibit or display
 *  @li@c  b_display_row24                              -The page display row 24 or not
 *  @li@c  b_has_side_pnl                                -The page has side panel or not
 *  @li@c  b_has_flof_info                                -The page has flof information or not
 *  @li@c  b_has_top_info                                -The page has top information or not
 *  @li@c  ui1_left_side_pnl_col_num                -The column number of left side panel 
 *  @li@c  ui2_subpage_cnt                              -The num of subpages
 *  @li@c  t_next_subpage_addr                        -Next subpage addr 
 *  @li@c  t_prev_subpage_addr                        -Previous subpage addr
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTX_PAGE_BASIC_INFO_T
{
    BOOL                                    b_row24_exist;
    BOOL                                    b_existed;
    BOOL                                    b_newsflash;
    BOOL                                    b_subtitle;
    BOOL                                    b_suppress_header;
    BOOL                                    b_inhibit_display;
    BOOL                                    b_display_row24;
    BOOL                                    b_has_side_pnl;
    BOOL                                    b_has_flof_info;
    BOOL                                    b_has_top_info;
    UINT8                                   ui1_left_side_pnl_col_num;
    UINT16                                  ui2_subpage_cnt;
    SM_TTX_PAGE_ADDR_T                      t_next_subpage_addr;
    SM_TTX_PAGE_ADDR_T                      t_prev_subpage_addr;
} SM_TTX_PAGE_BASIC_INFO_T;

/* Subpage info */
#define SM_TTX_SUBPAGE_INFO_MAX_PAGE_CNT       5    /**<The max num of subpage information   */
/*------------------------------------------------------------------*/
/*! @struct SM_TTX_SUBPAGE_INFO_T
 *  @brief This struct contains the teletext subpage information.Including the supage count and 
 subpage addr.
 *  @code
 *  typedef struct _SM_TTX_SUBPAGE_INFO_T
 *  {
 *      UINT8                                   ui1_used_subpage_addr_cnt;
 *      SM_TTX_PAGE_ADDR_T                      at_subpage_addr[SM_TTX_SUBPAGE_INFO_MAX_PAGE_CNT]; 
 *  } SM_TTX_SUBPAGE_INFO_T;
 *  @endcode
 *  @li@c  ui1_used_subpage_addr_cnt                         -
 *  @li@c  at_subpage_addr[SM_TTX_SUBPAGE_INFO_MAX_PAGE_CNT] -
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTX_SUBPAGE_INFO_T
{
    UINT8                                   ui1_used_subpage_addr_cnt;
    SM_TTX_PAGE_ADDR_T                      at_subpage_addr[SM_TTX_SUBPAGE_INFO_MAX_PAGE_CNT]; 
} SM_TTX_SUBPAGE_INFO_T;

/****************************************************************************** 
    FLOF Information
******************************************************************************/
/*------------------------------------------------------------------*/
/*! @enum SM_TTX_FLOF_LINK_T
 *  @brief Define the color key link index
 *  @code
 *  typedef enum _SM_TTX_FLOF_LINK_T
 *  {
 *      SM_TTX_FLOF_LINK_RED = 0,
 *      SM_TTX_FLOF_LINK_GREEN = 1,
 *      SM_TTX_FLOF_LINK_YELLOW = 2,
 *      SM_TTX_FLOF_LINK_BLUE = 3,
 *      SM_TTX_FLOF_LINK_INDEX = 4
 *  } SM_TTX_FLOF_LINK_T;
 *  @endcode
 *  @li@c  SM_TTX_FLOF_LINK_RED                              -The red color key index
 *  @li@c  SM_TTX_FLOF_LINK_GREEN                          -The green color key index
 *  @li@c  SM_TTX_FLOF_LINK_YELLOW                        -The yellow color key index
 *  @li@c  SM_TTX_FLOF_LINK_BLUE                            -The blue color key index
 *  @li@c  SM_TTX_FLOF_LINK_INDEX                          -The index link 
 */
/*------------------------------------------------------------------*/
typedef enum _SM_TTX_FLOF_LINK_T
{
    SM_TTX_FLOF_LINK_RED = 0,
    SM_TTX_FLOF_LINK_GREEN = 1,
    SM_TTX_FLOF_LINK_YELLOW = 2,
    SM_TTX_FLOF_LINK_BLUE = 3,
    SM_TTX_FLOF_LINK_INDEX = 4
} SM_TTX_FLOF_LINK_T;

#define SM_TTX_FLOF_MAX_LINK_ADDR_CNT               5    /**< The max num of FLOF link  */
/*------------------------------------------------------------------*/
/*! @struct SM_TTX_FLOF_INFO_T
 *  @brief  The FLOF information 
 *  @code
 *  typedef struct _SM_TTX_FLOF_INFO_T
 *  {
 *      SM_TTX_PAGE_ADDR_T                       at_link_addr[SM_TTX_FLOF_MAX_LINK_ADDR_CNT]; 
 *  } SM_TTX_FLOF_INFO_T;
 *  @endcode
 *  @li@c  at_link_addr[SM_TTX_FLOF_MAX_LINK_ADDR_CNT]       -The FLOF information data
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTX_FLOF_INFO_T
{
    /* five link address, in the order of SM_TTX_FLOF_LINK_T */
    SM_TTX_PAGE_ADDR_T                       at_link_addr[SM_TTX_FLOF_MAX_LINK_ADDR_CNT]; 
} SM_TTX_FLOF_INFO_T;


/****************************************************************************** 
    TOP Information
******************************************************************************/
/*------------------------------------------------------------------*/
/*! @enum  _SM_TTX_TOP_PAGE_LINK_T
 *  @brief  TOP link information index definition
 *  @code
 *  typedef enum _SM_TTX_TOP_PAGE_LINK_T
 *  {
 *      SM_TTX_TOP_LINK_CUR_BLOCK = 0,
 *      SM_TTX_TOP_LINK_CUR_GROUP,
 *      SM_TTX_TOP_LINK_NEXT_BLOCK,
 *      SM_TTX_TOP_LINK_NEXT_GROUP,
 *      SM_TTX_TOP_LINK_PREV_BLOCK,
 *      SM_TTX_TOP_LINK_PREV_GROUP,
 *      SM_TTX_TOP_LINK_NEXT_NORMAL_PAGE,
 *      SM_TTX_TOP_LINK_PREV_NORMAL_PAGE,
 *      SM_TTX_TOP_LINK_NEXT_PAGE,
 *      SM_TTX_TOP_LINK_PREV_PAGE
 *  } _SM_TTX_TOP_PAGE_LINK_T;
 *  @endcode
 *  @li@c  SM_TTX_TOP_LINK_CUR_BLOCK                    -Define the current block index for top
 *  @li@c  SM_TTX_TOP_LINK_CUR_GROUP                    -Define the current group index for top
 *  @li@c  SM_TTX_TOP_LINK_NEXT_BLOCK                   -Define the next block index for top
 *  @li@c  SM_TTX_TOP_LINK_NEXT_GROUP                   -Define the next group index for top
 *  @li@c  SM_TTX_TOP_LINK_PREV_BLOCK                    -Define the previous block index for  top
 *  @li@c  SM_TTX_TOP_LINK_PREV_GROUP                   -Define the previous group index
 *  @li@c  SM_TTX_TOP_LINK_NEXT_NORMAL_PAGE        -Define the  next normal page index for top
 *  @li@c  SM_TTX_TOP_LINK_PREV_NORMAL_PAGE        -Define previous normal page index for top
 *  @li@c  SM_TTX_TOP_LINK_NEXT_PAGE                     -Define the next page index for top
 *  @li@c  SM_TTX_TOP_LINK_PREV_PAGE                      -Define the previous page for top
 */
/*------------------------------------------------------------------*/
typedef enum _SM_TTX_TOP_PAGE_LINK_T
{
    SM_TTX_TOP_LINK_CUR_BLOCK = 0,
    SM_TTX_TOP_LINK_CUR_GROUP,
    SM_TTX_TOP_LINK_NEXT_BLOCK,
    SM_TTX_TOP_LINK_NEXT_GROUP,
    SM_TTX_TOP_LINK_PREV_BLOCK,
    SM_TTX_TOP_LINK_PREV_GROUP,
    SM_TTX_TOP_LINK_NEXT_NORMAL_PAGE,
    SM_TTX_TOP_LINK_PREV_NORMAL_PAGE,
    SM_TTX_TOP_LINK_NEXT_PAGE,
    SM_TTX_TOP_LINK_PREV_PAGE
} _SM_TTX_TOP_PAGE_LINK_T;

#define SM_TTX_PAGE_TOP_MAX_LINK_CNT                10    /**<The max num links for top page   */
#define SM_TTX_TOP_NAME_SIZE                        12    /**<The max size of top size        */

/*------------------------------------------------------------------*/
/*! @enum SM_TTX_TOP_BLOCK_TYPE_T
 *  @brief Define the top block type
 *  @code
 *  typedef enum _SM_TTX_TOP_BLOCK_TYPE_T
 *  {
 *      SM_TTX_TOP_BLOCK_TYPE_BOTH = 0,
 *      SM_TTX_TOP_BLOCK_TYPE_PREVIEW_BLOCK = 1,
 *      SM_TTX_TOP_BLOCK_TYPE_NORMAL_BLOCK = 2
 *  } SM_TTX_TOP_BLOCK_TYPE_T;
 *  @endcode
 *  @li@c  SM_TTX_TOP_BLOCK_TYPE_BOTH                        -
 *  @li@c  SM_TTX_TOP_BLOCK_TYPE_PREVIEW_BLOCK        -
 *  @li@c  SM_TTX_TOP_BLOCK_TYPE_NORMAL_BLOCK         -
 */
/*------------------------------------------------------------------*/
typedef enum _SM_TTX_TOP_BLOCK_TYPE_T
{
    SM_TTX_TOP_BLOCK_TYPE_BOTH = 0,
    SM_TTX_TOP_BLOCK_TYPE_PREVIEW_BLOCK = 1,
    SM_TTX_TOP_BLOCK_TYPE_NORMAL_BLOCK = 2
} SM_TTX_TOP_BLOCK_TYPE_T;

#define SM_TTX_TOP_BLOCK_MAX_LINK_CNT               5    /**<The max link num of TOP block */

#define SM_TTX_TOP_GROUP_MAX_LINK_CNT               5    /**<The max link num of TOP group */

#define SM_TTX_TOP_GROUP_PAGE_LINK_CNT              5    /**<The max link num of page in the same top group */

#define SM_TTX_TOP_AIT_MAX_LINK_CNT                 5    /**<The max num of AIT page link */

/* Subtitle Page Info */
#define SM_TTX_SUBTITLE_PAGE_LINK_CNT                  5    /**< The max num of subititle page */
/*------------------------------------------------------------------*/
/*! @struct SM_TTX_SUBTITLE_PAGE_INFO_T
 *  @brief Storing  the subtitle page information
 *  @code
 *  typedef struct _SM_TTX_SUBTITLE_PAGE_INFO_T
 *  {
 *      UINT8                                   ui1_subtitle_page_num;
 *      SM_TTX_PAGE_ADDR_T                      at_subtitle_page_addr[SM_TTX_SUBTITLE_PAGE_LINK_CNT];
 *  } SM_TTX_SUBTITLE_PAGE_INFO_T;
 *  @endcode
 *  @li@c  ui1_subtitle_page_num                             -The num of subtitle pages
 *  @li@c  at_subtitle_page_addr[SM_TTX_SUBTITLE_PAGE_LINK_CNT]    -The data of subtitle page link
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTX_SUBTITLE_PAGE_INFO_T
{
    UINT8                                   ui1_subtitle_page_num;
    SM_TTX_PAGE_ADDR_T                      at_subtitle_page_addr[SM_TTX_SUBTITLE_PAGE_LINK_CNT];
} SM_TTX_SUBTITLE_PAGE_INFO_T;

 /*------------------------------------------------------------------*/
/*! @struct SM_TTX_EPG_PAGE_INFO_T
 *  @brief This struct contains the teletext epge page information.Including the epg page num in 
 the teletext stream.
 *  @code
 *  typedef struct _SM_TTX_EPG_PAGE_INFO_T
 *  {
 *      BOOL                                    b_has_epg_page;
 *      SM_TTX_PAGE_ADDR_T                      t_epg_page_addr;
 *  } SM_TTX_EPG_PAGE_INFO_T;
 *  @endcode
 *  @li@c  b_has_epg_page                                    -True,it has epg page;False,not have.
 *  @li@c  t_epg_page_addr                                   -The page num of epg page.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTX_EPG_PAGE_INFO_T
{
    BOOL                                    b_has_epg_page;
    SM_TTX_PAGE_ADDR_T                      t_epg_page_addr;
} SM_TTX_EPG_PAGE_INFO_T;

#endif /* _U_SM_TTX_COMMON_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_STRM_MNGR_TTX_HDLR*/
/*----------------------------------------------------------------------------*/


