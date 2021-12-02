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
 *   WBAMR_WB_VAD.C
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR-WB codec
 *
 * Author:
 * -------
 *   Eddy Wu
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_cnst.h"
#include "wbamr_wb_vad.h"
#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_math_op.h"
#include "wbamr_private.h"
#include "wbamr_sections.h"



#define INLINE

/*
 * log2
 *
 *  Calculate WBAMR_Log2 and scale the signal:
 *
 *  ilog2(fract32 in) = -1024*log10(in * 2^-31)/log10(2), where in = [1, 2^31-1]
 *
 *  input   output
 *  32768   16384
 *  1       31744
 *
 * When input is in the range of [1,2^16], max error is 0.0380%.
 *
 */



WBAMR_CODE_SECTION fract16 WBAMR_ilog2 ( /* return: output value of the log2 */
  fract16 mant                           /* i: value to be converted */
)
{
  int_native i;
  fract32 l_temp;
  fract16 ex, ex2, res;

  if (mant <= 0)
  {
    mant = 1;
  }
  ex = WBAMR_Norm_s(mant);
  mant = Shl_fr1x16(mant, ex);

  for (i = 0; i < 3; i++)
    mant = Mult_fr1x16(mant, mant);
    l_temp = Mult_fr1x32(mant, mant);

  ex2 = Norm_fr1x32(l_temp);
  mant = Extract_hi_fr1x32(Shl_fr1x32(l_temp, ex2));
  res = Shl_fr1x16(Add_fr1x16(ex, 16), 10);
  res = Add_fr1x16(res, Shl_fr1x16(ex2, 6));
  res = Sub_fr1x16(Add_fr1x16(res, 127), Shr_fr1x16(mant, 8));

  return (res);

}

#if (AWB_OPT_VER == 2)
/****************************************************************************
*
*     Function   : WBAMR_Level_calculation
*     Purpose    : Calculate signal level in a Sub_fr1x16-band. Level is cal
*                  by summing absolute values of the input data.
*
*                  Signal level calculated from of the end of the frame
*                  (data[count1 - count2]) is stored to (*sub_level)
*                  and added to the level of the next frame.
*
*****************************************************************************/

WBAMR_CODE_SECTION fract16 WBAMR_Level_calculation ( /* return: signal level */
  fract32 data[],             /* i   : signal buffer               */
  fract16 * sub_level,        /* i   : level calculated at the end */
    			      /* o   : level of signal calculated  */
			      /* (count2 - count1) samples         */
  fract16 count1,             /* i   : number of samples           */
  fract16 count2,             /* i   : number of samples to count  */
  fract16 ind_m,              /* i   : step size for the index     */
  fract16 ind_a,              /* i   : starting index              */
  fract16 scale               /* i   : scaling for the level cal   */
)
{
  int_native i;
  fract32 l_temp1, l_temp2;
  fract16 level;

  l_temp1 = 0L;

  for (i = count1; i < count2; i++)
  {
    l_temp1 = Mac_fr16_32(l_temp1, 1, Abs_fr1x16(data[ind_m * i + ind_a]));
  }

  l_temp2 = Add_fr1x32(l_temp1, Shl_fr1x32(*sub_level, Sub_fr1x16(16, scale)));
  *sub_level = Extract_hi_fr1x32(Shl_fr1x32(l_temp1, scale));      

  for (i = 0; i < count1; i++)
  {
    l_temp2 = Mac_fr16_32(l_temp2, 1, Abs_fr1x16(data[ind_m * i + ind_a]));
  }

  level = Extract_hi_fr1x32(Shl_fr1x32(l_temp2, scale));

  return level;

}

/**************************************************************************
*
*     Function     : filter_bank
*     Purpose      : Divide input signal into bands
*
***************************************************************************/

