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
 *   oper_32b.c
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

#ifdef LINUX_TURNKEY_SOLUTION

/*****************************************************************************
 *  $Id $
 *
 *  This file contains operations in double precision.                       *
 *  These operations are not standard double precision operations.           *
 *  They are used where single precision is not enough but the full 32 bits  *
 *  precision is not necessary. For example, the function Div_32() has a     *
 *  24 bits precision which is enough for our purposes.                      *
 *                                                                           *
 *  The double precision numbers use a special representation:               *
 *                                                                           *
 *     L_32 = hi<<16 + lo<<1                                                 *
 *                                                                           *
 *  L_32 is a 32 bit integer.                                                *
 *  hi and lo are 16 bit signed integers.                                    *
 *  As the low part also contains the sign, this allows fast multiplication. *
 *                                                                           *
 *      0x8000 0000 <= L_32 <= 0x7fff fffe.                                  *
 *                                                                           *
 *  We will use DPF (Double Precision Format )in this file to specify        *
 *  this special format.                                                     *
 *****************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"


/*****************************************************************************
 *                Tables
 *****************************************************************************
*/


/*
static const Word16 table[33] =
{
    0, 1455, 2866, 4236, 5568, 6863, 8124, 9352, 10549, 11716,
    12855, 13967, 15054, 16117, 17156, 18172, 19167, 20142, 21097, 22033,
    22951, 23852, 24735, 25603, 26455, 27291, 28113, 28922, 29716, 30497,
    31266, 32023, 32767
};
*/

static const Word32 log2_tab[32] =
{
    (Word32)    0<<16,    (Word32) 1455<<16,    (Word32) 2866<<16,    (Word32) 4236<<16,
    (Word32) 5568<<16,    (Word32) 6863<<16,    (Word32) 8124<<16,    (Word32) 9352<<16,
    (Word32)10549<<16,    (Word32)11716<<16,    (Word32)12855<<16,    (Word32)13967<<16,
    (Word32)15054<<16,    (Word32)16117<<16,    (Word32)17156<<16,    (Word32)18172<<16,
    (Word32)19167<<16,    (Word32)20142<<16,    (Word32)21097<<16,    (Word32)22033<<16,
    (Word32)22951<<16,    (Word32)23852<<16,    (Word32)24735<<16,    (Word32)25603<<16,
    (Word32)26455<<16,    (Word32)27291<<16,    (Word32)28113<<16,    (Word32)28922<<16,
    (Word32)29716<<16,    (Word32)30497<<16,    (Word32)31266<<16,    (Word32)32023<<16
};
static const Word16 log2_tab_diff[32] =
{
    (Word16)(    0- 1455),    (Word16)( 1455- 2866),    (Word16)( 2866- 4236),    (Word16)( 4236- 5568),
    (Word16)( 5568- 6863),    (Word16)( 6863- 8124),    (Word16)( 8124- 9352),    (Word16)( 9352-10549),
    (Word16)(10549-11716),    (Word16)(11716-12855),    (Word16)(12855-13967),    (Word16)(13967-15054),
    (Word16)(15054-16117),    (Word16)(16117-17156),    (Word16)(17156-18172),    (Word16)(18172-19167),
    (Word16)(19167-20142),    (Word16)(20142-21097),    (Word16)(21097-22033),    (Word16)(22033-22951),
    (Word16)(22951-23852),    (Word16)(23852-24735),    (Word16)(24735-25603),    (Word16)(25603-26455),
    (Word16)(26455-27291),    (Word16)(27291-28113),    (Word16)(28113-28922),    (Word16)(28922-29716),
    (Word16)(29716-30497),    (Word16)(30497-31266),    (Word16)(31266-32023),    (Word16)(32023-32767)
};


