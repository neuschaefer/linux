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
 *   dec_amr.c
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

/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : dec_amr.c
*      Purpose          : Decoding of one speech frame using given codec mode
*
*****************************************************************************
*/

/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "dec_amr.h"



#include "typedef.h"
#include "basic_op.h"
#include "copy.h"
#include "set_zero.h"
#include "syn_filt.h"
#include "d_plsf.h"
#include "agc.h"
#include "int_lpc.h"
#include "dec_gain.h"
#include "dec_lag3.h"
#include "dec_lag6.h"
#include "d2_9pf.h"
#include "d2_11pf.h"
#include "d3_14pf.h"
#include "d4_17pf.h"
#include "d8_31pf.h"
#include "d1035pf.h"
#include "pred_lt.h"
#include "d_gain_p.h"
#include "d_gain_c.h"
#include "dec_gain.h"
#include "ec_gains.h"
#include "ph_disp.h"
#include "c_g_aver.h"
#include "int_lsf.h"
#include "lsp_lsf.h"
#include "lsp_avg.h"
#include "bgnscd.h"
#include "ex_ctrl.h"
#include "sqrt_l.h"

#include "tab_lsp.h"
#include "tab_bitno.h"
#include "b_cn_cod.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/
/*-----------------------------------------------------------------*
 *   Decoder constant parameters (defined in "cnst.h")             *
 *-----------------------------------------------------------------*
 *   L_FRAME       : Frame size.                                   *
 *   L_FRAME_BY2   : Half the frame size.                          *
 *   L_SUBFR       : Sub-frame size.                               *
 *   M             : LPC order.                                    *
 *   MP1           : LPC order+1                                   *
 *   PIT_MIN       : Minimum pitch lag.                            *
 *   PIT_MIN_MR122 : Minimum pitch lag for the MR122 mode.         *
 *   PIT_MAX       : Maximum pitch lag.                            *
 *   L_INTERPOL    : Length of filter for interpolation            *
 *   PRM_SIZE      : size of vector containing analysis parameters *
 *-----------------------------------------------------------------*/

/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/

#if 0

/*
**************************************************************************
*
*  Function    : Decoder_amr_init
*  Purpose     : Allocates and initializes state memory
*
**************************************************************************
*/
int Decoder_amr_init (Decoder_amrState **state)
{
  Decoder_amrState* s;
  Word32 i;

  *state = NULL;

  /* allocate memory */
  if ((s= (Decoder_amrState *) malloc(  ((sizeof(Decoder_amrState)+3)&(~3))  )) == NULL){
      fprintf(stderr, "Decoder_amr_init: can not malloc state structure\n");
      return -1;
  }

  s->T0_lagBuff = 40;
  s->inBackgroundNoise = 0;
  s->voicedHangover = 0;
  for (i = 0; i < 9; i++)
     s->ltpGainHistory[i] = 0;

//  s->lsfState = NULL;
//  s->ec_gain_p_st = NULL;
//  s->ec_gain_c_st = NULL;
//  s->pred_state = NULL;
//  s->ph_disp_st = NULL;
//  s->dtxDecoderState = NULL;

//  if (
//  		D_plsf_init(&s->lsfState) ||
//      ec_gain_pitch_init(&s->ec_gain_p_st) ||
//      ec_gain_code_init(&s->ec_gain_c_st) ||
//      gc_pred_init(&s->pred_state) ||
//      Cb_gain_average_init(&s->Cb_gain_averState) ||
//      lsp_avg_init(&s->lsp_avg_st) ||
//      Bgn_scd_init(&s->background_state) ||      
//      ph_disp_init(&s->ph_disp_st) ||
//      dtx_dec_init(&s->dtxDecoderState)
//      )
//	{
//      Decoder_amr_exit(&s);
//      return -1;
//  }

  Decoder_amr_reset(s, (enum Mode)0);
  *state = s;

  return 0;
}


/*
**************************************************************************
*
*  Function    : Decoder_amr_exit
*  Purpose     : The memory used for state memory is freed
*
**************************************************************************
*/
void Decoder_amr_exit (Decoder_amrState **state)
{
  if (state == NULL || *state == NULL)
      return;

//  D_plsf_exit(&(*state)->lsfState);
//  ec_gain_pitch_exit(&(*state)->ec_gain_p_st);
//  ec_gain_code_exit(&(*state)->ec_gain_c_st);
//  gc_pred_exit(&(*state)->pred_state);
//  Bgn_scd_exit(&(*state)->background_state);
//  ph_disp_exit(&(*state)->ph_disp_st);
//  Cb_gain_average_exit(&(*state)->Cb_gain_averState);
//  lsp_avg_exit(&(*state)->lsp_avg_st);
//  dtx_dec_exit(&(*state)->dtxDecoderState);

  /* deallocate memory */
  free(*state);
  *state = NULL;

  return;
}



