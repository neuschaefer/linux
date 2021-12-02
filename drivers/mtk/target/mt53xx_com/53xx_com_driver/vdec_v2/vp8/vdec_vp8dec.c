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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_vp8dec.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_vp8dec.c
 *  This source file include vp8 decode function 
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_assert.h"
#include "x_printf.h"
#include "vdec_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END
#include "srm_drvif.h"
#include "vdec_vp8webp_if.h"
#include "vdec_vp8util.h"
#include "vdec_vp8dec.h"
#include "vdec_vp8hw.h"


EXTERN UINT32 _VDEC_Vp8PsrWebPRequestData(VDEC_VP8_INFO_T *prVpDecInfo);
EXTERN VDEC_VP8_INFO_T *VDEC_Vp8GetLockedInstance(void);

static const UINT8 default_coef_probs [VP8_BLOCK_TYPES] [VP8_COEF_BANDS] [VP8_PREV_COEF_CONTEXTS] [VP8_COEF_TOKENS - 1] =
{
{
{
{ 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128},
{ 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128},
{ 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}
},
{
{ 253, 136, 254, 255, 228, 219, 128, 128, 128, 128, 128},
{ 189, 129, 242, 255, 227, 213, 255, 219, 128, 128, 128},
{ 106, 126, 227, 252, 214, 209, 255, 255, 128, 128, 128}
},
{
{ 1, 98, 248, 255, 236, 226, 255, 255, 128, 128, 128},
{ 181, 133, 238, 254, 221, 234, 255, 154, 128, 128, 128},
{ 78, 134, 202, 247, 198, 180, 255, 219, 128, 128, 128}
},
{
{ 1, 185, 249, 255, 243, 255, 128, 128, 128, 128, 128},
{ 184, 150, 247, 255, 236, 224, 128, 128, 128, 128, 128},
{ 77, 110, 216, 255, 236, 230, 128, 128, 128, 128, 128}
},
{
{ 1, 101, 251, 255, 241, 255, 128, 128, 128, 128, 128},
{ 170, 139, 241, 252, 236, 209, 255, 255, 128, 128, 128},
{ 37, 116, 196, 243, 228, 255, 255, 255, 128, 128, 128}
},
{
{ 1, 204, 254, 255, 245, 255, 128, 128, 128, 128, 128},
{ 207, 160, 250, 255, 238, 128, 128, 128, 128, 128, 128},
{ 102, 103, 231, 255, 211, 171, 128, 128, 128, 128, 128}
},
{
{ 1, 152, 252, 255, 240, 255, 128, 128, 128, 128, 128},
{ 177, 135, 243, 255, 234, 225, 128, 128, 128, 128, 128},
{ 80, 129, 211, 255, 194, 224, 128, 128, 128, 128, 128}
},
{
{ 1, 1, 255, 128, 128, 128, 128, 128, 128, 128, 128},
{ 246, 1, 255, 128, 128, 128, 128, 128, 128, 128, 128},
{ 255, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}
}
},
{
{
{ 198, 35, 237, 223, 193, 187, 162, 160, 145, 155, 62},
{ 131, 45, 198, 221, 172, 176, 220, 157, 252, 221, 1},
{ 68, 47, 146, 208, 149, 167, 221, 162, 255, 223, 128}
},
{
{ 1, 149, 241, 255, 221, 224, 255, 255, 128, 128, 128},
{ 184, 141, 234, 253, 222, 220, 255, 199, 128, 128, 128},
{ 81, 99, 181, 242, 176, 190, 249, 202, 255, 255, 128}
},
{
{ 1, 129, 232, 253, 214, 197, 242, 196, 255, 255, 128},
{ 99, 121, 210, 250, 201, 198, 255, 202, 128, 128, 128},
{ 23, 91, 163, 242, 170, 187, 247, 210, 255, 255, 128}
},
{
{ 1, 200, 246, 255, 234, 255, 128, 128, 128, 128, 128},
{ 109, 178, 241, 255, 231, 245, 255, 255, 128, 128, 128},
{ 44, 130, 201, 253, 205, 192, 255, 255, 128, 128, 128}
},
{
{ 1, 132, 239, 251, 219, 209, 255, 165, 128, 128, 128},
{ 94, 136, 225, 251, 218, 190, 255, 255, 128, 128, 128},
{ 22, 100, 174, 245, 186, 161, 255, 199, 128, 128, 128}
},
{
{ 1, 182, 249, 255, 232, 235, 128, 128, 128, 128, 128},
{ 124, 143, 241, 255, 227, 234, 128, 128, 128, 128, 128},
{ 35, 77, 181, 251, 193, 211, 255, 205, 128, 128, 128}
},
{
{ 1, 157, 247, 255, 236, 231, 255, 255, 128, 128, 128},
{ 121, 141, 235, 255, 225, 227, 255, 255, 128, 128, 128},
{ 45, 99, 188, 251, 195, 217, 255, 224, 128, 128, 128}
},
{
{ 1, 1, 251, 255, 213, 255, 128, 128, 128, 128, 128},
{ 203, 1, 248, 255, 255, 128, 128, 128, 128, 128, 128},
{ 137, 1, 177, 255, 224, 255, 128, 128, 128, 128, 128}
}
},
{
{
{ 253, 9, 248, 251, 207, 208, 255, 192, 128, 128, 128},
{ 175, 13, 224, 243, 193, 185, 249, 198, 255, 255, 128},
{ 73, 17, 171, 221, 161, 179, 236, 167, 255, 234, 128}
},
{
{ 1, 95, 247, 253, 212, 183, 255, 255, 128, 128, 128},
{ 239, 90, 244, 250, 211, 209, 255, 255, 128, 128, 128},
{ 155, 77, 195, 248, 188, 195, 255, 255, 128, 128, 128}
},
{
{ 1, 24, 239, 251, 218, 219, 255, 205, 128, 128, 128},
{ 201, 51, 219, 255, 196, 186, 128, 128, 128, 128, 128},
{ 69, 46, 190, 239, 201, 218, 255, 228, 128, 128, 128}
},
{
{ 1, 191, 251, 255, 255, 128, 128, 128, 128, 128, 128},
{ 223, 165, 249, 255, 213, 255, 128, 128, 128, 128, 128},
{ 141, 124, 248, 255, 255, 128, 128, 128, 128, 128, 128}
},
{
{ 1, 16, 248, 255, 255, 128, 128, 128, 128, 128, 128},
{ 190, 36, 230, 255, 236, 255, 128, 128, 128, 128, 128},
{ 149, 1, 255, 128, 128, 128, 128, 128, 128, 128, 128}
},
{
{ 1, 226, 255, 128, 128, 128, 128, 128, 128, 128, 128},
{ 247, 192, 255, 128, 128, 128, 128, 128, 128, 128, 128},
{ 240, 128, 255, 128, 128, 128, 128, 128, 128, 128, 128}
},
{
{ 1, 134, 252, 255, 255, 128, 128, 128, 128, 128, 128},
{ 213, 62, 250, 255, 255, 128, 128, 128, 128, 128, 128},
{ 55, 93, 255, 128, 128, 128, 128, 128, 128, 128, 128}
},
{
{ 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128},
{ 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128},
{ 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}
}
},
{
{
{ 202, 24, 213, 235, 186, 191, 220, 160, 240, 175, 255},
{ 126, 38, 182, 232, 169, 184, 228, 174, 255, 187, 128},
{ 61, 46, 138, 219, 151, 178, 240, 170, 255, 216, 128}
},
{
{ 1, 112, 230, 250, 199, 191, 247, 159, 255, 255, 128},
{ 166, 109, 228, 252, 211, 215, 255, 174, 128, 128, 128},
{ 39, 77, 162, 232, 172, 180, 245, 178, 255, 255, 128}
},
{
{ 1, 52, 220, 246, 198, 199, 249, 220, 255, 255, 128},
{ 124, 74, 191, 243, 183, 193, 250, 221, 255, 255, 128},
{ 24, 71, 130, 219, 154, 170, 243, 182, 255, 255, 128}
},
{
{ 1, 182, 225, 249, 219, 240, 255, 224, 128, 128, 128},
{ 149, 150, 226, 252, 216, 205, 255, 171, 128, 128, 128},
{ 28, 108, 170, 242, 183, 194, 254, 223, 255, 255, 128}
},
{
{ 1, 81, 230, 252, 204, 203, 255, 192, 128, 128, 128},
{ 123, 102, 209, 247, 188, 196, 255, 233, 128, 128, 128},
{ 20, 95, 153, 243, 164, 173, 255, 203, 128, 128, 128}
},
{
{ 1, 222, 248, 255, 216, 213, 128, 128, 128, 128, 128},
{ 168, 175, 246, 252, 235, 205, 255, 255, 128, 128, 128},
{ 47, 116, 215, 255, 211, 212, 255, 255, 128, 128, 128}
},
{
{ 1, 121, 236, 253, 212, 214, 255, 255, 128, 128, 128},
{ 141, 84, 213, 252, 201, 202, 255, 219, 128, 128, 128},
{ 42, 80, 160, 240, 162, 185, 255, 205, 128, 128, 128}
},
{
{ 1, 1, 255, 128, 128, 128, 128, 128, 128, 128, 128},
{ 244, 1, 255, 128, 128, 128, 128, 128, 128, 128, 128},
{ 238, 1, 255, 128, 128, 128, 128, 128, 128, 128, 128}
}
}
};

static const INT16 _i2rVp8DcQLookup[VP8_QINDEX_RANGE] =
{
    4,    5,    6,    7,    8,    9,   10,   10,   11,   12,   13,   14,   15,   16,   17,   17,
    18,   19,   20,   20,   21,   21,   22,   22,   23,   23,   24,   25,   25,   26,   27,   28,
    29,   30,   31,   32,   33,   34,   35,   36,   37,   37,   38,   39,   40,   41,   42,   43,
    44,   45,   46,   46,   47,   48,   49,   50,   51,   52,   53,   54,   55,   56,   57,   58,
    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,
    75,   76,   76,   77,   78,   79,   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
    91,   93,   95,   96,   98,  100,  101,  102,  104,  106,  108,  110,  112,  114,  116,  118,
    122,  124,  126,  128,  130,  132,  134,  136,  138,  140,  143,  145,  148,  151,  154,  157,
};

static const INT16 _i2rVp8AcQLookup[VP8_QINDEX_RANGE] =
{
    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
    52,   53,   54,   55,   56,   57,   58,   60,   62,   64,   66,   68,   70,   72,   74,   76,
    78,   80,   82,   84,   86,   88,   90,   92,   94,   96,   98,  100,  102,  104,  106,  108,
    110,  112,  114,  116,  119,  122,  125,  128,  131,  134,  137,  140,  143,  146,  149,  152,
    155,  158,  161,  164,  167,  170,  173,  177,  181,  185,  189,  193,  197,  201,  205,  209,
    213,  217,  221,  225,  229,  234,  239,  245,  249,  254,  259,  264,  269,  274,  279,  284,
};

static const VDEC_VP8_MV_CONTEXT_T vp8_default_mv_context[2] =
{
    {{
        // row
        162,                                        // is short
        128,                                        // sign
        225, 146, 172, 147, 214,  39, 156,          // short tree
        128, 129, 132,  75, 145, 178, 206, 239, 254, 254 // long bits
    }},

    {{
        // same for column
        164,                                        // is short
        128,
        204, 170, 119, 235, 140, 230, 228,
        128, 130, 130,  74, 148, 180, 203, 236, 254, 254 // long bits

    }}
};

