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
/*! @file x_wgl_cc_isdb_type.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 */
/*----------------------------------------------------------------------------*/
#ifndef _X_WGL_CC_ISDB_TYPE_H_
#define _X_WGL_CC_ISDB_TYPE_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "x_wgl.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_WGL_CCI
*  @{  
*/
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define WGL_CMD_CCI_ADD_BLOCK_GRP       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC_ISDB, 0)    /**< Sent new block group to CCI widget */
#define WGL_CMD_CCI_CLEAR_ALL           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC_ISDB, 1)    /**< Clear all data in CCI widget */
#define WGL_CMD_CCI_ERASE_GRP           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC_ISDB, 2)    /**< Erase block group in CCI widget */
#define WGL_CMD_CCI_ROLL_MODE           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC_ISDB, 3)    /**< Make CCI widget to roll mode */


/* ------------------------------parent class--------------------------------------- */
/*------------------------------------------------------------------*/
/*! @enum WGL_CCI_BLK_TYPE_T
 *  @brief This enumeration contains the block type
 *  @code
 *  typedef enum
 *  {
 *      WGL_CCI_BLK_TYPE_UNKNOWN = 0,
 *      WGL_CCI_BLK_TYPE_PRO,
 *      WGL_CCI_BLK_TYPE_BITMAP,
 *      WGL_CCI_BLK_TYPE_TEXT,
 *      WGL_CCI_BLK_TYPE_PRIVATE
 *  }   WGL_CCI_BLK_TYPE_T;
 *  @endcode
 *  @li@c  WGL_CCI_BLK_TYPE_UNKNOWN                          - Type is UNKNOWN
 *  @li@c  WGL_CCI_BLK_TYPE_PRO                              - Type is PROPERTY
 *  @li@c  WGL_CCI_BLK_TYPE_BITMAP                           - Type is BITMAP 
 *  @li@c  WGL_CCI_BLK_TYPE_TEXT                             - Type is TEXT   
 *  @li@c  WGL_CCI_BLK_TYPE_PRIVATE                          - Type is PRIVATE   
 */
/*------------------------------------------------------------------*/
typedef enum
{
    WGL_CCI_BLK_TYPE_UNKNOWN = 0,
    WGL_CCI_BLK_TYPE_PRO,
    WGL_CCI_BLK_TYPE_BITMAP,
    WGL_CCI_BLK_TYPE_TEXT,
    WGL_CCI_BLK_TYPE_PRIVATE
}   WGL_CCI_BLK_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_CCI_BLK_T
 *  @brief This structure is father class of block
 *  @code
 *  typedef struct  _WGL_CCI_BLK_T
 *  {
 *      WGL_CCI_BLK_TYPE_T          t_blk_type;
 *  }   WGL_CCI_BLK_T;
 *  @endcode
 *  @li@c  t_blk_type                                        - Block type
 */
/*------------------------------------------------------------------*/
typedef struct  _WGL_CCI_BLK_T
{
    WGL_CCI_BLK_TYPE_T          t_blk_type;
}   WGL_CCI_BLK_T;


/* ------------------------------children class 1-------------property--------------- */
typedef UINT16                          WGL_CCI_BLK_PRO_TYPE;    /**<        */
#define WGL_CCI_BLK_PRO_TYPE_NONE       ((WGL_CCI_BLK_PRO_TYPE)0x0000)    /**< Property type is NONE */
#define WGL_CCI_BLK_PRO_TYPE_COOR       ((WGL_CCI_BLK_PRO_TYPE)0x0001)    /**< Property type is COOR */
#define WGL_CCI_BLK_PRO_TYPE_POSI       ((WGL_CCI_BLK_PRO_TYPE)0x0002)    /**< Property type is POSI */
#define WGL_CCI_BLK_PRO_TYPE_COLOR      ((WGL_CCI_BLK_PRO_TYPE)0x0004)    /**< Property type is COLOR */
#define WGL_CCI_BLK_PRO_TYPE_FONT       ((WGL_CCI_BLK_PRO_TYPE)0x0008)    /**< Property type is FONT */
#define WGL_CCI_BLK_PRO_TYPE_CMD        ((WGL_CCI_BLK_PRO_TYPE)0x0010)    /**< Property type is CMD */
#define WGL_CCI_BLK_PRO_TYPE_ROLL       ((WGL_CCI_BLK_PRO_TYPE)0x0020)    /**< Property type is ROLL */