#endif

/*
**************************************************************************
*
*  Function    : Decoder_amr_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int Decoder_amr_reset (Decoder_amrState *state, enum Mode mode)
{
//  Word32 i;

  /* Initialize static pointer */
  state->exc = state->old_exc + PIT_MAX + L_INTERPOL;

  /* Static vectors to zero */
  Set_zero (state->old_exc, PIT_MAX + L_INTERPOL);

  if (mode != MRDTX)
     Set_zero (state->mem_syn, M);

  /* initialize pitch sharpening */
  state->sharp = SHARPMIN;
  state->old_T0 = 40;

  /* Initialize state->lsp_old [] */

  if (mode != MRDTX) {
      Copy(lsp_init_data, &state->lsp_old[0], M);
  }

  /* Initialize memories of bad frame handling */
  state->prev_bf = 0;
  state->prev_pdf = 0;
  state->state = 0;

  state->T0_lagBuff = 40;
  state->inBackgroundNoise = 0;
  state->voicedHangover = 0;
	if (mode != MRDTX)
	{
		state->excEnergyHist[0] = 0;
		state->excEnergyHist[1] = 0;
		state->excEnergyHist[2] = 0;
		state->excEnergyHist[3] = 0;
		state->excEnergyHist[4] = 0;
		state->excEnergyHist[5] = 0;
		state->excEnergyHist[6] = 0;
		state->excEnergyHist[7] = 0;
		state->excEnergyHist[8] = 0;
  	}

	state->ltpGainHistory[0] = 0;
	state->ltpGainHistory[1] = 0;
	state->ltpGainHistory[2] = 0;
	state->ltpGainHistory[3] = 0;
	state->ltpGainHistory[4] = 0;
	state->ltpGainHistory[5] = 0;
	state->ltpGainHistory[6] = 0;
	state->ltpGainHistory[7] = 0;
	state->ltpGainHistory[8] = 0;


  Cb_gain_average_reset(&(state->Cb_gain_averState));

  if (mode != MRDTX)
     lsp_avg_reset(&(state->lsp_avg_st));

  D_plsf_reset(&(state->lsfState));
  ec_gain_pitch_reset(&(state->ec_gain_p_st));
  ec_gain_code_reset(&(state->ec_gain_c_st));

  if (mode != MRDTX)
     gc_pred_reset(&(state->pred_state));

  Bgn_scd_reset(&(state->background_state));
  state->nodataSeed = 21845;
  ph_disp_reset(&(state->ph_disp_st));

  if (mode != MRDTX)
     dtx_dec_reset(&(state->dtxDecoderState));

  return 0;

}

