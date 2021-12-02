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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: x_mid.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file x_mid.h
 *  x_mid.h Memory intrusion detection module
 */

#ifndef X_MID_H
#define X_MID_H

#include "x_typedef.h"
#include "x_hal_5381.h"
#include "hw_vdoin.h"

//=====================================================================
// Constant definitions

#define MID_MAX_RW_REGIONS              3   // Regions allowing one agent to write
#define MID_MAX_RO_REGIONS              2   // Read-only regions
#define MID_MAX_ML_REGIONS              2   // Multiple agent regions
#define MID_SINGLE_REGIONS              (MID_MAX_RW_REGIONS + MID_MAX_RO_REGIONS)
#define MID_MAX_REGIONS                 (MID_MAX_RW_REGIONS + MID_MAX_RO_REGIONS + MID_MAX_ML_REGIONS)

#define MID_ADDR_ALIGN_SHIFT            2
#define MID_ADDR_ALIGNMENT              (1 << MID_ADDR_ALIGN_SHIFT)
#define GROUP2_FIRST_AGENT_ID           16
#define GROUP3_FIRST_AGENT_ID           23

//===========================================================================
// Macros for register read/write

#define DRAM_READ8(offset)			IO_READ8(DRAM_BASE, (offset))
#define DRAM_READ16(offset)			IO_READ16(DRAM_BASE, (offset))
#define DRAM_READ32(offset)			IO_READ32(DRAM_BASE, (offset))

#define DRAM_WRITE8(offset, value)	IO_WRITE8(DRAM_BASE, (offset), (value))
#define DRAM_WRITE16(offset, value)	IO_WRITE16(DRAM_BASE, (offset), (value))
#define DRAM_WRITE32(offset, value)	IO_WRITE32(DRAM_BASE, (offset), (value))

#define DRAM_REG8(offset)			IO_REG8(DRAM_BASE, (offset))
#define DRAM_REG16(offset)			IO_REG16(DRAM_BASE, (offset))
#define DRAM_REG32(offset)			IO_REG32(DRAM_BASE, (offset))

//=====================================================================
// Register definitions
#define INTRU_ADDR_SHIFT        (8U)
#define MSK32BIT Fld(32, 0, AC_FULLDW) //31:0

#define CPU1PROT (IO_VIRT+0x07048)
    #define CPUAGID1 Fld(5, 24, AC_MSKB3) //28:24
    #define MCKMOD Fld(1, 19, AC_MSKB2) //19
    #define CPEN Fld(1, 18, AC_MSKB2) //18
    #define MEMCPEN1 Fld(1, 17, AC_MSKB2) //17
    #define MEMCPEN2 Fld(1, 16, AC_MSKB2) //16
    #define PTAUDMODE Fld(1, 15, AC_MSKB1) //15
    #define PATAUDINTLV Fld(1, 14, AC_MSKB1) //14
    #define INTCLR Fld(1, 13, AC_MSKB1) //13
    #define PATAUDINIT Fld(5, 8, AC_MSKB1) //12:8
    #define PATAUDPSTWR Fld(1, 7, AC_MSKB0) //7
    #define PATAUDINC Fld(5, 0, AC_MSKB0) //4:0

#define PROT0UP (IO_VIRT+0x0705c)
    #define P0MDE Fld(1, 31, AC_MSKB3) //31
    #define PROT0 Fld(1, 30, AC_MSKB3) //30
    #define PROT0UPA Fld(27, 2, AC_MSKDW) //28:2
    #define P0CPU Fld(1, 1, AC_MSKB0) //1
    #define P0CPUSEL Fld(1, 0, AC_MSKB0) //0
#define PROT0LW (IO_VIRT+0x07060)
    #define PROT0AG1 Fld(3, 29, AC_MSKB3) //31:29
    #define PROT0LWA Fld(27, 2, AC_MSKDW) //28:2
    #define PROT0AG Fld(2, 0, AC_MSKB0) //1:0
#define PROT1UP (IO_VIRT+0x07064)
    #define P1MDE Fld(1, 31, AC_MSKB3) //31
    #define PROT1 Fld(1, 30, AC_MSKB3) //30
    #define PROT1UPA Fld(27, 2, AC_MSKDW) //28:2
    #define P1CPU Fld(1, 1, AC_MSKB0) //1
    #define P1CPUSEL Fld(1, 0, AC_MSKB0) //0
