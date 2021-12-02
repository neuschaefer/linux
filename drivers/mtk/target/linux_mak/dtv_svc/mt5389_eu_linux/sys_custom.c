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
 * $RCSfile: sys_custom.c,v $
 * $Revision: #3 $
 * $Date: 2012/05/31 $
 * $Author: jifeng.fan $
 *
 * Description: 
 *         This header file contains specific implementation.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "sys_custom.h"

#include "u_gl.h"
#include "u_gl_nc.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#if defined(APP_COLOR_MODE_PALLET)

static GL_COLOR_T _at_default_clut[] = 
{
    /* 0xff, {0xfc}, {0x2b}, {0xff},*/ /* CLUT entry   0 */
    0x00, {0x00}, {0x00}, {0x00}, /* CLUT entry   0 */
    0xff, {0x00}, {0x00}, {0x00}, /* CLUT entry   1 */
    0xff, {0x00}, {0x00}, {0x55}, /* CLUT entry   2 */
    0xff, {0x00}, {0x00}, {0xaa}, /* CLUT entry   3 */
    0xff, {0x00}, {0x00}, {0xff}, /* CLUT entry   4 */
    0xff, {0x00}, {0x55}, {0x00}, /* CLUT entry   5 */
    0xff, {0x00}, {0x55}, {0x55}, /* CLUT entry   6 */
    0xff, {0x00}, {0x55}, {0xaa}, /* CLUT entry   7 */
    0xff, {0x00}, {0x55}, {0xff}, /* CLUT entry   8 */
    0xff, {0x00}, {0xaa}, {0x00}, /* CLUT entry   9 */
    0xff, {0x00}, {0xaa}, {0x55}, /* CLUT entry  10 */
    0xff, {0x00}, {0xaa}, {0xaa}, /* CLUT entry  11 */
    0xff, {0x00}, {0xaa}, {0xff}, /* CLUT entry  12 */
    0xff, {0x00}, {0xff}, {0x00}, /* CLUT entry  13 */
    0xff, {0x00}, {0xff}, {0x55}, /* CLUT entry  14 */
    0xff, {0x00}, {0xff}, {0xaa}, /* CLUT entry  15 */
    0xff, {0x00}, {0xff}, {0xff}, /* CLUT entry  16 */
    0xff, {0x55}, {0x00}, {0x00}, /* CLUT entry  17 */
    0xff, {0x55}, {0x00}, {0x55}, /* CLUT entry  18 */
    0xff, {0x55}, {0x00}, {0xaa}, /* CLUT entry  19 */
    0xff, {0x55}, {0x00}, {0xff}, /* CLUT entry  20 */
    0xff, {0x55}, {0x55}, {0x00}, /* CLUT entry  21 */
    0xff, {0x55}, {0x55}, {0x55}, /* CLUT entry  22 */
    0xff, {0x55}, {0x55}, {0xaa}, /* CLUT entry  23 */
    0xff, {0x55}, {0x55}, {0xff}, /* CLUT entry  24 */
    0xff, {0x55}, {0xaa}, {0x00}, /* CLUT entry  25 */
    0xff, {0x55}, {0xaa}, {0x55}, /* CLUT entry  26 */
    0xff, {0x55}, {0xaa}, {0xaa}, /* CLUT entry  27 */
    0xff, {0x55}, {0xaa}, {0xff}, /* CLUT entry  28 */
    0xff, {0x55}, {0xff}, {0x00}, /* CLUT entry  29 */
    0xff, {0x55}, {0xff}, {0x55}, /* CLUT entry  30 */
    0xff, {0x55}, {0xff}, {0xaa}, /* CLUT entry  31 */
    0xff, {0x55}, {0xff}, {0xff}, /* CLUT entry  32 */
    0xff, {0xaa}, {0x00}, {0x00}, /* CLUT entry  33 */
    0xff, {0xaa}, {0x00}, {0x55}, /* CLUT entry  34 */
    0xff, {0xaa}, {0x00}, {0xaa}, /* CLUT entry  35 */
    0xff, {0xaa}, {0x00}, {0xff}, /* CLUT entry  36 */
    0xff, {0xaa}, {0x55}, {0x00}, /* CLUT entry  37 */
    0xff, {0xaa}, {0x55}, {0x55}, /* CLUT entry  38 */
    0xff, {0xaa}, {0x55}, {0xaa}, /* CLUT entry  39 */
    0xff, {0xaa}, {0x55}, {0xff}, /* CLUT entry  40 */
    0xff, {0xaa}, {0xaa}, {0x00}, /* CLUT entry  41 */
    0xff, {0xaa}, {0xaa}, {0x55}, /* CLUT entry  42 */
    0xff, {0xaa}, {0xaa}, {0xaa}, /* CLUT entry  43 */
    0xff, {0xaa}, {0xaa}, {0xff}, /* CLUT entry  44 */
    0xff, {0xaa}, {0xff}, {0x00}, /* CLUT entry  45 */
    0xff, {0xaa}, {0xff}, {0x55}, /* CLUT entry  46 */
    0xff, {0xaa}, {0xff}, {0xaa}, /* CLUT entry  47 */
    0xff, {0xaa}, {0xff}, {0xff}, /* CLUT entry  48 */
    0xff, {0xff}, {0x00}, {0x00}, /* CLUT entry  49 */
    0xff, {0xff}, {0x00}, {0x55}, /* CLUT entry  50 */
    0xff, {0xff}, {0x00}, {0xaa}, /* CLUT entry  51 */
    0xff, {0xff}, {0x00}, {0xff}, /* CLUT entry  52 */
    0xff, {0xff}, {0x55}, {0x00}, /* CLUT entry  53 */
    0xff, {0xff}, {0x55}, {0x55}, /* CLUT entry  54 */
    0xff, {0xff}, {0x55}, {0xaa}, /* CLUT entry  55 */
    0xff, {0xff}, {0x55}, {0xff}, /* CLUT entry  56 */
    0xff, {0xff}, {0xaa}, {0x00}, /* CLUT entry  57 */
    0xff, {0xff}, {0xaa}, {0x55}, /* CLUT entry  58 */
    0xff, {0xff}, {0xaa}, {0xaa}, /* CLUT entry  59 */
    0xff, {0xff}, {0xaa}, {0xff}, /* CLUT entry  60 */
    0xff, {0xff}, {0xff}, {0x00}, /* CLUT entry  61 */
    0xff, {0xff}, {0xff}, {0x55}, /* CLUT entry  62 */
    0xff, {0xff}, {0xff}, {0xaa}, /* CLUT entry  63 */
    0xff, {0xff}, {0xff}, {0xff}, /* CLUT entry  64 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  65 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  66 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  67 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  68 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  69 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  70 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  71 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  72 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  73 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  74 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  75 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  76 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  77 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  78 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  79 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  80 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  81 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  82 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  83 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  84 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  85 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  86 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  87 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  88 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  89 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  90 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  91 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  92 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  93 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  94 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  95 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  96 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  97 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  98 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry  99 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 100 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 101 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 102 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 103 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 104 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 105 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 106 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 107 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 108 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 109 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 110 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 111 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 112 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 113 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 114 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 115 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 116 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 117 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 118 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 119 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 120 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 121 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 122 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 123 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 124 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 125 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 126 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 127 */
    0xff, {0xd0}, {0x07}, {0x91}, /* CLUT entry 128 */
    0xff, {0x0c}, {0x04}, {0x05}, /* CLUT entry 129 */
    0xff, {0x14}, {0x0c}, {0x14}, /* CLUT entry 130 */
    0xff, {0x04}, {0x04}, {0x10}, /* CLUT entry 131 */
    0xff, {0x34}, {0x34}, {0xcc}, /* CLUT entry 132 */
    0xff, {0x0b}, {0x0c}, {0x23}, /* CLUT entry 133 */
    0xff, {0x04}, {0x11}, {0x6c}, /* CLUT entry 134 */
    0xff, {0x04}, {0x0c}, {0x47}, /* CLUT entry 135 */
    0xff, {0x14}, {0x1c}, {0x54}, /* CLUT entry 136 */
    0xff, {0x14}, {0x19}, {0x3b}, /* CLUT entry 137 */
    0xff, {0x24}, {0x2c}, {0x64}, /* CLUT entry 138 */
    0xff, {0x2c}, {0x34}, {0x74}, /* CLUT entry 139 */
    0xff, {0x14}, {0x1c}, {0x4c}, /* CLUT entry 140 */
    0xff, {0x14}, {0x1c}, {0x4c}, /* CLUT entry 141 */
    0xff, {0x0c}, {0x14}, {0x3c}, /* CLUT entry 142 */
    0xff, {0x0d}, {0x14}, {0x34}, /* CLUT entry 143 */
    0xff, {0xe2}, {0xe3}, {0xe8}, /* CLUT entry 144 */
    0xff, {0x04}, {0x1c}, {0x80}, /* CLUT entry 145 */
    0xff, {0x24}, {0x34}, {0x7d}, /* CLUT entry 146 */
    0xff, {0x24}, {0x34}, {0x74}, /* CLUT entry 147 */
    0xff, {0x2c}, {0x3c}, {0x7c}, /* CLUT entry 148 */
    0xff, {0x0c}, {0x1c}, {0x54}, /* CLUT entry 149 */
    0xff, {0x0c}, {0x1c}, {0x54}, /* CLUT entry 150 */
    0xff, {0x1c}, {0x2c}, {0x64}, /* CLUT entry 151 */
    0xff, {0x04}, {0x27}, {0x91}, /* CLUT entry 152 */
    0xff, {0x24}, {0x34}, {0x66}, /* CLUT entry 153 */
    0xff, {0x89}, {0x92}, {0xb0}, /* CLUT entry 154 */
    0xff, {0x04}, {0x14}, {0x44}, /* CLUT entry 155 */
    0xff, {0x0c}, {0x1c}, {0x4c}, /* CLUT entry 156 */
    0xff, {0x24}, {0x3c}, {0x7c}, /* CLUT entry 157 */
    0xff, {0x2c}, {0x44}, {0x84}, /* CLUT entry 158 */
    0xff, {0x3c}, {0x54}, {0x9c}, /* CLUT entry 159 */
    0xff, {0x2e}, {0x3f}, {0x6f}, /* CLUT entry 160 */
    0xff, {0xa5}, {0xad}, {0xc3}, /* CLUT entry 161 */
    0xff, {0xb7}, {0xbd}, {0xce}, /* CLUT entry 162 */
    0xff, {0x04}, {0x14}, {0x3c}, /* CLUT entry 163 */
    0xff, {0x1c}, {0x34}, {0x70}, /* CLUT entry 164 */
    0xff, {0x34}, {0x4c}, {0x8a}, /* CLUT entry 165 */
    0xff, {0x04}, {0x1c}, {0x50}, /* CLUT entry 166 */
    0xff, {0x0e}, {0x45}, {0xbf}, /* CLUT entry 167 */
    0xff, {0x0a}, {0x30}, {0x85}, /* CLUT entry 168 */
    0xff, {0x15}, {0x2a}, {0x59}, /* CLUT entry 169 */
    0xff, {0x24}, {0x3c}, {0x74}, /* CLUT entry 170 */
    0xff, {0x17}, {0x25}, {0x44}, /* CLUT entry 171 */
    0xff, {0x2f}, {0x46}, {0x7b}, /* CLUT entry 172 */
    0xff, {0x3d}, {0x55}, {0x8a}, /* CLUT entry 173 */
    0xff, {0x45}, {0x5e}, {0x96}, /* CLUT entry 174 */
    0xff, {0x56}, {0x6c}, {0x9b}, /* CLUT entry 175 */
    0xff, {0x3d}, {0x4c}, {0x6c}, /* CLUT entry 176 */
    0xff, {0xc8}, {0xce}, {0xdc}, /* CLUT entry 177 */
    0xff, {0x0a}, {0x3a}, {0x9d}, /* CLUT entry 178 */
    0xff, {0x07}, {0x27}, {0x67}, /* CLUT entry 179 */
    0xff, {0x23}, {0x53}, {0xb3}, /* CLUT entry 180 */
    0xff, {0x0c}, {0x1c}, {0x3c}, /* CLUT entry 181 */
    0xff, {0x1c}, {0x34}, {0x64}, /* CLUT entry 182 */
    0xff, {0x0c}, {0x14}, {0x24}, /* CLUT entry 183 */
    0xff, {0x14}, {0x15}, {0x17}, /* CLUT entry 184 */
    0xff, {0x04}, {0x14}, {0x32}, /* CLUT entry 185 */
    0xff, {0x1c}, {0x3c}, {0x74}, /* CLUT entry 186 */
    0xff, {0x04}, {0x1c}, {0x44}, /* CLUT entry 187 */
    0xff, {0x0e}, {0x45}, {0xa3}, /* CLUT entry 188 */
    0xff, {0x14}, {0x58}, {0xc8}, /* CLUT entry 189 */
    0xff, {0x5d}, {0x7e}, {0xb4}, /* CLUT entry 190 */
    0xff, {0x0c}, {0x1c}, {0x34}, /* CLUT entry 191 */
    0xff, {0x0c}, {0x1c}, {0x34}, /* CLUT entry 192 */
    0xff, {0x1c}, {0x3c}, {0x6b}, /* CLUT entry 193 */
    0xff, {0x26}, {0x3a}, {0x59}, /* CLUT entry 194 */
    0xff, {0x9f}, {0xc3}, {0xf9}, /* CLUT entry 195 */
    0xff, {0x1d}, {0x21}, {0x27}, /* CLUT entry 196 */
    0xff, {0x3f}, {0x84}, {0xde}, /* CLUT entry 197 */
    0xff, {0x77}, {0x9f}, {0xd5}, /* CLUT entry 198 */
    0xff, {0x09}, {0x39}, {0x72}, /* CLUT entry 199 */
    0xff, {0x14}, {0x68}, {0xd0}, /* CLUT entry 200 */
    0xff, {0x04}, {0x0c}, {0x15}, /* CLUT entry 201 */
    0xff, {0x82}, {0xae}, {0xe0}, /* CLUT entry 202 */
    0xff, {0x94}, {0xc4}, {0xfc}, /* CLUT entry 203 */
    0xff, {0xac}, {0xc5}, {0xe0}, /* CLUT entry 204 */
    0xff, {0x2c}, {0x91}, {0xf4}, /* CLUT entry 205 */
    0xff, {0x23}, {0x5a}, {0x90}, /* CLUT entry 206 */
    0xff, {0x56}, {0xa3}, {0xf3}, /* CLUT entry 207 */
    0xff, {0x8b}, {0xbf}, {0xf4}, /* CLUT entry 208 */
    0xff, {0x8b}, {0xba}, {0xeb}, /* CLUT entry 209 */
    0xff, {0x94}, {0xc4}, {0xf4}, /* CLUT entry 210 */
    0xff, {0x9c}, {0xcc}, {0xfc}, /* CLUT entry 211 */
    0xff, {0x4c}, {0x4f}, {0x52}, /* CLUT entry 212 */
    0xff, {0x12}, {0x34}, {0x54}, /* CLUT entry 213 */
    0xff, {0x0b}, {0x14}, {0x1c}, /* CLUT entry 214 */
    0xff, {0x94}, {0xcc}, {0xfc}, /* CLUT entry 215 */
    0xff, {0x94}, {0xc4}, {0xec}, /* CLUT entry 216 */
    0xff, {0xba}, {0xdc}, {0xf8}, /* CLUT entry 217 */
    0xff, {0x05}, {0x1c}, {0x2c}, /* CLUT entry 218 */
    0xff, {0x94}, {0xcd}, {0xf4}, /* CLUT entry 219 */
    0xff, {0xf1}, {0xf4}, {0xf6}, /* CLUT entry 220 */
    0xff, {0xd8}, {0xee}, {0xf9}, /* CLUT entry 221 */
    0xff, {0x05}, {0x84}, {0x04}, /* CLUT entry 222 */
    0xff, {0x6f}, {0xd4}, {0x6c}, /* CLUT entry 223 */
    0xff, {0x42}, {0xb2}, {0x34}, /* CLUT entry 224 */
    0xff, {0x0f}, {0x14}, {0x0c}, /* CLUT entry 225 */
    0xff, {0x0c}, {0x0c}, {0x06}, /* CLUT entry 226 */
    0xff, {0x6a}, {0x69}, {0x57}, /* CLUT entry 227 */
    0xff, {0xfb}, {0xe1}, {0x0c}, /* CLUT entry 228 */
    0xff, {0xb4}, {0xb3}, {0xaa}, /* CLUT entry 229 */
    0xff, {0xf0}, {0xec}, {0xdc}, /* CLUT entry 230 */
    0xff, {0xa0}, {0x91}, {0x5b}, /* CLUT entry 231 */
    0xff, {0xac}, {0x88}, {0x15}, /* CLUT entry 232 */
    0xff, {0xc7}, {0x94}, {0x0a}, /* CLUT entry 233 */
    0xff, {0xc7}, {0x9b}, {0x21}, /* CLUT entry 234 */
    0xff, {0xd3}, {0xa7}, {0x2e}, /* CLUT entry 235 */
    0xff, {0xc4}, {0x8c}, {0x05}, /* CLUT entry 236 */
    0xff, {0xd3}, {0x9c}, {0x13}, /* CLUT entry 237 */
    0xff, {0x41}, {0x30}, {0x07}, /* CLUT entry 238 */
    0xff, {0xdf}, {0xbb}, {0x61}, /* CLUT entry 239 */
    0xff, {0xec}, {0xd8}, {0xa4}, /* CLUT entry 240 */
    0xff, {0xfa}, {0xad}, {0x04}, /* CLUT entry 241 */
    0xff, {0x2e}, {0x21}, {0x04}, /* CLUT entry 242 */
    0xff, {0x77}, {0x56}, {0x0b}, /* CLUT entry 243 */
    0xff, {0xdf}, {0xa4}, {0x1c}, /* CLUT entry 244 */
    0xff, {0xea}, {0xac}, {0x25}, /* CLUT entry 245 */
    0xff, {0x8a}, {0x6b}, {0x22}, /* CLUT entry 246 */
    0xff, {0xf1}, {0xc6}, {0x67}, /* CLUT entry 247 */
    0xff, {0x1c}, {0x14}, {0x04}, /* CLUT entry 248 */
    0xff, {0xfb}, {0x84}, {0x0c}, /* CLUT entry 249 */
    0xff, {0x14}, {0x0c}, {0x06}, /* CLUT entry 250 */
    0xff, {0xfc}, {0x84}, {0x32}, /* CLUT entry 251 */
    0xff, {0xfc}, {0x1c}, {0x1c}, /* CLUT entry 252 */
    0xff, {0x00}, {0x00}, {0x00}, /* CLUT entry 253 */
    0xff, {0xe9}, {0x88}, {0x45}, /* CLUT entry 254 */
    0xff, {0xf4}, {0xc3}, {0xa1}  /* CLUT entry 255 */
};

