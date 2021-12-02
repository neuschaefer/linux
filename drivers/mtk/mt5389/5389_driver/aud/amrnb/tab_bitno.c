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
 *   tab_bitno.c
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
*      Purpose          : Tables for bit2prm and prm2bit
********************************************************************************
*/

#include "typedef.h"



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

/* number of parameters per modes (values must be <= MAX_PRM_SIZE!) */
const Word16 prmno[N_MODES] = {
  PRMNO_MR475,
  PRMNO_MR515,
  PRMNO_MR59,
  PRMNO_MR67,
  PRMNO_MR74,
  PRMNO_MR795,
  PRMNO_MR102,
  PRMNO_MR122,
  PRMNO_MRDTX
};


/* number of parameters to first subframe */
#define PRMNOFSF_MR475 7
#define PRMNOFSF_MR515 7
#define PRMNOFSF_MR59  7
#define PRMNOFSF_MR67  7
#define PRMNOFSF_MR74  7
#define PRMNOFSF_MR795 8
#define PRMNOFSF_MR102 12
#define PRMNOFSF_MR122 18

/* number of parameters to first subframe per modes */
const Word16 prmnofsf[N_MODES - 1] = {
  PRMNOFSF_MR475,
  PRMNOFSF_MR515,
  PRMNOFSF_MR59,
  PRMNOFSF_MR67,
  PRMNOFSF_MR74,
  PRMNOFSF_MR795,
  PRMNOFSF_MR102,
  PRMNOFSF_MR122
};

/* parameter sizes (# of bits), one table per mode */
const Word16 bitno_MR475[PRMNO_MR475] = {
   8, 8, 7,                                 /* LSP VQ          */
   8, 7, 2, 8,                              /* first subframe  */
   4, 7, 2,                                 /* second subframe */
   4, 7, 2, 8,                              /* third subframe  */
   4, 7, 2,                                 /* fourth subframe */
};

const Word16 bitno_MR515[PRMNO_MR515] = {
   8, 8, 7,                                 /* LSP VQ          */
   8, 7, 2, 6,                              /* first subframe  */
   4, 7, 2, 6,                              /* second subframe */
   4, 7, 2, 6,                              /* third subframe  */
   4, 7, 2, 6,                              /* fourth subframe */
};

const Word16 bitno_MR59[PRMNO_MR59] = {
   8, 9, 9,                                 /* LSP VQ          */
   8, 9, 2, 6,                              /* first subframe  */
   4, 9, 2, 6,                              /* second subframe */
   8, 9, 2, 6,                              /* third subframe  */
   4, 9, 2, 6,                              /* fourth subframe */
};

const Word16 bitno_MR67[PRMNO_MR67] = {
   8, 9, 9,                                 /* LSP VQ          */
   8, 11, 3, 7,                             /* first subframe  */
   4, 11, 3, 7,                             /* second subframe */
   8, 11, 3, 7,                             /* third subframe  */
   4, 11, 3, 7,                             /* fourth subframe */
};

const Word16 bitno_MR74[PRMNO_MR74] = {
   8, 9, 9,                                 /* LSP VQ          */
   8, 13, 4, 7,                             /* first subframe  */
   5, 13, 4, 7,                             /* second subframe */
   8, 13, 4, 7,                             /* third subframe  */
   5, 13, 4, 7,                             /* fourth subframe */
};

const Word16 bitno_MR795[PRMNO_MR795] = {
   9, 9, 9,                                 /* LSP VQ          */
   8, 13, 4, 4, 5,                          /* first subframe  */
   6, 13, 4, 4, 5,                          /* second subframe */
   8, 13, 4, 4, 5,                          /* third subframe  */
   6, 13, 4, 4, 5,                          /* fourth subframe */
};

const Word16 bitno_MR102[PRMNO_MR102] = {
   8, 9, 9,                                 /* LSP VQ          */
   8, 1, 1, 1, 1, 10, 10, 7, 7,             /* first subframe  */
   5, 1, 1, 1, 1, 10, 10, 7, 7,             /* second subframe */
   8, 1, 1, 1, 1, 10, 10, 7, 7,             /* third subframe  */
   5, 1, 1, 1, 1, 10, 10, 7, 7,             /* fourth subframe */
};

const Word16 bitno_MR122[PRMNO_MR122] = {
   7, 8, 9, 8, 6,                           /* LSP VQ          */
   9, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 5,   /* first subframe  */
   6, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 5,   /* second subframe */
   9, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 5,   /* third subframe  */
   6, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 5    /* fourth subframe */
};

const Word16 bitno_MRDTX[PRMNO_MRDTX] = {
  3,
  8, 9, 9,
  6
};

/* overall table with all parameter sizes for all modes */
const Word16 * const bitno[N_MODES] = {
   bitno_MR475,
   bitno_MR515,
   bitno_MR59,
   bitno_MR67,
   bitno_MR74,
   bitno_MR795,
   bitno_MR102,
   bitno_MR122,
   bitno_MRDTX
};

#endif




