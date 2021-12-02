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
 *   WBAMR_DTX.C
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
#include "wbamr_math_op.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_acelp.h" 
#include "wbamr_bits.h"
#include "wbamr_dtx.h"
#include "wbamr_log2.h"
#include "wbamr_private.h"
#include "wbamr_sections.h"


/************************************************************************
 *
 *
 * Function    : WBAMR_Dtx_enc_init
 *
 *
 ************************************************************************/
 
WBAMR_CODE_SECTION fract16 WBAMR_Dtx_enc_init ( 
  WBAMR_Static_Mem_Enc_Struct *enc_obj_ptr, 
  const fract16 isf_init[]
 )

{
  WBAMR_Dtx_enc_reset ( enc_obj_ptr, isf_init );
   
  return 0;
}


/************************************************************************
 *
 *
 * Function    : WBAMR_Dtx_enc_reset
 *
 *
 ************************************************************************/

WBAMR_CODE_SECTION fract16 WBAMR_Dtx_enc_reset (
  WBAMR_Static_Mem_Enc_Struct *enc_obj_ptr, 
  const fract16 isf_init[]
 )

{
  int_native i;

  enc_obj_ptr->hist_ptr = 0;                      
  enc_obj_ptr->log_en_index = 0;                  
  /* added for NSYNC functionality */
  enc_obj_ptr->nsync_vad_latch = 0;   

  /* Init isf_hist[] */
  for ( i = 0; i < DTX_HIST_SIZE; i++ )
  {
    WBAMR_Copy ( (fract16 *)isf_init, &enc_obj_ptr->isf_hist[i * M11], M11 );
  }
  
  enc_obj_ptr->cng_seed = RANDOM_INITSEED;        

  /* Reset energy history */
  WBAMR_Set_zero ( enc_obj_ptr->log_en_hist, DTX_HIST_SIZE );

  enc_obj_ptr->dtxHangoverCount = DTX_HANG_CONST; 
  enc_obj_ptr->decAnaElapsedCount = 32767;        

  for (i = 0; i < 28; i++ )
  {
    enc_obj_ptr->D[i] = 0;                      
  }

  for (i = 0; i < DTX_HIST_SIZE - 1; i++ )
  {
    enc_obj_ptr->sumD[i] = 0;                   
  }

  return 1;
}


/************************************************************************
 *
 *
 * Function    : dtx_enc
 *
 *
 ************************************************************************/

WBAMR_CODE_SECTION fract16 WBAMR_Dtx_enc ( 
  fract16 isf[],
  fract16 exc2[],
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr,  /* i/o : State struct */
  void* tmp_scr_ptr,
  uint8 ** prms
  )

{
  int_native i, j;
  fract16 log_en, gain, level, exp, exp0, tmp;
  fract16 log_en_int_e, log_en_int_m;
  fract32 ener32, level32;
  fract16 CN_dith;
  wbamr_enc_dtxenc *wbamr_enc_dtxenc_scr_ptr = 
                   ( wbamr_enc_dtxenc * )tmp_scr_ptr ;

  /* VOX mode computation of SID parameters */
  log_en = 0;
    
  for (i = 0; i < M11; i++ )
  {
    wbamr_enc_dtxenc_scr_ptr->L_isf[i] = 0;
  }

  /* average energy and isf */
  for (i = 0; i < DTX_HIST_SIZE; i++ )
  {
    /* Division by DTX_HIST_SIZE = 8 has been done in dtx_buffer.
       log_en is in Q10 */
    log_en = Add_fr1x16 ( log_en, enc_obj_ptr->log_en_hist[i] );
  }

  WBAMR_Find_frame_indices ( enc_obj_ptr->isf_hist, 
                             wbamr_enc_dtxenc_scr_ptr->isf_order, 
                             enc_obj_ptr );
  
  WBAMR_Aver_isf_history ( enc_obj_ptr->isf_hist, 
                           wbamr_enc_dtxenc_scr_ptr->isf_order, 
                           wbamr_enc_dtxenc_scr_ptr->L_isf, 
                           wbamr_enc_dtxenc_scr_ptr->isf_tmp );

  for (j = 0; j < M11; j++ )
  {
    isf[j] = 
      Extract_lo_fr1x32 ( Shr_fr1x32 ( wbamr_enc_dtxenc_scr_ptr->L_isf[j],
                                       3 ) );    /* divide by 8 */
  }

  /* quantize logarithmic energy to 6 bits (-6 : 66 dB) 
     which corresponds to -2:22 in log2(E).  */
  /* enc_obj_ptr->log_en_index = (fract16)((log_en + 2.0) * 2.625); */

  /* increase dynamics to 7 bits (Q8) */
  log_en = Shr_fr1x16 ( log_en, 2 );

  /* Add 2 in Q8 = 512 to get log2(E) between 0:24 */
  log_en = Add_fr1x16 ( log_en, 512 );

  /* Multiply by 2.625 to get full 6 bit range. 2.625 = 21504 in Q13. 
     The result is in Q6 */
  log_en = Mult_fr1x16 ( log_en, 21504 );

  /* Quantize Energy */
  enc_obj_ptr->log_en_index = Shr_fr1x16 ( log_en, 6 );

  if (Sub_fr1x16 ( enc_obj_ptr->log_en_index, 63 ) > 0 )
  {
    enc_obj_ptr->log_en_index = 63;
  }
    
  if  ( enc_obj_ptr->log_en_index < 0 )
  {
    enc_obj_ptr->log_en_index = 0;
  }

  /* Quantize ISFs */
  WBAMR_Qisf_ns ( isf, isf, wbamr_enc_dtxenc_scr_ptr->indice );

  WBAMR_Parm_serial ( wbamr_enc_dtxenc_scr_ptr->indice[0], 6, prms );
  WBAMR_Parm_serial ( wbamr_enc_dtxenc_scr_ptr->indice[1], 6, prms );
  WBAMR_Parm_serial ( wbamr_enc_dtxenc_scr_ptr->indice[2], 6, prms );
  WBAMR_Parm_serial ( wbamr_enc_dtxenc_scr_ptr->indice[3], 5, prms );
  WBAMR_Parm_serial ( wbamr_enc_dtxenc_scr_ptr->indice[4], 5, prms );
  WBAMR_Parm_serial (  ( enc_obj_ptr->log_en_index ), 6, prms );

  CN_dith = WBAMR_Dithering_control ( enc_obj_ptr );
  WBAMR_Parm_serial ( CN_dith, 1, prms );

  /* level = (float)(pow(2.0f, 
                         (float)enc_obj_ptr->log_en_index / 2.625 - 2.0)); */
  /* log2(E) in Q9 (log2(E) lies in between -2:22) */
  log_en = Shl_fr1x16 ( enc_obj_ptr->log_en_index, 15 - 6 );

  /* Divide by 2.625; log_en will be between 0:24  */
  log_en = Mult_fr1x16 ( log_en, 12483 );
  /* the result corresponds to log2 ( gain ) in Q10 */

  /* Find integer part  */
  log_en_int_e = Shr_fr1x16 ( log_en, 10 );

  /* Find fractional part */
  log_en_int_m =  ( fract16 ) ( log_en & 0x3ff );   
  log_en_int_m = Shl_fr1x16 ( log_en_int_m, 5 );

  /* Subtract 2 from log_en in Q9, i.e divide the gain by 2 (energy by 4 ) */
  /* Add 16 in order to have the result of WBAMR_Pow2 in Q16 */
  log_en_int_e = Add_fr1x16 ( log_en_int_e, 16 - 1 );

  level32 = WBAMR_Pow2 ( log_en_int_e, log_en_int_m ); /* Q16 */
  exp0 = Norm_fr1x32 ( level32 );
  level32 = Shl_fr1x32 ( level32, exp0 );              /* level in Q31 */
  exp0 = Sub_fr1x16 ( 15, exp0 ); 
  level = Extract_hi_fr1x32 ( level32 );               /* level in Q15 */

  /* generate white noise vector */
  for ( i = 0; i < L_FRAME; i++ )
  {
    exc2[i] = Shr_fr1x16 ( WBAMR_Random ( &enc_obj_ptr->cng_seed ), 4 );      
  }

  /* gain = level / sqrt(ener ) * sqrt(L_FRAME ) */
  /* energy of generated excitation */
  ener32 = WBAMR_Dot_product12 ( exc2, exc2, L_FRAME, &exp );

  WBAMR_Isqrt_n ( &ener32, &exp );

  gain = Extract_hi_fr1x32 ( ener32 );

  gain = Mult_fr1x16 ( level, gain );                  /* gain in Q15 */

  exp = Add_fr1x16 ( exp0, exp );

  /* Multiply by sqrt(L_FRAME )=16, i.e. shift left by 4 */
  exp = Add_fr1x16 ( exp, 4 );

  for ( i = 0; i < L_FRAME; i++ )
  {
    tmp = Mult_fr1x16 ( exc2[i], gain );               /* Q0 * Q15 */
    exc2[i] = Shl_fr1x16 ( tmp, exp );           
  }

  return 0;
}


