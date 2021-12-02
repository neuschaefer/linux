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
/*! @file u_mm_sbtl_engine.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Subtitle Stream handler specific 
 *         events.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_MM_SBTL_HDLR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


#ifndef _U_MM_SBTL_ENGINE_H_
#define _U_MM_SBTL_ENGINE_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "u_gl.h"
#include "u_wgl_common.h"
#include "u_wgl_sbtl.h"
#include "u_wgl_tools.h"
#include "u_wgl_obj_type.h"
//#include "inc/x_mm_common.h"
#include "u_scdb.h"

#else /*__KERNEL__*/

//#include "x_mm_common.h"

#endif /*__KERNEL__*/




/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define MM_SBTL_PREFERED_LANG_LIST_LEN          128    /**<The max list length of subtitle prefered language */
#define MM_MAX_MKV_CONTENT_ENCODING_COUNT       1    /**<The max encoding count of mkv content */
#define MM_MAX_MKV_COMP_SETTINGS_LENGTH         256    /**<The max lenght of mkv setting compress algo*/
#define MM_MAX_MKV_CODEC_PRIVATE_LENGTH         512    /**<The max length of mkv private codec*/
#define MM_SBTL_CACHE_MAX                       ((UINT16)512)  /**<The max length of cache buffer for one subtitle*/
#define MM_SBTL_PARSE_MAX_LENGTH                ((UINT64) (5*1024)) /**<read max length when verify subtltle*/

#define MM_SBTL_INF_MSK_NONE                    ((UINT32)0)
#define MM_SBTL_INF_MSK_FONT_INF                MAKE_BIT_MASK_32(0)
#define MM_SBTL_INF_MSK_ENCODING                MAKE_BIT_MASK_32(1)
#define MM_SBTL_INF_MSK_TM_OFST                 MAKE_BIT_MASK_32(2)
#define MM_SBTL_INF_MSK_OSD_OFST                MAKE_BIT_MASK_32(3)
#define MM_SBTL_INF_MSK_FG_COLOR                MAKE_BIT_MASK_32(4)
#define MM_SBTL_INF_MSK_BG_COLOR                MAKE_BIT_MASK_32(5)
#define MM_SBTL_INF_MSK_PAGE_INF                MAKE_BIT_MASK_32(6)
#define MM_SBTL_INF_MSK_EDGE_COLOR              MAKE_BIT_MASK_32(7)



#define MM_SBTL_CMD_MSK_NONE                    ((UINT32)0)
#define MM_SBTL_CMD_MSK_FONT_INF                MAKE_BIT_MASK_32(0)
#define MM_SBTL_CMD_MSK_ENCODING                MAKE_BIT_MASK_32(1)
#define MM_SBTL_CMD_MSK_TM_OFST                 MAKE_BIT_MASK_32(2)
#define MM_SBTL_CMD_MSK_OSD_OFST                MAKE_BIT_MASK_32(3)
#define MM_SBTL_CMD_MSK_FG_COLOR                MAKE_BIT_MASK_32(4)
#define MM_SBTL_CMD_MSK_BG_COLOR                MAKE_BIT_MASK_32(5)
#define MM_SBTL_CMD_MSK_PAGE_INF                MAKE_BIT_MASK_32(6)
#define MM_SBTL_CMD_MSK_EDGE_COLOR              MAKE_BIT_MASK_32(7)


#define MM_SBTL_CMD_PAGE_INF               MAKE_BIT_MASK_32(0)
#define MM_SBTL_CMD_TIME_OFST              MAKE_BIT_MASK_32(1)

/*Define the display default value*/
#define MM_SBTL_DFT_PAGE_MODE              ((MM_SBTL_PAGE_MODE)MM_SBTL_PAGE_SCROLL_LINE)
#define MM_SBTL_DFT_PAGE_LINE              ((UINT16) 8)
#define MM_SBTL_DFT_PAGE_HLT_FIX_LINE      ((UINT16) 4)
#define MM_SBTL_DFT_PAGE_LINE_GAP          ((INT32) 5)
#define MM_SBTL_DFT_PAGE_INTERVAL_LINE     ((INT32) 0)

/*Define the default font information*/
#ifdef SBTL_DFT_FONT_FOR_CHINA
#define MM_SBTL_DFT_FONT_NAME               SN_FONT_DEFAULT
#define MM_SBTL_DFT_FONT_SIZE               FE_FNT_SIZE_MEDIUM
#define MM_SBTL_DFT_FONT_STYLE              FE_FNT_STYLE_REGULAR
#define MM_SBTL_DFT_FONT_CMAP               FE_CMAP_ENC_UNICODE
#define MM_SBTL_DFT_FONT_WIDTH              ((INT16) 0)
#define MM_SBTL_DFT_FONT_CSTM_SIZE          ((UINT8) 30)
#define MM_SBTL_DFT_ENCODING                MM_SBTL_ENCODE_GB2312
#else
#define MM_SBTL_DFT_FONT_NAME               SN_FONT_DEFAULT
#define MM_SBTL_DFT_FONT_SIZE               FE_FNT_SIZE_MEDIUM
#define MM_SBTL_DFT_FONT_STYLE              FE_FNT_STYLE_REGULAR
#define MM_SBTL_DFT_FONT_CMAP               FE_CMAP_ENC_UNICODE
#define MM_SBTL_DFT_FONT_WIDTH              ((INT16) 0)
#define MM_SBTL_DFT_FONT_CSTM_SIZE          ((UINT8) 30)
#define MM_SBTL_DFT_ENCODING                MM_SBTL_ENCODE_ASCI
#endif

/*Define the default fg color information*/
#define MM_SBTL_DFT_FG_TYPE                 WGL_COLOR_SET_STANDARD   
#define MM_SBTL_DFT_FG_EN_COLOR             { 255, { 255 }, { 255 }, { 255 } }   
#define MM_SBTL_DFT_FG_DIS_COLOR            { 255, { 255 }, { 255 }, { 255 } }   
#define MM_SBTL_DFT_FG_HLT_COLOR            { 255, {86}, {  216}, {   67 } }   

/*Define the default bg color information*/
#define MM_SBTL_DFT_BG_TYPE                 WGL_COLOR_SET_STANDARD   
#define MM_SBTL_DFT_BG_EN_COLOR             { 0, {   0 }, {   0 }, {   0 } }   
#define MM_SBTL_DFT_BG_DIS_COLOR            { 0, {   0 }, {   0 }, {   0 } }   
#define MM_SBTL_DFT_BG_HLT_COLOR            { 255, { 255 }, { 255 }, { 255 } }   

/*Define the default bg color information*/
#define MM_SBTL_DFT_FONT_BG_TYPE            WGL_COLOR_SET_STANDARD   
#define MM_SBTL_DFT_FONT_BG_EN_COLOR        { 0, {   0 }, {   0 }, {   0 } }   
#define MM_SBTL_DFT_FONT_BG_DIS_COLOR       { 0, {   0 }, {   0 }, {   0 } }   
#define MM_SBTL_DFT_FONT_BG_HLT_COLOR       {255,{ 127 }, { 127 }, { 127 } }   

/*Define the default text align*/
#define MM_SBTL_DFT_TEXT_ALIGN              WGL_AS_CENTER_BOTTOM

#define SBTL_RECT_EMPTY(rect) \
            (((rect)->i4_right <= (rect)->i4_left) || ((rect)->i4_bottom <= (rect)->i4_top))    /**< check if this rectangle is empty */

#ifdef __KERNEL__
#define ISO_639_LANG_LEN  4    /**<Max of ISO 639 languages */

/*------------------------------------------------------------------*/
/*! @struct SCDB_SUBTITLE_DIVX_T
 *  @brief This struct contains the subtitle divx information in the scdb.
 *  @code
 *  typedef struct         _SCDB_SUBTITLE_DIVX_T 
 *  { 
 *      UINT32         ui4_stream_id; 
 *      UINT32         ui4_scale; 
 *      UINT32         ui4_rate; 
 *      UINT32         ui4_subtitle_type; 
 *      UINT32         ui4_sample_size;     
 *      INT32          i4_width; 
 *      INT32          i4_height;
 *      CHAR           s_lang[ISO_639_LANG_LEN];
 *      BOOL           b_default; 
 *  } SCDB_SUBTITLE_DIVX_T;
 *  @endcode
 *  @li@c  ui4_stream_id                                     -Stream id
 *  @li@c  ui4_scale                                         -The scale value of subtitle 
 *  @li@c  ui4_rate                                          -The subtitle rate
 *  @li@c  ui4_subtitle_type                                 -The subtitle type
 *  @li@c  ui4_sample_size                                   -The sample size of divx
 *  @li@c  i4_width                                          -The widht of divx video resource
 *  @li@c  i4_height                                         -The height of divx video resource
 *  @li@c  s_lang[ISO_639_LANG_LEN]                          -The lanuage string
 *  @li@c  b_default                                         -True:using default mode
 */
/*------------------------------------------------------------------*/
typedef struct         _SCDB_SUBTITLE_DIVX_T 
{ 
    UINT32         ui4_stream_id; 
    UINT32         ui4_scale; 
    UINT32         ui4_rate; 
    UINT32         ui4_subtitle_type; 
    UINT32         ui4_sample_size;     
    INT32          i4_width; 
    INT32          i4_height;
    CHAR           s_lang[ISO_639_LANG_LEN];
    BOOL           b_default; 
} SCDB_SUBTITLE_DIVX_T;

