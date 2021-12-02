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
 *   b_cn_cod.h
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
*      File             : b_cn_cod.h
*      Purpose          : Contains function for comfort noise generation.
*
********************************************************************************
*/
#ifndef b_cn_cod_h
#define b_cn_cod_h "$Id $"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"

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

/*************************************************************************
 *
 *   FUNCTION NAME: pseudonoise
 *
 *   PURPOSE: Generate a random integer value to use in comfort noise
 *            generation. The algorithm uses polynomial x^31 + x^3 + 1
 *            (length of PN sequence is 2^31 - 1).
 *
 *   INPUTS:      *shift_reg    Old CN generator shift register state
 *
 *
 *   OUTPUTS:     *shift_reg    Updated CN generator shift register state
 *
 *   RETURN VALUE: Generated random integer value
 *
 *************************************************************************/

Word16 pseudonoise (
    Word32 *shift_reg, /* i/o : Old CN generator shift register state */
    Word16 no_bits     /* i   : Number of bits                        */
);

/*************************************************************************
 *
 *   FUNCTION NAME: build_CN_code
 *
 *   PURPOSE: Compute the comfort noise fixed codebook excitation. The
 *            gains of the pulses are always +/-1.
 *
 *   INPUTS:      *seed         Old CN generator shift register state
 *
 *   OUTPUTS:     cod[0..39]    Generated comfort noise fixed codebook vector
 *                *seed         Updated CN generator shift register state
 *
 *   RETURN VALUE: none
 *
 *************************************************************************/
void build_CN_code (
    Word32 *seed,         /* i/o : Old CN generator shift register state */
    Word16 cod[]          /* o   : Generated CN fixed codebook vector    */
);

/*************************************************************************
 *
 *   FUNCTION NAME: build_CN_param
 *
 *   PURPOSE: Randomize the speech parameters. So that they
 *            do not produce tonal artifacts if used by ECU.
 *
 *   INPUTS:      *seed            Old CN generator shift register state
 *                n_param,         Number of parameters to randomize
 *                parm_size_table,
 *
 *
 *   OUTPUTS:      parm    CN Generated Parameters
 *                *seed    Updated CN generator shift register state
 *
 *   RETURN VALUE: none
 *
 *************************************************************************/
void build_CN_param (
    Word16 *seed,              /* i/o : Old CN generator shift register state */
    const Word16 n_param,            /* i : number of params     */
    const Word16 param_size_table[], /* i : size of params       */
    Word16 parm[]              /* o   : CN Generated Params*/
);

#endif
