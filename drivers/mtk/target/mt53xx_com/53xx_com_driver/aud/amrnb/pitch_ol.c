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
 *   pitch_ol.c
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
*      File             : pitch_ol.c
*      Purpose          : Compute the open loop pitch lag.
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "pitch_ol.h"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "vad.h"
#include "calc_cor.h"
#include "hp_max.h"
 
/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define THRESHOLD 27853

/*
********************************************************************************
*                         LOCAL PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  Lag_max
 *
 *  PURPOSE: Find the lag that has maximum correlation of scal_sig[] in a
 *           given delay range.
 *
 *  DESCRIPTION:
 *      The correlation is given by
 *           cor[t] = <scal_sig[n],scal_sig[n-t]>,  t=lag_min,...,lag_max
 *      The functions outputs the maximum correlation after normalization
 *      and the corresponding lag.
 *
 *************************************************************************/

static Word16 Lag_max ( /* o   : lag found                               */
    vadState *vadSt,    /* i/o : VAD state struct                        */
    Word32 corr[],      /* i   : correlation vector.                     */
    Word16 scal_sig[],  /* i   : scaled signal.                          */
    Word32 scal_fac,    /* i   : scaled signal factor.                   */
    Word32 scal_flag,   /* i   : if 1 use EFR compatible scaling         */
    Word32 L_frame,     /* i   : length of frame to compute pitch        */
    Word32 lag_max,     /* i   : maximum lag                             */
    Word32 lag_min,     /* i   : minimum lag                             */
    Word16 *cor_max,    /* o   : normalized correlation of selected lag  */
    Flag dtx            /* i   : dtx flag; use dtx=1, do not use dtx=0   */
    )
{
   Word32 i;
   Word32 temp,*p1;
	Word32 max, t0;
   Word32 p_max = 0; /* initialization only needed to keep gcc silent */
   Word16 *p;

   max = MIN_32;
   p_max = lag_max;
   corr -= lag_max;

   for (i = lag_max; i >= lag_min; i--)
   {
      if ( *corr >= max )
      {
         max = *corr;
         p_max = i;
      }
      corr++;
   }

   /* compute energy */
   t0 = 0;
   p =  &scal_sig[-p_max];
   p1 = (Word32*) &scal_sig[-p_max];

	if(_check_word_align(p1)!=0)
	{
	   i = L_frame/4;
   	do{
	      temp = *p++;
			t0 = qdadd(t0, smulbb(temp, temp));
      	temp = *p++;
			t0 = qdadd(t0, smulbb(temp, temp));
   	   temp = *p++;
			t0 = qdadd(t0, smulbb(temp, temp));
	      temp = *p++;
			t0 = qdadd(t0, smulbb(temp, temp));
	   }while( --i != 0 );
	}
	else
   {
	   i = (L_frame/4);
   	do{

	      temp = *p1++;
			t0 = qdadd(t0, smulbb(temp, temp));
			t0 = qdadd(t0, smultt(temp, temp));

   	   temp = *p1++;
			t0 = qdadd(t0, smulbb(temp, temp));
			t0 = qdadd(t0, smultt(temp, temp));

	   }while( --i != 0 );
   }



   /* 1/sqrt(energy) */

   if (dtx)
   {  /* no test() call since this if is only in simulation env */
      /* check tone */
      vad_tone_detection (vadSt, max, t0);
   }

   t0 = Inv_sqrt (t0);  /* function result */

   if (scal_flag)
   {
      t0 = L_shl (t0, 1);
   }

    /* max = max/sqrt(energy)  */

#if (AMR_OPT_VER == 2)
	t0  = Mpy_32_o(L_Extract_o(max), L_Extract_o(t0));
#elif (AMR_OPT_VER == 1)
   t0  = DPF_Mult( max, t0 );
#endif


   if (scal_flag)
   {
   	t0 = L_shr (t0, scal_fac);
     	*cor_max = (Word16) ( (t0) >> 1); /* divide by 2 */
   }
   else
   {
      *cor_max = (Word16)(t0);
   }

    return (p_max);
}

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  Pitch_ol
 *
 *  PURPOSE: Compute the open loop pitch lag.
 *
 *  DESCRIPTION:
 *      The open-loop pitch lag is determined based on the perceptually
 *      weighted speech signal. This is done in the following steps:
 *        - find three maxima of the correlation <sw[n],sw[n-T]>,
 *          dividing the search range into three parts:
 *               pit_min ... 2*pit_min-1
 *             2*pit_min ... 4*pit_min-1
 *             4*pit_min ...   pit_max
 *        - divide each maximum by <sw[n-t], sw[n-t]> where t is the delay at
 *          that maximum correlation.
 *        - select the delay of maximum normalized correlation (among the
 *          three candidates) while favoring the lower delay ranges.
 *
 *************************************************************************/