/************************************************************************
 *
 *
 * Function    : WBAMR_Dtx_buffer Purpose     : handles the DTX buffer
 *
 *
 ************************************************************************/

WBAMR_CODE_SECTION fract16 WBAMR_Dtx_buffer ( 
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr,  /* i/o : State struct         */
  fract16 isf_new[],                          /* i   : isf vector           */
  fract32 enr,                                /* i   : WBAMR_Residual 
                                                       energy (in L_FRAME ) */
  fract16 codec_mode
 )
 
{
  fract16 log_en;

  fract16 log_en_e;
  fract16 log_en_m;

  enc_obj_ptr->hist_ptr = Add_fr1x16 ( enc_obj_ptr->hist_ptr, 1 );   
    
  if ( Sub_fr1x16 ( enc_obj_ptr->hist_ptr, DTX_HIST_SIZE ) == 0 )
  {
    enc_obj_ptr->hist_ptr = 0;
  }
  
  /* copy lsp vector into buffer */
  WBAMR_Copy ( isf_new, 
               &enc_obj_ptr->isf_hist[enc_obj_ptr->hist_ptr * M11],
               M11 );

  /* log_en =  ( float )log10(enr*0.0059322 )/(float )log10(2.0f );  */
  WBAMR_Log2 ( enr, &log_en_e, &log_en_m );

  /* convert exponent and mantissa to fract16 Q7. 
     Q7 is used to simplify averaging in dtx_enc */
  log_en = Shl_fr1x16 ( log_en_e, 7 );             /* Q7 */
  log_en = Add_fr1x16 ( log_en, Shr_fr1x16 ( log_en_m, 15 - 7 ) );

  /* Find energy per sample by multiplying with 0.0059322,
     i.e subtract log2(1/0.0059322 ) = 7.39722 The
     constant 0.0059322 takes into account windowings and 
     analysis length from WBAMR_Autocorrelation
     computations; 7.39722 in Q7 = 947  */
  /* Subtract 3 dB = 0.99658 in log2(E ) = 127 in Q7. */
  /* log_en = Sub_fr1x16( log_en, 947 + en_adjust[codec_mode]  ); */

  /* Find energy per sample (divide by L_FRAME=256 ), 
     i.e subtract log2(256 ) = 8.0  (1024 in Q7 ) */
  /* Subtract 3 dB = 0.99658 in log2(E ) = 127 in Q7. */
  log_en = Sub_fr1x16 ( log_en, Add_fr1x16 ( 1024, en_adjust[codec_mode] ) );

  /* Insert into the buffer */
  enc_obj_ptr->log_en_hist[enc_obj_ptr->hist_ptr] = log_en;
  
  return 0;
}


/************************************************************************
 *
 *
 * Function  : WBAMR_Tx_dtx_handler Purpose : adds extra speech hangover
 *                                            to analyze speech on
 *                                            the decoding side.
 *

 ************************************************************************/

WBAMR_CODE_SECTION void WBAMR_Tx_dtx_handler ( 
  WBAMR_Static_Mem_Enc_Struct *enc_obj_ptr,  /* i/o : State struct           */
  fract16 vad_flag,                          /* i   : vad decision           */
  fract16 *usedMode,                         /* i/o : mode changed or not    */
  uint8 nsync                                /* TX/RX DTX handler sync mode  */
 )

