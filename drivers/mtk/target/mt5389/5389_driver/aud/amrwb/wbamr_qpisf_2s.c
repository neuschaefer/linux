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
 *   WBAMR_QPIST_2S.C
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
 * $Revision: #1 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 * 09 21 2010 eddy.wu
 * [WCPSP00000503] Checkin SWIP Modification
 * .
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_acelp.h"
#include "wbamr_private.h"
#include "wbamr_sections.h"


/*-------------------------------------------------------------------*
 * Function   WBAMR_Qpisf_2s_46b()                                   *
 *            ~~~~~~~~~                                              *
 * Quantization of isf parameters with prediction. (46 bits)         *
 *                                                                   *
 * The isf vector is quantized using two-stage VQ with split-by-2 in *
 *  1st stage and split-by-5 in the second stage.                    *
 *-------------------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Qpisf_2s_46b(
  fract16 * indice,      /* input:  quantization indices */
  fract16 * isf_q,       /* output: quantized ISF in frequency domain */
  fract16 * past_isfq,   /* (io)Q15 : past ISF quantizer */
  fract16 nb_surv,       /* (i)     : number of survivor (1, 2, 3 or 4)*/
  void* tmp_scr_ptr,
  fract16 * enc_obj_temp_ptr
)
{

  int_native i, k;
  fract32 temp, min_err, distance;
  fract16 *isf1;

  wbamr_enc_qpisf46b *wbamr_enc_qpisf46b_scr_ptr =
                                   (wbamr_enc_qpisf46b *)tmp_scr_ptr;

  isf1=isf_q;

  for (i = 0; i < ORDER; i++)
  {
    /* isf[i] = isf1[i] - mean_isf[i] - MU*past_isfq[i] */
    wbamr_enc_qpisf46b_scr_ptr->isf[i] = Sub_fr1x16(isf1[i], mean_isf[i]);
    wbamr_enc_qpisf46b_scr_ptr->isf[i] = Sub_fr1x16(
            wbamr_enc_qpisf46b_scr_ptr->isf[i], Mult_fr1x16(MU, past_isfq[i]));
  }

  WBAMR_VQ_stage1( &wbamr_enc_qpisf46b_scr_ptr->isf[0],
                     dico1_isf,
                     9,
                     SIZE_BK1,
                     wbamr_enc_qpisf46b_scr_ptr->surv1,
                     nb_surv,
                     wbamr_enc_qpisf46b_scr_ptr->dist_min,
                     enc_obj_temp_ptr);

  distance = MAX_32;

  for (k = 0; k < nb_surv; k++)
  {
    for (i = 0; i < 9; i++)
    {
      wbamr_enc_qpisf46b_scr_ptr->isf_stage2[i] = Sub_fr1x16(
                         wbamr_enc_qpisf46b_scr_ptr->isf[i],
                          dico1_isf[i + wbamr_enc_qpisf46b_scr_ptr->surv1[k] * 9]);
    }

    wbamr_enc_qpisf46b_scr_ptr->tmp_ind[0] = WBAMR_Sub_VQ(
                                   &wbamr_enc_qpisf46b_scr_ptr->isf_stage2[0],
                                   dico21_isf, 3, SIZE_BK21, &min_err);
    temp = min_err;

    wbamr_enc_qpisf46b_scr_ptr->tmp_ind[1] = WBAMR_Sub_VQ(
                                   &wbamr_enc_qpisf46b_scr_ptr->isf_stage2[3],
                                   dico22_isf, 3, SIZE_BK22, &min_err);

    temp = Add_fr1x32(temp, min_err);

    wbamr_enc_qpisf46b_scr_ptr->tmp_ind[2] = WBAMR_Sub_VQ(
    				   &wbamr_enc_qpisf46b_scr_ptr->isf_stage2[6],
    				   dico23_isf, 3, SIZE_BK23, &min_err);

    temp = Add_fr1x32(temp, min_err);


    if (Sub_fr1x32(temp, distance) < (fract32) 0)
    {
      distance = temp;
      indice[0] = wbamr_enc_qpisf46b_scr_ptr->surv1[k];

      for (i = 0; i < 3; i++)
      {
        indice[i + 2] = wbamr_enc_qpisf46b_scr_ptr->tmp_ind[i];
      }
    }
  }


  WBAMR_VQ_stage1( &wbamr_enc_qpisf46b_scr_ptr->isf[9],
                     dico2_isf,
                     7,
                     SIZE_BK2,
                     wbamr_enc_qpisf46b_scr_ptr->surv1,
                     nb_surv,
                     wbamr_enc_qpisf46b_scr_ptr->dist_min,
                     enc_obj_temp_ptr);

  distance = MAX_32;

  for (k = 0; k < nb_surv; k++)
  {
    for (i = 0; i < 7; i++)
    {
      wbamr_enc_qpisf46b_scr_ptr->isf_stage2[i] = Sub_fr1x16(
                     wbamr_enc_qpisf46b_scr_ptr->isf[9 + i],
                     dico2_isf[i + wbamr_enc_qpisf46b_scr_ptr->surv1[k] * 7]);
    }

    wbamr_enc_qpisf46b_scr_ptr->tmp_ind[0] = WBAMR_Sub_VQ(
    			&wbamr_enc_qpisf46b_scr_ptr->isf_stage2[0],
    			dico24_isf, 3, SIZE_BK24, &min_err);

    temp = min_err;

    wbamr_enc_qpisf46b_scr_ptr->tmp_ind[1] = WBAMR_Sub_VQ(
    			&wbamr_enc_qpisf46b_scr_ptr->isf_stage2[3],
    			dico25_isf, 4, SIZE_BK25, &min_err);

    temp = Add_fr1x32(temp, min_err);


    if (Sub_fr1x32(temp, distance) < (fract32) 0)
    {
      distance = temp;
      indice[1] = wbamr_enc_qpisf46b_scr_ptr->surv1[k];

      for (i = 0; i < 2; i++)
      {
        indice[i + 5] = wbamr_enc_qpisf46b_scr_ptr->tmp_ind[i];
      }
    }
  }

  WBAMR_Dpisf_2s_46b( indice,
                        isf_q,
                        past_isfq,
                        isf_q,
                        isf_q,
                        0,
                        0,
                        wbamr_enc_qpisf46b_scr_ptr->ref_isf);

  return;

}



