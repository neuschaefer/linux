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
 *   residu.c
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
*      File             : residu.c
*      Purpose          : Computes the LP residual.
*      Description      : The LP residual is computed by filtering the input
*                       : speech through the LP inverse filter A(z).
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "residu.h"

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
* Constants (defined in cnst.h         *
*--------------------------------------*
*  M         : LPC order               *
*--------------------------------------*
*/

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
void Residu (
    Word16 a[], /* (i)     : prediction coefficients                      */
    Word16 x[], /* (i)     : speech signal                                */
    Word16 y[] /* (o)     : residual signal                              */
)
{
   Word32 i;
   Word32 s1,t2;
	Word32 *at,*xt;
	register Word32 a0,a1,a2,a3,a4,a5;

	xt = (Word32 *)x;
	at = (Word32 *)a;
	a0 = at[0];
	a1 = at[1];
	a2 = at[2];
	a3 = at[3];
	a4 = at[4];
	a5 = at[5];

#if (AMR_OPT_VER == 2)

	i = L_SUBFR/2;
   xt -= 5;
   do
	{
      t2 = smulbt( a0,  xt[5] ); 		t2 = qadd(t2,t2);
      s1 = smulbb( a0,  xt[5] );			s1 = qadd(s1,s1);
      t2 = qdadd(t2 ,smultb( a0,  xt[5] ));
      s1 = qdadd(s1 ,smultt( a0,  xt[4] ));
      t2 = qdadd(t2 ,smulbt( a1,  xt[4] ));
      s1 = qdadd(s1 ,smulbb( a1,  xt[4] ));
      t2 = qdadd(t2 ,smultb( a1,  xt[4] ));
      s1 = qdadd(s1 ,smultt( a1,  xt[3] ));
      t2 = qdadd(t2 ,smulbt( a2,  xt[3] ));
      s1 = qdadd(s1 ,smulbb( a2,  xt[3] ));
      t2 = qdadd(t2 ,smultb( a2,  xt[3] ));
      s1 = qdadd(s1 ,smultt( a2,  xt[2] ));
      t2 = qdadd(t2 ,smulbt( a3,  xt[2] ));
      s1 = qdadd(s1 ,smulbb( a3,  xt[2] ));
      t2 = qdadd(t2 ,smultb( a3,  xt[2] ));
      s1 = qdadd(s1 ,smultt( a3,  xt[1] ));
      t2 = qdadd(t2 ,smulbt( a4,  xt[1] ));
      s1 = qdadd(s1 ,smulbb( a4,  xt[1] ));
      t2 = qdadd(t2 ,smultb( a4,  xt[1] ));
      s1 = qdadd(s1 ,smultt( a4,  xt[0] ));
      t2 = qdadd(t2 ,smulbt( a5,  xt[0] ));
      s1 = qdadd(s1 ,smulbb( a5,  xt[0] ));
//      *y++ = round (L_shl (s1, 3));   
//      *y++ = round (L_shl (t2, 3)); 

//      *y++ = (Word16)(qadd((L_shl (s1, 3)),0x8000) >> 16);
//      *y++ = (Word16)(qadd((L_shl (t2, 3)),0x8000) >> 16);
      *y++ = (Word16)(qadd(s1,(1<<12)) >> 13);
      *y++ = (Word16)(qadd(t2,(1<<12)) >> 13);

      xt++;
   } while( --i > 0 );

#elif (AMR_OPT_VER == 1)

	{
	   Word32 s11,t22;

		i = L_SUBFR/2;
	   xt -= 5;
   	do
		{
   	   t2 = smulbt( a0,  xt[5] );
      	s1 = smulbb( a0,  xt[5] );
		   t2 = smlatb( a0,  xt[5], t2 );
	      s1 = smlatt( a0,  xt[4], s1 );
   	   t2 = smlabt( a1,  xt[4], t2 );
      	s1 = smlabb( a1,  xt[4], s1 );
	      t2 = smlatb( a1,  xt[4], t2 );
   	   s1 = smlatt( a1,  xt[3], s1 );
      	t2 = smlabt( a2,  xt[3], t2 );
	      s1 = smlabb( a2,  xt[3], s1 );

   	   t22 = smultb( a2,  xt[3]);
      	s11 = smultt( a2,  xt[2]);
	      t22 = smlabt( a3,  xt[2], t22 );
   	   s11 = smlabb( a3,  xt[2], s11 );
      	t22 = smlatb( a3,  xt[2], t22 );
	      s11 = smlatt( a3,  xt[1], s11 );
   	   t22 = smlabt( a4,  xt[1], t22 );
      	s11 = smlabb( a4,  xt[1], s11 );
	      t22 = smlatb( a4,  xt[1], t22 );
   	   s11 = smlatt( a4,  xt[0], s11 );
      	t22 = smlabt( a5,  xt[0], t22 );
	      s11 = smlabb( a5,  xt[0], s11 );

			s1 = qadd(s1,s11);
			t2 = qadd(t2,t22);

//	      *y++ = round (L_shl (s1, 4));   
//   	   *y++ = round (L_shl (t2, 4)); 

//	      *y++ = (Word16)(qadd((L_shl (s1, 4)),0x8000)>>16);
//   	   *y++ = (Word16)(qadd((L_shl (t2, 4)),0x8000)>>16);

	      *y++ = (Word16)(qadd(s1,(1<<11)) >> 12);
   	   *y++ = (Word16)(qadd(t2,(1<<11)) >> 12);

      	xt++;
	   } while( --i > 0 );
	}
#endif



}

#endif

