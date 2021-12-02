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
 *   q_plsf_5.c
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
*      File             : q_plsf_5.c
*      Purpose          : Quantization of 2 sets of LSF parameters using 1st
*                         order MA prediction and split by 5 matrix
*                         quantization (split-MQ)
*
********************************************************************************
*/

 
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "q_plsf.h"
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "lsp_lsf.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/

//#include "q_plsf_5.tab"         /* Codebooks of LSF prediction residual */

#include "tab_lsf_5.h"         /* Codebooks of LSF prediction residual */


/*
********************************************************************************
*                         LOCAL PROGRAM CODE
********************************************************************************
*/
/* Quantization of a 4 dimensional subvector */

static Word16 Vq_subvec (/* o : quantization index,            Q0  */
    Word16 *lsf_r1,      /* i : 1st LSF residual vector,       Q15 */
    Word16 *lsf_r2,      /* i : 2nd LSF residual vector,       Q15 */
    const Word16 *dico,  /* i : quantization codebook,         Q15 */
    Word16 *wf1,         /* i : 1st LSF weighting factors      Q13 */
    Word16 *wf2,         /* i : 2nd LSF weighting factors      Q13 */
    Word16 dico_size     /* i : size of quantization codebook, Q0  */
)
{
   Word32 index = 0; /* initialization only needed to keep gcc silent */
   Word32 i, temp;
   Word32 dist_min, dist;
   const Word16 *p_dico;


   dist_min = MAX_32;
   p_dico = dico;

//   for (i = 0; i < dico_size; i++)
   for (i = dico_size-1; i >= 0; i--)
   {
   	temp = (lsf_r1[0] - *p_dico++);
      temp = smulbb(wf1[0], temp) >> 15;
      dist = smulbb (temp, temp);

      temp = (lsf_r1[1] - *p_dico++);
      temp = smulbb(wf1[1], temp) >> 15;
      dist = qadd(dist,smulbb( temp, temp));

      temp = (lsf_r2[0] - *p_dico++);
      temp = smulbb(wf2[0], temp) >> 15;
      dist = qadd(dist, smulbb(temp, temp));

      temp = (lsf_r2[1] - *p_dico++);
      temp = smulbb(wf2[1], temp) >> 15;
      dist = qadd(dist, smulbb(temp, temp));


      if(dist < dist_min)
      {
      	dist_min = dist;
         index = i;
      }

   }


   /* Reading the selected vector */

	index = dico_size-1 - index;


   p_dico = &dico[(index << 2)];
   lsf_r1[0] = *p_dico++;
   lsf_r1[1] = *p_dico++;
   lsf_r2[0] = *p_dico++;
   lsf_r2[1] = *p_dico++;

   return index;

}

/* Quantization of a 4 dimensional subvector with a signed codebook */