typedef struct _SCDB_SUBTITLE_PS_T
{
    UINT16           ui2_strm_id;
}SCDB_SUBTITLE_PS_T;


typedef struct _SCDB_SUBTITLE_MP4_T
{
    UINT32         ui4_stream_id;
    INT32          i4_width;
    INT32          i4_height;
                    
}SCDB_SUBTITLE_MP4_T;

/*------------------------------------------------------------------*/
/*! @struct GL_RECT_T
 *  @brief This struct defines rectangle information
 *  @code
 *  typedef struct _GL_RECT_T
 *  {
 *      INT32   i4_left;
 *      INT32   i4_top;
 *      INT32   i4_right;
 *      INT32   i4_bottom;
 *  } GL_RECT_T;
 *  @endcode
 *  @li@c  i4_left                                           -The left of rectangle
 *  @li@c  i4_top                                            -The top of rectangle
 *  @li@c  i4_right                                          -The right of rectangle
 *  @li@c  i4_bottom                                         -The bottom of rectangle
 */
/*------------------------------------------------------------------*/
typedef struct _GL_RECT_T
{
    INT32   i4_left;
    INT32   i4_top;
    INT32   i4_right;
    INT32   i4_bottom;
} GL_RECT_T;

/*------------------------------------------------------------------*/
/*! @struct GL_SIZE_T
 *  @brief This struct defines the size information
 *  @code
 *  typedef struct _GL_SIZE_T
 *  {
 *      UINT32  ui4_width;
 *      UINT32  ui4_height;
 *  } GL_SIZE_T;
 *  @endcode
 *  @li@c  ui4_width                                         -The width information
 *  @li@c  ui4_height                                        -The height information
 */
/*------------------------------------------------------------------*/
typedef struct _GL_SIZE_T
{
    UINT32  ui4_width;
    UINT32  ui4_height;
} GL_SIZE_T;

/*------------------------------------------------------------------*/
/*! @struct GL_POINT_T
 *  @brief This struct defines the point information
 *  @code
 *  typedef struct _GL_POINT_T
 *  {
 *      INT32   i4_x;
 *      INT32   i4_y;
 *  } GL_POINT_T;
 *  @endcode
 *  @li@c  i4_x                                              -x position
 *  @li@c  i4_y                                              -y position
 */
/*------------------------------------------------------------------*/
typedef struct _GL_POINT_T
{
    INT32   i4_x;
    INT32   i4_y;
} GL_POINT_T;

/*------------------------------------------------------------------*/
/*! @struct GL_COLOR_T
 *  @brief This struct defines the color information
 *  @code
 *  typedef struct _GL_COLOR_T
 *  {
 *      UINT8   a;
 *      union {
 *          UINT8   r;
 *          UINT8   y;
 *      } u1;
 *      union {
 *          UINT8   g;
 *          UINT8   u;
 *      } u2;
 *      union {
 *          UINT8   b;
 *          UINT8   v;
 *          UINT8   index;
 *      } u3;
 *  } GL_COLOR_T;
 *  @endcode
 *  @li@c  a                                                 -alpha of color
 *  @li@c  r                                                 -red value of color
 *  @li@c  y                                                 -y value of color
 *  @li@c  g                                                 -g value of color
 *  @li@c  u                                                 -u value of color
 *  @li@c  b                                                 -b value of color
 *  @li@c  v                                                 -v value of color
 *  @li@c  index                                             -index value of color palette
 */
/*------------------------------------------------------------------*/
typedef struct _GL_COLOR_T
{
    UINT8   a;

    union {
        UINT8   r;
        UINT8   y;
    } u1;

    union {
        UINT8   g;
        UINT8   u;
    } u2;
    
    union {
        UINT8   b;
        UINT8   v;
        UINT8   index;
    } u3;
} GL_COLOR_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_INSET_T
 *  @brief  This struct define the widget inset information
 *  @code
 *  typedef struct _WGL_INSET_T
 *  {
 *      INT32                           i4_left;
 *      INT32                           i4_right;
 *      INT32                           i4_top;
 *      INT32                           i4_bottom;    
 *  } WGL_INSET_T;    
 *  @endcode
 *  @li@c  i4_left                                           -The left value of inset widget
 *  @li@c  i4_right                                          -The right value of inset widget
 *  @li@c  i4_top                                            -The top value of inset widget
 *  @li@c  i4_bottom                                         -The bottom value of inset widget
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_INSET_T
{
    INT32                           i4_left;
    INT32                           i4_right;
    INT32                           i4_top;
    INT32                           i4_bottom;    
} WGL_INSET_T;    

/*------------------------------------------------------------------*/
/*! @enum WGL_COLOR_SET_TYPE_T
 *  @brief This struct defines the widget color set type
 *  @code
 *  typedef enum
 *  {
 *      WGL_COLOR_SET_BASIC = 0,
 *      WGL_COLOR_SET_STANDARD,
 *      WGL_COLOR_SET_EXTEND
 *  } WGL_COLOR_SET_TYPE_T;
 *  @endcode
 *  @li@c  WGL_COLOR_SET_BASIC                               -Using basic color set
 *  @li@c  WGL_COLOR_SET_STANDARD                            -Using standard color set
 *  @li@c  WGL_COLOR_SET_EXTEND                              -Using extend color set
 */
/*------------------------------------------------------------------*/
typedef enum
{
    WGL_COLOR_SET_BASIC = 0,
    WGL_COLOR_SET_STANDARD,
    WGL_COLOR_SET_EXTEND
} WGL_COLOR_SET_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_COLOR_BASIC_SET_T
 *  @brief This struct defines basic color set information
 *  @code
 *  typedef struct _WGL_COLOR_BASIC_SET_T
 *  {
 *      GL_COLOR_T              t_enable;
 *      GL_COLOR_T              t_disable;
 *  } WGL_COLOR_BASIC_SET_T;
 *  @endcode
 *  @li@c  t_enable                                          -The enable used color
 *  @li@c  t_disable                                         -The disable used color
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_COLOR_BASIC_SET_T
{
    GL_COLOR_T              t_enable;
    GL_COLOR_T              t_disable;
} WGL_COLOR_BASIC_SET_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_COLOR_STD_SET_T
 *  @brief This struct defines the standard color set information
 *  @code
 *  typedef struct _WGL_COLOR_STD_SET_T
 *  {
 *      GL_COLOR_T              t_enable;
 *      GL_COLOR_T              t_disable;
 *      GL_COLOR_T              t_highlight;
 *  } WGL_COLOR_STD_SET_T;
 *  @endcode
 *  @li@c  t_enable                                          -The enable used color for stardard 
 *  @li@c  t_disable                                         -The disable used color for stardard 
 *  @li@c  t_highlight                                       -The highlight used color for stardard 
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_COLOR_STD_SET_T
{
    GL_COLOR_T              t_enable;
    GL_COLOR_T              t_disable;
    GL_COLOR_T              t_highlight;
} WGL_COLOR_STD_SET_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_COLOR_EXT_SET_T
 *  @brief This struct defines extand color set information
 *  @code
 *  typedef struct _WGL_COLOR_EXT_SET_T
 *  {
 *      GL_COLOR_T              t_enable;
 *      GL_COLOR_T              t_disable;
 *      GL_COLOR_T              t_highlight;
 *      GL_COLOR_T              t_highlight_inactive;
 *      GL_COLOR_T              t_push;
 *      GL_COLOR_T              t_push_unhighlight;
 *  } WGL_COLOR_EXT_SET_T;
 *  @endcode
 *  @li@c  t_enable                                          -The enable used color for extend
 *  @li@c  t_disable                                         -The disable used color for extend
 *  @li@c  t_highlight                                       -The hightlight used color for extend
 *  @li@c  t_highlight_inactive                              -The hightlight inactive used color for extend
 *  @li@c  t_push                                            -The push used color for extend
 *  @li@c  t_push_unhighlight                                -The push unhightlight used color for extend
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_COLOR_EXT_SET_T
{
    GL_COLOR_T              t_enable;
    GL_COLOR_T              t_disable;
    GL_COLOR_T              t_highlight;
    GL_COLOR_T              t_highlight_inactive;
    GL_COLOR_T              t_push;
    GL_COLOR_T              t_push_unhighlight;
} WGL_COLOR_EXT_SET_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_COLOR_INFO_T
 *  @brief This struct define color information 
 *  @code
 *  typedef struct _WGL_COLOR_INFO_T
 *  {
 *      WGL_COLOR_SET_TYPE_T            e_type;
 *      union
 *      {
 *          WGL_COLOR_BASIC_SET_T   t_basic;
 *          WGL_COLOR_STD_SET_T     t_standard;
 *          WGL_COLOR_EXT_SET_T     t_extend;
 *      } u_color_data;
 *  } WGL_COLOR_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            -Color type
 *  @li@c  t_basic                                           -Defines basic color set information
 *  @li@c  t_standard                                        -Defines standard color set information
 *  @li@c  t_extend                                          -Defines extend color set information
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_COLOR_INFO_T
{
    WGL_COLOR_SET_TYPE_T            e_type;
    union
    {
        WGL_COLOR_BASIC_SET_T   t_basic;
        WGL_COLOR_STD_SET_T     t_standard;
        WGL_COLOR_EXT_SET_T     t_extend;
    } u_color_data;
} WGL_COLOR_INFO_T;


