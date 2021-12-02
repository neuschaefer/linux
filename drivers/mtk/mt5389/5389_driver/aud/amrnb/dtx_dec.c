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
 *   dtx_dec.c
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
*      File             : dtx_dec.c
*      Purpose          : Decode comfort noise when in DTX
*
*****************************************************************************
*/
/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "dtx_dec.h"

/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "copy.h"
#include "set_zero.h"
#include "lsp_az.h"
#include "pow2.h"
#include "a_refl.h"
#include "b_cn_cod.h"
#include "syn_filt.h"
#include "lsp_lsf.h"

#include "tab_lsp.h"

#include "tab_lsf_5.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/
#define PN_INITIAL_SEED 0x70816958L   /* Pseudo noise generator seed value  */

/***************************************************
 * Scaling factors for the lsp variability operation *
 ***************************************************/
static const Word16 lsf_hist_mean_scale[M] = {
   20000,
   20000,
   20000,
   20000,
   20000,
   18000,
   16384,
    8192,
       0,
       0
};

/*************************************************
 * level adjustment for different modes Q11      *
 *************************************************/
static const Word16 dtx_log_en_adjust[9] =
{
  -1023, /* MR475 */
   -878, /* MR515 */
   -732, /* MR59  */
   -586, /* MR67  */
   -440, /* MR74  */
   -294, /* MR795 */
   -148, /* MR102 */
      0, /* MR122 */
      0, /* MRDTX */
};

/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/


/*
**************************************************************************
*
*  Function    : dtx_dec_reset
*
**************************************************************************
*/
int dtx_dec_reset (dtx_decState *st)
{
//   int i;

   st->since_last_sid = 0;
   st->true_sid_period_inv = (1 << 13);

   st->log_en = 3500;
   st->old_log_en = 3500;
   /* low level noise for better performance in  DTX handover cases*/

   st->L_pn_seed_rx = PN_INITIAL_SEED;

   /* Initialize state->lsp [] and state->lsp_old [] */
   Copy(lsp_init_data, &st->lsp[0], M);
   Copy(lsp_init_data, &st->lsp_old[0], M);

   st->lsf_hist_ptr = 0;
   st->log_pg_mean = 0;
   st->log_en_hist_ptr = 0;

   /* initialize decoder lsf history */
   Copy(mean_lsf, &st->lsf_hist[0], M);

/*
   for (i = 1; i < DTX_HIST_SIZE; i++)
   {
      Copy(&st->lsf_hist[0], &st->lsf_hist[M*i], M);
   }
*/
	Copy(&st->lsf_hist[0], &st->lsf_hist[M], M);
	Copy(&st->lsf_hist[0], &st->lsf_hist[M*2], M);
	Copy(&st->lsf_hist[0], &st->lsf_hist[M*3], M);
	Copy(&st->lsf_hist[0], &st->lsf_hist[M*4], M);
	Copy(&st->lsf_hist[0], &st->lsf_hist[M*5], M);
	Copy(&st->lsf_hist[0], &st->lsf_hist[M*6], M);
	Copy(&st->lsf_hist[0], &st->lsf_hist[M*7], M);


   Set_zero(st->lsf_hist_mean, M*DTX_HIST_SIZE);

   /* initialize decoder log frame energy */
/*
   for (i = 0; i < DTX_HIST_SIZE; i++)
   {
      st->log_en_hist[i] = st->log_en;
   }
*/
	st->log_en_hist[0] = st->log_en;
	st->log_en_hist[1] = st->log_en;
	st->log_en_hist[2] = st->log_en;
	st->log_en_hist[3] = st->log_en;
	st->log_en_hist[4] = st->log_en;
	st->log_en_hist[5] = st->log_en;
	st->log_en_hist[6] = st->log_en;
	st->log_en_hist[7] = st->log_en;



   st->log_en_adjust = 0;

   st->dtxHangoverCount = DTX_HANG_CONST;
   st->decAnaElapsedCount = 32767;

   st->sid_frame = 0;
   st->valid_data = 0;
   st->dtxHangoverAdded = 0;

   st->dtxGlobalState = DTX;
   st->data_updated = 0;
   return 0;

}

