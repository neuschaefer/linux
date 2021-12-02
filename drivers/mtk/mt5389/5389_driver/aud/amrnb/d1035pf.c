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
 *   d1035pf.c
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
*      File             : d1035pf.c
*      Purpose          : Builds the innovative codevector
*
********************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION
 
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "d1035pf.h"

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
#define NB_PULSE  10            /* number of pulses  */

#include "tab_gray.h"

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:   dec_10i40_35bits()
 *
 *  PURPOSE:  Builds the innovative codevector from the received
 *            index of algebraic codebook.
 *
 *   See  c1035pf.c  for more details about the algebraic codebook structure.
 *
 *************************************************************************/
void dec_10i40_35bits (
    Word16 index[],    /* (i)     : index of 10 pulses (sign+position)       */
    Word16 cod[]       /* (o)     : algebraic (fixed) codebook excitation    */
)
{
   Word32 i, j, pos1, pos2, sign, tmp;
   Word32 *lptr,t;

   lptr = (Word32 *)cod;
   i = L_CODE/8;
   do
   {
      *lptr++ = 0;   	*lptr++ = 0;   	*lptr++ = 0;   	*lptr++ = 0;
   }while(--i != 0);
/*
   for (i = 0; i < L_CODE; i++)
   {
      cod[i] = 0;
   }
*/

   /* decode the positions and signs of pulses and build the codeword */
   for (j = 0; j < NB_TRACK; j++)
   {
      /* compute index i */
      tmp = index[j];
      i = tmp & 7;
      i = dgray[i];

		t = smulbb(i, 5); t = qadd(t,t);
      i = (Word16)(t >> 1);
      pos1 = (i + j); /* position of pulse "j" */
      i = (tmp >> 3) & 1;

      if (i == 0)
      {
         sign = 4096;                                 /* +1.0 */
      }
      else
      {
         sign = -4096;                                /* -1.0 */
      }

      cod[pos1] = sign;

      /* compute index i */
      i = index[(j + 5)] & 7;
      i = dgray[i];

		t = smulbb(i, 5); t = qadd(t,t);
      i = (Word16)(t >> 1);

      pos2 = (i + j);      /* position of pulse "j+5" */

      if(pos2 < pos1)
      {
         sign = -(sign);
      }
      cod[pos2] = (cod[pos2] + sign);
   }

   return;
}

#endif

