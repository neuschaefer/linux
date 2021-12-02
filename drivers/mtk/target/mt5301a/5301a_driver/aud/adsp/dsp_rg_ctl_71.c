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
 * $RCSfile: dsp_rg_ctl_71.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#define DSP_RGCTL_C
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "dsp_intf.h"
#include "dsp_reg.h"
#include "dsp_data.h"
#include "dsp_rg_ctl.h"
#include "dsp_func.h"
#include "dsp_shm.h"

#include "aud_if.h"
#include "aud_debug.h"
#include "drv_adsp.h"
#include "d2rc_shm.h"
#include "aud_hw.h"

#include "x_os.h"
#include "x_hal_926.h"
LINT_EXT_HEADER_END

static BOOL _afgSpdifOutPort[AUD_DEC_NUM] = {TRUE, TRUE};

void vResetAndHoldDSP (void);
void vTriggerDSP (void);
void vSetDspBootFromRam (void);

#if (0) // CORDIC test
UINT32 u4CordicTest[]={
        0x04dba77,0x0200000,0x012e405,0x009fb38,0x0051112,0x0028b0d,0x00145d8,0x000a2f6,
        0x000517c,0x00028be,0x000145f,0x0000a30,0x0000518,0x000028c,0x0000146,0x00000a3,
        0x0000051,0x0000029,0x0000014,0x000000a,0x0000005,0x0000003,0x0000001,0x0000001,
        0x0000000,0x0800000,0x0000000,0x0400000,0x0000000,0x0800000,0x0ffffff,0x0000000,
        0x0000001,0x0000000,0x0ffffff,0x0000001,0x0ffffff,0x0000001,0x0ffffff,0x0000001,
        0x0800000,0x0800000,0x07fffff,0x07fffff,0x0800000,0x07a3fda,0x05609a3,0x0cbad6e,
        0x0c19000,0x03e4247,0x082e3bc,0x003f9f3,0x08a8c71,0x0af0284,0x07f98c9,0x08ad23a,
        0x080000f,0x0814f37,0x0874606,0x0196c18,0x0ddd7fc,0x0d0fe70,0x03f8127,0x02d23f4,
        0x08adbc6,0x0c75fd3,0x03da2af,0x0359430,0x098861f,0x0719f6b,0x02cd293,0x0f51e5f,
        0x08227c2,0x016f656,0x07fc18c,0x086e0c8,0x0eb6d33,0x07eaf84,0x0b247de,0x07993fc,
        0x09bf87e,0x0c79dde,0x0dbd9b2,0x0289a02,0x07c6ff9,0x0941589,0x0792878,0x07c5ce6,
        0x0ef970d,0x05a4931,0x07347c7,0x0bce817,0x0c768af,0x0cee126,0x0fa125a,0x0da5afb,
        0x08b3c5e,0x0454d2e,0x0f1bb7e,0x06c19a5,0x07ff3f1,0x09ebc20,0x076d825,0x07f8619,
        0x0ac77d1,0x07d8b77,0x0b1b8d4,0x0765bd6,0x0a2571e,0x08d2895,0x0520948,0x090154d,
        0x0beaff9,0x0866d90,0x09f7ce3,0x045fbbf,0x06da039,0x08df459,0x08ebeac,0x05a60fd,
        0x0898988,0x082e63e,0x0e9d770,0x0e32e41,0x083edda,0x0eb0bbb,0x00b0dea,0x080517e,
        0x06a349d,0x07f803e,0x05e02a9,0x0752c95,0x0c89e5d,0x0cacbdf,0x09be604,0x0460753,
        0x0c1807b,0x08009b2,0x09386b2,0x05ec33c,0x0de2f98,0x09a8a28,0x0728807,0x052f262,
        0x055955e,0x08a0160,0x091b612,0x0be6a5d,0x0382324,0x0aec562,0x0af9949,0x0cc0d0e,
        0x04812e6,0x07e2f35,0x07d987d,0x0fbe772,0x0173484,0x09712d9,0x064e4db,0x087fa6b,
        0x07ffe6e,0x048b656,0x07a46de,0x0264ab3,0x09375ee,0x0c9c022,0x0934f41,0x0d342b6,
        0x075e1d5,0x0d22b1d,0x0c0ed9a,0x0f69139,0x0ddbf3c,0x086d54d,0x0615e2e,0x08ee0cd,
        0x0ae0cb8,0x086c23f,0x07f5db6,0x0acf04b,0x002ecc0,0x0b57001,0x09bbc35,0x00e64e6,
        0x0eb07d6,0x0bfb896,0x0419165,0x0e21112,0x0799ddb,0x062ad18,0x0719823,0x0daca71,
        0x0785f1c,0x06639e1,0x07fdd1a,0x0c25c73,0x074afc5,0x0d766ee,0x0b811da,0x0d34dd1,
        0x09f452c,0x04528a0,0x0ddc8c1,0x063f0b0,0x0d7a4c7,0x07ebbd3,0x07bc108,0x065584e,
        0x080bd34,0x0f348f3,0x0c2cd1d,0x0677a76,0x06e4167,0x082b0bf,0x0dc799b,0x0a59cea,
        0x099dc22,0x07090b4,0x0ddeced,0x07ae7f6,0x0b4ffc6,0x08b690c,0x082cf6c,0x0ec57f0,
        0x02cfd1f,0x0afe013,0x05f3bb8,0x01f17f5,0x00f25d5,0x02c132b,0x08e44e3,0x07f95a0,
        0x0173675,0x0c2e725,0x04a1471,0x072b4e0,0x080b3fe,0x0354a96,0x07f2818,0x0356e80,
        0x04729e0,0x092ee54,0x07c27a6,0x0e5c34a,0x0826815,0x0ac4379,0x08b788f,0x056570c,
        0x08604b9,0x03f186a,0x00bf8ad,0x080b53c,0x07a869d,0x07d1195,0x01e1461,0x0ac318c,
        0x057b3a8,0x08117d5,0x0934b15,0x0808605,0x05ed879,0x0184108,0x0ba01c5,0x07e0db3,
        0x04da15d,0x0aebf47,0x087a747,0x0a34487,0x0511530,0x0ca704d,0x00d2a07,0x0767c14,
        0x0b8cd18,0x0907bd2,0x07c8c97,0x0ed817c,0x081a75f,0x0876fd1,0x036766b,0x0835000,
        0x0000000,0x0000000,0x0800000,0x0000000,0x0400000,0x0800000,0x0000000,0x0ffffff,
        0x0000000,0x0000001,0x0ffffff,0x0000001,0x0800000,0x0800000,0x07fffff,0x07fffff,
        0x0ffffff,0x0000001,0x0ffffff,0x0000001,0x0800000,0x080bae1,0x051f58f,0x0867b30,
        0x0cb71f5,0x0797453,0x0fcdcf1,0x0ea0624,0x06a3fa3,0x09e9314,0x09b133e,0x09d85f2,
        0x0748220,0x0899644,0x080205a,0x0250ecd,0x068383a,0x01d8bf9,0x07e3bb9,0x0779e2d,
        0x07f9850,0x0c0b857,0x02af477,0x07ad6cd,0x0825a6f,0x052aa13,0x02158a7,0x09eaa28,
        0x07ff54c,0x0697880,0x07c2448,0x0324cb8,0x09e0502,0x083a87d,0x08fcbca,0x092571c,
        0x06ddf42,0x0ab57e7,0x001869b,0x05596d6,0x08dbc0d,0x0b8f972,0x0163b0c,0x015ef28,
        0x082cf6c,0x055738f,0x028d9b0,0x07736d3,0x07c663a,0x08c4b5d,0x07b8d51,0x08fe0bb,
        0x01ce408,0x08f8841,0x05a1f25,0x095fc8e,0x0ed817c,0x0ee39ef,0x0c7d86c,0x0a2aed4,
        0x04254a5,0x0da91c0,0x0a01083,0x01abd3d,0x0e41fad,0x00e202f,0x07b9929,0x08cdccf,
        0x03fa6e0,0x01c6abb,0x0116729,0x08054df,0x099fc51,0x07e2b39,0x07eb7a7,0x07801ce,
        0x081d547,0x0e7c7b3,0x061e230,0x0c48543,0x0bc10b9,0x0794c9c,0x050d363,0x0901178,
        0x071c3b8,0x0afe951,0x01cfb1f,0x09edaf9,0x08b5f45,0x0437027,0x0811961,0x077bae9,
        0x087364d,0x0802f19,0x080e12c,0x0cfdfbe,0x092a633,0x0315679,0x0ac2121,0x0c9b008,
        0x0201b02,0x084ddc1,0x09b0987,0x0780aba,0x033373e,0x07ffe2b,0x0a1a05f,0x04af906,
        0x0bdff11,0x0ee6306,0x030287f,0x0adca05,0x07e2939,0x0917320,0x02b89ce,0x010a89e,
        0x028e12d,0x0357945,0x0ead339,0x08dc165,0x07ee80a,0x0b0c249,0x080ff13,0x099c6b8,
        0x0b8cd18,0x0b2b912,0x031da2e,0x002a1b2,0x035823e,0x08a9772,0x09ee168,0x0230268,
        0x0d64643,0x053acd6,0x0cdf770,0x0830b31,0x00882a8,0x07e5364,0x06ef4af,0x05ba847,
        0x07fffdf,0x0c87013,0x04567bf,0x0a8bf74,0x0878fcc,0x067a443,0x01fac0a,0x0335265,
        0x02bab3d,0x0f22f59,0x04461d2,0x0e30f76,0x078e8a0,0x07c2d68,0x084ddc1,0x0c06969,
        0x0aba808,0x0af2bd9,0x0152dc0,0x08a58d9,0x071483b,0x02b8617,0x08a5b2e,0x0ccc7da,
        0x07e396e,0x07ff991,0x0c05d69,0x083edda,0x0cdfce5,0x0089074,0x0286949,0x012a695,
        0x076008a,0x0aecd05,0x0d0fe70,0x0785b14,0x0801d5a,0x013f90f,0x0ccea42,0x080b0b3,
        0x0b8adea,0x05949e8,0x07ff623,0x0d73db8,0x07a8177,0x057e319,0x07ff358,0x0fcf69d,
        0x01bb301,0x0ed9cdb,0x0ee2854,0x04535ec,0x07215f6,0x0313cf3,0x0760cc5,0x004c004,
        0x0d5f7db,0x05adc35,0x072345b,0x0a38342,0x05e4aee,0x02fd440,0x057dbeb,0x0843526,
        0x080d71d,0x0f549ae,0x0416c0e,0x0b9170a,0x05ec490,0x007917d,0x0901923,0x06b36c5,
        0x0c7ca39,0x0b92922,0x0af0bb2,0x072aa59,0x0efac9a,0x034f450,0x07fd10f,0x0b4f62b,
        0x076d76a,0x0dbbb61,0x016c9a1,0x07cd882,0x07b2537,0x07ff572,0x08204d4,0x07a8979,
        0x041602a,0x0c19373,0x07b7918,0x0c3aa7b,0x0de9ec4,0x0a4f539,0x05c94a1,0x0b31e83,
        0x0400000,0x0800000,0x0c00000,0x0000000,0x0400000,0x0a00000,0x0800000,0x0c00000,
        0x0000000,0x0400000,0x0a00000,0x0200000,0x0bffffe,0x0c00002,0x0400002,0x03ffffe,
        0x0800002,0x07ffffe,0x0fffffe,0x0000002,0x0a00000,0x0df2e28,0x01f02d8,0x0af6f36,
        0x09c81ca,0x02cb348,0x081057a,0x0c74b16,0x06209e8,0x0a3be20,0x0e4bbe8,0x09c79b6,
        0x061e9da,0x09e9fe0,0x0a12bc8,0x0278070,0x04ce754,0x06922da,0x02d0262,0x0314c36,
        0x05e42c2,0x0a2423e,0x018cc82,0x02f3e0c,0x0a3ee26,0x019a05c,0x01a0f8c,0x0bb76c6,
        0x05fa924,0x0374412,0x01f6a5a,0x06ff680,0x0b7913c,0x0e06162,0x0a75174,0x0e2191e,
        0x05e1740,0x0a80f8e,0x07e4806,0x02df3f6,0x0e1bc34,0x097b766,0x00764c6,0x0071ce8,
        0x0bab086,0x01ee124,0x00de012,0x054e2c4,0x0516512,0x0afa47c,0x041f41a,0x0b2cd62,
        0x0761e0c,0x0d68010,0x04665b4,0x0e065a6,0x0fa26c6,0x0875d32,0x0ee03ec,0x0e642d8,
        0x064a7fc,0x0f431be,0x0a41d96,0x0090d7e,0x08bc958,0x07b0374,0x0282020,0x0a093f2,
        0x060864e,0x076a4f4,0x078bb32,0x0d46e22,0x0e176f8,0x05df20a,0x05db77c,0x025b34a,
        0x0a14860,0x087caae,0x049120a,0x0ad9de6,0x094692c,0x046f83a,0x03a7642,0x09d5362,
        0x021664c,0x0e92614,0x00c2f00,0x0e3cbf6,0x0adefe4,0x05b36fc,0x0a4c9b6,0x02a6fe6,
        0x0ac8ba4,0x09ffa08,0x0a337d2,0x0ecda9e,0x0b3c7f0,0x06d8f1e,0x0e63f3e,0x0e8612c,
        0x00e9f5e,0x0a0de44,0x09e33d6,0x0545f8a,0x01e222a,0x0570a5e,0x0a34016,0x058b046,
        0x0e1ca3e,0x0fa5970,0x00eeb10,0x0bdf8c0,0x03896ba,0x0a1102c,0x010992c,0x07a617c,
        0x00c9884,0x019d5ea,0x0f9037a,0x0cd2d40,0x05cd3fa,0x0a78a62,0x0a328ce,0x0af30fc,
        0x0e9dcb8,0x0aa3072,0x064bf84,0x074ea34,0x057332e,0x09f5bb4,0x0e00d52,0x073c57a,
        0x0933022,0x0676322,0x0f0bfcc,0x0a818f4,0x032833e,0x055b998,0x05df082,0x039a740,
        0x0469db4,0x09d0aac,0x0212874,0x0b288ec,0x0e032c6,0x020ffee,0x00b1422,0x0598f2c,
        0x00e2df8,0x0fa86e6,0x0140262,0x091e272,0x020b954,0x04cdfce,0x0aa7588,0x0a709a4,
        0x09d379e,0x0dcd614,0x0696aea,0x0dcb0ce,0x04df192,0x00d7a72,0x0e107f2,0x0ecef0e,
        0x0602aa8,0x04408ee,0x0a0cb8a,0x0dc52a4,0x0eea644,0x07d3840,0x05d61c2,0x077b58c,
        0x05d11be,0x0e689ee,0x0a671a8,0x01f92a6,0x0aa639e,0x079165c,0x08fc91a,0x0b9c22e,
        0x0d6edc2,0x05dcc58,0x025eea2,0x0da8d52,0x03afccc,0x02d0f86,0x05d9b06,0x0ff07cc,
        0x02394c8,0x08be91c,0x0f65ec0,0x0161e98,0x0623a4a,0x01e63de,0x01e7cce,0x0039cd4,
        0x0ea4356,0x063b358,0x01e4cd6,0x0b4bcd2,0x065c2da,0x06adb04,0x065ad9a,0x0d8d04a,
        0x0a0d8c0,0x0f925e0,0x038a040,0x094b544,0x01ad2c2,0x0027668,0x0cab2fa,0x05b0838,
        0x0e8c702,0x094be46,0x09a1486,0x0622756,0x0f90e0a,0x02e8024,0x05469a4,0x0ea1f6c,
        0x0286c92,0x0911ae8,0x078601c,0x05a0790,0x028469c,0x05026d6,0x0c43df6,0x020af5a,
        0x061bc76,0x094cacc,0x01fd2bc,0x0b3e1a0,0x08a8614,0x09a5a32,0x02a55a4,0x0968310,
        0x0000000,0x01fffff,0x01fffff,0x00fffff,0x00fffff,0x02d413c,0x0000000,0x0000000,
        0x0000000,0x0000000,0x0000000,0x0000000,0x01fffff,0x01fffff,0x01fffff,0x01fffff,
        0x01fffff,0x01fffff,0x01fffff,0x01fffff,0x02d413c,0x02c1e3e,0x01db4e8,0x021137b,
        0x0146724,0x0221ed8,0x01f4994,0x005954d,0x0279830,0x01fac5a,0x028abe8,0x0264416,
        0x02b4562,0x02b5a71,0x02bf701,0x00b3c23,0x01b6b22,0x00de158,0x02353a5,0x01ff684,
        0x02b4d9b,0x0153aa8,0x012c81f,0x02180fc,0x028b158,0x02320be,0x00df770,0x0187c44,
        0x02cde34,0x01afc3e,0x02c88c3,0x020c9a9,0x019077e,0x02c60b3,0x0221f79,0x028ee84,
        0x025257d,0x0196db9,0x0090ba2,0x017aecc,0x02a3c42,0x0204c42,0x01ecb90,0x01f9211,
        0x01f90af,0x01f13f8,0x01e9377,0x0223309,0x0222aa8,0x01f6cd4,0x01eec6c,0x01d9177,
        0x01e1241,0x02106b5,0x016cf9e,0x025da23,0x0205215,0x018b80b,0x020dc5c,0x027814b,
        0x01aaa89,0x020c060,0x01ef465,0x01e55db,0x0186e16,0x01ced3e,0x0251621,0x0282455,
        0x016ccbf,0x01f3656,0x0188464,0x02465dc,0x0256fef,0x02a84b2,0x02a7cfe,0x0258eed,
        0x02b4429,0x01fdb1b,0x019170e,0x01085fc,0x0235cef,0x01ec621,0x0146500,0x02a7310,
        0x026e895,0x025a457,0x0189811,0x0260d39,0x02046fb,0x015797d,0x028684b,0x022ad2c,
        0x021ffef,0x02d3733,0x029c71f,0x01a921b,0x01c9d68,0x01c347f,0x0237d3a,0x018c95e,
        0x016da10,0x02aa2b3,0x0256176,0x0223287,0x012ff58,0x025e5ec,0x01efea3,0x016cd8f,
        0x0186eb5,0x01fda17,0x021a0c4,0x0149405,0x02011c0,0x0261aae,0x01b78c6,0x01e4b07,
        0x0219744,0x0169083,0x01f0630,0x01e1f6c,0x029bf6d,0x018020c,0x029ca71,0x01be577,
        0x0226dc6,0x0167622,0x0141942,0x00272bf,0x00fe21e,0x02a2e05,0x0226180,0x01d9a96,
        0x016fd84,0x024d3ff,0x02235d2,0x02582d3,0x0023fee,0x024ac1d,0x02562fd,0x01731f6,
        0x0206d2b,0x0153db7,0x017dea7,0x0170f9b,0x02aca48,0x023c69d,0x01d7b53,0x00fd90a,
        0x0200311,0x019c9f1,0x024400d,0x011060b,0x02a0208,0x020a94d,0x023a6a7,0x0136e68,
        0x0201598,0x01a982b,0x00a0f5c,0x02697b6,0x01e1060,0x0217ff0,0x02aafea,0x01c635f,
        0x02ccf87,0x02026b7,0x0161269,0x02863c8,0x01e445b,0x01f6685,0x00d7396,0x01712a3,
        0x0270453,0x022b2ec,0x00e83aa,0x02b0161,0x025105f,0x01d926f,0x021cd88,0x020345e,
        0x01514c4,0x01dfdfb,0x027e0a7,0x00cd0f5,0x01edc12,0x0189473,0x02acc2b,0x01fe7b7,
        0x00908f0,0x00ff374,0x0130cb0,0x0217e04,0x02abc14,0x0122393,0x02b606d,0x00d691b,
        0x014a98f,0x0237d36,0x02a2c4e,0x01808c0,0x0274338,0x0181bbd,0x0247c1b,0x025bb5e,
      	0x02c0d47,0x00fffe3,0x010a088,0x0246d75,0x026b97f,0x01f5309,0x01cf7f0,0x0220541,
      	0x01a0acf,0x02455ba,0x021e27a,0x02add6a,0x0180f62,0x00e8fa5,0x0246e79,0x024aca8,
      	0x0237cc9,0x0163eb3,0x01e9f09,0x026e121,0x024dc4c,0x022adda,0x01faab1,0x02a9cef,
      	0x0182a45,0x01ff326,0x02bd84f,0x00fc6b7,0x020ab98,0x025c4e7,0x01ada5d,0x0249efd,
      	0x0000000,0x0ffff00,0x000ff00,0x0ff0000,0x0555500,0x0aaaa00,0x0666600,0x0999900,
      	0x0333300,0x0cccc00,0x00f0f00,0x006aa00,0x0065a00,0x0056a00,0x0059a00,0x002aa00,
      	0x0025a00,0x0016a00,0x0019a00,0x006a600,0x0065600,0x0056600,0x0059600,0x002a600,
      	0x0025600,0x0016600,0x0019600,0x0555500,0x0665300,0x0569500,0x065a300,0x0556500,
      	0x0666300,0x05aa500,0x069a300,0x0595500,0x06a5300,0x05a9500,0x0000000,0x007ff00,
      	0x006aa00,0x0015500,0x007c000,0x0003f00,0x0026600,0x0059900,0x004cc00,0x0033300,
      	0x000f800,0x0070700,0x0000000,0x0015500,0x006aa00,0x007ff00};
