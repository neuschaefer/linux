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
 *   coder.c
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
 * 02 21 2011 richie.hsieh
 * [WCPSP00000575] [IPComm] SWIP protection
 * .
 *
 * 12 20 2010 scholar.chang
 * [WCPSP00000557] [SWIP]Compile warning removed.
 * Finish the amr SWIP compile warning removal.
 *
 * 09 24 2010 scholar.chang
 * [WCPSP00000519] Remove AMR codec SWIP assert function
 * .
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
*      File             : coder.c
*      Purpose          : Speech encoder main program.
*
*****************************************************************************
*
*    Usage : coder speech_file  bitstream_file
*
*    Format for speech_file:
*      Speech is read from a binary file of 16 bits data.
*
*    Format for bitstream_file:
*        1 word (2-byte) for the TX frame type
*          (see frame.h for possible values)
*      244 words (2-byte) containing 244 bits.
*          Bit 0 = 0x0000 and Bit 1 = 0x0001
*        1 word (2-byte) for the mode indication
*          (see mode.h for possible values)
*        4 words for future use, currently written as zero
*
*****************************************************************************
*/

/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/

#include "typedef.h"

#include "sp_enc.h"
#include "pre_proc.h"
#include "sid_sync.h"

#include "e_homing.h"
#include "sp_dec.h"

#include "tab_bitno.h"
#include "d_homing.h"
#include "audip_exp.h"

#ifdef MMS_IO
#define AMR_MAGIC_NUMBER "#!AMR\n"
#define MAX_PACKED_SIZE (MAX_SERIAL_SIZE / 8 + 2)
#endif


//#define CAL_MIPS


/* frame size in serial bitstream file (frame type + serial stream + flags) */
#define SERIAL_FRAMESIZE (1+MAX_SERIAL_SIZE+5)


#if !__TARGET_FEATURE_DSPMUL
armdsp_flagdata_union armdsp_flagdata_AMR;
#endif


#define BIT_0      0
#define BIT_1      1

/*
*****************************************************************************
*                         CONST TABLE
*****************************************************************************
*/
//#ifdef MMS_IO

/* table of contents byte for each mode index */
const UWord8 toc_byte[16] = {0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C,
                              0x44, 0x4C, 0x54, 0x5C, 0x64, 0x6C, 0x74, 0x7C};

/* number of speech bits for all modes */
const Word16 unpacked_size[16] = {95, 103, 118, 134, 148, 159, 204, 244,
                                   35,   0,   0,   0,   0,   0,   0,   0};

/* size of packed frame for each mode */
const Word16 packed_size[16] = {13, 14, 16, 18, 20, 21, 27, 32,
                                  6,  0,  0,  0,  0,  0,  0,  1};

/* number of unused speech bits in packed format for each mode */
const Word16 unused_size[16] = {1, 1, 2, 2, 4, 1, 4, 4, 1, 0, 0, 0, 0, 0, 0, 0};

/* sorting tables for all modes */

const Word16 sort_475[95] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
   10, 11, 12, 13, 14, 15, 23, 24, 25, 26,
   27, 28, 48, 49, 61, 62, 82, 83, 47, 46,
   45, 44, 81, 80, 79, 78, 17, 18, 20, 22,
   77, 76, 75, 74, 29, 30, 43, 42, 41, 40,
   38, 39, 16, 19, 21, 50, 51, 59, 60, 63,
   64, 72, 73, 84, 85, 93, 94, 32, 33, 35,
   36, 53, 54, 56, 57, 66, 67, 69, 70, 87,
   88, 90, 91, 34, 55, 68, 89, 37, 58, 71,
   92, 31, 52, 65, 86
};

const Word16 sort_515[103] = {
    7,  6,  5,   4,   3,   2,  1,  0, 15, 14,
   13, 12, 11,  10,   9,   8, 23, 24, 25, 26,
   27, 46, 65,  84,  45,  44, 43, 64, 63, 62,
   83, 82, 81, 102, 101, 100, 42, 61, 80, 99,
   28, 47, 66,  85,  18,  41, 60, 79, 98, 29,
   48, 67, 17,  20,  22,  40, 59, 78, 97, 21,
   30, 49, 68,  86,  19,  16, 87, 39, 38, 58,
   57, 77, 35,  54,  73,  92, 76, 96, 95, 36,
   55, 74, 93,  32,  51,  33, 52, 70, 71, 89,
   90, 31, 50,  69,  88,  37, 56, 75, 94, 34,
   53, 72, 91
};

