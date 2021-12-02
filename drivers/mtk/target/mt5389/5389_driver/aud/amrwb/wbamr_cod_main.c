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
 *   WBAMR_COD_MAIN.C
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
#include "wbamr_math_op.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_acelp.h"
#include "wbamr_private.h"
#include "wbamr_bits.h"
#include "wbamr_dtx.h"
#include "wbamr_wb_vad.h"
#include "wbamr_cod_main.h"
#include "wbamr_sid_sync.h"
#include "wbamr_sections.h"
#include "SCShared_Lib.h"
#include "WBAMR_Lib.h"
#include "awb_exp.h"

#ifdef __ANDROID__
    #include "drvb_api.h"
#else
    #include "audip_exp.h"
#endif

awb_armdsp_flagdata_union armdsp_flagdata_AWB;

void AWBEnc_GetBufferSize(
   unsigned int *int_buf_size,
   unsigned int *tmp_buf_size,
   unsigned int *bs_buf_size,
   unsigned int *pcm_buf_size
)
{
   *int_buf_size = ((sizeof(WBAMR_Static_Mem_Enc_Struct) + 3 ) & (~3)) +
                   ((sizeof(struct WBAMR_IO_Enc_Struct) + 3) & (~3));
   *tmp_buf_size = (sizeof(WBAMR_Temp_Mem_Enc_Struct) + 3 ) & (~3);
   *bs_buf_size = ((((NB_BITS_MAX + 7) / 8) + 1) + 3) & (~3);
   *pcm_buf_size = (L_FRAME16k * sizeof(short) + 3) & (~3);
}
AWB_ENC_HANDLE *AWBEnc_Init(
   void *pInterBuf,
   AWB_BITRATE bitRate,
   int dtxEnable
)
{
   struct WBAMR_IO_Enc_Struct *encStruct;
   WBAMR_Static_Mem_Enc_Struct *wbamr_enc_obj_ptr;
   int hdlSize;
   if((pInterBuf == NULL) || ((int)pInterBuf & 3) != 0) {
      return NULL;
   }
   encStruct = (struct WBAMR_IO_Enc_Struct *)pInterBuf;
   hdlSize = (sizeof( struct WBAMR_IO_Enc_Struct) + 3) & (~3);
   wbamr_enc_obj_ptr = (WBAMR_Static_Mem_Enc_Struct *)((uint8 *)pInterBuf + hdlSize);
   encStruct->dtx_mode     = dtxEnable;
   encStruct->mode         = bitRate;
   encStruct->usedMode     = 0;
   encStruct->nsync_mode   = 0;          /* default is NSYNC off */
   encStruct->vad_callback = NULL;       /* no VAD callback function */
   WBAMR_Enc_Init(wbamr_enc_obj_ptr);
   return (AWB_ENC_HANDLE *)pInterBuf;
}
int AWB_Encode(
   AWB_ENC_HANDLE *awbEncHandle,
   void *pTmpBuf,
   short *pPcmBuf,
   unsigned char *pBsBuf,
   AWB_BITRATE bitRate
)
{
   struct WBAMR_IO_Enc_Struct *enc_io_ptr;
   WBAMR_Static_Mem_Enc_Struct *wbamr_enc_obj_ptr;
   int i, hdlSize;

#ifdef __ANDROID__
    CHECK_MTK_HW(enc_io_ptr)
#else
    AUDIP_DRVSET();
    AUDIP_COMMON();
#endif

   enc_io_ptr = (struct WBAMR_IO_Enc_Struct *)awbEncHandle;
   hdlSize = (sizeof(struct WBAMR_IO_Enc_Struct) + 3) & (~3);
   wbamr_enc_obj_ptr = (WBAMR_Static_Mem_Enc_Struct *)((uint8 *)awbEncHandle + hdlSize);
   enc_io_ptr->speech = (fract16 *)pPcmBuf;
   enc_io_ptr->parms  = (uint16 *)pBsBuf;
   enc_io_ptr->mode   = bitRate;
   enc_io_ptr->temp_0 = pTmpBuf;
   enc_io_ptr->temp_1 = NULL;
    for ( i = 0; i < 320; i++ )
    {
      enc_io_ptr->speech[i] = ( fract16 ) ( enc_io_ptr->speech[i] & 0xfffC );
    }
    WBAMR_AIL_Enc(enc_io_ptr, wbamr_enc_obj_ptr);
    return AWB_packed_size[enc_io_ptr->usedMode] + 1;
}


WBAMR_CODE_SECTION void WBAMR_Reset_encoder (
  WBAMR_Static_Mem_Enc_Struct *enc_obj_ptr,
  fract16 reset_all
)
{
  fract16 i;

  WBAMR_Set_zero ( enc_obj_ptr->old_exc, PIT_MAX + L_INTERPOL );
  WBAMR_Set_zero ( enc_obj_ptr->mem_syn, M11 );
  WBAMR_Set_zero ( enc_obj_ptr->past_isfq, M11 );

  enc_obj_ptr->mem_w0 = 0;
  enc_obj_ptr->tilt_code = 0;
  enc_obj_ptr->first_frame = 1;

  WBAMR_Init_gp_clip ( enc_obj_ptr->gp_clip );

  enc_obj_ptr->L_gc_thres = 0;

  if ( reset_all != 0 )
  {
    /* Static vectors to zero */
    WBAMR_Set_zero ( enc_obj_ptr->old_speech, L_TOTAL - L_FRAME );
    WBAMR_Set_zero ( enc_obj_ptr->old_wsp, ( PIT_MAX / OPL_DECIM ) );
    WBAMR_Set_zero ( enc_obj_ptr->mem_decim2, 3 );

    /* routines initialization */
    WBAMR_Init_Decim_12k8 ( enc_obj_ptr->mem_decim );
    WBAMR_Init_HP50_12k8 ( enc_obj_ptr->mem_sig_in );
    WBAMR_Init_Levinson ( enc_obj_ptr->mem_levinson );
    WBAMR_Init_Q_gain2 ( enc_obj_ptr->qua_gain );
    WBAMR_Init_Hp_wsp ( enc_obj_ptr->hp_wsp_mem );

    /* isp initialization */
    WBAMR_Copy ( (fract16 *)isp_init, enc_obj_ptr->ispold, M11 );
    WBAMR_Copy ( (fract16 *)isp_init, enc_obj_ptr->ispold_q, M11 );

    /* variable initialization */
    enc_obj_ptr->mem_preemph = 0;
    enc_obj_ptr->mem_wsp = 0;
    enc_obj_ptr->Q_old = 15;
    enc_obj_ptr->Q_max[0] = 15;
    enc_obj_ptr->Q_max[1] = 15;
    enc_obj_ptr->old_wsp_max = 0;
    enc_obj_ptr->old_wsp_shift = 0;

    /* pitch ol initialization */
    enc_obj_ptr->old_T0_med = 40;
    enc_obj_ptr->ol_gain = 0;
    enc_obj_ptr->ada_w = 0;
    enc_obj_ptr->ol_wght_flg = 0;
    for ( i = 0; i < 5; i++ )
    {
      enc_obj_ptr->old_ol_lag[i] = 40;
    }
    WBAMR_Set_zero ( enc_obj_ptr->old_hp_wsp,
                     ( L_FRAME / 2 ) / OPL_DECIM + ( PIT_MAX / OPL_DECIM ) );

    WBAMR_Set_zero ( enc_obj_ptr->mem_syn_hf, M11 );
#if (AWB_OPT_VER == 2)
    for(i = 0; i < M11; i++) {
       enc_obj_ptr->mem_syn_sig[i] = 0;
    }
#else
    WBAMR_Set_zero ( enc_obj_ptr->mem_syn_hi, M11 );
    WBAMR_Set_zero ( enc_obj_ptr->mem_syn_lo, M11 );
#endif

    WBAMR_Init_HP50_12k8 ( enc_obj_ptr->mem_sig_out );
    WBAMR_Init_Filt_6k_7k ( enc_obj_ptr->mem_hf );
    WBAMR_Init_HP400_12k8 ( enc_obj_ptr->mem_hp400 );

    WBAMR_Copy ( (fract16 *)isf_init, enc_obj_ptr->isfold, M11 );

    enc_obj_ptr->mem_deemph = 0;

    enc_obj_ptr->seed2 = 21845;

    WBAMR_Init_Filt_6k_7k ( enc_obj_ptr->mem_hf2 );
    enc_obj_ptr->gain_alpha = 32767;

    enc_obj_ptr->vad_hist = 0;

    WBAMR_Wb_vad_reset ( enc_obj_ptr );
    WBAMR_Dtx_enc_reset ( enc_obj_ptr, isf_init );
  }

  return;
}


/*-----------------------------------------------------------------*
 *   Funtion  WBAMR_Encoder                                        *
 *            ~~~~~~~~~~~~~                                        *
 *   ->Main coder routine.                                         *
 *                                                                 *
 *-----------------------------------------------------------------*/
