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
 *   az_lsp.c
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

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : az_lsp.c
*      Purpose          : Compute the LSPs from the LP coefficients
*
********************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "az_lsp.h"
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "copy.h"
/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/

#define NC   M/2                  /* M = LPC order, NC = M/2 */

/*-------------------------------------------------------------*
 *  Table for az_lsf()                                         *
 *                                                             *
 * grid[0] = 1.0;                                              *
 * grid[grid_points+1] = -1.0;                                 *
 * for (i = 1; i < grid_points; i++)                           *
 *   grid[i] = cos((6.283185307*i)/(2.0*grid_points));         *
 *                                                             *
 *-------------------------------------------------------------*/

#define grid_points 60

static const Word16 AMR_grid[grid_points + 1] =
{
    32760, 32723, 32588, 32364, 32051, 31651,
    31164, 30591, 29935, 29196, 28377, 27481,
    26509, 25465, 24351, 23170, 21926, 20621,
    19260, 17846, 16384, 14876, 13327, 11743,
    10125, 8480, 6812, 5126, 3425, 1714,
    0, -1714, -3425, -5126, -6812, -8480,
    -10125, -11743, -13327, -14876, -16384, -17846,
    -19260, -20621, -21926, -23170, -24351, -25465,
    -26509, -27481, -28377, -29196, -29935, -30591,
    -31164, -31651, -32051, -32364, -32588, -32723,
    -32760
};




/*
********************************************************************************
*                         LOCAL PROGRAM CODE
********************************************************************************
*/
/*
**************************************************************************
*
*  Function    : Chebps
*  Purpose     : Evaluates the Chebyshev polynomial series
*  Description : - The polynomial order is   n = m/2 = 5
*                - The polynomial F(z) (F1(z) or F2(z)) is given by
*                   F(w) = 2 exp(-j5w) C(x)
*                  where
*                   C(x) = T_n(x) + f(1)T_n-1(x) + ... +f(n-1)T_1(x) + f(n)/2
*                  and T_m(x) = cos(mw) is the mth order Chebyshev
*                  polynomial ( x=cos(w) )
*  Returns     : C(x) for the input x.
*
**************************************************************************
*/
#if (AMR_OPT_VER == 2)
static Word32 Chebps (Word32 x,
                      Word16 f[] /* (n) */ )
{
   Word32 b1,b2,s,t0;

   s = ((Word32 *)f)[0];
   t0 = (x << 10);          					/* 2*x                 */
   t0 = qdadd (t0, smultb( s, 8192));     /* + f[1]              */
   b1 = L_Extract_o (t0);  					/* b1 = 2*x + f[1]     */

   s = ((Word32 *)f)[1];
   t0 = Mpy_32_16_o (b1, x);    			 	/* t0 = 2.0*x*b1        */
   t0 = qadd (t0, t0);
   t0 = qsub( t0, 0x1000000 );  			/* t0 = 2.0*x*b1 - b2   */
   t0 = qdadd (t0,smulbb( s, 8192));      /* t0 = 2.0*x*b1 - b2 + f[i] */
   b2 = L_Extract_o (t0);           		/* b0 = 2.0*x*b1 - b2 + f[i]*/


   t0 = Mpy_32_16_o (b2, x);         		/* t0 = 2.0*x*b1        */
	t0 = qadd (t0, t0);
	t0 = qsub( t0, L_Comp_o(b1) );       	/* t0 = 2.0*x*b1 - b2   */
   t0 = qdadd(t0, smultb( s, 8192));      /* t0 = 2.0*x*b1 - b2 + f[i] */
   b1 = L_Extract_o (t0);                	/* b0 = 2.0*x*b1 - b2 + f[i]*/


   s = ((Word32 *)f)[2];
   t0 = Mpy_32_16_o (b1, x);         		/* t0 = 2.0*x*b1        */
   t0 = qadd (t0, t0);
	t0 = qsub( t0, L_Comp_o(b2) );			/* t0 = 2.0*x*b1 - b2   */
   t0 = qdadd (t0,smulbb( s, 8192));      /* t0 = 2.0*x*b1 - b2 + f[i] */
   b2 = L_Extract_o (t0);           		/* b0 = 2.0*x*b1 - b2 + f[i]*/

   t0 = Mpy_32_16_o (b2 , x);             /* t0 = x*b1; */
   t0 = qsub( t0, L_Comp_o(b1) );        /* t0 = x*b1 - b2   */
   t0 = qdadd (t0, smultb(s, 4096));      /* t0 = x*b1 - b2 + f[i]/2 */
   t0 = L_shl (t0, 6);

   return (t0>>16);

}
#elif (AMR_OPT_VER == 1)
static Word32 Chebps (Word32 x,
                      Word16 f[] /* (n) */ )
{
   Word32 b1,b2,s,t0;

   s = ((Word32 *)f)[0];
   t0 = (x << 10);          					/* 2*x                 */
   b1 = qdadd (t0, smultb( s, 8192));     /* + f[1]              */

   s = ((Word32 *)f)[1];
   t0 = DPF_Mult16 (b1, x);    			 	/* t0 = 2.0*x*b1        */
   t0 = qadd (t0, t0);
   t0 = qsub( t0, 0x1000000 );  			/* t0 = 2.0*x*b1 - b2   */
   b2 = qdadd (t0,smulbb( s, 8192));      /* t0 = 2.0*x*b1 - b2 + f[i] */


   t0 = DPF_Mult16 (b2, x);         		/* t0 = 2.0*x*b1        */
	t0 = qadd (t0, t0);
	t0 = qsub( t0, b1 );       				/* t0 = 2.0*x*b1 - b2   */
   b1 = qdadd(t0, smultb( s, 8192));      /* t0 = 2.0*x*b1 - b2 + f[i] */


   s = ((Word32 *)f)[2];
   t0 = DPF_Mult16 (b1, x);         		/* t0 = 2.0*x*b1        */
   t0 = qadd (t0, t0);
	t0 = qsub( t0, b2 );						/* t0 = 2.0*x*b1 - b2   */
   b2 = qdadd (t0,smulbb( s, 8192));      /* t0 = 2.0*x*b1 - b2 + f[i] */


   t0 = DPF_Mult16 (b2 , x);              /* t0 = x*b1; */
   t0 = qsub( t0, b1 );        				/* t0 = x*b1 - b2   */
   t0 = qdadd (t0, smultb(s, 4096));      /* t0 = x*b1 - b2 + f[i]/2 */
   t0 = L_shl (t0, 6);

   return (t0>>16);

}

