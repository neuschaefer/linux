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
 *   WBAMR_ISFEXTRP.C
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
#include "wbamr_oper_32b.h"
#include "wbamr_cnst.h"
#include "wbamr_acelp.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Isf_Extrapolation ( 
  fract16 HfIsf[]                    /* i/o */
 )

{
  int_native i;
  fract16 IsfDiff[M11 - 2];
  fract32 IsfCorr[3];
  fract32 L_tmp;
  fract16 coeff, mean, tmp, tmp2, tmp3;
  fract16 exp, exp2, hi, lo;
    
  fract16 MaxCorr;

  HfIsf[M16k - 1] = HfIsf[M11 - 1];        

  /* Difference vector */
  for ( i = 1; i < ( M11 - 1 ); i++ )
  {
    IsfDiff[i - 1] = Sub_fr1x16 ( HfIsf[i], HfIsf[i - 1] );   
  }
  
  L_tmp = 0;                             
  /* Mean of difference vector */
  for ( i = 3; i < ( M11 - 1 ); i++ )
    L_tmp = Mac_fr16_32 ( L_tmp, IsfDiff[i - 1], INV_LENGTH );
  
  mean = Round_fr1x32 ( L_tmp );

  IsfCorr[0] = 0;                        
  IsfCorr[1] = 0;                        
  IsfCorr[2] = 0;                        

  tmp = 0;                               
  for ( i = 0; i < ( M11 - 2 ); i++ )
  {
    if ( Sub_fr1x16 ( IsfDiff[i], tmp ) > 0 )
    {
      tmp = IsfDiff[i];              
    }
  }
  
  exp = WBAMR_Norm_s ( tmp );
  for ( i = 0; i < ( M11 - 2 ); i++ )
  {
    IsfDiff[i] = Shl_fr1x16 ( IsfDiff[i], exp ); 
  }
  
  mean = Shl_fr1x16 ( mean, exp );
  
  for ( i = 7; i < ( M11 - 2 ); i++ )
  {
    tmp2 = Sub_fr1x16 ( IsfDiff[i], mean );
    tmp3 = Sub_fr1x16 ( IsfDiff[i - 2], mean );
    L_tmp = Mult_fr1x32 ( tmp2, tmp3 );
    
    WBAMR_L_Extract ( L_tmp, &hi, &lo );
    
    L_tmp = WBAMR_Mpy_32 ( hi, lo, hi, lo );
    IsfCorr[0] = Add_fr1x32 ( IsfCorr[0], L_tmp );  
  }
  
  for ( i = 7; i < ( M11 - 2 ); i++ )
  {
    tmp2 = Sub_fr1x16 ( IsfDiff[i], mean );
    tmp3 = Sub_fr1x16 ( IsfDiff[i - 3], mean );
    L_tmp = Mult_fr1x32 ( tmp2, tmp3 );
  
    WBAMR_L_Extract ( L_tmp, &hi, &lo );
  
    L_tmp = WBAMR_Mpy_32 ( hi, lo, hi, lo );
    IsfCorr[1] = Add_fr1x32 ( IsfCorr[1], L_tmp );  
  }
  
  for ( i = 7; i < ( M11 - 2 ); i++ )
  {
    tmp2 = Sub_fr1x16 ( IsfDiff[i], mean );
    tmp3 = Sub_fr1x16 ( IsfDiff[i - 4], mean );
    L_tmp = Mult_fr1x32 ( tmp2, tmp3 );
  
    WBAMR_L_Extract ( L_tmp, &hi, &lo );
  
    L_tmp = WBAMR_Mpy_32 ( hi, lo, hi, lo );
    IsfCorr[2] = Add_fr1x32 ( IsfCorr[2], L_tmp );  
  }
    
  if ( Sub_fr1x32 ( IsfCorr[0], IsfCorr[1] ) > 0 )
  {
    MaxCorr = 0;                       
  } 
  else
  {
    MaxCorr = 1;                       
  }

  if ( Sub_fr1x32 ( IsfCorr[2], IsfCorr[MaxCorr] ) > 0 )
     MaxCorr = 2;                       

  MaxCorr = Add_fr1x16 ( MaxCorr, 1 );       
                             /* Maximum correlation of difference vector */

  for ( i = M11 - 1; i < ( M16k - 1 ); i++ )
  {
    tmp = Sub_fr1x16 ( HfIsf[i - 1 - MaxCorr], HfIsf[i - 2 - MaxCorr] );
    HfIsf[i] = Add_fr1x16 ( HfIsf[i - 1], tmp ); 
  }

  /* tmp = 7965+( HfIsf[2]-HfIsf[3]-HfIsf[4] )/6; */
  tmp = Add_fr1x16 ( HfIsf[4], HfIsf[3] );
  tmp = Sub_fr1x16 ( HfIsf[2], tmp );
  tmp = Mult_fr1x16 ( tmp, 5461 );
  tmp = Add_fr1x16 ( tmp, 20390 );
    
  if ( Sub_fr1x16 ( tmp, 19456 ) > 0 )
  {                    
    tmp = 19456;             /* Maximum value of ISF should be 
                                at most 7600 Hz */
  }
  tmp = Sub_fr1x16 ( tmp, HfIsf[M11 - 2] );
  tmp2 = Sub_fr1x16 ( HfIsf[M16k - 2], HfIsf[M11 - 2] );

  exp2 = WBAMR_Norm_s ( tmp2 );
  exp = WBAMR_Norm_s ( tmp );
  exp = Sub_fr1x16 ( exp, 1 );
  tmp = Shl_fr1x16 ( tmp, exp );
  tmp2 = Shl_fr1x16 ( tmp2, exp2 );
  coeff = Divs_fr1x16 ( tmp, tmp2 );
                             /* Coefficient for stretching the ISF vector */
  exp = Sub_fr1x16 ( exp2, exp );

  for ( i = M11 - 1; i < ( M16k - 1 ); i++ )
  {
    tmp = Mult_fr1x16 ( Sub_fr1x16 ( HfIsf[i], HfIsf[i - 1] ), coeff );
    IsfDiff[i - ( M11 - 1 )] = Shl_fr1x16 ( tmp, exp );   
  }

  for ( i = M11; i < ( M16k - 1 ); i++ )
  {
    /* The difference between ISF( n ) and ISF( n-2 ) 
       should be at least 500 Hz */
    tmp = Sub_fr1x16 ( Add_fr1x16 ( IsfDiff[i - ( M11 - 1 )], IsfDiff[i - M11] ), 
                      1280 );
        
    if ( tmp < 0 )
    {
      if ( Sub_fr1x16 ( IsfDiff[i - ( M11 - 1 )], IsfDiff[i - M11] ) > 0 )
      {
        IsfDiff[i - M11] = Sub_fr1x16 ( 1280, IsfDiff[i - ( M11 - 1 )] );       
      }
      else
      {
        IsfDiff[i - ( M11 - 1 )] = Sub_fr1x16 ( 1280, IsfDiff[i - M11] );       
      }
    }
  }

  for ( i = M11 - 1; i < ( M16k - 1 ); i++ )
  {
    HfIsf[i] = Add_fr1x16 ( HfIsf[i - 1], IsfDiff[i - ( M11 - 1 )] );     
  }

  for ( i = 0; i < ( M16k - 1 ); i++ )
  {
    HfIsf[i] = Mult_fr1x16 ( HfIsf[i], 26214 );  
                             /* Scale the ISF vector correctly for 16000 kHz */
  }

  WBAMR_Isf_isp ( HfIsf, HfIsf, M16k );

  return;
}

#endif

