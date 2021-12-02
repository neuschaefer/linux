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
 * $RCSfile: dvb_si_txt_cnv_8859.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision$
 * $SWAuthor$
 * $MD5HEX$
 *
 * Description:
 *         This file contains the handling of TEXT type objects.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "mm_util_table.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/
#ifdef MINFO_ID3_ENCODE_BY_LANGUAGE
static UTF16_T aw2_8859_2_cnv_tbl[]=
{
    /*0xA1*/    0x0104,    /*#	LATIN CAPITAL LETTER A WITH OGONEK*/
    /*0xA2*/    0x02D8,    /*#	BREVE*/
    /*0xA3*/    0x0141,    /*#	LATIN CAPITAL LETTER L WITH STROKE*/
    /*0xA4*/    0x00A4,    /*#	CURRENCY SIGN*/
    /*0xA5*/    0x013D,    /*#	LATIN CAPITAL LETTER L WITH CARON*/
    /*0xA6*/    0x015A,    /*#	LATIN CAPITAL LETTER S WITH ACUTE*/
    /*0xA7*/    0x00A7,    /*#	SECTION SIGN*/
    /*0xA8*/    0x00A8,    /*#	DIAERESIS*/
    /*0xA9*/    0x0160,    /*#	LATIN CAPITAL LETTER S WITH CARON*/
    /*0xAA*/    0x015E,    /*#	LATIN CAPITAL LETTER S WITH CEDILLA*/
    /*0xAB*/    0x0164,    /*#	LATIN CAPITAL LETTER T WITH CARON*/
    /*0xAC*/    0x0179,    /*#	LATIN CAPITAL LETTER Z WITH ACUTE*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0x017D,    /*#	LATIN CAPITAL LETTER Z WITH CARON*/
    /*0xAF*/    0x017B,    /*#	LATIN CAPITAL LETTER Z WITH DOT ABOVE*/
    /*0xB0*/    0x00B0,    /*#	DEGREE SIGN*/
    /*0xB1*/    0x0105,    /*#	LATIN SMALL LETTER A WITH OGONEK*/
    /*0xB2*/    0x02DB,    /*#	OGONEK*/
    /*0xB3*/    0x0142,    /*#	LATIN SMALL LETTER L WITH STROKE*/
    /*0xB4*/    0x00B4,    /*#	ACUTE ACCENT*/
    /*0xB5*/    0x013E,    /*#	LATIN SMALL LETTER L WITH CARON*/
    /*0xB6*/    0x015B,    /*#	LATIN SMALL LETTER S WITH ACUTE*/
    /*0xB7*/    0x02C7,    /*#	CARON*/
    /*0xB8*/    0x00B8,    /*#	CEDILLA*/
    /*0xB9*/    0x0161,    /*#	LATIN SMALL LETTER S WITH CARON*/
    /*0xBA*/    0x015F,    /*#	LATIN SMALL LETTER S WITH CEDILLA*/
    /*0xBB*/    0x0165,    /*#	LATIN SMALL LETTER T WITH CARON*/
    /*0xBC*/    0x017A,    /*#	LATIN SMALL LETTER Z WITH ACUTE*/
    /*0xBD*/    0x02DD,    /*#	DOUBLE ACUTE ACCENT*/
    /*0xBE*/    0x017E,    /*#	LATIN SMALL LETTER Z WITH CARON*/
    /*0xBF*/    0x017C,    /*#	LATIN SMALL LETTER Z WITH DOT ABOVE*/
    /*0xC0*/    0x0154,    /*#	LATIN CAPITAL LETTER R WITH ACUTE*/
    /*0xC1*/    0x00C1,    /*#	LATIN CAPITAL LETTER A WITH ACUTE*/
    /*0xC2*/    0x00C2,    /*#	LATIN CAPITAL LETTER A WITH CIRCUMFLEX*/
    /*0xC3*/    0x0102,    /*#	LATIN CAPITAL LETTER A WITH BREVE*/
    /*0xC4*/    0x00C4,    /*#	LATIN CAPITAL LETTER A WITH DIAERESIS*/
    /*0xC5*/    0x0139,    /*#	LATIN CAPITAL LETTER L WITH ACUTE*/
    /*0xC6*/    0x0106,    /*#	LATIN CAPITAL LETTER C WITH ACUTE*/
    /*0xC7*/    0x00C7,    /*#	LATIN CAPITAL LETTER C WITH CEDILLA*/
    /*0xC8*/    0x010C,    /*#	LATIN CAPITAL LETTER C WITH CARON*/
    /*0xC9*/    0x00C9,    /*#	LATIN CAPITAL LETTER E WITH ACUTE*/
    /*0xCA*/    0x0118,    /*#	LATIN CAPITAL LETTER E WITH OGONEK*/
    /*0xCB*/    0x00CB,    /*#	LATIN CAPITAL LETTER E WITH DIAERESIS*/
    /*0xCC*/    0x011A,    /*#	LATIN CAPITAL LETTER E WITH CARON*/
    /*0xCD*/    0x00CD,    /*#	LATIN CAPITAL LETTER I WITH ACUTE*/
    /*0xCE*/    0x00CE,    /*#	LATIN CAPITAL LETTER I WITH CIRCUMFLEX*/
    /*0xCF*/    0x010E,    /*#	LATIN CAPITAL LETTER D WITH CARON*/
    /*0xD0*/    0x0110,    /*#	LATIN CAPITAL LETTER D WITH STROKE*/
    /*0xD1*/    0x0143,    /*#	LATIN CAPITAL LETTER N WITH ACUTE*/
    /*0xD2*/    0x0147,    /*#	LATIN CAPITAL LETTER N WITH CARON*/
    /*0xD3*/    0x00D3,    /*#	LATIN CAPITAL LETTER O WITH ACUTE*/
    /*0xD4*/    0x00D4,    /*#	LATIN CAPITAL LETTER O WITH CIRCUMFLEX*/
    /*0xD5*/    0x0150,    /*#	LATIN CAPITAL LETTER O WITH DOUBLE ACUTE*/
    /*0xD6*/    0x00D6,    /*#	LATIN CAPITAL LETTER O WITH DIAERESIS*/
    /*0xD7*/    0x00D7,    /*#	MULTIPLICATION SIGN*/
    /*0xD8*/    0x0158,    /*#	LATIN CAPITAL LETTER R WITH CARON*/
    /*0xD9*/    0x016E,    /*#	LATIN CAPITAL LETTER U WITH RING ABOVE*/
    /*0xDA*/    0x00DA,    /*#	LATIN CAPITAL LETTER U WITH ACUTE*/
    /*0xDB*/    0x0170,    /*#	LATIN CAPITAL LETTER U WITH DOUBLE ACUTE*/
    /*0xDC*/    0x00DC,    /*#	LATIN CAPITAL LETTER U WITH DIAERESIS*/
    /*0xDD*/    0x00DD,    /*#	LATIN CAPITAL LETTER Y WITH ACUTE*/
    /*0xDE*/    0x0162,    /*#	LATIN CAPITAL LETTER T WITH CEDILLA*/
    /*0xDF*/    0x00DF,    /*#	LATIN SMALL LETTER SHARP S*/
    /*0xE0*/    0x0155,    /*#	LATIN SMALL LETTER R WITH ACUTE*/
    /*0xE1*/    0x00E1,    /*#	LATIN SMALL LETTER A WITH ACUTE*/
    /*0xE2*/    0x00E2,    /*#	LATIN SMALL LETTER A WITH CIRCUMFLEX*/
    /*0xE3*/    0x0103,    /*#	LATIN SMALL LETTER A WITH BREVE*/
    /*0xE4*/    0x00E4,    /*#	LATIN SMALL LETTER A WITH DIAERESIS*/
    /*0xE5*/    0x013A,    /*#	LATIN SMALL LETTER L WITH ACUTE*/
    /*0xE6*/    0x0107,    /*#	LATIN SMALL LETTER C WITH ACUTE*/
    /*0xE7*/    0x00E7,    /*#	LATIN SMALL LETTER C WITH CEDILLA*/
    /*0xE8*/    0x010D,    /*#	LATIN SMALL LETTER C WITH CARON*/
    /*0xE9*/    0x00E9,    /*#	LATIN SMALL LETTER E WITH ACUTE*/
    /*0xEA*/    0x0119,    /*#	LATIN SMALL LETTER E WITH OGONEK*/
    /*0xEB*/    0x00EB,    /*#	LATIN SMALL LETTER E WITH DIAERESIS*/
    /*0xEC*/    0x011B,    /*#	LATIN SMALL LETTER E WITH CARON*/
    /*0xED*/    0x00ED,    /*#	LATIN SMALL LETTER I WITH ACUTE*/
    /*0xEE*/    0x00EE,    /*#	LATIN SMALL LETTER I WITH CIRCUMFLEX*/
    /*0xEF*/    0x010F,    /*#	LATIN SMALL LETTER D WITH CARON*/
    /*0xF0*/    0x0111,    /*#	LATIN SMALL LETTER D WITH STROKE*/
    /*0xF1*/    0x0144,    /*#	LATIN SMALL LETTER N WITH ACUTE*/
    /*0xF2*/    0x0148,    /*#	LATIN SMALL LETTER N WITH CARON*/
    /*0xF3*/    0x00F3,    /*#	LATIN SMALL LETTER O WITH ACUTE*/
    /*0xF4*/    0x00F4,    /*#	LATIN SMALL LETTER O WITH CIRCUMFLEX*/
    /*0xF5*/    0x0151,    /*#	LATIN SMALL LETTER O WITH DOUBLE ACUTE*/
    /*0xF6*/    0x00F6,    /*#	LATIN SMALL LETTER O WITH DIAERESIS*/
    /*0xF7*/    0x00F7,    /*#	DIVISION SIGN*/
    /*0xF8*/    0x0159,    /*#	LATIN SMALL LETTER R WITH CARON*/
    /*0xF9*/    0x016F,    /*#	LATIN SMALL LETTER U WITH RING ABOVE*/
    /*0xFA*/    0x00FA,    /*#	LATIN SMALL LETTER U WITH ACUTE*/
    /*0xFB*/    0x0171,    /*#	LATIN SMALL LETTER U WITH DOUBLE ACUTE*/
    /*0xFC*/    0x00FC,    /*#	LATIN SMALL LETTER U WITH DIAERESIS*/
    /*0xFD*/    0x00FD,    /*#	LATIN SMALL LETTER Y WITH ACUTE*/
    /*0xFE*/    0x0163,    /*#	LATIN SMALL LETTER T WITH CEDILLA*/
    /*0xFF*/    0x02D9,    /*#	DOT ABOVE*/
};

static UTF16_T aw2_8859_3_cnv_tbl[]=
{
    /*0xA1*/    0x0126,    /*#	LATIN CAPITAL LETTER H WITH STROKE*/
    /*0xA2*/    0x02D8,    /*#	BREVE*/
    /*0xA3*/    0x00A3,    /*#	POUND SIGN*/
    /*0xA4*/    0x00A4,    /*#	CURRENCY SIGN*/
    /*0xA5*/    0xFFFF,    /**/
    /*0xA6*/    0x0124,    /*#	LATIN CAPITAL LETTER H WITH CIRCUMFLEX*/
    /*0xA7*/    0x00A7,    /*#	SECTION SIGN*/
    /*0xA8*/    0x00A8,    /*#	DIAERESIS*/
    /*0xA9*/    0x0130,    /*#	LATIN CAPITAL LETTER I WITH DOT ABOVE*/
    /*0xAA*/    0x015E,    /*#	LATIN CAPITAL LETTER S WITH CEDILLA*/
    /*0xAB*/    0x011E,    /*#	LATIN CAPITAL LETTER G WITH BREVE*/
    /*0xAC*/    0x0134,    /*#	LATIN CAPITAL LETTER J WITH CIRCUMFLEX*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0xFFFF,    /**/
    /*0xAF*/    0x017B,    /*#	LATIN CAPITAL LETTER Z WITH DOT ABOVE*/
    /*0xB0*/    0x00B0,    /*#	DEGREE SIGN*/
    /*0xB1*/    0x0127,    /*#	LATIN SMALL LETTER H WITH STROKE*/
    /*0xB2*/    0x00B2,    /*#	SUPERSCRIPT TWO*/
    /*0xB3*/    0x00B3,    /*#	SUPERSCRIPT THREE*/
    /*0xB4*/    0x00B4,    /*#	ACUTE ACCENT*/
    /*0xB5*/    0x00B5,    /*#	MICRO SIGN*/
    /*0xB6*/    0x0125,    /*#	LATIN SMALL LETTER H WITH CIRCUMFLEX*/
    /*0xB7*/    0x00B7,    /*#	MIDDLE DOT*/
    /*0xB8*/    0x00B8,    /*#	CEDILLA*/
    /*0xB9*/    0x0131,    /*#	LATIN SMALL LETTER DOTLESS I*/
    /*0xBA*/    0x015F,    /*#	LATIN SMALL LETTER S WITH CEDILLA*/
    /*0xBB*/    0x011F,    /*#	LATIN SMALL LETTER G WITH BREVE*/
    /*0xBC*/    0x0135,    /*#	LATIN SMALL LETTER J WITH CIRCUMFLEX*/
    /*0xBD*/    0x00BD,    /*#	VULGAR FRACTION ONE HALF*/
    /*0xBE*/    0xFFFF,    /**/
    /*0xBF*/    0x017C,    /*#	LATIN SMALL LETTER Z WITH DOT ABOVE*/
    /*0xC0*/    0x00C0,    /*#	LATIN CAPITAL LETTER A WITH GRAVE*/
    /*0xC1*/    0x00C1,    /*#	LATIN CAPITAL LETTER A WITH ACUTE*/
    /*0xC2*/    0x00C2,    /*#	LATIN CAPITAL LETTER A WITH CIRCUMFLEX*/
    /*0xC3*/    0xFFFF,    /**/
    /*0xC4*/    0x00C4,    /*#	LATIN CAPITAL LETTER A WITH DIAERESIS*/
    /*0xC5*/    0x010A,    /*#	LATIN CAPITAL LETTER C WITH DOT ABOVE*/
    /*0xC6*/    0x0108,    /*#	LATIN CAPITAL LETTER C WITH CIRCUMFLEX*/
    /*0xC7*/    0x00C7,    /*#	LATIN CAPITAL LETTER C WITH CEDILLA*/
    /*0xC8*/    0x00C8,    /*#	LATIN CAPITAL LETTER E WITH GRAVE*/
    /*0xC9*/    0x00C9,    /*#	LATIN CAPITAL LETTER E WITH ACUTE*/
    /*0xCA*/    0x00CA,    /*#	LATIN CAPITAL LETTER E WITH CIRCUMFLEX*/
    /*0xCB*/    0x00CB,    /*#	LATIN CAPITAL LETTER E WITH DIAERESIS*/
    /*0xCC*/    0x00CC,    /*#	LATIN CAPITAL LETTER I WITH GRAVE*/
    /*0xCD*/    0x00CD,    /*#	LATIN CAPITAL LETTER I WITH ACUTE*/
    /*0xCE*/    0x00CE,    /*#	LATIN CAPITAL LETTER I WITH CIRCUMFLEX*/
    /*0xCF*/    0x00CF,    /*#	LATIN CAPITAL LETTER I WITH DIAERESIS*/
    /*0xD0*/    0xFFFF,    /**/
    /*0xD1*/    0x00D1,    /*#	LATIN CAPITAL LETTER N WITH TILDE*/
    /*0xD2*/    0x00D2,    /*#	LATIN CAPITAL LETTER O WITH GRAVE*/
    /*0xD3*/    0x00D3,    /*#	LATIN CAPITAL LETTER O WITH ACUTE*/
    /*0xD4*/    0x00D4,    /*#	LATIN CAPITAL LETTER O WITH CIRCUMFLEX*/
    /*0xD5*/    0x0120,    /*#	LATIN CAPITAL LETTER G WITH DOT ABOVE*/
    /*0xD6*/    0x00D6,    /*#	LATIN CAPITAL LETTER O WITH DIAERESIS*/
    /*0xD7*/    0x00D7,    /*#	MULTIPLICATION SIGN*/
    /*0xD8*/    0x011C,    /*#	LATIN CAPITAL LETTER G WITH CIRCUMFLEX*/
    /*0xD9*/    0x00D9,    /*#	LATIN CAPITAL LETTER U WITH GRAVE*/
    /*0xDA*/    0x00DA,    /*#	LATIN CAPITAL LETTER U WITH ACUTE*/
    /*0xDB*/    0x00DB,    /*#	LATIN CAPITAL LETTER U WITH CIRCUMFLEX*/
    /*0xDC*/    0x00DC,    /*#	LATIN CAPITAL LETTER U WITH DIAERESIS*/
    /*0xDD*/    0x016C,    /*#	LATIN CAPITAL LETTER U WITH BREVE*/
    /*0xDE*/    0x015C,    /*#	LATIN CAPITAL LETTER S WITH CIRCUMFLEX*/
    /*0xDF*/    0x00DF,    /*#	LATIN SMALL LETTER SHARP S*/
    /*0xE0*/    0x00E0,    /*#	LATIN SMALL LETTER A WITH GRAVE*/
    /*0xE1*/    0x00E1,    /*#	LATIN SMALL LETTER A WITH ACUTE*/
    /*0xE2*/    0x00E2,    /*#	LATIN SMALL LETTER A WITH CIRCUMFLEX*/
    /*0xE3*/    0xFFFF,    /**/
    /*0xE4*/    0x00E4,    /*#	LATIN SMALL LETTER A WITH DIAERESIS*/
    /*0xE5*/    0x010B,    /*#	LATIN SMALL LETTER C WITH DOT ABOVE*/
    /*0xE6*/    0x0109,    /*#	LATIN SMALL LETTER C WITH CIRCUMFLEX*/
    /*0xE7*/    0x00E7,    /*#	LATIN SMALL LETTER C WITH CEDILLA*/
    /*0xE8*/    0x00E8,    /*#	LATIN SMALL LETTER E WITH GRAVE*/
    /*0xE9*/    0x00E9,    /*#	LATIN SMALL LETTER E WITH ACUTE*/
    /*0xEA*/    0x00EA,    /*#	LATIN SMALL LETTER E WITH CIRCUMFLEX*/
    /*0xEB*/    0x00EB,    /*#	LATIN SMALL LETTER E WITH DIAERESIS*/
    /*0xEC*/    0x00EC,    /*#	LATIN SMALL LETTER I WITH GRAVE*/
    /*0xED*/    0x00ED,    /*#	LATIN SMALL LETTER I WITH ACUTE*/
    /*0xEE*/    0x00EE,    /*#	LATIN SMALL LETTER I WITH CIRCUMFLEX*/
    /*0xEF*/    0x00EF,    /*#	LATIN SMALL LETTER I WITH DIAERESIS*/
    /*0xF0*/    0xFFFF,    /**/
    /*0xF1*/    0x00F1,    /*#	LATIN SMALL LETTER N WITH TILDE*/
    /*0xF2*/    0x00F2,    /*#	LATIN SMALL LETTER O WITH GRAVE*/
    /*0xF3*/    0x00F3,    /*#	LATIN SMALL LETTER O WITH ACUTE*/
    /*0xF4*/    0x00F4,    /*#	LATIN SMALL LETTER O WITH CIRCUMFLEX*/
    /*0xF5*/    0x0121,    /*#	LATIN SMALL LETTER G WITH DOT ABOVE*/
    /*0xF6*/    0x00F6,    /*#	LATIN SMALL LETTER O WITH DIAERESIS*/
    /*0xF7*/    0x00F7,    /*#	DIVISION SIGN*/
    /*0xF8*/    0x011D,    /*#	LATIN SMALL LETTER G WITH CIRCUMFLEX*/
    /*0xF9*/    0x00F9,    /*#	LATIN SMALL LETTER U WITH GRAVE*/
    /*0xFA*/    0x00FA,    /*#	LATIN SMALL LETTER U WITH ACUTE*/
    /*0xFB*/    0x00FB,    /*#	LATIN SMALL LETTER U WITH CIRCUMFLEX*/
    /*0xFC*/    0x00FC,    /*#	LATIN SMALL LETTER U WITH DIAERESIS*/
    /*0xFD*/    0x016D,    /*#	LATIN SMALL LETTER U WITH BREVE*/
    /*0xFE*/    0x015D,    /*#	LATIN SMALL LETTER S WITH CIRCUMFLEX*/
    /*0xFF*/    0x02D9,    /*#	DOT ABOVE*/
};

