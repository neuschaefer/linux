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
 *   s10_8pf.c
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
 * $Revision: #2 $ 1.0.0
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
*      File             : s10_8pf.c
*      Purpose          : Searches a 35/31 bit algebraic codebook containing
*                       : 10/8 pulses in a frame of 40 samples.
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "s10_8pf.h"

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

/*************************************************************************
 *
 *  FUNCTION  search_10and8i40()
 *
 *  PURPOSE: Search the best codevector; determine positions of the 10/8
 *           pulses in the 40-sample frame.
 *
 *   search_10and8i40 (10,5,5,dn, rr, ipos, pos_max, codvec);   for GSMEFR
 *   search_10and8i40 (8, 4,4,dn, rr, ipos, pos_max, codvec);   for 10.2
 *
 *************************************************************************/

#define _1_2    (Word16)(32768L/2)
#define _1_4    (Word16)(32768L/4)
#define _1_8    (Word16)(32768L/8)
#define _1_16   (Word16)(32768L/16)
#define _1_32   (Word16)(32768L/32)
#define _1_64   (Word16)(32768L/64)
#define _1_128  (Word16)(32768L/128)


void search_10and8i40 (
    Word16 nbPulse,      /* i : nbpulses to find                       */
    Word16 step,         /* i :  stepsize                              */
    Word16 nbTracks,     /* i :  nbTracks                              */
    Word16 dn[],         /* i : correlation between target and h[]     */
    Word16 rr[][L_CODE], /* i : matrix of autocorrelation              */
    Word16 ipos[],       /* i : starting position for each pulse       */
    Word16 pos_max[],    /* i : position of maximum of dn[]            */
    Word16 codvec[]      /* o : algebraic codebook vector              */
)
{
	Word32 i, j, k;
   Word32 s, alp0, alp1, alp2;
   Word32 i0, i1, i2, i3, i4, i5, i6, i7, i8, i9;
   Word32 pos, ia, ib;
   Word32 psk, ps, ps0, ps1, ps2, sq, sq2;
   Word32 alpk, alp, alp_16;
   Word32 gsmefrFlag;
   Word16 rrv[L_CODE];
	Word16 *ptr16_i0,*ptr16_i1,*ptr16_i2,*ptr16_i3;


   if ((nbPulse == 10) )
   {
      gsmefrFlag=1;
   }
   else
   {
      gsmefrFlag=0;
   }

   /* fix i0 on maximum of correlation position */
   i0 = pos_max[ipos[0]];

   /*------------------------------------------------------------------*
    * i1 loop:                                                         *
    *------------------------------------------------------------------*/

   /* Default value */
   psk = -1;
   alpk = 1;

	i = nbPulse-1;
	do
   {
      codvec[i] = i;
      codvec[i-1] = i-1;
		i-=2;
   }while(i>=0);


   for (i = 1; i < nbTracks; i++)
   {
      i1 = pos_max[ipos[1]];


		ptr16_i0 = &(rr[i0][0]);
		ptr16_i1 = &(rr[i1][0]);

      alp0 = smulbb(*(ptr16_i0+i0), _1_16);
      alp0 = smlabb( *(ptr16_i1+i1), _1_16 ,alp0);
      alp0 = smlabb( *(ptr16_i0+i1), _1_8  ,alp0);

      /*----------------------------------------------------------------*
       * i2 and i3 loop:                                                *
       *----------------------------------------------------------------*/
      /* initialize 4 indices for next loop. */
      /* initialize "rr[i3][i3]" pointer */
      /* initialize "rr[i0][i3]" pointer */
      /* initialize "rr[i1][i3]" pointer */
      /* initialize "rrv[i3]" pointer    */
		i3 = ipos[3];
		ptr16_i3 = &(rr[i3][i3]);

		do
      {
         s = smulbb(*ptr16_i3, _1_8);       /* index incr= step+L_CODE */
			ptr16_i3 += (step*L_CODE+step);
//			s = smulbb( rr[i3][i3], _1_8);

         s = smlabb( rr[i0][i3], _1_4 ,s);     /* index increment = step  */
         s = smlabb( rr[i1][i3], _1_4 ,s);     /* index increment = step  */

			s = qadd(s,s);
         rrv[i3] = AMR_round(s);                 

			i3 += step;
      }while(i3 < L_CODE);


      /* Default value */
      sq = -1;
      alp = 1;
      ps = 0;
      ia = ipos[2];
      ib = ipos[3];

      /* initialize 4 indices for i2 loop. */
       /* initialize "dn[i2]" pointer     */
       /* initialize "rr[i2][i2]" pointer */
       /* initialize "rr[i0][i2]" pointer */
       /* initialize "rr[i1][i2]" pointer */

      ps0 = (dn[i0] + dn[i1]);
      for (i2 = ipos[2]; i2 < L_CODE; i2 += step)
      {
         /* index increment = step  */
         /* index incr= step+L_CODE */

//         alp1 = smlabb( rr[i2][i2], _1_16 , alp0);
         alp1 = smlabb( rr[i2][i2], _1_16 , alp0);

         /* index increment = step  */
         alp1 = smlabb( rr[i0][i2], _1_8 , alp1);
         /* index increment = step  */
         alp1 = smlabb( rr[i1][i2], _1_8 , alp1);


         /* initialize 3 indices for i3 inner loop */
         /* initialize "dn[i3]" pointer     */
         /* initialize "rrv[i3]" pointer    */
         /* initialize "rr[i2][i3]" pointer */
			i3 = ipos[3];
			ptr16_i2 = &(rr[i2][i3]);
         ps1 = (ps0 + dn[i2]);

			do
         {
            /* index increment = step */
            ps2 = (ps1 + dn[i3]);

            /* index increment = step */
            alp2 = smlabb( rrv[i3], _1_2 , alp1);

            alp2 = smlabb( *ptr16_i2, _1_8 ,alp2);
				ptr16_i2 += step;


				alp2 = qadd(alp2,alp2);

            sq2 = smulbb(ps2, ps2) >> 15;
            alp_16 = AMR_round (alp2);            

            if ((alp*sq2) > (sq*alp_16))
            {
               sq = sq2;
               ps = ps2;
               alp = alp_16;
               ia = i2;
               ib = i3;
            }

				i3 += step;
         }while(i3 < L_CODE);



      }

      i2 = ia;
      i3 = ib;

      /*----------------------------------------------------------------*
       * i4 and i5 loop:                                                *
       *----------------------------------------------------------------*/
      ps0 = ps;
      alp0 = smulbb(alp, _1_2);

      /* initialize 6 indices for next loop (see i2-i3 loop) */
		for (i5 = ipos[5]; i5 < L_CODE; i5 += step)
      {
      	s = smulbb(rr[i5][i5], _1_8);
         s = smlabb(rr[i0][i5], _1_4,s);
         s = smlabb(rr[i1][i5], _1_4,s);
         s = smlabb(rr[i2][i5], _1_4,s);
         s = smlabb(rr[i3][i5], _1_4,s);
			s = qadd(s,s);
         rrv[i5] = AMR_round (s);
      }

      /* Default value */
      sq = -1;
      alp = 1;
      ps = 0;
      ia = ipos[4];
      ib = ipos[5];

      /* initialize 6 indices for i4 loop (see i2-i3 loop) */

		i4 = ipos[4];
		do
      {
         alp1 = smlabb( rr[i4][i4], _1_32 , alp0);
         alp1 = smlabb( rr[i0][i4], _1_16 , alp1);
         alp1 = smlabb( rr[i1][i4], _1_16 , alp1);
         alp1 = smlabb( rr[i2][i4], _1_16 , alp1);
         alp1 = smlabb( rr[i3][i4], _1_16 , alp1);

	      ps1 = (ps0 + dn[i4]);
         /* initialize 3 indices for i5 inner loop (see i2-i3 loop) */

			i5 = ipos[5];
			do
         {
   	      ps2 = (ps1 + dn[i5]);
            alp2 = smlabb( rrv[i5], _1_4 , alp1);
            alp2 = smlabb( rr[i4][i5], _1_16 , alp2);
				alp2 = qadd(alp2,alp2);
            sq2 = smulbb(ps2, ps2) >> 15;
            alp_16 = AMR_round(alp2);

            if ((alp*sq2) > (sq*alp_16))
            {
      	      sq = sq2;
               ps = ps2;
               alp = alp_16;
               ia = i4;
               ib = i5;
            }
				i5 += step;
			}while(i5 < L_CODE);
			i4 += step;
		}while(i4 < L_CODE);

      i4 = ia;
      i5 = ib;
      /*----------------------------------------------------------------*
       * i6 and i7 loop:                                                *
       *----------------------------------------------------------------*/
      ps0 = ps;
      alp0 = smulbb(alp, _1_2);

      /* initialize 8 indices for next loop (see i2-i3 loop) */
		for (i7 = ipos[7]; i7 < L_CODE; i7 += step)
      {
      	s = smulbb(rr[i7][i7], _1_16);
         s = smlabb(rr[i0][i7], _1_8 ,s);
         s = smlabb(rr[i1][i7], _1_8 ,s);
         s = smlabb(rr[i2][i7], _1_8 ,s);
         s = smlabb(rr[i3][i7], _1_8 ,s);
         s = smlabb(rr[i4][i7], _1_8 ,s);
         s = smlabb(rr[i5][i7], _1_8 ,s);
			s = qadd(s,s);
         rrv[i7] = AMR_round (s);
		}

      /* Default value */
      sq = -1;
      alp = 1;
      ps = 0;
      ia = ipos[6];
      ib = ipos[7];

      /* initialize 8 indices for i6 loop (see i2-i3 loop) */
      for (i6 = ipos[6]; i6 < L_CODE; i6 += step)
      {
         alp1 = smlabb( rr[i6][i6], _1_64, alp0);
         alp1 = smlabb( rr[i0][i6], _1_32, alp1);
         alp1 = smlabb( rr[i1][i6], _1_32, alp1);
         alp1 = smlabb( rr[i2][i6], _1_32, alp1);
         alp1 = smlabb( rr[i3][i6], _1_32, alp1);
         alp1 = smlabb( rr[i4][i6], _1_32, alp1);
         alp1 = smlabb( rr[i5][i6], _1_32, alp1);

	      ps1 = (ps0 + dn[i6]);

         /* initialize 3 indices for i7 inner loop (see i2-i3 loop) */

			i7 = ipos[7];
			do
         {
	         ps2 = (ps1 + dn[i7]);

            alp2 = smlabb(rrv[i7], _1_4 , alp1);
            alp2 = smlabb(rr[i6][i7], _1_32 , alp2);
				alp2 = qadd(alp2,alp2);

            sq2 = smulbb(ps2, ps2) >> 15;
            alp_16 = AMR_round(alp2);

            if ((alp*sq2) > (sq*alp_16))
            {
	            sq = sq2;
               ps = ps2;
               alp = alp_16;
               ia = i6;
               ib = i7;
            }
				i7 += step;
         }while(i7 < L_CODE);

		}

      i6 = ia;
      i7 = ib;
      /* now finished searching a set of 8 pulses */

      if(gsmefrFlag != 0)
		{
      	/* go on with the two last pulses for GSMEFR                      */
         /*----------------------------------------------------------------*
          * i8 and i9 loop:                                                *
          *----------------------------------------------------------------*/

         ps0 = ps;
         alp0 = smulbb(alp, _1_2);

         /* initialize 10 indices for next loop (see i2-i3 loop) */
         for (i9 = ipos[9]; i9 < L_CODE; i9 += step)
         {
	         s = smulbb(rr[i9][i9], _1_16);
            s = smlabb( rr[i0][i9], _1_8, s);
            s = smlabb( rr[i1][i9], _1_8, s);
            s = smlabb( rr[i2][i9], _1_8, s);
            s = smlabb( rr[i3][i9], _1_8, s);
            s = smlabb( rr[i4][i9], _1_8, s);
            s = smlabb( rr[i5][i9], _1_8, s);
            s = smlabb( rr[i6][i9], _1_8, s);
            s = smlabb( rr[i7][i9], _1_8, s);
            s = qadd(s,s);
            rrv[i9] = AMR_round (s);
         }


         /* Default value */
         sq = -1;
         alp = 1;
         ps = 0;
         ia = ipos[8];
         ib = ipos[9];

         /* initialize 10 indices for i8 loop (see i2-i3 loop) */

			i8 = ipos[8];
			do
         {
            alp1 = smlabb( rr[i8][i8], _1_128, alp0);
            alp1 = smlabb( rr[i0][i8], _1_64, alp1);
            alp1 = smlabb( rr[i1][i8], _1_64, alp1);
            alp1 = smlabb( rr[i2][i8], _1_64, alp1);
            alp1 = smlabb( rr[i3][i8], _1_64, alp1);
            alp1 = smlabb( rr[i4][i8], _1_64, alp1);
            alp1 = smlabb( rr[i5][i8], _1_64, alp1);
            alp1 = smlabb( rr[i6][i8], _1_64, alp1);
            alp1 = smlabb( rr[i7][i8], _1_64, alp1);

	         ps1 = (ps0 + dn[i8]);

            /* initialize 3 indices for i9 inner loop (see i2-i3 loop) */

				i9 = ipos[9];
				do
            {
   	         ps2 = (ps1 + dn[i9]);

               alp2 = smlabb( rrv[i9], _1_8 , alp1);
               alp2 = smlabb( rr[i8][i9], _1_64 , alp2);
					alp2 = qadd(alp2,alp2);

               sq2 = smulbb(ps2, ps2) >> 15;
               alp_16 = AMR_round (alp2);

               if ((alp*sq2) > (sq*alp_16))
               {
	               sq = sq2;
                  ps = ps2;
                  alp = alp_16;
                  ia = i8;
                  ib = i9;
               }
					i9 += step;

            }while(i9 < L_CODE);

				i8 += step;

         }while(i8 < L_CODE);

      }/* end  gsmefrFlag */

      /*----------------------------------------------------------------  *
       * test and memorise if this combination is better than the last one.*
       *----------------------------------------------------------------*/

      if ((alpk*sq) > (psk*alp))
      {
	      psk = sq;
         alpk = alp;
         codvec[0] = i0;
         codvec[1] = i1;
         codvec[2] = i2;
         codvec[3] = i3;
         codvec[4] = i4;
         codvec[5] = i5;
         codvec[6] = i6;
         codvec[7] = i7;

         if (gsmefrFlag != 0)
         {
         	codvec[8] = ia;
            codvec[9] = ib;
         }
      }
      /*----------------------------------------------------------------*
       * Cyclic permutation of i1,i2,i3,i4,i5,i6,i7,(i8 and i9).          *
       *----------------------------------------------------------------*/
      pos = ipos[1];
      for (j = 1, k = 2; k < nbPulse; j++, k++)
      {
 	     ipos[j] = ipos[k];
      }
   	ipos[(nbPulse-1)] = pos;

	} /* end 1..nbTracks  loop*/

}

#endif



