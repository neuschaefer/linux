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
 *   inter_36.c
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
*      File             : inter_36.c
*      Purpose          : Interpolating the normalized correlation
*                       : with 1/3 or 1/6 resolution.
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "inter_36.h"
 
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
#define UP_SAMP_MAX  6

//#include "inter_36.tab"


#define FIR_SIZE     (UP_SAMP_MAX*L_INTER_SRCH+1)
/* 1/6 resolution interpolation filter  (-3 dB at 3600 Hz) */

/* 
Note: The IS641 (7.4) 1/3 resolution filter is simply a subsampled         
version of the 1/6 resolution filter, i.e. it uses         
every second coefficient:                  
inter_3[k] = inter_6[2*k], 0 <= k <= 3*L_INTER_SRCH */

static const Word16 inter_6[FIR_SIZE] =
{    
	29519,    
	28316, 24906, 19838, 13896, 7945, 2755,    
	-1127, -3459, -4304, -3969, -2899, -1561,    
	-336, 534, 970, 1023, 823, 516,    
	220, 0, -131, -194, -215, 0
};

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  Interpol_3or6()
 *
 *  PURPOSE:  Interpolating the normalized correlation with 1/3 or 1/6
 *            resolution.
 *
 *************************************************************************/
Word16 Interpol_3or6 (  /* o : interpolated value                        */
    Word16 *x,          /* i : input vector                              */
    Word32 frac,        /* i : fraction  (-2..2 for 3*, -3..3 for 6*)    */
    Word32 flag3        /* i : if set, upsampling rate = 3 (6 otherwise) */
)
{
   Word32 s;
   Word16 *x1;
   const Word16 *c1, *c2;


	if (flag3 != 0)
   {
		frac = (frac << 1);   /* inter_3[k] = inter_6[2*k] -> k' = 2*k */
   }
    
   if (frac < 0)
   {
   	frac = (frac + UP_SAMP_MAX);
		x--;
   }

#if (AMR_OPT_VER == 2)	   
   x1 = x;                                               
   c1 = &inter_6[frac];                
   s = 0;    

   s = qdadd (0, smulbb(*x1-- , c1[0]));
   s = qdadd (s, smulbb(*x1--, c1[6]));
   s = qdadd (s, smulbb(*x1--, c1[12]));
   s = qdadd (s, smulbb(*x1, c1[18]));

   x1 = x; 		
   c2 = &inter_6[ (UP_SAMP_MAX - frac) ]; 	
   s = qdadd (s, smulbb(*++x1, c2[0]));	 
   s = qdadd (s, smulbb(*++x1, c2[6]));	 
   s = qdadd (s, smulbb(*++x1, c2[12])); 
   s = qdadd (s, smulbb(*++x1, c2[18]));	  
#elif (AMR_OPT_VER == 1)

   x1 = x;                                               
   c1 = &inter_6[frac];                
   s = 0;    

   s = smulbb(*x1--, c1[0]);
   s = smlabb(*x1--, c1[6] ,s);
   s = smlabb(*x1--, c1[12],s);
   s = smlabb(*x1  , c1[18],s);

   x1 = x; 		
   c2 = &inter_6[ (UP_SAMP_MAX - frac) ]; 	
   s = smlabb(*++x1, c2[0],s);	 
   s = smlabb(*++x1, c2[6],s);	 
   s = smlabb(*++x1, c2[12],s); 
   s = smlabb(*++x1, c2[18],s);	
	s = qadd(s,s);
#endif 

   return (Word16)(qadd(s,0x8000) >> 16);
	
}

#endif