static UTF16_T aw2_8859_4_cnv_tbl[]=
{
    /*0xA1*/    0x0104,    /*#	LATIN CAPITAL LETTER A WITH OGONEK*/
    /*0xA2*/    0x0138,    /*#	LATIN SMALL LETTER KRA*/
    /*0xA3*/    0x0156,    /*#	LATIN CAPITAL LETTER R WITH CEDILLA*/
    /*0xA4*/    0x00A4,    /*#	CURRENCY SIGN*/
    /*0xA5*/    0x0128,    /*#	LATIN CAPITAL LETTER I WITH TILDE*/
    /*0xA6*/    0x013B,    /*#	LATIN CAPITAL LETTER L WITH CEDILLA*/
    /*0xA7*/    0x00A7,    /*#	SECTION SIGN*/
    /*0xA8*/    0x00A8,    /*#	DIAERESIS*/
    /*0xA9*/    0x0160,    /*#	LATIN CAPITAL LETTER S WITH CARON*/
    /*0xAA*/    0x0112,    /*#	LATIN CAPITAL LETTER E WITH MACRON*/
    /*0xAB*/    0x0122,    /*#	LATIN CAPITAL LETTER G WITH CEDILLA*/
    /*0xAC*/    0x0166,    /*#	LATIN CAPITAL LETTER T WITH STROKE*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0x017D,    /*#	LATIN CAPITAL LETTER Z WITH CARON*/
    /*0xAF*/    0x00AF,    /*#	MACRON*/
    /*0xB0*/    0x00B0,    /*#	DEGREE SIGN*/
    /*0xB1*/    0x0105,    /*#	LATIN SMALL LETTER A WITH OGONEK*/
    /*0xB2*/    0x02DB,    /*#	OGONEK*/
    /*0xB3*/    0x0157,    /*#	LATIN SMALL LETTER R WITH CEDILLA*/
    /*0xB4*/    0x00B4,    /*#	ACUTE ACCENT*/
    /*0xB5*/    0x0129,    /*#	LATIN SMALL LETTER I WITH TILDE*/
    /*0xB6*/    0x013C,    /*#	LATIN SMALL LETTER L WITH CEDILLA*/
    /*0xB7*/    0x02C7,    /*#	CARON*/
    /*0xB8*/    0x00B8,    /*#	CEDILLA*/
    /*0xB9*/    0x0161,    /*#	LATIN SMALL LETTER S WITH CARON*/
    /*0xBA*/    0x0113,    /*#	LATIN SMALL LETTER E WITH MACRON*/
    /*0xBB*/    0x0123,    /*#	LATIN SMALL LETTER G WITH CEDILLA*/
    /*0xBC*/    0x0167,    /*#	LATIN SMALL LETTER T WITH STROKE*/
    /*0xBD*/    0x014A,    /*#	LATIN CAPITAL LETTER ENG*/
    /*0xBE*/    0x017E,    /*#	LATIN SMALL LETTER Z WITH CARON*/
    /*0xBF*/    0x014B,    /*#	LATIN SMALL LETTER ENG*/
    /*0xC0*/    0x0100,    /*#	LATIN CAPITAL LETTER A WITH MACRON*/
    /*0xC1*/    0x00C1,    /*#	LATIN CAPITAL LETTER A WITH ACUTE*/
    /*0xC2*/    0x00C2,    /*#	LATIN CAPITAL LETTER A WITH CIRCUMFLEX*/
    /*0xC3*/    0x00C3,    /*#	LATIN CAPITAL LETTER A WITH TILDE*/
    /*0xC4*/    0x00C4,    /*#	LATIN CAPITAL LETTER A WITH DIAERESIS*/
    /*0xC5*/    0x00C5,    /*#	LATIN CAPITAL LETTER A WITH RING ABOVE*/
    /*0xC6*/    0x00C6,    /*#	LATIN CAPITAL LETTER AE*/
    /*0xC7*/    0x012E,    /*#	LATIN CAPITAL LETTER I WITH OGONEK*/
    /*0xC8*/    0x010C,    /*#	LATIN CAPITAL LETTER C WITH CARON*/
    /*0xC9*/    0x00C9,    /*#	LATIN CAPITAL LETTER E WITH ACUTE*/
    /*0xCA*/    0x0118,    /*#	LATIN CAPITAL LETTER E WITH OGONEK*/
    /*0xCB*/    0x00CB,    /*#	LATIN CAPITAL LETTER E WITH DIAERESIS*/
    /*0xCC*/    0x0116,    /*#	LATIN CAPITAL LETTER E WITH DOT ABOVE*/
    /*0xCD*/    0x00CD,    /*#	LATIN CAPITAL LETTER I WITH ACUTE*/
    /*0xCE*/    0x00CE,    /*#	LATIN CAPITAL LETTER I WITH CIRCUMFLEX*/
    /*0xCF*/    0x012A,    /*#	LATIN CAPITAL LETTER I WITH MACRON*/
    /*0xD0*/    0x0110,    /*#	LATIN CAPITAL LETTER D WITH STROKE*/
    /*0xD1*/    0x0145,    /*#	LATIN CAPITAL LETTER N WITH CEDILLA*/
    /*0xD2*/    0x014C,    /*#	LATIN CAPITAL LETTER O WITH MACRON*/
    /*0xD3*/    0x0136,    /*#	LATIN CAPITAL LETTER K WITH CEDILLA*/
    /*0xD4*/    0x00D4,    /*#	LATIN CAPITAL LETTER O WITH CIRCUMFLEX*/
    /*0xD5*/    0x00D5,    /*#	LATIN CAPITAL LETTER O WITH TILDE*/
    /*0xD6*/    0x00D6,    /*#	LATIN CAPITAL LETTER O WITH DIAERESIS*/
    /*0xD7*/    0x00D7,    /*#	MULTIPLICATION SIGN*/
    /*0xD8*/    0x00D8,    /*#	LATIN CAPITAL LETTER O WITH STROKE*/
    /*0xD9*/    0x0172,    /*#	LATIN CAPITAL LETTER U WITH OGONEK*/
    /*0xDA*/    0x00DA,    /*#	LATIN CAPITAL LETTER U WITH ACUTE*/
    /*0xDB*/    0x00DB,    /*#	LATIN CAPITAL LETTER U WITH CIRCUMFLEX*/
    /*0xDC*/    0x00DC,    /*#	LATIN CAPITAL LETTER U WITH DIAERESIS*/
    /*0xDD*/    0x0168,    /*#	LATIN CAPITAL LETTER U WITH TILDE*/
    /*0xDE*/    0x016A,    /*#	LATIN CAPITAL LETTER U WITH MACRON*/
    /*0xDF*/    0x00DF,    /*#	LATIN SMALL LETTER SHARP S*/
    /*0xE0*/    0x0101,    /*#	LATIN SMALL LETTER A WITH MACRON*/
    /*0xE1*/    0x00E1,    /*#	LATIN SMALL LETTER A WITH ACUTE*/
    /*0xE2*/    0x00E2,    /*#	LATIN SMALL LETTER A WITH CIRCUMFLEX*/
    /*0xE3*/    0x00E3,    /*#	LATIN SMALL LETTER A WITH TILDE*/
    /*0xE4*/    0x00E4,    /*#	LATIN SMALL LETTER A WITH DIAERESIS*/
    /*0xE5*/    0x00E5,    /*#	LATIN SMALL LETTER A WITH RING ABOVE*/
    /*0xE6*/    0x00E6,    /*#	LATIN SMALL LETTER AE*/
    /*0xE7*/    0x012F,    /*#	LATIN SMALL LETTER I WITH OGONEK*/
    /*0xE8*/    0x010D,    /*#	LATIN SMALL LETTER C WITH CARON*/
    /*0xE9*/    0x00E9,    /*#	LATIN SMALL LETTER E WITH ACUTE*/
    /*0xEA*/    0x0119,    /*#	LATIN SMALL LETTER E WITH OGONEK*/
    /*0xEB*/    0x00EB,    /*#	LATIN SMALL LETTER E WITH DIAERESIS*/
    /*0xEC*/    0x0117,    /*#	LATIN SMALL LETTER E WITH DOT ABOVE*/
    /*0xED*/    0x00ED,    /*#	LATIN SMALL LETTER I WITH ACUTE*/
    /*0xEE*/    0x00EE,    /*#	LATIN SMALL LETTER I WITH CIRCUMFLEX*/
    /*0xEF*/    0x012B,    /*#	LATIN SMALL LETTER I WITH MACRON*/
    /*0xF0*/    0x0111,    /*#	LATIN SMALL LETTER D WITH STROKE*/
    /*0xF1*/    0x0146,    /*#	LATIN SMALL LETTER N WITH CEDILLA*/
    /*0xF2*/    0x014D,    /*#	LATIN SMALL LETTER O WITH MACRON*/
    /*0xF3*/    0x0137,    /*#	LATIN SMALL LETTER K WITH CEDILLA*/
    /*0xF4*/    0x00F4,    /*#	LATIN SMALL LETTER O WITH CIRCUMFLEX*/
    /*0xF5*/    0x00F5,    /*#	LATIN SMALL LETTER O WITH TILDE*/
    /*0xF6*/    0x00F6,    /*#	LATIN SMALL LETTER O WITH DIAERESIS*/
    /*0xF7*/    0x00F7,    /*#	DIVISION SIGN*/
    /*0xF8*/    0x00F8,    /*#	LATIN SMALL LETTER O WITH STROKE*/
    /*0xF9*/    0x0173,    /*#	LATIN SMALL LETTER U WITH OGONEK*/
    /*0xFA*/    0x00FA,    /*#	LATIN SMALL LETTER U WITH ACUTE*/
    /*0xFB*/    0x00FB,    /*#	LATIN SMALL LETTER U WITH CIRCUMFLEX*/
    /*0xFC*/    0x00FC,    /*#	LATIN SMALL LETTER U WITH DIAERESIS*/
    /*0xFD*/    0x0169,    /*#	LATIN SMALL LETTER U WITH TILDE*/
    /*0xFE*/    0x016B,    /*#	LATIN SMALL LETTER U WITH MACRON*/
    /*0xFF*/    0x02D9,    /*#	DOT ABOVE*/
};

static UTF16_T aw2_8859_7_cnv_tbl[]=
{
    /*0xA1*/    0x2018,    /**/
    /*0xA2*/    0x2019,    /**/
    /*0xA3*/    0x00A3,    /**/
    /*0xA4*/    0x20AC,    /**/
    /*0xA5*/    0x20AF,    /**/
    /*0xA6*/    0x00A6,    /**/
    /*0xA7*/    0x00A7,    /**/
    /*0xA8*/    0x00A8,    /**/
    /*0xA9*/    0x00A9,    /**/
    /*0xAA*/    0x037A,    /**/
    /*0xAB*/    0x00AB,    /**/
    /*0xAC*/    0x00AC,    /**/
    /*0xAD*/    0x00AD,    /**/
    /*0xAE*/    0xFFFF,    /**/
    /*0xAF*/    0x2015,    /**/
    /*0xB0*/    0x00B0,    /**/
    /*0xB1*/    0x00B1,    /**/
    /*0xB2*/    0x00B2,    /**/
    /*0xB3*/    0x00B3,    /**/
    /*0xB4*/    0x0384,    /**/
    /*0xB5*/    0x0385,    /**/
    /*0xB6*/    0x0386,    /**/
    /*0xB7*/    0x00B7,    /**/
    /*0xB8*/    0x0388,    /**/
    /*0xB9*/    0x0389,    /**/
    /*0xBA*/    0x038A,    /**/
    /*0xBB*/    0x00BB,    /**/
    /*0xBC*/    0x038C,    /**/
    /*0xBD*/    0x00BD,    /**/
    /*0xBE*/    0x038E,    /**/
    /*0xBF*/    0x038F,    /**/
    /*0xC0*/    0x0390,    /**/
    /*0xC1*/    0x0391,    /**/
    /*0xC2*/    0x0392,    /**/
    /*0xC3*/    0x0393,    /**/
    /*0xC4*/    0x0394,    /**/
    /*0xC5*/    0x0395,    /**/
    /*0xC6*/    0x0396,    /**/
    /*0xC7*/    0x0397,    /**/
    /*0xC8*/    0x0398,    /**/
    /*0xC9*/    0x0399,    /**/
    /*0xCA*/    0x039A,    /**/
    /*0xCB*/    0x039B,    /**/
    /*0xCC*/    0x039C,    /**/
    /*0xCD*/    0x039D,    /**/
    /*0xCE*/    0x039E,    /**/
    /*0xCF*/    0x039F,    /**/
    /*0xD0*/    0x03A0,    /**/
    /*0xD1*/    0x03A1,    /**/
    /*0xD2*/    0xFFFF,    /**/
    /*0xD3*/    0x03A3,    /**/
    /*0xD4*/    0x03A4,    /**/
    /*0xD5*/    0x03A5,    /**/
    /*0xD6*/    0x03A6,    /**/
    /*0xD7*/    0x03A7,    /**/
    /*0xD8*/    0x03A8,    /**/
    /*0xD9*/    0x03A9,    /**/
    /*0xDA*/    0x03AA,    /**/
    /*0xDB*/    0x03AB,    /**/
    /*0xDC*/    0x03AC,    /**/
    /*0xDD*/    0x03AD,    /**/
    /*0xDE*/    0x03AE,    /**/
    /*0xDF*/    0x03AF,    /**/
    /*0xE0*/    0x03B0,    /**/
    /*0xE1*/    0x03B1,    /**/
    /*0xE2*/    0x03B2,    /**/
    /*0xE3*/    0x03B3,    /**/
    /*0xE4*/    0x03B4,    /**/
    /*0xE5*/    0x03B5,    /**/
    /*0xE6*/    0x03B6,    /**/
    /*0xE7*/    0x03B7,    /**/
    /*0xE8*/    0x03B8,    /**/
    /*0xE9*/    0x03B9,    /**/
    /*0xEA*/    0x03BA,    /**/
    /*0xEB*/    0x03BB,    /**/
    /*0xEC*/    0x03BC,    /**/
    /*0xED*/    0x03BD,    /**/
    /*0xEE*/    0x03BE,    /**/
    /*0xEF*/    0x03BF,    /**/
    /*0xF0*/    0x03C0,    /**/
    /*0xF1*/    0x03C1,    /**/
    /*0xF2*/    0x03C2,    /**/
    /*0xF3*/    0x03C3,    /**/
    /*0xF4*/    0x03C4,    /**/
    /*0xF5*/    0x03C5,    /**/
    /*0xF6*/    0x03C6,    /**/
    /*0xF7*/    0x03C7,    /**/
    /*0xF8*/    0x03C8,    /**/
    /*0xF9*/    0x03C9,    /**/
    /*0xFA*/    0x03CA,    /**/
    /*0xFB*/    0x03CB,    /**/
    /*0xFC*/    0x03CC,    /**/
    /*0xFD*/    0x03CD,    /**/
    /*0xFE*/    0x03CE,    /**/
    /*0xFF*/    0xFFFF,    /**/
};