{

  /* this state machine is in synch with the GSMEFR txDtx machine */
  enc_obj_ptr->decAnaElapsedCount = 
               Add_fr1x16 ( enc_obj_ptr->decAnaElapsedCount, 1 );    

  /* NSYNC functionality has been added to this function. From 3GPP 
     TS 26.193 version 5.0.0 Section A.5.1.1: 

     "In order to ensure TX/RX DTX handler synchronisation at handover,
     the uplink TX DTX handler in the MS shall accept messages from the
     TX RSS with control parameter NSYNC."
   
     "If...during this period of NSYNC frames, VAD flag is equal to 1
     at least for one speech frame, TX DTX handler shall send SPEECH
     frames for the rest of the period of the NSYNC frames."
   
     "Note the TX DTX handler shall send SPEECH frames at least for 
     the duration of the hangover period."
  */
  if ( nsync == 0 )
  {
    /* Not in NSYNC, so always clear vad flag latch */
    enc_obj_ptr->nsync_vad_latch = 0;
  }
  else
  {
    if ( ( vad_flag != 0 ) && ( enc_obj_ptr->nsync_vad_latch == 0 ) )
    {
      /*
       * We're in NSYNC, VAD flag is set, and this is the first time that
       * VAD has been set during this NSYNC.  So, we will latch it, and
       * set decAnaElapsedCount to a very high number.  This assures that
       * a hangover period will always take place the next time VAD goes
       * low, no matter when the most recent SID was computed.
       */
      enc_obj_ptr->nsync_vad_latch        =  vad_flag;
      enc_obj_ptr->decAnaElapsedCount     =  32767;
    }
  }
    
  if ( vad_flag != 0 )
  {
    enc_obj_ptr->dtxHangoverCount = DTX_HANG_CONST;  
  }
  else
  {
    /* non-speech */
    if ( enc_obj_ptr->dtxHangoverCount == 0 )
    {
      /* out of decoder analysis hangover  */
      /*
       * Normal decoder analysis hangover complete.  Continue hangover
       * period (outputting speech ) by staying in speech mode *usedMode
       * iff we're in NSYNC and a vad_flag has been latched.  Otherwise,
       * generate a new SID.
       */
      if ( enc_obj_ptr->nsync_vad_latch == 0 )
      {
        enc_obj_ptr->decAnaElapsedCount = 0;    
        *usedMode = MRDTX;
      }                
    } 
    else
    {
      /* in possible analysis hangover */
      enc_obj_ptr->dtxHangoverCount = 
                   Sub_fr1x16 ( enc_obj_ptr->dtxHangoverCount, 1 );

      /* decAnaElapsedCount + dtxHangoverCount < DTX_ELAPSED_FRAMES_THRESH */
      if  ( Sub_fr1x16 ( Add_fr1x16 ( enc_obj_ptr->decAnaElapsedCount, 
                                      enc_obj_ptr->dtxHangoverCount ),
                         DTX_ELAPSED_FRAMES_THRESH ) < 0 )
      {
        *usedMode = MRDTX;         
        /* if short time since decoder update, do not Add_fr1x16 extra HO */
      }
      /* else override VAD and stay in speech mode *usedMode and 
         Add_fr1x16 extra hangover */
    }
  }

  return;
}


/**************************************************************************
 *
 *
 * Function    : WBAMR_Dtx_dec_init
 *
 *
 **************************************************************************/
 
WBAMR_CODE_SECTION fract16 WBAMR_Dtx_dec_init ( 
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr, 
  const fract16 isf_init[]
 )

{

  WBAMR_Dtx_dec_reset ( dec_obj_ptr, isf_init );

  return 0;
}


/**************************************************************************
 *
 *
 * Function    : WBAMR_Dtx_dec_reset
 *
 *
 **************************************************************************/
 
WBAMR_CODE_SECTION fract16 WBAMR_Dtx_dec_reset ( 
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr, 
  const fract16 isf_init[]
 )

{
  int_native i;

  dec_obj_ptr->since_last_sid = 0;                
  dec_obj_ptr->true_sid_period_inv =  ( 1 << 13 );     /* 0.25 in Q15 */

  dec_obj_ptr->log_en = 3500;                     
  dec_obj_ptr->old_log_en = 3500;                 

  /* low level noise for better performance in  DTX handover cases */
  dec_obj_ptr->cng_seed = RANDOM_INITSEED;        

  dec_obj_ptr->hist_ptr = 0;                      

  /* Init isf_hist[] and decoder log frame energy */
  WBAMR_Copy ( (fract16 *)isf_init, dec_obj_ptr->isf, M11 );
  WBAMR_Copy ( (fract16 *)isf_init, dec_obj_ptr->isf_old, M11 );

  for ( i = 0; i < DTX_HIST_SIZE; i++ )
  {
    WBAMR_Copy ( (fract16 *)isf_init, &dec_obj_ptr->isf_hist[i * M11], M11 );
    dec_obj_ptr->log_en_hist[i] = dec_obj_ptr->log_en;   
  }

  dec_obj_ptr->dtxHangoverCount = DTX_HANG_CONST; 
  dec_obj_ptr->decAnaElapsedCount = 32767;        

  dec_obj_ptr->sid_frame = 0;                     
  dec_obj_ptr->valid_data = 0;                    
  dec_obj_ptr->dtxHangoverAdded = 0;              

  dec_obj_ptr->dtxGlobalState = SPEECH;           
  dec_obj_ptr->data_updated = 0;                  

  dec_obj_ptr->dither_seed = RANDOM_INITSEED;     
  dec_obj_ptr->CN_dith = 0;

  return 0;
}


/*
     Table of new SPD synthesis states
     ---------------------------------------------------------------
                           |     previous SPD_synthesis_state
     Incoming              |
     frame_type            | SPEECH       | DTX           | DTX_MUTE
     ---------------------------------------------------------------
     RX_SPEECH_GOOD ,      |              |               |
     RX_SPEECH_PR_DEGRADED | SPEECH       | SPEECH        | SPEECH
     ----------------------------------------------------------------
     RX_SPEECH_BAD,        | SPEECH       | DTX           | DTX_MUTE
     ----------------------------------------------------------------
     RX_SID_FIRST,         | DTX          | DTX/(DTX_MUTE)| DTX_MUTE
     ----------------------------------------------------------------
     RX_SID_UPDATE,        | DTX          | DTX           | DTX
     ----------------------------------------------------------------
     RX_SID_BAD,           | DTX          | DTX/(DTX_MUTE)| DTX_MUTE
     ----------------------------------------------------------------
     RX_NO_DATA,           | SPEECH       | DTX/(DTX_MUTE)| DTX_MUTE
     RX_SPARE              |(class2 garb.)|               |
     ----------------------------------------------------------------
*/


/**************************************************************************
 *
 *
 * Function    : dtx_dec
 *
 *
 **************************************************************************/
 
