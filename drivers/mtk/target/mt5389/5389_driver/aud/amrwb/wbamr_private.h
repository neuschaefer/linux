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
 *   WBAMR_PRIVATE.H
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


#ifndef WBAMR_PRIVATE_H
#define WBAMR_PRIVATE_H


#include "wbamr_typedef.h"                          
#include "wbamr_cnst.h"      


/*************************************************************************
 * Global Declarations
 *************************************************************************/

/*************************************************************************
 * 
 * Name: WBAMR_Static_Mem_Enc_Struct
 * Type: <structure>
 * Scope Of Usage:
 * Description: Stucture containing all the internal states needed 
 *              by the encoder
 *************************************************************************/
typedef struct
{
  fract16 old_wsp[PIT_MAX / OPL_DECIM];   /* old decimated weighted speech */
  fract16 mem_decim[2 * L_FILT16k];       /* speech decimated filter memory */
  fract16 mem_sig_in[6];                  /* hp50 filter memory */
  fract16 mem_preemph;                    /* speech preemph filter memory */
  fract16 old_speech[L_TOTAL - L_FRAME];  /* old speech vector at 12.8kHz */
  fract16 old_exc[PIT_MAX + L_INTERPOL];  /* old excitation vector */
  fract16 mem_levinson[M11 + 2];          /* levinson routine memory */
  fract16 ispold[M11];                    /* old isp(immitt spectral pairs) */
  fract16 ispold_q[M11];                  /* quantized old isp */
  fract16 past_isfq[M11];                 /* past isf quantizer */
  fract16 mem_decim2[3];                  /* wsp decimation filter memory */
  fract16 mem_syn[M11];                   /* synthesis memory */
  fract16 Q_max[2];                       /* old maximum scaling factor */
  fract16 gp_clip[2];                     /* gain of pitch clipping memory */
  fract16 qua_gain[4];                    /* gain quantizer memory */
  fract16 old_ol_lag[5];
  fract16 hp_wsp_mem[9];
  fract16 old_hp_wsp[L_FRAME / OPL_DECIM + (PIT_MAX / OPL_DECIM)];
  fract16 isfold[M11];                 /* old isf (frequency domain) */
#if (AWB_OPT_VER == 2)
  fract32 mem_syn_sig[M11];             
#else
  fract16 mem_syn_hi[M11];             /* modified synthesis memory (MSB) */
  fract16 mem_syn_lo[M11];             /* modified synthesis memory (LSB) */
#endif
  fract16 mem_sig_out[6];              /* hp50 filter memory for synthesis */
  fract16 mem_hp400[6];                /* hp400 filter memory for synthesis */
  fract16 mem_oversamp[2 * L_FILT];    /* synthesis oversampled filter memory*/
  fract16 mem_syn_hf[M11];             /* HF synthesis memory */
  fract16 mem_hf[2 * L_FILT16k];       /* HF band-pass filter memory */
  fract16 mem_hf2[2 * L_FILT16k];      /* HF band-pass filter memory */
 
  /** dtx.h  **/
  fract16 isf_hist[M11 * DTX_HIST_SIZE];
  fract16 log_en_hist[DTX_HIST_SIZE];
    
  /* DTX handler stuff */
  fract32 D[28];
  fract32 sumD[DTX_HIST_SIZE];

  /** wb_vad.h **/
  fract16 bckr_est[COMPLEN];          /* background noise estimate */
  fract16 ave_level[COMPLEN];         /* aver i/p components for stat est */
  fract16 old_level[COMPLEN];         /* i/p levels of the previous frame */
  fract16 sub_level[COMPLEN];         /* i/p levels calc at eof(lookahead) */
#if (AWB_OPT_VER == 2)
  fract32 a_data5[F_5TH_CNT][2];      /* memory for the filter bank  */
  fract32 a_data3[F_3TH_CNT];         /* memory for the filter bank */
#else
  fract16 a_data5[F_5TH_CNT][2];      /* memory for the filter bank  */
  fract16 a_data3[F_3TH_CNT];         /* memory for the filter bank */
#endif

  
  /* Note that each of the following two variables holds 15 flags. 
   * Each flag reserves 1 bit of the  variable. The newest flag 
   * is in the bit 15 (assuming that LSB is bit 1 and MSB is bit 16).
   */
  fract32 L_gc_thres;                /* threshold for noise enhancer */
  fract32 prev_pow_sum;              /* power of previous frame  */
  fract16 vadreg;                    /* flags for intermediate VAD decisions*/
  fract16 tone_flag;                 /* tone detection flags */
  fract16 sp_est_cnt;                /* counter for speech level estimation */
  fract16 sp_max;                    /* maximum level */
  fract16 sp_max_cnt;                /* counts frames that contains speech */
  fract16 speech_level;              /* estimated speech level             */
  fract16 sid_update_counter;        /* From Tx_state */
  fract16 sid_handover_debt;
  fract16 prev_ft;	
  fract16 mem_wsp;                   /* wsp vector memory */
  fract16 mem_w0;                    /* target vector memory */
  fract16 tilt_code;                 /* tilt of code */
  fract16 old_wsp_max;               /* old wsp maximum value */
  fract16 old_wsp_shift;             /* old wsp shift */
  fract16 Q_old;                     /* old scaling factor */
  fract16 old_T0_med;
  fract16 ol_gain;
  fract16 ada_w;
  fract16 ol_wght_flg;
  fract16 first_frame;
  fract16 mem_deemph;               /* speech deemph filter memory */
  fract16 seed2;                    /* random memory for HF generation */
  fract16 vad_hist;
    
  /*dtx.h */
  fract16 gain_alpha;
  fract16 hist_ptr;
  fract16 log_en_index;
  fract16 cng_seed;
  fract16 dtxHangoverCount;
  fract16 decAnaElapsedCount;
  fract16 burst_count;             /* counts length of a speech burst */
  fract16 hang_count;              /* hangover counter */
  fract16 stat_count;              /* stationary counter */
  int16 nsync_vad_latch;           /* NSYNC & VAD flag stuff */
    
} WBAMR_Static_Mem_Enc_Struct;



