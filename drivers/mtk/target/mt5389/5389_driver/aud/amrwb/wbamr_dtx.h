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
 *   WBAMR_DTX.H
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

#ifndef DTX_H
#define DTX_H

#include "wbamr_cnst.h"
#include "wbamr_private.h"


fract16 WBAMR_Dtx_enc_init (
  WBAMR_Static_Mem_Enc_Struct* enc_obj_ptr,
  const fract16 isf_init[] );
			   
fract16 WBAMR_Dtx_enc_reset (
  WBAMR_Static_Mem_Enc_Struct* enc_obj_ptr,
  const fract16 isf_init[] );

fract16 WBAMR_Dtx_enc (
  fract16 isf[],
  fract16 exc2[],
  WBAMR_Static_Mem_Enc_Struct* enc_obj_ptr, 
  void *tmp_scr_ptr,
  uint8 ** prms );

fract16 WBAMR_Dtx_buffer (
  WBAMR_Static_Mem_Enc_Struct* enc_obj_ptr,  
  fract16 isf_new[],                         
  fract32 enr,                               
  fract16 codec_mode );

void WBAMR_Tx_dtx_handler (
  WBAMR_Static_Mem_Enc_Struct* enc_obj_ptr,    
  fract16 vad_flag,  
  fract16 *usedMode, 
  uint8 nsync );

void Qisf_ns(
  fract16 *isf1,
  fract16 *isf_q,
  fract16 *indice );

fract16 WBAMR_Dtx_dec_init (
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr, 
  const fract16 isf_init[] );
	
fract16 WBAMR_Dtx_dec_reset (
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr, 
  const fract16 isf_init[] );
     
void WBAMR_Dtx_dec_exit ( 
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr );

fract16 WBAMR_Dtx_dec(
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr,  
  fract16 *exc2,                             
  fract16 new_state,                      
  fract16 isf[],                          
  uint8   **prms );

void WBAMR_Dtx_dec_activity_update ( 
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr,
  fract16 isf[],
  fract16 exc[] );

fract16 WBAMR_Rx_dtx_handler (
  WBAMR_Static_Mem_Dec_Struct* dec_obj_ptr,  
  fract16 frame_type );

void WBAMR_Disf_ns (
  fract16 *indice,
  fract16 *isf_q );

void WBAMR_Aver_isf_history (
  fract16 isf_old[],
  fract16 indices[],
  fract32 isf_aver[],
  fract16 isf_tmp[] );
  
void WBAMR_Find_frame_indices (
  fract16 isf_old_tx[],
  fract16 indices[],
  WBAMR_Static_Mem_Enc_Struct *enc_obj_ptr );

fract16 WBAMR_Dithering_control (
  WBAMR_Static_Mem_Enc_Struct *enc_obj_ptr );

void WBAMR_CN_dithering (
  fract16 isf[M11],
  fract32 *L_log_en_int,
  fract16 *dither_seed );

#endif   /* DTX_H */
