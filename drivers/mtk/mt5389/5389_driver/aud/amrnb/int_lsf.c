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
 *   int_lsf.c
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
*      File             : int_lsf.c
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "int_lsf.h"
 
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
*--------------------------------------*
* Constants (defined in cnst.h)        *
*--------------------------------------*
*  M         : LSF order               *
*--------------------------------------*
*/
 
/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*
**************************************************************************
*
*  Function    : Int_lsf
*  Purpose     : Interpolates the LSFs for selected subframe
*  Description : The 20 ms speech frame is divided into 4 subframes.
*                The LSFs are interpolated at the 1st, 2nd and 3rd
*                subframe and only forwarded at the 4th subframe.
*
*                      |------|------|------|------|
*                         sf1    sf2    sf3    sf4
*                   F0                          F1
*      
*                 sf1:   3/4 F0 + 1/4 F1         sf3:   1/4 F0 + 3/4 F1
*                 sf2:   1/2 F0 + 1/2 F1         sf4:       F1
*  Returns     : void
*
**************************************************************************
*/
void Int_lsf(
    Word16 lsf_old[], /* i : LSF vector at the 4th SF of past frame          */
    Word16 lsf_new[], /* i : LSF vector at the 4th SF of present frame       */
    Word16 i_subfr,   /* i : Pointer to current sf (equal to 0,40,80 or 120) */
    Word16 lsf_out[]  /* o : interpolated LSF parameters for current sf      */
)
{
   Word32 i;
   Word32 *lptr_out,*lptr_old,*lptr_new,s,t;
	Word16 *sptr_out;
   
   if ( i_subfr == 0 )
   {        
      
		sptr_out = lsf_out;
		lptr_old = (Word32 *)lsf_old;		
		lptr_new = (Word32 *)lsf_new;
		i=M/2;
		do
      {
      	t = *lptr_old++;	s = *lptr_new++;
         *sptr_out++ = ( ((Word16) t      - ((Word16)t >> 2)) + ( (Word16)s >> 2 ));
         *sptr_out++ = (Word16)( ( (Word16)(t>>16) - ( (t>>18) )) + ((s>>18)) );			
      }while(--i != 0);
		
/*
      for (i = 0; i < M; i++) 
      {
         lsf_out[i] = ( (lsf_old[i] - (lsf_old[i] >> 2)) + (lsf_new[i] >> 2) );
      }
*/		
   }
   else if(i_subfr == 40)
   {
   
		sptr_out = lsf_out;
		lptr_old = (Word32 *)lsf_old;		
		lptr_new = (Word32 *)lsf_new;
		i=M/2;
		do
      {
      	t = *lptr_old++;	s = *lptr_new++;
         *sptr_out++ = ( ((Word16)t >> 1)  + ( (Word16)s >> 1 ));
         *sptr_out++ = (Word16)( (t>>17) + (s>>17) );			
      }while(--i != 0);
		

/*	
      for (i = 0; i < M; i++) 
      {
         lsf_out[i] = ( (lsf_old[i] >> 1) + (lsf_new[i] >> 1) );
      }
*/		
   }
   else if(i_subfr == 80)
   {
      for (i = 0; i < M; i++) 
      {
         lsf_out[i] = ( (lsf_old[i] >> 2) + (lsf_new[i] - (lsf_new[i] >> 2) ) );
         
      }
   }
   else if(i_subfr == 120)
   {         

		lptr_out = (Word32 *)lsf_out;
		lptr_new = (Word32 *)lsf_new;

		*lptr_out++ = *lptr_new++;
		*lptr_out++ = *lptr_new++;
		*lptr_out++ = *lptr_new++;
		*lptr_out++ = *lptr_new++;
		*lptr_out++ = *lptr_new++;

/*		
      for (i = 0; i < M; i++) 
      {
         lsf_out[i] = lsf_new[i];                       
      }
*/		
   }
   
   return;
}

#endif

