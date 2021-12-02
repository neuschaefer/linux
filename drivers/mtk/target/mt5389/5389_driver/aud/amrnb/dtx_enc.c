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
 *   dtx_enc.c
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
*      File             : dtx_enc.c
*      Purpose          : DTX mode computation of SID parameters
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "dtx_enc.h"
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "q_plsf.h"
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "copy.h"
#include "set_zero.h"
#include "lsp_lsf.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#include "tab_lsp.h"

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/


/*
**************************************************************************
*
*  Function    : dtx_enc_reset
*
**************************************************************************
*/
int dtx_enc_reset (dtx_encState *st)
{
  Word32 i;

  st->hist_ptr = 0;
  st->log_en_index = 0;
  st->init_lsf_vq_index = 0;
  st->lsp_index[0] = 0;
  st->lsp_index[1] = 0;
  st->lsp_index[2] = 0;

  /* Init lsp_hist[] */
  for(i = 0; i < DTX_HIST_SIZE; i++)
  {
    Copy(lsp_init_data, &st->lsp_hist[i * M], M);
  }

  /* Reset energy history */
  Set_zero(st->log_en_hist, M);

  st->dtxHangoverCount = DTX_HANG_CONST;
  st->decAnaElapsedCount = 32767;

  return 1;
}



/*
**************************************************************************
*
*  Function    : dtx_enc
*
**************************************************************************
*/
int dtx_enc(
			   AMR_Encode_FrameState *Enc,
				dtx_encState *st,        /* i/o : State struct                    */
            Word16 computeSidFlag,   /* i   : compute SID                     */
//            Q_plsfState *qSt,        /* i/o : Qunatizer state struct          */
				Word16 past_rq[],
            gc_predState* predState, /* i/o : State struct                    */
	    Word16 **anap            /* o   : analysis parameters             */
	    )
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i,j;
   Word16 log_en;

//   Word32 L_lsp[M];
//   Word16 lsf[M];
//   Word16 lsp[M];
//   Word16 lsp_q[M];
   Word32 *L_lsp = Enc_share->shabuf.stE.u2E.s3E.L_lsp;
   Word16 *lsf = Enc_share->shabuf.stE.u2E.s3E.lsf;
   Word16 *lsp = Enc_share->shabuf.stE.u2E.s3E.lsp;
   Word16 *lsp_q = Enc_share->shabuf.stE.u2E.s3E.lsp_q;


   /* VOX mode computation of SID parameters */
   if ((computeSidFlag != 0))
   {
      /* compute new SID frame if safe i.e don't
       * compute immediately after a talk spurt  */
      log_en = 0;
      for (i = 0; i < M; i++)
      {
         L_lsp[i] = 0;
      }

      /* average energy and lsp */
      for (i = 0; i < DTX_HIST_SIZE; i++)
      {
         log_en = (log_en + (st->log_en_hist[i]>>2));

         for (j = 0; j < M; j++)
         {
            L_lsp[j] = qadd(L_lsp[j], (st->lsp_hist[i * M + j]));
         }
      }

      log_en = (log_en >> 1);
      for (j = 0; j < M; j++)
      {
         lsp[j] = (Word16)( (L_lsp[j] >> 3));   /* divide by 8 */
      }

      /*  quantize logarithmic energy to 6 bits */
      st->log_en_index = (log_en + 2560);          /* +2.5 in Q10      */
      st->log_en_index = (st->log_en_index + 128); /* add 0.5/4 in Q10 */
      st->log_en_index = (st->log_en_index >> 8);


      if(st->log_en_index > 63)
      {
         st->log_en_index = 63;
      }

      if (st->log_en_index < 0)
      {
         st->log_en_index = 0;
      }

      /* update gain predictor memory */
      log_en = sature(st->log_en_index << 8); /* Q11 and divide by 4 */
      log_en = (log_en- 2560);            /* add 2.5 in Q11      */

      log_en = (log_en- 9000);

      if (log_en > 0)
      {
         log_en = 0;
      }

      if(log_en < -14436)
      {
         log_en = -14436;
      }

      /* past_qua_en for other modes than MR122 */
      predState->past_qua_en[0] = log_en;
      predState->past_qua_en[1] = log_en;
      predState->past_qua_en[2] = log_en;
      predState->past_qua_en[3] = log_en;

      /* scale down by factor 20*log10(2) in Q15 */
      log_en = smulbb(5443, log_en) >> 15;

      /* past_qua_en for mode MR122 */
      predState->past_qua_en_MR122[0] = log_en;
      predState->past_qua_en_MR122[1] = log_en;
      predState->past_qua_en_MR122[2] = log_en;
      predState->past_qua_en_MR122[3] = log_en;

      /* make sure that LSP's are ordered */
      Lsp_lsf(lsp, lsf);
      Reorder_lsf(lsf);
      Lsf_lsp(lsf, lsp);

      /* Quantize lsp and put on parameter list */
      Q_plsf_3(Enc , past_rq, MRDTX, lsp, lsp_q, st->lsp_index, &st->init_lsf_vq_index);
   }

   *(*anap)++ = st->init_lsf_vq_index; /* 3 bits */

   *(*anap)++ = st->lsp_index[0];      /* 8 bits */
   *(*anap)++ = st->lsp_index[1];      /* 9 bits */
   *(*anap)++ = st->lsp_index[2];      /* 9 bits */

   *(*anap)++ = st->log_en_index;      /* 6 bits    */
                                       /* = 35 bits */

   return 0;
}

