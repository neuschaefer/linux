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

static const unsigned long PVR_IMEM[] = {
0x24010001, 0x24150001, 0x24160002, 0x24120004, 0x240affff, 
0x24010bbb, 0x46a5e80a, 0x4425f00a, 0x4405f80a, 0x24010008, 
0x4425e80a, 0x4405f00a, 0x4405f80a, 0x24010009, 0x4425e80a, 
0x24010008, 0x4425f00a, 0x4405f80a, 0x24020100, 0x2401000a, 
0x4425e80a, 0x4445f00a, 0x4405f80a, 0x2401000b, 0x4425e80a, 
0x4405f00a, 0x4405f80a, 0x2401000c, 0x4425e80a, 0x4405f00a, 
0x4405f80a, 0x2401000d, 0x4425e80a, 0x24040038, 0x4485f00a, 
0x4405f80a, 0x2401000e, 0x4425e80a, 0x4545f00a, 0x4405f80a, 
0x2401000f, 0x4425e80a, 0x4545f00a, 0x4405f80a, 0x24010010, 
0x4425e80a, 0x4405f00a, 0x4405f80a, 0x24010011, 0x4425e80a, 
0x24010010, 0x4425f00a, 0x4405f80a, 0x24010012, 0x4425e80a, 
0x24020100, 0x4445f00a, 0x4405f80a, 0x24010013, 0x4425e80a, 
0x4405f00a, 0x4405f80a, 0x24010014, 0x4425e80a, 0x4405f00a, 
0x4405f80a, 0x24010015, 0x4425e80a, 0x4485f00a, 0x4405f80a, 
0x24010016, 0x4425e80a, 0x4545f00a, 0x4405f80a, 0x24010017, 
0x4425e80a, 0x4545f00a, 0x4405f80a, 0x24010018, 0x4425e80a, 
0x4405f00a, 0x4405f80a, 0x24010019, 0x4425e80a, 0x24010018, 
0x4425f00a, 0x4405f80a, 0x2401001a, 0x4425e80a, 0x24020100, 
0x4445f00a, 0x4405f80a, 0x2401001b, 0x4425e80a, 0x4405f00a, 
0x4405f80a, 0x2401001c, 0x4425e80a, 0x4405f00a, 0x4405f80a, 
0x2401001d, 0x4425e80a, 0x4485f00a, 0x4405f80a, 0x2401001e, 
0x4425e80a, 0x4545f00a, 0x4405f80a, 0x2401001f, 0x4425e80a, 
0x4545f00a, 0x4405f80a, 0xd8030000, 0x240d0000, 0x24040000, 
0x4403a009, 0x44236009, 0x44e33809, 0x00e20870, 0x240b0000, 
0x00401080, 0x34420002, 0x4444680a, 0x45631009, 0x005d5af0, 
0x6ba00007, 0x44431809, 0x44431809, 0x44431809, 0x44431809, 
0x24420004, 0x24840004, 0x305b00ff, 0x44431809, 0x44431809, 
0x44431809, 0x44437009, 0x01dc2930, 0x73960004, 0x44431809, 
0x24840001, 0x00832021, 0x24810004, 0x783b0002, 0x27610000, 
0x0361d823, 0x00eb0030, 0x00f010b0, 0x31c3003f, 0x69600004, 
0x02001180, 0x00621825, 0x34630080, 0x24020003, 0x6ba00002, 
0x24420004, 0x4443180a, 0x4463100a, 0x240b0000, 0x84020000, 
0x84030002, 0x4444180a, 0x4464200a, 0x84020004, 0x84030006, 
0x4444280a, 0x4464300a, 0x8402000c, 0x8403000e, 0x4444480a, 
0x4464500a, 0x84020010, 0x84030012, 0x4444380a, 0x4464400a, 
0x4404780a, 0x24190000, 0x4403180a, 0x081f00a2, 0x4423180a, 
0x0321c821, 0x68400009, 0x46a4700a, 0x4586080a, 0x4446000a, 
0xa6800004, 0xa68a0006, 0xa68a0008, 0x4403200a, 0x0800ffb3, 
0x45244009, 0x45444809, 0x6b600072, 0x00eb0030, 0x00e40870, 
0x6a00000a, 0x8681000c, 0x4424800a, 0x8681000e, 0x4424880a, 
0x86810010, 0x4424900a, 0x86810012, 0x4424980a, 0x08000009, 
0x8681001c, 0x4424800a, 0x8681001e, 0x4424880a, 0x86810020, 
0x4424900a, 0x86810022, 0x4424980a, 0x68800025, 0x6a00fff7, 
0x86810014, 0x4424a00a, 0x86810016, 0x4424a80a, 0x86810018, 
0x4424b00a, 0x8681001a, 0x4424b80a, 0x08000009, 0x86810024, 
0x4424a00a, 0x86810026, 0x4424a80a, 0x86810028, 0x4424b00a, 
0x8681002a, 0x4424b80a, 0x6a00ffe5, 0x8681000c, 0x4424c00a, 
0x8681000e, 0x4424c80a, 0x86810010, 0x4424d00a, 0x86810012, 
0x4424d80a, 0x08000009, 0x8681001c, 0x4424c00a, 0x8681001e, 
0x4424c80a, 0x86810020, 0x4424d00a, 0x86810022, 0x4424d80a, 
0x4545000a, 0x86810008, 0x4425080a, 0x86810006, 0x4425100a, 
0x86810004, 0x4425180a, 0x27610000, 0x081f0061, 0x68400009, 
0x46a4700a, 0x4586080a, 0x4446000a, 0xa6800004, 0xa68a0006, 
0xa68a0008, 0x4403200a, 0x0800ff65, 0x44250809, 0xa6810008, 
0x44450809, 0xa6810006, 0x44650809, 0xa6810004, 0x45244009, 
0x45444809, 0x84130018, 0x8417001a, 0x26730001, 0x76600003, 
0x26f70001, 0xa417001a, 0xa4130018, 0x00e218f0, 0x68400003, 
0x25ad0002, 0x08000014, 0x44e52009, 0x68800012, 0x24030000, 
0x46850809, 0x46a51009, 0x742a0006, 0x3441e000, 0x742a0004, 
0x24630001, 0x6c64000a, 0x0800fff9, 0x0040d342, 0x00000000, 
0x4666200a, 0x46e6280a, 0x4586180a, 0x03400a00, 0x24210004, 
0x4426100a, 0xa40a000e, 0xa408000c, 0xa409000e, 0x84030014, 
0x84040016, 0x24630001, 0x6c640005, 0xa4030014, 0x75a00005, 
0x4403200a, 0x0800ff34, 0x25ad0001, 0xa4000014, 0x00000000, 
0x4506200a, 0x4526280a, 0x4586180a, 0x45a6100a, 0x4403200a, 
0x0800ff2b, 0x6820000d, 0x00201200, 0x00591025, 0x4443300a, 
0x24020002, 0x004b1023, 0x4443280a, 0x24000000, 0x44a31009, 
0x7440ffff, 0x46c4700a, 0xdc040000, 0x45e41009, 0x1fe00000, 
0x6820ffff, 0x24030000, 0x44431009, 0xa0620028, 0x4444000a, 
0x24630001, 0x7061fffc, 0x24000001, 0x24000001, 0x24000001, 
0x46c4700a, 0x24000001, 0xdc04fff1, 0x45e41009, 0x1fe00000, 
0x6820000d, 0x24030000, 0x44431009, 0x4444000a, 0x24630001, 
0x7061fffd, 0x24000001, 0x24000001, 0x24000001, 0x46c4700a, 
0x24000001, 0xdc04ffff, 0x45e41009, 0x1fe00000, 0x24000000, 
0x24000000, 0x8688001c, 0x8689001e, 0x1fe00000 };
#define PVR_IMEM_LEN   374
#define PVR_BOOT_ADDR   0