/*------------------------------------------------------------------*/
/*! @enum FE_FNT_SIZE
 *  @brief Define the font size type
 *  @code
 *  typedef enum _FE_FNT_SIZE
 *  {
 *      FE_FNT_SIZE_SMALL = 0,
 *      FE_FNT_SIZE_MEDIUM,
 *      FE_FNT_SIZE_LARGE,
 *      FE_FNT_SIZE_CUSTOM,
 *      FE_FNT_SIZE_NUMBER              
 *  } FE_FNT_SIZE;
 *  @endcode
 *  @li@c  FE_FNT_SIZE_SMALL                                 -The small type of font size
 *  @li@c  FE_FNT_SIZE_MEDIUM                                -The medium type of font size
 *  @li@c  FE_FNT_SIZE_LARGE                                 -The large type of font size
 *  @li@c  FE_FNT_SIZE_CUSTOM                                -The custom type of font size
 *  @li@c  FE_FNT_SIZE_NUMBER                                -The number of size types
 */
/*------------------------------------------------------------------*/
typedef enum _FE_FNT_SIZE
{
    FE_FNT_SIZE_SMALL = 0,
    FE_FNT_SIZE_MEDIUM,
    FE_FNT_SIZE_LARGE,
    FE_FNT_SIZE_CUSTOM,

    FE_FNT_SIZE_NUMBER              /* used to count number */
} FE_FNT_SIZE;

/*------------------------------------------------------------------*/
/*! @enum FE_FNT_STYLE
 *  @brief Defines the font styles
 *  @code
 *  typedef enum _FE_FNT_STYLE
 *  {
 *  #ifdef FE_DISABLE_EDGE_EFFECT_SUPPORT
 *      FE_FNT_STYLE_REGULAR        =   MAKE_BIT_MASK_8 (0),
 *      FE_FNT_STYLE_ITALIC         =   MAKE_BIT_MASK_8 (1),
 *      FE_FNT_STYLE_BOLD           =   MAKE_BIT_MASK_8 (2),
 *      FE_FNT_STYLE_UNDERLINE      =   MAKE_BIT_MASK_8 (3),
 *      FE_FNT_STYLE_STRIKEOUT      =   MAKE_BIT_MASK_8 (4),
 *      FE_FNT_STYLE_OUTLINE        =   MAKE_BIT_MASK_8 (5),
 *      FE_FNT_STYLE_BLURRED        =   MAKE_BIT_MASK_8 (6)
 *  #else
 *      FE_FNT_STYLE_REGULAR        =   MAKE_BIT_MASK_16 (0),
 *      FE_FNT_STYLE_ITALIC         =   MAKE_BIT_MASK_16 (1),
 *      FE_FNT_STYLE_BOLD           =   MAKE_BIT_MASK_16 (2),
 *      FE_FNT_STYLE_UNDERLINE      =   MAKE_BIT_MASK_16 (3),
 *      FE_FNT_STYLE_STRIKEOUT      =   MAKE_BIT_MASK_16 (4),
 *      FE_FNT_STYLE_OUTLINE        =   MAKE_BIT_MASK_16 (5),
 *      FE_FNT_STYLE_SHADOW_RIGHT   =   MAKE_BIT_MASK_16 (6),
 *      FE_FNT_STYLE_SHADOW_LEFT    =   MAKE_BIT_MASK_16 (7),
 *      FE_FNT_STYLE_DEPRESSED      =   MAKE_BIT_MASK_16 (8),
 *      FE_FNT_STYLE_RAISED         =   MAKE_BIT_MASK_16 (9),
 *      FE_FNT_STYLE_UNIFORM        =   MAKE_BIT_MASK_16 (10),
 *      FE_FNT_STYLE_BLURRED        =   MAKE_BIT_MASK_16 (11)
 *  #endif
 *  } FE_FNT_STYLE;
 *  @endcode
 *  @li@c  FE_FNT_STYLE_REGULAR                              -Define regular stype
 *  @li@c  FE_FNT_STYLE_ITALIC                               -Define italic stype
 *  @li@c  FE_FNT_STYLE_BOLD                                 -Define bold stype
 *  @li@c  FE_FNT_STYLE_UNDERLINE                            -Define uderline stype
 *  @li@c  FE_FNT_STYLE_STRIKEOUT                            -Define strikout stype
 *  @li@c  FE_FNT_STYLE_OUTLINE                              -Define outline stype
 *  @li@c  FE_FNT_STYLE_BLURRED                              -Define blurred stype
 *  @li@c  FE_FNT_STYLE_REGULAR                              -Define regular stype
 *  @li@c  FE_FNT_STYLE_ITALIC                               -Define italic stype
 *  @li@c  FE_FNT_STYLE_BOLD                                 -Define bold stype
 *  @li@c  FE_FNT_STYLE_UNDERLINE                            -Define udnerline stype
 *  @li@c  FE_FNT_STYLE_STRIKEOUT                            -Define strikeout stype
 *  @li@c  FE_FNT_STYLE_OUTLINE                              -Define outline stype
 *  @li@c  FE_FNT_STYLE_SHADOW_RIGHT                         -Define shadow riht stype
 *  @li@c  FE_FNT_STYLE_SHADOW_LEFT                          -Define shadow left stype
 *  @li@c  FE_FNT_STYLE_DEPRESSED                            -Define depressed  stype
 *  @li@c  FE_FNT_STYLE_RAISED                               -Define raised stype
 *  @li@c  FE_FNT_STYLE_UNIFORM                              -Define uniform stype
 *  @li@c  FE_FNT_STYLE_BLURRED                              -Define blurred stype
 */
/*------------------------------------------------------------------*/
typedef enum _FE_FNT_STYLE
{
#ifdef FE_DISABLE_EDGE_EFFECT_SUPPORT
    FE_FNT_STYLE_REGULAR        =   MAKE_BIT_MASK_8 (0),
    FE_FNT_STYLE_ITALIC         =   MAKE_BIT_MASK_8 (1),
    FE_FNT_STYLE_BOLD           =   MAKE_BIT_MASK_8 (2),
    FE_FNT_STYLE_UNDERLINE      =   MAKE_BIT_MASK_8 (3),
    FE_FNT_STYLE_STRIKEOUT      =   MAKE_BIT_MASK_8 (4),
    FE_FNT_STYLE_OUTLINE        =   MAKE_BIT_MASK_8 (5),
    FE_FNT_STYLE_BLURRED        =   MAKE_BIT_MASK_8 (6)
#else
    FE_FNT_STYLE_REGULAR        =   MAKE_BIT_MASK_16 (0),
    FE_FNT_STYLE_ITALIC         =   MAKE_BIT_MASK_16 (1),
    FE_FNT_STYLE_BOLD           =   MAKE_BIT_MASK_16 (2),
    FE_FNT_STYLE_UNDERLINE      =   MAKE_BIT_MASK_16 (3),
    FE_FNT_STYLE_STRIKEOUT      =   MAKE_BIT_MASK_16 (4),
    FE_FNT_STYLE_OUTLINE        =   MAKE_BIT_MASK_16 (5),
    FE_FNT_STYLE_SHADOW_RIGHT   =   MAKE_BIT_MASK_16 (6),
    FE_FNT_STYLE_SHADOW_LEFT    =   MAKE_BIT_MASK_16 (7),
    FE_FNT_STYLE_DEPRESSED      =   MAKE_BIT_MASK_16 (8),
    FE_FNT_STYLE_RAISED         =   MAKE_BIT_MASK_16 (9),
    FE_FNT_STYLE_UNIFORM        =   MAKE_BIT_MASK_16 (10),
    FE_FNT_STYLE_BLURRED        =   MAKE_BIT_MASK_16 (11)
#endif
} FE_FNT_STYLE;

#define FE_ENC_TAG(value, a, b, c, d)       \
            value = (((UINT32)(a) << 24) |  \
                     ((UINT32)(b) << 16) |  \
                     ((UINT32)(c) <<  8) |  \
                      (UINT32)(d)       )    /**<The font charset encodint function*/

