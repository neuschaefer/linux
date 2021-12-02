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
 *   WBAMR_Q_PULSE.C
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
 * $Revision: #1 $ 1.0.0
 * $Modtime:$  
 * $Log:$
 *
 * 12 23 2010 eddy.wu
 * [WCPSP00000503] Checkin SWIP Modification
 * .
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION
 
#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_q_pulse.h"
#include "wbamr_private.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION fract32 WBAMR_Quant_1p_N1(
  fract16 pos,       /* (i) position of the pulse */
  fract16 N         /* (i) number of bits for position */
)  
{
    
  fract16 mask; fract32 index;

  mask = Sub_fr1x16(Shl_fr1x16(1, N), 1); /* mask = ((1<<N)-1); */
  index = WBAMR_L_deposit_l((fract16) (pos & mask));
    
  if ((pos & NB_POS16) != 0)
  {
    index = Add_fr1x32(index, WBAMR_L_deposit_l(Shl_fr1x16(1, N))); 
  }

  return (index);
  
}


WBAMR_CODE_SECTION void WBAMR_Dec_1p_N1(
  fract32 index, 
  fract16 N, 
  fract16 offset, 
  fract16 pos[]
)
{

  fract16 pos1;
  fract32 mask, i;

  mask = WBAMR_L_deposit_l(Sub_fr1x16(Shl_fr1x16(1, N), 1));   
  
  /* pos1 = ((index & mask) + offset); */
  pos1 = Add_fr1x16(Extract_lo_fr1x32(index & mask), offset);  
  
  
  /* i = ((index >> N) & 1); */
  i = (Shr_fr1x32(index, N) & 1L);              
  
  if (Sub_fr1x32(i, 1) == 0)
  {
    pos1 = Add_fr1x16(pos1, NB_POS16);
  }
  
  pos[0] = pos1;                         

  return;
  
}


WBAMR_CODE_SECTION fract32 WBAMR_Quant_2p_2N1(
                                /* (o) return (2*N)+1 bits         */
  fract16 pos1,                 /* (i) position of the pulse 1     */
  fract16 pos2,                 /* (i) position of the pulse 2     */
  fract16 N                     /* (i) number of bits for position */
)  
{
    
  fract16 mask, tmp;
  fract32 index;

  /* mask = ((1<<N)-1); */
  mask = Sub_fr1x16(Shl_fr1x16(1, N), 1);       
  
  if (((pos2 ^ pos1) & NB_POS16) == 0)
  {
    /* sign of 1st pulse == sign of 2th pulse */
    if (Sub_fr1x16(pos1, pos2) <= 0)        
    {
      /* index = ((pos1 & mask) << N) + (pos2 & mask); */
      index = WBAMR_L_deposit_l(Add_fr1x16(Shl_fr1x16(
                ((fract16) (pos1 & mask)), N), ((fract16) (pos2 & mask))));
    } 
    else
    {
      /* ((pos2 & mask) << N) + (pos1 & mask); */
      index = WBAMR_L_deposit_l(Add_fr1x16(Shl_fr1x16((
                (fract16) (pos2 & mask)), N), ((fract16) (pos1 & mask))));
    }
        
    if ((pos1 & NB_POS16) != 0)
    {
      tmp = Shl_fr1x16(N, 1);
      index = Add_fr1x32(index, Shl_fr1x32(1L, tmp));  
    }
  } 
  else
  {
    /* sign of 1st pulse != sign of 2th pulse */
    if (Sub_fr1x16((fract16) (pos1 & mask), (fract16) (pos2 & mask)) <= 0)
    {
      /* index = ((pos2 & mask) << N) + (pos1 & mask); */
      index = WBAMR_L_deposit_l(Add_fr1x16(Shl_fr1x16(
            ((fract16) (pos2 & mask)), N), ((fract16) (pos1 & mask))));       
            
      if ((pos2 & NB_POS16) != 0)
      {
        tmp = Shl_fr1x16(N, 1);           /* index += 1 << (2*N); */
        index = Add_fr1x32(index, Shl_fr1x32(1L, tmp));
      }
    } 
    else
    {
      /* index = ((pos1 & mask) << N) + (pos2 & mask);	 */
      index = WBAMR_L_deposit_l(Add_fr1x16(Shl_fr1x16(
                ((fract16) (pos1 & mask)), N), ((fract16) (pos2 & mask))));       
            
      if ((pos1 & NB_POS16) != 0)
      {
        tmp = Shl_fr1x16(N, 1);
        index = Add_fr1x32(index, Shl_fr1x32(1, tmp));    
      }
    }
  }
  
  return (index);

}