/*************************************************************************
 * 
 * Name: WBAMR_Common_Temp_Struct
 * Type: <structure>
 * Scope Of Usage:
 * Description: Stucture containing all the common temp needed 
 *              by the encoder and decoder
 **************************************************************************/

typedef struct
{
  union
  {
    struct
    {
      fract32 f2[NC16k];
      fract32 f1[NC16k + 1];
      fract16 isp[M11];
    }int_lpc;
   
    fract16 x[L_SUBFR16k + (L_FIR - 1)];
    fract16 y_buf[L_SUBFR16k + M16k];
  }u;  	
  
} Common_Scratch;



/*************************************************************************
 * 
 * Name: WBAMR_Temp_Mem_Enc_Struct
 * Type: <structure>
 * Scope Of Usage:
 * Description: Stucture containing all the internal scratch needed 
 *              by the encoder
 **************************************************************************/

typedef struct
{
  fract16 old_speech[L_TOTAL];    /* Speech vector */
  fract16 r_h[M11 + 2];             
  fract16 r_l[M11 + 2];           /* Autocorrelations of windowed speech */
  
  union
  {
    fract16 synth[L_SUBFR];       /* 12.8kHz synthesis vector   */
    fract16 code2[L_SUBFR];       /* Fixed codebook excitation  */         
    fract16 y1[L_SUBFR];
  }u3;
    
  union
  {
    fract16 rc[M11];
    fract16 Ap[M11 ]; 
  }u4;
    
  fract16 ispnew[M11];             /* immittance spectral pairs at 4nd sfr */
  fract16 ispnew_q[M11];           /* quantized ISPs at 4nd subframe */
  fract16 isf[M11];                /* ISF (frequency domain) at 4nd sfr */
  fract16 A[NB_SUBFR * (M11 + 1)]; /* A(z) unquantized for the 4 subframes */
  fract16 Aq[NB_SUBFR * (M11 + 1)];/* A(z) quantized for the 4 subframes */
  fract16 xn[L_SUBFR];             /* Target vector for pitch search */
  fract16 dn[L_SUBFR];             /* Correlation between xn2 and h1 */
  fract16 cn[L_SUBFR];             /* Target vector in residual domain */
  fract16 code[L_SUBFR];           /* Fixed codebook excitation */
  
  union
  {
    fract16 h2[L_SUBFR];           /* Impulse response vector */
    fract16 error[M11 + L_SUBFR];  /* error of quantization */
  }u5;
  
  union
  {  	
    fract16 xn2[L_SUBFR];          /* Target vector for codebook search  */
    fract16 h1[L_SUBFR];           /* Impulse response vector            */
  }u6;
  
  fract16 indice[8];
  fract16 g_coeff[4];
  fract16 g_coeff2[4];
  fract16 f1[8 + 2];  	           /* M/2  + 1   */
  fract16 f2[8];      	           /* M/2        */
  
  union
  {
    fract16 exc2[L_FRAME];         /* excitation vector */    	
    fract16 buf[L_FRAME];
  }u7;
  
  fract16 y2[L_SUBFR];             /* Filtered adaptive excitation */
  
  union
  {
    fract16 old_wsp[L_FRAME + (PIT_MAX / OPL_DECIM)];
    fract16 old_exc[(L_FRAME + 1) + PIT_MAX + L_INTERPOL +1];
    uint8 OrdparamsBuf[NB_BITS_MAX + 3];
  }u8;

  uint8 paramsBuf[NB_BITS_MAX + 3];

}wbamr_enc_cod_main;



