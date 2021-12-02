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
 *   WBAMR_DEC_MAIN.C
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
 * 02 21 2011 richie.hsieh
 * [WCPSP00000575] [IPComm] SWIP protection
 * .
 *
 * 09 21 2010 eddy.wu
 * [WCPSP00000503] Checkin SWIP Modification
 * .
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_oper_32b.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_acelp.h"
#include "wbamr_private.h"
#include "wbamr_dtx.h"
#include "wbamr_wb_vad.h"
#include "wbamr_bits.h"
#include "wbamr_math_op.h"
#include "wbamr_dec_main.h"
#include "wbamr_sections.h"
#include "WBAMR_Lib.h"
#include "awb_exp.h"

#ifdef __ANDROID__
    #include "drvb_api.h"
#else
    #include "audip_exp.h"
#endif

void AWBDec_GetBufferSize(
   unsigned int *int_buf_size,
   unsigned int *tmp_buf_size,
   unsigned int *bs_buf_size,
   unsigned int *pcm_buf_size
)
{
   *int_buf_size = ((sizeof(WBAMR_Static_Mem_Dec_Struct) + 3 ) & (~3)) +
                   ((sizeof(struct WBAMR_IO_Dec_Struct) + 3) & (~3));
   *tmp_buf_size = ( sizeof(WBAMR_Temp_Mem_Dec_Struct) + 3 ) & (~3);
   *bs_buf_size = ((((NB_BITS_MAX + 7) / 8) + 1) + 3) & (~3);
   *pcm_buf_size = L_FRAME16k * sizeof(short);
}
AWB_DEC_HANDLE *AWBDec_Init(
   void *pInterBuf
)
{
   struct WBAMR_IO_Dec_Struct *decStruct;
   WBAMR_Static_Mem_Dec_Struct *wbamr_dec_obj_ptr;
   int hdlSize;
   if((pInterBuf == NULL) || ((int)pInterBuf & 3) != 0) {
      return NULL;
   }
   decStruct = (struct WBAMR_IO_Dec_Struct *)pInterBuf;
   hdlSize = (sizeof( struct WBAMR_IO_Dec_Struct) + 3) & (~3);
   wbamr_dec_obj_ptr = (WBAMR_Static_Mem_Dec_Struct *)((uint8 *)pInterBuf + hdlSize);
   decStruct->mode = 0;
   decStruct->prev_mode = 0;
   WBAMR_Dec_Init(wbamr_dec_obj_ptr);
   return (AWB_DEC_HANDLE *)pInterBuf;
}
int AWB_Decode(
   AWB_DEC_HANDLE *awbDecHandle,
   void *tmpBuf,
   short *pcmBuf,
   unsigned char *bsBuf
)
{
   struct WBAMR_IO_Dec_Struct *dec_io_ptr;
   WBAMR_Static_Mem_Dec_Struct *wbamr_dec_obj_ptr;
   int hdlSize;

#ifdef __ANDROID__
    CHECK_MTK_HW(dec_io_ptr)
#else
    AUDIP_DRVSET();
    AUDIP_COMMON();
#endif

   dec_io_ptr = (struct WBAMR_IO_Dec_Struct *)awbDecHandle;
   hdlSize = (sizeof(struct WBAMR_IO_Dec_Struct) + 3) & (~3);
   wbamr_dec_obj_ptr = (WBAMR_Static_Mem_Dec_Struct *)((uint8 *)awbDecHandle + hdlSize);
   dec_io_ptr->speech = (fract16 *)pcmBuf;
   dec_io_ptr->parms  = (uint16 *)bsBuf;
   dec_io_ptr->temp_0 = tmpBuf;
   dec_io_ptr->temp_1 = NULL;
   WBAMR_AIL_Dec(dec_io_ptr, wbamr_dec_obj_ptr);
   return AWB_packed_size[dec_io_ptr->mode] + 1;
}


WBAMR_CODE_SECTION void WBAMR_Reset_decoder (
  WBAMR_Static_Mem_Dec_Struct *dec_obj_ptr,
  fract16 reset_all
)
{
  int_native i;

  WBAMR_Set_zero ( dec_obj_ptr->old_exc, PIT_MAX + L_INTERPOL );
  WBAMR_Set_zero ( dec_obj_ptr->past_isfq, M11 );

  dec_obj_ptr->reset_flag = 0;
  dec_obj_ptr->reset_flag_old = 1;

  /* old pitch value = 64.0 */
  dec_obj_ptr->old_T0_frac = 0;
  dec_obj_ptr->old_T0 = 64;
  dec_obj_ptr->first_frame = 1;
  dec_obj_ptr->L_gc_thres = 0;
  dec_obj_ptr->tilt_code = 0;

  WBAMR_Init_Phase_dispersion ( dec_obj_ptr->disp_mem );

  /* scaling memories for excitation */
  dec_obj_ptr->Q_old = Q_MAX;
  dec_obj_ptr->Qsubfr[3] = Q_MAX;
  dec_obj_ptr->Qsubfr[2] = Q_MAX;
  dec_obj_ptr->Qsubfr[1] = Q_MAX;
  dec_obj_ptr->Qsubfr[0] = Q_MAX;

  if ( reset_all != 0 )
  {
    /* routines initialization */
    WBAMR_Init_D_gain2 ( dec_obj_ptr->dec_gain );
    WBAMR_Init_Oversamp_16k ( dec_obj_ptr->mem_oversamp );
    WBAMR_Init_HP50_12k8 ( dec_obj_ptr->mem_sig_out );
    WBAMR_Init_Filt_6k_7k ( dec_obj_ptr->mem_hf );
    WBAMR_Init_Filt_7k ( dec_obj_ptr->mem_hf3 );
    WBAMR_Init_HP400_12k8 ( dec_obj_ptr->mem_hp400 );
    WBAMR_Init_Lagconc ( dec_obj_ptr->lag_hist );

    /* isp initialization */
    WBAMR_Copy ( (fract16 *)isp_init, dec_obj_ptr->ispold, M11 );
    WBAMR_Copy ( (fract16 *)isf_init, dec_obj_ptr->isfold, M11 );
    for ( i = 0; i < L_MEANBUF; i++ )
      WBAMR_Copy ( (fract16 *)isf_init, &dec_obj_ptr->isf_buf[i * M11], M11 );

    /* variable initialization */
    dec_obj_ptr->mem_deemph = 0;

    /* init random with 21845 */
    dec_obj_ptr->seed = 21845;
    dec_obj_ptr->seed2 = 21845;
    dec_obj_ptr->seed3 = 21845;

    dec_obj_ptr->state = 0;
    dec_obj_ptr->prev_bfi = 0;

    /* Static vectors to zero */
    WBAMR_Set_zero ( dec_obj_ptr->mem_syn_hf, M16k );
#if (AWB_OPT_VER == 2)
    for(i = 0; i < M11; i++) {
       dec_obj_ptr->mem_syn_sig[i] = 0;
    }
#else
    WBAMR_Set_zero ( dec_obj_ptr->mem_syn_hi, M11 );
    WBAMR_Set_zero ( dec_obj_ptr->mem_syn_lo, M11 );
#endif

    WBAMR_Dtx_dec_reset ( dec_obj_ptr, isf_init );
    dec_obj_ptr->vad_hist = 0;
  }

  return;
}

