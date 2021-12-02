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
 * $RCSfile: fci_type.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fci_type.h
 *  All types related to FCI are declared in this file.
 */

#ifndef FCI_TYPE_H
#define FCI_TYPE_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

LINT_SAVE_AND_DISABLE
#include "x_typedef.h"

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

// DMA memory alignment
#define FCI_MEM_ALIGN            4

/// slot number is based on FCIIC define.
#define NONE_SLOT   (3)
#define SDC_SLOT    (0)
#define MSP_SLOT    (1)
#define SMM_SLOT    (2)

#define SLOT_STATE_EMPTY            0
#define SLOT_STATE_INIT               1
#define SLOT_STATE_UNKNOWN     2
#define SLOT_STATE_READY           3
#define SLOT_STATE_BUSY              4
#define SLOT_STATE_CMD               5
#define SLOT_STATE_TRANS           6

#define FCI_UNKNOWN_CARD         0
#define FCI_SD_CARD                      1
#define FCI_MMC_CARD                   2
#define FCI_MS_CARD                      3
#define FCI_MSPRO_CARD               4
#define FCI_SM_CARD                      5
#define FCI_XD_CARD                      6
#define FCI_SDIO_CARD                  7
#define FCI_SDIO_COMBO_CARD   8
#define FCI_SDHC_CARD                 9

#define MS_DEV_TOTALBYTE(mba) (((UINT32)(mba.u2EffBlkNumber - 2)) * mba.u2BlkSz * 1024)

#define MSPRO_DEV_TOTALBYTE(sysinfo) (((UINT32)sysinfo->u2UnitSz) * sysinfo->u2BlkSz * sysinfo->u2UsrAreaBlks)

/** SCR field and memory mapping
__packed struct strucSD_SCR {
    UINT32 b4SdSpec:4;      // bit 59-56
    UINT32 b4SCRStruct:4;  // bit 63-60

    UINT32 b4SdBusWid:4;   // bit 51-48
    UINT32 b3SdSecu:3;       // bit 54-52
    UINT32 b1DtStat:1;        // bit 55-55

    UINT16 u2Reserv;           // bit 47-32
    UINT32 u4ManuUsage;    // bit 31-0
};
*/
/// get SCR register field: SD_BUS_WIDTHS [51:48]
#define SD_SCR_SPEC(scrptr)         		((*scrptr) & 0xF)
#define SD_SCR_STRUCTURE(scrptr)         	(((*scrptr) >> 4) & 0xF)
#define SD_SCR_BUS_WIDTHS(scrptr)         (((*scrptr) >> 8) & 0xF)
#define SD_SCR_SECURITY(scrptr)         	(((*scrptr) >> 12) & 0x7)
/**
__packed struct strucSD_CSD {
    UINT32 b1One:1;
    UINT32 b7CRC:7;
    UINT32 b2Reserv5:2;
    UINT32 b2FileFormat:2;
    UINT32 b1TmpWrProtect:1;
    UINT32 b1PermWrProtect:1;
    UINT32 b1CopyFlag:1;
    UINT32 b1FileFormatGp:1;
    UINT32 b5Reserv4:5;
    UINT32 b1PtBlkWrEn:1;
    UINT32 b4MaxWrBlkLen:4;
    UINT32 b3WrSpeedFactor:3;
    UINT32 b2MMCReserv:2;
    UINT32 b1WrGpEn:1;
    UINT32 b7WrGpSize:7;
    UINT32 b7EraseSize:7;
    UINT32 b1EraseEn:1;
    UINT32 b3CSizeMult:3;
    UINT32 b3MaxWrVddMax:3;
    UINT32 b3MaxWrVddMin:3;
    UINT32 b3MaxRdVddMax:3;
    UINT32 b3MaxRdVddMin:3;
    UINT32 b12CSize:12;
    UINT32 b2Reserv2:2;
    UINT32 b1DsrImp:1;
    UINT32 b1RdBlkMis:1;
    UINT32 b1WrBlkMis:1;
    UINT32 b1PtBlkRdEn:1;
    UINT32 b4MaxDtLen:4;
    UINT32 b12CCC:12;
    UINT8 u1MaxXfer;
    UINT8 u1RdT2;
    UINT8 u1RdT1;
    UINT32 b6Reserv1:6;
    UINT32 b2CSDStruct:2;
}
*/