WBAMR_CODE_SECTION void WBAMR_filter_bank (
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr, /* i/o : State struct */
  fract16 in[],                              /* i: input frame  */
  fract32 tmp_buf[]
)
{
   /*  question a_data5 will be used? */
  int_native i;
   fract16 *p_in;
   fract32 *p_buf;
   fract32 coeff1, coeff2, coeff3;
   fract32 d0, d1, d2, d3, t0, t1, t2, t3;

   p_in = in;
   coeff1 = 0xAA3D55C3;
   //coeff2 = 0xCE1431EC; // -6390 * 2/ 6390 * 2
   coeff2 = 0xE70A18F6;
   coeff3 = 0x979A6866;


   p_buf = tmp_buf;
   d0 = enc_obj_ptr->a_data5[0][0];
   d1 = enc_obj_ptr->a_data5[0][1];
   i = FRAME_LEN >> 2;
   do {
      t0 = AWB_smlawt(d0, coeff1, (fract32)(*p_in++ >> 1));
      d0 = AWB_smlawb(t0, coeff1, d0); // -21955 / 21955
      t1 = AWB_smlawt(d1, coeff2, (fract32)(*p_in++ >> 1));
      d1 = AWB_smlawb(t1, coeff2, d1); // -6390 / 6390
      *p_buf++ = (d0 + d1) >> 1;
      *p_buf++ = (d0 - d1) >> 1;

      d0 = AWB_smlawt(t0, coeff1, (fract32)(*p_in++ >> 1));
      t0 = AWB_smlawb(d0, coeff1, t0); // -21955 / 21955
      d1 = AWB_smlawt(t1, coeff2, (fract32)(*p_in++ >> 1));
      t1 = AWB_smlawb(d1, coeff2, t1); // -6390 / 6390
      *p_buf++ = (t0 + t1) >> 1;
      *p_buf++ = (t0 - t1) >> 1;
   } while(--i != 0);
   enc_obj_ptr->a_data5[0][0] = d0;
   enc_obj_ptr->a_data5[0][1] = d1;

   p_buf = tmp_buf - 8;
   d0 = enc_obj_ptr->a_data5[1][0];
   d1 = enc_obj_ptr->a_data5[1][1];
   d2 = enc_obj_ptr->a_data5[2][0];
   d3 = enc_obj_ptr->a_data5[2][1];
   i = FRAME_LEN >> 3;
   do {
      t0 = AWB_smlawt(d0, coeff1, *(p_buf+=8));
      d0 = AWB_smlawb(t0, coeff1, d0); // -21955 / 21955
      t1 = AWB_smlawt(d1, coeff2, *(p_buf+2));
      d1 = AWB_smlawb(t1, coeff2, d1); // -6390 / 6390
      *(p_buf) = (d0 + d1) >> 1;
      *(p_buf+2) = (d0 - d1) >> 1;
      d0 = AWB_smlawt(t0, coeff1, *(p_buf+4));
      t0 = AWB_smlawb(d0, coeff1, t0); // -21955 / 21955
      d1 = AWB_smlawt(t1, coeff2, *(p_buf+6));
      t1 = AWB_smlawb(d1, coeff2, t1); // -6390 / 6390
      *(p_buf+4) = (t0 + t1) >> 1;
      *(p_buf+6) = (t0 - t1) >> 1;

      t2 = AWB_smlawt(d2, coeff1, *(p_buf+1));
      d2 = AWB_smlawb(t2, coeff1, d2); // -21955 / 21955
      t3 = AWB_smlawt(d3, coeff2, *(p_buf+3));
      d3 = AWB_smlawb(t3, coeff2, d3); // -6390 / 6390
      *(p_buf+1) = (d2 + d3) >> 1;
      *(p_buf+3) = (d2 - d3) >> 1;
      d2 = AWB_smlawt(t2, coeff1, *(p_buf+5));
      t2 = AWB_smlawb(d2, coeff1, t2); // -21955 / 21955
      d3 = AWB_smlawt(t3, coeff2, *(p_buf+7));
      t3 = AWB_smlawb(d3, coeff2, t3); // -6390 / 6390
      *(p_buf+5) = (t2 + t3) >> 1;
      *(p_buf+7) = (t2 - t3) >> 1;
   } while(--i != 0);
   enc_obj_ptr->a_data5[1][0] = d0;
   enc_obj_ptr->a_data5[1][1] = d1;
   enc_obj_ptr->a_data5[2][0] = d2;
   enc_obj_ptr->a_data5[2][1] = d3;

   p_buf = tmp_buf - 16;
   d0 = enc_obj_ptr->a_data5[3][0];
   d1 = enc_obj_ptr->a_data5[3][1];
   i = FRAME_LEN >> 4;
   do {
      t0 = AWB_smlawt(d0, coeff1, *(p_buf+=16));
      d0 = AWB_smlawb(t0, coeff1, d0); // -21955 / 21955
      t1 = AWB_smlawt(d1, coeff2, *(p_buf+4));
      d1 = AWB_smlawb(t1, coeff2, d1); // -6390 / 6390
      *(p_buf) = (d0 + d1) >> 1;
      *(p_buf+4) = (d0 - d1) >> 1;
      d0 = AWB_smlawt(t0, coeff1, *(p_buf+8));
      t0 = AWB_smlawb(d0, coeff1, t0); // -21955 / 21955
      d1 = AWB_smlawt(t1, coeff2, *(p_buf+12));
      t1 = AWB_smlawb(d1, coeff2, t1); // -6390 / 6390
      *(p_buf+8) = (t0 + t1) >> 1;
      *(p_buf+12) = (t0 - t1) >> 1;
   } while(--i != 0);
   enc_obj_ptr->a_data5[3][0] = d0;
   enc_obj_ptr->a_data5[3][1] = d1;

   p_buf = tmp_buf - 14;
   d0 = enc_obj_ptr->a_data5[4][0];
   d1 = enc_obj_ptr->a_data5[4][1];
   d2 = enc_obj_ptr->a_data3[0];
   i = FRAME_LEN >> 4;
   do {
      t0 = AWB_smlawt(d0, coeff1, *(p_buf+=16));
      d0 = AWB_smlawb(t0, coeff1, d0); // -21955 / 21955
      t1 = AWB_smlawt(d1, coeff2, *(p_buf+4));
      d1 = AWB_smlawb(t1, coeff2, d1); // -6390 / 6390
      *(p_buf) = (d0 + d1) >> 1;
      *(p_buf+4) = (d0 - d1) >> 1;
      d0 = AWB_smlawt(t0, coeff1, *(p_buf+8));
      t0 = AWB_smlawb(d0, coeff1, t0); // -21955 / 21955
      d1 = AWB_smlawt(t1, coeff2, *(p_buf+12));
      t1 = AWB_smlawb(d1, coeff2, t1); // -6390 / 6390
      *(p_buf+8) = (t0 + t1) >> 1;
      *(p_buf+12) = (t0 - t1) >> 1;

      t2 = AWB_smlawt(d2, coeff3, *(p_buf+5));
      d2 = AWB_smlawb(t2, coeff3, d2); // - 13363 / 13363
      t3 = *(p_buf+1);
      *(p_buf+1) = (t3 + d2) >> 1;
      *(p_buf+5) = (t3 - d2) >> 1;
      d2 = AWB_smlawt(t2, coeff3, *(p_buf+13));
      t2 = AWB_smlawb(d2, coeff3, t2); // - 13363 / 13363
      t3 = *(p_buf+9);
      *(p_buf+9) = (t3 + d2) >> 1;
      *(p_buf+13) = (t3 - d2) >> 1;
   } while(--i != 0);
   enc_obj_ptr->a_data5[4][0] = d0;
   enc_obj_ptr->a_data5[4][1] = d1;
   enc_obj_ptr->a_data3[0] = d2;


   p_buf = tmp_buf;
   d0 = enc_obj_ptr->a_data3[1];
   d1 = enc_obj_ptr->a_data3[2];
   d2 = enc_obj_ptr->a_data3[3];
   i = FRAME_LEN >> 5;
   do {
      t0 = AWB_smlawt(d0, coeff3, *(p_buf+8));
      d0 = AWB_smlawb(t0, coeff3, d0); // - 13363 / 13363
      t3 = *(p_buf);
      *(p_buf) = (t3 + d0) >> 1;
      *(p_buf+8) = (t3 - d0) >> 1;
      t1 = AWB_smlawt(d1, coeff3, *(p_buf+12));
      d1 = AWB_smlawb(t1, coeff3, d1); // - 13363 / 13363
      t3 = *(p_buf+4);
      *(p_buf+4) = (t3 + d1) >> 1;
      *(p_buf+12) = (t3 - d1) >> 1;
      t2 = AWB_smlawt(d2, coeff3, *(p_buf+14));
      d2 = AWB_smlawb(t2, coeff3, d2); // - 13363 / 13363
      t3 = *(p_buf+6);
      *(p_buf+6) = (t3 + d2) >> 1;
      *(p_buf+14) = (t3 - d2) >> 1;
      p_buf += 16;

      d0 = AWB_smlawt(t0, coeff3, *(p_buf+8));
      t0 = AWB_smlawb(d0, coeff3, t0); // - 13363 / 13363
      d3 = *(p_buf);
      *(p_buf) = (t3 + t0) >> 1;
      *(p_buf+8) = (t3 - t0) >> 1;
      d1 = AWB_smlawt(t1, coeff3, *(p_buf+12));
      t1 = AWB_smlawb(d1, coeff3, t1); // - 13363 / 13363
      d3 = *(p_buf+4);
      *(p_buf+4) = (t3 + t1) >> 1;
      *(p_buf+12) = (t3 - t1) >> 1;
      d2 = AWB_smlawt(t2, coeff3, *(p_buf+14));
      t2 = AWB_smlawb(d2, coeff3, t2); // - 13363 / 13363
      d3 = *(p_buf+6);
      *(p_buf+6) = (t3 + t2) >> 1;
      *(p_buf+14) = (t3 - t2) >> 1;

      p_buf += 16;
   } while(--i != 0);
   enc_obj_ptr->a_data3[1] = d0;
   enc_obj_ptr->a_data3[2] = d1;
   enc_obj_ptr->a_data3[3] = d2;

   p_buf = tmp_buf;
   d0 = enc_obj_ptr->a_data3[4];
   d1 = enc_obj_ptr->a_data3[5];
   i = FRAME_LEN >> 6;
   do {
      t0 = AWB_smlawt(d0, coeff3, *(p_buf+16));
      d0 = AWB_smlawb(t0, coeff3, d0); // - 13363 / 13363
      t3 = *(p_buf);
      *(p_buf) = (t3 + d0) >> 1;
      *(p_buf+16) = (t3 - d0) >> 1;
      t1 = AWB_smlawt(d1, coeff3, *(p_buf+24));
      d1 = AWB_smlawb(t1, coeff3, d1); // - 13363 / 13363
      t3 = *(p_buf+8);
      *(p_buf+8) = (t3 + d1) >> 1;
      *(p_buf+24) = (t3 - d1) >> 1;
      p_buf += 32;

      d0 = AWB_smlawt(t0, coeff3, *(p_buf+16));
      t0 = AWB_smlawb(d0, coeff3, t0); // - 13363 / 13363
      d3 = *(p_buf);
      *(p_buf) = (t3 + t0) >> 1;
      *(p_buf+16) = (t3 - t0) >> 1;
      d1 = AWB_smlawt(t1, coeff3, *(p_buf+24));
      t1 = AWB_smlawb(d1, coeff3, t1); // - 13363 / 13363
      d3 = *(p_buf+8);
      *(p_buf+8) = (t3 + t1) >> 1;
      *(p_buf+24) = (t3 - t1) >> 1;
      p_buf += 32;
   } while(--i != 0);
   enc_obj_ptr->a_data3[4] = d0;
   enc_obj_ptr->a_data3[5] = d1;
}
#else

