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
 *   WBAMR_CAELP.DEC_HOMING.C
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

#include "wbamr_private.h"
#include "wbamr_dtx.h"
#include "wbamr_bits.h"
#include "wbamr_basic_op.h"
#include "wbamr_dec_main.h"
#include "wbamr_sections.h"
#include "wbamr_rom.h"


WBAMR_CODE_SECTION fract16 WBAMR_Dhf_test ( 
  uint8 input_frame[], 
  fract16 mode, 
  fract16 nparms
)
{
  fract16 i, j;
  fract16 tmp, shift;
  fract16 param[DHF_PARMS_MAX];
  uint8 *prms;
    
  prms = input_frame;
  j = 0;
  i = 0;
    
  if ( Sub_fr1x16 ( mode, MRDTX ) != 0 )
  {
    if ( Sub_fr1x16 ( mode, MODE_24k ) != 0 ) 
    {
      /* convert the received serial bits */
      tmp = Sub_fr1x16 ( nparms, 15 );
      while ( Sub_fr1x16 ( tmp, j ) > 0 )
      {
        param[i] = WBAMR_Serial_parm ( 15, &prms );
        j = Add_fr1x16 ( j, 15 );
        i = Add_fr1x16 ( i, 1 );
      }
      tmp = Sub_fr1x16 ( nparms, j );
      param[i] = WBAMR_Serial_parm ( tmp, &prms );
      shift = Sub_fr1x16 ( 15, tmp );
      param[i] = Shl_fr1x16 ( param[i], shift );
    }
    else 
    {
      /*If mode is 23.85Kbit/s, remove high band energy bits */
      for ( i = 0; i < 10; i++ )
      {
        param[i] = WBAMR_Serial_parm ( 15, &prms );
      }
      param[10] = WBAMR_Serial_parm ( 15, &prms ) & 0x61FF;
      for ( i = 11; i < 17; i++ )
      {
        param[i] = WBAMR_Serial_parm ( 15, &prms );
      }
      param[17] = WBAMR_Serial_parm ( 15, &prms ) & 0xE0FF;
      for ( i = 18; i < 24; i++ )
      {
        param[i] = WBAMR_Serial_parm ( 15, &prms );
      }
      param[24] = WBAMR_Serial_parm ( 15, &prms ) & 0x7F0F;
      for ( i = 25; i < 31; i++ )
      {
        param[i] = WBAMR_Serial_parm ( 15, &prms );
      }
      tmp = WBAMR_Serial_parm ( 8, &prms );
      param[31] = Shl_fr1x16 ( tmp, 7 );
      shift=0;
    }
        
    /* check if the parameters matches 
       the parameters of the corresponding decoder homing frame */
    tmp = i;
    j = 0;
    for ( i = 0; i < tmp; i++ )
    {
      j = ( fract16 ) ( param[i] ^ dhf[mode][i] );
      if ( j )
        break;
    }
    tmp = 0x7fff;
    tmp = Shr_fr1x16 ( tmp, shift );
    tmp = Shl_fr1x16 ( tmp, shift );
    tmp = ( fract16 ) ( dhf[mode][i] & tmp );
    tmp = ( fract16 ) ( param[i] ^ tmp );
    j = ( fract16 ) ( j | tmp );
  }
  else
  {
    j = 1;
  }
    
  return ( fract16 ) ( !j );
}


WBAMR_CODE_SECTION fract16 WBAMR_Decoder_homing_frame_test ( 
  uint8 input_frame[],
  fract16 mode
)
{
  /* perform test for COMPLETE parameter frame */
  return WBAMR_Dhf_test ( input_frame, mode, nb_of_bits[mode] );
}


WBAMR_CODE_SECTION fract16 WBAMR_Decoder_homing_frame_test_first ( 
  uint8 input_frame[], 
  fract16 mode
)
{
  /* perform test for FIRST SUBFRAME of parameter frame ONLY */
  return WBAMR_Dhf_test ( input_frame, mode, AWB_prmnofsf[mode] );
}


/*-----------------------------------------------------------------*
 *   Funtion  init_decoder	                                   *
 *            ~~~~~~~~~~~~                                         *
 *   ->Initialization of variables for the decoder section.        *
 *-----------------------------------------------------------------*/
 
WBAMR_CODE_SECTION void WBAMR_Dec_Init (  
  WBAMR_Static_Mem_Dec_Struct *dec_obj_ptr 
)
{
  WBAMR_Dtx_dec_init ( dec_obj_ptr, isf_init );
  WBAMR_Reset_decoder ( dec_obj_ptr, 1 );
  WBAMR_Reset_read_serial ( dec_obj_ptr );
}

#endif

