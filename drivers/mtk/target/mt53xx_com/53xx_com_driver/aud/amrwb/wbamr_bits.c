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
 *   WBAMR_BITS.C
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
#include "wbamr_rom.h"
#include "wbamr_bits.h"
#include "wbamr_acelp.h"
#include "wbamr_sections.h"
#include "WBAMR_Lib.h"



WBAMR_CODE_SECTION void WBAMR_Reset_write_serial ( 
  WBAMR_Static_Mem_Enc_Struct *enc_obj_ptr 
) 
{
  enc_obj_ptr->sid_update_counter = 3;
  enc_obj_ptr->sid_handover_debt = 0;
  enc_obj_ptr->prev_ft = TX_SPEECH;
}


WBAMR_CODE_SECTION void WBAMR_Reset_read_serial (
  WBAMR_Static_Mem_Dec_Struct *dec_obj_ptr 
) 
{
  dec_obj_ptr->prev_ft   = RX_SPEECH_GOOD;
  dec_obj_ptr->prev_mode = 0;
}

/*
 * WBAMR_Parm_serial -> convert parameters to serial stream
 */

WBAMR_CODE_SECTION void WBAMR_Parm_serial ( 
  fract16 value,                         /* input : parameter value */
  fract16 no_of_bits,                    /* input : number of bits  */
  uint8 ** prms
 ) 
{
  int_native i;
  uint8 bit;

  *prms += no_of_bits;                   

  for ( i = 0; i < no_of_bits; i++ ) 
  {
    bit = ( uint8 ) ( value & 0x0001 );     /* get lsb */
        *--( *prms )  = bit;
        value = Shr_fr1x16 ( value, 1 );             
  }
  *prms += no_of_bits;                   
  
  return;
}


/*
 * WBAMR_Serial_parm -> convert serial stream to parameters
 */

WBAMR_CODE_SECTION fract16 WBAMR_Serial_parm ( /* Return the parameter    */
  fract16 no_of_bits,                          /* input : number of bits  */
  uint8 ** prms
 ) 
{   
  int_native i;	
  fract16 value=0;
  fract16 bit;

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
   uint8 *ptr;

   ptr = *prms;
   i = no_of_bits;
   do
   {
      value <<= 1;
      bit = *ptr++ ;                
      value |= bit;
   } while(--i != 0);
   *prms = ptr;
#else
   for ( i = 0; i < no_of_bits; i++ ) 
   {
      value = Shl_fr1x16 ( value, 1 );
      bit = *( (*prms)++ );                
      value |= bit;
   }
#endif
   return ( value );
}