#define PROT1LW (IO_VIRT+0x07068)
    #define PROT1AG1 Fld(3, 29, AC_MSKB3) //31:29
    #define PROT1LWA Fld(27, 2, AC_MSKDW) //28:2
    #define PROT1AG Fld(2, 0, AC_MSKB0) //1:0
#define PROT2UP (IO_VIRT+0x0706c)
    #define P2MDE Fld(1, 31, AC_MSKB3) //31
    #define PROT2 Fld(1, 30, AC_MSKB3) //30
    #define PROT2UPA Fld(27, 2, AC_MSKDW) //28:2
    #define P2CPU Fld(1, 1, AC_MSKB0) //1
    #define P2CPUSEL Fld(1, 0, AC_MSKB0) //0
#define PROT2LW (IO_VIRT+0x07070)
    #define PROT2AG1 Fld(3, 29, AC_MSKB3) //31:29
    #define PROT2LWA Fld(27, 2, AC_MSKDW) //28:2
    #define PROT2AG Fld(2, 0, AC_MSKB0) //1:0
#define PROT3UP (IO_VIRT+0x07074)
    #define PROT3WR Fld(1, 31, AC_MSKB3) //31
    #define PROT3 Fld(1, 30, AC_MSKB3) //30
    #define PROT3UPA Fld(27, 2, AC_MSKDW) //28:2
#define PROT3LW (IO_VIRT+0x07078)
    #define PROT3AG1 Fld(3, 29, AC_MSKB3) //31:29
    #define PROT3LWA Fld(27, 2, AC_MSKDW) //28:2
    #define PROT3AG Fld(2, 0, AC_MSKB0) //1:0

#define EMRS_BM (IO_VIRT+0x07080)
    #define BMGP1AG1 Fld(1, 30, AC_MSKB3) //30
    #define EMRS Fld(6, 24, AC_MSKB3) //29:24
    #define MRSBK Fld(2, 22, AC_MSKB2) //23:22
    #define MRS Fld(6, 16, AC_MSKB2) //21:16
    #define DMBMFRZ Fld(1, 15, AC_MSKB1) //15
    #define BMGP3 Fld(1, 14, AC_MSKB1) //14
    #define BMGP2 Fld(1, 13, AC_MSKB1) //13
    #define BMGP1 Fld(1, 12, AC_MSKB1) //12
    #define BMGP1AG Fld(4, 8, AC_MSKB1) //11:8
    #define PACNTEN Fld(1, 7, AC_MSKB0) //7
    #define BMGP2AG Fld(3, 4, AC_MSKB0) //6:4
    #define REQCNTEN Fld(1, 3, AC_MSKB0) //3
    #define BMGP3AG Fld(3, 0, AC_MSKB0) //2:0
#define BMCYC (IO_VIRT+0x07084)
    #define BMCYCM Fld(32, 0, AC_FULLDW) //31:0

#define PROT4UP (IO_VIRT+0x070c4)
    #define PROT4WR Fld(1, 31, AC_MSKB3) //31
    #define PROT4 Fld(1, 30, AC_MSKB3) //30
    #define PROT4UPA Fld(27, 2, AC_MSKDW) //28:2
#define PROT4LW (IO_VIRT+0x070c8)
    #define PROT4AG1 Fld(3, 29, AC_MSKB3) //31:29
    #define PROT4LWA Fld(27, 2, AC_MSKDW) //28:2
    #define PROT4AG Fld(2, 0, AC_MSKB0) //1:0

#define PROT5UP (IO_VIRT+0x07140)
    #define PROT5WR Fld(1, 31, AC_MSKB3) //31
    #define PROT5 Fld(1, 30, AC_MSKB3) //30
    #define PROT5UPA Fld(27, 2, AC_MSKDW) //28:2
#define PROT5LW (IO_VIRT+0x07144)
    #define PROT5LWA Fld(27, 2, AC_MSKDW) //28:2
#define PROT6UP (IO_VIRT+0x07148)
    #define PROT6WR Fld(1, 31, AC_MSKB3) //31
    #define PROT6 Fld(1, 30, AC_MSKB3) //30
    #define PROT6UPA Fld(27, 2, AC_MSKDW) //28:2
#define PROT6LW (IO_VIRT+0x0714c)
    #define PROT6LWA Fld(27, 2, AC_MSKDW) //28:2
