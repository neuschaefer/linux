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
 *   dec_lag3.c
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
*      File             : dec_lag3.c
*      Purpose          : Decoding of fractional pitch lag with 1/3 resolution.
*                         Extract the integer and fraction parts of the pitch lag from
*                         the received adaptive codebook index.
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "dec_lag3.h"


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

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *   FUNCTION:   Dec_lag3
 *
 *   PURPOSE:  Decoding of fractional pitch lag with 1/3 resolution.
 *             Extract the integer and fraction parts of the pitch lag from
 *             the received adaptive codebook index.
 *
 *    See "Enc_lag3.c" for more details about the encoding procedure.
 *
 *    The fractional lag in 1st and 3rd subframes is encoded with 8 bits
 *    while that in 2nd and 4th subframes is relatively encoded with 4, 5
 *    and 6 bits depending on the mode.
 *
 *************************************************************************/
void Dec_lag3(Word16 index,     /* i : received pitch index                 */
              Word16 t0_min,    /* i : minimum of search range              */
              Word16 t0_max,    /* i : maximum of search range              */
              Word16 i_subfr,   /* i : subframe flag                        */
              Word16 T0_prev,   /* i : integer pitch delay of last subframe
                                       used in 2nd and 4th subframes        */
              Word16 * T0,      /* o : integer part of pitch lag            */
              Word16 * T0_frac, /* o : fractional part of pitch lag         */
              Word16 flag4      /* i : flag for encoding with 4 bits        */
              )
{
   Word32 i;

   if (i_subfr == 0)
   {    /* if 1st or 3rd subframe */

      if(index < 197)
      {
         *T0 = ( (smulbb((index+2),10923)>>15) + 19);
         i = (*T0 + *T0 + *T0);
         *T0_frac = ((index - i) + 58);
      }
      else
      {
         *T0 = (index - 112);
         *T0_frac = 0;
      }
   }
   else
   {    /* 2nd or 4th subframe */
      if (flag4 == 0)
		{
         /* 'normal' decoding: either with 5 or 6 bit resolution */
         i = ( (smulbb((index + 2), 10923)>>15) - 1);
         *T0 = (i + t0_min);

         i = (i + i + i);
         *T0_frac = ((index - 2) - i);
      }
      else
		{
         /* decoding with 4 bit resolution */
 			t0_min = (t0_min + 5);
			t0_max = (t0_max - 4);

         if( T0_prev > t0_min)
            T0_prev = t0_min;

         if ( t0_max > T0_prev)
            T0_prev = t0_max;

         if(index < 4)
         {
            *T0 = ((T0_prev - 5) + index);
            *T0_frac = 0;
         }
         else
         {
            if(index < 12)
            {
               i = ( (smulbb((index-5), 10923)>>15) - 1);
               *T0 = (i + T0_prev);

               i = (i + i + i);
               *T0_frac = ((index - 9)- i);
            }
            else
            {
               *T0 = ( index - 11 + T0_prev);
               *T0_frac = 0;
            }
         }


      } /* end if (decoding with 4 bit resolution) */
   }

   return;
}

#endif