WBAMR_CODE_SECTION void WBAMR_MIME_Pack_Bits ( 
  fract16 *usedMode,
  fract16 mode,
  fract16 *frame_type,
  uint8 *in_bitstream,
  uint16  *out_packed  
) 
{
   int i;
   uint8 temp;
   uint8 *pack_ptr;
   fract16 cmode = *usedMode;
   
   temp = 0;
   pack_ptr = (uint8 *)out_packed;

	/* change mode index in case of SID frame */
	if (*usedMode == MRDTX)
	{
		*usedMode = 9;

		if (*frame_type == TX_SID_FIRST)
		{
			for (i = 0; i < NBBITS_SID; i++)	in_bitstream[i] = 0;
		}
	}

	/* we cannot handle unspecified frame types (modes 10 - 13) */
	/* -> force NO_DATA frame */
	if (*usedMode < 0 || *usedMode > 15 || (*usedMode > 9 && *usedMode < 14))
	{
		*usedMode = 15;
	}

	/* mark empty frames between SID updates as NO_DATA frames */
	if (*usedMode == 9 && *frame_type == TX_NO_DATA)
	{
		*usedMode = 15;
	}
   
   /* insert table of contents (ToC) byte at the beginning of the frame */
   *pack_ptr++ = AWB_toc_byte[*usedMode];
   
   temp = 0;   
   if(*usedMode <= 9)
   {
	   /* pack speech bits */
	   for (i = 0; i < nb_of_bits[cmode] ; i++)
	   {
		  if (in_bitstream[i] != 0)
		  {
			 temp++;
		  }      
		  if ( (i&7) != 7)
		  {
			 temp <<= 1;
		  }
		  else
		  {
			 *pack_ptr++ = temp;
			 temp = 0;
		  }
	   }  
   }

	/* insert SID type indication and speech mode in case of SID frame */
	if (*usedMode == 9)
	{
		if (*frame_type == TX_SID_UPDATE)
		{
			temp++;
		}
		temp <<= 4;
		
		temp += mode & 0x000F;
	}

	/* insert unused bits (zeros) at the tail of the last byte */
	if (AWB_unused_size[*usedMode])
	{
		temp <<= (AWB_unused_size[*usedMode] - 1);
	}
   *pack_ptr = temp;
}
void WBAMR_MIME_Unpack_Bits ( 
  struct WBAMR_IO_Dec_Struct *dec_io_ptr,
  fract16 *mode,
  fract16 *frame_type,
  uint16 *in_packed,
  uint8  *out_bitstream_ord,
  uint8  *out_bitstream
) 
{
   int i;
   uint8 *pack_ptr, temp;
   uint8 toc, qbit;
   uint8 *out_ptr;
   fract16 cmode;

   pack_ptr = (uint8 *)in_packed;

   toc = *pack_ptr++;

   /* extract q and mode from ToC */
   qbit  = (toc >> 2) & 0x01;
   *mode = (toc >> 3) & 0x0F;

  // Eddy
  dec_io_ptr->mode = *mode;


   if(*mode >= 0 && *mode < 9) {
      out_ptr = out_bitstream_ord;
   } else {
      out_ptr = out_bitstream;
   }

   temp = *pack_ptr++;
   //pack_ptr++;

   cmode = *mode;
   if(*mode == 9)
	   cmode = MRDTX;
   /* unpack and unsort speech or SID bits */
   if(cmode <= MRDTX) {
	   for (i = 0; i < nb_of_bits[cmode]; i++)
	   {
		   if (temp & 0x80)
			 *out_ptr++ = 1;
		   else
			 *out_ptr++ = 0;
		   
		   if ((i & 7) != 7)
		   {
			   temp <<= 1;
		   }
		   else
		   {
			   temp = *pack_ptr++;
		   }
	   }
	   if(cmode < 9) {
		   WBAMR_Deorder_Bits(cmode, out_bitstream_ord, out_bitstream);
	   }
   }

   /* set frame type */
   switch (*mode)
   {
   case MODE_7k:
   case MODE_9k:
   case MODE_12k:
   case MODE_14k:
   case MODE_16k:
   case MODE_18k:
   case MODE_20k:
   case MODE_23k:
   case MODE_24k:
	   if (qbit)
         *frame_type = RX_SPEECH_GOOD;
	   else
         *frame_type = RX_SPEECH_BAD;
	   break;
   case 9:
	   if (qbit)
	   {
		   if (temp & 0x80)
            *frame_type = RX_SID_UPDATE;
		   else
            *frame_type = RX_SID_FIRST;
	   }
	   else
	   {
		   *frame_type = RX_SID_BAD;
	   }
	   /* set mode index */
	   *mode = (temp >> 3) & 0x0F;
	   break;
   case 14:		/* SPEECH_LOST */
	   *frame_type = RX_SPEECH_LOST;
	   *mode = dec_io_ptr->prev_mode;
	   break;
   case 15:		/* NO_DATA */
	   *frame_type = RX_NO_DATA;
	   *mode = dec_io_ptr->prev_mode;
	   break;
   default:		/* replace frame with unused mode index by NO_DATA frame */
	   *frame_type = RX_NO_DATA;
	   *mode = dec_io_ptr->prev_mode;
	   break;
   }

   dec_io_ptr->prev_mode = *mode;
}
/*
 * Function Name:  WBAMR_Pack_Bits () 
 *
 *    *in_bitstream : IN,     [0..nb_of_bits[mode]]
 *    *out_packed   : OUT,    [0..nb_of_bits[mode]>>4]
 *
 *    returns       : void
 */
 
