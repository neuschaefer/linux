/*
 *
 * cppi41_hw_mbox_if.h
 * Description:
 * CPPI access via HW mailbox
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

#ifndef __CPPI41_HW_MBOX_IF_H_
#define __CPPI41_HW_MBOX_IF_H_

#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>

 /************************************************/
/*                 Structures                   */
/************************************************/

/* CPPI HW mailbox commands enum */
typedef enum
{
    CPPI41_HWMBOX_CMD_ACC_CH_OPEN = 0           ,    /* Accumulator open command */
    CPPI41_HWMBOX_CMD_ACC_CH_OPEN_REPLY         ,    /* Accumulator open reply command */
    CPPI41_HWMBOX_CMD_ACC_CH_CLOSE              ,    /* Accumulator close command */
    CPPI41_HWMBOX_CMD_GET_REGION_MEM_INFO       ,    /* Descriptors region memory info command */
    CPPI41_HWMBOX_CMD_GET_REGION_MEM_INFO_REPLY ,    /* Descriptors region memory info reply command */
    CPPI41_HWMBOX_CMD_GET_OFFCHIP_MEM_INFO_REPLY,    /* OffChip memory info reply command */
    CPPI41_HWMBOX_CMD_GET_BPOOL_MEM_INFO_REPLY  ,    /* OffChip bPool memory info reply command*/
    CPPI41_HWMBOX_CMD_FAILED                    ,    /* command processing failed command, use only for reply - the error code is from the type Cppi41HwMboxRetCode_e */
    CPPI41_HWMBOX_CMD_COUNT
}Cppi41HwMboxCmd_e;

/* error codes for CPPI41_HWMBOX_CMD_FAILED command */
typedef enum
{
    CPPI41_HWMBOX_RET_CODE_OK = 0,
    CPPI41_HWMBOX_RET_CODE_UNSUPPORTED_CMD,
    CPPI41_HWMBOX_RET_CODE_CMD_ERROR,
    CPPI41_HWMBOX_RET_CODE_COUNT,
}Cppi41HwMboxRetCode_e;

/* Cppi HW mailbox fail message */
typedef struct
{
    Cppi41HwMboxCmd_e       cmd;        /* command type - must be CPPI41_HWMBOX_CMD_FAILED */
    Cppi41HwMboxRetCode_e   failCode;   /* fail code */
} Cppi41HwMboxFailMsg_t;

/* Cppi HW mailbox message to open accumulator channel */
typedef struct
{
    Cppi41HwMboxCmd_e       cmd;        /* command type - must be CPPI41_HWMBOX_CMD_ACC_CH_OPEN  */
    Cppi4AccumulatorCfg     initCfg;    /* The accumulator channel init configuration */
} Cppi41HwMboxAccChOpenMsg_t;

/* Cppi HW mailbox response message to open accumulator channel */
typedef struct
{
    Cppi41HwMboxCmd_e   cmd;            /* command type - must be CPPI41_HWMBOX_CMD_ACC_CH_OPEN_REPLY */
    Ptr                 curPage;        /* Current accumulator page physical address */
    PAL_Cppi4AccChHnd   accChHnd;       /* handle to accumulator channel handle, this field is mandatory for closing the channel */
}Cppi41HwMboxAccChOpenReplyMsg_t;

/* Cppi HW mailbox message to close accumulator channel */
typedef struct
{
    Cppi41HwMboxCmd_e   cmd;        /* command type */
    PAL_Cppi4AccChHnd   accChHnd;   /* The accumulator channel init configuration */
} Cppi41HwMboxAccChCloseMsg_t;

/* Cppi HW mailbox message to get descriptor region memory info, phyical address and length */
typedef struct
{
    Cppi41HwMboxCmd_e           cmd;    /* command type */
    PAL_CPPI_PP_DESC_REGIONs_e  region; /* region number */
    Uint32                      qMgr;   /* queue manager */
} Cppi41HwMboxDescRegionMemInfoGetMsg_t;

/* Cppi HW mailbox message to reply to a get descriptor region memory info message */
typedef struct
{
    Cppi41HwMboxCmd_e   cmd;            /* command type */
    Ptr                 regionPhyAddr;  /* region memory physical address */
    Uint32              length;         /* region memory length in bytes */
} Cppi41HwMboxDescRegionMemInfoReplyMsg_t;

/* Cppi HW mailbox message to reply to a get offChip memory info message */
typedef struct
{
    Cppi41HwMboxCmd_e   cmd;             /* command type */
    Uint32              offChipPhyAddr;  /* offChip physical address */
    Uint32              length;          /* offChip memory length in bytes */
} Cppi41HwMboxOffChipMemInfoReplyMsg_t;

typedef struct
{
    Cppi41HwMboxCmd_e   cmd;              /* command type */
    Uint32              poolNum;          /* buffer pool number */
    Uint32              reserved;            /* Place holder */
} Cppi41HwMboxCppiBpoolMemInfoGetMsg_t;

/* Cppi HW mailbox message to reply to a get offChip cppi buffer pool memory info message */
typedef struct
{
    Cppi41HwMboxCmd_e   cmd;               /* command type */
    Uint32              offChipPhyAddr;    /* offChip physical address */
    Uint32              length;            /* offChip memory length in bytes */
} Cppi41HwMboxCppiBpoolMemInfoReplyMsg_t;

bool  Cppi41HwMboxAccChangeEndianness(Cppi41HwMboxAccChOpenMsg_t *destCfgData, endianness_e endianity);
#endif /* __CPPI41_HW_MBOX_IF_H_ */
