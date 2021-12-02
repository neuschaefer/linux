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
 *   ph_disp.c
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
*      File             : ph_disp.c
*      Purpose          : Perform adaptive phase dispersion of the excitation
*                         signal.
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "ph_disp.h"


/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "copy.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/

//#include "ph_disp.tab"
/*
********************************************************************************
*      Purpose          : Table of impulse responses of phase dispersion filters
********************************************************************************
*/

/* The following tables are constant, but not declared constant due to
   compiler warnings in conjunction with the program code              */

/* All impulse responses are in Q15 */

static const Word16 ph_imp_low_MR795[] =
{
  26777,    801,   2505,   -683,  -1382,    582,    604,  -1274,   3511,  -5894,
   4534,   -499,  -1940,   3011,  -5058,   5614,  -1990,  -1061,  -1459,   4442,
   -700,  -5335,   4609,    452,   -589,  -3352,   2953,   1267,  -1212,  -2590,
   1731,   3670,  -4475,   -975,   4391,  -2537,    949,  -1363,   -979,   5734
};
static const Word16 ph_imp_mid_MR795[] =
{
  30274,   3831,  -4036,   2972,  -1048,  -1002,   2477,  -3043,   2815,  -2231,
   1753,  -1611,   1714,  -1775,   1543,  -1008,    429,   -169,    472,  -1264,
   2176,  -2706,   2523,  -1621,    344,    826,  -1529,   1724,  -1657,   1701,
  -2063,   2644,  -3060,   2897,  -1978,    557,    780,  -1369,    842,    655
};

static const Word16 ph_imp_low[] =
{
  14690,  11518,   1268,  -2761,  -5671,   7514,    -35,  -2807,  -3040,   4823,
   2952,  -8424,   3785,   1455,   2179,  -8637,   8051,  -2103,  -1454,    777,
   1108,  -2385,   2254,   -363,   -674,  -2103,   6046,  -5681,   1072,   3123,
  -5058,   5312,  -2329,  -3728,   6924,  -3889,    675,  -1775,     29,  10145
};
static const Word16 ph_imp_mid[] =
{
  30274,   3831,  -4036,   2972,  -1048,  -1002,   2477,  -3043,   2815,  -2231,
   1753,  -1611,   1714,  -1775,   1543,  -1008,    429,   -169,    472,  -1264,
   2176,  -2706,   2523,  -1621,    344,    826,  -1529,   1724,  -1657,   1701,
  -2063,   2644,  -3060,   2897,  -1978,    557,    780,  -1369,    842,    655
};



/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/


/*************************************************************************
*
*  Function:   ph_disp_reset
*
**************************************************************************
*/
int ph_disp_reset (ph_dispState *state)
{

   state->gainMem[0] = 0;
   state->gainMem[1] = 0;
   state->gainMem[2] = 0;
   state->gainMem[3] = 0;
   state->gainMem[4] = 0;

   state->prevState = 0;
   state->prevCbGain = 0;
   state->lockFull = 0;
   state->onset = 0;          /* assume no onset in start */

   return 0;
}

/*************************************************************************
*
*  Function:   ph_disp_lock
*
**************************************************************************
*/
void ph_disp_lock (ph_dispState *state)
{
   state->lockFull = 1;
   return;
}

/*************************************************************************
*
*  Function:   ph_disp_release
*
**************************************************************************
*/
void ph_disp_release (ph_dispState *state)
{
   state->lockFull = 0;
   return;
}

