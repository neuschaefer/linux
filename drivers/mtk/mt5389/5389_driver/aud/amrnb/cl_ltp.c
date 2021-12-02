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
 *   cl_ltp.c
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
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : cl_ltp.c
*
*****************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "cl_ltp.h"

 
/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "convolve.h"
#include "g_pitch.h"
#include "pred_lt.h"
#include "pitch_fr.h"
#include "q_gain_p.h"
#include "ton_stab.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/

/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/
/*************************************************************************
*
*  Function:   cl_ltp_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/

/*************************************************************************
*
*  Function:   cl_ltp
*  Purpose:    closed-loop fractional pitch search
*
**************************************************************************
*/


int cl_ltp (
   AMR_Encode_FrameState *Enc,
    enum Mode mode,      /* i   : coder mode                                */
    Word16 frameOffset,  /* i   : Offset to subframe                        */
    Word16 T_op[],       /* i   : Open loop pitch lags                      */
    Word16 lsp_flag,     /* i   : LSP resonance flag                        */
    Word16 xn2[],        /* o   : Target vector for codebook search      Q0 */
    Word16 y1[],         /* o   : Filtered adaptive excitation           Q0 */
    Word16 *T0,          /* o   : Pitch delay (integer part)                */
    Word16 *T0_frac,     /* o   : Pitch delay (fractional part)             */
    Word16 *gain_pit,    /* o   : Pitch gain                            Q14 */
    Word16 **anap,       /* o   : Analysis parameters                       */
    Word16 *gp_limit     /* o   : pitch gain limit                          */
)
{
    Word32 i,s,k;
    Word32 L_temp,L_temp1;     /* temporarily variable */
	 Word32 *lptrx , *lptry , *lptre , *lptrr;
    Word16 index;
    Word16 resu3;      /* flag for upsample resolution */
    Word16 gpc_flag;

	 tonStabState *tonStt;
    Word16 *h1t     = Enc->cod_amr_state.h1 ;
    Word16 *exct    = &(Enc->cod_amr_state.exc[frameOffset]);
	 Word16 *xnt     = Enc->share->shabuf.stE.xn;
	 Word16 *g_coeff = Enc->share->shabuf.stE.u0E.gCoeff;
	 Word16 *res2t   = Enc->share->shabuf.stE.res2;
   /*----------------------------------------------------------------------*
    *                 Closed-loop fractional pitch search                  *
    *----------------------------------------------------------------------*/
   *T0 = Pitch_fr(Enc,
   					&(Enc->cod_amr_state.T0_prev_subframe) ,
   					mode,
   					T_op,
                  frameOffset,
                  T0_frac,
                  &resu3,
                  &index);

   *(*anap)++ = index;

   /*-----------------------------------------------------------------*
    *   - find unity gain pitch excitation (adapitve codebook entry)  *
    *     with fractional interpolation.                              *
    *   - find filtered pitch exc. y1[]=exc[] convolve with h1[])     *
    *   - compute pitch gain and limit between 0 and 1.2              *
    *   - update target vector for codebook search                    *
    *   - find LTP residual.                                          *
    *-----------------------------------------------------------------*/

   Pred_lt_3or6(exct, *T0, *T0_frac, resu3);

   Convolve(exct, h1t, y1);

   /* gain_pit is Q14 for all modes */
   *gain_pit = G_pitch(mode, xnt, y1, g_coeff);


   /* check if the pitch gain should be limit due to resonance in LPC filter */
   gpc_flag = 0;
   *gp_limit = MAX_16;

	 tonStt= &(Enc->cod_amr_state.tonStabSt);

   if ( (lsp_flag != 0) && (*gain_pit > GP_CLIP) )
   {
		/* Purpose: Verify that the sum of the last (N_FRAME+1) pitch*/
		Word32 sum;
   	sum = (*gain_pit >> 3);          /* Division by 8 */
	   sum = (sum + tonStt->gp[0]);
	   sum = (sum + tonStt->gp[1]);
	   sum = (sum + tonStt->gp[2]);
	   sum = (sum + tonStt->gp[3]);
	   sum = (sum + tonStt->gp[4]);
	   sum = (sum + tonStt->gp[5]);
	   sum = (sum + tonStt->gp[6]);

	   if (sum > GP_CLIP)
   	{
	      gpc_flag = 1;
   	}
	   else
   	{
	      gpc_flag = 0;
   	}

   }

   /* special for the MR475, MR515 mode; limit the gain to 0.85 to */
   /* cope with bit errors in the decoder in a better way.         */
   if ( (mode == MR475)  ||  (mode == MR515) )
	{
      if (*gain_pit > 13926)
		{
         *gain_pit = 13926;   /* 0.85 in Q14 */
      }

      if (gpc_flag != 0)
		{
          *gp_limit = GP_CLIP;
      }
   }
   else
   {
       if (gpc_flag != 0)
       {
           *gp_limit = GP_CLIP;
           *gain_pit = GP_CLIP;
       }
       /* For MR122, gain_pit is quantized here and not in gainQuant */
       if ( mode == MR122 )
       {
           *(*anap)++ = q_gain_pitch(MR122, *gp_limit, gain_pit,
                                     NULL, NULL);
       }
   }
   /* update target vector und evaluate LTP residual */

	lptrx = (Word32 *) xnt;
	lptry = (Word32 *) y1;
	lptre = (Word32 *) exct;
	lptrr = (Word32 *) res2t;

	i = L_SUBFR/4;
	do
	{
		s = *lptry++;    		k = *lptrx++;
      L_temp = smulbb(s, *gain_pit);
      L_temp1 = smultb(s, *gain_pit);
      *xn2++ = ((Word16)k - (Word16)(L_temp>>14));
      *xn2++ = ((Word16)(k>>16) - (Word16)(L_temp1>>14));

		s = *lptry++;    		k = *lptrx++;
      L_temp = smulbb(s, *gain_pit);
      L_temp1 = smultb(s, *gain_pit);
      *xn2++ = ((Word16)k - (Word16)(L_temp>>14));
      *xn2++ = ((Word16)(k>>16) - (Word16)(L_temp1>>14));

      s = *lptre++;  k = *lptrr++;
      L_temp = smulbb(s, *gain_pit);
      L_temp1 = smultb(s, *gain_pit);
      *res2t++ = ((Word16)k - (Word16)(L_temp>>14));
      *res2t++ = ((Word16)(k>>16) - (Word16)(L_temp1>>14));

      s = *lptre++;  k = *lptrr++;
      L_temp = smulbb(s, *gain_pit);
      L_temp1 = smultb(s, *gain_pit);
      *res2t++ = ((Word16)k - (Word16)(L_temp>>14));
      *res2t++ = ((Word16)(k>>16) - (Word16)(L_temp1>>14));
   }while(--i != 0);

   return 0;

}

#endif

