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
 *   vad1.c
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
**-------------------------------------------------------------------------**
**                                                                         **
**     GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001       **
**                               R99   Version 3.3.0                       **
**                               REL-4 Version 4.1.0                       **
**                                                                         **
**-------------------------------------------------------------------------**
*****************************************************************************
*
*      File             : vad1.c
*      Purpose          : Voice Activity Detection (VAD) for AMR (option 1)
*
*****************************************************************************
*/

/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "vad.h"


/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst_vad.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/

/*
********************************************************************************
*                         PRIVATE PROGRAM CODE
********************************************************************************
*/
/****************************************************************************
 *
 *     Function     : first_filter_stage
 *     Purpose      : Scale input down by one bit. Calculate 5th order
 *                    half-band lowpass/highpass filter pair with
 *                    decimation.
 *
 ***************************************************************************/
static void first_filter_stage(Word16 in[],  /* i   : input signal                  */
                               Word16 out[], /* o   : output values, every other    */
                                             /*       output is low-pass part and   */
                                             /*       other is high-pass part every */
                               Word16 data[] /* i/o : filter memory                 */
                               )
{
   Word32 temp0, temp1, temp2, temp3, i;
   Word16 *ptr16in,*ptr16out;
   Word16 data0, data1;


   data0 = data[0];
   data1 = data[1];

   ptr16in  = in;
   ptr16out = out;

	i = FRAME_LEN/4;
	do
   {
      temp0 = ((*ptr16in++ >> 2)- (smulbb(COEFF5_1, data0)>>15));
      temp1 = (data0+ (smulbb(COEFF5_1, temp0)>>15));

      temp3 = ((*ptr16in++ >> 2)- (smulbb(COEFF5_2, data1)>>15));
      temp2 = (data1+ (smulbb(COEFF5_2, temp3)>>15));

      *ptr16out++ = (temp1+ temp2);
      *ptr16out++ = (temp1- temp2);

      data0 = ((*ptr16in++ >> 2)- (smulbb(COEFF5_1, temp0)>>15));
      temp1 = (temp0 + (smulbb(COEFF5_1, data0)>>15));

      data1 = ((*ptr16in++ >> 2)- (smulbb(COEFF5_2, temp3)>>15));
      temp2 = (temp3 + (smulbb(COEFF5_2, data1)>>15));

      *ptr16out++ = (temp1+ temp2);
      *ptr16out++ = (temp1- temp2);
   }while(--i != 0) ;


   data[0] = data0;
   data[1] = data1;

}

/****************************************************************************
 *
 *     Function     : filter5
 *     Purpose      : Fifth-order half-band lowpass/highpass filter pair with
 *                    decimation.
 *
 ***************************************************************************/
static void filter5(Word16 *in0,    /* i/o : input values; output low-pass part  */
                    Word16 *in1,    /* i/o : input values; output high-pass part */
                    Word16 data[]   /* i/o : updated filter memory               */
                    )
{
   Word32 temp0, temp1, temp2;

   temp0 = (*in0 - (smulbb(COEFF5_1, data[0])>>15));
   temp1 = (data[0] + (smulbb(COEFF5_1, temp0)>>15));
   data[0] = (Word16)temp0;

   temp0 = (*in1 - (smulbb(COEFF5_2, data[1])>>15));
   temp2 = (data[1] + (smulbb(COEFF5_2, temp0)>>15));
   data[1] = (Word16)temp0;

   *in0 = ((temp1 + temp2)>> 1);
   *in1 = ((temp1 - temp2)>> 1);
}

/****************************************************************************
 *
 *     Function     : filter3
 *     Purpose      : Third-order half-band lowpass/highpass filter pair with
 *                    decimation.
 *     Return value :
 *
 ***************************************************************************/
static void filter3(Word16 *in0,   /* i/o : input values; output low-pass part  */
                    Word16 *in1,   /* i/o : input values; output high-pass part */
                    Word16 *data   /* i/o : updated filter memory               */
                    )
{
   Word32 temp1, temp2;

   temp1 = (*in1  - (smulbb(COEFF3, *data)>>15));
   temp2 = (*data + (smulbb(COEFF3, temp1)>>15));
   *data = (Word16)temp1;

   *in1 = ((*in0 - temp2) >> 1);
   *in0 = ((*in0 + temp2) >> 1);
}