WBAMR_CODE_SECTION void WBAMR_Encoder (
  fract16  mode,                      /* i :  requested mode */
  fract16* usedMode,                  /* o :  used mode */
  fract16  speech16k[],               /* i :  320 new  samples */
  uint16   prms[],                    /* o :  output parameters */
  fract16  allow_dtx,                 /* i :  DTX ON/OFF */
  fract16* tx_type,                   /* o :  frame type */
  uint8    nsync,                     /* i :  TX/RX DTX handler sync mode */
  uint8    *vad,                      /* o :  vad reporting flag */
  void     *vad_callback,             /* o :  ptr for early VAD reporting */
  WBAMR_Static_Mem_Enc_Struct *enc_obj_ptr, /* i/o   :  State structure */
  WBAMR_Temp_Mem_Enc_Struct  *enc_scr_ptr,  /* i/o   :  scratch structure */
  fract16* enc_obj_temp_ptr                 /* i/o   : alt scratch pointer */
 )
{

  /* Speech vector */
  fract16 *new_speech, *speech, *p_window;
  fract16 *wsp;
  fract16 *exc;
  fract16 *p_A, *p_Aq;          /* ptr to A(z) for the 4 subframes */
  fract16 ser_size;             /* bit rate of the used mode       */

  /* Scalars */
  int_native i, j, i_subfr;
  fract16 select, pit_flag, clip_gain, vad_flag;
  fract16 T_op, T_op2, T0, T0_min, T0_max, T0_frac, index;
  fract16 gain_pit, gain_code;
  fract16 tmp, gain1, gain2, exp, exp1, Q_new, mu, shift, max;
  fract16 voice_fac;

  fract32 L_gain_code, L_max;
  fract16 stab_fac, fac, gain_code_lo;
  fract16 corr_gain;
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr;
  uint8* paramsBuf;
  void*  tmp_scr_ptr;
  void* wbamr_enc_y_buf_ptr;
  void*  tmp_scr_alt_ptr;
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
  fract16 *pU5error, *pU6h1;
  register fract32 L_tmp;
#else
  fract32 L_tmp;
#endif

#if (AWB_OPT_VER == 2)
   fract32 vfac, gain_c, gain_p, now_code, prev_code1, prev_code2;
   fract32 *pcode;
   fract16 *pexc;
#endif


  wbamr_enc_cod_main_scr_ptr = & ( enc_scr_ptr->cod_main_scr );
  tmp_scr_ptr = & ( enc_scr_ptr->u2 );

  paramsBuf = wbamr_enc_cod_main_scr_ptr->paramsBuf;
  ser_size = nb_of_bits[mode];
  *usedMode = mode;
  /*-----------------------------------------------------------------------*
   *       Initialize pointers to speech vector.                           *
   *                                                                       *
   *                                                                       *
   *                 |-------|-------|-------|-------|-------|-------|     *
   *                  past sp   sf1     sf2     sf3     sf4    L_NEXT      *
   *                 <-------  Total speech buffer ( L_TOTAL )   ------>   *
   *           old_speech                                                  *
   *                 <-------  LPC analysis window ( L_WINDOW )  ------>   *
   *                 |       <-- present frame ( L_FRAME ) ---->           *
   *                p_window |       <----- new speech ( L_FRAME ) ---->   *
   *                         |       |                                     *
   *                       speech    |                                     *
   *                              new_speech                               *
   *-----------------------------------------------------------------------*/

  new_speech = wbamr_enc_cod_main_scr_ptr->old_speech +
               ( L_TOTAL - L_FRAME - L_FILT ); /* New speech     */
  speech = wbamr_enc_cod_main_scr_ptr->old_speech +
           ( L_TOTAL - L_FRAME - L_NEXT );     /* Present frame  */
  p_window = wbamr_enc_cod_main_scr_ptr->old_speech + ( L_TOTAL - L_WINDOW );

  exc = wbamr_enc_cod_main_scr_ptr->u8.old_exc + PIT_MAX + L_INTERPOL;
  wsp = wbamr_enc_cod_main_scr_ptr->u8.old_wsp + ( PIT_MAX / OPL_DECIM );

  /* copy coder memory state into working space ( internal memory for DSP ) */
  WBAMR_Copy ( enc_obj_ptr->old_speech,
               wbamr_enc_cod_main_scr_ptr->old_speech,
               L_TOTAL - L_FRAME );
  WBAMR_Copy ( enc_obj_ptr->old_wsp,
               wbamr_enc_cod_main_scr_ptr->u8.old_wsp,
               PIT_MAX / OPL_DECIM );

  /*---------------------------------------------------------------*
   * Down sampling signal from 16kHz to 12.8kHz                    *
   * -> The signal is extended by L_FILT samples ( padded to zero )*
   * to avoid additional delay ( L_FILT samples ) in the coder.    *
   * The last L_FILT samples are approximated after decimation and *
   * are used ( and windowed ) only in autocorrelations.           *
   *---------------------------------------------------------------*/
  WBAMR_Decim_12k8 ( speech16k,
                     L_FRAME16k,
                     new_speech,
                     enc_obj_ptr->mem_decim,
                     enc_scr_ptr );

  /* last L_FILT samples for autocorrelation window */
  WBAMR_Copy ( enc_obj_ptr->mem_decim,
               wbamr_enc_cod_main_scr_ptr->code,
               2 * L_FILT16k );
   /* set next sample to zero */
  WBAMR_Set_zero ( wbamr_enc_cod_main_scr_ptr->u5.error, L_FILT16k );
  WBAMR_Decim_12k8 ( wbamr_enc_cod_main_scr_ptr->u5.error,
                     L_FILT16k,
                     new_speech + L_FRAME,
                     wbamr_enc_cod_main_scr_ptr->code,
                     enc_scr_ptr );

  /*---------------------------------------------------------------*
   * Perform 50Hz HP filtering of input signal.                    *
   *---------------------------------------------------------------*/
  WBAMR_HP50_12k8 ( new_speech, L_FRAME, enc_obj_ptr->mem_sig_in );

  /* last L_FILT samples for autocorrelation window */
  WBAMR_Copy ( enc_obj_ptr->mem_sig_in, wbamr_enc_cod_main_scr_ptr->code, 6 );
  WBAMR_HP50_12k8 ( new_speech + L_FRAME,
                    L_FILT,
                    wbamr_enc_cod_main_scr_ptr->code );

  /*---------------------------------------------------------------*
   * Perform fixed preemphasis through 1 - g z^-1                  *
   * Scale signal to get maximum of precision in filtering         *
   *---------------------------------------------------------------*/


  mu = Shr_fr1x16 ( PREEMPH_FAC, 1 );  /* Q15 --> Q14 */
  /* get max of new preemphased samples ( L_FRAME+L_FILT ) */
  L_tmp = Mult_fr1x32 ( new_speech[0], 16384 );
  L_tmp = WBAMR_L_msu ( L_tmp, enc_obj_ptr->mem_preemph, mu );
  L_max = Abs_fr1x32 ( L_tmp );
  for ( i = 1; i < L_FRAME + L_FILT; i++ )
  {
    L_tmp = Mult_fr1x32 ( new_speech[i], 16384 );
    L_tmp = WBAMR_L_msu ( L_tmp, new_speech[i - 1], mu );
    L_tmp = Abs_fr1x32 ( L_tmp );

    if ( Sub_fr1x32 ( L_tmp, L_max ) > ( fract32 ) 0 )
    {
      L_max = L_tmp;
    }
  }

  /* get scaling factor for new and previous samples */
  /* limit scaling to Q_MAX to keep dynamic for ringing in low signal */
  /* limit scaling to Q_MAX also to avoid a[0]<1 in syn_filt_32 */
  tmp = Extract_hi_fr1x32 ( L_max );

  if ( tmp == 0 )
  {
    shift = Q_MAX;
  }
  else
  {
    shift = Sub_fr1x16 ( WBAMR_Norm_s ( tmp ), 1 );

    if ( shift < 0 )
    {
      shift = 0;
    }

    if ( Sub_fr1x16 ( shift, Q_MAX ) > 0 )
    {
      shift = Q_MAX;
    }
  }
  Q_new = shift;

  if ( Sub_fr1x16 ( Q_new, enc_obj_ptr->Q_max[0] ) > 0 )
  {
    Q_new = enc_obj_ptr->Q_max[0];
  }

  if ( Sub_fr1x16 ( Q_new, enc_obj_ptr->Q_max[1] ) > 0 )
  {
    Q_new = enc_obj_ptr->Q_max[1];
  }
  exp = Sub_fr1x16 ( Q_new, enc_obj_ptr->Q_old );
  enc_obj_ptr->Q_old = Q_new;
  enc_obj_ptr->Q_max[1] = enc_obj_ptr->Q_max[0];
  enc_obj_ptr->Q_max[0] = shift;

  /* preemphasis with scaling ( L_FRAME+L_FILT ) */
  tmp = new_speech[L_FRAME - 1];

  for ( i = L_FRAME + L_FILT - 1; i > 0; i-- )
  {
    L_tmp = Mult_fr1x32 ( new_speech[i], 16384 );
    L_tmp = WBAMR_L_msu ( L_tmp, new_speech[i - 1], mu );
    L_tmp = Shl_fr1x32 ( L_tmp, Q_new );
    new_speech[i] = Round_fr1x32 ( L_tmp );
  }
  L_tmp = Mult_fr1x32 ( new_speech[0], 16384 );
  L_tmp = WBAMR_L_msu ( L_tmp, enc_obj_ptr->mem_preemph, mu );
  L_tmp = Shl_fr1x32 ( L_tmp, Q_new );
  new_speech[0] = Round_fr1x32 ( L_tmp );

  enc_obj_ptr->mem_preemph = tmp;

  /* scale previous samples and memory */
  WBAMR_Scale_sig ( wbamr_enc_cod_main_scr_ptr->old_speech,
                    L_TOTAL - L_FRAME - L_FILT,
                    exp );
  exp1 = exp;

  WBAMR_Scale_sig ( enc_obj_ptr->mem_syn, M11, exp );
  WBAMR_Scale_sig ( enc_obj_ptr->mem_decim2, 3, exp );
  WBAMR_Scale_sig ( & ( enc_obj_ptr->mem_wsp ), 1, exp );
  WBAMR_Scale_sig ( & ( enc_obj_ptr->mem_w0 ), 1, exp );

  /*------------------------------------------------------------------------*
   *  Call VAD                                                              *
   *  Preemphesis scale down signal in low frequency and keep dynamic in HF.*
   *  Vad work slightly in futur ( new_speech = speech + L_NEXT - L_FILT ). *
   *------------------------------------------------------------------------*/
  WBAMR_Copy ( new_speech, wbamr_enc_cod_main_scr_ptr->u7.buf, L_FRAME );

  WBAMR_Scale_sig ( wbamr_enc_cod_main_scr_ptr->u7.buf,
                    L_FRAME,
                    Sub_fr1x16 ( 1, Q_new ) );

  vad_flag = WBAMR_Wb_vad ( enc_obj_ptr,
                            wbamr_enc_cod_main_scr_ptr->u7.buf,
                            enc_scr_ptr );

  if ( vad_flag == 0 )
  {
    enc_obj_ptr->vad_hist = Add_fr1x16 ( enc_obj_ptr->vad_hist, 1 );
  }
  else
  {
    enc_obj_ptr->vad_hist = 0;
  }

  /* DTX processing */
  if ( allow_dtx != 0 )
  {
    /* Note that mode may change here */
    WBAMR_Tx_dtx_handler ( enc_obj_ptr, vad_flag, usedMode, nsync );
    ser_size = nb_of_bits[*usedMode];

    /*
     * VAD and usedMode are already set to their final values here, so
     * use VAD callback function to report VAD and usedMode as early
     * as possible, if the caller wants them early
     */
    if (  vad_callback != NULL  )
    {
      ( *( ( void (*) (fract16, fract16) ) vad_callback ) ) ( vad_flag, *usedMode );
    }

    /* save vad_flag for eventual return to caller */
    *vad = (uint8)vad_flag;
  }

  if ( Sub_fr1x16 ( *usedMode, MRDTX ) != 0 )
  {
    WBAMR_Parm_serial ( vad_flag, 1, &paramsBuf );
  }
  /*------------------------------------------------------------------------*
   *  Perform LPC analysis                                                  *
   *  ~~~~~~~~~~~~~~~~~~~~                                                  *
   *   - autocorrelation + lag windowing                                    *
   *   - Levinson-durbin algorithm to find a[]                              *
   *   - convert a[] to isp[]                                               *
   *   - convert isp[] to isf[] for quantization                            *
   *   - quantize and code the isf[]                                        *
   *   - convert isf[] to isp[] for interpolation                           *
   *   - find the interpolated ISPs and convert to a[] for the 4 subframes  *
   *------------------------------------------------------------------------*/

/* LP analysis centered at 4nd subframe */
  wbamr_enc_y_buf_ptr = ( fract16* )enc_scr_ptr->u1.y;

  WBAMR_Autocorr ( p_window,
                   wbamr_enc_cod_main_scr_ptr->r_h,
                   wbamr_enc_cod_main_scr_ptr->r_l,
                   wbamr_enc_y_buf_ptr,
                   enc_obj_temp_ptr );   /* Autocorrelations */

  /* Lag windowing    */
  WBAMR_Lag_window ( wbamr_enc_cod_main_scr_ptr->r_h,
                     wbamr_enc_cod_main_scr_ptr->r_l );
  /* Levinson Durbin  */
  WBAMR_Levinson ( wbamr_enc_cod_main_scr_ptr->r_h,
                   wbamr_enc_cod_main_scr_ptr->r_l,
                   wbamr_enc_cod_main_scr_ptr->A,
                   wbamr_enc_cod_main_scr_ptr->u4.rc,
                   tmp_scr_ptr,
                   enc_obj_ptr->mem_levinson );
  /* From A ( z ) to ISP */
  WBAMR_Az_isp ( enc_obj_ptr->ispold,wbamr_enc_cod_main_scr_ptr );
  /* Find the interpolated ISPs and convert to a[] for all subframes */
  WBAMR_Int_isp ( enc_obj_ptr->ispold,
                  wbamr_enc_cod_main_scr_ptr->ispnew,
                  interpol_frac,
                  wbamr_enc_cod_main_scr_ptr->A,
                  tmp_scr_ptr );

  /* update ispold[] for the next frame */
  WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->ispnew, enc_obj_ptr->ispold, M11 );

  /* Convert ISPs to frequency domain 0..6400 */
  WBAMR_Isp_isf ( wbamr_enc_cod_main_scr_ptr->ispnew,
                  wbamr_enc_cod_main_scr_ptr->isf );

  /* check resonance for pitch clipping algorithm */
  WBAMR_Gp_clip_test_isf ( wbamr_enc_cod_main_scr_ptr->isf,
                           enc_obj_ptr->gp_clip );

  /*----------------------------------------------------------------------*
   *  Perform PITCH_OL analysis                                           *
   *  ~~~~~~~~~~~~~~~~~~~~~~~~~                                           *
   * - Find the residual res[] for the whole speech frame                 *
   * - Find the weighted input speech wsp[] for the whole speech frame    *
   * - scale wsp[] to avoid overflow in pitch estimation                  *
   * - Find open loop pitch lag for whole speech frame                    *
   *----------------------------------------------------------------------*/

  p_A = wbamr_enc_cod_main_scr_ptr->A;
  for ( i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR )
  {
    WBAMR_Weight_a ( p_A, wbamr_enc_cod_main_scr_ptr->u4.Ap, GAMMA1, M11 );
    WBAMR_Residu ( wbamr_enc_cod_main_scr_ptr->u4.Ap,
                   &speech[i_subfr],
                   &wsp[i_subfr],
                   L_SUBFR,
                   enc_obj_temp_ptr );

    p_A += ( M11 + 1 );
  }
  WBAMR_Deemph2 ( wsp,  L_FRAME, & ( enc_obj_ptr->mem_wsp ) );

  /* find maximum value on wsp[] for 12 bits scaling */
  max = 0;
  for ( i = 0; i < L_FRAME; i++ )
  {
    tmp = Abs_fr1x16 ( wsp[i] );

    if ( Sub_fr1x16 ( tmp, max ) > 0 )
    {
      max = tmp;
    }
  }
  tmp = enc_obj_ptr->old_wsp_max;

  if ( Sub_fr1x16 ( max, tmp ) > 0 )
  {
    tmp = max;               /* tmp = max ( wsp_max, old_wsp_max ) */
  }
  enc_obj_ptr->old_wsp_max = max;

  shift = Sub_fr1x16 ( WBAMR_Norm_s ( tmp ), 3 );

  if ( shift > 0 )
  {
    shift = 0;               /* shift = 0..-3 */
  }
  /* decimation of wsp[] to search pitch in LF and to reduce complexity */
  WBAMR_LP_Decim2 ( wsp, enc_obj_ptr->mem_decim2, enc_scr_ptr );

  /* scale wsp[] in 12 bits to avoid overflow */
  WBAMR_Scale_sig ( wsp, L_FRAME / OPL_DECIM, shift );

  /* scale old_wsp ( warning: exp must be Q_new-Q_old ) */
  exp = Add_fr1x16 ( exp, Sub_fr1x16 ( shift, enc_obj_ptr->old_wsp_shift ) );
  enc_obj_ptr->old_wsp_shift = shift;
  WBAMR_Scale_sig ( wbamr_enc_cod_main_scr_ptr->u8.old_wsp,
                    PIT_MAX / OPL_DECIM,
                    exp );
  WBAMR_Scale_sig ( enc_obj_ptr->old_hp_wsp, PIT_MAX / OPL_DECIM, exp );
  WBAMR_Scale_mem_Hp_wsp ( enc_obj_ptr->hp_wsp_mem, exp );

  /* Find open loop pitch lag for whole speech frame */
  if ( Sub_fr1x16 ( ser_size, NBBITS_7k ) == 0 )
  {
    /* Find open loop pitch lag for whole speech frame */
    T_op = WBAMR_Pitch_med_ol ( wsp,
                                PIT_MIN / OPL_DECIM, PIT_MAX / OPL_DECIM,
                                L_FRAME / OPL_DECIM, enc_obj_ptr->old_T0_med,
                                & ( enc_obj_ptr->ol_gain ),
                                enc_obj_ptr->hp_wsp_mem,
                                enc_obj_ptr->old_hp_wsp,
                                enc_obj_ptr->ol_wght_flg,
                                enc_obj_temp_ptr );
  }
  else
  {
    /* Find open loop pitch lag for first 1/2 frame */
    T_op = WBAMR_Pitch_med_ol ( wsp,
                                PIT_MIN / OPL_DECIM, PIT_MAX / OPL_DECIM,
                                ( L_FRAME / 2 ) / OPL_DECIM,
                                enc_obj_ptr->old_T0_med,
                                & ( enc_obj_ptr->ol_gain ),
                                enc_obj_ptr->hp_wsp_mem,
                                enc_obj_ptr->old_hp_wsp,
                                enc_obj_ptr->ol_wght_flg,
                                enc_obj_temp_ptr );
  }

  if ( Sub_fr1x16 ( enc_obj_ptr->ol_gain, 19661 ) > 0 ) /* 0.6 in Q15 */
  {
    enc_obj_ptr->old_T0_med = WBAMR_Med_olag ( T_op, enc_obj_ptr->old_ol_lag );
    enc_obj_ptr->ada_w = 32767;
  }
  else
  {
    enc_obj_ptr->ada_w = Mult_fr1x16 ( enc_obj_ptr->ada_w, 29491 );
  }

  if ( Sub_fr1x16 ( enc_obj_ptr->ada_w, 26214 ) < 0 )
    enc_obj_ptr->ol_wght_flg = 0;
  else
    enc_obj_ptr->ol_wght_flg = 1;

  WBAMR_Wb_vad_tone_detection ( enc_obj_ptr, enc_obj_ptr->ol_gain );

  T_op *= OPL_DECIM;

  if ( Sub_fr1x16 ( ser_size, NBBITS_7k ) != 0 )
  {
    /* Find open loop pitch lag for second 1/2 frame */
    T_op2 = WBAMR_Pitch_med_ol ( wsp + (  ( L_FRAME / 2 ) / OPL_DECIM ),
                                 PIT_MIN / OPL_DECIM, PIT_MAX / OPL_DECIM,
                                 ( L_FRAME / 2 ) / OPL_DECIM,
                                 enc_obj_ptr->old_T0_med,
                                 & ( enc_obj_ptr->ol_gain ),
                                 enc_obj_ptr->hp_wsp_mem,
                                 enc_obj_ptr->old_hp_wsp,
                                 enc_obj_ptr->ol_wght_flg,
                                 enc_obj_temp_ptr );

    if ( Sub_fr1x16 ( enc_obj_ptr->ol_gain, 19661 ) > 0 )   /* 0.6 in Q15 */
    {
      enc_obj_ptr->old_T0_med = WBAMR_Med_olag ( T_op2,
                                                 enc_obj_ptr->old_ol_lag );
      enc_obj_ptr->ada_w = 32767;
    }
    else
    {
      enc_obj_ptr->ada_w = Mult_fr1x16 ( enc_obj_ptr->ada_w, 29491 );
    }

    if ( Sub_fr1x16 ( enc_obj_ptr->ada_w, 26214 ) < 0 )
      enc_obj_ptr->ol_wght_flg = 0;
    else
      enc_obj_ptr->ol_wght_flg = 1;

    WBAMR_Wb_vad_tone_detection ( enc_obj_ptr, enc_obj_ptr->ol_gain );

    T_op2 *= OPL_DECIM;
  }
  else
  {
    T_op2 = T_op;
  }

  WBAMR_Copy ( &wbamr_enc_cod_main_scr_ptr->u8.old_wsp[L_FRAME / OPL_DECIM],
               enc_obj_ptr->old_wsp,
               PIT_MAX / OPL_DECIM );
  WBAMR_Copy ( enc_obj_ptr->old_exc,
               wbamr_enc_cod_main_scr_ptr->u8.old_exc,
               PIT_MAX + L_INTERPOL );
  WBAMR_Scale_sig ( wbamr_enc_cod_main_scr_ptr->u8.old_exc,
                    PIT_MAX + L_INTERPOL,
                    exp1 );

  /*
   * Updating frame type info.
   */
  WBAMR_Sid_sync ( *usedMode, tx_type, nsync, enc_obj_ptr );

  /*----------------------------------------------------------------------*
   *                              DTX-CNG                                 *
   *----------------------------------------------------------------------*/
  if ( Sub_fr1x16 ( *usedMode, MRDTX ) == 0 )  /* CNG mode */
  {
     //printf("Enter DTX\n");
    /* Buffer isf's and energy */
    WBAMR_Residu ( &wbamr_enc_cod_main_scr_ptr->A[3* ( M11+1 )],
                   speech,
                   exc,
                   L_FRAME,
                   enc_obj_temp_ptr );

    for ( i = 0; i < L_FRAME; i++ )
    {
      wbamr_enc_cod_main_scr_ptr->u7.exc2[i] = Shr_fr1x16 ( exc[i], Q_new );
    }

    L_tmp = 0;
    for ( i = 0; i < L_FRAME; i++ )
      L_tmp = Mac_fr16_32 ( L_tmp,
                            wbamr_enc_cod_main_scr_ptr->u7.exc2[i],
                            wbamr_enc_cod_main_scr_ptr->u7.exc2[i] );
    L_tmp = Shr_fr1x32 ( L_tmp, 1 );

    WBAMR_Dtx_buffer ( enc_obj_ptr,
                       wbamr_enc_cod_main_scr_ptr->isf,
                       L_tmp,
                       mode );

    /* Quantize and code the ISFs */
    WBAMR_Dtx_enc ( wbamr_enc_cod_main_scr_ptr->isf,
                    wbamr_enc_cod_main_scr_ptr->u7.exc2,
                    enc_obj_ptr,
                    tmp_scr_ptr,
                    &paramsBuf );

    /* Convert ISFs to the cosine domain */
    WBAMR_Isf_isp ( wbamr_enc_cod_main_scr_ptr->isf,
                    wbamr_enc_cod_main_scr_ptr->ispnew_q,
                    M11 );
    WBAMR_Isp_Az ( wbamr_enc_cod_main_scr_ptr->ispnew_q,
                   wbamr_enc_cod_main_scr_ptr->Aq,
                   M11,
                   tmp_scr_ptr );

    for ( i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR )
    {
      corr_gain = WBAMR_Enc_synthesis (
                         wbamr_enc_cod_main_scr_ptr->Aq,
                         &wbamr_enc_cod_main_scr_ptr->u7.exc2[i_subfr],
                         0,
                         &speech16k[i_subfr * 5 / 4],
                         enc_obj_ptr,
                         enc_scr_ptr,
                         tmp_scr_ptr,
                         enc_obj_temp_ptr );
    }
    WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->isf, enc_obj_ptr->isfold, M11 );

    /* reset speech coder memories */
    WBAMR_Reset_encoder ( enc_obj_ptr, 0 );

    /*--------------------------------------------------*
     * Update signal for next frame.                    *
     * -> save past of speech[] and wsp[].              *
     *--------------------------------------------------*/

    WBAMR_Copy ( &wbamr_enc_cod_main_scr_ptr->old_speech[L_FRAME],
                 enc_obj_ptr->old_speech,
                 L_TOTAL - L_FRAME );

    goto return_end;
  }

    /*----------------------------------------------------------------------*
     *                               ACELP                                  *
     *----------------------------------------------------------------------*/

    /* Quantize and code the ISFs */
  if ( Sub_fr1x16 ( ser_size, NBBITS_7k ) <= 0 )
  {
    WBAMR_Qpisf_2s_36b ( wbamr_enc_cod_main_scr_ptr->indice,
                         wbamr_enc_cod_main_scr_ptr->isf,
                         enc_obj_ptr->past_isfq,
                         4,
                         tmp_scr_ptr,
                         enc_obj_temp_ptr );

    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[0], 8, &paramsBuf );
    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[1], 8, &paramsBuf );
    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[2], 7, &paramsBuf );
    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[3], 7, &paramsBuf );
    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[4], 6, &paramsBuf );
  }
  else
  {
    WBAMR_Qpisf_2s_46b ( wbamr_enc_cod_main_scr_ptr->indice,
                         wbamr_enc_cod_main_scr_ptr->isf,
                         enc_obj_ptr->past_isfq,
                         4,
                         tmp_scr_ptr,
                         enc_obj_temp_ptr );

    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[0], 8, &paramsBuf );
    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[1], 8, &paramsBuf );
    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[2], 6, &paramsBuf );
    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[3], 7, &paramsBuf );
    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[4], 7, &paramsBuf );
    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[5], 5, &paramsBuf );
    WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[6], 5, &paramsBuf );
  }

  /* Check stability on isf : distance between old isf and current isf */

  L_tmp = 0;
  for ( i = 0; i < M11 - 1; i++ )
  {
    tmp = Sub_fr1x16 ( wbamr_enc_cod_main_scr_ptr->isf[i],
                       enc_obj_ptr->isfold[i] );
    L_tmp = Mac_fr16_32 ( L_tmp, tmp, tmp );
  }

  tmp = Extract_hi_fr1x32 ( Shl_fr1x32 ( L_tmp, 8 ) );
                                         /* saturation can occur here */

  tmp = Mult_fr1x16 ( tmp, 26214 );      /* tmp = L_tmp*0.8/256 */
  tmp = Sub_fr1x16 ( 20480, tmp );       /* 1.25 - tmp ( in Q14 ) */

  stab_fac = Shl_fr1x16 ( tmp, 1 );      /* saturation can occur here */

  if ( stab_fac < 0 )
  {
    stab_fac = 0;
  }
  WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->isf, enc_obj_ptr->isfold, M11 );

  /* Convert ISFs to the cosine domain */
  WBAMR_Isf_isp ( wbamr_enc_cod_main_scr_ptr->isf,
                  wbamr_enc_cod_main_scr_ptr->ispnew_q,
                  M11 );

  if ( enc_obj_ptr->first_frame != 0 )
  {
    enc_obj_ptr->first_frame = 0;
    WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->ispnew_q,
                 enc_obj_ptr->ispold_q,
                 M11 );
  }
  /* Find the interpolated ISPs and convert to a[] for all subframes */
  WBAMR_Int_isp ( enc_obj_ptr->ispold_q,
                  wbamr_enc_cod_main_scr_ptr->ispnew_q,
                  interpol_frac,
                  wbamr_enc_cod_main_scr_ptr->Aq,
                  tmp_scr_ptr );

  /* update ispold[] for the next frame */
  WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->ispnew_q,
               enc_obj_ptr->ispold_q,
               M11 );

  p_Aq = wbamr_enc_cod_main_scr_ptr->Aq;
  for ( i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR )
  {
    WBAMR_Residu ( p_Aq, &speech[i_subfr], &exc[i_subfr], L_SUBFR,
                    enc_obj_temp_ptr );

    p_Aq += ( M11 + 1 );
  }

  /* Buffer isf's and energy for dtx on non-speech frame */
  if ( vad_flag == 0 )
  {
    for ( i = 0; i < L_FRAME; i++ )
    {
      wbamr_enc_cod_main_scr_ptr->u7.exc2[i] = Shr_fr1x16 ( exc[i], Q_new );
    }
    L_tmp = 0;
    for ( i = 0; i < L_FRAME; i++ )
      L_tmp = Mac_fr16_32 ( L_tmp,
                            wbamr_enc_cod_main_scr_ptr->u7.exc2[i],
                            wbamr_enc_cod_main_scr_ptr->u7.exc2[i] );
    L_tmp = Shr_fr1x32 ( L_tmp, 1 );

    WBAMR_Dtx_buffer ( enc_obj_ptr,
                       wbamr_enc_cod_main_scr_ptr->isf,
                       L_tmp,
                       mode );
  }

  /* range for closed loop pitch search in 1st subframe */
  T0_min = Sub_fr1x16 ( T_op, 8 );

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

  /*------------------------------------------------------------------------*
   *          Loop for every subframe in the analysis frame                 *
   *------------------------------------------------------------------------*
   *  To find the pitch and innovation parameters. The subframe size is     *
   *  L_SUBFR and the loop is repeated L_FRAME/L_SUBFR times.               *
   *     - compute the target signal for pitch search                       *
   *     - compute impulse response of weighted synthesis filter ( h1[] )   *
   *     - find the closed-loop pitch parameters                            *
   *     - encode the pitch dealy                                           *
   *     - find 2 lt prediction ( with / without LP filter for lt pred )    *
   *     - find 2 pitch gains and choose the best lt prediction.            *
   *     - find target vector for codebook search                           *
   *     - update the impulse response h1[] for codebook search             *
   *     - correlation between target vector and impulse response           *
   *     - codebook search and encoding                                     *
   *     - VQ of pitch and codebook gains                                   *
   *     - find voicing factor and tilt of code for next subframe.          *
   *     - update states of weighting filter                                *
   *     - find excitation and synthesis speech                             *
   *------------------------------------------------------------------------*/
  p_A = wbamr_enc_cod_main_scr_ptr->A;
  p_Aq = wbamr_enc_cod_main_scr_ptr->Aq;

  for ( i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR )
  {
    pit_flag = ( fract16 ) i_subfr;

    if ( ( ( i_subfr- ( 2*L_SUBFR ) ) == 0 ) &&
         ( Sub_fr1x16 ( ser_size, NBBITS_7k ) > 0 ) )
    {
      pit_flag = 0;

      /* range for closed loop pitch search in 3rd subframe */
      T0_min = Sub_fr1x16 ( T_op2, 8 );

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
    /*-----------------------------------------------------------------------*
     *                                                                       *
     *        Find the target vector for pitch search:                       *
     *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                        *
     *                                                                       *
     *             |------|  res[n]                                          *
     * speech[n]---| A ( z ) |--------                                       *
     *             |------|       |   |--------| error[n]  |------|          *
     *           zero -- ( - )--| 1/A ( z ) |-----------| W ( z ) |-- target *
     *                   exc          |--------|           |------|          *
     *                                                                       *
     * Instead of subtracting the zero-input response of filters from        *
     * the weighted input speech, the above configuration is used to         *
     * compute the target vector.                                            *
     *                                                                       *
     *-----------------------------------------------------------------------*/

    for ( i = 0; i < M11; i++ )
    {
      wbamr_enc_cod_main_scr_ptr->u5.error[i] =
           Sub_fr1x16 ( speech[i + i_subfr - M11], enc_obj_ptr->mem_syn[i] );
    }
    WBAMR_Residu ( p_Aq,&speech[i_subfr], &exc[i_subfr], L_SUBFR,
                    enc_obj_temp_ptr );

    WBAMR_Syn_filt ( p_Aq,
                     M11,
                     &exc[i_subfr],
                     &wbamr_enc_cod_main_scr_ptr->u5.error[M11],
                     L_SUBFR,
                     wbamr_enc_cod_main_scr_ptr->u5.error,
                     0,
                     tmp_scr_ptr );

    WBAMR_Weight_a ( p_A, wbamr_enc_cod_main_scr_ptr->u4.Ap, GAMMA1, M11 );

    WBAMR_Residu ( wbamr_enc_cod_main_scr_ptr->u4.Ap,
                   &wbamr_enc_cod_main_scr_ptr->u5.error[M11],
                   wbamr_enc_cod_main_scr_ptr->xn,
                   L_SUBFR,
                   enc_obj_temp_ptr );

    WBAMR_Deemph2 ( wbamr_enc_cod_main_scr_ptr->xn,
                    L_SUBFR,
                    &( enc_obj_ptr->mem_w0 ) );

    /*----------------------------------------------------------------------*
     * Find approx. target in residual domain "cn[]" for inovation search.  *
     *----------------------------------------------------------------------*/

    /* first half: xn[] --> cn[] */
    WBAMR_Set_zero ( wbamr_enc_cod_main_scr_ptr->code, M11 );
    WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->xn,
                 &wbamr_enc_cod_main_scr_ptr->code[M11],
                 L_SUBFR / 2 );
    tmp = 0;
    WBAMR_Preemph2 ( &wbamr_enc_cod_main_scr_ptr->code[M11], &tmp );
    WBAMR_Weight_a ( p_A, wbamr_enc_cod_main_scr_ptr->u4.Ap, GAMMA1, M11 );
    WBAMR_Syn_filt ( wbamr_enc_cod_main_scr_ptr->u4.Ap,
                     M11,
                     &wbamr_enc_cod_main_scr_ptr->code[M11],
                     &wbamr_enc_cod_main_scr_ptr->code[M11],
                     L_SUBFR / 2,
                     wbamr_enc_cod_main_scr_ptr->code,
                     0,
                     tmp_scr_ptr );
    WBAMR_Residu ( p_Aq,
                   &wbamr_enc_cod_main_scr_ptr->code[M11],
                   wbamr_enc_cod_main_scr_ptr->cn,
                   L_SUBFR / 2,
                   enc_obj_temp_ptr );

    /* second half: res[] --> cn[] ( approximated and faster ) */
    WBAMR_Copy ( &exc[i_subfr + ( L_SUBFR / 2 )],
                 &wbamr_enc_cod_main_scr_ptr->cn [ ( L_SUBFR / 2 )],
                 L_SUBFR / 2 );

    /*---------------------------------------------------------------*
     * Compute impulse response, h1[], of weighted synthesis filter  *
     *---------------------------------------------------------------*/

    WBAMR_Set_zero ( wbamr_enc_cod_main_scr_ptr->u5.error, M11 + L_SUBFR );
    WBAMR_Weight_a ( p_A,
                     &wbamr_enc_cod_main_scr_ptr->u5.error[M11],
                     GAMMA1,
                     M11 );

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
    pU5error = &wbamr_enc_cod_main_scr_ptr->u5.error[M11];
    pU6h1 = wbamr_enc_cod_main_scr_ptr->u6.h1;
    p_Aq += 1;
    i = L_SUBFR >> 1;
    do //for ( i = 0; i < L_SUBFR; i++ )
    {
      L_tmp = (*pU5error-- << 15);
      L_tmp = -L_tmp;

      L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
      L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
      L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
      L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
      j = 3;
      do {
         L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
         L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
         L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
         L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
         //L_tmp = Mac_fr16_32 ( L_tmp, p_Aq[j], wbamr_enc_cod_main_scr_ptr->u5.error[i + M11 - j] );
         //smlabb(p_Aq[j], wbamr_enc_cod_main_scr_ptr->u5.error[i + M11 - j], L_tmp);
      } while (--j != 0);
      L_tmp = -L_tmp;

      p_Aq -= 16;
      pU5error += 17;

      L_tmp = Round_fr1x32 ( Shl_fr1x32 ( L_tmp, 3 ) );
      *pU5error++ = L_tmp;
      *pU6h1++ = L_tmp;

      L_tmp = (*pU5error-- << 15);
      L_tmp = -L_tmp;

      L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
      L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
      L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
      L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
      j = 3;
      do {
         L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
         L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
         L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
         L_tmp = AWB_qdadd ( L_tmp, AWB_smulbb(*p_Aq++, *pU5error-- ));
         //L_tmp = Mac_fr16_32 ( L_tmp, p_Aq[j], wbamr_enc_cod_main_scr_ptr->u5.error[i + M11 - j] );
         //smlabb(p_Aq[j], wbamr_enc_cod_main_scr_ptr->u5.error[i + M11 - j], L_tmp);
      } while (--j != 0);
      L_tmp = -L_tmp;

      p_Aq -= 16;
      pU5error += 17;

      L_tmp = Round_fr1x32 ( Shl_fr1x32 ( L_tmp, 3 ) );
      *pU5error++ = L_tmp;
      *pU6h1++ = L_tmp;
    } while(--i != 0);
    p_Aq -= 1;
#else
    for ( i = 0; i < L_SUBFR; i++ )
    {
      L_tmp = Mult_fr1x32 ( wbamr_enc_cod_main_scr_ptr->u5.error[i + M11],
                            16384 );        /* x4 ( Q12 to Q14 ) */
      for ( j = 1; j <= M11; j++ )
        L_tmp = WBAMR_L_msu (
                       L_tmp,
                       p_Aq[j],
                       wbamr_enc_cod_main_scr_ptr->u5.error[i + M11 - j] );

      wbamr_enc_cod_main_scr_ptr->u6.h1[i] =
      wbamr_enc_cod_main_scr_ptr->u5.error[i + M11] =
                             Round_fr1x32 ( Shl_fr1x32 ( L_tmp, 3 ) );
    }
#endif
    /* deemph without division by 2 -> Q14 to Q15 */
    tmp = 0;
    WBAMR_Deemph2 ( wbamr_enc_cod_main_scr_ptr->u6.h1,  L_SUBFR, &tmp );
                                                /* h1 in Q14 */

    /* h2 in Q12 for codebook search */
    WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->u6.h1,
                 wbamr_enc_cod_main_scr_ptr->u5.h2,
                 L_SUBFR );
    WBAMR_Scale_sig ( wbamr_enc_cod_main_scr_ptr->u5.h2, L_SUBFR, -2 );

    /*---------------------------------------------------------------*
     * scale xn[] and h1[] to avoid overflow in dot_product12 ()     *
     *---------------------------------------------------------------*/
    /* scaling of xn[] to limit dynamic at 12 bits */
    WBAMR_Scale_sig ( wbamr_enc_cod_main_scr_ptr->xn, L_SUBFR, shift );
    /* set h1[] in Q15 with scaling for convolution */
    WBAMR_Scale_sig ( wbamr_enc_cod_main_scr_ptr->u6.h1,
                      L_SUBFR,
                      Add_fr1x16 ( 1, shift ) );

    /*----------------------------------------------------------------------*
     *                 Closed-loop fractional pitch search                  *
     *----------------------------------------------------------------------*/
    /* find closed loop fractional pitch  lag */
    if ( Sub_fr1x16 ( ser_size, NBBITS_9k ) <= 0 )
    {
      T0 = WBAMR_Pitch_fr4 ( &exc[i_subfr],
                             wbamr_enc_cod_main_scr_ptr->xn,
                             wbamr_enc_cod_main_scr_ptr->u6.h1,
                             T0_min,
                             T0_max,
                             &T0_frac,
                             pit_flag,
                             PIT_MIN,
                             PIT_FR1_8b,
                             tmp_scr_ptr,
                             enc_obj_temp_ptr );

      /* encode pitch lag */
      if ( pit_flag == 0 )             /* if 1st/3rd subframe */
      {
        /*--------------------------------------------------------------*
         * The pitch range for the 1st/3rd subframe is encoded with     *
         * 8 bits and is divided as follows:                            *
         *   PIT_MIN to PIT_FR1-1  resolution 1/2 ( frac = 0 or 2 )     *
         *   PIT_FR1 to PIT_MAX    resolution 1   ( frac = 0 )          *
         *--------------------------------------------------------------*/
        if ( Sub_fr1x16 ( T0, PIT_FR1_8b ) < 0 )
        {
          index = Sub_fr1x16 ( Add_fr1x16 ( Shl_fr1x16 ( T0, 1 ),
                               Shr_fr1x16 ( T0_frac, 1 ) ), ( PIT_MIN * 2 ) );
        }
        else
        {
          index = Add_fr1x16 ( Sub_fr1x16 ( T0, PIT_FR1_8b ),
                               ( ( PIT_FR1_8b - PIT_MIN ) * 2 ) );
        }

        WBAMR_Parm_serial ( index, 8, &paramsBuf );

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
      { /* if subframe 2 or 4 */
        /*--------------------------------------------------------------*
         * The pitch range for subframe 2 or 4 is encoded with 5 bits:  *
         *   T0_min  to T0_max     resolution 1/2 ( frac = 0 or 2 )     *
         *--------------------------------------------------------------*/
        i = Sub_fr1x16 ( T0, T0_min );
        index = Add_fr1x16 ( Shl_fr1x16 ( ( fract16 )i, 1 ),
                             Shr_fr1x16 ( T0_frac, 1 ) );

        WBAMR_Parm_serial ( index, 5, &paramsBuf );
      }
    }
    else
    {
      T0 = WBAMR_Pitch_fr4 ( &exc[i_subfr],
                             wbamr_enc_cod_main_scr_ptr->xn,
                             wbamr_enc_cod_main_scr_ptr->u6.h1,
                             T0_min,
                             T0_max,
                             &T0_frac,
                             pit_flag,
                             PIT_FR2,
                             PIT_FR1_9b,
                             tmp_scr_ptr,
                             enc_obj_temp_ptr );

      /* encode pitch lag */
      if ( pit_flag == 0 )  /* if 1st/3rd subframe */
      {
        /*--------------------------------------------------------------*
         * The pitch range for the 1st/3rd subframe is encoded with     *
         * 9 bits and is divided as follows:                            *
         *   PIT_MIN to PIT_FR2-1  resolution 1/4 ( frac = 0,1,2 or 3 ) *
         *   PIT_FR2 to PIT_FR1-1  resolution 1/2 ( frac = 0 or 1 )     *
         *   PIT_FR1 to PIT_MAX    resolution 1   ( frac = 0 )          *
         *--------------------------------------------------------------*/
        if ( Sub_fr1x16 ( T0, PIT_FR2 ) < 0 )
        {
          index = Sub_fr1x16 ( Add_fr1x16 ( Shl_fr1x16 ( T0, 2 ), T0_frac ),
                               ( PIT_MIN * 4 ) );
        }
        else if ( Sub_fr1x16 ( T0, PIT_FR1_9b ) < 0 )
        {
          index = Add_fr1x16 ( Sub_fr1x16 (
                                  Add_fr1x16 ( Shl_fr1x16 ( T0, 1 ),
                                               Shr_fr1x16 ( T0_frac, 1 ) ),
                                  ( PIT_FR2 * 2 ) ),
                               ( ( PIT_FR2 - PIT_MIN ) * 4 ) );
        }
        else
        {
          index = Add_fr1x16 ( Add_fr1x16 ( Sub_fr1x16 ( T0, PIT_FR1_9b ),
                                          ( ( PIT_FR2 - PIT_MIN ) * 4 ) ),
                               ( ( PIT_FR1_9b - PIT_FR2 ) * 2 ) );
        }

        WBAMR_Parm_serial ( index, 9, &paramsBuf );

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
      {                              /* if subframe 2 or 4 */
        /*--------------------------------------------------------------*
         * The pitch range for subframe 2 or 4 is encoded with 6 bits:  *
         *   T0_min  to T0_max     resolution 1/4 ( frac = 0,1,2 or 3 ) *
         *--------------------------------------------------------------*/
        i = Sub_fr1x16 ( T0, T0_min );
        index = Add_fr1x16 ( Shl_fr1x16 ( ( fract16 )i, 2 ), T0_frac );
        WBAMR_Parm_serial ( index, 6, &paramsBuf );
      }
    }

    /*-----------------------------------------------------------------*
     * Gain clipping test to avoid unstable synthesis on frame erasure *
     *-----------------------------------------------------------------*/
    clip_gain = WBAMR_Gp_clip ( enc_obj_ptr->gp_clip );

    /*-----------------------------------------------------------------*
     * - find unity gain pitch excitation ( adaptive codebook entry )  *
     *   with fractional interpolation.                                *
     * - find filtered pitch exc. y1[]=exc[] convolved with h1[] )     *
     * - compute pitch gain1                                           *
     *-----------------------------------------------------------------*/
    /* find pitch exitation */
    WBAMR_Pred_lt4 ( &exc[i_subfr], T0, T0_frac );

    if ( Sub_fr1x16 ( ser_size, NBBITS_9k ) > 0 )
    {

        tmp_scr_alt_ptr = wbamr_enc_cod_main_scr_ptr->u6.h1;


     WBAMR_Convolve ( &exc[i_subfr],
                      tmp_scr_alt_ptr,
                      wbamr_enc_cod_main_scr_ptr->u3.y1 );

     gain1 = WBAMR_G_pitch ( wbamr_enc_cod_main_scr_ptr->xn,
                              wbamr_enc_cod_main_scr_ptr->u3.y1,
                              wbamr_enc_cod_main_scr_ptr->g_coeff );

      /* clip gain if necessary to avoid problem at decoder */
      if ( ( clip_gain != 0 ) && ( Sub_fr1x16 ( gain1, GP_CLIP ) > 0 ) )
      {
        gain1 = GP_CLIP;
      }
      /* find energy of new target xn2[] */
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy
      WBAMR_Updt_tar_interleave ( wbamr_enc_cod_main_scr_ptr->xn,
                       wbamr_enc_cod_main_scr_ptr->dn,
                       wbamr_enc_cod_main_scr_ptr->u3.y1,
                       gain1 );       /* dn used temporary */
#else
      WBAMR_Updt_tar ( wbamr_enc_cod_main_scr_ptr->xn,
                       wbamr_enc_cod_main_scr_ptr->dn,
                       wbamr_enc_cod_main_scr_ptr->u3.y1,
                       gain1 );       /* dn used temporary */
#endif
    }
    else
    {
      gain1 = 0;
    }

      /*-----------------------------------------------------------------*
       * - find pitch excitation filtered by 1st order LP filter.        *
       * - find filtered pitch exc. y2[]=exc[] convolved with h1[] )     *
       * - compute pitch gain2                                           *
       *-----------------------------------------------------------------*/
      /* find pitch excitation with lp filter */
    for ( i = 0; i < L_SUBFR; i++ )
    {
      L_tmp = Mult_fr1x32 ( 5898, exc[i - 1 + i_subfr] );
      L_tmp = Mac_fr16_32 ( L_tmp, 20972, exc[i + i_subfr] );
      L_tmp = Mac_fr16_32 ( L_tmp, 5898, exc[i + 1 + i_subfr] );
      wbamr_enc_cod_main_scr_ptr->code[i] = Round_fr1x32 ( L_tmp );

    }

      tmp_scr_alt_ptr = wbamr_enc_cod_main_scr_ptr->code;

    WBAMR_Convolve ( tmp_scr_alt_ptr,
                     wbamr_enc_cod_main_scr_ptr->u6.h1,
                     wbamr_enc_cod_main_scr_ptr->y2 );


    gain2 = WBAMR_G_pitch ( wbamr_enc_cod_main_scr_ptr->xn,
                            wbamr_enc_cod_main_scr_ptr->y2,
                            wbamr_enc_cod_main_scr_ptr->g_coeff2 );

    /* clip gain if necessary to avoid problem at decoder */
    if ( ( clip_gain != 0 ) && ( Sub_fr1x16 ( gain2, GP_CLIP ) > 0 ) )
    {
      gain2 = GP_CLIP;
    }
    /* find energy of new target xn2[] */
    WBAMR_Updt_tar ( wbamr_enc_cod_main_scr_ptr->xn,
                     wbamr_enc_cod_main_scr_ptr->u6.xn2,
                     wbamr_enc_cod_main_scr_ptr->y2,
                     gain2 );

    /*-----------------------------------------------------------------*
     * use the best prediction ( minimise quadratic error ).           *
     *-----------------------------------------------------------------*/
    select = 0;

    if ( Sub_fr1x16 ( ser_size, NBBITS_9k ) > 0 )
    {
      L_tmp = 0L;
      for ( i = 0; i < L_SUBFR; i++ )
        L_tmp = Mac_fr16_32 ( L_tmp,
                              wbamr_enc_cod_main_scr_ptr->dn[i],
                              wbamr_enc_cod_main_scr_ptr->dn[i] );
      for ( i = 0; i < L_SUBFR; i++ )
        L_tmp = WBAMR_L_msu ( L_tmp,
                              wbamr_enc_cod_main_scr_ptr->u6.xn2[i],
                              wbamr_enc_cod_main_scr_ptr->u6.xn2[i] );

      if ( L_tmp <= 0 )
      {
        select = 1;
      }
      WBAMR_Parm_serial ( select, 1, &paramsBuf );
    }

    if ( select == 0 )
    {
      /* use the lp filter for pitch excitation prediction */
      gain_pit = gain2;
      WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->code, &exc[i_subfr], L_SUBFR );
      WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->y2,
                   wbamr_enc_cod_main_scr_ptr->u3.y1,
                   L_SUBFR );
      WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->g_coeff2,
                   wbamr_enc_cod_main_scr_ptr->g_coeff,
                   4 );
    }
    else
    {
      /* no filter used for pitch excitation prediction */
      gain_pit = gain1;
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy
      WBAMR_Copy_interleave ( wbamr_enc_cod_main_scr_ptr->dn,
                   wbamr_enc_cod_main_scr_ptr->u6.xn2);        /* target vector for codebook search */
#else
      WBAMR_Copy ( wbamr_enc_cod_main_scr_ptr->dn,
                   wbamr_enc_cod_main_scr_ptr->u6.xn2,
                   L_SUBFR );        /* target vector for codebook search */
#endif
    }

    /*-----------------------------------------------------------------*
     * - update cn[] for codebook search                               *
     *-----------------------------------------------------------------*/
    WBAMR_Updt_tar ( wbamr_enc_cod_main_scr_ptr->cn,
                     wbamr_enc_cod_main_scr_ptr->cn,
                     &exc[i_subfr],
                     gain_pit );
    /* scaling of cn[] to limit dynamic at 12 bits */
    WBAMR_Scale_sig ( wbamr_enc_cod_main_scr_ptr->cn, L_SUBFR, shift );

    /*-----------------------------------------------------------------*
     * - include fixed-gain pitch contribution into impulse resp. h1[] *
     *-----------------------------------------------------------------*/
    tmp = 0;
    WBAMR_Preemph ( wbamr_enc_cod_main_scr_ptr->u5.h2,
                    enc_obj_ptr->tilt_code,
                    &tmp );

    if ( T0_frac > 2 )
      T0 = Add_fr1x16 ( T0, 1 );
    WBAMR_Pit_shrp ( wbamr_enc_cod_main_scr_ptr->u5.h2, T0 );

    /*-----------------------------------------------------------------*
     * - Correlation between target xn2[] and impulse response h1[]    *
     * - Innovative codebook search                                    *
     *-----------------------------------------------------------------*/
    WBAMR_Cor_h_x ( wbamr_enc_cod_main_scr_ptr->u6.xn2,
                    wbamr_enc_cod_main_scr_ptr->u5.h2,
                    wbamr_enc_cod_main_scr_ptr->dn,
                    tmp_scr_ptr,
                    enc_obj_temp_ptr );

    if ( Sub_fr1x16 ( ser_size, NBBITS_7k ) <= 0 )
    {
      WBAMR_ACELP_2t64_fx ( wbamr_enc_cod_main_scr_ptr,
                            enc_scr_ptr,
                            enc_obj_temp_ptr );

      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[0],
                          12,
                          &paramsBuf );
    }
    else if ( Sub_fr1x16 ( ser_size, NBBITS_9k ) <= 0 )
    {
      WBAMR_ACELP_4t64_fx ( 20,
                            ser_size,
                            wbamr_enc_cod_main_scr_ptr,
                            enc_scr_ptr,
                            enc_obj_temp_ptr );

      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[0],
                          5,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[1],
                          5,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[2],
                          5,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[3],
                          5,
                          &paramsBuf );
    }
    else if ( Sub_fr1x16 ( ser_size, NBBITS_12k ) <= 0 )
    {
      WBAMR_ACELP_4t64_fx ( 36,
                            ser_size,
                            wbamr_enc_cod_main_scr_ptr,
                            enc_scr_ptr,
                            enc_obj_temp_ptr );

      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[0],
                          9,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[1],
                          9,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[2],
                          9,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[3],
                          9,
                          &paramsBuf );
    }
    else if ( Sub_fr1x16 ( ser_size, NBBITS_14k ) <= 0 )
    {
      WBAMR_ACELP_4t64_fx ( 44,
                            ser_size,
                            wbamr_enc_cod_main_scr_ptr,
                            enc_scr_ptr,
                            enc_obj_temp_ptr );

      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[0],
                          13,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[1],
                          13,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[2],
                          9,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[3],
                          9,
                          &paramsBuf );
    }
    else if ( Sub_fr1x16 ( ser_size, NBBITS_16k ) <= 0 )
    {
      WBAMR_ACELP_4t64_fx ( 52,
                            ser_size,
                            wbamr_enc_cod_main_scr_ptr,
                            enc_scr_ptr,
                            enc_obj_temp_ptr );

      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[0],
                          13,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[1],
                          13,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[2],
                          13,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[3],
                          13,
                          &paramsBuf );
    }
    else if ( Sub_fr1x16 ( ser_size, NBBITS_18k ) <= 0 )
    {
      WBAMR_ACELP_4t64_fx ( 64,
                            ser_size,
                            wbamr_enc_cod_main_scr_ptr,
                            enc_scr_ptr,
                            enc_obj_temp_ptr );

      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[0],
                          2,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[1],
                          2,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[2],
                          2,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[3],
                          2,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[4],
                          14,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[5],
                          14,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[6],
                          14,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[7],
                          14,
                          &paramsBuf );
    }
    else if ( Sub_fr1x16 ( ser_size, NBBITS_20k ) <= 0 )
    {
      WBAMR_ACELP_4t64_fx ( 72,
                            ser_size,
                            wbamr_enc_cod_main_scr_ptr,
                            enc_scr_ptr,
                            enc_obj_temp_ptr );

      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[0],
                          10,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[1],
                          10,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[2],
                          2,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[3],
                          2,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[4],
                          10,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[5],
                          10,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[6],
                          14,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[7],
                          14,
                          &paramsBuf );
    }
    else
    {
      WBAMR_ACELP_4t64_fx ( 88,
                            ser_size,
                            wbamr_enc_cod_main_scr_ptr,
                            enc_scr_ptr,
                            enc_obj_temp_ptr );

      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[0],
                          11,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[1],
                          11,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[2],
                          11,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[3],
                          11,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[4],
                          11,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[5],
                          11,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[6],
                          11,
                          &paramsBuf );
      WBAMR_Parm_serial ( wbamr_enc_cod_main_scr_ptr->indice[7],
                          11,
                          &paramsBuf );
    }

    /*-------------------------------------------------------*
     * - Add the fixed-gain pitch contribution to code[].    *
     *-------------------------------------------------------*/
    tmp = 0;
    WBAMR_Preemph ( wbamr_enc_cod_main_scr_ptr->code,
                    enc_obj_ptr->tilt_code,
                    &tmp );

    WBAMR_Pit_shrp ( wbamr_enc_cod_main_scr_ptr->code, T0 );

    /*----------------------------------------------------------*
     *  - Compute the fixed codebook gain                       *
     *  - quantize fixed codebook gain                          *
     *----------------------------------------------------------*/
    if ( Sub_fr1x16 ( ser_size, NBBITS_9k ) <= 0 )
    {
      index = WBAMR_Q_gain2 ( Add_fr1x16 ( Q_new, shift ),
                              6,
                              &gain_pit,
                              &L_gain_code,
                              clip_gain,
                              enc_obj_ptr->qua_gain,
                              wbamr_enc_cod_main_scr_ptr,
                              tmp_scr_ptr );

      WBAMR_Parm_serial ( index, 6, &paramsBuf );
    }
    else
    {
      index = WBAMR_Q_gain2 ( Add_fr1x16 ( Q_new, shift ),
                              7,
                              &gain_pit,
                              &L_gain_code,
                              clip_gain,
                              enc_obj_ptr->qua_gain,
                              wbamr_enc_cod_main_scr_ptr,
                              tmp_scr_ptr );

      WBAMR_Parm_serial ( index, 7, &paramsBuf );
    }

    /* test quantized gain of pitch for pitch clipping algorithm */
    WBAMR_Gp_clip_test_gain_pit ( gain_pit, enc_obj_ptr->gp_clip );

    L_tmp = Shl_fr1x32 ( L_gain_code, Q_new );/* saturation can occur here */
    gain_code = Round_fr1x32 ( L_tmp );       /* scaled gain_code with Qnew */

    /*----------------------------------------------------------*
     * Update parameters for the next subframe.                 *
     * - tilt of code: 0.0 ( unvoiced ) to 0.5 ( voiced )       *
     *----------------------------------------------------------*/
    /* find voice factor in Q15 ( 1=voiced, -1=unvoiced ) */
    WBAMR_Copy ( &exc[i_subfr], wbamr_enc_cod_main_scr_ptr->u7.exc2, L_SUBFR );
    WBAMR_Scale_sig ( wbamr_enc_cod_main_scr_ptr->u7.exc2, L_SUBFR, shift );

    voice_fac = WBAMR_voice_factor ( wbamr_enc_cod_main_scr_ptr->u7.exc2,
                                     shift,
                                     gain_pit,
                                     wbamr_enc_cod_main_scr_ptr->code,
                                     gain_code );

    /* tilt of code for next subframe: 0.5=voiced, 0=unvoiced */
    enc_obj_ptr->tilt_code = Add_fr1x16 ( Shr_fr1x16 ( voice_fac, 2 ), 8192 );

    /*------------------------------------------------------*
     * - Update filter's memory "mem_w0" for finding the    *
     *   target vector in the next subframe.                *
     * - Find the total excitation                          *
     * - Find synthesis speech to update mem_syn[].         *
     *------------------------------------------------------*/
    /* y2 in Q9, gain_pit in Q14 */
    L_tmp = Mult_fr1x32 ( gain_code,
                          wbamr_enc_cod_main_scr_ptr->y2[L_SUBFR - 1] );
    L_tmp = Shl_fr1x32 ( L_tmp, Add_fr1x16 ( 5, shift ) );
    L_tmp = Negate_fr1x32 ( L_tmp );
    L_tmp = Mac_fr16_32 ( L_tmp,
                          wbamr_enc_cod_main_scr_ptr->xn[L_SUBFR - 1],
                          16384 );
    L_tmp = WBAMR_L_msu ( L_tmp,
                          wbamr_enc_cod_main_scr_ptr->u3.y1[L_SUBFR - 1],
                          gain_pit );
    L_tmp = Shl_fr1x32 ( L_tmp, Sub_fr1x16 ( 1, shift ) );
    enc_obj_ptr->mem_w0 = Round_fr1x32 ( L_tmp );

    if ( Sub_fr1x16 ( ser_size, NBBITS_24k ) >= 0 )
      WBAMR_Copy ( &exc[i_subfr],
                   wbamr_enc_cod_main_scr_ptr->u7.exc2,
                   L_SUBFR );

