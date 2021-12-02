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

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_drv_cli.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_timer.h"
#include "x_assert.h"
#include "fbm_drvif.h"
#include "drvcust_if.h"

#include "jpg_if.h"
#include "img_lib_if.h"

#include "x_dram.h"
#include "srm_drvif.h"
#include "x_dram.h"
#include "x_ckgen.h"

#ifdef JPEG_AUTO_TEST
#define CC_EMULATION
#endif

#if defined (CC_EMULATION) || defined (__MODEL_slt__)
#include "drv_t32.h"

#ifdef JPEG_AUTO_TEST

#ifdef JPEG_DRAM_ACCESS_BYTE_MEASUREMENT
typedef struct
{
    UINT32 u4Reserved                       : 8;
    UINT32 u4Bmgp1ag                        : 5;
    UINT32                                  : 2;
    UINT32 fgBmbp1                          : 1;
    UINT32 u4Bmgp2ag                        : 3;
    UINT32 fgBmbp2                          : 1;
    UINT32 u4Bmgp3ag                        : 3;
    UINT32 fgBmbp3                          : 1;
    UINT32 fgReadOnly                       : 1;
    UINT32 fgWriteOnly                      : 1;
    UINT32                                  : 2;
    UINT32 fgFreeze                         : 1;
    UINT32 fgDynPrioEn                      : 1;
    UINT32 fgReqAle                         : 1;
    UINT32 fgPreAct                         : 1;
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed))  HAL_DRAMC_T;
#endif

#ifdef JPEG_AUTO_TEST_PERFORMANCE
extern UINT32 mh_width;
extern UINT32 mh_height;
#endif

//#include "auto_test.h"
#ifndef __MODEL_slt__
#define PICTURE_BUFFER_SIZE  (1024*1024*10)
UINT32 pAutoRunCheckSum[]=
{
0x3383f48d,0x962c5bfa,0x3383f48d,0x9aea43a7,0x64cbd2da,0x1a1a1598,0xbe8e75e5,0x66c7034f,0x72c56ab3,0x78a1f8a,0x8958182,
0xecd752ea,0x9db3acfc,0xddf26871,0x62ff77aa,0xc44aec80,0xd7e752d8,0xc9819598,0xf0cbeaf7,0x7d359ac0,0x8dfc0d13,
0xe6afe702,0x78711193,0x4b2f600d,0x2900a88f,0x9ad3e16f,0xb8bf7d4b,0x5e1c6f00,0x8af87aea,0x99eff9d,0xaf05259b,
0x6430b5c7,0x61502eca,0x44031592,0x76246146,0x5bd49fd0,0x7c1a0b41,0x9c74db83,0xd354e313,0x7d01cf10,0xdef6d2a3,
0x39b7e133,0x999db209,0x69d57ee3,0x3688e524,0x4fcbce76,0xb6745e63,0xd3468f70,0x22c97846,0x2f175500,0x5d19988a,
0x66d9a807,0x61c65d95,0xf27f2d66,0xeb67102,0x555f1c50,0x35377cb9,0x6df18adc,0x6a939f46,0xf4ea6e3d,0xdd5ab4f,
0xfa489c2e,0x1c710c13,0x33911a83,0x9134cc6a,0x3bd819f8,0x3fa29608,0x1c723c5a,0xdaffc1ec,0xf1efd53b,0x33a1d302,
0x37b974c3,0x85819fa1,0x64b2c23b,0x9f246512,0x7dbdaf99,0xf61f9775,0xb848a641,0x82c93c5e,0xa20cea71,0x6fad691c,
0xdc18a44a,0x803c0b4e,0x152b6f6f,0xb29a3ed3,0x882fea3f,0x797ee47b,0xcfd3b65d,0xa2ea059c,0xd81705a2,0xd896b378,
0x17fce32,0xd8449fb3,0xa77490af,0xefa6e257,0xfff23166,0x642149d1,0x330b7f58,0x9b18ab9a,0xb3cc5cf1,0xff09de57,
0xc8650259,0xd4586bcb,0x18acfd0c,0xcc115200,0xc6fad0bc,0x2477ede5,0xdc779023,0xa5b177f4,0x17e2383d,0xca52a8a0,
0x427d922c,0x9a61f91b,0xc3b74817,0x7f296552,0x91c9c28e,0xda3bbfe,0x938d3cda,0x3e9b4f44,0xdca84af3,0xe772d2a8,
0x42c2019c,0x4c9f90ad,0xe1637dd5,0xf2b7baeb,0x16b84dd2,0x6a512de8,0x28ef459d,0xd3a3c826,0x7e3cf85b,0x70384c38,
0x178c13a3,0x5b8617b7,0xf0bb65fd,0x44565c24,0x1f871882,0x9fcba2bc,0xf87d3f64,0xf353b7af,0x38bb5ea5,0x14748a0f,
0xa6eb1a15,0xabb030e3,0xf0ab823b,0xe78b1f61,0xcffd68aa,0xf794f966,0x6f30f691,0x4f653515,0xb66c984d,0x63121,
0x6f96affc,0xaa39059,0x1ee540ae,0xfdda9435,0x8fac3f3c,0xd71d0467,0xe4ca1058,0x1034492f,0x563b669b,0x10379893,
0xc5abb3e3,0xd69b9707,0xcdc0af1a,0x289be1e7,0xf786af37,0x5fe93fb4,0x2ac5684d,0x67f64b89,0xebac58f,0x84d78556,
0xa9af6004,0xa60b70dd,0x7325f8f2,0x602c5f60,0x5754475a,0xd34a5667,0x8addfd14,0x864f69cd,0xafcd6d11,0x5a66d98a,
0x835ac1f3,0x63f651e5,0x6db83d96,0x4a6a4fd7,0x2b679f1,0x6f77c15a,0xb8d6f7e1,0xc9df3c4e,0x8b4cf01d,0x9051c510,
0x9f0115c4,0x7ff3aa52,0x8072078f,0xc90888b9,0x4461b6b6,0x3b40881c,0x47f829bf,0x836bf63f,0x4b4c456a,0x46ed0f91,
0xfbb57866,0x7f3e927,0x1d621df1,0x507186a1,0x9099e599,0x91dc6fd,0x663f6b4,0x5ec3407a,0x3f0e3971,0xe91147ae,
0xd3c7e070,0x8bb3e2c1,0x604e2cad,0x32116d26,0x6993d844,0x2da85200,0x66a89961,0xdc4e68c8,0xcf0f2c9b,0x3dc1f8c4,
0x96311d64,0xdceab61a,0xd0fce9af,0x869271cd,0x790f028f,0x8bce7616,0xc7dcd81e,0xb79ba329,0xce5945cf,0xcd7e494f,
0xe8d75ce0,0x455490a0,0x3dab6dbf,0xb0b5f72a,0xe3778b2b,0x335334c2,0xab8700b7,0xf4486d32,0x5bb705b2,0x7e513810,
0xe6ae80dc,0x3457cd95,0xd4462757,0xf025a507,0xc586512,0xa53bbbfe,0xf2d995fa,0xfa5e17ba,0x2b0d027d,0x94aca2dc,
0x67ff842f,0x3d0b52e6,0xb213d76c,0x1fad174f,0x9320c7b,0x1e67f08a,0x28c43f1f,0xa3b4b3f7,0xfb90f4e2,0x8b8d0ad5,
0x2ceb286f,0x8795616d,0x357052d0,0x4ccacff9,0x2f326abe,0x898fa433,0xc6ef1bb9,0x82dbcbad,0x44700956,0x4e8b6b00,
0x62a68feb,0x4f31ed65,0x1ada113a,0xf40b3c05,0xf5eaa316,0xde660022,0x6646aa5e,0xf8ea54a,0x8f65737e,0xf6bcd7e7,
0x6a39337a,0x54366fa7,0x9419c57d,0x6287b798,0xbd129a1f,0x6d6cbd47,0x4c9bcc21,0x27719f74,0x898db4c2,0x10e2f3dc,
0x9e32f45e,0x869574df,0x80dce4c8,0xc986f1b9,0x84b24a9a,0x6579e91c,0x4527b524,0x28b41eee,0x7889a8bf,0x302ec3e0,
0x30ae7e75,0x627f0fe9,0x5c275470,0xa2456bc5,0x4a65dc3a,0x34c95167,0x29986a44,0x91799037,0x26cd7254,0x85f1a22a,
0xf145bb1a,0xa46d0a7e,0xd51d422e,0xcf11dbbd,0x232f9a5e,0xb77dc15e,0x2335eb02,0xe81b7281,0xb72a4929,0x9749b50e,
0x580265ea,0x313f8b03,0xa2aa30d3,0xc1a7411e,0xba5f01a4,0x846546ea,0x1c65285f,0x8cef7955,0xdaf5e067,0xfe10b8a7,
0x628e6d56,0x6ac23cbf,0x479042fe,0x80d20e9e,0x518d68e4,0x74d3530b,0xc0956f35,0x3d24b388,0x1154cafb,0xbbe3a2f9,
0x7e34fbd,0x23812a91,0xa6f0195a,0xd0add497,0x46169955,0x8d70008b,0x956dacc3,0x47b85d58,0x4679fbbd,0x39ff7d0a,
0x944cfff8,0x81a538d8,0x7b8170bc,0xa109d705,0x7652ac1d,0xe47ea484,0xc6591e3f,0x6b4b4d97,0xb8a0deb8,0x5765fd05,
0x54b909ff,0x411a302d,0xc5ca3103,0x68ac7026,0x30a1cb5c,0x630959fa,0x33c97c85,0x7fad3d1c,0xaca3519a,0xab707bef,
0x5fde5876,0x5a4771ff,0x5e755ce3,0xb8ddcd99,0x8ff9a693,0x533e9b3a,0xfc1413c4,0x11ce001,0x1fbe388e,0x57003ce3,
0x3266d24b,0xf4bb34d7,0xb677a84b,0x5df558ea,0x7dc32c53,0xaf38c8aa,0x5702040f,0xfea32029,0xdba5861a,0x32994b04,
0x47bca006,0xda5bc2f9,0xfee6011c,0x7ba3fc0,0x72ec2bf4,0xe7033766,0x1faeeb72,0x3d748cbc,0x8b052183,0x2d4a4d69,
0x2cc4db41,0x1fa9ee06,0x1e192c9,0x35ebc43f,0x6eaa2dfc,0xd9148690,0xa06caed0,0xf23984d5,0x9edb6f6c,0xf255f12f,
0xf82475d0,0x1aa8672c,0xa2f4bfac,0x690b4a9e,0xf6760b57,0x481f73c2,0xd401b2bc,0x93e22a7e,0xf8bd305e,0xd81d7fd6,
0x4f3cdbd6,0xe2a0c65e,0xe8db924a,0xfeecc699,0x2a692bb8,0xe2db3706,0xb17cd217,0x69203bee,0xde814e77,0x6e52e7f8,
0x68423d7e,0x7a508f9,0x336d2a6a,0x245052d6,0xbad8a980,0xbf5975aa,0x8aa2b502,0x4aa71048,0x95f6c254,0xab601768,
0xcdf038e7,0x835150d3,0xf8c0c998,0xdd176c80,0x8d14aa8,0xb9380316,0x31dd9944,0x61777909,0x214d1e6,0x147a58c4,
0x78dd6a18,0x42aa95de,0xfbe0c9f0,0x9f811018,0xe81f9367,0x1092d140,0x3c02e694,0x49ec1223,0xf0da000,0xdd9dea24,
0x656de680,0x2de27920,0xdf37957e,0x4a581064,0x5f9a6715,0xe067a032,0xc9cf33eb,0x83f225a8,0x693454b3,0xb8eccb9e,
0x522d817b,0xd9fa518c,0x7bd3585c,0x1a4173a0,0x420ff48b,0x64d8c67d,0x64c2eb5c,0x518f5a7e,0x1d74b4dc,0x8610129b,
0x677b7dba,0x9fdcaff8,0xa74127cc,0x888d7875,0x2d82ff78,0x37f320ed,0x8c052ddf,0x3a7e8c8f,0x7b7712fa,0x542e68f7,
0xb072bb9b,0x3648a60a,0xbb6bbc75,0x358d7525,0x9dbc85a7,0x6353d862,0x5e4f91da,0x14edc66,0xe843bb33,0x82782bb8,
0xaffac799,0x656ef8eb,0x62d6febe,0xed83b5b7,0xecd51c06,0x13696b0b,0x8f48e5f7,0xce7107e8,0x512b2512,0x944a8f0f,
0x47be1bfa,0x7a72222c,0x78642d1,0xf8468dfa,0x3214ec26,0x3bfb2efc,0x8ccd154,0x2c10b9cb,0xdfd33b7e,0x6ffd7ee,
0x8de6089f,0xc24b3bb3,0xca6fc31e,0x96351b2d,0x9ca7c614,0xa7bc7d5d,0x26bffd4f,0xc8a495d1,0x43d2cc08,0x3d97d2e9,
0x4ac55ac8,0x52989802,0xfac163be,0xcd174aba,0x83e3a90f,0x1f7f4a18,0xdf6b7c34,0x858edc40,0xd54d3498,0x5547fe3,
0xf6230ff9,0x8567660c,0x1de35856,0x263580d7,0x671a1e4a,0xec8945c9,0xd29f6b09,0x8b0a90bc,0xc2aa5bcd,0x1499c29f,
0xc8339b7c,0xda4ac70e,0x501a8cef,0x311ce932,0x470c2348,0x7263d105,0x9a24f829,0xff8bfe9b,0x8f1a50da,0x9dab8ae3,
0x39cfe6e8,0x891ab208,0x7716a979,0x834a790f,0xa42dc5e4,0x652fe5ae,0xd4cbd573,0xd6933708,0x37c64542,0xb654dd2b,
0x34cf9b9,0xe11a46a5,0x771f9f6e,0xfc35c4d4,0x86fe70,0x5ebbab9e,0x5a9551d2,0xbc3632fb,0x21ad763a,0xccc2eac8,
0xee240662,0x6311b809,0xe944de3a,0xb6305261,0x23bb8300,0x1ff36aa4,0xa2906895,0x30dca148,0x5c7e67e0,0x54aafc95,
0x8f2b0bec,0xbeed4277,0xad3ef9d5,0x33f2739a,0x65af79ac,0x1a445b99,0x49f70f2b,0xb87654ff,0x3a154b03,0x9210753,
0x7d216607,0xdea3b97c,0xef76eb3f,0x88018cd5,0x11952c34,0xd6aca9ee,0xba0c70b3,0xb2e6f43d,0x3a7282d1,0x50b9f09c,
0x7aa80c42,0x133c8bac,0xa490c559,0x1642f8b6,0xfbb20106,0xa2ee8163,0xf27b6250,0xa38fdc2d,0x8717039d,0xd030b6f1,
0xc070a7c8,0x12c6844c,0x75e4cb80,0x137d0433,0xb6ad9b26,0xe9c6d48c,0xecb8ae36,0xf54fb3e8,0xdfded7d1,0xf5fe74c4,
0xb6eb6ab9,0xc316ebd7,0xcad8cf88,0x25a33fb1,0x470ee411,0xdae3a366,0x6adc55,0x4b024c82,0x60697c2e,0x7cae465a,
0xf3bf139c,0x581eb459,0x3b375a2c,0x5c67f255,0xa897ebd7,0x9efc8028,0x7e9f1fa0,0x138b340,0x8cb1c9d4,0x944884e9,
0xb7b68a65,0xe726a5f3,0x22066fa3,0x898aa75e,0x57f9cf11,0x719c2e31,0xbf652fe1,0x53ecb37a,0xc7f1fbed,0x6a8dad06,
0xb4f3c441,0x3f603141,0xae20e6e,0x7775b0b3,0x11d7c991,0xc9c56b54,0x3defe7,0x717100e7,0x3cd3280f,0x5ca44efa,
0x301889c6,0xb7fa934e,0xf35f6b9f,0x16880e57,0x76199ddc,0x9e6aeac8,0x72da8768,0x10363180,0xae281745,0xa9ed0cb8,
0xad6d8336,0x655d328e,0x13f933ae,0xd3d417c3,0x2d3203fd,0x24fdb523,0xcc35de8d,0x979317b6,0xe9b3779a,0xc00410ca,
0x9073f925,0x128da60e,0xa93321ac,0x694e31bb,0x9d2d6baa,0xdc75e195,0x28b87f08,0x603e61ad,0x5f5c1ec,0x20f13cde,
0xc1e7202d,0xb9bf3e6b,0x6fb3da32,0x294841f2,0x23dc3709,0x12097157,0xf48fc33b,0x390dc4f6,0xa57c0ea8,0x64d4bab9,
0xb3bf0a08,0xce81d54d,0x6170bf83,0x3d466e38,0x33dedd1a,0x677e1369,0x2344d70f,0x13a7a2a6,0x73f7f90c,0xfe7c64f9,
0xb8fb5cfb,0xb8f2935b,0x2662f539,0x58c97ec4,0xfefb2861,0x41a92a40,0x74b2b741,0xca27c308,0x61e91002,0x153e181f,
0x4a2ca9c0,0xaa77e729,0xc11f657b,0x2830420f,0xf6f49ae2,0x8e62207e,0x15df85ef,0xd659a99d,0xc2d6eee6,0x826ee391,
0xc7297f3d,0x9a8c05ed,0x1ffa181d,0xf60b0ebd,0x3ad82127,0xff340cae,0x30d0b3b,0x86d1a693,0xb78a31ba,0x4ed8c739,
0xdf0e53cc,0xb3ef25f7,0x1f8a5cf2,0xf95176e1,0x3e8dc83e,0xd1d398b5,0xd6269573,0xa4200a3c,0xad5e7ea2,0x995de629,
0x7681d5de,0x46f1ac9b,0xaff78dcb,0x68fb9ad7,0x116857b4,0xd491dc3c,0x7b5c1c9,0x749d4ca2,0x30083d0f,0xdbc30b6e,
0x499a0887,0x455502e9,0x95151e85,0x42026d44,0x1406ef50,0xd8f9e4fa,0x1f33545b,0xc09f1255,0x1164f4fc,0x573a0c8f,
0x5994f117,0x45b144d4,0xc3581978,0x362ee9fb,0xbae728d2,0xe1126c30,0xc70e941c,0x3a58c957,0xb86d8917,0x1b7bd8e5,
0x2ce64678,0x42b9b8f2,0xe3671141,0xfb267b92,0x3130d5d7,0xe3d75e1d,0x976ef752,0xafd0a173,0x53a10adf,0xfe6ad1a9,
0x7344fba4,0x6d0a108e,0xb257aaba,0xf40f5388,0xceda1c0b,0x742ddf09,0xeda917ab,0xbef93fb2,0x6c20f497,0x500bbe95,
0xffe293ce,0xf4c47961,0x3511a6e,0x8ddf22d4,0x95c80465,0x7d7f10f2,0x80e78f70,0x85abe8d4,0x8f411dd7,0xef21a08f,
0xbcee575b,0x9272e13b,0x77251ab9,0xbaab7d29,0x407c6721,0xc3774ad8,0xe0e77128,0x9b583daa,0x6189288c,0x99126b61,
0xfc54a755,0x1b8c729f,0xa19b1df0,0xc1c1f7f1,0xf11f71d,0x1d67077f,0x74cb58d0,0x6d4b948,0xb803fe1f,0x7e6a73d2,
0xe3808c95,0x6ba9b37f,0xc938fdd4,0x809503f9,0xf4f06b00,0x4c61347e,0x768e08ed,0x16269c6,0x7ed6fed2,0xd314e956,
0x9bc6c1a9,0x91510c6,0x8099d1cb,0xcb9541f8,0x92855ac3,0xc1bc947b,0x3c998279,0xf61ffe69,0xfa366d0c,0xd387a8e,
0x9b41f5f1,0xc6c595dc,0x1587b85c,0x16ba3e4f,0xdb0a6057,0xdfbee067,0x8777fb48,0x949f3b5c,0x61f1f673,0x3da964fa,
0x183c94bc,0x237d505d,0x891ff17f,0xcd37462c,0xfddce3e5,0x38151262,0x3f879f67,0xc1989012,0xae5cb7b3,0xa65f7df8,
0xb0bcd3f,0x8938b857,0x24a5e848,0x88748a07,0x171b2a43,0x2e098db8,0xc49411d2,0xf69a850a,0x9896f903,0xf47ebffc,
0x73cb52a0,0xe7a362a7,0x307e2e43,0x9d84d05b,0xb68f03df,0xd755a6e1,0x6e385688,0x740adcc,0xff64dcc5,0x9346ed47,
0x7a43132c,0x64caf344,0x58efc668,0x5f53253a,0x75fa53f6,0x38360d88,0xf36cb7b9,0xb6b3cbcd,0x3379a0cb,0xa26af6f4,
0xbfd25105,0x25a6d60d,0xcc1a5895,0xea1af4fb,0xea0e1ecf,0x9c42d80b,0xfb99accd,0x56d7fe67,0xdf639fc,0x7d18c5c2,
0x4052c1b5,0x5155a8ee,0xba994b41,0x5b48a171,0xf9d415b,0xdac15ee6,0xb7586211,0xf0502434,0x69a282c8,0x1a278ebf,
0x8f89fb8b,0x4f500197,0x5f9663a1,0x4afcf8a0,0x6aac0cd4,0x80a7bcfa,0xdbdf1f1c,0xdfb1ad09,0x95b90468,0xd1f15cfc,
0x4ac5ee89,0x808845b0,0x56a029b4,0xa45ee3d9,0xca2b8135,0x2dd20975,0x6abec20,0xe08b7406,0x33956476,0x4e874c7,
0x97b7d139,0xe9148cc,0xf4769eb8,0x3d94d2e3,0xaff1ff7b,0x269ff80,0xf8da597b,0x6c2abd3a,0xb7491b49,0x29facb90,
0x8b00086d,0xf8cffab0,0xec808364,0x3fb67daf,0x92a2cca5,0x1a977114,0xcb22a482,0xd4089896,0x9fee73c2,0x58071216,
0x3837f332,0xf7d01218,0x4ee47cb3,0x1a9b9c00,0x37913f07,0x4fd9f7d0,0x4f958c84,0xba977597,0xaa8515a7,0x2511448e,
0x5e10c397,0x4b6ade61,0x908a9a2b,0xef4a9cde,0xf90ddac2,0xe75d9672,0xce893b09,0xbd455d25,0x1033ec31,0xff52f1a8,
0x8f720761,0x9cb3ca3f,0x8129a8e5,0x1a9aba98,0x47ce0723,0xf5fbc298,0xc80e01d,0x2756eb2,0xea4c8a65,0x8addf817,
0xa2a8ca45,0x51d06809,0xc3f09174,0xe51b1289,0xa2e32934,0x59efe06c,0xb55e68f,0x634d39e4,0x183f4f02,0x291eeb3b,
0x9a1931d9,0xbc6c4d9,0x977cdcd5,0x96ccf78e,0x1624f16,0x2712ba09,0x97954d20,0x201db9c8,0x64623480,0x3330ef58,
0xf7ff1a3b,0xc013e8c4,0xa0450989,0x183f4f02,0x291eeb3b,0x977cdcd5,0x96ccf78e,0xbd455d25,0x9f0110ec,0x72815fff,
0x4b8be3c9,0xf2cc16c3,0xb21f9591,0x25437d3c,0xf7e7fb16,0xcea3d3fc,0xf69945d7,0xa157cb13,0x6ef53241,0x5e752ad3,
0x186d55b9,0xb81f0d1b,0x47bf6260,0x9344113c,0xac2e4901,0x2277eb86,0xda3cdf5b,0x884fd65f,0xf523e7aa,0x1100b904,
0x9f389278,0x4fe43bdc,0xcae06d69,0x146290ea,0xb577d396,0xcda43a41,0x424b88e1,0x92f2612b,0xeaa32b18,0xe2a6a02d,
0x544d5376,0x38753ecf,0x780717d3,0x4b3511ef,0xc23dc2a8,0xd87d8355,0xdc980dec,0x3dae6f7b,0x7f345059,0xf318300d,
0x9f894e01,0x9a1b8303,0xe910bc44,0x6b555a1b,0x6cf03878,0x625ee98c,0x22b3a64f,0x6b9e47a6,0x6d3c71d2,0x3df978f3,
0x7f5b73ee,0xe8026e4a,0xd1d73562,0xd9c27979,0xfbc6d49f,0x7645fe3f,0xfa53f1dc,0x5b2c394f,0xf9dc4eae,0x6144730d,
0xd80db89d,0x4a89d4a7,0x3383f48d,0x8a4b4efa,0x6f02dcc7,0x1b78583b,0x9aea43a7,0x1fdf3a9

};
#else
UINT32 pAutoRunCheckSum[5]={0};
#define PICTURE_BUFFER_SIZE  (10)
#endif