/* Font character mapping */
/*------------------------------------------------------------------*/
/*! @enum FE_CMAP_ENCODING
 *  @brief Define the font character encoding types
 *  @code
 *  typedef enum _FE_CMAP_ENCODING
 *  {
 *      FE_ENC_TAG(FE_CMAP_ENC_NONE,           0,   0,   0,   0  ),
 *      FE_ENC_TAG(FE_CMAP_ENC_MS_SYMBOL,      's', 'y', 'm', 'b'),
 *      FE_ENC_TAG(FE_CMAP_ENC_UNICODE,        'u', 'n', 'i', 'c'),
 *      FE_ENC_TAG(FE_CMAP_ENC_SJIS,           's', 'j', 'i', 's'),
 *      FE_ENC_TAG(FE_CMAP_ENC_GB2312,         'g', 'b', ' ', ' '),
 *      FE_ENC_TAG(FE_CMAP_ENC_BIG5,           'b', 'i', 'g', '5'),
 *      FE_ENC_TAG(FE_CMAP_ENC_WANSUNG,        'w', 'a', 'n', 's'),
 *      FE_ENC_TAG(FE_CMAP_ENC_JOHAB,          'j', 'o', 'h', 'a'),
 *      FE_ENC_TAG(FE_CMAP_ENC_ADOBE_STANDARD, 'A', 'D', 'O', 'B'),
 *      FE_ENC_TAG(FE_CMAP_ENC_ADOBE_EXPERT,   'A', 'D', 'B', 'E'),
 *      FE_ENC_TAG(FE_CMAP_ENC_ADOBE_CUSTOM,   'A', 'D', 'B', 'C'),
 *      FE_ENC_TAG(FE_CMAP_ENC_ADOBE_LATIN_1,  'l', 'a', 't', '1'),
 *      FE_ENC_TAG(FE_CMAP_ENC_OLD_LATIN_2,    'l', 'a', 't', '2'),
 *      FE_ENC_TAG(FE_CMAP_ENC_APPLE_ROMAN,    'a', 'r', 'm', 'n')
 *  } FE_CMAP_ENCODING;
 *  @endcode
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_NONE                       -None 
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_MS_SYMBOL                  -Using symb encoding type
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_UNICODE                    -Using unicode encoding type
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_SJIS                       -Using sjis encoding type
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_GB2312                     - Using GB2312 encode type
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_BIG5                       -Using BIG5 encoding type
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_WANSUNG                    -Using wansung encoding type
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_JOHAB                      -Using joha encoding type 
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_ADOBE_STANDARD             -adobe standard encoding
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_ADOBE_EXPERT               -adobe expert encoding
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_ADOBE_CUSTOM               -adobe custom encoding
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_ADOBE_LATIN_1              -adobe latin 1 encoding
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_OLD_LATIN_2                -old latin 2 encoding
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_APPLE_ROMAN                -apple roman encoding
 */
/*------------------------------------------------------------------*/
typedef enum _FE_CMAP_ENCODING
{
    FE_ENC_TAG(FE_CMAP_ENC_NONE,           0,   0,   0,   0  ),

    FE_ENC_TAG(FE_CMAP_ENC_MS_SYMBOL,      's', 'y', 'm', 'b'),
    FE_ENC_TAG(FE_CMAP_ENC_UNICODE,        'u', 'n', 'i', 'c'),

    FE_ENC_TAG(FE_CMAP_ENC_SJIS,           's', 'j', 'i', 's'),
    FE_ENC_TAG(FE_CMAP_ENC_GB2312,         'g', 'b', ' ', ' '),
    FE_ENC_TAG(FE_CMAP_ENC_BIG5,           'b', 'i', 'g', '5'),
    FE_ENC_TAG(FE_CMAP_ENC_WANSUNG,        'w', 'a', 'n', 's'),
    FE_ENC_TAG(FE_CMAP_ENC_JOHAB,          'j', 'o', 'h', 'a'),

    FE_ENC_TAG(FE_CMAP_ENC_ADOBE_STANDARD, 'A', 'D', 'O', 'B'),
    FE_ENC_TAG(FE_CMAP_ENC_ADOBE_EXPERT,   'A', 'D', 'B', 'E'),
    FE_ENC_TAG(FE_CMAP_ENC_ADOBE_CUSTOM,   'A', 'D', 'B', 'C'),
    FE_ENC_TAG(FE_CMAP_ENC_ADOBE_LATIN_1,  'l', 'a', 't', '1'),
    FE_ENC_TAG(FE_CMAP_ENC_OLD_LATIN_2,    'l', 'a', 't', '2'),
    FE_ENC_TAG(FE_CMAP_ENC_APPLE_ROMAN,    'a', 'r', 'm', 'n')
} FE_CMAP_ENCODING;

#define WGL_MAX_FONT_NAME               32    /**<        */

/*------------------------------------------------------------------*/
/*! @struct WGL_FONT_INFO_T
 *  @brief This struct defines widget font information
 *  @code
 *  typedef struct _WGL_FONT_INFO_T
 *  {
 *      FE_FNT_SIZE                     e_font_size;
 *      FE_FNT_STYLE                    e_font_style;
 *      FE_CMAP_ENCODING                e_font_cmap;
 *      CHAR                            a_c_font_name[WGL_MAX_FONT_NAME];
 *      INT16                           i2_width;                           
 *  } WGL_FONT_INFO_T;
 *  @endcode
 *  @li@c  e_font_size                                       -Define the font size widget
 *  @li@c  e_font_style                                      -Define the font style for widget
 *  @li@c  e_font_cmap                                       -Define the font encoding type
 *  @li@c  a_c_font_name[WGL_MAX_FONT_NAME]                  -Define the font lib name
 *  @li@c  i2_width                                          -Font width. non-zero: fixed width. zero: width from font file
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_FONT_INFO_T
{
    FE_FNT_SIZE                     e_font_size;
    FE_FNT_STYLE                    e_font_style;
    FE_CMAP_ENCODING                e_font_cmap;
    CHAR                            a_c_font_name[WGL_MAX_FONT_NAME];
    INT16                           i2_width;                           /*font width. non-zero: fixed width. zero: width from font file.*/
    UINT8                           ui1_custom_size;    
} WGL_FONT_INFO_T;
#endif /*__KERNEL__*/


/* Sutitle service internal used return value */
#define MMSBTLR_EOT                             ((INT32) 2)    /**<Mutilmedia subtitle is in theend*/
#define MMSBTLR_CONTENT_UPDATE                  ((INT32) 1)    /**<Update content of Mutilmedia subtitle */
#define MMSBTLR_OK                              ((INT32) 0)    /**<Ok*/
#define MMSBTLR_INV_ARG                         ((INT32) -1)    /**<Invalide argument*/
#define MMSBTLR_ALREADY_INIT                    ((INT32) -2)    /**<Mutilmedia subtitle is already initial*/
#define MMSBTLR_NOT_INIT                        ((INT32) -3)    /**<Mutilmedia subtitle has not initial*/
#define MMSBTLR_NOT_FOUND                       ((INT32) -4)    /**<Not found mutilmedia subtitle         */
#define MMSBTLR_INSUFFICIENT_MEMORY             ((INT32) -5)    /**<Insufficient memory*/
#define MMSBTLR_WRONG_SEGMENT_DATA              ((INT32) -6)    /**<Wrong segment data*/
#define MMSBTLR_HOOK_FUNC_NOT_FOUND             ((INT32) -7)    /**<Hook function not found*/
#define MMSBTLR_INTERNAL_STATE_ERROR            ((INT32) -8)    /**<Internal state error*/
#define MMSBTLR_WGL_FAIL                        ((INT32) -9)    /**<Subtitle widget fail */
#define MMSBTLR_PAGE_INST_OUT_OF_HANDLE         ((INT32) -10)    /**<Subtitle page instance handle invalid */
#define MMSBTLR_DMX_FAIL                        ((INT32) -11)    /**< Subtitle demux fail*/
#define MMSBTLR_PTS_FAIL                        ((INT32) -12)    /**<Subtitle PTS fail*/
#define MMSBTLR_SCDB_FAIL                       ((INT32) -13)    /**<Mutilmedia subtitle SCDB fail*/
#define MMSBTLR_PM_FAIL                         ((INT32) -14)    /**<Mutilmedia subtitle PM fail        */
#define MMSBTLR_CORE                            ((INT32) -15)    /**<Important core of mutilmedia subtitle engine fail */
#define MMSBTLR_EXIST                           ((INT32) -16)    /**<Mutilmedia subtitle exist*/
#define MMSBTLR_OUT_OF_HANDLE                   ((INT32) -17)    /**<Mutilmedia subtitle handle fail*/
#define MMSBTLR_DEVICE_ERROR                    ((INT32) -18)    /**<Mutilmedia subtitle device create error*/
#define MMSBTLR_CAN_NOT_OP_IN_THIS_STATE        ((INT32) -19)    /**<Mutilmedia subtitle state error*/
#define MMSBTLR_ALREADY_PLAYED                  ((INT32) -20)    /**<Mutilmedia subtitle is in played stated*/
#define MMSBTLR_ALREADY_STOPPED                 ((INT32) -21)    /**<Mutilmedia subtitle is in stopped state*/
#define MMSBTLR_NOT_SUPPORT                     ((INT32) -22)    /**<Mutilmedia subtitle type is not support*/
#define MMSBTLR_PENDING                         ((INT32) -23)    /**<Mutilmedia subtitle is pending*/
#define MMSBTLR_NOT_ALLOW                       ((INT32) -24)    /**<Mutilmedia subtitle is not allow*/
#define MMSBTLR_INTERNAL_ERROR                    ((INT32) -25)
#define MMSBTLR_INSUFFICIENT_SIZE                ((INT32) -26)

#define MMSBTLR_CLI_ERROR                       ((INT32) -27)    /**<Mutilmedia subtitle cli error*/




typedef UINT32 SBTL_COND_T;    /**<Mutilmedia subtitle condition parameter type*/