WBAMR_CODE_SECTION void WBAMR_Dec_2p_2N1(
  fract32 index, 
  fract16 N, 
  fract16 offset, 
  fract16 pos[]
)
{

  fract16 pos1, pos2, tmp;
  fract32 mask, i;

  mask = WBAMR_L_deposit_l(Sub_fr1x16(Shl_fr1x16(1, N), 1)); 
  
  /* pos1 = (((index >> N) & mask) + offset); */
  pos1 = Extract_lo_fr1x32(Add_fr1x32((Shr_fr1x32(index, N) & mask), 
                    WBAMR_L_deposit_l(offset)));     
  
  tmp = Shl_fr1x16(N, 1);
  
  i = (Shr_fr1x32(index, tmp) & 1L);         
  
  pos2 = Add_fr1x16(Extract_lo_fr1x32(index & mask), offset);
    
  if (Sub_fr1x16(pos2, pos1) < 0)           /* ((pos2 - pos1) < 0) */
  {
    if (Sub_fr1x32(i, 1L) == 0)
    {                                       /* (i == 1) */
      pos1 = Add_fr1x16(pos1, NB_POS16);    /* pos1 += NB_POS16; */
    } 
    else
    {
      pos2 = Add_fr1x16(pos2, NB_POS16);    /* pos2 += NB_POS16;	 */
    }
  } 
  else
  {
    if (Sub_fr1x32(i, 1L) == 0)
    {                                       /* (i == 1) */
      pos1 = Add_fr1x16(pos1, NB_POS16);    /* pos1 += NB_POS16; */
      pos2 = Add_fr1x16(pos2, NB_POS16);    /* pos2 += NB_POS16; */
    }
  }

  pos[0] = pos1;                         
  pos[1] = pos2;                         

  return;

}


WBAMR_CODE_SECTION fract32 WBAMR_Quant_3p_3N1( 
                            /* (o) return (3*N)+1 bits         */
  fract16 pos1,             /* (i) position of the pulse 1     */
  fract16 pos2,             /* (i) position of the pulse 2     */
  fract16 pos3,             /* (i) position of the pulse 3     */
  fract16 N                 /* (i) number of bits for position */
)
{
    
  fract16 nb_pos;
  fract32 index;
  
  nb_pos = Shl_fr1x16(1, Sub_fr1x16(N, 1));   
   
  if (((pos1 ^ pos2) & nb_pos) == 0)
  {
    index = WBAMR_Quant_2p_2N1(pos1, pos2, Sub_fr1x16(N, 1));    
  
    /* index += (pos1 & nb_pos) << N; */
    index = Add_fr1x32(index, Shl_fr1x32(WBAMR_L_deposit_l(
                (fract16) (pos1 & nb_pos)), N));  
    
    /* index += WBAMR_Quant_1p_N1(pos3, N) << (2*N); */
    index = Add_fr1x32(index, Shl_fr1x32(
                WBAMR_Quant_1p_N1(pos3, N), Shl_fr1x16(N, 1)));

  } 
  else if (((pos1 ^ pos3) & nb_pos) == 0)
  {
    index = WBAMR_Quant_2p_2N1(pos1, pos3, Sub_fr1x16(N, 1));
    index = Add_fr1x32(index, Shl_fr1x32(
                WBAMR_L_deposit_l((fract16) (pos1 & nb_pos)), N));  
    
    /* index += (pos1 & nb_pos) << N; */
    index = Add_fr1x32(index, Shl_fr1x32(
                WBAMR_Quant_1p_N1(pos2, N), Shl_fr1x16(N, 1)));
  } 
  else
  {
    index = WBAMR_Quant_2p_2N1(pos2, pos3, Sub_fr1x16(N, 1)); 
    index = Add_fr1x32(index, Shl_fr1x32(
                WBAMR_L_deposit_l((fract16) (pos2 & nb_pos)), N));  
    
    index = Add_fr1x32(index, Shl_fr1x32(
                WBAMR_Quant_1p_N1(pos1, N), Shl_fr1x16(N, 1)));
  }
  
  return (index);
  
}