#define PROGRESSIVE_AUTO_TEST_NUM	  (29)

#ifdef JDEC_HW_IO_MMU
UINT32 pTestBuffer = 0x3600000;
#else
UCHAR pTestBuffer[PICTURE_BUFFER_SIZE+64]={0};
#endif
#define SRC_BUFFER_ALIGNED      (UCHAR*)(((((INT32)pTestBuffer)+63)/64)*64)

#define VERTIFY_DATA_Y "Y.bin"
#define VERTIFY_DATA_C "C.bin"
#define VERTIFY_DATA_CB "Cb.bin"
#define VERTIFY_DATA_CR "Cr.bin"
#define VERTIFY_DATA_PIC "PIC.bin"

#define VERTIFY_DATA_YUV_Y "yuv1.y"
#define VERTIFY_DATA_YUV_U "yuv1.u"
#define VERTIFY_DATA_YUV_V "yuv1.v"

#define JPEG_VERTIFY_CHECKSUM_MAX 4
#define JPEG_VERTIFY_CHECKFILE_MAX 5
#define ALIGN2048(addr) (addr+0x800)&(0xfffff800)


typedef struct
{
  UCHAR *pu1InputFileName;
  UINT32 puInputFileSize;
  UCHAR *pu1Comd;
  UINT32 u4FlgSave;
  UINT32 aru4CheckSum[JPEG_VERTIFY_CHECKSUM_MAX];
}T_JPG_AUTO_TEST_CASE;

T_JPG_AUTO_TEST_CASE _rBaseTestCase[]=
{
    {"KOALA_BASE_443x600_37586.jpg",37586,NULL,1,{0,0,0}},
    {"CANON_G2_1024x768_133404.jpg",133404,NULL,1,{0,0,0}},
    {"CANON_S410_1704x2272_1181151.jpg",1181151,NULL,1,{0,0,0}},
    {"FUJI_FINEPIX500_1280x1024_609347.jpg",609347,NULL,1,{0,0,0}},
    {"FUJI_A201_1600X1200_777701.jpg",777701,NULL,1,{0,0,0}},
    {"OLYMPUS_C220Z_1600x1200_415536.jpg",415536,NULL,1,{0,0,0}},
    {"SONY_DSC_2592X1944_2173531.jpg",2173531,NULL,1,{0,0,0}},
    {"SONY_SORA_T30_2048x1536_1187521.jpg",1187521,NULL,1,{0,0,0}},
    
    {NULL,0,NULL,0,{0,0,0}}, 
};
T_JPG_AUTO_TEST_CASE _rAutoTestCase[]=
{

	//error bistream
  {"0020_MAUI_00116561.jpg",36666,NULL,0,{0,0,0}},	
  {"021.jpg",209789,NULL,0,{0,0,0}},	
  {"1004.jpg",478362,NULL,0,{0,0,0}},	
  {"1201miss22.jpg",84827,NULL,0,{0,0,0}},	
  {"concert.jpg",1726808,NULL,0,{0,0,0}},	
  {"DataLost_jpg_2.jpg",1225917,NULL,0,{0,0,0}},	
  {"DCAM00044.JPG",2424587,NULL,0,{0,0,0}},	
  {"DSC_0006.JPG",2383551,NULL,0,{0,0,0}},	
  {"FinepixF610_04.jpg",2433985,NULL,0,{0,0,0}},	
  {"Logitech_webcam_test.jpg",50567,NULL,0,{0,0,0}},	  
  {"P33853.jpg",6013860,NULL,0,{0,0,0}},	  
  {"W11.jpg",278321,NULL,0,{0,0,0}},	    
  {"19_4000X.jpg",2190838,NULL,0,{0,0,0}},	  

	//small pictures
  {"woodup_1x1.jpg",2220,NULL,0,{0,0,0}},	
  {"woodup_1x2.jpg",2115,NULL,0,{0,0,0}},	
  {"woodup_1x4.jpg",2018,NULL,0,{0,0,0}},	
  {"woodup_2x1.jpg",2131,NULL,0,{0,0,0}},	
  {"woodup_2x2.jpg",2072,NULL,0,{0,0,0}},	
  {"woodup_2x4.jpg",2009,NULL,0,{0,0,0}},	
  {"woodup_4x1.jpg",2051,NULL,0,{0,0,0}},	
  {"woodup_4x2.jpg",2015,NULL,0,{0,0,0}},	  

	//special baseline
  {"normal_CANON_G2_1024x768_133404.jpg",133404,NULL,0,{0,0,0}},
  {"normal_CANON_S410_1704x2272_1181151.jpg",1181151,NULL,0,{0,0,0}},	//MultiDQT
  {"normal_FUJI_A201_1600X1200_777701.jpg",777701,NULL,0,{0,0,0}},
  {"normal_FUJI_FINEPIX500_1280x1024_609347.jpg",609347,NULL,0,{0,0,0}},
  {"normal_KOALA_BASE_443x600_37586.jpg",37586,NULL,0,{0,0,0}},
  {"normal_OLYMPUS_C220Z_1200x1600_395007.jpg",395007,NULL,0,{0,0,0}},
  {"normal_OLYMPUS_C220Z_1600x1200_415536.jpg",415536,NULL,0,{0,0,0}},
  {"normal_SONY_DSC_2592X1944_2173531.jpg",2173531,NULL,0,{0,0,0}},
  {"normal_SONY_SORA_T30_2048x1536_1187521.jpg",1187521,NULL,0,{0,0,0}},
  {"spec_444_2950x2094_633228.jpg",633228,NULL,0,{0,0,0}},  
  {"spec_1x5000_776.jpg",776,"jset 8 1 *de **",1,{0,0,0}},
  {"spec_4x4_705.jpg",705,"jset 8 1 *de **",1,{0,0,0}},
  {"spec_4x500_1921.jpg",1921,"jset 8 1 *de **",1,{0,0,0}},
  {"spec_gray1_512x512_56170.jpg",56170,"jset 8 1 *jset 14 1 *de **",0,{0,0,0}},  
  {"spec_gray3_600x400_37078.jpg",37078,"jset 8 1 *jset 14 1 *de **",0,{0,0,0}},  
  {"spec_MulitDQT_1200x1600_415536.jpg",415536,"jset 8 1 *jset 14 0 *de **",0,{0,0,0}},  
  {"spec_small_1x1_306.jpg",306,"jset 8 1 *jset 14 0 *de **",1,{0,0,0}}, 
  {"spec_6000x5000_351870.jpg",351870,"jset 8 3 *jset 15 0 *jset 10 1 *jset 11 0 *de **",1,{0,0,0}}, 

  {NULL,0,NULL,0,{0,0,0}}, 
};

#if  defined(CC_MT5387) && !defined(CC_MT5363)   
typedef enum
{
  VDEC_VLC_CLK_108MHZ,
  VDEC_VLC_CLK_150MHZ,
  VDEC_VLC_CLK_162MHZ,
  VDEC_VLC_CLK_180MHZ,
  VDEC_VLC_CLK_200MHZ,
  VDEC_VLC_CLK_300MHZ,
  VDEC_VLC_CLK_MAX
}e_vdec_vld_clk;
#elif defined(JPG_CLK_FROM_JPGDEC_CKCFG)
typedef enum
{
  VDEC_VLC_CLK_240MHZ,  
  VDEC_VLC_CLK_216MHZ, 
  VDEC_VLC_CLK_192MHZ,
  VDEC_VLC_CLK_162MHZ,
  VDEC_VLC_CLK_108MHZ,
  VDEC_VLC_CLK_80MHZ,
  VDEC_VLC_CLK_60MHZ,  
  VDEC_VLC_CLK_MAX
}e_vdec_vld_clk;
#elif defined(JPG_CLK_FROM_GRAPHIC_5395_PLL)
typedef enum
{
  VDEC_VLC_CLK_108MHZ,
  VDEC_VLC_CLK_120MHZ,
  VDEC_VLC_CLK_144MHZ,
  VDEC_VLC_CLK_168MHZ,
  VDEC_VLC_CLK_216MHZ,  
  VDEC_VLC_CLK_240MHZ,
  VDEC_VLC_CLK_MED_MHZ,  
  VDEC_VLC_CLK_MAX
}e_vdec_vld_clk;
#elif defined(JPG_CLK_FROM_GRAPHIC)
typedef enum
{
  VDEC_VLC_CLK_171MHZ,
  VDEC_VLC_CLK_172MHZ,
  VDEC_VLC_CLK_216MHZ,
  VDEC_VLC_CLK_229MHZ,
  VDEC_VLC_CLK_240MHZ,
  VDEC_VLC_CLK_MAX
}e_vdec_vld_clk;
#else
typedef enum
{
  VDEC_VLC_CLK_100MHZ,
  VDEC_VLC_CLK_108MHZ,
  VDEC_VLC_CLK_133MHZ,
  VDEC_VLC_CLK_162MHZ,
  VDEC_VLC_CLK_180MHZ,
  VDEC_VLC_CLK_200MHZ,
  VDEC_VLC_CLK_MAX
}e_vdec_vld_clk;
#endif

