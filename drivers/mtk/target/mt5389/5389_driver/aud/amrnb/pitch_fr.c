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
 *   pitch_fr.c
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
*      File             : pitch_fr.c
*      Purpose          : Find the pitch period with 1/3 or 1/6 subsample
*                       : resolution (closed loop).
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "pitch_fr.h"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "inter_36.h"
#include "convolve.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/

/*
 * mode dependent parameters used in Pitch_fr()
 * Note: order of MRxx in 'enum Mode' is important!
 */

static const struct {
    Word32 max_frac_lag;     /* lag up to which fractional lags are used    */
    Word32 flag3;            /* enable 1/3 instead of 1/6 fract. resolution */
    Word32 first_frac;       /* first fractional to check                   */
    Word32 last_frac;        /* last fractional to check                    */
    Word32 delta_int_low;    /* integer lag below TO to start search from   */
    Word32 delta_int_range;  /* integer range around T0                     */
    Word32 delta_frc_low;    /* fractional below T0                         */
    Word32 delta_frc_range;  /* fractional range around T0                  */
    Word32 pit_min;          /* minimum pitch                               */
} mode_dep_parm[N_MODES] = {
    /* MR475 */  { 84,  1, -2,  2,  5, 10,  5,  9, PIT_MIN },
    /* MR515 */  { 84,  1, -2,  2,  5, 10,  5,  9, PIT_MIN },
    /* MR59  */  { 84,  1, -2,  2,  3,  6,  5,  9, PIT_MIN },
    /* MR67  */  { 84,  1, -2,  2,  3,  6,  5,  9, PIT_MIN },
    /* MR74  */  { 84,  1, -2,  2,  3,  6,  5,  9, PIT_MIN },
    /* MR795 */  { 84,  1, -2,  2,  3,  6, 10, 19, PIT_MIN },
    /* MR102 */  { 84,  1, -2,  2,  3,  6,  5,  9, PIT_MIN },
    /* MR122 */  { 94,  0, -3,  3,  3,  6,  5,  9, PIT_MIN_MR122 }
};


//Word16 T0_prev_subframe;



/*
********************************************************************************
*                         LOCAL PROGRAM CODE
********************************************************************************
*/


/*************************************************************************
 *
 *   FUNCTION:  Enc_lag3
 *
 *   PURPOSE:  Encoding of fractional pitch lag with 1/3 resolution.
 *
 *   DESCRIPTION:
 *                    First and third subframes:
 *                    --------------------------
 *   The pitch range is divided as follows:
 *           19 1/3  to   84 2/3   resolution 1/3
 *           85      to   143      resolution 1
 *
 *   The period is encoded with 8 bits.
 *   For the range with fractions:
 *     index = (T-19)*3 + frac - 1;
 *                         where T=[19..85] and frac=[-1,0,1]
 *   and for the integer only range
 *     index = (T - 85) + 197;        where T=[86..143]
 *
 *                    Second and fourth subframes:
 *                    ----------------------------
 *   For the 2nd and 4th subframes a resolution of 1/3 is always used,
 *   and the search range is relative to the lag in previous subframe.
 *   If t0 is the lag in the previous subframe then
 *   t_min=t0-5   and  t_max=t0+4   and  the range is given by
 *        t_min - 2/3   to  t_max + 2/3
 *
 *   The period in the 2nd (and 4th) subframe is encoded with 5 bits:
 *     index = (T-(t_min-1))*3 + frac - 1;
 *                 where T=[t_min-1..t_max+1]
 *
 *************************************************************************/