/* change defalut key to 0 of table */
static GL_COLOR_T _t_default_colorkey = {0, {0}, {0}, {0}};

#else
static GL_COLOR_T _t_default_colorkey;
static GL_COLOR_T _at_default_clut[] = {{0x0}, {0x0}, {0x0}, {0x0}};
#endif /* APP_COLOR_MODE_PALLET */

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

UINT32 sys_cust_get_screen_width( VOID )
{
    return SCREEN_WIDTH;
}

UINT32 sys_cust_get_screen_height( VOID )
{
    return SCREEN_HEIGHT;
}
#ifdef SYS_OAD_SUPPORT
UINT32 sys_get_oad_OUI(VOID)
{
    return CUST_OAD_OUI;
}

UINT16 sys_get_oad_hw_version(VOID)
{
    return CUST_OAD_HW_VERSION;
}

UINT16 sys_get_oad_hw_model(VOID)
{
    return CUST_OAD_HW_MODEL;
}

UINT16 sys_get_oad_sw_model(VOID)
{
    return CUST_OAD_SW_MODEL;
}

UINT16 sys_get_oad_sw_version(VOID)
{
    return CUST_OAD_SW_VERSION;
}
#endif /* SYS_OAD_SUPPORT */
VOID sys_cust_set_dvd_bundled_input_src( VOID ) 
{
    /* if need to configure one input source to be coupled with DVD 
     * please set its related ISL_REC_T here
     * e.g. set index = 0's input source to be DVD bundled
     *
     * #include "app_util/a_isl.h"
     *
     * ISL_REC_T t_isl_rec;
     * INT32     i4_ret;
     * 
     * i4_ret = a_isl_get_rec_by_idx(0, &t_isl_rec);
     * if (i4_ret == ISLR_OK) 
     * {
     *     t_isl_rec.ui4_attr_bits |= ISL_ATTR_BIT_DVD_BUNDLE;
     *     a_isl_set_attr_bits_by_idx(0, t_isl_rec.ui4_attr_bits); 
     * }      
     */
}

