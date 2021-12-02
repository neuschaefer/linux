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
 *   inter_36.h
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
*      File             : lpc.c
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "lpc.h"


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
#include "tab_window.h"


/*-----------------------------------------------------*
 | Table of lag_window for autocorrelation.            |
 |                                                     |
 | noise floor = 1.0001   = (0.9999  on r[1] ..r[10])  |
 | Bandwitdh expansion = 60 Hz                         |
 |                                                     |
 |                                                     |
 | lag_wind[0] =  1.00000000    (not stored)           |
 | lag_wind[1] =  0.99879038                           |
 | lag_wind[2] =  0.99546897                           |
 | lag_wind[3] =  0.98995781                           |
 | lag_wind[4] =  0.98229337                           |
 | lag_wind[5] =  0.97252619                           |
 | lag_wind[6] =  0.96072036                           |
 | lag_wind[7] =  0.94695264                           |
 | lag_wind[8] =  0.93131179                           |
 | lag_wind[9] =  0.91389757                           |
 | lag_wind[10]=  0.89481968                           |
 -----------------------------------------------------*/
__align(4)
static const Word32 AMR_lag[M] = {
    (32728<<16)+11904,  /* 0.998790406653 */
    (32619<<16)+17280,  /* 0.995468979207 */
    (32438<<16)+30720,  /* 0.989957793633 */
    (32187<<16)+25856,  /* 0.982293355135 */
    (31867<<16)+24192,  /* 0.972526205446 */
    (31480<<16)+28992,  /* 0.960720367768 */
    (31029<<16)+24384,  /* 0.946952647902 */
    (30517<<16)+ 7360,  /* 0.931311802222 */
    (29946<<16)+19520,  /* 0.913897585113 */
    (29321<<16)+14784  /* 0.894819690160 */
};


//static Word16 old_A[M + 1];     /* Last A(z) for case of unstable filter */


/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
*
*  Function:   lpc_init
*
**************************************************************************
*/


/*
**************************************************************************
*
*  Function    : autocorr
*  Purpose     : Compute autocorrelations of signal with windowing
*
**************************************************************************
*/
#if (AMR_OPT_VER == 2)
void Autocorr (
   Word16 x[],            /* (i)    : Input signal (L_WINDOW)            */
	Word32 r[],
	Word16 y[],
   const Word16 wind[]    /* (i)    : window for LPC analysis (L_WINDOW) */
)
{
   register Word32 i, j, sum;
	Word32 norm;
	Word32 *ptr32w,*lptr;
   Word32 t, s;
   Word16 overfl_shft;
   Word16 *sptr;

   /* Windowing of signal */
	ptr32w = (Word32 *)wind;
	lptr   = (Word32 *)x;
	sptr	 = (Word16 *)y;
	i = L_WINDOW/4;
	do
   {
      s = *lptr++;  t = *ptr32w++;
   	*sptr++ = (Word16)( (qadd(0x4000, smulbb(s, t)))>>15  );
   	*sptr++ = (Word16)( (qadd(0x4000, smultt(s, t)))>>15  );
      s = *lptr++;  t = *ptr32w++;
   	*sptr++ = (Word16)( (qadd(0x4000, smulbb(s, t)))>>15  );
   	*sptr++ = (Word16)( (qadd(0x4000, smultt(s, t)))>>15  );
	}while(--i != 0);


   /* Compute r[0] and test for overflow */

	overfl_shft = 0;
	lptr = (Word32 *)y;
   sum = 0L;
	i = L_WINDOW/4;
	do
   {
   	s = *lptr++;
   	sum = qdadd (sum, smulbb(s,s) );
   	sum = qdadd (sum, smultt(s,s) );
   	s = *lptr++;
   	sum = qdadd (sum, smulbb(s,s) );
   	sum = qdadd (sum, smultt(s,s) );
	}while(--i !=0);

	while( sum == MAX_32)
  	{
   	overfl_shft = (overfl_shft + 4);
	   sum = 0L;
    	sptr = (Word16 *)y;
	   i = L_WINDOW;
		do
      {
	      s = *sptr >> 2;
   	   sum = qdadd( sum, smulbb( s, s));
      	*sptr++ = s;
      }while(--i != 0);
  	}

   sum = sum + 1;             /* Avoid the case of all zeros */

    /* Normalization of r[0] */
   norm = norm_l (sum);
	sum = L_shl (sum, norm);

	lptr = r;
	*lptr++ = L_Extract_o(sum);

    /* r[1] to r[m] */

  i = 0;
  do
  {
    Word32 *yp, *ypp;
    Word32 sum2;
    yp = (Word32 *)y;
    ypp = yp + i;
    s = *yp++;
    sum = smulbt(s, *ypp);
    sum = qadd(sum, sum);
    sum2 = 0;

    j = L_WINDOW/2-1-i;
    do {
      t = *(++ypp);
      sum2 = qdadd(sum2, smulbb(s, t));
      sum2 = qdadd(sum2, smultt(s, t));
      sum = qdadd(sum, smultb(s, t));
      s = *yp++;
      sum = qdadd(sum, smulbt(s, t));
    } while(--j != 0);

    *lptr++ = L_Extract_o(L_shl_e(sum, norm));
    *lptr++ = L_Extract_o(L_shl_e(sum2, norm));
  } while( ++i < M/2);


}

