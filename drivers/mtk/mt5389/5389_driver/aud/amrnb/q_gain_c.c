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
 *   q_gain_c.c
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
*      File             : q_gain_c.c
*      Purpose          : Scalar quantization of the innovative
*                       : codebook gain.
*
********************************************************************************
*/


/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "q_gain_c.h"

 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "pow2.h"

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

/*--------------------------------------------------------------------------*
 * Function q_gain_code()                                                   *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                  *
 * Scalar quantization of the innovative codebook gain.                     *
 *                                                                          *
 *--------------------------------------------------------------------------*/
Word16 q_gain_code (        /* o  : quantization index,            Q0  */
    enum Mode mode,         /* i  : AMR mode                           */
    Word16 exp_gcode0,      /* i  : predicted CB gain (exponent),  Q0  */
    Word16 frac_gcode0,     /* i  : predicted CB gain (fraction),  Q15 */
    Word16 *gain,           /* i/o: quantized fixed codebook gain, Q1  */
    Word16 *qua_ener_MR122, /* o  : quantized energy error,        Q10 */
                            /*      (for MR122 MA predictor update)    */
    Word16 *qua_ener        /* o  : quantized energy error,        Q10 */
                            /*      (for other MA predictor update)    */
)
{
    const Word16 *p;
    Word32 i,index;
    Word16 gcode0, err, err_min;
    Word16 g_q0;


    g_q0 = 0;

    if(mode == MR122)
    {
       g_q0 = (*gain >> 1); /* Q1 -> Q0 */
    }

    /*-------------------------------------------------------------------*
     *  predicted codebook gain                                          *
     *  ~~~~~~~~~~~~~~~~~~~~~~~                                          *
     *  gc0     = Pow2(int(d)+frac(d))                                   *
     *          = 2^exp + 2^frac                                         *
     *                                                                   *
     *-------------------------------------------------------------------*/

    gcode0 = (Word16)(Pow2 (exp_gcode0, frac_gcode0));  /* predicted gain */


    if(mode == MR122)
    {
       gcode0 = sature(gcode0 << 4);
    }
    else
    {
       gcode0 = sature(gcode0 << 5);
    }

    /*-------------------------------------------------------------------*
     *                   Search for best quantizer                        *
     *-------------------------------------------------------------------*/

    p = &qua_gain_code[0];

    if(mode == MR122)
    {
       err_min = abs_s ( g_q0 -  (smulbb(gcode0, *p++)>>15)  );
    }
    else
    {
       err_min = abs_s ( *gain - (smulbb(gcode0, *p++)>>15)  );
    }
    p += 2;                                  /* skip quantized energy errors */
    index = 0;

	//NB_QUA_CODE = 32
    for (i = 1; i < NB_QUA_CODE; i++)
    {

       if(mode == MR122)
       {
          err = abs_s ( g_q0 -  (smulbb(gcode0, *p++)>>15)  );
       }
       else
       {
          err = abs_s ( *gain - (smulbb(gcode0, *p++)>>15)  );
       }

       p += 2;                              /* skip quantized energy error */

       if(err < err_min)
       {
          err_min = err;
          index = i;
       }
    }

    p = &qua_gain_code[ ( index + index + index)];

    if(mode == MR122)
    {
       *gain = ( smulbb(gcode0, *p++)>>15 ) << 1 ;
    }
    else
    {
       *gain = smulbb(gcode0, *p++) >> 15;
    }


    /* quantized error energies (for MA predictor update) */
    *qua_ener_MR122 = *p++;

    *qua_ener = *p;


    return (Word16)index;

}

#endif

