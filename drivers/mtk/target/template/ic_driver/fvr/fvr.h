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
 * $RCSfile: fvr.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fvr.h
 *  FVR driver - FVR header
 */

#ifndef _FVR_H
#define _FVR_H

#include "fvr_if.h"
#include "x_hal_5381.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//
// PID Data struct 
//
#define FVR_NUM_PID_INDEX                   32  
#define FVR_SCRAMBLE_PID_INDEX              8

#define FVR_GBL_SIZE                        9   // 36 bytes
#define FVR_PID_SIZE                        16  // 64 bytes
#define FVR_GBL_COUNT                       1
#define FVR_PID_COUNT                       32

//
// Base addresses
//
#if 0
#define FVR_REG_BASE		                (0x2001A000)
#define FVR_IMEM_BASE                       (0x20000000)
#define FVR_DMEM_BASE                       (0x20005000)
#define FVR_PID_TABLE_BASE                  (0x20022000)
#else
#define FVR_REG_BASE		                (0x20035000)
#define FVR_IMEM_BASE                       (0x20036000)
#define FVR_DMEM_BASE                       (0x20037000)
#define FVR_PID_TABLE_BASE                  (0x20038000)
#endif

#define FVR_IMEM_LEN                        (2048)
#define FVR_DMEM_LEN                        (2048)
#define FVR_PID_TABLE_LEN                   256//(2048)

#define FVR_GBL_PID_BASE                    FVR_DMEM_BASE
#define FVR_PER_PID_BASE                    (FVR_GBL_PID_BASE + FVR_GBL_SIZE* 4)

//
#define FVR_INT_QUEUE_DEPTH                 32

//
// FVR registers
//
#define FVR_REG_GLOBAL_CTRL                0
#define FVR_REG_CONFIG_1                   1
#define FVR_REG_CONFIG_2                   2
#define FVR_REG_CONFIG_3                   3
#define FVR_REG_CONFIG_4                   4

#define FVR_REG_FVR_uP_CTRL                96
#define FVR_REG_FVR_uP_CONFIG              97
#define FVR_REG_FVR_DBM_CTRL               16
#define FVR_REG_RECORD_BYPASS              29
#define FVR_REG_DBM_SPACING                31
#define FVR_REG_REC_TIMESTAMP_DIV          33
#define FVR_REG_REC_TIMESTAMP_COUNTER      34
#define FVR_REG_REC_TIMESTAMP_MASK         35

#define FVR_REG_INTR_STATUS                256
#define FVR_REG_INTR_MASK                  257
#define FVR_REG_REC_DBM_ERROR_STATUS       280
#define FVR_REG_REC_DBM_NON_ERROR_STATUS   281
#define FVR_REG_uP_ERROR_STATUS            304
#define FVR_REG_uP_NON_ERROR_STATUS        305
#define FVR_REG_uP_NON_ERROR2_STATUS       306

//
// Interrupt
//
#define FVR_INT_ERR_MASK                   0xFFFF
#define FVR_INT_ERR_REC_DBM                (1 << 0)
#define FVR_INT_ERR_uP                     (1 << 2)

#define FVR_INT_STATUS_MASK                0xFFFF0000
#define FVR_INT_STATUS_REC_DBM             (1 << 16)
#define FVR_INT_STATUS_uP                  (1 << 18)


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define GET_BYTE(arg, n)                ((UINT8*)&(arg))[n]

///
/// FVR register access commands
///
#define FVR_READ8(offset)			    IO_READ8(FVR_REG_BASE, (offset))
#define FVR_READ16(offset)		    	IO_READ16(FVR_REG_BASE, (offset))
#define FVR_READ32(offset)		    	IO_READ32(FVR_REG_BASE, ((offset) * 4))
#define FVR_WRITE8(offset, value)	    IO_WRITE8(FVR_REG_BASE, (offset), (value))
#define FVR_WRITE16(offset, value)	    IO_WRITE16(FVR_REG_BASE, (offset), (value))
#define FVR_WRITE32(offset, value)  	IO_WRITE32(FVR_REG_BASE, ((offset) * 4), (value))
#define FVR_REG32(offset)			    FVR_READ32(offset)

///
/// FVR PID data structure access commands
///
#define FVR_GBL_PID_W(word)             ((volatile UINT32*)(FVR_GBL_PID_BASE))[(word)]

#define FVR_PER_PID_W(word)             ((volatile UINT32*)(FVR_PER_PID_BASE))[(word)]
#define FVR_PER_PID_S(pidx)             FVR_PER_PID_W(FVR_PID_SIZE * (pidx))
#define FVR_PER_PID_S_W(pidx, word)     FVR_PER_PID_W((FVR_PID_SIZE * (pidx)) + (word))

