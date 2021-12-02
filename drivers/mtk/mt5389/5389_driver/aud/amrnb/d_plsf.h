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
 *   d_plsf.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR codec implementation
 *
 * Author:
 * -------
 *   Stan Huang
 *
 *------------------------------------------------------------------------------
 * $Revision: #1 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : d_plsf.h
*      Purpose          : common part (init, exit, reset) of LSF decoder
*                         module (rest in d_plsf_3.c and d_plsf_5.c)
*
********************************************************************************
*/
#ifndef d_plsf_h
#define d_plsf_h "$Id $"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
/*
*--------------------------------------------------*
* Constants (defined in cnst.h)                    *
*--------------------------------------------------*
* M            : LPC Order                         *
*--------------------------------------------------*
*/

/*
**************************************************************************
*
*  Function    : D_plsf_reset
*  Purpose     : Resets state memory
*  Returns     : 0 on success
*
**************************************************************************
*/
int D_plsf_reset (D_plsfState *st);


/*
**************************************************************************
*
*  Function    : D_plsf_5
*  Purpose     : Decodes the 2 sets of LSP parameters in a frame
*                using the received quantization indices.
*  Description : The two sets of LSFs are quantized using split by
*                5 matrix quantization (split-MQ) with 1st order MA
*                prediction.
*                See "q_plsf_5.c" for more details about the
*                quantization procedure
*  Returns     : 0
*
**************************************************************************
*/
int D_plsf_5 (
    AMR_Decode_FrameState *Dec,
    D_plsfState *st,  /* i/o: State variables                            */
    Word16 bfi,       /* i  : bad frame indicator (set to 1 if a bad
                              frame is received)                         */
    Word16 *indice,   /* i  : quantization indices of 5 submatrices, Q0  */
    Word16 *lsp1_q,   /* o  : quantized 1st LSP vector (M)           Q15 */
    Word16 *lsp2_q    /* o  : quantized 2nd LSP vector (M)           Q15 */
);

/*************************************************************************
 *
 *  FUNCTION:   D_plsf_3()
 *
 *  PURPOSE: Decodes the LSP parameters using the received quantization
 *           indices.1st order MA prediction and split by 3 matrix
 *           quantization (split-MQ)
 *
 *************************************************************************/

void D_plsf_3(
    Word16 *lsf1_q_t,
    D_plsfState *st,  /* i/o: State struct                               */
    enum Mode mode,   /* i  : coder mode                                 */
    Word16 bfi,       /* i  : bad frame indicator (set to 1 if a         */
                      /*      bad frame is received)                     */
    Word16 * indice,  /* i  : quantization indices of 3 submatrices, Q0  */
    Word16 * lsp1_q   /* o  : quantized 1st LSP vector,              Q15 */
);

/*************************************************************************
 *
 *  FUNCTION:   Init_D_plsf_3()
 *
 *  PURPOSE: Set the past_r_q[M] vector to one of the eight
 *           past_rq_init vectors.
 *
 *************************************************************************/
void Init_D_plsf_3(D_plsfState *st,  /* i/o: State struct                */
		   Word16 index      /* i  : past_rq_init[] index [0, 7] */
);

#endif