/*
**************************************************************************
*
*  Function    : dtx_buffer
*  Purpose     : handles the DTX buffer
*
**************************************************************************
*/
int dtx_buffer(dtx_encState *st,   /* i/o : State struct                    */
	       Word16 lsp_new[],   /* i   : LSP vector                      */
	       Word16 speech[]     /* i   : speech samples                  */
	       )
{
   Word32 i;
   Word32 L_frame_en , *ptr32_sp , s;
   Word16 log_en_e;
   Word16 log_en_m;
   Word16 log_en;

   /* update pointer to circular buffer      */
   st->hist_ptr = (st->hist_ptr + 1);

   if(st->hist_ptr == DTX_HIST_SIZE)
   {
      st->hist_ptr = 0;
   }

   /* copy lsp vector into buffer */
   Copy(lsp_new, &st->lsp_hist[st->hist_ptr * M], M);


   /* compute log energy based on frame energy */
   L_frame_en = 0;     /* Q0 */


	ptr32_sp = (Word32 *)speech;

	i = L_FRAME/2;
	do
   {
   	s = *ptr32_sp++;
      L_frame_en = qdadd(L_frame_en, smulbb(s, s));
      L_frame_en = qdadd(L_frame_en, smultt(s, s));
   }while(-- i != 0);


   Log2(L_frame_en, &log_en_e, &log_en_m);

   /* convert exponent and mantissa to Word16 Q10 */
   log_en = sature(log_en_e << 10);  /* Q10 */
   log_en = (log_en + (log_en_m >> (15-10)));

   /* divide with L_FRAME i.e subtract with log2(L_FRAME) = 7.32193 */
   log_en = (log_en - 8521);

   /* insert into log energy buffer with division by 2 */
   log_en = (log_en >> 1);
   st->log_en_hist[st->hist_ptr] = log_en; /* Q10 */

   return 0;
}

/*
**************************************************************************
*
*  Function    : tx_dtx_handler
*  Purpose     : adds extra speech hangover to analyze speech on the decoding side.
*
**************************************************************************
*/
Word16 tx_dtx_handler(dtx_encState *st,      /* i/o : State struct           */
                      Word16 vad_flag,       /* i   : vad decision           */
                      enum Mode *usedMode    /* i/o : mode changed or not    */
                      )
{
   Word32 compute_new_sid_possible;

   msr_cpsr_f_0();
   /* this state machine is in synch with the GSMEFR txDtx machine      */
   st->decAnaElapsedCount = add(st->decAnaElapsedCount, 1);

   compute_new_sid_possible = 0;


   if (vad_flag != 0)
   {
      st->dtxHangoverCount = DTX_HANG_CONST;
   }
   else
   {  /* non-speech */

      if (st->dtxHangoverCount == 0)
      {  /* out of decoder analysis hangover  */
         st->decAnaElapsedCount = 0;
         *usedMode = MRDTX;
         compute_new_sid_possible = 1;
      }
      else
      { /* in possible analysis hangover */
         st->dtxHangoverCount = sub(st->dtxHangoverCount, 1);

         /* decAnaElapsedCount + dtxHangoverCount < DTX_ELAPSED_FRAMES_THRESH */

         if (( (st->decAnaElapsedCount + st->dtxHangoverCount)-
                 DTX_ELAPSED_FRAMES_THRESH) < 0)
         {
            *usedMode = MRDTX;
            /* if short time since decoder update, do not add extra HO */
         }
         /*
          else
            override VAD and stay in
            speech mode *usedMode
            and add extra hangover
         */
      }
   }

   return (Word16)compute_new_sid_possible;

}

#endif

