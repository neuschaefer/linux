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
 *   lsp_lsf.h
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
*      File             : lsp_lsf.c
*      Purpose          : Lsp_lsf:   Transformation lsp to lsf
*                       : Lsf_lsp:   Transformation lsf to lsp
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "lsp_lsf.h"


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
//#include "lsp_lsf.tab"          /* Look-up table for transformations */


/*******************************************************************************
*
*      File             : lsp_lsf.tab
*      Purpose          : Tables for function Lsf_lsp() and Lsp_lsf()
*      $Id $
*
********************************************************************************
*/
/* table of cos(x) */

static const Word16 table[65] =
{
    32767, 32729, 32610, 32413, 32138, 31786, 31357, 30853,
    30274, 29622, 28899, 28106, 27246, 26320, 25330, 24279,
    23170, 22006, 20788, 19520, 18205, 16846, 15447, 14010,
    12540, 11039, 9512, 7962, 6393, 4808, 3212, 1608,
    0, -1608, -3212, -4808, -6393, -7962, -9512, -11039,
    -12540, -14010, -15447, -16846, -18205, -19520, -20788, -22006,
    -23170, -24279, -25330, -26320, -27246, -28106, -28899, -29622,
    -30274, -30853, -31357, -31786, -32138, -32413, -32610, -32729,
    (Word16) 0x8000
};

/* 0x8000 = -32768 (used to silence the compiler) */

/* slope used to compute y = acos(x) */

static const Word16 slope[64] =
{
    -26887, -8812, -5323, -3813, -2979, -2444, -2081, -1811,
    -1608, -1450, -1322, -1219, -1132, -1059, -998, -946,
    -901, -861, -827, -797, -772, -750, -730, -713,
    -699, -687, -677, -668, -662, -657, -654, -652,
    -652, -654, -657, -662, -668, -677, -687, -699,
    -713, -730, -750, -772, -797, -827, -861, -901,
    -946, -998, -1059, -1132, -1219, -1322, -1450, -1608,
    -1811, -2081, -2444, -2979, -3813, -5323, -8812, -26887
};

static const Word16 table_diff[64] = {
   -38, -119, -197, -275, -352, -429, -504, -579,
   -652, -723, -793, -860, -926, -990, -1051, -1109,
   -1164, -1218, -1268, -1315, -1359, -1399, -1437, -1470,
   -1501, -1527, -1550, -1569, -1585, -1596, -1604, -1608,
   -1608, -1604, -1596, -1585, -1569, -1550, -1527, -1501,
   -1470, -1437, -1399, -1359, -1315, -1268, -1218, -1164,
   -1109, -1051, -990, -926, -860, -793, -723, -652,
   -579, -504, -429, -352, -275, -197, -119, -39,
};


/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *   FUNCTIONS:  Lsp_lsf and Lsf_lsp
 *
 *   PURPOSE:
 *      Lsp_lsf:   Transformation lsp to lsf
 *      Lsf_lsp:   Transformation lsf to lsp
 *
 *   DESCRIPTION:
 *         lsp[i] = cos(2*pi*lsf[i]) and lsf[i] = arccos(lsp[i])/(2*pi)
 *
 *   The transformation from lsp[i] to lsf[i] and lsf[i] to lsp[i] are
 *   approximated by a look-up table and interpolation.
 *
 *************************************************************************/
void Lsf_lsp (
    Word16 lsf[],       /* (i) : lsf[m] normalized (range: 0.0<=val<=0.5) */
    Word16 lsp[]       /* (o) : lsp[m] (range: -1<=val<1)                */
)
{
   Word32 i, ind;

	i=M;
   do {
      ind = *lsf >> 8;              /* ind    = b8-b15 of lsf[I] */  /* offset = b0-b7  of lsf[i] */
      /* lsp[i] = table[ind]+ ((table[ind+1]-table[ind])*offset) / 256 */
      *lsp++ = (Word16)(((*lsf & 0x00ff) * table_diff[ind] >> 8) + table[ind]);
      lsf++;
   } while( --i != 0 );

    return;
}


