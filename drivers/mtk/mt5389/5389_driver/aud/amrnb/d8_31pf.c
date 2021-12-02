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
 *   d8_31pf.c
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
*      File             : d8_31pf.c
*      Purpose          : Builds the innovative codevector
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "d8_31pf.h"
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define NB_PULSE  8           /* number of pulses  */

/* define values/representation for output codevector and sign */
#define POS_CODE  8191
#define NEG_CODE  8191

static void decompress10 (
   Word16 MSBs,        /* i : MSB part of the index                 */
   Word16 LSBs,        /* i : LSB part of the index                 */
   Word16 index1,      /* i : index for first pos in pos_index[]    */
   Word16 index2,      /* i : index for second pos in pos_index[]   */
   Word16 index3,      /* i : index for third pos in pos_index[]    */
   Word16 pos_indx[])  /* o : position of 3 pulses (decompressed)   */
{
   Word32 ia, ib, ic;

   /*
   pos_indx[index1] = ((MSBs-25*(MSBs/25))%5)*2 + (LSBs-4*(LSBs/4))%2;
   pos_indx[index2] = ((MSBs-25*(MSBs/25))/5)*2 + (LSBs-4*(LSBs/4))/2;
   pos_indx[index3] = (MSBs/25)*2 + LSBs/4;
   */

   if(MSBs > 124)
   {
      MSBs = 124;
   }

   ia = smulbb(MSBs, 1311) >> 15;

	ic = smulbb(ia, 25);
	ic = qadd(ic,ic);
   ia = (MSBs - (Word16)( (ic >> 1)));

	ic = smulbb( (smulbb(ia, 6554)>>15), 5);
	ic = qadd(ic,ic);
   ib = sature( (ia - (Word16)(ic >> 1) ) << 1);

   ic = sature( (LSBs >> 2) << 2);
   ic = (LSBs - ic);
   pos_indx[index1] = (ib + (ic & 1));

   ib = sature( (smulbb(ia, 6554)>>15) << 1);
   pos_indx[index2] = (ib + (ic >> 1));

   pos_indx[index3] = (sature( (smulbb(MSBs, 1311)>>15) << 1) + (LSBs>> 2) );

   return;
}

/*************************************************************************
 *
 *  FUNCTION:  decompress_code()
 *
 *  PURPOSE: decompression of the linear codewords to 4+three indeces
 *           one bit from each pulse is made robust to errors by
 *           minimizing the phase shift of a bit error.
 *           4 signs (one for each track)
 *           i0,i4,i1 => one index (7+3) bits, 3   LSBs more robust
 *           i2,i6,i5 => one index (7+3) bits, 3   LSBs more robust
 *           i3,i7    => one index (5+2) bits, 2-3 LSbs more robust
 *
 *************************************************************************/
