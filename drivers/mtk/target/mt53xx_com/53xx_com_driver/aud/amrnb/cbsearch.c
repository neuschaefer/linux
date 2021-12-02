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
 *   cbsearch.c
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


/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : cbsearch.c
*      Purpose          : Inovative codebook search (find index and gain)
*
*****************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "cbsearch.h"

 
/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "typedef.h"
#include "c2_9pf.h"
#include "c2_11pf.h"
#include "c3_14pf.h"
#include "c4_17pf.h"
#include "c8_31pf.h"
#include "c1035pf.h"
#include "basic_op.h"


/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/


void cbsearch(
			   AMR_Encode_FrameState *Enc,
                            /*     filter h[-L_subfr..-1] must be set to   */
                            /*     zero. Q12                               */
             Word16 T0,     /* i : Pitch lag                               */
             Word16 pitch_sharp, /* i : Last quantized pitch gain, Q14     */
             Word16 gain_pit,    /* i : Pitch gain, Q14                    */
             Word16 **anap, /* o : Signs of the pulses                     */
             enum Mode mode,/* i : coder mode                              */
             Word16 subNr)  /* i : subframe number                         */
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
	Word32 i,sharp;
   Word16 index;
   Word16 temp, pit_sharpTmp;

	Word16 *ht    = Enc->cod_amr_state.h1;
	Word16 *codet = Enc_share->shabuf.stE.code;

   /* For MR74, the pre and post CB pitch sharpening is included in the
    * codebook search routine, while for MR122 is it not.
    */
	if( (mode != MR102) && (mode != MR122) )
	{
	   sharp = (pitch_sharp << 1); /* i : Last quantized pitch gain   */
   	if (T0 < L_CODE)
	   {
   	   Word16 *ptr = (Word16*)ht + T0;
			i = L_CODE - T0;
			do
			{
      	    *ptr = (*ptr + (smulbb(*(ptr - T0), sharp) >>15) );
				 ptr++;
	      }while(--i!=0);
		}

		switch (mode)
		{
			case MR475:
			case MR515:
		      *(*anap)++ = code_2i40_9bits(Enc,
													  subNr,
													  Enc_share->shabuf.stE.xn2,
		      									  ht,
		      									  codet,
		      									  Enc_share->shabuf.stE.y2,
		      									  &index);

   	   	*(*anap)++ = index;    /* sign index */
				break;
			case MR59:
	   	   *(*anap)++ = code_2i40_11bits(Enc,
														Enc_share->shabuf.stE.xn2,
	   	   										ht,
	   	   										codet,
	   	   										Enc_share->shabuf.stE.y2,
	   	   										&index);

   	   	*(*anap)++ = index;    /* sign index */
				break;
			case MR67:
	   	   *(*anap)++ = code_3i40_14bits(Enc,
														Enc_share->shabuf.stE.xn2,
	   	   										ht,
	   	   										codet,
	   	   										Enc_share->shabuf.stE.y2,
	   	   										&index);

		      *(*anap)++ = index;    /* sign index */
				break;
			case MR74:
			case MR795:
		      *(*anap)++ = code_4i40_17bits(Enc,
														Enc_share->shabuf.stE.xn2,
		      										ht,
		      										codet,
		      										Enc_share->shabuf.stE.y2,
		      										&index);

   		   *(*anap)++ = index;    /* sign index */
				break;
			default:
				break;
		}

   	/*-----------------------------------------------------------------*
	   * Compute innovation vector gain.                                 *
	   * Include fixed-gain pitch contribution into code[].              *
   	*-----------------------------------------------------------------*/
		if (T0 < L_CODE)
   	{
      	Word16 *ptr = (Word16*)codet + T0;
			i = L_CODE - T0;
			do
			{
         	 *ptr = (*ptr + (smulbb(*(ptr - T0), sharp)>>15) );
				 ptr++;
			}while(--i!=0);
	   }

	}
	else
	{
     	/*-------------------------------------------------------------*
       * - include pitch contribution into impulse resp. h1[]        *
  	    *-------------------------------------------------------------*/
     	/* pit_sharpTmp = pit_sharp;                     */
      /* if (pit_sharpTmp > 1.0) pit_sharpTmp = 1.0;   */
     	/*-------------------------------------------------------------*
       * - include pitch contribution into impulse resp. h1[]        *
  	    *-------------------------------------------------------------*/
      /* pit_sharpTmp = gain_pit;                      */
  	   /* if (pit_sharpTmp > 1.0) pit_sharpTmp = 1.0;   */
	   if ( mode == MR102 )
   	{   /* MR102 */
   	   pit_sharpTmp = sature(pitch_sharp << 1);
	   }
	   else if ( mode == MR122 )
   	{  /* MR122 */
      	pit_sharpTmp = sature(gain_pit << 1);
	   }


      {
			Word16 *sptr,*sptro;
			sptr  = (Word16*)ht+T0;
			sptro = (Word16*)ht;
			for (i = L_SUBFR-T0; i > 0; i--)
  		   {

	     	   temp = smulbb(*sptro, pit_sharpTmp) >> 15;
   	     	*sptr = (*sptr + temp);
				sptr++;
				sptro++;
	      }
		}

		switch(mode)
		{
			case MR102:
   		   code_8i40_31bits (Enc ,
   		   						Enc_share->shabuf.stE.xn2,
   		   						Enc_share->shabuf.stE.res2,
   		   						ht,
   		   						codet,
   		   						Enc_share->shabuf.stE.y2,
   		   						*anap);

	      	*anap += 7;
				break;
			case MR122:
	   	   code_10i40_35bits (Enc ,
										 *anap);
   	   	*anap += 10;
				break;
			default:
				break;
		}

      /*-------------------------------------------------------*
  	    * - Add the pitch contribution to code[].               *
     	 *-------------------------------------------------------*/
      {
			Word16 *sptr,*sptro;

			sptr  = (Word16*)codet+T0;
			sptro = (Word16*)codet;

			for (i = L_SUBFR-T0; i > 0; i--)
  		   {
        		*sptr = (*sptr + (smulbb(*sptro, pit_sharpTmp) >> 15));
				sptr++;
				sptro++;
	      }

		}

	}

   return;

}

#endif