/// get CSD register field: DSR_IMP [76:76]
#define SD_CSD_DSRIMP(csdptr)         (((*(csdptr + 2)) >> 12) & 0x1)

/*
C_SIZE: Physical Layer Specification Version 2.00 page 109.
This field is expanded to 22 bits and can indicate up to 2 TBytes (It is the same as the maximum
memory space specified by a 32-bit block address.)
This parameter is used to calculate the user data area capacity in the SD memory card (not include the
protected area). The user data area capacity is calculated from C_SIZE as follows:
memory capacity = (C_SIZE+1) * 512K byte
As the maximum capacity of the Physical Layer Specification Version 2.00 is 32 GB, the upper 6 bits of
this field shall be set to 0.
*/
#define SDHC_CSD_CSIZE(csdptr)         ((((*(csdptr + 1)) >> 16) & 0xFFFF) | ((*(csdptr + 2)) & 0x3F))

/*
C_SIZE: Physical Layer Specification Version 1.x.
*/
#define SD_CSD_CSIZE(csdptr)         ((((*(csdptr + 1)) >> 30) & 0x3) | (((*(csdptr + 2)) & 0x3FF) << 2))
#define SD_CSD_CSIZEMULT(csdptr)         (((*(csdptr + 1)) >> 15) & 0x7)

/*
READ_BL_LEN: Physical Layer Specification Version 2.00 page 109.
This field is fixed to 9h, which indicates READ_BL_LEN=512 Byte.
*/
#define SD_CSD_READBLLEN(csdptr)         (((*(csdptr + 2)) >> 16) & 0xF)
#define SD_CSD_SPEED(csdptr)                 ((*(csdptr + 3)) & 0xFF)

/*
__packed struct strucSD_CID {
    UINT32 b1One:1;
    UINT32 b7CRC:7;
    UINT32 b12ManuDate:12;
    UINT32 b4Reserv:4;
    UINT32 u4SerialNum;
    UINT8 u1PdRev;
    UINT8 au1PdName[5];
    UINT16 u2AppID;
    UINT8 u1ManuID;
}

__packed struct strucMMC_CID {
    UINT32 b1One:1;
    UINT32 b7CRC:7;
    UINT8 u1ManuDate;
    UINT32 u4SerialNum;
    UINT8 u1PdRev;
    UINT8 au1PdName[6];
    UINT16 u2AppID;
    UINT8 u1ManuID;
}
*/
/// SD CID register field: PNM [103:64]
#define SD_CID_PNM_B5(cidptr)        ((*(cidptr + 2)) & 0xFF)
#define SD_CID_PNM_B4(cidptr)        (((*(cidptr + 2)) >> 8) & 0xFF)
#define SD_CID_PNM_B3(cidptr)        (((*(cidptr + 2)) >> 16) & 0xFF)
#define SD_CID_PNM_B2(cidptr)        (((*(cidptr + 2)) >> 24) & 0xFF)
#define SD_CID_PNM_B1(cidptr)        ((*(cidptr + 3)) & 0xFF)

/// MMC CID register field: PNM [103:56]
#define MMC_CID_PNM_B6(cidptr)        (((*(cidptr + 1)) >> 24) & 0xFF)
#define MMC_CID_PNM_B5(cidptr)        ((*(cidptr + 2)) & 0xFF)
#define MMC_CID_PNM_B4(cidptr)        (((*(cidptr + 2)) >> 8) & 0xFF)
#define MMC_CID_PNM_B3(cidptr)        (((*(cidptr + 2)) >> 16) & 0xFF)
#define MMC_CID_PNM_B2(cidptr)        (((*(cidptr + 2)) >> 24) & 0xFF)
#define MMC_CID_PNM_B1(cidptr)        ((*(cidptr + 3)) & 0xFF)