static UTF16_T aw2_8859_10_cnv_tbl[]=
{
    /*0xA1*/    0x0104,    /*#	LATIN CAPITAL LETTER A WITH OGONEK*/
    /*0xA2*/    0x0112,    /*#	LATIN CAPITAL LETTER E WITH MACRON*/
    /*0xA3*/    0x0122,    /*#	LATIN CAPITAL LETTER G WITH CEDILLA*/
    /*0xA4*/    0x012A,    /*#	LATIN CAPITAL LETTER I WITH MACRON*/
    /*0xA5*/    0x0128,    /*#	LATIN CAPITAL LETTER I WITH TILDE*/
    /*0xA6*/    0x0136,    /*#	LATIN CAPITAL LETTER K WITH CEDILLA*/
    /*0xA7*/    0x00A7,    /*#	SECTION SIGN*/
    /*0xA8*/    0x013B,    /*#	LATIN CAPITAL LETTER L WITH CEDILLA*/
    /*0xA9*/    0x0110,    /*#	LATIN CAPITAL LETTER D WITH STROKE*/
    /*0xAA*/    0x0160,    /*#	LATIN CAPITAL LETTER S WITH CARON*/
    /*0xAB*/    0x0166,    /*#	LATIN CAPITAL LETTER T WITH STROKE*/
    /*0xAC*/    0x017D,    /*#	LATIN CAPITAL LETTER Z WITH CARON*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0x016A,    /*#	LATIN CAPITAL LETTER U WITH MACRON*/
    /*0xAF*/    0x014A,    /*#	LATIN CAPITAL LETTER ENG*/
    /*0xB0*/    0x00B0,    /*#	DEGREE SIGN*/
    /*0xB1*/    0x0105,    /*#	LATIN SMALL LETTER A WITH OGONEK*/
    /*0xB2*/    0x0113,    /*#	LATIN SMALL LETTER E WITH MACRON*/
    /*0xB3*/    0x0123,    /*#	LATIN SMALL LETTER G WITH CEDILLA*/
    /*0xB4*/    0x012B,    /*#	LATIN SMALL LETTER I WITH MACRON*/
    /*0xB5*/    0x0129,    /*#	LATIN SMALL LETTER I WITH TILDE*/
    /*0xB6*/    0x0137,    /*#	LATIN SMALL LETTER K WITH CEDILLA*/
    /*0xB7*/    0x00B7,    /*#	MIDDLE DOT*/
    /*0xB8*/    0x013C,    /*#	LATIN SMALL LETTER L WITH CEDILLA*/
    /*0xB9*/    0x0111,    /*#	LATIN SMALL LETTER D WITH STROKE*/
    /*0xBA*/    0x0161,    /*#	LATIN SMALL LETTER S WITH CARON*/
    /*0xBB*/    0x0167,    /*#	LATIN SMALL LETTER T WITH STROKE*/
    /*0xBC*/    0x017E,    /*#	LATIN SMALL LETTER Z WITH CARON*/
    /*0xBD*/    0x2015,    /*#	HORIZONTAL BAR*/
    /*0xBE*/    0x016B,    /*#	LATIN SMALL LETTER U WITH MACRON*/
    /*0xBF*/    0x014B,    /*#	LATIN SMALL LETTER ENG*/
    /*0xC0*/    0x0100,    /*#	LATIN CAPITAL LETTER A WITH MACRON*/
    /*0xC1*/    0x00C1,    /*#	LATIN CAPITAL LETTER A WITH ACUTE*/
    /*0xC2*/    0x00C2,    /*#	LATIN CAPITAL LETTER A WITH CIRCUMFLEX*/
    /*0xC3*/    0x00C3,    /*#	LATIN CAPITAL LETTER A WITH TILDE*/
    /*0xC4*/    0x00C4,    /*#	LATIN CAPITAL LETTER A WITH DIAERESIS*/
    /*0xC5*/    0x00C5,    /*#	LATIN CAPITAL LETTER A WITH RING ABOVE*/
    /*0xC6*/    0x00C6,    /*#	LATIN CAPITAL LETTER AE*/
    /*0xC7*/    0x012E,    /*#	LATIN CAPITAL LETTER I WITH OGONEK*/
    /*0xC8*/    0x010C,    /*#	LATIN CAPITAL LETTER C WITH CARON*/
    /*0xC9*/    0x00C9,    /*#	LATIN CAPITAL LETTER E WITH ACUTE*/
    /*0xCA*/    0x0118,    /*#	LATIN CAPITAL LETTER E WITH OGONEK*/
    /*0xCB*/    0x00CB,    /*#	LATIN CAPITAL LETTER E WITH DIAERESIS*/
    /*0xCC*/    0x0116,    /*#	LATIN CAPITAL LETTER E WITH DOT ABOVE*/
    /*0xCD*/    0x00CD,    /*#	LATIN CAPITAL LETTER I WITH ACUTE*/
    /*0xCE*/    0x00CE,    /*#	LATIN CAPITAL LETTER I WITH CIRCUMFLEX*/
    /*0xCF*/    0x00CF,    /*#	LATIN CAPITAL LETTER I WITH DIAERESIS*/
    /*0xD0*/    0x00D0,    /*#	LATIN CAPITAL LETTER ETH (Icelandic)*/
    /*0xD1*/    0x0145,    /*#	LATIN CAPITAL LETTER N WITH CEDILLA*/
    /*0xD2*/    0x014C,    /*#	LATIN CAPITAL LETTER O WITH MACRON*/
    /*0xD3*/    0x00D3,    /*#	LATIN CAPITAL LETTER O WITH ACUTE*/
    /*0xD4*/    0x00D4,    /*#	LATIN CAPITAL LETTER O WITH CIRCUMFLEX*/
    /*0xD5*/    0x00D5,    /*#	LATIN CAPITAL LETTER O WITH TILDE*/
    /*0xD6*/    0x00D6,    /*#	LATIN CAPITAL LETTER O WITH DIAERESIS*/
    /*0xD7*/    0x0168,    /*#	LATIN CAPITAL LETTER U WITH TILDE*/
    /*0xD8*/    0x00D8,    /*#	LATIN CAPITAL LETTER O WITH STROKE*/
    /*0xD9*/    0x0172,    /*#	LATIN CAPITAL LETTER U WITH OGONEK*/
    /*0xDA*/    0x00DA,    /*#	LATIN CAPITAL LETTER U WITH ACUTE*/
    /*0xDB*/    0x00DB,    /*#	LATIN CAPITAL LETTER U WITH CIRCUMFLEX*/
    /*0xDC*/    0x00DC,    /*#	LATIN CAPITAL LETTER U WITH DIAERESIS*/
    /*0xDD*/    0x00DD,    /*#	LATIN CAPITAL LETTER Y WITH ACUTE*/
    /*0xDE*/    0x00DE,    /*#	LATIN CAPITAL LETTER THORN (Icelandic)*/
    /*0xDF*/    0x00DF,    /*#	LATIN SMALL LETTER SHARP S (German)*/
    /*0xE0*/    0x0101,    /*#	LATIN SMALL LETTER A WITH MACRON*/
    /*0xE1*/    0x00E1,    /*#	LATIN SMALL LETTER A WITH ACUTE*/
    /*0xE2*/    0x00E2,    /*#	LATIN SMALL LETTER A WITH CIRCUMFLEX*/
    /*0xE3*/    0x00E3,    /*#	LATIN SMALL LETTER A WITH TILDE*/
    /*0xE4*/    0x00E4,    /*#	LATIN SMALL LETTER A WITH DIAERESIS*/
    /*0xE5*/    0x00E5,    /*#	LATIN SMALL LETTER A WITH RING ABOVE*/
    /*0xE6*/    0x00E6,    /*#	LATIN SMALL LETTER AE*/
    /*0xE7*/    0x012F,    /*#	LATIN SMALL LETTER I WITH OGONEK*/
    /*0xE8*/    0x010D,    /*#	LATIN SMALL LETTER C WITH CARON*/
    /*0xE9*/    0x00E9,    /*#	LATIN SMALL LETTER E WITH ACUTE*/
    /*0xEA*/    0x0119,    /*#	LATIN SMALL LETTER E WITH OGONEK*/
    /*0xEB*/    0x00EB,    /*#	LATIN SMALL LETTER E WITH DIAERESIS*/
    /*0xEC*/    0x0117,    /*#	LATIN SMALL LETTER E WITH DOT ABOVE*/
    /*0xED*/    0x00ED,    /*#	LATIN SMALL LETTER I WITH ACUTE*/
    /*0xEE*/    0x00EE,    /*#	LATIN SMALL LETTER I WITH CIRCUMFLEX*/
    /*0xEF*/    0x00EF,    /*#	LATIN SMALL LETTER I WITH DIAERESIS*/
    /*0xF0*/    0x00F0,    /*#	LATIN SMALL LETTER ETH (Icelandic)*/
    /*0xF1*/    0x0146,    /*#	LATIN SMALL LETTER N WITH CEDILLA*/
    /*0xF2*/    0x014D,    /*#	LATIN SMALL LETTER O WITH MACRON*/
    /*0xF3*/    0x00F3,    /*#	LATIN SMALL LETTER O WITH ACUTE*/
    /*0xF4*/    0x00F4,    /*#	LATIN SMALL LETTER O WITH CIRCUMFLEX*/
    /*0xF5*/    0x00F5,    /*#	LATIN SMALL LETTER O WITH TILDE*/
    /*0xF6*/    0x00F6,    /*#	LATIN SMALL LETTER O WITH DIAERESIS*/
    /*0xF7*/    0x0169,    /*#	LATIN SMALL LETTER U WITH TILDE*/
    /*0xF8*/    0x00F8,    /*#	LATIN SMALL LETTER O WITH STROKE*/
    /*0xF9*/    0x0173,    /*#	LATIN SMALL LETTER U WITH OGONEK*/
    /*0xFA*/    0x00FA,    /*#	LATIN SMALL LETTER U WITH ACUTE*/
    /*0xFB*/    0x00FB,    /*#	LATIN SMALL LETTER U WITH CIRCUMFLEX*/
    /*0xFC*/    0x00FC,    /*#	LATIN SMALL LETTER U WITH DIAERESIS*/
    /*0xFD*/    0x00FD,    /*#	LATIN SMALL LETTER Y WITH ACUTE*/
    /*0xFE*/    0x00FE,    /*#	LATIN SMALL LETTER THORN (Icelandic)*/
    /*0xFF*/    0x0138,    /*#	LATIN SMALL LETTER KRA*/
};

static UTF16_T aw2_8859_13_cnv_tbl[]=
{
    /*0xA1*/    0x201D,    /*#	RIGHT DOUBLE QUOTATION MARK*/
    /*0xA2*/    0x00A2,    /*#	CENT SIGN*/
    /*0xA3*/    0x00A3,    /*#	POUND SIGN*/
    /*0xA4*/    0x00A4,    /*#	CURRENCY SIGN*/
    /*0xA5*/    0x201E,    /*#	DOUBLE LOW-9 QUOTATION MARK*/
    /*0xA6*/    0x00A6,    /*#	BROKEN BAR*/
    /*0xA7*/    0x00A7,    /*#	SECTION SIGN*/
    /*0xA8*/    0x00D8,    /*#	LATIN CAPITAL LETTER O WITH STROKE*/
    /*0xA9*/    0x00A9,    /*#	COPYRIGHT SIGN*/
    /*0xAA*/    0x0156,    /*#	LATIN CAPITAL LETTER R WITH CEDILLA*/
    /*0xAB*/    0x00AB,    /*#	LEFT-POINTING DOUBLE ANGLE QUOTATION MARK*/
    /*0xAC*/    0x00AC,    /*#	NOT SIGN*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0x00AE,    /*#	REGISTERED SIGN*/
    /*0xAF*/    0x00C6,    /*#	LATIN CAPITAL LETTER AE*/
    /*0xB0*/    0x00B0,    /*#	DEGREE SIGN*/
    /*0xB1*/    0x00B1,    /*#	PLUS-MINUS SIGN*/
    /*0xB2*/    0x00B2,    /*#	SUPERSCRIPT TWO*/
    /*0xB3*/    0x00B3,    /*#	SUPERSCRIPT THREE*/
    /*0xB4*/    0x201C,    /*#	LEFT DOUBLE QUOTATION MARK*/
    /*0xB5*/    0x00B5,    /*#	MICRO SIGN*/
    /*0xB6*/    0x00B6,    /*#	PILCROW SIGN*/
    /*0xB7*/    0x00B7,    /*#	MIDDLE DOT*/
    /*0xB8*/    0x00F8,    /*#	LATIN SMALL LETTER O WITH STROKE*/
    /*0xB9*/    0x00B9,    /*#	SUPERSCRIPT ONE*/
    /*0xBA*/    0x0157,    /*#	LATIN SMALL LETTER R WITH CEDILLA*/
    /*0xBB*/    0x00BB,    /*#	RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK*/
    /*0xBC*/    0x00BC,    /*#	VULGAR FRACTION ONE QUARTER*/
    /*0xBD*/    0x00BD,    /*#	VULGAR FRACTION ONE HALF*/
    /*0xBE*/    0x00BE,    /*#	VULGAR FRACTION THREE QUARTERS*/
    /*0xBF*/    0x00E6,    /*#	LATIN SMALL LETTER AE*/
    /*0xC0*/    0x0104,    /*#	LATIN CAPITAL LETTER A WITH OGONEK*/
    /*0xC1*/    0x012E,    /*#	LATIN CAPITAL LETTER I WITH OGONEK*/
    /*0xC2*/    0x0100,    /*#	LATIN CAPITAL LETTER A WITH MACRON*/
    /*0xC3*/    0x0106,    /*#	LATIN CAPITAL LETTER C WITH ACUTE*/
    /*0xC4*/    0x00C4,    /*#	LATIN CAPITAL LETTER A WITH DIAERESIS*/
    /*0xC5*/    0x00C5,    /*#	LATIN CAPITAL LETTER A WITH RING ABOVE*/
    /*0xC6*/    0x0118,    /*#	LATIN CAPITAL LETTER E WITH OGONEK*/
    /*0xC7*/    0x0112,    /*#	LATIN CAPITAL LETTER E WITH MACRON*/
    /*0xC8*/    0x010C,    /*#	LATIN CAPITAL LETTER C WITH CARON*/
    /*0xC9*/    0x00C9,    /*#	LATIN CAPITAL LETTER E WITH ACUTE*/
    /*0xCA*/    0x0179,    /*#	LATIN CAPITAL LETTER Z WITH ACUTE*/
    /*0xCB*/    0x0116,    /*#	LATIN CAPITAL LETTER E WITH DOT ABOVE*/
    /*0xCC*/    0x0122,    /*#	LATIN CAPITAL LETTER G WITH CEDILLA*/
    /*0xCD*/    0x0136,    /*#	LATIN CAPITAL LETTER K WITH CEDILLA*/
    /*0xCE*/    0x012A,    /*#	LATIN CAPITAL LETTER I WITH MACRON*/
    /*0xCF*/    0x013B,    /*#	LATIN CAPITAL LETTER L WITH CEDILLA*/
    /*0xD0*/    0x0160,    /*#	LATIN CAPITAL LETTER S WITH CARON*/
    /*0xD1*/    0x0143,    /*#	LATIN CAPITAL LETTER N WITH ACUTE*/
    /*0xD2*/    0x0145,    /*#	LATIN CAPITAL LETTER N WITH CEDILLA*/
    /*0xD3*/    0x00D3,    /*#	LATIN CAPITAL LETTER O WITH ACUTE*/
    /*0xD4*/    0x014C,    /*#	LATIN CAPITAL LETTER O WITH MACRON*/
    /*0xD5*/    0x00D5,    /*#	LATIN CAPITAL LETTER O WITH TILDE*/
    /*0xD6*/    0x00D6,    /*#	LATIN CAPITAL LETTER O WITH DIAERESIS*/
    /*0xD7*/    0x00D7,    /*#	MULTIPLICATION SIGN*/
    /*0xD8*/    0x0172,    /*#	LATIN CAPITAL LETTER U WITH OGONEK*/
    /*0xD9*/    0x0141,    /*#	LATIN CAPITAL LETTER L WITH STROKE*/
    /*0xDA*/    0x015A,    /*#	LATIN CAPITAL LETTER S WITH ACUTE*/
    /*0xDB*/    0x016A,    /*#	LATIN CAPITAL LETTER U WITH MACRON*/
    /*0xDC*/    0x00DC,    /*#	LATIN CAPITAL LETTER U WITH DIAERESIS*/
    /*0xDD*/    0x017B,    /*#	LATIN CAPITAL LETTER Z WITH DOT ABOVE*/
    /*0xDE*/    0x017D,    /*#	LATIN CAPITAL LETTER Z WITH CARON*/
    /*0xDF*/    0x00DF,    /*#	LATIN SMALL LETTER SHARP S (German)*/
    /*0xE0*/    0x0105,    /*#	LATIN SMALL LETTER A WITH OGONEK*/
    /*0xE1*/    0x012F,    /*#	LATIN SMALL LETTER I WITH OGONEK*/
    /*0xE2*/    0x0101,    /*#	LATIN SMALL LETTER A WITH MACRON*/
    /*0xE3*/    0x0107,    /*#	LATIN SMALL LETTER C WITH ACUTE*/
    /*0xE4*/    0x00E4,    /*#	LATIN SMALL LETTER A WITH DIAERESIS*/
    /*0xE5*/    0x00E5,    /*#	LATIN SMALL LETTER A WITH RING ABOVE*/
    /*0xE6*/    0x0119,    /*#	LATIN SMALL LETTER E WITH OGONEK*/
    /*0xE7*/    0x0113,    /*#	LATIN SMALL LETTER E WITH MACRON*/
    /*0xE8*/    0x010D,    /*#	LATIN SMALL LETTER C WITH CARON*/
    /*0xE9*/    0x00E9,    /*#	LATIN SMALL LETTER E WITH ACUTE*/
    /*0xEA*/    0x017A,    /*#	LATIN SMALL LETTER Z WITH ACUTE*/
    /*0xEB*/    0x0117,    /*#	LATIN SMALL LETTER E WITH DOT ABOVE*/
    /*0xEC*/    0x0123,    /*#	LATIN SMALL LETTER G WITH CEDILLA*/
    /*0xED*/    0x0137,    /*#	LATIN SMALL LETTER K WITH CEDILLA*/
    /*0xEE*/    0x012B,    /*#	LATIN SMALL LETTER I WITH MACRON*/
    /*0xEF*/    0x013C,    /*#	LATIN SMALL LETTER L WITH CEDILLA*/
    /*0xF0*/    0x0161,    /*#	LATIN SMALL LETTER S WITH CARON*/
    /*0xF1*/    0x0144,    /*#	LATIN SMALL LETTER N WITH ACUTE*/
    /*0xF2*/    0x0146,    /*#	LATIN SMALL LETTER N WITH CEDILLA*/
    /*0xF3*/    0x00F3,    /*#	LATIN SMALL LETTER O WITH ACUTE*/
    /*0xF4*/    0x014D,    /*#	LATIN SMALL LETTER O WITH MACRON*/
    /*0xF5*/    0x00F5,    /*#	LATIN SMALL LETTER O WITH TILDE*/
    /*0xF6*/    0x00F6,    /*#	LATIN SMALL LETTER O WITH DIAERESIS*/
    /*0xF7*/    0x00F7,    /*#	DIVISION SIGN*/
    /*0xF8*/    0x0173,    /*#	LATIN SMALL LETTER U WITH OGONEK*/
    /*0xF9*/    0x0142,    /*#	LATIN SMALL LETTER L WITH STROKE*/
    /*0xFA*/    0x015B,    /*#	LATIN SMALL LETTER S WITH ACUTE*/
    /*0xFB*/    0x016B,    /*#	LATIN SMALL LETTER U WITH MACRON*/
    /*0xFC*/    0x00FC,    /*#	LATIN SMALL LETTER U WITH DIAERESIS*/
    /*0xFD*/    0x017C,    /*#	LATIN SMALL LETTER Z WITH DOT ABOVE*/
    /*0xFE*/    0x017E,    /*#	LATIN SMALL LETTER Z WITH CARON*/
    /*0xFF*/    0x2019,    /*#	RIGHT SINGLE QUOTATION MARK*/
};