#elif (AMR_OPT_VER == 1)

void Autocorr (
   Word16 x[],            /* (i)    : Input signal (L_WINDOW)            */
	Word32 r[],
	Word16 y[],
   const Word16 wind[]    /* (i)    : window for LPC analysis (L_WINDOW) */
)
{
   register Word32 i, j, sum;
	Word32 norm;
	Word32 *ptr32w,*lptr;
   Word32 t, s;
   Word16  overfl_shft;
   Word16 *sptr;


    /* Windowing of signal */
	ptr32w = (Word32 *)wind;
	lptr   = (Word32 *)x;
	sptr	 = (Word16 *)y;
	i = L_WINDOW/4;
	do
   {
      s = *lptr++;  t = *ptr32w++;
   	*sptr++ = (Word16)( (qadd(0x4000, smulbb(s, t)))>>15  );
   	*sptr++ = (Word16)( (qadd(0x4000, smultt(s, t)))>>15  );
      s = *lptr++;  t = *ptr32w++;
   	*sptr++ = (Word16)( (qadd(0x4000, smulbb(s, t)))>>15  );
   	*sptr++ = (Word16)( (qadd(0x4000, smultt(s, t)))>>15  );
	}while(--i != 0);


   /* Compute r[0] and test for overflow */
	overfl_shft = 0;
	lptr = (Word32 *)y;
   sum = 0L;
	i = L_WINDOW/4;
	do
   {
   	s = *lptr++;
   	sum = qdadd (sum, smulbb(s,s) );
   	sum = qdadd (sum, smultt(s,s) );
   	s = *lptr++;
   	sum = qdadd (sum, smulbb(s,s) );
   	sum = qdadd (sum, smultt(s,s) );
	}while(--i !=0);

	while( sum == MAX_32)
  	{
   	overfl_shft = (overfl_shft + 4);
	   sum = 0L;
    	sptr = (Word16 *)y;
	   i = L_WINDOW;
		do
      {
	      s = *sptr >> 2;
   	   sum = qdadd( sum, smulbb( s, s));
      	*sptr++ = s;
      }while(--i != 0);
  	}

   sum = sum + 1;             /* Avoid the case of all zeros */

    /* Normalization of r[0] */
   norm = norm_l (sum);
	sum = L_shl_e (sum, norm);

	lptr = r;
	*lptr++ = sum ;

    /* r[1] to r[m] */
  i = 0;
  do
  {
    Word32 *yp, *ypp;
    Word32 sum2;
    yp = (Word32 *)y;
    ypp = yp + i;
    s = *yp++;
    sum = smulbt(s, *ypp);
    sum = qadd(sum, sum);
    sum2 = 0;

    j = L_WINDOW/2-1-i;
    do {
      t = *(++ypp);
      sum2 = qdadd(sum2, smulbb(s, t));
      sum2 = qdadd(sum2, smultt(s, t));
      sum = qdadd(sum, smultb(s, t));
      s = *yp++;
      sum = qdadd(sum, smulbt(s, t));
    } while(--j != 0);

    *lptr++ = L_shl_e(sum, norm);
    *lptr++ = L_shl_e(sum2, norm);
  } while( ++i < M/2);


}


