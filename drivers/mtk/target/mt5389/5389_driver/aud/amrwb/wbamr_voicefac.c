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
 *   WBAMR_VOICEFAC.C
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
#include "wbamr_math_op.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION fract16 WBAMR_voice_factor ( 
			/* (o) Q15   : factor (-1=unvoiced to 1=voiced) */
  fract16 exc[],        /* (i) Q_exc : pitch excitation                 */
  fract16 Q_exc,        /* (i)       : exc format                       */
  fract16 gain_pit,     /* (i) Q14   : gain of pitch                    */
  fract16 code[],       /* (i) Q9    : Fixed codebook excitation        */
  fract16 gain_code     /* (i) Q0    : gain of code                     */
)
{
    
  int_native i;
  fract16 tmp, exp, ener1, exp1, ener2, exp2;
  fract32 L_tmp;

  ener1 = Extract_hi_fr1x32(WBAMR_Dot_product12(exc, exc, L_SUBFR, &exp1));
  exp1 = Sub_fr1x16(exp1, Add_fr1x16(Q_exc, Q_exc));
  L_tmp = Mult_fr1x32(gain_pit, gain_pit);
  exp = Norm_fr1x32(L_tmp);
  tmp = Extract_hi_fr1x32(Shl_fr1x32(L_tmp, exp));
  ener1 = Mult_fr1x16(ener1, tmp);
  exp1 = Sub_fr1x16(Sub_fr1x16(exp1, exp), 10);        
  ener2 = Extract_hi_fr1x32(WBAMR_Dot_product12(code, code, L_SUBFR, &exp2));
  exp = WBAMR_Norm_s(gain_code);
  tmp = Shl_fr1x16(gain_code, exp);
  tmp = Mult_fr1x16(tmp, tmp);
  ener2 = Mult_fr1x16(ener2, tmp);
  exp2 = Sub_fr1x16(exp2, Add_fr1x16(exp, exp));
  i = Sub_fr1x16(exp1, exp2);

  if (i >= 0)
  {
    ener1 = Shr_fr1x16(ener1, 1);
    ener2 = Shr_fr1x16(ener2, i+1);
  } 
  else
  {
    /*bug with INTRINSIC Shr_fr1x16, modified to Shr_fr1x32 */
    /* ener1 = Shr_fr1x16(ener1, Sub_fr1x16(1, i)); */
    ener1 =(fract16) Shr_fr1x32(ener1, Sub_fr1x16(1, (fract16)i));
    ener2 = Shr_fr1x16(ener2, 1);

  }

  tmp = Sub_fr1x16(ener1, ener2);
  ener1 = Add_fr1x16(Add_fr1x16(ener1, ener2), 1);

  if (tmp >= 0)
  {
    tmp = Divs_fr1x16(tmp, ener1);
    
  } 
  else
  {
    tmp = Negate_fr1x16(Divs_fr1x16(Negate_fr1x16(tmp), ener1));
    
  }

  return (tmp);
}

#endif