/****************************************************************************
*
*     Function   : WBAMR_Level_calculation
*     Purpose    : Calculate signal level in a Sub_fr1x16-band. Level is cal
*                  by summing absolute values of the input data.
*
*                  Signal level calculated from of the end of the frame
*                  (data[count1 - count2]) is stored to (*sub_level)
*                  and added to the level of the next frame.
*
*****************************************************************************/

WBAMR_CODE_SECTION fract16 WBAMR_Level_calculation ( /* return: signal level */
  fract16 data[],             /* i   : signal buffer               */
  fract16 * sub_level,        /* i   : level calculated at the end */
    			      /* o   : level of signal calculated  */
			      /* (count2 - count1) samples         */
  fract16 count1,             /* i   : number of samples           */
  fract16 count2,             /* i   : number of samples to count  */
  fract16 ind_m,              /* i   : step size for the index     */
  fract16 ind_a,              /* i   : starting index              */
  fract16 scale               /* i   : scaling for the level cal   */
)
{
  int_native i;
  fract32 l_temp1, l_temp2;
  fract16 level;

  l_temp1 = 0L;

  for (i = count1; i < count2; i++)
  {
    l_temp1 = Mac_fr16_32(l_temp1, 1, Abs_fr1x16(data[ind_m * i + ind_a]));
  }

  l_temp2 = Add_fr1x32(l_temp1, Shl_fr1x32(*sub_level, Sub_fr1x16(16, scale)));
  *sub_level = Extract_hi_fr1x32(Shl_fr1x32(l_temp1, scale));      

  for (i = 0; i < count1; i++)
  {
    l_temp2 = Mac_fr16_32(l_temp2, 1, Abs_fr1x16(data[ind_m * i + ind_a]));
  }

  level = Extract_hi_fr1x32(Shl_fr1x32(l_temp2, scale));

  return level;

}

/***************************************************************************
*
*     Function     : WBAMR_Filter5
*     Purpose      : Fifth-order half-band lowpass/highpass filter pair with
*                    decimation.
*
****************************************************************************/