#endif



/*************************************************************************
 *
 *   FUNCTION:  Levinson()
 *
 *   PURPOSE:  Levinson-Durbin algorithm in double precision. To compute the
 *             LP filter parameters from the speech autocorrelations.
 *
 *   DESCRIPTION:
 *       R[i]    autocorrelations.
 *       A[i]    filter coefficients.
 *       K       reflection coefficients.
 *       Alpha   prediction gain.
 *
 *       Initialisation:
 *               A[0] = 1
 *               K    = -R[1]/R[0]
 *               A[1] = K
 *               Alpha = R[0] * (1-K**2]
 *
 *       Do for  i = 2 to M
 *
 *            S =  SUM ( R[j]*A[i-j] ,j=1,i-1 ) +  R[i]
 *
 *            K = -S / Alpha
 *
 *            An[j] = A[j] + K*A[i-j]   for j=1 to i-1
 *                                      where   An[i] = new A[i]
 *            An[i]=K
 *
 *            Alpha=Alpha * (1-K**2)
 *
 *       END
 *
 *************************************************************************/
#if (AMR_OPT_VER == 2)
void Levinson (
   AMR_Encode_FrameState *Enc,
	Word16 old_A[],
	Word32 R[],
   Word16 A[],        /* o : A[m]    LPC coefficients  (m = 10)       */
   Word16 rc[]        /* o : rc[4]   First 4 reflection coefficients  */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i, j;
   Word32 t0, t1, t2;            /* temporary variable                    */
	Word32 K,alp;
   Word32 alp_exp; /* Prediction gain; hi lo and exponent   */
	Word32 *AD = Enc_share->shabuf.stE.u2E.s1E.AD ;
	Word32 *An = Enc_share->shabuf.stE.u2E.s1E.An ;


   /* K = A[1] = -R[1] / R[0] */
   t1  = L_Comp_o(R[1]);
   t2  = L_abs(t1);                    /* abs R[1]         */
   t0  = Div_32_o(t2, R[0]);           /* R[1]/R[0]        */

   if (t1 > 0)
      t0 = L_negate (t0);              /* -R[1]/R[0]       */

   K = L_Extract_o(t0);           		/* K in DPF         */

   rc[0] = (Word16)(qadd(t0,0x8000) >> 16);

   t0 = (t0 >> 4);                 /* A[1] in          */
	AD[1] = L_Extract_o(t0);     /* A[1] in DPF      */


   /*  Alpha = R[0] * (1-K**2) */
	t0 = Mpy_32_o(K, K); 		         /* K*K             */
   t0 = L_abs (t0);                    /* Some case <0 !! */
   t0 = qsub( (Word32)0x7fffffffL, t0 );/* 1 - K*K        */
   t0 = Mpy_32_o(R[0], L_Extract_o(t0)); /* Alpha in        */

   /* Normalize Alpha */
   alp_exp = norm_l (t0);
   t0 = L_shl (t0, alp_exp);
	alp = L_Extract_o(t0);

   /*--------------------------------------*
    * ITERATIONS  I=2 to M                 *
    *--------------------------------------*/

    for (i = 2; i <= M; i++)
    {
		{
      	Word32 *pwR = &R[1], *pwAD = &AD[i-1];
	      t0 = 0;
   	   j = i-1;
      	do
			{
	        t0 = qadd(t0, Mpy_32_o(*pwR++, *pwAD--));
   	   } while(--j != 0);

	      t0 = L_shl (t0, 4);              /* result in Q27 -> convert to Q31 */
	      t0 = qadd(t0, L_Comp_o(R[i]));  /* add R[i] in Q31                 */
   	}

       /* K = -t0 / Alpha */
       t1 = L_abs (t0);
       t2 = Div_32_o(t1, alp);  /* abs(t0)/Alpha              */

       if (t0 > 0)
          t2 = L_negate (t2);         /* K =-t0/Alpha                */

       t2 = L_shl(t2, alp_exp);       /* denormalize; compare to Alpha */
       K = L_Extract_o(t2);       		/* K in DPF                      */

       if(i < 5)
       {
          rc[i - 1] = (Word16)(qadd(t2,0x8000) >> 16);
       }
       /* Test for unstable filter. If unstable keep old A(z) */

       if(abs_s((Word16)(K >> 16)) > 32750)
       {
          for (j = 0; j <= M; j++)
          {
             A[j] = old_A[j];
          }

          for (j = 0; j < 4; j++)
          {
             rc[j] = 0;
          }

          return;
       }
       /*------------------------------------------*
        *  Compute new LPC coeff. -> An[i]         *
        *  An[j]= A[j] + K*A[i-j]     , j=1 to i-1 *
        *  An[i]= K                                *
        *------------------------------------------*/
		{
			Word32 *pwAD = &AD[1], *pwAD2 = &AD[i-1], *pwAn = &An[1];
			j = i-1;
			do
			{
				Word32 L_tmp = qadd(L_Comp_o(*pwAD++), Mpy_32_o(K, *pwAD2--));
				*pwAn++ = L_Extract_o(L_tmp);
			} while(--j != 0);
			t2 = (t2 >> 4);                  /* An[i] = K in Q31 ->convert to Q27  */
			*pwAn = L_Extract_o(t2);
		}

		/*  Alpha = Alpha * (1-K**2) */
	   t0 = Mpy_32_o(K, K);          /* K*K             */
      t0 = L_abs (t0);                        /* Some case <0 !! */
      t0 = qsub( (Word32)0x7fffffffL, t0 );  /* 1 - K*K        */
		t0 = Mpy_32_o(alp, L_Extract_o(t0));    /* DPF format      */

      /* Normalize Alpha */
      j = norm_l (t0);
      t0 = (t0 << j);
      alp = L_Extract_o(t0);                  /* DPF format    */
      alp_exp = (alp_exp + j);             /* Add normalization to                                                  alp_exp */

       /* A[j] = An[j] */

		AMR_Copy32(&An[1], &AD[1], i);

	}

	{
   	Word32 *pwAD = &AD[1];
		Word16 *pwA = &A[1], *pwold_A = &old_A[1];
		A[0] = 4096;
		i = M;
		do
		{
			Word32 L_tmp = L_Comp_o(*pwAD++);
			*pwold_A++ = *pwA++ = (Word16)(qadd(qadd(L_tmp, L_tmp),0x8000) >> 16);
		} while(--i != 0);
	}

	return;

}

