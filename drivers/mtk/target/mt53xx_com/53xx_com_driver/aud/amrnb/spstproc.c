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
 *   spstproc.c
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
*      File             : subframePostProc.c
*      Purpose          : Subframe post processing
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "spstproc.h"


/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "syn_filt.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
int subframePostProc(
   AMR_Encode_FrameState *Enc,
    Word16 *speech,   /* i   : speech segment                        */
    enum Mode mode,   /* i   : coder mode                            */
    Word16 i_subfr,   /* i   : Subframe nr                           */
    Word16 gain_pit,  /* i   : Pitch gain                       Q14  */
    Word16 gain_code, /* i   : Decoded innovation gain               */
    Word16 *Aq,       /* i   : A(z) quantized for the 4 subframes    */
    Word16 synth[],   /* i   : Local snthesis                        */
    Word16 xn[],      /* i   : Target vector for pitch search        */
    Word16 code[],    /* i   : Fixed codebook exitation              */
    Word16 y1[],      /* i   : Filtered adaptive exitation           */
    Word16 y2[],      /* i   : Filtered fixed codebook excitation    */
    Word16 *mem_syn,  /* i/o : memory of synthesis filter            */
    Word16 *mem_err,  /* o   : pointer to error signal               */
    Word16 *mem_w0,   /* o   : memory of weighting filter            */
    Word16 *exc,      /* o   : long term prediction residual         */
    Word16 *sharp     /* o   : pitch sharpening value                */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
	Word32 i, k;
   Word32 L_temp;
	Word32 *ptr32c,t;
   Word32 tempShift;
   Word32 kShift;
   Word32 pitch_fac;
   Word32 *tmp_w = Enc_share->shabuf.stE.u2E.s4E.tmp_w;
   Word16 temp;
	Word16 *ptr16e,*ptr16y,*ptr16x,*ptr16y1,*ptr16y2;


   if(mode != MR122)
   {
      tempShift = 1;
      kShift = 2;
      pitch_fac = gain_pit;
   }
   else
   {
      tempShift = 2;
      kShift = 4;
      pitch_fac = (gain_pit >> 1);
   }

   /*------------------------------------------------------------*
    * - Update pitch sharpening "sharp" with quantized gain_pit  *
    *------------------------------------------------------------*/
   *sharp = gain_pit;

   if(*sharp > SHARPMAX)
   {
      *sharp = SHARPMAX;
   }
   /*------------------------------------------------------*
    * - Find the total excitation                          *
    * - find synthesis speech corresponding to exc[]       *
    * - update filters memories for finding the target     *
    *   vector in the next subframe                        *
    *   (update error[-m..-1] and mem_w_err[])             *
    *------------------------------------------------------*/

	ptr16e = &(exc[i_subfr]);
	ptr32c = (Word32 *)code;

	i = L_SUBFR/2;
	do
	{
		t = *ptr32c++;
      L_temp    = smulbb(*ptr16e, pitch_fac);
      L_temp    = qadd(L_temp,L_temp);
      L_temp    = qdadd(L_temp, smulbb(t, gain_code));
      *ptr16e++ = (Word16)( (qadd(L_temp, 1<<(15-tempShift))  >> (16-tempShift)) );
      L_temp    = smulbb(*ptr16e, pitch_fac);  L_temp = qadd(L_temp,L_temp);
      L_temp    = qdadd(L_temp, smultb(t, gain_code));
      *ptr16e++ = (Word16)( (qadd(L_temp, 1<<(15-tempShift))  >> (16-tempShift)) );
   }while(-- i != 0);


   Syn_filt(tmp_w,Aq, &exc[i_subfr], &synth[i_subfr], L_SUBFR, mem_syn, 1);


	ptr16e = &(speech[i_subfr + L_SUBFR - M]);
	ptr16y = &(synth[i_subfr + L_SUBFR - M]);
	ptr16x = &(xn[L_SUBFR - M]);;
	ptr16y1= &(y1[L_SUBFR - M]);
	ptr16y2= &(y2[L_SUBFR - M]);;
	i = M;
	do
	{
		*mem_err++ = (*ptr16e++ - *ptr16y++);
      temp = (Word16)(L_shl(smulbb(*ptr16y1++, gain_pit), 2) >> 16);
      k = (Word16)(L_shl(smulbb(*ptr16y2++, gain_code), (kShift+1)) >> 16);
      *mem_w0++ = (*ptr16x++ - (temp + k));
   }while(--i != 0);


   return 0;
}

#endif