#define PROT5EN (IO_VIRT+0x07150)
    #define PROT5EN_GP3 Fld(8, 24, AC_FULLB3) //31:24
    #define PROT5EN_GP2 Fld(8, 16, AC_FULLB2) //23:16
    #define PROT5EN_GP1 Fld(16, 0, AC_FULLW10) //15:0
#define PROT6EN (IO_VIRT+0x07154)
    #define PROT6EN_GP3 Fld(8, 24, AC_FULLB3) //31:24
    #define PROT6EN_GP2 Fld(8, 16, AC_FULLB2) //23:16
    #define PROT6EN_GP1 Fld(16, 0, AC_FULLW10) //15:0

#define INTRUDE (IO_VIRT+0x07360)
    #define INTRUDE4_SID Fld(3, 16, AC_MSKB2) //18:16
    #define INTRUDE3_SID Fld(3, 12, AC_MSKB1) //14:12
    #define INTRUDE2_SID Fld(3, 8, AC_MSKB1) //10:8
    #define INTRUDE1_SID Fld(3, 4, AC_MSKB0) //6:4
    #define INTRUDE40_SID Fld(3, 0, AC_MSKB0) //2:0
#define INTRU8 (IO_VIRT+0x07364)
    #define INRG5 Fld(1, 31, AC_MSKB3) //31
    #define INTRU_RG5_GP1AG Fld(5, 16, AC_MSKB2) //20:16
    #define INRG6 Fld(1, 15, AC_MSKB1) //15
    #define INTRU_RG6_GP1AG Fld(5, 0, AC_MSKB0) //4:0
#define INTRU9 (IO_VIRT+0x07368)
    #define RG5ADR Fld(28, 0, AC_MSKDW) //27:0
#define INTRU10 (IO_VIRT+0x0736c)
    #define RG6ADR Fld(28, 0, AC_MSKDW) //27:0

#define BMLEN1 (IO_VIRT+0x073c8)
    #define BMLENGP1_CNT Fld(32, 0, AC_FULLDW) //31:0
#define BMLEN2 (IO_VIRT+0x073cc)
    #define BMLENGP2_CNT Fld(32, 0, AC_FULLDW) //31:0
#define BMLEN3 (IO_VIRT+0x073d0)
    #define BMLENGP3_CNT Fld(32, 0, AC_FULLDW) //31:0
#define INTRU6 (IO_VIRT+0x073d4)
    #define RG4AG Fld(5, 27, AC_MSKB3) //31:27
    #define RG4ADR Fld(27, 0, AC_MSKDW) //26:0
#define INTRU7 (IO_VIRT+0x073d8)
    #define AUTOK_SM Fld(3, 28, AC_MSKB3) //30:28
    #define AUTOK_VAL Fld(4, 24, AC_MSKB3) //27:24
    #define CMP_ERR Fld(1, 20, AC_MSKB2) //20
    #define INTRUDE4 Fld(1, 14, AC_MSKB1) //14
    #define INTRUDE3 Fld(1, 13, AC_MSKB1) //13
    #define INTRUDE2 Fld(1, 12, AC_MSKB1) //12
    #define INTRUDE1 Fld(1, 11, AC_MSKB1) //11
    #define INTRUDE0 Fld(1, 10, AC_MSKB1) //10
    #define CMPCNT Fld(6, 4, AC_MSKW10) //9:4

#define INTRU2 (IO_VIRT+0x073ec)
    #define RG0AG Fld(5, 27, AC_MSKB3) //31:27
    #define RG0ADR Fld(27, 0, AC_MSKDW) //26:0
#define INTRU3 (IO_VIRT+0x073f0)
    #define RG1AG Fld(5, 27, AC_MSKB3) //31:27
    #define RG1ADR Fld(27, 0, AC_MSKDW) //26:0
#define INTRU4 (IO_VIRT+0x073f4)
    #define RG2AG Fld(5, 27, AC_MSKB3) //31:27
    #define RG2ADR Fld(27, 0, AC_MSKDW) //26:0
#define INTRU5 (IO_VIRT+0x073f8)
    #define RG3AG Fld(5, 27, AC_MSKB3) //31:27
    #define RG3ADR Fld(27, 0, AC_MSKDW) //26:0

