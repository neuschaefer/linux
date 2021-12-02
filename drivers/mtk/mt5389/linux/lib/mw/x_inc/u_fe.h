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
/*! @file u_fe.h
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *         This header file contains font library specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_FONT Font Engine
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of the Font Engine
 *   .. Font Engine is a middleware component that is responsible for render character from font file
 *
 *  %MW Application API would be used by TV Application, lot of these APIs
 *  would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_FE_H_
#define _U_FE_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_os.h"
#include "u_handle.h"
#include "u_handle_grp.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* handle type of font object */
typedef HANDLE_T                    HFONT_T;    /**<Font Handle.       */
/* character code */
typedef UINT32                      CHAR_CODE;    /**<UTF16.       */


/* handle */
#define FE_FNT_OBJ                  (HT_GROUP_FONT + ((HANDLE_TYPE_T) 1))    /**<Handle.        */


/* return values */
#define FER_OK                      ((INT32)    0)    /**<  Ok.      */
#define FER_INV_HANDLE              ((INT32) -  1)    /**<  Font Handle is invalid. */
#define FER_INV_ARG                 ((INT32) -  2)    /**<  Argument is invalid.     */
#define FER_NOT_INIT                ((INT32) -  3)    /**<  Font Engine not init.      */
#define FER_NO_MEMORY               ((INT32) -  4)    /**<  Not enough memory.      */
#define FER_OUT_OF_HANDLES          ((INT32) -  5)    /**<  Out of handle.      */
#define FER_INV_FONT                ((INT32) -  6)    /**<  Font file is invalid.      */
#define FER_NO_FONT_FILE            ((INT32) -  7)    /**<  Font file is not exist.      */
#define FER_NO_FONT_STYLE           ((INT32) -  8)    /**<  Font style is not invalid.  */
#define FER_NO_FONT_CMAP            ((INT32) -  9)    /**<  Font character map is not invalid. */
#define FER_NO_FONT_SIZE            ((INT32) - 10)    /**<  Font size is not init.      */
#define FER_NO_INFO                 ((INT32) - 11)    /**<  Unkown Font error.       */
#define FER_NO_CHAR_CODE            ((INT32) - 12)    /**<  Unicode is not exist in this font.      */
#define FER_INTERNAL_ERR            ((INT32) - 13)    /**<  Font Engine internal error.      */
#define FER_FONT_EXIST              ((INT32) - 14)    /**<  Font file is exist in system.     */
#define FER_CLI_ERROR               ((INT32) - 15)    /**<  Cli commnd error.      */
#define FER_WOULD_BLOCK             ((INT32) - 16)    /**<  Font engine is blocked. */
#define FER_FONT_IN_USE             ((INT32) - 17)    /**<  Font file is in use.      */
#define FER_NO_FONT                 ((INT32) - 18)    /**<  No font file.      */
#define FER_CACHE_FULL              ((INT32) - 19)    /**<  Glyph Cache is full.      */
#define FER_ALREADY_INIT            ((INT32) -101)    /**<  Font Engine inited. */
#define FER_INIT_FAILED             ((INT32) -102)    /**<  Font Engine init failed. */

#define FE_CC_LEADING_CHAR          (0xEA00)    /**<   Leading character for close caption.     */

#define FE_UTF16_SPACE_USER_START   (0xEA01)    /**<   Space repleaced these character for close caption.     */
#define FE_UTF16_SPACE_USER_END     (0xEAF0)    /**<   Space repleaced these character for close caption.     */

#define FE_IS_CHAR_NO_UNDERLINE(_w2_char)   \
            ((_w2_char) >= 0xEA00 &&        \
             (_w2_char) <= 0xEA7F )    /**<  Character in this rang have no underline.      */

#define FE_DEFAULT_UNDERLINE_THICKNESS  (-1)    /**< Underline thickness.      */

