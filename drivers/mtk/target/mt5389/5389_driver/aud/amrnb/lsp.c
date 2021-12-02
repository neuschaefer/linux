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
 *   lsp.h
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
*      File             : lsp.c
*      Purpose          : From A(z) to lsp. LSP quantization and interpolation
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "lsp.h"
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "q_plsf.h"
#include "copy.h"
#include "az_lsp.h"
#include "int_lpc.h"

/*
#include "lsp.tab"
*/

static const Word16 lsp_init_data[M] = {30000, 26000, 21000, 15000, 8000,
                                        0, -8000, -15000, -21000, -26000};


void lsp_reset (cod_amrState *st)
{

	/* Init lsp_old[] */
//	Copy(lsp_init_data, &st->lsp_old[0], M);
	AMR_Copy32((Word32 *)lsp_init_data, (Word32 *)&st->lsp_old[0], M/2);


	/* Initialize lsp_old_q[] */
//	Copy(st->lsp_old, st->lsp_old_q, M);
	AMR_Copy32((Word32 *)st->lsp_old, (Word32 *)st->lsp_old_q, M/2);

	/* Reset quantization state */
	Q_plsf_reset(st->past_rq);

	return;
}


/*************************************************************************
 *
 *   FUNCTION:  lsp()
 *
 ************************************************************************/
int lsp(
	   AMR_Encode_FrameState *Enc,
		cod_amrState *st,
      enum Mode req_mode,  /* i   : requested coder mode                    */
      enum Mode used_mode, /* i   : used coder mode                         */
      Word16 az[],         /* i/o : interpolated LP parameters Q12          */
      Word16 azQ[],        /* o   : quantization interpol. LP parameters Q12*/
      Word16 lsp_new[],    /* o   : new lsp vector                          */
      Word16 **anap        /* o   : analysis parameters                     */)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word16 pred_init_i; /* init index for MA prediction in DTX mode */

	Word16 *lsp_new_q = Enc_share->shabuf.stE.u2E.s2E.lsp_new_q;    /* LSPs at 4th subframe           */
   Word16 *lsp_mid = Enc_share->shabuf.stE.u2E.s2E.lsp_mid;     /* LSPs at 2nd subframe           */
	Word16 *lsp_mid_q = Enc_share->shabuf.stE.u2E.s2E.lsp_mid_q;

   if(req_mode == MR122)
   {
       Az_lsp (Enc , &az[MP1+1], lsp_mid, st->lsp_old);
       Az_lsp (Enc , &az[(MP1+1) * 3], lsp_new, lsp_mid);

       /*--------------------------------------------------------------------*
        * Find interpolated LPC parameters in all subframes (both quantized  *
        * and unquantized).                                                  *
        * The interpolated parameters are in array A_t[] of size (M+1)*4     *
        * and the quantized interpolated parameters are in array Aq_t[]      *
        *--------------------------------------------------------------------*/
       Int_lpc_1and3_2 (Enc, st->lsp_old, lsp_mid, lsp_new, az);

       if(used_mode != MRDTX)
       {
          /* LSP quantization (lsp_mid[] and lsp_new[] jointly quantized) */
          Q_plsf_5 (Enc , st->past_rq, lsp_mid, lsp_new, lsp_mid_q, lsp_new_q, *anap);

          Int_lpc_1and3 (Enc_share->shabuf.stE.u2E.s2E.f11,Enc_share->shabuf.stE.u2E.s2E.f22,
			 	Enc_share->shabuf.stE.u2E.s2E.lsp,
			 	st->lsp_old_q, lsp_mid_q, lsp_new_q, azQ);

          /* Advance analysis parameters pointer */
          (*anap) += 5;
       }
   }
   else
   {
       Az_lsp(Enc , &az[(MP1+1) * 3], lsp_new, st->lsp_old);  /* From A(z) to lsp  */

       /*--------------------------------------------------------------------*
        * Find interpolated LPC parameters in all subframes (both quantized  *
        * and unquantized).                                                  *
        * The interpolated parameters are in array A_t[] of size (M+1)*4     *
        * and the quantized interpolated parameters are in array Aq_t[]      *
        *--------------------------------------------------------------------*/

       Int_lpc_1to3_2(Enc, st->lsp_old, lsp_new, az);

       if(used_mode != MRDTX)
       {
          /* LSP quantization */
          Q_plsf_3(Enc , st->past_rq, req_mode, lsp_new, lsp_new_q, *anap, &pred_init_i);

          Int_lpc_1to3(Enc_share->shabuf.stE.u2E.s2E.f11,Enc_share->shabuf.stE.u2E.s2E.f22,
			 	Enc_share->shabuf.stE.u2E.s2E.lsp,
			 	st->lsp_old_q, lsp_new_q, azQ);

          /* Advance analysis parameters pointer */
          (*anap) += 3;
       }
   }

   /* update the LSPs for the next frame */
//   Copy (lsp_new, st->lsp_old, M);
//   Copy (lsp_new_q, st->lsp_old_q, M);

   AMR_Copy32((Word32 *)lsp_new, (Word32 *)st->lsp_old, M/2);
   AMR_Copy32((Word32 *)lsp_new_q, (Word32 *)st->lsp_old_q, M/2);

   return 0;
}

#endif