/*-----------------------------------------------------------------*
 *   Funtion decoder                                               *
 *       ~~~~~~~	                                           *
 *   ->Main decoder routine.                                       *
 *                                                                 *
 *-----------------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Decoder (
  fract16 coding_mode,            /* input : used mode               */
  uint16 prms[],                  /* input : parameter vector        */
  fract16 synth16k[],             /* output: synthesis speech        */
  fract16 frame_type,             /* input : received frame type     */
  WBAMR_Static_Mem_Dec_Struct *dec_obj_ptr, /* i/o : State structure */
  WBAMR_Temp_Mem_Dec_Struct *dec_scr_ptr,   /* i/o : Scratch struct  */
  fract16 *dec_obj_temp_ptr         /* i/o   : alternate Scratch     */
)
{
  fract16 *exc;

  /* LPC coefficients */
  fract16 *p_Aq;              /* ptr to A ( z ) for the 4 subframes   */

  fract16 fac, stab_fac, voice_fac, Q_new = 0;
  fract32 L_tmp, L_gain_code;

  /* Scalars */
  int_native i, j, i_subfr;
  fract16 index, tmp;
  fract16 T0, T0_frac, pit_flag, T0_max, select, T0_min = 0;
  fract16 gain_pit, gain_code, gain_code_lo;
  fract16 newDTXState, bfi, unusable_frame, nb_bits;
  fract16 vad_flag,mode;
  fract16 pit_sharp;
  uint32  reset_flag_decoder=0;
  wbamr_dec_main *wbamr_dec_main_scr_ptr;
  uint8* paramsBuf;
  fract16 corr_gain = 0;

  wbamr_dec_main_scr_ptr = &( dec_scr_ptr->dec_main_scr );
  paramsBuf = wbamr_dec_main_scr_ptr->paramsBuf;

  mode = coding_mode;
    /* Override mode to MRDTX */



  /*
   * Taking care of homing frame sequence
   */
  if ( ( frame_type == RX_NO_DATA ) | ( frame_type == RX_SPEECH_LOST ) )
  {
    mode = dec_obj_ptr->prev_mode;
  }
  else
  {
    dec_obj_ptr->prev_mode = mode;

    /*
     * if homed: check if this frame is another homing frame
     */
    if ( dec_obj_ptr->reset_flag_old == 1 )
    {
      /*
       * only check until end of first subframe
       */
      reset_flag_decoder = WBAMR_Decoder_homing_frame_test_first ( paramsBuf,
                                                                   mode );
    }
  }

  dec_obj_ptr->reset_flag = reset_flag_decoder;

  /*
   * if not homed: check whether current frame is a homing frame
   */
  // Eddy 2009.05.07
  /*
  if ( dec_obj_ptr->reset_flag_old == 0 )
  {
    // check whole frame
    dec_obj_ptr->reset_flag = WBAMR_Decoder_homing_frame_test ( paramsBuf,
                                                                mode );
  }
   */

  /*
   * produce encoder homing frame if homed & input=decoder homing frame
   */
  if ( ( reset_flag_decoder != 0 ) && ( dec_obj_ptr->reset_flag_old != 0 ) )
  {
    for ( i = 0; i < L_FRAME16k; i++ )
    {
      synth16k[i] = EHF_MASK;
    }
    return;
  }

  /*
   * if not Homing frame ,do processing the frame.
   */
  /* mode verification */
  nb_bits = nb_of_bits[mode];

  /* find the new  DTX state  SPEECH OR DTX */
  newDTXState = WBAMR_Rx_dtx_handler ( dec_obj_ptr, frame_type );

  if ( Sub_fr1x16 ( newDTXState, SPEECH ) != 0 )
  {
    WBAMR_Dtx_dec ( dec_obj_ptr,
                    wbamr_dec_main_scr_ptr->exc2,
                    newDTXState,
                    wbamr_dec_main_scr_ptr->isf,
                    &paramsBuf );
  }
  /* SPEECH action state machine  */
  if ( ( Sub_fr1x16 ( frame_type, RX_SPEECH_BAD ) == 0 ) ||
       ( Sub_fr1x16 ( frame_type, RX_SPEECH_PROBABLY_DEGRADED ) == 0 ) )
  {
    /* bfi for all index, bits are not usable */
    bfi = 1;
    unusable_frame = 0;
  }
  else if ( ( Sub_fr1x16 ( frame_type, RX_NO_DATA ) == 0 ) ||
  	    ( Sub_fr1x16 ( frame_type, RX_SPEECH_LOST ) == 0 ) )
  {
    /* bfi only for lsf, gains and pitch period */
    bfi = 1;
    unusable_frame = 1;
  }
  else
  {
    bfi = 0;
    unusable_frame = 0;
  }

  if ( bfi != 0 )
  {
    dec_obj_ptr->state = Add_fr1x16 ( dec_obj_ptr->state, 1 );

    if ( Sub_fr1x16 ( dec_obj_ptr->state, 6 ) > 0 )
    {
      dec_obj_ptr->state = 6;
    }
  }
  else
  {
    dec_obj_ptr->state = Shr_fr1x16 ( dec_obj_ptr->state, 1 );
  }

  /* If this frame is the first speech frame after CNI period,     */
  /* set the BFH state machine to an appropriate state depending   */
  /* on whether there was DTX muting before start of speech or not */
  /* If there was DTX muting, the first speech frame is muted.     */
  /* If there was no DTX muting, the first speech frame is not     */
  /* muted. The BFH state machine starts from state 5, however, to */
  /* keep the audible noise resulting from a SID frame which is    */
  /* erroneously interpreted as a good speech frame as small as    */
  /* possible ( the decoder output in this case is quickly muted ) */

  if ( Sub_fr1x16 ( dec_obj_ptr->dtxGlobalState, DTX ) == 0 )
  {
    dec_obj_ptr->state = 5;
    dec_obj_ptr->prev_bfi = 0;
  }
  else if ( Sub_fr1x16 ( dec_obj_ptr->dtxGlobalState, DTX_MUTE ) == 0 )
  {
    dec_obj_ptr->state = 5;
    dec_obj_ptr->prev_bfi = 1;
  }

  if ( Sub_fr1x16 ( newDTXState, SPEECH ) == 0 )
  {
    vad_flag = WBAMR_Serial_parm ( 1, &paramsBuf );

    if ( bfi == 0 )
    {
      if ( vad_flag == 0 )
      {
        dec_obj_ptr->vad_hist = Add_fr1x16 ( dec_obj_ptr->vad_hist, 1 );
      } else
      {
        dec_obj_ptr->vad_hist = 0;
      }
    }
  }
  /*----------------------------------------------------------------------*
   *                DTX-CNG                                               *
   *----------------------------------------------------------------------*/

  if ( Sub_fr1x16 ( newDTXState, SPEECH ) != 0 )   /* CNG mode */
  {
    /* increase slightly energy of noise below 200 Hz */
    /* Convert ISFs to the cosine domain */
    WBAMR_Isf_isp ( wbamr_dec_main_scr_ptr->isf,
                    wbamr_dec_main_scr_ptr->ispnew,
                    M11 );

    WBAMR_Isp_Az ( wbamr_dec_main_scr_ptr->ispnew,
                   wbamr_dec_main_scr_ptr->Aq,
                   M11,
                   &dec_scr_ptr->comm_scr );

    WBAMR_Copy ( dec_obj_ptr->isfold, wbamr_dec_main_scr_ptr->isf_tmp, M11 );

    //Eddy
    for ( i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR )
    {
      j = Shr_fr1x16 ( ( fract16 )i_subfr, 6 );
      for ( i = 0; i < M11; i++ )
      {
        L_tmp = Mult_fr1x32 ( wbamr_dec_main_scr_ptr->isf_tmp[i],
                              Sub_fr1x16 ( 32767, interpol_frac[j] ) );
        L_tmp = Mac_fr16_32 ( L_tmp,
                              wbamr_dec_main_scr_ptr->isf[i],
                              interpol_frac[j] );
        wbamr_dec_main_scr_ptr->HfIsf[i] = Round_fr1x32 ( L_tmp );
      }
      WBAMR_Dec_synthesis ( wbamr_dec_main_scr_ptr->Aq,
                            &wbamr_dec_main_scr_ptr->exc2[i_subfr],
                            0,
                            &synth16k[i_subfr * 5 / 4],
                            ( fract16 ) 1,
                            wbamr_dec_main_scr_ptr->HfIsf,
                            nb_bits,
                            newDTXState,
                            dec_obj_ptr,
                            bfi,
                            dec_scr_ptr,
                            dec_obj_temp_ptr );
    }

    /* reset speech coder memories */
    WBAMR_Reset_decoder ( dec_obj_ptr, 0 );

    WBAMR_Copy ( wbamr_dec_main_scr_ptr->isf, dec_obj_ptr->isfold, M11 );

    dec_obj_ptr->prev_bfi = bfi;
    dec_obj_ptr->dtxGlobalState = newDTXState;

    return;
  }
  /*----------------------------------------------------------------------*
   *                                ACELP                                 *
   *----------------------------------------------------------------------*/

  /* copy coder memory state into working space (internal memory for DSP) */

  WBAMR_Copy ( dec_obj_ptr->old_exc,
               wbamr_dec_main_scr_ptr->u2.old_exc,
               PIT_MAX + L_INTERPOL );
  exc = wbamr_dec_main_scr_ptr->u2.old_exc + PIT_MAX + L_INTERPOL;

  /* Decode the ISFs */
  if ( Sub_fr1x16 ( nb_bits, NBBITS_7k ) <= 0 )
  {
    wbamr_dec_main_scr_ptr->ind[0] = WBAMR_Serial_parm ( 8, &paramsBuf );
    wbamr_dec_main_scr_ptr->ind[1] = WBAMR_Serial_parm ( 8, &paramsBuf );
    wbamr_dec_main_scr_ptr->ind[2] = WBAMR_Serial_parm ( 7, &paramsBuf );
    wbamr_dec_main_scr_ptr->ind[3] = WBAMR_Serial_parm ( 7, &paramsBuf );
    wbamr_dec_main_scr_ptr->ind[4] = WBAMR_Serial_parm ( 6, &paramsBuf );

    WBAMR_Dpisf_2s_36b ( wbamr_dec_main_scr_ptr->ind,
                         wbamr_dec_main_scr_ptr->isf,
                         dec_obj_ptr->past_isfq,
                         dec_obj_ptr->isfold,
                         dec_obj_ptr->isf_buf,
                         bfi,
                         1,
                         wbamr_dec_main_scr_ptr->u1.ref_isf );
  }
  else
  {
    wbamr_dec_main_scr_ptr->ind[0] = WBAMR_Serial_parm ( 8, &paramsBuf );
    wbamr_dec_main_scr_ptr->ind[1] = WBAMR_Serial_parm ( 8, &paramsBuf );
    wbamr_dec_main_scr_ptr->ind[2] = WBAMR_Serial_parm ( 6, &paramsBuf );
    wbamr_dec_main_scr_ptr->ind[3] = WBAMR_Serial_parm ( 7, &paramsBuf );
    wbamr_dec_main_scr_ptr->ind[4] = WBAMR_Serial_parm ( 7, &paramsBuf );
    wbamr_dec_main_scr_ptr->ind[5] = WBAMR_Serial_parm ( 5, &paramsBuf );
    wbamr_dec_main_scr_ptr->ind[6] = WBAMR_Serial_parm ( 5, &paramsBuf );

    WBAMR_Dpisf_2s_46b ( wbamr_dec_main_scr_ptr->ind,
                         wbamr_dec_main_scr_ptr->isf,
                         dec_obj_ptr->past_isfq,
                         dec_obj_ptr->isfold,
                         dec_obj_ptr->isf_buf,
                         bfi,
                         1,
                         wbamr_dec_main_scr_ptr->u1.ref_isf );
  }

  /* Convert ISFs to the cosine domain */
  WBAMR_Isf_isp ( wbamr_dec_main_scr_ptr->isf,
                  wbamr_dec_main_scr_ptr->ispnew,
                  M11 );

  if ( dec_obj_ptr->first_frame != 0 )
  {
    dec_obj_ptr->first_frame = 0;
    WBAMR_Copy ( wbamr_dec_main_scr_ptr->ispnew, dec_obj_ptr->ispold, M11 );
  }
  /* Find the interpolated ISPs and convert to a[] for all subframes */
  WBAMR_Int_isp ( dec_obj_ptr->ispold,
                  wbamr_dec_main_scr_ptr->ispnew,
                  interpol_frac,
                  wbamr_dec_main_scr_ptr->Aq,
                  &dec_scr_ptr->comm_scr );

  /* update ispold[] for the next frame */
  WBAMR_Copy ( wbamr_dec_main_scr_ptr->ispnew, dec_obj_ptr->ispold, M11 );

  /* Check stability on isf : distance between old isf and current isf */
  L_tmp = 0;
  for ( i = 0; i < M11 - 1; i++ )
  {
    tmp = Sub_fr1x16 ( wbamr_dec_main_scr_ptr->isf[i], dec_obj_ptr->isfold[i] );
    L_tmp = Mac_fr16_32 ( L_tmp, tmp, tmp );
  }
  tmp = Extract_hi_fr1x32 ( Shl_fr1x32 ( L_tmp, 8 ) );
  tmp = Mult_fr1x16 ( tmp, 26214 );        /* tmp = L_tmp*0.8/256 */

  tmp = Sub_fr1x16 ( 20480, tmp );         /* 1.25 - tmp */
  stab_fac = Shl_fr1x16 ( tmp, 1 );        /* Q14 -> Q15 with saturation */

  if ( stab_fac < 0 )
  {
    stab_fac = 0;
  }
  WBAMR_Copy ( dec_obj_ptr->isfold, wbamr_dec_main_scr_ptr->isf_tmp, M11 );
  WBAMR_Copy ( wbamr_dec_main_scr_ptr->isf, dec_obj_ptr->isfold, M11 );

  /*------------------------------------------------------------------------*
   *        Loop for every subframe in the analysis frame                   *
   *------------------------------------------------------------------------*
   * The subframe size is L_SUBFR and the loop is repeated L_FRAME/L_SUBFR  *
   * times                                                                  *
   *   - decode the pitch delay and filter mode                             *
   *   - decode algebraic code                                              *
   *   - decode pitch and codebook gains                                    *
   *   - find voicing factor and tilt of code for next subframe.            *
   *   - find the excitation and compute synthesis speech                   *
   *------------------------------------------------------------------------*/
  /* pointer to interpolated LPC parameters */
  p_Aq = wbamr_dec_main_scr_ptr->Aq;

  for ( i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR )
  {
    pit_flag = ( fract16 ) i_subfr;

    if ( ( ( i_subfr-( 2*L_SUBFR ) ) == 0 ) &&
         ( Sub_fr1x16 ( nb_bits, NBBITS_7k ) > 0 ) )
    {
      pit_flag = 0;
    }
    /*-------------------------------------------------*
     * - Decode pitch lag                              *
     * Lag indeces received also in case of BFI,       *
     * so that the parameter pointer stays in sync.    *
     *-------------------------------------------------*/

    if ( pit_flag == 0 )
    {
      if ( Sub_fr1x16 ( nb_bits, NBBITS_9k ) <= 0 )
      {
        index = WBAMR_Serial_parm ( 8, &paramsBuf );
        if ( Sub_fr1x16 ( index, ( PIT_FR1_8b - PIT_MIN ) * 2 ) < 0 )
        {
          T0 = Add_fr1x16 ( PIT_MIN, Shr_fr1x16 ( index, 1 ) );
          T0_frac = Sub_fr1x16 ( index,
                                 Shl_fr1x16 ( Sub_fr1x16(T0, PIT_MIN), 1 ) );
          T0_frac = Shl_fr1x16 ( T0_frac, 1 );
        }
        else
        {
          T0 = Add_fr1x16 ( index,
                            PIT_FR1_8b - ( ( PIT_FR1_8b - PIT_MIN ) * 2 ) );
          T0_frac = 0;
        }
      }
      else
      {
        index = WBAMR_Serial_parm ( 9, &paramsBuf );

        if ( Sub_fr1x16 ( index, ( PIT_FR2 - PIT_MIN ) * 4 ) < 0 )
        {
          T0 = Add_fr1x16 ( PIT_MIN, Shr_fr1x16 ( index, 2 ) );
          T0_frac = Sub_fr1x16 ( index,
                                 Shl_fr1x16 ( Sub_fr1x16(T0, PIT_MIN), 2 ) );
        }
        else if ( Sub_fr1x16 (
                   index,
                   (((PIT_FR2 - PIT_MIN) * 4) + ((PIT_FR1_9b - PIT_FR2) * 2)))
                  < 0 )
        {
          index = Sub_fr1x16 ( index, ( PIT_FR2 - PIT_MIN ) * 4 );
          T0 = Add_fr1x16 ( PIT_FR2, Shr_fr1x16 ( index, 1 ) );
          T0_frac = Sub_fr1x16 ( index,
                                 Shl_fr1x16 ( Sub_fr1x16(T0, PIT_FR2), 1 ) );
          T0_frac = Shl_fr1x16 ( T0_frac, 1 );
        }
        else
        {
          T0 = Add_fr1x16 (
                index,
                (PIT_FR1_9b-((PIT_FR2-PIT_MIN)*4)-((PIT_FR1_9b-PIT_FR2)*2)));
          T0_frac = 0;
        }
      }

      /* find T0_min and T0_max for subframe 2 and 4 */

      T0_min = Sub_fr1x16 ( T0, 8 );

      if ( Sub_fr1x16 ( T0_min, PIT_MIN ) < 0 )
      {
        T0_min = PIT_MIN;
      }
      T0_max = Add_fr1x16 ( T0_min, 15 );

      if ( Sub_fr1x16 ( T0_max, PIT_MAX ) > 0 )
      {
        T0_max = PIT_MAX;
        T0_min = Sub_fr1x16 ( T0_max, 15 );
      }
    }
    else
    {                  /* if subframe 2 or 4 */
      if ( Sub_fr1x16 ( nb_bits, NBBITS_9k ) <= 0 )
      {
        index = WBAMR_Serial_parm ( 5, &paramsBuf );

        T0 = Add_fr1x16 ( T0_min, Shr_fr1x16 ( index, 1 ) );
        T0_frac = Sub_fr1x16 ( index,
                               Shl_fr1x16 ( Sub_fr1x16 ( T0,T0_min ), 1 ) );
        T0_frac = Shl_fr1x16 ( T0_frac, 1 );
      } else
      {
        index = WBAMR_Serial_parm ( 6, &paramsBuf );

        T0 = Add_fr1x16 ( T0_min, Shr_fr1x16 ( index, 2 ) );
        T0_frac = Sub_fr1x16 ( index,
                               Shl_fr1x16 ( Sub_fr1x16 ( T0, T0_min ), 2 ) );
      }
    }

    /* check BFI after pitch lag decoding */
    if ( bfi != 0 ) /* if frame erasure */
    {
      WBAMR_Lagconc ( &( dec_obj_ptr->dec_gain[17] ),
                      dec_obj_ptr->lag_hist,
                      &T0,
                      &( dec_obj_ptr->old_T0 ),
                      &( dec_obj_ptr->seed3 ),
                      unusable_frame,
                      wbamr_dec_main_scr_ptr->u1.lag_hist2 );
      T0_frac = 0;
    }
    /*-------------------------------------------------*
     * - Find the pitch gain, the interpolation filter *
     *   and the adaptive codebook vector.             *
     *-------------------------------------------------*/
    WBAMR_Pred_lt4 ( &exc[i_subfr], T0, T0_frac );

    if ( unusable_frame )
    {
      select = 1;
    }
    else
    {
      if ( Sub_fr1x16 ( nb_bits, NBBITS_9k ) <= 0 )
      {
        select = 0;
      } else
      {
        select = WBAMR_Serial_parm ( 1, &paramsBuf );
      }
    }

    if ( select == 0 )
    {
      /* find pitch excitation with lp filter */
      for ( i = 0; i < L_SUBFR; i++ )
      {
        L_tmp = Mult_fr1x32 ( 5898, exc[i - 1 + i_subfr] );
        L_tmp = Mac_fr16_32 ( L_tmp, 20972, exc[i + i_subfr] );
        L_tmp = Mac_fr16_32 ( L_tmp, 5898, exc[i + 1 + i_subfr] );
        wbamr_dec_main_scr_ptr->code[i] = Round_fr1x32 ( L_tmp );
      }
      WBAMR_Copy ( wbamr_dec_main_scr_ptr->code, &exc[i_subfr], L_SUBFR );
    }
    /*-------------------------------------------------------*
     * - Decode innovative codebook.                         *
     * - Add the fixed-gain pitch contribution to code[].    *
     *-------------------------------------------------------*/
   //Eddy
    if ( unusable_frame != 0 )
    {
      /* the innovative code doesn't need to be scaled ( see Q_gain2 ) */
      for ( i = 0; i < L_SUBFR; i++ )
      {
        wbamr_dec_main_scr_ptr->code[i] = Shr_fr1x16 (
                                          WBAMR_Random(&( dec_obj_ptr->seed )),
                                          3 );
      }
    }
    else if ( Sub_fr1x16 ( nb_bits, NBBITS_7k ) <= 0 )
    {
      wbamr_dec_main_scr_ptr->ind[0] = WBAMR_Serial_parm ( 12, &paramsBuf );
      WBAMR_DEC_ACELP_2t64_fx ( wbamr_dec_main_scr_ptr->ind[0],
                                wbamr_dec_main_scr_ptr->code );
    }
    else if ( Sub_fr1x16 ( nb_bits, NBBITS_9k ) <= 0 )
    {
      for ( i = 0; i < 4; i++ )
      {
        wbamr_dec_main_scr_ptr->ind[i] = WBAMR_Serial_parm ( 5, &paramsBuf );
      }
      WBAMR_DEC_ACELP_4t64_fx ( 20,
                                wbamr_dec_main_scr_ptr->ind,
                                wbamr_dec_main_scr_ptr->code,
                                dec_scr_ptr );
    }
    else if ( Sub_fr1x16 ( nb_bits, NBBITS_12k ) <= 0 )
    {
      for ( i = 0; i < 4; i++ )
      {
        wbamr_dec_main_scr_ptr->ind[i] = WBAMR_Serial_parm ( 9, &paramsBuf );
      }
      WBAMR_DEC_ACELP_4t64_fx ( 36,
                                wbamr_dec_main_scr_ptr->ind,
                                wbamr_dec_main_scr_ptr->code,
                                dec_scr_ptr );
    }
    else if ( Sub_fr1x16 ( nb_bits, NBBITS_14k ) <= 0 )
    {
      wbamr_dec_main_scr_ptr->ind[0] = WBAMR_Serial_parm ( 13, &paramsBuf );
      wbamr_dec_main_scr_ptr->ind[1] = WBAMR_Serial_parm ( 13, &paramsBuf );
      wbamr_dec_main_scr_ptr->ind[2] = WBAMR_Serial_parm ( 9, &paramsBuf );
      wbamr_dec_main_scr_ptr->ind[3] = WBAMR_Serial_parm ( 9, &paramsBuf );
      WBAMR_DEC_ACELP_4t64_fx ( 44,
                                wbamr_dec_main_scr_ptr->ind,
                                wbamr_dec_main_scr_ptr->code,
                                dec_scr_ptr );
    }
    else if ( Sub_fr1x16 ( nb_bits, NBBITS_16k ) <= 0 )
    {
      for ( i = 0; i < 4; i++ )
      {
        wbamr_dec_main_scr_ptr->ind[i] = WBAMR_Serial_parm ( 13, &paramsBuf );
      }
      WBAMR_DEC_ACELP_4t64_fx ( 52,
                                wbamr_dec_main_scr_ptr->ind,
                                wbamr_dec_main_scr_ptr->code,
                                dec_scr_ptr );
    }
    else if ( Sub_fr1x16 ( nb_bits, NBBITS_18k ) <= 0 )
    {
      for ( i = 0; i < 4; i++ )
      {
        wbamr_dec_main_scr_ptr->ind[i] = WBAMR_Serial_parm ( 2, &paramsBuf );
      }
      for ( i = 4; i < 8; i++ )
      {
        wbamr_dec_main_scr_ptr->ind[i] = WBAMR_Serial_parm ( 14, &paramsBuf );
      }
      WBAMR_DEC_ACELP_4t64_fx ( 64,
                                wbamr_dec_main_scr_ptr->ind,
                                wbamr_dec_main_scr_ptr->code,
                                dec_scr_ptr );
    }
    else if ( Sub_fr1x16 ( nb_bits, NBBITS_20k ) <= 0 )
    {
      wbamr_dec_main_scr_ptr->ind[0] = WBAMR_Serial_parm ( 10, &paramsBuf );
      wbamr_dec_main_scr_ptr->ind[1] = WBAMR_Serial_parm ( 10, &paramsBuf );
      wbamr_dec_main_scr_ptr->ind[2] = WBAMR_Serial_parm ( 2, &paramsBuf );
      wbamr_dec_main_scr_ptr->ind[3] = WBAMR_Serial_parm ( 2, &paramsBuf );
      wbamr_dec_main_scr_ptr->ind[4] = WBAMR_Serial_parm ( 10, &paramsBuf );
      wbamr_dec_main_scr_ptr->ind[5] = WBAMR_Serial_parm ( 10, &paramsBuf );
      wbamr_dec_main_scr_ptr->ind[6] = WBAMR_Serial_parm ( 14, &paramsBuf );
      wbamr_dec_main_scr_ptr->ind[7] = WBAMR_Serial_parm ( 14, &paramsBuf );
      WBAMR_DEC_ACELP_4t64_fx ( 72,
                                wbamr_dec_main_scr_ptr->ind,
                                wbamr_dec_main_scr_ptr->code,
                                dec_scr_ptr );
    }
    else
    {
      for ( i = 0; i < 4; i++ )
      {
        wbamr_dec_main_scr_ptr->ind[i] = WBAMR_Serial_parm ( 11, &paramsBuf );
      }
      for ( i = 4; i < 8; i++ )
      {
        wbamr_dec_main_scr_ptr->ind[i] = WBAMR_Serial_parm ( 11, &paramsBuf );
      }
      WBAMR_DEC_ACELP_4t64_fx ( 88,
                                wbamr_dec_main_scr_ptr->ind,
                                wbamr_dec_main_scr_ptr->code,
                                dec_scr_ptr );
    }

    tmp = 0;
    WBAMR_Preemph ( wbamr_dec_main_scr_ptr->code,
                    dec_obj_ptr->tilt_code,
                    &tmp );
    tmp = T0;
    if ( Sub_fr1x16 ( T0_frac, 2 ) > 0 )
    {
      tmp = Add_fr1x16 ( tmp, 1 );
    }
    WBAMR_Pit_shrp ( wbamr_dec_main_scr_ptr->code, tmp );

    /*-------------------------------------------------*
     * - Decode codebooks gains.                       *
     *-------------------------------------------------*/

    if ( Sub_fr1x16 ( nb_bits, NBBITS_9k ) <= 0 )
    {
      index = WBAMR_Serial_parm ( 6, &paramsBuf ); /* codebook gain index */

      WBAMR_D_gain2 ( index,
                      6,
                      wbamr_dec_main_scr_ptr->code,
                      &gain_pit,
                      &L_gain_code,
                      bfi,
                      unusable_frame,
                      dec_obj_ptr );
    }
    else
    {
      index = WBAMR_Serial_parm ( 7, &paramsBuf ); /* codebook gain index */

      WBAMR_D_gain2 ( index,
                      7,
                      wbamr_dec_main_scr_ptr->code,
                      &gain_pit,
                      &L_gain_code,
                      bfi,
                      unusable_frame,
                      dec_obj_ptr );
    }

    /* find best scaling to perform on excitation ( Q_new ) */
    tmp = dec_obj_ptr->Qsubfr[0];
    for ( i = 1; i < 4; i++ )
    {
      if ( Sub_fr1x16 ( dec_obj_ptr->Qsubfr[i], tmp ) < 0 )
      {
        tmp = dec_obj_ptr->Qsubfr[i];
      }
    }

    /* limit scaling (Q_new) to Q_MAX: see cnst.h and syn_filt_32 () */

    if ( Sub_fr1x16 ( tmp, Q_MAX ) > 0 )
    {
      tmp = Q_MAX;
    }
    Q_new = 0;
    L_tmp = L_gain_code;                     /* L_gain_code in Q16 */

    while ( ( Sub_fr1x32 ( L_tmp, 0x08000000L ) < 0 ) &&
            ( Sub_fr1x16 ( Q_new, tmp ) < 0 ) )
    {
      L_tmp = Shl_fr1x32 ( L_tmp, 1 );
      Q_new = Add_fr1x16 ( Q_new, 1 );
    }
    gain_code = Round_fr1x32 ( L_tmp );      /* scaled gain_code with Qnew */

    WBAMR_Scale_sig ( exc + i_subfr - ( PIT_MAX + L_INTERPOL ),
                      PIT_MAX + L_INTERPOL + L_SUBFR,
                      Sub_fr1x16 ( Q_new, dec_obj_ptr->Q_old ) );
    dec_obj_ptr->Q_old = Q_new;


    /*----------------------------------------------------------*
     * Update parameters for the next subframe.                 *
     * - tilt of code: 0.0 ( unvoiced ) to 0.5 ( voiced )       *
     *----------------------------------------------------------*/

    if ( bfi == 0 )
    {
      /* LTP-Lag history update */
      for ( i = 4; i > 0; i-- )
      {
        dec_obj_ptr->lag_hist[i] = dec_obj_ptr->lag_hist[i - 1];
      }
      dec_obj_ptr->lag_hist[0] = T0;

      dec_obj_ptr->old_T0 = T0;
      dec_obj_ptr->old_T0_frac = 0; /* Remove fraction in case of BFI */
    }
    /* find voice factor in Q15 ( 1=voiced, -1=unvoiced ) */
    WBAMR_Copy ( &exc[i_subfr], wbamr_dec_main_scr_ptr->exc2, L_SUBFR );
    WBAMR_Scale_sig ( wbamr_dec_main_scr_ptr->exc2, L_SUBFR, -3 );

    //Eddy
    /* post processing of excitation elements */
    if ( Sub_fr1x16 ( nb_bits, NBBITS_9k ) <= 0 )
    {
#if (AWB_OPT_VER == 2)
      {
         fract32 pit_square;
         fract32 *pExc2;
         fract16 *pExcp;
         fract32 wExc2;

         pExc2 = (fract32 *)wbamr_dec_main_scr_ptr->exc2;
         pExcp = wbamr_dec_main_scr_ptr->excp;

         pit_sharp = AWB_qadd(gain_pit, gain_pit);
         pit_square = AWB_smulbb(pit_sharp, pit_sharp);

         if ( pit_sharp >  16384)
         {
            i = L_SUBFR >> 2;
            do {
               wExc2 = *pExc2++;
               L_tmp = AWB_smlawb(pit_square, wExc2, 0x8000);
               *pExcp++ = (fract16)(L_tmp >> 16);
               L_tmp = AWB_smlawt(pit_square, wExc2, 0x8000);
               *pExcp++ = (fract16)(L_tmp >> 16);
               wExc2 = *pExc2++;
               L_tmp = AWB_smlawb(pit_square, wExc2, 0x8000);
               *pExcp++ = (fract16)(L_tmp >> 16);
               L_tmp = AWB_smlawt(pit_square, wExc2, 0x8000);
               *pExcp++ = (fract16)(L_tmp >> 16);
            } while(--i != 0);
         }
      }
#else
      pit_sharp = Shl_fr1x16 ( gain_pit, 1 );

      if ( Sub_fr1x16 ( pit_sharp, 16384 ) > 0 )
      {
        for ( i = 0; i < L_SUBFR; i++ )
        {
          tmp = Mult_fr1x16 ( wbamr_dec_main_scr_ptr->exc2[i], pit_sharp );
          L_tmp = Mult_fr1x32 ( tmp, gain_pit );
          L_tmp = Shr_fr1x32 ( L_tmp, 1 );
          wbamr_dec_main_scr_ptr->excp[i] = Round_fr1x32 ( L_tmp );

        }
      }
#endif
    }
    else
    {
      pit_sharp = 0;
    }


    voice_fac = WBAMR_voice_factor ( wbamr_dec_main_scr_ptr->exc2,
                                     -3,
                                     gain_pit,
                                     wbamr_dec_main_scr_ptr->code,
                                     gain_code );

    /* tilt of code for next subframe: 0.5=voiced, 0=unvoiced */
    dec_obj_ptr->tilt_code = Add_fr1x16 ( Shr_fr1x16 ( voice_fac, 2 ),
                                          8192 );

    /*-------------------------------------------------------*
     * - Find the total excitation.                          *
     * - Find synthesis speech corresponding to exc[].       *
     *-------------------------------------------------------*/
    WBAMR_Copy ( &exc[i_subfr], wbamr_dec_main_scr_ptr->exc2, L_SUBFR );

#if (AWB_OPT_VER == 2)
    {
       fract32 mini, maxi;
       fract32 gco, gpi;
       fract32 wCode, wExc;
       fract32 *pExc, *pCode;
       fract16 *pExc16;

       pCode = (fract32 *)wbamr_dec_main_scr_ptr->code;
       pExc = (fract32 *)(&exc[i_subfr]);
       pExc16 = &exc[i_subfr];
       gco = ((fract32)gain_code) << 16;
       gpi = ((fract32)gain_pit) << 11;
       mini = -1;
       maxi = 1;


       i = L_SUBFR >> 1;
       do {
          wCode = *pCode++;
          wExc = *pExc++;
          L_tmp = AWB_smlawb(gco, wCode, 0x0100);
          L_tmp = AWB_smlawb(gpi, wExc, L_tmp);
          if(L_tmp >= 0) {
             if(L_tmp > maxi)
                maxi = L_tmp;
             if(L_tmp >= 0x01000000)
                *pExc16++ = 0x7fff;
             else
                *pExc16++ = (fract16)(L_tmp >> 9);
          } else {
             if(L_tmp < mini)
                mini = L_tmp;
             if(L_tmp < 0xFF000000)
                *pExc16++ = (fract16)0x8000;
             else
                *pExc16++ = (fract16)(L_tmp >> 9);
          }
          L_tmp = AWB_smlawt(gco, wCode, 0x0100);
          L_tmp = AWB_smlawt(gpi, wExc, L_tmp);
          if(L_tmp >= 0) {
             if(L_tmp > maxi)
                maxi = L_tmp;
             if(L_tmp > 0x01000000)
                *pExc16++ = 0x7fff;
             else
                *pExc16++ = (fract16)(L_tmp >> 9);
          } else {
             if(L_tmp < mini)
                mini = L_tmp;
             if(L_tmp < 0xFF000000)
                *pExc16++ = (fract16)(0x8000);
             else
                *pExc16++ = (fract16)(L_tmp >> 9);
          }

       } while(--i != 0);

       if(maxi + mini < 0) {
          maxi = -mini;
       }
       maxi = Shr_fr1x32(maxi, 9);
       /* tmp = scaling possible according to max value of excitation */
       tmp = WBAMR_Norm_s ( (fract16)maxi ) + Q_new - 1;
    }

#else
	fract16 max;
	{
		for ( i = 0; i < L_SUBFR; i++ )
		{
		  L_tmp = Mult_fr1x32 ( wbamr_dec_main_scr_ptr->code[i], gain_code );
		  L_tmp = Shl_fr1x32 ( L_tmp, 5 );
		  L_tmp = Mac_fr16_32 ( L_tmp, exc[i + i_subfr], gain_pit );
		  L_tmp = Shl_fr1x32 ( L_tmp, 1 );
		  exc[i + i_subfr] = Round_fr1x32 ( L_tmp );
		}

		/* find maximum value of excitation for next scaling */
		max = 1;
		for ( i = 0; i < L_SUBFR; i++ )
		{
		  tmp = Abs_fr1x16 ( exc[i + i_subfr] );

		  if ( Sub_fr1x16 ( tmp, max ) > 0 )
		  {
			max = tmp;
		  }
		}
		//printf("max:%d\n", max);

		/* tmp = scaling possible according to max value of excitation */
		tmp = Sub_fr1x16 ( Add_fr1x16 ( WBAMR_Norm_s ( max ), Q_new ), 1 );
	}
#endif

    dec_obj_ptr->Qsubfr[3] = dec_obj_ptr->Qsubfr[2];
    dec_obj_ptr->Qsubfr[2] = dec_obj_ptr->Qsubfr[1];
    dec_obj_ptr->Qsubfr[1] = dec_obj_ptr->Qsubfr[0];
    dec_obj_ptr->Qsubfr[0] = tmp;

    /*------------------------------------------------------------*
     * phase dispersion to enhance noise in low bit rate          *
     *------------------------------------------------------------*/

    /* L_gain_code in Q16 */
    WBAMR_L_Extract ( L_gain_code, &gain_code, &gain_code_lo );

    if ( Sub_fr1x16 ( nb_bits, NBBITS_7k ) <= 0 )
      j = 0;             /* high dispersion for rate <= 7.5 kbit/s */
    else if ( Sub_fr1x16 ( nb_bits, NBBITS_9k ) <= 0 )
      j = 1;             /* low dispersion for rate <= 9.6 kbit/s */
    else
      j = 2;             /* no dispersion for rate > 9.6 kbit/s */

    WBAMR_Phase_dispersion ( gain_code,
                             gain_pit,
                             wbamr_dec_main_scr_ptr->code,
                             ( fract16 )j,
                             dec_obj_ptr->disp_mem,
                             dec_scr_ptr );

    /*------------------------------------------------------------*
     * noise enhancer                                             *
     * ~~~~~~~~~~~~~~                                             *
     * - Enhance excitation on noise. ( modify gain of code )     *
     *   If signal is noisy and LPC filter is stable, move gain   *
     *   of code 1.5 dB toward gain of code threshold.            *
     *   This decrease by 3 dB noise energy variation.            *
     *------------------------------------------------------------*/

    tmp = Sub_fr1x16 ( 16384, Shr_fr1x16 ( voice_fac, 1 ) );
                                  /* 1=unvoiced, 0=voiced */
    fac = Mult_fr1x16 ( stab_fac, tmp );

    L_tmp = L_gain_code;

    if ( Sub_fr1x32 ( L_tmp, dec_obj_ptr->L_gc_thres ) < 0 )
    {
      L_tmp = Add_fr1x32 ( L_tmp,
                           WBAMR_Mpy_32_16 ( gain_code, gain_code_lo, 6226 ) );

      if ( Sub_fr1x32 ( L_tmp, dec_obj_ptr->L_gc_thres ) > 0 )
      {
        L_tmp = dec_obj_ptr->L_gc_thres;
      }
    }
    else
    {
      L_tmp = WBAMR_Mpy_32_16 ( gain_code, gain_code_lo, 27536 );

      if ( Sub_fr1x32 ( L_tmp, dec_obj_ptr->L_gc_thres ) < 0 )
      {
        L_tmp = dec_obj_ptr->L_gc_thres;
      }
    }
    dec_obj_ptr->L_gc_thres = L_tmp;

    L_gain_code = WBAMR_Mpy_32_16 ( gain_code,
                                    gain_code_lo,
                                    Sub_fr1x16 ( 32767, fac ) );
    WBAMR_L_Extract ( L_tmp, &gain_code, &gain_code_lo );
    L_gain_code = Add_fr1x32 ( L_gain_code,
                               WBAMR_Mpy_32_16 ( gain_code,
                                                 gain_code_lo, fac ) );

    /*------------------------------------------------------------*
     * pitch enhancer                                             *
     * ~~~~~~~~~~~~~~                                             *
     * - Enhance excitation on voice. ( HP filtering of code )    *
     *   On voiced signal, filtering of code by a smooth fir HP   *
     *   filter to decrease energy of code in low frequency.      *
     *------------------------------------------------------------*/

#if (AWB_OPT_VER == 2)
    {
       fract32 gco, gpi;
       fract32 wCode, halfCode, wExc2;
       fract32 *pExc2, *pCode;
       fract16 *pExc16;

       tmp = -(( voice_fac > 3 ) + 4096);
       gain_code = Round_fr1x32 ( Shl_fr1x32 ( L_gain_code, Q_new ) );

       gco = ((fract32)gain_code) << 16;
       gpi = ((fract32)gain_pit) << 11;
       pCode = (fract32 *)wbamr_dec_main_scr_ptr->code;
       pExc2 = (fract32 *)wbamr_dec_main_scr_ptr->exc2;
       pExc16 = wbamr_dec_main_scr_ptr->exc2;

       wCode = *pCode++;
       wExc2 = *pExc2++;
       halfCode = wCode << 16;
       L_tmp = AWB_smlatb(wCode, tmp, 0x8000);  // code[1]
       L_tmp = AWB_qadd(halfCode, L_tmp); // code[0]
       L_tmp = AWB_smlawt(gco, L_tmp, 0x0100);
       L_tmp = AWB_smlawb(gpi, wExc2, L_tmp);
       if(L_tmp >= 0) {
          if(L_tmp >= 0x01000000)
             *pExc16++ = 0x7fff;
          else
             *pExc16++ = (fract16)(L_tmp >> 9);
       } else {
          if(L_tmp < 0xFF000000)
             *pExc16++ = (fract16)(0x8000);
          else
             *pExc16++ = (fract16)(L_tmp >> 9);
       }

       i = (L_SUBFR >> 1) - 1;
       do {
          halfCode = wCode - ((unsigned int)halfCode >> 16);
          L_tmp = AWB_smlabb(wCode, tmp, 0x8000);  // code[0]
          wCode = *pCode++;
          L_tmp = AWB_smlabb(wCode, tmp, L_tmp);  // code[2]
          L_tmp = AWB_qadd(halfCode, L_tmp); // code[1]
          L_tmp = AWB_smlawt(gco, L_tmp, 0x0100);
          L_tmp = AWB_smlawt(gpi, wExc2, L_tmp);
          if(L_tmp >= 0) {
             if(L_tmp >= 0x01000000)
                *pExc16++ = 0x7fff;
             else
                *pExc16++ = (fract16)(L_tmp >> 9);
          } else {
             if(L_tmp < 0xFF000000)
                *pExc16++ = (fract16)(0x8000);
             else
                *pExc16++ = (fract16)(L_tmp >> 9);
          }
          wExc2 = *pExc2++;
          L_tmp = AWB_smlatb(halfCode, tmp, 0x8000);  // code[1]
          halfCode = wCode << 16;
          L_tmp = AWB_smlatb(wCode, tmp, L_tmp);  // code[3]
          L_tmp = AWB_qadd(halfCode, L_tmp); // code[2]
          L_tmp = AWB_smlawt(gco, L_tmp, 0x0100);
          L_tmp = AWB_smlawb(gpi, wExc2, L_tmp);
          if(L_tmp >= 0) {
             if(L_tmp >= 0x01000000)
                *pExc16++ = 0x7fff;
             else
                *pExc16++ = (fract16)(L_tmp >> 9);
          } else {
             if(L_tmp < 0xFF000000)
                *pExc16++ = (fract16)(0x8000);
             else
                *pExc16++ = (fract16)(L_tmp >> 9);
          }
       } while(--i != 0);
       halfCode = wCode - ((unsigned int)halfCode >> 16);
       L_tmp = AWB_smlabb(wCode, tmp, 0x8000);  // code[0]
       L_tmp = AWB_qadd(halfCode, L_tmp); // code[1]
       L_tmp = AWB_smlawt(gco, L_tmp, 0x0100);
       L_tmp = AWB_smlawt(gpi, wExc2, L_tmp);
       if(L_tmp >= 0) {
          if(L_tmp >= 0x01000000)
             *pExc16++ = 0x7fff;
          else
             *pExc16++ = (fract16)(L_tmp >> 9);
       } else {
          if(L_tmp < 0xFF000000)
             *pExc16++ = (fract16)(0x8000);
          else
             *pExc16++ = (fract16)(L_tmp >> 9);
       }
    }
#else
    tmp = Add_fr1x16 ( Shr_fr1x16 ( voice_fac, 3 ), 4096 );
                              /* 0.25=voiced, 0=unvoiced */

    L_tmp = WBAMR_L_deposit_h ( wbamr_dec_main_scr_ptr->code[0] );
    L_tmp = WBAMR_L_msu ( L_tmp, wbamr_dec_main_scr_ptr->code[1], tmp );
    wbamr_dec_main_scr_ptr->code2[0] = Round_fr1x32 ( L_tmp );


    for ( i = 1; i < L_SUBFR - 1; i++ )
    {
      L_tmp = WBAMR_L_deposit_h ( wbamr_dec_main_scr_ptr->code[i] );
      L_tmp = WBAMR_L_msu ( L_tmp, wbamr_dec_main_scr_ptr->code[i + 1], tmp );
      L_tmp = WBAMR_L_msu ( L_tmp, wbamr_dec_main_scr_ptr->code[i - 1], tmp );
      wbamr_dec_main_scr_ptr->code2[i] = Round_fr1x32 ( L_tmp );

    }

    L_tmp = WBAMR_L_deposit_h ( wbamr_dec_main_scr_ptr->code[L_SUBFR - 1] );
    L_tmp = WBAMR_L_msu ( L_tmp,
                          wbamr_dec_main_scr_ptr->code[L_SUBFR - 2],
                          tmp );
    wbamr_dec_main_scr_ptr->code2[L_SUBFR - 1] = Round_fr1x32 ( L_tmp );

    /* build excitation */
    gain_code = Round_fr1x32 ( Shl_fr1x32 ( L_gain_code, Q_new ) );

    for ( i = 0; i < L_SUBFR; i++ )
    {
      L_tmp = Mult_fr1x32 ( wbamr_dec_main_scr_ptr->code2[i], gain_code );
      L_tmp = Shl_fr1x32 ( L_tmp, 5 );
      L_tmp = Mac_fr16_32 ( L_tmp, wbamr_dec_main_scr_ptr->exc2[i], gain_pit );
      L_tmp = Shl_fr1x32 ( L_tmp, 1 );     /* saturation can occur here */
      wbamr_dec_main_scr_ptr->exc2[i] = Round_fr1x32 ( L_tmp );

    }
#endif

    if ( Sub_fr1x16 ( nb_bits, NBBITS_9k ) <= 0 )
    {
      if ( Sub_fr1x16 ( pit_sharp, 16384 ) > 0 )
      {
        for ( i = 0; i < L_SUBFR; i++ )
        {
          wbamr_dec_main_scr_ptr->excp[i] =
                      Add_fr1x16 ( wbamr_dec_main_scr_ptr->excp[i],
                                   wbamr_dec_main_scr_ptr->exc2[i] );
        }
        WBAMR_Agc2 ( wbamr_dec_main_scr_ptr->exc2,
                     wbamr_dec_main_scr_ptr->excp,
                     L_SUBFR );
        WBAMR_Copy ( wbamr_dec_main_scr_ptr->excp,
                     wbamr_dec_main_scr_ptr->exc2,
                     L_SUBFR );
      }
    }
    if ( Sub_fr1x16 ( nb_bits, NBBITS_7k ) <= 0 )
    {
      j = Shr_fr1x16 ( ( fract16 )i_subfr, 6 );

      for ( i = 0; i < M11; i++ )
      {
        L_tmp = Mult_fr1x32 ( wbamr_dec_main_scr_ptr->isf_tmp[i],
                              Sub_fr1x16 ( 32767, interpol_frac[j] ) );
        L_tmp = Mac_fr16_32 ( L_tmp,
                              wbamr_dec_main_scr_ptr->isf[i],
                              interpol_frac[j] );
        wbamr_dec_main_scr_ptr->HfIsf[i] = Round_fr1x32 ( L_tmp );
      }
    }
    else
    {
      WBAMR_Set_zero ( dec_obj_ptr->mem_syn_hf, M16k - M11 );
    }

    if ( Sub_fr1x16 ( nb_bits, NBBITS_24k ) >= 0 )
    {
      corr_gain = WBAMR_Serial_parm ( 4, &paramsBuf );
      WBAMR_Dec_synthesis ( p_Aq,
                            wbamr_dec_main_scr_ptr->exc2,
                            Q_new,
                            &synth16k[i_subfr * 5 / 4],
                            corr_gain,
                            wbamr_dec_main_scr_ptr->HfIsf,
                            nb_bits,
                            newDTXState,
                            dec_obj_ptr,
                            bfi,
                            dec_scr_ptr,
                            dec_obj_temp_ptr );
    }
    else
    {
      WBAMR_Dec_synthesis ( p_Aq,
                            wbamr_dec_main_scr_ptr->exc2,
                            Q_new,
                            &synth16k[i_subfr * 5 / 4],
                            0,
                            wbamr_dec_main_scr_ptr->HfIsf,
                            nb_bits,
                            newDTXState,
                            dec_obj_ptr,
                            bfi,
                            dec_scr_ptr,
                            dec_obj_temp_ptr );
    }
    p_Aq += ( M11 + 1 ); /* interpolated LPC parameters for next subframe */
  }

  /*--------------------------------------------------*
   * Update signal for next frame.                    *
   * -> save past of exc[].                           *
   * -> save pitch parameters.                        *
   *--------------------------------------------------*/

  WBAMR_Copy ( &wbamr_dec_main_scr_ptr->u2.old_exc[L_FRAME],
               dec_obj_ptr->old_exc,
               PIT_MAX + L_INTERPOL );

  WBAMR_Scale_sig ( exc, L_FRAME, Sub_fr1x16 ( 0, Q_new ) );
  WBAMR_Dtx_dec_activity_update ( dec_obj_ptr,
                                  wbamr_dec_main_scr_ptr->isf,
                                  exc );

  dec_obj_ptr->dtxGlobalState = newDTXState;
  dec_obj_ptr->prev_bfi = bfi;

  return;
}