static INLINE void WBAMR_Filter5 (
   fract16 * in0,       /* i/o : input values; output low-pass part  */
   fract16 * in1,       /* i/o : input values; output high-pass part */
   fract16 data[]       /* i/o : filter memory                       */
)
{
  fract16 temp0, temp1, temp2;

  temp0 = Sub_fr1x16(*in0, Mult_fr1x16(COEFF5_1, data[0]));
  temp1 = Add_fr1x16(data[0], Mult_fr1x16(COEFF5_1, temp0));
  data[0] = temp0;
  temp0 = Sub_fr1x16(*in1, Mult_fr1x16(COEFF5_2, data[1]));
  temp2 = Add_fr1x16(data[1], Mult_fr1x16(COEFF5_2, temp0));
  data[1] = temp0;
  *in0 = Extract_hi_fr1x32(Shl_fr1x32(Add_fr1x32(temp1, temp2), 15));   
  *in1 = Extract_hi_fr1x32(Shl_fr1x32(Sub_fr1x32(temp1, temp2), 15));   

}


/***************************************************************************
*
*     Function     : WBAMR_Filter3
*     Purpose      : Third-order half-band lowpass/highpass filter pair with
*                    decimation.
*
*****************************************************************************/
WBAMR_CODE_SECTION void WBAMR_Filter3 (
  fract16 * in0,           /* i/o : input values; output low-pass part  */
  fract16 * in1,           /* i/o : input values; output high-pass part */
  fract16 * data           /* i/o : filter memory                       */
)
{
  fract16 temp1, temp2;

  temp1 = Sub_fr1x16(*in1, Mult_fr1x16(COEFF3, *data));
  temp2 = Add_fr1x16(*data, Mult_fr1x16(COEFF3, temp1));
  *data = temp1;

  *in1 = Extract_hi_fr1x32(Shl_fr1x32(Sub_fr1x32(*in0, temp2), 15));    
  *in0 = Extract_hi_fr1x32(Shl_fr1x32(Add_fr1x32(*in0, temp2), 15));    

}

/**************************************************************************
*
*     Function     : filter_bank
*     Purpose      : Divide input signal into bands
*
***************************************************************************/

WBAMR_CODE_SECTION void WBAMR_filter_bank (
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr, /* i/o : State struct */
  fract16 in[],                              /* i: input frame  */
  fract16 tmp_buf[]
)
{
  int_native i;

  /* shift input 1 bit down for safe scaling */
  for (i = 0; i < FRAME_LEN; i++)
  {
    tmp_buf[i] = Shr_fr1x16(in[i], 1);
  }

  /* run the filter bank */
  for (i = 0; i < FRAME_LEN / 2; i++)
  {
    WBAMR_Filter5(&tmp_buf[2*i], &tmp_buf[2*i + 1],enc_obj_ptr->a_data5[0]);
  }

  for (i = 0; i < FRAME_LEN / 4; i++)
  {
    WBAMR_Filter5(&tmp_buf[4*i], &tmp_buf[4*i + 2], enc_obj_ptr->a_data5[1]);
    WBAMR_Filter5(&tmp_buf[4*i + 1],&tmp_buf[4*i + 3],enc_obj_ptr->a_data5[2]);
  }

  for (i = 0; i < FRAME_LEN / 8; i++)
  {
    WBAMR_Filter5(&tmp_buf[8*i], &tmp_buf[8*i + 4], enc_obj_ptr->a_data5[3]);
    WBAMR_Filter5(&tmp_buf[8*i +2],&tmp_buf[8*i +6], enc_obj_ptr->a_data5[4]);
    WBAMR_Filter3(&tmp_buf[8*i +3],&tmp_buf[8*i +7], &enc_obj_ptr->a_data3[0]);
  }

  for (i = 0; i < FRAME_LEN / 16; i++)
  {
    WBAMR_Filter3(&tmp_buf[16*i+0],&tmp_buf[16*i+8],&enc_obj_ptr->a_data3[1]);
    WBAMR_Filter3(&tmp_buf[16*i+4],&tmp_buf[16*i+12],&enc_obj_ptr->a_data3[2]);
    WBAMR_Filter3(&tmp_buf[16*i+6],&tmp_buf[16*i+14],&enc_obj_ptr->a_data3[3]);
  }

  for (i = 0; i < FRAME_LEN / 32; i++)
  {
    WBAMR_Filter3(&tmp_buf[32*i+0],&tmp_buf[32*i+16],&enc_obj_ptr->a_data3[4]);
    WBAMR_Filter3(&tmp_buf[32*i+8],&tmp_buf[32*i+24],&enc_obj_ptr->a_data3[5]);
  }

}
#endif

/**************************************************************************
*
*     Function     : level_cal
*     Purpose      : Calculate level of the signal in each band
*
***************************************************************************/
WBAMR_CODE_SECTION void WBAMR_level_cal (
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr, /* i/o : State struct */
  fract16 in[],                              /* i: input frame  */
  fract16 level[],                           /* 0: signal levels */
#if (AWB_OPT_VER == 2)
  fract32 tmp_buf[]
#else
  fract16 tmp_buf[]
#endif
)
{
// Eddy 2008.06.23 Remove warning
//  int_native i;

  /* calculate levels in each frequency band */
  /* 4800 - 6400 Hz */
  level[11] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[11],
              FRAME_LEN / 4 - 48, FRAME_LEN / 4, 4, 1, 14);

  /* 4000 - 4800 Hz */
  level[10] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[10],
              FRAME_LEN / 8 - 24, FRAME_LEN / 8, 8, 7, 15);

  /* 3200 - 4000 Hz */
  level[9] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[9],
             FRAME_LEN / 8 - 24, FRAME_LEN / 8, 8, 3, 15);

  /* 2400 - 3200 Hz */
  level[8] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[8],
             FRAME_LEN / 8 - 24, FRAME_LEN / 8, 8, 2, 15);

  /* 2000 - 2400 Hz */
  level[7] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[7],
             FRAME_LEN / 16 - 12, FRAME_LEN / 16, 16, 14, 16);

  /* 1600 - 2000 Hz */
  level[6] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[6],
             FRAME_LEN / 16 - 12, FRAME_LEN / 16, 16, 6, 16);

  /* 1200 - 1600 Hz */
  level[5] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[5],
             FRAME_LEN / 16 - 12, FRAME_LEN / 16, 16, 4, 16);

  /* 800 - 1200 Hz */
  level[4] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[4],
             FRAME_LEN / 16 - 12, FRAME_LEN / 16, 16, 12, 16);

  /* 600 - 800 Hz */
  level[3] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[3],
             FRAME_LEN / 32 - 6, FRAME_LEN / 32, 32, 8, 17);

  /* 400 - 600 Hz */
  level[2] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[2],
             FRAME_LEN / 32 - 6, FRAME_LEN / 32, 32, 24, 17);

  /* 200 - 400 Hz */
  level[1] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[1],
             FRAME_LEN / 32 - 6, FRAME_LEN / 32, 32, 16, 17);

  /* 0 - 200 Hz */
  level[0] = WBAMR_Level_calculation(tmp_buf, &enc_obj_ptr->sub_level[0],
             FRAME_LEN / 32 - 6, FRAME_LEN / 32, 32, 0, 17);

}


