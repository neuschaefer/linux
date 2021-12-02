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

EXTERN BOOL TZ_CTL(UINT32 u4Cmd, VOID *pvArg2, VOID *pvArg3, VOID *pvArg4, VOID *pvArg5, VOID *pvArg6, VOID *pvArg7);
EXTERN void TZ_RunUT(UINT32 u4Cmd1, UINT32 u4Cmd2, UINT32 u4Cmd3);
EXTERN void TZ_EnableDebug(BOOL fgEnable);
EXTERN BOOL TZ_IO_WRITE32(UINT32 u4Base, UINT32 u4Offset, UINT32 u4Val);
EXTERN BOOL TZ_IO_WRITE16(UINT32 u4Base, UINT32 u4Offset, UINT16 u2Val);
EXTERN BOOL TZ_IO_WRITE8(UINT32 u4Base, UINT32 u4Offset, UINT8 u1Val);
EXTERN BOOL TZ_IO_READ32(UINT32 u4Base, UINT32 u4Offset, UINT32 *pu4Val);
EXTERN BOOL TZ_IO_READ16(UINT32 u4Base, UINT32 u4Offset, UINT16 *pu2Val);
EXTERN BOOL TZ_IO_READ8(UINT32 u4Base, UINT32 u4Offset, UINT8 *pu1Val);

EXTERN BOOL TZ_DRM_WV_SetAssetKey(UINT8 *pu1Key, UINT32 u4KeyLen);
EXTERN BOOL TZ_DRM_WV_DeriveCW(UINT8 *pu1Ecm, UINT32 *pu4Flags);
EXTERN BOOL TZ_DRM_WV_PassKeybox(UINT8 *pu1Keybox, UINT32 u4Size);
EXTERN BOOL TZ_HCI_Init(void *prKernParam);
EXTERN BOOL TZ_HCI_Fin(void);
EXTERN BOOL TZ_HCI_KeyRefCreate(void *prKernParam);
EXTERN BOOL TZ_HCI_KeyRefCheck(void *prKernParam);
EXTERN BOOL TZ_HCI_KeyRefDestroy(void *prKernParam);
EXTERN BOOL TZ_HCI_DevIdsGetNum(void *prKernParam);
EXTERN BOOL TZ_HCI_DevIdsGetSize(void *prKernParam);
EXTERN BOOL TZ_HCI_DevIdsGetName(void *prKernParam);
EXTERN BOOL TZ_HCI_ImportKey(void *prKernParam);
EXTERN BOOL TZ_HCI_ExportKeySize(void *prKernParam);
EXTERN BOOL TZ_HCI_ExportKey(void *prKernParam);
EXTERN BOOL TZ_HCI_StarfishIdGetSize(void *prKernParam);
EXTERN BOOL TZ_HCI_StarfishIdGet(void *prKernParam);
EXTERN BOOL TZ_HCI_KeyDataGetIfStarfishSize(void *prKernParam);
EXTERN BOOL TZ_HCI_KeyDataGetIfStarfish(void *prKernParam);
EXTERN BOOL TZ_HCI_ImportStarfishKey(void *prKernParam);
EXTERN BOOL TZ_HCI_DataSignSize(void *prKernParam);
EXTERN BOOL TZ_HCI_DataSign(void *prKernParam);
EXTERN BOOL TZ_HCI_DataDecryptSize(void *prKernParam);
EXTERN BOOL TZ_HCI_DataDecrypt(void *prKernParam);
EXTERN BOOL TZ_HCI_StreamDecryptSize(void *prKernParam);
EXTERN BOOL TZ_HCI_StreamDecrypt(void *prKernParam);
EXTERN BOOL TZ_HCI_KeyDataEncryptSize(void *prKernParam);
EXTERN BOOL TZ_HCI_KeyDataEncrypt(void *prKernParam);
EXTERN BOOL TZ_HCI_HmacCheck(void *prKernParam);
EXTERN BOOL TZ_HCI_HashCheck(void *prKernParam);
EXTERN BOOL TZ_HCI_MoveKeySize(void *prKernParam);
EXTERN BOOL TZ_HCI_MoveKey(void *prKernParam);
EXTERN BOOL TZ_HCI_PersonalityKeyLoad(void *prKernParam);
EXTERN BOOL TZ_HCI_PersonalityKeyCheck(void *prKernParam);
EXTERN BOOL TZ_HCI_ImportSasSize(void *prKernParam);
EXTERN BOOL TZ_HCI_ImportSas(void *prKernParam);
EXTERN BOOL TZ_HCI_ImportSasIds(void *prKernParam);
EXTERN BOOL TZ_SKB_GetInstance(void *prKernParam);
EXTERN BOOL TZ_SKB_Release(void *prKernParam);
EXTERN BOOL TZ_SKB_GetInfo(void *prKernParam);
EXTERN BOOL TZ_SKB_GetProertiesSize(void *prKernParam);
EXTERN BOOL TZ_SKB_GetProertiesData(void *prKernParam);
EXTERN BOOL TZ_SKB_DataFromWrapped(void *prKernParam);
EXTERN BOOL TZ_SKB_DataFromExported(void *prKernParam);
EXTERN BOOL TZ_SKB_CreateTransform(void *prKernParam);
EXTERN BOOL TZ_SKB_CreateCipher(void *prKernParam);
EXTERN BOOL TZ_SKB_SecureData_Release(void *prKernParam);
EXTERN BOOL TZ_SKB_SecureDataGetInfo(void *prKernParam);
EXTERN BOOL TZ_SKB_SecureData_Export(void *prKernParam);
EXTERN BOOL TZ_SKB_SecureData_Derive(void *prKernParam);
EXTERN BOOL TZ_SKB_Transform_Release(void *prKernParam);
EXTERN BOOL TZ_SKB_Transform_AddBytes(void *prKernParam);
EXTERN BOOL TZ_SKB_Transform_AddSecureData(void *prKernParam);
EXTERN BOOL TZ_SKB_Transform_GetOutput(void *prKernParam);
EXTERN BOOL TZ_SKB_Cipher_ProcessBuffer(void *prKernParam);
EXTERN BOOL TZ_SKB_Cipher_Release(void *prKernParam);
EXTERN BOOL TZ_DRM_TZ_SetSecretDataType(void *prKernParam);
EXTERN BOOL TZ_DRM_TZ_Open(void *prKernParam);
EXTERN BOOL TZ_DRM_TZ_Close(void *prKernParam);
EXTERN BOOL TZ_DRM_TZ_DecodePacketsVOD(void *prKernParam);

#endif  // TZ_IF_H