UINT16 sys_cust_get_start_ch_num_for_non_lcn( VOID )
{
    return START_CH_NUM_FOR_NON_LCN; 
}

UINT16 sys_cust_get_ci_sw_version( VOID )
{
    return CI_SW_VERSION;
}

GL_COLORMODE_T sys_cust_get_color_mode(VOID)
{
    return COLOR_MODE;
}

CHAR* sys_cust_get_usb_upg_version(VOID)
{
    return USB_UPG_VERSION;
}

BOOL sys_cust_enable_ttx_share_buf(VOID)
{
#ifdef TTX_SHARE_BUFFER
    return TRUE;
#else
    return FALSE;
#endif
}

UINT8 sys_cust_get_is_bdf_font(VOID)
{
    #ifdef APP_USE_BDF_FONT
    return 1;
    #else
    return 0;
    #endif /* #endif APP_USE_BDF_FONT */
}

/*
Removed due to dynamically initializing palette mode or 4444 mode
#ifdef APP_COLOR_MODE_PALLET
*/
VOID sys_cust_get_default_clut(GL_COLOR_T** ppt_clut, UINT16* pui2_size)
{
    *ppt_clut  = (GL_COLOR_T*) _at_default_clut;
    *pui2_size = sizeof(_at_default_clut) / sizeof(GL_COLOR_T);
}

