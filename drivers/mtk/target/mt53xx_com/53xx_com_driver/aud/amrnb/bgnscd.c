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
 *   bgnscd.c
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

/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : bgnscd.c
*      Purpose          : Background noise source charateristic detector (SCD)
*
********************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "bgnscd.h"


//#include <stdlib.h>
//#include <stdio.h>
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
*  Function    : Bgn_scd_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
Word16 Bgn_scd_reset (Bgn_scdState *state)
{

   /* Static vectors to zero */
   Set_zero (state->frameEnergyHist, L_ENERGYHIST);

   /* Initialize hangover handling */
   state->bgHangover = 0;

   return 0;
}

/*
**************************************************************************
*
*  Function    : Bgn_scd
*  Purpose     : Charaterice synthesis speech and detect background noise
*  Returns     : background noise decision; 0 = no bgn, 1 = bgn
*
**************************************************************************
*/
Word16 Bgn_scd (Bgn_scdState *st,      /* i : State variables for bgn SCD */
                Word16 ltpGainHist[],  /* i : LTP gain history            */
                Word16 speech[],       /* o : synthesis speech frame      */
                Word16 *voicedHangover /* o : # of frames after last
                                              voiced frame                */
                )
{
   Word32 s,i,*lptr;
   Word32 prevVoiced, inbgNoise;
   Word32 temp,ltpLimit;
   Word32 currEnergy, noiseFloor, maxEnergy, maxEnergyLastPart;
   Word16 frameEnergyMin,*sptr;

   /* Update the inBackgroundNoise flag (valid for use in next frame if BFI) */
   /* it now works as a energy detector floating on top                      */
   /* not as good as a VAD.                                                  */
   currEnergy = 0;
   s = (Word32) 0;

	lptr = (Word32 *)speech;
	i = L_FRAME/4;
	do
   {
   	temp = *lptr++;
      s = qdadd (s,smulbb(temp, temp));
      s = qdadd (s,smultt(temp, temp));
   	temp = *lptr++;
      s = qdadd (s,smulbb(temp, temp));
      s = qdadd (s,smultt(temp, temp));
   }while(--i != 0);

/*
   for (i = 0; i < L_FRAME; i++)
   {
       s = L_mac (s, speech[i], speech[i]);
   }
*/

   s = L_shl(s, 2);

   currEnergy = (Word16)(s>>16);

   frameEnergyMin = 32767;

   for (i = 0; i < L_ENERGYHIST; i++)
   {
      if(st->frameEnergyHist[i] < frameEnergyMin)
         frameEnergyMin = st->frameEnergyHist[i];
   }

   noiseFloor = sature(frameEnergyMin << 4); /* Frame Energy Margin of 16 */

   maxEnergy = st->frameEnergyHist[0];
	sptr = &(st->frameEnergyHist[1]);
	i = L_ENERGYHIST-5;
	do
   {
      if(maxEnergy < *sptr)
      {
         maxEnergy = *sptr;
      }
		sptr++;
   }while(--i != 0);

   maxEnergyLastPart = st->frameEnergyHist[2*L_ENERGYHIST/3];
	sptr = &(st->frameEnergyHist[2*L_ENERGYHIST/3+1]);
	i = L_ENERGYHIST-(2*L_ENERGYHIST/3+1);
	do
   {
      if(maxEnergyLastPart < *sptr )
      {
         maxEnergyLastPart = *sptr;
      }
		sptr++;
   }while(--i != 0);


   inbgNoise = 0;        /* false */

   /* Do not consider silence as noise */
   /* Do not consider continuous high volume as noise */
   /* Or if the current noise level is very low */
   /* Mark as noise if under current noise limit */
   /* OR if the maximum energy is below the upper limit */

   if ( (maxEnergy > LOWERNOISELIMIT)  &&
        (currEnergy < FRAMEENERGYLIMIT)  &&
        (currEnergy > LOWERNOISELIMIT)  &&
        ( (currEnergy < noiseFloor)  ||
          (maxEnergyLastPart < UPPERNOISELIMIT) ))
   {
      if(st->bgHangover  > 29)
      {
         st->bgHangover = 30;
      } else
      {
         st->bgHangover = (st->bgHangover + 1);
      }
   }
   else
   {
      st->bgHangover = 0;
   }

   /* make final decision about frame state , act somewhat cautiosly */

   if(st->bgHangover > 1)
      inbgNoise = 1;       /* true  */


   for (i = L_ENERGYHIST-2; i >= 0 ; i--)
   {
      st->frameEnergyHist[i] = st->frameEnergyHist[i+1];
   }
   st->frameEnergyHist[L_ENERGYHIST-1] = currEnergy;

   /* prepare for voicing decision; tighten the threshold after some
      time in noise */
   ltpLimit = 13926;             /* 0.85  Q14 */

   if(st->bgHangover > 8)
   {
      ltpLimit = 15565;          /* 0.95  Q14 */
   }

   if(st->bgHangover > 15)
   {
      ltpLimit = 16383;          /* 1.00  Q14 */
   }

   /* weak sort of voicing indication. */
   prevVoiced = 0;        /* false */


   if(gmed_n(&ltpGainHist[4], 5) > ltpLimit)
   {
      prevVoiced = 1;     /* true  */
   }

   if(st->bgHangover > 20)
	{
      if(gmed_n(ltpGainHist, 9) > ltpLimit)
      {
         prevVoiced = 1;  /* true  */
      }
      else
      {
         prevVoiced = 0;  /* false  */
      }
   }

   if (prevVoiced)
   {
      *voicedHangover = 0;
   }
   else
   {
      temp = (*voicedHangover + 1);

      if(temp > 10)
      {
         *voicedHangover = 10;
      }
      else
      {
         *voicedHangover = (Word16)temp;
      }
   }

   return (Word16)inbgNoise;
}

#endif