typedef struct
{  
  fract16 posA[6];
  fract16 posB[6];
  fract16 posA1[6];
  fract16 posB1[6];
  fract16 dn2[L_SUBFR];
#if (AWB_OPT_VER == 2)
  fract16 sign[2 * L_SUBFR];
  fract16 ind[NPMAXPT * NB_TRACK4];
  fract16 codvec[NB_PULSE_MAX];
  fract16 nbpos[10];
  fract16 pos_max[NB_TRACK4];
  fract16 h_buf[L_SUBFR];
  fract16 rrixiy[10][MSIZE4]; // for new algorith
  fract32 cor_accum[NB_TRACK4][NB_POS16]; // for new algorith
#else
  fract16 sign[L_SUBFR];
  fract16 vec[L_SUBFR];
  fract16 ind[NPMAXPT * NB_TRACK4];
  fract16 codvec[NB_PULSE_MAX];
  fract16 nbpos[10];
  fract16 cor_x[NB_POS16];
  fract16 cor_y[NB_POS16];
  fract16 pos_max[NB_TRACK4];
  fract16 h_buf[4 * L_SUBFR];
  fract16 rrixix[NB_TRACK4][NB_POS16];
  fract16 rrixiy[NB_TRACK4][MSIZE4];
#endif
  fract16 ipos[NB_PULSE_MAX];
  
}wbamr_enc_c4t64;


typedef struct 
{  
  fract16 sign[L_SUBFR];
  fract16 vec[L_SUBFR];
  fract16 dn2[L_SUBFR];
  fract16 h_buf[4 * L_SUBFR];
  fract16 rrixix[NB_TRACK][NB_POS];
  fract16 rrixiy[MSIZE];
  
}wbamr_enc_c2t64; 


typedef struct 
{  
  fract16 coeff[6];
  fract16 coeff_lo[6];
  fract16 exp_coeff[6];
  fract16 exp_max[6];
  
}wbamr_enc_qgain;


typedef struct 
{  
  fract16 excf[L_SUBFR];
  fract16 corr_v[40];
  
}wbamr_enc_pitch; 
  
         
typedef struct 
{  
  fract32 dist_min[N_SURV_MAX];
  fract16 surv1[N_SURV_MAX];
  fract16 isf[ORDER];
  fract16 isf_stage2[ORDER];
  fract16 ref_isf[M11];
  fract16 tmp_ind[6];
  
}wbamr_enc_qpisf46b; 
  
         
typedef struct 
{  
  fract32 L_isf[M11];
  fract16 isf_tmp[2 * M11];
  fract16 indice[7+1];  
  fract16 isf_order[3+1];
    
}wbamr_enc_dtxenc;
  
         
typedef struct 
{  
#if (AWB_OPT_VER == 2)
  fract32 synth_sig[M11 + L_SUBFR];
#else
  fract16 synth_hi[M11 + L_SUBFR];
  fract16 synth_lo[M11 + L_SUBFR];
#endif
  fract16 synth[L_SUBFR];
  fract16 HF[L_SUBFR16k];    
  fract16 HF_SP[L_SUBFR16k];
  fract16 Ap[M11 + 2];
  
}wbamr_enc_syn;

        
typedef struct 
{  
  fract16 Ah[M11 + 2];
  fract16 Al[M11 + 2];          
  fract16 Anh[M11 + 2];
  fract16 Anl[M11 + 2];         
  
}wbamr_enc_levin;
  
        
typedef struct 
{  
#if (AWB_OPT_VER == 2)
   fract32 tmp_buf[FRAME_LEN];
#else
   fract16 tmp_buf[FRAME_LEN];
#endif
   fract16 level[COMPLEN];
  
} wbamr_enc_wb_vad;



