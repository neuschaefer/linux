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
 *   WBAMR_D_GAIN2.C
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
 * $Revision: #1 $ 1.0.0
 * $Modtime:$  
 * $Log:$
 *
 * 12 23 2010 eddy.wu
 * [WCPSP00000503] Checkin SWIP Modification
 * .
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_oper_32b.h"
#include "wbamr_math_op.h"
#include "wbamr_log2.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_acelp.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Init_D_gain2 ( 
  fract16 * mem          /* output  :static memory ( 4 words ) */
)
{
  int_native i;

  /* 4nd order quantizer energy predictor ( init to -14.0 in Q10 ) */
  mem[0] = -14336;                         /* past_qua_en[0] */
  mem[1] = -14336;                         /* past_qua_en[1] */
  mem[2] = -14336;                         /* past_qua_en[2] */
  mem[3] = -14336;                         /* past_qua_en[3] */

  mem[4] = 0;                              /* *past_gain_pit  */
  mem[5] = 0;                              /* *past_gain_code */
  mem[6] = 0;                              /* *prev_gc */

  for ( i = 0; i < 5; i++ )
  {
    mem[i + 7] = 0;                        /* pbuf[i] */
  }
  for ( i = 0; i < 5; i++ )
  {
    mem[i + 12] = 0;                       /* gbuf[i] */
  }
  for ( i = 0; i < 5; i++ )
  {
    mem[i + 17] = 0;                       /* pbuf2[i] */
  }
  mem[22] = 21845;                         /* seed */

  return;
}


