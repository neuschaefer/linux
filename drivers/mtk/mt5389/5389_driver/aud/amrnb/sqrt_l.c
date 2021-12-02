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
 *   sqrt_l.c
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
*      File             : sqrt_l.c
*      Purpose          : Computes sqrt(L_x),  where  L_x is positive.
*                       : If L_var is negative or zero, the result is 0
*      Description      :
*            The function sqrt(L_x) is approximated by a table and linear
*            interpolation. The square root is computed using the
*            following steps:
*                1- Normalization of L_x.
*                2- If exponent is even then shift right once.
*                3- exponent = exponent/2
*                4- i = bit25-b31 of L_x;  16<=i<=63  because of normalization.
*                5- a = bit10-b24
*                6- i -=16
*                7- L_y = table[i]<<16 - (table[i] - table[i+1]) * a * 2
*                8- return L_y and exponent so caller can do denormalization
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "sqrt_l.h"

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
//#include "sqrt_l.tab" /* Table for sqrt_l_exp() */

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*
********************************************************************************
*      Purpose          : Table for routine sqrt_l_exp()
*      $Id $
********************************************************************************
*/

/* table[i] = sqrt((i+16)*2^-6) * 2^15, i.e. sqrt(x) scaled Q15 */
static const Word32 sqrt_tab[48] =
{
 (Word32)16384<<16,
 (Word32)16888<<16,
 (Word32)17378<<16,
 (Word32)17854<<16,
 (Word32)18318<<16,
 (Word32)18770<<16,
 (Word32)19212<<16,
 (Word32)19644<<16,
 (Word32)20066<<16,
 (Word32)20480<<16,
 (Word32)20886<<16,
 (Word32)21283<<16,
 (Word32)21674<<16,
 (Word32)22058<<16,
 (Word32)22435<<16,
 (Word32)22806<<16,
 (Word32)23170<<16,
 (Word32)23530<<16,
 (Word32)23884<<16,
 (Word32)24232<<16,
 (Word32)24576<<16,
 (Word32)24915<<16,
 (Word32)25249<<16,
 (Word32)25580<<16,
 (Word32)25905<<16,
 (Word32)26227<<16,
 (Word32)26545<<16,
 (Word32)26859<<16,
 (Word32)27170<<16,
 (Word32)27477<<16,
 (Word32)27780<<16,
 (Word32)28081<<16,
 (Word32)28378<<16,
 (Word32)28672<<16,
 (Word32)28963<<16,
 (Word32)29251<<16,
 (Word32)29537<<16,
 (Word32)29819<<16,
 (Word32)30099<<16,
 (Word32)30377<<16,
 (Word32)30652<<16,
 (Word32)30924<<16,
 (Word32)31194<<16,
 (Word32)31462<<16,
 (Word32)31727<<16,
 (Word32)31991<<16,
 (Word32)32252<<16,
 (Word32)32511<<16
};

static const Word16 sqrt_tab_diff[48] =
{
 (Word16)(16384- 16888),
 (Word16)(16888- 17378),
 (Word16)(17378- 17854),
 (Word16)(17854- 18318),
 (Word16)(18318- 18770),
 (Word16)(18770- 19212),
 (Word16)(19212- 19644),
 (Word16)(19644- 20066),
 (Word16)(20066- 20480),
 (Word16)(20480- 20886),
 (Word16)(20886- 21283),
 (Word16)(21283- 21674),
 (Word16)(21674- 22058),
 (Word16)(22058- 22435),
 (Word16)(22435- 22806),
 (Word16)(22806- 23170),
 (Word16)(23170- 23530),
 (Word16)(23530- 23884),
 (Word16)(23884- 24232),
 (Word16)(24232- 24576),
 (Word16)(24576- 24915),
 (Word16)(24915- 25249),
 (Word16)(25249- 25580),
 (Word16)(25580- 25905),
 (Word16)(25905- 26227),
 (Word16)(26227- 26545),
 (Word16)(26545- 26859),
 (Word16)(26859- 27170),
 (Word16)(27170- 27477),
 (Word16)(27477- 27780),
 (Word16)(27780- 28081),
 (Word16)(28081- 28378),
 (Word16)(28378- 28672),
 (Word16)(28672- 28963),
 (Word16)(28963- 29251),
 (Word16)(29251- 29537),
 (Word16)(29537- 29819),
 (Word16)(29819- 30099),
 (Word16)(30099- 30377),
 (Word16)(30377- 30652),
 (Word16)(30652- 30924),
 (Word16)(30924- 31194),
 (Word16)(31194- 31462),
 (Word16)(31462- 31727),
 (Word16)(31727- 31991),
 (Word16)(31991- 32252),
 (Word16)(32252- 32511),
 (Word16)(32511- 32767)
};

