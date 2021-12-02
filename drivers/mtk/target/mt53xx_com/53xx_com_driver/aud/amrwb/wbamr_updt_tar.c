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
 *   WBAMR_UPDT_TAR.C
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
#include "wbamr_cnst.h"
#include "wbamr_sections.h"

#if (AWB_OPT_VER == 1 || AWB_OPT_VER== 2)
// Memory adjust, Eddy

WBAMR_CODE_SECTION void WBAMR_Updt_tar_interleave (
   fract16 * x,          /* (i) Q0  : old target (for pitch search)     */
   fract16 * x2,         /* (o) Q0  : new target (for codebook search)  */
   fract16 * y,          /* (i) Q0  : filtered adaptive codebook vector */
   fract16 gain          /* (i) Q14 : adaptive codebook gain            */
)
{

   int_native i;

   fract32 L_tmp;


   fract16 *p1, *p2, *p3;
   p1 = x2 + 16;
   p2 = p1 + 16;
   p3 = p2 + 16;

   i = 16;
   do
   {
      L_tmp = AWB_smulbb(*x++, 16384);
      L_tmp = L_tmp - AWB_smulbb(*y++, gain);
      *x2++ = Extract_hi_fr1x32(Shl_fr1x32(L_tmp, 2));

      L_tmp = AWB_smulbb(*x++, 16384);
      L_tmp = L_tmp - AWB_smulbb(*y++, gain);
      *p1++ = Extract_hi_fr1x32(Shl_fr1x32(L_tmp, 2));

      L_tmp = AWB_smulbb(*x++, 16384);
      L_tmp = L_tmp - AWB_smulbb(*y++, gain);
      *p2++ = Extract_hi_fr1x32(Shl_fr1x32(L_tmp, 2));

      L_tmp = AWB_smulbb(*x++, 16384);
      L_tmp = L_tmp - AWB_smulbb(*y++, gain);
      *p3++ = Extract_hi_fr1x32(Shl_fr1x32(L_tmp, 2));
   } while (--i != 0);

   return;

}
#endif

WBAMR_CODE_SECTION void WBAMR_Updt_tar (
  fract16 * x,          /* (i) Q0  : old target (for pitch search)     */
  fract16 * x2,         /* (o) Q0  : new target (for codebook search)  */
  fract16 * y,          /* (i) Q0  : filtered adaptive codebook vector */
  fract16 gain          /* (i) Q14 : adaptive codebook gain            */
)
{

  int_native i;

  fract32 L_tmp;


  for (i = 0; i < L_SUBFR; i++)
  {
     L_tmp = Mult_fr1x32(x[i], 16384);

     L_tmp = WBAMR_L_msu(L_tmp, y[i], gain);

     x2[i] = Extract_hi_fr1x32(Shl_fr1x32(L_tmp, 1));  
  }

  return;

}

#endif

