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
 *   WBAMR_QISF_NS.C
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
#include "wbamr_acelp.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Qisf_ns(
  fract16 * isf1,     /* input : ISF in the frequency domain (0..0.5) */
  fract16 * isf_q,    /* output: quantized ISF */
  fract16 * indice    /* output: quantization indices */
)
{
  int_native i;
  fract32 tmp;

  for (i = 0; i < ORDER; i++)
  {
    isf_q[i] = Sub_fr1x16(isf1[i], mean_isf_noise[i]);     
  }

  indice[0] = WBAMR_Sub_VQ( &isf_q[0], 
                           dico1_isf_noise, 2, SIZE_BK_NOISE1, &tmp);    
                           
  indice[1] = WBAMR_Sub_VQ(&isf_q[2], 
                           dico2_isf_noise, 3, SIZE_BK_NOISE2, &tmp);    
  
  indice[2] = WBAMR_Sub_VQ(&isf_q[5], 
                           dico3_isf_noise, 3, SIZE_BK_NOISE3, &tmp);    
  
  indice[3] = WBAMR_Sub_VQ(&isf_q[8], 
                           dico4_isf_noise, 4, SIZE_BK_NOISE4, &tmp);    
                           
  indice[4] = WBAMR_Sub_VQ(&isf_q[12], 
                           dico5_isf_noise, 4, SIZE_BK_NOISE5, &tmp);   

  /* decoding the ISFs */
  WBAMR_Disf_ns(indice, isf_q);

  return;

}


/*-------------------------------------------------------------------*
 * routine:   WBAMR_Disf_ns()                                        *
 *            ~~~~~~~~~                                              *
 * Decoding of ISF parameters                                        *
 *-------------------------------------------------------------------*
 *  Arguments:                                                       *
 *    indice[] : indices of the selected codebook entries            *
 *    isf[]    : quantized ISFs (in frequency domain)                *
 *-------------------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Disf_ns(
  fract16 * indice,    /* input:  quantization indices */
  fract16 * isf_q      /* input : ISF in the frequency domain (0..0.5) */
)
{
    
  int_native i;

  for (i = 0; i < 2; i++)
  {
    isf_q[i] = dico1_isf_noise[indice[0] * 2 + i];  
  }
 
  for (i = 0; i < 3; i++)
  {
    isf_q[i + 2] = dico2_isf_noise[indice[1] * 3 + i];      
  }
   
  for (i = 0; i < 3; i++)
  {
    isf_q[i + 5] = dico3_isf_noise[indice[2] * 3 + i];      
  }
  
  for (i = 0; i < 4; i++)
  {
    isf_q[i + 8] = dico4_isf_noise[indice[3] * 4 + i];      
  }
  
  for (i = 0; i < 4; i++)
  {
    isf_q[i + 12] = dico5_isf_noise[indice[4] * 4 + i];     
  }

  for (i = 0; i < ORDER; i++)
  {
    isf_q[i] = Add_fr1x16(isf_q[i], mean_isf_noise[i]);    
  }

  WBAMR_Reorder_isf(isf_q, ISF_GAP, ORDER);

  return;

}

#endif