/*
**************************************************************************
*
*  Function    : dtx_dec
*
**************************************************************************
*/
int dtx_dec(
   AMR_Decode_FrameState *Dec,
   dtx_decState *st,                /* i/o : State struct                    */
   Word16 mem_syn[],                /* i/o : AMR decoder state               */
   D_plsfState* lsfState,           /* i/o : decoder lsf states              */
   gc_predState* predState,         /* i/o : prediction states               */
   Cb_gain_averageState* averState, /* i/o : CB gain average states          */
   enum DTXStateType new_state,     /* i   : new DTX state                   */
   enum Mode mode,                  /* i   : AMR mode                        */
   Word16 parm[],                   /* i   : Vector of synthesis parameters  */
   Word16 synth[],                  /* o   : synthesised speech              */
   Word16 A_t[]                     /* o   : decoded LP filter in 4 subframes*/
   )
{
	AMR_Shared_Buf *Dec_share = Dec->share;

   Flag negative;
   Word32 L_log_en_int;
   Word32 L_lsf_mean;
   Word32 i, j;

#if 0
	Word32 L_lsf[M];
   Word16 lsp_int[M];
   Word16 acoeff[M + 1 +1];
   Word16 refl[M];
   Word16 ex[L_SUBFR];
   Word16 lsf_int[M];
   Word16 lsf_int_variab[M];
   Word16 lsp_int_variab[M];
   Word16 acoeff_variab[M + 1 +1];
   Word16 lsf[M];
#endif

	Word32 *L_lsf = Dec_share->shabuf.stDtx_D.L_lsf;
   Word16 *lsp_int = Dec_share->shabuf.stDtx_D.lsp_int;
   Word16 *acoeff = Dec_share->shabuf.stDtx_D.acoeff;
   Word16 *refl = Dec_share->shabuf.stDtx_D.refl;
   Word16 *ex = Dec_share->shabuf.stDtx_D.ex;
   Word16 *lsf_int = Dec_share->shabuf.stDtx_D.lsf_int;
   Word16 *lsf_int_variab = Dec_share->shabuf.stDtx_D.lsf_int_variab;
   Word16 *lsp_int_variab = Dec_share->shabuf.stDtx_D.lsp_int_variab;
   Word16 *acoeff_variab = Dec_share->shabuf.stDtx_D.acoeff_variab;
   Word16 *lsf = Dec_share->shabuf.stDtx_D.lsf;



   Word16 log_en_index;
   Word16 int_fac;
   Word16 log_en_int_e;
   Word16 log_en_int_m;
   Word16 level;
   Word16 pred_err;
   Word16 ma_pred_init;
   Word16 log_pg_e, log_pg_m;
   Word16 log_pg;
   Word16 lsf_mean;
   Word16 lsf_variab_index;
   Word16 lsf_variab_factor;
   Word16 ptr;
   Word16 tmp_int_length;

   /*  This function is called if synthesis state is not SPEECH
    *  the globally passed  inputs to this function are
    * st->sid_frame
    * st->valid_data
    * st->dtxHangoverAdded
    * new_state  (SPEECH, DTX, DTX_MUTE)
    */
   if( (st->dtxHangoverAdded != 0) && (st->sid_frame != 0) )
   {
      /* sid_first after dtx hangover period */
      /* or sid_upd after dtxhangover        */

      /* set log_en_adjust to correct value */
      st->log_en_adjust = dtx_log_en_adjust[mode];
      ptr = (st->lsf_hist_ptr + M);

      if(ptr == 80)
      {
         ptr = 0;
      }

      Copy( &st->lsf_hist[st->lsf_hist_ptr],&st->lsf_hist[ptr],M);
      ptr = (st->log_en_hist_ptr + 1);

      if(ptr == DTX_HIST_SIZE)
      {
         ptr = 0;
      }
      st->log_en_hist[ptr] = st->log_en_hist[st->log_en_hist_ptr]; /* Q11 */
      /* compute mean log energy and lsp *
       * from decoded signal (SID_FIRST) */
      st->log_en = 0;

 		for (i = 0; i < M; i+=2)
      {
         L_lsf[i] = 0;
         L_lsf[i+1] = 0;
      }

      /* average energy and lsp */
      for (i = 0; i < DTX_HIST_SIZE; i++)
      {
         st->log_en = (st->log_en + (st->log_en_hist[i] >> 3));

         for (j = 0; j < M; j++)
         {
            L_lsf[j] = qadd(L_lsf[j], (st->lsf_hist[i * M + j]));
         }
      }

      for (j = 0; j < M; j++)
      {
         lsf[j] = (Word16)(L_lsf[j] >> 3); /* divide by 8 */
      }

      Lsf_lsp(lsf, st->lsp);

      /* make log_en speech coder mode independent */
      /* added again later before synthesis        */
      st->log_en = (st->log_en - st->log_en_adjust);

      /* compute lsf variability vector */
//    Copy(st->lsf_hist, st->lsf_hist_mean, 80);
      AMR_Copy32((Word32 *)st->lsf_hist, (Word32 *)st->lsf_hist_mean, 40);

      for (i = 0; i < M; i++)
      {
         L_lsf_mean = 0;
         /* compute mean lsf */
         for (j = 0; j < 8; j++)
         {
            L_lsf_mean = qadd(L_lsf_mean, (st->lsf_hist_mean[i+j*M]));
         }

         lsf_mean = (Word16)(L_lsf_mean >> 3);
         /* subtract mean and limit to within reasonable limits  *
          * moreover the upper lsf's are attenuated              */
         for (j = 0; j < 8; j++)
         {
            /* subtract mean */
            st->lsf_hist_mean[i+j*M] = (st->lsf_hist_mean[i+j*M] - lsf_mean);

            /* attenuate deviation from mean, especially for upper lsf's */
            st->lsf_hist_mean[i+j*M] = (smulbb(st->lsf_hist_mean[i+j*M], lsf_hist_mean_scale[i]) >> 15);

            /* limit the deviation */

            if (st->lsf_hist_mean[i+j*M] < 0)
            {
               negative = 1;
            }
            else
            {
               negative = 0;
            }
            st->lsf_hist_mean[i+j*M] = abs_s(st->lsf_hist_mean[i+j*M]);

            /* apply soft limit */

            if(st->lsf_hist_mean[i+j*M] > 655)
            {
               st->lsf_hist_mean[i+j*M] = (655 + ((st->lsf_hist_mean[i+j*M] - 655) >> 2));
            }

            /* apply hard limit */

            if(st->lsf_hist_mean[i+j*M] > 1310)
            {
               st->lsf_hist_mean[i+j*M] = 1310;
            }

            if (negative != 0)
            {
               st->lsf_hist_mean[i+j*M] = -st->lsf_hist_mean[i+j*M];
            }

         }
      }
   }


   if (st->sid_frame != 0 )
   {
      /* Set old SID parameters, always shift */
      /* even if there is no new valid_data   */
      Copy(st->lsp, st->lsp_old, M);
      st->old_log_en = st->log_en;


      if (st->valid_data != 0 )  /* new data available (no CRC) */
      {
         /* Compute interpolation factor, since the division only works *
          * for values of since_last_sid < 32 we have to limit the      *
          * interpolation to 32 frames                                  */
         tmp_int_length = st->since_last_sid;
         st->since_last_sid = 0;


         if(tmp_int_length > 32)
         {
            tmp_int_length = 32;
         }

         if(tmp_int_length >= 2)
         {

            st->true_sid_period_inv = div_s(1 << 10, sature(tmp_int_length << 10));
         }
         else
         {
            st->true_sid_period_inv = 1 << 14; /* 0.5 it Q15 */
         }

         Init_D_plsf_3(lsfState, parm[0]);  /* temporay initialization */
         D_plsf_3(Dec_share->shabuf.stDtx_D.u0D.lsf1_q , lsfState, MRDTX, 0, &parm[1], st->lsp);
         Set_zero(lsfState->past_r_q, M);   /* reset for next speech frame */

         log_en_index = parm[4];
         /* Q11 and divide by 4 */
         st->log_en = sature(log_en_index << (11 - 2));

         /* Subtract 2.5 in Q11 */
         st->log_en = (st->log_en - (2560 * 2));

         /* Index 0 is reserved for silence */

         if (log_en_index == 0)
         {
            st->log_en = MIN_16;
         }

         /* no interpolation at startup after coder reset        */
         /* or when SID_UPD has been received right after SPEECH */

         if ((st->data_updated == 0) ||
              (st->dtxGlobalState == SPEECH)
             )
         {
            Copy(st->lsp, st->lsp_old, M);
            st->old_log_en = st->log_en;
         }
      } /* endif valid_data */

      /* initialize gain predictor memory of other modes */
      ma_pred_init = ((st->log_en >> 1) - 9000);

      if (ma_pred_init > 0)
      {
         ma_pred_init = 0;
      }

      if(ma_pred_init < -14436)
      {
         ma_pred_init = -14436;
      }

      predState->past_qua_en[0] = ma_pred_init;
      predState->past_qua_en[1] = ma_pred_init;
      predState->past_qua_en[2] = ma_pred_init;
      predState->past_qua_en[3] = ma_pred_init;

      /* past_qua_en for other modes than MR122 */
      ma_pred_init = smulbb(5443, ma_pred_init) >> 15;
      /* scale down by factor 20*log10(2) in Q15 */
      predState->past_qua_en_MR122[0] = ma_pred_init;
      predState->past_qua_en_MR122[1] = ma_pred_init;
      predState->past_qua_en_MR122[2] = ma_pred_init;
      predState->past_qua_en_MR122[3] = ma_pred_init;
   } /* endif sid_frame */

   /* CN generation */
   /* recompute level adjustment factor Q11             *
    * st->log_en_adjust = 0.9*st->log_en_adjust +       *
    *                     0.1*dtx_log_en_adjust[mode]); */

   st->log_en_adjust = ( (smulbb(st->log_en_adjust, 29491)>>15) +
                           (smulbb(sature(dtx_log_en_adjust[mode]<<5),3277)>> 20 ) );

   /* Interpolate SID info */
   int_fac = sature((1 + st->since_last_sid) << 10); /* Q10 */
   int_fac = (smulbb(int_fac, st->true_sid_period_inv) >> 15); /* Q10 * Q15 -> Q10 */

   /* Maximize to 1.0 in Q10 */

   if(int_fac > 1024)
   {
      int_fac = 1024;
   }
   int_fac = sature(int_fac << 4); /* Q10 -> Q14 */

   L_log_en_int = smulbb(int_fac, st->log_en); /* Q14 * Q11->Q26 */
	L_log_en_int = qadd(L_log_en_int,L_log_en_int);

   for(i = 0; i < M; i++)
   {
      lsp_int[i] = smulbb(int_fac, st->lsp[i]) >> 15 ; /* Q14 * Q15 -> Q14 */
   }

   int_fac = (16384 - int_fac); /* 1-k in Q14 */

   /* (Q14 * Q11 -> Q26) + Q26 -> Q26 */

   L_log_en_int = qdadd(L_log_en_int,smulbb(int_fac, st->old_log_en));

   for(i = 0; i < M; i++)
   {
      /* Q14 + (Q14 * Q15 -> Q14) -> Q14 */
      lsp_int[i] = (lsp_int[i] + (smulbb(int_fac, st->lsp_old[i])>>15) );
      lsp_int[i] = sature(lsp_int[i] << 1); /* Q14 -> Q15 */
   }

   /* compute the amount of lsf variability */
   lsf_variab_factor = (st->log_pg_mean - 2457); /* -0.6 in Q12 */
   /* *0.3 Q12*Q15 -> Q12 */
   lsf_variab_factor = (4096 - (smulbb(lsf_variab_factor, 9830)>>15) );

   /* limit to values between 0..1 in Q12 */

   if(lsf_variab_factor > 4096)
   {
      lsf_variab_factor = 4096;
   }

   if (lsf_variab_factor < 0)
   {
      lsf_variab_factor = 0;
   }

   lsf_variab_factor = sature(lsf_variab_factor << 3); /* -> Q15 */

   /* get index of vector to do variability with */
   lsf_variab_index = pseudonoise(&st->L_pn_seed_rx, 3);

   /* convert to lsf */
   Lsp_lsf(lsp_int, lsf_int);

   /* apply lsf variability */
   Copy(lsf_int, lsf_int_variab, M);
   for(i = 0; i < M; i++)
   {

      lsf_int_variab[i] = (lsf_int_variab[i] +
                              (smulbb(lsf_variab_factor,
                                   st->lsf_hist_mean[i+lsf_variab_index*M])>>15)  );
   }

   /* make sure that LSP's are ordered */
   Reorder_lsf(lsf_int);
   Reorder_lsf(lsf_int_variab);

   /* copy lsf to speech decoders lsf state */
   Copy(lsf_int, lsfState->past_lsf_q, M);

   /* convert to lsp */
   Lsf_lsp(lsf_int, lsp_int);
   Lsf_lsp(lsf_int_variab, lsp_int_variab);

   /* Compute acoeffs Q12 acoeff is used for level    *
    * normalization and postfilter, acoeff_variab is  *
    * used for synthesis filter                       *
    * by doing this we make sure that the level       *
    * in high frequenncies does not jump up and down  */

//   Lsp_Az(Dec_share->shabuf.stDtx_D.u0D.s0D.f1,Dec_share->shabuf.stDtx_D.u0D.s0D.f2 ,lsp_int, acoeff);
//   Lsp_Az(Dec_share->shabuf.stDtx_D.u0D.s0D.f1,Dec_share->shabuf.stDtx_D.u0D.s0D.f2 ,lsp_int_variab, acoeff_variab);
   Lsp_Az(Dec_share->shabuf.stDtx_D.f1 ,Dec_share->shabuf.stDtx_D.f2 ,
   			lsp_int, acoeff);
   Lsp_Az(Dec_share->shabuf.stDtx_D.f1 ,Dec_share->shabuf.stDtx_D.f2 ,
				lsp_int_variab, acoeff_variab);


   /* For use in postfilter */
   Copy(acoeff, &A_t[0],              M + 1+1);
   Copy(acoeff, &A_t[M + 1 +1],       M + 1+1);
   Copy(acoeff, &A_t[2 * (M + 1 +1)], M + 1+1);
   Copy(acoeff, &A_t[3 * (M + 1 +1)], M + 1+1);

   /* Compute reflection coefficients Q15 */
   A_Refl(Dec_share->shabuf.stDtx_D.u0D.s1D.aState,Dec_share->shabuf.stDtx_D.u0D.s1D.bState,
			   &acoeff[1], refl);

   /* Compute prediction error in Q15 */
   pred_err = MAX_16; /* 0.99997 in Q15 */
	{
		Word32 *lptr,t;
		lptr = (Word32 *)refl;
		i = M/2;
		do
	   {
	   	t = *lptr++;
   	   pred_err = smulbb(pred_err, (MAX_16- (smulbb(t, t)>>15)) ) >> 15;
   	   pred_err = smulbb(pred_err, (MAX_16- (smultt(t, t)>>15)) ) >> 15;

	   }while(--i != 0);
   }
   /* compute logarithm of prediction gain */
   Log2((pred_err), &log_pg_e, &log_pg_m);

   /* convert exponent and mantissa to Word16 Q12 */
   log_pg = sature( (log_pg_e - 15) << 12);  /* Q12 */
   log_pg = ( (0 - (log_pg + (log_pg_m >> (15-12) ))) >> 1);
   st->log_pg_mean = ( (smulbb(29491,st->log_pg_mean)>>15) + (smulbb(3277, log_pg)>>15) );

   /* Compute interpolated log energy */
   L_log_en_int = (L_log_en_int >> 10); /* Q26 -> Q16 */

   /* Add 4 in Q16 */
   L_log_en_int = qadd(L_log_en_int, 4 * 65536L);

   /* subtract prediction gain */
   L_log_en_int = qsub(L_log_en_int, L_shl((log_pg), 4));

   /* adjust level to speech coder mode */
   L_log_en_int = qadd(L_log_en_int, L_shl((st->log_en_adjust), 5) );

   log_en_int_e = (Word16)(L_log_en_int >> 16);

   log_en_int_m = (Word16)((qsub(L_log_en_int,(log_en_int_e<<16)) >> 1));

   level = (Word16)(Pow2(log_en_int_e, log_en_int_m)); /* Q4 */

   for (i = 0; i < 4; i++)
   {
   	Word32 *lptr,t;
		Word16 *sptr;
      /* Compute innovation vector */
      build_CN_code(&st->L_pn_seed_rx, ex);

		lptr = (Word32 *)ex;
		sptr = ex;
		j = L_SUBFR/4;
		do
      {
      	t = *lptr++;
         *sptr++ = smulbb(level, t) >> 15;
         *sptr++ = smulbt(level, t) >> 15;
      	t = *lptr++;
         *sptr++ = smulbb(level, t) >> 15;
         *sptr++ = smulbt(level, t) >> 15;
      }while(--j != 0);


      /* Synthesize */
      Syn_filt(Dec_share->shabuf.stDtx_D.u0D.tmp_w ,
      acoeff_variab, ex, &synth[i * L_SUBFR], L_SUBFR, mem_syn, 1);

   } /* next i */

   /* reset codebook averaging variables */
   averState->hangVar = 20;
   averState->hangCount = 0;

   if(new_state == DTX_MUTE)
   {
      /* mute comfort noise as it has been quite a long time since
       * last SID update  was performed                            */

      tmp_int_length = st->since_last_sid;

      if(tmp_int_length > 32)
      {
         tmp_int_length = 32;
      }

      /* safety guard against division by zero */

      if(tmp_int_length <= 0) {
         tmp_int_length = 8;
      }

      st->true_sid_period_inv = div_s(1 << 10, sature(tmp_int_length << 10));

      st->since_last_sid = 0;
      Copy(st->lsp, st->lsp_old, M);
      st->old_log_en = st->log_en;
      /* subtract 1/8 in Q11 i.e -6/8 dB */
      st->log_en = (st->log_en - 256);
   }

   /* reset interpolation length timer
    * if data has been updated.        */

   if ((st->sid_frame != 0) &&
       ((st->valid_data != 0) ||
        ((st->valid_data == 0) &&  (st->dtxHangoverAdded) != 0)))
   {
      st->since_last_sid =  0;
      st->data_updated = 1;
   }

   return 0;
}