#ifndef VP8_HEADERPARSE_HWACCELATOR
static const UINT8 _uVp8CoefUpdateProbs[VP8_BLOCK_TYPES] [VP8_COEF_BANDS] [VP8_PREV_COEF_CONTEXTS] [VP8_COEF_TOKENS-1] =
{
        {
            {
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {176, 246, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {223, 241, 252, 255, 255, 255, 255, 255, 255, 255, 255, },
                {249, 253, 253, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 244, 252, 255, 255, 255, 255, 255, 255, 255, 255, },
                {234, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 246, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {239, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {254, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 248, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {251, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {251, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {254, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 254, 253, 255, 254, 255, 255, 255, 255, 255, 255, },
                {250, 255, 254, 255, 254, 255, 255, 255, 255, 255, 255, },
                {254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
        },
        {
            {
                {217, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {225, 252, 241, 253, 255, 255, 254, 255, 255, 255, 255, },
                {234, 250, 241, 250, 253, 255, 253, 254, 255, 255, 255, },
            },
            {
                {255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {223, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {238, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 248, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {249, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {247, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 254, 253, 255, 255, 255, 255, 255, 255, 255, 255, },
                {250, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
        },
        {
            {
                {186, 251, 250, 255, 255, 255, 255, 255, 255, 255, 255, },
                {234, 251, 244, 254, 255, 255, 255, 255, 255, 255, 255, },
                {251, 251, 243, 253, 254, 255, 254, 255, 255, 255, 255, },
            },
            {
                {255, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {236, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {251, 253, 253, 254, 254, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
        },
        {
            {
                {248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {250, 254, 252, 254, 255, 255, 255, 255, 255, 255, 255, },
                {248, 254, 249, 253, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 253, 253, 255, 255, 255, 255, 255, 255, 255, 255, },
                {246, 253, 253, 255, 255, 255, 255, 255, 255, 255, 255, },
                {252, 254, 251, 254, 254, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 254, 252, 255, 255, 255, 255, 255, 255, 255, 255, },
                {248, 254, 253, 255, 255, 255, 255, 255, 255, 255, 255, },
                {253, 255, 254, 254, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 251, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {245, 251, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {253, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 251, 253, 255, 255, 255, 255, 255, 255, 255, 255, },
                {252, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {249, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 255, 253, 255, 255, 255, 255, 255, 255, 255, 255, },
                {250, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
            {
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
                {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
            },
       },
};
const VDEC_VP8_MV_CONTEXT_T vp8_mv_update_probs[2] =
{
    {{
        237,
        246,
        253, 253, 254, 254, 254, 254, 254,
        254, 254, 254, 254, 254, 250, 250, 252, 254, 254
    }},
    {{
        231,
        243,
        245, 253, 254, 254, 254, 254, 254,
        254, 254, 254, 254, 254, 251, 251, 254, 254, 254
    }}
};
#endif

#ifndef VP8_USE_SMALLQT
static INT8 _Vp8DefaultZigZag1d[16]=
{
  0,1,4,8,
  5,2,3,6,
  9,12,13,10,
  7,11,14,15
};
static INT16 _rVp8Dequant[VP8_QTALBE_MAX][VP8_QINDEX_RANGE][4][4];
#else
static INT16 _rVp8Dequant[VP8_QTALBE_MAX][VP8_QINDEX_RANGE][1][2];
#endif
#ifdef VP8_EMUCOVERAGE_STATISTIC
static UINT8 _puStatisticBuf[VP8_EMUCOVERAGE_BUFSIZE];
#endif

#ifdef VP8_TEST_BUFRING
static UINT32 u4FrameIndex=0;
#endif

static INT32 _Vp8MbFeatureDataBits[VP8_MBLVL_MAX]={7,6};
static BOOL _VDEC_Vp8IsDecFinish(VOID);
static UINT32 _VDEC_Vp8GetErrInfo(VOID);
#if defined(VP8_ENABLE_CRC) || defined(WEBP_ENABLE_CRC)
static VOID _VDEC_Vp8GetCrcValue(UINT32 puCrcBuf[][4],UINT32 *pu4DbkType);
#endif

#ifdef VDEC_VP8_MCPERFORMANCE_TEST
static VOID _VDEC_Vp8HwPrintMCPerformanceInfo(VDEC_VP8_INFO_T *prVdecInfo)
{
    #if 1
    extern UINT32 _u4LogBufSa;
    extern UINT32 _u4LogBufSz;
    extern UINT32 _u4LogSz;
    static CHAR _acLogTmp[128];
    UINT32 u4LogSz;

    UINT32 u4HalVal1=0,u4HalVal2=0,u4HalVal3=0;
    
    u4HalVal1=VDEC_MC_READ32(RO_MC_CYCLE_DRAM);
    sprintf(_acLogTmp, "VDEC_perf_measure:seq_name=%s,pic_idx=%d,pic_width=%d,pic_height=%d\n", \
      prVdecInfo->rDecParam.puFileName,prVdecInfo->rDecParam.u4FrameCounter-1,     \
      prVdecInfo->rDecParam.u2Width,prVdecInfo->rDecParam.u2Height);
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);

    sprintf(_acLogTmp, "VDEC_perf_measure2:pic_type=%s,framepic=1,CYCLE_DRAM=%d\n", \
       prVdecInfo->rDecParam.fgKeyFrame ? "I" : "P",u4HalVal1);
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);

    u4HalVal1=VDEC_MC_READ32(RO_MC_DLE_NUM);
    u4HalVal2=VDEC_MC_READ32(RO_MC_MBX);
    u4HalVal3=VDEC_MC_READ32(RO_MC_MBY);
    sprintf(_acLogTmp, "VDEC_perf_measure3:MC_DLE_NUM=%d,MC_MBX =%d,MC_MBY =%d\n", \
      u4HalVal1,u4HalVal2,u4HalVal3);
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);

    u4HalVal1=VDEC_MC_READ32(RO_MC_CYC_SYS);
    u4HalVal2=VDEC_MC_READ32(RO_MC_INTRA_CNT);
    u4HalVal3=VDEC_MC_READ32(RO_MC_YBLK_CNT);
    sprintf(_acLogTmp, "VDEC_perf_measure4:CYC_SYS=%d,INTRA_CNT=%d,Y_BLK_CNT=%d\n", \
     u4HalVal1,u4HalVal2,u4HalVal3);
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);

    u4HalVal1=VDEC_MC_READ32(RO_MC_CBLK_CNT);
    u4HalVal2=VDEC_MC_READ32(RO_MC_WAIT_CNT);
    u4HalVal3=VDEC_MC_READ32(RO_MC_REQ_CNT);
    sprintf(_acLogTmp, "VDEC_perf_measure5:C_BLK_CNT=%d,WAIT_CNT=%d,REQ_CNT=%d\n", \
     u4HalVal1,u4HalVal2,u4HalVal3);
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);

    u4HalVal1=VDEC_MC_READ32(RW_MC_PARA_BEHAVIOR);
    sprintf(_acLogTmp, "VDEC_perf_measure6:PARA4_BEHAVIOR=%d,PARA8_BEHAVIOR=%d\n", \
     u4HalVal1&1,(u4HalVal1>>4)&0x1);
    u4LogSz = x_strlen(_acLogTmp);
    x_memcpy(_u4LogBufSa + _u4LogSz, _acLogTmp, u4LogSz);
    _u4LogSz += u4LogSz;
    ASSERT(_u4LogSz < _u4LogBufSz);

    #else
   UINT32 u4HalVal1=0,u4HalVal2=0,u4HalVal3=0;
   
   u4HalVal1=VDEC_MC_READ32(RO_MC_CYCLE_DRAM);
   Printf("VDEC_perf_measure:seq_name=%s,pic_idx=%d,pic_width=%d,pic_height=%d\n", \
     prVdecInfo->rDecParam.puFileName,prVdecInfo->rDecParam.u4FrameCounter-1,     \
     prVdecInfo->rDecParam.u2Width,prVdecInfo->rDecParam.u2Height);
   
   Printf("VDEC_perf_measure2:pic_type=%s,framepic=1,CYCLE_DRAM=%d\n", \
   	  prVdecInfo->rDecParam.fgKeyFrame ? "I" : "P",u4HalVal1);
   
   u4HalVal1=VDEC_MC_READ32(RO_MC_DLE_NUM);
   u4HalVal2=VDEC_MC_READ32(RO_MC_MBX);
   u4HalVal3=VDEC_MC_READ32(RO_MC_MBY);
   Printf("VDEC_perf_measure3:MC_DLE_NUM=%d,MC_MBX =%d,MC_MBY =%d\n", \
   	 u4HalVal1,u4HalVal2,u4HalVal3);

   u4HalVal1=VDEC_MC_READ32(RO_MC_CYC_SYS);
   u4HalVal2=VDEC_MC_READ32(RO_MC_INTRA_CNT);
   u4HalVal3=VDEC_MC_READ32(RO_MC_YBLK_CNT);
   Printf("VDEC_perf_measure4:CYC_SYS=%d,INTRA_CNT=%d,Y_BLK_CNT=%d\n", \
   	u4HalVal1,u4HalVal2,u4HalVal3);

   u4HalVal1=VDEC_MC_READ32(RO_MC_CBLK_CNT);
   u4HalVal2=VDEC_MC_READ32(RO_MC_WAIT_CNT);
   u4HalVal3=VDEC_MC_READ32(RO_MC_REQ_CNT);
   Printf("VDEC_perf_measure5:C_BLK_CNT=%d,WAIT_CNT=%d,REQ_CNT=%d\n", \
   	u4HalVal1,u4HalVal2,u4HalVal3);

   u4HalVal1=VDEC_MC_READ32(RW_MC_PARA_BEHAVIOR);
   Printf("VDEC_perf_measure6:PARA4_BEHAVIOR=%d,PARA8_BEHAVIOR=%d\n", \
   	u4HalVal1&1,(u4HalVal1>>4)&0x1);
   /*
        VDEC_perf_measure:seq_name=%d,pic_idx=%d,pic_width=%d,pic_height=%d\n
        VDEC_perf_measure2:pic_type=I(orP/B),framepic=%d,CYCLE_DRAM=%d\n
        VDEC_perf_measure3:MC_DLE_NUM=%d,MC_MBX =%d,MC_MBY =%d\n
        VDEC_perf_measure4:CYC_SYS=%d,INTRA_CNT=%d,Y_BLK_CNT=%d\n
        VDEC_perf_measure5:C_BLK_CNT=%d,WAIT_CNT=%d,REQ_CNT=%d\n
        VDEC_perf_measure6:PARA4_BEHAVIOR=%d,PARA8_BEHAVIOR=%d\n
    */
    #endif
}
#endif

#ifdef SUPPORT_DEC_ISR
static HANDLE_T _hVP8VdecFinSema = (HANDLE_T)NULL;
static VDEC_VP8_INFO_T *prCurDecoder=NULL;
static UINT32 _VDEC_Vp8HwWebPIsrType( VOID )
{
   UINT32 u4IsrType=0;
   u4IsrType=VP8_VLD_READ32(RO_VP8_VOKR);
   VP8_VLD_WRITE32(RO_VP8_VOKR,0);
   return u4IsrType;
}

static void _VDEC_Vp8HwIsr(UINT16 u2Vector)
{  
   if(u2Vector == AVLD_VECTOR)
   {
     //VDEC_MISC_WRITE32(RW_VDEC_MISC_INT_CFG, INT_CFG_WAIT_BITS_NOP|INT_CFG_INT_CLR|INT_CFG_CLR_INT_MD);	 // 0x1011
     //VDEC_MISC_WRITE32(RW_VDEC_MISC_INT_CFG, INT_CFG_WAIT_BITS_NOP|INT_CFG_CLR_INT_MD);	 // 0x1001
      LOG(9,"Webm Triger Vp8 Isr\n");
      if(_VDEC_Vp8GetErrInfo() || _VDEC_Vp8IsDecFinish())
      {
      	 VDEC_VLD_TOP_WRITE32(RW_VLD_OK_CLR,1);
		 VDEC_VLD_TOP_WRITE32(RW_VLD_OK_CLR,0);
#ifdef VDEC_VP8_MCPERFORMANCE_TEST
         _VDEC_Vp8HwPrintMCPerformanceInfo(prCurDecoder);
#endif
	      BIM_ClearIrq(AVLD_VECTOR);
          LOG(9,"Release Vp8 Sema\n");
          VERIFY(x_sema_unlock(_hVP8VdecFinSema) == OSR_OK);
      }
	  else
	  {
		  BIM_ClearIrq(AVLD_VECTOR);
	  }
   }
}

static void _VDEC_Vp8HwWebPIsr(UINT16 u2Vector)
{
	if(u2Vector == AVLD_VECTOR)
	{       
	   if(_VDEC_Vp8GetErrInfo())
	   {
		  VDEC_VLD_TOP_WRITE32(RW_VLD_OK_CLR,1);
		  VDEC_VLD_TOP_WRITE32(RW_VLD_OK_CLR,0);
	      BIM_ClearIrq(AVLD_VECTOR);
          LOG(5,"Webp Isr Error 0x%x\n",_VDEC_Vp8GetErrInfo());
	      VDEC_SETFLG(prCurDecoder->rDecParam.u4Flag,VP8_DEC_FLG_DECERROR);
		  VERIFY(x_sema_unlock(_hVP8VdecFinSema) == OSR_OK);
	   }
	   else if(_VDEC_Vp8IsDecFinish())
	   {
		  VDEC_SETFLG(prCurDecoder->rDecParam.u4Flag,VP8_DEC_FLG_FINISHED);
          LOG(5,"Webp Isr DecFinish\n");
#ifdef WEBP_ENABLE_CRC
          _VDEC_Vp8GetCrcValue(prCurDecoder->rDecParam.pu4CrcVal,&prCurDecoder->rDecParam.u4DbkType);
#endif
		  VDEC_VLD_TOP_WRITE32(RW_VLD_OK_CLR,1);
		  VDEC_VLD_TOP_WRITE32(RW_VLD_OK_CLR,0);
	      BIM_ClearIrq(AVLD_VECTOR);
		  VERIFY(x_sema_unlock(_hVP8VdecFinSema) == OSR_OK);
	   }
	   else
	   {
	      UINT32 u4InterType=0;
	      BIM_ClearIrq(AVLD_VECTOR);
		  u4InterType=_VDEC_Vp8HwWebPIsrType();
		  if(VDEC_FLGSET(u4InterType,VP8_INTTYPE_PARTCHG))
		  {  
             LOG(8,"Webp Isr Partition Change %d\n",prCurDecoder->rWebPInfo.u4RingBufIndex);
             VDEC_SETFLG(prCurDecoder->rDecParam.u4Flag,VP8_DEC_FLG_PARTCHG);
             //VERIFY(x_sema_unlock(_hVP8VdecFinSema) == OSR_OK);
		  }
		  
		  if(VDEC_FLGSET(u4InterType,VP8_INTTYPE_BUFCTRL))
		  {
              LOG(8,"Webp Isr Buf Ctrl\n");
              //prCurDecoder->rWebPInfo.rSrcBufInfo[prCurDecoder->rWebPInfo.u4RingBufIndex].u4UsedBitCount=_VDEC_Vp8GetBitCnt(VDEC_VLD2);
              VDEC_SETFLG(prCurDecoder->rDecParam.u4Flag,VP8_DEC_FLG_MCUROW);              
			  VERIFY(x_sema_unlock(_hVP8VdecFinSema) == OSR_OK);
		  }
	   }
	}
}

#endif


#if 0

static VOID _VDEC_Vp8WriteCtxSram(UINT32 u4Address,UINT32 u4Data,BOOL fgResetAddr)
{
   if(fgResetAddr)
   {
      VP8_VLD_WRITE32(RW_VP8_CSASET,u4Address);
   }
   else
   {
      VP8_VLD_WRITE32(RW_VP8_CSDSET,u4Data);
   }
}

static UINT32 _VDEC_Vp8ReadCtxSram(UINT32 u4Address,BOOL fgResetAddr)
{
   if(fgResetAddr)
   {
       VP8_VLD_WRITE32(RW_VP8_CSASET,u4Address);
       return 0;
   }
   else
   {
       return VP8_VLD_READ32(RW_VP8_CSDSET);
   }
}

static VOID _VDEC_Vp8WriteBusSram(UINT32 u4Address,UINT32 u4Data,BOOL fgResetAddr)
{
   if(fgResetAddr)
   {
      VP8_VLD_WRITE32(RW_VP8_BSASET,u4Address);
   }
   else
   {
      VP8_VLD_WRITE32(RW_VP8_BSDSET,u4Data);
   }
}

static UINT32 _VDEC_Vp8ReadBusSram(UINT32 u4Address,BOOL fgResetAddr)
{
   if(fgResetAddr)
   {
       VP8_VLD_WRITE32(RW_VP8_BSASET,u4Address);
       return 0;
   }
   else
   {
       return VP8_VLD_READ32(RW_VP8_BSDSET);
   }
}
#endif

#ifdef VP8_TEST_BUFRING
static VOID _VDEC_Vp8TestBufRing(VDEC_INFO_VP8_FRM_HDR_T *prFrmHdr)
{
   UINT32 u4MoveSize=0,u4AlignWritePtr=0;
   UINT32 u4FirstDataLen=0,u4RealWritePos=0;
   UCHAR *puData=(UCHAR *)prFrmHdr->u4VldStartPos;
   
   u4FirstDataLen=(puData[0]|(puData[1]<<8)|(puData[2]<<16))>>5;
   prFrmHdr->u4FifoEnd=0xff00000;
   prFrmHdr->u4FifoStart=0xfd00000;
   u4MoveSize=prFrmHdr->u4FrameSize>>1;
   x_memcpy((void *)(prFrmHdr->u4FifoEnd-u4MoveSize),(void *)prFrmHdr->u4VldStartPos,u4MoveSize);
   x_memcpy((void *)prFrmHdr->u4FifoStart,(void *)(prFrmHdr->u4VldStartPos+u4MoveSize),prFrmHdr->u4FrameSize-u4MoveSize);
   HalFlushInvalidateDCache();
   prFrmHdr->u4VldStartPos=prFrmHdr->u4FifoEnd-u4MoveSize;
   u4RealWritePos=prFrmHdr->u4FifoStart+(prFrmHdr->u4FrameSize-u4MoveSize);
   
   if(u4FrameIndex&1)
   {
       prFrmHdr->u4WritePos=prFrmHdr->u4VldStartPos-1;
       prFrmHdr->u4WritePos=prFrmHdr->u4WritePos-(prFrmHdr->u4WritePos&0xf)-0x10;
       if(prFrmHdr->u4WritePos>=prFrmHdr->u4VldStartPos)
       {
          ASSERT(0);
       }
   }
   else
   {
      prFrmHdr->u4WritePos=prFrmHdr->u4FifoStart;
      if(u4MoveSize<u4FirstDataLen)
      {
         u4MoveSize=u4FirstDataLen-u4MoveSize+512;
      }
      else
      {
         if(u4MoveSize-u4FirstDataLen>512)
         {
            u4MoveSize=0;
         }
         else
         {
            u4MoveSize=512-(u4MoveSize-u4FirstDataLen);
         }
      }
      
      prFrmHdr->u4WritePos+=u4MoveSize;
      prFrmHdr->u4WritePos=(prFrmHdr->u4WritePos+0xf)&(~0xf);
   }
   Printf("RingBuf Test(%d):u4FirstDataLen=0x%x,Start=0x%x,End=0x%x,Write=(0x%x,0x%x,%s),Read=0x%x,Size=0x%x\n", \
       u4FrameIndex,u4FirstDataLen,prFrmHdr->u4FifoStart,prFrmHdr->u4FifoEnd,u4RealWritePos,prFrmHdr->u4WritePos,\
       u4RealWritePos>prFrmHdr->u4WritePos ? "Pause" : "No Pause",prFrmHdr->u4VldStartPos,prFrmHdr->u4FrameSize);
   u4FrameIndex++;
}
#endif

static VOID _VDEC_Vp8FlushCtxProbs(VDEC_VP8FRAME_CONTEXT_T *prCtxt,UINT32 u4FrameType)
{
   UINT32 u4HalValue=0;
   UINT8 *prMvc=(UINT8 *)prCtxt->MVC;
   
   if(u4FrameType==VP68_KEY_FRAME)
   {
       // vp8_default_mv_context
       VP8_VLD_WRITE32(RW_VP8_MVPROB,0x8080a2a4); // mv_default_prob_update 128 128 162 164
       VP8_VLD_WRITE32(RW_VP8_CSASET,0xc);   //CTX STRAM address, MV_SHORT_PROB_Y_Offset
       VP8_VLD_WRITE32(RW_VP8_CSDSET,0x93ac92e1); // MV_SHORT_PROB_Y 147 172 146 225 
       VP8_VLD_WRITE32(RW_VP8_CSDSET,0x9c27d6);   //MV_SHORT_PROB_Y 156 39 214
       VP8_VLD_WRITE32(RW_VP8_CSASET,0x10);       // MV_LONG_PROB_Y_OFFSET
       VP8_VLD_WRITE32(RW_VP8_CSDSET,0x4b848180); //MV_LONG_PROB_Y 75 132 129 128
       VP8_VLD_WRITE32(RW_VP8_CSDSET,0xefceb291); // 239 206 178 145
       VP8_VLD_WRITE32(RW_VP8_CSDSET,0xfefe);     // 254 254
       
       VP8_VLD_WRITE32(RW_VP8_CSASET,0x14);       // mv_short_prob_x_offset
       VP8_VLD_WRITE32(RW_VP8_CSDSET,0xeb77aacc); 
       VP8_VLD_WRITE32(RW_VP8_CSDSET,0xe4e68c);
       VP8_VLD_WRITE32(RW_VP8_CSASET,0x18);       // mv_long_prob_x_offset
       VP8_VLD_WRITE32(RW_VP8_CSDSET,0x4a828280); 
       VP8_VLD_WRITE32(RW_VP8_CSDSET,0xeccbb494);
       VP8_VLD_WRITE32(RW_VP8_CSDSET,0xfefe);

   }
   else
   {
       VP8_VLD_WRITE32(RW_VP8_CSASET,4);
       u4HalValue=(prCtxt->YModeProb[3]<<24)+(prCtxt->YModeProb[2]<<16)
                  +(prCtxt->YModeProb[1]<<8)+prCtxt->YModeProb[0];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       
       VP8_VLD_WRITE32(RW_VP8_CSASET,8);
       u4HalValue=(prCtxt->UVModeProb[2]<<16)+(prCtxt->UVModeProb[1]<<8)
                  +prCtxt->UVModeProb[0];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       
       u4HalValue=(prMvc[1]<<24)+(prMvc[20]<<16)+(prMvc[0]<<8)+prMvc[19];
       VP8_VLD_WRITE32(RW_VP8_MVPROB,u4HalValue);
       
       VP8_VLD_WRITE32(RW_VP8_CSASET,0XC);
       u4HalValue=(prMvc[5]<<24)+(prMvc[4]<<16)+(prMvc[3]<<8)+prMvc[2];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       u4HalValue=(prMvc[8]<<16)+(prMvc[7]<<8)+prMvc[6];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       
       VP8_VLD_WRITE32(RW_VP8_CSASET,0X10);
       u4HalValue=(prMvc[12]<<24)+(prMvc[11]<<16)+(prMvc[10]<<8)+prMvc[9];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       u4HalValue=(prMvc[16]<<24)+(prMvc[15]<<16)+(prMvc[14]<<8)+prMvc[13];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       u4HalValue=(prMvc[18]<<8)+prMvc[17];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       
       VP8_VLD_WRITE32(RW_VP8_CSASET,0X14);
       u4HalValue=(prMvc[24]<<24)+(prMvc[23]<<16)+(prMvc[22]<<8)+prMvc[21];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       u4HalValue=(prMvc[27]<<16)+(prMvc[26]<<8)+prMvc[25];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       
       VP8_VLD_WRITE32(RW_VP8_CSASET,0X18);
       u4HalValue=(prMvc[31]<<24)+(prMvc[30]<<16)+(prMvc[29]<<8)+prMvc[28];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       u4HalValue=(prMvc[35]<<24)+(prMvc[34]<<16)+(prMvc[33]<<8)+prMvc[32];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
       u4HalValue=(prMvc[37]<<8)+prMvc[36];
       VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
   }

}

static VOID _VDEC_Vp8FlushCoefProbs(VDEC_VP8FRAME_CONTEXT_T *prCtxt)
{
   UINT32 u4BusSramAddr=0,u4CoefProb=0,u4Mod=0;
   UINT32 u4BlockType=0,u4Band=0,u4Context=0,u4Tolen=0;
   for(u4BlockType=0;u4BlockType<VP8_BLOCK_TYPES;u4BlockType++)
   {
      for(u4Band=0;u4Band<VP8_COEF_BANDS;u4Band++)
      {
         for(u4Context=0;u4Context<VP8_PREV_COEF_CONTEXTS;u4Context++)
         {  
         
            VP8_VLD_WRITE32(RW_VP8_BSASET,u4BusSramAddr);
            for(u4Tolen=0;u4Tolen<VP8_COEF_TOKENS-1;u4Tolen++)
            {
               u4Mod=u4Tolen%4;
               if(!u4Mod)
               {
                 u4CoefProb=0;
               }
               
               u4CoefProb+=prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][u4Tolen]<<(u4Mod<<3);
               if(u4Mod==3)
               {
                 VP8_VLD_WRITE32(RW_VP8_BSDSET,u4CoefProb);
               }
            }
            VP8_VLD_WRITE32(RW_VP8_BSDSET,u4CoefProb);
            u4BusSramAddr+=4;
         }
      }
   }
}

static VOID _VDEC_Vp8LoadCtxProbs(VDEC_VP8FRAME_CONTEXT_T *prCtxt)
{
  UINT32 u4HalValue=0;
  UINT8 *prMvc=(UINT8 *)prCtxt->MVC;
  
  VP8_VLD_WRITE32(RW_VP8_CSASET,4);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prCtxt->YModeProb[0]=(UINT8)(u4HalValue&0xff);
  prCtxt->YModeProb[1]=(UINT8)((u4HalValue>>8)&0xff);
  prCtxt->YModeProb[2]=(UINT8)((u4HalValue>>16)&0xff);
  prCtxt->YModeProb[3]=(UINT8)((u4HalValue>>24)&0xff);

  
  VP8_VLD_WRITE32(RW_VP8_CSASET,8);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prCtxt->UVModeProb[0]=(UINT8)(u4HalValue&0xff);
  prCtxt->UVModeProb[1]=(UINT8)((u4HalValue>>8)&0xff);
  prCtxt->UVModeProb[2]=(UINT8)((u4HalValue>>16)&0xff);


  u4HalValue=VP8_VLD_READ32(RW_VP8_MVPROB);
  prMvc[19]=(UINT8)(u4HalValue&0xff);
  prMvc[0]=(UINT8)((u4HalValue>>8)&0xff);
  prMvc[20]=(UINT8)((u4HalValue>>16)&0xff);
  prMvc[1]=(UINT8)((u4HalValue>>24)&0xff);

  
  VP8_VLD_WRITE32(RW_VP8_CSASET,0XC);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prMvc[2]=(UINT8)(u4HalValue&0xff);
  prMvc[3]=(UINT8)((u4HalValue>>8)&0xff);
  prMvc[4]=(UINT8)((u4HalValue>>16)&0xff);
  prMvc[5]=(UINT8)((u4HalValue>>24)&0xff);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prMvc[6]=(UINT8)(u4HalValue&0xff);
  prMvc[7]=(UINT8)((u4HalValue>>8)&0xff);
  prMvc[8]=(UINT8)((u4HalValue>>16)&0xff); 

  
  VP8_VLD_WRITE32(RW_VP8_CSASET,0X10);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prMvc[9]=(UINT8)(u4HalValue&0xff);
  prMvc[10]=(UINT8)((u4HalValue>>8)&0xff);
  prMvc[11]=(UINT8)((u4HalValue>>16)&0xff);
  prMvc[12]=(UINT8)((u4HalValue>>24)&0xff);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prMvc[13]=(UINT8)(u4HalValue&0xff);
  prMvc[14]=(UINT8)((u4HalValue>>8)&0xff);
  prMvc[15]=(UINT8)((u4HalValue>>16)&0xff);
  prMvc[16]=(UINT8)((u4HalValue>>24)&0xff);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prMvc[17]=(UINT8)(u4HalValue&0xff);
  prMvc[18]=(UINT8)((u4HalValue>>8)&0xff);

  
  VP8_VLD_WRITE32(RW_VP8_CSASET,0X14);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prMvc[21]=(UINT8)(u4HalValue&0xff);
  prMvc[22]=(UINT8)((u4HalValue>>8)&0xff);
  prMvc[23]=(UINT8)((u4HalValue>>16)&0xff);
  prMvc[24]=(UINT8)((u4HalValue>>24)&0xff);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prMvc[25]=(UINT8)(u4HalValue&0xff);
  prMvc[26]=(UINT8)((u4HalValue>>8)&0xff);
  prMvc[27]=(UINT8)((u4HalValue>>16)&0xff);

  
  VP8_VLD_WRITE32(RW_VP8_CSASET,0X18);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prMvc[28]=(UINT8)(u4HalValue&0xff);
  prMvc[29]=(UINT8)((u4HalValue>>8)&0xff);
  prMvc[30]=(UINT8)((u4HalValue>>16)&0xff);
  prMvc[31]=(UINT8)((u4HalValue>>24)&0xff);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prMvc[32]=(UINT8)(u4HalValue&0xff);
  prMvc[33]=(UINT8)((u4HalValue>>8)&0xff);
  prMvc[34]=(UINT8)((u4HalValue>>16)&0xff);
  prMvc[35]=(UINT8)((u4HalValue>>24)&0xff);
  u4HalValue=VP8_VLD_READ32(RW_VP8_CSDSET);
  prMvc[36]=(UINT8)(u4HalValue&0xff);
  prMvc[37]=(UINT8)((u4HalValue>>8)&0xff);
}

static VOID _VDEC_Vp8LoadCoefProbs(VDEC_VP8FRAME_CONTEXT_T *prCtxt)
{
   UINT32 u4BusSramAddr=0,u4CoefProb=0;
   UINT32 u4BlockType=0,u4Band=0,u4Context=0;
   for(u4BlockType=0;u4BlockType<VP8_BLOCK_TYPES;u4BlockType++)
   {
      for(u4Band=0;u4Band<VP8_COEF_BANDS;u4Band++)
      {
         for(u4Context=0;u4Context<VP8_PREV_COEF_CONTEXTS;u4Context++)
         {  
            VP8_VLD_WRITE32(RW_VP8_BSASET,u4BusSramAddr);
            u4CoefProb=VP8_VLD_READ32(RW_VP8_BSDSET);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][0]=(UINT8)(u4CoefProb&0xff);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][1]=(UINT8)((u4CoefProb>>8)&0xff);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][2]=(UINT8)((u4CoefProb>>16)&0xff);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][3]=(UINT8)((u4CoefProb>>24)&0xff);
            u4CoefProb=VP8_VLD_READ32(RW_VP8_BSDSET);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][4]=(UINT8)(u4CoefProb&0xff);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][5]=(UINT8)((u4CoefProb>>8)&0xff);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][6]=(UINT8)((u4CoefProb>>16)&0xff);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][7]=(UINT8)((u4CoefProb>>24)&0xff);
            u4CoefProb=VP8_VLD_READ32(RW_VP8_BSDSET);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][8]=(UINT8)(u4CoefProb&0xff);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][9]=(UINT8)((u4CoefProb>>8)&0xff);
            prCtxt->CoefProbs[u4BlockType][u4Band][u4Context][10]=(UINT8)((u4CoefProb>>16)&0xff);
            u4BusSramAddr+=4;
         }
      }
   }  
}

static VOID _VDEC_Vp8InitDeQTable(VDEC_PARAM_VP8DEC_T *pVp8DecParam,BOOL *fgChange)
{
  UINT32 u4Index=0,u4QType=0;
  INT16 i2Q=0,i2Qvalue=0,i2QNewIndex;

  for(u4QType=VP8_QTYPE_Y1AC;u4QType<VP8_QTYPE_MAX;u4QType++)
  {
     if(!fgChange[u4QType])
     {
        continue;
     }

     i2QNewIndex=pVp8DecParam->QIndexInfo[u4QType];
     if(i2QNewIndex==VP8_QTYPE_Y1AC)
     {
        i2QNewIndex=0;
     }
     
     if(u4QType==VP8_QTYPE_Y1DC|| u4QType==VP8_QTYPE_Y2DC || u4QType==VP8_QTYPE_UVDC)
     {
         for(u4Index=0;u4Index<VP8_QINDEX_RANGE;u4Index++)
         {
            i2Q=u4Index+i2QNewIndex;
            i2Q=VDEC_VP8_DQINDEXCLAMP(i2Q);
            i2Qvalue=_i2rVp8DcQLookup[i2Q];
            if(u4QType==VP8_QTYPE_Y2DC)
            {
               i2Qvalue<<=1;
            }
            else if(u4QType==VP8_QTYPE_UVDC)
            {
               i2Qvalue= i2Qvalue>132 ? 132 : i2Qvalue;
            }
            
            _rVp8Dequant[u4QType>>1][u4Index][0][0]=i2Qvalue;
         }
     }
     else
     {
#ifndef VP8_USE_SMALLQT
         UINT32 u4AcIndex=0,u4Row,u4Col;
         for(u4Index=0;u4Index<VP8_QINDEX_RANGE;u4Index++)
         {
           for(u4AcIndex=1;u4AcIndex<16;u4AcIndex++)
           {
              u4Row=_Vp8DefaultZigZag1d[u4AcIndex]>>2;
              u4Col=_Vp8DefaultZigZag1d[u4AcIndex]&3;
              i2Q=u4Index+i2QNewIndex;
              i2Q=VDEC_VP8_DQINDEXCLAMP(i2Q);
              i2Qvalue=_i2rVp8AcQLookup[i2Q];
              if(u4QType==VP8_QTYPE_Y2AC)
              {
                i2Qvalue=(i2Qvalue*155)/100;
                if(i2Qvalue<8)
                {
                  i2Qvalue=8;
                }
              }
              _rVp8Dequant[u4QType>>1][u4Index][u4Row][u4Col]=i2Qvalue;
           }
         }
#else
         for(u4Index=0;u4Index<VP8_QINDEX_RANGE;u4Index++)
         {
            i2Q=u4Index+i2QNewIndex;
            i2Q=VDEC_VP8_DQINDEXCLAMP(i2Q);
            i2Qvalue=_i2rVp8AcQLookup[i2Q];
            if(u4QType==VP8_QTYPE_Y2AC)
            {
               i2Qvalue=(i2Qvalue*155)/100;
               if(i2Qvalue<8)
               {
                 i2Qvalue=8;
               }
            }
            _rVp8Dequant[u4QType>>1][u4Index][0][1]=i2Qvalue;
         }
#endif
     }
  }

}


static VOID _VDEC_Vp8MbInitDequantizer(VDEC_PARAM_VP8DEC_T *prVp8DecParam)
{
   UINT32 u4SegIndex=0,u4Value;
   INT32 i4QIndex;
   if(1)
   {
      for(u4SegIndex=0;u4SegIndex<MAX_MB_SEGMENTS;u4SegIndex++)
      {
         if(VDEC_FLGSET(prVp8DecParam->u4FlagParam,VP8PARAM_SEGMENT_ENABLE))
         {
            if(VDEC_FLGSET(prVp8DecParam->u4FlagParam,VP8PARAM_SEGMENT_ABSDATA))
            {
               i4QIndex=prVp8DecParam->SegmentFeatureData[VP8_MBLVL_ALT_Q][u4SegIndex];
            }
            else
            {
               i4QIndex=prVp8DecParam->QIndexInfo[VP8_QTYPE_Y1AC]+prVp8DecParam->SegmentFeatureData[VP8_MBLVL_ALT_Q][u4SegIndex];
               i4QIndex = VDEC_VP8_DQINDEXCLAMP(i4QIndex);
            }
         }
         else
         {
            i4QIndex=prVp8DecParam->QIndexInfo[VP8_QTYPE_Y1AC];
         }

         
         u4Value=(_rVp8Dequant[VP8_QTABLE_Y2][i4QIndex][0][0]>>8)&0xff;
         u4Value=(u4Value<<8)|(_rVp8Dequant[VP8_QTABLE_Y2][i4QIndex][0][0]&0xff);
         u4Value=(u4Value<<8)|((_rVp8Dequant[VP8_QTABLE_Y2][i4QIndex][0][1]>>8)&0xff);
         u4Value=(u4Value<<8)|(_rVp8Dequant[VP8_QTABLE_Y2][i4QIndex][0][1]&0xff);         
         VDEC_VLD_WRITE32(RW_VLD_SCL_ADDR,u4SegIndex<<4);
         VDEC_VLD_WRITE32(RW_VLD_SCL_DATA,u4Value);

         u4Value=(_rVp8Dequant[VP8_QTABLE_Y1][i4QIndex][0][0]>>8)&0xff;
         u4Value=(u4Value<<8)|(_rVp8Dequant[VP8_QTABLE_Y1][i4QIndex][0][0]&0xff);
         u4Value=(u4Value<<8)|((_rVp8Dequant[VP8_QTABLE_Y1][i4QIndex][0][1]>>8)&0xff);
         u4Value=(u4Value<<8)|(_rVp8Dequant[VP8_QTABLE_Y1][i4QIndex][0][1]&0xff);
         VDEC_VLD_WRITE32(RW_VLD_SCL_ADDR,(u4SegIndex<<4)+4);
         VDEC_VLD_WRITE32(RW_VLD_SCL_DATA,u4Value);

         u4Value=(_rVp8Dequant[VP8_QTABLE_UV][i4QIndex][0][0]>>8)&0xff;
         u4Value=(u4Value<<8)|(_rVp8Dequant[VP8_QTABLE_UV][i4QIndex][0][0]&0xff);
         u4Value=(u4Value<<8)|((_rVp8Dequant[VP8_QTABLE_UV][i4QIndex][0][1]>>8)&0xff);
         u4Value=(u4Value<<8)|(_rVp8Dequant[VP8_QTABLE_UV][i4QIndex][0][1]&0xff);
         VDEC_VLD_WRITE32(RW_VLD_SCL_ADDR,(u4SegIndex<<4)+8);
         VDEC_VLD_WRITE32(RW_VLD_SCL_DATA,u4Value);
      }
   }
}

#if 0
static VOID _VDEC_Vp8PrintRegValue(VDEC_INFO_VP8_FRM_HDR_T *pVp8DecInfo)
{
    UINT32 i=0;
    printf("******* VP8 VLD Registers *******\n");
    for (i = 41; i < 68; i++)
    {
        printf("VP8 VLD %d (0x%04x): 0x%08x\n", i, i * 4, VP8_VLD_READ32(i * 4));
    }
    
    for (i = 72; i < 97; i++)
    {
       printf("VP8 VLD %d (0x%04x): 0x%08x\n", i, i * 4, VP8_VLD_READ32(i * 4));
    }

    Printf("********VP8 PP Registers *************\n");
    printf("VP8 PP %d (0x%04x): 0x%08x\n", 8, 8 * 4, VDEC_PP_READ32(8 * 4));
    printf("VP8 PP %d (0x%04x): 0x%08x\n", 9, 9 * 4, VDEC_PP_READ32(9 * 4));
    printf("VP8 PP %d (0x%04x): 0x%08x\n", 10, 10 * 4, VDEC_PP_READ32(10 * 4));
    printf("VP8 PP %d (0x%04x): 0x%08x\n", 11, 11 * 4, VDEC_PP_READ32(11 * 4));


    printf("******* MV Prob *******\n");
    _VDEC_Vp8LoadCtxProbs(&pVp8DecInfo->rVp8DecParam.rLastFC);
    for(i=0;i<VP8_YMODES-1;i++)
    {
      printf("YModeProb(%d)=0x%x\n",pVp8DecInfo->rVp8DecParam.rLastFC.YModeProb[i]);
    }

    for(i=0;i<VP8_UVMODES-1;i++)
    {
        printf("UVModeProb(%d)=0x%x\n",pVp8DecInfo->rVp8DecParam.rLastFC.UVModeProb[i]);
    }

    for(i=0;i<VP8_MVDEF_MVPCOUNT;i++)
    {
        printf("MVC0Prob(%d)=0x%x\n",pVp8DecInfo->rVp8DecParam.rLastFC.MVC[0].Prob[i]);
    }
    
    for(i=0;i<VP8_MVDEF_MVPCOUNT;i++)
    {
        printf("MVC1Prob(%d)=0x%x\n",pVp8DecInfo->rVp8DecParam.rLastFC.MVC[1].Prob[i]);
    }

    printf("******* Coefficient Prob *******\n");
    
    _VDEC_Vp8FlushCoefProbs(&pVp8DecInfo->rVp8DecParam.rLastFC);
    {
        UINT32 u4BlockType=0,u4Band=0,u4Context=0,u4Tolen=0;
        for(u4BlockType=0;u4BlockType<VP8_BLOCK_TYPES;u4BlockType++)
        {
           for(u4Band=0;u4Band<VP8_COEF_BANDS;u4Band++)
           {
              for(u4Context=0;u4Context<VP8_PREV_COEF_CONTEXTS;u4Context++)
              {  
                  for(u4Tolen=0;u4Tolen<VP8_COEF_TOKENS-1;u4Tolen++)
                  {
                     printf("CoefProb[%d][%d][%d][%d]=0x%x\n",u4BlockType,u4Band,u4Context,u4Tolen, \
                        pVp8DecInfo->rVp8DecParam.rLastFC.CoefProbs[u4BlockType][u4Band][u4Context][u4Tolen]);
                  }
              }
           }
        }       
    }
    printf("****** Dump VLD Registers End*******\n");

}
#endif

#if defined(VP8_ENABLE_CRC) || defined(WEBP_ENABLE_CRC)
static VOID _VDEC_Vp8GetCrcValue(UINT32 puCrcBuf[][4],UINT32 *pu4DbkType)
{
   if(!puCrcBuf)
   {
     return;
   }
   
   puCrcBuf[0][0]=VDEC_MISC_READ32(RW_CRC_Y_0);
   puCrcBuf[0][1]=VDEC_MISC_READ32(RW_CRC_Y_1);
   puCrcBuf[0][2]=VDEC_MISC_READ32(RW_CRC_Y_2);
   puCrcBuf[0][3]=VDEC_MISC_READ32(RW_CRC_Y_3);

   puCrcBuf[1][0]=VDEC_MISC_READ32(RW_CRC_C_0);
   puCrcBuf[1][1]=VDEC_MISC_READ32(RW_CRC_C_1);
   puCrcBuf[1][2]=VDEC_MISC_READ32(RW_CRC_C_2);
   puCrcBuf[1][3]=VDEC_MISC_READ32(RW_CRC_C_3); 

   if(pu4DbkType)
   {
     *pu4DbkType=VDEC_MC_READ32(RW_MC_PP_DBLK_MODE);
   }

   return;
}
#endif


static UINT32 _VDEC_Vp8GetBitCnt(UINT32 u4VldId)
{
   if(u4VldId==VDEC_VLD1)
   {
      return VP8_VLD_READ32(RO_VP8_VLD1_BITCNT);
   }
   else
   {
      return VP8_VLD_READ32(RO_VP8_PART_BITCNT);
   }
}

static BOOL _VDEC_Vp8DataExhaust(VOID)
{
   UINT32 u4ReadPos=0,u4WritePos=0;
   //UINT32 u4Status=0,u4WaitTime=0;
   
   u4WritePos=VDEC_VLD2_READ32(WO_VLD_WPTR);
   u4ReadPos=VDEC_VLD2_READ32(RO_VLD_VRPTR);
   if(u4ReadPos!=u4WritePos)
   {
      return FALSE;
   }
   else
   {
    
     u4ReadPos=VDEC_VLD2_READ32(RO_VLD_CS);
     if(u4ReadPos!=u4WritePos)
     {
        return FALSE;
     }
	 else
	 {
		 return TRUE;
	 }

/*
     do
     {
        u4Status=VDEC_VLD2_READ32(RO_VLD_STATUS);
        //if((u4Status&0xffff)==0x2000)
        if((u4Status&0xffff)==0x800)
        {
           return TRUE;
        }
        else
        {
           u4WaitTime++;
           x_thread_delay(5);
        }

        if(u4WaitTime>10)
        {
           break;
        }
     }while(1);	 
	 return FALSE;
*/
   }
}

static BOOL _VDEC_Vp8DecWait(UINT32 u4TimeOut)
{
   UINT32 fgDecFin=TRUE;
#ifdef SUPPORT_DEC_ISR
   if(x_sema_lock_timeout(_hVP8VdecFinSema,u4TimeOut)!=OSR_OK)
   {
     fgDecFin=FALSE;
     LOG(3,"VP8 webm decoder time out %d\n",u4TimeOut);
   }
   else
   {
     fgDecFin=TRUE;
   }
#else
   x_thread_delay(u4TimeOut);
#endif
   return fgDecFin;
}

static VOID _VDEC_Vp8VersonCfg(UINT32 u4Verson,VDEC_PARAM_VP8DEC_T *pVP8Param)
{
    switch(u4Verson)
    {
       case 0:
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_NO_LPF);
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_SIMPLER_LPF);
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_BILINER_MCFILTER);
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_FULL_PIXEL);
         break;
       case 1:
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_NO_LPF);
         VDEC_SETFLG(pVP8Param->u4FlagParam,VP8PARAM_SIMPLER_LPF);
         VDEC_SETFLG(pVP8Param->u4FlagParam,VP8PARAM_BILINER_MCFILTER);
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_FULL_PIXEL);
         break;
       case 2:
         VDEC_SETFLG(pVP8Param->u4FlagParam,VP8PARAM_NO_LPF);
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_SIMPLER_LPF);
         VDEC_SETFLG(pVP8Param->u4FlagParam,VP8PARAM_BILINER_MCFILTER);
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_FULL_PIXEL);        
         break;
       case 3:
         VDEC_SETFLG(pVP8Param->u4FlagParam,VP8PARAM_NO_LPF);
         VDEC_SETFLG(pVP8Param->u4FlagParam,VP8PARAM_SIMPLER_LPF);
         VDEC_SETFLG(pVP8Param->u4FlagParam,VP8PARAM_BILINER_MCFILTER);
         VDEC_SETFLG(pVP8Param->u4FlagParam,VP8PARAM_FULL_PIXEL);        
         break;
       default:
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_NO_LPF);
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_SIMPLER_LPF);
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_BILINER_MCFILTER);
         VDEC_CLRFLG(pVP8Param->u4FlagParam,VP8PARAM_FULL_PIXEL);
         break;
    }
}