static void decompress_code (
    Word16 indx[],      /* i : position and sign of 8 pulses (compressed) */
    Word16 sign_indx[], /* o : signs of 4 pulses (signs only)             */
    Word16 pos_indx[]   /* o : position index of 8 pulses (position only) */
)
{
   Word32 i, ia, ib;
	Word16 MSBs, LSBs, MSBs0_24;

   sign_indx[0] = indx[0];
   sign_indx[1] = indx[1];
   sign_indx[2] = indx[2];
   sign_indx[3] = indx[3];

   /*
   First index: 10x10x10 -> 2x5x2x5x2x5-> 125x2x2x2 -> 7+1x3 bits
   MSBs = indx[NB_TRACK]/8;
   LSBs = indx[NB_TRACK]%8;
   */
   MSBs = (indx[NB_TRACK_MR102] >> 3);
   LSBs = indx[NB_TRACK_MR102] & 7;
   decompress10 (MSBs, LSBs, 0, 4, 1, pos_indx);

   /*
   Second index: 10x10x10 -> 2x5x2x5x2x5-> 125x2x2x2 -> 7+1x3 bits
   MSBs = indx[NB_TRACK+1]/8;
   LSBs = indx[NB_TRACK+1]%8;
   */
   MSBs = (indx[NB_TRACK_MR102+1] >> 3);
   LSBs = indx[NB_TRACK_MR102+1] & 7;
   decompress10 (MSBs, LSBs, 2, 6, 5, pos_indx);

   /*
   Third index: 10x10 -> 2x5x2x5-> 25x2x2 -> 5+1x2 bits
   MSBs = indx[NB_TRACK+2]/4;
   LSBs = indx[NB_TRACK+2]%4;
   MSBs0_24 = (MSBs*25+12)/32;
   if ((MSBs0_24/5)%2==1)
   pos_indx[3] = (4-(MSBs0_24%5))*2 + LSBs%2;
   else
   pos_indx[3] = (MSBs0_24%5)*2 + LSBs%2;
   pos_indx[7] = (MSBs0_24/5)*2 + LSBs/2;
   */
   MSBs = (indx[NB_TRACK_MR102+2] >> 2);
   LSBs = indx[NB_TRACK_MR102+2] & 3;


	i = smulbb(MSBs, 25);
	i = qadd(i,i);
   MSBs0_24 = (((Word16)( (i >> 1)) + 12) >> 5);

   ia = (smulbb(MSBs0_24, 6554)>>15) & 1;

	i = smulbb( (smulbb(MSBs0_24, 6554)>>15), 5);
	i = qadd(i,i);
   ib = (MSBs0_24 - (Word16)(i >> 1) );

   if(ia == 1)
   {
      ib = (4 - ib);
   }
   pos_indx[3] = (sature(ib << 1) + (LSBs & 1));

   ia = sature( (smulbb(MSBs0_24, 6554)>>15) << 1);
   pos_indx[7] = (ia + (LSBs >> 1));
}

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:   dec_8i40_31bits()
 *
 *  PURPOSE:  Builds the innovative codevector from the received
 *            index of algebraic codebook.
 *
 *   See  c8_31pf.c  for more details about the algebraic codebook structure.
 *
 *************************************************************************/

void dec_8i40_31bits (
    AMR_Decode_FrameState *Dec,
    Word16 index[],    /* i : index of 8 pulses (sign+position)         */
    Word16 cod[]       /* o : algebraic (fixed) codebook excitation     */
)
{
   Word32 i, j, pos1, pos2, sign;
   Word32 *lptr,t;
//   Word16 linear_signs[NB_TRACK_MR102];
//   Word16 linear_codewords[NB_PULSE];
	AMR_Shared_Buf *Dec_share = Dec->share;
   Word16 *linear_signs     = Dec_share->shabuf.stD.u2D.s1D.linear_signs;
   Word16 *linear_codewords = Dec_share->shabuf.stD.u2D.s1D.linear_codewords;

   lptr = (Word32 *)cod;
   i = L_CODE/8;
   do
   {
      *lptr++ = 0;   	*lptr++ = 0;   	*lptr++ = 0;   	*lptr++ = 0;
   }while(--i != 0);

   decompress_code (index, linear_signs, linear_codewords);

   /* decode the positions and signs of pulses and build the codeword */
   for (j = 0; j < NB_TRACK_MR102; j++)
   {
      /* compute index i */
      i = linear_codewords[j];
		t = smulbb(i, 4); t = qadd(t,t);
      i = (Word16)(t >> 1);
      pos1 = (i + j);   /* position of pulse "j" */

      if (linear_signs[j] == 0)
      {
         sign = POS_CODE;                              /* +1.0 */
      }
      else
      {
         sign = -NEG_CODE;                             /* -1.0 */
      }
      cod[pos1] = sign;

      /* compute index i */
      i = linear_codewords[(j + 4)];
		t = smulbb(i, 4); t = qadd(t,t);
      i = (Word16)(t >> 1);
      pos2 = (i + j);      /* position of pulse "j+4" */

      if(pos2 < pos1)
      {
         sign = -(sign);
      }
      cod[pos2] = (cod[pos2] + sign);

   }

   return;
}

#endif