#endif

BOOL AUD_IsSpdifOutPortOn(UINT8 u1DecId)
{	
    return _afgSpdifOutPort[u1DecId];
}

/*
 * 1. Start block registers
 * 2. putbits registers
 * 3. compact mode blk registers
 * 4. Instruction cache registers
 */
void IniBlkRegs (void)
{
    UINT32 dwBlkData;

    dwBlkData = (_u4DspDramBuf[0] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspDramBuf[1] << 8) & 0xffff0000);
    WriteREG (RW_DSP_SBLK01, dwBlkData);

    dwBlkData = (_u4DspDramBuf[2] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspDramBuf[3] << 8) & 0xffff0000);
    WriteREG (RW_DSP_SBLK23, dwBlkData);

    dwBlkData = (_u4DspCmptBuf[0] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspCmptBuf[1] << 8) & 0xffff0000);
    WriteREG (RW_DRAM_CMPT_SBLK01, dwBlkData);  // Temp hack for cmpt mode

    dwBlkData = (_u4DspCmptBuf[2] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspCmptBuf[3] << 8) & 0xffff0000);
    WriteREG (RW_DRAM_CMPT_SBLK23, dwBlkData);  // Temp hack for cmpt mode

    dwBlkData = _u4DspICacheAddr[0] & 0xffff;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[1] << 16);
    WriteREG (RW_DSP_IC_DRAM_SBLK01, dwBlkData);

    dwBlkData = ((UINT32) (_u4DspICacheAddr[2])) & 0xffff;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[3] << 16);
    WriteREG (RW_DSP_IC_DRAM_SBLK23, dwBlkData);

    dwBlkData = ((UINT32) (_u4DspICacheAddr[4])) & 0xffff;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[5] << 16);
    WriteREG (RW_DSP_IC_DRAM_SBLK45, dwBlkData);