/*
#define REG_REGION0UPPER        0x5c
    #define REGION_SEL_INV      (1U << 31)
    #define REGION_ENABLE       (1U << 30)
    #define AGENT_ID_SHIFT      25
    #define AGENT_ID_MASK       0x1fU
    #define INTRU_ADDR_SHIFT    MID_ADDR_ALIGN_SHIFT
    #define INTRU_ADDR_MASK     0x0fffffffU      // Address mask, 256MB

#define REG_REGION0LOWER        0x60
    #define REGION_CPU_ACCESS   (1U << 31)

#define REG_REGION1UPPER        0x64
#define REG_REGION1LOWER        0x68
#define REG_REGION2UPPER        0x6d
#define REG_REGION2LOWER        0x70

#define REG_REGION3UPPER        0x74
#define REG_REGION3LOWER        0x78
#define REG_REGION4UPPER        0xc4
#define REG_REGION4LOWER        0xc8

#define REG_DRAMSTAT_EN         0x80
    #define GPAG1_EN            (1U << 12)
    #define GPAG2_EN            (1U << 13)
    #define GPAG3_EN            (1U << 14)
    #define GPAG1_MASK          (0xfU << 8)
    #define GPAG2_MASK          (0xfU << 4)
    #define GPAG3_MASK          (0xfU << 0)
    #define GPAG1_ID(x)         (((x) & 0xf) << 8)
    #define GPAG2_ID(x)         (((x) & 0xf) << 4)
    #define GPAG3_ID(x)         (((x) & 0xf) << 0)
#define REG_DRAMSTAT_CLKLEN     0x84

#define REG_GPAG1_STAT          0x3c8
#define REG_GPAG2_STAT          0x3cc
#define REG_GPAG3_STAT          0x3d0
#define REG_INTRUDER_EXT2       0x3d8           // More ID info of group 2/3
    #define G2_REGION4_SHIFT    16
    #define G3_REGION4_SHIFT    0

#define REG_INTRUDER_0          0x3ec           // Region 0 - 3
    #define INTRU_INTRUDED      (1U << 29)       // Intruded status
    #define INTRU_AGENTID_SHIFT 25              // Agent ID shift bits
    #define INTRU_AGENTID_MASK  0xf             // Agent ID mask bits

#define REG_INTRUDER_4          0x3d4           // Region 4 intruded status

#define REG_INTRUDER_EXT        0x3e8           // More ID info of group 2/3
    #define G23_AGENT_ID_MASK   0x7             // Agent ID mask, 3 bits
    #define G2_REGION0_SHIFT    28
    #define G2_REGION1_SHIFT    24
    #define G2_REGION2_SHIFT    20
    #define G2_REGION3_SHIFT    16
    #define G3_REGION0_SHIFT    12
    #define G3_REGION1_SHIFT    8
    #define G3_REGION2_SHIFT    4
    #define G3_REGION3_SHIFT    0
*/

//=====================================================================
// Type definitions

typedef enum
{
    G1_AGENT_AUDIO = 0,
    G1_AGENT_DEMUX = 1,
    G1_AGENT_VDOIN = 2,
    G1_AGENT_OSD = 3,
    G1_AGENT_PSCAN = 4,
    G1_AGENT_B2R = 5,
    G1_AGENT_CPU = 6,
    G1_AGENT_SCPOS = 7,
    G1_AGENT_VDECMC = 8,
    G1_AGENT_VDECMISC = 9,
    G1_AGENT_SKIP1 = 10,
    G1_AGENT_GFXHI = 11,
    G1_AGENT_SKIP2 = 12,
    G1_AGENT_TEST0 = 13,
    G1_AGENT_GROUP2 = 14,
    G1_AGENT_GROUP3 = 15
} G1_AGENT_ID_T;

typedef enum
{
    G2_AGENT_USB = 0,
    G2_AGENT_IMGRZ = 1,
    G2_AGENT_DDI = 2,
    G2_AGENT_SKIP1 = 3,
    G2_AGENT_DSP = 4,
    G2_AGENT_SKIP2 = 5,
    G2_AGENT_TEST1 = 6,
    G2_AGENT_SKIP3 = 7
} G2_AGENT_ID_T;

typedef enum
{
    G3_AGENT_GFXLO = 0,
    G3_AGENT_USB = 1,
    G3_AGENT_SKIP1 = 2,
    G3_AGENT_TCM = 3,
    G3_AGENT_IDETEST = 4,
    G3_AGENT_RS232 = 5,
    G3_AGENT_TEST2 = 6,
    G3_AGENT_SKIP2 = 7
} G3_AGENT_ID_T;