static char* decode_mode_str[] = \
    {"MCU(0)", "MCU Row(1)", "MCU Row+IMGRZ(2)", "Picture + IMGRZ(3)", "Video_Skype(4)", \
     "MCU_MULTI_COLLECT(5)", "MCU_ENHANCE(6)", "MCU_ROW_MULTI_COLLECT(7)", "MCU_ROW_ENHANCE(8)", "SCAN_MULTI_COLLECT(9)", "SCAN_ENHANCE(10)"};

typedef enum
{
  PIC_DEC_MODE_BASELINE_MCU=0,
  PIC_DEC_MODE_MCU_ROW,
  PIC_DEC_MODE_MCU_ROW_IMGRZ,
  PIC_DEC_MODE_PICTURE_IMGRZ,
  PIC_DEC_MODE_PICTURE_VIDEO_MODE,  // 4
  
  PIC_DEC_MODE_PROG_MCU_MULTI_COLLECT,
  PIC_DEC_MODE_PROG_MCU_ENHANCE,
  PIC_DEC_MODE_PROG_ROW_MULTI_COLLECT,
  PIC_DEC_MODE_PROG_ROW_ENHANCE,
  PIC_DEC_MODE_PROG_SCAN_MULTI_COLLECT,
  PIC_DEC_MODE_PROG_SCAN_ENHANCE,
}e_decode_mode;
#define IS_PROGRESSIVE_DECODER (_JpegVerParam.i4JpegDecMode >= PIC_DEC_MODE_PROG_MCU_MULTI_COLLECT &&\
                             _JpegVerParam.i4JpegDecMode <= PIC_DEC_MODE_PROG_SCAN_ENHANCE)
typedef enum
{
   JPEG_VERIFY_PARAM_MULTI_SET=0,                     
  JPEG_VERIFY_PARAM_CHECKSUM,
  JPEG_VERIFY_PARAM_CHECKFILE,
  JPEG_VERIFY_PARAM_SRC_BUFFER,
  JPEG_VERIFY_PARAM_SRC_BUFSIZE,
  JPEG_VERIFY_PARAM_PIC_SIZE,   //5
  JPEG_VERIFY_PARAM_TAY_BUFFER,
  JPEG_VERIFY_PARAM_TAC_BUFFER, //7
  JPEG_VERIFY_PARAM_DEC_MODE,
  JPEG_VERIFY_PARAM_VLD_CLK,
  JPEG_VERIFY_PARAM_TO_OSD,
  JPEG_VERIFY_PARAM_OSD_CM,
  JPEG_VERIFY_PARAM_RST_OUT,   //12
  JPEG_VERIFY_PARAM_OUT_MODE,
  JPEG_VERIFY_PARAM_GRAY_PIC,
  JPEG_VERIFY_PARAM_DUMP_COLBUF,
  JPEG_VERIFY_PARAM_BIG_PIC,
  JPEG_VERIFY_PARAM_OUTSWAP,   //17
  JPEG_VERIFY_PARAM_MID,
  JPEG_VERIFY_PARAM_PIC_STOP, 
  JPEG_VERIFY_PARAM_DEC_TIME,//20
  JPEG_VERIFY_PARAM_RZ_TIME,
  JPEG_VERIFY_PARAM_PIC_FORMAT, //22
  JPEG_VERIFY_PARAM_MULTI_INIT=100,
  JPEG_VERIFY_PARAM_SHOW_HELP=200
}E_JPG_VERIFY_PARAM;


typedef struct _T_JPG_VERIFY_PARAM
{
  INT32 i4CheckSumNum;
  INT32 i4CheckFileNum;
  INT32 arCheckSum[JPEG_VERTIFY_CHECKSUM_MAX]; 
  INT32 arCheckFile[JPEG_VERTIFY_CHECKFILE_MAX];
  INT32 arCompAddr[JPEG_VERTIFY_CHECKFILE_MAX];
  INT32 arCompSize[JPEG_VERTIFY_CHECKFILE_MAX];
  INT32 i4JpegSrcBufAddr;
  INT32 i4JpegSrcBufSize;
  INT32 i4JpegPicSize;
  INT32 i4JpegTaYBufAddr;
  INT32 i4JpegTaCBufAddr;
  INT32 i4JpegDecMode;
  INT32 i4JpgVldClock;
  INT32 i4IsVdo2OSD;
  INT32 i4JpgOsdModeCM;
  INT32 i4IsRastOut;
  INT32 i4JpegModeOutPutMode;
  INT32 i4IsGrayPicture;
  INT32 i4DumpColorBuffer;
  INT32 i4IsBigPicture;
  INT32 i4OutSwapMode;
  INT32 i4MidTest;
  INT32 i4PicModeStopTest;
  INT32 i4GetDecTimeProfile;
  INT32 i4GetRZTimeProfile;
  INT32 i4PicFormat;
}T_JPG_VERIFY_PARAM,*P_JPG_VERIFY_PARAM;

#endif

#endif

#ifdef __MODEL_slt__
#include "x_fm.h"
static UINT32 _u4JpgSlt = 0;
static CHAR _acJpgFileName[50];
#endif

#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"

//#include "drv_t32.h"

extern void MPV_PllInit(void);

/******************************************************************************
* Macro            : helper macros to reduce code
******************************************************************************/
#define NONCACHE(addr)                                (((addr) & 0x0fffffff) | 0x30000000)
#define TRY(X) \
    /* following do {} while (0) should be safe */ \
    LINT_SUPPRESS_NEXT_STATEMENT(717) \
    do \
    { \
        i4Ret = (X); \
        if (i4Ret) \
        { \
            goto _catch_err; \
        } \
    } while (0)

typedef enum _ENUM_CLIJPG_ERR_CODE_T
{
    E_CLIJPG_OK                = 0,
    E_CLIJPG_ERR_GENERAL    = 1000,
    E_CLIJPG_ERR_PARAM        = 1001,
    E_CLIJPG_ERR_MEM		=1002,
    // add here
    E_CLIJPG_UNKNOWN        = 1999
} ENUM_CLIJPG_ERR_CODE_T;

typedef struct _JPGCLI_TEST_SET_T
{
    CHAR *szShortName;
    UINT32 u4ByteSize;
    CHAR *szFileName;
} JPGCLI_TEST_SET_T;

/******************************************************************************
* Test suit        : cli "jpg.t32test" data
******************************************************************************/
#ifndef JPG_SEMI_HOST
#define JPG_SEMI_HOST
#endif

#define NEW_VERIFY_FLOW 1

#if defined (CC_EMULATION) || defined (__MODEL_slt__)

#ifdef JPG_SEMI_HOST
static CHAR _szOutputDir[] =
    "d:/jpg_test/output";
#endif

JPGCLI_TEST_SET_T *_arJpgTestSet = NULL;
static CHAR *_szInputDir = NULL;
static UINT32 _u4fileSize = 0, _u4filled_size = 0;
#ifndef JPEG_AUTO_TEST
static INT32 _i4TestSetIdx = 0;
#endif
static BOOL _fgJpegInstance = FALSE;
static BOOL _fgJpegWriteEnable = FALSE;
static DRV_OSD_T *_prOsd = NULL;
static ENUM_IMG_DISP_COLOR_MODE _eDispMode = AYCBCR8888_DIRECT32;
static UINT32 _au4BytePerPixel[] = {2, 0, 0, 2};		///< bit depth (left shift value), index=ENUM_IMG_DISP_COLOR_MODE
static UINT32 _u4NtyValue = JPG_HW_NFY_FINISHED;
	
static HANDLE_T _hJPGDecSema;
extern void JPGCLI_GetTestSet(INT32 i4Index, JPGCLI_TEST_SET_T **pprSet, CHAR **pszInputDir);
extern INT32 JPGCLI_GetMaxSet(void);
static UINT8* _pu1Bs = NULL;