#define WGL_CCI_BLK_PRO_TYPE_ALL        ((WGL_CCI_BLK_PRO_TYPE)0x000f)    /**< Property type is ALL */
#define WGL_CCI_BLK_PRO_TYPE_ROLL_ALL   ((WGL_CCI_BLK_PRO_TYPE)0x002f)    /**< Property type is ROLL_ALL */


/*------------------------------------------------------------------*/
/*! @enum WGL_CCI_WRITE_FORM
 *  @brief This structure contains writing format
 *  @code
 *  typedef enum
 *  {
 *      WGL_CCI_WRITING_UNKNOWN = 0,
 *      WGL_CCI_HORI_WRITING    = 1,
 *      WGL_CCI_VERT_WRITING    = 2
 *  }   WGL_CCI_WRITE_FORM;
 *  @endcode
 *  @li@c  WGL_CCI_WRITING_UNKNOWN                           - unknown writing
 *  @li@c  WGL_CCI_HORI_WRITING                              - horizontal writing
 *  @li@c  WGL_CCI_VERT_WRITING                              - vertical writing
 */
/*------------------------------------------------------------------*/
typedef enum
{
    WGL_CCI_WRITING_UNKNOWN = 0,
    WGL_CCI_HORI_WRITING    = 1,
    WGL_CCI_VERT_WRITING    = 2
}   WGL_CCI_WRITE_FORM;


/*------------------------------------------------------------------*/
/*! @struct WGL_CCI_BLK_COOR_T
 *  @brief This structure contains coordinate block data
 *  @code
 *  typedef struct  _WGL_CCI_BLK_COOR_T
 *  {
 *      WGL_CCI_WRITE_FORM          t_write_form;
 *      UINT16                      ui2_coor_w;
 *      UINT16                      ui2_coor_h;
 *      UINT16                      ui2_sdp_h;
 *      UINT16                      ui2_sdp_v;
 *      UINT16                      ui2_sdf_h;
 *      UINT16                      ui2_sdf_v;
 *  }   WGL_CCI_BLK_COOR_T;                                 
 *  @endcode
 *  @li@c  t_write_form                                      - writing format
 *  @li@c  ui2_coor_w                                        - coordinate width
 *  @li@c  ui2_coor_h                                        - coordinate height
 *  @li@c  ui2_sdp_h                                         - sdp horizontal;
 *  @li@c  ui2_sdp_v                                         - sdp vertical;
 *  @li@c  ui2_sdf_h                                         - sdf horizontal;
 *  @li@c  ui2_sdf_v                                         - sdf vertical;
 */
/*------------------------------------------------------------------*/
typedef struct  _WGL_CCI_BLK_COOR_T
{
    WGL_CCI_WRITE_FORM          t_write_form;
    UINT16                      ui2_coor_w;
    UINT16                      ui2_coor_h;
    UINT16                      ui2_sdp_h;
    UINT16                      ui2_sdp_v;
    UINT16                      ui2_sdf_h;
    UINT16                      ui2_sdf_v;
}   WGL_CCI_BLK_COOR_T;                                 /* coordinate */


/*------------------------------------------------------------------*/
/*! @struct WGL_CCI_BLK_COLOR_T
 *  @brief This structure contains color block data
 *  @code
 *  typedef struct  _WGL_CCI_BLK_COLOR_T
 *  {
 *      UINT8                       ui1_fc_idx;             
 *      UINT8                       ui1_bc_idx;             
 *      UINT8                       ui1_ec_idx;             
 *      UINT8                       ui1_eec_idx;            
 *      UINT8                       ui1_stl;
 *      UINT8                       ui1_hlc;
 *  }   WGL_CCI_BLK_COLOR_T;
 *  @endcode
 *  @li@c  ui1_fc_idx                                        - foreground color index
 *  @li@c  ui1_bc_idx                                        - background color index
 *  @li@c  ui1_ec_idx                                        - edge color index
 *  @li@c  ui1_eec_idx                                       - edge of edge color index
 *  @li@c  ui1_stl                                           - STL color index
 *  @li@c  ui1_hlc                                           - HLC color index
 *  @li@c  ui1_orn                                           - ORN type
 *  @li@c  ui1_ornc_idx                                      - ORN color index
 */
