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
 *   cod_amr.c
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
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : cod_amr.c
*      Purpose          : Main encoder routine operating on a frame basis.
*
*****************************************************************************
*/
#include "cod_amr.h"


/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/


#include "typedef.h"
#include "basic_op.h"
#include "copy.h"
#include "set_zero.h"


#include "lpc.h"
#include "lsp.h"
#include "pre_big.h"
#include "ol_ltp.h"
#include "p_ol_wgh.h"
#include "spreproc.h"
#include "cl_ltp.h"
#include "pred_lt.h"
#include "spstproc.h"
#include "cbsearch.h"
#include "gain_q.h"
#include "copy.h"
#include "convolve.h"
#include "ton_stab.h"
#include "vad.h"
#include "dtx_enc.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/

/*
*****************************************************************************
*                         PUBLIC VARIABLES AND TABLES
*****************************************************************************
*/
/* Spectral expansion factors */

__align(4)
static const Word16 gamma1[M] =
{
   30802, 28954, 27217, 25584, 24049,
   22606, 21250, 19975, 18777, 17650
};

/* gamma1 differs for the 12k2 coder */

static const Word16 gamma1_12k2[M] =
{
    29491, 26542, 23888, 21499, 19349,
    17414, 15672, 14105, 12694, 11425
};


static const Word16 gamma2[M] =
{
   19661, 11797, 7078, 4247, 2548,
   1529, 917, 550, 330, 198
};

/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/

/*
**************************************************************************
*
*  Function    : cod_amr_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int cod_amr_reset (cod_amrState *st)
{
   /*-----------------------------------------------------------------------*
    *          Initialize pointers to speech vector.                        *
    *-----------------------------------------------------------------------*/
   st->new_speech = st->old_speech + L_TOTAL - L_FRAME;   /* New speech     */

   st->speech = st->new_speech - L_NEXT;                  /* Present frame  */

   st->p_window = st->old_speech + L_TOTAL - L_WINDOW;    /* For LPC window */
   st->p_window_12k2 = st->p_window - L_NEXT; /* EFR LPC window: no lookahead */

   /* Initialize static pointers */
   st->wsp = st->old_wsp + PIT_MAX + 1;
   st->exc = st->old_exc + PIT_MAX + L_INTERPOL;
   st->zero = st->ai_zero + MP1;
   st->error = st->mem_err + M;
   st->h1 = &st->hvec[L_SUBFR];

   /* Static vectors to zero */
   Set_zero(st->old_speech, L_TOTAL);
   Set_zero(st->old_exc,    PIT_MAX + L_INTERPOL);
   Set_zero(st->old_wsp,    PIT_MAX+1);
   Set_zero(st->mem_syn,    M);
   Set_zero(st->mem_w,      M);
   Set_zero(st->mem_w0,     M);
   Set_zero(st->mem_err,    M);
   Set_zero(st->zero,       L_SUBFR);
   Set_zero(st->hvec,       L_SUBFR);    /* set to zero "h1[-L_SUBFR..-1]" */

   /* OL LTP states */
	st->old_lags[0] = 40;
	st->old_lags[1] = 40;
	st->old_lags[2] = 40;
	st->old_lags[3] = 40;
	st->old_lags[4] = 40;

   /* Reset lpc states */
   lpc_reset(st->old_A);
   /* Reset lsp states */
   lsp_reset(st);

   /* Reset clLtp states */
	Pitch_fr_reset(st);
   gainQuant_reset(&(st->gainQuantSt));
   p_ol_wgh_reset(&(st->pitchOLWghtSt));
   ton_stab_reset(&(st->tonStabSt));

   vad1_reset(&(st->vadSt));

   dtx_enc_reset(&(st->dtx_encSt));
   st->sharp = SHARPMIN;
   return 0;
}


/***************************************************************************
 *   FUNCTION: cod_amr
 *
 *   PURPOSE:  Main encoder routine.
 *
 *   DESCRIPTION: This function is called every 20 ms speech frame,
 *       operating on the newly read 160 speech samples. It performs the
 *       principle encoding functions to produce the set of encoded parameters
 *       which include the LSP, adaptive codebook, and fixed codebook
 *       quantization indices (addresses and gains).
 *
 *   INPUTS:
 *       No input argument are passed to this function. However, before
 *       calling this function, 160 new speech data should be copied to the
 *       vector new_speech[]. This is a global pointer which is declared in
 *       this file (it points to the end of speech buffer minus 160).
 *
 *   OUTPUTS:
 *
 *       ana[]:     vector of analysis parameters.
 *       synth[]:   Local synthesis speech (for debugging purposes)
 *
 ***************************************************************************/
