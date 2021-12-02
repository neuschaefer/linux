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
 *   p_ol_wgh.c
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
*      File             : p_ol_wgh.c
*      Purpose          : Compute the open loop pitch lag with weighting
*
*************************************************************************/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "p_ol_wgh.h"

/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "gmed_n.h"
#include "vad.h"
#include "calc_cor.h"
#include "hp_max.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/

/*****************************************************************************
 *                Tables
 *****************************************************************************
*/
static const Word16 corrweight[251] = {
	20473,	20506,	20539,	20572,	20605,	20644,	20677,
	20716,	20749,	20788,	20821,	20860,	20893,	20932,
	20972,	21011,	21050,	21089,	21129,	21168,	21207,
	21247,	21286,	21332,	21371,	21417,	21456,	21502,
	21542,	21588,	21633,	21679,	21725,	21771,	21817,
	21863,	21909,	21961,	22007,	22059,	22105,	22158,
	22210,	22263,	22315,	22367,	22420,	22472,	22531,
	22584,	22643,	22702,	22761,	22820,	22879,	22938,
	23003,	23062,	23128,	23193,	23252,	23324,	23390,
	23455,	23527,	23600,	23665,	23744,	23816,	23888,
	23967,	24045,	24124,	24202,	24288,	24366,	24451,
	24537,	24628,	24714,	24805,	24904,	24995,	25094,
	25192,	25297,	25395,	25500,	25611,	25723,	25834,
	25952,	26070,	26188,	26313,	26444,	26575,	26706,
	26844,	26988,	27132,	27283,	27440,	27597,	27761,
	27931,	28108,	28285,	28475,	28665,	28869,	29078,
	29295,	29524,	29760,	30002,	30258,	30527,	30808,
	31457,	32767,	32767,	32767,	32767,	32767,	32767,
	32767,	31457,	30808,	30527,	30258,	30002,	29760,
	29524,	29295,	29078,	28869,	28665,	28475,	28285,
	28108,	27931,	27761,	27597,	27440,	27283,	27132,
	26988,	26844,	26706,	26575,	26444,	26313,	26188,
	26070,	25952,	25834,	25723,	25611,	25500,	25395,
	25297,	25192,	25094,	24995,	24904,	24805,	24714,
	24628,	24537,	24451,	24366,	24288,	24202,	24124,
	24045,	23967,	23888,	23816,	23744,	23665,	23600,
	23527,	23455,	23390,	23324,	23252,	23193,	23128,
	23062,	23003,	22938,	22879,	22820,	22761,	22702,
	22643,	22584,	22531,	22472,	22420,	22367,	22315,
	22263,	22210,	22158,	22105,	22059,	22007,	21961,
	21909,	21863,	21817,	21771,	21725,	21679,	21633,
	21588,	21542,	21502,	21456,	21417,	21371,	21332,
	21286,	21247,	21207,	21168,	21129,	21089,	21050,
	21011,	20972,	20932,	20893,	20860,	20821,	20788,
	20749,	20716,	20677,	20644,	20605,	20572,	20539,
	20506,	20473,	20434,	20401,	20369,	20336};


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
static Word32 Lag_max ( /* o : lag found                               */
    vadState *vadSt,    /* i/o : VAD state struct                      */
    Word32 corr[],      /* i   : correlation vector.                   */
    Word16 scal_sig[],  /* i : scaled signal.                          */
    Word16 old_lag,     /* i : old open-loop lag                       */

    Word16 wght_flg,    /* i : is weighting function used              */
    Word16 *gain_flg,   /* o : open-loop flag                          */
    Flag dtx            /* i   : dtx flag; use dtx=1, do not use dtx=0 */
    )
{
   Word32 i, j;
   Word32 p_max;
   Word32 max, t0, t1;
	Word32 *p_32;
   Word16 *p1;
   const Word16 *ww, *we;

   ww = &corrweight[250];
   we = &corrweight[123 + PIT_MAX - old_lag];
   max = MIN_32;
   p_max = PIT_MAX;
	corr -= PIT_MAX;

	i = PIT_MAX;
	do
	{
		t0 = *corr++;
      /* Weighting of the correlation function.   */
#if (AMR_OPT_VER == 2)
      t0 = L_Extract_o(t0);
		t0 = Mpy_32_16_o(t0,*ww); //t0 = Mpy_32_16_o(t0,(const Word32)*ww);
#elif (AMR_OPT_VER == 1)
      t0 = DPF_Mult16(t0, *ww); //t0 = DPF_Mult16(t0, (const Word32)*ww);
#endif
      ww--;

      if (wght_flg > 0)
		{
         /* Weight the neighbourhood of the old lag. */

#if (AMR_OPT_VER == 2)
			t0 = L_Extract_o(t0);
			t0 = Mpy_32_16_o(t0,*we); //t0 = Mpy_32_16_o(t0,(const Word32)*we);
#elif (AMR_OPT_VER == 1)
         t0 = DPF_Mult16(t0, *we); //t0 = DPF_Mult16(t0, (const Word32)*we);
#endif
         we--;
      }

      if ( t0 >= max )
      {
         max = t0;
         p_max = i;
		}

	}while(--i >= PIT_MIN);

	p_32 = (Word32 *) &scal_sig[0];
   p1 = &scal_sig[-p_max];
   t0 = t1 = 0;

//#if (AMR_OPT_VER == 2)

	j = L_FRAME_BY2/4;
	do
   {
		t0 = qdadd(t0 , smulbb(*p_32,*p1));
		t1 = qdadd(t1 , smulbb(*p1,*p1));
		p1++;
		t0 = qdadd(t0 , smultb(*p_32,*p1));
		t1 = qdadd(t1 , smulbb(*p1,*p1));
		p_32++, p1++;

		t0 = qdadd(t0 , smulbb(*p_32,*p1));
		t1 = qdadd(t1 , smulbb(*p1,*p1));
		p1++;
		t0 = qdadd(t0 , smultb(*p_32,*p1));
		t1 = qdadd(t1 , smulbb(*p1,*p1));
		p_32++, p1++;

   }while(--j != 0);
/*
#elif (AMR_OPT_VER == 1)
	j = L_FRAME_BY2/4;
	do
   {
		t0 =  smlabb(*p_32,*p1,t0);
		t1 =  smlabb(*p1,*p1,t1);
		p1++;
		t0 =  smlatb(*p_32,*p1,t0);
		t1 =  smlabb(*p1,*p1,t1);
		p_32++, p1++;

		t0 =  smlabb(*p_32,*p1,t0);
		t1 =  smlabb(*p1,*p1,t1);
		p1++;
		t0 =  smlatb(*p_32,*p1,t0);
		t1 =  smlabb(*p1,*p1,t1);
		p_32++, p1++;

   }while(--j != 0);
	t0 = qadd(t0,t0);
	t1 = qadd(t1,t1);
#endif
*/

	if (dtx)
   {  /* no test() call since this if is only in simulation env */
      /* update and detect tone */
      vad_tone_detection_update (vadSt, 0);
      vad_tone_detection (vadSt, t0, t1);
   }

   /* gain flag is set according to the open_loop gain */
   /* is t2/t1 > 0.4 ? */
//   *gain_flg = round(L_msu(t0, round(t1), 13107));
   *gain_flg = AMR_round( qdsub(t0, smulbb(AMR_round(t1), 13107)));



   return (p_max);
}

