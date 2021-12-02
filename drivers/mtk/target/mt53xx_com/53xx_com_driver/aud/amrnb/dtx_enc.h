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
 *   dtx_enc.h
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
*      File             : dtx_enc.h
*      Purpose          : DTX mode computation of SID parameters
*
********************************************************************************
*/
#ifndef dtx_enc_h
#define dtx_enc_h "$Id $"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "q_plsf.h"
#include "gc_pred.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/


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
/*
**************************************************************************
*  Function    : dtx_enc_init
*  Purpose     : Allocates memory and initializes state variables
*  Description : Stores pointer to filter status struct in *st. This
*                pointer has to be passed to dtx_enc in each call.
*  Returns     : 0 on success
*
**************************************************************************
*/
int dtx_enc_init (dtx_encState **st);

/*
**************************************************************************
*
*  Function    : dtx_enc_reset
*  Purpose     : Resets state memory
*  Returns     : 0 on success
*
**************************************************************************
*/
int dtx_enc_reset (dtx_encState *st);

/*
**************************************************************************
*
*  Function    : dtx_enc_exit
*  Purpose     : The memory used for state memory is freed
*  Description : Stores NULL in *st
*
**************************************************************************
*/
void dtx_enc_exit (dtx_encState **st);

/*
**************************************************************************
*
*  Function    : dtx_enc
*  Purpose     :
*  Description :
*
**************************************************************************
*/
int dtx_enc(
			   AMR_Encode_FrameState *Enc,
				dtx_encState *st,        /* i/o : State struct                    */
            Word16 computeSidFlag,   	/* i   : compute SID                     */
	    		Word16 past_rq[],        	/* i/o : Qunatizer state struct          */

            gc_predState* predState, 	/* i/o : State struct                    */
	    		Word16 **anap            	/* o   : analysis parameters             */
	    );

/*
**************************************************************************
*
*  Function    : dtx_buffer
*  Purpose     : handles the DTX buffer
*
**************************************************************************
*/
int dtx_buffer(dtx_encState *st,   /* i/o : State struct                    */
	       Word16 lsp_new[],   /* i   : LSP vector                      */
	       Word16 speech[]     /* i   : speech samples                  */
	       );

/*
**************************************************************************
*
*  Function    : tx_dtx_handler
*  Purpose     : adds extra speech hangover to analyze speech on the decoding side.
*  Description : returns 1 when a new SID analysis may be made
*                otherwise it adds the appropriate hangover after a sequence
*                with out updates of SID parameters .
*
**************************************************************************
*/
Word16 tx_dtx_handler(dtx_encState *st,       /* i/o : State struct          */
                      Word16 vadFlag,         /* i   : vad control variable  */
                      enum Mode *usedMode     /* o   : mode changed or not   */
                      );

#endif
