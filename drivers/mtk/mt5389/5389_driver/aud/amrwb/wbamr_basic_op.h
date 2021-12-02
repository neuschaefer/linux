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
 *   WBAMR_BAISC_OP.H
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

#ifndef BASIC_OP_H
#define BASIC_OP_H


#include "lib_arith.h"


/*
 * Prototypes for basic arithmetic operators                               
 */

#define WBAMR_L_msu(s,x,y)  Sub_fr1x32 (s,Mult_fr1x32(x,y))


  fract16 WBAMR_Norm_s ( fract16 var1 );        /* Short norm,  15       */
  
  fract32 WBAMR_L_deposit_l ( fract16 var1 );   /* 16 bit var1 -> LSB, 2 */
  
  fract32 WBAMR_L_deposit_h ( fract16 var1 );   /* 16 bit var1 -> MSB, 2 */
  
  fract16 WBAMR_Msu_r ( 
    fract32 L_var3, 
    fract16 var1,
    fract16 var2 );         /* Msu with rounding,2   */
  
  fract16 WBAMR_Mac_r ( 
    fract32 L_var3, 
    fract16 var1, 
    fract16 var2 );        /* Mac with rounding,2 */


  fract16 WBAMR_Shr_r (
    fract16 var1, 
    fract16 var2 );         /* Shift right with round, 2 */

  fract32 WBAMR_L_shr_r (
    fract32 L_var1, 
    fract16 var2 );         /* Long shift right withround, 3 */




#endif    /* BASIC_OP_H */
