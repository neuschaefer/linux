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
/**
 *   @file dsp_pat_table.h
 *		
 *   @author
 *
 *   Revision: 5368_0003
 */

#include "x_common.h" 

#define PAT_REV "5368_0003"

#define UINST_TST_SIZE 148
extern const UINT8 UINST_TST_TABLE[UINST_TST_SIZE];
#define TRAP_TST_SIZE 436
extern const UINT8 TRAP_TST_TABLE[TRAP_TST_SIZE];
#define BS64_TST_SIZE 2920
extern const UINT8 BS64_TST_TABLE[BS64_TST_SIZE];
#define BUTFLY_TST_SIZE 1820
extern const UINT8 BUTFLY_TST_TABLE[BUTFLY_TST_SIZE];
#define CORDIC_TST_LACTM_SIZE 692
extern const UINT8 CORDIC_TST_LACTM_TABLE[CORDIC_TST_LACTM_SIZE];
#define CORDIC_TST_PCRDC_SIZE 716
extern const UINT8 CORDIC_TST_PCRDC_TABLE[CORDIC_TST_PCRDC_SIZE];
#define CORDIC_VECTOR_SIZE 4408
extern const UINT8 CORDIC_VECTOR_TABLE[CORDIC_VECTOR_SIZE];
#define DRAM_TST_SIZE 4620
extern const UINT8 DRAM_TST_TABLE[DRAM_TST_SIZE];
#define FADD_TST_SIZE 920
extern const UINT8 FADD_TST_TABLE[FADD_TST_SIZE];
#define GETBITS_TST_NORMAL_SIZE 1116
extern const UINT8 GETBITS_TST_NORMAL_TABLE[GETBITS_TST_NORMAL_SIZE];
#define GETBITS_TST_14BIT_SIZE 740
extern const UINT8 GETBITS_TST_14BIT_TABLE[GETBITS_TST_14BIT_SIZE];
#define GETBITS_TST_VORBIS_SIZE 1080
extern const UINT8 GETBITS_TST_VORBIS_TABLE[GETBITS_TST_VORBIS_SIZE];
#define IIR64_TST_SIZE 672
extern const UINT8 IIR64_TST_TABLE[IIR64_TST_SIZE];
#define INST_TST_SIZE 7540
extern const UINT8 INST_TST_TABLE[INST_TST_SIZE];
#define INDEX_TST_SIZE 976
extern const UINT8 INDEX_TST_TABLE[INDEX_TST_SIZE];
#define MEM_TST_SIZE 1012
extern const UINT8 MEM_TST_TABLE[MEM_TST_SIZE];
#define PROM_TST_ROM_SIZE 4088
extern const UINT8 PROM_TST_ROM_TABLE[PROM_TST_ROM_SIZE];
#define PRAM_TST_ROM_SIZE 4088
extern const UINT8 PRAM_TST_ROM_TABLE[PRAM_TST_ROM_SIZE];
#define PRAM_TST_CRI_SIZE 496
extern const UINT8 PRAM_TST_CRI_TABLE[PRAM_TST_CRI_SIZE];
#define PRAM_TST_DEC2_SIZE 2032
extern const UINT8 PRAM_TST_DEC2_TABLE[PRAM_TST_DEC2_SIZE];
#define PRAM_17BIT_TST_ROM_SIZE 4092
extern const UINT8 PRAM_17BIT_TST_ROM_TABLE[PRAM_17BIT_TST_ROM_SIZE];
#define PRAM_17BIT_TST_CRI_SIZE 496
extern const UINT8 PRAM_17BIT_TST_CRI_TABLE[PRAM_17BIT_TST_CRI_SIZE];
#define PRAM_17BIT_TST_DEC2_SIZE 876
extern const UINT8 PRAM_17BIT_TST_DEC2_TABLE[PRAM_17BIT_TST_DEC2_SIZE];
#define PSRDMA_TST_SIZE 672
extern const UINT8 PSRDMA_TST_TABLE[PSRDMA_TST_SIZE];
#define PUTB_TST_SIZE 3788
extern const UINT8 PUTB_TST_TABLE[PUTB_TST_SIZE];
#define RAND_TST_SIZE 280
extern const UINT8 RAND_TST_TABLE[RAND_TST_SIZE];
#define RORC_TST_SIZE 88
extern const UINT8 RORC_TST_TABLE[RORC_TST_SIZE];
#define SQURT_TST_SIZE 1500
extern const UINT8 SQURT_TST_TABLE[SQURT_TST_SIZE];
#define ROM_CS_TST_SIZE 200
extern const UINT8 ROM_CS_TST_TABLE[ROM_CS_TST_SIZE];
#define MIDI_TST_SIZE 444
extern const UINT8 MIDI_TST_TABLE[MIDI_TST_SIZE];
#define SBRCRC_TST_SIZE 10204
extern const UINT8 SBRCRC_TST_TABLE[SBRCRC_TST_SIZE];
#define CRC11_TST_SIZE 12936
extern const UINT8 CRC11_TST_TABLE[CRC11_TST_SIZE];
#define MADS64_TST_SIZE 524
extern const UINT8 MADS64_TST_TABLE[MADS64_TST_SIZE];
