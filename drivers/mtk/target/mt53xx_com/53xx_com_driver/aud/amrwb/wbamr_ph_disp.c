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
 *   WBAMR_PH_DISP.C
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


WBAMR_CODE_SECTION void WBAMR_Init_Phase_dispersion(
  fract16 disp_mem[]                     /* (i/o): static memory (size = 8) */
)
{

  WBAMR_Set_zero(disp_mem, 8);

  return;

}


WBAMR_CODE_SECTION void WBAMR_Phase_dispersion(
  fract16 gain_code,                     /* (i) Q0  : gain of code */
  fract16 gain_pit,                      /* (i) Q14 : gain of pitch */
  fract16 code[],                        /* (i/o)   : code vector */
  fract16 mode,                          /* (i): level, 0=hi, 1=lo, 2=off */
  fract16 disp_mem[],                    /* (i/o): static memory (size = 8) */
  WBAMR_Temp_Mem_Dec_Struct  *dec_scr_ptr
)
{

  int_native i, j;
  fract16 state;
  fract16 *prev_gain_pit, *prev_gain_code, *prev_state;
  fract16 *code2 = dec_scr_ptr->u.code2;

  prev_state = disp_mem;
  prev_gain_code = disp_mem + 1;
  prev_gain_pit = disp_mem + 2;

  WBAMR_Set_zero(code2, 2 * L_SUBFR);

  if (Sub_fr1x16(gain_pit, PITCH_0_6) < 0)
    state = 0;

  else if (Sub_fr1x16(gain_pit, PITCH_0_9) < 0)
    state = 1;

  else
    state = 2;

  for (i = 5; i > 0; i--)
  {
    prev_gain_pit[i] = prev_gain_pit[i - 1];
  }

  prev_gain_pit[0] = gain_pit;

  if (Sub_fr1x16(Sub_fr1x16(gain_code, *prev_gain_code),
                          Shl_fr1x16(*prev_gain_code, 1)) > 0)
  {
    /* onset */
    if (Sub_fr1x16(state, 2) < 0)
      state = Add_fr1x16(state, 1);
  }
  else
  {
    j = 0;

    for (i = 0; i < 6; i++)
    {
      if (Sub_fr1x16(prev_gain_pit[i], PITCH_0_6) < 0)
        j = j+1;
    }

    if ((j-2) > 0)
    {
      state = 0;
    }

    if (Sub_fr1x16(Sub_fr1x16(state, *prev_state), 1) > 0)
      state = Sub_fr1x16(state, 1);
  }

  *prev_gain_code = gain_code;
  *prev_state = state;

  /* circular convolution */
  state = Add_fr1x16(state, mode);              /* level of dispersion */

#if (AWB_OPT_VER == 2)
   if (state == 0 || state == 1)
   {
      fract16 *pCode2, *pCode;
      fract32 wCode, wC0, wCode2;
      const fract16 *pImpLow;

      pCode2 = code2;
      pCode = code;
      pImpLow = ph_imp_low;

      i = L_SUBFR;
      do {
         if((wCode = *pCode) == 0) {
            wC0 = *pCode2 << 16;
            wCode2 = *(pCode2 + L_SUBFR) << 16;
            pCode2++;
         }
         else {
            wC0 = *pCode2;
            wC0 = AWB_qadd(wC0 << 16, AWB_smulbb(wCode, *pImpLow++));
            *pCode2++ = wC0 >> 16;
            j = 63; // L_SUBFR - 1
            do {
               wCode2 = *pCode2;
               wCode2 = AWB_qadd(wCode2 << 16, AWB_smulbb(wCode, *pImpLow++));
               *pCode2++ = wCode2 >> 16;
            } while(--j != 0);
            pCode2 -= 63; // L_SUBFR - 1
            pImpLow -= L_SUBFR;
         }
         *pCode++ = AWB_qadd(wC0, wCode2) >> 16;
      } while(--i != 0);

   }
   else if (state < 2)
   {
      for (i = 0; i < L_SUBFR; i++)
      {
         code[i] = Add_fr1x16(code2[i], code2[i + L_SUBFR]);
      }
   }
#else
  if (state == 0)
  {
    for (i = 0; i < L_SUBFR; i++)
    {
      if (code[i] != 0)
      {
        for (j = 0; j < L_SUBFR; j++)
        {
          code2[i + j] = Add_fr1x16(code2[i + j], Multr_fr1x16(code[i],
                                       ph_imp_low[j]));
        }
      }
    }
  }
  else if (Sub_fr1x16(state, 1) == 0)
  {
    for (i = 0; i < L_SUBFR; i++)
    {
      if (code[i] != 0)
      {
        for (j = 0; j < L_SUBFR; j++)
        {
          code2[i + j] = Add_fr1x16(code2[i + j], Multr_fr1x16(code[i],
                                      ph_imp_mid[j]));
        }
      }
    }
  }

  if (Sub_fr1x16(state, 2) < 0)
  {
    for (i = 0; i < L_SUBFR; i++)
    {
      code[i] = Add_fr1x16(code2[i], code2[i + L_SUBFR]);
    }
  }
#endif

  return;

}

#endif


