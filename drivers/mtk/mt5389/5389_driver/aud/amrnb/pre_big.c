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
 *   pre_big.c
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
*      File             : pre_big.c
*      Purpose          : Big subframe (2 subframes) preprocessing
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "pre_big.h"

 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "syn_filt.h"
#include "weight_a.h"
#include "residu.h"


/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
int pre_big(
   AMR_Encode_FrameState *Enc,
   enum Mode mode,            /* i  : coder mode                             */
   const Word16 gamma1[],     /* i  : spectral exp. factor 1                 */
   const Word16 gamma1_12k2[],/* i  : spectral exp. factor 1 for EFR         */
   const Word16 gamma2[],     /* i  : spectral exp. factor 2                 */
   Word16 A_t[],              /* i  : A(z) unquantized, for 4 subframes, Q12 */
   Word16 frameOffset,        /* i  : Start position in speech vector,   Q0  */
   Word16 speech[],           /* i  : speech,                            Q0  */
   Word16 mem_w[],            /* i/o: synthesis filter memory state,     Q0  */
   Word16 wsp[]               /* o  : weighted speech                    Q0  */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i;
   Word32 aOffset;
   const Word16 *g1;           /* Pointer to correct gammma1 vector    */

//   Word16 Ap1[MP1+1];            /* A(z) with spectral expansion         */

   Word32 *tmp_w = Enc_share->shabuf.stE.u2E.s4E.tmp_w;
   Word16 *Ap1   = Enc_share->shabuf.stE.u2E.s4E.Ap1;            /* A(z) with spectral expansion         */

   if ( mode <= MR795 )
   {
       g1 = gamma1;
   }
   else
   {
       g1 = gamma1_12k2;
   }

   if (frameOffset > 0) {
      aOffset = 2*(MP1+1);
   }
   else {
      aOffset = 0;
   }

   /* process two subframes (which form the "big" subframe) */
/*
   for (i = 0; i < 2; i++)
   {
       Weight_Ai(&A_t[aOffset], g1, Ap1);
       Weight_Ai(&A_t[aOffset], gamma2, Ap2);
       Residu(Ap1, &speech[frameOffset], &wsp[frameOffset]);

       Syn_filt(Ap2, &wsp[frameOffset], &wsp[frameOffset], L_SUBFR, mem_w, 1);
       aOffset = (aOffset + MP1+1);
       frameOffset = (frameOffset + L_SUBFR);
   }
*/
   for (i = 0; i < 2; i++)
   {
       Weight_Ai(&A_t[aOffset], g1, Ap1);
       Residu(Ap1, &speech[frameOffset], &wsp[frameOffset]);

       Weight_Ai(&A_t[aOffset], gamma2, Ap1);
       Syn_filt(tmp_w ,Ap1, &wsp[frameOffset], &wsp[frameOffset], L_SUBFR, mem_w, 1);
       aOffset = (aOffset + MP1+1);
       frameOffset = (frameOffset + L_SUBFR);
   }

   return 0;
}

#endif