/*-----------------------------------------------------*
 * Function synthesis ()                               *
 *                                                     *
 * Synthesis of signal at 16kHz with HF extension.     *
 *                                                     *
 *-----------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Dec_synthesis (
  fract16 Aq[],              /* A ( z ) : quantized Az               */
  fract16 exc[],             /* ( i )   : excitation at 12kHz        */
  fract16 Q_new,             /* ( i )   : scaling performed on exc   */
  fract16 synth16k[],        /* ( o )   : 16kHz synthesis signal     */
  fract16 prms,              /* ( i )   : parameter                  */
  fract16 HfIsf[],
  fract16 nb_bits,
  fract16 newDTXState,
  WBAMR_Static_Mem_Dec_Struct *dec_obj_ptr, /* (i/o) : State structure     */
  fract16 bfi,                              /* (i)   : bad frame indicator */
  WBAMR_Temp_Mem_Dec_Struct *dec_scr_ptr,   /* i/o   : Scratch structure   */
  fract16 *dec_obj_temp_ptr
)
{
  fract16 i;
  fract16 fac, tmp, exp;
  fract16 ener, exp_ener;
  fract32 L_tmp;

  fract16 HF_corr_gain;
  fract16 HF_gain_ind;
  fract16 gain1, gain2;
  fract16 weight1, weight2;
  wbamr_dec_decsynth *wbamr_dec_decsynth_scr_ptr;
  void *temp_alt_scr_ptr;
#if (AWB_OPT_VER == 2)
   fract32 *p_syn;
   fract32 L_tmp2, wA, wB;
#endif

   wbamr_dec_decsynth_scr_ptr = &( dec_scr_ptr->u.decsynth_scr );

  /*------------------------------------------------------------*
   * speech synthesis                                           *
   * ~~~~~~~~~~~~~~~~                                           *
   * - Find synthesis speech corresponding to exc2[].           *
   * - Perform fixed deemphasis and hp 50hz filtering.          *
   * - Oversampling from 12.8kHz to 16kHz.                      *
   *------------------------------------------------------------*/
#if (AWB_OPT_VER == 2)
  WBAMR_Copy32 ( dec_obj_ptr->mem_syn_sig,
               wbamr_dec_decsynth_scr_ptr->u2.synth_sig,
               M11 );

  WBAMR_Syn_filt_32 (  Aq,
             exc,
             Q_new,
             &wbamr_dec_decsynth_scr_ptr->u2.synth_sig[M11]);

  WBAMR_Copy32 ( wbamr_dec_decsynth_scr_ptr->u2.synth_sig + L_SUBFR,
               dec_obj_ptr->mem_syn_sig,
               M11 );

  WBAMR_Deemph_32 ( &wbamr_dec_decsynth_scr_ptr->u2.synth_sig[M11],
                    wbamr_dec_decsynth_scr_ptr->synth,
                    &( dec_obj_ptr->mem_deemph ) );
#else
  WBAMR_Copy ( dec_obj_ptr->mem_syn_hi,
               wbamr_dec_decsynth_scr_ptr->u1.synth_hi,
               M11 );
  WBAMR_Copy ( dec_obj_ptr->mem_syn_lo,
               wbamr_dec_decsynth_scr_ptr->u2.synth_lo,
               M11 );

  WBAMR_Syn_filt_32 (  Aq,
             exc,
             Q_new,
             &wbamr_dec_decsynth_scr_ptr->u1.synth_hi[M11],
             &wbamr_dec_decsynth_scr_ptr->u2.synth_lo[M11] );

  WBAMR_Copy ( wbamr_dec_decsynth_scr_ptr->u1.synth_hi + L_SUBFR,
               dec_obj_ptr->mem_syn_hi,
               M11 );
  WBAMR_Copy ( wbamr_dec_decsynth_scr_ptr->u2.synth_lo + L_SUBFR,
               dec_obj_ptr->mem_syn_lo,
               M11 );

  WBAMR_Deemph_32 ( &wbamr_dec_decsynth_scr_ptr->u1.synth_hi[M11],
                    &wbamr_dec_decsynth_scr_ptr->u2.synth_lo[M11],
                    wbamr_dec_decsynth_scr_ptr->synth,
                    &( dec_obj_ptr->mem_deemph ) );
#endif

  WBAMR_HP50_12k8 ( wbamr_dec_decsynth_scr_ptr->synth,
                    L_SUBFR,
                    dec_obj_ptr->mem_sig_out );

  WBAMR_Oversamp_16k ( wbamr_dec_decsynth_scr_ptr->synth,
                       L_SUBFR,
                       synth16k,
                       dec_obj_ptr->mem_oversamp,
                       dec_scr_ptr );

  /*------------------------------------------------------*
  * HF noise synthesis                                    *
  * ~~~~~~~~~~~~~~~~~~                                    *
  * - Generate HF noise between 5.5 and 7.5 kHz.          *
  * - Set energy of noise according to synthesis tilt.    *
  *   tilt > 0.8 ==> - 14 dB ( voiced )                   *
  *   tilt   0.5 ==> - 6 dB  ( voiced or noise )          *
  *   tilt < 0.0 ==>   0 dB  ( noise )                    *
  *------------------------------------------------------*/

  /* generate white noise vector */
  for ( i = 0; i < L_SUBFR16k; i++ )
  {
    wbamr_dec_decsynth_scr_ptr->u1.HF[i] =
                  Shr_fr1x16 ( WBAMR_Random ( &( dec_obj_ptr->seed2 ) ), 3 );
  }
  /* energy of excitation */

  WBAMR_Scale_sig ( exc, L_SUBFR, -3 );
  Q_new = Sub_fr1x16 ( Q_new, 3 );

  ener = Extract_hi_fr1x32 ( WBAMR_Dot_product12 ( exc,
                                                   exc,
                                                   L_SUBFR,
                                                   &exp_ener ) );
  exp_ener = Sub_fr1x16 ( exp_ener, Add_fr1x16 ( Q_new, Q_new ) );

  /* set energy of white noise to energy of excitation */
  tmp = Extract_hi_fr1x32 ( WBAMR_Dot_product12 (
                                wbamr_dec_decsynth_scr_ptr->u1.HF,
                                wbamr_dec_decsynth_scr_ptr->u1.HF,
                                L_SUBFR16k,
                                &exp ) );

  if ( Sub_fr1x16 ( tmp, ener ) > 0 )
  {
    tmp = Shr_fr1x16 ( tmp, 1 ); /* Be sure tmp < ener */
    exp = Add_fr1x16 ( exp, 1 );
  }
  L_tmp = WBAMR_L_deposit_h ( Divs_fr1x16 ( tmp, ener ) );
                         /* result is normalized */
  exp = Sub_fr1x16 ( exp, exp_ener );
  WBAMR_Isqrt_n ( &L_tmp, &exp );
  L_tmp = Shl_fr1x32 ( L_tmp, Add_fr1x16 ( exp, 1 ) );
                         /* L_tmp x 2, L_tmp in Q31 */
  tmp = Extract_hi_fr1x32 ( L_tmp );
                         /* tmp = 2 x sqrt ( ener_exc/ener_hf ) */
  for ( i = 0; i < L_SUBFR16k; i++ )
  {
    wbamr_dec_decsynth_scr_ptr->u1.HF[i] =
                Mult_fr1x16 ( wbamr_dec_decsynth_scr_ptr->u1.HF[i], tmp );
  }
  /* find tilt of synthesis speech ( tilt: 1=voiced, -1=unvoiced ) */

  WBAMR_HP400_12k8 ( wbamr_dec_decsynth_scr_ptr->synth,
                     dec_obj_ptr->mem_hp400 );
#if (AWB_OPT_VER == 2)
   p_syn = (fract32 *)wbamr_dec_decsynth_scr_ptr->synth;
   L_tmp = 1L;
   L_tmp2 = 1L;
   wA = *p_syn++;
   L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(wA, wA));
   L_tmp = AWB_qdadd(L_tmp, AWB_smultt(wA, wA));
   L_tmp2 = AWB_qdadd(L_tmp2, AWB_smulbt(wA, wA));
   i = 15;
   do {
      wB = *p_syn++;
      L_tmp2 = AWB_qdadd(L_tmp2, AWB_smultb(wA, wB));
      L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(wB, wB));
      L_tmp = AWB_qdadd(L_tmp, AWB_smultt(wB, wB));
      L_tmp2 = AWB_qdadd(L_tmp2, AWB_smulbt(wB, wB));
      wA = *p_syn++;
      L_tmp2 = AWB_qdadd(L_tmp2, AWB_smultb(wB, wA));
      L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(wA, wA));
      L_tmp = AWB_qdadd(L_tmp, AWB_smultt(wA, wA));
      L_tmp2 = AWB_qdadd(L_tmp2, AWB_smulbt(wA, wA));

   } while (--i != 0);
   wB = *p_syn++;
   L_tmp2 = AWB_qdadd(L_tmp2, AWB_smultb(wA, wB));
   L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(wB, wB));
   L_tmp = AWB_qdadd(L_tmp, AWB_smultt(wB, wB));
   L_tmp2 = AWB_qdadd(L_tmp2, AWB_smulbt(wB, wB));

   exp = Norm_fr1x32 ( L_tmp );
   ener = (fract16)((L_tmp << exp) >> 16);   /* ener = r[0] */
   tmp = (fract16)((L_tmp2 << exp) >> 16);
