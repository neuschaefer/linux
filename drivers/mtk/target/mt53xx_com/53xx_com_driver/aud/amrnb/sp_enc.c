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
 *   sp_enc.c
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
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0                
*                                REL-4 Version 4.1.0                
*
*****************************************************************************
*
*      File             : sp_enc.c
*      Purpose          : Pre filtering and encoding of one speech frame.
*
*****************************************************************************
*/
 
/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
 
/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "set_zero.h"
#include "pre_proc.h"
#include "cod_amr.h"
#include "sid_sync.h"

//#ifdef MMS_IO
#include "tab_bitno.h"
//#endif

#include "sp_enc.h"

#define MASK      0x0001

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/
/*---------------------------------------------------------------*
 *    Constants (defined in "cnst.h")                            *
 *---------------------------------------------------------------*
 * L_FRAME     :
 * M           :
 * PRM_SIZE    :
 * AZ_SIZE     :
 * SERIAL_SIZE :
 *---------------------------------------------------------------*/

/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/




/*************************************************************************
 *
 *  FUNCTION:  Int2bin
 *
 *  PURPOSE:  convert integer to binary and write the bits to the array
 *            bitstream[]. The most significant bits are written first.
 *
 *************************************************************************/

static void Int2bin (
    Word16 value,       /* input : value to be converted to binary      */
    Word16 no_of_bits,  /* input : number of bits associated with value */
    Word16 *bitstream   /* output: address where bits are written       */
)
{
	Word32  i, bit;
   Word16 *pt_bitstream;
   pt_bitstream = &bitstream[no_of_bits];      

   for (i = 0; i < no_of_bits; i++)
   {
	   bit = value & MASK;                     
      if (bit == 0)
      {
   	   *--pt_bitstream = BIT_0;           
      }
      else
      {
   	   *--pt_bitstream = BIT_1;            
      }
      value = (value >> 1);
	}
}

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:    Prm2bits
 *
 *  PURPOSE:     converts the encoder parameter vector into a vector of serial
 *               bits.
 *
 *  DESCRIPTION: depending on the mode, different numbers of parameters
 *               (with differing numbers of bits) are processed. Details
 *               are found in bitno.tab
 *
 *************************************************************************/
void Prm2bits (
    enum Mode mode,    /* i : AMR mode                                      */
    Word16 prm[],      /* i : analysis parameters (size <= MAX_PRM_SIZE)    */
    Word16 bits[]      /* o : serial bits         (size <= MAX_SERIAL_SIZE) */
)
{
   Word32 i;
   for (i = 0; i < prmno[mode]; i++)
   {
       Int2bin (prm[i], bitno[mode][i], bits);
       bits += bitno[mode][i];
   }      
	
   return;
}



 
/*************************************************************************
*
*  Function:   Speech_Encode_Frame_reset
*  Purpose:    Resetses state memory
*
**************************************************************************
*/
int Speech_Encode_Frame_reset (AMR_Encode_FrameState *state)
{ 
   Pre_Process_reset(&(state->prestate));     
   cod_amr_reset(&(state->cod_amr_state));

   sid_sync_reset(&(state->sid_state));
   
   return 0;
}


int Speech_Encode_Frame (
                         AMR_Encode_FrameState *Enc, /* i/o : post filter states          */
                         enum Mode mode,               /* i   : speech coder mode           */
                         Word16 *new_speech,           /* i   : speech input                */
                         Word16 *serial,               /* o   : serial bit stream           */
//                         Word16 *bitstream,
                         enum Mode *usedMode           /* o   : used speech coder mode */
                         )
{
   Word32 i;
	AMR_Shared_Buf *Enc_share = Enc->share;
	
	Word16 *ptr16;	
   Word16 *parm = Enc_share->parm;   		/* Analysis parameters.                  */
   Word16 *syn = Enc_share->syn;        /* Buffer for synthesis speech           */
      
   /* initialize the serial output frame to zero */
#if !defined(NO13BIT)
   /* Delete the 3 LSBs (13-bit input) */
	ptr16 = new_speech;
	i = L_FRAME/4;
	do
	{
      *ptr16 = (*ptr16) & 0xfff8;    ptr16++;
      *ptr16 = (*ptr16) & 0xfff8;    ptr16++;
      *ptr16 = (*ptr16) & 0xfff8;    ptr16++;
      *ptr16 = (*ptr16) & 0xfff8;    ptr16++;
	}while(--i != 0);
	
#endif
   
   /* filter + downscaling */
   Pre_Process(&Enc->prestate, new_speech);   

   /* Call the speech encoder */	
   cod_amr(Enc , &(Enc->cod_amr_state), mode, new_speech, parm, usedMode, syn);   
	
   /* Parameters to serial bits */
   Prm2bits(*usedMode, parm, &serial[0]);

   return 0;
}

#endif