#if (0) // Putbits test
    {
    UINT32 _u4DspPutbBufAddr = PHYSICAL((UINT32)u4GetDspWorkBuf());
    UINT32 _u4DspPutbBufSta, _u4DspPutbBufEna;
    
    _u4DspPutbBufSta = (_u4DspPutbBufAddr + DSP_PUTBITS_SA) >> 8;
    _u4DspPutbBufEna = (_u4DspPutbBufAddr + DSP_PUTBITS_SA+DSP_PUTBITS_SZ) >> 8;
    dwBlkData = _u4DspPutbBufEna & 0xffff;
    dwBlkData = dwBlkData | (_u4DspPutbBufSta << 16);
    WriteREG (RW_PUTBS_BLK0_DSP, dwBlkData);
    WriteREG (RW_PUTBS_BLK1_DSP, dwBlkData);    
    }
#endif
}

/***************************************************************************
     Function : fgDspBusy
  Description : Check DSP Status
    Parameter : None
    Return    : fgCkRzt
                   TRUE: DSP Busy
                  FALSE: DSP Not Busy
***************************************************************************/
BOOL fgDspBusy (void)
{
    return (ReadREG (RO_R2D_STA) & DSP_B);
}

/***************************************************************************
     Function : vDSPSendInt
  Description : Send Interrupt to DSP
    Parameter : None
    Return    : None
***************************************************************************/
void vDspSendInt (void)
{
    static INT32 i4IntCnt = 0;
    
    LOG(9, "vDspSendInt (%d)\n",i4IntCnt);
    i4IntCnt ++;
    WriteREG (RW_LD, _u4DspSIntLD); // long data
    SendDSPInt (_uDspSIntAddr, _u4DspSIntSD);   // short data & interrupt
    _fgDspSInt = FALSE;
}

/***************************************************************************
     Function : dwReadDspLongData
  Description : Read Long data from DSP
    Parameter : None
    Return    : None
***************************************************************************/
UINT32 dwReadDspLongData ()
{
    return ReadREG (RO_D2R_RD);
}

/***************************************************************************
     Function : vSendDspLongInt
  Description : Send Interrupt to DSP
    Parameter : None
    Return    : None
***************************************************************************/
void vSendDspLongInt (UINT32 u4Addr, UINT32 u4ShortD, UINT32 u4LongD)
{
    WriteREG (RW_LD, u4LongD);  // long data
    SendDSPInt (u4Addr, u4ShortD);  // short data & interrupt
}