/*-------------------------------------------------------------------*
 * Function   WBAMR_Qpisf_2s_36b()                                   *
 *            ~~~~~~~~~                                              *
 * Quantization of isf parameters with prediction. (36 bits)         *
 *                                                                   *
 * The isf vector is quantized using two-stage VQ with split-by-2 in *
 *  1st stage and split-by-3 in the second stage.                    *
 *-------------------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Qpisf_2s_36b(
  fract16 * indice,       /* input:  quantization indices  */
  fract16 * isf_q,        /* output: quantized ISF in frequency domain */
  fract16 * past_isfq,    /* (io)Q15 : past ISF quantizer  */
  fract16 nb_surv,        /* (i)     : number of survivor  */
  void* tmp_scr_ptr,
  fract16 * enc_obj_temp_ptr
)
{

  int_native i, k;
  fract32 temp, min_err, distance;
  fract16 *isf1;

  wbamr_enc_qpisf46b *wbamr_enc_qpisf46b_scr_ptr =
  				(wbamr_enc_qpisf46b *)tmp_scr_ptr;

  isf1=isf_q;

  for (i = 0; i < ORDER; i++)
  {
    /* isf[i] = isf1[i] - mean_isf[i] - MU*past_isfq[i] */
    wbamr_enc_qpisf46b_scr_ptr->isf[i] = Sub_fr1x16(isf1[i], mean_isf[i]);
    wbamr_enc_qpisf46b_scr_ptr->isf[i] = Sub_fr1x16(wbamr_enc_qpisf46b_scr_ptr->isf[i], Mult_fr1x16(MU, past_isfq[i]));
  }

  WBAMR_VQ_stage1( &wbamr_enc_qpisf46b_scr_ptr->isf[0],
                     dico1_isf,
                     9,
                     SIZE_BK1,
                     wbamr_enc_qpisf46b_scr_ptr->surv1,
                     nb_surv,
                     wbamr_enc_qpisf46b_scr_ptr->dist_min,
                     enc_obj_temp_ptr);

  distance = MAX_32;

  for (k = 0; k < nb_surv; k++)
  {
    for (i = 0; i < 9; i++)
    {
      wbamr_enc_qpisf46b_scr_ptr->isf_stage2[i] = Sub_fr1x16(
      				wbamr_enc_qpisf46b_scr_ptr->isf[i],
      		     dico1_isf[i + wbamr_enc_qpisf46b_scr_ptr->surv1[k] * 9]);
    }

    wbamr_enc_qpisf46b_scr_ptr->tmp_ind[0] = WBAMR_Sub_VQ(
             &wbamr_enc_qpisf46b_scr_ptr->isf_stage2[0],
             dico21_isf_36b, 5, SIZE_BK21_36b, &min_err);

    temp = min_err;
    wbamr_enc_qpisf46b_scr_ptr->tmp_ind[1] = WBAMR_Sub_VQ(
             &wbamr_enc_qpisf46b_scr_ptr->isf_stage2[5],
             dico22_isf_36b, 4, SIZE_BK22_36b, &min_err);

    temp = Add_fr1x32(temp, min_err);

    if (Sub_fr1x32(temp, distance) < (fract32) 0)
    {
      distance = temp;
      indice[0] = wbamr_enc_qpisf46b_scr_ptr->surv1[k];

      for (i = 0; i < 2; i++)
      {
        indice[i + 2] = wbamr_enc_qpisf46b_scr_ptr->tmp_ind[i];
      }

    }

  }

  WBAMR_VQ_stage1( &wbamr_enc_qpisf46b_scr_ptr->isf[9],
                     dico2_isf,
                     7,
                     SIZE_BK2,
                     wbamr_enc_qpisf46b_scr_ptr->surv1,
                     nb_surv,
                     wbamr_enc_qpisf46b_scr_ptr->dist_min,
                     enc_obj_temp_ptr);

  distance = MAX_32;

  for (k = 0; k < nb_surv; k++)
  {
    for (i = 0; i < 7; i++)
    {
      wbamr_enc_qpisf46b_scr_ptr->isf_stage2[i] = Sub_fr1x16(
                      wbamr_enc_qpisf46b_scr_ptr->isf[9 + i],
                      dico2_isf[i + wbamr_enc_qpisf46b_scr_ptr->surv1[k] * 7]);
    }

    wbamr_enc_qpisf46b_scr_ptr->tmp_ind[0] = WBAMR_Sub_VQ(
                     &wbamr_enc_qpisf46b_scr_ptr->isf_stage2[0],
                     dico23_isf_36b, 7, SIZE_BK23_36b, &min_err);

    temp = min_err;

    if (Sub_fr1x32(temp, distance) < (fract32) 0)
    {
      distance = temp;
      indice[1] = wbamr_enc_qpisf46b_scr_ptr->surv1[k];
      indice[4] = wbamr_enc_qpisf46b_scr_ptr->tmp_ind[0];
    }
  }

  WBAMR_Dpisf_2s_36b( indice,
                        isf_q,
                        past_isfq,
                        isf_q, isf_q,
                        0,
                        0,
                        wbamr_enc_qpisf46b_scr_ptr->ref_isf);

  return;

}