#elif (AMR_OPT_VER == 1)

void Levinson (
   AMR_Encode_FrameState *Enc,
	Word16 old_A[],
	Word32 R[],
   Word16 A[],        /* o : A[m]    LPC coefficients  (m = 10)       */
   Word16 rc[]        /* o : rc[4]   First 4 reflection coefficients  */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i, j;
   Word32 t0, t1, t2;            /* temporary variable                    */
	Word32 K,alp;
   Word32 alp_exp; /* Prediction gain; hi lo and exponent   */
	Word32 *AD = Enc_share->shabuf.stE.u2E.s1E.AD ;
	Word32 *An = Enc_share->shabuf.stE.u2E.s1E.An ;


   /* K = A[1] = -R[1] / R[0] */
   t1  = (R[1]);
   t2  = L_abs(t1);                    /* abs R[1]         */
   t0  = DPF_Div(t2, R[0]);           /* R[1]/R[0]        */

   if (t1 > 0)
      t0 = L_negate(t0);              /* -R[1]/R[0]       */
   K = (t0);           		/* K in DPF         */

   rc[0] = (Word16)(qadd(t0,0x8000) >> 16);

   t0 = (t0 >> 4);                 /* A[1] in          */
	AD[1] = (t0);     /* A[1] in DPF      */


   /*  Alpha = R[0] * (1-K**2) */
	t0 = DPF_Mult(K, K); 		         /* K*K             */
//   t0 = L_abs (t0);                    /* Some case <0 !! */
   t0 = qsub( (Word32)0x7fffffffL, t0 );/* 1 - K*K        */
   t0 = DPF_Mult(R[0], (t0)); /* Alpha in        */

   /* Normalize Alpha */
   alp_exp = norm_l (t0);
   t0 = (t0 << alp_exp);
	alp = (t0);

   /*--------------------------------------*
    * ITERATIONS  I=2 to M                 *
    *--------------------------------------*/

    for (i = 2; i <= M; i++)
    {
       /* t0 = SUM ( R[j]*A[i-j] ,j=1,i-1 ) +  R[i] */

		{
      	Word32 *pwR = &R[1], *pwAD = &AD[i-1];
	      t0 = 0;
   	   j = i-1;
      	do
			{
	        t0 = qadd(t0, DPF_Mult(*pwR++, *pwAD--));
   	   } while(--j != 0);

	      t0 = L_shl (t0, 4);              /* result in Q27 -> convert to Q31 */
	      t0 = qadd(t0, (R[i]));  /* add R[i] in Q31                 */
   	}

       /* K = -t0 / Alpha */
       t1 = L_abs (t0);
       t2 = DPF_Div(t1, alp);  /* abs(t0)/Alpha              */

       if (t0 > 0)
          t2 = L_negate (t2);         /* K =-t0/Alpha                */

       t2 = L_shl(t2, alp_exp);       /* denormalize; compare to Alpha */
       K = (t2);       	            /* K in DPF                      */

       if(i < 5)
       {
          rc[i - 1] = (Word16)(qadd(t2,0x8000) >> 16);
       }
       /* Test for unstable filter. If unstable keep old A(z) */

       if(abs_s((Word16)(K >> 16)) > 32750)
       {
          for (j = 0; j <= M; j++)
          {
             A[j] = old_A[j];
          }

          for (j = 0; j < 4; j++)
          {
             rc[j] = 0;
          }

          return;
       }
       /*------------------------------------------*
        *  Compute new LPC coeff. -> An[i]         *
        *  An[j]= A[j] + K*A[i-j]     , j=1 to i-1 *
        *  An[i]= K                                *
        *------------------------------------------*/

		{
			Word32 *pwAD = &AD[1], *pwAD2 = &AD[i-1], *pwAn = &An[1];
			j = i-1;
			do
			{
				Word32 L_tmp = qadd((*pwAD++), DPF_Mult(K, *pwAD2--));
				*pwAn++ = (L_tmp);
			} while(--j != 0);
			t2 = (t2 >> 4);                  /* An[i] = K in Q31 ->convert to Q27  */
			*pwAn = (t2);
		}

		/*  Alpha = Alpha * (1-K**2) */
	   t0 = DPF_Mult(K, K);          /* K*K             */
//      t0 = L_abs (t0);                        /* Some case <0 !! */
      t0 = qsub( (Word32)0x7fffffffL, t0 );  /* 1 - K*K        */
		t0 = DPF_Mult(alp, (t0));               /* DPF format      */

      /* Normalize Alpha */
      j = norm_l (t0);
      t0 =  (t0 << j);
      alp = (t0);                  /* DPF format    */
      alp_exp = (alp_exp + j);             /* Add normalization to                                                  alp_exp */

       /* A[j] = An[j] */

		AMR_Copy32(&An[1], &AD[1], i);

	}

	{
   	Word32 *pwAD = &AD[1];
		Word16 *pwA = &A[1], *pwold_A = &old_A[1];
		A[0] = 4096;
		i = M;
		do
		{
			Word32 L_tmp = (*pwAD++);
			*pwold_A++ = *pwA++ = (Word16)(qadd(qadd(L_tmp, L_tmp),0x8000) >> 16);
		} while(--i != 0);
	}

	return;

}
#endif