int cod_amr(
    AMR_Encode_FrameState *Enc,
    cod_amrState *st,          /* i/o : State struct                   */
    enum Mode mode,            /* i   : AMR mode                       */
    Word16 new_speech[],       /* i   : speech input (L_FRAME)         */
    Word16 ana[],              /* o   : Analysis parameters            */
    enum Mode *usedMode,       /* o   : used mode                    */
    Word16 synth[]             /* o   : Local synthesis                */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;

   Word16 *A, *Aq;             /* Pointer on A_t and Aq_t              */
   /* Scalars */
   Word16 i_subfr, subfrNr;
   Word16 T0, T0_frac;
   Word16 gain_pit, gain_code;
   /* Flags */
   Word16 lsp_flag = 0;        /* indicates resonance in LPC filter  */
   Word16 gp_limit;            /* pitch gain limit value             */
   Word16 vad_flag;            /* VAD decision flag                  */
   Word16 compute_sid_flag;    /* SID analysis  flag                 */
   Word16 T_op[2];

	//Word16 testLPC[(MP1)];
	//Word16 kk;

   /* LPC coefficients */
   Word16 *A_t     = Enc_share->shabuf.stE.A_t;
   Word16 *Aq_t    = Enc_share->shabuf.stE.Aq_t;
   Word16 *lsp_new;
   /* Other vectors */
   Word16 *xn     ;
   Word16 *xn2    ;
   Word16 *code   ;
   Word16 *y1     ;
   Word16 *y2     ;
   Word16 *gCoeff ;
   Word16 *res    ;
   Word16 *res2   ;
	/* Vector and scalars needed for the MR475 */
   Word16 *xn_sf0       ;
   Word16 *y2_sf0       ;
   Word16 *code_sf0     ;
   Word16 *h1_sf0       ;
   Word16 *mem_syn_save ;
   Word16 *mem_w0_save  ;
   Word16 *mem_err_save ;

	Word16 sharp_save;          /* Sharpening                           */
   Word16 evenSubfr;           /* Even subframe indicator              */
   Word16 T0_sf0 = 0;          /* Integer pitch lag of sf0             */
   Word16 T0_frac_sf0 = 0;     /* Fractional pitch lag of sf0          */
   Word16 i_subfr_sf0 = 0;     /* Position in exc[] for sf0            */
   Word16 gain_pit_sf0;        /* Quantized pitch gain for sf0         */
   Word16 gain_code_sf0;       /* Quantized codebook gain for sf0      */

   AMR_Copy32((Word32 *)new_speech, (Word32 *)st->new_speech, L_FRAME/2);

   *usedMode = mode;

   /* DTX processing */
   if (st->dtx)
   {  /* no test() call since this if is only in simulation env */
      /* Find VAD decision */
      vad_flag = vad1(Enc , &(st->vadSt), st->new_speech);
      /* NB! usedMode may change here */
      compute_sid_flag = tx_dtx_handler(&(st->dtx_encSt),
                                        vad_flag,
                                        usedMode);
   }
   else
   {
		compute_sid_flag = 0;
   }

   /*------------------------------------------------------------------------*
    *  - Perform LPC analysis:                                               *
    *       * autocorrelation + lag windowing                                *
    *       * Levinson-durbin algorithm to find a[]                          *
    *       * convert a[] to lsp[]                                           *
    *       * quantize and code the LSPs                                     *
    *       * find the interpolated LSPs and convert to a[] for all          *
    *         subframes (both quantized and unquantized)                     *
    *------------------------------------------------------------------------*/

   lsp_new = Enc_share->shabuf.stE.u0E.lsp_new;

   /* LP analysis */
   lpc(Enc , st->old_A , mode, st->p_window, st->p_window_12k2, A_t);

	//{
		//Word16 *ptr16;
		//ptr16 = &A_t[(MP1+1) * 3];

		//for(kk=0 ; kk<=10 ; kk++)
		//{
		//	testLPC[kk] = *ptr16++;
		//}
	//}

   /* From A(z) to lsp. LSP quantization and interpolation */
   lsp(Enc , st, mode, *usedMode, A_t, Aq_t, lsp_new, &ana);

   /* Buffer lsp's and energy */
   dtx_buffer(&(st->dtx_encSt), lsp_new, st->new_speech);

   /* Check if in DTX mode */
   if (*usedMode == MRDTX)
   {
      dtx_enc( Enc,
					&(st->dtx_encSt),
               compute_sid_flag,
               st->past_rq,
               &(st->gainQuantSt.gc_predSt),
               &ana);

      Set_zero(st->old_exc,    PIT_MAX + L_INTERPOL);
      Set_zero(st->mem_w0,     M);
      Set_zero(st->mem_err,    M);
      Set_zero(st->zero,       L_SUBFR);
      Set_zero(st->hvec,       L_SUBFR);    /* set to zero "h1[-L_SUBFR..-1]" */
      /* Reset lsp states */
      lsp_reset(st);

      Copy(lsp_new, st->lsp_old, M);
      Copy(lsp_new, st->lsp_old_q, M);

      /* Reset clLtp states */
		Pitch_fr_reset(st);

      st->sharp = SHARPMIN;
   }
   else
   {
       /* check resonance in the filter */
      lsp_flag = check_lsp(&(st->tonStabSt), st->lsp_old);
   }
   /*----------------------------------------------------------------------*
    * - Find the weighted input speech w_sp[] for the whole speech frame   *
    * - Find the open-loop pitch delay for first 2 subframes               *
    * - Set the range for searching closed-loop pitch in 1st subframe      *
    * - Find the open-loop pitch delay for last 2 subframes                *
    *----------------------------------------------------------------------*/
   for(subfrNr = 0, i_subfr = 0;
       subfrNr < 2;
       subfrNr++, i_subfr += L_FRAME_BY2)
   {
      /* Pre-processing on 80 samples */
      pre_big(Enc ,
      			mode,
      			gamma1,
      			gamma1_12k2,
      			gamma2,
      			A_t,
      			i_subfr,
      			st->speech,
      			st->mem_w,
      			st->wsp);

      if ( (mode != MR475) && (mode != MR515))
      {
         /* Find open loop pitch lag for two subframes */
         ol_ltp(Enc ,
         		&(st->pitchOLWghtSt),
         		&(st->vadSt),
         		mode,
         		&st->wsp[i_subfr],
               &T_op[subfrNr],
               st->old_lags,
               st->ol_gain_flg,
               subfrNr,
               st->dtx);
      }
   }

   if ( (mode==MR475) || (mode==MR515) )
   {
      /* Find open loop pitch lag for ONE FRAME ONLY */
      /* search on 160 samples */

      ol_ltp(Enc ,
      		&(st->pitchOLWghtSt),
      		&(st->vadSt),
      		mode,
      		&st->wsp[0],
      		&T_op[0],
            st->old_lags,
            st->ol_gain_flg,
            1,
            st->dtx);

      T_op[1] = T_op[0];
   }

   /* run VAD pitch detection */
   if (st->dtx)
   {  /* no test() call since this if is only in simulation env */
      vad_pitch_detection(&(st->vadSt), T_op);
   }

   if(*usedMode == MRDTX)
   {
	   /*--------------------------------------------------*
   	 * Update signal for next frame.                    *
	    *--------------------------------------------------*/
   	Copy(&st->old_wsp[L_FRAME+1], &st->old_wsp[1], PIT_MAX);
	   AMR_Copy32((Word32 *)&st->old_speech[L_FRAME],(Word32 *) &st->old_speech[0], (L_TOTAL - L_FRAME)/2);
   	return 0;
   }

   /*------------------------------------------------------------------------*
    *          Loop for every subframe in the analysis frame                 *
    *------------------------------------------------------------------------*
    *  To find the pitch and innovation parameters. The subframe size is     *
    *  L_SUBFR and the loop is repeated L_FRAME/L_SUBFR times.               *
    *     - find the weighted LPC coefficients                               *
    *     - find the LPC residual signal res[]                               *
    *     - compute the target signal for pitch search                       *
    *     - compute impulse response of weighted synthesis filter (h1[])     *
    *     - find the closed-loop pitch parameters                            *
    *     - encode the pitch dealy                                           *
    *     - update the impulse response h1[] by including fixed-gain pitch   *
    *     - find target vector for codebook search                           *
    *     - codebook search                                                  *
    *     - encode codebook address                                          *
    *     - VQ of pitch and codebook gains                                   *
    *     - find synthesis speech                                            *
    *     - update states of weighting filter                                *
    *------------------------------------------------------------------------*/

   A = A_t;      /* pointer to interpolated LPC parameters */
   Aq = Aq_t;    /* pointer to interpolated quantized LPC parameters */

   /* Other vectors */
   xn           = Enc_share->shabuf.stE.xn;
   xn2          = Enc_share->shabuf.stE.xn2;
   code         = Enc_share->shabuf.stE.code;
   y1           = Enc_share->shabuf.stE.y1;
   y2           = Enc_share->shabuf.stE.y2;
   gCoeff       = Enc_share->shabuf.stE.u0E.gCoeff;
   res          = Enc_share->shabuf.stE.res;
   res2         = Enc_share->shabuf.stE.res2;
	/* Vector and scalars needed for the MR475 */
   xn_sf0       = Enc_share->shabuf.stE.xn_sf0;
   y2_sf0       = Enc_share->shabuf.stE.u0E.y2_sf0;
   code_sf0     = Enc_share->shabuf.stE.code_sf0;
   h1_sf0       = Enc_share->shabuf.stE.h1_sf0;
   mem_syn_save = Enc_share->shabuf.stE.mem_syn_save;
   mem_w0_save  = Enc_share->shabuf.stE.mem_w0_save;
   mem_err_save = Enc_share->shabuf.stE.mem_err_save;


   evenSubfr = 0;
   subfrNr = -1;
   for (i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR)
   {
      subfrNr = (subfrNr + 1);
      evenSubfr = (1 - evenSubfr);

      /* Save states for the MR475 mode */
      if ((evenSubfr != 0) && ( *usedMode == MR475 ))
      {
         Copy(st->mem_syn, mem_syn_save, M);
         Copy(st->mem_w0, mem_w0_save, M);
         Copy(st->mem_err, mem_err_save, M);
         sharp_save = st->sharp;
      }

      /*-----------------------------------------------------------------*
       * - Preprocessing of subframe                                     *
       *-----------------------------------------------------------------*/
      if(*usedMode != MR475)
      {
         subframePreProc(Enc ,
								 *usedMode,
								 gamma1,
								 gamma1_12k2,
                         gamma2,
                         A,
                         Aq,
                         &st->speech[i_subfr],
                         st->mem_err,
                         st->mem_w0,
                         st->zero,
                         st->ai_zero,
                         &st->exc[i_subfr],
                         st->h1,
                         xn,
                         res,
                         st->error);
      }
      else
      { /* MR475 */
         subframePreProc(Enc ,
								 *usedMode,
								 gamma1,
								 gamma1_12k2,
                         gamma2,
                         A,
                         Aq,
                         &st->speech[i_subfr],
                         st->mem_err,
                         mem_w0_save,
                         st->zero,
                         st->ai_zero,
                         &st->exc[i_subfr],
                         st->h1,
                         xn,
                         res,
                         st->error);

         /* save impulse response (modified in cbsearch) */
         if (evenSubfr != 0)
         {
             AMR_Copy32((Word32 *)st->h1, (Word32 *)h1_sf0, L_SUBFR/2);
         }
      }

      /* copy the LP residual (res2 is modified in the CL LTP search)    */
      AMR_Copy32 ((Word32 *)res, (Word32 *)res2, L_SUBFR/2);

      /*-----------------------------------------------------------------*
       * - Closed-loop LTP search                                        *
       *-----------------------------------------------------------------*/
      cl_ltp(Enc ,
      		 *usedMode,
      		 i_subfr,
      		 T_op,
             lsp_flag,
             xn2,
             y1,
             &T0,
             &T0_frac,
             &gain_pit,
             &ana,
             &gp_limit);

      /* update LTP lag history */
      if ((subfrNr == 0) && (st->ol_gain_flg[0] > 0))
      {
         st->old_lags[1] = T0;
      }
      if ( (subfrNr == 3)  && (st->ol_gain_flg[1] > 0))
      {
         st->old_lags[0] = T0;
      }
      /*-----------------------------------------------------------------*
       * - Inovative codebook search (find index and gain)               *
       *-----------------------------------------------------------------*/
      cbsearch(Enc ,
      			T0,
      			st->sharp,
      			gain_pit,
               &ana,
               *usedMode,
               subfrNr);

      /*------------------------------------------------------*
       * - Quantization of gains.                             *
       *------------------------------------------------------*/
      gainQuant(Enc ,
      			 &(st->gainQuantSt),
      			 *usedMode,
      			 res,
      			 &st->exc[i_subfr],
      			 code,
                xn,
                xn2,
                y1,
                y2,
                gCoeff,
                evenSubfr,
                gp_limit,
                &gain_pit_sf0,
                &gain_code_sf0,
                &gain_pit,
                &gain_code,
                &ana);


      /* update gain history */
      update_gp_clipping(&(st->tonStabSt), gain_pit);


      if (*usedMode != MR475)
      {
         /* Subframe Post Porcessing */
         subframePostProc(Enc,st->speech,
         					  *usedMode,
         					  i_subfr,
         					  gain_pit,
                          gain_code,
                          Aq,
                          synth,
                          xn,
                          code,
                          y1,
                          y2,
                          st->mem_syn,
                          st->mem_err,
                          st->mem_w0,
                          st->exc,
                          &st->sharp);
      }
      else
      {
         if (evenSubfr != 0)
         {
            i_subfr_sf0 = i_subfr;

            AMR_Copy32((Word32 *)xn, (Word32 *)xn_sf0, L_SUBFR/2);
            AMR_Copy32((Word32 *)y2, (Word32 *)y2_sf0, L_SUBFR/2);
            AMR_Copy32((Word32 *)code, (Word32 *)code_sf0, L_SUBFR/2);

            T0_sf0 = T0;
            T0_frac_sf0 = T0_frac;

            /* Subframe Post Porcessing */
            subframePostProc(Enc,st->speech,
            					  *usedMode,
            					  i_subfr,
            					  gain_pit,
                             gain_code,
                             Aq,
                             synth,
                             xn,
                             code,
                             y1,
                             y2,
                             mem_syn_save,
                             st->mem_err,
                             mem_w0_save,
                             st->exc,
                             &st->sharp);

            st->sharp = sharp_save;
         }
         else
         {
            /* update both subframes for the MR475 */
            /* Restore states for the MR475 mode */
            AMR_Copy32((Word32 *)mem_err_save, (Word32 *)st->mem_err, M/2);

            /* re-build excitation for sf 0 */
            Pred_lt_3or6(&st->exc[i_subfr_sf0], T0_sf0, T0_frac_sf0,1);
            Convolve(&st->exc[i_subfr_sf0], h1_sf0, y1);

            Aq -= (MP1+1);
            subframePostProc( Enc,
										st->speech,
										*usedMode,
										i_subfr_sf0,
                              gain_pit_sf0,
                              gain_code_sf0,
                              Aq,
                              synth,
                              xn_sf0,
                              code_sf0,
                              y1,
                              y2_sf0,
                              st->mem_syn,
                              st->mem_err,
                              st->mem_w0, st->exc,
                              &sharp_save); /* overwrites sharp_save */
            Aq += MP1+1;

            /* re-run pre-processing to get xn right (needed by postproc) */
            /* (this also reconstructs the unsharpened h1 for sf 1)       */
            subframePreProc( Enc ,
            					  *usedMode,
            					  gamma1,
            					  gamma1_12k2,
                             gamma2,
                             A,
                             Aq,
                             &st->speech[i_subfr],
                             st->mem_err,
                             st->mem_w0,
                             st->zero,
                             st->ai_zero,
                             &st->exc[i_subfr],
                             st->h1,
                             xn,
                             res,
                             st->error);

            /* re-build excitation sf 1 (changed if lag < L_SUBFR) */
            Pred_lt_3or6(&st->exc[i_subfr], T0, T0_frac,  1);
            Convolve(&st->exc[i_subfr], st->h1, y1);

            subframePostProc( Enc,
										st->speech,
										*usedMode,
										i_subfr,
										gain_pit,
                              gain_code,
                              Aq,
                              synth,
                              xn,
                              code,
                              y1,
                              y2,
                              st->mem_syn,
                              st->mem_err,
                              st->mem_w0,
                              st->exc,
                              &st->sharp);

         }

      }

      A += MP1+1;    /* interpolated LPC parameters for next subframe */
      Aq += MP1+1;

   }

   AMR_Copy32((Word32 *)&st->old_exc[L_FRAME],(Word32 *) &st->old_exc[0], (PIT_MAX + L_INTERPOL)/2);

   /*--------------------------------------------------*
    * Update signal for next frame.                    *
    *--------------------------------------------------*/
//   Copy(&st->old_wsp[L_FRAME+1], &st->old_wsp[1], PIT_MAX);
	st->old_wsp[1] = st->old_wsp[L_FRAME+1];
   AMR_Copy32((Word32 *)&st->old_wsp[L_FRAME+2], (Word32 *)&st->old_wsp[2], (PIT_MAX-1)/2);


   AMR_Copy32((Word32 *)&st->old_speech[L_FRAME],(Word32 *) &st->old_speech[0], (L_TOTAL - L_FRAME)/2);

   return 0;
}

#endif