WBAMR_CODE_SECTION void WBAMR_Dec_3p_3N1(
  fract32 index, 
  fract16 N, 
  fract16 offset, 
  fract16 pos[]
)
{

  fract16 j, tmp;
  fract32 mask, idx;

  tmp = Sub_fr1x16(Shl_fr1x16(N, 1), 1);  /* mask = ((1<<((2*N)-1))-1); */
  mask = Sub_fr1x32(Shl_fr1x32(1L, tmp), 1L);
  idx = index & mask;                    
  j = offset;
  tmp = Sub_fr1x16(Shl_fr1x16(N, 1), 1);
  
  if ((Shr_fr1x32(index, tmp) & 1L) != 0L)
  {  
    /* if (((index >> ((2*N)-1)) & 1) == 1){ */
    j = Add_fr1x16(j, Shl_fr1x16(1, Sub_fr1x16(N, 1))); /* j += (1<<(N-1)); */
  }
  
  WBAMR_Dec_2p_2N1(idx, (fract16) (N - 1), j, pos);

  mask = Sub_fr1x16(Shl_fr1x16(1, Add_fr1x16(N, 1)), 1); 
  tmp = Shl_fr1x16(N, 1);     /* idx = (index >> (2*N)) & mask; */
  idx = Shr_fr1x32(index, tmp) & mask;        

  WBAMR_Dec_1p_N1(idx, N, offset, pos + 2);    

  return;
  
}


WBAMR_CODE_SECTION fract32 WBAMR_Quant_4p_4N1( 
                            /* (o) return (4*N)+1 bits         */
  fract16 pos1,             /* (i) position of the pulse 1     */
  fract16 pos2,             /* (i) position of the pulse 2     */
  fract16 pos3,             /* (i) position of the pulse 3     */
  fract16 pos4,             /* (i) position of the pulse 4     */
  fract16 N                 /* (i) number of bits for position */
)
{
  
  fract16 nb_pos;
  fract32 index;
  nb_pos = Shl_fr1x16(1, Sub_fr1x16(N, 1)); /* nb_pos = (1<<(N-1));  */
    
  if (((pos1 ^ pos2) & nb_pos) == 0)
  {
    /* index = WBAMR_Quant_2p_2N1(pos1, pos2, (N-1)); */
    index = WBAMR_Quant_2p_2N1(pos1, pos2, Sub_fr1x16(N, 1)); 
    
    /* index += (pos1 & nb_pos) << N;	 */
    index = Add_fr1x32(index, Shl_fr1x32(
                    WBAMR_L_deposit_l((fract16) (pos1 & nb_pos)), N));  
    
    /* index += WBAMR_Quant_2p_2N1(pos3, pos4, N) << (2*N); */
    index = Add_fr1x32(index, Shl_fr1x32(
                    WBAMR_Quant_2p_2N1(pos3, pos4, N), Shl_fr1x16(N, 1)));
  } 
  else if (((pos1 ^ pos3) & nb_pos) == 0)
  {
    index = WBAMR_Quant_2p_2N1(pos1, pos3, Sub_fr1x16(N, 1));
  
    /* index += (pos1 & nb_pos) << N; */
    index = Add_fr1x32(index, Shl_fr1x32(
                WBAMR_L_deposit_l((fract16) (pos1 & nb_pos)), N));  
    
    /* index += WBAMR_Quant_2p_2N1(pos2, pos4, N) << (2*N); */
    index = Add_fr1x32(index, Shl_fr1x32(
                WBAMR_Quant_2p_2N1(pos2, pos4, N), Shl_fr1x16(N, 1)));
  } 
  else
  {
    index = WBAMR_Quant_2p_2N1(pos2, pos3, Sub_fr1x16(N, 1));
    
    /* index += (pos2 & nb_pos) << N; */
    index = Add_fr1x32(index, Shl_fr1x32(
                WBAMR_L_deposit_l((fract16) (pos2 & nb_pos)), N));  
    
    /* index += WBAMR_Quant_2p_2N1(pos1, pos4, N) << (2*N); */
    index = Add_fr1x32(index, Shl_fr1x32(
                WBAMR_Quant_2p_2N1(pos1, pos4, N), Shl_fr1x16(N, 1)));
  }
  
  return (index);
  
}