/*------------------------------------------------------------------*/
typedef struct  _WGL_CCI_BLK_COLOR_T
{
    UINT8                       ui1_fc_idx;             /* foreground color index */
    UINT8                       ui1_bc_idx;             /* background color index */
    UINT8                       ui1_ec_idx;             /* edge color */
    UINT8                       ui1_eec_idx;            /* edge of edge color */
    UINT8                       ui1_stl;
    UINT8                       ui1_hlc;
    UINT8                       ui1_orn;
    UINT8                       ui1_ornc_idx;
}   WGL_CCI_BLK_COLOR_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_CCI_BLK_POSITION_T
 *  @brief This structure contains position block data
 *  @code
 *  typedef struct  _WGL_CCI_BLK_POSITION_T
 *  {
 *      UINT16                      ui2_left;
 *      UINT16                      ui2_top;
 *  }   WGL_CCI_BLK_POSITION_T;
 *  @endcode
 *  @li@c  ui2_left                                          - Left position
 *  @li@c  ui2_top                                           - Top position
 */
/*------------------------------------------------------------------*/
typedef struct  _WGL_CCI_BLK_POSITION_T
{
    UINT16                      ui2_left;
    UINT16                      ui2_top;
}   WGL_CCI_BLK_POSITION_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_CCI_BLK_FONT_T
 *  @brief This structure contains font block data
 *  @code
 *  typedef struct  _WGL_CCI_BLK_FONT_T
 *  {
 *      UINT16                      ui2_char_h;
 *      UINT16                      ui2_char_v;
 *      UINT16                      ui2_space_h;
 *      UINT16                      ui2_space_v;
 *  }   WGL_CCI_BLK_FONT_T;
 *  @endcode
 *  @li@c  ui2_char_h                                        - Character width
 *  @li@c  ui2_char_v                                        - Character height
 *  @li@c  ui2_space_h                                       - Character width
 *  @li@c  ui2_space_v                                       - Character height
 */
/*------------------------------------------------------------------*/
typedef struct  _WGL_CCI_BLK_FONT_T
{
    UINT16                      ui2_char_h;
    UINT16                      ui2_char_v;
    UINT16                      ui2_space_h;
    UINT16                      ui2_space_v;
}   WGL_CCI_BLK_FONT_T;


/*------------------------------------------------------------------*/
/*! @enum WGL_CCI_BLK_CMD_TYPE_T
 *  @brief This enumeration contains command type
 *  @code
 *  typedef enum
 *  {
 *      WGL_CCI_BLK_CMD_UNKNOWN = 0,
 *      WGL_CCI_BLK_CMD_CS,
 *      WGL_CCI_BLK_CMD_RCS,
 *  }   WGL_CCI_BLK_CMD_TYPE_T;
 *  @endcode
 *  @li@c  WGL_CCI_BLK_CMD_UNKNOWN                           - the command is UNKNOWN
 *  @li@c  WGL_CCI_BLK_CMD_CS                                - the command is CS
 *  @li@c  WGL_CCI_BLK_CMD_RCS                               - the command is RCS
 */
/*------------------------------------------------------------------*/
typedef enum
{
    WGL_CCI_BLK_CMD_UNKNOWN = 0,
    WGL_CCI_BLK_CMD_CS,
    WGL_CCI_BLK_CMD_RCS,
}   WGL_CCI_BLK_CMD_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_CCI_BLK_CMD_T
 *  @brief This structure contains command block data
 *  @code
 *  typedef struct  _WGL_CCI_BLK_CMD_T
 *  {
 *      WGL_CCI_BLK_CMD_TYPE_T      t_cmd_type;
 *      UINT32                      ui4_cmd_data;
 *  }   WGL_CCI_BLK_CMD_T;
 *  @endcode
 *  @li@c  t_cmd_type                                        - command type
 *  @li@c  ui4_cmd_data                                      - command data
 */
