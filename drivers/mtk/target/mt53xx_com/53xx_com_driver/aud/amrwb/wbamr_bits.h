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
 *   WBAMR_BITS.H
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


#ifndef BITS_H
#define BITS_H

#include "wbamr_typedef.h"
#include "wbamr_cnst.h"
#include "wbamr_private.h"
#include "WBAMR_Lib.h"

void WBAMR_Reset_write_serial ( 
  WBAMR_Static_Mem_Enc_Struct *enc_obj_ptr );

void WBAMR_Reset_read_serial ( 
  WBAMR_Static_Mem_Dec_Struct *dec_obj_ptr );

void WBAMR_Parm_serial ( 
  fract16 value,                     
  fract16 no_of_bits,                
  uint8   ** prms );
     	
fract16 WBAMR_Serial_parm (                 
  fract16 no_of_bits,                   
  uint8   ** prms );

void WBAMR_Pack_Bits (
  fract16 mode,
  uint8   *in_bitstream,
  uint16  *out_packed );

void WBAMR_Unpack_Bits (
  fract16 mode,
  uint16  *in_packed,
  uint8   *out_bitstream );

void WBAMR_Order_Bits (
  fract16 mode,
  uint8   *in_bitstream,
  uint8   *out_bitstream );

void WBAMR_Deorder_Bits (
  fract16 mode,
  uint8   *in_bitstream,
  uint8   *out_bitstream );

void WBAMR_MIME_Pack_Bits ( 
  fract16 *usedMode,
  fract16 mode,
  fract16 *frame_type,
  uint8 *in_bitstream,
  uint16  *out_packed  
);

void WBAMR_MIME_Unpack_Bits ( 
  struct WBAMR_IO_Dec_Struct *dec_io_ptr,
  fract16 *mode,
  fract16 *frame_type,
  uint16 *in_packed,
  uint8  *out_bitstream_ord,
  uint8  *out_bitstream
);

#endif /* BITS_H */
