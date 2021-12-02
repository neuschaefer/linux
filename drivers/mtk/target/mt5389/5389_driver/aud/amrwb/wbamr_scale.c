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
 *   WBAMR_SCALE.C
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
#include "wbamr_sections.h"



WBAMR_CODE_SECTION void WBAMR_Scale_sig (
  fract16 x[],           /* (i/o) : signal to scale */
  fract16 lg,            /* (i)   : size of x[] */
  fract16 exp            /* (i)   : exponent: x = Round_fr1x32(x << exp) */
)
{

  int_native i;
  fract32 L_tmp;

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
  fract16 *pX;
  fract32 remain, lg32, exp32;

  if(exp == 0) {
     return;
  }

  pX = x;
  exp32 = (fract32)exp + 16;
  lg32 = (fract32)lg;
  remain = lg32 & 3;
  lg32 >>= 2;
  if(exp32 < 0)
  {
     exp32 = -exp32;
     for (i = lg32 - 1; i >= 0; i--)
     {
       L_tmp = (fract32)(*pX);
       L_tmp >>= exp32;
       *pX++ = Round_fr1x32(L_tmp);
       L_tmp = (fract32)(*pX);
       L_tmp >>= exp32;
       *pX++ = Round_fr1x32(L_tmp);
       L_tmp = (fract32)(*pX);
       L_tmp >>= exp32;
       *pX++ = Round_fr1x32(L_tmp);
       L_tmp = (fract32)(*pX);
       L_tmp >>= exp32;
       *pX++ = Round_fr1x32(L_tmp);
     }

     for(i = remain - 1; i >= 0; i--) {
       L_tmp = (fract32)(*pX);
       L_tmp >>= exp32;
       *pX++ = Round_fr1x32(L_tmp);
     }
  }
  else if( exp32 <= 16)
  {
     for (i = lg32 - 1; i >= 0; i--)
     {
       L_tmp = (fract32)(*pX);
       L_tmp <<= exp32;
       *pX++ = Round_fr1x32(L_tmp);
       L_tmp = (fract32)(*pX);
       L_tmp <<= exp32;
       *pX++ = Round_fr1x32(L_tmp);
       L_tmp = (fract32)(*pX);
       L_tmp <<= exp32;
       *pX++ = Round_fr1x32(L_tmp);
       L_tmp = (fract32)(*pX);
       L_tmp <<= exp32;
       *pX++ = Round_fr1x32(L_tmp);
     }

     for(i = remain - 1; i >= 0; i--) {
       L_tmp = (fract32)(*pX);
       L_tmp <<= exp32;
       *pX++ = Round_fr1x32(L_tmp);
     }
  }
  else
  {
     if(exp32 == 17) {
        for (i = lg32 - 1; i >= 0; i--)
        {
          L_tmp = (fract32)(*pX) << 16;
          L_tmp = AWB_qdadd(0x8000, L_tmp);
          *pX++ = (fract16)(L_tmp >> 16);
          L_tmp = (fract32)(*pX) << 16;
          L_tmp = AWB_qdadd(0x8000, L_tmp);
          *pX++ = (fract16)(L_tmp >> 16);
          L_tmp = (fract32)(*pX) << 16;
          L_tmp = AWB_qdadd(0x8000, L_tmp);
          *pX++ = (fract16)(L_tmp >> 16);
          L_tmp = (fract32)(*pX) << 16;
          L_tmp = AWB_qdadd(0x8000, L_tmp);
          *pX++ = (fract16)(L_tmp >> 16);
        }

        for(i = remain - 1; i >= 0; i--) {
          L_tmp = (fract32)(*pX) << 16;
          L_tmp = AWB_qdadd(0x8000, L_tmp);
          *pX++ = (fract16)(L_tmp >> 16);
        }
     }
     else
     {
        for (i = lg32 - 1; i >= 0; i--)
        {
          L_tmp = (fract32)(*pX);
          L_tmp = Shl_fr1x32(L_tmp, exp32);  /* saturation can occur here */
          *pX++ = Round_fr1x32(L_tmp);
          L_tmp = (fract32)(*pX);
          L_tmp = Shl_fr1x32(L_tmp, exp32);  /* saturation can occur here */
          *pX++ = Round_fr1x32(L_tmp);
          L_tmp = (fract32)(*pX);
          L_tmp = Shl_fr1x32(L_tmp, exp32);  /* saturation can occur here */
          *pX++ = Round_fr1x32(L_tmp);
          L_tmp = (fract32)(*pX);
          L_tmp = Shl_fr1x32(L_tmp, exp32);  /* saturation can occur here */
          *pX++ = Round_fr1x32(L_tmp);
        }

        for(i = remain - 1; i >= 0; i--) {
          L_tmp = (fract32)(*pX);
          L_tmp = Shl_fr1x32(L_tmp, exp32);  /* saturation can occur here */
          *pX++ = Round_fr1x32(L_tmp);
        }
     }
  }
#else
  for (i = 0; i < lg; i++)
  {
    L_tmp = WBAMR_L_deposit_h(x[i]);
    L_tmp = Shl_fr1x32(L_tmp, exp);  /* saturation can occur here */
    x[i] = Round_fr1x32(L_tmp);
  }
#endif

  return;

}

#endif

