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
 *   dec_amr.h
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
*      File             : dec_amr.h
*      Purpose          : Speech decoder routine.
*
*****************************************************************************
*/
#ifndef dec_amr_h
#define dec_amr_h "$Id $"

/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/
#include "typedef.h"
#include "dtx_dec.h"
#include "d_plsf.h"
#include "gc_pred.h"
#include "ec_gains.h"
#include "ph_disp.h"
#include "c_g_aver.h"
#include "bgnscd.h"
#include "lsp_avg.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/
/*---------------------------------------------------------------*
 *    Postfilter constant parameters (defined in "cnst.h")       *
 *---------------------------------------------------------------*
 *   L_FRAME     : Frame size.                                   *
 *   PIT_MAX     : Maximum pitch lag.                            *
 *   L_INTERPOL  : Length of filter for interpolation.           *
 *   M           : LPC order.                                    *
 *---------------------------------------------------------------*/


#if 0
/*
**************************************************************************
*
*  Function    : Decoder_amr_init
*  Purpose     : Allocates initializes state memory
*  Description : Stores pointer to filter status struct in *st. This
*                pointer has to be passed to Decoder_amr in each call.
*  Returns     : 0 on success
*
**************************************************************************
*/
int Decoder_amr_init (Decoder_amrState **st);

/*
**************************************************************************
*
*  Function    : Decoder_amr_exit
*  Purpose     : The memory used for state memory is freed
*  Description : Stores NULL in *s
*  Returns     : void
*
**************************************************************************
*/
void Decoder_amr_exit (Decoder_amrState **st);
#endif



/*
**************************************************************************
*
*  Function    : Decoder_amr_reset
*  Purpose     : Resets state memory
*  Returns     : 0 on success
*
**************************************************************************
*/
int Decoder_amr_reset (Decoder_amrState *st,enum Mode mode);


/*
**************************************************************************
*
*  Function    : Decoder_amr
*  Purpose     : Speech decoder routine.
*  Returns     : 0
*
**************************************************************************
*/
int Decoder_amr (
    AMR_Decode_FrameState *Dec,
    Decoder_amrState *st,  /* i/o : State variables                       */
    enum Mode mode,        /* i   : AMR mode                              */
    Word16 parm[],         /* i   : vector of synthesis parameters
                                    (PRM_SIZE)                            */
    enum RXFrameType frame_type, /* i   : received frame type               */
    Word16 synth[],        /* o   : synthesis speech (L_FRAME)            */
    Word16 A_t[]           /* o   : decoded LP filter in 4 subframes
                                    (AZ_SIZE)                             */
);

#endif
