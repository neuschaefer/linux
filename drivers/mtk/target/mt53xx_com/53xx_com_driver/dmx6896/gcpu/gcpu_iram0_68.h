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
0x001c6ad2, 0x001d6865, 0x001e38a4, 0x00102a64, 0x000f4857, 
0x001e38e5, 0x001ceb54, 0x0009e8fa, 0x001d6855, 0x001c6ae8, 
0x0009e8fa, 0x001e0875, 0x001d0846, 0x00126a35, 0x001e0814, 
0x001e0827, 0x001b2047, 0x0008e8fa, 0x000ce857, 0x00086486, 
0x001b6875, 0x001b6cd4, 0x001a2067, 0x000f4847, 0x0009e8fa, 
0x001e0874, 0x001e0815, 0x001d0846, 0x00126a35, 0x001e0827, 
0x001b2047, 0x0008e8fa, 0x000864b6, 0x000ce015, 0x001b6875, 
0x001b6cd4, 0x001a2067, 0x000f4875, 0x0009e8fa, 0x001e28a6, 
0x001b2446, 0x000f4872, 0x001e28d1, 0x001d1445, 0x001d2784, 
0x000ce244, 0x001d27a5, 0x000ce105, 0x001b7845, 0x001d1844, 
0x001d6fa7, 0x001d6966, 0x001cea51, 0x001ca25b, 0x001cba5a, 
0x001ce250, 0x001cfa53, 0x001d6815, 0x000ce052, 0x001cf252, 
0x000ce8b5, 0x001b6855, 0x000f4869, 0x00117c70, 0x000f4872, 
0x001e28a6, 0x001b2446, 0x000f4814, 0x0009ee7d, 0x001b6c54, 
0x0008c81a, 0x001d3044, 0x001a6c67, 0x001d6966, 0x001c8a5b, 
0x001c825a, 0x001b7845, 0x000868a1, 0x001e28d1, 0x000ee855, 
0x001b6c54, 0x0008c80f, 0x001d3044, 0x001a6c67, 0x001d6926, 
0x001c9a5b, 0x001c925a, 0x001b7c45, 0x000868a1, 0x001e28d0, 
0x000ee855, 0x001d6845, 0x001d60a4, 0x001d1e40, 0x00112241, 
0x0008c83b, 0x00192b81, 0x001b3841, 0x000f4830, 0x000ceae6, 
0x001b3c60, 0x00192741, 0x00183b41, 0x00113a61, 0x0008c839, 
0x00182546, 0x001b2865, 0x0009c80b, 0x001d6864, 0x0009c825, 
0x001d6844, 0x001a2865, 0x000ee855, 0x0019447c, 0x00124d3c, 
0x001e093f, 0x0009c8d4, 0x001e08b3, 0x001b4c5c, 0x0008c8c5, 
0x001d7054, 0x00116d75, 0x000fc829, 0x001b4c54, 0x00116c95, 
0x000fc82a, 0x001b7054, 0x00084810, 0x00117073, 0x001e0935, 
0x001c6a5b, 0x00016a15, 0x00126815, 0x001c6a5a, 0x001b4045, 
0x001d3044, 0x001a6c67, 0x001d6926, 0x000868a1, 0x001e28df, 
0x00114c7c, 0x001b7053, 0x0008ca79, 0x0009c823, 0x00194475, 
0x00126935, 0x0011405f, 0x001e092d, 0x00036a15, 0x001b6858, 
0x00114355, 0x000fc8cc, 0x0009c8cf, 0x001241ff, 0x001caa5b, 
0x001209fb, 0x001c1a5a, 0x001d685c, 0x00114d35, 0x0008c891, 
0x001d687a, 0x001d6837, 0x00124176, 0x001a6077, 0x000fc895, 
0x001b545a, 0x0008c8ea, 0x001d6874, 0x001a6015, 0x000f48fa, 
0x001d7054, 0x00114575, 0x00116c55, 0x000fc8fa, 0x00114574, 
0x001b6445, 0x001d3044, 0x001a6c67, 0x001d6966, 0x000868a1, 
0x001b6c47, 0x0008480e, 0x001b2c5a, 0x001a6015, 0x000f48ee, 
0x00126656, 0x00124e5c, 0x00126536, 0x00116595, 0x000fc8ea, 
0x001165f6, 0x0009c8f6, 0x001b545a, 0x000f4899, 0x001b4c7c, 
0x0009c8cb, 0x00194475, 0x00126935, 0x0012405f, 0x001b445c, 
0x00114195, 0x000fc823, 0x001141ff, 0x0009c823, 0x001d6875, 
0x00196bb5, 0x001c6b5a, 0x001c4b56, 0x001c5b52, 0x001b0849, 
0x001b5048, 0x00194474, 0x00126d34, 0x00116d73, 0x00124171, 
0x00197414, 0x00127073, 0x00117871, 0x00117bd4, 0x000fc888, 
0x0009c9f1, 0x001279f1, 0x0009c9f1, 0x001d69b7, 0x00104817, 
0x0008c94a, 0x001a6195, 0x0008c8bc, 0x001a61b5, 0x0008c8b2, 
0x0009c94a, 0x001d58d6, 0x0009c8bf, 0x001d6a56, 0x001a4c5c, 
0x000fc8ab, 0x001b4c45, 0x001d2944, 0x001d68c7, 0x00086899, 
0x00194bd5, 0x00186bd5, 0x001a69a5, 0x000f48ad, 0x001b4cc5, 
0x001d2964, 0x001d68c7, 0x00086899, 0x001a6985, 0x000f48ab, 
0x001b4cc5, 0x001d2904, 0x001d68c7, 0x00086899, 0x001d6857, 
0x00194bd5, 0x00186bd5, 0x001a6995, 0x000f4953, 0x00116c15, 
0x001a6a55, 0x000fc975, 0x001d2964, 0x00194bd5, 0x00186bd5, 
0x001a69b5, 0x000f4959, 0x001d2904, 0x00120005, 0x001d68c7, 
0x00084961, 0x001c725e, 0x00086947, 0x001d2965, 0x00194bd5, 
0x00186bd5, 0x001a69b5, 0x000f4942, 0x001d2905, 0x00120004, 
0x001d68c7, 0x00084910, 0x001b6257, 0x00116c15, 0x000fc94a, 
0x00116c15, 0x000f48aa, 0x000ee855, 0x001e2915, 0x001c6a5c, 
0x001e2915, 0x001c6a5c, 0x001e2915, 0x001c6a5c, 0x001e2915, 
0x001c6a5c, 0x00120005, 0x001d2904, 0x001d68c7, 0x00084961, 
0x001d48d5, 0x001c6a5e, 0x00086947, 0x001d2905, 0x00120004, 
0x001d68c7, 0x00084910, 0x0009c94a, 0x000de847, 0x00192b87, 
0x00182327, 0x00182805, 0x001d6c40, 0x00113e00, 0x000c4916, 
0x000cea46, 0x001b2865, 0x001b2047, 0x0008c916, 0x000cea41, 
0x00152606, 0x00163ae1, 0x001726c6, 0x000ce224, 0x000ee855, 
0x000de847, 0x00192f87, 0x00182327, 0x00182c04, 0x001d6c40, 
0x00113e00, 0x000c4909, 0x000cea43, 0x001b2047, 0x0008c90b, 
0x000cea61, 0x00163206, 0x00163ae1, 0x00153ae1, 0x001726c6, 
0x000ce224, 0x001b2c64, 0x000cea61, 0x001532e3, 0x00153a01, 
0x00163a01, 0x001732c3, 0x000ce284, 0x00136855, 0x000ee855, 
0x000ce284, 0x001b2c64, 0x0009c909, 0x001d6a5d, 0x001e0869, 
0x001e0808, 0x001e0831, 0x001e08cb, 0x001e08ea, 0x001e0890, 
0x001e0952, 0x001e0975, 0x001e0914, 0x00187475, 0x001977b2, 
0x001877b2, 0x00196bb5, 0x00186bb5, 0x001b6855, 0x000f4922, 
0x001d6859, 0x001d6855, 0x001c6b58, 0x001c6b5a, 0x0009c9f6, 
0x001e09d5, 0x00196b54, 0x00186f54, 0x00196a53, 0x00187353, 
0x0019695e, 0x0018475e, 0x00186b59, 0x001e09f5, 0x00196917, 
0x001863f7, 0x001e099c, 0x001e09b5, 0x0019687f, 0x0018407f, 
0x001a6055, 0x0008c9f6, 0x001a60f5, 0x0008c820, 0x001b7053, 
0x0008c9c4, 0x001e0925, 0x001d3044, 0x001a7067, 0x001d6866, 
0x000868a1, 0x001e08a5, 0x00116c95, 0x00112855, 0x000fca7b, 
0x001c8a5b, 0x001c825a, 0x001b7845, 0x001d3055, 0x00126884, 
0x00116c95, 0x001a6867, 0x001d6966, 0x000868a1, 0x001e28d1, 
0x001e08b3, 0x00117053, 0x001a6075, 0x0008c9fa, 0x001e08b3, 
0x001b7053, 0x0008ca7c, 0x00197414, 0x000368e9, 0x00126c5e, 
0x001b4454, 0x001d6877, 0x00117075, 0x000fca6a, 0x00084810, 
0x00117073, 0x00036be5, 0x00127455, 0x000ce85b, 0x000368aa, 
0x00105057, 0x00036812, 0x00116055, 0x0008c9ee, 0x001e0955, 
0x001b6955, 0x001c6b5d, 0x0009c824, 0x00197415, 0x0011445e, 
0x001a44de, 0x001b68d4, 0x001d085a, 0x001e090d, 0x001209ad, 
0x00195815, 0x001b6875, 0x00120856, 0x001e0979, 0x000ce838, 
0x001b6416, 0x000ce835, 0x00186bb5, 0x001e0957, 0x0023ec77, 
0x00096815, 0x001a6436, 0x000ce837, 0x00095017, 0x001061f7, 
0x00176815, 0x0008c9b7, 0x00116755, 0x0008c98b, 0x001a6436, 
0x001a5879, 0x000f4993, 0x001d6856, 0x001e09f5, 0x00196875, 
0x00186875, 0x001c6b58, 0x0009ca5c, 0x001e0975, 0x00196815, 
0x00126b56, 0x0009c980, 0x00116759, 0x00185819, 0x001b641c, 
0x001b643f, 0x0019407f, 0x0018407f, 0x001e09f5, 0x001d6877, 
0x001963b7, 0x00176815, 0x001c6b58, 0x000ce97c, 0x00184fd5, 
0x001848dd, 0x001948dd, 0x0017485d, 0x00194cdc, 0x00184cdc, 
0x001d0855, 0x00124c5c, 0x000ce91f, 0x001241bf, 0x0019407f, 
0x0018407f, 0x0019501b, 0x0018501b, 0x00036be5, 0x00127455, 
0x000ce195, 0x00195278, 0x00185fb8, 0x0019521a, 0x001857ba, 
0x0019524d, 0x00180bad, 0x001950ce, 0x001807ce, 0x0019500f, 
0x0018038f, 0x001d684c, 0x001a0015, 0x000fca4c, 0x001d1e57, 
0x001b6077, 0x001260b7, 0x000ce817, 0x00196356, 0x00186756, 
0x001b6476, 0x001a6475, 0x0008ca42, 0x0019624c, 0x00180fac, 
0x00126c34, 0x0011443e, 0x001d1e4f, 0x0019000f, 0x0012006f, 
0x001b4454, 0x001d685e, 0x001e09f5, 0x00186bb5, 0x001b6855, 
0x0008ca76, 0x0008488a, 0x001d7fd4, 0x00197415, 0x00126c54, 
0x00084805, 0x0009c9f1, 0x0009c9f1, 0x001d6857, 0x001d6854, 
0x0009ca19, 0x001d68f7, 0x0009ca19, 0x001e08a7, 0x001b2047, 
0x0008ca19, 0x001b7845, 0x001d3044, 0x00122c84, 0x001a2067, 
0x001d6966, 0x000868a1, 0x001e28d1, 0x001e0925, 0x00122885, 
0x001d6826, 0x000868a1, 0x001e08b5, 0x00127053, 0x0009ca19, 
0x001b7053, 0x0008ca19, 0x001c8a5b, 0x001c825a, 0x001b7845, 
0x001d3044, 0x001a7067, 0x001d6966, 0x000868a1, 0x001e28d1, 
0x001e0925, 0x001d6826, 0x000868a1, 0x001e09b5, 0x00186bb5, 
0x001b6855, 0x0008ca0d, 0x001e0869, 0x001e0808, 0x001e0831, 
0x001d6855, 0x001c6b5a, 0x001b104b, 0x0008ca0d, 0x0009c9d3, 
0x00196f54, 0x00186f54, 0x00197353, 0x00187253, 0x0019475e, 
0x0018455e, 0x00195b59, 0x00176c95, 0x00176935, 0x001769d5, 
0x001c6b59, 0x001e09f5, 0x00186bb5, 0x00196bb5, 0x00196237, 
0x00176815, 0x001c6b58, 0x001c235b, 0x001e09b5, 0x00186bb5, 
0x00196bb5, 0x0019407f, 0x0018407f, 0x00176915, 0x001c6b5a, 
0x001d6853, 0x001c4b56, 0x001c4353, 0x001c5b52, 0x0009e8fa
};

#define GCPU_IRAM0_LEN   0x9D8

// built on  2011/09/22 12:38
// add the TS Descramble with Auto-Sync command