static UINT32 _VDEC_Vp8DecInit(VDEC_INFO_VP8_FRM_HDR_T *pVp8DecInfo)
{
  UINT32 u4FlagParam=0;
  VDEC_VP8FRAME_CONTEXT_T *prCtx=&pVp8DecInfo->rVp8DecParam.rCurFc;
  BOOL fgQChange[VP8_QTYPE_MAX]={TRUE,TRUE,TRUE,TRUE,TRUE,TRUE};
  x_memset(pVp8DecInfo,0,sizeof(VDEC_INFO_VP8_FRM_HDR_T));
  VDEC_SETFLG(u4FlagParam,VP8PARAM_NOCOEF_SKIP);
  VDEC_SETFLG(u4FlagParam,VP8PARAM_REFRESH_PROBS);
  _VDEC_Vp8InitDeQTable(&pVp8DecInfo->rVp8DecParam,fgQChange);
  //x_memcpy(pVp8DecInfo->rVp8DecParam.rCurFc.PRE_MVC,vp8_default_mv_context,sizeof(vp8_default_mv_context));
  x_memcpy(prCtx->MVC,vp8_default_mv_context,sizeof(vp8_default_mv_context));
  x_memcpy(prCtx->CoefProbs,default_coef_probs,sizeof(default_coef_probs));
  prCtx->YModeProb[0]=112;// {112,86,140,37};
  prCtx->YModeProb[1]=86;
  prCtx->YModeProb[2]=140;
  prCtx->YModeProb[3]=37;
  prCtx->UVModeProb[0]=162;//{162,101,204};
  prCtx->UVModeProb[1]=101;
  prCtx->UVModeProb[2]=204;
  x_memcpy(pVp8DecInfo->rVp8DecParam.rCurFc.CoefProbs,default_coef_probs,sizeof(default_coef_probs));
  x_memcpy(&pVp8DecInfo->rVp8DecParam.rLastFC,&pVp8DecInfo->rVp8DecParam.rCurFc,sizeof(VDEC_VP8FRAME_CONTEXT_T));
  x_memset(pVp8DecInfo->rVp8DecParam.SegmentTreeProbs,255,MB_FEATURE_TREE_PROBS);
  pVp8DecInfo->rVp8DecParam.u4FlagParam=u4FlagParam;
  return 0;
}

