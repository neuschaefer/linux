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
 *   WBAMR_HP7K.C
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
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_acelp.h"
#include "wbamr_private.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Init_Filt_7k (
  fract16 mem[]                        /* i/o : memory ( size=30)   */
 )

{
  WBAMR_Set_zero ( mem, L_FIR - 1 );

  return;
}


WBAMR_CODE_SECTION void WBAMR_Filt_7k (
  fract16 signal[],                    /* i   : input signal       */
  fract16 mem[],                       /* i/o : memory ( size=30 ) */
  fract16 x[]                          /* i/o                      */
 )

{
#if (AWB_OPT_VER == 2)
  int_native i, j;
  fract32 *pX, *pSig, *pFir;
  fract32 L_tmp1, L_tmp2, L_tmp3, L_tmp4;
  fract32 wA, wB, wC, wD;

  WBAMR_Copy ( mem, x, L_FIR - 1 );
  pX = (fract32 *)(&x[L_FIR - 1]);
  pSig = (fract32 *)signal;

  i = 10;
  do
  {
     *pX++ = *pSig++;
     *pX++ = *pSig++;
     *pX++ = *pSig++;
     *pX++ = *pSig++;
  } while(--i != 0);

  pFir = (fract32 *)fir_7k;
  pX = (fract32 *)x;
//4p
  i = 20;
  do {
     wA = *pFir++;
     wC = *pX++;
     L_tmp1 = AWB_smlabb(wA, wC, 0x4000);
     L_tmp2 = AWB_smlabt(wA, wC, 0x4000);
     L_tmp1 = AWB_smlatt(wA, wC, L_tmp1);
     wD = *pX++;
     L_tmp3 = AWB_smlabb(wA, wD, 0x4000);
     L_tmp2 = AWB_smlatb(wA, wD, L_tmp2);
     L_tmp4 = AWB_smlabt(wA, wD, 0x4000);
     L_tmp3 = AWB_smlatt(wA, wD, L_tmp3);
     wB = *pFir++;
     L_tmp1 = AWB_smlabb(wB, wD, L_tmp1);
     L_tmp2 = AWB_smlabt(wB, wD, L_tmp2);
     L_tmp1 = AWB_smlatt(wB, wD, L_tmp1);

     j = 6;
     do
     {
	     wC = *pX++;
	     L_tmp3 = AWB_smlabb(wB, wC, L_tmp3);
	     L_tmp4 = AWB_smlabt(wB, wC, L_tmp4);
	     L_tmp2 = AWB_smlatb(wB, wC, L_tmp2);
	     L_tmp3 = AWB_smlatt(wB, wC, L_tmp3);
	     L_tmp4 = AWB_smlatb(wA, wC, L_tmp4);
	     wA = *pFir++;
	     L_tmp1 = AWB_smlabb(wA, wC, L_tmp1);
	     L_tmp2 = AWB_smlabt(wA, wC, L_tmp2);
	     L_tmp1 = AWB_smlatt(wA, wC, L_tmp1);
	     wD = *pX++;
	     L_tmp3 = AWB_smlabb(wA, wD, L_tmp3);
	     L_tmp4 = AWB_smlabt(wA, wD, L_tmp4);
	     L_tmp2 = AWB_smlatb(wA, wD, L_tmp2);
	     L_tmp3 = AWB_smlatt(wA, wD, L_tmp3);
	     L_tmp4 = AWB_smlatb(wB, wD, L_tmp4);
	     wB = *pFir++;
	     L_tmp1 = AWB_smlabb(wB, wD, L_tmp1);
	     L_tmp2 = AWB_smlabt(wB, wD, L_tmp2);
	     L_tmp1 = AWB_smlatt(wB, wD, L_tmp1);
     } while( --j != 0);
	  wC = *pX++;
	  L_tmp3 = AWB_smlabb(wB, wC, L_tmp3);
	  L_tmp4 = AWB_smlabt(wB, wC, L_tmp4);
	  L_tmp2 = AWB_smlatb(wB, wC, L_tmp2);
	  L_tmp3 = AWB_smlatt(wB, wC, L_tmp3);
	  L_tmp4 = AWB_smlatb(wA, wC, L_tmp4);
	  wA = *pFir++;
	  L_tmp1 = AWB_smlabb(wA, wC, L_tmp1);
	  L_tmp2 = AWB_smlabt(wA, wC, L_tmp2);
	  L_tmp1 = AWB_smlatt(wA, wC, L_tmp1);
	  wD = *pX++;
	  L_tmp3 = AWB_smlabb(wA, wD, L_tmp3);
	  L_tmp4 = AWB_smlabt(wA, wD, L_tmp4);
	  L_tmp2 = AWB_smlatb(wA, wD, L_tmp2);
	  L_tmp3 = AWB_smlatt(wA, wD, L_tmp3);
	  L_tmp4 = AWB_smlatb(wB, wD, L_tmp4);
	  wB = *pFir++;
	  L_tmp1 = AWB_smlabb(wB, wD, L_tmp1);
	  L_tmp2 = AWB_smlabt(wB, wD, L_tmp2);
	  wC = *pX++;
	  L_tmp3 = AWB_smlabb(wB, wC, L_tmp3);
	  L_tmp4 = AWB_smlabt(wB, wC, L_tmp4);
	  L_tmp4 = AWB_smlatb(wA, wC, L_tmp4);

     *signal++ = (fract16)(L_tmp1 >> 15);
     *signal++ = (fract16)(L_tmp2 >> 15);
     *signal++ = (fract16)(L_tmp3 >> 15);
     *signal++ = (fract16)(L_tmp4 >> 15);

     pFir -= 16;
     pX -= 15;
  }while(--i != 0);
#else
  int_native i, j;
  fract32 L_tmp;

  WBAMR_Copy ( mem, x, L_FIR - 1 );
  for ( i = 0; i < L_SUBFR16k; i++ )
  {
    x[i + L_FIR - 1] = signal[i];
  }

  for ( i = 0; i < L_SUBFR16k; i++ )
  {
    L_tmp = 0;
    for ( j = 0; j < L_FIR; j++ )
       L_tmp = Mac_fr16_32 ( L_tmp, x[i + j], fir_7k[j] );

    signal[i] = Round_fr1x32 ( L_tmp );
  }
#endif

  WBAMR_Copy ( x + L_SUBFR16k, mem, L_FIR - 1 );

  return;
}

#endif