Word16
Enc_lag3(                /* o  : Return index of encoding             */
    Word32 T0,           /* i  : Pitch delay                          */
    Word32 T0_frac,      /* i  : Fractional pitch delay               */
    Word16 T0_prev,      /* i  : Integer pitch delay of last subframe */
    Word32 T0_min,       /* i  : minimum of search range              */
    Word32 T0_max,       /* i  : maximum of search range              */
    Word32 flag4         /* i  : Flag for encoding with 4 bits        */
    )
{
   Word32 i;
   Word16 index ;

   {   /* if 2nd or 4th subframe */

      if (flag4 == 0)
		{
         /* 'normal' encoding: either with 5 or 6 bit resolution */
         /* index = 3*(T0 - T0_min) + 2 + T0_frac */
         i = (T0 - T0_min);
         index = 3*i + 2 + T0_frac;
      }
      else
		{
         /* encoding with 4 bit resolution */
         //tmp_lag = T0_prev;

			T0_min = T0_min + 5;
			T0_max = T0_max - 4;

         if ( T0_prev > T0_min)
            T0_prev = T0_min;

         if ( T0_prev < T0_max)
            T0_prev = T0_max;

         T0_min   = T0 + T0+ T0+ T0_frac;
			T0_max = 3*T0_prev - 6;

         if ( T0_max >= T0_min)
			{
            index =  T0 - T0_prev + 5;
         }
         else
			{
				i = T0_max + 9;
            if ( i > T0_min )
				{
                index = T0_min - T0_max+ 3;
            }
            else
				{
               index = T0 - T0_prev+ 11;
            }
         }

      } /* end if (encoding with 4 bit resolution) */
   }   /* end if (second of fourth subframe) */

   return index;

}


/*************************************************************************
 *
 *   FUNCTION:  Enc_lag6
 *
 *   PURPOSE:  Encoding of fractional pitch lag with 1/6 resolution.
 *
 *   DESCRIPTION:
 *                    First and third subframes:
 *                    --------------------------
 *   The pitch range is divided as follows:
 *           17 3/6  to   94 3/6   resolution 1/6
 *           95      to   143      resolution 1
 *
 *   The period is encoded with 9 bits.
 *   For the range with fractions:
 *     index = (T-17)*6 + frac - 3;
 *                         where T=[17..94] and frac=[-2,-1,0,1,2,3]
 *   and for the integer only range
 *     index = (T - 95) + 463;        where T=[95..143]
 *
 *                    Second and fourth subframes:
 *                    ----------------------------
 *   For the 2nd and 4th subframes a resolution of 1/6 is always used,
 *   and the search range is relative to the lag in previous subframe.
 *   If t0 is the lag in the previous subframe then
 *   t_min=t0-5   and  t_max=t0+4   and  the range is given by
 *       (t_min-1) 3/6   to  (t_max) 3/6
 *
 *   The period in the 2nd (and 4th) subframe is encoded with 6 bits:
 *     index = (T-(t_min-1))*6 + frac - 3;
 *                 where T=[t_min-1..t_max] and frac=[-2,-1,0,1,2,3]
 *
 *   Note that only 61 values are used. If the decoder receives 61, 62,
 *   or 63 as the relative pitch index, it means that a transmission
 *   error occurred and the pitch from previous subframe should be used.
 *
 *************************************************************************/
Word16 Enc_lag6 (        /* o : Return index of encoding             */
    Word32 T0,           /* i : Pitch delay                          */
    Word32 T0_frac,      /* i : Fractional pitch delay               */
    Word32 T0_min,       /* i : minimum of search range              */
    Word32 delta_flag    /* i : Flag for 1st (or 3rd) subframe       */
)
{
    Word16 index;

    if (delta_flag == 0)          /* if 1st or 3rd subframe */
    {
       /* encode pitch delay (with fraction) */
       if ( T0 <= 94)
       {
          /* index = T0*6 - 105 + T0_frac */
          index = T0*6 - 105 + T0_frac;
       }
       else
       {
          index = T0 + 368;
       }
    }
    else
    /* if second or fourth subframe */
    {
       /* index = 6*(T0-T0_min) + 3 + T0_frac  */
       index =  6*(T0 - T0_min) + 3 + T0_frac;
    }

    return index;
}



/*************************************************************************
 *
 *  FUNCTION:   Norm_Corr()
 *
 *  PURPOSE: Find the normalized correlation between the target vector
 *           and the filtered past excitation.
 *
 *  DESCRIPTION:
 *     The normalized correlation is given by the correlation between the
 *     target and filtered past excitation divided by the square root of
 *     the energy of filtered excitation.
 *                   corr[k] = <x[], y_k[]>/sqrt(y_k[],y_k[])
 *     where x[] is the target vector and y_k[] is the filtered past
 *     excitation at delay k.
 *
 *************************************************************************/
