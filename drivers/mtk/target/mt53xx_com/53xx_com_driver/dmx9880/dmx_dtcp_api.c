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
 * $RCSfile: dmx_dtcp_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_mm_api.c
 *  Demux program stream driver
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dmx_mm.h"
#include "dmx_debug.h"
#include "dmx_drm_drvif.h"

#include "gcpu_if.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"

#ifndef CC_MTK_LOADER
#include "aes.h"
#endif
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
// Static variables
//-----------------------------------------------------------------------------
static AES_PARAM_T _arAesStruct[AES_MAX_ENGINE_INDEX];
static BOOL _afgAesUsed[AES_MAX_ENGINE_INDEX];
static GCPU_STRUCT_T *prGcpuMtd = NULL;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** _DMX_GetAesStruct
*/
//-----------------------------------------------------------------------------
AES_PARAM_T* _DMX_GetAesStruct(UINT8 u1Idx)
{
    ASSERT(u1Idx < AES_MAX_ENGINE_INDEX);

    return &_arAesStruct[u1Idx];
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_DTCP_SetAES
*/
//-----------------------------------------------------------------------------
BOOL _DMX_DTCP_SetAES(UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    return _DMX_AES_SetDecryptInfo(u4Flags, prDRM);
}


//-----------------------------------------------------------------------------
/** _DMX_DTCP_MoveData
*/
//-----------------------------------------------------------------------------
BOOL _DMX_DTCP_MoveData(const DMX_DTCP_DATA_T *prData)
{
    return _DMX_AES_MoveData((DMX_AES_DATA_T *)prData);
}


//-----------------------------------------------------------------------------
/** _DMX_DTCP_MoveBlockData
*/
//-----------------------------------------------------------------------------
BOOL _DMX_DTCP_MoveBlockData(const DMX_DTCP_BLKDATA_T *prBlkData)
{
    return _DMX_AES_MoveBlockData((DMX_AES_BLKDATA_T *)prBlkData);
}


//-----------------------------------------------------------------------------
/** _DMX_AES_SetDecryptInfo
*   Use DTCP as general AES decryption engine
*/
//-----------------------------------------------------------------------------
BOOL _DMX_AES_SetDecryptInfo(UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    AES_PARAM_T *prParam;

    if(prDRM == NULL)
    {
        return FALSE;
    }

    if (u4Flags == DMX_MM_DRM_FLAG_NONE)
    {
        return TRUE;
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_OFFSET) != 0)
    {
        if(prDRM->u4Offset > 0)     // Parser hardware limitation
        {
            ASSERT(0);
        }
    }

    prParam = _DMX_GetAesStruct(AES_LEGACY_INDEX);

    if ((u4Flags & DMX_MM_DRM_FLAG_CBC) != 0)
    {
        prParam->eMode = (prDRM->fgCBC)?BLOCK_CIPHER_MODE_CBC:BLOCK_CIPHER_MODE_ECB;
        prParam->fgEncrypt = prDRM->fgEncrypt;
        x_memcpy(prParam->au1Iv, prDRM->au1IV, 16);
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_KEY) != 0)
    {
        prParam->eKeyBitLen = (KEY_BIT_LEN)(prDRM->u4KeyBitLen & 0xFFFF);
        x_memcpy(prParam->au1Key, prDRM->au1Key, 32);
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_ENCRYPT) != 0)
    {
        prParam->fgEncrypt = prDRM->fgEncrypt;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_AES_MoveData
*   Use DTCP as general AES decryption engine
*/
//-----------------------------------------------------------------------------
BOOL _DMX_AES_MoveData(const DMX_AES_DATA_T *prData)
{
    AES_PARAM_T *prParam;

    if(prData == NULL)
    {
        return FALSE;
    }

    prParam = _DMX_GetAesStruct(AES_LEGACY_INDEX);

    prParam->u4SrcStartAddr = prData->u4SrcStart;
    prParam->u4SrcBufStart = prData->u4SrcBufStart;
    prParam->u4SrcBufEnd = prData->u4SrcBufEnd;
    prParam->u4DstStartAddr = prData->u4DstStart;
    prParam->u4DstBufStart = prData->u4DstBufStart;
    prParam->u4DstBufEnd = prData->u4DstBufEnd;
    prParam->u4DatLen = prData->u4Size;

    return (GCPU_Cmd(0, GCPU_AES, prParam) == S_GCPU_OK)?TRUE:FALSE;
}


//-----------------------------------------------------------------------------
/** _DMX_AES_MoveBlockData
*   Use DTCP as general AES decryption engine
*/
//-----------------------------------------------------------------------------
BOOL _DMX_AES_MoveBlockData(const DMX_AES_BLKDATA_T *prBlkData)
{
    AES_PARAM_T *prParam;

    if(prBlkData == NULL)
    {
        return FALSE;
    }

    prParam = _DMX_GetAesStruct(AES_LEGACY_INDEX);

    prParam->u4SrcStartAddr = prBlkData->u4SrcAddr;
    prParam->u4SrcBufStart = GCPU_LINER_BUFFER_START(prBlkData->u4SrcAddr);
    prParam->u4SrcBufEnd = GCPU_LINER_BUFFER_END(prBlkData->u4SrcAddr + prBlkData->u4Size);
    prParam->u4DstStartAddr = prBlkData->u4DstAddr;
    prParam->u4DstBufStart = GCPU_LINER_BUFFER_START(prBlkData->u4DstAddr);
    prParam->u4DstBufEnd = GCPU_LINER_BUFFER_END(prBlkData->u4DstAddr + prBlkData->u4Size);
    prParam->u4DatLen = prBlkData->u4Size;

    return (GCPU_Cmd(0, GCPU_AES, prParam) == S_GCPU_OK)?TRUE:FALSE;
}


//-----------------------------------------------------------------------------
/** _DMX_AES_AllocEngine
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_AES_AllocEngine()
{
    UINT8 i;

    for(i = AES_RESERVED_ENGINE_NUM; i < AES_MAX_ENGINE_INDEX; i++)
    {
        if(!_afgAesUsed[i])
        {
            break;
        }
    }

    if(i >= AES_MAX_ENGINE_INDEX)
    {
        return AES_NULL_ENGINE_INDEX;
    }

    _afgAesUsed[i] = TRUE;

    return i;
}


//-----------------------------------------------------------------------------
/** _DMX_AES_FreeEngine
*/
//-----------------------------------------------------------------------------
BOOL _DMX_AES_FreeEngine(UINT8 u1Idx)
{
    if(u1Idx < AES_RESERVED_ENGINE_NUM || u1Idx >= AES_MAX_ENGINE_INDEX)
    {
        return FALSE;
    }

    _afgAesUsed[u1Idx] = FALSE;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_AES_SetDecryptInfo
*   Use DTCP as general AES decryption engine
*/
//-----------------------------------------------------------------------------
BOOL _DMX_AES_SetDecryptInfoEx(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    if (prDRM == NULL)
    {
        return FALSE;
    }

    if (u4Flags == DMX_MM_DRM_FLAG_NONE)
    {
        return TRUE;
    }

    if (u1Idx >= AES_MAX_ENGINE_INDEX)
    {
        return FALSE;
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_OFFSET) != 0)
    {
        if(prDRM->u4Offset > 0)     // Parser hardware limitation
        {
            ASSERT(0);
        }
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_CBC) != 0)
    {
        _arAesStruct[u1Idx].eMode = (prDRM->fgCBC)?BLOCK_CIPHER_MODE_CBC:BLOCK_CIPHER_MODE_ECB;
        x_memcpy(_arAesStruct[u1Idx].au1Iv, prDRM->au1IV, 16);
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_KEY) != 0)
    {
        // Always set to Key 0
        _arAesStruct[u1Idx].eKeyBitLen = (KEY_BIT_LEN)(prDRM->u4KeyBitLen & 0xFFFF);
        x_memcpy(_arAesStruct[u1Idx].au1Key, prDRM->au1Key, 32);
    }

    if ((u4Flags & DMX_MM_DRM_FLAG_ENCRYPT) != 0)
    {
        _arAesStruct[u1Idx].fgEncrypt = prDRM->fgEncrypt;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_AES_MoveDataEx
*   Use DTCP as general AES decryption engine
*/
//-----------------------------------------------------------------------------
BOOL _DMX_AES_MoveDataEx(UINT8 u1Idx, const DMX_AES_DATA_T *prData)
{
    BOOL fgRet;

    if(prData == NULL)
    {
        return FALSE;
    }

    if (u1Idx >= AES_MAX_ENGINE_INDEX)
    {
        return FALSE;
    }

    _arAesStruct[u1Idx].u4SrcStartAddr = prData->u4SrcStart;
    _arAesStruct[u1Idx].u4SrcBufStart = prData->u4SrcBufStart;
    _arAesStruct[u1Idx].u4SrcBufEnd = prData->u4SrcBufEnd;
    _arAesStruct[u1Idx].u4DstStartAddr = prData->u4DstStart;
    _arAesStruct[u1Idx].u4DstBufStart = prData->u4DstBufStart;
    _arAesStruct[u1Idx].u4DstBufEnd = prData->u4DstBufEnd;
    _arAesStruct[u1Idx].u4DatLen = prData->u4Size;

    fgRet = (GCPU_Cmd(0, GCPU_AES, &_arAesStruct[u1Idx]) == S_GCPU_OK)?TRUE:FALSE;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_AES_MoveBlockDataEx
*   Use DTCP as general AES decryption engine
*/
//-----------------------------------------------------------------------------
BOOL _DMX_AES_MoveBlockDataEx(UINT8 u1Idx, const DMX_AES_BLKDATA_T *prBlkData)
{
    BOOL fgRet;

    if(prBlkData == NULL)
    {
        return FALSE;
    }

    if (u1Idx >= AES_MAX_ENGINE_INDEX)
    {
        return FALSE;
    }

    _arAesStruct[u1Idx].u4SrcStartAddr = prBlkData->u4SrcAddr;
    _arAesStruct[u1Idx].u4SrcBufStart = GCPU_LINER_BUFFER_START(prBlkData->u4SrcAddr);
    _arAesStruct[u1Idx].u4SrcBufEnd = GCPU_LINER_BUFFER_END(prBlkData->u4SrcAddr + prBlkData->u4Size);
    _arAesStruct[u1Idx].u4DstStartAddr = prBlkData->u4DstAddr;
    _arAesStruct[u1Idx].u4DstBufStart = GCPU_LINER_BUFFER_START(prBlkData->u4DstAddr);
    _arAesStruct[u1Idx].u4DstBufEnd = GCPU_LINER_BUFFER_END(prBlkData->u4DstAddr + prBlkData->u4Size);
    _arAesStruct[u1Idx].u4DatLen = prBlkData->u4Size;

    fgRet = (GCPU_Cmd(0, GCPU_AES, &_arAesStruct[u1Idx]) == S_GCPU_OK)?TRUE:FALSE;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_Marlin_SetDecryptInfo
*/
//-----------------------------------------------------------------------------
BOOL _DMX_Marlin_SetDecryptInfo(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    return _DMX_AES_SetDecryptInfoEx(u1Idx, u4Flags, prDRM);
}


//-----------------------------------------------------------------------------
/** _DMX_Marlin_MoveData
*/
//-----------------------------------------------------------------------------
BOOL _DMX_Marlin_MoveData(UINT8 u1Idx, const DMX_DTCP_DATA_T *prData)
{
    BOOL fgRet = FALSE;
#ifndef CC_MTK_LOADER
    UINT32 u4ResidualLen, u4TmpAddr;
    UINT8 au1ResidualIv[AES_BLOCK_LEN], au1ResidualInput[AES_BLOCK_LEN], au1ResidualOutput[AES_BLOCK_LEN];

    if(prData == NULL)
    {
        return FALSE;
    }

    if (u1Idx >= AES_MAX_ENGINE_INDEX)
    {
        return FALSE;
    }

    u4ResidualLen = (prData->u4Size) % AES_BLOCK_LEN;

    if(u4ResidualLen > 0)
    {
        // prepare residual iv for decryption
        if(_arAesStruct[u1Idx].fgEncrypt == FALSE)
        {
            u4TmpAddr = ADDR_INCR_IN_RING(prData->u4SrcStart,
                prData->u4Size - AES_BLOCK_LEN - u4ResidualLen,
                prData->u4SrcBufStart, prData->u4SrcBufEnd);
            UNUSED(_DMX_CopyRingBuffer((UINT32)au1ResidualIv, (UINT32)au1ResidualIv,
                (UINT32)(au1ResidualIv + AES_BLOCK_LEN), u4TmpAddr, prData->u4SrcBufStart,
                prData->u4SrcBufEnd, AES_BLOCK_LEN));
        }

        // no matter encrypt or decrypt, we need to save residual source input
        u4TmpAddr = ADDR_INCR_IN_RING(prData->u4SrcStart,
            prData->u4Size - u4ResidualLen,
            prData->u4SrcBufStart, prData->u4SrcBufEnd);
        UNUSED(_DMX_CopyRingBuffer((UINT32)au1ResidualInput, (UINT32)au1ResidualInput,
            (UINT32)(au1ResidualInput + AES_BLOCK_LEN), u4TmpAddr, prData->u4SrcBufStart,
            prData->u4SrcBufEnd, u4ResidualLen));
    }

    fgRet = _DMX_AES_MoveDataEx(u1Idx, (DMX_AES_DATA_T *)prData);
    if(fgRet != TRUE)
    {
        LOG(5, "%s: fail to move marlin data\n", __FUNCTION__);
        return fgRet;
    }

    // prepare residual iv for encryption
    if((u4ResidualLen > 0) && (_arAesStruct[u1Idx].fgEncrypt == TRUE))
    {
        u4TmpAddr = ADDR_INCR_IN_RING(prData->u4DstStart,
            prData->u4Size - AES_BLOCK_LEN - u4ResidualLen,
            prData->u4DstBufStart, prData->u4DstBufEnd);
        UNUSED(_DMX_CopyRingBuffer((UINT32)au1ResidualIv, (UINT32)au1ResidualIv,
            (UINT32)(au1ResidualIv + AES_BLOCK_LEN), u4TmpAddr, prData->u4DstBufStart,
            prData->u4DstBufEnd, AES_BLOCK_LEN));
    }

    // residual termination block processing...
    if((u4ResidualLen > 0) && (_arAesStruct[u1Idx].fgEncrypt == FALSE))
    {
        aes_context rCtx;
        if(_arAesStruct[u1Idx].fgEncrypt == FALSE)
        {
            aes_setkey_dec(&rCtx, _arAesStruct[u1Idx].au1Key, _arAesStruct[u1Idx].eKeyBitLen);
        }
        else
        {
            aes_setkey_enc(&rCtx, _arAesStruct[u1Idx].au1Key, _arAesStruct[u1Idx].eKeyBitLen);
        }
        aes_crypt_cbc_scte52(&rCtx, AES_ENCRYPT, AES_BLOCK_LEN, au1ResidualIv,
            au1ResidualInput, au1ResidualOutput);

        u4TmpAddr = ADDR_INCR_IN_RING(prData->u4DstStart,
            prData->u4Size - u4ResidualLen,
            prData->u4DstBufStart, prData->u4DstBufEnd);
        UNUSED(_DMX_CopyRingBuffer(u4TmpAddr, prData->u4DstBufStart,
            prData->u4DstBufEnd, (UINT32)au1ResidualOutput, (UINT32)au1ResidualOutput,
            (UINT32)(au1ResidualOutput + AES_BLOCK_LEN), u4ResidualLen));
    }
#endif
    return fgRet;
}


#if 0
BOOL _DMX_NAND_AES_INIT(UINT8 *pu1Key, UINT16 u2KeyLen)
{
    // only use AES 128 bit key
    if(u2KeyLen > 128)
    {
        return FALSE;
    }

    GCPU_Init(0);
    if(prGcpuMtd == NULL)
    {
        prGcpuMtd = (GCPU_STRUCT_T *)GCPU_CreateHandle();
    }
    _arAesStruct[AES_MTD_INDEX].eMode = BLOCK_CIPHER_MODE_ECB;
    _arAesStruct[AES_MTD_INDEX].eKeyBitLen = KEY_BIT_LEN_128;
    prGcpuMtd->u4AesKeyPtr = AES_MTD_SECURE_KEY_PTR;

    #if defined(CC_MTK_LOADER) && !defined(CC_UBOOT)
    GCPU_SetSecureSlot(prGcpuMtd->u4AesKeyPtr, pu1Key, 16);
    #endif

    return TRUE;
}
#endif


BOOL _DMX_NAND_AES_INIT(UINT8 *pu1Key, UINT16 u2KeyLen)
{
    // only use AES 128 bit key
    if(u2KeyLen > 128)
    {
        return FALSE;
    }

    GCPU_Init(0);

    _arAesStruct[AES_MTD_INDEX].eMode = BLOCK_CIPHER_MODE_ECB;
    _arAesStruct[AES_MTD_INDEX].eKeyBitLen = KEY_BIT_LEN_128;
    x_memset(_arAesStruct[AES_MTD_INDEX].au1Key, 0, 32);

    return TRUE;
}


BOOL _DMX_NAND_AES_Encryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
    _arAesStruct[AES_MTD_INDEX].u4SrcStartAddr = u4InBufStart;
    _arAesStruct[AES_MTD_INDEX].u4SrcBufStart = GCPU_LINER_BUFFER_START(u4InBufStart);
    _arAesStruct[AES_MTD_INDEX].u4SrcBufEnd = GCPU_LINER_BUFFER_END(u4InBufStart + u4BufSize);
    _arAesStruct[AES_MTD_INDEX].u4DstStartAddr = u4OutBufStart;
    _arAesStruct[AES_MTD_INDEX].u4DstBufStart = GCPU_LINER_BUFFER_START(u4OutBufStart);
    _arAesStruct[AES_MTD_INDEX].u4DstBufEnd = GCPU_LINER_BUFFER_END(u4OutBufStart + u4BufSize);
    _arAesStruct[AES_MTD_INDEX].u4DatLen = u4BufSize;
    _arAesStruct[AES_MTD_INDEX].fgEncrypt = TRUE;

    return (GCPU_Cmd((UINT32)prGcpuMtd, GCPU_AES, &_arAesStruct[AES_MTD_INDEX]) == S_GCPU_OK)?TRUE:FALSE;
}


BOOL _DMX_NAND_AES_Decryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
    _arAesStruct[AES_MTD_INDEX].u4SrcStartAddr = u4InBufStart;
    _arAesStruct[AES_MTD_INDEX].u4SrcBufStart = GCPU_LINER_BUFFER_START(u4InBufStart);
    _arAesStruct[AES_MTD_INDEX].u4SrcBufEnd = GCPU_LINER_BUFFER_END(u4InBufStart + u4BufSize);
    _arAesStruct[AES_MTD_INDEX].u4DstStartAddr = u4OutBufStart;
    _arAesStruct[AES_MTD_INDEX].u4DstBufStart = GCPU_LINER_BUFFER_START(u4OutBufStart);
    _arAesStruct[AES_MTD_INDEX].u4DstBufEnd = GCPU_LINER_BUFFER_END(u4OutBufStart + u4BufSize);
    _arAesStruct[AES_MTD_INDEX].u4DatLen = u4BufSize;
    _arAesStruct[AES_MTD_INDEX].fgEncrypt = FALSE;

    return (GCPU_Cmd((UINT32)prGcpuMtd, GCPU_AES, &_arAesStruct[AES_MTD_INDEX]) == S_GCPU_OK)?TRUE:FALSE;
}

