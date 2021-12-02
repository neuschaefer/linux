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
 *   WBAMR_CAELP.H
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


#ifndef  ACELP_H
#define  ACELP_H

#include "wbamr_typedef.h"
#include "wbamr_private.h"


void WBAMR_Insertion_Sort ( 
  fract16 array[], 
  fract16 n );

void WBAMR_Insert (
  fract16 array[], 
  fract16 num, 
  fract16 x );

void WBAMR_Isf_Extrapolation (
  fract16 HfIsf[] );

void WBAMR_Init_Lagconc ( 
  fract16 lag_hist[] );

void WBAMR_Lagconc (
  fract16 gain_hist[],  
  fract16 lag_hist[],   
  fract16 *T0,
  fract16 *old_T0,
  fract16 *seed,
  fract16 unusable_frame,
  fract16 lag_hist2[] );

void WBAMR_Agc2 (
  fract16 *sig_in,   
  fract16 *sig_out,  
  fract16 l_trm );

void WBAMR_Init_Filt_7k  (
  fract16 mem[] );

void WBAMR_Filt_7k (
  fract16 signal[],
  fract16 mem[],
  fract16 x[] );

fract16 WBAMR_Median5 (
  fract16 x[] );

void WBAMR_Autocorr (
  fract16 x[],
  fract16 r_h[],
  fract16 r_l[],
  fract16 *enc_scr_ptr,
  fract16 *enc_obj_temp_ptr );

void WBAMR_Lag_window (
  fract16 r_h[],
  fract16 r_l[] );
  
void WBAMR_Init_Levinson (
  fract16 *mem );
  
void WBAMR_Levinson (
  fract16 Rh[],
  fract16 Rl[],
  fract16 A[],
  fract16 rc[],
  void *tmp_scr_ptr,
  fract16 *mem );

void WBAMR_Az_isp (
  fract16 old_isp[], 
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr );

void WBAMR_Isp_Az (
  fract16 isp[], 
  fract16 a[],
  fract16 m,
  Common_Scratch *com_scr_ptr ); 
  
void WBAMR_Isp_isf (
  fract16 isp[],                         
  fract16 isf[] );
  
void WBAMR_Isf_isp (
  fract16 isf[],
  fract16 isp[],
  fract16 m );
  
void WBAMR_Int_isp (
  fract16 isp_old[],
  fract16 isp_new[],
  const fract16 frac[],   
  fract16 Az[],     
  Common_Scratch *com_scr_ptr );
  
void WBAMR_Weight_a (
  fract16 a[],   
  fract16 ap[],
  fract16 gamma,
  fract16 m );


/*    isf quantizers  */

void WBAMR_Qpisf_2s_46b (
  fract16 *indice,
  fract16 *isf_q, 
  fract16 *past_isfq,
  fract16 nb_surv,   
  void    *tmp_scr_ptr,
  fract16 * enc_obj_temp_ptr );
  
void WBAMR_Qpisf_2s_36b (
  fract16 *indice,
  fract16 *isf_q,
  fract16 *past_isfq,
  fract16 nb_surv,   
  void    *tmp_scr_ptr,
  fract16 * enc_obj_temp_ptr );
  
void WBAMR_Dpisf_2s_46b (
  fract16 *indice,
  fract16 *isf_q,
  fract16 *past_isfq,
  fract16 *isfold,   
  fract16 *isf_buf,  
  fract16 bfi,       
  fract16 enc_dec,
  fract16 *ref_isf );
  
void WBAMR_Dpisf_2s_36b (
  fract16 *indice,
  fract16 *isf_q,
  fract16 *past_isfq,
  fract16 *isfold,
  fract16 *isf_buf,
  fract16 bfi,  
  fract16 enc_dec,
  fract16 *ref_isf );
  
void WBAMR_Qisf_ns (
  fract16 *isf1,
  fract16 *isf_q, 
  fract16 *indice );
  
void WBAMR_Disf_ns (
  fract16 *indice,
  fract16 *isf_q );
     
fract16 WBAMR_Sub_VQ (
  fract16 *x,      
  const fract16 *dico,   
  fract16 dim,     
  fract16 dico_size,
  fract32 *distance );
  
void WBAMR_Reorder_isf (
  fract16 *isf,
  fract16 min_dist,
  fract16 n );


/*    filter prototypes  */

void WBAMR_Init_Decim_12k8 (
  fract16 mem[] );
  
void WBAMR_Decim_12k8 (
  fract16 sig16k[],
  fract16 lg,   
  fract16 sig12k8[], 
  fract16 mem[],     
  WBAMR_Temp_Mem_Enc_Struct  *enc_scr_ptr );

void WBAMR_Init_Oversamp_16k (
  fract16 mem[] );
  