// 5391
extern INT32 vJpegVerify(INT32 i4Argc, const CHAR ** szArgv);
#ifndef JPEG_AUTO_TEST
static INT32 _JpgCmdSetDecMode(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _JpgCmdSetTgWH(INT32 i4Argc, const CHAR ** szArgv);


/******************************************************************************
* Prototype        : cli "jpg" commands
******************************************************************************/

CLI_EXEC_T* GetJpgCmdTbl(void);
static INT32 _JpgCmdPcAutoTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdAutoSaveData(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _JpgCmdAutoSetDecModPath(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _JpgCmdAutoSetVLDClock(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _JpgCmdAutoTestImgRzParam(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdAutoLoadData(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _JpgCmdAutoProgDecode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdAutoDecode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdInit(INT32 i4Argc, const CHAR ** szArgv);
#ifndef JPEG_AUTO_TEST
static INT32 _JpgCmdDecode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdChangeSet(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdT32Test(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdWriteEnable(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdWriteDisable(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _JPGDecodePicture(INT32 i4TestMode, INT32 i4Index, UINT8* pu1Addr, UINT32 u4Size);
static UINT32 _CalcCheckSum(void* pvData, UINT32 u4ByteLen);
static INT32 _JpgCmdAutoGetFmt(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdAutoGetAlign(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdJSet(INT32 i4Argc, const CHAR ** szArgv);

#ifdef __MODEL_slt__
static INT32 _JpgCmdSlt(INT32 i4Argc, const CHAR ** szArgv);
#endif

void _JPGFillBufCB(
UINT32 u4ImgID,
void *pv_tag,
void *pv_data,
UINT32 u4State);

#endif//ifdef cc emulation
/******************************************************************************
* Variable        : cli "jpg" command table
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(JPG)

static CLI_EXEC_T _arJpgCmdTbl[] =
{
#if defined (CC_EMULATION) || defined (__MODEL_slt__)   
    {"init",        "i",    _JpgCmdInit, NULL,            "Jpeg init,(0:OSD AYCbCr8888, 1:OSD YCbYCr, 2:VP)", CLI_SUPERVISOR},
#ifdef JPEG_AUTO_TEST 
    {"decode",       "de",    _JpgCmdAutoDecode, NULL,        "Decode a jpeg file,if decode mode is 4-progressive,please add a index", CLI_SUPERVISOR},
    {"format",       "fmt",    _JpgCmdAutoGetFmt, NULL,        "Get decoded picture format", CLI_SUPERVISOR},
#else
    {"decode",        "de",    _JpgCmdDecode, NULL,        "Decode a jpeg file", CLI_SUPERVISOR},
    {"changeset",     "cs",    _JpgCmdChangeSet, NULL,        "Change test set", CLI_SUPERVISOR},
    {"enable",        "en",    _JpgCmdWriteEnable, NULL,    "Test result write via t32host", CLI_SUPERVISOR},
    {"disable",       "di",    _JpgCmdWriteDisable, NULL,    "Test result ignore", CLI_SUPERVISOR},
    {"t32test",       "tt",    _JpgCmdT32Test, NULL,        "T32 Host Test Suit", CLI_SUPERVISOR},
#endif
   {"8520",        NULL,    vJpegVerify, NULL,        "8520 verification", CLI_SUPERVISOR}, 
#ifdef JPEG_AUTO_TEST
    {"AutoSave",        "save",    _JpgCmdAutoSaveData, NULL,        "Auto save data output data to D:\\jpg_test\\dump\\upload", CLI_SUPERVISOR},
    {"AutoLoad",        "load",    _JpgCmdAutoLoadData, NULL,        "Load file data from D:\\jpg_test\\default", CLI_SUPERVISOR},
    {"AutoTest",        "run",      _JpgCmdPcAutoTest, NULL,        "Auto Burning Test,should set decode mode as mcu-row(jset.8 1) first ", CLI_SUPERVISOR},
    {"SetParam",        "jset",    _JpgCmdJSet, NULL,        "Set Jpeg and imgrz Verify Param", CLI_SUPERVISOR},
    {"cal",		NULL,	 _JpgCmdAutoGetAlign, NULL, 	   "Calculate Align", CLI_SUPERVISOR}, 
#else
    {"decmod",        NULL,    _JpgCmdSetDecMode, NULL,        "5391 set dec mode", CLI_SUPERVISOR},
#endif
    {"wh",        NULL,    _JpgCmdSetTgWH, NULL,        "set target width & height for imgrz output", CLI_SUPERVISOR},
#ifdef __MODEL_slt__    
    {"slt",        NULL,    _JpgCmdSlt, NULL,        "slt", CLI_SUPERVISOR},
#endif 

#endif
    CLIMOD_DEBUG_CLIENTRY(JPG),
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Jpg) 
{
    "jpg",
    NULL,
    NULL,
    _arJpgCmdTbl,
    "Jpeg command",
    CLI_SUPERVISOR
};

/******************************************************************************
* Variable        : cli "jpg" enums
******************************************************************************/


/******************************************************************************
* Flag(s)        :
******************************************************************************/

/******************************************************************************
* Implementation        : helper functions for commands parsing
******************************************************************************/
#if defined (CC_EMULATION) || defined (__MODEL_slt__)
#ifdef JPEG_AUTO_TEST

extern void JPG_SetDecMode5391(INT32 i4DecMode);
extern void i4JpegSetProgressiveDecodeMode(INT32 newMode);
extern UINT32 _JPG_GetYTgW(void);
extern UINT32 _JPG_GetYTgH(void);
extern VOID vJpegVertifyAutoCreateBuffer(UCHAR *pBufferAddr);
extern INT32 vJpegVerifyAuto(INT32 index,INT32 i4JpgSize);
extern UINT32 StrToDec(const CHAR* pszStr, UINT32 u4Len);
static INT32 _CLI_CMD(const CHAR* szCmd, ...);

static T_JPG_VERIFY_PARAM _JpegVerParam;

static UCHAR *_arOutFileName[]={VERTIFY_DATA_Y,VERTIFY_DATA_C,VERTIFY_DATA_CB,VERTIFY_DATA_CR,VERTIFY_DATA_PIC,VERTIFY_DATA_YUV_Y,VERTIFY_DATA_YUV_U,VERTIFY_DATA_YUV_V};
static UINT32 _CheckSumCompmareInner=0,_InnerComparePicIndex=0;

#if  defined(CC_MT5387) && !defined(CC_MT5363)   
static INT32 _u4VLDCLKTable[VDEC_VLC_CLK_MAX+1][2]={{108,3},{150,6},{162,1},{180,2},{200,5},{300,4},{200,5}};
#elif defined(JPG_CLK_FROM_JPGDEC_CKCFG)
static INT32 _u4VLDCLKTable[VDEC_VLC_CLK_MAX+1][2]={{240,1},{216,2},{192,3},{162,4},{108,5},{80,6}, {60,7}, {216,2}};
#elif defined(JPG_CLK_FROM_GRAPHIC_5395_PLL)
static INT32 _u4VLDCLKTable[VDEC_VLC_CLK_MAX+1][2]={{108,6},{120,5},{144,4},{168,3},{216,2},{240,1},{999,9},{240,1}};
#elif defined(JPG_CLK_FROM_GRAPHIC)
static INT32 _u4VLDCLKTable[VDEC_VLC_CLK_MAX+1][2]={{171,4},{172,3},{216,2},{229,5},{240,1},{240,1}};
#else
static INT32 _u4VLDCLKTable[VDEC_VLC_CLK_MAX+1][2]={{100,6},{108,3},{133,5},{162,1},{180,2},{200,4},{200,4}};
#endif

INT32 StrToSInt(const CHAR* pszStr)
{
   BOOL fgNa=FALSE;
   const CHAR* readPos=pszStr;
   UINT32 u4Len;
   UINT32 u4DecVaue=0;
   if (pszStr == NULL)
   {
		return 0;
   }

   if(readPos[0]=='-')
   {
     fgNa=TRUE;
	 readPos++;
   }
   u4Len = x_strlen(readPos);
   u4DecVaue=StrToDec(readPos, u4Len);
   return fgNa ? (-u4DecVaue) : u4DecVaue;
}

INT32 _JpgVerifyCmdSetParam(INT32 i4ParamType,INT32 Param)
{
   _JpgCmdJSet(0xffff0000|i4ParamType,(const CHAR**)Param);
   return 0;
}

static INT32 _JpgCmdJSet(INT32 i4Argc, const CHAR ** szArgv)
{
  E_JPG_VERIFY_PARAM e_JpegParam=JPEG_VERIFY_PARAM_MULTI_INIT;
  BOOL bIsCLI=TRUE,bDecModeChange=FALSE;
  
  
  if((i4Argc&0Xffff0000))
  {
	  bIsCLI=FALSE;
	  e_JpegParam=(E_JPG_VERIFY_PARAM)(i4Argc&0xffff);
  }
  else
  {
    if(i4Argc>1)
	  e_JpegParam=(E_JPG_VERIFY_PARAM)StrToInt(szArgv[1]);
	else
	  e_JpegParam=JPEG_VERIFY_PARAM_SHOW_HELP;
  }
  
  switch(e_JpegParam)
  {
    case JPEG_VERIFY_PARAM_SRC_BUFFER:
	case JPEG_VERIFY_PARAM_TAY_BUFFER:
	case JPEG_VERIFY_PARAM_TAC_BUFFER:
	case JPEG_VERIFY_PARAM_TO_OSD:
	case JPEG_VERIFY_PARAM_RST_OUT:
	case JPEG_VERIFY_PARAM_OUT_MODE:
	case JPEG_VERIFY_PARAM_GRAY_PIC:
	case JPEG_VERIFY_PARAM_DUMP_COLBUF:
	case JPEG_VERIFY_PARAM_BIG_PIC:
	case JPEG_VERIFY_PARAM_VLD_CLK:
	case JPEG_VERIFY_PARAM_OUTSWAP:
	case JPEG_VERIFY_PARAM_OSD_CM:
	case JPEG_VERIFY_PARAM_PIC_FORMAT:
	case JPEG_VERIFY_PARAM_DEC_MODE:
	case JPEG_VERIFY_PARAM_PIC_SIZE:
    case JPEG_VERIFY_PARAM_SRC_BUFSIZE:
	case JPEG_VERIFY_PARAM_MID:
	case JPEG_VERIFY_PARAM_PIC_STOP:
	case JPEG_VERIFY_PARAM_DEC_TIME:
	case JPEG_VERIFY_PARAM_RZ_TIME:

	{
	   INT32 *pShiftBase=NULL,i4ParamShift=0,i4ParamValue=0;
       pShiftBase= &_JpegVerParam.i4JpegSrcBufAddr;
	   i4ParamShift=(e_JpegParam-JPEG_VERIFY_PARAM_SRC_BUFFER);
	   i4ParamValue=bIsCLI ? (INT32)StrToSInt(szArgv[2]) : (INT32)szArgv;
	   if(e_JpegParam==JPEG_VERIFY_PARAM_OSD_CM)
	   	  i4ParamValue+=3;
	   else if(e_JpegParam==JPEG_VERIFY_PARAM_DEC_MODE)
	   	  bDecModeChange=TRUE;
	   else if(e_JpegParam==JPEG_VERIFY_PARAM_VLD_CLK)
       {
          if(i4ParamValue > VDEC_VLC_CLK_MAX)
          {
            i4ParamValue = 0;
          }
          Printf("New VLD Clock: %dMHz\n", _u4VLDCLKTable[i4ParamValue][0]);
       }
	   pShiftBase[i4ParamShift]=i4ParamValue;
       break;
	}
	case JPEG_VERIFY_PARAM_CHECKSUM:
	{
		 INT32 *Param=(INT32 *)szArgv;
		 INT32 CheckSumNum=0,i=0;
         if(bIsCLI)
		    break;
		 
		 CheckSumNum=Param[0];
		 if(CheckSumNum==0 ||CheckSumNum>JPEG_VERTIFY_CHECKSUM_MAX)
		 	break;
		 _JpegVerParam.i4CheckSumNum=CheckSumNum;
		 
         for(i=0;i<CheckSumNum;i++)
         {
           _JpegVerParam.arCheckSum[i]=Param[i+1];
         }
		 break;
	}

	case JPEG_VERIFY_PARAM_CHECKFILE:
	{
		INT32 *Param=(INT32 *)szArgv;
		INT32 CheckFileNum=0,i=0;

        CheckFileNum= bIsCLI ? (INT32)StrToInt(szArgv[2]) : Param[0];
		if(CheckFileNum==0 ||CheckFileNum>JPEG_VERTIFY_CHECKFILE_MAX)
		  break;

		_JpegVerParam.i4CheckFileNum=CheckFileNum;

		for(i=0;i<CheckFileNum;i++)
		{   
		     //Fileindex, Address, Size
			_JpegVerParam.arCheckFile[i]=bIsCLI ? (INT32)StrToInt(szArgv[2+i*3]) : Param[1+i*3];
			_JpegVerParam.arCompAddr[i]= bIsCLI ? (INT32)StrToInt(szArgv[2+i*3+1]) : Param[1+i*3+1];
			_JpegVerParam.arCompSize[i]= bIsCLI ? (INT32)StrToInt(szArgv[2+i*3+2]) : Param[1+i*3+2];
		}
		break;
	}

	case JPEG_VERIFY_PARAM_MULTI_INIT:
	{
	  _JpegVerParam.i4JpegDecMode=1;
	  _JpegVerParam.i4IsVdo2OSD=0;
	  _JpegVerParam.i4IsRastOut=1;
	  _JpegVerParam.i4JpgOsdModeCM=5;
	  _JpegVerParam.i4JpegModeOutPutMode=-1;
	  _JpegVerParam.i4OutSwapMode=0;
	  _JpegVerParam.i4IsGrayPicture=0;
	  _JpegVerParam.i4DumpColorBuffer=0;
	  _JpegVerParam.i4IsBigPicture=0;
	  _JpegVerParam.i4GetDecTimeProfile=0;
	  _JpegVerParam.i4GetRZTimeProfile=0;
	  _JpegVerParam.i4JpgVldClock=VDEC_VLC_CLK_MAX;
	  bDecModeChange=TRUE;
	
	  break;
	}
	
	case JPEG_VERIFY_PARAM_MULTI_SET:
	{
	   INT32 i4ParamNum=0,i=0,i4ParamValue=0;
	   INT32 *Param=(INT32 *)szArgv;
	   i4ParamNum=bIsCLI ? i4Argc-2 : Param[0];
	   
       if(bIsCLI && !i4ParamNum)
       {
	      Printf("arg: decmode(0:mcu, 1:row, 2:row_rz, 3:pic_rz,4:Prog)\n");
		  Printf("     Vdo2Osd(0,1)  \nOsdCM(0:ARGB8888,1:YCbYCr,2:CbYCrY)||VdoIsRastOut(0,1)\n");  
		  Printf("     OutPutMode(-1,0,1,2) \n OutSwapMode(0,6)\n");
		  Printf("	   GrayPic(0,1)  \nDumpColorBuffer(0:no,1:yes) \n TestBigPicture(0:n,1:yes)\n");
		  Printf("	   ShowDecTimeProfile(0,1)  ShowRZTimeProfile(0,1)\n");
	   }

       for(i=0;i<i4ParamNum;i++)
       {
         i4ParamValue= bIsCLI ? (INT32)StrToSInt(szArgv[2+i]) : Param[1+i];

		 if(i==0)
		 {
		     _JpegVerParam.i4JpegDecMode=i4ParamValue;
			 bDecModeChange=TRUE;
		 }
		 else if(i==1)
		   _JpegVerParam.i4IsVdo2OSD=i4ParamValue;
		 else if(i==2)
		 {
		   if(_JpegVerParam.i4IsVdo2OSD)
		   	{
			   _JpegVerParam.i4JpgOsdModeCM=i4ParamValue+3;
			   _JpegVerParam.i4IsRastOut=1;
		   	}
		   else
		   	{
		   	   _JpegVerParam.i4IsRastOut=i4ParamValue;
			   _JpegVerParam.i4JpgOsdModeCM=5;
		   	}
		 }
		 else if(i==3)
		 {
			 _JpegVerParam.i4JpegModeOutPutMode=i4ParamValue;
		 }
		 else if(i==4)
            _JpegVerParam.i4OutSwapMode=i4ParamValue;
		 else if(i==5)
            _JpegVerParam.i4IsGrayPicture=i4ParamValue;
		 else if(i==6)
            _JpegVerParam.i4DumpColorBuffer=i4ParamValue;
		 else if(i==7)
            _JpegVerParam.i4IsBigPicture=i4ParamValue;
		 else if(i==8)
           _JpegVerParam.i4GetDecTimeProfile=i4ParamValue;
		 else if(i==9)
           _JpegVerParam.i4GetRZTimeProfile=i4ParamValue;

       }
	   if(bIsCLI)
	   	{
	       Printf("--------------------------------------------------------------------\n");
	       Printf("DecMode:%s\n",_JpegVerParam.i4JpegDecMode==0 ? "MCU" : (_JpegVerParam.i4JpegDecMode==1 ? "MCU_ROW" : (_JpegVerParam.i4JpegDecMode==2 ? "MCU_ROW+IMGRZ" : (_JpegVerParam.i4JpegDecMode==3 ? "Pic+IMGRZ" : (_JpegVerParam.i4JpegDecMode==4 ? "Progressive" : "Pic+Video mode output")) ) ));
	       Printf("IsVdo2Osd:%s\n",_JpegVerParam.i4IsVdo2OSD ? "Yes" : "No");
	       if(_JpegVerParam.i4IsVdo2OSD)
		      Printf("OSDOut:%s\n",_JpegVerParam.i4JpgOsdModeCM==3 ? "ARGB8888" : (_JpegVerParam.i4JpgOsdModeCM==4 ? "YCbYCr" : "CbYCrY"));
	       else
		      Printf("VDO RastOut:%s\n",_JpegVerParam.i4IsRastOut ? "Yes" : "No");

		   Printf("OutPutMode:%s\n",_JpegVerParam.i4JpegModeOutPutMode<0 ? "Auto" : (_JpegVerParam.i4JpegModeOutPutMode==0 ? "420" : (_JpegVerParam.i4JpegModeOutPutMode==1 ? "422" : "444") ) );
		   Printf("OutSwapMode:%s\n",_JpegVerParam.i4OutSwapMode==0 ? "16x32" : "64x32");
		   Printf("GrayPictrue:%s\n",_JpegVerParam.i4IsGrayPicture ? "Yes" : "No");
		   Printf("DumpColorBuffer:%s\n",_JpegVerParam.i4DumpColorBuffer ? "Yes" : "No");
		   Printf("BigPictrue:%s\n",_JpegVerParam.i4IsBigPicture ? "Yes" : "No");
		   Printf("ShowRZTimeProfile:%s\n",_JpegVerParam.i4GetDecTimeProfile ? "Yes" : "No");
		   Printf("ShowDecTimeProfile:%s\n",_JpegVerParam.i4GetRZTimeProfile ? "Yes" : "No");
		   Printf("--------------------------------------------------------------------\n");
	   	}
	   break;
	}

	case JPEG_VERIFY_PARAM_SHOW_HELP:
	{
		INT32 i=0,i4Pos=0;
		UCHAR pu1LogStr[256]={0};
		Printf("-------------------------------------------------------\n");
        Printf("Cmd Type:jpg.jset type [value...]\n");
		Printf(" 0-->MultiSet(decmode isVdo2Osd [OsdCm,RstOut] OutPutMode OutSwapMode GrayPic DumpColorBuf BigPicture DecTime RZTime)\n");
	    Printf(" 1-->CheckSum(%d",_JpegVerParam.i4CheckSumNum);
		for(i=0;i<_JpegVerParam.i4CheckSumNum;i++)
		{
           Printf(" 0x%x ",_JpegVerParam.arCheckSum[i]);
		}
		Printf(")\n 2-->CheckFile(%d",_JpegVerParam.i4CheckFileNum);
		for(i=0;i<_JpegVerParam.i4CheckFileNum;i++)
		{
			Printf("[%s Addr:0x%x Size:0x%x]",_arOutFileName[_JpegVerParam.arCheckFile[i]],_JpegVerParam.arCompAddr[i],_JpegVerParam.arCompSize[i]);
		}
		Printf(")\n 3-->SetSrcBufferAddr(0x%x)\n 4-->SetSrcBuffSize(0x%x)\n",_JpegVerParam.i4JpegSrcBufAddr,_JpegVerParam.i4JpegSrcBufSize);
        Printf(" 5-->SetSrcPicSize(%d Bytes)\n",_JpegVerParam.i4JpegPicSize);
		Printf(" 6-->SetYTargetBuffer(0x%x)\n 7-->SetCTargetBuffer(0x%x)\n",_JpegVerParam.i4JpegTaYBufAddr,_JpegVerParam.i4JpegTaCBufAddr);
        i4Pos=0;
		for(i=0;i<VDEC_VLC_CLK_MAX;i++)
        {
          UCHAR *puStr=pu1LogStr+i4Pos;
		  x_sprintf((CHAR *)puStr,"%d:%dMHz ",i,_u4VLDCLKTable[i][0]);
	      i4Pos+=9;
        }
		pu1LogStr[i4Pos]=0;
		
		Printf(" 8-->SetDecMode(%d)(0:MCU 1:MCU-Row 2:MCU-Row+Imgrz 3:Pic+imgrz 4: Pic+video mode output 5-10:progressive mode )\n 9-->SetVLDClock(%d %dMHz %d) (%s)\n",_JpegVerParam.i4JpegDecMode,_JpegVerParam.i4JpgVldClock,_u4VLDCLKTable[_JpegVerParam.i4JpgVldClock][0],_u4VLDCLKTable[_JpegVerParam.i4JpgVldClock][1],pu1LogStr);
		Printf("10-->IsVdo2Osd(%d)(IMGRZ:if set, imgrz output is one component,else imgrz output is Y/C two component)\n11-->OsdCM(%d)(IMGRZ:0-AYCbCr8888,1-YCbYCr4444,2-CbYCrY4444.used when IsVdo2Osd is seted.)\n12-->RstOut(%d)(IMGRZ:output is rast or block for Y/C two component,used when IsVdo2Osd is not be seted)\n",_JpegVerParam.i4IsVdo2OSD,_JpegVerParam.i4JpgOsdModeCM-3,_JpegVerParam.i4IsRastOut);
		Printf("13-->OutPutMode(%d)(-1:Default,0:420 1:422)(IMGRZ:if IsVdo2Osd is not set, this define for imgrz output format)\n14-->GrayPic(%d)(If input is a gray picture,please set this flag)\n15-->DumpColorBuffer(%d)(if want force dump color buffer,please set this flag)\n",_JpegVerParam.i4JpegModeOutPutMode,_JpegVerParam.i4IsGrayPicture,_JpegVerParam.i4DumpColorBuffer);
		Printf("16-->IsBigPicture(%d)(not use)\n17-->OutSwapMode(%d)(IMGRZ:0--16x32,6:64x32)\n18-->TestMID(%d)(not use)\n",_JpegVerParam.i4IsBigPicture,_JpegVerParam.i4OutSwapMode,_JpegVerParam.i4MidTest);
		Printf("19-->TestPicStop(%d)(force stop decode)\n20-->DecTime(%d)\n21-->ReSizeTime(%d)\n",_JpegVerParam.i4PicModeStopTest,_JpegVerParam.i4GetDecTimeProfile,_JpegVerParam.i4GetRZTimeProfile);
		Printf("22-->PicFormat(%d)(read only)\n",_JpegVerParam.i4PicFormat);
		Printf("100-->Init\n200->Help\n");
		Printf("-------------------------------------------------------\n");
		break;
	}
	
	default:
	{
      Printf("No This Param:%d\n",e_JpegParam);
	  break;
    }
	
  }

  if(bDecModeChange)
  {
	  if(_JpegVerParam.i4JpegDecMode == PIC_DEC_MODE_BASELINE_MCU)
	  {
		JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU);
	  }
	  else if(_JpegVerParam.i4JpegDecMode == PIC_DEC_MODE_MCU_ROW)
	  {
        JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU_ROW);
	  }
	  else if(_JpegVerParam.i4JpegDecMode == PIC_DEC_MODE_MCU_ROW_IMGRZ)
	  {
		JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ);
	  }
	  else if(_JpegVerParam.i4JpegDecMode == PIC_DEC_MODE_PICTURE_IMGRZ)
	  {
		JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ);
	  }
	  else if(IS_PROGRESSIVE_DECODER)
	  {
        i4JpegSetProgressiveDecodeMode(_JpegVerParam.i4JpegDecMode-PIC_DEC_MODE_PROG_MCU_MULTI_COLLECT);
	  }
	  else if(_JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_PICTURE_VIDEO_MODE)
	  {
		JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_PIC_VIDEO_OUTPUT);
	  }
      else
      {
		Printf("invalid decode mode:%d\n", _JpegVerParam.i4JpegDecMode);
        return 0;
      }
      Printf("New decode mode: %s\n", decode_mode_str[_JpegVerParam.i4JpegDecMode]);
  }
  
  return 0;
}


INT32 _JpgVerifyCmdGetParam(INT32 i4ParamType, INT32 Param)
{
  switch(i4ParamType)
  {
    case JPEG_VERIFY_PARAM_SRC_BUFFER:
	case JPEG_VERIFY_PARAM_TAY_BUFFER:
	case JPEG_VERIFY_PARAM_TAC_BUFFER:
	case JPEG_VERIFY_PARAM_TO_OSD:
	case JPEG_VERIFY_PARAM_RST_OUT:
	case JPEG_VERIFY_PARAM_OUT_MODE:
	case JPEG_VERIFY_PARAM_GRAY_PIC:
	case JPEG_VERIFY_PARAM_DUMP_COLBUF:
	case JPEG_VERIFY_PARAM_BIG_PIC:
	case JPEG_VERIFY_PARAM_CHECKSUM:
	case JPEG_VERIFY_PARAM_VLD_CLK:
	case JPEG_VERIFY_PARAM_OUTSWAP:
	case JPEG_VERIFY_PARAM_OSD_CM:
	case JPEG_VERIFY_PARAM_PIC_FORMAT:
	case JPEG_VERIFY_PARAM_DEC_MODE:
	case JPEG_VERIFY_PARAM_PIC_SIZE:
    case JPEG_VERIFY_PARAM_SRC_BUFSIZE:
    case JPEG_VERIFY_PARAM_MID:
	case JPEG_VERIFY_PARAM_PIC_STOP:
	case JPEG_VERIFY_PARAM_DEC_TIME:
	case JPEG_VERIFY_PARAM_RZ_TIME:
   	{
		 
		INT32 *pShiftBase=NULL,i4ParamShift=0,i4Ret=0;
		pShiftBase= &_JpegVerParam.i4JpegSrcBufAddr;
		i4ParamShift=(i4ParamType-JPEG_VERIFY_PARAM_SRC_BUFFER);
		
		if(Param)
		{
          *((INT32 *)Param)=pShiftBase[i4ParamShift];
		}

		if(i4ParamType==JPEG_VERIFY_PARAM_VLD_CLK)
		   i4Ret=_u4VLDCLKTable[pShiftBase[i4ParamShift]][1];
		else
		   i4Ret=pShiftBase[i4ParamShift];
		return i4Ret ;
   	}
	
    default:
	{
       Printf("Not Support %d,Please Eidt This Functin\n",i4ParamType);
	   break;
    }
  }
  return 0;
}


#ifdef JPG_SEMI_HOST_BY_USB
#include "x_fm.h"
BOOL _JPGLoadUsbFile(CHAR *pcFileName, UINT32 u4Addr)
{
    UINT64 u8FileSize, u8Tmp;
    UINT32 u4Request, u4Read;
    HANDLE_T  h_file;

    VERIFY(FMR_OK == x_fm_open(FM_ROOT_HANDLE, pcFileName, FM_READ_ONLY, 0777, FALSE, &h_file));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_END, &u8FileSize));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_BGN, &u8Tmp));

    while (u8FileSize > 0)
    {
        if (u8FileSize >> 32)
        {
            u4Request = 0xffffffff;
        }
        else
        {
            u4Request = (UINT32)u8FileSize;
        }
        x_fm_read(h_file, (VOID *)u4Addr, u4Request, &u4Read);
        ASSERT(u4Request == u4Read);
        u4Addr += u4Read;
        u8FileSize -= u4Read;
    }

    VERIFY(FMR_OK == x_fm_close(h_file));

    return TRUE;
}
#endif

static INT32 _JpgCmdAutoLoadData(INT32 i4Argc, const CHAR ** szArgv)
{
       UCHAR *szFileName=NULL;

#ifdef JPG_SEMI_HOST_BY_USB
   	CHAR pcIjpgFileName3[256];
#endif

	if(szArgv == NULL)
	{
		return 0;
	}

	if (i4Argc < 3)
	{
		Printf("arg: filename | filesize\n");
		return 0;
	}

	 szFileName=(UCHAR *)(szArgv[1]);
     _JpegVerParam.i4JpegPicSize=(INT32)StrToInt(szArgv[2]);
#ifdef JPG_SEMI_HOST
	  LOG(0,"Load File:%s",szFileName);
	  LOG(0,"\n");
      if(IS_PROGRESSIVE_DECODER)
      {
		  vJpegVertifyAutoCreateBuffer((UCHAR *)_JpegVerParam.i4JpegSrcBufAddr);
	  }

#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
    HalFlushInvalidateDCacheMultipleLine((UINT32)(_JpegVerParam.i4JpegSrcBufAddr), (UINT32)(_JpegVerParam.i4JpegSrcBufSize));
#else
    HalFlushInvalidateDCache();
#endif
	#ifdef JPG_SEMI_HOST_BY_USB
		x_sprintf(pcIjpgFileName3 , "/mnt/usb_0/Jpg_test/default/");			     
		x_strcat(pcIjpgFileName3, (CHAR *)szFileName);
		//Printf("116 imgrz load %s \n", pcIjpgFileName3);              
		//Printf("117 load addr %x \n", _JpegVerParam.i4JpegSrcBufAddr);
		_JPGLoadUsbFile(pcIjpgFileName3, _JpegVerParam.i4JpegSrcBufAddr);
	#else
		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\%s\" 0x%08x /ny", "D:\\jpg_test\\default",szFileName,_JpegVerParam.i4JpegSrcBufAddr));
	#endif
#endif

        #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine((UINT32)(_JpegVerParam.i4JpegSrcBufAddr), (UINT32)(_JpegVerParam.i4JpegSrcBufSize));
        #else
        HalFlushInvalidateDCache();
        #endif	
    
	  return 0;
}

static INT32 _JpgCmdAutoGetFmt(INT32 i4Argc, const CHAR ** szArgv)
{
   if(_JpegVerParam.i4PicFormat==4)
   	{
   	   Printf("Format:Unknow\n");
	   return 0;
   	}
   
   Printf("Format:%s\n",_JpegVerParam.i4PicFormat==0 ? "444" : (_JpegVerParam.i4PicFormat==1 ? "422V" : (_JpegVerParam.i4PicFormat==2 ? "422":"420")));
   return 0;
}

static INT32 _JpgCmdAutoDecode(INT32 i4Argc, const CHAR ** szArgv)
{
  
	if(!IS_PROGRESSIVE_DECODER)
	{
	    if(_JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_MCU_ROW_IMGRZ || _JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_PICTURE_IMGRZ|| _JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_PICTURE_VIDEO_MODE)
	    {
	       UINT32 u4TagH,u4TagW;
		   u4TagW=_JPG_GetYTgW();
	       u4TagH=_JPG_GetYTgH();
		   if(_JpegVerParam.i4IsVdo2OSD)
		   {
		       if(_JpegVerParam.i4JpgOsdModeCM==3)
		       {
				   x_memset((void *)(_JpegVerParam.i4JpegTaYBufAddr),0,(u4TagW+16)*4*u4TagH);
		       }
			   else
			   {
				   x_memset((void *)(_JpegVerParam.i4JpegTaYBufAddr),0,(u4TagW+16)*2*u4TagH);
			   }
		   }
		   else
		   {
			   x_memset((void *)(_JpegVerParam.i4JpegTaYBufAddr),0,(u4TagW+16)*u4TagW);
			   x_memset((void *)(_JpegVerParam.i4JpegTaCBufAddr),0,(u4TagW+16)*u4TagW);
		   }
	    }
        _JPGDecodePicture(0, -1, (UINT8 *)(_JpegVerParam.i4JpegSrcBufAddr), _JpegVerParam.i4JpegPicSize);
	}
	else
	{
	    INT32 i4Index=0;
	    if(i4Argc<2)
	    {
           Printf("de index(index in progressive file table,-1 to decodloaded progressive file)\n   if is not decode progressive file,pls non't set decode mode is 4 (jpg.jset 8 value)\n");
		   return 0;
	    }

		if(szArgv[1][0]=='-')
		{
          i4Index=-1;
		}
		else
		{
          i4Index=(INT32)StrToInt(szArgv[1]);
		}
		vJpegVertifyAutoCreateBuffer((UCHAR *)(_JpegVerParam.i4JpegSrcBufAddr));
		vJpegVerifyAuto(i4Index,_JpegVerParam.i4JpegPicSize);
	}
	_JpgCmdAutoGetFmt(0,NULL);
    return E_CLIJPG_OK;
}
static INT32 _JpgCmdAutoSaveData(INT32 i4Argc, const CHAR ** szArgv)
{
   INT32 i=0;
   //UCHAR **pUploadFileName=NULL;
   UCHAR *prefix=NULL;
   const CHAR ** szArgParam=szArgv+1;
   
   if(i4Argc>1 && szArgv[1][0]=='-' && szArgv[1][1]=='p')
   {
     prefix=(UCHAR *)(szArgv[2]);
	 szArgParam=szArgv+3;
   }
   
   if(_JpegVerParam.i4CheckFileNum>=JPEG_VERTIFY_CHECKFILE_MAX)
   {
	   return 0;
   }       
    
   for(i=0;i<_JpegVerParam.i4CheckFileNum;i++)
   {
        #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine((UINT32)(_JpegVerParam.arCompAddr[i]), (UINT32)(_JpegVerParam.arCompSize[i]));
        #else
        HalFlushInvalidateDCache();
        #endif
    
#ifdef JPG_SEMI_HOST
     if(prefix)
     {         
		 Printf("Save File:%s/%s_%s\n","D:/jpg_test/dump/upload",prefix,_arOutFileName[_JpegVerParam.arCheckFile[i]]);
		 UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s_%s\" 0x%08x--0x%08x","D:/jpg_test/dump/upload",prefix,_arOutFileName[_JpegVerParam.arCheckFile[i]],_JpegVerParam.arCompAddr[i],_JpegVerParam.arCompAddr[i]+_JpegVerParam.arCompSize[i]-1));
     }
	 else
	 {
		 if(i4Argc>_JpegVerParam.i4CheckFileNum)
		 {
			Printf("Save File:%s/%s\n","D:/jpg_test/dump/upload",szArgParam[i]);
			UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s\" 0x%08x--0x%08x","D:/jpg_test/dump/upload",szArgParam[i],_JpegVerParam.arCompAddr[i],_JpegVerParam.arCompAddr[i]+_JpegVerParam.arCompSize[i]-1));
		 }
		 else
		 {
      		Printf("Save File:%s/%s\n","D:/jpg_test/dump/upload",_arOutFileName[_JpegVerParam.arCheckFile[i]]);
   			UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s\" 0x%08x--0x%08x","D:/jpg_test/dump/upload",_arOutFileName[_JpegVerParam.arCheckFile[i]],_JpegVerParam.arCompAddr[i],_JpegVerParam.arCompAddr[i]+_JpegVerParam.arCompSize[i]-1));		   
    	 }
	 }
     