/***************************************************************************
     Function : vCloseIec
  Description : 
    Parameter : None
    Return    : None
***************************************************************************/
void vCloseIec (void)
{
    UINT32 dwCurrAUDCFG;

    dwCurrAUDCFG = ReadREG (RW_AUD_CFG);
    WriteREG (RW_AUD_CFG, dwCurrAUDCFG | IEC_OFF);  // bit 13
}
void vCloseIecDec2 (void)
{
    UINT32 dwCurrAUDCFG;

    dwCurrAUDCFG = ReadREG (RW_AUD_CFG);
    WriteREG (RW_AUD_CFG, dwCurrAUDCFG | IEC_OFF_DEC2); // bit 21
}

/***************************************************************************
     Function : vOpenIec
  Description : 
    Parameter : None
    Return    : None
***************************************************************************/
void vOpenIec (void)
{
    UINT32 dwCurrAUDCFG;

    dwCurrAUDCFG = ReadREG (RW_AUD_CFG);
    WriteREG (RW_AUD_CFG, dwCurrAUDCFG & ~IEC_OFF); // bit 13
}
void vOpenIecDec2 (void)
{
    UINT32 dwCurrAUDCFG;

    dwCurrAUDCFG = ReadREG (RW_AUD_CFG);
    WriteREG (RW_AUD_CFG, dwCurrAUDCFG & ~IEC_OFF_DEC2);    // bit 21
}

/***************************************************************************
     Function : vSetIecDownSample
  Description : 
    Parameter : None
    Return    : None
***************************************************************************/
void vSetIecDownSample (UINT32 u4DownSample)
{
    UINT32 dwCurrAUDCFG;

    dwCurrAUDCFG = ReadREG (RW_AUD_CFG);
    dwCurrAUDCFG &= ~IEC_DOWNSAMPLE_MASK;
    dwCurrAUDCFG |= u4DownSample << 14;
    WriteREG (RW_AUD_CFG, dwCurrAUDCFG);
}

void vSetIecDownSampleDec2 (UINT32 u4DownSample)
{
    UINT32 dwCurrAUDCFG;

    dwCurrAUDCFG = ReadREG (RW_AUD_CFG);
    dwCurrAUDCFG &= ~IEC_DOWNSAMPLE_MASK_DEC2;
    dwCurrAUDCFG |= u4DownSample << 22;
    WriteREG (RW_AUD_CFG, dwCurrAUDCFG);
}

void vSetSpdifOutPort(UINT8 u1DecId, BOOL fgEnable)
{
    if (u1DecId == AUD_DEC_MAIN)
    {
        _afgSpdifOutPort[AUD_DEC_MAIN] = fgEnable;
    }
    else
    {
        _afgSpdifOutPort[AUD_DEC_AUX] = fgEnable; 
    }
}

/***************************************************************************
     Function : vSetAudCfg (Only for first decoder)
  Description : 
    Parameter : None
    Return    : None
***************************************************************************/
/*lint -e{818}*/
void vSetAudCfg (AUDIO_CFG_T * prAudCfg, BOOL fgFirstDecoder)
{
    UINT32 dwCurrAUDCFG;
    UINT32 dwTargetAUDCFG;
    //UINT32 dwMicConfig;
    UINT32 u4DownSample, dwChanSelect;
    UINT32 dwIecOutBitnum;
    BOOL fgIecMute;
    //UINT32 dwSACDChNum;

    if (prAudCfg == NULL)
    {
        return;
    }
    u4DownSample = (UINT32) prAudCfg->u2IecDownSamp;
    dwChanSelect = (UINT32) prAudCfg->u2IecOutCh;
    fgIecMute = ((prAudCfg->fgIecMute) || 
    	         (!AUD_IsSpdifOutPortOn((fgFirstDecoder ) ? AUD_DEC_MAIN : AUD_DEC_AUX)));
/*
    Printf("fgIecMute (%x) =  (%x) || (%x)\n", fgIecMute, prAudCfg->fgIecMute,
    	    (!AUD_IsSpdifOutPortOn((fgFirstDecoder == TRUE) ? AUD_DEC_MAIN : AUD_DEC_AUX)));
*/    	    
    dwIecOutBitnum = (UINT32) prAudCfg->u2IecOutBitNum;

    dwCurrAUDCFG = ReadREG (RW_AUD_CFG);

    if (fgFirstDecoder)
    {                           // first decoder
        // Set RW_AUD_CFG
        dwTargetAUDCFG = (dwCurrAUDCFG & 0xffff00FF) |
            (dwIecOutBitnum << 8) |
            (dwChanSelect << 10) | (fgIecMute << 13) | (u4DownSample << 14);
        if (dwTargetAUDCFG == dwCurrAUDCFG)
        {
            return;
        }

        // First signal off, then change config
        // Comment out by sammy 20040315 because off IEC only when
        // sampling rate change
        //WriteREG(RW_AUD_CFG,dwCurrAUDCFG   | 0x1000 );
        //WriteREG(RW_AUD_CFG,dwTargetAUDCFG | 0x1000 ); // off IEC

        // 20040316 sammy modified, if we change downsample, the amplifier will
        // unlock
        if (((dwCurrAUDCFG & IEC_DOWNSAMPLE_MASK) >> 14) != u4DownSample)
        {
            vCloseIec ();
        }
        WriteREG (RW_AUD_CFG, dwTargetAUDCFG);
    }
    else
    {                           // second decoder
        // Set RW_AUD_CFG
        dwTargetAUDCFG = (dwCurrAUDCFG & 0xFF00ffff) |
            (dwIecOutBitnum << 16) |
            (dwChanSelect << 18) | (fgIecMute << 21) | (u4DownSample << 22);
        if (dwTargetAUDCFG == dwCurrAUDCFG)
        {
            return;
        }
        // First signal off, then change config
        // Comment out by sammy 20040315 because off IEC only when
        // sampling rate change
        //WriteREG(RW_AUD_CFG,dwCurrAUDCFG   | 0x1000 );
        //WriteREG(RW_AUD_CFG,dwTargetAUDCFG | 0x1000 ); // off IEC
        // 20040316 sammy modified, if we change downsample, the amplifier will
        // unlock
        if (((dwCurrAUDCFG & IEC_DOWNSAMPLE_MASK_DEC2) >> 22) != u4DownSample)
        {
            vCloseIecDec2 ();
        }
        WriteREG (RW_AUD_CFG, dwTargetAUDCFG);
    }
}


/***************************************************************************
     Function : vFlushDspICache
  Description : Directly flush the instruction cache.
                If we need to take care the timing issue.
                We run it in the flush state machine.
                See DspState.c
    Parameter : fgWaitDsp : No use
    Return    :
***************************************************************************/
void vFlushDspICache (BOOL fgWaitDsp)
{
    UINT32 dwTmp;
    UINT32 dwDramStatus;
    UINT32 dwCounter = 0;

    dwTmp = ReadREG (RW_ADSP_CFG);
    WriteREG (RW_ADSP_CFG, dwTmp & ~ADSP_CFG_ICACHE_FLUSH);
    WriteREG (RW_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH);
    do
    {
        dwDramStatus = ReadREG (RW_ADSP_CFG) & ADSP_CFG_ICACHE_FLUSH;
        dwCounter++;
        SLEEP (0);
    }
    while ( (dwDramStatus != 0) && (dwCounter < 5) );
    
    UNUSED(fgWaitDsp);
}

void HdStpDsp (void)
{
    vResetAndHoldDSP ();
}

/***************************************************************************
     Function : vSetCacheSize
       Author : sammy
  Description : Set cache size 
    Parameter : bSize
    Return    : 
***************************************************************************/
void vSetCacheSize (UINT8 bSize)
{
    UINT32 dwTmp;

    dwTmp = ReadREG (RW_ADSP_CFG);
    dwTmp = dwTmp & (~ADSP_CACHE_SETTING);
    dwTmp =
        dwTmp | ((bSize & ADSP_CACHE_SETTING_MASK) << ADSP_CACHE_SETTING_POS);
    WriteREG (RW_ADSP_CFG, dwTmp);
}

/***************************************************************************
     Function : vSetCacheInstLen
       Author : sammy
  Description : Set cache buffer length 
    Parameter : bSize
    Return    : 
***************************************************************************/
void vSetCacheInstLen (UINT8 bSize)
{
    UINT32 dwTmp;

    dwTmp = ReadREG (RW_ADSP_CFG);
    dwTmp = dwTmp & (~ADSP_CACHE_INST_SIZE);
    dwTmp =
        dwTmp | ((bSize & ADSP_CACHE_INST_SIZE_MASK) <<
                 ADSP_CACHE_INST_SIZE_POS);
    WriteREG (RW_ADSP_CFG, dwTmp);
}

