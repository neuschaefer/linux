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
 *   ex_ctrl.c
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

/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : ex_ctrl.c
*      Purpose          : Excitation Control module in background noise
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "ex_ctrl.h"

#include "typedef.h"
#include "basic_op.h"
#include "copy.h"
#include "set_zero.h"
#include "gmed_n.h"
#include "sqrt_l.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
/*-----------------------------------------------------------------*
 *   Decoder constant parameters (defined in "cnst.h")             *
 *-----------------------------------------------------------------*
 *   L_FRAME       : Frame size.                                   *
 *   L_SUBFR       : Sub-frame size.                               *
 *-----------------------------------------------------------------*/

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*
**************************************************************************
*
*  Function    : Ex_ctrl
*  Purpose     : Charaterice synthesis speech and detect background noise
*  Returns     : background noise decision; 0 = no bgn, 1 = bgn
*
**************************************************************************
*/
Word16 Ex_ctrl (Word16 excitation[],   /*i/o: Current subframe excitation   */
                Word16 excEnergy,      /* i : Exc. Energy, sqrt(totEx*totEx)*/
                Word16 exEnergyHist[], /* i : History of subframe energies  */
                Word16 voicedHangover, /* i : # of fr. after last voiced fr.*/
                Word16 prevBFI,        /* i : Set i previous BFI            */
                Word16 carefulFlag     /* i : Restrict dymamic in scaling   */
                )
{
   Word32 i, exp,t0;
   Word32 testEnergy, scaleFactor, avgEnergy, prevEnergy;
	Word16 *sptr;

   /* get target level */
   avgEnergy = gmed_n(exEnergyHist, 9);

   prevEnergy = (  (exEnergyHist[7] + exEnergyHist[8]) >> 1);

   if(exEnergyHist[8] < prevEnergy)
   {
      prevEnergy = exEnergyHist[8];
   }

   /* upscaling to avoid too rapid energy rises  for some cases */

   if ( (excEnergy < avgEnergy)  && (excEnergy > 5) )
   {
      testEnergy = sature(prevEnergy << 2);  /* testEnergy = 4*prevEnergy; */

      if ( (voicedHangover < 7)  || prevBFI != 0 )
      {
         /* testEnergy = 3*prevEnergy */
         testEnergy = (testEnergy - prevEnergy);
      }

      if(avgEnergy > testEnergy)
      {
         avgEnergy = testEnergy;
      }

      /* scaleFactor=avgEnergy/excEnergy in Q0 (const 29 below)*/
      exp = norm_s (excEnergy);
      excEnergy = (excEnergy << exp);
      excEnergy = div_s ((Word16) 16383, excEnergy);
      t0 = smulbb(avgEnergy, excEnergy);
		t0 = qadd(t0,t0);
      t0 = L_shr (t0, (20 - exp));  /* const=30 for t0 in Q0, 20 for Q10 */

      if ( t0 > 32767)
      {
         t0 = 32767;   /* saturate  */
      }
      scaleFactor = (Word16)(t0);

      /* test if scaleFactor > 3.0 */

      if ( carefulFlag != 0 && (scaleFactor > 3072)  )
      {
         scaleFactor = 3072;
      }

      /* scale the excitation by scaleFactor */

		sptr = excitation;
		i = L_SUBFR/2;
		do
      {
         t0 = smulbb(scaleFactor, *sptr);
			t0 = qadd(t0,t0);
         t0 = (t0 >> 11);
         *sptr++ = (Word16)(t0);
         t0 = smulbb(scaleFactor, *sptr);
			t0 = qadd(t0,t0);
         t0 = (t0 >> 11);
         *sptr++ = (Word16)(t0);
      }while(--i != 0);
/*
      for (i = 0; i < L_SUBFR; i++)
      {
         t0 = smulbb(scaleFactor, excitation[i]);
			t0 = qadd(t0,t0);
         t0 = (t0 >> 11);
         excitation[i] = (Word16)(t0);
      }
*/
   }

   return 0;

}

#endif