WBAMR_CODE_SECTION void WBAMR_D_gain2 ( 
  fract16 index,                         /* (i)    : index of quantization. */
  fract16 nbits,                         /* (i)    : number of bits(6 or 7) */
  fract16 code[],                        /* (i)Q9  : Innovative vector.     */
  fract16 * gain_pit,                    /* (o)Q14 : Pitch gain.            */
  fract32 * gain_cod,                    /* (o)Q16 : Code gain.             */
  fract16 bfi,                           /* (i)    : bad frame indicator    */
  fract16 unusable_frame,                /* (i)    : UF indicator           */
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr
)
{
  int_native i;
  fract32 L_tmp;
  const fract16 *p;
  fract16 *past_gain_pit, *past_gain_code,
          *past_qua_en, *gbuf, *pbuf, *prev_gc;
  fract16 *pbuf2;//, *seed;
  fract16 tmp, exp, frac, gcode0, exp_gcode0, qua_ener, gcode_inov;
  fract16 g_code;
  fract16 prev_bfi,state,vad_hist,*mem;
    
  prev_bfi=dec_obj_ptr->prev_bfi;
  state=dec_obj_ptr->state;
  vad_hist=dec_obj_ptr->vad_hist;
  mem=dec_obj_ptr->dec_gain;

  past_qua_en = mem;                     
  past_gain_pit = mem + 4;               
  past_gain_code = mem + 5;              
  prev_gc = mem + 6;                     
  pbuf = mem + 7;                        
  gbuf = mem + 12;                       
  pbuf2 = mem + 17;                      
  //seed = mem + 22;                       

  /*-----------------------------------------------------------------*
   *  Find energy of code and compute:                               *
   *                                                                 *
   *    L_tmp = 1.0 / sqrt ( energy of code/ L_SUBFR )               *
   *-----------------------------------------------------------------*/

  L_tmp = WBAMR_Dot_product12 ( code, code, L_SUBFR, &exp );
  exp = Sub_fr1x16 ( exp, 18 + 6 );   /* exp: -18(code in Q9), -6(/L_SUBFR) */

  WBAMR_Isqrt_n ( &L_tmp, &exp );

  gcode_inov = Extract_hi_fr1x32 ( Shl_fr1x32 ( L_tmp, Sub_fr1x16 ( exp, 3 ) ) );  
                                      /* g_code_inov in Q12 */

  /*-------------------------------*
   * Case of erasure.              *
   *-------------------------------*/
    
  if ( bfi != 0 )
  {
    tmp = WBAMR_Median5 ( &pbuf[2] );
    *past_gain_pit = tmp;              
        
    if ( Sub_fr1x16 ( *past_gain_pit, 15565 ) > 0 )
    {
      *past_gain_pit = 15565;         /* 0.95 in Q14 */
    }
        
    if ( unusable_frame != 0 )
    {
      *gain_pit = Mult_fr1x16 ( pdown_unusable[state], *past_gain_pit );    
    }
    else
    {
      *gain_pit = Mult_fr1x16 ( pdown_usable[state], *past_gain_pit );      
    }
    tmp = WBAMR_Median5 ( &gbuf[2] );
        
    if ( Sub_fr1x16 ( vad_hist, 2 ) > 0 )
    {
      *past_gain_code = tmp;         
    }
    else
    {
    if ( unusable_frame != 0 )
    {
      *past_gain_code = Mult_fr1x16 ( cdown_unusable[state], tmp );     
    }
    else
    {
      *past_gain_code = Mult_fr1x16 ( cdown_usable[state], tmp );       
    }
  }

  /* update table of past quantized energies */
  L_tmp = Mult_fr1x32 ( past_qua_en[0], 8192 );            /* x 0.25 */
  L_tmp = Mac_fr16_32 ( L_tmp, past_qua_en[1], 8192 );     /* x 0.25 */
  L_tmp = Mac_fr16_32 ( L_tmp, past_qua_en[2], 8192 );     /* x 0.25 */
  L_tmp = Mac_fr16_32 ( L_tmp, past_qua_en[3], 8192 );     /* x 0.25 */
  qua_ener = Extract_hi_fr1x32 ( L_tmp );
  qua_ener = Sub_fr1x16 ( qua_ener, 3072 );                /* -3 in Q10 */
        
  if ( Sub_fr1x16 ( qua_ener, -14336 ) < 0 )
    qua_ener = -14336;                                     /* -14 in Q10 */

  past_qua_en[3] = past_qua_en[2];   
  past_qua_en[2] = past_qua_en[1];   
  past_qua_en[1] = past_qua_en[0];   
  past_qua_en[0] = qua_ener;         

  for ( i = 1; i < 5; i++ )
  {
    gbuf[i - 1] = gbuf[i];         
  }
  gbuf[4] = *past_gain_code;         

  for ( i = 1; i < 5; i++ )
  {
    pbuf[i - 1] = pbuf[i];         
  }
  pbuf[4] = *past_gain_pit;          

  /* adjust gain according to energy of code */
  /* past_gain_code ( Q3 ) * gcode_inov ( Q12 ) => Q16 */
  *gain_cod = Mult_fr1x32 ( *past_gain_code, gcode_inov );        

  return;
  }
  /*-----------------------------------------------------------------*
   * Compute gcode0.                                                 *
   *  = Sum ( i=0,1 ) pred[i]*past_qua_en[i] + mean_ener - ener_code *
   *-----------------------------------------------------------------*/

  L_tmp = WBAMR_L_deposit_h ( MEAN_ENER );              /* MEAN_ENER in Q16 */
  L_tmp = Shl_fr1x32 ( L_tmp, 8 );                      /* From Q16 to Q24 */
  L_tmp = Mac_fr16_32 ( L_tmp, pred[0], past_qua_en[0] ); /* Q13*Q10 -> Q24 */
  L_tmp = Mac_fr16_32 ( L_tmp, pred[1], past_qua_en[1] ); /* Q13*Q10 -> Q24 */
  L_tmp = Mac_fr16_32 ( L_tmp, pred[2], past_qua_en[2] ); /* Q13*Q10 -> Q24 */
  L_tmp = Mac_fr16_32 ( L_tmp, pred[3], past_qua_en[3] ); /* Q13*Q10 -> Q24 */

  gcode0 = Extract_hi_fr1x32 ( L_tmp );                   /* From Q24 to Q8 */

  /*-----------------------------------------------------------------*
   * gcode0 = pow ( 10.0, gcode0/20 )                                *
   *        = pow ( 2, 3.321928*gcode0/20 )                          *
   *        = pow ( 2, 0.166096*gcode0 )                             *
   *-----------------------------------------------------------------*/

  L_tmp = Mult_fr1x32 ( gcode0, 5443 );         /* *0.166096 in Q15 -> Q24   */
  L_tmp = Shr_fr1x32 ( L_tmp, 8 );              /* From Q24 to Q16           */
  WBAMR_L_Extract ( L_tmp, &exp_gcode0, &frac );/* Extract exponant of gcode0*/

  gcode0 = Extract_lo_fr1x32 ( WBAMR_Pow2 ( 14, frac ) );
  /* Put 14 as exponant so that  */
  /* output of WBAMR_Pow2 (  ) will be:   */
  /* 16384 < WBAMR_Pow2 (  ) <= 32767     */
  exp_gcode0 = Sub_fr1x16 ( exp_gcode0, 14 );

  /* Read the quantized gains */
  if ( Sub_fr1x16 ( nbits, 6 ) == 0 )
  {
    p = &t_qua_gain6b[Add_fr1x16 ( index, index )];   
  }
  else
  {
    p = &t_qua_gain7b[Add_fr1x16 ( index, index )];   
  }
  *gain_pit = *p++;                        /* selected pitch gain in Q14 */
  g_code = *p++;                           /* selected code gain in Q11  */

  L_tmp = Mult_fr1x32 ( g_code, gcode0 );  /* Q11*Q0 -> Q12 */
  L_tmp = Shl_fr1x32 ( L_tmp, Add_fr1x16 ( exp_gcode0, 4 ) ); /* Q12 -> Q16 */

  *gain_cod = L_tmp;                       /* gain of code in Q16 */
    
  if (  ( Sub_fr1x16 ( prev_bfi, 1 ) == 0 ) )
  {
    L_tmp = Mult_fr1x32 ( *prev_gc, 5120 ); /* prev_gc(Q3) * 1.25(Q12)=Q16 */
    /* if((*gain_cod > ((*prev_gc) * 1.25))&&(*gain_cod > 100.0)) */
        
    if ( ( Sub_fr1x32 ( *gain_cod, L_tmp ) > 0 ) &&
         ( Sub_fr1x32 ( *gain_cod, 6553600 ) > 0 ) )
    {
      *gain_cod = L_tmp;             
    }
  }
  /* keep past gain code in Q3 for frame erasure (can saturate) */
  *past_gain_code = Round_fr1x32 ( Shl_fr1x32 ( *gain_cod, 3 ) );       
  *past_gain_pit = *gain_pit;            

  *prev_gc = *past_gain_code;            
  for ( i = 1; i < 5; i++ )
  {
    gbuf[i - 1] = gbuf[i];             
  }
  gbuf[4] = *past_gain_code;             

  for ( i = 1; i < 5; i++ )
  {
    pbuf[i - 1] = pbuf[i];             
  }
  pbuf[4] = *past_gain_pit;              

  for ( i = 1; i < 5; i++ )
  {
    pbuf2[i - 1] = pbuf2[i];           
  }
  pbuf2[4] = *past_gain_pit;             

  /* adjust gain according to energy of code */
  WBAMR_L_Extract ( *gain_cod, &exp, &frac );
  L_tmp = WBAMR_Mpy_32_16 ( exp, frac, gcode_inov );
  *gain_cod = Shl_fr1x32 ( L_tmp, 3 );  /* gcode_inov in Q12 */

  /*---------------------------------------------------*
   * qua_ener = 20*log10 ( g_code )                    *
   *          = 6.0206*log2 ( g_code )                 *
   *          = 6.0206* ( log2 ( g_codeQ11 ) - 11 )    *
   *---------------------------------------------------*/

  L_tmp = WBAMR_L_deposit_l ( g_code );
  WBAMR_Log2 ( L_tmp, &exp, &frac );
  exp = Sub_fr1x16 ( exp, 11 );
  L_tmp = WBAMR_Mpy_32_16 ( exp, frac, 24660 );          /* x 6.0206 in Q12 */

  qua_ener = Extract_lo_fr1x32 ( Shr_fr1x32 ( L_tmp, 3 ) ); /*result in Q10*/

  /* update table of past quantized energies */

  past_qua_en[3] = past_qua_en[2];       
  past_qua_en[2] = past_qua_en[1];       
  past_qua_en[1] = past_qua_en[0];       
  past_qua_en[0] = qua_ener;             

  return;
}

#endif