void dtx_dec_activity_update(dtx_decState *st,
                             Word16 lsf[],
                             Word16 frame[])
{
   Word32 i,t,*lptr;
   Word32 L_frame_en;
   Word16 log_en_e, log_en_m, log_en;

   /* update lsp history */
   st->lsf_hist_ptr = (st->lsf_hist_ptr + M);

   if(st->lsf_hist_ptr == 80)
   {
      st->lsf_hist_ptr = 0;
   }

   Copy(lsf, &st->lsf_hist[st->lsf_hist_ptr], M);

   /* compute log energy based on frame energy */
   L_frame_en = 0;     /* Q0 */

/*
	for (i=0; i < L_FRAME; i++)
   {
      L_frame_en = L_mac(L_frame_en, frame[i], frame[i]);
   }
*/
/*
	i = L_FRAME/4;
	do
   {
      L_frame_en = qdadd(L_frame_en, smulbb(*frame,*frame) );
		frame++;
      L_frame_en = qdadd(L_frame_en, smulbb(*frame,*frame) );
		frame++;
      L_frame_en = qdadd(L_frame_en, smulbb(*frame,*frame) );
		frame++;
      L_frame_en = qdadd(L_frame_en, smulbb(*frame,*frame) );
		frame++;
   }while(--i != 0);
*/
	lptr = (Word32 *)frame;
	i = L_FRAME/4;
	do
   {
   	t = *lptr++;
      L_frame_en = qdadd(L_frame_en, smulbb(t,t) );
      L_frame_en = qdadd(L_frame_en, smultt(t,t) );
   	t = *lptr++;
      L_frame_en = qdadd(L_frame_en, smulbb(t,t) );
      L_frame_en = qdadd(L_frame_en, smultt(t,t) );
   }while(--i != 0);


   Log2(L_frame_en, &log_en_e, &log_en_m);

   /* convert exponent and mantissa to Word16 Q10 */
   log_en = sature(log_en_e << 10);  /* Q10 */
   log_en = (log_en + (log_en_m >> (15-10) ));

   /* divide with L_FRAME i.e subtract with log2(L_FRAME) = 7.32193 */
   log_en = (log_en - (7497+1024));

   /* insert into log energy buffer, no division by two as  *
    * log_en in decoder is Q11                              */
   st->log_en_hist_ptr = (st->log_en_hist_ptr + 1);

   if(st->log_en_hist_ptr == DTX_HIST_SIZE)
   {
      st->log_en_hist_ptr = 0;
   }

   st->log_en_hist[st->log_en_hist_ptr] = log_en; /* Q11 */

}

