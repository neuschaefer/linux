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
 *   q_gain_p.c
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
*      File             : q_gain_p.c
*      Purpose          : Scalar quantization of the pitch gain
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "q_gain_p.h"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
 
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
Word16 q_gain_pitch (   /* Return index of quantization                      */
    enum Mode mode,     /* i  : AMR mode                                     */
    Word16 gp_limit,    /* i  : pitch gain limit                             */
    Word16 *gain,       /* i/o: Pitch gain (unquant/quant),              Q14 */
    Word16 gain_cand[], /* o  : pitch gain candidates (3),   MR795 only, Q14 */
    Word16 gain_cind[]  /* o  : pitch gain cand. indices (3),MR795 only, Q0  */
)
{
    Word32 i, index, err, err_min;
    err_min = abs_s( (*gain - qua_gain_pitch[0]));
    index = 0;

    for (i = 1; i < NB_QUA_PITCH; i++)
    {

        if ( (qua_gain_pitch[i] <= gp_limit) )
        {
            err = abs_s( (*gain - qua_gain_pitch[i]));

            if(err < err_min)
            {
                err_min = err;
                index = i;
            }
        }
    }

    if ( mode == MR795)
    {
        /* in MR795 mode, compute three gain_pit candidates around the index
         * found in the quantization loop: the index found and the two direct
         * neighbours, except for the extreme cases (i=0 or i=NB_QUA_PITCH-1),
         * where the direct neighbour and the neighbour to that is used.
         */
        Word32 ii;
        if (index == 0)
        {
            ii = index;
        }
        else
        {
            if (   (index == NB_QUA_PITCH-1)
                || (qua_gain_pitch[index+1] > gp_limit) )
            {
                ii = (index - 2);
            }
            else
            {
                ii = (index - 1);
            }
        }

        /* store candidate indices and values */
//        for (i = 0; i < 3; i++)
        {
            gain_cind[0] = ii;
            gain_cand[0] = qua_gain_pitch[ii];
            ii = (ii + 1);

            gain_cind[1] = ii;
            gain_cand[1] = qua_gain_pitch[ii];
            ii = (ii + 1);

            gain_cind[2] = ii;
            gain_cand[2] = qua_gain_pitch[ii];
            ii = (ii + 1);
        }

        *gain = qua_gain_pitch[index];
    }
    else
    {
        /* in MR122 mode, just return the index and gain pitch found.
         * If bitexactness is required, mask away the two LSBs (because
         * in the original EFR, gain_pit was scaled Q12)
         */

       if (mode == MR122)
       {
          /* clear 2 LSBits */
          *gain = qua_gain_pitch[index] & 0xFFFC;
       }
       else
       {
          *gain = qua_gain_pitch[index];
       }
    }
    return (Word16)index;
}

#endif