WBAMR_CODE_SECTION void WBAMR_Dec_4p_4N1(
  fract32 index, 
  fract16 N, 
  fract16 offset, 
  fract16 pos[]
)
{

  fract16 j, tmp;
  fract32 mask, idx;
 
  tmp = Sub_fr1x16(Shl_fr1x16(N, 1), 1);    /* mask = ((1<<((2*N)-1))-1); */
  mask = Sub_fr1x32(Shl_fr1x32(1L, tmp), 1L);
  idx = index & mask;                    
  j = offset;                            
  tmp = Sub_fr1x16(Shl_fr1x16(N, 1), 1);
    
  if ((Shr_fr1x32(index, tmp) & 1L) != 0L)
  {    
    /* (((index >> ((2*N)-1)) & 1) == 1) */
    j = Add_fr1x16(j, Shl_fr1x16(1, Sub_fr1x16(N, 1)));    
    	 
  }
  
  WBAMR_Dec_2p_2N1(idx, (fract16) (N - 1), j, pos);
  tmp = Add_fr1x16(Shl_fr1x16(N, 1), 1);      /* mask = ((1<<((2*N)+1))-1); */
  mask = Sub_fr1x32(Shl_fr1x32(1L, tmp), 1L);
  idx = Shr_fr1x32(index, Shl_fr1x16(N, 1)) & mask;  
  
  /* WBAMR_Dec_2p_2N1(idx, N, offset, pos+2); */
  WBAMR_Dec_2p_2N1(idx, N, offset, pos + 2); 

  return;
  
}