/**************************************************************************
*
*     Function   : update_cntrl
*     Purpose    : Control update of the background noise estimate.
*
***************************************************************************/

WBAMR_CODE_SECTION void WBAMR_update_cntrl (
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr, /* i/o : State structure */
  fract16 level[]     /* i: Sub_fr1x16-band levels of the input frame */
)
{
  int_native i;
  fract16 temp, stat_rat, exp;
  fract16 num, denom;
  fract16 alpha;

  /* if a tone has been detected for a while, initialize stat_count */
  if (Sub_fr1x16((fract16) (enc_obj_ptr->tone_flag & 0x7c00), 0x7c00) == 0)
  {
     enc_obj_ptr->stat_count = STAT_COUNT;
  }
  else
  {
    /* if 8 last vad-decisions have been "0", reinitialize stat_count */
    if ((enc_obj_ptr->vadreg & 0x7f80) == 0)
    {
      enc_obj_ptr->stat_count = STAT_COUNT;
    }
    else
    {
      stat_rat = 0;

      for (i = 0; i < COMPLEN; i++)
      {
        if (Sub_fr1x16(level[i], enc_obj_ptr->ave_level[i]) > 0)
        {
          num = level[i];
          denom = enc_obj_ptr->ave_level[i];
        }
        else
        {
          num = enc_obj_ptr->ave_level[i];
          denom = level[i];
        }

        /* Limit nimimum value of num and denom to STAT_THR_LEVEL */
        if (Sub_fr1x16(num, STAT_THR_LEVEL) < 0)
        {
          num = STAT_THR_LEVEL;
        }

        if (Sub_fr1x16(denom, STAT_THR_LEVEL) < 0)
        {
          denom = STAT_THR_LEVEL;
        }

        exp = WBAMR_Norm_s(denom);
        denom = Shl_fr1x16(denom, exp);

        /* stat_rat = num/denom * 64 */
        temp = Divs_fr1x16(Shr_fr1x16(num, 1), denom);
        stat_rat = Add_fr1x16(stat_rat, Shr_fr1x16(temp, Sub_fr1x16(8, exp)));

      }

      /* compare stat_rat with a threshold and update stat_count */
      if (Sub_fr1x16(stat_rat, STAT_THR) > 0)
      {
        enc_obj_ptr->stat_count = STAT_COUNT;
      }
      else
      {
        if ((enc_obj_ptr->vadreg & 0x4000) != 0)
        {
          if (enc_obj_ptr->stat_count != 0)
          {
            enc_obj_ptr->stat_count = Sub_fr1x16(enc_obj_ptr->stat_count, 1);
          }

        }

      }

    }

  }

  /* Update average amplitude estimate for stationarity estimation */
  alpha = ALPHA4;

  if (Sub_fr1x16(enc_obj_ptr->stat_count, STAT_COUNT) == 0)
  {
    alpha = 32767;
  }
  else if ((enc_obj_ptr->vadreg & 0x4000) == 0)
  {
    alpha = ALPHA5;
  }

  for (i = 0; i < COMPLEN; i++)
  {
    enc_obj_ptr->ave_level[i] = Add_fr1x16(enc_obj_ptr->ave_level[i],
    Multr_fr1x16(alpha, Sub_fr1x16(level[i], enc_obj_ptr->ave_level[i])));
  }

}


/*************************************************************************
*
*     Function     : hangover_addition
*     Purpose      : Add hangover after speech bursts
*
**************************************************************************/

WBAMR_CODE_SECTION fract16 WBAMR_hangover_addition(
				/* return: VAD_flag indicating VAD dec */
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr, /* i/o : State structure */
  fract16 low_power,                    /* i: flag power of the i/p fr*/
  fract16 hang_len,                     /* i: hangover length */
  fract16 burst_len                     /* i: min burst for hangover */
)
{

  /* if the input power (pow_sum) is lower than a threshold, clear counters and set VAD_flag to "0"         */
  if (low_power != 0)
  {
    enc_obj_ptr->burst_count = 0;
    enc_obj_ptr->hang_count = 0;

    return 0;
  }

  /* update the counters (hang_count, burst_count) */
  if ((enc_obj_ptr->vadreg & 0x4000) != 0)
  {
    enc_obj_ptr->burst_count = Add_fr1x16(enc_obj_ptr->burst_count, 1);

    if (Sub_fr1x16(enc_obj_ptr->burst_count, burst_len) >= 0)
    {
      enc_obj_ptr->hang_count = hang_len;
    }

   return 1;

  }
  else
  {
    enc_obj_ptr->burst_count = 0;

    if (enc_obj_ptr->hang_count > 0)
    {
      enc_obj_ptr->hang_count = Sub_fr1x16(enc_obj_ptr->hang_count, 1);

      return 1;
    }

  }

  return 0;

}


/*************************************************************************
*
*     Function   : noise_estimate_update
*     Purpose    : Update of background noise estimate
*
***************************************************************************/

