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
 *   WBAMR_UTIL.C
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR-WB codec
 *
 * Author:
 * -------
 *   Eddy Wu
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$  
 * $Log:$
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION
 
#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_sections.h"


/*-------------------------------------------------------------------*
 * Function  Set zero()                                              *
 *           ~~~~~~~~~~                                              *
 * Set vector x[] to zero                                            *
 *-------------------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Set_zero (
  fract16 x[],              /* (o)    : vector to clear     */
  fract16 L                 /* (i)    : length of vector    */
)
{


  int_native i;
  
  for (i = 0; i < L; i++)
  {
     x[i] = 0;                          
  }

  return;
  
}


/*-------------------------------------------------------------------*
 * Function  Copy:                                                   *
 *           ~~~~~                                                   *
 * Copy vector x[] to y[]                                            *
 *-------------------------------------------------------------------*/
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy

WBAMR_CODE_SECTION void WBAMR_Copy_interleave (
   fract16 x[],             /* (i)   : input vector   */
   fract16 y[]              /* (o)   : output vector  */
)
{
  
   int_native i;
   fract16 *p1, *p2, *p3;

   p1 = x + 16;
   p2 = p1 + 16;
   p3 = p2 + 16;

   i = 16;
   do {
      *y++ = *x++;
      *y++ = *p1++;
      *y++ = *p2++;
      *y++ = *p3++;
   } while(--i != 0);

   return;
  
}

#endif

WBAMR_CODE_SECTION void WBAMR_Copy (
  fract16 x[],             /* (i)   : input vector   */
  fract16 y[],             /* (o)   : output vector  */
  fract16 L                /* (i)   : vector length  */
)
{

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
  int remain;
  int L32;

  remain = L & 3;
  for (L32 = (fract32)L >> 2; L32 > 0; L32--)
  {
     *y++ = *x++;
     *y++ = *x++;
     *y++ = *x++;
     *y++ = *x++;
  }
  for( ;remain > 0; remain--)
  {
     *y++ = *x++;
  }
#else
  int_native i;

  for (i = 0; i < L; i++)
  {
     y[i] = x[i];                       
  }
#endif

  return;
  
}

#if (AWB_OPT_VER == 2)
WBAMR_CODE_SECTION void WBAMR_Copy32 (
  fract32 x[],             /* (i)   : input vector   */
  fract32 y[],             /* (o)   : output vector  */
  fract32 L                /* (i)   : vector length  */
)
{
  
  int L32;

  L32 = L >> 2;
  do
  {
     *y++ = *x++;
     *y++ = *x++;
     *y++ = *x++;
     *y++ = *x++;
  } while(--L32 != 0);

  return;
  
}
#endif

#endif

