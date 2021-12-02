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
 * $RCSfile: c_fe.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/6 $
 * $SWAuthor: Ben Tsai $
 * $MD5HEX: 8a12a4cc64029e658858979807967a27 $
 *
 * Description:
 *         This header file contains font library specific definitions,
 *         which are exported for application.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "aee/aee.h"
#include "unicode/x_uc_str.h"
#include "font/fe.h"
#include "font/x_fe.h"
#include "font/c_fe.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    static variables
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  c_fe_add_font
 *
 * Description: Applications could call this function to add a font into system.
 *              When this function is called, font engine will read and validate
 *              the specified font file. Clients should also specify three font 
 *              sizes (Small / Medium / Large) for this font. For vector font,
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
 *          FER_NO_MEMORY       Out of system memory
 ----------------------------------------------------------------------------*/
INT32 c_fe_add_file_font(
    const CHAR*             ps_font,        /* font name                     */
    const CHAR*             ps_file,        /* font file name                */
    UINT8                   ui1_small,      /* size of FE_FNT_SIZE_SMALL     */
    UINT8                   ui1_medium,     /* size of FE_FNT_SIZE_MEDIUM    */
    UINT8                   ui1_large)      /* size of FE_FNT_SIZE_LARGE     */
{
    return x_fe_add_file_font( ps_font,
                         ps_file,
                         ui1_small,
                         ui1_medium,
                         ui1_large);
}


INT32 c_fe_add_multiple_file_font(
    const CHAR*             ps_font,
    FE_INIT_FILE            at_init_file[],
    UINT8                   ui1_file_count)
    {

         return x_fe_add_multiple_file_font(ps_font, at_init_file, ui1_file_count);
    }



/*-----------------------------------------------------------------------------
 * Name:  c_fe_add_mem_font
 *
 * Description: Applications could call this function to add a font into system.
 *              When this function is called, font engine will read and validate
 *              the specified font file. Clients should also specify three font 
 *              sizes (Small / Medium / Large) for this font. For vector font,
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
 *          FER_NO_FONT_FILE    The ps_file font file doesn't exist.
 *          FER_INV_FONT        The ps_file file is not a valid font file.
 *          FER_NO_FONT_SIZE    Some specified font sizes are not available.
 *          FER_NO_MEMORY       Out of system memory
 *          FER_FONT_EXIST      The same font existed.
 ----------------------------------------------------------------------------*/
INT32 c_fe_add_mem_font(
    const CHAR*             ps_font,        /* font name                     */
    const UINT8*            pui1_fontdata,  /* font data                     */
    UINT32                  ui4_size,       /* size of font data             */
    UINT8                   ui1_small,      /* size of FE_FNT_SIZE_SMALL     */
    UINT8                   ui1_medium,     /* size for FE_FNT_SIZE_MEDIUM   */
    UINT8                   ui1_large)      /* size for FE_FNT_SIZE_LARGE    */
{
    return x_fe_add_mem_font( ps_font,
                         pui1_fontdata,
                         ui4_size,
                         ui1_small,
                         ui1_medium,
                         ui1_large);
}


/*-----------------------------------------------------------------------------
 * Name:  c_fe_add_combine_bmp_mem_font
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
INT32 c_fe_add_combine_bmp_mem_font(
    const CHAR*             ps_font,                /* font name                     */
    const UINT8*            pui1_fontdata_small,    /* data of small-sized font      */
    UINT32                  ui4_size_small,         /* data size of small-sized font */
    const UINT8*            pui1_fontdata_medium,   /* data of medium-sized font      */
    UINT32                  ui4_size_medium,        /* data size of medium-sized font */
    const UINT8*            pui1_fontdata_large,    /* data of large-sized font      */
    UINT32                  ui4_size_large)         /* data size of large-sized font */
{
    return x_fe_add_combine_bmp_mem_font( ps_font,
                                          pui1_fontdata_small,
                                          ui4_size_small,
                                          pui1_fontdata_medium,
                                          ui4_size_medium,
                                          pui1_fontdata_large,
                                          ui4_size_large );
}

INT32 c_fe_add_multiple_mem_font(
    const CHAR*             ps_font,
    FE_INIT_DATA            at_init_data[],
    UINT8                   ui1_data_count)
{
    return x_fe_add_multiple_mem_font(ps_font, at_init_data, ui1_data_count);
}

INT32 c_fe_change_font_resource(
    const CHAR              *ps_font,
    UINT8                   ui1_font_id)
{
    return x_fe_change_font_resource(ps_font, ui1_font_id);
}


INT32 c_fe_get_cur_font_resource(
	HFONT_T h_fnt,
	UINT8  *pui1_cur_font_id)
{
	return x_fe_get_cur_font_resource(h_fnt, pui1_cur_font_id);
}
/*-----------------------------------------------------------------------------
 * Name: c_fe_remove_font
 *
 * Description: This function removes a font from system.
 *
 * Inputs:  ps_font             Specify the Font name.
 *          e_option            Specify X_FE_OPTION_FORCE to force the removal 
 *                              of specified font; the font engine will free
 *                              all the associated objects, handles and other
 *                              resources; conversely, if existed any handle
 *                              associated with this font, the font engine
 *                              will return the error status FER_FONT_IN_USE.
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
INT32 c_fe_remove_font(
    const CHAR*             ps_font,        /* font name                     */
    FE_REMOVE_OPTION_T      e_option)
{
    return x_fe_remove_font( ps_font,
                            e_option);
}