/***************************************************************************
     Function : vSetCmptRDThreshold
       Author : Light
  Description : Set DSP compact read pre-fetch threshold
    Parameter : bThreshold
    Return    : 
***************************************************************************/
void vSetCmptRDThreshold(UINT8 bThreshold)
{
    UINT32 dwTmp;

    dwTmp = ReadREG (RW_ADSP_CFG);
    dwTmp = dwTmp & (~ADSP_CMPTRD_THRES);
    dwTmp =
        dwTmp | ((bThreshold & ADSP_CMPTRD_THRES_MASK) <<
                 ADSP_CMPTRD_THRES_POS);
    WriteREG (RW_ADSP_CFG, dwTmp);
}

/***************************************************************************
     Function : HdRstDsp
       Author : sammy
  Description : Reset DSP hardware 
                set cache size, flush cache, load rom code if needed
    Parameter : 
    Return    : 
***************************************************************************/
void HdRstDsp (void)
{
    // Enable DSP clock
    WriteREG(RW_ADSP_CFG, ADSP_CFG_CLK_ENA); 

    //Reset Dsp
    // First reset and hold dsp
    vResetAndHoldDSP ();
    // Set cache size
    //vSetCacheSize (CACHE_256);
    vSetCacheSize (CACHE_1K);
    // set instruction buffer size
    vSetCacheInstLen (SIZE32X64);
    // set compact read threshold
    vSetCmptRDThreshold(THRES_UNFULL);


    if (BSP_IsFPGA())
    {
        //Rice: load ROM code into FPGA SRAM
        UINT32 dwOfst;
        UINT32 dwSz;
        UINT32 dwFlashOfst;
		//UINT32 u4Datatmp[1024];
	#if 1 //PROM
        dwFlashOfst = u4GetDspImgAddr ();   // set to dsp image addr	
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_PROM_SA << 2));
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_PROM_SZ << 2));

        if (dwSz >= 1)
        {
            UINT32 i;
            UINT32* puSrc= ((UINT32*) (u4GetDspImgAddr () + ((dwOfst)*4)));

            // Src: u4GetDspImgAddr () + (dwOfst * 4)
            // Size: (dwSz - 1) * 4 bytes
            WriteREG (RW_ADSPCR, PROM_WR);  // disable dsp
            WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0

            for (i=0; i<(dwSz-1); i++)
            {
                WriteREG (RW_PROM_DNLD, puSrc[i]);
            }

            // Verify
            WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0
            for (i= 0; i<(dwSz-1); i++)
            {
                //u4Datatmp[i] = ReadREG (RW_PROM_DNLD);
                UINT32 u4Data = ReadREG (RW_PROM_DNLD);
                if (u4Data != puSrc[i])
                {
                    break;
                }
            }

            WriteREG (RW_ADSPCR, 0);    // clear datarom write
		#if 0 //For PROM test pattern
            //PROM Size-1
            if(dwSz>257)	dwSz=257;
            WriteDspDecCacheDram(0x1, (dwSz-1)-1);
            WriteDspDecCacheDram(0x3, 0x800500);

            // write to share memory
            for (i= 0; i<(dwSz-1); i++)
            {
                WriteDspDecCacheDram (0x5+i, puSrc[i]);
            }
		#endif
        }		
	#endif
	#if 1 //DROM
        dwFlashOfst = u4GetDspImgAddr ();   // set to dsp image addr
        //dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_PL2_POST_SA << 2));
        //dwSz = u4GetFlashData (dwFlashOfst + (DSPF_PL2_POST_SZ << 2));
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DROM_SA << 2));
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DROM_SZ << 2));
		
        if (dwSz >= 1)
        {
            UINT32 i;
            UINT32* puSrc= ((UINT32*) (u4GetDspImgAddr () + ((dwOfst)*4)));

			// Src: u4GetDspImgAddr () + (dwOfst * 4)
			// Size: (dwSz - 1) * 4 bytes
            WriteREG (RW_ADSPCR, DROM_WR);  // disable dsp
            WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0

			for (i=0; i<(dwSz-1); i++)
			{
				WriteREG (RW_DROM_DNLD, puSrc[i]);
			}

			// Verify
			WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0
			for (i= 0; i<(dwSz-1); i++)
			{
				UINT32 u4Data = ReadREG (RW_DROM_DNLD);
				//u4Datatmp[i] = ReadREG (RW_DROM_DNLD);
				if (u4Data != puSrc[i])
				{
					break;
				}
			}

			WriteREG (RW_ADSPCR, 0);    // clear datarom write
			#if 0 //For DROM test pattern
			//DROM Size-1
			if(dwSz>257)	dwSz=257;
			WriteDspDecCacheDram(0x2, (dwSz-1)-1);
			WriteDspDecCacheDram(0x4, 0x800500);
			
			// write to share memory
			for (i= 0; i<(dwSz-1)*4; i+=3)
			{
				tmpW = (puSrc[i]<<16)|(puSrc[i+1]<<8)|(puSrc[i+2]);
				WriteDspDecCacheDram (0x105+(i/3), tmpW);    // clear datarom write
			}
			#endif
		}
	#endif
	}

	//if (BSP_GetIcVersion()==IC_VER_A) // Light: For AA/AB version, use RAM boot 

    // Rice: changed and modified _fgRamBoot to FALSE @20060731
    /*lint -e{506}*/
    /*lint -e{774}*/
    if (_fgRamBoot)
    {
        // Load Rom Code
        vDspLoadRamCode (AUD_ROMCOD);
        // Enable Ram Boot
        vSetDspBootFromRam ();
    }
	
    vDspLoadRamCode (AUD_AININTRAM);
    
#if (0) // CORDIC test
{
#define _ADSP_GetDRAMDec1_Addr(addr)   ( (UINT32*)(NONCACHE(_u4DspDramBuf[TYPE_DEC1_DRAM_IDX]+((addr)<<2))) )

    UINT32 i;
    UINT32 u4Size;
    UINT32 *Dst;
    
    u4Size = sizeof(u4CordicTest)/sizeof(UINT32);
    Dst = _ADSP_GetDRAMDec1_Addr(0x400);
    for (i=0; i<u4Size; i++)
    {
        *Dst++ = u4CordicTest[i];
    }
}
#endif
    // Let dsp go
    vTriggerDSP ();
    
	// ramboot
    // Flush cache
    //vFlushDspICache (FALSE);

}


/***************************************************************************
     Function : vResetAndHoldDSP
       Author : sammy
  Description : Reset and hold dsp 
    Parameter :
    Return    : 
***************************************************************************/
void vResetAndHoldDSP (void)
{
    UINT32 dwTmp;

    dwTmp = ReadREG (RW_ADSP_CFG);
    WriteREG (RW_ADSP_CFG, dwTmp & (~ADSP_CFG_RST_B));  // reset
}

/***************************************************************************
     Function : vTriggerDSP
       Author : sammy
  Description : Let DSP go
    Parameter :
    Return    : 
***************************************************************************/
// ramboot
#if 0
void vTriggerDSP (void)
{
    UINT32 dwTmp;

    dwTmp = ReadREG (RW_ADSP_CFG);
    WriteREG (RW_ADSP_CFG, dwTmp | ADSP_CFG_RST_B); // enable dsp
}
#else

#define FLUSH_CYCLE 1024

void vTriggerDSP(void)
{
  UINT32 dwTmp;
  UINT32 dwDramStatus;
  UINT32 dwRetryCnt=0;
  
  dwTmp = ReadREG (RW_ADSP_CFG);
  
  while(1)
  {
    #if 1
    WriteREG (RW_ADSP_CFG, dwTmp & (~ADSP_CFG_ICACHE_FLUSH) & (~ADSP_CFG_RST_B));
    WriteREG (RW_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH );
    // Light: Remove delay
    //x_thread_delay(10);                                      // delay 10 ms
    WriteREG (RW_ADSP_CFG, dwTmp | ADSP_CFG_RST_B );    
    #else
    WriteREG (RW_ADSP_CFG, dwTmp & (~ADSP_CFG_ICACHE_FLUSH) & (~ADSP_CFG_RST_B));
    WriteREG (RW_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);
    WriteREG (RW_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);    
    #endif
    
    // wait cycles to flush
    x_thread_delay(20); // Light: delay 20 ms
    // check if flush taken
    dwDramStatus =   ReadREG(RW_ADSP_CFG) & ADSP_CFG_ICACHE_FLUSH ;
    if( (dwDramStatus == 0) || (dwRetryCnt > 20) )
    {
      if(dwDramStatus != 0)
      {
      	//_dwDspFlushFailCnt ++;
      	LOG(1, "DSP flush cache fail\n");
      }
      break;
    }
    dwRetryCnt ++;
  }
  // Light add LOG of Icache flush retry count
  if(dwRetryCnt)
  {
      LOG(9,"ADSP ICache flush retry count = %d\n",dwRetryCnt);
  }

#if 1
  dwTmp = ReadREG (RW_ADSP_CFG);
  WriteREG(RW_ADSP_CFG, dwTmp & ~ADSP_CFG_RST_B);  // Reset again
  dwTmp |= ADSP_CFG_RST_B;  // enable dsp  
  WriteREG(RW_ADSP_CFG, dwTmp);  // enable dsp
#endif
}