/****************************************************************************
 *
 *     Function     : level_calculation
 *     Purpose      : Calculate signal level in a sub-band. Level is calculated
 *                    by summing absolute values of the input data.
 *     Return value : signal level
 *
 ***************************************************************************/
static Word16 level_calculation(
    Word16 data[],     /* i   : signal buffer                                    */
    Word16 *sub_level, /* i   : level calculate at the end of the previous frame */
                       /* o   : level of signal calculated from the last         */
                       /*       (count2 - count1) samples                        */
    Word16 count1,     /* i   : number of samples to be counted                  */
    Word16 count2,     /* i   : number of samples to be counted                  */
    Word16 ind_m,      /* i   : step size for the index of the data buffer       */
    Word16 ind_a,      /* i   : starting index of the data buffer                */
    Word16 scale       /* i   : scaling for the level calculation                */
    )
{
   Word32 l_temp1, l_temp2;
   Word32 level, i;

   l_temp1 = 0L;
   for (i = count1; i < count2; i++)
   {
//      l_temp1 = L_mac(l_temp1, 1, abs_s(data[ind_m*i+ind_a]));
      l_temp1 = qdadd(l_temp1, smulbb( 1, abs_s(data[ind_m*i+ind_a]) ));
   }

   l_temp2 = qadd(l_temp1, L_shl(*sub_level, (16 - scale)));
   *sub_level = (Word16)(L_shl(l_temp1, scale) >> 16);

   for (i = 0; i < count1; i++)
   {
      l_temp2 = qdadd(l_temp2, smulbb(1, abs_s(data[ind_m*i+ind_a]) ));
   }
   level = (Word16)(L_shl(l_temp2, scale) >> 16);

   return (Word16)level;
}

/****************************************************************************
 *
 *     Function     : filter_bank
 *     Purpose      : Divides input signal into 9-bands and calculas level of
 *                    the signal in each band
 *
 ***************************************************************************/
static void filter_bank(
   AMR_Encode_FrameState *Enc,
	vadState1 *st,  /* i/o : State struct               */
   Word16 in[],   /* i   : input frame                */
   Word16 level[] /* 0   : signal levels at each band */
                        )
{
   Word32 i;
  	AMR_Shared_Buf *Enc_share = Enc->share;
	Word16 *ptr16;
   Word16 *tmp_buf = Enc_share->shabuf.stE.u2E.s0E.tmp_buf;


   /* calculate the filter bank */
   first_filter_stage(in, tmp_buf, st->a_data5[0]);

	ptr16 = tmp_buf;
	i = FRAME_LEN/4;
	do
   {
      filter5(ptr16, ptr16+2, st->a_data5[1]);
      filter5(ptr16+1, ptr16+3, st->a_data5[2]);
		ptr16 += 4;
   }while(-- i != 0);


	ptr16 = tmp_buf;
	i = FRAME_LEN/8;
	do
   {
      filter3(ptr16, ptr16+4, &st->a_data3[0]);
      filter3(ptr16+2, ptr16+6, &st->a_data3[1]);
      filter3(ptr16+3, ptr16+7, &st->a_data3[4]);
		ptr16 += 8;
   }while(-- i != 0);

	ptr16 = tmp_buf;
	i = FRAME_LEN/16;
	do
   {
      filter3(ptr16, ptr16+8, &st->a_data3[2]);
      filter3(ptr16+4, ptr16+12, &st->a_data3[3]);
		ptr16 += 16;
   }while(-- i != 0);



   /* calculate levels in each frequency band */

   /* 3000 - 4000 Hz*/
   level[8] = level_calculation(tmp_buf, &st->sub_level[8], FRAME_LEN/4-8, FRAME_LEN/4, 4, 1, 15);

   /* 2500 - 3000 Hz*/
   level[7] = level_calculation(tmp_buf, &st->sub_level[7], FRAME_LEN/8-4, FRAME_LEN/8, 8, 7, 16);

   /* 2000 - 2500 Hz*/
   level[6] = level_calculation(tmp_buf, &st->sub_level[6], FRAME_LEN/8-4, FRAME_LEN/8, 8, 3, 16);

   /* 1500 - 2000 Hz*/
   level[5] = level_calculation(tmp_buf, &st->sub_level[5], FRAME_LEN/8-4, FRAME_LEN/8, 8, 2, 16);

   /* 1000 - 1500 Hz*/
   level[4] = level_calculation(tmp_buf, &st->sub_level[4], FRAME_LEN/8-4, FRAME_LEN/8, 8, 6, 16);

   /* 750 - 1000 Hz*/
   level[3] = level_calculation(tmp_buf, &st->sub_level[3], FRAME_LEN/16-2, FRAME_LEN/16, 16, 4, 16);

   /* 500 - 750 Hz*/
   level[2] = level_calculation(tmp_buf, &st->sub_level[2], FRAME_LEN/16-2, FRAME_LEN/16, 16, 12, 16);

   /* 250 - 500 Hz*/
   level[1] = level_calculation(tmp_buf, &st->sub_level[1], FRAME_LEN/16-2, FRAME_LEN/16, 16, 8, 16);

   /* 0 - 250 Hz*/
   level[0] = level_calculation(tmp_buf, &st->sub_level[0], FRAME_LEN/16-2, FRAME_LEN/16, 16, 0, 16);

}