#endif
   }
        //HalFlushInvalidateDCache();

	Printf("------------\n");
        
   return 0;
}

/*
static BOOL _JpgRunFlow(INT32 i4Argc, const CHAR ** szArgv)
{
    IMG_Lib_Init();   
    x_memset((VOID *)&rDecOut, 0x0, sizeof(IMG_REGION_T));
    rInfo.pu1InBuffer = (UINT8 *)_rPesInfo.u4FifoStart;
    rInfo.u8BufSize = (UINT32)(_rPesInfo.u4FifoEnd - _rPesInfo.u4FifoStart);
    rInfo.rDstRegion.u4StartX = (UINT32)0;
    rInfo.rDstRegion.u4StartY = (UINT32)0;
    
    rInfo.rDstRegion.u4Width = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
    rInfo.rDstRegion.u4Height = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_height;
    rInfo.rDstRegion.u4Pitch = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
    rInfo.eDispColorMode = Y_CBCR420_SEP16;
    rInfo.eRotateDegree = CLOCKWISE_0;
    rInfo.bKeepRatio = TRUE;
    rInfo.prDecOut = &rDecOut;
    
    rInfo.rDstRegion.pu1Addr = (UINT8 *)u4YAddr;
    rInfo.rDstRegion.u4CbCrOffset = u4CAddr - u4YAddr;
    //TO DO
    rInfo.rMjpg.u4RingOffset = _rPesInfo.u4VldReadPtr - _rPesInfo.u4FifoStart;
    rInfo.rMjpg.ucFbgId = _ucMJPEGFbgId;

    IMG_SetFillBsBufEnd((UINT32)rInfo.u8BufSize, 0, TRUE);
    IMG_Decode(rInfo); 

}

*/
static BOOL _JpgChecksumAutoCheck(UINT32 *pru4Checsum)
{
   INT32 i4CheckSumIndex;
   BOOL bRet=TRUE;//,bNeedCheck=TRUE;

   for(i4CheckSumIndex=0;i4CheckSumIndex<_JpegVerParam.i4CheckSumNum;i4CheckSumIndex++)
   {
     if(pru4Checsum[i4CheckSumIndex]!=0)
     {
       break;
     }
   }

   if(i4CheckSumIndex==_JpegVerParam.i4CheckSumNum)
   {
   	 for(i4CheckSumIndex=0;i4CheckSumIndex<_JpegVerParam.i4CheckSumNum;i4CheckSumIndex++)
	 {
	   Printf("ResultChecSum[%d]=0x%x\n",i4CheckSumIndex,_JpegVerParam.arCheckSum[i4CheckSumIndex]);
	 }
     Printf("No golden checksum to compare\n");
	 return TRUE;
   }
    
   for(i4CheckSumIndex=0;i4CheckSumIndex<_JpegVerParam.i4CheckSumNum;i4CheckSumIndex++)
   {
	 if(_JpegVerParam.arCheckSum[i4CheckSumIndex]!=pru4Checsum[i4CheckSumIndex])
	 {
		Printf("ChecSum[%d]=(Golden:0x%x,Result:0x%x)\n",i4CheckSumIndex,pru4Checsum[i4CheckSumIndex],_JpegVerParam.arCheckSum[i4CheckSumIndex]);
        bRet=FALSE;
	 }
   }
   return bRet;
}
static INT32 _JpgCmdAutoGetAlign(INT32 i4Argc, const CHAR ** szArgv)
{
  INT32 i4Number=0,i4Align=0;
  if(i4Argc<3)
  {
    Printf("arg:Num Align\n");
    return 0;
  }
  i4Number=(INT32)StrToInt(szArgv[1]);
  i4Align=(INT32)StrToInt(szArgv[2]);

  if(i4Align==0)
  	return 0;
  
  Printf("Align=%d\n",i4Align*((i4Number+(i4Align-1))/i4Align));
  return 0;
}


static INT32 _JpgCmdMcuRowBurningTest(UINT32 u4StartPicture)
{
   UINT32 u4PictureIndex=0,u4PicSetIndex=0,u4MaxSetIndex=0;
   INT32 i4Ret=0;
   u4MaxSetIndex=JPGCLI_GetMaxSet();

   if(u4StartPicture!=0)
   {
	   for(u4PicSetIndex=0;u4PicSetIndex<u4MaxSetIndex;u4PicSetIndex++)
	   {
	     INT32 i4PicInSet=0;
	      _InnerComparePicIndex=u4PictureIndex;
		  JPGCLI_GetTestSet(u4PicSetIndex, &_arJpgTestSet, &_szInputDir);
		  while( _arJpgTestSet[i4PicInSet].szShortName != NULL)
		  {
              u4PictureIndex++;
			  if(u4PictureIndex>=u4StartPicture)
			  {
                 break;
			  }
			  i4PicInSet++;
		  }

		  if(_arJpgTestSet[i4PicInSet].szShortName!=NULL)
		  {
              break;
		  }
	   }
   }
   else
   {
	   _InnerComparePicIndex=0;
   }

   Printf("StartSet=%d,VLDClock=%dMHz,RegisterVal=%d\n",u4PicSetIndex,_u4VLDCLKTable[_JpegVerParam.i4JpgVldClock][0],_u4VLDCLKTable[_JpegVerParam.i4JpgVldClock][1]);
   for(;u4PicSetIndex<u4MaxSetIndex;u4PicSetIndex++)
   {
	   JPGCLI_GetTestSet(u4PicSetIndex, &_arJpgTestSet, &_szInputDir);
	   u4PictureIndex=0;
	   while( _arJpgTestSet[u4PictureIndex].szShortName != NULL)
	   {
		   i4Ret = _JPGDecodePicture(0, u4PictureIndex, NULL, 0);

		   if(i4Ret!=0)
		   {
             LOG(0,"Picture Decode Error: Set=%d,Pic=%d,Ret=%d\n",u4PicSetIndex,u4PicSetIndex,i4Ret);
		   }
           u4PictureIndex++;
		   _InnerComparePicIndex++;
	   }
   }
   _InnerComparePicIndex=0;
   return 0;
}

