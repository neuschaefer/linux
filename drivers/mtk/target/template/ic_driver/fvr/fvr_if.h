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
 * $RCSfile: fvr_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fvr_if.h
 *  FVR driver - FVR Interface Header
 */

#ifndef _FVR_IF_H
#define _FVR_IF_H

#include "dmx_mm_if.h"

#if 0
//------------------------------------------------------------
// Scramble
//------------------------------------------------------------
#define FVR_SCRAMBLE_FLAG_ENABLE        0x00000001
#define FVR_SCRAMBLE_FLAG_ENCRYPT_TYPE  0x00000002
#define FVR_SCRAMBLE_FLAG_KEY_TYPE      0x00000004
#define FVR_SCRAMBLE_FLAG_EVEN0_KEY     0x00000008
#define FVR_SCRAMBLE_FLAG_EVEN1_KEY     0x00000010
#define FVR_SCRAMBLE_FLAG_ODD0_KEY      0x00000020
#define FVR_SCRAMBLE_FLAG_ODD1_KEY      0x00000040
#define FVR_SCRAMBLE_FLAG_NONE          0x00000000
#define FVR_SCRAMBLE_FLAG_ALL           0xFFFFFFFF

typedef enum
{
    FVR_ENCRYPT_DES,
    FVR_ENCRYPT_3DES,
} FVR_ENCRYPT_TYPE_T;

typedef struct
{
    BOOL fgEnable;
    FVR_ENCRYPT_TYPE_T rEncryptType;
    FVR_KEY_TYPE_T rKeyType;    
    UINT32 u4EvenKey[4];
    UINT32 u4OddKey[4];
} FVR_SCRAMBLE_T;
#endif

#define FVR_PIC_I_TYPE       0x1
#define FVR_PIC_P_TYPE       0x2
#define FVR_PIC_B_TYPE       0x4
#define FVR_PIC_UNKNOWN      0x8

#define FVR_PIC_I_MASK       0x1
#define FVR_PIC_P_MASK       0x2
#define FVR_PIC_B_MASK       0x4


//------------------------------------------------------------
// TimeStamp
//------------------------------------------------------------
#define FVR_TIMESTAMP_FLAG_ENABLE           0x00000001
#define FVR_TIMESTAMP_FLAG_CLKDIV           0x00000002
#define FVR_TIMESTAMP_FLAG_MASK             0x00000004
#define FVR_TIMESTAMP_FLAG_COUNTER          0x00000008
#define FVR_TIMESTAMP_FLAG_NONE             0x00000000
#define FVR_TIMESTAMP_FLAG_ALL              0xFFFFFFFF

typedef struct
{
    BOOL fgEnable;
    UINT16 u2ClkDiv;
    UINT32 u4Mask;
    UINT32 u4Counter;
} FVR_TIMESTAMP_T;

typedef enum
{
    FVR_STATE_STOP,
    FVR_STATE_RECORDING,
} FVR_STATE_T;


//------------------------------------------------------------
// Callback
//------------------------------------------------------------

typedef void (*PFN_FVR_GBL_PKTCOUNT)(UINT32 u4Addr, UINT32 u4Size, UINT32 u4PktCount);

typedef void (*PFN_FVR_PID_PKTSWAP)(UINT8 u1Pidx, UINT32 u4Addr);

typedef void (*PFN_FVR_PID_PICSEARCH)(UINT8 u1Pidx, UINT8 u1Type, UINT32 u4PktCount);

typedef void (*PFN_FVR_TIME_NOTIFY)(UINT32 u4PktCount);


//-----------------------------------------------------------------------------
// Function bitmap of PID operations
//-----------------------------------------------------------------------------
#define FVR_GBL_FLAG_BUFFER             0x00000001
#define FVR_GBL_FLAG_TARGET_PKT         0x00000004
#define FVR_GBL_FLAG_CBC_IV             0x00000010
#define FVR_GBL_FLAG_NOTIFY             0x00000020
#define FVR_GBL_FLAG_TIMESTAMP          0x00000040
#define FVR_GBL_FLAG_TIME_NOTIFY        0x00000080
#define FVR_GBL_FLAG_NONE               0x00000000
#define FVR_GBL_FLAG_ALL                0xFFFFFFFF

#define FVR_PID_FLAG_ENABLE             0x00000001
//#define FVR_PID_FLAG_TS_INDEX           0x00000002    // TS index only has two value
                                                        // 0 : Enable and input from DMX
                                                        // 0xF: Disable