/****************************************************************************
 *
 *     Function   : update_cntrl
 *     Purpose    : Control update of the background noise estimate.
 *     Inputs     : pitch:      flags for pitch detection
 *                  stat_count: stationary counter
 *                  tone:       flags indicating presence of a tone
 *                  complex:      flags for complex  detection
 *                  vadreg:     intermediate VAD flags
 *     Output     : stat_count: stationary counter
 *
 ***************************************************************************/
static void update_cntrl(vadState1 *st,  /* i/o : State struct                       */
                         Word16 level[] /* i   : sub-band levels of the input frame */
                         )
{
	Word32 i,exp,stat_rat,num,denom,alpha,temp;

   /* handle highband complex signal input  separately       */
   /* if ther has been highband correlation for some time    */
   /* make sure that the VAD update speed is low for a while */
   if (st->complex_warning != 0)
   {
      if(st->stat_count < CAD_MIN_STAT_COUNT)
      {
         st->stat_count = CAD_MIN_STAT_COUNT;
      }
   }
   /* NB stat_count is allowed to be decreased by one below again  */
   /* deadlock in speech is not possible unless the signal is very */
   /* complex and need a high rate                                 */

   /* if fullband pitch or tone have been detected for a while, initialize stat_count */

   if (( ((st->pitch & 0x6000) == 0x6000) ) ||
      ( ((st->tone & 0x7c00) == 0x7c00) ))
   {
      st->stat_count = STAT_COUNT;
   }
   else
   {
      /* if 8 last vad-decisions have been "0", reinitialize stat_count */

      if ((st->vadreg & 0x7f80) == 0)
      {
         st->stat_count = STAT_COUNT;
      }
      else
      {
         stat_rat = 0;
         for (i = 0; i < COMPLEN; i++)
         {
            if(level[i] > st->ave_level[i])
            {
               num = level[i];
               denom = st->ave_level[i];
            }
            else
            {
               num = st->ave_level[i];
               denom = level[i];
            }
            /* Limit nimimum value of num and denom to STAT_THR_LEVEL */

            if(num < STAT_THR_LEVEL)
            {
               num = STAT_THR_LEVEL;
            }

            if(denom < STAT_THR_LEVEL)
            {
               denom = STAT_THR_LEVEL;
            }

            exp = norm_s(denom);
            denom = (denom << exp);

            /* stat_rat = num/denom * 64 */
            temp = div_s((Word16)(num >> 1), (Word16)denom);
            stat_rat = (stat_rat + shr(temp, (8 - exp)));
         }

         /* compare stat_rat with a threshold and update stat_count */

         if(stat_rat > STAT_THR)
         {
            st->stat_count = STAT_COUNT;
         }
         else
         {
            if ((st->vadreg & 0x4000) != 0)
            {
               if (st->stat_count != 0)
               {
                  st->stat_count = (st->stat_count - 1);
               }
            }
         }

      }

   }

   /* Update average amplitude estimate for stationarity estimation */
   alpha = ALPHA4;

   if(st->stat_count == STAT_COUNT)
   {
      alpha = 32767;
   }
   else if ((st->vadreg & 0x4000) == 0)
   {

      alpha = ALPHA5;
   }

   for (i = 0; i < COMPLEN; i++)
   {
      st->ave_level[i] = (st->ave_level[i]+
         mult_r(alpha, (level[i]- st->ave_level[i])));

   }
}