/*****************************************************************************
 * Macro definitions for ms_l2p.c
 */
#define MAX_MS_EXTRA_NUM            (16)
#define MAX_PG_IN_BLK               (32)
#define MAX_SEG_NUMBER              (16)
#define FIRSTSEG_LOGICBLK           (494)
#define MAX_LOGICBLK_INSEG          (496)
#define LOGICBLK_MINUS_NUM          (MAX_LOGICBLK_INSEG - FIRSTSEG_LOGICBLK)
#define PHYBLKS_PERSEG              (512)
#define LOGICBLK_START(seg)         (((seg)==0) ? 0 : (((seg) * MAX_LOGICBLK_INSEG) - LOGICBLK_MINUS_NUM))

#define FCI_STATE_INIT          0
#define FCI_STATE_IDLE          1
#define FCI_STATE_BUSY          2

#define MSP_REG_SIZE        0x20
#define MSPRO_PAGE_SIZE         0x200

#define IO_CMD_READ             0
#define IO_CMD_WRITE            1
#define IO_CMD_ERASE            2
#define IO_CMD_QUERY            3

/*
 * Error code.
 */
#define S_OK                                            0
#define E_CMD_TIMEOUT                       1
#define E_NO_CARD                               2
#define E_SD_R3_OCR_BUSY                3
#define E_INVALID_CARD                       5
#define E_INVALID_BLOCK_SIZE           6 /* over 2048 or not multiple of 4 */
#define E_DATA_CRC_ERROR                 7
#define E_CMD_RSP_CRC_ERR               9
#define E_DATA_TIMEOUT                      10
#define E_STATUS                                    11
#define E_INVALID_ARG                         12     /* Joshua add */
#define E_SD_APPCMD_FAILED              13
#define E_RESPBUSY                                14 /* Joshua add */
#define E_NO_RESPONSE                        15
#define E_SD_NOT_SUPPORT_WIDE_BUS       16
#define E_UNKNOWN_CARD                     20
#define E_MS_CMD_CRC_ERROR             21
#define E_MS_SET_RW_REG_ADRS         22
#define E_CARD_IS_LOCKED                    60
#define E_FAIL                                          0xFF

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

__packed struct strucDevInfoEntry
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT8 u1DevId;
    UINT8 au1Reserved[3];
};

typedef struct strucDevInfoEntry DEVINFO_ENTRY_T;

__packed struct strucMsproSysInfo
{
    UINT8 u1MSClass;      /* 2, MSPro, other reserved. */
    UINT8 u1Reserved2;
    UINT16 u2BlkSz;       /* Erase size, number of Units */
    UINT16 u2TotalBlks;   /* Number of the whole block */
    UINT16 u2UsrAreaBlks; /* Number of User Area blocks */
    UINT16 u2PageSz;      /* Writing and reading size (number of Units) */
    UINT16 u2Reserved3;
    UINT8 u1TimeZone;     /* Assembly date */
    UINT16 u2Year;
    UINT8 u1Month;
    UINT8 u1Day;
    UINT8 u1Hour;
    UINT8 u1Minute;
    UINT8 u1Second;
    UINT32 u4SerialNumber;
    UINT8 u1AssemblyMaker;
    UINT8 au1AssemblyModel[3];
    UINT16 u2MemoryMaker;
    UINT16 u2MemoryModel;
    UINT32 u4Reserved4;
    UINT8 u1VCC;
    UINT8 u1VPP;
    UINT16 u2CtrlNumber;
    UINT16 u2CtrlFunction;
    UINT16 u2StartSector;
    UINT16 u2UnitSz; /* Number of bytes per unit. */
    UINT8 u1SubClass;
    UINT32 u4Reserved5;
    UINT8 u1InterfaceType; /* 0: Serial only, 1:Parallel & Serial, others reserved. */
    UINT16 u2CtrlCode;
    UINT8 u1FormatType;    /* 1: FAT, Others: reserved. */
    UINT8 u1Reserved6;
    UINT8 u1DevType;       /* 0:read/write, 1:ROM, 2:ROM2, 3:ROM3, Others:reserved. */
    UINT8 au1Reserved7[7];
    UINT8 au1MSProId[16];
    UINT8 au1Reserved8[16];
};