static UTF16_T aw2_8859_14_cnv_tbl[]=
{
    /*0xA1*/    0x1E02,    /*#	LATIN CAPITAL LETTER B WITH DOT ABOVE*/
    /*0xA2*/    0x1E03,    /*#	LATIN SMALL LETTER B WITH DOT ABOVE*/
    /*0xA3*/    0x00A3,    /*#	POUND SIGN*/
    /*0xA4*/    0x010A,    /*#	LATIN CAPITAL LETTER C WITH DOT ABOVE*/
    /*0xA5*/    0x010B,    /*#	LATIN SMALL LETTER C WITH DOT ABOVE*/
    /*0xA6*/    0x1E0A,    /*#	LATIN CAPITAL LETTER D WITH DOT ABOVE*/
    /*0xA7*/    0x00A7,    /*#	SECTION SIGN*/
    /*0xA8*/    0x1E80,    /*#	LATIN CAPITAL LETTER W WITH GRAVE*/
    /*0xA9*/    0x00A9,    /*#	COPYRIGHT SIGN*/
    /*0xAA*/    0x1E82,    /*#	LATIN CAPITAL LETTER W WITH ACUTE*/
    /*0xAB*/    0x1E0B,    /*#	LATIN SMALL LETTER D WITH DOT ABOVE*/
    /*0xAC*/    0x1EF2,    /*#	LATIN CAPITAL LETTER Y WITH GRAVE*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0x00AE,    /*#	REGISTERED SIGN*/
    /*0xAF*/    0x0178,    /*#	LATIN CAPITAL LETTER Y WITH DIAERESIS*/
    /*0xB0*/    0x1E1E,    /*#	LATIN CAPITAL LETTER F WITH DOT ABOVE*/
    /*0xB1*/    0x1E1F,    /*#	LATIN SMALL LETTER F WITH DOT ABOVE*/
    /*0xB2*/    0x0120,    /*#	LATIN CAPITAL LETTER G WITH DOT ABOVE*/
    /*0xB3*/    0x0121,    /*#	LATIN SMALL LETTER G WITH DOT ABOVE*/
    /*0xB4*/    0x1E40,    /*#	LATIN CAPITAL LETTER M WITH DOT ABOVE*/
    /*0xB5*/    0x1E41,    /*#	LATIN SMALL LETTER M WITH DOT ABOVE*/
    /*0xB6*/    0x00B6,    /*#	PILCROW SIGN*/
    /*0xB7*/    0x1E56,    /*#	LATIN CAPITAL LETTER P WITH DOT ABOVE*/
    /*0xB8*/    0x1E81,    /*#	LATIN SMALL LETTER W WITH GRAVE*/
    /*0xB9*/    0x1E57,    /*#	LATIN SMALL LETTER P WITH DOT ABOVE*/
    /*0xBA*/    0x1E83,    /*#	LATIN SMALL LETTER W WITH ACUTE*/
    /*0xBB*/    0x1E60,    /*#	LATIN CAPITAL LETTER S WITH DOT ABOVE*/
    /*0xBC*/    0x1EF3,    /*#	LATIN SMALL LETTER Y WITH GRAVE*/
    /*0xBD*/    0x1E84,    /*#	LATIN CAPITAL LETTER W WITH DIAERESIS*/
    /*0xBE*/    0x1E85,    /*#	LATIN SMALL LETTER W WITH DIAERESIS*/
    /*0xBF*/    0x1E61,    /*#	LATIN SMALL LETTER S WITH DOT ABOVE*/
    /*0xC0*/    0x00C0,    /*#	LATIN CAPITAL LETTER A WITH GRAVE*/
    /*0xC1*/    0x00C1,    /*#	LATIN CAPITAL LETTER A WITH ACUTE*/
    /*0xC2*/    0x00C2,    /*#	LATIN CAPITAL LETTER A WITH CIRCUMFLEX*/
    /*0xC3*/    0x00C3,    /*#	LATIN CAPITAL LETTER A WITH TILDE*/
    /*0xC4*/    0x00C4,    /*#	LATIN CAPITAL LETTER A WITH DIAERESIS*/
    /*0xC5*/    0x00C5,    /*#	LATIN CAPITAL LETTER A WITH RING ABOVE*/
    /*0xC6*/    0x00C6,    /*#	LATIN CAPITAL LETTER AE*/
    /*0xC7*/    0x00C7,    /*#	LATIN CAPITAL LETTER C WITH CEDILLA*/
    /*0xC8*/    0x00C8,    /*#	LATIN CAPITAL LETTER E WITH GRAVE*/
    /*0xC9*/    0x00C9,    /*#	LATIN CAPITAL LETTER E WITH ACUTE*/
    /*0xCA*/    0x00CA,    /*#	LATIN CAPITAL LETTER E WITH CIRCUMFLEX*/
    /*0xCB*/    0x00CB,    /*#	LATIN CAPITAL LETTER E WITH DIAERESIS*/
    /*0xCC*/    0x00CC,    /*#	LATIN CAPITAL LETTER I WITH GRAVE*/
    /*0xCD*/    0x00CD,    /*#	LATIN CAPITAL LETTER I WITH ACUTE*/
    /*0xCE*/    0x00CE,    /*#	LATIN CAPITAL LETTER I WITH CIRCUMFLEX*/
    /*0xCF*/    0x00CF,    /*#	LATIN CAPITAL LETTER I WITH DIAERESIS*/
    /*0xD0*/    0x0174,    /*#	LATIN CAPITAL LETTER W WITH CIRCUMFLEX*/
    /*0xD1*/    0x00D1,    /*#	LATIN CAPITAL LETTER N WITH TILDE*/
    /*0xD2*/    0x00D2,    /*#	LATIN CAPITAL LETTER O WITH GRAVE*/
    /*0xD3*/    0x00D3,    /*#	LATIN CAPITAL LETTER O WITH ACUTE*/
    /*0xD4*/    0x00D4,    /*#	LATIN CAPITAL LETTER O WITH CIRCUMFLEX*/
    /*0xD5*/    0x00D5,    /*#	LATIN CAPITAL LETTER O WITH TILDE*/
    /*0xD6*/    0x00D6,    /*#	LATIN CAPITAL LETTER O WITH DIAERESIS*/
    /*0xD7*/    0x1E6A,    /*#	LATIN CAPITAL LETTER T WITH DOT ABOVE*/
    /*0xD8*/    0x00D8,    /*#	LATIN CAPITAL LETTER O WITH STROKE*/
    /*0xD9*/    0x00D9,    /*#	LATIN CAPITAL LETTER U WITH GRAVE*/
    /*0xDA*/    0x00DA,    /*#	LATIN CAPITAL LETTER U WITH ACUTE*/
    /*0xDB*/    0x00DB,    /*#	LATIN CAPITAL LETTER U WITH CIRCUMFLEX*/
    /*0xDC*/    0x00DC,    /*#	LATIN CAPITAL LETTER U WITH DIAERESIS*/
    /*0xDD*/    0x00DD,    /*#	LATIN CAPITAL LETTER Y WITH ACUTE*/
    /*0xDE*/    0x0176,    /*#	LATIN CAPITAL LETTER Y WITH CIRCUMFLEX*/
    /*0xDF*/    0x00DF,    /*#	LATIN SMALL LETTER SHARP S*/
    /*0xE0*/    0x00E0,    /*#	LATIN SMALL LETTER A WITH GRAVE*/
    /*0xE1*/    0x00E1,    /*#	LATIN SMALL LETTER A WITH ACUTE*/
    /*0xE2*/    0x00E2,    /*#	LATIN SMALL LETTER A WITH CIRCUMFLEX*/
    /*0xE3*/    0x00E3,    /*#	LATIN SMALL LETTER A WITH TILDE*/
    /*0xE4*/    0x00E4,    /*#	LATIN SMALL LETTER A WITH DIAERESIS*/
    /*0xE5*/    0x00E5,    /*#	LATIN SMALL LETTER A WITH RING ABOVE*/
    /*0xE6*/    0x00E6,    /*#	LATIN SMALL LETTER AE*/
    /*0xE7*/    0x00E7,    /*#	LATIN SMALL LETTER C WITH CEDILLA*/
    /*0xE8*/    0x00E8,    /*#	LATIN SMALL LETTER E WITH GRAVE*/
    /*0xE9*/    0x00E9,    /*#	LATIN SMALL LETTER E WITH ACUTE*/
    /*0xEA*/    0x00EA,    /*#	LATIN SMALL LETTER E WITH CIRCUMFLEX*/
    /*0xEB*/    0x00EB,    /*#	LATIN SMALL LETTER E WITH DIAERESIS*/
    /*0xEC*/    0x00EC,    /*#	LATIN SMALL LETTER I WITH GRAVE*/
    /*0xED*/    0x00ED,    /*#	LATIN SMALL LETTER I WITH ACUTE*/
    /*0xEE*/    0x00EE,    /*#	LATIN SMALL LETTER I WITH CIRCUMFLEX*/
    /*0xEF*/    0x00EF,    /*#	LATIN SMALL LETTER I WITH DIAERESIS*/
    /*0xF0*/    0x0175,    /*#	LATIN SMALL LETTER W WITH CIRCUMFLEX*/
    /*0xF1*/    0x00F1,    /*#	LATIN SMALL LETTER N WITH TILDE*/
    /*0xF2*/    0x00F2,    /*#	LATIN SMALL LETTER O WITH GRAVE*/
    /*0xF3*/    0x00F3,    /*#	LATIN SMALL LETTER O WITH ACUTE*/
    /*0xF4*/    0x00F4,    /*#	LATIN SMALL LETTER O WITH CIRCUMFLEX*/
    /*0xF5*/    0x00F5,    /*#	LATIN SMALL LETTER O WITH TILDE*/
    /*0xF6*/    0x00F6,    /*#	LATIN SMALL LETTER O WITH DIAERESIS*/
    /*0xF7*/    0x1E6B,    /*#	LATIN SMALL LETTER T WITH DOT ABOVE*/
    /*0xF8*/    0x00F8,    /*#	LATIN SMALL LETTER O WITH STROKE*/
    /*0xF9*/    0x00F9,    /*#	LATIN SMALL LETTER U WITH GRAVE*/
    /*0xFA*/    0x00FA,    /*#	LATIN SMALL LETTER U WITH ACUTE*/
    /*0xFB*/    0x00FB,    /*#	LATIN SMALL LETTER U WITH CIRCUMFLEX*/
    /*0xFC*/    0x00FC,    /*#	LATIN SMALL LETTER U WITH DIAERESIS*/
    /*0xFD*/    0x00FD,    /*#	LATIN SMALL LETTER Y WITH ACUTE*/
    /*0xFE*/    0x0177,    /*#	LATIN SMALL LETTER Y WITH CIRCUMFLEX*/
    /*0xFF*/    0x00FF,    /*#	LATIN SMALL LETTER Y WITH DIAERESIS*/
};

static UTF16_T aw2_8859_16_cnv_tbl[]=
{
    /*0xA1*/    0x0104,    /**/
    /*0xA2*/    0x0105,    /**/
    /*0xA3*/    0x0141,    /**/
    /*0xA4*/    0x20AC,    /**/
    /*0xA5*/    0x201E,    /**/
    /*0xA6*/    0x0160,    /**/
    /*0xA7*/    0x00A7,    /**/
    /*0xA8*/    0x0161,    /**/
    /*0xA9*/    0x00A9,    /**/
    /*0xAA*/    0x0218,    /**/
    /*0xAB*/    0x00AB,    /**/
    /*0xAC*/    0x0179,    /**/
    /*0xAD*/    0x00AD,    /**/
    /*0xAE*/    0x017A,    /**/
    /*0xAF*/    0x017B,    /**/
    /*0xB0*/    0x00B0,    /**/
    /*0xB1*/    0x00B1,    /**/
    /*0xB2*/    0x010C,    /**/
    /*0xB3*/    0x0142,    /**/
    /*0xB4*/    0x017D,    /**/
    /*0xB5*/    0x201D,    /**/
    /*0xB6*/    0x00B6,    /**/
    /*0xB7*/    0x00B7,    /**/
    /*0xB8*/    0x017E,    /**/
    /*0xB9*/    0x010D,    /**/
    /*0xBA*/    0x0219,    /**/
    /*0xBB*/    0x00BB,    /**/
    /*0xBC*/    0x0152,    /**/
    /*0xBD*/    0x0153,    /**/
    /*0xBE*/    0x0178,    /**/
    /*0xBF*/    0x017C,    /**/
    /*0xC0*/    0x00C0,    /**/
    /*0xC1*/    0x00C1,    /**/
    /*0xC2*/    0x00C2,    /**/
    /*0xC3*/    0x0102,    /**/
    /*0xC4*/    0x00C4,    /**/
    /*0xC5*/    0x0106,    /**/
    /*0xC6*/    0x00C6,    /**/
    /*0xC7*/    0x00C7,    /**/
    /*0xC8*/    0x00C8,    /**/
    /*0xC9*/    0x00C9,    /**/
    /*0xCA*/    0x00CA,    /**/
    /*0xCB*/    0x00CB,    /**/
    /*0xCC*/    0x00CC,    /**/
    /*0xCD*/    0x00CD,    /**/
    /*0xCE*/    0x00CE,    /**/
    /*0xCF*/    0x00CF,    /**/
    /*0xD0*/    0x0110,    /**/
    /*0xD1*/    0x0143,    /**/
    /*0xD2*/    0x00D2,    /**/
    /*0xD3*/    0x00D3,    /**/
    /*0xD4*/    0x00D4,    /**/
    /*0xD5*/    0x0150,    /**/
    /*0xD6*/    0x00D6,    /**/
    /*0xD7*/    0x015A,    /**/
    /*0xD8*/    0x0170,    /**/
    /*0xD9*/    0x00D9,    /**/
    /*0xDA*/    0x00DA,    /**/
    /*0xDB*/    0x00DB,    /**/
    /*0xDC*/    0x00DC,    /**/
    /*0xDD*/    0x0118,    /**/
    /*0xDE*/    0x021A,    /**/
    /*0xDF*/    0x00DF,    /**/
    /*0xE0*/    0x00E0,    /**/
    /*0xE1*/    0x00E1,    /**/
    /*0xE2*/    0x00E2,    /**/
    /*0xE3*/    0x0103,    /**/
    /*0xE4*/    0x00E4,    /**/
    /*0xE5*/    0x0107,    /**/
    /*0xE6*/    0x00E6,    /**/
    /*0xE7*/    0x00E7,    /**/
    /*0xE8*/    0x00E8,    /**/
    /*0xE9*/    0x00E9,    /**/
    /*0xEA*/    0x00EA,    /**/
    /*0xEB*/    0x00EB,    /**/
    /*0xEC*/    0x00EC,    /**/
    /*0xED*/    0x00ED,    /**/
    /*0xEE*/    0x00EE,    /**/
    /*0xEF*/    0x00EF,    /**/
    /*0xF0*/    0x0111,    /**/
    /*0xF1*/    0x0144,    /**/
    /*0xF2*/    0x00F2,    /**/
    /*0xF3*/    0x00F3,    /**/
    /*0xF4*/    0x00F4,    /**/
    /*0xF5*/    0x0151,    /**/
    /*0xF6*/    0x00F6,    /**/
    /*0xF7*/    0x015B,    /**/
    /*0xF8*/    0x0171,    /**/
    /*0xF9*/    0x00F9,    /**/
    /*0xFA*/    0x00FA,    /**/
    /*0xFB*/    0x00FB,    /**/
    /*0xFC*/    0x00FC,    /**/
    /*0xFD*/    0x0119,    /**/
    /*0xFE*/    0x021B,    /**/
    /*0xFF*/    0x00FF,    /**/
};