#define FVR_PID_FLAG_PID                0x00000004
#define FVR_PID_FLAG_PICSEARCH          0x00000008
#define FVR_PID_FLAG_PKTSWAP            0x00000020
#define FVR_PID_FLAG_ENCRYPT_METHOD     0x00000080
#define FVR_PID_FLAG_SCRAMBLE_ENABLE    0x00000100
#define FVR_PID_FLAG_CBC_MODE           0x00000200
#define FVR_PID_FLAG_EVEN_KEY_1         0x00000400
#define FVR_PID_FLAG_EVEN_KEY_2         0x00000800
#define FVR_PID_FLAG_ODD_KEY_1          0x00001000
#define FVR_PID_FLAG_ODD_KEY_2          0x00002000
#define FVR_PID_FLAG_KEY_TYPE           0x00004000
#define FVR_PID_FLAG_NONE               0x00000000
#define FVR_PID_FLAG_ALL                0xFFFFFFFF

#define FVR_PID_FLAG_KEY                (FVR_PID_FLAG_EVEN_KEY_1 | FVR_PID_FLAG_EVEN_KEY_2 | \
                                         FVR_PID_FLAG_ODD_KEY_1 | FVR_PID_FLAG_ODD_KEY_2)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
    FVR_KEY_EVEN,
    FVR_KEY_ODD,
    FVR_KEY_BOTH,
} FVR_KEY_TYPE_T;

typedef struct
{
    UINT16 u2TargetPktCount;
    BOOL fgAllocateBuffer;
    UINT32 u4BufStart;
    UINT32 u4BufSize;

    // Encryption CBC initial vector
    UINT32 u4CBCIVLow;
    UINT32 u4CBCIVHigh;

    //UINT32 u4DBMErrors;
    //UINT32 u4uPErrors;
    //UINT32 u4TotalErrors;

    PFN_FVR_GBL_PKTCOUNT pfnPktCountNotify;

    BOOL fgEnableTimeNotify;
    PFN_FVR_TIME_NOTIFY pfnTimeNotify;
    UINT32 u4TimeToNotify;
} FVR_GBL_T;

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
} FVR_PID_T;

typedef struct
{
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    UINT32 u4Wp;
    UINT32 u4Rp;
    UINT32 u4PktCount;
} FVR_QUERY_INFO_T;


//------------------------------------------------------------
// Function
//------------------------------------------------------------
EXTERN BOOL FVR_Diag(void);

EXTERN BOOL FVR_Init(void);
EXTERN BOOL FVR_Start(void);
EXTERN BOOL FVR_Stop(void);
EXTERN FVR_STATE_T FVR_GetState(void);

EXTERN BOOL FVR_SetGBL(UINT32 u4Flags, const FVR_GBL_T* prPid);
EXTERN BOOL FVR_GetGBL(UINT32 u4Flags, FVR_GBL_T* prPid);

EXTERN BOOL FVR_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const FVR_PID_T* prPid);
EXTERN BOOL FVR_GetPid(UINT8 u1Pidx, UINT32 u4Flags, FVR_PID_T* prPid);
EXTERN BOOL FVR_FreePid(UINT8 u1Pidx);

EXTERN void FVR_SetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp);
EXTERN void FVR_GetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp);

EXTERN BOOL FVR_FlushBuffer(void);
EXTERN BOOL FVR_GetBuffer(UINT32 u4Addr, UINT32 u4Size, UINT8 *pu1Buf, UINT32 *pu4RetSize);
EXTERN BOOL FVR_GetWritePointer(UINT32 u4Addr, UINT32 u4Size, 
                                UINT32 *pu4RetAddr, UINT32 *pu4RetSize);
EXTERN BOOL FVR_UnlockBuffer(UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL FVR_FreeBuffer(void);

EXTERN UINT32 FVR_GetDataSize(void);
EXTERN void FVR_QueryStatus(FVR_QUERY_INFO_T *prInfo);

EXTERN void FVR_SetByPass(BOOL fgEnable, UINT8 u1Pidx, UINT16 u2CtrlBits);

//
// AP
//
EXTERN BOOL TS_Loopback_Init(UINT32 u4BufSize, UINT32 u4RecPktCount, UINT32 u4PlayThreshold);
EXTERN BOOL TS_Loopback_SetPid(UINT8 u1Pidx, UINT16 u2PidNum, DMX_PID_TYPE_T eType);
EXTERN BOOL TS_Loopback_Start(UINT32 u4Distance);
EXTERN BOOL TS_Loopback_Test(UINT32 u4BufSize);
EXTERN UINT32 TS_Loopback_GetDataSize(void);

EXTERN BOOL TSHIFT_Init(void);
EXTERN BOOL TSHIFT_SetPid(UINT8 u1Pidx, UINT16 u2PidNum, DMX_PID_TYPE_T eType);
EXTERN BOOL TSHIFT_PVRStart(void);
EXTERN BOOL TSHIFT_PVRStop(void);

EXTERN BOOL TShift_Buf_Init(UINT32 u4BufSize);
EXTERN BOOL TShift_Buf_Write(UINT32 u4Addr, UINT32 u4Size, UINT32* pu4RetSize);
EXTERN BOOL TShift_Buf_Read(UINT32 u4Addr, UINT32 u4Size, UINT32 *pu4RetSize);

#endif	// FVR_IF_H