const Word16 sort_59[118] = {
    0,   1,   4,   5,   3,   6,   7,   2,  13,  15,
    8,   9,  11,  12,  14,  10,  16,  28,  74,  29,
   75,  27,  73,  26,  72,  30,  76,  51,  97,  50,
   71,  96, 117,  31,  77,  52,  98,  49,  70,  95,
  116,  53,  99,  32,  78,  33,  79,  48,  69,  94,
  115,  47,  68,  93, 114,  46,  67,  92, 113,  19,
   21,  23,  22,  18,  17,  20,  24, 111,  43,  89,
  110,  64,  65,  44,  90,  25,  45,  66,  91, 112,
   54, 100,  40,  61,  86, 107,  39,  60,  85, 106,
   36,  57,  82, 103,  35,  56,  81, 102,  34,  55,
   80, 101,  42,  63,  88, 109,  41,  62,  87, 108,
   38,  59,  84, 105,  37,  58,  83, 104
};

const Word16 sort_67[134] = {
    0,   1,   4,   3,   5,   6,  13,   7,   2,   8,
    9,  11,  15,  12,  14,  10,  28,  82,  29,  83,
   27,  81,  26,  80,  30,  84,  16,  55, 109,  56,
  110,  31,  85,  57, 111,  48,  73, 102, 127,  32,
   86,  51,  76, 105, 130,  52,  77, 106, 131,  58,
  112,  33,  87,  19,  23,  53,  78, 107, 132,  21,
   22,  18,  17,  20,  24,  25,  50,  75, 104, 129,
   47,  72, 101, 126,  54,  79, 108, 133,  46,  71,
  100, 125, 128, 103,  74,  49,  45,  70,  99, 124,
   42,  67,  96, 121,  39,  64,  93, 118,  38,  63,
   92, 117,  35,  60,  89, 114,  34,  59,  88, 113,
   44,  69,  98, 123,  43,  68,  97, 122,  41,  66,
   95, 120,  40,  65,  94, 119,  37,  62,  91, 116,
   36,  61,  90, 115
};

const Word16 sort_74[148] = {
    0,   1,	  2,   3,   4,   5,   6,   7,   8,   9,
   10,  11,	 12,  13,  14,  15,  16,  26,  87,  27,
   88,  28,	 89,  29,  90,  30,  91,  51,  80, 112,
  141,  52,  81, 113, 142,  54,  83, 115, 144,  55,
   84, 116, 145,  58, 119,  59, 120,  21,  22,  23,
   17,  18,	 19,  31,  60,  92, 121,  56,  85, 117,
  146,  20,	 24,  25,  50,  79, 111, 140,  57,  86,
  118, 147,	 49,  78, 110, 139,  48,  77,  53,  82,
  114, 143, 109, 138,  47,  76, 108, 137,  32,  33,
   61,  62,	 93,  94, 122, 123,  41,  42,  43,  44,
   45,  46,	 70,  71,  72,  73,  74,  75, 102, 103,
  104, 105, 106, 107, 131, 132, 133, 134, 135, 136,
   34,  63,	 95, 124,  35,  64,  96, 125,  36,  65,
   97, 126,	 37,  66,  98, 127,	 38,  67,  99, 128,
   39,  68, 100, 129,  40,  69, 101, 130
};

const Word16 sort_795[159] = {
    8,   7,   6,   5,   4,   3,   2,  14,  16,   9,
   10,  12,  13,  15,  11,  17,  20,  22,  24,  23,
   19,  18,  21,  56,  88, 122, 154,  57,  89, 123,
  155,  58,  90, 124, 156,  52,  84, 118, 150,  53,
   85, 119, 151,  27,  93,  28,  94,  29,  95,  30,
   96,  31,  97,  61, 127,  62, 128,  63, 129,  59,
   91, 125, 157,  32,  98,  64, 130,   1,   0,  25,
   26,  33,  99,  34, 100,  65, 131,  66, 132,  54,
   86, 120, 152,  60,  92, 126, 158,  55,  87, 121,
  153, 117, 116, 115,  46,  78, 112, 144,  43,  75,
  109, 141,  40,  72, 106, 138,  36,  68, 102, 134,
  114, 149, 148, 147, 146,  83,  82,  81,  80,  51,
   50,  49,  48,  47,  45,  44,  42,  39,  35,  79,
   77,  76,  74,  71,  67, 113, 111, 110, 108, 105,
  101, 145, 143, 142, 140, 137, 133,  41,  73, 107,
  139,  37,  69, 103, 135,  38,  70, 104, 136
};

