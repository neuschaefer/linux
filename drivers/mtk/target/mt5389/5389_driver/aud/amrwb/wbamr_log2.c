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
 *   WBAMR_LOG2.C
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
 
#include "wbamr_log2.h"
#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"     
#include "wbamr_sections.h"


/*************************************************************************
 *
 *   FUNCTION:   WBAMR_Log2_norm()
 *
 *   PURPOSE:   Computes log2(L_x, exp),  where   L_x is positive and
 *              normalized, and exp is the normalisation exponent
 *              If L_x is negative or zero, the result is 0.
 *
 *   DESCRIPTION:
 *        The function WBAMR_Log2(L_x) is approximated by a table and linear
 *        interpolation. 
 *        The following steps are used to compute WBAMR_Log2(L_x)
 *
 *           1- exponent = 30-norm_exponent
 *           2- i = bit25-b31 of L_x;  32<=i<=63  (because of normalization).
 *           3- a = bit10-b24
 *           4- i -=32
 *           5- fraction = table[i]<<16 - (table[i] - table[i+1]) * a * 2
 *
 *************************************************************************/

WBAMR_CODE_SECTION void WBAMR_Log2_norm (
  fract32 L_x,                         /* i : input value (normalized) */
  fract16 exp,                         /* i : Norm_fr1x32 (L_x)        */
  fract16 *exponent,                   /* o : Integer part of WBAMR_Log2.   
                                                   (range: 0<=val<=30) */
  fract16 *fraction                    /* o : Fractional part of WBAMR_Log2. 
                                                   (range: 0<=val<1)  */
 )

{
  int_native i;
  fract16 a, tmp;
  fract32 L_y;

  if ( L_x <= (fract32) 0 )
  {
    *exponent = 0;           
    *fraction = 0;           
    return;
  }

  *exponent = Sub_fr1x16 ( 30, exp );   

  L_x = Shr_fr1x32 ( L_x, 9 );
  i = Extract_hi_fr1x32 ( L_x );       /* Extract b25-b31 */
  L_x = Shr_fr1x32 ( L_x, 1 );
  a = Extract_lo_fr1x32 ( L_x );       /* Extract b10-b24 of fraction */
  a = (fract16)(a & (fract16)0x7fff);     

  i = i-32;

  L_y = WBAMR_L_deposit_h ( table_log[i] );
                                       /* table[i] << 16        */
  tmp = Sub_fr1x16 ( table_log[i], table_log[i + 1] ); 
                                       /* table[i] - table[i+1] */
  L_y = WBAMR_L_msu ( L_y, tmp, a );   /* L_y -= tmp*a*2        */

  *fraction = Extract_hi_fr1x32 ( L_y ); 

  return;
}


/*************************************************************************
 *
 *   FUNCTION:   WBAMR_Log2()
 *
 *   PURPOSE:   Computes log2(L_x),  where   L_x is positive.
 *              If L_x is negative or zero, the result is 0.
 *
 *   DESCRIPTION:
 *        normalizes L_x and then calls WBAMR_Log2_norm().
 *
 *************************************************************************/

WBAMR_CODE_SECTION void WBAMR_Log2 (
  fract32 L_x,                         /* (i) : input value                 */
  fract16 *exponent,                   /* (o) : Integer part of WBAMR_Log2.   
                                                        (range: 0<=val<=30) */
  fract16 *fraction                    /* (o) : Fractional part of WBAMR_Log2. 
                                                         (range: 0<=val<1)  */
 )

{
  fract16 exp;

  exp = Norm_fr1x32 ( L_x );
  WBAMR_Log2_norm ( Shl_fr1x32 ( L_x, exp ), exp, exponent, fraction );
}

#endif

