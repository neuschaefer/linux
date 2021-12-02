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
 * $RCSfile: x_fe.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/17 $
 * $SWAuthor: Ben Tsai $
 * $MD5HEX: a6706234253e2900389cd727489cdfef $
 *
 * Description:
 *         This header file contains font library specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/
#ifndef _X_FE_H_
#define _X_FE_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_fe.h"
#include "u_gl.h"


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* Pixel Mode of FE_BITMAP_T's buffer.
 * Mono mode: each pixel is described as 1-bit.
 * Gray mode: each pixel is described as 1-byte, i.e, 256-level gray */
typedef enum
{
    FE_PIXEL_MODE_MONO = 1,
    FE_PIXEL_MODE_GRAY = 2
} FE_PIXEL_MODE_T;

/* Bitmap of a 'char code'
 * For PIXEL_MODE_GRAY, i4_width is equal to i4_pitch.
 * For PIXEL_MODE_MONO, i4_pitch is euqal to ceil(i4_width/8) */
typedef struct _FE_BITMAP_T
{
    INT32           i4_height;              /* number of bitmap rows.        */
    INT32           i4_width;               /* number of pixels per row.     */
    INT32           i4_pitch;               /* number of bytes per row       */
    FE_PIXEL_MODE_T e_mode;                 /* Mono or Gray                  */
    UINT8*          pui1_buf;               /* pointer to the bitmap buffer. */
} FE_BITMAP_T;

typedef struct _FE_VECTOR_T
{
    INT32           i4_x;
    INT32           i4_y;
} FE_VECTOR_T;

/* Glyph of a 'char code' */
typedef struct _FE_GLYPH_T
{
    INT32               i4_left;                /* left location. => Bearing X   */
    INT32               i4_top;                 /* top location.  => Bearing Y   */
    FE_BITMAP_T         t_bmp;
    FE_RENDER_MODE_T    e_render_mode;
    FE_VECTOR_T         t_advance;
    BOOL                b_from_cache;
} FE_GLYPH_T;


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/**
    system startup/release functions
 **/

/*-----------------------------------------------------------------------------
 * Name:  fe_init
 *
 * Description: This function initializes the Font Engine and must be called
 *              before invocation of any font-related APIs.
 *
 * Inputs:  ui4_dpi_hori        Horizontal resolution (dpi) of display.
 *          ui4_dpi_vert        Vertical resolution (dpi) of display.
 *
 * Outputs: -
 *
 * Returns: FER_OK              Font Engine is successfully initialized.
 *          FER_INIT_FAILED     The Font Engine initialization failed.
 *          FER_ALREADY_INIT    Font Engine is already initialized.
 ----------------------------------------------------------------------------*/
INT32 fe_init(
    UINT32                  ui4_dpi_hori,   /* horizontal resolution in dpi  */
    UINT32                  ui4_dpi_vert);  /* vertical   resolution in dpi  */
                                            /* To do: enable cache manager   */


/*-----------------------------------------------------------------------------
 * Name:  fe_set_resolution
 *
 * Description: This function changes the resolution (dpi).
 *
 * Inputs:  ui4_dpi_hori        Horizontal resolution (dpi) of display.
 *          ui4_dpi_vert        Vertical resolution (dpi) of display.
 *
 * Outputs: -
 *
 * Returns: FER_OK              Font Engine is successfully initialized.
 ----------------------------------------------------------------------------*/
INT32 fe_set_resolution(
    UINT32                  ui4_dpi_hori,   /* horizontal resolution in dpi  */
    UINT32                  ui4_dpi_vert);  /* vertical   resolution in dpi  */