typedef struct strucMsproSysInfo MSPRO_SYSINFO_T;

__packed struct strucMSPRO_ATR
{
    UINT16 u2Signature; /* 0xA5C3 */
    UINT16 u2Version;   /* 0x01xx */
    UINT8 u1DevInfoCnt;
    UINT8 au1Reserved1[11];
    DEVINFO_ENTRY_T arDevInfoEntry[12];
    UINT8 au1Reserved256[256];
    UINT8 au1Padding[112 + 512];

    MSPRO_SYSINFO_T *prSysInfo;
    UINT8 *pu1ModelName;
};

__packed struct strucMS_BATR
{
    /*- Header ---------------*/ /* Page 0 */
    UINT16 u2BlockId;  /* 0x0001 */
    UINT16 u2Format;   /* 0x0100 */
    UINT8 au1HdrReserved1[184];
    UINT8 u1DataEntry; /* 2 */
    UINT8 au1HdrReserved2[179];
    /*- System entry ---------*/
    UINT32 u4DisBlkStartAddr;  /* 0x00000000 */
    UINT32 u4DisBlkSz;         /* - */
    UINT8 u1DisBlkId;          /* 0x01 */
    UINT8 au1SysReserved1[3];  /* 0x00... */
    UINT32 u4CisIdiStartAddr;  /* 0x00000200 */
    UINT32 u4CisIdiSz;         /* 0x00000200 */
    UINT8 u1CisIdiId;          /* 0x0A */
    UINT8 au1SysReserved2[3];  /* 0x00... */
    UINT8 au1SysReserved3[24]; /* 0x00... */
    /*- Boot & Attribute Information -*/
    UINT8 u1MSClass;  /* 1: ver1.xx, other:reserved. */
    UINT8 u1FmtUVal2; /* 2 */
    UINT16 u2BlkSz;   /* 16KB:0x10, 8KB: 0x08, other: reserved. */
    UINT16 u2BlkNumber;
    UINT16 u2EffBlkNumber;
    UINT16 u2PageSz;  /* 0x0200 fixed. */
    UINT8 u1ExtraSz;  /* Extra Data Area Size, 0x10, fixed. */
    UINT8 u1FmtUVal1; /* 1 */
    UINT8 au1AssemblyDate[8];
    UINT8 u1FmtUVal0; /* 0 */
    UINT8 au1ManuArea[3];
    UINT8 u1AssemblyManuCode;
    UINT8 au1AssemblyModel[3];
    UINT16 u2MemoryManu;
    UINT16 u2MemoryDev;
    UINT16 u2TotalMB;
    UINT8 u1FmtUVal1_1; /* 1 */
    UINT8 u1FmtUVal1_2; /* 1 */
    UINT8 u1VCC;
    UINT8 u1VPP;
    UINT16 u2CtrlNumber;
    UINT16 u2CtrlFunction; /* 0x1001: Normal, 0x01xx: MagicGate support. */
    UINT8 au1Reserved[9];
    UINT8 u1InterfaceType; /* 0: Serial, 1: Serial&Parallel, other: reserved. */
    UINT16 u2FmtUVal0_1;   /* 0x0000 */
    UINT8 u1FormatType;    /* 1: FAT, others: reserved. */
    UINT8 u1Application;   /* 0: General purpose, other reserved. */
    UINT8 u1DevType;       /* 0, MemoryStick, 1:ROM, 2:ROM2, 3:ROM3, other:reserved */
    UINT8 au1Reserved1[22];
    UINT8 u1FmtUVala;      /* 0x0a */
    UINT8 u1FmtUVal1_3;    /* 1 */
    UINT8 au1Reserved2[15];
    /*- Disabled Block Data --*/ /* Page 1 */
    UINT16 u2InfoBlkNo;
    UINT16 au2InitDefectBlkNo[255];
    /*- CIS ------------------*/ /* Page 2 */
    UINT8 au1CisDev[6];
    UINT8 au1CisDevOc[6];
    UINT8 au1CisJedec_C[4];
    UINT8 au1CisManfid[6];
    UINT8 au1CisVers_1[32];
    UINT8 au1CisFuncCid[4];
    UINT8 au1CisFuncCe[4];
    UINT8 au1CisFuncCe2[5];
    UINT8 au1CisConfig[7];
    UINT8 au1CisSomething[90];
    UINT8 u1CisEnd; /* 0xff */
    UINT8 au1CisReserved[91];
    /*- IDI ------------------*/
    UINT16 u2Config;         /* 0x848A */
    UINT16 u2Cylinders;
    UINT16 u2Reserved1;      /* 0x0000 */
    UINT16 u2Headers;
    UINT16 u2BytesInTrack;   /* 0x0000 */
    UINT16 u2BytesInSectors; /* 0x0200 */
    UINT16 u2SectorsInTrack;
    UINT16 u2MSWSectors;
    UINT16 u2LSWSectors;
    UINT16 u2Reserved2;        /* 0x0000 */
    UINT8 au1SerialNumber[20]; /* 0x2020... */
    UINT16 u2BufType;          /* 0x0002 */
    UINT16 u2BufSz;            /* 0x0002 - 512 bytes per unit in this field. */
    UINT16 u2LongCmdEcc;       /* 0x0004 */
    UINT8 au1FirmVer[8];
    UINT8 au1ModelName[40];
    UINT16 u2Reserved3;  /* 0x0001 */
    UINT16 u2DualSup;    /* 0x0000 */
    UINT16 u2DmaSup;     /* 0x0200 */
    UINT16 u2Reserved4;  /* 0x0000 */
    UINT16 u2PioMode;    /* 0x0100 */
    UINT16 u2DmaMode;    /* 0x0000 */
    UINT16 u2FieldValid; /* 0x0001 */
    UINT16 u2CurCylinders;
    UINT16 u2CurHeads;
    UINT16 u2CurSectorsInTrack;
    UINT32 u4CurTotalSectors;
    UINT16 u2MultiSectorSetting; /* 0x0100 */
    UINT32 u4TotalUserSectors;
    UINT16 u2SingleDma;          /* 0x0000 */
    UINT16 u2MultiDma;           /* 0x0000 */
    UINT8 au1Reserved3[128];
};