/*-------------------------------------------------------------------*
 * routine:   Disf_2s_46b()                                          *
 *            ~~~~~~~~~                                              *
 * Decoding of ISF parameters                                        *
 *-------------------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Dpisf_2s_46b(
  fract16 * indice,     /* input:  quantization indices */
  fract16 * isf_q,      /* output: quantized ISF in frequency domain */
  fract16 * past_isfq,  /* i/0   : past ISF quantizer */
  fract16 * isfold,     /* input : past quantized ISF */
  fract16 * isf_buf,    /* input : isf buffer  */
  fract16 bfi,          /* input : Bad frame indicator */
  fract16 enc_dec,
  fract16 *ref_isf
)
{

  int_native i, j;
  fract32 L_tmp;
  fract16 tmp;

  if (bfi == 0)         /* Good frame */
  {
    for (i = 0; i < 9; i++)
    {
      isf_q[i] = dico1_isf[indice[0] * 9 + i];
    }

    for (i = 0; i < 7; i++)
    {
      isf_q[i + 9] = dico2_isf[indice[1] * 7 + i];
    }

    for (i = 0; i < 3; i++)
    {
      isf_q[i] = Add_fr1x16(isf_q[i], dico21_isf[indice[2] * 3 + i]);
    }

    for (i = 0; i < 3; i++)
    {
      isf_q[i + 3] = Add_fr1x16(isf_q[i + 3], dico22_isf[indice[3] * 3 + i]);
    }

    for (i = 0; i < 3; i++)
    {
      isf_q[i + 6] = Add_fr1x16(isf_q[i + 6], dico23_isf[indice[4] * 3 + i]);
    }

    for (i = 0; i < 3; i++)
    {
      isf_q[i + 9] = Add_fr1x16(isf_q[i + 9], dico24_isf[indice[5] * 3 + i]);
    }

    for (i = 0; i < 4; i++)
    {
      isf_q[i + 12] = Add_fr1x16(isf_q[i + 12], dico25_isf[indice[6] * 4 + i]);
    }

    for (i = 0; i < ORDER; i++)
    {
      tmp = isf_q[i];
      isf_q[i] = Add_fr1x16(tmp, mean_isf[i]);
      isf_q[i] = Add_fr1x16(isf_q[i], Mult_fr1x16(MU, past_isfq[i]));
      past_isfq[i] = tmp;
    }


    if (enc_dec)
    {
       for (i = 0; i < M11; i++)
       {
         for (j = (L_MEANBUF - 1); j > 0; j--)
         {
           isf_buf[j * M11 + i] = isf_buf[(j - 1) * M11 + i];
         }

         isf_buf[i] = isf_q[i];
       }
     }
  }
  else
  {                              /* bad frame */
    for (i = 0; i < M11; i++)
    {
      L_tmp = Mult_fr1x32(mean_isf[i], 8192);

      for (j = 0; j < L_MEANBUF; j++)
      {
         L_tmp = Mac_fr16_32(L_tmp, isf_buf[j * M11 + i], 8192);
      }

      ref_isf[i] = Round_fr1x32(L_tmp);
    }

    /* use the past ISFs slightly shifted towards their mean */
    for (i = 0; i < ORDER; i++)
    {
      isf_q[i] = Add_fr1x16(Mult_fr1x16(ALPHA, isfold[i]),
                       Mult_fr1x16(ONE_ALPHA, ref_isf[i]));
    }

    /* estimate past quantized WBAMR_Residual to be used in next frame */
    for (i = 0; i < ORDER; i++)
    {
      tmp = Add_fr1x16(ref_isf[i],
                 Mult_fr1x16(past_isfq[i], MU));

      past_isfq[i] = Sub_fr1x16(isf_q[i], tmp);
      past_isfq[i] = Shr_fr1x16(past_isfq[i], 1);   /* past_isfq[i] *= 0.5 */
    }
  }

  WBAMR_Reorder_isf(isf_q, ISF_GAP, ORDER);

  return;

}