const Word16 sort_102[204] = {
    7,   6,   5,   4,   3,   2,   1,   0,  16,  15,
   14,  13,  12,  11,  10,   9,   8,  26,  27,  28,
   29,  30,  31, 115, 116, 117, 118, 119, 120,  72,
   73, 161, 162,  65,  68,  69, 108, 111, 112, 154,
  157, 158, 197, 200, 201,  32,  33, 121, 122,  74,
   75, 163, 164,  66, 109, 155, 198,  19,  23,  21,
   22,  18,  17,  20,  24,  25,  37,  36,  35,  34,
   80,  79,  78,  77, 126, 125, 124, 123, 169, 168,
  167, 166,  70,  67,  71, 113, 110, 114, 159, 156,
  160, 202, 199, 203,  76, 165,  81,  82,  92,  91,
   93,  83,  95,  85,  84,  94, 101, 102,  96, 104,
   86, 103,  87,  97, 127, 128, 138, 137, 139, 129,
  141, 131, 130, 140, 147, 148, 142, 150, 132, 149,
  133, 143, 170, 171, 181, 180, 182, 172, 184, 174,
  173, 183, 190, 191, 185, 193, 175, 192, 176, 186,
   38,  39,  49,  48,  50,  40,  52,  42,  41,  51,
   58,  59,  53,  61,  43,  60,  44,  54, 194, 179,
  189, 196, 177, 195, 178, 187, 188, 151, 136, 146,
  153, 134, 152, 135, 144, 145, 105,  90, 100, 107,
   88, 106,  89,  98,  99,  62,  47,  57,  64,  45,
   63,  46,  55,  56
};

const Word16 sort_122[244] = {
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
   10,  11,  12,  13,  14,  23,  15,  16,  17,  18,
   19,  20,  21,  22,  24,  25,  26,  27,  28,  38,
  141,  39, 142,  40, 143,  41, 144,  42, 145,  43,
  146,  44, 147,  45, 148,  46, 149,  47,  97, 150,
  200,  48,  98, 151, 201,  49,  99, 152, 202,  86,
  136, 189, 239,  87, 137, 190, 240,  88, 138, 191,
  241,  91, 194,  92, 195,  93, 196,  94, 197,  95,
  198,  29,  30,  31,  32,  33,  34,  35,  50, 100,
  153, 203,  89, 139, 192, 242,  51, 101, 154, 204,
   55, 105, 158, 208,  90, 140, 193, 243,  59, 109,
  162, 212,  63, 113, 166, 216,  67, 117, 170, 220,
   36,  37,  54,  53,  52,  58,  57,  56,  62,  61,
   60,  66,  65,  64,  70,  69,  68, 104, 103, 102,
  108, 107, 106, 112, 111, 110, 116, 115, 114, 120,
  119, 118, 157, 156, 155, 161, 160, 159, 165, 164,
  163, 169, 168, 167, 173, 172, 171, 207, 206, 205,
  211, 210, 209, 215, 214, 213, 219, 218, 217, 223,
  222, 221,  73,  72,  71,  76,  75,  74,  79,  78,
   77,  82,  81,  80,  85,  84,  83, 123, 122, 121,
  126, 125, 124, 129, 128, 127, 132, 131, 130, 135,
  134, 133, 176, 175, 174, 179, 178, 177, 182, 181,
  180, 185, 184, 183, 188, 187, 186, 226, 225, 224,
  229, 228, 227, 232, 231, 230, 235, 234, 233, 238,
  237, 236,  96, 199
};

const Word16 sort_SID[35] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
   10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
   20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
   30, 31, 32, 33, 34
};

/* pointer table for bit sorting tables */
const Word16 *const sort_ptr[16] = {sort_475, sort_515, sort_59, sort_67, sort_74, sort_795, sort_102, sort_122,
                               sort_SID,     NULL,   NULL,    NULL,     NULL,    NULL,     NULL,      NULL};