WBAMR_CODE_SECTION fract32 WBAMR_Quant_4p_4N( 
  fract16 pos[],       /* (i) position of the pulse 1..4  */
  fract16 N,           /* (i) number of bits for position */
  fract16 posA1[],                   
  fract16 posB1[]                 
 )
{
  
  int_native i, j, k;
  fract16 nb_pos, n_1, tmp;
  fract32 index;

  n_1 = (fract16) (N - 1);                
  nb_pos = Shl_fr1x16(1, n_1);              /* nb_pos = (1<<n_1); */
  //mask = Sub_fr1x16(Shl_fr1x16(1, N), 1);   /* mask = ((1<<N)-1); */

  i = 0;                                 
  j = 0;                                 
  
  for (k = 0; k < 4; k++)
  {
    if ((pos[k] & nb_pos) == 0)
    {
      posA1[i++] = pos[k];            
    } 
    else
    {
      posB1[j++] = pos[k];            
    }
  }

  switch (i)
  {
    case 0:
      tmp = Sub_fr1x16(Shl_fr1x16(N, 2), 3);     
      index = Shl_fr1x32(1L, tmp);
      
      /* index += WBAMR_Quant_4p_4N1(posB[0], posB[1], posB[2], posB[3], n_1)*/
      index = Add_fr1x32(index, WBAMR_Quant_4p_4N1(posB1[0], 
                        posB1[1], posB1[2], posB1[3], n_1));
      break;
    
    case 1:
      /* index = WBAMR_Quant_1p_N1(posA[0], n_1) << ((3*n_1)+1); */
      tmp = Add_fr1x16(Extract_lo_fr1x32(Shr_fr1x32(
                        Mult_fr1x32(3, n_1), 1)), 1);
      
      index = Shl_fr1x32(WBAMR_Quant_1p_N1(posA1[0], n_1), tmp);
      
      /* index += WBAMR_Quant_3p_3N1(posB[0], posB[1], posB[2], n_1); */
      index = Add_fr1x32(index, WBAMR_Quant_3p_3N1(posB1[0], 
                        posB1[1], posB1[2], n_1));
      break;
    
    case 2:
      tmp = Add_fr1x16(Shl_fr1x16(n_1, 1), 1);     
      index = Shl_fr1x32(WBAMR_Quant_2p_2N1(posA1[0], posA1[1], n_1), tmp);
      
      /* index += WBAMR_Quant_2p_2N1(posB[0], posB[1], n_1); */
      index = Add_fr1x32(index, WBAMR_Quant_2p_2N1(posB1[0], posB1[1], n_1));
      
      break;
    
    case 3:
      /* index = WBAMR_Quant_3p_3N1(posA[0], posA[1], posA[2], n_1) << N; */
      index = Shl_fr1x32(WBAMR_Quant_3p_3N1(posA1[0], 
                            posA1[1], posA1[2], n_1), N);
      
      index = Add_fr1x32(index, WBAMR_Quant_1p_N1(posB1[0], n_1)); 
      
      break;
    
    case 4:
      index = WBAMR_Quant_4p_4N1(posA1[0], posA1[1], posA1[2], posA1[3], n_1);
    
      break;
    
    default:
      index = 0;
  }
  
  tmp = Sub_fr1x16(Shl_fr1x16(N, 2), 2);     
  index = Add_fr1x32(index, Shl_fr1x32((
                WBAMR_L_deposit_l((fract16)i) & (3L)), tmp));  

  return (index);
  
}


WBAMR_CODE_SECTION void WBAMR_Dec_4p_4N(
  fract32 index, 
  fract16 N, 
  fract16 offset, 
  fract16 pos[]
)
{

  fract16 j, n_1, tmp;

  n_1 = (fract16) (N - 1);                
  j = Add_fr1x16(offset, Shl_fr1x16(1, n_1));  
  tmp = Sub_fr1x16(Shl_fr1x16(N, 2), 2);
  switch (Shr_fr1x32(index, tmp) & 3)
    {            
      /* ((index >> ((4*N)-2)) & 3) */
      case 0:
        tmp = Add_fr1x16(Shl_fr1x16(n_1, 2), 1);
        
        if ((Shr_fr1x32(index, tmp) & 1) == 0)
        {  
           /* (((index >> ((4*n_1)+1)) & 1) == 0) */
           WBAMR_Dec_4p_4N1(index, n_1, offset, pos);
        } 
        else
        {
          WBAMR_Dec_4p_4N1(index, n_1, j, pos);
        }
        break;
    
    case 1:
      tmp = Add_fr1x16(Extract_lo_fr1x32(
                        Shr_fr1x32(Mult_fr1x32(3, n_1), 1)), 1); 
      
      WBAMR_Dec_1p_N1(Shr_fr1x32(index, tmp), n_1, offset, pos);
      WBAMR_Dec_3p_3N1(index, n_1, j, pos + 1);
      break;
    
    case 2:
      tmp = Add_fr1x16(Shl_fr1x16(n_1, 1), 1);  
      WBAMR_Dec_2p_2N1(Shr_fr1x32(index, tmp), n_1, offset, pos);
      WBAMR_Dec_2p_2N1(index, n_1, j, pos + 2);
      break;
      
    case 3:
      tmp = Add_fr1x16(n_1, 1);
      WBAMR_Dec_3p_3N1(Shr_fr1x32(index, tmp), n_1, offset, pos);
      WBAMR_Dec_1p_N1(index, n_1, j, pos + 3); 
      break;
      
  }
  
  return;
  
}


