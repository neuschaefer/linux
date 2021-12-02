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
 *   WBAMR_D2T64FX.C
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
 * 12 bits algebraic codebook decoder.                               *
 * 2 tracks x 32 positions per track = 64 samples.                   *
 *                                                                   *
 * 12 bits --> 2 pulses in a frame of 64 samples.                    *
 *                                                                   *
 * All pulses can have two (2) possible amplitudes: +1 or -1.        *
 * Each pulse can have 32 possible positions.                        *
 *                                                                   *
 * See dec2t64.c for more details of the algebraic code.             *
 *-------------------------------------------------------------------*/


#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_DEC_ACELP_2t64_fx (
  fract16 index,   /* (i) :    12 bits index                         */
  fract16 code[]   /* (o) :Q9  algebraic (fixed) codebook excitation */
)
{
  int_native i;
  fract16 i0, i1;

  for ( i = 0; i < L_CODE; i++ )
  {
    code[i] = 0;                       
  }

  /* decode the positions and signs of pulses and build the codeword */
  i0 = (fract16) ( Shr_fr1x16 ( index, 5 ) & 0x003E );
  i1 = (fract16) ( Add_fr1x16 ( Shl_fr1x16( (fract16) ( index & 0x001F ), 1 ),
                                1 ) );  
    
  if ( ( Shr_fr1x16( index, 6 ) & NB_POS ) == 0 )
    code[i0] = 512;
  else
    code[i0] = -512;
    
  if ( ( index & NB_POS ) == 0 )
    code[i1] = 512;
  else
    code[i1] = -512;

  return;
}

#endif