static UTF16_T aw2_8859_6_cnv_tbl[]=
{
    /*0xA1*/    0xFFFF,    /**/
    /*0xA2*/    0xFFFF,    /**/
    /*0xA3*/    0xFFFF,    /**/
    /*0xA4*/    0x00A4,    /*#	CURRENCY SIGN*/
    /*0xA5*/    0xFFFF,    /**/
    /*0xA6*/    0xFFFF,    /**/
    /*0xA7*/    0xFFFF,    /**/
    /*0xA8*/    0xFFFF,    /**/
    /*0xA9*/    0xFFFF,    /**/
    /*0xAA*/    0xFFFF,    /**/
    /*0xAB*/    0xFFFF,    /**/
    /*0xAC*/    0x060C,    /*#	ARABIC COMMA*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0xFFFF,    /**/
    /*0xAF*/    0xFFFF,    /**/
    /*0xB0*/    0xFFFF,    /**/
    /*0xB1*/    0xFFFF,    /**/
    /*0xB2*/    0xFFFF,    /**/
    /*0xB3*/    0xFFFF,    /**/
    /*0xB4*/    0xFFFF,    /**/
    /*0xB5*/    0xFFFF,    /**/
    /*0xB6*/    0xFFFF,    /**/
    /*0xB7*/    0xFFFF,    /**/
    /*0xB8*/    0xFFFF,    /**/
    /*0xB9*/    0xFFFF,    /**/
    /*0xBA*/    0xFFFF,    /**/
    /*0xBB*/    0x061B,    /*#	ARABIC SEMICOLON*/
    /*0xBC*/    0xFFFF,    /**/
    /*0xBD*/    0xFFFF,    /**/
    /*0xBE*/    0xFFFF,    /**/
    /*0xBF*/    0x061F,    /*#	ARABIC QUESTION MARK*/
    /*0xC0*/    0xFFFF,    /**/
    /*0xC1*/    0x0621,    /*#	ARABIC LETTER HAMZA*/
    /*0xC2*/    0x0622,    /*#	ARABIC LETTER ALEF WITH MADDA ABOVE*/
    /*0xC3*/    0x0623,    /*#	ARABIC LETTER ALEF WITH HAMZA ABOVE*/
    /*0xC4*/    0x0624,    /*#	ARABIC LETTER WAW WITH HAMZA ABOVE*/
    /*0xC5*/    0x0625,    /*#	ARABIC LETTER ALEF WITH HAMZA BELOW*/
    /*0xC6*/    0x0626,    /*#	ARABIC LETTER YEH WITH HAMZA ABOVE*/
    /*0xC7*/    0x0627,    /*#	ARABIC LETTER ALEF*/
    /*0xC8*/    0x0628,    /*#	ARABIC LETTER BEH*/
    /*0xC9*/    0x0629,    /*#	ARABIC LETTER TEH MARBUTA*/
    /*0xCA*/    0x062A,    /*#	ARABIC LETTER TEH*/
    /*0xCB*/    0x062B,    /*#	ARABIC LETTER THEH*/
    /*0xCC*/    0x062C,    /*#	ARABIC LETTER JEEM*/
    /*0xCD*/    0x062D,    /*#	ARABIC LETTER HAH*/
    /*0xCE*/    0x062E,    /*#	ARABIC LETTER KHAH*/
    /*0xCF*/    0x062F,    /*#	ARABIC LETTER DAL*/
    /*0xD0*/    0x0630,    /*#	ARABIC LETTER THAL*/
    /*0xD1*/    0x0631,    /*#	ARABIC LETTER REH*/
    /*0xD2*/    0x0632,    /*#	ARABIC LETTER ZAIN*/
    /*0xD3*/    0x0633,    /*#	ARABIC LETTER SEEN*/
    /*0xD4*/    0x0634,    /*#	ARABIC LETTER SHEEN*/
    /*0xD5*/    0x0635,    /*#	ARABIC LETTER SAD*/
    /*0xD6*/    0x0636,    /*#	ARABIC LETTER DAD*/
    /*0xD7*/    0x0637,    /*#	ARABIC LETTER TAH*/
    /*0xD8*/    0x0638,    /*#	ARABIC LETTER ZAH*/
    /*0xD9*/    0x0639,    /*#	ARABIC LETTER AIN*/
    /*0xDA*/    0x063A,    /*#	ARABIC LETTER GHAIN*/
    /*0xDB*/    0xFFFF,    /**/
    /*0xDC*/    0xFFFF,    /**/
    /*0xDD*/    0xFFFF,    /**/
    /*0xDE*/    0xFFFF,    /**/
    /*0xDF*/    0xFFFF,    /**/
    /*0xE0*/    0x0640,    /*#	ARABIC TATWEEL*/
    /*0xE1*/    0x0641,    /*#	ARABIC LETTER FEH*/
    /*0xE2*/    0x0642,    /*#	ARABIC LETTER QAF*/
    /*0xE3*/    0x0643,    /*#	ARABIC LETTER KAF*/
    /*0xE4*/    0x0644,    /*#	ARABIC LETTER LAM*/
    /*0xE5*/    0x0645,    /*#	ARABIC LETTER MEEM*/
    /*0xE6*/    0x0646,    /*#	ARABIC LETTER NOON*/
    /*0xE7*/    0x0647,    /*#	ARABIC LETTER HEH*/
    /*0xE8*/    0x0648,    /*#	ARABIC LETTER WAW*/
    /*0xE9*/    0x0649,    /*#	ARABIC LETTER ALEF MAKSURA*/
    /*0xEA*/    0x064A,    /*#	ARABIC LETTER YEH*/
    /*0xEB*/    0x064B,    /*#	ARABIC FATHATAN*/
    /*0xEC*/    0x064C,    /*#	ARABIC DAMMATAN*/
    /*0xED*/    0x064D,    /*#	ARABIC KASRATAN*/
    /*0xEE*/    0x064E,    /*#	ARABIC FATHA*/
    /*0xEF*/    0x064F,    /*#	ARABIC DAMMA*/
    /*0xF0*/    0x0650,    /*#	ARABIC KASRA*/
    /*0xF1*/    0x0651,    /*#	ARABIC SHADDA*/
    /*0xF2*/    0x0652,    /*#	ARABIC SUKUN*/
    /*0xF3*/    0xFFFF,    /**/
    /*0xF4*/    0xFFFF,    /**/
    /*0xF5*/    0xFFFF,    /**/
    /*0xF6*/    0xFFFF,    /**/
    /*0xF7*/    0xFFFF,    /**/
    /*0xF8*/    0xFFFF,    /**/
    /*0xF9*/    0xFFFF,    /**/
    /*0xFA*/    0xFFFF,    /**/
    /*0xFB*/    0xFFFF,    /**/
    /*0xFC*/    0xFFFF,    /**/
    /*0xFD*/    0xFFFF,    /**/
    /*0xFE*/    0xFFFF,    /**/
    /*0xFF*/    0xFFFF,    /**/
};

static UTF16_T aw2_8859_8_cnv_tbl[]=
{
    /*0xA1*/    0xFFFF,    /**/
    /*0xA2*/    0x00A2,    /*#	CENT SIGN*/
    /*0xA3*/    0x00A3,    /*#	POUND SIGN*/
    /*0xA4*/    0x00A4,    /*#	CURRENCY SIGN*/
    /*0xA5*/    0x00A5,    /*#	YEN SIGN*/
    /*0xA6*/    0x00A6,    /*#	BROKEN BAR*/
    /*0xA7*/    0x00A7,    /*#	SECTION SIGN*/
    /*0xA8*/    0x00A8,    /*#	DIAERESIS*/
    /*0xA9*/    0x00A9,    /*#	COPYRIGHT SIGN*/
    /*0xAA*/    0x00D7,    /*#	MULTIPLICATION SIGN*/
    /*0xAB*/    0x00AB,    /*#	LEFT-POINTING DOUBLE ANGLE QUOTATION MARK*/
    /*0xAC*/    0x00AC,    /*#	NOT SIGN*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0x00AE,    /*#	REGISTERED SIGN*/
    /*0xAF*/    0x00AF,    /*#	MACRON*/
    /*0xB0*/    0x00B0,    /*#	DEGREE SIGN*/
    /*0xB1*/    0x00B1,    /*#	PLUS-MINUS SIGN*/
    /*0xB2*/    0x00B2,    /*#	SUPERSCRIPT TWO*/
    /*0xB3*/    0x00B3,    /*#	SUPERSCRIPT THREE*/
    /*0xB4*/    0x00B4,    /*#	ACUTE ACCENT*/
    /*0xB5*/    0x00B5,    /*#	MICRO SIGN*/
    /*0xB6*/    0x00B6,    /*#	PILCROW SIGN*/
    /*0xB7*/    0x00B7,    /*#	MIDDLE DOT*/
    /*0xB8*/    0x00B8,    /*#	CEDILLA*/
    /*0xB9*/    0x00B9,    /*#	SUPERSCRIPT ONE*/
    /*0xBA*/    0x00F7,    /*#	DIVISION SIGN*/
    /*0xBB*/    0x00BB,    /*#	RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK*/
    /*0xBC*/    0x00BC,    /*#	VULGAR FRACTION ONE QUARTER*/
    /*0xBD*/    0x00BD,    /*#	VULGAR FRACTION ONE HALF*/
    /*0xBE*/    0x00BE,    /*#	VULGAR FRACTION THREE QUARTERS*/
    /*0xBF*/    0xFFFF,    /**/
    /*0xC0*/    0xFFFF,    /**/
    /*0xC1*/    0xFFFF,    /**/
    /*0xC2*/    0xFFFF,    /**/
    /*0xC3*/    0xFFFF,    /**/
    /*0xC4*/    0xFFFF,    /**/
    /*0xC5*/    0xFFFF,    /**/
    /*0xC6*/    0xFFFF,    /**/
    /*0xC7*/    0xFFFF,    /**/
    /*0xC8*/    0xFFFF,    /**/
    /*0xC9*/    0xFFFF,    /**/
    /*0xCA*/    0xFFFF,    /**/
    /*0xCB*/    0xFFFF,    /**/
    /*0xCC*/    0xFFFF,    /**/
    /*0xCD*/    0xFFFF,    /**/
    /*0xCE*/    0xFFFF,    /**/
    /*0xCF*/    0xFFFF,    /**/
    /*0xD0*/    0xFFFF,    /**/
    /*0xD1*/    0xFFFF,    /**/
    /*0xD2*/    0xFFFF,    /**/
    /*0xD3*/    0xFFFF,    /**/
    /*0xD4*/    0xFFFF,    /**/
    /*0xD5*/    0xFFFF,    /**/
    /*0xD6*/    0xFFFF,    /**/
    /*0xD7*/    0xFFFF,    /**/
    /*0xD8*/    0xFFFF,    /**/
    /*0xD9*/    0xFFFF,    /**/
    /*0xDA*/    0xFFFF,    /**/
    /*0xDB*/    0xFFFF,    /**/
    /*0xDC*/    0xFFFF,    /**/
    /*0xDD*/    0xFFFF,    /**/
    /*0xDE*/    0xFFFF,    /**/
    /*0xDF*/    0x2017,    /*#	DOUBLE LOW LINE*/
    /*0xE0*/    0x05D0,    /*#	HEBREW LETTER ALEF*/
    /*0xE1*/    0x05D1,    /*#	HEBREW LETTER BET*/
    /*0xE2*/    0x05D2,    /*#	HEBREW LETTER GIMEL*/
    /*0xE3*/    0x05D3,    /*#	HEBREW LETTER DALET*/
    /*0xE4*/    0x05D4,    /*#	HEBREW LETTER HE*/
    /*0xE5*/    0x05D5,    /*#	HEBREW LETTER VAV*/
    /*0xE6*/    0x05D6,    /*#	HEBREW LETTER ZAYIN*/
    /*0xE7*/    0x05D7,    /*#	HEBREW LETTER HET*/
    /*0xE8*/    0x05D8,    /*#	HEBREW LETTER TET*/
    /*0xE9*/    0x05D9,    /*#	HEBREW LETTER YOD*/
    /*0xEA*/    0x05DA,    /*#	HEBREW LETTER FINAL KAF*/
    /*0xEB*/    0x05DB,    /*#	HEBREW LETTER KAF*/
    /*0xEC*/    0x05DC,    /*#	HEBREW LETTER LAMED*/
    /*0xED*/    0x05DD,    /*#	HEBREW LETTER FINAL MEM*/
    /*0xEE*/    0x05DE,    /*#	HEBREW LETTER MEM*/
    /*0xEF*/    0x05DF,    /*#	HEBREW LETTER FINAL NUN*/
    /*0xF0*/    0x05E0,    /*#	HEBREW LETTER NUN*/
    /*0xF1*/    0x05E1,    /*#	HEBREW LETTER SAMEKH*/
    /*0xF2*/    0x05E2,    /*#	HEBREW LETTER AYIN*/
    /*0xF3*/    0x05E3,    /*#	HEBREW LETTER FINAL PE*/
    /*0xF4*/    0x05E4,    /*#	HEBREW LETTER PE*/
    /*0xF5*/    0x05E5,    /*#	HEBREW LETTER FINAL TSADI*/
    /*0xF6*/    0x05E6,    /*#	HEBREW LETTER TSADI*/
    /*0xF7*/    0x05E7,    /*#	HEBREW LETTER QOF*/
    /*0xF8*/    0x05E8,    /*#	HEBREW LETTER RESH*/
    /*0xF9*/    0x05E9,    /*#	HEBREW LETTER SHIN*/
    /*0xFA*/    0x05EA,    /*#	HEBREW LETTER TAV*/
    /*0xFB*/    0xFFFF,    /**/
    /*0xFC*/    0xFFFF,    /**/
    /*0xFD*/    0x200E,    /*#	LEFT-TO-RIGHT MARK*/
    /*0xFE*/    0x200F,    /*#	RIGHT-TO-LEFT MARK*/
    /*0xFF*/    0xFFFF,    /**/
};

static UTF16_T aw2_8859_11_cnv_tbl[]=
{
    /*0xA1*/    0x0E01,    /**/
    /*0xA2*/    0x0E02,    /**/
    /*0xA3*/    0x0E03,    /**/
    /*0xA4*/    0x0E04,    /**/
    /*0xA5*/    0x0E05,    /**/
    /*0xA6*/    0x0E06,    /**/
    /*0xA7*/    0x0E07,    /**/
    /*0xA8*/    0x0E08,    /**/
    /*0xA9*/    0x0E09,    /**/
    /*0xAA*/    0x0E0A,    /**/
    /*0xAB*/    0x0E0B,    /**/
    /*0xAC*/    0x0E0C,    /**/
    /*0xAD*/    0x0E0D,    /**/
    /*0xAE*/    0x0E0E,    /**/
    /*0xAF*/    0x0E0F,    /**/
    /*0xB0*/    0x0E10,    /**/
    /*0xB1*/    0x0E11,    /**/
    /*0xB2*/    0x0E12,    /**/
    /*0xB3*/    0x0E13,    /**/
    /*0xB4*/    0x0E14,    /**/
    /*0xB5*/    0x0E15,    /**/
    /*0xB6*/    0x0E16,    /**/
    /*0xB7*/    0x0E17,    /**/
    /*0xB8*/    0x0E18,    /**/
    /*0xB9*/    0x0E19,    /**/
    /*0xBA*/    0x0E1A,    /**/
    /*0xBB*/    0x0E1B,    /**/
    /*0xBC*/    0x0E1C,    /**/
    /*0xBD*/    0x0E1D,    /**/
    /*0xBE*/    0x0E1E,    /**/
    /*0xBF*/    0x0E1F,    /**/
    /*0xC0*/    0x0E20,    /**/
    /*0xC1*/    0x0E21,    /**/
    /*0xC2*/    0x0E22,    /**/
    /*0xC3*/    0x0E23,    /**/
    /*0xC4*/    0x0E24,    /**/
    /*0xC5*/    0x0E25,    /**/
    /*0xC6*/    0x0E26,    /**/
    /*0xC7*/    0x0E27,    /**/
    /*0xC8*/    0x0E28,    /**/
    /*0xC9*/    0x0E29,    /**/
    /*0xCA*/    0x0E2A,    /**/
    /*0xCB*/    0x0E2B,    /**/
    /*0xCC*/    0x0E2C,    /**/
    /*0xCD*/    0x0E2D,    /**/
    /*0xCE*/    0x0E2E,    /**/
    /*0xCF*/    0x0E2F,    /**/
    /*0xD0*/    0x0E30,    /**/
    /*0xD1*/    0x0E31,    /**/
    /*0xD2*/    0x0E32,    /**/
    /*0xD3*/    0x0E33,    /**/
    /*0xD4*/    0x0E34,    /**/
    /*0xD5*/    0x0E35,    /**/
    /*0xD6*/    0x0E36,    /**/
    /*0xD7*/    0x0E37,    /**/
    /*0xD8*/    0x0E38,    /**/
    /*0xD9*/    0x0E39,    /**/
    /*0xDA*/    0x0E3A,    /**/
    /*0xDB*/    0xFFFF,    /**/
    /*0xDC*/    0xFFFF,    /**/
    /*0xDD*/    0xFFFF,    /**/
    /*0xDE*/    0xFFFF,    /**/
    /*0xDF*/    0x0E3F,    /**/
    /*0xE0*/    0x0E40,    /**/
    /*0xE1*/    0x0E41,    /**/
    /*0xE2*/    0x0E42,    /**/
    /*0xE3*/    0x0E43,    /**/
    /*0xE4*/    0x0E44,    /**/
    /*0xE5*/    0x0E45,    /**/
    /*0xE6*/    0x0E46,    /**/
    /*0xE7*/    0x0E47,    /**/
    /*0xE8*/    0x0E48,    /**/
    /*0xE9*/    0x0E49,    /**/
    /*0xEA*/    0x0E4A,    /**/
    /*0xEB*/    0x0E4B,    /**/
    /*0xEC*/    0x0E4C,    /**/
    /*0xED*/    0x0E4D,    /**/
    /*0xEE*/    0x0E4E,    /**/
    /*0xEF*/    0x0E4F,    /**/
    /*0xF0*/    0x0E50,    /**/
    /*0xF1*/    0x0E51,    /**/
    /*0xF2*/    0x0E52,    /**/
    /*0xF3*/    0x0E53,    /**/
    /*0xF4*/    0x0E54,    /**/
    /*0xF5*/    0x0E55,    /**/
    /*0xF6*/    0x0E56,    /**/
    /*0xF7*/    0x0E57,    /**/
    /*0xF8*/    0x0E58,    /**/
    /*0xF9*/    0x0E59,    /**/
    /*0xFA*/    0x0E5A,    /**/
    /*0xFB*/    0x0E5B,    /**/
    /*0xFC*/    0xFFFF,    /**/
    /*0xFD*/    0xFFFF,    /**/
    /*0xFE*/    0xFFFF,    /**/
    /*0xFF*/    0xFFFF,    /**/
};

