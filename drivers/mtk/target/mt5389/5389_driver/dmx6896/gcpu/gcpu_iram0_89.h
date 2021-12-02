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

/** @file gcpu_iram.h
 *  gcpu driver - instruction ram uCode inclusion
 */

static const unsigned long GCPU_IRAM0_TRAPPING[] = {
0x82760278,     // bypass AES security check, this trapping should always in array last item index
0x860e2027,     // wait for DMA to finish before writing SHA/MD5 result to DMEM
0x80942161,     // add the TS Descramble with Auto-Sync command
};

static const unsigned long GCPU_IRAM0_CODE[] = {
0x001c8bd2, 0x001d8965, 0x001ed9a4, 0x0010cb64, 0x000fa957, 
0x001ed9e5, 0x001c0a54, 0x000909fa, 0x001d8955, 0x001c8be8, 
0x000909fa, 0x001ee975, 0x001de946, 0x00128b35, 0x001ee914, 
0x001ee927, 0x001bc147, 0x000809fa, 0x000c0957, 0x00088586, 
0x001b8975, 0x001b8dd4, 0x001ac167, 0x000fa947, 0x000909fa, 
0x001ee974, 0x001ee915, 0x001de946, 0x00128b35, 0x001ee927, 
0x001bc147, 0x000809fa, 0x000885b6, 0x000c0115, 0x001b8975, 
0x001b8dd4, 0x001ac167, 0x000fa975, 0x000909fa, 0x001ec9a6, 
0x001bc546, 0x000fa972, 0x001ec9d1, 0x001df545, 0x001dc684, 
0x000c0344, 0x001dc6a5, 0x000c0005, 0x001b9945, 0x001df944, 
0x001d8ea7, 0x001d8866, 0x001c0b51, 0x001c435b, 0x001c5b5a, 
0x001c0350, 0x001c1b53, 0x001d8915, 0x000c0152, 0x001c1352, 
0x000c09b5, 0x001b8955, 0x000fa969, 0x00119d70, 0x000fa972, 
0x001ec9a6, 0x001bc546, 0x000fa914, 0x00090f7d, 0x001b8d54, 
0x0008291a, 0x001dd144, 0x001a8d67, 0x001d8866, 0x001c6b5b, 
0x001c635a, 0x001b9945, 0x000889a1, 0x001ec9d1, 0x000e0955, 
0x001b8d54, 0x0008290f, 0x001dd144, 0x001a8d67, 0x001d8826, 
0x001c7b5b, 0x001c735a, 0x001b9d45, 0x000889a1, 0x001ec9d0, 
0x000e0955, 0x001d8945, 0x001d81a4, 0x001dff40, 0x0011c341, 
0x0008293b, 0x0019ca81, 0x001bd941, 0x000fa930, 0x000c0be6, 
0x001bdd60, 0x0019c641, 0x0018da41, 0x0011db61, 0x00082939, 
0x0018c446, 0x001bc965, 0x0009290b, 0x001d8964, 0x00092925, 
0x001d8944, 0x001ac965, 0x000e0955, 0x0019a57c, 0x0012ac3c, 
0x001ee83f, 0x000929d4, 0x001ee9b3, 0x001bad5c, 0x000829c5, 
0x001d9154, 0x00118c75, 0x000f2929, 0x001bad54, 0x00118d95, 
0x000f292a, 0x001b9154, 0x0008a910, 0x00119173, 0x001ee835, 
0x001c8b5b, 0x00018b15, 0x00128915, 0x001c8b5a, 0x001ba145, 
0x001dd144, 0x001a8d67, 0x001d8826, 0x000889a1, 0x001ec9df, 
0x0011ad7c, 0x001b9153, 0x00082b79, 0x00092923, 0x0019a575, 
0x00128835, 0x0011a15f, 0x001ee82d, 0x00038b15, 0x001b8958, 
0x0011a255, 0x000f29cc, 0x000929cf, 0x0012a0ff, 0x001c4b5b, 
0x0012e8fb, 0x001cfb5a, 0x001d895c, 0x0011ac35, 0x00082991, 
0x001d897a, 0x001d8937, 0x0012a076, 0x001a8177, 0x000f2995, 
0x001bb55a, 0x000829ea, 0x001d8974, 0x001a8115, 0x000fa9fa, 
0x001d9154, 0x0011a475, 0x00118d55, 0x000f29fa, 0x0011a474, 
0x001b8545, 0x001dd144, 0x001a8d67, 0x001d8866, 0x000889a1, 
0x001b8d47, 0x0008a90e, 0x001bcd5a, 0x001a8115, 0x000fa9ee, 
0x00128756, 0x0012af5c, 0x00128436, 0x00118495, 0x000f29ea, 
0x001184f6, 0x000929f6, 0x001bb55a, 0x000fa999, 0x001bad7c, 
0x000929cb, 0x0019a575, 0x00128835, 0x0012a15f, 0x001ba55c, 
0x0011a095, 0x000f2923, 0x0011a0ff, 0x00092923, 0x001d8975, 
0x00198ab5, 0x001c8a5a, 0x001caa56, 0x001cba52, 0x001be949, 
0x001bb148, 0x0019a574, 0x00128c34, 0x00118c73, 0x0012a071, 
0x00199514, 0x00129173, 0x00119971, 0x00119ad4, 0x000f2988, 
0x000928f1, 0x001298f1, 0x000928f1, 0x001d88b7, 0x0010a917, 
0x0008284a, 0x001a8095, 0x000829bc, 0x001a80b5, 0x000829b2, 
0x0009284a, 0x001db9d6, 0x000929bf, 0x001d8b56, 0x001aad5c, 
0x000f29ab, 0x001bad45, 0x001dc844, 0x001d89c7, 0x00088999, 
0x0019aad5, 0x00188ad5, 0x001a88a5, 0x000fa9ad, 0x001badc5, 
0x001dc864, 0x001d89c7, 0x00088999, 0x001a8885, 0x000fa9ab, 
0x001badc5, 0x001dc804, 0x001d89c7, 0x00088999, 0x001d8957, 
0x0019aad5, 0x00188ad5, 0x001a8895, 0x000fa853, 0x00118d15, 
0x001a8b55, 0x000f2875, 0x001dc864, 0x0019aad5, 0x00188ad5, 
0x001a88b5, 0x000fa859, 0x001dc804, 0x0012e105, 0x001d89c7, 
0x0008a861, 0x001c935e, 0x00088847, 0x001dc865, 0x0019aad5, 
0x00188ad5, 0x001a88b5, 0x000fa842, 0x001dc805, 0x0012e104, 
0x001d89c7, 0x0008a810, 0x001b8357, 0x00118d15, 0x000f284a, 
0x00118d15, 0x000fa9aa, 0x000e0955, 0x001ec815, 0x001c8b5c, 
0x001ec815, 0x001c8b5c, 0x001ec815, 0x001c8b5c, 0x001ec815, 
0x001c8b5c, 0x0012e105, 0x001dc804, 0x001d89c7, 0x0008a861, 
0x001da9d5, 0x001c8b5e, 0x00088847, 0x001dc805, 0x0012e104, 
0x001d89c7, 0x0008a810, 0x0009284a, 0x000d0947, 0x0019ca87, 
0x0018c227, 0x0018c905, 0x001d8d40, 0x0011df00, 0x000ca816, 
0x000c0b46, 0x001bc965, 0x001bc147, 0x00082816, 0x000c0b41, 
0x0015c706, 0x0016dbe1, 0x0017c7c6, 0x000c0324, 0x000e0955, 
0x000d0947, 0x0019ce87, 0x0018c227, 0x0018cd04, 0x001d8d40, 
0x0011df00, 0x000ca809, 0x000c0b43, 0x001bc147, 0x0008280b, 
0x000c0b61, 0x0016d306, 0x0016dbe1, 0x0015dbe1, 0x0017c7c6, 
0x000c0324, 0x001bcd64, 0x000c0b61, 0x0015d3e3, 0x0015db01, 
0x0016db01, 0x0017d3c3, 0x000c0384, 0x00138955, 0x000e0955, 
0x000c0384, 0x001bcd64, 0x00092809, 0x001d8b5d, 0x001ee969, 
0x001ee908, 0x001ee931, 0x001ee9cb, 0x001ee9ea, 0x001ee990, 
0x001ee852, 0x001ee875, 0x001ee814, 0x00189575, 0x001996b2, 
0x001896b2, 0x00198ab5, 0x00188ab5, 0x001b8955, 0x000fa822, 
0x001d8959, 0x001d8955, 0x001c8a58, 0x001c8a5a, 0x000928f6, 
0x001ee8d5, 0x00198a54, 0x00188e54, 0x00198b53, 0x00189253, 
0x0019885e, 0x0018a65e, 0x00188a59, 0x001ee8f5, 0x00198817, 
0x001882f7, 0x001ee89c, 0x001ee8b5, 0x0019897f, 0x0018a17f, 
0x001a8155, 0x000828f6, 0x001a81f5, 0x00082920, 0x001b9153, 
0x000828c4, 0x001ee825, 0x001dd144, 0x001a9167, 0x001d8966, 
0x000889a1, 0x001ee9a5, 0x00118d95, 0x0011c955, 0x000f2b7b, 
0x001c6b5b, 0x001c635a, 0x001b9945, 0x001dd155, 0x00128984, 
0x00118d95, 0x001a8967, 0x001d8866, 0x000889a1, 0x001ec9d1, 
0x001ee9b3, 0x00119153, 0x001a8175, 0x000828fa, 0x001ee9b3, 
0x001b9153, 0x00082b7c, 0x00199514, 0x000389e9, 0x00128d5e, 
0x001ba554, 0x001d8977, 0x00119175, 0x000f2b6a, 0x0008a910, 
0x00119173, 0x00038ae5, 0x00129555, 0x000c095b, 0x000389aa, 
0x0010b157, 0x00038912, 0x00118155, 0x000828ee, 0x001ee855, 
0x001b8855, 0x001c8a5d, 0x00092924, 0x00199515, 0x0011a55e, 
0x001aa5de, 0x001b89d4, 0x001de95a, 0x001ee80d, 0x0012e8ad, 
0x0019b915, 0x001b8975, 0x0012e956, 0x001ee879, 0x000c0938, 
0x001b8516, 0x000c0935, 0x00188ab5, 0x001ee857, 0x00230d77, 
0x00098915, 0x001a8536, 0x000c0937, 0x0009b117, 0x001080f7, 
0x00178915, 0x000828b7, 0x00118655, 0x0008288b, 0x001a8536, 
0x001ab979, 0x000fa893, 0x001d8956, 0x001ee8f5, 0x00198975, 
0x00188975, 0x001c8a58, 0x00092b5c, 0x001ee875, 0x00198915, 
0x00128a56, 0x00092880, 0x00118659, 0x0018b919, 0x001b851c, 
0x001b853f, 0x0019a17f, 0x0018a17f, 0x001ee8f5, 0x001d8977, 
0x001982b7, 0x00178915, 0x001c8a58, 0x000c087c, 0x0018aed5, 
0x0018a9dd, 0x0019a9dd, 0x0017a95d, 0x0019addc, 0x0018addc, 
0x001de955, 0x0012ad5c, 0x000c081f, 0x0012a0bf, 0x0019a17f, 
0x0018a17f, 0x0019b11b, 0x0018b11b, 0x00038ae5, 0x00129555, 
0x000c0095, 0x0019b378, 0x0018beb8, 0x0019b31a, 0x0018b6ba, 
0x0019b34d, 0x0018eaad, 0x0019b1ce, 0x0018e6ce, 0x0019b10f, 
0x0018e28f, 0x001d894c, 0x001ae115, 0x000f2b4c, 0x001dff57, 
0x001b8177, 0x001281b7, 0x000c0917, 0x00198256, 0x00188656, 
0x001b8576, 0x001a8575, 0x00082b42, 0x0019834c, 0x0018eeac, 
0x00128d34, 0x0011a53e, 0x001dff4f, 0x0019e10f, 0x0012e16f, 
0x001ba554, 0x001d895e, 0x001ee8f5, 0x00188ab5, 0x001b8955, 
0x00082b76, 0x0008a98a, 0x001d9ed4, 0x00199515, 0x00128d54, 
0x0008a905, 0x000928f1, 0x000928f1, 0x001d8957, 0x001d8954, 
0x00092b19, 0x001d89f7, 0x00092b19, 0x001ee9a7, 0x001bc147, 
0x00082b19, 0x001b9945, 0x001dd144, 0x0012cd84, 0x001ac167, 
0x001d8866, 0x000889a1, 0x001ec9d1, 0x001ee825, 0x0012c985, 
0x001d8926, 0x000889a1, 0x001ee9b5, 0x00129153, 0x00092b19, 
0x001b9153, 0x00082b19, 0x001c6b5b, 0x001c635a, 0x001b9945, 
0x001dd144, 0x001a9167, 0x001d8866, 0x000889a1, 0x001ec9d1, 
0x001ee825, 0x001d8926, 0x000889a1, 0x001ee8b5, 0x00188ab5, 
0x001b8955, 0x00082b0d, 0x001ee969, 0x001ee908, 0x001ee931, 
0x001d8955, 0x001c8a5a, 0x001bf14b, 0x00082b0d, 0x000928d3, 
0x00198e54, 0x00188e54, 0x00199253, 0x00189353, 0x0019a65e, 
0x0018a45e, 0x0019ba59, 0x00178d95, 0x00178835, 0x001788d5, 
0x001c8a59, 0x001ee8f5, 0x00188ab5, 0x00198ab5, 0x00198337, 
0x00178915, 0x001c8a58, 0x001cc25b, 0x001ee8b5, 0x00188ab5, 
0x00198ab5, 0x0019a17f, 0x0018a17f, 0x00178815, 0x001c8a5a, 
0x001d8953, 0x001caa56, 0x001ca253, 0x001cba52, 0x000909fa
};

#define GCPU_IRAM0_LEN   0x9D8

// built on  2011/09/22 12:38
// add the TS Descramble with Auto-Sync command