/*
static const Word16 table[49] ={
32767, 31790, 30894, 30070, 29309, 28602, 27945, 27330, 26755, 26214,
25705, 25225, 24770, 24339, 23930, 23541, 23170, 22817, 22479, 22155,
21845, 21548, 21263, 20988, 20724, 20470, 20225, 19988, 19760, 19539,
19326, 19119, 18919, 18725, 18536, 18354, 18176, 18004, 17837, 17674,
17515, 17361, 17211, 17064, 16921, 16782, 16646, 16514, 16384};
*/
static const Word32 inv_sqrt_tab[48] =
{
   (Word32)32767<<15,   (Word32)31790<<15,   (Word32)30894<<15,   (Word32)30070<<15,
   (Word32)29309<<15,   (Word32)28602<<15,   (Word32)27945<<15,   (Word32)27330<<15,
   (Word32)26755<<15,   (Word32)26214<<15,   (Word32)25705<<15,   (Word32)25225<<15,
   (Word32)24770<<15,   (Word32)24339<<15,   (Word32)23930<<15,   (Word32)23541<<15,
   (Word32)23170<<15,   (Word32)22817<<15,   (Word32)22479<<15,   (Word32)22155<<15,
   (Word32)21845<<15,   (Word32)21548<<15,   (Word32)21263<<15,   (Word32)20988<<15,
   (Word32)20724<<15,   (Word32)20470<<15,   (Word32)20225<<15,   (Word32)19988<<15,
   (Word32)19760<<15,   (Word32)19539<<15,   (Word32)19326<<15,   (Word32)19119<<15,
   (Word32)18919<<15,   (Word32)18725<<15,   (Word32)18536<<15,   (Word32)18354<<15,
   (Word32)18176<<15,   (Word32)18004<<15,   (Word32)17837<<15,   (Word32)17674<<15,
   (Word32)17515<<15,   (Word32)17361<<15,   (Word32)17211<<15,   (Word32)17064<<15,
   (Word32)16921<<15,   (Word32)16782<<15,   (Word32)16646<<15,   (Word32)16514<<15
};

static const Word16 inv_sqrt_tab_diff[48] =
{
   (Word16)(32767-31790),   (Word16)(31790-30894),   (Word16)(30894-30070),   (Word16)(30070-29309),
   (Word16)(29309-28602),   (Word16)(28602-27945),   (Word16)(27945-27330),   (Word16)(27330-26755),
   (Word16)(26755-26214),   (Word16)(26214-25705),   (Word16)(25705-25225),   (Word16)(25225-24770),
   (Word16)(24770-24339),   (Word16)(24339-23930),   (Word16)(23930-23541),   (Word16)(23541-23170),
   (Word16)(23170-22817),   (Word16)(22817-22479),   (Word16)(22479-22155),   (Word16)(22155-21845),
   (Word16)(21845-21548),   (Word16)(21548-21263),   (Word16)(21263-20988),   (Word16)(20988-20724),
   (Word16)(20724-20470),   (Word16)(20470-20225),   (Word16)(20225-19988),   (Word16)(19988-19760),
   (Word16)(19760-19539),   (Word16)(19539-19326),   (Word16)(19326-19119),   (Word16)(19119-18919),
   (Word16)(18919-18725),   (Word16)(18725-18536),   (Word16)(18536-18354),   (Word16)(18354-18176),
   (Word16)(18176-18004),   (Word16)(18004-17837),   (Word16)(17837-17674),   (Word16)(17674-17515),
   (Word16)(17515-17361),   (Word16)(17361-17211),   (Word16)(17211-17064),   (Word16)(17064-16921),
   (Word16)(16921-16782),   (Word16)(16782-16646),   (Word16)(16646-16514),   (Word16)(16514-16384)
};


/*****************************************************************************
 *                                                                           *
 *  Function L_Extract()                                                     *
 *                                                                           *
 *  Extract from a 32 bit integer two 16 bit DPF.                            *
 *                                                                           *
 *  Arguments:                                                               *
 *                                                                           *
 *   L_32      : 32 bit integer.                                             *
 *               0x8000 0000 <= L_32 <= 0x7fff ffff.                         *
 *   hi        : b16 to b31 of L_32                                          *
 *   lo        : (L_32 - hi<<16)>>1                                          *
 *****************************************************************************
*/

