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
 *   WBAMR_OPER_32B.C
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR-WB codec
 *
 * Author:
 * -------
 *   Eddy Wu
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$  
 * $Log:$
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_oper_32b.h"
#include "wbamr_sections.h"

 
 /*****************************************************************************
 *  This file contains operations in double precision.                       *
 *  These operations are not standard double precision operations.           *
 *  They are used where single precision is not enough but the full 32 bits  *
 *  precision is not necessary. For example, the function WBAMR_Div_32() has a
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

/*****************************************************************************
 *                                                                           *
 *  Function WBAMR_L_Extract()                                               *
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

WBAMR_CODE_SECTION void WBAMR_L_Extract ( 
  fract32 L_32, 
  fract16 *hi, 
  fract16 *lo
 )

{
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
   *hi = (fract16)(L_32 >> 16);
   *lo = (fract16)((unsigned int)(L_32 << 16) >> 17);
#else
  *hi = Extract_hi_fr1x32 (L_32);
  *lo = Extract_lo_fr1x32 (WBAMR_L_msu (Shr_fr1x32 (L_32, 1), *hi, 16384));
#endif
  /*   *lo = Shr_fr1x32 ((L_32&0xffff), 1);*/
  return;
}


/*****************************************************************************
 *                                                                           *
 *  Function WBAMR_L_Comp()                                                  *
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
 *             32 bit long signed integer (fract32) whose value falls in the *
 *             range : 0x8000 0000 <= L_32 <= 0x7fff fff0.                   *
 *                                                                           *
 *****************************************************************************
*/

WBAMR_CODE_SECTION fract32 WBAMR_L_Comp (
  fract16 hi,
  fract16 lo
 )
{
  fract32 L_32;

  L_32 = WBAMR_L_deposit_h ( hi );
  return ( Mac_fr16_32 ( L_32, lo, 1 ) );    /* = hi<<16 + lo<<1 */
}


/*****************************************************************************
 * Function WBAMR_Mpy_32()                                                   *
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


WBAMR_CODE_SECTION fract32 WBAMR_Mpy_32 (
  fract16 hi1, 
  fract16 lo1, 
  fract16 hi2, 
  fract16 lo2
 )

{
  fract32 L_32;

  L_32 = Mult_fr1x32 ( hi1, hi2 );
  L_32 = Mac_fr16_32 ( L_32, Mult_fr1x16 ( hi1, lo2 ), 1 );
  L_32 = Mac_fr16_32 ( L_32, Mult_fr1x16 ( lo1, hi2 ), 1 );

  return ( L_32 );
}


/*****************************************************************************
 * Function WBAMR_Mpy_32_16()                                                *
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


WBAMR_CODE_SECTION fract32 WBAMR_Mpy_32_16 (
  fract16 hi, 
  fract16 lo, 
  fract16 n
 )

{
  fract32 L_32;

  L_32 = Mult_fr1x32 ( hi, n );
  L_32 = Mac_fr16_32 ( L_32, Mult_fr1x16 ( lo, n ), 1 );

  return ( L_32 );
}


/*****************************************************************************
 *                                                                           *
 *   Function Name : WBAMR_Div_32                                            *
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
 *             32 bit long signed integer (fract32) whose value falls in the *
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
 *             32 bit long signed integer (fract32) whose value falls in the *
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

WBAMR_CODE_SECTION fract32 WBAMR_Div_32 (
  fract32 L_num, 
  fract16 denom_hi,
  fract16 denom_lo
 )

{
  fract16 approx, hi, lo, n_hi, n_lo;
  fract32 L_32;

  /* First approximation: 1 / L_denom = 1/denom_hi */
  approx = Divs_fr1x16 ( (fract16) 0x3fff, denom_hi );

  /* 1/L_denom = approx * (2.0 - L_denom * approx) */
  L_32 = WBAMR_Mpy_32_16 ( denom_hi, denom_lo, approx );

  L_32 = Sub_fr1x32 ( (fract32) 0x7fffffffL, L_32 );

  WBAMR_L_Extract ( L_32, &hi, &lo );

  L_32 = WBAMR_Mpy_32_16 ( hi, lo, approx );

  /* L_num * (1/L_denom) */
  WBAMR_L_Extract ( L_32, &hi, &lo );
  WBAMR_L_Extract ( L_num, &n_hi, &n_lo );
  L_32 = WBAMR_Mpy_32 ( n_hi, n_lo, hi, lo );
  L_32 = Shl_fr1x32 ( L_32, 2 );

  return ( L_32 );
}

#endif