#define SBTL_COND_IGNORE                ((SBTL_COND_T) 0)    /**<Mutilmedia subtitle ignore condition*/
#define SBTL_COND_OPENING               ((SBTL_COND_T) 1)    /**<Mutilmedia subtitle opening condition*/
#define SBTL_COND_OPENED                ((SBTL_COND_T) 2)    /**<Mutilmedia subtitle opened condition */
#define SBTL_COND_STARTING              ((SBTL_COND_T) 3)    /**<Mutilmedia subtitle  starting condition*/
#define SBTL_COND_STARTED               ((SBTL_COND_T) 4)    /**<Mutilmedia subtitle started condition*/
#define SBTL_COND_STOPPING              ((SBTL_COND_T) 5)    /**<Mutilmedia subtitle stopping condition*/
#define SBTL_COND_CLOSING               ((SBTL_COND_T) 6)    /**<Mutilmedia subtitle closing condition*/
#define SBTL_COND_CLOSED                ((SBTL_COND_T) 7)    /**<Mutilmedia subtitle closed condition*/
#define SBTL_COND_ERROR                 ((SBTL_COND_T) 8)    /**<Mutilmedia subtitle closed condition*/
#define SBTL_COND_LAST_ENTRY            ((SBTL_COND_T) 128)    /**<The last entry*/


typedef UINT32 MM_EVN_TYPE_T;    /**<Mutilmedia subtitle event parameter type*/

#define EVN_MM_SBTL_IGNORE                          ((MM_EVN_TYPE_T)0)    /**<Mutilmedia subtitle ingore event */
#define EVN_MM_SBTL_APP_OPEN_REQ                    ((MM_EVN_TYPE_T)1)    /**<Mutilmedia subtitle app open require event*/
#define EVN_MM_SBTL_APP_PLAY_REQ                    ((MM_EVN_TYPE_T)2)    /**<Mutilmedia subtitle app play require event*/
#define EVN_MM_SBTL_APP_STOP_REQ                    ((MM_EVN_TYPE_T)3)    /**<Mutilmedia subtitle stop require event*/
#define EVN_MM_SBTL_APP_CLOSE_REQ                   ((MM_EVN_TYPE_T)4)    /**<Mutilmedia subtitle close requre event*/
#define EVN_MM_SBTL_DRV_OPENED                      ((MM_EVN_TYPE_T)5)    /**<Mutilmedia subtitle driver opened event*/
#define EVN_MM_SBTL_DRV_PLAYED                      ((MM_EVN_TYPE_T)6)    /**<Mutilmedia subtitle driver played event*/
#define EVN_MM_SBTL_DRV_STOPPED                     ((MM_EVN_TYPE_T)7)    /**<Mutilmedia subtitle driver stopped event*/
#define EVN_MM_SBTL_DRV_CLOSED                      ((MM_EVN_TYPE_T)8)    /**<Mutilmedia subtitle closed event*/
#define EVN_MM_SBTL_DRV_ERR                         ((MM_EVN_TYPE_T)9)    /**<Mutilmedia subtitle driver error event*/
#define EVN_MM_SBTL_SCDB_RECORD_DEL                 ((MM_EVN_TYPE_T)10)    /**<Mutilmedia subtitle scdb record delete event*/
#define EVN_MM_SBTL_SCDB_RECORD_MOD                 ((MM_EVN_TYPE_T)11)    /**<Mutilmedia subtitle scdb record mode event*/
#define EVN_MM_SBTL_INTERNAL_ERR                    ((MM_EVN_TYPE_T)12)    /**<Mutilmedia subtitle internal error */

#define EVN_MM_SBTL_SET_VIDEO_RECT                  ((MM_EVN_TYPE_T)13)    /**<Mutilmedia subtitle set video rectangle event*/
#define EVN_MM_SBTL_SET_VIDEO_OVERSCAN              ((MM_EVN_TYPE_T)14)    /**<Mutilmedia subtitle set video overscan event*/
#define EVN_MM_SBTL_SET_OSD_RECT                    ((MM_EVN_TYPE_T)15)    /**<Mutilmedia subtitle set osd rectangle event*/
#define EVN_MM_SBTL_SET_OSD_INSECT                  ((MM_EVN_TYPE_T)16)    /**<Mutilmedia subtitle set osd insect event*/
#define EVN_MM_SBTL_SET_OSD_OFFSET                  ((MM_EVN_TYPE_T)17)    /**<Mutilmedia subtitle set osd offset event*/
#define EVN_MM_SBTL_SET_OSD_FG_COLOR                ((MM_EVN_TYPE_T)18)    /**<Mutilmedia subtitle set osd frontground color event*/
#define EVN_MM_SBTL_SET_OSD_BG_COLOR                ((MM_EVN_TYPE_T)19)    /**<Mutilmedia subtitle set osd background color event*/
#define EVN_MM_SBTL_SET_OSD_FONT_INFO               ((MM_EVN_TYPE_T)20)    /**<Mutilmedia subtitle set osd font information event*/
#define EVN_MM_SBTL_SET_OSD_TEXT_ALIGNMENT          ((MM_EVN_TYPE_T)21)    /**<Mutilmedia subtitle set text alignment event*/
#define EVN_MM_SBTL_SET_DIVX_MENU_ITEM              ((MM_EVN_TYPE_T)22)    /**<Mutilmedia subtitle set divx menu item event*/
#define EVN_MM_SBTL_SET_SPEED                       ((MM_EVN_TYPE_T)23)    /**<Mutilmedia subtitle set speed event*/
#define EVN_MM_SBTL_SET_STREAM_INDEX                ((MM_EVN_TYPE_T)24)    /**<Mutilmedia subtitle set stream index event*/
#define EVN_MM_SBTL_SET_LANGUAGE                    ((MM_EVN_TYPE_T)25)    /**<Mutilmedia subtitle set language event*/

#define EVN_MM_SBTL_GET_STREAM_INFO                 ((MM_EVN_TYPE_T)26)    /**<Mutilmedia subtitle get stream information*/
#define EVN_MM_SBTL_SET_ADJUST_TIME           ((MM_EVN_TYPE_T)27)    /**<Mutilmedia subtitle adjust time.*/
#define EVN_MM_SBTL_SET_ENCODING                    ((MM_EVN_TYPE_T)28)    /**<Mutilmedia subtitle set encoding type.*/
#define EVN_MM_SBTL_SET_HLT_FG_COLOR                ((MM_EVN_TYPE_T)29)    /**<Mutilmedia subtitle set hightlight fontground color.*/
#define EVN_MM_SBTL_SET_HLT_BG_COLOR                ((MM_EVN_TYPE_T)30)    /**<Mutilmedia subtitle set hightlight backgroud color.*/
#define EVN_MM_SBTL_SET_PAGE_INF                    ((MM_EVN_TYPE_T)31)    /**<Mutilmedia subtitle set display information.*/
#define EVN_MM_SBTL_SET_LINE_GAP                    ((MM_EVN_TYPE_T)32)    /**<Mutilmedia subtitle set display page line gap value.*/
#define EVN_MM_SBTL_SET_SHOW_HIDE                   ((MM_EVN_TYPE_T)33)    /**<Mutilmedia subtitle set display page line gap value.*/
#define EVN_MM_SBTL_SET_FONT_BG_COLOR               ((MM_EVN_TYPE_T)34)    /**<Mutilmedia subtitle set font bg color.*/
#define EVN_MM_SBTL_SET_OSD_FONT_EDGE_COLOR         ((MM_EVN_TYPE_T)35)    /**<Mutilmedia subtitle set osd font edge color event*/

#define EVN_MM_SBTL_GET_VIDEO_RECT                  ((MM_EVN_TYPE_T)65)    /**<Mutilmedia subtitle get video rectangle event*/
#define EVN_MM_SBTL_GET_VIDEO_OVERSCAN              ((MM_EVN_TYPE_T)66)    /**<Mutilmedia subtitle get video overscan event*/
#define EVN_MM_SBTL_GET_OSD_RECT                    ((MM_EVN_TYPE_T)67)    /**<Mutilmedia subtitle get osd rectangle event*/
#define EVN_MM_SBTL_GET_OSD_INSECT                  ((MM_EVN_TYPE_T)68)    /**<Mutilmedia subtitle get osd insect event*/
#define EVN_MM_SBTL_GET_OSD_OFFSET                  ((MM_EVN_TYPE_T)69)    /**<Mutilmedia subtitle get osd offset event*/
#define EVN_MM_SBTL_GET_OSD_FG_COLOR                ((MM_EVN_TYPE_T)70)    /**<Mutilmedia subtitle get osd frontground color event*/
#define EVN_MM_SBTL_GET_OSD_BG_COLOR                ((MM_EVN_TYPE_T)71)    /**<Mutilmedia subtitle get osd background color event*/
#define EVN_MM_SBTL_GET_OSD_FONT_INFO               ((MM_EVN_TYPE_T)72)    /**<Mutilmedia subtitle get osd font information event*/
#define EVN_MM_SBTL_GET_OSD_TEXT_ALIGNMENT          ((MM_EVN_TYPE_T)73)    /**<Mutilmedia subtitle get text alignment event*/
#define EVN_MM_SBTL_GET_DIVX_MENU_ITEM              ((MM_EVN_TYPE_T)74)    /**<Mutilmedia subtitle get divx menu item event*/
#define EVN_MM_SBTL_GET_SPEED                       ((MM_EVN_TYPE_T)75)    /**<Mutilmedia subtitle get speed event*/
#define EVN_MM_SBTL_GET_STREAM_INDEX                ((MM_EVN_TYPE_T)76)    /**<Mutilmedia subtitle get stream index event*/
#define EVN_MM_SBTL_GET_LANGUAGE                    ((MM_EVN_TYPE_T)77)    /**<Mutilmedia subtitle get language event*/