/*-----------------------------------------------------------------------------
 * Name:  fe_done
 *
 * Description: This function terminate the Font Engine.
 *              Only call this when no more any font requirements.
 *              Note: In normal case, this will *NOT* be called forever!
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fe_done(
    VOID);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_add_font
 *
 * Description: In system initialization, integrator should call this function
 *              to add a font into system. There must be at least one font in
 *              whole system to display text. When this function is called,
 *              font engine will read and validate the specified font file.
 *              Clients should also specify three font sizes
 *              (Small / Medium / Large) for this font. For vector font,
 *              the font size is assumed to be 'Point Size', and for bitmap
 *              font, 'Pixel Size' instead.  If the specified font size inside
 *              a vector font is an embedded bitmap, the size is turned to be
 *              'Pixel Size' also.
 *
 * Inputs:  ps_font             Specify the Font name. User can define the font
 *                              name. To use the real font family name is not
 *                              necessary.
 *          ps_file             Specify the real font file name.
 *          ui1_small           Font size for 'Small Font'.
 *          ui1_medium          Font size for 'Medium Font'.
 *          ui1_large           Font size for 'Large Font'.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_NO_FONT_FILE    The ps_file font file doesn't exist.
 *          FER_INV_FONT        The ps_file file is not a valid font file.
 *          FER_NO_FONT_SIZE    Some specified font sizes are not available.
 *          FER_NO_MEMORY       Out of system memory.
 ----------------------------------------------------------------------------*/
INT32 x_fe_add_file_font(
    const CHAR*             ps_font,        /* font name                     */
    const CHAR*             ps_file,        /* font file name                */
    UINT8                   ui1_small,      /* size of FE_FNT_SIZE_SMALL     */
    UINT8                   ui1_medium,     /* size for FE_FNT_SIZE_MEDIUM   */
    UINT8                   ui1_large);     /* size for FE_FNT_SIZE_LARGE    */


/*-----------------------------------------------------------------------------
 * Name:  x_fe_add_mem_font
 *
 * Description: In system initialization, integrator should call this function
 *              to add a font into system. There must be at least one font in
 *              whole system to display text. When this function is called,
 *              font engine will read and validate the specified font file.
 *              Clients should also specify three font sizes
 *              (Small / Medium / Large) for this font. For vector font,
 *              the font size is assumed to be 'Point Size', and for bitmap
 *              font, 'Pixel Size' instead.  If the specified font size inside
 *              a vector font is an embedded bitmap, the size is turned to be
 *              'Pixel Size' also.
 *
 * Inputs:  ps_font             Specify the Font name. User can define the font
 *                              name. To use the real font family name is not
 *                              necessary.
 *          pui1_fontdata       Specify the font data.
 *          ui4_size            Size of font data.
 *          ui1_small           Font size for 'Small Font'.
 *          ui1_medium          Font size for 'Medium Font'.
 *          ui1_large           Font size for 'Large Font'.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_INV_FONT        The ps_file file is not a valid font file.
 *          FER_NO_FONT_SIZE    Some specified font sizes are not available.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_FONT_EXIST      The same font existed.
 ----------------------------------------------------------------------------*/
INT32 x_fe_add_mem_font(
    const CHAR*             ps_font,        /* font name                     */
    const UINT8*            pui1_fontdata,  /* font data                     */
    UINT32                  ui4_size,       /* size of font data             */
    UINT8                   ui1_small,      /* size of FE_FNT_SIZE_SMALL     */
    UINT8                   ui1_medium,     /* size for FE_FNT_SIZE_MEDIUM   */
    UINT8                   ui1_large);     /* size for FE_FNT_SIZE_LARGE    */


/*-----------------------------------------------------------------------------
 * Name:  x_fe_add_combine_bmp_mem_font
 *
 * Description: Client should specify three font files, of which each one must
 *              be a bitmap font of one and only one font size.
 *
 * Inputs:  ps_font             Specify the Font name. User can define the font
 *                              name. To use the real font family name is not
 *                              necessary.
 *          pui1_fontdata_small     Specify the data of small-sized font.
 *          ui4_size_small          Specify the data size of small-sized font.
 *          pui1_fontdata_medium    Specify the data of medium-sized font.
 *          ui4_size_medium         Specify the data size of medium-sized font.
 *          pui1_fontdata_large     Specify the data of large-sized font.
 *          ui4_size_large          Specify the data size of large-sized font.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_NO_FONT_FILE    The ps_file font file doesn't exist.
 *          FER_INV_FONT        The ps_file file is not a valid font file.
 *          FER_NO_FONT_SIZE    Some specified font sizes are not available.
 *          FER_NO_MEMORY       Out of system memory
 *          FER_FONT_EXIST      The same font existed.
 ----------------------------------------------------------------------------*/