void L_Extract (Word32 L_32, Word16 *hi, Word16 *lo)
{
//    *hi = extract_h (L_32);
//    *lo = extract_l (L_msu (L_shr (L_32, 1), *hi, 16384));
    *hi = (Word16)(L_32 >> 16);
    *lo = (Word16)(qdsub ( (L_32 >> 1),smulbb(*hi,16384) ));
    return;
}

/*****************************************************************************
 *                                                                           *
 *  Function L_Comp()                                                        *
 *                                                                           *
 *  Compose from two 16 bit DPF a 32 bit integer.                            *
 *                                                                           *
 *     L_32 = hi<<16 + lo<<1                                                 *
 *                                                                           *
 *  Arguments:                                                               *
 *                                                                           *
 *   hi        msb                                                           *
 *   lo        lsf (with sign)                                               *
 *                                                                           *
 *   Return Value :                                                          *
 *                                                                           *
 *             32 bit long signed integer (Word32) whose value falls in the  *
 *             range : 0x8000 0000 <= L_32 <= 0x7fff fff0.                   *
 *                                                                           *
 *****************************************************************************
*/

Word32 L_Comp (Word16 hi, Word16 lo)
{
    Word32 L_32;

    L_32 = (hi << 16);
    return (qdadd(L_32, smulbb(lo, 1)));       /* = hi<<16 + lo<<1 */
}

/*****************************************************************************
 * Function Mpy_32()                                                         *
 *                                                                           *
 *   Multiply two 32 bit integers (DPF). The result is divided by 2**31      *
 *                                                                           *
 *   L_32 = (hi1*hi2)<<1 + ( (hi1*lo2)>>15 + (lo1*hi2)>>15 )<<1              *
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
#if 0
Word32 Mpy_32 (Word16 hi1, Word16 lo1, Word16 hi2, Word16 lo2)
{
    Word32 L_32;

    L_32 = L_mult (hi1, hi2);
    L_32 = L_mac (L_32, mult (hi1, lo2), 1);
    L_32 = L_mac (L_32, mult (lo1, hi2), 1);

    return (L_32);
}
#endif
/*****************************************************************************
 * Function Mpy_32_16()                                                      *
 *                                                                           *
 *   Multiply a 16 bit integer by a 32 bit (DPF). The result is divided      *
 *   by 2**15                                                                *
 *                                                                           *
 *                                                                           *
 *   L_32 = (hi1*lo2)<<1 + ((lo1*lo2)>>15)<<1                                *
 *                                                                           *
 * Arguments:                                                                *
 *                                                                           *
 *  hi          hi part of 32 bit number.                                    *
 *  lo          lo part of 32 bit number.                                    *
 *  n           16 bit number.                                               *
 *                                                                           *
 *****************************************************************************
*/

Word32 Mpy_32_16 (Word16 hi, Word16 lo, Word16 n)
{
   Word32 L_32;

   L_32 = smulbb(hi, n);
	L_32 = qadd(L_32,L_32);
   L_32 = qdadd(L_32,smulbb( (smulbb(lo, n)>>15), 1));

   return (L_32);
}

