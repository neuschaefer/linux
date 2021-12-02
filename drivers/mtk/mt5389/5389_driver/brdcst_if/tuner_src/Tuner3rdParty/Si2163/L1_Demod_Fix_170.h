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
#define PATCH_VERSION 	 170
#define NB_STOP_LINES 	 1
#define PATCH_CRC 	 65120
/*NB_PATCH_INIT_LINES alone is not valid, only NB_PATCH_INIT_LINES+NB_PATCH_CORE_LINES is valid*/
#define NB_PATCH_INIT_LINES 	 0
#define NB_PATCH_CORE_LINES 	 9
#define NB_RESTART_LINES 	 5
#define TABLE_LENGTH 	 439

unsigned long PATCH_TABLE[ TABLE_LENGTH ] = {
	1, 0x90000000, 0x000004f0,

	1, 0x60000000, 0x600009ac,
	1, 0x6000029c, 0x40020300,
	64, 0x600003c0, 0x00000000, 0x9de3bda0, 0x25180000, 0xa214a000, 0x25180000, 0xac07be00, 0xec24a37c, 0xd2046104, 0x9fc24000, 0x2b3c0000, 0x96156000, 0xe80ae642, 0xa6152002, 0xe62ae642, 0xde02e2f4, 0xde22e0f8, 0x9a102012, 0xda22e374, 0x90102002, 0xd022e370, 0xd80ae342, 0x94132001, 0xd42ae342, 0xd627be00, 0x4000006c, 0xd004a37c, 0x03180002, 0xaa100011, 0xac1061f4, 0xae100012, 0xa8102000, 0x21180000, 0xc2056108, 0x9fc04000, 0xa6142000, 0x80a52001, 0xa4102000, 0x23180000, 0x1280001e, 0xa0102001, 0x650012b0, 0x90102005, 0xd604e108, 0x9fc2c000, 0x01000000, 0xd804637c, 0xc2030000, 0xe810623c, 0x80a52000, 0x22800002, 0xa4102001, 0xa0843fff, 0x1cbffff4, 0x80a4a001, 0x02bfffe9, 0xa8102000, 0x23180000, 0xd404637c, 0xe0028000, 0xe4042440, 0x80a4a000, 0x12bfffe2, 0xa8102003, 0x9010000a,
	64, 0x600004c0, 0x400000d0, 0xa8102002, 0x10bfffde, 0x21180000, 0x1a800014, 0x80a52002, 0x65001292, 0x90102001, 0x13180000, 0xe602637c, 0xd804c000, 0xd00322f4, 0xd02320f8, 0xde0322f4, 0x9b2be001, 0x94034016, 0xd612bff2, 0xd633224e, 0xe213223c, 0x80a46001, 0x32bfffcc, 0x21180000, 0x10bfffc9, 0xa8102001, 0x1280001d, 0x21180000, 0x400000cf, 0xd005e37c, 0xde05e37c, 0xd403c000, 0xda02a440, 0x80a36001, 0x0280000e, 0x03180002, 0xd812a23c, 0x80a32000, 0x32bfffbc, 0x21180000, 0x23180002, 0xe01461e4, 0x25180002, 0xe032a24e, 0xd60ca1e8, 0xd62aa0c8, 0x10bfffb3, 0xa8102001, 0xe81061e4, 0x13180002, 0xe832a24e, 0xe60a61e8, 0xe62aa0c8, 0x10bfffac, 0xa8102003, 0xda04237c, 0xd4034000, 0xd002a440, 0x80a22000, 0x32bfffa7, 0x21180000, 0x03180002, 0x6500125c, 0xd00861e6, 0xd204237c, 0xe6024000,
	64, 0x600005c0, 0xde04e440, 0x10bfffd3, 0x80a3e000, 0x01000000, 0x9de3bf98, 0xfa060000, 0xf80763b0, 0xc20f60a1, 0xb2086040, 0x80a00019, 0xb6602000, 0x92100018, 0xb40efffd, 0xb12f2010, 0xc20f60a1, 0xb7362010, 0xb008603f, 0x825ec018, 0xb206a004, 0xb60e60ff, 0x9e58401b, 0xf60f60a0, 0x832be005, 0xb420400f, 0xf20f60a2, 0x825ec019, 0xb32ea002, 0xb406400f, 0xb3286008, 0x81800000, 0x01000000, 0x01000000, 0x01000000, 0x82768019, 0x31180000, 0xb21621e4, 0xf40760e0, 0x80a6a000, 0xf8366034, 0x0280005e, 0xc2366036, 0xf80760e0, 0x80a72002, 0x0280005b, 0xb7286010, 0xf60f60a0, 0xf20f60a3, 0xb45ec019, 0xf60760e4, 0x39180000, 0xba1721e4, 0x31180002, 0xf80e21ec, 0x832ea001, 0x81800000, 0x01000000, 0x01000000, 0x01000000, 0xb073c001, 0xf4176036, 0x11180002, 0xf037603a, 0xf82a21e6, 0xb12ea010,
	64, 0x600006c0, 0xb336e007, 0x81800000, 0x01000000, 0x01000000, 0x01000000, 0xb4760019, 0xc2024000, 0xf00062f4, 0x80a62007, 0x02800039, 0xf4376038, 0xf8176034, 0xb72f2002, 0x8206c01c, 0xf00a21e6, 0xb25e2f42, 0xf6176038, 0x81800000, 0x01000000, 0x01000000, 0x01000000, 0xb476401b, 0xb3286003, 0xf42a21e6, 0xf417603a, 0x8226401c, 0x81800000, 0x01000000, 0x01000000, 0x01000000, 0xba70401a, 0xb0076005, 0xba5e8018, 0xb25ea3e8, 0xb72ee008, 0x81800000, 0x01000000, 0x01000000, 0x01000000, 0x8276401b, 0x31180002, 0x81800000, 0x01000000, 0x01000000, 0x01000000, 0xba77401c, 0xc23621ea, 0xb9286010, 0xb45ea36b, 0xb20f60ff, 0xb1372011, 0x81800000, 0x01000000, 0x01000000, 0x01000000, 0x8276801b, 0xb2064018, 0x80a04019, 0x1a80000f, 0xb1372010, 0x82004018, 0x80a04019, 0x2abfffff, 0x82004018,
	64, 0x600007c0, 0x1080000a, 0xb20f60ff, 0xb52f2001, 0x10bfffc8, 0xf42a21e6, 0xb7286010, 0xb536e011, 0x10bfffa6, 0xf4366036, 0xb20f60ff, 0xba204019, 0x39180002, 0xfa3721e2, 0x01000000, 0x81c7e008, 0x81e80000, 0xd6020000, 0xd812e24e, 0xd20ae0c8, 0x15180002, 0xd22aa1e8, 0x03180002, 0xd4020000, 0xd61061e2, 0xc202a2f4, 0x13180002, 0x80a0600a, 0x02800009, 0xd83261e4, 0xd802a2f4, 0x80a32008, 0x22800006, 0x15180002, 0xc02aa0c8, 0x82103fff, 0xc232a24e, 0x15180002, 0x11180002, 0xd632a1e0, 0x81c3e008, 0xc02a21e7, 0x9de3bf98, 0xd8060000, 0xd60322f4, 0x941ae00a, 0x80a0000a, 0x92402000, 0x821ae008, 0x80a00001, 0x82402000, 0x808a4001, 0x02800039, 0x9e10200a, 0x11180002, 0xde2320f8, 0xd40221f0, 0x1b180002, 0xd21361e0, 0x90102000, 0x9002000a, 0x80a20009, 0x2abfffff, 0x9002000a, 0x03180002,
	64, 0x600008c0, 0xda0861e7, 0xd62320f8, 0x19180000, 0xe20322a8, 0x94036001, 0xd42861e7, 0x19180002, 0xe00b21e6, 0xe4046014, 0x13001800, 0x96148009, 0x80a42000, 0xd6246014, 0x02800013, 0xa0102000, 0x1f180000, 0xa413e000, 0xa210000c, 0x65001186, 0x90102001, 0xd204a108, 0x9fc24000, 0xa0042001, 0xd0060000, 0xd6022440, 0x80a2e001, 0x02800006, 0x940c20ff, 0xd80c61e6, 0x80a2800c, 0x0abffff4, 0x01000000, 0x13180002, 0x15180002, 0xd80a61e7, 0xd612a1ea, 0x9a0b2007, 0x9f32e004, 0x03180002, 0xa45b400f, 0xe21061e2, 0xa0244012, 0x31180002, 0xe03621e0, 0x01000000, 0x81c7e008, 0x81e80000, 0x13180002, 0x832a2010, 0xd40261f0, 0x91306010, 0x92102000, 0x9202400a, 0x80a24008, 0x0abfffff, 0x9202400a, 0x01000000, 0x81c3e008, 0x01000000, 0x133c0000, 0xc20260ec, 0x80a06005, 0x02800005, 0x01000000,
	16, 0x600009c0, 0x8213c000, 0x65001cd0, 0x9e104000, 0x8213c000, 0x7ffffe7d, 0x9e104000, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04000000, 0x00000004, 0x00000000, 0x000001eb, 0x00000000,

	1, 0x80000090, 0x00000000,
	2, 0x90080010, 0x00000000, 0x00000004,
	1, 0x90000000, 0x00000000,
	1, 0x8000009C, 0x0000FFFE,
	1, 0x90000000, 0x00000000,
};