/* font size */
/*------------------------------------------------------------------*/
/*! @enum FE_FNT_SIZE
 *  @brief This structure enumerates font size.
 *  @code
 *  typedef enum _FE_FNT_SIZE
 *  {
 *
 *        FE_FNT_SIZE_SMALL
 *        FE_FNT_SIZE_MEDIUM
 *        FE_FNT_SIZE_LARGE
 *        FE_FNT_SIZE_CUSTOM
 *        FE_FNT_SIZE_NUMBER
 *  } FE_FNT_SIZE;
 *  @endcode
 *  @li@c  FE_FNT_SIZE_SMALL                                 - Selects the Small size font.
 *  @li@c  FE_FNT_SIZE_MEDIUM                                - Selects the Medium size font.
 *  @li@c  FE_FNT_SIZE_LARGE                                 - Selects the Large size font.
 *  @li@c  FE_FNT_SIZE_CUSTOM                                -Selects the Custom size font.
 *  @li@c  FE_FNT_SIZE_NUMBER                                -Used to count size number.
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


/* font styles */
/*------------------------------------------------------------------*/
/*! @enum FE_FNT_STYLE
 *  @brief This structure enumerates font style.
 *  @code
 *  typedef enum _FE_FNT_STYLE
 *  {
 *  #ifdef FE_DISABLE_EDGE_EFFECT_SUPPORT
 *      FE_FNT_STYLE_REGULAR        =   MAKE_BIT_MASK_8 (0),
 *      FE_FNT_STYLE_ITALIC         =   MAKE_BIT_MASK_8 (1),
 *      FE_FNT_STYLE_BOLD           =   MAKE_BIT_MASK_8 (2),
 *      FE_FNT_STYLE_UNDERLINE      =   MAKE_BIT_MASK_8 (3),
 *      FE_FNT_STYLE_STRIKEOUT      =   MAKE_BIT_MASK_8 (4),
 *      FE_FNT_STYLE_OUTLINE        =   MAKE_BIT_MASK_8 (5)
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
 *  @li@c  FE_FNT_STYLE_REGULAR                              - Selects the regular font, I.E,
                                                                                 not italic, bold, underline and strikeout.
 *  @li@c  FE_FNT_STYLE_ITALIC                                 - Set the font to be italic.
 *  @li@c  FE_FNT_STYLE_BOLD                                   - Set the font to be bold.
 *  @li@c  FE_FNT_STYLE_UNDERLINE                            -Set the font to have underline.
 *  @li@c  FE_FNT_STYLE_STRIKEOUT                            -Set the font to have strikeout line.
 *  @li@c  FE_FNT_STYLE_OUTLINE                              -Set the font to show only outline.
 *  @li@c  FE_FNT_STYLE_REGULAR                              -Selects the regular font, I.E,
                                                                                 not italic, bold, underline and strikeout.
 *  @li@c  FE_FNT_STYLE_ITALIC                               -Set the font to be italic.
 *  @li@c  FE_FNT_STYLE_BOLD                                 -Set the font to be bold.
 *  @li@c  FE_FNT_STYLE_UNDERLINE                            -Set the font to have underline.
 *  @li@c  FE_FNT_STYLE_STRIKEOUT                            -Set the font to have strikeout line.
 *  @li@c  FE_FNT_STYLE_OUTLINE                              -Set the font to show only outline.
 *  @li@c  FE_FNT_STYLE_SHADOW_RIGHT                         -Set the font to show shadow on right side.
 *  @li@c  FE_FNT_STYLE_SHADOW_LEFT                          -Set the font to show shadow on left side.
 *  @li@c  FE_FNT_STYLE_DEPRESSED                            -Set the font to show depressed effect.
 *  @li@c  FE_FNT_STYLE_RAISED                               -Set the font to show raised effect.
 *  @li@c  FE_FNT_STYLE_UNIFORM                              -Set the font to show uniform effect.
 *  @li@c  FE_FNT_STYLE_BLURRED                              -Set the font to show blurred effect.
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
    FE_FNT_STYLE_OUTLINE        =   MAKE_BIT_MASK_8 (5)
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

#ifndef FE_DISABLE_EDGE_EFFECT_SUPPORT
#define FE_FNT_STYLE_EDGE_EFFECT \
                    ( FE_FNT_STYLE_SHADOW_RIGHT | \
                    FE_FNT_STYLE_SHADOW_LEFT | \
                    FE_FNT_STYLE_DEPRESSED | \
                    FE_FNT_STYLE_RAISED | \
                    FE_FNT_STYLE_UNIFORM| \
                    FE_FNT_STYLE_BLURRED)    /**<Judge font if support edge effect or not. */
#endif


