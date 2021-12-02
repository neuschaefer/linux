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
 *   p_ol_wgh.h
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

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : p_ol_wgh.h
*      Purpose          : Compute the open loop pitch lag with weighting.
*
********************************************************************************
*/
#ifndef p_ol_wgh_h
#define p_ol_wgh_h "$Id $"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "vad.h"

/*
********************************************************************************
*                         DEFINITION OF DATA TYPES
********************************************************************************
*/


/*
********************************************************************************
*                         DECLARATION OF PROTOTYPES
********************************************************************************
*/
#if 0
int p_ol_wgh_init (pitchOLWghtState **st);
/* initialize one instance of the pre processing state.
   Stores pointer to filter status struct in *st. This pointer has to
   be passed to p_ol_wgh in each call.
   returns 0 on success
 */

void p_ol_wgh_exit (pitchOLWghtState **st);
/* de-initialize pre processing state (i.e. free status struct)
   stores NULL in *st
 */
#endif

int p_ol_wgh_reset (pitchOLWghtState *st);
/* reset of pre processing state (i.e. set state memory to zero)
   returns 0 on success
 */


Word32 Pitch_ol_wgh (     /* o   : open loop pitch lag                            */
   AMR_Encode_FrameState *Enc,
   pitchOLWghtState *st, /* i/o : State struct                                   */
   vadState *vadSt,      /* i/o : VAD state struct                               */
   Word16 signal[],      /* i   : signal used to compute the open loop pitch     */
                         /*       signal[-pit_max] to signal[-1] should be known */
   Word16 old_lags[],    /* i   : history with old stored Cl lags                */
   Word16 ol_gain_flg[], /* i   : OL gain flag                                   */
   Word16 idx,           /* i   : index                                          */
   Flag dtx              /* i   : dtx flag; use dtx=1, do not use dtx=0          */
);

#endif
