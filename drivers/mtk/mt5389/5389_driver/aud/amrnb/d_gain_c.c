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
 *   d_gain_c.c
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
*      File             : d_gain_c.c
*
********************************************************************************
*/


/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "d_gain_c.h"


/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "pow2.h"
#include "gc_pred.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#include "tab_gains.h"

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*
**************************************************************************
*
*  Function    : d_gain_code
*  Purpose     : Decode the fixed codebook gain using the received index.
*
**************************************************************************
*/
void d_gain_code (
    gc_predState *pred_state, /* i/o : MA predictor state               */
    enum Mode mode,           /* i   : AMR mode (MR795 or MR122)        */
    Word16 index,             /* i   : received quantization index      */
    Word16 code[],            /* i   : innovation codevector            */
    Word16 *gain_code         /* o   : decoded innovation gain          */
)
{
    Word32 L_tmp,gcode0;
    Word16 exp, frac;
    Word16 qua_ener_MR122, qua_ener;
    Word16 exp_inn_en;
    Word16 frac_inn_en;
    const Word16 *p;

    /*-------------- Decode codebook gain ---------------*/
    /*-------------------------------------------------------------------*
     *  predict codebook gain                                            *
     *  ~~~~~~~~~~~~~~~~~~~~~                                            *
     *  gc0     = Pow2(int(d)+frac(d))                                   *
     *          = 2^exp + 2^frac                                         *
     *                                                                   *
     *-------------------------------------------------------------------*/
    gc_pred(pred_state, mode, code, &exp, &frac, &exp_inn_en, &frac_inn_en);
    p = &qua_gain_code[ (index + index + index)];

    /* Different scalings between MR122 and the other modes */
    if ( (mode == MR122) )
    {
        gcode0 = (Word16) (Pow2 (exp, frac));  /* predicted gain */
        gcode0 = sature(gcode0 << 4);
        *gain_code = sature( (smulbb(gcode0, *p++)>>15) << 1);
    }
    else
    {
        gcode0 = (Word16)(Pow2(14, frac));
        L_tmp = smulbb(*p++, gcode0);
		  L_tmp = qadd(L_tmp,L_tmp);
        L_tmp = L_shr(L_tmp, (9 - exp));
        *gain_code = (Word16)(L_tmp>>16);          /* Q1 */
    }

    /*-------------------------------------------------------------------*
     *  update table of past quantized energies                          *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                          *
     *-------------------------------------------------------------------*/
    qua_ener_MR122 = *p++;
    qua_ener = *p++;
    gc_pred_update(pred_state, qua_ener_MR122, qua_ener);

    return;

}

#endif