Word16 Pitch_ol (      /* o   : open loop pitch lag                         */
   AMR_Encode_FrameState *Enc,
   vadState *vadSt,   /* i/o : VAD state struct                            */
   enum Mode mode,    /* i   : coder mode                                  */
   Word16 signal[],   /* i   : signal used to compute the open loop pitch  */
                      /*    signal[-pit_max] to signal[-1] should be known */
   Word32 pit_min,    /* i   : minimum pitch lag                           */
   Word32 L_frame,    /* i   : length of frame to compute pitch            */
   Word16 idx,        /* i   : frame index                                 */
   Flag dtx           /* i   : dtx flag; use dtx=1, do not use dtx=0       */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i;
   Word32 p_max1, p_max2, p_max3;
   Word32 scal_flag = 0 , scal_fac;
   Word32 t0;
	Word32 *corr_ptr;
//   Word32 corr[PIT_MAX+1];
   Word32 *corr = Enc_share->shabuf.stE.u2E.s5E.corr;

   /* Scaled signal */
   Word32 *sig_ptr,temp;

   Word16 max1, max2, max3;
   Word16 corr_hp_max;
//   Word16 scaled_signal[L_FRAME + PIT_MAX+1];
   Word16 *scaled_signal = Enc_share->shabuf.stE.u2E.s5E.scaled_signal;


   Word16 *scal_sig;
   Word16 *sig_ptr16,*scalsig_ptr16;


   if (dtx)
   {  /* no test() call since this if is only in simulation env */
      /* update tone detection */

      if ( (mode == MR475) || (mode == MR515) )
      {
         vad_tone_detection_update (vadSt, 1);
      }
      else
      {
         vad_tone_detection_update (vadSt, 0);
      }
   }

   sig_ptr = (Word32*)(signal - PIT_MAX - 1);

/*
#if (AMR_OPT_VER == 2)
*/
	t0 = qdadd( 0, smultt( *sig_ptr, *sig_ptr ));
   i = ((L_frame + PIT_MAX - 1) / 2)-1;
   sig_ptr++;
   do {
      temp = *sig_ptr++;
      t0 = qdadd(t0, smulbb(temp, temp));
      t0 = qdadd(t0, smultt(temp, temp));
   } while( --i >= 0 );
/*
#elif (AMR_OPT_VER == 1)

	t0 = smlatt( *sig_ptr, *sig_ptr, 0 );
   i = ((L_frame + PIT_MAX - 1) / 2)-1;
   sig_ptr++;
   do {
      temp = *sig_ptr++;
      t0 = smlabb(temp, temp , t0);
      t0 = smlatt(temp, temp , t0);
   } while( --i >= 0 );

	t0 = qadd(t0,t0);

#endif
*/
    /*--------------------------------------------------------*
     * Scaling of input signal.                               *
     *                                                        *
     *   if Overflow        -> scal_sig[i] = signal[i]>>3     *
     *   else if t0 < 1^20  -> scal_sig[i] = signal[i]<<3     *
     *   else               -> scal_sig[i] = signal[i]        *
     *--------------------------------------------------------*/

    /*--------------------------------------------------------*
     *  Verification for risk of overflow.                    *
     *--------------------------------------------------------*/

	scal_sig = &scaled_signal[PIT_MAX+1];

	if (t0 == MAX_32)
   {
		sig_ptr16     = signal   - PIT_MAX -1;
		scalsig_ptr16 = scal_sig - PIT_MAX -1;

		i= (PIT_MAX + L_frame +1)/4 ;
		do{
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) >> 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) >> 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) >> 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) >> 3);
		}while(--i != 0);

		scal_fac = 3;
    }

    else if (t0 < 1048576L)
        /* if (t0 < 2^20) */
    {
		sig_ptr16     = signal   - PIT_MAX -1;
		scalsig_ptr16 = scal_sig - PIT_MAX -1;

		i= (PIT_MAX + L_frame +1)/4 ;
		do{
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) << 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) << 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) << 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) << 3);
		}while(--i != 0);

		scal_fac = -3;
	}
   else
   {
		scal_sig = signal;
		scal_fac = 0;
   }

    /* calculate all coreelations of scal_sig, from pit_min to pit_max */
    corr_ptr = &corr[PIT_MAX];

    comp_corr (scal_sig, L_frame, pit_min, corr_ptr);

    /*--------------------------------------------------------------------*
     *  The pitch lag search is divided in three sections.                *
     *  Each section cannot have a pitch multiple.                        *
     *  We find a maximum for each section.                               *
     *  We compare the maximum of each section by favoring small lags.    *
     *                                                                    *
     *  First section:  lag delay = pit_max     downto 4*pit_min          *
     *  Second section: lag delay = 4*pit_min-1 downto 2*pit_min          *
     *  Third section:  lag delay = 2*pit_min-1 downto pit_min            *
     *--------------------------------------------------------------------*/

    /* mode dependent scaling in Lag_max */
/*
    if ( mode == MR122)
    {
       scal_flag = 1;
    }
    else
    {
       scal_flag = 0;
    }
*/
	scal_flag = ((mode == MR122) ? 1:0 );


    p_max1 = Lag_max (vadSt, corr_ptr, scal_sig, scal_fac, scal_flag, L_frame,
                      PIT_MAX, pit_min << 2 , &max1, dtx);

    p_max2 = Lag_max (vadSt, corr_ptr, scal_sig, scal_fac, scal_flag, L_frame,
                      (pit_min << 2)-1 , pit_min << 1 , &max2, dtx);

    p_max3 = Lag_max (vadSt, corr_ptr, scal_sig, scal_fac, scal_flag, L_frame,
                      (pit_min << 1)-1, pit_min, &max3, dtx);

    if (dtx)
    {  /* no test() call since this if is only in simulation env */

       if ( idx == 1)
       {
          /* calculate max high-passed filtered correlation of all lags */
          hp_max (corr_ptr, scal_sig, L_frame, PIT_MAX, pit_min, &corr_hp_max);

          /* update complex background detector */
          vad_complex_detection_update(vadSt, corr_hp_max);
       }
    }


    /*--------------------------------------------------------------------*
     * Compare the 3 sections maximum, and favor small lag.               *
     *--------------------------------------------------------------------*/


    if ( ((max1*THRESHOLD)>>15) < max2 )
    {
        max1 = max2;
        p_max1 = p_max2;
    }

    if ( ((max1*THRESHOLD)>>15) < max3 )
    {
        p_max1 = p_max3;
    }


    return (p_max1);
}

#endif

