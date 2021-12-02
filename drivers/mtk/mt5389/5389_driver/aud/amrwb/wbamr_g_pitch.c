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
 *   WBAMR_G_PITCH.C
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


WBAMR_CODE_SECTION fract16 WBAMR_G_pitch (  
                          /* (o) Q14 : Gain of pitch lag saturated to 1.2   */
  fract16 xn[],           /* (i)     : Pitch target.                        */
  fract16 y1[],           /* (i)     : filtered adaptive codebook.          */
  fract16 g_coeff[]       /* : Correlations need for gain quantization.     */
 )

{
  int_native i;
  fract32 L_sum;
  fract16 xy, yy, exp_xy, exp_yy, gain, sft;
  
  

  /* Compute scalar product <y1[],y1[]> */
  yy = Extract_hi_fr1x32 ( WBAMR_Dot_product12 ( y1, 
                                    y1, L_SUBFR, &exp_yy ) );

  
  /* 
    xy = Extract_hi_fr1x32 ( WBAMR_Dot_product12 ( 
                                xn, y1, L_SUBFR, &exp_xy ) );
  
  */
  
  L_sum = 1L;                            
  for (i = 0; i < L_SUBFR; i++ )
    L_sum = Mac_fr16_32 ( L_sum, xn[i], y1[i] );

  
  sft = Norm_fr1x32 ( L_sum );
  xy = Extract_hi_fr1x32(Shl_fr1x32 ( L_sum, sft ));
  exp_xy = Sub_fr1x16 ( 30, sft );       
                             /* exponent = 0..30 */

  g_coeff[0] = yy;                       
  g_coeff[1] = exp_yy;                   
  g_coeff[2] = xy;                       
  g_coeff[3] = exp_xy;                   

  /* If (xy < 0) gain = 0 */
  if ( xy < 0 )
    return ( (fract16) 0 );

  /* compute gain = xy/yy */
  xy = Shr_fr1x16 ( xy, 1 );                /* Be sure xy < yy */
  gain = Divs_fr1x16 ( xy, yy );

  i = Add_fr1x16 ( exp_xy, 1 - 1 );         /* -1 -> gain in Q14 */
  i = Sub_fr1x16 ( (fract16)i, exp_yy );

  gain = Shl_fr1x16 ( gain, (fract16)i );   /* saturation can occur here */

    /* if (gain > 1.2) gain = 1.2  in Q14 */
    
  if ( Sub_fr1x16( gain, 19661 ) > 0)
  {
    gain = 19661;                      
  }
  
  return ( gain );
}

#endif