WBAMR_CODE_SECTION fract16 WBAMR_Dtx_dec ( 
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr,  /* i/o : State struct   */
  fract16 * exc2,                            /* o   : CN excitation  */
  fract16 new_state,                         /* i   : New DTX state  */
  fract16 isf[],                             /* o   : CN ISF vector  */
  uint8 ** prms
 )
{
  int_native i, j;
  fract16 log_en_index;
  fract16 ind[7];
  fract16 int_fac;
  fract16 gain;

  fract32 L_isf[M11], L_log_en_int, level32, ener32;
  fract16 ptr;
  fract16 tmp_int_length;
  fract16 tmp, exp, exp0, log_en_int_e, log_en_int_m, level;

  /* This function is called if synthesis state is not SPEECH the globally 
     passed  inputs to this function are enc_obj_ptr->sid_frame,
     enc_obj_ptr->valid_data, enc_obj_ptr->dtxHangoverAdded, new_state
     (SPEECH, DTX, DTX_MUTE ) */
    
  if ( ( dec_obj_ptr->dtxHangoverAdded != 0 ) &&
         ( dec_obj_ptr->sid_frame != 0 ) )
  {
    /* sid_first after dtx hangover period */
    /* or sid_upd after dtxhangover        */

    /* consider  twice the last frame */
    ptr = Add_fr1x16 ( dec_obj_ptr->hist_ptr, 1 );
        
    if ( Sub_fr1x16 ( ptr, DTX_HIST_SIZE ) == 0 )
      ptr = 0;                       

    WBAMR_Copy ( &dec_obj_ptr->isf_hist[dec_obj_ptr->hist_ptr * M11],
                 &dec_obj_ptr->isf_hist[ptr * M11],
                 M11 );

    dec_obj_ptr->log_en_hist[ptr] = 
                 dec_obj_ptr->log_en_hist[dec_obj_ptr->hist_ptr];

    /* compute mean log energy and isf from decoded signal (SID_FIRST ) */
    dec_obj_ptr->log_en = 0;                    
    for ( i = 0; i < M11; i++ )
    {
      L_isf[i] = 0;                  
    }

    /* average energy and isf */
    for ( i = 0; i < DTX_HIST_SIZE; i++ )
    {
      /* Division by DTX_HIST_SIZE = 8 has been done in 
         dtx_buffer log_en is in Q10 */
      dec_obj_ptr->log_en = Add_fr1x16 ( dec_obj_ptr->log_en, 
                                         dec_obj_ptr->log_en_hist[i] );   

      for ( j = 0; j < M11; j++ )
      {
        L_isf[j] = Add_fr1x32 ( L_isf[j],
                   WBAMR_L_deposit_l ( dec_obj_ptr->isf_hist[i * M11 + j] ) );
      }
    }

    /* dec_obj_ptr->log_en in Q9 */
    dec_obj_ptr->log_en = Shr_fr1x16 ( dec_obj_ptr->log_en, 1 );   

    /* Add 2 in Q9, in order to have only positive values for WBAMR_Pow2 */
    /* this value is subtracted back after WBAMR_Pow2 function */
    dec_obj_ptr->log_en = Add_fr1x16 ( dec_obj_ptr->log_en, 1024 );
        
    if ( dec_obj_ptr->log_en < 0 )
      dec_obj_ptr->log_en = 0;                

    for ( j = 0; j < M11; j++ )
    {
      /* divide by 8 */
      dec_obj_ptr->isf[j] = Extract_lo_fr1x32 ( Shr_fr1x32 ( L_isf[j], 3 ) );
    }
  }
    
  if ( dec_obj_ptr->sid_frame != 0 )
  {
    /* Set old SID parameters, always shift */
    /* even if there is no new valid_data   */

    WBAMR_Copy ( dec_obj_ptr->isf, dec_obj_ptr->isf_old, M11 );
    dec_obj_ptr->old_log_en = dec_obj_ptr->log_en;       
        
    /* new data available (no CRC) */
    if ( dec_obj_ptr->valid_data != 0 )           
    {
      /* enc_obj_ptr->true_sid_period_inv = 
                      1.0f/enc_obj_ptr->since_last_sid; */
      /* Compute interpolation factor, since the division only works 
       * for values of since_last_sid < 32 we have to limit the
       * interpolation to 32 frames                     */
      tmp_int_length = dec_obj_ptr->since_last_sid;        

      if ( Sub_fr1x16 ( tmp_int_length, 32 ) > 0 )
      {
        tmp_int_length = 32;       
      }
            
      if ( Sub_fr1x16 ( tmp_int_length, 2 ) >= 0 )
      {
        dec_obj_ptr->true_sid_period_inv = 
                  Divs_fr1x16 ( 1 << 10, Shl_fr1x16 ( tmp_int_length, 10 ) );
      }
      else
      {
      	/* 0.5 it Q15 */
        dec_obj_ptr->true_sid_period_inv = 1 << 14;      
      }

      ind[0] = WBAMR_Serial_parm ( 6, prms ); 
      ind[1] = WBAMR_Serial_parm ( 6, prms ); 
      ind[2] = WBAMR_Serial_parm ( 6, prms ); 
      ind[3] = WBAMR_Serial_parm ( 5, prms ); 
      ind[4] = WBAMR_Serial_parm ( 5, prms ); 

      WBAMR_Disf_ns ( ind, dec_obj_ptr->isf );

      log_en_index = WBAMR_Serial_parm ( 6, prms );

      /* read background noise stationarity information */
      dec_obj_ptr->CN_dith = WBAMR_Serial_parm ( 1, prms ); 

      /* enc_obj_ptr->log_en =  ( float)log_en_index / 2.625 - 2.0;  */
      /* log2(E) in Q9  ( log2(E) lies in between -2:22) */
      dec_obj_ptr->log_en = Shl_fr1x16 ( log_en_index, 15 - 6 );     

      /* Divide by 2.625  */
      dec_obj_ptr->log_en = Mult_fr1x16 ( dec_obj_ptr->log_en, 12483 );       
      /* Subtract 2 in Q9 is done later, after WBAMR_Pow2 function  */

      /* no interpolation at startup after coder reset        */
      /* or when SID_UPD has been received right after SPEECH */
            
      if ( ( dec_obj_ptr->data_updated == 0 ) ||
             ( Sub_fr1x16 ( dec_obj_ptr->dtxGlobalState, SPEECH ) == 0 ) )
      {
        WBAMR_Copy ( dec_obj_ptr->isf, dec_obj_ptr->isf_old, M11 );
        dec_obj_ptr->old_log_en = dec_obj_ptr->log_en;    
      }
    }                                  /* endif valid_data */
  }                                    /* endif sid_frame */
    
  if ( ( dec_obj_ptr->sid_frame != 0 ) &&  ( dec_obj_ptr->valid_data != 0 ) )
  {
    dec_obj_ptr->since_last_sid = 0;            
  }
  
  /* Interpolate SID info */
  /* Q10 */
  int_fac = Shl_fr1x16 ( dec_obj_ptr->since_last_sid, 10 );
  /* Q10 * Q15 -> Q10 */              
  int_fac = Mult_fr1x16 ( int_fac, dec_obj_ptr->true_sid_period_inv );

  /* Maximize to 1.0 in Q10 */
  if ( Sub_fr1x16 ( int_fac, 1024 ) > 0 )
  {
    int_fac = 1024;                    
  }
  
  /* Q10 -> Q14 */
  int_fac = Shl_fr1x16 ( int_fac, 4 );

  /* Q14 * Q9 -> Q24 */
  L_log_en_int = Mult_fr1x32 ( int_fac, dec_obj_ptr->log_en ); 

  for ( i = 0; i < M11; i++ )
  {
    /* Q14 * Q15 -> Q14 */  	
    isf[i] = Mult_fr1x16 ( int_fac, dec_obj_ptr->isf[i] );   
  }

  /* 1-k in Q14 */
  int_fac = Sub_fr1x16 ( 16384, int_fac );             

  /* ( Q14 * Q9 -> Q24  ) + Q24 -> Q24 */
  L_log_en_int = Mac_fr16_32(L_log_en_int, int_fac, dec_obj_ptr->old_log_en );

  for ( i = 0; i < M11; i++ )
  {
    /* Q14 + (Q14 * Q15 -> Q14) -> Q14 */
    isf[i] = Add_fr1x16 ( isf[i], 
                          Mult_fr1x16 ( int_fac, dec_obj_ptr->isf_old[i] ) );
    /* Q14 -> Q15 */
    isf[i] = Shl_fr1x16 ( isf[i], 1 );               
  }

  /* If background noise is non-stationary, insert comfort noise dithering */
  if ( dec_obj_ptr->CN_dith != 0 )
  {
    WBAMR_CN_dithering ( isf, &L_log_en_int, &dec_obj_ptr->dither_seed );
  }
  
  /* L_log_en_int corresponds to log2(E)+2 in Q24, i.e log2(gain)+1 in Q25 */
  /* Q25 -> Q16 */
  L_log_en_int = Shr_fr1x32(L_log_en_int, 9 );

  /* Find integer part  */
  log_en_int_e = Extract_hi_fr1x32 ( L_log_en_int );

  /* Find fractional part */
  log_en_int_m = 
    Extract_lo_fr1x32 ( Shr_fr1x32 ( 
                        Sub_fr1x32 ( L_log_en_int, 
                                     WBAMR_L_deposit_h ( log_en_int_e ) ),
                        1 ) );

  /* Subtract 2 from L_log_en_int in Q9, 
     i.e divide the gain by 2 (energy by 4) */
  /* Add 16 in order to have the result of WBAMR_Pow2 in Q16 */
  log_en_int_e = Add_fr1x16 ( log_en_int_e, 16 - 1 );

  /* level =  ( float )( pow( 2.0f, log_en ) );  */
  /* Q16 */
  level32 = WBAMR_Pow2 ( log_en_int_e, log_en_int_m ); 
  exp0 = Norm_fr1x32 ( level32 );
  level32 = Shl_fr1x32 ( level32, exp0 );        /* level in Q31 */
  exp0 = Sub_fr1x16 ( 15, exp0 );
  level = Extract_hi_fr1x32 ( level32 );         /* level in Q15 */

  /* generate white noise vector */
  for ( i = 0; i < L_FRAME; i++ )
  {
    exc2[i] = Shr_fr1x16 ( WBAMR_Random ( &dec_obj_ptr->cng_seed ), 4 );
  }

  /* gain = level / sqrt(ener) * sqrt(L_FRAME) */
  /* energy of generated excitation */
  ener32 = WBAMR_Dot_product12 ( exc2, exc2, L_FRAME, &exp );

  WBAMR_Isqrt_n ( &ener32, &exp );

  gain = Extract_hi_fr1x32 ( ener32 );
  gain = Mult_fr1x16 ( level, gain );            /* gain in Q15 */

  exp = Add_fr1x16 ( exp0, exp );

  /* Multiply by sqrt ( L_FRAME)=16, i.e. shift left by 4 */
  exp = Add_fr1x16 ( exp, 4 );

  for ( i = 0; i < L_FRAME; i++ )
  {
    tmp = Mult_fr1x16 ( exc2[i], gain );         /* Q0 * Q15 */
    exc2[i] = Shl_fr1x16 ( tmp, exp );           
  }
    
  if ( Sub_fr1x16 ( new_state, DTX_MUTE ) == 0 )
  {
    /* mute comfort noise as it has been quite a long time 
       since last SID update  was performed */
    tmp_int_length = dec_obj_ptr->since_last_sid;    
        
    if ( Sub_fr1x16 ( tmp_int_length, 32 ) > 0 )
    {
      tmp_int_length = 32;           
    }
        
    dec_obj_ptr->true_sid_period_inv = 
                 Divs_fr1x16 ( 1 << 10, Shl_fr1x16 ( tmp_int_length, 10 ) );

    dec_obj_ptr->since_last_sid = 0;            
    dec_obj_ptr->old_log_en = dec_obj_ptr->log_en;       
    /* subtract 1/8 in Q9 (energy), i.e -3/8 dB */
    dec_obj_ptr->log_en = Sub_fr1x16 ( dec_obj_ptr->log_en, 64 );  
  }
    
  /* reset interpolation length timer if data has been updated. */
  if ( ( dec_obj_ptr->sid_frame != 0 ) &&
       ( ( dec_obj_ptr->valid_data != 0 ) ||
       ( ( dec_obj_ptr->valid_data == 0 ) &&  
         ( dec_obj_ptr->dtxHangoverAdded ) != 0 ) ) )
  {
    dec_obj_ptr->since_last_sid = 0;            
    dec_obj_ptr->data_updated = 1;              
  }
  
  return 0;
}