//#endif



/*
********************************************************************************
*                             MAIN PROGRAM
********************************************************************************
*/



/*
**************************************************************************
*
*  Function    : UnpackBits
*  Purpose     : Unpack and re-arrange bits from file storage format to the
*                format required by speech decoder.
*
**************************************************************************
*/
int UnpackBits (
   Word8  q,              /* i : Q-bit (i.e. BFI)        */
	Word16 ft,             /* i : frame type (i.e. mode)  */
   UWord8 packed_bits[],  /* i : sorted & packed bits    */
	Word32 *mode,       /* o : mode information        */
   Word16 bits[]          /* o : serial bits             */
)
{
	Word32 i, sid_type;
	UWord8 *pack_ptr, temp;

    if (ft > 15 || ft < 0)
        ft = 0;

	pack_ptr = (UWord8*)packed_bits;

	/* real NO_DATA frame or unspecified frame type */
	if (ft == 15 || (ft > 8 && ft < 15))
	{
		*mode = -1;
		return RX_NO_DATA;
	}

	temp = *pack_ptr;
	pack_ptr++;

	for (i = 0; i < unpacked_size[ft]; i++)
	{
		if (temp & 0x80)
			bits[sort_ptr[ft][i]] = BIT_1;
		else
			bits[sort_ptr[ft][i]] = BIT_0;

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
		if (temp & 0x80)
			sid_type = 1;
		else
			sid_type = 0;

		*mode = ((temp >> 4) & 0x07);
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
		*mode = ft;

		if (q)
			return RX_SPEECH_GOOD;
		else
			return RX_SPEECH_BAD;
	}
}



void AMREnc_GetBufferSize(
   UWord32 *int_buf_size,
   UWord32 *tmp_buf_size,
   UWord32 *bs_buf_size,
   UWord32 *pcm_buf_size
)
{
   *int_buf_size = (aligned_sizeof(AMR_Encode_FrameState));
   *tmp_buf_size = (aligned_sizeof(AMR_Shared_Buf));
   *pcm_buf_size = (((L_FRAME*2)+3)&(~3));          // 160*2 = 320 bytes
   *bs_buf_size =  ((MAX_PACKED_SIZE+3)&(~3));//(((SERIAL_FRAMESIZE*2)+3)&(~3));  // 250*2 = 500 bytes
}

void AMRDec_GetBufferSize(
   UWord32 *int_buf_size,
   UWord32 *tmp_buf_size,
   UWord32 *bs_buf_size,
   UWord32 *pcm_buf_size
)
{
	*int_buf_size = (aligned_sizeof(AMR_Decode_FrameState));
   *tmp_buf_size = (aligned_sizeof(AMR_Shared_Buf));
   *pcm_buf_size = (((L_FRAME*2)+3)&(~3));                   // 160*2 = 320 bytes
   *bs_buf_size =  ((MAX_PACKED_SIZE+3)&(~3));//(((SERIAL_FRAMESIZE*2)+3)&(~3));  // 250*2 = 500 bytes

}



AMR_ENC_HANDLE *AMREnc_Init(
   void *int_buffer,
   AMR_BitRate BitRate,
   int dtx
)
{
	void *data;
	AMR_ENC_HANDLE *pdata;
	AMR_Encode_FrameState *enc_state = (AMR_Encode_FrameState *)int_buffer;

//	AMR_ASSERT(int_buffer != NULL);
  if (!(int_buffer != NULL)) {
    return NULL;
  }
//  AMR_ASSERT(is_aligned(int_buffer));
  if (!(is_aligned(int_buffer))) {
    return NULL;
  }

	pdata = &(enc_state->amr_enc_handle);

	/* Initialization of encoder */
   enc_state->dtx = dtx;
	enc_state->cod_amr_state.dtx = dtx;
	enc_state->sid_state.sid_update_rate = 8;
   Speech_Encode_Frame_reset(enc_state);
	data = (void *)enc_state;

	pdata->amr_enc_data = data;
   pdata->BitRate      = BitRate;
	pdata->dtx_enable   = dtx;

	return(pdata);

}