typedef enum
{
    // Group 1
    MID_AGENT_AUDIO = 0,
    MID_AGENT_DEMUX = 1,
    MID_AGENT_VDOIN = 2,
    MID_AGENT_OSD = 3,
    MID_AGENT_PSCAN = 4,
    MID_AGENT_B2R = 5,
    MID_AGENT_CPU = 6,
    MID_AGENT_SCPOS = 7,
    MID_AGENT_MPEG1 = 8,
    MID_AGENT_JPEG = 9,
    MID_AGENT_SKIP0 = 10,
    MID_AGENT_GFXHI = 11,
    MID_AGENT_SKIP1 = 12,
    MID_AGENT_TEST0 = 13,
    MID_AGENT_GROUP2 = 14,
    MID_AGENT_GROUP3 = 15,

    // Group 2
    MID_AGENT_USB = GROUP2_FIRST_AGENT_ID,
    MID_AGENT_IMGRZ = 17,
    MID_AGENT_DDI = 18,
    MID_AGENT_SKIP2 = 19,
    MID_AGENT_DSP = 20,
    MID_AGENT_FCI = 21,
    MID_AGENT_TEST1 = 22,
    MID_AGENT_NONE = 30,        // None, for read-only region

    // Group 3
    MID_AGENT_GFXLO = GROUP3_FIRST_AGENT_ID,
    MID_AGENT_SKIP3 = 24,
    MID_AGENT_SKIP4 = 25,
    MID_AGENT_TCM = 26,
    MID_AGENT_IDETEST = 27,
    MID_AGENT_RS232 = 28,
    MID_AGENT_TEST2 = 29,

    // Special IDs
    MID_AGENT_MAX = 31,
    MID_AGENT_INVALID = 32
} MID_AGENT_ID_T;

#define MID_FLAG_INVERSE    (1U << 0)
#define MID_FLAG_CPUACCESS  (1U << 1)
#define MID_FLAG_WRITEABLE  (1U << 2)

typedef struct
{
    BOOL fgSet;
    BOOL fgEnabled;
    UINT32 u4LowAddr;
    UINT32 u4HighAddr;
    UINT32 u4Flags;
    UINT32 u4AgentList;
    MID_AGENT_ID_T eAgentId;
} REGION_STATE_T;

typedef void (*PFN_MID_CALLBACK)(UINT32 u4Region, MID_AGENT_ID_T eAgentId,
    UINT32 u4Addr);

//=====================================================================
// Function declarations

EXTERN BOOL MID_Init(void);

EXTERN BOOL MID_Exit(void);

EXTERN BOOL MID_Reset(void);

EXTERN PFN_MID_CALLBACK MID_RegisterHandler(PFN_MID_CALLBACK pfnHandler);

EXTERN BOOL MID_SetRegion(UINT32 u4Region, MID_AGENT_ID_T eAllowedAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr);

EXTERN BOOL MID_SetRegionEx(UINT32 u4Region, MID_AGENT_ID_T eAllowedAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr, UINT32 u4Flags);

EXTERN BOOL MID_SetRegionEx2(UINT32 u4Region, MID_AGENT_ID_T eAllowedAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr, UINT32 u4Flags, UINT32 u4AgentList);

EXTERN BOOL MID_ResetRegion(UINT32 u4Region);

EXTERN BOOL MID_AllocateFreeRegion(BOOL fgReadOnly, UINT32* pu4Region);

EXTERN BOOL MID_EnableRegionProtect(UINT32 u4Region);

EXTERN BOOL MID_DisableRegionProtect(UINT32 u4Region);

EXTERN BOOL MID_GetRegionState(UINT32 u4Region, REGION_STATE_T* prRegion);

EXTERN const CHAR* MID_AgentIdToString(MID_AGENT_ID_T eAgentId);

EXTERN MID_AGENT_ID_T MID_StringToAgentId(const CHAR* szAgent);

EXTERN UINT32 MID_CntBandWidthSet(UINT32 fgEnable, const CHAR *szAgentName,
                UINT32 u4Clk);

EXTERN UINT32 MID_CntBandWidthGet(UINT32 u4GrpId, CHAR *szAgentName,
                UINT32 *pu4AgentId, UINT32 *pu4Enable);

EXTERN VOID MID_ResetStatus(void);        
EXTERN VOID MID_QueryIntrusionStatus(void);

//---------------------------------------------------------------------


#endif  // X_MID_H

