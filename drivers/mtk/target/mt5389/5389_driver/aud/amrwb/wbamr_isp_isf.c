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
 *   WBAMR_ISP_ISF.C
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
#include "wbamr_private.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Isp_isf ( 
  fract16 isp[],             /* i : (Q15) : isp[m] ( range: -1<=val<1) */
  fract16 isf[]              /* 0 : (Q15) : isf[m] normalized 
                                            ( range: 0.0<=val<=0.5)    */
 )                         

{
  int_native i;
  fract16 ind;
  fract32 L_tmp;

  ind = 127;                 /* beging at end of table -1 */

  for ( i = ( M11 - 1 ); i >= 0; i-- )
  {
    if ( ( i-( M11-2 ) ) >= 0 ) 
                             /* M11-2 is a constant */
    {                                    
      ind = 127;             /* beging at end of table -1 */
    }
    
    /* find value in table that is just greater than isp[i] */
    while ( Sub_fr1x16 ( table[ind], isp[i] ) < 0 )
           ind--;

    /* acos(isp[i])= ind*128 + ((isp[i]-table[ind]) * slope[ind])/2048 */
    L_tmp = Mult_fr1x32 ( Sub_fr1x16 ( isp[i], table[ind] ), slope[ind] );
    isf[i] = Round_fr1x32 ( Shl_fr1x32 ( L_tmp, 4 ) );   
                             /* ( isp[i]-table[ind] )*slope[ind] )>>11 */
        
    isf[i] = Add_fr1x16 ( isf[i], Shl_fr1x16( ind, 7 ) ); 
  }

  isf[M11 - 1] = Shr_fr1x16 ( isf[M11 - 1], 1 );       

  return;
}


WBAMR_CODE_SECTION void WBAMR_Isf_isp( 
  fract16 isf[],   /* i : (Q15) : isf[m] normalized ( range: 0.0<=val<=0.5 ) */
  fract16 isp[],   /* o : (Q15) : isp[m] ( range: -1<=val<1 )                */
  fract16 m        /* i :         LPC order                                  */
 )

{
  int_native i;
  fract16 ind, offset;
  fract32 L_tmp;

  for ( i = 0; i < m - 1; i++ )
  {
    isp[i] = isf[i];                   
  }
  isp[m - 1] = Shl_fr1x16 ( isf[m - 1], 1 );

  for ( i = 0; i < m; i++ )
  {
    ind = Shr_fr1x16 ( isp[i], 7 );      
                   /* ind    = b7-b15 of isf[i] */
    offset = ( fract16 ) ( isp[i] & 0x007f );      
                   /* offset = b0-b6  of isf[i] */

    /* isp[i] = table[ind]+ ( ( table[ind+1]-table[ind] )*offset ) / 128 */
    L_tmp = Mult_fr1x32 ( Sub_fr1x16 ( table[ind + 1], table[ind] ), offset );
    isp[i] = Add_fr1x16 ( table[ind], 
                          Extract_lo_fr1x32 ( Shr_fr1x32 ( L_tmp, 8 ) ) );
  }

  return;
}

#endif