static UINT32 _VDEC_Vp8DecHeader(VDEC_INFO_VP8_FRM_HDR_T *pVp8DecInfo)
{
  VDEC_PARAM_VP8DEC_T *prVp8DecParam=&pVp8DecInfo->rVp8DecParam;
  UINT32 u4HalValue=0,u4ReadOffset;
  UINT32 u4FlagParam=prVp8DecParam->u4FlagParam;

  u4HalValue=_VDEC_VP8_VLDShiftBit(VDEC_VLD1,24);
  VDEC_INTREVERSE(u4HalValue,3);
  pVp8DecInfo->uFrameType=u4HalValue&1;
  pVp8DecInfo->uVersion=(u4HalValue>>1)&7;
  pVp8DecInfo->uShowFrame=(u4HalValue>>4)&1;
  pVp8DecInfo->u4rPartitionSize[0]=u4HalValue>>5;
  pVp8DecInfo->u4FrameSize-=3;
  u4ReadOffset=3;
  pVp8DecInfo->u4FrameSize-=pVp8DecInfo->u4rPartitionSize[0];
  //VDEC_RPOS_INC(pVp8DecInfo->u4VldStartPos,3,pVp8DecInfo->u4FifoStart,pVp8DecInfo->u4FifoEnd);
  //VDEC_RPOS_INC(pVp8DecInfo->u4VldStartPos,pVp8DecInfo->u4rPartitionSize[0],pVp8DecInfo->u4FifoStart,pVp8DecInfo->u4FifoEnd);
  _VDEC_Vp8VersonCfg(pVp8DecInfo->uVersion,prVp8DecParam);
  u4FlagParam=prVp8DecParam->u4FlagParam;
  if(pVp8DecInfo->uFrameType==VP68_KEY_FRAME)
  {
     u4HalValue=_VDEC_VP8_VLDShiftBit(VDEC_VLD1,24);
     if((u4HalValue)!=0x9d012a)
     {
         LOG(3,"VP8 Header Identify error (0x%x)\n",u4HalValue);
         return FALSE;
     }
     
     u4HalValue=_VDEC_VP8_VLDShiftBit(VDEC_VLD1,16);
     VDEC_INTREVERSE(u4HalValue,2);
     pVp8DecInfo->u2Width=u4HalValue&0x3fff;
     pVp8DecInfo->uHScale=u4HalValue>>14;
     u4HalValue=_VDEC_VP8_VLDShiftBit(VDEC_VLD1,16);
     VDEC_INTREVERSE(u4HalValue,2);
     pVp8DecInfo->u2Height=u4HalValue&0x3fff;
     pVp8DecInfo->uVScale=u4HalValue>>14;     
     pVp8DecInfo->u4FrameSize-=7;
     if(pVp8DecInfo->u2Width==0 || pVp8DecInfo->u2Height==0)
     {
        LOG(3,"VP8 Header W/H error \n");
        return FALSE;
     }
     u4ReadOffset+=7;
     //VDEC_RPOS_INC(pVp8DecInfo->u4VldStartPos,7,pVp8DecInfo->u4FifoStart,pVp8DecInfo->u4FifoEnd);
  }
  
  u4HalValue=0;
  VDEC_REG_SET_VALUE(u4HalValue,((pVp8DecInfo->u2Width+15)>>4)-1,RW_PIC_WIDTH_IN_MBS_S,RW_PIC_WIDTH_IN_MBS_E);
  VDEC_REG_SET_VALUE(u4HalValue,((pVp8DecInfo->u2Height+15)>>4)-1,RW_PIC_HEIGHT_IN_MBS_S,RW_PIC_HEIGHT_IN_MBS_E);
  VDEC_VLD_TOP_WRITE32(RW_PIC_MB_SIZE_M1,u4HalValue);
  
  u4HalValue=0;
  VDEC_REG_SET_VALUE(u4HalValue,(pVp8DecInfo->u4rPartitionSize[0]+u4ReadOffset),RW_VP8_FSTPB_S,RW_VP8_FSTPB_E);
  VDEC_REG_SET_VALUE(u4HalValue,pVp8DecInfo->uVersion,RW_VP8_VERSION_S,RW_VP8_VERSION_E);
  VDEC_REG_SET_FLAG(u4HalValue,pVp8DecInfo->uFrameType,RW_VP8_PICTYPE_FLAG);
  VP8_VLD_WRITE32(RW_VP8_HEADER2,u4HalValue);
  
  if(pVp8DecInfo->uFrameType==VP68_KEY_FRAME)
  {
    _VDEC_Vp8FlushCtxProbs(NULL,VP68_KEY_FRAME);
  }
  
  _VDEC_VP8_InitBoolDecoder(VDEC_VLD1);
  
  VDEC_RPOS_INC(pVp8DecInfo->u4VldStartPos,u4ReadOffset,pVp8DecInfo->u4FifoStart,pVp8DecInfo->u4FifoEnd);
#ifdef USE_VP68_SW_VLD
  _VDEC_VP68_InitSwBoolDecoder(VDEC_VLD1,pVp8DecInfo->u4VldStartPos,pVp8DecInfo->u4FifoStart,pVp8DecInfo->u4FifoEnd);
#endif

  if(pVp8DecInfo->uFrameType==VP68_KEY_FRAME)
  {
    VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_COLOR,_VDEC_VP8_VLDReadLiteral(1));
    VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_CLAMP_TYPE,_VDEC_VP8_VLDReadLiteral(1));
    x_memcpy(prVp8DecParam->rCurFc.MVC,vp8_default_mv_context,sizeof(vp8_default_mv_context));
    x_memcpy(pVp8DecInfo->rVp8DecParam.rCurFc.CoefProbs,default_coef_probs,sizeof(default_coef_probs));
    prVp8DecParam->rCurFc.YModeProb[0]=112;// {112,86,140,37};
    prVp8DecParam->rCurFc.YModeProb[1]=86;
    prVp8DecParam->rCurFc.YModeProb[2]=140;
    prVp8DecParam->rCurFc.YModeProb[3]=37;
    prVp8DecParam->rCurFc.UVModeProb[0]=162;// {162,101,204};
    prVp8DecParam->rCurFc.UVModeProb[1]=101;
    prVp8DecParam->rCurFc.UVModeProb[2]=204;
    x_memset(prVp8DecParam->SegmentFeatureData,0,sizeof(prVp8DecParam->SegmentFeatureData));
    VDEC_CLRFLG(u4FlagParam,VP8PARAM_SEGMENT_ABSDATA);// use delta data
    x_memset(prVp8DecParam->RefLFDeltas,0,sizeof(prVp8DecParam->RefLFDeltas));
    x_memset(prVp8DecParam->ModeLFDeltas,0,sizeof(prVp8DecParam->ModeLFDeltas));
    VDEC_SETFLG(u4FlagParam,VP8PARAM_REFRESH_GOLDEN);
    VDEC_SETFLG(u4FlagParam,VP8PARAM_REFRESH_ALTRF);
    prVp8DecParam->uCopyBuf2Gf=0;
    prVp8DecParam->uCopyBuf2Arf=0;
    if(VDEC_FLGSET(u4FlagParam,VP8PARAM_COLOR))
    {
      LOG(3,"VP8 Header Color Space unsupport\n");
      return FALSE;
    }
  }

  VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_SEGMENT_ENABLE,_VDEC_VP8_VLDReadLiteral(1));

  if(VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_ENABLE))
  {
     VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_SEGMENT_UPDATE_MAP,_VDEC_VP8_VLDReadLiteral(1));
     VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_SEGMENT_UPDATE_DATA,_VDEC_VP8_VLDReadLiteral(1));
     
     if(VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_UPDATE_DATA))
     {
       UINT32 u4LvlType,u4SegIndex;
       VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_SEGMENT_ABSDATA,_VDEC_VP8_VLDReadLiteral(1));
       for(u4LvlType=0;u4LvlType<VP8_MBLVL_MAX;u4LvlType++)
       {
         for(u4SegIndex=0;u4SegIndex<MAX_MB_SEGMENTS;u4SegIndex++)
         {
            prVp8DecParam->SegmentFeatureData[u4LvlType][u4SegIndex]=0;
            if(_VDEC_VP8_VLDReadLiteral(1))
            {
               prVp8DecParam->SegmentFeatureData[u4LvlType][u4SegIndex]=_VDEC_VP8_VLDReadLiteral(_Vp8MbFeatureDataBits[u4LvlType]);
               if(_VDEC_VP8_VLDReadLiteral(1))
               {
                  prVp8DecParam->SegmentFeatureData[u4LvlType][u4SegIndex]=-prVp8DecParam->SegmentFeatureData[u4LvlType][u4SegIndex];
               }
            }
         }
       }
     }
     else
     {
        VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_SEGMENT_ABSDATA,VDEC_FLGSET(prVp8DecParam->u4LastParam,VP8PARAM_SEGMENT_ABSDATA));
     }

     if(VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_UPDATE_MAP))
     {
       UINT32 u4ProbIndex=0;
       for(;u4ProbIndex<MB_FEATURE_TREE_PROBS;u4ProbIndex++)
       {
         prVp8DecParam->SegmentTreeProbs[u4ProbIndex]=255;
         if(_VDEC_VP8_VLDReadLiteral(1))
         {
            prVp8DecParam->SegmentTreeProbs[u4ProbIndex]=(UINT8)_VDEC_VP8_VLDReadLiteral(8);
         }
       }
     }
     
     u4HalValue=(prVp8DecParam->SegmentTreeProbs[2]<<16)+(prVp8DecParam->SegmentTreeProbs[1]<<8)
                +prVp8DecParam->SegmentTreeProbs[0];
     //ctx_stram address: SET_ID_PROB_OFFSET
     VP8_VLD_WRITE32(RW_VP8_CSASET,0);
     VP8_VLD_WRITE32(RW_VP8_CSDSET,u4HalValue);
  }
  /*
  else
  {
     VDEC_CLRFLG(u4FlagParam,VP8PARAM_SEGMENT_UPDATE_MAP);
     VDEC_CLRFLG(u4FlagParam,VP8PARAM_SEGMENT_UPDATE_DATA);
     VDEC_CLRFLG(u4FlagParam,VP8PARAM_SEGMENT_ABSDATA);
  }
  */
  prVp8DecParam->eLoopFilterType=(VDEC_VP8_FILTER_TYPE_T)_VDEC_VP8_VLDReadLiteral(1);
  prVp8DecParam->iLoopFilterLvl=_VDEC_VP8_VLDReadLiteral(6);
  prVp8DecParam->iSharpLvl=_VDEC_VP8_VLDReadLiteral(3);

  VDEC_CLRFLG(u4FlagParam,VP8PARAM_MODEREF_IFDELTA_UPDATE);
  VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_MODEREF_LFDELTA_ENABLE,_VDEC_VP8_VLDReadLiteral(1));

  if(VDEC_FLGSET(u4FlagParam,VP8PARAM_MODEREF_LFDELTA_ENABLE))
  {
     VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_MODEREF_IFDELTA_UPDATE,_VDEC_VP8_VLDReadLiteral(1));
     if(VDEC_FLGSET(u4FlagParam,VP8PARAM_MODEREF_IFDELTA_UPDATE))
     {
        UINT32 u4DeltaIndex=0;
        for(u4DeltaIndex=0;u4DeltaIndex<MAX_REF_LF_DELTAS;u4DeltaIndex++)
        {
           if(_VDEC_VP8_VLDReadLiteral(1))
           {
             prVp8DecParam->RefLFDeltas[u4DeltaIndex]=(INT8)_VDEC_VP8_VLDReadLiteral(6);
             if(_VDEC_VP8_VLDReadLiteral(1))
             {
                prVp8DecParam->RefLFDeltas[u4DeltaIndex]=prVp8DecParam->RefLFDeltas[u4DeltaIndex]*(-1);
             }
           }
        }

        for(u4DeltaIndex=0;u4DeltaIndex<MAX_MODE_LF_DELTAS;u4DeltaIndex++)
        {
            if(_VDEC_VP8_VLDReadLiteral(1))
            {
              prVp8DecParam->ModeLFDeltas[u4DeltaIndex]=(INT8)_VDEC_VP8_VLDReadLiteral(6);
              if(_VDEC_VP8_VLDReadLiteral(1))
              {
                 prVp8DecParam->ModeLFDeltas[u4DeltaIndex]=prVp8DecParam->ModeLFDeltas[u4DeltaIndex]*(-1);
              }
            }
        }
     }
  }

  VDEC_RPOS_INC(pVp8DecInfo->u4VldStartPos,pVp8DecInfo->u4rPartitionSize[0],pVp8DecInfo->u4FifoStart,pVp8DecInfo->u4FifoEnd);

  while(!_VDEC_VP8_InitBarrelShifter(VDEC_VLD2,pVp8DecInfo->u4VldStartPos,pVp8DecInfo->u4WritePos))
  {
      LOG(5,"VP8 Init BarrelShifter fail\n");
      _VDEC_VP8_VLDReset(VDEC_VLD2);
  }
#ifdef USE_VP68_SW_VLD
      _VDEC_VP68_InitSwBarrelShifter(VDEC_VLD2,pVp8DecInfo->u4VldStartPos,pVp8DecInfo->u4FifoStart,pVp8DecInfo->u4FifoEnd);
      _VDEC_VP68_InitSwBoolDecoder(VDEC_VLD2,pVp8DecInfo->u4VldStartPos,pVp8DecInfo->u4FifoStart,pVp8DecInfo->u4FifoEnd);
#endif

  // address or offset ?
  prVp8DecParam->uDataPartitionToken=_VDEC_VP8_VLDReadLiteral(2);
  prVp8DecParam->uDataPartitionNum=1<<prVp8DecParam->uDataPartitionToken;
  u4ReadOffset=3*(prVp8DecParam->uDataPartitionNum-1);
  VDEC_RPOS_INC(pVp8DecInfo->u4VldStartPos,u4ReadOffset,pVp8DecInfo->u4FifoStart,pVp8DecInfo->u4FifoEnd);
  VP8_VLD_WRITE32(RW_VP8_COEFR0,pVp8DecInfo->u4VldStartPos);
  
  pVp8DecInfo->u4FrameSize-=u4ReadOffset;
  ASSERT(prVp8DecParam->uDataPartitionNum<=MAX_VP8_DATAPARTITION-1);
  {
    UINT32 u4PartionOffset=0,u4Index=0;
    for(u4Index=0;u4Index<prVp8DecParam->uDataPartitionNum;u4Index++)
    {
      if(u4Index<prVp8DecParam->uDataPartitionNum-1)
      {
          u4HalValue=_VDEC_VP8_VLDShiftBit(VDEC_VLD2,24);
          VDEC_INTREVERSE(u4HalValue,3);
          VP8_VLD_WRITE32(RW_VP8_COEFPB0+u4Index*4,u4HalValue);
          u4PartionOffset=VP8_VLD_READ32(RW_VP8_COEFR0+u4Index*4);
          VDEC_RPOS_INC(u4PartionOffset,u4HalValue,pVp8DecInfo->u4FifoStart,pVp8DecInfo->u4FifoEnd);
		  if(u4Index+1<MAX_VP8_DATAPARTITION)
		  {
			  pVp8DecInfo->u4rPartitionSize[u4Index+1]=u4HalValue;
		  }
          VP8_VLD_WRITE32(RW_VP8_COEFR1+u4Index*4,u4PartionOffset);
          ASSERT(pVp8DecInfo->u4FrameSize>=u4HalValue);
          pVp8DecInfo->u4FrameSize-=u4HalValue;
      }
      else
      {
          VP8_VLD_WRITE32(RW_VP8_COEFPB0+u4Index*4,pVp8DecInfo->u4FrameSize);
		  
		  if(u4Index+1<MAX_VP8_DATAPARTITION)
		  {
			  pVp8DecInfo->u4rPartitionSize[u4Index+1]=pVp8DecInfo->u4FrameSize;
		  }
      }
    }
  }

  if((!VDEC_FLGSET(u4FlagParam,VP8PARAM_WEBP_MODE)) || (!VDEC_FLGSET(u4FlagParam,VP8PARAM_WEBP_RESET_BUF)))
  {
      u4HalValue=VDEC_GET_FLAGVAL(1,RW_VP8_BCRT2_FLAG);
      VP8_VLD_WRITE32(RW_VP8_BCRT,u4HalValue);
      VP8_VLD_WRITE32(RW_VP8_BCRT,0);
      _VDEC_VP8_InitBoolDecoder(VDEC_VLD2);

  }
  
  {
     UINT32 u4Index=0;
     BOOL fgQupdate=FALSE;
     BOOL fgQChange[VP8_QTYPE_MAX]={FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};
     INT16 i2QValue=0;
     prVp8DecParam->QIndexInfo[VP8_QTYPE_Y1AC]=(INT16)_VDEC_VP8_VLDReadLiteral(7);
     for(u4Index=VP8_QTYPE_Y1DC;u4Index<VP8_QTYPE_MAX;u4Index++)
     {
        i2QValue=prVp8DecParam->QIndexInfo[u4Index];
        if(_VDEC_VP8_VLDReadLiteral(1))
        {
           i2QValue=(INT16)_VDEC_VP8_VLDReadLiteral(4);
           if(_VDEC_VP8_VLDReadLiteral(1))
           {
             i2QValue=-i2QValue; 
           }
        }
        
        if(i2QValue!=prVp8DecParam->QIndexInfo[u4Index])
        {
           fgQupdate=TRUE;
           fgQChange[u4Index]=TRUE;
           prVp8DecParam->QIndexInfo[u4Index]=i2QValue;
        }
     }

     prVp8DecParam->u4FlagParam = u4FlagParam;
     VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_QINDEX_UPDATE,fgQupdate);
     if(fgQupdate)
     {
        _VDEC_Vp8InitDeQTable(prVp8DecParam,fgQChange);
     }
     _VDEC_Vp8MbInitDequantizer(prVp8DecParam);
     u4FlagParam=prVp8DecParam->u4FlagParam;
  }

  if(pVp8DecInfo->uFrameType!=VP68_KEY_FRAME)
  {
      VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_REFRESH_GOLDEN,_VDEC_VP8_VLDReadLiteral(1));
      VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_REFRESH_ALTRF,_VDEC_VP8_VLDReadLiteral(1));
      prVp8DecParam->uCopyBuf2Gf=0;
      
      if(!VDEC_FLGSET(u4FlagParam,VP8PARAM_REFRESH_GOLDEN))
      {
         prVp8DecParam->uCopyBuf2Gf=_VDEC_VP8_VLDReadLiteral(2);
      }
      prVp8DecParam->uCopyBuf2Arf=0;
      if(!VDEC_FLGSET(u4FlagParam,VP8PARAM_REFRESH_ALTRF))
      {
         prVp8DecParam->uCopyBuf2Arf=_VDEC_VP8_VLDReadLiteral(2);
      }
   
      prVp8DecParam->RefFrameSignBias[VP8_MVREF_GOLDEN_FRAME]=_VDEC_VP8_VLDReadLiteral(1);
      prVp8DecParam->RefFrameSignBias[VP8_MVREF_ALTREF_FRAME]=_VDEC_VP8_VLDReadLiteral(1);
      u4HalValue=(prVp8DecParam->RefFrameSignBias[VP8_MVREF_ALTREF_FRAME]<<1)
                 +prVp8DecParam->RefFrameSignBias[VP8_MVREF_GOLDEN_FRAME];
      VDEC_MV_WRITE32(RW_VP8_MV_RFSB,u4HalValue);
  }

  VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_REFRESH_PROBS,_VDEC_VP8_VLDReadLiteral(1));
  
  if(!VDEC_FLGSET(u4FlagParam,VP8PARAM_REFRESH_PROBS))
  {
     x_memcpy(&prVp8DecParam->rLastFC,&prVp8DecParam->rCurFc,sizeof(VDEC_VP8FRAME_CONTEXT_T));
  }
  
  if(pVp8DecInfo->uFrameType==VP68_KEY_FRAME)
  {
     VDEC_SETFLG(u4FlagParam,VP8PARAM_REFRESH_LASTFRAME);
  }
  else
  {
     VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_REFRESH_LASTFRAME,_VDEC_VP8_VLDReadLiteral(1));
  }

  _VDEC_Vp8FlushCoefProbs(&prVp8DecParam->rCurFc);
  
#ifndef VP8_HEADERPARSE_HWACCELATOR
  {
    UINT32 u4BlockType=0,u4Band=0,u4Context=0,u4Tolen=0;
    for(u4BlockType=0;u4BlockType<VP8_BLOCK_TYPES;u4BlockType++)
    {
       for(u4Band=0;u4Band<VP8_COEF_BANDS;u4Band++)
       {
          for(u4Context=0;u4Context<VP8_PREV_COEF_CONTEXTS;u4Context++)
          {
             for(u4Tolen=0;u4Tolen<VP8_COEF_TOKENS-1;u4Tolen++)
             {
                if(_VDEC_VP8_VLDReadBit(_uVp8CoefUpdateProbs[u4BlockType][u4Band][u4Context][u4Tolen]))
                {
                   prVp8DecParam->rCurFc.CoefProbs[u4BlockType][u4Band][u4Context][u4Tolen]=_VDEC_VP8_VLDReadLiteral(8);
                }
             }
          }
       }
    }
  }
  _VDEC_Vp8FlushCoefProbs(&prVp8DecParam->rCurFc);
