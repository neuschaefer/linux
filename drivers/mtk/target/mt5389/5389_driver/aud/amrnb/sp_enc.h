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
 *   sp_enc.h
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
*      File             : sp_enc.h
*      Purpose          : Encoding of one speech frame.
*
*****************************************************************************
*/
#ifndef sp_enc_h
#define sp_enc_h "$Id $"
 
/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/
#include "typedef.h"
#include "pre_proc.h"
#include "cod_amr.h"

/*
*****************************************************************************
*                         DEFINITION OF DATA TYPES
*****************************************************************************
*/

 
/*
*****************************************************************************
*                         ENCLARATION OF PROTOTYPES
*****************************************************************************
*/
 
int Speech_Encode_Frame_init (AMR_Encode_FrameState **st, Flag dtx);
/* initialize one instance of the speech encoder
   Stores pointer to filter status struct in *st. This pointer has to
   be passed to Speech_Encode_Frame in each call.
   returns 0 on success
 */
 
int Speech_Encode_Frame_reset (AMR_Encode_FrameState *st);
/* reset speech encoder (i.e. set state memory to zero)
   returns 0 on success
 */
 
void Speech_Encode_Frame_exit (AMR_Encode_FrameState **st);
/* de-initialize speech encoder (i.e. free status struct)
   stores NULL in *s
 */

#if 0 
int Speech_Encode_Frame_First (
    Speech_Encode_FrameState *st, /* i/o : post filter states     */
    Word16 *new_speech);          /* i   : speech input           */
#endif

int Speech_Encode_Frame (
    AMR_Encode_FrameState *st, /* i/o : encoder states         */
    enum Mode mode,               /* i   : speech coder mode      */
    Word16 *new_speech,           /* i   : input speech           */
//    Word16 *serial,               /* o   : serial bit stream      */
    Word16 *bitstream,
    enum Mode *usedMode           /* o   : used speech coder mode */
);
/*    return 0 on success
 */

#ifdef MMS_IO

#if 0
Word16 PackBits(
    enum Mode used_mode,       /* i : actual AMR mode             */
    enum Mode mode,            /* i : requested AMR (speech) mode */
    enum TXFrameType fr_type,  /* i : frame type                  */
    Word16 bits[],             /* i : serial bits                 */
    UWord8 packed_bits[]       /* o : sorted&packed bits          */  
);
#endif

#endif
 
#endif