/*
 * WBAMR_Temp_Mem_Enc_Struct main structure
 */    	

typedef struct
{
  wbamr_enc_cod_main  cod_main_scr;

  union
  {  
    fract16           y[L_WINDOW];
    fract16           x_buf[L_FRAME + L_MEM + 1]; /* odd */
    fract16           signal[L_FRAME16k + (2 * NB_COEF_DOWN)];  
    wbamr_enc_wb_vad  wb_vad_scr; 
    wbamr_enc_syn     syn_scr; 
    wbamr_enc_c2t64   c2t64_scr; 
    wbamr_enc_c4t64   c4t64_scr; 
    
  }u1;

  union
  {  
    Common_Scratch     comm_scr;
    fract32            y32[L_SUBFR];
    wbamr_enc_levin    levin_scr; 
    wbamr_enc_dtxenc   dtxenc_scr; 
    wbamr_enc_qpisf46b qpisf46b_scr; 
    wbamr_enc_pitch    pitch_scr; 
    wbamr_enc_qgain    qgain_scr; 
    
  }u2;
        	 
}WBAMR_Temp_Mem_Enc_Struct;

    
           
/*****************************************************************************
 * 
 * Name: WBAMR_Static_Mem_Dec_Struct
 * Type: <structure>
 * Scope Of Usage:
 * Description: Stucture containing all the internal states needed 
 *              by the decoder
 ****************************************************************************/

typedef struct
{
  fract16 old_exc[PIT_MAX + L_INTERPOL];  /* old excitation vector */
  fract16 ispold[M11];                    /* old isp */
  fract16 isfold[M11];                    /* old isf (frequency domain) */
  fract16 isf_buf[L_MEANBUF * M11];       /* isf buffer(frequency domain)*/
  fract16 past_isfq[M11];                 /* past isf quantizer */
  fract16 Qsubfr[4];                      /* old maximum scaling factor */
#if (AWB_OPT_VER == 2)
  fract32 mem_syn_sig[M11];             
#else
  fract16 mem_syn_hi[M11];                /* modified synth memory (MSB) */
  fract16 mem_syn_lo[M11];                /* modified synth memory (LSB) */
#endif
  fract16 mem_sig_out[6];                 /* hp50 filter memory for synt */
  fract16 mem_oversamp[2 * L_FILT];       /* synth oversampled filter memory */
  fract16 mem_syn_hf[M16k];               /* HF synthesis memory */
  fract16 mem_hf[2 * L_FILT16k];          /* HF band-pass filter memory */
  fract16 mem_hf2[2 * L_FILT16k];         /* HF band-pass filter memory */
  fract16 mem_hf3[2 * L_FILT16k];         /* HF band-pass filter memory */
  fract16 dec_gain[23];                   /* gain decoder memory */
  fract16 disp_mem[8];                    /* phase dispersion memory */
  fract16 mem_hp400[6];                   /* hp400 filter memory for synt*/
  fract16 isf[M11];
  fract16 isf_old[M11];
  fract16 isf_hist[M11 * DTX_HIST_SIZE];
  fract16 log_en_hist[DTX_HIST_SIZE];
  fract16 lag_hist[5];
  fract16 prev_ft;                        /* From Rx_state */
  fract16 prev_mode;	
  fract16 tilt_code;                      /* tilt of code */
  fract16 Q_old;                          /* old scaling factor */
  fract32 L_gc_thres;                     /* threshold for noise enhancer */
  fract16 mem_deemph;                     /* speech deemph filter memory */
  fract16 seed;                           /* random memory for frame erasure*/
  fract16 seed2;                          /* random memory for HF generation*/
  fract16 old_T0;                         /* old pitch lag */
  fract16 old_T0_frac;                    /* old pitch fraction lag */
  fract16 seed3;                          /* random memory for lag conceal */
  fract16 prev_bfi;
  fract16 state;
  fract16 first_frame;
  fract16 vad_hist;
  
  /** dtx.h  **/
  fract16 since_last_sid;
  fract16 true_sid_period_inv;
  fract16 log_en;
  fract16 old_log_en;
  fract16 level;
  fract16 hist_ptr;
  fract16 cng_seed;
  fract16 dtxHangoverCount;
  fract16 decAnaElapsedCount;
  fract16 sid_frame;
  fract16 valid_data;
  fract16 dtxHangoverAdded;
  fract16 dtxGlobalState;        /* contains previous state */
  fract16 data_updated;         /* marker to know if CNI data renewed*/
  fract16 dither_seed;
  fract16 CN_dith;
  uint32 reset_flag;
  uint32 reset_flag_old;
    
} WBAMR_Static_Mem_Dec_Struct;