#endif


/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*
**************************************************************************
*
*  Function    : Az_lsp
*  Purpose     : Compute the LSPs from  the LP coefficients
*
**************************************************************************
*/
void Az_lsp (
    AMR_Encode_FrameState *Enc,
    Word16 a[],         /* (i)  : predictor coefficients (MP1)               */
    Word16 lsp[],       /* (o)  : line spectral pairs (M)                    */
    Word16 old_lsp[]    /* (i)  : old lsp[] (in case not found 10 roots) (M) */
)
{
 	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i, j,exp,sign;
	Word32 nf, ip;
   Word32 xlow, ylow, xhigh, yhigh, xmid, ymid, xint;
   Word32 t0;
   Word32 x, y;
   Word16 *coef;

	Word16 *f1 = Enc_share->shabuf.stE.u2E.s2E.f1;
	Word16 *f2 = Enc_share->shabuf.stE.u2E.s2E.f2;

    msr_cpsr_f_0();

    /*-------------------------------------------------------------*
     *  find the sum and diff. pol. F1(z) and F2(z)                *
     *    F1(z) <--- F1(z)/(1+z**-1) & F2(z) <--- F2(z)/(1-z**-1)  *
     *                                                             *
     * f1[0] = 1.0;                                                *
     * f2[0] = 1.0;                                                *
     *                                                             *
     * for (i = 0; i< NC; i++)                                     *
     * {                                                           *
     *   f1[i+1] = a[i+1] + a[M-i] - f1[i] ;                       *
     *   f2[i+1] = a[i+1] - a[M-i] + f2[i] ;                       *
     * }                                                           *
     *-------------------------------------------------------------*/

	f1[0] = 1024;                   /* f1[0] = 1.0 */
   f2[0] = 1024;                   /* f2[0] = 1.0 */

	{
      Word16 f_prev_1 = f1[0];
      //Word16 f_prev_2 = f2[0];
      Word16 *pf1 = &f1[1], *pf2 = &f2[1];
      Word16 *paf = &a[1], *pab = &a[M];
	   for (i = 0; i < NC; i++)
   	{
      	Word32 f1_t = *paf++, f2_t = *pab--;

		   /* x = (a[i+1] + a[M-i]) >> 2  */
   	   x = (f1_t + f2_t) >> 2;
      	/* f1[i+1] = a[i+1] + a[M-i] - f1[i] */
	      *pf1++ = f_prev_1 = sub(x , f_prev_1);

	   	/* x = (a[i+1] - a[M-i]) >> 2 */
   	   x = (f1_t - f2_t) >> 2;
	      /* f2[i+1] = a[i+1] - a[M-i] + f2[i] */
   	   *pf2++ = add(x , f2[i]); //*pf2++ = f_prev_2 = add(x , f2[i]);
   	}
	}

   /*-------------------------------------------------------------*
    * find the LSPs using the Chebychev pol. evaluation           *
    *-------------------------------------------------------------*/

   nf = 0;                         /* number of found frequencies */
   ip = 0;                         /* indicator for f1 or f2      */

   coef = f1;

   xlow = AMR_grid[0];
   ylow = Chebps (xlow, coef);

   j = 0;

   /* while ( (nf < M) && (j < grid_points) ) */
   while ( (nf < M)  &&  (j < grid_points) )
   {
   	j++;
      xhigh = xlow;
      yhigh = ylow;
      xlow = AMR_grid[j];
      ylow = Chebps (xlow, coef);

      if ( (ylow * yhigh) <= 0)
      {
      	/* divide 4 times the interval */
         for (i = 0; i < 4; i++)
         {
         	/* xmid = (xlow + xhigh)/2 */
            xmid = ( (xlow >> 1) +  (xhigh >> 1));
            ymid = Chebps (xmid, coef);

            if ( (ylow * ymid) <= 0)
            {
            	yhigh = ymid;
               xhigh = xmid;
            }
            else
            {
	            ylow = ymid;
               xlow = xmid;
            }
         }

         /*-------------------------------------------------------------*
          * Linear interpolation                                        *
          *    xint = xlow - ylow*(xhigh-xlow)/(yhigh-ylow);            *
          *-------------------------------------------------------------*/

         x = sub(xhigh , xlow);
         y = sub(yhigh , ylow);

         if (y == 0)
         {
   	      xint = xlow;
         }
         else
         {
            sign = y;
            y = abs_s (y);
            exp = norm_s (y);
            y =  (y << exp);
            y = div_s ((Word16) 16383,(Word16) y);
            t0 = smulbb (x, y);  t0 = qadd(t0,t0);
            t0 = L_shr (t0, (20 - exp));
            y = (Word16)(t0);     /* y= (xhigh-xlow)/(yhigh-ylow) */

            if (sign < 0)
                y = -(y);

            t0 = smulbb(ylow, y);  t0 = qadd(t0,t0);
            t0 = (t0 >> 11);
            xint =  (xlow - (Word16)(t0)); /* xint = xlow - ylow*y */
         }

         lsp[nf] = xint;
         xlow = xint;
         nf++;

         if (ip == 0)
         {
	         ip = 1;
            coef = f2;
         }
         else
         {
   	      ip = 0;
            coef = f1;
         }
         ylow = Chebps (xlow, coef);

   	}
	}

    /* Check if M roots found */
	if(nf < M)
   {
		AMR_Copy32((Word32 *)old_lsp, (Word32 *)lsp, M/2);
//		Copy(old_lsp, lsp, M);

   }

   return;

}

#endif