WBAMR_CODE_SECTION void WBAMR_Dtx_dec_activity_update ( 
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr,
  fract16 isf[],
  fract16 exc[]
 )

{
  int_native i;

  fract32 L_frame_en;
  fract16 log_en_e, log_en_m, log_en;

  dec_obj_ptr->hist_ptr = Add_fr1x16 ( dec_obj_ptr->hist_ptr, 1 );   
  
  if ( Sub_fr1x16 ( dec_obj_ptr->hist_ptr, DTX_HIST_SIZE ) == 0 )
  {
    dec_obj_ptr->hist_ptr = 0;                  
  }
    
  WBAMR_Copy ( isf, 
               &dec_obj_ptr->isf_hist[dec_obj_ptr->hist_ptr * M11],
               M11 );

  /* compute log energy based on excitation frame energy in Q0 */
  L_frame_en = 0;                        
  for ( i = 0; i < L_FRAME; i++ )
  {
    L_frame_en = Mac_fr16_32 ( L_frame_en, exc[i], exc[i] );
  }
  
  L_frame_en = Shr_fr1x32 ( L_frame_en, 1 );

  /* log_en = (float)log10(L_frame_en/(float)L_FRAME)/(float)log10(2.0f); */
  WBAMR_Log2 ( L_frame_en, &log_en_e, &log_en_m );

  /* convert exponent and mantissa to fract16 Q7. 
     Q7 is used to simplify averaging in dtx_enc */
  /* Q7 */     
  log_en = Shl_fr1x16 ( log_en_e, 7 );             
  log_en = Add_fr1x16 ( log_en, Shr_fr1x16 ( log_en_m, 15 - 7 ) );

  /* Divide by L_FRAME = 256, i.e subtract 8 in Q7 = 1024 */
  log_en = Sub_fr1x16 ( log_en, 1024 );

  /* insert into log energy buffer */
  dec_obj_ptr->log_en_hist[dec_obj_ptr->hist_ptr] = log_en;

  return;
}


