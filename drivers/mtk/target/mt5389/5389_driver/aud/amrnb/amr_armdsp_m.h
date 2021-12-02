/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*******************************************************************************
 *
 * Filename:
 * ---------
 *   AMR_armdsp_m.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR codec implementation
 *
 * Author:
 * -------
 *   Stan Huang
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 * 12 20 2010 scholar.chang
 * [WCPSP00000557] [SWIP]Compile warning removed.
 * Finish the amr SWIP compile warning removal.
 *
 *
 *******************************************************************************/

/* armdsp.h
 *
 * Copyright 2001 ARM Limited. All rights reserved.
 *
 * RCS $Revision: #2 $
 * Checkin $Date: 2012/06/28 $
 * Revising $Author: kai.wang $
 */

/* ----------------------------------------------------------------------
 * This header file provides a set of DSP-type primitive
 * operations, such as saturating integer arithmetic and 16x16 bit
 * multiplication. The operations it provides are exactly the same
 * operations supported directly by the ARM9E.
 *
 * This header file is intended as an example implementation. It
 * demonstrates how to use the inline assembly feature in the ARM
 * compilers, to construct intrinsic functions that provide
 * C-language access to the ARM9E's DSP capability.
 */

#ifndef ARMDSP_ARMDSP_H
#define ARMDSP_ARMDSP_H

#include "basic_op.h"
#include "typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !__TARGET_FEATURE_DSPMUL
INLINE Word32 _saturate(Word64 ret);
#endif

/*
 * Saturating 32-bit integer addition.
 */
INLINE Word32 qadd(Word32 x, Word32 y);

/*
 * Saturating 32-bit integer subtraction.
 */
INLINE Word32 qsub(Word32 x, Word32 y);

/*
 * Saturating 32-bit integer addition, with the second operand
 * multiplied by two. (i.e. return x + 2 * y)
 */
INLINE Word32 qdadd(Word32 x, Word32 y);

/*
 * Saturating 32-bit integer subtraction, with the second operand
 * multiplied by two. (i.e. return x - 2 * y)
 */
INLINE Word32 qdsub(Word32 x, Word32 y);

/*
 * Multiply two signed 16-bit numbers, taken from the bottom or top
 * halves of the input words. For example, smulbt(x,y) multiplies
 * the bottom 16 bits of x with the top 16 bits of y.
 */

/* smulbb - multiply bottom half of x and bottom half of y */
INLINE Word32 smulbb(Word32 x, Word32 y);

/* smulbt - multiply bottom half of x and top half of y */
INLINE Word32 smulbt(Word32 x, Word32 y);

/* smultb - multiply top half of x and bottom half of y */
INLINE Word32 smultb(Word32 x, Word32 y);

/* smultt - multiply top half of x and top half of y */
INLINE Word32 smultt(Word32 x, Word32 y);

/*
 * Multiply two signed 16-bit numbers, taken from the bottom or top
 * halves of the input words x and y, and add to the input word a.
 * For example, smlabt(x,y,a) multiplies the bottom 16 bits of x with
 * the top 16 bits of y, and then adds a.
 */

/* smlabb - multiply bottom half of x and bottom half of y */
INLINE Word32 smlabb(Word32 x, Word32 y, Word32 a);

/* smlabt - multiply bottom half of x and top half of y */
INLINE Word32 smlabt(Word32 x, Word32 y, Word32 a);

/* smlatb - multiply top half of x and bottom half of y */
INLINE Word32 smlatb(Word32 x, Word32 y, Word32 a);

/* smlatt - multiply top half of x and top half of y */
INLINE Word32 smlatt(Word32 x, Word32 y, Word32 a);

/*
 * Multiply two signed 16-bit numbers, taken from the bottom or top
 * halves of the input words x and y, and add to the 64-bit integer
 * stored in the input words lo and hi. For example,
 * smlalbt(&lo,&hi,x,y) multiplies the bottom 16 bits of x with the
 * top 16 bits of y, and then adds the result to (hi,lo).
 *
 * WARNING: if you use these primitives too often inside the same
 * function, they may fail to be inlined.
 */

/* smlalbb - multiply bottom half of x and bottom half of y */
INLINE void smlalbb(Word32 *lo, Word32 *hi, Word32 x, Word32 y);

/* smlalbt - multiply bottom half of x and top half of y */
INLINE void smlalbt(Word32 *lo, Word32 *hi, Word32 x, Word32 y);

/* smlaltb - multiply top half of x and bottom half of y */
INLINE void smlaltb(Word32 *lo, Word32 *hi, Word32 x, Word32 y);

/* smlaltt - multiply top half of x and top half of y */
INLINE void smlaltt(Word32 *lo, Word32 *hi, Word32 x, Word32 y);

/*
 * Multiply a 32-bit signed integer (the input word x) with a
 * 16-bit signed integer (taken from either the top or the bottom
 * half of the input word y). Discard the bottom 16 bits of the
 * 48-bit product.
 */

/* smulwb - multiply x by the bottom half of y */
INLINE Word32 smulwb(Word32 x, Word32 y);

/* smulwt - multiply x by the top half of y */
INLINE Word32 smulwt(Word32 x, Word32 y);

/*
 * Multiply a 32-bit signed integer (the input word x) with a
 * 16-bit signed integer (taken from either the top or the bottom
 * half of the input word y). Discard the bottom 16 bits of the
 * 48-bit product, and add to the input word a.
 */

/* smlawb - multiply x by the bottom half of y */
INLINE Word32 smlawb(Word32 x, Word32 y, Word32 a);

/* smlawt - multiply x by the top half of y */
INLINE Word32 smlawt(Word32 x, Word32 y, Word32 a);

INLINE Word32 smull(Word32 x, Word32 y);

INLINE Word64 smlal(Word64 LL_acc, Word32 x, Word32 y);

INLINE Word32 clz(Word32 absx);

INLINE void msr_cpsr_f_0(void);

#ifdef __cplusplus
}
#endif

#endif /* ARMDSP_ARMDSP_H */


