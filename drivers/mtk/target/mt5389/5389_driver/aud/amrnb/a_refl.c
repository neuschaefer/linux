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
 *   a_refl.c
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
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "a_refl.h"

 

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"

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
/*
**************************************************************************
*
*  Function    : A_Refl
*
**************************************************************************
*/
void A_Refl(
	Word16 *aState_t,
	Word16 *bState_t,
   Word16 a[],	      /* i   : Directform coefficients */
   Word16 refl[]      /* o   : Reflection coefficients */
)
{
   /* local variables */
   Word32 i,j;
   Word32 L_acc;
   Word32 L_temp;
   Word32 normShift;
   Word32 scale;

//   Word16 aState[M];
//   Word16 bState[M];
   Word16 *aState = aState_t;
   Word16 *bState = bState_t;

   Word16 normProd;
   Word16 temp;
   Word16 mult;
	Word16 *sptr,*sptrs;

   /* initialize states */
/*
   for (i = 0; i < M; i++)
   {
      aState[i] = a[i];
   }
*/
	sptr = a;
	sptrs = aState;
	i = M/2;
	do
	{
		*sptrs++ = *sptr++;
		*sptrs++ = *sptr++;
	}while(--i != 0);

	sptr = &(refl[M-1]);

   /* backward Levinson recursion */
   for (i = M-1; i >= 0; i--)
   {
      if(abs_s(aState[i]) >= 4096)
      {
//         goto ExitRefl;
		   for (i = 0; i < M; i++)
		   {
		      refl[i] = 0;
		   }
			return;
      }

      *sptr = sature(aState[i] << 3);

      L_temp = smulbb(*sptr , *sptr );
		L_temp = qadd(L_temp,L_temp);
      L_acc = qsub(MAX_32, L_temp);

      normShift = norm_l(L_acc);
      scale = (15 - normShift);

      L_acc = (L_acc << normShift);
      normProd = (Word16)(qadd(L_acc,0x8000) >> 16);

      mult = div_s(16384, normProd);

      for (j = 0; j < i; j++)
      {
         L_acc = (Word32)(aState[j] << 16);
         L_acc = qdsub(L_acc, smulbb(*sptr , aState[i-j-1]));

         temp = (Word16)(qadd(L_acc,0x8000) >> 16);
         L_temp = smulbb(mult, temp);
			L_temp = qadd(L_temp,L_temp);
         L_temp = L_shr_r(L_temp, scale);

         if(L_abs(L_temp) > 32767)
         {
//            goto ExitRefl;
			   for (i = 0; i < M; i++)
			   {
			      refl[i] = 0;
			   }
				return;
         }

         bState[j] = (Word16)(L_temp);
      }

      for (j = 0; j < i; j++)
      {
         aState[j] = bState[j];
      }

		sptr--;

   }

   return;

}

#endif