/*****************************************************************************
 *                                                                           *
 *   Function Name : Div_32                                                  *
 *                                                                           *
 *   Purpose :                                                               *
 *             Fractional integer division of two 32 bit numbers.            *
 *             L_num / L_denom.                                              *
 *             L_num and L_denom must be positive and L_num < L_denom.       *
 *             L_denom = denom_hi<<16 + denom_lo<<1                          *
 *             denom_hi is a normalize number.                               *
 *                                                                           *
 *   Inputs :                                                                *
 *                                                                           *
 *    L_num                                                                  *
 *             32 bit long signed integer (Word32) whose value falls in the  *
 *             range : 0x0000 0000 < L_num < L_denom                         *
 *                                                                           *
 *    L_denom = denom_hi<<16 + denom_lo<<1      (DPF)                        *
 *                                                                           *
 *       denom_hi                                                            *
 *             16 bit positive normalized integer whose value falls in the   *
 *             range : 0x4000 < hi < 0x7fff                                  *
 *       denom_lo                                                            *
 *             16 bit positive integer whose value falls in the              *
 *             range : 0 < lo < 0x7fff                                       *
 *                                                                           *
 *   Return Value :                                                          *
 *                                                                           *
 *    L_div                                                                  *
 *             32 bit long signed integer (Word32) whose value falls in the  *
 *             range : 0x0000 0000 <= L_div <= 0x7fff ffff.                  *
 *                                                                           *
 *  Algorithm:                                                               *
 *                                                                           *
 *  - find = 1/L_denom.                                                      *
 *      First approximation: approx = 1 / denom_hi                           *
 *      1/L_denom = approx * (2.0 - L_denom * approx )                       *
 *                                                                           *
 *  -  result = L_num * (1/L_denom)                                          *
 *****************************************************************************
*/
#if 0
Word32 Div_32 (Word32 L_num, Word16 denom_hi, Word16 denom_lo)
{
    Word16 approx, hi, lo, n_hi, n_lo;
    Word32 L_32;

    /* First approximation: 1 / L_denom = 1/denom_hi */

    approx = div_s ((Word16) 0x3fff, denom_hi);

    /* 1/L_denom = approx * (2.0 - L_denom * approx) */

    L_32 = Mpy_32_16 (denom_hi, denom_lo, approx);

    L_32 = L_sub ((Word32) 0x7fffffffL, L_32);

    L_Extract (L_32, &hi, &lo);

    L_32 = Mpy_32_16 (hi, lo, approx);

    /* L_num * (1/L_denom) */

    L_Extract (L_32, &hi, &lo);
    L_Extract (L_num, &n_hi, &n_lo);
    L_32 = Mpy_32 (n_hi, n_lo, hi, lo);
    L_32 = L_shl (L_32, 2);

    return (L_32);
}
#endif

Word32 Div_32_o(Word32 L_num, Word32 denom)
{
  Word32 approx, L_tmp, L_tmp_n;
  Word32 L_32;
  /* First approximation: 1 / L_denom = 1/denom_hi */
  approx = div_s( (Word16)0x3fff, (Word16)(denom >> 16));    /* result in Q14 */
                                  		              			/* Note: 3fff = 0.5 in Q15 */

  /* 1/L_denom = approx * (2.0 - L_denom * approx) */
  L_32 = Mpy_32_16_o(denom, approx); /* result in Q30 */
  L_32 = qsub( (Word32)0x7fffffffL, L_32);      /* result in Q30 */
  L_tmp = L_Extract_o(L_32);
  L_32 = Mpy_32_16_o(L_tmp, approx);             /* = 1/L_denom in Q29 */

  /* L_num * (1/L_denom) */
  L_tmp = L_Extract_o(L_32);
  L_tmp_n = L_Extract_o(L_num);
  L_32 = Mpy_32_o(L_tmp_n, L_tmp);              /* result in Q29   */
  L_32 = L_shl(L_32, 2);                        /* From Q29 to Q31 */

  return( L_32 );
}


Word32 DPF_Div( Word32 L_num, Word32 denom )
{
   Word32 L_32;
   Word16 approx;
   /* First approximation: 1 / L_denom = 1/denom_hi */

   approx = div_s( (Word16)0x3fff, (Word16)(denom >> 16) );

   /* 1/L_denom = approx * (2.0 - L_denom * approx) */

   L_32 = DPF_Mult16( denom, approx);
   L_32 = qsub ((Word32) 0x7fffffffL, L_32);
   L_32 = DPF_Mult16( L_32, approx);
   L_32 = DPF_Mult( L_num, L_32 );
   L_32 = L_shl (L_32, 2);

   return (L_32);
}