#define EVN_MM_SBTL_GET_ADJUST_TIME                 ((MM_EVN_TYPE_T)78)    /**<Mutilmedia subtitle get adjust time.*/
#define EVN_MM_SBTL_GET_ENCODING                    ((MM_EVN_TYPE_T)79)    /**<Mutilmedia subtitle get encoding type.*/
#define EVN_MM_SBTL_GET_HLT_FG_COLOR                ((MM_EVN_TYPE_T)80)    /**<Mutilmedia subtitle get hightlight fontground color.*/
#define EVN_MM_SBTL_GET_HLT_BG_COLOR                ((MM_EVN_TYPE_T)81)    /**<Mutilmedia subtitle get hightlight backgroud color.*/
#define EVN_MM_SBTL_GET_PAGE_INF                    ((MM_EVN_TYPE_T)82)    /**<Mutilmedia subtitle get display information.*/
#define EVN_MM_SBTL_GET_LINE_GAP                    ((MM_EVN_TYPE_T)83)    /**<Mutilmedia subtitle get display page line gap value.*/
#define EVN_MM_SBTL_GET_SHOW_HIDE                   ((MM_EVN_TYPE_T)84)    /**<Mutilmedia subtitle get display page line gap value.*/
#define EVN_MM_SBTL_GET_SBTL_TYPE                   ((MM_EVN_TYPE_T)85)    /**<Mutilmedia subtitle get subtitle type.*/
#define EVN_MM_SBTL_GET_OSD_FONT_EDGE_COLOR         ((MM_EVN_TYPE_T)86)    /**<Mutilmedia subtitle get osd font edge color event*/

/*MM subtitle language type*/
/*------------------------------------------------------------------*/
/*! @enum MM_SBTL_LANG_T
 *  @brief Define MM subtitle language type
 *  @code
 *  typedef enum
 *  {
 *      MM_SBTL_LANG_ASCI,
 *      MM_SBTL_LANG_UNICODE,
 *      MM_SBTL_LANG_END
 *  } MM_SBTL_LANG_T;
 *  @endcode
 *  @li@c  MM_SBTL_LANG_ASCI                                 -Define ASCI language type for subtitle
 *  @li@c  MM_SBTL_LANG_UNICODE                              -Define unicode language type for subtitle
 *  @li@c  MM_SBTL_LANG_END                                  -The end of language type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SBTL_LANG_ASCI,
    MM_SBTL_LANG_UNICODE,
    MM_SBTL_LANG_BIG5,
    MM_SBTL_LANG_BG2312,
    MM_SBTL_LANG_END
} MM_SBTL_LANG_T;


typedef enum
{
    MM_SBTL_ENCODE_AUTO,
    MM_SBTL_ENCODE_ASCI,
    MM_SBTL_ENCODE_UTF8,
    MM_SBTL_ENCODE_UTF16,
    MM_SBTL_ENCODE_BIG5,
    MM_SBTL_ENCODE_GB2312,
    MM_SBTL_ENCODE_ISO8859_2,
    MM_SBTL_ENCODE_ISO8859_5,
    MM_SBTL_ENCODE_ISO8859_7,
    MM_SBTL_ENCODE_ISO8859_8,
    MM_SBTL_ENCODE_ISO8859_9,
    MM_SBTL_ENCODE_ISO8859_16,
    MM_SBTL_ENCODE_END
} MM_SBTL_ENCODING_T;


/*------------------------------------------------------------------*/
/*! @enum MM_SBTL_TYPE_T
 *  @brief Define the subtitle type 
 *  @code
 *  typedef enum
 *  {
 *      MM_SBTL_TYPE_DIVX,
 *      MM_SBTL_TYPE_VOBSUB,
 *      MM_SBTL_TYPE_MKV_VOBSUB,
 *      MM_SBTL_TYPE_MKV_SRT,
 *      MM_SBTL_TYPE_MKV_SSA,
 *      MM_SBTL_TYPE_TEXT,
 *      MM_SBTL_TYPE_TEXT_SRT,
 *      MM_SBTL_TYPE_TEXT_SAMI,
 *      MM_SBTL_TYPE_TEXT_SVSUB,
 *      MM_SBTL_TYPE_TEXT_MPLAYER2,
 *      MM_SBTL_TYPE_TEXT_SSA,
 *      MM_SBTL_TYPE_TEXT_MDVDSUB,
 *      MM_SBTL_TYPE_TEXT_TXT,
 *      MM_SBTL_TYPE_TEXT_POWERDIVX,
 *      MM_SBTL_TYPE_TEXT_DVDSS,
 *      MM_SBTL_TYPE_END
 *  } MM_SBTL_TYPE_T;
 *  @endcode
 *  @li@c  MM_SBTL_TYPE_DIVX                                 -Divx subtitle
 *  @li@c  MM_SBTL_TYPE_VOBSUB                               -Vobsub subtitle
 *  @li@c  MM_SBTL_TYPE_PS_VOBSUB                               -Vob subtitle
 *  @li@c  MM_SBTL_TYPE_MKV_VOBSUB                           -mkv vobsub subtitle
 *  @li@c  MM_SBTL_TYPE_MKV_SRT                              -mkv srt subtitle 
 *  @li@c  MM_SBTL_TYPE_MKV_SSA                              -mkv ssa subtitle
 *  @li@c  MM_SBTL_TYPE_TEXT                                 -using text type for subtitle
 *  @li@c  MM_SBTL_TYPE_TEXT_SRT                             -srt text subtitle
 *  @li@c  MM_SBTL_TYPE_TEXT_SAMI                            -sami text subtitle
 *  @li@c  MM_SBTL_TYPE_TEXT_SVSUB                           -svsub text subtitle
 *  @li@c  MM_SBTL_TYPE_TEXT_MPLAYER2                        -mplayer2 text subtitle
 *  @li@c  MM_SBTL_TYPE_TEXT_SSA                             -ssa text subtitle
 *  @li@c  MM_SBTL_TYPE_TEXT_MDVDSUB                         -mdvdsub text subtitle
 *  @li@c  MM_SBTL_TYPE_TEXT_TXT                             -txt subtitle
 *  @li@c  MM_SBTL_TYPE_TEXT_POWERDIVX                       -powerdivx subtitle 
 *  @li@c  MM_SBTL_TYPE_TEXT_DVDSS                           -dvdss subtitle 
 *  @li@c  MM_SBTL_TYPE_END                                  -end type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SBTL_TYPE_DIVX,
    MM_SBTL_TYPE_MP4_BLURAY,
    MM_SBTL_TYPE_MP4_VOBSUB,
    MM_SBTL_TYPE_VOBSUB,
    MM_SBTL_TYPE_PS_VOBSUB,
    MM_SBTL_TYPE_MKV_VOBSUB,
    MM_SBTL_TYPE_MKV_SRT,
    MM_SBTL_TYPE_MKV_SSA,
    MM_SBTL_TYPE_TEXT,
    MM_SBTL_TYPE_TEXT_SRT,
    MM_SBTL_TYPE_TEXT_SAMI,
    MM_SBTL_TYPE_TEXT_SVSUB,
    MM_SBTL_TYPE_TEXT_MPLAYER2,
    MM_SBTL_TYPE_TEXT_SSA,
    MM_SBTL_TYPE_TEXT_MDVDSUB,
    MM_SBTL_TYPE_TEXT_TXT,
    MM_SBTL_TYPE_TEXT_POWERDIVX,
    MM_SBTL_TYPE_TEXT_DVDSS,
    MM_SBTL_TYPE_LYRIC_LRC,
    MM_SBTL_TYPE_LYRIC_KRK_KING,
    MM_SBTL_TYPE_LYRIC_MP3_KRK,
    MM_SBTL_TYPE_END
} MM_SBTL_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum MM_SBTL_DIVX_TYPE_T
 *  @brief Define the divx subtitle type
 *  @code
 *  typedef enum
 *  {    
 *      MM_DIVX_SBTL_TITLE_TYPE,    
 *      MM_DIVX_SBTL_MENU_TYPE
 *  } MM_SBTL_DIVX_TYPE_T;
 *  @endcode
 *  @li@c  MM_DIVX_SBTL_TITLE_TYPE                           -title divx subtitle
 *  @li@c  MM_DIVX_SBTL_MENU_TYPE                            -menu divx subtitle
 */