#if (AWB_OPT_VER == 2)
    pexc = &exc[i_subfr];
    pcode = (fract32 *)wbamr_enc_cod_main_scr_ptr->code;
    i = 32;
    do
    {
       now_code = *pcode++;
      /* code in Q9, gain_pit in Q14 */
      L_tmp = AWB_smulbb( gain_code,  now_code);
      L_tmp <<= 5;
      L_tmp = AWB_smlabb(*pexc, gain_pit, L_tmp);
      L_tmp = AWB_qadd(L_tmp, L_tmp);
      *pexc++ = (fract16)(AWB_qdadd(0x8000, L_tmp) >> 16);

      L_tmp = AWB_smulbt( gain_code,  now_code);
      L_tmp <<= 5;
      L_tmp = AWB_smlabb(*pexc, gain_pit, L_tmp);
      L_tmp = AWB_qadd(L_tmp, L_tmp);
      *pexc++ = (fract16)(AWB_qdadd(0x8000, L_tmp) >> 16);
    } while(--i != 0);
#else

    for ( i = 0; i < L_SUBFR; i++ )
    {
      /* code in Q9, gain_pit in Q14 */
      L_tmp = Mult_fr1x32 ( gain_code, wbamr_enc_cod_main_scr_ptr->code[i] );
      L_tmp = Shl_fr1x32 ( L_tmp, 5 );
      L_tmp = Mac_fr16_32 ( L_tmp, exc[i + i_subfr], gain_pit );
      L_tmp = Shl_fr1x32 ( L_tmp, 1 );       /* saturation can occur here */
      exc[i + i_subfr] = Round_fr1x32 ( L_tmp );
    }