static Word16 Vq_subvec_s ( /* o : quantization index            Q0  */
    Word16 *lsf_r1,         /* i : 1st LSF residual vector       Q15 */
    Word16 *lsf_r2,         /* i : and LSF residual vector       Q15 */
    const Word16 *dico,     /* i : quantization codebook         Q15 */
    Word16 *wf1,            /* i : 1st LSF weighting factors     Q13 */
    Word16 *wf2,            /* i : 2nd LSF weighting factors     Q13 */
    Word16 dico_size)       /* i : size of quantization codebook Q0  */
{
   Word32 index = 0;  /* initialization only needed to keep gcc silent */
   Word32 sign = 0;   /* initialization only needed to keep gcc silent */
   Word32 i, temp;
   Word32 dist_min, dist;
   const Word16 *p_dico;


   dist_min = MAX_32;
   p_dico = dico;

//   for (i = 0; i < dico_size; i++)
   for (i = dico_size-1; i >= 0; i--)
   {
   	/* test positive */

      temp = (lsf_r1[0] - *p_dico++);
      temp = smulbb(wf1[0], temp) >> 15;
      dist = smulbb(temp, temp);

      temp = (lsf_r1[1] - *p_dico++);
      temp = smulbb(wf1[1], temp) >> 15;
      dist = qadd(dist,smulbb( temp, temp));

      temp = (lsf_r2[0] - *p_dico++);
      temp = smulbb(wf2[0], temp) >> 15;
      dist = qadd(dist,smulbb( temp, temp));

      temp = (lsf_r2[1] - *p_dico++);
      temp = smulbb(wf2[1], temp) >> 15;
      dist = qadd(dist,smulbb( temp, temp));

      if (dist < dist_min)
      {
      	dist_min = dist;
         index = i;
         sign = 0;
      }
      /* test negative */

      p_dico -= 4;
      temp = (lsf_r1[0] + *p_dico++);
      temp = smulbb(wf1[0], temp) >> 15;
      dist = smulbb(temp, temp);

      temp = (lsf_r1[1] + *p_dico++);
      temp = smulbb(wf1[1], temp) >> 15;
      dist = qadd(dist,smulbb( temp, temp));

      temp = (lsf_r2[0] + *p_dico++);
      temp = smulbb(wf2[0], temp) >> 15;
      dist = qadd(dist,smulbb( temp, temp));

      temp = (lsf_r2[1] + *p_dico++);
      temp = smulbb(wf2[1], temp) >> 15;
      dist = qadd(dist,smulbb( temp, temp));

      if(dist < dist_min)
      {
	      dist_min = dist;
         index = i;
         sign = 1;
      }

   }

   /* Reading the selected vector */

	index = dico_size-1 - index;

   p_dico = &dico[(index << 2)];

   if (sign == 0)
   {
	   lsf_r1[0] = *p_dico++;
      lsf_r1[1] = *p_dico++;
      lsf_r2[0] = *p_dico++;
      lsf_r2[1] = *p_dico++;
   }
   else
   {
      lsf_r1[0] = -(*p_dico++);
      lsf_r1[1] = -(*p_dico++);
      lsf_r2[0] = -(*p_dico++);
      lsf_r2[1] = -(*p_dico++);
   }

   index = (index << 1);
   index = (index + sign);

   return index;

}

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *   FUNCTION:  Q_plsf_5()
 *
 *   PURPOSE:  Quantization of 2 sets of LSF parameters using 1st order MA
 *             prediction and split by 5 matrix quantization (split-MQ)
 *
 *   DESCRIPTION:
 *
 *        p[i] = pred_factor*past_rq[i];   i=0,...,m-1
 *        r1[i]= lsf1[i] - p[i];           i=0,...,m-1
 *        r2[i]= lsf2[i] - p[i];           i=0,...,m-1
 *   where:
 *        lsf1[i]           1st mean-removed LSF vector.
 *        lsf2[i]           2nd mean-removed LSF vector.
 *        r1[i]             1st residual prediction vector.
 *        r2[i]             2nd residual prediction vector.
 *        past_r2q[i]       Past quantized residual (2nd vector).
 *
 *   The residual vectors r1[i] and r2[i] are jointly quantized using
 *   split-MQ with 5 codebooks. Each 4th dimension submatrix contains 2
 *   elements from each residual vector. The 5 submatrices are as follows:
 *     {r1[0], r1[1], r2[0], r2[1]};  {r1[2], r1[3], r2[2], r2[3]};
 *     {r1[4], r1[5], r2[4], r2[5]};  {r1[6], r1[7], r2[6], r2[7]};
 *                    {r1[8], r1[9], r2[8], r2[9]};
 *
 *************************************************************************/

