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
 *   WBAMR_ROM.H
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


#ifndef WBAMR_ROM_H
#define WBAMR_ROM_H

#include "wbamr_typedef.h"
#include "wbamr_cnst.h"

extern const fract16 window[];
extern const fract16 grid[];
extern const fract16 tipos[];
extern const fract16 isp_init[];
extern const fract16 pdown_unusable[];
extern const fract16 cdown_unusable[];
extern const fract16 pdown_usable[] ;
extern const fract16 cdown_usable[] ;
extern const fract16 t_qua_gain6b[];
extern const fract16 t_qua_gain7b[];
extern const fract16 interpol_frac[];
extern const fract16 HP_gain[];
extern const fract16 isf_init[] ;
extern const fract16 fir_down[];
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
extern const fract32 fir_up[];
#else
extern const fract16 fir_up[];
#endif
extern const fract16 en_adjust[]; 
extern const fract16 en_adjust[];
extern const fract16 AWB_prmnofsf[];
extern const fract16 dfh_M7k[] ;
extern const fract16 dfh_M9k[] ;
extern const fract16 dfh_M12k[];
extern const fract16 dfh_M14k[];
extern const fract16 dfh_M16k[];
extern const fract16 dfh_M18k[];
extern const fract16 dfh_M20k[];
extern const fract16 dfh_M23k[];
extern const fract16 dfh_M24k[];
extern const fract16 *dhf[];
extern const fract16 a_hpw[];
extern const fract16 b_hpw[];
extern const fract16 b_hp4[];
extern const fract16 a_hp4[];
extern const fract16 b_hp5[];
extern const fract16 a_hp5[];
extern const fract16 window[] ;
extern const fract16 fir_7k[];
extern const fract16 table[] ;
extern const fract16 slope[];
extern const fract16 lag_l[];
extern const fract16 lag_h[];
extern const fract16 h_fir[];
extern const fract16 corrweight[];
extern const fract16 ph_imp_low[];
extern const fract16 ph_imp_mid[];
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
extern const fract32 inter4_2[];
#else
extern const fract16 inter4_2[];
#endif
extern const fract16 pred[];
extern const fract16 fir_6k_7k[]; 
extern const fract16 mean_isf[];
extern const fract16 dico1_isf[];
extern const fract16 dico2_isf[];
extern const fract16 dico21_isf[];
extern const fract16 dico22_isf[];
extern const fract16 dico23_isf[];
extern const fract16 dico24_isf[];
extern const fract16 dico25_isf[];
extern const fract16 dico21_isf_36b[];
extern const fract16 dico22_isf_36b[];
extern const fract16 dico23_isf_36b[];
extern const fract16 nb_of_bits[];
extern const fract16 table_log[];
extern const fract16 mean_isf_noise[];
extern const fract16 dico1_isf_noise[];
extern const fract16 dico2_isf_noise[];
extern const fract16 dico3_isf_noise[];
extern const fract16 dico4_isf_noise[];
extern const fract16 dico5_isf_noise[]; 
extern const fract16 table_isqrt[];
extern const fract16 table_pow2[];
extern const fract16 inter4_1[];
extern const uint16 SubjectiveReordering_660[];
extern const uint16 SubjectiveReordering_885[];
extern const uint16 SubjectiveReordering_1265[];
extern const uint16 SubjectiveReordering_1425[];
extern const uint16 SubjectiveReordering_1585[];
extern const uint16 SubjectiveReordering_1825[];
extern const uint16 SubjectiveReordering_1985[];
extern const uint16 SubjectiveReordering_2305[];
extern const uint16 SubjectiveReordering_2385[];
extern const uint16 SubjectiveReordering_660[];
extern const uint16* SubjectiveReordering[];
#if (AWB_OPT_VER == 2)
extern const fract32 accumIndex[][12];
#endif
extern const uint8 AWB_toc_byte[16];
extern const int16 AWB_unused_size[16];
extern const int16 AWB_packed_size[16];
#endif  /* WBAMR_ROM_H */