/*------------------------------------------------------------------*/
/*! @enum FE_OPTION_T
 *  @brief This structure enumerates font operater option.
 *  @code
 *  typedef enum
 *  {
 *      X_FE_OPTION_WAIT = 1,
 *      X_FE_OPTION_NOWAIT
 *  }   FE_OPTION_T;
 *  @endcode
 *  @li@c  X_FE_OPTION_WAIT                                  - Lock option wait.
 *  @li@c  X_FE_OPTION_NOWAIT                                -Lock option no wait.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    X_FE_OPTION_WAIT = 1,
    X_FE_OPTION_NOWAIT
}   FE_OPTION_T;

/*------------------------------------------------------------------*/
/*! @enum FE_REMOVE_OPTION_T
 *  @brief This structure enumerates font remove option.
 *  @code
 *  typedef enum
 *  {
 *      X_FE_OPTION_NORMAL = 1,
 *      X_FE_OPTION_FORCE
 *  }   FE_REMOVE_OPTION_T;
 *  @endcode
 *  @li@c  X_FE_OPTION_NORMAL                                -Remove font resource not force mode.
 *  @li@c  X_FE_OPTION_FORCE                                 -Remove font resource force mode.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    X_FE_OPTION_NORMAL = 1,
    X_FE_OPTION_FORCE
}   FE_REMOVE_OPTION_T;

/*------------------------------------------------------------------*/
/*! @enum FE_ROUNDING_TYPE_T
 *  @brief This structure enumerates rounding type.
 *  @code
 *  typedef enum
 *  {
 *      FE_RND_CEILING,
 *      FE_RND_MIDDLE,
 *      FE_RND_FLOOR
 *  } FE_ROUNDING_TYPE_T;
 *  @endcode
 *  @li@c  FE_RND_CEILING                                    -When calculate pixel size use ceiling mode.
 *  @li@c  FE_RND_MIDDLE                                     -When calculate pixel size use middle mode.
 *  @li@c  FE_RND_FLOOR                                      -When calculate pixel size use floor mode.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    FE_RND_CEILING,
    FE_RND_MIDDLE,
    FE_RND_FLOOR
} FE_ROUNDING_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum FE_ENCODING_TYPE_T
 *  @brief This structure enumerates encoding mode.
 *  @code
 *  typedef enum
 *  {
 *      FE_ENCODING_ASCII     = 0x00,
 *      FE_ENCODING_UTF8      = 0x01,
 *      FE_ENCODING_UTF16     = 0x02,
 *      FE_ENCODING_SHIFT_JIS = 0x03,
 *      FE_ENCODING_EUC_KR    = 0x04,
 *      FE_ENCODING_GB18030   = 0x05,
 *      FE_ENCODING_GB2312    = 0x06,
 *      FE_ENCODING_BIG5      = 0x07,
 *      FE_ENCODING_UCS2      = 0x08
 *  } FE_ENCODING_TYPE_T;
 *  @endcode
 *  @li@c  FE_ENCODING_ASCII                                 - ASCII encodeing mode.
 *  @li@c  FE_ENCODING_UTF8                                  -UTF8 encodeing mode.
 *  @li@c  FE_ENCODING_UTF16                                 -UTF16 encodeing mode.
 *  @li@c  FE_ENCODING_SHIFT_JIS                             -SHIFT_JIS encodeing mode.
 *  @li@c  FE_ENCODING_EUC_KR                                -EUC_KR encodeing mode.
 *  @li@c  FE_ENCODING_GB18030                               -GB18030 encodeing mode.
 *  @li@c  FE_ENCODING_GB2312                                -GB2312 encodeing mode.
 *  @li@c  FE_ENCODING_BIG5                                  -BIG5  encodeing mode.
 *  @li@c  FE_ENCODING_UCS2                                  -UCS2  encodeing mode.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    FE_ENCODING_ASCII     = 0x00,
    FE_ENCODING_UTF8      = 0x01,
    FE_ENCODING_UTF16     = 0x02,
    FE_ENCODING_SHIFT_JIS = 0x03,
    FE_ENCODING_EUC_KR    = 0x04,
    FE_ENCODING_GB18030   = 0x05,
    FE_ENCODING_GB2312    = 0x06,
    FE_ENCODING_BIG5      = 0x07,
    FE_ENCODING_UCS2      = 0x08
} FE_ENCODING_TYPE_T;

#define FE_ENC_TAG(value, a, b, c, d)       \
            value = (((UINT32)(a) << 24) |  \
                     ((UINT32)(b) << 16) |  \
                     ((UINT32)(c) <<  8) |  \
                      (UINT32)(d)       )    /**<Get font encoding tag.        */

