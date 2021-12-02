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
 * $RCSfile: storage_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file storage_if.h
 *  storage_if.h The Storage devices upper layer interface header file.
 */

#ifndef STORAGE_IF_H
#define STORAGE_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define STORG_DEV_MODELNAME_LEN     60

#define SECTOR_2N               (9)
#define SECTOR_BSIZE            (1 << SECTOR_2N)

#define STORG_FLAG_CF           ((UINT32) 1 << 0)
#define STORG_FLAG_HD           ((UINT32) 1 << 1)
// FCI.
#define STORG_FLAG_MS           ((UINT32) 1 << 2)
#define STORG_FLAG_SD           ((UINT32) 1 << 3)
#define STORG_FLAG_SMXD         ((UINT32) 1 << 4)

#define STORG_FLAG_NAND         ((UINT32) 1 << 5)
#define STORG_FLAG_NOR          ((UINT32) 1 << 6)
#define STORG_FLAG_EEPROM       ((UINT32) 1 << 7)

#define FCI_UNKNOWN_CARD        0
#define FCI_SD_CARD             1
#define FCI_MMC_CARD            2
#define FCI_MS_CARD             3
#define FCI_MSPRO_CARD          4
#define FCI_SM_CARD             5
#define FCI_XD_CARD             6

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define BYTE2SECTOR(x)          (((x + SECTOR_BSIZE) - 1) >> SECTOR_2N)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum
{
    STORG_DEV_ABSENT = 0,
    STORG_DEV_FAIL,
    STORG_DEV_READY,    
    STORG_DEV_INIT,
    STORG_DEV_LAST
}STORG_DEV_STATUS_T;

typedef enum
{
    STORG_TYPE_INVALID = 0,
    STORG_TYPE_BLK,
    STORG_TYPE_CHR,
    STORG_TYPE_LAST
}STORG_TYPE_T;

typedef enum
{
    STORG_OP_NONE = 0,
    STORG_OP_READ,
    STORG_OP_WRITE,
    STORG_OP_ERASE,
    STORG_OP_FLUSH,     /* HD only. */
    STORG_OP_STDBYTIME, /* HD only. */
    STORG_OP_INSERT,
    STORG_OP_REMOVE,
    STORG_OP_LAST
}STORG_OP_T;

typedef enum
{
    COND_FAIL = -1,    /* BLKDEV_COND_FAIL */
    COND_INIT,         /* Request just initial. */
    COND_READ_OK,      /* BLKDEV_COND_READ_OK */
    COND_WRITE_OK,     /* BLKDEV_COND_WRITE_OK */
    COND_ERASE_OK,     /* BLKDEV_COND_ERASE_OK */
    COND_FLUSH_OK,     /* BLKDEV_COND_FLUSH_OK */
    COND_STDBYTIME_OK, /* Not used in middleware. */
    COND_INSERT_OK,    /* BLKDEV_STATUS_READY */
    COND_REMOVE_OK,    /* BLKDEV_STATUS_ABSENT */
    COND_HOTSWAP_FAIL, /* BLKDEV_STATUS_FAIL */
    CNOD_LAST
}STORG_COND_T;

/* function pointer type */
typedef VOID (*STORG_NFY_FCT)(VOID *pvNfyTag, STORG_COND_T eNfyCond, VOID *pvData);
typedef VOID (*STORG_HOTSWAP_NFY_FCT)(VOID *pvNfyTag, STORG_COND_T eNfyCond, VOID *pvData);

/* successful operation will return 0, otherwise failed. */
typedef INT32 (*STORG_READ_FCT)(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
typedef INT32 (*STORG_WRITE_FCT)(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
typedef INT32 (*STORG_ERASE_FCT)(UINT64 u8Offset, UINT32 u4BlkNum);
typedef INT32 (*STORG_FLUSH_FCT)(VOID);
typedef INT32 (*STORG_STDBYTIME_FCT)(UINT32 u4Seconds); /* set standby timer, HDD only. */
typedef INT32 (*STORG_INSERT_FCT)(VOID);                /* media insert */
typedef INT32 (*STORG_REMOVE_FCT)(VOID);                /* media remove */

typedef struct _STORG_FUNCTBL_T
{
    STORG_READ_FCT pfnRead;
    STORG_WRITE_FCT pfnWrite;
    STORG_ERASE_FCT pfnErase;
    STORG_FLUSH_FCT pfnFlush;
    STORG_STDBYTIME_FCT pfnStdbyTime;
    STORG_INSERT_FCT pfnInsert;
    STORG_REMOVE_FCT pfnRemove;
} STORG_FUNCTBL_T;

typedef struct
{
    UINT32 u4DevFlag;
    HANDLE_T hRequestMsg;
    HANDLE_T hThread;
    HANDLE_T hSemaExit;
    HANDLE_T hSemaLock;
    HANDLE_T hSemaDone;
} STORG_THRD_T;

typedef struct
{
    UINT32 u4DevFlag;   /* Input from outside. */
    INT32 fgInit;
    INT32 i4RefCnt;     /* Open Device reference. */
    VOID *pvHotswapTag; /* Set from middleware driver. */
    STORG_HOTSWAP_NFY_FCT pfnHotswapNfy;
    /* Below all are generated from u4DevFlag. */
    STORG_TYPE_T eDevType; /* blk dev must check io bytes, chr needn't. */
    STORG_THRD_T *prStorgThrd;
    STORG_FUNCTBL_T *prFuncTbl;
} STORG_DEV_T;

typedef struct
{
    STORG_DEV_T *prDev;
    STORG_OP_T eOp;
    VOID *pvData;
    VOID *pvTag;
    UINT64 u8Offset;      /* storage device byte offset. */
    UINT32 u4Count;       /* memory used byte, or storage used bytes */
    VOID *pvBuf;          /* read/write memory pointer. */
    STORG_NFY_FCT pfnNfy; /* NULL - sync, otherwise - async. */
    UINT32 *pu4XferCnt;
    STORG_COND_T eResult;    
} STORG_REQ_T;

//-----------------------------------------------------------------------------
//  Public functions
//-----------------------------------------------------------------------------
EXTERN INT32 STORGInit(VOID);
EXTERN INT32 STORGStop(VOID);
EXTERN STORG_DEV_T *STORGOpenDev(UINT32 u4DevFlag);
EXTERN INT32 STORGCloseDev(STORG_DEV_T *prDrv);
EXTERN INT32 STORGSendReq(STORG_DEV_T *prDev, STORG_OP_T eOp, VOID *pvData, VOID *pvTag,
                          STORG_NFY_FCT pfnNfy, UINT64 u8Offset, UINT32 u4Count, VOID *pvBuf,
                          UINT32 *pu4XferCnt);
EXTERN UINT64 STORG_GetDevSize(UINT32 u4DevFlag, UINT16 ui2_id);
EXTERN STORG_DEV_STATUS_T STORG_GetDevStatus(UINT32 u4DevFlag);
EXTERN CHAR *STORG_GetDevModel(UINT32 u4DevFlag);
EXTERN INT32 STORG_GetDevCard(UINT32 u4DevFlag);
EXTERN INT32 STORG_GetDevStandbyTime(UINT32 u4DevFlag);
#endif /* STORAGE_IF_H */