WBAMR_CODE_SECTION fract32 WBAMR_Quant_5p_5N(
  fract16 pos[],             /* (i) position of the pulse 1..5  */
  fract16 N,                 /* (i) number of bits for position */
  fract16 posA1[],                   
  fract16 posB1[]                 
)
{
    
  int_native i, j, k;
  fract16 nb_pos, n_1, tmp;
  fract32 index, tmp2;

  n_1 = (fract16) (N - 1);                
  nb_pos = Shl_fr1x16(1, n_1);            /* nb_pos = (1<<n_1); */

  i = 0;                                 
  j = 0;                                 
  
  for (k = 0; k < 5; k++)
  {
    if ((pos[k] & nb_pos) == 0)
    {
      posA1[i++] = pos[k];            
    } 
    else
    {
      posB1[j++] = pos[k];            
    }
  }

  switch (i)
  {
    case 0:
      tmp = Sub_fr1x16(Extract_lo_fr1x32(Shr_fr1x32(Mult_fr1x32(5, N), 1)), 1);
      index = Shl_fr1x32(1L, tmp);            /* index = 1 << ((5*N)-1); */
      tmp = Add_fr1x16(Shl_fr1x16(N, 1), 1);  
      
      tmp2 = Shl_fr1x32(WBAMR_Quant_3p_3N1(posB1[0], 
                    posB1[1], posB1[2], n_1), tmp);
      
      index = Add_fr1x32(index, tmp2);
      index = Add_fr1x32(index, WBAMR_Quant_2p_2N1(posB1[3], posB1[4], N));       
      break;
    
    case 1:
      tmp = Sub_fr1x16(Extract_lo_fr1x32(Shr_fr1x32(Mult_fr1x32(5, N), 1)), 1);
      index = Shl_fr1x32(1L, tmp);
      tmp = Add_fr1x16(Shl_fr1x16(N, 1), 1);    
      
      tmp2 = Shl_fr1x32(WBAMR_Quant_3p_3N1(posB1[0], posB1[1],
                         posB1[2], n_1), tmp);
      
      index = Add_fr1x32(index, tmp2);
      index = Add_fr1x32(index, WBAMR_Quant_2p_2N1(posB1[3], posA1[0], N));      
      break;
    
    case 2:
      tmp = Sub_fr1x16(Extract_lo_fr1x32(Shr_fr1x32(Mult_fr1x32(5, N), 1)), 1);
      index = Shl_fr1x32(1L, tmp);     
      tmp = Add_fr1x16(Shl_fr1x16(N, 1), 1);          
      
      tmp2 = Shl_fr1x32(WBAMR_Quant_3p_3N1(posB1[0], posB1[1], 
                            posB1[2], n_1), tmp);
      
      index = Add_fr1x32(index, tmp2);
      index = Add_fr1x32(index, WBAMR_Quant_2p_2N1(posA1[0], posA1[1], N));       
      break;
    
    case 3:
      tmp = Add_fr1x16(Shl_fr1x16(N, 1), 1); 
      index = Shl_fr1x32(WBAMR_Quant_3p_3N1(posA1[0], 
                    posA1[1], posA1[2], n_1), tmp);
      index = Add_fr1x32(index, WBAMR_Quant_2p_2N1(posB1[0], posB1[1], N));     
      break;
    
    case 4:
      tmp = Add_fr1x16(Shl_fr1x16(N, 1), 1);   
      index = Shl_fr1x32(WBAMR_Quant_3p_3N1(posA1[0],
                             posA1[1], posA1[2], n_1), tmp);
      index = Add_fr1x32(index, WBAMR_Quant_2p_2N1(posA1[3], posB1[0], N));       
      break;
    
    /* NOTE: if i is more than 4, compiler is creating temporary local buffer
       in section data1. To avoid this, outside if comparisions are used */          

    default:
      index = 0;
      
  }
    
  if(i==5)
  {    
    tmp = Add_fr1x16(Shl_fr1x16(N, 1), 1);           
    index = Shl_fr1x32(WBAMR_Quant_3p_3N1(posA1[0], 
                        posA1[1], posA1[2], n_1), tmp);
    
    index = Add_fr1x32(index, WBAMR_Quant_2p_2N1(posA1[3], posA1[4], N));      
  }
             
  return (index);
  
}