static INT32 _JpgCmdBaseAutoTest(T_JPG_AUTO_TEST_CASE *prTestCase)
{
    UCHAR decode_mode_idx = 0;
    static UINT32 decode_mode[] = {PIC_DEC_MODE_MCU_ROW, PIC_DEC_MODE_MCU_ROW_IMGRZ, PIC_DEC_MODE_PICTURE_IMGRZ};
    static UCHAR *decode_mode_name[] = {"MCU_ROW", "ROW_RZ", "PIC_RZ"};
    
    while(prTestCase->pu1InputFileName)
    {
        UCHAR pszName[64]={0},*uNamePos;
        UINT32 u4StrIndex=0;
        uNamePos=prTestCase->pu1InputFileName;
        while(*uNamePos!='.')
        {
            pszName[u4StrIndex++]=*uNamePos++;
        }

         _CLI_CMD("load %s %d",prTestCase->pu1InputFileName,prTestCase->puInputFileSize);
        for(decode_mode_idx=0; decode_mode_idx<sizeof(decode_mode)/sizeof(decode_mode[0]); decode_mode_idx++)
        {
            if(decode_mode[decode_mode_idx] == PIC_DEC_MODE_BASELINE_MCU)
            {
                _CLI_CMD("jset 15 0");
            }
            else if(decode_mode[decode_mode_idx] == PIC_DEC_MODE_MCU_ROW_IMGRZ)/*use video mode*/
            {
                _CLI_CMD("jset 10 0");
                _CLI_CMD("jset 11 1");
            }
            else if(decode_mode[decode_mode_idx] == PIC_DEC_MODE_PICTURE_IMGRZ)/*use osd mode*/
            {
                _CLI_CMD("jset 10 1");
                _CLI_CMD("jset 11 0");
            }
            _CLI_CMD("jset 8 %d", decode_mode[decode_mode_idx]);
            _CLI_CMD("de");

            _CLI_CMD("save -p %s(%s)", pszName, decode_mode_name[decode_mode_idx]);
        }
        prTestCase++;
    }
  return 0;
}

static INT32 _JpgCmdProgAutoTest()
{
  INT32 i4ProgIndex=0;
  
  if(!IS_PROGRESSIVE_DECODER)
  {
    Printf("Current decode mode is not progressive: %s\n", decode_mode_str[_JpegVerParam.i4JpegDecMode]);
    return 0;
  }
  
  for(i4ProgIndex=0;i4ProgIndex<PROGRESSIVE_AUTO_TEST_NUM;i4ProgIndex++)
  {
    _CLI_CMD("de %d",i4ProgIndex);
  }

  return 0;
}

static INT32 _JpgCmdNormalAutoTest(T_JPG_AUTO_TEST_CASE *prTestCase)
{
  UCHAR puDoCmd[256],*puCmd;
  UINT32 u4CmdIndex=0;
  //BOOL bfgDoCmd=TRUE;
  _CLI_CMD("jset 8 1");
  while(prTestCase->pu1InputFileName)
  {
     _CLI_CMD("load %s %d",prTestCase->pu1InputFileName,prTestCase->puInputFileSize);
	 puCmd=prTestCase->pu1Comd;
	 if(puCmd)
	 {
	    do
	    {
			u4CmdIndex=0;
			while(*puCmd!='*')
			{
			  puDoCmd[u4CmdIndex++]=*puCmd++;
			}
			puDoCmd[u4CmdIndex]=0;
			_CLI_CMD((const CHAR*)(puDoCmd));
			puCmd++;
	    }while(*puCmd!='*');

        if(_JpgChecksumAutoCheck(prTestCase->aru4CheckSum))
        {
           Printf("Normal Test Pass:%s\n",prTestCase->pu1InputFileName);
        }
		else
	    {
			Printf("Normal Test Fail:%s\n",prTestCase->pu1InputFileName);
		}

		
	 }
	 else
	 {   
		 //CMU ROW
		 _CLI_CMD("jset 8 1");
		 _CLI_CMD("de");
	     if(_JpgChecksumAutoCheck(prTestCase->aru4CheckSum))
         {
            Printf("MCU-ROW Normal Test Pass:%s\n",prTestCase->pu1InputFileName);
         }
		 else
	     {
			Printf("MCU-ROW Normal Test Fail:%s\n",prTestCase->pu1InputFileName);
		 }
		 
		 //PIC-RZ
		 
		 _CLI_CMD("jset 8 3");
		 _CLI_CMD("jset 10 1");
		 _CLI_CMD("jset 11 0");
		 //_CLI_CMD("wh 768 512");
		 _CLI_CMD("de");
         prTestCase->u4FlgSave=TRUE;
		  
	 }

	 if(prTestCase->u4FlgSave)
	 {   
	     UCHAR pszName[64]={0},*uNamePos;
		 UINT32 u4StrIndex=0;
		 uNamePos=prTestCase->pu1InputFileName;
		 while(*uNamePos!='.')
		 {
            pszName[u4StrIndex++]=*uNamePos++;
		 }
		 _CLI_CMD("save -p %s",pszName);
	 }
	 prTestCase++;
  }

  return 0;
}
static INT32 _JpgCmdPcAutoTest(INT32 i4Argc, const CHAR ** szArgv)
{
   INT32 i4InnerBurningTestType=0,i4Loop=0,i4StartClk=0,i4EndClk=0,i=0,j=0;
   UINT32 u4StartPicIndex=0;
   INT32 i4RandomClk[VDEC_VLC_CLK_MAX]={0};
   
   if(i4Argc<2)
   {
       Printf("run 0 common baseline jpeg file verify\n");
   	   Printf("run 1 ClockIndex LoopTime\n");
   	   Printf("run 2 StartClkIndex EndClkIndex LoopTime\n");
   	   Printf("run 3 ClockIndex1,ClockIndex2,ClockIndex... LoopTime\n");
   	   Printf("run 4 Auto test normal and special picture\n");
   	   Printf("run 5 Auto test progressive picture\n");
	   return 0;
   }
    
    i4InnerBurningTestType=(INT32)StrToInt(szArgv[1]);

    if(i4InnerBurningTestType==0)
	{
        _JpgCmdBaseAutoTest(_rBaseTestCase);
	   return 0;
	}
    else if(i4InnerBurningTestType==1)// loop one clk for some times;
    {
       if(i4Argc<4)
       {
		 Printf("run 1 ClockIndex LoopTime\n run at one clk for some loop\n");
		 return 0;
       }
  
	   _JpegVerParam.i4JpgVldClock=(INT32)StrToInt(szArgv[2]);
	   i4Loop=(INT32)StrToInt(szArgv[3]);
	   if(i4Argc>=5)
	   {
          u4StartPicIndex=(INT32)StrToInt(szArgv[4]);
	   }
    }
    else if(i4InnerBurningTestType==2) // form start to end clk for some times.
    {
      if(i4Argc<5)
      {
		  Printf("run 2 StartClkIndex EndClkIndex LoopTime\n run form start to end clk for some loop\n");
		  return 0;
      }
	  
      i4StartClk=(INT32)StrToInt(szArgv[2]);
	  i4EndClk=(INT32)StrToInt(szArgv[3]);
      i4Loop=(INT32)StrToInt(szArgv[4]);
	  if(i4Argc>=6)
	  {
          u4StartPicIndex=(INT32)StrToInt(szArgv[5]);
	  }
    }
	else if(i4InnerBurningTestType==3) // do some clk for some times.
	{
	   if(i4Argc<4)
	   {
		 Printf("run 3 ClockIndex1,ClockIndex2,ClockIndex... loopTime\n run at some clk for some loop\n");
		 return 0;
	   }
	   
       for(i=2;i<i4Argc-1;i++)
       {
           i4RandomClk[i-2]=(INT32)StrToInt(szArgv[i]);
       }
	   i4Loop=(INT32)StrToInt(szArgv[i]);
	}
	else if(i4InnerBurningTestType==4)
	{
	   _JpgCmdNormalAutoTest(_rAutoTestCase);
       return 0;
	}
	else if(i4InnerBurningTestType==5)
	{
       _JpgCmdProgAutoTest();
	   return 0;
	}
	
	
	_CheckSumCompmareInner=1;

    for(i=0;i<i4Loop;i++)
    {
       Printf("MCU_ROW Stress Test:Loop=%d\n",i);
       if(i4InnerBurningTestType==1)
	      _JpgCmdMcuRowBurningTest(u4StartPicIndex);
	   else if(i4InnerBurningTestType==2)
	   {
          for(_JpegVerParam.i4JpgVldClock=i4StartClk;_JpegVerParam.i4JpgVldClock<=i4EndClk;_JpegVerParam.i4JpgVldClock++)
          {
             _JpgCmdMcuRowBurningTest(u4StartPicIndex);
			 u4StartPicIndex=0;
          }
	   }
	   else if(i4InnerBurningTestType==3)
	   {
          for(j=0;j<i4Argc-3;j++)
          {
             _JpegVerParam.i4JpgVldClock=i4RandomClk[j];
             _JpgCmdMcuRowBurningTest(u4StartPicIndex);
          }
	   }
	   u4StartPicIndex=0;
    }

	_CheckSumCompmareInner=0;
	return 0;
}


#endif
extern void _JPG_SetYTgW(UINT32 u4Width);
extern void _JPG_SetYTgH(UINT32 u4Height);
static INT32 _CLI_CMD(const CHAR* szCmd, ...)
{
    CHAR szBuf[256];
    VA_LIST t_ap;

    VA_START(t_ap, szCmd);
    x_vsprintf(szBuf, szCmd, t_ap);
    VA_END(t_ap);

    return CLI_Parser(szBuf);
}
static INT32 _JpgCmdSetTgWH(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
	if(szArgv == NULL)
	{
		return 0;
	}
	
	if (i4Argc < 3)
	{
	    Printf("arg: width height");
	}
	
    u4Width  = StrToInt(szArgv[1]);
    u4Height = StrToInt(szArgv[2]);
    
    _JPG_SetYTgW(u4Width);
    _JPG_SetYTgH(u4Height);

    Printf("target width = %u, height = %u\n", u4Width, u4Height);
    return 0;
}
/*
static INT32 _JpgCmdSetTgH(INT32 i4Argc, const CHAR ** szArgv)
{
    return 0;
}
*/
#ifndef JPEG_AUTO_TEST
static INT32 _JpgCmdWriteEnable(INT32 i4Argc, const CHAR ** szArgv)
{
	UNUSED(i4Argc);
	UNUSED(szArgv);
	_fgJpegWriteEnable = TRUE;
	return 0;
}

static INT32 _JpgCmdWriteDisable(INT32 i4Argc, const CHAR ** szArgv)
{
	UNUSED(i4Argc);
	UNUSED(szArgv);
	_fgJpegWriteEnable = FALSE;
	return 0;
}
#endif

static INT32 _JpgCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4DispMode;
	FBM_POOL_T*prFbmPool=NULL;
	
        #ifdef JPEG_AUTO_TEST_PERFORMANCE
        UINT32 u4FbmId=FBM_POOL_TYPE_SCPOS_DYN_MAIN,u4FbmId2=FBM_POOL_TYPE_SCPOS_DYN_SUB;
        #else
        UINT32 u4FbmId=FBM_POOL_TYPE_B2R,u4FbmId2=FBM_POOL_TYPE_SCPOS_MAIN;
        #endif
#if 1
	if(szArgv == NULL)
	{
		return 0;
	}
	if (i4Argc > 1)
	{
		u4DispMode = StrToInt(szArgv[1]);   
		if(u4DispMode >= (UINT32)COLOR_MODE_MAX)
		{
			return (INT32)E_CLIJPG_ERR_GENERAL;
		}

		if(i4Argc > 3)
		{
          u4FbmId=StrToInt(szArgv[2]);
		      u4FbmId2=StrToInt(szArgv[3]);
		}
    else
    {
            UINT32 u4FbmAddr, u4FbmSize;
            u4FbmAddr = DRVCUST_OptGet(eFbmMemAddr);
            u4FbmSize = DRVCUST_OptGet(eFbmMemSize);
            u4FbmAddr = u4FbmAddr + u4FbmSize - 10 * 1024 * 1024;
            u4FbmId = u4FbmAddr;
            u4FbmId2 = u4FbmId + 5 * 1024 * 1024;
    }


		/*
		if(u1FbmId>=FBM_POOL_TYPE_TOTAL || u1FbmId2>=FBM_POOL_TYPE_TOTAL)
			
		{
           return (INT32)E_CLIJPG_ERR_GENERAL;
		}
		*/
	}
	else
	{
		LOG(0, "usage:\n\t jpg.i {color mode,TagFbmId}\n");
		LOG(0, "\t {color mode} 0:OSD AYCbCr8888, 1:Video Plane\n");
		return (INT32)E_CLIJPG_OK;
	}
	
	if (!_fgJpegInstance)
	{
		_fgJpegInstance = TRUE;
		if(u4DispMode == 0)		//OSD ayuv8888
		{
			_eDispMode = AYCBCR8888_DIRECT32;
			_prOsd = (DRV_OSD_T *)IMG_OSD_create(6); //OSD_CM_AYCBCR8888_DIRECT32
			if(_prOsd == NULL)
			{
				LOG(0, "OSD create failed\n");
				return (INT32)E_CLIJPG_ERR_GENERAL;
			}
		}
		else
		{
			if(IMG_VDP_Connect(0) != E_IMG_OK) //VDP_1 main
			{
				return (INT32)E_CLIJPG_ERR_GENERAL;
			}		
		}

		if(IMG_Lib_Init()!= E_IMG_OK)
		{
			return (INT32)E_CLIJPG_ERR_GENERAL;
		}
		VERIFY (x_sema_create(&_hJPGDecSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
	}
	UNUSED(szArgv);
#endif
	// init mpv
	//MPV_PllInit();
	//*(volatile UINT32*)0x2000d048 = 3;
#ifdef JPEG_AUTO_TEST
    x_memset(&_JpegVerParam,0,sizeof(T_JPG_VERIFY_PARAM));
	_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_MULTI_INIT,NULL);
	
	#if 0//def JDEC_HW_IO_MMU
	_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_SRC_BUFFER,(INT32)(0x3600000));
	#else
	//_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_SRC_BUFFER,((INT32)pTestBuffer));	
	_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_SRC_BUFFER, (INT32)SRC_BUFFER_ALIGNED);
	#endif
	
	_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_SRC_BUFSIZE,PICTURE_BUFFER_SIZE);
    if(u4FbmId>FBM_POOL_TYPE_TOTAL)
    {   
        u4FbmId=ALIGN2048(u4FbmId);
	    _JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_TAY_BUFFER,u4FbmId);
    }
	else
	{
		prFbmPool = FBM_GetPoolInfo((UINT8)u4FbmId);
	    if(prFbmPool==NULL || prFbmPool->u4Addr==NULL || prFbmPool->u4Size==0)
	    {
	       LOG(0,"Get Y Target Buffer fail\n");
		   return 0;
	    }
		
	    _JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_TAY_BUFFER,ALIGN2048(prFbmPool->u4Addr));
	}

    if(u4FbmId2>FBM_POOL_TYPE_TOTAL)
    {
        u4FbmId=ALIGN2048(u4FbmId);
		_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_TAC_BUFFER,u4FbmId2);
    }
	else
	{
		prFbmPool=NULL;
		prFbmPool=FBM_GetPoolInfo((UINT8)u4FbmId2);
		
		if(prFbmPool==NULL || prFbmPool->u4Addr==NULL || prFbmPool->u4Size==0)
		{
			LOG(0,"Get C Target Buffer fail\n");
			return 0;
		}
		_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_TAC_BUFFER,ALIGN2048(prFbmPool->u4Addr));
	}

    _JPG_SetYTgW(1024);
    _JPG_SetYTgH(768);

	#ifdef JDEC_HW_IO_MMU
	VLDJPG_MMU_init();
	#endif
	
    //_CLI_CMD("d_l 0");
	Printf("SrcAddr=0x%x,TaYaddr=0x%x,TaCaddr=0x%x\n",_JpgVerifyCmdGetParam(JPEG_VERIFY_PARAM_SRC_BUFFER,0),_JpgVerifyCmdGetParam(JPEG_VERIFY_PARAM_TAY_BUFFER,0),_JpgVerifyCmdGetParam(JPEG_VERIFY_PARAM_TAC_BUFFER,0));