WBAMR_CODE_SECTION void WBAMR_Noise_estimate_update (
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr, /* i/o : State structure */
  fract16 level[]    /* i   : Sub_fr1x16-band levels of the input frame */
)
{
  int_native i;
  fract16 alpha_up, alpha_down, bckr_add;
  fract16 temp;

  /* Control update of bckr_est[] */
  WBAMR_update_cntrl(enc_obj_ptr, level);

  /* Reason for using bckr_add is to avoid problems caused by fixed-point
   * dynamics when noise level and required change is very small.
   */
  bckr_add = 2;

  /* Choose update speed */
  if ((0x7800 & enc_obj_ptr->vadreg) == 0)
  {
    alpha_up = ALPHA_UP1;
    alpha_down = ALPHA_DOWN1;
  }
  else
  {
    if ((enc_obj_ptr->stat_count == 0))
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
    temp = Sub_fr1x16(enc_obj_ptr->old_level[i], enc_obj_ptr->bckr_est[i]);

    if (temp < 0)
    {                                  /* update downwards */
      enc_obj_ptr->bckr_est[i] = Add_fr1x16(-2, Add_fr1x16(
      	  enc_obj_ptr->bckr_est[i],Multr_fr1x16(alpha_down, temp)));

      /* limit minimum value of the noise estimate to NOISE_MIN */
      if (Sub_fr1x16(enc_obj_ptr->bckr_est[i], NOISE_MIN) < 0)
      {
        enc_obj_ptr->bckr_est[i] = NOISE_MIN;
      }
    }
    else
    {                                /* update upwards */
      enc_obj_ptr->bckr_est[i] = Add_fr1x16(bckr_add,
           Add_fr1x16(enc_obj_ptr->bckr_est[i], Multr_fr1x16(alpha_up, temp)));

      /* limit maximum value of the noise estimate to NOISE_MAX */
      if (Sub_fr1x16(enc_obj_ptr->bckr_est[i], NOISE_MAX) > 0)
      {
        enc_obj_ptr->bckr_est[i] = NOISE_MAX;
      }
    }
  }

  /* Update signal levels of the previous frame (old_level) */
  for (i = 0; i < COMPLEN; i++)
  {
    enc_obj_ptr->old_level[i] = level[i];
  }

}


/************************************************************************
*
*     Function     : vad_decision
*     Purpose      : Calculates VAD_flag
*
*************************************************************************/

WBAMR_CODE_SECTION fract16 WBAMR_vad_decision( /* return value: VAD_flag */
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr,   /* i/o :State structure */
  fract16 level[COMPLEN],                      /* i: band levels of i/p */
  fract32 pow_sum                              /* i: power of the i/p */
)
{
  int_native i;
  fract32 L_WBAMR_snr_sum;
  fract32 L_temp;
  fract16 vad_thr, temp, noise_level;
  fract16 low_power_flag;
  fract16 hang_len, burst_len;
  fract16 ilog2_speech_level, ilog2_noise_level;
  fract16 temp2;
  fract16 exp;

  /* Calculate squared sum of the input levels (level) divided by the
   * background noise components (bckr_est).
   */
  L_WBAMR_snr_sum = 0;

  for (i = 0; i < COMPLEN; i++)
  {
    exp = WBAMR_Norm_s(enc_obj_ptr->bckr_est[i]);
    temp = Shl_fr1x16(enc_obj_ptr->bckr_est[i], exp);
    temp = Divs_fr1x16(Shr_fr1x16(level[i], 1), temp);
    temp = Shl_fr1x16(temp, Sub_fr1x16(exp, UNIRSHFT - 1));
    L_WBAMR_snr_sum = Mac_fr16_32(L_WBAMR_snr_sum, temp, temp);

  }

  /* Calculate average level of estimated background noise */
  L_temp = 0;

  for (i = 1; i < COMPLEN; i++)          /* ignore lowest band */
  {
    L_temp = Add_fr1x32(L_temp, enc_obj_ptr->bckr_est[i]);
  }

  noise_level = Extract_hi_fr1x32(Shl_fr1x32(L_temp, 12));

  /* if WBAMR_snr is lower than a threshold (MIN_SPEECH_WBAMR_snr),
   * and increase speech_level
   */
  temp = Shl_fr1x16(Mult_fr1x16(noise_level, MIN_SPEECH_SNR), 3);

  if (Sub_fr1x16(enc_obj_ptr->speech_level, temp) < 0)
  {
    enc_obj_ptr->speech_level = temp;
  }

  ilog2_noise_level = WBAMR_ilog2(noise_level);

  /* If WBAMR_snr is very poor, speech_level is probably corrupted by noise level.
   * This is correctred by subtracting MIN_SPEECH_WBAMR_snr
   * noise_level from speech level
   */
  ilog2_speech_level = WBAMR_ilog2(Sub_fr1x16(enc_obj_ptr->speech_level, temp));

  temp = Add_fr1x16(Mult_fr1x16(NO_SLOPE,
              Sub_fr1x16(ilog2_noise_level, NO_P1)), THR_HIGH);

  temp2 = Add_fr1x16(SP_CH_MIN, Mult_fr1x16(SP_SLOPE,
              Sub_fr1x16(ilog2_speech_level, SP_P1)));

  if (Sub_fr1x16(temp2, SP_CH_MIN) < 0)
  {
    temp2 = SP_CH_MIN;
  }

  if (Sub_fr1x16(temp2, SP_CH_MAX) > 0)
  {
    temp2 = SP_CH_MAX;
  }

  vad_thr = Add_fr1x16(temp, temp2);

  if (Sub_fr1x16(vad_thr, THR_MIN) < 0)
  {
    vad_thr = THR_MIN;
  }

  /* Shift VAD decision register */
  enc_obj_ptr->vadreg = Shr_fr1x16(enc_obj_ptr->vadreg, 1);

  /* Make intermediate VAD decision */
  if (Sub_fr1x32(L_WBAMR_snr_sum, Mult_fr1x32(vad_thr, 512 * COMPLEN)) > 0)
  {
    enc_obj_ptr->vadreg = (fract16) (enc_obj_ptr->vadreg | 0x4000);
  }

  /* check if the input power (pow_sum) is lower than a threshold" */
  if (Sub_fr1x32(pow_sum, VAD_POW_LOW) < 0)
  {
    low_power_flag = 1;
  }
  else
  {
    low_power_flag = 0;
  }

  /* Update background noise estimates */
  WBAMR_Noise_estimate_update(enc_obj_ptr, level);

  /* Calculate values for hang_len and burst_len based on vad_thr */
  hang_len = Add_fr1x16(Mult_fr1x16(HANG_SLOPE,
                       Sub_fr1x16(vad_thr, HANG_P1)), HANG_HIGH);

  if (Sub_fr1x16(hang_len, HANG_LOW) < 0)
  {
    hang_len = HANG_LOW;
  };

  burst_len = Add_fr1x16(Mult_fr1x16(BURST_SLOPE,
                       Sub_fr1x16(vad_thr, BURST_P1)), BURST_HIGH);

  return (WBAMR_hangover_addition(enc_obj_ptr,low_power_flag,
                       hang_len, burst_len));
}