INT32 x_fe_add_combine_bmp_mem_font(
    const CHAR*             ps_font,                /* font name                     */
    const UINT8*            pui1_fontdata_small,    /* data of small-sized font      */
    UINT32                  ui4_size_small,         /* data size of small-sized font */
    const UINT8*            pui1_fontdata_medium,   /* data of medium-sized font      */
    UINT32                  ui4_size_medium,        /* data size of medium-sized font */
    const UINT8*            pui1_fontdata_large,    /* data of large-sized font      */
    UINT32                  ui4_size_large);        /* data size of large-sized font */

INT32 x_fe_add_multiple_mem_font(
    const CHAR*             ps_font,
    FE_INIT_DATA            at_init_data[],
    UINT8                   ui1_data_count);

INT32 x_fe_change_font_resource(
    const CHAR              *ps_font,
    UINT8                   ui1_font_id);


 INT32 x_fe_add_multiple_file_font(
    const CHAR*             ps_font,
    FE_INIT_FILE            at_init_file[],
    UINT8                   ui1_file_count);

INT32 x_fe_get_cur_font_resource(
	HFONT_T h_fnt,
	UINT8  *pui1_cur_font_id);

/*-----------------------------------------------------------------------------
 * Name: x_fe_remove_font
 *
 * Description: This function removes a font from system. The user must free
 *              all the associated handles before calling this function
 *              unless the forece mode is specified.
 *
 * Inputs:  ps_font             Specify the Font name.
 *          e_option            Specify X_FE_OPTION_FORCE to force the removal
 *                              of specified font - the font engine will free
 *                              all the associated handles accordingly.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_FONT_IN_USE     One or more handle(s) are associated with this
 *                              font.
 *
 ----------------------------------------------------------------------------*/
INT32 x_fe_remove_font(
    const CHAR*             ps_font,        /* font name                     */
    FE_REMOVE_OPTION_T             e_option);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_dfl_font
 *
 * Description: This function will set the specified font as system default
 *              font. When calling x_fe_create_font without specify font name,
 *              the system default font will be used.
 *
 * Inputs:  ps_font             Font name to be used as system default font.
 *          e_style             System default font style.
 *                              (name,style) pair is used to specify a font.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_FONT        The font is not in system. The font should be
 *                              added into system by calling x_fe_add_font.
 *          FER_NO_FONT_STYLE   The font style is not supported by this font.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_dfl_font(
    const CHAR*             ps_font,        /* font name                     */
    FE_FNT_STYLE            e_style);       /* font style                    */

/**
    system functions (for querying ...)
 **/

/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_font
 *
 * Description: Applications can use this function to check if the font is in
 *              this system.
 *
 * Inputs:  ps_font             Contains the font name.
 *          e_style             Font style.
 *
 * Outputs: -
 *
 * Returns: TRUE                This system contains the font.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_font(
    const CHAR*             ps_font,
    FE_FNT_STYLE            e_style);


/**
    font handle functions (create/release)
 **/

/*-----------------------------------------------------------------------------
 * Name:  x_fe_create_font
 *
 * Description: This function creates a font object according to the specified
 *              font face, size, style and charmap.
 *
 * Inputs:  ps_font             Contains the font name.
 *                              If ps_font is NULL, use system default font,
 *                              and e_style parameter is ignored.
 *          e_size              Contains the font size. Can be
 *                              FE_FNT_SIZE_SMALL,
 *                              FE_FNT_SIZE_MEDIUM or
 *                              FE_FNT_SIZE_LARGE.
 *          e_style             Contains the font style, and can be bit-ored.
 *                              Style can be
 *                              FE_FNT_STYLE_REGULAR,
 *                              FE_FNT_STYLE_ITALIC,
 *                              FE_FNT_STYLE_BOLD,
 *                              FE_FNT_STYLE_UNDERLINE and
 *                              FE_FNT_STYLE_STRIKEOUT.
 *          e_cmap              Contains the character map encoding method.
 *                              Can be
 *                              FE_CMAP_ENC_MS_SYMBOL,
 *                              FE_CMAP_ENC_UNICODE,
 *                              FE_CMAP_ENC_SJIS,
 *                              FE_CMAP_ENC_GB2312,
 *                              FE_CMAP_ENC_BIG5,
 *                              FE_CMAP_ENC_WANSUNG,
 *                              FE_CMAP_ENC_JOHAB,
 *                              FE_CMAP_ENC_ADOBE_STANDARD,
 *                              FE_CMAP_ENC_ADOBE_EXPERT,
 *                              FE_CMAP_ENC_ADOBE_CUSTOM,
 *                              FE_CMAP_ENC_ADOBE_LATIN_1,
 *                              FE_CMAP_ENC_OLD_LATIN_2,
 *                              FE_CMAP_ENC_APPLE_ROMAN.
 *
 * Outputs: ph_fnt              References the handle of the new font object if
 *                              the routine was successful. In all other cases,
 *                              the value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_INV_FONT        The font is not in system.
 *          FER_NO_FONT_CMAP    The font character map is not supported.
 *          FER_NO_FONT_STYLE   The font style is not supported by this font.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_OUT_OF_HANDLES  There are no more handles available.
 ----------------------------------------------------------------------------*/