/****************************************************************************
 *
 *     Function     : hangover_addition
 *     Purpose      : Add hangover for complex signal or after speech bursts
 *     Inputs       : burst_count:  counter for the length of speech bursts
 *                    hang_count:   hangover counter
 *                    vadreg:       intermediate VAD decision
 *     Outputs      : burst_count:  counter for the length of speech bursts
 *                    hang_count:   hangover counter
 *     Return value : VAD_flag indicating final VAD decision
 *
 ***************************************************************************/
static Word16 hangover_addition(
              vadState1 *st,       /* i/o : State struct                     */
              Word32 noise_level, /* i   : average level of the noise       */
                                  /*       estimates                        */
              Word32 low_power    /* i   : flag power of the input frame    */
              )
{
   Word16 hang_len, burst_len;

   /*
   Calculate burst_len and hang_len
   burst_len: number of consecutive intermediate vad flags with "1"-decision
   required for hangover addition
   hang_len:  length of the hangover
   */


   if(noise_level > HANG_NOISE_THR)
   {
      burst_len = BURST_LEN_HIGH_NOISE;
      hang_len = HANG_LEN_HIGH_NOISE;
   }
   else
   {
      burst_len = BURST_LEN_LOW_NOISE;
      hang_len = HANG_LEN_LOW_NOISE;
   }

   /* if the input power (pow_sum) is lower than a threshold, clear
   counters and set VAD_flag to "0"  "fast exit"                 */

   if (low_power != 0)
   {
      st->burst_count = 0;
      st->hang_count = 0;
      st->complex_hang_count = 0;
      st->complex_hang_timer = 0;
      return 0;
   }

   if(st->complex_hang_timer > CVAD_HANG_LIMIT)
   {
      if(st->complex_hang_count < CVAD_HANG_LENGTH)
      {
         st->complex_hang_count = CVAD_HANG_LENGTH;
      }
   }

   /* long time very complex signal override VAD output function */

   if (st->complex_hang_count != 0)
   {
      st->burst_count = BURST_LEN_HIGH_NOISE;
      st->complex_hang_count = (st->complex_hang_count - 1);
      return 1;
   }
   else
   {
      /* let hp_corr work in from a noise_period indicated by the VAD */

      if (((st->vadreg & 0x3ff0) == 0) &&
         ( (st->corr_hp_fast > CVAD_THRESH_IN_NOISE) ))
      {
         return 1;
      }
   }

   /* update the counters (hang_count, burst_count) */

   if ((st->vadreg & 0x4000) != 0)
   {
      st->burst_count = (st->burst_count + 1);
      if(st->burst_count >=  burst_len)
      {
         st->hang_count = hang_len;
      }
      return 1;
   }
   else
   {
      st->burst_count = 0;
      if (st->hang_count > 0)
      {
         st->hang_count = (st->hang_count - 1);
         return 1;
      }
   }
   return 0;
}

/****************************************************************************
 *
 *     Function   : noise_estimate_update
 *     Purpose    : Update of background noise estimate
 *     Inputs     : bckr_est:   background noise estimate
 *                  pitch:      flags for pitch detection
 *                  stat_count: stationary counter
 *     Outputs    : bckr_est:   background noise estimate
 *
 ***************************************************************************/
