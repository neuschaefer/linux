/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
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

    #define IMGRZ_CMDQUE_INC_FLUSH_COUNT(u4Which)    IMGRZ_CmdQueFlushCount(u4Which)
    #define IMGRZ_CMDQUE_INC_INT_COUNT(u4Which)      IMGRZ_CmdQueIntCount(u4Which)
    
#else       // normal mode

    #define IMGRZ_CMDQUE_INC_FLUSH_COUNT(u4Which)
    #define IMGRZ_CMDQUE_INC_INT_COUNT(u4Which)

#endif // #if defined(IMGRZ_DEBUG_MODE)


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern void IMGRZ_CmdQueInit(UINT32 u4Which);

extern void IMGRZ_CmdQueReset(UINT32 u4Which);

extern void IMGRZ_CmdQueAction(UINT32 u4Which);

extern void IMGRZ_RiscPushBack(UINT32 u4Which, UINT32 u4Reg, UINT32 u4Val);

extern void IMGRZ_CmdQuePushBack(UINT32 u4Which, UINT32 u4Reg, UINT32 u4Val);

extern void IMGRZ_CmdQueCalculateCapacity(INT32 i4CmdqConfig);

extern void IMGRZ_CmdQueSetup(UINT32 u4Which);



// for debug use
#if defined(IMGRZ_DEBUG_MODE)
extern void IMGRZ_CmdQueDbgInfo(UINT32 u4Which);

extern void IMGRZ_CmdQueSetNewConfig(UINT32 u4Which, INT32 i4NewConfig);

extern void IMGRZ_CmdQueFlushCount(UINT32 u4Which);

extern void IMGRZ_CmdQueIntCount(UINT32 u4Which);
#endif // #if defined(IMGRZ_DEBUG_MODE)


#endif // IMGRZ_CMDQUE_H


