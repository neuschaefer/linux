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
 *   d_plsf_5.c
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
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : d_plsf_5.c
*
*****************************************************************************
*/


/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "d_plsf.h"

/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "lsp_lsf.h"
#include "copy.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/
//#include "q_plsf_5.tab"         /* Codebooks of LSF prediction residual */

#include "tab_lsf_5.h"         /* Codebooks of LSF prediction residual */

/* ALPHA    ->  0.95       */
/* ONE_ALPHA-> (1.0-ALPHA) */
#define ALPHA     31128
#define ONE_ALPHA 1639

/*
*--------------------------------------------------*
* Constants (defined in cnst.h)                    *
*--------------------------------------------------*
*  M                    : LPC order
*--------------------------------------------------*
*/

/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/
/*
**************************************************************************
*
*  Function    : D_plsf_5
*  Purpose     : Decodes the 2 sets of LSP parameters in a frame
*                using the received quantization indices.
*
**************************************************************************
*/
int D_plsf_5 (
    AMR_Decode_FrameState *Dec,
    D_plsfState *st,    /* i/o: State variables                            */
    Word16 bfi,         /* i  : bad frame indicator (set to 1 if a bad
                                frame is received)                         */
    Word16 *indice,     /* i  : quantization indices of 5 submatrices, Q0  */
    Word16 *lsp1_q,     /* o  : quantized 1st LSP vector (M),          Q15 */
    Word16 *lsp2_q      /* o  : quantized 2nd LSP vector (M),          Q15 */
)
{
	AMR_Shared_Buf *Dec_share = Dec->share;
   Word32 i,sign;
/*
   Word16 lsf1_r[M];
	Word16 lsf2_r[M];
   Word16 lsf1_q[M];
	Word16 lsf2_q[M];
*/
   Word16 *lsf1_r = Dec_share->shabuf.stD.u2D.s0D.lsf1_r;
	Word16 *lsf2_r = Dec_share->shabuf.stD.u2D.s0D.lsf2_r;
   Word16 *lsf1_q = Dec_share->shabuf.stD.u2D.s0D.lsf1_q;
	Word16 *lsf2_q = Dec_share->shabuf.stD.u2D.s0D.lsf2_q;

   Word16 temp;
   const Word16 *p_dico;


   if (bfi != 0)                               /* if bad frame */
   {
      /* use the past LSFs slightly shifted towards their mean */
      for (i = 0; i < M; i++)
      {
         /* lsfi_q[i] = ALPHA*st->past_lsf_q[i] + ONE_ALPHA*mean_lsf[i]; */

         lsf1_q[i] = ( (smulbb(st->past_lsf_q[i], ALPHA)>>15) +
                       (smulbb(mean_lsf[i], ONE_ALPHA)>>15)  );

         lsf2_q[i] = lsf1_q[i];
      }

      /* estimate past quantized residual to be used in next frame */
      for (i = 0; i < M; i++)
      {
         /* temp  = mean_lsf[i] +  st->past_r_q[i] * LSP_PRED_FAC_MR122; */

         temp = (mean_lsf[i] + (smulbb(st->past_r_q[i],LSP_PRED_FAC_MR122) >> 15) );

         st->past_r_q[i] = (lsf2_q[i] - temp);

      }
   }
   else
   /* if good LSFs received */
   {
      Word16    i2idx;
      /* decode prediction residuals from 5 received indices */
      i2idx = sature(indice[0] << 2);
      if (i2idx >= DICO1_SIZE * 4 || i2idx < 0)
        i2idx = 0;

      p_dico = &dico1_lsf[i2idx];
      lsf1_r[0] = *p_dico++;
      lsf1_r[1] = *p_dico++;
      lsf2_r[0] = *p_dico++;
      lsf2_r[1] = *p_dico++;

      i2idx = sature(indice[1] << 2);
      if (i2idx >= DICO2_SIZE * 4 || i2idx < 0)
        i2idx = 0;

      p_dico = &dico2_lsf[i2idx];
      lsf1_r[2] = *p_dico++;
      lsf1_r[3] = *p_dico++;
      lsf2_r[2] = *p_dico++;
      lsf2_r[3] = *p_dico++;

      sign = indice[2] & 1;
      i = (indice[2] >> 1);

      i2idx = sature(i << 2);
      if (i2idx >= DICO3_SIZE * 4 || i2idx < 0)
        i2idx = 0;

      p_dico = &dico3_lsf[i2idx];

      if (sign == 0)
      {
         lsf1_r[4] = *p_dico++;
         lsf1_r[5] = *p_dico++;
         lsf2_r[4] = *p_dico++;
         lsf2_r[5] = *p_dico++;
      }
      else
      {
         lsf1_r[4] = negate(*p_dico++);
         lsf1_r[5] = negate(*p_dico++);
         lsf2_r[4] = negate(*p_dico++);
         lsf2_r[5] = negate(*p_dico++);
      }

      i2idx = sature(indice[3] << 2);
      if (i2idx >= DICO4_SIZE * 4 || i2idx < 0)
        i2idx = 0;

      p_dico = &dico4_lsf[i2idx];
      lsf1_r[6] = *p_dico++;
      lsf1_r[7] = *p_dico++;
      lsf2_r[6] = *p_dico++;
      lsf2_r[7] = *p_dico++;

      i2idx = sature(indice[4] << 2);
      if (i2idx >= DICO5_SIZE * 4 || i2idx < 0)
        i2idx = 0;

      p_dico = &dico5_lsf[i2idx];
      lsf1_r[8] = *p_dico++;
      lsf1_r[9] = *p_dico++;
      lsf2_r[8] = *p_dico++;
      lsf2_r[9] = *p_dico++;

      /* Compute quantized LSFs and update the past quantized residual */
      for (i = 0; i < M; i++)
      {
         temp = (mean_lsf[i] + (smulbb(st->past_r_q[i],LSP_PRED_FAC_MR122)>>15) );
         st->past_r_q[i] = lsf2_r[i];

         lsf1_q[i] = (lsf1_r[i] + temp);
         lsf2_q[i] = (lsf2_r[i] + temp);
      }
   }

   /* verification that LSFs have minimum distance of LSF_GAP Hz */

   Reorder_lsf (lsf1_q);
   Reorder_lsf (lsf2_q);

   Copy (lsf2_q, st->past_lsf_q, M);

   /*  convert LSFs to the cosine domain */

   Lsf_lsp (lsf1_q, lsp1_q);
   Lsf_lsp (lsf2_q, lsp2_q);

   return 0;

}

#endif