static void noise_estimate_update(
                  vadState1 *st,    /* i/o : State struct                       */
                  Word16 level[]   /* i   : sub-band levels of the input frame */
                  )
{
   Word32 i, alpha_up, alpha_down, bckr_add;

   /* Control update of bckr_est[] */
   update_cntrl(st, level);

   /* Choose update speed */
   bckr_add = 2;


   if ( ((0x7800 & st->vadreg) == 0) && ((st->pitch & 0x7800) == 0)
      &&  (st->complex_hang_count == 0))
   {
      alpha_up = ALPHA_UP1;
      alpha_down = ALPHA_DOWN1;
   }
   else
   {
      if ((st->stat_count == 0)
         && (st->complex_hang_count == 0))
      {
         alpha_up = ALPHA_UP2;
         alpha_down = ALPHA_DOWN2;
      }
      else
      {
         alpha_up = 0;
         alpha_down = ALPHA3;
         bckr_add = 0;
      }
   }

   /* Update noise estimate (bckr_est) */
   for (i = 0; i < COMPLEN; i++)
   {
      Word16 temp;
      temp = (st->old_level[i] - st->bckr_est[i]);

      if (temp < 0)
      { /* update downwards*/
//         st->bckr_est[i] = (-2+ (st->bckr_est[i]+ mult_r(alpha_down, temp)));
         st->bckr_est[i] = (-2+ (st->bckr_est[i]+ (Word16)(qdadd(0x8000, smulbb(alpha_down, temp))>>16) ));

         /* limit minimum value of the noise estimate to NOISE_MIN */
         if(st->bckr_est[i] < NOISE_MIN)
         {
            st->bckr_est[i] = NOISE_MIN;
         }
      }
      else
      { /* update upwards */
         st->bckr_est[i] = (bckr_add+ (st->bckr_est[i]+ (Word16)(qdadd(0x8000,smulbb(alpha_up, temp))>>16)  ));

         /* limit maximum value of the noise estimate to NOISE_MAX */
         if(st->bckr_est[i] > NOISE_MAX)
         {
            st->bckr_est[i] = NOISE_MAX;
         }
      }
   }

   /* Update signal levels of the previous frame (old_level) */
/*
   for(i = 0; i < COMPLEN; i++)
   {
      st->old_level[i] = level[i];
   }
*/
   st->old_level[0] = level[0];
   st->old_level[1] = level[1];
   st->old_level[2] = level[2];
   st->old_level[3] = level[3];
   st->old_level[4] = level[4];
   st->old_level[5] = level[5];
   st->old_level[6] = level[6];
   st->old_level[7] = level[7];
   st->old_level[8] = level[8];



}


/****************************************************************************
 *
 *     Function   : complex_estimate_adapt
 *     Purpose    : Update/adapt of complex signal estimate
 *     Inputs     : low_power:   low signal power flag
 *     Outputs    : st->corr_hp_fast:   long term complex signal estimate
 *
 ***************************************************************************/
static void complex_estimate_adapt(
         vadState1 *st,       /* i/o : VAD state struct                       */
         Word32 low_power    /* i   : very low level flag of the input frame */
         )
{
   Word32 alpha;            /* Q15 */
   Word32 L_tmp;            /* Q31 */

   /* adapt speed on own state */
   if(st->best_corr_hp < st->corr_hp_fast)  /* decrease */
   {
      if(st->corr_hp_fast < CVAD_THRESH_ADAPT_HIGH)
      {  /* low state  */
         alpha = CVAD_ADAPT_FAST;
      }
      else
      {  /* high state */
         alpha = CVAD_ADAPT_REALLY_FAST;
      }
   }
   else  /* increase */
   {
      if(st->corr_hp_fast < CVAD_THRESH_ADAPT_HIGH)
      {
         alpha = CVAD_ADAPT_FAST;
      }
      else
      {
         alpha = CVAD_ADAPT_SLOW;
      }
   }

   L_tmp = (st->corr_hp_fast) << 16;
   L_tmp = qdsub(L_tmp, smulbb( alpha, st->corr_hp_fast));
   L_tmp = qdadd(L_tmp, smulbb( alpha, st->best_corr_hp));
   st->corr_hp_fast = (Word16)(qadd(L_tmp,0x8000)>>16);           /* Q15 */


   if(st->corr_hp_fast < CVAD_MIN_CORR)
   {
      st->corr_hp_fast = CVAD_MIN_CORR;
   }

   if (low_power != 0)
   {
      st->corr_hp_fast = CVAD_MIN_CORR;
   }

}

/****************************************************************************
 *
 *     Function     : complex_vad
 *     Purpose      : complex background decision
 *     Return value : the complex background decision
 *
 ***************************************************************************/
