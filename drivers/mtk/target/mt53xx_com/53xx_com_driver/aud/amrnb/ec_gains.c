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
 *   ec_gains.c
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
*      File             : ec_gains.c
*      Purpose:         : Error concealment for pitch and codebook gains
*
********************************************************************************
*/


/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "ec_gains.h"
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "gmed_n.h"
#include "gc_pred.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#include "tab_gains.h"

static const Word16 cdown[7] =
{
	32767, 32112, 32112, 32112, 32112, 32112, 22937
};

static const Word16 pdown[7] =
{
32767, 32112, 32112, 26214,9830, 6553, 6553
};

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/


/*
**************************************************************************
*
*  Function    : ec_gain_code_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int ec_gain_code_reset (ec_gain_codeState *state)
{

   state->gbuf[0] = 1;
   state->gbuf[1] = 1;
   state->gbuf[2] = 1;
   state->gbuf[3] = 1;
   state->gbuf[4] = 1;

   state->past_gain_code = 0;
   state->prev_gc = 1;

   return 0;
}


/*
**************************************************************************
*
*  Function    : ec_gain_code
*  Purpose     : conceal the codebook gain
*                Call this function only in BFI (instead of normal gain
*                decoding function)
*
**************************************************************************
*/
void ec_gain_code (
    ec_gain_codeState *st,    /* i/o : State struct                     */
    gc_predState *pred_state, /* i/o : MA predictor state               */
    Word16 state,             /* i   : state of the state machine       */
    Word16 *gain_code         /* o   : decoded innovation gain          */
)
{
   Word16 tmp;
   Word16 qua_ener_MR122;
   Word16 qua_ener;

   /* calculate median of last five gain values */
   tmp = gmed_n(st->gbuf,5);

   /* new gain = minimum(median, past_gain) * cdown[state] */

   if(tmp > st->past_gain_code)
   {
      tmp = st->past_gain_code;
   }
   tmp = (Word16)smulbb(tmp, cdown[state]) >> 15;
   *gain_code = tmp;

   /* update table of past quantized energies with average of
   * current values
   */
   gc_pred_average_limited(pred_state, &qua_ener_MR122, &qua_ener);
   gc_pred_update(pred_state, qua_ener_MR122, qua_ener);
}

/*
**************************************************************************
*
*  Function    : ec_gain_code_update
*  Purpose     : update the codebook gain concealment state;
*                limit gain_code if the previous frame was bad
*                Call this function always after decoding (or concealing)
*                the gain
*
**************************************************************************
*/
void ec_gain_code_update (
    ec_gain_codeState *st,    /* i/o : State struct                     */
    Word16 bfi,               /* i   : flag: frame is bad               */
    Word16 prev_bf,           /* i   : flag: previous frame was bad     */
    Word16 *gain_code         /* i/o : decoded innovation gain          */
)
{
   /* limit gain_code by previous good gain if previous frame was bad */
   if (bfi == 0)
   {
      if (prev_bf != 0)
      {
         if(*gain_code > st->prev_gc)
         {
            *gain_code = st->prev_gc;
         }
      }
      st->prev_gc = *gain_code;
   }

   /* update EC states: previous gain, gain buffer */
   st->past_gain_code = *gain_code;

   st->gbuf[0] = st->gbuf[1];
   st->gbuf[1] = st->gbuf[2];
   st->gbuf[2] = st->gbuf[3];
   st->gbuf[3] = st->gbuf[4];

   st->gbuf[4] = *gain_code;

   return;
}


/*
**************************************************************************
*
*  Function:   ec_gain_pitch_reset
*  Purpose:    Resets state memory
*
**************************************************************************
*/
int ec_gain_pitch_reset (ec_gain_pitchState *state)
{

   state->pbuf[0] = 1640;
   state->pbuf[1] = 1640;
   state->pbuf[2] = 1640;
   state->pbuf[3] = 1640;
   state->pbuf[4] = 1640;

   state->past_gain_pit = 0;
   state->prev_gp = 16384;

   return 0;
}

/*
**************************************************************************
*
*  Function    : ec_gain_pitch
*  Purpose     : conceal the pitch gain
*                Call this function only in BFI (instead of normal gain
*                decoding function)
*
**************************************************************************
*/
void ec_gain_pitch (
    ec_gain_pitchState *st, /* i/o : state variables                   */
    Word16 state,           /* i   : state of the state machine        */
    Word16 *gain_pitch      /* o   : pitch gain (Q14)                  */
)
{
   Word32 tmp;

   /* calculate median of last five gains */
   tmp = gmed_n (st->pbuf, 5);

   /* new gain = minimum(median, past_gain) * pdown[state] */
   if(tmp > st->past_gain_pit)
   {
      tmp = st->past_gain_pit;
   }

   *gain_pitch = (Word16)(smulbb(tmp, pdown[state])>>15);

}


/*
**************************************************************************
*
*  Function    : ec_gain_pitch_update
*  Purpose     : update the pitch gain concealment state;
*                limit gain_pitch if the previous frame was bad
*                Call this function always after decoding (or concealing)
*                the gain
*
**************************************************************************
*/
void ec_gain_pitch_update (
    ec_gain_pitchState *st, /* i/o : state variables                   */
    Word16 bfi,             /* i   : flag: frame is bad                */
    Word16 prev_bf,         /* i   : flag: previous frame was bad      */
    Word16 *gain_pitch      /* i/o : pitch gain                        */
)
{

   if (bfi == 0)
   {
      if (prev_bf != 0)
      {
         if(*gain_pitch > st->prev_gp)
         {
            *gain_pitch = st->prev_gp;
         }
      }
      st->prev_gp = *gain_pitch;
   }

   st->past_gain_pit = *gain_pitch;

   if(st->past_gain_pit > 16384)   /* if (st->past_gain_pit > 1.0) */
   {
      st->past_gain_pit = 16384;
   }

   st->pbuf[0] = st->pbuf[1];
   st->pbuf[1] = st->pbuf[2];
   st->pbuf[2] = st->pbuf[3];
   st->pbuf[3] = st->pbuf[4];

   st->pbuf[4] = st->past_gain_pit;

}

#endif