/*
     Table of new SPD synthesis states
     ---------------------------------------------------------------
                           |     previous SPD_synthesis_state
     Incoming              |
     frame_type            | SPEECH       | DTX           | DTX_MUTE
     ---------------------------------------------------------------
     RX_SPEECH_GOOD ,      |              |               |
     RX_SPEECH_PR_DEGRADED | SPEECH       | SPEECH        | SPEECH
     ----------------------------------------------------------------
     RX_SPEECH_BAD,        | SPEECH       | DTX           | DTX_MUTE
     ----------------------------------------------------------------
     RX_SID_FIRST,         | DTX          | DTX/(DTX_MUTE)| DTX_MUTE
     ----------------------------------------------------------------
     RX_SID_UPDATE,        | DTX          | DTX           | DTX
     ----------------------------------------------------------------
     RX_SID_BAD,           | DTX          | DTX/(DTX_MUTE)| DTX_MUTE
     ----------------------------------------------------------------
     RX_NO_DATA,           | SPEECH       | DTX/(DTX_MUTE)| DTX_MUTE
     RX_SPARE              |(class2 garb.)|               |
     ----------------------------------------------------------------
*/


WBAMR_CODE_SECTION fract16 WBAMR_Rx_dtx_handler ( 
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr,       /* i/o : State struct     */
  fract16 frame_type                              /* i   : Frame type       */
 )

{
  fract16 newState;
  fract16 encState;

  /* DTX if SID frame or previously in DTX{_MUTE} and (NO_RX OR BAD_SPEECH) */
  if ( ( Sub_fr1x16 ( frame_type, RX_SID_FIRST ) == 0 ) ||
       ( Sub_fr1x16 ( frame_type, RX_SID_UPDATE ) == 0 ) ||
       ( Sub_fr1x16 ( frame_type, RX_SID_BAD ) == 0 ) ||
       ( ( ( Sub_fr1x16 ( dec_obj_ptr->dtxGlobalState, DTX ) == 0 ) ||
           ( Sub_fr1x16 ( dec_obj_ptr->dtxGlobalState, DTX_MUTE ) == 0 ) ) &&
           ( ( Sub_fr1x16 ( frame_type, RX_NO_DATA ) == 0 ) ||
             ( Sub_fr1x16 ( frame_type, RX_SPEECH_BAD ) == 0 ) ||
             ( Sub_fr1x16 ( frame_type, RX_SPEECH_LOST ) == 0 ) ) ) )
  {
    newState = DTX;                    

    /* stay in mute for these input types */
    if ( ( Sub_fr1x16 ( dec_obj_ptr->dtxGlobalState, DTX_MUTE ) == 0 ) &&
           ( ( Sub_fr1x16 ( frame_type, RX_SID_BAD ) == 0 ) ||
             ( Sub_fr1x16 ( frame_type, RX_SID_FIRST ) == 0 ) ||
             ( Sub_fr1x16 ( frame_type, RX_SPEECH_LOST ) == 0 ) ||
             ( Sub_fr1x16 ( frame_type, RX_NO_DATA ) == 0 ) ) )
    {
      newState = DTX_MUTE;           
    }
    
    /* evaluate if noise parameters are too old                     */
    /* since_last_sid is reset when CN parameters have been updated */
    dec_obj_ptr->since_last_sid = 
                 Add_fr1x16 ( dec_obj_ptr->since_last_sid, 1 );        

    /* no update of sid parameters in DTX for a long while */
    if ( Sub_fr1x16 ( dec_obj_ptr->since_last_sid, 
                      DTX_MAX_EMPTY_THRESH ) > 0 )
    {
      newState = DTX_MUTE;           
    }
  }
  else
  {
    newState = SPEECH;                 
    dec_obj_ptr->since_last_sid = 0;            
  }

  /* reset the decAnaElapsed Counter when receiving CNI data the first time,
     to robustify counter missmatch after handover this might delay
     the bwd CNI analysis in the new decoder slightly. */
  if ( ( dec_obj_ptr->data_updated == 0 ) &&
       ( Sub_fr1x16 ( frame_type, RX_SID_UPDATE ) == 0 ) )
  {
    dec_obj_ptr->decAnaElapsedCount = 0;        
  }
    
  /* update the SPE-SPD DTX hangover synchronization */
  /* to know when SPE has added dtx hangover         */
  dec_obj_ptr->decAnaElapsedCount = 
               Add_fr1x16 ( dec_obj_ptr->decAnaElapsedCount, 1 );    
  dec_obj_ptr->dtxHangoverAdded = 0;              
    
  if ( ( Sub_fr1x16 ( frame_type, RX_SID_FIRST ) == 0 ) ||
       ( Sub_fr1x16 ( frame_type, RX_SID_UPDATE ) == 0 ) ||
       ( Sub_fr1x16 ( frame_type, RX_SID_BAD ) == 0 ) ||
       ( Sub_fr1x16 ( frame_type, RX_NO_DATA ) == 0 ) )
  {
    encState = DTX;                    
  }
  else
  {
    encState = SPEECH;                 
  }
    
  if ( Sub_fr1x16 ( encState, SPEECH ) == 0 )
  {
    dec_obj_ptr->dtxHangoverCount = DTX_HANG_CONST;  
  }
  else
  {
    if ( Sub_fr1x16 ( dec_obj_ptr->decAnaElapsedCount,
                      DTX_ELAPSED_FRAMES_THRESH ) > 0 )
    {
      dec_obj_ptr->dtxHangoverAdded = 1;      
      dec_obj_ptr->decAnaElapsedCount = 0;    
      dec_obj_ptr->dtxHangoverCount = 0;      
    }
    else if ( dec_obj_ptr->dtxHangoverCount == 0 )
    {
      dec_obj_ptr->decAnaElapsedCount = 0;    
    }
    else
    {
      dec_obj_ptr->dtxHangoverCount = 
                   Sub_fr1x16 ( dec_obj_ptr->dtxHangoverCount, 1 );
    }
  }
    
  if ( Sub_fr1x16 ( newState, SPEECH ) != 0 )
  {
    /* DTX or DTX_MUTE CN data is not in a first SID, 
       first SIDs are marked as SID_BAD but will do
       backwards analysis if a hangover period has been 
       added according to the state machine above */

    dec_obj_ptr->sid_frame = 0;                 
    dec_obj_ptr->valid_data = 0;                
        
    if ( Sub_fr1x16 ( frame_type, RX_SID_FIRST ) == 0 )
    {
      dec_obj_ptr->sid_frame = 1;             
    }
    else if ( Sub_fr1x16 ( frame_type, RX_SID_UPDATE ) == 0 )
    {
      dec_obj_ptr->sid_frame = 1;             
      dec_obj_ptr->valid_data = 1;            
    }
    else if ( Sub_fr1x16 ( frame_type, RX_SID_BAD ) == 0 )
    {
      dec_obj_ptr->sid_frame = 1;             
      dec_obj_ptr->dtxHangoverAdded = 0;         /* use old data */
    }
  }
    
  return newState;
  /* newState is used by both SPEECH AND DTX synthesis routines */
}