/*-------------------------------------------------------------------*
 * routine:   Disf_2s_36b()                                          *
 *            ~~~~~~~~~                                              *
 * Decoding of ISF parameters                                        *
 *-------------------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Dpisf_2s_36b(
  fract16 * indice,     /* input:  quantization indices */
  fract16 * isf_q,      /* output: quantized ISF in frequency domain */
  fract16 * past_isfq,  /* i/0   : past ISF quantizer */
  fract16 * isfold,     /* input : past quantized ISF */
  fract16 * isf_buf,    /* input : isf buffer */
  fract16 bfi,          /* input : Bad frame indicator */
  fract16 enc_dec,
  fract16 *ref_isf
)
{

  int_native i, j;
  fract32 L_tmp;
  fract16 tmp;

  if (bfi == 0)                          /* Good frame */
  {
    for (i = 0; i < 9; i++)
    {
      isf_q[i] = dico1_isf[indice[0] * 9 + i];
    }

    for (i = 0; i < 7; i++)
    {
      isf_q[i + 9] = dico2_isf[indice[1] * 7 + i];
    }

    for (i = 0; i < 5; i++)
    {
      isf_q[i] = Add_fr1x16(isf_q[i], dico21_isf_36b[indice[2] * 5 + i]);
    }

    for (i = 0; i < 4; i++)
    {
      isf_q[i + 5] = Add_fr1x16(isf_q[i + 5],
                           dico22_isf_36b[indice[3]* 4 + i]);
    }

    for (i = 0; i < 7; i++)
    {
      isf_q[i + 9] = Add_fr1x16(isf_q[i + 9],
                      dico23_isf_36b[indice[4] * 7 + i]);
    }

    for (i = 0; i < ORDER; i++)
    {
      tmp = isf_q[i];
      isf_q[i] = Add_fr1x16(tmp, mean_isf[i]);
      isf_q[i] = Add_fr1x16(isf_q[i], Mult_fr1x16(MU, past_isfq[i]));
      past_isfq[i] = tmp;
    }


    if (enc_dec)
    {
      for (i = 0; i < M11; i++)
      {
        for (j = (L_MEANBUF - 1); j > 0; j--)
        {
          isf_buf[j * M11 + i] = isf_buf[(j - 1) * M11 + i];
        }

        isf_buf[i] = isf_q[i];
      }
    }
  }
  else
  {                                      /* bad frame */
    for (i = 0; i < M11; i++)
    {
      L_tmp = Mult_fr1x32(mean_isf[i], 8192);
      for (j = 0; j < L_MEANBUF; j++)
      {
        L_tmp = Mac_fr16_32(L_tmp, isf_buf[j * M11 + i], 8192);
      }

      ref_isf[i] = Round_fr1x32(L_tmp);
    }

    /* use the past ISFs slightly shifted towards their mean */
    for (i = 0; i < ORDER; i++)
    {
      isf_q[i] = Add_fr1x16(Mult_fr1x16(ALPHA, isfold[i]), Mult_fr1x16(ONE_ALPHA, ref_isf[i]));
    }

    /* estimate past quantized WBAMR_Residual to be used in next frame */
    for (i = 0; i < ORDER; i++)
    {
      tmp = Add_fr1x16(ref_isf[i], Mult_fr1x16(past_isfq[i], MU));
      past_isfq[i] = Sub_fr1x16(isf_q[i], tmp);
      past_isfq[i] = Shr_fr1x16(past_isfq[i], 1);
    }
  }

  WBAMR_Reorder_isf(isf_q, ISF_GAP, ORDER);

  return;

}



