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
0x001c94d2, 0x001d9665, 0x001ec6a4, 0x0010d464, 0x000fb657, 
0x001ec6e5, 0x001c1554, 0x000916fa, 0x001d9655, 0x001c94e8, 
0x000916fa, 0x001ef675, 0x001df646, 0x00129435, 0x001ef614, 
0x001ef627, 0x001bde47, 0x000816fa, 0x000c1657, 0x00089a86, 
0x001b9675, 0x001b92d4, 0x001ade67, 0x000fb647, 0x000916fa, 
0x001ef674, 0x001ef615, 0x001df646, 0x00129435, 0x001ef627, 
0x001bde47, 0x000816fa, 0x00089ab6, 0x000c1e15, 0x001b9675, 
0x001b92d4, 0x001ade67, 0x000fb675, 0x000916fa, 0x001ed6a6, 
0x001bda46, 0x000fb672, 0x001ed6d1, 0x001dea45, 0x001dd984, 
0x000c1c44, 0x001dd9a5, 0x000c1f05, 0x001b8645, 0x001de644, 
0x001d91a7, 0x001d9766, 0x001c1451, 0x001c5c5b, 0x001c445a, 
0x001c1c50, 0x001c0453, 0x001d9615, 0x000c1e52, 0x001c0c52, 
0x000c16b5, 0x001b9655, 0x000fb669, 0x00118270, 0x000fb672, 
0x001ed6a6, 0x001bda46, 0x000fb614, 0x0009107d, 0x001b9254, 
0x0008361a, 0x001dce44, 0x001a9267, 0x001d9766, 0x001c745b, 
0x001c7c5a, 0x001b8645, 0x000896a1, 0x001ed6d1, 0x000e1655, 
0x001b9254, 0x0008360f, 0x001dce44, 0x001a9267, 0x001d9726, 
0x001c645b, 0x001c6c5a, 0x001b8245, 0x000896a1, 0x001ed6d0, 
0x000e1655, 0x001d9645, 0x001d9ea4, 0x001de040, 0x0011dc41, 
0x0008363b, 0x0019d581, 0x001bc641, 0x000fb630, 0x000c14e6, 
0x001bc260, 0x0019d941, 0x0018c541, 0x0011c461, 0x00083639, 
0x0018db46, 0x001bd665, 0x0009360b, 0x001d9664, 0x00093625, 
0x001d9644, 0x001ad665, 0x000e1655, 0x0019ba7c, 0x0012b33c, 
0x001ef73f, 0x000936d4, 0x001ef6b3, 0x001bb25c, 0x000836c5, 
0x001d8e54, 0x00119375, 0x000f3629, 0x001bb254, 0x00119295, 
0x000f362a, 0x001b8e54, 0x0008b610, 0x00118e73, 0x001ef735, 
0x001c945b, 0x00019415, 0x00129615, 0x001c945a, 0x001bbe45, 
0x001dce44, 0x001a9267, 0x001d9726, 0x000896a1, 0x001ed6df, 
0x0011b27c, 0x001b8e53, 0x00083479, 0x00093623, 0x0019ba75, 
0x00129735, 0x0011be5f, 0x001ef72d, 0x00039415, 0x001b9658, 
0x0011bd55, 0x000f36cc, 0x000936cf, 0x0012bfff, 0x001c545b, 
0x0012f7fb, 0x001ce45a, 0x001d965c, 0x0011b335, 0x00083691, 
0x001d967a, 0x001d9637, 0x0012bf76, 0x001a9e77, 0x000f3695, 
0x001baa5a, 0x000836ea, 0x001d9674, 0x001a9e15, 0x000fb6fa, 
0x001d8e54, 0x0011bb75, 0x00119255, 0x000f36fa, 0x0011bb74, 
0x001b9a45, 0x001dce44, 0x001a9267, 0x001d9766, 0x000896a1, 
0x001b9247, 0x0008b60e, 0x001bd25a, 0x001a9e15, 0x000fb6ee, 
0x00129856, 0x0012b05c, 0x00129b36, 0x00119b95, 0x000f36ea, 
0x00119bf6, 0x000936f6, 0x001baa5a, 0x000fb699, 0x001bb27c, 
0x000936cb, 0x0019ba75, 0x00129735, 0x0012be5f, 0x001bba5c, 
0x0011bf95, 0x000f3623, 0x0011bfff, 0x00093623, 0x001d9675, 
0x001995b5, 0x001c955a, 0x001cb556, 0x001ca552, 0x001bf649, 
0x001bae48, 0x0019ba74, 0x00129334, 0x00119373, 0x0012bf71, 
0x00198a14, 0x00128e73, 0x00118671, 0x001185d4, 0x000f3688, 
0x000937f1, 0x001287f1, 0x000937f1, 0x001d97b7, 0x0010b617, 
0x0008374a, 0x001a9f95, 0x000836bc, 0x001a9fb5, 0x000836b2, 
0x0009374a, 0x001da6d6, 0x000936bf, 0x001d9456, 0x001ab25c, 
0x000f36ab, 0x001bb245, 0x001dd744, 0x001d96c7, 0x00089699, 
0x0019b5d5, 0x001895d5, 0x001a97a5, 0x000fb6ad, 0x001bb2c5, 
0x001dd764, 0x001d96c7, 0x00089699, 0x001a9785, 0x000fb6ab, 
0x001bb2c5, 0x001dd704, 0x001d96c7, 0x00089699, 0x001d9657, 
0x0019b5d5, 0x001895d5, 0x001a9795, 0x000fb753, 0x00119215, 
0x001a9455, 0x000f3775, 0x001dd764, 0x0019b5d5, 0x001895d5, 
0x001a97b5, 0x000fb759, 0x001dd704, 0x0012fe05, 0x001d96c7, 
0x0008b761, 0x001c8c5e, 0x00089747, 0x001dd765, 0x0019b5d5, 
0x001895d5, 0x001a97b5, 0x000fb742, 0x001dd705, 0x0012fe04, 
0x001d96c7, 0x0008b710, 0x001b9c57, 0x00119215, 0x000f374a, 
0x00119215, 0x000fb6aa, 0x000e1655, 0x001ed715, 0x001c945c, 
0x001ed715, 0x001c945c, 0x001ed715, 0x001c945c, 0x001ed715, 
0x001c945c, 0x0012fe05, 0x001dd704, 0x001d96c7, 0x0008b761, 
0x001db6d5, 0x001c945e, 0x00089747, 0x001dd705, 0x0012fe04, 
0x001d96c7, 0x0008b710, 0x0009374a, 0x000d1647, 0x0019d587, 
0x0018dd27, 0x0018d605, 0x001d9240, 0x0011c000, 0x000cb716, 
0x000c1446, 0x001bd665, 0x001bde47, 0x00083716, 0x000c1441, 
0x0015d806, 0x0016c4e1, 0x0017d8c6, 0x000c1c24, 0x000e1655, 
0x000d1647, 0x0019d187, 0x0018dd27, 0x0018d204, 0x001d9240, 
0x0011c000, 0x000cb709, 0x000c1443, 0x001bde47, 0x0008370b, 
0x000c1461, 0x0016cc06, 0x0016c4e1, 0x0015c4e1, 0x0017d8c6, 
0x000c1c24, 0x001bd264, 0x000c1461, 0x0015cce3, 0x0015c401, 
0x0016c401, 0x0017ccc3, 0x000c1c84, 0x00139655, 0x000e1655, 
0x000c1c84, 0x001bd264, 0x00093709, 0x001d945d, 0x001ef669, 
0x001ef608, 0x001ef631, 0x001ef6cb, 0x001ef6ea, 0x001ef690, 
0x001ef752, 0x001ef775, 0x001ef714, 0x00188a75, 0x001989b2, 
0x001889b2, 0x001995b5, 0x001895b5, 0x001b9655, 0x000fb722, 
0x001d9659, 0x001d9655, 0x001c9558, 0x001c955a, 0x000937f6, 
0x001ef7d5, 0x00199554, 0x00189154, 0x00199453, 0x00188d53, 
0x0019975e, 0x0018b95e, 0x00189559, 0x001ef7f5, 0x00199717, 
0x00189df7, 0x001ef79c, 0x001ef7b5, 0x0019967f, 0x0018be7f, 
0x001a9e55, 0x000837f6, 0x001a9ef5, 0x00083620, 0x001b8e53, 
0x000837c4, 0x001ef725, 0x001dce44, 0x001a8e67, 0x001d9666, 
0x000896a1, 0x001ef6a5, 0x00119295, 0x0011d655, 0x000f347b, 
0x001c745b, 0x001c7c5a, 0x001b8645, 0x001dce55, 0x00129684, 
0x00119295, 0x001a9667, 0x001d9766, 0x000896a1, 0x001ed6d1, 
0x001ef6b3, 0x00118e53, 0x001a9e75, 0x000837fa, 0x001ef6b3, 
0x001b8e53, 0x0008347c, 0x00198a14, 0x000396e9, 0x0012925e, 
0x001bba54, 0x001d9677, 0x00118e75, 0x000f346a, 0x0008b610, 
0x00118e73, 0x000395e5, 0x00128a55, 0x000c165b, 0x000396aa, 
0x0010ae57, 0x00039612, 0x00119e55, 0x000837ee, 0x001ef755, 
0x001b9755, 0x001c955d, 0x00093624, 0x00198a15, 0x0011ba5e, 
0x001abade, 0x001b96d4, 0x001df65a, 0x001ef70d, 0x0012f7ad, 
0x0019a615, 0x001b9675, 0x0012f656, 0x001ef779, 0x000c1638, 
0x001b9a16, 0x000c1635, 0x001895b5, 0x001ef757, 0x00231277, 
0x00099615, 0x001a9a36, 0x000c1637, 0x0009ae17, 0x00109ff7, 
0x00179615, 0x000837b7, 0x00119955, 0x0008378b, 0x001a9a36, 
0x001aa679, 0x000fb793, 0x001d9656, 0x001ef7f5, 0x00199675, 
0x00189675, 0x001c9558, 0x0009345c, 0x001ef775, 0x00199615, 
0x00129556, 0x00093780, 0x00119959, 0x0018a619, 0x001b9a1c, 
0x001b9a3f, 0x0019be7f, 0x0018be7f, 0x001ef7f5, 0x001d9677, 
0x00199db7, 0x00179615, 0x001c9558, 0x000c177c, 0x0018b1d5, 
0x0018b6dd, 0x0019b6dd, 0x0017b65d, 0x0019b2dc, 0x0018b2dc, 
0x001df655, 0x0012b25c, 0x000c171f, 0x0012bfbf, 0x0019be7f, 
0x0018be7f, 0x0019ae1b, 0x0018ae1b, 0x000395e5, 0x00128a55, 
0x000c1f95, 0x0019ac78, 0x0018a1b8, 0x0019ac1a, 0x0018a9ba, 
0x0019ac4d, 0x0018f5ad, 0x0019aece, 0x0018f9ce, 0x0019ae0f, 
0x0018fd8f, 0x001d964c, 0x001afe15, 0x000f344c, 0x001de057, 
0x001b9e77, 0x00129eb7, 0x000c1617, 0x00199d56, 0x00189956, 
0x001b9a76, 0x001a9a75, 0x00083442, 0x00199c4c, 0x0018f1ac, 
0x00129234, 0x0011ba3e, 0x001de04f, 0x0019fe0f, 0x0012fe6f, 
0x001bba54, 0x001d965e, 0x001ef7f5, 0x001895b5, 0x001b9655, 
0x00083476, 0x0008b68a, 0x001d81d4, 0x00198a15, 0x00129254, 
0x0008b605, 0x000937f1, 0x000937f1, 0x001d9657, 0x001d9654, 
0x00093419, 0x001d96f7, 0x00093419, 0x001ef6a7, 0x001bde47, 
0x00083419, 0x001b8645, 0x001dce44, 0x0012d284, 0x001ade67, 
0x001d9766, 0x000896a1, 0x001ed6d1, 0x001ef725, 0x0012d685, 
0x001d9626, 0x000896a1, 0x001ef6b5, 0x00128e53, 0x00093419, 
0x001b8e53, 0x00083419, 0x001c745b, 0x001c7c5a, 0x001b8645, 
0x001dce44, 0x001a8e67, 0x001d9766, 0x000896a1, 0x001ed6d1, 
0x001ef725, 0x001d9626, 0x000896a1, 0x001ef7b5, 0x001895b5, 
0x001b9655, 0x0008340d, 0x001ef669, 0x001ef608, 0x001ef631, 
0x001d9655, 0x001c955a, 0x001bee4b, 0x0008340d, 0x000937d3, 
0x00199154, 0x00189154, 0x00198d53, 0x00188c53, 0x0019b95e, 
0x0018bb5e, 0x0019a559, 0x00179295, 0x00179735, 0x001797d5, 
0x001c9559, 0x001ef7f5, 0x001895b5, 0x001995b5, 0x00199c37, 
0x00179615, 0x001c9558, 0x001cdd5b, 0x001ef7b5, 0x001895b5, 
0x001995b5, 0x0019be7f, 0x0018be7f, 0x00179715, 0x001c955a, 
0x001d9653, 0x001cb556, 0x001cbd53, 0x001ca552, 0x000916fa
};

#define GCPU_IRAM0_LEN   0x9D8

// built on  2011/09/22 12:38
// add the TS Descramble with Auto-Sync command