typedef struct strucMSPRO_ATR MSPRO_ATR_T;
typedef struct strucMS_BATR MS_BATR_T;

/** MS, MSpro card boot structure.
*  This structure is used to store boot attribute information.
*/
typedef union __unionATR
{
    MSPRO_ATR_T rMsproAtr; ///< MSpro boot block format..
    MS_BATR_T rMsBootAtr;  ///< MS boot block format. 3 pages. 3*512 Bytes.
    UINT8 au1Space[2048];
} MSP_ATR_T;

/*****************************************************************************
 * Type definitions for ms_l2p.c
 */

/** MS card segment structure.
*  This structure is used to ms segment information.
*/
typedef struct __structSegInfo
{
    INT32 fgInit;                        ///< MS segment table initial flag.
    INT32 i4FreeBlks;                    ///< MS segment the number of free blocks.
    UINT16 au4FreeTable[PHYBLKS_PERSEG]; ///< MS segment record free block table.
} SEGINFO_T;

/** MS, MSP card data structure.
 *  This structure is used to store card information.
 */
typedef struct __MSPSlot
{
    UINT32 u4State;                                                             ///< MSP Slot run-time state.
    UINT32 u4CardType;                                                          ///< MSP Slot media card type.
    UINT32 fgWriteProtected;                                                    ///< MSP write protect flag.
    UINT32 fgSupportParallel;                                                   ///< MSP support 4 bit bus flag.
    UINT32 fg4BitEn;                                                            ///< Parallel enable/disable flag.
    UINT32 fgLastpage;                                                            ///< Read MS last page or not.
    INT32 fgL2PInit;                                                            ///< Segment table initial flag..
    MSP_ATR_T uATR;                                                             ///< MS, MSpro attribute table.
    UINT8 au1MsReg[MSP_REG_SIZE];                                               ///< MS register table.
    UINT16 au4LPTable[(MAX_SEG_NUMBER *MAX_LOGICBLK_INSEG)-LOGICBLK_MINUS_NUM]; ///< MS logical to physical mapping table. Max 128 MB card.
    SEGINFO_T aSegInfo[MAX_SEG_NUMBER];                                         ///< Segment table.
    UINT8   *pu1MsMemBuf;
    UINT8   *pu1Extra;
} MSP_T;