/*
     Table of new SPD synthesis states

                           |     previous SPD_synthesis_state
     Incoming              |
     frame_type            | SPEECH       | DTX           | DTX_MUTE
     ---------------------------------------------------------------
     RX_SPEECH_GOOD ,      |              |               |
     RX_SPEECH_PR_DEGRADED | SPEECH       | SPEECH        | SPEECH
     ----------------------------------------------------------------
     RX_SPEECH_BAD,        | SPEECH       | DTX           | DTX_MUTE
     ----------------------------------------------------------------
     RX_SID_FIRST,         | DTX          | DTX/(DTX_MUTE)| DTX_MUTE
     ----------------------------------------------------------------
     RX_SID_UPDATE,        | DTX          | DTX           | DTX
     ----------------------------------------------------------------
     RX_SID_BAD,           | DTX          | DTX/(DTX_MUTE)| DTX_MUTE
     ----------------------------------------------------------------
     RX_NO_DATA            | SPEECH       | DTX/(DTX_MUTE)| DTX_MUTE
                           |(class2 garb.)|               |
     ----------------------------------------------------------------
     RX_ONSET              | SPEECH       | DTX/(DTX_MUTE)| DTX_MUTE
                           |(class2 garb.)|               |
     ----------------------------------------------------------------
*/


