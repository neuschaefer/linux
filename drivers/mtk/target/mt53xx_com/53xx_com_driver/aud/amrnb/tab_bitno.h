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
 *   tab_bitno.h
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

/*
********************************************************************************
*      
*      Purpose          : Tables for bit2prm and prm2bit
********************************************************************************
*/

#define BIT_0      0
#define BIT_1      1

#define PRMNO_MR475 17
#define PRMNO_MR515 19
#define PRMNO_MR59  19
#define PRMNO_MR67  19
#define PRMNO_MR74  19
#define PRMNO_MR795 23
#define PRMNO_MR102 39
#define PRMNO_MR122 57
#define PRMNO_MRDTX 5

#define PRMNOFSF_MR475 7
#define PRMNOFSF_MR515 7
#define PRMNOFSF_MR59  7
#define PRMNOFSF_MR67  7
#define PRMNOFSF_MR74  7
#define PRMNOFSF_MR795 8
#define PRMNOFSF_MR102 12
#define PRMNOFSF_MR122 18

extern const Word16 prmno[N_MODES];
extern const Word16 prmnofsf[N_MODES - 1];
extern const Word16 bitno_MR475[PRMNO_MR475];
extern const Word16 bitno_MR515[PRMNO_MR515];
extern const Word16 bitno_MR59[PRMNO_MR59];
extern const Word16 bitno_MR67[PRMNO_MR67];
extern const Word16 bitno_MR74[PRMNO_MR74];
extern const Word16 bitno_MR795[PRMNO_MR795];
extern const Word16 bitno_MR102[PRMNO_MR102];
extern const Word16 bitno_MR122[PRMNO_MR122];
extern const Word16 bitno_MRDTX[PRMNO_MRDTX];
extern const Word16 * const bitno[N_MODES];



#if 0
#ifdef MMS_IO

/* table of contents byte for each mode index */
extern const UWord8 toc_byte[16];

/* number of speech bits for all modes */
extern const Word16 unpacked_size[16];

/* size of packed frame for each mode */
extern const Word16 packed_size[16];

/* number of unused speech bits in packed format for each mode */
extern const Word16 unused_size[16];

/* sorting tables for all modes */

extern const Word16 sort_475[95];

extern const Word16 sort_515[103];

extern const Word16 sort_59[118];

extern const Word16 sort_67[134];
extern const Word16 sort_74[148];

extern const Word16 sort_795[159];

extern const Word16 sort_102[204];

extern const Word16 sort_122[244];

extern const Word16 sort_SID[35];

/* pointer table for bit sorting tables */
extern const Word16 *sort_ptr[16];
#endif


#endif















