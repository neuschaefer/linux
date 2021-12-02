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
 * $RCSfile: _cl_desc_dec_table.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 78a46ff279870d6d5962fef68341eb73 $ Asa
 *
 * Description: 
 *         This file contains the byte array of the huffman program description 
 *         decoding table. 
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/* the byte arry of program description decoding table for huffman decoding engine*/

UCHAR _desc_dec_table[1782] = {
1,    0,    1,    44,    1,    46,    1,    48,    1,    50,    
1,    52,    1,    54,    1,    56,    1,    58,    1,    60,    
1,    62,    1,    64,    1,    66,    1,    68,    1,    70,    
1,    72,    1,    74,    1,    76,    1,    78,    1,    80,    
1,    82,    1,    84,    1,    86,    1,    88,    1,    90,    
1,    92,    1,    94,    1,    96,    1,    98,    1,    100,    
1,    102,    1,    104,    1,    106,    1,    222,    1,    224,    
1,    234,    1,    236,    1,    238,    1,    240,    1,    242,    
1,    248,    1,    250,    1,    252,    1,    254,    2,    0,    
2,    4,    2,    22,    2,    32,    2,    34,    2,    44,    
2,    50,    2,    56,    2,    60,    2,    64,    2,    68,    
2,    70,    2,    74,    2,    76,    2,    84,    2,    86,    
2,    88,    2,    90,    2,    92,    2,    94,    2,    96,    
2,    98,    2,    118,    2,    132,    2,    148,    2,    162,    
2,    178,    2,    186,    2,    200,    2,    210,    2,    222,    
2,    234,    2,    242,    2,    252,    3,    8,    3,    16,    
3,    26,    3,    40,    3,    42,    3,    52,    3,    74,    
3,    90,    3,    94,    3,    100,    3,    110,    3,    112,    
3,    114,    3,    116,    3,    118,    3,    120,    3,    122,    
3,    124,    3,    126,    3,    128,    3,    180,    3,    206,    
3,    240,    4,    26,    4,    88,    4,    110,    4,    142,    
4,    172,    4,    216,    4,    224,    4,    244,    5,    36,    
5,    64,    5,    118,    5,    174,    5,    206,    5,    208,    
6,    6,    6,    52,    6,    96,    6,    134,    6,    146,    
6,    170,    6,    184,    6,    220,    6,    236,    6,    238,    
6,    240,    6,    242,    6,    244,    20,    21,    155,    214,    
201,    207,    215,    199,    1,    162,    206,    203,    2,    3,    
197,    204,    198,    200,    4,    196,    5,    194,    6,    195,    
210,    7,    211,    8,    202,    212,    9,    205,    208,    10,    
193,    11,    12,    13,    14,    15,    16,    17,    18,    19,    
155,    155,    155,    155,    155,    155,    155,    155,    155,    155,    
155,    155,    155,    155,    155,    155,    155,    155,    155,    155,    
155,    155,    155,    155,    155,    155,    155,    155,    155,    155,    
155,    155,    155,    155,    155,    155,    155,    155,    155,    155,    
155,    155,    155,    155,    155,    155,    155,    155,    155,    155,    
155,    155,    155,    155,    155,    155,    155,    155,    155,    155,    
155,    155,    56,    57,    173,    175,    183,    218,    168,    179,    
181,    1,    2,    155,    180,    241,    162,    213,    214,    217,    
3,    4,    5,    207,    6,    201,    249,    234,    235,    245,    
246,    7,    8,    9,    178,    197,    198,    177,    10,    238,    
203,    11,    212,    12,    196,    200,    210,    13,    14,    15,    
199,    202,    206,    208,    215,    16,    194,    17,    204,    236,    
229,    231,    18,    205,    19,    20,    195,    21,    22,    23,    
237,    24,    25,    242,    26,    211,    27,    28,    228,    29,    
193,    227,    30,    233,    240,    226,    247,    31,    243,    230,    
32,    33,    34,    232,    239,    35,    36,    37,    38,    39,    
40,    41,    42,    244,    43,    44,    45,    46,    47,    225,    
48,    49,    50,    51,    52,    53,    54,    55,    155,    155,    
3,    4,    128,    174,    200,    212,    1,    2,    155,    160,    
155,    155,    155,    155,    155,    155,    155,    155,    2,    243,    
160,    244,    155,    1,    155,    155,    172,    155,    155,    155,    
155,    155,    1,    160,    155,    162,    7,    8,    226,    228,    
229,    230,    160,    242,    225,    1,    2,    243,    227,    3,    
4,    5,    155,    6,    4,    128,    202,    211,    162,    1,    
155,    2,    3,    160,    155,    160,    3,    4,    155,    183,    
244,    160,    176,    243,    1,    2,    185,    2,    184,    155,    
160,    1,    174,    2,    182,    155,    1,    160,    160,    1,    
155,    176,    174,    1,    155,    160,    174,    1,    160,    155,    
155,    155,    155,    1,    172,    174,    155,    155,    2,    3,    
155,    160,    181,    182,    184,    1,    155,    160,    155,    160,    
155,    155,    155,    155,    155,    155,    155,    160,    155,    155,    
8,    9,    230,    245,    243,    244,    155,    228,    1,    237,    
2,    3,    4,    242,    5,    6,    236,    238,    7,    160,    
5,    6,    155,    236,    245,    1,    2,    225,    239,    229,    
233,    242,    3,    4,    6,    7,    155,    233,    249,    242,    
245,    1,    2,    3,    236,    239,    225,    4,    232,    5,    
5,    6,    249,    242,    245,    155,    229,    239,    1,    2,    
233,    225,    3,    4,    6,    7,    225,    233,    238,    246,    
228,    236,    243,    1,    2,    242,    3,    4,    155,    5,    
2,    3,    229,    236,    155,    239,    1,    242,    5,    6,    
245,    239,    155,    236,    233,    1,    225,    242,    2,    229,    
3,    4,    3,    4,    155,    229,    233,    245,    225,    1,    
239,    2,    4,    5,    160,    201,    243,    155,    174,    242,    
1,    2,    3,    238,    239,    5,    155,    174,    233,    229,    
1,    245,    2,    225,    3,    4,    229,    3,    225,    233,    
242,    155,    1,    2,    3,    4,    155,    233,    245,    1,    
229,    2,    239,    225,    225,    5,    155,    227,    239,    1,    
245,    229,    2,    3,    233,    4,    229,    3,    155,    233,    
1,    225,    239,    2,    3,    4,    167,    238,    236,    242,    
243,    1,    155,    2,    225,    6,    155,    232,    233,    1,    
242,    236,    2,    239,    3,    229,    4,    5,    155,    155,    
3,    4,    155,    174,    1,    233,    2,    225,    229,    239,    
9,    10,    246,    249,    1,    174,    227,    233,    245,    155,    
229,    239,    2,    3,    225,    4,    232,    5,    6,    244,    
7,    8,    232,    7,    229,    247,    214,    225,    155,    233,    
242,    1,    2,    3,    4,    239,    5,    6,    174,    1,    
155,    238,    233,    2,    229,    155,    160,    1,    3,    4,    
155,    232,    229,    225,    239,    1,    233,    2,    155,    155,    
155,    239,    155,    155,    155,    155,    155,    155,    155,    155,    
155,    155,    155,    155,    155,    155,    24,    25,    232,    239,    
248,    155,    167,    247,    250,    1,    2,    3,    4,    229,    
174,    5,    230,    226,    6,    246,    235,    245,    233,    7,    
240,    249,    231,    8,    9,    228,    10,    227,    11,    237,    
12,    243,    13,    14,    15,    236,    16,    244,    17,    18,    
242,    160,    19,    20,    21,    238,    22,    23,    11,    12,    
228,    243,    155,    174,    226,    1,    2,    3,    236,    160,    
4,    233,    242,    245,    5,    249,    225,    6,    239,    7,    
229,    8,    9,    10,    15,    16,    241,    174,    196,    249,    
172,    1,    227,    2,    155,    242,    3,    4,    160,    236,    
245,    5,    6,    233,    7,    235,    8,    244,    9,    229,    
10,    239,    225,    232,    11,    12,    13,    14,    19,    20,    
167,    187,    230,    237,    247,    231,    246,    1,    2,    155,    
238,    3,    4,    236,    5,    245,    6,    172,    228,    249,    
242,    7,    8,    9,    174,    10,    239,    11,    225,    243,    
12,    233,    13,    14,    15,    16,    229,    17,    18,    160,    
29,    30,    169,    232,    245,    155,    1,    173,    187,    235,    
250,    2,    167,    230,    226,    231,    3,    4,    5,    6,    
233,    248,    7,    172,    239,    240,    8,    237,    246,    249,    
9,    247,    10,    11,    174,    12,    227,    13,    229,    244,    
14,    15,    228,    16,    236,    17,    225,    18,    19,    20,    
21,    22,    238,    243,    23,    24,    242,    160,    25,    26,    
27,    28,    9,    10,    174,    155,    236,    1,    245,    2,    
244,    230,    3,    225,    229,    233,    4,    242,    239,    5,    
6,    7,    160,    8,    14,    15,    173,    231,    155,    167,    
249,    1,    236,    2,    172,    242,    3,    174,    243,    245,    
4,    5,    239,    6,    7,    233,    225,    8,    9,    232,    
10,    11,    229,    12,    160,    13,    13,    14,    167,    172,    
243,    173,    1,    2,    155,    249,    245,    174,    3,    238,    
4,    242,    5,    6,    244,    7,    8,    9,    239,    225,    
160,    10,    233,    11,    12,    229,    20,    21,    172,    226,    
248,    155,    174,    250,    1,    235,    2,    160,    3,    4,    
240,    5,    6,    230,    246,    7,    228,    237,    231,    8,    
225,    239,    242,    9,    10,    11,    236,    12,    229,    227,    
13,    244,    14,    243,    15,    16,    17,    238,    18,    19,    
3,    239,    155,    225,    229,    245,    1,    2,    8,    9,    
236,    249,    167,    238,    1,    172,    155,    174,    2,    3,    
4,    243,    5,    233,    6,    160,    7,    229,    22,    23,    
167,    173,    238,    227,    235,    242,    155,    226,    1,    2,    
245,    3,    244,    172,    4,    5,    230,    237,    246,    6,    
174,    240,    7,    8,    243,    9,    10,    228,    11,    12,    
249,    13,    239,    14,    225,    15,    16,    233,    236,    17,    
160,    229,    18,    19,    20,    21,    12,    13,    167,    187,    
155,    1,    249,    174,    226,    2,    237,    243,    3,    245,    
239,    240,    4,    5,    233,    6,    7,    8,    9,    160,    
225,    229,    10,    11,    25,    26,    173,    187,    226,    234,    
237,    242,    250,    230,    236,    1,    2,    3,    155,    245,    
4,    167,    246,    249,    5,    6,    235,    239,    7,    8,    
9,    10,    172,    11,    12,    227,    174,    13,    238,    233,    
14,    225,    15,    243,    16,    17,    244,    18,    231,    229,    
19,    20,    228,    21,    22,    23,    160,    24,    26,    27,    
194,    155,    173,    172,    248,    1,    174,    2,    3,    229,    
231,    232,    249,    233,    235,    4,    227,    225,    5,    246,    
6,    228,    7,    226,    240,    8,    9,    243,    244,    247,    
239,    10,    11,    12,    13,    236,    14,    15,    16,    245,    
237,    17,    230,    160,    18,    242,    19,    20,    21,    238,    
22,    23,    24,    25,    14,    15,    173,    237,    249,    155,    
174,    1,    243,    2,    3,    245,    244,    240,    4,    239,    
5,    233,    6,    232,    160,    225,    236,    7,    242,    8,    
229,    9,    10,    11,    12,    13,    155,    245,    25,    26,    
169,    187,    246,    230,    1,    155,    173,    226,    240,    2,    
167,    3,    4,    5,    245,    227,    172,    231,    242,    6,    
235,    7,    236,    237,    238,    249,    8,    174,    9,    10,    
228,    11,    12,    244,    13,    243,    14,    15,    16,    225,    
239,    17,    233,    18,    19,    229,    20,    160,    21,    22,    
23,    24,    160,    22,    162,    167,    226,    235,    237,    238,    
155,    247,    1,    2,    3,    187,    249,    240,    4,    5,    
236,    6,    7,    8,    245,    225,    9,    172,    227,    10,    
232,    11,    233,    12,    239,    243,    174,    13,    14,    229,    
15,    16,    17,    244,    18,    19,    20,    21,    20,    21,    
187,    226,    173,    237,    1,    155,    167,    227,    172,    236,    
238,    2,    247,    3,    4,    249,    5,    6,    7,    8,    
244,    174,    245,    9,    10,    242,    225,    243,    11,    12,    
13,    233,    14,    15,    239,    229,    16,    160,    232,    17,    
18,    19,    17,    18,    239,    246,    155,    235,    249,    1,    
160,    226,    2,    225,    3,    237,    4,    227,    233,    5,    
228,    229,    231,    6,    236,    240,    7,    8,    9,    10,    
11,    243,    12,    244,    238,    13,    242,    14,    15,    16,    
5,    229,    243,    249,    155,    1,    239,    2,    3,    225,    
4,    233,    10,    11,    174,    155,    236,    237,    1,    2,    
243,    238,    242,    3,    229,    4,    232,    160,    225,    5,    
239,    6,    7,    8,    233,    9,    5,    6,    160,    172,    
173,    244,    233,    1,    2,    225,    229,    3,    155,    4,    
17,    160,    191,    225,    226,    230,    237,    228,    233,    247,    
167,    1,    2,    187,    3,    4,    236,    5,    155,    238,    
6,    239,    7,    172,    229,    243,    8,    9,    10,    174,    
11,    12,    13,    14,    15,    16,    6,    7,    160,    174,    
225,    229,    236,    250,    155,    239,    233,    1,    2,    3,    
4,    5,    155,    155,    155,    155,    155,    155,    155,    155,    
155,    155,    
};
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
