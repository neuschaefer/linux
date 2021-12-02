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
 *   tab_gains.c
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
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*
*      Purpose          : Scalar quantization tables of the pitch gain and
*                       : the codebook gain.
********************************************************************************
*/

#include "typedef.h"

#define NB_QUA_PITCH 16

const Word16 qua_gain_pitch[NB_QUA_PITCH] =
{
    0, 3277, 6556, 8192, 9830, 11469, 12288, 13107,
    13926, 14746, 15565, 16384, 17203, 18022, 18842, 19661
};

#define NB_QUA_CODE 32

const Word16 qua_gain_code[NB_QUA_CODE*3] =
{
/* gain factor (g_fac) and quantized energy error (qua_ener_MR122, qua_ener)
 * are stored:
 *
 * qua_ener_MR122 = log2(g_fac)      (not the rounded floating point value, but
 *                                    the value the original EFR algorithm
 *                                    calculates from g_fac [using Log2])
 * qua_ener       = 20*log10(g_fac); (rounded floating point value)
 *
 *
 * g_fac (Q11), qua_ener_MR122 (Q10), qua_ener (Q10)
 */
          159,                -3776,          -22731,
          206,                -3394,          -20428,
          268,                -3005,          -18088,
          349,                -2615,          -15739,
          419,                -2345,          -14113,
          482,                -2138,          -12867,
          554,                -1932,          -11629,
          637,                -1726,          -10387,
          733,                -1518,           -9139,
          842,                -1314,           -7906,
          969,                -1106,           -6656,
         1114,                 -900,           -5416,
         1281,                 -694,           -4173,
         1473,                 -487,           -2931,
         1694,                 -281,           -1688,
         1948,                  -75,            -445,
         2241,                  133,             801,
         2577,                  339,            2044,
         2963,                  545,            3285,
         3408,                  752,            4530,
         3919,                  958,            5772,
         4507,                 1165,            7016,
         5183,                 1371,            8259,
         5960,                 1577,            9501,
         6855,                 1784,           10745,
         7883,                 1991,           11988,
         9065,                 2197,           13231,
        10425,                 2404,           14474,
        12510,                 2673,           16096,
        16263,                 3060,           18429,
        21142,                 3448,           20763,
        27485,                 3836,           23097
};

#endif

