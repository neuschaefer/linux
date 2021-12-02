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
 *   lsp_az.c
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

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : lsp_az.c
*      Purpose          : Converts from the line spectral pairs (LSP) to
*                       : LP coefficients, for a 10th order filter.
*      Description      :
*                 - Find the coefficients of F1(z) and F2(z) (see Get_lsp_pol)
*                 - Multiply F1(z) by 1+z^{-1} and F2(z) by 1-z^{-1}
*                 - A(z) = ( F1(z) + F2(z) ) / 2
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "lsp_az.h"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/

/*
********************************************************************************
*                         LOCAL PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  Get_lsp_pol
 *
 *  PURPOSE:  Find the polynomial F1(z) or F2(z) from the LSPs.
 *            If the LSP vector is passed at address 0  F1(z) is computed
 *            and if it is passed at address 1  F2(z) is computed.
 *
 *  DESCRIPTION:
 *       This is performed by expanding the product polynomials:
 *
 *           F1(z) =   product   ( 1 - 2 lsp[i] z^-1 + z^-2 )
 *                   i=0,2,4,6,8
 *           F2(z) =   product   ( 1 - 2 lsp[i] z^-1 + z^-2 )
 *                   i=1,3,5,7,9
 *
 *       where lsp[] is the LSP vector in the cosine domain.
 *
 *       The expansion is performed using the following recursion:
 *
 *            f[0] = 1
 *            b = -2.0 * lsp[0]
 *            f[1] = b
 *            for i=2 to 5 do
 *               b = -2.0 * lsp[2*i-2];
 *               f[i] = b*f[i-1] + 2.0*f[i-2];
 *               for j=i-1 down to 2 do
 *                   f[j] = f[j] + b*f[j-1] + f[j-2];
 *               f[1] = f[1] + b;
 *
 *************************************************************************/
#if (AMR_OPT_VER == 2)
static void Get_lsp_pol (Word16 *lsp, Word32 *f)
{
	Word32 i, j ,h;
   Word32 t0;


   /* f[0] = 1.0; */
   *f++ = (4096 * 2048) << 1 ;
   *f++ = -((*lsp * 512) << 1);    /* f[1] =  -2.0 * lsp[0];  */

   lsp += 2;                              /* Advance lsp pointer     */

   for (i = 2; i <= 5; i++)
   {
   	*f = f[-2];

      for (j = 1; j < i; j++)
      {
         h = L_Extract_o (*(f-1));
         t0 = Mpy_32_16_o (h, *lsp); 		/* t0 = f[-1] * lsp    */
			*f = qdsub (qadd(*f,*(f-2)) , t0);
			f--;
		}

      *f = qdsub(*f, smulbb(*lsp, 512));         	 	/* *f -= lsp<<9     */
      f += i;                            		/* Advance f pointer   */
      lsp += 2;                          		/* Advance lsp pointer */

	}

   return;
}

#elif (AMR_OPT_VER == 1)

static void Get_lsp_pol (Word16 *lsp, Word32 *f)
{
	Word32 i, j ,h;
   Word32 t0;

   /* f[0] = 1.0; */
   *f++ = (4096 * 2048) << 1 ;
   *f++ = -((*lsp * 512) << 1);    /* f[1] =  -2.0 * lsp[0];  */

   lsp += 2;                              /* Advance lsp pointer     */

   for (i = 2; i <= 5; i++)
   {
   	*f = f[-2];

      for (j = 1; j < i; j++)
      {
         h  = (*(f-1));
         t0 = DPF_Mult16 (h, *lsp); 		/* t0 = f[-1] * lsp    */
			*f = qdsub (qadd(*f,*(f-2)) , t0);
			f--;
		}

      *f = qdsub(*f, smulbb(*lsp, 512));         	 	/* *f -= lsp<<9     */
      f += i;                            		/* Advance f pointer   */
      lsp += 2;                          		/* Advance lsp pointer */

	}

   return;
}


#endif
/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:   Lsp_Az
 *
 *  PURPOSE:  Converts from the line spectral pairs (LSP) to
 *            LP coefficients, for a 10th order filter.
 *
 *  DESCRIPTION:
 *     - Find the coefficients of F1(z) and F2(z) (see Get_lsp_pol)
 *     - Multiply F1(z) by 1+z^{-1} and F2(z) by 1-z^{-1}
 *     - A(z) = ( F1(z) + F2(z) ) / 2
 *
 *************************************************************************/
void Lsp_Az (
	 Word32 *f1t,
	 Word32 *f2t,

    Word16 lsp[],        /* (i)  : line spectral frequencies            */
    Word16 a[]           /* (o)  : predictor coefficients (order = 10)  */
)
{
	Word32 i, s , t;
   Word32 t0;
//   Word32 f1[6];
//   Word32 f2[6];
	Word32 *f1 = f1t;
	Word32 *f2 = f2t;

   Get_lsp_pol (&lsp[0], f1);
   Get_lsp_pol (&lsp[1], f2);

   a[0] = 4096;
   for (i = 5; i > 0; i--)
   {
   	s = qadd(f1[i], f1[i - 1]);				/* f1[i] += f1[i-1]; */
		s = qadd(s, (1<<12) );						/*rounding*/
		t = qsub(f2[i], f2[i - 1]);				/* f2[i] -= f2[i-1]; */

   	t0      =  qadd(s, t);         	  	/* f1[i] + f2[i] */
      a[i]    =  (Word16)( (t0 >> 13));

      t0      =  qsub(s, t);           		/* f1[i] - f2[i] */
      a[11-i] =  (Word16)( (t0 >> 13));

   }

   return;

}

#endif

