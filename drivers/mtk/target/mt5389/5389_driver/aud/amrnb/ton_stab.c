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
 *   ton_stab.c
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
*      File             : ton_stab.c
*
*****************************************************************************
*/

/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "ton_stab.h"

 
/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "set_zero.h"
#include "copy.h"

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
/*************************************************************************
 *
 *  Function:   ton_stab_init
 *  Purpose:    Allocates state memory and initializes state memory
 *
 **************************************************************************
 */

/*************************************************************************
 *
 *  Function:   ton_stab_reset
 *  Purpose:    Initializes state memory to zero
 *
 **************************************************************************
 */
int ton_stab_reset (tonStabState *st)
{

    /* initialize tone stabilizer state */
    st->count = 0;
    Set_zero(st->gp, N_FRAME);    /* Init Gp_Clipping */

    return 0;
}

/***************************************************************************
 *                                                                          *
 *  Function:  check_lsp()                                                  *
 *  Purpose:   Check the LSP's to detect resonances                         *
 *                                                                          *
 ****************************************************************************
 */
Word16 check_lsp(tonStabState *st, /* i/o : State struct            */
                 Word16 *lsp       /* i   : unquantized LSP's       */
)
{
   Word32 i;
	Word16 dist, dist_min1, dist_min2, dist_th;

   /* Check for a resonance:                             */
   /* Find minimum distance between lsp[i] and lsp[i+1]  */

   dist_min1 = MAX_16;
   for (i = 3; i < M-2; i++)
   {
      dist = (lsp[i] - lsp[i+1]);

      if(dist < dist_min1)
      {
         dist_min1 = dist;
      }

   }

   dist_min2 = MAX_16;
   for (i = 1; i < 3; i++)
   {
      dist = (lsp[i] - lsp[i+1]);

      if(dist < dist_min2)
      {
         dist_min2 = dist;
      }

   }

   if (lsp[1] > 32000)
   {
      dist_th = 600;
   }
   else if(lsp[1] > 30500)
   {
      dist_th = 800;
   }
   else
   {
      dist_th = 1100;
   }


   if ( (dist_min1 < 1500) || (dist_min2 < dist_th) )
   {
      st->count = (st->count + 1);
   }
   else
   {
      st->count = 0;
   }

   /* Need 12 consecutive frames to set the flag */

   if(st->count >= 12)
   {
      st->count = 12;
      return 1;
   }
   else
   {
      return 0;
   }

}


/***************************************************************************
 *
 *  Function:  Update_Gp_Clipping()
 *  Purpose:   Update past pitch gain memory
 *
 ***************************************************************************
 */

void update_gp_clipping(tonStabState *st, /* i/o : State struct            */
                        Word16 g_pitch    /* i   : pitch gain              */
)
{
//   Copy(&st->gp[1], &st->gp[0], N_FRAME-1);
	st->gp[0] = st->gp[1];
	st->gp[1] = st->gp[2];
	st->gp[2] = st->gp[3];
	st->gp[3] = st->gp[4];
	st->gp[4] = st->gp[5];
	st->gp[5] = st->gp[6];
   st->gp[6] = (g_pitch >> 3);
}

#endif



