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
 *   lsp_avg.h
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
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : lsp_avg.c
*      Purpose:         : LSP averaging and history
*
*****************************************************************************
*/


/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "lsp_avg.h"
 
/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "basic_op.h"
#include "oper_32b.h"
#include "copy.h"

#include "tab_lsf_5.h"


/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/
/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/

/*
**************************************************************************
*
*  Function    : lsp_avg_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int lsp_avg_reset (lsp_avgState *st)
{

  Copy(mean_lsf, &st->lsp_meanSave[0], M);

  return 0;
}

/*
**************************************************************************
*
*  Function    : lsp_avg
*  Purpose     : Calculate the LSP averages
*
**************************************************************************
*/

void lsp_avg (
    lsp_avgState *st,         /* i/o : State struct                 Q15 */
    Word16 *lsp               /* i   : state of the state machine   Q15 */
)
{
	Word32 i;
   Word32 L_tmp;            /* Q31 */
	Word16 *sptr;

	sptr = (st->lsp_meanSave);
	i = M/2;
	do
 	{
   	/* mean = 0.84*mean */
      L_tmp = (Word32)(*sptr << 16);
      L_tmp = qdsub(L_tmp,smulbb(EXPCONST, *sptr));
      /* Add 0.16 of newest LSPs to mean */
      L_tmp = qdadd(L_tmp,smulbb(EXPCONST, *lsp++));
      /* Save means */
      *sptr++ = (Word16)(qadd(L_tmp,0x8000)>>16);            /* Q15 */

   	/* mean = 0.84*mean */
      L_tmp = (Word32)(*sptr << 16);
      L_tmp = qdsub(L_tmp,smulbb(EXPCONST, *sptr));
      /* Add 0.16 of newest LSPs to mean */
      L_tmp = qdadd(L_tmp,smulbb(EXPCONST, *lsp++));
      /* Save means */
      *sptr++ = (Word16)(qadd(L_tmp,0x8000)>>16);            /* Q15 */
	}while(--i != 0);

   return;
}

#endif