#else
  _VDEC_VP8_HwAccCoefProbUpdate();
#endif
  
  VDEC_SETFLG_COND(u4FlagParam,VP8PARAM_NOCOEF_SKIP,_VDEC_VP8_VLDReadLiteral(1));

  u4HalValue=0;
  VDEC_REG_SET_FLAG(u4HalValue,VDEC_FLGSET(u4FlagParam,VP8PARAM_CLAMP_TYPE),RW_VP8_CLTYPE_FLAG);
  VDEC_REG_SET_FLAG(u4HalValue,VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_ENABLE),RW_VP8_SEGEN_FLAG);
  VDEC_REG_SET_FLAG(u4HalValue,VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_UPDATE_MAP),RW_VP8_UMBSM_FLAG);
  VDEC_REG_SET_FLAG(u4HalValue,VDEC_FLGSET(u4FlagParam,VP8PARAM_NOCOEF_SKIP),RW_VP8_MBSKIP_FLAG);
  VDEC_REG_SET_VALUE(u4HalValue,prVp8DecParam->uDataPartitionToken,RW_VP8_MTP_S,RW_VP8_MTP_E);
  VP8_VLD_WRITE32(RW_VP8_HEADER3,u4HalValue);
  
  if(pVp8DecInfo->uFrameType==VP68_KEY_FRAME)
  {
     if(VDEC_FLGSET(u4FlagParam,VP8PARAM_NOCOEF_SKIP))
     {
       prVp8DecParam->uSkipFalseProb=_VDEC_VP8_VLDReadLiteral(8);
       VP8_VLD_WRITE32(RW_VP8_PICPROB,(UINT32)prVp8DecParam->uSkipFalseProb);
     }
  }
  else
  {
      prVp8DecParam->uSkipFalseProb=0;
      if(VDEC_FLGSET(u4FlagParam,VP8PARAM_NOCOEF_SKIP))
      {
        prVp8DecParam->uSkipFalseProb=_VDEC_VP8_VLDReadLiteral(8);
      }
      
      prVp8DecParam->uIntraProb=_VDEC_VP8_VLDReadLiteral(8);
      prVp8DecParam->uLastProb=_VDEC_VP8_VLDReadLiteral(8);
      prVp8DecParam->uGoldenProb=_VDEC_VP8_VLDReadLiteral(8);
      u4HalValue=(prVp8DecParam->uGoldenProb<<24)+(prVp8DecParam->uLastProb<<16)
                 +(prVp8DecParam->uIntraProb<<8)+prVp8DecParam->uSkipFalseProb;

      VP8_VLD_WRITE32(RW_VP8_PICPROB,u4HalValue);
      
      if(_VDEC_VP8_VLDReadLiteral(1))
      {
         INT32 i4YMode=0;
         do
         {
           prVp8DecParam->rCurFc.YModeProb[i4YMode]=(UINT8)_VDEC_VP8_VLDReadLiteral(8);
         }while(++i4YMode<(VP8_YMODES-1));
      }
      
      if(_VDEC_VP8_VLDReadLiteral(1))
      {
         INT32 i4UVMode=0;
         do
         {
            prVp8DecParam->rCurFc.UVModeProb[i4UVMode]=(UINT8)_VDEC_VP8_VLDReadLiteral(8);
         }while(++i4UVMode<(VP8_UVMODES-1));
      }
#ifndef VP8_HEADERPARSE_HWACCELATOR
        {
          UINT32 u4Index=0;
          do
          {
            const UINT8 *pMvUpdateProb=vp8_mv_update_probs[u4Index].Prob;
            UINT8 *pCurFrameProb=prVp8DecParam->rCurFc.MVC[u4Index].Prob;
            UINT8 *const pStop=pCurFrameProb+VP8_MVDEF_MVPCOUNT;
            do
            {
               if(_VDEC_VP8_VLDReadBit(*pMvUpdateProb++))
               {
                  const UINT8 x=(UINT8)_VDEC_VP8_VLDReadLiteral(7);
                  *pCurFrameProb = x ? (x<<1) : 1;
               }
            }while(++pCurFrameProb<pStop);
          }while(++u4Index<2);
        }
        _VDEC_Vp8FlushCtxProbs(&prVp8DecParam->rCurFc,VP68_INTER_FRAME);
#else   
        _VDEC_Vp8FlushCtxProbs(&prVp8DecParam->rCurFc,VP68_INTER_FRAME);
        _VDEC_VP8_HwAccMVProbUpdate();
#endif
  }
  
  prVp8DecParam->u4FlagParam=u4FlagParam;
  return TRUE;
}

static UINT32 _VDEC_Vp8DecStart(VDEC_INFO_VP8_FRM_HDR_T *pVp8DecInfo,BOOL fgTrigerDec)
{
   UINT32 u4HalValue=0;
   VDEC_PARAM_VP8DEC_T *prVp8DecParam=&pVp8DecInfo->rVp8DecParam;
   UINT32 u4FlagParam=prVp8DecParam->u4FlagParam;

   VDEC_MC_WRITE32(RW_MC_UMV_PIC_WIDTH,(pVp8DecInfo->u2Width+15)&0xfff0);
   VDEC_MC_WRITE32(RW_MC_UMV_PIC_HEIGHT,(pVp8DecInfo->u2Height+15)&0xfff0);

   if(VDEC_FLGSET(u4FlagParam,VP8PARAM_64X32_OUTPUT))
   {
      VDEC_MC_WRITE32(RW_MC_ADDRSWAP,ADDR_MODE_5351_NORMAL);
      VDEC_MC_WRITE32(RW_MC_PITCH,(((pVp8DecInfo->u2Width+63)>>6)<<2)&0x1FF);
      VDEC_MC_WRITE32(RW_MC_NBM_CTRL, ((VDEC_MC_READ32(RW_MC_NBM_CTRL)& 0xFFFFFFF8)|ADDR_MODE_5351_NORMAL));
   }
   else
   {
      VDEC_MC_WRITE32(RW_MC_ADDRSWAP,ADDR_MODE_8520);
      VDEC_MC_WRITE32(RW_MC_PITCH, ((pVp8DecInfo->u2Width + 15)>> 4) & 0x1FF);
      VDEC_MC_WRITE32(RW_MC_NBM_CTRL, ((VDEC_MC_READ32(RW_MC_NBM_CTRL)& 0xFFFFFFF8)|ADDR_MODE_8520));
   }

   //RW_MC_VP8SETTING
   u4HalValue=VDEC_MC_READ32(RW_MC_VP8SETTING);
   u4HalValue=u4HalValue&0x1;
   VDEC_REG_SET_FLAG(u4HalValue,VDEC_FLGSET(u4FlagParam,VP8PARAM_FULL_PIXEL),RW_MC_VP8FULLPEL_FLAG);
   if(!VDEC_FLGSET(u4FlagParam,VP8PARAM_BILINER_MCFILTER))
   {
      VDEC_REG_SET_FLAG(u4HalValue,1,RW_MC_BILINEAR_OR_SIXTAP_FLAG);
   }
   
   VDEC_MC_WRITE32(RW_MC_VP8SETTING,u4HalValue);

   ASSERT(((pVp8DecInfo->u4CurCAddr-pVp8DecInfo->u4CurYAddr) & 0x7F) == 0);
   ASSERT((pVp8DecInfo->u4GldYAddr & 0x1FF) == 0);
   ASSERT((pVp8DecInfo->u4AlfYAddr & 0x1FF) == 0);
   ASSERT((pVp8DecInfo->u4LstYAddr & 0x1FF) == 0);
   VDEC_MC_WRITE32(RW_MC_LUMA_SIZE,pVp8DecInfo->u4CurCAddr-pVp8DecInfo->u4CurYAddr);
   VDEC_MC_WRITE32(RW_MC_PIC1Y_ADD,pVp8DecInfo->u4LstYAddr);
   VDEC_MC_WRITE32(RW_MC_PIC2Y_ADD,pVp8DecInfo->u4GldYAddr);
   VDEC_MC_WRITE32(RW_MC_PIC3Y_ADD,pVp8DecInfo->u4AlfYAddr);

   
   VDEC_MC_WRITE32(RW_VLD_MBDRAM_SEL, VDEC_MC_READ32(RW_VLD_MBDRAM_SEL) | (1 << 16));

   if(prVp8DecParam->iLoopFilterLvl>0 
     || VDEC_FLGSET(u4FlagParam,VP8PARAM_64X32_OUTPUT)
     ||(VDEC_FLGSET(u4FlagParam,VP8PARAM_WEBP_FORCE_PPON)))
   {
      VDEC_MC_WRITE32(RW_MC_PP_ENABLE, 0x00000001);
      VDEC_MC_WRITE32(RW_MC_PP_WB_BY_POST, 0x00000001);
      VDEC_MC_WRITE32(RW_MC_PP_Y_ADDR, pVp8DecInfo->u4CurYAddr >> 9);
      VDEC_MC_WRITE32(RW_MC_PP_C_ADDR, pVp8DecInfo->u4CurCAddr >> 8);
      
	  if(VDEC_FLGSET(u4FlagParam,VP8PARAM_64X32_OUTPUT))
	  {
        VDEC_MC_WRITE32(RW_MC_PP_MB_WIDTH, ((pVp8DecInfo->u2Width+63)>>6)<<2);
	  }
	  else
	  {
        VDEC_MC_WRITE32(RW_MC_PP_MB_WIDTH, (pVp8DecInfo->u2Width+15)>>4);
	  }
      VDEC_MC_WRITE32(RW_MC_PP_X_RANGE, ((pVp8DecInfo->u2Width + 15) >> 4) - 1);
      VDEC_MC_WRITE32(RW_MC_PP_Y_RANGE, ((pVp8DecInfo->u2Height+ 15) >> 4) - 1);
   }
   else
   {
      VDEC_MC_WRITE32(RW_MC_OPBUF, 4);
      VDEC_MC_WRITE32(RW_MC_BY1,pVp8DecInfo->u4CurYAddr>>9);
      VDEC_MC_WRITE32(RW_MC_BC1,pVp8DecInfo->u4CurCAddr>>8);
      VDEC_MC_WRITE32(RW_MC_PP_ENABLE,0);
      VDEC_MC_WRITE32(RW_MC_PP_WB_BY_POST,0);
      VDEC_MC_WRITE32(RW_MC_PP_MB_WIDTH,((pVp8DecInfo->u2Width + 15) >> 4) - 1); 
   }

   if(VDEC_FLGSET(u4FlagParam,VP8PARAM_WAPPER_OFF))
   {
      // vld dram wrapper
      VDEC_VLD_TOP_WRITE32(RW_PRED_DRAM_B_ADDR,pVp8DecInfo->u4VLDWrapper);
      VDEC_VLD_TOP_WRITE32(RW_SEGID_DRAM_ADDR,pVp8DecInfo->u4SegIdWrapper);
      u4HalValue=0x80000000;
      VDEC_REG_SET_FLAG(u4HalValue,1,RW_PRED_SRAM_CFG_SRAM_BYPASS_FLAG);
	  VDEC_VLD_TOP_WRITE32(RW_PRED_SRAM_CFG,u4HalValue);

      u4HalValue=0;
      VDEC_REG_SET_VALUE(u4HalValue,3,RW_NUM_ROW_SEGID_DATA_M1_S,RW_NUM_ROW_SEGID_DATA_M1_E);
      VDEC_REG_SET_VALUE(u4HalValue,1,RW_DRAM_BURST_MODE_S,RW_DRAM_BURST_MODE_E);
      ASSERT(u4HalValue==0X13);
      VDEC_VLD_TOP_WRITE32(RW_PRED_MODE,u4HalValue);
      
	  // PP dram wrapper
	  //VDEC_MC_WRITE32(RW_MC_DDR3_EN, VDEC_MC_READ32(RW_MC_DDR3_EN)|0x1);
      VDEC_MC_WRITE32(RW_MC_DDR3_EN,1);
      VDEC_MC_WRITE32(RW_MC_BYPASS_DBLK_WRAP,1);
	  VDEC_MC_WRITE32(RW_MC_PP_DBLK_Y_ADDR,pVp8DecInfo->u4PPWrapperY>>4);
	  VDEC_MC_WRITE32(RW_MC_PP_DBLK_C_ADDR,pVp8DecInfo->u4PPWrapperC>>4);
   }
   else
   {
	  VDEC_MC_WRITE32(RW_MC_BYPASS_DBLK_WRAP,0);
   }
   
   VDEC_MC_WRITE32(0x5E4, (VDEC_MC_READ32(0x5E4) |(0x1 <<12)) );
   VDEC_MC_WRITE32(0x660, (VDEC_MC_READ32(0x660) |(0x80000000)));

    //PP Setting
    if(prVp8DecParam->iLoopFilterLvl>0)
    { 
      UINT32 u4Index=0;
      INT32 i4Level=0;
      
      if(prVp8DecParam->eLoopFilterType==VP8_LF_TYPE_NORMAL)
      {
        VDEC_MC_WRITE32(RW_MC_PP_DBLK_MODE,3);
      }
      else
      {
        VDEC_MC_WRITE32(RW_MC_PP_DBLK_MODE,2);
      }
      
      u4HalValue=1; //VP8 ENABLE
      VDEC_REG_SET_FLAG(u4HalValue,prVp8DecParam->eLoopFilterType,RW_PP_FLTTYPE_FLAG);
      VDEC_REG_SET_FLAG(u4HalValue,VDEC_FLGSET(u4FlagParam,VP8PARAM_MODEREF_LFDELTA_ENABLE),RW_PP_MODEREF_DELTA_ENABLE_FLAG);
      VDEC_REG_SET_FLAG(u4HalValue,pVp8DecInfo->uFrameType,RW_PP_FRAMETYPE_FLAG);
      VDEC_REG_SET_VALUE(u4HalValue,prVp8DecParam->iSharpLvl,RW_PP_SHARPLVL_S,RW_PP_SHARPLVL_E);
      VDEC_PP_WRITE32(RW_PP_VP8PARA,u4HalValue);
      
      u4HalValue=0;
      for(u4Index=0;u4Index<MAX_MB_SEGMENTS;u4Index++)
      {
         if(VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_ENABLE))
         {
            if(VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_ABSDATA))
            {
               i4Level=prVp8DecParam->SegmentFeatureData[VP8_MBLVL_ALT_LF][u4Index];
            }
            else
            {
               i4Level=prVp8DecParam->iLoopFilterLvl
                        +prVp8DecParam->SegmentFeatureData[VP8_MBLVL_ALT_LF][u4Index];
               i4Level=i4Level>0 ? (i4Level<=MAX_FILTER_LVL ? i4Level : MAX_FILTER_LVL) : 0;
            }
         }
         else
         {
            i4Level=prVp8DecParam->iLoopFilterLvl;
         }
         u4HalValue|=i4Level<<(u4Index*8);
      }

      VDEC_PP_WRITE32(RW_PP_VP8FILTER_LEVEL,u4HalValue);
      u4HalValue=0;
      for(u4Index=0;u4Index<MAX_REF_LF_DELTAS;u4Index++)
      {
        u4HalValue|=(((UINT8)prVp8DecParam->RefLFDeltas[u4Index])<<(u4Index*8));
      }
      VDEC_PP_WRITE32(RW_PP_VP8REFDELTA,u4HalValue);
      u4HalValue=0;
      for(u4Index=0;u4Index<MAX_MODE_LF_DELTAS;u4Index++)
      {
        u4HalValue|=(((UINT8)prVp8DecParam->ModeLFDeltas[u4Index])<<(u4Index*8));
      }
      VDEC_PP_WRITE32(RW_PP_VP8MODEDELTA,u4HalValue);
   }
   
	if(VDEC_FLGSET(u4FlagParam,VP8PARAM_DDR3_CONFIG))
	{
      //Turn on DDR3 mode
      VDEC_MC_WRITE32(RW_MC_DDR_CTRL0,(VDEC_MC_READ32(RW_MC_DDR_CTRL0)&0xFFFFFFFE)|0x1);
      VDEC_MC_WRITE32(RW_MC_DDR_CTRL1,(VDEC_MC_READ32(RW_MC_DDR_CTRL1)&0xFFFFFFFE)|0x1);
      VDEC_MC_WRITE32((406<<2), VDEC_MC_READ32(406<<2)&0xFFFFFFEF);
      //Turn-on DDR3, Set 0x834[0] = 0
      VDEC_MC_WRITE32(RW_MC_DDR3_EN, VDEC_MC_READ32(RW_MC_DDR3_EN)  & 0xFFFFFFFE);
      VDEC_MC_WRITE32(RW_MC_PP_ENABLE, (VDEC_MC_READ32(RW_MC_PP_ENABLE)  | 0x1));
	}

    if(VDEC_FLGSET(u4FlagParam,VP8PARAM_WEBP_MODE))
    {
       UINT32 u4Index=0;
      
       // need width greate than 16, is ic problem.
       if(prVp8DecParam->iLoopFilterLvl>0 && prVp8DecParam->eLoopFilterType==VP8_LF_TYPE_SIMPLE)
       {
           VDEC_PP_WRITE32(RW_PP_REG_17,1);
           VDEC_MC_WRITE32(RW_MC_PP_DBLK_MODE,3);
       }
       
       if(pVp8DecInfo->u2Width>16/* && pVp8DecInfo->u2Height>=16*/)
       {
          u4HalValue=VDEC_VLD_TOP_READ32(RW_WEBP_CTRL);
          VDEC_REG_SET_FLAG(u4HalValue,1,RW_BUFCTRL_ON_FLAG);
          if(VDEC_FLGSET(u4FlagParam,VP8PARAM_WEBP_PICMODE))
          {
             VDEC_REG_SET_FLAG(u4HalValue,1,RW_RESIZE_INTER_ON_FLAG);
          }
          
          VDEC_VLD_TOP_WRITE32(RW_WEBP_CTRL,u4HalValue);
          VDEC_PP_WRITE32(RW_PP_BRC_OFFSET,((pVp8DecInfo->u2Width + 15) >> 4)*32);
       }

       if(VDEC_FLGSET(u4FlagParam,VP8PARAM_RINGBUF_CTRL))
       {
          VP8_VLD_WRITE32(RW_VP8_PART_CHG_PAUSE,VP8_VLD_READ32(RW_VP8_PART_CHG_PAUSE)|0x1);
          for(u4Index=0;u4Index<pVp8DecInfo->rVp8DecParam.uDataPartitionNum;u4Index++)
          {
			  VP8_VLD_WRITE32(RW_VP8_COEFR0+u4Index*4,0);
          }
       }
       else if(VDEC_FLGSET(u4FlagParam,VP8PARAM_WEBP_RESET_BUF))
       {
           VP8_VLD_WRITE32(RW_VP8_COEFR0,pVp8DecInfo->u4VldStartPos);
          for(u4Index=1;u4Index<pVp8DecInfo->rVp8DecParam.uDataPartitionNum;u4Index++)
          {
			  VP8_VLD_WRITE32(RW_VP8_COEFR0+u4Index*4,VP8_VLD_READ32(RW_VP8_COEFR0+((u4Index-1)*4))+pVp8DecInfo->u4rPartitionSize[u4Index]);
          }
       }
    }
	
#if defined(VP8_ENABLE_CRC) || defined(WEBP_ENABLE_CRC)
    u4HalValue=VDEC_MISC_READ32(RW_CRC_CTRL);
    ASSERT(u4HalValue&0x1);
    u4HalValue=((VDEC_MC_READ32(RW_MC_PP_ENABLE)&0x1)<<1)+1;
    VDEC_MISC_WRITE32(RW_CRC_CTRL,u4HalValue);
#endif