static Word16 complex_vad(vadState1 *st,    /* i/o : VAD state struct              */
                          Word32 low_power /* i   : flag power of the input frame */
                          )
{

   st->complex_high = (st->complex_high >> 1);
   st->complex_low = (st->complex_low >> 1);


   if (low_power == 0)
   {

      if(st->corr_hp_fast > CVAD_THRESH_ADAPT_HIGH)
      {
         st->complex_high = st->complex_high | 0x4000;
      }


      if(st->corr_hp_fast > CVAD_THRESH_ADAPT_LOW)
      {
         st->complex_low = st->complex_low | 0x4000;
      }
   }


   if(st->corr_hp_fast > CVAD_THRESH_HANG)
   {
      st->complex_hang_timer = (st->complex_hang_timer + 1);
   }
   else
   {
      st->complex_hang_timer =  0;
   }


   return ((((st->complex_high & 0x7f80)- 0x7f80) == 0) ||
      (((st->complex_low & 0x7fff)- 0x7fff) == 0));
}

/****************************************************************************
 *
 *     Function     : vad_decision
 *     Purpose      : Calculates VAD_flag
 *     Inputs       : bckr_est:    background noise estimate
 *                    vadreg:      intermediate VAD flags
 *     Outputs      : noise_level: average level of the noise estimates
 *                    vadreg:      intermediate VAD flags
 *     Return value : VAD_flag
 *
 ***************************************************************************/
static Word16 vad_decision(
             vadState1 *st,          /* i/o : State struct                       */
             Word16 level[COMPLEN], /* i   : sub-band levels of the input frame */
             Word32 pow_sum         /* i   : power of the input frame           */
             )
{
   Word32 i;
   Word32 L_temp,noise_level,low_power_flag;
   Word16 snr_sum;
   Word16 vad_thr, temp ;

   /*
      Calculate squared sum of the input levels (level)
      divided by the background noise components (bckr_est).
    */
   L_temp = 0;
//   for (i = 0; i < COMPLEN; i++)
	i = COMPLEN-1;
	do
   {
      Word32 exp;

      exp = norm_s(st->bckr_est[i]);
      temp = (st->bckr_est[i] << exp);
      temp = div_s( (Word16)(level[i] >> 1), temp);
      temp = shl(temp, sub(exp, UNIRSHFT-1));
      L_temp = qdadd(L_temp,smulbb( temp, temp));
   }while(--i >= 0);

   snr_sum = (Word16)(L_shl(L_temp, 6) >> 16);
   snr_sum = smulbb(snr_sum, INV_COMPLEN) >> 15;

   /* Calculate average level of estimated background noise */
   L_temp = 0;
/*
   for (i = 0; i < COMPLEN; i++)
   {
      L_temp = L_add(L_temp, st->bckr_est[i]);
   }
*/
   L_temp = qadd(L_temp, st->bckr_est[0]);
   L_temp = qadd(L_temp, st->bckr_est[1]);
   L_temp = qadd(L_temp, st->bckr_est[2]);
   L_temp = qadd(L_temp, st->bckr_est[3]);
   L_temp = qadd(L_temp, st->bckr_est[4]);
   L_temp = qadd(L_temp, st->bckr_est[5]);
   L_temp = qadd(L_temp, st->bckr_est[6]);
   L_temp = qadd(L_temp, st->bckr_est[7]);
   L_temp = qadd(L_temp, st->bckr_est[8]);



   noise_level = (Word16)(L_shl(L_temp, 13) >> 16);

   /* Calculate VAD threshold */
   vad_thr = ( (smulbb(VAD_SLOPE, (noise_level - VAD_P1))>>15) + VAD_THR_HIGH);


   if(vad_thr < VAD_THR_LOW)
   {
      vad_thr = VAD_THR_LOW;
   }

   /* Shift VAD decision register */
   st->vadreg = (st->vadreg >> 1);

   /* Make intermediate VAD decision */

   if(snr_sum > vad_thr)
   {
      st->vadreg = st->vadreg | 0x4000;
   }
   /* primary vad decsion made */

   /* check if the input power (pow_sum) is lower than a threshold" */

   if(pow_sum < VAD_POW_LOW)
   {
      low_power_flag = 1;
   }
   else
   {
      low_power_flag = 0;
   }

   /* update complex signal estimate st->corr_hp_fast and hangover reset timer using */
   /* low_power_flag and corr_hp_fast  and various adaptation speeds                 */
   complex_estimate_adapt(st, low_power_flag);

   /* check multiple thresholds of the st->corr_hp_fast value */
   st->complex_warning = complex_vad(st, low_power_flag);

   /* Update speech subband vad background noise estimates */
   noise_estimate_update(st, level);

   /*  Add speech and complex hangover and return speech VAD_flag */
   /*  long term complex hangover may be added */
   st->speech_vad_decision = hangover_addition(st, noise_level, low_power_flag);


   return (st->speech_vad_decision);
}

