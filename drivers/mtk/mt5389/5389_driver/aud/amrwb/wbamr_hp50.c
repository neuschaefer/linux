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
 *   WBAMR_HP50.C
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

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_oper_32b.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_acelp.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"


/*-----------------------------------------------------------------------*
 * 2nd order high pass filter with cut off frequency at 31 Hz.           *
 * Designed with cheby2 function in MATLAB.                              *
 * Optimized for fixed-point to get the following frequency response:    *
 *                                                                       *
 *  frequency:     0Hz    14Hz  24Hz   31Hz   37Hz   41Hz   47Hz         *
 *  dB loss:     -infdB  -15dB  -6dB   -3dB  -1.5dB  -1dB  -0.5dB        *
 *                                                                       *
 * Algorithm:                                                            *
 *                                                                       *
 *  y[i] = b[0]*x[i] + b[1]*x[i-1] + b[2]*x[i-2]                         *
 *                   + a[1]*y[i-1] + a[2]*y[i-2];                        *
 *                                                                       *
 *  fract16 b[3] = {4053, -8106, 4053};       in Q12                     *
 *  fract16 a[3] = {8192, 16211, -8021};       in Q12                    *
 *                                                                       *
 *  float -->   b[3] = {0.989501953, -1.979003906,  0.989501953};        *
 *              a[3] = {1.000000000,  1.978881836, -0.979125977};        *
 *-----------------------------------------------------------------------*/

/* Initialization of static values */

WBAMR_CODE_SECTION void WBAMR_Init_HP50_12k8 (
  fract16 mem[]              /* i/o: filter memory [6]   */
 )

{
    WBAMR_Set_zero ( mem, 6 );
}


WBAMR_CODE_SECTION void WBAMR_HP50_12k8 (
  fract16 signal[],          /* i/o: input/output signal */
  fract16 lg,                /* i  : lenght of signal    */
  fract16 mem[]              /* i/o: filter memory [6]   */
 )

{
  int_native i;
  fract16 x2;
  fract16 x0, x1;
  fract32 L_tmp;

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
   fract32 a_hp5 = 0x3F53E0AB; // T: 16211, B: -8021
   fract32 b_hp5 = 0x0FD5E056; // T: 4053, B: -8106
   fract32 L_tmp2;
   fract32 y2, y1;

   y2 = (mem[0] << 16) + mem[1];
   y1 = (mem[2] << 16) + mem[3];
   x0    = mem[4];
   x1    = mem[5];

   i = lg;
   do
   {
      x2 = x1;
      x1 = x0;
      x0 = *signal;

    /* y[i] = b[0]*x[i] + b[1]*x[i-1] + b140[2]*x[i-2]  */
    /*                  + a[1]*y[i-1] + a[2] * y[i-2];  */

      L_tmp = 8192L;
      L_tmp = AWB_smlabt ( y1, a_hp5, L_tmp );
      L_tmp = AWB_smlabb ( y2, a_hp5, L_tmp );
      L_tmp >>= 14;
      L_tmp2 = AWB_smultt ( y1, a_hp5);
      L_tmp2 = AWB_smlatb ( y2, a_hp5, L_tmp2 );
      L_tmp2 = AWB_smlabt ( x0, b_hp5, L_tmp2 );
      L_tmp2 = AWB_smlabb ( x1, b_hp5, L_tmp2 );
      L_tmp2 = AWB_smlabt ( x2, b_hp5, L_tmp2 );
      L_tmp = AWB_qdadd(L_tmp, L_tmp2);

      L_tmp <<= 2;

      y2 = y1;

      y1 = ((L_tmp >> 16) << 16) | ((unsigned int)(L_tmp << 16) >> 17);

#if __CC_ARM
      L_tmp =AWB_qadd(L_tmp, L_tmp);    /* coeff Q14 --> Q15 with saturation */
#else
      L_tmp = Shl_fr1x32 ( L_tmp, 1 );
#endif

      *signal++ = Round_fr1x32 ( L_tmp );
   } while( --i != 0);

   mem[0] = y2 >> 16;
   mem[1] = (y2 << 16) >> 16;
   mem[2] = y1 >> 16;
   mem[3] = (y1 << 16) >> 16;
   mem[4] = x0;
   mem[5] = x1;
#else
  fract16 y2_hi, y2_lo, y1_hi, y1_lo;
  y2_hi = mem[0];
  y2_lo = mem[1];
  y1_hi = mem[2];
  y1_lo = mem[3];
  x0    = mem[4];
  x1    = mem[5];

  for ( i = 0; i < lg; i++ )
  {
    x2 = x1;
    x1 = x0;
    x0 = signal[i];

    /* y[i] = b[0]*x[i] + b[1]*x[i-1] + b140[2]*x[i-2]  */
    /*                  + a[1]*y[i-1] + a[2] * y[i-2];  */

    L_tmp = 16384L;                    /* rounding to maximise precision */
    L_tmp = Mac_fr16_32 ( L_tmp, y1_lo, a_hp5[1] );
    L_tmp = Mac_fr16_32 ( L_tmp, y2_lo, a_hp5[2] );
    L_tmp = Shr_fr1x32 ( L_tmp, 15 );
    L_tmp = Mac_fr16_32 ( L_tmp, y1_hi, a_hp5[1] );
    L_tmp = Mac_fr16_32 ( L_tmp, y2_hi, a_hp5[2] );
    L_tmp = Mac_fr16_32 ( L_tmp, x0, b_hp5[0] );
    L_tmp = Mac_fr16_32 ( L_tmp, x1, b_hp5[1] );
    L_tmp = Mac_fr16_32 ( L_tmp, x2, b_hp5[2] );

    L_tmp = Shl_fr1x32 ( L_tmp, 2 );    /* coeff Q12 --> Q14 */

    y2_hi = y1_hi;
    y2_lo = y1_lo;

    WBAMR_L_Extract ( L_tmp, &y1_hi, &y1_lo );

    L_tmp = Shl_fr1x32 ( L_tmp, 1 );    /* coeff Q14 --> Q15 with saturation */
    signal[i] = Round_fr1x32 ( L_tmp );
  }

  mem[0] = y2_hi;
  mem[1] = y2_lo;
  mem[2] = y1_hi;
  mem[3] = y1_lo;
  mem[4] = x0;
  mem[5] = x1;
#endif

  return;
}

#endif