GL_COLOR_T* sys_cust_get_default_colorkey(VOID)
{
    return (&_t_default_colorkey);
}

UINT8 sys_cust_get_ttx_sbtl_plt_range_start(VOID)
{
    return TTX_SBTL_PLT_RANGE_START;
}

UINT8 sys_cust_get_ttx_sbtl_plt_range_end(VOID)
{
    return TTX_SBTL_PLT_RANGE_END;
}
/*
Removed due to dynamically initializing palette mode or 4444 mode
#endif  APP_COLOR_MODE_PALLET 
*/

CHAR*  sys_cust_get_cust_model( VOID )
{
    return MODEL_NAME;
}

CHAR*  sys_cust_get_cust_serial( VOID )
{
    return SERIAL_NUMBER;
}

#ifdef CI_PLUS_SUPPORT
static VOID* _memcpy(
    VOID*   pui1_dst,
    VOID*   pui1_src,
    SIZE_T  z_len)
{
    SIZE_T z_i;

    if ( (!pui1_dst) || (!pui1_src))
    {
        return NULL;
    }

    for( z_i = 0; z_i<z_len; ++z_i )
    {
        ((UINT8*)pui1_dst)[z_i] = ((UINT8*)pui1_src)[z_i];
    }

    return pui1_src;
}