WBAMR_CODE_SECTION void WBAMR_Dec_5p_5N(
  fract32 index, 
  fract16 N, 
  fract16 offset, 
  fract16 pos[]
)
{
    
  fract16 j, n_1, tmp;
  fract32 idx;

  n_1 = (fract16) (N - 1);                
  j = Add_fr1x16(offset, Shl_fr1x16(1, n_1));  /* j = offset + (1 << n_1); */
  tmp = Add_fr1x16(Shl_fr1x16(N, 1), 1);       /* idx = (index >> ((2*N)+1))*/
  idx = Shr_fr1x32(index, tmp);
  tmp = Sub_fr1x16(Extract_lo_fr1x32(Shr_fr1x32(Mult_fr1x32(5, N), 1)), 1);   

  if ((Shr_fr1x32(index, tmp) & 1) == 0)      
  {
    /* ((index >> ((5*N)-1)) & 1)  */
    WBAMR_Dec_3p_3N1(idx, n_1, offset, pos);
    WBAMR_Dec_2p_2N1(index, N, offset, pos + 3);  
  } 
  else
  {
    WBAMR_Dec_3p_3N1(idx, n_1, j, pos);
    WBAMR_Dec_2p_2N1(index, N, offset, pos + 3);  
  }
  
  return;
  
}