#endif
    WBAMR_Syn_filt ( p_Aq,
                     M11,
                     &exc[i_subfr],
                     wbamr_enc_cod_main_scr_ptr->u3.synth,
                     L_SUBFR,
                     enc_obj_ptr->mem_syn,
                     1,
                     tmp_scr_ptr );

    if ( Sub_fr1x16 ( ser_size, NBBITS_24k ) >= 0 )
    {
      /*------------------------------------------------------------*
       * phase dispersion to enhance noise in low bit rate          *
       *------------------------------------------------------------*/
      /* L_gain_code in Q16 */
      WBAMR_L_Extract ( L_gain_code, &gain_code, &gain_code_lo );

      /*------------------------------------------------------------*
       * noise enhancer                                             *
       * ~~~~~~~~~~~~~~                                             *
       * - Enhance excitation on noise. ( modify gain of code )     *
       *   If signal is noisy and LPC filter is stable, move gain   *
       *   of code 1.5 dB toward gain of code threshold.            *
       *   This decrease by 3 dB noise energy variation.            *
       *------------------------------------------------------------*/
      /* 1=unvoiced, 0=voiced */
      tmp = Sub_fr1x16 ( 16384, Shr_fr1x16 ( voice_fac, 1 ) );
      fac = Mult_fr1x16 ( stab_fac, tmp );

      L_tmp = L_gain_code;

      if ( Sub_fr1x32 ( L_tmp, enc_obj_ptr->L_gc_thres ) < 0 )
      {
        L_tmp = Add_fr1x32 ( L_tmp, WBAMR_Mpy_32_16 ( gain_code,
                                                      gain_code_lo, 6226 ) );

        if ( Sub_fr1x32 ( L_tmp, enc_obj_ptr->L_gc_thres ) > 0 )
        {
          L_tmp = enc_obj_ptr->L_gc_thres;
        }
      } else
      {
        L_tmp = WBAMR_Mpy_32_16 ( gain_code, gain_code_lo, 27536 );

        if ( Sub_fr1x32 ( L_tmp, enc_obj_ptr->L_gc_thres ) < 0 )
        {
          L_tmp = enc_obj_ptr->L_gc_thres;
        }
      }
      enc_obj_ptr->L_gc_thres = L_tmp;

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
      vfac = - ( (voice_fac >> 3) + 4096 );
      gain_c = AWB_qadd( Shl_fr1x32 ( L_gain_code, Q_new ), 0x8000 );
      gain_p = (fract32)gain_pit << 10;
      pcode = (fract32 *)wbamr_enc_cod_main_scr_ptr->u3.code2;
      pexc = wbamr_enc_cod_main_scr_ptr->u7.exc2;

      now_code = *pcode++;
      L_tmp = AWB_smulbb(now_code, 0x4000); // code[0]
      L_tmp = AWB_qadd(L_tmp, L_tmp);
      prev_code1 = AWB_smultb(now_code, vfac); //code[1]
      prev_code2 = AWB_smulbb(now_code, vfac); //code[0]
      L_tmp = AWB_qadd(L_tmp, prev_code1);
      L_tmp = AWB_smlawb(L_tmp, gain_c, 0x80);
      L_tmp = AWB_smlawb(gain_p, (fract32)(*pexc), L_tmp); //exc[2]
      if(AWB_clz(L_tmp ^ (L_tmp>>31)) > 8) {
         *pexc++ = (fract16)(L_tmp >> 8);
      }
      else if (L_tmp < 0) {
         *pexc++ = (fract16)(0x8000);
      }
      else {
         *pexc++ = 0x7fff;
      }

      i = 31;
      do {

         L_tmp = AWB_qdadd(prev_code2, AWB_smultb(now_code, 0x4000)); // code[1]
         now_code = *pcode++;
         prev_code2 = AWB_smulbb(now_code, vfac); //code[2]
         L_tmp = AWB_qadd(L_tmp, prev_code2);
         L_tmp = AWB_smlawb(L_tmp, gain_c, 0x80);
         L_tmp = AWB_smlawb(gain_p, (fract32)(*pexc), L_tmp); //exc[1]
         if(AWB_clz(L_tmp ^ (L_tmp>>31)) > 8) {
            *pexc++ = (fract16)(L_tmp >> 8);
         }
         else if (L_tmp < 0) {
            *pexc++ = (fract16)(0x8000);
         }
         else {
            *pexc++ = 0x7fff;
         }

         L_tmp = AWB_qdadd(prev_code1, AWB_smulbb(now_code, 0x4000)); // code[2]
         prev_code1 = AWB_smultb(now_code, vfac); //code[3]
         L_tmp = AWB_qadd(L_tmp, prev_code1);
         L_tmp = AWB_smlawb(L_tmp, gain_c, 0x80);
         L_tmp = AWB_smlawb(gain_p, (fract32)(*pexc), L_tmp); //exc[2]
         if(AWB_clz(L_tmp ^ (L_tmp>>31)) > 8) {
            *pexc++ = (fract16)(L_tmp >> 8);
         }
         else if (L_tmp < 0) {
            *pexc++ = (fract16)(0x8000);
         }
         else {
            *pexc++ = 0x7fff;
         }

      } while(--i != 0);
      L_tmp = AWB_qdadd(prev_code2, AWB_smultb(now_code, 0x4000)); // code[1]
      L_tmp = AWB_smlawb(L_tmp, gain_c, 0x80);
      L_tmp = AWB_smlawb(gain_p, (fract32)(*pexc), L_tmp); //exc[1]
      if(AWB_clz(L_tmp ^ (L_tmp>>31)) > 8) {
         *pexc++ = (fract16)(L_tmp >> 8);
      }
      else if (L_tmp < 0) {
         *pexc++ = (fract16)(0x8000);
      }
      else {
         *pexc++ = 0x7fff;
      }
#else
      tmp = Add_fr1x16 ( Shr_fr1x16 ( voice_fac, 3 ), 4096 );
                                /* 0.25=voiced, 0=unvoiced */
      L_tmp = WBAMR_L_deposit_h ( wbamr_enc_cod_main_scr_ptr->code[0] );
      L_tmp = WBAMR_L_msu ( L_tmp, wbamr_enc_cod_main_scr_ptr->code[1], tmp );
      wbamr_enc_cod_main_scr_ptr->u3.code2[0] = Round_fr1x32 ( L_tmp );

      for ( i = 1; i < L_SUBFR - 1; i++ )
      {
        L_tmp = WBAMR_L_deposit_h ( wbamr_enc_cod_main_scr_ptr->code[i] );
        L_tmp = WBAMR_L_msu ( L_tmp,
                              wbamr_enc_cod_main_scr_ptr->code[i + 1],
                              tmp );
        L_tmp = WBAMR_L_msu ( L_tmp,
                              wbamr_enc_cod_main_scr_ptr->code[i - 1],
                              tmp );
        wbamr_enc_cod_main_scr_ptr->u3.code2[i] = Round_fr1x32 ( L_tmp );
      }

      L_tmp = WBAMR_L_deposit_h ( wbamr_enc_cod_main_scr_ptr->code[L_SUBFR - 1] );
      L_tmp = WBAMR_L_msu ( L_tmp,
                            wbamr_enc_cod_main_scr_ptr->code[L_SUBFR - 2],
                            tmp );
      wbamr_enc_cod_main_scr_ptr->u3.code2[L_SUBFR - 1] =
                                                     Round_fr1x32 ( L_tmp );

      /* build excitation */
      gain_code = Round_fr1x32 ( Shl_fr1x32 ( L_gain_code, Q_new ) );

      for ( i = 0; i < L_SUBFR; i++ )
      {
        L_tmp = Mult_fr1x32 ( wbamr_enc_cod_main_scr_ptr->u3.code2[i],
                              gain_code );
        L_tmp = Shl_fr1x32 ( L_tmp, 5 );
        L_tmp = Mac_fr16_32 ( L_tmp,
                              wbamr_enc_cod_main_scr_ptr->u7.exc2[i],
                              gain_pit );
        L_tmp = Shl_fr1x32 ( L_tmp, 1 );   /* saturation can occur here */
        wbamr_enc_cod_main_scr_ptr->u7.exc2[i] = Round_fr1x32 ( L_tmp );
      }
#endif

      corr_gain = WBAMR_Enc_synthesis ( p_Aq,
                                        wbamr_enc_cod_main_scr_ptr->u7.exc2,
                                        Q_new,
                                        &speech16k[i_subfr * 5 / 4],
                                        enc_obj_ptr,
                                        enc_scr_ptr,
                                        tmp_scr_ptr,
                                        enc_obj_temp_ptr );

      WBAMR_Parm_serial ( corr_gain, 4, &paramsBuf );
    }
    p_A += ( M11 + 1 );
    p_Aq += ( M11 + 1 );
  }  /* end of subframe loop */

  /*--------------------------------------------------*
   * Update signal for next frame.                    *
   * -> save past of speech[], wsp[] and exc[].       *
   *--------------------------------------------------*/
  WBAMR_Copy ( &wbamr_enc_cod_main_scr_ptr->old_speech[L_FRAME],
               enc_obj_ptr->old_speech,
               L_TOTAL - L_FRAME );
  WBAMR_Copy ( &wbamr_enc_cod_main_scr_ptr->u8.old_exc[L_FRAME],
               enc_obj_ptr->old_exc,
               PIT_MAX + L_INTERPOL );