/*-----------------------------------------------------------------------------
 * Name:  c_fe_has_font
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
BOOL c_fe_has_font(
    const CHAR*             ps_font,
    FE_FNT_STYLE            e_style)
{
    return x_fe_has_font(ps_font, e_style);
}


/*-----------------------------------------------------------------------------
 * Name:  c_fe_create_font
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
INT32 c_fe_create_font(
    const CHAR*             ps_font,        /* font name                     */
    FE_FNT_SIZE             e_size,         /* size of font                  */
    FE_FNT_STYLE            e_style,        /* italic/bold/underline/...     */
    FE_CMAP_ENCODING        e_cmap,         /* character map identifier      */
    HFONT_T*                ph_fnt)         /* return : font hadle           */
{
    HANDLE_T  h_aux;
    INT32     i4_res;

    i4_res = aee_grab_handle_resource(AEE_FLAG,
                                      1,
                                      &h_aux);

    if (i4_res == AEER_OK)
    {
        i4_res = x_fe_create_font(ps_font, e_size, e_style, e_cmap, ph_fnt);

        if (i4_res == AEER_OK)
        {
            handle_link_to_aux(h_aux, *ph_fnt);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }

    return i4_res;
}


/*-----------------------------------------------------------------------------
 * Name:  c_fe_has_cmap
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
BOOL c_fe_has_cmap(
    const CHAR*             ps_font,
    FE_FNT_STYLE            e_style,
    FE_CMAP_ENCODING        e_cmap)
{
    return x_fe_has_cmap(ps_font, e_style, e_cmap);
}


/*-----------------------------------------------------------------------------
 * Name:  c_fe_get_string_size
 *
 * Description: This API calculates the pixel size of the string
 *              (when displayed on screen).
 *
 * Inputs:  h_fnt               Contains a handle of font object.
 *          w2s_str             The string to be evaluated.
 *          i4_length           Maximum length to be evaluated.
 *                              If <0, use whole w2s_str. (NULL-terminated)
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
 ----------------------------------------------------------------------------*/
INT32 c_fe_get_string_size(
    HFONT_T                 h_fnt,
    const UTF16_T*          w2s_str,        /* string to be evaluated        */
    INT32                   i4_length,      /* max length to be evaluated    */
    INT32*                  pi4_width,      /* width of string               */
    INT32*                  pi4_height)     /* height of string              */
{
    return x_fe_get_string_size(h_fnt,
                                w2s_str,
                                i4_length,
                                pi4_width,
                                pi4_height);
}


/*-----------------------------------------------------------------------------
 * Name:  c_fe_set_resolution
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
INT32 c_fe_set_resolution(
    UINT32                  ui4_dpi_hori,   /* horizontal resolution in dpi  */
    UINT32                  ui4_dpi_vert)   /* vertical   resolution in dpi  */
{
    return fe_set_resolution(ui4_dpi_hori, ui4_dpi_vert);
}

/*-----------------------------------------------------------------------------
 * Name:  c_fe_set_rendering_offset
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
INT32 c_fe_set_rendering_offset(
    const CHAR          *ps_font,
    UINT8               ui1_font_id,
    INT32               i4_offset_small,
    INT32               i4_offset_medium,
    INT32               i4_offset_large)
{
    return x_fe_set_rendering_offset(ps_font, ui1_font_id, i4_offset_small, i4_offset_medium, i4_offset_large);
}
/*-----------------------------------------------------------------------------
 * Name:  c_fe_set_unrecognizable_char
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

INT32 c_fe_set_unrecognizable_char(
    HFONT_T             h_font,
    CHAR_CODE           t_ch)

{
    return x_fe_set_unrecognizable_char(h_font, t_ch);
}
/*-----------------------------------------------------------------------------
 * Name:  c_fe_get_unrecognizable_char
 *
 * Description: This function gets the character to be displayed when a unrecognizable character is met
 *
 * Inputs:  h_font              font handle
 *
 * Outputs: pw2_ch              pointer to the desired character
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/

INT32 c_fe_get_unrecognizable_char(
    HFONT_T             h_font,
    CHAR_CODE           *pt_ch)
{
    return x_fe_get_unrecognizable_char(h_font, pt_ch);
}

/*-----------------------------------------------------------------------------
 * Name:  c_fe_cache_flush
 *
 * Description: This function empty the cache memory.
 *
 * Returns: FER_OK              The routine was successful.
 ----------------------------------------------------------------------------*/

INT32 c_fe_cache_flush(VOID)
{
    return x_fe_cache_flush();
}

/*-----------------------------------------------------------------------------
 * Name:  c_fe_preload_char
 *
 * Description: This function preload characters to be displayed
 *
 * Inputs:  h_font              font handle
 *             e_render_mode  font render mode
 *             w2s_preload_string  string for preload
                ui4_str_size            string size
                
 * Outputs: pui4_str_size_loaded              size of preloaded
 *
 * Returns: FER_OK              The routine was successful.
 *          FER_INV_HANDLE      Invalid handle.
 ----------------------------------------------------------------------------*/

INT32 c_fe_preload_char(
    HFONT_T                     h_fnt,
    FE_RENDER_MODE_T            e_render_mode,
    UTF16_T*                    w2s_preload_string,
    UINT32                      ui4_str_size,
    UINT32*                     pui4_str_size_loaded)
{
    return x_fe_preload_char(h_fnt, e_render_mode, w2s_preload_string, ui4_str_size, pui4_str_size_loaded);
}