INT32 x_fe_create_font(
    const CHAR*             ps_font,        /* font name                     */
    FE_FNT_SIZE             e_size,         /* size of font                  */
    FE_FNT_STYLE            e_style,        /* italic/bold/underline/...     */
    FE_CMAP_ENCODING        e_cmap,         /* character map identifier      */
    HFONT_T*                ph_fnt);        /* return : font hadle           */


/**
    character map specific functions
 **/

/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_cmap
 *
 * Description: This function is used to check if the font contains the
 *              character map.
 *
 * Inputs:  ps_font             The font name to be checked.
 *          e_style             Font style.
 *          e_cmap              Character Map to be checked.
 *
 * Outputs: -
 *
 * Returns: TRUE                The character map is supported by this font.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_cmap(
    const CHAR*             ps_font,
    FE_FNT_STYLE            e_style,
    FE_CMAP_ENCODING        e_cmap);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_sel_cmap
 *
 * Description: This API selects the specified charmap for the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          e_cmap              Contains the character map encoding method.
 *                              Can be
 *                              FE_CMAP_ENC_MS_SYMBOL,
 *                              FE_CMAP_ENC_UNICODE,
 *                              FE_CMAP_ENC_SJIS,
 *                              FE_CMAP_ENC_GB2312,
 *                              FE_CMAP_ENC_BIG5,
 *                              FE_CMAP_ENC_WANSUNG,
 *                              FE_CMAP_ENC_JOHAB,
 *                              FE_CMAP_ENC_ADOBE_STANDARD,
 *                              FE_CMAP_ENC_ADOBE_EXPERT,
 *                              FE_CMAP_ENC_ADOBE_CUSTOM,
 *                              FE_CMAP_ENC_ADOBE_LATIN_1,
 *                              FE_CMAP_ENC_OLD_LATIN_2,
 *                              FE_CMAP_ENC_APPLE_ROMAN.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_FONT_CMAP    The charmap encoding is not supported.
 ----------------------------------------------------------------------------*/
INT32 x_fe_sel_cmap(
    HFONT_T                 h_fnt,
    FE_CMAP_ENCODING        e_cmap);        /* character map identifier      */



/**
    font attributes functions
 **/

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_size
 *
 * Description: This API sets the size of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          e_size              Contains the new font size. Can be
 *                              FE_FNT_SIZE_SMALL,
 *                              FE_FNT_SIZE_MEDIUM or
 *                              FE_FNT_SIZE_LARGE.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_size(
    HFONT_T                 h_fnt,
    FE_FNT_SIZE             e_size);        /* size of font                  */

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_custom_size
 *
 * Description: This API sets the custom size of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          ui1_custom_size     Contains the new custom font size.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_custom_size(
    HFONT_T                 h_fnt,
    UINT8                   ui1_custom_size);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_outline_width
 *
 * Description: This API sets the outline width of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          ui2_width           Contains the width of the outline
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_outline_width(
    HFONT_T                 h_fnt,
    UINT16                  ui2_width);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_rendering_offset
 *
 * Description: This function set the rendering offset of a font.
 *
 * Inputs:  ps_font             font name
 *          ui1_font_id         font ID
 *          i4_offset_small     offset for size SMALL
 *          i4_offset_medium    offset for size MEDIUM
 *          i4_offset_large     offset for size LARGE
 *
 * Outputs: -
 *
 * Returns: FER_OK              Font Engine is successfully initialized.
 *          FER_NO_FONT         No such font name or font ID is found.
 ----------------------------------------------------------------------------*/
