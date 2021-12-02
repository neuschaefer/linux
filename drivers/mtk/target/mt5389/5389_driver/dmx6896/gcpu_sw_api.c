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
 * $RCSfile: dmx_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gcpu_api.c
 *  gcpu driver - main implementation
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "gcpu.h"
#include "aes.h"

#include "x_hal_926.h"
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

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

BOOL _GCPU_AES_SW_Ring(AES_PARAM_T *prLParam)
{
#ifndef CC_MTK_LOADER
    static UINT32 u4TmpBufSize = 0;
    static UINT32 u4TmpBufSrc = 0;
    static UINT32 u4TmpBufDst = 0;

    aes_context rCtx;
    UINT32 i;
    INT32 i4KenLen = 128;
    BOOL fgCbc = FALSE;
   	UINT8 au1InitVector[16];

    if(prLParam->eMode != BLOCK_CIPHER_MODE_ECB &&
        prLParam->eMode != BLOCK_CIPHER_MODE_CBC)
    {
        LOG(3, "This is a not support AES mode\n");
        return FALSE;
    }

    i4KenLen = prLParam->eKeyBitLen;
    fgCbc = (prLParam->eMode == BLOCK_CIPHER_MODE_ECB)?FALSE:TRUE;

    // realloc memory if need
    if(prLParam->u4DatLen >  u4TmpBufSize)
    {
        if(u4TmpBufSize != 0)
        {
            x_mem_free((void *)u4TmpBufSrc);
            x_mem_free((void *)u4TmpBufDst);
        }

        u4TmpBufSize = prLParam->u4DatLen;
        u4TmpBufSrc = (UINT32)x_mem_alloc(u4TmpBufSize);
        u4TmpBufDst = (UINT32)x_mem_alloc(u4TmpBufSize);
    }

    //LOG(2, "Use SW: 0x%x\n", prLParam->u4DatLen);

    UNUSED(_GCPU_CopyRingBuffer(u4TmpBufSrc, u4TmpBufSrc, u4TmpBufSrc + u4TmpBufSize,
                    prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd, prLParam->u4DatLen));

    if(!prLParam->fgEncrypt)
    {
        if (fgCbc)
        {
            x_memcpy(au1InitVector, prLParam->au1Iv, 16);
        }

        // Setup key scheduling
        aes_setkey_dec(&rCtx, prLParam->au1Key, i4KenLen);

        for (i = 0; i <= prLParam->u4DatLen - 16; i += 16)
        {
            // Encrypt
            if (fgCbc)
            {
                aes_crypt_cbc(&rCtx, AES_DECRYPT, 16, au1InitVector, (UINT8 *)(u4TmpBufSrc + i),
                    (UINT8 *)(u4TmpBufDst + i));
            }
            else
            {
                aes_crypt_ecb(&rCtx, AES_DECRYPT, (UINT8 *)(u4TmpBufSrc + i),
                    (UINT8 *)(u4TmpBufDst + i));
            }
        }

        // if there is residual data
        x_memcpy((UINT8 *)(u4TmpBufDst + i), (UINT8 *)(u4TmpBufSrc + i), prLParam->u4DatLen - i);
    }
    else// ecnrypt case
    {
        if (fgCbc)
        {
            x_memcpy(au1InitVector, prLParam->au1Iv, 16);
        }

        // Setup key scheduling
        aes_setkey_enc(&rCtx, prLParam->au1Key, i4KenLen);

        for (i = 0; i <= prLParam->u4DatLen - 16; i += 16)
        {
            // Encrypt
            if (fgCbc)
            {
                aes_crypt_cbc(&rCtx, AES_ENCRYPT, 16, au1InitVector, (UINT8 *)(u4TmpBufSrc + i),
                    (UINT8 *)(u4TmpBufDst + i));
            }
            else
            {
                aes_crypt_ecb(&rCtx, AES_ENCRYPT, (UINT8 *)(u4TmpBufSrc + i),
                    (UINT8 *)(u4TmpBufDst + i));
            }
        }

        // if there is residual data
        x_memcpy((UINT8 *)(u4TmpBufDst + i), (UINT8 *)(u4TmpBufSrc + i), prLParam->u4DatLen - i);
    }

    UNUSED(_GCPU_CopyRingBuffer(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
        prLParam->u4DstBufEnd, u4TmpBufDst, u4TmpBufDst,
        u4TmpBufDst + u4TmpBufSize, prLParam->u4DatLen));

    HalFlushInvalidateDCache();
#endif

    return TRUE;
}