/************************************************************************
*
*     Estimate_Speech()
*     Purpose      : Estimate speech level
*
* Maximum signal level is searched and stored to the variable sp_max.
* The speech frames must locate within SP_EST_COUNT number of frames.
* Thus, noisy frames having occasional VAD = "1" decisions will not
* affect to the estimated speech_level.
*
*************************************************************************/

WBAMR_CODE_SECTION void WBAMR_Estimate_Speech (
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr, /* i/o : State structure */
  fract16 in_level                           /* level of the input frame */
)
{
  fract16 alpha;
  fract16 tmp;

  /* if the required activity count cannot be achieved, reset counters */
  if (Sub_fr1x16(Sub_fr1x16(enc_obj_ptr->sp_est_cnt, enc_obj_ptr->sp_max_cnt),
                            SP_EST_COUNT - SP_ACTIVITY_COUNT) > 0)
  {
    enc_obj_ptr->sp_est_cnt = 0;
    enc_obj_ptr->sp_max = 0;
    enc_obj_ptr->sp_max_cnt = 0;
  }

  enc_obj_ptr->sp_est_cnt = Add_fr1x16(enc_obj_ptr->sp_est_cnt, 1);

  if (((enc_obj_ptr->vadreg & 0x4000) || (Sub_fr1x16(in_level,
            enc_obj_ptr->speech_level) > 0))
            && (Sub_fr1x16(in_level, MIN_SPEECH_LEVEL1) > 0))
  {

     /* update sp_max */
     if (Sub_fr1x16(in_level, enc_obj_ptr->sp_max) > 0)
     {
        enc_obj_ptr->sp_max = in_level;
     }

     enc_obj_ptr->sp_max_cnt = Add_fr1x16(enc_obj_ptr->sp_max_cnt, 1);

     if (Sub_fr1x16(enc_obj_ptr->sp_max_cnt, SP_ACTIVITY_COUNT) >= 0)
     {
        /* update speech estimate */
        tmp = Shr_fr1x16(enc_obj_ptr->sp_max, 1);

        /* select update speed */
        if (Sub_fr1x16(tmp, enc_obj_ptr->speech_level) > 0)
        {
          alpha = ALPHA_SP_UP;
        }
        else
        {
          alpha = ALPHA_SP_DOWN;
        }

        if (Sub_fr1x16(tmp, MIN_SPEECH_LEVEL2) > 0)
        {
          enc_obj_ptr->speech_level = Add_fr1x16(enc_obj_ptr->speech_level,
                                       Multr_fr1x16(alpha, Sub_fr1x16(tmp,
                                         enc_obj_ptr->speech_level)));
        }

        /* clear all counters used for speech estimation */
        enc_obj_ptr->sp_max = 0;
        enc_obj_ptr->sp_max_cnt = 0;
        enc_obj_ptr->sp_est_cnt = 0;

     }

   }

}


/*************************************************************************
*
*  Function:   WBAMR_Wb_vad_reset
*  Purpose:    Initializes state memory
*
**************************************************************************/

WBAMR_CODE_SECTION fract16 WBAMR_Wb_vad_reset(
				/* return: non-zero with error */
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr    /* i/o : State structure */
)
{

  int_native i, j;
  enc_obj_ptr->tone_flag = 0;
  enc_obj_ptr->vadreg = 0;
  enc_obj_ptr->hang_count = 0;
  enc_obj_ptr->burst_count = 0;
  enc_obj_ptr->hang_count = 0;

  /* initialize memory used by the filter bank */
  for (i = 0; i < F_5TH_CNT; i++)
  {
    for (j = 0; j < 2; j++)
    {
      enc_obj_ptr->a_data5[i][j] = 0;
    }
  }

  for (i = 0; i < F_3TH_CNT; i++)
  {
    enc_obj_ptr->a_data3[i] = 0;
  }

  /* initialize the rest of the memory */
  for (i = 0; i < COMPLEN; i++)
  {
    enc_obj_ptr->bckr_est[i] = NOISE_INIT;
    enc_obj_ptr->old_level[i] = NOISE_INIT;
    enc_obj_ptr->ave_level[i] = NOISE_INIT;
    enc_obj_ptr->sub_level[i] = 0;
  }

  enc_obj_ptr->sp_est_cnt = 0;
  enc_obj_ptr->sp_max = 0;
  enc_obj_ptr->sp_max_cnt = 0;
  enc_obj_ptr->speech_level = SPEECH_LEVEL_INIT;
  enc_obj_ptr->prev_pow_sum = 0;

  return 0;
}


/************************************************************************
*
*  Function:   WBAMR_Wb_vad_init
*  Purpose:    Allocates state memory and initializes state memory
*
*************************************************************************/

WBAMR_CODE_SECTION fract16 WBAMR_Wb_vad_init (
                           /* return: non-zero with error, zero for ok. */
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr    /* i/o : State structure */
)
{

  WBAMR_Wb_vad_reset(enc_obj_ptr);

  return 0;

}


/************************************************************************
*
*     Function     : WBAMR_Wb_vad_tone_detection
*     Purpose      : Search maximum pitch gain from a frame. Set tone flag
*                    if pitch gain is high. This is used to detect
*                    signaling tones and other signals with high pitch gain.
*
*************************************************************************/