/*------------------------------------------------------------------*/
typedef struct  _WGL_CCI_BLK_CMD_T
{
    WGL_CCI_BLK_CMD_TYPE_T      t_cmd_type;
    UINT32                      ui4_cmd_data;
}   WGL_CCI_BLK_CMD_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_CCI_BLK_ROLL_T
 *  @brief This structure contains roll mode block data
 *  @code
 *  typedef struct  _WGL_CCI_BLK_ROLL_T
 *  {
 *      UINT16                      ui2_left;
 *      UINT16                      ui2_top;
 *      UINT16                      ui2_width;
 *      UINT16                      ui2_height;
 *  }   WGL_CCI_BLK_ROLL_T;
 *  @endcode
 *  @li@c  ui2_left                                          - roll area left position
 *  @li@c  ui2_top                                           - roll area top position
 *  @li@c  ui2_width                                         - roll area width
 *  @li@c  ui2_height                                        - roll area height
 */
/*------------------------------------------------------------------*/
typedef struct  _WGL_CCI_BLK_ROLL_T
{
    UINT16                      ui2_left;
    UINT16                      ui2_top;
    UINT16                      ui2_width;
    UINT16                      ui2_height;
}   WGL_CCI_BLK_ROLL_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_CCI_BLK_PRO_T
 *  @brief This structure contains property block data
 *  @code
 *  typedef struct  _WGL_CCI_BLK_PRO_T
 *  {
 *      WGL_CCI_BLK_T               t_blk;                  
 *      WGL_CCI_BLK_PRO_TYPE        t_pro_type;             
 *      union
 *      {
 *          WGL_CCI_BLK_COOR_T      t_coor;
 *          WGL_CCI_BLK_COLOR_T     t_color;
 *          WGL_CCI_BLK_POSITION_T  t_position;
 *          WGL_CCI_BLK_FONT_T      t_font;
 *          WGL_CCI_BLK_CMD_T       t_cmd;
 *          WGL_CCI_BLK_ROLL_T      t_roll;
 *      }   u;
 *  }   WGL_CCI_BLK_PRO_T;
 *  @endcode
 *  @li@c  t_blk                                             - father class
 *  @li@c  t_pro_type                                        - property type
 *  @li@c  t_coor                                            - coordinate structure
 *  @li@c  t_color                                           - color structure
 *  @li@c  t_position                                        - position structure
 *  @li@c  t_font                                            - font structure
 *  @li@c  t_cmd                                             - command structure
 *  @li@c  t_roll                                            - roll mode structure
 */
/*------------------------------------------------------------------*/
typedef struct  _WGL_CCI_BLK_PRO_T
{
    WGL_CCI_BLK_T               t_blk;                  /* !!! parent feature !!! */
    
    WGL_CCI_BLK_PRO_TYPE        t_pro_type;             /* WGL_CCI_BLK_PRO_TYPE */

    union
    {
        WGL_CCI_BLK_COOR_T      t_coor;
        WGL_CCI_BLK_COLOR_T     t_color;
        WGL_CCI_BLK_POSITION_T  t_position;
        WGL_CCI_BLK_FONT_T      t_font;
        WGL_CCI_BLK_CMD_T       t_cmd;
        WGL_CCI_BLK_ROLL_T      t_roll;
    }   u;
    
}   WGL_CCI_BLK_PRO_T;


/* - -----------------------------children class 2-------------text--------------- */

/* If the text length is bigger than WGL_CCI_BLK_TEXT_DEFAULT_LEN */
/* handler can use more than one text block to translate the text */
#define         WGL_CCI_BLK_TEXT_DEFAULT_LEN    ((UINT16)20)    /**< The default text string length in text block */


