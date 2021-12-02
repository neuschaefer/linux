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
 *   WBAMR_LAGCONC.C
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
#include "wbamr_cnst.h"
#include "wbamr_acelp.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Init_Lagconc ( 
  fract16 lag_hist[]
 )

{
  int_native i;

  for ( i = 0; i < L_LTPHIST; i++ )
  {
    lag_hist[i] = 64;
  }
}


WBAMR_CODE_SECTION void WBAMR_Lagconc ( 
  fract16 gain_hist[],            /* i : Gain history     */
  fract16 lag_hist[],             /* i : Subframe size    */
  fract16 * T0,
  fract16 * old_T0,
  fract16 * seed,
  fract16 unusable_frame,
  fract16 lag_hist2[]
 )

{
  int_native i;
  fract16 maxLag, minLag, lastLag, lagDif, meanLag = 0;
  fract16 tmp, tmp2;
  fract16 minGain, lastGain, secLastGain;
  fract16 D, D2;

  /* Is lag index such that it can be aplied directly 
     or does it has to be subtituted */

  lastGain = gain_hist[4];               
  secLastGain = gain_hist[3];            

  lastLag = lag_hist[0];                 

  /***********SMALLEST history lag***********/
  minLag = lag_hist[0];                  
  for ( i = 1; i < L_LTPHIST; i++ )
  {
    if ( Sub_fr1x16 ( lag_hist[i], minLag ) < 0 )
    {
      minLag = lag_hist[i];          
    }
  }
  
  /**********BIGGEST history lag*************/
  maxLag = lag_hist[0];                  
  for ( i = 1; i < L_LTPHIST; i++ )
  {
    if ( Sub_fr1x16 ( lag_hist[i], maxLag ) > 0 )
    {
      maxLag = lag_hist[i];          
    }
  }
  
  /***********SMALLEST history gain**********/
  minGain = gain_hist[0];                
  for ( i = 1; i < L_LTPHIST; i++ )
  {
    if ( Sub_fr1x16 ( gain_hist[i], minGain ) < 0 )
    {
      minGain = gain_hist[i];        
    }
  }
  
  /****Difference between MAX and MIN lag****/
  lagDif = Sub_fr1x16 ( maxLag, minLag );
    
  if ( unusable_frame != 0 )
  {
    /* LTP-lag for RX_SPEECH_LOST */
    /**********Recognition of the LTP-history*********/
    if ( ( Sub_fr1x16 ( minGain, 8192 ) > 0 ) &&
         ( Sub_fr1x16 ( lagDif, 10 ) < 0 ) )
    {
      *T0 = *old_T0;                 
    }
    else if ( Sub_fr1x16 ( lastGain, 8192 ) > 0 && 
              Sub_fr1x16 ( secLastGain, 8192 ) > 0 )
    {
      *T0 = lag_hist[0];             
    }
    else
    {
      /********SORT************/
      /* The sorting of the lag history */
      for ( i = 0; i < L_LTPHIST; i++ )
      {
        lag_hist2[i] = lag_hist[i];
      }
      
      WBAMR_Insertion_Sort ( lag_hist2, 5 );

      /* Lag is weighted towards bigger lags */
      /* and WBAMR_Random variation is added */
      lagDif = Sub_fr1x16 ( lag_hist2[4], lag_hist2[2] );
            
      if ( Sub_fr1x16 ( lagDif, 40 ) > 0 )
         lagDif = 40;               

      D = WBAMR_Random ( seed ); /* D={-1, ...,1} */
      /* D2={-lagDif/2..lagDif/2} */
      tmp = Shr_fr1x16 ( lagDif, 1 );
      D2 = Mult_fr1x16 ( tmp, D );
      tmp = Add_fr1x16 ( Add_fr1x16 ( lag_hist2[2], lag_hist2[3] ), 
                         lag_hist2[4] );
      *T0 = Add_fr1x16 ( Mult_fr1x16 ( tmp, ONE_PER_3 ), D2 );        
    }
    
    /* New lag is not allowed to be bigger or smaller than last lag values */
    if ( Sub_fr1x16 ( *T0, maxLag ) > 0 )
    {
      *T0 = maxLag;                  
    }
        
    if ( Sub_fr1x16 ( *T0, minLag ) < 0 )
    {
      *T0 = minLag;                  
    }
  }
  else
  {
    /* LTP-lag for RX_BAD_FRAME */
    /***********MEAN lag**************/
    meanLag = 0;                       
    for ( i = 0; i < L_LTPHIST; i++ )
    {
      meanLag = Add_fr1x16 ( meanLag, lag_hist[i] );
    }
    meanLag = Mult_fr1x16 ( meanLag, ONE_PER_LTPHIST );

    tmp = Sub_fr1x16 ( *T0, maxLag );
    tmp2 = Sub_fr1x16 ( *T0, lastLag );

    if ( Sub_fr1x16 ( lagDif, 10 ) < 0 && 
         ( Sub_fr1x16 ( *T0, Sub_fr1x16 ( minLag, 5 ) ) > 0 ) && 
         ( Sub_fr1x16 ( tmp, 5 ) < 0 ) )
    {
      *T0 = *T0;                     
    } 
    else if ( Sub_fr1x16 ( lastGain, 8192 ) > 0 && 
              Sub_fr1x16 ( secLastGain, 8192 ) > 0 && 
              ( Add_fr1x16 ( tmp2, 10 ) > 0 && 
              Sub_fr1x16 ( tmp2, 10 ) < 0 ) )
    {
      *T0 = *T0;                     
    }
    else if ( Sub_fr1x16 ( minGain, 6554 ) < 0 && 
              Sub_fr1x16 ( lastGain, minGain ) == 0 && 
              ( Sub_fr1x16 ( *T0, minLag ) > 0 && 
              Sub_fr1x16 ( *T0, maxLag ) < 0 ) )
    {
      *T0 = *T0;                     
    }
    else if ( Sub_fr1x16 ( lagDif, 70 ) < 0 && 
              Sub_fr1x16 ( *T0, minLag ) > 0 && 
              Sub_fr1x16 ( *T0, maxLag ) < 0 )
    {
      *T0 = *T0;                     
    }
    else if ( Sub_fr1x16 ( *T0, meanLag ) > 0 && 
              Sub_fr1x16 ( *T0, maxLag ) < 0 )
    {
      *T0 = *T0;                     
    }
    else
    {
      if ( ( Sub_fr1x16 ( minGain, 8192 ) > 0 ) & 
           ( Sub_fr1x16 ( lagDif, 10 ) < 0 ) )
      {
        *T0 = lag_hist[0];         
      }
      else if ( Sub_fr1x16 ( lastGain, 8192 ) > 0 && 
                Sub_fr1x16 ( secLastGain, 8192 ) > 0 )
      {
        *T0 = lag_hist[0];         
      }
      else
      {
        /********SORT************/
        /* The sorting of the lag history */
        for ( i = 0; i < L_LTPHIST; i++ )
        {
          lag_hist2[i] = lag_hist[i]; 
        }
        
        WBAMR_Insertion_Sort ( lag_hist2, 5 );

        /* Lag is weighted towards bigger lags */
        /* and WBAMR_Random variation is added */
        lagDif = Sub_fr1x16 ( lag_hist2[4], lag_hist2[2] );
                
        if ( Sub_fr1x16 ( lagDif, 40 ) > 0 )
           lagDif = 40;           

        /* D={-1,.., 1} */
        D = WBAMR_Random ( seed ); 
        /* D2={-lagDif/2..lagDif/2} */
        tmp = Shr_fr1x16 ( lagDif, 1 );
        D2 = Mult_fr1x16 ( tmp, D );
        tmp = Add_fr1x16 ( Add_fr1x16 ( lag_hist2[2], lag_hist2[3] ), 
                           lag_hist2[4] );
        *T0 = Add_fr1x16 ( Mult_fr1x16 ( tmp, ONE_PER_3 ), D2 );    
      }
      
      /* New lag is not allowed to be bigger or smaller than last lag values */
      if ( Sub_fr1x16 ( *T0, maxLag ) > 0 )
      {
        *T0 = maxLag;              
      }

      if ( Sub_fr1x16 ( *T0, minLag ) < 0 )
      {
        *T0 = minLag;              
      }
    }
  }
}


WBAMR_CODE_SECTION void WBAMR_Insertion_Sort ( 
  fract16 array[],
  fract16 n
 )

{
  int_native i;

  for ( i = 0; i < n; i++ )
  {
    WBAMR_Insert ( array, ( fract16 )i, array[i] );
  }
}


WBAMR_CODE_SECTION void WBAMR_Insert ( 
  fract16 array[],
  fract16 n,
  fract16 x
 )

{
  int_native i;

  for ( i = ( fract16 ) ( n - 1 ); i >= 0; i-- )
  {
    if ( Sub_fr1x16 ( x, array[i] ) < 0 )
    {
      array[i + 1] = array[i];       
    }
    else
      break;
  }
  
  array[i + 1] = x;                      
}

#endif