static CHAR cust_key[CI_AES_128_CBC_KEY_SIZE+1] = "9f966becfdd211c1";
static CHAR cust_iv[CI_AES_128_CBC_IV_SIZE+1] = "c6e577b26a8500c7";
VOID sys_cust_get_fix_key(CHAR* key, CHAR* iv)
{
    UINT8 ui1_i;
    for (ui1_i=0; ui1_i<CI_AES_128_CBC_KEY_SIZE; ui1_i++ )
    {
        key[ui1_i] = cust_key[ui1_i];
    }
    for (ui1_i=0; ui1_i<CI_AES_128_CBC_IV_SIZE; ui1_i++ )
    {
        iv[ui1_i] = cust_iv[ui1_i];
    }    
    key[CI_AES_128_CBC_KEY_SIZE] = '\0';
    iv[CI_AES_128_CBC_IV_SIZE] = '\0';
}

BOOL sys_cust_get_fix_encrypt_mode(
    BOOL  *b_fix_encrypt,
    UINT8 *ui1_encrypt_mode,
    UINT8 *ui1_key_len,
    UINT8 *ui1_iv_len)
{
    *b_fix_encrypt = CI_FIX_ENCRYPT;
    *ui1_encrypt_mode = CI_ENCRYPT_MODE;
    *ui1_key_len = CI_AES_128_CBC_KEY_SIZE;
    *ui1_iv_len = CI_AES_128_CBC_IV_SIZE;
    if ((*b_fix_encrypt != TRUE)||
        (*ui1_encrypt_mode != 0x01))
    {
        return FALSE;
    }
    return TRUE;
}