#endif

/***************************************************************************
     Function : vSetDspBootFromRam
       Author : sammy
  Description : Set boot from ram flag.
                It is needed to be set when DSP is in reset and hold status
    Parameter : 

    Return    : 
***************************************************************************/
void vSetDspBootFromRam (void)
{
    UINT32 u4Data;

    u4Data = ReadREG (RW_ADSP_CFG);
    WriteREG (RW_ADSP_CFG, u4Data | ADSP_BOOT_FROM_RAM);
}

UINT32 dwGetDSPNSADR (UINT32 u4Type)
{
    UINT32 u4Addr;

    u4Addr = (_u4DspDramBuf[TYPE_DEC1_DRAM_IDX]);
    if (u4Type < TYPE_AUD_DEC2)
    {
        u4Addr = (_u4DspDramBuf[TYPE_DEC1_DRAM_IDX]);
    }
    else if (( (u4Type >= TYPE_AUD_DEC2) && (u4Type < TYPE_AUD_MIX) ))
    {
        u4Addr = _u4DspDramBuf[TYPE_DEC2_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_MIX) && (u4Type < TYPE_AUD_CMM) ))
    {
        u4Addr = _u4DspDramBuf[TYPE_MIX_DRAM_IDX];
    }
    else
    {
        u4Addr = _u4DspDramBuf[TYPE_COMM_DRAM_IDX];
    }
    return u4Addr;
}

UINT32 dwGetDSPCSADR (UINT32 u4Type)
{
    UINT32 u4Addr;

    u4Addr = _u4DspCmptBuf[TYPE_DEC1_DRAM_IDX];
    if (u4Type < TYPE_AUD_DEC2)
    {
        u4Addr = _u4DspCmptBuf[TYPE_DEC1_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_DEC2) && (u4Type < TYPE_AUD_MIX) ))
    {
        u4Addr = _u4DspCmptBuf[TYPE_DEC2_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_MIX) && (u4Type < TYPE_AUD_CMM) ))
    {
        u4Addr = _u4DspCmptBuf[TYPE_MIX_DRAM_IDX];
    }
    else
    {
        u4Addr = _u4DspCmptBuf[TYPE_COMM_DRAM_IDX];
    }
    return u4Addr;
}

UINT32 dwGetDspIcacheAdr (UINT8 uRamCodeType)
{
    return (((UINT32) _u4DspICacheAddr[uRamCodeType]) << 6);
}

/***************************************************************************
     Function : vSetGetBitsNotHold
  Description : Set get bits hold or not hold when bitstream buffer is empty
                It is depend on decoder setting, for ex. vorbis is set to not 
                hold, default is hold.
                It is configured in LoadDramTable
    Parameter : fgGetBitsNotHold
                TRUE : not hold
                FALSE: hold
    Return    :
***************************************************************************/
void vSetGetBitsNotHold (BOOL fgGetBitsNotHold)
{
    UINT32 u4Data;

    u4Data = ReadREG (RW_ADSP_CFG);
    u4Data = u4Data & (~NO_HOLD_GBS);
    if (fgGetBitsNotHold)
    {
        u4Data |= NO_HOLD_GBS;
    }
    WriteREG (RW_ADSP_CFG, u4Data);    
}

void vSetLineInFIFO(UINT32 u4StartAdr,UINT32 u4EndAdr, UINT8 uDecIndex)
{
    UINT32 u4SPLIN_BLK=0;
    u4SPLIN_BLK = (u4StartAdr<<8)|((u4EndAdr>>8)&0xffff);
    if(uDecIndex == SECOND_DECODER)
    {
#ifndef EMU_MT8105
        // Light: MT5371 has only one SPDIF/Line IN
        WriteREG(RW_SPLIN_BLK,u4SPLIN_BLK);
        //WriteREG(RW_SPLIN_BLK_DEC2,u4SPLIN_BLK);
#endif        
    }
    else
    {
        WriteREG(RW_SPLIN_BLK,u4SPLIN_BLK);
    }
}

/***************************************************************************
     Function : vSetSPDIFInFIFO
  Description : set input FIFO for SPDIF in
    Parameter : u4StartAdr: start address in BYTEs
                u4EndAdr:end address in BYTEs
                uDecIndex: 0: first decoder,1: second decoder
    Return    :
***************************************************************************/
void vSetSPDIFInFIFO(UINT32 u4StartAdr,UINT32 u4EndAdr, UINT8 uDecIndex)
{
    UINT32 u4SPLIN_BLK=0;
    u4SPLIN_BLK = (u4StartAdr<<8)|((u4EndAdr>>8)&0xffff);
    if(uDecIndex == SECOND_DECODER)
    {
#ifndef EMU_MT8105
        // Light: MT5371 has only one SPDIF/Line IN
        WriteREG(RW_SPLIN_BLK,u4SPLIN_BLK);
        //WriteREG(RW_SPLIN_BLK_DEC2,u4SPLIN_BLK);
#endif        
    }
    else
    {
        WriteREG(RW_SPLIN_BLK,u4SPLIN_BLK);
    }
}
/***************************************************************************
     Function : vSetSPDIFInCtrl
  Description : setup SPDIF in control register
    Parameter : u4Ctrl: configuration
                uDecIndex: 0: first decoder,1: second decoder
    Return    :
***************************************************************************/
void vSetSPDIFInCtrl(UINT8 uDecIndex,UINT32 u4Ctrl)
{
    if(uDecIndex==SECOND_DECODER)
    {
#ifndef EMU_MT8105
        WriteREG(RW_SPLIN_CTL, (u4Ctrl<<16)|(ReadREG(RW_SPLIN_CTL) & 0xFFFF));
#endif        
    }
    else
    {
        WriteREG(RW_SPLIN_CTL, u4Ctrl|(ReadREG(RW_SPLIN_CTL) & 0xFFFF0000));
    }
}
/***************************************************************************
     Function : vSetSPDIFInCtrl
  Description : set input FIFO for SPDIF in
    Parameter : u4StartAdr: start address in BYTEs
                u4EndAdr:end address in BYTEs
                uDecIndex: 0: first decoder, 1: second decoder
    Return    :
***************************************************************************/

UINT32 u4GetSPDIFInType(UINT8 uDecIndex)
{
    if(uDecIndex==SECOND_DECODER)
    {
#ifndef EMU_MT8105
        // Light: MT5371 has only one SPDIF/Line IN
        return(ReadREG(RW_SPLIN_TYPE)); 
        //return(ReadREG(RW_SPLIN_TYPE_DEC2)); 
#endif        
    }
    else
    {
        return(ReadREG(RW_SPLIN_TYPE)); 
    }
}

//-----------------------------------------------------------------------------
// vDspRegW_ByArguement
//
/** Write DSP REG by arguement.
 *
 *  @param  u4RegAdr    Register address.
 *  @param  u4RegData   Register data.
 */
//-----------------------------------------------------------------------------
void vDspRegW_ByArguement(UINT32 u4RegAdr, UINT32 u4RegData)
{
    WriteREG(u4RegAdr, u4RegData);
}

//-----------------------------------------------------------------------------
// u4DspRegR_D2rSda
//
/** Read DSP REG - D2R_SDA.
 *
 *  @retval     D2R_SDA data.
 */
//-----------------------------------------------------------------------------
UINT32 u4DspRegR_D2rSda(void)
{
    return (ReadREG(RO_D2R_SDA));
}

//-----------------------------------------------------------------------------
// u4DspRegR_ByArguement
//
/** Read DSP REG by arguement.
 *
 *  @param      u4RegAdr    Register address.
 *
 *  @retval     register data.
 */
//-----------------------------------------------------------------------------
UINT32 u4DspRegR_ByArguement(UINT32 u4RegAdr)
{
    return (ReadREG(u4RegAdr));
}

#define VDOIN_WRITE32(offset, value)  IO_WRITE32(VIDEO_IN0_BASE, (offset), (value))
#define VFE_0A 0x4a8

//-----------------------------------------------------------------------------
// vADCInit
//
/** Initialize TV audio ADC.
 */