WBAMR_CODE_SECTION void WBAMR_Aver_isf_history ( 
  fract16 isf_old[],
  fract16 indices[],
  fract32 isf_aver[],
  fract16 isf_tmp[]
 )
 
{
  int_native i, j, k;
    
  fract32 L_tmp;

  /* Memorize in isf_tmp[][] the ISF vectors to be replaced by */
  /* the median ISF vector prior to the averaging               */
  for ( k = 0; k < 2; k++ )
  {
    if ( Add_fr1x16 ( indices[k], 1 ) != 0 )
    {
      for ( i = 0; i < M11; i++ )
      {
        isf_tmp[k * M11 + i] = isf_old[indices[k] * M11 + i];       
        isf_old[indices[k] * M11 + i] = isf_old[indices[2] * M11 + i];      
      }
    }
  }

  /* Perform the ISF averaging */
  for ( j = 0; j < M11; j++ )
  {
    L_tmp = 0;                         

    for ( i = 0; i < DTX_HIST_SIZE; i++ )
    {
      L_tmp = Add_fr1x32 ( L_tmp, 
                           WBAMR_L_deposit_l ( isf_old[i * M11 + j] ) );
    }
    isf_aver[j] = L_tmp;               
  }

  /* Retrieve from isf_tmp[][] the ISF vectors saved prior to averaging */
  for ( k = 0; k < 2; k++ )
  {
    if ( Add_fr1x16 ( indices[k], 1 ) != 0 )
    {
      for ( i = 0; i < M11; i++ )
      {
        isf_old[indices[k] * M11 + i] = isf_tmp[k * M11 + i];       
      }
    }
  }

  return;
}


WBAMR_CODE_SECTION void WBAMR_Find_frame_indices ( 
  fract16 isf_old_tx[],
  fract16 indices[],
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr
 )

{
  int_native i, j;
  fract32 L_tmp, summin, summax, summax2nd;
  fract16 tmp;
  fract16 ptr;

  /* Remove the effect of the oldest frame from the column */
  /* sum sumD[0..DTX_HIST_SIZE-1]. sumD[DTX_HIST_SIZE] is  */
  /* not updated since it will be removed later.           */

  tmp = DTX_HIST_SIZE_MIN_ONE;           
  j = -1;                                
  for ( i = 0; i < DTX_HIST_SIZE_MIN_ONE; i++ )
  {
    j = Add_fr1x16 ( (fract16)j, tmp );
    enc_obj_ptr->sumD[i] = Sub_fr1x32 ( enc_obj_ptr->sumD[i], 
                                        enc_obj_ptr->D[j] );     
    tmp = Sub_fr1x16 ( tmp, 1 );
  }

  /* Shift the column sum sumD. The element sumD[DTX_HIST_SIZE-1] */
  /* corresponding to the oldest frame is removed. The sum of     */
  /* the distances between the latest isf and other isfs,         */
  /* i.e. the element sumD[0], will be computed during this call. */
  /* Hence this element is initialized to zero.                   */

  for ( i = DTX_HIST_SIZE_MIN_ONE; i > 0; i-- )
  {
    enc_obj_ptr->sumD[i] = enc_obj_ptr->sumD[i - 1];     
  }
  
  enc_obj_ptr->sumD[0] = 0;                       

  /* Remove the oldest frame from the distance matrix.           */
  /* Note that the distance matrix is replaced by a one-         */
  /* dimensional array to save static memory.                    */

  tmp = 0;                               
  for ( i = 27; i >= 12; i =  ( fract16 )  ( i - tmp ) )
  {
    tmp = Add_fr1x16 ( tmp, 1 );
    for ( j = tmp; j > 0; j-- )
    {
      enc_obj_ptr->D[i - j + 1] = enc_obj_ptr->D[i - j - tmp];      
    }
  }

  /* Compute the first column of the distance matrix D            */
  /* (squared Euclidean distances from isf1[] to isf_old_tx[][]). */

  ptr = enc_obj_ptr->hist_ptr;                    
  for ( i = 1; i < DTX_HIST_SIZE; i++ )
  {
    /* Compute the distance between the latest isf and the other isfs. */
    ptr = Sub_fr1x16 ( ptr, 1 );
        
    if ( ptr < 0 )
    {
      ptr = DTX_HIST_SIZE_MIN_ONE;   
    }
    L_tmp = 0;                         
    for ( j = 0; j < M11; j++ )
    {
      tmp = Sub_fr1x16 ( isf_old_tx[enc_obj_ptr->hist_ptr * M11 + j],
                         isf_old_tx[ptr * M11 + j] );
      L_tmp = Mac_fr16_32 ( L_tmp, tmp, tmp );
    }
    enc_obj_ptr->D[i - 1] = L_tmp;              

    /* Update also the column sums. */
    enc_obj_ptr->sumD[0] = Add_fr1x32 ( enc_obj_ptr->sumD[0], 
                                        enc_obj_ptr->D[i - 1] ); 
    enc_obj_ptr->sumD[i] = Add_fr1x32 ( enc_obj_ptr->sumD[i], 
                                        enc_obj_ptr->D[i - 1] ); 
  }

  /* Find the minimum and maximum distances */
  summax = enc_obj_ptr->sumD[0];                  
  summin = enc_obj_ptr->sumD[0];                  
  indices[0] = 0;                        
  indices[2] = 0;                        
  for ( i = 1; i < DTX_HIST_SIZE; i++ )
  {
    if ( Sub_fr1x32 ( enc_obj_ptr->sumD[i], summax ) > 0 )
    {
      indices[0] = ( fract16 ) i;                
      summax = enc_obj_ptr->sumD[i];          
    }
        
    if ( Sub_fr1x32 ( enc_obj_ptr->sumD[i], summin ) < 0 )
    {
      indices[2] =  ( fract16 )i;                
      summin = enc_obj_ptr->sumD[i];          
    }
  }

  /* Find the second largest distance */
  summax2nd = -2147483647L;              
  indices[1] = -1;                       
  for ( i = 0; i < DTX_HIST_SIZE; i++ )
  {
    if ( ( Sub_fr1x32 ( enc_obj_ptr->sumD[i], summax2nd ) > 0 ) &&
         ( Sub_fr1x16 ( (fract16)i, indices[0] ) != 0 ) )
    {
      indices[1] = (fract16)i;                
      summax2nd = enc_obj_ptr->sumD[i];       
    }
  }

  for ( i = 0; i < 3; i++ )
  {
    indices[i] = Sub_fr1x16 ( enc_obj_ptr->hist_ptr, indices[i] );     
        
    if ( indices[i] < 0 )
    {
      indices[i] = Add_fr1x16 ( indices[i], DTX_HIST_SIZE );        
    }
  }

  /* If maximum distance/MED_THRESH is smaller than minimum distance */
  /* then the median ISF vector replacement is not performed         */
  tmp = Norm_fr1x32 ( summax );
  summax = Shl_fr1x32 ( summax, tmp );
  summin = Shl_fr1x32 ( summin, tmp );
  L_tmp = Mult_fr1x32 ( Round_fr1x32 ( summax ), INV_MED_THRESH );
    
  if ( Sub_fr1x32 ( L_tmp, summin ) <= 0 )
  {
    indices[0] = -1;                   
  }
  
  /* If second largest distance/MED_THRESH is smaller than         */
  /* minimum distance then the median ISF vector replacement is    */
  /* not performed                                                 */
  summax2nd = Shl_fr1x32 ( summax2nd, tmp );
  L_tmp = Mult_fr1x32 ( Round_fr1x32 ( summax2nd ), INV_MED_THRESH );
    
  if ( Sub_fr1x32 ( L_tmp, summin ) <= 0 )
  {
    indices[1] = -1;                   
  }
    
  return;
}


