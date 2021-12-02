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
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tz_if.h
 *  Interface of trustzone driver
 */


#ifndef TZ_IF_H
#define TZ_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------
EXTERN BOOL TZ_Init(void);
EXTERN BOOL TZ_IsTrustzoneEnable(void);

EXTERN void TZ_InvaliateICache(void);
EXTERN void TZ_InvaliateDCache(void);
EXTERN void TZ_EnableL2Cache(void);
EXTERN void TZ_FlushL2Cache(void);

EXTERN void TZ_DMX_SetDesIV(UINT32 u4IvHi, UINT32 u4IvLo);
EXTERN void TZ_DMX_SetDesIVEx(UINT32 u4IvHi, UINT32 u4IvLo, BOOL fgEven);
EXTERN BOOL TZ_DMX_SetDesKey(UINT8 u1Index, BOOL fg3Des, BOOL fgEven,
    const UINT32 au4Key[6]); 
EXTERN void TZ_DMX_SetAesIVEx(const UINT32 au4Iv[4], BOOL fgEven);
EXTERN void TZ_DMX_SetAesIV(const UINT32 au4Iv[4]); 
EXTERN BOOL TZ_DMX_SetAesKeyLen(UINT32 u4KeyLen); 
EXTERN BOOL TZ_DMX_SetAesKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[8]); 
EXTERN void TZ_DMX_SetDmemAesIV(const UINT32 au4Iv[4]); 
EXTERN BOOL TZ_DMX_SetMulti2Key(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[2]); 
EXTERN void TZ_DMX_SetMulti2CbcIV(UINT32 u4IvHi, UINT32 u4IvLo);
EXTERN void TZ_DMX_SetMulti2OfbIV(UINT32 u4IvHi, UINT32 u4IvLo);  
EXTERN void TZ_DMX_SetMulti2SysKey(const UINT32 au4Key[8]); 
EXTERN void TZ_DMX_SetMulti2Config(BOOL fgOfbFreeRun, UINT16 u2Iteration);

EXTERN void TZ_DMX_SetCpuDescKeyIv(const UINT32 au4Key[8], const UINT32 au4Iv[4]);
EXTERN void TZ_DMX_SetCpuDescIntrClear(void);

EXTERN BOOL TZ_DRM_WV_SetAssetKey(UINT8 *pu1Key, UINT32 u4KeyLen);
EXTERN BOOL TZ_DRM_WV_DeriveCW(UINT8 *pu1Ecm, UINT32 *pu4Flags);
EXTERN BOOL TZ_DRM_WV_PassKeybox(UINT8 *pu1Keybox, UINT32 u4Size);
EXTERN BOOL TZ_BIM_GetRomBytes(UINT32 u4Offset, UINT8 *pu1Buf, UINT32 u4Size);

EXTERN BOOL TZ_BIM_GetRomBytes(UINT32 u4Offset, UINT8 *pu1Buf, UINT32 u4Size);

EXTERN BOOL TZ_CTL(UINT32 u4Cmd, VOID *pvArg2, VOID *pvArg3, VOID *pvArg4, VOID *pvArg5, VOID *pvArg6, VOID *pvArg7);
EXTERN void TZ_RunUT(UINT32 u4Cmd1, UINT32 u4Cmd2, UINT32 u4Cmd3);
EXTERN void TZ_EnableDebug(BOOL fgEnable);

#endif  // TZ_IF_H