//-----------------------------------------------------------------------------
void vADCInit(void)
{
    UINT32 u4Data;
    
    // power on APGA
    u4Data = ReadREG(RW_ADSP_ENV_BAK);
    u4Data = (u4Data & (~APGA_MASK)) |
    	     APGA_GAIN_0DB | APGA_BIAS3_20UA | APGA_BIAS2_20UA | APGA_BIAS1_20UA |
    	     APGA_LPF15MEN_15MHZ | APGA_FLREN_EN | APGA_PD_POWER_ON;
    WriteREG(RW_ADSP_ENV_BAK, u4Data);

     if (BSP_GetIcVersion() == IC_VER_AA)
     {
         u4Data = ADCCKSEL_RISING_EDGE | NCK_CTRL_300PS | CKSEL_INTERN_CLK | CKDIV_FS_2 | 
             TRGSEL_RISING_EDGE | CLR_DIS | VREFTEST_DIS | ADCTEST_DIS;
         // set AADC_CFG0
         WriteREG(RW_AADC_CFG0, u4Data);
     }
     else if (BSP_GetIcVersion() >= IC_VER_AB)
     {
         //u4Data = RISC_SEL_ECO|APGA_GAIN_0DB_ECO|APGA_OFFSET_0V_ECO | ADCCKSEL_RISING_EDGE_ECO| NCK_CTRL_300PS_ECO | CKSEL_INTERN_CLK_ECO | CKDIV_FS_2_ECO | 
          //   TRGSEL_RISING_EDGE_ECO | CLR_DIS_ECO | VREFTEST_DIS_ECO | ADCTEST_DIS_ECO;
          u4Data = APGA_GAIN_0DB_ECO|APGA_OFFSET_0V_ECO | ADCCKSEL_RISING_EDGE_ECO| NCK_CTRL_300PS_ECO | CKSEL_INTERN_CLK_ECO | CKDIV_FS_2_ECO | 
             TRGSEL_RISING_EDGE_ECO | CLR_DIS_ECO | VREFTEST_DIS_ECO | ADCTEST_DIS_ECO;
          
          // set AADC_CFG0
         WriteREG(RW_AADC_CFG0_ECO, u4Data);
     }
    
    // set AADC_CFG1
    u4Data = (UINT32)(ADC_REFPADJ_2_000V | ADC_REFNADJ_1_000V | ADC_VICMADJ_1_500V |
             ADC_VOCMADJ_1_500V | IFLASH_20UA | IVREF_20UA | ISH_60UA | IGBIAS_20UA |
             ISUB4_20UA | ISUB3_20UA | ISUB2_20UA | ISUB1_20UA);
    WriteREG(RW_AADC_CFG1, u4Data);
    
    // set AADC_CFG2
    u4Data = IMDAC4_20UA | IMDAC3_20UA | IMDAC2_20UA | IMDAC1_20UA |
    	     CHSEL_CH0 | PD_REF_POWER_ON | PD_ADC_POWER_ON;
    WriteREG(RW_AADC_CFG2, u4Data);
    
    // set APGA_CFG0 default value
    u4Data = APGA_OFFSET_0V | APGA_GAIN6DB_DIS;
    WriteREG(RW_APGA_CFG0, u4Data);
    
    // set APGA_CFG1
    u4Data = APGA_VOCMADJ10_1_50V | APGA_REFPADJ_2_00V | APGA_REFNADJ_1_00V;
    WriteREG(RW_APGA_CFG1, u4Data);
    
    // set APGA_CFG2
    u4Data = AUCHSEL_SIF | APGA_VOCMADJ2_1_50V;
    WriteREG(RW_APGA_CFG2, u4Data);
    
    // set APGA_CFG3
    u4Data = APGA_TEST_DIS;
    WriteREG(RW_APGA_CFG3, u4Data);
    
    // APGA_CFG4
    u4Data = APGA_TEST_MODE_NONE;
    WriteREG(RW_APGA_CFG4, u4Data);
    
    // APGA_CFG5 reserved
    u4Data = APGA_RESERVE_DEFAULT;
    WriteREG(RW_APGA_CFG5, u4Data);
    // Light: Added for testing APGA/AADC
    VDOIN_WRITE32(VFE_0A,0x00d00004);
}

//-----------------------------------------------------------------------------
// vADCPowerOn
//
/** Power on TV audio ADC.
 */
//-----------------------------------------------------------------------------
void vADCPowerOn(void)
{
    UINT32 u4Data;
    
    // power on APGA
    u4Data = ReadREG(RW_ADSP_ENV_BAK);
    u4Data = (u4Data & (~APGA_PD)) | APGA_PD_POWER_ON;
    WriteREG(RW_ADSP_ENV_BAK, u4Data);

    // power on ADC core, ADC reference
    u4Data = ReadREG(RW_AADC_CFG1);
    u4Data = (u4Data & (~PD_ADC) & (~PD_REF))
    	     | PD_ADC_POWER_ON| PD_REF_POWER_ON;
    WriteREG(RW_AADC_CFG1, u4Data);
}

//-----------------------------------------------------------------------------
// vADCPowerDown
//
/** Power down TV audio ADC.
 */
//-----------------------------------------------------------------------------
void vADCPowerDown(void)
{
    UINT32 u4Data;
    
    // power down APGA
    u4Data = ReadREG(RW_ADSP_ENV_BAK);
    u4Data = (u4Data & (~APGA_PD)) | APGA_PD_POWER_DOWN;
    WriteREG(RW_ADSP_ENV_BAK, u4Data);

    // power down ADC core, ADC reference
    u4Data = ReadREG(RW_AADC_CFG1);
    u4Data = (u4Data & (~PD_ADC) & (~PD_REF))
    	     | PD_ADC_POWER_DOWN | PD_REF_POWER_DOWN;
    WriteREG(RW_AADC_CFG1, u4Data);
}

//-----------------------------------------------------------------------------
// DSP_SetAIFMux
//
/** Set TV audio ADC for AF or SIF path (NTSC).
 *
 *  @param  uAIFPath    0: SIF path,1: AF path.
 */
//-----------------------------------------------------------------------------
void DSP_SetAIFMux(UINT8 uAIFPath)
{
    UINT32 u4Data;

    if (uAIFPath==AF_PATH)
    {
        // set AF path, bias current isn't set (default)
        u4Data = ReadREG(RW_APGA_CFG2);
        u4Data = (u4Data & (~AUCHSEL)) | AUCHSEL_AF;
        WriteREG(RW_APGA_CFG2, u4Data);
        // AF, set LPF cutoff freq to 300K Hz
        u4Data = ReadREG(RW_ADSP_ENV_BAK);
        u4Data = (u4Data &(~APGA_LPF15MEN))| APGA_LPF15MEN_300KHZ;
        WriteREG(RW_ADSP_ENV_BAK, u4Data);
    }
    else // SIF path
    {
        u4Data = ReadREG(RW_APGA_CFG2);
        u4Data = (u4Data & (~AUCHSEL)) | AUCHSEL_SIF;
        WriteREG(RW_APGA_CFG2, u4Data);
        // SIF, set LPF cutoff freq to 15M Hz
        u4Data = ReadREG(RW_ADSP_ENV_BAK);
        u4Data = (u4Data &(~APGA_LPF15MEN))| APGA_LPF15MEN_15MHZ;
        WriteREG(RW_ADSP_ENV_BAK, u4Data);
    }
}

//-----------------------------------------------------------------------------
// DSP_SPDIFOutSetSrc
//
/** Set the input PCM data of SPDIF-out from dec1 or dec2.
 *  Also change the AOUT CLK to the sampling rate of specified decoder.
 *
 *  @param  u1DecId     Decoder ID.
 */
//-----------------------------------------------------------------------------
void DSP_SPDIFOutSetSrc(UINT8 u1DecId)
{
    UINT32 u4Tmp;
    UINT32 u4Set = (UINT32)u1DecId;
    
    // Read Reg 0x2000500c
    u4Tmp = ReadREG(RW_AUD_CFG);
    // Set or Reset bit-25
    u4Tmp &= ~(SPDIFOUT_SW_SPDF_DEC2);
    u4Tmp |= u4Set << SPDIFOUT_SW_SPDF_DEC2_POS;
    // Write back to the Reg
    WriteREG(RW_AUD_CFG, u4Tmp);
}

//-----------------------------------------------------------------------------
// DSP_ClearInt
//
/** Clear DSP interrupt.
 */
//-----------------------------------------------------------------------------
void DSP_ClearInt(void)
{
    WriteREG(RO_D2R_SDA, 0x0);
}

//-----------------------------------------------------------------------------
// DSP_UpdateAdcTuneCfg_AA
//
/** Ain update ADC tune cfg for IC version = AA.
 */
//-----------------------------------------------------------------------------
void DSP_UpdateAdcTuneCfg_AA(void)
{
    UINT32 u4DspRIntData;
    UINT32 u4TempData;

    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    u4DspRIntData = dReadDspCommDram(ADDR_D2RC_ADC_TUNE_CFG);
    // Update ADC_GAIN
    u4TempData = ReadREG(RW_ADSP_ENV_BAK);
    u4TempData = (u4TempData & (~APGA_GAIN)) |
        (((u4DspRIntData & DRAM_ADC_GAIN_MASK)>>DRAM_ADC_GAIN_POS)<<APGA_GAIN_POS);
    WriteREG(RW_ADSP_ENV_BAK, u4TempData);
    // Update ADC_OFFSET
    u4TempData = ReadREG(RW_APGA_CFG0);
    u4TempData = (u4TempData & (~APGA_OFFSET)) |
        (((u4DspRIntData & DRAM_ADC_OFFSET_MASK)>>DRAM_ADC_OFFSET_POS)<<APGA_OFFSET_POS);
    WriteREG(RW_APGA_CFG0, u4TempData);
}