/** SD, SDIO function enum.
 *  define SD, SDIO function number.
 */
typedef enum __SDC_FUNCNUM
{
    SDC_COMMOM = 0,
    SDIO_FUNC_1,
/*
    SDIO_FUNC_2,
    SDIO_FUNC_3,
    SDIO_FUNC_4,
    SDIO_FUNC_5,
    SDIO_FUNC_6,
    SDIO_FUNC_7,
*/
    SDC_FUNC_END
} SDC_FUNCNUM_T;

/** SD card data structure.
 *  This structure is used to store card information.
 */
typedef struct __SDCSlot
{
    UINT32 u4State;    ///< card operating state.
    UINT32 u4CardType; ///< SD or MMC type.
    UINT32 u4CardSize; ///< card size in block (512 bytes/block).
    UINT32 fg4BitEn;   ///< card support 4 bit parallel bus or not.
    BOOL    fgSingleBlockAccess;  // single block access or multiple block access

    UINT32 au4CID[4];  ///< CID: Card Identification register, 128 bits.
    UINT32 au4CSD[4];  ///< CSD: Card-Specific Data register, 128 bits.
    UINT32 au4SCR[2];  ///< SCR: SD card configuration register, 64 bits.

    UINT16 u2BlkSz[SDC_FUNC_END];    ///< SD, SDIO block size. SD use index 0, SDIO use index 1-7.
    UINT16 u2RCA;      ///< SD relative card address.

    UINT8 u1PNM[7];    ///< store product name string.

    // SDIO data structure.
    UINT8   u1MemPresent;   ///< SDIO Memory present, 1 bit.
    UINT8   u1NumIO;            ///< SDIO number of IO functions, 3 bits.
    //UINT8   u1SelectIO;          ///< Select SDIO IO function.
} SDC_T;

/** SM/xD card data structure.
 *  This structure is used to store card information.
 */
typedef struct __SMXDSlot
{
    UINT32 u4PhyBlkNs;    ///< total no of physical blocks in card.
    UINT32 u4LogBlkNs;    ///< total no of logical blocks in card.
    UINT16 u2CardType;    ///< card is SM or xD.
    UINT16 u2NowZone;     ///< read/write operation in this zone now . 0xFF is NULL zone.
    UINT16 u2TotalZone;   ///< total zone in this card.
    UINT16 u2Size;        //< card size, unit = MBytes.
    UINT16 u2PageSz;      ///< page size, unit = Bytes.
    UINT16 u2BlkSz;       ///< nof pages/block.
    BOOL fgDetect;        ///< card exist or not.
    BOOL fgX4MulBlk;      ///< card support x4 multiple block program.
    UINT8 au1ManuName[8]; ///< card manufacture name.
    UINT8 au1ProdName[5]; ///< card product name.
    UINT8 au1VersName[4]; ///< card version name.
} SMXD_T;

/** FCI card data structure.
 *  Store SD, MMC, MS, MSpro, SM/xD information.
 */
typedef struct __FCIController
{
    UINT32 u4State;
    UINT32 au4Padding[3];
    SDC_T rSDCSlot;   ///< SD/MMC card data structure.
    MSP_T rMSPSlot;   ///< MS/MSpro card data structure.

#ifdef CC_FCI_SMXD_SUPPORT
    SMXD_T rSMXDSlot; ///< SM/xD card data structure.
#endif  // #ifdef CC_FCI_SMXD_SUPPORT
} FCISC_T;

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

LINT_RESTORE
#endif // FCI_TYPE_H
