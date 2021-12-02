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
 *   dec_gain.c
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
*      File             : dec_gain.c
*      Purpose          : Decode the pitch and codebook gains
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "dec_gain.h"


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

#include "tab_qua_gain.h"
#include "tab_qgain475.h"

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *
 *   FUNCTION:  Dec_gain()
 *
 *   PURPOSE: Decode the pitch and codebook gains
 *
 ************************************************************************/
void Dec_gain(
    gc_predState *pred_state, /* i/o: MA predictor state           */
    enum Mode mode,           /* i  : AMR mode                     */
    Word16 index,             /* i  : index of quantization.       */
    Word16 code[],            /* i  : Innovative vector.           */
    Word16 evenSubfr,         /* i  : Flag for even subframes      */
    Word16 * gain_pit,        /* o  : Pitch gain.                  */
    Word16 * gain_cod         /* o  : Code gain.                   */
)
{
    Word32 L_tmp;
    Word32 g_code,gcode0;
    const Word16 *p;
    Word16 frac, exp, qua_ener, qua_ener_MR122;

    /* Read the quantized gains (table depends on mode) */
    index = sature(index << 2);


    if (    (mode == MR102)
         || (mode == MR74)
         || (mode == MR67) )
    {
        if (index >= VQ_SIZE_HIGHRATES*4)
            index = 0;

        p = &table_gain_highrates[index];
        *gain_pit = *p++;
        g_code = *p++;
        qua_ener_MR122 = *p++;
        qua_ener = *p;
    }
    else
    {
        if(mode == MR475)
        {
            index = (index + ((1 - evenSubfr) << 1));
            if (index >= MR475_VQ_SIZE*4)
                index = 0;
            p = &table_gain_MR475[index];

            *gain_pit = *p++;
            g_code = *p++;
            /*---------------------------------------------------------*
             *  calculate predictor update values (not stored in 4.75  *
             *  quantizer table to save space):                        *
             *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  *
             *                                                         *
             *   qua_ener       = log2(g)                              *
             *   qua_ener_MR122 = 20*log10(g)                          *
             *---------------------------------------------------------*/
            /* Log2(x Q12) = log2(x) + 12 */
            Log2((g_code), &exp, &frac);
            exp = (exp - 12);

            qua_ener_MR122 = (shr_r(frac, 5) + sature(exp << 10));

            /* 24660 Q12 ~= 6.0206 = 20*log10(2) */
            L_tmp = Mpy_32_16(exp, frac, 24660);
            qua_ener = (Word16)(qadd(L_shl_p(L_tmp, 13),0x8000)>>16); /* Q12 * Q0 = Q13 -> Q10 */
        }
        else
        {
            if (index >= VQ_SIZE_LOWRATES*4)
                index = 0;
            p = &table_gain_lowrates[index];
            *gain_pit = *p++;
            g_code = *p++;
            qua_ener_MR122 = *p++;
            qua_ener = *p;
        }
    }

    /*-------------------------------------------------------------------*
     *  predict codebook gain                                            *
     *  ~~~~~~~~~~~~~~~~~~~~~                                            *
     *  gc0     = Pow2(int(d)+frac(d))                                   *
     *          = 2^exp + 2^frac                                         *
     *                                                                   *
     *  gcode0 (Q14) = 2^14*2^frac = gc0 * 2^(14-exp)                    *
     *-------------------------------------------------------------------*/
    gc_pred(pred_state, mode, code, &exp, &frac, NULL, NULL);

    gcode0 = (Word16)(Pow2(14, frac));
    /*------------------------------------------------------------------*
     *  read quantized gains, update table of past quantized energies   *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   *
     *  st->past_qua_en(Q10) = 20 * Log10(g_fac) / constant             *
     *                       = Log2(g_fac)                              *
     *                       = qua_ener                                 *
     *                                           constant = 20*Log10(2) *
     *------------------------------------------------------------------*/
    L_tmp = smulbb(g_code, gcode0);
	 L_tmp = qadd(L_tmp,L_tmp);
    L_tmp = L_shr(L_tmp, (10 - exp));
    *gain_cod = (Word16)(L_tmp >> 16);

    /* update table of past quantized energies */
    gc_pred_update(pred_state, qua_ener_MR122, qua_ener);

    return;
}

#endif