//-----------------------------------------------------------------------------
// DSP_UpdateAdcTuneCfg_AB
//
/** Ain update ADC tune cfg for IC version >= AB.
 */
//-----------------------------------------------------------------------------
void DSP_UpdateAdcTuneCfg_AB(void)
{
    UINT32 u4DspRIntData;
    UINT32 u4TempData;

    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro    
    u4DspRIntData = dReadDspCommDram(ADDR_D2RC_ADC_TUNE_CFG);
    // Update ADC_GAIN
    u4TempData = ReadREG(RW_AADC_CFG0_ECO);
    u4TempData = (u4TempData & (~APGA_GAIN_ECO)) |
        (((u4DspRIntData & DRAM_ADC_GAIN_MASK)>>DRAM_ADC_GAIN_POS)<<APGA_GAIN_POS_ECO);
    WriteREG(RW_AADC_CFG0_ECO, u4TempData);
    // Update ADC_OFFSET
    u4TempData = ReadREG(RW_AADC_CFG0_ECO);
    u4TempData = (u4TempData & (~APGA_OFFSET_ECO)) |
        (((u4DspRIntData & DRAM_ADC_OFFSET_MASK)>>DRAM_ADC_OFFSET_POS)<<APGA_OFFSET_POS_ECO);
    WriteREG(RW_AADC_CFG0_ECO, u4TempData);
}

//-----------------------------------------------------------------------------
// vTriggerDSPSLT
//
/** SLT DSP Trigger.
 */
//-----------------------------------------------------------------------------
void vTriggerDSPSLT(void)
{
    UINT32 dwTmp;
    UINT32 dwDramStatus;
    UINT32 dwRetryCnt = 0;
    
    dwTmp = ReadREG(RW_ADSP_CFG);
  
    while(1)
    {
        #if 1
        WriteREG(RW_ADSP_CFG, dwTmp & (~ADSP_CFG_ICACHE_FLUSH) & (~ADSP_CFG_RST_B));
        WriteREG(RW_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH );
        //x_thread_delay(10);                                      // delay 10 ms
        WriteREG(RW_ADSP_CFG, dwTmp | ADSP_CFG_RST_B);    
        #else
        WriteREG(RW_ADSP_CFG, dwTmp & (~ADSP_CFG_ICACHE_FLUSH) & (~ADSP_CFG_RST_B));
        WriteREG(RW_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);
        WriteREG(RW_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);    
        #endif
        // wait cycles to flush
        //x_thread_delay(10);                                      // delay 10 ms
        // check if flush taken
        dwDramStatus = ReadREG(RW_ADSP_CFG) & ADSP_CFG_ICACHE_FLUSH;
        if ( (dwDramStatus == 0) || (dwRetryCnt > 20) )
        {
            if (dwDramStatus != 0)
            {
                //_dwDspFlushFailCnt ++;
                Printf("DSP flush cache fail\n");
            }
            break;
        }
        dwRetryCnt ++;
    }
    // Light add LOG of Icache flush retry count
    if (dwRetryCnt)
    {
        Printf("ADSP ICache flush retry count = %d\n",dwRetryCnt);
    }
    #if 0
    WriteREG(RW_ADSP_CFG, dwTmp & ~ADSP_CFG_RST);  // Reset again
    dwTmp |= ADSP_CFG_RST;                         // enable dsp
    x_thread_delay(20);                              // delay 10 ms
    WriteREG(RW_ADSP_CFG, dwTmp);                    // enable dsp
    #endif
}

//-----------------------------------------------------------------------------
// DSP_ResetDsp
//
/** Reset DSP.
 */
//-----------------------------------------------------------------------------
void DSP_ResetDsp(void)
{
    WriteREG(RW_ADSP_CFG, ReadREG(RW_ADSP_CFG) & ~ADSP_CFG_RST_B);
    WriteREG(RW_ADSP_CFG, ReadREG(RW_ADSP_CFG) | ADSP_CFG_RST_B);
}

//-----------------------------------------------------------------------------
// DSP_SetAudDramBlock
//
/** Setup Audio DRAM block.
 *
 *  @retval     Sucess or failed.
 */
//-----------------------------------------------------------------------------
CHAR DSP_SetAudDramBlock(UINT8 u1MemBlock)
{
    CHAR error = 0;

    switch (u1MemBlock)
    {
    case 0:
        AUD_WRITE32(REG_AENV_CFG, (AUD_READ32(REG_AENV_CFG) & ~DRAM_BLOCK_MASK) | DRAM_BLOCK_0_16MB); 
        break;
    case	 1:
        AUD_WRITE32(REG_AENV_CFG, (AUD_READ32(REG_AENV_CFG) & ~DRAM_BLOCK_MASK) | DRAM_BLOCK_16_32MB); 
        break;
    case 2:
        AUD_WRITE32(REG_AENV_CFG, (AUD_READ32(REG_AENV_CFG) & ~DRAM_BLOCK_MASK) | DRAM_BLOCK_32_48MB); 
        break;
    case 3:
        AUD_WRITE32(REG_AENV_CFG, (AUD_READ32(REG_AENV_CFG) & ~DRAM_BLOCK_MASK) | DRAM_BLOCK_48_64MB); 
        break;
    default:
        error = 1;
        break;
    }

    return error;
}

//-----------------------------------------------------------------------------
// IecInfoInit
//
/** Iec power on init (for both decoders).
 */
//-----------------------------------------------------------------------------
void IecInfoInit (void)
{
    UINT32 dwDspData   = 0;
    UINT16 wPcmChan    = 0;          // Default is LR output
    UINT16 wIecMute    = 0;
    BOOL fgIecMute     = FALSE;
    BOOL fgIecFlag     = FALSE;
    UINT32 dwOtherCfg;

    dwOtherCfg = ReadREG (RW_AUD_CFG) & 0x000000FF; // Remaining not iec config
    
    /* for first decoder */
    fgIecFlag = uReadDspShmBYTE (B_IECFLAG)  & 0x01;
    fgIecMute = uReadDspShmBYTE (B_IEC_MUTE) & 0x01;    // IEC signal on/off state
    _fgIecMuteState = FALSE;    // IEC mute on DSP side
    // decide IEC mute
    if ((fgIecFlag && fgIecMute) || !AUD_IsSpdifOutPortOn(AUD_DEC_MAIN))
    {
        wIecMute = (UINT16) IEC_OFF;
    }
    // get IEC output selection
    wPcmChan = ((UINT32) uReadDspShmBYTE (B_IEC_PCMCH)) << IEC_CHSEL_POS; // bit 10~12
    // update IEC info
    dwDspData = wIecMute | wPcmChan | dwOtherCfg;

    /* for second decoder */
    fgIecFlag = uReadDspShmBYTE (B_IECFLAG_DEC2)  & 0x01;
    fgIecMute = uReadDspShmBYTE (B_IEC_MUTE_DEC2) & 0x01;   // IEC signal on/off state
    _fgIecMuteState = FALSE;    // IEC mute on DSP side
    // decide IEC mute
    if ((fgIecFlag && fgIecMute) || !AUD_IsSpdifOutPortOn(AUD_DEC_AUX))
    {
        wIecMute = (UINT16) IEC_OFF_DEC2;
    }
    // get IEC output selection
    wPcmChan = (UINT16) (uReadDspShmBYTE (B_IEC_PCMCH_DEC2)) << IEC_CHSEL_POS_DEC2;   // bit 18~20
    // update IEC info
    dwDspData = wIecMute | wPcmChan | dwDspData;

    // update RW_AUD_CFG
    WriteREG (RW_AUD_CFG, (UINT32) (dwDspData));
}

//-----------------------------------------------------------------------------
// _ADSP_CFG_Reset
//
/** Reset ADSP Cfg.
 */
//-----------------------------------------------------------------------------
void _ADSP_CFG_Reset(void)
{
    WriteREG(RW_ADSP_CFG, ADSP_CFG_CLK_ENA);    // Enable DSP clock
    vResetAndHoldDSP();                         // Reset and hold DSP
    vSetCacheSize(CACHE_1K);                    // Set cache size
    vSetCacheInstLen(SIZE32X64);                // Set instruction buffer size
    vSetCmptRDThreshold(THRES_UNFULL);          // Set compact read threshold
}