return_end:

  if ( *usedMode != MRDTX )
  {
    WBAMR_Order_Bits ( *usedMode,
                       wbamr_enc_cod_main_scr_ptr->paramsBuf,
                       wbamr_enc_cod_main_scr_ptr->u8.OrdparamsBuf );
    WBAMR_MIME_Pack_Bits ( usedMode,
		                   mode,
                           tx_type,
                           wbamr_enc_cod_main_scr_ptr->u8.OrdparamsBuf,
                           prms );
  }
  else
  {
    //WBAMR_Pack_Bits ( *usedMode, wbamr_enc_cod_main_scr_ptr->paramsBuf,prms );
    WBAMR_MIME_Pack_Bits ( usedMode,
		                   mode,
                           tx_type,
                           wbamr_enc_cod_main_scr_ptr->paramsBuf,
                           prms );
  }


  return;
}

/*-----------------------------------------------------*
 * Function synthesis ()                               *
 *                                                     *
 * Synthesis of signal at 16kHz with HF extension.     *
 *                                                     *
 *-----------------------------------------------------*/

WBAMR_CODE_SECTION fract16 WBAMR_Enc_synthesis (
  fract16 Aq[],                             /* A(z): quantized Az            */
  fract16 exc[],                            /* (i): excitation at 12kHz      */
  fract16 Q_new,                            /* (i): scaling performed on exc */
  fract16 synth16k[],                       /* (o): 16kHz synthesis signal   */
  WBAMR_Static_Mem_Enc_Struct * enc_obj_ptr,/* (i/o): State structure        */
  WBAMR_Temp_Mem_Enc_Struct  *enc_scr_ptr,
  void* tmp_scr_ptr,
  fract16* enc_obj_temp_ptr
 )
{
  fract16 i;
  fract16 fac, tmp, exp;
  fract16 ener, exp_ener;
  fract32 L_tmp;

  fract16 HP_est_gain, HP_calc_gain, HP_corr_gain;
  fract16 dist_min, dist;
  fract16 HP_gain_ind = 0;
  fract16 gain1, gain2;
  fract16 weight1, weight2;
  wbamr_enc_syn *wbamr_enc_syn_scr_ptr;
  void *tmp_alt_scr_ptr;

  wbamr_enc_syn_scr_ptr = & ( enc_scr_ptr->u1.syn_scr );

  /*------------------------------------------------------------*
   * speech synthesis                                           *
   * ~~~~~~~~~~~~~~~~                                           *
   * - Find synthesis speech corresponding to exc2[].           *
   * - Perform fixed deemphasis and hp 50hz filtering.          *
   * - Oversampling from 12.8kHz to 16kHz.                      *
   *------------------------------------------------------------*/
#if (AWB_OPT_VER == 2)
  WBAMR_Copy32 ( enc_obj_ptr->mem_syn_sig, wbamr_enc_syn_scr_ptr->synth_sig, M11 );

  WBAMR_Syn_filt_32 ( Aq,
                      exc,
                      Q_new,
                      &wbamr_enc_syn_scr_ptr->synth_sig[M11]);

  WBAMR_Copy32 ( &wbamr_enc_syn_scr_ptr->synth_sig[L_SUBFR],
               enc_obj_ptr->mem_syn_sig,
               M11 );

  WBAMR_Deemph_32 ( &wbamr_enc_syn_scr_ptr->synth_sig[M11],
                    wbamr_enc_syn_scr_ptr->synth,
                    & ( enc_obj_ptr->mem_deemph ) );
#else
  WBAMR_Copy ( enc_obj_ptr->mem_syn_hi, wbamr_enc_syn_scr_ptr->synth_hi, M11 );
  WBAMR_Copy ( enc_obj_ptr->mem_syn_lo, wbamr_enc_syn_scr_ptr->synth_lo, M11 );

  WBAMR_Syn_filt_32 ( Aq,
                      exc,
                      Q_new,
                      &wbamr_enc_syn_scr_ptr->synth_hi[M11],
                      &wbamr_enc_syn_scr_ptr->synth_lo[M11] );

  WBAMR_Copy ( &wbamr_enc_syn_scr_ptr->synth_hi[L_SUBFR],
               enc_obj_ptr->mem_syn_hi,
               M11 );
  WBAMR_Copy ( &wbamr_enc_syn_scr_ptr->synth_lo[L_SUBFR],
               enc_obj_ptr->mem_syn_lo,
               M11 );

  WBAMR_Deemph_32 ( &wbamr_enc_syn_scr_ptr->synth_hi[M11],
                    &wbamr_enc_syn_scr_ptr->synth_lo[M11],
                    wbamr_enc_syn_scr_ptr->synth,
                    & ( enc_obj_ptr->mem_deemph ) );
#endif
  WBAMR_HP50_12k8 ( wbamr_enc_syn_scr_ptr->synth,
                    L_SUBFR,
                    enc_obj_ptr->mem_sig_out );

  /* Original speechsignal as reference for high band gain quantisation */
  for ( i = 0; i < L_SUBFR16k; i++  )
  {
    wbamr_enc_syn_scr_ptr->HF_SP[i] = synth16k[i];
  }

  /*------------------------------------------------------*
   * HF noise synthesis                                   *
   * ~~~~~~~~~~~~~~~~~~                                   *
   * - Generate HF noise between 5.5 and 7.5 kHz.         *
   * - Set energy of noise according to synthesis tilt.   *
   *     tilt > 0.8 ==> - 14 dB ( voiced )                *
   *     tilt   0.5 ==> - 6 dB  ( voiced or noise )       *
   *     tilt < 0.0 ==>   0 dB  ( noise )                 *
   *------------------------------------------------------*/
  /* generate white noise vector */
  for ( i = 0; i < L_SUBFR16k; i++ )
  {
    wbamr_enc_syn_scr_ptr->HF[i] = Shr_fr1x16 (
                                     WBAMR_Random ( &( enc_obj_ptr->seed2) ),
                                     3 );
  }

  /* energy of excitation */
  WBAMR_Scale_sig ( exc, L_SUBFR, -3 );
  Q_new = Sub_fr1x16 ( Q_new, 3 );

  ener = Extract_hi_fr1x32 (
                 WBAMR_Dot_product12 ( exc, exc, L_SUBFR, &exp_ener ) );
  exp_ener = Sub_fr1x16 ( exp_ener, Add_fr1x16 ( Q_new, Q_new ) );

  /* set energy of white noise to energy of excitation */
  tmp = Extract_hi_fr1x32 ( WBAMR_Dot_product12 ( wbamr_enc_syn_scr_ptr->HF,
                                                  wbamr_enc_syn_scr_ptr->HF,
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
  tmp = Extract_hi_fr1x32 ( L_tmp ); /* tmp = 2 x sqrt (ener_exc/ener_hf) */

  for ( i = 0; i < L_SUBFR16k; i++ )
  {
    wbamr_enc_syn_scr_ptr->HF[i] = Mult_fr1x16 ( wbamr_enc_syn_scr_ptr->HF[i],
                                                 tmp );
  }

  /* find tilt of synthesis speech ( tilt: 1=voiced, -1=unvoiced ) */

  WBAMR_HP400_12k8 ( wbamr_enc_syn_scr_ptr->synth, enc_obj_ptr->mem_hp400 );

  L_tmp = 1L;
  for ( i = 0; i < L_SUBFR; i++ )
    L_tmp = Mac_fr16_32 ( L_tmp,
                          wbamr_enc_syn_scr_ptr->synth[i],
                          wbamr_enc_syn_scr_ptr->synth[i] );

  exp = Norm_fr1x32 ( L_tmp );
  ener = Extract_hi_fr1x32 ( Shl_fr1x32 ( L_tmp, exp ) ); /* ener = r[0] */

  L_tmp = 1L;
  for ( i = 1; i < L_SUBFR; i++ )
    L_tmp = Mac_fr16_32 ( L_tmp,
                          wbamr_enc_syn_scr_ptr->synth[i],
                          wbamr_enc_syn_scr_ptr->synth[i - 1] );

  tmp = Extract_hi_fr1x32 ( Shl_fr1x32 ( L_tmp, exp ) );    /* tmp = r[1] */

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

  if ( enc_obj_ptr->vad_hist > 0 )
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
  HP_est_gain = tmp;

  if ( Sub_fr1x16 ( HP_est_gain, 3277 ) < 0 )
  {
    HP_est_gain = 3277;                /* 0.1 in Q15 */
  }
  /* synthesis of noise: 4.8kHz..5.6kHz --> 6kHz..7kHz */
  WBAMR_Weight_a ( Aq, wbamr_enc_syn_scr_ptr->Ap, 19661, M11 ); /* fac=0.6 */

    tmp_alt_scr_ptr=wbamr_enc_syn_scr_ptr->Ap;

  WBAMR_Syn_filt ( tmp_alt_scr_ptr,
                   M11,
                   wbamr_enc_syn_scr_ptr->HF,
                   wbamr_enc_syn_scr_ptr->HF,
                   L_SUBFR16k,
                   enc_obj_ptr->mem_syn_hf,
                   1,
                   tmp_scr_ptr );

  /* noise High Pass filtering ( 1ms of delay ) */
  WBAMR_Filt_6k_7k ( wbamr_enc_syn_scr_ptr->HF,
                     enc_obj_ptr->mem_hf,
                     tmp_scr_ptr );

  /* filtering of the original signal */
  WBAMR_Filt_6k_7k ( wbamr_enc_syn_scr_ptr->HF_SP,
                     enc_obj_ptr->mem_hf2,
                     tmp_scr_ptr );

  /* check the gain difference */
  WBAMR_Scale_sig ( wbamr_enc_syn_scr_ptr->HF_SP, L_SUBFR16k, -1 );

  ener = Extract_hi_fr1x32 ( WBAMR_Dot_product12 (
                                             wbamr_enc_syn_scr_ptr->HF_SP,
                                             wbamr_enc_syn_scr_ptr->HF_SP,
                                             L_SUBFR16k,
                                             &exp_ener ) );

  /* set energy of white noise to energy of excitation */
  tmp = Extract_hi_fr1x32 ( WBAMR_Dot_product12 ( wbamr_enc_syn_scr_ptr->HF,
                                                  wbamr_enc_syn_scr_ptr->HF,
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
  L_tmp = Shl_fr1x32 ( L_tmp, exp ); /* L_tmp, L_tmp in Q31 */
  HP_calc_gain = Extract_hi_fr1x32 ( L_tmp );
                       /* tmp = sqrt ( ener_input/ener_hf ) */

  /* enc_obj_ptr->gain_alpha *= enc_obj_ptr->dtxHangoverCount/7 */
  L_tmp = Shl_fr1x32 ( Mult_fr1x32 ( enc_obj_ptr->dtxHangoverCount, 4681 ),
                       15 );
  enc_obj_ptr->gain_alpha = Mult_fr1x16 ( enc_obj_ptr->gain_alpha,
                                          Extract_hi_fr1x32 ( L_tmp ) );

  if ( Sub_fr1x16 ( enc_obj_ptr->dtxHangoverCount, 6 ) > 0 )
    enc_obj_ptr->gain_alpha = 32767;
  HP_est_gain = Shr_fr1x16 ( HP_est_gain, 1 );     /* From Q15 to Q14 */
  HP_corr_gain = Add_fr1x16 ( Mult_fr1x16 ( HP_calc_gain,
                                            enc_obj_ptr->gain_alpha ),
                              Mult_fr1x16 ( Sub_fr1x16 (
                                                   32767,
                                                   enc_obj_ptr->gain_alpha ),
                                            HP_est_gain ) );

  /* Quantise the correction gain */
  dist_min = 32767;
  for ( i = 0; i < 16; i++ )
  {
    dist = Mult_fr1x16 ( Sub_fr1x16 ( HP_corr_gain, HP_gain[i] ),
                         Sub_fr1x16 ( HP_corr_gain, HP_gain[i] ) );

    if ( dist_min > dist )
    {
      dist_min = dist;
      HP_gain_ind = i;
    }
  }

  HP_corr_gain = HP_gain[HP_gain_ind];

  /* return the quantised gain index using highest mode, otherwise zero */
  return ( HP_gain_ind );
}

#endif

