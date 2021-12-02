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
 *   pre_proc.c
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
*      File             : pre_proc.c
*      Purpose          : Preprocessing of input speech.
*
********************************************************************************
*/


/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "pre_proc.h"
/*
const char pre_proc_id[] = "@(#)$Id $" pre_proc_h;
*/

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
/* filter coefficients (fc = 80 Hz, coeff. b[] is divided by 2) */
/*
static const Word16 b[3] = {1899, -3798, 1899};
static const Word16 a[3] = {4096, 7807, -3733};
*/


/*
********************************************************************************
*                         PRIVATE DATA
********************************************************************************
*/


/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
*
*  Function:   Pre_Process_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/

int Pre_Process_reset (PreState *prestate)
{
  prestate->y1 = 0;
  prestate->y2 = 0;
  prestate->x0 = 0;
  prestate->x1 = 0;

  return 0;
}

/*************************************************************************
*
*  Function:   Pre_Process_exit
*  Purpose:    The memory used for state memory is freed
*
**************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  Pre_Process()
 *
 *  PURPOSE: Preprocessing of input speech.
 *
 *  DESCRIPTION:
 *     - 2nd order high pass filtering with cut off frequency at 80 Hz.
 *     - Divide input by two.
 *
 *
 * Algorithm:
 *
 *  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b[2]*x[i-2]/2
 *                     + a[1]*y[i-1]   + a[2]*y[i-2];
 *
 *
 *  Input is divided by two in the filtering process.
 *
 *************************************************************************/
int Pre_Process (
	PreState *prestate,
	Word16 signal[]) /* input/output signal */

{
	Word32 i, x2,t;
	Word32 L_tmp,L_tmp1;
	Word32 x0_t = prestate->x0, x1_t = prestate->x1, y1_t = prestate->y1, y2_t = prestate->y2;
   const Word32 coeffa = (Word32)((unsigned)(7807) + ((unsigned)((-3733)& 0xffff) << 16));
   const Word32 coeffb = (Word32)((unsigned)(1899) + ((unsigned)((-3798)& 0xffff) << 16));
	Word16 *sig = signal;

	i=L_FRAME/2;
	do
	{
		x2   = x1_t;
		x1_t = x0_t;
		x0_t = *sig;
      /*  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b140[2]*x[i-2]/2  */
      /*                     + a[1]*y[i-1] + a[2] * y[i-2];      */
      L_tmp = smulwb( y1_t, coeffa );
		L_tmp = qadd(L_tmp,L_tmp);

		t = smulwt(y2_t, coeffa);
		t = qadd(t,t);
      L_tmp = qadd( L_tmp, t );

		t = smulwt(y1_t, coeffa);
		t = qadd(t,t);

      L_tmp = qdadd(L_tmp, smulbb(x0_t, coeffb));
      L_tmp = qdadd(L_tmp, smulbt(x1_t, coeffb));
      L_tmp = qdadd(L_tmp, smulbb(x2, 1899));

      y2_t = y1_t;
      y1_t = L_shl (L_tmp, 3);
      *sig++ = (Word16)(qadd(y1_t, 1<<15) >> 16);

      L_tmp1 = smulwb(y1_t, coeffa);
		L_tmp1 = qadd(L_tmp1,L_tmp1);

      L_tmp1 = qadd(L_tmp1, t);

      L_tmp1 = qdadd(L_tmp1,smulbt(x0_t, coeffb));
      L_tmp1 = qdadd(L_tmp1,smulbb(x1_t, 1899));

		x2   = x1_t;
		x1_t = x0_t;
		x0_t = *sig;
      L_tmp1 = qdadd(L_tmp1,smulbb(x0_t, coeffb));

      y2_t = y1_t;
      y1_t = L_shl (L_tmp1, 3);
      *sig++ = (Word16)(qadd(y1_t, 1<<15) >> 16);

	}while(--i != 0);

	prestate->x0 = x0_t;
	prestate->x1 = x1_t;
	prestate->y1 = y1_t;
	prestate->y2 = y2_t;

	return 0;

}


#if 0
int Pre_Process (
	PreState *prestate,
	Word16 signal[]) /* input/output signal */

{
	Word32 i, x2,t;
	Word32 L_tmp;
	Word32 x0_t = prestate->x0, x1_t = prestate->x1, y1_t = prestate->y1, y2_t = prestate->y2;
   const Word32 coeffa = (unsigned)(7807) + ((unsigned)((-3733)& 0xffff) << 16);
   const Word32 coeffb = (unsigned)(1899) + ((unsigned)((-3798)& 0xffff) << 16);
	Word16 *sig = signal;

	i=L_FRAME;
	do
	{
		x2   = x1_t;
		x1_t = x0_t;
		x0_t = *sig;
      /*  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b140[2]*x[i-2]/2  */
      /*                     + a[1]*y[i-1] + a[2] * y[i-2];      */

//      L_tmp = DPF_Mult16( y1_t, 7807 );
//      L_tmp = qadd( L_tmp, DPF_Mult16( y2_t, -3733 ) );
      L_tmp = smulwb( y1_t, coeffa );
		L_tmp = qadd(L_tmp,L_tmp);

		t = smulwt(y2_t, coeffa);
		t = qadd(t,t);
      L_tmp = qadd( L_tmp, t );

//      L_tmp = qdadd(L_tmp, smulbb(x0_t, 1899));
//      L_tmp = qdadd(L_tmp, smulbb(x1_t, -3798));
      L_tmp = qdadd(L_tmp, smulbb(x0_t, coeffb));
      L_tmp = qdadd(L_tmp, smulbt(x1_t, coeffb));

      L_tmp = qdadd(L_tmp, smulbb(x2, 1899));

      y2_t = y1_t;
      y1_t = L_shl (L_tmp, 3);
      *sig++ = (Word16)(qadd(y1_t, 1<<15) >> 16);
	}while(--i != 0);

	prestate->x0 = x0_t;
	prestate->x1 = x1_t;
	prestate->y1 = y1_t;
	prestate->y2 = y2_t;

	return 0;

}
#endif

#endif

