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
 *   b_cn_cod.c
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

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : b_cn_cod.c
*      Purpose          : Contains function for comfort noise generation.
*
********************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "b_cn_cod.h"


/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"

#include "tab_window.h"
/*
********************************************************************************
*                         LOCAL CONSTANTS
********************************************************************************
*/
#define  NB_PULSE 10 /* number of random pulses in DTX operation   */



/*************************************************************************
 *
 * Hamming_cos windows for LPC analysis.
 *
 *************************************************************************/

/* window for non-EFR modesm; uses 40 samples lookahead */


/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *   FUNCTION NAME: pseudonoise
 *
 *************************************************************************/
Word16 pseudonoise (
    Word32 *shift_reg, /* i/o : Old CN generator shift register state */
    Word16 no_bits     /* i   : Number of bits                        */
)
{
	Word32 i,Sn;
   Word16 noise_bits;

   noise_bits = 0;
   for (i = 0; i < no_bits; i++)
   {
      /* State n == 31 */

      if ((*shift_reg & 0x00000001L) != 0)
      {
         Sn = 1;
      }
      else
      {
         Sn = 0;
      }

      /* State n == 3 */

      if ((*shift_reg & 0x10000000L) != 0)
      {
         Sn = Sn ^ 1;
      }
      else
      {
         Sn = Sn ^ 0;
      }

      noise_bits = sature(noise_bits << 1);
      noise_bits = noise_bits | ((Word16)(*shift_reg) & 1);

      *shift_reg = (*shift_reg >> 1);

      if (Sn & 1)
      {
         *shift_reg = *shift_reg | 0x40000000L;
      }
   }

   return noise_bits;

}

/***************************************************************************
*
*  Function    : build_CN_code
*
***************************************************************************/
void build_CN_code (
    Word32 *seed,         /* i/o : Old CN generator shift register state */
    Word16 cod[]          /* o   : Generated CN fixed codebook vector    */
)
{
   Word32 i, j, k;
	Word32 *ptr32;

	ptr32 = (Word32 *)cod;
	i = L_SUBFR/8;
	do
   {
   	*ptr32++ = 0;   	*ptr32++ = 0;   	*ptr32++ = 0;   	*ptr32++ = 0;
   }while(--i != 0);


   for (k = 0; k < NB_PULSE; k++)
   {
      i = pseudonoise(seed, 2);      /* generate pulse position */
      i = ((Word16)(L_mult(i, 10)) >> 1);
      i = (i + k);
      j = pseudonoise(seed, 1);      /* generate sign           */

      if (j > 0)
      {
         cod[i] = 4096;
      }
      else
      {
         cod[i] = -4096;
      }
   }

   return;
}

/*************************************************************************
 *
 *   FUNCTION NAME: build_CN_param
 *
 *************************************************************************/
void build_CN_param (
    Word16 *seed,             /* i/o : Old CN generator shift register state */
    const Word16 n_param,           /* i  : number of params */
    const Word16 param_size_table[],/* i : size of params */
    Word16 parm[]             /* o : CN Generated params */
    )
{
   Word32 i,t;
   const Word16 *p;

	t = smulbb(*seed, 31821);
	t = qadd(t,t);
   *seed = (Word16)(qadd( (t >> 1), 13849L));

   p = &window_200_40[*seed & 0x7F];

   for(i=0; i< n_param;i++)
	{
      parm[i] = *p++ & ~(0xFFFF << param_size_table[i]);
   }

}

#endif