static UTF16_T aw2_8859_5_cnv_tbl[]=
{
    /*0xA1*/    0x0401,    /*#	CYRILLIC CAPITAL LETTER IO*/
    /*0xA2*/    0x0402,    /*#	CYRILLIC CAPITAL LETTER DJE*/
    /*0xA3*/    0x0403,    /*#	CYRILLIC CAPITAL LETTER GJE*/
    /*0xA4*/    0x0404,    /*#	CYRILLIC CAPITAL LETTER UKRAINIAN IE*/
    /*0xA5*/    0x0405,    /*#	CYRILLIC CAPITAL LETTER DZE*/
    /*0xA6*/    0x0406,    /*#	CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I*/
    /*0xA7*/    0x0407,    /*#	CYRILLIC CAPITAL LETTER YI*/
    /*0xA8*/    0x0408,    /*#	CYRILLIC CAPITAL LETTER JE*/
    /*0xA9*/    0x0409,    /*#	CYRILLIC CAPITAL LETTER LJE*/
    /*0xAA*/    0x040A,    /*#	CYRILLIC CAPITAL LETTER NJE*/
    /*0xAB*/    0x040B,    /*#	CYRILLIC CAPITAL LETTER TSHE*/
    /*0xAC*/    0x040C,    /*#	CYRILLIC CAPITAL LETTER KJE*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0x040E,    /*#	CYRILLIC CAPITAL LETTER SHORT U*/
    /*0xAF*/    0x040F,    /*#	CYRILLIC CAPITAL LETTER DZHE*/
    /*0xB0*/    0x0410,    /*#	CYRILLIC CAPITAL LETTER A*/
    /*0xB1*/    0x0411,    /*#	CYRILLIC CAPITAL LETTER BE*/
    /*0xB2*/    0x0412,    /*#	CYRILLIC CAPITAL LETTER VE*/
    /*0xB3*/    0x0413,    /*#	CYRILLIC CAPITAL LETTER GHE*/
    /*0xB4*/    0x0414,    /*#	CYRILLIC CAPITAL LETTER DE*/
    /*0xB5*/    0x0415,    /*#	CYRILLIC CAPITAL LETTER IE*/
    /*0xB6*/    0x0416,    /*#	CYRILLIC CAPITAL LETTER ZHE*/
    /*0xB7*/    0x0417,    /*#	CYRILLIC CAPITAL LETTER ZE*/
    /*0xB8*/    0x0418,    /*#	CYRILLIC CAPITAL LETTER I*/
    /*0xB9*/    0x0419,    /*#	CYRILLIC CAPITAL LETTER SHORT I*/
    /*0xBA*/    0x041A,    /*#	CYRILLIC CAPITAL LETTER KA*/
    /*0xBB*/    0x041B,    /*#	CYRILLIC CAPITAL LETTER EL*/
    /*0xBC*/    0x041C,    /*#	CYRILLIC CAPITAL LETTER EM*/
    /*0xBD*/    0x041D,    /*#	CYRILLIC CAPITAL LETTER EN*/
    /*0xBE*/    0x041E,    /*#	CYRILLIC CAPITAL LETTER O*/
    /*0xBF*/    0x041F,    /*#	CYRILLIC CAPITAL LETTER PE*/
    /*0xC0*/    0x0420,    /*#	CYRILLIC CAPITAL LETTER ER*/
    /*0xC1*/    0x0421,    /*#	CYRILLIC CAPITAL LETTER ES*/
    /*0xC2*/    0x0422,    /*#	CYRILLIC CAPITAL LETTER TE*/
    /*0xC3*/    0x0423,    /*#	CYRILLIC CAPITAL LETTER U*/
    /*0xC4*/    0x0424,    /*#	CYRILLIC CAPITAL LETTER EF*/
    /*0xC5*/    0x0425,    /*#	CYRILLIC CAPITAL LETTER HA*/
    /*0xC6*/    0x0426,    /*#	CYRILLIC CAPITAL LETTER TSE*/
    /*0xC7*/    0x0427,    /*#	CYRILLIC CAPITAL LETTER CHE*/
    /*0xC8*/    0x0428,    /*#	CYRILLIC CAPITAL LETTER SHA*/
    /*0xC9*/    0x0429,    /*#	CYRILLIC CAPITAL LETTER SHCHA*/
    /*0xCA*/    0x042A,    /*#	CYRILLIC CAPITAL LETTER HARD SIGN*/
    /*0xCB*/    0x042B,    /*#	CYRILLIC CAPITAL LETTER YERU*/
    /*0xCC*/    0x042C,    /*#	CYRILLIC CAPITAL LETTER SOFT SIGN*/
    /*0xCD*/    0x042D,    /*#	CYRILLIC CAPITAL LETTER E*/
    /*0xCE*/    0x042E,    /*#	CYRILLIC CAPITAL LETTER YU*/
    /*0xCF*/    0x042F,    /*#	CYRILLIC CAPITAL LETTER YA*/
    /*0xD0*/    0x0430,    /*#	CYRILLIC SMALL LETTER A*/
    /*0xD1*/    0x0431,    /*#	CYRILLIC SMALL LETTER BE*/
    /*0xD2*/    0x0432,    /*#	CYRILLIC SMALL LETTER VE*/
    /*0xD3*/    0x0433,    /*#	CYRILLIC SMALL LETTER GHE*/
    /*0xD4*/    0x0434,    /*#	CYRILLIC SMALL LETTER DE*/
    /*0xD5*/    0x0435,    /*#	CYRILLIC SMALL LETTER IE*/
    /*0xD6*/    0x0436,    /*#	CYRILLIC SMALL LETTER ZHE*/
    /*0xD7*/    0x0437,    /*#	CYRILLIC SMALL LETTER ZE*/
    /*0xD8*/    0x0438,    /*#	CYRILLIC SMALL LETTER I*/
    /*0xD9*/    0x0439,    /*#	CYRILLIC SMALL LETTER SHORT I*/
    /*0xDA*/    0x043A,    /*#	CYRILLIC SMALL LETTER KA*/
    /*0xDB*/    0x043B,    /*#	CYRILLIC SMALL LETTER EL*/
    /*0xDC*/    0x043C,    /*#	CYRILLIC SMALL LETTER EM*/
    /*0xDD*/    0x043D,    /*#	CYRILLIC SMALL LETTER EN*/
    /*0xDE*/    0x043E,    /*#	CYRILLIC SMALL LETTER O*/
    /*0xDF*/    0x043F,    /*#	CYRILLIC SMALL LETTER PE*/
    /*0xE0*/    0x0440,    /*#	CYRILLIC SMALL LETTER ER*/
    /*0xE1*/    0x0441,    /*#	CYRILLIC SMALL LETTER ES*/
    /*0xE2*/    0x0442,    /*#	CYRILLIC SMALL LETTER TE*/
    /*0xE3*/    0x0443,    /*#	CYRILLIC SMALL LETTER U*/
    /*0xE4*/    0x0444,    /*#	CYRILLIC SMALL LETTER EF*/
    /*0xE5*/    0x0445,    /*#	CYRILLIC SMALL LETTER HA*/
    /*0xE6*/    0x0446,    /*#	CYRILLIC SMALL LETTER TSE*/
    /*0xE7*/    0x0447,    /*#	CYRILLIC SMALL LETTER CHE*/
    /*0xE8*/    0x0448,    /*#	CYRILLIC SMALL LETTER SHA*/
    /*0xE9*/    0x0449,    /*#	CYRILLIC SMALL LETTER SHCHA*/
    /*0xEA*/    0x044A,    /*#	CYRILLIC SMALL LETTER HARD SIGN*/
    /*0xEB*/    0x044B,    /*#	CYRILLIC SMALL LETTER YERU*/
    /*0xEC*/    0x044C,    /*#	CYRILLIC SMALL LETTER SOFT SIGN*/
    /*0xED*/    0x044D,    /*#	CYRILLIC SMALL LETTER E*/
    /*0xEE*/    0x044E,    /*#	CYRILLIC SMALL LETTER YU*/
    /*0xEF*/    0x044F,    /*#	CYRILLIC SMALL LETTER YA*/
    /*0xF0*/    0x2116,    /*#	NUMERO SIGN*/
    /*0xF1*/    0x0451,    /*#	CYRILLIC SMALL LETTER IO*/
    /*0xF2*/    0x0452,    /*#	CYRILLIC SMALL LETTER DJE*/
    /*0xF3*/    0x0453,    /*#	CYRILLIC SMALL LETTER GJE*/
    /*0xF4*/    0x0454,    /*#	CYRILLIC SMALL LETTER UKRAINIAN IE*/
    /*0xF5*/    0x0455,    /*#	CYRILLIC SMALL LETTER DZE*/
    /*0xF6*/    0x0456,    /*#	CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I*/
    /*0xF7*/    0x0457,    /*#	CYRILLIC SMALL LETTER YI*/
    /*0xF8*/    0x0458,    /*#	CYRILLIC SMALL LETTER JE*/
    /*0xF9*/    0x0459,    /*#	CYRILLIC SMALL LETTER LJE*/
    /*0xFA*/    0x045A,    /*#	CYRILLIC SMALL LETTER NJE*/
    /*0xFB*/    0x045B,    /*#	CYRILLIC SMALL LETTER TSHE*/
    /*0xFC*/    0x045C,    /*#	CYRILLIC SMALL LETTER KJE*/
    /*0xFD*/    0x00A7,    /*#	SECTION SIGN*/
    /*0xFE*/    0x045E,    /*#	CYRILLIC SMALL LETTER SHORT U*/
    /*0xFF*/    0x045F,    /*#	CYRILLIC SMALL LETTER DZHE*/
};

static UTF16_T aw2_8859_9_cnv_tbl[]=
{
    /*0xA1*/    0x00A1,    /*#	INVERTED EXCLAMATION MARK*/
    /*0xA2*/    0x00A2,    /*#	CENT SIGN*/
    /*0xA3*/    0x00A3,    /*#	POUND SIGN*/
    /*0xA4*/    0x00A4,    /*#	CURRENCY SIGN*/
    /*0xA5*/    0x00A5,    /*#	YEN SIGN*/
    /*0xA6*/    0x00A6,    /*#	BROKEN BAR*/
    /*0xA7*/    0x00A7,    /*#	SECTION SIGN*/
    /*0xA8*/    0x00A8,    /*#	DIAERESIS*/
    /*0xA9*/    0x00A9,    /*#	COPYRIGHT SIGN*/
    /*0xAA*/    0x00AA,    /*#	FEMININE ORDINAL INDICATOR*/
    /*0xAB*/    0x00AB,    /*#	LEFT-POINTING DOUBLE ANGLE QUOTATION MARK*/
    /*0xAC*/    0x00AC,    /*#	NOT SIGN*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0x00AE,    /*#	REGISTERED SIGN*/
    /*0xAF*/    0x00AF,    /*#	MACRON*/
    /*0xB0*/    0x00B0,    /*#	DEGREE SIGN*/
    /*0xB1*/    0x00B1,    /*#	PLUS-MINUS SIGN*/
    /*0xB2*/    0x00B2,    /*#	SUPERSCRIPT TWO*/
    /*0xB3*/    0x00B3,    /*#	SUPERSCRIPT THREE*/
    /*0xB4*/    0x00B4,    /*#	ACUTE ACCENT*/
    /*0xB5*/    0x00B5,    /*#	MICRO SIGN*/
    /*0xB6*/    0x00B6,    /*#	PILCROW SIGN*/
    /*0xB7*/    0x00B7,    /*#	MIDDLE DOT*/
    /*0xB8*/    0x00B8,    /*#	CEDILLA*/
    /*0xB9*/    0x00B9,    /*#	SUPERSCRIPT ONE*/
    /*0xBA*/    0x00BA,    /*#	MASCULINE ORDINAL INDICATOR*/
    /*0xBB*/    0x00BB,    /*#	RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK*/
    /*0xBC*/    0x00BC,    /*#	VULGAR FRACTION ONE QUARTER*/
    /*0xBD*/    0x00BD,    /*#	VULGAR FRACTION ONE HALF*/
    /*0xBE*/    0x00BE,    /*#	VULGAR FRACTION THREE QUARTERS*/
    /*0xBF*/    0x00BF,    /*#	INVERTED QUESTION MARK*/
    /*0xC0*/    0x00C0,    /*#	LATIN CAPITAL LETTER A WITH GRAVE*/
    /*0xC1*/    0x00C1,    /*#	LATIN CAPITAL LETTER A WITH ACUTE*/
    /*0xC2*/    0x00C2,    /*#	LATIN CAPITAL LETTER A WITH CIRCUMFLEX*/
    /*0xC3*/    0x00C3,    /*#	LATIN CAPITAL LETTER A WITH TILDE*/
    /*0xC4*/    0x00C4,    /*#	LATIN CAPITAL LETTER A WITH DIAERESIS*/
    /*0xC5*/    0x00C5,    /*#	LATIN CAPITAL LETTER A WITH RING ABOVE*/
    /*0xC6*/    0x00C6,    /*#	LATIN CAPITAL LETTER AE*/
    /*0xC7*/    0x00C7,    /*#	LATIN CAPITAL LETTER C WITH CEDILLA*/
    /*0xC8*/    0x00C8,    /*#	LATIN CAPITAL LETTER E WITH GRAVE*/
    /*0xC9*/    0x00C9,    /*#	LATIN CAPITAL LETTER E WITH ACUTE*/
    /*0xCA*/    0x00CA,    /*#	LATIN CAPITAL LETTER E WITH CIRCUMFLEX*/
    /*0xCB*/    0x00CB,    /*#	LATIN CAPITAL LETTER E WITH DIAERESIS*/
    /*0xCC*/    0x00CC,    /*#	LATIN CAPITAL LETTER I WITH GRAVE*/
    /*0xCD*/    0x00CD,    /*#	LATIN CAPITAL LETTER I WITH ACUTE*/
    /*0xCE*/    0x00CE,    /*#	LATIN CAPITAL LETTER I WITH CIRCUMFLEX*/
    /*0xCF*/    0x00CF,    /*#	LATIN CAPITAL LETTER I WITH DIAERESIS*/
    /*0xD0*/    0x011E,    /*#	LATIN CAPITAL LETTER G WITH BREVE*/
    /*0xD1*/    0x00D1,    /*#	LATIN CAPITAL LETTER N WITH TILDE*/
    /*0xD2*/    0x00D2,    /*#	LATIN CAPITAL LETTER O WITH GRAVE*/
    /*0xD3*/    0x00D3,    /*#	LATIN CAPITAL LETTER O WITH ACUTE*/
    /*0xD4*/    0x00D4,    /*#	LATIN CAPITAL LETTER O WITH CIRCUMFLEX*/
    /*0xD5*/    0x00D5,    /*#	LATIN CAPITAL LETTER O WITH TILDE*/
    /*0xD6*/    0x00D6,    /*#	LATIN CAPITAL LETTER O WITH DIAERESIS*/
    /*0xD7*/    0x00D7,    /*#	MULTIPLICATION SIGN*/
    /*0xD8*/    0x00D8,    /*#	LATIN CAPITAL LETTER O WITH STROKE*/
    /*0xD9*/    0x00D9,    /*#	LATIN CAPITAL LETTER U WITH GRAVE*/
    /*0xDA*/    0x00DA,    /*#	LATIN CAPITAL LETTER U WITH ACUTE*/
    /*0xDB*/    0x00DB,    /*#	LATIN CAPITAL LETTER U WITH CIRCUMFLEX*/
    /*0xDC*/    0x00DC,    /*#	LATIN CAPITAL LETTER U WITH DIAERESIS*/
    /*0xDD*/    0x0130,    /*#	LATIN CAPITAL LETTER I WITH DOT ABOVE*/
    /*0xDE*/    0x015E,    /*#	LATIN CAPITAL LETTER S WITH CEDILLA*/
    /*0xDF*/    0x00DF,    /*#	LATIN SMALL LETTER SHARP S*/
    /*0xE0*/    0x00E0,    /*#	LATIN SMALL LETTER A WITH GRAVE*/
    /*0xE1*/    0x00E1,    /*#	LATIN SMALL LETTER A WITH ACUTE*/
    /*0xE2*/    0x00E2,    /*#	LATIN SMALL LETTER A WITH CIRCUMFLEX*/
    /*0xE3*/    0x00E3,    /*#	LATIN SMALL LETTER A WITH TILDE*/
    /*0xE4*/    0x00E4,    /*#	LATIN SMALL LETTER A WITH DIAERESIS*/
    /*0xE5*/    0x00E5,    /*#	LATIN SMALL LETTER A WITH RING ABOVE*/
    /*0xE6*/    0x00E6,    /*#	LATIN SMALL LETTER AE*/
    /*0xE7*/    0x00E7,    /*#	LATIN SMALL LETTER C WITH CEDILLA*/
    /*0xE8*/    0x00E8,    /*#	LATIN SMALL LETTER E WITH GRAVE*/
    /*0xE9*/    0x00E9,    /*#	LATIN SMALL LETTER E WITH ACUTE*/
    /*0xEA*/    0x00EA,    /*#	LATIN SMALL LETTER E WITH CIRCUMFLEX*/
    /*0xEB*/    0x00EB,    /*#	LATIN SMALL LETTER E WITH DIAERESIS*/
    /*0xEC*/    0x00EC,    /*#	LATIN SMALL LETTER I WITH GRAVE*/
    /*0xED*/    0x00ED,    /*#	LATIN SMALL LETTER I WITH ACUTE*/
    /*0xEE*/    0x00EE,    /*#	LATIN SMALL LETTER I WITH CIRCUMFLEX*/
    /*0xEF*/    0x00EF,    /*#	LATIN SMALL LETTER I WITH DIAERESIS*/
    /*0xF0*/    0x011F,    /*#	LATIN SMALL LETTER G WITH BREVE*/
    /*0xF1*/    0x00F1,    /*#	LATIN SMALL LETTER N WITH TILDE*/
    /*0xF2*/    0x00F2,    /*#	LATIN SMALL LETTER O WITH GRAVE*/
    /*0xF3*/    0x00F3,    /*#	LATIN SMALL LETTER O WITH ACUTE*/
    /*0xF4*/    0x00F4,    /*#	LATIN SMALL LETTER O WITH CIRCUMFLEX*/
    /*0xF5*/    0x00F5,    /*#	LATIN SMALL LETTER O WITH TILDE*/
    /*0xF6*/    0x00F6,    /*#	LATIN SMALL LETTER O WITH DIAERESIS*/
    /*0xF7*/    0x00F7,    /*#	DIVISION SIGN*/
    /*0xF8*/    0x00F8,    /*#	LATIN SMALL LETTER O WITH STROKE*/
    /*0xF9*/    0x00F9,    /*#	LATIN SMALL LETTER U WITH GRAVE*/
    /*0xFA*/    0x00FA,    /*#	LATIN SMALL LETTER U WITH ACUTE*/
    /*0xFB*/    0x00FB,    /*#	LATIN SMALL LETTER U WITH CIRCUMFLEX*/
    /*0xFC*/    0x00FC,    /*#	LATIN SMALL LETTER U WITH DIAERESIS*/
    /*0xFD*/    0x0131,    /*#	LATIN SMALL LETTER DOTLESS I*/
    /*0xFE*/    0x015F,    /*#	LATIN SMALL LETTER S WITH CEDILLA*/
    /*0xFF*/    0x00FF,    /*#	LATIN SMALL LETTER Y WITH DIAERESIS*/
};

