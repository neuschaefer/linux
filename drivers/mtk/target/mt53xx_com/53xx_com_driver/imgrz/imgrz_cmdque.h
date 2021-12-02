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
 * $RCSfile: imgrz_cmdque.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_cmdque.h
 *  image resizer command queue function prototypes.
 */

#ifndef IMGRZ_CMDQUE_H
#define IMGRZ_CMDQUE_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

// command queue info
//  que capacity, que size, is idle, que top, next que (if multiple),
//  previous index, read index, write index.
//  basically, it is a software maintained hardware que information
typedef struct _IMGRZ_CMDQUE_T
{
    INT32 i4QueConfig;
    INT32 i4QueCapacity;
    INT32 i4QueSize;
    INT32 i4PrevIndex;
    INT32 i4ReadIndex;
    INT32 i4WriteIndex;
    INT32 i4QueFlushCount;
    INT32 i4QueIntCount;
    UINT8 *pu1PrevAddr;
    UINT8 *pu1ReadAddr;
    UINT8 *pu1WriteAddr;
    volatile UINT64 *pu8QueTop;
    struct _IMGRZ_CMDQUE_T *prNext;
} IMGRZ_CMDQUE_T;


// cmdque capacity
//  each command occupys 8 bytes,
//  there are always even commands in queue 
//  when engine start
// ------------------------------
//   256 * 8 =   2048 =   2 KB
//   512 * 8 =   4096 =   4 KB
//  1024 * 8 =   8192 =   8 KB
//  2048 * 8 =  16384 =  16 KB
//  4096 * 8 =  32768 =  32 KB
//  8192 * 8 =  65536 =  64 KB
// 16384 * 8 = 131072 = 128 KB
// 32768 * 8 = 262144 = 256 KB
// ------------------------------
enum EIMGRZ_CMDQ_CAPACITY_T
{
    E_IMGRZ_CMDQ_CAP_2KB   =   256,
    E_IMGRZ_CMDQ_CAP_4KB   =   512,
    E_IMGRZ_CMDQ_CAP_8KB   =  1024,
    E_IMGRZ_CMDQ_CAP_16KB  =  2048,
    E_IMGRZ_CMDQ_CAP_32KB  =  4096,
    E_IMGRZ_CMDQ_CAP_64KB  =  8192,
    E_IMGRZ_CMDQ_CAP_128KB = 16384,
    E_IMGRZ_CMDQ_CAP_256KB = 32768,
    E_IMGRZ_CMDQ_CAP_MAX   = 99999
};

// cmdque size configuration
enum EIMGRZ_CMDQ_SIZE_CONFIG_T
{
    E_IMGRZ_CMDQ_CFG_2KB = 0,
    E_IMGRZ_CMDQ_CFG_4KB,
    E_IMGRZ_CMDQ_CFG_8KB,
    E_IMGRZ_CMDQ_CFG_16KB,
    E_IMGRZ_CMDQ_CFG_32KB,
    E_IMGRZ_CMDQ_CFG_64KB,
    E_IMGRZ_CMDQ_CFG_128KB,
    E_IMGRZ_CMDQ_CFG_256KB,
    E_IMGRZ_CMDQ_CFG_MAX
};


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#if defined(IMGRZ_DEBUG_MODE)

    #define IMGRZ_CMDQUE_INC_FLUSH_COUNT()    IMGRZ_CmdQueFlushCount()
    #define IMGRZ_CMDQUE_INC_INT_COUNT()      IMGRZ_CmdQueIntCount()
    
#else       // normal mode

    #define IMGRZ_CMDQUE_INC_FLUSH_COUNT()
    #define IMGRZ_CMDQUE_INC_INT_COUNT()

#endif // #if defined(IMGRZ_DEBUG_MODE)


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern void IMGRZ_CmdQueInit(void);

extern void IMGRZ_CmdQueReset(void);

extern void IMGRZ_CmdQueAction(void);

extern void IMGRZ_RiscPushBack(UINT32 u4Reg, UINT32 u4Val);

extern void IMGRZ_CmdQuePushBack(UINT32 u4Reg, UINT32 u4Val);

extern void IMGRZ_CmdQueCalculateCapacity(INT32 i4CmdqConfig);

extern void IMGRZ_CmdQueSetup(void);



// for debug use
#if defined(IMGRZ_DEBUG_MODE)
extern void IMGRZ_CmdQueDbgInfo(void);

extern void IMGRZ_CmdQueSetNewConfig(INT32 i4NewConfig);

extern void IMGRZ_CmdQueFlushCount(void);

extern void IMGRZ_CmdQueIntCount(void);
#endif // #if defined(IMGRZ_DEBUG_MODE)


#endif // IMGRZ_CMDQUE_H