static void Norm_Corr (Word16 exc[], Word16 xn[], Word16 h[],
                       Word32 t_min, Word32 t_max, Word16 corr_norm[])
{

   /* Usally dynamic allocation of (L_subfr) */
   Word32 i, j;
   Word32 scaling;
	Word32 *ptr32, *ptr32_1;
   Word32 corr, norm , s;
   Word32 excf[L_SUBFR/2];
	Word16 *ptr16;

   exc = exc - t_min;
   /* compute the filtered excitation for the first delay t_min */

   Convolve (exc, h, (Word16 *)excf);

   /* Compute 1/sqrt(energy of excf[]) */
   s = 0;
	ptr32 = (Word32 *)excf;

#if (AMR_OPT_VER == 2)

	j = L_SUBFR/2;
	do
	{
      s = qdadd (s, smulbb(*ptr32, *ptr32));
      s = qdadd (s, smultt(*ptr32, *ptr32));
		ptr32++;
   }while(--j != 0);

#elif (AMR_OPT_VER == 1)

	j = L_SUBFR/2;
	do
	{
      s = smlabb(*ptr32, *ptr32 ,s);
      s = smlatt(*ptr32, *ptr32 ,s);
		ptr32++;
   }while(--j != 0);
	s = qadd(s,s);

#endif

   if ( s <= 67108864L ) {            /* if (s <= 2^26) */
		scaling = 12;
   }
   else
	{
	   /* scale "excf[]" to avoid overflow */
		ptr16 = (Word16 *)excf;
		j=L_SUBFR/2;
		do
		{
			*ptr16 = *ptr16 >> 2 ; ptr16++;
			*ptr16 = *ptr16 >> 2 ; ptr16++;
	   }while(--j != 0);

      /* "excf[]" is divided by 2 */
		scaling = 14;
   }

   /* loop for every possible period */
   for (i = t_min; i <= t_max; i++)
	{
		register Word32 t0,t1,t2,t3;

      /* Compute 1/sqrt(energy of excf[]) */
      /* Compute correlation between xn[] and excf[] */

#if (AMR_OPT_VER == 2)
		norm = 0; 		corr = 0;
		ptr32   = (Word32 *) excf;
		ptr32_1 = (Word32 *) xn;
		j = L_SUBFR/4;
		do
		{
			t0 = *ptr32++ ; t1 =  *ptr32_1++ ;
			t2 = *ptr32++ ; t3 =  *ptr32_1++ ;
			norm = qdadd(norm,smulbb(t0,t0));
			norm = qdadd(norm,smultt(t0,t0));
			corr = qdadd(corr,smulbb(t1,t0));
			corr = qdadd(corr,smultt(t1,t0));
			norm = qdadd(norm,smulbb(t2,t2));
			norm = qdadd(norm,smultt(t2,t2));
			corr = qdadd(corr,smulbb(t3,t2));
			corr = qdadd(corr,smultt(t3,t2));
		}while(--j != 0 );

#elif (AMR_OPT_VER == 1)

		norm = 0; 		corr = 0;
		ptr32   = (Word32 *) excf;
		ptr32_1 = (Word32 *) xn;
		j = L_SUBFR/4;
		do
		{
			t0 = *ptr32++ ; t1 =  *ptr32_1++ ;
			t2 = *ptr32++ ; t3 =  *ptr32_1++ ;
			norm = smlabb(t0,t0,norm);
			norm = smlatt(t0,t0,norm);
			corr = smlabb(t1,t0,corr);
			corr = smlatt(t1,t0,corr);

			norm = smlabb(t2,t2,norm);
			norm = smlatt(t2,t2,norm);
			corr = smlabb(t3,t2,corr);
			corr = smlatt(t3,t2,corr);
		}while(--j != 0 );
		norm = qadd(norm,norm);
		corr = qadd(corr,corr);

#endif

      norm = Inv_sqrt (norm);

      /* Normalize correlation = correlation * (1/sqrt(energy)) */

#if (AMR_OPT_VER == 2)
		s  = Mpy_32_o(L_Extract_o(norm), L_Extract_o(corr));
      *corr_norm++ = (Word16)(L_shl(s, 16)>>16);
#elif (AMR_OPT_VER == 1)
   	s  = DPF_Mult( norm, corr );
      *corr_norm++ = (Word16)(L_shl(s, 16)>>16);
#endif


      /* modify the filtered excitation excf[] for the next iteration */
      if ( i != t_max )
		{
         t0 = *(--exc);                            // t0 = *--exc
         ptr16 = (Word16 *)excf;                             // ptr16 = excf
         ptr32 = (Word32 *)h;                      // ptr32 = h

         t1 = *ptr16;                              // t1 = excf[0]
         *ptr16 = (Word16) (t0 >> (scaling-12));   // excf[0] = *exc >> scaling;
         t2 = *ptr32++;                            // t2 = h
         s = smulbt( t0, t2 );                  // s = L_mult (*exc, h[j]);
         s = (s >> scaling) + t1;			   // s = (Word16)(s>>16) + excf[j - 1]
         t1 = *(++ptr16);                           // t1 = ptr16 = excf
			*ptr16 = (Word16)s;                    // excf[j] = s;

         j = (L_SUBFR - 1 - 1)/2;
         do
			{
            t2 = *ptr32++;                         // t2 = h
            s = smulbb( t0, t2 );               // s = L_mult (*exc, h[j]);
            s = (s >> scaling) + t1;         // s = (Word16)(s>>16) + excf[j - 1]
            t1 = *(++ptr16);                       // t1 = ptr16 = excf

				*ptr16 = (Word16)s;                 // excf[j] = s;
            s = smulbt( t0, t2 );               // s = L_mult (*exc, h[j]);
            s = (s >> scaling) + t1;         // s = (Word16)(s>>16) + excf[j - 1]
            t1 = *(++ptr16);                       // t1 = ptr16 = excf
				*ptr16 = (Word16)s;                 // excf[j] = s;
         } while( --j > 0 );

      }


   }

}

