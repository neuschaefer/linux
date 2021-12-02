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
 *   oper_32b.h
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
 * $Revision: #1 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

/* Double precision operations */
/* $Id $ */

#ifndef OPER_32B_AMR_H
#define OPER_32B_AMR_H


#include "basic_op.h"


void L_Extract (Word32 L_32, Word16 *hi, Word16 *lo);
Word32 L_Comp (Word16 hi, Word16 lo);
//Word32 Mpy_32 (Word16 hi1, Word16 lo1, Word16 hi2, Word16 lo2);
Word32 Mpy_32_16 (Word16 hi, Word16 lo, Word16 n);


Word32 Inv_sqrt( Word32 L_x );
void Log2 ( Word32 L_x, Word16 *exponent, Word16 *fraction );
void Log2_norm ( Word32 L_x, Word32 exp, Word16 *exponent, Word16 *fraction );
Word32 DPF_Div( Word32 L_num, Word32 denom );



INLINE Word32 DPF_Mult( Word32 a1, Word32 a2 );

INLINE Word32 DPF_Mult16( Word32 L_32, Word32 n );


//Word32 Div_32 (Word32 L_num, Word16 denom_hi, Word16 denom_lo);



/*----------------------------------------------------------------------*/
Word32 Div_32_o(Word32 L_num, Word32 denom);

INLINE Word32 L_Extract_o(Word32 L_32);

INLINE Word32 L_Comp_o(Word32 x);

INLINE Word32 Mpy_32_o(Word32 x, Word32 y);

INLINE Word32 Mpy_32_16_o(Word32 x, Word32 n);




/*****************************************************************************
 * Function Mac_32()                                                         *
 *                                                                           *
 *   Multiply two 32 bit integers (DPF) and accumulate with (normal) 32 bit  *
 *   integer. The multiplication result is divided by 2**31                  *
 *                                                                           *
 *   L_32 = L_32 + (hi1*hi2)<<1 + ( (hi1*lo2)>>15 + (lo1*hi2)>>15 )<<1       *
 *                                                                           *
 *   This operation can also be viewed as the multiplication of two Q31      *
 *   number and the result is also in Q31.                                   *
 *                                                                           *
 * Arguments:                                                                *
 *                                                                           *
 *  hi1         hi part of first number                                      *
 *  lo1         lo part of first number                                      *
 *  hi2         hi part of second number                                     *
 *  lo2         lo part of second number                                     *
 *                                                                           *
 *****************************************************************************
*/
Word32 Mac_32_o(Word32 L32a, Word32 x, Word32 n);

/*****************************************************************************
 * Function Mac_32_16()                                                      *
 *                                                                           *
 *   Multiply a 16 bit integer by a 32 bit (DPF) and accumulate with (normal)*
 *   32 bit integer. The multiplication result is divided by 2**15           *
 *                                                                           *
 *                                                                           *
 *   L_32 = L_32 + (hi1*lo2)<<1 + ((lo1*lo2)>>15)<<1                         *
 *                                                                           *
 * Arguments:                                                                *
 *                                                                           *
 *  hi          hi part of 32 bit number.                                    *
 *  lo          lo part of 32 bit number.                                    *
 *  n           16 bit number.                                               *
 *                                                                           *
 *****************************************************************************
*/
Word32 Mac_32_16_o (Word32 L32a, Word32 x, Word32 n);





#endif