WBAMR_CODE_SECTION void WBAMR_Wb_vad_tone_detection (
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr, /* i/o : State struct */
  fract16 p_gain                             /* pitch gain */
)
{

  /* update tone flag */
  enc_obj_ptr->tone_flag = Shr_fr1x16(enc_obj_ptr->tone_flag, 1);

  /* if (pitch_gain > TONE_THR) set tone flag */
  if (Sub_fr1x16(p_gain, TONE_THR) > 0)
  {
    enc_obj_ptr->tone_flag = (fract16) (enc_obj_ptr->tone_flag | 0x4000);
  }

}


/************************************************************************
*
*     Function     : wb_vad
*     Purpose      : Main program for Voice Activity Detection (VAD)
*
*************************************************************************/

WBAMR_CODE_SECTION fract16 WBAMR_Wb_vad(
		  /* Return value : VAD Decision, 1 = speech, 0 = noise */
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr,  /* i/o : State structure */
  fract16 in_buf[],                           /* i: sam of the i/p frame */
  WBAMR_Temp_Mem_Enc_Struct  *enc_scr_ptr

)
{

  int_native i;
  fract32 L_temp, pow_sum;
  fract16 VAD_flag, temp;
  wbamr_enc_wb_vad *wbamr_enc_wb_vad_scr_ptr;


    wbamr_enc_wb_vad_scr_ptr = &(enc_scr_ptr->u1.wb_vad_scr);



  /* Calculate power of the input frame. */
  L_temp = 0L;

  for (i = 0; i < FRAME_LEN; i++)
  {
    L_temp = Mac_fr16_32(L_temp, in_buf[i], in_buf[i]);
  }

  /* pow_sum = power of current frame and previous frame */
  pow_sum = Add_fr1x32(L_temp, enc_obj_ptr->prev_pow_sum);

  /* save power of current frame for next call */
  enc_obj_ptr->prev_pow_sum = L_temp;

  /* If input power is very low, clear tone flag */
  if (Sub_fr1x32(pow_sum, POW_TONE_THR) < 0)
  {
    enc_obj_ptr->tone_flag = (fract16) (enc_obj_ptr->tone_flag & 0x1fff);
  }

  /* Run the filter bank and calculate signal levels at each band */
  WBAMR_filter_bank(enc_obj_ptr, in_buf, wbamr_enc_wb_vad_scr_ptr->tmp_buf);

  /*
  printf("a_data:\n%d, ", enc_obj_ptr->a_data5[0][0]);
  printf("%d, ", enc_obj_ptr->a_data5[0][1]);
  printf("%d, ", enc_obj_ptr->a_data5[1][0]);
  printf("%d, ", enc_obj_ptr->a_data5[1][1]);
  printf("%d, ", enc_obj_ptr->a_data5[2][0]);
  printf("%d, ", enc_obj_ptr->a_data5[2][1]);
  printf("%d, ", enc_obj_ptr->a_data5[3][0]);
  printf("%d, ", enc_obj_ptr->a_data5[3][1]);
  printf("%d, ", enc_obj_ptr->a_data5[4][0]);
  printf("%d\n", enc_obj_ptr->a_data5[4][1]);
  printf("%d, ", enc_obj_ptr->a_data3[0]);
  printf("%d, ", enc_obj_ptr->a_data3[1]);
  printf("%d, ", enc_obj_ptr->a_data3[2]);
  printf("%d, ", enc_obj_ptr->a_data3[3]);
  printf("%d, ", enc_obj_ptr->a_data3[4]);
  printf("%d\n", enc_obj_ptr->a_data3[5]);

  printf("tmp_buf:\n");
  for(i = 0; i < FRAME_LEN >> 3; i++) {
     printf("%d %d %d %d %d %d %d %d\n", wbamr_enc_wb_vad_scr_ptr->tmp_buf[i]
                                       , wbamr_enc_wb_vad_scr_ptr->tmp_buf[i+1]
                                       , wbamr_enc_wb_vad_scr_ptr->tmp_buf[i+2]
                                       , wbamr_enc_wb_vad_scr_ptr->tmp_buf[i+3]
                                       , wbamr_enc_wb_vad_scr_ptr->tmp_buf[i+4]
                                       , wbamr_enc_wb_vad_scr_ptr->tmp_buf[i+5]
                                       , wbamr_enc_wb_vad_scr_ptr->tmp_buf[i+6]
                                       , wbamr_enc_wb_vad_scr_ptr->tmp_buf[i+7]);
  }
  */

  WBAMR_level_cal(enc_obj_ptr, in_buf, wbamr_enc_wb_vad_scr_ptr->level,
                            wbamr_enc_wb_vad_scr_ptr->tmp_buf);

  /*
  printf("Level:\n%d %d %d %d %d %d\n%d %d %d %d %d %d\n"
            , wbamr_enc_wb_vad_scr_ptr->level[0]
            , wbamr_enc_wb_vad_scr_ptr->level[1]
            , wbamr_enc_wb_vad_scr_ptr->level[2]
            , wbamr_enc_wb_vad_scr_ptr->level[3]
            , wbamr_enc_wb_vad_scr_ptr->level[4]
            , wbamr_enc_wb_vad_scr_ptr->level[5]
            , wbamr_enc_wb_vad_scr_ptr->level[6]
            , wbamr_enc_wb_vad_scr_ptr->level[7]
            , wbamr_enc_wb_vad_scr_ptr->level[8]
            , wbamr_enc_wb_vad_scr_ptr->level[9]
            , wbamr_enc_wb_vad_scr_ptr->level[10]
            , wbamr_enc_wb_vad_scr_ptr->level[11]);
            */
  /* compute VAD decision */
  VAD_flag = WBAMR_vad_decision(enc_obj_ptr,
                       wbamr_enc_wb_vad_scr_ptr->level, pow_sum);

  //printf("VAD_FLAG: %d\n", VAD_flag);

  /* Calculate input level */
  L_temp = 0;

  for (i = 1; i < COMPLEN; i++)          /* ignore lowest band */
  {
    L_temp = Add_fr1x32(L_temp, wbamr_enc_wb_vad_scr_ptr->level[i]);
  }

  temp = Extract_hi_fr1x32(Shl_fr1x32(L_temp, 12));

  WBAMR_Estimate_Speech(enc_obj_ptr, temp); /* Estimate speech level */

  return (VAD_flag);

}

#endif