/*
**************************************************************************
*
*  Function    : Decoder_amr
*  Purpose     : Speech decoder routine.
*
**************************************************************************
*/
int Decoder_amr (
    AMR_Decode_FrameState *Dec,
    Decoder_amrState *st,      /* i/o : State variables                   */
    enum Mode mode,            /* i   : AMR mode                          */
    Word16 parm[],             /* i   : vector of synthesis parameters
                                        (PRM_SIZE)                        */
    enum RXFrameType frame_type, /* i   : received frame type             */
    Word16 synth[],            /* o   : synthesis speech (L_FRAME)        */
    Word16 A_t[]               /* o   : decoded LP filter in 4 subframes
                                        (AZ_SIZE)                         */
)
{
	AMR_Shared_Buf *Dec_share = Dec->share;

   Word32 L_temp;
   Word32 temp;
  	Word32 ovf_coef;

   Word16 i;

   /* LPC coefficients */
   Word16 *Az;                /* Pointer on A_t */

#if 0
   /* LSPs */
   Word16 lsp_new[M];
   Word16 lsp_mid[M];
   /* LSFs */
   Word16 prev_lsf[M];
   Word16 lsf_i[M];
   /* Algebraic codevector */
   Word16 code[L_SUBFR];
   /* excitation */
   Word16 excp[L_SUBFR];
   Word16 exc_enhanced[L_SUBFR];
#endif
   /* LSPs */
   Word16 *lsp_new = Dec_share->shabuf.stD.u0D.lsp_new;
   Word16 *lsp_mid = Dec_share->shabuf.stD.u1D.lsp_mid;
   /* LSFs */
   Word16 *prev_lsf = Dec_share->shabuf.stD.prev_lsf;
   Word16 *lsf_i = Dec_share->shabuf.stD.u0D.lsf_i;
   /* Algebraic codevector */
   Word16 *code = Dec_share->shabuf.stD.code;
   /* excitation */
   Word16 *excp = Dec_share->shabuf.stD.u1D.excp;
   Word16 *exc_enhanced = Dec_share->shabuf.stD.u0D.exc_enhanced;



   /* Scalars */
   Word16 i_subfr;
   Word16 T0, T0_frac, index, index_mr475 = 0;
   Word16 gain_pit, gain_code, gain_code_mix, pit_sharp, pit_flag, pitch_fac;
   Word16 t0_min, t0_max;
   Word16 delta_frc_low, delta_frc_range;
   Word16 tmp_shift;
   Word16 flag4;
   Word16 carefulFlag;
   Word16 excEnergy;
   Word16 subfrNr;
   Word16 evenSubfr = 0;

   Word16 bfi = 0;   /* bad frame indication flag                          */
   Word16 pdfi = 0;  /* potential degraded bad frame flag                  */

   enum DTXStateType newDTXState;  /* SPEECH , DTX, DTX_MUTE */

   /* find the new  DTX state  SPEECH OR DTX */
   newDTXState = rx_dtx_handler(&(st->dtxDecoderState), frame_type);


   /* DTX actions */
   if(newDTXState != SPEECH)
   {
      Decoder_amr_reset (st, MRDTX);

      dtx_dec(Dec,
			&(st->dtxDecoderState),
         st->mem_syn,
         &(st->lsfState),
         &(st->pred_state),
         &(st->Cb_gain_averState),
         newDTXState,
         mode,
         parm, synth, A_t);
      /* update average lsp */

      Lsf_lsp(st->lsfState.past_lsf_q, st->lsp_old);
      lsp_avg(&(st->lsp_avg_st), st->lsfState.past_lsf_q);

//      goto the_end;
		st->dtxDecoderState.dtxGlobalState = newDTXState;
   	return 0;
   }

   /* SPEECH action state machine  */
   if ( (frame_type == RX_SPEECH_BAD) ||
        (frame_type == RX_NO_DATA) ||
        (frame_type == RX_ONSET) )
   {
      bfi = 1;
      if ((frame_type == RX_NO_DATA) || (frame_type == RX_ONSET) )
      {
         build_CN_param(&st->nodataSeed, prmno[mode], bitno[mode], parm);
      }
   }
   else if(frame_type == RX_SPEECH_DEGRADED)
   {
      pdfi = 1;
   }

   if (bfi != 0)
   {
      st->state = (st->state + 1);
   }
   else if(st->state == 6)
   {
      st->state = 5;
   }
   else
   {
      st->state = 0;
   }

   if(st->state > 6)
   {
      st->state = 6;
   }

   /* If this frame is the first speech frame after CNI period,     */
   /* set the BFH state machine to an appropriate state depending   */
   /* on whether there was DTX muting before start of speech or not */
   /* If there was DTX muting, the first speech frame is muted.     */
   /* If there was no DTX muting, the first speech frame is not     */
   /* muted. The BFH state machine starts from state 5, however, to */
   /* keep the audible noise resulting from a SID frame which is    */
   /* erroneously interpreted as a good speech frame as small as    */
   /* possible (the decoder output in this case is quickly muted)   */
   if(st->dtxDecoderState.dtxGlobalState == DTX)
   {
      st->state = 5;
      st->prev_bf = 0;
   }
   else if(st->dtxDecoderState.dtxGlobalState == DTX_MUTE)
   {
      st->state = 5;
      st->prev_bf = 1;
   }

   /* save old LSFs for CB gain smoothing */
   Copy (st->lsfState.past_lsf_q, prev_lsf, M);

   /* decode LSF parameters and generate interpolated lpc coefficients
   for the 4 subframes */

   if(mode != MR122)
   {
      D_plsf_3(Dec_share->shabuf.stD.u2D.s0D.lsf1_q , &(st->lsfState), mode, bfi, parm, lsp_new);

      /* Advance synthesis parameters pointer */
      parm += 3;

      Int_lpc_1to3(Dec_share->shabuf.stD.f1 ,Dec_share->shabuf.stD.f2 ,
			Dec_share->shabuf.stD.u2D.s0D.lsp , st->lsp_old, lsp_new, A_t);
   }
   else
   {
      D_plsf_5(Dec , &(st->lsfState), bfi, parm, lsp_mid, lsp_new);

      /* Advance synthesis parameters pointer */
      parm += 5;
      Int_lpc_1and3 (Dec_share->shabuf.stD.f1 ,Dec_share->shabuf.stD.f2 ,
			Dec_share->shabuf.stD.u2D.s0D.lsp , st->lsp_old, lsp_mid, lsp_new, A_t);
   }

   /* update the LSPs for the next frame */
   {
   	Word16 *sptro,*sptrn;

		sptro = st->lsp_old;
		sptrn = lsp_new;
		i = M/2;
		do
		{
			*sptro++ = *sptrn++;
			*sptro++ = *sptrn++;
		}while(--i != 0);
   }
   /*------------------------------------------------------------------------*
   *          Loop for every subframe in the analysis frame                 *
   *------------------------------------------------------------------------*
   * The subframe size is L_SUBFR and the loop is repeated L_FRAME/L_SUBFR  *
   *  times                                                                 *
   *     - decode the pitch delay                                           *
   *     - decode algebraic code                                            *
   *     - decode pitch and codebook gains                                  *
   *     - find the excitation and compute synthesis speech                 *
   *------------------------------------------------------------------------*/
   /* pointer to interpolated LPC parameters */
   Az = A_t;

   evenSubfr = 0;
   subfrNr = -1;
   for (i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR)
   {
      subfrNr = (subfrNr + 1);
      evenSubfr = (1 - evenSubfr);

      /* flag for first and 3th subframe */
      pit_flag = i_subfr;

      if(i_subfr == L_FRAME_BY2)
      {
         if ( (mode != MR475) && (mode != MR515) )
         {
            pit_flag = 0;
         }
      }

      /* pitch index */
      index = *parm++;

      /*-------------------------------------------------------*
      * - decode pitch lag and find adaptive codebook vector. *
      *-------------------------------------------------------*/
      if(mode != MR122)
      {
         /* flag4 indicates encoding with 4 bit resolution;     */
         /* this is needed for mode MR475, MR515, MR59 and MR67 */
         flag4 = 0;

         if (  (mode == MR475) ||
               (mode == MR515) ||
               (mode == MR59)  ||
               (mode == MR67)  )
         {
            flag4 = 1;
         }

         /*-------------------------------------------------------*
         * - get ranges for the t0_min and t0_max                *
         * - only needed in delta decoding                       *
         *-------------------------------------------------------*/
         delta_frc_low = 5;
         delta_frc_range = 9;

         if(mode == MR795)
         {
            delta_frc_low = 10;
            delta_frc_range = 19;
         }

         t0_min = (st->old_T0 - delta_frc_low);

         if(t0_min < PIT_MIN)
         {
            t0_min = PIT_MIN;
         }
         t0_max = (t0_min + delta_frc_range);

         if(t0_max > PIT_MAX)
         {
            t0_max = PIT_MAX;
            t0_min = (t0_max - delta_frc_range);
         }

         Dec_lag3 (index, t0_min, t0_max, pit_flag, st->old_T0, &T0, &T0_frac, flag4);
         st->T0_lagBuff = T0;

         if (bfi != 0)
         {
            if(st->old_T0 < PIT_MAX)
            {                                      /* Graceful pitch */
               st->old_T0 = (st->old_T0 + 1);    /* degradation    */
            }
            T0 = st->old_T0;
            T0_frac = 0;

            if ( st->inBackgroundNoise != 0  && 
                 (st->voicedHangover > 4) &&
               ( (mode == MR475)  ||
                 (mode == MR515)  ||
                 (mode == MR59)  )
               )
            {
               T0 = st->T0_lagBuff;
            }
         }
         Pred_lt_3or6 (st->exc, T0, T0_frac, 1);
      }
      else
      {
         Dec_lag6 (index, PIT_MIN_MR122, PIT_MAX, pit_flag, &T0, &T0_frac);

         if ( bfi == 0 && (pit_flag == 0 ||  (index < 61) ) )
         {
         }
         else
         {
            st->T0_lagBuff = T0;
            T0 = st->old_T0;
            T0_frac = 0;
         }

         Pred_lt_3or6 (st->exc, T0, T0_frac, 0);
      }

      /*-------------------------------------------------------*
      * - (MR122 only: Decode pitch gain.)                    *
      * - Decode innovative codebook.                         *
      * - set pitch sharpening factor                         *
      *-------------------------------------------------------*/
      if((mode == MR475) || (mode == MR515))
      {   /* MR475, MR515 */
         index = *parm++;        /* index of position */
         i = *parm++;            /* signs             */
         decode_2i40_9bits (Dec, subfrNr, i, index, code);
         pit_sharp = (st->sharp << 1);
      }
      else if(mode == MR59)
      {   /* MR59 */

         index = *parm++;        /* index of position */
         i = *parm++;            /* signs             */
         decode_2i40_11bits (Dec, i, index, code);
         pit_sharp = (st->sharp << 1);
      }
      else if(mode == MR67)
      {   /* MR67 */
         index = *parm++;        /* index of position */
         i = *parm++;            /* signs             */
         decode_3i40_14bits(Dec, i, index, code);
         pit_sharp = (st->sharp << 1);
      }
      else if(mode <= MR795)
      {   /* MR74, MR795 */

         index = *parm++;        /* index of position */
         i = *parm++;            /* signs             */
         decode_4i40_17bits (Dec, i, index, code);
         pit_sharp = (st->sharp << 1);
      }
      else if(mode == MR102)
      {  /* MR102 */
         dec_8i40_31bits (Dec, parm, code);
         parm += 7;
         pit_sharp = (st->sharp << 1);
      }
      else
      {  /* MR122 */
         index = *parm++;
         if (bfi != 0)
         {
            ec_gain_pitch(&(st->ec_gain_p_st), st->state, &gain_pit);
         }
         else
         {
            gain_pit = d_gain_pitch(mode, index);
         }
         ec_gain_pitch_update(&(st->ec_gain_p_st), bfi, st->prev_bf, &gain_pit);
         dec_10i40_35bits (parm, code);
         parm += 10;
         /* pit_sharp = gain_pit;                   */
         /* if (pit_sharp > 1.0) pit_sharp = 1.0;   */
         pit_sharp = sature(gain_pit << 1);
      }

      /*-------------------------------------------------------*
      * - Add the pitch contribution to code[].               *
      *-------------------------------------------------------*/
      {
   	   Word16 *sptr = (Word16*)code + T0;
   	   Word16 *sptro = (Word16*)code;
			for (i = L_SUBFR-T0; i > 0; i--)
   	   {
         	*sptr = ( *sptr + (smulbb(*sptro, pit_sharp) >> 15) );
				sptr++;
				sptro++;
	      }
      }

      /*------------------------------------------------------------*
      * - Decode codebook gain (MR122) or both pitch               *
      *   gain and codebook gain (all others)                      *
      * - Update pitch sharpening "sharp" with quantized gain_pit  *
      *------------------------------------------------------------*/

      if(mode == MR475)
      {
         /* read and decode pitch and code gain */
         if (evenSubfr != 0)
         {
            index_mr475 = *parm++;         /* index of gain(s) */
         }

         if (bfi == 0)
         {
            Dec_gain(&(st->pred_state), mode, index_mr475, code,
               evenSubfr, &gain_pit, &gain_code);
         }
         else
         {
            ec_gain_pitch (&(st->ec_gain_p_st), st->state, &gain_pit);
            ec_gain_code (&(st->ec_gain_c_st), &(st->pred_state), st->state, &gain_code);
         }
         ec_gain_pitch_update (&(st->ec_gain_p_st), bfi, st->prev_bf, &gain_pit);
         ec_gain_code_update (&(st->ec_gain_c_st), bfi, st->prev_bf, &gain_code);

         pit_sharp = gain_pit;

         if(pit_sharp > SHARPMAX)
         {
            pit_sharp = SHARPMAX;
         }

      }
      else if ( (mode <= MR74) || (mode == MR102) )
      {
         /* read and decode pitch and code gain */
         index = *parm++;                 /* index of gain(s) */

         if (bfi == 0)
         {
            Dec_gain(&(st->pred_state), mode, index, code,
               evenSubfr, &gain_pit, &gain_code);
         }
         else
         {
            ec_gain_pitch (&(st->ec_gain_p_st), st->state, &gain_pit);
            ec_gain_code (&(st->ec_gain_c_st), &(st->pred_state), st->state, &gain_code);
         }
         ec_gain_pitch_update (&(st->ec_gain_p_st), bfi, st->prev_bf, &gain_pit);
         ec_gain_code_update (&(st->ec_gain_c_st), bfi, st->prev_bf, &gain_code);

         pit_sharp = gain_pit;

         if(pit_sharp > SHARPMAX)
         {
            pit_sharp = SHARPMAX;
         }

         if(mode == MR102)
         {
            if(st->old_T0 > (L_SUBFR + 5))
            {
               pit_sharp = (pit_sharp >> 2);
            }
         }
      }
      else
      {
         /* read and decode pitch gain */
         index = *parm++;                /* index of gain(s) */

         if(mode == MR795)
         {
            /* decode pitch gain */
            if (bfi != 0)
            {
               ec_gain_pitch (&(st->ec_gain_p_st), st->state, &gain_pit);
            }
            else
            {
               gain_pit = d_gain_pitch(mode, index);
            }
            ec_gain_pitch_update (&(st->ec_gain_p_st), bfi, st->prev_bf, &gain_pit);

            /* read and decode code gain */
            index = *parm++;

            if (bfi == 0)
            {
               d_gain_code (&(st->pred_state), mode, index, code, &gain_code);
            }
            else
            {
               ec_gain_code (&(st->ec_gain_c_st), &(st->pred_state), st->state,&gain_code);
            }
            ec_gain_code_update (&(st->ec_gain_c_st), bfi, st->prev_bf, &gain_code);


            pit_sharp = gain_pit;

            if(pit_sharp > SHARPMAX)
            {
               pit_sharp = SHARPMAX;
            }
         }
         else
         { /* MR122 */

				if (bfi == 0)
            {
               d_gain_code (&(st->pred_state), mode, index, code, &gain_code);
            }
            else
            {
               ec_gain_code (&(st->ec_gain_c_st), &(st->pred_state), st->state,&gain_code);
            }
            ec_gain_code_update (&(st->ec_gain_c_st), bfi, st->prev_bf,&gain_code);

            pit_sharp = gain_pit;
         }

      }

      /* store pitch sharpening for next subframe          */
      /* (for modes which use the previous pitch gain for
      pitch sharpening in the search phase)             */
      /* do not update sharpening in even subframes for MR475 */

      if ( (mode != MR475) || (evenSubfr == 0) )
      {
         st->sharp = gain_pit;

         if(st->sharp > SHARPMAX)
         {
            st->sharp = SHARPMAX;
         }
      }

      pit_sharp = sature(pit_sharp << 1);

      if(pit_sharp > 16384)
      {
			Word32 *sptr_st,t;
			Word16 *sptr_excp;

			sptr_st = (Word32 *)st->exc;
			sptr_excp = excp;

         //for (i = 0; i < L_SUBFR; i++)
			i = L_SUBFR/2;
			do
         {
         	t = *sptr_st++;
            temp = smulbb(t, pit_sharp) >> 15;
            L_temp = smulbb(temp, gain_pit);
            L_temp = qadd(L_temp,L_temp);
            if(mode == MR122)
            {
               L_temp = (L_temp >> 1);
            }
            *sptr_excp++ = (Word16)(qadd(L_temp,0x8000) >> 16);

            temp = smultb(t, pit_sharp) >> 15;
            L_temp = smulbb(temp, gain_pit);
            L_temp = qadd(L_temp,L_temp);
            if(mode == MR122)
            {
               L_temp = (L_temp >> 1);
            }
            *sptr_excp++ = (Word16)(qadd(L_temp,0x8000) >> 16);

         }while(--i != 0);

      }

      /*-------------------------------------------------------*
      * - Store list of LTP gains needed in the source        *
      *   characteristic detector (SCD)                       *
      *-------------------------------------------------------*/

      if ( bfi == 0 )
      {
         st->ltpGainHistory[0] = st->ltpGainHistory[1];
         st->ltpGainHistory[1] = st->ltpGainHistory[2];
         st->ltpGainHistory[2] = st->ltpGainHistory[3];
         st->ltpGainHistory[3] = st->ltpGainHistory[4];
         st->ltpGainHistory[4] = st->ltpGainHistory[5];
         st->ltpGainHistory[5] = st->ltpGainHistory[6];
         st->ltpGainHistory[6] = st->ltpGainHistory[7];
         st->ltpGainHistory[7] = st->ltpGainHistory[8];

         st->ltpGainHistory[8] = gain_pit;
      }

      /*-------------------------------------------------------*
      * - Limit gain_pit if in background noise and BFI       *
      *   for MR475, MR515, MR59                              *
      *-------------------------------------------------------*/


      if ( ( (st->prev_bf != 0) || (bfi != 0) ) && (st->inBackgroundNoise != 0) &&
          ( (mode == MR475) ||
            (mode == MR515)  ||
            (mode == MR59)   )
         )
      {
         if(gain_pit > 12288)     /* if (gain_pit > 0.75) in Q14*/
            gain_pit = ( ( (gain_pit - 12288) >> 1 ) + 12288 );
         /* gain_pit = (gain_pit-0.75)/2.0 + 0.75; */

         if(gain_pit > 14745)     /* if (gain_pit > 0.90) in Q14*/
         {
            gain_pit = 14745;
         }
      }

      /*-------------------------------------------------------*
      *  Calculate CB mixed gain                              *
      *-------------------------------------------------------*/
      Int_lsf(prev_lsf, st->lsfState.past_lsf_q, i_subfr, lsf_i);
      gain_code_mix = Cb_gain_average(Dec ,
         &(st->Cb_gain_averState), mode, gain_code,
         lsf_i, st->lsp_avg_st.lsp_meanSave, bfi,
         st->prev_bf, pdfi, st->prev_pdf,
         st->inBackgroundNoise, st->voicedHangover);         

      /* make sure that MR74, MR795, MR122 have original code_gain*/

      if ( (mode > MR67)  && (mode != MR102)  )
         /* MR74, MR795, MR122 */
      {
         gain_code_mix = gain_code;
      }

      /*-------------------------------------------------------*
      * - Find the total excitation.                          *
      * - Find synthesis speech corresponding to st->exc[].   *
      *-------------------------------------------------------*/
      if(mode <= MR102)  /* MR475, MR515, MR59, MR67, MR74, MR795, MR102*/
      {
         pitch_fac = gain_pit;
         tmp_shift = 1;
      }
      else       /* MR122 */
      {
         pitch_fac = (gain_pit >> 1);
         tmp_shift = 2;
      }

      /* copy unscaled LTP excitation to exc_enhanced (used in phase
      * dispersion below) and compute total excitation for LTP feedback
      */
     	{
			Word16 *sptrenh,*sptrexc,*sptrcod;

			sptrenh  = exc_enhanced;
			sptrexc = st->exc;
			sptrcod = code;

			i = L_SUBFR;
			do
   	   {
      	   *sptrenh++ = *sptrexc;

	         /* st->exc[i] = gain_pit*st->exc[i] + gain_code*code[i]; */
   	      L_temp = smulbb(*sptrexc, pitch_fac);
				L_temp = qadd(L_temp,L_temp);

	         /* 12.2: Q0 * Q13 */
   	      /*  7.4: Q0 * Q14 */
      	   L_temp = qdadd(L_temp,smulbb(*sptrcod++, gain_code));

	         /* 12.2: Q12 * Q1 */
   	      /*  7.4: Q13 * Q1 */
      	   L_temp = L_shl(L_temp, tmp_shift);                   /* Q16 */
         	*sptrexc++ = (Word16)(qadd(L_temp,0x8000)>>16);
	      }while(--i != 0);

      }
      /*-------------------------------------------------------*
      * - Adaptive phase dispersion                           *
      *-------------------------------------------------------*/
      ph_disp_release(&(st->ph_disp_st)); /* free phase dispersion adaption */

      if ( ( (mode == MR475)  ||
              (mode == MR515)  ||
              (mode == MR59)  )     &&
          (st->voicedHangover > 3)  &&
         (st->inBackgroundNoise != 0) &&
         (bfi != 0) )
      {
         ph_disp_lock(&(st->ph_disp_st)); /* Always Use full Phase Disp. */
      }                                /* if error in bg noise       */

                                       /* apply phase dispersion to innovation (if enabled) and
      compute total excitation for synthesis part           */
      ph_disp(Dec , &(st->ph_disp_st), mode,
         exc_enhanced, gain_code_mix, gain_pit, code,
         pitch_fac, tmp_shift);

         /*-------------------------------------------------------*
         * - The Excitation control module are active during BFI.*
         * - Conceal drops in signal energy if in bg noise.      *
      *-------------------------------------------------------*/
      {
      	Word32 *lptr,t;

			lptr = (Word32 *)exc_enhanced;

	      L_temp = 0;
			i = L_SUBFR/4;
			do
      	{
      		t = *lptr++;
         	L_temp = qdadd(L_temp,smulbb( t,t ) );
	         L_temp = qdadd(L_temp,smultt( t,t ) );
      		t = *lptr++;
         	L_temp = qdadd(L_temp,smulbb( t,t ) );
	         L_temp = qdadd(L_temp,smultt( t,t ) );

   	   }while(--i != 0);
      }

      L_temp = (L_temp >> 1);     /* excEnergy = sqrt(L_temp) in Q0 */
      L_temp = sqrt_l_exp(L_temp, (Word16 *)&temp);
      L_temp = L_shr(L_temp, ( (temp >> 1) + 15));
      L_temp = (L_temp >> 2);       /* To cope with 16-bit and  */
      excEnergy = (Word16)(L_temp);   /* scaling in ex_ctrl()     */

      if ( (  (mode == MR475)    ||
               (mode == MR515)   ||
               (mode == MR59)  )  &&
              (st->voicedHangover > 5)   &&
              (st->inBackgroundNoise != 0) &&
              (st->state < 4)  &&
         ( (pdfi != 0 && st->prev_pdf != 0) ||
            (bfi != 0) ||
            (st->prev_bf != 0) ) )
      {
         carefulFlag = 0;

         if ( pdfi != 0 && bfi == 0 )
         {
            carefulFlag = 1;
         }

         Ex_ctrl(exc_enhanced,
            excEnergy,
            st->excEnergyHist,
            st->voicedHangover,
            st->prev_bf,
            carefulFlag);
      }


      if ( st->inBackgroundNoise != 0 &&
         ( bfi != 0 || st->prev_bf != 0 ) &&
         (st->state < 4) )
      {
         ; /* do nothing! */
      }
      else
      {
         /* Update energy history for all modes */
         st->excEnergyHist[0] = st->excEnergyHist[1];
         st->excEnergyHist[1] = st->excEnergyHist[2];
         st->excEnergyHist[2] = st->excEnergyHist[3];
         st->excEnergyHist[3] = st->excEnergyHist[4];
         st->excEnergyHist[4] = st->excEnergyHist[5];
         st->excEnergyHist[5] = st->excEnergyHist[6];
         st->excEnergyHist[6] = st->excEnergyHist[7];
         st->excEnergyHist[7] = st->excEnergyHist[8];

         st->excEnergyHist[8] = excEnergy;
      }
      /*-------------------------------------------------------*
      * Excitation control module end.                        *
      *-------------------------------------------------------*/
      if(pit_sharp > 16384)
      {
			Word16 *sptr_excp,*sptr_exce;

			sptr_exce = exc_enhanced;
			sptr_excp =	excp;
			i = L_SUBFR/2;
			do
			{
            *sptr_excp = *sptr_excp + *sptr_exce;     sptr_excp++; sptr_exce++;
            *sptr_excp = *sptr_excp + *sptr_exce;     sptr_excp++; sptr_exce++;

			}while(--i != 0);

         agc2(exc_enhanced, excp);

         _check_overflow_start();

         Syn_filt (Dec_share->shabuf.stD.u2D.s3D.tmp_w ,
				Az, excp, &synth[i_subfr], L_SUBFR, st->mem_syn, 0);

         ovf_coef = _check_overflow_end();
      }
      else
      {
         _check_overflow_start();

         Syn_filt (Dec_share->shabuf.stD.u2D.s3D.tmp_w ,
				Az, exc_enhanced, &synth[i_subfr], L_SUBFR, st->mem_syn, 0);

         ovf_coef = _check_overflow_end();
      }

      if (ovf_coef != 0)    /* Test for overflow */
      {
			Word16 *sptr;

			sptr = st->old_exc;
			i = (PIT_MAX + L_INTERPOL + L_SUBFR)/2;
			do
			{
            *sptr = (*sptr >> 2);     sptr++;
            *sptr = (*sptr >> 2);     sptr++;
			}while(--i != 0);


			sptr = exc_enhanced;
			i = L_SUBFR/2;
			do
			{
            *sptr = (*sptr >> 2);     sptr++;
            *sptr = (*sptr >> 2);     sptr++;

			}while(--i != 0);

         Syn_filt(Dec_share->shabuf.stD.u2D.s3D.tmp_w ,
				Az, exc_enhanced, &synth[i_subfr], L_SUBFR, st->mem_syn, 1);

      }
      else
      {
         Copy(&synth[i_subfr+L_SUBFR-M], st->mem_syn, M);
      }

      /*--------------------------------------------------*
      * Update signal for next frame.                    *
      * -> shift to the left by L_SUBFR  st->exc[]       *
      *--------------------------------------------------*/

//      Copy (&st->old_exc[L_SUBFR], &st->old_exc[0], PIT_MAX + L_INTERPOL);
      AMR_Copy32((Word32 *)&st->old_exc[L_SUBFR], (Word32 *)&st->old_exc[0], (PIT_MAX + L_INTERPOL)/2);

      /* interpolated LPC parameters for next subframe */
      Az += MP1+1;

      /* store T0 for next subframe */
      st->old_T0 = T0;
    }

    /*-------------------------------------------------------*
    * Call the Source Characteristic Detector which updates *
    * st->inBackgroundNoise and st->voicedHangover.         *
    *-------------------------------------------------------*/

    st->inBackgroundNoise = Bgn_scd(&(st->background_state),
       &(st->ltpGainHistory[0]),
       &(synth[0]),
       &(st->voicedHangover) );

    dtx_dec_activity_update(&(st->dtxDecoderState),
       st->lsfState.past_lsf_q,
       synth);


    /* store bfi for next subframe */
    st->prev_bf = bfi;
    st->prev_pdf = pdfi;

    /*--------------------------------------------------*
    * Calculate the LSF averages on the eight          *
    * previous frames                                  *
    *--------------------------------------------------*/

    lsp_avg(&(st->lsp_avg_st), st->lsfState.past_lsf_q);

//the_end:
    st->dtxDecoderState.dtxGlobalState = newDTXState;

    return 0;
}

#endif