VOID sys_cust_get_custom_code_len(
        BOOL  *b_custom_code,
        UINT8 *ui1_custom_code_len,
        UINT8 *ui1_max_custom_code_len)
{
    *b_custom_code = CI_CUSTOM_CODE_SET;
    *ui1_custom_code_len = CI_CUSTOM_CODE_SIZE;
    *ui1_max_custom_code_len = CI_MAX_CUSTOM_CODE_SIZE;
}

BOOL sys_cust_get_custom_code_info(
        UINT8 ui1_custom_code_len,
        CHAR  *uc_custom_code)
{
    if ((sizeof(CI_CUSTOM_CODE) != (CI_CUSTOM_CODE_SIZE+1))||
        (ui1_custom_code_len != CI_CUSTOM_CODE_SIZE) ||
        (uc_custom_code == NULL))
    {
        return FALSE;
    }
    _memcpy(uc_custom_code, CI_CUSTOM_CODE, sizeof(CI_CUSTOM_CODE));
    return TRUE;
}

BOOL sys_cust_get_search_mode_pattern_len(
        UINT8 *ui1_search_mode, 
        UINT8 *ui1_max_search_len,
        UINT8 *ui1_pattern1_len,
        UINT8 *ui1_pattern2_len)
{
    *ui1_search_mode = CI_SEARCH_MODE;
    *ui1_max_search_len = CI_MAX_SEARCH_PATTERN_SIZE;
   
    switch(*ui1_search_mode)
    {
        case 0x01:
            *ui1_pattern1_len = CI_SEARCH_FULL_NAME_SIZE;
            *ui1_pattern2_len = 0;
            break;
            
        case 0x02:
            *ui1_pattern1_len = CI_SEARCH_PREFIX_EXT_P_SIZE;
            *ui1_pattern2_len = CI_SEARCH_PREFIX_EXT_E_SIZE;
            break;
            
        case 0x03:
            *ui1_pattern1_len= CI_SEARCH_PREFIX_SIZE;
            *ui1_pattern2_len = 0;
            break;
            
        case 0x04:
            *ui1_pattern1_len = CI_SEARCH_EXTENSION_SIZE;
            *ui1_pattern2_len = 0;
            break;
            
        default:
            return FALSE;
    }
    return TRUE;
}