extern INT32 x_fe_set_rendering_offset(
    const CHAR          *ps_font,
    UINT8               ui1_font_id,
    INT32               i4_offset_small,
    INT32               i4_offset_medium,
    INT32               i4_offset_large);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_style
 *
 * Description: This API sets the style of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          pt_style            References the style if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_style(
    HFONT_T                 h_fnt,
    FE_FNT_STYLE*           pt_style);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_uniform_width
 *
 * Description: This API sets the uniform width of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          ui2_width           Contains the width of the uniform
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_uniform_width(
    HFONT_T                 h_fnt,
    UINT16                  ui2_width);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_fixed_advance
 *
 * Description: This API sets the horizontal and vertical advance of the font
 *              object to fixed values; the glyph advance and kerning
 *              information contained in the font file would then be ignored.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          i2_hori             Contains the horizontal advance. A value of
 *                              zero indicates that the fixed horizontal
 *                              advance is canceled.
 *          i2_vert             Contains the vertical advance. A value of zero
 *                              indicates that the fixed vertical advance is
 *                              canceled.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_fixed_advance(
    HFONT_T                 h_fnt,
    INT16                   i2_hori,        /* advance width                 */
    INT16                   i2_vert);       /* advance height                */



/**
    font layout parameters functions
 **/

/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_vertical
 *
 * Description: This API determines if the font object has vertical glyph.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font has vertical glyph.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_vertical(
    HFONT_T                 h_fnt);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_is_scalable
 *
 * Description: This API determines if the font is scaleable, in other words,
 *              vectorial and outline fonts.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font is scaleable.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_is_scalable(
    HFONT_T                 h_fnt);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_is_fixed_width
 *
 * Description: This API determines if the font is fixed width or proportional
 *              width font.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font is fixed width font.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_is_fixed_width(
    HFONT_T                 h_fnt);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_kerning
 *
 * Description: This API determines if the font contains kerning information.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font contains kerning information.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_kerning(
    HFONT_T                 h_fnt);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_fixed_sizes
 *
 * Description: This API determines if the font contains fixed size bitmap.
 *              For non-scaleable fonts, this function will always return TRUE.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font contains fixed size bitmap.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_fixed_sizes(
    HFONT_T                 h_fnt);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_ascender
 *
 * Description: This API retrieves the ascent value of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_asc             References the ascent value if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_ascender(
    HFONT_T                 h_fnt,
    INT16*                  pi2_asc);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_descender
 *
 * Description: This API retrieves the descent value of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_desc            References the descent value if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_descender(
    HFONT_T                 h_fnt,
    INT16*                  pi2_desc);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_height
 *
 * Description: This API retrieves the height of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_height          References the height if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_height(
    HFONT_T                 h_fnt,
    INT16*                  pi2_height);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_name
 *
 * Description: This API retrieves the font name.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          i4_len              Specify the size of buffer allocated for the
 *                              name string.
 *
 * Outputs: ps_name             References the name if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The font is added successfully.
 *          FER_NOT_INIT        The Font Engine is not yet initialized.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_name(
    HFONT_T                 h_fnt,
    CHAR*                   ps_name,
    INT32                   i4_len);

INT32 x_fe_get_rendering_offset(
    HFONT_T                 h_font,
    INT32                   *pi4_offset);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_size
 *
 * Description: This API retrieves the size of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pe_size             References the font size if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_size(
    HFONT_T                 h_fnt,
    FE_FNT_SIZE*            pe_size);