/*--------------------------------------------------------------------------*
 * procedure  WBAMR_Reorder_isf()                                           *
 *            ~~~~~~~~~~~~~                                                 *
 * To make sure that the  isfs are properly order and to keep a certain     *
 * minimum distance between consecutive isfs.                               *
 *--------------------------------------------------------------------------*
 *    Argument         description                     in/out               *
 *    ~~~~~~~~         ~~~~~~~~~~~                     ~~~~~~               *
 *     isf[]           vector of isfs                    i/o                *
 *     min_dist        minimum required distance         i                  *
 *     n               LPC order                         i                  *
 *--------------------------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Reorder_isf(
  fract16 * isf,    /* (i/o) Q15: ISF in the frequency domain (0..0.5) */
  fract16 min_dist, /* (i) Q15  : minimum distance to keep */
  fract16 n         /* (i)      : number of ISF */
)
{

  int_native i;
  fract16 isf_min;

  isf_min = min_dist;

  for (i = 0; i < n - 1; i++)
  {
    if (Sub_fr1x16(isf[i], isf_min) < 0)
    {
      isf[i] = isf_min;
    }

    isf_min = Add_fr1x16(isf[i], min_dist);
  }

  return;

}




WBAMR_CODE_SECTION fract16 WBAMR_Sub_VQ(
			/* output: return quantization index */
   fract16 * x,          /* input : ISF WBAMR_Residual vector */
   const fract16 * dico,       /* input : quantization codebook */
   fract16 dim,          /* input : dimention of vector */
   fract16 dico_size,    /* input : size of quantization codebook */
   fract32 * distance    /* output: error of quantization */
)
{

   int_native i, j;
   fract32 dist_min, dist;
#if (AWB_OPT_VER == 2)
  fract16 *pX;
  const fract16 *p_dico16;
  fract32 L_temp, L_dico;
  fract32 index;
  fract32 x0, x1, x2, x3, x4, x5, x6;
  fract32 *p_dico;

  dist_min = MAX_32;
  p_dico = (fract32 *)dico;
  index = 0;

  switch(dim) {
  case 2:
     i = dico_size;
     x0 = (fract32)(*x++) << 16;  x0 = x0 + ((unsigned int)x0 >> 16);
     x1 = (fract32)(*x++) << 16;  x1 = x1 + ((unsigned int)x1 >> 16);
     do
     {
        L_dico = *p_dico++;
        L_temp = x0 - (L_dico << 16);    dist = AWB_smultt(L_temp, L_temp);
        L_temp = x1 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);

        if (dist < dist_min) {
          dist_min = dist;
          index = i;
        }
     } while(--i != 0);
     break;
  case 3:
     i = dico_size;
     x0 = (fract32)(*x++) << 16;  x0 = x0 + ((unsigned int)x0 >> 16);
     x1 = (fract32)(*x++) << 16;  x1 = x1 + ((unsigned int)x1 >> 16);
     x2 = (fract32)(*x++) << 16;  x2 = x2 + ((unsigned int)x2 >> 16);
     do
     {
        L_dico = *p_dico++;
        L_temp = x0 - (L_dico << 16);   dist = AWB_smultt(L_temp, L_temp);
        L_temp = x1 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        L_dico = *p_dico++;
        L_temp = x2 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);

        if (dist < dist_min) {
          dist_min = dist;
          index = i;
        }
        L_temp = x0 - (L_dico >> 16);   dist = AWB_smulbb(L_temp, L_temp);
        L_dico = *p_dico++;
        L_temp = x1 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);
        L_temp = x2 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        if (dist < dist_min) {
          dist_min = dist;
          index = i - 1;
        }
     } while((i -= 2) > 0);
     break;
  case 4:
     i = dico_size;
     x0 = (fract32)(*x++) << 16;  x0 = x0 + ((unsigned int)x0 >> 16);
     x1 = (fract32)(*x++) << 16;  x1 = x1 + ((unsigned int)x1 >> 16);
     x2 = (fract32)(*x++) << 16;  x2 = x2 + ((unsigned int)x2 >> 16);
     x3 = (fract32)(*x++) << 16;  x3 = x3 + ((unsigned int)x3 >> 16);
     do
     {
        L_dico = *p_dico++;
        L_temp = x0 - (L_dico << 16);   dist = AWB_smultt(L_temp, L_temp);
        L_temp = x1 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        L_dico = *p_dico++;
        L_temp = x2 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);
        L_temp = x3 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);

        if (dist < dist_min) {
          dist_min = dist;
          index = i;
        }
     } while(--i != 0);
     break;
  case 5:
     i = dico_size;
     x0 = (fract32)(*x++) << 16;  x0 = x0 + ((unsigned int)x0 >> 16);
     x1 = (fract32)(*x++) << 16;  x1 = x1 + ((unsigned int)x1 >> 16);
     x2 = (fract32)(*x++) << 16;  x2 = x2 + ((unsigned int)x2 >> 16);
     x3 = (fract32)(*x++) << 16;  x3 = x3 + ((unsigned int)x3 >> 16);
     x4 = (fract32)(*x++) << 16;  x4 = x4 + ((unsigned int)x4 >> 16);
     do
     {
        L_dico = *p_dico++;
        L_temp = x0 - (L_dico << 16);   dist = AWB_smultt(L_temp, L_temp);
        L_temp = x1 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        L_dico = *p_dico++;
        L_temp = x2 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);
        L_temp = x3 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        L_dico = *p_dico++;
        L_temp = x4 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);

        if (dist < dist_min) {
          dist_min = dist;
          index = i;
        }
        L_temp = x0 - (L_dico >> 16);   dist = AWB_smulbb(L_temp, L_temp);
        L_dico = *p_dico++;
        L_temp = x1 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);
        L_temp = x2 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        L_dico = *p_dico++;
        L_temp = x3 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);
        L_temp = x4 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        if (dist < dist_min) {
          dist_min = dist;
          index = i - 1;
        }
     } while((i -= 2) > 0);
     break;
  case 7:
     i = dico_size;
     x0 = (fract32)(*x++) << 16;  x0 = x0 + ((unsigned int)x0 >> 16);
     x1 = (fract32)(*x++) << 16;  x1 = x1 + ((unsigned int)x1 >> 16);
     x2 = (fract32)(*x++) << 16;  x2 = x2 + ((unsigned int)x2 >> 16);
     x3 = (fract32)(*x++) << 16;  x3 = x3 + ((unsigned int)x3 >> 16);
     x4 = (fract32)(*x++) << 16;  x4 = x4 + ((unsigned int)x4 >> 16);
     x5 = (fract32)(*x++) << 16;  x5 = x5 + ((unsigned int)x5 >> 16);
     x6 = (fract32)(*x++) << 16;  x6 = x6 + ((unsigned int)x6 >> 16);
     do
     {
        L_dico = *p_dico++;
        L_temp = x0 - (L_dico << 16);   dist = AWB_smultt(L_temp, L_temp);
        L_temp = x1 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        L_dico = *p_dico++;
        L_temp = x2 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);
        L_temp = x3 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        L_dico = *p_dico++;
        L_temp = x4 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);
        L_temp = x5 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        L_dico = *p_dico++;
        L_temp = x6 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);

        if (dist < dist_min) {
          dist_min = dist;
          index = i;
        }
        L_temp = x0 - (L_dico >> 16);   dist = AWB_smulbb(L_temp, L_temp);
        L_dico = *p_dico++;
        L_temp = x1 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);
        L_temp = x2 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        L_dico = *p_dico++;
        L_temp = x3 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);
        L_temp = x4 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        L_dico = *p_dico++;
        L_temp = x5 - (L_dico << 16);   dist = AWB_smlatt(L_temp, L_temp, dist);
        L_temp = x6 - (L_dico >> 16);   dist = AWB_smlabb(L_temp, L_temp, dist);
        if (dist < dist_min) {
          dist_min = dist;
          index = i - 1;
        }
     } while((i -= 2) > 0);
     break;
  }


  *distance = dist_min;
  index = dico_size - index;

  /* Reading the selected vector */
  p_dico16 = &dico[index * dim];

  pX = x - dim;
  j = dim;
  do
  {
    *pX++ = *p_dico16++;
  } while( --j != 0);