#endif 
	return (INT32)E_CLIJPG_OK;
}

#ifndef JPEG_AUTO_TEST
#if defined (CC_EMULATION) || defined (__MODEL_slt__)
static INT32 _JpgCmdDecode(INT32 i4Argc, const CHAR ** szArgv)
{
	// jpg.dump
	INT32 i4Ret = (INT32)E_CLIJPG_OK;
	INT32 i4TestMode;
	UINT8 *pu1JpegAddr;
	UINT32 u4JpegSize;

	if (i4Argc > 2)
	{ 
		if((szArgv == NULL) || (szArgv[1]==NULL) || (szArgv[2]==NULL) || (szArgv[3]==NULL))
		{
			return 0;
		}
		i4TestMode = (INT32)StrToInt(szArgv[1]);
		pu1JpegAddr    = (UINT8*)StrToInt(szArgv[2]);
		u4JpegSize    = StrToInt(szArgv[3]);

		if((i4TestMode < 0) || (i4TestMode > 4))
		{
			LOG(0, "\t test mode unsupport\n");
			LOG(0, "usage:\n\t jpg.decode {test mode} {address} {size}\n");
			LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:VP\n");	
			return (INT32)E_CLIJPG_OK;
		}
		if((i4TestMode > 0) && (i4TestMode < 4) && (_prOsd == NULL))
		{
			LOG(0, "Not init yet\n");
			LOG(0, "usage:\n\t jpg.decode {test mode} {address} {size}\n");
			LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:VP\n");	
			return (INT32)E_CLIJPG_OK;	
		}

		if (!((pu1JpegAddr == NULL) || (u4JpegSize == 0)))
		{
			i4Ret = _JPGDecodePicture(i4TestMode, -1, pu1JpegAddr, u4JpegSize);
			return i4Ret;
		}
	}
	UNUSED(szArgv);
	LOG(0, "usage:\n\t jpg.decode {test mode} {address} {size}\n");
	LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:VP\n");	
	
	return (INT32)E_CLIJPG_OK;
}
#endif

static INT32 _JpgCmdChangeSet        (INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4SetIndex = 0;
    
	if(szArgv == NULL)
	{
		return 0;
	}
	if (i4Argc > 1)
	{
		i4SetIndex = (INT32)StrToInt(szArgv[1]);
	}
	_i4TestSetIdx = i4SetIndex;
	UNUSED(szArgv);
	return 0;
}
#endif

#if defined (CC_EMULATION) || defined (__MODEL_slt__)
#ifndef JPEG_AUTO_TEST
static INT32 _JpgCmdT32Test(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4Index;
	INT32 i4Ret;
	INT32 i4SetIndex = 0, i4MaxSet = 0;
	INT32 i4PicIndex = 0;
	INT32 i4TestMode = 0;
    
	if(szArgv == NULL)
	{
		return 0;
	}

	if (i4Argc > 2)		// decode distinct pattern
	{
		// szArgv[2] is pattern index       
		if ((*szArgv[2] <= '9') && (*szArgv[2] >= '0'))
		{
			i4TestMode = (INT32)StrToInt(szArgv[1]);                
			i4PicIndex = (INT32)StrToInt(szArgv[2]);
			i4SetIndex = _i4TestSetIdx;
			i4MaxSet = _i4TestSetIdx + 1;
		}
	}
	else if(i4Argc == 2)	// decode all cases
	{
		i4TestMode = (INT32)StrToInt(szArgv[1]);        
		i4PicIndex = 0;
		i4SetIndex = _i4TestSetIdx;      
		i4MaxSet = JPGCLI_GetMaxSet();
	}
	else
	{
		LOG(0, "usage:\n\t jpg.tt {test mode} {test pattern number}\n");
		LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:Video Plane\n");
		return (INT32)E_CLIJPG_OK;
	}

	if((i4TestMode < 0) || (i4TestMode > 4))
	{
		LOG(0, "Test mode unsupport\n");
		LOG(0, "usage:\n\t jpg.tt {test mode} {test pattern number}\n");		
		LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:Video Plane\n");	
		return (INT32)E_CLIJPG_OK;
	}

	if((i4TestMode > 0) && (i4TestMode < 4) && (_prOsd == NULL))
	{
		LOG(0, "Not init yet\n");
		LOG(0, "usage:\n\t jpg.tt {test mode} {test pattern number}\n");		
		LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:Video Plane\n");	
		return (INT32)E_CLIJPG_OK;	
	}
	
	for( ;i4SetIndex < i4MaxSet; i4SetIndex++)
	{
		i4Index = i4PicIndex;
		JPGCLI_GetTestSet(i4SetIndex, &_arJpgTestSet, &_szInputDir);
    
		while( _arJpgTestSet[i4Index].szShortName != NULL)
		{
			printf("filename : %s\n", _arJpgTestSet[i4Index].szFileName);			
			printf("filesize : %d\n", _arJpgTestSet[i4Index].u4ByteSize);
			i4Ret = _JPGDecodePicture(i4TestMode, i4Index, NULL, 0);
			if (i4Ret != 0)
			{
				if((i4Ret == -27) || (i4Ret == 3))
				{
					LOG(0, "Format Unsupport\n");
					i4Index++;
					continue;
				}
				if(i4Ret == -(INT32)E_CLIJPG_ERR_MEM) // memory alloc error if file too large, should not happen
				{
					i4Index++;
					continue;
				}
				// verification FPGA needs retry sometimes
				LOG(0, "Error while decode picture %s : ret(%d)\n", _arJpgTestSet[i4Index].szFileName, i4Ret);
				LOG(0, "Retry ...\n");
				i4Ret = _JPGDecodePicture(i4TestMode, i4Index, NULL, 0);
				if (i4Ret != 0)
				{
					LOG(0, "Error while decode picture %s : ret(%d)\n", _arJpgTestSet[i4Index].szFileName, i4Ret);
					LOG(0, "Fatal error !!!\n");
					//ASSERT(i4Ret == 0);
				}
			}
			if(i4Argc > 2)  // test distinct pattern
			{
				break;
			}
			i4Index++;
		}
		i4Index = 0;
	}

	UNUSED(szArgv);
	return (INT32)E_CLIJPG_OK;
}
#endif

#ifdef JPEG_DRAM_ACCESS_BYTE_MEASUREMENT
//this function is for 5395
void _JPGDramAccessBtye_start(void)
{

#ifdef CC_MT5395
    UINT32 u4Channel = 0;
    UINT32 u4AgendId;
    UINT32 u4GroupId;
    UINT32 u4Total;
    UINT32 u4Base;
    volatile HAL_DRAMC_T* _prDramcReg; 
    HAL_DRAMC_T rDramcReg;

    if (u4Channel == 0)
    {
        u4Base =  DRAM_BASE;   
    }
    else
    {
        u4Base = DRAM_CHB_BASE;
    }
    _prDramcReg = (HAL_DRAMC_T*) (u4Base + 0x180);

    u4Total = 0;

    u4AgendId = 11;
    u4GroupId = 1;

    IO_REG32(u4Base, 0x18C) =(BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ;;

    rDramcReg = *_prDramcReg;
    rDramcReg.fgBmbp1 = 0;
    rDramcReg.fgBmbp2 = 0;
    rDramcReg.fgBmbp3 = 0;
    if (u4Total != 0)
    {
        // All agent, including group 1, 2, and 3
        rDramcReg.u4Bmgp1ag = 0x1F;
    }
    else
    if (u4GroupId == 1)
    {
        rDramcReg.u4Bmgp1ag = u4AgendId;
    }
    else
    if (u4GroupId == 2)
    {
        rDramcReg.u4Bmgp2ag = u4AgendId;
    }
    else
    if (u4GroupId == 3)
    {
        rDramcReg.u4Bmgp3ag = u4AgendId;
    }
    *_prDramcReg = rDramcReg;
    if (u4GroupId == 1)
    {
        rDramcReg.fgBmbp1 = 1;
    }
    else
    if (u4GroupId == 2)
    {
        rDramcReg.fgBmbp2 = 1;
    }
    else
    if (u4GroupId == 3)
    {
        rDramcReg.fgBmbp3 = 1;
    }

    rDramcReg.fgReqAle = 0;
    *_prDramcReg = rDramcReg;
#endif
}

UINT32 _JPGDramAccessBtye_end(UINT32 u4GroupId)
{
#ifdef CC_MT5395
    UINT32 u4Result;
    UINT32 u4Channel = 0;
    UINT32 u4Base;

    if (u4Channel == 0)
    {
        u4Base =  DRAM_BASE;   
    }
    else
    {
        u4Base = DRAM_CHB_BASE;
    }
    
    if (u4GroupId == 1)
    {
        u4Result = IO_REG32(u4Base, 0x190);
    }
    else if(u4GroupId == 2)
    {
        u4Result = IO_REG32(u4Base, 0x194);
    }
    else if (u4GroupId == 3)
    {
        u4Result = IO_REG32(u4Base, 0x198);
    }
    else
    {
        u4Result = 0;
    }

    return u4Result*16;
    //Printf("Total BW bytes %d\n", u4Result*16);  
#endif
}
#endif

static INT32 _JPGDecodePicture(INT32 i4TestMode, INT32 i4Index, UINT8 *pu1JpegAddr, UINT32 u4JpegSize)
{
	//_arJpgTestSet
	INT32 i4Ret = 0;
	JMEM_YUV_LINEAR_T    *prYuvLinear = NULL;
	HAL_RAW_TIME_T        rRawTime1;
	HAL_RAW_TIME_T        rRawTime2;
	HAL_TIME_T            rHalTime1;
	HAL_TIME_T            rHalTime2;
	HAL_TIME_T            rHalTimeDuration;
	UINT8                *pu1JpegStream = NULL;
	IMG_PARAM_T rInfo;
	IMG_REGION_T rDecOut;
	IMG_BUF_PARAM_T rCmd;	
	UINT32 u4ImageSize;
	IMG_VP_INFO_T rVpInfo;
	static ENUM_IMG_ROTATE_PHASE eRotatePhase = CLOCKWISE_0;
	static BOOL bKeepRatio = FALSE;
	JPEGHANDLE hDec = (JPEGHANDLE)0;

#ifdef JPG_SEMI_HOST_BY_USB
   UINT32 i=0;	
   CHAR pcIjpgFileName[256];
   CHAR pcIjpgFileName_2[256];
#endif
      
	//---------- prepare memory buffer for input image and data structure
	if(i4Index >= 0)		//use test set pattern
	{
		pu1JpegStream =(UINT8 *)(_JpegVerParam.i4JpegSrcBufAddr);//x_mem_alloc(_arJpgTestSet[i4Index].u4ByteSize + 128);
		u4ImageSize = _arJpgTestSet[i4Index].u4ByteSize;
		//pu1JpegStream = NONCACHE((INT32)pu1JpegStream);
		if (pu1JpegStream == NULL ||u4ImageSize>10*1024*1024 )
		{
			LOG(0, "Error while load test data : %s\n", _arJpgTestSet[i4Index].szFileName);
			LOG(0, "Can not allocate memory of size %d bytes\n", _arJpgTestSet[i4Index].u4ByteSize);
			return -(INT32)E_CLIJPG_ERR_MEM;
		}
		
		// sync ram and cache
                //HalFlushInvalidateDCache();

		// load test case
		#ifdef JPG_SEMI_HOST
			#ifdef JPG_SEMI_HOST_BY_USB
				x_strcpy(pcIjpgFileName, _szInputDir);

		                for (i = 3; pcIjpgFileName[i] != '\0'; i++)
		                {
		                    if (pcIjpgFileName[i] == '\\')
		                    {
		                        pcIjpgFileName[i] = '/';
		                    }
		                }			
				x_sprintf(pcIjpgFileName_2, "/mnt/usb_0/%s/", &pcIjpgFileName[3]);		
				x_strcat(pcIjpgFileName_2, (CHAR *)_arJpgTestSet[i4Index].szFileName);
				_JPGLoadUsbFile(pcIjpgFileName_2, (UINT32)pu1JpegStream);
			#else
				UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\%s\" 0x%08x /ny", _szInputDir, _arJpgTestSet[i4Index].szFileName, pu1JpegStream));
			            #ifndef JPEG_AUTO_TEST_PERFORMANCE
			            printf("d.load.binary \"%s\\%s\" 0x%08x /ny\n", _szInputDir, _arJpgTestSet[i4Index].szFileName, (UINT32)pu1JpegStream);
			            #endif			
			#endif
		#endif



		// sync ram and cache
            #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
            HalFlushInvalidateDCacheMultipleLine((UINT32)(pu1JpegStream), (UINT32)(_arJpgTestSet[i4Index].u4ByteSize));
            #else
            HalFlushInvalidateDCache();
            #endif        
	}
	else				
	{
		pu1JpegStream = (UINT8 *)(_JpegVerParam.i4JpegSrcBufAddr);//pu1JpegAddr;
		u4ImageSize = u4JpegSize;
#ifdef __MODEL_slt__
		HANDLE_T hImgFile;
		UINT32 u4Read;
		pu1JpegStream = x_mem_alloc(u4ImageSize + 128);
		if (pu1JpegStream == NULL)
		{
			LOG(0, "Error while load test data : %s\n", _arJpgTestSet[i4Index].szFileName);
			LOG(0, "Can not allocate memory of size %d bytes\n", u4ImageSize);
			return -(INT32)E_CLIJPG_ERR_MEM;
		}
		if (FMR_OK != x_fm_open(FM_ROOT_HANDLE, _acJpgFileName, FM_READ_ONLY, 0777, FALSE, &hImgFile))
        {
            LOG(0, "Open file fail!\n");
            return -1;    
        }
	    if(FMR_OK != x_fm_read(hImgFile, pu1JpegStream, u4ImageSize, &u4Read))
        {
            LOG(0, "Read file fail!\n");
            return -1;        
        }
		x_fm_close(hImgFile);
#endif
	}

	HAL_GetRawTime(&rRawTime1);
	
	//----------start decoding

        #ifdef JPEG_DRAM_ACCESS_BYTE_MEASUREMENT
        _JPGDramAccessBtye_start();
        #endif
	
	if(i4TestMode == 0)	//verification
	{
		hDec = JDEC_AllocInstance();
		if (hDec == (JPEGHANDLE)NULL)
		{
			LOG(0, "Error while allocate jpeg decoder handle.\n");
			if(i4Index > 0 && pu1JpegStream != SRC_BUFFER_ALIGNED)
			{
				x_mem_free(pu1JpegStream);
			}
			return -(INT32)E_CLIJPG_ERR_GENERAL;
		}

		if (JDEC_New(hDec))
		{
			if(i4Index > 0 && pu1JpegStream!= SRC_BUFFER_ALIGNED)
			{
				x_mem_free(pu1JpegStream);
			}
			JDEC_FreeInstance(hDec);
			LOG(0, "Error while new jpeg decoder instance.\n");
			return -(INT32)E_CLIJPG_ERR_GENERAL;
		}
		
		TRY(JDEC_DecodeYUV(hDec, pu1JpegStream, u4ImageSize, &prYuvLinear));	
#ifdef JPEG_AUTO_TEST
        _JpegVerParam.i4PicFormat=prYuvLinear->u4ColorFMT;
        if(_JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_BASELINE_MCU||_JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_MCU_ROW || _JpegVerParam.i4DumpColorBuffer)
        {
		   INT32 arCheckFileParam[10];
		   arCheckFileParam[0]=3;
		   
		   arCheckFileParam[1]=5;
		   arCheckFileParam[2]=(INT32)(prYuvLinear->pvComp[0]);
		   arCheckFileParam[3]=prYuvLinear->au4AllocSize[0];

		   arCheckFileParam[4]=6;
		   arCheckFileParam[5]=(INT32)(prYuvLinear->pvComp[1]);
		   arCheckFileParam[6]=prYuvLinear->au4AllocSize[1];

		   arCheckFileParam[7]=7;
		   arCheckFileParam[8]=(INT32)(prYuvLinear->pvComp[2]);
		   arCheckFileParam[9]=prYuvLinear->au4AllocSize[2];
		   _JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_CHECKFILE,(INT32)arCheckFileParam);
         }