/// PID index table access command
#define FVR_PID_INDEX_TABLE(pidx)       ((volatile UINT32*)(FVR_PID_TABLE_BASE))[(pidx)]

#define DATASIZE(rp, wp, size)	    \
    (((rp) <= (wp)) ? ((wp) - (rp)) : (((wp) + (size)) - (rp)))

//-----------------------------------------------------------------------------
// Type def
//-----------------------------------------------------------------------------
typedef struct
{
    UINT32 u4BufStart;
    UINT32 u4BufEnd;    
    UINT32 u4Rp;
    UINT32 u4Wp;    
    UINT16 u2CurPktCount;
    UINT16 u2TargetPktCount;
    UINT32 u4LongPktCount;
    UINT32 u4PktRp;
    
    BOOL fgAllocateBuffer;
    UINT32 u4BufSize;

    PFN_FVR_GBL_PKTCOUNT pfnPktCountNotify;

    BOOL fgEnableTimeNotify;
    PFN_FVR_TIME_NOTIFY pfnTimeNotify;
    UINT32 u4TimeToNotify;

    // Encryption CBC initial vector
    UINT32 u4CBCIVLow;
    UINT32 u4CBCIVHigh;
    
} FVR_GBL_STRUCT_T;

typedef struct
{
    BOOL fgEnable;
    //UINT8 u1TsIndex;    
    UINT16 u2Pid;
    
    BOOL fgPktSwap;    
    PFN_FVR_PID_PKTSWAP pfnSwapNotify;
    BOOL fgPicSearch;
    UINT8 u1PicSearchMask;
    PFN_FVR_PID_PICSEARCH pfnPicSearchNotify;

    BOOL fgScramble;
    BOOL fg3DES;
    BOOL fgCBCMode;

    FVR_KEY_TYPE_T rKeyType;

    UINT32 u4OddKey1Low;
    UINT32 u4OddKey1High;
    UINT32 u4OddKey2Low;
    UINT32 u4OddKey2High;
    UINT32 u4EvenKey1Low;
    UINT32 u4EvenKey1High;
    UINT32 u4EvenKey2Low;
    UINT32 u4EvenKey2High;

    // 
    BOOL fgOddKey;
    
} FVR_PID_STRUCT_T;

typedef enum
{
    FVR_MSG_PKTCOUNT,
    FVR_MSG_PKTSWAP,
    FVR_MSG_PICSEARCH,
} FVR_MSG_TYPE_T;

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//
// fvr_init.c
//
EXTERN BOOL _FVR_Diag(void);
EXTERN BOOL _FVR_Init(void);

EXTERN BOOL _FVR_Start(void);
EXTERN BOOL _FVR_Stop(void);
EXTERN BOOL _FVR_Reset(void);

//
// fvr_isr.c
//
EXTERN void _FVR_InitISR(void);

//
// fvr_api.c
//

EXTERN void _FVR_Lock(void);
EXTERN void _FVR_Unlock(void);

EXTERN void _FVR_InitAPI(void);

EXTERN BOOL _FVR_SetGBL(UINT32 u4Flags, const FVR_GBL_T* prGBL);
EXTERN BOOL _FVR_GetGBL(UINT32 u4Flags, FVR_GBL_T *prGBL);
EXTERN FVR_GBL_STRUCT_T* _FVR_GetGBLStruct(void);

EXTERN BOOL _FVR_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const FVR_PID_T* prPid);
EXTERN BOOL _FVR_GetPid(UINT8 u1Pidx, UINT32 u4Flags, FVR_PID_T *prPid);
EXTERN FVR_PID_STRUCT_T* _FVR_GetPidStruct(UINT8 u1Pidx);
EXTERN BOOL _FVR_FreePid(UINT8 u1Pidx);

EXTERN void _FVR_SetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp);
EXTERN void _FVR_GetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp);

EXTERN BOOL _FVR_FlushBuffer(void);
EXTERN BOOL _FVR_GetBuffer(UINT32 u4Addr, UINT32 u4Size, UINT8 *pu1Buf, UINT32 *pu4RetAddr);
EXTERN BOOL _FVR_GetWritePointer(UINT32 u4Addr, UINT32 u4Size, 
                                 UINT32 *pu4RetAddr, UINT32 *pu4RetSize);
EXTERN BOOL _FVR_UnlockBuffer(UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL _FVR_FreeBuffer(void);

EXTERN UINT32 _FVR_GetDataSize(void);
EXTERN void _FVR_QueryStatus(FVR_QUERY_INFO_T *prInfo);

EXTERN void _FVR_SetByPass(BOOL fgEnable, UINT8 u1Pidx, UINT16 u2CtrlBits);

#endif	// _FVR_H