/*------------------------------------------------------------------*/
/*! @enum WGL_CCI_FLC_TYPE
 *  @brief This enumeration contains flash type
 *  @code
 *  typedef enum
 *  {
 *      WGL_CCI_NO_FLC      = 0,
 *      WGL_CCI_NORMAL_FLC  = 1,
 *      WGL_CCI_REVERSE_FLC = 2,
 *      WGL_CCI_UNKNOWN_FLC = 3
 *  }   WGL_CCI_FLC_TYPE;
 *  @endcode
 *  @li@c  WGL_CCI_NO_FLC                                    - need not to flash
 *  @li@c  WGL_CCI_NORMAL_FLC                                - normal flash
 *  @li@c  WGL_CCI_REVERSE_FLC                               - color reverse flash
 *  @li@c  WGL_CCI_UNKNOWN_FLC                               - unknown flash
 */
/*------------------------------------------------------------------*/
typedef enum
{
    WGL_CCI_NO_FLC      = 0,
    WGL_CCI_NORMAL_FLC  = 1,
    WGL_CCI_REVERSE_FLC = 2,
    WGL_CCI_UNKNOWN_FLC = 3
}   WGL_CCI_FLC_TYPE;


/*------------------------------------------------------------------*/
/*! @enum WGL_CCI_SCR_TYPE
 *  @brief This enumeration contains scroll designation type
 *  @code
 *  typedef enum
 *  {
 *      WGL_CCI_NO_SCR      = 0,
 *      WGL_CCI_NO_OUT_SCR  = 1,
 *      WGL_CCI_OUT_SCR     = 2,
 *      WGL_CCI_UNKNOWN_SCR = 3
 *  }   WGL_CCI_SCR_TYPE;                                   
 *  @endcode
 *  @li@c  WGL_CCI_NO_SCR                                    - need not to scroll designation
 *  @li@c  WGL_CCI_NO_OUT_SCR                                - scroll designation need not to out
 *  @li@c  WGL_CCI_OUT_SCR                                   - scroll designation need to out
 *  @li@c  WGL_CCI_UNKNOWN_SCR                               - unknown scroll designation
 */
/*------------------------------------------------------------------*/
typedef enum
{
    WGL_CCI_NO_SCR      = 0,
    WGL_CCI_NO_OUT_SCR  = 1,
    WGL_CCI_OUT_SCR     = 2,
    WGL_CCI_UNKNOWN_SCR = 3
}   WGL_CCI_SCR_TYPE;                                   /* scroll designation */


/*------------------------------------------------------------------*/
/*! @struct WGL_CCI_BLK_TEXT_T
 *  @brief This structure contains text block data
 *  @code
 *  typedef struct  _WGL_CCI_BLK_TEXT_T
 *  {
 *      WGL_CCI_BLK_T               t_blk;                  
 *      WGL_CCI_FLC_TYPE            t_flc_type;             
 *      WGL_CCI_SCR_TYPE            t_scr_type;             
 *      UINT16                      ui2_scr_speed;          
 *      UTF16_T                     ac_utf16_txt[WGL_CCI_BLK_TEXT_DEFAULT_LEN + 1];
 *      SIZE_T                      z_utf16_len;
 *      UINT16                      ui2_private1;           
 *      INT32                       ai4_private2[4];        
 *      UINT32                      ui4_private3[2];        
 *  }   WGL_CCI_BLK_TEXT_T;
 *  @endcode
 *  @li@c  t_blk                                             - father class
 *  @li@c  t_flc_type                                        - flash type
 *  @li@c  t_scr_type                                        - scroll designation type
 *  @li@c  ui2_scr_speed                                     - scroll designation speed
 *  @li@c  ac_utf16_txt[WGL_CCI_BLK_TEXT_DEFAULT_LEN + 1]    - text string
 *  @li@c  z_utf16_len                                       - string length
 *  @li@c  ui2_private1                                      - widget private1 data
 *  @li@c  ai4_private2[4]                                   - widget private2 data
 *  @li@c  ui4_private3[2]                                   - widget private3 data
 */
