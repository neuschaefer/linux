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
 *   cod_amr.h
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
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : cod_amr.h
*      Purpose          : Main encoder routine operating on a frame basis.
*
*****************************************************************************
*/
#ifndef cod_amr_h
#define cod_amr_h "$Id $"

/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/
#include "typedef.h"
#include "lpc.h"
#include "lsp.h"
#include "cl_ltp.h"
#include "gain_q.h"
#include "p_ol_wgh.h"
#include "ton_stab.h"
#include "vad.h"
#include "dtx_enc.h"

/*
*****************************************************************************
*                         DEFINITION OF DATA TYPES
*****************************************************************************
*/

/*
********************************************************************************
*                         DECLARATION OF PROTOTYPES
********************************************************************************
*/
#if 0
/*
**************************************************************************
*
*  Function    : cod_amr_init
*  Purpose     : Allocates memory and initializes state variables
*  Description : Stores pointer to filter status struct in *st. This
*                pointer has to be passed to cod_amr in each call.
 *                 - initilize pointers to speech buffer
 *                 - initialize static  pointers
 *                 - set static vectors to zero
*  Returns     : 0 on success
*
**************************************************************************
*/
int cod_amr_init (cod_amrState **st, Flag dtx);

/*
**************************************************************************
*
*  Function    : cod_amr_exit
*  Purpose     : The memory used for state memory is freed
*  Description : Stores NULL in *st
*
**************************************************************************
*/
void cod_amr_exit (cod_amrState **st);

#endif

/*
**************************************************************************
*
*  Function    : cod_amr_reset
*  Purpose     : Resets state memory
*  Returns     : 0 on success
*
**************************************************************************
*/
int cod_amr_reset (cod_amrState *st);

/***************************************************************************
 *   FUNCTION:   cod_amr_first
 *
 *   PURPOSE:  Copes with look-ahead.
 *
 *   INPUTS:
 *       No input argument are passed to this function. However, before
 *       calling this function, 40 new speech data should be copied to the
 *       vector new_speech[]. This is a global pointer which is declared in
 *       this file (it points to the end of speech buffer minus 200).
 *
 ***************************************************************************/

#if 0
int cod_amr_first(cod_amrState *st,     /* i/o : State struct            */
                  Word16 new_speech[]   /* i   : speech input (L_FRAME)  */
);
#endif

/***************************************************************************
 *   FUNCTION:   cod_amr
 *
 *   PURPOSE:  Main encoder routine.
 *
 *   DESCRIPTION: This function is called every 20 ms speech frame,
 *       operating on the newly read 160 speech samples. It performs the
 *       principle encoding functions to produce the set of encoded parameters
 *       which include the LSP, adaptive codebook, and fixed codebook
 *       quantization indices (addresses and gains).
 *
 *   INPUTS:
 *       No input argument are passed to this function. However, before
 *       calling this function, 160 new speech data should be copied to the
 *       vector new_speech[]. This is a global pointer which is declared in
 *       this file (it points to the end of speech buffer minus 160).
 *
 *   OUTPUTS:
 *
 *       ana[]:     vector of analysis parameters.
 *       synth[]:   Local synthesis speech (for debugging purposes)
 *
 ***************************************************************************/

int cod_amr(
				AMR_Encode_FrameState *Enc,
				cod_amrState *st,         /* i/o : State struct                 */
            enum Mode mode,           /* i   : AMR mode                     */
            Word16 new_speech[],      /* i   : speech input (L_FRAME)       */
            Word16 ana[],             /* o   : Analysis parameters          */
            enum Mode *usedMode,      /* o   : used mode                    */
            Word16 synth[]            /* o   : Local synthesis              */
);

#endif


