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
 *   sp_dec.h
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
*      File             : sp_dec.h
*      Purpose          : Decoding and post filtering of one speech frame.
*
*****************************************************************************
*/
#ifndef sp_dec_h
#define sp_dec_h "$Id $"
 
/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/
#include "typedef.h"
#include "dec_amr.h"
#include "pstfilt.h"
#include "post_pro.h"

 
/*
*****************************************************************************
*                         DECLARATION OF PROTOTYPES
*****************************************************************************
*/
#if 0
int Speech_Decode_Frame_init (AMR_Decode_FrameState **st);
/* initialize one instance of the speech decoder
   Stores pointer to filter status struct in *st. This pointer has to
   be passed to Speech_Decode_Frame in each call.
   returns 0 on success
 */

void Speech_Decode_Frame_exit (AMR_Decode_FrameState **st);
/* de-initialize speech decoder (i.e. free status struct)
   stores NULL in *s
 */
#endif
 
//int AMR_Dec_Init (AMR_Decode_FrameState *st);
//int AMR_Dec_Init (UWord8 *int_buf,UWord8 *sha_buf);
int Speech_Decode_Frame_reset (AMR_Decode_FrameState *state);
/* reset speech decoder (i.e. set state memory to zero)
   returns 0 on success
 */
 

 
int Speech_Decode_Frame (
    AMR_Decode_FrameState *st, /* io: post filter states                */
    enum Mode mode,               /* i : AMR mode                          */
    Word16 *serial,               /* i : serial bit stream                 */
    enum RXFrameType frame_type,  /* i : Frame type                        */
    Word16 *synth                 /* o : synthesis speech (postfiltered    */
                                  /*     output)                           */
);
/*    return 0 on success
 */

#ifdef MMS_IO

#if 0
enum RXFrameType UnpackBits (
    Word8  q,              /* i : Q-bit (i.e. BFI)        */
	Word16 ft,             /* i : frame type (i.e. mode)  */
    UWord8 packed_bits[],  /* i : sorted & packed bits    */
	enum Mode *mode,       /* o : mode information        */
    Word16 bits[]          /* o : serial bits             */
);
#endif

#endif
 
#endif