/* Font character mapping */
/*------------------------------------------------------------------*/
/*! @enum FE_CMAP_ENCODING
 *  @brief This structure enumerates character map.
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
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_NONE                       -The encoding value 0 is reserved.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_MS_SYMBOL                  -Corresponds to the Unicode character set.
                                                                                            This value covers all versions of the
                                                                                            Unicode repertoire, including ASCII and
                                                                                            Latin-1. Most fonts include a Unicode charmap,
                                                                                            but not all of them.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_UNICODE                    -Corresponds to the Microsoft Symbol encoding,
                                                                                        used to encode mathematical symbols in the 32..255
                                                                                        character code range.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_SJIS                       -   Corresponds to Japanese SJIS encoding.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_GB2312                     - Corresponds to an encoding system for Simplified
                                                                                        Chinese as used used in mainland China.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_BIG5                       - Corresponds to an encoding system for Traditional
                                                                                        Chinese as used in Taiwan and Hong Kong.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_WANSUNG                    -Corresponds to the Korean encoding system known as Wansung.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_JOHAB                      -The Korean standard character set (KS C-5601-1992),
                                                                                        which corresponds to MS Windows code page 1361.
                                                                                        This character set includes all possible Hangeul character
                                                                                        combinations.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_ADOBE_STANDARD             -Corresponds to the Adobe Standard encoding,
                                                                                            as found in Type 1, CFF, and OpenType/CFF fonts.
                                                                                            It is limited to 256 character codes.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_ADOBE_EXPERT               -Corresponds to the Adobe Expert encoding, as found in Type 1,
                                                                                            CFF, and OpenType/CFF fonts. It is limited to 256 character codes.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_ADOBE_CUSTOM               -Corresponds to a custom encoding, as found in Type 1,
                                                                                            CFF, and OpenType/CFF fonts. It is limited to 256 character codes.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_ADOBE_LATIN_1              -Corresponds to a Latin-1 encoding as defined in a Type 1 Postscript font.
                                                                                            It is limited to 256 character codes.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_OLD_LATIN_2                -This value is deprecated and was never used nor reported by
                                                                                            FreeType. Don't use or test for it.
 *  @li@c  FE_ENC_TAG(FE_CMAP_ENC_APPLE_ROMAN                -Corresponds to the 8-bit Apple roman encoding.
                                                                                            Many TrueType and OpenType fonts contain a charmap for this encoding,
                                                                                            since older versions of Mac OS are able to use it.
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

/*------------------------------------------------------------------*/
/*! @struct FE_INIT_DATA
 *  @brief This structure enumerates memory font init data.
 *  @code
 *  typedef struct _FE_INIT_DATA
 *  {
 *      UINT8*      pui1_fontdata;
 *      UINT32      ui4_size;
 *      UINT8       ui1_small;
 *      UINT8       ui1_medium;
 *      UINT8       ui1_large;
 *  } FE_INIT_DATA;
 *  @endcode
 *  @li@c  pui1_fontdata                                     -Font memory data generate by font convert tool.
 *  @li@c  ui4_size                                          -Size of the font data in bytes.
 *  @li@c  ui1_small                                         -Small size of this font.
 *  @li@c  ui1_medium                                        -Medium size of this font.
 *  @li@c  ui1_large                                         -Large size of this font.
 */
/*------------------------------------------------------------------*/
typedef struct _FE_INIT_DATA
{
    UINT8*      pui1_fontdata;  /* font data                     */
    UINT32      ui4_size;       /* size of font data             */
    UINT8       ui1_small;      /* size of FE_FNT_SIZE_SMALL     */
    UINT8       ui1_medium;     /* size for FE_FNT_SIZE_MEDIUM   */
    UINT8       ui1_large;      /* size for FE_FNT_SIZE_LARGE    */
} FE_INIT_DATA;

/*------------------------------------------------------------------*/
/*! @struct FE_INIT_FILE
 *  @brief This structure enumerates font file init data.
 *  @code
 *  typedef struct _FE_INIT_FILE
 *  {
 *      CHAR*       ps_file;
 *      UINT8       ui1_small;
 *      UINT8       ui1_medium;
 *      UINT8       ui1_large;
 *  } FE_INIT_FILE;
 *  @endcode
 *  @li@c  ps_file                                           -Font file absolute path.
 *  @li@c  ui1_small                                         -Small size of this font.
 *  @li@c  ui1_medium                                        -Medium size of this font.
 *  @li@c  ui1_large                                         - Large size of this font.
 */
/*------------------------------------------------------------------*/
typedef struct _FE_INIT_FILE
{
    CHAR*       ps_file;        /* file absolut path             */
    UINT8       ui1_small;      /* size of FE_FNT_SIZE_SMALL     */
    UINT8       ui1_medium;     /* size for FE_FNT_SIZE_MEDIUM   */
    UINT8       ui1_large;      /* size for FE_FNT_SIZE_LARGE    */
} FE_INIT_FILE;