/*************************************************************************
 *
 *   FUNCTION:   Log2_norm()
 *
 *   PURPOSE:   Computes log2(L_x, exp),  where   L_x is positive and
 *              normalized, and exp is the normalisation exponent
 *              If L_x is negative or zero, the result is 0.
 *
 *   DESCRIPTION:
 *        The function Log2(L_x) is approximated by a table and linear
 *        interpolation. The following steps are used to compute Log2(L_x)
 *
 *           1- exponent = 30-norm_exponent
 *           2- i = bit25-b31 of L_x;  32<=i<=63  (because of normalization).
 *           3- a = bit10-b24
 *           4- i -=32
 *           5- fraction = table[i]<<16 - (table[i] - table[i+1]) * a * 2
 *
 *************************************************************************/
void Log2_norm (
    Word32 L_x,         /* (i) : input value (normalized)                    */
    Word32 exp,         /* (i) : norm_l (L_x)                                */
    Word16 *exponent,   /* (o) : Integer part of Log2.   (range: 0<=val<=30) */
    Word16 *fraction    /* (o) : Fractional part of Log2. (range: 0<=val<1)  */
)
{
   Word32 i;

   if( L_x <= 0 ) {
      *exponent = 0;
      *fraction = 0;
      return;
   }

   *exponent = (Word16)(30 - exp);

   L_x >>= 10;
   i = (L_x >> 15) - 32;

    if (i>=32 || i<0)
        i = 0;

   /* L_y = (table[i] << 16) - (table[i] - table[i+1])*a*2  */
   L_x = qdsub( log2_tab[i], smulbb( log2_tab_diff[i], L_x & 0x7fff ) );

   *fraction = (Word16)(L_x >> 16);



}

/*************************************************************************
 *
 *   FUNCTION:   Log2()
 *
 *   PURPOSE:   Computes log2(L_x),  where   L_x is positive.
 *              If L_x is negative or zero, the result is 0.
 *
 *   DESCRIPTION:
 *        normalizes L_x and then calls Log2_norm().
 *
 *************************************************************************/
void Log2 (
    Word32 L_x,         /* (i) : input value                                 */
    Word16 *exponent,   /* (o) : Integer part of Log2.   (range: 0<=val<=30) */
    Word16 *fraction    /* (o) : Fractional part of Log2. (range: 0<=val<1) */
)
{
    Word32 exp;

    exp = norm_l (L_x);
    Log2_norm ( (L_x << exp), exp, exponent, fraction);
}


/*
********************************************************************************
*
*      File             : inv_sqrt.c
*      Purpose          : Computes 1/sqrt(L_x),  where  L_x is positive.
*                       : If L_x is negative or zero,
*                       : the result is 1 (3fff ffff).
*      Description      :
*            The function 1/sqrt(L_x) is approximated by a table and linear
*            interpolation. The inverse square root is computed using the
*            following steps:
*                1- Normalization of L_x.
*                2- If (30-exponent) is even then shift right once.
*                3- exponent = (30-exponent)/2  +1
*                4- i = bit25-b31 of L_x;  16<=i<=63  because of normalization.
*                5- a = bit10-b24
*                6- i -=16
*                7- L_y = table[i]<<16 - (table[i] - table[i+1]) * a * 2
*                8- L_y >>= exponent
*
********************************************************************************
*/
Word32 Inv_sqrt (       /* (o) : output value   */
    Word32 L_x          /* (i) : input value    */
)
{
   Word32 exp, i;

   if( L_x <= 0 )
      return 0x3fffffffL;

   exp = norm_l( L_x );
   L_x <<= exp;      /* L_x is normalize */

   exp = 30 - exp;

   if((exp & 1) == 0)            /* If exponent even -> shift right */
      L_x >>= 1;

   L_x >>= 10;
   i = (L_x >> 15) - 16;         /* Extract b25-b31 */

   L_x = qsub( inv_sqrt_tab[i], smulbb(inv_sqrt_tab_diff[i], L_x & 0x7fff ) );

   exp >>= 1;

   return ( L_x >> exp );     /* denormalization */
}
/*
********************************************************************************
*      Purpose          : 32 x 32 and 32 x 16 bit DPF multiy & accumulate
*                         (similar as Mpy_32 and Mpy_32_16 in oper_32b.c)
********************************************************************************
*/
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
#if 0
Word32 Mac_32(Word32 L32a, Word16 hi1, Word16 lo1, Word16 hi2, Word16 lo2)
{
   L32a = L_mac (L32a, hi1, hi2);
   L32a = L_mac (L32a, mult (hi1, lo2), 1);
   L32a = L_mac (L32a, mult (lo1, hi2), 1);
   return (L32a);
}
#endif

