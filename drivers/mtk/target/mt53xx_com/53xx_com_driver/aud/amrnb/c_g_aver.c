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
 *   c_g_aver.c
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


/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : c_g_aver.c
*      Purpose          :
*
********************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "c_g_aver.h"


#include "typedef.h"
#include "basic_op.h"
#include "set_zero.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
/*-----------------------------------------------------------------*
 *   Decoder constant parameters (defined in "cnst.h")             *
 *-----------------------------------------------------------------*
 *   L_FRAME       : Frame size.                                   *
 *   L_SUBFR       : Sub-frame size.                               *
 *-----------------------------------------------------------------*/

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*
**************************************************************************
*
*  Function    : Cb_gain_average_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
Word16 Cb_gain_average_reset (Cb_gain_averageState *state)
{

   /* Static vectors to zero */
   Set_zero (state->cbGainHistory, L_CBGAINHIST);

   /* Initialize hangover handling */
   state->hangVar = 0;
   state->hangCount= 0;

   return 0;
}

/*
**************************************************************************
*
*  Function    : Cb_gain_average
*  Purpose     :
*  Returns     : The mix cb gains for MR475, MR515, MR59, MR67, MR102; gain_code other modes
*
**************************************************************************
*/
Word16 Cb_gain_average (
   AMR_Decode_FrameState *Dec,
   Cb_gain_averageState *st, /* i/o : State variables for CB gain avergeing   */
   enum Mode mode,           /* i   : AMR mode                                */
   Word16 gain_code,         /* i   : CB gain                              Q1 */
   Word16 lsp[],             /* i   : The LSP for the current frame       Q15 */
   Word16 lspAver[],         /* i   : The average of LSP for 8 frames     Q15 */
   Word16 bfi,               /* i   : bad frame indication flag               */
   Word16 prev_bf,           /* i   : previous bad frame indication flag      */
   Word16 pdfi,              /* i   : potential degraded bad frame ind flag   */
   Word16 prev_pdf,          /* i   : prev pot. degraded bad frame ind flag   */
   Word16 inBackgroundNoise, /* i   : background noise decision               */
   Word16 voicedHangover     /* i   : # of frames after last voiced frame     */
   )
{
/*---------------------------------------------------------*
* Compute mixed cb gain, used to make cb gain more        *
* smooth in background noise for modes 5.15, 5.9 and 6.7  *
* states that needs to be updated by all                  *
   *---------------------------------------------------------*/
   Word32 i;
   Word32 L_sum,cbGainMix, shift1, shift2,shift;
   Word32 diff, tmp_diff, bgMix, cbGainMean;
   Word16 tmp1, tmp2,*sptr;
//	Word16 tmp[M];
	AMR_Shared_Buf *Dec_share = Dec->share;
	Word16 *tmp = Dec_share->shabuf.stD.u2D.s1D.tmp;

   /*-------------------------------------------------------*
   *   Store list of CB gain needed in the CB gain         *
   *   averaging                                           *
   *-------------------------------------------------------*/
   st->cbGainHistory[0] = st->cbGainHistory[1];
   st->cbGainHistory[1] = st->cbGainHistory[2];
   st->cbGainHistory[2] = st->cbGainHistory[3];
   st->cbGainHistory[3] = st->cbGainHistory[4];
   st->cbGainHistory[4] = st->cbGainHistory[5];
   st->cbGainHistory[5] = st->cbGainHistory[6];
   st->cbGainHistory[L_CBGAINHIST-1] = gain_code;

   /* compute lsp difference */
	sptr = tmp;
   diff = 0;

	i= M;
	do
	{
      tmp1 = abs_s((*lspAver - *lsp));  /* Q15       */
      shift1 = (norm_s(tmp1) - 1);          /* Qn        */
      tmp1 = (tmp1 << shift1);               /* Q15+Qn    */
      shift2 = norm_s(*lspAver);            /* Qm        */
      tmp2 = (*lspAver << shift2);         /* Q15+Qm    */
      *sptr = div_s(tmp1, tmp2);             /* Q15+(Q15+Qn)-(Q15+Qm) */

		lspAver++;
		lsp++;

      shift = ((2 + shift1) - shift2);
      if (shift >= 0)
      {
         *sptr = (*sptr >> shift);           /* Q15+Qn-Qm-Qx=Q13 */
      }
      else
      {
         *sptr = sature(*sptr << negate(shift));   /* Q15+Qn-Qm-Qx=Q13 */
      }
      diff = (diff + *sptr);       /* Q13 */

		sptr++;

   }while(-- i != 0);


   /* Compute hangover */
   if(diff > 5325)   /* 0.65 in Q11 */
   {
      st->hangVar = (st->hangVar + 1);
   }
   else
   {
      st->hangVar = 0;
   }


   if(st->hangVar > 10)
   {
      st->hangCount = 0;  /* Speech period, reset hangover variable */
   }

   /* Compute mix constant (bgMix) */
   bgMix = 8192;    /* 1 in Q13 */


   /* set correct cbGainMix for MR74, MR795, MR122 */
   cbGainMix = gain_code;


   if ( (mode <= MR67)  || (mode == MR102))
      /* MR475, MR515, MR59, MR67, MR102 */
   {
      /* if errors and presumed noise make smoothing probability stronger */
      if ((  (  ((pdfi != 0) && (prev_pdf != 0)) || (bfi != 0) || (prev_bf != 0)) &&
          ( (voicedHangover > 1) ) && (inBackgroundNoise != 0) && 
           ( (mode == MR475)  ||
             (mode == MR515)   ||
             (mode == MR59) )
             ))
      {
         /* bgMix = min(0.25, max(0.0, diff-0.55)) / 0.25; */
         tmp_diff = (diff - 4506);   /* 0.55 in Q13 */

         /* max(0.0, diff-0.55) */

         if (tmp_diff > 0)
         {
            tmp1 = tmp_diff;
         }
         else
         {
            tmp1 = 0;
         }

         /* min(0.25, tmp1) */

         if(2048 < tmp1)
         {
            bgMix = 8192;
         }
         else
         {
            bgMix = sature(tmp1 << 2);
         }
      }
      else
      {
         /* bgMix = min(0.25, max(0.0, diff-0.40)) / 0.25; */
         tmp_diff = (diff - 3277); /* 0.4 in Q13 */

         /* max(0.0, diff-0.40) */

         if (tmp_diff > 0)
         {
            tmp1 = tmp_diff;
         }
         else
         {
            tmp1 = 0;
         }

         /* min(0.25, tmp1) */

         if(2048 < tmp1)
         {
            bgMix = 8192;
         }
         else
         {
            bgMix = sature(tmp1 << 2);
         }
      }


      if ( (st->hangCount < 40) || (diff > 5325) ) /* 0.65 in Q13 */
      {
         bgMix = 8192;  /* disable mix if too short time since */
      }

      /* Smoothen the cb gain trajectory  */
      /* smoothing depends on mix constant bgMix */
      L_sum = smulbb(6554, st->cbGainHistory[2]);     /* 0.2 in Q15; L_sum in Q17 */
		L_sum = qadd(L_sum,L_sum);

      L_sum = qdadd(L_sum, smulbb(6554, st->cbGainHistory[3]) );
      L_sum = qdadd(L_sum, smulbb(6554, st->cbGainHistory[4]) );
      L_sum = qdadd(L_sum, smulbb(6554, st->cbGainHistory[5]) );
      L_sum = qdadd(L_sum, smulbb(6554, st->cbGainHistory[6]) );

      cbGainMean = (Word16)(qadd(L_sum,0x8000)>>16);                      /* Q1 */

      /* more smoothing in error and bg noise (NB no DFI used  here) */

      if ( ((bfi != 0) || (prev_bf != 0)) && (inBackgroundNoise != 0) &&
            ( (mode == MR475)  ||
             (mode == MR515)  ||
             (mode == MR59) )
          )
      {

         L_sum = 0;                            /* 0.143 in Q15; L_sum in Q17 */
         for (i = 0; i < L_CBGAINHIST; i++)
         {
            L_sum = qdadd(L_sum,smulbb( 4681, st->cbGainHistory[i]));
         }
         cbGainMean = (Word16)(qadd(L_sum,0x8000)>>16);                   /* Q1 */

      }

      /* cbGainMix = bgMix*cbGainMix + (1-bgMix)*cbGainMean; */
      L_sum = smulbb(bgMix, cbGainMix);               /* L_sum in Q15 */
		L_sum = qadd(L_sum ,L_sum );

      L_sum = qdadd(L_sum,smulbb(8192, cbGainMean));

      L_sum = qdsub(L_sum,smulbb(bgMix, cbGainMean));

      cbGainMix = (Word16)(qadd(L_shl(L_sum, 2),0x8000)>>16);             /* Q1 */

   }

   st->hangCount = (st->hangCount + 1);
   return (Word16)cbGainMix;

}

#endif