/*************************************************************************
 *
 *  FUNCTION:   searchFrac()
 *
 *  PURPOSE: Find fractional pitch
 *
 *  DESCRIPTION:
 *     The function interpolates the normalized correlation at the
 *     fractional positions around lag T0. The position at which the
 *     interpolation function reaches its maximum is the fractional pitch.
 *     Starting point of the search is frac, end point is last_frac.
 *     frac is overwritten with the fractional pitch.
 *
 *************************************************************************/
static void searchFrac (
    Word32 *lag,       /* i/o : integer pitch           */
    Word32 *frac,      /* i/o : start point of search -
                                fractional pitch        */
    Word32 last_frac,  /* i   : endpoint of search      */
    Word16 corr[],     /* i   : normalized correlation  */
    Word32 flag3       /* i   : subsample resolution
                                (3: =1 / 6: =0)         */
)
{
   Word32 i;
   Word16 max;
   Word16 corr_int;

   /* Test the fractions around T0 and choose the one which maximizes   */
   /* the interpolated normalized correlation.                          */
   max = Interpol_3or6 (&corr[*lag], *frac, flag3);

   for (i = (*frac + 1); i <= last_frac; i++)
	{
      corr_int = Interpol_3or6 (&corr[*lag], i, flag3);

      if ( corr_int > max )
		{
         max = corr_int;
         *frac = i;
      }
   }

   if (flag3 == 0) {
      /* Limit the fraction value in the interval [-2,-1,0,1,2,3] */
      if (*frac == -3)
		{
         *frac = 3;
         *lag  -= 1;
      }
   }
   else {
      /* limit the fraction value between -1 and 1 */
      if (*frac == -2)
		{
         *frac = 1;
         *lag -= 1;
      }

      if (*frac == 2)
		{
         *frac = -1;
         *lag += 1;
      }
   }
}

/*************************************************************************
 *
 *  FUNCTION:   getRange()
 *
 *  PURPOSE: Sets range around open-loop pitch or integer pitch of last subframe
 *
 *  DESCRIPTION:
 *     Takes integer pitch T0 and calculates a range around it with
 *       t0_min = T0-delta_low  and t0_max = (T0-delta_low) + delta_range
 *     t0_min and t0_max are bounded by pitmin and pitmax
 *
 *************************************************************************/
static void getRange (
    Word16 T0,           /* i : integer pitch          */
    Word32 delta_low,    /* i : search start offset    */
    Word32 delta_range,  /* i : search range           */
    Word32 pitmin,       /* i : minimum pitch          */
    Word32 *t0_min,      /* o : search range minimum   */
    Word32 *t0_max)      /* o : search range maximum   */
{
   *t0_min = (T0 - delta_low);

   if(*t0_min < pitmin)
	{
      *t0_min = pitmin;
   }

   *t0_max = (*t0_min + delta_range);

   if(*t0_max > PIT_MAX)
	{
      *t0_max = PIT_MAX;
      *t0_min = (*t0_max - delta_range);
   }
}

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *
 *  FUNCTION:   Pitch_fr()
 *
 *  PURPOSE: Find the pitch period with 1/3 or 1/6 subsample resolution
 *           (closed loop).
 *
 *  DESCRIPTION:
 *        - find the normalized correlation between the target and filtered
 *          past excitation in the search range.
 *        - select the delay with maximum normalized correlation.
 *        - interpolate the normalized correlation at fractions -3/6 to 3/6
 *          with step 1/6 around the chosen delay.
 *        - The fraction which gives the maximum interpolated value is chosen.
 *
 *************************************************************************/