WBAMR_CODE_SECTION fract16 WBAMR_Dithering_control ( 
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr
 )

{
  int_native i;
  fract16 tmp, mean, CN_dith, gain_diff;
  fract32 ISF_diff;

  /* determine how stationary the spectrum of background noise is */
  ISF_diff = 0;
  for  ( i = 0; i < 8; i++ )
  {
    ISF_diff = Add_fr1x32 ( ISF_diff, enc_obj_ptr->sumD[i] );
  }
  
  if ( Shr_fr1x32 ( ISF_diff, 26 ) > 0 )
  {
    CN_dith = 1;
  }
  else
  {
    CN_dith = 0;
  }

  /* determine how stationary the energy of background noise is */
  mean = 0;
  for ( i = 0; i < DTX_HIST_SIZE; i++ )
  {
    mean = Add_fr1x16 ( mean, enc_obj_ptr->log_en_hist[i] );
  }
  
  mean = Shr_fr1x16 ( mean, 3 );
  gain_diff = 0;
  
  for ( i = 0; i < DTX_HIST_SIZE; i++ )
  {
    tmp = Abs_fr1x16 ( Sub_fr1x16 ( enc_obj_ptr->log_en_hist[i], mean ) );
    gain_diff = Add_fr1x16 ( gain_diff, tmp );
  }
  
  if ( Sub_fr1x16 ( gain_diff, GAIN_THR ) > 0 )
  {
    CN_dith = 1;
  }
  
  return CN_dith;
}


WBAMR_CODE_SECTION void WBAMR_CN_dithering ( 
  fract16 isf[M11],
  fract32 * L_log_en_int,
  fract16 * dither_seed
 )

{   
  int_native i; 	
  fract16 temp, temp1, dither_fac, rand_dith;
  fract16 rand_dith2;

  /* Insert comfort noise dithering for energy parameter */
  rand_dith = Shr_fr1x16 ( WBAMR_Random ( dither_seed ), 1 );
  rand_dith2 = Shr_fr1x16 ( WBAMR_Random ( dither_seed ), 1 );
  rand_dith = Add_fr1x16 ( rand_dith, rand_dith2 );
  *L_log_en_int = Add_fr1x32 ( *L_log_en_int, 
                               Mult_fr1x32 ( rand_dith, GAIN_FACTOR ) );
  if ( *L_log_en_int < 0 )
  {
    *L_log_en_int = 0;
  }
    
  /* Insert comfort noise dithering for spectral parameters (ISF-vector) */
  dither_fac = ISF_FACTOR_LOW;

  rand_dith = Shr_fr1x16 ( WBAMR_Random ( dither_seed ), 1 );
  rand_dith2 = Shr_fr1x16 ( WBAMR_Random ( dither_seed ), 1 );
  rand_dith = Add_fr1x16 ( rand_dith, rand_dith2 );
  temp = Add_fr1x16 ( isf[0], Multr_fr1x16 ( rand_dith, dither_fac ) );

  /* Make sure that isf[0] will not get negative values */
  if ( Sub_fr1x16 ( temp, ISF_GAP ) < 0 )
  {
    isf[0] = ISF_GAP;
  }
  else
  {
    isf[0] = temp;
  }

  for ( i = 1; i < M11 - 1; i++ )
  {
    dither_fac = Add_fr1x16 ( dither_fac, ISF_FACTOR_STEP );

    rand_dith = Shr_fr1x16 ( WBAMR_Random ( dither_seed ), 1 );
    rand_dith2 = Shr_fr1x16 ( WBAMR_Random ( dither_seed ), 1 );
    rand_dith = Add_fr1x16 ( rand_dith, rand_dith2 );
    temp = Add_fr1x16 ( isf[i], Multr_fr1x16 ( rand_dith, dither_fac ) );
    temp1 = Sub_fr1x16 ( temp, isf[i - 1] );

    /* Make sure that isf spacing remains at least ISF_DITH_GAP Hz */
    if ( Sub_fr1x16 ( temp1, ISF_DITH_GAP ) < 0 )
    {
      isf[i] = Add_fr1x16 ( isf[i - 1], ISF_DITH_GAP );
    }
    else
    {
      isf[i] = temp;
    }
  }

  /* Make sure that isf[M11-2] will not get values above 16384 */
  if ( Sub_fr1x16 ( isf[M11 - 2], 16384 ) > 0 )
  {
    isf[M11 - 2] = 16384;
  }
    
  return;
}

#endif