void WBAMR_Oversamp_16k (
  fract16 sig12k8[], 
  fract16 lg,        
  fract16 sig16k[],  
  fract16 mem[],     
  WBAMR_Temp_Mem_Dec_Struct  *dec_scr_ptr );

void WBAMR_Init_HP50_12k8 (
  fract16 mem[] );

void WBAMR_HP50_12k8 (
  fract16 signal[],   
  fract16 lg,         
  fract16 mem[] );
  
void WBAMR_Init_HP400_12k8 (
  fract16 mem[] );

void WBAMR_HP400_12k8 (
  fract16 signal[],   
  fract16 mem[] );

void WBAMR_Init_Filt_6k_7k ( 
  fract16 mem[] );

void WBAMR_Filt_6k_7k (
  fract16 signal[],
  fract16 mem[],
  Common_Scratch * com_scr_ptr );

void WBAMR_LP_Decim2(
  fract16 x[], 
  fract16 mem[],
  WBAMR_Temp_Mem_Enc_Struct  *enc_scr_ptr );

void WBAMR_Preemph (
  fract16 x[], 
  fract16 mu,
  fract16 *mem );
  
void WBAMR_Preemph2 (
  fract16 x[],   
  fract16 *mem );
  
void Deemph (
  fract16 x[],
  fract16 mu,
  fract16 L, 
  fract16 *mem );
  
void WBAMR_Deemph2 (
  fract16 x[],  
  fract16 L,    
  fract16 *mem );
  
#if (AWB_OPT_VER == 2)
void WBAMR_Deemph_32 (
  fract32 x_sig[], 
  fract16 y[],    
  fract16 *mem );
#else
void WBAMR_Deemph_32 (
  fract16 x_hi[], 
  fract16 x_lo[], 
  fract16 y[],    
  fract16 *mem );
#endif

void WBAMR_Convolve (
  fract16 x[],   
  fract16 h[],   
  fract16 y[] );


void WBAMR_Residu (
  fract16 a[],
  fract16 x[],
  fract16 y[],
  fract16 lg,
  fract16 * enc_obj_temp_ptr );
  
void WBAMR_Syn_filt (
  fract16 a[],   
  fract16 m,     
  fract16 x[],   
  fract16 y[],   
  fract16 lg,    
  fract16 mem[], 
  fract16 update,
  Common_Scratch *com_scr_ptr );
  
#if (AWB_OPT_VER == 2)
void WBAMR_Syn_filt_32 (
   fract16 a[],
   fract16 exc[],
   fract16 Qnew,
   fract32 sig32[]);
#else
void WBAMR_Syn_filt_32 (
  fract16 a[],
  fract16 exc[],
  fract16 Qnew, 
  fract16 sig_hi[],
  fract16 sig_lo[] );
#endif  
    
/*
 * pitch prototypes
 */

fract16 Pitch_ol (      
  fract16 signal[], 			
  fract16 pit_min,   
  fract16 pit_max,   
  fract16 L_frame );

fract16 WBAMR_Pitch_med_ol ( 
  fract16 wsp[],            
  fract16 L_min,         
  fract16 L_max,         
  fract16 L_frame,       
  fract16 L_0,           
  fract16 *gain,         
  fract16 *hp_wsp_mem,   
  fract16 *old_hp_wsp,   
  fract16 wght_flg,      
  fract16* enc_obj_temp_ptr );
  
fract16 WBAMR_Med_olag (
  fract16 prev_ol_lag,
  fract16 old_ol_lag[5] );
  
void WBAMR_Init_Hp_wsp (
  fract16 mem[] );

void WBAMR_Scale_mem_Hp_wsp (
  fract16 mem[], 
  fract16 exp );
  
void WBAMR_Hp_wsp (
  fract16 wsp[],
  fract16 hp_wsp[],
  fract16 lg,
  fract16 mem[] );

fract16 WBAMR_Pitch_fr4 (
  fract16 exc[],      
  fract16 xn[],                            
  fract16 h[],                             
  fract16 t0_min,     
  fract16 t0_max,     
  fract16 *pit_frac,  
  fract16 i_subfr,    
  fract16 t0_fr2,     
  fract16 t0_fr1,     
  void    *tmp_scr_ptr,
  fract16* enc_obj_temp_ptr );
  
void WBAMR_Pred_lt4 (
  fract16 exc[], 
  fract16 T0,
  fract16 frac );
  
  
 /*
  * gain prototypes
  */

fract16 WBAMR_G_pitch (
  fract16 xn[],     
  fract16 y1[],     
  fract16 g_coeff[] );
  
void WBAMR_Init_Q_gain2 (
  fract16 *mem );
  
fract16 WBAMR_Q_gain2 (
  fract16 Q_xn,     
  fract16 nbits,    
  fract16 *gain_pit,
  fract32 *gain_cod,
  fract16 gp_clip,  
  fract16 *mem,     
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  void    *tmp_scr_ptr );