/*------------------------------------------------------------------*/
/*! @enum FE_RENDER_MODE_T
 *  @brief This structure enumerates font render mode.
 *  @code
 *  typedef enum
 *  {
 *      FE_RENDER_MODE_NORMAL = 0,
 *      FE_RENDER_MODE_MONO
 *  } FE_RENDER_MODE_T;
 *  @endcode
 *  @li@c  FE_RENDER_MODE_NORMAL                -8-bit anti-aliased bitmaps, using 256 levels of opacity
 *  @li@c  FE_RENDER_MODE_MONO                                         -render a 1-bit monochrome glyph bitmap
 */
/*------------------------------------------------------------------*/

typedef enum
{
    FE_RENDER_MODE_NORMAL = 0,
    FE_RENDER_MODE_MONO
} FE_RENDER_MODE_T;

/* font resource type */
typedef enum
{
    FE_FNT_RSC_TYPE_FILE,
    FE_FNT_RSC_TYPE_MEM
} FE_FNT_RSC_TYPE_T;

#endif  /* _U_FE_H */

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_FONT*/
/*----------------------------------------------------------------------------*/


/*!@page pageMW_Font Font Engine Detailed Description
  *@par
  *@section secMW_Font_1 1.Introduction
  *@par
  *     The task of Font Engine is to offer the functions to show the characters normally on OSD and widget. It calls FreeType library interface
  *to load font file and get char bitmap.

  *@par
  *@section secMW_Font_2 2.Concept
  *
  *@par
  *@subsection ssecMW_Font_2_1 2.1.Font
  *@par
  *     Font is a set of characters, including letters, numbers and symbols and so on. It has steted height, with points or other device-independent
  *units to measure. It contains a collection of various character images that can be used to display or print text.
  *@par
  *     One has to distinguish between a <b>font family</b> and its multiple <b>font faces</b>, which usually differ in style though come from the
  *same template.
  *@par
  *     @image html Font_Face_Family.gif "Figure 1: Font Family and Font Face"
  *
  *@par
  *@subsection ssecMW_Font_2_2 2.2.Font File
  *@par
  *     Font file contains character images, character metrics, as well as other kind of information important to the layout of text and the processing
  *of specific character encodings.
  *@par
  *     One font file can contain several font faces, but for True Type fon file, it is common that one file contains one face.
  *
  *@par
  *@subsection ssecMW_Font_2_3 2.3.Glyphs and Charmap
  *@par
  *     The character images are called <b>glyphs</b>. A single charecter can have several distinct images. Several characters can also take a single
  *glyph.
  *@par
  *     The font file contains one or more tables, called a <b>character map</b>(Charmap), which is used to convert character codes for a given
  *encoding(e.g. ASCII, Unicode, DBCS, Big5) into glyph.
  *
  *@par
  *@section secMW_Font_3 3.Sample Code
  *     @dontinclude font_engine_samplecode.c
  *@par
  *@subsection ssecMW_Font_3_1 3.1.c_fe_add_file_font
  *@par
  *     Application could call this function to add a font into system. When this function is called, font engine will read and validate the specified font
  *file.
  *     @skip c_fe_add_file_font
  *     @skip {
  *     @until }
  *@par
  *@subsection ssecMW_Font_3_2 3.2.c_fe_add_mem_font
  *     @skip c_fe_add_mem_font
  *     @skip {
  *     @until }
  *@subsection ssecMW_Font_3_3 3.3.c_fe_add_combine_bmp_font
  *     @skip c_fe_add_combine_bmp_font
  *     @skip {
  *     @until }
  *@subsection ssecMW_Font_3_4 3.4.c_fe_remove_font
  *     @skip c_fe_remove_font
  *     @skip {
  *     @until }
  *@subsection ssecMW_Font_3_5 3.5.c_fe_has_font
  *     @skip c_fe_has_font
  *     @skip {
  *     @until }
  *@subsection ssecMW_Font_3_6 3.6.c_fe_create_font
  *     @skip c_fe_create_font
  *     @skip {
  *     @until }
  *@subsection ssecMW_Font_3_7 3.7.c_fe_get_string_size
  *     @skip c_fe_get_string_size
  *     @skip {
  *     @until }
  *@subsection ssecMW_Font_3_8 3.8.c_fe_set_unrecognizable_char
  *     @skip c_fe_set_unrecognizable_char
  *     @skip {
  *     @until }
  *@subsection ssecMW_Font_3_9 3.9.c_fe_preload_char
  *     @skip c_fe_preload_char
  *     @skip {
  *     @until }
  *
  *
  *
  *
  */