/*****************************************************************************
 * 
 * Name: WBAMR_Temp_Mem_Dec_Struct
 * Type: <structure>
 * Scope Of Usage:
 * Description: Stucture containing all the internal scratch needed 
 *              by the decoder
 ****************************************************************************/

typedef struct
{
  union
  {
    fract16 old_exc[(L_FRAME + 1) + PIT_MAX + L_INTERPOL +1];
    uint8 OrdparamsBuf[NB_BITS_MAX + 3];
  }u2;
  
  fract16 Aq[NB_SUBFR * (M11 + 1)]; /* A(z)   quantized for the 4 subframes*/
  fract16 ispnew[M11];              /* immittance spectral pairs at 4nd sfr*/
  fract16 isf[M11];                 /* ISF (frequency domain) at 4nd sfr */
  fract16 code[L_SUBFR];            /* algebraic codevector */
  fract16 code2[L_SUBFR];           /* algebraic codevector */
  fract16 exc2[L_FRAME];            /* excitation vector  */
  fract16 ind[8];
  fract16 excp[L_SUBFR];
  fract16 isf_tmp[M11];
  fract16 HfIsf[M16k];
  
  union
  {
    fract16 ref_isf[M11];
    fract16 lag_hist2[L_LTPHIST];        
  }u1;    
    
  uint8 paramsBuf[NB_BITS_MAX];

}wbamr_dec_main;


typedef struct
{
#if (AWB_OPT_VER == 2)
  union
  {
    fract16 HF[L_SUBFR16k]; 
    
  }u1;
  
  union
  { 
    fract32 synth_sig[M11 + L_SUBFR];
    fract16 x[L_SUBFR16k + (L_FIR - 1)];
    
    struct
    {\
      fract16 Ap[M16k + 2];
      fract16 HfA[M16k + 2];
    
    }st1;

  }u2; 
#else
  union
  {
    fract16 synth_hi[M11 + L_SUBFR];
    fract16 HF[L_SUBFR16k]; 
    
  }u1;
  
  union
  { 
    fract16 synth_lo[M11 + L_SUBFR];
    fract16 x[L_SUBFR16k + (L_FIR - 1)];
    
    struct
    {
      fract16 Ap[M16k + 2];
      fract16 HfA[M16k + 2];
    
    }st1;

  }u2; 
#endif
  fract16 synth[L_SUBFR];
    
}wbamr_dec_decsynth;


/*
 * WBAMR_Temp_Mem_Dec_Struct main structure
 */    	

typedef struct
{
  wbamr_dec_main  dec_main_scr;
  Common_Scratch  comm_scr;      
  
  union
  {
    fract16 signal1[L_SUBFR + (2 * NB_COEF_UP)];
    fract16 pos[6];
    fract16 code2[2 * L_SUBFR];
    wbamr_dec_decsynth  decsynth_scr;

  }u;	
   
} WBAMR_Temp_Mem_Dec_Struct;

#endif /* WBAMR_PRIVATE_H */