#else

  L_tmp = 1L;
  for ( i = 0; i < L_SUBFR; i++ )
    L_tmp = Mac_fr16_32 ( L_tmp,
                          wbamr_dec_decsynth_scr_ptr->synth[i],
                          wbamr_dec_decsynth_scr_ptr->synth[i] );

  exp = Norm_fr1x32 ( L_tmp );
  ener = Extract_hi_fr1x32 ( Shl_fr1x32 ( L_tmp, exp ) );   /* ener = r[0] */

  L_tmp = 1L;
  for ( i = 1; i < L_SUBFR; i++ )
    L_tmp = Mac_fr16_32 ( L_tmp,
                          wbamr_dec_decsynth_scr_ptr->synth[i],
                          wbamr_dec_decsynth_scr_ptr->synth[i - 1] );

  tmp = Extract_hi_fr1x32 ( Shl_fr1x32 ( L_tmp, exp ) );  /* tmp = r[1] */

#endif

  if ( tmp > 0 )
  {
    fac = Divs_fr1x16 ( tmp, ener );
  }
  else
  {
    fac = 0;
  }

  /* modify energy of white noise according to synthesis tilt */
  gain1 = Sub_fr1x16 ( 32767, fac );
  gain2 = Mult_fr1x16 ( Sub_fr1x16 ( 32767, fac ), 20480 );
  gain2 = Shl_fr1x16 ( gain2, 1 );

  if ( dec_obj_ptr->vad_hist > 0 )
  {
    weight1 = 0;
    weight2 = 32767;
  }
  else
  {
    weight1 = 32767;
    weight2 = 0;
  }
  tmp = Mult_fr1x16 ( weight1, gain1 );
  tmp = Add_fr1x16 ( tmp, Mult_fr1x16 ( weight2, gain2 ) );

  if ( tmp != 0 )
  {
    tmp = Add_fr1x16 ( tmp, 1 );
  }

  if ( Sub_fr1x16 ( tmp, 3277 ) < 0 )
  {
    tmp = 3277;            /* 0.1 in Q15 */

  }

  if (  ( Sub_fr1x16 ( nb_bits, NBBITS_24k ) >= 0  ) && ( bfi == 0 ) )
  {
    /* HF correction gain */
    HF_gain_ind = prms;
    HF_corr_gain = HP_gain[HF_gain_ind];

    /* HF gain */
    for ( i = 0; i < L_SUBFR16k; i++ )
    {
      wbamr_dec_decsynth_scr_ptr->u1.HF[i] =
              Shl_fr1x16 ( Mult_fr1x16 ( wbamr_dec_decsynth_scr_ptr->u1.HF[i],
                                         HF_corr_gain ),
                           1 );
    }
  }
  else
  {
    for ( i = 0; i < L_SUBFR16k; i++ )
    {
      wbamr_dec_decsynth_scr_ptr->u1.HF[i] =
                  Mult_fr1x16 ( wbamr_dec_decsynth_scr_ptr->u1.HF[i], tmp );
    }
  }

  if ( ( Sub_fr1x16 ( nb_bits, NBBITS_7k ) <= 0 ) &&
       ( Sub_fr1x16 ( newDTXState, SPEECH ) == 0 ) )
  {
    WBAMR_Isf_Extrapolation ( HfIsf );
    WBAMR_Isp_Az ( HfIsf,
                   wbamr_dec_decsynth_scr_ptr->u2.st1.HfA,
                   M16k,
                   &dec_scr_ptr->comm_scr );

    WBAMR_Weight_a ( wbamr_dec_decsynth_scr_ptr->u2.st1.HfA,
                     wbamr_dec_decsynth_scr_ptr->u2.st1.Ap,
                     29491,
                     M16k );  /* fac=0.9 */


      temp_alt_scr_ptr=wbamr_dec_decsynth_scr_ptr->u2.st1.Ap;


    WBAMR_Syn_filt ( /* wbamr_dec_decsynth_scr_ptr->u2.st1.Ap, */
                     temp_alt_scr_ptr,
                     M16k,
                     wbamr_dec_decsynth_scr_ptr->u1.HF,
                     wbamr_dec_decsynth_scr_ptr->u1.HF,
                     L_SUBFR16k,
                     dec_obj_ptr->mem_syn_hf,
                     1,
                     &dec_scr_ptr->comm_scr );
  }
  else
  {
    /* synthesis of noise: 4.8kHz..5.6kHz --> 6kHz..7kHz */
    WBAMR_Weight_a ( Aq, wbamr_dec_decsynth_scr_ptr->u2.st1.Ap, 19661, M11 );
                                                                /* fac=0.6 */

      temp_alt_scr_ptr=wbamr_dec_decsynth_scr_ptr->u2.st1.Ap;



    WBAMR_Syn_filt ( /* wbamr_dec_decsynth_scr_ptr->u2.st1.Ap,*/
                     temp_alt_scr_ptr,
                     M11,
                     wbamr_dec_decsynth_scr_ptr->u1.HF,
                     wbamr_dec_decsynth_scr_ptr->u1.HF,
                     L_SUBFR16k,
                     dec_obj_ptr->mem_syn_hf + ( M16k - M11 ),
                     1,
                     &dec_scr_ptr->comm_scr );
  }

  /* noise High Pass filtering ( 1ms of delay ) */
  WBAMR_Filt_6k_7k ( wbamr_dec_decsynth_scr_ptr->u1.HF,
                     dec_obj_ptr->mem_hf,
                     &dec_scr_ptr->comm_scr );

  if ( Sub_fr1x16 ( nb_bits, NBBITS_24k ) >= 0 )
  {
    /* Low Pass filtering ( 7 kHz ) */
    WBAMR_Filt_7k ( wbamr_dec_decsynth_scr_ptr->u1.HF,
                    dec_obj_ptr->mem_hf3,
                    wbamr_dec_decsynth_scr_ptr->u2.x );
  }
  /* Add_fr1x16 filtered HF noise to speech synthesis */
  for ( i = 0; i < L_SUBFR16k; i++ )
  {
    synth16k[i] = Add_fr1x16 ( synth16k[i],
                               wbamr_dec_decsynth_scr_ptr->u1.HF[i] );
  }

  return;
}

#endif