/*------------------------------------------------------------------*/
typedef struct  _WGL_CCI_BLK_TEXT_T
{
    WGL_CCI_BLK_T               t_blk;                  /* !!! parent feature !!! */

    WGL_CCI_FLC_TYPE            t_flc_type;             /* between foreground and background color */
    WGL_CCI_SCR_TYPE            t_scr_type;             /* line roll type */
    UINT16                      ui2_scr_speed;          /* valid only when it is WGL_CCI_NO_OUT_SCR or WGL_CCI_OUT_SCR */
    
    UTF16_T                     ac_utf16_txt[WGL_CCI_BLK_TEXT_DEFAULT_LEN + 1];
    SIZE_T                      z_utf16_len;

    UINT16                      ui2_private1;           /* width in HORI_WRITING/ height in VERT_WRITING */
    INT32                       ai4_private2[4];        /* physical rectangle: left+top+right+bottom */
    UINT32                      ui4_private3[2];        /* backgroud and foreground surface */
}   WGL_CCI_BLK_TEXT_T;


/* - -----------------------------children class 3-------------bitmap------------- */
/*------------------------------------------------------------------*/
/*! @struct WGL_CCI_BLK_BITMAP_T
 *  @brief This structure contains text bitmap data
 *  @code
 *  typedef struct  _WGL_CCI_BLK_BITMAP_T
 *  {
 *      WGL_CCI_BLK_T               t_blk;                  
 *      UINT8                       ui1_rcs_color;
 *      UINT16                      ui2_position_x;
 *      UINT16                      ui2_position_y;    
 *      UINT8*                      pui1_flash_color;       
 *      UINT8                       ui1_num_colors;    
 *      UINT8*                      pui1_data;
 *      UINT32                      ui4_data_len;
 *      UINT16                      ui2_bmp_width; 
 *      UINT16                      ui2_bmp_height;   
 *      HANDLE_T                    h_odd_surface; 
 *      HANDLE_T                    h_even_surface;   
 *      INT32                       ai4_private1[4];        
 *  }   WGL_CCI_BLK_BITMAP_T;
 *  @endcode
 *  @li@c  t_blk                                             - father class
 *  @li@c  ui1_rcs_color                                     - RCS color
 *  @li@c  ui2_position_x                                    - left position
 *  @li@c  ui2_position_y                                    - top position
 *  @li@c  pui1_flash_color                                  - flash colors
 *  @li@c  ui1_num_colors                                    - flash color number
 *  @li@c  pui1_data                                         - bitmap data
 *  @li@c  ui4_data_len                                      - bitmap data length
 *  @li@c  ui2_bmp_width                                     - logical width
 *  @li@c  ui2_bmp_height                                    - logical height
 *  @li@c  h_odd_surface                                     - bitmap odd surface
 *  @li@c  h_even_surface                                    - bitmap even surface
 *  @li@c  ai4_private1[4]                                   - widget private1 data
 */
/*------------------------------------------------------------------*/
typedef struct  _WGL_CCI_BLK_BITMAP_T
{
    WGL_CCI_BLK_T               t_blk;                  /* !!! parent feature !!! */

    UINT16                      ui2_position_x;
    UINT16                      ui2_position_y;    
    UINT8                       ui1_num_colors;    
    UINT16                      ui2_bmp_width;          /* logical width */
    UINT16                      ui2_bmp_height;         /* logical height*/
    HANDLE_T                    h_odd_surface;          /* h_odd_surface */
    HANDLE_T                    h_even_surface;         /* h_even_surface*/

    INT32                       ai4_private1[4];        /* physical rectangle: left+top+right+bottom */
}   WGL_CCI_BLK_BITMAP_T;


/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief Return the block data
 *  @param [in]     pt_block                - The block structrue
 *  @param [in]     pv_tag                  - The user private data
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      WGLR_OK                - The routine was successful.
 *  @retval      WGLR_OUT_OF_MEMORY     - The routine was fail.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_cci_block_nfy)(
                WGL_CCI_BLK_T*          pt_block,
                VOID*                   pv_tag );