INT32 x_fe_get_font_size(
    HFONT_T                 h_fnt,
    FE_FNT_SIZE*            pe_size,
    UINT8*                  pu1_size);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_style
 *
 * Description: This API retrieves the style of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pt_style            References the style if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_style(
    HFONT_T                 h_fnt,
    FE_FNT_STYLE*           pt_style);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_cmap
 *
 * Description: This API retrieves the character map encoding method of the font
 *              object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pe_cmap             References the character map encoding method
 *                              if the routine was successful. In all other
 *                              cases, the value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_ARG         One or more parameter(s) are invalid.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_cmap(
    HFONT_T                 h_fnt,
    FE_CMAP_ENCODING*       pe_cmap);        /* character map identifier      */


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_underline_position
 *
 * Description: This API retrieves the underline position of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_udl_pos         References the underline position if the
 *                              routine was successful. In all other cases,
 *                              the value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_underline_position(
    HFONT_T                 h_fnt,
    INT16*                  pi2_udl_pos);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_underline_thickness
 *
 * Description: This API retrieves the underline thickness of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_udl_thk         References the underline thickness if the
 *                              routine was successful. In all other cases,
 *                              the value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_underline_thickness(
    HFONT_T                 h_fnt,
    INT16*                  pi2_udl_thk);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_underline_thickness
 *
 * Description: This API sets the underline thickness of the font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          i2_udl_thk          References the underline thickness if the
 *                              routine was successful. In all other cases,
 *                              the value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_underline_thickness(
    HFONT_T                 h_fnt,
    INT16                   i2_udl_thk);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_max_advance
 *
 * Description: This API retrieves the max advance width and height of the
 *              font object.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi2_max_adv_w       References the max advance width if the routine
 *                              was successful. In all other cases, the value
 *                              of this argument is undetermined.
 *          pi2_max_adv_h       References the max advance height if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no this information.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_max_advance(
    HFONT_T                 h_fnt,
    INT16*                  pi2_max_adv_w,  /* maximum advance width         */
    INT16*                  pi2_max_adv_h); /* maximum advance height        */


/*-----------------------------------------------------------------------------
 * Name:  x_fe_has_char_code
 *
 * Description: This function is used to check if the font contains the
 *              character code.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          cc_char_code        The character code.
 *
 * Outputs: -
 *
 * Returns: TRUE                The font contains this character code.
 *          FALSE               Otherwise.
 ----------------------------------------------------------------------------*/
BOOL x_fe_has_char_code(
    HFONT_T                 h_fnt,
    CHAR_CODE               cc_char_code);


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_kerning
 *
 * Description: This API retrieves the kerning of the specified two character
 *              codes.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          cc_lchar_code       The left character code.
 *          cc_rchar_code       The right character code.
 *
 * Outputs: pi4_x               References the horizontal kerning distance if
 *                              the routine was successful. In all other cases,
 *                              the value of this argument is undetermined.
 *          pi4_y               References the vertical kerning distance if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_NO_INFO         There is no kerning information of this font or
 *                              of these two char code.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_NO_CHAR_CODE    Left or right char_code do not exist.
 *          FER_NO_FONT_CMAP    The font character map is not supported.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_kerning(
    HFONT_T                 h_fnt,
    CHAR_CODE               cc_lchar_code,  /* left char_code                */
    CHAR_CODE               cc_rchar_code,  /* right char_code               */
    INT32*                  pi4_x,          /* return: horizontal kerning    */
    INT32*                  pi4_y);         /* return: vertical kerning      */



/**
    glyph-specific metrics
 **/

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_glyph_advance
 *
 * Description: This API retrieves the glyph advance of the specified character
 *              code.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          cc_char_code        The character code to be refered.
 *
 * Outputs: pi4_hori            References the horizontal glyph advance if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *          pi4_vert            References the vertical glyph advance if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_INV_ARG         The specified char code is invalid.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_NO_CHAR_CODE    Left or right char_code do not exist.
 *          FER_NO_FONT_CMAP    The font character map is not supported.
 *          FER_NO_INFO         Fail to load glyph info.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_glyph_advance(
    HFONT_T                 h_fnt,
    CHAR_CODE               cc_char_code,
    INT32*                  pi4_hori,       /* advance width                 */
    INT32*                  pi4_vert);      /* advance height                */

INT32 x_fe_get_glyph_advance_cc(
       HFONT_T                 h_fnt,
       CHAR_CODE               cc_char_code,
       INT32*                  pi4_hori,       /* advance for horizontal layout */
       INT32*                  pi4_vert);      /* advance for vertical   layout */


/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_character_size
 *
 * Description: This API retrieves the glyph size of the specified character
 *              code.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          cc_char_code        The character code to be refered.
 *
 * Outputs: pi4_width            References the glyph width.
 *              pi4_height           References the glyph height.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_INV_ARG         The specified char code is invalid.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_NO_CHAR_CODE    Left or right char_code do not exist.
 *          FER_NO_FONT_CMAP    The font character map is not supported.
 *          FER_NO_INFO         Fail to load glyph info.
 ----------------------------------------------------------------------------*/

