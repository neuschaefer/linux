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
 *   WBAMR_API.C
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

/*
 * include files
 */
#include "wbamr_private.h"
#include "WBAMR_Lib.h"
//#include "lib_alloc.h"
//#include "WBAMR_Mem.h"
#include "wbamr_cnst.h"
#include "wbamr_rom.h"
#include "wbamr_cod_main.h"
#include "wbamr_dec_main.h"
#include "wbamr_sections.h"
#include "wbamr_bits.h"


/*
 * These are rounding mode utilities
 */

// Eddy 2008.06.23 Remove warning

WBAMR_CODE_SECTION static void set_rnd_mode_to_unbiased (void)
{
}


/*
 * This is the entry point to the wrapper routine
 * for the AIL interfaced encoder
 */

WBAMR_CODE_SECTION int WBAMR_AIL_Enc (
  struct WBAMR_IO_Enc_Struct *Ptr0,
  void * Ptr1 )
{
  int_native reset_flag;     /* used for store the previous reset flag */
  fract16 mode;              /* Mode from Higher layer */
  fract16 *SpeechBuff;       /* 320 PCM speech samples buffer pointer */
  fract16 *usedMode;         /* Actual used mode */
  fract16 *tx_type;          /* Frame type */
  uint16 *SpeechPara;        /* 30 words Output encoder parameters */
  uint16 giDTXon;            /* dtx flag */
  int32 rnd_mode = 0;        /* Rounding mode status flag */
  uint8 vad;                 /* vad reporting flag */
  uint8 nsync;               /* TX/RX DTX handler sync mode */
  void *vad_callback;        /* function ptr for early VAD reporting */

  WBAMR_Static_Mem_Enc_Struct *wbamr_enc_obj_ptr = Ptr1;


  void *wbamr_enc_temp_ptr, *wbamr_enc_obj_temp_ptr;

  wbamr_enc_temp_ptr     = ( void * ) Ptr0->temp_0;
  wbamr_enc_obj_temp_ptr = ( void * ) Ptr0->temp_1;

  /*
   * Input parameters extracting from IO pointer
   */
  SpeechPara   = Ptr0->parms;
                  /* pointer to output buffer */
  SpeechBuff   = Ptr0->speech;
                  /* pointer to input buffer  */
  giDTXon      = Ptr0->dtx_mode;
                  /* dtx mode on /off         */
  mode         = Ptr0->mode;
                  /* bit rate mode requested  */
  usedMode     = &Ptr0->usedMode;
                  /* bit rate mode used       */
  tx_type      = &Ptr0->tx_type;
                  /* pointer to frame transmitted type    */
  nsync        = ( uint8 ) Ptr0->nsync_mode;
                  /* TX/RX DTX handler sync mode          */
  vad_callback = ( void * ) Ptr0->vad_callback;
                  /* function ptr for early VAD reporting */

  /*
   * check for homing frame
   */
  reset_flag = WBAMR_Encoder_homing_frame_test ( SpeechBuff );

  WBAMR_Encoder (
                 mode,
                 usedMode,
                 SpeechBuff,
                 SpeechPara,
                 giDTXon,
                 tx_type,
                 nsync,
                 &vad,
                 vad_callback,
                 wbamr_enc_obj_ptr,
                 wbamr_enc_temp_ptr,
                 wbamr_enc_obj_temp_ptr );

  /*
   * perform homing if homing frame was detected at encoder input
   */
  if ( reset_flag != 0 )
  {
    WBAMR_Reset_encoder ( wbamr_enc_obj_ptr, 1 );
  }

  /*
   * Output VAD flag
   */
  Ptr0->vad = vad;

  /*
   * Restore rounding mode to its state before encode
   */
  if ( rnd_mode == 0 ) set_rnd_mode_to_unbiased ();

   /*
    * Free encoder temp memory structures
    */
  /*
   * no possible errors at this time, so return success
   */
  return ( SC_RETURN_SUCCESS );
}


/*
 * This is the entry point to the wrapper routine
 * for the AIL interfaced decoder
 */
