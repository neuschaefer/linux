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
 *   hp_max.c
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
*      File             : hp_max.c
*      Purpose          : Find the maximum correlation of scal_sig[] in a given
*                         delay range.
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "hp_max.h"

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
Word16 hp_max (  
    Word32 corr[],      /* i   : correlation vector.                      */
    Word16 scal_sig[],  /* i   : scaled signal.                           */
    Word32 L_frame,     /* i   : length of frame to compute pitch         */
    Word32 lag_max,     /* i   : maximum lag                              */
    Word32 lag_min,     /* i   : minimum lag                              */
    Word16 *cor_hp_max) /* o   : max high-pass filtered norm. correlation */
{
   Word32 i;
   Word32 max, t0, t1;	 
   Word32 s, *lptr,*p, *p1;
   Word16 *p2;
   Word16 max16, t016, cor_max;
   Word16 shift, shift1, shift2;
   
   max = MIN_32;               
   t0 = 0L;                      

	p  = corr - lag_max+1;  
	p1 = p -1;
	lptr = p +1;

   for (i = lag_max-1; i > lag_min; i--)
   {
      /* high-pass filtering */
      t0 = qsub (qsub(L_shl(*p, 1), *p1), *lptr);   
		p++; p1++; lptr++;
		
      t0 = L_abs (t0);      
      if(t0 >= max)
      {
         max = t0;             
      }
   }
   
   /* compute energy */
   p2 = &scal_sig[-1];          
   lptr = (Word32 *)scal_sig;
   
   t0 = 0L;                    
   t1 = 0L; 	    
   i = L_frame/2;	
   do
   {
      s = *lptr++;
      t0 = qdadd(t0,smulbb( s, s));
      t0 = qdadd(t0,smultt( s, s));
		
      t1 = qdadd(t1,smulbb( s, *p2));	p2++;	
      t1 = qdadd(t1,smultb( s, s));  p2++;				 
   }while(--i != 0);

   /* high-pass filtering */
   t0 = qsub(L_shl(t0, 1), L_shl(t1, 1));
   t0 = L_abs (t0);
   
   /* max/t0 */
   shift1 = (norm_l(max)- 1);                 
   max16  = (Word16)((max << shift1)>>16);       
   shift2 = norm_l(t0);                          
   t016 =  (Word16)((t0 << shift2)>>16);         
   
   if (t016 != 0)
   {
      cor_max = div_s(max16, t016);              
   }
   else
   {
      cor_max = 0;                                
   }
   
   shift = (shift1 - shift2);       
   
   
   if (shift >= 0)
   {
      *cor_hp_max = (cor_max >> shift);           /* Q15 */
   }
   else
   {
      *cor_hp_max = sature(cor_max << negate(shift));   /* Q15 */
   }
   
   return 0;
}

#endif