INT32 x_fe_get_character_size(
    HFONT_T                 h_fnt,
    CHAR_CODE               cc_char_code,
    INT32*                  pi4_width,       /* advance for width layout */
    INT32*                  pi4_height);       /* advance for height   layout */

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_string_size
 *
 * Description: This API calculates the pixel size of the string
 *              (when displayed on screen).
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          pv_str              The string to be evaluated.
 *          i4_length           Maximum length to be evaluated.
 *                              If <0, use whole pv_str. (NULL-terminated)
 *
 * Outputs: pi2_width           References the width of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *          pi2_height          References the height of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_INV_ARG         One or more characters in the string is invalid
 *                              or not in font.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_string_size(
    HFONT_T                 h_fnt,
    const VOID*             pv_str,         /* string to be evaluated        */
    INT32                   i4_length,      /* max length to be evaluated    */
    INT32*                  pi4_width,      /* width of string               */
    INT32*                  pi4_height);    /* height of string              */

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_top_2_down_string_size
 *
 * Description: This API calculates the pixel size of the string
 *              (when displayed on screen) drawn vertically.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          pv_str              The string to be evaluated.
 *          i4_length           Maximum length to be evaluated.
 *                              If <0, use whole pv_str. (NULL-terminated)
 *          aui2_ch_orient_idx  id array for laying-down characters
 *          ui2_ch_orient_num   size of the id array
 *          e_ch_orientation    laying-down orientation
 *
 * Outputs: pi2_width           References the width of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *          pi2_height          References the height of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_INV_ARG         One or more characters in the string is invalid
 *                              or not in font.
 *          FER_NO_MEMORY       Out of system memory.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_top_2_down_string_size(
    HFONT_T                 h_fnt,
    const VOID*             pv_str,              /* string to be evaluated              */
    INT32                   i4_length,           /* max length to be evaluated          */
    UINT16                  *aui2_ch_orient_idx, /* id array for laying-down characters */
    UINT16                  ui2_ch_orient_num,   /* size of the id array                */
    GL_CHAR_ORIENT_T        e_ch_orientation,    /* laying-down orientation             */
    INT32*                  pi4_width,           /* width of string                     */
    INT32*                  pi4_height);         /* height of string                    */

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_add_width_by_synthesis
 *
 * Description: This API retrieves the width increased after synthesis
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: pi4_width           References to the width.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_ARG         Invalid arguments.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_add_width_by_synthesis(
    HFONT_T                 h_font,
    INT32                   *pi4_width);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_load_char
 *
 * Description: This API retrieves the bitmap of the specified char code.
 *              Note: The pui1_buf of t_bmp which inside pt_glyph is allocated
 *              and maintained by font engine itself. Client need to allocate
 *              FE_GLYPH_T and free it, but not the pui1_buf pointer.
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          cc_char_code        The character code to be loaded.
 *
 * Outputs: pt_glyph            References the bitmap if the routine was
 *                              successful. In all other cases, the value of
 *                              this argument is undetermined.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_NOT_INIT        The Font Engine is not initialized yet.
 *          FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *          FER_INV_ARG         The char code is invalid.
 *          FER_NO_MEMORY       Out of system memory.
 *          FER_NO_CHAR_CODE    Left or right char_code do not exist.
 *          FER_NO_FONT_CMAP    The font character map is not supported.
 ----------------------------------------------------------------------------*/