static UTF16_T aw2_8859_15_cnv_tbl[]=
{
    /*0xA1*/    0x00A1,    /*#	INVERTED EXCLAMATION MARK*/
    /*0xA2*/    0x00A2,    /*#	CENT SIGN*/
    /*0xA3*/    0x00A3,    /*#	POUND SIGN*/
    /*0xA4*/    0x20AC,    /*#	EURO SIGN*/
    /*0xA5*/    0x00A5,    /*#	YEN SIGN*/
    /*0xA6*/    0x0160,    /*#	LATIN CAPITAL LETTER S WITH CARON*/
    /*0xA7*/    0x00A7,    /*#	SECTION SIGN*/
    /*0xA8*/    0x0161,    /*#	LATIN SMALL LETTER S WITH CARON*/
    /*0xA9*/    0x00A9,    /*#	COPYRIGHT SIGN*/
    /*0xAA*/    0x00AA,    /*#	FEMININE ORDINAL INDICATOR*/
    /*0xAB*/    0x00AB,    /*#	LEFT-POINTING DOUBLE ANGLE QUOTATION MARK*/
    /*0xAC*/    0x00AC,    /*#	NOT SIGN*/
    /*0xAD*/    0x00AD,    /*#	SOFT HYPHEN*/
    /*0xAE*/    0x00AE,    /*#	REGISTERED SIGN*/
    /*0xAF*/    0x00AF,    /*#	MACRON*/
    /*0xB0*/    0x00B0,    /*#	DEGREE SIGN*/
    /*0xB1*/    0x00B1,    /*#	PLUS-MINUS SIGN*/
    /*0xB2*/    0x00B2,    /*#	SUPERSCRIPT TWO*/
    /*0xB3*/    0x00B3,    /*#	SUPERSCRIPT THREE*/
    /*0xB4*/    0x017D,    /*#	LATIN CAPITAL LETTER Z WITH CARON*/
    /*0xB5*/    0x00B5,    /*#	MICRO SIGN*/
    /*0xB6*/    0x00B6,    /*#	PILCROW SIGN*/
    /*0xB7*/    0x00B7,    /*#	MIDDLE DOT*/
    /*0xB8*/    0x017E,    /*#	LATIN SMALL LETTER Z WITH CARON*/
    /*0xB9*/    0x00B9,    /*#	SUPERSCRIPT ONE*/
    /*0xBA*/    0x00BA,    /*#	MASCULINE ORDINAL INDICATOR*/
    /*0xBB*/    0x00BB,    /*#	RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK*/
    /*0xBC*/    0x0152,    /*#	LATIN CAPITAL LIGATURE OE*/
    /*0xBD*/    0x0153,    /*#	LATIN SMALL LIGATURE OE*/
    /*0xBE*/    0x0178,    /*#	LATIN CAPITAL LETTER Y WITH DIAERESIS*/
    /*0xBF*/    0x00BF,    /*#	INVERTED QUESTION MARK*/
    /*0xC0*/    0x00C0,    /*#	LATIN CAPITAL LETTER A WITH GRAVE*/
    /*0xC1*/    0x00C1,    /*#	LATIN CAPITAL LETTER A WITH ACUTE*/
    /*0xC2*/    0x00C2,    /*#	LATIN CAPITAL LETTER A WITH CIRCUMFLEX*/
    /*0xC3*/    0x00C3,    /*#	LATIN CAPITAL LETTER A WITH TILDE*/
    /*0xC4*/    0x00C4,    /*#	LATIN CAPITAL LETTER A WITH DIAERESIS*/
    /*0xC5*/    0x00C5,    /*#	LATIN CAPITAL LETTER A WITH RING ABOVE*/
    /*0xC6*/    0x00C6,    /*#	LATIN CAPITAL LETTER AE*/
    /*0xC7*/    0x00C7,    /*#	LATIN CAPITAL LETTER C WITH CEDILLA*/
    /*0xC8*/    0x00C8,    /*#	LATIN CAPITAL LETTER E WITH GRAVE*/
    /*0xC9*/    0x00C9,    /*#	LATIN CAPITAL LETTER E WITH ACUTE*/
    /*0xCA*/    0x00CA,    /*#	LATIN CAPITAL LETTER E WITH CIRCUMFLEX*/
    /*0xCB*/    0x00CB,    /*#	LATIN CAPITAL LETTER E WITH DIAERESIS*/
    /*0xCC*/    0x00CC,    /*#	LATIN CAPITAL LETTER I WITH GRAVE*/
    /*0xCD*/    0x00CD,    /*#	LATIN CAPITAL LETTER I WITH ACUTE*/
    /*0xCE*/    0x00CE,    /*#	LATIN CAPITAL LETTER I WITH CIRCUMFLEX*/
    /*0xCF*/    0x00CF,    /*#	LATIN CAPITAL LETTER I WITH DIAERESIS*/
    /*0xD0*/    0x00D0,    /*#	LATIN CAPITAL LETTER ETH*/
    /*0xD1*/    0x00D1,    /*#	LATIN CAPITAL LETTER N WITH TILDE*/
    /*0xD2*/    0x00D2,    /*#	LATIN CAPITAL LETTER O WITH GRAVE*/
    /*0xD3*/    0x00D3,    /*#	LATIN CAPITAL LETTER O WITH ACUTE*/
    /*0xD4*/    0x00D4,    /*#	LATIN CAPITAL LETTER O WITH CIRCUMFLEX*/
    /*0xD5*/    0x00D5,    /*#	LATIN CAPITAL LETTER O WITH TILDE*/
    /*0xD6*/    0x00D6,    /*#	LATIN CAPITAL LETTER O WITH DIAERESIS*/
    /*0xD7*/    0x00D7,    /*#	MULTIPLICATION SIGN*/
    /*0xD8*/    0x00D8,    /*#	LATIN CAPITAL LETTER O WITH STROKE*/
    /*0xD9*/    0x00D9,    /*#	LATIN CAPITAL LETTER U WITH GRAVE*/
    /*0xDA*/    0x00DA,    /*#	LATIN CAPITAL LETTER U WITH ACUTE*/
    /*0xDB*/    0x00DB,    /*#	LATIN CAPITAL LETTER U WITH CIRCUMFLEX*/
    /*0xDC*/    0x00DC,    /*#	LATIN CAPITAL LETTER U WITH DIAERESIS*/
    /*0xDD*/    0x00DD,    /*#	LATIN CAPITAL LETTER Y WITH ACUTE*/
    /*0xDE*/    0x00DE,    /*#	LATIN CAPITAL LETTER THORN*/
    /*0xDF*/    0x00DF,    /*#	LATIN SMALL LETTER SHARP S*/
    /*0xE0*/    0x00E0,    /*#	LATIN SMALL LETTER A WITH GRAVE*/
    /*0xE1*/    0x00E1,    /*#	LATIN SMALL LETTER A WITH ACUTE*/
    /*0xE2*/    0x00E2,    /*#	LATIN SMALL LETTER A WITH CIRCUMFLEX*/
    /*0xE3*/    0x00E3,    /*#	LATIN SMALL LETTER A WITH TILDE*/
    /*0xE4*/    0x00E4,    /*#	LATIN SMALL LETTER A WITH DIAERESIS*/
    /*0xE5*/    0x00E5,    /*#	LATIN SMALL LETTER A WITH RING ABOVE*/
    /*0xE6*/    0x00E6,    /*#	LATIN SMALL LETTER AE*/
    /*0xE7*/    0x00E7,    /*#	LATIN SMALL LETTER C WITH CEDILLA*/
    /*0xE8*/    0x00E8,    /*#	LATIN SMALL LETTER E WITH GRAVE*/
    /*0xE9*/    0x00E9,    /*#	LATIN SMALL LETTER E WITH ACUTE*/
    /*0xEA*/    0x00EA,    /*#	LATIN SMALL LETTER E WITH CIRCUMFLEX*/
    /*0xEB*/    0x00EB,    /*#	LATIN SMALL LETTER E WITH DIAERESIS*/
    /*0xEC*/    0x00EC,    /*#	LATIN SMALL LETTER I WITH GRAVE*/
    /*0xED*/    0x00ED,    /*#	LATIN SMALL LETTER I WITH ACUTE*/
    /*0xEE*/    0x00EE,    /*#	LATIN SMALL LETTER I WITH CIRCUMFLEX*/
    /*0xEF*/    0x00EF,    /*#	LATIN SMALL LETTER I WITH DIAERESIS*/
    /*0xF0*/    0x00F0,    /*#	LATIN SMALL LETTER ETH*/
    /*0xF1*/    0x00F1,    /*#	LATIN SMALL LETTER N WITH TILDE*/
    /*0xF2*/    0x00F2,    /*#	LATIN SMALL LETTER O WITH GRAVE*/
    /*0xF3*/    0x00F3,    /*#	LATIN SMALL LETTER O WITH ACUTE*/
    /*0xF4*/    0x00F4,    /*#	LATIN SMALL LETTER O WITH CIRCUMFLEX*/
    /*0xF5*/    0x00F5,    /*#	LATIN SMALL LETTER O WITH TILDE*/
    /*0xF6*/    0x00F6,    /*#	LATIN SMALL LETTER O WITH DIAERESIS*/
    /*0xF7*/    0x00F7,    /*#	DIVISION SIGN*/
    /*0xF8*/    0x00F8,    /*#	LATIN SMALL LETTER O WITH STROKE*/
    /*0xF9*/    0x00F9,    /*#	LATIN SMALL LETTER U WITH GRAVE*/
    /*0xFA*/    0x00FA,    /*#	LATIN SMALL LETTER U WITH ACUTE*/
    /*0xFB*/    0x00FB,    /*#	LATIN SMALL LETTER U WITH CIRCUMFLEX*/
    /*0xFC*/    0x00FC,    /*#	LATIN SMALL LETTER U WITH DIAERESIS*/
    /*0xFD*/    0x00FD,    /*#	LATIN SMALL LETTER Y WITH ACUTE*/
    /*0xFE*/    0x00FE,    /*#	LATIN SMALL LETTER THORN*/
    /*0xFF*/    0x00FF,    /*#	LATIN SMALL LETTER Y WITH DIAERESIS*/
};