/*------------------------------------------------------------------*/
typedef enum
{    
    MM_DIVX_SBTL_TITLE_TYPE,    
    MM_DIVX_SBTL_MENU_TYPE
} MM_SBTL_DIVX_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum MM_SBTL_FEEDER_TYPE_T
 *  @brief This enum defines the feeder file type
 *  @code
 *  typedef enum
 *  {
 *      MM_SBTL_FEEDER_SUBTITLE = 0,
 *      MM_SBTL_FEEDER_INDEX,
 *      MM_SBTL_FEEDER_MAX
 *  } MM_SBTL_FEEDER_TYPE_T;
 *  @endcode
 *  @li@c  MM_SBTL_FEEDER_SUBTITLE                           -The feeder type is subtitel content
 *  @li@c  MM_SBTL_FEEDER_INDEX                              -The feeder type is subtitle index information
 *  @li@c  MM_SBTL_FEEDER_MAX                                -The max num of feeder
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SBTL_FEEDER_SUBTITLE = 0,
    MM_SBTL_FEEDER_INDEX,
    MM_SBTL_FEEDER_MAX
} MM_SBTL_FEEDER_TYPE_T;

/*------------------------------------------------------------------*/
/*! @brief The notify fucntion for subtitle state
 *  @param [in]  pv_tag:the subtitle state notify tag
 *                     e_cond: the notfiy condition
 *                     e_event: the notify event  
 *                     ui4_data: the notify data
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*sbtl_state_notify_fct)( 
    VOID*                 pv_tag,
    SBTL_COND_T           e_cond,
    UINT32                e_event,
    UINT32                ui4_data );


/*------------------------------------------------------------------*/
/*! @struct SBTL_MKV_CONTENT_ENCODING_T
 *  @brief This struct defines the mkv content encodeing information
 *  @code
 *  typedef struct _SBTL_MKV_CONTENT_ENCODING_T
 *  {
 *      UINT8       ui1_order;
 *      UINT8       ui1_scope;
 *      UINT8       ui1_type;
 *      UINT8       ui1_compress_algo;
 *      UINT32      ui4_comp_settings_length;
 *      UINT8       aui1_comp_settings_data[MM_MAX_MKV_COMP_SETTINGS_LENGTH];
 *  }SBTL_MKV_CONTENT_ENCODING_T;
 *  @endcode
 *  @li@c  ui1_order                                         -
 *  @li@c  ui1_scope                                         -
 *  @li@c  ui1_type                                          -
 *  @li@c  ui1_compress_algo                                 -
 *  @li@c  ui4_comp_settings_length                          -
 *  @li@c  aui1_comp_settings_data[MM_MAX_MKV_COMP_SETTINGS_LENGTH]    -
 */
/*------------------------------------------------------------------*/
typedef struct _SBTL_MKV_CONTENT_ENCODING_T
{
    UINT8       ui1_order;
    UINT8       ui1_scope;
    UINT8       ui1_type;
    UINT8       ui1_compress_algo;
    UINT32      ui4_comp_settings_length;
    UINT8       aui1_comp_settings_data[MM_MAX_MKV_COMP_SETTINGS_LENGTH];
}SBTL_MKV_CONTENT_ENCODING_T;


/*------------------------------------------------------------------*/
/*! @struct SBTL_MKV_CONTENT_ENCODINGS_T
 *  @brief This struct contains the mkv content encodings information
 *  @code
 *  typedef struct _SBTL_MKV_CONTENT_ENCODINGS_T
 *  {
 *      UINT8                           ui1_encoding_number;
 *      SBTL_MKV_CONTENT_ENCODING_T*    at_encodings;
 *  }SBTL_MKV_CONTENT_ENCODINGS_T;
 *  @endcode
 *  @li@c  ui1_encoding_number                               -The number of enconding
 *  @li@c  at_encodings                                      -the encodings information
 */
/*------------------------------------------------------------------*/
typedef struct _SBTL_MKV_CONTENT_ENCODINGS_T
{
    UINT8                           ui1_encoding_number;
    SBTL_MKV_CONTENT_ENCODING_T*    at_encodings;
}SBTL_MKV_CONTENT_ENCODINGS_T;

/*------------------------------------------------------------------*/
/*! @struct SBTL_MKV_CODEC_PRIVATE_T
 *  @brief This struct define the private codec
 *  @code
 *  typedef struct _SBTL_MKV_CODEC_PRIVATE_T
 *  {
 *      UINT64                      ui8_codec_private_len;
 *      UINT8*                      aui1_codec_private;
 *  }SBTL_MKV_CODEC_PRIVATE_T;
 *  @endcode
 *  @li@c  ui8_codec_private_len                             -The mkv codec private length
 *  @li@c  aui1_codec_private                                -The mkv codec private information
 */
/*------------------------------------------------------------------*/
typedef struct _SBTL_MKV_CODEC_PRIVATE_T
{
    UINT64                      ui8_codec_private_len;
    UINT8*                      aui1_codec_private;
}SBTL_MKV_CODEC_PRIVATE_T;

/*------------------------------------------------------------------*/
/*! @struct SBTL_MKV_SBTL_CMD_T
 *  @brief Define the mkv subtitle command information
 *  @code
 *  typedef struct _SBTL_MKV_SBTL_CMD_T
 *  {
 *      SBTL_MKV_CODEC_PRIVATE_T        t_mkv_sbtl_prv;
 *      SBTL_MKV_CONTENT_ENCODINGS_T    t_mkv_sbtl_cont_encode;
 *  }SBTL_MKV_SBTL_CMD_T;
 *  @endcode
 *  @li@c  t_mkv_sbtl_prv                                    -mkv subtitle private codec information
 *  @li@c  t_mkv_sbtl_cont_encode                            -mkv subtitle content encoding information
 */
/*------------------------------------------------------------------*/
typedef struct _SBTL_MKV_SBTL_CMD_T
{
    SBTL_MKV_CODEC_PRIVATE_T        t_mkv_sbtl_prv;
    SBTL_MKV_CONTENT_ENCODINGS_T    t_mkv_sbtl_cont_encode;
}SBTL_MKV_SBTL_CMD_T;

/*------------------------------------------------------------------*/
/*! @struct SBTL_DISPLAY_INFO_CMD_T
 *  @brief This struct defines the subtitle display command information 
 *  @code
 *  typedef struct _SBTL_DISPLAY_INFO_CMD_T
 *  {
 *      BOOL                        b_display_info;
 *      GL_RECT_T                   t_rect_refer;
 *      UINT8                       ui1_l_mgn_percent; 
 *      UINT8                       ui1_r_mgn_percent;
 *      UINT8                       ui1_b_mgn_percent;
 *      UINT8                       ui1_h_percent;
 *      UINT8                       ui1_font_min_grade;
 *      UINT8                       ui1_font_max_grade;
 *  }SBTL_DISPLAY_INFO_CMD_T;
 *  @endcode
 *  @li@c  b_display_info                                    -True:using the display information 
 for subtitle ;False:the subtitle display using default.
 *  @li@c  t_rect_refer                                      -preference region
 *  @li@c  ui1_l_mgn_percent                                 -The left percent of margin according to preference region
 *  @li@c  ui1_r_mgn_percent                                 -The right percent of margin according to preference region
 *  @li@c  ui1_b_mgn_percent                                 -The bottom percent of margin according to preference region
 *  @li@c  ui1_h_percent                                     -The height of subtitle region according to preference region
 *  @li@c  ui1_font_min_grade                              -The mix percent of subtitle font according to preference region
 *  @li@c  ui1_font_max_grade                              -The max percent of subtitle font according to preference region
 */
/*------------------------------------------------------------------*/
typedef struct _SBTL_DISPLAY_INFO_CMD_T
{
    /*True :subtitle has display information and need to do;False:use default*/
    BOOL                        b_display_info;

    /*The reference region*/
    GL_RECT_T                   t_rect_refer;
    UINT32                      ui4_scrn_width;
    UINT32                      ui4_scrn_height;

    /*The left,right,bottom margin and height percent according the video region*/    
    UINT8                       ui1_l_mgn_percent; 
    UINT8                       ui1_r_mgn_percent;
    UINT8                       ui1_b_mgn_percent;
    UINT8                       ui1_h_percent;

    /*The subtitle font display min\max height information*/
    UINT8                       ui1_font_min_grade;
    UINT8                       ui1_font_max_grade;
}SBTL_DISPLAY_INFO_CMD_T;

typedef struct _SBTL_LYRIC_INFO_T
{
    BOOL                       b_page_oneline;
    UINT8                      ui1_row_num_page; 
    UINT8                      ui1_max_chars_row; 
    UINT8                      ui1_row_s;                         
    UINT8                      ui1_disp_row_max;                         
}SBTL_LYRIC_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MM_SBTL_PAGE_MODE
 *  @brief Define the page display mode
 *  @code
 *  typedef enum
 *  {
 *      MM_SBTL_PAGE_SCROLL_LINE ,
 *      MM_SBTL_PAGE_HLT_FIXED ,
 *   } MM_SBTL_PAGE_MODE;
 *  @endcode
 *  @li@c  MM_SBTL_PAGE_SCROLL_LINE              -Scroll highlight the next line and fresh page only when on the last or top line.
 *  @li@c  MM_SBTL_PAGE_HLT_FIXED                 -Fixed highlight on the page.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SBTL_PAGE_SCROLL_LINE ,
    MM_SBTL_PAGE_HLT_FIXED 
} MM_SBTL_PAGE_MODE;