INT32 x_fe_load_char(
    HFONT_T                 h_fnt,
    CHAR_CODE               cc_char_code,
    FE_GLYPH_T*             pt_glyph);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_rounding_type
 *
 * Description: This API sets the rounding criteria of this font
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          e_rnd_type          The desired rounding type.
 *
 * Outputs:
 *
 * Returns: FER_OK              Success.
 *          FER_NOT_INIT        Font Engine is not initialized.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_rounding_type(
    HFONT_T                 h_fnt,
    FE_ROUNDING_TYPE_T      e_rnd_type);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_rounding_type
 *
 * Description: This API gets the rounding criteria of this font
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *
 * Outputs: e_rnd_type          The desired rounding type.
 *
 * Returns: FER_OK              Success.
 *          FER_NOT_INIT        Font Engine is not initialized.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_rounding_type(
    HFONT_T                 h_fnt,
    FE_ROUNDING_TYPE_T      *pe_rnd_type);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_char_range
 *
 * Description: This function gets the character code range of the current character amp.
 *
 * Inputs:  h_fnt               Font handle.
 *          at_char_code[2]     The array to store the character code range.
 *
 * Outputs: b_available         If this is TRUE, the range is specified in at_char_code[].
 *                              Otherwise, this information is not available.
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 *          FER_INV_ARG         Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_char_range(
    HFONT_T     h_fnt,
    CHAR_CODE   at_char_code[2],
    BOOL        *pb_available);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_delete_font
 *
 * Description: This function deletes a font object.
 *
 * Inputs:  h_fnt               Font handle.
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_delete_font(
    HFONT_T     h_fnt);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_strlen
 *
 * Description: This function retrieves the length of a string with a specific
 *              encoding.
 *
 * Inputs:  pui1_str            Pointer to the string.
 *          e_encoding          Encoding of this string.
 *
 * Outputs: -
 *
 * Returns: The string length.
 ----------------------------------------------------------------------------*/
SIZE_T x_fe_strlen(
    const UINT8        *pui1_str,
    FE_ENCODING_TYPE_T e_encoding,
    SIZE_T z_max_size);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_char
 *
 * Description: This function retrieves the character code of a specific character.
 *              of a string.
 *
 * Inputs:  pui1_str            Pointer to the string.
 *          ui4_index           index of the character.
 *          e_encoding          Encoding of this string.
 *
 * Outputs: -
 *
 * Returns: The character code.
 ----------------------------------------------------------------------------*/
CHAR_CODE x_fe_get_char(
    const UINT8        *pui1_str,
    UINT32             ui4_index,
    FE_ENCODING_TYPE_T e_encoding);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_encoding
 *
 * Description: This function sets the encoding of this font
 *
 * Inputs:  h_font              font handle
 *          e_encoding          encoding specified
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_encoding(
    HFONT_T             h_font,
    FE_ENCODING_TYPE_T  e_encoding);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_encoding
 *
 * Description: This function gets the encoding of this font
 *
 * Inputs:  h_font              font handle
 *
 * Outputs: pe_encoding         font encoding
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_encoding(
    HFONT_T             h_font,
    FE_ENCODING_TYPE_T  *pe_encoding);

#endif  /* _X_FE_H */

/*-----------------------------------------------------------------------------
 * Name:  x_fe_set_unrecognizable_char
 *
 * Description: This function sets the character to be displayed when a unrecognizable character is met
 *
 * Inputs:  h_font              font handle
 *          w2_ch               desired character
 *
 * Outputs: -
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_set_unrecognizable_char(
    HFONT_T             h_font,
    CHAR_CODE           t_ch);

/*-----------------------------------------------------------------------------
 * Name:  x_fe_get_unrecognizable_char
 *
 * Description: This function sets the character to be displayed when a unrecognizable character is met
 *
 * Inputs:  h_font              font handle
 *
 * Outputs: pw2_ch              pointer to the desired character
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 x_fe_get_unrecognizable_char(
    HFONT_T             h_font,
    CHAR_CODE           *pt_ch);


INT32 x_fe_set_blurred_width(
        HFONT_T                 h_fnt,
        UINT16                  ui2_width);

INT32 x_fe_cache_flush(VOID);

INT32 x_fe_preload_char(
    HFONT_T                     h_fnt,
    FE_RENDER_MODE_T            e_render_mode,
    UTF16_T*                    w2s_preload_string,
    UINT32                      ui4_str_size,
    UINT32*                     pui4_str_size_loaded);

INT32   x_fe_get_fontdata_by_name(const CHAR*         ps_font,
								UINT8**				ppui1_fontdata,
                                                UINT32*             pui4_fontsize,
                                                FE_FNT_RSC_TYPE_T*  pt_src_type);

INT32	x_fe_get_size_from_height(
					HFONT_T		h_fnt, 
					UINT16		ui2_height,
					UINT8*		pui1_point_size);



