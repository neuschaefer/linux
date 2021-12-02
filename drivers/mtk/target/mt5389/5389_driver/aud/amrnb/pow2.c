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
 *   pow2.c
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
*      File             : pow2.c
*      Purpose          : computes  L_x = pow(2.0, exponent.fraction)
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "pow2.h"


/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
//#include "pow2.tab"     /* Table for Pow2() */

/*
static const Word16 table[33] =
{
    16384, 16743, 17109, 17484, 17867, 18258, 18658, 19066, 19484, 19911,
    20347, 20792, 21247, 21713, 22188, 22674, 23170, 23678, 24196, 24726,
    25268, 25821, 26386, 26964, 27554, 28158, 28774, 29405, 30048, 30706,
    31379, 32066, 32767
};
*/
static const Word32 pow_tab[32] = {
   16384<<16, 16743<<16, 17109<<16, 17484<<16, 17867<<16, 18258<<16, 18658<<16, 19066<<16,
   19484<<16, 19911<<16, 20347<<16, 20792<<16, 21247<<16, 21713<<16, 22188<<16, 22674<<16,
   23170<<16, 23678<<16, 24196<<16, 24726<<16, 25268<<16, 25821<<16, 26386<<16, 26964<<16,
   27554<<16, 28158<<16, 28774<<16, 29405<<16, 30048<<16, 30706<<16, 31379<<16, 32066<<16
};

static const Word16 pow_tab_d[32] = {
   -359*2, -366*2, -375*2, -383*2, -391*2, -400*2, -408*2, -418*2,
   -427*2, -436*2, -445*2, -455*2, -466*2, -475*2, -486*2, -496*2,
   -508*2, -518*2, -530*2, -542*2, -553*2, -565*2, -578*2, -590*2,
   -604*2, -616*2, -631*2, -643*2, -658*2, -673*2, -687*2, -701*2
};


/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *   FUNCTION:  Pow2()
 *
 *   PURPOSE: computes  L_x = pow(2.0, exponent.fraction)
 *
 *   DESCRIPTION:
 *       The function Pow2(L_x) is approximated by a table and linear
 *       interpolation.
 *          1- i = bit10-b15 of fraction,   0 <= i <= 31
 *          2- a = bit0-b9   of fraction
 *          3- L_x = table[i]<<16 - (table[i] - table[i+1]) * a * 2
 *          4- L_x = L_x >> (30-exponent)     (with rounding)
 *
 *************************************************************************/

Word32 Pow2 (           /* (o)  : result       (range: 0<=val<=0x7fffffff) */
    Word16 exponent,    /* (i)  : Integer part.      (range: 0<=val<=30)   */
    Word16 fraction     /* (i)  : Fractional part.  (range: 0.0<=val<1.0) */
)
{
    Word32 L_x, i,exp;

    L_x =  (fraction << 6); /* L_x = fraction<<6           */
    i   =  (L_x >> 16);         /* Extract b10-b16 of fraction */
    L_x = (L_x >> 1);

    L_x = pow_tab[i] - pow_tab_d[i] * (L_x&0x7fff);

    exp = (30 - exponent);
    L_x = L_shr_r(L_x, exp);

    return (L_x);
}

#endif