void Q_plsf_5 (
   AMR_Encode_FrameState *Enc,
   Word16 *past_rq,
   Word16 *lsp1,      /* i : 1st LSP vector,                     Q15 */
   Word16 *lsp2,      /* i : 2nd LSP vector,                     Q15 */
   Word16 *lsp1_q,    /* o : quantized 1st LSP vector,           Q15 */
   Word16 *lsp2_q,    /* o : quantized 2nd LSP vector,           Q15 */
   Word16 *indice     /* o : quantization indices of 5 matrices, Q0  */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i;
/*
   Word16 wf1[M];
	Word16 wf2[M];
	Word16 lsf_p[M];
	Word16 lsf_r1[M];
	Word16 lsf_r2[M];
   Word16 lsf1_q[M];
	Word16 lsf2_q[M];
	Word16 lsf1[M];
	Word16 lsf2[M];
*/
   Word16 *wf1    = Enc_share->shabuf.stE.u2E.s2E.wf1;
	Word16 *wf2    = Enc_share->shabuf.stE.u2E.s2E.wf2;
	Word16 *lsf_p  = Enc_share->shabuf.stE.u2E.s2E.lsf_p;
	Word16 *lsf_r1 = Enc_share->shabuf.stE.u2E.s2E.lsf_r1;
	Word16 *lsf_r2 = Enc_share->shabuf.stE.u2E.s2E.lsf_r2;
   Word16 *lsf1_q = Enc_share->shabuf.stE.u2E.s2E.lsf1_q;
	Word16 *lsf2_q = Enc_share->shabuf.stE.u2E.s2E.lsf2_q;
	Word16 *lsf1   = Enc_share->shabuf.stE.u2E.s2E.lsf1;
	Word16 *lsf2   = Enc_share->shabuf.stE.u2E.s2E.lsf2;


   /* convert LSFs to normalize frequency domain 0..16384  */

   Lsp_lsf (lsp1, lsf1);
   Lsp_lsf (lsp2, lsf2);

   /* Compute LSF weighting factors (Q13) */

   Lsf_wt (lsf1, wf1);
   Lsf_wt (lsf2, wf2);

   /* Compute predicted LSF and prediction error */


   for (i = M-1; i >= 0; i--)
   {
   	lsf_p[i] =  (mean_lsf[i] + (smulbb(past_rq[i], LSP_PRED_FAC_MR122) >> 15) );
      lsf_r1[i] = (lsf1[i] - lsf_p[i]);
      lsf_r2[i] = (lsf2[i] - lsf_p[i]);
   }

/*
#define DICO1_SIZE  128
#define DICO2_SIZE  256
#define DICO3_SIZE  256
#define DICO4_SIZE  256
#define DICO5_SIZE  64
*/
    /*---- Split-MQ of prediction error ----*/

    indice[0] = Vq_subvec (&lsf_r1[0], &lsf_r2[0], dico1_lsf,
                           &wf1[0], &wf2[0], DICO1_SIZE);

    indice[1] = Vq_subvec (&lsf_r1[2], &lsf_r2[2], dico2_lsf,
                           &wf1[2], &wf2[2], DICO2_SIZE);

    indice[2] = Vq_subvec_s (&lsf_r1[4], &lsf_r2[4], dico3_lsf,
                             &wf1[4], &wf2[4], DICO3_SIZE);

    indice[3] = Vq_subvec (&lsf_r1[6], &lsf_r2[6], dico4_lsf,
                           &wf1[6], &wf2[6], DICO4_SIZE);

    indice[4] = Vq_subvec (&lsf_r1[8], &lsf_r2[8], dico5_lsf,
                           &wf1[8], &wf2[8], DICO5_SIZE);

    /* Compute quantized LSFs and update the past quantized residual */
    for (i = M-1; i >= 0; i--)
    {
        lsf1_q[i] = (lsf_r1[i] + lsf_p[i]);
        lsf2_q[i] = (lsf_r2[i] + lsf_p[i]);
        past_rq[i] = lsf_r2[i];
    }

    /* verification that LSFs has minimum distance of LSF_GAP */
    Reorder_lsf (lsf1_q);
    Reorder_lsf (lsf2_q);

    /*  convert LSFs to the cosine domain */
    Lsf_lsp (lsf1_q, lsp1_q);
    Lsf_lsp (lsf2_q, lsp2_q);

}

#endif

