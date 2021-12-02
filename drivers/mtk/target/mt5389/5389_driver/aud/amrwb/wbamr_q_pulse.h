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
 *   WBAMR_Q_PULSE.H
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

 
#ifndef Q_PULSE_H
#define Q_PULSE_H

#include "wbamr_typedef.h"
#include "wbamr_private.h"


fract32 WBAMR_Quant_1p_N1 (
  fract16 pos,             
  fract16 N ); 

fract32 WBAMR_Quant_2p_2N1 (    
  fract16 pos1, 
  fract16 pos2, 
  fract16 N );

fract32 WBAMR_Quant_3p_3N1(  
  fract16 pos1, 
  fract16 pos2, 
  fract16 pos3,
  fract16 N );  

fract32 WBAMR_Quant_4p_4N1 (
  fract16 pos1,          
  fract16 pos2, 
  fract16 pos3,
  fract16 pos4,  
  fract16 N );

fract32 WBAMR_Quant_4p_4N (  
  fract16 pos[],         
  fract16 N,             
  fract16 posA1[],                   
  fract16 posB1[] );     

fract32 WBAMR_Quant_5p_5N (
  fract16 pos[],
  fract16 N,    
  fract16 posA1[],                   
  fract16 posB1[] );

fract32 WBAMR_Quant_6p_6N_2 (
  fract16 pos[],
  fract16 N,
  fract16 posA[],                   
  fract16 posB[],                 
  fract16 posA1[],                   
  fract16 posB1[] );

void WBAMR_Dec_1p_N1 (
  fract32 index, 
  fract16 N, 
  fract16 offset, 
  fract16 pos[] );
 
void WBAMR_Dec_2p_2N1 (
  fract32 index, 
  fract16 N, 
  fract16 offset,
  fract16 pos[] );
  
void WBAMR_Dec_3p_3N1 ( 
  fract32 index, 
  fract16 N, 
  fract16 offset,
  fract16 pos[] );
  
void WBAMR_Dec_4p_4N1 (
  fract32 index, 
  fract16 N, 
  fract16 offset,
  fract16 pos[] );
  
void WBAMR_Dec_4p_4N (
  fract32 index, 
  fract16 N, 
  fract16 offset, 
  fract16 pos[] );
  
void WBAMR_Dec_5p_5N (
  fract32 index, 
  fract16 N, 
  fract16 offset, 
  fract16 pos[] );
  
void WBAMR_Dec_6p_6N_2 (
  fract32 index, 
  fract16 N,
  fract16 offset,
  fract16 pos[] );

#endif   /* Q_PULSE_H */
