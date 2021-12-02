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
 *   post_pro.c
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
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 * 12 20 2010 scholar.chang
 * [WCPSP00000557] [SWIP]Compile warning removed.
 * Finish the amr SWIP compile warning removal.
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
*      File             : post_pro.c
*      Purpose          : Postprocessing of output speech.
*
*                         - 2nd order high pass filtering with cut
*                           off frequency at 60 Hz.
*                         - Multiplication of output by two.
*
********************************************************************************
*/

 
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "post_pro.h"
 
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
//filter coefficients (fc = 60 Hz)
static const Word16 b[3] = {7699, -15398, 7699};
static const Word16 a[3] = {8192, 15836, -7667};
*/

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
*
*  Function:   Post_Process_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/

int Post_Process_reset (Post_ProcessState *posstate)
{
   posstate->y2 = 0;
   posstate->y1 = 0;
   posstate->x0 = 0;
   posstate->x1 = 0;

   return 0;
}

/*************************************************************************
 *
 *  FUNCTION:  Post_Process()
 *
 *  PURPOSE: Postprocessing of input speech.
 *
 *  DESCRIPTION:
 *     - 2nd order high pass filtering with cut off frequency at 60 Hz.
 *     - Multiplication of output by two.
 *
 * Algorithm:
 *
 *  y[i] = b[0]*x[i]*2 + b[1]*x[i-1]*2 + b[2]*x[i-2]*2
 *                     + a[1]*y[i-1]   + a[2]*y[i-2];
 *
 *
 *************************************************************************/
int Post_Process (
    Post_ProcessState *posstate,  /* i/o : post process state                   */
    Word16 signal[]        /* i/o : signal                               */
    )
{
	Word32 i, x2,t;
	Word32 L_tmp,L_tmp1;
	Word32 x0_t = posstate->x0, x1_t = posstate->x1, y1_t = posstate->y1, y2_t = posstate->y2;
   const Word32 coeffa = (Word32)((unsigned)(15836) + ((unsigned)((-7667)& 0xffff) << 16));
   const Word32 coeffb = (Word32)((unsigned)(7699) + ((unsigned)((-15398)& 0xffff) << 16));
	Word16 *sig = signal;

	i=L_FRAME/2;
	do
   {
		x2   = x1_t;
		x1_t = x0_t;
		x0_t = *sig;
      /*  y[i] = b[0]*x[i]*2 + b[1]*x[i-1]*2 + b140[2]*x[i-2]/2  */
      /*                     + a[1]*y[i-1] + a[2] * y[i-2];      */
      L_tmp = smulwb(y1_t, coeffa);
		L_tmp = qadd(L_tmp,L_tmp);

		t = smulwt(y2_t, coeffa);
		t = qadd(t,t);
      L_tmp = qadd(L_tmp, t );


		t = smulwt(y1_t, coeffa);
		t = qadd(t,t);


      L_tmp = qdadd(L_tmp,smulbb(x0_t, coeffb));
      L_tmp = qdadd(L_tmp,smulbt(x1_t, coeffb));
      L_tmp = qdadd(L_tmp,smulbb(x2, 7699));

      y2_t = y1_t;
      y1_t = L_shl(L_tmp, 2);
      *sig++ = (Word16)(qadd(L_shl(y1_t, 1),1<<15)>>16);

      L_tmp1 = smulwb(y1_t, coeffa);
		L_tmp1 = qadd(L_tmp1,L_tmp1);

      L_tmp1 = qadd(L_tmp1, t);

      L_tmp1 = qdadd(L_tmp1,smulbt(x0_t, coeffb));
      L_tmp1 = qdadd(L_tmp1,smulbb(x1_t, 7699));
		x2   = x1_t;
		x1_t = x0_t;
		x0_t = *sig;
      L_tmp1 = qdadd(L_tmp1,smulbb(x0_t, coeffb));

      y2_t = y1_t;
      y1_t = L_shl(L_tmp1, 2);
      *sig++ = (Word16)(qadd(L_shl(y1_t, 1),1<<15)>>16);


   }while(--i != 0);

	posstate->x0 = x0_t;
	posstate->x1 = x1_t;
	posstate->y1 = y1_t;
	posstate->y2 = y2_t;
   return 0;
}

#if 0
int Post_Process (
    Post_ProcessState *posstate,  /* i/o : post process state                   */
    Word16 signal[]        /* i/o : signal                               */
    )
{
	Word32 i, x2,t;
	Word32 L_tmp;
	Word32 x0_t = posstate->x0, x1_t = posstate->x1, y1_t = posstate->y1, y2_t = posstate->y2;
   const Word32 coeffa = (unsigned)(15836) + ((unsigned)((-7667)& 0xffff) << 16);
   const Word32 coeffb = (unsigned)(7699) + ((unsigned)((-15398)& 0xffff) << 16);
	Word16 *sig = signal;

	i=L_FRAME;
	do
   {
		x2   = x1_t;
		x1_t = x0_t;
		x0_t = *sig;
      /*  y[i] = b[0]*x[i]*2 + b[1]*x[i-1]*2 + b140[2]*x[i-2]/2  */
      /*                     + a[1]*y[i-1] + a[2] * y[i-2];      */
      L_tmp = smulwb(y1_t, coeffa);
		L_tmp = qadd(L_tmp,L_tmp);

		t = smulwt(y2_t, coeffa);
		t = qadd(t,t);
      L_tmp = qadd(L_tmp, t );

      L_tmp = qdadd(L_tmp,smulbb(x0_t, coeffb));
      L_tmp = qdadd(L_tmp,smulbt(x1_t, coeffb));

      L_tmp = qdadd(L_tmp,smulbb(x2, 7699));

      y2_t = y1_t;
      y1_t = L_shl(L_tmp, 2);
      *sig++ = (Word16)(qadd(L_shl(y1_t, 1),1<<15)>>16);

   }while(--i != 0);

	posstate->x0 = x0_t;
	posstate->x1 = x1_t;
	posstate->y1 = y1_t;
	posstate->y2 = y2_t;
   return 0;
}
#endif

#endif

