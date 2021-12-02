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
 *   sp_dec.c
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
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : sp_dec.c
*      Purpose          : Decoding and post filtering of one speech frame.
*
*****************************************************************************
*/

/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "sp_dec.h"
 
/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "set_zero.h"
#include "dec_amr.h"
#include "pstfilt.h"
#include "bits2prm.h"
#include "post_pro.h"

#ifdef MMS_IO
#include "tab_bitno.h"
#endif

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
*  Function:   Speech_Decode_Frame_reset
*  Purpose:    Resetses state memory
*
**************************************************************************
*/
int Speech_Decode_Frame_reset (AMR_Decode_FrameState *state)
{
   Decoder_amr_reset(&(state->decoder_amrState), (enum Mode)0);
   Post_Filter_reset(&(state->post_state));
   Post_Process_reset(&(state->postHP_state));

   state->prev_mode = (enum Mode)0;

   return 0;
}


int Speech_Decode_Frame (
    AMR_Decode_FrameState *Dec, /* io: post filter states                */
    enum Mode mode,               /* i : AMR mode                          */
    Word16 *serial,               /* i : serial bit stream                 */
    enum RXFrameType frame_type,    /* i : Frame type                        */
    Word16 *synth                 /* o : synthesis speech (postfiltered    */
                                  /*     output)                           */
)
{
	AMR_Shared_Buf *Dec_share = Dec->share;
	Word16 *sptr;

#if 0
   Word16 parm[MAX_PRM_SIZE + 1];  /* Synthesis parameters                */
   Word16 Az_dec[AZ_SIZE+4];         /* Decoded Az for post-filter          */
#endif

   Word16 *parm = Dec_share->parm;  /* Synthesis parameters       58         */
   Word16 *Az_dec = Dec_share->Az_dec;         /* Decoded Az for post-filter          */


   /* in 4 subframes                      */

#if !defined(NO13BIT)
   Word16 i;
#endif

   /* Serial to parameters   */
	if( (frame_type == RX_SID_BAD) || (frame_type == RX_SID_UPDATE) )
	{
   	/* Override mode to MRDTX */
		Bits2prm (MRDTX, serial, parm);
	}
	else
	{
		Bits2prm (mode, serial, parm);
	}

   /* Synthesis */
   Decoder_amr(Dec , &(Dec->decoder_amrState), mode, parm, frame_type, synth, Az_dec);

   Post_Filter(Dec , &(Dec->post_state), mode, synth, Az_dec);   /* Post-filter */

   /* post HP filter, and 15->16 bits */
   Post_Process(&(Dec->postHP_state), synth);

#if !defined(NO13BIT)
   /* Truncate to 13 bits */
	sptr = synth;
	i = L_FRAME/4;
	do
   {
      *sptr = *sptr & 0xfff8;    sptr++;
      *sptr = *sptr & 0xfff8;    sptr++;
      *sptr = *sptr & 0xfff8;    sptr++;
      *sptr = *sptr & 0xfff8;    sptr++;
   }while(--i != 0);

/*
   for (i = 0; i < L_FRAME; i++)
   {
      synth[i] = synth[i] & 0xfff8;
   }
*/
#endif


   return 0;
}

#ifdef MMS_IO


#if 0
/*
**************************************************************************
*
*  Function    : UnpackBits
*  Purpose     : Unpack and re-arrange bits from file storage format to the
*                format required by speech decoder.
*
**************************************************************************
*/
enum RXFrameType UnpackBits (
    Word8  q,              /* i : Q-bit (i.e. BFI)        */
	Word16 ft,             /* i : frame type (i.e. mode)  */
    UWord8 packed_bits[],  /* i : sorted & packed bits    */
	enum Mode *mode,       /* o : mode information        */
    Word16 bits[]          /* o : serial bits             */
)
{
	Word32 i, sid_type;
	UWord8 *pack_ptr, temp;

	pack_ptr = (UWord8*)packed_bits;

	/* real NO_DATA frame or unspecified frame type */
	if (ft == 15 || (ft > 8 && ft < 15))
	{
		*mode = (enum Mode)-1;
		return RX_NO_DATA;
	}

	temp = *pack_ptr;
	pack_ptr++;

	for (i = 0; i < unpacked_size[ft]; i++)
	{
		if (temp & 0x80)	bits[sort_ptr[ft][i]] = BIT_1;
		else				bits[sort_ptr[ft][i]] = BIT_0;

      if ( (i&7) != 7)
		{
			temp <<= 1;
		}
		else
		{
			temp = *pack_ptr;
			pack_ptr++;
		}
	}

	/* SID frame */
	if (ft == MRDTX)
	{
		if (temp & 0x80)	sid_type = 1;
		else				sid_type = 0;

		*mode = (enum Mode)((temp >> 4) & 0x07);
		*mode = ((*mode & 0x4) >> 2) | (*mode & 0x2) | ((*mode & 0x1) << 2);

		if (q)
		{
			if (sid_type)	return  RX_SID_UPDATE;
			else			return	RX_SID_FIRST;
		}
		else
		{
			return	RX_SID_BAD;
		}
	}
	/* speech frame */
	else
	{
		*mode = (enum Mode)ft;

		if (q)	return RX_SPEECH_GOOD;
		else	return RX_SPEECH_BAD;
	}
}

#endif

#endif

#endif