/*************************************************************************
*
*  Function:   ph_disp
*
*              Adaptive phase dispersion; forming of total excitation
*              (for synthesis part of decoder)
*
**************************************************************************
*/
void ph_disp (
	   AMR_Decode_FrameState *Dec,
      ph_dispState *state, /* i/o     : State struct                     */
      enum Mode mode,      /* i       : codec mode                       */
      Word16 x[],          /* i/o Q0  : in:  LTP excitation signal       */
                           /*           out: total excitation signal     */
      Word16 cbGain,       /* i   Q1  : Codebook gain                    */
      Word16 ltpGain,      /* i   Q14 : LTP gain                         */
      Word16 inno[],       /* i/o Q13 : Innovation vector (Q12 for 12.2) */
      Word16 pitch_fac,    /* i   Q14 : pitch factor used to scale the
                                        LTP excitation (Q13 for 12.2)    */
      Word16 tmp_shift     /* i   Q0  : shift factor applied to sum of
                                        scaled LTP ex & innov. before
                                        rounding                         */
)
{
   Word32 i,j,i1;
   Word32 L_temp;
   Word32 nze, nPulse, ppos;
   Word32 impNr;           /* indicator for amount of disp./filter used */

//   Word16 inno_sav[L_SUBFR];
//   Word16 ps_poss[L_SUBFR];

	AMR_Shared_Buf *Dec_share = Dec->share;
   Word16 *inno_sav = Dec_share->shabuf.stD.u2D.s2D.inno_sav;
   Word16 *ps_poss  = Dec_share->shabuf.stD.u2D.s2D.ps_poss;

   Word16 tmp1;
   const Word16 *ph_imp;   /* Pointer to phase dispersion filter */

   /* Update LTP gain memory */

   state->gainMem[4] = state->gainMem[3];
   state->gainMem[3] = state->gainMem[2];
   state->gainMem[2] = state->gainMem[1];
   state->gainMem[1] = state->gainMem[0];

   state->gainMem[0] = ltpGain;

   /* basic adaption of phase dispersion */

   if(ltpGain < PHDTHR2LTP)
	{    /* if (ltpGain < 0.9) */

      if(ltpGain > PHDTHR1LTP)
      {  /* if (ltpGain > 0.6 */
         impNr = 1; /* medium dispersion */
      }
      else
      {
         impNr = 0; /* maximum dispersion */
      }
   }
   else
   {
      impNr = 2; /* no dispersion */
   }

   /* onset indicator */
   /* onset = (cbGain  > onFact * cbGainMem[0]) */
   i = smulbb(state->prevCbGain, ONFACTPLUS1);
	i = qadd(i,i);
   tmp1 = (Word16)(qadd((L_shl(i,2)),0x8000)>>16);

   if(cbGain > tmp1)
   {
      state->onset = ONLENGTH;
   }
   else
   {
      if (state->onset > 0)
      {
         state->onset = (state->onset - 1);
      }
   }

   /* if not onset, check ltpGain buffer and use max phase dispersion if
   half or more of the ltpGain-parameters say so */

   if (state->onset == 0)
   {
      /* Check LTP gain memory and set filter accordingly */
      i1 = 0;
      {
         if(state->gainMem[0] < PHDTHR1LTP)
         {
            i1 = (i1 + 1);
         }
         if(state->gainMem[1] < PHDTHR1LTP)
         {
            i1 = (i1 + 1);
         }
         if(state->gainMem[2] < PHDTHR1LTP)
         {
            i1 = (i1 + 1);
         }
         if(state->gainMem[3] < PHDTHR1LTP)
         {
            i1 = (i1 + 1);
         }
         if(state->gainMem[4] < PHDTHR1LTP)
         {
            i1 = (i1 + 1);
         }

      }

      if(i1 > 2)
      {
         impNr = 0;
      }
   }

   /* Restrict decrease in phase dispersion to one step if not onset */

   if( (impNr > (state->prevState+1))  && (state->onset == 0))
   {
      impNr = (impNr - 1);
   }
   /* if onset, use one step less phase dispersion */

   if( (impNr < 2) && (state->onset > 0) )
   {
      impNr = (impNr + 1);
   }

   /* disable for very low levels */

   if(cbGain < 10)
   {
      impNr = 2;
   }

   if(state->lockFull == 1)
   {
      impNr = 0;
   }

   /* update static memory */
   state->prevState = (Word16)impNr;
   state->prevCbGain = cbGain;

   /* do phase dispersion for all modes but 12.2 and 7.4;
   don't modify the innovation if impNr >=2 (= no phase disp) */

   if( (mode != MR122) && (mode != MR102)  &&
       (mode != MR74)  && (impNr < 2) )
   {
   /* track pulse positions, save innovation,
      and initialize new innovation          */
      nze = 0;
      for (i = 0; i < L_SUBFR; i++)
      {
         if (inno[i] != 0)
         {
            ps_poss[nze] = i;
            nze = (nze + 1);
         }
         inno_sav[i] = inno[i];
         inno[i] = 0;
      }
      /* Choose filter corresponding to codec mode and dispersion criterium */

      if(mode == MR795)
      {
         if (impNr == 0)
         {
            ph_imp = ph_imp_low_MR795;
         }
         else
         {
            ph_imp = ph_imp_mid_MR795;
         }
      }
      else
      {
         if (impNr == 0)
         {
            ph_imp = ph_imp_low;
         }
         else
         {
            ph_imp = ph_imp_mid;
         }
      }

      /* Do phase dispersion of innovation */
      for (nPulse = 0; nPulse < nze; nPulse++)
      {
         ppos = ps_poss[nPulse];

         /* circular convolution with impulse response */
         j = 0;
         for (i = ppos; i < L_SUBFR; i++)
         {
            /* inno[i1] += inno_sav[ppos] * ph_imp[i1-ppos] */
            tmp1 = smulbb(inno_sav[ppos], ph_imp[j++]) >> 15;
            inno[i] = (inno[i] + tmp1);
         }

         for (i = 0; i < ppos; i++)
         {
            /* inno[i] += inno_sav[ppos] * ph_imp[L_SUBFR-ppos+i] */
            tmp1 = smulbb(inno_sav[ppos], ph_imp[j++]) >> 15;
            inno[i] = (inno[i] + tmp1);
         }
      }
   }

   /* compute total excitation for synthesis part of decoder
   (using modified innovation if phase dispersion is active) */

	i = L_SUBFR/2;
	do
   {
      /* x[i] = gain_pit*x[i] + cbGain*code[i]; */
      L_temp = smulbb(        *x,    pitch_fac);
		L_temp = qadd(L_temp,L_temp);
      /* 12.2: Q0 * Q13 */
      /*  7.4: Q0 * Q14 */
      L_temp = qdadd(L_temp,smulbb( *inno++, cbGain));
      /* 12.2: Q12 * Q1 */
      /*  7.4: Q13 * Q1 */
      L_temp = L_shl(L_temp, tmp_shift);                 /* Q16 */
      *x++ = (Word16)((qadd(L_temp,0x8000)) >> 16);

      j = smulbb(        *x,    pitch_fac);
		j = qadd(j,j);
      j = qdadd(j,smulbb( *inno++, cbGain));
      j = L_shl(j, tmp_shift);                 /* Q16 */
      *x++ = (Word16)((qadd(j,0x8000)) >> 16);

   }while(--i != 0);

   return;

}

#endif


