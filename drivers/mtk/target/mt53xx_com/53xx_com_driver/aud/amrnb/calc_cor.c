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
 *   calc_cor.h
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
*      File             : calc_cor.c
*      Purpose          : Calculate all correlations for prior the OL LTP
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "calc_cor.h"

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
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION: comp_corr
 *
 *  PURPOSE: Calculate all correlations of scal_sig[] in a given delay
 *           range.
 *
 *  DESCRIPTION:
 *      The correlation is given by
 *           cor[t] = <scal_sig[n],scal_sig[n-t]>,  t=lag_min,...,lag_max
 *      The functions outputs the all correlations
 *
 *************************************************************************/
void comp_corr (
    Word16 scal_sig[],  /* i   : scaled signal.                          */
    Word32 L_frame,     /* i   : length of frame to compute pitch        */
    Word32 lag_min,     /* i   : minimum lag                             */
    Word32 corr[])      /* o   : correlation of selected lag             */
{
/*
	Word16 i, j;
	Word16 *p, *p1;
	Word32 t0;

	for (i = lag_max; i >= lag_min; i--)
	{
		p = scal_sig;
		p1 = &scal_sig[-i];
		t0 = 0;

		for (j = 0; j < L_frame; j++)
		{
         t0 = qdadd (t0, smulbb(*p, *p1));
			p++;
			p1++;
      }
      corr[-i] = t0;
	}
*/
   Word32 i, j;
   Word32 t0, t1, t2, t3, t4;
	Word32 *p_32;
   Word32 cur1, cur2, past1, past2;
   Word32 *p , *p1 , *scal_sig_tmp;
	Word16 *p1_16;

   corr -= PIT_MAX;
   scal_sig_tmp = (Word32*)&scal_sig[-PIT_MAX-1];
   i = (PIT_MAX + 1 - PIT_MIN) / 4;
   do {
      t1 = t2 = t3 = t4 = 0;
      p = (Word32*)scal_sig;
      p1 = scal_sig_tmp;
		scal_sig_tmp += 2;
      j = L_frame / 4;
      past1 = *p1++;
      past2 = *p1++;
      do {
         cur1 = *p++;
         cur2 = *p++;
         t1 = smlabt( cur1, past1, t1 );
         t2 = smlabb( cur1, past2, t2 );
         t3 = smlabt( cur1, past2, t3 );
         past1 = *p1++;
         t4 = smlabb( cur1, past1, t4 );

         t1 = smlatb( cur1, past2, t1 );
         t2 = smlatt( cur1, past2, t2 );
         t3 = smlatb( cur1, past1, t3 );
         t4 = smlatt( cur1, past1, t4 );

         t1 = smlabt( cur2, past2, t1 );
         past2 = *p1++;
         t2 = smlabb( cur2, past1, t2 );
         t3 = smlabt( cur2, past1, t3 );
         t4 = smlabb( cur2, past2, t4 );

         t1 = smlatb( cur2, past1, t1 );
         t2 = smlatt( cur2, past1, t2 );
         t3 = smlatb( cur2, past2, t3 );
         t4 = smlatt( cur2, past2, t4 );
      } while( --j != 0 );
      *corr++ = t1<<1;
      *corr++ = t2<<1;
      *corr++ = t3<<1;
      *corr++ = t4<<1;
   } while( --i != 0 );


	if(lag_min == PIT_MIN_MR122)
	{
		i=PIT_MIN-1;
		do
		{
	      p_32 = (Word32*)scal_sig;
		   //p1_16 = &scal_sig[-i];
		   p1_16 = (Word16*)(p_32-i);
		   t0 = 0;
			j=L_frame/4;
			do
			{
  	         t0 =   smlabb(*p_32, *p1_16++ , t0);
  	         t0 =   smlatb(*p_32++, *p1_16++ , t0);
  	         t0 =   smlabb(*p_32, *p1_16++ , t0);
  	         t0 =   smlatb(*p_32++, *p1_16++ , t0);
         }while(--j != 0);

   	   *corr++ = t0 << 1 ;

		}while(--i >= PIT_MIN_MR122);

	}




	return;
}

#endif