void WBAMR_Init_D_gain2 (
  fract16 *mem );
  
void WBAMR_D_gain2 (
  fract16 index, 
  fract16 nbits,
  fract16 code[],
  fract16 *gain_pit,
  fract32 *gain_cod,
  fract16 bfi,   
  fract16 unusable_frame,   
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr );
  

/*
 * acelp prototypes
 */

void WBAMR_Cor_h_x (
  fract16 xn2[], 
  fract16 h2[], 
  fract16 dn[],
  fract32 y32[],
  fract16 *enc_obj_temp_ptr );
  
void WBAMR_ACELP_2t64_fx (
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  WBAMR_Temp_Mem_Enc_Struct  *enc_scr_ptr,
  fract16* enc_obj_temp_ptr );
  
void WBAMR_DEC_ACELP_2t64_fx (
  fract16 index,          
  fract16 code[] );
  
void WBAMR_ACELP_4t64_fx (
  fract16 nbbits,     
  fract16 ser_size,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  WBAMR_Temp_Mem_Enc_Struct  *enc_scr_ptr,
  fract16* enc_obj_temp_ptr );
  
void WBAMR_DEC_ACELP_4t64_fx (
  fract16 nbbits,         
  fract16 index[],        
  fract16 code[],         
  WBAMR_Temp_Mem_Dec_Struct  *dec_scr_ptr );

void WBAMR_Pit_shrp (
  fract16 *x,                            
  fract16 pit_lag );



/*
 * others prototypes 
 */
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy
void WBAMR_Copy_interleave (
  fract16 x[], 
  fract16 y[]);
#endif

void WBAMR_Copy (
  fract16 x[], 
  fract16 y[],
  fract16 L );
#if (AWB_OPT_VER == 2)
void WBAMR_Copy32 (
  fract32 x[], 
  fract32 y[],
  fract32 L );
#endif
  
void WBAMR_Set_zero (
  fract16 x[],
  fract16 L );

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy
void WBAMR_Updt_tar_interleave (
  fract16 *x,    
  fract16 *x2,
  fract16 *y,
  fract16 gain );
#endif
  
void WBAMR_Updt_tar (
  fract16 *x,    
  fract16 *x2,
  fract16 *y,
  fract16 gain );
  
fract16 WBAMR_voice_factor (
  fract16 exc[],         
  fract16 Q_exc,         
  fract16 gain_pit,      
  fract16 code[],        
  fract16 gain_code );
  
void WBAMR_Scale_sig (
  fract16 x[],    
  fract16 lg,     
  fract16 exp );

  fract16 WBAMR_Random ( 
    fract16 * seed );

void WBAMR_Init_gp_clip (
  fract16 mem[] );
  
fract16 WBAMR_Gp_clip (
  fract16 mem[] );
  
void WBAMR_Gp_clip_test_isf (
  fract16 isf[],         
  fract16 mem[] );
  
void WBAMR_Gp_clip_test_gain_pit (
  fract16 gain_pit,           
  fract16 mem[] );

void WBAMR_Init_Phase_dispersion (
  fract16 disp_mem[] );
  
void WBAMR_Phase_dispersion (
  fract16 gain_code,                     
  fract16 gain_pit,                      
  fract16 code[],                     
  fract16 mode,                       
  fract16 disp_mem[],                 
  WBAMR_Temp_Mem_Dec_Struct  *dec_scr_ptr );

fract16 WBAMR_Chebps2 (
  fract16 x, 
  fract16 f[], 
  fract16 n );

void WBAMR_Add_pulses (
  fract16 pos[], 
  fract16 nb_pulse, 
  fract16 track, 
  fract16 code[] );

void WBAMR_Down_samp (
  fract16 *sig,   
  fract16 *sig_d, 
  fract16 L_frame_d );
  
void WBAMR_Up_samp (
  fract16 *sig_d,
  fract16 *sig_u,
  fract16 L_frame );
  
fract16 WBAMR_Interpol ( 
  fract16 *x,       
  fract16 *fir,
  fract16 frac,
  fract16 resol,
  fract16 nb_coef );


void WBAMR_Dec_synthesis (
  fract16 Aq[],
  fract16 exc[],  
  fract16 Q_new,  
  fract16 synth16k[],
  fract16 prms,      
  fract16 HfIsf[],
  fract16 nb_bits,
  fract16 newDTXState,
  WBAMR_Static_Mem_Dec_Struct *dec_obj_ptr, 
  fract16 bfi,                              
  WBAMR_Temp_Mem_Dec_Struct *dec_scr_ptr,
  fract16 *dec_obj_temp_ptr );

 void WBAMR_Get_isp_pol (
  fract16 *isp, 
  fract32 *f, 
  fract16 n );
     
 void WBAMR_Get_isp_pol_16kHz (
  fract16 *isp, 
  fract32 *f, 
  fract16 n );