void Pitch_fr_reset (cod_amrState *st)
{
   st->T0_prev_subframe = 0;
   return;
}


Word16 Pitch_fr (        /* o   : pitch period (integer)                    */
   AMR_Encode_FrameState *Enc,
	 Word16 *T0_prev_subframe,
    enum Mode mode,      /* i   : codec mode                                */
    Word16 T_op[],       /* i   : open loop pitch lags                      */
    Word16 i_subfr,      /* i   : subframe offset                           */
    Word16 *pit_frac,    /* o   : pitch period (fractional)                 */
    Word16 *resu3,       /* o   : subsample resolution 1/3 (=1) or 1/6 (=0) */
    Word16 *ana_index    /* o   : index of encoding                         */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i;
   Word32 t_min, t_max;
   Word32 t0_min, t0_max , lag, frac ;
   Word32 tmp_lag;
   Word32 max_frac_lag;
   Word32 flag3, flag4;
   Word32 last_frac;
   Word32 delta_int_low, delta_int_range;
   Word32 delta_frc_low, delta_frc_range;
   Word32 pit_min;
   Word32 frame_offset;
   Word32 delta_search;
   Word16 max;
   Word16 *corr;
   Word16 *corr_v = Enc_share->shabuf.stE.u2E.corr_v;    /* Total length = t0_max-t0_min+1+2*L_INTER_SRCH */

   msr_cpsr_f_0();
   /*-----------------------------------------------------------------------*
   *                      set mode specific variables                      *
   *-----------------------------------------------------------------------*/
   max_frac_lag    = mode_dep_parm[mode].max_frac_lag;
   flag3           = mode_dep_parm[mode].flag3;
   frac            = mode_dep_parm[mode].first_frac;
   last_frac       = mode_dep_parm[mode].last_frac;
   delta_int_low   = mode_dep_parm[mode].delta_int_low;
   delta_int_range = mode_dep_parm[mode].delta_int_range;

   delta_frc_low   = mode_dep_parm[mode].delta_frc_low;
   delta_frc_range = mode_dep_parm[mode].delta_frc_range;
   pit_min         = mode_dep_parm[mode].pit_min;
   /*-----------------------------------------------------------------------*
   *                 decide upon full or differential search               *
   *-----------------------------------------------------------------------*/
   delta_search = 1;
	if ((i_subfr == 0) || (i_subfr==L_FRAME_BY2))
	{
      /* Subframe 1 and 3 */
      if ( ( (mode!=MR475) && (mode!=MR515) ) || (i_subfr != L_FRAME_BY2)  )
      {
         /* set t0_min, t0_max for full search */
         /* this is *not* done for mode MR475, MR515 in subframe 3 */
         delta_search = 0; /* no differential search */

         /* calculate index into T_op which contains the open-loop */
         /* pitch estimations for the 2 big subframes */
         frame_offset = 1;

         if (i_subfr == 0)
            frame_offset = 0;

         /* get T_op from the corresponding half frame and */
         /* set t0_min, t0_max */
         getRange (T_op[frame_offset], delta_int_low, delta_int_range,
            pit_min, &t0_min, &t0_max);

      }
      else {

         /* mode MR475, MR515 and 3. Subframe: delta search as well */
         getRange (*T0_prev_subframe, delta_frc_low, delta_frc_range,
            pit_min, &t0_min, &t0_max);
      }
   }
   else
	{
      /* for Subframe 2 and 4 */
      /* get range around T0 of previous subframe for delta search */      
      getRange (*T0_prev_subframe, delta_frc_low, delta_frc_range,
         pit_min, &t0_min, &t0_max);
   }

   /*-----------------------------------------------------------------------*
   *           Find interval to compute normalized correlation             *
   *-----------------------------------------------------------------------*/

   t_min =  (t0_min - L_INTER_SRCH);
   t_max =  (t0_max + L_INTER_SRCH);

   corr = &corr_v[-t_min];

   /*-----------------------------------------------------------------------*
   * Compute normalized correlation between target and filtered excitation *
   *-----------------------------------------------------------------------*/

//   Norm_Corr (exc, xn, h, t_min, t_max, corr);
   Norm_Corr(&(Enc->cod_amr_state.exc[i_subfr]), Enc_share->shabuf.stE.xn,
   				Enc->cod_amr_state.h1, t_min, t_max, corr_v);


   /*-----------------------------------------------------------------------*
   *                           Find integer pitch                          *
   *-----------------------------------------------------------------------*/
   max = corr[t0_min];
   lag = t0_min;
   for (i = t0_min + 1; i <= t0_max; i++)
	{
      if ( corr[i] >= max )
		{
         max = corr[i];
         lag = i;
      }
   }
   /*-----------------------------------------------------------------------*
   *                        Find fractional pitch                          *
   *-----------------------------------------------------------------------*/
   if ( (delta_search == 0) &&  (lag > max_frac_lag) )
	{
      /* full search and integer pitch greater than max_frac_lag */
      /* fractional search is not needed, set fractional to zero */
      frac = 0;
   }
   else {

      /* if differential search AND mode MR475 OR MR515 OR MR59 OR MR67   */
      /* then search fractional with 4 bits resolution           */
      if ((delta_search != 0) &&
         ((  mode == MR475 ) ||
         (  mode == MR515 ) ||
         (  mode == MR59 ) ||
         (  mode == MR67 )))
      {
         /* modify frac or last_frac according to position of last */
         /* integer pitch: either search around integer pitch, */
         /* or only on left or right side */

         tmp_lag = *T0_prev_subframe;

         if( tmp_lag > t0_min + 5)
            tmp_lag = (t0_min + 5);

         if (  t0_max > tmp_lag + 4)
            tmp_lag = (t0_max - 4);


         if ( (lag == tmp_lag) || ( lag == (tmp_lag-1) ))
         {
            /* normal search in fractions around T0 */            
            searchFrac (&lag, &frac, last_frac, corr, flag3);
         }
         else if ( lag == (tmp_lag-2) )
			{
            /* limit search around T0 to the right side */
            frac = 0;
            searchFrac (&lag, &frac, last_frac, corr, flag3);
         }
         else if ( lag == (tmp_lag + 1) )
			{
            /* limit search around T0 to the left side */
            last_frac = 0;
            searchFrac (&lag, &frac, last_frac, corr, flag3);
         }
         else
			{
            /* no fractional search */
            frac = 0;
         }
      }
      else
         /* test the fractions around T0 */
         searchFrac (&lag, &frac, last_frac, corr, flag3);
   }

   /*-----------------------------------------------------------------------*
   *                           encode pitch                                *
   *-----------------------------------------------------------------------*/

   if (flag3 != 0)
	{
      /* flag4 indicates encoding with 4 bit resolution;         */
      /* this is needed for mode MR475, MR515 and MR59           */
      flag4 = 0;

      if ( ( mode == MR475 ) ||
         ( mode == MR515 ) ||
         ( mode == MR59 ) ||
         ( mode == MR67 ) )
      {
         flag4 = 1;
      }

      /* encode with 1/3 subsample resolution */
		if(delta_search == 0)
		{/* if 1st or 3rd subframe */
		 /* encode pitch delay (with fraction) */
			*ana_index = (Word16)( (lag<=85) ? 3*lag-58+frac : lag+112 );  /* index = T0*3 - 58 + T0_frac   */
		}
		else
		{
	      *ana_index = Enc_lag3(lag, frac, *T0_prev_subframe,
   	      t0_min, t0_max, flag4);
		}

   }
   else
   {
      /* encode with 1/6 subsample resolution */
      *ana_index = Enc_lag6(lag, frac, t0_min, delta_search);
   }

   /*-----------------------------------------------------------------------*
   *                          update state variables                       *
   *-----------------------------------------------------------------------*/

   *T0_prev_subframe = lag;

   /*-----------------------------------------------------------------------*
   *                      update output variables                          *
   *-----------------------------------------------------------------------*/

   *resu3    = flag3;

   *pit_frac = frac;

   return (lag);
}

#endif

