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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_imem.h
 *  Demux driver - uCode inclusion
 */

#ifndef DMX_IMEM_DRM_H
#define DMX_IMEM_DRM_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

static const UINT32 DRM_IMEM[] = {
0x34010512, 0x4420100a, 0x240100fe, 0x4427f80a, 0x24150001, 
0x24160002, 0x24120004, 0x200affff, 0x4405a00a, 0x24010007, 
0x4425c00a, 0x4541400a, 0x84010ae4, 0x84020ae6, 0x24210001, 
0x00401028, 0xa4010ae4, 0xa4020ae6, 0xd801fffa, 0x4401a009, 
0x44216009, 0xa2800040, 0x241a0101, 0x44201009, 0x305b00ff, 
0x868f0004, 0x86900006, 0x75e00002, 0x6a000193, 0x4461c809, 
0x9281000f, 0x68200005, 0x0321c821, 0x4721180a, 0x0361d823, 
0xa280000f, 0x928b0003, 0x4566000a, 0x4567080a, 0x12ab0002, 
0x46a7080a, 0x9293000c, 0x6a600051, 0xa280000c, 0x86810000, 
0x002131b0, 0x68200023, 0x081f01c7, 0x76760002, 0x24130000, 
0x081f01b2, 0x4403000a, 0x4403000a, 0xa280000d, 0x92810002, 
0x24210001, 0xa2810002, 0x4403000a, 0x4403000a, 0x8681001c, 
0x4423000a, 0x00200a02, 0x4423000a, 0x8681001e, 0x4423000a, 
0x00200a02, 0x4423000a, 0x8681001c, 0x4423000a, 0x00200a02, 
0x4423000a, 0x8681001e, 0x4423000a, 0x00200a02, 0x4423000a, 
0x46c3700a, 0xdc030000, 0x45230809, 0x45431009, 0xa6810034, 
0xa6820036, 0x7675002a, 0x69750003, 0x69720002, 0x08000003, 
0x92810001, 0x4426080a, 0x46c7080a, 0x2401000f, 0x4426200a, 
0x20010d80, 0x20020d70, 0x2c210002, 0x84230000, 0x4466500a, 
0x6022fffd, 0x20010d70, 0x20020d50, 0x2c210002, 0x84230000, 
0x4466180a, 0x6022fffd, 0x20010d88, 0x20020d80, 0x2c210002, 
0x84230000, 0x4466280a, 0x6022fffd, 0x7576000f, 0x20010da0, 
0x20020d70, 0x2c210002, 0x84230000, 0x4466300a, 0x6022fffd, 
0x24010003, 0x4426080a, 0x45060809, 0x7420ffff, 0x4406380a, 
0x4406380a, 0x4406380a, 0x4406380a, 0x92820000, 0x30420080, 
0x68400002, 0x081f0186, 0x081f0169, 0x69600020, 0x9291000e, 
0x01f17823, 0x0200802f, 0x69750022, 0x6976000a, 0x62000005, 
0x61fb0004, 0x24110000, 0x000f0821, 0x08000085, 0x03710821, 
0x30310007, 0x03710823, 0x08000081, 0x62000008, 0x61fb0007, 
0x24110000, 0x000f0821, 0x24020009, 0x6a60000a, 0x2402000a, 
0x08000008, 0x03710821, 0x30310007, 0x26310008, 0x03710823, 
0x24020008, 0x6a600002, 0x24020007, 0x4446080a, 0x08000070, 
0x24110000, 0x27610000, 0x62000079, 0x61fb0078, 0x000f0821, 
0x08000076, 0x868d003c, 0x868e003e, 0x75a00002, 0x69c0001a, 
0x24110000, 0x27610000, 0x61c00003, 0x61bb0002, 0x000d0821, 
0x01a16823, 0x01c0702f, 0xa68d003c, 0xa68e003e, 0x0361d823, 
0x01e17823, 0x0200802f, 0xa68f0004, 0xa6900006, 0x4406000a, 
0x4407080a, 0x081f0105, 0x46c3700a, 0xdc030000, 0x6b600087, 
0x46a6000a, 0x46a7080a, 0x4461c809, 0xa2950040, 0x08000082, 
0x868d0008, 0x868e000a, 0x01b16823, 0x01c0702f, 0xa68d0008, 
0xa68e000a, 0x61c0003f, 0x61bb003e, 0x92810001, 0x2c210082, 
0x68200038, 0x01b10821, 0x3022000f, 0x68400004, 0x92830001, 
0x006331b0, 0x74600002, 0xa2800003, 0x10410004, 0x01a20823, 
0x24110000, 0x08000033, 0x081e00ef, 0x45e30809, 0x7420fffe, 
0x92820001, 0x004131b0, 0x74200009, 0x6a200006, 0x00001021, 
0x90440da0, 0x4483000a, 0x24420001, 0x6222fffd, 0xa6800008, 
0x08000051, 0x304200fe, 0x34420002, 0x4446080a, 0x4407000a, 
0x2f390010, 0x4721180a, 0x24010010, 0x081f00d1, 0xdc060000, 
0x4406580a, 0x24010007, 0x4427000a, 0x036dd823, 0x01ed7823, 
0x0200802f, 0xa68f0004, 0xa6900006, 0x44410809, 0x45861009, 
0x00401a02, 0x00230826, 0x4423000a, 0x2dad0001, 0x69a00006, 
0x44410809, 0x00220826, 0x4423000a, 0x2dad0001, 0x75a0fff5, 
0x0800ffe1, 0x24110000, 0x000d0821, 0x08000004, 0x03710821, 
0x3031000f, 0x03710823, 0x01a16823, 0x01c0702f, 0xa68d0008, 
0xa68e000a, 0x9283000e, 0x00232021, 0x4486480a, 0x68600009, 
0x00001021, 0x90440da0, 0x4486100a, 0x24420001, 0x44462009, 
0x081e00b0, 0x7480fffe, 0x6062fffa, 0x0361d823, 0x01e17823, 
0x0200802f, 0xa68f0004, 0xa6900006, 0xa691000e, 0x081f009e, 
0x081e00a6, 0xdc06ffff, 0x46c3700a, 0xdc030000, 0x7575001e, 
0x92810001, 0x2c210082, 0x6820001b, 0x868d0008, 0x868e000a, 
0x61c00018, 0x24010010, 0x11a10016, 0x4461c809, 0xa2950040, 
0x92810001, 0x002131b0, 0x74200011, 0xa6800008, 0xa2800003, 
0x0800000e, 0x27610000, 0x62000003, 0x61fb0002, 0x25e10000, 
0x4406000a, 0x4407080a, 0x01e17823, 0x4461c809, 0x081f0080, 
0xa6800008, 0xa2800003, 0x46c3700a, 0xdc030000, 0x45234009, 
0x45434809, 0x44450809, 0xa6810012, 0x44250809, 0xa6810048, 
0x44050809, 0xa6810010, 0x4405f80a, 0x001a2821, 0x86810000, 
0x002131b0, 0x6820001f, 0x92810000, 0x30210080, 0x6820001c, 
0xd805001b, 0x081f00a1, 0x081f008e, 0x44a51809, 0x306300ff, 
0x9282000d, 0x00431021, 0xa282000d, 0x24040040, 0x46451009, 
0x4443000a, 0x00401202, 0x4443000a, 0x46651009, 0x4443000a, 
0x00401202, 0x4443000a, 0x60820002, 0x24050003, 0x2c630001, 
0x6060fff5, 0x46c3700a, 0xdc030000, 0x45230809, 0x45431009, 
0xa6810034, 0xa6820036, 0xa6880020, 0xa6890022, 0xa68f0004, 
0xa6900006, 0x75e00002, 0x6a00000d, 0x6a200008, 0x00111821, 
0x00001021, 0x44410809, 0xa0410da0, 0x24420001, 0x2c630001, 
0x7460fffc, 0x74ba0005, 0x9281000d, 0x240200c8, 0x6041002f, 
0x001a2821, 0xdc040000, 0x86810000, 0x002131b0, 0x6820002f, 
0x86810030, 0x86820032, 0x4424200a, 0x4444280a, 0x081f006e, 
0x9281000d, 0x240200c8, 0x60410004, 0xa296000c, 0xa280000d, 
0x001a2821, 0x92820002, 0x4423000a, 0x4443000a, 0x4403000a, 
0x4403000a, 0x4503000a, 0x01001202, 0x4443000a, 0x4523000a, 
0x01201202, 0x4443000a, 0x46c3700a, 0xdc030000, 0x4584180a, 
0x75e00006, 0x76000005, 0x24050005, 0x44a4100a, 0xa2800040, 
0x08000003, 0x44a4100a, 0x74ba0009, 0xa688001c, 0xa689001e, 
0x86810034, 0xa6810030, 0x86810036, 0xa6810032, 0xa6880020, 
0xa6890022, 0x92810040, 0xa2800040, 0x7420fe73, 0x4401200a, 
0x0800fe59, 0x4504200a, 0x4524280a, 0x4584180a, 0x24050005, 
0x44a4100a, 0xa688001c, 0xa689001e, 0xa6880020, 0xa6890022, 
0x4401200a, 0x0800fe4e, 0x68200008, 0x00200a00, 0x00390825, 
0x4421300a, 0x46a1280a, 0x081e0004, 0x44a10809, 0x7420fffe, 
0x1fe00000, 0x4420580a, 0x4440600a, 0x4460680a, 0x4480700a, 
0x86810024, 0x86820026, 0x86830024, 0x86840026, 0x7423fffc, 
0x7444fffb, 0x4423380a, 0x4443400a, 0x45600809, 0x45801009, 
0x45a01809, 0x45c02009, 0x1fc00000, 0x4420580a, 0x4440600a, 
0x4460680a, 0x4480700a, 0x86810038, 0x8682003a, 0x86830038, 
0x8684003a, 0x7423fffc, 0x7444fffb, 0x0800ffef, 0x081efff5, 
0x45e30809, 0x7420fffe, 0x1fe00000, 0x86880014, 0x86890016, 
0x4503180a, 0x4523200a, 0x86880018, 0x8689001a, 0x4503280a, 
0x4523300a, 0x081effd8, 0x86880020, 0x86890022, 0x4503480a, 
0x4523500a, 0x1fe00000, 0x86810034, 0x86820036, 0x4423480a, 
0x4443500a, 0x86810028, 0x8682002a, 0x4423180a, 0x4443200a, 
0x8681002c, 0x8682002e, 0x4423280a, 0x4443300a, 0x081effd7, 
0x1fe00000, 0x4420580a, 0x4440600a, 0x4460680a, 0x4480700a, 
0x86820012, 0x4445100a, 0x86820048, 0x4445080a, 0x86820010, 
0x4445000a, 0x20020a50, 0x24010007, 0x90430001, 0x68600002, 
0x24010017, 0x4425c00a, 0x2441004c, 0x24040009, 0x2c210008, 
0x2c840001, 0x4485c80a, 0x84230000, 0x4465d00a, 0x84230002, 
0x4465d80a, 0x84230004, 0x4465e00a, 0x84230006, 0x4465e80a, 
0x6080fff5, 0x84420002, 0x4445f80a, 0x45600809, 0x45801009, 
0x45a01809, 0x45c02009, 0x1fe00000};

#define DRM_IMEM_LEN   553

LINT_EXT_HEADER_END

#endif  // DMX_IMEM_DRM_H

// built on  2010/03/30 10:20
// version 5.18
// relocate the key address to allow more than 16 PIDs for DRM
// fix duplicate chunk complete interrupts when there is no clear data at the end of the chunk
