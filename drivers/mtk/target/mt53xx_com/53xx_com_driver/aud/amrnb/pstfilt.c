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
 *   pstfilt.c
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
********************************************************************************
*
*      File             : pstfilt.c
*      Purpose          : Performs adaptive postfiltering on the synthesis
*                       : speech
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "pstfilt.h"
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "set_zero.h"
#include "weight_a.h"
#include "residu.h"
#include "copy.h"
#include "syn_filt.h"
#include "preemph.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
/*---------------------------------------------------------------*
 *    Postfilter constant parameters (defined in "cnst.h")       *
 *---------------------------------------------------------------*
 *   L_FRAME     : Frame size.                                   *
 *   L_SUBFR     : Sub-frame size.                               *
 *   M           : LPC order.                                    *
 *   MP1         : LPC order+1                                   *
 *   MU          : Factor for tilt compensation filter           *
 *   AGC_FAC     : Factor for automatic gain control             *
 *---------------------------------------------------------------*/

#define L_H 22  /* size of truncated impulse response of A(z/g1)/A(z/g2) */


/* Spectral expansion factors */
__align(4)
static const Word16 gamma3_MR122[M] = {
  22938, 16057, 11240, 7868, 5508,
  3856, 2699, 1889, 1322, 925
};

__align(4)
static const Word16 gamma4_MR122[M] = {
  24576, 18432, 13824, 10368, 7776,
  5832, 4374, 3281, 2461, 1846
};

__align(4)
static const Word16 gamma3[M] = {
  18022, 9912, 5451, 2998, 1649, 907, 499, 274, 151, 83
};

__align(4)
static const Word16 gamma4[M] = {
  22938, 16057, 11240, 7868, 5508, 3856, 2699, 1889, 1322, 925
};

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
*
*  Function:   Post_Filter_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int Post_Filter_reset (Post_FilterState *state)
{

   Set_zero (state->mem_syn_pst, M);
   Set_zero (state->res2, L_SUBFR);
   Set_zero (state->synth_buf, L_FRAME + M);
   //  agc_reset(state->agc_state);
   //  preemphasis_reset(state->preemph_state);
   agc_reset(state);
   preemphasis_reset(state);

   return 0;
}



/*
**************************************************************************
*  Function:  Post_Filter
*  Purpose:   postfiltering of synthesis speech.
*  Description:
*      The postfiltering process is described as follows:
*
*          - inverse filtering of syn[] through A(z/0.7) to get res2[]
*          - tilt compensation filtering; 1 - MU*k*z^-1
*          - synthesis filtering through 1/A(z/0.75)
*          - adaptive gain control
*
**************************************************************************
*/

int Post_Filter (
    AMR_Decode_FrameState *Dec,
    Post_FilterState *st, /* i/o : post filter states                        */
    enum Mode mode,       /* i   : AMR mode                                  */
    Word16 *syn,          /* i/o : synthesis speech (postfiltered is output) */
    Word16 *Az_4          /* i   : interpolated LPC parameters in all subfr. */
)
{
/*-------------------------------------------------------------------*
*           Declaration of parameters                               *
   *-------------------------------------------------------------------*/
	AMR_Shared_Buf *Dec_share = Dec->share;

	Word32 i;
   Word32 L_tmp,L_tmp1;
   Word32 i_subfr;             /* index for beginning of subframe  */
   Word32 *lptr,t,s;
	Word32 *tmp_w = Dec_share->shabuf.stD.u2D.s3D.tmp_w;

   Word16 *Ap3 = Dec_share->shabuf.stD.u2D.s3D.Ap3;		   /* bandwidth expanded LP parameters */
	Word16 *Ap4 = Dec_share->shabuf.stD.u2D.s3D.Ap4;
   Word16 *h = Dec_share->shabuf.stD.u2D.s3D.h;

   Word16 *Az;                 /* pointer to Az_4:                 */
   /*  LPC parameters in each subframe */
   Word16 temp1, temp2;
   Word16 *syn_work = &st->synth_buf[M];

   /*-----------------------------------------------------*
   * Post filtering                                      *
   *-----------------------------------------------------*/
   //    Copy (syn, syn_work , L_FRAME);
   AMR_Copy32((Word32 *)syn, (Word32 *)syn_work , L_FRAME/2);

   Az = Az_4;
   for (i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR)
   {
      /* Find weighted filter coefficients Ap3[] and ap[4] */
      if ( (mode == MR122) || (mode == MR102) )
      {
         Weight_Ai (Az, gamma3_MR122, Ap3);
         Weight_Ai (Az, gamma4_MR122, Ap4);
      }
      else
      {
         Weight_Ai (Az, gamma3, Ap3);
         Weight_Ai (Az, gamma4, Ap4);
      }

      /* filtering of synthesis speech by A(z/0.7) to find res2[] */
      Residu (Ap3, &syn_work[i_subfr], st->res2);

      /* tilt compensation filter */
      /* impulse response of A(z/0.7)/A(z/0.75) */
      Copy(Ap3, h, M + 1);

      //       Set_zero (&h[M + 1], L_H - M - 1);
      //       Syn_filt (Ap4, h, h, L_H, &h[M + 1], 0);
      Set_zero (&(h[M + 1]), L_H - M - 1);
      Syn_filt_Post (tmp_w , Ap4, h,
			h, L_H, &(h[M + 1]), 0);

      /* 1st correlation of h[] */
		lptr = (Word32 *)(h);
      L_tmp = 0;
		i = L_H/2;
		do
      {
      	t = *lptr++;
         L_tmp = qdadd(L_tmp,smulbb(t, t));
         L_tmp = qdadd(L_tmp,smultt(t, t));
      }while(--i != 0);
      temp1 = (Word16)(L_tmp>>16);

		lptr = (Word32 *)(h);
     	t = *lptr++;
      L_tmp1 = 0;
      L_tmp1 = qdadd(L_tmp1,smulbt(t,t));
		i = (L_H - 2)/4;
		do
      {
        	s = *lptr++;
         L_tmp1 = qdadd(L_tmp1,smultb(t, s));
         L_tmp1 = qdadd(L_tmp1,smulbt(s, s));

        	t = *lptr++;
         L_tmp1 = qdadd(L_tmp1,smultb(s, t));
         L_tmp1 = qdadd(L_tmp1,smulbt(t, t));

      }while(--i != 0);
      temp2 = (Word16)(L_tmp1>>16);

      if (temp2 <= 0)
      {
         temp2 = 0;
      }
      else
      {
         temp2 = smulbb(temp2, MU) >> 15;
         temp2 = div_s(temp2, temp1);
      }

      preemphasis (&(st->mem_pre), st->res2, temp2);

      /* filtering through  1/A(z/0.75) */
      Syn_filt(tmp_w , Ap4, st->res2, &syn[i_subfr],
      				L_SUBFR, st->mem_syn_pst, 1);

      /* scale output to input */
      agc(&(st->past_gain), &syn_work[i_subfr], &syn[i_subfr],AGC_FAC);

      Az += MP1+1;
   }

   /* update syn_work[] buffer */
   Copy (&syn_work[L_FRAME - M], &syn_work[-M], M);

   return 0;
}

#endif

