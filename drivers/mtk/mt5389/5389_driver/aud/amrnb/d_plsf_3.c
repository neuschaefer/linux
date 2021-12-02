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
 *   d_plsf_3.c
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
*      File             : d_plsf_3.c
*      Purpose          : Decodes the LSP parameters using the received
*                         quantization indices. 1st order MA prediction and
*                         split by 3 vector quantization (split-VQ)
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "d_plsf.h"



/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "lsp_lsf.h"
#include "copy.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
//#include "q_plsf_3.tab"    /* Codebooks of LSF prediction residual */

#include "tab_lsf_3.h"    /* Codebooks of LSF prediction residual */


/* ALPHA    ->  0.9                                            */
/* ONE_ALPHA-> (1.0-ALPHA)                                     */

#define ALPHA     29491
#define ONE_ALPHA 3277

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:   D_plsf_3()
 *
 *  PURPOSE: Decodes the LSP parameters using the received quantization
 *           indices.1st order MA prediction and split by 3 vector
 *           quantization (split-VQ)
 *
 *************************************************************************/

void D_plsf_3(
    Word16 *lsf1_q_t,
    D_plsfState *st,   /* i/o: State struct                               */
    enum Mode mode,    /* i  : coder mode                                 */
    Word16 bfi,        /* i  : bad frame indicator (set to 1 if a         */
                       /*      bad frame is received)                     */
    Word16 * indice,   /* i  : quantization indices of 3 submatrices, Q0  */
    Word16 * lsp1_q    /* o  : quantized 1st LSP vector,              Q15 */
)
{
   Word32 i, index;
//   Word16 lsf1_q[M];

   Word16 *lsf1_q = lsf1_q_t;

   Word16 temp;
   const Word16 *p_cb1, *p_cb2, *p_cb3, *p_dico;

   if (bfi != 0)   /* if bad frame */
   {
      /* use the past LSFs slightly shifted towards their mean */

      for (i = 0; i < M; i++)
      {
         /* lsfi_q[i] = ALPHA*past_lsf_q[i] + ONE_ALPHA*mean_lsf[i]; */

         lsf1_q[i] = ( (smulbb(st->past_lsf_q[i], ALPHA)>>15) +
            (smulbb(mean_lsf3[i], ONE_ALPHA)>>15) );

      }

      /* estimate past quantized residual to be used in next frame */

      if(mode != MRDTX)
      {
         for (i = 0; i < M; i++)
         {
            /* temp  = mean_lsf[i] +  past_r2_q[i] * PRED_FAC; */
            temp = (mean_lsf3[i] + (smulbb(st->past_r_q[i], pred_fac[i])>>15) );
            st->past_r_q[i] = (lsf1_q[i] - temp);
         }
      }
      else
      {
         for (i = 0; i < M; i++)
         {
            /* temp  = mean_lsf[i] +  past_r2_q[i]; */
            temp = (mean_lsf3[i] + st->past_r_q[i]);
            st->past_r_q[i] = (lsf1_q[i] - temp);
         }
      }
   }
   else  /* if good LSFs received */
   {
      if ( (mode == MR475) ||  (mode == MR515) )
      {   /* MR475, MR515 */
         p_cb1 = dico1_lsf3;
         p_cb2 = dico2_lsf3;
         p_cb3 = mr515_3_lsf;
      }
      else if ( (mode == MR795) )
      {   /* MR795 */

         p_cb1 = mr795_1_lsf;
         p_cb2 = dico2_lsf3;
         p_cb3 = dico3_lsf3;
      }
      else
      {   /* MR59, MR67, MR74, MR102, MRDTX */
         p_cb1 = dico1_lsf3;
         p_cb2 = dico2_lsf3;
         p_cb3 = dico3_lsf3;
      }

      /* decode prediction residuals from 3 received indices */

      index = *indice++;
      p_dico = &p_cb1[(index + (index + index))];
      lsf1_q[0] = *p_dico++;
      lsf1_q[1] = *p_dico++;
      lsf1_q[2] = *p_dico++;

      index = *indice++;

      if (( (mode == MR475) ) || ( (mode == MR515) ))
      {   /* MR475, MR515 only using every second entry */
         index = sature(index << 1);
      }

      p_dico = &p_cb2[(index + (index + index))];
      lsf1_q[3] = *p_dico++;
      lsf1_q[4] = *p_dico++;
      lsf1_q[5] = *p_dico++;

      index = *indice++;
      p_dico = &p_cb3[sature(index << 2)];
      lsf1_q[6] = *p_dico++;
      lsf1_q[7] = *p_dico++;
      lsf1_q[8] = *p_dico++;
      lsf1_q[9] = *p_dico++;

      /* Compute quantized LSFs and update the past quantized residual */

      if ((mode != MRDTX) )
      {
         for (i = 0; i < M; i++)
         {
            temp = (mean_lsf3[i] + (smulbb(st->past_r_q[i], pred_fac[i])>>15) );
            st->past_r_q[i] = lsf1_q[i];

            lsf1_q[i] = (lsf1_q[i] + temp);

         }
      }
      else
      {
         for (i = 0; i < M; i++)
         {
            temp = (mean_lsf3[i] + st->past_r_q[i]);
            st->past_r_q[i] = lsf1_q[i];

            lsf1_q[i] = (lsf1_q[i] + temp);
         }
      }
   }

   /* verification that LSFs has minimum distance of LSF_GAP Hz */

   Reorder_lsf(lsf1_q);

   Copy (lsf1_q, st->past_lsf_q, M);

   /*  convert LSFs to the cosine domain */

   Lsf_lsp(lsf1_q, lsp1_q);

   return;
}


void Init_D_plsf_3(D_plsfState *st,  /* i/o: State struct                */
		   Word16 index      /* i  : past_rq_init[] index [0, 7] */)
{
   Copy(&past_rq_init[index * M], st->past_r_q, M);
}

#endif

