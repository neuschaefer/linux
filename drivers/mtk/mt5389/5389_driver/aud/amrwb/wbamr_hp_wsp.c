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
 *   WBAMR_HP_WSP.C
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
#include "wbamr_acelp.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"

 
/*-----------------------------------------------------------------------*
 *                                                                       *
 * Algorithm:                                                            *
 *                                                                       *
 *  y[i] = b[0]*x[i] + b[1]*x[i-1] + b[2]*x[i-2] + b[3]*x[i-3]           *
 *                   + a[1]*y[i-1] + a[2]*y[i-2] + a[3]*y[i-3];          *
 *                                                                       *
 * float a_coef[HP_ORDER] = {                                            *
 *                           -2.64436711600664f,                         *
 *                            2.35087386625360f,                         *
 *                           -0.70001156927424f                          *
 *                           };                                          *
 *                                                                       *
 * float b_coef[HP_ORDER+1] = {                                          *
 *                             -0.83787057505665f,                       *
 *                              2.50975570071058f,                       *
 *                             -2.50975570071058f,                       *
 *                              0.83787057505665f};                      *
 *                                                                       *
 *-----------------------------------------------------------------------*/

/* Initialization of static values */

WBAMR_CODE_SECTION void WBAMR_Init_Hp_wsp ( 
  fract16 mem[]                    /* i/o : filter memory [9]   */
 )

{
  WBAMR_Set_zero ( mem, 9 );
  return;
}


WBAMR_CODE_SECTION void WBAMR_Scale_mem_Hp_wsp ( 
  fract16 mem[],                   /* i/o : filter memory [9]   */ 
  fract16 exp                      /* i   : exponent vector     */
 )

{
  int_native i;
  fract32 L_tmp;

  for ( i = 0; i < 6; i += 2 )
  {
    L_tmp = WBAMR_L_Comp ( mem[i], mem[i + 1] );    /* y_hi, y_lo */
    L_tmp = Shl_fr1x32 ( L_tmp, exp );
    WBAMR_L_Extract ( L_tmp, &mem[i], &mem[i + 1] );
  }

  for ( i = 6; i < 9; i++ )
  {
    L_tmp = WBAMR_L_deposit_h ( mem[i] );           /* x[i] */
    L_tmp = Shl_fr1x32 ( L_tmp, exp );
    mem[i] = Round_fr1x32 ( L_tmp );             
  }

  return;
}


WBAMR_CODE_SECTION void WBAMR_Hp_wsp ( 
  fract16 wsp[],                   /* i   : wsp[]  signal       */
  fract16 WBAMR_Hp_wsp[],          /* o   : hypass wsp[]        */
  fract16 lg,                      /* i   : length of signal    */
  fract16 mem[]                    /* i/o : filter memory [9]   */
 )

{
  int_native i;
  fract16 x0, x1, x2, x3;
  fract16 y3_hi, y3_lo, y2_hi, y2_lo, y1_hi, y1_lo;
  fract32 L_tmp;

  y3_hi = mem[0];                        
  y3_lo = mem[1];                        
  y2_hi = mem[2];                        
  y2_lo = mem[3];                        
  y1_hi = mem[4];                        
  y1_lo = mem[5];                        
  x0    = mem[6];                           
  x1    = mem[7];                           
  x2    = mem[8];                           

  for ( i = 0; i < lg; i++ )
  {
    x3 = x2;                           
    x2 = x1;                           
    x1 = x0;                           
    x0 = wsp[i];                       

    /* y[i] = b[0]*x[i] + b[1]*x[i-1] + b140[2]*x[i-2] + b[3]*x[i-3]  */
    /*                  + a[1]*y[i-1] + a[2] * y[i-2]  + a[3]*y[i-3]  */

    L_tmp = 16384L;                    /* rounding to maximise precision */
    L_tmp = Mac_fr16_32 ( L_tmp, y1_lo, a_hpw[1] );
    L_tmp = Mac_fr16_32 ( L_tmp, y2_lo, a_hpw[2] );
    L_tmp = Mac_fr16_32 ( L_tmp, y3_lo, a_hpw[3] );
    L_tmp = Shr_fr1x32 ( L_tmp, 15 );
    L_tmp = Mac_fr16_32 ( L_tmp, y1_hi, a_hpw[1] );
    L_tmp = Mac_fr16_32 ( L_tmp, y2_hi, a_hpw[2] );
    L_tmp = Mac_fr16_32 ( L_tmp, y3_hi, a_hpw[3] );
    L_tmp = Mac_fr16_32 ( L_tmp, x0, b_hpw[0] );
    L_tmp = Mac_fr16_32 ( L_tmp, x1, b_hpw[1] );
    L_tmp = Mac_fr16_32 ( L_tmp, x2, b_hpw[2] );
    L_tmp = Mac_fr16_32 ( L_tmp, x3, b_hpw[3] );

    L_tmp = Shl_fr1x32 ( L_tmp, 2 );   /* coeff Q12 --> Q15 */

    y3_hi = y2_hi;                     
    y3_lo = y2_lo;                     
    y2_hi = y1_hi;                     
    y2_lo = y1_lo;                     

    WBAMR_L_Extract ( L_tmp, &y1_hi, &y1_lo );

    L_tmp = Shl_fr1x32 ( L_tmp, 1 );   /* coeff Q14 --> Q15 */

    WBAMR_Hp_wsp[i] = Round_fr1x32 ( L_tmp );          
  }

  mem[0] = y3_hi;                        
  mem[1] = y3_lo;                        
  mem[2] = y2_hi;                        
  mem[3] = y2_lo;                        
  mem[4] = y1_hi;                        
  mem[5] = y1_lo;                        
  mem[6] = x0;                           
  mem[7] = x1;                           
  mem[8] = x2;                           

  return;
}

#endif