AMR_DEC_HANDLE *AMRDec_Init(
   void *int_buffer
)
{

	AMR_DEC_HANDLE *pdata;
	void *data;
	AMR_Decode_FrameState *dec_state = (AMR_Decode_FrameState *) int_buffer;

//	AMR_ASSERT(int_buffer != NULL);
  if (!(int_buffer != NULL)) {
    return NULL;
  }
//  AMR_ASSERT(is_aligned(int_buffer));
  if (!(is_aligned(int_buffer))) {
    return NULL;
  }

	pdata = &(dec_state->amr_dec_handle);

	/* Initialization of decoder */
	dec_state->reset_flag	   = 0;
	dec_state->reset_flag_old  = 1;
	Speech_Decode_Frame_reset(dec_state);

	data = (void *)dec_state;
	pdata->amr_dec_data  = data;


	return(pdata);
}







int AMR_Encode_tmp(
   AMR_ENC_HANDLE *amr_enc_handle,
   void *tmp_buffer,
   short *pcm_buffer,
   short *bs_buffer,
	AMR_BitRate bitrate,
	enum Mode *used_bitrate
)
{
	Word32 reset_flag;
	Word32 i;
	enum TXFrameType tx_type;

   AMR_Encode_FrameState *amrenc_handle = (AMR_Encode_FrameState *)amr_enc_handle->amr_enc_data;

	amrenc_handle->share = (AMR_Shared_Buf *) tmp_buffer;


	/* zero flags and parameter bits */
	for (i = 0; i < SERIAL_FRAMESIZE; i++)
     	bs_buffer[i] = 0;


	/* check for homing frame */
	reset_flag = encoder_homing_frame_test(pcm_buffer);

	/* encode speech */
//	Speech_Encode_Frame(amrenc_handle, mode, new_speech, bitstream, used_mode);
	Speech_Encode_Frame(amrenc_handle, (enum Mode)bitrate, pcm_buffer, bs_buffer, (enum Mode *)used_bitrate);


   /* include frame type and mode information in serial bitstream */
   sid_sync (&(amrenc_handle->sid_state), (enum Mode)*used_bitrate, &tx_type);

	/* perform homing if homing frame was detected at encoder input */
	if (reset_flag != 0)
	{
	  Speech_Encode_Frame_reset(amrenc_handle);
     sid_sync_reset(&(amrenc_handle->sid_state));
	}

	return tx_type;

}

/*************************************************************************
*
*  FUNCTION:    PackBits
*
*  PURPOSE:     Sorts speech bits according decreasing subjective importance
*               and packs into octets according to AMR file storage format
*               as specified in RFC 3267 (Sections 5.1 and 5.3).
*
*  DESCRIPTION: Depending on the mode, different numbers of bits are
*               processed. Details can be found in specification mentioned
*               above and in file "bitno.tab".
*
*************************************************************************/
Word16 PackBits(
                enum Mode used_mode,       /* i : actual AMR mode             */
                int mode,            /* i : requested AMR (speech) mode */
                int fr_type,  /* i : frame type                  */
                Word16 bits[],             /* i : serial bits                 */
                UWord8 packed_bits[]       /* o : sorted&packed bits          */
                )
{
   Word32 i;
   UWord8 temp;
   UWord8 *pack_ptr;

   temp = 0;
   pack_ptr = (UWord8*)packed_bits;

   /* file storage format can handle only speech frames, AMR SID frames and NO_DATA frames */
   /* -> force NO_DATA frame */
   if (used_mode > 15 || (used_mode > 8 && used_mode < 15)) //if (used_mode < 0 || used_mode > 15 || (used_mode > 8 && used_mode < 15))
   {
      used_mode = (enum Mode)15;
   }

   /* mark empty frames between SID updates as NO_DATA frames */
   if (used_mode == MRDTX && fr_type == TX_NO_DATA)
   {
      used_mode = (enum Mode)15;
   }

   /* insert table of contents (ToC) byte at the beginning of the frame */
   *pack_ptr = toc_byte[used_mode];
   pack_ptr++;

   /* note that NO_DATA frames (used_mode==15) do not need further processing */
   if (used_mode == 15)
   {
      return 1;
   }

   temp = 0;
   /* sort and pack speech bits */
   for (i = 0; i < unpacked_size[used_mode] ; i++)
   {
      if (bits[sort_ptr[used_mode][i]] == BIT_1)
      {
         temp++;
      }
      if ( (i&7) != 7)
      {
         temp <<= 1;
      }
      else
      {
         *pack_ptr = temp;
         pack_ptr++;
         temp = 0;
      }
   }

   /* insert SID type indication and speech mode in case of SID frame */
   if (used_mode == MRDTX)
   {
      if (fr_type == TX_SID_UPDATE)
      {
         temp++;
      }
      temp <<= 3;
      temp += ((mode & 0x4) >> 2) | (mode & 0x2) | ((mode & 0x1) << 2);
      temp <<= 1;
   }
   /* insert unused bits (zeros) at the tail of the last byte */
   temp <<= (unused_size[used_mode] - 1);
   *pack_ptr = temp;

   return packed_size[used_mode];
}