#elif (AWB_OPT_VER == 1)
   fract16 *p_dico;
   fract16 *pX;
   fract32 L_temp;
   fract32 index;

   dist_min = MAX_32;
   p_dico = dico;
   index = 0;

   for (i = 0; i < dico_size; i++)
   {
      dist = 0;

      pX = x;
      j = dim;
      do
      {
         L_temp = *pX++ - *p_dico++;
         dist = AWB_smlabb(L_temp, L_temp, dist);
      } while (--j != 0);

      if (dist < dist_min)
      {
        dist_min = dist;
        index = i;
      }
   }

   *distance = dist_min;

   /* Reading the selected vector */
   p_dico = &dico[index * dim];

   pX = x;
   j = dim;
   do
   {
      *pX++ = *p_dico++;
   } while( --j != 0);
#else
   fract16 *p_dico;
  fract16 index, temp;

  dist_min = MAX_32;
  p_dico = dico;
  index = 0;
  for (i = 0; i < dico_size; i++)
  {
    dist = 0;
    for (j = 0; j < dim; j++)
    {
      temp = Sub_fr1x16(x[j], *p_dico++);
      dist = Mac_fr16_32(dist, temp, temp);
    }

    if (Sub_fr1x32(dist, dist_min) < (fract32) 0)
    {
      dist_min = dist;
      index = (fract16)i;
    }
  }

  *distance = dist_min;

  /* Reading the selected vector */
  p_dico = &dico[index * dim];

  for (j = 0; j < dim; j++)
  {
    x[j] = *p_dico++;
  }
