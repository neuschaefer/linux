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
 *   convolve.c
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
*      File             : convolve.c
*      Purpose          : Perform the convolution between two vectors x[]
*                       : and h[] and write the result in the vector y[].
*                       : All vectors are of length L and only the first
*                       : L samples of the convolution are computed.
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "convolve.h"


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
 *
 *  FUNCTION:   Convolve
 *
 *  PURPOSE:
 *     Perform the convolution between two vectors x[] and h[] and
 *     write the result in the vector y[]. All vectors are of length L
 *     and only the first L samples of the convolution are computed.
 *
 *  DESCRIPTION:
 *     The convolution is given by
 *
 *          y[n] = sum_{i=0}^{n} x[i] h[n-i],        n=0,...,L-1
 *
 *************************************************************************/
void Convolve (
    Word16 x[],        /* (i)     : input vector                           */
    Word16 h[],        /* (i)     : impulse response                       */
    Word16 y[]        /* (o)     : output vector                          */
)
{
   Word32 i, n , s;
   Word16 *ptr, *ptr2;

	n=0;
	do
   {
   	i=n;
      ptr = x;
      ptr2 = &h[n];

#if (AMR_OPT_VER == 2)

      s = qdadd(0 , smulbb( *ptr++, *ptr2-- ));
      while( i >= 2 )
      {
      	s = qdadd( s, smulbb(*ptr++, *ptr2--) );
      	s = qdadd( s, smulbb(*ptr++, *ptr2--) );
			i-=2;
      }
      if( i > 0 )
		{
         s = qdadd( s, smulbb( *ptr++, *ptr2-- ) );
      }

#elif (AMR_OPT_VER == 1)

      s = smulbb( *ptr++ , *ptr2-- );
      while( i >= 2 )
      {
      	s = smlabb( *ptr++, *ptr2--, s );
      	s = smlabb( *ptr++, *ptr2--, s );
			i-=2;
      }
      if( i > 0 )
		{
         s = smlabb( *ptr++, *ptr2-- ,s );
      }
		s = qadd(s,s);

#endif

      *y++ = (Word16)( L_shl (s, 3) >> 16);

   }while(++n < L_SUBFR);

   return;
}

#endif

