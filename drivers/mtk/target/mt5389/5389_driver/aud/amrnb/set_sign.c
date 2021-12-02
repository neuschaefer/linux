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
 *   set_sign.c
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
*      File             : set_sign.c
*      Purpose          : Builds sign vector according to "dn[]" and "cn[]".
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "set_sign.h"

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
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *
 *  FUNCTION  set_sign()
 *
 *  PURPOSE: Builds sign[] vector according to "dn[]" and "cn[]".
 *           Also finds the position of maximum of correlation in each track
 *           and the starting position for each pulse.
 *
 *************************************************************************/
void set_sign(Word16 dn[],   	/* i/o : correlation between target and h[]    */
              Word16 sign[], 		/* o   : sign of dn[]                          */
              Word16 dn2[],  		/* o   : maximum of correlation in each track. */
              Word16 n       		/* i   : # of maximum correlations in dn2[]    */
)
{
   Word32 i, j, k,val,min;
   Word32 pos = 0; /* initialization only needed to keep gcc silent */
	Word16 *ptr16dn,*ptr16dn2;

	ptr16dn  = dn;
	ptr16dn2	= dn2;

   /* set sign according to dn[] */
	i = L_CODE;
	do
	{
      val = *ptr16dn;

      if (val >= 0)
		{
         *sign = 32767;
      }
		else
      {
         *sign = -32767;
         val = -(val);
      }

		*ptr16dn2 = *ptr16dn  = val;     /* modify dn[] according to the fixed sign */

		sign++;	ptr16dn++;	ptr16dn2++;

   }while(--i != 0);

   /* keep 8-n maximum positions/8 of each track and store it in dn2[] */
   i = NB_TRACK-1;
	do
   {
		k = (8 - n);
		do
      {
			min = 0x7fff;
         for (j = i; j < L_CODE; j += STEP)
         {

            if (dn2[j] >= 0)
            {
               val = (dn2[j] - min);

               if (val < 0)
               {
                  min = dn2[j];
                  pos = j;
               }
            }
         }

         dn2[pos] = -1;

      }while(--k > 0);

   }while(--i >= 0);

   return;
}

/*************************************************************************
 *
 *  FUNCTION  set_sign12k2()
 *
 *  PURPOSE: Builds sign[] vector according to "dn[]" and "cn[]", and modifies
 *           dn[] to include the sign information (dn[i]=sign[i]*dn[i]).
 *           Also finds the position of maximum of correlation in each track
 *           and the starting position for each pulse.
 *
 *************************************************************************/
void set_sign12k2 (
   Word16 *ent,
    Word16 dn[],      /* i/o : correlation between target and h[]         */
    Word16 cn[],      /* i   : residual after long term prediction        */
    Word16 sign[],    /* o   : sign of d[n]                               */
    Word16 pos_max[], /* o   : position of maximum correlation            */
    Word16 nb_track,  /* i   : number of tracks tracks                    */
    Word16 ipos[],    /* o   : starting position for each pulse           */
    Word16 step       /* i   : the step size in the tracks                */
)
{
   Word32 i, j;
   Word32 s,t,*ptr32n;
   Word16 val, cor, k_cn, k_dn, max, max_of_all;
   Word16 pos = 0; /* initialization only needed to keep gcc silent */
   Word16 *en = ent;                  /* correlation vector */


    /* calculate energy for normalization of cn[] and dn[] */

	ptr32n = (Word32 *)cn;
   s = 256;
	i = L_CODE/4;

	do
   {
   	t = *ptr32n++;
   	s = qdadd(s,smulbb( t, t));
   	s = qdadd(s,smultt( t, t));
   	t = *ptr32n++;
   	s = qdadd(s,smulbb( t, t));
   	s = qdadd(s,smultt( t, t));
   }while(--i != 0);

   s = Inv_sqrt (s);
   k_cn = (Word16)(L_shl (s, 5) >> 16);


	ptr32n = (Word32 *)dn;
   s = 256;
	i = L_CODE/4;

#if (AMR_OPT_VER == 2)
	do
   {
   	t = *ptr32n++;
   	s = qdadd(s,smulbb( t, t));
   	s = qdadd(s,smultt( t, t));
   	t = *ptr32n++;
   	s = qdadd(s,smulbb( t, t));
   	s = qdadd(s,smultt( t, t));
   }while(--i != 0);
#elif (AMR_OPT_VER == 1)
	do
   {
   	t = *ptr32n++;
   	s = smlabb( t, t,s);
   	s = smlatt( t, t,s);
   	t = *ptr32n++;
   	s = smlabb( t, t,s);
   	s = smlatt( t, t,s);
   }while(--i != 0);
	s = qadd(s,s);
#endif


   s = Inv_sqrt (s);
   k_dn = (Word16)(L_shl (s, 5) >> 16);


   for (i = 0; i < L_CODE; i++)
   {
	   val = dn[i];

		t = L_mult(k_cn, cn[i]);
		t = qdadd(t, smulbb(k_dn, val));
      cor = qadd(t, 1<<5 ) >> 6;

      if (cor >= 0)
      {
 	     sign[i] = 32767;                      /* sign = +1 */
      }
      else
      {
   	   sign[i] = -32767;                     /* sign = -1 */
         cor = negate(cor);
         val = negate(val);
      }
      /* modify dn[] according to the fixed sign */
      dn[i] = val;
      en[i] = cor;
	}

   max_of_all = -1;
   for (i = 0; i < nb_track; i++)
   {
	   max = -1;

      for (j = i; j < L_CODE; j += step)
      {
   	   cor = en[j];
         val = (cor - max);

         if (val > 0)
         {
         	max = cor;
            pos = j;
         }
      }
      /* store maximum correlation position */
      pos_max[i] = pos;
      val = (max - max_of_all);

      if (val > 0)
      {
	      max_of_all = max;
         /* starting position for i0 */
         ipos[0] = i;
      }
	}

    /*----------------------------------------------------------------*
     *     Set starting position of each pulse.                       *
     *----------------------------------------------------------------*/

	pos = ipos[0];
   ipos[nb_track] = pos;

   for (i = 1; i < nb_track; i++)
   {
   	pos = (pos + 1);

      if(pos >= nb_track)
      {
      	pos = 0;
      }
      ipos[i] = pos;
      ipos[(i + nb_track)] = pos;
	}

}

#endif