BOOL sys_cust_get_search_mode_pattern_info(
        UINT8 ui1_search_mode, 
        UINT8 ui1_pattern1_len, 
        CHAR *uc_pattern1,
        UINT8 ui1_pattern2_len,
        CHAR *uc_pattern2)
{
    switch(ui1_search_mode)
    {
        case 0x01:
            if ((sizeof(CI_SEARCH_FULL_NAME)!= (CI_SEARCH_FULL_NAME_SIZE+1)) ||
                (ui1_pattern1_len != CI_SEARCH_FULL_NAME_SIZE) ||
                (ui1_pattern2_len != 0) ||
                (uc_pattern1 == NULL))
            {
                return FALSE;
            }
            _memcpy(uc_pattern1, CI_SEARCH_FULL_NAME, sizeof(CI_SEARCH_FULL_NAME));
            break;
            
        case 0x02:
            if ((sizeof(CI_SEARCH_PREFIX_EXT_P) != (CI_SEARCH_PREFIX_EXT_P_SIZE+1)) ||
                (ui1_pattern1_len != CI_SEARCH_PREFIX_EXT_P_SIZE) ||
                (uc_pattern1 == NULL))
            {
                return FALSE;
            }
            if ((sizeof(CI_SEARCH_PREFIX_EXT_E) != (CI_SEARCH_PREFIX_EXT_E_SIZE+1)) ||
                (ui1_pattern2_len != CI_SEARCH_PREFIX_EXT_E_SIZE) ||
                (uc_pattern2 == NULL))
            {
                return FALSE;
            }            
            _memcpy(uc_pattern1, CI_SEARCH_PREFIX_EXT_P, sizeof(CI_SEARCH_PREFIX_EXT_P));
            _memcpy(uc_pattern2, CI_SEARCH_PREFIX_EXT_E, sizeof(CI_SEARCH_PREFIX_EXT_E));
            break;
            
        case 0x03:
            if ((sizeof(CI_SEARCH_PREFIX) != (CI_SEARCH_PREFIX_SIZE+1)) ||
                (ui1_pattern1_len != CI_SEARCH_PREFIX_SIZE) ||
                (ui1_pattern2_len != 0)||
                (uc_pattern1 == NULL))
            {
                return FALSE;
            }
            _memcpy(uc_pattern1, CI_SEARCH_PREFIX, sizeof(CI_SEARCH_PREFIX));
            break;
            
        case 0x04:
            if ((sizeof(CI_SEARCH_EXTENSION) != (CI_SEARCH_EXTENSION_SIZE+1)) ||
                (ui1_pattern1_len != CI_SEARCH_EXTENSION_SIZE) ||
                (ui1_pattern2_len != 0) ||
                (uc_pattern1 == NULL))
            {
                return FALSE;
            }
            _memcpy(uc_pattern1, CI_SEARCH_EXTENSION, sizeof(CI_SEARCH_EXTENSION));            
            break;
            
        default:
            return FALSE;
    }
    return TRUE;
}

VOID sys_cust_get_rename_prefix_len(
        UINT8 *ui1_rename_prefix_len, 
        UINT8 *ui1_max_rename_prefix_len)
{
    *ui1_rename_prefix_len = CI_RENAME_PREFIX_SIZE;
    *ui1_max_rename_prefix_len = CI_MAX_RENAME_PREFIX_SIZE;
}

BOOL sys_cust_get_rename_prefix_info(
        UINT8 ui1_rename_prefix_len, 
        CHAR *uc_rename_prefix)
{
    if ((sizeof(CI_RENAME_PREFIX) != (CI_RENAME_PREFIX_SIZE+1))||
        (ui1_rename_prefix_len != CI_RENAME_PREFIX_SIZE) ||
        (uc_rename_prefix == NULL))
    {
        return FALSE;
    }
    _memcpy(uc_rename_prefix, CI_RENAME_PREFIX, sizeof(CI_RENAME_PREFIX));
    return TRUE;
}

VOID sys_cust_get_max_serial_num_len(UINT8 *ui1_serial_num_len)
{
    *ui1_serial_num_len = CI_MAX_SERIAL_NUM_SIZE;
}
#endif