#endif

  return index;

}





WBAMR_CODE_SECTION void WBAMR_VQ_stage1(
  fract16* x,         /* input : ISF WBAMR_Residual vector */
  const fract16* dico,      /* input : quantization codebook */
  fract16 dim,        /* input : dimention of vector */
  fract16 dico_size,  /* input : size of quantization codebook */
  fract16* index,     /* output: indices of survivors */
  fract16 surv,       /* input : number of survivor */
  fract32* dist_min,
  fract16 * enc_obj_temp_ptr
)
{

  int_native i, k, l;
  fract32 dist;

#if (AWB_OPT_VER == 2)
  fract32 *p_dico32;
  fract16 *pX;
  fract32 L_temp;
  fract32 thres_min;
  fract32 x0, x1, x2, x3, x4, dico32;

  for (i = 0; i < surv; i++)
  {
    dist_min[i] = MAX_32;
    index[i] = (fract16)i;
  }

  p_dico32 = (fract32 *)dico;
  pX = x;

  thres_min = dist_min[surv - 1];

  if(dim == 7) {
     x0 = (*pX++);
     x0 = (*pX++ & 0x0000ffff) + (x0 << 16);
     x1 = (*pX++);
     x1 = (*pX++ & 0x0000ffff) + (x1 << 16);
     x2 = (*pX++);
     x2 = (*pX++ & 0x0000ffff) + (x2 << 16);
     x3 = (*pX++) << 16;

     i = dico_size;
     do
     {
       dist = 0;

       dico32 = *p_dico32++;
       L_temp = x0 - (dico32 << 16);
       dist = AWB_smlatt(L_temp, L_temp, dist);
       L_temp = x0 - ((unsigned int)dico32 >> 16);
       dist = AWB_smlabb(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x1 - (dico32 << 16);
       dist = AWB_smlatt(L_temp, L_temp, dist);
       L_temp = x1 - ((unsigned int)dico32 >> 16);
       dist = AWB_smlabb(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x2 - (dico32 << 16);
       dist = AWB_smlatt(L_temp, L_temp, dist);
       L_temp = x2 - ((unsigned int)dico32 >> 16);
       dist = AWB_smlabb(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x3 - (*p_dico32 << 16);
       dist = AWB_smlatt(L_temp, L_temp, dist);

       if(dist < thres_min) {

          for (k = 0; k < surv; k++)
          {
            if (dist < dist_min[k])
            {
              for (l = surv - 1; l > k; l--)
              {
                dist_min[l] = dist_min[l - 1];
                index[l] = index[l - 1];
              }

              dist_min[k] = dist;
              index[k] = (fract16)(dico_size - i);
              thres_min = dist_min[surv - 1];
              break;
            }
          }
       }
       dist = 0;

       dico32 = *p_dico32++;
       L_temp = x0 - dico32;
       dist = AWB_smlatt(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x0 - dico32;
       dist = AWB_smlabb(L_temp, L_temp, dist);
       L_temp = x1 - dico32;
       dist = AWB_smlatt(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x1 - dico32;
       dist = AWB_smlabb(L_temp, L_temp, dist);
       L_temp = x2 - dico32;
       dist = AWB_smlatt(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x2 - dico32;
       dist = AWB_smlabb(L_temp, L_temp, dist);
       L_temp = x3 - dico32;
       dist = AWB_smlatt(L_temp, L_temp, dist);

       if(dist < thres_min) {
          for (k = 0; k < surv; k++)
          {
            if (dist < dist_min[k])
            {
              for (l = surv - 1; l > k; l--)
              {
                dist_min[l] = dist_min[l - 1];
                index[l] = index[l - 1];
              }

              dist_min[k] = dist;
              index[k] = (fract16)(dico_size - i + 1);
              thres_min = dist_min[surv - 1];
              break;
            }
          }
       }
     } while((i -= 2) != 0);


  }
  else if(dim == 9) {
     x0 = (*pX++);
     x0 = (*pX++ & 0x0000ffff) + (x0 << 16);
     x1 = (*pX++);
     x1 = (*pX++ & 0x0000ffff) + (x1 << 16);
     x2 = (*pX++);
     x2 = (*pX++ & 0x0000ffff) + (x2 << 16);
     x3 = (*pX++);
     x3 = (*pX++ & 0x0000ffff) + (x3 << 16);
     x4 = (*pX++) << 16;

     i = dico_size;
     do
     {
       dist = 0;

       dico32 = *p_dico32++;
       L_temp = x0 - (dico32 << 16);
       dist = AWB_smlatt(L_temp, L_temp, dist);
       L_temp = x0 - ((unsigned int)dico32 >> 16);
       dist = AWB_smlabb(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x1 - (dico32 << 16);
       dist = AWB_smlatt(L_temp, L_temp, dist);
       L_temp = x1 - ((unsigned int)dico32 >> 16);
       dist = AWB_smlabb(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x2 - (dico32 << 16);
       dist = AWB_smlatt(L_temp, L_temp, dist);
       L_temp = x2 - ((unsigned int)dico32 >> 16);
       dist = AWB_smlabb(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x3 - (dico32 << 16);
       dist = AWB_smlatt(L_temp, L_temp, dist);
       L_temp = x3 - ((unsigned int)dico32 >> 16);
       dist = AWB_smlabb(L_temp, L_temp, dist);
       L_temp = x4 - (*p_dico32 << 16);
       dist = AWB_smlatt(L_temp, L_temp, dist);

       if(dist < thres_min) {

          for (k = 0; k < surv; k++)
          {
            if (dist < dist_min[k])
            {
              for (l = surv - 1; l > k; l--)
              {
                dist_min[l] = dist_min[l - 1];
                index[l] = index[l - 1];
              }

              dist_min[k] = dist;
              index[k] = (fract16)(dico_size - i);
              thres_min = dist_min[surv - 1];
              break;
            }
          }
       }
       dist = 0;

       dico32 = *p_dico32++;
       L_temp = x0 - dico32;
       dist = AWB_smlatt(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x0 - dico32;
       dist = AWB_smlabb(L_temp, L_temp, dist);
       L_temp = x1 - dico32;
       dist = AWB_smlatt(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x1 - dico32;
       dist = AWB_smlabb(L_temp, L_temp, dist);
       L_temp = x2 - dico32;
       dist = AWB_smlatt(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x2 - dico32;
       dist = AWB_smlabb(L_temp, L_temp, dist);
       L_temp = x3 - dico32;
       dist = AWB_smlatt(L_temp, L_temp, dist);
       dico32 = *p_dico32++;
       L_temp = x3 - dico32;
       dist = AWB_smlabb(L_temp, L_temp, dist);
       L_temp = x4 - dico32;
       dist = AWB_smlatt(L_temp, L_temp, dist);

       if(dist < thres_min) {

          for (k = 0; k < surv; k++)
          {
            if (dist < dist_min[k])
            {
              for (l = surv - 1; l > k; l--)
              {
                dist_min[l] = dist_min[l - 1];
                index[l] = index[l - 1];
              }

              dist_min[k] = dist;
              index[k] = (fract16)(dico_size - i + 1);
              thres_min = dist_min[surv - 1];
              break;
            }
          }
       }

     } while((i -= 2) != 0);
  }

#elif (AWB_OPT_VER == 1)
   fract16 *pX;
   fract32 L_temp;
   fract32 thres_min;
   int_native j;
  fract16 *p_dico;


   for (i = 0; i < surv; i++)
   {
      dist_min[i] = MAX_32;
      index[i] = (fract16)i;
   }

   p_dico = dico;

   thres_min = dist_min[surv - 1];

   for (i = 0; i < dico_size; i++)
   {
      dist = 0;

      pX = x;

      j = dim;
      do
      {
         L_temp = *pX++ - *p_dico++;
         dist = AWB_smlabb(L_temp, L_temp, dist);
      } while (--j != 0);

      if(dist >= thres_min)
         continue;

      for (k = 0; k < surv; k++)
      {
         if (dist < dist_min[k])
         {
            for (l = surv - 1; l > k; l--)
            {
               dist_min[l] = dist_min[l - 1];
               index[l] = index[l - 1];
            }

            dist_min[k] = dist;
            index[k] = (fract16)i;
            thres_min = dist_min[surv - 1];
            break;
         }
      }
   }

#else
  fract16 temp;
   int_native j;
  fract16 *p_dico;


  for (i = 0; i < surv; i++)
  {
    dist_min[i] = MAX_32;
    index[i] = (fract16)i;
  }

  p_dico = dico;

  for (i = 0; i < dico_size; i++)
  {
    dist = 0;

    for (j = 0; j < dim; j++)
    {
      temp = Sub_fr1x16(x[j], *p_dico++);
      dist = Mac_fr16_32(dist, temp, temp);
    }

    for (k = 0; k < surv; k++)
    {
      if (Sub_fr1x32(dist, dist_min[k]) < (fract32) 0)
      {
        for (l = Sub_fr1x16(surv, 1); l > k; l--)
        {
          dist_min[l] = dist_min[l - 1];
          index[l] = index[l - 1];
        }

        dist_min[k] = dist;
        index[k] = (fract16)i;
        break;
      }
    }
  }
#endif

  return;

}

#endif