#endif
	}
	else if(i4TestMode == 4)	//video plane test, use RING_IN_DOUBLE_OUT mode
	{
		TRY((INT32)IMG_GetVideoPlaneInfo(&rVpInfo));
		
		rInfo.u8BufSize = u4ImageSize;
		rInfo.rDstRegion.u4Width = rVpInfo.u4Width;
		rInfo.rDstRegion.u4Height = rVpInfo.u4Height;
		rInfo.rDstRegion.u4StartX = 0;
		rInfo.rDstRegion.u4StartY = 0;		
		_eDispMode = rVpInfo.eDispColorMode;
		rInfo.rDstRegion.u4Pitch = (rVpInfo.u4Width<<_au4BytePerPixel[_eDispMode]) & (~(rVpInfo.u4PitchAlign - 1));
		rInfo.eDispColorMode = _eDispMode;
		rInfo.eRotateDegree = eRotatePhase; //CLOCKWISE_0
		rInfo.bKeepRatio = bKeepRatio;
		rInfo.prDecOut = &rDecOut;

		_pu1Bs = pu1JpegStream;
		_u4fileSize = u4ImageSize;
		_u4filled_size = 0;		
		rInfo.pu1InBuffer = NULL; //use internal ring buffer
		rInfo.rDstRegion.pu1Addr = rVpInfo.pu1BackFrameAddr[0];
		rInfo.rDstRegion.u4CbCrOffset = (UINT32)rVpInfo.pu1BackFrameAddr[1] - (UINT32)rVpInfo.pu1BackFrameAddr[0];
			
		TRY((INT32)IMG_Init(RING_IN_DOUBLE_OUT, NULL, 0, _JPGFillBufCB));

		TRY((INT32)IMG_Decode(rInfo));
		VERIFY (x_sema_lock(_hJPGDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);
		if(_u4NtyValue == JPG_HW_NFY_ERROR)
		{
			_u4NtyValue = JPG_HW_NFY_FINISHED;
			goto _catch_err;	
		}
		TRY((INT32)IMG_Flip());

		//set operation
		rCmd.eDispColorMode = _eDispMode;
		rCmd.eRotateDegree = eRotatePhase;
		rCmd.rSrcRegion = rDecOut;
		rInfo.rDstRegion.pu1Addr = rVpInfo.pu1CurFrameAddr[0];
		rInfo.rDstRegion.u4Depth = 1 << _au4BytePerPixel[_eDispMode];
		rInfo.rDstRegion.u4TotalLine = rVpInfo.u4Height;
		rInfo.rDstRegion.u4StartX = 0;
		rInfo.rDstRegion.u4StartY = 0;
		rCmd.rDstRegion = rInfo.rDstRegion;
		rCmd.bKeepRatio = bKeepRatio;
		IMG_Set(rCmd);	
		TRY((INT32)IMG_Flip());
		LOG(0, "phase %d\n", (UINT32)eRotatePhase);
		eRotatePhase  = (ENUM_IMG_ROTATE_PHASE)(((UINT32)eRotatePhase + 1) % (UINT32)PHASE_MAX);
		//bKeepRatio = ~bKeepRatio;
	}
	else						//image library API
	{
		rInfo.u8BufSize = u4ImageSize;
		rInfo.rDstRegion.u4Width = _prOsd->u4Width;
		rInfo.rDstRegion.u4Height = _prOsd->u4Height;
		rInfo.rDstRegion.u4StartX = 0;
		rInfo.rDstRegion.u4StartY = 0;		
		rInfo.rDstRegion.u4Pitch = _prOsd->u4Width<<_au4BytePerPixel[_eDispMode];
		rInfo.rDstRegion.u4CbCrOffset = rInfo.rDstRegion.u4Pitch * _prOsd->u4Height;
		rInfo.eDispColorMode = _eDispMode;
		rInfo.eRotateDegree = eRotatePhase;
		if(_eDispMode == AYCBCR8888_DIRECT32)
		{
			eRotatePhase  = (ENUM_IMG_ROTATE_PHASE)(((UINT32)eRotatePhase + 1) % (UINT32)PHASE_MAX);
			rInfo.rDstRegion.u4CbCrOffset = 0;
		}
		rInfo.bKeepRatio = bKeepRatio;

		rInfo.prDecOut = &rDecOut;
		
		if(i4TestMode == 1)
		{
			rInfo.pu1InBuffer = pu1JpegStream;
			rInfo.rDstRegion.pu1Addr = NULL;
			TRY((INT32)IMG_Init(LINEAR_IN_LINEAR_OUT, NULL, 0, NULL));
			TRY(IMG_GetOutputBuffer((void **)&prYuvLinear));
		}
		else if(i4TestMode == 2)
		{
			rInfo.pu1InBuffer = pu1JpegStream;
			rInfo.rDstRegion.pu1Addr = (UINT8 *)0x3000000;//_prOsd->pu1FrameBaseAddr;
			TRY((INT32)IMG_Init(LINEAR_IN_SINGLE_OUT, NULL, 0, NULL));
		}
		else
		{
			_pu1Bs = pu1JpegStream;
			_u4fileSize = u4ImageSize;
			_u4filled_size = 0;		
			rInfo.pu1InBuffer = NULL; //use internal ring buffer
			rInfo.rDstRegion.pu1Addr = (UINT8 *)0x3000000;//_prOsd->pu1FrameBaseAddr;
			TRY((INT32)IMG_Init(RING_IN_SINGLE_OUT, NULL, 0, _JPGFillBufCB));
		}

		TRY((INT32)IMG_Decode(rInfo));
		VERIFY (x_sema_lock(_hJPGDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);		
		bKeepRatio = ~bKeepRatio;
    }
      
      //-----------finish decoding

      #ifdef JPEG_DRAM_ACCESS_BYTE_MEASUREMENT
      Printf("JPG Total BW bytes %d\n", _JPGDramAccessBtye_end(1));  
      #endif
      
	HAL_GetRawTime(&rRawTime2);
	HAL_RawToTime(&rRawTime1, &rHalTime1);
	HAL_RawToTime(&rRawTime2, &rHalTime2);
	HAL_GetDeltaTime(&rHalTimeDuration, &rHalTime1, &rHalTime2);	

        #ifdef JPEG_AUTO_TEST_PERFORMANCE
        printf("%d \t %u.%06u \t %8d \t %6d \t %6d \t %s\n",_InnerComparePicIndex, rHalTimeDuration.u4Seconds, rHalTimeDuration.u4Micros, u4ImageSize, mh_width, mh_height , _JpegVerParam.i4PicFormat==0 ? "444" : (_JpegVerParam.i4PicFormat==1 ? "422V" : (_JpegVerParam.i4PicFormat==2 ? "422":"420")));
        #else
        printf("Decoded successully.\n");
        printf("Decode duration : %u.%06u seconds\n",
        	rHalTimeDuration.u4Seconds,
        	rHalTimeDuration.u4Micros);	
        #endif 
        
	if((i4TestMode < 2) && (prYuvLinear)) //verification or image library LINEAR_IN_LINEAR_OUT
	{
                // sync ram and cache
                #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
                HalFlushInvalidateDCacheMultipleLine((UINT32)(prYuvLinear->pvComp[0]), (UINT32)(prYuvLinear->au4AllocSize[0]+prYuvLinear->au4AllocSize[1]+prYuvLinear->au4AllocSize[2]));
                #else
                HalFlushInvalidateDCache();
                #endif 

		// save YUV outputs or checksum
		if (_fgJpegWriteEnable)
		{
			LOG(3, "Output data to output folder...\n");
			#ifdef JPG_SEMI_HOST
			UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s.yuv\" 0x%08x--0x%08x",
				_szOutputDir,
				_arJpgTestSet[i4Index].szFileName,
				(void*)((UINT32)prYuvLinear->pvComp[0]),
				(void*)(((UINT32)prYuvLinear->pvComp[0] +
					prYuvLinear->au4AllocSize[0] +
					prYuvLinear->au4AllocSize[1] +
					prYuvLinear->au4AllocSize[2]) - 1)));
			#endif
		}
		else
		{
			UINT32 u4ChkSum32=0;
#ifndef JPEG_AUTO_TEST
            u4ChkSum32 = _CalcCheckSum(prYuvLinear->pvComp[0],
			prYuvLinear->au4AllocSize[0] +
			prYuvLinear->au4AllocSize[1] +
			prYuvLinear->au4AllocSize[2]);
            printf("Checksum : 0x%08x\n", u4ChkSum32);
#else
            #ifdef JPEG_AUTO_TEST_PERFORMANCE
            if(0)
            #else
            if(_JpegVerParam.i4JpegDecMode!=PIC_DEC_MODE_PICTURE_IMGRZ)
            #endif
            {
				u4ChkSum32 = _CalcCheckSum(prYuvLinear->pvComp[0],
				prYuvLinear->au4AllocSize[0] +
				prYuvLinear->au4AllocSize[1] +
				prYuvLinear->au4AllocSize[2]);
				printf("Checksum : 0x%08x\n", u4ChkSum32);
				
				if(_CheckSumCompmareInner)
				{
				   if(u4ChkSum32==pAutoRunCheckSum[_InnerComparePicIndex])
					{
						Printf("%d:CheckSum Pass\n",_InnerComparePicIndex);
					}
				   else
					{
						Printf("%d,CheckSum Fail\n",_InnerComparePicIndex);
					}
				}
				else
				{
					INT32 arCheckSum[2]={0};
					arCheckSum[0]=1;
					arCheckSum[1]=u4ChkSum32;
					_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_CHECKSUM,(INT32)arCheckSum);
				}
            }
#endif
UNUSED(u4ChkSum32);

#ifdef __MODEL_slt__
            _u4JpgSlt = u4ChkSum32;
#endif
		}
	}

        #ifndef JPEG_AUTO_TEST_PERFORMANCE
        printf("done.\n");
        #endif
       
	_catch_err:
	IMG_Release();	
	if(i4Index >= 0 && pu1JpegStream && pu1JpegStream !=SRC_BUFFER_ALIGNED)
	{
		x_mem_free(pu1JpegStream);
	}
	if(i4TestMode == 0)
	{
		JDEC_FreeInstance(hDec);
	}
    return i4Ret;
}
#endif

#ifndef JPEG_AUTO_TEST
static INT32 _JpgCmdSetDecMode(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4DecMode = 0;
    
	if(szArgv == NULL)
	{
		return 0;
	}

	if (i4Argc < 2)
	{
		Printf("arg: decmode (0:mcu, 1:row, 2:row_rz, 3:pic_rz) \n");
	}
	
	i4DecMode = (INT32)StrToInt(szArgv[1]);

	if (i4DecMode == 0)
	{
	    Printf("Set JPG decode mode = MCU\n");
	    JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU);
	}
	else if (i4DecMode == 1)
	{
	    Printf("Set JPG decode mode = MCU_ROW\n");
	    JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU_ROW);
	}
	else if (i4DecMode == 2)
	{
	    Printf("Set JPG decode mode = MCU_ROW + Resizer\n");
	    JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ);
	}
	else if (i4DecMode == 3)
	{
	    Printf("Set JPG decode mode = PIC + Resizer\n");
	    JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ);
	}
	
	UNUSED(szArgv);
	return 0;
}

#endif
void _JPGFillBufCB(
UINT32 u4ImgID,
void *pv_tag,
void *pv_data,
UINT32 u4State)
{
	IMG_LIB_FILL_RING_BUF_T *prBufInfo = (IMG_LIB_FILL_RING_BUF_T *)pv_data;
	IMG_LIB_ENTRY_BUF_T *prEntry1;
	IMG_LIB_ENTRY_BUF_T *prEntry2;
	BOOL bEntry1_valid, bEntry2_valid, isEOF=FALSE; 
	UINT32 u4Addr1, u4Addr2;
	UINT32 u4Required_len1, u4Required_len2;
	UINT32 u4curFilledSize1=0, u4curFilledSize2=0;

	if(u4State == JPG_HW_NFY_PROGRESS)
	{
		return;	//ignore the progress notify
	}
	else if((u4State == JPG_HW_NFY_FINISHED) ||(u4State == JPG_HW_NFY_ERROR))
	{
		_u4NtyValue = u4State;
		VERIFY (x_sema_unlock(_hJPGDecSema) == OSR_OK);
		return;
	}
	if(prBufInfo==NULL)
	{
		return;
	}
	prEntry1 = (IMG_LIB_ENTRY_BUF_T *)&prBufInfo->rEntry1;
	prEntry2 = (IMG_LIB_ENTRY_BUF_T *)&prBufInfo->rEntry2;
	
	bEntry1_valid = prBufInfo->rEntry1.bValid;
	u4Addr1 = (UINT32)prBufInfo->rEntry1.pvStartAddr;
	u4Required_len1 = prBufInfo->rEntry1.u4RequiredLen;
	bEntry2_valid = prBufInfo->rEntry2.bValid;
	u4Addr2 = (UINT32)prBufInfo->rEntry2.pvStartAddr;
	u4Required_len2 = prBufInfo->rEntry2.u4RequiredLen;
	
	if(bEntry1_valid>0)
	{
		if((_u4filled_size + u4Required_len1) > _u4fileSize)
		{
			u4curFilledSize1 = _u4fileSize - _u4filled_size;
			isEOF = TRUE;
		}
		else
		{
			u4curFilledSize1 = u4Required_len1;
		}
		
		x_memcpy((void *)u4Addr1, (void *)_pu1Bs, u4curFilledSize1);

				  #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
                HalFlushInvalidateDCacheMultipleLine((UINT32)(u4Addr1), (UINT32)(u4curFilledSize1));
                #else
                HalFlushInvalidateDCache();
                #endif 
                
		_u4filled_size += u4curFilledSize1;
		_pu1Bs += u4curFilledSize1;
	}
	if(bEntry2_valid>0)
	{
		if((_u4filled_size + u4Required_len2) > _u4fileSize)
		{
			u4curFilledSize2 = _u4fileSize - _u4filled_size;
			isEOF = TRUE;
		}
		else
		{
			u4curFilledSize2 = u4Required_len2;
		}
		
		x_memcpy((void *)u4Addr2, (void *)_pu1Bs, u4curFilledSize2);
		
                #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
                HalFlushInvalidateDCacheMultipleLine((UINT32)(u4Addr2), (UINT32)(u4curFilledSize2));
                #else
                HalFlushInvalidateDCache();
                #endif
                
		_u4filled_size += u4curFilledSize2;
		_pu1Bs += u4curFilledSize2;
	}
	prEntry1->u4FillLen = u4curFilledSize1;
	prEntry2->u4FillLen = u4curFilledSize2;
	prBufInfo->bEOF = isEOF;

	// clean lint warning
	UNUSED(u4ImgID);
	if(pv_tag == NULL)
	{
		return;
	}
	else
	{
		*((UINT8 *)pv_tag) = 0;
	}
	//IMG_SetFillBsBufEnd(u4curFilledSize1, u4curFilledSize2, isEOF);
}

static UINT32 _CalcCheckSum(void* pvData, UINT32 u4ByteLen)
{
	UINT32* pu4Data;
	UINT32 u4Sum = 0;

	ASSERT(pvData!=NULL);
	pu4Data = (UINT32*)pvData;
	while (u4ByteLen > 3)
	{
		u4Sum += *pu4Data;
		if (u4ByteLen >= 4)
		{
			u4ByteLen -= 4;
		}
		else
		{
			u4ByteLen = 0;
		}
		pu4Data++;
	}

	return u4Sum;
}

#ifdef __MODEL_slt__
INT32 _JpgCmdSlt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SltAns;
    UINT32 u4JpegSize;

    if (i4Argc < 4)
    {
        Printf("jpg.slt <filename> <filesize> <checksum>\n");
        x_sprintf(_acJpgFileName, "/mnt/usb_0/Baseline.jpg");
        u4JpegSize = (UINT32)37586;
        //u4SltAns = (UINT32)0x9aea43a7;
        u4SltAns = (UINT32)0xc603ee98;        
    }
    else
    {
        x_sprintf(_acJpgFileName, "/mnt/usb_0/%s", szArgv[1]);
        u4JpegSize = (UINT32)StrToInt(szArgv[2]);
        u4SltAns = (UINT32)StrToInt(szArgv[3]);
    }
        
    _u4JpgSlt = 0;
    _CLI_CMD("fbm.i");
    _CLI_CMD("jpg.i 0");
    //_CLI_CMD("jpg.decmod 1");
    if (_JPGDecodePicture(0, -1, NULL, u4JpegSize))
    {
        Printf("jpeg decode error\n");
		Printf("JPGFAIL\n");
        return -1;
    }

    if (_u4JpgSlt != u4SltAns)
    {
        Printf("[JPG] Fail Count = %d\n", (INT32)_u4JpgSlt);
        return -1;
    }
    Printf("[JPG] %x , SLT JPGOK!\n", (INT32)u4SltAns);
    return 0;
}
#endif

#endif