#ifdef VP8_ENABLE_ERRDETECT
    /*
      u4HalValue=0;
      VDEC_REG_SET_FLAG(u4HalValue,1,RO_VP8_ERR_FSTPARTERR);
      VDEC_REG_SET_FLAG(u4HalValue,1,RO_VP8_ERR_SECPARTERR);
      VDEC_REG_SET_FLAG(u4HalValue,1,RO_VP8_ERR_MC_TIMEOUT);
      VDEC_REG_SET_FLAG(u4HalValue,1,RO_VP8_ERR_VDEC_TIMEOUT);
    */
    u4HalValue=VP8_ERROR_DETECT_TYPE;
    VDEC_VLD_TOP_WRITE32(RW_ERR_TYPE_ENABLE,u4HalValue);
    VDEC_VLD_TOP_WRITE32(RW_MC_BUSY_THRESHOLD,(UINT32)-1);
    VDEC_VLD_TOP_WRITE32(RW_MDEC_TIMEOUT_THRESHOLD,(UINT32)-1);
    VDEC_VLD_TOP_WRITE32(RW_MDEC_TIMEOUT_INT_SWITCH,1); // enable timeout

	u4HalValue=0;
	if(VDEC_FLGSET(u4FlagParam,VP8PARAM_WEBP_MODE))
	{
	    u4HalValue=0x80000000;
		//VDEC_REG_SET_FLAG(u4HalValue,1,RW_RETURN_IF_ERROR_FLAG);
	}
	else
	{
#ifdef VP8_ENABLE_ERRCONCEALMENT
		VDEC_REG_SET_VALUE(u4HalValue,2,RW_ERROR_SWITCH_S,RW_ERROR_SWITCH_E);
		VDEC_REG_SET_VALUE(u4HalValue,1,RW_SLICE_RECONCEL_MODE_S,RW_SLICE_RECONCEL_MODE_E);
		VDEC_REG_SET_FLAG(u4HalValue,1,RW_SLICE_RECONCEL_FLAG);
#else
	    u4HalValue=0x80000000;
		//VDEC_REG_SET_FLAG(u4HalValue,1,RW_RETURN_IF_ERROR_FLAG);
#endif
	}
	
    VDEC_VLD_TOP_WRITE32(RW_ERR_CONCEALMENT,u4HalValue);
#endif

   //VDEC_VLD_TOP_WRITE32(0x7C, (VDEC_VLD_TOP_READ32(0x7C) & (~0x01l)));
   VDEC_MC_WRITE32(0x9F8, VDEC_MC_READ32(0x9F8)|0x01);
   if(fgTrigerDec)
   {
#ifdef VDEC_VP8_DUMP_REG
     {
       UINT32 u4Tmp;
       u4Tmp = VP8_VLD_READ32(RO_VP8_VBSR1);
       Printf("*** VLD1 Input Window 0x%x\n", u4Tmp);
       
       u4Tmp = VP8_VLD_READ32(RO_VP8_VBSR2);
       Printf("*** VLD2 Input Window 0x%x\n", u4Tmp);
     }
#endif
     
#ifdef VP8_TIME_PROFILE
     {
        VDEC_VP8_INFO_T *prVdecInfo=VDEC_Vp8GetLockedInstance();
        HAL_GetTime(&prVdecInfo->rTimeProfile.rTimeE);
        HAL_GetDeltaTime(&prVdecInfo->rTimeProfile.rTimeDif,\
	    &prVdecInfo->rTimeProfile.rTimeS, \
	    &prVdecInfo->rTimeProfile.rTimeE);
        prVdecInfo->rTimeProfile.u4SwMicro=prVdecInfo->rTimeProfile.rTimeDif.u4Micros;
        HAL_GetTime(&prVdecInfo->rTimeProfile.rTimeS);
		prVdecInfo->rTimeProfile.u4DelayClock=(BSP_GetDomainClock(SRC_MEM_CLK) >> 1);
		//prVdecInfo->rTimeProfile.u4CurDramCycle= SRM_DramMonitor(0,(UINT32)SRM_DRV_VDEC_MC, prVdecInfo->rTimeProfile.u4DelayClock, 0);
     }
#endif
      prVp8DecParam->u4LastParam=u4FlagParam;
      //Trigger decoder
      VP8_VLD_WRITE32(RW_VP8_HDR,1);
      VP8_VLD_WRITE32(RW_VP8_HDR,0);
   }
   return 0;
}

static BOOL _VDEC_Vp8IsDecFinish(VOID)
{
   return (BOOL )((VP8_VLD_READ32(RO_VP8_PFR)&1)||(VP8_VLD_READ32(RO_VP8_VOKR)&1));
}

static UINT32 _VDEC_Vp8GetErrInfo(VOID)
{
   return VP8_VLD_READ32(RO_VP8_ERTR);
}



static VOID _VDEC_Vp8DecFinish(VDEC_INFO_VP8_FRM_HDR_T *pVp8DecInfo)
{
   VDEC_PARAM_VP8DEC_T *prVp8DecParam=&pVp8DecInfo->rVp8DecParam;
   if(!VDEC_FLGSET(prVp8DecParam->u4FlagParam,VP8PARAM_REFRESH_PROBS))
   {
      x_memcpy(&prVp8DecParam->rCurFc,&prVp8DecParam->rLastFC,sizeof(VDEC_VP8FRAME_CONTEXT_T));
   }
   else if(VDEC_FLGSET(prVp8DecParam->u4FlagParam,VP8PARAM_REFRESH_PROBS))
   {
      _VDEC_Vp8LoadCoefProbs(&prVp8DecParam->rCurFc);
      if(pVp8DecInfo->uFrameType==VP68_INTER_FRAME)
      {
         _VDEC_Vp8LoadCtxProbs(&prVp8DecParam->rCurFc);
      }
   }
}

static UINT32 _VDEC_Vp8DecInfo(VOID)
{
   UINT32 u4DecInfo=0;
   u4DecInfo=VP8_VLD_READ32(RO_VP8_PICFSM)&((1<<19)-1);
   LOG(5,"PicState(0x%x=0x%x)\n",RO_VP8_PICFSM,u4DecInfo);
   u4DecInfo=VP8_VLD_READ32(RO_VP8_MBFSM)&((1<<23)-1);
   LOG(5,"MBState(0x%x=0x%x)\n",RO_VP8_MBFSM,u4DecInfo);
   u4DecInfo=VP8_VLD_READ32(RO_VP8_COEFFSM)&((1<<3)-1);
   LOG(5,"CoefState(0x%x=0x%x)\n",RO_VP8_COEFFSM,u4DecInfo);
   u4DecInfo=VP8_VLD_READ32(RO_VP8_ERTR);
   LOG(5,"ErrType(0x%x=0x%x)\n",RO_VP8_ERTR,u4DecInfo);
   u4DecInfo=VP8_VLD_READ32(RO_VP8_ERRXY);
   LOG(5,"ErrXY(0x%x=0x%x)\n",RO_VP8_ERRXY,u4DecInfo);
   u4DecInfo=VP8_VLD_READ32(RO_VP8_VOKR)&0X1;
   LOG(5,"VOKR(0x%x=0x%x)\n",RO_VP8_VOKR,u4DecInfo);
   u4DecInfo=VP8_VLD_READ32(RO_VP8_PFR)&0X1;
   LOG(5,"PFR(0x%x=0x%x)\n",RO_VP8_PFR,u4DecInfo);
   u4DecInfo=VP8_VLD_READ32(RO_VP8_STATUS);
   LOG(5,"STATUS(0x%x=0x%x)\n",RO_VP8_STATUS,u4DecInfo);
   u4DecInfo=VP8_VLD_READ32(RO_VP8_XY);
   LOG(5,"CURXY(0x%x=0x%x)\n",RO_VP8_XY,u4DecInfo);
   u4DecInfo=VP8_VLD_READ32(RO_VP8_HWA);
   LOG(5,"RUN(0x%x=0x%x)\n",RO_VP8_HWA,u4DecInfo);
   UNUSED(u4DecInfo);
   return 0;
}
static BOOL _VDEC_Vp8DecReset(VDEC_INFO_VP8_FRM_HDR_T *pVp8DecInfo,BOOL fgInit)
{
   _VDEC_VP8_VLDReset(VDEC_VLD1);
   //_VDEC_Vp8PrintRegValue(pVp8DecInfo);
   if(!fgInit)
   {
       UINT32 u4HalValue;
       
       VDEC_VLD_WRITE32(RW_VLD_RDY_SWTICH, READY_TO_RISC_1);
       VDEC_VLD_WRITE32(RW_VLD_VSTART, pVp8DecInfo->u4FifoStart >> 6);
       VDEC_VLD_WRITE32(RW_VLD_VEND, pVp8DecInfo->u4FifoEnd>> 6);

       while(!_VDEC_VP8_InitBarrelShifter(VDEC_VLD1,pVp8DecInfo->u4VldStartPos,pVp8DecInfo->u4WritePos))
       {
           LOG(5,"VP8 Init VLD1 BarrelShifter fail\n");
           _VDEC_VP8_VLDReset(VDEC_VLD1);
       }

       u4HalValue=VDEC_GET_FLAGVAL(1,RW_VP8_BCRT1_FLAG);
       VP8_VLD_WRITE32(RW_VP8_BCRT,u4HalValue);
       VP8_VLD_WRITE32(RW_VP8_BCRT,0);
   }

   _VDEC_VP8_VLDReset(VDEC_VLD2);
   if(!fgInit)
   {
       VDEC_VLD2_WRITE32(RW_VLD_RDY_SWTICH, READY_TO_RISC_1);
       VDEC_VLD2_WRITE32(RW_VLD_VSTART, pVp8DecInfo->u4FifoStart >> 6);
       VDEC_VLD2_WRITE32(RW_VLD_VEND, pVp8DecInfo->u4FifoEnd>> 6);       
   }
   
#if defined(VP8_ENABLE_CRC) || defined(WEBP_ENABLE_CRC)
   if(fgInit||VDEC_FLGSET(pVp8DecInfo->rVp8DecParam.u4FlagParam,VP8PARAM_WEBP_MODE))
   {
      VDEC_MISC_WRITE32(RW_CRC_CTRL,1);
   }
#endif
   return TRUE;
}

VOID _Vdec_Vp8DecProgress(UINT32 *pu4XMb,UINT32 *pu4YMb)
{
    if(pu4XMb)
    {
      *pu4XMb=VDEC_MC_READ32(RO_MC_MBX);
    }

    if(pu4YMb)
    {
      *pu4YMb=VDEC_MC_READ32(RO_MC_MBY);
    }
}