/*************************************************************************
 *
 *  FUNCTION:  Lag_window()
 *
 *  PURPOSE:  Lag windowing of autocorrelations.
 *
 *  DESCRIPTION:
 *         r[i] = r[i]*lag_wind[i],   i=1,...,10
 *
 *     r[i] and lag_wind[i] are in special double precision format.
 *     See "oper_32b.c" for the format.
 *
 *************************************************************************/
#if (AMR_OPT_VER == 2)
void Lag_window (Word32 r[])
{
	Word32 i;
	Word32 *pwr = &r[1], *lagp = (Word32 *)AMR_lag;
	Word32 L_tmp = *pwr;

	i=M;
	do
	{
		L_tmp = L_Extract_o(Mpy_32_o(*pwr, *lagp++));
		*pwr++  = L_tmp;
	} while(--i != 0);

   return;


}

#elif (AMR_OPT_VER == 1)

void Lag_window (Word32 r[])
{

	Word32 i;
	Word32 *pwr = &r[1], *lagp = (Word32 *)AMR_lag;
	Word32 L_tmp = *pwr;

    msr_cpsr_f_0();
	i=M/2;
	do
	{
		L_tmp = DPF_Mult(*pwr, *lagp++);
		*pwr++  = L_tmp;
		L_tmp = DPF_Mult(*pwr, *lagp++);
		*pwr++  = L_tmp;
	} while(--i != 0);

}
#endif