WBAMR_CODE_SECTION void WBAMR_Pack_Bits ( 
  fract16 mode,
  uint8 *in_bitstream,
  uint16  *out_packed  
) 
{
  int_native i, j;
  fract32 word;
  uint8 *bits;
  int_native leftoverbits;

  /* pack into 16bit words */
  for ( i=0; i< ( nb_of_bits[mode]>>4 ); i++ ) 
  {
    /* pack one 16-bit word */
    word = 0;
    bits = &in_bitstream[16*i];
    for ( j=15; j>=0; j-- ) 
    {
      word = Shl_fr1x32 (  word, 1  ) ;
      word |= (fract32) bits[j];
    }
    *out_packed++ = (uint16) word;
  }
  /* compute how many more bits are left */
  leftoverbits = nb_of_bits[mode] & 0xF;
  /* perform packing */  
  if ( leftoverbits > 0 )
  {
    bits = ( in_bitstream + nb_of_bits[mode] ) -1;
    *out_packed = *bits--;

    for ( i = 1; i < leftoverbits; i++ ) 
    {
      *out_packed <<= 1;
      *out_packed |= *bits--;
    }
    /* right justify bits */
  }
}


/*
 * Function Name:  WBAMR_Unpack_Bits () 
 *
 *    *in_packed     : IN,     [0..nb_of_bits[mode]>>4]
 *    *out_bitstream : OUT,    [0..nb_of_bits[mode]]
 *
 *    returns        : void
 */
 
WBAMR_CODE_SECTION void WBAMR_Unpack_Bits ( 
  fract16 mode,
  uint16 *in_packed,
  uint8  *out_bitstream
) 
{
  fract16 word=0;
  int_native i, j;
  int_native leftoverbits;

  /* unpack 16bit words */
  for ( i=0; i< ( nb_of_bits[mode]>>4 ); i++ ) 
  {
    for ( j=0, word= (fract16) *in_packed++; j<16; j++ )
    {
      /* extract lsb first... */
      *out_bitstream++ = (uint8) word & 0x0001;
      word = Shr_fr1x16 ( word, 1 );
    }
  }
  word= (fract16) *in_packed;
  /* compute how many more bits are left */
  leftoverbits = nb_of_bits[mode] & 0xF;
  /* perform unpacking */
  if ( leftoverbits > 0 ) 
  {
    for  ( i = 0; i < leftoverbits; i++ ) 
    { 
      *out_bitstream++ = (uint8) word & 0x0001;
       word = word >>1;
    }
  }
}


/*
 * Function Name:  WBAMR_Order_Bits () 
 *
 *    in_bitstream  : IN,     [0..nb_of_bits[mode]]
 *    out_bitstream : OUT,    [0..nb_of_bits[mode]]
 *
 *    returns       : void
 *
 */

WBAMR_CODE_SECTION void WBAMR_Order_Bits ( 
  fract16 mode,
  uint8 *in_bitstream,
  uint8 *out_bitstream
) 
{
  int_native  i;
  const uint16 *WBAMR_allo = SubjectiveReordering[mode];
       
  for ( i=0; i<nb_of_bits[mode]; i++ ) 
  {
    *out_bitstream++ = in_bitstream[WBAMR_allo[i]];
  }
}


/*
 * Function Name:  WBAMR_Deorder_Bits () 
 *
 *    in_bitstream  : IN,     [0..nb_of_bits[mode]]
 *    out_bitstream : OUT,    [0..nb_of_bits[mode]]
 *
 *    returns       : void
 */

WBAMR_CODE_SECTION void WBAMR_Deorder_Bits ( 
  fract16 mode,
  uint8 *in_bitstream,
  uint8 *out_bitstream 
) 
{
  int_native i;
  const uint16 *WBAMR_allo = SubjectiveReordering[mode];
   
  for ( i=0; i<nb_of_bits[mode]; i++ ) 
  {
    out_bitstream[WBAMR_allo[i]] = *in_bitstream++;
  }
}

#endif

