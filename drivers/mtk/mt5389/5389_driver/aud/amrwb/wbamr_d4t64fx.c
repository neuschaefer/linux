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
 *   WBAMR_D4T64FX.C
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

 /*------------------------------------------------------------------*
 *                         D2T64FX.C				     *
 *-------------------------------------------------------------------*
 * 20, 36, 44, 52, 64, 72, 88 bits algebraic codebook decoder.       *
 * 4 tracks x 16 positions per track = 64 samples.                   *
 *                                                                   *
 * 20 bits --> 4 pulses in a frame of 64 samples.                    *
 * 36 bits --> 8 pulses in a frame of 64 samples.                    *
 * 44 bits --> 10 pulses in a frame of 64 samples.                   *
 * 52 bits --> 12 pulses in a frame of 64 samples.                   *
 * 64 bits --> 16 pulses in a frame of 64 samples.                   *
 * 72 bits --> 18 pulses in a frame of 64 samples.                   *
 * 88 bits --> 24 pulses in a frame of 64 samples.                   *
 *                                                                   *
 * All pulses can have two ( 2 ) possible amplitudes: +1 or -1.        *
 * Each pulse can have sixteen ( 16 ) possible positions.              *
 *                                                                   *
 * See c36_64fx.c for more details of the algebraic code.            *
 *-------------------------------------------------------------------*/


#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_private.h"
#include "wbamr_acelp.h"
#include "wbamr_cnst.h"
#include "wbamr_q_pulse.h"
#include "wbamr_sections.h"


/* local function */
WBAMR_CODE_SECTION void WBAMR_DEC_ACELP_4t64_fx ( 
  fract16 nbbits,   /* (i) : 20, 36, 44, 52, 64, 72 or 88 bits        */
  fract16 index[],  /* (i) : 12 bits index                            */
  fract16 code[],   /* (o) :Q9  algebraic (fixed) codebook excitation */
  WBAMR_Temp_Mem_Dec_Struct *dec_scr_ptr
)
{
  int_native i, k;
  fract32 L_index;
  fract16 *pos = dec_scr_ptr->u.pos;

  for ( i = 0; i < L_CODE; i++ )
  {
    code[i] = 0;                       
  }

  /* decode the positions and signs of pulses and build the codeword */
  if ( Sub_fr1x16 ( nbbits, 20 ) == 0 )
  {
    for ( k = 0; k < NB_TRACK4; k++ )
    {
      L_index = index[k];            
      WBAMR_Dec_1p_N1 ( L_index, 4, 0, pos );
      WBAMR_Add_pulses ( pos, 1, ( fract16 )k, code );
    }
  }
  else if ( Sub_fr1x16 ( nbbits, 36 ) == 0 )
  {
    for ( k = 0; k < NB_TRACK4; k++ )
    {
      L_index = index[k];            
      WBAMR_Dec_2p_2N1 ( L_index, 4, 0, pos );
      WBAMR_Add_pulses ( pos, 2, ( fract16 )k, code );
    }
  } 
  else if ( Sub_fr1x16 ( nbbits, 44 ) == 0 )
  {
    for ( k = 0; k < NB_TRACK4 - 2; k++ )
    {
      L_index = index[k];            
      WBAMR_Dec_3p_3N1 ( L_index, 4, 0, pos );
      WBAMR_Add_pulses ( pos, 3, ( fract16 )k, code );
    }
    for ( k = 2; k < NB_TRACK4; k++ )
    {
      L_index = index[k];            
      WBAMR_Dec_2p_2N1 ( L_index, 4, 0, pos );
      WBAMR_Add_pulses ( pos, 2, ( fract16 )k, code );
    }
  } 
  else if ( Sub_fr1x16 ( nbbits, 52 ) == 0 )
  {
    for ( k = 0; k < NB_TRACK4; k++ )
    {
      L_index = index[k];            
      WBAMR_Dec_3p_3N1 ( L_index, 4, 0, pos );
      WBAMR_Add_pulses ( pos, 3, ( fract16 )k, code );
    }
  } 
  else if ( Sub_fr1x16 ( nbbits, 64 ) == 0 )
  {
    for ( k = 0; k < NB_TRACK4; k++ )
    {
      L_index = Add_fr1x32 ( Shl_fr1x32 ( index[k], 14 ),
                             index[k + NB_TRACK4] );
      WBAMR_Dec_4p_4N ( L_index, 4, 0, pos );
      WBAMR_Add_pulses ( pos, 4, ( fract16 )k, code );
    }
  } 
  else if ( Sub_fr1x16 ( nbbits, 72 ) == 0 )
  {
    for ( k = 0; k < NB_TRACK4 - 2; k++ )
    {
      L_index = Add_fr1x32 ( Shl_fr1x32 ( index[k], 10 ),
                             index[k + NB_TRACK4] );
      WBAMR_Dec_5p_5N ( L_index, 4, 0, pos );
      WBAMR_Add_pulses ( pos, 5, ( fract16 )k, code );
    }
    for ( k = 2; k < NB_TRACK4; k++ )
    {
      L_index = Add_fr1x32 ( Shl_fr1x32 ( index[k], 14 ),
                             index[k + NB_TRACK4] );
      WBAMR_Dec_4p_4N ( L_index, 4, 0, pos );
      WBAMR_Add_pulses ( pos, 4, ( fract16 )k, code );
    }
  } 
  else if ( Sub_fr1x16 ( nbbits, 88 ) == 0 )
  {
    for ( k = 0; k < NB_TRACK4; k++ )
    {
      L_index = Add_fr1x32 ( Shl_fr1x32 ( index[k], 11 ),
                             index[k + NB_TRACK4] );
      WBAMR_Dec_6p_6N_2 ( L_index, 4, 0, pos );
      WBAMR_Add_pulses ( pos, 6, ( fract16 )k, code );
    }
  }
  
  return;
}


WBAMR_CODE_SECTION void WBAMR_Add_pulses ( 
  fract16 pos[], 
  fract16 nb_pulse, 
  fract16 track, 
  fract16 code[]
)
{
  int_native i, k;

  for ( k = 0; k < nb_pulse; k++ )
  {
    /* i = ( ( pos[k] & ( NB_POS-1 ) )*NB_TRACK4 ) + track; */
    i = (fract16) ( Add_fr1x16 ( 
                        Shl_fr1x16 ( (fract16) ( pos[k] & ( NB_POS16 - 1 ) ),
                                      2 ),
                        track ) );    
        
    if ( ( pos[k] & NB_POS16 ) == 0 )
      code[i] = Add_fr1x16 ( code[i], 512 );
    else
      code[i] = Sub_fr1x16 ( code[i], 512 );
  }

  return;
}

#endif