enum DTXStateType rx_dtx_handler(
                      dtx_decState *st,           /* i/o : State struct     */
                      enum RXFrameType frame_type /* i   : Frame type       */
                      )
{
   enum DTXStateType newState;
   enum DTXStateType encState;

   /* DTX if SID frame or previously in DTX{_MUTE} and (NO_RX OR BAD_SPEECH) */

   if ((frame_type == RX_SID_FIRST)    ||
       (frame_type == RX_SID_UPDATE)   ||
       (frame_type == RX_SID_BAD)      ||
       (( (st->dtxGlobalState == DTX)  ||
          (st->dtxGlobalState == DTX_MUTE) ) &&
        ( (frame_type == RX_NO_DATA)  ||
          (frame_type == RX_SPEECH_BAD)  ||
          (frame_type == RX_ONSET) ))
       )
   {
      newState = DTX;

      /* stay in mute for these input types */
      if( (st->dtxGlobalState == DTX_MUTE)  &&
          ( (frame_type == RX_SID_BAD)  ||
           (frame_type == RX_SID_FIRST)  ||
           (frame_type == RX_ONSET)  ||
           (frame_type == RX_NO_DATA) )
         )
      {
         newState = DTX_MUTE;
      }

      /* evaluate if noise parameters are too old                     */
      /* since_last_sid is reset when CN parameters have been updated */
      st->since_last_sid = (st->since_last_sid + 1);

      /* no update of sid parameters in DTX for a long while */
      /* Due to the delayed update of  st->since_last_sid counter
         SID_UPDATE frames need to be handled separately to avoid
         entering DTX_MUTE for late SID_UPDATE frames
         */

      if( (frame_type != RX_SID_UPDATE)  &&
          (st->since_last_sid > DTX_MAX_EMPTY_THRESH) )
      {
         newState = DTX_MUTE;
      }
   }
   else
   {
      newState = SPEECH;
      st->since_last_sid = 0;
   }

   /*
      reset the decAnaElapsed Counter when receiving CNI data the first
      time, to robustify counter missmatch after handover
      this might delay the bwd CNI analysis in the new decoder slightly.
   */

   if ((st->data_updated == 0) &&
       (frame_type == RX_SID_UPDATE) )
   {
      st->decAnaElapsedCount = 0;
   }

   /* update the SPE-SPD DTX hangover synchronization */
   /* to know when SPE has added dtx hangover         */
   st->decAnaElapsedCount = add(st->decAnaElapsedCount , 1);
   st->dtxHangoverAdded = 0;

   if ((frame_type == RX_SID_FIRST)   ||
       (frame_type == RX_SID_UPDATE)  ||
       (frame_type == RX_SID_BAD)     ||
       (frame_type == RX_ONSET)       ||
       (frame_type == RX_NO_DATA) )
   {
      encState = DTX;

      /*
         In frame errors simulations RX_NO_DATA may occasionally mean that
         a speech packet was probably sent by the encoder,
         the assumed _encoder_ state should be SPEECH in such cases.
      */

      if( (frame_type == RX_NO_DATA)  &&
          (newState == SPEECH) )
      {
         encState = SPEECH;
      }


      /* Note on RX_ONSET operation differing from RX_NO_DATA operation:
         If a  RX_ONSET is received in the decoder (by "accident")
         it is still most likely that the encoder  state
         for the "ONSET frame" was DTX.
      */
   }
   else
   {
      encState = SPEECH;
   }


   if(encState == SPEECH)
   {
      st->dtxHangoverCount = DTX_HANG_CONST;
   }
   else
   {

      if(st->decAnaElapsedCount > DTX_ELAPSED_FRAMES_THRESH)
      {
         st->dtxHangoverAdded = 1;
         st->decAnaElapsedCount = 0;
         st->dtxHangoverCount = 0;
      }
      else if ( st->dtxHangoverCount == 0)
      {
         st->decAnaElapsedCount = 0;
      }
      else
      {
         st->dtxHangoverCount = (st->dtxHangoverCount - 1);
      }
   }

   if(newState != SPEECH)
   {
      /* DTX or DTX_MUTE
       * CN data is not in a first SID, first SIDs are marked as SID_BAD
       *  but will do backwards analysis if a hangover period has been added
       *  according to the state machine above
       */

      st->sid_frame = 0;
      st->valid_data = 0;


      if(frame_type == RX_SID_FIRST)
      {
         st->sid_frame = 1;
      }
      else if(frame_type == RX_SID_UPDATE)
      {
         st->sid_frame = 1;
         st->valid_data = 1;
      }
      else if(frame_type == RX_SID_BAD)
      {
         st->sid_frame = 1;
         st->dtxHangoverAdded = 0; /* use old data */
      }
   }

   return newState;
   /* newState is used by both SPEECH AND DTX synthesis routines */

}

#endif