Word32 Mac_32_o(Word32 L32a, Word32 x, Word32 n)
{
	Word32 L_32, L_tmp;
   L_32 = qdadd(L32a, smultt(x,n) );

   L_tmp = smultb(x, n);
   L_tmp = qadd(L_tmp, L_tmp);
   L_tmp = L_tmp >> 16;
   L_32 = qdadd(L_32, L_tmp);

   L_tmp = smulbt(x, n);
   L_tmp = qadd(L_tmp, L_tmp);
   L_tmp = L_tmp >> 16;
   L_32 = qdadd(L_32, L_tmp);

   return (L_32);
}
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
#if 0
Word32 Mac_32_16 (Word32 L_32, Word16 hi, Word16 lo, Word16 n)
{
   L_32 = L_mac (L_32, hi, n);
   L_32 = L_mac (L_32, mult(lo, n), 1);
   return (L_32);
}
#endif

Word32 Mac_32_16_o (Word32 L32a, Word32 x, Word32 n)
{

	Word32 L_32, L_tmp;
	L_32 = qdadd(L32a , smultb(x,n));
   L_tmp = smulbb(x, n);
   L_tmp = qadd(L_tmp, L_tmp);
   L_tmp = L_tmp >> 16;
   L_32 = qdadd(L_32, L_tmp);
   return (L_32);
}

INLINE Word32 DPF_Mult( Word32 a1, Word32 a2 )
{
   a1 = smull(a1, a2);
   return qadd( a1, a1 );
}

INLINE Word32 DPF_Mult16( Word32 L_32, Word32 n )
{
   L_32 = smulwb( L_32, n );
   return qadd( L_32, L_32 );
}


//Word32 Div_32 (Word32 L_num, Word16 denom_hi, Word16 denom_lo);



/*----------------------------------------------------------------------*/
Word32 Div_32_o(Word32 L_num, Word32 denom);

INLINE Word32 L_Extract_o(Word32 L_32){
  Word32 L_tmp = (L_32 & 0x0ffff) >> 1;
  L_32 = L_32 & 0xffff0000;
  return(L_32 | L_tmp);
}

INLINE Word32 L_Comp_o(Word32 x){
  Word32 L_tmp = (x & 0x0ffff) << 1;
  x = x & 0xffff0000;
  return(x | L_tmp);
}

INLINE Word32 Mpy_32_o(Word32 x, Word32 y){
  Word32 L_tmp, L_32;
  L_32 = smultt(x, y);
  L_32 = qadd(L_32, L_32);
  L_tmp = smultb(x, y);
  L_tmp = qadd(L_tmp, L_tmp);
  L_tmp = L_tmp >> 16;
  L_32 = qdadd(L_32, L_tmp);
  L_tmp = smulbt(x, y);
  L_tmp = qadd(L_tmp, L_tmp);
  L_tmp = L_tmp >> 16;
  L_32 = qdadd(L_32, L_tmp);
  return( L_32 );
}

INLINE Word32 Mpy_32_16_o(Word32 x, Word32 n){
  Word32 L_32, L_tmp;
  L_32 = smultb(x, n);
  L_32 = qadd(L_32, L_32);
  L_tmp = smulbb(x, n);
  L_tmp = qadd(L_tmp, L_tmp);
  L_tmp = L_tmp >> 16;
  L_32 = qdadd(L_32, L_tmp);
  return( L_32 );
}

#endif



