/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/
/*************************************************************************
*
*  Function:   vad1_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
#if 0
int vad1_init (vadState1 **state)
{
    vadState1* s;

    if (state == (vadState1 **) NULL){
        fprintf(stderr, "vad_init: invalid parameter\n");
        return -1;
    }
    *state = NULL;

    /* allocate memory */
    if ((s = (vadState1 *) malloc(  ((sizeof(vadState1)+3)&(~3))  )) == NULL){
        fprintf(stderr, "vad_init: can not malloc state structure\n");
        return -1;
    }

    vad1_reset(s);

    *state = s;

    return 0;
}
#endif

/*************************************************************************
*
*  Function:   vad1_exit
*  Purpose:    The memory used for state memory is freed
*
**************************************************************************
*/
#if 0
void vad1_exit (vadState1 **state)
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
*  Function:   vad1_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int vad1_reset (vadState1 *state)
{
   Word32 i;
/*
   if (state == (vadState1 *) NULL){
      fprintf(stderr, "vad_reset: invalid parameter\n");
      return -1;
   }
*/
   /* Initialize pitch detection variables */
   state->oldlag_count = 0;
   state->oldlag = 0;
   state->pitch = 0;
   state->tone = 0;

   state->complex_high = 0;
   state->complex_low = 0;
   state->complex_hang_timer = 0;

   state->vadreg = 0;

   state->stat_count = 0;
   state->burst_count = 0;
   state->hang_count = 0;
   state->complex_hang_count = 0;

   /* initialize memory used by the filter bank */
/*
   for (i = 0; i < 3; i++)
   {
      for (j = 0; j < 2; j++)
      {
         state->a_data5[i][j] = 0;
      }
   }
*/
   state->a_data5[0][0] = 0;
   state->a_data5[0][1] = 0;
   state->a_data5[1][0] = 0;
   state->a_data5[1][1] = 0;
   state->a_data5[2][0] = 0;
   state->a_data5[2][1] = 0;

/*
   for (i = 0; i < 5; i++)
   {
      state->a_data3[i] = 0;
   }
*/

   state->a_data3[0] = 0;
   state->a_data3[1] = 0;
   state->a_data3[2] = 0;
   state->a_data3[3] = 0;
   state->a_data3[4] = 0;

   /* initialize the rest of the memory */
   for (i = 0; i < COMPLEN; i++)
   {
      state->bckr_est[i] = NOISE_INIT;
      state->old_level[i] = NOISE_INIT;
      state->ave_level[i] = NOISE_INIT;
      state->sub_level[i] = 0;
   }

   state->best_corr_hp = CVAD_LOWPOW_RESET;

   state->speech_vad_decision = 0;
   state->complex_warning = 0;
   state->sp_burst_count = 0;

   state->corr_hp_fast = CVAD_LOWPOW_RESET;

   return 0;
}



/****************************************************************************
 *
 *     Function     : vad_complex_detection_update
 *     Purpose      : update vad->bestCorr_hp  complex signal feature state
 *
 ***************************************************************************/
void vad_complex_detection_update (vadState1 *st,       /* i/o : State struct */
                                   Word16 best_corr_hp /* i   : best Corr    */
                                   )
{
   st->best_corr_hp = best_corr_hp;
}

/****************************************************************************
 *
 *     Function     : vad_tone_detection
 *     Purpose      : Set tone flag if pitch gain is high. This is used to detect
 *                    signaling tones and other signals with high pitch gain.
 *     Inputs       : tone: flags indicating presence of a tone
 *     Outputs      : tone: flags indicating presence of a tone
 *
 ***************************************************************************/