WBAMR_CODE_SECTION fract32 WBAMR_Quant_6p_6N_2(
  fract16 pos[],           /* (i) position of the pulse 1..6  */
  fract16 N,               /* (i) number of bits for position */
  fract16 posA[],                   
  fract16 posB[],                 
  fract16 posA1[],                   
  fract16 posB1[]                 
)
{
    
  int_native i, j, k;
  fract16 nb_pos, n_1;
  fract32 index;
  
  /* !!  N and n_1 are constants -> 
   * it doesn't need to be operated by Basic Operators 
   */
  n_1 = (fract16) (N - 1);                
  nb_pos = Shl_fr1x16(1, n_1);            /* nb_pos = (1<<n_1); */

  i = 0;                                 
  j = 0;                                 
  
  for (k = 0; k < 6; k++)
  {
    if ((pos[k] & nb_pos) == 0)
    {
      posA[i++] = pos[k];            
    } 
    else
    {
      posB[j++] = pos[k];            
    }
  }

  switch (i)
  {
    case 0:
      index = Shl_fr1x32(1L, (fract16) (6 * N - 5));        
      index = Add_fr1x32(index, Shl_fr1x32(WBAMR_Quant_5p_5N(
                    posB, n_1, posA1, posB1), N)); 
      
      /* index += WBAMR_Quant_1p_N1(posB[5], n_1); */
      index = Add_fr1x32(index, WBAMR_Quant_1p_N1(posB[5], n_1));        
      break;
    
    case 1:
      index = Shl_fr1x32(1L, (fract16) (6 * N - 5));    
      index = Add_fr1x32(index, Shl_fr1x32(WBAMR_Quant_5p_5N(posB, 
                        n_1, posA1, posB1), N)); 
      
      index = Add_fr1x32(index, WBAMR_Quant_1p_N1(posA[0], n_1));   
      break;
      
    case 2:
      index = Shl_fr1x32(1L, (fract16) (6 * N - 5));  
      
      /* index += WBAMR_Quant_4p_4N(posB, n_1) << ((2*n_1)+1); */
      index = Add_fr1x32(index, Shl_fr1x32(WBAMR_Quant_4p_4N(posB, 
                        n_1, posA1, posB1), (fract16) (2 * n_1 + 1)));
      
      index = Add_fr1x32(index, WBAMR_Quant_2p_2N1(posA[0], posA[1], n_1)); 
      break;
    
    case 3:
      index = Shl_fr1x32(WBAMR_Quant_3p_3N1(posA[0], 
                    posA[1], posA[2], n_1), (fract16) (3 * n_1 + 1)); 
                       
      index = Add_fr1x32(index, WBAMR_Quant_3p_3N1(posB[0], posB[1], posB[2], n_1));    
      break;
      
    case 4:
      i = 2;                             
      index = Shl_fr1x32(WBAMR_Quant_4p_4N(posA, n_1, 
                        posA1, posB1), (fract16) (2 * n_1 + 1));  
      
      index = Add_fr1x32(index, WBAMR_Quant_2p_2N1(posB[0], posB[1], n_1)); 
      break;
    
    /* NOTE: if i is more than 4, compiler is creating temporary local buffer
       in section data1. To avoid this, outside if comparisions are used */       

    default:
      index = 0;
  }

  if(i==5)
  {
  	i = 1;                             
    index = Shl_fr1x32(WBAMR_Quant_5p_5N(posA, n_1, posA1, posB1), N);   
    index = Add_fr1x32(index, WBAMR_Quant_1p_N1(posB[0], n_1));          
  }
  else if(i==6)
  {
  	i = 0;                             
    index = Shl_fr1x32(WBAMR_Quant_5p_5N(posA, n_1, posA1, posB1), N);    
    index = Add_fr1x32(index, WBAMR_Quant_1p_N1(posA[5], n_1));           
  }
   
  index = Add_fr1x32(index, Shl_fr1x32(
                (WBAMR_L_deposit_l((fract16)i) & 3L), (fract16) (6 * N - 4)));  

  return (index);
  
}


WBAMR_CODE_SECTION void WBAMR_Dec_6p_6N_2(
  fract32 index, 
  fract16 N, 
  fract16 offset, 
  fract16 pos[]
)
{

  fract16 j, n_1, offsetA, offsetB;

  n_1 = (fract16) (N - 1);                
  j = Add_fr1x16(offset, Shl_fr1x16(1, n_1));         

  offsetA = offsetB = j;                 
  
  if ((Shr_fr1x32(index, (fract16) (6 * N - 5)) & 1L) == 0)
  {                                     
    offsetA = offset;                  
  } 
  else
  {
    offsetB = offset;                  
  }
    
  switch (Shr_fr1x32(index, (fract16) (6 * N - 4)) & 3)
  { 
    /* (index >> ((6*N)-4)) & 3 */
    case 0:
      WBAMR_Dec_5p_5N(Shr_fr1x32(index, N), n_1, offsetA, pos);  
      WBAMR_Dec_1p_N1(index, n_1, offsetA, pos + 5);        
      break;
    
    case 1:
      WBAMR_Dec_5p_5N(Shr_fr1x32(index, N), n_1, offsetA, pos);  
      WBAMR_Dec_1p_N1(index, n_1, offsetB, pos + 5);        
      break;
    
    case 2:
      WBAMR_Dec_4p_4N(Shr_fr1x32(index, (fract16) (2 * n_1 + 1)),
                             n_1, offsetA, pos); 
                             
      WBAMR_Dec_2p_2N1(index, n_1, offsetB, pos + 4);       
      break;
    
    case 3:
      WBAMR_Dec_3p_3N1(Shr_fr1x32(index, (fract16) (3 * n_1 + 1)), 
                                n_1, offset, pos); 
                                
      WBAMR_Dec_3p_3N1(index, n_1, j, pos + 3);
      break;
    
  }
  
  return;
  
}

#endif