int AMR_Encode(
   AMR_ENC_HANDLE *amr_enc_handle,
   void *tmp_buffer,
   short *pcm_buffer,
   unsigned char *bs_buffer,
	AMR_BitRate bitrate
)
{
	int tx_type;
	int packed_size;
	short tmp_bs[SERIAL_FRAMESIZE];
	enum Mode used_mode;

	AUDIP_COMMON()
    AUDIP_DRVSET()

	tx_type = AMR_Encode_tmp(amr_enc_handle,tmp_buffer,pcm_buffer,tmp_bs,bitrate,&used_mode);

	packed_size = PackBits(used_mode, bitrate, tx_type, &tmp_bs[0], bs_buffer);

	return packed_size;

}




void AMR_Decode_tmp(
   AMR_DEC_HANDLE *amr_dec_handle,
   void *tmp_buffer,
   short *pcm_buffer,
   short *bs_buffer,
	int rx_type,
	int bitrate
)
{
   Word32 i;
   AMR_Decode_FrameState *amrdec_handle = (AMR_Decode_FrameState *)amr_dec_handle->amr_dec_data;
	amrdec_handle->share = (AMR_Shared_Buf *) tmp_buffer;

	if (rx_type == RX_NO_DATA)
	{
		bitrate = (int)amrdec_handle->prev_mode;
	}
	else
	{
		amrdec_handle->prev_mode = (enum Mode)bitrate;
	}

	/* if homed: check if this frame is another homing frame */
	if (amrdec_handle->reset_flag_old == 1)
	{
		/* only check until end of first subframe */
      amrdec_handle->reset_flag = decoder_homing_frame_test_first(&bs_buffer[0],(enum Mode)bitrate);
   }
   /* produce encoder homing frame if homed & input=decoder homing frame */

   if ((amrdec_handle->reset_flag != 0) && (amrdec_handle->reset_flag_old != 0))
   {
		for (i = 0; i < L_FRAME; i++)
      {
      	pcm_buffer[i] = EHF_MASK;
      }
	}
   else
   {
   	/* decode frame */
      Speech_Decode_Frame(amrdec_handle, (enum Mode)bitrate, &bs_buffer[0],(enum RXFrameType)rx_type, pcm_buffer);
   }

	/* if not homed: check whether current frame is a homing frame */
   if (amrdec_handle->reset_flag_old == 0)
   {
   	/* check whole frame */
      amrdec_handle->reset_flag = decoder_homing_frame_test(&bs_buffer[0], (enum Mode)bitrate);
   }
   /* reset decoder if current frame is a homing frame */
   if (amrdec_handle->reset_flag != 0)
   {
	   Speech_Decode_Frame_reset(amrdec_handle);
	}
   amrdec_handle->reset_flag_old = amrdec_handle->reset_flag;
}


void AMR_Decode(
   AMR_DEC_HANDLE *amr_dec_handle,
   void *tmp_buffer,
   short *pcm_buffer,
   unsigned char *bs_buffer,
	unsigned char quality_bit,
	unsigned char frame_type
)
{
	int rx_type;
	int bitrate;
	short tmp_bs[SERIAL_FRAMESIZE];

    AUDIP_COMMON()
    AUDIP_DRVSET()

   rx_type = UnpackBits(quality_bit, (short)frame_type, bs_buffer, &bitrate, tmp_bs);
	AMR_Decode_tmp(amr_dec_handle,tmp_buffer,pcm_buffer,tmp_bs, rx_type, bitrate);

}

#endif