/*------------------------------------------------------------------*/
/*! @struct MM_SBTL_PAGE_MODE
 *  @brief Define the page display mode
 *  @code
 *  typedef struct _MM_SBTL_DISP_PAGE_T
 *  {
 *      MM_SBTL_PAGE_MODE           e_page_mode;
 *      UINT16                      ui2_page_line_num;                         
 *      UINT16                      ui2_hlt_fix_line; 
 *      UINT16                      ui2_interval_line; 
 *      INT32                       i4_line_gap;
 *   } MM_SBTL_DISP_PAGE_T;
 *  @endcode
 *  @li@c  e_page_mode              -Idicate the display page mode.
 *  @li@c  ui2_page_line_num      -Idicate the display total line number.
 *  @li@c  ui2_hlt_fix_line            -Idicate the fixed highlight line,it is valid when e_page_mode is MM_SBTL_PAGE_HLT_FIXED.
 *  @li@c  ui2_interval_line          -Idicates the interval line number.
 *  @li@c  i4_line_gap              -Idicates the line gap.The line height is font height + i4_line_gap.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SBTL_DISP_PAGE_T
{
    MM_SBTL_PAGE_MODE           e_page_mode;
    UINT16                      ui2_page_line_num;                         
    UINT16                      ui2_hlt_fix_line; 
    UINT16                      ui2_interval_line; 
    INT32                       i4_line_gap;
} MM_SBTL_DISP_PAGE_T;

/*------------------------------------------------------------------*/
/*! @struct MM_SBTL_TIME_OFST_MODE
 *  @brief Define timeoffset mode
 *  @code
 *  typedef enum
 *  {
 *      MM_SBTL_TIME_OFST_OFF ,
 *      MM_SBTL_TIME_OFST_AUTO ,
 *      MM_SBTL_TIME_OFST_USER
 *   } MM_SBTL_TIME_OFST_MODE;
 *  @endcode
 *  @li@c  MM_SBTL_TIME_OFST_OFF             -Igore the time offset tag in the lyric file.
 *  @li@c  MM_SBTL_TIME_OFST_AUTO           -Use the offset tag in the lyric file if available.
 *  @li@c  MM_SBTL_TIME_OFST_USER           -Use the offset value that user inputs.
 */
/*------------------------------------------------------------------*/
typedef enum
{    
    MM_SBTL_TM_OFST_MODE_OFF,    
    MM_SBTL_TM_OFST_MODE_AUTO,
    MM_SBTL_TM_OFST_MODE_USER
} MM_SBTL_TM_OFST_MODE;
/*------------------------------------------------------------------*/
/*! @struct MM_SBTL_TM_OFST_INF
 *  @brief Define the time offset mode information.
 *  @code
 *  typedef struct _MM_SBTL_TIME_OFFSET
 *  {
 *      MM_SBTL_TM_OFST_MODE           e_ofst_mode;
 *      INT64                      i8_time_ofst;                         
 *   } MM_SBTL_TM_OFST_INF;
 *  @endcode
 *  @li@c  e_ofst_mode              -Idicate the time offset mode.
 *  @li@c  i8_time_ofst      -Idicate the offset time when the mode is MM_SBTL_TIME_OFST_USER.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SBTL_TM_OFST_INF
{
    MM_SBTL_TM_OFST_MODE       e_ofst_mode;
    INT64                      i8_time_ofst;                         
} MM_SBTL_TM_OFST_INF;

typedef enum
{    
    MM_SBTL_INF_RCD_VIDEO,    
    MM_SBTL_INF_RCD_AUDIO,    
    MM_SBTL_INF_RCD_NUM    
} MM_SBTL_INF_RCD_TYPE;

typedef struct _MM_SBTL_RECORD_INF
{
    UINT32                       aui1_rcd_mask[MM_SBTL_INF_RCD_NUM];
    WGL_FONT_INFO_T             at_font_info[MM_SBTL_INF_RCD_NUM];
    MM_SBTL_ENCODING_T          e_coding[MM_SBTL_INF_RCD_NUM];
	MM_SBTL_TM_OFST_INF         t_tm_ofst[MM_SBTL_INF_RCD_NUM];
	GL_POINT_T                  t_osd_ofst[MM_SBTL_INF_RCD_NUM];
	WGL_COLOR_INFO_T            t_fg_color[MM_SBTL_INF_RCD_NUM];
	WGL_COLOR_INFO_T            t_bg_color[MM_SBTL_INF_RCD_NUM];
} MM_SBTL_RECORD_INF;

typedef BOOL (*x_mm_sbtl_pts_cb) (  VOID*           pv_nfy_tag,
                                      UINT64*         pui4_data );

typedef struct _MM_SBTL_PTS_CB
{
    x_mm_sbtl_pts_cb            pf_pts_cb;
    VOID*                       pv_pts_cb_tag;
} MM_SBTL_PTS_CB;



#define CONTAINER_LEN   16    /**<The max length of container name.*/
#define SAMI_CC_SZ      3    /**<The size of sami stream information*/

/*------------------------------------------------------------------*/
/*! @struct SMAI_CC_INF_T
 *  @brief The smai container information
 *  @code
 *   typedef struct _SMAI_CC_INF_T
 *  {
 *    UINT16  s_class_name[CONTAINER_LEN];   
 *    UINT8 ui1_name_len;
 *    UINT8 aui1_lang[2];                      
 *  }SMAI_CC_INF_T;
 *  @endcode
 *  @li@c  s_class_name[CONTAINER_LEN]                       -The container name 
 *  @li@c  ui1_name_len                                      -The length of container name
 *  @li@c  aui1_lang[2]                                      -The language id
 */
/*------------------------------------------------------------------*/
 typedef struct _SMAI_CC_INF_T
{
  UINT16  s_class_name[CONTAINER_LEN];   /*container name, max for 12 bytes now*/
  UINT8 ui1_name_len;
  UINT8 aui1_lang[2];                      /*lang idx*/
}SMAI_CC_INF_T;


/*------------------------------------------------------------------*/
/*! @struct SAMI_STREAM_INFO_T
 *  @brief This struct contains the sami stream information
 *  @code
 *  typedef struct _SAMI_STREAM_INFO_T
 *  {
 *      UINT8           ui1_total_stream_count;
 *      UINT8           ui1_stream_idx;
 *      SMAI_CC_INF_T   at_stream_info[SAMI_CC_SZ];
 *  }SAMI_STREAM_INFO_T;
 *  @endcode
 *  @li@c  ui1_total_stream_count                            -Total num of stream
 *  @li@c  ui1_stream_idx                                    -The stream index
 *  @li@c  at_stream_info[SAMI_CC_SZ]                        -The information of stream
 */
/*------------------------------------------------------------------*/
typedef struct _SAMI_STREAM_INFO_T
{
    UINT8           ui1_total_stream_count;
    UINT8           ui1_stream_idx;
    SMAI_CC_INF_T   at_stream_info[SAMI_CC_SZ];
}SAMI_STREAM_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SBTL_LANG_DESC_T
 *  @brief This struct contains the langugage information in subtitle file
 *  @code
 *  typedef struct _SBTL_LANG_DESC_T
 *  {
 *      UINT8                      ui1_lang_count;
 *      CHAR                       c_lang_list[MM_SBTL_PREFERED_LANG_LIST_LEN][3];
 *      CHAR*                      pc_sblt_file_name;    
 *      CHAR*                      pc_related_file_name;
 *  }SBTL_LANG_DESC_T;
 *  @endcode
 *  @li@c  ui1_lang_count                                 -Total num of language
 *  @li@c  c_lang_list                                       -The language list
 *  @li@c  pc_file_name                                   -The subtitle file name
 *  @li@c  pc_related_file_name                        -The related subtitle file name
 */
/*------------------------------------------------------------------*/
typedef struct _SBTL_LANG_DESC_T
{
    BOOL                       b_is_http_file;
    UINT8                      ui1_lang_count;
    CHAR                       c_lang_list[MM_SBTL_PREFERED_LANG_LIST_LEN][3];
    CHAR*                      pc_sbtl_file_name;
    CHAR*                      pc_related_file_name;
}SBTL_LANG_DESC_T;

extern INT32 x_sbtl_util_fcopybytes_fct(
                        VOID*       pv_obj, 
                        VOID*       pv_buf, 
                        SIZE_T      z_buf_leng, 
                        SIZE_T      z_copy_size,
                        UINT32*     pui4_size);

extern INT32 x_sbtl_util_fsetpos_fct(
    VOID*        pv_obj, 
    INT64        i8_offset, 
    UINT8        ui1_whence);

extern INT32 x_sbtl_util_fgetpos_fct(
    VOID*       pv_obj, 
    UINT64*     pui8_cur_pos);

extern INT32 x_sbtl_util_finput1bytes_fct(
    VOID*       pv_obj, 
    UINT8*      pui1_data);

extern INT32 x_sbtl_parse_lang(
                SBTL_LANG_DESC_T* t_sbtl_lang_desc
                );
extern INT32 x_vobsub_parse_lang(
                HANDLE_T h_file_subtitle, 
                HANDLE_T h_file_index,
                MM_SBTL_TYPE_T t_mm_sbtl_type,
                SBTL_LANG_DESC_T* t_sbtl_lang_desc);
#endif /* _U_MM_SBTL_ENGINE_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_STRM_MNGR_MM_SBTL_HDLR*/
/*----------------------------------------------------------------------------*/