void WBAMR_Norm_Corr (
  fract16 exc[],
  fract16 xn[],
  fract16 h[], 
  fract16 t_min, 
  fract16 t_max, 
  fract16 corr_norm[],   
  fract16 excf[],
  fract16* enc_obj_temp_ptr );

fract16 WBAMR_Interpol_4(   
  fract16 *x,            
  fract16 frac );

void WBAMR_VQ_stage1 (
  fract16 *x,     
  const fract16 *dico,  
  fract16 dim,    
  fract16 dico_size,
  fract16 *index,   
  fract16 surv,     
  fract32 *dist_min,
  fract16 * enc_obj_temp_ptr );
  
void WBAMR_C4t64_nbpos (
  fract16 nbbits,
  fract16 ser_size,
  fract16 *nbiter,
  fract16 *alp,
  fract16 *nb_pulse,
  wbamr_enc_c4t64  *wbamr_enc_c4t64_scr_ptr );

void WBAMR_C4t64_dn2 (
  wbamr_enc_c4t64    *wbamr_enc_c4t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  fract16 nb_pulse,
  fract16 alp );
  
 
void WBAMR_C4t64_ipos (
  wbamr_enc_c4t64    *wbamr_enc_c4t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  fract16 *enc_obj_temp_ptr,
  fract16 nbbits,
  fract16 nbiter,
  fract16 nb_pulse );
              
void WBAMR_C4t64_codeword (
  wbamr_enc_c4t64    *wbamr_enc_c4t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  fract16 h_shift,
  fract16 nb_pulse );
  
void WBAMR_C4t64_indice (
  wbamr_enc_c4t64    *wbamr_enc_c4t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  fract16 nbbits );  
  
#if (AWB_OPT_VER == 2)
void WBAMR_C4t64_search_ixiy (
   fract16 nb_pos_ix,
   fract16 track_x,  
   fract16 track_y,  
   fract32 *ps,      
   fract32 *alp,     
   fract16 *ix,      
   fract16 *iy,      
   fract16 dn[],     
   wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr );
void WBAMR_C4t64_corrMatrix (
   wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr,
   wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
   fract16 nb_pulse,
   fract16 *h_shift) ;
void WBAMR_C4t64_Accumulate (
   wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr,
   int track_x,
   int pulse_x,
   int pulse_y) ;

#else
void WBAMR_C4t64_search_ixiy (
  fract16 nb_pos_ix,
  fract16 track_x,  
  fract16 track_y,  
  fract16 *ps,      
  fract16 *alp,     
  fract16 *ix,      
  fract16 *iy,      
  fract16 dn[],     
  wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr );
void WBAMR_C4t64_rrixix (
  wbamr_enc_c4t64    *wbamr_enc_c4t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  fract16 nb_pulse,
  fract16 *h_shift );
  
void WBAMR_C4t64_rrixiy (
  wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr,
  fract16* enc_obj_temp_ptr );
  
#if (AWB_OPT_VER == 1)
void WBAMR_Cor_h_vec_0 (
  fract16 h[],    
  fract16 vec[],  
  fract16 sign[], 
  fract16 rrixix[][NB_POS16],   
  fract16 cor_x[],
  fract16 cor_y[] );

void WBAMR_Cor_h_vec_1 (
  fract16 h[],    
  fract16 vec[],  
  fract16 sign[], 
  fract16 rrixix[][NB_POS16],   
  fract16 cor_x[],
  fract16 cor_y[] );

void WBAMR_Cor_h_vec_2 (
  fract16 h[],    
  fract16 vec[],  
  fract16 sign[], 
  fract16 rrixix[][NB_POS16],   
  fract16 cor_x[],
  fract16 cor_y[] );

void WBAMR_Cor_h_vec_3 (
  fract16 h[],    
  fract16 vec[],  
  fract16 sign[], 
  fract16 rrixix[][NB_POS16],   
  fract16 cor_x[],
  fract16 cor_y[] );

#else

void WBAMR_Cor_h_vec (
  fract16 h[],    
  fract16 vec[],  
  fract16 track,  
  fract16 sign[], 
  fract16 rrixix[][NB_POS16],   
  fract16 cor[] );
#endif

#endif

void WBAMR_C2t64_dn2(
  wbamr_enc_c2t64 *wbamr_enc_c2t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr );

void WBAMR_C2t64_rrixix (
  wbamr_enc_c2t64 *wbamr_enc_c2t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr );

void WBAMR_C2t64_rrixiy (
  wbamr_enc_c2t64 *wbamr_enc_c2t64_scr_ptr,
  fract16* enc_obj_temp_ptr );

void WBAMR_C2t64_search_ixiy (
  wbamr_enc_c2t64 *wbamr_enc_c2t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr );  


#endif  /* ACELP_H */
