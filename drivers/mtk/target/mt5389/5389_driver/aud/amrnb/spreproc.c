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
 *   spreproc.c
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

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : spreproc.c
*      Purpose          : Subframe preprocessing
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "spreproc.h"
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "weight_a.h"
#include "syn_filt.h"
#include "residu.h"
#include "copy.h"

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
int subframePreProc(
   AMR_Encode_FrameState *Enc,
   enum Mode mode,            /* i  : coder mode                            */
   const Word16 gamma1[],     /* i  : spectral exp. factor 1                */
   const Word16 gamma1_12k2[],/* i  : spectral exp. factor 1 for EFR        */
   const Word16 gamma2[],     /* i  : spectral exp. factor 2                */
   Word16 *A,                 /* i  : A(z) unquantized for the 4 subframes  */
   Word16 *Aq,                /* i  : A(z)   quantized for the 4 subframes  */
   Word16 *speech,            /* i  : speech segment                        */
   Word16 *mem_err,           /* i  : pointer to error signal               */
   Word16 *mem_w0,            /* i  : memory of weighting filter            */
   Word16 *zero,              /* i  : pointer to zero vector                */
   Word16 ai_zero[],          /* o  : history of weighted synth. filter     */
   Word16 exc[],              /* o  : long term prediction residual         */
   Word16 h1[],               /* o  : impulse response                      */
   Word16 xn[],               /* o  : target vector for pitch search        */
   Word16 res2[],             /* o  : long term prediction residual         */
   Word16 error[]             /* o  : error of LPC synthesis filter         */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i;
   const Word16 *g1;             	/* Pointer to correct gammma1 vector    */
   Word32 *tmp_w = Enc_share->shabuf.stE.u2E.s4E.tmp_w;
   Word16 *Ap1 = Enc_share->shabuf.stE.u2E.s4E.Ap1;		/* A(z) with spectral expansion         */

   /*---------------------------------------------------------------*
    * mode specific pointer to gamma1 values                        *
    *---------------------------------------------------------------*/

	if ( (mode == MR122) || (mode == MR102) )
        {
           g1 = gamma1_12k2;
	}
        else
        {
           g1 = gamma1;
	}
   /*---------------------------------------------------------------*
    * Find the weighted LPC coefficients for the weighting filter.  *
    *---------------------------------------------------------------*/
   Weight_Ai(A, g1, Ap1);

   /*---------------------------------------------------------------*
    * Compute impulse response, h1[], of weighted synthesis filter  *
    *---------------------------------------------------------------*/
   for (i = 0; i <= M; i++)
   {
      ai_zero[i] = Ap1[i];
   }

   Syn_filt(tmp_w , Aq, ai_zero, h1, L_SUBFR, zero+1, 0);
   /*------------------------------------------------------------------------*
    *                                                                        *
    *          Find the target vector for pitch search:                      *
    *                                                                        *
    *------------------------------------------------------------------------*/
   /* LPC residual */
   Residu(Aq, speech, res2);

   AMR_Copy32((Word32 *)res2, (Word32 *)exc, L_SUBFR/2);

   Syn_filt(tmp_w , Aq, exc, error, L_SUBFR, mem_err, 0);

   Residu(Ap1, error, xn);

   /*---------------------------------------------------------------*
    * Find the weighted LPC coefficients for the weighting filter.  *
    *---------------------------------------------------------------*/
   Weight_Ai(A, gamma2, Ap1);
   /*---------------------------------------------------------------*
    * Compute impulse response, h1[], of weighted synthesis filter  *
    *---------------------------------------------------------------*/
   Syn_filt(tmp_w , Ap1, h1, h1, L_SUBFR, zero+1, 0);
   /* target signal xn[]*/
   Syn_filt(tmp_w , Ap1, xn, xn, L_SUBFR, mem_w0, 0);

   return 0;

}

#endif