Word32 sqrt_l_exp (/* o : output value,                          Q31 */
    Word32 L_x,    /* i : input value,                           Q31 */
    Word16 *exp    /* o : right shift to be applied to result,   Q1  */
)
{
    /*
          y = sqrt(x)
          x = f * 2^-e,   0.5 <= f < 1   (normalization)
          y = sqrt(f) * 2^(-e/2)

          a) e = 2k   --> y = sqrt(f)   * 2^-k  (k = e div 2,
                                                 0.707 <= sqrt(f) < 1)
          b) e = 2k+1 --> y = sqrt(f/2) * 2^-k  (k = e div 2,
                                                 0.5 <= sqrt(f/2) < 0.707)
     */
   Word32 e, i;
   Word32 L_y;

   if (L_x <= (Word32) 0)
   {
   	*exp = 0;
      return (Word32) 0;
   }

   e = norm_l (L_x) & 0xFFFE;  				 /* get next lower EVEN norm. exp  */
   L_x = (L_x << e);                   /* L_x is normalized to [0.25..1) */
   *exp = e;                   				  /* return 2*exponent (or Q1)      */

   L_x >>= 10;
   i = (L_x >> 15) - 16;

   L_y = qdsub(sqrt_tab[i], smulbb( sqrt_tab_diff[i], (L_x & 0x7fff) ));
	/* L_y -= tmp*a*2                 */
   /* L_y = L_shr (L_y, *exp); */          /* denormalization done by caller */

   return (L_y);
}



#if 0


static const Word16 table[49] =
{
 16384,
 16888,
 17378,
 17854,
 18318,
 18770,
 19212,
 19644,
 20066,
 20480,
 20886,
 21283,
 21674,
 22058,
 22435,
 22806,
 23170,
 23530,
 23884,
 24232,
 24576,
 24915,
 25249,
 25580,
 25905,
 26227,
 26545,
 26859,
 27170,
 27477,
 27780,
 28081,
 28378,
 28672,
 28963,
 29251,
 29537,
 29819,
 30099,
 30377,
 30652,
 30924,
 31194,
 31462,
 31727,
 31991,
 32252,
 32511,
 32767
};



Word32 sqrt_l_exp (/* o : output value,                          Q31 */
    Word32 L_x,    /* i : input value,                           Q31 */
    Word16 *exp    /* o : right shift to be applied to result,   Q1  */
)
{
    /*
          y = sqrt(x)
          x = f * 2^-e,   0.5 <= f < 1   (normalization)
          y = sqrt(f) * 2^(-e/2)

          a) e = 2k   --> y = sqrt(f)   * 2^-k  (k = e div 2,
                                                 0.707 <= sqrt(f) < 1)
          b) e = 2k+1 --> y = sqrt(f/2) * 2^-k  (k = e div 2,
                                                 0.5 <= sqrt(f/2) < 0.707)
     */


    Word32 e, i, a, tmp;
    Word32 L_y;

    if (L_x <= (Word32) 0)
    {
        *exp = 0;
        return (Word32) 0;
    }

    e = norm_l (L_x) & 0xFFFE;  				 /* get next lower EVEN norm. exp  */
    L_x = (L_x << e);                   /* L_x is normalized to [0.25..1) */
    *exp = e;                   				  /* return 2*exponent (or Q1)      */

    L_x = (L_x >> 9);
    i = (Word16)(L_x >> 16);           /* Extract b25-b31, 16 <= i <= 63 because
                                       of normalization                       */
    L_x = (L_x >> 1);
    a = (Word16)(L_x);                 /* Extract b10-b24                        */
    a = a & (Word16) 0x7fff;

    i = (i - 16);                /* 0 <= i <= 47                           */

    L_y = (sqrt_tab[i]);           /* table[i] << 16                 */
    tmp = sqrt_tab_diff[i];     /* table[i] - table[i+1])         */
    L_y = qdsub(L_y, smulbb( tmp, a));              /* L_y -= tmp*a*2                 */

    /* L_y = L_shr (L_y, *exp); */          /* denormalization done by caller */

    return (L_y);
}
#endif

#endif