/*------------------------------------------------------------------*/
/*! @brief Release the block group
 *  @param [in]     ui4_grp_data            - The group data
 *  @param [in]     ui4_grp_id              - The group id
 *  @param [in]     pv_tag                  - User private data
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      WGLR_OK                - The routine was successful.
 *  @retval      WGLR_OUT_OF_MEMORY     - The routine was fail.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_cci_release_group)(
                UINT32                  ui4_grp_data,
                UINT32                  ui4_grp_id,
                VOID*                   pv_tag );


/*------------------------------------------------------------------*/
/*! @brief Get blocks from the group
 *  @param [in]     ui4_grp_data            - The group data
 *  @param [in]     ui4_grp_id              - The group id
 *  @param [in]     pv_tag                  - User private data
 *  @param [out]    pf_blk_nfy              - Notification function of block
 *  @param [out]    pv_blk_nfy_tag          - Notification function private data
 *  @note
 *  @see
 *  @return
 *  @retval      WGLR_OK                - The routine was successful.
 *  @retval      WGLR_OUT_OF_MEMORY     - The routine was fail.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_cci_get_grp_blocks)(
                UINT32                  ui4_grp_data,
                UINT32                  ui4_grp_id,
                VOID*                   pv_tag,
                wgl_cci_block_nfy       pf_blk_nfy,
                VOID*                   pv_blk_nfy_tag );


/*------------------------------------------------------------------*/
/*! @brief Get a DRCS pattern with special uincode
 *  @param [in]     pv_tag                  - User private data
 *  @param [in]     ui2_unicode,            - The uincode
 *  @param [out]    pui1_width,             - pattern width
 *  @param [out]    pui1_height,            - pattern height
 *  @param [out]    ppui1_pattern_data      - pattern data
 *  @note
 *  @see
 *  @return
 *  @retval      WGLR_OK                - The routine was successful.
 *  @retval      WGLR_OUT_OF_MEMORY     - The routine was fail.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_cci_get_drcs_pattern)(
                VOID*                   pv_tag,
                UINT16                  ui2_unicode,
                UINT8*                  pui1_width,
                UINT8*                  pui1_height,
                UINT8**                 ppui1_pattern_data );


/*------------------------------------------------------------------*/
/*! @brief Get a DRCS pattern with special uincode
 *  @param [in]     pv_tag                  - User private data
 *  @note
 *  @see
 *  @return
 *  @retval      WGLR_OK                - The routine was successful.
 *  @retval      WGLR_OUT_OF_MEMORY     - The routine was fail.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_cci_destroy_finished)(
                VOID*                   pv_tag );


/*------------------------------------------------------------------*/
/*! @struct
 *  @brief This structure contains the function and tag for getting block data
 *  @code
 *  typedef struct _WGL_CCI_INIT_T
 *  {
 *      wgl_cci_get_grp_blocks              pf_get_blk;
 *      VOID*                               pv_get_blk_tag;
 *      wgl_cci_release_group               pf_relase_grp;
 *      VOID*                               pv_relase_grp_tag;
 *      wgl_cci_get_drcs_pattern            pf_get_drcs;
 *      VOID*                               pv_get_drcs_tag;        
 *      wgl_cci_destroy_finished            pf_destroy_finished;
 *      VOID*                               pv_destroy_finished_tag;
 *  }   WGL_CCI_INIT_T; 
 *  @endcode
 *  @li@c  pf_get_blk                                        - The way of getting block
 *  @li@c  pv_get_blk_tag                                    - getting block private data
 *  @li@c  pf_relase_grp                                     - The way of releasing block
 *  @li@c  pv_relase_grp_tag                                 - releasing block private data
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_CCI_INIT_T
{
    wgl_cci_get_grp_blocks              pf_get_blk;
    VOID*                               pv_get_blk_tag;
    wgl_cci_release_group               pf_relase_grp;
    VOID*                               pv_relase_grp_tag;
    wgl_cci_get_drcs_pattern            pf_get_drcs;
    VOID*                               pv_get_drcs_tag;               
    wgl_cci_destroy_finished            pf_destroy_finished;
    VOID*                               pv_destroy_finished_tag;
}   WGL_CCI_INIT_T; 


/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/


#endif /* _X_WGL_CC_ISDB_TYPE_H_ */


/*----------------------------------------------------------------------------*/
/*! @} */                                /* end of groupMW_WGL_CCI */
/*----------------------------------------------------------------------------*/