void lpc_reset (Word16 old_A[])
{

	Word32 i;

	old_A[0] = 4096;
	for(i = 1; i <= M ; i++)
	{
   	old_A[i] = 0;
	}

//  	old_A[0] = 4096;
//   memset( &(old_A[1]), 0, M*sizeof(Word16) );

}

int lpc(
   AMR_Encode_FrameState *Enc,
	Word16 old_A[],
	enum Mode mode,   /* i  : coder mode                  */
   Word16 x[],       /* i  : Input signal           Q15  */
   Word16 x_12k2[],  /* i  : Input signal (EFR)     Q15  */
   Word16 a[]        /* o  : predictor coefficients Q12  */
)
{

  	AMR_Shared_Buf *Enc_share = Enc->share;

   Word16 rc[4];                  /* First 4 reflection coefficients Q15 */
                                  /* Autocorrelations low and hi      */
                                  /* No fixed Q value but normalized  */
                                  /* so that overflow is avoided      */
	Word32 *r     = Enc_share->shabuf.stE.u2E.s1E.r ;
   Word16 *y_tmp = Enc_share->shabuf.stE.u2E.s1E.y_tmp ;


   if ( mode == MR122 )
   {
       /* Autocorrelations */
       Autocorr(x_12k2, r , y_tmp , window_160_80);

       /* Lag windowing    */
       Lag_window(r);

       /* Levinson Durbin  */
       Levinson(Enc, old_A ,r, &a[MP1+1], rc);

       /* Autocorrelations */
       Autocorr(x_12k2, r, y_tmp , window_232_8);

       /* Lag windowing    */
       Lag_window(r);


       /* Levinson Durbin  */
       Levinson(Enc, old_A ,r, &a[(MP1+1) * 3], rc);


   }
   else
   {

       /* Autocorrelations */
       Autocorr(x, r, y_tmp, window_200_40);

       /* Lag windowing    */
       Lag_window(r);

       /* Levinson Durbin  */
       Levinson(Enc, old_A ,r, &a[(MP1+1) * 3], rc);

   }

   return 0;
}

#endif

