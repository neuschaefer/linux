/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/

/** @file io_papi_fe_chantbl.h
 *  Channel tables for PAPI ioctl interface of MT539X tuner driver.
 */

#ifndef IO_PAPI_FE_CHANTBL_H
#define IO_PAPI_FE_CHANTBL_H

#define PAPI_IO_FE_CHANTBL_NUM 1

typedef struct _IO_PAPI_FE_CHANTBL_T {
    Nat32   chan;           // channel number
    Nat32   central_freq;   // central frequency (kHz)
    Nat32   initial_freq;   // initial frequency (kHz)
    Nat32   final_freq;     // final frequency (kHz)
} IO_PAPI_FE_CHANTBL_T, *PIO_PAPI_FE_CHANTBL_T;

typedef struct _IO_PAPI_FE_CHANTBL_PAIR_T {
    tmFe_ChanTable_t        id;
    PIO_PAPI_FE_CHANTBL_T   table;
    Nat32                   siz;
} IO_PAPI_FE_CHANTBL_PAIR_T, *PIO_PAPI_FE_CHANTBL_PAIR_T;

#define CHANTBL_USCABLE_SIZ    (68)
IO_PAPI_FE_CHANTBL_T fe_TableUsCable[CHANTBL_USCABLE_SIZ] = {
    {2   , 57000  , 54000  , 60000},
    {3   , 63000  , 60000  , 66000},
    {4   , 69000  , 66000  , 72000},
    {5   , 79000  , 76000  , 82000},
    {6   , 85000  , 82000  , 88000},
    {7   , 177000 , 174000 , 180000},
    {8   , 183000 , 180000 , 186000},
    {9   , 189000 , 186000 , 192000},
    {10  , 195000 , 192000 , 198000},
    {11  , 201000 , 198000 , 204000},
    {12  , 207000 , 204000 , 210000},
    {13  , 213000 , 210000 , 216000},
    {14  , 473000 , 470000 , 476000},
    {15  , 479000 , 476000 , 482000},
    {16  , 485000 , 482000 , 488000},
    {17  , 491000 , 488000 , 494000},
    {18  , 497000 , 494000 , 500000},
    {19  , 503000 , 500000 , 506000},
    {20  , 509000 , 506000 , 512000},
    {21  , 515000 , 512000 , 518000},
    {22  , 521000 , 518000 , 524000},
    {23  , 527000 , 524000 , 530000},
    {24  , 533000 , 530000 , 536000},
    {25  , 539000 , 536000 , 542000},
    {26  , 545000 , 542000 , 548000},
    {27  , 551000 , 548000 , 554000},
    {28  , 557000 , 554000 , 560000},
    {29  , 563000 , 560000 , 566000},
    {30  , 569000 , 566000 , 572000},
    {31  , 575000 , 572000 , 578000},
    {32  , 581000 , 578000 , 584000},
    {33  , 587000 , 584000 , 590000},
    {34  , 593000 , 590000 , 596000},
    {35  , 599000 , 596000 , 602000},
    {36  , 605000 , 602000 , 608000},
    {37  , 611000 , 608000 , 614000},
    {38  , 617000 , 614000 , 620000},
    {39  , 623000 , 620000 , 626000},
    {40  , 629000 , 626000 , 632000},
    {41  , 635000 , 632000 , 638000},
    {42  , 641000 , 638000 , 644000},
    {43  , 647000 , 644000 , 650000},
    {44  , 653000 , 650000 , 656000},
    {45  , 659000 , 656000 , 662000},
    {46  , 665000 , 662000 , 668000},
    {47  , 671000 , 668000 , 674000},
    {48  , 677000 , 674000 , 680000},
    {49  , 683000 , 680000 , 686000},
    {50  , 689000 , 686000 , 692000},
    {51  , 695000 , 692000 , 698000},
    {52  , 701000 , 698000 , 704000},
    {53  , 707000 , 704000 , 710000},
    {54  , 713000 , 710000 , 716000},
    {55  , 719000 , 716000 , 722000},
    {56  , 725000 , 722000 , 728000},
    {57  , 731000 , 728000 , 734000},
    {58  , 737000 , 734000 , 740000},
    {59  , 743000 , 740000 , 746000},
    {60  , 749000 , 746000 , 752000},
    {61  , 755000 , 752000 , 758000},
    {62  , 761000 , 758000 , 764000},
    {63  , 767000 , 764000 , 770000},
    {64  , 773000 , 770000 , 776000},
    {65  , 779000 , 776000 , 782000},
    {66  , 785000 , 782000 , 788000},
    {67  , 791000 , 788000 , 794000},
    {68  , 797000 , 794000 , 800000},
    {69  , 803000 , 800000 , 806000}
};

IO_PAPI_FE_CHANTBL_PAIR_T io_papi_fe_chantbl_pair[PAPI_IO_FE_CHANTBL_NUM] = {
    // id,              table,            siz
    {tmFe_TableUsCable, &fe_TableUsCable, CHANTBL_USCABLE_SIZ}
};

#endif//IO_PAPI_FE_CHANTBL_H