static UTF16_T aw2_8859_12_cnv_tbl[]=
{
    /*0xA1*/    0x1EAF,    /*LATIN SMALL LETTER A WITH BREVE AND ACUTE*/
    /*0xA2*/    0x1EB1,    /*LATIN SMALL LETTER A WITH BREVE AND GRAVE*/
    /*0xA3*/    0x1EB7,    /*LATIN SMALL LETTER A WITH BREVE AND DOT BELOW*/
    /*0xA4*/    0x1EA5,    /*LATIN SMALL LETTER A WITH CIRCUMFLEX AND ACUTE*/
    /*0xA5*/    0x1EA7,    /*LATIN SMALL LETTER A WITH CIRCUMFLEX AND GRAVE*/
    /*0xA6*/    0x1EA8,    /*LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND HOOK ABOVE*/
    /*0xA7*/    0x1EAD,    /*LATIN SMALL LETTER A WITH CIRCUMFLEX AND DOT BELOW*/
    /*0xA8*/    0x1EBD,    /*LATIN SMALL LETTER E WITH TILDE*/
    /*0xA9*/    0x1EB9,    /*LATIN SMALL LETTER E WITH DOT BELOW*/
    /*0xAA*/    0x1EBF,    /*LATIN SMALL LETTER E WITH CIRCUMFLEX AND ACUTE*/
    /*0xAB*/    0x1EC1,    /*LATIN SMALL LETTER E WITH CIRCUMFLEX AND GRAVE*/
    /*0xAC*/    0x1EC3,    /*LATIN SMALL LETTER E WITH CIRCUMFLEX AND HOOK ABOVE*/
    /*0xAD*/    0x1EC5,    /*LATIN SMALL LETTER E WITH CIRCUMFLEX AND TILDE*/
    /*0xAE*/    0x1EC7,    /*LATIN SMALL LETTER E WITH CIRCUMFLEX AND DOT BELOW*/
    /*0xAF*/    0x1ED1,    /*LATIN SMALL LETTER O WITH CIRCUMFLEX AND ACUTE*/
    /*0xB0*/    0x1ED3,    /*LATIN SMALL LETTER O WITH CIRCUMFLEX AND GRAVE*/
    /*0xB1*/    0x1ED5,    /*LATIN SMALL LETTER O WITH CIRCUMFLEX AND HOOK ABOVE*/
    /*0xB2*/    0x1ED7,    /*LATIN SMALL LETTER O WITH CIRCUMFLEX AND TILDE*/
    /*0xB3*/    0x1EE0,    /*LATIN CAPITAL LETTER O WITH HORN AND TILDE*/
    /*0xB4*/    0x01A0,    /*LATIN CAPITAL LETTER O WITH HORN*/
    /*0xB5*/    0x1ED9,    /*LATIN SMALL LETTER O WITH CIRCUMFLEX AND DOT BELOW*/
    /*0xB6*/    0x1EDD,    /*LATIN SMALL LETTER O WITH HORN AND GRAVE*/
    /*0xB7*/    0x1EDF,    /*LATIN SMALL LETTER O WITH HORN AND HOOK ABOVE*/
    /*0xB8*/    0x1ECB,    /*LATIN SMALL LETTER I WITH DOT BELOW*/
    /*0xB9*/    0x1EF0,    /*LATIN CAPITAL LETTER U WITH HORN AND DOT BELOW*/
    /*0xBA*/    0x1EE8,    /*LATIN CAPITAL LETTER U WITH HORN AND ACUTE*/
    /*0xBB*/    0x1EEA,    /*LATIN CAPITAL LETTER U WITH HORN AND GRAVE*/
    /*0xBC*/    0x1EEC,    /*LATIN CAPITAL LETTER U WITH HORN AND HOOK ABOVE*/
    /*0xBD*/    0x01A1,    /*LATIN SMALL LETTER O WITH HORN*/
    /*0xBE*/    0x1EDB,    /*LATIN SMALL LETTER O WITH HORN AND ACUTE*/
    /*0xBF*/    0x01AF,    /*LATIN CAPITAL LETTER U WITH HORN*/
    /*0xC0*/    0x00C0,    /*LATIN CAPITAL LETTER A WITH GRAVE*/
    /*0xC1*/    0x00C1,    /*LATIN CAPITAL LETTER A WITH ACUTE*/
    /*0xC2*/    0x00C2,    /*LATIN CAPITAL LETTER A WITH CIRCUMFLEX*/
    /*0xC3*/    0x00C3,    /*LATIN CAPITAL LETTER A WITH TILDE*/
    /*0xC4*/    0x1EA2,    /*LATIN CAPITAL LETTER A WITH HOOK ABOVE*/
    /*0xC5*/    0x0102,    /*LATIN CAPITAL LETTER A WITH BREVE*/
    /*0xC6*/    0x1EB3,    /*LATIN SMALL LETTER A WITH BREVE AND HOOK ABOVE*/
    /*0xC7*/    0x1EB5,    /*LATIN SMALL LETTER A WITH BREVE AND TILDE*/
    /*0xC8*/    0x00C8,    /*LATIN CAPITAL LETTER E WITH GRAVE*/
    /*0xC9*/    0x00C9,    /*LATIN CAPITAL LETTER E WITH ACUTE*/
    /*0xCA*/    0x00CA,    /*LATIN CAPITAL LETTER E WITH CIRCUMFLEX*/
    /*0xCB*/    0x1EBA,    /*LATIN CAPITAL LETTER E WITH HOOK ABOVE*/
    /*0xCC*/    0x00CC,    /*LATIN CAPITAL LETTER I WITH GRAVE*/
    /*0xCD*/    0x00CD,    /*LATIN CAPITAL LETTER I WITH ACUTE*/
    /*0xCE*/    0x0128,    /*LATIN CAPITAL LETTER I WITH TILDE*/
    /*0xCF*/    0x1EF3,    /*LATIN SMALL LETTER Y WITH GRAVE*/
    /*0xD0*/    0x0110,    /*LATIN CAPITAL LETTER D WITH STROKE*/
    /*0xD1*/    0x1EE9,    /*LATIN SMALL LETTER U WITH HORN AND ACUTE*/
    /*0xD2*/    0x00D2,    /*LATIN CAPITAL LETTER O WITH GRAVE*/
    /*0xD3*/    0x00D3,    /*LATIN CAPITAL LETTER O WITH ACUTE*/
    /*0xD4*/    0x00D4,    /*LATIN CAPITAL LETTER O WITH CIRCUMFLEX*/
    /*0xD5*/    0x1EA1,    /*LATIN SMALL LETTER A WITH DOT BELOW*/
    /*0xD6*/    0x1EF7,    /*LATIN SMALL LETTER Y WITH HOOK ABOVE*/
    /*0xD7*/    0x1EEB,    /*LATIN SMALL LETTER U WITH HORN AND GRAVE*/
    /*0xD8*/    0x1EED,    /*LATIN SMALL LETTER U WITH HORN AND HOOK ABOVE*/
    /*0xD9*/    0x00D9,    /*LATIN CAPITAL LETTER U WITH GRAVE*/
    /*0xDA*/    0x00DA,    /*LATIN CAPITAL LETTER U WITH ACUTE*/
    /*0xDB*/    0x1EF9,    /*LATIN SMALL LETTER Y WITH TILDE*/
    /*0xDC*/    0x1EF5,    /*LATIN SMALL LETTER Y WITH DOT BELOW*/
    /*0xDD*/    0x00DD,    /*LATIN CAPITAL LETTER Y WITH ACUTE*/
    /*0xDE*/    0x1EE1,    /*LATIN SMALL LETTER O WITH HORN AND TILDE*/
    /*0xDF*/    0x01B0,    /*LATIN SMALL LETTER U WITH HORN*/
    /*0xE0*/    0x00E0,    /*LATIN SMALL LETTER A WITH GRAVE*/
    /*0xE1*/    0x00E1,    /*LATIN SMALL LETTER A WITH ACUTE*/
    /*0xE2*/    0x00E2,    /*LATIN SMALL LETTER A WITH CIRCUMFLEX*/
    /*0xE3*/    0x00E3,    /*LATIN SMALL LETTER A WITH TILDE*/
    /*0xE4*/    0x1EA3,    /*LATIN SMALL LETTER A WITH HOOK ABOVE*/
    /*0xE5*/    0x0103,    /*LATIN SMALL LETTER A WITH BREVE*/
    /*0xE6*/    0x1EEF,    /*LATIN SMALL LETTER U WITH HORN AND TILDE*/
    /*0xE7*/    0x1EAB,    /*LATIN SMALL LETTER A WITH CIRCUMFLEX AND TILDE*/
    /*0xE8*/    0x00E8,    /*LATIN SMALL LETTER E WITH GRAVE*/
    /*0xE9*/    0x00E9,    /*LATIN SMALL LETTER E WITH ACUTE*/
    /*0xEA*/    0x00EA,    /*LATIN SMALL LETTER E WITH CIRCUMFLEX*/
    /*0xEB*/    0x1EBB,    /*LATIN SMALL LETTER E WITH HOOK ABOVE*/
    /*0xEC*/    0x00EC,    /*LATIN SMALL LETTER I WITH GRAVE*/
    /*0xED*/    0x00ED,    /*LATIN SMALL LETTER I WITH ACUTE*/
    /*0xEE*/    0x0129,    /*LATIN SMALL LETTER I WITH TILDE*/
    /*0xEF*/    0x1EC9,    /*LATIN SMALL LETTER I WITH HOOK ABOVE*/
    /*0xF0*/    0x0111,    /*LATIN SMALL LETTER D WITH STROKE*/
    /*0xF1*/    0x1EF1,    /*LATIN SMALL LETTER U WITH HORN AND DOT BELOW*/
    /*0xF2*/    0x00F2,    /*LATIN SMALL LETTER O WITH GRAVE*/
    /*0xF3*/    0x00F3,    /*LATIN SMALL LETTER O WITH ACUTE*/
    /*0xF4*/    0x00F4,    /*LATIN SMALL LETTER O WITH CIRCUMFLEX*/
    /*0xF5*/    0x00F5,    /*LATIN SMALL LETTER O WITH TILDE*/
    /*0xF6*/    0x1ECF,    /*LATIN SMALL LETTER O WITH HOOK ABOVE*/
    /*0xF7*/    0x1ECD,    /*LATIN SMALL LETTER O WITH DOT BELOW*/
    /*0xF8*/    0x1EE5,    /*LATIN SMALL LETTER U WITH DOT BELOW*/
    /*0xF9*/    0x00F9,    /*LATIN SMALL LETTER U WITH GRAVE*/
    /*0xFA*/    0x00FA,    /*LATIN SMALL LETTER U WITH ACUTE*/
    /*0xFB*/    0x0169,    /*LATIN SMALL LETTER U WITH TILDE*/
    /*0xFC*/    0x1EE7,    /*LATIN SMALL LETTER U WITH HOOK ABOVE*/
    /*0xFD*/    0x00FD,    /*LATIN SMALL LETTER Y WITH ACUTE*/
    /*0xFE*/    0x1EE3,    /*LATIN SMALL LETTER O WITH HORN AND DOT BELOW*/
    /*0xFF*/    0x1EEE,    /*LATIN CAPITAL LETTER U WITH HORN AND TILDE*/
};

const static ID3_TAG_LCTBL_T at_id3_lctbl[] = 
{
    {"heb", MM_UTIL_TABLE_ISO_8859_8},/*Hebrew,       ISO-8859-8*/
    {"hbr", MM_UTIL_TABLE_ISO_8859_8},/*Hebrew,       ISO-8859-8*/
    {"kaz", MM_UTIL_TABLE_ISO_8859_5},/*Kazakh,       ISO-8859-5*/
    {"bul", MM_UTIL_TABLE_ISO_8859_5},/*BULGARIAN, ISO-8859-5*/
    {"cat", MM_UTIL_TABLE_ISO_8859_1},/*CATALAN,     ISO-8859-1*/
    {"cze", MM_UTIL_TABLE_ISO_8859_2},/*CZECH,        ISO-8859-2*/
    {"hrv", MM_UTIL_TABLE_ISO_8859_2},/*CROATIAN,   ISO-8859-2*/
    {"dan", MM_UTIL_TABLE_ISO_8859_1},/*DANISH,       ISO-8859-1*/
    {"dut", MM_UTIL_TABLE_ISO_8859_1},/*?DUTCH,        ISO-8859-1*/
    {"eng", MM_UTIL_TABLE_ISO_8859_1},/*ENGLISH,     ISO-8859-1*/
    {"est", MM_UTIL_TABLE_ISO_8859_1},/*ESTONIAN,   ISO-8859-1*/
    {"fin", MM_UTIL_TABLE_ISO_8859_1},/*FINNISH,      ISO-8859-1*/
    {"fre", MM_UTIL_TABLE_ISO_8859_1},/*FRENCH,      ISO-8859-1*/
    {"deu", MM_UTIL_TABLE_ISO_8859_1},/*?GERMAN,      ISO-8859-1*/
    {"gre", MM_UTIL_TABLE_ISO_8859_7},/*GREEK,         ISO-8859-7*/
    {"hun", MM_UTIL_TABLE_ISO_8859_2},/*HUNGARIAN, ISO-8859-2*/
    {"ita", MM_UTIL_TABLE_ISO_8859_1},/*ITALIAN,        ISO-8859-1*/
    {"lav", MM_UTIL_TABLE_ISO_8859_13},/*LATVIAN,     ISO-8859-13*/
    {"lit", MM_UTIL_TABLE_ISO_8859_13},/*LITHUANIAN, ISO-8859-13*/
    {"nor", MM_UTIL_TABLE_ISO_8859_1},/*NORWEGIAN, ISO-8859-1*/
    {"pol", MM_UTIL_TABLE_ISO_8859_2},/*POLISH,         ISO-8859-2*/
    {"por", MM_UTIL_TABLE_ISO_8859_1},/*PORTUGUESE, ISO-8859-1*/
    {"ron", MM_UTIL_TABLE_ISO_8859_2},/*ROMANIAN,     ISO-8859-2*/
    {"rus", MM_UTIL_TABLE_ISO_8859_5},/*RUSSIAN,        ISO-8859-5*/
    {"slk", MM_UTIL_TABLE_ISO_8859_2},/*SLOVAK,         ISO-8859-2*/
    {"slv", MM_UTIL_TABLE_ISO_8859_2},/*SLOVENIAN,    ISO-8859-2*/
    {"spa", MM_UTIL_TABLE_ISO_8859_1},/*SPANISH,        ISO-8859-1*/
    {"swe", MM_UTIL_TABLE_ISO_8859_1},/*SWEDISH,       ISO-8859-1*/
    {"tur", MM_UTIL_TABLE_ISO_8859_9},/*TURKISH,        ISO-8859-9*/
    {"ukr", MM_UTIL_TABLE_ISO_8859_5} /*UKRAINAN,      ISO-8859-5*/
};



/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name:  mm_cvt_8859_ps_to_w2s
 * Description: This API convert a ISO/IEC 8859 string to UTF-16 string.
 * Inputs:  ui2_part_num            Contains the part number.
 *             ps_src                  Source string to be converted.
 *             ui2_src_len             Size of input string, in byte units.
 * Outputs: w2s_dst                 The converted string.
 *              pui2_dst_len            Buffer size of w2s_dst, in character units.
 * Returns:
 ----------------------------------------------------------------------------*/
INT32 mm_cvt_8859_ps_to_w2s(
    UINT16          ui2_part_num,
    const CHAR*     ps_src,
    UINT16          ui2_src_len,
    UTF16_T*        w2s_dst,
    UINT16*         pui2_dst_len)
{
    UINT8                   ui1_char;
    UINT32                  ui4_src_idx;
    UINT32                  ui4_dst_idx;
    UTF16_T                 w2_char;

    if ((ps_src == NULL)    ||
        (ui2_src_len == 0)  ||
        (w2s_dst == NULL))
    {
        return MM_UTILR_INV_ARG;
    }

    if ((ui2_part_num == 0)||
        (ui2_part_num >= 18))
    {
        return MM_UTILR_NO_SUPPORT;
    }

    if (*pui2_dst_len == 0)
    {
        return MM_UTILR_OUT_OF_MEM;
    }

    ui4_src_idx = 0;
    ui4_dst_idx = 0;
    
    while (ui4_src_idx < ui2_src_len)
    {
        ui1_char = (UINT8) ps_src[ui4_src_idx];
        w2_char = 0xFFFF;

        if (ui1_char <= 0xA0)
        {
            w2_char = (UTF16_T) ui1_char;
        }
        else
        {
            switch (ui2_part_num)
            {
                case 1:
                    w2_char = (UTF16_T) ui1_char;
                    break;
                case 2:
                    w2_char = aw2_8859_2_cnv_tbl[ui1_char - 0xA1];
                    break;
                case 3:
                    w2_char = aw2_8859_3_cnv_tbl[ui1_char - 0xA1];
                    break;
                case 4:
                    w2_char = aw2_8859_4_cnv_tbl[ui1_char - 0xA1];
                    break;
                case 5:
                    w2_char = aw2_8859_5_cnv_tbl[ui1_char - 0xA1];
                    break;                
                case 6:
                    w2_char = aw2_8859_6_cnv_tbl[ui1_char - 0xA1];
                    break;
                case 7:
                    w2_char = aw2_8859_7_cnv_tbl[ui1_char - 0xA1];
                    break;
                case 8:
                    w2_char = aw2_8859_8_cnv_tbl[ui1_char - 0xA1];
                    break;    
                case 9:
                case 17: /* DVB_TEXT_CHAR_CODE_ISO_8859_9_HUN */
                    w2_char = aw2_8859_9_cnv_tbl[ui1_char - 0xA1];

                    if (ui2_part_num == 17) /* DVB_TEXT_CHAR_CODE_ISO_8859_9_HUN */
                    {
                        switch (ui1_char)
                        {
                            case 0xD5:
                                w2_char = 0x0150;//0x00D6;
                                break;
                            case 0xDB:
                                w2_char = 0x0170;//0x00DC;
                                break;
                            case 0xF5:
                                w2_char = 0x0151;//0x00F6;
                                break;
                            case 0xFB:
                                w2_char = 0x0171;//0x00FC;
                                break;
                        }
                    }
                    break;
                case 10:
                    w2_char = aw2_8859_10_cnv_tbl[ui1_char - 0xA1];
                    break;
                case 11:
                    w2_char = aw2_8859_11_cnv_tbl[ui1_char - 0xA1];
                    break;
                case 12:
                    w2_char = aw2_8859_12_cnv_tbl[ui1_char - 0xA1];
                    break;
                case 13:
                    w2_char = aw2_8859_13_cnv_tbl[ui1_char - 0xA1];     
                    break;
                case 14:
                    w2_char = aw2_8859_14_cnv_tbl[ui1_char - 0xA1];
                    break;
                case 15:
                    w2_char = aw2_8859_15_cnv_tbl[ui1_char - 0xA1];
                    break;
                case 16:
                    w2_char = aw2_8859_16_cnv_tbl[ui1_char - 0xA1];
                    break;
                default:
                    /* Never here */
                    break;
            }
        }

        if ( (w2_char != (UTF16_T) 0xFFFFU) && (w2_char != (UTF16_T)0x0))
        {
            if (ui4_dst_idx >= (UINT32) (*pui2_dst_len - 1))
            {
                return MM_UTILR_OUT_OF_MEM;
            }
            
            w2s_dst[ui4_dst_idx] = w2_char;
            ui4_dst_idx++;
        }

        ui4_src_idx++;
    }

    *pui2_dst_len = (UINT16) ui4_dst_idx;

    return MM_UTILR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  mm_cvt_lang_to_chartbl
 * Description: This API convert Language string to ISO-8859-x.
 * Inputs:  e_lang     -language
 * Outputs:
 * Returns:ISO-8859-x
 ----------------------------------------------------------------------------*/
E_MM_UTIL_CHAR_TBL mm_cvt_lang_to_chartbl(ISO_639_LANG_T e_lang)
{
    UINT16 ui2_i;
    UINT16 ui2_len;
    ui2_len = sizeof(at_id3_lctbl)/sizeof(ID3_TAG_LCTBL_T);
        
    for (ui2_i=0; ui2_i<ui2_len; ui2_i++)
    {
        if ((at_id3_lctbl[ui2_i].e_langage[0] == e_lang[0]) &&
            (at_id3_lctbl[ui2_i].e_langage[1] == e_lang[1]) &&
            (at_id3_lctbl[ui2_i].e_langage[2] == e_lang[2]))
        {
            return at_id3_lctbl[ui2_i].e_chr_table;
        }
    }
    return MM_UTIL_TABLE_ISO_UNKNOWN;
}
#endif