void vad_tone_detection (vadState1 *st,  /* i/o : State struct            */
                         Word32 t0,     /* i   : autocorrelation maxima  */
                         Word32 t1      /* i   : energy                  */
                         )
{
   Word32 temp;
   /*
      if (t0 > TONE_THR * t1)
      set tone flag
      */
   temp = (Word16)(qadd(t1,0x8000)>>16);


   if ((temp > 0) && (qdsub(t0, smulbb( temp, TONE_THR)) > 0))
   {
      st->tone = st->tone | 0x4000;
   }
}

/****************************************************************************
 *
 *     Function     : vad_tone_detection_update
 *     Purpose      : Update the tone flag register. Tone flags are shifted right
 *                    by one bit. This function should be called from the speech
 *                    encoder before call to Vad_tone_detection() function.
 *
 ***************************************************************************/
void vad_tone_detection_update (
                vadState1 *st,              /* i/o : State struct              */
                Word16 one_lag_per_frame   /* i   : 1 if one open-loop lag is
                                              calculated per each frame,
                                              otherwise 0                     */
                )
{
   /* Shift tone flags right by one bit */
   st->tone = shr(st->tone, 1);

   /* If open-loop lag is calculated only once in each frame, do extra update
      and assume that the other tone flag of the frame is one. */
   if (one_lag_per_frame != 0)
   {
      st->tone = shr(st->tone, 1);
      st->tone = st->tone | 0x2000;
   }
}

/****************************************************************************
 *
 *     Function     : vad_pitch_detection
 *     Purpose      : Test whether signal contains pitch or other periodic
 *                    component.
 *     Return value : Boolean voiced / unvoiced decision in state variable
 *
 ***************************************************************************/
void vad_pitch_detection (vadState1 *st,   /* i/o : State struct                  */
                          Word16 T_op[]   /* i   : speech encoder open loop lags */
                          )
{
   Word32 lagcount, i;

   lagcount = 0;

   for (i = 0; i < 2; i++)
   {
      if(abs_s((st->oldlag - T_op[i])) < LTHRESH)
      {
         lagcount = (lagcount + 1);
      }
      /* Save the current LTP lag */
      st->oldlag = T_op[i];
   }

   /* Make pitch decision.
      Save flag of the pitch detection to the variable pitch.
      */
   st->pitch = (st->pitch >> 1);

   if( (st->oldlag_count + lagcount) >= NTHRESH)
   {
      st->pitch = st->pitch | 0x4000;
   }

   /* Update oldlagcount */
   st->oldlag_count = (Word16)lagcount;
}

/****************************************************************************
 *
 *     Function     : vad
 *     Purpose      : Main program for Voice Activity Detection (VAD) for AMR
 *     Return value : VAD Decision, 1 = speech, 0 = noise
 *
 ***************************************************************************/
Word16 vad1(
   AMR_Encode_FrameState *Enc,
	vadState1 *st,      /* i/o : State struct                 */
   Word16 in_buf[]    /* i   : samples of the input frame   */
           )
{
   Word32 pow_sum;
   Word32 i;
	Word32 *ptr32,s;
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word16 *level = Enc_share->shabuf.stE.u2E.s0E.level;

   /* Calculate power of the input frame. */
   pow_sum = 0L;
	ptr32 = (Word32 *) &(in_buf[-LOOKAHEAD]);

	i = FRAME_LEN/2;
	do
   {
   	s = *ptr32++;
      pow_sum = qdadd(pow_sum, smulbb(s,s));
      pow_sum = qdadd(pow_sum, smultt(s,s));
   }while(--i != 0);

   /*
     If input power is very low, clear pitch flag of the current frame
     */

   if( pow_sum < POW_PITCH_THR )
   {
      st->pitch = st->pitch & 0x3fff;
   }

   /*
     If input power is very low, clear complex flag of the "current" frame
     */

   if(pow_sum < POW_COMPLEX_THR)
   {
      st->complex_low = st->complex_low & 0x3fff;
   }

   /*
     Run the filter bank which calculates signal levels at each band
     */
   filter_bank(Enc , st, in_buf, level);

   return (vad_decision(st, level, pow_sum));
}

#endif

