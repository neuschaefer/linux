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
 *   d2_11pf.c
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
*      File             : d2_11pf.c
*      Purpose          : Algebraic codebook decoder
*
********************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "d2_11pf.h"

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
#define NB_PULSE 2           /* number of pulses */

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  decode_2i40_11bits (decod_ACELP())
 *
 *  PURPOSE:   Algebraic codebook decoder
 *
 *************************************************************************/

void decode_2i40_11bits(
    AMR_Decode_FrameState *Dec,
    Word16 sign,   /* i : signs of 2 pulses.                       */
    Word16 index,  /* i : Positions of the 2 pulses.               */
    Word16 cod[]   /* o : algebraic (fixed) codebook excitation    */
)
{
   Word32 i, j;
   Word32 *lptr;
//   Word16 pos[NB_PULSE];
	AMR_Shared_Buf *Dec_share = Dec->share;
   Word16 *pos = Dec_share->shabuf.stD.u2D.s1D.pos;


   /* Decode the positions */
   j = index & 1;
   index = (index >> 1);
   i = index & 7;

   i = (i + sature(i << 2));        /* pos0 =i*5+1+j*2 */
   i = (i + 1);
   j = sature(j << 1);
   pos[0] = (i + j);

   index = (index >> 3);
   j = index & 3;
   index = (index >> 2);
   i = index & 7;

   if(j == 3)
   {
      i = (i + sature(i << 2));        /* pos1 =i*5+4 */
      pos[1] = (i + 4);
   }
   else
   {
      i = (i + sature(i << 2));        /* pos1 =i*5+j */
      pos[1] = (i + j);
   }

   /* decode the signs  and build the codeword */
   lptr = (Word32 *)cod;
   i = L_SUBFR/8;
   do
   {
      *lptr++ = 0;   	*lptr++ = 0;   	*lptr++ = 0;   	*lptr++ = 0;
   }while(--i != 0);


   for (j = 0; j < NB_PULSE; j++)
   {
      i = sign & 1;
      sign = (sign >> 1);

      if (i != 0) {
         cod[pos[j]] = 8191;                           /* +1.0 */
      } else {
         cod[pos[j]] = -8192;                          /* -1.0 */
      }
   }

   return;
}

#endif

