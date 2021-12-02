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

#ifdef CC_FCI_ENABLE
#ifdef CC_FCI_SDIO_SUPPORT

#include "fci_if.h"
#include "wlan_kal.h"
#include "basic.h"
#include "equates.h"
#include "mt5911hw.h"
#include "mt5911reg.h"

#include "sdio_func.h"

#ifdef CC_BD_FCI
#include "x_debug.h"
#endif

#define MAX_SD_RW_BYTES         512

INT32 SDIO_ReadMCR(UINT16 offset, UINT32 *pValue)
{
    INT32  i4Ret;

    if (SDCHW_IsEmpty())
    {
        LOG(1, "SDIO card is not inserted.\n");
        return E_FAIL;
    }

    i4Ret = SDIO_ReadBytes(1, offset, 4, (UINT8 *)pValue, SDIO_OP_INCADR);

    return i4Ret;
}

INT32 SDIO_WriteMCR(UINT16 offset, UINT32 value)
{
    INT32  i4Ret;

    if (SDCHW_IsEmpty())
    {
        LOG(1, "SDIO card is not inserted.\n");
        return E_FAIL;
    }

    i4Ret = SDIO_WriteBytes(1, offset, 4, (UINT8 *)&value, SDIO_OP_INCADR);

    return i4Ret;
}

INT32 SDIO_WritePort(IN UINT8 *pucBuf, IN UINT16 u2Len, IN UINT16 u2Port)
{
#if 0
    INT32  i4Ret = E_FAIL;
    
    ASSERT(pucBuf != NULL);

    if (SDCHW_IsEmpty())
    {
        LOG(1, "SDIO card is not inserted.\n");
        return E_FAIL;
    }
    
    //NOTICE: OP code = 0 or 1 are all OK.

    i4Ret = SDIO_WriteBytes(1, u2Port, u2Len, pucBuf, SDIO_OP_INCADR);

#else
    INT32  i4Ret = E_FAIL;
    UINT16 u2BlockNum = 0, u2ByteNum = 0;
    SDC_T   *prSDIO;
    
    ASSERT(pucBuf != NULL);

    if (SDCHW_IsEmpty())
    {
        LOG(1, "SDIO card is not inserted.\n");
        return E_FAIL;
    }

    prSDIO = FCI_GetSDCSlot();

    //UTIL_Printf("slot->write_len=%d, %d\n", prSlot->write_len, u2Len);
    // Check if we apply block mode to transfer data.
    if (prSDIO->u2BlkSz[1])
    {
        u2BlockNum = u2Len / prSDIO->u2BlkSz[1];
        u2ByteNum = u2Len % prSDIO->u2BlkSz[1];
    }
    else
    {
        u2BlockNum = 0;
        u2ByteNum = u2Len;
    }

    if (u2BlockNum > 0)
    {
        i4Ret = SDIO_WriteBlocks(1, u2Port, u2BlockNum, pucBuf, SDIO_OP_INCADR);
        pucBuf += (u2BlockNum * prSDIO->u2BlkSz[1]);
    }

    if (u2ByteNum > 0)
    {
        i4Ret = SDIO_WriteBytes(1, u2Port, u2ByteNum, pucBuf, SDIO_OP_INCADR);
    }

#endif

    return i4Ret;
}


INT32 SDIO_ReadPort(OUT UINT8 *pucBuf, IN UINT16 u2Len, IN UINT16 u2Port)
{
#if 0
    INT32  i4Ret = E_FAIL;
    
    //NOTICE: OP code = 0 or 1 are all OK.

    ASSERT(pucBuf != NULL);

    if (SDCHW_IsEmpty())
    {
        LOG(1, "SDIO card is not inserted.\n");
        return E_FAIL;
    }
    
    i4Ret = SDIO_ReadBytes(1, u2Port, u2Len, pucBuf, SDIO_OP_INCADR);

#else
    INT32  i4Ret = E_FAIL;
    UINT16 u2BlockNum = 0, u2ByteNum = 0;
    SDC_T   *prSDIO;

    ASSERT(pucBuf != NULL);

    if (SDCHW_IsEmpty())
    {
        LOG(1, "SDIO card is not inserted.\n");
        return E_FAIL;
    }
    
    prSDIO = FCI_GetSDCSlot();

    //UTIL_Printf("slot->write_len=%d, %d\n", prSlot->write_len, u2Len);
    // Check if we apply block mode to transfer data.
    if (prSDIO->u2BlkSz[1])
    {
        u2BlockNum = u2Len / prSDIO->u2BlkSz[1];
        u2ByteNum = u2Len % prSDIO->u2BlkSz[1];
    }
    else
    {
        u2BlockNum = 0;
        u2ByteNum = u2Len;
    }

    if (u2BlockNum > 0)
    {
        i4Ret = SDIO_ReadBlocks(1, u2Port, u2BlockNum, pucBuf, SDIO_OP_INCADR);
        pucBuf += (u2BlockNum * prSDIO->u2BlkSz[1]);
    }

    if (u2ByteNum > 0)
    {
        i4Ret = SDIO_ReadBytes(1, u2Port, u2ByteNum, pucBuf, SDIO_OP_INCADR);
    }

#endif

    return i4Ret;
}

//-------------------------------------------------------------------------
/** i4SdioDrvInit
 *  Register SDIO function pointer
 *  @param  prSdioDrvfnc  pointer to SDIO function structure.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------

INT32 i4SdioDrvInit(SDIO_DRV_IF_T *prSdioDrvfnc)
{
    INT32 i4Ret;

    ASSERT(prSdioDrvfnc != NULL);

    /* Init and enable SDIO bus interface */
    i4Ret = FCI_Init();

    if(i4Ret != FCI_TYPE_SDIO)
    {
        LOG(1,"FCI init fail\n");
        return E_FAIL;
    }

    /* register sdio driver callback function */
    prSdioDrvfnc->pf_sdio_init = (sdio_init_fnct) i4SdioDrvInit;
    prSdioDrvfnc->pf_sdio_deinit = (sdio_deinit_fnct) i4SdioDrvDeInit;
    prSdioDrvfnc->pf_sdio_r_bytes = (sdio_rb_fnct) SDIO_ReadMCR;
    prSdioDrvfnc->pf_sdio_w_bytes = (sdio_wb_fnct) SDIO_WriteMCR;
    prSdioDrvfnc->pf_sdio_r_port = (sdio_rp_fnct) SDIO_ReadPort;
    prSdioDrvfnc->pf_sdio_w_port = (sdio_wp_fnct) SDIO_WritePort;

    // Enable SDIO function 1 & register call back function
    return SDIO_EnableIO(1, (FCI_CALLBACK) prSdioDrvfnc->pf_irq_callback);
}

//-------------------------------------------------------------------------
/** i4SdioDrvDeInit
 *  Register SDIO function pointer
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------

INT32 i4SdioDrvDeInit(VOID)
{
    SDC_T *prSDC;
    INT32  i4Ret;

    // Remove Card
    prSDC = FCI_GetSDCSlot();
    prSDC->u4CardType = FCI_UNKNOWN_CARD;

    // De-Init FCI isr & semaphore
    i4Ret = FCI_RegDeInit();

    return i4Ret;
}

#endif // #ifdef CC_FCI_SDIO_SUPPORT
#endif  // #ifdef CC_FCI_ENABLE