WBAMR_CODE_SECTION int WBAMR_AIL_Dec (
  struct WBAMR_IO_Dec_Struct *Ptr0,
  void * Ptr1 )
{
  fract32 i;
  fract16 mode;
  fract16 rx_type;
  fract16 *DecodedSpeechFrame;     /* resconstructed speech           */
  uint16 *SpeechPara;              /* input encoded speech parameters */
  int32 rnd_mode=0;                /* Rounding mode status flag       */
  uint32 reset_flag;
  WBAMR_Static_Mem_Dec_Struct* wbamr_dec_obj_ptr = Ptr1;

  /*
   * Cast the incoming pointer to scratch areas into known structs
   */

    void *wbamr_dec_temp_ptr = ( void * ) Ptr0->temp_0;
    void *wbamr_dec_obj_temp_ptr = ( void * ) Ptr0->temp_1;


  DecodedSpeechFrame = ( fract16* ) Ptr0->speech;
                        /* pointer to output buffer of 320 */
  SpeechPara         = Ptr0->parms;
                        /* pointer to input buffer  of 30  */
  //mode               = Ptr0->mode;
                        /* encoded frame  mode             */
  //rx_type            = Ptr0->rx_type;
                        /* recieved frame type             */
  WBAMR_MIME_Unpack_Bits (Ptr0,
                          &mode,
                          &rx_type,
                          SpeechPara,
                          ((WBAMR_Temp_Mem_Dec_Struct *)(wbamr_dec_temp_ptr))->dec_main_scr.u2.OrdparamsBuf,
                          ((WBAMR_Temp_Mem_Dec_Struct *)(wbamr_dec_temp_ptr))->dec_main_scr.paramsBuf );

  /*
  if ( ( rx_type == RX_SID_BAD ) || ( rx_type == RX_SID_UPDATE ) )
  {
    // Override mode to MRDTX
    mode = MRDTX;
  }

  // Eddy
  //Ptr0->mode = mode;

  if ( mode <= MRDTX )
  {
    WBAMR_Deorder_Bits ( mode,
                         ((WBAMR_Temp_Mem_Dec_Struct *)(wbamr_dec_temp_ptr))->dec_main_scr.u2.OrdparamsBuf,
                         ((WBAMR_Temp_Mem_Dec_Struct *)(wbamr_dec_temp_ptr))->dec_main_scr.paramsBuf );
  }
  */


  WBAMR_Decoder (
                 mode,
                 SpeechPara,
                 DecodedSpeechFrame,
                 rx_type,
                 Ptr1,
                 wbamr_dec_temp_ptr,
                 wbamr_dec_obj_temp_ptr );

   // Eddy 2009.05.07
   for (i = 0; i < L_FRAME16k; i++)   /* Delete the 2 LSBs (14-bit output) */
   {
      DecodedSpeechFrame[i] = (fract16) (DecodedSpeechFrame[i] & 0xfffC);
   }
   if ( wbamr_dec_obj_ptr->reset_flag_old == 0 )
   {
      // check whole frame
      wbamr_dec_obj_ptr->reset_flag = WBAMR_Decoder_homing_frame_test ( ((WBAMR_Temp_Mem_Dec_Struct *)wbamr_dec_temp_ptr)->dec_main_scr.paramsBuf, mode );
   }

  /*
   * reset decoder if current frame is a homing frame
   */
  reset_flag = wbamr_dec_obj_ptr->reset_flag;
  if ( reset_flag != 0 )
  {
    WBAMR_Reset_decoder ( wbamr_dec_obj_ptr, 1 );
  }
  wbamr_dec_obj_ptr->reset_flag_old = reset_flag;

  /*
   * Restore rounding mode to its state before decode
   */
  if ( rnd_mode == 0 ) set_rnd_mode_to_unbiased ();

  /*
   * Free Decoder temp memory structures
   */
  /*
   * no possible errors at this time, so return success
   */
  return ( SC_RETURN_SUCCESS );
}


/***
 * The following two functions are
 * initialization of the WBAMR encoder and decoder.
 *
 * They take an input pointer to state and initialize it.
 ***/

WBAMR_CODE_SECTION void *WBAMR_AIL_Static_Mem_Enc_Allocate (
  void *wbamr_enc_obj_ptr )
{
// Eddy 2008.06.23 Remove warning
//  int EncStateBytes = sizeof ( WBAMR_Static_Mem_Enc_Struct );

//  if ( wbamr_enc_obj_ptr == NULL )
//  {
//    ADIalloc ( int8, wbamr_enc_obj_ptr, EncStateBytes );
//  }

  if ( wbamr_enc_obj_ptr != NULL )
  {
    WBAMR_Enc_Init ( wbamr_enc_obj_ptr );
    /* encoder static structure initialization */
  }

  return ( wbamr_enc_obj_ptr );
}


WBAMR_CODE_SECTION void *WBAMR_AIL_Static_Mem_Dec_Allocate (
  void *wbamr_dec_obj_ptr )
{
// Eddy 2008.06.23 Remove warning
//  int DecStateBytes = sizeof ( WBAMR_Static_Mem_Dec_Struct );

//  if ( wbamr_dec_obj_ptr == NULL )
//  {
//    ADIalloc ( int8, wbamr_dec_obj_ptr, DecStateBytes );
//  }

  if ( wbamr_dec_obj_ptr != NULL )
  {
    WBAMR_Dec_Init ( wbamr_dec_obj_ptr );
    /* decoder static structure initialization */
  }

  return ( wbamr_dec_obj_ptr );
}


/***
 * The following two functions are
 * uninitialization of the WBAMR encoder and decoder.
 *
 * They take an input pointer to state and uninitialize it.
 ***/

WBAMR_CODE_SECTION void WBAMR_AIL_Static_Mem_Enc_DeAllocate ( void *Ptr )
{
//  ADIfree ( Ptr );
}


WBAMR_CODE_SECTION void WBAMR_AIL_Static_Mem_Dec_DeAllocate ( void *Ptr )
{
//  ADIfree ( Ptr );
}

#endif