void Lsp_lsf (
    Word16 lsp[],       /* (i)  : lsp[m] (range: -1<=val<1)                */
    Word16 lsf[]       /* (o)  : lsf[m] normalized (range: 0.0<=val<=0.5) */
)
{
   Word32 i, ind , s , t;
   const Word16 *ptr16;

   ind = 63;                        /* begin at end of table -1 */

   for (i = M - 1; i >= 0; i--)
   {
	   /* find value in table that is just greater than lsp[i] */

	   t = lsp[i];

      ptr16 = table + ind;

      while((s = t - *ptr16--) > 0 )
      {
   	   ind--;
      }

      /* acos(lsp[i])= ind*256 + ( ( lsp[i]-table[ind] )*slope[ind] )/4096 */
      /*(lsp[i]-table[ind])*slope[ind])>>12*/
      lsf[i] = (Word16)((ind<<8) + ((s * slope[ind] + 0x800) >> 12));

	}

    return;

}


/****************************************************
 *
 * FUNCTION  Lsf_wt                                                         *
 *                                                                          *
 ****************************************************
 * Compute LSF weighting factors                                            *
 *                                                                          *
 *  d[i] = lsf[i+1] - lsf[i-1]                                              *
 *                                                                          *
 *  The weighting factors are approximated by two line segment.             *
 *                                                                          *
 *  First segment passes by the following 2 points:                         *
 *                                                                          *
 *     d[i] = 0Hz     wf[i] = 3.347                                         *
 *     d[i] = 450Hz   wf[i] = 1.8                                           *
 *                                                                          *
 *  Second segment passes by the following 2 points:                        *
 *                                                                          *
 *     d[i] = 450Hz   wf[i] = 1.8                                           *
 *     d[i] = 1500Hz  wf[i] = 1.0                                           *
 *                                                                          *
 *  if( d[i] < 450Hz )                                                      *
 *    wf[i] = 3.347 - ( (3.347-1.8) / (450-0)) *  d[i]                      *
 *  else                                                                    *
 *    wf[i] = 1.8 - ( (1.8-1.0) / (1500-450)) *  (d[i] - 450)               *
 *                                                                          *
 *                                                                          *
 *  if( d[i] < 1843)                                                        *
 *    wf[i] = 3427 - (28160*d[i])>>15                                       *
 *  else                                                                    *
 *    wf[i] = 1843 - (6242*(d[i]-1843))>>15                                 *
 *                                                                          *
 *--------------------------------------------------------------------------*/

void Lsf_wt (
    Word16 *lsf,         /* input : LSF vector                  */
    Word16 *wf)          /* output: square of weighting factors */
{
   Word32 temp;
	Word32 i;


   /* wf[0] = lsf[1] - 0  */
   wf[0] = lsf[1];
   wf[1] = (Word16)(lsf[2] - lsf[0]);
   wf[2] = (Word16)(lsf[3] - lsf[1]);
   wf[3] = (Word16)(lsf[4] - lsf[2]);
   wf[4] = (Word16)(lsf[5] - lsf[3]);
   wf[5] = (Word16)(lsf[6] - lsf[4]);
   wf[6] = (Word16)(lsf[7] - lsf[5]);
   wf[7] = (Word16)(lsf[8] - lsf[6]);
   wf[8] = (Word16)(lsf[9] - lsf[7]);
   /* wf[9] = 0.5 - lsf[8] */
   wf[9] = (Word16)(16384 - lsf[8]);

	i = M;
   do {
      temp = *wf - 1843;
      if (temp < 0)
         *wf = (Word16)((3427 - (*wf * 28160>>15))<<3);
      else
         *wf = (Word16)((1843 - (temp * 6242>>15))<<3);
      wf++;
   } while( --i != 0 );


    return;

}



/*************************************************************************
 *
 *  FUNCTION:  Reorder_lsf()
 *
 *  PURPOSE: To make sure that the LSFs are properly ordered and to keep a
 *           certain minimum distance between adjacent LSFs.
 *
 *           The LSFs are in the frequency range 0-0.5 and represented in Q15
 *
 *************************************************************************/
void Reorder_lsf (
    Word16 *lsf        /* (i/o)     : vector of LSFs   (range: 0<=val<=0.5) */
)
{
   Word32 i;
   Word16 lsf_min;

   lsf_min = LSF_GAP;
	i = M;
	do
   {
      if(*lsf < lsf_min)
      {
   	   *lsf = lsf_min;
      }
      lsf_min = (*lsf + LSF_GAP);
		lsf++;

	}while(--i != 0);

}

#endif