/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/
/*************************************************************************
*
*  Function:   p_ol_wgh_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
#if 0
int p_ol_wgh_init (pitchOLWghtState **state)
{
    pitchOLWghtState* s;
    *state = NULL;

    /* allocate memory */
    if ((s= (pitchOLWghtState *) malloc(  ((sizeof(pitchOLWghtState)+3)&(~3))  )) == NULL){
        fprintf(stderr, "p_ol_wgh_init: can not malloc state structure\n");
        return -1;
    }
    p_ol_wgh_reset(s);
    *state = s;
    return 0;
}
#endif

/*************************************************************************
*
*  Function:   p_ol_wgh_exit
*  Purpose:    The memory used for state memory is freed
*
**************************************************************************
*/
#if 0
void p_ol_wgh_exit (pitchOLWghtState **state)
{
	if (state == NULL || *state == NULL)
   	return;

	/* deallocate memory */
	free(*state);
	*state = NULL;

	return;
}
#endif

/*************************************************************************
*
*  Function:   p_ol_wgh_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int p_ol_wgh_reset (pitchOLWghtState *st)
{
   /* Reset pitch search states */
   st->old_T0_med = 40;
   st->ada_w = 0;
   st->wght_flg = 0;

   return 0;
}

/*************************************************************************
*
*  Function:   p_ol_wgh
*  Purpose:    open-loop pitch search with weighting
*
**************************************************************************
*/
Word32 Pitch_ol_wgh (     /* o   : open loop pitch lag                            */
   AMR_Encode_FrameState *Enc,

   pitchOLWghtState *st, /* i/o : State struct                                   */
   vadState *vadSt,      /* i/o : VAD state struct                               */
   Word16 signal[],      /* i   : signal used to compute the open loop pitch     */
                         /*       signal[-pit_max] to signal[-1] should be known */

   Word16 old_lags[],    /* i   : history with old stored Cl lags                */
   Word16 ol_gain_flg[], /* i   : OL gain flag                                   */
   Word16 idx,           /* i   : index                                          */
   Flag dtx              /* i   : dtx flag; use dtx=1, do not use dtx=0          */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i;
   Word32 t0;
	Word32 *corr_ptr;
//   Word32 corr[PIT_MAX+1];
   Word32 *corr = Enc_share->shabuf.stE.u2E.s5E.corr;

   Word32 *sig_ptr,temp;

   Word16 p_max1;
   Word16 corr_hp_max;

   /* Scaled signal */
//   Word16 scaled_signal[PIT_MAX + L_FRAME + 1];
   Word16 *scaled_signal = Enc_share->shabuf.stE.u2E.s5E.scaled_signal;

   Word16 *scal_sig;
   Word16 *sig_ptr16,*scalsig_ptr16;

   sig_ptr = (Word32*)(signal - PIT_MAX - 1);

/*
#if (AMR_OPT_VER == 2)
*/
	t0 = qdadd( 0, smultt( *sig_ptr, *sig_ptr ));
   i = ((L_FRAME_BY2 + PIT_MAX - 1) / 2)-1;
   sig_ptr++;
   do {
      temp = *sig_ptr++;
      t0 = qdadd(t0, smulbb(temp, temp));
      t0 = qdadd(t0, smultt(temp, temp));
   } while( --i >= 0 );
/*
#elif (AMR_OPT_VER == 1)
	t0 =  smlatt( *sig_ptr, *sig_ptr , 0 );
   i = ((L_FRAME_BY2 + PIT_MAX - 1) / 2)-1;
   sig_ptr++;
   do {
      temp = *sig_ptr++;
      t0 =  smlabb(temp, temp , t0);
      t0 =  smlatt(temp, temp , t0);
   } while( --i >= 0 );

	t0 = qadd(t0,t0);

#endif
*/

    /*--------------------------------------------------------*
     * Scaling of input signal.                               *
     *                                                        *
     *   if Overflow        -> scal_sig[i] = signal[i]>>2     *
     *   else if t0 < 1^22  -> scal_sig[i] = signal[i]<<2     *
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

		i= (PIT_MAX + L_FRAME_BY2 +1)/4 ;
		do{
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) >> 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) >> 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) >> 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) >> 3);
		}while(--i != 0);

    }

    else if (t0 < 1048576L)
        /* if (t0 < 2^20) */
    {
		sig_ptr16     = signal   - PIT_MAX -1;
		scalsig_ptr16 = scal_sig - PIT_MAX -1;

		i= (PIT_MAX + L_FRAME_BY2 +1)/4 ;
		do{
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) << 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) << 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) << 3);
			*scalsig_ptr16++ = (Word16)( (*sig_ptr16++) << 3);
		}while(--i != 0);

	}
   else
   {
		scal_sig = signal;
   }


    /* calculate all coreelations of scal_sig, from pit_min to pit_max */
    corr_ptr = &corr[PIT_MAX];

    comp_corr (scal_sig, L_FRAME_BY2, PIT_MIN, corr_ptr);

    p_max1 = Lag_max (vadSt, corr_ptr, scal_sig,st->old_T0_med,
	 	                st->wght_flg, &ol_gain_flg[idx],dtx);



    if (ol_gain_flg[idx] > 0)
    {
       /* Calculate 5-point median of previous lags */ /* Shift buffer */

       old_lags[4] = old_lags[3];
       old_lags[3] = old_lags[2];
       old_lags[2] = old_lags[1];
       old_lags[1] = old_lags[0];
       old_lags[0] = p_max1;

       st->old_T0_med = gmed_n (old_lags, 5);
       st->ada_w = 32767;                           /* Q15 = 1.0 */
       st->wght_flg = 1;
    }
    else
    {
       st->old_T0_med = p_max1;
       st->ada_w = (st->ada_w * 29491) >> 15;      /* = ada_w = ada_w * 0.9 */

	    if ( st->ada_w < 9830 )  /* ada_w - 0.3 */
   	 {
      	 st->wght_flg = 0;
	    }
   	 else
	    {
   	    st->wght_flg = 1;
	    }
    }


    if (dtx)
    {  /* no test() call since this if is only in simulation env */
       if ( idx == 1)
       {
          /* calculate max high-passed filtered correlation of all lags */
          hp_max (corr_ptr, scal_sig, L_FRAME_BY2, PIT_MAX, PIT_MIN, &corr_hp_max);

          /* update complex background detector */
          vad_complex_detection_update(vadSt, corr_hp_max);
       }
    }

    return (p_max1);
}

#endif