VOID _Vdec_VP8IsrInit(VDEC_VP8_INFO_T* prDecInfo)
{
    #ifndef VDEC_COMMON_ISR
    x_os_isr_fct pfnOldIsr;
    #endif
    prCurDecoder=prDecInfo;
#ifdef SUPPORT_DEC_ISR   
    //LOG(3,"Init vp8 webm isr\n");
    if(_hVP8VdecFinSema == (HANDLE_T)NULL)
    {
        VERIFY(x_sema_create(&_hVP8VdecFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    }
    
    // reg ISR      
    if(prDecInfo->eDecType==VP8_DEC_TYPE_WEBM)
    {
        #ifdef VDEC_COMMON_ISR
        _VDEC_RegIsr(AVLD_VECTOR, _VDEC_Vp8HwIsr);
        #else
        if (x_reg_isr(AVLD_VECTOR, _VDEC_Vp8HwIsr, &pfnOldIsr) != OSR_OK)
        {
            ASSERT(0);
        }
        #endif
    }
	else if(prDecInfo->eDecType==VP8_DEC_TYPE_WEBP)
	{
        #ifdef VDEC_COMMON_ISR
        _VDEC_RegIsr(AVLD_VECTOR, _VDEC_Vp8HwWebPIsr);
        #else
        if (x_reg_isr(AVLD_VECTOR, _VDEC_Vp8HwWebPIsr, &pfnOldIsr) != OSR_OK)
        {
            ASSERT(0);
        }
        #endif
	}
	else
	{
       ASSERT(0);
	}

#else 
    // unreg ISR  
    #ifdef VDEC_COMMON_ISR
    _VDEC_RegIsr(AVLD_VECTOR, NULL);
    #else    
    if (x_reg_isr(AVLD_VECTOR, NULL, &pfnOldIsr) != OSR_OK)
    {
        ASSERT(0);
    }
    #endif

    UNUSED(prDecInfo);
#endif
}

BOOL _Vdec_VP8HwInit(VDEC_VP8_INFO_T *prVdecInfo)
{
    _VDEC_Vp8DecInit(&prVdecInfo->rVp8FrmHdr);
	_Vdec_VP8HwResetWapper();
    return _VDEC_Vp8DecReset(&prVdecInfo->rVp8FrmHdr,TRUE);
}

BOOL _Vdec_VP8HwParseHeader(VDEC_VP8_INFO_T *prVdecInfo)
{
    VDEC_INFO_VP8_FRM_HDR_T *prFrmHdr = &prVdecInfo->rVp8FrmHdr;
    //VDEC_PARAM_VP8DEC_T *prVp8DecParam=&prFrmHdr->rVp8DecParam;
    //UINT32 u4FlagParam=prVp8DecParam->u4FlagParam;
#ifdef VP8_TIME_PROFILE
    prVdecInfo->rTimeProfile.u4CurDecCycle=0;
	prVdecInfo->rTimeProfile.u4CurDramCycle=0;
	prVdecInfo->rTimeProfile.u4DecMs=0;
	prVdecInfo->rTimeProfile.u4SwMicro=0;
    HAL_GetTime(&prVdecInfo->rTimeProfile.rTimeS);
#endif
    prFrmHdr->u4FifoStart=PHYSICAL(prVdecInfo->rPesInfo.u4FifoStart);
    prFrmHdr->u4FifoEnd= PHYSICAL(prVdecInfo->rPesInfo.u4FifoEnd);
    prFrmHdr->u4VldStartPos=PHYSICAL(prVdecInfo->rPesInfo.u4VldReadPtr);

#ifdef CC_VP8_EMULATION
    prFrmHdr->u4FrameSize = prVdecInfo->rPesInfo.u4DmxFrameType;
#else
    if(prVdecInfo->rDecParam.u4WPtr+512<prVdecInfo->rPesInfo.u4FifoEnd)
    {
       prVdecInfo->rDecParam.u4WPtr+=512;
    }
    else
    {
       prVdecInfo->rDecParam.u4WPtr=prVdecInfo->rPesInfo.u4FifoStart+512;
    }
    prFrmHdr->u4FrameSize= prVdecInfo->rDecParam.u4WPtr > prVdecInfo->rPesInfo.u4VldReadPtr ? 
        (prVdecInfo->rDecParam.u4WPtr-prVdecInfo->rPesInfo.u4VldReadPtr) :
        ((prVdecInfo->rDecParam.u4WPtr-prVdecInfo->rPesInfo.u4FifoStart)
            +(prVdecInfo->rPesInfo.u4FifoEnd-prVdecInfo->rPesInfo.u4VldReadPtr));
    
#endif
    
#ifdef CC_VP8_EMULATION
    prVdecInfo->rDecParam.u4WPtr=prFrmHdr->u4FifoEnd;
    //prVdecInfo->rDecParam.u4WPtr=(prVdecInfo->rDecParam.u4WPtr+WPTR_OFFSET)&(~(WPTR_OFFSET-1));
#endif
    prFrmHdr->u4WritePos=PHYSICAL(prVdecInfo->rDecParam.u4WPtr);
#ifdef VP8_TEST_BUFRING
    _VDEC_Vp8TestBufRing(prFrmHdr);
#endif

    prFrmHdr->rVp8DecParam.u4FlagParam = 0;
#ifdef VDEC_VP8HW_DEBUG
     if(prVdecInfo->rDecParam.u4LogFrameIndex==prVdecInfo->rDecParam.u4FrameCounter)
     {
       _VDEC_VP8_IO_Log(TRUE);
     }
     else
     {
       _VDEC_VP8_IO_Log(FALSE);
     }
#endif

#ifdef USE_VP68_SW_VLD
    _VDEC_VP68_InitSwBarrelShifter(VDEC_VLD1,prFrmHdr->u4VldStartPos,prFrmHdr->u4FifoStart,prFrmHdr->u4FifoEnd);
#endif


#ifdef VP8_EMUCOVERAGE_STATISTIC
    if(prVdecInfo->rDecParam.u4FrameCounter==0)
    {
        _Vdec_VP8UpdateEmuItemInfo(prFrmHdr,1);
    }
#endif

    _VDEC_Vp8DecReset(prFrmHdr,FALSE);
    if(!_VDEC_Vp8DecHeader(prFrmHdr))
    {
       return FALSE;
    }

#ifdef VP8_EMUCOVERAGE_STATISTIC
    _Vdec_VP8UpdateEmuItemInfo(prFrmHdr,0);
#endif
    //_VDEC_Vp8PrintRegValue(prFrmHdr);
    prVdecInfo->rDecParam.u2NewHeight=prFrmHdr->u2Height;
    prVdecInfo->rDecParam.u2NewWidth=prFrmHdr->u2Width;
    prVdecInfo->rDecParam.fgKeyFrame=(prFrmHdr->uFrameType==VP68_KEY_FRAME) ? TRUE : FALSE;
    return TRUE;
}

BOOL _Vdec_VP8HwStartDecode(VDEC_VP8_INFO_T *prVdecInfo)
{
  VDEC_INFO_VP8_FRM_HDR_T *prFrmHdr=&prVdecInfo->rVp8FrmHdr;
  VP8_DEC_PARAM_T *prDecParam=&prVdecInfo->rDecParam;
  UINT32 u4BufYAdd=0,u4BufCAdd=0;

  if(prDecParam->ucLastFbId!=FBM_FB_ID_UNKNOWN)
  {
	  FBM_GetFrameBufferAddr(prDecParam->ucFbgId,prDecParam->ucLastFbId,&u4BufYAdd,&u4BufCAdd);
	  prFrmHdr->u4LstYAddr=u4BufYAdd;
  }
  else
  {
	  prFrmHdr->u4LstYAddr=0;
  }

  if(prDecParam->ucGoldenFbId!=FBM_FB_ID_UNKNOWN)
  {
	  FBM_GetFrameBufferAddr(prDecParam->ucFbgId,prDecParam->ucGoldenFbId,&u4BufYAdd,&u4BufCAdd);
	  prFrmHdr->u4GldYAddr=u4BufYAdd;
  }
  else
  {
      prFrmHdr->u4GldYAddr=0;
  }

  if(prDecParam->ucAltFbId!=FBM_FB_ID_UNKNOWN)
  {
	  FBM_GetFrameBufferAddr(prDecParam->ucFbgId,prDecParam->ucAltFbId,&u4BufYAdd,&u4BufCAdd);
	  prFrmHdr->u4AlfYAddr=u4BufYAdd;
  }
  else
  {
      prFrmHdr->u4AlfYAddr=0;
  }


  FBM_GetFrameBufferAddr(prDecParam->ucFbgId,prDecParam->ucCurFbId,&u4BufYAdd,&u4BufCAdd);
  prFrmHdr->u4CurYAddr=u4BufYAdd;
  prFrmHdr->u4CurCAddr=u4BufCAdd;

#if defined(VP8_DISABLE_WRAPPER) && defined(CC_VP8_EMULATION)
  {
    UINT32 u4WapperBuffer=prDecParam->u4WorkBuf;
    VDEC_SETFLG(prFrmHdr->rVp8DecParam.u4FlagParam,VP8PARAM_WAPPER_OFF);
    VDEC_VALUE_ALIGN(u4WapperBuffer,512);
    prFrmHdr->u4VLDWrapper=u4WapperBuffer;
    u4WapperBuffer+=64*1024;
    //prDecParam->u4WorkBuf+=((prFrmHdr->u2Width+15)/16)*64;
    prFrmHdr->u4SegIdWrapper=u4WapperBuffer;
    //prDecParam->u4WorkBuf+=64*((prFrmHdr->u2Height+15)/16);
    u4WapperBuffer+=256*1024;
    VDEC_VALUE_ALIGN(u4WapperBuffer,512);
    prFrmHdr->u4PPWrapperY=u4WapperBuffer;
    //prDecParam->u4WorkBuf+=((prFrmHdr->u2Width+15)/16)*64;
    u4WapperBuffer+=64*1024;
    VDEC_VALUE_ALIGN(u4WapperBuffer,512);
    prFrmHdr->u4PPWrapperC=u4WapperBuffer;
    u4WapperBuffer+=64*1024;
 }

#endif

#ifdef VDEC_DDR3_SUPPORT
  if(!VDEC_FLGSET(prFrmHdr->rVp8DecParam.u4FlagParam, VP8PARAM_WAPPER_OFF))
  {
    VDEC_SETFLG(prFrmHdr->rVp8DecParam.u4FlagParam,VP8PARAM_DDR3_CONFIG);
  }
#endif
#ifdef VP8_64X32_BLK_MODE
  VDEC_SETFLG(prFrmHdr->rVp8DecParam.u4FlagParam,VP8PARAM_64X32_OUTPUT);
#endif

  _VDEC_Vp8DecStart(prFrmHdr,TRUE);
  return TRUE;
}

VOID _Vdec_VP8HwWebPReset(WEBP_BUF_INFO *prBufInfo,BOOL fgDecStarted)
{
   _VDEC_VP8_VLDReset(VDEC_VLD2);
   
   VDEC_VLD2_WRITE32(RW_VLD_RDY_SWTICH, READY_TO_RISC_1);
   VDEC_VLD2_WRITE32(RW_VLD_VSTART, prBufInfo->u4Start>> 6);
   VDEC_VLD2_WRITE32(RW_VLD_VEND, prBufInfo->u4End>> 6);
   ASSERT((prBufInfo->u4RealWritePos&WPTR_ALIGN_MARK)==0);
   while(!_VDEC_VP8_InitBarrelShifter(VDEC_VLD2,prBufInfo->u4ReadPos,prBufInfo->u4RealWritePos))
   {
      LOG(5,"VP8 WebP Init VLD2 BarrelShifter fail\n");
      _VDEC_VP8_VLDReset(VDEC_VLD2);
   }
   
   _VDEC_VP8_VLDShiftBit(VDEC_VLD2,prBufInfo->u4UsedBitCount&0x7);
   if(fgDecStarted)
   {
	   VP8_VLD_WRITE32(RW_VP8_DEC_RESUME,0x1);
   }
   else
   {
      UINT32 u4HalValue=VDEC_GET_FLAGVAL(1,RW_VP8_BCRT2_FLAG);
      VP8_VLD_WRITE32(RW_VP8_BCRT,u4HalValue);
      VP8_VLD_WRITE32(RW_VP8_BCRT,0);
       _VDEC_VP8_InitBoolDecoder(VDEC_VLD2);
   }
   return;
}

VOID _Vdec_VP8HwWebPMcuRowTrigger(BOOL fgDecStarted)
{
#if 0
         Printf("VP8 VLD[%d:%x]=0x%x\n",RO_VP8_VBSR1/4,RO_VP8_VBSR1,VP8_VLD_READ32(RO_VP8_VBSR1));
         Printf("VP8 VLD[%d:%x]=0x%x\n",RO_VP8_VBSR2/4,RO_VP8_VBSR2,VP8_VLD_READ32(RO_VP8_VBSR2));
         Printf("VP8 VLD[%d:%x]=0x%x\n",RW_VP8_CTRL/4,RW_VP8_CTRL,VP8_VLD_READ32(RW_VP8_CTRL));
         Printf("VP8 VLD[%d:%x]=0x%x\n",RO_VP8_ERTR/4,RO_VP8_ERTR,VP8_VLD_READ32(RO_VP8_ERTR));
         Printf("VP8 VLD[%d:%x]=0x%x\n",RO_VP8_VOKR/4,RO_VP8_VOKR,VP8_VLD_READ32(RO_VP8_VOKR));
         Printf("VP8 VLD[%d:%x]=0x%x\n",RO_VP8_PFR/4,RO_VP8_PFR,VP8_VLD_READ32(RO_VP8_PFR));
         Printf("VP8 VLD[%d:%x]=0x%x\n",RO_VP8_XY/4,RO_VP8_XY,VP8_VLD_READ32(RO_VP8_XY));
         Printf("VP8 VLD[%d:%x]=0x%x\n",RO_VP8_PICFSM/4,RO_VP8_PICFSM,VP8_VLD_READ32(RO_VP8_PICFSM));
         Printf("VLD TOP[%d:%x]=0x%x\n",45,45*4,VDEC_VLD_TOP_READ32(45*4));
#endif
          

   if(fgDecStarted)
   {
	   VDEC_VLD_TOP_WRITE32(RW_MB_DECSTART,1);
	   //VDEC_VLD_TOP_WRITE32(RW_MB_DECSTART,0);
   }
   else
   {
#ifdef VP8_TIME_PROFILE
	  VDEC_VP8_INFO_T *prVdecInfo=VDEC_Vp8GetLockedInstance();
	  HAL_GetTime(&prVdecInfo->rTimeProfile.rTimeE);
	  HAL_GetDeltaTime(&prVdecInfo->rTimeProfile.rTimeDif,\
			  &prVdecInfo->rTimeProfile.rTimeS, \
			   &prVdecInfo->rTimeProfile.rTimeE);
	  prVdecInfo->rTimeProfile.u4SwMicro=prVdecInfo->rTimeProfile.rTimeDif.u4Micros;
	  HAL_GetTime(&prVdecInfo->rTimeProfile.rTimeS);
	  prVdecInfo->rTimeProfile.u4DelayClock=(BSP_GetDomainClock(SRC_MEM_CLK) >> 1);
	  //prVdecInfo->rTimeProfile.u4CurDramCycle= SRM_DramMonitor(0,(UINT32)SRM_DRV_VDEC_MC, prVdecInfo->rTimeProfile.u4DelayClock, 0);
#endif
	   VP8_VLD_WRITE32(RW_VP8_HDR,1);
	   VP8_VLD_WRITE32(RW_VP8_HDR,0);
   }
}

BOOL _Vdec_VP8HwWebPParseHeader(VDEC_VP8_INFO_T *prVdecInfo)
{
    VDEC_INFO_VP8_FRM_HDR_T *prFrmHdr = &prVdecInfo->rVp8FrmHdr;
    WEBP_DEC_INFO_T *prWebPInfo=&prVdecInfo->rWebPInfo;
    _VDEC_Vp8DecInit(prFrmHdr);
	prFrmHdr->u4FifoStart=prWebPInfo->rSrcBufInfo[0].u4Start;
	prFrmHdr->u4FifoEnd=prWebPInfo->rSrcBufInfo[0].u4End;
	prFrmHdr->u4VldStartPos=prFrmHdr->u4FifoStart;
	prVdecInfo->rDecParam.u4WPtr=prFrmHdr->u4FifoEnd;
    prFrmHdr->u4WritePos=PHYSICAL(prVdecInfo->rDecParam.u4WPtr);
    prFrmHdr->u4FrameSize = prWebPInfo->prDecInParam->u4FileSize-prWebPInfo->prDecInParam->u4FileOffset;
    prFrmHdr->rVp8DecParam.u4FlagParam = 0;
    prFrmHdr->rVp8DecParam.u4LastParam = 0;
    VDEC_SETFLG(prFrmHdr->rVp8DecParam.u4FlagParam,VP8PARAM_WEBP_MODE);
    if(!prWebPInfo->fgBufEnough)
    {
       VDEC_SETFLG(prFrmHdr->rVp8DecParam.u4FlagParam,VP8PARAM_WEBP_RESET_BUF);
    }

     _VDEC_Vp8DecReset(prFrmHdr,FALSE);
    if(!_VDEC_Vp8DecHeader(prFrmHdr))
    {
       return FALSE;
    }
    
    prVdecInfo->rDecParam.u2Height=prFrmHdr->u2Height;
    prVdecInfo->rDecParam.u2Width=prFrmHdr->u2Width;
	prWebPInfo->u4HeaderLen=VP8_KEYHDR_PREPARSE_LEN+prFrmHdr->u4rPartitionSize[0]+(prFrmHdr->rVp8DecParam.uDataPartitionNum-1)*3;
    prWebPInfo->rSrcBufInfo[0].u4UsedBitCount=_VDEC_Vp8GetBitCnt(VDEC_VLD1);
    return TRUE;
}

BOOL _Vdec_VP8HwWebPDecode(VDEC_VP8_INFO_T *prVdecInfo)
{
  VDEC_INFO_VP8_FRM_HDR_T *prFrmHdr=&prVdecInfo->rVp8FrmHdr;
  WEBP_DEC_INFO_T *prWebPInfo=&prVdecInfo->rWebPInfo;
  prFrmHdr->u4LstYAddr=0;
  prFrmHdr->u4GldYAddr=0;
  prFrmHdr->u4AlfYAddr=0;
  prFrmHdr->u4CurYAddr=prWebPInfo->rBufLayOut.u4ColorBufY;
  prFrmHdr->u4CurCAddr=prWebPInfo->rBufLayOut.u4ColorBufC;
   if(prWebPInfo->fgWrapperOff)
  {
	 VDEC_SETFLG(prFrmHdr->rVp8DecParam.u4FlagParam,VP8PARAM_WAPPER_OFF);
	 prFrmHdr->u4VLDWrapper=prWebPInfo->rBufLayOut.u4VLDWrapper;
	 prFrmHdr->u4SegIdWrapper=prWebPInfo->rBufLayOut.u4SegIdWapper;
	 prFrmHdr->u4PPWrapperY=prWebPInfo->rBufLayOut.u4PPWrapperY;
	 prFrmHdr->u4PPWrapperC=prWebPInfo->rBufLayOut.u4PPWrapperC;
	 //VDEC_SETFLG(prFrmHdr->rVp8DecParam.u4FlagParam,VP8PARAM_WEBP_FORCE_PPON);
  }
   VDEC_SETFLG(prFrmHdr->rVp8DecParam.u4FlagParam,VP8PARAM_WEBP_FORCE_PPON);

  if(prWebPInfo->fgPartitionRing)
  {
	 VDEC_SETFLG(prFrmHdr->rVp8DecParam.u4FlagParam,VP8PARAM_RINGBUF_CTRL);
  }
  else if(!prWebPInfo->fgBufEnough)
  {
     prFrmHdr->u4VldStartPos=prWebPInfo->rSrcBufInfo[1].u4ReadPos;
  }
  
  if(prWebPInfo->u1DecMode==VDEC_VP8_DEC_PIC_MODE)
  {
     VDEC_SETFLG(prFrmHdr->rVp8DecParam.u4FlagParam,VP8PARAM_WEBP_PICMODE);
	 _VDEC_Vp8DecStart(prFrmHdr,TRUE);
  }
  else
  {
	 _VDEC_Vp8DecStart(prFrmHdr,FALSE);
  }
  return TRUE;
}

BOOL _Vdec_VP8HwWebPWaitDecodeDone(VDEC_VP8_INFO_T *prVdecInfo,UINT32 WaiteMode)
{
    WEBP_DEC_INFO_T *prWebPInfo=&prVdecInfo->rWebPInfo;
    WEBP_BUF_INFO *prBufInfo=NULL;
    UINT32 u4Flag=0,u4WaiteTime=0;
    BOOL fgRetVal=TRUE;
    if(WaiteMode==VDEC_VP8_WAITEMODE_POLLING)
    {
       do
       {
           u4Flag=prVdecInfo->rDecParam.u4Flag;
           if(VDEC_FLGSET(u4Flag,VP8_DEC_FLG_FINISHED)
           ||VDEC_FLGSET(u4Flag,VP8_DEC_FLG_MCUROW))
           {
              fgRetVal=TRUE;
              VDEC_CLRFLG(prVdecInfo->rDecParam.u4Flag,VP8_DEC_FLG_MCUROW);
              _VDEC_Vp8DecWait(5);
              break;
           }
           else if(VDEC_FLGSET(u4Flag,VP8_DEC_FLG_DECERROR))
           {
              fgRetVal=FALSE;
              LOG(2,"Webp decode error 1\n");
              _VDEC_Vp8DecWait(5);
              break;
           }
           else if(VDEC_FLGSET(u4Flag,VP8_DEC_FLG_FORCESTOP))
           {
              fgRetVal=FALSE;
              LOG(2,"Webp decode stop 1\n");
              break;
           }  
           
           if(VDEC_FLGSET(u4Flag,VP8_DEC_FLG_PARTCHG))
           {
              prBufInfo=&prWebPInfo->rSrcBufInfo[prWebPInfo->u4RingBufIndex];
              prBufInfo->u4UsedBitCount=_VDEC_Vp8GetBitCnt(VDEC_VLD2);
              _VDEC_Vp8PsrWebPRequestData(prVdecInfo);
              prWebPInfo->u4RingBufIndex=((prWebPInfo->u4RingBufIndex+1)%(prVdecInfo->rVp8FrmHdr.rVp8DecParam.uDataPartitionNum+1));
              if(prWebPInfo->u4RingBufIndex==0)
              {
                prWebPInfo->u4RingBufIndex=1;
              }
              prBufInfo=prWebPInfo->rSrcBufInfo+prWebPInfo->u4RingBufIndex;
              VDEC_CLRFLG(prVdecInfo->rDecParam.u4Flag,VP8_DEC_FLG_PARTCHG);
              _Vdec_VP8HwWebPReset(prBufInfo,TRUE);
              u4WaiteTime=0;
           }
           else
           {
              prBufInfo=&prWebPInfo->rSrcBufInfo[prWebPInfo->u4RingBufIndex];
              if(prWebPInfo->fgPartitionRing && _VDEC_Vp8DataExhaust())
              {
                 prBufInfo->u4UsedBitCount=_VDEC_Vp8GetBitCnt(VDEC_VLD2);
                 LOG(3,"Buffer %d empty, request data\n",prWebPInfo->u4RingBufIndex);
                 VDEC_SETFLG(prVdecInfo->rDecParam.u4Flag,VP8_DEC_FLG_DATAEMPTY);
                 if(_VDEC_Vp8PsrWebPRequestData(prVdecInfo)==0)
                 {
                    x_thread_delay(10);
                    u4WaiteTime+=10;
                 }
                 else
                 {
                    u4WaiteTime=0;
                 }
                 VDEC_CLRFLG(prVdecInfo->rDecParam.u4Flag,VP8_DEC_FLG_DATAEMPTY);                          
              }
              else
              {
                x_thread_delay(10);
                u4WaiteTime+=10;
              }
           }  

#if 0
      Printf("Vld1 FetchPos=0x%x,ReadPos=0x%x,WritePos=0x%x\n", \
        _Vdec_VP8GetFetchPos(VDEC_VLD1), \
        prWebPInfo->rSrcBufInfo[0].u4Start+(_VDEC_Vp8GetBitCnt(VDEC_VLD1)/8), \
          VDEC_VLD_READ32(WO_VLD_WPTR));
      Printf("Vld2 FetchPos=0x%x,ReadPos=0x%x,WritePos=0x%x\n", \
        _Vdec_VP8GetFetchPos(VDEC_VLD2), \
        prBufInfo->u4Start+(_VDEC_Vp8GetBitCnt(VDEC_VLD2)/8), \
        VDEC_VLD2_READ32(WO_VLD_WPTR));
#endif
           if(u4WaiteTime>5000)
           {
             LOG(0,"Webp decode timeout 1\n");
             _VDEC_Vp8DecInfo();
             if (VP8_VLD_READ32(RO_VP8_PICFSM) & (0x1ul << 19))
             {
                LOG(0, "!!!!!!!!!!!!!!!!!!!RESUME!!!!!!!!!!!!!!!!!!!!!!!!!!1\n");
                VP8_VLD_WRITE32(RW_VP8_DEC_RESUME,0x1);
             }
             else
             {
                ASSERT(0);
             }
             
             x_thread_delay(10);
             
             fgRetVal=FALSE;
             break;
           } 
       }while(1);
    }
    else
    {
       fgRetVal=TRUE;
       if(prWebPInfo->u1DecMode==VDEC_VP8_DEC_PIC_MODE)
       {
          u4WaiteTime=10000;
       }
       else
       {
          u4WaiteTime=1000;
       }
       
       fgRetVal=_VDEC_Vp8DecWait(u4WaiteTime);
       if(fgRetVal==FALSE)
       {
           LOG(0,"Webp decode timeout(%d) 2\n",u4WaiteTime);
           fgRetVal=FALSE;
       }
       else
       {
           u4Flag=prVdecInfo->rDecParam.u4Flag;
           if(VDEC_FLGSET(u4Flag,VP8_DEC_FLG_FINISHED)
           ||VDEC_FLGSET(u4Flag,VP8_DEC_FLG_MCUROW))
           {
              fgRetVal=TRUE;
           }
           else if(VDEC_FLGSET(u4Flag,VP8_DEC_FLG_DECERROR))
           {
              fgRetVal=FALSE;
              LOG(2,"Webp decode error 2\n");
           
           }
           else if(VDEC_FLGSET(u4Flag,VP8_DEC_FLG_FORCESTOP))
           {
              fgRetVal=FALSE;
              LOG(2,"Webp decode stop 2\n");
           } 
       }
    }

	if(fgRetVal==FALSE)
	{
	   _VDEC_Vp8DecInfo();
	   _VDEC_Vp8DecReset(&prVdecInfo->rVp8FrmHdr,TRUE);
	}
    return fgRetVal;
}


BOOL _Vdec_VP8HwWaitDecodeDone(VDEC_VP8_INFO_T *prVdecInfo)
{
   VDEC_INFO_VP8_FRM_HDR_T *prFrmHdr=&prVdecInfo->rVp8FrmHdr;
   UINT32 u4CntTimeChk=0,u4Cnt=0,u4LastXMb=0,u4LastYMb=0,u4ErrType=0;
   BOOL fgDecFin=FALSE;
#ifdef VP8_TEST_BUFRING
   UINT32 u4WiteTime=0;
#endif

#ifdef SUPPORT_DEC_ISR
   if(x_sema_lock_timeout(_hVP8VdecFinSema,VDEC_VP8WAIT_TIME)!=OSR_OK)
   {
      fgDecFin=FALSE;
      LOG(3,"VP8 webm decoder time out\n");
   }
   else
   {
#ifdef VP8_TIME_PROFILE
       prVdecInfo->rTimeProfile.u4CurDramCycle=IO_REG32(DRAM_BASE, 0x190);
       prVdecInfo->rTimeProfile.u4CurDecCycle=IO_REG32(MC0_BASE, 0x778);
	   HAL_GetTime(&prVdecInfo->rTimeProfile.rTimeE);
	   HAL_GetDeltaTime(&prVdecInfo->rTimeProfile.rTimeDif, \
	   	     &prVdecInfo->rTimeProfile.rTimeS, \
		     &prVdecInfo->rTimeProfile.rTimeE);
		prVdecInfo->rTimeProfile.u4DecMs=prVdecInfo->rTimeProfile.rTimeDif.u4Micros;
		HAL_GetTime(&prVdecInfo->rTimeProfile.rTimeS);
#endif
      fgDecFin=TRUE;
   }
#endif
   
   while(u4CntTimeChk<DEC_RETRY_NUM)
   {    
#ifndef SUPPORT_DEC_ISR
      if((u4Cnt&0x3f)==0x3f)
#else
      if((u4Cnt&0x3f)==0x3f || (u4Cnt==0&&fgDecFin))
#endif
      {
         if(_VDEC_Vp8IsDecFinish())
         {
            UINT32 u4XMb=0,u4YMb=0;
            _Vdec_Vp8DecProgress(&u4XMb,&u4YMb);
            if(((u4XMb+1)>=(prFrmHdr->u2Width>>4))&&((u4YMb+1)>=(prFrmHdr->u2Height>>4)))
            {
               fgDecFin=TRUE;
               break;
            }
            
            if(u4XMb!=u4LastXMb || u4LastYMb!=u4YMb)
            {
               u4CntTimeChk=0;
            }
            else
            {
                u4CntTimeChk++;
            }
            if(u4CntTimeChk>100)
            {
               fgDecFin=FALSE;
               break;
            }
            u4LastXMb=u4XMb;
            u4LastYMb=u4YMb;
         }
        
         x_thread_delay(1);
#ifdef VP8_TEST_BUFRING
         prVdecInfo->rDecParam.u4VldReadPtr=VDEC_VLD2_READ32(RO_VLD_VRPTR);
         if((prVdecInfo->rDecParam.u4VldReadPtr==prFrmHdr->u4WritePos)&&!(_VDEC_Vp8IsDecFinish()))
         {
            Printf("Data exhausted %d,ReadPtr=0x%x\n",u4FrameIndex,prVdecInfo->rDecParam.u4VldReadPtr);
            x_thread_delay(1000);
            if(u4WiteTime==8)
            {
                prFrmHdr->u4WritePos=prFrmHdr->u4FifoEnd-(prVdecInfo->rPesInfo.u4DmxFrameType>>1)-1;
                prFrmHdr->u4WritePos=prFrmHdr->u4WritePos-(prFrmHdr->u4WritePos&0XF);
                _VDEC_VP8_VLDSetWritePtr(prFrmHdr->u4WritePos);
            }
            u4WiteTime++;
         }
#endif
      } 
      if(u4Cnt==0x3f*2000)
      {
         _Vdec_VP8HwPrintRegInfo(prVdecInfo);
      }
      u4Cnt++;
   }

   u4ErrType=_VDEC_Vp8GetErrInfo();
   if(fgDecFin)
   {
#ifdef VP8_ENABLE_ERRDETECT
      if(u4ErrType&VP8_ERROR_DETECT_TYPE)
      {
          LOG(5,"Vp8 decode error concealment 0x%x\n",u4ErrType);
          _VDEC_Vp8DecInfo();
      }
      else if(u4ErrType!=0)
      {
          fgDecFin=FALSE;
      }
#else
      if(u4ErrType!=0)
      {
          fgDecFin=FALSE;
      }
#endif     
   }

   if(!fgDecFin)
   {
      LOG(3,"Vp8 decode error 0x%x\n",u4ErrType);
      _VDEC_Vp8DecInfo();
   }
   
   _VDEC_Vp8DecFinish(prFrmHdr);
   
   if (fgDecFin==FALSE
#ifdef CC_VP8_EMULATION
    || prVdecInfo->rDecParam.u4FrameCounter == prVdecInfo->rDecParam.u4DumpFrameIndex
#endif
    )
   {
       _Vdec_VP8HwPrintRegInfo(prVdecInfo);
       ASSERT(0);
   }

#ifdef VP8_ENABLE_CRC
   if(fgDecFin)
   {
      _VDEC_Vp8GetCrcValue(prVdecInfo->rDecParam.pu4CrcVal,&prVdecInfo->rDecParam.u4DbkType);
   }
#endif

   prVdecInfo->rDecParam.u4VldReadPtr=_VDEC_VP8_VLDGetReadPtr(&prVdecInfo->rDecParam.u4VldReadBits);
#ifdef VP8_TEST_BUFRING
   Printf("EndAddr:(0x%x,0x%x)\n",prVdecInfo->rDecParam.u4VldReadPtr,prVdecInfo->rDecParam.u4VldReadBits);
#endif

   if(!fgDecFin)
   {
       LOG(3,"Vp8 decode error\n");
       _VDEC_Vp8DecInfo();
	   _VDEC_Vp8DecReset(&prVdecInfo->rVp8FrmHdr,TRUE);
       return FALSE;
   }
   
   return TRUE;
}

VOID _Vdec_VP8HwResetWapper(VOID)
{
	VDEC_VLD_TOP_WRITE32(RW_PRED_DRAM_B_ADDR,0);
	VDEC_VLD_TOP_WRITE32(RW_SEGID_DRAM_ADDR,0x4000);
	VDEC_VLD_TOP_WRITE32(RW_PRED_SRAM_CFG,0x80000000);
	VDEC_VLD_TOP_WRITE32(RW_PRED_MODE,3);
	VDEC_MC_WRITE32(RW_MC_DDR3_EN,0x00200000);
	VDEC_MC_WRITE32(RW_MC_PP_DBLK_Y_ADDR,0);
	VDEC_MC_WRITE32(RW_MC_PP_DBLK_C_ADDR,0);
	return;
}

VOID _Vdec_VP8HwPowerOff(VOID)
{
	return _VDEC_VP8_VLD_PowerOff();
}

UINT32 _Vdec_VP8GetReadPtr(UINT32 *pu4Bits)
{
   return _VDEC_VP8_VLDGetReadPtr(pu4Bits);
}

UINT32 _Vdec_VP8GetFetchPos(UINT32 u4VldId)
{
   if(u4VldId==VDEC_VLD1)
   {
       return VDEC_VLD_READ32(RO_VLD_VRPTR);
   }
   else
   {
       return VDEC_VLD2_READ32(RO_VLD_VRPTR);
   }
}

VOID _Vdec_VP8SetWritePtr(UINT32 u4WritePtr)
{
   return _VDEC_VP8_VLDSetWritePtr(u4WritePtr);
}

VOID _Vdec_VP8HwPrintRegInfo(VDEC_VP8_INFO_T *prVdecInfo)
{
        UINT32 i;
		if(!VDEC_FLGSET(prVdecInfo->rDecParam.u4Flag,VP8_DEC_FLG_LOGERRINFO))
		{
           return;
		}

        /* VLD Registers */
        x_thread_delay(10000);
        
        LOG(0, "\n*******Start Dump VLD Registers *******\n");
        LOG(0, "VLD1 ReadPtr: 0x%08x\n", VDEC_VLD_READ32(RW_VLD_RPTR));
        LOG(0, "VLD2 ReadPtr: 0x%08x\n", VDEC_VLD2_READ32(RW_VLD_RPTR));
        
        /* VP8 VLD */
        LOG(0, "******* VP8 VLD Registers *******\n");
        for (i = 41; i < 68; i++)
        {
            LOG(0, "VP8 VLD %d (0x%04x): 0x%08x\n", i, i * 4, VP8_VLD_READ32(i * 4));
            x_thread_delay(10);
        }
        
        for (i = 72; i < 97; i++)
        {
           LOG(0, "VP8 VLD %d (0x%04x): 0x%08x\n", i, i * 4, VP8_VLD_READ32(i * 4));
           x_thread_delay(10);
        }
    
        /* TOP VLD */
        LOG(0, "******* TOP VLD Registers *******\n");
        for (i = 0; i < 100; i++)
        {
            LOG(0, "TOP VLD %d (0x%04x): 0x%08x\n", i, i * 4, VDEC_VLD_TOP_READ32(i * 4));
            x_thread_delay(10);
        }
        
        /* MC */
        LOG(0, "******* MC Registers *******\n");
        for (i = 0; i < 700; i++)
        {
            LOG(0, "MC %d (0x%04x): 0x%08x\n", i, i * 4, VDEC_MC_READ32(i * 4));
            x_thread_delay(10);
        }
        
        /* MV */
        LOG(0, "******* MV Registers *******\n");
        LOG(0, "MV 181 (0x0x02D4): 0x%08x\n", VDEC_MV_READ32(i * 4));
    
        
        /* PP */
        LOG(0, "******* PP Registers *******\n");
        for (i = 0; i < 640; i++)
        {
            LOG(0, "PP %d (0x%04x): 0x%08x\n", i, i * 4, VDEC_PP_READ32(i * 4));
            x_thread_delay(10);
        }
    
#if defined(VP8_ENABLE_CRC) || defined(WEBP_ENABLE_CRC)
        /* CRC */
        LOG(0, "******* MISC Registers *******\n");
        for (i = 0; i < 10; i++)
        {
            LOG(0, "MISC %d (0x%04x): 0x%08x\n", i, i * 4, VDEC_MISC_READ32(i * 4));
            x_thread_delay(10);
        }
#endif
    
        LOG(0, "******* IQ Registers *******\n");
        for(i=0;i<MAX_MB_SEGMENTS;i++)
        {
           VDEC_VLD_WRITE32(RW_VLD_SCL_ADDR,0x100+(i<<4));
           LOG(0, "IQ Y2 %d (0x%04x): 0x%08x\n", i, i<<4,VDEC_VLD_READ32(RW_VLD_SCL_DATA));
           x_thread_delay(10);
    
           VDEC_VLD_WRITE32(RW_VLD_SCL_ADDR,0x100+(i<<4)+4);
           LOG(0, "IQ Y1 %d (0x%04x): 0x%08x\n", i, (i<<4)+4, VDEC_VLD_READ32(RW_VLD_SCL_DATA));
           x_thread_delay(10);
          
           VDEC_VLD_WRITE32(RW_VLD_SCL_ADDR,0x100+(i<<4)+8);
           LOG(0, "IQ UV %d (0x%04x): 0x%08x\n", i, (i<<4)+8, VDEC_VLD_READ32(RW_VLD_SCL_DATA));
           x_thread_delay(10);
        }
    
        LOG(0, "******* MV Prob *******\n");
        
        _VDEC_Vp8LoadCtxProbs(&prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC);
        for(i=0;i<VP8_YMODES-1;i++)
        {
          LOG(0, "YModeProb(%d)=0x%x\n",i,prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC.YModeProb[i]);
          x_thread_delay(10);
          //x_thread_delay(3);
        }
    
        for(i=0;i<VP8_UVMODES-1;i++)
        {
            LOG(0, "UVModeProb(%d)=0x%x\n",i,prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC.UVModeProb[i]);
            x_thread_delay(10);
        }
    
        for(i=0;i<VP8_MVDEF_MVPCOUNT;i++)
        {
            LOG(0, "MVC0Prob(%d)=0x%x\n",i,prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC.MVC[0].Prob[i]);
            x_thread_delay(10);
        }
        
        for(i=0;i<VP8_MVDEF_MVPCOUNT;i++)
        {
            LOG(0, "MVC1Prob(%d)=0x%x\n",i,prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC.MVC[1].Prob[i]);
            x_thread_delay(10);
        }
    
        LOG(0, "******* Coefficient Prob *******\n");
        
        _VDEC_Vp8FlushCoefProbs(&prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC);
        {
            UINT32 u4BlockType=0,u4Band=0,u4Context=0,u4Tolen=0;
            for(u4BlockType=0;u4BlockType<VP8_BLOCK_TYPES;u4BlockType++)
            {
               for(u4Band=0;u4Band<VP8_COEF_BANDS;u4Band++)
               {
                  for(u4Context=0;u4Context<VP8_PREV_COEF_CONTEXTS;u4Context++)
                  {  
                      for(u4Tolen=0;u4Tolen<VP8_COEF_TOKENS-1;u4Tolen++)
                      {
                         LOG(0, "CoefProb[%d][%d][%d][%d]=0x%x\n",u4BlockType,u4Band,u4Context,u4Tolen, \
                            prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC.CoefProbs[u4BlockType][u4Band][u4Context][u4Tolen]);
                         x_thread_delay(10);
                      }
                  }
               }
            }       
        }
        LOG(0, "****** Dump VLD Registers End*******\n");
        x_thread_delay(20000);

#if 0
    UINT32 i;

    /* VLD Registers */
    x_thread_delay(10000);
    
    printf("\n*******Start Dump VLD Registers *******\n");
    printf("VLD1 ReadPtr: 0x%08x\n", VDEC_VLD_READ32(RW_VLD_RPTR));
    printf("VLD2 ReadPtr: 0x%08x\n", VDEC_VLD2_READ32(RW_VLD_RPTR));
    
    /* VP8 VLD */
    printf("******* VP8 VLD Registers *******\n");
    for (i = 41; i < 68; i++)
    {
        printf("VP8 VLD %d (0x%04x): 0x%08x\n", i, i * 4, VP8_VLD_READ32(i * 4));
    }
    
    for (i = 72; i < 97; i++)
    {
       printf("VP8 VLD %d (0x%04x): 0x%08x\n", i, i * 4, VP8_VLD_READ32(i * 4));
    }

    /* TOP VLD */
    printf("******* TOP VLD Registers *******\n");
    for (i = 0; i < 100; i++)
    {
        printf("TOP VLD %d (0x%04x): 0x%08x\n", i, i * 4, VDEC_VLD_TOP_READ32(i * 4));
    }
    
    /* MC */
    printf("******* MC Registers *******\n");
    for (i = 0; i < 700; i++)
    {
        printf("MC %d (0x%04x): 0x%08x\n", i, i * 4, VDEC_MC_READ32(i * 4));
    }
    
    /* MV */
    printf("******* MV Registers *******\n");
    printf("MV 181 (0x0x02D4): 0x%08x\n", VDEC_MV_READ32(i * 4));

    
    /* PP */
    printf("******* PP Registers *******\n");
    for (i = 0; i < 30; i++)
    {
        printf("PP %d (0x%04x): 0x%08x\n", i, i * 4, VDEC_PP_READ32(i * 4));
    }

#ifdef VP8_ENABLE_CRC
    /* CRC */
    printf("******* MISC Registers *******\n");
    for (i = 0; i < 10; i++)
    {
        printf("MISC %d (0x%04x): 0x%08x\n", i, i * 4, VDEC_MISC_READ32(i * 4));
    }
#endif

    printf("******* IQ Registers *******\n");
    for(i=0;i<MAX_MB_SEGMENTS;i++)
    {
       VDEC_VLD_WRITE32(RW_VLD_SCL_ADDR,0x100+(i<<4));
       printf("IQ Y2 %d (0x%04x): 0x%08x\n", i, i<<4,VDEC_VLD_READ32(RW_VLD_SCL_DATA));

       VDEC_VLD_WRITE32(RW_VLD_SCL_ADDR,0x100+(i<<4)+4);
       printf("IQ Y1 %d (0x%04x): 0x%08x\n", i, (i<<4)+4, VDEC_VLD_READ32(RW_VLD_SCL_DATA));
      
       VDEC_VLD_WRITE32(RW_VLD_SCL_ADDR,0x100+(i<<4)+8);
       printf("IQ UV %d (0x%04x): 0x%08x\n", i, (i<<4)+8, VDEC_VLD_READ32(RW_VLD_SCL_DATA));
    }

    printf("******* MV Prob *******\n");
    
    _VDEC_Vp8LoadCtxProbs(&prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC);
    for(i=0;i<VP8_YMODES-1;i++)
    {
      printf("YModeProb(%d)=0x%x\n",i,prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC.YModeProb[i]);
    }

    for(i=0;i<VP8_UVMODES-1;i++)
    {
        printf("UVModeProb(%d)=0x%x\n",i,prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC.UVModeProb[i]);
    }

    for(i=0;i<VP8_MVDEF_MVPCOUNT;i++)
    {
        printf("MVC0Prob(%d)=0x%x\n",i,prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC.MVC[0].Prob[i]);
    }
    
    for(i=0;i<VP8_MVDEF_MVPCOUNT;i++)
    {
        printf("MVC1Prob(%d)=0x%x\n",i,prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC.MVC[1].Prob[i]);
    }

    printf("******* Coefficient Prob *******\n");
    
    _VDEC_Vp8FlushCoefProbs(&prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC);
    {
        UINT32 u4BlockType=0,u4Band=0,u4Context=0,u4Tolen=0;
        for(u4BlockType=0;u4BlockType<VP8_BLOCK_TYPES;u4BlockType++)
        {
           for(u4Band=0;u4Band<VP8_COEF_BANDS;u4Band++)
           {
              for(u4Context=0;u4Context<VP8_PREV_COEF_CONTEXTS;u4Context++)
              {  
                  for(u4Tolen=0;u4Tolen<VP8_COEF_TOKENS-1;u4Tolen++)
                  {
                     printf("CoefProb[%d][%d][%d][%d]=0x%x\n",u4BlockType,u4Band,u4Context,u4Tolen, \
                        prVdecInfo->rVp8FrmHdr.rVp8DecParam.rLastFC.CoefProbs[u4BlockType][u4Band][u4Context][u4Tolen]);
                  }
              }
           }
        }       
    }
    printf("****** Dump VLD Registers End*******\n");
    x_thread_delay(20000);
#endif
}

#ifdef VP8_EMUCOVERAGE_STATISTIC
VOID _Vdec_VP8UpdateEmuItemInfo(VDEC_INFO_VP8_FRM_HDR_T *pVp8DecInfo,UINT8 uType)
{
  VDEC_VP8_EMUCOVERAGE_INFO_T *prItems=NULL;
  UINT32 u4Index=0;
  if(uType==1) // init
  {
     UINT8 *puData=_puStatisticBuf;
     UINT8 rMaxMin[VP8_EMUCOVERAGE_ITEM_NUM][2]=
          {
            {0,3},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},
            // abs
            {0,1},{0,1},{0,63},{0,7},{0,1},{0,1},{0,3},
            // update q
            {0,1},{0,1},{0,1},{0,1},{0,2},{0,2},{0,1},{0,1},{0,255}
          };

     if(pVp8DecInfo->prEmuItems==NULL)
     {
       pVp8DecInfo->prEmuItems=x_mem_alloc(VP8_EMUCOVERAGE_ITEM_NUM*sizeof(VDEC_VP8_EMUCOVERAGE_INFO_T));
       ASSERT(pVp8DecInfo->prEmuItems!=0);
       x_memset(pVp8DecInfo->prEmuItems,0,sizeof(VP8_EMUCOVERAGE_ITEM_NUM*sizeof(VDEC_VP8_EMUCOVERAGE_INFO_T)));
     }
     
     prItems=pVp8DecInfo->prEmuItems;
     for(u4Index=0;u4Index<VP8_EMUCOVERAGE_ITEM_NUM;u4Index++)
     {
       prItems[u4Index].puData=puData;
       puData+=(rMaxMin[u4Index][1]-rMaxMin[u4Index][0])+1;
       ASSERT(puData<(_puStatisticBuf+VP8_EMUCOVERAGE_BUFSIZE));
     }
     x_memset(_puStatisticBuf,0,VP8_EMUCOVERAGE_BUFSIZE);
  }
  else if(uType==2) // end print
  {
     UINT32 u4VlaueIndex=0,u4Min,u4Max,u4Dot=0;
     prItems=pVp8DecInfo->prEmuItems;
     UINT8 rMaxMin[VP8_EMUCOVERAGE_ITEM_NUM][2]=
          {
            {0,3},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},
            // abs
            {0,1},{0,1},{0,63},{0,7},{0,1},{0,1},{0,3},
            // update q
            {0,1},{0,1},{0,1},{0,1},{0,2},{0,2},{0,1},{0,1},{0,255}
          };

     Printf("***Bitstream statistic Info***\n:",u4Index);
     Printf("Widht=%d,Height=%d\n",pVp8DecInfo->u2Width,pVp8DecInfo->u2Height);
     for(u4Index=0;u4Index<VP8_EMUCOVERAGE_ITEM_NUM-1;u4Index++)
     {
        u4Dot=0;
        u4Min=rMaxMin[u4Index][0];
        u4Max=rMaxMin[u4Index][1];

        for(u4VlaueIndex=u4Min;u4VlaueIndex<=u4Max;u4VlaueIndex++)
        {
          if(prItems[u4Index].puData[u4VlaueIndex-u4Min])
          {
             if(u4Dot)
             {
                Printf(",");
             }
             Printf("%d",u4VlaueIndex);
             u4Dot=1;
          }
        }
        
        Printf("\t");
     }
     Printf("\n");
     x_mem_free(pVp8DecInfo->prEmuItems);
     pVp8DecInfo->prEmuItems=NULL;
  }
  else  // update
  {
     UINT32 u4FlagParam=pVp8DecInfo->rVp8DecParam.u4FlagParam;
     prItems=pVp8DecInfo->prEmuItems;
     // verson.
     prItems[0].puData[pVp8DecInfo->uVersion]=1;
     prItems[1].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_BILINER_MCFILTER)]=1;
     prItems[2].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_FULL_PIXEL)]=1;
     prItems[3].puData[pVp8DecInfo->uShowFrame]=1;
     if(pVp8DecInfo->uFrameType==VP68_KEY_FRAME)
     {
         prItems[4].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_COLOR)]=1;
         prItems[5].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_CLAMP_TYPE)]=1;
     }
     
     prItems[6].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_ENABLE)]=1;
     if(VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_ENABLE))
     {
         prItems[7].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_UPDATE_MAP)]=1;
         prItems[8].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_UPDATE_DATA)]=1;
     }
     
     prItems[9].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_SEGMENT_ABSDATA)]=1;
     prItems[10].puData[pVp8DecInfo->rVp8DecParam.eLoopFilterType]=1;
     prItems[11].puData[pVp8DecInfo->rVp8DecParam.iLoopFilterLvl]=1;
     prItems[12].puData[pVp8DecInfo->rVp8DecParam.iSharpLvl]=1;
     prItems[13].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_MODEREF_LFDELTA_ENABLE)]=1;
     
     if(VDEC_FLGSET(u4FlagParam,VP8PARAM_MODEREF_LFDELTA_ENABLE))
     {
         prItems[14].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_MODEREF_IFDELTA_UPDATE)]=1;
     }
     
     prItems[15].puData[pVp8DecInfo->rVp8DecParam.uDataPartitionToken]=1;
     prItems[16].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_QINDEX_UPDATE)]=1;
     prItems[17].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_REFRESH_GOLDEN)]=1;
     prItems[18].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_REFRESH_ALTRF)]=1;
     prItems[19].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_REFRESH_LASTFRAME)]=1;
     
     prItems[20].puData[pVp8DecInfo->rVp8DecParam.uCopyBuf2Gf]=1;
     prItems[21].puData[pVp8DecInfo->rVp8DecParam.uCopyBuf2Arf]=1;
     prItems[22].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_REFRESH_PROBS)]=1;
     prItems[23].puData[VDEC_FLGSET(u4FlagParam,VP8PARAM_NOCOEF_SKIP)]=1;
     prItems[24].puData[pVp8DecInfo->rVp8DecParam.uSkipFalseProb]=1;
  }
}
#endif

